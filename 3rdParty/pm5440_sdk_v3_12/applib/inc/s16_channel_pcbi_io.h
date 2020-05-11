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
 *     and register accessor functions for the s16_channel_pcbi block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing s16_channel_pcbi io functions is:
 | {
 |     s16_channel_pcbi_buffer_t b_ptr[1];
 |     s16_channel_pcbi_buffer_init( b_ptr, h_ptr, A );
 |     value = s16_channel_pcbi_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     s16_channel_pcbi_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     s16_channel_pcbi_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = s16_channel_pcbi_field_<FIELD>_get( NULL, h_ptr, A );
 | s16_channel_pcbi_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------------------+
 | | Register 0x00000000 CHANNEL_PCBI_MDSP_CFG_1 |
 | +---------------------------------------------+
 | | bit  0 R  INV_DFE_OUT                       |
 | +---------------------------------------------+
 | 
 | UINT32 s16_channel_pcbi_field_INV_DFE_OUT_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------------------+
 | | Register 0x00000004 CHANNEL_PCBI_RATE_ADAPT_FIFO_CFG_1 |
 | +--------------------------------------------------------+
 | | bit  2:0 R/W  RD_PTR_SYNC_VAL                          |
 | +--------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_RD_PTR_SYNC_VAL_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RD_PTR_SYNC_VAL_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_RD_PTR_SYNC_VAL_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_RD_PTR_SYNC_VAL_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000008 CHANNEL_PCBI_ADC_OC_DATA_CFG_1 |
 | +----------------------------------------------------+
 | | bit  31:28 R/W  LIN_STEP_DATA                      |
 | | bit  27:24 R/W  ACC_DELAY_DATA                     |
 | | bit  23:16 R/W  DAC_DELAY_DATA                     |
 | | bit  15:8  R/W  CAL_DELAY_DATA                     |
 | | bit  7:0   R/W  N_ACCUM_CYCLES_DATA                |
 | +----------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_LIN_STEP_DATA_set             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_LIN_STEP_DATA_get             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_LIN_STEP_DATA_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_LIN_STEP_DATA_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_ACC_DELAY_DATA_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ACC_DELAY_DATA_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_ACC_DELAY_DATA_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_ACC_DELAY_DATA_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_DAC_DELAY_DATA_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_DAC_DELAY_DATA_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_DAC_DELAY_DATA_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_DAC_DELAY_DATA_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_CAL_DELAY_DATA_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CAL_DELAY_DATA_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_CAL_DELAY_DATA_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_CAL_DELAY_DATA_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_N_ACCUM_CYCLES_DATA_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_N_ACCUM_CYCLES_DATA_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_N_ACCUM_CYCLES_DATA_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_N_ACCUM_CYCLES_DATA_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000000c CHANNEL_PCBI_ADC_OC_DATA_CFG_2 |
 | +----------------------------------------------------+
 | | bit  26:18 R/W  THR_MAP_DATA_DEFAULT_26_18         |
 | | bit  17:9  R/W  THR_MAP_DATA_DEFAULT_17_9          |
 | | bit  8:0   R/W  THR_MAP_DATA_DEFAULT_8_0           |
 | +----------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_26_18_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_26_18_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_26_18_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_26_18_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_17_9_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_17_9_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_17_9_set  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_17_9_get  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_8_0_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_8_0_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_8_0_set   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_8_0_get   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000010 CHANNEL_PCBI_ADC_OC_DATA_CFG_3 |
 | +----------------------------------------------------+
 | | bit  26:18 R/W  THR_MAP_DATA_DEFAULT_53_45         |
 | | bit  17:9  R/W  THR_MAP_DATA_DEFAULT_44_36         |
 | | bit  8:0   R/W  THR_MAP_DATA_DEFAULT_35_27         |
 | +----------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_53_45_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_53_45_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_53_45_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_53_45_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_44_36_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_44_36_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_44_36_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_44_36_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_35_27_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_35_27_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_35_27_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_35_27_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000014 CHANNEL_PCBI_ADC_OC_DATA_CFG_4 |
 | +----------------------------------------------------+
 | | bit  26:18 R/W  THR_MAP_DATA_DEFAULT_80_72         |
 | | bit  17:9  R/W  THR_MAP_DATA_DEFAULT_71_63         |
 | | bit  8:0   R/W  THR_MAP_DATA_DEFAULT_62_54         |
 | +----------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_80_72_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_80_72_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_80_72_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_80_72_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_71_63_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_71_63_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_71_63_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_71_63_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_62_54_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_62_54_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_62_54_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_62_54_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000018 CHANNEL_PCBI_ADC_OC_DATA_CFG_5 |
 | +----------------------------------------------------+
 | | bit  8:0 R/W  THR_MAP_DATA_DEFAULT_89_81           |
 | +----------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_89_81_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_THR_MAP_DATA_DEFAULT_89_81_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_89_81_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_THR_MAP_DATA_DEFAULT_89_81_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000001c CHANNEL_PCBI_ADC_OC_DATA_CFG_6 |
 | +----------------------------------------------------+
 | | bit  31    R    ADC_FSM_ERR_DATA                   |
 | | bit  30    R    ADC_BIST_ERR_DATA                  |
 | | bit  29    R    ADC_BIST_DONE_DATA                 |
 | | bit  20    R/W  ADC_OC_DATA_INV_POL                |
 | | bit  19:11 R/W  MAX_THRESHOLD_DATA                 |
 | | bit  10:2  R/W  MIN_THRESHOLD_DATA                 |
 | | bit  1     R/W  ADC_BIST_START_DATA                |
 | | bit  0     R/W  OFFSET_EN_DATA                     |
 | +----------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_ADC_OC_DATA_INV_POL_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_OC_DATA_INV_POL_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_MAX_THRESHOLD_DATA_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_MAX_THRESHOLD_DATA_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_MAX_THRESHOLD_DATA_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_MAX_THRESHOLD_DATA_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_MIN_THRESHOLD_DATA_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_MIN_THRESHOLD_DATA_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_MIN_THRESHOLD_DATA_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_MIN_THRESHOLD_DATA_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_ADC_BIST_START_DATA_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_BIST_START_DATA_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_OFFSET_EN_DATA_set           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OFFSET_EN_DATA_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_ADC_FSM_ERR_DATA_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_ADC_BIST_ERR_DATA_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_ADC_BIST_DONE_DATA_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------------------------+
 | | Register 0x00000020 CHANNEL_PCBI_ADC_OC_TIMING_CFG_1 |
 | +------------------------------------------------------+
 | | bit  31:28 R/W  LIN_STEP_TIMING                      |
 | | bit  27:24 R/W  ACC_DELAY_TIMING                     |
 | | bit  23:16 R/W  DAC_DELAY_TIMING                     |
 | | bit  15:8  R/W  CAL_DELAY_TIMING                     |
 | | bit  7:0   R/W  N_ACCUM_CYCLES_TIMING                |
 | +------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_LIN_STEP_TIMING_set             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_LIN_STEP_TIMING_get             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_LIN_STEP_TIMING_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_LIN_STEP_TIMING_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_ACC_DELAY_TIMING_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ACC_DELAY_TIMING_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_ACC_DELAY_TIMING_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_ACC_DELAY_TIMING_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_DAC_DELAY_TIMING_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_DAC_DELAY_TIMING_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_DAC_DELAY_TIMING_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_DAC_DELAY_TIMING_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_CAL_DELAY_TIMING_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CAL_DELAY_TIMING_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_CAL_DELAY_TIMING_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_CAL_DELAY_TIMING_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_N_ACCUM_CYCLES_TIMING_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_N_ACCUM_CYCLES_TIMING_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_N_ACCUM_CYCLES_TIMING_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_N_ACCUM_CYCLES_TIMING_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x00000024 CHANNEL_PCBI_ADC_OC_TIMING_CFG_2 |
 | +------------------------------------------------------+
 | | bit  26:18 R/W  THR_MAP_TIMING_DEFAULT_26_18         |
 | | bit  17:9  R/W  THR_MAP_TIMING_DEFAULT_17_9          |
 | | bit  8:0   R/W  THR_MAP_TIMING_DEFAULT_8_0           |
 | +------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_THR_MAP_TIMING_DEFAULT_26_18_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_THR_MAP_TIMING_DEFAULT_26_18_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_THR_MAP_TIMING_DEFAULT_26_18_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_THR_MAP_TIMING_DEFAULT_26_18_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_THR_MAP_TIMING_DEFAULT_17_9_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_THR_MAP_TIMING_DEFAULT_17_9_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_THR_MAP_TIMING_DEFAULT_17_9_set  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_THR_MAP_TIMING_DEFAULT_17_9_get  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_THR_MAP_TIMING_DEFAULT_8_0_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_THR_MAP_TIMING_DEFAULT_8_0_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_THR_MAP_TIMING_DEFAULT_8_0_set   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_THR_MAP_TIMING_DEFAULT_8_0_get   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x00000028 CHANNEL_PCBI_ADC_OC_TIMING_CFG_3 |
 | +------------------------------------------------------+
 | | bit  8:0 R/W  THR_MAP_TIMING_DEFAULT_35_27           |
 | +------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_THR_MAP_TIMING_DEFAULT_35_27_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_THR_MAP_TIMING_DEFAULT_35_27_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_THR_MAP_TIMING_DEFAULT_35_27_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_THR_MAP_TIMING_DEFAULT_35_27_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x0000002c CHANNEL_PCBI_ADC_OC_TIMING_CFG_4 |
 | +------------------------------------------------------+
 | | bit  31    R    ADC_FSM_ERR_TIMING                   |
 | | bit  30    R    ADC_BIST_ERR_TIMING                  |
 | | bit  29    R    ADC_BIST_DONE_TIMING                 |
 | | bit  20    R/W  ADC_OC_TIMING_INV_POL                |
 | | bit  19:11 R/W  MAX_THRESHOLD_TIMING                 |
 | | bit  10:2  R/W  MIN_THRESHOLD_TIMING                 |
 | | bit  1     R/W  ADC_BIST_START_TIMING                |
 | | bit  0     R/W  OFFSET_EN_TIMING                     |
 | +------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_ADC_OC_TIMING_INV_POL_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_OC_TIMING_INV_POL_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_MAX_THRESHOLD_TIMING_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_MAX_THRESHOLD_TIMING_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_MAX_THRESHOLD_TIMING_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_MAX_THRESHOLD_TIMING_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_MIN_THRESHOLD_TIMING_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_MIN_THRESHOLD_TIMING_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_MIN_THRESHOLD_TIMING_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_MIN_THRESHOLD_TIMING_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_ADC_BIST_START_TIMING_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_BIST_START_TIMING_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_OFFSET_EN_TIMING_set           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OFFSET_EN_TIMING_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_ADC_FSM_ERR_TIMING_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_ADC_BIST_ERR_TIMING_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_ADC_BIST_DONE_TIMING_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------------------------------+
 | | Register 0x00000030 CHANNEL_PCBI_ADC_OC_GENERAL_CFG_1 |
 | +-------------------------------------------------------+
 | | bit  26    R/W  DATA_RAM_SEL_OVR                      |
 | | bit  25    R/W  DATA_RAM_SEL_OVR_EN                   |
 | | bit  24    R/W  UPDATE_CLK_BYP                        |
 | | bit  23    R/W  ADC_OC_BRIDGE_SYNC_RESET              |
 | | bit  22    R/W  ADC_RD_OVR                            |
 | | bit  21    R/W  ADC_WR_OVR                            |
 | | bit  20:12 R/W  ADC_D_OVR                             |
 | | bit  11:4  R/W  ADC_A_OVR                             |
 | | bit  2     R/W  MABC_ADC_OC_INT_OVR_EN                |
 | | bit  1     R/W  CALIB_PROG_EN                         |
 | +-------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_DATA_RAM_SEL_OVR_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_DATA_RAM_SEL_OVR_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_DATA_RAM_SEL_OVR_EN_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_DATA_RAM_SEL_OVR_EN_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_UPDATE_CLK_BYP_set           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_UPDATE_CLK_BYP_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_ADC_OC_BRIDGE_SYNC_RESET_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_OC_BRIDGE_SYNC_RESET_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_ADC_RD_OVR_set               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_RD_OVR_get               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_ADC_WR_OVR_set               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_WR_OVR_get               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_ADC_D_OVR_set                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_D_OVR_get                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_ADC_D_OVR_set          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_ADC_D_OVR_get          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_ADC_A_OVR_set                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_A_OVR_get                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_ADC_A_OVR_set          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_ADC_A_OVR_get          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_MABC_ADC_OC_INT_OVR_EN_set   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_MABC_ADC_OC_INT_OVR_EN_get   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_CALIB_PROG_EN_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CALIB_PROG_EN_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------------------------------+
 | | Register 0x00000034 CHANNEL_PCBI_ADC_OC_GENERAL_CFG_2 |
 | +-------------------------------------------------------+
 | | bit  29:28 R/W  OFFSET_RAM_RAM_ERR_INJECT             |
 | | bit  25    R/W  OFFSET_RAM_SD                         |
 | | bit  24    R/W  OFFSET_RAM_DS                         |
 | | bit  23    R/W  OFFSET_RAM_LS                         |
 | | bit  22:14 R/W  OFFSET_RAM_DIN                        |
 | | bit  12:4  R/W  OFFSET_RAM_ADDR                       |
 | | bit  3     R/W  OFFSET_RAM_RWB                        |
 | | bit  2     R/W  OFFSET_RAM_CEB                        |
 | | bit  1     R/W  OFFSET_RAM_CLK_SEL                    |
 | | bit  0     R/W  OFFSET_RAM_PCLK_EN                    |
 | +-------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_OFFSET_RAM_RAM_ERR_INJECT_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OFFSET_RAM_RAM_ERR_INJECT_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_OFFSET_RAM_RAM_ERR_INJECT_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_OFFSET_RAM_RAM_ERR_INJECT_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_OFFSET_RAM_SD_set                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OFFSET_RAM_SD_get                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_OFFSET_RAM_DS_set                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OFFSET_RAM_DS_get                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_OFFSET_RAM_LS_set                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OFFSET_RAM_LS_get                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_OFFSET_RAM_DIN_set                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OFFSET_RAM_DIN_get                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_OFFSET_RAM_DIN_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_OFFSET_RAM_DIN_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_OFFSET_RAM_ADDR_set                 ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OFFSET_RAM_ADDR_get                 ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_OFFSET_RAM_ADDR_set           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_OFFSET_RAM_ADDR_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_OFFSET_RAM_RWB_set                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OFFSET_RAM_RWB_get                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_OFFSET_RAM_CEB_set                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OFFSET_RAM_CEB_get                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_OFFSET_RAM_CLK_SEL_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OFFSET_RAM_CLK_SEL_get              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_OFFSET_RAM_PCLK_EN_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OFFSET_RAM_PCLK_EN_get              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------------------------------+
 | | Register 0x00000038 CHANNEL_PCBI_ADC_OC_GENERAL_CFG_3 |
 | +-------------------------------------------------------+
 | | bit  8:0 R  OFFSET_RAM_DOUT                           |
 | +-------------------------------------------------------+
 | 
 | UINT32 s16_channel_pcbi_field_OFFSET_RAM_DOUT_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_range_OFFSET_RAM_DOUT_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000003c CHANNEL_PCBI_RAM_CAPTURE_CFG_1 |
 | +----------------------------------------------------+
 | | bit  30 R/W  RAM_CAPTURE_SD                        |
 | | bit  29 R/W  RAM_CAPTURE_DS                        |
 | | bit  28 R/W  RAM_CAPTURE_LS                        |
 | | bit  5  R/W  RAMCAPT_DATA_RD_EN                    |
 | | bit  4  R/W  RAMCAPT_RST_RD_ADDR                   |
 | | bit  3  R/W  RAMCAPT_CAPT_START                    |
 | | bit  2  R/W  RAMCAPT_STOP_EVENT_SELECT             |
 | | bit  1  R/W  RAMCAPT_CAPT_MODE                     |
 | | bit  0  R/W  RAMCAPT_ENB                           |
 | +----------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_RAM_CAPTURE_SD_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RAM_CAPTURE_SD_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RAM_CAPTURE_DS_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RAM_CAPTURE_DS_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RAM_CAPTURE_LS_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RAM_CAPTURE_LS_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RAMCAPT_RST_RD_ADDR_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RAMCAPT_RST_RD_ADDR_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RAMCAPT_CAPT_START_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RAMCAPT_CAPT_START_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RAMCAPT_STOP_EVENT_SELECT_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RAMCAPT_STOP_EVENT_SELECT_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RAMCAPT_CAPT_MODE_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RAMCAPT_CAPT_MODE_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RAMCAPT_ENB_set               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RAMCAPT_ENB_get               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RAMCAPT_DATA_RD_EN_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RAMCAPT_DATA_RD_EN_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000040 CHANNEL_PCBI_RAM_CAPTURE_CFG_2 |
 | +----------------------------------------------------+
 | | bit  31:0 R  RAMCAPT_DATA_OUT                      |
 | +----------------------------------------------------+
 | 
 | UINT32 s16_channel_pcbi_field_RAMCAPT_DATA_OUT_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_range_RAMCAPT_DATA_OUT_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00000044 CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_1 |
 | +-----------------------------------------------------------+
 | | bit  3 R/W  VHIST_PERBIN_VALUE_RD_ENB                     |
 | | bit  2 R/W  VHIST_CAPT_START                              |
 | | bit  1 R/W  VHIST_TITAN_TARGET_AMP_WR_EN                  |
 | | bit  0 R/W  VHIST_CAPT_ENB                                |
 | +-----------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_VHIST_PERBIN_VALUE_RD_ENB_set    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_VHIST_PERBIN_VALUE_RD_ENB_get    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_VHIST_CAPT_START_set             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_VHIST_CAPT_START_get             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_VHIST_TITAN_TARGET_AMP_WR_EN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_VHIST_TITAN_TARGET_AMP_WR_EN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_VHIST_CAPT_ENB_set               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_VHIST_CAPT_ENB_get               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00000048 CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_2 |
 | +-----------------------------------------------------------+
 | | bit  31:0 R/W  VHIST_CNT_TIM_LIMIT_31_0                   |
 | +-----------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_VHIST_CNT_TIM_LIMIT_31_0_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_VHIST_CNT_TIM_LIMIT_31_0_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_VHIST_CNT_TIM_LIMIT_31_0_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_VHIST_CNT_TIM_LIMIT_31_0_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x0000004c CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_3 |
 | +-----------------------------------------------------------+
 | | bit  7:0 R/W  VHIST_CNT_TIM_LIMIT_39_32                   |
 | +-----------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_VHIST_CNT_TIM_LIMIT_39_32_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_VHIST_CNT_TIM_LIMIT_39_32_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_VHIST_CNT_TIM_LIMIT_39_32_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_VHIST_CNT_TIM_LIMIT_39_32_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00000050 CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_4 |
 | +-----------------------------------------------------------+
 | | bit  10  R  VHIST_PERBIN_VALUE_OVFL                       |
 | | bit  9:4 R  VHIST_READBACK_ADDR                           |
 | | bit  3:1 R  VHIST_FSM_STATE                               |
 | | bit  0   R  VHIST_CAPT_RUNNING                            |
 | +-----------------------------------------------------------+
 | 
 | UINT32 s16_channel_pcbi_field_VHIST_PERBIN_VALUE_OVFL_get   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_VHIST_READBACK_ADDR_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_range_VHIST_READBACK_ADDR_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 s16_channel_pcbi_field_VHIST_FSM_STATE_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_range_VHIST_FSM_STATE_get     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 s16_channel_pcbi_field_VHIST_CAPT_RUNNING_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00000054 CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_5 |
 | +-----------------------------------------------------------+
 | | bit  31:0 R  VHIST_PERBIN_VALUE_31_0                      |
 | +-----------------------------------------------------------+
 | 
 | UINT32 s16_channel_pcbi_field_VHIST_PERBIN_VALUE_31_0_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_range_VHIST_PERBIN_VALUE_31_0_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00000058 CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_6 |
 | +-----------------------------------------------------------+
 | | bit  7:0 R  VHIST_PERBIN_VALUE_39_32                      |
 | +-----------------------------------------------------------+
 | 
 | UINT32 s16_channel_pcbi_field_VHIST_PERBIN_VALUE_39_32_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_range_VHIST_PERBIN_VALUE_39_32_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0000005c CHANNEL_PCBI_SSI_CFG_1 |
 | +--------------------------------------------+
 | | bit  25:21 R/W  SSI_THRESHOLD_OVR          |
 | | bit  20    R/W  SSI_THRESHOLD_OVR_EN       |
 | | bit  19    R/W  SSI_OVR                    |
 | | bit  18    R/W  SSI_ADJ_THR_RELEASES       |
 | | bit  17:4  R/W  SSI_LIMIT                  |
 | | bit  3:0   R/W  SSI_ACC_CNT                |
 | +--------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_SSI_THRESHOLD_OVR_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_SSI_THRESHOLD_OVR_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_SSI_THRESHOLD_OVR_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_SSI_THRESHOLD_OVR_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_SSI_THRESHOLD_OVR_EN_set    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_SSI_THRESHOLD_OVR_EN_get    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_SSI_OVR_set                 ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_SSI_OVR_get                 ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_SSI_ADJ_THR_RELEASES_set    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_SSI_ADJ_THR_RELEASES_get    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_SSI_LIMIT_set               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_SSI_LIMIT_get               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_SSI_LIMIT_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_SSI_LIMIT_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_SSI_ACC_CNT_set             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_SSI_ACC_CNT_get             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_SSI_ACC_CNT_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_SSI_ACC_CNT_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000060 CHANNEL_PCBI_SSI_CFG_2 |
 | +--------------------------------------------+
 | | bit  5:1 R  SSI_CURR_THRESHOLD             |
 | | bit  0   R  SSI_ADJUST_DONE                |
 | +--------------------------------------------+
 | 
 | UINT32 s16_channel_pcbi_field_SSI_CURR_THRESHOLD_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_range_SSI_CURR_THRESHOLD_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 s16_channel_pcbi_field_SSI_ADJUST_DONE_get          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------+
 | | Register 0x00000064 CHANNEL_PCBI_LOS_CFG_1 |
 | +--------------------------------------------+
 | | bit  3 R/W  RXLOS_EXT_LOOPBACK_EN          |
 | | bit  2 R/W  RXLOS_INT_LOOPBACK_EN          |
 | | bit  1 R/W  TX_SQUELCH_OVR                 |
 | | bit  0 R/W  TX_SQUELCH_OVR_EN              |
 | +--------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_RXLOS_EXT_LOOPBACK_EN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RXLOS_EXT_LOOPBACK_EN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RXLOS_INT_LOOPBACK_EN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RXLOS_INT_LOOPBACK_EN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_TX_SQUELCH_OVR_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TX_SQUELCH_OVR_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_TX_SQUELCH_OVR_EN_set     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TX_SQUELCH_OVR_EN_get     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------------------------------+
 | | Register 0x00000068 CHANNEL_PCBI_TX_DCD_OFFSET_CORRECTION_CFG_1 |
 | +-----------------------------------------------------------------+
 | | bit  31:27 R/W  OC_OVERRIDE                                     |
 | | bit  26    R/W  OC_OVERRIDE_ENB                                 |
 | | bit  25    R/W  OC_SENSE_ENB                                    |
 | | bit  9:6   R/W  OC_THRESHOLD                                    |
 | | bit  5:4   R/W  OCC_DONE_GEN_SEL                                |
 | | bit  2     R/W  OC_INVERT                                       |
 | | bit  1     R/W  OCC_CONT                                        |
 | +-----------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_OC_OVERRIDE_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OC_OVERRIDE_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_OC_OVERRIDE_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_OC_OVERRIDE_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_OC_OVERRIDE_ENB_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OC_OVERRIDE_ENB_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_OC_SENSE_ENB_set           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OC_SENSE_ENB_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_OC_THRESHOLD_set           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OC_THRESHOLD_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_OC_THRESHOLD_set     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_OC_THRESHOLD_get     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_OCC_DONE_GEN_SEL_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OCC_DONE_GEN_SEL_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_OCC_DONE_GEN_SEL_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_OCC_DONE_GEN_SEL_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_OC_INVERT_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OC_INVERT_get              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_OCC_CONT_set               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OCC_CONT_get               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------------------------------+
 | | Register 0x0000006c CHANNEL_PCBI_TX_DCD_OFFSET_CORRECTION_CFG_2 |
 | +-----------------------------------------------------------------+
 | | bit  30:15 R/W  OC_INTERVAL                                     |
 | | bit  14:0  R/W  OC_TOLERANCE                                    |
 | +-----------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_OC_INTERVAL_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OC_INTERVAL_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_OC_INTERVAL_set  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_OC_INTERVAL_get  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_OC_TOLERANCE_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OC_TOLERANCE_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_OC_TOLERANCE_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_OC_TOLERANCE_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00000070 CHANNEL_PCBI_TX_DCD_OFFSET_CORR_CFG_3 |
 | +-----------------------------------------------------------+
 | | bit  31:27 R/W  OC_OVERRIDE_DIV3                          |
 | | bit  26    R/W  OC_OVERRIDE_ENB_DIV3                      |
 | | bit  9:6   R/W  OC_THRESHOLD_DIV3                         |
 | | bit  5:4   R/W  OCC_DONE_GEN_SEL_DIV3                     |
 | | bit  2     R/W  OC_INVERT_DIV3                            |
 | | bit  1     R/W  OCC_CONT_DIV3                             |
 | +-----------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_OC_OVERRIDE_DIV3_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OC_OVERRIDE_DIV3_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_OC_OVERRIDE_DIV3_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_OC_OVERRIDE_DIV3_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_OC_OVERRIDE_ENB_DIV3_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OC_OVERRIDE_ENB_DIV3_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_OC_THRESHOLD_DIV3_set           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OC_THRESHOLD_DIV3_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_OC_THRESHOLD_DIV3_set     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_OC_THRESHOLD_DIV3_get     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_OCC_DONE_GEN_SEL_DIV3_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OCC_DONE_GEN_SEL_DIV3_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_OCC_DONE_GEN_SEL_DIV3_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_OCC_DONE_GEN_SEL_DIV3_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_OC_INVERT_DIV3_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OC_INVERT_DIV3_get              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_OCC_CONT_DIV3_set               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OCC_CONT_DIV3_get               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00000074 CHANNEL_PCBI_TX_DCD_OFFSET_CORR_CFG_4 |
 | +-----------------------------------------------------------+
 | | bit  30:15 R/W  OC_INTERVAL_DIV3                          |
 | | bit  14:0  R/W  OC_TOLERANCE_DIV3                         |
 | +-----------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_OC_INTERVAL_DIV3_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OC_INTERVAL_DIV3_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_OC_INTERVAL_DIV3_set  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_OC_INTERVAL_DIV3_get  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_OC_TOLERANCE_DIV3_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OC_TOLERANCE_DIV3_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_OC_TOLERANCE_DIV3_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_OC_TOLERANCE_DIV3_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------------+
 | | Register 0x00000078 CHANNEL_PCBI_TX_DCD_OFFSET_CORRECTION_CFG_5 |
 | +-----------------------------------------------------------------+
 | | bit  5   R  OC_THRESHOLD_CHECK_FAILB                            |
 | | bit  4:0 R  OCDAC_UNENC                                         |
 | +-----------------------------------------------------------------+
 | 
 | UINT32 s16_channel_pcbi_field_OC_THRESHOLD_CHECK_FAILB_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_OCDAC_UNENC_get              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_range_OCDAC_UNENC_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x0000007c CHANNEL_PCBI_TX_DCD_OFFSET_CORR_CFG_6 |
 | +-----------------------------------------------------------+
 | | bit  5   R  OC_THRESHOLD_CHECK_FAILB_DIV3                 |
 | | bit  4:0 R  OCDAC_UNENC_DIV3                              |
 | +-----------------------------------------------------------+
 | 
 | UINT32 s16_channel_pcbi_field_OC_THRESHOLD_CHECK_FAILB_DIV3_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_OCDAC_UNENC_DIV3_get              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_range_OCDAC_UNENC_DIV3_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x00000080 CHANNEL_PCBI_TX_CTRL_LOGIC_CFG_1 |
 | +------------------------------------------------------+
 | | bit  30:26 R/W  P1B_PT                               |
 | | bit  25:21 R/W  P1B_D2                               |
 | | bit  20:16 R/W  P1B_D1                               |
 | | bit  15:11 R/W  P1A_PT                               |
 | | bit  10:6  R/W  P1A_D2                               |
 | | bit  5:1   R/W  P1A_D1                               |
 | | bit  0     R/W  TX_CTRL_LOGIC_OVR_EN                 |
 | +------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_P1B_PT_set               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P1B_PT_get               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P1B_PT_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P1B_PT_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P1B_D2_set               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P1B_D2_get               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P1B_D2_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P1B_D2_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P1B_D1_set               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P1B_D1_get               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P1B_D1_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P1B_D1_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P1A_PT_set               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P1A_PT_get               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P1A_PT_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P1A_PT_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P1A_D2_set               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P1A_D2_get               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P1A_D2_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P1A_D2_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P1A_D1_set               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P1A_D1_get               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P1A_D1_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P1A_D1_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_TX_CTRL_LOGIC_OVR_EN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TX_CTRL_LOGIC_OVR_EN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------------------------+
 | | Register 0x00000084 CHANNEL_PCBI_TX_CTRL_LOGIC_CFG_2 |
 | +------------------------------------------------------+
 | | bit  31    R/W  PISO_PRE2_EN                         |
 | | bit  30    R/W  PISO_PRE1_EN                         |
 | | bit  29:25 R/W  P2B_PT                               |
 | | bit  24:20 R/W  P2B_D2                               |
 | | bit  19:15 R/W  P2B_D1                               |
 | | bit  14:10 R/W  P2A_PT                               |
 | | bit  9:5   R/W  P2A_D2                               |
 | | bit  4:0   R/W  P2A_D1                               |
 | +------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_PISO_PRE2_EN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PISO_PRE2_EN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_PISO_PRE1_EN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PISO_PRE1_EN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_P2B_PT_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P2B_PT_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P2B_PT_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P2B_PT_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P2B_D2_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P2B_D2_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P2B_D2_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P2B_D2_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P2B_D1_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P2B_D1_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P2B_D1_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P2B_D1_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P2A_PT_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P2A_PT_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P2A_PT_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P2A_PT_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P2A_D2_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P2A_D2_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P2A_D2_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P2A_D2_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P2A_D1_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P2A_D1_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P2A_D1_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P2A_D1_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------+
 | | Register 0x00000088 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_1 |
 | +------------------------------------------------------------------+
 | | bit  13   R/W  CHK_INV                                           |
 | | bit  12:9 R/W  CHK_MODE                                          |
 | | bit  8    R/W  CHK_READ                                          |
 | | bit  7    R/W  CHK_EN                                            |
 | | bit  6    R/W  GEN_ERR_INJ                                       |
 | | bit  5    R/W  GEN_INV                                           |
 | | bit  4:1  R/W  GEN_MODE                                          |
 | | bit  0    R/W  GEN_EN                                            |
 | +------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_CHK_INV_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CHK_INV_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_CHK_MODE_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CHK_MODE_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_CHK_MODE_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_CHK_MODE_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_CHK_EN_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CHK_EN_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_GEN_INV_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_GEN_INV_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_GEN_MODE_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_GEN_MODE_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_GEN_MODE_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_GEN_MODE_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_GEN_EN_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_GEN_EN_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_CHK_READ_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CHK_READ_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_GEN_ERR_INJ_set    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_GEN_ERR_INJ_get    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------------------------------------+
 | | Register 0x0000008c CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_2 |
 | +------------------------------------------------------------------+
 | | bit  31:0 R/W  GEN_USR_PAT_31_TO_0                               |
 | +------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_GEN_USR_PAT_31_TO_0_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_GEN_USR_PAT_31_TO_0_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_GEN_USR_PAT_31_TO_0_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_GEN_USR_PAT_31_TO_0_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------+
 | | Register 0x00000090 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_3 |
 | +------------------------------------------------------------------+
 | | bit  31:0 R/W  GEN_USR_PAT_63_TO_32                              |
 | +------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_GEN_USR_PAT_63_TO_32_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_GEN_USR_PAT_63_TO_32_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_GEN_USR_PAT_63_TO_32_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_GEN_USR_PAT_63_TO_32_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------+
 | | Register 0x00000094 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_4 |
 | +------------------------------------------------------------------+
 | | bit  31:0 R/W  GEN_USR_PAT_95_TO_64                              |
 | +------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_GEN_USR_PAT_95_TO_64_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_GEN_USR_PAT_95_TO_64_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_GEN_USR_PAT_95_TO_64_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_GEN_USR_PAT_95_TO_64_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------+
 | | Register 0x00000098 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_5 |
 | +------------------------------------------------------------------+
 | | bit  31:0 R/W  GEN_USR_PAT_127_TO_96                             |
 | +------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_GEN_USR_PAT_127_TO_96_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_GEN_USR_PAT_127_TO_96_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_GEN_USR_PAT_127_TO_96_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_GEN_USR_PAT_127_TO_96_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------+
 | | Register 0x0000009c CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_6 |
 | +------------------------------------------------------------------+
 | | bit  31:0 R/W  GEN_USR_PAT_159_TO_128                            |
 | +------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_GEN_USR_PAT_159_TO_128_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_GEN_USR_PAT_159_TO_128_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_GEN_USR_PAT_159_TO_128_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_GEN_USR_PAT_159_TO_128_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------+
 | | Register 0x000000a0 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_7 |
 | +------------------------------------------------------------------+
 | | bit  31:0 R/W  GEN_USR_PAT_191_TO_160                            |
 | +------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_GEN_USR_PAT_191_TO_160_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_GEN_USR_PAT_191_TO_160_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_GEN_USR_PAT_191_TO_160_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_GEN_USR_PAT_191_TO_160_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------+
 | | Register 0x000000a4 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_8 |
 | +------------------------------------------------------------------+
 | | bit  31:0 R/W  GEN_USR_PAT_223_TO_192                            |
 | +------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_GEN_USR_PAT_223_TO_192_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_GEN_USR_PAT_223_TO_192_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_GEN_USR_PAT_223_TO_192_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_GEN_USR_PAT_223_TO_192_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------+
 | | Register 0x000000a8 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_9 |
 | +------------------------------------------------------------------+
 | | bit  31:0 R/W  GEN_USR_PAT_255_TO_224                            |
 | +------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_GEN_USR_PAT_255_TO_224_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_GEN_USR_PAT_255_TO_224_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_GEN_USR_PAT_255_TO_224_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_GEN_USR_PAT_255_TO_224_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x000000ac CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_10 |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  GEN_USR_PAT_287_TO_256                             |
 | +-------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_GEN_USR_PAT_287_TO_256_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_GEN_USR_PAT_287_TO_256_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_GEN_USR_PAT_287_TO_256_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_GEN_USR_PAT_287_TO_256_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x000000b0 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_11 |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  GEN_USR_PAT_319_TO_288                             |
 | +-------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_GEN_USR_PAT_319_TO_288_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_GEN_USR_PAT_319_TO_288_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_GEN_USR_PAT_319_TO_288_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_GEN_USR_PAT_319_TO_288_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x000000b4 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_12 |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  CHK_USR_PAT_31_TO_0                                |
 | +-------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_CHK_USR_PAT_31_TO_0_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CHK_USR_PAT_31_TO_0_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_CHK_USR_PAT_31_TO_0_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_CHK_USR_PAT_31_TO_0_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x000000b8 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_13 |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  CHK_USR_PAT_63_TO_32                               |
 | +-------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_CHK_USR_PAT_63_TO_32_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CHK_USR_PAT_63_TO_32_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_CHK_USR_PAT_63_TO_32_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_CHK_USR_PAT_63_TO_32_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x000000bc CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_14 |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  CHK_USR_PAT_95_TO_64                               |
 | +-------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_CHK_USR_PAT_95_TO_64_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CHK_USR_PAT_95_TO_64_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_CHK_USR_PAT_95_TO_64_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_CHK_USR_PAT_95_TO_64_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x000000c0 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_15 |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  CHK_USR_PAT_127_TO_96                              |
 | +-------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_CHK_USR_PAT_127_TO_96_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CHK_USR_PAT_127_TO_96_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_CHK_USR_PAT_127_TO_96_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_CHK_USR_PAT_127_TO_96_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x000000c4 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_16 |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  CHK_USR_PAT_159_TO_128                             |
 | +-------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_CHK_USR_PAT_159_TO_128_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CHK_USR_PAT_159_TO_128_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_CHK_USR_PAT_159_TO_128_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_CHK_USR_PAT_159_TO_128_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x000000c8 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_17 |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  CHK_USR_PAT_191_TO_160                             |
 | +-------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_CHK_USR_PAT_191_TO_160_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CHK_USR_PAT_191_TO_160_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_CHK_USR_PAT_191_TO_160_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_CHK_USR_PAT_191_TO_160_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x000000cc CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_18 |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  CHK_USR_PAT_223_TO_192                             |
 | +-------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_CHK_USR_PAT_223_TO_192_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CHK_USR_PAT_223_TO_192_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_CHK_USR_PAT_223_TO_192_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_CHK_USR_PAT_223_TO_192_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x000000d0 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_19 |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  CHK_USR_PAT_255_TO_224                             |
 | +-------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_CHK_USR_PAT_255_TO_224_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CHK_USR_PAT_255_TO_224_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_CHK_USR_PAT_255_TO_224_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_CHK_USR_PAT_255_TO_224_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x000000d4 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_20 |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  CHK_USR_PAT_287_TO_256                             |
 | +-------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_CHK_USR_PAT_287_TO_256_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CHK_USR_PAT_287_TO_256_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_CHK_USR_PAT_287_TO_256_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_CHK_USR_PAT_287_TO_256_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x000000d8 CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_21 |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  CHK_USR_PAT_319_TO_288                             |
 | +-------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_CHK_USR_PAT_319_TO_288_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_CHK_USR_PAT_319_TO_288_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_CHK_USR_PAT_319_TO_288_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_CHK_USR_PAT_319_TO_288_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x000000dc CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_22 |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R  CHK_ERR_CNT                                          |
 | +-------------------------------------------------------------------+
 | 
 | UINT32 s16_channel_pcbi_field_CHK_ERR_CNT_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_range_CHK_ERR_CNT_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x000000e0 CHANNEL_PCBI_TX_CFG_1 |
 | +-------------------------------------------+
 | | bit  20:16 R/W  RX_TX_DESKEW_AF           |
 | | bit  15:11 R/W  RX_TX_DESKEW_AE           |
 | | bit  10:8  R/W  TX_RX_DESKEW_SYNC_VAL     |
 | | bit  7     R/W  TX_RX_DESKEW_SYNC         |
 | | bit  6:4   R/W  RX_TX_DESKEW_SYNC_VAL     |
 | | bit  3     R/W  RX_TX_DESKEW_SYNC         |
 | | bit  2     R/W  LOOPBACK_TX_RX_EN         |
 | | bit  1:0   R/W  TX_SOURCE                 |
 | +-------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_RX_TX_DESKEW_AF_set             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RX_TX_DESKEW_AF_get             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_RX_TX_DESKEW_AF_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_RX_TX_DESKEW_AF_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_RX_TX_DESKEW_AE_set             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RX_TX_DESKEW_AE_get             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_RX_TX_DESKEW_AE_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_RX_TX_DESKEW_AE_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_TX_RX_DESKEW_SYNC_VAL_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TX_RX_DESKEW_SYNC_VAL_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_TX_RX_DESKEW_SYNC_VAL_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_TX_RX_DESKEW_SYNC_VAL_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_TX_RX_DESKEW_SYNC_set           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TX_RX_DESKEW_SYNC_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RX_TX_DESKEW_SYNC_VAL_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RX_TX_DESKEW_SYNC_VAL_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_RX_TX_DESKEW_SYNC_VAL_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_RX_TX_DESKEW_SYNC_VAL_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_RX_TX_DESKEW_SYNC_set           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RX_TX_DESKEW_SYNC_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_LOOPBACK_TX_RX_EN_set           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_LOOPBACK_TX_RX_EN_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_TX_SOURCE_set                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TX_SOURCE_get                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_TX_SOURCE_set             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_TX_SOURCE_get             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------+
 | | Register 0x000000e4 CHANNEL_PCBI_PGA_COMMON_MODE_CALIBRATION_CFG_1 |
 | +--------------------------------------------------------------------+
 | | bit  13   R/W  TPD_CTRL_OVR_EN                                     |
 | | bit  12   R/W  TPD_CTRL_OVR                                        |
 | | bit  11:8 R/W  PGA_CTRL                                            |
 | | bit  7    R    PGA_NS_SENSE                                        |
 | | bit  6    R/W  PGA_NS_SENSE_ENB                                    |
 | | bit  5:1  R/W  PGA_NS_REF                                          |
 | | bit  0    R/W  PGA_NS_SEL                                          |
 | +--------------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_TPD_CTRL_OVR_EN_set  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TPD_CTRL_OVR_EN_get  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_TPD_CTRL_OVR_set     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TPD_CTRL_OVR_get     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_PGA_CTRL_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PGA_CTRL_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_PGA_CTRL_set   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_PGA_CTRL_get   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_PGA_NS_SENSE_ENB_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PGA_NS_SENSE_ENB_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_PGA_NS_REF_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PGA_NS_REF_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_PGA_NS_REF_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_PGA_NS_REF_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_PGA_NS_SEL_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PGA_NS_SEL_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_PGA_NS_SENSE_get     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------------------------+
 | | Register 0x000000e8 CHANNEL_PCBI_PGA_LUT_OVERRIDE_CFG_1 |
 | +---------------------------------------------------------+
 | | bit  21    R/W  PGA_LUT_PCBI_OVR_EN                     |
 | | bit  20:18 R/W  PGA_CD_S1_OVR                           |
 | | bit  17:15 R/W  PGA_CD_S2_OVR                           |
 | | bit  14:10 R/W  PGA_RG_S1_OVR                           |
 | | bit  9:6   R/W  PGA_RG_S2_OVR                           |
 | | bit  5:3   R/W  PGA_CG_S1_OVR                           |
 | | bit  2:0   R/W  PGA_CG_S2_OVR                           |
 | +---------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_PGA_LUT_PCBI_OVR_EN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PGA_LUT_PCBI_OVR_EN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_PGA_CD_S1_OVR_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PGA_CD_S1_OVR_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_PGA_CD_S1_OVR_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_PGA_CD_S1_OVR_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_PGA_CD_S2_OVR_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PGA_CD_S2_OVR_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_PGA_CD_S2_OVR_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_PGA_CD_S2_OVR_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_PGA_RG_S1_OVR_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PGA_RG_S1_OVR_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_PGA_RG_S1_OVR_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_PGA_RG_S1_OVR_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_PGA_RG_S2_OVR_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PGA_RG_S2_OVR_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_PGA_RG_S2_OVR_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_PGA_RG_S2_OVR_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_PGA_CG_S1_OVR_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PGA_CG_S1_OVR_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_PGA_CG_S1_OVR_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_PGA_CG_S1_OVR_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_PGA_CG_S2_OVR_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PGA_CG_S2_OVR_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_PGA_CG_S2_OVR_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_PGA_CG_S2_OVR_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x000000ec CHANNEL_PCBI_MABC_CFG_1 |
 | +---------------------------------------------+
 | | bit  18:17 R/W  ADC_CM_SEL                  |
 | | bit  16    R/W  ADC_BYPASS_DIV8             |
 | | bit  15    R/W  ADC_BYPASS_DIV4             |
 | | bit  14:13 R/W  T_PISO_MAIN_EN              |
 | | bit  12    R/W  RX_CMCOR_CTRL               |
 | | bit  11    R/W  T_TEST_EN                   |
 | | bit  10    R/W  T_PISO_DLB_EN               |
 | | bit  9     R/W  ADC_DLB_EN                  |
 | | bit  8     R/W  PGA_MLB_ENB                 |
 | | bit  7:2   R/W  TRS_TEST_MODE               |
 | | bit  1     R/W  TRS_ENB                     |
 | | bit  0     R/W  RX_CM_ENB                   |
 | +---------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_ADC_CM_SEL_set           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_CM_SEL_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_ADC_CM_SEL_set     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_ADC_CM_SEL_get     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_ADC_BYPASS_DIV8_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_BYPASS_DIV8_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_ADC_BYPASS_DIV4_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_BYPASS_DIV4_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_T_PISO_MAIN_EN_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_T_PISO_MAIN_EN_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_T_PISO_MAIN_EN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_T_PISO_MAIN_EN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_RX_CMCOR_CTRL_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RX_CMCOR_CTRL_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_T_TEST_EN_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_T_TEST_EN_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_T_PISO_DLB_EN_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_T_PISO_DLB_EN_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_ADC_DLB_EN_set           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_DLB_EN_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_PGA_MLB_ENB_set          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PGA_MLB_ENB_get          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_TRS_TEST_MODE_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TRS_TEST_MODE_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_TRS_TEST_MODE_set  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_TRS_TEST_MODE_get  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_TRS_ENB_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TRS_ENB_get              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RX_CM_ENB_set            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RX_CM_ENB_get            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------------+
 | | Register 0x000000f0 CHANNEL_PCBI_SIPO_BIST_CFG_1 |
 | +--------------------------------------------------+
 | | bit  4 R    SIPO_BIST_TIMING_DONE                |
 | | bit  3 R    SIPO_BIST_DATA_DONE                  |
 | | bit  2 R/W  SIPO_BIST_START                      |
 | | bit  1 R/W  ADC_BIST                             |
 | | bit  0 R/W  SIPO_BIST_DATA_TIME_SEL              |
 | +--------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_SIPO_BIST_START_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_SIPO_BIST_START_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_ADC_BIST_set                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_BIST_get                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_SIPO_BIST_DATA_TIME_SEL_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_SIPO_BIST_DATA_TIME_SEL_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_SIPO_BIST_TIMING_DONE_get   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_SIPO_BIST_DATA_DONE_get     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------------+
 | | Register 0x000000f4 CHANNEL_PCBI_SIPO_BIST_CFG_2 |
 | +--------------------------------------------------+
 | | bit  31:0 R  SIPO_BIST_DATA_FAIL                 |
 | +--------------------------------------------------+
 | 
 | UINT32 s16_channel_pcbi_field_SIPO_BIST_DATA_FAIL_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_range_SIPO_BIST_DATA_FAIL_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x000000f8 CHANNEL_PCBI_SIPO_BIST_CFG_3 |
 | +--------------------------------------------------+
 | | bit  31:0 R  SIPO_BIST_TIMING_FAIL               |
 | +--------------------------------------------------+
 | 
 | UINT32 s16_channel_pcbi_field_SIPO_BIST_TIMING_FAIL_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_range_SIPO_BIST_TIMING_FAIL_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x000000fc CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_1 |
 | +-----------------------------------------------------------+
 | | bit  27    R    OCD_CM                                    |
 | | bit  26    R    ROP_RECALCUL_DONE                         |
 | | bit  25    R    ROP_OVERFLOW                              |
 | | bit  24    R    ROP_CAL_DONE                              |
 | | bit  23    R    VGP_OVERFLOW                              |
 | | bit  22    R    VGP_OCC_DONE                              |
 | | bit  21:14 R/W  VGP_UPPERLIMIT                            |
 | | bit  13:12 R/W  TX_EMI_CALIB_OCC_DONE_GEN_SEL             |
 | | bit  11    R/W  VGP_CLR_OCDAC                             |
 | | bit  10    R/W  VGP_OCC_CONT                              |
 | | bit  9     R/W  VGP_OCC_ENABLE_OVR_EN                     |
 | | bit  8     R/W  VGP_OCC_ENABLE_OVR                        |
 | | bit  7     R/W  VGP_OCC_START_OVR_EN                      |
 | | bit  6     R/W  VGP_OCC_START_OVR                         |
 | | bit  5     R/W  RECALCUL_OVR_EN                           |
 | | bit  4     R/W  RECALCUL_OVR                              |
 | | bit  3     R/W  START_ROPRON_CAL_OVR_EN                   |
 | | bit  2     R/W  START_ROPRON_CAL_OVR                      |
 | | bit  1     R/W  VGP_OCD_CM_INVERT                         |
 | | bit  0     R/W  ROP_OCD_CM_INVERT                         |
 | +-----------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_VGP_UPPERLIMIT_set                      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_VGP_UPPERLIMIT_get                      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_VGP_UPPERLIMIT_set                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_VGP_UPPERLIMIT_get                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_TX_EMI_CALIB_OCC_DONE_GEN_SEL_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TX_EMI_CALIB_OCC_DONE_GEN_SEL_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_TX_EMI_CALIB_OCC_DONE_GEN_SEL_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_TX_EMI_CALIB_OCC_DONE_GEN_SEL_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_VGP_CLR_OCDAC_set                       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_VGP_CLR_OCDAC_get                       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_VGP_OCC_CONT_set                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_VGP_OCC_CONT_get                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_VGP_OCC_ENABLE_OVR_EN_set               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_VGP_OCC_ENABLE_OVR_EN_get               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_VGP_OCC_ENABLE_OVR_set                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_VGP_OCC_ENABLE_OVR_get                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_VGP_OCC_START_OVR_EN_set                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_VGP_OCC_START_OVR_EN_get                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_VGP_OCC_START_OVR_set                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_VGP_OCC_START_OVR_get                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RECALCUL_OVR_EN_set                     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RECALCUL_OVR_EN_get                     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RECALCUL_OVR_set                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RECALCUL_OVR_get                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_START_ROPRON_CAL_OVR_EN_set             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_START_ROPRON_CAL_OVR_EN_get             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_START_ROPRON_CAL_OVR_set                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_START_ROPRON_CAL_OVR_get                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_VGP_OCD_CM_INVERT_set                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_VGP_OCD_CM_INVERT_get                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_ROP_OCD_CM_INVERT_set                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ROP_OCD_CM_INVERT_get                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_OCD_CM_get                              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_ROP_RECALCUL_DONE_get                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_ROP_OVERFLOW_get                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_ROP_CAL_DONE_get                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_VGP_OVERFLOW_get                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_VGP_OCC_DONE_get                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00000100 CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_2 |
 | +-----------------------------------------------------------+
 | | bit  30:16 R/W  TX_EMI_CALIB_OC_TOLERANCE                 |
 | | bit  15:0  R/W  TX_EMI_CALIB_OC_INTERVAL                  |
 | +-----------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_TX_EMI_CALIB_OC_TOLERANCE_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TX_EMI_CALIB_OC_TOLERANCE_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_TX_EMI_CALIB_OC_TOLERANCE_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_TX_EMI_CALIB_OC_TOLERANCE_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_TX_EMI_CALIB_OC_INTERVAL_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TX_EMI_CALIB_OC_INTERVAL_get        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_TX_EMI_CALIB_OC_INTERVAL_set  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_TX_EMI_CALIB_OC_INTERVAL_get  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00000104 CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_3 |
 | +-----------------------------------------------------------+
 | | bit  19:0 R/W  PWRDATA                                    |
 | +-----------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_PWRDATA_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PWRDATA_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_PWRDATA_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_PWRDATA_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00000108 CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_4 |
 | +-----------------------------------------------------------+
 | | bit  19:10 R    PGENERIC_DATA_OUT                         |
 | | bit  9:6   R/W  PRDLOCK                                   |
 | | bit  5:0   R/W  PGENERIC_RD_ADD                           |
 | +-----------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_PRDLOCK_set                 ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PRDLOCK_get                 ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_PRDLOCK_set           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_PRDLOCK_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_PGENERIC_RD_ADD_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PGENERIC_RD_ADD_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_PGENERIC_RD_ADD_set   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_PGENERIC_RD_ADD_get   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 s16_channel_pcbi_field_PGENERIC_DATA_OUT_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_channel_pcbi_field_range_PGENERIC_DATA_OUT_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x0000010c CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_5 |
 | +-----------------------------------------------------------+
 | | bit  29:20 R/W  P1AD1D_IN                                 |
 | | bit  19:10 R/W  P1BD2D_IN                                 |
 | | bit  9:0   R/W  P1AD2D_IN                                 |
 | +-----------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_P1AD1D_IN_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P1AD1D_IN_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P1AD1D_IN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P1AD1D_IN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P1BD2D_IN_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P1BD2D_IN_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P1BD2D_IN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P1BD2D_IN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P1AD2D_IN_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P1AD2D_IN_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P1AD2D_IN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P1AD2D_IN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00000110 CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_6 |
 | +-----------------------------------------------------------+
 | | bit  29:20 R/W  P2BD2D_IN                                 |
 | | bit  19:10 R/W  P2AD2D_IN                                 |
 | | bit  9:0   R/W  P1BD1D_IN                                 |
 | +-----------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_P2BD2D_IN_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P2BD2D_IN_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P2BD2D_IN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P2BD2D_IN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P2AD2D_IN_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P2AD2D_IN_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P2AD2D_IN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P2AD2D_IN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P1BD1D_IN_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P1BD1D_IN_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P1BD1D_IN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P1BD1D_IN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00000114 CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_7 |
 | +-----------------------------------------------------------+
 | | bit  19:10 R/W  P2BD1D_IN                                 |
 | | bit  9:0   R/W  P2AD1D_IN                                 |
 | +-----------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_P2BD1D_IN_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P2BD1D_IN_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P2BD1D_IN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P2BD1D_IN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_P2AD1D_IN_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_P2AD1D_IN_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_P2AD1D_IN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_P2AD1D_IN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register 0x00000118 CHANNEL_PCBI_ANALOG_TESTBUS_MODE_CFG_1 |
 | +------------------------------------------------------------+
 | | bit  16   R/W  TRS_CTRL_0                                  |
 | | bit  13   R/W  TRCS_ATMSB                                  |
 | | bit  12:8 R/W  ATIN                                        |
 | | bit  7    R/W  PI_I_ATMSB                                  |
 | | bit  6    R/W  PI_Q_ATMSB                                  |
 | | bit  5    R/W  RX_ATMSB                                    |
 | | bit  4    R/W  TRS_ATMSB                                   |
 | | bit  3    R/W  T_ATMSB                                     |
 | | bit  2    R/W  PI_ATMSB                                    |
 | | bit  1    R/W  PGA_ATMSB                                   |
 | | bit  0    R/W  ADC_ATMSB                                   |
 | +------------------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_TRS_CTRL_0_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TRS_CTRL_0_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_TRCS_ATMSB_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TRCS_ATMSB_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_ATIN_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ATIN_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_ATIN_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_ATIN_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_PI_I_ATMSB_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PI_I_ATMSB_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_PI_Q_ATMSB_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PI_Q_ATMSB_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RX_ATMSB_set   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RX_ATMSB_get   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_TRS_ATMSB_set  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TRS_ATMSB_get  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_T_ATMSB_set    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_T_ATMSB_get    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_PI_ATMSB_set   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PI_ATMSB_get   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_PGA_ATMSB_set  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PGA_ATMSB_get  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_ADC_ATMSB_set  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_ATMSB_get  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------------+
 | | Register 0x0000011c CHANNEL_PCBI_RESET_CFG_1 |
 | +----------------------------------------------+
 | | bit  17  R/W  DIAGNOSTICS_SW_RESET           |
 | | bit  16  R/W  SSI_SW_RESET                   |
 | | bit  15  R/W  OCC_TX_RX_SW_RESET             |
 | | bit  14  R/W  PATT_CHK_SW_RESET              |
 | | bit  13  R/W  PATT_GEN_SW_RESET              |
 | | bit  12  R/W  TX_EMI_CAL_SW_RESET            |
 | | bit  11  R/W  SIPO_BIST_SW_RESET             |
 | | bit  10  R/W  ADC_OC_SW_RESET                |
 | | bit  9   R/W  TX_LANE_INVERT                 |
 | | bit  8   R/W  RX_LANE_INVERT                 |
 | | bit  7:6 R/W  MTSB_BUS_WIDTH                 |
 | | bit  5   R/W  PI_RSTB                        |
 | | bit  3   R/W  T_PISO_RSTB                    |
 | | bit  2   R/W  T_EMI_RSTB                     |
 | | bit  1   R/W  ADC_RSTB                       |
 | | bit  0   R/W  ADC_DAC_RSTB                   |
 | +----------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_DIAGNOSTICS_SW_RESET_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_DIAGNOSTICS_SW_RESET_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_SSI_SW_RESET_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_SSI_SW_RESET_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_OCC_TX_RX_SW_RESET_set   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_OCC_TX_RX_SW_RESET_get   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_PATT_CHK_SW_RESET_set    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PATT_CHK_SW_RESET_get    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_PATT_GEN_SW_RESET_set    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PATT_GEN_SW_RESET_get    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_TX_EMI_CAL_SW_RESET_set  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TX_EMI_CAL_SW_RESET_get  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_SIPO_BIST_SW_RESET_set   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_SIPO_BIST_SW_RESET_get   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_ADC_OC_SW_RESET_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_OC_SW_RESET_get      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_TX_LANE_INVERT_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_TX_LANE_INVERT_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_RX_LANE_INVERT_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_RX_LANE_INVERT_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_MTSB_BUS_WIDTH_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_MTSB_BUS_WIDTH_get       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_range_MTSB_BUS_WIDTH_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_channel_pcbi_field_range_MTSB_BUS_WIDTH_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_channel_pcbi_field_PI_RSTB_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PI_RSTB_get              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_T_PISO_RSTB_set          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_T_PISO_RSTB_get          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_T_EMI_RSTB_set           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_T_EMI_RSTB_get           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_ADC_RSTB_set             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_RSTB_get             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_ADC_DAC_RSTB_set         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_ADC_DAC_RSTB_get         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000120 CHANNEL_PCBI_LANE_N_STAT_INT_EN |
 | +-----------------------------------------------------+
 | | bit  2 R/W  LOS_E                                   |
 | | bit  1 R/W  SSI_E                                   |
 | | bit  0 R/W  PATT_GEN_MON_CHK_ERROR_E                |
 | +-----------------------------------------------------+
 | 
 | void   s16_channel_pcbi_field_LOS_E_set                    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_LOS_E_get                    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_SSI_E_set                    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_SSI_E_get                    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_channel_pcbi_field_PATT_GEN_MON_CHK_ERROR_E_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_channel_pcbi_field_PATT_GEN_MON_CHK_ERROR_E_get ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------------------+
 | | Register 0x00000124 CHANNEL_PCBI_LANE_N_STAT_INT_EVENT |
 | +--------------------------------------------------------+
 | | bit  2 R/W  LOS_I                                      |
 | | bit  1 R/W  SSI_I                                      |
 | | bit  0 R/W  PATT_GEN_MON_CHK_ERROR_I                   |
 | +--------------------------------------------------------+
 | 
 | void                 s16_channel_pcbi_field_LOS_I_set_to_clear                    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_channel_pcbi_field_LOS_I_get                             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_channel_pcbi_field_LOS_I_poll                            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_channel_pcbi_field_SSI_I_set_to_clear                    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_channel_pcbi_field_SSI_I_get                             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_channel_pcbi_field_SSI_I_poll                            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_channel_pcbi_field_PATT_GEN_MON_CHK_ERROR_I_set_to_clear ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_channel_pcbi_field_PATT_GEN_MON_CHK_ERROR_I_get          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_channel_pcbi_field_PATT_GEN_MON_CHK_ERROR_I_poll         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000128 CHANNEL_PCBI_LANE_N_STAT_INT |
 | +--------------------------------------------------+
 | | bit  2 R  LOS_V                                  |
 | | bit  1 R  SSI_V                                  |
 | | bit  0 R  PATT_GEN_MON_CHK_ERROR_V               |
 | +--------------------------------------------------+
 | 
 | UINT32               s16_channel_pcbi_field_LOS_V_get                     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_channel_pcbi_field_LOS_V_poll                    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_channel_pcbi_field_SSI_V_get                     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_channel_pcbi_field_SSI_V_poll                    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_channel_pcbi_field_PATT_GEN_MON_CHK_ERROR_V_get  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_channel_pcbi_field_PATT_GEN_MON_CHK_ERROR_V_poll ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _S16_CHANNEL_PCBI_IO_H
#define _S16_CHANNEL_PCBI_IO_H

#include "s16_loc.h"
#include "s16_channel_pcbi_regs.h"
#include "s16_channel_pcbi_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 s16_channel_pcbi_buffer_init               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void                 s16_channel_pcbi_buffer_flush              ( s16_channel_pcbi_buffer_t *b_ptr )
 | UINT32               s16_channel_pcbi_reg_read                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 s16_channel_pcbi_reg_write                 ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 s16_channel_pcbi_field_set                 ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 s16_channel_pcbi_action_on_write_field_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 s16_channel_pcbi_burst_read                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 s16_channel_pcbi_burst_write               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE s16_channel_pcbi_poll                      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_RATE_ADAPT_FIFO_CFG_1_write                     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_RATE_ADAPT_FIFO_CFG_1_field_set                 ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_RATE_ADAPT_FIFO_CFG_1_read                      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_1_write                         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_1_field_set                     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_1_read                          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_2_write                         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_2_field_set                     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_2_read                          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_3_write                         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_3_field_set                     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_3_read                          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_4_write                         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_4_field_set                     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_4_read                          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_5_write                         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_5_field_set                     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_5_read                          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_6_write                         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_6_field_set                     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_DATA_CFG_6_read                          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_TIMING_CFG_1_write                       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_TIMING_CFG_1_field_set                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_TIMING_CFG_1_read                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_TIMING_CFG_2_write                       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_TIMING_CFG_2_field_set                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_TIMING_CFG_2_read                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_TIMING_CFG_3_write                       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_TIMING_CFG_3_field_set                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_TIMING_CFG_3_read                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_TIMING_CFG_4_write                       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_TIMING_CFG_4_field_set                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_TIMING_CFG_4_read                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_GENERAL_CFG_1_write                      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_GENERAL_CFG_1_field_set                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_GENERAL_CFG_1_read                       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_GENERAL_CFG_2_write                      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_GENERAL_CFG_2_field_set                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_GENERAL_CFG_2_read                       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_RAM_CAPTURE_CFG_1_write                         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_RAM_CAPTURE_CFG_1_field_set                     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_RAM_CAPTURE_CFG_1_read                          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_1_write                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_1_field_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_1_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_2_write                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_2_field_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_2_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_3_write                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_3_field_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_3_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_SSI_CFG_1_write                                 ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_SSI_CFG_1_field_set                             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_SSI_CFG_1_read                                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_LOS_CFG_1_write                                 ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_LOS_CFG_1_field_set                             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_LOS_CFG_1_read                                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_DCD_OFFSET_CORRECTION_CFG_1_write            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_DCD_OFFSET_CORRECTION_CFG_1_field_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_DCD_OFFSET_CORRECTION_CFG_1_read             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_DCD_OFFSET_CORRECTION_CFG_2_write            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_DCD_OFFSET_CORRECTION_CFG_2_field_set        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_DCD_OFFSET_CORRECTION_CFG_2_read             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_DCD_OFFSET_CORR_CFG_3_write                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_DCD_OFFSET_CORR_CFG_3_field_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_DCD_OFFSET_CORR_CFG_3_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_DCD_OFFSET_CORR_CFG_4_write                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_DCD_OFFSET_CORR_CFG_4_field_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_DCD_OFFSET_CORR_CFG_4_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_CTRL_LOGIC_CFG_1_write                       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_CTRL_LOGIC_CFG_1_field_set                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_CTRL_LOGIC_CFG_1_read                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_CTRL_LOGIC_CFG_2_write                       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_CTRL_LOGIC_CFG_2_field_set                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_CTRL_LOGIC_CFG_2_read                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_1_write           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_1_field_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_1_read            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_2_write           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_2_field_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_2_read            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_3_write           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_3_field_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_3_read            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_4_write           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_4_field_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_4_read            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_5_write           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_5_field_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_5_read            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_6_write           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_6_field_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_6_read            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_7_write           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_7_field_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_7_read            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_8_write           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_8_field_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_8_read            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_9_write           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_9_field_set       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_9_read            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_10_write          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_10_field_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_10_read           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_11_write          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_11_field_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_11_read           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_12_write          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_12_field_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_12_read           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_13_write          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_13_field_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_13_read           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_14_write          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_14_field_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_14_read           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_15_write          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_15_field_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_15_read           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_16_write          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_16_field_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_16_read           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_17_write          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_17_field_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_17_read           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_18_write          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_18_field_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_18_read           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_19_write          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_19_field_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_19_read           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_20_write          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_20_field_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_20_read           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_21_write          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_21_field_set      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_21_read           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_CFG_1_write                                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_CFG_1_field_set                              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_CFG_1_read                                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PGA_COMMON_MODE_CALIBRATION_CFG_1_write         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PGA_COMMON_MODE_CALIBRATION_CFG_1_field_set     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PGA_COMMON_MODE_CALIBRATION_CFG_1_read          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PGA_LUT_OVERRIDE_CFG_1_write                    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_PGA_LUT_OVERRIDE_CFG_1_field_set                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PGA_LUT_OVERRIDE_CFG_1_read                     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_MABC_CFG_1_write                                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_MABC_CFG_1_field_set                            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_MABC_CFG_1_read                                 ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_SIPO_BIST_CFG_1_write                           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_SIPO_BIST_CFG_1_field_set                       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_SIPO_BIST_CFG_1_read                            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_1_write                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_1_field_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_1_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_2_write                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_2_field_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_2_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_3_write                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_3_field_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_3_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_4_write                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_4_field_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_4_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_5_write                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_5_field_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_5_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_6_write                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_6_field_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_6_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_7_write                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_7_field_set              ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_EMI_CALIBRATION_CFG_7_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ANALOG_TESTBUS_MODE_CFG_1_write                 ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_ANALOG_TESTBUS_MODE_CFG_1_field_set             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_ANALOG_TESTBUS_MODE_CFG_1_read                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_RESET_CFG_1_write                               ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_RESET_CFG_1_field_set                           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_RESET_CFG_1_read                                ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_LANE_N_STAT_INT_EVENT_write                     ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_LANE_N_STAT_INT_EVENT_action_on_write_field_set ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_LANE_N_STAT_INT_EVENT_read                      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_channel_pcbi_reg_CHANNEL_PCBI_LANE_N_STAT_INT_EVENT_poll                      ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_LANE_N_STAT_INT_EN_write                        ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_channel_pcbi_reg_CHANNEL_PCBI_LANE_N_STAT_INT_EN_field_set                    ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_LANE_N_STAT_INT_EN_read                         ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_LANE_N_STAT_INT_read                            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_channel_pcbi_reg_CHANNEL_PCBI_LANE_N_STAT_INT_poll                            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_MDSP_CFG_1_read                                 ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_ADC_OC_GENERAL_CFG_3_read                       ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_RAM_CAPTURE_CFG_2_read                          ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_4_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_5_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_VERTICAL_HISTOGRAM_CFG_6_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_SSI_CFG_2_read                                  ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_DCD_OFFSET_CORRECTION_CFG_5_read             ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_TX_DCD_OFFSET_CORR_CFG_6_read                   ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_PATTERN_GENERATOR_AND_MON_CFG_22_read           ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_SIPO_BIST_CFG_2_read                            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_channel_pcbi_reg_CHANNEL_PCBI_SIPO_BIST_CFG_3_read                            ( s16_channel_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _S16_CHANNEL_PCBI_IO_H */
