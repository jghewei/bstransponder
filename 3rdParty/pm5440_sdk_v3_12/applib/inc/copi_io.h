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
 *     and register accessor functions for the copi block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing copi io functions is:
 | {
 |     copi_buffer_t b_ptr[1];
 |     copi_buffer_init( b_ptr, h_ptr, A );
 |     value = copi_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     copi_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     copi_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = copi_field_<FIELD>_get( NULL, h_ptr, A );
 | copi_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------+
 | | Register 0x00004000 SOFT_RESET    |
 | +-----------------------------------+
 | | bit  18 R/W  OOB_LOWPWR_EN        |
 | | bit  17 R/W  ODUK_LOWPWR_EN       |
 | | bit  16 R/W  CPB_LOWPWR_EN        |
 | | bit  6  R/W  ODUK_TX_SOFT         |
 | | bit  5  R/W  CPB_TX_SOFT          |
 | | bit  4  R/W  IL_SOFT_RST_RX_FC_X2 |
 | | bit  3  R/W  IL_RX_SOFT           |
 | | bit  2  R/W  IL_TX_SOFT           |
 | | bit  1  R/W  ILAKEN_SOFT          |
 | | bit  0  R/W  COPI_SOFT            |
 | +-----------------------------------+
 | 
 | void   copi_field_OOB_LOWPWR_EN_set        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_OOB_LOWPWR_EN_get        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_ODUK_LOWPWR_EN_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_ODUK_LOWPWR_EN_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_CPB_LOWPWR_EN_set        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_CPB_LOWPWR_EN_get        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_ODUK_TX_SOFT_set         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_ODUK_TX_SOFT_get         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_CPB_TX_SOFT_set          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_CPB_TX_SOFT_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_IL_SOFT_RST_RX_FC_X2_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_IL_SOFT_RST_RX_FC_X2_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_IL_RX_SOFT_set           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_IL_RX_SOFT_get           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_IL_TX_SOFT_set           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_IL_TX_SOFT_get           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_ILAKEN_SOFT_set          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_ILAKEN_SOFT_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_COPI_SOFT_set            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_COPI_SOFT_get            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------+
 | | Register 0x00004004 CFG      |
 | +------------------------------+
 | | bit  9:8 R/W  DIAG_CNTR_MODE |
 | | bit  0   R/W  PMON_UPDATE    |
 | +------------------------------+
 | 
 | void   copi_field_DIAG_CNTR_MODE_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_DIAG_CNTR_MODE_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_DIAG_CNTR_MODE_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_DIAG_CNTR_MODE_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_PMON_UPDATE_set          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_PMON_UPDATE_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------+
 | | Register 0x00004008 TX_INTF_CFG      |
 | +--------------------------------------+
 | | bit  11:10 R/W  TX_ODUK_6B_INS_EN    |
 | | bit  9     R/W  TX_ODUK_CHEXT_EN     |
 | | bit  8:7   R/W  TX_ODUK_BURSTMAX     |
 | | bit  6     R/W  TX_ODUK_INTERFACE_EN |
 | | bit  5     R/W  TX_ODUK_PKT_MODE     |
 | | bit  4     R/W  TX_CPB_TYPE_EN       |
 | | bit  3:2   R/W  TX_CPB_BURSTMAX      |
 | | bit  1     R/W  TX_CPB_INTERFACE_EN  |
 | | bit  0     R/W  TX_CPB_PKT_MODE      |
 | +--------------------------------------+
 | 
 | void   copi_field_TX_ODUK_6B_INS_EN_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TX_ODUK_6B_INS_EN_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_TX_ODUK_6B_INS_EN_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_TX_ODUK_6B_INS_EN_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_TX_ODUK_CHEXT_EN_set        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TX_ODUK_CHEXT_EN_get        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_TX_ODUK_BURSTMAX_set        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TX_ODUK_BURSTMAX_get        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_TX_ODUK_BURSTMAX_set  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_TX_ODUK_BURSTMAX_get  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_TX_ODUK_INTERFACE_EN_set    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TX_ODUK_INTERFACE_EN_get    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_TX_ODUK_PKT_MODE_set        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TX_ODUK_PKT_MODE_get        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_TX_CPB_TYPE_EN_set          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TX_CPB_TYPE_EN_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_TX_CPB_BURSTMAX_set         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TX_CPB_BURSTMAX_get         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_TX_CPB_BURSTMAX_set   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_TX_CPB_BURSTMAX_get   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_TX_CPB_INTERFACE_EN_set     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TX_CPB_INTERFACE_EN_get     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_TX_CPB_PKT_MODE_set         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TX_CPB_PKT_MODE_get         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------+
 | | Register 0x00004010 RX_INTF_CFG  |
 | +----------------------------------+
 | | bit  18    R/W  RX_ODUK_2B_MODE  |
 | | bit  17:16 R/W  RX_ODUK_6B_CHK   |
 | | bit  9     R/W  RX_ODUK_CHEXT_EN |
 | | bit  4     R/W  RX_CPB_TYPE_EN   |
 | | bit  3:2   R/W  RX_BURSTMAX      |
 | | bit  1     R/W  RX_INTERFACE_EN  |
 | | bit  0     R/W  RX_PKT_MODE      |
 | +----------------------------------+
 | 
 | void   copi_field_RX_ODUK_2B_MODE_set      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_ODUK_2B_MODE_get      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_ODUK_6B_CHK_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_ODUK_6B_CHK_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_RX_ODUK_6B_CHK_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_RX_ODUK_6B_CHK_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_RX_ODUK_CHEXT_EN_set     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_ODUK_CHEXT_EN_get     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_CPB_TYPE_EN_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_CPB_TYPE_EN_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_BURSTMAX_set          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_BURSTMAX_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_RX_BURSTMAX_set    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_RX_BURSTMAX_get    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_RX_INTERFACE_EN_set      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_INTERFACE_EN_get      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_PKT_MODE_set          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_PKT_MODE_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------------+
 | | Register (0x00004014 + (N) * 0x4) TX_FIFO_TH |
 | |     N[0..1]                                  |
 | +----------------------------------------------+
 | | bit  22:16 R/W  TX_H_WM                      |
 | | bit  6:0   R/W  TX_L_WM                      |
 | +----------------------------------------------+
 | 
 | void   copi_field_TX_H_WM_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_TX_H_WM_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_range_TX_H_WM_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_TX_H_WM_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_TX_L_WM_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_TX_L_WM_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_range_TX_L_WM_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_TX_L_WM_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x0000401c RX_FIFO_TH |
 | +--------------------------------+
 | | bit  24:16 R/W  RX_H_WM        |
 | | bit  8:0   R/W  RX_L_WM        |
 | +--------------------------------+
 | 
 | void   copi_field_RX_H_WM_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_H_WM_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_RX_H_WM_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_RX_H_WM_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_RX_L_WM_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_L_WM_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_RX_L_WM_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_RX_L_WM_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00004020 TX_FLOW_CTRL_CFG |
 | +--------------------------------------+
 | | bit  17 R/W  TX_ODUK_CH_FC_EN        |
 | | bit  16 R/W  TX_ODUK_LINK_FC_EN      |
 | | bit  5  R/W  TX_CPB_CH_FC_EN         |
 | | bit  1  R/W  TX_OOB_EN               |
 | | bit  0  R/W  TX_IB_EN                |
 | +--------------------------------------+
 | 
 | void   copi_field_TX_ODUK_CH_FC_EN_set   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TX_ODUK_CH_FC_EN_get   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_TX_ODUK_LINK_FC_EN_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TX_ODUK_LINK_FC_EN_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_TX_CPB_CH_FC_EN_set    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TX_CPB_CH_FC_EN_get    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_TX_OOB_EN_set          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TX_OOB_EN_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_TX_IB_EN_set           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TX_IB_EN_get           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------+
 | | Register 0x00004024 RX_FLOW_CTRL_CFG |
 | +--------------------------------------+
 | | bit  21:20 R/W  RX_ODUK_ZONE_HI_TH   |
 | | bit  19:18 R/W  RX_ODUK_ZONE_LW_TH   |
 | | bit  16    R/W  RX_ODUK_CH_FC_EN     |
 | | bit  9:8   R/W  RX_CPB_ZONE_HI_TH    |
 | | bit  7:6   R/W  RX_CPB_ZONE_LW_TH    |
 | | bit  5     R/W  RX_LINK_FC_EN        |
 | | bit  4     R/W  RX_CPB_CH_FC_EN      |
 | | bit  1     R/W  RX_OOB_EN            |
 | | bit  0     R/W  RX_IB_EN             |
 | +--------------------------------------+
 | 
 | void   copi_field_RX_ODUK_ZONE_HI_TH_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_ODUK_ZONE_HI_TH_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_RX_ODUK_ZONE_HI_TH_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_RX_ODUK_ZONE_HI_TH_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_RX_ODUK_ZONE_LW_TH_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_ODUK_ZONE_LW_TH_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_RX_ODUK_ZONE_LW_TH_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_RX_ODUK_ZONE_LW_TH_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_RX_ODUK_CH_FC_EN_set         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_ODUK_CH_FC_EN_get         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_CPB_ZONE_HI_TH_set        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_CPB_ZONE_HI_TH_get        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_RX_CPB_ZONE_HI_TH_set  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_RX_CPB_ZONE_HI_TH_get  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_RX_CPB_ZONE_LW_TH_set        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_CPB_ZONE_LW_TH_get        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_RX_CPB_ZONE_LW_TH_set  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_RX_CPB_ZONE_LW_TH_get  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_RX_LINK_FC_EN_set            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_LINK_FC_EN_get            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_CPB_CH_FC_EN_set          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_CPB_CH_FC_EN_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_OOB_EN_set                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_OOB_EN_get                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_IB_EN_set                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_IB_EN_get                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------+
 | | Register 0x00004038 DPI_LB      |
 | +---------------------------------+
 | | bit  11 R/W  DPI_FC_EXT_ODUK_LB |
 | | bit  10 R/W  DPI_FC_EXT_CPB_LB  |
 | | bit  9  R/W  DPI_FC_INT_ODUK_LB |
 | | bit  8  R/W  DPI_FC_INT_CPB_LB  |
 | | bit  3  R/W  DPI_EXT_ODUK_LB    |
 | | bit  2  R/W  DPI_EXT_CPB_LB     |
 | | bit  1  R/W  DPI_INT_ODUK_LB    |
 | | bit  0  R/W  DPI_INT_CPB_LB     |
 | +---------------------------------+
 | 
 | void   copi_field_DPI_FC_EXT_ODUK_LB_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_DPI_FC_EXT_ODUK_LB_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_DPI_FC_EXT_CPB_LB_set  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_DPI_FC_EXT_CPB_LB_get  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_DPI_FC_INT_ODUK_LB_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_DPI_FC_INT_ODUK_LB_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_DPI_FC_INT_CPB_LB_set  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_DPI_FC_INT_CPB_LB_get  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_DPI_EXT_ODUK_LB_set    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_DPI_EXT_ODUK_LB_get    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_DPI_EXT_CPB_LB_set     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_DPI_EXT_CPB_LB_get     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_DPI_INT_ODUK_LB_set    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_DPI_INT_ODUK_LB_get    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_DPI_INT_CPB_LB_set     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_DPI_INT_CPB_LB_get     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------+
 | | Register 0x00004130 TOP_SUMMARY |
 | +---------------------------------+
 | | bit  10  R  TIP_INT0            |
 | | bit  7   R  RX_INTF_ERR_INT0    |
 | | bit  6:5 R  TX_INTF_ERR_INT0    |
 | | bit  4   R  SERDES_RX_LOS_INT0  |
 | | bit  1   R  RX_FIFO_INT0        |
 | | bit  0   R  TX_FIFO_INT0        |
 | +---------------------------------+
 | 
 | UINT32               copi_field_TIP_INT0_get                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_TIP_INT0_poll               ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               copi_field_RX_INTF_ERR_INT0_get        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_INTF_ERR_INT0_poll       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               copi_field_TX_INTF_ERR_INT0_get        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32               copi_field_range_TX_INTF_ERR_INT0_get  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE copi_field_range_TX_INTF_ERR_INT0_poll ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE copi_field_TX_INTF_ERR_INT0_poll       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               copi_field_SERDES_RX_LOS_INT0_get      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_SERDES_RX_LOS_INT0_poll     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               copi_field_RX_FIFO_INT0_get            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_FIFO_INT0_poll           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               copi_field_TX_FIFO_INT0_get            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_TX_FIFO_INT0_poll           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x00004134 TX_FIFO_INT_E0       |
 | +------------------------------------------+
 | | bit  29:28 R/W  ODUK_TX_BUF_ALIGN_INT_E0 |
 | | bit  23:20 R/W  ODUK_TX_BUF_UN_INT_E0    |
 | | bit  13:12 R/W  CPB_TX_BUF_ALIGN_INT_E0  |
 | | bit  7:4   R/W  CPB_TX_BUF_UN_INT_E0     |
 | +------------------------------------------+
 | 
 | void   copi_field_ODUK_TX_BUF_ALIGN_INT_E0_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_ODUK_TX_BUF_ALIGN_INT_E0_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_ODUK_TX_BUF_ALIGN_INT_E0_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_ODUK_TX_BUF_ALIGN_INT_E0_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_ODUK_TX_BUF_UN_INT_E0_set          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_ODUK_TX_BUF_UN_INT_E0_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_ODUK_TX_BUF_UN_INT_E0_set    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_ODUK_TX_BUF_UN_INT_E0_get    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_CPB_TX_BUF_ALIGN_INT_E0_set        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_CPB_TX_BUF_ALIGN_INT_E0_get        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_CPB_TX_BUF_ALIGN_INT_E0_set  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_CPB_TX_BUF_ALIGN_INT_E0_get  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_CPB_TX_BUF_UN_INT_E0_set           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_CPB_TX_BUF_UN_INT_E0_get           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_CPB_TX_BUF_UN_INT_E0_set     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_CPB_TX_BUF_UN_INT_E0_get     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0000413c TX_FIFO_INT0         |
 | +------------------------------------------+
 | | bit  29:28 R/W  ODUK_TX_BUF_ALIGN_INT_I0 |
 | | bit  23:20 R/W  ODUK_TX_BUF_UN_INT_I0    |
 | | bit  13:12 R/W  CPB_TX_BUF_ALIGN_INT_I0  |
 | | bit  7:4   R/W  CPB_TX_BUF_UN_INT_I0     |
 | +------------------------------------------+
 | 
 | void                 copi_field_ODUK_TX_BUF_ALIGN_INT_I0_set_to_clear       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_ODUK_TX_BUF_ALIGN_INT_I0_get                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void                 copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_set_to_clear ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE copi_field_range_ODUK_TX_BUF_ALIGN_INT_I0_poll         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE copi_field_ODUK_TX_BUF_ALIGN_INT_I0_poll               ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_ODUK_TX_BUF_UN_INT_I0_set_to_clear          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_ODUK_TX_BUF_UN_INT_I0_get                   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void                 copi_field_range_ODUK_TX_BUF_UN_INT_I0_set_to_clear    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               copi_field_range_ODUK_TX_BUF_UN_INT_I0_get             ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE copi_field_range_ODUK_TX_BUF_UN_INT_I0_poll            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE copi_field_ODUK_TX_BUF_UN_INT_I0_poll                  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_CPB_TX_BUF_ALIGN_INT_I0_set_to_clear        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_CPB_TX_BUF_ALIGN_INT_I0_get                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void                 copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_set_to_clear  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_get           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE copi_field_range_CPB_TX_BUF_ALIGN_INT_I0_poll          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE copi_field_CPB_TX_BUF_ALIGN_INT_I0_poll                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_CPB_TX_BUF_UN_INT_I0_set_to_clear           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_CPB_TX_BUF_UN_INT_I0_get                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void                 copi_field_range_CPB_TX_BUF_UN_INT_I0_set_to_clear     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               copi_field_range_CPB_TX_BUF_UN_INT_I0_get              ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE copi_field_range_CPB_TX_BUF_UN_INT_I0_poll             ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE copi_field_CPB_TX_BUF_UN_INT_I0_poll                   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00004140 RX_FIFO_INT_E0 |
 | +------------------------------------+
 | | bit  10:9 R/W  RX_BUF_ALIGN_INT_E0 |
 | | bit  5:3  R/W  RX_BUF_UN_INT_E0    |
 | +------------------------------------+
 | 
 | void   copi_field_RX_BUF_ALIGN_INT_E0_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_BUF_ALIGN_INT_E0_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_RX_BUF_ALIGN_INT_E0_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_RX_BUF_ALIGN_INT_E0_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_RX_BUF_UN_INT_E0_set          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_BUF_UN_INT_E0_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_RX_BUF_UN_INT_E0_set    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_RX_BUF_UN_INT_E0_get    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00004148 RX_FIFO_INT0   |
 | +------------------------------------+
 | | bit  10:9 R/W  RX_BUF_ALIGN_INT_I0 |
 | | bit  5:3  R/W  RX_BUF_UN_INT_I0    |
 | +------------------------------------+
 | 
 | void                 copi_field_RX_BUF_ALIGN_INT_I0_set_to_clear       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_BUF_ALIGN_INT_I0_get                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void                 copi_field_range_RX_BUF_ALIGN_INT_I0_set_to_clear ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               copi_field_range_RX_BUF_ALIGN_INT_I0_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE copi_field_range_RX_BUF_ALIGN_INT_I0_poll         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE copi_field_RX_BUF_ALIGN_INT_I0_poll               ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_RX_BUF_UN_INT_I0_set_to_clear          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_BUF_UN_INT_I0_get                   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void                 copi_field_range_RX_BUF_UN_INT_I0_set_to_clear    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               copi_field_range_RX_BUF_UN_INT_I0_get             ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE copi_field_range_RX_BUF_UN_INT_I0_poll            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE copi_field_RX_BUF_UN_INT_I0_poll                  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x00004164 SERDES_RX_LOS_INT_E0 |
 | +------------------------------------------+
 | | bit  23:0 R/W  SER_RX_LOS_INT_E0         |
 | +------------------------------------------+
 | 
 | void   copi_field_SER_RX_LOS_INT_E0_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_SER_RX_LOS_INT_E0_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_SER_RX_LOS_INT_E0_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_SER_RX_LOS_INT_E0_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0000416c SERDES_RX_LOS_INT0 |
 | +----------------------------------------+
 | | bit  23:0 R/W  SER_RX_LOS_INT_I0       |
 | +----------------------------------------+
 | 
 | void                 copi_field_SER_RX_LOS_INT_I0_set_to_clear       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_SER_RX_LOS_INT_I0_get                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void                 copi_field_range_SER_RX_LOS_INT_I0_set_to_clear ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               copi_field_range_SER_RX_LOS_INT_I0_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE copi_field_range_SER_RX_LOS_INT_I0_poll         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE copi_field_SER_RX_LOS_INT_I0_poll               ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------------------+
 | | Register (0x00004170 + (N) * 0x4) TX_INTF_ERR_INT_E0 |
 | |     N[0..1]                                          |
 | +------------------------------------------------------+
 | | bit  7 R/W  TX_EOP_ERR_INT_E0                        |
 | | bit  6 R/W  TX_SOP_ERR_INT_E0                        |
 | | bit  5 R/W  TX_EOB_ERR_INT_E0                        |
 | | bit  3 R/W  TX_PKT_CH_CHANGE_ERR_INT_E0              |
 | | bit  2 R/W  TX_BURST_DROP_ERR_INT_E0                 |
 | | bit  1 R/W  TX_BURST_LEN_ERR_INT_E0                  |
 | | bit  0 R/W  TX_SOB_ERR_INT_E0                        |
 | +------------------------------------------------------+
 | 
 | void   copi_field_TX_EOP_ERR_INT_E0_set           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_TX_EOP_ERR_INT_E0_get           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_TX_SOP_ERR_INT_E0_set           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_TX_SOP_ERR_INT_E0_get           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_TX_EOB_ERR_INT_E0_set           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_TX_EOB_ERR_INT_E0_get           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_TX_PKT_CH_CHANGE_ERR_INT_E0_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_TX_PKT_CH_CHANGE_ERR_INT_E0_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_TX_BURST_DROP_ERR_INT_E0_set    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_TX_BURST_DROP_ERR_INT_E0_get    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_TX_BURST_LEN_ERR_INT_E0_set     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_TX_BURST_LEN_ERR_INT_E0_get     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_TX_SOB_ERR_INT_E0_set           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_TX_SOB_ERR_INT_E0_get           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +----------------------------------------------------+
 | | Register (0x00004180 + (N) * 0x4) TX_INTF_ERR_INT0 |
 | |     N[0..1]                                        |
 | +----------------------------------------------------+
 | | bit  7 R/W  TX_EOP_ERR_INT_I0                      |
 | | bit  6 R/W  TX_SOP_ERR_INT_I0                      |
 | | bit  5 R/W  TX_EOB_ERR_INT_I0                      |
 | | bit  3 R/W  TX_PKT_CH_CHANGE_ERR_INT_I0            |
 | | bit  2 R/W  TX_BURST_DROP_ERR_INT_I0               |
 | | bit  1 R/W  TX_BURST_LEN_ERR_INT_I0                |
 | | bit  0 R/W  TX_SOB_ERR_INT_I0                      |
 | +----------------------------------------------------+
 | 
 | void                 copi_field_TX_EOP_ERR_INT_I0_set_to_clear           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               copi_field_TX_EOP_ERR_INT_I0_get                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE copi_field_TX_EOP_ERR_INT_I0_poll                   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_TX_SOP_ERR_INT_I0_set_to_clear           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               copi_field_TX_SOP_ERR_INT_I0_get                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE copi_field_TX_SOP_ERR_INT_I0_poll                   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_TX_EOB_ERR_INT_I0_set_to_clear           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               copi_field_TX_EOB_ERR_INT_I0_get                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE copi_field_TX_EOB_ERR_INT_I0_poll                   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_set_to_clear ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE copi_field_TX_PKT_CH_CHANGE_ERR_INT_I0_poll         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_TX_BURST_DROP_ERR_INT_I0_set_to_clear    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               copi_field_TX_BURST_DROP_ERR_INT_I0_get             ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE copi_field_TX_BURST_DROP_ERR_INT_I0_poll            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_TX_BURST_LEN_ERR_INT_I0_set_to_clear     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               copi_field_TX_BURST_LEN_ERR_INT_I0_get              ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE copi_field_TX_BURST_LEN_ERR_INT_I0_poll             ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_TX_SOB_ERR_INT_I0_set_to_clear           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               copi_field_TX_SOB_ERR_INT_I0_get                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE copi_field_TX_SOB_ERR_INT_I0_poll                   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------------+
 | | Register 0x00004188 RX_INTF_ERR_INT_E0            |
 | +---------------------------------------------------+
 | | bit  19    R/W  RX_ODUK_H4B_CMPR_ERR_INT_E0       |
 | | bit  18    R/W  RX_ODUK_H2B_CMPR_ERR_INT_E0       |
 | | bit  17    R/W  RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0 |
 | | bit  16    R/W  RX_ODUK_H2B_CAM_ERR_INT_E0        |
 | | bit  11:10 R/W  RX_IL_DAT_ERR_INT_E0              |
 | | bit  9     R/W  RX_IL_ALIGN_INT_E0                |
 | | bit  8     R/W  RX_IL_ERR_ALL_INT_E0              |
 | | bit  7     R/W  RX_EOP_ERR_INT_E0                 |
 | | bit  6     R/W  RX_SOP_ERR_INT_E0                 |
 | | bit  5     R/W  RX_EOB_ERR_INT_E0                 |
 | | bit  3     R/W  RX_PKT_CH_CHANGE_ERR_INT_E0       |
 | | bit  2     R/W  RX_BURST_DROP_ERR_INT_E0          |
 | | bit  1     R/W  RX_BURST_LEN_ERR_INT_E0           |
 | | bit  0     R/W  RX_SOB_ERR_INT_E0                 |
 | +---------------------------------------------------+
 | 
 | void   copi_field_RX_ODUK_H4B_CMPR_ERR_INT_E0_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_ODUK_H4B_CMPR_ERR_INT_E0_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_ODUK_H2B_CMPR_ERR_INT_E0_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_ODUK_H2B_CMPR_ERR_INT_E0_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_E0_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_ODUK_H2B_CAM_ERR_INT_E0_set        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_ODUK_H2B_CAM_ERR_INT_E0_get        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_IL_DAT_ERR_INT_E0_set              ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_IL_DAT_ERR_INT_E0_get              ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_range_RX_IL_DAT_ERR_INT_E0_set        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_RX_IL_DAT_ERR_INT_E0_get        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   copi_field_RX_IL_ALIGN_INT_E0_set                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_IL_ALIGN_INT_E0_get                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_IL_ERR_ALL_INT_E0_set              ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_IL_ERR_ALL_INT_E0_get              ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_EOP_ERR_INT_E0_set                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_EOP_ERR_INT_E0_get                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_SOP_ERR_INT_E0_set                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_SOP_ERR_INT_E0_get                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_EOB_ERR_INT_E0_set                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_EOB_ERR_INT_E0_get                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_PKT_CH_CHANGE_ERR_INT_E0_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_PKT_CH_CHANGE_ERR_INT_E0_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_BURST_DROP_ERR_INT_E0_set          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_BURST_DROP_ERR_INT_E0_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_BURST_LEN_ERR_INT_E0_set           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_BURST_LEN_ERR_INT_E0_get           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   copi_field_RX_SOB_ERR_INT_E0_set                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_SOB_ERR_INT_E0_get                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------------------+
 | | Register 0x00004190 RX_INTF_ERR_INT0              |
 | +---------------------------------------------------+
 | | bit  19    R/W  RX_ODUK_H4B_CMPR_ERR_INT_I0       |
 | | bit  18    R/W  RX_ODUK_H2B_CMPR_ERR_INT_I0       |
 | | bit  17    R/W  RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0 |
 | | bit  16    R/W  RX_ODUK_H2B_CAM_ERR_INT_I0        |
 | | bit  11:10 R/W  RX_IL_DAT_ERR_INT_I0              |
 | | bit  9     R/W  RX_IL_ALIGN_INT_I0                |
 | | bit  8     R/W  RX_IL_ERR_ALL_INT_I0              |
 | | bit  7     R/W  RX_EOP_ERR_INT_I0                 |
 | | bit  6     R/W  RX_SOP_ERR_INT_I0                 |
 | | bit  5     R/W  RX_EOB_ERR_INT_I0                 |
 | | bit  3     R/W  RX_PKT_CH_CHANGE_ERR_INT_I0       |
 | | bit  2     R/W  RX_BURST_DROP_ERR_INT_I0          |
 | | bit  1     R/W  RX_BURST_LEN_ERR_INT_I0           |
 | | bit  0     R/W  RX_SOB_ERR_INT_I0                 |
 | +---------------------------------------------------+
 | 
 | void                 copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_set_to_clear       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_get                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_ODUK_H4B_CMPR_ERR_INT_I0_poll               ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_set_to_clear       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_get                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_ODUK_H2B_CMPR_ERR_INT_I0_poll               ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_set_to_clear ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_ODUK_H2B_CAM_MMATCH_ERR_INT_I0_poll         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_set_to_clear        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_get                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_ODUK_H2B_CAM_ERR_INT_I0_poll                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_RX_IL_DAT_ERR_INT_I0_set_to_clear              ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_IL_DAT_ERR_INT_I0_get                       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void                 copi_field_range_RX_IL_DAT_ERR_INT_I0_set_to_clear        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               copi_field_range_RX_IL_DAT_ERR_INT_I0_get                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE copi_field_range_RX_IL_DAT_ERR_INT_I0_poll                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE copi_field_RX_IL_DAT_ERR_INT_I0_poll                      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_RX_IL_ALIGN_INT_I0_set_to_clear                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_IL_ALIGN_INT_I0_get                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_IL_ALIGN_INT_I0_poll                        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_RX_IL_ERR_ALL_INT_I0_set_to_clear              ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_IL_ERR_ALL_INT_I0_get                       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_IL_ERR_ALL_INT_I0_poll                      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_RX_EOP_ERR_INT_I0_set_to_clear                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_EOP_ERR_INT_I0_get                          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_EOP_ERR_INT_I0_poll                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_RX_SOP_ERR_INT_I0_set_to_clear                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_SOP_ERR_INT_I0_get                          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_SOP_ERR_INT_I0_poll                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_RX_EOB_ERR_INT_I0_set_to_clear                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_EOB_ERR_INT_I0_get                          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_EOB_ERR_INT_I0_poll                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_set_to_clear       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_get                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_PKT_CH_CHANGE_ERR_INT_I0_poll               ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_RX_BURST_DROP_ERR_INT_I0_set_to_clear          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_BURST_DROP_ERR_INT_I0_get                   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_BURST_DROP_ERR_INT_I0_poll                  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_RX_BURST_LEN_ERR_INT_I0_set_to_clear           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_BURST_LEN_ERR_INT_I0_get                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_BURST_LEN_ERR_INT_I0_poll                   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 copi_field_RX_SOB_ERR_INT_I0_set_to_clear                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_RX_SOB_ERR_INT_I0_get                          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_RX_SOB_ERR_INT_I0_poll                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------+
 | | Register 0x000041ac COM_INT_E0 |
 | +--------------------------------+
 | | bit  0 R/W  TIP_INT_E0         |
 | +--------------------------------+
 | 
 | void   copi_field_TIP_INT_E0_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_TIP_INT_E0_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------+
 | | Register 0x000041b4 TIP_INT0 |
 | +------------------------------+
 | | bit  0 R/W  TIP_INT_I0       |
 | +------------------------------+
 | 
 | void                 copi_field_TIP_INT_I0_set_to_clear ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               copi_field_TIP_INT_I0_get          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_field_TIP_INT_I0_poll         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------+
 | | Register (0x000041b8 + (N) * 0x4) TX_ERR_CNT |
 | |     N[0..1]                                  |
 | +----------------------------------------------+
 | | bit  31:0 R  TX_ERR_CNT                      |
 | +----------------------------------------------+
 | 
 | UINT32 copi_field_TX_ERR_CNT_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 copi_field_range_TX_ERR_CNT_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x000041c0 RX_ERR_CNT |
 | +--------------------------------+
 | | bit  31:0 R  RX_ERR_CNT        |
 | +--------------------------------+
 | 
 | UINT32 copi_field_RX_ERR_CNT_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 copi_field_range_RX_ERR_CNT_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x000041c4 + (N) * 0x4) TX_DIAG_IN_CNT |
 | |     N[0..1]                                      |
 | +--------------------------------------------------+
 | | bit  31:0 R  TX_DIAG_CNT                         |
 | +--------------------------------------------------+
 | 
 | UINT32 copi_field_TX_DIAG_CNT_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 copi_field_range_TX_DIAG_CNT_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x000041cc + (N) * 0x4) RX_DIAG_CNT |
 | |     N[0..1]                                   |
 | +-----------------------------------------------+
 | | bit  31:0 R  RX_DIAG_IN_CNT                   |
 | +-----------------------------------------------+
 | 
 | UINT32 copi_field_RX_DIAG_IN_CNT_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 copi_field_range_RX_DIAG_IN_CNT_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00004200 + (N) * 0x4) TX_CPB_ARB_CH_LK |
 | |     N[0..149]                                      |
 | +----------------------------------------------------+
 | | bit  7:0 R/W  TX_CPB_ARB_CH                        |
 | +----------------------------------------------------+
 | 
 | void   copi_field_TX_CPB_ARB_CH_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_TX_CPB_ARB_CH_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_range_TX_CPB_ARB_CH_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_TX_CPB_ARB_CH_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00004500 + (N) * 0x4) TX_ODUK_ARB_CH_LK |
 | |     N[0..103]                                       |
 | +-----------------------------------------------------+
 | | bit  7:0 R/W  TX_ODUK_ARB_CH                        |
 | +-----------------------------------------------------+
 | 
 | void   copi_field_TX_ODUK_ARB_CH_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_TX_ODUK_ARB_CH_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_range_TX_ODUK_ARB_CH_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_TX_ODUK_ARB_CH_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00004700 + (N) * 0x4) TX_ODUK_4B_LK |
 | |     N[0..103]                                   |
 | +-------------------------------------------------+
 | | bit  31:0 R/W  TX_ODUK_4B_LK                    |
 | +-------------------------------------------------+
 | 
 | void   copi_field_TX_ODUK_4B_LK_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_TX_ODUK_4B_LK_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_range_TX_ODUK_4B_LK_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_TX_ODUK_4B_LK_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00004900 + (N) * 0x4) TX_ODUK_2B_LK |
 | |     N[0..103]                                   |
 | +-------------------------------------------------+
 | | bit  15:0 R/W  TX_ODUK_2B_LK                    |
 | +-------------------------------------------------+
 | 
 | void   copi_field_TX_ODUK_2B_LK_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_TX_ODUK_2B_LK_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_range_TX_ODUK_2B_LK_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_TX_ODUK_2B_LK_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00004b00 + (N) * 0x4) RX_ARB_CH_LK |
 | |     N[0..253]                                  |
 | +------------------------------------------------+
 | | bit  8   R/W  RX_CPB_ODUK_PATH_SEL             |
 | | bit  7:0 R/W  RX_ARB_CH                        |
 | +------------------------------------------------+
 | 
 | void   copi_field_RX_CPB_ODUK_PATH_SEL_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_RX_CPB_ODUK_PATH_SEL_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_RX_ARB_CH_set            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_RX_ARB_CH_get            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_range_RX_ARB_CH_set      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_RX_ARB_CH_get      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00004f00 + (N) * 0x4) RX_ODUK_4B_LK |
 | |     N[0..103]                                   |
 | +-------------------------------------------------+
 | | bit  31:0 R/W  RX_ODUK_4B_LK                    |
 | +-------------------------------------------------+
 | 
 | void   copi_field_RX_ODUK_4B_LK_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_RX_ODUK_4B_LK_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_range_RX_ODUK_4B_LK_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_RX_ODUK_4B_LK_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00005100 + (N) * 0x4) RX_ODUK_2B_LK |
 | |     N[0..103]                                   |
 | +-------------------------------------------------+
 | | bit  15:0 R/W  RX_ODUK_2B_LK                    |
 | +-------------------------------------------------+
 | 
 | void   copi_field_RX_ODUK_2B_LK_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 copi_field_RX_ODUK_2B_LK_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   copi_field_range_RX_ODUK_2B_LK_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 copi_field_range_RX_ODUK_2B_LK_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x000052a0 RX_ODUK_H2B_CAM_ERR |
 | +-----------------------------------------+
 | | bit  31   R/W  RX_ODUK_H2B_CAM_ERR_SET  |
 | | bit  23:0 R    RX_ODUK_H2B_CAM_ERR      |
 | +-----------------------------------------+
 | 
 | void   copi_field_RX_ODUK_H2B_CAM_ERR_SET_set   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_ODUK_H2B_CAM_ERR_SET_get   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 copi_field_RX_ODUK_H2B_CAM_ERR_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 copi_field_range_RX_ODUK_H2B_CAM_ERR_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x000052a4 RX_ODUK_H2B_CMPR_ERR |
 | +------------------------------------------+
 | | bit  31   R/W  RX_ODUK_H2B_CMPR_ERR_SET  |
 | | bit  23:0 R    RX_ODUK_H2B_CMPR_ERR      |
 | +------------------------------------------+
 | 
 | void   copi_field_RX_ODUK_H2B_CMPR_ERR_SET_set   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_ODUK_H2B_CMPR_ERR_SET_get   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 copi_field_RX_ODUK_H2B_CMPR_ERR_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 copi_field_range_RX_ODUK_H2B_CMPR_ERR_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x000052a8 RX_ODUK_H4B_CMPR_ERR0 |
 | +-------------------------------------------+
 | | bit  31    R/W  RX_ODUK_H4B_CMPR_ERR_SET  |
 | | bit  23:16 R    RX_ODUK_H4B_CMPR_ERR0     |
 | +-------------------------------------------+
 | 
 | void   copi_field_RX_ODUK_H4B_CMPR_ERR_SET_set    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 copi_field_RX_ODUK_H4B_CMPR_ERR_SET_get    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 copi_field_RX_ODUK_H4B_CMPR_ERR0_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 copi_field_range_RX_ODUK_H4B_CMPR_ERR0_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x000052ac RX_ODUK_H4B_CMPR_ERR1 |
 | +-------------------------------------------+
 | | bit  31:0 R  RX_ODUK_H4B_CMPR_ERR1        |
 | +-------------------------------------------+
 | 
 | UINT32 copi_field_RX_ODUK_H4B_CMPR_ERR1_get       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 copi_field_range_RX_ODUK_H4B_CMPR_ERR1_get ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _COPI_IO_H
