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
 *     and register accessor functions for the s16_mtsb_ctrl block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing s16_mtsb_ctrl io functions is:
 | {
 |     s16_mtsb_ctrl_buffer_t b_ptr[1];
 |     s16_mtsb_ctrl_buffer_init( b_ptr, h_ptr, A );
 |     value = s16_mtsb_ctrl_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     s16_mtsb_ctrl_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     s16_mtsb_ctrl_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = s16_mtsb_ctrl_field_<FIELD>_get( NULL, h_ptr, A );
 | s16_mtsb_ctrl_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------------------------+
 | | Register 0x00000400 DIGI120_MTSB_CTRL_LANE_N_CFG_1 |
 | +----------------------------------------------------+
 | | bit  6 R/W  CTRL_INITCAL                           |
 | | bit  5 R/W  CONTINUOUS_ADAPT_SNW                   |
 | | bit  4 R/W  ADAPT_INIT_FORCE                       |
 | | bit  2 R/W  RESET_CLBRT_FSM                        |
 | | bit  1 R/W  RESET_ADPT_FSM                         |
 | | bit  0 R/W  RESET_SYNCR                            |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_CTRL_INITCAL_set         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_CTRL_INITCAL_get         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_CONTINUOUS_ADAPT_SNW_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_CONTINUOUS_ADAPT_SNW_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADAPT_INIT_FORCE_set     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_INIT_FORCE_get     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_RESET_CLBRT_FSM_set      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RESET_CLBRT_FSM_get      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_RESET_ADPT_FSM_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RESET_ADPT_FSM_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_RESET_SYNCR_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RESET_SYNCR_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------------------+
 | | Register 0x00000404 DIGI120_MTSB_CTRL_LANE_N_PGA_CFG_1 |
 | +--------------------------------------------------------+
 | | bit  27:24 R/W  RX_CM_STG2                             |
 | | bit  23:19 R/W  RX_CM_STG1                             |
 | | bit  18:14 R/W  RX_CM_TERM                             |
 | | bit  8:4   R/W  PGA_GAIN_PRELOAD                       |
 | | bit  3     R/W  RX_PEAK_ENB                            |
 | | bit  2:0   R/W  RX_PEAK                                |
 | +--------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_RX_CM_STG2_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RX_CM_STG2_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_RX_CM_STG2_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_RX_CM_STG2_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_RX_CM_STG1_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RX_CM_STG1_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_RX_CM_STG1_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_RX_CM_STG1_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_RX_CM_TERM_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RX_CM_TERM_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_RX_CM_TERM_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_RX_CM_TERM_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PGA_GAIN_PRELOAD_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PGA_GAIN_PRELOAD_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PGA_GAIN_PRELOAD_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PGA_GAIN_PRELOAD_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_RX_PEAK_ENB_set            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RX_PEAK_ENB_get            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_RX_PEAK_set                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RX_PEAK_get                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_RX_PEAK_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_RX_PEAK_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register 0x00000408 DIGI120_MTSB_CTRL_LANE_N_PISO_CFG_1 |
 | +---------------------------------------------------------+
 | | bit  31:26 R/W  PRECURSOR                               |
 | | bit  23:18 R/W  POSTCURSOR                              |
 | | bit  14:8  R/W  AMPLITUDE                               |
 | | bit  4     R/W  T_PISO_PRE2_SEL                         |
 | | bit  3     R/W  T_PISO_EDGE_DELAY_SEL                   |
 | | bit  2     R/W  T_PISO_PRE2_MODE1                       |
 | | bit  1     R/W  T_PISO_PRE2_MODE0                       |
 | | bit  0     R/W  IMPEDANCE                               |
 | +---------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PRECURSOR_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PRECURSOR_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PRECURSOR_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PRECURSOR_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_POSTCURSOR_set            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_POSTCURSOR_get            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_POSTCURSOR_set      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_POSTCURSOR_get      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_AMPLITUDE_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_AMPLITUDE_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_AMPLITUDE_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_AMPLITUDE_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_T_PISO_PRE2_SEL_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_T_PISO_PRE2_SEL_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_T_PISO_EDGE_DELAY_SEL_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_T_PISO_EDGE_DELAY_SEL_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_T_PISO_PRE2_MODE1_set     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_T_PISO_PRE2_MODE1_get     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_T_PISO_PRE2_MODE0_set     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_T_PISO_PRE2_MODE0_get     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_IMPEDANCE_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_IMPEDANCE_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------------------------+
 | | Register 0x0000040c DIGI120_MTSB_CTRL_LANE_N_ADAPT_CFG_1 |
 | +----------------------------------------------------------+
 | | bit  5:0 R/W  DP_FFE_M_PRELOAD                           |
 | +----------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_DP_FFE_M_PRELOAD_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_DP_FFE_M_PRELOAD_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_DP_FFE_M_PRELOAD_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_DP_FFE_M_PRELOAD_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x0000041c DIGI120_MTSB_CTRL_LANE_N_CFG_REG |
 | +------------------------------------------------------+
 | | bit  30    R    ADC_CALIB_PASS_TIMING_OBS            |
 | | bit  29    R    ADC_CALIB_PASS_DATA_OBS              |
 | | bit  28    R/W  PISO_CLK_EN                          |
 | | bit  27    R/W  FIFO_HW_RESET                        |
 | | bit  26    R/W  MDSP_CLK_EN                          |
 | | bit  24    R    TX_DCD_CALIB_REQUEST_FSM_OBS         |
 | | bit  23    R    ADC_CALIB_REQUEST_FSM_OBS            |
 | | bit  22    R    TX_DCD_CALIB_DONE_OBS                |
 | | bit  21    R    TX_DCD_CALIB_DONE_DIV3_OBS           |
 | | bit  20    R    ADC_CALIB_DONE_OBS                   |
 | | bit  14:13 R    OCC_STATE_OBS                        |
 | | bit  12:9  R    CLBRT_STATE_OBS                      |
 | | bit  8     R/W  CAPTURE_REQ_ALL_STATES               |
 | | bit  7     R/W  CLR_OCDAC                            |
 | | bit  6     R/W  CLR_OCDAC_DIV3                       |
 | | bit  5     R/W  ADAPT_CLK_ENB_OVR                    |
 | | bit  4     R/W  ADAPT_CLK_ENB_OVR_EN                 |
 | | bit  3     R/W  ADAPT_ENB_OVR                        |
 | | bit  2     R/W  ADAPT_ENB_OVR_EN                     |
 | | bit  1     R/W  ADAPT_STATUS_0_OVR                   |
 | | bit  0     R/W  ADAPT_STATUS_0_OVR_EN                |
 | +------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PISO_CLK_EN_set                  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PISO_CLK_EN_get                  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_FIFO_HW_RESET_set                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_FIFO_HW_RESET_get                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_MDSP_CLK_EN_set                  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MDSP_CLK_EN_get                  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_CLR_OCDAC_set                    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_CLR_OCDAC_get                    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_CLR_OCDAC_DIV3_set               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_CLR_OCDAC_DIV3_get               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADAPT_CLK_ENB_OVR_set            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_CLK_ENB_OVR_get            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADAPT_CLK_ENB_OVR_EN_set         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_CLK_ENB_OVR_EN_get         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADAPT_ENB_OVR_set                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_ENB_OVR_get                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADAPT_ENB_OVR_EN_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_ENB_OVR_EN_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADAPT_STATUS_0_OVR_set           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_STATUS_0_OVR_get           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADAPT_STATUS_0_OVR_EN_set        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_STATUS_0_OVR_EN_get        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_CAPTURE_REQ_ALL_STATES_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_CAPTURE_REQ_ALL_STATES_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_ADC_CALIB_PASS_TIMING_OBS_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_ADC_CALIB_PASS_DATA_OBS_get      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_TX_DCD_CALIB_REQUEST_FSM_OBS_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_ADC_CALIB_REQUEST_FSM_OBS_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_TX_DCD_CALIB_DONE_OBS_get        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_TX_DCD_CALIB_DONE_DIV3_OBS_get   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_ADC_CALIB_DONE_OBS_get           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_OCC_STATE_OBS_get                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_range_OCC_STATE_OBS_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 s16_mtsb_ctrl_field_CLBRT_STATE_OBS_get              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_range_CLBRT_STATE_OBS_get        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register 0x00000420 DIGI120_MTSB_CTRL_LANE_N_OBS_1_REG |
 | +--------------------------------------------------------+
 | | bit  11 R/W  ALL_CALIB_REQUEST_FSM_OVR_EN              |
 | | bit  10 R/W  TX_EMI_ROP_RON_CAL_REQUEST_OVR            |
 | | bit  9  R/W  PHGEN_RTUNE_CAL_REQUEST_OVR               |
 | | bit  6  R/W  ADC_CALIB_REQUEST_OVR                     |
 | | bit  4  R/W  CALIB_REQUEST_OVR_EN                      |
 | +--------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_ALL_CALIB_REQUEST_FSM_OVR_EN_set   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ALL_CALIB_REQUEST_FSM_OVR_EN_get   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_TX_EMI_ROP_RON_CAL_REQUEST_OVR_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_TX_EMI_ROP_RON_CAL_REQUEST_OVR_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_RTUNE_CAL_REQUEST_OVR_set    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_RTUNE_CAL_REQUEST_OVR_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADC_CALIB_REQUEST_OVR_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADC_CALIB_REQUEST_OVR_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_CALIB_REQUEST_OVR_EN_set           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_CALIB_REQUEST_OVR_EN_get           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------------------+
 | | Register 0x00000424 DIGI120_MTSB_CTRL_LANE_N_OBS_2_REG |
 | +--------------------------------------------------------+
 | | bit  11 R  PHGEN_RTUNE_CAL_DONE_OBS                    |
 | | bit  10 R  PHGEN_CTUNE_CAL_DONE_OBS                    |
 | | bit  9  R  PHGEN_FTUNE_CAL_DONE_OBS                    |
 | | bit  8  R  TX_EMI_ROP_RON_CAL_REQUEST_FSM_OBS          |
 | | bit  7  R  PHGEN_RTUNE_CAL_REQUEST_FSM_OBS             |
 | | bit  6  R  PHGEN_CTUNE_CAL_REQUEST_FSM_OBS             |
 | | bit  5  R  PHGEN_FTUNE_CAL_REQUEST_FSM_OBS             |
 | | bit  1  R  TX_EMI_ROP_RON_CAL_DONE_OBS                 |
 | | bit  0  R  PHGEN_HOLD_OBS                              |
 | +--------------------------------------------------------+
 | 
 | UINT32 s16_mtsb_ctrl_field_PHGEN_RTUNE_CAL_DONE_OBS_get           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_CTUNE_CAL_DONE_OBS_get           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FTUNE_CAL_DONE_OBS_get           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_TX_EMI_ROP_RON_CAL_REQUEST_FSM_OBS_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_RTUNE_CAL_REQUEST_FSM_OBS_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_CTUNE_CAL_REQUEST_FSM_OBS_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FTUNE_CAL_REQUEST_FSM_OBS_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_TX_EMI_ROP_RON_CAL_DONE_OBS_get        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_HOLD_OBS_get                     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------------------------+
 | | Register 0x00000428 DIGI120_MTSB_CTRL_LANE_N_STAT_INT_EN |
 | +----------------------------------------------------------+
 | | bit  2 R/W  ADPT_DONE_E                                  |
 | | bit  1 R/W  ALL_CALIB_DONE_E                             |
 | | bit  0 R/W  ADAPT_STATUS_0_E                             |
 | +----------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_ADPT_DONE_E_set      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADPT_DONE_E_get      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ALL_CALIB_DONE_E_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ALL_CALIB_DONE_E_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADAPT_STATUS_0_E_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_STATUS_0_E_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x0000042c DIGI120_MTSB_CTRL_LANE_N_STAT_INT_EVENT |
 | +-------------------------------------------------------------+
 | | bit  2 R/W  ADPT_DONE_I                                     |
 | | bit  1 R/W  ALL_CALIB_DONE_I                                |
 | | bit  0 R/W  ADAPT_STATUS_0_I                                |
 | +-------------------------------------------------------------+
 | 
 | void                 s16_mtsb_ctrl_field_ADPT_DONE_I_set_to_clear      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_mtsb_ctrl_field_ADPT_DONE_I_get               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_ADPT_DONE_I_poll              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_mtsb_ctrl_field_ALL_CALIB_DONE_I_set_to_clear ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_mtsb_ctrl_field_ALL_CALIB_DONE_I_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_ALL_CALIB_DONE_I_poll         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_mtsb_ctrl_field_ADAPT_STATUS_0_I_set_to_clear ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_mtsb_ctrl_field_ADAPT_STATUS_0_I_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_ADAPT_STATUS_0_I_poll         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------------------+
 | | Register 0x00000430 DIGI120_MTSB_CTRL_LANE_N_STAT_INT |
 | +-------------------------------------------------------+
 | | bit  2 R  ADPT_DONE_V                                 |
 | | bit  1 R  ALL_CALIB_DONE_V                            |
 | | bit  0 R  ADAPT_STATUS_0_V                            |
 | +-------------------------------------------------------+
 | 
 | UINT32               s16_mtsb_ctrl_field_ADPT_DONE_V_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_ADPT_DONE_V_poll      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_mtsb_ctrl_field_ALL_CALIB_DONE_V_get  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_ALL_CALIB_DONE_V_poll ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_mtsb_ctrl_field_ADAPT_STATUS_0_V_get  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_ADAPT_STATUS_0_V_poll ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------------------+
 | | Register 0x00000440 DIGI120_MTSB_CTRL_GLOBAL_PISO_CFG_1 |
 | +---------------------------------------------------------+
 | | bit  17   R/W  T_PISO_ENB                               |
 | | bit  14   R/W  T_TX_OENB                                |
 | | bit  12:8 R/W  T_PISO_CTRL                              |
 | +---------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_T_PISO_ENB_set        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_T_PISO_ENB_get        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_T_TX_OENB_set         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_T_TX_OENB_get         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_T_PISO_CTRL_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_T_PISO_CTRL_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_T_PISO_CTRL_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_T_PISO_CTRL_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000444 MTSB_CTRL_GLOBAL_ADC_CFG_1 |
 | +------------------------------------------------+
 | | bit  20    R/W  ADC_CAL_DAC_OVR_EN             |
 | | bit  19    R/W  ADC_CAL_DAC_ENB_DEFAULT        |
 | | bit  18    R/W  ADC_CAL_DAC_ENB_CALIB          |
 | | bit  17    R/W  ADC_CAL_DAC_ENB_OVR            |
 | | bit  16    R/W  OC_OFFSET_ENB                  |
 | | bit  15    R/W  OC_ENB                         |
 | | bit  14    R/W  PHGEN_ENB                      |
 | | bit  13    R/W  PHGEN_DLYCOR_ENB               |
 | | bit  12    R/W  ADC_DAC_ENB                    |
 | | bit  11:10 R/W  ADC_RATE_SEL                   |
 | | bit  9     R/W  ADC_CLK_1200M_ENB              |
 | | bit  7:4   R/W  ADC_ENB                        |
 | +------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_ADC_CAL_DAC_OVR_EN_set      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADC_CAL_DAC_OVR_EN_get      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADC_CAL_DAC_ENB_DEFAULT_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADC_CAL_DAC_ENB_DEFAULT_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADC_CAL_DAC_ENB_CALIB_set   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADC_CAL_DAC_ENB_CALIB_get   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADC_CAL_DAC_ENB_OVR_set     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADC_CAL_DAC_ENB_OVR_get     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_OC_OFFSET_ENB_set           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_OC_OFFSET_ENB_get           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_OC_ENB_set                  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_OC_ENB_get                  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_ENB_set               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_ENB_get               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_DLYCOR_ENB_set        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_DLYCOR_ENB_get        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADC_DAC_ENB_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADC_DAC_ENB_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADC_RATE_SEL_set            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADC_RATE_SEL_get            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_ADC_RATE_SEL_set      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_ADC_RATE_SEL_get      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_ADC_CLK_1200M_ENB_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADC_CLK_1200M_ENB_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADC_ENB_set                 ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADC_ENB_get                 ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_ADC_ENB_set           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_ADC_ENB_get           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register 0x00000448 DIGI120_MTSB_CTRL_GLOBAL_MTSB_CFG_1 |
 | +---------------------------------------------------------+
 | | bit  9:7 R/W  TCB_RESET_DURATION                        |
 | | bit  6   R/W  RX_CM_TERM_SEL                            |
 | | bit  5   R/W  RX_CMCOR_ENB                              |
 | | bit  4   R/W  T_EMI_DAC_ENB                             |
 | | bit  3   R/W  T_OC_SENSE_ENB                            |
 | | bit  2   R/W  T_OFFSET_ENB                              |
 | +---------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_TCB_RESET_DURATION_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_TCB_RESET_DURATION_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_TCB_RESET_DURATION_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_TCB_RESET_DURATION_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_RX_CM_TERM_SEL_set           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RX_CM_TERM_SEL_get           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_RX_CMCOR_ENB_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RX_CMCOR_ENB_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_T_EMI_DAC_ENB_set            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_T_EMI_DAC_ENB_get            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_T_OC_SENSE_ENB_set           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_T_OC_SENSE_ENB_get           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_T_OFFSET_ENB_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_T_OFFSET_ENB_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------------------+
 | | Register 0x0000044c DIGI120_MTSB_CTRL_GLOBAL_TCB_CFG_1 |
 | +--------------------------------------------------------+
 | | bit  30:29 R/W  LF_INTEG_OUT_MODE                      |
 | | bit  28:27 R/W  BLOCK_SIZE                             |
 | | bit  26:15 R/W  LF_INTEG_OUT_OVR                       |
 | | bit  14:12 R/W  LF_INTEG_LEFT_SHIFT                    |
 | | bit  11:7  R/W  LF_INTEG_CTRL_GAIN                     |
 | | bit  6:5   R/W  LF_PROP_LEFT_SHIFT                     |
 | | bit  4:0   R/W  LF_PROP_CTRL_GAIN                      |
 | +--------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_LF_INTEG_OUT_MODE_set         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_LF_INTEG_OUT_MODE_get         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_LF_INTEG_OUT_MODE_set   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_LF_INTEG_OUT_MODE_get   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_BLOCK_SIZE_set                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_BLOCK_SIZE_get                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_BLOCK_SIZE_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_BLOCK_SIZE_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_LF_INTEG_OUT_OVR_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_LF_INTEG_OUT_OVR_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_LF_INTEG_OUT_OVR_set    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_LF_INTEG_OUT_OVR_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_LF_INTEG_LEFT_SHIFT_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_LF_INTEG_LEFT_SHIFT_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_LF_INTEG_LEFT_SHIFT_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_LF_INTEG_LEFT_SHIFT_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_LF_INTEG_CTRL_GAIN_set        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_LF_INTEG_CTRL_GAIN_get        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_LF_INTEG_CTRL_GAIN_set  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_LF_INTEG_CTRL_GAIN_get  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_LF_PROP_LEFT_SHIFT_set        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_LF_PROP_LEFT_SHIFT_get        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_LF_PROP_LEFT_SHIFT_set  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_LF_PROP_LEFT_SHIFT_get  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_LF_PROP_CTRL_GAIN_set         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_LF_PROP_CTRL_GAIN_get         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_LF_PROP_CTRL_GAIN_set   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_LF_PROP_CTRL_GAIN_get   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register 0x00000450 DIGI120_MTSB_CTRL_GLOBAL_TCB_CFG_2 |
 | +--------------------------------------------------------+
 | | bit  23:12 R/W  LF_CLAMP_MIN                           |
 | | bit  11:0  R/W  LF_CLAMP_MAX                           |
 | +--------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_LF_CLAMP_MIN_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_LF_CLAMP_MIN_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_LF_CLAMP_MIN_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_LF_CLAMP_MIN_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_LF_CLAMP_MAX_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_LF_CLAMP_MAX_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_LF_CLAMP_MAX_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_LF_CLAMP_MAX_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register 0x00000454 DIGI120_MTSB_CTRL_GLOBAL_TCB_STS_1 |
 | +--------------------------------------------------------+
 | | bit  24:21 R  ADPT_STATE_OBS                           |
 | +--------------------------------------------------------+
 | 
 | UINT32 s16_mtsb_ctrl_field_ADPT_STATE_OBS_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_range_ADPT_STATE_OBS_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000458 DIGI120_MTSB_CTRL_GLOBAL_CFG_1 |
 | +----------------------------------------------------+
 | | bit  31    R/W  PGA_ENB_CALIB                      |
 | | bit  30    R/W  PGA_ENB_DEFAULT                    |
 | | bit  29    R/W  PGA_ENB_OVR                        |
 | | bit  28    R/W  PGA_ENB_OVR_EN                     |
 | | bit  27    R/W  PHGEN_REGV_CAL_EN                  |
 | | bit  26    R/W  PHGEN_CT_CAL_EN                    |
 | | bit  25    R/W  PHGEN_FT_CAL_EN                    |
 | | bit  24    R/W  TX_EMI_ROP_RON_CAL_EN              |
 | | bit  22    R/W  TX_DCD_CALIB_EN                    |
 | | bit  21    R/W  TX_DCD_CALIB_DIV3_EN               |
 | | bit  20    R/W  ADC_CALIB_EN                       |
 | | bit  19:16 R/W  ADAPT_MISSION_CNTR                 |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PGA_ENB_CALIB_set            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PGA_ENB_CALIB_get            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PGA_ENB_DEFAULT_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PGA_ENB_DEFAULT_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PGA_ENB_OVR_set              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PGA_ENB_OVR_get              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PGA_ENB_OVR_EN_set           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PGA_ENB_OVR_EN_get           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_REGV_CAL_EN_set        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_REGV_CAL_EN_get        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_CT_CAL_EN_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_CT_CAL_EN_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_FT_CAL_EN_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_CAL_EN_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_TX_EMI_ROP_RON_CAL_EN_set    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_TX_EMI_ROP_RON_CAL_EN_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_TX_DCD_CALIB_EN_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_TX_DCD_CALIB_EN_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_TX_DCD_CALIB_DIV3_EN_set     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_TX_DCD_CALIB_DIV3_EN_get     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADC_CALIB_EN_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADC_CALIB_EN_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADAPT_MISSION_CNTR_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_MISSION_CNTR_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_ADAPT_MISSION_CNTR_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_ADAPT_MISSION_CNTR_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000045c DIGI120_MTSB_CTRL_GLOBAL_CFG_2 |
 | +----------------------------------------------------+
 | | bit  23:21 R/W  MU_STEP_GAIN_PASS7                 |
 | | bit  20:18 R/W  MU_STEP_GAIN_PASS6                 |
 | | bit  17:15 R/W  MU_STEP_GAIN_PASS5                 |
 | | bit  14:12 R/W  MU_STEP_GAIN_PASS4                 |
 | | bit  11:9  R/W  MU_STEP_GAIN_PASS3                 |
 | | bit  8:6   R/W  MU_STEP_GAIN_PASS2                 |
 | | bit  5:3   R/W  MU_STEP_GAIN_PASS1                 |
 | | bit  2:0   R/W  MU_STEP_GAIN_PASS0                 |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS7_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS7_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS7_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS7_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS6_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS6_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS6_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS6_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS5_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS5_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS5_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS5_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS4_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS4_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS4_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS4_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS3_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS3_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS3_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS3_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS2_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS2_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS2_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS2_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS1_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS1_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS1_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS1_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS0_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_GAIN_PASS0_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS0_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_GAIN_PASS0_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000460 DIGI120_MTSB_CTRL_GLOBAL_CFG_3 |
 | +----------------------------------------------------+
 | | bit  23:21 R/W  MU_STEP_OFFSET_PASS7               |
 | | bit  20:18 R/W  MU_STEP_OFFSET_PASS6               |
 | | bit  17:15 R/W  MU_STEP_OFFSET_PASS5               |
 | | bit  14:12 R/W  MU_STEP_OFFSET_PASS4               |
 | | bit  11:9  R/W  MU_STEP_OFFSET_PASS3               |
 | | bit  8:6   R/W  MU_STEP_OFFSET_PASS2               |
 | | bit  5:3   R/W  MU_STEP_OFFSET_PASS1               |
 | | bit  2:0   R/W  MU_STEP_OFFSET_PASS0               |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS7_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS7_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS7_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS7_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS6_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS6_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS6_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS6_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS5_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS5_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS5_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS5_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS4_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS4_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS4_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS4_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS3_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS3_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS3_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS3_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS2_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS2_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS2_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS2_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS1_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS1_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS1_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS1_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS0_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_OFFSET_PASS0_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS0_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_OFFSET_PASS0_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000464 DIGI120_MTSB_CTRL_GLOBAL_CFG_4 |
 | +----------------------------------------------------+
 | | bit  23:21 R/W  MU_STEP_COEF_PASS7                 |
 | | bit  20:18 R/W  MU_STEP_COEF_PASS6                 |
 | | bit  17:15 R/W  MU_STEP_COEF_PASS5                 |
 | | bit  14:12 R/W  MU_STEP_COEF_PASS4                 |
 | | bit  11:9  R/W  MU_STEP_COEF_PASS3                 |
 | | bit  8:6   R/W  MU_STEP_COEF_PASS2                 |
 | | bit  5:3   R/W  MU_STEP_COEF_PASS1                 |
 | | bit  2:0   R/W  MU_STEP_COEF_PASS0                 |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_MU_STEP_COEF_PASS7_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_COEF_PASS7_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS7_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS7_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_COEF_PASS6_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_COEF_PASS6_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS6_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS6_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_COEF_PASS5_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_COEF_PASS5_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS5_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS5_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_COEF_PASS4_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_COEF_PASS4_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS4_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS4_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_COEF_PASS3_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_COEF_PASS3_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS3_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS3_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_COEF_PASS2_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_COEF_PASS2_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS2_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS2_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_COEF_PASS1_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_COEF_PASS1_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS1_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS1_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_COEF_PASS0_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_COEF_PASS0_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS0_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_COEF_PASS0_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000468 DIGI120_MTSB_CTRL_GLOBAL_CFG_5 |
 | +----------------------------------------------------+
 | | bit  23:21 R/W  MU_STEP_ADC_MAP_PASS7              |
 | | bit  20:18 R/W  MU_STEP_ADC_MAP_PASS6              |
 | | bit  17:15 R/W  MU_STEP_ADC_MAP_PASS5              |
 | | bit  14:12 R/W  MU_STEP_ADC_MAP_PASS4              |
 | | bit  11:9  R/W  MU_STEP_ADC_MAP_PASS3              |
 | | bit  8:6   R/W  MU_STEP_ADC_MAP_PASS2              |
 | | bit  5:3   R/W  MU_STEP_ADC_MAP_PASS1              |
 | | bit  2:0   R/W  MU_STEP_ADC_MAP_PASS0              |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS7_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS7_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS7_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS7_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS6_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS6_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS6_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS6_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS5_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS5_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS5_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS5_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS4_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS4_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS4_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS4_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS3_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS3_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS3_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS3_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS2_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS2_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS2_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS2_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS1_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS1_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS1_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS1_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS0_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_MU_STEP_ADC_MAP_PASS0_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS0_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_MU_STEP_ADC_MAP_PASS0_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000046c DIGI120_MTSB_CTRL_GLOBAL_CFG_6 |
 | +----------------------------------------------------+
 | | bit  31:28 R/W  ADAPT_LOOPS_PASS7                  |
 | | bit  27:24 R/W  ADAPT_LOOPS_PASS6                  |
 | | bit  23:20 R/W  ADAPT_LOOPS_PASS5                  |
 | | bit  19:16 R/W  ADAPT_LOOPS_PASS4                  |
 | | bit  15:12 R/W  ADAPT_LOOPS_PASS3                  |
 | | bit  11:8  R/W  ADAPT_LOOPS_PASS2                  |
 | | bit  7:4   R/W  ADAPT_LOOPS_PASS1                  |
 | | bit  3:0   R/W  ADAPT_LOOPS_PASS0                  |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS7_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS7_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS7_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS7_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS6_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS6_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS6_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS6_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS5_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS5_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS5_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS5_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS4_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS4_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS4_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS4_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS3_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS3_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS3_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS3_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS2_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS2_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS2_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS2_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS1_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS1_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS1_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS1_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS0_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_LOOPS_PASS0_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS0_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_ADAPT_LOOPS_PASS0_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000470 DIGI120_MTSB_CTRL_GLOBAL_CFG_7 |
 | +----------------------------------------------------+
 | | bit  25    R/W  SSI_OVR_EN                         |
 | | bit  23:19 R/W  SSI_MIN_THRESHOLD                  |
 | | bit  2     R/W  SSI_ADJUST_THRESHOLD               |
 | | bit  1:0   R/W  SSI_REL_AMP                        |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_SSI_OVR_EN_set              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_SSI_OVR_EN_get              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_SSI_MIN_THRESHOLD_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_SSI_MIN_THRESHOLD_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_SSI_MIN_THRESHOLD_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_SSI_MIN_THRESHOLD_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_SSI_ADJUST_THRESHOLD_set    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_SSI_ADJUST_THRESHOLD_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_SSI_REL_AMP_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_SSI_REL_AMP_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_SSI_REL_AMP_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_SSI_REL_AMP_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x00000474 DIGI120_MTSB_CTRL_ADAPT_CFG_1 |
 | +---------------------------------------------------+
 | | bit  28:25 R/W  RR_PERIOD_GAIN                    |
 | | bit  24:21 R/W  RR_PERIOD_ADC_MAP                 |
 | | bit  20:17 R/W  RR_PERIOD_OFFSET                  |
 | | bit  16:13 R/W  RR_PERIOD_COEF                    |
 | | bit  8:5   R/W  RR_PERIOD_DSS                     |
 | | bit  4     R/W  ADAPT_MISSION_ENB                 |
 | | bit  3:1   R/W  ADAPT_DELAY                       |
 | | bit  0     R/W  DSS_ENB                           |
 | +---------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_RR_PERIOD_GAIN_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RR_PERIOD_GAIN_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_RR_PERIOD_GAIN_set    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_RR_PERIOD_GAIN_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_RR_PERIOD_ADC_MAP_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RR_PERIOD_ADC_MAP_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_RR_PERIOD_ADC_MAP_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_RR_PERIOD_ADC_MAP_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_RR_PERIOD_OFFSET_set        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RR_PERIOD_OFFSET_get        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_RR_PERIOD_OFFSET_set  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_RR_PERIOD_OFFSET_get  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_RR_PERIOD_COEF_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RR_PERIOD_COEF_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_RR_PERIOD_COEF_set    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_RR_PERIOD_COEF_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_RR_PERIOD_DSS_set           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_RR_PERIOD_DSS_get           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_RR_PERIOD_DSS_set     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_RR_PERIOD_DSS_get     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_ADAPT_MISSION_ENB_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_MISSION_ENB_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_ADAPT_DELAY_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADAPT_DELAY_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_ADAPT_DELAY_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_ADAPT_DELAY_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_DSS_ENB_set                 ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_DSS_ENB_get                 ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------------------+
 | | Register 0x00000478 DIGI120_MTSB_CTRL_ADAPT_CFG_2 |
 | +---------------------------------------------------+
 | | bit  29  R/W  PATH_PATTERN_ENB                    |
 | | bit  28  R/W  PRELOAD_ENB_P1_7                    |
 | | bit  27  R/W  PRELOAD_ENB_P0                      |
 | | bit  8:6 R/W  CLIP_RATE_THRESH                    |
 | | bit  5:0 R/W  CLIP_RATE_AMP                       |
 | +---------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PATH_PATTERN_ENB_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PATH_PATTERN_ENB_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PRELOAD_ENB_P1_7_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PRELOAD_ENB_P1_7_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PRELOAD_ENB_P0_set         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PRELOAD_ENB_P0_get         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_CLIP_RATE_THRESH_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_CLIP_RATE_THRESH_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_CLIP_RATE_THRESH_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_CLIP_RATE_THRESH_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_CLIP_RATE_AMP_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_CLIP_RATE_AMP_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_CLIP_RATE_AMP_set    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_CLIP_RATE_AMP_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x0000047c DIGI120_MTSB_CTRL_GLOBAL_ADAPT_3 |
 | +------------------------------------------------------+
 | | bit  30:26 R/W  TR_FFE_B_PRELOAD                     |
 | | bit  25:21 R/W  TR_FFE_A_PRELOAD                     |
 | | bit  20:14 R/W  DP_FFE_C_PRELOAD                     |
 | | bit  13:7  R/W  DP_FFE_B_PRELOAD                     |
 | | bit  6:0   R/W  DP_FFE_A_PRELOAD                     |
 | +------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_TR_FFE_B_PRELOAD_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_TR_FFE_B_PRELOAD_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_TR_FFE_B_PRELOAD_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_TR_FFE_B_PRELOAD_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_TR_FFE_A_PRELOAD_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_TR_FFE_A_PRELOAD_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_TR_FFE_A_PRELOAD_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_TR_FFE_A_PRELOAD_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_DP_FFE_C_PRELOAD_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_DP_FFE_C_PRELOAD_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_DP_FFE_C_PRELOAD_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_DP_FFE_C_PRELOAD_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_DP_FFE_B_PRELOAD_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_DP_FFE_B_PRELOAD_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_DP_FFE_B_PRELOAD_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_DP_FFE_B_PRELOAD_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_DP_FFE_A_PRELOAD_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_DP_FFE_A_PRELOAD_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_DP_FFE_A_PRELOAD_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_DP_FFE_A_PRELOAD_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x00000480 DIGI120_MTSB_CTRL_GLOBAL_ADAPT_4 |
 | +------------------------------------------------------+
 | | bit  25:18 R/W  PGA_OFFSET_PRELOAD                   |
 | | bit  17:0  R/W  LMS_DATA_SEL                         |
 | +------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PGA_OFFSET_PRELOAD_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PGA_OFFSET_PRELOAD_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PGA_OFFSET_PRELOAD_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PGA_OFFSET_PRELOAD_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_LMS_DATA_SEL_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_LMS_DATA_SEL_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_LMS_DATA_SEL_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_LMS_DATA_SEL_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x00000484 DIGI120_MTSB_CTRL_GLOBAL_ADAPT_5 |
 | +------------------------------------------------------+
 | | bit  26:18 R/W  DFE_COEF2_PRELOAD                    |
 | | bit  17:9  R/W  DFE_COEF1_PRELOAD                    |
 | | bit  8:0   R/W  DFE_COEF0_PRELOAD                    |
 | +------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_DFE_COEF2_PRELOAD_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_DFE_COEF2_PRELOAD_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_DFE_COEF2_PRELOAD_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_DFE_COEF2_PRELOAD_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_DFE_COEF1_PRELOAD_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_DFE_COEF1_PRELOAD_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_DFE_COEF1_PRELOAD_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_DFE_COEF1_PRELOAD_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_DFE_COEF0_PRELOAD_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_DFE_COEF0_PRELOAD_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_DFE_COEF0_PRELOAD_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_DFE_COEF0_PRELOAD_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x00000490 DIGI120_MTSB_CTRL_GLOBAL_ADAPT_8 |
 | +------------------------------------------------------+
 | | bit  9:4 R/W  OFL_DECAY                              |
 | | bit  3:0 R/W  ADC_MAP_PRELOAD                        |
 | +------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_OFL_DECAY_set             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_OFL_DECAY_get             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_OFL_DECAY_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_OFL_DECAY_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_ADC_MAP_PRELOAD_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_ADC_MAP_PRELOAD_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_ADC_MAP_PRELOAD_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_ADC_MAP_PRELOAD_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x00000494 DIGI120_MTSB_CTRL_ADAPT_CFG_9 |
 | +---------------------------------------------------+
 | | bit  17:0 R/W  OBJECTS_EN_PASS0                   |
 | +---------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_OBJECTS_EN_PASS0_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_OBJECTS_EN_PASS0_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS0_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS0_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000498 DIGI120_MTSB_CTRL_ADAPT_CFG_10 |
 | +----------------------------------------------------+
 | | bit  17:0 R/W  OBJECTS_EN_PASS1                    |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_OBJECTS_EN_PASS1_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_OBJECTS_EN_PASS1_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS1_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS1_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000049c DIGI120_MTSB_CTRL_ADAPT_CFG_11 |
 | +----------------------------------------------------+
 | | bit  17:0 R/W  OBJECTS_EN_PASS2                    |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_OBJECTS_EN_PASS2_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_OBJECTS_EN_PASS2_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS2_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS2_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x000004a0 DIGI120_MTSB_CTRL_ADAPT_CFG_12 |
 | +----------------------------------------------------+
 | | bit  17:0 R/W  OBJECTS_EN_PASS3                    |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_OBJECTS_EN_PASS3_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_OBJECTS_EN_PASS3_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS3_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS3_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x000004a4 DIGI120_MTSB_CTRL_ADAPT_CFG_13 |
 | +----------------------------------------------------+
 | | bit  17:0 R/W  OBJECTS_EN_PASS4                    |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_OBJECTS_EN_PASS4_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_OBJECTS_EN_PASS4_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS4_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS4_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x000004a8 DIGI120_MTSB_CTRL_ADAPT_CFG_14 |
 | +----------------------------------------------------+
 | | bit  17:0 R/W  OBJECTS_EN_PASS5                    |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_OBJECTS_EN_PASS5_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_OBJECTS_EN_PASS5_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS5_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS5_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x000004ac DIGI120_MTSB_CTRL_ADAPT_CFG_15 |
 | +----------------------------------------------------+
 | | bit  17:0 R/W  OBJECTS_EN_PASS6                    |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_OBJECTS_EN_PASS6_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_OBJECTS_EN_PASS6_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS6_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS6_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x000004b0 DIGI120_MTSB_CTRL_ADAPT_CFG_16 |
 | +----------------------------------------------------+
 | | bit  17:0 R/W  OBJECTS_EN_PASS7                    |
 | +----------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_OBJECTS_EN_PASS7_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_OBJECTS_EN_PASS7_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS7_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_OBJECTS_EN_PASS7_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register 0x000004b8 DIGI120_MTSB_CTRL_PGA_LUT_TBL_CFG_1 |
 | +---------------------------------------------------------+
 | | bit  31    R/W  PGA_LUT_RD                              |
 | | bit  30    R/W  PGA_LUT_WR                              |
 | | bit  29    R/W  PGA_LUT_PEAK_EN                         |
 | | bit  28:24 R/W  PGA_LUT_ADDRESS                         |
 | | bit  20:0  R/W  PGA_LUT_WR_DATA                         |
 | +---------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PGA_LUT_PEAK_EN_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PGA_LUT_PEAK_EN_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PGA_LUT_ADDRESS_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PGA_LUT_ADDRESS_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PGA_LUT_ADDRESS_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PGA_LUT_ADDRESS_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PGA_LUT_WR_DATA_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PGA_LUT_WR_DATA_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PGA_LUT_WR_DATA_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PGA_LUT_WR_DATA_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PGA_LUT_RD_set            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PGA_LUT_RD_get            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PGA_LUT_WR_set            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PGA_LUT_WR_get            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------------------------+
 | | Register 0x000004bc DIGI120_MTSB_CTRL_PGA_LUT_TBL_STS_1 |
 | +---------------------------------------------------------+
 | | bit  20:0 R  PGA_LUT_RD_DATA                            |
 | +---------------------------------------------------------+
 | 
 | UINT32 s16_mtsb_ctrl_field_PGA_LUT_RD_DATA_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_range_PGA_LUT_RD_DATA_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register 0x000004c0 DIGI120_MTSB_CTRL_GLOBAL_ADC_CFG_2 |
 | +--------------------------------------------------------+
 | | bit  3:0 R/W  PI_ENB                                   |
 | +--------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PI_ENB_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PI_ENB_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PI_ENB_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PI_ENB_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register 0x000004c4 DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_1 |
 | +----------------------------------------------------------+
 | | bit  31:0 R/W  PHGEN_REGV_TIMER_VALUE                    |
 | +----------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PHGEN_REGV_TIMER_VALUE_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_REGV_TIMER_VALUE_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_REGV_TIMER_VALUE_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_REGV_TIMER_VALUE_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register 0x000004c8 DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_2 |
 | +----------------------------------------------------------+
 | | bit  24    R/W  PHGEN_REGV_INVERT                        |
 | | bit  23:16 R/W  PHGEN_REGV_TOLERANCE                     |
 | | bit  15:0  R/W  PHGEN_REGV_WINDOW_SIZE                   |
 | +----------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PHGEN_REGV_INVERT_set            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_REGV_INVERT_get            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_REGV_TOLERANCE_set         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_REGV_TOLERANCE_get         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_REGV_TOLERANCE_set   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_REGV_TOLERANCE_get   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_REGV_WINDOW_SIZE_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_REGV_WINDOW_SIZE_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_REGV_WINDOW_SIZE_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_REGV_WINDOW_SIZE_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register 0x000004cc DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_3 |
 | +----------------------------------------------------------+
 | | bit  26:18 R/W  PHGEN_REGV_FT_START_VAL                  |
 | | bit  17:9  R/W  PHGEN_REGV_HARD_MAX_LIMIT                |
 | | bit  8:0   R/W  PHGEN_REGV_SOFT_MAX_LIMIT                |
 | +----------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PHGEN_REGV_FT_START_VAL_set         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_REGV_FT_START_VAL_get         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_REGV_FT_START_VAL_set   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_REGV_FT_START_VAL_get   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_REGV_HARD_MAX_LIMIT_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_REGV_HARD_MAX_LIMIT_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_REGV_HARD_MAX_LIMIT_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_REGV_HARD_MAX_LIMIT_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_REGV_SOFT_MAX_LIMIT_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_REGV_SOFT_MAX_LIMIT_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_REGV_SOFT_MAX_LIMIT_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_REGV_SOFT_MAX_LIMIT_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register 0x000004d0 DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_4 |
 | +----------------------------------------------------------+
 | | bit  26:18 R/W  PHGEN_REGV_OFFSET_VAL                    |
 | | bit  17:9  R/W  PHGEN_REGV_HARD_MIN_LIMIT                |
 | | bit  8:0   R/W  PHGEN_REGV_SOFT_MIN_LIMIT                |
 | +----------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PHGEN_REGV_OFFSET_VAL_set           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_REGV_OFFSET_VAL_get           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_REGV_OFFSET_VAL_set     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_REGV_OFFSET_VAL_get     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_REGV_HARD_MIN_LIMIT_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_REGV_HARD_MIN_LIMIT_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_REGV_HARD_MIN_LIMIT_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_REGV_HARD_MIN_LIMIT_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_REGV_SOFT_MIN_LIMIT_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_REGV_SOFT_MIN_LIMIT_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_REGV_SOFT_MIN_LIMIT_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_REGV_SOFT_MIN_LIMIT_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000004d4 DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_1 |
 | +-------------------------------------------------------------+
 | | bit  31:0 R/W  PHGEN_FT_TIMER_VALUE                         |
 | +-------------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PHGEN_FT_TIMER_VALUE_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_TIMER_VALUE_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_FT_TIMER_VALUE_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_FT_TIMER_VALUE_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000004d8 DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_2 |
 | +-------------------------------------------------------------+
 | | bit  24    R/W  PHGEN_FT_INVERT                             |
 | | bit  23:16 R/W  PHGEN_FT_TOLERANCE                          |
 | | bit  15:0  R/W  PHGEN_FT_WINDOW_SIZE                        |
 | +-------------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PHGEN_FT_INVERT_set            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_INVERT_get            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_FT_TOLERANCE_set         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_TOLERANCE_get         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_FT_TOLERANCE_set   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_FT_TOLERANCE_get   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_FT_WINDOW_SIZE_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_WINDOW_SIZE_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_FT_WINDOW_SIZE_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_FT_WINDOW_SIZE_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000004dc DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_3 |
 | +-------------------------------------------------------------+
 | | bit  17:9 R/W  PHGEN_FT_HARD_MAX_LIMIT                      |
 | | bit  8:0  R/W  PHGEN_FT_SOFT_MAX_LIMIT                      |
 | +-------------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PHGEN_FT_HARD_MAX_LIMIT_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_HARD_MAX_LIMIT_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_FT_HARD_MAX_LIMIT_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_FT_HARD_MAX_LIMIT_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_FT_SOFT_MAX_LIMIT_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_SOFT_MAX_LIMIT_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_FT_SOFT_MAX_LIMIT_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_FT_SOFT_MAX_LIMIT_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000004e0 DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_4 |
 | +-------------------------------------------------------------+
 | | bit  26:18 R/W  PHGEN_FT_OFFSET_VAL                         |
 | | bit  17:9  R/W  PHGEN_FT_HARD_MIN_LIMIT                     |
 | | bit  8:0   R/W  PHGEN_FT_SOFT_MIN_LIMIT                     |
 | +-------------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PHGEN_FT_OFFSET_VAL_set           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_OFFSET_VAL_get           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_FT_OFFSET_VAL_set     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_FT_OFFSET_VAL_get     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_FT_HARD_MIN_LIMIT_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_HARD_MIN_LIMIT_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_FT_HARD_MIN_LIMIT_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_FT_HARD_MIN_LIMIT_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_FT_SOFT_MIN_LIMIT_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_SOFT_MIN_LIMIT_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_FT_SOFT_MIN_LIMIT_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_FT_SOFT_MIN_LIMIT_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register 0x000004e4 DIGI120_MTSB_CTRL_GLOBAL_PHGEN_MABC_CFG_1 |
 | +---------------------------------------------------------------+
 | | bit  28:23 R/W  PHGEN_CT_SOFT_MAX_LIMIT                       |
 | | bit  22:17 R/W  PHGEN_CT_SOFT_MIN_LIMIT                       |
 | | bit  16:15 R/W  PHGEN_MODE_7D6                                |
 | | bit  14:10 R/W  PHGEN_MODE_5D1                                |
 | | bit  9     R/W  PHGEN_CT_INVERT                               |
 | | bit  8     R/W  PHGEN_ATMSB                                   |
 | | bit  7:5   R/W  PHGEN_CT_SETTLE                               |
 | | bit  4:3   R/W  PHGEN_CT_NUM_CYCLE                            |
 | +---------------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PHGEN_CT_SOFT_MAX_LIMIT_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_CT_SOFT_MAX_LIMIT_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_CT_SOFT_MAX_LIMIT_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_CT_SOFT_MAX_LIMIT_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_CT_SOFT_MIN_LIMIT_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_CT_SOFT_MIN_LIMIT_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_CT_SOFT_MIN_LIMIT_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_CT_SOFT_MIN_LIMIT_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_MODE_7D6_set                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_MODE_7D6_get                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_MODE_7D6_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_MODE_7D6_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_MODE_5D1_set                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_MODE_5D1_get                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_MODE_5D1_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_MODE_5D1_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_CT_INVERT_set               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_CT_INVERT_get               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_ATMSB_set                   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_ATMSB_get                   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_CT_SETTLE_set               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_CT_SETTLE_get               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_CT_SETTLE_set         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_CT_SETTLE_get         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_CT_NUM_CYCLE_set            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_CT_NUM_CYCLE_get            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_CT_NUM_CYCLE_set      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_CT_NUM_CYCLE_get      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register 0x000004e8 DIGI120_MTSB_CTRL_GLOBAL_PHGEN_OVR_1 |
 | +----------------------------------------------------------+
 | | bit  17    R/W  PHGEN_CT_OVR_EN                          |
 | | bit  16    R/W  PHGEN_CT_OVR_DONE                        |
 | | bit  15:10 R/W  PHGEN_CT_OVR_VALUE                       |
 | | bit  9     R/W  PHGEN_FT_OVR_EN                          |
 | | bit  8:0   R/W  PHGEN_FT_OVR_VALUE                       |
 | +----------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PHGEN_CT_OVR_EN_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_CT_OVR_EN_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_CT_OVR_DONE_set        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_CT_OVR_DONE_get        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_CT_OVR_VALUE_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_CT_OVR_VALUE_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_CT_OVR_VALUE_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_CT_OVR_VALUE_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_mtsb_ctrl_field_PHGEN_FT_OVR_EN_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_OVR_EN_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_FT_OVR_VALUE_set       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_OVR_VALUE_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_range_PHGEN_FT_OVR_VALUE_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_FT_OVR_VALUE_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register 0x000004ec DIGI120_MTSB_CTRL_GLOBAL_PHGEN_OBS_1 |
 | +----------------------------------------------------------+
 | | bit  18:13 R  PHGEN_CTUNE_OBS                            |
 | | bit  12:4  R  PHGEN_FTUNE_OBS                            |
 | | bit  3     R  PHGEN_DLYCOR_DATA_OBS                      |
 | | bit  2     R  PHGEN_REGV_TIMER_OUT                       |
 | | bit  1     R  PHGEN_FT_TIMER_OUT                         |
 | | bit  0     R  PHGEN_CT_DONE                              |
 | +----------------------------------------------------------+
 | 
 | UINT32 s16_mtsb_ctrl_field_PHGEN_CTUNE_OBS_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_CTUNE_OBS_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FTUNE_OBS_get       ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_range_PHGEN_FTUNE_OBS_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_DLYCOR_DATA_OBS_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_REGV_TIMER_OUT_get  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_TIMER_OUT_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_CT_DONE_get         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------------+
 | | Register 0x000004f0 DIGI120_MTSB_CTRL_GLOBAL_INT |
 | +--------------------------------------------------+
 | | bit  4 R/W  PHGEN_CT_OVR_LIMIT_INT_E             |
 | | bit  3 R/W  PHGEN_REGV_INT_MAX_LIMIT_E           |
 | | bit  2 R/W  PHGEN_REGV_INT_MIN_LIMIT_E           |
 | | bit  1 R/W  PHGEN_FT_INT_MAX_LIMIT_E             |
 | | bit  0 R/W  PHGEN_FT_INT_MIN_LIMIT_E             |
 | +--------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_PHGEN_CT_OVR_LIMIT_INT_E_set   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_CT_OVR_LIMIT_INT_E_get   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_REGV_INT_MAX_LIMIT_E_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_REGV_INT_MAX_LIMIT_E_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_REGV_INT_MIN_LIMIT_E_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_REGV_INT_MIN_LIMIT_E_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_FT_INT_MAX_LIMIT_E_set   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_INT_MAX_LIMIT_E_get   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_mtsb_ctrl_field_PHGEN_FT_INT_MIN_LIMIT_E_set   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_PHGEN_FT_INT_MIN_LIMIT_E_get   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------------------+
 | | Register 0x000004f4 DIGI120_MTSB_CTRL_GLOBAL_INT_E |
 | +----------------------------------------------------+
 | | bit  4 R/W  PHGEN_CT_OVR_LIMIT_INT_I               |
 | | bit  3 R/W  PHGEN_REGV_INT_MAX_LIMIT_I             |
 | | bit  2 R/W  PHGEN_REGV_INT_MIN_LIMIT_I             |
 | | bit  1 R/W  PHGEN_FT_INT_MAX_LIMIT_I               |
 | | bit  0 R/W  PHGEN_FT_INT_MIN_LIMIT_I               |
 | +----------------------------------------------------+
 | 
 | void                 s16_mtsb_ctrl_field_PHGEN_CT_OVR_LIMIT_INT_I_set_to_clear   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_mtsb_ctrl_field_PHGEN_CT_OVR_LIMIT_INT_I_get            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_PHGEN_CT_OVR_LIMIT_INT_I_poll           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_mtsb_ctrl_field_PHGEN_REGV_INT_MAX_LIMIT_I_set_to_clear ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_mtsb_ctrl_field_PHGEN_REGV_INT_MAX_LIMIT_I_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_PHGEN_REGV_INT_MAX_LIMIT_I_poll         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_mtsb_ctrl_field_PHGEN_REGV_INT_MIN_LIMIT_I_set_to_clear ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_mtsb_ctrl_field_PHGEN_REGV_INT_MIN_LIMIT_I_get          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_PHGEN_REGV_INT_MIN_LIMIT_I_poll         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_mtsb_ctrl_field_PHGEN_FT_INT_MAX_LIMIT_I_set_to_clear   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_mtsb_ctrl_field_PHGEN_FT_INT_MAX_LIMIT_I_get            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_PHGEN_FT_INT_MAX_LIMIT_I_poll           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_mtsb_ctrl_field_PHGEN_FT_INT_MIN_LIMIT_I_set_to_clear   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_mtsb_ctrl_field_PHGEN_FT_INT_MIN_LIMIT_I_get            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_PHGEN_FT_INT_MIN_LIMIT_I_poll           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------------+
 | | Register 0x000004f8 DIGI120_MTSB_CTRL_GLOBAL_INT_V |
 | +----------------------------------------------------+
 | | bit  4 R  PHGEN_CT_OVR_LIMIT_INT_V                 |
 | | bit  3 R  PHGEN_REGV_INT_MAX_LIMIT_V               |
 | | bit  2 R  PHGEN_REGV_INT_MIN_LIMIT_V               |
 | | bit  1 R  PHGEN_FT_INT_MAX_LIMIT_V                 |
 | | bit  0 R  PHGEN_FT_INT_MIN_LIMIT_V                 |
 | +----------------------------------------------------+
 | 
 | UINT32               s16_mtsb_ctrl_field_PHGEN_CT_OVR_LIMIT_INT_V_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_PHGEN_CT_OVR_LIMIT_INT_V_poll   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_mtsb_ctrl_field_PHGEN_REGV_INT_MAX_LIMIT_V_get  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_PHGEN_REGV_INT_MAX_LIMIT_V_poll ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_mtsb_ctrl_field_PHGEN_REGV_INT_MIN_LIMIT_V_get  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_PHGEN_REGV_INT_MIN_LIMIT_V_poll ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_mtsb_ctrl_field_PHGEN_FT_INT_MAX_LIMIT_V_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_PHGEN_FT_INT_MAX_LIMIT_V_poll   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_mtsb_ctrl_field_PHGEN_FT_INT_MIN_LIMIT_V_get    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_field_PHGEN_FT_INT_MIN_LIMIT_V_poll   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000004fc DIGI120_MTSB_CTRL_GLOBAL_VGP_STRT_CFG_1 |
 | +-------------------------------------------------------------+
 | | bit  15 R/W  VGP_OCC_START                                  |
 | +-------------------------------------------------------------+
 | 
 | void   s16_mtsb_ctrl_field_VGP_OCC_START_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_mtsb_ctrl_field_VGP_OCC_START_get ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _S16_MTSB_CTRL_IO_H
#define _S16_MTSB_CTRL_IO_H

#include "s16_loc.h"
#include "s16_mtsb_ctrl_regs.h"
#include "s16_mtsb_ctrl_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 s16_mtsb_ctrl_buffer_init               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void                 s16_mtsb_ctrl_buffer_flush              ( s16_mtsb_ctrl_buffer_t *b_ptr )
 | UINT32               s16_mtsb_ctrl_reg_read                  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 s16_mtsb_ctrl_reg_write                 ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 s16_mtsb_ctrl_field_set                 ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 s16_mtsb_ctrl_action_on_write_field_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 s16_mtsb_ctrl_burst_read                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 s16_mtsb_ctrl_burst_write               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_poll                      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_CFG_1_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_CFG_1_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_CFG_1_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_PGA_CFG_1_write                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_PGA_CFG_1_field_set                      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_PGA_CFG_1_read                           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_PISO_CFG_1_write                         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_PISO_CFG_1_field_set                     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_PISO_CFG_1_read                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_ADAPT_CFG_1_write                        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_ADAPT_CFG_1_field_set                    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_ADAPT_CFG_1_read                         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_CFG_REG_write                            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_CFG_REG_field_set                        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_CFG_REG_read                             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_OBS_1_REG_write                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_OBS_1_REG_field_set                      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_OBS_1_REG_read                           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PISO_CFG_1_write                         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PISO_CFG_1_field_set                     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PISO_CFG_1_read                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_MTSB_CTRL_GLOBAL_ADC_CFG_1_write                                  ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_MTSB_CTRL_GLOBAL_ADC_CFG_1_field_set                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_MTSB_CTRL_GLOBAL_ADC_CFG_1_read                                   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_MTSB_CFG_1_write                         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_MTSB_CFG_1_field_set                     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_MTSB_CFG_1_read                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_TCB_CFG_1_write                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_TCB_CFG_1_field_set                      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_TCB_CFG_1_read                           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_TCB_CFG_2_write                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_TCB_CFG_2_field_set                      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_TCB_CFG_2_read                           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_1_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_1_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_1_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_2_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_2_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_2_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_3_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_3_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_3_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_4_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_4_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_4_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_5_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_5_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_5_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_6_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_6_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_6_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_7_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_7_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_CFG_7_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_1_write                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_1_field_set                           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_1_read                                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_2_write                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_2_field_set                           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_2_read                                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADAPT_3_write                            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADAPT_3_field_set                        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADAPT_3_read                             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADAPT_4_write                            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADAPT_4_field_set                        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADAPT_4_read                             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADAPT_5_write                            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADAPT_5_field_set                        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADAPT_5_read                             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADAPT_8_write                            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADAPT_8_field_set                        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADAPT_8_read                             ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_9_write                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_9_field_set                           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_9_read                                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_10_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_10_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_10_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_11_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_11_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_11_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_12_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_12_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_12_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_13_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_13_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_13_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_14_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_14_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_14_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_15_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_15_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_15_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_16_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_16_field_set                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_ADAPT_CFG_16_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_PGA_LUT_TBL_CFG_1_write                         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_PGA_LUT_TBL_CFG_1_field_set                     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_PGA_LUT_TBL_CFG_1_read                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADC_CFG_2_write                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADC_CFG_2_field_set                      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_ADC_CFG_2_read                           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_1_write                        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_1_field_set                    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_1_read                         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_2_write                        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_2_field_set                    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_2_read                         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_3_write                        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_3_field_set                    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_3_read                         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_4_write                        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_4_field_set                    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_CFG_4_read                         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_1_write                     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_1_field_set                 ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_1_read                      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_2_write                     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_2_field_set                 ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_2_read                      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_3_write                     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_3_field_set                 ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_3_read                      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_4_write                     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_4_field_set                 ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_FT_CFG_4_read                      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_MABC_CFG_1_write                   ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_MABC_CFG_1_field_set               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_MABC_CFG_1_read                    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_OVR_1_write                        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_OVR_1_field_set                    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_OVR_1_read                         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_VGP_STRT_CFG_1_write                     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_VGP_STRT_CFG_1_field_set                 ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_VGP_STRT_CFG_1_read                      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_STAT_INT_EVENT_write                     ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_STAT_INT_EVENT_action_on_write_field_set ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_STAT_INT_EVENT_read                      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_STAT_INT_EVENT_poll                      ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_INT_E_write                              ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_INT_E_action_on_write_field_set          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_INT_E_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_INT_E_poll                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_STAT_INT_EN_write                        ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_STAT_INT_EN_field_set                    ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_STAT_INT_EN_read                         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_INT_write                                ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_INT_field_set                            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_INT_read                                 ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_STAT_INT_read                            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_STAT_INT_poll                            ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_INT_V_read                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_INT_V_poll                               ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_LANE_N_OBS_2_REG_read                           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_TCB_STS_1_read                           ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_PGA_LUT_TBL_STS_1_read                          ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_mtsb_ctrl_reg_DIGI120_MTSB_CTRL_GLOBAL_PHGEN_OBS_1_read                         ( s16_mtsb_ctrl_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _S16_MTSB_CTRL_IO_H */
