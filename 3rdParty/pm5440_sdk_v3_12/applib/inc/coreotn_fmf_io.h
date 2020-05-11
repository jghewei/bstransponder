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
 *     and register accessor functions for the coreotn_fmf block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing coreotn_fmf io functions is:
 | {
 |     coreotn_fmf_buffer_t b_ptr[1];
 |     coreotn_fmf_buffer_init( b_ptr, h_ptr, A );
 |     value = coreotn_fmf_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     coreotn_fmf_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     coreotn_fmf_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = coreotn_fmf_field_<FIELD>_get( NULL, h_ptr, A );
 | coreotn_fmf_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------+
 | | Register 0x000ac000 SW_RESET_REG |
 | +----------------------------------+
 | | bit  10 R/W  PRBS_RST            |
 | | bit  9  R/W  ODUKSC2_RST         |
 | | bit  8  R/W  ODUJAT_RST          |
 | | bit  7  R/W  FRM_RCP96_RST       |
 | | bit  6  R/W  FRM_RCP12_RST       |
 | | bit  5  R/W  RFRM12_RST          |
 | | bit  4  R/W  TFRM12_RST          |
 | | bit  3  R/W  RFRM96_RST          |
 | | bit  2  R/W  TFRM96_RST          |
 | | bit  1  R/W  ODTU_DMX_RST        |
 | | bit  0  R/W  ODTU_MUX_RST        |
 | +----------------------------------+
 | 
 | void   coreotn_fmf_field_PRBS_RST_set      ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_PRBS_RST_get      ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_ODUKSC2_RST_set   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_ODUKSC2_RST_get   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_ODUJAT_RST_set    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_ODUJAT_RST_get    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_FRM_RCP96_RST_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_FRM_RCP96_RST_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_FRM_RCP12_RST_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_FRM_RCP12_RST_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_RFRM12_RST_set    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_RFRM12_RST_get    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_TFRM12_RST_set    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_TFRM12_RST_get    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_RFRM96_RST_set    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_RFRM96_RST_get    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_TFRM96_RST_set    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_TFRM96_RST_get    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_ODTU_DMX_RST_set  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_ODTU_DMX_RST_get  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_ODTU_MUX_RST_set  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_ODTU_MUX_RST_get  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------+
 | | Register 0x000ac004 CLK_GATING_REG |
 | +------------------------------------+
 | | bit  11 R/W  FILTER_LOWPWR         |
 | | bit  10 R/W  PRBS_LOWPWR           |
 | | bit  9  R/W  ODUKSC2_LOWPWR        |
 | | bit  8  R/W  ODUJAT_LOWPWR         |
 | | bit  7  R/W  FRM_RCP96_LOWPWR      |
 | | bit  6  R/W  FRM_RCP12_LOWPWR      |
 | | bit  5  R/W  RFRM12_LOWPWR         |
 | | bit  4  R/W  TFRM12_LOWPWR         |
 | | bit  3  R/W  RFRM96_LOWPWR         |
 | | bit  2  R/W  TFRM96_LOWPWR         |
 | | bit  1  R/W  ODTU_DMX_LOWPWR       |
 | | bit  0  R/W  ODTU_MUX_LOWPWR       |
 | +------------------------------------+
 | 
 | void   coreotn_fmf_field_FILTER_LOWPWR_set    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_FILTER_LOWPWR_get    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_PRBS_LOWPWR_set      ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_PRBS_LOWPWR_get      ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_ODUKSC2_LOWPWR_set   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_ODUKSC2_LOWPWR_get   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_ODUJAT_LOWPWR_set    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_ODUJAT_LOWPWR_get    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_FRM_RCP96_LOWPWR_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_FRM_RCP96_LOWPWR_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_FRM_RCP12_LOWPWR_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_FRM_RCP12_LOWPWR_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_RFRM12_LOWPWR_set    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_RFRM12_LOWPWR_get    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_TFRM12_LOWPWR_set    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_TFRM12_LOWPWR_get    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_RFRM96_LOWPWR_set    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_RFRM96_LOWPWR_get    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_TFRM96_LOWPWR_set    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_TFRM96_LOWPWR_get    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_ODTU_DMX_LOWPWR_set  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_ODTU_DMX_LOWPWR_get  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_ODTU_MUX_LOWPWR_set  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_ODTU_MUX_LOWPWR_get  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------+
 | | Register 0x000ac008 RAM_LOW_POWER_REG |
 | +---------------------------------------+
 | | bit  10 R/W  PRBS_RAM_LOWPWR          |
 | | bit  9  R/W  ODUKSC2_RAM_LOWPWR       |
 | | bit  8  R/W  ODUJAT_RAM_LOWPWR        |
 | | bit  5  R/W  RFRM12_RAM_LOWPWR        |
 | | bit  4  R/W  TFRM12_RAM_LOWPWR        |
 | | bit  3  R/W  RFRM96_RAM_LOWPWR        |
 | | bit  2  R/W  TFRM96_RAM_LOWPWR        |
 | | bit  1  R/W  ODTU_DMX_RAM_LOWPWR      |
 | | bit  0  R/W  ODTU_MUX_RAM_LOWPWR      |
 | +---------------------------------------+
 | 
 | void   coreotn_fmf_field_PRBS_RAM_LOWPWR_set     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_PRBS_RAM_LOWPWR_get     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_ODUKSC2_RAM_LOWPWR_set  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_ODUKSC2_RAM_LOWPWR_get  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_ODUJAT_RAM_LOWPWR_set   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_ODUJAT_RAM_LOWPWR_get   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_RFRM12_RAM_LOWPWR_set   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_RFRM12_RAM_LOWPWR_get   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_TFRM12_RAM_LOWPWR_set   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_TFRM12_RAM_LOWPWR_get   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_RFRM96_RAM_LOWPWR_set   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_RFRM96_RAM_LOWPWR_get   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_TFRM96_RAM_LOWPWR_set   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_TFRM96_RAM_LOWPWR_get   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_ODTU_DMX_RAM_LOWPWR_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_ODTU_DMX_RAM_LOWPWR_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_ODTU_MUX_RAM_LOWPWR_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_ODTU_MUX_RAM_LOWPWR_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------+
 | | Register 0x000ac014 DCI_CONFIG_REG |
 | +------------------------------------+
 | | bit  16    R/W  DCI_MUX_CTRL_5     |
 | | bit  13:12 R/W  DCI_MUX_CTRL_4     |
 | | bit  8     R/W  DCI_MUX_CTRL_3     |
 | | bit  4     R/W  DCI_MUX_CTRL_2     |
 | | bit  0     R/W  DCI_MUX_CTRL_1     |
 | +------------------------------------+
 | 
 | void   coreotn_fmf_field_DCI_MUX_CTRL_5_set       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_DCI_MUX_CTRL_5_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_DCI_MUX_CTRL_4_set       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_DCI_MUX_CTRL_4_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_range_DCI_MUX_CTRL_4_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fmf_field_range_DCI_MUX_CTRL_4_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   coreotn_fmf_field_DCI_MUX_CTRL_3_set       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_DCI_MUX_CTRL_3_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_DCI_MUX_CTRL_2_set       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_DCI_MUX_CTRL_2_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_DCI_MUX_CTRL_1_set       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_DCI_MUX_CTRL_1_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------+
 | | Register 0x000ac018 DCI_XOFF_CONFIG_REG |
 | +-----------------------------------------+
 | | bit  8 R/W  DCI_XOFF_CTRL_3             |
 | | bit  4 R/W  DCI_XOFF_CTRL_2             |
 | | bit  0 R/W  DCI_XOFF_CTRL_1             |
 | +-----------------------------------------+
 | 
 | void   coreotn_fmf_field_DCI_XOFF_CTRL_3_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_DCI_XOFF_CTRL_3_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_DCI_XOFF_CTRL_2_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_DCI_XOFF_CTRL_2_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_DCI_XOFF_CTRL_1_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_DCI_XOFF_CTRL_1_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------+
 | | Register 0x000ac01c TSB_CONFIG_REG |
 | +------------------------------------+
 | | bit  8 R/W  NORM_RATE_SELECT       |
 | | bit  0 R/W  ODTU_MUX_MPMX          |
 | +------------------------------------+
 | 
 | void   coreotn_fmf_field_NORM_RATE_SELECT_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_NORM_RATE_SELECT_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_ODTU_MUX_MPMX_set    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_ODTU_MUX_MPMX_get    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------------+
 | | Register 0x000ac020 FRM_RCP_12_CONFIG_REG_1 |
 | +---------------------------------------------+
 | | bit  11:0 R/W  REMOTE_RCP12_CFG             |
 | +---------------------------------------------+
 | 
 | void   coreotn_fmf_field_REMOTE_RCP12_CFG_set       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_REMOTE_RCP12_CFG_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_range_REMOTE_RCP12_CFG_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fmf_field_range_REMOTE_RCP12_CFG_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x000ac024 FRM_RCP_12_CONFIG_REG_2 |
 | +---------------------------------------------+
 | | bit  11:0 R/W  CFC_FIFO1_CLRB_CFG           |
 | +---------------------------------------------+
 | 
 | void   coreotn_fmf_field_CFC_FIFO1_CLRB_CFG_set       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_CFC_FIFO1_CLRB_CFG_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_range_CFC_FIFO1_CLRB_CFG_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fmf_field_range_CFC_FIFO1_CLRB_CFG_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x000ac034 FRM_RCP_96_CONFIG_REG_1 |
 | +---------------------------------------------+
 | | bit  31:0 R/W  REMOTE_RCP96_CFG             |
 | +---------------------------------------------+
 | 
 | void   coreotn_fmf_lfield_REMOTE_RCP96_CFG_set       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3] )
 | void   coreotn_fmf_lfield_REMOTE_RCP96_CFG_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3] )
 | void   coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fmf_lfield_range_REMOTE_RCP96_CFG_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x000ac040 FRM_RCP_96_CONFIG_REG_4 |
 | +---------------------------------------------+
 | | bit  31:0 R/W  CFC_FIFO2_CLRB_CFG           |
 | +---------------------------------------------+
 | 
 | void   coreotn_fmf_lfield_CFC_FIFO2_CLRB_CFG_set       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3] )
 | void   coreotn_fmf_lfield_CFC_FIFO2_CLRB_CFG_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3] )
 | void   coreotn_fmf_lfield_range_CFC_FIFO2_CLRB_CFG_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fmf_lfield_range_CFC_FIFO2_CLRB_CFG_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------+
 | | Register 0x000ac094 GPO_REG |
 | +-----------------------------+
 | | bit  31:0 R/W  SPARE_GPO    |
 | +-----------------------------+
 | 
 | void   coreotn_fmf_field_SPARE_GPO_set       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_SPARE_GPO_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_range_SPARE_GPO_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fmf_field_range_SPARE_GPO_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x000ac09c + (N) * 0x4) PRBS_CHANNEL_CFG_REG |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  14:8 R/W  PRBS_CHID                               |
 | | bit  3    R/W  PRBS_SEL_INV_NONINV                     |
 | | bit  2    R/W  PRBS_SEL_31_11                          |
 | | bit  1    R/W  PRBS_MON_EN                             |
 | | bit  0    R/W  PRBS_GEN_EN                             |
 | +--------------------------------------------------------+
 | 
 | void   coreotn_fmf_field_PRBS_CHID_set           ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 coreotn_fmf_field_PRBS_CHID_get           ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   coreotn_fmf_field_range_PRBS_CHID_set     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fmf_field_range_PRBS_CHID_get     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   coreotn_fmf_field_PRBS_SEL_INV_NONINV_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 coreotn_fmf_field_PRBS_SEL_INV_NONINV_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   coreotn_fmf_field_PRBS_SEL_31_11_set      ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 coreotn_fmf_field_PRBS_SEL_31_11_get      ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   coreotn_fmf_field_PRBS_MON_EN_set         ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 coreotn_fmf_field_PRBS_MON_EN_get         ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   coreotn_fmf_field_PRBS_GEN_EN_set         ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 coreotn_fmf_field_PRBS_GEN_EN_get         ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +-------------------------------------------------------+
 | | Register (0x000ac0cc + (N) * 0x4) PRBS_DEFECT_CFG_REG |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  0 R/W  PRBS_INS_DEFECT                           |
 | +-------------------------------------------------------+
 | 
 | void   coreotn_fmf_field_PRBS_INS_DEFECT_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 coreotn_fmf_field_PRBS_INS_DEFECT_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +-----------------------------------------------------+
 | | Register (0x000ac0fc + (N) * 0x4) PRBS_TSE_STAT_REG |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  31:0 R  PRBS_TSE_CHAN_REG                      |
 | +-----------------------------------------------------+
 | 
 | UINT32 coreotn_fmf_field_PRBS_TSE_CHAN_REG_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 coreotn_fmf_field_range_PRBS_TSE_CHAN_REG_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x000ac12c PRBS_BER_TH_LOWER_CFG_REG |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  PRBS_BER_THRES                 |
 | +-----------------------------------------------+
 | 
 | void   coreotn_fmf_lfield_PRBS_BER_THRES_set       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | void   coreotn_fmf_lfield_PRBS_BER_THRES_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | void   coreotn_fmf_lfield_range_PRBS_BER_THRES_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fmf_lfield_range_PRBS_BER_THRES_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x000ac134 PRBS_RESEED_REG  |
 | +--------------------------------------+
 | | bit  31:16 R/W  PRBS_RESEED_THR      |
 | | bit  15:0  R/W  PRBS_RESEED_INTERVAL |
 | +--------------------------------------+
 | 
 | void   coreotn_fmf_field_PRBS_RESEED_THR_set            ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_PRBS_RESEED_THR_get            ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_range_PRBS_RESEED_THR_set      ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fmf_field_range_PRBS_RESEED_THR_get      ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   coreotn_fmf_field_PRBS_RESEED_INTERVAL_set       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_PRBS_RESEED_INTERVAL_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_range_PRBS_RESEED_INTERVAL_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fmf_field_range_PRBS_RESEED_INTERVAL_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x000ac140 PRBS_TSE_ROLLOVER_STAT_REG |
 | +------------------------------------------------+
 | | bit  11:0 R  PRBS_TSE_ROVER                    |
 | +------------------------------------------------+
 | 
 | UINT32 coreotn_fmf_field_PRBS_TSE_ROVER_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | UINT32 coreotn_fmf_field_range_PRBS_TSE_ROVER_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x000ac144 PRBS_DLSS_INT_EN_REG |
 | +------------------------------------------+
 | | bit  11:0 R/W  PRBS_DLSS_E               |
 | +------------------------------------------+
 | 
 | void   coreotn_fmf_field_PRBS_DLSS_E_set       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fmf_field_PRBS_DLSS_E_get       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fmf_field_range_PRBS_DLSS_E_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fmf_field_range_PRBS_DLSS_E_get ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x000ac148 PRBS_DLSS_INT_STAT_REG |
 | +--------------------------------------------+
 | | bit  11:0 R/W  PRBS_DLSS_I                 |
 | +--------------------------------------------+
 | 
 | void                 coreotn_fmf_field_PRBS_DLSS_I_set_to_clear       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               coreotn_fmf_field_PRBS_DLSS_I_get                ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void                 coreotn_fmf_field_range_PRBS_DLSS_I_set_to_clear ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               coreotn_fmf_field_range_PRBS_DLSS_I_get          ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE coreotn_fmf_field_range_PRBS_DLSS_I_poll         ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE coreotn_fmf_field_PRBS_DLSS_I_poll               ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x000ac14c PRBS_DLSS_INT_VAL_REG |
 | +-------------------------------------------+
 | | bit  11:0 R  PRBS_DLSS_V                  |
 | +-------------------------------------------+
 | 
 | UINT32               coreotn_fmf_field_PRBS_DLSS_V_get        ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | UINT32               coreotn_fmf_field_range_PRBS_DLSS_V_get  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE coreotn_fmf_field_range_PRBS_DLSS_V_poll ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE coreotn_fmf_field_PRBS_DLSS_V_poll       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _COREOTN_FMF_IO_H
