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
 *     and register accessor functions for the sysotn_sfis_rx block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing sysotn_sfis_rx io functions is:
 | {
 |     sysotn_sfis_rx_buffer_t b_ptr[1];
 |     sysotn_sfis_rx_buffer_init( b_ptr, h_ptr );
 |     value = sysotn_sfis_rx_field_<FIELD>_get( b_ptr, h_ptr );
 |     sysotn_sfis_rx_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     sysotn_sfis_rx_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = sysotn_sfis_rx_field_<FIELD>_get( NULL, h_ptr );
 | sysotn_sfis_rx_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------+
 | | Register 0x00004000 RX_CFG |
 | +----------------------------+
 | | bit  5   R/W  RX_ODD_INV   |
 | | bit  4   R/W  RX_SW_RST    |
 | | bit  3:0 R/W  RX_NUM_LANES |
 | +----------------------------+
 | 
 | void   sysotn_sfis_rx_field_RX_ODD_INV_set         ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RX_ODD_INV_get         ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RX_NUM_LANES_set       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RX_NUM_LANES_get       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_range_RX_NUM_LANES_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_range_RX_NUM_LANES_get ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_sfis_rx_field_RX_SW_RST_set          ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RX_SW_RST_get          ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00004004 RX_INT_SUMMARY |
 | +------------------------------------+
 | | bit  3:0 R  RX_I                   |
 | +------------------------------------+
 | 
 | UINT32               sysotn_sfis_rx_field_RX_I_get        ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32               sysotn_sfis_rx_field_range_RX_I_get  ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RX_I_poll ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RX_I_poll       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------+
 | | Register 0x00004008 RX_INT_0 |
 | +------------------------------+
 | | bit  25:16 R/W  RXOOA_I      |
 | | bit  11    R/W  RXOOF_I      |
 | | bit  10    R/W  RXLOS_DSC_I  |
 | | bit  9:0   R/W  RXLOS_I      |
 | +------------------------------+
 | 
 | void                 sysotn_sfis_rx_field_RXOOA_I_set_to_clear       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXOOA_I_get                ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 sysotn_sfis_rx_field_range_RXOOA_I_set_to_clear ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_range_RXOOA_I_get          ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXOOA_I_poll         ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOOA_I_poll               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXOOF_I_set_to_clear       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXOOF_I_get                ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOOF_I_poll               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXLOS_DSC_I_set_to_clear   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXLOS_DSC_I_get            ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXLOS_DSC_I_poll           ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXLOS_I_set_to_clear       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXLOS_I_get                ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 sysotn_sfis_rx_field_range_RXLOS_I_set_to_clear ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_range_RXLOS_I_get          ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXLOS_I_poll         ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXLOS_I_poll               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------+
 | | Register 0x0000400c RX_INT_1 |
 | +------------------------------+
 | | bit  26  R/W  RXOVR_DSC_I    |
 | | bit  25  R/W  RXOVR9_I       |
 | | bit  24  R/W  RXOVR8_I       |
 | | bit  23  R/W  RXOVR7_I       |
 | | bit  22  R/W  RXOVR6_I       |
 | | bit  21  R/W  RXOVR5_I       |
 | | bit  20  R/W  RXOVR4_I       |
 | | bit  19  R/W  RXOVR3_I       |
 | | bit  18  R/W  RXOVR2_I       |
 | | bit  17  R/W  RXOVR1_I       |
 | | bit  16  R/W  RXOVR0_I       |
 | | bit  10  R/W  RXUDR_DSC_I    |
 | | bit  9:0 R/W  RXUDR_I        |
 | +------------------------------+
 | 
 | void                 sysotn_sfis_rx_field_RXOVR_DSC_I_set_to_clear   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXOVR_DSC_I_get            ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR_DSC_I_poll           ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXOVR9_I_set_to_clear      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXOVR9_I_get               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR9_I_poll              ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXOVR8_I_set_to_clear      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXOVR8_I_get               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR8_I_poll              ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXOVR7_I_set_to_clear      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXOVR7_I_get               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR7_I_poll              ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXOVR6_I_set_to_clear      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXOVR6_I_get               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR6_I_poll              ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXOVR5_I_set_to_clear      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXOVR5_I_get               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR5_I_poll              ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXOVR4_I_set_to_clear      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXOVR4_I_get               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR4_I_poll              ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXOVR3_I_set_to_clear      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXOVR3_I_get               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR3_I_poll              ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXOVR2_I_set_to_clear      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXOVR2_I_get               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR2_I_poll              ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXOVR1_I_set_to_clear      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXOVR1_I_get               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR1_I_poll              ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXOVR0_I_set_to_clear      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXOVR0_I_get               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOVR0_I_poll              ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXUDR_DSC_I_set_to_clear   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXUDR_DSC_I_get            ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXUDR_DSC_I_poll           ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RXUDR_I_set_to_clear       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RXUDR_I_get                ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 sysotn_sfis_rx_field_range_RXUDR_I_set_to_clear ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_range_RXUDR_I_get          ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXUDR_I_poll         ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXUDR_I_poll               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------+
 | | Register 0x00004010 RX_INT_2  |
 | +-------------------------------+
 | | bit  10  R/W  RX_BITERR_DSC_I |
 | | bit  9:0 R/W  RX_BITERR_I     |
 | +-------------------------------+
 | 
 | void                 sysotn_sfis_rx_field_RX_BITERR_DSC_I_set_to_clear   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RX_BITERR_DSC_I_get            ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RX_BITERR_DSC_I_poll           ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_sfis_rx_field_RX_BITERR_I_set_to_clear       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_RX_BITERR_I_get                ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 sysotn_sfis_rx_field_range_RX_BITERR_I_set_to_clear ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               sysotn_sfis_rx_field_range_RX_BITERR_I_get          ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RX_BITERR_I_poll         ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RX_BITERR_I_poll               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------+
 | | Register 0x00004018 RX_INT_EN_0 |
 | +---------------------------------+
 | | bit  25:16 R/W  RXOOA_E         |
 | | bit  11    R/W  RXOOF_E         |
 | | bit  10    R/W  RXLOS_DSC_E     |
 | | bit  9:0   R/W  RXLOS_E         |
 | +---------------------------------+
 | 
 | void   sysotn_sfis_rx_field_RXOOA_E_set       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOOA_E_get       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_range_RXOOA_E_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_range_RXOOA_E_get ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_sfis_rx_field_RXOOF_E_set       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOOF_E_get       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXLOS_DSC_E_set   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXLOS_DSC_E_get   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXLOS_E_set       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXLOS_E_get       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_range_RXLOS_E_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_range_RXLOS_E_get ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x0000401c RX_INT_EN_1 |
 | +---------------------------------+
 | | bit  26  R/W  RXOVR_DSC_E       |
 | | bit  25  R/W  RXOVR9_E          |
 | | bit  24  R/W  RXOVR8_E          |
 | | bit  23  R/W  RXOVR7_E          |
 | | bit  22  R/W  RXOVR6_E          |
 | | bit  21  R/W  RXOVR5_E          |
 | | bit  20  R/W  RXOVR4_E          |
 | | bit  19  R/W  RXOVR3_E          |
 | | bit  18  R/W  RXOVR2_E          |
 | | bit  17  R/W  RXOVR1_E          |
 | | bit  16  R/W  RXOVR0_E          |
 | | bit  10  R/W  RXUDR_DSC_E       |
 | | bit  9:0 R/W  RXUDR_E           |
 | +---------------------------------+
 | 
 | void   sysotn_sfis_rx_field_RXOVR_DSC_E_set   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOVR_DSC_E_get   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXOVR9_E_set      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOVR9_E_get      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXOVR8_E_set      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOVR8_E_get      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXOVR7_E_set      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOVR7_E_get      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXOVR6_E_set      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOVR6_E_get      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXOVR5_E_set      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOVR5_E_get      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXOVR4_E_set      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOVR4_E_get      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXOVR3_E_set      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOVR3_E_get      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXOVR2_E_set      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOVR2_E_get      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXOVR1_E_set      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOVR1_E_get      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXOVR0_E_set      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOVR0_E_get      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXUDR_DSC_E_set   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXUDR_DSC_E_get   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXUDR_E_set       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXUDR_E_get       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_range_RXUDR_E_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_range_RXUDR_E_get ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00004020 RX_INT_EN_2 |
 | +---------------------------------+
 | | bit  10  R/W  RX_BITERR_DSC_E   |
 | | bit  9:0 R/W  RX_BITERR_E       |
 | +---------------------------------+
 | 
 | void   sysotn_sfis_rx_field_RX_BITERR_DSC_E_set   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RX_BITERR_DSC_E_get   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RX_BITERR_E_set       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RX_BITERR_E_get       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_range_RX_BITERR_E_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_range_RX_BITERR_E_get ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00004028 RX_INT_VAL_0 |
 | +----------------------------------+
 | | bit  25:16 R  RXOOA_V            |
 | | bit  11    R  RXOOF_V            |
 | | bit  10    R  RXLOS_DSC_V        |
 | | bit  9:0   R  RXLOS_V            |
 | +----------------------------------+
 | 
 | UINT32               sysotn_sfis_rx_field_RXOOA_V_get        ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32               sysotn_sfis_rx_field_range_RXOOA_V_get  ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXOOA_V_poll ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOOA_V_poll       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_sfis_rx_field_RXOOF_V_get        ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXOOF_V_poll       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_sfis_rx_field_RXLOS_DSC_V_get    ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXLOS_DSC_V_poll   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_sfis_rx_field_RXLOS_V_get        ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32               sysotn_sfis_rx_field_range_RXLOS_V_get  ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_range_RXLOS_V_poll ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_field_RXLOS_V_poll       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x0000402c RX_BUFFER_CFG |
 | +-----------------------------------+
 | | bit  7 R/W  RX_FIFO_CENTER        |
 | | bit  6 R/W  RX_FIFO_AUTO_CENTER   |
 | +-----------------------------------+
 | 
 | void   sysotn_sfis_rx_field_RX_FIFO_AUTO_CENTER_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RX_FIFO_AUTO_CENTER_get ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RX_FIFO_CENTER_set      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RX_FIFO_CENTER_get      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +--------------------------------------------+
 | | Register 0x00004030 RX_FRAMING_CTRLLER_CFG |
 | +--------------------------------------------+
 | | bit  15:8 R/W  RX_MATCH_TIMEOUT_THRESH     |
 | | bit  7:4  R/W  RX_MATCH_THRESH             |
 | | bit  3:0  R/W  RX_BAD_THRESH               |
 | +--------------------------------------------+
 | 
 | void   sysotn_sfis_rx_field_RX_MATCH_TIMEOUT_THRESH_set       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RX_MATCH_TIMEOUT_THRESH_get       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_range_RX_MATCH_TIMEOUT_THRESH_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_range_RX_MATCH_TIMEOUT_THRESH_get ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_sfis_rx_field_RX_MATCH_THRESH_set               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RX_MATCH_THRESH_get               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_range_RX_MATCH_THRESH_set         ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_range_RX_MATCH_THRESH_get         ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_sfis_rx_field_RX_BAD_THRESH_set                 ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RX_BAD_THRESH_get                 ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_range_RX_BAD_THRESH_set           ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_range_RX_BAD_THRESH_get           ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x00004034 RX_DESKEW_CTRLLER_CFG    |
 | +----------------------------------------------+
 | | bit  15:8 R/W  RX_ALIGN_MATCH_TIMEOUT_THRESH |
 | | bit  7:4  R/W  RX_ALIGN_MATCH_THRESH         |
 | | bit  3:0  R/W  RX_ALIGN_BAD_THRESH           |
 | +----------------------------------------------+
 | 
 | void   sysotn_sfis_rx_field_RX_ALIGN_MATCH_TIMEOUT_THRESH_set       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RX_ALIGN_MATCH_TIMEOUT_THRESH_get       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_TIMEOUT_THRESH_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_TIMEOUT_THRESH_get ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_sfis_rx_field_RX_ALIGN_MATCH_THRESH_set               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RX_ALIGN_MATCH_THRESH_get               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_THRESH_set         ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_range_RX_ALIGN_MATCH_THRESH_get         ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_sfis_rx_field_RX_ALIGN_BAD_THRESH_set                 ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RX_ALIGN_BAD_THRESH_get                 ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_range_RX_ALIGN_BAD_THRESH_set           ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_range_RX_ALIGN_BAD_THRESH_get           ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0000403c RX_ALARM_INTEGRATION |
 | +------------------------------------------+
 | | bit  3 R/W  RXOOA_ALARM_EN               |
 | | bit  2 R/W  RXOOF_ALARM_EN               |
 | | bit  1 R/W  RXLOS_ALARM_EN               |
 | | bit  0 R/W  RX_FORCE_LOS                 |
 | +------------------------------------------+
 | 
 | void   sysotn_sfis_rx_field_RXOOA_ALARM_EN_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOOA_ALARM_EN_get ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXOOF_ALARM_EN_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXOOF_ALARM_EN_get ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RXLOS_ALARM_EN_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RXLOS_ALARM_EN_get ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_sfis_rx_field_RX_FORCE_LOS_set   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_sfis_rx_field_RX_FORCE_LOS_get   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SYSOTN_SFIS_RX_IO_H
#define _SYSOTN_SFIS_RX_IO_H

#include "lineotn_loc.h"
#include "sysotn_sfis_rx_regs.h"
#include "sysotn_sfis_rx_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sysotn_sfis_rx_buffer_init               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 sysotn_sfis_rx_buffer_flush              ( sysotn_sfis_rx_buffer_t *b_ptr )
 | UINT32               sysotn_sfis_rx_reg_read                  ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 sysotn_sfis_rx_reg_write                 ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 sysotn_sfis_rx_field_set                 ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 sysotn_sfis_rx_action_on_write_field_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 sysotn_sfis_rx_burst_read                ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 sysotn_sfis_rx_burst_write               ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_poll                      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sysotn_sfis_rx_reg_RX_CFG_write                       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_rx_reg_RX_CFG_field_set                   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_rx_reg_RX_CFG_read                        ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_sfis_rx_reg_RX_BUFFER_CFG_write                ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_rx_reg_RX_BUFFER_CFG_field_set            ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_rx_reg_RX_BUFFER_CFG_read                 ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_write       ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_field_set   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_rx_reg_RX_FRAMING_CTRLLER_CFG_read        ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_write        ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_field_set    ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_rx_reg_RX_DESKEW_CTRLLER_CFG_read         ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_write         ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_field_set     ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_rx_reg_RX_ALARM_INTEGRATION_read          ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_sfis_rx_reg_RX_INT_EN_0_write                  ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_rx_reg_RX_INT_EN_0_field_set              ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_rx_reg_RX_INT_EN_0_read                   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_sfis_rx_reg_RX_INT_EN_1_write                  ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_rx_reg_RX_INT_EN_1_field_set              ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_rx_reg_RX_INT_EN_1_read                   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_sfis_rx_reg_RX_INT_EN_2_write                  ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_rx_reg_RX_INT_EN_2_field_set              ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_rx_reg_RX_INT_EN_2_read                   ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | UINT32               sysotn_sfis_rx_reg_RX_INT_SUMMARY_read                ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_SUMMARY_poll                ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sysotn_sfis_rx_reg_RX_INT_0_write                     ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_rx_reg_RX_INT_0_action_on_write_field_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_rx_reg_RX_INT_0_read                      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_0_poll                      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sysotn_sfis_rx_reg_RX_INT_1_write                     ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_rx_reg_RX_INT_1_action_on_write_field_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_rx_reg_RX_INT_1_read                      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_1_poll                      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sysotn_sfis_rx_reg_RX_INT_2_write                     ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_sfis_rx_reg_RX_INT_2_action_on_write_field_set ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_sfis_rx_reg_RX_INT_2_read                      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_2_poll                      ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               sysotn_sfis_rx_reg_RX_INT_VAL_0_read                  ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_sfis_rx_reg_RX_INT_VAL_0_poll                  ( sysotn_sfis_rx_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SYSOTN_SFIS_RX_IO_H */
