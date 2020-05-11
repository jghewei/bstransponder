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
 *     and register accessor functions for the enet_fege block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing enet_fege io functions is:
 | {
 |     enet_fege_buffer_t b_ptr[1];
 |     enet_fege_buffer_init( b_ptr, h_ptr );
 |     value = enet_fege_field_<FIELD>_get( b_ptr, h_ptr );
 |     enet_fege_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     enet_fege_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = enet_fege_field_<FIELD>_get( NULL, h_ptr );
 | enet_fege_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-------------------------------------------+
 | | Register 0x00000000 MODE_CFG              |
 | +-------------------------------------------+
 | | bit  31    R/W  TSTMP_CORR_UPD_BYPASS     |
 | | bit  29:25 R/W  TSTMP_CORR_UPD_SKIP_BYTES |
 | | bit  20    R/W  AUTO_ERR_CLEAR_EN         |
 | | bit  19    R/W  FORCE_ERR_PATT            |
 | | bit  18    R/W  CLEAR_ERR_PATT            |
 | | bit  17:16 R/W  IPG_RED_ON_THRESH         |
 | | bit  15:14 R/W  IPG_RED_OFF_THRESH        |
 | | bit  13:12 R/W  PAUSE_ON_THRESH           |
 | | bit  11:10 R/W  PAUSE_OFF_THRESH          |
 | | bit  9     R/W  MAC_LOWPWR                |
 | | bit  8     R/W  GE_GFPT_LOWPWR            |
 | | bit  7     R    L1RPP_FEGE_CARRIER_SENSE  |
 | | bit  4     R    L1RPP_FEGE_RXERROR_STATUS |
 | | bit  3     R/W  UNIDIRECTIONAL_MODE       |
 | | bit  2:0   R/W  FEGE_MODE                 |
 | +-------------------------------------------+
 | 
 | void   enet_fege_field_TSTMP_CORR_UPD_BYPASS_set           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TSTMP_CORR_UPD_BYPASS_get           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_TSTMP_CORR_UPD_SKIP_BYTES_set       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TSTMP_CORR_UPD_SKIP_BYTES_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_TSTMP_CORR_UPD_SKIP_BYTES_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_TSTMP_CORR_UPD_SKIP_BYTES_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_fege_field_AUTO_ERR_CLEAR_EN_set               ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_AUTO_ERR_CLEAR_EN_get               ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_IPG_RED_ON_THRESH_set               ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_IPG_RED_ON_THRESH_get               ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_IPG_RED_ON_THRESH_set         ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_IPG_RED_ON_THRESH_get         ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_fege_field_IPG_RED_OFF_THRESH_set              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_IPG_RED_OFF_THRESH_get              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_IPG_RED_OFF_THRESH_set        ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_IPG_RED_OFF_THRESH_get        ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_fege_field_PAUSE_ON_THRESH_set                 ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_PAUSE_ON_THRESH_get                 ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_PAUSE_ON_THRESH_set           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_PAUSE_ON_THRESH_get           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_fege_field_PAUSE_OFF_THRESH_set                ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_PAUSE_OFF_THRESH_get                ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_PAUSE_OFF_THRESH_set          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_PAUSE_OFF_THRESH_get          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_fege_field_MAC_LOWPWR_set                      ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_MAC_LOWPWR_get                      ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_GE_GFPT_LOWPWR_set                  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_GE_GFPT_LOWPWR_get                  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_UNIDIRECTIONAL_MODE_set             ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_UNIDIRECTIONAL_MODE_get             ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_FEGE_MODE_set                       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_FEGE_MODE_get                       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_FEGE_MODE_set                 ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_FEGE_MODE_get                 ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_fege_field_FORCE_ERR_PATT_set                  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_FORCE_ERR_PATT_get                  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_CLEAR_ERR_PATT_set                  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_CLEAR_ERR_PATT_get                  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 enet_fege_field_L1RPP_FEGE_CARRIER_SENSE_get        ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 enet_fege_field_L1RPP_FEGE_RXERROR_STATUS_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +--------------------------------+
 | | Register 0x00000004 SW_RESET   |
 | +--------------------------------+
 | | bit  10 R/W  TX_TSTMP_FIFO_RST |
 | | bit  9  R/W  PTP_NTP_RST       |
 | | bit  8  R/W  GE_GFPT_RST       |
 | | bit  7  R/W  MSTATX_RST        |
 | | bit  6  R/W  L1RPP_FEGE_RST    |
 | | bit  5  R/W  RMAC_RST          |
 | | bit  4  R/W  RX_ECLASS_RST     |
 | | bit  3  R/W  L1TPP_FEGE_RST    |
 | | bit  2  R/W  TMAC_RST          |
 | | bit  1  R/W  TX_ECLASS_RST     |
 | | bit  0  R/W  ENET_FEGE_RST     |
 | +--------------------------------+
 | 
 | void   enet_fege_field_TX_TSTMP_FIFO_RST_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TX_TSTMP_FIFO_RST_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_PTP_NTP_RST_set       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_PTP_NTP_RST_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_GE_GFPT_RST_set       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_GE_GFPT_RST_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_MSTATX_RST_set        ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_MSTATX_RST_get        ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_L1RPP_FEGE_RST_set    ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_L1RPP_FEGE_RST_get    ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_RMAC_RST_set          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_RMAC_RST_get          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_RX_ECLASS_RST_set     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_RX_ECLASS_RST_get     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_L1TPP_FEGE_RST_set    ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_L1TPP_FEGE_RST_get    ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_TMAC_RST_set          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TMAC_RST_get          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_TX_ECLASS_RST_set     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TX_ECLASS_RST_get     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_ENET_FEGE_RST_set     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_ENET_FEGE_RST_get     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x00000008 PTP_NTP_CFG         |
 | +-----------------------------------------+
 | | bit  31    R/W  RX_TSTMP_OFFSET_UPD     |
 | | bit  30    R/W  TX_TSTMP_OFFSET_UPD     |
 | | bit  29    R/W  RX_TSTMP_FRAC_STAT_SIGN |
 | | bit  28    R/W  RX_TSTMP_FRAC_DYN_SIGN  |
 | | bit  27    R/W  TX_TSTMP_FRAC_STAT_SIGN |
 | | bit  26    R/W  TX_TSTMP_FRAC_DYN_SIGN  |
 | | bit  23:16 R/W  CORR_POSITION           |
 | | bit  15:8  R/W  TSTMP_POSITION          |
 | | bit  7:5   R/W  TAG_FOR_UPD             |
 | | bit  4     R/W  ONE_STEP_CLK_FCS        |
 | | bit  2:1   R/W  TSTMP_MODE              |
 | | bit  0     R/W  PTP_NTP_EN              |
 | +-----------------------------------------+
 | 
 | void   enet_fege_field_RX_TSTMP_OFFSET_UPD_set     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_RX_TSTMP_OFFSET_UPD_get     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_TX_TSTMP_OFFSET_UPD_set     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TX_TSTMP_OFFSET_UPD_get     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_RX_TSTMP_FRAC_STAT_SIGN_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_RX_TSTMP_FRAC_STAT_SIGN_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_RX_TSTMP_FRAC_DYN_SIGN_set  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_RX_TSTMP_FRAC_DYN_SIGN_get  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_TX_TSTMP_FRAC_STAT_SIGN_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TX_TSTMP_FRAC_STAT_SIGN_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_TX_TSTMP_FRAC_DYN_SIGN_set  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TX_TSTMP_FRAC_DYN_SIGN_get  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_CORR_POSITION_set           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_CORR_POSITION_get           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_CORR_POSITION_set     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_CORR_POSITION_get     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_fege_field_TSTMP_POSITION_set          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TSTMP_POSITION_get          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_TSTMP_POSITION_set    ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_TSTMP_POSITION_get    ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_fege_field_TAG_FOR_UPD_set             ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TAG_FOR_UPD_get             ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_TAG_FOR_UPD_set       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_TAG_FOR_UPD_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_fege_field_ONE_STEP_CLK_FCS_set        ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_ONE_STEP_CLK_FCS_get        ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_TSTMP_MODE_set              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TSTMP_MODE_get              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_TSTMP_MODE_set        ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_TSTMP_MODE_get        ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_fege_field_PTP_NTP_EN_set              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_PTP_NTP_EN_get              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +--------------------------------------------------------------------+
 | | Register 0x0000000c PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET |
 | +--------------------------------------------------------------------+
 | | bit  31:0 R/W  RX_TSTMP_FRAC_DYN_OFFSET                            |
 | +--------------------------------------------------------------------+
 | 
 | void   enet_fege_field_RX_TSTMP_FRAC_DYN_OFFSET_set       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_RX_TSTMP_FRAC_DYN_OFFSET_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_RX_TSTMP_FRAC_DYN_OFFSET_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_RX_TSTMP_FRAC_DYN_OFFSET_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x00000010 PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  RX_TSTMP_FRAC_STAT_OFFSET                          |
 | +-------------------------------------------------------------------+
 | 
 | void   enet_fege_field_RX_TSTMP_FRAC_STAT_OFFSET_set       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_RX_TSTMP_FRAC_STAT_OFFSET_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_RX_TSTMP_FRAC_STAT_OFFSET_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_RX_TSTMP_FRAC_STAT_OFFSET_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------+
 | | Register 0x00000014 PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET |
 | +--------------------------------------------------------------------+
 | | bit  31:0 R/W  TX_TSTMP_FRAC_DYN_OFFSET                            |
 | +--------------------------------------------------------------------+
 | 
 | void   enet_fege_field_TX_TSTMP_FRAC_DYN_OFFSET_set       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TX_TSTMP_FRAC_DYN_OFFSET_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_TX_TSTMP_FRAC_DYN_OFFSET_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_TX_TSTMP_FRAC_DYN_OFFSET_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------------+
 | | Register 0x00000018 PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET |
 | +-------------------------------------------------------------------------+
 | | bit  31:0 R/W  TX_TSTMP_FRAC_STAT_OFFSET                                |
 | +-------------------------------------------------------------------------+
 | 
 | void   enet_fege_field_TX_TSTMP_FRAC_STAT_OFFSET_set       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TX_TSTMP_FRAC_STAT_OFFSET_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_TX_TSTMP_FRAC_STAT_OFFSET_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_TX_TSTMP_FRAC_STAT_OFFSET_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x0000001c L1RPP_FEGE_BIT_ALIGNMENT |
 | +----------------------------------------------+
 | | bit  4   R/W  AUTO_BIT_ALIGN_OFFSET          |
 | | bit  3:0 R    L1RPP_FEGE_BIT_ALIGN           |
 | +----------------------------------------------+
 | 
 | void   enet_fege_field_AUTO_BIT_ALIGN_OFFSET_set      ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_AUTO_BIT_ALIGN_OFFSET_get      ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 enet_fege_field_L1RPP_FEGE_BIT_ALIGN_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 enet_fege_field_range_L1RPP_FEGE_BIT_ALIGN_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000020 TX_TSTMP_FIFO_CFG |
 | +---------------------------------------+
 | | bit  8   R/W  ERR_PKT_DROP            |
 | | bit  7:0 R/W  EXTRACT_TAG             |
 | +---------------------------------------+
 | 
 | void   enet_fege_field_ERR_PKT_DROP_set      ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_ERR_PKT_DROP_get      ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_EXTRACT_TAG_set       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_EXTRACT_TAG_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_EXTRACT_TAG_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_EXTRACT_TAG_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000024 TX_TSTMP_FIFO_ACCESS |
 | +------------------------------------------+
 | | bit  1 R    TX_TSTMP_PKT_AVAIL           |
 | | bit  0 R/W  Q_BUSY_RD                    |
 | +------------------------------------------+
 | 
 | void                 enet_fege_field_Q_BUSY_RD_set          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               enet_fege_field_Q_BUSY_RD_get          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_field_Q_BUSY_RD_poll         ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               enet_fege_field_TX_TSTMP_PKT_AVAIL_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +-----------------------------------------------------------------+
 | | Register 0x00000028 TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP |
 | +-----------------------------------------------------------------+
 | | bit  31:24 R  TOD_TSTMP                                         |
 | | bit  5     R  Q_SOP                                             |
 | | bit  4:1   R  Q_EOP                                             |
 | | bit  0     R  Q_ERR                                             |
 | +-----------------------------------------------------------------+
 | 
 | UINT32 enet_fege_field_TOD_TSTMP_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 enet_fege_field_range_TOD_TSTMP_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 enet_fege_field_Q_SOP_get           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 enet_fege_field_Q_EOP_get           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 enet_fege_field_range_Q_EOP_get     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 enet_fege_field_Q_ERR_get           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +-----------------------------------------------+
 | | Register 0x0000002c TX_TSTMP_FIFO_PACKET_DATA |
 | +-----------------------------------------------+
 | | bit  31:0 R  Q_DATA                           |
 | +-----------------------------------------------+
 | 
 | UINT32 enet_fege_field_Q_DATA_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 enet_fege_field_range_Q_DATA_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000030 INT_EN             |
 | +----------------------------------------+
 | | bit  6 R/W  TX_TSTMPR_TOD_SIGN_OVRFL_E |
 | | bit  5 R/W  RX_TSTMPR_TOD_SIGN_OVRFL_E |
 | | bit  4 R/W  OVERFLOW_E                 |
 | | bit  3 R/W  RX_COMMA_DEL_CHG_E         |
 | | bit  2 R/W  TX_TSTMP_FIFO_E            |
 | | bit  1 R/W  TX_SOF_E                   |
 | | bit  0 R/W  RX_SOF_E                   |
 | +----------------------------------------+
 | 
 | void   enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_E_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_E_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_E_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_E_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_OVERFLOW_E_set                 ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_OVERFLOW_E_get                 ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_RX_COMMA_DEL_CHG_E_set         ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_RX_COMMA_DEL_CHG_E_get         ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_TX_TSTMP_FIFO_E_set            ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TX_TSTMP_FIFO_E_get            ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_TX_SOF_E_set                   ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_TX_SOF_E_get                   ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_RX_SOF_E_set                   ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_RX_SOF_E_get                   ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000034 INT_VAL          |
 | +--------------------------------------+
 | | bit  6 R  TX_TSTMPR_TOD_SIGN_OVRFL_V |
 | | bit  5 R  RX_TSTMPR_TOD_SIGN_OVRFL_V |
 | +--------------------------------------+
 | 
 | UINT32               enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_V_get  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_V_poll ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_V_get  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_V_poll ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x00000038 INT                |
 | +----------------------------------------+
 | | bit  6 R/W  TX_TSTMPR_TOD_SIGN_OVRFL_I |
 | | bit  5 R/W  RX_TSTMPR_TOD_SIGN_OVRFL_I |
 | | bit  4 R/W  OVERFLOW_I                 |
 | | bit  3 R/W  RX_COMMA_DEL_CHG_I         |
 | | bit  2 R/W  TX_TSTMP_FIFO_I            |
 | | bit  1 R/W  TX_SOF_I                   |
 | | bit  0 R/W  RX_SOF_I                   |
 | +----------------------------------------+
 | 
 | void                 enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_I_set_to_clear ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_I_get          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_field_TX_TSTMPR_TOD_SIGN_OVRFL_I_poll         ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_I_set_to_clear ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_I_get          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_field_RX_TSTMPR_TOD_SIGN_OVRFL_I_poll         ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enet_fege_field_OVERFLOW_I_set_to_clear                 ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               enet_fege_field_OVERFLOW_I_get                          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_field_OVERFLOW_I_poll                         ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enet_fege_field_RX_COMMA_DEL_CHG_I_set_to_clear         ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               enet_fege_field_RX_COMMA_DEL_CHG_I_get                  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_field_RX_COMMA_DEL_CHG_I_poll                 ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enet_fege_field_TX_TSTMP_FIFO_I_set_to_clear            ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               enet_fege_field_TX_TSTMP_FIFO_I_get                     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_field_TX_TSTMP_FIFO_I_poll                    ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enet_fege_field_TX_SOF_I_set_to_clear                   ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               enet_fege_field_TX_SOF_I_get                            ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_field_TX_SOF_I_poll                           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enet_fege_field_RX_SOF_I_set_to_clear                   ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               enet_fege_field_RX_SOF_I_get                            ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_field_RX_SOF_I_poll                           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------+
 | | Register 0x0000003c INT_INTP_EN |
 | +---------------------------------+
 | | bit  3 R/W  FEGE_P_E            |
 | | bit  2 R/W  MSTATX_P_E          |
 | | bit  0 R/W  L1RPP_FEGE_P_E      |
 | +---------------------------------+
 | 
 | void   enet_fege_field_FEGE_P_E_set       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_FEGE_P_E_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_MSTATX_P_E_set     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_MSTATX_P_E_get     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_L1RPP_FEGE_P_E_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_L1RPP_FEGE_P_E_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00000040 INT_INTM_EN |
 | +---------------------------------+
 | | bit  3 R/W  FEGE_M_E            |
 | | bit  2 R/W  MSTATX_M_E          |
 | | bit  0 R/W  L1RPP_FEGE_M_E      |
 | +---------------------------------+
 | 
 | void   enet_fege_field_FEGE_M_E_set       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_FEGE_M_E_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_MSTATX_M_E_set     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_MSTATX_M_E_get     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_L1RPP_FEGE_M_E_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_L1RPP_FEGE_M_E_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00000044 INT_INTD_EN |
 | +---------------------------------+
 | | bit  3 R/W  FEGE_D_E            |
 | | bit  2 R/W  MSTATX_D_E          |
 | | bit  0 R/W  L1RPP_FEGE_D_E      |
 | +---------------------------------+
 | 
 | void   enet_fege_field_FEGE_D_E_set       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_FEGE_D_E_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_MSTATX_D_E_set     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_MSTATX_D_E_get     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_L1RPP_FEGE_D_E_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_L1RPP_FEGE_D_E_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00000048 INT_SUMMARY |
 | +---------------------------------+
 | | bit  3 R  FEGE_INT              |
 | | bit  2 R  MSTATX_INT            |
 | | bit  0 R  L1RPP_FEGE_INT        |
 | +---------------------------------+
 | 
 | UINT32 enet_fege_field_FEGE_INT_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 enet_fege_field_MSTATX_INT_get     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 enet_fege_field_L1RPP_FEGE_INT_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x0000004c TIP_SUMMARY |
 | +---------------------------------+
 | | bit  2 R  MSTATX_TIP            |
 | | bit  0 R  L1RPP_FEGE_TIP        |
 | +---------------------------------+
 | 
 | UINT32               enet_fege_field_MSTATX_TIP_get      ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_field_MSTATX_TIP_poll     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               enet_fege_field_L1RPP_FEGE_TIP_get  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_field_L1RPP_FEGE_TIP_poll ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000058 UDP_CHKSUM_CORRECTION_CFG |
 | +-----------------------------------------------+
 | | bit  11  R/W  UDP_EN                          |
 | | bit  10  R/W  UDP_CORR_POS_EN                 |
 | | bit  9   R/W  UDP_CORR_POS_UPD                |
 | | bit  8:0 R/W  UDP_CORR_POS                    |
 | +-----------------------------------------------+
 | 
 | void   enet_fege_field_UDP_EN_set             ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_UDP_EN_get             ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_UDP_CORR_POS_EN_set    ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_UDP_CORR_POS_EN_get    ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_UDP_CORR_POS_set       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_UDP_CORR_POS_get       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   enet_fege_field_range_UDP_CORR_POS_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enet_fege_field_range_UDP_CORR_POS_get ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enet_fege_field_UDP_CORR_POS_UPD_set   ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 enet_fege_field_UDP_CORR_POS_UPD_get   ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ENET_FEGE_IO_H
#define _ENET_FEGE_IO_H

#include "enet_fege_loc.h"
#include "enet_fege_regs.h"
#include "enet_fege_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 enet_fege_buffer_init               ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void                 enet_fege_buffer_flush              ( enet_fege_buffer_t *b_ptr )
 | UINT32               enet_fege_reg_read                  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 enet_fege_reg_write                 ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 enet_fege_field_set                 ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 enet_fege_action_on_write_field_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 enet_fege_burst_read                ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 enet_fege_burst_write               ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE enet_fege_poll                      ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 enet_fege_reg_MODE_CFG_write                                                ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_MODE_CFG_field_set                                            ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_MODE_CFG_read                                                 ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 enet_fege_reg_SW_RESET_write                                                ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_SW_RESET_field_set                                            ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_SW_RESET_read                                                 ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 enet_fege_reg_PTP_NTP_CFG_write                                             ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_PTP_NTP_CFG_field_set                                         ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_PTP_NTP_CFG_read                                              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_write          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_field_set      ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_read           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_write           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_field_set       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_PTP_NTP_RX_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_read            ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_write          ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_field_set      ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_PTP_NTP_TX_TIMESTAMP_FRACTIONAL_DYNAMIC_OFFSET_read           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_write     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_field_set ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_PTP_NTP_TRASNMIT_TIMESTAMP_FRACTIONAL_STATIC_OFFSET_read      ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_write                                ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_field_set                            ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_L1RPP_FEGE_BIT_ALIGNMENT_read                                 ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 enet_fege_reg_TX_TSTMP_FIFO_CFG_write                                       ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_TX_TSTMP_FIFO_CFG_field_set                                   ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_TX_TSTMP_FIFO_CFG_read                                        ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 enet_fege_reg_INT_INTP_EN_write                                             ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_INT_INTP_EN_field_set                                         ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_INT_INTP_EN_read                                              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 enet_fege_reg_INT_INTM_EN_write                                             ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_INT_INTM_EN_field_set                                         ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_INT_INTM_EN_read                                              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 enet_fege_reg_INT_INTD_EN_write                                             ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_INT_INTD_EN_field_set                                         ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_INT_INTD_EN_read                                              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_write                               ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_field_set                           ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_UDP_CHKSUM_CORRECTION_CFG_read                                ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 enet_fege_reg_TX_TSTMP_FIFO_ACCESS_write                                    ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_TX_TSTMP_FIFO_ACCESS_field_set                                ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_TX_TSTMP_FIFO_ACCESS_read                                     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_reg_TX_TSTMP_FIFO_ACCESS_poll                                     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 enet_fege_reg_INT_write                                                     ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_INT_action_on_write_field_set                                 ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_INT_read                                                      ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_reg_INT_poll                                                      ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 enet_fege_reg_INT_EN_write                                                  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 enet_fege_reg_INT_EN_field_set                                              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enet_fege_reg_INT_EN_read                                                   ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               enet_fege_reg_INT_VAL_read                                                  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_reg_INT_VAL_poll                                                  ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               enet_fege_reg_TX_TSTMP_FIFO_PACKET_STAT_AND_TOD_TIMESTAMP_read              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               enet_fege_reg_TX_TSTMP_FIFO_PACKET_DATA_read                                ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               enet_fege_reg_INT_SUMMARY_read                                              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               enet_fege_reg_TIP_SUMMARY_read                                              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enet_fege_reg_TIP_SUMMARY_poll                                              ( enet_fege_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ENET_FEGE_IO_H */
