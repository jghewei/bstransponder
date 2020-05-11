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
 *     and register accessor functions for the ohfs_insert block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing ohfs_insert io functions is:
 | {
 |     ohfs_insert_buffer_t b_ptr[1];
 |     ohfs_insert_buffer_init( b_ptr, h_ptr );
 |     value = ohfs_insert_field_<FIELD>_get( b_ptr, h_ptr );
 |     ohfs_insert_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     ohfs_insert_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = ohfs_insert_field_<FIELD>_get( NULL, h_ptr );
 | ohfs_insert_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000020 + (N) * 0x04) CHANNEL_RP_NCS_CONFIG |
 | |     N[0..95]                                             |
 | +----------------------------------------------------------+
 | | bit  1 R/W  NCS_EN                                       |
 | | bit  0 R/W  RP                                           |
 | +----------------------------------------------------------+
 | 
 | void   ohfs_insert_field_NCS_EN_set ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_NCS_EN_get ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_RP_set     ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_RP_get     ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000200 + (N) * 0x20) CHANNEL_COMMON_CONFIG |
 | |     N[0..95]                                             |
 | +----------------------------------------------------------+
 | | bit  26    R/W  DBG_UNDERRUN_INT_SRC                     |
 | | bit  25    R/W  DBG_MASK_SOMF_SYNC                       |
 | | bit  24    R/W  EXT_PAYLOAD_TYPE                         |
 | | bit  23:16 R/W  PT                                       |
 | | bit  15:10 R/W  PAD_CNT                                  |
 | | bit  9:3   R/W  MAX_SLOT_NUM                             |
 | | bit  2     R/W  OMFI_INSERT                              |
 | | bit  0     R/W  NULL_CHAN_EN                             |
 | +----------------------------------------------------------+
 | 
 | void   ohfs_insert_field_DBG_UNDERRUN_INT_SRC_set ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_DBG_UNDERRUN_INT_SRC_get ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_DBG_MASK_SOMF_SYNC_set   ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_DBG_MASK_SOMF_SYNC_get   ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_EXT_PAYLOAD_TYPE_set     ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_EXT_PAYLOAD_TYPE_get     ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_PT_set                   ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_PT_get                   ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_range_PT_set             ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_insert_field_range_PT_get             ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_insert_field_PAD_CNT_set              ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_PAD_CNT_get              ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_range_PAD_CNT_set        ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_insert_field_range_PAD_CNT_get        ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_insert_field_MAX_SLOT_NUM_set         ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_MAX_SLOT_NUM_get         ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_range_MAX_SLOT_NUM_set   ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_insert_field_range_MAX_SLOT_NUM_get   ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_insert_field_OMFI_INSERT_set          ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_OMFI_INSERT_get          ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_NULL_CHAN_EN_set         ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_NULL_CHAN_EN_get         ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000204 + (N) * 0x20) FIXED_STUFF_CONFIG |
 | |     N[0..95]                                          |
 | +-------------------------------------------------------+
 | | bit  26:15 R/W  STUFF_COL_POS_2                       |
 | | bit  14:3  R/W  STUFF_COL_POS_1                       |
 | | bit  2     R/W  STUFF_POS_SEL                         |
 | | bit  1     R/W  STUFF_MODE                            |
 | | bit  0     R/W  STUFF_EN                              |
 | +-------------------------------------------------------+
 | 
 | void   ohfs_insert_field_STUFF_COL_POS_2_set       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_STUFF_COL_POS_2_get       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_range_STUFF_COL_POS_2_set ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_insert_field_range_STUFF_COL_POS_2_get ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_insert_field_STUFF_COL_POS_1_set       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_STUFF_COL_POS_1_get       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_range_STUFF_COL_POS_1_set ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_insert_field_range_STUFF_COL_POS_1_get ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_insert_field_STUFF_POS_SEL_set         ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_STUFF_POS_SEL_get         ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_STUFF_MODE_set            ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_STUFF_MODE_get            ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_STUFF_EN_set              ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_STUFF_EN_get              ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000208 + (N) * 0x20) RESIZE_OVERHEAD_CFG |
 | |     N[0..95]                                           |
 | +--------------------------------------------------------+
 | | bit  23:0 R/W  RCOH                                    |
 | +--------------------------------------------------------+
 | 
 | void   ohfs_insert_field_RCOH_set       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_RCOH_get       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_range_RCOH_set ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_insert_field_range_RCOH_get ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x0000020c + (N) * 0x20) XOFF_WM_CFG |
 | |     N[0..95]                                   |
 | +------------------------------------------------+
 | | bit  7:0 R/W  XOFF_WM_LEVEL                    |
 | +------------------------------------------------+
 | 
 | void   ohfs_insert_field_XOFF_WM_LEVEL_set       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_XOFF_WM_LEVEL_get       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_range_XOFF_WM_LEVEL_set ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_insert_field_range_XOFF_WM_LEVEL_get ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00000210 + (N) * 0x20) JC_RD_IPT_ENTRY |
 | |     N[0..95]                                       |
 | +----------------------------------------------------+
 | | bit  16   R/W  JC_RD_IPT_VALID                     |
 | | bit  14:8 R/W  JC_RD_IPT_ID                        |
 | | bit  6:0  R/W  JC_RD_IPT_VA                        |
 | +----------------------------------------------------+
 | 
 | void   ohfs_insert_field_JC_RD_IPT_VALID_set    ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_JC_RD_IPT_VALID_get    ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_JC_RD_IPT_ID_set       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_JC_RD_IPT_ID_get       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_range_JC_RD_IPT_ID_set ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_insert_field_range_JC_RD_IPT_ID_get ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_insert_field_JC_RD_IPT_VA_set       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_JC_RD_IPT_VA_get       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_range_JC_RD_IPT_VA_set ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_insert_field_range_JC_RD_IPT_VA_get ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00000214 + (N) * 0x20) JC_WR_IPT_ENTRY |
 | |     N[0..95]                                       |
 | +----------------------------------------------------+
 | | bit  16   R/W  JC_WR_IPT_VALID                     |
 | | bit  14:8 R/W  JC_WR_IPT_ID                        |
 | | bit  6:0  R/W  JC_WR_IPT_VA                        |
 | +----------------------------------------------------+
 | 
 | void   ohfs_insert_field_JC_WR_IPT_VALID_set    ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_JC_WR_IPT_VALID_get    ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_JC_WR_IPT_ID_set       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_JC_WR_IPT_ID_get       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_range_JC_WR_IPT_ID_set ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_insert_field_range_JC_WR_IPT_ID_get ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_insert_field_JC_WR_IPT_VA_set       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_JC_WR_IPT_VA_get       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_range_JC_WR_IPT_VA_set ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_insert_field_range_JC_WR_IPT_VA_get ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x00000218 + (N) * 0x20) CHANNEL_CSF_INSERT_CONFIG |
 | |     N[0..95]                                                 |
 | +--------------------------------------------------------------+
 | | bit  0 R/W  CSF_INSERT                                       |
 | +--------------------------------------------------------------+
 | 
 | void   ohfs_insert_field_CSF_INSERT_set ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_CSF_INSERT_get ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------------+
 | | Register (0x0000021c + (N) * 0x20) LO_CFG_REG |
 | |     N[0..95]                                  |
 | +-----------------------------------------------+
 | | bit  23:16 R/W  MSI                           |
 | | bit  12    R/W  TS_LAST                       |
 | | bit  11    R/W  TS_VALID                      |
 | | bit  10:8  R/W  MAP_TYPE                      |
 | | bit  6:0   R/W  LO_CHID                       |
 | +-----------------------------------------------+
 | 
 | void   ohfs_insert_field_MSI_set            ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_MSI_get            ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_range_MSI_set      ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_insert_field_range_MSI_get      ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_insert_field_TS_LAST_set        ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_TS_LAST_get        ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_TS_VALID_set       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_TS_VALID_get       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_MAP_TYPE_set       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_MAP_TYPE_get       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_range_MAP_TYPE_set ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_insert_field_range_MAP_TYPE_get ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_insert_field_LO_CHID_set        ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_insert_field_LO_CHID_get        ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | void   ohfs_insert_field_range_LO_CHID_set  ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_insert_field_range_LO_CHID_get  ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _OHFS_INSERT_IO_H
