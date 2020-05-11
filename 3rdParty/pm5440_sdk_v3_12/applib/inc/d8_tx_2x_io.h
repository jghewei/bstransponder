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
 *     and register accessor functions for the d8_tx_2x block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing d8_tx_2x io functions is:
 | {
 |     d8_tx_2x_buffer_t b_ptr[1];
 |     d8_tx_2x_buffer_init( b_ptr, h_ptr );
 |     value = d8_tx_2x_field_<FIELD>_get( b_ptr, h_ptr );
 |     d8_tx_2x_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     d8_tx_2x_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = d8_tx_2x_field_<FIELD>_get( NULL, h_ptr );
 | d8_tx_2x_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------+
 | | Register 0x00000000 SW_RESET_CTRL       |
 | +-----------------------------------------+
 | | bit  12 R/W  PSLB_CLK_DEGLITCH_B_RSTB   |
 | | bit  11 R/W  PSLB_CLK_DEGLITCH_A_RSTB   |
 | | bit  10 R/W  TPCOUT_CLK_DEGLITCH_B_RSTB |
 | | bit  9  R/W  TPCOUT_CLK_DEGLITCH_A_RSTB |
 | | bit  8  R/W  PATT_MON_B_RSTB            |
 | | bit  7  R/W  PATT_MON_A_RSTB            |
 | | bit  6  R/W  PATT_GEN_B_RSTB            |
 | | bit  5  R/W  PATT_GEN_A_RSTB            |
 | | bit  4  R/W  GEN_PISO_B_RSTB            |
 | | bit  3  R/W  GEN_PISO_A_RSTB            |
 | | bit  2  R/W  DESKEW_FIFO_B_RSTB         |
 | | bit  1  R/W  DESKEW_FIFO_A_RSTB         |
 | | bit  0  R/W  DIGITAL_RSTB               |
 | +-----------------------------------------+
 | 
 | void   d8_tx_2x_field_PSLB_CLK_DEGLITCH_B_RSTB_set   ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_PSLB_CLK_DEGLITCH_B_RSTB_get   ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_PSLB_CLK_DEGLITCH_A_RSTB_set   ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_PSLB_CLK_DEGLITCH_A_RSTB_get   ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_B_RSTB_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_B_RSTB_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_A_RSTB_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_A_RSTB_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_PATT_MON_B_RSTB_set            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_PATT_MON_B_RSTB_get            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_PATT_MON_A_RSTB_set            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_PATT_MON_A_RSTB_get            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_PATT_GEN_B_RSTB_set            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_PATT_GEN_B_RSTB_get            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_PATT_GEN_A_RSTB_set            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_PATT_GEN_A_RSTB_get            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_GEN_PISO_B_RSTB_set            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_GEN_PISO_B_RSTB_get            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_GEN_PISO_A_RSTB_set            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_GEN_PISO_A_RSTB_get            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_DESKEW_FIFO_B_RSTB_set         ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_DESKEW_FIFO_B_RSTB_get         ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_DESKEW_FIFO_A_RSTB_set         ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_DESKEW_FIFO_A_RSTB_get         ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_DIGITAL_RSTB_set               ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_DIGITAL_RSTB_get               ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000004 ANALOG_TEST_CTRL |
 | +--------------------------------------+
 | | bit  13:12 R/W  TX_JTAG_IN           |
 | | bit  11:10 R/W  TX_JTAG_EN           |
 | | bit  8     R/W  DLB_SLICE_EN         |
 | | bit  6     R/W  DLB_CH_SEL           |
 | | bit  5     R/W  TS_ATMSB             |
 | | bit  4:0   R/W  ATIN                 |
 | +--------------------------------------+
 | 
 | void   d8_tx_2x_field_TX_JTAG_IN_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_TX_JTAG_IN_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_range_TX_JTAG_IN_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_field_range_TX_JTAG_IN_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_tx_2x_field_TX_JTAG_EN_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_TX_JTAG_EN_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_range_TX_JTAG_EN_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_field_range_TX_JTAG_EN_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_tx_2x_field_DLB_SLICE_EN_set     ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_DLB_SLICE_EN_get     ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_DLB_CH_SEL_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_DLB_CH_SEL_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_TS_ATMSB_set         ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_TS_ATMSB_get         ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_ATIN_set             ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_ATIN_get             ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_range_ATIN_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_field_range_ATIN_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------+
 | | Register 0x00000008 INT_REG  |
 | +------------------------------+
 | | bit  1 R/W  PATT_MON_ERR_B_I |
 | | bit  0 R/W  PATT_MON_ERR_A_I |
 | +------------------------------+
 | 
 | void                 d8_tx_2x_field_PATT_MON_ERR_B_I_set_to_clear ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_tx_2x_field_PATT_MON_ERR_B_I_get          ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_tx_2x_field_PATT_MON_ERR_B_I_poll         ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 d8_tx_2x_field_PATT_MON_ERR_A_I_set_to_clear ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32               d8_tx_2x_field_PATT_MON_ERR_A_I_get          ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_tx_2x_field_PATT_MON_ERR_A_I_poll         ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------+
 | | Register 0x0000000c INT_EN_REG |
 | +--------------------------------+
 | | bit  1 R/W  PATT_MON_ERR_B_E   |
 | | bit  0 R/W  PATT_MON_ERR_A_E   |
 | +--------------------------------+
 | 
 | void   d8_tx_2x_field_PATT_MON_ERR_B_E_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_PATT_MON_ERR_B_E_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void   d8_tx_2x_field_PATT_MON_ERR_A_E_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | UINT32 d8_tx_2x_field_PATT_MON_ERR_A_E_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | 
 | +----------------------------------+
 | | Register 0x00000010 INT_STAT_REG |
 | +----------------------------------+
 | | bit  1 R  PATT_MON_ERR_B_V       |
 | | bit  0 R  PATT_MON_ERR_A_V       |
 | +----------------------------------+
 | 
 | UINT32               d8_tx_2x_field_PATT_MON_ERR_B_V_get  ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_tx_2x_field_PATT_MON_ERR_B_V_poll ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               d8_tx_2x_field_PATT_MON_ERR_A_V_get  ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_tx_2x_field_PATT_MON_ERR_A_V_poll ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x00000014 + (N) * 0x4) SFI5_1_TX_2X_MABC_CFG_1 |
 | |     N[0..1]                                               |
 | +-----------------------------------------------------------+
 | | bit  17    R/W  TX_SQUELCH_OVR                            |
 | | bit  16    R/W  TX_SQUELCH_OVR_EN                         |
 | | bit  15:11 R/W  TX_SWING                                  |
 | | bit  8     R/W  TX_ATMSB                                  |
 | | bit  4     R/W  TX_TEST_EN                                |
 | | bit  3     R/W  TX_PISO_PBUS_WIDTH                        |
 | | bit  2     R/W  PISO_RSTB                                 |
 | | bit  1     R/W  PISO_ENB                                  |
 | | bit  0     R/W  TX_ENB                                    |
 | +-----------------------------------------------------------+
 | 
 | void   d8_tx_2x_field_TX_SQUELCH_OVR_set     ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_TX_SQUELCH_OVR_get     ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_TX_SQUELCH_OVR_EN_set  ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_TX_SQUELCH_OVR_EN_get  ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_TX_SWING_set           ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_TX_SWING_get           ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_range_TX_SWING_set     ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_field_range_TX_SWING_get     ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_tx_2x_field_TX_ATMSB_set           ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_TX_ATMSB_get           ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_TX_TEST_EN_set         ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_TX_TEST_EN_get         ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_TX_PISO_PBUS_WIDTH_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_TX_PISO_PBUS_WIDTH_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_PISO_RSTB_set          ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_PISO_RSTB_get          ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_PISO_ENB_set           ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_PISO_ENB_get           ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_TX_ENB_set             ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_TX_ENB_get             ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x0000001c + (N) * 0x4) SFI5_1_TX_2X_MABC_CFG_2 |
 | |     N[0..1]                                               |
 | +-----------------------------------------------------------+
 | | bit  31:8 R/W  TX_MODE                                    |
 | | bit  7:0  R/W  TX_CTRL                                    |
 | +-----------------------------------------------------------+
 | 
 | void   d8_tx_2x_field_TX_MODE_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_TX_MODE_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_range_TX_MODE_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_field_range_TX_MODE_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_tx_2x_field_TX_CTRL_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_TX_CTRL_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_range_TX_CTRL_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_field_range_TX_CTRL_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x00000024 + (N) * 0x4) DESKEW_FIFO_CFG |
 | |     N[0..1]                                       |
 | +---------------------------------------------------+
 | | bit  7:4 R/W  RD_PTR_SYNC_VAL                     |
 | | bit  0   R/W  RD_PTR_SYNC                         |
 | +---------------------------------------------------+
 | 
 | void   d8_tx_2x_field_RD_PTR_SYNC_VAL_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_RD_PTR_SYNC_VAL_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_range_RD_PTR_SYNC_VAL_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_field_range_RD_PTR_SYNC_VAL_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_tx_2x_field_RD_PTR_SYNC_set           ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_RD_PTR_SYNC_get           ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------------+
 | | Register (0x0000002c + (N) * 0x4) DATAPATH_CFG |
 | |     N[0..1]                                    |
 | +------------------------------------------------+
 | | bit  10  R/W  INV_DAT_POL                      |
 | | bit  9   R/W  TCLKOUT_SEL                      |
 | | bit  8   R/W  PSLB_CLK_DEGLITCH_EN             |
 | | bit  7   R/W  TPCOUT_CLK_DEGLITCH_EN           |
 | | bit  6   R/W  PATT_INSERT_EN                   |
 | | bit  5   R/W  P2S_DLB_EN                       |
 | | bit  4   R/W  RX2TX_DLB_EN                     |
 | | bit  3   R/W  GEN_PISO_BUS_WIDTH               |
 | | bit  2:0 R/W  GEN_PISO_DIV_RATIO               |
 | +------------------------------------------------+
 | 
 | void   d8_tx_2x_field_INV_DAT_POL_set              ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_INV_DAT_POL_get              ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_TCLKOUT_SEL_set              ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_TCLKOUT_SEL_get              ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_PSLB_CLK_DEGLITCH_EN_set     ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_PSLB_CLK_DEGLITCH_EN_get     ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_EN_set   ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_TPCOUT_CLK_DEGLITCH_EN_get   ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_PATT_INSERT_EN_set           ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_PATT_INSERT_EN_get           ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_P2S_DLB_EN_set               ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_P2S_DLB_EN_get               ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_RX2TX_DLB_EN_set             ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_RX2TX_DLB_EN_get             ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_GEN_PISO_BUS_WIDTH_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_GEN_PISO_BUS_WIDTH_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_GEN_PISO_DIV_RATIO_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_GEN_PISO_DIV_RATIO_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_range_GEN_PISO_DIV_RATIO_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_field_range_GEN_PISO_DIV_RATIO_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x00000034 + (N) * 0x4) PATTERN_GENERATOR_CFG_1 |
 | |     N[0..1]                                               |
 | +-----------------------------------------------------------+
 | | bit  13:8 R/W  GEN_MON_BUS_WIDTH                          |
 | | bit  7:4  R/W  GEN_MODE                                   |
 | | bit  2    R/W  GEN_ERR_INJ                                |
 | | bit  1    R/W  GEN_INV                                    |
 | | bit  0    R/W  GEN_EN                                     |
 | +-----------------------------------------------------------+
 | 
 | void   d8_tx_2x_field_GEN_MON_BUS_WIDTH_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_GEN_MON_BUS_WIDTH_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_range_GEN_MON_BUS_WIDTH_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_field_range_GEN_MON_BUS_WIDTH_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_tx_2x_field_GEN_MODE_set                ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_GEN_MODE_get                ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_range_GEN_MODE_set          ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_field_range_GEN_MODE_get          ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_tx_2x_field_GEN_ERR_INJ_set             ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_GEN_ERR_INJ_get             ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_GEN_INV_set                 ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_GEN_INV_get                 ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_GEN_EN_set                  ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_GEN_EN_get                  ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x0000003c + (N) * 0x4) PATTERN_GEN_USER_PATTERN_1 |
 | |     N[0..1]                                                  |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  GEN_USR_PAT_LSB                               |
 | +--------------------------------------------------------------+
 | 
 | void   d8_tx_2x_lfield_GEN_USR_PAT_LSB_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value[2] )
 | void   d8_tx_2x_lfield_GEN_USR_PAT_LSB_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value[2] )
 | void   d8_tx_2x_lfield_range_GEN_USR_PAT_LSB_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_lfield_range_GEN_USR_PAT_LSB_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x00000044 + (N) * 0x4) PATTERN_GEN_USER_PATTERN_2 |
 | |     N[0..1]                                                  |
 | +--------------------------------------------------------------+
 | | bit  7:0 R/W  GEN_USR_PAT_MSB                                |
 | +--------------------------------------------------------------+
 | 
 | void   d8_tx_2x_field_GEN_USR_PAT_MSB_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_GEN_USR_PAT_MSB_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_range_GEN_USR_PAT_MSB_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_field_range_GEN_USR_PAT_MSB_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x0000004c + (N) * 0x4) PATTERN_MON_CFG_1 |
 | |     N[0..1]                                         |
 | +-----------------------------------------------------+
 | | bit  7:4 R/W  MON_MODE                              |
 | | bit  2   R/W  MON_READ                              |
 | | bit  1   R/W  MON_INV                               |
 | | bit  0   R/W  MON_EN                                |
 | +-----------------------------------------------------+
 | 
 | void   d8_tx_2x_field_MON_MODE_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_MON_MODE_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_range_MON_MODE_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_field_range_MON_MODE_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   d8_tx_2x_field_MON_READ_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_MON_READ_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_MON_INV_set        ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_MON_INV_get        ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_MON_EN_set         ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_MON_EN_get         ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x00000054 + (N) * 0x4) PATTERN_MON_USER_PATTERN_1 |
 | |     N[0..1]                                                  |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  MON_USR_PAT_LSB                               |
 | +--------------------------------------------------------------+
 | 
 | void   d8_tx_2x_lfield_MON_USR_PAT_LSB_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value[2] )
 | void   d8_tx_2x_lfield_MON_USR_PAT_LSB_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value[2] )
 | void   d8_tx_2x_lfield_range_MON_USR_PAT_LSB_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_lfield_range_MON_USR_PAT_LSB_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x0000005c + (N) * 0x4) PATTERN_MON_USER_PATTERN_2 |
 | |     N[0..1]                                                  |
 | +--------------------------------------------------------------+
 | | bit  7:0 R/W  MON_USR_PAT_MSB                                |
 | +--------------------------------------------------------------+
 | 
 | void   d8_tx_2x_field_MON_USR_PAT_MSB_set       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 d8_tx_2x_field_MON_USR_PAT_MSB_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | void   d8_tx_2x_field_range_MON_USR_PAT_MSB_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 d8_tx_2x_field_range_MON_USR_PAT_MSB_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000064 + (N) * 0x4) PATTERN_MON_ERR_CNT_1 |
 | |     N[0..1]                                             |
 | +---------------------------------------------------------+
 | | bit  31:0 R  MON_ERR_CNT                                |
 | +---------------------------------------------------------+
 | 
 | void   d8_tx_2x_lfield_MON_ERR_CNT_get       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 d8_tx_2x_lfield_range_MON_ERR_CNT_get ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _D8_TX_2X_IO_H