#define _COREOTN_FMF_IO_H

#include "coreotn_loc.h"
#include "coreotn_fmf_regs.h"
#include "coreotn_fmf_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 coreotn_fmf_buffer_init               ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void                 coreotn_fmf_buffer_flush              ( coreotn_fmf_buffer_t *b_ptr )
 | UINT32               coreotn_fmf_reg_read                  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 coreotn_fmf_reg_write                 ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 coreotn_fmf_field_set                 ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 coreotn_fmf_action_on_write_field_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 coreotn_fmf_burst_read                ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 coreotn_fmf_burst_write               ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE coreotn_fmf_poll                      ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 coreotn_fmf_reg_SW_RESET_REG_write                               ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_SW_RESET_REG_field_set                           ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_SW_RESET_REG_read                                ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_CLK_GATING_REG_write                             ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_CLK_GATING_REG_field_set                         ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_CLK_GATING_REG_read                              ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_RAM_LOW_POWER_REG_write                          ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_RAM_LOW_POWER_REG_field_set                      ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_RAM_LOW_POWER_REG_read                           ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_DCI_CONFIG_REG_write                             ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_DCI_CONFIG_REG_field_set                         ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_DCI_CONFIG_REG_read                              ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_write                        ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_field_set                    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_DCI_XOFF_CONFIG_REG_read                         ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_TSB_CONFIG_REG_write                             ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_TSB_CONFIG_REG_field_set                         ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_TSB_CONFIG_REG_read                              ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_write                    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_field_set                ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_1_read                     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_write                    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_field_set                ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_FRM_RCP_12_CONFIG_REG_2_read                     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_write                    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_field_set                ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_1_read                     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_write                    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_field_set                ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_2_read                     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_write                    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_field_set                ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_3_read                     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_write                    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_field_set                ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_4_read                     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_write                    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_field_set                ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_5_read                     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_write                    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_field_set                ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_FRM_RCP_96_CONFIG_REG_6_read                     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_GPO_REG_write                                    ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_GPO_REG_field_set                                ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_GPO_REG_read                                     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_write                  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_field_set              ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_PRBS_BER_TH_LOWER_CFG_REG_read                   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_write                  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_field_set              ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_PRBS_BER_TH_UPPER_CFG_REG_read                   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_PRBS_RESEED_REG_write                            ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_PRBS_RESEED_REG_field_set                        ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_PRBS_RESEED_REG_read                             ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_write                 ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_field_set             ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_PRBS_CHANNEL_CFG_REG_array_read                  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 coreotn_fmf_reg_PRBS_DEFECT_CFG_REG_array_write                  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 coreotn_fmf_reg_PRBS_DEFECT_CFG_REG_array_field_set              ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_PRBS_DEFECT_CFG_REG_array_read                   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_write                       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_field_set                   ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_PRBS_DLSS_INT_EN_REG_read                        ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_write                     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_action_on_write_field_set ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_read                      ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE coreotn_fmf_reg_PRBS_DLSS_INT_STAT_REG_poll                      ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               coreotn_fmf_reg_PRBS_DLSS_INT_VAL_REG_read                       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE coreotn_fmf_reg_PRBS_DLSS_INT_VAL_REG_poll                       ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               coreotn_fmf_reg_PRBS_TSE_ROLLOVER_STAT_REG_read                  ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               coreotn_fmf_reg_PRBS_TSE_STAT_REG_array_read                     ( coreotn_fmf_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _COREOTN_FMF_IO_H */
