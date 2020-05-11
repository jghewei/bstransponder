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
 *     and register accessor functions for the expand_ibuf block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing expand_ibuf io functions is:
 | {
 |     expand_ibuf_buffer_t b_ptr[1];
 |     expand_ibuf_buffer_init( b_ptr, h_ptr );
 |     value = expand_ibuf_field_<FIELD>_get( b_ptr, h_ptr );
 |     expand_ibuf_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     expand_ibuf_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = expand_ibuf_field_<FIELD>_get( NULL, h_ptr );
 | expand_ibuf_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +------------------------------------+
 | | Register 0x00000360 MEM_CFG_STA_ID |
 | +------------------------------------+
 | | bit  14:8 R/W  MEM_LO_ID           |
 | | bit  6:0  R/W  MEM_HO_ID           |
 | +------------------------------------+
 | 
 | void   expand_ibuf_field_MEM_LO_ID_set       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 value )
 | UINT32 expand_ibuf_field_MEM_LO_ID_get       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr )
 | void   expand_ibuf_field_range_MEM_LO_ID_set ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_MEM_LO_ID_get ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   expand_ibuf_field_MEM_HO_ID_set       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 value )
 | UINT32 expand_ibuf_field_MEM_HO_ID_get       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr )
 | void   expand_ibuf_field_range_MEM_HO_ID_set ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_MEM_HO_ID_get ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register (0x00000400 + (N) * 0x8) CFG_UPDATE |
 | |     N[0..95]                                 |
 | +----------------------------------------------+
 | | bit  16 R/W  NORM_CMD                        |
 | | bit  8  R/W  CONFIG_MODE                     |
 | | bit  0  R/W  CONFIG_UPDATE                   |
 | +----------------------------------------------+
 | 
 | void                 expand_ibuf_field_NORM_CMD_set       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               expand_ibuf_field_NORM_CMD_get       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void                 expand_ibuf_field_CONFIG_MODE_set    ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               expand_ibuf_field_CONFIG_MODE_get    ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void                 expand_ibuf_field_CONFIG_UPDATE_set  ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               expand_ibuf_field_CONFIG_UPDATE_get  ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE expand_ibuf_field_CONFIG_UPDATE_poll ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------------+
 | | Register (0x00000404 + (N) * 0x8) CHANNEL_CFG_REG |
 | |     N[0..95]                                      |
 | +---------------------------------------------------+
 | | bit  31:17 R/W  HO_CAL_CYCLE_TOT                  |
 | | bit  16    R/W  HO_LAST_CYCLE                     |
 | | bit  15    R/W  HO_FIRST_CYCLE                    |
 | | bit  14:8  R/W  HO_ID                             |
 | | bit  7:0   R/W  MF_TOT                            |
 | +---------------------------------------------------+
 | 
 | void   expand_ibuf_field_HO_CAL_CYCLE_TOT_set       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_HO_CAL_CYCLE_TOT_get       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_range_HO_CAL_CYCLE_TOT_set ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_HO_CAL_CYCLE_TOT_get ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   expand_ibuf_field_HO_LAST_CYCLE_set          ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_HO_LAST_CYCLE_get          ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_HO_FIRST_CYCLE_set         ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_HO_FIRST_CYCLE_get         ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_HO_ID_set                  ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_HO_ID_get                  ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_range_HO_ID_set            ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_HO_ID_get            ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   expand_ibuf_field_MF_TOT_set                 ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_MF_TOT_get                 ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_range_MF_TOT_set           ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_MF_TOT_get           ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00000800 + (N) * 0x4) LO_CYCLE_CFG |
 | |     N[0..95]                                   |
 | +------------------------------------------------+
 | | bit  8 R/W  EOMF_DATA_CYCLE                    |
 | | bit  0 R/W  LO_FIRST_CYCLE                     |
 | +------------------------------------------------+
 | 
 | void   expand_ibuf_field_EOMF_DATA_CYCLE_set ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_EOMF_DATA_CYCLE_get ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_LO_FIRST_CYCLE_set  ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_LO_FIRST_CYCLE_get  ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------------+
 | | Register (0x00000c00 + (N) * 0x10) TXJC_MGEN_CFGS |
 | |     N[0..95]                                      |
 | +---------------------------------------------------+
 | | bit  30:28 R/W  CHAN_MAP_TYPE                     |
 | | bit  27:21 R/W  M                                 |
 | | bit  20:0  R/W  SERVER_SIZE                       |
 | +---------------------------------------------------+
 | 
 | void   expand_ibuf_field_CHAN_MAP_TYPE_set       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_CHAN_MAP_TYPE_get       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_range_CHAN_MAP_TYPE_set ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_CHAN_MAP_TYPE_get ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   expand_ibuf_field_M_set                   ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_M_get                   ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_range_M_set             ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_M_get             ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   expand_ibuf_field_SERVER_SIZE_set         ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_SERVER_SIZE_get         ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_range_SERVER_SIZE_set   ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_SERVER_SIZE_get   ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00000c04 + (N) * 0x10) HAO_CFG_REG |
 | |     N[0..95]                                   |
 | +------------------------------------------------+
 | | bit  31:17 R/W  ODTU_CNT_TOT                   |
 | | bit  16:10 R/W  EOMF_LASTCYCLE_DATA            |
 | | bit  9:7   R/W  NUM_TRIG                       |
 | | bit  6:0   R/W  FIRST_CYCLE                    |
 | +------------------------------------------------+
 | 
 | void   expand_ibuf_field_ODTU_CNT_TOT_set              ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_ODTU_CNT_TOT_get              ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_range_ODTU_CNT_TOT_set        ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_ODTU_CNT_TOT_get        ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   expand_ibuf_field_EOMF_LASTCYCLE_DATA_set       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_EOMF_LASTCYCLE_DATA_get       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_range_EOMF_LASTCYCLE_DATA_set ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_EOMF_LASTCYCLE_DATA_get ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   expand_ibuf_field_NUM_TRIG_set                  ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_NUM_TRIG_get                  ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_range_NUM_TRIG_set            ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_NUM_TRIG_get            ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   expand_ibuf_field_FIRST_CYCLE_set               ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_FIRST_CYCLE_get               ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_range_FIRST_CYCLE_set         ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_FIRST_CYCLE_get         ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00000c08 + (N) * 0x10) CFC_XOFF_LVL |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  9:0 R/W  CFC_FILL_LVL                      |
 | +-------------------------------------------------+
 | 
 | void   expand_ibuf_field_CFC_FILL_LVL_set       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_CFC_FILL_LVL_get       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_range_CFC_FILL_LVL_set ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_CFC_FILL_LVL_get ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00001800 + (N) * 0x4) SOMF_CFG_REG |
 | |     N[0..95]                                   |
 | +------------------------------------------------+
 | | bit  22:16 R/W  SOMF_FIRSTCYCLE_DATA           |
 | | bit  14:0  R/W  SOMF_CYCLE_CNT                 |
 | +------------------------------------------------+
 | 
 | void   expand_ibuf_field_SOMF_FIRSTCYCLE_DATA_set       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_SOMF_FIRSTCYCLE_DATA_get       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_range_SOMF_FIRSTCYCLE_DATA_set ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_SOMF_FIRSTCYCLE_DATA_get ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   expand_ibuf_field_SOMF_CYCLE_CNT_set             ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 expand_ibuf_field_SOMF_CYCLE_CNT_get             ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | void   expand_ibuf_field_range_SOMF_CYCLE_CNT_set       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 expand_ibuf_field_range_SOMF_CYCLE_CNT_get       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _EXPAND_IBUF_IO_H
