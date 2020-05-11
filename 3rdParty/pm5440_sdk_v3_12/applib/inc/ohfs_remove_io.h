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
 *     and register accessor functions for the ohfs_remove block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing ohfs_remove io functions is:
 | {
 |     ohfs_remove_buffer_t b_ptr[1];
 |     ohfs_remove_buffer_init( b_ptr, h_ptr );
 |     value = ohfs_remove_field_<FIELD>_get( b_ptr, h_ptr );
 |     ohfs_remove_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     ohfs_remove_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = ohfs_remove_field_<FIELD>_get( NULL, h_ptr );
 | ohfs_remove_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +------------------------------------------------+
 | | Register 0x00000000 PT_STAT_UPDATE_DISABLE_CFG |
 | +------------------------------------------------+
 | | bit  0 R/W  PT_STAT_UPDATE_DISABLE             |
 | +------------------------------------------------+
 | 
 | void   ohfs_remove_field_PT_STAT_UPDATE_DISABLE_set ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 value )
 | UINT32 ohfs_remove_field_PT_STAT_UPDATE_DISABLE_get ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr )
 | 
 | +-------------------------------------------------+
 | | Register 0x00000004 MSI_STAT_UPDATE_DISABLE_CFG |
 | +-------------------------------------------------+
 | | bit  0 R/W  MSI_STAT_UPDATE_DISABLE             |
 | +-------------------------------------------------+
 | 
 | void   ohfs_remove_field_MSI_STAT_UPDATE_DISABLE_set ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 value )
 | UINT32 ohfs_remove_field_MSI_STAT_UPDATE_DISABLE_get ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x00000008 RCOH_STAT_LATCH_CFG |
 | +-----------------------------------------+
 | | bit  0 R/W  TRNSFR_RCOH_TO_SHADOW_EN    |
 | +-----------------------------------------+
 | 
 | void   ohfs_remove_field_TRNSFR_RCOH_TO_SHADOW_EN_set ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 value )
 | UINT32 ohfs_remove_field_TRNSFR_RCOH_TO_SHADOW_EN_get ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register (0x00000400 + (N) * 0x8) COMMON_CFG |
 | |     N[0..95]                                 |
 | +----------------------------------------------+
 | | bit  31:24 R/W  PT                           |
 | | bit  21:20 R/W  MSI_ACCEPTANCE_CNT           |
 | | bit  17:16 R/W  PT_ACCEPTANCE_CNT            |
 | | bit  14:12 R/W  TS_NUM_TYPE                  |
 | | bit  10    R/W  MAP_DLOOMFI_TO_SSF           |
 | | bit  9     R/W  HO_RCOH_EN                   |
 | | bit  8     R/W  FLEX_RCOH_EN                 |
 | | bit  7     R/W  MAP_DCSF_TO_SSF              |
 | | bit  6     R/W  MAP_DPLM_TO_SSF              |
 | | bit  5     R/W  MAP_SF_TO_SSF                |
 | | bit  4     R/W  CSF_CHK_EN                   |
 | | bit  3     R/W  MSI_CHK_EN                   |
 | | bit  2     R/W  PT_CHK_EN                    |
 | | bit  1     R/W  OPUKHDR_DATA                 |
 | | bit  0     R/W  NULL_FRAME_MODE              |
 | +----------------------------------------------+
 | 
 | void   ohfs_remove_field_PT_set                       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_PT_get                       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_range_PT_set                 ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_remove_field_range_PT_get                 ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_remove_field_MSI_ACCEPTANCE_CNT_set       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_MSI_ACCEPTANCE_CNT_get       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_range_MSI_ACCEPTANCE_CNT_set ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_remove_field_range_MSI_ACCEPTANCE_CNT_get ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_remove_field_PT_ACCEPTANCE_CNT_set        ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_PT_ACCEPTANCE_CNT_get        ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_range_PT_ACCEPTANCE_CNT_set  ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_remove_field_range_PT_ACCEPTANCE_CNT_get  ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_remove_field_TS_NUM_TYPE_set              ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_TS_NUM_TYPE_get              ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_range_TS_NUM_TYPE_set        ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_remove_field_range_TS_NUM_TYPE_get        ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_remove_field_MAP_DLOOMFI_TO_SSF_set       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_MAP_DLOOMFI_TO_SSF_get       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_HO_RCOH_EN_set               ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_HO_RCOH_EN_get               ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_FLEX_RCOH_EN_set             ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_FLEX_RCOH_EN_get             ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_MAP_DCSF_TO_SSF_set          ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_MAP_DCSF_TO_SSF_get          ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_MAP_DPLM_TO_SSF_set          ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_MAP_DPLM_TO_SSF_get          ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_MAP_SF_TO_SSF_set            ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_MAP_SF_TO_SSF_get            ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_CSF_CHK_EN_set               ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_CSF_CHK_EN_get               ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_MSI_CHK_EN_set               ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_MSI_CHK_EN_get               ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_PT_CHK_EN_set                ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_PT_CHK_EN_get                ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_OPUKHDR_DATA_set             ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_OPUKHDR_DATA_get             ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_NULL_FRAME_MODE_set          ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_NULL_FRAME_MODE_get          ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------------------------------------+
 | | Register (0x00000404 + (N) * 0x8) FIXED_STUFF_REMOVAL_CONFIGUTRATION |
 | |     N[0..95]                                                         |
 | +----------------------------------------------------------------------+
 | | bit  31:20 R/W  FS2_ADDR                                             |
 | | bit  17    R/W  FS2_16_8                                             |
 | | bit  16    R/W  FS2_EN                                               |
 | | bit  15:4  R/W  FS1_ADDR                                             |
 | | bit  1     R/W  FS1_16_8                                             |
 | | bit  0     R/W  FS1_EN                                               |
 | +----------------------------------------------------------------------+
 | 
 | void   ohfs_remove_field_FS2_ADDR_set       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_FS2_ADDR_get       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_range_FS2_ADDR_set ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_remove_field_range_FS2_ADDR_get ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_remove_field_FS2_16_8_set       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_FS2_16_8_get       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_FS2_EN_set         ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_FS2_EN_get         ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_FS1_ADDR_set       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_FS1_ADDR_get       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_range_FS1_ADDR_set ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_remove_field_range_FS1_ADDR_get ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_remove_field_FS1_16_8_set       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_FS1_16_8_get       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_FS1_EN_set         ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_FS1_EN_get         ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------------+
 | | Register (0x00000800 + (N) * 0x10) TS_ADDRESS_IPT |
 | |     N[0..95]                                      |
 | +---------------------------------------------------+
 | | bit  22:16 R/W  TS_IPT_FRM_NUM                    |
 | | bit  14:8  R/W  TS_IPT_ID                         |
 | | bit  0     R/W  TS_IPT_VALID                      |
 | +---------------------------------------------------+
 | 
 | void   ohfs_remove_field_TS_IPT_FRM_NUM_set       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_TS_IPT_FRM_NUM_get       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_range_TS_IPT_FRM_NUM_set ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_remove_field_range_TS_IPT_FRM_NUM_get ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_remove_field_TS_IPT_ID_set            ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_TS_IPT_ID_get            ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_range_TS_IPT_ID_set      ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_remove_field_range_TS_IPT_ID_get      ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_remove_field_TS_IPT_VALID_set         ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_TS_IPT_VALID_get         ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000804 + (N) * 0x10) TRIB_SLOT_BASED_CFG |
 | |     N[0..95]                                           |
 | +--------------------------------------------------------+
 | | bit  10:8 R/W  CHAN_MAP_TYPE                           |
 | | bit  2    R/W  LAST_TS                                 |
 | | bit  1    R/W  FIRST_TS                                |
 | | bit  0    R/W  TS_VALID                                |
 | +--------------------------------------------------------+
 | 
 | void   ohfs_remove_field_CHAN_MAP_TYPE_set       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_CHAN_MAP_TYPE_get       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_range_CHAN_MAP_TYPE_set ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_remove_field_range_CHAN_MAP_TYPE_get ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_remove_field_LAST_TS_set             ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_LAST_TS_get             ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_FIRST_TS_set            ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_FIRST_TS_get            ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_TS_VALID_set            ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_TS_VALID_get            ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------+
 | | Register (0x00000808 + (N) * 0x10) LO_ID_MSI_CFG |
 | |     N[0..95]                                     |
 | +--------------------------------------------------+
 | | bit  14:8 R/W  LO_ID                             |
 | | bit  7:0  R/W  MSI                               |
 | +--------------------------------------------------+
 | 
 | void   ohfs_remove_field_LO_ID_set       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_LO_ID_get       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_range_LO_ID_set ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_remove_field_range_LO_ID_get ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ohfs_remove_field_MSI_set         ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ohfs_remove_field_MSI_get         ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | void   ohfs_remove_field_range_MSI_set   ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ohfs_remove_field_range_MSI_get   ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00001000 + (N) * 0x4) PT_STAT_REG |
 | |     N[0..95]                                  |
 | +-----------------------------------------------+
 | | bit  7:0 R  PT_STATUS                         |
 | +-----------------------------------------------+
 | 
 | UINT32 ohfs_remove_field_PT_STATUS_get       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | UINT32 ohfs_remove_field_range_PT_STATUS_get ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00001200 + (N) * 0x4) MSI_STAT_REG |
 | |     N[0..95]                                   |
 | +------------------------------------------------+
 | | bit  7:0 R  MSI_STATUS                         |
 | +------------------------------------------------+
 | 
 | UINT32 ohfs_remove_field_MSI_STATUS_get       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | UINT32 ohfs_remove_field_range_MSI_STATUS_get ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00001400 + (N) * 0x4) RCOH_STAT_REG |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  15:0 R  RCOH_STATUS                        |
 | +-------------------------------------------------+
 | 
 | UINT32 ohfs_remove_field_RCOH_STATUS_get       ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | UINT32 ohfs_remove_field_range_RCOH_STATUS_get ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _OHFS_REMOVE_IO_H
