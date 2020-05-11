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
 *     and register accessor functions for the sfi51_rx_18x_serdes_glue block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing sfi51_rx_18x_serdes_glue io functions is:
 | {
 |     sfi51_rx_18x_serdes_glue_buffer_t b_ptr[1];
 |     sfi51_rx_18x_serdes_glue_buffer_init( b_ptr, h_ptr );
 |     value = sfi51_rx_18x_serdes_glue_field_<FIELD>_get( b_ptr, h_ptr );
 |     sfi51_rx_18x_serdes_glue_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     sfi51_rx_18x_serdes_glue_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = sfi51_rx_18x_serdes_glue_field_<FIELD>_get( NULL, h_ptr );
 | sfi51_rx_18x_serdes_glue_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +------------------------------------+
 | | Register 0x00000000 SW_RESET_CTRL  |
 | +------------------------------------+
 | | bit  5 R/W  EFUSE_INT_RSTB         |
 | | bit  4 R/W  REFCLK_DIVIDER_RSTB    |
 | | bit  3 R/W  EFUSE_CTRL_RSTB        |
 | | bit  2 R/W  HRCLKDIV4_DIVIDER_RSTB |
 | | bit  1 R/W  DIGITAL_RSTB           |
 | | bit  0 R/W  SW_RESET               |
 | +------------------------------------+
 | 
 | void   sfi51_rx_18x_serdes_glue_field_EFUSE_INT_RSTB_set         ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_EFUSE_INT_RSTB_get         ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_REFCLK_DIVIDER_RSTB_set    ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_REFCLK_DIVIDER_RSTB_get    ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_EFUSE_CTRL_RSTB_set        ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_EFUSE_CTRL_RSTB_get        ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIVIDER_RSTB_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIVIDER_RSTB_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_DIGITAL_RSTB_set           ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_DIGITAL_RSTB_get           ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_SW_RESET_set               ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_SW_RESET_get               ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | +-----------------------------+
 | | Register 0x00000004 INT_REG |
 | +-----------------------------+
 | | bit  1 R/W  TX_SFI_LOS_I    |
 | | bit  0 R/W  FUSE_FAIL_OUT_I |
 | +-----------------------------+
 | 
 | void                 sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_I_set_to_clear    ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32               sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_I_get             ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_I_poll            ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_I_set_to_clear ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32               sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_I_get          ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_I_poll         ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------+
 | | Register 0x00000008 INT_EN_REG |
 | +--------------------------------+
 | | bit  1 R/W  TX_SFI_LOS_E       |
 | | bit  0 R/W  FUSE_FAIL_OUT_E    |
 | +--------------------------------+
 | 
 | void   sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_E_set    ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_E_get    ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | +----------------------------------+
 | | Register 0x0000000c INT_STAT_REG |
 | +----------------------------------+
 | | bit  1 R  TX_SFI_LOS_V           |
 | | bit  0 R  FUSE_FAIL_OUT_V        |
 | +----------------------------------+
 | 
 | UINT32               sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_V_get     ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_V_poll    ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_V_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_field_FUSE_FAIL_OUT_V_poll ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x00000010 HRCLKDIV4_DIV_CFG |
 | +---------------------------------------+
 | | bit  0 R/W  HRCLKDIV4_DIV_EN          |
 | +---------------------------------------+
 | 
 | void   sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIV_EN_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_HRCLKDIV4_DIV_EN_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000014 EFUSE_REFCLK_CFG |
 | +--------------------------------------+
 | | bit  0 R/W  EFUSE_REFCLK_DIV_EN      |
 | +--------------------------------------+
 | 
 | void   sfi51_rx_18x_serdes_glue_field_EFUSE_REFCLK_DIV_EN_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_EFUSE_REFCLK_DIV_EN_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000018 EFUSE_INT_CFG_REG |
 | +---------------------------------------+
 | | bit  29:24 R/W  RAMS_ADDR             |
 | | bit  23:16 R    RAMS_DATA_RD          |
 | | bit  15:8  R/W  RAMS_DATA_WR          |
 | | bit  4     R/W  RAMS_RD_EN            |
 | | bit  3     R/W  RAMS_WR_EN            |
 | | bit  2     R/W  PCLK_EN               |
 | | bit  1:0   R/W  RAM_EXT_INT_SEL       |
 | +---------------------------------------+
 | 
 | void   sfi51_rx_18x_serdes_glue_field_RAMS_ADDR_set             ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_ADDR_get             ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_range_RAMS_ADDR_set       ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_range_RAMS_ADDR_get       ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sfi51_rx_18x_serdes_glue_field_RAMS_DATA_WR_set          ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_DATA_WR_get          ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_WR_set    ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_WR_get    ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sfi51_rx_18x_serdes_glue_field_RAMS_RD_EN_set            ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_RD_EN_get            ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RAMS_WR_EN_set            ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_WR_EN_get            ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_PCLK_EN_set               ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_PCLK_EN_get               ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RAM_EXT_INT_SEL_set       ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RAM_EXT_INT_SEL_get       ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_range_RAM_EXT_INT_SEL_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_range_RAM_EXT_INT_SEL_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RAMS_DATA_RD_get          ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_range_RAMS_DATA_RD_get    ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x0000001c GLUE_LOGIC_CFG |
 | +------------------------------------+
 | | bit  7   R/W  TX_SFI_LOS_POL       |
 | | bit  6   R/W  TX_SFI_OOA_LOF_POL   |
 | | bit  5   R/W  TXDCK_SLICE_EN       |
 | | bit  2:0 R/W  REFCLK_SEL           |
 | +------------------------------------+
 | 
 | void   sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_POL_set     ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_TX_SFI_LOS_POL_get     ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_TX_SFI_OOA_LOF_POL_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_TX_SFI_OOA_LOF_POL_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_TXDCK_SLICE_EN_set     ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_TXDCK_SLICE_EN_get     ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_REFCLK_SEL_set         ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_REFCLK_SEL_get         ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_range_REFCLK_SEL_set   ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_range_REFCLK_SEL_get   ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00000020 LOS_STAT_REG |
 | +----------------------------------+
 | | bit  17 R  LOS_17                |
 | | bit  16 R  LOS_16                |
 | | bit  15 R  LOS_15                |
 | | bit  14 R  LOS_14                |
 | | bit  13 R  LOS_13                |
 | | bit  12 R  LOS_12                |
 | | bit  11 R  LOS_11                |
 | | bit  10 R  LOS_10                |
 | | bit  9  R  LOS_9                 |
 | | bit  8  R  LOS_8                 |
 | | bit  7  R  LOS_7                 |
 | | bit  6  R  LOS_6                 |
 | | bit  5  R  LOS_5                 |
 | | bit  4  R  LOS_4                 |
 | | bit  3  R  LOS_3                 |
 | | bit  2  R  LOS_2                 |
 | | bit  1  R  LOS_1                 |
 | | bit  0  R  LOS_0                 |
 | +----------------------------------+
 | 
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_17_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_16_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_15_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_14_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_13_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_12_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_11_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_10_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_9_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_8_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_7_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_6_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_5_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_4_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_3_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_2_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_1_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_LOS_0_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000024 DEVICE_INT_EN_REG |
 | +---------------------------------------+
 | | bit  10 R/W  RX_18X_GLUE_INT_DEV_E    |
 | | bit  9  R/W  RX_2X_SLICE8_INT_DEV_E   |
 | | bit  8  R/W  RX_2X_SLICE7_INT_DEV_E   |
 | | bit  7  R/W  RX_2X_SLICE6_INT_DEV_E   |
 | | bit  6  R/W  RX_2X_SLICE5_INT_DEV_E   |
 | | bit  5  R/W  RX_2X_SLICE4_INT_DEV_E   |
 | | bit  4  R/W  RX_2X_SLICE3_INT_DEV_E   |
 | | bit  3  R/W  RX_2X_SLICE2_INT_DEV_E   |
 | | bit  2  R/W  RX_2X_SLICE1_INT_DEV_E   |
 | | bit  1  R/W  RX_2X_SLICE0_INT_DEV_E   |
 | | bit  0  R/W  CSU_INT_DEV_E            |
 | +---------------------------------------+
 | 
 | void   sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_DEV_E_set  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_DEV_E_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_DEV_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_DEV_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_DEV_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_DEV_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_DEV_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_DEV_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_DEV_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_DEV_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_DEV_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_DEV_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_DEV_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_DEV_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_DEV_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_DEV_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_DEV_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_DEV_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_DEV_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_DEV_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_CSU_INT_DEV_E_set          ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_CSU_INT_DEV_E_get          ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x00000028 MIPS_INT_EN_REG |
 | +-------------------------------------+
 | | bit  10 R/W  RX_18X_GLUE_INT_MIP_E  |
 | | bit  9  R/W  RX_2X_SLICE8_INT_MIP_E |
 | | bit  8  R/W  RX_2X_SLICE7_INT_MIP_E |
 | | bit  7  R/W  RX_2X_SLICE6_INT_MIP_E |
 | | bit  6  R/W  RX_2X_SLICE5_INT_MIP_E |
 | | bit  5  R/W  RX_2X_SLICE4_INT_MIP_E |
 | | bit  4  R/W  RX_2X_SLICE3_INT_MIP_E |
 | | bit  3  R/W  RX_2X_SLICE2_INT_MIP_E |
 | | bit  2  R/W  RX_2X_SLICE1_INT_MIP_E |
 | | bit  1  R/W  RX_2X_SLICE0_INT_MIP_E |
 | | bit  0  R/W  CSU_INT_MIP_E          |
 | +-------------------------------------+
 | 
 | void   sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_MIP_E_set  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_MIP_E_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_MIP_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_MIP_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_MIP_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_MIP_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_MIP_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_MIP_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_MIP_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_MIP_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_MIP_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_MIP_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_MIP_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_MIP_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_MIP_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_MIP_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_MIP_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_MIP_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_MIP_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_MIP_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_CSU_INT_MIP_E_set          ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_CSU_INT_MIP_E_get          ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x0000002c PCIE_INT_EN_REG |
 | +-------------------------------------+
 | | bit  10 R/W  RX_18X_GLUE_INT_PCI_E  |
 | | bit  9  R/W  RX_2X_SLICE8_INT_PCI_E |
 | | bit  8  R/W  RX_2X_SLICE7_INT_PCI_E |
 | | bit  7  R/W  RX_2X_SLICE6_INT_PCI_E |
 | | bit  6  R/W  RX_2X_SLICE5_INT_PCI_E |
 | | bit  5  R/W  RX_2X_SLICE4_INT_PCI_E |
 | | bit  4  R/W  RX_2X_SLICE3_INT_PCI_E |
 | | bit  3  R/W  RX_2X_SLICE2_INT_PCI_E |
 | | bit  2  R/W  RX_2X_SLICE1_INT_PCI_E |
 | | bit  1  R/W  RX_2X_SLICE0_INT_PCI_E |
 | | bit  0  R/W  CSU_INT_PCI_E          |
 | +-------------------------------------+
 | 
 | void   sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_PCI_E_set  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_PCI_E_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_PCI_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_PCI_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_PCI_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_PCI_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_PCI_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_PCI_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_PCI_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_PCI_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_PCI_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_PCI_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_PCI_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_PCI_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_PCI_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_PCI_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_PCI_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_PCI_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_PCI_E_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_PCI_E_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void   sfi51_rx_18x_serdes_glue_field_CSU_INT_PCI_E_set          ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 sfi51_rx_18x_serdes_glue_field_CSU_INT_PCI_E_get          ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x00000030 GLOBAL_INT_STAT_REG |
 | +-----------------------------------------+
 | | bit  10 R  RX_18X_GLUE_INT_V            |
 | | bit  9  R  RX_2X_SLICE8_INT_V           |
 | | bit  8  R  RX_2X_SLICE7_INT_V           |
 | | bit  7  R  RX_2X_SLICE6_INT_V           |
 | | bit  6  R  RX_2X_SLICE5_INT_V           |
 | | bit  5  R  RX_2X_SLICE4_INT_V           |
 | | bit  4  R  RX_2X_SLICE3_INT_V           |
 | | bit  3  R  RX_2X_SLICE2_INT_V           |
 | | bit  2  R  RX_2X_SLICE1_INT_V           |
 | | bit  1  R  RX_2X_SLICE0_INT_V           |
 | | bit  0  R  CSU_INT_V                    |
 | +-----------------------------------------+
 | 
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_18X_GLUE_INT_V_get  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE8_INT_V_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE7_INT_V_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE6_INT_V_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE5_INT_V_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE4_INT_V_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE3_INT_V_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE2_INT_V_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE1_INT_V_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_RX_2X_SLICE0_INT_V_get ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | UINT32 sfi51_rx_18x_serdes_glue_field_CSU_INT_V_get          ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SFI51_RX_18X_SERDES_GLUE_IO_H
#define _SFI51_RX_18X_SERDES_GLUE_IO_H

#include "sfi51_d8_rx_18x.h"
#include "sfi51_rx_18x_serdes_glue_regs.h"
#include "sfi51_rx_18x_serdes_glue_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sfi51_rx_18x_serdes_glue_buffer_init               ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | void                 sfi51_rx_18x_serdes_glue_buffer_flush              ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr )
 | UINT32               sfi51_rx_18x_serdes_glue_reg_read                  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 sfi51_rx_18x_serdes_glue_reg_write                 ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 sfi51_rx_18x_serdes_glue_field_set                 ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 sfi51_rx_18x_serdes_glue_action_on_write_field_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 sfi51_rx_18x_serdes_glue_burst_read                ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 sfi51_rx_18x_serdes_glue_burst_write               ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_poll                      ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_write               ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | void                 sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_field_set           ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_rx_18x_serdes_glue_reg_SW_RESET_CTRL_read                ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | void                 sfi51_rx_18x_serdes_glue_reg_HRCLKDIV4_DIV_CFG_write           ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | void                 sfi51_rx_18x_serdes_glue_reg_HRCLKDIV4_DIV_CFG_field_set       ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_rx_18x_serdes_glue_reg_HRCLKDIV4_DIV_CFG_read            ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | void                 sfi51_rx_18x_serdes_glue_reg_EFUSE_REFCLK_CFG_write            ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | void                 sfi51_rx_18x_serdes_glue_reg_EFUSE_REFCLK_CFG_field_set        ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_rx_18x_serdes_glue_reg_EFUSE_REFCLK_CFG_read             ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | void                 sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_write           ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | void                 sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_field_set       ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_rx_18x_serdes_glue_reg_EFUSE_INT_CFG_REG_read            ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | void                 sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_write              ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | void                 sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_field_set          ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_rx_18x_serdes_glue_reg_GLUE_LOGIC_CFG_read               ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | void                 sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_write           ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | void                 sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_field_set       ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_rx_18x_serdes_glue_reg_DEVICE_INT_EN_REG_read            ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | void                 sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_write             ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | void                 sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_field_set         ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_rx_18x_serdes_glue_reg_MIPS_INT_EN_REG_read              ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | void                 sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_write             ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | void                 sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_field_set         ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_rx_18x_serdes_glue_reg_PCIE_INT_EN_REG_read              ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | void                 sfi51_rx_18x_serdes_glue_reg_INT_REG_write                     ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | void                 sfi51_rx_18x_serdes_glue_reg_INT_REG_action_on_write_field_set ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_rx_18x_serdes_glue_reg_INT_REG_read                      ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_reg_INT_REG_poll                      ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_write                  ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 value )
 | void                 sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_field_set              ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_rx_18x_serdes_glue_reg_INT_EN_REG_read                   ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | UINT32               sfi51_rx_18x_serdes_glue_reg_INT_STAT_REG_read                 ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sfi51_rx_18x_serdes_glue_reg_INT_STAT_REG_poll                 ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               sfi51_rx_18x_serdes_glue_reg_LOS_STAT_REG_read                 ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 | UINT32               sfi51_rx_18x_serdes_glue_reg_GLOBAL_INT_STAT_REG_read          ( sfi51_rx_18x_serdes_glue_buffer_t *b_ptr, sfi51_d8_rx_18x_handle_t *h_ptr )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SFI51_RX_18X_SERDES_GLUE_IO_H */