#define _COPI_IO_H

#include "sifd_api.h"
#include "copi_regs.h"
#include "copi_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 copi_buffer_init               ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void                 copi_buffer_flush              ( copi_buffer_t *b_ptr )
 | UINT32               copi_reg_read                  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 copi_reg_write                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 copi_field_set                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 copi_action_on_write_field_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 copi_burst_read                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 copi_burst_write               ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE copi_poll                      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 copi_reg_SOFT_RESET_write                                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_SOFT_RESET_field_set                             ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_SOFT_RESET_read                                  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_CFG_write                                        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_CFG_field_set                                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_CFG_read                                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_TX_INTF_CFG_write                                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_TX_INTF_CFG_field_set                            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_TX_INTF_CFG_read                                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_RX_INTF_CFG_write                                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_RX_INTF_CFG_field_set                            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_RX_INTF_CFG_read                                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_RX_FIFO_TH_write                                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_RX_FIFO_TH_field_set                             ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_RX_FIFO_TH_read                                  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_TX_FLOW_CTRL_CFG_write                           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_TX_FLOW_CTRL_CFG_field_set                       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_TX_FLOW_CTRL_CFG_read                            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_RX_FLOW_CTRL_CFG_write                           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_RX_FLOW_CTRL_CFG_field_set                       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_RX_FLOW_CTRL_CFG_read                            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_DPI_LB_write                                     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_DPI_LB_field_set                                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_DPI_LB_read                                      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_RX_ODUK_2B_LK_array_write                        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 copi_reg_RX_ODUK_2B_LK_array_field_set                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_RX_ODUK_2B_LK_array_read                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 copi_reg_TX_FIFO_TH_array_write                           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 copi_reg_TX_FIFO_TH_array_field_set                       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_TX_FIFO_TH_array_read                            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 copi_reg_RX_ODUK_H2B_CAM_ERR_write                        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_RX_ODUK_H2B_CAM_ERR_field_set                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_RX_ODUK_H2B_CAM_ERR_read                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_RX_ODUK_H2B_CMPR_ERR_write                       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_RX_ODUK_H2B_CMPR_ERR_field_set                   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_RX_ODUK_H2B_CMPR_ERR_read                        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_RX_ODUK_H4B_CMPR_ERR0_write                      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_RX_ODUK_H4B_CMPR_ERR0_field_set                  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_RX_ODUK_H4B_CMPR_ERR0_read                       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_TX_FIFO_INT_E0_write                             ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_TX_FIFO_INT_E0_field_set                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_TX_FIFO_INT_E0_read                              ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_RX_FIFO_INT_E0_write                             ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_RX_FIFO_INT_E0_field_set                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_RX_FIFO_INT_E0_read                              ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_SERDES_RX_LOS_INT_E0_write                       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_SERDES_RX_LOS_INT_E0_field_set                   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_SERDES_RX_LOS_INT_E0_read                        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_RX_INTF_ERR_INT_E0_write                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_RX_INTF_ERR_INT_E0_field_set                     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_RX_INTF_ERR_INT_E0_read                          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_COM_INT_E0_write                                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_COM_INT_E0_field_set                             ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_COM_INT_E0_read                                  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void                 copi_reg_TX_INTF_ERR_INT_E0_array_write                   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 copi_reg_TX_INTF_ERR_INT_E0_array_field_set               ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_TX_INTF_ERR_INT_E0_array_read                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32               copi_reg_TOP_SUMMARY_read                                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_reg_TOP_SUMMARY_poll                                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 copi_reg_TX_FIFO_INT0_write                               ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_TX_FIFO_INT0_action_on_write_field_set           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_TX_FIFO_INT0_read                                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_reg_TX_FIFO_INT0_poll                                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 copi_reg_RX_FIFO_INT0_write                               ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_RX_FIFO_INT0_action_on_write_field_set           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_RX_FIFO_INT0_read                                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_reg_RX_FIFO_INT0_poll                                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 copi_reg_SERDES_RX_LOS_INT0_write                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_SERDES_RX_LOS_INT0_action_on_write_field_set     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_SERDES_RX_LOS_INT0_read                          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_reg_SERDES_RX_LOS_INT0_poll                          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 copi_reg_RX_INTF_ERR_INT0_write                           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_RX_INTF_ERR_INT0_action_on_write_field_set       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_RX_INTF_ERR_INT0_read                            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_reg_RX_INTF_ERR_INT0_poll                            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 copi_reg_TIP_INT0_write                                   ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 copi_reg_TIP_INT0_action_on_write_field_set               ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_TIP_INT0_read                                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE copi_reg_TIP_INT0_poll                                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 copi_reg_TX_INTF_ERR_INT0_array_write                     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 copi_reg_TX_INTF_ERR_INT0_array_action_on_write_field_set ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_TX_INTF_ERR_INT0_array_read                      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE copi_reg_TX_INTF_ERR_INT0_array_poll                      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 copi_reg_TX_ODUK_ARB_CH_LK_array_write                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 copi_reg_TX_ODUK_ARB_CH_LK_array_field_set                ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_TX_ODUK_ARB_CH_LK_array_read                     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 copi_reg_TX_ODUK_4B_LK_array_write                        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 copi_reg_TX_ODUK_4B_LK_array_field_set                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_TX_ODUK_4B_LK_array_read                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 copi_reg_TX_ODUK_2B_LK_array_write                        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 copi_reg_TX_ODUK_2B_LK_array_field_set                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_TX_ODUK_2B_LK_array_read                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 copi_reg_RX_ODUK_4B_LK_array_write                        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 copi_reg_RX_ODUK_4B_LK_array_field_set                    ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_RX_ODUK_4B_LK_array_read                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 copi_reg_TX_CPB_ARB_CH_LK_array_write                     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 copi_reg_TX_CPB_ARB_CH_LK_array_field_set                 ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_TX_CPB_ARB_CH_LK_array_read                      ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 copi_reg_RX_ARB_CH_LK_array_write                         ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 copi_reg_RX_ARB_CH_LK_array_field_set                     ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               copi_reg_RX_ARB_CH_LK_array_read                          ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32               copi_reg_RX_ERR_CNT_read                                  ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               copi_reg_RX_ODUK_H4B_CMPR_ERR1_read                       ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               copi_reg_TX_ERR_CNT_array_read                            ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32               copi_reg_TX_DIAG_IN_CNT_array_read                        ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32               copi_reg_RX_DIAG_CNT_array_read                           ( copi_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _COPI_IO_H */
