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
 *     and register accessor functions for the otn_mld block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing otn_mld io functions is:
 | {
 |     otn_mld_buffer_t b_ptr[1];
 |     otn_mld_buffer_init( b_ptr, h_ptr );
 |     value = otn_mld_field_<FIELD>_get( b_ptr, h_ptr );
 |     otn_mld_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     otn_mld_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = otn_mld_field_<FIELD>_get( NULL, h_ptr );
 | otn_mld_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------+
 | | Register 0x00010000 MASTER_CONFIG |
 | +-----------------------------------+
 | | bit  6   R/W  SFIS_N10_ENABLE_N5  |
 | | bit  4   R/W  OTU4_SEL            |
 | | bit  3:2 R/W  HADAPT40G_RESET     |
 | | bit  1   R/W  HADAPT100G_RESET    |
 | | bit  0   R/W  MLD_TOP_RESET       |
 | +-----------------------------------+
 | 
 | void   otn_mld_field_SFIS_N10_ENABLE_N5_set    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_SFIS_N10_ENABLE_N5_get    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_OTU4_SEL_set              ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_OTU4_SEL_get              ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_HADAPT40G_RESET_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G_RESET_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT40G_RESET_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT40G_RESET_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   otn_mld_field_HADAPT100G_RESET_set      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_RESET_get      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_MLD_TOP_RESET_set         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MLD_TOP_RESET_get         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x00010004 LOWPWR_CONFIG |
 | +-----------------------------------+
 | | bit  13:12 R/W  SFI51_LOWPWR      |
 | | bit  11:8  R/W  SFIS_LOWPWR       |
 | | bit  7:6   R/W  HADAPT40G_LOWPWR  |
 | | bit  5     R/W  ADAPT100G_LOWPWR  |
 | | bit  4:2   R/W  OPSM23_LOWPWR     |
 | | bit  1     R/W  OPSM4_LOWPWR      |
 | | bit  0     R/W  MLD_LOWPWR        |
 | +-----------------------------------+
 | 
 | void   otn_mld_field_SFI51_LOWPWR_set           ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_SFI51_LOWPWR_get           ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_SFI51_LOWPWR_set     ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_SFI51_LOWPWR_get     ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   otn_mld_field_SFIS_LOWPWR_set            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_SFIS_LOWPWR_get            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_SFIS_LOWPWR_set      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_SFIS_LOWPWR_get      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   otn_mld_field_HADAPT40G_LOWPWR_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G_LOWPWR_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT40G_LOWPWR_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT40G_LOWPWR_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   otn_mld_field_ADAPT100G_LOWPWR_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_ADAPT100G_LOWPWR_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_OPSM23_LOWPWR_set          ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_OPSM23_LOWPWR_get          ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_OPSM23_LOWPWR_set    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_OPSM23_LOWPWR_get    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   otn_mld_field_OPSM4_LOWPWR_set           ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_OPSM4_LOWPWR_get           ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_MLD_LOWPWR_set             ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MLD_LOWPWR_get             ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +------------------------------------------------+
 | | Register (0x00010008 + (N) * 0x04) LANE_CONFIG |
 | |     N[0..11]                                   |
 | +------------------------------------------------+
 | | bit  2 R/W  PRECODER_32_RX_BYPASS              |
 | | bit  1 R/W  PRECODER_32_TX_BYPASS              |
 | | bit  0 R/W  BERGEN_32_BYPASS                   |
 | +------------------------------------------------+
 | 
 | void   otn_mld_field_PRECODER_32_RX_BYPASS_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_PRECODER_32_RX_BYPASS_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   otn_mld_field_PRECODER_32_TX_BYPASS_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_PRECODER_32_TX_BYPASS_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   otn_mld_field_BERGEN_32_BYPASS_set      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_BERGEN_32_BYPASS_get      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------------------------+
 | | Register (0x00010038 + (N) * 0x04) DISTRIBUTION_MUX_CONFIG |
 | |     N[0..11]                                               |
 | +------------------------------------------------------------+
 | | bit  7:0 R/W  LANE_10G_TX_SRC                              |
 | +------------------------------------------------------------+
 | 
 | void   otn_mld_field_LANE_10G_TX_SRC_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_LANE_10G_TX_SRC_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   otn_mld_field_range_LANE_10G_TX_SRC_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_LANE_10G_TX_SRC_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00010068 MUX0_CONFIG |
 | +---------------------------------+
 | | bit  1:0 R/W  MUX0_LINE_STREAM  |
 | +---------------------------------+
 | 
 | void   otn_mld_field_MUX0_LINE_STREAM_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX0_LINE_STREAM_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX0_LINE_STREAM_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX0_LINE_STREAM_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x0001006c MUX1_CONFIG |
 | +---------------------------------+
 | | bit  1:0 R/W  MUX1_LINE_STREAM  |
 | +---------------------------------+
 | 
 | void   otn_mld_field_MUX1_LINE_STREAM_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX1_LINE_STREAM_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX1_LINE_STREAM_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX1_LINE_STREAM_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00010070 MUX2_CONFIG |
 | +---------------------------------+
 | | bit  3:1 R/W  MUX2_SYS_STREAM   |
 | +---------------------------------+
 | 
 | void   otn_mld_field_MUX2_SYS_STREAM_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX2_SYS_STREAM_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX2_SYS_STREAM_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX2_SYS_STREAM_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00010074 MUX3_CONFIG  |
 | +----------------------------------+
 | | bit  1:0 R/W  MUX3_LINERX_STREAM |
 | +----------------------------------+
 | 
 | void   otn_mld_field_MUX3_LINERX_STREAM_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX3_LINERX_STREAM_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX3_LINERX_STREAM_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX3_LINERX_STREAM_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00010078 MUX4_CONFIG |
 | +---------------------------------+
 | | bit  1:0 R/W  MUX4_LINE_STREAM  |
 | +---------------------------------+
 | 
 | void   otn_mld_field_MUX4_LINE_STREAM_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX4_LINE_STREAM_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX4_LINE_STREAM_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX4_LINE_STREAM_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x0001007c MUX5_CONFIG |
 | +---------------------------------+
 | | bit  1:0 R/W  MUX5_LINE_STREAM  |
 | +---------------------------------+
 | 
 | void   otn_mld_field_MUX5_LINE_STREAM_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX5_LINE_STREAM_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX5_LINE_STREAM_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX5_LINE_STREAM_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00010080 MUX6_CONFIG |
 | +---------------------------------+
 | | bit  1:0 R/W  MUX6_LINE_STREAM  |
 | +---------------------------------+
 | 
 | void   otn_mld_field_MUX6_LINE_STREAM_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX6_LINE_STREAM_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX6_LINE_STREAM_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX6_LINE_STREAM_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00010084 MUX7_CONFIG |
 | +---------------------------------+
 | | bit  4:2 R/W  MUX7_SYS_STREAM   |
 | | bit  1:0 R/W  MUX7_LINE_STREAM  |
 | +---------------------------------+
 | 
 | void   otn_mld_field_MUX7_SYS_STREAM_set        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX7_SYS_STREAM_get        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX7_SYS_STREAM_set  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX7_SYS_STREAM_get  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   otn_mld_field_MUX7_LINE_STREAM_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX7_LINE_STREAM_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX7_LINE_STREAM_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX7_LINE_STREAM_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00010088 MUX8_CONFIG |
 | +---------------------------------+
 | | bit  1:0 R/W  MUX8_LINE_STREAM  |
 | +---------------------------------+
 | 
 | void   otn_mld_field_MUX8_LINE_STREAM_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX8_LINE_STREAM_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX8_LINE_STREAM_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX8_LINE_STREAM_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x0001008c MUX9_CONFIG |
 | +---------------------------------+
 | | bit  1:0 R/W  MUX9_LINE_STREAM  |
 | +---------------------------------+
 | 
 | void   otn_mld_field_MUX9_LINE_STREAM_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX9_LINE_STREAM_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX9_LINE_STREAM_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX9_LINE_STREAM_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00010090 MUX10_CONFIG |
 | +----------------------------------+
 | | bit  1:0 R/W  MUX10_LINE_STREAM  |
 | +----------------------------------+
 | 
 | void   otn_mld_field_MUX10_LINE_STREAM_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX10_LINE_STREAM_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX10_LINE_STREAM_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX10_LINE_STREAM_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00010094 MUX11_CONFIG |
 | +----------------------------------+
 | | bit  4:2 R/W  MUX11_SYS_STREAM   |
 | | bit  1:0 R/W  MUX11_LINE_STREAM  |
 | +----------------------------------+
 | 
 | void   otn_mld_field_MUX11_SYS_STREAM_set        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX11_SYS_STREAM_get        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX11_SYS_STREAM_set  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX11_SYS_STREAM_get  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   otn_mld_field_MUX11_LINE_STREAM_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX11_LINE_STREAM_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX11_LINE_STREAM_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX11_LINE_STREAM_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00010098 MUX12_CONFIG |
 | +----------------------------------+
 | | bit  1:0 R/W  MUX12_LINE_STREAM  |
 | +----------------------------------+
 | 
 | void   otn_mld_field_MUX12_LINE_STREAM_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX12_LINE_STREAM_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX12_LINE_STREAM_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX12_LINE_STREAM_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x0001009c MUX13_CONFIG |
 | +----------------------------------+
 | | bit  3:1 R/W  MUX13_SYS_STREAM   |
 | +----------------------------------+
 | 
 | void   otn_mld_field_MUX13_SYS_STREAM_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_MUX13_SYS_STREAM_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_MUX13_SYS_STREAM_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_MUX13_SYS_STREAM_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x000100a0 + (N) * 0x04) OPSM23_SFI_PATH_CONFIG |
 | |     N[0..2]                                               |
 | +-----------------------------------------------------------+
 | | bit  2 R/W  PRECODER_128_RX_BYPASS                        |
 | | bit  1 R/W  PRECODER_128_TX_BYPASS                        |
 | +-----------------------------------------------------------+
 | 
 | void   otn_mld_field_PRECODER_128_RX_BYPASS_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_PRECODER_128_RX_BYPASS_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   otn_mld_field_PRECODER_128_TX_BYPASS_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_PRECODER_128_TX_BYPASS_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------------------------+
 | | Register (0x000100ac + (N) * 0x4) DEFECT_RCP_CHNL_CONFIG |
 | |     N[0..11]                                             |
 | +----------------------------------------------------------+
 | | bit  3:0 R/W  DEFECT_RCP_CHNL                            |
 | +----------------------------------------------------------+
 | 
 | void   otn_mld_field_DEFECT_RCP_CHNL_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_DEFECT_RCP_CHNL_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   otn_mld_field_range_DEFECT_RCP_CHNL_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_DEFECT_RCP_CHNL_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000100dc + (N) * 0x4) DLOM_TO_FFAIL_MAPPING |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  0 R/W  DLOM_TO_FFAIL_MAPPING                       |
 | +---------------------------------------------------------+
 | 
 | void   otn_mld_field_DLOM_TO_FFAIL_MAPPING_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_DLOM_TO_FFAIL_MAPPING_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------+
 | | Register 0x0001010c INT_SUMMARY |
 | +---------------------------------+
 | | bit  10:0 R  INT_SUMMARY        |
 | +---------------------------------+
 | 
 | UINT32 otn_mld_field_INT_SUMMARY_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32 otn_mld_field_range_INT_SUMMARY_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00010110 HADAPT_INT_SUMMARY |
 | +----------------------------------------+
 | | bit  2 R  HADAPT100G_INT_SUMMARY       |
 | | bit  1 R  HADAPT40G1_INT_SUMMARY       |
 | | bit  0 R  HADAPT40G0_INT_SUMMARY       |
 | +----------------------------------------+
 | 
 | UINT32               otn_mld_field_HADAPT100G_INT_SUMMARY_get  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_INT_SUMMARY_poll ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               otn_mld_field_HADAPT40G1_INT_SUMMARY_get  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G1_INT_SUMMARY_poll ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               otn_mld_field_HADAPT40G0_INT_SUMMARY_get  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G0_INT_SUMMARY_poll ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00010114 + (N) * 0x4) RINGOSC_32_CONFIG |
 | |     N[0..35]                                        |
 | +-----------------------------------------------------+
 | | bit  1 R/W  RINGOSC_32_BYPASS                       |
 | | bit  0 R/W  RINGOSC_32_ENABLE                       |
 | +-----------------------------------------------------+
 | 
 | void   otn_mld_field_RINGOSC_32_BYPASS_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_RINGOSC_32_BYPASS_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   otn_mld_field_RINGOSC_32_ENABLE_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_RINGOSC_32_ENABLE_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------------------+
 | | Register (0x000101a4 + (N) * 0x4) BERGEN32_CONFIG1 |
 | |     N[0..11]                                       |
 | +----------------------------------------------------+
 | | bit  31:8 R/W  BERGEN32_BER_IN                     |
 | | bit  7    R/W  BERGEN32_USE_BITLANE_ERR_LIMIT      |
 | | bit  6:3  R/W  BERGEN32_BITLANE_ERR_LIMIT          |
 | | bit  2:1  R/W  BERGEN32_ERR_MODE                   |
 | | bit  0    R/W  BERGEN32_ENABLE                     |
 | +----------------------------------------------------+
 | 
 | void   otn_mld_field_BERGEN32_BER_IN_set                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_BERGEN32_BER_IN_get                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   otn_mld_field_range_BERGEN32_BER_IN_set            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_BERGEN32_BER_IN_get            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otn_mld_field_BERGEN32_USE_BITLANE_ERR_LIMIT_set   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_BERGEN32_USE_BITLANE_ERR_LIMIT_get   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   otn_mld_field_BERGEN32_BITLANE_ERR_LIMIT_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_BERGEN32_BITLANE_ERR_LIMIT_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   otn_mld_field_range_BERGEN32_BITLANE_ERR_LIMIT_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_BERGEN32_BITLANE_ERR_LIMIT_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otn_mld_field_BERGEN32_ERR_MODE_set                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_BERGEN32_ERR_MODE_get                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   otn_mld_field_range_BERGEN32_ERR_MODE_set          ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_BERGEN32_ERR_MODE_get          ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otn_mld_field_BERGEN32_ENABLE_set                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_BERGEN32_ENABLE_get                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------------------+
 | | Register (0x000101d4 + (N) * 0x4) BERGEN32_CONFIG2 |
 | |     N[0..11]                                       |
 | +----------------------------------------------------+
 | | bit  31:0 R/W  BERGEN32_ENABLE_BITLANE             |
 | +----------------------------------------------------+
 | 
 | void   otn_mld_field_BERGEN32_ENABLE_BITLANE_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otn_mld_field_BERGEN32_ENABLE_BITLANE_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   otn_mld_field_range_BERGEN32_ENABLE_BITLANE_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_BERGEN32_ENABLE_BITLANE_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00010318 HADAPT40G0_CONFIG  |
 | +----------------------------------------+
 | | bit  1 R/W  HADAPT40G0_DPQPSK_MODE     |
 | | bit  0 R/W  HADAPT40G0_PMC_TX_DATA_SEL |
 | +----------------------------------------+
 | 
 | void   otn_mld_field_HADAPT40G0_DPQPSK_MODE_set     ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G0_DPQPSK_MODE_get     ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_HADAPT40G0_PMC_TX_DATA_SEL_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G0_PMC_TX_DATA_SEL_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x0001031c HADAPT40G0_MODE0 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT40G0_MODE0      |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT40G0_MODE0_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G0_MODE0_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT40G0_MODE0_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT40G0_MODE0_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00010320 HADAPT40G0_MODE1 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT40G0_MODE1      |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT40G0_MODE1_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G0_MODE1_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT40G0_MODE1_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT40G0_MODE1_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00010324 HADAPT40G0_MODE2 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT40G0_MODE2      |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT40G0_MODE2_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G0_MODE2_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT40G0_MODE2_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT40G0_MODE2_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00010328 HADAPT40G0_MODE3 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT40G0_MODE3      |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT40G0_MODE3_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G0_MODE3_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT40G0_MODE3_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT40G0_MODE3_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x0001032c HADAPT40G0_MODE4 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT40G0_MODE4      |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT40G0_MODE4_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G0_MODE4_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT40G0_MODE4_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT40G0_MODE4_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00010330 HADAPT40G0_INT_EN |
 | +---------------------------------------+
 | | bit  31:0 R/W  HADAPT40G0_INT_E       |
 | +---------------------------------------+
 | 
 | void   otn_mld_field_HADAPT40G0_INT_E_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G0_INT_E_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT40G0_INT_E_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT40G0_INT_E_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00010334 HADAPT40G0_INT |
 | +------------------------------------+
 | | bit  31:0 R/W  HADAPT40G0_INT_I    |
 | +------------------------------------+
 | 
 | void                 otn_mld_field_HADAPT40G0_INT_I_set_to_clear       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               otn_mld_field_HADAPT40G0_INT_I_get                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 otn_mld_field_range_HADAPT40G0_INT_I_set_to_clear ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otn_mld_field_range_HADAPT40G0_INT_I_get          ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT40G0_INT_I_poll         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G0_INT_I_poll               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x00010338 HADAPT40G0_INT_VAL |
 | +----------------------------------------+
 | | bit  31:0 R  HADAPT40G0_INT_V          |
 | +----------------------------------------+
 | 
 | UINT32               otn_mld_field_HADAPT40G0_INT_V_get        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32               otn_mld_field_range_HADAPT40G0_INT_V_get  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT40G0_INT_V_poll ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G0_INT_V_poll       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x0001033c HADAPT40G0_STAT |
 | +-------------------------------------+
 | | bit  31:0 R  HADAPT40G0_STATUS      |
 | +-------------------------------------+
 | 
 | UINT32 otn_mld_field_HADAPT40G0_STATUS_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32 otn_mld_field_range_HADAPT40G0_STATUS_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00010340 HADAPT40G1_CONFIG  |
 | +----------------------------------------+
 | | bit  1 R/W  HADAPT40G1_DPQPSK_MODE     |
 | | bit  0 R/W  HADAPT40G1_PMC_TX_DATA_SEL |
 | +----------------------------------------+
 | 
 | void   otn_mld_field_HADAPT40G1_DPQPSK_MODE_set     ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G1_DPQPSK_MODE_get     ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_HADAPT40G1_PMC_TX_DATA_SEL_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G1_PMC_TX_DATA_SEL_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00010344 HADAPT40G1_MODE0 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT40G1_MODE0      |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT40G1_MODE0_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G1_MODE0_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT40G1_MODE0_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT40G1_MODE0_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00010348 HADAPT40G1_MODE1 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT40G1_MODE1      |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT40G1_MODE1_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G1_MODE1_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT40G1_MODE1_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT40G1_MODE1_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x0001034c HADAPT40G1_MODE2 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT40G1_MODE2      |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT40G1_MODE2_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G1_MODE2_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT40G1_MODE2_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT40G1_MODE2_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00010350 HADAPT40G1_MODE3 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT40G1_MODE3      |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT40G1_MODE3_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G1_MODE3_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT40G1_MODE3_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT40G1_MODE3_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00010354 HADAPT40G1_MODE4 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT40G1_MODE4      |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT40G1_MODE4_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G1_MODE4_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT40G1_MODE4_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT40G1_MODE4_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00010358 HADAPT40G1_INT_EN |
 | +---------------------------------------+
 | | bit  31:0 R/W  HADAPT40G1_INT_E       |
 | +---------------------------------------+
 | 
 | void   otn_mld_field_HADAPT40G1_INT_E_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT40G1_INT_E_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT40G1_INT_E_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT40G1_INT_E_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x0001035c HADAPT40G1_INT |
 | +------------------------------------+
 | | bit  31:0 R/W  HADAPT40G1_INT_I    |
 | +------------------------------------+
 | 
 | void                 otn_mld_field_HADAPT40G1_INT_I_set_to_clear       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               otn_mld_field_HADAPT40G1_INT_I_get                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 otn_mld_field_range_HADAPT40G1_INT_I_set_to_clear ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otn_mld_field_range_HADAPT40G1_INT_I_get          ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT40G1_INT_I_poll         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G1_INT_I_poll               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x00010360 HADAPT40G1_INT_VAL |
 | +----------------------------------------+
 | | bit  31:0 R  HADAPT40G1_INT_V          |
 | +----------------------------------------+
 | 
 | UINT32               otn_mld_field_HADAPT40G1_INT_V_get        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32               otn_mld_field_range_HADAPT40G1_INT_V_get  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT40G1_INT_V_poll ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT40G1_INT_V_poll       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00010364 HADAPT40G1_STAT |
 | +-------------------------------------+
 | | bit  31:0 R  HADAPT40G1_STATUS      |
 | +-------------------------------------+
 | 
 | UINT32 otn_mld_field_HADAPT40G1_STATUS_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32 otn_mld_field_range_HADAPT40G1_STATUS_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00010368 HADAPT100G_CONFIG |
 | +---------------------------------------+
 | | bit  0 R/W  HADAPT100G_SFI_MODE_100G  |
 | +---------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_SFI_MODE_100G_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_SFI_MODE_100G_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x0001036c HADAPT100G_MODE0 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_MODE_0     |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_MODE_0_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_MODE_0_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_MODE_0_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_MODE_0_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00010370 HADAPT100G_MODE1 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_MODE_1     |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_MODE_1_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_MODE_1_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_MODE_1_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_MODE_1_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00010374 HADAPT100G_MODE2 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_MODE_2     |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_MODE_2_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_MODE_2_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_MODE_2_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_MODE_2_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00010378 HADAPT100G_MODE3 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_MODE_3     |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_MODE_3_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_MODE_3_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_MODE_3_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_MODE_3_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x0001037c HADAPT100G_MODE4 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_MODE_4     |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_MODE_4_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_MODE_4_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_MODE_4_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_MODE_4_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00010380 HADAPT100G_MODE5 |
 | +--------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_MODE_5     |
 | +--------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_MODE_5_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_MODE_5_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_MODE_5_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_MODE_5_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00010384 HADAPT100G_PATTERN0 |
 | +-----------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_PATTERN_0     |
 | +-----------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_PATTERN_0_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_PATTERN_0_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_PATTERN_0_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_PATTERN_0_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00010388 HADAPT100G_PATTERN1 |
 | +-----------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_PATTERN_1     |
 | +-----------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_PATTERN_1_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_PATTERN_1_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_PATTERN_1_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_PATTERN_1_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0001038c HADAPT100G_PATTERN2 |
 | +-----------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_PATTERN_2     |
 | +-----------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_PATTERN_2_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_PATTERN_2_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_PATTERN_2_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_PATTERN_2_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00010390 HADAPT100G_PATTERN3 |
 | +-----------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_PATTERN_3     |
 | +-----------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_PATTERN_3_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_PATTERN_3_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_PATTERN_3_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_PATTERN_3_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00010394 HADAPT100G_PATTERN4 |
 | +-----------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_PATTERN_4     |
 | +-----------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_PATTERN_4_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_PATTERN_4_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_PATTERN_4_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_PATTERN_4_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00010398 HADAPT100G_PATTERN5 |
 | +-----------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_PATTERN_5     |
 | +-----------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_PATTERN_5_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_PATTERN_5_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_PATTERN_5_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_PATTERN_5_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0001039c HADAPT100G_PATTERN6 |
 | +-----------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_PATTERN_6     |
 | +-----------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_PATTERN_6_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_PATTERN_6_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_PATTERN_6_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_PATTERN_6_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x000103a0 HADAPT100G_PATTERN7 |
 | +-----------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_PATTERN_7     |
 | +-----------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_PATTERN_7_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_PATTERN_7_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_PATTERN_7_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_PATTERN_7_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x000103a4 HADAPT100G_PATTERN8 |
 | +-----------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_PATTERN_8     |
 | +-----------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_PATTERN_8_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_PATTERN_8_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_PATTERN_8_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_PATTERN_8_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x000103a8 HADAPT100G_PATTERN9 |
 | +-----------------------------------------+
 | | bit  31:0 R/W  HADAPT100G_PATTERN_9     |
 | +-----------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_PATTERN_9_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_PATTERN_9_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_PATTERN_9_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_PATTERN_9_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000103ac HADAPT100G_INT_EN  |
 | +----------------------------------------+
 | | bit  15:8 R/W  HADAPT100G_RXCLK2_INT_E |
 | | bit  7:0  R/W  HADAPT100G_RXCLK1_INT_E |
 | +----------------------------------------+
 | 
 | void   otn_mld_field_HADAPT100G_RXCLK2_INT_E_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_RXCLK2_INT_E_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_RXCLK2_INT_E_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_RXCLK2_INT_E_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   otn_mld_field_HADAPT100G_RXCLK1_INT_E_set       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 otn_mld_field_HADAPT100G_RXCLK1_INT_E_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   otn_mld_field_range_HADAPT100G_RXCLK1_INT_E_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otn_mld_field_range_HADAPT100G_RXCLK1_INT_E_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000103b0 HADAPT100G_INT     |
 | +----------------------------------------+
 | | bit  15:8 R/W  HADAPT100G_RXCLK2_INT_I |
 | | bit  7:0  R/W  HADAPT100G_RXCLK1_INT_I |
 | +----------------------------------------+
 | 
 | void                 otn_mld_field_HADAPT100G_RXCLK2_INT_I_set_to_clear       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               otn_mld_field_HADAPT100G_RXCLK2_INT_I_get                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_set_to_clear ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_get          ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT100G_RXCLK2_INT_I_poll         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_RXCLK2_INT_I_poll               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 otn_mld_field_HADAPT100G_RXCLK1_INT_I_set_to_clear       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               otn_mld_field_HADAPT100G_RXCLK1_INT_I_get                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_set_to_clear ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_get          ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT100G_RXCLK1_INT_I_poll         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_RXCLK1_INT_I_poll               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x000103b4 HADAPT100G_INT_VAL |
 | +----------------------------------------+
 | | bit  15:8 R  HADAPT100G_RXCLK2_INT_V   |
 | | bit  7:0  R  HADAPT100G_RXCLK1_INT_V   |
 | +----------------------------------------+
 | 
 | UINT32               otn_mld_field_HADAPT100G_RXCLK2_INT_V_get        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32               otn_mld_field_range_HADAPT100G_RXCLK2_INT_V_get  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT100G_RXCLK2_INT_V_poll ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_RXCLK2_INT_V_poll       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               otn_mld_field_HADAPT100G_RXCLK1_INT_V_get        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32               otn_mld_field_range_HADAPT100G_RXCLK1_INT_V_get  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otn_mld_field_range_HADAPT100G_RXCLK1_INT_V_poll ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otn_mld_field_HADAPT100G_RXCLK1_INT_V_poll       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x000103b8 HADAPT100G_STAT |
 | +-------------------------------------+
 | | bit  15:0 R  HADAPT100G_STATUS      |
 | +-------------------------------------+
 | 
 | UINT32 otn_mld_field_HADAPT100G_STATUS_get       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32 otn_mld_field_range_HADAPT100G_STATUS_get ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _OTN_MLD_IO_H
