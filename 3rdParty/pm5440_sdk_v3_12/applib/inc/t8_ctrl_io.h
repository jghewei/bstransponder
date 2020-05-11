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
 *     and register accessor functions for the t8_ctrl block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing t8_ctrl io functions is:
 | {
 |     t8_ctrl_buffer_t b_ptr[1];
 |     t8_ctrl_buffer_init( b_ptr, h_ptr );
 |     value = t8_ctrl_field_<FIELD>_get( b_ptr, h_ptr );
 |     t8_ctrl_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     t8_ctrl_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = t8_ctrl_field_<FIELD>_get( NULL, h_ptr );
 | t8_ctrl_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +------------------------------------------------------------------+
 | | Register (0x00000700 + (N) * 0x4) T8_6G_LR_MTSB_FSM_LANE_N_CFG_1 |
 | |     N[0..1]                                                      |
 | +------------------------------------------------------------------+
 | | bit  21 R/W  LOS_ENB                                             |
 | | bit  20 R/W  ADC_CALIB_DONE_OUT_OVR                              |
 | | bit  19 R/W  ADC_CALIB_REQUEST_OVR                               |
 | | bit  18 R/W  ADC_CAL_DAC_ENB_OVR                                 |
 | | bit  17 R/W  PGA_ENB_OVR                                         |
 | | bit  16 R/W  CALIB_FSM_OVR_EN                                    |
 | | bit  15 R/W  ADC_CALIB_FORCE                                     |
 | | bit  14 R/W  ADC_CALIB_START_EN                                  |
 | | bit  13 R/W  SYNC_RESET_CLBRT_FSM                                |
 | | bit  12 R/W  UPDATE_VALUES_OVR                                   |
 | | bit  11 R/W  UPDATE_VALUES_OVR_EN                                |
 | | bit  10 R/W  ADAPT_CLK_ENB_OVR                                   |
 | | bit  9  R/W  ADAPT_CLK_ENB_OVR_EN                                |
 | | bit  8  R/W  ADAPT_ENB_OVR                                       |
 | | bit  7  R/W  ADAPT_ENB_OVR_EN                                    |
 | | bit  6  R/W  FORCE_ENABLE_COUNTER_TRIG                           |
 | | bit  5  R/W  ADAPT_STATUS_0_OVR                                  |
 | | bit  4  R/W  ADAPT_STATUS_0_OVR_EN                               |
 | | bit  3  R/W  ADAPT_INIT_FORCE                                    |
 | | bit  2  R/W  TO_CMN_PCBI_OVR_EN                                  |
 | | bit  1  R/W  FROM_CMN_PCBI_OVR_EN                                |
 | | bit  0  R/W  RESET_ADPT_FSM                                      |
 | +------------------------------------------------------------------+
 | 
 | void   t8_ctrl_field_LOS_ENB_set                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_LOS_ENB_get                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADC_CALIB_DONE_OUT_OVR_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADC_CALIB_DONE_OUT_OVR_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADC_CALIB_REQUEST_OVR_set     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADC_CALIB_REQUEST_OVR_get     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADC_CAL_DAC_ENB_OVR_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADC_CAL_DAC_ENB_OVR_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_PGA_ENB_OVR_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_PGA_ENB_OVR_get               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_CALIB_FSM_OVR_EN_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_CALIB_FSM_OVR_EN_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADC_CALIB_FORCE_set           ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADC_CALIB_FORCE_get           ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADC_CALIB_START_EN_set        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADC_CALIB_START_EN_get        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_SYNC_RESET_CLBRT_FSM_set      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_SYNC_RESET_CLBRT_FSM_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_UPDATE_VALUES_OVR_set         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_UPDATE_VALUES_OVR_get         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_UPDATE_VALUES_OVR_EN_set      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_UPDATE_VALUES_OVR_EN_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADAPT_CLK_ENB_OVR_set         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_CLK_ENB_OVR_get         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADAPT_CLK_ENB_OVR_EN_set      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_CLK_ENB_OVR_EN_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADAPT_ENB_OVR_set             ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_ENB_OVR_get             ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADAPT_ENB_OVR_EN_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_ENB_OVR_EN_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_FORCE_ENABLE_COUNTER_TRIG_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_FORCE_ENABLE_COUNTER_TRIG_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADAPT_STATUS_0_OVR_set        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_STATUS_0_OVR_get        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADAPT_STATUS_0_OVR_EN_set     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_STATUS_0_OVR_EN_get     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADAPT_INIT_FORCE_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_INIT_FORCE_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_TO_CMN_PCBI_OVR_EN_set        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_TO_CMN_PCBI_OVR_EN_get        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_FROM_CMN_PCBI_OVR_EN_set      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_FROM_CMN_PCBI_OVR_EN_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_RESET_ADPT_FSM_set            ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_RESET_ADPT_FSM_get            ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------------------------------+
 | | Register (0x00000708 + (N) * 0x4) T8_6G_LR_MTSB_FSM_LANE_N_CFG_2 |
 | |     N[0..1]                                                      |
 | +------------------------------------------------------------------+
 | | bit  29:24 R/W  DP_FFE_M_PRELOAD_MODE3                           |
 | | bit  23:18 R/W  DP_FFE_M_PRELOAD_MODE2                           |
 | | bit  17:12 R/W  DP_FFE_M_PRELOAD_MODE1                           |
 | | bit  11:9  R/W  PRELOAD_LUT_INDEX_MODE3                          |
 | | bit  8:6   R/W  PRELOAD_LUT_INDEX_MODE2                          |
 | | bit  5:3   R/W  PRELOAD_LUT_INDEX_MODE1                          |
 | | bit  2     R/W  PRELOAD_LUT_EN_MODE3                             |
 | | bit  1     R/W  PRELOAD_LUT_EN_MODE2                             |
 | | bit  0     R/W  PRELOAD_LUT_EN_MODE1                             |
 | +------------------------------------------------------------------+
 | 
 | void   t8_ctrl_field_DP_FFE_M_PRELOAD_MODE3_set        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_M_PRELOAD_MODE3_get        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_range_DP_FFE_M_PRELOAD_MODE3_set  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_M_PRELOAD_MODE3_get  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_M_PRELOAD_MODE2_set        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_M_PRELOAD_MODE2_get        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_range_DP_FFE_M_PRELOAD_MODE2_set  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_M_PRELOAD_MODE2_get  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_M_PRELOAD_MODE1_set        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_M_PRELOAD_MODE1_get        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_range_DP_FFE_M_PRELOAD_MODE1_set  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_M_PRELOAD_MODE1_get  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_PRELOAD_LUT_INDEX_MODE3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_PRELOAD_LUT_INDEX_MODE3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_range_PRELOAD_LUT_INDEX_MODE3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_PRELOAD_LUT_INDEX_MODE3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_PRELOAD_LUT_INDEX_MODE2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_PRELOAD_LUT_INDEX_MODE2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_range_PRELOAD_LUT_INDEX_MODE2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_PRELOAD_LUT_INDEX_MODE2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_PRELOAD_LUT_INDEX_MODE1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_PRELOAD_LUT_INDEX_MODE1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_range_PRELOAD_LUT_INDEX_MODE1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_PRELOAD_LUT_INDEX_MODE1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_PRELOAD_LUT_EN_MODE3_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_PRELOAD_LUT_EN_MODE3_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_PRELOAD_LUT_EN_MODE2_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_PRELOAD_LUT_EN_MODE2_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_PRELOAD_LUT_EN_MODE1_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_PRELOAD_LUT_EN_MODE1_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------------------------------+
 | | Register (0x00000710 + (N) * 0x4) T8_6G_LR_MTSB_FSM_LANE_N_CFG_3 |
 | |     N[0..1]                                                      |
 | +------------------------------------------------------------------+
 | | bit  26    R/W  RX_PEAK_ENB_MODE3                                |
 | | bit  25    R/W  RX_PEAK_ENB_MODE2                                |
 | | bit  24    R/W  RX_PEAK_ENB_MODE1                                |
 | | bit  23:21 R/W  RX_PEAK_MODE3                                    |
 | | bit  20:18 R/W  RX_PEAK_MODE2                                    |
 | | bit  17:15 R/W  RX_PEAK_MODE1                                    |
 | | bit  14:10 R/W  PGA_GAIN_PRELOAD_MODE3                           |
 | | bit  9:5   R/W  PGA_GAIN_PRELOAD_MODE2                           |
 | | bit  4:0   R/W  PGA_GAIN_PRELOAD_MODE1                           |
 | +------------------------------------------------------------------+
 | 
 | void   t8_ctrl_field_RX_PEAK_ENB_MODE3_set            ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_RX_PEAK_ENB_MODE3_get            ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_RX_PEAK_ENB_MODE2_set            ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_RX_PEAK_ENB_MODE2_get            ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_RX_PEAK_ENB_MODE1_set            ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_RX_PEAK_ENB_MODE1_get            ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_RX_PEAK_MODE3_set                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_RX_PEAK_MODE3_get                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_range_RX_PEAK_MODE3_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RX_PEAK_MODE3_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RX_PEAK_MODE2_set                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_RX_PEAK_MODE2_get                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_range_RX_PEAK_MODE2_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RX_PEAK_MODE2_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RX_PEAK_MODE1_set                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_RX_PEAK_MODE1_get                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_range_RX_PEAK_MODE1_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RX_PEAK_MODE1_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_PGA_GAIN_PRELOAD_MODE3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_PGA_GAIN_PRELOAD_MODE3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_range_PGA_GAIN_PRELOAD_MODE3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_PGA_GAIN_PRELOAD_MODE3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_PGA_GAIN_PRELOAD_MODE2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_PGA_GAIN_PRELOAD_MODE2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_range_PGA_GAIN_PRELOAD_MODE2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_PGA_GAIN_PRELOAD_MODE2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_PGA_GAIN_PRELOAD_MODE1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_PGA_GAIN_PRELOAD_MODE1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_range_PGA_GAIN_PRELOAD_MODE1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_PGA_GAIN_PRELOAD_MODE1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00000720 + (N) * 0x4) STAT_INT_EN |
 | |     N[0..1]                                   |
 | +-----------------------------------------------+
 | | bit  7 R/W  START_ADAPT_ERROR_E               |
 | | bit  6 R/W  ADC_CALIB_DONE_IN_E               |
 | | bit  5 R/W  ADC_CALIB_START_E                 |
 | | bit  4 R/W  ADAPT_STATUS_0_E                  |
 | | bit  3 R/W  RX_ADAPT_DONE_E                   |
 | | bit  2 R/W  ADAPT_MODE_1_E                    |
 | | bit  1 R/W  ADAPT_MODE_0_E                    |
 | | bit  0 R/W  RX_ADAPT_START_E                  |
 | +-----------------------------------------------+
 | 
 | void   t8_ctrl_field_START_ADAPT_ERROR_E_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_START_ADAPT_ERROR_E_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADC_CALIB_DONE_IN_E_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADC_CALIB_DONE_IN_E_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADC_CALIB_START_E_set   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADC_CALIB_START_E_get   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADAPT_STATUS_0_E_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_STATUS_0_E_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_RX_ADAPT_DONE_E_set     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_RX_ADAPT_DONE_E_get     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADAPT_MODE_1_E_set      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_MODE_1_E_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_ADAPT_MODE_0_E_set      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_MODE_0_E_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_RX_ADAPT_START_E_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_RX_ADAPT_START_E_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------+
 | | Register (0x00000728 + (N) * 0x4) STAT_INT_EVENT |
 | |     N[0..1]                                      |
 | +--------------------------------------------------+
 | | bit  7 R/W  START_ADAPT_ERROR_I                  |
 | | bit  6 R/W  ADC_CALIB_DONE_IN_I                  |
 | | bit  5 R/W  ADC_CALIB_START_I                    |
 | | bit  4 R/W  ADAPT_STATUS_0_I                     |
 | | bit  3 R/W  RX_ADAPT_DONE_I                      |
 | | bit  2 R/W  ADAPT_MODE_1_I                       |
 | | bit  1 R/W  ADAPT_MODE_0_I                       |
 | | bit  0 R/W  RX_ADAPT_START_I                     |
 | +--------------------------------------------------+
 | 
 | void                 t8_ctrl_field_ADAPT_MODE_1_I_set_to_clear      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               t8_ctrl_field_ADAPT_MODE_1_I_get               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_ADAPT_MODE_1_I_poll              ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_ctrl_field_ADAPT_MODE_0_I_set_to_clear      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               t8_ctrl_field_ADAPT_MODE_0_I_get               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_ADAPT_MODE_0_I_poll              ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_ctrl_field_START_ADAPT_ERROR_I_set_to_clear ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               t8_ctrl_field_START_ADAPT_ERROR_I_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_START_ADAPT_ERROR_I_poll         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_ctrl_field_ADC_CALIB_DONE_IN_I_set_to_clear ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               t8_ctrl_field_ADC_CALIB_DONE_IN_I_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_ADC_CALIB_DONE_IN_I_poll         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_ctrl_field_ADC_CALIB_START_I_set_to_clear   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               t8_ctrl_field_ADC_CALIB_START_I_get            ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_ADC_CALIB_START_I_poll           ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_ctrl_field_ADAPT_STATUS_0_I_set_to_clear    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               t8_ctrl_field_ADAPT_STATUS_0_I_get             ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_ADAPT_STATUS_0_I_poll            ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_ctrl_field_RX_ADAPT_DONE_I_set_to_clear     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               t8_ctrl_field_RX_ADAPT_DONE_I_get              ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_RX_ADAPT_DONE_I_poll             ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_ctrl_field_RX_ADAPT_START_I_set_to_clear    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               t8_ctrl_field_RX_ADAPT_START_I_get             ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_RX_ADAPT_START_I_poll            ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register (0x00000730 + (N) * 0x4) STAT_INT |
 | |     N[0..1]                                |
 | +--------------------------------------------+
 | | bit  7 R  START_ADAPT_ERROR_V              |
 | | bit  6 R  ADC_CALIB_DONE_IN_V              |
 | | bit  5 R  ADC_CALIB_START_V                |
 | | bit  4 R  ADAPT_STATUS_0_V                 |
 | | bit  3 R  RX_ADAPT_DONE_V                  |
 | | bit  2 R  ADAPT_MODE_1_V                   |
 | | bit  1 R  ADAPT_MODE_0_V                   |
 | | bit  0 R  RX_ADAPT_START_V                 |
 | +--------------------------------------------+
 | 
 | UINT32               t8_ctrl_field_START_ADAPT_ERROR_V_get  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_START_ADAPT_ERROR_V_poll ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_ctrl_field_ADC_CALIB_DONE_IN_V_get  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_ADC_CALIB_DONE_IN_V_poll ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_ctrl_field_ADC_CALIB_START_V_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_ADC_CALIB_START_V_poll   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_ctrl_field_ADAPT_STATUS_0_V_get     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_ADAPT_STATUS_0_V_poll    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_ctrl_field_RX_ADAPT_DONE_V_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_RX_ADAPT_DONE_V_poll     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_ctrl_field_ADAPT_MODE_1_V_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_ADAPT_MODE_1_V_poll      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_ctrl_field_ADAPT_MODE_0_V_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_ADAPT_MODE_0_V_poll      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_ctrl_field_RX_ADAPT_START_V_get     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_field_RX_ADAPT_START_V_poll    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register (0x00000738 + (N) * 0x4)       |
 | | T8_6G_LR_MTSB_FSM_LANE_N_OBSERVABILITY_ |
 | |     N[0..1]                             |
 | +-----------------------------------------+
 | | bit  12   R    ADC_CALIB_REQUEST_OBS    |
 | | bit  11   R    ADC_CALIB_DONE_IN_OBS    |
 | | bit  10:9 R    CLBRT_STATE_OBS          |
 | | bit  8:6  R    ADPT_STATE_OBS           |
 | | bit  5    R/W  CAPTURE_REQ_ALL_STATES   |
 | | bit  4    R    RX_ADAPT_DONE_OBS        |
 | | bit  3:2  R    ADAPT_MODE_OBS           |
 | | bit  1    R    RX_ADAPT_START_OBS       |
 | | bit  0    R/W  CAPTURE_REQ_CMN_PCBI     |
 | +-----------------------------------------+
 | 
 | void   t8_ctrl_field_CAPTURE_REQ_ALL_STATES_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_CAPTURE_REQ_ALL_STATES_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | void   t8_ctrl_field_CAPTURE_REQ_CMN_PCBI_set   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 t8_ctrl_field_CAPTURE_REQ_CMN_PCBI_get   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | UINT32 t8_ctrl_field_ADC_CALIB_REQUEST_OBS_get  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | UINT32 t8_ctrl_field_ADC_CALIB_DONE_IN_OBS_get  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | UINT32 t8_ctrl_field_CLBRT_STATE_OBS_get        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | UINT32 t8_ctrl_field_range_CLBRT_STATE_OBS_get  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_ctrl_field_ADPT_STATE_OBS_get         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | UINT32 t8_ctrl_field_range_ADPT_STATE_OBS_get   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_ctrl_field_RX_ADAPT_DONE_OBS_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | UINT32 t8_ctrl_field_ADAPT_MODE_OBS_get         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | UINT32 t8_ctrl_field_range_ADAPT_MODE_OBS_get   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_ctrl_field_RX_ADAPT_START_OBS_get     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000740 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_1 |
 | +----------------------------------------------------+
 | | bit  15  R/W  FFE_CONSTRAINED_MODE3                |
 | | bit  14  R/W  FFE_CONSTRAINED_MODE2                |
 | | bit  13  R/W  FFE_CONSTRAINED_MODE1                |
 | | bit  12  R/W  DP_ADAPT_ORDER_MODE3                 |
 | | bit  11  R/W  DP_ADAPT_ORDER_MODE2                 |
 | | bit  10  R/W  DP_ADAPT_ORDER_MODE1                 |
 | | bit  9   R/W  ADAPT_L0_ENB_MODE3                   |
 | | bit  8   R/W  ADAPT_L0_ENB_MODE2                   |
 | | bit  7   R/W  ADAPT_L0_ENB_MODE1                   |
 | | bit  6   R/W  DSS_ENB_MODE3                        |
 | | bit  5   R/W  DSS_ENB_MODE2                        |
 | | bit  4   R/W  DSS_ENB_MODE1                        |
 | | bit  3:0 R/W  ADAPT_L0_CNTR                        |
 | +----------------------------------------------------+
 | 
 | void   t8_ctrl_field_FFE_CONSTRAINED_MODE3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_FFE_CONSTRAINED_MODE3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_FFE_CONSTRAINED_MODE2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_FFE_CONSTRAINED_MODE2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_FFE_CONSTRAINED_MODE1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_FFE_CONSTRAINED_MODE1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_DP_ADAPT_ORDER_MODE3_set  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_ADAPT_ORDER_MODE3_get  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_DP_ADAPT_ORDER_MODE2_set  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_ADAPT_ORDER_MODE2_get  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_DP_ADAPT_ORDER_MODE1_set  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_ADAPT_ORDER_MODE1_get  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_ADAPT_L0_ENB_MODE3_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_L0_ENB_MODE3_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_ADAPT_L0_ENB_MODE2_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_L0_ENB_MODE2_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_ADAPT_L0_ENB_MODE1_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_L0_ENB_MODE1_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_DSS_ENB_MODE3_set         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DSS_ENB_MODE3_get         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_DSS_ENB_MODE2_set         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DSS_ENB_MODE2_get         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_DSS_ENB_MODE1_set         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DSS_ENB_MODE1_get         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_ADAPT_L0_CNTR_set         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_L0_CNTR_get         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_L0_CNTR_set   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_L0_CNTR_get   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000744 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_2 |
 | +----------------------------------------------------+
 | | bit  17:15 R/W  MU_STEP_GAIN_PASS5                 |
 | | bit  14:12 R/W  MU_STEP_GAIN_PASS4                 |
 | | bit  11:9  R/W  MU_STEP_GAIN_PASS3                 |
 | | bit  8:6   R/W  MU_STEP_GAIN_PASS2                 |
 | | bit  5:3   R/W  MU_STEP_GAIN_PASS1                 |
 | | bit  2:0   R/W  MU_STEP_GAIN_PASS0                 |
 | +----------------------------------------------------+
 | 
 | void   t8_ctrl_field_MU_STEP_GAIN_PASS5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_GAIN_PASS5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_GAIN_PASS5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_GAIN_PASS5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_GAIN_PASS4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_GAIN_PASS4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_GAIN_PASS4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_GAIN_PASS4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_GAIN_PASS3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_GAIN_PASS3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_GAIN_PASS3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_GAIN_PASS3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_GAIN_PASS2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_GAIN_PASS2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_GAIN_PASS2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_GAIN_PASS2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_GAIN_PASS1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_GAIN_PASS1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_GAIN_PASS1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_GAIN_PASS1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_GAIN_PASS0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_GAIN_PASS0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_GAIN_PASS0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_GAIN_PASS0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000748 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_3 |
 | +----------------------------------------------------+
 | | bit  17:15 R/W  MU_STEP_OFFSET_PASS5               |
 | | bit  14:12 R/W  MU_STEP_OFFSET_PASS4               |
 | | bit  11:9  R/W  MU_STEP_OFFSET_PASS3               |
 | | bit  8:6   R/W  MU_STEP_OFFSET_PASS2               |
 | | bit  5:3   R/W  MU_STEP_OFFSET_PASS1               |
 | | bit  2:0   R/W  MU_STEP_OFFSET_PASS0               |
 | +----------------------------------------------------+
 | 
 | void   t8_ctrl_field_MU_STEP_OFFSET_PASS5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_OFFSET_PASS5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_OFFSET_PASS5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_OFFSET_PASS5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_OFFSET_PASS4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_OFFSET_PASS4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_OFFSET_PASS4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_OFFSET_PASS4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_OFFSET_PASS3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_OFFSET_PASS3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_OFFSET_PASS3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_OFFSET_PASS3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_OFFSET_PASS2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_OFFSET_PASS2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_OFFSET_PASS2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_OFFSET_PASS2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_OFFSET_PASS1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_OFFSET_PASS1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_OFFSET_PASS1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_OFFSET_PASS1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_OFFSET_PASS0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_OFFSET_PASS0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_OFFSET_PASS0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_OFFSET_PASS0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000074c T8_6G_LR_MTSB_FSM_GLOBAL_CFG_4 |
 | +----------------------------------------------------+
 | | bit  17:15 R/W  MU_STEP_COEF_PASS5                 |
 | | bit  14:12 R/W  MU_STEP_COEF_PASS4                 |
 | | bit  11:9  R/W  MU_STEP_COEF_PASS3                 |
 | | bit  8:6   R/W  MU_STEP_COEF_PASS2                 |
 | | bit  5:3   R/W  MU_STEP_COEF_PASS1                 |
 | | bit  2:0   R/W  MU_STEP_COEF_PASS0                 |
 | +----------------------------------------------------+
 | 
 | void   t8_ctrl_field_MU_STEP_COEF_PASS5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_COEF_PASS5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_COEF_PASS5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_COEF_PASS5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_COEF_PASS4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_COEF_PASS4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_COEF_PASS4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_COEF_PASS4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_COEF_PASS3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_COEF_PASS3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_COEF_PASS3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_COEF_PASS3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_COEF_PASS2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_COEF_PASS2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_COEF_PASS2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_COEF_PASS2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_COEF_PASS1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_COEF_PASS1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_COEF_PASS1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_COEF_PASS1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_COEF_PASS0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_COEF_PASS0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_COEF_PASS0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_COEF_PASS0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000750 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_5 |
 | +----------------------------------------------------+
 | | bit  17:15 R/W  MU_STEP_ADC_MAP_PASS5              |
 | | bit  14:12 R/W  MU_STEP_ADC_MAP_PASS4              |
 | | bit  11:9  R/W  MU_STEP_ADC_MAP_PASS3              |
 | | bit  8:6   R/W  MU_STEP_ADC_MAP_PASS2              |
 | | bit  5:3   R/W  MU_STEP_ADC_MAP_PASS1              |
 | | bit  2:0   R/W  MU_STEP_ADC_MAP_PASS0              |
 | +----------------------------------------------------+
 | 
 | void   t8_ctrl_field_MU_STEP_ADC_MAP_PASS5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_ADC_MAP_PASS5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_ADC_MAP_PASS5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_ADC_MAP_PASS5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_ADC_MAP_PASS4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_ADC_MAP_PASS4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_ADC_MAP_PASS4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_ADC_MAP_PASS4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_ADC_MAP_PASS3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_ADC_MAP_PASS3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_ADC_MAP_PASS3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_ADC_MAP_PASS3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_ADC_MAP_PASS2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_ADC_MAP_PASS2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_ADC_MAP_PASS2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_ADC_MAP_PASS2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_ADC_MAP_PASS1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_ADC_MAP_PASS1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_ADC_MAP_PASS1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_ADC_MAP_PASS1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_MU_STEP_ADC_MAP_PASS0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_MU_STEP_ADC_MAP_PASS0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_MU_STEP_ADC_MAP_PASS0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_MU_STEP_ADC_MAP_PASS0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000754 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_6 |
 | +----------------------------------------------------+
 | | bit  29:26 R/W  RR_PERIOD_COEF_MODE3               |
 | | bit  25:22 R/W  RR_PERIOD_COEF_MODE2               |
 | | bit  21:18 R/W  RR_PERIOD_COEF_MODE1               |
 | | bit  17:15 R/W  ADAPT_DELAY_MODE3_PASS1TO5         |
 | | bit  14:12 R/W  ADAPT_DELAY_MODE3_PASS0            |
 | | bit  11:9  R/W  ADAPT_DELAY_MODE2_PASS1TO5         |
 | | bit  8:6   R/W  ADAPT_DELAY_MODE2_PASS0            |
 | | bit  5:3   R/W  ADAPT_DELAY_MODE1_PASS1TO5         |
 | | bit  2:0   R/W  ADAPT_DELAY_MODE1_PASS0            |
 | +----------------------------------------------------+
 | 
 | void   t8_ctrl_field_RR_PERIOD_COEF_MODE3_set             ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_COEF_MODE3_get             ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_COEF_MODE3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_COEF_MODE3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RR_PERIOD_COEF_MODE2_set             ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_COEF_MODE2_get             ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_COEF_MODE2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_COEF_MODE2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RR_PERIOD_COEF_MODE1_set             ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_COEF_MODE1_get             ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_COEF_MODE1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_COEF_MODE1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_DELAY_MODE3_PASS1TO5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_DELAY_MODE3_PASS1TO5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_DELAY_MODE3_PASS1TO5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_DELAY_MODE3_PASS1TO5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_DELAY_MODE3_PASS0_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_DELAY_MODE3_PASS0_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_DELAY_MODE3_PASS0_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_DELAY_MODE3_PASS0_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_DELAY_MODE2_PASS1TO5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_DELAY_MODE2_PASS1TO5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_DELAY_MODE2_PASS1TO5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_DELAY_MODE2_PASS1TO5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_DELAY_MODE2_PASS0_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_DELAY_MODE2_PASS0_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_DELAY_MODE2_PASS0_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_DELAY_MODE2_PASS0_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_DELAY_MODE1_PASS1TO5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_DELAY_MODE1_PASS1TO5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_DELAY_MODE1_PASS1TO5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_DELAY_MODE1_PASS1TO5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_DELAY_MODE1_PASS0_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_DELAY_MODE1_PASS0_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_DELAY_MODE1_PASS0_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_DELAY_MODE1_PASS0_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000758 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_7 |
 | +----------------------------------------------------+
 | | bit  23:20 R/W  RR_PERIOD_DSS_MODE3                |
 | | bit  19:16 R/W  RR_PERIOD_DSS_MODE2                |
 | | bit  15:12 R/W  RR_PERIOD_DSS_MODE1                |
 | | bit  11:8  R/W  RR_PERIOD_ADC_MAP_MODE3            |
 | | bit  7:4   R/W  RR_PERIOD_ADC_MAP_MODE2            |
 | | bit  3:0   R/W  RR_PERIOD_ADC_MAP_MODE1            |
 | +----------------------------------------------------+
 | 
 | void   t8_ctrl_field_RR_PERIOD_DSS_MODE3_set           ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_DSS_MODE3_get           ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_DSS_MODE3_set     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_DSS_MODE3_get     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RR_PERIOD_DSS_MODE2_set           ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_DSS_MODE2_get           ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_DSS_MODE2_set     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_DSS_MODE2_get     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RR_PERIOD_DSS_MODE1_set           ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_DSS_MODE1_get           ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_DSS_MODE1_set     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_DSS_MODE1_get     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RR_PERIOD_ADC_MAP_MODE3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_ADC_MAP_MODE3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_ADC_MAP_MODE3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_ADC_MAP_MODE3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RR_PERIOD_ADC_MAP_MODE2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_ADC_MAP_MODE2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_ADC_MAP_MODE2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_ADC_MAP_MODE2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RR_PERIOD_ADC_MAP_MODE1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_ADC_MAP_MODE1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_ADC_MAP_MODE1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_ADC_MAP_MODE1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000075c T8_6G_LR_MTSB_FSM_GLOBAL_CFG_8 |
 | +----------------------------------------------------+
 | | bit  23:20 R/W  RR_PERIOD_GAIN_MODE3               |
 | | bit  19:16 R/W  RR_PERIOD_GAIN_MODE2               |
 | | bit  15:12 R/W  RR_PERIOD_GAIN_MODE1               |
 | | bit  11:8  R/W  RR_PERIOD_OFFSET_MODE3             |
 | | bit  7:4   R/W  RR_PERIOD_OFFSET_MODE2             |
 | | bit  3:0   R/W  RR_PERIOD_OFFSET_MODE1             |
 | +----------------------------------------------------+
 | 
 | void   t8_ctrl_field_RR_PERIOD_GAIN_MODE3_set         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_GAIN_MODE3_get         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_GAIN_MODE3_set   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_GAIN_MODE3_get   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RR_PERIOD_GAIN_MODE2_set         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_GAIN_MODE2_get         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_GAIN_MODE2_set   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_GAIN_MODE2_get   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RR_PERIOD_GAIN_MODE1_set         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_GAIN_MODE1_get         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_GAIN_MODE1_set   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_GAIN_MODE1_get   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RR_PERIOD_OFFSET_MODE3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_OFFSET_MODE3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_OFFSET_MODE3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_OFFSET_MODE3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RR_PERIOD_OFFSET_MODE2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_OFFSET_MODE2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_OFFSET_MODE2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_OFFSET_MODE2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RR_PERIOD_OFFSET_MODE1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RR_PERIOD_OFFSET_MODE1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_RR_PERIOD_OFFSET_MODE1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_RR_PERIOD_OFFSET_MODE1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000760 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_9 |
 | +----------------------------------------------------+
 | | bit  27:21 R/W  DFE_COEF3_PRELOAD                  |
 | | bit  20:14 R/W  DFE_COEF2_PRELOAD                  |
 | | bit  13:7  R/W  DFE_COEF1_PRELOAD                  |
 | | bit  6:0   R/W  DFE_COEF0_PRELOAD                  |
 | +----------------------------------------------------+
 | 
 | void   t8_ctrl_field_DFE_COEF3_PRELOAD_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DFE_COEF3_PRELOAD_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DFE_COEF3_PRELOAD_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DFE_COEF3_PRELOAD_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DFE_COEF2_PRELOAD_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DFE_COEF2_PRELOAD_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DFE_COEF2_PRELOAD_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DFE_COEF2_PRELOAD_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DFE_COEF1_PRELOAD_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DFE_COEF1_PRELOAD_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DFE_COEF1_PRELOAD_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DFE_COEF1_PRELOAD_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DFE_COEF0_PRELOAD_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DFE_COEF0_PRELOAD_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DFE_COEF0_PRELOAD_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DFE_COEF0_PRELOAD_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000764 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_10 |
 | +-----------------------------------------------------+
 | | bit  21:14 R/W  PGA_OFFSET_PRELOAD                  |
 | | bit  13:7  R/W  DFE_COEF5_PRELOAD                   |
 | | bit  6:0   R/W  DFE_COEF4_PRELOAD                   |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_PGA_OFFSET_PRELOAD_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_PGA_OFFSET_PRELOAD_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_PGA_OFFSET_PRELOAD_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_PGA_OFFSET_PRELOAD_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DFE_COEF5_PRELOAD_set        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DFE_COEF5_PRELOAD_get        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DFE_COEF5_PRELOAD_set  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DFE_COEF5_PRELOAD_get  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DFE_COEF4_PRELOAD_set        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DFE_COEF4_PRELOAD_get        ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DFE_COEF4_PRELOAD_set  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DFE_COEF4_PRELOAD_get  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000768 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_11 |
 | +-----------------------------------------------------+
 | | bit  30:26 R/W  TR_FFE_B_PRELOAD_PCBI               |
 | | bit  25:21 R/W  TR_FFE_A_PRELOAD_PCBI               |
 | | bit  20:14 R/W  DP_FFE_C_PRELOAD_PCBI               |
 | | bit  13:7  R/W  DP_FFE_B_PRELOAD_PCBI               |
 | | bit  6:0   R/W  DP_FFE_A_PRELOAD_PCBI               |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_TR_FFE_B_PRELOAD_PCBI_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_B_PRELOAD_PCBI_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_B_PRELOAD_PCBI_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_B_PRELOAD_PCBI_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_TR_FFE_A_PRELOAD_PCBI_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_A_PRELOAD_PCBI_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_A_PRELOAD_PCBI_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_A_PRELOAD_PCBI_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_C_PRELOAD_PCBI_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_C_PRELOAD_PCBI_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_C_PRELOAD_PCBI_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_C_PRELOAD_PCBI_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_B_PRELOAD_PCBI_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_B_PRELOAD_PCBI_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_B_PRELOAD_PCBI_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_B_PRELOAD_PCBI_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_A_PRELOAD_PCBI_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_A_PRELOAD_PCBI_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_A_PRELOAD_PCBI_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_A_PRELOAD_PCBI_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x0000076c T8_6G_LR_MTSB_FSM_GLOBAL_CFG_12 |
 | +-----------------------------------------------------+
 | | bit  15:14 R/W  LF_INTEG_OUT_MODE_NORMAL            |
 | | bit  13:12 R/W  LF_INTEG_OUT_MODE_LOS               |
 | | bit  11:4  R/W  LF_INTEG_OUT_OVR                    |
 | | bit  3:0   R/W  ADC_MAP_PRELOAD                     |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_LF_INTEG_OUT_MODE_NORMAL_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_INTEG_OUT_MODE_NORMAL_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_LF_INTEG_OUT_MODE_NORMAL_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_LF_INTEG_OUT_MODE_NORMAL_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_LF_INTEG_OUT_MODE_LOS_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_INTEG_OUT_MODE_LOS_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_LF_INTEG_OUT_MODE_LOS_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_LF_INTEG_OUT_MODE_LOS_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_LF_INTEG_OUT_OVR_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_INTEG_OUT_OVR_get               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_LF_INTEG_OUT_OVR_set         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_LF_INTEG_OUT_OVR_get         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADC_MAP_PRELOAD_set                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADC_MAP_PRELOAD_get                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADC_MAP_PRELOAD_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADC_MAP_PRELOAD_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000770 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_13 |
 | +-----------------------------------------------------+
 | | bit  26:24 R/W  CLIP_RATE_THRESH_MODE3              |
 | | bit  23:21 R/W  CLIP_RATE_THRESH_MODE2              |
 | | bit  20:18 R/W  CLIP_RATE_THRESH_MODE1              |
 | | bit  17:12 R/W  CLIP_RATE_AMP_MODE3                 |
 | | bit  11:6  R/W  CLIP_RATE_AMP_MODE2                 |
 | | bit  5:0   R/W  CLIP_RATE_AMP_MODE1                 |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_CLIP_RATE_THRESH_MODE3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_CLIP_RATE_THRESH_MODE3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_CLIP_RATE_THRESH_MODE3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_CLIP_RATE_THRESH_MODE3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_CLIP_RATE_THRESH_MODE2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_CLIP_RATE_THRESH_MODE2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_CLIP_RATE_THRESH_MODE2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_CLIP_RATE_THRESH_MODE2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_CLIP_RATE_THRESH_MODE1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_CLIP_RATE_THRESH_MODE1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_CLIP_RATE_THRESH_MODE1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_CLIP_RATE_THRESH_MODE1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_CLIP_RATE_AMP_MODE3_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_CLIP_RATE_AMP_MODE3_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_CLIP_RATE_AMP_MODE3_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_CLIP_RATE_AMP_MODE3_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_CLIP_RATE_AMP_MODE2_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_CLIP_RATE_AMP_MODE2_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_CLIP_RATE_AMP_MODE2_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_CLIP_RATE_AMP_MODE2_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_CLIP_RATE_AMP_MODE1_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_CLIP_RATE_AMP_MODE1_get          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_CLIP_RATE_AMP_MODE1_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_CLIP_RATE_AMP_MODE1_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000774 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_14 |
 | +-----------------------------------------------------+
 | | bit  5 R/W  PATH_PATTERN_ENB_MODE3_PASS5            |
 | | bit  4 R/W  PATH_PATTERN_ENB_MODE3_PASS0TO4         |
 | | bit  3 R/W  PATH_PATTERN_ENB_MODE2_PASS5            |
 | | bit  2 R/W  PATH_PATTERN_ENB_MODE2_PASS0TO4         |
 | | bit  1 R/W  PATH_PATTERN_ENB_MODE1_PASS5            |
 | | bit  0 R/W  PATH_PATTERN_ENB_MODE1_PASS0TO4         |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_PATH_PATTERN_ENB_MODE3_PASS5_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_PATH_PATTERN_ENB_MODE3_PASS5_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_PATH_PATTERN_ENB_MODE3_PASS0TO4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_PATH_PATTERN_ENB_MODE3_PASS0TO4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_PATH_PATTERN_ENB_MODE2_PASS5_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_PATH_PATTERN_ENB_MODE2_PASS5_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_PATH_PATTERN_ENB_MODE2_PASS0TO4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_PATH_PATTERN_ENB_MODE2_PASS0TO4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_PATH_PATTERN_ENB_MODE1_PASS5_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_PATH_PATTERN_ENB_MODE1_PASS5_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_PATH_PATTERN_ENB_MODE1_PASS0TO4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_PATH_PATTERN_ENB_MODE1_PASS0TO4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000778 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_15 |
 | +-----------------------------------------------------+
 | | bit  23:20 R/W  ADAPT_LOOPS_MODE1_PASS5             |
 | | bit  19:16 R/W  ADAPT_LOOPS_MODE1_PASS4             |
 | | bit  15:12 R/W  ADAPT_LOOPS_MODE1_PASS3             |
 | | bit  11:8  R/W  ADAPT_LOOPS_MODE1_PASS2             |
 | | bit  7:4   R/W  ADAPT_LOOPS_MODE1_PASS1             |
 | | bit  3:0   R/W  ADAPT_LOOPS_MODE1_PASS0             |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE1_PASS5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE1_PASS5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE1_PASS5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE1_PASS5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE1_PASS4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE1_PASS4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE1_PASS4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE1_PASS4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE1_PASS3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE1_PASS3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE1_PASS3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE1_PASS3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE1_PASS2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE1_PASS2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE1_PASS2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE1_PASS2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE1_PASS1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE1_PASS1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE1_PASS1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE1_PASS1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE1_PASS0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE1_PASS0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE1_PASS0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE1_PASS0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x0000077c T8_6G_LR_MTSB_FSM_GLOBAL_CFG_16 |
 | +-----------------------------------------------------+
 | | bit  23:20 R/W  ADAPT_LOOPS_MODE2_PASS5             |
 | | bit  19:16 R/W  ADAPT_LOOPS_MODE2_PASS4             |
 | | bit  15:12 R/W  ADAPT_LOOPS_MODE2_PASS3             |
 | | bit  11:8  R/W  ADAPT_LOOPS_MODE2_PASS2             |
 | | bit  7:4   R/W  ADAPT_LOOPS_MODE2_PASS1             |
 | | bit  3:0   R/W  ADAPT_LOOPS_MODE2_PASS0             |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE2_PASS5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE2_PASS5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE2_PASS5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE2_PASS5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE2_PASS4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE2_PASS4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE2_PASS4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE2_PASS4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE2_PASS3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE2_PASS3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE2_PASS3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE2_PASS3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE2_PASS2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE2_PASS2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE2_PASS2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE2_PASS2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE2_PASS1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE2_PASS1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE2_PASS1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE2_PASS1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE2_PASS0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE2_PASS0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE2_PASS0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE2_PASS0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000780 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_17 |
 | +-----------------------------------------------------+
 | | bit  23:20 R/W  ADAPT_LOOPS_MODE3_PASS5             |
 | | bit  19:16 R/W  ADAPT_LOOPS_MODE3_PASS4             |
 | | bit  15:12 R/W  ADAPT_LOOPS_MODE3_PASS3             |
 | | bit  11:8  R/W  ADAPT_LOOPS_MODE3_PASS2             |
 | | bit  7:4   R/W  ADAPT_LOOPS_MODE3_PASS1             |
 | | bit  3:0   R/W  ADAPT_LOOPS_MODE3_PASS0             |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE3_PASS5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE3_PASS5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE3_PASS5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE3_PASS5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE3_PASS4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE3_PASS4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE3_PASS4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE3_PASS4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE3_PASS3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE3_PASS3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE3_PASS3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE3_PASS3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE3_PASS2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE3_PASS2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE3_PASS2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE3_PASS2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE3_PASS1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE3_PASS1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE3_PASS1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE3_PASS1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_ADAPT_LOOPS_MODE3_PASS0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_LOOPS_MODE3_PASS0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_LOOPS_MODE3_PASS0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_LOOPS_MODE3_PASS0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000784 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_18 |
 | +-----------------------------------------------------+
 | | bit  29:15 R/W  OBJECTS_EN_MODE1_PASS1              |
 | | bit  14:0  R/W  OBJECTS_EN_MODE1_PASS0              |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_OBJECTS_EN_MODE1_PASS1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE1_PASS1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE1_PASS1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE1_PASS1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_OBJECTS_EN_MODE1_PASS0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE1_PASS0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE1_PASS0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE1_PASS0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000788 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_19 |
 | +-----------------------------------------------------+
 | | bit  29:15 R/W  OBJECTS_EN_MODE1_PASS3              |
 | | bit  14:0  R/W  OBJECTS_EN_MODE1_PASS2              |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_OBJECTS_EN_MODE1_PASS3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE1_PASS3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE1_PASS3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE1_PASS3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_OBJECTS_EN_MODE1_PASS2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE1_PASS2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE1_PASS2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE1_PASS2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x0000078c T8_6G_LR_MTSB_FSM_GLOBAL_CFG_20 |
 | +-----------------------------------------------------+
 | | bit  29:15 R/W  OBJECTS_EN_MODE1_PASS5              |
 | | bit  14:0  R/W  OBJECTS_EN_MODE1_PASS4              |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_OBJECTS_EN_MODE1_PASS5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE1_PASS5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE1_PASS5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE1_PASS5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_OBJECTS_EN_MODE1_PASS4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE1_PASS4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE1_PASS4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE1_PASS4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000790 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_21 |
 | +-----------------------------------------------------+
 | | bit  29:15 R/W  OBJECTS_EN_MODE2_PASS1              |
 | | bit  14:0  R/W  OBJECTS_EN_MODE2_PASS0              |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_OBJECTS_EN_MODE2_PASS1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE2_PASS1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE2_PASS1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE2_PASS1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_OBJECTS_EN_MODE2_PASS0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE2_PASS0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE2_PASS0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE2_PASS0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000794 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_22 |
 | +-----------------------------------------------------+
 | | bit  29:15 R/W  OBJECTS_EN_MODE2_PASS3              |
 | | bit  14:0  R/W  OBJECTS_EN_MODE2_PASS2              |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_OBJECTS_EN_MODE2_PASS3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE2_PASS3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE2_PASS3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE2_PASS3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_OBJECTS_EN_MODE2_PASS2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE2_PASS2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE2_PASS2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE2_PASS2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000798 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_23 |
 | +-----------------------------------------------------+
 | | bit  29:15 R/W  OBJECTS_EN_MODE2_PASS5              |
 | | bit  14:0  R/W  OBJECTS_EN_MODE2_PASS4              |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_OBJECTS_EN_MODE2_PASS5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE2_PASS5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE2_PASS5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE2_PASS5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_OBJECTS_EN_MODE2_PASS4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE2_PASS4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE2_PASS4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE2_PASS4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x0000079c T8_6G_LR_MTSB_FSM_GLOBAL_CFG_24 |
 | +-----------------------------------------------------+
 | | bit  29:15 R/W  OBJECTS_EN_MODE3_PASS1              |
 | | bit  14:0  R/W  OBJECTS_EN_MODE3_PASS0              |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_OBJECTS_EN_MODE3_PASS1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE3_PASS1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE3_PASS1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE3_PASS1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_OBJECTS_EN_MODE3_PASS0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE3_PASS0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE3_PASS0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE3_PASS0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007a0 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_25 |
 | +-----------------------------------------------------+
 | | bit  29:15 R/W  OBJECTS_EN_MODE3_PASS3              |
 | | bit  14:0  R/W  OBJECTS_EN_MODE3_PASS2              |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_OBJECTS_EN_MODE3_PASS3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE3_PASS3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE3_PASS3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE3_PASS3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_OBJECTS_EN_MODE3_PASS2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE3_PASS2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE3_PASS2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE3_PASS2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007a4 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_26 |
 | +-----------------------------------------------------+
 | | bit  29:15 R/W  OBJECTS_EN_MODE3_PASS5              |
 | | bit  14:0  R/W  OBJECTS_EN_MODE3_PASS4              |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_OBJECTS_EN_MODE3_PASS5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE3_PASS5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE3_PASS5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE3_PASS5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_OBJECTS_EN_MODE3_PASS4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OBJECTS_EN_MODE3_PASS4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OBJECTS_EN_MODE3_PASS4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OBJECTS_EN_MODE3_PASS4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007a8 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_27 |
 | +-----------------------------------------------------+
 | | bit  23:19 R/W  OFL_DECAY                           |
 | | bit  18:4  R/W  LMS_DATA_SEL                        |
 | | bit  3:2   R/W  SEL_LMS_CLIP                        |
 | | bit  1     R/W  PRELOAD_ENB_PASS1TO5                |
 | | bit  0     R/W  PRELOAD_ENB_PASS0                   |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_OFL_DECAY_set            ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_OFL_DECAY_get            ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_OFL_DECAY_set      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_OFL_DECAY_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_LMS_DATA_SEL_set         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LMS_DATA_SEL_get         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_LMS_DATA_SEL_set   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_LMS_DATA_SEL_get   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_SEL_LMS_CLIP_set         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_SEL_LMS_CLIP_get         ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_SEL_LMS_CLIP_set   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_SEL_LMS_CLIP_get   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_PRELOAD_ENB_PASS1TO5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_PRELOAD_ENB_PASS1TO5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_PRELOAD_ENB_PASS0_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_PRELOAD_ENB_PASS0_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007ac T8_6G_LR_MTSB_FSM_GLOBAL_CFG_28 |
 | +-----------------------------------------------------+
 | | bit  27:21 R/W  DP_FFE_A_LUT_3                      |
 | | bit  20:14 R/W  DP_FFE_A_LUT_2                      |
 | | bit  13:7  R/W  DP_FFE_A_LUT_1                      |
 | | bit  6:0   R/W  DP_FFE_A_LUT_0                      |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_DP_FFE_A_LUT_3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_A_LUT_3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_A_LUT_3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_A_LUT_3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_A_LUT_2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_A_LUT_2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_A_LUT_2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_A_LUT_2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_A_LUT_1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_A_LUT_1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_A_LUT_1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_A_LUT_1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_A_LUT_0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_A_LUT_0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_A_LUT_0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_A_LUT_0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007b0 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_29 |
 | +-----------------------------------------------------+
 | | bit  27:21 R/W  DP_FFE_A_LUT_7                      |
 | | bit  20:14 R/W  DP_FFE_A_LUT_6                      |
 | | bit  13:7  R/W  DP_FFE_A_LUT_5                      |
 | | bit  6:0   R/W  DP_FFE_A_LUT_4                      |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_DP_FFE_A_LUT_7_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_A_LUT_7_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_A_LUT_7_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_A_LUT_7_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_A_LUT_6_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_A_LUT_6_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_A_LUT_6_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_A_LUT_6_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_A_LUT_5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_A_LUT_5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_A_LUT_5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_A_LUT_5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_A_LUT_4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_A_LUT_4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_A_LUT_4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_A_LUT_4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007b4 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_30 |
 | +-----------------------------------------------------+
 | | bit  27:21 R/W  DP_FFE_B_LUT_3                      |
 | | bit  20:14 R/W  DP_FFE_B_LUT_2                      |
 | | bit  13:7  R/W  DP_FFE_B_LUT_1                      |
 | | bit  6:0   R/W  DP_FFE_B_LUT_0                      |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_DP_FFE_B_LUT_3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_B_LUT_3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_B_LUT_3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_B_LUT_3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_B_LUT_2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_B_LUT_2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_B_LUT_2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_B_LUT_2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_B_LUT_1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_B_LUT_1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_B_LUT_1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_B_LUT_1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_B_LUT_0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_B_LUT_0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_B_LUT_0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_B_LUT_0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007b8 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_31 |
 | +-----------------------------------------------------+
 | | bit  27:21 R/W  DP_FFE_B_LUT_7                      |
 | | bit  20:14 R/W  DP_FFE_B_LUT_6                      |
 | | bit  13:7  R/W  DP_FFE_B_LUT_5                      |
 | | bit  6:0   R/W  DP_FFE_B_LUT_4                      |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_DP_FFE_B_LUT_7_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_B_LUT_7_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_B_LUT_7_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_B_LUT_7_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_B_LUT_6_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_B_LUT_6_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_B_LUT_6_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_B_LUT_6_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_B_LUT_5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_B_LUT_5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_B_LUT_5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_B_LUT_5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_B_LUT_4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_B_LUT_4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_B_LUT_4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_B_LUT_4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007bc T8_6G_LR_MTSB_FSM_GLOBAL_CFG_32 |
 | +-----------------------------------------------------+
 | | bit  27:21 R/W  DP_FFE_C_LUT_3                      |
 | | bit  20:14 R/W  DP_FFE_C_LUT_2                      |
 | | bit  13:7  R/W  DP_FFE_C_LUT_1                      |
 | | bit  6:0   R/W  DP_FFE_C_LUT_0                      |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_DP_FFE_C_LUT_3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_C_LUT_3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_C_LUT_3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_C_LUT_3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_C_LUT_2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_C_LUT_2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_C_LUT_2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_C_LUT_2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_C_LUT_1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_C_LUT_1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_C_LUT_1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_C_LUT_1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_C_LUT_0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_C_LUT_0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_C_LUT_0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_C_LUT_0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007c0 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_33 |
 | +-----------------------------------------------------+
 | | bit  27:21 R/W  DP_FFE_C_LUT_7                      |
 | | bit  20:14 R/W  DP_FFE_C_LUT_6                      |
 | | bit  13:7  R/W  DP_FFE_C_LUT_5                      |
 | | bit  6:0   R/W  DP_FFE_C_LUT_4                      |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_DP_FFE_C_LUT_7_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_C_LUT_7_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_C_LUT_7_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_C_LUT_7_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_C_LUT_6_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_C_LUT_6_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_C_LUT_6_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_C_LUT_6_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_C_LUT_5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_C_LUT_5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_C_LUT_5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_C_LUT_5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_DP_FFE_C_LUT_4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_DP_FFE_C_LUT_4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_DP_FFE_C_LUT_4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_DP_FFE_C_LUT_4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007c4 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_34 |
 | +-----------------------------------------------------+
 | | bit  29:25 R/W  TR_FFE_A_LUT_5                      |
 | | bit  24:20 R/W  TR_FFE_A_LUT_4                      |
 | | bit  19:15 R/W  TR_FFE_A_LUT_3                      |
 | | bit  14:10 R/W  TR_FFE_A_LUT_2                      |
 | | bit  9:5   R/W  TR_FFE_A_LUT_1                      |
 | | bit  4:0   R/W  TR_FFE_A_LUT_0                      |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_TR_FFE_A_LUT_5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_A_LUT_5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_A_LUT_5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_A_LUT_5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_TR_FFE_A_LUT_4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_A_LUT_4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_A_LUT_4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_A_LUT_4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_TR_FFE_A_LUT_3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_A_LUT_3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_A_LUT_3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_A_LUT_3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_TR_FFE_A_LUT_2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_A_LUT_2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_A_LUT_2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_A_LUT_2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_TR_FFE_A_LUT_1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_A_LUT_1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_A_LUT_1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_A_LUT_1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_TR_FFE_A_LUT_0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_A_LUT_0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_A_LUT_0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_A_LUT_0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007c8 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_35 |
 | +-----------------------------------------------------+
 | | bit  9:5 R/W  TR_FFE_A_LUT_7                        |
 | | bit  4:0 R/W  TR_FFE_A_LUT_6                        |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_TR_FFE_A_LUT_7_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_A_LUT_7_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_A_LUT_7_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_A_LUT_7_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_TR_FFE_A_LUT_6_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_A_LUT_6_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_A_LUT_6_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_A_LUT_6_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007cc T8_6G_LR_MTSB_FSM_GLOBAL_CFG_36 |
 | +-----------------------------------------------------+
 | | bit  29:25 R/W  TR_FFE_B_LUT_5                      |
 | | bit  24:20 R/W  TR_FFE_B_LUT_4                      |
 | | bit  19:15 R/W  TR_FFE_B_LUT_3                      |
 | | bit  14:10 R/W  TR_FFE_B_LUT_2                      |
 | | bit  9:5   R/W  TR_FFE_B_LUT_1                      |
 | | bit  4:0   R/W  TR_FFE_B_LUT_0                      |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_TR_FFE_B_LUT_5_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_B_LUT_5_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_B_LUT_5_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_B_LUT_5_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_TR_FFE_B_LUT_4_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_B_LUT_4_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_B_LUT_4_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_B_LUT_4_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_TR_FFE_B_LUT_3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_B_LUT_3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_B_LUT_3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_B_LUT_3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_TR_FFE_B_LUT_2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_B_LUT_2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_B_LUT_2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_B_LUT_2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_TR_FFE_B_LUT_1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_B_LUT_1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_B_LUT_1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_B_LUT_1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_TR_FFE_B_LUT_0_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_B_LUT_0_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_B_LUT_0_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_B_LUT_0_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007d0 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_37 |
 | +-----------------------------------------------------+
 | | bit  9:5 R/W  TR_FFE_B_LUT_7                        |
 | | bit  4:0 R/W  TR_FFE_B_LUT_6                        |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_TR_FFE_B_LUT_7_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_B_LUT_7_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_B_LUT_7_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_B_LUT_7_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_TR_FFE_B_LUT_6_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_TR_FFE_B_LUT_6_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_TR_FFE_B_LUT_6_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_TR_FFE_B_LUT_6_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007d4 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_38 |
 | +-----------------------------------------------------+
 | | bit  17    R/W  LF_PROP_LEFT_SHIFT_MODE3            |
 | | bit  16    R/W  LF_PROP_LEFT_SHIFT_MODE2            |
 | | bit  15    R/W  LF_PROP_LEFT_SHIFT_MODE1            |
 | | bit  14:10 R/W  LF_PROP_CTRL_GAIN_MODE3             |
 | | bit  9:5   R/W  LF_PROP_CTRL_GAIN_MODE2             |
 | | bit  4:0   R/W  LF_PROP_CTRL_GAIN_MODE1             |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_LF_PROP_LEFT_SHIFT_MODE3_set      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_PROP_LEFT_SHIFT_MODE3_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_LF_PROP_LEFT_SHIFT_MODE2_set      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_PROP_LEFT_SHIFT_MODE2_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_LF_PROP_LEFT_SHIFT_MODE1_set      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_PROP_LEFT_SHIFT_MODE1_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_LF_PROP_CTRL_GAIN_MODE3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_PROP_CTRL_GAIN_MODE3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_LF_PROP_CTRL_GAIN_MODE3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_LF_PROP_CTRL_GAIN_MODE3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_LF_PROP_CTRL_GAIN_MODE2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_PROP_CTRL_GAIN_MODE2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_LF_PROP_CTRL_GAIN_MODE2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_LF_PROP_CTRL_GAIN_MODE2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_LF_PROP_CTRL_GAIN_MODE1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_PROP_CTRL_GAIN_MODE1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_LF_PROP_CTRL_GAIN_MODE1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_LF_PROP_CTRL_GAIN_MODE1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007d8 T8_6G_LR_MTSB_FSM_GLOBAL_CFG_39 |
 | +-----------------------------------------------------+
 | | bit  17    R/W  LF_INTEG_LEFT_SHIFT_MODE3           |
 | | bit  16    R/W  LF_INTEG_LEFT_SHIFT_MODE2           |
 | | bit  15    R/W  LF_INTEG_LEFT_SHIFT_MODE1           |
 | | bit  14:10 R/W  LF_INTEG_CTRL_GAIN_MODE3            |
 | | bit  9:5   R/W  LF_INTEG_CTRL_GAIN_MODE2            |
 | | bit  4:0   R/W  LF_INTEG_CTRL_GAIN_MODE1            |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_LF_INTEG_LEFT_SHIFT_MODE3_set      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_INTEG_LEFT_SHIFT_MODE3_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_LF_INTEG_LEFT_SHIFT_MODE2_set      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_INTEG_LEFT_SHIFT_MODE2_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_LF_INTEG_LEFT_SHIFT_MODE1_set      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_INTEG_LEFT_SHIFT_MODE1_get      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_LF_INTEG_CTRL_GAIN_MODE3_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_INTEG_CTRL_GAIN_MODE3_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_LF_INTEG_CTRL_GAIN_MODE3_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_LF_INTEG_CTRL_GAIN_MODE3_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_LF_INTEG_CTRL_GAIN_MODE2_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_INTEG_CTRL_GAIN_MODE2_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_LF_INTEG_CTRL_GAIN_MODE2_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_LF_INTEG_CTRL_GAIN_MODE2_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_LF_INTEG_CTRL_GAIN_MODE1_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_LF_INTEG_CTRL_GAIN_MODE1_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_LF_INTEG_CTRL_GAIN_MODE1_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_LF_INTEG_CTRL_GAIN_MODE1_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x000007dc T8_6G_LR_MTSB_FSM_GLOBAL_CFG_40 |
 | +-----------------------------------------------------+
 | | bit  3   R/W  RX_ADAPT_DONE_OVR                     |
 | | bit  2:1 R/W  ADAPT_MODE_OVR                        |
 | | bit  0   R/W  RX_ADAPT_START_OVR                    |
 | +-----------------------------------------------------+
 | 
 | void   t8_ctrl_field_RX_ADAPT_DONE_OVR_set    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RX_ADAPT_DONE_OVR_get    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_ADAPT_MODE_OVR_set       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_ADAPT_MODE_OVR_get       ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_ctrl_field_range_ADAPT_MODE_OVR_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_ctrl_field_range_ADAPT_MODE_OVR_get ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_ctrl_field_RX_ADAPT_START_OVR_set   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_ctrl_field_RX_ADAPT_START_OVR_get   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _T8_CTRL_IO_H
