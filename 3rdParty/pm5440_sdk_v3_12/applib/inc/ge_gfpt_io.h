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
 *     and register accessor functions for the ge_gfpt block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing ge_gfpt io functions is:
 | {
 |     ge_gfpt_buffer_t b_ptr[1];
 |     ge_gfpt_buffer_init( b_ptr, h_ptr );
 |     value = ge_gfpt_field_<FIELD>_get( b_ptr, h_ptr );
 |     ge_gfpt_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     ge_gfpt_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = ge_gfpt_field_<FIELD>_get( NULL, h_ptr );
 | ge_gfpt_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------+
 | | Register 0x00002100 GENERAL_CFG |
 | +---------------------------------+
 | | bit  0 R/W  SOFT_RESET          |
 | +---------------------------------+
 | 
 | void   ge_gfpt_field_SOFT_RESET_set ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_SOFT_RESET_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00002104 RX_CFG           |
 | +--------------------------------------+
 | | bit  24:21 R/W  RX_RESYNC_COUNT      |
 | | bit  20:17 R/W  RX_LOS_COUNT         |
 | | bit  16:13 R/W  RX_SYNC_COUNT        |
 | | bit  12:3  R/W  RX_10BIT_PATTERN     |
 | | bit  2     R/W  RX_10BIT_PATT_UPDATE |
 | | bit  1     R/W  RX_10BIT_PATT_EN     |
 | | bit  0     R/W  RX_SOFT_RESET        |
 | +--------------------------------------+
 | 
 | void   ge_gfpt_field_RX_RESYNC_COUNT_set        ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_RX_RESYNC_COUNT_get        ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_range_RX_RESYNC_COUNT_set  ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ge_gfpt_field_range_RX_RESYNC_COUNT_get  ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ge_gfpt_field_RX_LOS_COUNT_set           ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_RX_LOS_COUNT_get           ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_range_RX_LOS_COUNT_set     ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ge_gfpt_field_range_RX_LOS_COUNT_get     ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ge_gfpt_field_RX_SYNC_COUNT_set          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_RX_SYNC_COUNT_get          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_range_RX_SYNC_COUNT_set    ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ge_gfpt_field_range_RX_SYNC_COUNT_get    ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ge_gfpt_field_RX_10BIT_PATTERN_set       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_RX_10BIT_PATTERN_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_range_RX_10BIT_PATTERN_set ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ge_gfpt_field_range_RX_10BIT_PATTERN_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ge_gfpt_field_RX_10BIT_PATT_EN_set       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_RX_10BIT_PATT_EN_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_RX_SOFT_RESET_set          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_RX_SOFT_RESET_get          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_RX_10BIT_PATT_UPDATE_set   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_RX_10BIT_PATT_UPDATE_get   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00002108 RX_20_BIT_PATTERN |
 | +---------------------------------------+
 | | bit  21:2 R/W  RX_20BIT_PATTERN       |
 | | bit  1    R/W  RX_20BIT_PATT_UPDATE   |
 | | bit  0    R/W  RX_20BIT_PATT_EN       |
 | +---------------------------------------+
 | 
 | void   ge_gfpt_field_RX_20BIT_PATTERN_set       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_RX_20BIT_PATTERN_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_range_RX_20BIT_PATTERN_set ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ge_gfpt_field_range_RX_20BIT_PATTERN_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ge_gfpt_field_RX_20BIT_PATT_EN_set       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_RX_20BIT_PATT_EN_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_RX_20BIT_PATT_UPDATE_set   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_RX_20BIT_PATT_UPDATE_get   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00002110 RX_LCV_CNT0 |
 | +---------------------------------+
 | | bit  31:0 R  RXGE_GFPT_LCV_CNT  |
 | +---------------------------------+
 | 
 | void   ge_gfpt_lfield_RXGE_GFPT_LCV_CNT_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 ge_gfpt_lfield_range_RXGE_GFPT_LCV_CNT_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00002118 RX_IDLES_DELETED_CNT0 |
 | +-------------------------------------------+
 | | bit  31:0 R  RXGE_GFPT_IDLES_DEL_CNT      |
 | +-------------------------------------------+
 | 
 | void   ge_gfpt_lfield_RXGE_GFPT_IDLES_DEL_CNT_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 ge_gfpt_lfield_range_RXGE_GFPT_IDLES_DEL_CNT_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00002120 RX_CFG_ORDER_SETS_DELETED_CNT0 |
 | +----------------------------------------------------+
 | | bit  31:0 R  RXGE_GFPT_CONF_DEL_CNT                |
 | +----------------------------------------------------+
 | 
 | void   ge_gfpt_lfield_RXGE_GFPT_CONF_DEL_CNT_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 ge_gfpt_lfield_range_RXGE_GFPT_CONF_DEL_CNT_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------+
 | | Register 0x00002128 RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0 |
 | +----------------------------------------------------------------+
 | | bit  31:0 R  RXGE_GFPT_PROPGERR_DEL_CNT                        |
 | +----------------------------------------------------------------+
 | 
 | void   ge_gfpt_lfield_RXGE_GFPT_PROPGERR_DEL_CNT_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 ge_gfpt_lfield_range_RXGE_GFPT_PROPGERR_DEL_CNT_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00002130 RX_PACKET_DELETED_CNT |
 | +-------------------------------------------+
 | | bit  23:0 R  RXGE_GFPT_PKT_DEL_CNT        |
 | +-------------------------------------------+
 | 
 | UINT32 ge_gfpt_field_RXGE_GFPT_PKT_DEL_CNT_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 ge_gfpt_field_range_RXGE_GFPT_PKT_DEL_CNT_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00002134 RX_PREAMBLE_PACKET_DELETED_CNT |
 | +----------------------------------------------------+
 | | bit  23:0 R  RXGE_GFPT_PREPKT_DEL_CNT              |
 | +----------------------------------------------------+
 | 
 | UINT32 ge_gfpt_field_RXGE_GFPT_PREPKT_DEL_CNT_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | UINT32 ge_gfpt_field_range_RXGE_GFPT_PREPKT_DEL_CNT_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00002138 RX_PMON_UPDATE |
 | +------------------------------------+
 | | bit  0 R/W  RX_PMON_UPDATE         |
 | +------------------------------------+
 | 
 | void   ge_gfpt_field_RX_PMON_UPDATE_set ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_RX_PMON_UPDATE_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x0000213c TX_CFG           |
 | +--------------------------------------+
 | | bit  26:23 R/W  TX_RESYNC_COUNT      |
 | | bit  22:19 R/W  TX_LOS_COUNT         |
 | | bit  18:15 R/W  TX_SYNC_COUNT        |
 | | bit  14    R/W  DISP_INVERSION_EN    |
 | | bit  13    R/W  CLK_ADAPT_FIFO_RESET |
 | | bit  12:3  R/W  TX_10BIT_PATTERN     |
 | | bit  2     R/W  TX_10BIT_PATT_UPDATE |
 | | bit  1     R/W  TX_10BIT_PATT_EN     |
 | | bit  0     R/W  TX_SOFT_RESET        |
 | +--------------------------------------+
 | 
 | void   ge_gfpt_field_TX_RESYNC_COUNT_set        ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TX_RESYNC_COUNT_get        ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_range_TX_RESYNC_COUNT_set  ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ge_gfpt_field_range_TX_RESYNC_COUNT_get  ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ge_gfpt_field_TX_LOS_COUNT_set           ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TX_LOS_COUNT_get           ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_range_TX_LOS_COUNT_set     ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ge_gfpt_field_range_TX_LOS_COUNT_get     ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ge_gfpt_field_TX_SYNC_COUNT_set          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TX_SYNC_COUNT_get          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_range_TX_SYNC_COUNT_set    ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ge_gfpt_field_range_TX_SYNC_COUNT_get    ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ge_gfpt_field_DISP_INVERSION_EN_set      ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_DISP_INVERSION_EN_get      ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_CLK_ADAPT_FIFO_RESET_set   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_CLK_ADAPT_FIFO_RESET_get   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_TX_10BIT_PATTERN_set       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TX_10BIT_PATTERN_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_range_TX_10BIT_PATTERN_set ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ge_gfpt_field_range_TX_10BIT_PATTERN_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ge_gfpt_field_TX_10BIT_PATT_EN_set       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TX_10BIT_PATT_EN_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_TX_SOFT_RESET_set          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TX_SOFT_RESET_get          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_TX_10BIT_PATT_UPDATE_set   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TX_10BIT_PATT_UPDATE_get   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00002140 TX_20_BIT_PATTERN |
 | +---------------------------------------+
 | | bit  21:2 R/W  TX_20BIT_PATTERN       |
 | | bit  1    R/W  TX_20BIT_PATT_UPDATE   |
 | | bit  0    R/W  TX_20BIT_PATT_EN       |
 | +---------------------------------------+
 | 
 | void   ge_gfpt_field_TX_20BIT_PATTERN_set       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TX_20BIT_PATTERN_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_range_TX_20BIT_PATTERN_set ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ge_gfpt_field_range_TX_20BIT_PATTERN_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ge_gfpt_field_TX_20BIT_PATT_EN_set       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TX_20BIT_PATT_EN_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_TX_20BIT_PATT_UPDATE_set   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TX_20BIT_PATT_UPDATE_get   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +--------------------------------------------------------+
 | | Register 0x00002144 TX_RATE_ADAPTATION_DELETE_INTERVAL |
 | +--------------------------------------------------------+
 | | bit  15:0 R/W  TXGE_GFPT_DELETE_INTERVAL               |
 | +--------------------------------------------------------+
 | 
 | void   ge_gfpt_field_TXGE_GFPT_DELETE_INTERVAL_set       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TXGE_GFPT_DELETE_INTERVAL_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_range_TXGE_GFPT_DELETE_INTERVAL_set ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ge_gfpt_field_range_TXGE_GFPT_DELETE_INTERVAL_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register 0x00002148 TX_RATE_ADAPTATION_INSERT_INTERVAL |
 | +--------------------------------------------------------+
 | | bit  15:0 R/W  TXGE_GFPT_INSERT_INTERVAL               |
 | +--------------------------------------------------------+
 | 
 | void   ge_gfpt_field_TXGE_GFPT_INSERT_INTERVAL_set       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TXGE_GFPT_INSERT_INTERVAL_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_range_TXGE_GFPT_INSERT_INTERVAL_set ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ge_gfpt_field_range_TXGE_GFPT_INSERT_INTERVAL_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x0000214c TX_LCV_CNT0 |
 | +---------------------------------+
 | | bit  31:0 R  TXGE_GFPT_LCV_CNT  |
 | +---------------------------------+
 | 
 | void   ge_gfpt_lfield_TXGE_GFPT_LCV_CNT_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 ge_gfpt_lfield_range_TXGE_GFPT_LCV_CNT_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00002154 TX_IDLES_DELETED_CNT0 |
 | +-------------------------------------------+
 | | bit  31:0 R  TXGE_GFPT_IDLES_DEL_CNT      |
 | +-------------------------------------------+
 | 
 | void   ge_gfpt_lfield_TXGE_GFPT_IDLES_DEL_CNT_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 ge_gfpt_lfield_range_TXGE_GFPT_IDLES_DEL_CNT_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000215c TX_CFG_ORDER_SETS_DELETED_CNT0 |
 | +----------------------------------------------------+
 | | bit  31:0 R  TXGE_GFPT_CONF_DEL_CNT                |
 | +----------------------------------------------------+
 | 
 | void   ge_gfpt_lfield_TXGE_GFPT_CONF_DEL_CNT_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 ge_gfpt_lfield_range_TXGE_GFPT_CONF_DEL_CNT_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------+
 | | Register 0x00002164 TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0 |
 | +----------------------------------------------------------------+
 | | bit  31:0 R  TXGE_GFPT_PROPAGERR_DEL_CNT                       |
 | +----------------------------------------------------------------+
 | 
 | void   ge_gfpt_lfield_TXGE_GFPT_PROPAGERR_DEL_CNT_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 ge_gfpt_lfield_range_TXGE_GFPT_PROPAGERR_DEL_CNT_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0000216c TX_IDLES_INSERTED_CNT0 |
 | +--------------------------------------------+
 | | bit  31:0 R  TXGE_GFPT_IDLES_INSRT_CNT     |
 | +--------------------------------------------+
 | 
 | void   ge_gfpt_lfield_TXGE_GFPT_IDLES_INSRT_CNT_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 ge_gfpt_lfield_range_TXGE_GFPT_IDLES_INSRT_CNT_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register 0x00002174 TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0 |
 | +--------------------------------------------------------------+
 | | bit  31:0 R  TXGE_GFPT_CONF_INSRT_CNT                        |
 | +--------------------------------------------------------------+
 | 
 | void   ge_gfpt_lfield_TXGE_GFPT_CONF_INSRT_CNT_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 ge_gfpt_lfield_range_TXGE_GFPT_CONF_INSRT_CNT_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register 0x0000217c TX_PROPAGATION_ERRS_INSERTED_CNT0 |
 | +-------------------------------------------------------+
 | | bit  31:0 R  TXGE_GFPT_PROPG_INSRT_CNT                |
 | +-------------------------------------------------------+
 | 
 | void   ge_gfpt_lfield_TXGE_GFPT_PROPG_INSRT_CNT_get       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 ge_gfpt_lfield_range_TXGE_GFPT_PROPG_INSRT_CNT_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00002184 TX_PMON_UPDATE |
 | +------------------------------------+
 | | bit  0 R/W  TX_PMON_UPDATE         |
 | +------------------------------------+
 | 
 | void   ge_gfpt_field_TX_PMON_UPDATE_set ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TX_PMON_UPDATE_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00002188 DEFECT_STAT |
 | +---------------------------------+
 | | bit  1 R  TX_LOS_V              |
 | | bit  0 R  RX_LOS_V              |
 | +---------------------------------+
 | 
 | UINT32               ge_gfpt_field_TX_LOS_V_get  ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ge_gfpt_field_TX_LOS_V_poll ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               ge_gfpt_field_RX_LOS_V_get  ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ge_gfpt_field_RX_LOS_V_poll ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------+
 | | Register 0x0000218c INT_EN   |
 | +------------------------------+
 | | bit  2 R/W  TX_DISP_INVERT_E |
 | | bit  1 R/W  TX_LOS_E         |
 | | bit  0 R/W  RX_LOS_E         |
 | +------------------------------+
 | 
 | void   ge_gfpt_field_TX_DISP_INVERT_E_set ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TX_DISP_INVERT_E_get ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_TX_LOS_E_set         ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_TX_LOS_E_get         ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   ge_gfpt_field_RX_LOS_E_set         ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 ge_gfpt_field_RX_LOS_E_get         ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x00002190 INT       |
 | +-------------------------------+
 | | bit  2 R/W  TX_DISP_INVERT_IC |
 | | bit  1 R/W  TX_LOS_I          |
 | | bit  0 R/W  RX_LOS_I          |
 | +-------------------------------+
 | 
 | void                 ge_gfpt_field_TX_DISP_INVERT_IC_set_to_clear ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               ge_gfpt_field_TX_DISP_INVERT_IC_get          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ge_gfpt_field_TX_DISP_INVERT_IC_poll         ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 ge_gfpt_field_TX_LOS_I_set_to_clear          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               ge_gfpt_field_TX_LOS_I_get                   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ge_gfpt_field_TX_LOS_I_poll                  ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 ge_gfpt_field_RX_LOS_I_set_to_clear          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32               ge_gfpt_field_RX_LOS_I_get                   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ge_gfpt_field_RX_LOS_I_poll                  ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _GE_GFPT_IO_H
#define _GE_GFPT_IO_H

#include "enet_fege_loc.h"
#include "ge_gfpt_regs.h"
#include "ge_gfpt_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 ge_gfpt_buffer_init               ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void                 ge_gfpt_buffer_flush              ( ge_gfpt_buffer_t *b_ptr )
 | UINT32               ge_gfpt_reg_read                  ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 ge_gfpt_reg_write                 ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 ge_gfpt_field_set                 ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 ge_gfpt_action_on_write_field_set ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 ge_gfpt_burst_read                ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 ge_gfpt_burst_write               ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE ge_gfpt_poll                      ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 ge_gfpt_reg_GENERAL_CFG_write                               ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 ge_gfpt_reg_GENERAL_CFG_field_set                           ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ge_gfpt_reg_GENERAL_CFG_read                                ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 ge_gfpt_reg_RX_CFG_write                                    ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 ge_gfpt_reg_RX_CFG_field_set                                ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ge_gfpt_reg_RX_CFG_read                                     ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 ge_gfpt_reg_RX_20_BIT_PATTERN_write                         ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 ge_gfpt_reg_RX_20_BIT_PATTERN_field_set                     ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ge_gfpt_reg_RX_20_BIT_PATTERN_read                          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 ge_gfpt_reg_TX_CFG_write                                    ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 ge_gfpt_reg_TX_CFG_field_set                                ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ge_gfpt_reg_TX_CFG_read                                     ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 ge_gfpt_reg_TX_20_BIT_PATTERN_write                         ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 ge_gfpt_reg_TX_20_BIT_PATTERN_field_set                     ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ge_gfpt_reg_TX_20_BIT_PATTERN_read                          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_write        ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_field_set    ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ge_gfpt_reg_TX_RATE_ADAPTATION_DELETE_INTERVAL_read         ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_write        ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_field_set    ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ge_gfpt_reg_TX_RATE_ADAPTATION_INSERT_INTERVAL_read         ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 ge_gfpt_reg_RX_PMON_UPDATE_write                            ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 ge_gfpt_reg_RX_PMON_UPDATE_field_set                        ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ge_gfpt_reg_RX_PMON_UPDATE_read                             ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 ge_gfpt_reg_TX_PMON_UPDATE_write                            ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 ge_gfpt_reg_TX_PMON_UPDATE_field_set                        ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ge_gfpt_reg_TX_PMON_UPDATE_read                             ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void                 ge_gfpt_reg_INT_write                                       ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 ge_gfpt_reg_INT_action_on_write_field_set                   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ge_gfpt_reg_INT_read                                        ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ge_gfpt_reg_INT_poll                                        ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 ge_gfpt_reg_INT_EN_write                                    ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void                 ge_gfpt_reg_INT_EN_field_set                                ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ge_gfpt_reg_INT_EN_read                                     ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_DEFECT_STAT_read                                ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ge_gfpt_reg_DEFECT_STAT_poll                                ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               ge_gfpt_reg_RX_LCV_CNT0_read                                ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_RX_LCV_CNT1_read                                ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_RX_IDLES_DELETED_CNT0_read                      ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_RX_IDLES_DELETED_CNT1_read                      ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_RX_CFG_ORDER_SETS_DELETED_CNT0_read             ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_CFG_ORDER_SETS_DELETED_CNT1_read                ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_read ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_RX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_read ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_RX_PACKET_DELETED_CNT_read                      ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_RX_PREAMBLE_PACKET_DELETED_CNT_read             ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_TX_LCV_CNT0_read                                ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_TX_LCV_CNT1_read                                ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_TX_IDLES_DELETED_CNT0_read                      ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_TX_IDLES_DELETED_CNT1_read                      ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_TX_CFG_ORDER_SETS_DELETED_CNT0_read             ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_TX_CFG_ORDER_SETS_DELETED_CNT1_read             ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT0_read ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_TX_PROPAGATION_ERR_CHARACTERS_DELETED_CNT1_read ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_TX_IDLES_INSERTED_CNT0_read                     ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_TX_IDLES_INSERTED_CNT1_read                     ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT0_read   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_TX_CONFIGUATION_ORDER_SETS_INSERTED_CNT1_read   ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_TX_PROPAGATION_ERRS_INSERTED_CNT0_read          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32               ge_gfpt_reg_TX_PROPAGATION_ERRS_INSERTED_CNT1_read          ( ge_gfpt_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _GE_GFPT_IO_H */