#define _D8_TX_2X_IO_H

#include "d8_tx_2x.h"
#include "d8_tx_2x_regs.h"
#include "d8_tx_2x_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 d8_tx_2x_buffer_init               ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | void                 d8_tx_2x_buffer_flush              ( d8_tx_2x_buffer_t *b_ptr )
 | UINT32               d8_tx_2x_reg_read                  ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 d8_tx_2x_reg_write                 ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 d8_tx_2x_field_set                 ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 d8_tx_2x_action_on_write_field_set ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 d8_tx_2x_burst_read                ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 d8_tx_2x_burst_write               ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE d8_tx_2x_poll                      ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 d8_tx_2x_reg_SW_RESET_CTRL_write                          ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | void                 d8_tx_2x_reg_SW_RESET_CTRL_field_set                      ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_tx_2x_reg_SW_RESET_CTRL_read                           ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | 
 | void                 d8_tx_2x_reg_ANALOG_TEST_CTRL_write                       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | void                 d8_tx_2x_reg_ANALOG_TEST_CTRL_field_set                   ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_tx_2x_reg_ANALOG_TEST_CTRL_read                        ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | 
 | void                 d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_burst_write ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_field_set   ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_burst_read  ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_1_array_read        ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 N )
 | 
 | void                 d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_burst_write ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_field_set   ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_burst_read  ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_1_array_read        ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 N )
 | 
 | void                 d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_write          ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_field_set      ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_1_array_read           ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_write          ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_field_set      ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_tx_2x_reg_SFI5_1_TX_2X_MABC_CFG_2_array_read           ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_tx_2x_reg_DESKEW_FIFO_CFG_array_write                  ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_tx_2x_reg_DESKEW_FIFO_CFG_array_field_set              ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_tx_2x_reg_DESKEW_FIFO_CFG_array_read                   ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_tx_2x_reg_DATAPATH_CFG_array_write                     ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_tx_2x_reg_DATAPATH_CFG_array_field_set                 ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_tx_2x_reg_DATAPATH_CFG_array_read                      ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_write          ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_field_set      ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_tx_2x_reg_PATTERN_GENERATOR_CFG_1_array_read           ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_write       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_field_set   ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_tx_2x_reg_PATTERN_GEN_USER_PATTERN_2_array_read        ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_tx_2x_reg_PATTERN_MON_CFG_1_array_write                ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_tx_2x_reg_PATTERN_MON_CFG_1_array_field_set            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_tx_2x_reg_PATTERN_MON_CFG_1_array_read                 ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_write       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_field_set   ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_tx_2x_reg_PATTERN_MON_USER_PATTERN_2_array_read        ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32  N )
 | 
 | void                 d8_tx_2x_reg_INT_EN_REG_write                             ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | void                 d8_tx_2x_reg_INT_EN_REG_field_set                         ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_tx_2x_reg_INT_EN_REG_read                              ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | 
 | void                 d8_tx_2x_reg_INT_REG_write                                ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 value )
 | void                 d8_tx_2x_reg_INT_REG_action_on_write_field_set            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               d8_tx_2x_reg_INT_REG_read                                 ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_tx_2x_reg_INT_REG_poll                                 ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               d8_tx_2x_reg_INT_STAT_REG_read                            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE d8_tx_2x_reg_INT_STAT_REG_poll                            ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 d8_tx_2x_reg_PATTERN_MON_ERR_CNT_1_array_burst_read       ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               d8_tx_2x_reg_PATTERN_MON_ERR_CNT_1_array_read             ( d8_tx_2x_buffer_t *b_ptr, d8_tx_2x_handle_t *h_ptr, UINT32 N )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _D8_TX_2X_IO_H */
