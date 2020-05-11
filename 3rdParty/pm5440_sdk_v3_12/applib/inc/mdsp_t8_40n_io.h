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
 *     and register accessor functions for the mdsp_t8_40n block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing mdsp_t8_40n io functions is:
 | {
 |     mdsp_t8_40n_buffer_t b_ptr[1];
 |     mdsp_t8_40n_buffer_init( b_ptr, h_ptr, A );
 |     value = mdsp_t8_40n_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     mdsp_t8_40n_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     mdsp_t8_40n_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = mdsp_t8_40n_field_<FIELD>_get( NULL, h_ptr, A );
 | mdsp_t8_40n_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------+
 | | Register 0x00040a00 CONFIG_1_LW20_80_15 |
 | +-----------------------------------------+
 | | bit  29:24 R/W  ADC_LEVELS_VEC_29_TO_24 |
 | | bit  23:18 R/W  ADC_LEVELS_VEC_23_TO_18 |
 | | bit  17:12 R/W  ADC_LEVELS_VEC_17_TO_12 |
 | | bit  11:6  R/W  ADC_LEVELS_VEC_11_TO_6  |
 | | bit  5:0   R/W  ADC_LEVELS_VEC_5_TO_0   |
 | +-----------------------------------------+
 | 
 | void   mdsp_t8_40n_field_ADC_LEVELS_VEC_29_TO_24_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_ADC_LEVELS_VEC_29_TO_24_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_ADC_LEVELS_VEC_29_TO_24_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_ADC_LEVELS_VEC_29_TO_24_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_ADC_LEVELS_VEC_23_TO_18_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_ADC_LEVELS_VEC_23_TO_18_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_ADC_LEVELS_VEC_23_TO_18_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_ADC_LEVELS_VEC_23_TO_18_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_ADC_LEVELS_VEC_17_TO_12_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_ADC_LEVELS_VEC_17_TO_12_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_ADC_LEVELS_VEC_17_TO_12_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_ADC_LEVELS_VEC_17_TO_12_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_ADC_LEVELS_VEC_11_TO_6_set        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_ADC_LEVELS_VEC_11_TO_6_get        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_ADC_LEVELS_VEC_11_TO_6_set  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_ADC_LEVELS_VEC_11_TO_6_get  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_ADC_LEVELS_VEC_5_TO_0_set         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_ADC_LEVELS_VEC_5_TO_0_get         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_ADC_LEVELS_VEC_5_TO_0_set   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_ADC_LEVELS_VEC_5_TO_0_get   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00040a04 CONFIG_2_LW20_80_15 |
 | +-----------------------------------------+
 | | bit  29:24 R/W  ADC_LEVELS_VEC_59_TO_54 |
 | | bit  23:18 R/W  ADC_LEVELS_VEC_53_TO_48 |
 | | bit  17:12 R/W  ADC_LEVELS_VEC_47_TO_42 |
 | | bit  11:6  R/W  ADC_LEVELS_VEC_41_TO_36 |
 | | bit  5:0   R/W  ADC_LEVELS_VEC_35_TO_30 |
 | +-----------------------------------------+
 | 
 | void   mdsp_t8_40n_field_ADC_LEVELS_VEC_59_TO_54_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_ADC_LEVELS_VEC_59_TO_54_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_ADC_LEVELS_VEC_59_TO_54_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_ADC_LEVELS_VEC_59_TO_54_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_ADC_LEVELS_VEC_53_TO_48_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_ADC_LEVELS_VEC_53_TO_48_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_ADC_LEVELS_VEC_53_TO_48_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_ADC_LEVELS_VEC_53_TO_48_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_ADC_LEVELS_VEC_47_TO_42_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_ADC_LEVELS_VEC_47_TO_42_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_ADC_LEVELS_VEC_47_TO_42_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_ADC_LEVELS_VEC_47_TO_42_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_ADC_LEVELS_VEC_41_TO_36_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_ADC_LEVELS_VEC_41_TO_36_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_ADC_LEVELS_VEC_41_TO_36_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_ADC_LEVELS_VEC_41_TO_36_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_ADC_LEVELS_VEC_35_TO_30_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_ADC_LEVELS_VEC_35_TO_30_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_ADC_LEVELS_VEC_35_TO_30_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_ADC_LEVELS_VEC_35_TO_30_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register 0x00040a08                                    |
 | | CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB |
 | +--------------------------------------------------------+
 | | bit  31    R/W  FIFO_HW_RESET_EN                       |
 | | bit  30    R/W  FIFO_RESET_FROM_PCBI                   |
 | | bit  29:27 R/W  RD_PTR_RCNTR                           |
 | | bit  26    R/W  BUS_WIDTH                              |
 | | bit  18    R/W  ADD_DROP_HOLD                          |
 | | bit  17    R/W  DLB_ENB                                |
 | | bit  16    R/W  CFG_FROM_PCBI_EN                       |
 | | bit  15    R/W  PCBI_PARAM_EN                          |
 | | bit  14    R/W  CAPTURE_REQ                            |
 | | bit  11    R/W  DFE_BYPASS                             |
 | | bit  10    R/W  INV_DFE_OUT                            |
 | | bit  9     R/W  INV_DFE_ERROR                          |
 | | bit  8     R/W  INV_DP_FFE_IN                          |
 | | bit  5:0   R/W  ADC_LEVELS_VEC_65_TO_60                |
 | +--------------------------------------------------------+
 | 
 | void   mdsp_t8_40n_field_FIFO_HW_RESET_EN_set              ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_FIFO_HW_RESET_EN_get              ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_FIFO_RESET_FROM_PCBI_set          ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_FIFO_RESET_FROM_PCBI_get          ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_RD_PTR_RCNTR_set                  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_RD_PTR_RCNTR_get                  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_RD_PTR_RCNTR_set            ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_RD_PTR_RCNTR_get            ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_BUS_WIDTH_set                     ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_BUS_WIDTH_get                     ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_ADD_DROP_HOLD_set                 ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_ADD_DROP_HOLD_get                 ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_DLB_ENB_set                       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_DLB_ENB_get                       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_CFG_FROM_PCBI_EN_set              ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_CFG_FROM_PCBI_EN_get              ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_PCBI_PARAM_EN_set                 ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_PCBI_PARAM_EN_get                 ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_DFE_BYPASS_set                    ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_DFE_BYPASS_get                    ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_INV_DFE_OUT_set                   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_INV_DFE_OUT_get                   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_INV_DFE_ERROR_set                 ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_INV_DFE_ERROR_get                 ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_INV_DP_FFE_IN_set                 ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_INV_DP_FFE_IN_get                 ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_ADC_LEVELS_VEC_65_TO_60_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_ADC_LEVELS_VEC_65_TO_60_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_ADC_LEVELS_VEC_65_TO_60_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_ADC_LEVELS_VEC_65_TO_60_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_CAPTURE_REQ_set                   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_CAPTURE_REQ_get                   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------+
 | | Register 0x00040a0c CONFIG_1_LW20_80_36 |
 | +-----------------------------------------+
 | | bit  26    R/W  TR_FFE_B_TIMING_DIS     |
 | | bit  25    R/W  TR_FFE_A_TIMING_DIS     |
 | | bit  24    R/W  TR_FFE_B_DATA_DIS       |
 | | bit  23    R/W  TR_FFE_A_DATA_DIS       |
 | | bit  21    R/W  LF_INTEG_LEFT_SHIFT     |
 | | bit  20:16 R/W  LF_INTEG_CTRL_GAIN      |
 | | bit  14    R/W  LF_PROP_LEFT_SHIFT      |
 | | bit  13:9  R/W  LF_PROP_CTRL_GAIN       |
 | | bit  8     R/W  TED_HOLD_OUTPUT_EN      |
 | | bit  7:6   R/W  TED_MODE_SEL            |
 | | bit  4:0   R/W  LF_BIAS                 |
 | +-----------------------------------------+
 | 
 | void   mdsp_t8_40n_field_TR_FFE_B_TIMING_DIS_set      ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_TR_FFE_B_TIMING_DIS_get      ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_TR_FFE_A_TIMING_DIS_set      ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_TR_FFE_A_TIMING_DIS_get      ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_TR_FFE_B_DATA_DIS_set        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_TR_FFE_B_DATA_DIS_get        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_TR_FFE_A_DATA_DIS_set        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_TR_FFE_A_DATA_DIS_get        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_LF_INTEG_LEFT_SHIFT_set      ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_LF_INTEG_LEFT_SHIFT_get      ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_LF_INTEG_CTRL_GAIN_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_LF_INTEG_CTRL_GAIN_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_LF_INTEG_CTRL_GAIN_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_LF_INTEG_CTRL_GAIN_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_LF_PROP_LEFT_SHIFT_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_LF_PROP_LEFT_SHIFT_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_LF_PROP_CTRL_GAIN_set        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_LF_PROP_CTRL_GAIN_get        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_LF_PROP_CTRL_GAIN_set  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_LF_PROP_CTRL_GAIN_get  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_TED_HOLD_OUTPUT_EN_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_TED_HOLD_OUTPUT_EN_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_TED_MODE_SEL_set             ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_TED_MODE_SEL_get             ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_TED_MODE_SEL_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_TED_MODE_SEL_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_LF_BIAS_set                  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_LF_BIAS_get                  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_LF_BIAS_set            ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_LF_BIAS_get            ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00040a10 CONFIG_2_LW20_80_36 |
 | +-----------------------------------------+
 | | bit  25:21 R/W  PC_INTERP_IDX_OVR       |
 | | bit  20    R/W  PC_INTERP_IDX_OVR_EN    |
 | | bit  19:18 R/W  LF_INTEG_OUT_MODE       |
 | | bit  7:0   R/W  LF_INTEG_OUT_OVR        |
 | +-----------------------------------------+
 | 
 | void   mdsp_t8_40n_field_PC_INTERP_IDX_OVR_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_PC_INTERP_IDX_OVR_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_PC_INTERP_IDX_OVR_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_PC_INTERP_IDX_OVR_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_PC_INTERP_IDX_OVR_EN_set    ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_PC_INTERP_IDX_OVR_EN_get    ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_LF_INTEG_OUT_MODE_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_LF_INTEG_OUT_MODE_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_LF_INTEG_OUT_MODE_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_LF_INTEG_OUT_MODE_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_LF_INTEG_OUT_OVR_set        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_LF_INTEG_OUT_OVR_get        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_LF_INTEG_OUT_OVR_set  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_LF_INTEG_OUT_OVR_get  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00040a14 CONFIG_3_LW20_80_36 |
 | +-----------------------------------------+
 | | bit  29:24 R/W  PC_NEG_OFFSET           |
 | | bit  23:18 R/W  PC_POS_OFFSET           |
 | | bit  7:0   R/W  LF_CLAMP_MAX            |
 | +-----------------------------------------+
 | 
 | void   mdsp_t8_40n_field_PC_NEG_OFFSET_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_PC_NEG_OFFSET_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_PC_NEG_OFFSET_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_PC_NEG_OFFSET_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_PC_POS_OFFSET_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_PC_POS_OFFSET_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_PC_POS_OFFSET_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_PC_POS_OFFSET_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_LF_CLAMP_MAX_set        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_LF_CLAMP_MAX_get        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_LF_CLAMP_MAX_set  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_LF_CLAMP_MAX_get  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00040a18 CONFIG_4_LW20_80_36 |
 | +-----------------------------------------+
 | | bit  27:23 R/W  PC_TH_DROP              |
 | | bit  22:18 R/W  PC_TH_ADD               |
 | | bit  17:12 R/W  EQ_TARGET_AMP           |
 | | bit  7:0   R/W  LF_CLAMP_MIN            |
 | +-----------------------------------------+
 | 
 | void   mdsp_t8_40n_field_PC_TH_DROP_set          ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_PC_TH_DROP_get          ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_PC_TH_DROP_set    ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_PC_TH_DROP_get    ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_PC_TH_ADD_set           ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_PC_TH_ADD_get           ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_PC_TH_ADD_set     ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_PC_TH_ADD_get     ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_EQ_TARGET_AMP_set       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_EQ_TARGET_AMP_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_EQ_TARGET_AMP_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_EQ_TARGET_AMP_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   mdsp_t8_40n_field_LF_CLAMP_MIN_set        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_LF_CLAMP_MIN_get        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void   mdsp_t8_40n_field_range_LF_CLAMP_MIN_set  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mdsp_t8_40n_field_range_LF_CLAMP_MIN_get  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00040a1c A_INTERP_DATAIN_SMP_0_TO_5 |
 | +------------------------------------------------+
 | | bit  29:24 R  A_INTERP_DATAIN_SMP_4            |
 | | bit  23:18 R  A_INTERP_DATAIN_SMP_3            |
 | | bit  17:12 R  A_INTERP_DATAIN_SMP_2            |
 | | bit  11:6  R  A_INTERP_DATAIN_SMP_1            |
 | | bit  5:0   R  A_INTERP_DATAIN_SMP_0            |
 | +------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_4_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_4_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_3_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_3_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_2_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_2_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_1_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_1_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_0_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_0_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00040a20 A_INTERP_DATAIN_SMP_5_TO_9 |
 | +------------------------------------------------+
 | | bit  29:24 R  A_INTERP_DATAIN_SMP_9            |
 | | bit  23:18 R  A_INTERP_DATAIN_SMP_8            |
 | | bit  17:12 R  A_INTERP_DATAIN_SMP_7            |
 | | bit  11:6  R  A_INTERP_DATAIN_SMP_6            |
 | | bit  5:0   R  A_INTERP_DATAIN_SMP_5            |
 | +------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_9_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_9_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_8_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_8_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_7_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_7_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_6_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_6_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_5_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_5_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00040a24 A_INTERP_DATAIN_SMP_10_TO_37 |
 | +--------------------------------------------------+
 | | bit  29:24 R  A_INTERP_DATAIN_SMP_14             |
 | | bit  23:18 R  A_INTERP_DATAIN_SMP_13             |
 | | bit  17:12 R  A_INTERP_DATAIN_SMP_12             |
 | | bit  11:6  R  A_INTERP_DATAIN_SMP_11             |
 | | bit  5:0   R  A_INTERP_DATAIN_SMP_10             |
 | +--------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_14_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_14_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_13_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_13_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_12_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_12_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_11_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_11_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_10_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_10_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00040a28 A_INTERP_DATAIN_SMP_15_TO_19 |
 | +--------------------------------------------------+
 | | bit  29:24 R  A_INTERP_DATAIN_SMP_19             |
 | | bit  23:18 R  A_INTERP_DATAIN_SMP_18             |
 | | bit  17:12 R  A_INTERP_DATAIN_SMP_17             |
 | | bit  11:6  R  A_INTERP_DATAIN_SMP_16             |
 | | bit  5:0   R  A_INTERP_DATAIN_SMP_15             |
 | +--------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_19_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_19_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_18_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_18_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_17_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_17_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_16_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_16_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_15_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_15_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00040a2c A_INTERP_DATAIN_SMP_20_TO_23 |
 | +--------------------------------------------------+
 | | bit  23:18 R  A_INTERP_DATAIN_SMP_23             |
 | | bit  17:12 R  A_INTERP_DATAIN_SMP_22             |
 | | bit  11:6  R  A_INTERP_DATAIN_SMP_21             |
 | | bit  5:0   R  A_INTERP_DATAIN_SMP_20             |
 | +--------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_23_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_23_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_22_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_22_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_21_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_21_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_A_INTERP_DATAIN_SMP_20_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_A_INTERP_DATAIN_SMP_20_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00040a30 B_INTERP_DATAIN_SMP_0_TO_5 |
 | +------------------------------------------------+
 | | bit  29:24 R  B_INTERP_DATAIN_SMP_4            |
 | | bit  23:18 R  B_INTERP_DATAIN_SMP_3            |
 | | bit  17:12 R  B_INTERP_DATAIN_SMP_2            |
 | | bit  11:6  R  B_INTERP_DATAIN_SMP_1            |
 | | bit  5:0   R  B_INTERP_DATAIN_SMP_0            |
 | +------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_4_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_4_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_3_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_3_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_2_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_2_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_1_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_1_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_0_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_0_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00040a34 B_INTERP_DATAIN_SMP_5_TO_9 |
 | +------------------------------------------------+
 | | bit  29:24 R  B_INTERP_DATAIN_SMP_9            |
 | | bit  23:18 R  B_INTERP_DATAIN_SMP_8            |
 | | bit  17:12 R  B_INTERP_DATAIN_SMP_7            |
 | | bit  11:6  R  B_INTERP_DATAIN_SMP_6            |
 | | bit  5:0   R  B_INTERP_DATAIN_SMP_5            |
 | +------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_9_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_9_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_8_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_8_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_7_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_7_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_6_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_6_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_5_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_5_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00040a38 B_INTERP_DATAIN_SMP_10_TO_37 |
 | +--------------------------------------------------+
 | | bit  29:24 R  B_INTERP_DATAIN_SMP_14             |
 | | bit  23:18 R  B_INTERP_DATAIN_SMP_13             |
 | | bit  17:12 R  B_INTERP_DATAIN_SMP_12             |
 | | bit  11:6  R  B_INTERP_DATAIN_SMP_11             |
 | | bit  5:0   R  B_INTERP_DATAIN_SMP_10             |
 | +--------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_14_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_14_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_13_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_13_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_12_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_12_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_11_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_11_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_10_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_10_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00040a3c B_INTERP_DATAIN_SMP_15_TO_19 |
 | +--------------------------------------------------+
 | | bit  29:24 R  B_INTERP_DATAIN_SMP_19             |
 | | bit  23:18 R  B_INTERP_DATAIN_SMP_18             |
 | | bit  17:12 R  B_INTERP_DATAIN_SMP_17             |
 | | bit  11:6  R  B_INTERP_DATAIN_SMP_16             |
 | | bit  5:0   R  B_INTERP_DATAIN_SMP_15             |
 | +--------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_19_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_19_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_18_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_18_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_17_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_17_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_16_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_16_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_15_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_15_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00040a40 B_INTERP_DATAIN_SMP_20_TO_23 |
 | +--------------------------------------------------+
 | | bit  23:18 R  B_INTERP_DATAIN_SMP_23             |
 | | bit  17:12 R  B_INTERP_DATAIN_SMP_22             |
 | | bit  11:6  R  B_INTERP_DATAIN_SMP_21             |
 | | bit  5:0   R  B_INTERP_DATAIN_SMP_20             |
 | +--------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_23_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_23_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_22_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_22_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_21_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_21_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_B_INTERP_DATAIN_SMP_20_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_B_INTERP_DATAIN_SMP_20_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00040a44 C_INTERP_DATAIN_SMP_0_TO_5 |
 | +------------------------------------------------+
 | | bit  29:24 R  C_INTERP_DATAIN_SMP_4            |
 | | bit  23:18 R  C_INTERP_DATAIN_SMP_3            |
 | | bit  17:12 R  C_INTERP_DATAIN_SMP_2            |
 | | bit  11:6  R  C_INTERP_DATAIN_SMP_1            |
 | | bit  5:0   R  C_INTERP_DATAIN_SMP_0            |
 | +------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_4_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_4_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_3_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_3_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_2_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_2_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_1_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_1_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_0_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_0_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00040a48 C_INTERP_DATAIN_SMP_5_TO_9 |
 | +------------------------------------------------+
 | | bit  29:24 R  C_INTERP_DATAIN_SMP_9            |
 | | bit  23:18 R  C_INTERP_DATAIN_SMP_8            |
 | | bit  17:12 R  C_INTERP_DATAIN_SMP_7            |
 | | bit  11:6  R  C_INTERP_DATAIN_SMP_6            |
 | | bit  5:0   R  C_INTERP_DATAIN_SMP_5            |
 | +------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_9_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_9_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_8_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_8_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_7_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_7_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_6_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_6_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_5_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_5_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00040a4c C_INTERP_DATAIN_SMP_10_TO_37 |
 | +--------------------------------------------------+
 | | bit  29:24 R  C_INTERP_DATAIN_SMP_14             |
 | | bit  23:18 R  C_INTERP_DATAIN_SMP_13             |
 | | bit  17:12 R  C_INTERP_DATAIN_SMP_12             |
 | | bit  11:6  R  C_INTERP_DATAIN_SMP_11             |
 | | bit  5:0   R  C_INTERP_DATAIN_SMP_10             |
 | +--------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_14_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_14_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_13_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_13_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_12_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_12_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_11_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_11_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_10_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_10_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00040a50 C_INTERP_DATAIN_SMP_15_TO_19 |
 | +--------------------------------------------------+
 | | bit  29:24 R  C_INTERP_DATAIN_SMP_19             |
 | | bit  23:18 R  C_INTERP_DATAIN_SMP_18             |
 | | bit  17:12 R  C_INTERP_DATAIN_SMP_17             |
 | | bit  11:6  R  C_INTERP_DATAIN_SMP_16             |
 | | bit  5:0   R  C_INTERP_DATAIN_SMP_15             |
 | +--------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_19_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_19_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_18_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_18_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_17_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_17_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_16_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_16_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_15_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_15_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00040a54 C_INTERP_DATAIN_SMP_20_TO_23 |
 | +--------------------------------------------------+
 | | bit  23:18 R  C_INTERP_DATAIN_SMP_23             |
 | | bit  17:12 R  C_INTERP_DATAIN_SMP_22             |
 | | bit  11:6  R  C_INTERP_DATAIN_SMP_21             |
 | | bit  5:0   R  C_INTERP_DATAIN_SMP_20             |
 | +--------------------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_23_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_23_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_22_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_22_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_21_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_21_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_C_INTERP_DATAIN_SMP_20_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_C_INTERP_DATAIN_SMP_20_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00040a58 A_TIMING_CTRL |
 | +-----------------------------------+
 | | bit  27:26 R  ADD_DROP_A3         |
 | | bit  25:24 R  ADD_DROP_A2         |
 | | bit  23:22 R  ADD_DROP_A1         |
 | | bit  21:20 R  ADD_DROP_A0         |
 | | bit  19:15 R  INTERP_IDX_A3       |
 | | bit  14:10 R  INTERP_IDX_A2       |
 | | bit  9:5   R  INTERP_IDX_A1       |
 | | bit  4:0   R  INTERP_IDX_A0       |
 | +-----------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_ADD_DROP_A3_get         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_DROP_A3_get   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_ADD_DROP_A2_get         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_DROP_A2_get   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_ADD_DROP_A1_get         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_DROP_A1_get   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_ADD_DROP_A0_get         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_DROP_A0_get   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_INTERP_IDX_A3_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_INTERP_IDX_A3_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_INTERP_IDX_A2_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_INTERP_IDX_A2_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_INTERP_IDX_A1_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_INTERP_IDX_A1_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_INTERP_IDX_A0_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_INTERP_IDX_A0_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00040a5c B_TIMING_CTRL |
 | +-----------------------------------+
 | | bit  27:26 R  ADD_DROP_B3         |
 | | bit  25:24 R  ADD_DROP_B2         |
 | | bit  23:22 R  ADD_DROP_B1         |
 | | bit  21:20 R  ADD_DROP_B0         |
 | | bit  19:15 R  INTERP_IDX_B3       |
 | | bit  14:10 R  INTERP_IDX_B2       |
 | | bit  9:5   R  INTERP_IDX_B1       |
 | | bit  4:0   R  INTERP_IDX_B0       |
 | +-----------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_ADD_DROP_B3_get         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_DROP_B3_get   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_ADD_DROP_B2_get         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_DROP_B2_get   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_ADD_DROP_B1_get         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_DROP_B1_get   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_ADD_DROP_B0_get         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_DROP_B0_get   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_INTERP_IDX_B3_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_INTERP_IDX_B3_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_INTERP_IDX_B2_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_INTERP_IDX_B2_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_INTERP_IDX_B1_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_INTERP_IDX_B1_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_INTERP_IDX_B0_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_INTERP_IDX_B0_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00040a60 C_TIMING_CTRL |
 | +-----------------------------------+
 | | bit  27:26 R  ADD_DROP_C3         |
 | | bit  25:24 R  ADD_DROP_C2         |
 | | bit  23:22 R  ADD_DROP_C1         |
 | | bit  21:20 R  ADD_DROP_C0         |
 | | bit  19:15 R  INTERP_IDX_C3       |
 | | bit  14:10 R  INTERP_IDX_C2       |
 | | bit  9:5   R  INTERP_IDX_C1       |
 | | bit  4:0   R  INTERP_IDX_C0       |
 | +-----------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_ADD_DROP_C3_get         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_DROP_C3_get   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_ADD_DROP_C2_get         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_DROP_C2_get   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_ADD_DROP_C1_get         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_DROP_C1_get   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_ADD_DROP_C0_get         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_DROP_C0_get   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_INTERP_IDX_C3_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_INTERP_IDX_C3_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_INTERP_IDX_C2_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_INTERP_IDX_C2_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_INTERP_IDX_C1_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_INTERP_IDX_C1_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_INTERP_IDX_C0_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_INTERP_IDX_C0_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x00040a64 DFE_STATE |
 | +-------------------------------+
 | | bit  4:0 R  DFE_STATE_CAPT    |
 | +-------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_DFE_STATE_CAPT_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_DFE_STATE_CAPT_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x00040a68 TCB_STATE |
 | +-------------------------------+
 | | bit  29:18 R  PC_INTEGRAL_ACC |
 | | bit  13:0  R  LF_INTEGRAL_ACC |
 | +-------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_PC_INTEGRAL_ACC_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_PC_INTEGRAL_ACC_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mdsp_t8_40n_field_LF_INTEGRAL_ACC_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_LF_INTEGRAL_ACC_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00040a6c ADD_DROP_ADDCNT |
 | +-------------------------------------+
 | | bit  19:0 R  ADD_COUNT              |
 | +-------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_ADD_COUNT_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_COUNT_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00040a70 ADD_DROP_DROPCNT |
 | +--------------------------------------+
 | | bit  19:0 R  DROP_COUNT              |
 | +--------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_DROP_COUNT_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_DROP_COUNT_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00040a74 ADD_DROP_DIFFCNT |
 | +--------------------------------------+
 | | bit  19:0 R  ADD_DROP_DIFF_COUNT     |
 | +--------------------------------------+
 | 
 | UINT32 mdsp_t8_40n_field_ADD_DROP_DIFF_COUNT_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_DROP_DIFF_COUNT_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00040a78 ADD_DROP_BLOCKCNT |
 | +---------------------------------------+
 | | bit  31   R/W  ADD_DROP_UPDATE_REQ    |
 | | bit  19:0 R    ADD_DROP_BLOCK_COUNT   |
 | +---------------------------------------+
 | 
 | void   mdsp_t8_40n_field_ADD_DROP_UPDATE_REQ_set        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 mdsp_t8_40n_field_ADD_DROP_UPDATE_REQ_get        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_ADD_DROP_BLOCK_COUNT_get       ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | UINT32 mdsp_t8_40n_field_range_ADD_DROP_BLOCK_COUNT_get ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MDSP_T8_40N_IO_H
#define _MDSP_T8_40N_IO_H

#include "digi_api.h"
#include "mdsp_t8_40n_regs.h"
#include "mdsp_t8_40n_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mdsp_t8_40n_buffer_init               ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | void                 mdsp_t8_40n_buffer_flush              ( mdsp_t8_40n_buffer_t *b_ptr )
 | UINT32               mdsp_t8_40n_reg_read                  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 mdsp_t8_40n_reg_write                 ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 mdsp_t8_40n_field_set                 ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 mdsp_t8_40n_action_on_write_field_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mdsp_t8_40n_burst_read                ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 mdsp_t8_40n_burst_write               ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE mdsp_t8_40n_poll                      ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   mdsp_t8_40n_reg_CONFIG_1_LW20_80_15_write                                        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   mdsp_t8_40n_reg_CONFIG_1_LW20_80_15_field_set                                    ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mdsp_t8_40n_reg_CONFIG_1_LW20_80_15_read                                         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | void   mdsp_t8_40n_reg_CONFIG_2_LW20_80_15_write                                        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   mdsp_t8_40n_reg_CONFIG_2_LW20_80_15_field_set                                    ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mdsp_t8_40n_reg_CONFIG_2_LW20_80_15_read                                         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | void   mdsp_t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_write     ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   mdsp_t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_field_set ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mdsp_t8_40n_reg_CONFIG_LW20_80_16_LW20_80_15_LW20_80_37_LW20_80_12_TSB_read      ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | void   mdsp_t8_40n_reg_CONFIG_1_LW20_80_36_write                                        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   mdsp_t8_40n_reg_CONFIG_1_LW20_80_36_field_set                                    ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mdsp_t8_40n_reg_CONFIG_1_LW20_80_36_read                                         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | void   mdsp_t8_40n_reg_CONFIG_2_LW20_80_36_write                                        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   mdsp_t8_40n_reg_CONFIG_2_LW20_80_36_field_set                                    ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mdsp_t8_40n_reg_CONFIG_2_LW20_80_36_read                                         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | void   mdsp_t8_40n_reg_CONFIG_3_LW20_80_36_write                                        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   mdsp_t8_40n_reg_CONFIG_3_LW20_80_36_field_set                                    ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mdsp_t8_40n_reg_CONFIG_3_LW20_80_36_read                                         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | void   mdsp_t8_40n_reg_CONFIG_4_LW20_80_36_write                                        ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   mdsp_t8_40n_reg_CONFIG_4_LW20_80_36_field_set                                    ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mdsp_t8_40n_reg_CONFIG_4_LW20_80_36_read                                         ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | void   mdsp_t8_40n_reg_ADD_DROP_BLOCKCNT_write                                          ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   mdsp_t8_40n_reg_ADD_DROP_BLOCKCNT_field_set                                      ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mdsp_t8_40n_reg_ADD_DROP_BLOCKCNT_read                                           ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_A_INTERP_DATAIN_SMP_0_TO_5_read                                  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_A_INTERP_DATAIN_SMP_5_TO_9_read                                  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_A_INTERP_DATAIN_SMP_10_TO_37_read                                ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_A_INTERP_DATAIN_SMP_15_TO_19_read                                ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_A_INTERP_DATAIN_SMP_20_TO_23_read                                ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_B_INTERP_DATAIN_SMP_0_TO_5_read                                  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_B_INTERP_DATAIN_SMP_5_TO_9_read                                  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_B_INTERP_DATAIN_SMP_10_TO_37_read                                ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_B_INTERP_DATAIN_SMP_15_TO_19_read                                ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_B_INTERP_DATAIN_SMP_20_TO_23_read                                ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_C_INTERP_DATAIN_SMP_0_TO_5_read                                  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_C_INTERP_DATAIN_SMP_5_TO_9_read                                  ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_C_INTERP_DATAIN_SMP_10_TO_37_read                                ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_C_INTERP_DATAIN_SMP_15_TO_19_read                                ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_C_INTERP_DATAIN_SMP_20_TO_23_read                                ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_A_TIMING_CTRL_read                                               ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_B_TIMING_CTRL_read                                               ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_C_TIMING_CTRL_read                                               ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_DFE_STATE_read                                                   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_TCB_STATE_read                                                   ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_ADD_DROP_ADDCNT_read                                             ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_ADD_DROP_DROPCNT_read                                            ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 mdsp_t8_40n_reg_ADD_DROP_DIFFCNT_read                                            ( mdsp_t8_40n_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 A )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MDSP_T8_40N_IO_H */