#define _OTN_MLD_IO_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "otn_mld_regs.h"
#include "otn_mld_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 otn_mld_buffer_init               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 otn_mld_buffer_flush              ( otn_mld_buffer_t *b_ptr )
 | UINT32               otn_mld_reg_read                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 otn_mld_reg_write                 ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 otn_mld_field_set                 ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 otn_mld_action_on_write_field_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 otn_mld_burst_read                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 otn_mld_burst_write               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE otn_mld_poll                      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 otn_mld_reg_MASTER_CONFIG_write                      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MASTER_CONFIG_field_set                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MASTER_CONFIG_read                       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_LOWPWR_CONFIG_write                      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_LOWPWR_CONFIG_field_set                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_LOWPWR_CONFIG_read                       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_MUX0_CONFIG_write                        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MUX0_CONFIG_field_set                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MUX0_CONFIG_read                         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_MUX1_CONFIG_write                        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MUX1_CONFIG_field_set                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MUX1_CONFIG_read                         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_MUX2_CONFIG_write                        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MUX2_CONFIG_field_set                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MUX2_CONFIG_read                         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_MUX3_CONFIG_write                        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MUX3_CONFIG_field_set                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MUX3_CONFIG_read                         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_MUX4_CONFIG_write                        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MUX4_CONFIG_field_set                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MUX4_CONFIG_read                         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_MUX5_CONFIG_write                        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MUX5_CONFIG_field_set                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MUX5_CONFIG_read                         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_MUX6_CONFIG_write                        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MUX6_CONFIG_field_set                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MUX6_CONFIG_read                         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_MUX7_CONFIG_write                        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MUX7_CONFIG_field_set                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MUX7_CONFIG_read                         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_MUX8_CONFIG_write                        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MUX8_CONFIG_field_set                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MUX8_CONFIG_read                         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_MUX9_CONFIG_write                        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MUX9_CONFIG_field_set                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MUX9_CONFIG_read                         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_MUX10_CONFIG_write                       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MUX10_CONFIG_field_set                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MUX10_CONFIG_read                        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_MUX11_CONFIG_write                       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MUX11_CONFIG_field_set                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MUX11_CONFIG_read                        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_MUX12_CONFIG_write                       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MUX12_CONFIG_field_set                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MUX12_CONFIG_read                        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_MUX13_CONFIG_write                       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_MUX13_CONFIG_field_set                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_MUX13_CONFIG_read                        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT40G0_CONFIG_write                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G0_CONFIG_field_set              ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G0_CONFIG_read                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT40G0_MODE0_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G0_MODE0_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G0_MODE0_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT40G0_MODE1_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G0_MODE1_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G0_MODE1_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT40G0_MODE2_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G0_MODE2_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G0_MODE2_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT40G0_MODE3_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G0_MODE3_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G0_MODE3_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT40G0_MODE4_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G0_MODE4_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G0_MODE4_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT40G1_CONFIG_write                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G1_CONFIG_field_set              ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G1_CONFIG_read                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT40G1_MODE0_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G1_MODE0_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G1_MODE0_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT40G1_MODE1_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G1_MODE1_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G1_MODE1_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT40G1_MODE2_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G1_MODE2_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G1_MODE2_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT40G1_MODE3_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G1_MODE3_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G1_MODE3_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT40G1_MODE4_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G1_MODE4_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G1_MODE4_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_CONFIG_write                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_CONFIG_field_set              ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_CONFIG_read                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_MODE0_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_MODE0_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_MODE0_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_MODE1_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_MODE1_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_MODE1_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_MODE2_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_MODE2_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_MODE2_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_MODE3_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_MODE3_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_MODE3_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_MODE4_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_MODE4_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_MODE4_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_MODE5_write                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_MODE5_field_set               ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_MODE5_read                    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_PATTERN0_write                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_PATTERN0_field_set            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_PATTERN0_read                 ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_PATTERN1_write                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_PATTERN1_field_set            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_PATTERN1_read                 ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_PATTERN2_write                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_PATTERN2_field_set            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_PATTERN2_read                 ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_PATTERN3_write                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_PATTERN3_field_set            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_PATTERN3_read                 ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_PATTERN4_write                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_PATTERN4_field_set            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_PATTERN4_read                 ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_PATTERN5_write                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_PATTERN5_field_set            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_PATTERN5_read                 ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_PATTERN6_write                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_PATTERN6_field_set            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_PATTERN6_read                 ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_PATTERN7_write                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_PATTERN7_field_set            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_PATTERN7_read                 ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_PATTERN8_write                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_PATTERN8_field_set            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_PATTERN8_read                 ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_PATTERN9_write                ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_PATTERN9_field_set            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_PATTERN9_read                 ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_LANE_CONFIG_array_write                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otn_mld_reg_LANE_CONFIG_array_field_set              ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_LANE_CONFIG_array_read                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_write      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_field_set  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_DISTRIBUTION_MUX_CONFIG_array_read       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_write       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_field_set   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_DEFECT_RCP_CHNL_CONFIG_array_read        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otn_mld_reg_DLOM_TO_FFAIL_MAPPING_array_write        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otn_mld_reg_DLOM_TO_FFAIL_MAPPING_array_field_set    ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_DLOM_TO_FFAIL_MAPPING_array_read         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otn_mld_reg_BERGEN32_CONFIG1_array_write             ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otn_mld_reg_BERGEN32_CONFIG1_array_field_set         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_BERGEN32_CONFIG1_array_read              ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otn_mld_reg_BERGEN32_CONFIG2_array_write             ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otn_mld_reg_BERGEN32_CONFIG2_array_field_set         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_BERGEN32_CONFIG2_array_read              ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_write       ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_field_set   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_OPSM23_SFI_PATH_CONFIG_array_read        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otn_mld_reg_RINGOSC_32_CONFIG_array_write            ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otn_mld_reg_RINGOSC_32_CONFIG_array_field_set        ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_RINGOSC_32_CONFIG_array_read             ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otn_mld_reg_HADAPT40G0_INT_EN_write                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G0_INT_EN_field_set              ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G0_INT_EN_read                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT40G1_INT_EN_write                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G1_INT_EN_field_set              ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G1_INT_EN_read                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 otn_mld_reg_HADAPT100G_INT_EN_write                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_INT_EN_field_set              ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_INT_EN_read                   ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | UINT32               otn_mld_reg_HADAPT_INT_SUMMARY_read                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT_INT_SUMMARY_poll                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otn_mld_reg_HADAPT40G0_INT_write                     ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G0_INT_action_on_write_field_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G0_INT_read                      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT40G0_INT_poll                      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otn_mld_reg_HADAPT40G1_INT_write                     ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT40G1_INT_action_on_write_field_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT40G1_INT_read                      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT40G1_INT_poll                      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otn_mld_reg_HADAPT100G_INT_write                     ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 otn_mld_reg_HADAPT100G_INT_action_on_write_field_set ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otn_mld_reg_HADAPT100G_INT_read                      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT100G_INT_poll                      ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otn_mld_reg_HADAPT40G0_INT_VAL_read                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT40G0_INT_VAL_poll                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otn_mld_reg_HADAPT40G1_INT_VAL_read                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT40G1_INT_VAL_poll                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otn_mld_reg_HADAPT100G_INT_VAL_read                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otn_mld_reg_HADAPT100G_INT_VAL_poll                  ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otn_mld_reg_INT_SUMMARY_read                         ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | UINT32               otn_mld_reg_HADAPT40G0_STAT_read                     ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | UINT32               otn_mld_reg_HADAPT40G1_STAT_read                     ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | UINT32               otn_mld_reg_HADAPT100G_STAT_read                     ( otn_mld_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _OTN_MLD_IO_H */
