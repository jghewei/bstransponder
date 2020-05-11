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
 *     and register accessor functions for the config_and_status block
 *****************************************************************************/

/*----------------------------------------------------------------------------------------------------------------.
 | The pattern for reading indirect registers is:
 | {
 |     config_and_status_GENERATOR_B1_E1_VAL_buffer_t b_ptr[1];
 |     config_and_status_GENERATOR_B1_E1_VAL_buffer_init(b_ptr);
 |     config_and_status_GENERATOR_B1_E1_VAL_buffer_retrieve(h_ptr, some_index, some_other_index, b_ptr);
 |     value = config_and_status_<FIELD1>_get(b_ptr);
 |     value = config_and_status_<FIELD2>_get(b_ptr);
 | ...
 | 
 | The pattern for writing indirect registers is:
 | {
 |     config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t b_ptr[1];
 |     GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_init(b_ptr);
 |     value = config_and_status_<FIELD1>_set(b_ptr, value1);
 |     value = config_and_status_<FIELD2>_set(b_ptr, value2);
 | ...
 |     config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_applyh_ptr, some_index, some_other_index, b_ptr);
 | }
 | 
 | note: the indexing arguments are defined by the user.
 | 
 +---------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------------+
 | | Register 0x00000000 MON_STS_1_PATH_CFG |
 | +----------------------------------------+
 | | bit  3 W    RESYNC                     |
 | | bit  2 R/W  MON_INV_PRBS               |
 | | bit  0 R/W  MON_ENA                    |
 | +----------------------------------------+
 | 
 | void   config_and_status_ifield_RESYNC_set       ( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
 | void   config_and_status_ifield_MON_INV_PRBS_set ( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 config_and_status_ifield_MON_INV_PRBS_get ( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr )
 | void   config_and_status_ifield_MON_ENA_set      ( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 config_and_status_ifield_MON_ENA_get      ( config_and_status_MON_STS_1_PATH_CFG_buffer_t *b_ptr )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000001 MON_PRBS_22_7_ACCUMULATOR |
 | +-----------------------------------------------+
 | | bit  15:0 R/W  MON_PRBS                       |
 | +-----------------------------------------------+
 | 
 | void   config_and_status_ifield_MON_PRBS_set ( config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr, UINT32 value )
 | UINT32 config_and_status_ifield_MON_PRBS_get ( config_and_status_MON_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00000004 MON_ERR_CNT |
 | +---------------------------------+
 | | bit  15:0 R  ERR_CNT            |
 | +---------------------------------+
 | 
 | UINT32 config_and_status_ifield_ERR_CNT_get ( config_and_status_MON_ERR_CNT_buffer_t *b_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00000008 GENERATOR_STS_1_PATH_CFG |
 | +----------------------------------------------+
 | | bit  12  R/W  LINKENA0                       |
 | | bit  9:8 R/W  GEN_GPO                        |
 | | bit  7:6 R/W  S                              |
 | | bit  4   R/W  GEN_B1E1_ENA                   |
 | | bit  3   W    FORCE_ERR                      |
 | | bit  1   R/W  GEN_INV_PRBS                   |
 | | bit  0   R/W  GEN_AMODE                      |
 | +----------------------------------------------+
 | 
 | void   config_and_status_ifield_LINKENA0_set     ( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 config_and_status_ifield_LINKENA0_get     ( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
 | void   config_and_status_ifield_GEN_GPO_set      ( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 config_and_status_ifield_GEN_GPO_get      ( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
 | void   config_and_status_ifield_S_set            ( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 config_and_status_ifield_S_get            ( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
 | void   config_and_status_ifield_GEN_B1E1_ENA_set ( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 config_and_status_ifield_GEN_B1E1_ENA_get ( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
 | void   config_and_status_ifield_FORCE_ERR_set    ( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
 | void   config_and_status_ifield_GEN_INV_PRBS_set ( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 config_and_status_ifield_GEN_INV_PRBS_get ( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
 | void   config_and_status_ifield_GEN_AMODE_set    ( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 config_and_status_ifield_GEN_AMODE_get    ( config_and_status_GENERATOR_STS_1_PATH_CFG_buffer_t *b_ptr )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000009 GENERATOR_PRBS_22_7_ACCUMULATOR |
 | +-----------------------------------------------------+
 | | bit  15:0 R/W  GEN_PRBS                             |
 | +-----------------------------------------------------+
 | 
 | void   config_and_status_ifield_GEN_PRBS_set ( config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr, UINT32 value )
 | UINT32 config_and_status_ifield_GEN_PRBS_get ( config_and_status_GENERATOR_PRBS_22_7_ACCUMULATOR_buffer_t *b_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x0000000b GENERATOR_B1_E1_VAL |
 | +-----------------------------------------+
 | | bit  7:0 R/W  GEN_B1                    |
 | +-----------------------------------------+
 | 
 | void   config_and_status_ifield_GEN_B1_set ( config_and_status_GENERATOR_B1_E1_VAL_buffer_t *b_ptr, UINT32 value )
 | UINT32 config_and_status_ifield_GEN_B1_get ( config_and_status_GENERATOR_B1_E1_VAL_buffer_t *b_ptr )
 +---------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _CONFIG_AND_STATUS_IO_H
#define _CONFIG_AND_STATUS_IO_H

#include "cbrc_api.h"
#include "config_and_status_regs.h"
#include "config_and_status_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 config_and_status_buffer_init               ( config_and_status_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 config_and_status_buffer_flush              ( config_and_status_buffer_t *b_ptr )
 | UINT32               config_and_status_reg_read                  ( config_and_status_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 config_and_status_reg_write                 ( config_and_status_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 config_and_status_field_set                 ( config_and_status_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 config_and_status_action_on_write_field_set ( config_and_status_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 config_and_status_burst_read                ( config_and_status_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 config_and_status_burst_write               ( config_and_status_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE config_and_status_poll                      ( config_and_status_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _CONFIG_AND_STATUS_IO_H */
