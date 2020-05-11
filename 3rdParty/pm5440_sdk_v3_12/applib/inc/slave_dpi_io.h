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
 *     and register accessor functions for the slave_dpi block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing slave_dpi io functions is:
 | {
 |     slave_dpi_buffer_t b_ptr[1];
 |     slave_dpi_buffer_init( b_ptr, h_ptr, A );
 |     value = slave_dpi_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     slave_dpi_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     slave_dpi_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = slave_dpi_field_<FIELD>_get( NULL, h_ptr, A );
 | slave_dpi_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------+
 | | Register 0x00000000 ZONE_CONFIG |
 | +---------------------------------+
 | | bit  25:24 R/W  ZONE_OVRD_VAL   |
 | | bit  17    R/W  ZONE_EN         |
 | | bit  16    R/W  ZONE_CFG        |
 | | bit  7:0   R/W  ZONE_MAX_CH     |
 | +---------------------------------+
 | 
 | void   slave_dpi_field_ZONE_OVRD_VAL_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_ZONE_OVRD_VAL_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   slave_dpi_field_range_ZONE_OVRD_VAL_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_ZONE_OVRD_VAL_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   slave_dpi_field_ZONE_EN_set             ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_ZONE_EN_get             ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   slave_dpi_field_ZONE_CFG_set            ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_ZONE_CFG_get            ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   slave_dpi_field_ZONE_MAX_CH_set         ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_ZONE_MAX_CH_get         ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   slave_dpi_field_range_ZONE_MAX_CH_set   ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_ZONE_MAX_CH_get   ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00000010 BURST_CONFIG |
 | +----------------------------------+
 | | bit  0 R/W  BURST_EN             |
 | +----------------------------------+
 | 
 | void   slave_dpi_field_BURST_EN_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_BURST_EN_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------+
 | | Register 0x00000020 HRD_CONFIG |
 | +--------------------------------+
 | | bit  5:0 R/W  HDR_SIZE         |
 | +--------------------------------+
 | 
 | void   slave_dpi_field_HDR_SIZE_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_HDR_SIZE_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   slave_dpi_field_range_HDR_SIZE_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_HDR_SIZE_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000084 DROP_CNT_TRIG_REG |
 | +---------------------------------------+
 | | bit  0 R/W  CH_DROP_CNT_TRIG          |
 | +---------------------------------------+
 | 
 | void   slave_dpi_field_CH_DROP_CNT_TRIG_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_CH_DROP_CNT_TRIG_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x00000100 PMON_CNT_TRIG |
 | +-----------------------------------+
 | | bit  25 R/W  PMON_CNT3_RST        |
 | | bit  24 R/W  PMON_CNT3_TRIG       |
 | | bit  17 R/W  PMON_CNT2_RST        |
 | | bit  16 R/W  PMON_CNT2_TRIG       |
 | | bit  9  R/W  PMON_CNT1_RST        |
 | | bit  8  R/W  PMON_CNT1_TRIG       |
 | | bit  1  R/W  PMON_CNT0_RST        |
 | | bit  0  R/W  PMON_CNT0_TRIG       |
 | +-----------------------------------+
 | 
 | void                 slave_dpi_field_PMON_CNT3_RST_set   ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               slave_dpi_field_PMON_CNT3_RST_get   ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void                 slave_dpi_field_PMON_CNT2_RST_set   ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               slave_dpi_field_PMON_CNT2_RST_get   ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void                 slave_dpi_field_PMON_CNT1_RST_set   ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               slave_dpi_field_PMON_CNT1_RST_get   ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void                 slave_dpi_field_PMON_CNT0_RST_set   ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               slave_dpi_field_PMON_CNT0_RST_get   ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void                 slave_dpi_field_PMON_CNT3_TRIG_set  ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               slave_dpi_field_PMON_CNT3_TRIG_get  ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE slave_dpi_field_PMON_CNT3_TRIG_poll ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 slave_dpi_field_PMON_CNT2_TRIG_set  ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               slave_dpi_field_PMON_CNT2_TRIG_get  ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE slave_dpi_field_PMON_CNT2_TRIG_poll ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 slave_dpi_field_PMON_CNT1_TRIG_set  ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               slave_dpi_field_PMON_CNT1_TRIG_get  ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE slave_dpi_field_PMON_CNT1_TRIG_poll ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 slave_dpi_field_PMON_CNT0_TRIG_set  ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               slave_dpi_field_PMON_CNT0_TRIG_get  ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE slave_dpi_field_PMON_CNT0_TRIG_poll ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00000104 PMON_MODE_TRIG |
 | +------------------------------------+
 | | bit  24 R/W  PMON_CNT3_MODE        |
 | | bit  16 R/W  PMON_CNT2_MODE        |
 | | bit  8  R/W  PMON_CNT1_MODE        |
 | | bit  0  R/W  PMON_CNT0_MODE        |
 | +------------------------------------+
 | 
 | void   slave_dpi_field_PMON_CNT3_MODE_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_PMON_CNT3_MODE_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   slave_dpi_field_PMON_CNT2_MODE_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_PMON_CNT2_MODE_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   slave_dpi_field_PMON_CNT1_MODE_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_PMON_CNT1_MODE_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   slave_dpi_field_PMON_CNT0_MODE_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_PMON_CNT0_MODE_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------+
 | | Register 0x00000108 PMON_PKT_TRIG  |
 | +------------------------------------+
 | | bit  28:24 R/W  PMON_CNT3_PKT_FILT |
 | | bit  20:16 R/W  PMON_CNT2_PKT_FILT |
 | | bit  12:8  R/W  PMON_CNT1_PKT_FILT |
 | | bit  4:0   R/W  PMON_CNT0_PKT_FILT |
 | +------------------------------------+
 | 
 | void   slave_dpi_field_PMON_CNT3_PKT_FILT_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_PMON_CNT3_PKT_FILT_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   slave_dpi_field_range_PMON_CNT3_PKT_FILT_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_PMON_CNT3_PKT_FILT_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   slave_dpi_field_PMON_CNT2_PKT_FILT_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_PMON_CNT2_PKT_FILT_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   slave_dpi_field_range_PMON_CNT2_PKT_FILT_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_PMON_CNT2_PKT_FILT_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   slave_dpi_field_PMON_CNT1_PKT_FILT_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_PMON_CNT1_PKT_FILT_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   slave_dpi_field_range_PMON_CNT1_PKT_FILT_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_PMON_CNT1_PKT_FILT_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   slave_dpi_field_PMON_CNT0_PKT_FILT_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 slave_dpi_field_PMON_CNT0_PKT_FILT_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void   slave_dpi_field_range_PMON_CNT0_PKT_FILT_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_PMON_CNT0_PKT_FILT_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000110 PMON_CNT0_REG |
 | +-----------------------------------+
 | | bit  31:0 R  PMON_CNT0_VAL        |
 | +-----------------------------------+
 | 
 | UINT32 slave_dpi_field_PMON_CNT0_VAL_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | UINT32 slave_dpi_field_range_PMON_CNT0_VAL_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000114 PMON_CNT1_REG |
 | +-----------------------------------+
 | | bit  31:0 R  PMON_CNT1_VAL        |
 | +-----------------------------------+
 | 
 | UINT32 slave_dpi_field_PMON_CNT1_VAL_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | UINT32 slave_dpi_field_range_PMON_CNT1_VAL_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000118 PMON_CNT2_REG |
 | +-----------------------------------+
 | | bit  31:0 R  PMON_CNT2_VAL        |
 | +-----------------------------------+
 | 
 | UINT32 slave_dpi_field_PMON_CNT2_VAL_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | UINT32 slave_dpi_field_range_PMON_CNT2_VAL_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x0000011c PMON_CNT3_REG |
 | +-----------------------------------+
 | | bit  31:0 R  PMON_CNT3_VAL        |
 | +-----------------------------------+
 | 
 | UINT32 slave_dpi_field_PMON_CNT3_VAL_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | UINT32 slave_dpi_field_range_PMON_CNT3_VAL_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register (0x00000400 + (N) * 0x4) CH_ENABLE |
 | |     N[0..149]                               |
 | +---------------------------------------------+
 | | bit  8 R/W  HDR_EN                          |
 | | bit  3 R/W  QS3_CH_EN                       |
 | | bit  2 R/W  QS2_CH_EN                       |
 | | bit  1 R/W  QS1_CH_EN                       |
 | | bit  0 R/W  QS0_CH_EN                       |
 | +---------------------------------------------+
 | 
 | void   slave_dpi_field_HDR_EN_set    ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_HDR_EN_get    ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_QS3_CH_EN_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_QS3_CH_EN_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_QS2_CH_EN_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_QS2_CH_EN_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_QS1_CH_EN_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_QS1_CH_EN_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_QS0_CH_EN_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_QS0_CH_EN_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +---------------------------------------------+
 | | Register (0x00000800 + (N) * 0x4) CH_CONFIG |
 | |     N[0..255]                               |
 | +---------------------------------------------+
 | | bit  17:16 R/W  DCPB_IMUX_SEL               |
 | | bit  11    R/W  PMON_CNT3_EN                |
 | | bit  10    R/W  PMON_CNT2_EN                |
 | | bit  9     R/W  PMON_CNT1_EN                |
 | | bit  8     R/W  PMON_CNT0_EN                |
 | | bit  3     R/W  QS3_CH_MST                  |
 | | bit  2     R/W  QS2_CH_MST                  |
 | | bit  1     R/W  QS1_CH_MST                  |
 | | bit  0     R/W  QS0_CH_MST                  |
 | +---------------------------------------------+
 | 
 | void   slave_dpi_field_DCPB_IMUX_SEL_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_DCPB_IMUX_SEL_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_range_DCPB_IMUX_SEL_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_DCPB_IMUX_SEL_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   slave_dpi_field_PMON_CNT3_EN_set        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_PMON_CNT3_EN_get        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_PMON_CNT2_EN_set        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_PMON_CNT2_EN_get        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_PMON_CNT1_EN_set        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_PMON_CNT1_EN_get        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_PMON_CNT0_EN_set        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_PMON_CNT0_EN_get        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_QS3_CH_MST_set          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_QS3_CH_MST_get          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_QS2_CH_MST_set          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_QS2_CH_MST_get          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_QS1_CH_MST_set          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_QS1_CH_MST_get          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_QS0_CH_MST_set          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_QS0_CH_MST_get          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +--------------------------------------------+
 | | Register (0x00000c00 + (N) * 0x4) CH_Q_MAP |
 | |     N[0..255]                              |
 | +--------------------------------------------+
 | | bit  30:24 R/W  QS3_POP_QID                |
 | | bit  22:16 R/W  QS2_POP_QID                |
 | | bit  14:8  R/W  QS1_POP_QID                |
 | | bit  6:0   R/W  QS0_POP_QID                |
 | +--------------------------------------------+
 | 
 | void   slave_dpi_field_QS3_POP_QID_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_QS3_POP_QID_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_range_QS3_POP_QID_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_QS3_POP_QID_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   slave_dpi_field_QS2_POP_QID_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_QS2_POP_QID_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_range_QS2_POP_QID_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_QS2_POP_QID_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   slave_dpi_field_QS1_POP_QID_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_QS1_POP_QID_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_range_QS1_POP_QID_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_QS1_POP_QID_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   slave_dpi_field_QS0_POP_QID_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_QS0_POP_QID_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_range_QS0_POP_QID_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_QS0_POP_QID_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x00001000 + (N) * 0x4) ZONE_0_1 |
 | |     N[0..255]                              |
 | +--------------------------------------------+
 | | bit  16:0 R/W  ZONE_THR_0_1                |
 | +--------------------------------------------+
 | 
 | void   slave_dpi_field_ZONE_THR_0_1_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_ZONE_THR_0_1_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_range_ZONE_THR_0_1_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_ZONE_THR_0_1_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x00001400 + (N) * 0x4) ZONE_1_2 |
 | |     N[0..255]                              |
 | +--------------------------------------------+
 | | bit  16:0 R/W  ZONE_THR_1_2                |
 | +--------------------------------------------+
 | 
 | void   slave_dpi_field_ZONE_THR_1_2_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_ZONE_THR_1_2_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_range_ZONE_THR_1_2_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_ZONE_THR_1_2_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x00001800 + (N) * 0x4) ZONE_2_3 |
 | |     N[0..255]                              |
 | +--------------------------------------------+
 | | bit  16:0 R/W  ZONE_THR_2_3                |
 | +--------------------------------------------+
 | 
 | void   slave_dpi_field_ZONE_THR_2_3_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_ZONE_THR_2_3_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_range_ZONE_THR_2_3_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_ZONE_THR_2_3_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00001c00 + (N) * 0x4) ZONE_OVRD_CONFIG |
 | |     N[0..255]                                      |
 | +----------------------------------------------------+
 | | bit  25   R/W  ZONE_OVRD_EN                        |
 | | bit  24   R/W  ZONE_OVRD_CFG                       |
 | | bit  10:8 R/W  ZONE_OVRD_DPI_MST                   |
 | | bit  7:0  R/W  ZONE_OVRD_XOFF_CH                   |
 | +----------------------------------------------------+
 | 
 | void   slave_dpi_field_ZONE_OVRD_EN_set            ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_ZONE_OVRD_EN_get            ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_ZONE_OVRD_CFG_set           ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_ZONE_OVRD_CFG_get           ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_ZONE_OVRD_DPI_MST_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_ZONE_OVRD_DPI_MST_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_range_ZONE_OVRD_DPI_MST_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_ZONE_OVRD_DPI_MST_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   slave_dpi_field_ZONE_OVRD_XOFF_CH_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 slave_dpi_field_ZONE_OVRD_XOFF_CH_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   slave_dpi_field_range_ZONE_OVRD_XOFF_CH_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_field_range_ZONE_OVRD_XOFF_CH_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00002000 + (N) * 0x4) CH_DROP_INT |
 | |     N[0..149]                                 |
 | +-----------------------------------------------+
 | | bit  16 R/W  CH_DROP_FULL_I                   |
 | | bit  8  R/W  CH_DROP_CLIENT_I                 |
 | | bit  0  R/W  CH_DROP_OVRSIZE_I                |
 | +-----------------------------------------------+
 | 
 | void                 slave_dpi_field_CH_DROP_FULL_I_set_to_clear    ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               slave_dpi_field_CH_DROP_FULL_I_get             ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE slave_dpi_field_CH_DROP_FULL_I_poll            ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 slave_dpi_field_CH_DROP_CLIENT_I_set_to_clear  ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               slave_dpi_field_CH_DROP_CLIENT_I_get           ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE slave_dpi_field_CH_DROP_CLIENT_I_poll          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 slave_dpi_field_CH_DROP_OVRSIZE_I_set_to_clear ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               slave_dpi_field_CH_DROP_OVRSIZE_I_get          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE slave_dpi_field_CH_DROP_OVRSIZE_I_poll         ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------------+
 | | Register (0x00002800 + (N) * 0x4) CH_DROP_COUNT |
 | |     N[0..149]                                   |
 | +-------------------------------------------------+
 | | bit  31:0 R/W  CH_DROP_CNT_VAL                  |
 | +-------------------------------------------------+
 | 
 | void   slave_dpi_lfield_CH_DROP_CNT_VAL_set       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value[150] )
 | void   slave_dpi_lfield_CH_DROP_CNT_VAL_get       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value[150] )
 | void   slave_dpi_lfield_range_CH_DROP_CNT_VAL_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 slave_dpi_lfield_range_CH_DROP_CNT_VAL_get ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SLAVE_DPI_IO_H
