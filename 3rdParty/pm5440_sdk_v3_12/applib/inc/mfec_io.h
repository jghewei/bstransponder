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
 *     and register accessor functions for the mfec block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing mfec io functions is:
 | {
 |     mfec_buffer_t b_ptr[1];
 |     mfec_buffer_init( b_ptr, h_ptr );
 |     value = mfec_field_<FIELD>_get( b_ptr, h_ptr );
 |     mfec_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     mfec_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = mfec_field_<FIELD>_get( NULL, h_ptr );
 | mfec_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------+
 | | Register 0x00004000 MASTER_CONFIG |
 | +-----------------------------------+
 | | bit  1 R/W  MFEC_GLUE_LOWPWR      |
 | | bit  0 R/W  RESET                 |
 | +-----------------------------------+
 | 
 | void   mfec_field_MFEC_GLUE_LOWPWR_set ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_MFEC_GLUE_LOWPWR_get ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_RESET_set            ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_RESET_get            ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +----------------------------------------+
 | | Register 0x0000400c SWIZZLE100G_CONFIG |
 | +----------------------------------------+
 | | bit  5   R/W  SWIZZLE_ENC_REINIT       |
 | | bit  4:2 R/W  SWIZZLE_SEL_100G_40G     |
 | | bit  0   R/W  SWIZZLE100G_RAM_LOWPWR   |
 | +----------------------------------------+
 | 
 | void   mfec_field_SWIZZLE_SEL_100G_40G_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_SWIZZLE_SEL_100G_40G_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_SWIZZLE_SEL_100G_40G_set ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_SWIZZLE_SEL_100G_40G_get ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_SWIZZLE100G_RAM_LOWPWR_set     ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_SWIZZLE100G_RAM_LOWPWR_get     ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_SWIZZLE_ENC_REINIT_set         ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_SWIZZLE_ENC_REINIT_get         ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +--------------------------------------------------+
 | | Register 0x00004010 DECODER_CONFIG               |
 | +--------------------------------------------------+
 | | bit  30:27 R/W  DECODER_40G_SLICE2_RSFEC_10G_SEL |
 | | bit  26:23 R/W  DECODER_40G_SLICE1_RSFEC_10G_SEL |
 | | bit  22:19 R/W  DECODER_40G_SLICE0_RSFEC_10G_SEL |
 | | bit  18:14 R/W  DECODER_40G_SLICE2_FEC_SEL       |
 | | bit  13:9  R/W  DECODER_40G_SLICE1_FEC_SEL       |
 | | bit  8:4   R/W  DECODER_40G_SLICE0_FEC_SEL       |
 | | bit  3:1   R/W  DECODER_100G_FEC_SEL             |
 | | bit  0     R/W  DECODER_40G_100G_SEL             |
 | +--------------------------------------------------+
 | 
 | void   mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_DECODER_40G_SLICE2_RSFEC_10G_SEL_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_DECODER_40G_SLICE2_RSFEC_10G_SEL_set ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_DECODER_40G_SLICE2_RSFEC_10G_SEL_get ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_DECODER_40G_SLICE1_RSFEC_10G_SEL_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_DECODER_40G_SLICE1_RSFEC_10G_SEL_set ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_DECODER_40G_SLICE1_RSFEC_10G_SEL_get ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_DECODER_40G_SLICE0_RSFEC_10G_SEL_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_DECODER_40G_SLICE0_RSFEC_10G_SEL_set ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_DECODER_40G_SLICE0_RSFEC_10G_SEL_get ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_DECODER_40G_SLICE2_FEC_SEL_set             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_DECODER_40G_SLICE2_FEC_SEL_get             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_DECODER_40G_SLICE2_FEC_SEL_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_DECODER_40G_SLICE1_FEC_SEL_set             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_DECODER_40G_SLICE1_FEC_SEL_get             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_DECODER_40G_SLICE1_FEC_SEL_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_DECODER_40G_SLICE0_FEC_SEL_set             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_DECODER_40G_SLICE0_FEC_SEL_get             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_DECODER_40G_SLICE0_FEC_SEL_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_DECODER_100G_FEC_SEL_set                   ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_DECODER_100G_FEC_SEL_get                   ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_DECODER_100G_FEC_SEL_set             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_DECODER_100G_FEC_SEL_get             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_DECODER_40G_100G_SEL_set                   ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_DECODER_40G_100G_SEL_get                   ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00004014 DECODER_BYPASS |
 | +------------------------------------+
 | | bit  11:0 R/W  DECODER_BYPASS      |
 | +------------------------------------+
 | 
 | void   mfec_field_DECODER_BYPASS_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_DECODER_BYPASS_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_DECODER_BYPASS_set ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_DECODER_BYPASS_get ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00004018 ENCODER_CONFIG               |
 | +--------------------------------------------------+
 | | bit  30:27 R/W  ENCODER_40G_SLICE2_RSFEC_10G_SEL |
 | | bit  26:23 R/W  ENCODER_40G_SLICE1_RSFEC_10G_SEL |
 | | bit  22:19 R/W  ENCODER_40G_SLICE0_RSFEC_10G_SEL |
 | | bit  18:14 R/W  ENCODER_40G_SLICE2_FEC_SEL       |
 | | bit  13:9  R/W  ENCODER_40G_SLICE1_FEC_SEL       |
 | | bit  8:4   R/W  ENCODER_40G_SLICE0_FEC_SEL       |
 | | bit  3:1   R/W  ENCODER_100G_FEC_SEL             |
 | | bit  0     R/W  ENCODER_40G_100G_SEL             |
 | +--------------------------------------------------+
 | 
 | void   mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_ENCODER_40G_SLICE2_RSFEC_10G_SEL_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_ENCODER_40G_SLICE2_RSFEC_10G_SEL_set ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_ENCODER_40G_SLICE2_RSFEC_10G_SEL_get ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_ENCODER_40G_SLICE1_RSFEC_10G_SEL_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_ENCODER_40G_SLICE1_RSFEC_10G_SEL_set ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_ENCODER_40G_SLICE1_RSFEC_10G_SEL_get ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_ENCODER_40G_SLICE0_RSFEC_10G_SEL_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_ENCODER_40G_SLICE0_RSFEC_10G_SEL_set ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_ENCODER_40G_SLICE0_RSFEC_10G_SEL_get ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_ENCODER_40G_SLICE2_FEC_SEL_set             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_ENCODER_40G_SLICE2_FEC_SEL_get             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_ENCODER_40G_SLICE2_FEC_SEL_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_ENCODER_40G_SLICE1_FEC_SEL_set             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_ENCODER_40G_SLICE1_FEC_SEL_get             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_ENCODER_40G_SLICE1_FEC_SEL_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_ENCODER_40G_SLICE0_FEC_SEL_set             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_ENCODER_40G_SLICE0_FEC_SEL_get             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_ENCODER_40G_SLICE0_FEC_SEL_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_ENCODER_100G_FEC_SEL_set                   ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_ENCODER_100G_FEC_SEL_get                   ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_ENCODER_100G_FEC_SEL_set             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_ENCODER_100G_FEC_SEL_get             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_ENCODER_40G_100G_SEL_set                   ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_ENCODER_40G_100G_SEL_get                   ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x0000401c ENCODER_BYPASS |
 | +------------------------------------+
 | | bit  11:0 R/W  ENCODER_BYPASS      |
 | +------------------------------------+
 | 
 | void   mfec_field_ENCODER_BYPASS_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_ENCODER_BYPASS_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_ENCODER_BYPASS_set ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_ENCODER_BYPASS_get ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00004020 FEC_STAT_UPDATE  |
 | +--------------------------------------+
*/
/*
 | | bit  12:10 R/W  SWZ_FEC_STATS_UPDATE |
 | | bit  9:7   R/W  RS_FEC_STATS_UPDATE  |
 | | bit  6:4   R/W  I7_FEC_STATS_UPDATE  |
 | | bit  3:1   R/W  I4_FEC_STATS_UPDATE  |
 | | bit  0     R/W  ALL_FEC_STATS_UPDATE |
*/
/*
 | +--------------------------------------+
 | 
*/
/*
 | void   mfec_field_SWZ_FEC_STATS_UPDATE_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_SWZ_FEC_STATS_UPDATE_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_SWZ_FEC_STATS_UPDATE_set ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_SWZ_FEC_STATS_UPDATE_get ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_RS_FEC_STATS_UPDATE_set        ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_RS_FEC_STATS_UPDATE_get        ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_RS_FEC_STATS_UPDATE_set  ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_RS_FEC_STATS_UPDATE_get  ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_I7_FEC_STATS_UPDATE_set        ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_I7_FEC_STATS_UPDATE_get        ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_I7_FEC_STATS_UPDATE_set  ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_I7_FEC_STATS_UPDATE_get  ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_I4_FEC_STATS_UPDATE_set        ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_I4_FEC_STATS_UPDATE_get        ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   mfec_field_range_I4_FEC_STATS_UPDATE_set  ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_field_range_I4_FEC_STATS_UPDATE_get  ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mfec_field_ALL_FEC_STATS_UPDATE_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mfec_field_ALL_FEC_STATS_UPDATE_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
*/
/*
 | 
 | +---------------------------------------------------+
 | | Register (0x00004034 + (N) * 0x4) TOTAL_FEC_STATS |
 | |     N[0..11]                                      |
 | +---------------------------------------------------+
 | | bit  31:0 R/W  TOTAL_CORR_ERRS                    |
 | +---------------------------------------------------+
 | 
 | void   mfec_lfield_TOTAL_CORR_ERRS_set       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value[12] )
 | void   mfec_lfield_TOTAL_CORR_ERRS_get       ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value[12] )
 | void   mfec_lfield_range_TOTAL_CORR_ERRS_set ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mfec_lfield_range_TOTAL_CORR_ERRS_get ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MFEC_IO_H
#define _MFEC_IO_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "mfec_regs.h"
#include "mfec_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mfec_buffer_init               ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 mfec_buffer_flush              ( mfec_buffer_t *b_ptr )
 | UINT32               mfec_reg_read                  ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 mfec_reg_write                 ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 mfec_field_set                 ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 mfec_action_on_write_field_set ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mfec_burst_read                ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 mfec_burst_write               ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE mfec_poll                      ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   mfec_reg_MASTER_CONFIG_write               ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void   mfec_reg_MASTER_CONFIG_field_set           ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mfec_reg_MASTER_CONFIG_read                ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void   mfec_reg_SWIZZLE100G_CONFIG_write          ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void   mfec_reg_SWIZZLE100G_CONFIG_field_set      ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mfec_reg_SWIZZLE100G_CONFIG_read           ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void   mfec_reg_DECODER_CONFIG_write              ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void   mfec_reg_DECODER_CONFIG_field_set          ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mfec_reg_DECODER_CONFIG_read               ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void   mfec_reg_DECODER_BYPASS_write              ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void   mfec_reg_DECODER_BYPASS_field_set          ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mfec_reg_DECODER_BYPASS_read               ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void   mfec_reg_ENCODER_CONFIG_write              ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void   mfec_reg_ENCODER_CONFIG_field_set          ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mfec_reg_ENCODER_CONFIG_read               ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void   mfec_reg_ENCODER_BYPASS_write              ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void   mfec_reg_ENCODER_BYPASS_field_set          ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mfec_reg_ENCODER_BYPASS_read               ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void   mfec_reg_TOTAL_FEC_STATS_array_burst_write ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void   mfec_reg_TOTAL_FEC_STATS_array_field_set   ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void   mfec_reg_TOTAL_FEC_STATS_array_burst_read  ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 mfec_reg_TOTAL_FEC_STATS_array_read        ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 N )
 | 
 | void   mfec_reg_FEC_STAT_UPDATE_write             ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void   mfec_reg_FEC_STAT_UPDATE_field_set         ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mfec_reg_FEC_STAT_UPDATE_read              ( mfec_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MFEC_IO_H */
