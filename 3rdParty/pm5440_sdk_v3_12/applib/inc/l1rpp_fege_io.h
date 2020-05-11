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
 *     and register accessor functions for the l1rpp_fege block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing l1rpp_fege io functions is:
 | {
 |     l1rpp_fege_buffer_t b_ptr[1];
 |     l1rpp_fege_buffer_init( b_ptr, h_ptr );
 |     value = l1rpp_fege_field_<FIELD>_get( b_ptr, h_ptr );
 |     l1rpp_fege_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     l1rpp_fege_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = l1rpp_fege_field_<FIELD>_get( NULL, h_ptr );
 | l1rpp_fege_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------------+
 | | Register 0x00000080 INT              |
 | +--------------------------------------+
 | | bit  15 R/W  LOS_I                   |
 | | bit  14 R/W  SDET_IN_I               |
 | | bit  13 R/W  ALIGN_CHANGE_I          |
 | | bit  12 R/W  LINK_MON_I              |
 | | bit  11 R/W  FEC_STAT_I              |
 | | bit  10 R/W  CARRIER_STAT_I          |
 | | bit  9  R/W  RX_ERR_I                |
 | | bit  8  R/W  LCV_I                   |
 | | bit  7  R/W  AUTO_NEG_LP_ABLTY_UPD_I |
 | | bit  4  R/W  LINK_OK_I               |
 | | bit  0  R/W  SYNC_OK_I               |
 | +--------------------------------------+
 | 
 | void                 l1rpp_fege_field_LINK_MON_I_set_to_clear              ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               l1rpp_fege_field_LINK_MON_I_get                       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_LINK_MON_I_poll                      ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 l1rpp_fege_field_FEC_STAT_I_set_to_clear              ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               l1rpp_fege_field_FEC_STAT_I_get                       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_FEC_STAT_I_poll                      ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 l1rpp_fege_field_CARRIER_STAT_I_set_to_clear          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               l1rpp_fege_field_CARRIER_STAT_I_get                   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_CARRIER_STAT_I_poll                  ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 l1rpp_fege_field_LINK_OK_I_set_to_clear               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               l1rpp_fege_field_LINK_OK_I_get                        ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_LINK_OK_I_poll                       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 l1rpp_fege_field_SYNC_OK_I_set_to_clear               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               l1rpp_fege_field_SYNC_OK_I_get                        ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_SYNC_OK_I_poll                       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 l1rpp_fege_field_LOS_I_set_to_clear                   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               l1rpp_fege_field_LOS_I_get                            ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_LOS_I_poll                           ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 l1rpp_fege_field_SDET_IN_I_set_to_clear               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               l1rpp_fege_field_SDET_IN_I_get                        ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_SDET_IN_I_poll                       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 l1rpp_fege_field_ALIGN_CHANGE_I_set_to_clear          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               l1rpp_fege_field_ALIGN_CHANGE_I_get                   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_ALIGN_CHANGE_I_poll                  ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 l1rpp_fege_field_RX_ERR_I_set_to_clear                ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               l1rpp_fege_field_RX_ERR_I_get                         ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_RX_ERR_I_poll                        ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 l1rpp_fege_field_LCV_I_set_to_clear                   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               l1rpp_fege_field_LCV_I_get                            ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_LCV_I_poll                           ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_I_set_to_clear ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_I_get          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_I_poll         ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------+
 | | Register 0x00000084 INT_EN           |
 | +--------------------------------------+
 | | bit  15 R/W  LOS_E                   |
 | | bit  14 R/W  SDET_IN_E               |
 | | bit  13 R/W  ALIGN_CHANGE_E          |
 | | bit  12 R/W  LINK_MON_E              |
 | | bit  11 R/W  FEC_STAT_E              |
 | | bit  10 R/W  CARRIER_STAT_E          |
 | | bit  9  R/W  RX_ERR_E                |
 | | bit  8  R/W  LCV_E                   |
 | | bit  7  R/W  AUTO_NEG_LP_ABLTY_UPD_E |
 | | bit  4  R/W  LINK_OK_E               |
 | | bit  0  R/W  SYNC_OK_E               |
 | +--------------------------------------+
 | 
 | void   l1rpp_fege_field_LOS_E_set                   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_LOS_E_get                   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_SDET_IN_E_set               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_SDET_IN_E_get               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_ALIGN_CHANGE_E_set          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_ALIGN_CHANGE_E_get          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_LINK_MON_E_set              ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_LINK_MON_E_get              ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_FEC_STAT_E_set              ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_FEC_STAT_E_get              ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_CARRIER_STAT_E_set          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_CARRIER_STAT_E_get          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_RX_ERR_E_set                ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_RX_ERR_E_get                ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_LCV_E_set                   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_LCV_E_get                   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_E_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_E_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_LINK_OK_E_set               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_LINK_OK_E_get               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_SYNC_OK_E_set               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_SYNC_OK_E_get               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000088 INT_STAT       |
 | +------------------------------------+
 | | bit  15 R  LOS_V                   |
 | | bit  14 R  SDET_IN_V               |
 | | bit  13 R  ALIGN_CHANGE_V          |
 | | bit  12 R  LINK_MON_V              |
 | | bit  11 R  FEC_STAT_V              |
 | | bit  10 R  CARRIER_STAT_V          |
 | | bit  7  R  AUTO_NEG_LP_ABLTY_UPD_V |
 | | bit  4  R  LINK_OK_V               |
 | | bit  0  R  SYNC_OK_V               |
 | +------------------------------------+
 | 
 | UINT32               l1rpp_fege_field_LOS_V_get                    ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_LOS_V_poll                   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               l1rpp_fege_field_SDET_IN_V_get                ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_SDET_IN_V_poll               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               l1rpp_fege_field_ALIGN_CHANGE_V_get           ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_ALIGN_CHANGE_V_poll          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               l1rpp_fege_field_LINK_MON_V_get               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_LINK_MON_V_poll              ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               l1rpp_fege_field_FEC_STAT_V_get               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_FEC_STAT_V_poll              ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               l1rpp_fege_field_CARRIER_STAT_V_get           ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_CARRIER_STAT_V_poll          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_V_get  ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_AUTO_NEG_LP_ABLTY_UPD_V_poll ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               l1rpp_fege_field_LINK_OK_V_get                ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_LINK_OK_V_poll               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               l1rpp_fege_field_SYNC_OK_V_get                ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_field_SYNC_OK_V_poll               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------+
 | | Register 0x0000008c CFG_AND_STAT |
 | +----------------------------------+
 | | bit  15  R    L1RPP_STATUS       |
 | | bit  13  R    SDET               |
 | | bit  12  R    ENCDET             |
 | | bit  11  R/W  AUTO_SENSE         |
 | | bit  10  R/W  MANUAL_ACK_EN      |
 | | bit  9   R/W  AN_ABLTY_ADV_UPD   |
 | | bit  6:4 R/W  MODE               |
 | | bit  0   R/W  L1RPP_EN           |
 | +----------------------------------+
 | 
 | void   l1rpp_fege_field_AUTO_SENSE_set       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_AUTO_SENSE_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_MANUAL_ACK_EN_set    ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_MANUAL_ACK_EN_get    ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_MODE_set             ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_MODE_get             ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_range_MODE_set       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1rpp_fege_field_range_MODE_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   l1rpp_fege_field_L1RPP_EN_set         ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_L1RPP_EN_get         ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_AN_ABLTY_ADV_UPD_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_AN_ABLTY_ADV_UPD_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_L1RPP_STATUS_get     ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_SDET_get             ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_ENCDET_get           ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x00000090 LINK_SYNC_SM_CONFIG |
 | +-----------------------------------------+
 | | bit  14   R/W  LS_RESET                 |
 | | bit  11:8 R/W  RESYNC_COUNT             |
 | | bit  7:4  R/W  LOS_COUNT                |
 | | bit  3:0  R/W  SYNC_COUNT               |
 | +-----------------------------------------+
 | 
 | void   l1rpp_fege_field_LS_RESET_set           ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_LS_RESET_get           ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_RESYNC_COUNT_set       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_RESYNC_COUNT_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_range_RESYNC_COUNT_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1rpp_fege_field_range_RESYNC_COUNT_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   l1rpp_fege_field_LOS_COUNT_set          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_LOS_COUNT_get          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_range_LOS_COUNT_set    ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1rpp_fege_field_range_LOS_COUNT_get    ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   l1rpp_fege_field_SYNC_COUNT_set         ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_SYNC_COUNT_get         ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_range_SYNC_COUNT_set   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1rpp_fege_field_range_SYNC_COUNT_get   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000094 JITTER_PATTERN_STAT_AND_CONFIG |
 | +----------------------------------------------------+
 | | bit  15    R    R_PRBS_IND                         |
 | | bit  14    R    PRBS_IND                           |
 | | bit  13    R    R_PTRN_IND                         |
 | | bit  12    R    PTRN_IND                           |
 | | bit  11:10 R/W  PTRN_SEL                           |
 | | bit  9:0   R/W  CUS_PTRN                           |
 | +----------------------------------------------------+
 | 
 | void   l1rpp_fege_field_PTRN_SEL_set       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_PTRN_SEL_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_range_PTRN_SEL_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1rpp_fege_field_range_PTRN_SEL_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   l1rpp_fege_field_CUS_PTRN_set       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_CUS_PTRN_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_range_CUS_PTRN_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1rpp_fege_field_range_CUS_PTRN_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 l1rpp_fege_field_R_PRBS_IND_get     ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_PRBS_IND_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_R_PTRN_IND_get     ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_PTRN_IND_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +------------------------------+
 | | Register 0x00000098 MII_CTRL |
 | +------------------------------+
 | | bit  12 R/W  AN_ENABLE       |
 | | bit  9  R/W  AN_RESTART      |
 | +------------------------------+
 | 
 | void   l1rpp_fege_field_AN_ENABLE_set  ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_AN_ENABLE_get  ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_AN_RESTART_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_AN_RESTART_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +------------------------------+
 | | Register 0x0000009c MII_STAT |
 | +------------------------------+
 | | bit  5 R  AN_COMPLETE        |
 | | bit  4 R  REM_FLT_IND        |
 | | bit  2 R  LNK_STATE          |
 | +------------------------------+
 | 
 | UINT32 l1rpp_fege_field_AN_COMPLETE_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_REM_FLT_IND_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_LNK_STATE_get   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +--------------------------------------------------------+
 | | Register 0x000000a0 MII_AUTO_NEGOTIATION_ADVERTISEMENT |
 | +--------------------------------------------------------+
 | | bit  15:0 R/W  AN_ABLTY_ADV                            |
 | +--------------------------------------------------------+
 | 
 | void   l1rpp_fege_field_AN_ABLTY_ADV_set       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_AN_ABLTY_ADV_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_range_AN_ABLTY_ADV_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1rpp_fege_field_range_AN_ABLTY_ADV_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------------+
 | | Register 0x000000a4 MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY |
 | +-------------------------------------------------------------------------+
 | | bit  15:0 R  AN_LP_ABLTY                                                |
 | +-------------------------------------------------------------------------+
 | 
 | UINT32 l1rpp_fege_field_AN_LP_ABLTY_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_range_AN_LP_ABLTY_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x000000a8 MII_AUTO_NEGOTIATION_EXPANSION |
 | +----------------------------------------------------+
 | | bit  1 R  RX_PAGE                                  |
 | | bit  0 R  RESOLVE_PRIORITY                         |
 | +----------------------------------------------------+
 | 
 | UINT32 l1rpp_fege_field_RX_PAGE_get          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_RESOLVE_PRIORITY_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +-------------------------------------------------------+
 | | Register 0x000000ac MII_AUTO_NEGOTIATION_NEXT_PAGE_TX |
 | +-------------------------------------------------------+
 | | bit  15   R/W  LOC_NP_NEXT_PAGE                       |
 | | bit  13   R/W  LOC_NP_MSG_PAGE                        |
 | | bit  12   R/W  LOC_NP_ACK2                            |
 | | bit  11   R    LOC_NP_TOGGLE                          |
 | | bit  10:0 R/W  LOC_NP_CODE_FIELD                      |
 | +-------------------------------------------------------+
 | 
 | void   l1rpp_fege_field_LOC_NP_NEXT_PAGE_set  ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_LOC_NP_NEXT_PAGE_get  ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_LOC_NP_MSG_PAGE_set   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_LOC_NP_MSG_PAGE_get   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_LOC_NP_ACK2_set       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_LOC_NP_ACK2_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_LOC_NP_CODE_set       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_LOC_NP_CODE_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_range_LOC_NP_CODE_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1rpp_fege_field_range_LOC_NP_CODE_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 l1rpp_fege_field_LOC_NP_TOGGLE_get     ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +----------------------------------------------------------------------+
 | | Register 0x000000b0 MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE |
 | +----------------------------------------------------------------------+
 | | bit  15   R  LNK_NP_NEXT_PAGE                                        |
 | | bit  14   R  LNK_NP_ACK                                              |
 | | bit  13   R  LNK_NP_MSG_PAGE                                         |
 | | bit  12   R  LNK_NP_ACK2                                             |
 | | bit  11   R  LNK_NP_TOGGLE                                           |
 | | bit  10:0 R  LNK_NP_CODE_FIELD                                       |
 | +----------------------------------------------------------------------+
 | 
 | UINT32 l1rpp_fege_field_LNK_NP_NEXT_PAGE_get  ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_LNK_NP_ACK_get        ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_LNK_NP_MSG_PAGE_get   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_LNK_NP_ACK2_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_LNK_NP_TOGGLE_get     ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_LNK_NP_CODE_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_range_LNK_NP_CODE_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x000000bc DEBUG_LINK_TIMER_OVERRIDE |
 | +-----------------------------------------------+
 | | bit  15:0 R/W  LINK_TIMER                     |
 | +-----------------------------------------------+
 | 
 | void   l1rpp_fege_field_LINK_TIMER_set       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_LINK_TIMER_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_range_LINK_TIMER_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1rpp_fege_field_range_LINK_TIMER_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000000c0 LINK_TIME_DURATION |
 | +----------------------------------------+
 | | bit  13:0 R/W  LINK_TIME_DUR           |
 | +----------------------------------------+
 | 
 | void   l1rpp_fege_field_LINK_TIME_DUR_set       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_LINK_TIME_DUR_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_range_LINK_TIME_DUR_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1rpp_fege_field_range_LINK_TIME_DUR_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000c4 SGMII_GMII_MODE |
 | +-------------------------------------+
 | | bit  15 R/W  MODE_SEL               |
 | +-------------------------------------+
 | 
 | void   l1rpp_fege_field_MODE_SEL_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_MODE_SEL_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x000000c8 COMMA_DET |
 | +-------------------------------+
 | | bit  0 R/W  EN_CDET_SEL       |
 | +-------------------------------+
 | 
 | void   l1rpp_fege_field_EN_CDET_SEL_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_EN_CDET_SEL_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +--------------------------------+
 | | Register 0x000000cc PMON_CNT_0 |
 | +--------------------------------+
 | | bit  15:0 R  PMON_CNTR         |
 | +--------------------------------+
 | 
 | void   l1rpp_fege_lfield_PMON_CNTR_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 l1rpp_fege_lfield_range_PMON_CNTR_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x000000d8 PMON_UPDATE |
 | +---------------------------------+
 | | bit  0 R/W  PMON_UPDATE         |
 | +---------------------------------+
 | 
 | void   l1rpp_fege_field_PMON_UPDATE_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_PMON_UPDATE_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x000000dc BIT_ALIGNMENT |
 | +-----------------------------------+
 | | bit  3:0 R  BIT_ALIGN             |
 | +-----------------------------------+
 | 
 | UINT32 l1rpp_fege_field_BIT_ALIGN_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 l1rpp_fege_field_range_BIT_ALIGN_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x000000e0 MIN_LINK_TIME_DURATION |
 | +--------------------------------------------+
 | | bit  13:0 R/W  MIN_LINK_TIME_DUR           |
 | +--------------------------------------------+
 | 
 | void   l1rpp_fege_field_MIN_LINK_TIME_DUR_set       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 l1rpp_fege_field_MIN_LINK_TIME_DUR_get       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   l1rpp_fege_field_range_MIN_LINK_TIME_DUR_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 l1rpp_fege_field_range_MIN_LINK_TIME_DUR_get ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _L1RPP_FEGE_IO_H
#define _L1RPP_FEGE_IO_H

#include "enet_fege_loc.h"
#include "l1rpp_fege_regs.h"
#include "l1rpp_fege_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 l1rpp_fege_buffer_init               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void                 l1rpp_fege_buffer_flush              ( l1rpp_fege_buffer_t *b_ptr )
 | UINT32               l1rpp_fege_reg_read                  ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 l1rpp_fege_reg_write                 ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 l1rpp_fege_field_set                 ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 l1rpp_fege_action_on_write_field_set ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 l1rpp_fege_burst_read                ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 l1rpp_fege_burst_write               ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_poll                      ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 l1rpp_fege_reg_CFG_AND_STAT_write                                       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 l1rpp_fege_reg_CFG_AND_STAT_field_set                                   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               l1rpp_fege_reg_CFG_AND_STAT_read                                        ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_write                                ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_field_set                            ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               l1rpp_fege_reg_LINK_SYNC_SM_CONFIG_read                                 ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_write                     ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_field_set                 ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               l1rpp_fege_reg_JITTER_PATTERN_STAT_AND_CONFIG_read                      ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 l1rpp_fege_reg_MII_CTRL_write                                           ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 l1rpp_fege_reg_MII_CTRL_field_set                                       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               l1rpp_fege_reg_MII_CTRL_read                                            ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_write                 ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_field_set             ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               l1rpp_fege_reg_MII_AUTO_NEGOTIATION_ADVERTISEMENT_read                  ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_write                  ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_field_set              ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               l1rpp_fege_reg_MII_AUTO_NEGOTIATION_NEXT_PAGE_TX_read                   ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_write                          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_field_set                      ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               l1rpp_fege_reg_DEBUG_LINK_TIMER_OVERRIDE_read                           ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 l1rpp_fege_reg_LINK_TIME_DURATION_write                                 ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 l1rpp_fege_reg_LINK_TIME_DURATION_field_set                             ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               l1rpp_fege_reg_LINK_TIME_DURATION_read                                  ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 l1rpp_fege_reg_SGMII_GMII_MODE_write                                    ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 l1rpp_fege_reg_SGMII_GMII_MODE_field_set                                ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               l1rpp_fege_reg_SGMII_GMII_MODE_read                                     ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 l1rpp_fege_reg_COMMA_DET_write                                          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 l1rpp_fege_reg_COMMA_DET_field_set                                      ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               l1rpp_fege_reg_COMMA_DET_read                                           ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 l1rpp_fege_reg_PMON_UPDATE_write                                        ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 l1rpp_fege_reg_PMON_UPDATE_field_set                                    ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               l1rpp_fege_reg_PMON_UPDATE_read                                         ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 l1rpp_fege_reg_MIN_LINK_TIME_DURATION_write                             ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 l1rpp_fege_reg_MIN_LINK_TIME_DURATION_field_set                         ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               l1rpp_fege_reg_MIN_LINK_TIME_DURATION_read                              ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 l1rpp_fege_reg_INT_write                                                ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 l1rpp_fege_reg_INT_action_on_write_field_set                            ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               l1rpp_fege_reg_INT_read                                                 ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_reg_INT_poll                                                 ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 l1rpp_fege_reg_INT_EN_write                                             ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 l1rpp_fege_reg_INT_EN_field_set                                         ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               l1rpp_fege_reg_INT_EN_read                                              ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               l1rpp_fege_reg_INT_STAT_read                                            ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE l1rpp_fege_reg_INT_STAT_poll                                            ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               l1rpp_fege_reg_PMON_CNT_0_read                                          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               l1rpp_fege_reg_PMON_CNT_1_read                                          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               l1rpp_fege_reg_PMON_CNT_2_read                                          ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               l1rpp_fege_reg_MII_STAT_read                                            ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_BASE_PAGE_ABILITY_read ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               l1rpp_fege_reg_MII_AUTO_NEGOTIATION_EXPANSION_read                      ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               l1rpp_fege_reg_MII_AUTO_NEGOTIATION_LINK_PARTNER_RCVD_NEXT_PAGE_read    ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               l1rpp_fege_reg_BIT_ALIGNMENT_read                                       ( l1rpp_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _L1RPP_FEGE_IO_H */
