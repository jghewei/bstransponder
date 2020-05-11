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
 *     and register accessor functions for the rmac block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing rmac io functions is:
 | {
 |     rmac_buffer_t b_ptr[1];
 |     rmac_buffer_init( b_ptr, h_ptr );
 |     value = rmac_field_<FIELD>_get( b_ptr, h_ptr );
 |     rmac_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     rmac_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = rmac_field_<FIELD>_get( NULL, h_ptr );
 | rmac_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------+
 | | Register 0x00000100 CFG_1   |
 | +-----------------------------+
 | | bit  15    R    RMAC_STATUS |
 | | bit  14    R/W  PAMB_CK     |
 | | bit  13    R/W  TAG_AWARE   |
 | | bit  12    R/W  INRANGE_DIS |
 | | bit  11:10 R/W  INRANGE_CK  |
 | | bit  9     R/W  MAX_FRM_TRN |
 | | bit  8     R/W  MAX_FRM_DIS |
 | | bit  7     R/W  MAX_FRM_CK  |
 | | bit  6     R/W  MIN_FRM_DIS |
 | | bit  5     R/W  MIN_FRM_CK  |
 | | bit  4     R/W  CRC_DIS     |
 | | bit  3     R/W  CRC_CK      |
 | | bit  2     R/W  TRANSPARENT |
 | | bit  1     R/W  ETHER_MODE  |
 | | bit  0     R/W  RMAC_ENBL   |
 | +-----------------------------+
 | 
 | void   rmac_field_PAMB_CK_set          ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_PAMB_CK_get          ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_TAG_AWARE_set        ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_TAG_AWARE_get        ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_INRANGE_DIS_set      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_INRANGE_DIS_get      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_INRANGE_CK_set       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_INRANGE_CK_get       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_range_INRANGE_CK_set ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rmac_field_range_INRANGE_CK_get ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   rmac_field_MAX_FRM_TRN_set      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_MAX_FRM_TRN_get      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_MAX_FRM_DIS_set      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_MAX_FRM_DIS_get      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_MAX_FRM_CK_set       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_MAX_FRM_CK_get       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_MIN_FRM_DIS_set      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_MIN_FRM_DIS_get      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_MIN_FRM_CK_set       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_MIN_FRM_CK_get       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_CRC_DIS_set          ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_CRC_DIS_get          ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_CRC_CK_set           ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_CRC_CK_get           ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_TRANSPARENT_set      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_TRANSPARENT_get      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_ETHER_MODE_set       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_ETHER_MODE_get       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_RMAC_ENBL_set        ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_RMAC_ENBL_get        ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 rmac_field_RMAC_STATUS_get      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +--------------------------------+
 | | Register 0x00000104 CFG_2      |
 | +--------------------------------+
 | | bit  15    R/W  INTERPKT_L1_EN |
 | | bit  14:13 R/W  BLIND          |
 | | bit  12:7  R/W  MIN_FRM_LEN    |
 | | bit  6     R/W  PAD_STRIP_EN   |
 | | bit  5     R/W  CRC_STRIP_EN   |
 | | bit  4     R/W  CARRIER_EN     |
 | | bit  3     R/W  INPAUSE_EN     |
 | | bit  2     R/W  PAUSEFRM_DROP  |
 | | bit  1     R/W  CTRLFRM_DROP   |
 | | bit  0     R/W  DA_DROP        |
 | +--------------------------------+
 | 
 | void   rmac_field_INTERPKT_L1_EN_set    ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_INTERPKT_L1_EN_get    ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_BLIND_set             ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_BLIND_get             ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_range_BLIND_set       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rmac_field_range_BLIND_get       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   rmac_field_MIN_FRM_LEN_set       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_MIN_FRM_LEN_get       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_range_MIN_FRM_LEN_set ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rmac_field_range_MIN_FRM_LEN_get ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   rmac_field_PAD_STRIP_EN_set      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_PAD_STRIP_EN_get      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_CRC_STRIP_EN_set      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_CRC_STRIP_EN_get      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_CARRIER_EN_set        ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_CARRIER_EN_get        ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_INPAUSE_EN_set        ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_INPAUSE_EN_get        ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_PAUSEFRM_DROP_set     ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_PAUSEFRM_DROP_get     ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_CTRLFRM_DROP_set      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_CTRLFRM_DROP_get      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_DA_DROP_set           ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_DA_DROP_get           ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00000108 MAX_FRAME_LENGTH_CFG |
 | +------------------------------------------+
 | | bit  15:0 R/W  MAX_FRM_LEN               |
 | +------------------------------------------+
 | 
 | void   rmac_field_MAX_FRM_LEN_set       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_MAX_FRM_LEN_get       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_range_MAX_FRM_LEN_set ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rmac_field_range_MAX_FRM_LEN_get ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x0000010c STATION_ADR_HIGH_WORD_CFG |
 | +-----------------------------------------------+
 | | bit  15:0 R/W  STA                            |
 | +-----------------------------------------------+
 | 
 | void   rmac_lfield_STA_set       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | void   rmac_lfield_STA_get       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | void   rmac_lfield_range_STA_set ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rmac_lfield_range_STA_get ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000118 LINK_AND_STATISTIC_CFG |
 | +--------------------------------------------+
 | | bit  15  R    GIGA_STATUS                  |
 | | bit  9   R/W  CTRLFRM_STAT_EN              |
 | | bit  8   R/W  PAUSEFRM_STAT_EN             |
 | | bit  7:4 R/W  GPO                          |
 | +--------------------------------------------+
 | 
 | void   rmac_field_CTRLFRM_STAT_EN_set  ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_CTRLFRM_STAT_EN_get  ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_PAUSEFRM_STAT_EN_set ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_PAUSEFRM_STAT_EN_get ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_GPO_set              ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 rmac_field_GPO_get              ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   rmac_field_range_GPO_set        ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rmac_field_range_GPO_get        ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 rmac_field_GIGA_STATUS_get      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 +---------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _RMAC_IO_H
#define _RMAC_IO_H

#include "enet_fege_loc.h"
#include "rmac_regs.h"
#include "rmac_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 rmac_buffer_init               ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void                 rmac_buffer_flush              ( rmac_buffer_t *b_ptr )
 | UINT32               rmac_reg_read                  ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 rmac_reg_write                 ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 rmac_field_set                 ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 rmac_action_on_write_field_set ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rmac_burst_read                ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 rmac_burst_write               ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE rmac_poll                      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   rmac_reg_CFG_1_write                           ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   rmac_reg_CFG_1_field_set                       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 rmac_reg_CFG_1_read                            ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   rmac_reg_CFG_2_write                           ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   rmac_reg_CFG_2_field_set                       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 rmac_reg_CFG_2_read                            ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   rmac_reg_MAX_FRAME_LENGTH_CFG_write            ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   rmac_reg_MAX_FRAME_LENGTH_CFG_field_set        ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 rmac_reg_MAX_FRAME_LENGTH_CFG_read             ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   rmac_reg_STATION_ADR_HIGH_WORD_CFG_write       ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   rmac_reg_STATION_ADR_HIGH_WORD_CFG_field_set   ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 rmac_reg_STATION_ADR_HIGH_WORD_CFG_read        ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_write     ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_field_set ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 rmac_reg_STATION_ADR_MIDDLE_WORD_CFG_read      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   rmac_reg_STATION_ADR_LOW_WORD_CFG_write        ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   rmac_reg_STATION_ADR_LOW_WORD_CFG_field_set    ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 rmac_reg_STATION_ADR_LOW_WORD_CFG_read         ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   rmac_reg_LINK_AND_STATISTIC_CFG_write          ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   rmac_reg_LINK_AND_STATISTIC_CFG_field_set      ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 rmac_reg_LINK_AND_STATISTIC_CFG_read           ( rmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _RMAC_IO_H */
