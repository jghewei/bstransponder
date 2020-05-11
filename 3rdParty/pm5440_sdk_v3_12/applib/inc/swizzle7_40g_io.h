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
 *     and register accessor functions for the swizzle7_40g block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing swizzle7_40g io functions is:
 | {
 |     swizzle7_40g_buffer_t b_ptr[1];
 |     swizzle7_40g_buffer_init( b_ptr, h_ptr, A );
 |     value = swizzle7_40g_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     swizzle7_40g_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     swizzle7_40g_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = swizzle7_40g_field_<FIELD>_get( NULL, h_ptr, A );
 | swizzle7_40g_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------+
 | | Register 0x00020000 DECODER_CFG |
 | +---------------------------------+
 | | bit  7:1 R/W  LIMIT_LATENCY     |
 | | bit  0   R/W  DEC_DISABLE       |
 | +---------------------------------+
 | 
 | void   swizzle7_40g_field_LIMIT_LATENCY_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_LIMIT_LATENCY_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_LIMIT_LATENCY_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_LIMIT_LATENCY_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   swizzle7_40g_field_DEC_DISABLE_set         ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_DEC_DISABLE_get         ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------------+
 | | Register 0x00020020 DIAG_DECODER_CFG_1_2 |
 | +------------------------------------------+
 | | bit  16    R/W  FRAME_COMMIT_SEL         |
 | | bit  15    R/W  LONG_TRIGGER_MODE        |
 | | bit  14    R/W  TRAP_ENABLE              |
 | | bit  13    R/W  TRAP_TYPE                |
 | | bit  12    R/W  PDF_CAPTURE_ENABLE       |
 | | bit  11:10 R/W  STAT_SEL                 |
 | | bit  9     R/W  DISABLE_PRIORITY         |
 | | bit  8:1   R/W  STATISTICS_FRAMES        |
 | | bit  0     R/W  DEC_DISABLE_RESYNC       |
 | +------------------------------------------+
 | 
 | void   swizzle7_40g_field_FRAME_COMMIT_SEL_set        ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_FRAME_COMMIT_SEL_get        ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_LONG_TRIGGER_MODE_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_LONG_TRIGGER_MODE_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_TRAP_ENABLE_set             ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_TRAP_ENABLE_get             ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_TRAP_TYPE_set               ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_TRAP_TYPE_get               ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_PDF_CAPTURE_ENABLE_set      ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_PDF_CAPTURE_ENABLE_get      ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_STAT_SEL_set                ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_STAT_SEL_get                ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_STAT_SEL_set          ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_STAT_SEL_get          ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   swizzle7_40g_field_DISABLE_PRIORITY_set        ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_DISABLE_PRIORITY_get        ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_STATISTICS_FRAMES_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_STATISTICS_FRAMES_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_STATISTICS_FRAMES_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_STATISTICS_FRAMES_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   swizzle7_40g_field_DEC_DISABLE_RESYNC_set      ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_DEC_DISABLE_RESYNC_get      ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------------+
 | | Register 0x00020024 DIAG_DECODER_CFG_2_2 |
 | +------------------------------------------+
 | | bit  15:0 R/W  REPROCESS_CONTROL         |
 | +------------------------------------------+
 | 
 | void   swizzle7_40g_field_REPROCESS_CONTROL_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_REPROCESS_CONTROL_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_REPROCESS_CONTROL_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_REPROCESS_CONTROL_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x00020080 CONFIDENCE_FOR_T6_CODEWORDS_1_2_ |
 | +------------------------------------------------------+
 | | bit  30:24 R/W  CONF_T6_3                            |
 | | bit  22:16 R/W  CONF_T6_2                            |
 | | bit  14:8  R/W  CONF_T6_1                            |
 | | bit  6:0   R/W  CONF_T6_0                            |
 | +------------------------------------------------------+
 | 
 | void   swizzle7_40g_field_CONF_T6_3_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_CONF_T6_3_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_CONF_T6_3_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_CONF_T6_3_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   swizzle7_40g_field_CONF_T6_2_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_CONF_T6_2_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_CONF_T6_2_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_CONF_T6_2_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   swizzle7_40g_field_CONF_T6_1_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_CONF_T6_1_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_CONF_T6_1_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_CONF_T6_1_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   swizzle7_40g_field_CONF_T6_0_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_CONF_T6_0_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_CONF_T6_0_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_CONF_T6_0_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x00020084 CONFIDENCE_FOR_T6_CODEWORDS_2_2_ |
 | +------------------------------------------------------+
 | | bit  22:16 R/W  CONF_T6_6                            |
 | | bit  14:8  R/W  CONF_T6_5                            |
 | | bit  6:0   R/W  CONF_T6_4                            |
 | +------------------------------------------------------+
 | 
 | void   swizzle7_40g_field_CONF_T6_6_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_CONF_T6_6_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_CONF_T6_6_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_CONF_T6_6_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   swizzle7_40g_field_CONF_T6_5_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_CONF_T6_5_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_CONF_T6_5_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_CONF_T6_5_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   swizzle7_40g_field_CONF_T6_4_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_CONF_T6_4_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_CONF_T6_4_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_CONF_T6_4_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x00020088 CONFIDENCE_FOR_T5_CODEWORDS_1_2_ |
 | +------------------------------------------------------+
 | | bit  30:24 R/W  CONF_T5_3                            |
 | | bit  22:16 R/W  CONF_T5_2                            |
 | | bit  14:8  R/W  CONF_T5_1                            |
 | | bit  6:0   R/W  CONF_T5_0                            |
 | +------------------------------------------------------+
 | 
 | void   swizzle7_40g_field_CONF_T5_3_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_CONF_T5_3_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_CONF_T5_3_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_CONF_T5_3_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   swizzle7_40g_field_CONF_T5_2_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_CONF_T5_2_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_CONF_T5_2_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_CONF_T5_2_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   swizzle7_40g_field_CONF_T5_1_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_CONF_T5_1_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_CONF_T5_1_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_CONF_T5_1_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   swizzle7_40g_field_CONF_T5_0_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_CONF_T5_0_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_CONF_T5_0_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_CONF_T5_0_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x0002008c CONFIDENCE_FOR_T5_CODEWORDS_2_2_ |
 | +------------------------------------------------------+
 | | bit  14:8 R/W  CONF_T5_5                             |
 | | bit  6:0  R/W  CONF_T5_4                             |
 | +------------------------------------------------------+
 | 
 | void   swizzle7_40g_field_CONF_T5_5_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_CONF_T5_5_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_CONF_T5_5_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_CONF_T5_5_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   swizzle7_40g_field_CONF_T5_4_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_CONF_T5_4_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_CONF_T5_4_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_CONF_T5_4_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------+
 | | Register 0x000200f0 FEC_CTRL |
 | +------------------------------+
 | | bit  4 R/W  FEC_RAM_LOWPWR   |
 | | bit  1 R/W  FEC_LOWPWR       |
 | | bit  0 R/W  FEC_SW_RST       |
 | +------------------------------+
 | 
 | void   swizzle7_40g_field_FEC_RAM_LOWPWR_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_FEC_RAM_LOWPWR_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_FEC_LOWPWR_set     ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_FEC_LOWPWR_get     ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_FEC_SW_RST_set     ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_FEC_SW_RST_get     ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------------+
 | | Register 0x00020180 LPA_STATISTICS_MODE_CFG |
 | +---------------------------------------------+
 | | bit  1:0 R/W  LPA_STATS_COUNTER_MODE        |
 | +---------------------------------------------+
 | 
 | void   swizzle7_40g_field_LPA_STATS_COUNTER_MODE_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_LPA_STATS_COUNTER_MODE_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_LPA_STATS_COUNTER_MODE_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_LPA_STATS_COUNTER_MODE_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register 0x00020190 LPA_STATISTICS_OTU_FRAME_PULSE_CNT |
 | +--------------------------------------------------------+
 | | bit  31:0 R  LPA_STATS_FP                              |
 | +--------------------------------------------------------+
 | 
 | UINT32 swizzle7_40g_field_LPA_STATS_FP_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 swizzle7_40g_field_range_LPA_STATS_FP_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register 0x00020194 LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT |
 | +--------------------------------------------------------------+
 | | bit  31:0 R  LPA_STATS_UNCORR_CWDS                           |
 | +--------------------------------------------------------------+
 | 
 | UINT32 swizzle7_40g_field_LPA_STATS_UNCORR_CWDS_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 swizzle7_40g_field_range_LPA_STATS_UNCORR_CWDS_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00020198 LPA_STATISTICS_CORRECTED_1S_CNT |
 | +-----------------------------------------------------+
 | | bit  31:0 R  LPA_STATS_CORR_1S                      |
 | +-----------------------------------------------------+
 | 
 | UINT32 swizzle7_40g_field_LPA_STATS_CORR_1S_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 swizzle7_40g_field_range_LPA_STATS_CORR_1S_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x0002019c LPA_STATISTICS_CORRECTED_0S_CNT |
 | +-----------------------------------------------------+
 | | bit  31:0 R  LPA_STATS_CORR_0S                      |
 | +-----------------------------------------------------+
 | 
 | UINT32 swizzle7_40g_field_LPA_STATS_CORR_0S_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 swizzle7_40g_field_range_LPA_STATS_CORR_0S_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x000201a0 LLSA_STATISTICS_MODE_CFG |
 | +----------------------------------------------+
 | | bit  2   R/W  LLSA_STATS_LATCH_SOURCE        |
 | | bit  1:0 R/W  LLSA_STATS_COUNTER_MODE        |
 | +----------------------------------------------+
 | 
 | void   swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_LLSA_STATS_LATCH_SOURCE_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_LLSA_STATS_COUNTER_MODE_set       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 swizzle7_40g_field_LLSA_STATS_COUNTER_MODE_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   swizzle7_40g_field_range_LLSA_STATS_COUNTER_MODE_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 swizzle7_40g_field_range_LLSA_STATS_COUNTER_MODE_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00020200 LLSA_STATISTICS_FAILURE_STAT |
 | +--------------------------------------------------+
 | | bit  0 R  LLSA_STATS_OTU_FAIL                    |
 | +--------------------------------------------------+
 | 
 | UINT32 swizzle7_40g_field_LLSA_STATS_OTU_FAIL_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------------------------------+
 | | Register 0x0002021c LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT |
 | +---------------------------------------------------------------+
 | | bit  31:0 R  LLSA_STATS_UNCORR_CWDS                           |
 | +---------------------------------------------------------------+
 | 
 | UINT32 swizzle7_40g_field_LLSA_STATS_UNCORR_CWDS_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32 swizzle7_40g_field_range_LLSA_STATS_UNCORR_CWDS_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------------------------+
 | | Register (0x00020240 + (N) * 4) LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19 |
 | |     N[0..19]                                                                       |
 | +------------------------------------------------------------------------------------+
 | | bit  31:0 R  LLSA_STATS_LANE_CORR_1S                                               |
 | +------------------------------------------------------------------------------------+
 | 
 | void   swizzle7_40g_lfield_LLSA_STATS_LANE_CORR_1S_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value[20] )
 | UINT32 swizzle7_40g_lfield_range_LLSA_STATS_LANE_CORR_1S_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------------------------+
 | | Register (0x00020300 + (N) * 4) LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19 |
 | |     N[0..19]                                                                       |
 | +------------------------------------------------------------------------------------+
 | | bit  31:0 R  LLSA_STATS_LANE_CORR_0S                                               |
 | +------------------------------------------------------------------------------------+
 | 
 | void   swizzle7_40g_lfield_LLSA_STATS_LANE_CORR_0S_get       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value[20] )
 | UINT32 swizzle7_40g_lfield_range_LLSA_STATS_LANE_CORR_0S_get ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SWIZZLE7_40G_IO_H
