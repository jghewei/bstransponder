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
 *     and register accessor functions for the fecstat block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing fecstat io functions is:
 | {
 |     fecstat_buffer_t b_ptr[1];
 |     fecstat_buffer_init( b_ptr, h_ptr );
 |     value = fecstat_field_<FIELD>_get( b_ptr, h_ptr );
 |     fecstat_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     fecstat_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = fecstat_field_<FIELD>_get( NULL, h_ptr );
 | fecstat_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------+
 | | Register 0x00006000 FECSTAT_EN |
 | +--------------------------------+
 | | bit  0 R/W  FECSTAT_EN         |
 | +--------------------------------+
 | 
 | void   fecstat_field_FECSTAT_EN_set ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 fecstat_field_FECSTAT_EN_get ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +---------------------------------------------------+
 | | Register (0x00006004 + (N) * 0x20) STAT_SOURCE_EN |
 | |     N[0..47]                                      |
 | +---------------------------------------------------+
 | | bit  2:0 R/W  STAT_SOURCE_EN                      |
 | +---------------------------------------------------+
 | 
 | void   fecstat_field_STAT_SOURCE_EN_set       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 fecstat_field_STAT_SOURCE_EN_get       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   fecstat_field_range_STAT_SOURCE_EN_set ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fecstat_field_range_STAT_SOURCE_EN_get ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00006008 + (N) * 0x20) STAT_SOURCE_0_ADR |
 | |     N[0..47]                                         |
 | +------------------------------------------------------+
 | | bit  4:0 R/W  STAT_SOURCE0_ADDR                      |
 | +------------------------------------------------------+
 | 
 | void   fecstat_field_STAT_SOURCE0_ADDR_set       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 fecstat_field_STAT_SOURCE0_ADDR_get       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   fecstat_field_range_STAT_SOURCE0_ADDR_set ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fecstat_field_range_STAT_SOURCE0_ADDR_get ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x0000600c + (N) * 0x20) STAT_SOURCE_1_ADR |
 | |     N[0..47]                                         |
 | +------------------------------------------------------+
 | | bit  4:0 R/W  STAT_SOURCE1_ADDR                      |
 | +------------------------------------------------------+
 | 
 | void   fecstat_field_STAT_SOURCE1_ADDR_set       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 fecstat_field_STAT_SOURCE1_ADDR_get       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   fecstat_field_range_STAT_SOURCE1_ADDR_set ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fecstat_field_range_STAT_SOURCE1_ADDR_get ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00006010 + (N) * 0x20) STAT_SOURCE_2_ADR |
 | |     N[0..47]                                         |
 | +------------------------------------------------------+
 | | bit  4:0 R/W  STAT_SOURCE2_ADDR                      |
 | +------------------------------------------------------+
 | 
 | void   fecstat_field_STAT_SOURCE2_ADDR_set       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 fecstat_field_STAT_SOURCE2_ADDR_get       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   fecstat_field_range_STAT_SOURCE2_ADDR_set ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fecstat_field_range_STAT_SOURCE2_ADDR_get ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00006014 + (N) * 0x20) LOGICAL_LANE_ID |
 | |     N[0..47]                                       |
 | +----------------------------------------------------+
 | | bit  7:0 R/W  LLID                                 |
 | +----------------------------------------------------+
 | 
 | void   fecstat_field_LLID_set       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 fecstat_field_LLID_get       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   fecstat_field_range_LLID_set ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fecstat_field_range_LLID_get ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00006018 + (N) * 0x20) PHYSICAL_LANE_INDEX |
 | |     N[0..47]                                           |
 | +--------------------------------------------------------+
 | | bit  7:0 R/W  PHY_LN                                   |
 | +--------------------------------------------------------+
 | 
 | void   fecstat_field_PHY_LN_set       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 fecstat_field_PHY_LN_get       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   fecstat_field_range_PHY_LN_set ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fecstat_field_range_PHY_LN_get ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00006604 FWI_CLK_DIVIDER |
 | +-------------------------------------+
 | | bit  4 R/W  FWI_INV_GL_CLK          |
 | +-------------------------------------+
 | 
 | void   fecstat_field_FWI_INV_GL_CLK_set ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 fecstat_field_FWI_INV_GL_CLK_get ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +-----------------------------------------------+
 | | Register 0x00006608 FEC_COLLECTION_PERIOD_CNT |
 | +-----------------------------------------------+
 | | bit  27:0 R/W  FEC_COLLCT_CNT                 |
 | +-----------------------------------------------+
 | 
 | void   fecstat_field_FEC_COLLCT_CNT_set       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 fecstat_field_FEC_COLLCT_CNT_get       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   fecstat_field_range_FEC_COLLCT_CNT_set ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fecstat_field_range_FEC_COLLCT_CNT_get ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x0000660c FEC_ROW_EN_LOWER |
 | +--------------------------------------+
 | | bit  31:0 R/W  FEC_ROW_EN            |
 | +--------------------------------------+
 | 
 | void   fecstat_lfield_FEC_ROW_EN_set       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value[2] )
 | void   fecstat_lfield_FEC_ROW_EN_get       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value[2] )
 | void   fecstat_lfield_range_FEC_ROW_EN_set ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fecstat_lfield_range_FEC_ROW_EN_get ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00006614 AIR_ERR_INT_EN |
 | +------------------------------------+
 | | bit  2:0 R/W  AIR_ERR_INT_E        |
 | +------------------------------------+
 | 
 | void   fecstat_field_AIR_ERR_INT_E_set       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 fecstat_field_AIR_ERR_INT_E_get       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   fecstat_field_range_AIR_ERR_INT_E_set ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fecstat_field_range_AIR_ERR_INT_E_get ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00006618 AIR_ERR_INT_REQUEST |
 | +-----------------------------------------+
 | | bit  2:0 R/W  AIR_ERR_INT_I             |
 | +-----------------------------------------+
 | 
 | void                 fecstat_field_AIR_ERR_INT_I_set_to_clear       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               fecstat_field_AIR_ERR_INT_I_get                ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 fecstat_field_range_AIR_ERR_INT_I_set_to_clear ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               fecstat_field_range_AIR_ERR_INT_I_get          ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE fecstat_field_range_AIR_ERR_INT_I_poll         ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE fecstat_field_AIR_ERR_INT_I_poll               ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x0000661c FEC_READ_ERR_VECTOR |
 | +-----------------------------------------+
 | | bit  15:0 R  FEC_RD_ERR_VECTOR          |
 | +-----------------------------------------+
 | 
 | UINT32 fecstat_field_FEC_RD_ERR_VECTOR_get       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32 fecstat_field_range_FEC_RD_ERR_VECTOR_get ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00006620 FWI_WRITE_ERR_VECTOR |
 | +------------------------------------------+
 | | bit  15:0 R  FWI_WR_ERR_VECTOR           |
 | +------------------------------------------+
 | 
 | UINT32 fecstat_field_FWI_WR_ERR_VECTOR_get       ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32 fecstat_field_range_FWI_WR_ERR_VECTOR_get ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _FECSTAT_IO_H
