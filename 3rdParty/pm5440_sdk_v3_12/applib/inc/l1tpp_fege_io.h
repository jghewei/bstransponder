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
 *     and register accessor functions for the l1tpp_fege block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing l1tpp_fege io functions is:
 | {
 |     l1tpp_fege_buffer_t b_ptr[1];
 |     l1tpp_fege_buffer_init( b_ptr, h_ptr );
 |     value = l1tpp_fege_field_<FIELD>_get( b_ptr, h_ptr );
 |     l1tpp_fege_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     l1tpp_fege_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = l1tpp_fege_field_<FIELD>_get( NULL, h_ptr );
 | l1tpp_fege_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------+
 | | Register 0x00002040 CFG    |
 | +----------------------------+
 | | bit  15  R    L1TPP_STATUS |
 | | bit  8   R/W  RD_ALIGN_EN  |
 | | bit  3:1 R/W  MODE         |
 | | bit  0   R/W  L1TPP_EN     |
 | +----------------------------+
 | 
 | void   l1tpp_fege_field_RD_ALIGN_EN_set  ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1tpp_fege_field_RD_ALIGN_EN_get  ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1tpp_fege_field_MODE_set         ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1tpp_fege_field_MODE_get         ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1tpp_fege_field_range_MODE_set   ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1tpp_fege_field_range_MODE_get   ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   l1tpp_fege_field_L1TPP_EN_set     ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1tpp_fege_field_L1TPP_EN_get     ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1tpp_fege_field_L1TPP_STATUS_get ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00002044 TENB_ERR_RDNEG |
 | +------------------------------------+
 | | bit  9:0 R/W  TENB_ERR_RDNEG       |
 | +------------------------------------+
 | 
 | void   l1tpp_fege_field_TENB_ERR_RDNEG_set       ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1tpp_fege_field_TENB_ERR_RDNEG_get       ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1tpp_fege_field_range_TENB_ERR_RDNEG_set ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1tpp_fege_field_range_TENB_ERR_RDNEG_get ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00002048 CSF_ERR_CODE |
 | +----------------------------------+
 | | bit  8:0 R/W  CSF_ERR_CODE       |
 | +----------------------------------+
 | 
 | void   l1tpp_fege_field_CSF_ERR_CODE_set       ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1tpp_fege_field_CSF_ERR_CODE_get       ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1tpp_fege_field_range_CSF_ERR_CODE_set ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1tpp_fege_field_range_CSF_ERR_CODE_get ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x0000204c JITTER_PATTERN_GENERATION |
 | +-----------------------------------------------+
 | | bit  14:12 R/W  PTRN_SEL                      |
 | | bit  8:0   R/W  CUS_PTRN                      |
 | +-----------------------------------------------+
 | 
 | void   l1tpp_fege_field_PTRN_SEL_set       ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1tpp_fege_field_PTRN_SEL_get       ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1tpp_fege_field_range_PTRN_SEL_set ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1tpp_fege_field_range_PTRN_SEL_get ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   l1tpp_fege_field_CUS_PTRN_set       ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1tpp_fege_field_CUS_PTRN_get       ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1tpp_fege_field_range_CUS_PTRN_set ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1tpp_fege_field_range_CUS_PTRN_get ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------+
 | | Register 0x00002054 CTRL |
 | +--------------------------+
 | | bit  4 R/W  FEF_GEN_EN   |
 | | bit  0 R    INVLD_FRAME  |
 | +--------------------------+
 | 
 | void   l1tpp_fege_field_FEF_GEN_EN_set  ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1tpp_fege_field_FEF_GEN_EN_get  ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1tpp_fege_field_INVLD_FRAME_get ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _L1TPP_FEGE_IO_H
#define _L1TPP_FEGE_IO_H

#include "enet_fege_loc.h"
#include "l1tpp_fege_regs.h"
#include "l1tpp_fege_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 l1tpp_fege_buffer_init               ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void                 l1tpp_fege_buffer_flush              ( l1tpp_fege_buffer_t *b_ptr )
 | UINT32               l1tpp_fege_reg_read                  ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 l1tpp_fege_reg_write                 ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 l1tpp_fege_field_set                 ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 l1tpp_fege_action_on_write_field_set ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 l1tpp_fege_burst_read                ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 l1tpp_fege_burst_write               ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE l1tpp_fege_poll                      ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   l1tpp_fege_reg_CFG_write                           ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   l1tpp_fege_reg_CFG_field_set                       ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 l1tpp_fege_reg_CFG_read                            ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   l1tpp_fege_reg_TENB_ERR_RDNEG_write                ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   l1tpp_fege_reg_TENB_ERR_RDNEG_field_set            ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 l1tpp_fege_reg_TENB_ERR_RDNEG_read                 ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   l1tpp_fege_reg_CSF_ERR_CODE_write                  ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   l1tpp_fege_reg_CSF_ERR_CODE_field_set              ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 l1tpp_fege_reg_CSF_ERR_CODE_read                   ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   l1tpp_fege_reg_JITTER_PATTERN_GENERATION_write     ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   l1tpp_fege_reg_JITTER_PATTERN_GENERATION_field_set ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 l1tpp_fege_reg_JITTER_PATTERN_GENERATION_read      ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   l1tpp_fege_reg_CTRL_write                          ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   l1tpp_fege_reg_CTRL_field_set                      ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 l1tpp_fege_reg_CTRL_read                           ( l1tpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _L1TPP_FEGE_IO_H */
