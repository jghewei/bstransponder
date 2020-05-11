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
 *     and register accessor functions for the sfi51_tx_18x_serdes_glue block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing sfi51_tx_18x_serdes_glue io functions is:
 | {
 |     sfi51_tx_18x_serdes_glue_buffer_t b_ptr[1];
 |     sfi51_tx_18x_serdes_glue_buffer_init( b_ptr, h_ptr );
 |     value = sfi51_tx_18x_serdes_glue_field_<FIELD>_get( b_ptr, h_ptr );
 |     sfi51_tx_18x_serdes_glue_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     sfi51_tx_18x_serdes_glue_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = sfi51_tx_18x_serdes_glue_field_<FIELD>_get( NULL, h_ptr );
 | sfi51_tx_18x_serdes_glue_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------+
 | | Register 0x00000000 SW_RESET_CTRL |
 | +-----------------------------------+
 | | bit  3 R/W  RXS_CTRL_RSTB         |
 | | bit  2 R/W  LOOPTIME_RSTB         |
 | | bit  1 R/W  DIGITAL_RSTB          |
 | | bit  0 R/W  SW_RESET              |
 | +-----------------------------------+
 | 
 | void   sfi51_tx_18x_serdes_glue_field_RXS_CTRL_RSTB_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_RXS_CTRL_RSTB_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_LOOPTIME_RSTB_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_LOOPTIME_RSTB_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_DIGITAL_RSTB_set  ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_DIGITAL_RSTB_get  ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_SW_RESET_set      ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_SW_RESET_get      ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00000004 LOOPTIMING_CFG_REG_1 |
 | +------------------------------------------+
 | | bit  29:16 R/W  DSM_DENOM                |
 | | bit  15:12 R/W  LOOPTIME_FREEZE_CNT      |
 | | bit  8     R/W  LOOPTIME_FREEZE          |
 | | bit  7:6   R/W  RCLK_WIDTH               |
 | | bit  3:2   R/W  DSM_TYPE                 |
 | | bit  0     R/W  PMM_ENABLE               |
 | +------------------------------------------+
 | 
 | void   sfi51_tx_18x_serdes_glue_field_DSM_DENOM_set                 ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_DSM_DENOM_get                 ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_range_DSM_DENOM_set           ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_range_DSM_DENOM_get           ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_CNT_set       ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_CNT_get       ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_range_LOOPTIME_FREEZE_CNT_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_range_LOOPTIME_FREEZE_CNT_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_set           ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_LOOPTIME_FREEZE_get           ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_RCLK_WIDTH_set                ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_RCLK_WIDTH_get                ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_range_RCLK_WIDTH_set          ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_range_RCLK_WIDTH_get          ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sfi51_tx_18x_serdes_glue_field_DSM_TYPE_set                  ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_DSM_TYPE_get                  ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_range_DSM_TYPE_set            ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_range_DSM_TYPE_get            ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sfi51_tx_18x_serdes_glue_field_PMM_ENABLE_set                ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_PMM_ENABLE_get                ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00000008 LOOPTIMING_CFG_REG_2 |
 | +------------------------------------------+
 | | bit  31:16 R/W  PKT_PERIOD               |
 | | bit  13:9  R/W  INC_STEP                 |
 | | bit  8:0   R/W  PKT_SIZE                 |
 | +------------------------------------------+
 | 
 | void   sfi51_tx_18x_serdes_glue_field_PKT_PERIOD_set       ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_PKT_PERIOD_get       ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_range_PKT_PERIOD_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_range_PKT_PERIOD_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sfi51_tx_18x_serdes_glue_field_INC_STEP_set         ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_INC_STEP_get         ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_range_INC_STEP_set   ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_range_INC_STEP_get   ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sfi51_tx_18x_serdes_glue_field_PKT_SIZE_set         ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_PKT_SIZE_get         ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_range_PKT_SIZE_set   ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_range_PKT_SIZE_get   ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x0000000c RXS_CTRLLER_CFG_2 |
 | +---------------------------------------+
 | | bit  10  R/W  SQUELCH_OVR             |
 | | bit  9:8 R/W  SQUELCH_OVR_MODE        |
 | | bit  6   R/W  GEN_FORCE_NOMINAL       |
 | | bit  5   R/W  GEN_HOLDOVER            |
 | | bit  4   R/W  GEN_FREEZE              |
 | | bit  3:2 R/W  RXS_OVR_MODE            |
 | | bit  1   R/W  RXS_OVR                 |
 | | bit  0   R/W  RXS_CTRL_EN             |
 | +---------------------------------------+
 | 
 | void   sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_set            ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_get            ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_MODE_set       ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_SQUELCH_OVR_MODE_get       ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_range_SQUELCH_OVR_MODE_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_range_SQUELCH_OVR_MODE_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sfi51_tx_18x_serdes_glue_field_GEN_FORCE_NOMINAL_set      ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_GEN_FORCE_NOMINAL_get      ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_GEN_HOLDOVER_set           ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_GEN_HOLDOVER_get           ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_GEN_FREEZE_set             ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_GEN_FREEZE_get             ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_RXS_OVR_MODE_set           ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_RXS_OVR_MODE_get           ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_range_RXS_OVR_MODE_set     ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_range_RXS_OVR_MODE_get     ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sfi51_tx_18x_serdes_glue_field_RXS_OVR_set                ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_RXS_OVR_get                ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_RXS_CTRL_EN_set            ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_RXS_CTRL_EN_get            ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000010 GLUE_LOGIC_CFG |
 | +------------------------------------+
 | | bit  28    R/W  LI_RX_OOF_LOF_POL  |
 | | bit  27    R/W  RXS_POL            |
 | | bit  24:22 R/W  REFCLK_SEL         |
 | | bit  20    R/W  CKCTL_SEL          |
 | | bit  19:0  R/W  DCK_DATA_PATT      |
 | +------------------------------------+
 | 
 | void   sfi51_tx_18x_serdes_glue_field_LI_RX_OOF_LOF_POL_set   ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_LI_RX_OOF_LOF_POL_get   ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_RXS_POL_set             ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_RXS_POL_get             ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_REFCLK_SEL_set          ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_REFCLK_SEL_get          ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_range_REFCLK_SEL_set    ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_range_REFCLK_SEL_get    ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sfi51_tx_18x_serdes_glue_field_CKCTL_SEL_set           ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_CKCTL_SEL_get           ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_DCK_DATA_PATT_set       ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_DCK_DATA_PATT_get       ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_range_DCK_DATA_PATT_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_range_DCK_DATA_PATT_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000014 DEVICE_INT_EN_REG |
 | +---------------------------------------+
 | | bit  9 R/W  TX_2X_SLICE8_INT_DEV_E    |
 | | bit  8 R/W  TX_2X_SLICE7_INT_DEV_E    |
 | | bit  7 R/W  TX_2X_SLICE6_INT_DEV_E    |
 | | bit  6 R/W  TX_2X_SLICE5_INT_DEV_E    |
 | | bit  5 R/W  TX_2X_SLICE4_INT_DEV_E    |
 | | bit  4 R/W  TX_2X_SLICE3_INT_DEV_E    |
 | | bit  3 R/W  TX_2X_SLICE2_INT_DEV_E    |
 | | bit  2 R/W  TX_2X_SLICE1_INT_DEV_E    |
 | | bit  1 R/W  TX_2X_SLICE0_INT_DEV_E    |
 | | bit  0 R/W  CSU_INT_DEV_E             |
 | +---------------------------------------+
 | 
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_DEV_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_DEV_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_DEV_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_DEV_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_DEV_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_DEV_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_DEV_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_DEV_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_DEV_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_DEV_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_DEV_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_DEV_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_DEV_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_DEV_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_DEV_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_DEV_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_DEV_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_DEV_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_CSU_INT_DEV_E_set          ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_CSU_INT_DEV_E_get          ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x00000018 MIPS_INT_EN_REG |
 | +-------------------------------------+
 | | bit  9 R/W  TX_2X_SLICE8_INT_MIP_E  |
 | | bit  8 R/W  TX_2X_SLICE7_INT_MIP_E  |
 | | bit  7 R/W  TX_2X_SLICE6_INT_MIP_E  |
 | | bit  6 R/W  TX_2X_SLICE5_INT_MIP_E  |
 | | bit  5 R/W  TX_2X_SLICE4_INT_MIP_E  |
 | | bit  4 R/W  TX_2X_SLICE3_INT_MIP_E  |
 | | bit  3 R/W  TX_2X_SLICE2_INT_MIP_E  |
 | | bit  2 R/W  TX_2X_SLICE1_INT_MIP_E  |
 | | bit  1 R/W  TX_2X_SLICE0_INT_MIP_E  |
 | | bit  0 R/W  CSU_INT_MIP_E           |
 | +-------------------------------------+
 | 
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_MIP_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_MIP_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_MIP_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_MIP_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_MIP_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_MIP_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_MIP_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_MIP_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_MIP_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_MIP_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_MIP_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_MIP_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_MIP_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_MIP_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_MIP_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_MIP_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_MIP_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_MIP_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_CSU_INT_MIP_E_set          ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_CSU_INT_MIP_E_get          ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x0000001c PCIE_INT_EN_REG |
 | +-------------------------------------+
 | | bit  9 R/W  TX_2X_SLICE8_INT_PCI_E  |
 | | bit  8 R/W  TX_2X_SLICE7_INT_PCI_E  |
 | | bit  7 R/W  TX_2X_SLICE6_INT_PCI_E  |
 | | bit  6 R/W  TX_2X_SLICE5_INT_PCI_E  |
 | | bit  5 R/W  TX_2X_SLICE4_INT_PCI_E  |
 | | bit  4 R/W  TX_2X_SLICE3_INT_PCI_E  |
 | | bit  3 R/W  TX_2X_SLICE2_INT_PCI_E  |
 | | bit  2 R/W  TX_2X_SLICE1_INT_PCI_E  |
 | | bit  1 R/W  TX_2X_SLICE0_INT_PCI_E  |
 | | bit  0 R/W  CSU_INT_PCI_E           |
 | +-------------------------------------+
 | 
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_PCI_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_PCI_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_PCI_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_PCI_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_PCI_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_PCI_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_PCI_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_PCI_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_PCI_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_PCI_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_PCI_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_PCI_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_PCI_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_PCI_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_PCI_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_PCI_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_PCI_E_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_PCI_E_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   sfi51_tx_18x_serdes_glue_field_CSU_INT_PCI_E_set          ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_field_CSU_INT_PCI_E_get          ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x00000020 GLOBAL_INT_STAT_REG |
 | +-----------------------------------------+
 | | bit  9 R  TX_2X_SLICE8_INT_V            |
 | | bit  8 R  TX_2X_SLICE7_INT_V            |
 | | bit  7 R  TX_2X_SLICE6_INT_V            |
 | | bit  6 R  TX_2X_SLICE5_INT_V            |
 | | bit  5 R  TX_2X_SLICE4_INT_V            |
 | | bit  4 R  TX_2X_SLICE3_INT_V            |
 | | bit  3 R  TX_2X_SLICE2_INT_V            |
 | | bit  2 R  TX_2X_SLICE1_INT_V            |
 | | bit  1 R  TX_2X_SLICE0_INT_V            |
 | | bit  0 R  CSU_INT_V                     |
 | +-----------------------------------------+
 | 
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE8_INT_V_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE7_INT_V_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE6_INT_V_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE5_INT_V_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE4_INT_V_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE3_INT_V_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE2_INT_V_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE1_INT_V_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 sfi51_tx_18x_serdes_glue_field_TX_2X_SLICE0_INT_V_get ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 sfi51_tx_18x_serdes_glue_field_CSU_INT_V_get          ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SFI51_TX_18X_SERDES_GLUE_IO_H
#define _SFI51_TX_18X_SERDES_GLUE_IO_H

#include "sfi51_d8_tx_18x.h"
#include "sfi51_tx_18x_serdes_glue_regs.h"
#include "sfi51_tx_18x_serdes_glue_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sfi51_tx_18x_serdes_glue_buffer_init               ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void                 sfi51_tx_18x_serdes_glue_buffer_flush              ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr )
 | UINT32               sfi51_tx_18x_serdes_glue_reg_read                  ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 sfi51_tx_18x_serdes_glue_reg_write                 ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 sfi51_tx_18x_serdes_glue_field_set                 ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 sfi51_tx_18x_serdes_glue_action_on_write_field_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 sfi51_tx_18x_serdes_glue_burst_read                ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 sfi51_tx_18x_serdes_glue_burst_write               ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE sfi51_tx_18x_serdes_glue_poll                      ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_write            ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void   sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set        ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_reg_SW_RESET_CTRL_read             ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void   sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_write     ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void   sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_field_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_1_read      ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void   sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_write     ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void   sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_field_set ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_reg_LOOPTIMING_CFG_REG_2_read      ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void   sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_write        ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void   sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_field_set    ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_reg_RXS_CTRLLER_CFG_2_read         ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void   sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_write           ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void   sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set       ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read            ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void   sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_write        ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void   sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set    ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read         ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void   sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_write          ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void   sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set      ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read           ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void   sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_write          ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void   sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set      ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sfi51_tx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read           ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | UINT32 sfi51_tx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read       ( sfi51_tx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SFI51_TX_18X_SERDES_GLUE_IO_H */
