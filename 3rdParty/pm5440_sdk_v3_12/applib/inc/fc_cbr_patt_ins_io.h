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
 *     and register accessor functions for the fc_cbr_patt_ins block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing fc_cbr_patt_ins io functions is:
 | {
 |     fc_cbr_patt_ins_buffer_t b_ptr[1];
 |     fc_cbr_patt_ins_buffer_init( b_ptr, h_ptr, A );
 |     value = fc_cbr_patt_ins_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     fc_cbr_patt_ins_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     fc_cbr_patt_ins_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = fc_cbr_patt_ins_field_<FIELD>_get( NULL, h_ptr, A );
 | fc_cbr_patt_ins_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +------------------------------------+
 | | Register 0x00000080 CFG            |
 | +------------------------------------+
 | | bit  26:24 R/W  FAULT_SIZE         |
 | | bit  21:20 R/W  FAULT_MODE         |
 | | bit  18:16 R/W  BUS_WIDTH          |
 | | bit  12    R/W  PN11_INV           |
 | | bit  8     R/W  SCRAM_EN           |
 | | bit  6:5   R/W  ZONE_LEVEL         |
 | | bit  4     R/W  FORCE_FAULT        |
 | | bit  1     R/W  FC_CBR_PATT_INS_EN |
 | | bit  0     R/W  SOFT_RST           |
 | +------------------------------------+
 | 
 | void   fc_cbr_patt_ins_field_FAULT_SIZE_set         ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_FAULT_SIZE_get         ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_patt_ins_field_range_FAULT_SIZE_set   ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_range_FAULT_SIZE_get   ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc_cbr_patt_ins_field_FAULT_MODE_set         ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_FAULT_MODE_get         ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_patt_ins_field_range_FAULT_MODE_set   ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_range_FAULT_MODE_get   ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc_cbr_patt_ins_field_BUS_WIDTH_set          ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_BUS_WIDTH_get          ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_patt_ins_field_range_BUS_WIDTH_set    ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_range_BUS_WIDTH_get    ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc_cbr_patt_ins_field_PN11_INV_set           ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_PN11_INV_get           ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_patt_ins_field_SCRAM_EN_set           ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_SCRAM_EN_get           ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_patt_ins_field_ZONE_LEVEL_set         ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_ZONE_LEVEL_get         ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_patt_ins_field_range_ZONE_LEVEL_set   ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_range_ZONE_LEVEL_get   ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc_cbr_patt_ins_field_FORCE_FAULT_set        ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_FORCE_FAULT_get        ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_patt_ins_field_FC_CBR_PATT_INS_EN_set ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_FC_CBR_PATT_INS_EN_get ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_patt_ins_field_SOFT_RST_set           ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_SOFT_RST_get           ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------+
 | | Register 0x00000084 UPDATE    |
 | +-------------------------------+
 | | bit  31 R    TIP              |
 | | bit  4  R/W  UPDATE_PMON      |
 | | bit  0  R/W  UPDATE_USER_PATT |
 | +-------------------------------+
 | 
 | void                 fc_cbr_patt_ins_field_UPDATE_PMON_set      ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_patt_ins_field_UPDATE_PMON_get      ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void                 fc_cbr_patt_ins_field_UPDATE_USER_PATT_set ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_patt_ins_field_UPDATE_USER_PATT_get ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | UINT32               fc_cbr_patt_ins_field_TIP_get              ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_TIP_poll             ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x00000088 USER_FLT_PATT_31_0 |
 | +----------------------------------------+
 | | bit  31:0 R/W  USER_FLT_PATT           |
 | +----------------------------------------+
 | 
 | void   fc_cbr_patt_ins_lfield_USER_FLT_PATT_set       ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value[5] )
 | void   fc_cbr_patt_ins_lfield_USER_FLT_PATT_get       ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value[5] )
 | void   fc_cbr_patt_ins_lfield_range_USER_FLT_PATT_set ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_patt_ins_lfield_range_USER_FLT_PATT_get ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0000009c USER_RCVRY_PATT_31_0 |
 | +------------------------------------------+
 | | bit  31:0 R/W  USER_RCVRY_PATT           |
 | +------------------------------------------+
 | 
 | void   fc_cbr_patt_ins_lfield_USER_RCVRY_PATT_set       ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | void   fc_cbr_patt_ins_lfield_USER_RCVRY_PATT_get       ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | void   fc_cbr_patt_ins_lfield_range_USER_RCVRY_PATT_set ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_patt_ins_lfield_range_USER_RCVRY_PATT_get ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x000000a4 INTERRUPT_ENA          |
 | +--------------------------------------------+
 | | bit  24 R/W  RX_PRBS31_ALL_ONES_OR_ZEROS_E |
 | | bit  20 R/W  RX_PRBS31_ERR_E               |
 | | bit  16 R/W  RX_PN11_ERR_E                 |
 | | bit  12 R/W  TX_PN11_ERR_E                 |
 | | bit  8  R/W  RX_PN11_SYNC_E                |
 | | bit  4  R/W  TX_PN11_SYNC_E                |
 | | bit  0  R/W  UNDERRUN_E                    |
 | +--------------------------------------------+
 | 
 | void   fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_E_set ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_E_get ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_patt_ins_field_RX_PRBS31_ERR_E_set               ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ERR_E_get               ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_patt_ins_field_RX_PN11_ERR_E_set                 ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_RX_PN11_ERR_E_get                 ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_patt_ins_field_TX_PN11_ERR_E_set                 ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_TX_PN11_ERR_E_get                 ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_patt_ins_field_RX_PN11_SYNC_E_set                ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_RX_PN11_SYNC_E_get                ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_patt_ins_field_TX_PN11_SYNC_E_set                ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_TX_PN11_SYNC_E_get                ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_patt_ins_field_UNDERRUN_E_set                    ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_patt_ins_field_UNDERRUN_E_get                    ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------+
 | | Register 0x000000a8 INT_INDICATION         |
 | +--------------------------------------------+
 | | bit  24 R/W  RX_PRBS31_ALL_ONES_OR_ZEROS_I |
 | | bit  20 R/W  RX_PRBS31_ERR_I               |
 | | bit  16 R/W  RX_PN11_ERR_I                 |
 | | bit  12 R/W  TX_PN11_ERR_I                 |
 | | bit  8  R/W  RX_PN11_SYNC_I                |
 | | bit  4  R/W  TX_PN11_SYNC_I                |
 | | bit  0  R/W  UNDERRUN_I                    |
 | +--------------------------------------------+
 | 
 | void                 fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_set_to_clear ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_get          ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_I_poll         ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_cbr_patt_ins_field_RX_PN11_SYNC_I_set_to_clear                ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_patt_ins_field_RX_PN11_SYNC_I_get                         ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PN11_SYNC_I_poll                        ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_cbr_patt_ins_field_TX_PN11_SYNC_I_set_to_clear                ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_patt_ins_field_TX_PN11_SYNC_I_get                         ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_TX_PN11_SYNC_I_poll                        ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_cbr_patt_ins_field_UNDERRUN_I_set_to_clear                    ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_patt_ins_field_UNDERRUN_I_get                             ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_UNDERRUN_I_poll                            ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_set_to_clear               ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_get                        ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PRBS31_ERR_I_poll                       ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_cbr_patt_ins_field_RX_PN11_ERR_I_set_to_clear                 ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_patt_ins_field_RX_PN11_ERR_I_get                          ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PN11_ERR_I_poll                         ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_cbr_patt_ins_field_TX_PN11_ERR_I_set_to_clear                 ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_patt_ins_field_TX_PN11_ERR_I_get                          ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_TX_PN11_ERR_I_poll                         ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x000000ac PN11_SYNC_STAT       |
 | +------------------------------------------+
 | | bit  24 R  RX_PRBS31_ALL_ONES_OR_ZEROS_V |
 | | bit  8  R  RX_PN11_SYNC_V                |
 | | bit  4  R  TX_PN11_SYNC_V                |
 | | bit  0  R  UNDERRUN_V                    |
 | +------------------------------------------+
 | 
 | UINT32               fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_V_get  ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PRBS31_ALL_ONES_OR_ZEROS_V_poll ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc_cbr_patt_ins_field_RX_PN11_SYNC_V_get                 ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_RX_PN11_SYNC_V_poll                ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc_cbr_patt_ins_field_TX_PN11_SYNC_V_get                 ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_TX_PN11_SYNC_V_poll                ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc_cbr_patt_ins_field_UNDERRUN_V_get                     ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_field_UNDERRUN_V_poll                    ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x000000b0 TX_PN11_ERR_CNT |
 | +-------------------------------------+
 | | bit  15:0 R  TX_PN11_ERR_CNT        |
 | +-------------------------------------+
 | 
 | UINT32 fc_cbr_patt_ins_field_TX_PN11_ERR_CNT_get       ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | UINT32 fc_cbr_patt_ins_field_range_TX_PN11_ERR_CNT_get ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000b4 RX_PN11_ERR_CNT |
 | +-------------------------------------+
 | | bit  15:0 R  RX_PN11_ERR_CNT        |
 | +-------------------------------------+
 | 
 | UINT32 fc_cbr_patt_ins_field_RX_PN11_ERR_CNT_get       ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | UINT32 fc_cbr_patt_ins_field_range_RX_PN11_ERR_CNT_get ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x000000b8 RX_PRBS31_ERR_CNT |
 | +---------------------------------------+
 | | bit  15:0 R  RX_PRBS31_ERR_CNT        |
 | +---------------------------------------+
 | 
 | UINT32 fc_cbr_patt_ins_field_RX_PRBS31_ERR_CNT_get       ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | UINT32 fc_cbr_patt_ins_field_range_RX_PRBS31_ERR_CNT_get ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _FC_CBR_PATT_INS_IO_H
