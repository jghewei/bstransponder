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
 *     and register accessor functions for the dcsu block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing dcsu io functions is:
 | {
 |     dcsu_buffer_t b_ptr[1];
 |     dcsu_buffer_init( b_ptr, h_ptr, dcsu_num );
 |     value = dcsu_field_<FIELD>_get( b_ptr, h_ptr, dcsu_num );
 |     dcsu_field_<FIELD>_set( b_ptr, h_ptr, dcsu_num , value );
 | ...
 |     dcsu_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = dcsu_field_<FIELD>_get( NULL, h_ptr, dcsu_num );
 | dcsu_field_<FIELD>_set( NULL, h_ptr, dcsu_num , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------+
 | | Register 0x000a0000 SW_RESET_REG |
 | +----------------------------------+
 | | bit  16 R    BOOT_MODE           |
 | | bit  0  R/W  SOFTWARE_RESET      |
 | +----------------------------------+
 | 
 | UINT32 dcsu_field_BOOT_MODE_get      ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_SOFTWARE_RESET_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_SOFTWARE_RESET_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | +----------------------------------+
 | | Register 0x000a0004 DCO_CTRL_REG |
 | +----------------------------------+
 | | bit  2 R/W  DIV_RSTB             |
 | | bit  1 R/W  ARSTB                |
 | | bit  0 R/W  ENB                  |
 | +----------------------------------+
 | 
 | void   dcsu_field_DIV_RSTB_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_DIV_RSTB_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_ARSTB_set    ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_ARSTB_get    ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_ENB_set      ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_ENB_get      ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | +---------------------------------------+
 | | Register 0x000a0008 DCSU_CLK_CTRL_REG |
 | +---------------------------------------+
 | | bit  7:6 R/W  DIVIDER_CLKSEL          |
 | | bit  5:4 R/W  DCO_CTL_CLKSEL          |
 | | bit  3   R/W  DIV4_ENB                |
 | | bit  2   R/W  DIV3_ENB                |
 | | bit  1   R/W  DIV2_ENB                |
 | | bit  0   R/W  DIV1_ENB                |
 | +---------------------------------------+
 | 
 | void   dcsu_field_DIVIDER_CLKSEL_set       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_DIVIDER_CLKSEL_get       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_DIVIDER_CLKSEL_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_DIVIDER_CLKSEL_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | void   dcsu_field_DCO_CTL_CLKSEL_set       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_DCO_CTL_CLKSEL_get       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_DCO_CTL_CLKSEL_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_DCO_CTL_CLKSEL_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | void   dcsu_field_DIV4_ENB_set             ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_DIV4_ENB_get             ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_DIV3_ENB_set             ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_DIV3_ENB_get             ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_DIV2_ENB_set             ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_DIV2_ENB_get             ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_DIV1_ENB_set             ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_DIV1_ENB_get             ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | +-----------------------------------------+
 | | Register 0x000a000c PRE_DIVIDER_CFG_REG |
 | +-----------------------------------------+
 | | bit  4:0 R/W  PRE_DIV_BY                |
 | +-----------------------------------------+
 | 
 | void   dcsu_field_PRE_DIV_BY_set       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_PRE_DIV_BY_get       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_PRE_DIV_BY_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_PRE_DIV_BY_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x000a0010 POST_DIV_CTRL |
 | +-----------------------------------+
 | | bit  31    R/W  POST_UPDATE_EN    |
 | | bit  15:12 R/W  SHIFT_CLK_SEL     |
 | | bit  11:8  R/W  AC_PULSE_REQ      |
 | | bit  7:4   R/W  CLK_STOP          |
 | | bit  3:0   R/W  CLK_BYPASS_EN     |
 | +-----------------------------------+
 | 
 | void   dcsu_field_POST_UPDATE_EN_set      ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_POST_UPDATE_EN_get      ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_SHIFT_CLK_SEL_set       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_SHIFT_CLK_SEL_get       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_SHIFT_CLK_SEL_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_SHIFT_CLK_SEL_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | void   dcsu_field_AC_PULSE_REQ_set        ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_AC_PULSE_REQ_get        ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_AC_PULSE_REQ_set  ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_AC_PULSE_REQ_get  ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | void   dcsu_field_CLK_STOP_set            ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_CLK_STOP_get            ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_CLK_STOP_set      ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_CLK_STOP_get      ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | void   dcsu_field_CLK_BYPASS_EN_set       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_CLK_BYPASS_EN_get       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_CLK_BYPASS_EN_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_CLK_BYPASS_EN_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x000a0014 POST_DIVIDER_CFG_REG |
 | +------------------------------------------+
 | | bit  31:28 R/W  POST_SYNC_EN             |
 | | bit  24:20 R/W  POST_SYNC_CNT            |
 | | bit  19:0  R/W  POST_DIV_BY              |
 | +------------------------------------------+
 | 
 | void   dcsu_field_POST_SYNC_EN_set        ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_POST_SYNC_EN_get        ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_POST_SYNC_EN_set  ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_POST_SYNC_EN_get  ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | void   dcsu_field_POST_SYNC_CNT_set       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_POST_SYNC_CNT_get       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_POST_SYNC_CNT_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_POST_SYNC_CNT_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | void   dcsu_field_POST_DIV_BY_set         ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_POST_DIV_BY_get         ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_POST_DIV_BY_set   ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_POST_DIV_BY_get   ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x000a0018 FB_CONFIG |
 | +-------------------------------+
 | | bit  31  R/W  FB_UPDATE_EN    |
 | | bit  7:0 R/W  FB_INT_DIV      |
 | +-------------------------------+
 | 
 | void   dcsu_field_FB_UPDATE_EN_set     ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_FB_UPDATE_EN_get     ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_FB_INT_DIV_set       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_FB_INT_DIV_get       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_FB_INT_DIV_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_FB_INT_DIV_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x000a001c FB_NUMERATOR |
 | +----------------------------------+
 | | bit  31:0 R/W  FB_NUMERATOR      |
 | +----------------------------------+
 | 
 | void   dcsu_field_FB_NUMERATOR_set       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_FB_NUMERATOR_get       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_FB_NUMERATOR_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_FB_NUMERATOR_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x000a0020 FB_DENOMINATOR |
 | +------------------------------------+
 | | bit  31:0 R/W  FB_DENOMINATOR      |
 | +------------------------------------+
 | 
 | void   dcsu_field_FB_DENOMINATOR_set       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_FB_DENOMINATOR_get       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_FB_DENOMINATOR_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_FB_DENOMINATOR_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x000a0024 MODULATION_CFG |
 | +------------------------------------+
 | | bit  31  R/W  M_EN                 |
 | | bit  7:0 R/W  M_INT_DIV            |
 | +------------------------------------+
 | 
 | void   dcsu_field_M_EN_set            ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_M_EN_get            ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_M_INT_DIV_set       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_M_INT_DIV_get       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_M_INT_DIV_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_M_INT_DIV_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x000a0028 M_NUMERATOR |
 | +---------------------------------+
 | | bit  31:0 R/W  M_NUMERATOR      |
 | +---------------------------------+
 | 
 | void   dcsu_field_M_NUMERATOR_set       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_M_NUMERATOR_get       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_M_NUMERATOR_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_M_NUMERATOR_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x000a002c M_STEPSIZE |
 | +--------------------------------+
 | | bit  31:0 R/W  M_STEPSIZE      |
 | +--------------------------------+
 | 
 | void   dcsu_field_M_STEPSIZE_set       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_M_STEPSIZE_get       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_range_M_STEPSIZE_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 dcsu_field_range_M_STEPSIZE_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x000a0030 DCSU_INT_VAL_REG |
 | +--------------------------------------+
 | | bit  2 R  ERR_FAST_V                 |
 | | bit  1 R  ERR_SLOW_V                 |
 | | bit  0 R  LOCK_V                     |
 | +--------------------------------------+
 | 
 | UINT32               dcsu_field_ERR_FAST_V_get  ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | PMC_POLL_RETURN_TYPE dcsu_field_ERR_FAST_V_poll ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               dcsu_field_ERR_SLOW_V_get  ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | PMC_POLL_RETURN_TYPE dcsu_field_ERR_SLOW_V_poll ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               dcsu_field_LOCK_V_get      ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | PMC_POLL_RETURN_TYPE dcsu_field_LOCK_V_poll     ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------+
 | | Register 0x000a0034 DCSU_INTS |
 | +-------------------------------+
 | | bit  2 R/W  ERR_FAST_I        |
 | | bit  1 R/W  ERR_SLOW_I        |
 | | bit  0 R/W  LOCK_I            |
 | +-------------------------------+
 | 
 | void                 dcsu_field_ERR_FAST_I_set_to_clear ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32               dcsu_field_ERR_FAST_I_get          ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | PMC_POLL_RETURN_TYPE dcsu_field_ERR_FAST_I_poll         ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 dcsu_field_ERR_SLOW_I_set_to_clear ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32               dcsu_field_ERR_SLOW_I_get          ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | PMC_POLL_RETURN_TYPE dcsu_field_ERR_SLOW_I_poll         ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 dcsu_field_LOCK_I_set_to_clear     ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32               dcsu_field_LOCK_I_get              ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | PMC_POLL_RETURN_TYPE dcsu_field_LOCK_I_poll             ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------+
 | | Register 0x000a0038 INT_EN |
 | +----------------------------+
 | | bit  2 R/W  ERR_FAST_E     |
 | | bit  1 R/W  ERR_SLOW_E     |
 | | bit  0 R/W  LOCK_E         |
 | +----------------------------+
 | 
 | void   dcsu_field_ERR_FAST_E_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_ERR_FAST_E_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_ERR_SLOW_E_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_ERR_SLOW_E_get ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void   dcsu_field_LOCK_E_set     ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | UINT32 dcsu_field_LOCK_E_get     ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _DCSU_IO_H
#define _DCSU_IO_H

#include "digi_api.h"
#include "dcsu_regs.h"
#include "dcsu_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 dcsu_buffer_init               ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | void                 dcsu_buffer_flush              ( dcsu_buffer_t *b_ptr )
 | UINT32               dcsu_reg_read                  ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mem_type, UINT32 reg )
 | void                 dcsu_reg_write                 ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 dcsu_field_set                 ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 dcsu_action_on_write_field_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 dcsu_burst_read                ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 dcsu_burst_write               ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE dcsu_poll                      ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 dcsu_reg_DCO_CTRL_REG_write                  ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | void                 dcsu_reg_DCO_CTRL_REG_field_set              ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcsu_reg_DCO_CTRL_REG_read                   ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | void                 dcsu_reg_DCSU_CLK_CTRL_REG_write             ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | void                 dcsu_reg_DCSU_CLK_CTRL_REG_field_set         ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcsu_reg_DCSU_CLK_CTRL_REG_read              ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | void                 dcsu_reg_PRE_DIVIDER_CFG_REG_write           ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | void                 dcsu_reg_PRE_DIVIDER_CFG_REG_field_set       ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcsu_reg_PRE_DIVIDER_CFG_REG_read            ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | void                 dcsu_reg_POST_DIV_CTRL_write                 ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | void                 dcsu_reg_POST_DIV_CTRL_field_set             ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcsu_reg_POST_DIV_CTRL_read                  ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | void                 dcsu_reg_POST_DIVIDER_CFG_REG_write          ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | void                 dcsu_reg_POST_DIVIDER_CFG_REG_field_set      ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcsu_reg_POST_DIVIDER_CFG_REG_read           ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | void                 dcsu_reg_FB_CONFIG_write                     ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | void                 dcsu_reg_FB_CONFIG_field_set                 ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcsu_reg_FB_CONFIG_read                      ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | void                 dcsu_reg_FB_NUMERATOR_write                  ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | void                 dcsu_reg_FB_NUMERATOR_field_set              ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcsu_reg_FB_NUMERATOR_read                   ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | void                 dcsu_reg_FB_DENOMINATOR_write                ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | void                 dcsu_reg_FB_DENOMINATOR_field_set            ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcsu_reg_FB_DENOMINATOR_read                 ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | void                 dcsu_reg_MODULATION_CFG_write                ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | void                 dcsu_reg_MODULATION_CFG_field_set            ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcsu_reg_MODULATION_CFG_read                 ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | void                 dcsu_reg_M_NUMERATOR_write                   ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | void                 dcsu_reg_M_NUMERATOR_field_set               ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcsu_reg_M_NUMERATOR_read                    ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | void                 dcsu_reg_M_STEPSIZE_write                    ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | void                 dcsu_reg_M_STEPSIZE_field_set                ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcsu_reg_M_STEPSIZE_read                     ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | void                 dcsu_reg_DCSU_INTS_write                     ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | void                 dcsu_reg_DCSU_INTS_action_on_write_field_set ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcsu_reg_DCSU_INTS_read                      ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | PMC_POLL_RETURN_TYPE dcsu_reg_DCSU_INTS_poll                      ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 dcsu_reg_INT_EN_write                        ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | void                 dcsu_reg_INT_EN_field_set                    ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcsu_reg_INT_EN_read                         ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 | UINT32               dcsu_reg_DCSU_INT_VAL_REG_read               ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | PMC_POLL_RETURN_TYPE dcsu_reg_DCSU_INT_VAL_REG_poll               ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 dcsu_reg_SW_RESET_REG_write                  ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 value )
 | void                 dcsu_reg_SW_RESET_REG_field_set              ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               dcsu_reg_SW_RESET_REG_read                   ( dcsu_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 dcsu_num )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _DCSU_IO_H */