#define _SLAVE_DPI_IO_H

#include "cpb_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "slave_dpi_regs.h"
#include "slave_dpi_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 slave_dpi_buffer_init               ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | void                 slave_dpi_buffer_flush              ( slave_dpi_buffer_t *b_ptr )
 | UINT32               slave_dpi_reg_read                  ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 slave_dpi_reg_write                 ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 slave_dpi_field_set                 ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 slave_dpi_action_on_write_field_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 slave_dpi_burst_read                ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 slave_dpi_burst_write               ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE slave_dpi_poll                      ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 slave_dpi_reg_ZONE_CONFIG_write                           ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 slave_dpi_reg_ZONE_CONFIG_field_set                       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_ZONE_CONFIG_read                            ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 slave_dpi_reg_BURST_CONFIG_write                          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 slave_dpi_reg_BURST_CONFIG_field_set                      ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_BURST_CONFIG_read                           ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 slave_dpi_reg_HRD_CONFIG_write                            ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 slave_dpi_reg_HRD_CONFIG_field_set                        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_HRD_CONFIG_read                             ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 slave_dpi_reg_PMON_CNT_TRIG_write                         ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 slave_dpi_reg_PMON_CNT_TRIG_field_set                     ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_PMON_CNT_TRIG_read                          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE slave_dpi_reg_PMON_CNT_TRIG_poll                          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 slave_dpi_reg_PMON_MODE_TRIG_write                        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 slave_dpi_reg_PMON_MODE_TRIG_field_set                    ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_PMON_MODE_TRIG_read                         ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 slave_dpi_reg_PMON_PKT_TRIG_write                         ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 slave_dpi_reg_PMON_PKT_TRIG_field_set                     ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_PMON_PKT_TRIG_read                          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 slave_dpi_reg_CH_ENABLE_array_write                       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 slave_dpi_reg_CH_ENABLE_array_field_set                   ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_CH_ENABLE_array_read                        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 slave_dpi_reg_DROP_CNT_TRIG_REG_write                     ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 slave_dpi_reg_DROP_CNT_TRIG_REG_field_set                 ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_DROP_CNT_TRIG_REG_read                      ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | void                 slave_dpi_reg_CH_DROP_INT_array_write                     ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 slave_dpi_reg_CH_DROP_INT_array_action_on_write_field_set ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_CH_DROP_INT_array_read                      ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE slave_dpi_reg_CH_DROP_INT_array_poll                      ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 slave_dpi_reg_CH_DROP_COUNT_array_burst_write             ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 slave_dpi_reg_CH_DROP_COUNT_array_field_set               ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 slave_dpi_reg_CH_DROP_COUNT_array_burst_read              ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               slave_dpi_reg_CH_DROP_COUNT_array_read                    ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32 N )
 | 
 | void                 slave_dpi_reg_CH_CONFIG_array_write                       ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 slave_dpi_reg_CH_CONFIG_array_field_set                   ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_CH_CONFIG_array_read                        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 slave_dpi_reg_CH_Q_MAP_array_write                        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 slave_dpi_reg_CH_Q_MAP_array_field_set                    ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_CH_Q_MAP_array_read                         ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 slave_dpi_reg_ZONE_0_1_array_write                        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 slave_dpi_reg_ZONE_0_1_array_field_set                    ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_ZONE_0_1_array_read                         ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 slave_dpi_reg_ZONE_1_2_array_write                        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 slave_dpi_reg_ZONE_1_2_array_field_set                    ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_ZONE_1_2_array_read                         ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 slave_dpi_reg_ZONE_2_3_array_write                        ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 slave_dpi_reg_ZONE_2_3_array_field_set                    ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_ZONE_2_3_array_read                         ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 slave_dpi_reg_ZONE_OVRD_CONFIG_array_write                ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 slave_dpi_reg_ZONE_OVRD_CONFIG_array_field_set            ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               slave_dpi_reg_ZONE_OVRD_CONFIG_array_read                 ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32               slave_dpi_reg_PMON_CNT0_REG_read                          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               slave_dpi_reg_PMON_CNT1_REG_read                          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               slave_dpi_reg_PMON_CNT2_REG_read                          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               slave_dpi_reg_PMON_CNT3_REG_read                          ( slave_dpi_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 A )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SLAVE_DPI_IO_H */
