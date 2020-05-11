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
 *     and register accessor functions for the lifd_mux block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing lifd_mux io functions is:
 | {
 |     lifd_mux_buffer_t b_ptr[1];
 |     lifd_mux_buffer_init( b_ptr, h_ptr );
 |     value = lifd_mux_field_<FIELD>_get( b_ptr, h_ptr );
 |     lifd_mux_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     lifd_mux_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = lifd_mux_field_<FIELD>_get( NULL, h_ptr );
 | lifd_mux_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------+
 | | Register 0x00150148 SFI_IF  |
 | +-----------------------------+
 | | bit  12 R/W  SFI2_RXS_INV   |
 | | bit  11 R/W  SFI2_RXS_EN    |
 | | bit  10 R/W  SFI1_RXS_INV   |
 | | bit  9  R/W  SFI1_RXS_EN    |
 | | bit  8  R/W  SFI_IF_SWAP_EN |
 | | bit  7  R/W  SFI2_TX_IFSEL  |
 | | bit  6  R/W  SFI1_TX_IFSEL  |
 | | bit  5  R/W  SFI2_EN        |
 | | bit  4  R/W  SFI1_EN        |
 | | bit  3  R/W  SFI2_OTN_EN    |
 | | bit  2  R/W  SFI1_OTN_EN    |
 | | bit  1  R/W  SFI2_CBRC_EN   |
 | | bit  0  R/W  SFI1_CBRC_EN   |
 | +-----------------------------+
 | 
 | void   lifd_mux_field_SFI2_RXS_INV_set   ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI2_RXS_INV_get   ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_SFI2_RXS_EN_set    ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI2_RXS_EN_get    ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_SFI1_RXS_INV_set   ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI1_RXS_INV_get   ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_SFI1_RXS_EN_set    ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI1_RXS_EN_get    ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_SFI_IF_SWAP_EN_set ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI_IF_SWAP_EN_get ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_SFI2_TX_IFSEL_set  ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI2_TX_IFSEL_get  ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_SFI1_TX_IFSEL_set  ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI1_TX_IFSEL_get  ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_SFI2_EN_set        ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI2_EN_get        ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_SFI1_EN_set        ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI1_EN_get        ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_SFI2_OTN_EN_set    ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI2_OTN_EN_get    ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_SFI1_OTN_EN_set    ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI1_OTN_EN_get    ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_SFI2_CBRC_EN_set   ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI2_CBRC_EN_get   ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_SFI1_CBRC_EN_set   ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI1_CBRC_EN_get   ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +----------------------------------------+
 | | Register 0x0015014c SFI_C_CKCTL_SS_SEL |
 | +----------------------------------------+
 | | bit  3:2 R/W  SFI2_ACB_IF_SEL          |
 | | bit  1:0 R/W  SFI1_ACB_IF_SEL          |
 | +----------------------------------------+
 | 
 | void   lifd_mux_field_SFI2_ACB_IF_SEL_set       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI2_ACB_IF_SEL_get       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_range_SFI2_ACB_IF_SEL_set ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 lifd_mux_field_range_SFI2_ACB_IF_SEL_get ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   lifd_mux_field_SFI1_ACB_IF_SEL_set       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_SFI1_ACB_IF_SEL_get       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_range_SFI1_ACB_IF_SEL_set ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 lifd_mux_field_range_SFI1_ACB_IF_SEL_get ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00150150 PIN_LOS_0_19_INVERT |
 | +-----------------------------------------+
 | | bit  19:0 R/W  PIN_LOS_INV              |
 | +-----------------------------------------+
 | 
 | void   lifd_mux_field_PIN_LOS_INV_set       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_PIN_LOS_INV_get       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_range_PIN_LOS_INV_set ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 lifd_mux_field_range_PIN_LOS_INV_get ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00150154 PIN_LOS_0_19_EN |
 | +-------------------------------------+
 | | bit  19:0 R/W  PIN_LOS_EN           |
 | +-------------------------------------+
 | 
 | void   lifd_mux_field_PIN_LOS_EN_set       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_PIN_LOS_EN_get       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_range_PIN_LOS_EN_set ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 lifd_mux_field_range_PIN_LOS_EN_get ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00150158 SERDES_0_19_DLOS_INVERT |
 | +---------------------------------------------+
 | | bit  19:0 R/W  DLOS_INV                     |
 | +---------------------------------------------+
 | 
 | void   lifd_mux_field_DLOS_INV_set       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_DLOS_INV_get       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_range_DLOS_INV_set ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 lifd_mux_field_range_DLOS_INV_get ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0015015c SERDES_0_19_DLOS_EN |
 | +-----------------------------------------+
 | | bit  19:0 R/W  DLOS_EN                  |
 | +-----------------------------------------+
 | 
 | void   lifd_mux_field_DLOS_EN_set       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_DLOS_EN_get       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_range_DLOS_EN_set ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 lifd_mux_field_range_DLOS_EN_get ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00150160 LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19 |
 | +-----------------------------------------------------------+
 | | bit  19:0 R/W  LOS_POL_INV                                |
 | +-----------------------------------------------------------+
 | 
 | void   lifd_mux_field_LOS_POL_INV_set       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 lifd_mux_field_LOS_POL_INV_get       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   lifd_mux_field_range_LOS_POL_INV_set ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 lifd_mux_field_range_LOS_POL_INV_get ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _LIFD_MUX_IO_H
#define _LIFD_MUX_IO_H

#include "lifd_api.h"
#include "lifd_mux_regs.h"
#include "lifd_mux_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 lifd_mux_buffer_init               ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void                 lifd_mux_buffer_flush              ( lifd_mux_buffer_t *b_ptr )
 | UINT32               lifd_mux_reg_read                  ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 lifd_mux_reg_write                 ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 lifd_mux_field_set                 ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 lifd_mux_action_on_write_field_set ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 lifd_mux_burst_read                ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 lifd_mux_burst_write               ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE lifd_mux_poll                      ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   lifd_mux_reg_SFI_IF_write                                    ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void   lifd_mux_reg_SFI_IF_field_set                                ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 lifd_mux_reg_SFI_IF_read                                     ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void   lifd_mux_reg_SFI_C_CKCTL_SS_SEL_write                        ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void   lifd_mux_reg_SFI_C_CKCTL_SS_SEL_field_set                    ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 lifd_mux_reg_SFI_C_CKCTL_SS_SEL_read                         ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void   lifd_mux_reg_PIN_LOS_0_19_INVERT_write                       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void   lifd_mux_reg_PIN_LOS_0_19_INVERT_field_set                   ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 lifd_mux_reg_PIN_LOS_0_19_INVERT_read                        ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void   lifd_mux_reg_PIN_LOS_0_19_EN_write                           ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void   lifd_mux_reg_PIN_LOS_0_19_EN_field_set                       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 lifd_mux_reg_PIN_LOS_0_19_EN_read                            ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void   lifd_mux_reg_SERDES_0_19_DLOS_INVERT_write                   ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void   lifd_mux_reg_SERDES_0_19_DLOS_INVERT_field_set               ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 lifd_mux_reg_SERDES_0_19_DLOS_INVERT_read                    ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void   lifd_mux_reg_SERDES_0_19_DLOS_EN_write                       ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void   lifd_mux_reg_SERDES_0_19_DLOS_EN_field_set                   ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 lifd_mux_reg_SERDES_0_19_DLOS_EN_read                        ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void   lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_write     ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void   lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_field_set ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 lifd_mux_reg_LOS_POLARITY_CTRL_FOR_DSIS_LANES_0_19_read      ( lifd_mux_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _LIFD_MUX_IO_H */
