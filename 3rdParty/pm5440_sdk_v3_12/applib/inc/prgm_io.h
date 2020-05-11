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
 *     and register accessor functions for the prgm block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing prgm io functions is:
 | {
 |     prgm_buffer_t b_ptr[1];
 |     prgm_buffer_init( b_ptr, h_ptr, A );
 |     value = prgm_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     prgm_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     prgm_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = prgm_field_<FIELD>_get( NULL, h_ptr, A );
 | prgm_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------+
 | | Register 0x00001440 IND_ADR |
 | +-----------------------------+
 | | bit  15  R    BUSY          |
 | | bit  14  R/W  RWB           |
 | | bit  9:6 R/W  IADDR         |
 | | bit  3:0 R/W  PATH          |
 | +-----------------------------+
 | 
 | void                 prgm_field_RWB_set         ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_RWB_get         ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 prgm_field_IADDR_set       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_IADDR_get       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 prgm_field_range_IADDR_set ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               prgm_field_range_IADDR_get ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void                 prgm_field_PATH_set        ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_PATH_get        ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 prgm_field_range_PATH_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               prgm_field_range_PATH_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32               prgm_field_BUSY_get        ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_BUSY_poll       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------+
 | | Register 0x00001444 IND_DATA |
 | +------------------------------+
 | | bit  15:0 R/W  DATA          |
 | +------------------------------+
 | 
 | void   prgm_field_DATA_set       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_DATA_get       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_range_DATA_set ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 prgm_field_range_DATA_get ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00001448 GENERATOR_PAYLOAD_CFG |
 | +-------------------------------------------+
 | | bit  15   R/W  GEN_STS12CSL               |
 | | bit  14   R/W  GEN_STS12C                 |
 | | bit  10:8 R/W  GEN_MSSLEN                 |
 | +-------------------------------------------+
 | 
 | void   prgm_field_GEN_STS12CSL_set     ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_GEN_STS12CSL_get     ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_GEN_STS12C_set       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_GEN_STS12C_get       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_GEN_MSSLEN_set       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_GEN_MSSLEN_get       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_range_GEN_MSSLEN_set ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 prgm_field_range_GEN_MSSLEN_get ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x0000144c MON_PAYLOAD_CFG |
 | +-------------------------------------+
 | | bit  15   R/W  MON_STS12CSL         |
 | | bit  14   R/W  MON_STS12C           |
 | | bit  10:8 R/W  MON_MSSLEN           |
 | +-------------------------------------+
 | 
 | void   prgm_field_MON_STS12CSL_set     ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON_STS12CSL_get     ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON_STS12C_set       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON_STS12C_get       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON_MSSLEN_set       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON_MSSLEN_get       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_range_MON_MSSLEN_set ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 prgm_field_range_MON_MSSLEN_get ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00001450 MON_BYTE_ERR_INT_STAT |
 | +-------------------------------------------+
 | | bit  11 R/W  MON12_ERR_I                  |
 | | bit  10 R/W  MON11_ERR_I                  |
 | | bit  9  R/W  MON10_ERR_I                  |
 | | bit  8  R/W  MON9_ERR_I                   |
 | | bit  7  R/W  MON8_ERR_I                   |
 | | bit  6  R/W  MON7_ERR_I                   |
 | | bit  5  R/W  MON6_ERR_I                   |
 | | bit  4  R/W  MON5_ERR_I                   |
 | | bit  3  R/W  MON4_ERR_I                   |
 | | bit  2  R/W  MON3_ERR_I                   |
 | | bit  1  R/W  MON2_ERR_I                   |
 | | bit  0  R/W  MON1_ERR_I                   |
 | +-------------------------------------------+
 | 
 | void                 prgm_field_MON12_ERR_I_set_to_clear ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON12_ERR_I_get          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON12_ERR_I_poll         ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON11_ERR_I_set_to_clear ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON11_ERR_I_get          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON11_ERR_I_poll         ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON10_ERR_I_set_to_clear ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON10_ERR_I_get          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON10_ERR_I_poll         ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON9_ERR_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON9_ERR_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON9_ERR_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON8_ERR_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON8_ERR_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON8_ERR_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON7_ERR_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON7_ERR_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON7_ERR_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON6_ERR_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON6_ERR_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON6_ERR_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON5_ERR_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON5_ERR_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON5_ERR_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON4_ERR_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON4_ERR_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON4_ERR_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON3_ERR_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON3_ERR_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON3_ERR_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON2_ERR_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON2_ERR_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON2_ERR_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON1_ERR_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON1_ERR_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON1_ERR_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x00001454 MON_BYTE_ERR_INT_EN |
 | +-----------------------------------------+
 | | bit  11 R/W  MON12_ERR_E                |
 | | bit  10 R/W  MON11_ERR_E                |
 | | bit  9  R/W  MON10_ERR_E                |
 | | bit  8  R/W  MON9_ERR_E                 |
 | | bit  7  R/W  MON8_ERR_E                 |
 | | bit  6  R/W  MON7_ERR_E                 |
 | | bit  5  R/W  MON6_ERR_E                 |
 | | bit  4  R/W  MON5_ERR_E                 |
 | | bit  3  R/W  MON4_ERR_E                 |
 | | bit  2  R/W  MON3_ERR_E                 |
 | | bit  1  R/W  MON2_ERR_E                 |
 | | bit  0  R/W  MON1_ERR_E                 |
 | +-----------------------------------------+
 | 
 | void   prgm_field_MON12_ERR_E_set ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON12_ERR_E_get ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON11_ERR_E_set ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON11_ERR_E_get ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON10_ERR_E_set ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON10_ERR_E_get ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON9_ERR_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON9_ERR_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON8_ERR_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON8_ERR_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON7_ERR_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON7_ERR_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON6_ERR_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON6_ERR_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON5_ERR_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON5_ERR_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON4_ERR_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON4_ERR_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON3_ERR_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON3_ERR_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON2_ERR_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON2_ERR_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON1_ERR_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON1_ERR_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------+
 | | Register 0x00001464 MON_SYNC_INT_STAT |
 | +---------------------------------------+
 | | bit  11 R/W  MON12_SYNC_I             |
 | | bit  10 R/W  MON11_SYNC_I             |
 | | bit  9  R/W  MON10_SYNC_I             |
 | | bit  8  R/W  MON9_SYNC_I              |
 | | bit  7  R/W  MON8_SYNC_I              |
 | | bit  6  R/W  MON7_SYNC_I              |
 | | bit  5  R/W  MON6_SYNC_I              |
 | | bit  4  R/W  MON5_SYNC_I              |
 | | bit  3  R/W  MON4_SYNC_I              |
 | | bit  2  R/W  MON3_SYNC_I              |
 | | bit  1  R/W  MON2_SYNC_I              |
 | | bit  0  R/W  MON1_SYNC_I              |
 | +---------------------------------------+
 | 
 | void                 prgm_field_MON12_SYNC_I_set_to_clear ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON12_SYNC_I_get          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON12_SYNC_I_poll         ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON11_SYNC_I_set_to_clear ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON11_SYNC_I_get          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON11_SYNC_I_poll         ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON10_SYNC_I_set_to_clear ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON10_SYNC_I_get          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON10_SYNC_I_poll         ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON9_SYNC_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON9_SYNC_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON9_SYNC_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON8_SYNC_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON8_SYNC_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON8_SYNC_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON7_SYNC_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON7_SYNC_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON7_SYNC_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON6_SYNC_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON6_SYNC_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON6_SYNC_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON5_SYNC_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON5_SYNC_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON5_SYNC_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON4_SYNC_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON4_SYNC_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON4_SYNC_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON3_SYNC_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON3_SYNC_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON3_SYNC_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON2_SYNC_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON2_SYNC_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON2_SYNC_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 prgm_field_MON1_SYNC_I_set_to_clear  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_MON1_SYNC_I_get           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_MON1_SYNC_I_poll          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00001468 MON_SYNC_INT_EN |
 | +-------------------------------------+
 | | bit  11 R/W  MON12_SYNC_E           |
 | | bit  10 R/W  MON11_SYNC_E           |
 | | bit  9  R/W  MON10_SYNC_E           |
 | | bit  8  R/W  MON9_SYNC_E            |
 | | bit  7  R/W  MON8_SYNC_E            |
 | | bit  6  R/W  MON7_SYNC_E            |
 | | bit  5  R/W  MON6_SYNC_E            |
 | | bit  4  R/W  MON5_SYNC_E            |
 | | bit  3  R/W  MON4_SYNC_E            |
 | | bit  2  R/W  MON3_SYNC_E            |
 | | bit  1  R/W  MON2_SYNC_E            |
 | | bit  0  R/W  MON1_SYNC_E            |
 | +-------------------------------------+
 | 
 | void   prgm_field_MON12_SYNC_E_set ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON12_SYNC_E_get ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON11_SYNC_E_set ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON11_SYNC_E_get ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON10_SYNC_E_set ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON10_SYNC_E_get ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON9_SYNC_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON9_SYNC_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON8_SYNC_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON8_SYNC_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON7_SYNC_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON7_SYNC_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON6_SYNC_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON6_SYNC_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON5_SYNC_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON5_SYNC_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON4_SYNC_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON4_SYNC_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON3_SYNC_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON3_SYNC_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON2_SYNC_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON2_SYNC_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   prgm_field_MON1_SYNC_E_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 prgm_field_MON1_SYNC_E_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x0000146c MON_SYNC_STAT |
 | +-----------------------------------+
 | | bit  11 R  MON12_SYNC_V           |
 | | bit  10 R  MON11_SYNC_V           |
 | | bit  9  R  MON10_SYNC_V           |
 | | bit  8  R  MON9_SYNC_V            |
 | | bit  7  R  MON8_SYNC_V            |
 | | bit  6  R  MON7_SYNC_V            |
 | | bit  5  R  MON6_SYNC_V            |
 | | bit  4  R  MON5_SYNC_V            |
 | | bit  3  R  MON4_SYNC_V            |
 | | bit  2  R  MON3_SYNC_V            |
 | | bit  1  R  MON2_SYNC_V            |
 | | bit  0  R  MON1_SYNC_V            |
 | +-----------------------------------+
 | 
 | UINT32 prgm_field_MON12_SYNC_V_get ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 prgm_field_MON11_SYNC_V_get ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 prgm_field_MON10_SYNC_V_get ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 prgm_field_MON9_SYNC_V_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 prgm_field_MON8_SYNC_V_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 prgm_field_MON7_SYNC_V_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 prgm_field_MON6_SYNC_V_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 prgm_field_MON5_SYNC_V_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 prgm_field_MON4_SYNC_V_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 prgm_field_MON3_SYNC_V_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 prgm_field_MON2_SYNC_V_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 prgm_field_MON1_SYNC_V_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------+
 | | Register 0x00001470 PERF_CNT_TRANSFER_TRIG |
 | +--------------------------------------------+
 | | bit  0 R/W  TIP                            |
 | +--------------------------------------------+
 | 
 | void                 prgm_field_TIP_set  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               prgm_field_TIP_get  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_field_TIP_poll ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _PRGM_IO_H
#define _PRGM_IO_H

#include "cbrc_api.h"
#include "prgm_regs.h"
#include "prgm_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 prgm_buffer_init               ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 prgm_buffer_flush              ( prgm_buffer_t *b_ptr )
 | UINT32               prgm_reg_read                  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 prgm_reg_write                 ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 prgm_field_set                 ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 prgm_action_on_write_field_set ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 prgm_burst_read                ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 prgm_burst_write               ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE prgm_poll                      ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 prgm_reg_IND_ADR_write                                   ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 prgm_reg_IND_ADR_field_set                               ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               prgm_reg_IND_ADR_read                                    ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_reg_IND_ADR_poll                                    ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 prgm_reg_IND_DATA_write                                  ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 prgm_reg_IND_DATA_field_set                              ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               prgm_reg_IND_DATA_read                                   ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 prgm_reg_GENERATOR_PAYLOAD_CFG_write                     ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 prgm_reg_GENERATOR_PAYLOAD_CFG_field_set                 ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               prgm_reg_GENERATOR_PAYLOAD_CFG_read                      ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 prgm_reg_MON_PAYLOAD_CFG_write                           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 prgm_reg_MON_PAYLOAD_CFG_field_set                       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               prgm_reg_MON_PAYLOAD_CFG_read                            ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 prgm_reg_MON_BYTE_ERR_INT_EN_write                       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 prgm_reg_MON_BYTE_ERR_INT_EN_field_set                   ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               prgm_reg_MON_BYTE_ERR_INT_EN_read                        ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 prgm_reg_MON_BYTE_ERR_INT_STAT_write                     ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 prgm_reg_MON_BYTE_ERR_INT_STAT_action_on_write_field_set ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               prgm_reg_MON_BYTE_ERR_INT_STAT_read                      ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_reg_MON_BYTE_ERR_INT_STAT_poll                      ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 prgm_reg_MON_SYNC_INT_STAT_write                         ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 prgm_reg_MON_SYNC_INT_STAT_action_on_write_field_set     ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               prgm_reg_MON_SYNC_INT_STAT_read                          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_reg_MON_SYNC_INT_STAT_poll                          ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 prgm_reg_MON_SYNC_INT_EN_write                           ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 prgm_reg_MON_SYNC_INT_EN_field_set                       ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               prgm_reg_MON_SYNC_INT_EN_read                            ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               prgm_reg_MON_SYNC_STAT_read                              ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 prgm_reg_PERF_CNT_TRANSFER_TRIG_write                    ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 prgm_reg_PERF_CNT_TRANSFER_TRIG_field_set                ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               prgm_reg_PERF_CNT_TRANSFER_TRIG_read                     ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE prgm_reg_PERF_CNT_TRANSFER_TRIG_poll                     ( prgm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _PRGM_IO_H */
