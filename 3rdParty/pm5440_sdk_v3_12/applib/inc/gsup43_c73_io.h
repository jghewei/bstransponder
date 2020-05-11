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
 *     and register accessor functions for the gsup43_c73 block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing gsup43_c73 io functions is:
 | {
 |     gsup43_c73_buffer_t b_ptr[1];
 |     gsup43_c73_buffer_init( b_ptr, h_ptr );
 |     value = gsup43_c73_field_<FIELD>_get( b_ptr, h_ptr );
 |     gsup43_c73_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     gsup43_c73_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = gsup43_c73_field_<FIELD>_get( NULL, h_ptr );
 | gsup43_c73_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-------------------------+
 | | Register 0x00000000 CFG |
 | +-------------------------+
 | | bit  1 R/W  PMON_UPDATE |
 | | bit  0 R/W  SW_RST      |
 | +-------------------------+
 | 
 | void   gsup43_c73_field_SW_RST_set      ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_SW_RST_get      ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_PMON_UPDATE_set ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_PMON_UPDATE_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | +----------------------------+
 | | Register 0x00000004 RX_CFG |
 | +----------------------------+
 | | bit  31 R/W  RX_SW_RST     |
 | | bit  0  R/W  LOS_INV       |
 | +----------------------------+
 | 
 | void   gsup43_c73_field_RX_SW_RST_set ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_RX_SW_RST_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_LOS_INV_set   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_LOS_INV_get   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000008 RX_GSUP43_CFG  |
 | +------------------------------------+
 | | bit  31    R/W  RX_GSUP43_C73_EN   |
 | | bit  25:24 R/W  DSCRD_ZONE         |
 | | bit  23    R/W  RX_SIG_OS_DSCRD_EN |
 | | bit  22    R/W  RX_SEQ_OS_DSCRD_EN |
 | | bit  21    R/W  RX_OS_ALL_DSCRD    |
 | | bit  20    R/W  RX_FULL_OS_RPT_DIS |
 | | bit  19:16 R/W  RX_MIN_OS_RPT      |
 | | bit  15:8  R/W  RX_PKT_TYPE        |
 | | bit  7:0   R/W  RX_OS_TYPE         |
 | +------------------------------------+
 | 
 | void   gsup43_c73_field_RX_GSUP43_C73_EN_set    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_RX_GSUP43_C73_EN_get    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_DSCRD_ZONE_set          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_DSCRD_ZONE_get          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_range_DSCRD_ZONE_set    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 gsup43_c73_field_range_DSCRD_ZONE_get    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   gsup43_c73_field_RX_SIG_OS_DSCRD_EN_set  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_RX_SIG_OS_DSCRD_EN_get  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_RX_SEQ_OS_DSCRD_EN_set  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_RX_SEQ_OS_DSCRD_EN_get  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_RX_OS_ALL_DSCRD_set     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_RX_OS_ALL_DSCRD_get     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_RX_FULL_OS_RPT_DIS_set  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_RX_FULL_OS_RPT_DIS_get  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_RX_MIN_OS_RPT_set       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_RX_MIN_OS_RPT_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_range_RX_MIN_OS_RPT_set ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 gsup43_c73_field_range_RX_MIN_OS_RPT_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   gsup43_c73_field_RX_PKT_TYPE_set         ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_RX_PKT_TYPE_get         ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_range_RX_PKT_TYPE_set   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 gsup43_c73_field_range_RX_PKT_TYPE_get   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   gsup43_c73_field_RX_OS_TYPE_set          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_RX_OS_TYPE_get          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_range_RX_OS_TYPE_set    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 gsup43_c73_field_range_RX_OS_TYPE_get    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x0000000c RX_125US_CNT |
 | +----------------------------------+
 | | bit  15:0 R/W  RX_125US_CNT      |
 | +----------------------------------+
 | 
 | void   gsup43_c73_field_RX_125US_CNT_set       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_RX_125US_CNT_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_range_RX_125US_CNT_set ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 gsup43_c73_field_range_RX_125US_CNT_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000010 RX_INT_EN   |
 | +---------------------------------+
 | | bit  9 R/W  RX_LF_E             |
 | | bit  8 R/W  RX_RF_E             |
 | | bit  7 R/W  ALL_ONES_OR_ZEROS_E |
 | | bit  6 R/W  RX_PRBS31_ERR_E     |
 | | bit  5 R/W  BLK_TYPE_ERR_E      |
 | | bit  4 R/W  RX_FAULT_E          |
 | | bit  3 R/W  LINK_FAIL_E         |
 | | bit  2 R/W  HI_BER_E            |
 | | bit  1 R/W  LOS_SYNC_E          |
 | | bit  0 R/W  LOS_E               |
 | +---------------------------------+
 | 
 | void   gsup43_c73_field_RX_LF_E_set             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_RX_LF_E_get             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_RX_RF_E_set             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_RX_RF_E_get             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_ALL_ONES_OR_ZEROS_E_set ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_ALL_ONES_OR_ZEROS_E_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_RX_PRBS31_ERR_E_set     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_RX_PRBS31_ERR_E_get     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_BLK_TYPE_ERR_E_set      ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_BLK_TYPE_ERR_E_get      ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_RX_FAULT_E_set          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_RX_FAULT_E_get          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_LINK_FAIL_E_set         ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_LINK_FAIL_E_get         ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_HI_BER_E_set            ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_HI_BER_E_get            ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_LOS_SYNC_E_set          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_LOS_SYNC_E_get          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_LOS_E_set               ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_LOS_E_get               ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000014 RX_INT_INDICATION |
 | +---------------------------------------+
 | | bit  9 R/W  RX_LF_IC                  |
 | | bit  8 R/W  RX_RF_IC                  |
 | | bit  7 R/W  ALL_ONES_OR_ZEROS_IC      |
 | | bit  6 R/W  RX_PRBS31_ERR_I           |
 | | bit  5 R/W  BLK_TYPE_ERR_I            |
 | | bit  4 R/W  RX_FAULT_IC               |
 | | bit  3 R/W  LINK_FAIL_IC              |
 | | bit  2 R/W  HI_BER_IC                 |
 | | bit  1 R/W  LOS_SYNC_IC               |
 | | bit  0 R/W  LOS_IC                    |
 | +---------------------------------------+
 | 
 | void                 gsup43_c73_field_RX_LF_IC_set_to_clear             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_RX_LF_IC_get                      ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_LF_IC_poll                     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 gsup43_c73_field_RX_RF_IC_set_to_clear             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_RX_RF_IC_get                      ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_RF_IC_poll                     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_set_to_clear ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_get          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_ALL_ONES_OR_ZEROS_IC_poll         ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 gsup43_c73_field_RX_FAULT_IC_set_to_clear          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_RX_FAULT_IC_get                   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_FAULT_IC_poll                  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 gsup43_c73_field_LINK_FAIL_IC_set_to_clear         ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_LINK_FAIL_IC_get                  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_LINK_FAIL_IC_poll                 ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 gsup43_c73_field_HI_BER_IC_set_to_clear            ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_HI_BER_IC_get                     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_HI_BER_IC_poll                    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 gsup43_c73_field_LOS_SYNC_IC_set_to_clear          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_LOS_SYNC_IC_get                   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_LOS_SYNC_IC_poll                  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 gsup43_c73_field_LOS_IC_set_to_clear               ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_LOS_IC_get                        ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_LOS_IC_poll                       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 gsup43_c73_field_RX_PRBS31_ERR_I_set_to_clear      ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_RX_PRBS31_ERR_I_get               ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_PRBS31_ERR_I_poll              ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 gsup43_c73_field_BLK_TYPE_ERR_I_set_to_clear       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_BLK_TYPE_ERR_I_get                ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_BLK_TYPE_ERR_I_poll               ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------+
 | | Register 0x00000018 RX_STAT    |
 | +--------------------------------+
 | | bit  31 R  RF_FAULT            |
 | | bit  30 R  LF_FAULT            |
 | | bit  9  R  RX_LF_V             |
 | | bit  8  R  RX_RF_V             |
 | | bit  7  R  ALL_ONES_OR_ZEROS_V |
 | | bit  4  R  RX_FAULT_V          |
 | | bit  3  R  LINK_FAIL_V         |
 | | bit  2  R  HI_BER_V            |
 | | bit  1  R  LOS_SYNC_V          |
 | | bit  0  R  LOS_V               |
 | +--------------------------------+
 | 
 | UINT32               gsup43_c73_field_RX_LF_V_get              ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_LF_V_poll             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               gsup43_c73_field_RX_RF_V_get              ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_RF_V_poll             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               gsup43_c73_field_ALL_ONES_OR_ZEROS_V_get  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_ALL_ONES_OR_ZEROS_V_poll ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               gsup43_c73_field_RX_FAULT_V_get           ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_RX_FAULT_V_poll          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               gsup43_c73_field_LINK_FAIL_V_get          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_LINK_FAIL_V_poll         ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               gsup43_c73_field_HI_BER_V_get             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_HI_BER_V_poll            ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               gsup43_c73_field_LOS_SYNC_V_get           ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_LOS_SYNC_V_poll          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               gsup43_c73_field_LOS_V_get                ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_LOS_V_poll               ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               gsup43_c73_field_RF_FAULT_get             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | UINT32               gsup43_c73_field_LF_FAULT_get             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x0000001c RX_FRAME_ERR_CNT |
 | +--------------------------------------+
 | | bit  15:8 R  BLK_TYPE_ERR_CNT        |
 | | bit  7:0  R  HI_BER_CNT              |
 | +--------------------------------------+
 | 
 | UINT32 gsup43_c73_field_BLK_TYPE_ERR_CNT_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | UINT32 gsup43_c73_field_range_BLK_TYPE_ERR_CNT_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 gsup43_c73_field_HI_BER_CNT_get             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | UINT32 gsup43_c73_field_range_HI_BER_CNT_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000020 RX_BLOCK_ERR_CNTS |
 | +---------------------------------------+
 | | bit  31:0 R  RX_BLK_ERR_CNT           |
 | +---------------------------------------+
 | 
 | UINT32 gsup43_c73_field_RX_BLK_ERR_CNT_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | UINT32 gsup43_c73_field_range_RX_BLK_ERR_CNT_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x00000024 RX_DISCARDED_ORDERED_SET_CNT_LSB |
 | +------------------------------------------------------+
 | | bit  31:0 R  RX_OS_DSCRCD_CNT                        |
 | +------------------------------------------------------+
 | 
 | void   gsup43_c73_lfield_RX_OS_DSCRCD_CNT_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 gsup43_c73_lfield_range_RX_OS_DSCRCD_CNT_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------------+
 | | Register 0x00000028 RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT |
 | +-----------------------------------------------------------------+
 | | bit  31:16 R  PRBS31_ERR_CNT                                    |
 | | bit  7:0   R  RX_OS_DSCRCD_CNT                                  |
 | +-----------------------------------------------------------------+
 | 
 | UINT32 gsup43_c73_field_PRBS31_ERR_CNT_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | UINT32 gsup43_c73_field_range_PRBS31_ERR_CNT_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0000002c RX_DATA_BYTE_CNT_LSB |
 | +------------------------------------------+
 | | bit  31:0 R  RX_DATA_BYTE_CNT            |
 | +------------------------------------------+
 | 
 | void   gsup43_c73_lfield_RX_DATA_BYTE_CNT_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 gsup43_c73_lfield_range_RX_DATA_BYTE_CNT_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000030 RX_ORDERED_SET_CNT_LSB |
 | +--------------------------------------------+
 | | bit  31:0 R  RX_OS_CNT                     |
 | +--------------------------------------------+
 | 
 | void   gsup43_c73_lfield_RX_OS_CNT_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 gsup43_c73_lfield_range_RX_OS_CNT_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000038 RX_PACKET_CNT_LSB |
 | +---------------------------------------+
 | | bit  31:0 R  RX_PKT_CNT               |
 | +---------------------------------------+
 | 
 | void   gsup43_c73_lfield_RX_PKT_CNT_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 gsup43_c73_lfield_range_RX_PKT_CNT_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------+
 | | Register 0x00000040 TX_CFG_1 |
 | +------------------------------+
 | | bit  31   R/W  TX_SW_RST     |
 | | bit  16   R/W  TX_ERR_DROP   |
 | | bit  15:8 R/W  TX_PKT_TYPE   |
 | | bit  7:0  R/W  TX_OS_TYPE    |
 | +------------------------------+
 | 
 | void   gsup43_c73_field_TX_SW_RST_set         ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_TX_SW_RST_get         ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_TX_ERR_DROP_set       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_TX_ERR_DROP_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_TX_PKT_TYPE_set       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_TX_PKT_TYPE_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_range_TX_PKT_TYPE_set ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 gsup43_c73_field_range_TX_PKT_TYPE_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   gsup43_c73_field_TX_OS_TYPE_set        ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_TX_OS_TYPE_get        ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_range_TX_OS_TYPE_set  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 gsup43_c73_field_range_TX_OS_TYPE_get  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x00000044 TX_CFG_2   |
 | +--------------------------------+
 | | bit  31  R/W  TX_GSUP43_C73_EN |
 | | bit  10  R/W  FORCE_MIN_IPG    |
 | | bit  9   R/W  UNIDIR_EN        |
 | | bit  8   R/W  SEND_IDLE_EN     |
 | | bit  7   R/W  SEND_LF_EN       |
 | | bit  6   R/W  SEND_RF_EN       |
 | | bit  5   R/W  PRBS31_EN        |
 | | bit  4:0 R/W  FIFO_HMARK       |
 | +--------------------------------+
 | 
 | void   gsup43_c73_field_TX_GSUP43_C73_EN_set ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_TX_GSUP43_C73_EN_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_FORCE_MIN_IPG_set    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_FORCE_MIN_IPG_get    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_UNIDIR_EN_set        ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_UNIDIR_EN_get        ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_SEND_IDLE_EN_set     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_SEND_IDLE_EN_get     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_SEND_LF_EN_set       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_SEND_LF_EN_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_SEND_RF_EN_set       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_SEND_RF_EN_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_PRBS31_EN_set        ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_PRBS31_EN_get        ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_FIFO_HMARK_set       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_FIFO_HMARK_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_range_FIFO_HMARK_set ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 gsup43_c73_field_range_FIFO_HMARK_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000048 TX_PACKET_CNT_LSB |
 | +---------------------------------------+
 | | bit  31:0 R  TX_PKT_CNT               |
 | +---------------------------------------+
 | 
 | void   gsup43_c73_lfield_TX_PKT_CNT_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 gsup43_c73_lfield_range_TX_PKT_CNT_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000050 TX_DATA_BYTE_CNT_LSB |
 | +------------------------------------------+
 | | bit  31:0 R  TX_DATA_BYTE_CNT            |
 | +------------------------------------------+
 | 
 | void   gsup43_c73_lfield_TX_DATA_BYTE_CNT_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 gsup43_c73_lfield_range_TX_DATA_BYTE_CNT_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000054 TX_ORDERED_SET_CNT_LSB |
 | +--------------------------------------------+
 | | bit  31:0 R  TX_OS_CNT                     |
 | +--------------------------------------------+
 | 
 | void   gsup43_c73_lfield_TX_OS_CNT_get       ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 gsup43_c73_lfield_range_TX_OS_CNT_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x0000005c TX_INT_EN |
 | +-------------------------------+
 | | bit  5 R/W  TYPE_ERR_E        |
 | | bit  4 R/W  SEND_IDLE_E       |
 | | bit  3 R/W  SEND_LF_E         |
 | | bit  2 R/W  SEND_RF_E         |
 | | bit  1 R/W  FIFO_OVRUN_E      |
 | | bit  0 R/W  FIFO_UNDRUN_E     |
 | +-------------------------------+
 | 
 | void   gsup43_c73_field_TYPE_ERR_E_set    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_TYPE_ERR_E_get    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_SEND_IDLE_E_set   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_SEND_IDLE_E_get   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_SEND_LF_E_set     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_SEND_LF_E_get     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_SEND_RF_E_set     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_SEND_RF_E_get     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_FIFO_OVRUN_E_set  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_FIFO_OVRUN_E_get  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void   gsup43_c73_field_FIFO_UNDRUN_E_set ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32 gsup43_c73_field_FIFO_UNDRUN_E_get ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000060 TX_INT_INDICATION |
 | +---------------------------------------+
 | | bit  5 R/W  TYPE_ERR_I                |
 | | bit  4 R/W  SEND_IDLE_I               |
 | | bit  3 R/W  SEND_LF_I                 |
 | | bit  2 R/W  SEND_RF_I                 |
 | | bit  1 R/W  FIFO_OVRUN_I              |
 | | bit  0 R/W  FIFO_UNDRUN_I             |
 | +---------------------------------------+
 | 
 | void                 gsup43_c73_field_TYPE_ERR_I_set_to_clear    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_TYPE_ERR_I_get             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_TYPE_ERR_I_poll            ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 gsup43_c73_field_SEND_IDLE_I_set_to_clear   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_SEND_IDLE_I_get            ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_IDLE_I_poll           ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 gsup43_c73_field_SEND_LF_I_set_to_clear     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_SEND_LF_I_get              ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_LF_I_poll             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 gsup43_c73_field_SEND_RF_I_set_to_clear     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_SEND_RF_I_get              ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_RF_I_poll             ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 gsup43_c73_field_FIFO_OVRUN_I_set_to_clear  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_FIFO_OVRUN_I_get           ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_FIFO_OVRUN_I_poll          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 gsup43_c73_field_FIFO_UNDRUN_I_set_to_clear ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | UINT32               gsup43_c73_field_FIFO_UNDRUN_I_get          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_FIFO_UNDRUN_I_poll         ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------+
 | | Register 0x00000064 TX_STAT |
 | +-----------------------------+
 | | bit  4 R  SEND_IDLE_V       |
 | | bit  3 R  SEND_LF_V         |
 | | bit  2 R  SEND_RF_V         |
 | +-----------------------------+
 | 
 | UINT32               gsup43_c73_field_SEND_IDLE_V_get  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_IDLE_V_poll ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               gsup43_c73_field_SEND_LF_V_get    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_LF_V_poll   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               gsup43_c73_field_SEND_RF_V_get    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_field_SEND_RF_V_poll   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _GSUP43_C73_IO_H
#define _GSUP43_C73_IO_H

#include "gsup43_c73_loc.h"
#include "gsup43_c73_regs.h"
#include "gsup43_c73_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 gsup43_c73_buffer_init               ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | void                 gsup43_c73_buffer_flush              ( gsup43_c73_buffer_t *b_ptr )
 | UINT32               gsup43_c73_reg_read                  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 gsup43_c73_reg_write                 ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 gsup43_c73_field_set                 ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 gsup43_c73_action_on_write_field_set ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 gsup43_c73_burst_read                ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 gsup43_c73_burst_write               ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE gsup43_c73_poll                      ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 gsup43_c73_reg_CFG_write                                        ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | void                 gsup43_c73_reg_CFG_field_set                                    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               gsup43_c73_reg_CFG_read                                         ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | void                 gsup43_c73_reg_RX_CFG_write                                     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | void                 gsup43_c73_reg_RX_CFG_field_set                                 ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               gsup43_c73_reg_RX_CFG_read                                      ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | void                 gsup43_c73_reg_RX_GSUP43_CFG_write                              ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | void                 gsup43_c73_reg_RX_GSUP43_CFG_field_set                          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               gsup43_c73_reg_RX_GSUP43_CFG_read                               ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | void                 gsup43_c73_reg_RX_125US_CNT_write                               ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | void                 gsup43_c73_reg_RX_125US_CNT_field_set                           ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               gsup43_c73_reg_RX_125US_CNT_read                                ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | void                 gsup43_c73_reg_TX_CFG_1_write                                   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | void                 gsup43_c73_reg_TX_CFG_1_field_set                               ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               gsup43_c73_reg_TX_CFG_1_read                                    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | void                 gsup43_c73_reg_TX_CFG_2_write                                   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | void                 gsup43_c73_reg_TX_CFG_2_field_set                               ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               gsup43_c73_reg_TX_CFG_2_read                                    ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | void                 gsup43_c73_reg_RX_INT_INDICATION_write                          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | void                 gsup43_c73_reg_RX_INT_INDICATION_action_on_write_field_set      ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               gsup43_c73_reg_RX_INT_INDICATION_read                           ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_reg_RX_INT_INDICATION_poll                           ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 gsup43_c73_reg_TX_INT_INDICATION_write                          ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | void                 gsup43_c73_reg_TX_INT_INDICATION_action_on_write_field_set      ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               gsup43_c73_reg_TX_INT_INDICATION_read                           ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_reg_TX_INT_INDICATION_poll                           ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 gsup43_c73_reg_RX_INT_EN_write                                  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | void                 gsup43_c73_reg_RX_INT_EN_field_set                              ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               gsup43_c73_reg_RX_INT_EN_read                                   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | void                 gsup43_c73_reg_TX_INT_EN_write                                  ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 value )
 | void                 gsup43_c73_reg_TX_INT_EN_field_set                              ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               gsup43_c73_reg_TX_INT_EN_read                                   ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | UINT32               gsup43_c73_reg_RX_STAT_read                                     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_reg_RX_STAT_poll                                     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               gsup43_c73_reg_TX_STAT_read                                     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE gsup43_c73_reg_TX_STAT_poll                                     ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               gsup43_c73_reg_RX_FRAME_ERR_CNT_read                            ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | UINT32               gsup43_c73_reg_RX_BLOCK_ERR_CNTS_read                           ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | UINT32               gsup43_c73_reg_RX_DISCARDED_ORDERED_SET_CNT_LSB_read            ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | UINT32               gsup43_c73_reg_RX_PRBS31_AND_DISCARDED_ORDERED_SET_MSB_CNT_read ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | UINT32               gsup43_c73_reg_RX_DATA_BYTE_CNT_LSB_read                        ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | UINT32               gsup43_c73_reg_RX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read        ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | UINT32               gsup43_c73_reg_RX_ORDERED_SET_CNT_LSB_read                      ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | UINT32               gsup43_c73_reg_RX_PACKET_CNT_LSB_read                           ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | UINT32               gsup43_c73_reg_RX_PACKET_CNT_MSB_read                           ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | UINT32               gsup43_c73_reg_TX_PACKET_CNT_LSB_read                           ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | UINT32               gsup43_c73_reg_TX_PACKET_CNT_MSB_read                           ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | UINT32               gsup43_c73_reg_TX_DATA_BYTE_CNT_LSB_read                        ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | UINT32               gsup43_c73_reg_TX_ORDERED_SET_AND_DATA_BYTE_CNT_MSB_read        ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 | UINT32               gsup43_c73_reg_TX_ORDERED_SET_CNT_LSB_read                      ( gsup43_c73_buffer_t *b_ptr, gsup43_c73_handle_t *h_ptr )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _GSUP43_C73_IO_H */