#define _T8_CTRL_IO_H

#include "t8.h"
#include "t8_ctrl_regs.h"
#include "t8_ctrl_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 t8_ctrl_buffer_init               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void                 t8_ctrl_buffer_flush              ( t8_ctrl_buffer_t *b_ptr )
 | UINT32               t8_ctrl_reg_read                  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 t8_ctrl_reg_write                 ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 t8_ctrl_field_set                 ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 t8_ctrl_action_on_write_field_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 t8_ctrl_burst_read                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 t8_ctrl_burst_write               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE t8_ctrl_poll                      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_1_write                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_1_field_set                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_1_read                     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_2_write                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_2_field_set                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_2_read                     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_3_write                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_3_field_set                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_3_read                     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_4_write                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_4_field_set                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_4_read                     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_5_write                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_5_field_set                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_5_read                     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_6_write                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_6_field_set                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_6_read                     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_7_write                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_7_field_set                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_7_read                     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_8_write                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_8_field_set                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_8_read                     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_9_write                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_9_field_set                ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_9_read                     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_10_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_10_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_10_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_11_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_11_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_11_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_12_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_12_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_12_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_13_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_13_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_13_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_14_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_14_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_14_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_15_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_15_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_15_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_16_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_16_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_16_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_17_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_17_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_17_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_18_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_18_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_18_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_19_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_19_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_19_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_20_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_20_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_20_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_21_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_21_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_21_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_22_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_22_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_22_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_23_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_23_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_23_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_24_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_24_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_24_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_25_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_25_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_25_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_26_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_26_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_26_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_27_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_27_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_27_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_28_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_28_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_28_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_29_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_29_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_29_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_30_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_30_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_30_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_31_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_31_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_31_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_32_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_32_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_32_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_33_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_33_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_33_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_34_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_34_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_34_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_35_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_35_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_35_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_36_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_36_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_36_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_37_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_37_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_37_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_38_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_38_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_38_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_39_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_39_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_39_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_40_write                   ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_40_field_set               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_GLOBAL_CFG_40_read                    ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_LANE_N_CFG_1_array_write              ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_LANE_N_CFG_1_array_field_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_LANE_N_CFG_1_array_read               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_LANE_N_CFG_2_array_write              ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_LANE_N_CFG_2_array_field_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_LANE_N_CFG_2_array_read               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_LANE_N_CFG_3_array_write              ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_LANE_N_CFG_3_array_field_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_LANE_N_CFG_3_array_read               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | 
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_LANE_N_OBSERVABILITY__array_write     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 t8_ctrl_reg_T8_6G_LR_MTSB_FSM_LANE_N_OBSERVABILITY__array_field_set ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_T8_6G_LR_MTSB_FSM_LANE_N_OBSERVABILITY__array_read      ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | 
 | void                 t8_ctrl_reg_STAT_INT_EVENT_array_write                              ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 t8_ctrl_reg_STAT_INT_EVENT_array_action_on_write_field_set          ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_STAT_INT_EVENT_array_read                               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_reg_STAT_INT_EVENT_array_poll                               ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 t8_ctrl_reg_STAT_INT_EN_array_write                                 ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 t8_ctrl_reg_STAT_INT_EN_array_field_set                             ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_ctrl_reg_STAT_INT_EN_array_read                                  ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               t8_ctrl_reg_STAT_INT_array_read                                     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE t8_ctrl_reg_STAT_INT_array_poll                                     ( t8_ctrl_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _T8_CTRL_IO_H */
