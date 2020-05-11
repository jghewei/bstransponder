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
 *     and register accessor functions for the d8_rx_2x block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing d8_rx_2x io functions is:
 | {
 |     d8_rx_2x_buffer_t b_ptr[1];
 |     d8_rx_2x_buffer_init( b_ptr, h_ptr );
 |     value = d8_rx_2x_field_<FIELD>_get( b_ptr, h_ptr );
 |     d8_rx_2x_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     d8_rx_2x_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = d8_rx_2x_field_<FIELD>_get( NULL, h_ptr );
 | d8_rx_2x_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------------+
 | | Register 0x00000000 SW_RESET_CTRL     |
 | +---------------------------------------+
 | | bit  26 R/W  LOS_FLT_B_RSTB           |
 | | bit  25 R/W  LOS_FLT_A_RSTB           |
 | | bit  24 R/W  LOS_OC_B_RSTB            |
 | | bit  23 R/W  LOS_OC_A_RSTB            |
 | | bit  22 R/W  DIAGNOSTIC_B_RSTB        |
 | | bit  21 R/W  DIAGNOSTIC_A_RSTB        |
 | | bit  20 R/W  LB_CLK_DEGLITCH_B_RSTB   |
 | | bit  19 R/W  LB_CLK_DEGLITCH_A_RSTB   |
 | | bit  18 R/W  EFUSE_INT_B_RSTB         |
 | | bit  17 R/W  EFUSE_INT_A_RSTB         |
 | | bit  16 R/W  GCOC_B_RSTB              |
 | | bit  15 R/W  GCOC_A_RSTB              |
 | | bit  14 R/W  SIPO_BIST_B_RSTB         |
 | | bit  13 R/W  SIPO_BIST_A_RSTB         |
 | | bit  12 R/W  BADJ_CLK_DEGLITCH_B_RSTB |
 | | bit  11 R/W  BADJ_CLK_DEGLITCH_A_RSTB |
 | | bit  10 R/W  ADC_CLK_DEGLITCH_B_RSTB  |
 | | bit  9  R/W  ADC_CLK_DEGLITCH_A_RSTB  |
 | | bit  8  R/W  PATT_MON_B_RSTB          |
 | | bit  7  R/W  PATT_MON_A_RSTB          |
 | | bit  6  R/W  PATT_GEN_B_RSTB          |
 | | bit  5  R/W  PATT_GEN_A_RSTB          |
 | | bit  4  R/W  GEN_SIPO_B_RSTB          |
 | | bit  3  R/W  GEN_SIPO_A_RSTB          |
 | | bit  2  R/W  RXCPGEN_LOCKDET_B_RSTB   |
 | | bit  1  R/W  RXCPGEN_LOCKDET_A_RSTB   |
 | | bit  0  R/W  DIGITAL_RSTB             |
 | +---------------------------------------+
 | 
 | void   d8_rx_2x_field_LOS_FLT_B_RSTB_set           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_FLT_B_RSTB_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_LOS_FLT_A_RSTB_set           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_FLT_A_RSTB_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_LOS_OC_B_RSTB_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_OC_B_RSTB_get            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_LOS_OC_A_RSTB_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_OC_A_RSTB_get            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_DIAGNOSTIC_B_RSTB_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_DIAGNOSTIC_B_RSTB_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_DIAGNOSTIC_A_RSTB_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_DIAGNOSTIC_A_RSTB_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_LB_CLK_DEGLITCH_B_RSTB_set   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_LB_CLK_DEGLITCH_B_RSTB_get   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_LB_CLK_DEGLITCH_A_RSTB_set   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_LB_CLK_DEGLITCH_A_RSTB_get   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_EFUSE_INT_B_RSTB_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_EFUSE_INT_B_RSTB_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_EFUSE_INT_A_RSTB_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_EFUSE_INT_A_RSTB_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_GCOC_B_RSTB_set              ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_GCOC_B_RSTB_get              ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_GCOC_A_RSTB_set              ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_GCOC_A_RSTB_get              ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_SIPO_BIST_B_RSTB_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_SIPO_BIST_B_RSTB_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_SIPO_BIST_A_RSTB_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_SIPO_BIST_A_RSTB_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_BADJ_CLK_DEGLITCH_B_RSTB_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_BADJ_CLK_DEGLITCH_B_RSTB_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_BADJ_CLK_DEGLITCH_A_RSTB_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_BADJ_CLK_DEGLITCH_A_RSTB_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_ADC_CLK_DEGLITCH_B_RSTB_set  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_CLK_DEGLITCH_B_RSTB_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_ADC_CLK_DEGLITCH_A_RSTB_set  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_CLK_DEGLITCH_A_RSTB_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_PATT_MON_B_RSTB_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_PATT_MON_B_RSTB_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_PATT_MON_A_RSTB_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_PATT_MON_A_RSTB_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_PATT_GEN_B_RSTB_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_PATT_GEN_B_RSTB_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_PATT_GEN_A_RSTB_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_PATT_GEN_A_RSTB_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_GEN_SIPO_B_RSTB_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_GEN_SIPO_B_RSTB_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_GEN_SIPO_A_RSTB_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_GEN_SIPO_A_RSTB_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_LOCKDET_B_RSTB_set   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_LOCKDET_B_RSTB_get   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_LOCKDET_A_RSTB_set   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_LOCKDET_A_RSTB_get   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_DIGITAL_RSTB_set             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_DIGITAL_RSTB_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000004 ANALOG_TEST_CTRL |
 | +--------------------------------------+
 | | bit  13:12 R/W  TEST_DIVMODE         |
 | | bit  11:8  R/W  RX_MODE              |
 | | bit  7     R/W  TEST_SLICE_EN        |
 | | bit  5     R/W  RS_ATMSB             |
 | | bit  4:0   R/W  ATIN                 |
 | +--------------------------------------+
 | 
 | void   d8_rx_2x_field_TEST_DIVMODE_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_TEST_DIVMODE_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_range_TEST_DIVMODE_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_TEST_DIVMODE_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_RX_MODE_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RX_MODE_get            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_range_RX_MODE_set      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_RX_MODE_get      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_TEST_SLICE_EN_set      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_TEST_SLICE_EN_get      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RS_ATMSB_set           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RS_ATMSB_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_ATIN_set               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_ATIN_get               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_range_ATIN_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_ATIN_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000008 INT_REG_1         |
 | +---------------------------------------+
 | | bit  29 R/W  CALIB_PASS_INT_B_I       |
 | | bit  28 R/W  CALIB_PASS_INT_A_I       |
 | | bit  27 R/W  ADC_BIST_ERR_B_I         |
 | | bit  26 R/W  ADC_BIST_ERR_A_I         |
 | | bit  25 R/W  ADC_BIST_DONE_B_I        |
 | | bit  24 R/W  ADC_BIST_DONE_A_I        |
 | | bit  23 R/W  RAM_PARITY_ERR_B_I       |
 | | bit  22 R/W  RAM_PARITY_ERR_A_I       |
 | | bit  21 R/W  SIPO_BIST_SYNC_B_I       |
 | | bit  20 R/W  SIPO_BIST_SYNC_A_I       |
 | | bit  19 R/W  SIPO_BIST_ERR_B_I        |
 | | bit  18 R/W  SIPO_BIST_ERR_A_I        |
 | | bit  17 R/W  DIVCLK_DEAD_B_I          |
 | | bit  16 R/W  DIVCLK_DEAD_A_I          |
 | | bit  15 R/W  IN_RANGEN_FAILS_INT_B_I  |
 | | bit  14 R/W  IN_RANGEN_FAILS_INT_A_I  |
 | | bit  13 R/W  IN_RANGEN_PASSES_INT_B_I |
 | | bit  12 R/W  IN_RANGEN_PASSES_INT_A_I |
 | | bit  11 R/W  PLL_LOCK_LOSS_INT_B_I    |
 | | bit  10 R/W  PLL_LOCK_LOSS_INT_A_I    |
 | | bit  9  R/W  PLL_LOCK_GAIN_INT_B_I    |
 | | bit  8  R/W  PLL_LOCK_GAIN_INT_A_I    |
 | | bit  7  R/W  ADCOC_FSM_ERR_INT_B_I    |
 | | bit  6  R/W  ADCOC_FSM_ERR_INT_A_I    |
 | | bit  5  R/W  PROG_REF_DONE_INT_B_I    |
 | | bit  4  R/W  PROG_REF_DONE_INT_A_I    |
 | | bit  3  R/W  CALIB_DONE_INT_B_I       |
 | | bit  2  R/W  CALIB_DONE_INT_A_I       |
 | | bit  1  R/W  PATT_MON_ERR_B_I         |
 | | bit  0  R/W  PATT_MON_ERR_A_I         |
 | +---------------------------------------+
 | 
 | void                 d8_rx_2x_field_CALIB_PASS_INT_B_I_set_to_clear       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_CALIB_PASS_INT_B_I_get                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_CALIB_PASS_INT_B_I_poll               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_CALIB_PASS_INT_A_I_set_to_clear       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_CALIB_PASS_INT_A_I_get                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_CALIB_PASS_INT_A_I_poll               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_ADC_BIST_ERR_B_I_set_to_clear         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_ADC_BIST_ERR_B_I_get                  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_ADC_BIST_ERR_B_I_poll                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_ADC_BIST_ERR_A_I_set_to_clear         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_ADC_BIST_ERR_A_I_get                  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_ADC_BIST_ERR_A_I_poll                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_ADC_BIST_DONE_B_I_set_to_clear        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_ADC_BIST_DONE_B_I_get                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_ADC_BIST_DONE_B_I_poll                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_ADC_BIST_DONE_A_I_set_to_clear        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_ADC_BIST_DONE_A_I_get                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_ADC_BIST_DONE_A_I_poll                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_RAM_PARITY_ERR_B_I_set_to_clear       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_RAM_PARITY_ERR_B_I_get                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_RAM_PARITY_ERR_B_I_poll               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_RAM_PARITY_ERR_A_I_set_to_clear       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_RAM_PARITY_ERR_A_I_get                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_RAM_PARITY_ERR_A_I_poll               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_SIPO_BIST_SYNC_B_I_set_to_clear       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_SIPO_BIST_SYNC_B_I_get                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_SIPO_BIST_SYNC_B_I_poll               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_SIPO_BIST_SYNC_A_I_set_to_clear       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_SIPO_BIST_SYNC_A_I_get                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_SIPO_BIST_SYNC_A_I_poll               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_SIPO_BIST_ERR_B_I_set_to_clear        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_SIPO_BIST_ERR_B_I_get                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_SIPO_BIST_ERR_B_I_poll                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_SIPO_BIST_ERR_A_I_set_to_clear        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_SIPO_BIST_ERR_A_I_get                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_SIPO_BIST_ERR_A_I_poll                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_DIVCLK_DEAD_B_I_set_to_clear          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_DIVCLK_DEAD_B_I_get                   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_DIVCLK_DEAD_B_I_poll                  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_DIVCLK_DEAD_A_I_set_to_clear          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_DIVCLK_DEAD_A_I_get                   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_DIVCLK_DEAD_A_I_poll                  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_IN_RANGEN_FAILS_INT_B_I_set_to_clear  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_IN_RANGEN_FAILS_INT_B_I_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_IN_RANGEN_FAILS_INT_B_I_poll          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_IN_RANGEN_FAILS_INT_A_I_set_to_clear  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_IN_RANGEN_FAILS_INT_A_I_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_IN_RANGEN_FAILS_INT_A_I_poll          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_IN_RANGEN_PASSES_INT_B_I_set_to_clear ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_IN_RANGEN_PASSES_INT_B_I_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_IN_RANGEN_PASSES_INT_B_I_poll         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_IN_RANGEN_PASSES_INT_A_I_set_to_clear ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_IN_RANGEN_PASSES_INT_A_I_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_IN_RANGEN_PASSES_INT_A_I_poll         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_PLL_LOCK_LOSS_INT_B_I_set_to_clear    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_PLL_LOCK_LOSS_INT_B_I_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PLL_LOCK_LOSS_INT_B_I_poll            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_PLL_LOCK_LOSS_INT_A_I_set_to_clear    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_PLL_LOCK_LOSS_INT_A_I_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PLL_LOCK_LOSS_INT_A_I_poll            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_PLL_LOCK_GAIN_INT_B_I_set_to_clear    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_PLL_LOCK_GAIN_INT_B_I_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PLL_LOCK_GAIN_INT_B_I_poll            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_PLL_LOCK_GAIN_INT_A_I_set_to_clear    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_PLL_LOCK_GAIN_INT_A_I_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PLL_LOCK_GAIN_INT_A_I_poll            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_ADCOC_FSM_ERR_INT_B_I_set_to_clear    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_ADCOC_FSM_ERR_INT_B_I_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_ADCOC_FSM_ERR_INT_B_I_poll            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_ADCOC_FSM_ERR_INT_A_I_set_to_clear    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_ADCOC_FSM_ERR_INT_A_I_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_ADCOC_FSM_ERR_INT_A_I_poll            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_PROG_REF_DONE_INT_B_I_set_to_clear    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_PROG_REF_DONE_INT_B_I_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PROG_REF_DONE_INT_B_I_poll            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_PROG_REF_DONE_INT_A_I_set_to_clear    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_PROG_REF_DONE_INT_A_I_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PROG_REF_DONE_INT_A_I_poll            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_CALIB_DONE_INT_B_I_set_to_clear       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_CALIB_DONE_INT_B_I_get                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_CALIB_DONE_INT_B_I_poll               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_CALIB_DONE_INT_A_I_set_to_clear       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_CALIB_DONE_INT_A_I_get                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_CALIB_DONE_INT_A_I_poll               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_PATT_MON_ERR_B_I_set_to_clear         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_PATT_MON_ERR_B_I_get                  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PATT_MON_ERR_B_I_poll                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_PATT_MON_ERR_A_I_set_to_clear         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_PATT_MON_ERR_A_I_get                  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PATT_MON_ERR_A_I_poll                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x0000000c INT_EN_REG_1      |
 | +---------------------------------------+
 | | bit  29 R/W  CALIB_PASS_INT_B_E       |
 | | bit  28 R/W  CALIB_PASS_INT_A_E       |
 | | bit  27 R/W  ADC_BIST_ERR_B_E         |
 | | bit  26 R/W  ADC_BIST_ERR_A_E         |
 | | bit  25 R/W  ADC_BIST_DONE_B_E        |
 | | bit  24 R/W  ADC_BIST_DONE_A_E        |
 | | bit  23 R/W  RAM_PARITY_ERR_B_E       |
 | | bit  22 R/W  RAM_PARITY_ERR_A_E       |
 | | bit  21 R/W  SIPO_BIST_SYNC_B_E       |
 | | bit  20 R/W  SIPO_BIST_SYNC_A_E       |
 | | bit  19 R/W  SIPO_BIST_ERR_B_E        |
 | | bit  18 R/W  SIPO_BIST_ERR_A_E        |
 | | bit  17 R/W  DIVCLK_DEAD_B_E          |
 | | bit  16 R/W  DIVCLK_DEAD_A_E          |
 | | bit  15 R/W  IN_RANGEN_FAILS_INT_B_E  |
 | | bit  14 R/W  IN_RANGEN_FAILS_INT_A_E  |
 | | bit  13 R/W  IN_RANGEN_PASSES_INT_B_E |
 | | bit  12 R/W  IN_RANGEN_PASSES_INT_A_E |
 | | bit  11 R/W  PLL_LOCK_LOSS_INT_B_E    |
 | | bit  10 R/W  PLL_LOCK_LOSS_INT_A_E    |
 | | bit  9  R/W  PLL_LOCK_GAIN_INT_B_E    |
 | | bit  8  R/W  PLL_LOCK_GAIN_INT_A_E    |
 | | bit  7  R/W  ADCOC_FSM_ERR_INT_B_E    |
 | | bit  6  R/W  ADCOC_FSM_ERR_INT_A_E    |
 | | bit  5  R/W  PROG_REF_DONE_INT_B_E    |
 | | bit  4  R/W  PROG_REF_DONE_INT_A_E    |
 | | bit  3  R/W  CALIB_DONE_INT_B_E       |
 | | bit  2  R/W  CALIB_DONE_INT_A_E       |
 | | bit  1  R/W  PATT_MON_ERR_B_E         |
 | | bit  0  R/W  PATT_MON_ERR_A_E         |
 | +---------------------------------------+
 | 
 | void   d8_rx_2x_field_CALIB_PASS_INT_B_E_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_CALIB_PASS_INT_B_E_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_CALIB_PASS_INT_A_E_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_CALIB_PASS_INT_A_E_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_ADC_BIST_ERR_B_E_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_BIST_ERR_B_E_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_ADC_BIST_ERR_A_E_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_BIST_ERR_A_E_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_ADC_BIST_DONE_B_E_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_BIST_DONE_B_E_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_ADC_BIST_DONE_A_E_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_BIST_DONE_A_E_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RAM_PARITY_ERR_B_E_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RAM_PARITY_ERR_B_E_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RAM_PARITY_ERR_A_E_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RAM_PARITY_ERR_A_E_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_SIPO_BIST_SYNC_B_E_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_SIPO_BIST_SYNC_B_E_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_SIPO_BIST_SYNC_A_E_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_SIPO_BIST_SYNC_A_E_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_SIPO_BIST_ERR_B_E_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_SIPO_BIST_ERR_B_E_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_SIPO_BIST_ERR_A_E_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_SIPO_BIST_ERR_A_E_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_DIVCLK_DEAD_B_E_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_DIVCLK_DEAD_B_E_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_DIVCLK_DEAD_A_E_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_DIVCLK_DEAD_A_E_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_IN_RANGEN_FAILS_INT_B_E_set  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_IN_RANGEN_FAILS_INT_B_E_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_IN_RANGEN_FAILS_INT_A_E_set  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_IN_RANGEN_FAILS_INT_A_E_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_IN_RANGEN_PASSES_INT_B_E_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_IN_RANGEN_PASSES_INT_B_E_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_IN_RANGEN_PASSES_INT_A_E_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_IN_RANGEN_PASSES_INT_A_E_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_PLL_LOCK_LOSS_INT_B_E_set    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_PLL_LOCK_LOSS_INT_B_E_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_PLL_LOCK_LOSS_INT_A_E_set    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_PLL_LOCK_LOSS_INT_A_E_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_PLL_LOCK_GAIN_INT_B_E_set    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_PLL_LOCK_GAIN_INT_B_E_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_PLL_LOCK_GAIN_INT_A_E_set    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_PLL_LOCK_GAIN_INT_A_E_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_ADCOC_FSM_ERR_INT_B_E_set    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_ADCOC_FSM_ERR_INT_B_E_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_ADCOC_FSM_ERR_INT_A_E_set    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_ADCOC_FSM_ERR_INT_A_E_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_PROG_REF_DONE_INT_B_E_set    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_PROG_REF_DONE_INT_B_E_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_PROG_REF_DONE_INT_A_E_set    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_PROG_REF_DONE_INT_A_E_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_CALIB_DONE_INT_B_E_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_CALIB_DONE_INT_B_E_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_CALIB_DONE_INT_A_E_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_CALIB_DONE_INT_A_E_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_PATT_MON_ERR_B_E_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_PATT_MON_ERR_B_E_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_PATT_MON_ERR_A_E_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_PATT_MON_ERR_A_E_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x00000010 INT_STAT_REG_1  |
 | +-------------------------------------+
 | | bit  29 R  CALIB_PASS_INT_B_V       |
 | | bit  28 R  CALIB_PASS_INT_A_V       |
 | | bit  27 R  ADC_BIST_ERR_B_V         |
 | | bit  26 R  ADC_BIST_ERR_A_V         |
 | | bit  25 R  ADC_BIST_DONE_B_V        |
 | | bit  24 R  ADC_BIST_DONE_A_V        |
 | | bit  23 R  RAM_PARITY_ERR_B_V       |
 | | bit  22 R  RAM_PARITY_ERR_A_V       |
 | | bit  21 R  SIPO_BIST_SYNC_B_V       |
 | | bit  20 R  SIPO_BIST_SYNC_A_V       |
 | | bit  19 R  SIPO_BIST_ERR_B_V        |
 | | bit  18 R  SIPO_BIST_ERR_A_V        |
 | | bit  17 R  DIVCLK_DEAD_B_V          |
 | | bit  16 R  DIVCLK_DEAD_A_V          |
 | | bit  15 R  IN_RANGEN_FAILS_INT_B_V  |
 | | bit  14 R  IN_RANGEN_FAILS_INT_A_V  |
 | | bit  13 R  IN_RANGEN_PASSES_INT_B_V |
 | | bit  12 R  IN_RANGEN_PASSES_INT_A_V |
 | | bit  11 R  PLL_LOCK_LOSS_INT_B_V    |
 | | bit  10 R  PLL_LOCK_LOSS_INT_A_V    |
 | | bit  9  R  PLL_LOCK_GAIN_INT_B_V    |
 | | bit  8  R  PLL_LOCK_GAIN_INT_A_V    |
 | | bit  7  R  ADCOC_FSM_ERR_INT_B_V    |
 | | bit  6  R  ADCOC_FSM_ERR_INT_A_V    |
 | | bit  5  R  PROG_REF_DONE_INT_B_V    |
 | | bit  4  R  PROG_REF_DONE_INT_A_V    |
 | | bit  3  R  CALIB_DONE_INT_B_V       |
 | | bit  2  R  CALIB_DONE_INT_A_V       |
 | | bit  1  R  PATT_MON_ERR_B_V         |
 | | bit  0  R  PATT_MON_ERR_A_V         |
 | +-------------------------------------+
 | 
 | UINT32               d8_rx_2x_field_CALIB_PASS_INT_B_V_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_CALIB_PASS_INT_B_V_poll       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_CALIB_PASS_INT_A_V_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_CALIB_PASS_INT_A_V_poll       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_ADC_BIST_ERR_B_V_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_ADC_BIST_ERR_B_V_poll         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_ADC_BIST_ERR_A_V_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_ADC_BIST_ERR_A_V_poll         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_ADC_BIST_DONE_B_V_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_ADC_BIST_DONE_B_V_poll        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_ADC_BIST_DONE_A_V_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_ADC_BIST_DONE_A_V_poll        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_RAM_PARITY_ERR_B_V_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_RAM_PARITY_ERR_B_V_poll       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_RAM_PARITY_ERR_A_V_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_RAM_PARITY_ERR_A_V_poll       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_SIPO_BIST_SYNC_B_V_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_SIPO_BIST_SYNC_B_V_poll       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_SIPO_BIST_SYNC_A_V_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_SIPO_BIST_SYNC_A_V_poll       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_SIPO_BIST_ERR_B_V_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_SIPO_BIST_ERR_B_V_poll        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_SIPO_BIST_ERR_A_V_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_SIPO_BIST_ERR_A_V_poll        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_DIVCLK_DEAD_B_V_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_DIVCLK_DEAD_B_V_poll          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_DIVCLK_DEAD_A_V_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_DIVCLK_DEAD_A_V_poll          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_IN_RANGEN_FAILS_INT_B_V_get   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_IN_RANGEN_FAILS_INT_B_V_poll  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_IN_RANGEN_FAILS_INT_A_V_get   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_IN_RANGEN_FAILS_INT_A_V_poll  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_IN_RANGEN_PASSES_INT_B_V_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_IN_RANGEN_PASSES_INT_B_V_poll ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_IN_RANGEN_PASSES_INT_A_V_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_IN_RANGEN_PASSES_INT_A_V_poll ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_PLL_LOCK_LOSS_INT_B_V_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PLL_LOCK_LOSS_INT_B_V_poll    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_PLL_LOCK_LOSS_INT_A_V_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PLL_LOCK_LOSS_INT_A_V_poll    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_PLL_LOCK_GAIN_INT_B_V_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PLL_LOCK_GAIN_INT_B_V_poll    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_PLL_LOCK_GAIN_INT_A_V_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PLL_LOCK_GAIN_INT_A_V_poll    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_ADCOC_FSM_ERR_INT_B_V_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_ADCOC_FSM_ERR_INT_B_V_poll    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_ADCOC_FSM_ERR_INT_A_V_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_ADCOC_FSM_ERR_INT_A_V_poll    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_PROG_REF_DONE_INT_B_V_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PROG_REF_DONE_INT_B_V_poll    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_PROG_REF_DONE_INT_A_V_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PROG_REF_DONE_INT_A_V_poll    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_CALIB_DONE_INT_B_V_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_CALIB_DONE_INT_B_V_poll       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_CALIB_DONE_INT_A_V_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_CALIB_DONE_INT_A_V_poll       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_PATT_MON_ERR_B_V_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PATT_MON_ERR_B_V_poll         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_PATT_MON_ERR_A_V_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_PATT_MON_ERR_A_V_poll         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------+
 | | Register 0x00000014 INT_REG_2 |
 | +-------------------------------+
 | | bit  3 R/W  LOS_OC_FAIL_B_I   |
 | | bit  2 R/W  LOS_OC_FAIL_A_I   |
 | | bit  1 R/W  LOS_OC_PASS_B_I   |
 | | bit  0 R/W  LOS_OC_PASS_A_I   |
 | +-------------------------------+
 | 
 | void                 d8_rx_2x_field_LOS_OC_FAIL_B_I_set_to_clear ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_LOS_OC_FAIL_B_I_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_LOS_OC_FAIL_B_I_poll         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_LOS_OC_FAIL_A_I_set_to_clear ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_LOS_OC_FAIL_A_I_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_LOS_OC_FAIL_A_I_poll         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_LOS_OC_PASS_B_I_set_to_clear ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_LOS_OC_PASS_B_I_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_LOS_OC_PASS_B_I_poll         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_rx_2x_field_LOS_OC_PASS_A_I_set_to_clear ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_rx_2x_field_LOS_OC_PASS_A_I_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_LOS_OC_PASS_A_I_poll         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------+
 | | Register 0x00000018 INT_EN_REG_2 |
 | +----------------------------------+
 | | bit  3 R/W  LOS_OC_FAIL_B_E      |
 | | bit  2 R/W  LOS_OC_FAIL_A_E      |
 | | bit  1 R/W  LOS_OC_PASS_B_E      |
 | | bit  0 R/W  LOS_OC_PASS_A_E      |
 | +----------------------------------+
 | 
 | void   d8_rx_2x_field_LOS_OC_FAIL_B_E_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_OC_FAIL_B_E_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_LOS_OC_FAIL_A_E_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_OC_FAIL_A_E_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_LOS_OC_PASS_B_E_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_OC_PASS_B_E_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_LOS_OC_PASS_A_E_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_OC_PASS_A_E_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x0000001c INT_STAT_REG_2 |
 | +------------------------------------+
 | | bit  3 R  LOS_OC_FAIL_B_V          |
 | | bit  2 R  LOS_OC_FAIL_A_V          |
 | | bit  1 R  LOS_OC_PASS_B_V          |
 | | bit  0 R  LOS_OC_PASS_A_V          |
 | +------------------------------------+
 | 
 | UINT32               d8_rx_2x_field_LOS_OC_FAIL_B_V_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_LOS_OC_FAIL_B_V_poll ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_LOS_OC_FAIL_A_V_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_LOS_OC_FAIL_A_V_poll ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_LOS_OC_PASS_B_V_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_LOS_OC_PASS_B_V_poll ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_LOS_OC_PASS_A_V_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_LOS_OC_PASS_A_V_poll ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00000020 INT_SUMMARY_REG |
 | +-------------------------------------+
 | | bit  1 R  INT1_SUMMARY              |
 | | bit  0 R  INT0_SUMMARY              |
 | +-------------------------------------+
 | 
 | UINT32               d8_rx_2x_field_INT1_SUMMARY_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_INT1_SUMMARY_poll ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_rx_2x_field_INT0_SUMMARY_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_field_INT0_SUMMARY_poll ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00000024 + (N) * 0x4) EFUSE_INT_CFG_REG |
 | |     N[0..1]                                         |
 | +-----------------------------------------------------+
 | | bit  27:20 R    RAMS_DATA_RD                        |
 | | bit  19:12 R/W  RAMS_DATA_WR                        |
 | | bit  11:8  R/W  RAMS_ADDR                           |
 | | bit  4     R/W  RAMS_RD_EN                          |
 | | bit  3     R/W  RAMS_WR_EN                          |
 | | bit  2     R/W  PCLK_EN                             |
 | | bit  1:0   R/W  RAM_EXT_INT_SEL                     |
 | +-----------------------------------------------------+
 | 
 | void   d8_rx_2x_field_RAMS_DATA_WR_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAMS_DATA_WR_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_RAMS_DATA_WR_set    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_RAMS_DATA_WR_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_RAMS_ADDR_set             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAMS_ADDR_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_RAMS_ADDR_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_RAMS_ADDR_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_RAMS_RD_EN_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAMS_RD_EN_get            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RAMS_WR_EN_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAMS_WR_EN_get            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_PCLK_EN_set               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_PCLK_EN_get               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RAM_EXT_INT_SEL_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAM_EXT_INT_SEL_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_RAM_EXT_INT_SEL_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_RAM_EXT_INT_SEL_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 d8_rx_2x_field_RAMS_DATA_RD_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | UINT32 d8_rx_2x_field_range_RAMS_DATA_RD_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x0000002c + (N) * 0x4) GCOC_MEMORY_CFG_REG |
 | |     N[0..1]                                           |
 | +-------------------------------------------------------+
 | | bit  10:4 R/W  GCOC_MEM_DEGLITCH_DLY                  |
 | | bit  0    R/W  GCOC_MEM_CLK_SEL                       |
 | +-------------------------------------------------------+
 | 
 | void   d8_rx_2x_field_GCOC_MEM_DEGLITCH_DLY_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_GCOC_MEM_DEGLITCH_DLY_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_GCOC_MEM_DEGLITCH_DLY_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_GCOC_MEM_DEGLITCH_DLY_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_GCOC_MEM_CLK_SEL_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_GCOC_MEM_CLK_SEL_get            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------------+
 | | Register (0x00000034 + (N) * 0x4) ADC_OC_CFG_1 |
 | |     N[0..1]                                    |
 | +------------------------------------------------+
 | | bit  28    R/W  ADC_BIST_START                 |
 | | bit  27    R/W  CALIB_PROG_EN                  |
 | | bit  26:11 R/W  N_ACCUM_CYCLES                 |
 | | bit  10:3  R/W  AUX_OFFSET                     |
 | | bit  2     R/W  OFFSET_EN                      |
 | | bit  1     R/W  PROG_REF                       |
 | | bit  0     R/W  CALIB                          |
 | +------------------------------------------------+
 | 
 | void   d8_rx_2x_field_ADC_BIST_START_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_BIST_START_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_CALIB_PROG_EN_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_CALIB_PROG_EN_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_N_ACCUM_CYCLES_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_N_ACCUM_CYCLES_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_N_ACCUM_CYCLES_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_N_ACCUM_CYCLES_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_AUX_OFFSET_set           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_AUX_OFFSET_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_AUX_OFFSET_set     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_AUX_OFFSET_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_OFFSET_EN_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_OFFSET_EN_get            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_PROG_REF_set             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_PROG_REF_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_CALIB_set                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_CALIB_get                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------------+
 | | Register (0x0000003c + (N) * 0x4) ADC_OC_CFG_2 |
 | |     N[0..1]                                    |
 | +------------------------------------------------+
 | | bit  31:24 R/W  MAX_THRESHOLD                  |
 | | bit  23:16 R/W  MIN_THRESHOLD                  |
 | | bit  15:12 R/W  ACC_DELAY                      |
 | | bit  11:8  R/W  LIN_STEP                       |
 | | bit  7:0   R/W  DAC_DELAY                      |
 | +------------------------------------------------+
 | 
 | void   d8_rx_2x_field_MAX_THRESHOLD_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_MAX_THRESHOLD_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_MAX_THRESHOLD_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_MAX_THRESHOLD_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_MIN_THRESHOLD_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_MIN_THRESHOLD_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_MIN_THRESHOLD_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_MIN_THRESHOLD_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_ACC_DELAY_set           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ACC_DELAY_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_ACC_DELAY_set     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_ACC_DELAY_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_LIN_STEP_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LIN_STEP_get            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_LIN_STEP_set      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_LIN_STEP_get      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_DAC_DELAY_set           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_DAC_DELAY_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_DAC_DELAY_set     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_DAC_DELAY_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00000044 + (N) * 0x4) ADC_OC_CFG_3 |
 | |     N[0..1]                                    |
 | +------------------------------------------------+
 | | bit  31:24 R    ADC_Q                          |
 | | bit  23:16 R/W  ADC_A_OVR                      |
 | | bit  15:8  R/W  ADC_D_OVR                      |
 | | bit  2     R/W  ADC_RD_OVR                     |
 | | bit  1     R/W  ADC_WR_OVR                     |
 | | bit  0     R/W  MABC_INT_OVR_EN                |
 | +------------------------------------------------+
 | 
 | void   d8_rx_2x_field_ADC_A_OVR_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_A_OVR_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_ADC_A_OVR_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_ADC_A_OVR_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_ADC_D_OVR_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_D_OVR_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_ADC_D_OVR_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_ADC_D_OVR_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_ADC_RD_OVR_set      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_RD_OVR_get      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_ADC_WR_OVR_set      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_WR_OVR_get      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_MABC_INT_OVR_EN_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_MABC_INT_OVR_EN_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | UINT32 d8_rx_2x_field_ADC_Q_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | UINT32 d8_rx_2x_field_range_ADC_Q_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register (0x0000004c + (N) * 0x4) CDRU_CFG_ |
 | |     N[0..1]                                 |
 | +---------------------------------------------+
 | | bit  2:0 R/W  DECIM_FACT                    |
 | +---------------------------------------------+
 | 
 | void   d8_rx_2x_field_DECIM_FACT_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_DECIM_FACT_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_DECIM_FACT_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_DECIM_FACT_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00000054 + (N) * 0x4) RXCPGEN_LOCK_DETECTOR_CFG_1 |
 | |     N[0..1]                                                   |
 | +---------------------------------------------------------------+
 | | bit  29:26 R/W  LOCK_PRECISION                                |
 | | bit  25:20 R/W  LOCK_CNT_MAX                                  |
 | | bit  19:2  R/W  CLK_CNT_MAX                                   |
 | | bit  1     R/W  LOAD_CLK_CNTRS                                |
 | | bit  0     R/W  RESET_HISTORY                                 |
 | +---------------------------------------------------------------+
 | 
 | void   d8_rx_2x_field_LOCK_PRECISION_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOCK_PRECISION_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_LOCK_PRECISION_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_LOCK_PRECISION_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_LOCK_CNT_MAX_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOCK_CNT_MAX_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_LOCK_CNT_MAX_set   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_LOCK_CNT_MAX_get   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_CLK_CNT_MAX_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_CLK_CNT_MAX_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_CLK_CNT_MAX_set    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_CLK_CNT_MAX_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_LOAD_CLK_CNTRS_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOAD_CLK_CNTRS_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RESET_HISTORY_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RESET_HISTORY_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x0000005c + (N) * 0x4) RXCPGEN_LOCK_DETECTOR_CFG_2 |
 | |     N[0..1]                                                   |
 | +---------------------------------------------------------------+
 | | bit  31:26 R/W  FAIL_CNT_MAX                                  |
 | | bit  25:20 R/W  PASS_CNT_MAX                                  |
 | +---------------------------------------------------------------+
 | 
 | void   d8_rx_2x_field_FAIL_CNT_MAX_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_FAIL_CNT_MAX_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_FAIL_CNT_MAX_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_FAIL_CNT_MAX_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_PASS_CNT_MAX_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_PASS_CNT_MAX_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_PASS_CNT_MAX_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_PASS_CNT_MAX_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00000064 + (N) * 0x4) RXCPGEN_LOCK_DETECTOR_CFG_3 |
 | |     N[0..1]                                                   |
 | +---------------------------------------------------------------+
 | | bit  20   R    PPM_OFF_SIGN                                   |
 | | bit  19:1 R    PPM_OFF_VAL                                    |
 | | bit  0    R/W  PLL_RD                                         |
 | +---------------------------------------------------------------+
 | 
 | void   d8_rx_2x_field_PLL_RD_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_PLL_RD_get            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | UINT32 d8_rx_2x_field_PPM_OFF_SIGN_get      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | UINT32 d8_rx_2x_field_PPM_OFF_VAL_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | UINT32 d8_rx_2x_field_range_PPM_OFF_VAL_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x0000006c + (N) * 0x4) SIPO_BIST_CFG_REG |
 | |     N[0..1]                                         |
 | +-----------------------------------------------------+
 | | bit  6:2 R/W  SIPO_BIST_SELECT                      |
 | | bit  1   R/W  SIPO_BIST_TIME_REVERSE                |
 | | bit  0   R/W  SIPO_BIST_ENABLE                      |
 | +-----------------------------------------------------+
 | 
 | void   d8_rx_2x_field_SIPO_BIST_SELECT_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_SIPO_BIST_SELECT_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_SIPO_BIST_SELECT_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_SIPO_BIST_SELECT_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_SIPO_BIST_TIME_REVERSE_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_SIPO_BIST_TIME_REVERSE_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_SIPO_BIST_ENABLE_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_SIPO_BIST_ENABLE_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------------+
 | | Register (0x00000074 + (N) * 0x4) DATAPATH_CFG |
 | |     N[0..1]                                    |
 | +------------------------------------------------+
 | | bit  12:10 R/W  RCLKOUT_SEL                    |
 | | bit  9     R/W  LB_CLK_DEGLITCH_EN             |
 | | bit  8     R/W  BADJ_CLK_DEGLITCH_EN           |
 | | bit  7     R/W  ADC_CLK_DEGLITCH_EN            |
 | | bit  6     R/W  PATT_INSERT_EN                 |
 | | bit  5     R/W  P2S_DLB_EN                     |
 | | bit  4     R/W  RX2TX_DLB_EN                   |
 | | bit  3     R/W  GEN_SIPO_BUS_WIDTH             |
 | | bit  2:0   R/W  GEN_SIPO_DIV_RATIO             |
 | +------------------------------------------------+
 | 
 | void   d8_rx_2x_field_RCLKOUT_SEL_set              ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RCLKOUT_SEL_get              ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_RCLKOUT_SEL_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_RCLKOUT_SEL_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_LB_CLK_DEGLITCH_EN_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LB_CLK_DEGLITCH_EN_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_BADJ_CLK_DEGLITCH_EN_set     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_BADJ_CLK_DEGLITCH_EN_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_ADC_CLK_DEGLITCH_EN_set      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_CLK_DEGLITCH_EN_get      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_PATT_INSERT_EN_set           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_PATT_INSERT_EN_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_P2S_DLB_EN_set               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_P2S_DLB_EN_get               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RX2TX_DLB_EN_set             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RX2TX_DLB_EN_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_GEN_SIPO_BUS_WIDTH_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_GEN_SIPO_BUS_WIDTH_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_GEN_SIPO_DIV_RATIO_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_GEN_SIPO_DIV_RATIO_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_GEN_SIPO_DIV_RATIO_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_GEN_SIPO_DIV_RATIO_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x0000007c + (N) * 0x4) PATTERN_GENERATOR_CFG_1 |
 | |     N[0..1]                                               |
 | +-----------------------------------------------------------+
 | | bit  13:8 R/W  GEN_MON_BUS_WIDTH                          |
 | | bit  7:4  R/W  GEN_MODE                                   |
 | | bit  2    R/W  GEN_ERR_INJ                                |
 | | bit  1    R/W  GEN_INV                                    |
 | | bit  0    R/W  GEN_EN                                     |
 | +-----------------------------------------------------------+
 | 
 | void   d8_rx_2x_field_GEN_MON_BUS_WIDTH_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_GEN_MON_BUS_WIDTH_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_GEN_MON_BUS_WIDTH_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_GEN_MON_BUS_WIDTH_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_GEN_MODE_set                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_GEN_MODE_get                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_GEN_MODE_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_GEN_MODE_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_GEN_ERR_INJ_set             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_GEN_ERR_INJ_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_GEN_INV_set                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_GEN_INV_get                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_GEN_EN_set                  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_GEN_EN_get                  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x00000084 + (N) * 0x4) PATTERN_GEN_USER_PATTERN_1 |
 | |     N[0..1]                                                  |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  GEN_USR_PAT_LSB                               |
 | +--------------------------------------------------------------+
 | 
 | void   d8_rx_2x_lfield_GEN_USR_PAT_LSB_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value[2] )
 | void   d8_rx_2x_lfield_GEN_USR_PAT_LSB_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value[2] )
 | void   d8_rx_2x_lfield_range_GEN_USR_PAT_LSB_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_lfield_range_GEN_USR_PAT_LSB_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x0000008c + (N) * 0x4) PATTERN_GEN_USER_PATTERN_2 |
 | |     N[0..1]                                                  |
 | +--------------------------------------------------------------+
 | | bit  7:0 R/W  GEN_USR_PAT_MSB                                |
 | +--------------------------------------------------------------+
 | 
 | void   d8_rx_2x_field_GEN_USR_PAT_MSB_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_GEN_USR_PAT_MSB_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_GEN_USR_PAT_MSB_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_GEN_USR_PAT_MSB_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00000094 + (N) * 0x4) PATTERN_MON_CFG_1 |
 | |     N[0..1]                                         |
 | +-----------------------------------------------------+
 | | bit  7:4 R/W  MON_MODE                              |
 | | bit  2   R/W  MON_READ                              |
 | | bit  1   R/W  MON_INV                               |
 | | bit  0   R/W  MON_EN                                |
 | +-----------------------------------------------------+
 | 
 | void   d8_rx_2x_field_MON_MODE_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_MON_MODE_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_MON_MODE_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_MON_MODE_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_MON_READ_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_MON_READ_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_MON_INV_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_MON_INV_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_MON_EN_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_MON_EN_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x0000009c + (N) * 0x4) PATTERN_MON_USER_PATTERN_1 |
 | |     N[0..1]                                                  |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  MON_USR_PAT_LSB                               |
 | +--------------------------------------------------------------+
 | 
 | void   d8_rx_2x_lfield_MON_USR_PAT_LSB_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value[2] )
 | void   d8_rx_2x_lfield_MON_USR_PAT_LSB_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value[2] )
 | void   d8_rx_2x_lfield_range_MON_USR_PAT_LSB_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_lfield_range_MON_USR_PAT_LSB_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x000000a4 + (N) * 0x4) PATTERN_MON_USER_PATTERN_2 |
 | |     N[0..1]                                                  |
 | +--------------------------------------------------------------+
 | | bit  7:0 R/W  MON_USR_PAT_MSB                                |
 | +--------------------------------------------------------------+
 | 
 | void   d8_rx_2x_field_MON_USR_PAT_MSB_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_MON_USR_PAT_MSB_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_MON_USR_PAT_MSB_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_MON_USR_PAT_MSB_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000000ac + (N) * 0x4) PATTERN_MON_ERR_CNT_1 |
 | |     N[0..1]                                             |
 | +---------------------------------------------------------+
 | | bit  31:0 R  MON_ERR_CNT                                |
 | +---------------------------------------------------------+
 | 
 | void   d8_rx_2x_lfield_MON_ERR_CNT_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 d8_rx_2x_lfield_range_MON_ERR_CNT_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x000000b4 + (N) * 0x4) RAM_CAPTURE_CTRL_REG |
 | |     N[0..1]                                            |
 | +--------------------------------------------------------+
 | | bit  10:9 R/W  RAM_PARITY_ERR_INJECT                   |
 | | bit  7    R/W  RAM_SHUT_DOWN                           |
 | | bit  6    R/W  RAM_LIGHT_SLEEP                         |
 | | bit  5    R/W  RAMCAPT_DATA_RD_EN                      |
 | | bit  4    R/W  RAMCAPT_RST_RD_ADDR                     |
 | | bit  3    R/W  RAMCAPT_STOP_EVENT                      |
 | | bit  2    R/W  RAMCAPT_MODE                            |
 | | bit  1    R/W  RAMCAPT_START                           |
 | | bit  0    R/W  RAMCAPT_ENB                             |
 | +--------------------------------------------------------+
 | 
 | void   d8_rx_2x_field_RAM_PARITY_ERR_INJECT_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAM_PARITY_ERR_INJECT_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_RAM_PARITY_ERR_INJECT_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_RAM_PARITY_ERR_INJECT_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_RAM_SHUT_DOWN_set               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAM_SHUT_DOWN_get               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RAM_LIGHT_SLEEP_set             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAM_LIGHT_SLEEP_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RAMCAPT_DATA_RD_EN_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAMCAPT_DATA_RD_EN_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RAMCAPT_RST_RD_ADDR_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAMCAPT_RST_RD_ADDR_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RAMCAPT_STOP_EVENT_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAMCAPT_STOP_EVENT_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RAMCAPT_MODE_set                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAMCAPT_MODE_get                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RAMCAPT_START_set               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAMCAPT_START_get               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RAMCAPT_ENB_set                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAMCAPT_ENB_get                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------------------+
 | | Register (0x000000bc + (N) * 0x4) RAM_CAPTURE_READ_DATA_REG |
 | |     N[0..1]                                                 |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  RAMCAPT_RD_DATA                                |
 | +-------------------------------------------------------------+
 | 
 | void   d8_rx_2x_lfield_RAMCAPT_RD_DATA_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 d8_rx_2x_lfield_range_RAMCAPT_RD_DATA_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x000000c4 + (N) * 0x4) HHIST_CTRL_REG |
 | |     N[0..1]                                      |
 | +--------------------------------------------------+
 | | bit  11:8 R    HHIST_READBACK_ADDR               |
 | | bit  7    R    HHIST_PERBIN_VALUE_OVFL           |
 | | bit  3    R    HHIST_CAPT_RUNNING                |
 | | bit  2    R/W  HHIST_PERBIN_VALUE_RD_ENB         |
 | | bit  1    R/W  HHIST_CAPT_START                  |
 | | bit  0    R/W  HHIST_CAPT_ENB                    |
 | +--------------------------------------------------+
 | 
 | void   d8_rx_2x_field_HHIST_PERBIN_VALUE_RD_ENB_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_HHIST_PERBIN_VALUE_RD_ENB_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_HHIST_CAPT_START_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_HHIST_CAPT_START_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_HHIST_CAPT_ENB_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_HHIST_CAPT_ENB_get            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | UINT32 d8_rx_2x_field_HHIST_READBACK_ADDR_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | UINT32 d8_rx_2x_field_range_HHIST_READBACK_ADDR_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 d8_rx_2x_field_HHIST_PERBIN_VALUE_OVFL_get   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | UINT32 d8_rx_2x_field_HHIST_CAPT_RUNNING_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000000cc + (N) * 0x4) DIAG_HHIST_CTRL_REG_2 |
 | |     N[0..1]                                             |
 | +---------------------------------------------------------+
 | | bit  31:0 R/W  HHIST_CNT_TIM_LIMIT                      |
 | +---------------------------------------------------------+
 | 
 | void   d8_rx_2x_lfield_HHIST_CNT_TIM_LIMIT_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value[2] )
 | void   d8_rx_2x_lfield_HHIST_CNT_TIM_LIMIT_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value[2] )
 | void   d8_rx_2x_lfield_range_HHIST_CNT_TIM_LIMIT_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_lfield_range_HHIST_CNT_TIM_LIMIT_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x000000d4 + (N) * 0x4) DIAG_HHIST_VAL_REG |
 | |     N[0..1]                                          |
 | +------------------------------------------------------+
 | | bit  31:0 R  HHIST_PERBIN_VALUE                      |
 | +------------------------------------------------------+
 | 
 | void   d8_rx_2x_lfield_HHIST_PERBIN_VALUE_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 d8_rx_2x_lfield_range_HHIST_PERBIN_VALUE_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x000000dc + (N) * 0x4) RAM_OVERWRITE_CTRL_REG |
 | |     N[0..1]                                              |
 | +----------------------------------------------------------+
 | | bit  2 R/W  RAMCAPT_DATA_WR_EN                           |
 | | bit  1 R/W  RAMCAPT_RST_WR_ADDR                          |
 | | bit  0 R/W  RAMCAPT_PCBI_SEL                             |
 | +----------------------------------------------------------+
 | 
 | void   d8_rx_2x_field_RAMCAPT_DATA_WR_EN_set  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAMCAPT_DATA_WR_EN_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RAMCAPT_RST_WR_ADDR_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAMCAPT_RST_WR_ADDR_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RAMCAPT_PCBI_SEL_set    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RAMCAPT_PCBI_SEL_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------------------------+
 | | Register (0x000000e4 + (N) * 0x4) RAM_OVERWRITE_DATA_REG |
 | |     N[0..1]                                              |
 | +----------------------------------------------------------+
 | | bit  31:0 R/W  RAMCAPT_WR_DATA                           |
 | +----------------------------------------------------------+
 | 
 | void   d8_rx_2x_lfield_RAMCAPT_WR_DATA_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value[2] )
 | void   d8_rx_2x_lfield_RAMCAPT_WR_DATA_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value[2] )
 | void   d8_rx_2x_lfield_range_RAMCAPT_WR_DATA_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_lfield_range_RAMCAPT_WR_DATA_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x000000ec + (N) * 0x4) SFI5_1_RX_2X_MABC_CFG_1 |
 | |     N[0..1]                                               |
 | +-----------------------------------------------------------+
 | | bit  31   R/W  RX_LS_ENB                                  |
 | | bit  30   R/W  BADJ_ENB                                   |
 | | bit  29   R/W  ADC_CLK8_TEST_EN                           |
 | | bit  23   R/W  ADC_BYPASS_DIV8                            |
 | | bit  22   R/W  ADC_BYPASS_DIV4                            |
 | | bit  21   R/W  ADC_ATMSB                                  |
 | | bit  20   R/W  ADC_DAC_ENB                                |
 | | bit  19   R/W  ADC_DAC_RSTB                               |
 | | bit  18   R/W  ADC_DLB_EN                                 |
 | | bit  17   R/W  ADC_BIST                                   |
 | | bit  16   R/W  ADC_RSTB                                   |
 | | bit  15   R/W  ADC_ENB                                    |
 | | bit  14   R/W  RX_ADCCM_ENB                               |
 | | bit  13:8 R/W  ADC_MODE                                   |
 | | bit  7    R/W  JTAG_ENB                                   |
 | | bit  6    R/W  JTAG_ACINIT_N                              |
 | | bit  5    R/W  JTAG_ACINIT_P                              |
 | | bit  4    R/W  JTAG_ACFORCE                               |
 | | bit  3    R/W  JTAG_AC                                    |
 | | bit  2    R    DC_JTAG_OUTN                               |
 | | bit  1    R    DC_JTAG_OUTP                               |
 | | bit  0    R/W  RX_ATMSB                                   |
 | +-----------------------------------------------------------+
 | 
 | void   d8_rx_2x_field_RX_LS_ENB_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RX_LS_ENB_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_BADJ_ENB_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_BADJ_ENB_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_ADC_CLK8_TEST_EN_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_CLK8_TEST_EN_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_ADC_BYPASS_DIV8_set  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_BYPASS_DIV8_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_ADC_BYPASS_DIV4_set  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_BYPASS_DIV4_get  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_ADC_ATMSB_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_ATMSB_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_ADC_DAC_ENB_set      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_DAC_ENB_get      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_ADC_DAC_RSTB_set     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_DAC_RSTB_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_ADC_DLB_EN_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_DLB_EN_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_ADC_BIST_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_BIST_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_ADC_RSTB_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_RSTB_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_ADC_ENB_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_ENB_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RX_ADCCM_ENB_set     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RX_ADCCM_ENB_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_ADC_MODE_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_ADC_MODE_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_ADC_MODE_set   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_ADC_MODE_get   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_JTAG_ENB_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_JTAG_ENB_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_JTAG_ACINIT_N_set    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_JTAG_ACINIT_N_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_JTAG_ACINIT_P_set    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_JTAG_ACINIT_P_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_JTAG_ACFORCE_set     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_JTAG_ACFORCE_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_JTAG_AC_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_JTAG_AC_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RX_ATMSB_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RX_ATMSB_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | UINT32 d8_rx_2x_field_DC_JTAG_OUTN_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | UINT32 d8_rx_2x_field_DC_JTAG_OUTP_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------+
 | | Register 0x000000f4 RXCPGEN_VDDBIAS_CFG |
 | +-----------------------------------------+
 | | bit  31    R/W  RXCPGEN_CTRL_15         |
 | | bit  30    R/W  RXCPGEN_CTRL_14         |
 | | bit  29    R/W  RXCPGEN_CTRL_13         |
 | | bit  28    R/W  RXCPGEN_CTRL_12         |
 | | bit  27    R/W  RXCPGEN_CTRL_11         |
 | | bit  26    R/W  RXCPGEN_CTRL_10         |
 | | bit  25:24 R/W  RXCPGEN_CTRL_9_8        |
 | | bit  23    R/W  RXCPGEN_CTRL_7          |
 | | bit  22    R/W  RXCPGEN_CTRL_6          |
 | | bit  21:20 R/W  RXCPGEN_CTRL_5_4        |
 | | bit  19:17 R/W  RXCPGEN_CTRL_3_1        |
 | | bit  16    R/W  RXCPGEN_CTRL_0          |
 | | bit  15    R/W  RXCPGEN_MODE_7          |
 | | bit  14:13 R/W  RXCPGEN_MODE_6_5        |
 | | bit  12:11 R/W  RXCPGEN_MODE_4_3        |
 | | bit  10:8  R/W  RXCPGEN_MODE_2_0        |
 | | bit  7:6   R/W  VDDBIAS_MODE            |
 | | bit  5     R/W  VDDBIAS_ATMSB           |
 | | bit  4     R/W  VDDBIAS_ENB             |
 | | bit  3     R/W  RXCPGEN_ATMSB           |
 | | bit  2     R/W  RXCPGEN_ARSTB           |
 | | bit  1     R/W  RXCPGEN_RSTB            |
 | | bit  0     R/W  RXCPGEN_ENB             |
 | +-----------------------------------------+
 | 
 | void   d8_rx_2x_field_RXCPGEN_CTRL_15_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_CTRL_15_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_CTRL_14_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_CTRL_14_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_CTRL_13_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_CTRL_13_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_CTRL_12_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_CTRL_12_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_CTRL_11_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_CTRL_11_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_CTRL_10_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_CTRL_10_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_CTRL_9_8_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_CTRL_9_8_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_range_RXCPGEN_CTRL_9_8_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_RXCPGEN_CTRL_9_8_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_RXCPGEN_CTRL_7_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_CTRL_7_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_CTRL_6_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_CTRL_6_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_CTRL_5_4_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_CTRL_5_4_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_range_RXCPGEN_CTRL_5_4_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_RXCPGEN_CTRL_5_4_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_RXCPGEN_CTRL_3_1_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_CTRL_3_1_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_range_RXCPGEN_CTRL_3_1_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_RXCPGEN_CTRL_3_1_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_RXCPGEN_CTRL_0_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_CTRL_0_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_MODE_7_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_MODE_7_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_MODE_6_5_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_MODE_6_5_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_range_RXCPGEN_MODE_6_5_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_RXCPGEN_MODE_6_5_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_RXCPGEN_MODE_4_3_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_MODE_4_3_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_range_RXCPGEN_MODE_4_3_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_RXCPGEN_MODE_4_3_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_RXCPGEN_MODE_2_0_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_MODE_2_0_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_range_RXCPGEN_MODE_2_0_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_RXCPGEN_MODE_2_0_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_VDDBIAS_MODE_set           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_VDDBIAS_MODE_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_range_VDDBIAS_MODE_set     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_VDDBIAS_MODE_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_VDDBIAS_ATMSB_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_VDDBIAS_ATMSB_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_VDDBIAS_ENB_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_VDDBIAS_ENB_get            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_ATMSB_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_ATMSB_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_ARSTB_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_ARSTB_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_RSTB_set           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_RSTB_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void   d8_rx_2x_field_RXCPGEN_ENB_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_rx_2x_field_RXCPGEN_ENB_get            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | 
 | +----------------------------------------------------------+
 | | Register (0x000000f8 + (N) * 0x4) SAMPLE_CAPTURE_RAM_CFG |
 | |     N[0..1]                                              |
 | +----------------------------------------------------------+
 | | bit  8   R/W  SD                                         |
 | | bit  7:4 R/W  RM                                         |
 | | bit  3   R/W  DS                                         |
 | | bit  2   R/W  LS                                         |
 | | bit  1   R/W  RMEN                                       |
 | | bit  0   R/W  TEST1                                      |
 | +----------------------------------------------------------+
 | 
 | void   d8_rx_2x_field_SD_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_SD_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RM_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RM_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_RM_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_RM_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_DS_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_DS_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_LS_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LS_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_RMEN_set     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_RMEN_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_TEST1_set    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_TEST1_get    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------------+
 | | Register (0x00000100 + (N) * 0x4) LOS_OC_CFG_1 |
 | |     N[0..1]                                    |
 | +------------------------------------------------+
 | | bit  30:25 R    LOS_DAC                        |
 | | bit  24    R    LOS_CAL                        |
 | | bit  23:18 R/W  LOS_DAC_MAX_VAL                |
 | | bit  17:12 R/W  LOS_DAC_MIN_VAL                |
 | | bit  10:9  R/W  LOS_NUM_CYCLE                  |
 | | bit  8     R/W  LOS_CALIBRATE                  |
 | | bit  7:2   R/W  LOS_DAC_OVR                    |
 | | bit  1     R/W  LOS_DAC_OVR_EN                 |
 | | bit  0     R/W  LOS_CAL_OVR                    |
 | +------------------------------------------------+
 | 
 | void   d8_rx_2x_field_LOS_DAC_MAX_VAL_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_DAC_MAX_VAL_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_LOS_DAC_MAX_VAL_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_LOS_DAC_MAX_VAL_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_LOS_DAC_MIN_VAL_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_DAC_MIN_VAL_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_LOS_DAC_MIN_VAL_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_LOS_DAC_MIN_VAL_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_LOS_NUM_CYCLE_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_NUM_CYCLE_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_LOS_NUM_CYCLE_set   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_LOS_NUM_CYCLE_get   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_LOS_CALIBRATE_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_CALIBRATE_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_LOS_DAC_OVR_set           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_DAC_OVR_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_LOS_DAC_OVR_set     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_LOS_DAC_OVR_get     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_LOS_DAC_OVR_EN_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_DAC_OVR_EN_get        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_LOS_CAL_OVR_set           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_CAL_OVR_get           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | UINT32 d8_rx_2x_field_LOS_DAC_get               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | UINT32 d8_rx_2x_field_range_LOS_DAC_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 d8_rx_2x_field_LOS_CAL_get               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------------+
 | | Register (0x00000108 + (N) * 0x4) LOS_OC_CFG_2 |
 | |     N[0..1]                                    |
 | +------------------------------------------------+
 | | bit  9:0 R/W  LOS_SET_TIME                     |
 | +------------------------------------------------+
 | 
 | void   d8_rx_2x_field_LOS_SET_TIME_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_SET_TIME_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_LOS_SET_TIME_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_LOS_SET_TIME_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x00000110 + (N) * 0x4) LOS_CFG_ |
 | |     N[0..1]                                |
 | +--------------------------------------------+
 | | bit  22    R/W  LOS_ATMSB                  |
 | | bit  21    R/W  LOS_ENB                    |
 | | bit  20:16 R/W  LOS_REFAMP                 |
 | | bit  15    R/W  LOS_MODE_7                 |
 | | bit  14    R/W  LOS_MODE_6                 |
 | | bit  6:5   R/W  LOS_CTRL_6_5               |
 | | bit  4     R/W  LOS_CTRL_4                 |
 | | bit  3     R/W  LOS_CTRL_3                 |
 | | bit  2     R/W  LOS_CTRL_2                 |
 | | bit  1     R/W  LOS_CTRL_1                 |
 | | bit  0     R/W  LOS_CTRL_0                 |
 | +--------------------------------------------+
 | 
 | void   d8_rx_2x_field_LOS_ATMSB_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_ATMSB_get          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_LOS_ENB_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_ENB_get            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_LOS_REFAMP_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_REFAMP_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_LOS_REFAMP_set   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_LOS_REFAMP_get   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_LOS_MODE_7_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_MODE_7_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_LOS_MODE_6_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_MODE_6_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_LOS_CTRL_6_5_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_CTRL_6_5_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_LOS_CTRL_6_5_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_LOS_CTRL_6_5_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_LOS_CTRL_4_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_CTRL_4_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_LOS_CTRL_3_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_CTRL_3_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_LOS_CTRL_2_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_CTRL_2_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_LOS_CTRL_1_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_CTRL_1_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_LOS_CTRL_0_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_CTRL_0_get         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------------+
 | | Register (0x00000118 + (N) * 0x4) LOS_FLT_CFG_ |
 | |     N[0..1]                                    |
 | +------------------------------------------------+
 | | bit  23:16 R/W  LOS_FLT_THRESHOLD              |
 | | bit  15:8  R/W  LOS_FLT_CNT                    |
 | | bit  3:2   R/W  LOS_OVR_MODE                   |
 | | bit  1     R/W  LOS_OVR                        |
 | | bit  0     R/W  LOS_FLT_EN                     |
 | +------------------------------------------------+
 | 
 | void   d8_rx_2x_field_LOS_FLT_THRESHOLD_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_FLT_THRESHOLD_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_LOS_FLT_THRESHOLD_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_LOS_FLT_THRESHOLD_get ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_LOS_FLT_CNT_set             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_FLT_CNT_get             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_LOS_FLT_CNT_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_LOS_FLT_CNT_get       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_LOS_OVR_MODE_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_OVR_MODE_get            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_range_LOS_OVR_MODE_set      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_rx_2x_field_range_LOS_OVR_MODE_get      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_rx_2x_field_LOS_OVR_set                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_OVR_get                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_rx_2x_field_LOS_FLT_EN_set              ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_rx_2x_field_LOS_FLT_EN_get              ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _D8_RX_2X_IO_H