#define _OHFS_REMOVE_IO_H

#include "ohfs_remove.h"
#include "ohfs_remove_regs.h"
#include "ohfs_remove_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 ohfs_remove_buffer_init               ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr )
 | void                 ohfs_remove_buffer_flush              ( ohfs_remove_buffer_t *b_ptr )
 | UINT32               ohfs_remove_reg_read                  ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 ohfs_remove_reg_write                 ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 ohfs_remove_field_set                 ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 ohfs_remove_action_on_write_field_set ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 ohfs_remove_burst_read                ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 ohfs_remove_burst_write               ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE ohfs_remove_poll                      ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   ohfs_remove_reg_PT_STAT_UPDATE_DISABLE_CFG_write                   ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 value )
 | void   ohfs_remove_reg_PT_STAT_UPDATE_DISABLE_CFG_field_set               ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_remove_reg_PT_STAT_UPDATE_DISABLE_CFG_read                    ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr )
 | 
 | void   ohfs_remove_reg_MSI_STAT_UPDATE_DISABLE_CFG_write                  ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 value )
 | void   ohfs_remove_reg_MSI_STAT_UPDATE_DISABLE_CFG_field_set              ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_remove_reg_MSI_STAT_UPDATE_DISABLE_CFG_read                   ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr )
 | 
 | void   ohfs_remove_reg_TS_ADDRESS_IPT_array_write                         ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   ohfs_remove_reg_TS_ADDRESS_IPT_array_field_set                     ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_remove_reg_TS_ADDRESS_IPT_array_read                          ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | 
 | void   ohfs_remove_reg_RCOH_STAT_LATCH_CFG_write                          ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 value )
 | void   ohfs_remove_reg_RCOH_STAT_LATCH_CFG_field_set                      ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_remove_reg_RCOH_STAT_LATCH_CFG_read                           ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr )
 | 
 | void   ohfs_remove_reg_COMMON_CFG_array_write                             ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   ohfs_remove_reg_COMMON_CFG_array_field_set                         ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_remove_reg_COMMON_CFG_array_read                              ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | 
 | void   ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_write     ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_field_set ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_remove_reg_FIXED_STUFF_REMOVAL_CONFIGUTRATION_array_read      ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | 
 | void   ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_write                    ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_field_set                ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_remove_reg_TRIB_SLOT_BASED_CFG_array_read                     ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | 
 | void   ohfs_remove_reg_LO_ID_MSI_CFG_array_write                          ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   ohfs_remove_reg_LO_ID_MSI_CFG_array_field_set                      ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ohfs_remove_reg_LO_ID_MSI_CFG_array_read                           ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32 ohfs_remove_reg_PT_STAT_REG_array_read                             ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32 ohfs_remove_reg_MSI_STAT_REG_array_read                            ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32 ohfs_remove_reg_RCOH_STAT_REG_array_read                           ( ohfs_remove_buffer_t *b_ptr, ohfs_remove_handle_t *h_ptr, UINT32  N )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _OHFS_REMOVE_IO_H */
