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
 *     and register accessor functions for the pcie_al block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing pcie_al io functions is:
 | {
 |     pcie_al_buffer_t b_ptr[1];
 |     pcie_al_buffer_init( b_ptr, h_ptr );
 |     value = pcie_al_field_<FIELD>_get( b_ptr, h_ptr );
 |     pcie_al_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     pcie_al_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = pcie_al_field_<FIELD>_get( NULL, h_ptr );
 | pcie_al_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------------------------------------+
 | | Register 0x00009030 MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  MU_IB_DB_E2                                   |
 | +--------------------------------------------------------------+
 | 
 | void   pcie_al_field_MU_IB_DB_E2_set       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 pcie_al_field_MU_IB_DB_E2_get       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   pcie_al_field_range_MU_IB_DB_E2_set ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 pcie_al_field_range_MU_IB_DB_E2_get ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register 0x00009034 MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT |
 | +------------------------------------------------------------+
 | | bit  31:0 R/W  MU_IB_DB_I2                                 |
 | +------------------------------------------------------------+
 | 
 | void                 pcie_al_field_MU_IB_DB_I2_set_to_clear       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32               pcie_al_field_MU_IB_DB_I2_get                ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 pcie_al_field_range_MU_IB_DB_I2_set_to_clear ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               pcie_al_field_range_MU_IB_DB_I2_get          ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE pcie_al_field_range_MU_IB_DB_I2_poll         ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE pcie_al_field_MU_IB_DB_I2_poll               ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------------------------------+
 | | Register 0x00009038 MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG |
 | +---------------------------------------------------------------------+
 | | bit  31:0 R/W  MU_OB_DB_CLR_E3                                      |
 | +---------------------------------------------------------------------+
 | 
 | void   pcie_al_field_MU_OB_DB_CLR_E3_set       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 pcie_al_field_MU_OB_DB_CLR_E3_get       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   pcie_al_field_range_MU_OB_DB_CLR_E3_set ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 pcie_al_field_range_MU_OB_DB_CLR_E3_get ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x0000903c MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  MU_OB_DB_CLR_I3                                    |
 | +-------------------------------------------------------------------+
 | 
 | void                 pcie_al_field_MU_OB_DB_CLR_I3_set_to_clear       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32               pcie_al_field_MU_OB_DB_CLR_I3_get                ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 pcie_al_field_range_MU_OB_DB_CLR_I3_set_to_clear ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               pcie_al_field_range_MU_OB_DB_CLR_I3_get          ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE pcie_al_field_range_MU_OB_DB_CLR_I3_poll         ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE pcie_al_field_MU_OB_DB_CLR_I3_poll               ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x00009044 LEGACY_INT_EN_REG |
 | +---------------------------------------+
 | | bit  8 R/W  INTA_EN                   |
 | +---------------------------------------+
 | 
 | void   pcie_al_field_INTA_EN_set ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 pcie_al_field_INTA_EN_get ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00009340 MSI_X_PENDING_BIT_ARRAY |
 | +---------------------------------------------+
 | | bit  31:0 R  MSIX_PBA                       |
 | +---------------------------------------------+
 | 
 | UINT32 pcie_al_field_MSIX_PBA_get       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 pcie_al_field_range_MSIX_PBA_get ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00009344 PCIE_OB_DB_SET_REG |
 | +----------------------------------------+
 | | bit  31:0 R/W  PCIE_OB_DB_SET          |
 | +----------------------------------------+
 | 
 | void   pcie_al_field_PCIE_OB_DB_SET_set       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 pcie_al_field_PCIE_OB_DB_SET_get       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   pcie_al_field_range_PCIE_OB_DB_SET_set ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 pcie_al_field_range_PCIE_OB_DB_SET_get ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00009348 PCIE_OB_DB_CLR_REG |
 | +----------------------------------------+
 | | bit  31:0 R/W  PCIE_OB_DB_CLR          |
 | +----------------------------------------+
 | 
 | void   pcie_al_field_PCIE_OB_DB_CLR_set       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 pcie_al_field_PCIE_OB_DB_CLR_get       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   pcie_al_field_range_PCIE_OB_DB_CLR_set ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 pcie_al_field_range_PCIE_OB_DB_CLR_get ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x0000934c PCIE_OB_DB_EN_REG |
 | +---------------------------------------+
 | | bit  31:0 R  PCIE_OB_DB_EN            |
 | +---------------------------------------+
 | 
 | UINT32 pcie_al_field_PCIE_OB_DB_EN_get       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 pcie_al_field_range_PCIE_OB_DB_EN_get ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00009350 PCIE_OB_DB_STAT_REG |
 | +-----------------------------------------+
 | | bit  31:0 R  PCIE_OB_DB_STAT            |
 | +-----------------------------------------+
 | 
 | UINT32 pcie_al_field_PCIE_OB_DB_STAT_get       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 pcie_al_field_range_PCIE_OB_DB_STAT_get ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00009354 PCIE_OB_DB_RSTAT_REG |
 | +------------------------------------------+
 | | bit  31:0 R  PCIE_OB_DB_RSTAT            |
 | +------------------------------------------+
 | 
 | UINT32 pcie_al_field_PCIE_OB_DB_RSTAT_get       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 pcie_al_field_range_PCIE_OB_DB_RSTAT_get ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00009358 PCIE_OB_IRQ_SET_REG |
 | +-----------------------------------------+
 | | bit  31:0 R/W  PCIE_OB_IRQ_SET          |
 | +-----------------------------------------+
 | 
 | void   pcie_al_field_PCIE_OB_IRQ_SET_set       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 pcie_al_field_PCIE_OB_IRQ_SET_get       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   pcie_al_field_range_PCIE_OB_IRQ_SET_set ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 pcie_al_field_range_PCIE_OB_IRQ_SET_get ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0000935c PCIE_OB_IRQ_CLR_REG |
 | +-----------------------------------------+
 | | bit  31:0 R/W  PCIE_OB_IRQ_CLR          |
 | +-----------------------------------------+
 | 
 | void   pcie_al_field_PCIE_OB_IRQ_CLR_set       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 pcie_al_field_PCIE_OB_IRQ_CLR_get       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   pcie_al_field_range_PCIE_OB_IRQ_CLR_set ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 pcie_al_field_range_PCIE_OB_IRQ_CLR_get ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00009360 PCIE_OB_IRQ_EN_REG |
 | +----------------------------------------+
 | | bit  31:0 R  PCIE_OB_IRQ_EN            |
 | +----------------------------------------+
 | 
 | UINT32 pcie_al_field_PCIE_OB_IRQ_EN_get       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 pcie_al_field_range_PCIE_OB_IRQ_EN_get ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00009364 PCIE_OB_IRQ_STAT_REG |
 | +------------------------------------------+
 | | bit  31:0 R  PCIE_OB_IRQ_STAT            |
 | +------------------------------------------+
 | 
 | UINT32 pcie_al_field_PCIE_OB_IRQ_STAT_get       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 pcie_al_field_range_PCIE_OB_IRQ_STAT_get ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00009368 PCIE_OB_IRQ_RSTAT_REG |
 | +-------------------------------------------+
 | | bit  31:0 R  PCIE_OB_IRQ_RSTAT            |
 | +-------------------------------------------+
 | 
 | UINT32 pcie_al_field_PCIE_OB_IRQ_RSTAT_get       ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 pcie_al_field_range_PCIE_OB_IRQ_RSTAT_get ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _PCIE_AL_IO_H
#define _PCIE_AL_IO_H

#include "pmc_handle.h"
#include "pcie_al_regs.h"
#include "pcie_al_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 pcie_al_buffer_init               ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 pcie_al_buffer_flush              ( pcie_al_buffer_t *b_ptr )
 | UINT32               pcie_al_reg_read                  ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 pcie_al_reg_write                 ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 pcie_al_field_set                 ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 pcie_al_action_on_write_field_set ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 pcie_al_burst_read                ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 pcie_al_burst_write               ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE pcie_al_poll                      ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 pcie_al_reg_LEGACY_INT_EN_REG_write                                                 ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 pcie_al_reg_LEGACY_INT_EN_REG_field_set                                             ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pcie_al_reg_LEGACY_INT_EN_REG_read                                                  ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 pcie_al_reg_PCIE_OB_DB_SET_REG_write                                                ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 pcie_al_reg_PCIE_OB_DB_SET_REG_field_set                                            ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pcie_al_reg_PCIE_OB_DB_SET_REG_read                                                 ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 pcie_al_reg_PCIE_OB_DB_CLR_REG_write                                                ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 pcie_al_reg_PCIE_OB_DB_CLR_REG_field_set                                            ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pcie_al_reg_PCIE_OB_DB_CLR_REG_read                                                 ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 pcie_al_reg_PCIE_OB_IRQ_SET_REG_write                                               ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 pcie_al_reg_PCIE_OB_IRQ_SET_REG_field_set                                           ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pcie_al_reg_PCIE_OB_IRQ_SET_REG_read                                                ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 pcie_al_reg_PCIE_OB_IRQ_CLR_REG_write                                               ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 pcie_al_reg_PCIE_OB_IRQ_CLR_REG_field_set                                           ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pcie_al_reg_PCIE_OB_IRQ_CLR_REG_read                                                ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_write                            ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_action_on_write_field_set        ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_read                             ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_STAT_poll                             ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_write                     ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_action_on_write_field_set ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_read                      ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_STAT_poll                      ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_write                          ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_field_set                      ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pcie_al_reg_MESSAGE_UNIT_INBOUND_DOORBELL_INT_EN_REG_read                           ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_write                   ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_field_set               ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               pcie_al_reg_MESSAGE_UNIT_OUTBOUND_DOORBELL_CLEAR_INT_EN_REG_read                    ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32               pcie_al_reg_MSI_X_PENDING_BIT_ARRAY_read                                            ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32               pcie_al_reg_PCIE_OB_DB_EN_REG_read                                                  ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32               pcie_al_reg_PCIE_OB_DB_STAT_REG_read                                                ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32               pcie_al_reg_PCIE_OB_DB_RSTAT_REG_read                                               ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32               pcie_al_reg_PCIE_OB_IRQ_EN_REG_read                                                 ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32               pcie_al_reg_PCIE_OB_IRQ_STAT_REG_read                                               ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32               pcie_al_reg_PCIE_OB_IRQ_RSTAT_REG_read                                              ( pcie_al_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _PCIE_AL_IO_H */
