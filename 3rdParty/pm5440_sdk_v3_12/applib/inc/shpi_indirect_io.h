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
 *     and register accessor functions for the shpi_indirect block
 *****************************************************************************/

/*-----------------------------------------------------------------------------------------.
 | The pattern for reading indirect registers is:
 | {
 |     shpi_indirect_CFG_buffer_t b_ptr[1];
 |     shpi_indirect_CFG_buffer_init(b_ptr);
 |     shpi_indirect_CFG_buffer_retrieve(h_ptr, some_index, some_other_index, b_ptr);
 |     value = shpi_indirect_<FIELD1>_get(b_ptr);
 |     value = shpi_indirect_<FIELD2>_get(b_ptr);
 | ...
 | 
 | The pattern for writing indirect registers is:
 | {
 |     shpi_indirect_IND_INT_STAT_buffer_t b_ptr[1];
 |     IND_INT_STAT_buffer_init(b_ptr);
 |     value = shpi_indirect_<FIELD1>_set(b_ptr, value1);
 |     value = shpi_indirect_<FIELD2>_set(b_ptr, value2);
 | ...
 |     shpi_indirect_IND_INT_STAT_buffer_applyh_ptr, some_index, some_other_index, b_ptr);
 | }
 | 
 | note: the indexing arguments are defined by the user.
 | 
 +----------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------+
 | | Register 0x00000000 IND_INT_VAL |
 | +---------------------------------+
 | | bit  5 R  PAISCV                |
 | | bit  4 R  PLOPCV                |
 | | bit  3 R  PAISV                 |
 | | bit  2 R  PLOPV                 |
 | +---------------------------------+
 | 
 | UINT32 shpi_indirect_ifield_PAISCV_get ( shpi_indirect_IND_INT_VAL_buffer_t *b_ptr )
 | UINT32 shpi_indirect_ifield_PLOPCV_get ( shpi_indirect_IND_INT_VAL_buffer_t *b_ptr )
 | UINT32 shpi_indirect_ifield_PAISV_get  ( shpi_indirect_IND_INT_VAL_buffer_t *b_ptr )
 | UINT32 shpi_indirect_ifield_PLOPV_get  ( shpi_indirect_IND_INT_VAL_buffer_t *b_ptr )
 | 
 | +----------------------------------+
 | | Register 0x00000002 IND_INT_STAT |
 | +----------------------------------+
 | | bit  5 R/W  PAISCI               |
 | | bit  4 R/W  PLOPCI               |
 | | bit  3 R/W  PAISI                |
 | | bit  2 R/W  PLOPI                |
 | +----------------------------------+
 | 
 | void   shpi_indirect_ifield_PAISCI_set_to_clear ( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr, UINT32 value )
 | UINT32 shpi_indirect_ifield_PAISCI_get          ( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr )
 | void   shpi_indirect_ifield_PLOPCI_set_to_clear ( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr, UINT32 value )
 | UINT32 shpi_indirect_ifield_PLOPCI_get          ( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr )
 | void   shpi_indirect_ifield_PAISI_set_to_clear  ( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr, UINT32 value )
 | UINT32 shpi_indirect_ifield_PAISI_get           ( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr )
 | void   shpi_indirect_ifield_PLOPI_set_to_clear  ( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr, UINT32 value )
 | UINT32 shpi_indirect_ifield_PLOPI_get           ( shpi_indirect_IND_INT_STAT_buffer_t *b_ptr )
 | 
 | +-------------------------+
 | | Register 0x00000003 CFG |
 | +-------------------------+
 | | bit  4 R/W  NDFCNT      |
 | | bit  0 R/W  SSEN        |
 | +-------------------------+
 | 
 | void   shpi_indirect_ifield_NDFCNT_set ( shpi_indirect_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 shpi_indirect_ifield_NDFCNT_get ( shpi_indirect_CFG_buffer_t *b_ptr )
 | void   shpi_indirect_ifield_SSEN_set   ( shpi_indirect_CFG_buffer_t *b_ptr, UINT32 value )
 | UINT32 shpi_indirect_ifield_SSEN_get   ( shpi_indirect_CFG_buffer_t *b_ptr )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00000004 PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT |
 | +-----------------------------------------------------------+
 | | bit  12:0 R  PNJE                                         |
 | +-----------------------------------------------------------+
 | 
 | UINT32 shpi_indirect_ifield_PNJE_get ( shpi_indirect_PATH_NEGATIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00000005 PATH_POSITIVE_JUSTIFICATION_EVENT_CNT |
 | +-----------------------------------------------------------+
 | | bit  12:0 R  PPJE                                         |
 | +-----------------------------------------------------------+
 | 
 | UINT32 shpi_indirect_ifield_PPJE_get ( shpi_indirect_PATH_POSITIVE_JUSTIFICATION_EVENT_CNT_buffer_t *b_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00000006 POINTER_VAL |
 | +---------------------------------+
 | | bit  15:12 R  NDF               |
 | | bit  11:10 R  SSV               |
 | | bit  9:0   R  PTRV              |
 | +---------------------------------+
 | 
 | UINT32 shpi_indirect_ifield_NDF_get  ( shpi_indirect_POINTER_VAL_buffer_t *b_ptr )
 | UINT32 shpi_indirect_ifield_SSV_get  ( shpi_indirect_POINTER_VAL_buffer_t *b_ptr )
 | UINT32 shpi_indirect_ifield_PTRV_get ( shpi_indirect_POINTER_VAL_buffer_t *b_ptr )
 +-------------------------------------------------------------------------------------------------------------*/
#ifndef _SHPI_INDIRECT_IO_H
#define _SHPI_INDIRECT_IO_H

#include "cbrc_api.h"
#include "shpi_indirect_regs.h"
#include "shpi_indirect_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 shpi_indirect_buffer_init               ( shpi_indirect_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 shpi_indirect_buffer_flush              ( shpi_indirect_buffer_t *b_ptr )
 | UINT32               shpi_indirect_reg_read                  ( shpi_indirect_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 shpi_indirect_reg_write                 ( shpi_indirect_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 shpi_indirect_field_set                 ( shpi_indirect_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 shpi_indirect_action_on_write_field_set ( shpi_indirect_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 shpi_indirect_burst_read                ( shpi_indirect_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 shpi_indirect_burst_write               ( shpi_indirect_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE shpi_indirect_poll                      ( shpi_indirect_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SHPI_INDIRECT_IO_H */