#define _OHFS_INSERT_IO_H

#include "ohfs_insert_loc.h"
#include "ohfs_insert_regs.h"
#include "ohfs_insert_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 ohfs_insert_buffer_init               ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr )
 | void                 ohfs_insert_buffer_flush              ( ohfs_insert_buffer_t *b_ptr )
 | UINT32               ohfs_insert_reg_read                  ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 ohfs_insert_reg_write                 ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 ohfs_insert_field_set                 ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 ohfs_insert_action_on_write_field_set ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 ohfs_insert_burst_read                ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 ohfs_insert_burst_write               ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE ohfs_insert_poll                      ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_write         ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_field_set     ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_insert_reg_CHANNEL_RP_NCS_CONFIG_array_read          ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | 
 | void   ohfs_insert_reg_JC_RD_IPT_ENTRY_array_write               ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   ohfs_insert_reg_JC_RD_IPT_ENTRY_array_field_set           ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_insert_reg_JC_RD_IPT_ENTRY_array_read                ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | 
 | void   ohfs_insert_reg_JC_WR_IPT_ENTRY_array_write               ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   ohfs_insert_reg_JC_WR_IPT_ENTRY_array_field_set           ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_insert_reg_JC_WR_IPT_ENTRY_array_read                ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | 
 | void   ohfs_insert_reg_CHANNEL_CSF_INSERT_CONFIG_array_write     ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   ohfs_insert_reg_CHANNEL_CSF_INSERT_CONFIG_array_field_set ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_insert_reg_CHANNEL_CSF_INSERT_CONFIG_array_read      ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | 
 | void   ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_write         ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_field_set     ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_insert_reg_CHANNEL_COMMON_CONFIG_array_read          ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | 
 | void   ohfs_insert_reg_FIXED_STUFF_CONFIG_array_write            ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   ohfs_insert_reg_FIXED_STUFF_CONFIG_array_field_set        ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_insert_reg_FIXED_STUFF_CONFIG_array_read             ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | 
 | void   ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_write           ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_field_set       ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_insert_reg_RESIZE_OVERHEAD_CFG_array_read            ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | 
 | void   ohfs_insert_reg_XOFF_WM_CFG_array_write                   ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   ohfs_insert_reg_XOFF_WM_CFG_array_field_set               ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_insert_reg_XOFF_WM_CFG_array_read                    ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | 
 | void   ohfs_insert_reg_LO_CFG_REG_array_write                    ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   ohfs_insert_reg_LO_CFG_REG_array_field_set                ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_insert_reg_LO_CFG_REG_array_read                     ( ohfs_insert_buffer_t *b_ptr, ohfs_insert_handle_t *h_ptr, UINT32  N )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _OHFS_INSERT_IO_H */
