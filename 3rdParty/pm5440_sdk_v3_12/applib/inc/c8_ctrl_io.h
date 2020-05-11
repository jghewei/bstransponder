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
 *     and register accessor functions for the c8_ctrl block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing c8_ctrl io functions is:
 | {
 |     c8_ctrl_buffer_t b_ptr[1];
 |     c8_ctrl_buffer_init( b_ptr, h_ptr, A );
 |     value = c8_ctrl_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     c8_ctrl_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     c8_ctrl_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = c8_ctrl_field_<FIELD>_get( NULL, h_ptr, A );
 | c8_ctrl_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------+
 | | Register 0x00000c00 MTSB_FSM_LANE_CFG_1 |
 | +-----------------------------------------+
 | | bit  10 R/W  ADAPT_CLK_ENB_OVR          |
 | | bit  9  R/W  ADAPT_CLK_ENB_OVR_EN       |
 | | bit  8  R/W  ADAPT_ENB_OVR              |
 | | bit  7  R/W  ADAPT_ENB_OVR_EN           |
 | | bit  6  R/W  FORCE_ENABLE_COUNTER_TRIG  |
 | | bit  5  R/W  ADAPT_STATUS_0_OVR         |
 | | bit  4  R/W  ADAPT_STATUS_0_OVR_EN      |
 | | bit  3  R/W  ADAPT_INIT_FORCE           |
 | | bit  2  R/W  TO_PCS_PMA_OVR_EN          |
 | | bit  1  R/W  FROM_PCS_PMA_OVR_EN        |
 | | bit  0  R/W  RESET_ADPT_FSM             |
 | +-----------------------------------------+
 | 
 | void   c8_ctrl_field_ADAPT_CLK_ENB_OVR_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_CLK_ENB_OVR_get         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_ADAPT_CLK_ENB_OVR_EN_set      ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_CLK_ENB_OVR_EN_get      ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_ADAPT_ENB_OVR_set             ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_ENB_OVR_get             ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_ADAPT_ENB_OVR_EN_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_ENB_OVR_EN_get          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_FORCE_ENABLE_COUNTER_TRIG_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_FORCE_ENABLE_COUNTER_TRIG_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_ADAPT_STATUS_0_OVR_set        ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_STATUS_0_OVR_get        ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_ADAPT_STATUS_0_OVR_EN_set     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_STATUS_0_OVR_EN_get     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_ADAPT_INIT_FORCE_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_INIT_FORCE_get          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_TO_PCS_PMA_OVR_EN_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_TO_PCS_PMA_OVR_EN_get         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_FROM_PCS_PMA_OVR_EN_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_FROM_PCS_PMA_OVR_EN_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_RESET_ADPT_FSM_set            ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_RESET_ADPT_FSM_get            ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------+
 | | Register 0x00000c08 MTSB_FSM_LANE_CFG_2 |
 | +-----------------------------------------+
 | | bit  20:18 R/W  PRELOAD_ADC_INDEX_GEN3  |
 | | bit  17:15 R/W  PRELOAD_ADC_INDEX_GEN2  |
 | | bit  14:12 R/W  PRELOAD_ADC_INDEX_GEN1  |
 | | bit  11:9  R/W  PRELOAD_FFE_INDEX_GEN3  |
 | | bit  8:6   R/W  PRELOAD_FFE_INDEX_GEN2  |
 | | bit  5:3   R/W  PRELOAD_FFE_INDEX_GEN1  |
 | | bit  2     R/W  PRELOAD_LUT_EN_GEN3     |
 | | bit  1     R/W  PRELOAD_LUT_EN_GEN2     |
 | | bit  0     R/W  PRELOAD_LUT_EN_GEN1     |
 | +-----------------------------------------+
 | 
 | void   c8_ctrl_field_PRELOAD_ADC_INDEX_GEN3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PRELOAD_ADC_INDEX_GEN3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_PRELOAD_ADC_INDEX_GEN3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_PRELOAD_ADC_INDEX_GEN3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_PRELOAD_ADC_INDEX_GEN2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PRELOAD_ADC_INDEX_GEN2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_PRELOAD_ADC_INDEX_GEN2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_PRELOAD_ADC_INDEX_GEN2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_PRELOAD_ADC_INDEX_GEN1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PRELOAD_ADC_INDEX_GEN1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_PRELOAD_ADC_INDEX_GEN1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_PRELOAD_ADC_INDEX_GEN1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_PRELOAD_FFE_INDEX_GEN3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PRELOAD_FFE_INDEX_GEN3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_PRELOAD_FFE_INDEX_GEN3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_PRELOAD_FFE_INDEX_GEN3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_PRELOAD_FFE_INDEX_GEN2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PRELOAD_FFE_INDEX_GEN2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_PRELOAD_FFE_INDEX_GEN2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_PRELOAD_FFE_INDEX_GEN2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_PRELOAD_FFE_INDEX_GEN1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PRELOAD_FFE_INDEX_GEN1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_PRELOAD_FFE_INDEX_GEN1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_PRELOAD_FFE_INDEX_GEN1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_PRELOAD_LUT_EN_GEN3_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PRELOAD_LUT_EN_GEN3_get          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_PRELOAD_LUT_EN_GEN2_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PRELOAD_LUT_EN_GEN2_get          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_PRELOAD_LUT_EN_GEN1_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PRELOAD_LUT_EN_GEN1_get          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------+
 | | Register 0x00000c20 STAT_INT_EN |
 | +---------------------------------+
 | | bit  4 R/W  ADAPT_STATUS_0_E    |
 | | bit  3 R/W  RX_ADAPT_DONE_E     |
 | | bit  2 R/W  GEN_RATE_1_E        |
 | | bit  1 R/W  GEN_RATE_0_E        |
 | | bit  0 R/W  RX_ADAPT_START_E    |
 | +---------------------------------+
 | 
 | void   c8_ctrl_field_ADAPT_STATUS_0_E_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_STATUS_0_E_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_RX_ADAPT_DONE_E_set  ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_RX_ADAPT_DONE_E_get  ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_GEN_RATE_1_E_set     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_GEN_RATE_1_E_get     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_GEN_RATE_0_E_set     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_GEN_RATE_0_E_get     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_RX_ADAPT_START_E_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_RX_ADAPT_START_E_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------+
 | | Register 0x00000c28 STAT_INT_EVENT |
 | +------------------------------------+
 | | bit  4 R/W  ADAPT_STATUS_0_I       |
 | | bit  3 R/W  RX_ADAPT_DONE_I        |
 | | bit  2 R/W  GEN_RATE_1_I           |
 | | bit  1 R/W  GEN_RATE_0_I           |
 | | bit  0 R/W  RX_ADAPT_START_I       |
 | +------------------------------------+
 | 
 | void                 c8_ctrl_field_GEN_RATE_1_I_set_to_clear     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_ctrl_field_GEN_RATE_1_I_get              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_ctrl_field_GEN_RATE_1_I_poll             ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_ctrl_field_GEN_RATE_0_I_set_to_clear     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_ctrl_field_GEN_RATE_0_I_get              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_ctrl_field_GEN_RATE_0_I_poll             ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_ctrl_field_ADAPT_STATUS_0_I_set_to_clear ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_ctrl_field_ADAPT_STATUS_0_I_get          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_ctrl_field_ADAPT_STATUS_0_I_poll         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_ctrl_field_RX_ADAPT_DONE_I_set_to_clear  ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_ctrl_field_RX_ADAPT_DONE_I_get           ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_ctrl_field_RX_ADAPT_DONE_I_poll          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_ctrl_field_RX_ADAPT_START_I_set_to_clear ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_ctrl_field_RX_ADAPT_START_I_get          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_ctrl_field_RX_ADAPT_START_I_poll         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------+
 | | Register 0x00000c30 STAT_INT |
 | +------------------------------+
 | | bit  4 R  ADAPT_STATUS_0_V   |
 | | bit  3 R  RX_ADAPT_DONE_V    |
 | | bit  2 R  GEN_RATE_1_V       |
 | | bit  1 R  GEN_RATE_0_V       |
 | | bit  0 R  RX_ADAPT_START_V   |
 | +------------------------------+
 | 
 | UINT32               c8_ctrl_field_ADAPT_STATUS_0_V_get  ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_ctrl_field_ADAPT_STATUS_0_V_poll ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_ctrl_field_RX_ADAPT_DONE_V_get   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_ctrl_field_RX_ADAPT_DONE_V_poll  ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_ctrl_field_GEN_RATE_1_V_get      ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_ctrl_field_GEN_RATE_1_V_poll     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_ctrl_field_GEN_RATE_0_V_get      ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_ctrl_field_GEN_RATE_0_V_poll     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_ctrl_field_RX_ADAPT_START_V_get  ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_ctrl_field_RX_ADAPT_START_V_poll ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000c38 MTSB_FSM_LANE_OBSERVABILITY_ |
 | +--------------------------------------------------+
 | | bit  8:6 R    ADPT_STATE_OBS                     |
 | | bit  5   R/W  CAPTURE_REQ_ADPT_STATE             |
 | | bit  4   R    RX_ADAPT_DONE_OBS                  |
 | | bit  3:2 R    GEN_RATE_OBS                       |
 | | bit  1   R    RX_ADAPT_START_OBS                 |
 | | bit  0   R/W  CAPTURE_REQ_PCS_PMA                |
 | +--------------------------------------------------+
 | 
 | void   c8_ctrl_field_CAPTURE_REQ_ADPT_STATE_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_CAPTURE_REQ_ADPT_STATE_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_CAPTURE_REQ_PCS_PMA_set    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_CAPTURE_REQ_PCS_PMA_get    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_ctrl_field_ADPT_STATE_OBS_get         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_ctrl_field_range_ADPT_STATE_OBS_get   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 c8_ctrl_field_RX_ADAPT_DONE_OBS_get      ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_ctrl_field_GEN_RATE_OBS_get           ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_ctrl_field_range_GEN_RATE_OBS_get     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 c8_ctrl_field_RX_ADAPT_START_OBS_get     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------------------+
 | | Register 0x00000c40 MTSB_FSM_GLOBAL_CFG_1 |
 | +-------------------------------------------+
 | | bit  3:0 R/W  ADAPT_L0_CNTR               |
 | +-------------------------------------------+
 | 
 | void   c8_ctrl_field_ADAPT_L0_CNTR_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_L0_CNTR_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_L0_CNTR_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_L0_CNTR_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000c44 MTSB_FSM_GLOBAL_CFG_2 |
 | +-------------------------------------------+
 | | bit  26    R/W  DSS_ENB_GEN3              |
 | | bit  25    R/W  DSS_ENB_GEN2              |
 | | bit  24    R/W  DSS_ENB_GEN1              |
 | | bit  23:21 R/W  MU_STEP_ADC_MAP_PASS3     |
 | | bit  20:18 R/W  MU_STEP_ADC_MAP_PASS2     |
 | | bit  17:15 R/W  MU_STEP_ADC_MAP_PASS1     |
 | | bit  14:12 R/W  MU_STEP_ADC_MAP_PASS0     |
 | | bit  11:9  R/W  MU_STEP_COEF_PASS3        |
 | | bit  8:6   R/W  MU_STEP_COEF_PASS2        |
 | | bit  5:3   R/W  MU_STEP_COEF_PASS1        |
 | | bit  2:0   R/W  MU_STEP_COEF_PASS0        |
 | +-------------------------------------------+
 | 
 | void   c8_ctrl_field_DSS_ENB_GEN3_set                ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_DSS_ENB_GEN3_get                ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_DSS_ENB_GEN2_set                ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_DSS_ENB_GEN2_get                ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_DSS_ENB_GEN1_set                ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_DSS_ENB_GEN1_get                ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_MU_STEP_ADC_MAP_PASS3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_MU_STEP_ADC_MAP_PASS3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_MU_STEP_ADC_MAP_PASS3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_MU_STEP_ADC_MAP_PASS3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_MU_STEP_ADC_MAP_PASS2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_MU_STEP_ADC_MAP_PASS2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_MU_STEP_ADC_MAP_PASS2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_MU_STEP_ADC_MAP_PASS2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_MU_STEP_ADC_MAP_PASS1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_MU_STEP_ADC_MAP_PASS1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_MU_STEP_ADC_MAP_PASS1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_MU_STEP_ADC_MAP_PASS1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_MU_STEP_ADC_MAP_PASS0_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_MU_STEP_ADC_MAP_PASS0_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_MU_STEP_ADC_MAP_PASS0_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_MU_STEP_ADC_MAP_PASS0_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_MU_STEP_COEF_PASS3_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_MU_STEP_COEF_PASS3_get          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_MU_STEP_COEF_PASS3_set    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_MU_STEP_COEF_PASS3_get    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_MU_STEP_COEF_PASS2_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_MU_STEP_COEF_PASS2_get          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_MU_STEP_COEF_PASS2_set    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_MU_STEP_COEF_PASS2_get    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_MU_STEP_COEF_PASS1_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_MU_STEP_COEF_PASS1_get          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_MU_STEP_COEF_PASS1_set    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_MU_STEP_COEF_PASS1_get    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_MU_STEP_COEF_PASS0_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_MU_STEP_COEF_PASS0_get          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_MU_STEP_COEF_PASS0_set    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_MU_STEP_COEF_PASS0_get    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000c48 MTSB_FSM_GLOBAL_CFG_3 |
 | +-------------------------------------------+
 | | bit  23:20 R/W  RR_PERIOD_COEF_GEN3       |
 | | bit  19:16 R/W  RR_PERIOD_COEF_GEN2       |
 | | bit  15:12 R/W  RR_PERIOD_COEF_GEN1       |
 | | bit  11    R/W  ADAPT_L0_ENB_GEN3         |
 | | bit  10    R/W  ADAPT_L0_ENB_GEN2         |
 | | bit  9     R/W  ADAPT_L0_ENB_GEN1         |
 | +-------------------------------------------+
 | 
 | void   c8_ctrl_field_RR_PERIOD_COEF_GEN3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_RR_PERIOD_COEF_GEN3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_RR_PERIOD_COEF_GEN3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_RR_PERIOD_COEF_GEN3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_RR_PERIOD_COEF_GEN2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_RR_PERIOD_COEF_GEN2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_RR_PERIOD_COEF_GEN2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_RR_PERIOD_COEF_GEN2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_RR_PERIOD_COEF_GEN1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_RR_PERIOD_COEF_GEN1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_RR_PERIOD_COEF_GEN1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_RR_PERIOD_COEF_GEN1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_L0_ENB_GEN3_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_L0_ENB_GEN3_get         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_ADAPT_L0_ENB_GEN2_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_L0_ENB_GEN2_get         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_ADAPT_L0_ENB_GEN1_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_L0_ENB_GEN1_get         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------------------+
 | | Register 0x00000c4c MTSB_FSM_GLOBAL_CFG_4 |
 | +-------------------------------------------+
 | | bit  23:20 R/W  RR_PERIOD_DSS_GEN3        |
 | | bit  19:16 R/W  RR_PERIOD_DSS_GEN2        |
 | | bit  15:12 R/W  RR_PERIOD_DSS_GEN1        |
 | | bit  11:8  R/W  RR_PERIOD_ADC_MAP_GEN3    |
 | | bit  7:4   R/W  RR_PERIOD_ADC_MAP_GEN2    |
 | | bit  3:0   R/W  RR_PERIOD_ADC_MAP_GEN1    |
 | +-------------------------------------------+
 | 
 | void   c8_ctrl_field_RR_PERIOD_DSS_GEN3_set           ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_RR_PERIOD_DSS_GEN3_get           ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_RR_PERIOD_DSS_GEN3_set     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_RR_PERIOD_DSS_GEN3_get     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_RR_PERIOD_DSS_GEN2_set           ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_RR_PERIOD_DSS_GEN2_get           ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_RR_PERIOD_DSS_GEN2_set     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_RR_PERIOD_DSS_GEN2_get     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_RR_PERIOD_DSS_GEN1_set           ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_RR_PERIOD_DSS_GEN1_get           ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_RR_PERIOD_DSS_GEN1_set     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_RR_PERIOD_DSS_GEN1_get     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_RR_PERIOD_ADC_MAP_GEN3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_RR_PERIOD_ADC_MAP_GEN3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_RR_PERIOD_ADC_MAP_GEN3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_RR_PERIOD_ADC_MAP_GEN3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_RR_PERIOD_ADC_MAP_GEN2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_RR_PERIOD_ADC_MAP_GEN2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_RR_PERIOD_ADC_MAP_GEN2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_RR_PERIOD_ADC_MAP_GEN2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_RR_PERIOD_ADC_MAP_GEN1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_RR_PERIOD_ADC_MAP_GEN1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_RR_PERIOD_ADC_MAP_GEN1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_RR_PERIOD_ADC_MAP_GEN1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000c50 MTSB_FSM_GLOBAL_CFG_5  |
 | +--------------------------------------------+
 | | bit  14:10 R/W  DP_FFE_A_PRELOAD_PCBI_GEN3 |
 | | bit  9:5   R/W  DP_FFE_A_PRELOAD_PCBI_GEN2 |
 | | bit  4:0   R/W  DP_FFE_A_PRELOAD_PCBI_GEN1 |
 | +--------------------------------------------+
 | 
 | void   c8_ctrl_field_DP_FFE_A_PRELOAD_PCBI_GEN3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_DP_FFE_A_PRELOAD_PCBI_GEN3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_DP_FFE_A_PRELOAD_PCBI_GEN3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_DP_FFE_A_PRELOAD_PCBI_GEN3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_DP_FFE_A_PRELOAD_PCBI_GEN2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_DP_FFE_A_PRELOAD_PCBI_GEN2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_DP_FFE_A_PRELOAD_PCBI_GEN2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_DP_FFE_A_PRELOAD_PCBI_GEN2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_DP_FFE_A_PRELOAD_PCBI_GEN1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_DP_FFE_A_PRELOAD_PCBI_GEN1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_DP_FFE_A_PRELOAD_PCBI_GEN1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_DP_FFE_A_PRELOAD_PCBI_GEN1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000c54 MTSB_FSM_GLOBAL_CFG_6 |
 | +-------------------------------------------+
 | | bit  29:25 R/W  DP_FFE_A_LUT_5            |
 | | bit  24:20 R/W  DP_FFE_A_LUT_4            |
 | | bit  19:15 R/W  DP_FFE_A_LUT_3            |
 | | bit  14:10 R/W  DP_FFE_A_LUT_2            |
 | | bit  9:5   R/W  DP_FFE_A_LUT_1            |
 | | bit  4:0   R/W  DP_FFE_A_LUT_0            |
 | +-------------------------------------------+
 | 
 | void   c8_ctrl_field_DP_FFE_A_LUT_5_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_DP_FFE_A_LUT_5_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_DP_FFE_A_LUT_5_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_DP_FFE_A_LUT_5_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_DP_FFE_A_LUT_4_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_DP_FFE_A_LUT_4_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_DP_FFE_A_LUT_4_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_DP_FFE_A_LUT_4_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_DP_FFE_A_LUT_3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_DP_FFE_A_LUT_3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_DP_FFE_A_LUT_3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_DP_FFE_A_LUT_3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_DP_FFE_A_LUT_2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_DP_FFE_A_LUT_2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_DP_FFE_A_LUT_2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_DP_FFE_A_LUT_2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_DP_FFE_A_LUT_1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_DP_FFE_A_LUT_1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_DP_FFE_A_LUT_1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_DP_FFE_A_LUT_1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_DP_FFE_A_LUT_0_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_DP_FFE_A_LUT_0_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_DP_FFE_A_LUT_0_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_DP_FFE_A_LUT_0_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000c58 MTSB_FSM_GLOBAL_CFG_7 |
 | +-------------------------------------------+
 | | bit  9:5 R/W  DP_FFE_A_LUT_7              |
 | | bit  4:0 R/W  DP_FFE_A_LUT_6              |
 | +-------------------------------------------+
 | 
 | void   c8_ctrl_field_DP_FFE_A_LUT_7_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_DP_FFE_A_LUT_7_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_DP_FFE_A_LUT_7_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_DP_FFE_A_LUT_7_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_DP_FFE_A_LUT_6_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_DP_FFE_A_LUT_6_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_DP_FFE_A_LUT_6_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_DP_FFE_A_LUT_6_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000c5c MTSB_FSM_GLOBAL_CFG_8 |
 | +-------------------------------------------+
 | | bit  29:24 R/W  TARGET_AMP_PRELOAD_GEN3   |
 | | bit  23:18 R/W  TARGET_AMP_PRELOAD_GEN2   |
 | | bit  17:12 R/W  TARGET_AMP_PRELOAD_GEN1   |
 | | bit  11:8  R/W  ADC_MAP_PRELOAD_PCBI_GEN3 |
 | | bit  7:4   R/W  ADC_MAP_PRELOAD_PCBI_GEN2 |
 | | bit  3:0   R/W  ADC_MAP_PRELOAD_PCBI_GEN1 |
 | +-------------------------------------------+
 | 
 | void   c8_ctrl_field_TARGET_AMP_PRELOAD_GEN3_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_TARGET_AMP_PRELOAD_GEN3_get         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_TARGET_AMP_PRELOAD_GEN3_set   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_TARGET_AMP_PRELOAD_GEN3_get   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_TARGET_AMP_PRELOAD_GEN2_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_TARGET_AMP_PRELOAD_GEN2_get         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_TARGET_AMP_PRELOAD_GEN2_set   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_TARGET_AMP_PRELOAD_GEN2_get   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_TARGET_AMP_PRELOAD_GEN1_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_TARGET_AMP_PRELOAD_GEN1_get         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_TARGET_AMP_PRELOAD_GEN1_set   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_TARGET_AMP_PRELOAD_GEN1_get   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADC_MAP_PRELOAD_PCBI_GEN3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADC_MAP_PRELOAD_PCBI_GEN3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADC_MAP_PRELOAD_PCBI_GEN3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADC_MAP_PRELOAD_PCBI_GEN3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADC_MAP_PRELOAD_PCBI_GEN2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADC_MAP_PRELOAD_PCBI_GEN2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADC_MAP_PRELOAD_PCBI_GEN2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADC_MAP_PRELOAD_PCBI_GEN2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADC_MAP_PRELOAD_PCBI_GEN1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADC_MAP_PRELOAD_PCBI_GEN1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADC_MAP_PRELOAD_PCBI_GEN1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADC_MAP_PRELOAD_PCBI_GEN1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000c60 MTSB_FSM_GLOBAL_CFG_9 |
 | +-------------------------------------------+
 | | bit  23:20 R/W  ADC_MAP_SCALE_LUT_5       |
 | | bit  19:16 R/W  ADC_MAP_SCALE_LUT_4       |
 | | bit  15:12 R/W  ADC_MAP_SCALE_LUT_3       |
 | | bit  11:8  R/W  ADC_MAP_SCALE_LUT_2       |
 | | bit  7:4   R/W  ADC_MAP_SCALE_LUT_1       |
 | | bit  3:0   R/W  ADC_MAP_SCALE_LUT_0       |
 | +-------------------------------------------+
 | 
 | void   c8_ctrl_field_ADC_MAP_SCALE_LUT_5_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADC_MAP_SCALE_LUT_5_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADC_MAP_SCALE_LUT_5_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADC_MAP_SCALE_LUT_5_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADC_MAP_SCALE_LUT_4_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADC_MAP_SCALE_LUT_4_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADC_MAP_SCALE_LUT_4_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADC_MAP_SCALE_LUT_4_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADC_MAP_SCALE_LUT_3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADC_MAP_SCALE_LUT_3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADC_MAP_SCALE_LUT_3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADC_MAP_SCALE_LUT_3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADC_MAP_SCALE_LUT_2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADC_MAP_SCALE_LUT_2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADC_MAP_SCALE_LUT_2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADC_MAP_SCALE_LUT_2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADC_MAP_SCALE_LUT_1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADC_MAP_SCALE_LUT_1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADC_MAP_SCALE_LUT_1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADC_MAP_SCALE_LUT_1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADC_MAP_SCALE_LUT_0_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADC_MAP_SCALE_LUT_0_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADC_MAP_SCALE_LUT_0_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADC_MAP_SCALE_LUT_0_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000c64 MTSB_FSM_GLOBAL_CFG_10 |
 | +--------------------------------------------+
 | | bit  26:24 R/W  CLIP_RATE_THRESH_GEN3      |
 | | bit  23:21 R/W  CLIP_RATE_THRESH_GEN2      |
 | | bit  20:18 R/W  CLIP_RATE_THRESH_GEN1      |
 | | bit  17:12 R/W  CLIP_RATE_AMP_GEN3         |
 | | bit  11:6  R/W  CLIP_RATE_AMP_GEN2         |
 | | bit  5:0   R/W  CLIP_RATE_AMP_GEN1         |
 | +--------------------------------------------+
 | 
 | void   c8_ctrl_field_CLIP_RATE_THRESH_GEN3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_CLIP_RATE_THRESH_GEN3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_CLIP_RATE_THRESH_GEN3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_CLIP_RATE_THRESH_GEN3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_CLIP_RATE_THRESH_GEN2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_CLIP_RATE_THRESH_GEN2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_CLIP_RATE_THRESH_GEN2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_CLIP_RATE_THRESH_GEN2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_CLIP_RATE_THRESH_GEN1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_CLIP_RATE_THRESH_GEN1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_CLIP_RATE_THRESH_GEN1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_CLIP_RATE_THRESH_GEN1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_CLIP_RATE_AMP_GEN3_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_CLIP_RATE_AMP_GEN3_get          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_CLIP_RATE_AMP_GEN3_set    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_CLIP_RATE_AMP_GEN3_get    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_CLIP_RATE_AMP_GEN2_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_CLIP_RATE_AMP_GEN2_get          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_CLIP_RATE_AMP_GEN2_set    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_CLIP_RATE_AMP_GEN2_get    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_CLIP_RATE_AMP_GEN1_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_CLIP_RATE_AMP_GEN1_get          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_CLIP_RATE_AMP_GEN1_set    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_CLIP_RATE_AMP_GEN1_get    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000c68 MTSB_FSM_GLOBAL_CFG_11 |
 | +--------------------------------------------+
 | | bit  5 R/W  PATH_PATTERN_ENB_GEN3_P3       |
 | | bit  4 R/W  PATH_PATTERN_ENB_GEN3_P0T2     |
 | | bit  3 R/W  PATH_PATTERN_ENB_GEN2_P3       |
 | | bit  2 R/W  PATH_PATTERN_ENB_GEN2_P0T2     |
 | | bit  1 R/W  PATH_PATTERN_ENB_GEN1_P3       |
 | | bit  0 R/W  PATH_PATTERN_ENB_GEN1_P0T2     |
 | +--------------------------------------------+
 | 
 | void   c8_ctrl_field_PATH_PATTERN_ENB_GEN3_P3_set   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PATH_PATTERN_ENB_GEN3_P3_get   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_PATH_PATTERN_ENB_GEN3_P0T2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PATH_PATTERN_ENB_GEN3_P0T2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_PATH_PATTERN_ENB_GEN2_P3_set   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PATH_PATTERN_ENB_GEN2_P3_get   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_PATH_PATTERN_ENB_GEN2_P0T2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PATH_PATTERN_ENB_GEN2_P0T2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_PATH_PATTERN_ENB_GEN1_P3_set   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PATH_PATTERN_ENB_GEN1_P3_get   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_PATH_PATTERN_ENB_GEN1_P0T2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PATH_PATTERN_ENB_GEN1_P0T2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------+
 | | Register 0x00000c6c MTSB_FSM_GLOBAL_CFG_12 |
 | +--------------------------------------------+
 | | bit  15:12 R/W  ADAPT_LOOPS_GEN1_3         |
 | | bit  11:8  R/W  ADAPT_LOOPS_GEN1_2         |
 | | bit  7:4   R/W  ADAPT_LOOPS_GEN1_1         |
 | | bit  3:0   R/W  ADAPT_LOOPS_GEN1_0         |
 | +--------------------------------------------+
 | 
 | void   c8_ctrl_field_ADAPT_LOOPS_GEN1_3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_LOOPS_GEN1_3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_LOOPS_GEN1_3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_LOOPS_GEN1_3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_LOOPS_GEN1_2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_LOOPS_GEN1_2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_LOOPS_GEN1_2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_LOOPS_GEN1_2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_LOOPS_GEN1_1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_LOOPS_GEN1_1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_LOOPS_GEN1_1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_LOOPS_GEN1_1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_LOOPS_GEN1_0_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_LOOPS_GEN1_0_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_LOOPS_GEN1_0_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_LOOPS_GEN1_0_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000c70 MTSB_FSM_GLOBAL_CFG_13 |
 | +--------------------------------------------+
 | | bit  15:12 R/W  ADAPT_LOOPS_GEN2_3         |
 | | bit  11:8  R/W  ADAPT_LOOPS_GEN2_2         |
 | | bit  7:4   R/W  ADAPT_LOOPS_GEN2_1         |
 | | bit  3:0   R/W  ADAPT_LOOPS_GEN2_0         |
 | +--------------------------------------------+
 | 
 | void   c8_ctrl_field_ADAPT_LOOPS_GEN2_3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_LOOPS_GEN2_3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_LOOPS_GEN2_3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_LOOPS_GEN2_3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_LOOPS_GEN2_2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_LOOPS_GEN2_2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_LOOPS_GEN2_2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_LOOPS_GEN2_2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_LOOPS_GEN2_1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_LOOPS_GEN2_1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_LOOPS_GEN2_1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_LOOPS_GEN2_1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_LOOPS_GEN2_0_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_LOOPS_GEN2_0_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_LOOPS_GEN2_0_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_LOOPS_GEN2_0_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000c74 MTSB_FSM_GLOBAL_CFG_14 |
 | +--------------------------------------------+
 | | bit  15:12 R/W  ADAPT_LOOPS_GEN3_3         |
 | | bit  11:8  R/W  ADAPT_LOOPS_GEN3_2         |
 | | bit  7:4   R/W  ADAPT_LOOPS_GEN3_1         |
 | | bit  3:0   R/W  ADAPT_LOOPS_GEN3_0         |
 | +--------------------------------------------+
 | 
 | void   c8_ctrl_field_ADAPT_LOOPS_GEN3_3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_LOOPS_GEN3_3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_LOOPS_GEN3_3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_LOOPS_GEN3_3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_LOOPS_GEN3_2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_LOOPS_GEN3_2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_LOOPS_GEN3_2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_LOOPS_GEN3_2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_LOOPS_GEN3_1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_LOOPS_GEN3_1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_LOOPS_GEN3_1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_LOOPS_GEN3_1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_LOOPS_GEN3_0_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_LOOPS_GEN3_0_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_LOOPS_GEN3_0_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_LOOPS_GEN3_0_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000c78 MTSB_FSM_GLOBAL_CFG_15 |
 | +--------------------------------------------+
 | | bit  23:21 R/W  OBJECTS_EN_GEN2_PASS3      |
 | | bit  20:18 R/W  OBJECTS_EN_GEN2_PASS2      |
 | | bit  17:15 R/W  OBJECTS_EN_GEN2_PASS1      |
 | | bit  14:12 R/W  OBJECTS_EN_GEN2_PASS0      |
 | | bit  11:9  R/W  OBJECTS_EN_GEN1_PASS3      |
 | | bit  8:6   R/W  OBJECTS_EN_GEN1_PASS2      |
 | | bit  5:3   R/W  OBJECTS_EN_GEN1_PASS1      |
 | | bit  2:0   R/W  OBJECTS_EN_GEN1_PASS0      |
 | +--------------------------------------------+
 | 
 | void   c8_ctrl_field_OBJECTS_EN_GEN2_PASS3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_OBJECTS_EN_GEN2_PASS3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_OBJECTS_EN_GEN2_PASS3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_OBJECTS_EN_GEN2_PASS3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_OBJECTS_EN_GEN2_PASS2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_OBJECTS_EN_GEN2_PASS2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_OBJECTS_EN_GEN2_PASS2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_OBJECTS_EN_GEN2_PASS2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_OBJECTS_EN_GEN2_PASS1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_OBJECTS_EN_GEN2_PASS1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_OBJECTS_EN_GEN2_PASS1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_OBJECTS_EN_GEN2_PASS1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_OBJECTS_EN_GEN2_PASS0_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_OBJECTS_EN_GEN2_PASS0_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_OBJECTS_EN_GEN2_PASS0_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_OBJECTS_EN_GEN2_PASS0_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_OBJECTS_EN_GEN1_PASS3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_OBJECTS_EN_GEN1_PASS3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_OBJECTS_EN_GEN1_PASS3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_OBJECTS_EN_GEN1_PASS3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_OBJECTS_EN_GEN1_PASS2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_OBJECTS_EN_GEN1_PASS2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_OBJECTS_EN_GEN1_PASS2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_OBJECTS_EN_GEN1_PASS2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_OBJECTS_EN_GEN1_PASS1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_OBJECTS_EN_GEN1_PASS1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_OBJECTS_EN_GEN1_PASS1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_OBJECTS_EN_GEN1_PASS1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_OBJECTS_EN_GEN1_PASS0_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_OBJECTS_EN_GEN1_PASS0_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_OBJECTS_EN_GEN1_PASS0_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_OBJECTS_EN_GEN1_PASS0_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000c7c MTSB_FSM_GLOBAL_CFG_16 |
 | +--------------------------------------------+
 | | bit  11:9 R/W  OBJECTS_EN_GEN3_PASS3       |
 | | bit  8:6  R/W  OBJECTS_EN_GEN3_PASS2       |
 | | bit  5:3  R/W  OBJECTS_EN_GEN3_PASS1       |
 | | bit  2:0  R/W  OBJECTS_EN_GEN3_PASS0       |
 | +--------------------------------------------+
 | 
 | void   c8_ctrl_field_OBJECTS_EN_GEN3_PASS3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_OBJECTS_EN_GEN3_PASS3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_OBJECTS_EN_GEN3_PASS3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_OBJECTS_EN_GEN3_PASS3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_OBJECTS_EN_GEN3_PASS2_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_OBJECTS_EN_GEN3_PASS2_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_OBJECTS_EN_GEN3_PASS2_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_OBJECTS_EN_GEN3_PASS2_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_OBJECTS_EN_GEN3_PASS1_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_OBJECTS_EN_GEN3_PASS1_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_OBJECTS_EN_GEN3_PASS1_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_OBJECTS_EN_GEN3_PASS1_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_OBJECTS_EN_GEN3_PASS0_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_OBJECTS_EN_GEN3_PASS0_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_OBJECTS_EN_GEN3_PASS0_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_OBJECTS_EN_GEN3_PASS0_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000c80 MTSB_FSM_GLOBAL_CFG_17 |
 | +--------------------------------------------+
 | | bit  7:3 R/W  OFL_DECAY                    |
 | | bit  2   R/W  SEL_LMS_CLIP                 |
 | | bit  1   R/W  PRELOAD_ENB_P1T3             |
 | | bit  0   R/W  PRELOAD_ENB_P0               |
 | +--------------------------------------------+
 | 
 | void   c8_ctrl_field_OFL_DECAY_set        ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_OFL_DECAY_get        ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_OFL_DECAY_set  ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_OFL_DECAY_get  ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_SEL_LMS_CLIP_set     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_SEL_LMS_CLIP_get     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_PRELOAD_ENB_P1T3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PRELOAD_ENB_P1T3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_PRELOAD_ENB_P0_set   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_PRELOAD_ENB_P0_get   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------+
 | | Register 0x00000c84 MTSB_FSM_GLOBAL_CFG_18 |
 | +--------------------------------------------+
 | | bit  3   R/W  RX_ADAPT_DONE_OVR            |
 | | bit  2:1 R/W  GEN_RATE_OVR                 |
 | | bit  0   R/W  RX_ADAPT_START_OVR           |
 | +--------------------------------------------+
 | 
 | void   c8_ctrl_field_RX_ADAPT_DONE_OVR_set  ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_RX_ADAPT_DONE_OVR_get  ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_GEN_RATE_OVR_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_GEN_RATE_OVR_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_GEN_RATE_OVR_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_GEN_RATE_OVR_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_RX_ADAPT_START_OVR_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_RX_ADAPT_START_OVR_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------+
 | | Register 0x00000c88 MTSB_FSM_GLOBAL_CFG_19 |
 | +--------------------------------------------+
 | | bit  17:15 R/W  ADAPT_DELAY_GEN3_PASS1_3   |
 | | bit  14:12 R/W  ADAPT_DELAY_GEN3_PASS0     |
 | | bit  11:9  R/W  ADAPT_DELAY_GEN2_PASS1_3   |
 | | bit  8:6   R/W  ADAPT_DELAY_GEN2_PASS0     |
 | | bit  5:3   R/W  ADAPT_DELAY_GEN1_PASS1_3   |
 | | bit  2:0   R/W  ADAPT_DELAY_GEN1_PASS0     |
 | +--------------------------------------------+
 | 
 | void   c8_ctrl_field_ADAPT_DELAY_GEN3_PASS1_3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_DELAY_GEN3_PASS1_3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_DELAY_GEN3_PASS1_3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_DELAY_GEN3_PASS1_3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_DELAY_GEN3_PASS0_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_DELAY_GEN3_PASS0_get         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_DELAY_GEN3_PASS0_set   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_DELAY_GEN3_PASS0_get   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_DELAY_GEN2_PASS1_3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_DELAY_GEN2_PASS1_3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_DELAY_GEN2_PASS1_3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_DELAY_GEN2_PASS1_3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_DELAY_GEN2_PASS0_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_DELAY_GEN2_PASS0_get         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_DELAY_GEN2_PASS0_set   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_DELAY_GEN2_PASS0_get   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_DELAY_GEN1_PASS1_3_set       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_DELAY_GEN1_PASS1_3_get       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_DELAY_GEN1_PASS1_3_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_DELAY_GEN1_PASS1_3_get ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_ctrl_field_ADAPT_DELAY_GEN1_PASS0_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_ctrl_field_ADAPT_DELAY_GEN1_PASS0_get         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_ctrl_field_range_ADAPT_DELAY_GEN1_PASS0_set   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_ctrl_field_range_ADAPT_DELAY_GEN1_PASS0_get   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _C8_CTRL_IO_H
#define _C8_CTRL_IO_H

#include "c8.h"
#include "c8_ctrl_regs.h"
#include "c8_ctrl_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 c8_ctrl_buffer_init               ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void                 c8_ctrl_buffer_flush              ( c8_ctrl_buffer_t *b_ptr )
 | UINT32               c8_ctrl_reg_read                  ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 c8_ctrl_reg_write                 ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 c8_ctrl_field_set                 ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 c8_ctrl_action_on_write_field_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 c8_ctrl_burst_read                ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 c8_ctrl_burst_write               ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE c8_ctrl_poll                      ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 c8_ctrl_reg_MTSB_FSM_LANE_CFG_1_write                ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_LANE_CFG_1_field_set            ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_LANE_CFG_1_read                 ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_LANE_CFG_2_write                ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_LANE_CFG_2_field_set            ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_LANE_CFG_2_read                 ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_1_write              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_1_field_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_1_read               ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_2_write              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_2_field_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_2_read               ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_3_write              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_3_field_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_3_read               ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_4_write              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_4_field_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_4_read               ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_5_write              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_5_field_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_5_read               ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_6_write              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_6_field_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_6_read               ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_7_write              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_7_field_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_7_read               ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_8_write              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_8_field_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_8_read               ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_9_write              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_9_field_set          ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_9_read               ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_10_write             ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_10_field_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_10_read              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_11_write             ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_11_field_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_11_read              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_12_write             ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_12_field_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_12_read              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_13_write             ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_13_field_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_13_read              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_14_write             ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_14_field_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_14_read              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_15_write             ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_15_field_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_15_read              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_16_write             ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_16_field_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_16_read              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_17_write             ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_17_field_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_17_read              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_18_write             ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_18_field_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_18_read              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_19_write             ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_19_field_set         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_GLOBAL_CFG_19_read              ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_MTSB_FSM_LANE_OBSERVABILITY__write       ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_MTSB_FSM_LANE_OBSERVABILITY__field_set   ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_MTSB_FSM_LANE_OBSERVABILITY__read        ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_ctrl_reg_STAT_INT_EVENT_write                     ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_STAT_INT_EVENT_action_on_write_field_set ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_STAT_INT_EVENT_read                      ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_ctrl_reg_STAT_INT_EVENT_poll                      ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 c8_ctrl_reg_STAT_INT_EN_write                        ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_ctrl_reg_STAT_INT_EN_field_set                    ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_ctrl_reg_STAT_INT_EN_read                         ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               c8_ctrl_reg_STAT_INT_read                            ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_ctrl_reg_STAT_INT_poll                            ( c8_ctrl_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _C8_CTRL_IO_H */