#define _D8_RX_2X_IO_H

#include "d8_rx_2x.h"
#include "d8_rx_2x_regs.h"
#include "d8_rx_2x_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 d8_rx_2x_buffer_init               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | void                 d8_rx_2x_buffer_flush              ( d8_rx_2x_buffer_t *b_ptr )
 | UINT32               d8_rx_2x_reg_read                  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 d8_rx_2x_reg_write                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 d8_rx_2x_field_set                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 d8_rx_2x_action_on_write_field_set ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 d8_rx_2x_burst_read                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 d8_rx_2x_burst_write               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_poll                      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 d8_rx_2x_reg_SW_RESET_CTRL_write                          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | void                 d8_rx_2x_reg_SW_RESET_CTRL_field_set                      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_SW_RESET_CTRL_read                           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | 
 | void                 d8_rx_2x_reg_ANALOG_TEST_CTRL_write                       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | void                 d8_rx_2x_reg_ANALOG_TEST_CTRL_field_set                   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_ANALOG_TEST_CTRL_read                        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | 
 | void                 d8_rx_2x_reg_RXCPGEN_VDDBIAS_CFG_write                    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | void                 d8_rx_2x_reg_RXCPGEN_VDDBIAS_CFG_field_set                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_RXCPGEN_VDDBIAS_CFG_read                     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | 
 | void                 d8_rx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_burst_write ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 d8_rx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_field_set   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 d8_rx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_burst_read  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               d8_rx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_read        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 N )
 | 
 | void                 d8_rx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_burst_write ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 d8_rx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_field_set   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 d8_rx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_burst_read  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               d8_rx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_read        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 N )
 | 
 | void                 d8_rx_2x_reg_DIAG_HHIST_CTRL_REG_2_array_burst_write      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 d8_rx_2x_reg_DIAG_HHIST_CTRL_REG_2_array_field_set        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 d8_rx_2x_reg_DIAG_HHIST_CTRL_REG_2_array_burst_read       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               d8_rx_2x_reg_DIAG_HHIST_CTRL_REG_2_array_read             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 N )
 | 
 | void                 d8_rx_2x_reg_RAM_OVERWRITE_DATA_REG_array_burst_write     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 d8_rx_2x_reg_RAM_OVERWRITE_DATA_REG_array_field_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 d8_rx_2x_reg_RAM_OVERWRITE_DATA_REG_array_burst_read      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               d8_rx_2x_reg_RAM_OVERWRITE_DATA_REG_array_read            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 N )
 | 
 | void                 d8_rx_2x_reg_EFUSE_INT_CFG_REG_array_write                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_EFUSE_INT_CFG_REG_array_field_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_EFUSE_INT_CFG_REG_array_read                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_GCOC_MEMORY_CFG_REG_array_write              ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_GCOC_MEMORY_CFG_REG_array_field_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_GCOC_MEMORY_CFG_REG_array_read               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_ADC_OC_CFG_1_array_write                     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_ADC_OC_CFG_1_array_field_set                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_ADC_OC_CFG_1_array_read                      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_ADC_OC_CFG_2_array_write                     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_ADC_OC_CFG_2_array_field_set                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_ADC_OC_CFG_2_array_read                      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_ADC_OC_CFG_3_array_write                     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_ADC_OC_CFG_3_array_field_set                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_ADC_OC_CFG_3_array_read                      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_CDRU_CFG__array_write                        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_CDRU_CFG__array_field_set                    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_CDRU_CFG__array_read                         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_RXCPGEN_LOCK_DETECTOR_CFG_1_array_write      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_RXCPGEN_LOCK_DETECTOR_CFG_1_array_field_set  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_RXCPGEN_LOCK_DETECTOR_CFG_1_array_read       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_RXCPGEN_LOCK_DETECTOR_CFG_2_array_write      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_RXCPGEN_LOCK_DETECTOR_CFG_2_array_field_set  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_RXCPGEN_LOCK_DETECTOR_CFG_2_array_read       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_SIPO_BIST_CFG_REG_array_write                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_SIPO_BIST_CFG_REG_array_field_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_SIPO_BIST_CFG_REG_array_read                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_DATAPATH_CFG_array_write                     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_DATAPATH_CFG_array_field_set                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_DATAPATH_CFG_array_read                      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_PATTERN_GENERATOR_CFG_1_array_write          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_PATTERN_GENERATOR_CFG_1_array_field_set      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_PATTERN_GENERATOR_CFG_1_array_read           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_write       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_field_set   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_read        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_PATTERN_MON_CFG_1_array_write                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_PATTERN_MON_CFG_1_array_field_set            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_PATTERN_MON_CFG_1_array_read                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_write       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_field_set   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_read        ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_RAM_CAPTURE_CTRL_REG_array_write             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_RAM_CAPTURE_CTRL_REG_array_field_set         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_RAM_CAPTURE_CTRL_REG_array_read              ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_HHIST_CTRL_REG_array_write                   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_HHIST_CTRL_REG_array_field_set               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_HHIST_CTRL_REG_array_read                    ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_RAM_OVERWRITE_CTRL_REG_array_write           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_RAM_OVERWRITE_CTRL_REG_array_field_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_RAM_OVERWRITE_CTRL_REG_array_read            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_SFI5_1_RX_2X_MABC_CFG_1_array_write          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_SFI5_1_RX_2X_MABC_CFG_1_array_field_set      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_SFI5_1_RX_2X_MABC_CFG_1_array_read           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_SAMPLE_CAPTURE_RAM_CFG_array_write           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_SAMPLE_CAPTURE_RAM_CFG_array_field_set       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_SAMPLE_CAPTURE_RAM_CFG_array_read            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_LOS_OC_CFG_1_array_write                     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_LOS_OC_CFG_1_array_field_set                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_LOS_OC_CFG_1_array_read                      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_LOS_OC_CFG_2_array_write                     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_LOS_OC_CFG_2_array_field_set                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_LOS_OC_CFG_2_array_read                      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_LOS_CFG__array_write                         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_LOS_CFG__array_field_set                     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_LOS_CFG__array_read                          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_LOS_FLT_CFG__array_write                     ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_LOS_FLT_CFG__array_field_set                 ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_LOS_FLT_CFG__array_read                      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_RXCPGEN_LOCK_DETECTOR_CFG_3_array_write      ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_rx_2x_reg_RXCPGEN_LOCK_DETECTOR_CFG_3_array_field_set  ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_RXCPGEN_LOCK_DETECTOR_CFG_3_array_read       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_rx_2x_reg_INT_REG_2_write                              ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | void                 d8_rx_2x_reg_INT_REG_2_action_on_write_field_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_INT_REG_2_read                               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_reg_INT_REG_2_poll                               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 d8_rx_2x_reg_INT_EN_REG_1_write                           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | void                 d8_rx_2x_reg_INT_EN_REG_1_field_set                       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_INT_EN_REG_1_read                            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | 
 | void                 d8_rx_2x_reg_INT_EN_REG_2_write                           ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | void                 d8_rx_2x_reg_INT_EN_REG_2_field_set                       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_INT_EN_REG_2_read                            ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | 
 | UINT32               d8_rx_2x_reg_INT_SUMMARY_REG_read                         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_reg_INT_SUMMARY_REG_poll                         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 d8_rx_2x_reg_INT_REG_1_write                              ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 value )
 | void                 d8_rx_2x_reg_INT_REG_1_action_on_write_field_set          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_rx_2x_reg_INT_REG_1_read                               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_reg_INT_REG_1_poll                               ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               d8_rx_2x_reg_INT_STAT_REG_1_read                          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_reg_INT_STAT_REG_1_poll                          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               d8_rx_2x_reg_INT_STAT_REG_2_read                          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_rx_2x_reg_INT_STAT_REG_2_poll                          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 d8_rx_2x_reg_PATTERN_MON_ERR_CNT_1_array_burst_read       ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               d8_rx_2x_reg_PATTERN_MON_ERR_CNT_1_array_read             ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 N )
 | 
 | void                 d8_rx_2x_reg_RAM_CAPTURE_READ_DATA_REG_array_burst_read   ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               d8_rx_2x_reg_RAM_CAPTURE_READ_DATA_REG_array_read         ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 N )
 | 
 | void                 d8_rx_2x_reg_DIAG_HHIST_VAL_REG_array_burst_read          ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               d8_rx_2x_reg_DIAG_HHIST_VAL_REG_array_read                ( d8_rx_2x_buffer_t *b_ptr, d8_rx_2x_handle_t *h_ptr, UINT32 N )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _D8_RX_2X_IO_H */
