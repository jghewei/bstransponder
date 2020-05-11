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
 *     and register accessor functions for the fc_cbr block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing fc_cbr io functions is:
 | {
 |     fc_cbr_buffer_t b_ptr[1];
 |     fc_cbr_buffer_init( b_ptr, h_ptr, A );
 |     value = fc_cbr_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     fc_cbr_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     fc_cbr_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = fc_cbr_field_<FIELD>_get( NULL, h_ptr, A );
 | fc_cbr_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------+
 | | Register 0x00000000 CFG    |
 | +----------------------------+
 | | bit  31  R/W  RX_FRM_SCRAM |
 | | bit  5:3 R/W  BUS_WIDTH    |
 | | bit  2:1 R/W  MODE         |
 | | bit  0   R/W  SOFT_RST     |
 | +----------------------------+
 | 
 | void   fc_cbr_field_RX_FRM_SCRAM_set    ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_FRM_SCRAM_get    ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_BUS_WIDTH_set       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_BUS_WIDTH_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_range_BUS_WIDTH_set ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_field_range_BUS_WIDTH_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc_cbr_field_MODE_set            ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_MODE_get            ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_range_MODE_set      ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_field_range_MODE_get      ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc_cbr_field_SOFT_RST_set        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_SOFT_RST_get        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x00000014 RX_CFG        |
 | +-----------------------------------+
 | | bit  8 R/W  RX_FC_CONT_ALIGN_MODE |
 | | bit  2 R/W  RX_ENABLE_BYTE_ALIGN  |
 | | bit  1 R/W  RX_FORCE_REALIGN      |
 | | bit  0 R/W  RX_SOFT_RST           |
 | +-----------------------------------+
 | 
 | void   fc_cbr_field_RX_FC_CONT_ALIGN_MODE_set ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_FC_CONT_ALIGN_MODE_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_RX_ENABLE_BYTE_ALIGN_set  ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_ENABLE_BYTE_ALIGN_get  ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_RX_SOFT_RST_set           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_SOFT_RST_get           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_RX_FORCE_REALIGN_set      ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_FORCE_REALIGN_get      ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------+
 | | Register 0x0000001c RX_DLOLB_CLK_CFG |
 | +--------------------------------------+
 | | bit  31:0 R/W  RX_DLOLB_CLK_DIV      |
 | +--------------------------------------+
 | 
 | void   fc_cbr_field_RX_DLOLB_CLK_DIV_set       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_DLOLB_CLK_DIV_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_range_RX_DLOLB_CLK_DIV_set ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_field_range_RX_DLOLB_CLK_DIV_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000020 RX_DLOLB_EN     |
 | +-------------------------------------+
 | | bit  7   R/W  RX_LCV_DLOLB_EN       |
 | | bit  6   R/W  RX_XDET_DLOLB_EN      |
 | | bit  5   R/W  ASD_DLOLB_EN          |
 | | bit  4   R/W  RX_LOSS_SYNC_DLOLB_EN |
 | | bit  3   R/W  RX_DRL_DLOLB_EN       |
 | | bit  2:0 R/W  RX_DLOLB_INT_PER      |
 | +-------------------------------------+
 | 
 | void   fc_cbr_field_RX_LCV_DLOLB_EN_set        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_LCV_DLOLB_EN_get        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_RX_XDET_DLOLB_EN_set       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_XDET_DLOLB_EN_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_ASD_DLOLB_EN_set           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_ASD_DLOLB_EN_get           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_RX_LOSS_SYNC_DLOLB_EN_set  ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_LOSS_SYNC_DLOLB_EN_get  ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_RX_DRL_DLOLB_EN_set        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_DRL_DLOLB_EN_get        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_RX_DLOLB_INT_PER_set       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_DLOLB_INT_PER_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_range_RX_DLOLB_INT_PER_set ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_field_range_RX_DLOLB_INT_PER_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000024 RX_DEFECT_CFG      |
 | +----------------------------------------+
 | | bit  31:16 R/W  RX_LCV_DLOLB_THRSH     |
 | | bit  15:8  R/W  RX_XDET_INTERVAL       |
 | | bit  7:0   R/W  RX_XDET_DEASSERT_THRSH |
 | +----------------------------------------+
 | 
 | void   fc_cbr_field_RX_LCV_DLOLB_THRSH_set           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_LCV_DLOLB_THRSH_get           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_range_RX_LCV_DLOLB_THRSH_set     ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_field_range_RX_LCV_DLOLB_THRSH_get     ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc_cbr_field_RX_XDET_INTERVAL_set             ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_XDET_INTERVAL_get             ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_range_RX_XDET_INTERVAL_set       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_field_range_RX_XDET_INTERVAL_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc_cbr_field_RX_XDET_DEASSERT_THRSH_set       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_XDET_DEASSERT_THRSH_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_range_RX_XDET_DEASSERT_THRSH_set ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_field_range_RX_XDET_DEASSERT_THRSH_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00000028 RX_GF_WRD_CNTR |
 | +------------------------------------+
 | | bit  31:0 R  RX_GF_DWRD            |
 | +------------------------------------+
 | 
 | UINT32 fc_cbr_field_RX_GF_DWRD_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 fc_cbr_field_range_RX_GF_DWRD_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x0000002c RX_BF_WRD_CNTR |
 | +------------------------------------+
 | | bit  31:0 R  RX_BF_DWRD            |
 | +------------------------------------+
 | 
 | UINT32 fc_cbr_field_RX_BF_DWRD_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 fc_cbr_field_range_RX_BF_DWRD_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000030 RX_WORD_CNT |
 | +---------------------------------+
 | | bit  31:0 R  RX_WRD_CNT         |
 | +---------------------------------+
 | 
 | UINT32 fc_cbr_field_RX_WRD_CNT_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 fc_cbr_field_range_RX_WRD_CNT_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000034 RX_FRAMES_BAD_CRC |
 | +---------------------------------------+
 | | bit  31:0 R  RX_BAD_CRC               |
 | +---------------------------------------+
 | 
 | UINT32 fc_cbr_field_RX_BAD_CRC_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 fc_cbr_field_range_RX_BAD_CRC_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00000038 RX_TOTAL_PACKETS |
 | +--------------------------------------+
 | | bit  31:0 R  RX_TOT_PKTS             |
 | +--------------------------------------+
 | 
 | UINT32 fc_cbr_field_RX_TOT_PKTS_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 fc_cbr_field_range_RX_TOT_PKTS_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000040 TX_USER_PATTERN_LSB |
 | +-----------------------------------------+
 | | bit  31:0 R/W  TX_USR_PATT              |
 | +-----------------------------------------+
 | 
 | void   fc_cbr_lfield_TX_USR_PATT_set       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | void   fc_cbr_lfield_TX_USR_PATT_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | void   fc_cbr_lfield_range_TX_USR_PATT_set ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_lfield_range_TX_USR_PATT_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00000044 TX_CFG         |
 | +------------------------------------+
 | | bit  31:24 R/W  TX_USR_PATT        |
 | | bit  19:16 R/W  TX_USR_PATT_DISP   |
 | | bit  15    R/W  GE_RD_INV_EN       |
 | | bit  14    R/W  FC_RD_INV_EN       |
 | | bit  13    R/W  TX_RD_INV_EN       |
 | | bit  9     R/W  TX_USR_PATT_UPDATE |
 | | bit  7     R/W  TX_LCV_ERR_INS_EN  |
 | | bit  6     R/W  TX_PN11_INV        |
 | | bit  5     R/W  TX_NOS_PN11_SEL    |
 | | bit  4     R/W  TX_FORCE_NOS_PN11  |
 | | bit  0     R/W  TX_SOFT_RST        |
 | +------------------------------------+
 | 
 | void   fc_cbr_field_TX_USR_PATT_DISP_set       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_TX_USR_PATT_DISP_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_range_TX_USR_PATT_DISP_set ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_cbr_field_range_TX_USR_PATT_DISP_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc_cbr_field_GE_RD_INV_EN_set           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_GE_RD_INV_EN_get           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_FC_RD_INV_EN_set           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_FC_RD_INV_EN_get           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_TX_RD_INV_EN_set           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_TX_RD_INV_EN_get           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_TX_LCV_ERR_INS_EN_set      ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_TX_LCV_ERR_INS_EN_get      ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_TX_PN11_INV_set            ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_TX_PN11_INV_get            ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_TX_NOS_PN11_SEL_set        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_TX_NOS_PN11_SEL_get        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_TX_FORCE_NOS_PN11_set      ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_TX_FORCE_NOS_PN11_get      ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_TX_SOFT_RST_set            ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_TX_SOFT_RST_get            ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_TX_USR_PATT_UPDATE_set     ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_TX_USR_PATT_UPDATE_get     ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------+
 | | Register 0x00000090 LCV_CNT |
 | +-----------------------------+
 | | bit  15:0 R  RX_LCV_CNT     |
 | +-----------------------------+
 | 
 | UINT32 fc_cbr_field_RX_LCV_CNT_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 fc_cbr_field_range_RX_LCV_CNT_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x00000094 CNT_UPDATE |
 | +--------------------------------+
 | | bit  0 R/W  CNTR_UPDATE        |
 | +--------------------------------+
 | 
 | void   fc_cbr_field_CNTR_UPDATE_set ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_CNTR_UPDATE_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------+
 | | Register 0x000000b0 INT_EN   |
 | +------------------------------+
 | | bit  6 R/W  RX_DISP_INVERT_E |
 | | bit  5 R/W  RX_XDET_E        |
 | | bit  4 R/W  RX_SOF_DET_E     |
 | | bit  3 R/W  RX_DLOLB_E       |
 | | bit  2 R/W  RX_ASD_E         |
 | | bit  1 R/W  RX_SYNC_E        |
 | | bit  0 R/W  RX_LCV_E         |
 | +------------------------------+
 | 
 | void   fc_cbr_field_RX_DISP_INVERT_E_set ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_DISP_INVERT_E_get ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_RX_XDET_E_set        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_XDET_E_get        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_RX_SOF_DET_E_set     ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_SOF_DET_E_get     ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_RX_DLOLB_E_set       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_DLOLB_E_get       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_RX_ASD_E_set         ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_ASD_E_get         ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_RX_SYNC_E_set        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_SYNC_E_get        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void   fc_cbr_field_RX_LCV_E_set         ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_cbr_field_RX_LCV_E_get         ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------+
 | | Register 0x000000b4 INT      |
 | +------------------------------+
 | | bit  6 R/W  RX_DISP_INVERT_I |
 | | bit  5 R/W  RX_XDET_I        |
 | | bit  4 R/W  RX_SOF_DET_I     |
 | | bit  3 R/W  RX_DLOLB_I       |
 | | bit  2 R/W  RX_ASD_I         |
 | | bit  1 R/W  RX_SYNC_I        |
 | | bit  0 R/W  RX_LCV_I         |
 | +------------------------------+
 | 
 | void                 fc_cbr_field_RX_DISP_INVERT_I_set_to_clear ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_field_RX_DISP_INVERT_I_get          ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_field_RX_DISP_INVERT_I_poll         ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_cbr_field_RX_XDET_I_set_to_clear        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_field_RX_XDET_I_get                 ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_field_RX_XDET_I_poll                ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_cbr_field_RX_DLOLB_I_set_to_clear       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_field_RX_DLOLB_I_get                ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_field_RX_DLOLB_I_poll               ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_cbr_field_RX_ASD_I_set_to_clear         ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_field_RX_ASD_I_get                  ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_field_RX_ASD_I_poll                 ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_cbr_field_RX_SYNC_I_set_to_clear        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_field_RX_SYNC_I_get                 ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_field_RX_SYNC_I_poll                ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_cbr_field_RX_SOF_DET_I_set_to_clear     ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_field_RX_SOF_DET_I_get              ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_field_RX_SOF_DET_I_poll             ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_cbr_field_RX_LCV_I_set_to_clear         ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_cbr_field_RX_LCV_I_get                  ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_field_RX_LCV_I_poll                 ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------+
 | | Register 0x000000b8 INT_VAL |
 | +-----------------------------+
 | | bit  6 R  RX_DISP_INVERT_V  |
 | | bit  5 R  RX_XDET_V         |
 | | bit  3 R  RX_DLOLB_V        |
 | | bit  2 R  RX_ASD_V          |
 | | bit  1 R  RX_SYNC_V         |
 | | bit  0 R  RX_LCV_V          |
 | +-----------------------------+
 | 
 | UINT32               fc_cbr_field_RX_DISP_INVERT_V_get  ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_field_RX_DISP_INVERT_V_poll ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc_cbr_field_RX_XDET_V_get         ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_field_RX_XDET_V_poll        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc_cbr_field_RX_DLOLB_V_get        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_field_RX_DLOLB_V_poll       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc_cbr_field_RX_ASD_V_get          ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_field_RX_ASD_V_poll         ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc_cbr_field_RX_SYNC_V_get         ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_field_RX_SYNC_V_poll        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc_cbr_field_RX_LCV_V_get          ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_field_RX_LCV_V_poll         ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _FC_CBR_IO_H
#define _FC_CBR_IO_H

#include "fc800_pmg_api.h"
#include "fc_cbr_regs.h"
#include "fc_cbr_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fc_cbr_buffer_init               ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | void                 fc_cbr_buffer_flush              ( fc_cbr_buffer_t *b_ptr )
 | UINT32               fc_cbr_reg_read                  ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 fc_cbr_reg_write                 ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 fc_cbr_field_set                 ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 fc_cbr_action_on_write_field_set ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 fc_cbr_burst_read                ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 fc_cbr_burst_write               ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE fc_cbr_poll                      ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fc_cbr_reg_CFG_write                     ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_reg_CFG_field_set                 ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_reg_CFG_read                      ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_reg_RX_CFG_write                  ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_reg_RX_CFG_field_set              ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_reg_RX_CFG_read                   ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_reg_RX_DLOLB_CLK_CFG_write        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_reg_RX_DLOLB_CLK_CFG_field_set    ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_reg_RX_DLOLB_CLK_CFG_read         ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_reg_RX_DLOLB_EN_write             ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_reg_RX_DLOLB_EN_field_set         ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_reg_RX_DLOLB_EN_read              ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_reg_RX_DEFECT_CFG_write           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_reg_RX_DEFECT_CFG_field_set       ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_reg_RX_DEFECT_CFG_read            ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_reg_TX_USER_PATTERN_LSB_write     ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_reg_TX_USER_PATTERN_LSB_field_set ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_reg_TX_USER_PATTERN_LSB_read      ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_reg_TX_CFG_write                  ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_reg_TX_CFG_field_set              ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_reg_TX_CFG_read                   ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_reg_CNT_UPDATE_write              ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_reg_CNT_UPDATE_field_set          ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_reg_CNT_UPDATE_read               ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_cbr_reg_INT_write                     ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_reg_INT_action_on_write_field_set ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_reg_INT_read                      ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_reg_INT_poll                      ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 fc_cbr_reg_INT_EN_write                  ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_cbr_reg_INT_EN_field_set              ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_cbr_reg_INT_EN_read                   ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc_cbr_reg_INT_VAL_read                  ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_cbr_reg_INT_VAL_poll                  ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               fc_cbr_reg_RX_GF_WRD_CNTR_read           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc_cbr_reg_RX_BF_WRD_CNTR_read           ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc_cbr_reg_RX_WORD_CNT_read              ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc_cbr_reg_RX_FRAMES_BAD_CRC_read        ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc_cbr_reg_RX_TOTAL_PACKETS_read         ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc_cbr_reg_LCV_CNT_read                  ( fc_cbr_buffer_t *b_ptr, fc800_pmg_handle_t *h_ptr, UINT32 A )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _FC_CBR_IO_H */