#define _EXPAND_IBUF_IO_H

#include "expand_ibuf_loc.h"
#include "expand_ibuf_regs.h"
#include "expand_ibuf_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 expand_ibuf_buffer_init               ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr )
 | void                 expand_ibuf_buffer_flush              ( expand_ibuf_buffer_t *b_ptr )
 | UINT32               expand_ibuf_reg_read                  ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 expand_ibuf_reg_write                 ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 expand_ibuf_field_set                 ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 expand_ibuf_action_on_write_field_set ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 expand_ibuf_burst_read                ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 expand_ibuf_burst_write               ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE expand_ibuf_poll                      ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 expand_ibuf_reg_MEM_CFG_STA_ID_write            ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 value )
 | void                 expand_ibuf_reg_MEM_CFG_STA_ID_field_set        ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               expand_ibuf_reg_MEM_CFG_STA_ID_read             ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr )
 | 
 | void                 expand_ibuf_reg_CFG_UPDATE_array_write          ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 expand_ibuf_reg_CFG_UPDATE_array_field_set      ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               expand_ibuf_reg_CFG_UPDATE_array_read           ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE expand_ibuf_reg_CFG_UPDATE_array_poll           ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 expand_ibuf_reg_LO_CYCLE_CFG_array_write        ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 expand_ibuf_reg_LO_CYCLE_CFG_array_field_set    ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               expand_ibuf_reg_LO_CYCLE_CFG_array_read         ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | 
 | void                 expand_ibuf_reg_CHANNEL_CFG_REG_array_write     ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 expand_ibuf_reg_CHANNEL_CFG_REG_array_field_set ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               expand_ibuf_reg_CHANNEL_CFG_REG_array_read      ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | 
 | void                 expand_ibuf_reg_TXJC_MGEN_CFGS_array_write      ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 expand_ibuf_reg_TXJC_MGEN_CFGS_array_field_set  ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               expand_ibuf_reg_TXJC_MGEN_CFGS_array_read       ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | 
 | void                 expand_ibuf_reg_HAO_CFG_REG_array_write         ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 expand_ibuf_reg_HAO_CFG_REG_array_field_set     ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               expand_ibuf_reg_HAO_CFG_REG_array_read          ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | 
 | void                 expand_ibuf_reg_CFC_XOFF_LVL_array_write        ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 expand_ibuf_reg_CFC_XOFF_LVL_array_field_set    ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               expand_ibuf_reg_CFC_XOFF_LVL_array_read         ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | 
 | void                 expand_ibuf_reg_SOMF_CFG_REG_array_write        ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 expand_ibuf_reg_SOMF_CFG_REG_array_field_set    ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               expand_ibuf_reg_SOMF_CFG_REG_array_read         ( expand_ibuf_buffer_t *b_ptr, expand_ibuf_handle_t *h_ptr, UINT32  N )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _EXPAND_IBUF_IO_H */