#define _FECSTAT_IO_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "fecstat_regs.h"
#include "fecstat_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fecstat_buffer_init               ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 fecstat_buffer_flush              ( fecstat_buffer_t *b_ptr )
 | UINT32               fecstat_reg_read                  ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 fecstat_reg_write                 ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 fecstat_field_set                 ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 fecstat_action_on_write_field_set ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 fecstat_burst_read                ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 fecstat_burst_write               ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE fecstat_poll                      ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fecstat_reg_FECSTAT_EN_write                              ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 fecstat_reg_FECSTAT_EN_field_set                          ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fecstat_reg_FECSTAT_EN_read                               ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 fecstat_reg_FWI_CLK_DIVIDER_write                         ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 fecstat_reg_FWI_CLK_DIVIDER_field_set                     ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fecstat_reg_FWI_CLK_DIVIDER_read                          ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 fecstat_reg_FEC_COLLECTION_PERIOD_CNT_write               ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 fecstat_reg_FEC_COLLECTION_PERIOD_CNT_field_set           ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fecstat_reg_FEC_COLLECTION_PERIOD_CNT_read                ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 fecstat_reg_FEC_ROW_EN_LOWER_write                        ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 fecstat_reg_FEC_ROW_EN_LOWER_field_set                    ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fecstat_reg_FEC_ROW_EN_LOWER_read                         ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 fecstat_reg_FEC_ROW_EN_UPPER_write                        ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 fecstat_reg_FEC_ROW_EN_UPPER_field_set                    ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fecstat_reg_FEC_ROW_EN_UPPER_read                         ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 fecstat_reg_STAT_SOURCE_EN_array_write                    ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 fecstat_reg_STAT_SOURCE_EN_array_field_set                ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fecstat_reg_STAT_SOURCE_EN_array_read                     ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 fecstat_reg_STAT_SOURCE_0_ADR_array_write                 ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 fecstat_reg_STAT_SOURCE_0_ADR_array_field_set             ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fecstat_reg_STAT_SOURCE_0_ADR_array_read                  ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 fecstat_reg_STAT_SOURCE_1_ADR_array_write                 ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 fecstat_reg_STAT_SOURCE_1_ADR_array_field_set             ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fecstat_reg_STAT_SOURCE_1_ADR_array_read                  ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 fecstat_reg_STAT_SOURCE_2_ADR_array_write                 ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 fecstat_reg_STAT_SOURCE_2_ADR_array_field_set             ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fecstat_reg_STAT_SOURCE_2_ADR_array_read                  ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 fecstat_reg_LOGICAL_LANE_ID_array_write                   ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 fecstat_reg_LOGICAL_LANE_ID_array_field_set               ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fecstat_reg_LOGICAL_LANE_ID_array_read                    ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 fecstat_reg_PHYSICAL_LANE_INDEX_array_write               ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 fecstat_reg_PHYSICAL_LANE_INDEX_array_field_set           ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fecstat_reg_PHYSICAL_LANE_INDEX_array_read                ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 fecstat_reg_AIR_ERR_INT_EN_write                          ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 fecstat_reg_AIR_ERR_INT_EN_field_set                      ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fecstat_reg_AIR_ERR_INT_EN_read                           ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 fecstat_reg_AIR_ERR_INT_REQUEST_write                     ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 fecstat_reg_AIR_ERR_INT_REQUEST_action_on_write_field_set ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fecstat_reg_AIR_ERR_INT_REQUEST_read                      ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE fecstat_reg_AIR_ERR_INT_REQUEST_poll                      ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               fecstat_reg_FEC_READ_ERR_VECTOR_read                      ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | UINT32               fecstat_reg_FWI_WRITE_ERR_VECTOR_read                     ( fecstat_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _FECSTAT_IO_H */
