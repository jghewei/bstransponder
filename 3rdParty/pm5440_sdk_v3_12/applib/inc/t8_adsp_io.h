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
 *     and register accessor functions for the t8_adsp block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing t8_adsp io functions is:
 | {
 |     t8_adsp_buffer_t b_ptr[1];
 |     t8_adsp_buffer_init( b_ptr, h_ptr, id );
 |     value = t8_adsp_field_<FIELD>_get( b_ptr, h_ptr, id );
 |     t8_adsp_field_<FIELD>_set( b_ptr, h_ptr, id , value );
 | ...
 |     t8_adsp_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = t8_adsp_field_<FIELD>_get( NULL, h_ptr, id );
 | t8_adsp_field_<FIELD>_set( NULL, h_ptr, id , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------------+
 | | Register 0x00000500 ADAPT_DSP_CLK_CTRL |
 | +----------------------------------------+
 | | bit  0 R/W  ADAPT_CLK_ENB              |
 | +----------------------------------------+
 | 
 | void   t8_adsp_field_ADAPT_CLK_ENB_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_ADAPT_CLK_ENB_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +----------------------------------+
 | | Register 0x00000504 ADAPT_CTRL_1 |
 | +----------------------------------+
 | | bit  17   R/W  EQ_ADAPT_CTRL_ENB |
 | | bit  16   R/W  PCIE_FFE_MODE     |
 | | bit  15   R/W  INV_DFE_OUT       |
 | | bit  14   R/W  ADAPT_INITB       |
 | | bit  13   R/W  DP_ADAPT_ORDER    |
 | | bit  12   R/W  FFE_CONSTRAINED   |
 | | bit  11   R/W  PRELOAD_ENB       |
 | | bit  10:8 R/W  ADAPT_DELAY       |
 | | bit  7:4  R/W  ADAPT_LOOPS       |
 | | bit  3    R/W  ADAPT_ENB         |
 | | bit  1:0  R/W  SEL_LMS_CLIP      |
 | +----------------------------------+
 | 
 | void   t8_adsp_field_EQ_ADAPT_CTRL_ENB_set  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_EQ_ADAPT_CTRL_ENB_get  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_PCIE_FFE_MODE_set      ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_PCIE_FFE_MODE_get      ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_INV_DFE_OUT_set        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_INV_DFE_OUT_get        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_ADAPT_INITB_set        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_ADAPT_INITB_get        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_DP_ADAPT_ORDER_set     ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DP_ADAPT_ORDER_get     ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_FFE_CONSTRAINED_set    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_FFE_CONSTRAINED_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_PRELOAD_ENB_set        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_PRELOAD_ENB_get        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_ADAPT_DELAY_set        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_ADAPT_DELAY_get        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_ADAPT_DELAY_set  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_ADAPT_DELAY_get  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_ADAPT_LOOPS_set        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_ADAPT_LOOPS_get        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_ADAPT_LOOPS_set  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_ADAPT_LOOPS_get  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_ADAPT_ENB_set          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_ADAPT_ENB_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_SEL_LMS_CLIP_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_SEL_LMS_CLIP_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_SEL_LMS_CLIP_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_SEL_LMS_CLIP_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00000508 ADAPT_CTRL_2 |
 | +----------------------------------+
 | | bit  29:15 R/W  OBJECTS_EN       |
 | | bit  14:0  R/W  LMS_DATA_SEL     |
 | +----------------------------------+
 | 
 | void   t8_adsp_field_OBJECTS_EN_set         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_OBJECTS_EN_get         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_OBJECTS_EN_set   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_OBJECTS_EN_get   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_LMS_DATA_SEL_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_LMS_DATA_SEL_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_LMS_DATA_SEL_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_LMS_DATA_SEL_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x0000050c ADAPT_STAT              |
 | +---------------------------------------------+
 | | bit  11:1 R    ADAPT_STATUS                 |
 | | bit  0    R/W  CAPTURE_ADAPT_STATUS_REQUEST |
 | +---------------------------------------------+
 | 
 | void   t8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_CAPTURE_ADAPT_STATUS_REQUEST_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_ADAPT_STATUS_get                 ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_ADAPT_STATUS_get           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x00000510 ADAPT_RESULTS_CAPTURE_REQUEST |
 | +---------------------------------------------------+
 | | bit  0 R/W  CAPTURE_RESULTS_REQUEST               |
 | +---------------------------------------------------+
 | 
 | void   t8_adsp_field_CAPTURE_RESULTS_REQUEST_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_CAPTURE_RESULTS_REQUEST_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +---------------------------------------------+
 | | Register 0x00000514 ADAPT_RESULTS_CAPTURE_1 |
 | +---------------------------------------------+
 | | bit  30:26 R  TR_FFE_B                      |
 | | bit  25:21 R  TR_FFE_A                      |
 | | bit  20:14 R  DP_FFE_C                      |
 | | bit  13:7  R  DP_FFE_B                      |
 | | bit  6:0   R  DP_FFE_A                      |
 | +---------------------------------------------+
 | 
 | UINT32 t8_adsp_field_TR_FFE_B_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_TR_FFE_B_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_adsp_field_TR_FFE_A_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_TR_FFE_A_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_adsp_field_DP_FFE_C_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_DP_FFE_C_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_adsp_field_DP_FFE_B_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_DP_FFE_B_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_adsp_field_DP_FFE_A_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_DP_FFE_A_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000518 ADAPT_RESULTS_CAPTURE_2 |
 | +---------------------------------------------+
 | | bit  27:21 R  DFE_COEF3                     |
 | | bit  20:14 R  DFE_COEF2                     |
 | | bit  13:7  R  DFE_COEF1                     |
 | | bit  6:0   R  DFE_COEF0                     |
 | +---------------------------------------------+
 | 
 | UINT32 t8_adsp_field_DFE_COEF3_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_DFE_COEF3_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_adsp_field_DFE_COEF2_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_DFE_COEF2_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_adsp_field_DFE_COEF1_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_DFE_COEF1_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_adsp_field_DFE_COEF0_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_DFE_COEF0_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x0000051c ADAPT_RESULTS_CAPTURE_3 |
 | +---------------------------------------------+
 | | bit  13:7 R  DFE_COEF5                      |
 | | bit  6:0  R  DFE_COEF4                      |
 | +---------------------------------------------+
 | 
 | UINT32 t8_adsp_field_DFE_COEF5_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_DFE_COEF5_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_adsp_field_DFE_COEF4_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_DFE_COEF4_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000520 ADAPT_RESULTS_CAPTURE_4 |
 | +---------------------------------------------+
 | | bit  22:19 R  ADC_MAP_SCALE                 |
 | | bit  18:13 R  TARGET_AMP                    |
 | | bit  12:5  R  PGA_OFFSET                    |
 | | bit  4:0   R  PGA_GAIN                      |
 | +---------------------------------------------+
 | 
 | UINT32 t8_adsp_field_ADC_MAP_SCALE_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_ADC_MAP_SCALE_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_adsp_field_TARGET_AMP_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_TARGET_AMP_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_adsp_field_PGA_OFFSET_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_PGA_OFFSET_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_adsp_field_PGA_GAIN_get            ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_PGA_GAIN_get      ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000524 STAT_INT_EN |
 | +---------------------------------+
 | | bit  9 R/W  PROG_REF_DONE_E     |
 | | bit  8 R/W  PROG_REF_TIMEOUT_E  |
 | | bit  0 R/W  STOP_ADAPT_E        |
 | +---------------------------------+
 | 
 | void   t8_adsp_field_PROG_REF_DONE_E_set    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_PROG_REF_DONE_E_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_PROG_REF_TIMEOUT_E_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_PROG_REF_TIMEOUT_E_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_STOP_ADAPT_E_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_STOP_ADAPT_E_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +------------------------------------+
 | | Register 0x00000528 STAT_INT_EVENT |
 | +------------------------------------+
 | | bit  9 R/W  PROG_REF_DONE_I        |
 | | bit  8 R/W  PROG_REF_TIMEOUT_I     |
 | | bit  0 R/W  STOP_ADAPT_I           |
 | +------------------------------------+
 | 
 | void                 t8_adsp_field_PROG_REF_DONE_I_set_to_clear    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_adsp_field_PROG_REF_DONE_I_get             ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_adsp_field_PROG_REF_DONE_I_poll            ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_adsp_field_PROG_REF_TIMEOUT_I_set_to_clear ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_adsp_field_PROG_REF_TIMEOUT_I_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_adsp_field_PROG_REF_TIMEOUT_I_poll         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_adsp_field_STOP_ADAPT_I_set_to_clear       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_adsp_field_STOP_ADAPT_I_get                ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_adsp_field_STOP_ADAPT_I_poll               ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------+
 | | Register 0x0000052c STAT_INT |
 | +------------------------------+
 | | bit  9 R  PROG_REF_DONE_V    |
 | | bit  8 R  PROG_REF_TIMEOUT_V |
 | | bit  0 R  STOP_ADAPT_V       |
 | +------------------------------+
 | 
 | UINT32               t8_adsp_field_PROG_REF_DONE_V_get     ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_adsp_field_PROG_REF_DONE_V_poll    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_adsp_field_PROG_REF_TIMEOUT_V_get  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_adsp_field_PROG_REF_TIMEOUT_V_poll ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_adsp_field_STOP_ADAPT_V_get        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_adsp_field_STOP_ADAPT_V_poll       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------+
 | | Register 0x00000530 DSS_CTRL         |
 | +--------------------------------------+
 | | bit  29:27 R/W  EXPECTED_EQ2         |
 | | bit  26:24 R/W  EXPECTED_EQ1         |
 | | bit  22:16 R/W  H_LIMIT              |
 | | bit  15:9  R/W  S_LIMIT              |
 | | bit  7:3   R/W  OFL_DECAY            |
 | | bit  1     R/W  PATH_PATTERN_CHK_ENB |
 | | bit  0     R/W  DSS_ENB              |
 | +--------------------------------------+
 | 
 | void   t8_adsp_field_EXPECTED_EQ2_set         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_EXPECTED_EQ2_get         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_EXPECTED_EQ2_set   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_EXPECTED_EQ2_get   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_EXPECTED_EQ1_set         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_EXPECTED_EQ1_get         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_EXPECTED_EQ1_set   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_EXPECTED_EQ1_get   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_H_LIMIT_set              ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_H_LIMIT_get              ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_H_LIMIT_set        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_H_LIMIT_get        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_S_LIMIT_set              ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_S_LIMIT_get              ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_S_LIMIT_set        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_S_LIMIT_get        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_OFL_DECAY_set            ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_OFL_DECAY_get            ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_OFL_DECAY_set      ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_OFL_DECAY_get      ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_PATH_PATTERN_CHK_ENB_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_PATH_PATTERN_CHK_ENB_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_DSS_ENB_set              ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DSS_ENB_get              ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +----------------------------------------+
 | | Register 0x00000534 DSS_TAPS_1         |
 | +----------------------------------------+
 | | bit  30    R/W  DFE_COEF5_TAP_OFL1_EN  |
 | | bit  29    R/W  DFE_COEF4_TAP_OFL1_EN  |
 | | bit  28    R/W  DFE_COEF3_TAP_OFL1_EN  |
 | | bit  27    R/W  DFE_COEF2_TAP_OFL1_EN  |
 | | bit  26    R/W  DFE_COEF1_TAP_OFL1_EN  |
 | | bit  25    R/W  DFE_COEF0_TAP_OFL1_EN  |
 | | bit  24    R/W  TARGET_AMP_TAP_OFL1_EN |
 | | bit  23:20 R/W  DFE_COEF5_TAP          |
 | | bit  19:16 R/W  DFE_COEF4_TAP          |
 | | bit  15:12 R/W  DFE_COEF3_TAP          |
 | | bit  11:8  R/W  DFE_COEF2_TAP          |
 | | bit  7:4   R/W  DFE_COEF1_TAP          |
 | | bit  3:0   R/W  DFE_COEF0_TAP          |
 | +----------------------------------------+
 | 
 | void   t8_adsp_field_DFE_COEF5_TAP_OFL1_EN_set  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF5_TAP_OFL1_EN_get  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_DFE_COEF4_TAP_OFL1_EN_set  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF4_TAP_OFL1_EN_get  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_DFE_COEF3_TAP_OFL1_EN_set  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF3_TAP_OFL1_EN_get  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_DFE_COEF2_TAP_OFL1_EN_set  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF2_TAP_OFL1_EN_get  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_DFE_COEF1_TAP_OFL1_EN_set  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF1_TAP_OFL1_EN_get  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_DFE_COEF0_TAP_OFL1_EN_set  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF0_TAP_OFL1_EN_get  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_TARGET_AMP_TAP_OFL1_EN_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_TARGET_AMP_TAP_OFL1_EN_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_DFE_COEF5_TAP_set          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF5_TAP_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DFE_COEF5_TAP_set    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DFE_COEF5_TAP_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DFE_COEF4_TAP_set          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF4_TAP_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DFE_COEF4_TAP_set    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DFE_COEF4_TAP_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DFE_COEF3_TAP_set          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF3_TAP_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DFE_COEF3_TAP_set    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DFE_COEF3_TAP_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DFE_COEF2_TAP_set          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF2_TAP_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DFE_COEF2_TAP_set    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DFE_COEF2_TAP_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DFE_COEF1_TAP_set          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF1_TAP_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DFE_COEF1_TAP_set    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DFE_COEF1_TAP_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DFE_COEF0_TAP_set          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF0_TAP_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DFE_COEF0_TAP_set    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DFE_COEF0_TAP_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x00000538 DSS_TAPS_2 |
 | +--------------------------------+
 | | bit  3:0 R/W  GAIN_TAP         |
 | +--------------------------------+
 | 
 | void   t8_adsp_field_GAIN_TAP_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_GAIN_TAP_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_GAIN_TAP_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_GAIN_TAP_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x0000053c DSS_TAPS_3       |
 | +--------------------------------------+
 | | bit  29:27 R/W  TAP2                 |
 | | bit  26    R/W  DP_FFE_C_TAP_OFL1_EN |
 | | bit  25    R/W  DP_FFE_B_TAP_OFL1_EN |
 | | bit  24    R/W  DP_FFE_A_TAP_OFL1_EN |
 | | bit  23:20 R/W  DP_FFE_C_TAP         |
 | | bit  19:16 R/W  DP_FFE_B_TAP         |
 | | bit  15:12 R/W  DP_FFE_A_TAP         |
 | | bit  11:8  R/W  TARGET_AMP_TAP       |
 | | bit  7:4   R/W  ADC_TAP              |
 | | bit  3:0   R/W  OFFSET_TAP           |
 | +--------------------------------------+
 | 
 | void   t8_adsp_field_TAP2_set                 ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_TAP2_get                 ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_TAP2_set           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_TAP2_get           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DP_FFE_C_TAP_OFL1_EN_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DP_FFE_C_TAP_OFL1_EN_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_DP_FFE_B_TAP_OFL1_EN_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DP_FFE_B_TAP_OFL1_EN_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_DP_FFE_A_TAP_OFL1_EN_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DP_FFE_A_TAP_OFL1_EN_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_DP_FFE_C_TAP_set         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DP_FFE_C_TAP_get         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DP_FFE_C_TAP_set   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DP_FFE_C_TAP_get   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DP_FFE_B_TAP_set         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DP_FFE_B_TAP_get         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DP_FFE_B_TAP_set   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DP_FFE_B_TAP_get   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DP_FFE_A_TAP_set         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DP_FFE_A_TAP_get         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DP_FFE_A_TAP_set   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DP_FFE_A_TAP_get   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_TARGET_AMP_TAP_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_TARGET_AMP_TAP_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_TARGET_AMP_TAP_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_TARGET_AMP_TAP_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_ADC_TAP_set              ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_ADC_TAP_get              ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_ADC_TAP_set        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_ADC_TAP_get        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_OFFSET_TAP_set           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_OFFSET_TAP_get           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_OFFSET_TAP_set     ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_OFFSET_TAP_get     ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00000540 LMS_MU_FACTORS |
 | +------------------------------------+
 | | bit  31:28 R/W  MU_LEAK_ADC        |
 | | bit  27:24 R/W  MU_LEAK_COEF       |
 | | bit  23:20 R/W  MU_LEAK_OFFSET     |
 | | bit  19:16 R/W  MU_LEAK_GAIN       |
 | | bit  11:9  R/W  MU_STEP_ADC        |
 | | bit  8:6   R/W  MU_STEP_COEF       |
 | | bit  5:3   R/W  MU_STEP_OFFSET     |
 | | bit  2:0   R/W  MU_STEP_GAIN       |
 | +------------------------------------+
 | 
 | void   t8_adsp_field_MU_LEAK_ADC_set          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_MU_LEAK_ADC_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_MU_LEAK_ADC_set    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_MU_LEAK_ADC_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_MU_LEAK_COEF_set         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_MU_LEAK_COEF_get         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_MU_LEAK_COEF_set   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_MU_LEAK_COEF_get   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_MU_LEAK_OFFSET_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_MU_LEAK_OFFSET_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_MU_LEAK_OFFSET_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_MU_LEAK_OFFSET_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_MU_LEAK_GAIN_set         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_MU_LEAK_GAIN_get         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_MU_LEAK_GAIN_set   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_MU_LEAK_GAIN_get   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_MU_STEP_ADC_set          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_MU_STEP_ADC_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_MU_STEP_ADC_set    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_MU_STEP_ADC_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_MU_STEP_COEF_set         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_MU_STEP_COEF_get         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_MU_STEP_COEF_set   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_MU_STEP_COEF_get   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_MU_STEP_OFFSET_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_MU_STEP_OFFSET_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_MU_STEP_OFFSET_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_MU_STEP_OFFSET_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_MU_STEP_GAIN_set         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_MU_STEP_GAIN_get         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_MU_STEP_GAIN_set   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_MU_STEP_GAIN_get   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000544 ROUND_ROBIN_PERIODS |
 | +-----------------------------------------+
 | | bit  19:16 R/W  RR_PERIOD_DSS           |
 | | bit  15:12 R/W  RR_PERIOD_ADC_MAP       |
 | | bit  11:8  R/W  RR_PERIOD_OFFSET        |
 | | bit  7:4   R/W  RR_PERIOD_GAIN          |
 | | bit  3:0   R/W  RR_PERIOD_COEF          |
 | +-----------------------------------------+
 | 
 | void   t8_adsp_field_RR_PERIOD_DSS_set           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_RR_PERIOD_DSS_get           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_RR_PERIOD_DSS_set     ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_RR_PERIOD_DSS_get     ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_RR_PERIOD_ADC_MAP_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_RR_PERIOD_ADC_MAP_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_RR_PERIOD_ADC_MAP_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_RR_PERIOD_ADC_MAP_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_RR_PERIOD_OFFSET_set        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_RR_PERIOD_OFFSET_get        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_RR_PERIOD_OFFSET_set  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_RR_PERIOD_OFFSET_get  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_RR_PERIOD_GAIN_set          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_RR_PERIOD_GAIN_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_RR_PERIOD_GAIN_set    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_RR_PERIOD_GAIN_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_RR_PERIOD_COEF_set          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_RR_PERIOD_COEF_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_RR_PERIOD_COEF_set    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_RR_PERIOD_COEF_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000548 OBJECT_PRELOAD_VAL_1 |
 | +------------------------------------------+
 | | bit  30:26 R/W  TR_FFE_B_PRELOAD         |
 | | bit  25:21 R/W  TR_FFE_A_PRELOAD         |
 | | bit  20:14 R/W  DP_FFE_C_PRELOAD         |
 | | bit  13:7  R/W  DP_FFE_B_PRELOAD         |
 | | bit  6:0   R/W  DP_FFE_A_PRELOAD         |
 | +------------------------------------------+
 | 
 | void   t8_adsp_field_TR_FFE_B_PRELOAD_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_TR_FFE_B_PRELOAD_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_TR_FFE_B_PRELOAD_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_TR_FFE_B_PRELOAD_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_TR_FFE_A_PRELOAD_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_TR_FFE_A_PRELOAD_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_TR_FFE_A_PRELOAD_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_TR_FFE_A_PRELOAD_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DP_FFE_C_PRELOAD_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DP_FFE_C_PRELOAD_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DP_FFE_C_PRELOAD_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DP_FFE_C_PRELOAD_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DP_FFE_B_PRELOAD_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DP_FFE_B_PRELOAD_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DP_FFE_B_PRELOAD_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DP_FFE_B_PRELOAD_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DP_FFE_A_PRELOAD_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DP_FFE_A_PRELOAD_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DP_FFE_A_PRELOAD_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DP_FFE_A_PRELOAD_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0000054c OBJECT_PRELOAD_VAL_2 |
 | +------------------------------------------+
 | | bit  27:21 R/W  DFE_COEF3_PRELOAD        |
 | | bit  20:14 R/W  DFE_COEF2_PRELOAD        |
 | | bit  13:7  R/W  DFE_COEF1_PRELOAD        |
 | | bit  6:0   R/W  DFE_COEF0_PRELOAD        |
 | +------------------------------------------+
 | 
 | void   t8_adsp_field_DFE_COEF3_PRELOAD_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF3_PRELOAD_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DFE_COEF3_PRELOAD_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DFE_COEF3_PRELOAD_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DFE_COEF2_PRELOAD_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF2_PRELOAD_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DFE_COEF2_PRELOAD_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DFE_COEF2_PRELOAD_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DFE_COEF1_PRELOAD_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF1_PRELOAD_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DFE_COEF1_PRELOAD_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DFE_COEF1_PRELOAD_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DFE_COEF0_PRELOAD_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF0_PRELOAD_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DFE_COEF0_PRELOAD_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DFE_COEF0_PRELOAD_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000550 OBJECT_PRELOAD_VAL_3 |
 | +------------------------------------------+
 | | bit  13:7 R/W  DFE_COEF5_PRELOAD         |
 | | bit  6:0  R/W  DFE_COEF4_PRELOAD         |
 | +------------------------------------------+
 | 
 | void   t8_adsp_field_DFE_COEF5_PRELOAD_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF5_PRELOAD_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DFE_COEF5_PRELOAD_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DFE_COEF5_PRELOAD_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_DFE_COEF4_PRELOAD_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_DFE_COEF4_PRELOAD_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_DFE_COEF4_PRELOAD_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_DFE_COEF4_PRELOAD_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000554 OBJECT_PRELOAD_VAL_4 |
 | +------------------------------------------+
 | | bit  22:19 R/W  ADC_MAP_PRELOAD          |
 | | bit  18:13 R/W  TARGET_AMP_PRELOAD       |
 | | bit  12:5  R/W  PGA_OFFSET_PRELOAD       |
 | | bit  4:0   R/W  PGA_GAIN_PRELOAD         |
 | +------------------------------------------+
 | 
 | void   t8_adsp_field_ADC_MAP_PRELOAD_set          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_ADC_MAP_PRELOAD_get          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_ADC_MAP_PRELOAD_set    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_ADC_MAP_PRELOAD_get    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_TARGET_AMP_PRELOAD_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_TARGET_AMP_PRELOAD_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_TARGET_AMP_PRELOAD_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_TARGET_AMP_PRELOAD_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_PGA_OFFSET_PRELOAD_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_PGA_OFFSET_PRELOAD_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_PGA_OFFSET_PRELOAD_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_PGA_OFFSET_PRELOAD_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_PGA_GAIN_PRELOAD_set         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_PGA_GAIN_PRELOAD_get         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_PGA_GAIN_PRELOAD_set   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_PGA_GAIN_PRELOAD_get   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000558 CLIP_RATE_MON_PARAMS |
 | +------------------------------------------+
 | | bit  19    R/W  PROG_REF_OVR_EN          |
 | | bit  18    R/W  PROG_REF_OVR             |
 | | bit  17:14 R/W  PROG_REF_TIMEOUT_PERIOD  |
 | | bit  13:11 R/W  CLIP_RATE_THRESH         |
 | | bit  10:5  R/W  CLIP_RATE_AMP            |
 | | bit  4:2   R/W  GAIN_OFFSET              |
 | | bit  1:0   R/W  ADC_OFFSET               |
 | +------------------------------------------+
 | 
 | void   t8_adsp_field_PROG_REF_OVR_EN_set               ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_PROG_REF_OVR_EN_get               ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_PROG_REF_OVR_set                  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_PROG_REF_OVR_get                  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_PROG_REF_TIMEOUT_PERIOD_set       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_PROG_REF_TIMEOUT_PERIOD_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_PROG_REF_TIMEOUT_PERIOD_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_CLIP_RATE_THRESH_set              ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_CLIP_RATE_THRESH_get              ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_CLIP_RATE_THRESH_set        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_CLIP_RATE_THRESH_get        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_CLIP_RATE_AMP_set                 ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_CLIP_RATE_AMP_get                 ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_CLIP_RATE_AMP_set           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_CLIP_RATE_AMP_get           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_GAIN_OFFSET_set                   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_GAIN_OFFSET_get                   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_GAIN_OFFSET_set             ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_GAIN_OFFSET_get             ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_adsp_field_ADC_OFFSET_set                    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_ADC_OFFSET_get                    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_range_ADC_OFFSET_set              ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_adsp_field_range_ADC_OFFSET_get              ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x0000055c CLIP_RATE_MON_ADC_RESULTS |
 | +-----------------------------------------------+
 | | bit  9   R/W  CAPTURE_GAIN_CLIP_FLAG_REQUEST  |
 | | bit  7:2 R    CLIP_FLAG_ADC                   |
 | | bit  0   R/W  CAPTURE_ADC_CLIP_FLAGS_REQUEST  |
 | +-----------------------------------------------+
 | 
 | void   t8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_CAPTURE_GAIN_CLIP_FLAG_REQUEST_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_adsp_field_CAPTURE_ADC_CLIP_FLAGS_REQUEST_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_CLIP_FLAG_ADC_get                  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_CLIP_FLAG_ADC_get            ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000560 CLIP_RATE_MON_PGA_GAIN_RESULTS |
 | +----------------------------------------------------+
 | | bit  31:0 R  CLIP_FLAG_GAIN                        |
 | +----------------------------------------------------+
 | 
 | UINT32 t8_adsp_field_CLIP_FLAG_GAIN_get       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_adsp_field_range_CLIP_FLAG_GAIN_get ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _T8_ADSP_IO_H
#define _T8_ADSP_IO_H

#include "t8.h"
#include "t8_adsp_regs.h"
#include "t8_adsp_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 t8_adsp_buffer_init               ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void                 t8_adsp_buffer_flush              ( t8_adsp_buffer_t *b_ptr )
 | UINT32               t8_adsp_reg_read                  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mem_type, UINT32 reg )
 | void                 t8_adsp_reg_write                 ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 t8_adsp_field_set                 ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 t8_adsp_action_on_write_field_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 t8_adsp_burst_read                ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 t8_adsp_burst_write               ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE t8_adsp_poll                      ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 t8_adsp_reg_ADAPT_DSP_CLK_CTRL_write                 ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_ADAPT_DSP_CLK_CTRL_field_set             ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_ADAPT_DSP_CLK_CTRL_read                  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_ADAPT_CTRL_1_write                       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_ADAPT_CTRL_1_field_set                   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_ADAPT_CTRL_1_read                        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_ADAPT_CTRL_2_write                       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_ADAPT_CTRL_2_field_set                   ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_ADAPT_CTRL_2_read                        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_DSS_CTRL_write                           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_DSS_CTRL_field_set                       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_DSS_CTRL_read                            ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_DSS_TAPS_1_write                         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_DSS_TAPS_1_field_set                     ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_DSS_TAPS_1_read                          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_DSS_TAPS_2_write                         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_DSS_TAPS_2_field_set                     ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_DSS_TAPS_2_read                          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_DSS_TAPS_3_write                         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_DSS_TAPS_3_field_set                     ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_DSS_TAPS_3_read                          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_LMS_MU_FACTORS_write                     ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_LMS_MU_FACTORS_field_set                 ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_LMS_MU_FACTORS_read                      ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_ROUND_ROBIN_PERIODS_write                ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_ROUND_ROBIN_PERIODS_field_set            ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_ROUND_ROBIN_PERIODS_read                 ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_OBJECT_PRELOAD_VAL_1_write               ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_OBJECT_PRELOAD_VAL_1_field_set           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_OBJECT_PRELOAD_VAL_1_read                ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_OBJECT_PRELOAD_VAL_2_write               ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_OBJECT_PRELOAD_VAL_2_field_set           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_OBJECT_PRELOAD_VAL_2_read                ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_OBJECT_PRELOAD_VAL_3_write               ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_OBJECT_PRELOAD_VAL_3_field_set           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_OBJECT_PRELOAD_VAL_3_read                ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_OBJECT_PRELOAD_VAL_4_write               ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_OBJECT_PRELOAD_VAL_4_field_set           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_OBJECT_PRELOAD_VAL_4_read                ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_CLIP_RATE_MON_PARAMS_write               ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_CLIP_RATE_MON_PARAMS_field_set           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_CLIP_RATE_MON_PARAMS_read                ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_ADAPT_STAT_write                         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_ADAPT_STAT_field_set                     ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_ADAPT_STAT_read                          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_write      ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_field_set  ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_ADAPT_RESULTS_CAPTURE_REQUEST_read       ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_write          ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_field_set      ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_CLIP_RATE_MON_ADC_RESULTS_read           ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_adsp_reg_STAT_INT_EVENT_write                     ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_STAT_INT_EVENT_action_on_write_field_set ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_STAT_INT_EVENT_read                      ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_adsp_reg_STAT_INT_EVENT_poll                      ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 t8_adsp_reg_STAT_INT_EN_write                        ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_adsp_reg_STAT_INT_EN_field_set                    ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_adsp_reg_STAT_INT_EN_read                         ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | UINT32               t8_adsp_reg_STAT_INT_read                            ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_adsp_reg_STAT_INT_poll                            ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               t8_adsp_reg_ADAPT_RESULTS_CAPTURE_1_read             ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | UINT32               t8_adsp_reg_ADAPT_RESULTS_CAPTURE_2_read             ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | UINT32               t8_adsp_reg_ADAPT_RESULTS_CAPTURE_3_read             ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | UINT32               t8_adsp_reg_ADAPT_RESULTS_CAPTURE_4_read             ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | UINT32               t8_adsp_reg_CLIP_RATE_MON_PGA_GAIN_RESULTS_read      ( t8_adsp_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _T8_ADSP_IO_H */