#define _SWIZZLE7_40G_IO_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "swizzle7_40g_regs.h"
#include "swizzle7_40g_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 swizzle7_40g_buffer_init               ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void                 swizzle7_40g_buffer_flush              ( swizzle7_40g_buffer_t *b_ptr )
 | UINT32               swizzle7_40g_reg_read                  ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 swizzle7_40g_reg_write                 ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 swizzle7_40g_field_set                 ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 swizzle7_40g_action_on_write_field_set ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 swizzle7_40g_burst_read                ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 swizzle7_40g_burst_write               ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE swizzle7_40g_poll                      ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   swizzle7_40g_reg_DECODER_CFG_write                                                   ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   swizzle7_40g_reg_DECODER_CFG_field_set                                               ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 swizzle7_40g_reg_DECODER_CFG_read                                                    ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_write                                          ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_field_set                                      ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 swizzle7_40g_reg_DIAG_DECODER_CFG_1_2_read                                           ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_write                                          ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_field_set                                      ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 swizzle7_40g_reg_DIAG_DECODER_CFG_2_2_read                                           ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__write                              ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__field_set                          ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_1_2__read                               ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__write                              ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__field_set                          ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 swizzle7_40g_reg_CONFIDENCE_FOR_T6_CODEWORDS_2_2__read                               ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__write                              ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__field_set                          ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_1_2__read                               ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__write                              ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__field_set                          ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 swizzle7_40g_reg_CONFIDENCE_FOR_T5_CODEWORDS_2_2__read                               ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   swizzle7_40g_reg_FEC_CTRL_write                                                      ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   swizzle7_40g_reg_FEC_CTRL_field_set                                                  ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 swizzle7_40g_reg_FEC_CTRL_read                                                       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_write                                       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_field_set                                   ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 swizzle7_40g_reg_LPA_STATISTICS_MODE_CFG_read                                        ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_write                                      ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_field_set                                  ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 swizzle7_40g_reg_LLSA_STATISTICS_MODE_CFG_read                                       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 swizzle7_40g_reg_LPA_STATISTICS_OTU_FRAME_PULSE_CNT_read                             ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 swizzle7_40g_reg_LPA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_read                       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 swizzle7_40g_reg_LPA_STATISTICS_CORRECTED_1S_CNT_read                                ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 swizzle7_40g_reg_LPA_STATISTICS_CORRECTED_0S_CNT_read                                ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 swizzle7_40g_reg_LLSA_STATISTICS_FAILURE_STAT_read                                   ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 swizzle7_40g_reg_LLSA_STATISTICS_UNCORRECTED_CODEWORDS_CNT_read                      ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void   swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_array_burst_read ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_1S_CNT_FOR_LANES_0_TO_19_array_read       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 N )
 | 
 | void   swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_array_burst_read ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 swizzle7_40g_reg_LLSA_STATISTICS_CORRECTED_0S_CNT_FOR_LANES_0_TO_19_array_read       ( swizzle7_40g_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 N )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SWIZZLE7_40G_IO_H */