#define _FC_CBR_PATT_INS_IO_H

#include "cbr_fsgm_api.h"
#include "fc_cbr_patt_ins_regs.h"
#include "fc_cbr_patt_ins_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fc_cbr_patt_ins_buffer_init               ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | void                 fc_cbr_patt_ins_buffer_flush              ( fc_cbr_patt_ins_buffer_t *b_ptr )
 | UINT32               fc_cbr_patt_ins_reg_read                  ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 fc_cbr_patt_ins_reg_write                 ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 fc_cbr_patt_ins_field_set                 ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 fc_cbr_patt_ins_action_on_write_field_set ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 fc_cbr_patt_ins_burst_read                ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 fc_cbr_patt_ins_burst_write               ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_poll                      ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fc_cbr_patt_ins_reg_CFG_write                                ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_patt_ins_reg_CFG_field_set                            ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_patt_ins_reg_CFG_read                                 ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_write                 ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_field_set             ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_patt_ins_reg_USER_FLT_PATT_31_0_read                  ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_write                ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_field_set            ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_patt_ins_reg_USER_FLT_PATT_63_32_read                 ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_write                ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_field_set            ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_patt_ins_reg_USER_FLT_PATT_95_64_read                 ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_write               ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_field_set           ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_patt_ins_reg_USER_FLT_PATT_127_96_read                ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_write           ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_field_set       ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_patt_ins_reg_USER_FLT_PATTERN_159_128_read            ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_write               ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_field_set           ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_patt_ins_reg_USER_RCVRY_PATT_31_0_read                ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_write              ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_field_set          ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_patt_ins_reg_USER_RCVRY_PATT_39_32_read               ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_patt_ins_reg_UPDATE_write                             ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_patt_ins_reg_UPDATE_field_set                         ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_patt_ins_reg_UPDATE_read                              ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_reg_UPDATE_poll                              ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 fc_cbr_patt_ins_reg_INT_INDICATION_write                     ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_patt_ins_reg_INT_INDICATION_action_on_write_field_set ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_patt_ins_reg_INT_INDICATION_read                      ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_reg_INT_INDICATION_poll                      ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 fc_cbr_patt_ins_reg_INTERRUPT_ENA_write                      ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_patt_ins_reg_INTERRUPT_ENA_field_set                  ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_patt_ins_reg_INTERRUPT_ENA_read                       ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc_cbr_patt_ins_reg_PN11_SYNC_STAT_read                      ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_patt_ins_reg_PN11_SYNC_STAT_poll                      ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               fc_cbr_patt_ins_reg_TX_PN11_ERR_CNT_read                     ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc_cbr_patt_ins_reg_RX_PN11_ERR_CNT_read                     ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc_cbr_patt_ins_reg_RX_PRBS31_ERR_CNT_read                   ( fc_cbr_patt_ins_buffer_t *b_ptr, cbr_fsgm_handle_t *h_ptr, UINT32 A )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _FC_CBR_PATT_INS_IO_H */
