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
 *     and register accessor functions for the sber_indirect block
 *****************************************************************************/

/*-----------------------------------------------------------------------------------------------.
 | The pattern for reading indirect registers is:
 | {
 |     sber_indirect_SD_BERM_BER_TH_buffer_t b_ptr[1];
 |     sber_indirect_SD_BERM_BER_TH_buffer_init(b_ptr);
 |     sber_indirect_SD_BERM_BER_TH_buffer_retrieve(h_ptr, some_index, some_other_index, b_ptr);
 |     value = sber_indirect_<FIELD1>_get(b_ptr);
 |     value = sber_indirect_<FIELD2>_get(b_ptr);
 | ...
 | 
 | The pattern for writing indirect registers is:
 | {
 |     sber_indirect_SD_BERM_CLEAR_TH_buffer_t b_ptr[1];
 |     SD_BERM_CLEAR_TH_buffer_init(b_ptr);
 |     value = sber_indirect_<FIELD1>_set(b_ptr, value1);
 |     value = sber_indirect_<FIELD2>_set(b_ptr, value2);
 | ...
 |     sber_indirect_SD_BERM_CLEAR_TH_buffer_applyh_ptr, some_index, some_other_index, b_ptr);
 | }
 | 
 | note: the indexing arguments are defined by the user.
 | 
 +----------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------.
 | 
 | +-------------------------------------+
 | | Register 0x00000000 SD_SF_BERM_STAT |
 | +-------------------------------------+
 | | bit  5 R  SFPBS                     |
 | | bit  4 R  SDPBS                     |
 | | bit  3 R  SFCO                      |
 | | bit  2 R  SDCO                      |
 | | bit  1 R  SF_V                      |
 | | bit  0 R  SD_V                      |
 | +-------------------------------------+
 | 
 | UINT32 sber_indirect_ifield_SFPBS_get ( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
 | UINT32 sber_indirect_ifield_SDPBS_get ( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
 | UINT32 sber_indirect_ifield_SFCO_get  ( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
 | UINT32 sber_indirect_ifield_SDCO_get  ( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
 | UINT32 sber_indirect_ifield_SF_V_get  ( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
 | UINT32 sber_indirect_ifield_SD_V_get  ( sber_indirect_SD_SF_BERM_STAT_buffer_t *b_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000001 SD_SF_BERM_INT_EN |
 | +---------------------------------------+
 | | bit  1 R/W  SF_E                      |
 | | bit  0 R/W  SD_E                      |
 | +---------------------------------------+
 | 
 | void   sber_indirect_ifield_SF_E_set ( sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr, UINT32 value )
 | UINT32 sber_indirect_ifield_SF_E_get ( sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr )
 | void   sber_indirect_ifield_SD_E_set ( sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr, UINT32 value )
 | UINT32 sber_indirect_ifield_SD_E_get ( sber_indirect_SD_SF_BERM_INT_EN_buffer_t *b_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x00000002 SD_SF_BERM_INT_STAT |
 | +-----------------------------------------+
 | | bit  1 R/W  SF_I                        |
 | | bit  0 R/W  SD_I                        |
 | +-----------------------------------------+
 | 
 | void   sber_indirect_ifield_SF_I_set_to_clear ( sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr, UINT32 value )
 | UINT32 sber_indirect_ifield_SF_I_get          ( sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr )
 | void   sber_indirect_ifield_SD_I_set_to_clear ( sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr, UINT32 value )
 | UINT32 sber_indirect_ifield_SD_I_get          ( sber_indirect_SD_SF_BERM_INT_STAT_buffer_t *b_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000003 SD_BERM_BER_TH |
 | +------------------------------------+
 | | bit  3:0 R/W  SDTR                 |
 | +------------------------------------+
 | 
 | void   sber_indirect_ifield_SDTR_set ( sber_indirect_SD_BERM_BER_TH_buffer_t *b_ptr, UINT32 value )
 | UINT32 sber_indirect_ifield_SDTR_get ( sber_indirect_SD_BERM_BER_TH_buffer_t *b_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000004 SD_BERM_SET_TH |
 | +------------------------------------+
 | | bit  11:0 R/W  SDST                |
 | +------------------------------------+
 | 
 | void   sber_indirect_ifield_SDST_set ( sber_indirect_SD_BERM_SET_TH_buffer_t *b_ptr, UINT32 value )
 | UINT32 sber_indirect_ifield_SDST_get ( sber_indirect_SD_BERM_SET_TH_buffer_t *b_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000005 SD_BERM_CLEAR_TH |
 | +--------------------------------------+
 | | bit  11:0 R/W  SDCT                  |
 | +--------------------------------------+
 | 
 | void   sber_indirect_ifield_SDCT_set ( sber_indirect_SD_BERM_CLEAR_TH_buffer_t *b_ptr, UINT32 value )
 | UINT32 sber_indirect_ifield_SDCT_get ( sber_indirect_SD_BERM_CLEAR_TH_buffer_t *b_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000006 SD_PATH_MONING_EN |
 | +---------------------------------------+
 | | bit  0 R/W  SD_EN                     |
 | +---------------------------------------+
 | 
 | void   sber_indirect_ifield_SD_EN_set ( sber_indirect_SD_PATH_MONING_EN_buffer_t *b_ptr, UINT32 value )
 | UINT32 sber_indirect_ifield_SD_EN_get ( sber_indirect_SD_PATH_MONING_EN_buffer_t *b_ptr )
 | 
 | +------------------------------------------------------+
 | | Register 0x00000007 SD_BERM_SET_INTEGRATION_PERIOD_2 |
 | +------------------------------------------------------+
 | | bit  15:0 R/W  SDIP                                  |
 | +------------------------------------------------------+
 | 
 | void   sber_indirect_ifield_SDIP_set ( sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr, UINT32 value )
 | UINT32 sber_indirect_ifield_SDIP_get ( sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000013 SF_BERM_BER_TH |
 | +------------------------------------+
 | | bit  3:0 R/W  SFTR                 |
 | +------------------------------------+
 | 
 | void   sber_indirect_ifield_SFTR_set ( sber_indirect_SF_BERM_BER_TH_buffer_t *b_ptr, UINT32 value )
 | UINT32 sber_indirect_ifield_SFTR_get ( sber_indirect_SF_BERM_BER_TH_buffer_t *b_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000014 SF_BERM_SET_TH |
 | +------------------------------------+
 | | bit  11:0 R/W  SFST                |
 | +------------------------------------+
 | 
 | void   sber_indirect_ifield_SFST_set ( sber_indirect_SF_BERM_SET_TH_buffer_t *b_ptr, UINT32 value )
 | UINT32 sber_indirect_ifield_SFST_get ( sber_indirect_SF_BERM_SET_TH_buffer_t *b_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000015 SF_BERM_CLEAR_TH |
 | +--------------------------------------+
 | | bit  11:0 R/W  SFCT                  |
 | +--------------------------------------+
 | 
 | void   sber_indirect_ifield_SFCT_set ( sber_indirect_SF_BERM_CLEAR_TH_buffer_t *b_ptr, UINT32 value )
 | UINT32 sber_indirect_ifield_SFCT_get ( sber_indirect_SF_BERM_CLEAR_TH_buffer_t *b_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000016 SF_PATH_MONING_EN |
 | +---------------------------------------+
 | | bit  0 R/W  SF_EN                     |
 | +---------------------------------------+
 | 
 | void   sber_indirect_ifield_SF_EN_set ( sber_indirect_SF_PATH_MONING_EN_buffer_t *b_ptr, UINT32 value )
 | UINT32 sber_indirect_ifield_SF_EN_get ( sber_indirect_SF_PATH_MONING_EN_buffer_t *b_ptr )
 | 
 | +------------------------------------------------------+
 | | Register 0x00000017 SF_BERM_SET_INTEGRATION_PERIOD_2 |
 | +------------------------------------------------------+
 | | bit  15:0 R/W  SFIP                                  |
 | +------------------------------------------------------+
 | 
 | void   sber_indirect_ifield_SFIP_set ( sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr, UINT32 value )
 | UINT32 sber_indirect_ifield_SFIP_get ( sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_t *b_ptr )
 +----------------------------------------------------------------------------------------------------------------------*/
#ifndef _SBER_INDIRECT_IO_H
#define _SBER_INDIRECT_IO_H

#include "sdh_pmg_api.h"
#include "sber_indirect_regs.h"
#include "sber_indirect_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sber_indirect_buffer_init               ( sber_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 sber_indirect_buffer_flush              ( sber_indirect_buffer_t *b_ptr )
 | UINT32               sber_indirect_reg_read                  ( sber_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 sber_indirect_reg_write                 ( sber_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 sber_indirect_field_set                 ( sber_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 sber_indirect_action_on_write_field_set ( sber_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 sber_indirect_burst_read                ( sber_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 sber_indirect_burst_write               ( sber_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE sber_indirect_poll                      ( sber_indirect_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SBER_INDIRECT_IO_H */
