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
 *     and register accessor functions for the tmac block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing tmac io functions is:
 | {
 |     tmac_buffer_t b_ptr[1];
 |     tmac_buffer_init( b_ptr, h_ptr );
 |     value = tmac_field_<FIELD>_get( b_ptr, h_ptr );
 |     tmac_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     tmac_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = tmac_field_<FIELD>_get( NULL, h_ptr );
 | tmac_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-------------------------------+
 | | Register 0x00002000 CFG_1     |
 | +-------------------------------+
 | | bit  15    R    TMAC_STATUS   |
 | | bit  14    R/W  CRC_CK        |
 | | bit  13:12 R/W  PAD_ADD_EN    |
 | | bit  11    R/W  CRC_ADD_EN    |
 | | bit  10    R/W  MIN_FRM_CK    |
 | | bit  9     R/W  TRUNC_EN      |
 | | bit  8     R/W  MAX_FRM_CK    |
 | | bit  7     R/W  EBUF_PAUSE_EN |
 | | bit  6     R/W  IBUF_HTHR_EN  |
 | | bit  5     R/W  XON_EN        |
 | | bit  4     R/W  RMAC_PAUSE_EN |
 | | bit  3     R/W  EBUF_ERR_EN   |
 | | bit  2     R/W  TRANSPARENT   |
 | | bit  1     R/W  HW_STOP_EN    |
 | | bit  0     R/W  TMAC_ENBL     |
 | +-------------------------------+
 | 
 | void   tmac_field_CRC_CK_set           ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_CRC_CK_get           ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_PAD_ADD_EN_set       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_PAD_ADD_EN_get       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_range_PAD_ADD_EN_set ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tmac_field_range_PAD_ADD_EN_get ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tmac_field_CRC_ADD_EN_set       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_CRC_ADD_EN_get       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_MIN_FRM_CK_set       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_MIN_FRM_CK_get       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_TRUNC_EN_set         ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_TRUNC_EN_get         ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_MAX_FRM_CK_set       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_MAX_FRM_CK_get       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_EBUF_PAUSE_EN_set    ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_EBUF_PAUSE_EN_get    ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_IBUF_HTHR_EN_set     ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_IBUF_HTHR_EN_get     ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_XON_EN_set           ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_XON_EN_get           ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_RMAC_PAUSE_EN_set    ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_RMAC_PAUSE_EN_get    ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_EBUF_ERR_EN_set      ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_EBUF_ERR_EN_get      ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_TRANSPARENT_set      ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_TRANSPARENT_get      ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_HW_STOP_EN_set       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_HW_STOP_EN_get       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_TMAC_ENBL_set        ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_TMAC_ENBL_get        ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 tmac_field_TMAC_STATUS_get      ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +---------------------------+
 | | Register 0x00002004 CFG_2 |
 | +---------------------------+
 | | bit  7   R/W  LATE_COL_EN |
 | | bit  4   R/W  SFD_EN      |
 | | bit  3:0 R/W  PAMB_LEN    |
 | +---------------------------+
 | 
 | void   tmac_field_LATE_COL_EN_set    ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_LATE_COL_EN_get    ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_SFD_EN_set         ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_SFD_EN_get         ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_PAMB_LEN_set       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_PAMB_LEN_get       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_range_PAMB_LEN_set ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tmac_field_range_PAMB_LEN_get ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00002008 INTER_PACKET_GAP_CFG_2 |
 | +--------------------------------------------+
 | | bit  15:12 R/W  FIPG1_LEN                  |
 | | bit  11:8  R/W  FIPG2_LEN                  |
 | +--------------------------------------------+
 | 
 | void   tmac_field_FIPG1_LEN_set       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_FIPG1_LEN_get       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_range_FIPG1_LEN_set ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tmac_field_range_FIPG1_LEN_get ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tmac_field_FIPG2_LEN_set       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_FIPG2_LEN_get       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_range_FIPG2_LEN_set ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tmac_field_range_FIPG2_LEN_get ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x0000200c STATION_ADR_HIGH_WORD_CFG |
 | +-----------------------------------------------+
 | | bit  15:0 R/W  STA                            |
 | +-----------------------------------------------+
 | 
 | void   tmac_lfield_STA_set       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | void   tmac_lfield_STA_get       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | void   tmac_lfield_range_STA_set ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tmac_lfield_range_STA_get ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00002018 MAX_FRAME_LENGTH_CFG |
 | +------------------------------------------+
 | | bit  15:0 R/W  MAX_FRM_LEN               |
 | +------------------------------------------+
 | 
 | void   tmac_field_MAX_FRM_LEN_set       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_MAX_FRM_LEN_get       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_range_MAX_FRM_LEN_set ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tmac_field_range_MAX_FRM_LEN_get ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0000201c MIN_FRAME_LENGTH_CFG |
 | +------------------------------------------+
 | | bit  6   R/W  MIN_FRM_TAG                |
 | | bit  5:0 R/W  MIN_FRM_LEN                |
 | +------------------------------------------+
 | 
 | void   tmac_field_MIN_FRM_TAG_set       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_MIN_FRM_TAG_get       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_MIN_FRM_LEN_set       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_MIN_FRM_LEN_get       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_range_MIN_FRM_LEN_set ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tmac_field_range_MIN_FRM_LEN_get ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00002020 PAUSE_FRAME_TIME_CFG |
 | +------------------------------------------+
 | | bit  15:0 R/W  PAUSE_TIME                |
 | +------------------------------------------+
 | 
 | void   tmac_field_PAUSE_TIME_set       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_PAUSE_TIME_get       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_range_PAUSE_TIME_set ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tmac_field_range_PAUSE_TIME_get ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x00002024 PAUSE_FRAME_INTERVAL_CFG |
 | +----------------------------------------------+
 | | bit  15:0 R/W  PAUSE_IVAL                    |
 | +----------------------------------------------+
 | 
 | void   tmac_field_PAUSE_IVAL_set       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 tmac_field_PAUSE_IVAL_get       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   tmac_field_range_PAUSE_IVAL_set ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tmac_field_range_PAUSE_IVAL_get ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +---------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _TMAC_IO_H
#define _TMAC_IO_H

#include "enet_fege_loc.h"
#include "tmac_regs.h"
#include "tmac_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 tmac_buffer_init               ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void                 tmac_buffer_flush              ( tmac_buffer_t *b_ptr )
 | UINT32               tmac_reg_read                  ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 tmac_reg_write                 ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 tmac_field_set                 ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 tmac_action_on_write_field_set ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 tmac_burst_read                ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 tmac_burst_write               ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE tmac_poll                      ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   tmac_reg_CFG_1_write                           ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   tmac_reg_CFG_1_field_set                       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tmac_reg_CFG_1_read                            ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   tmac_reg_CFG_2_write                           ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   tmac_reg_CFG_2_field_set                       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tmac_reg_CFG_2_read                            ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   tmac_reg_INTER_PACKET_GAP_CFG_2_write          ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   tmac_reg_INTER_PACKET_GAP_CFG_2_field_set      ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tmac_reg_INTER_PACKET_GAP_CFG_2_read           ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   tmac_reg_STATION_ADR_HIGH_WORD_CFG_write       ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   tmac_reg_STATION_ADR_HIGH_WORD_CFG_field_set   ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tmac_reg_STATION_ADR_HIGH_WORD_CFG_read        ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_write     ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_field_set ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tmac_reg_STATION_ADR_MIDDLE_WORD_CFG_read      ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   tmac_reg_STATION_ADR_LOW_WORD_CFG_write        ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   tmac_reg_STATION_ADR_LOW_WORD_CFG_field_set    ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tmac_reg_STATION_ADR_LOW_WORD_CFG_read         ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   tmac_reg_MAX_FRAME_LENGTH_CFG_write            ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   tmac_reg_MAX_FRAME_LENGTH_CFG_field_set        ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tmac_reg_MAX_FRAME_LENGTH_CFG_read             ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   tmac_reg_MIN_FRAME_LENGTH_CFG_write            ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   tmac_reg_MIN_FRAME_LENGTH_CFG_field_set        ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tmac_reg_MIN_FRAME_LENGTH_CFG_read             ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   tmac_reg_PAUSE_FRAME_TIME_CFG_write            ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   tmac_reg_PAUSE_FRAME_TIME_CFG_field_set        ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tmac_reg_PAUSE_FRAME_TIME_CFG_read             ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   tmac_reg_PAUSE_FRAME_INTERVAL_CFG_write        ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   tmac_reg_PAUSE_FRAME_INTERVAL_CFG_field_set    ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tmac_reg_PAUSE_FRAME_INTERVAL_CFG_read         ( tmac_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _TMAC_IO_H */
