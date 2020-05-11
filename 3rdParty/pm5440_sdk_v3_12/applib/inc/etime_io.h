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
 *     and register accessor functions for the etime block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing etime io functions is:
 | {
 |     etime_buffer_t b_ptr[1];
 |     etime_buffer_init( b_ptr, h_ptr );
 |     value = etime_field_<FIELD>_get( b_ptr, h_ptr );
 |     etime_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     etime_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = etime_field_<FIELD>_get( NULL, h_ptr );
 | etime_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------+
 | | Register 0x00000000 REFLLRR     |
 | +---------------------------------+
 | | bit  11 R/W  REFL_LINK_RESET_11 |
 | | bit  10 R/W  REFL_LINK_RESET_10 |
 | | bit  9  R/W  REFL_LINK_RESET_9  |
 | | bit  8  R/W  REFL_LINK_RESET_8  |
 | | bit  7  R/W  REFL_LINK_RESET_7  |
 | | bit  6  R/W  REFL_LINK_RESET_6  |
 | | bit  5  R/W  REFL_LINK_RESET_5  |
 | | bit  4  R/W  REFL_LINK_RESET_4  |
 | | bit  3  R/W  REFL_LINK_RESET_3  |
 | | bit  2  R/W  REFL_LINK_RESET_2  |
 | | bit  1  R/W  REFL_LINK_RESET_1  |
 | | bit  0  R/W  REFL_LINK_RESET_0  |
 | +---------------------------------+
 | 
 | void   etime_field_REFL_LINK_RESET_11_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_LINK_RESET_11_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_LINK_RESET_10_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_LINK_RESET_10_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_LINK_RESET_9_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_LINK_RESET_9_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_LINK_RESET_8_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_LINK_RESET_8_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_LINK_RESET_7_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_LINK_RESET_7_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_LINK_RESET_6_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_LINK_RESET_6_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_LINK_RESET_5_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_LINK_RESET_5_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_LINK_RESET_4_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_LINK_RESET_4_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_LINK_RESET_3_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_LINK_RESET_3_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_LINK_RESET_2_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_LINK_RESET_2_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_LINK_RESET_1_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_LINK_RESET_1_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_LINK_RESET_0_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_LINK_RESET_0_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000004 INT_BEHAV_CTRL |
 | +------------------------------------+
 | | bit  1 R/W  WCI_MODE_CONTROL       |
 | | bit  0 R/W  CLR_INT_CONTROL        |
 | +------------------------------------+
 | 
 | void   etime_field_WCI_MODE_CONTROL_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_WCI_MODE_CONTROL_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_CLR_INT_CONTROL_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CLR_INT_CONTROL_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000008 RAM_BIST_CTRL  |
 | +------------------------------------+
 | | bit  4   R/W  RAM_LOWPOWER_REG_BIT |
 | | bit  3   R/W  PG_OVERRIDE          |
 | | bit  2   R/W  ECC_BYPASS           |
 | | bit  1:0 R/W  RAM_ERR_INJECT       |
 | +------------------------------------+
 | 
 | void   etime_field_RAM_LOWPOWER_REG_BIT_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_RAM_LOWPOWER_REG_BIT_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_PG_OVERRIDE_set          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_PG_OVERRIDE_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_ECC_BYPASS_set           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_ECC_BYPASS_get           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_RAM_ERR_INJECT_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_RAM_ERR_INJECT_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_RAM_ERR_INJECT_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_RAM_ERR_INJECT_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0000000c SB_ECC_ERR_INT_REG |
 | +----------------------------------------+
 | | bit  16:0 R/W  RAM_BIT_SB_ECC_INT_I    |
 | +----------------------------------------+
 | 
 | void                 etime_field_RAM_BIT_SB_ECC_INT_I_set_to_clear       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_RAM_BIT_SB_ECC_INT_I_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void                 etime_field_range_RAM_BIT_SB_ECC_INT_I_set_to_clear ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etime_field_range_RAM_BIT_SB_ECC_INT_I_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_RAM_BIT_SB_ECC_INT_I_poll         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_RAM_BIT_SB_ECC_INT_I_poll               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x00000010 MB_ECC_ERR_INT_REG |
 | +----------------------------------------+
 | | bit  16:0 R/W  RAM_BIT_MB_ECC_INT_I    |
 | +----------------------------------------+
 | 
 | void                 etime_field_RAM_BIT_MB_ECC_INT_I_set_to_clear       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_RAM_BIT_MB_ECC_INT_I_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void                 etime_field_range_RAM_BIT_MB_ECC_INT_I_set_to_clear ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etime_field_range_RAM_BIT_MB_ECC_INT_I_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_RAM_BIT_MB_ECC_INT_I_poll         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_RAM_BIT_MB_ECC_INT_I_poll               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x00000014 REFL_OFLOW_ERR_INT_REG |
 | +--------------------------------------------+
 | | bit  11:0 R/W  EXT_RAM_OFLOW_INT_I         |
 | +--------------------------------------------+
 | 
 | void                 etime_field_EXT_RAM_OFLOW_INT_I_set_to_clear       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_EXT_RAM_OFLOW_INT_I_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void                 etime_field_range_EXT_RAM_OFLOW_INT_I_set_to_clear ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etime_field_range_EXT_RAM_OFLOW_INT_I_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_EXT_RAM_OFLOW_INT_I_poll         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_EXT_RAM_OFLOW_INT_I_poll               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00000018 TS_DIS_INT_REG  |
 | +-------------------------------------+
 | | bit  23:12 R/W  TS_RAM_WR_DIS_INT_I |
 | | bit  11:0  R/W  TS_RAM_RD_DIS_INT_I |
 | +-------------------------------------+
 | 
 | void                 etime_field_TS_RAM_WR_DIS_INT_I_set_to_clear       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_TS_RAM_WR_DIS_INT_I_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void                 etime_field_range_TS_RAM_WR_DIS_INT_I_set_to_clear ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etime_field_range_TS_RAM_WR_DIS_INT_I_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_TS_RAM_WR_DIS_INT_I_poll         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_TS_RAM_WR_DIS_INT_I_poll               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_TS_RAM_RD_DIS_INT_I_set_to_clear       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_TS_RAM_RD_DIS_INT_I_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void                 etime_field_range_TS_RAM_RD_DIS_INT_I_set_to_clear ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etime_field_range_TS_RAM_RD_DIS_INT_I_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_TS_RAM_RD_DIS_INT_I_poll         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_TS_RAM_RD_DIS_INT_I_poll               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x0000001c PACKET_DIS_INT_REG  |
 | +-----------------------------------------+
 | | bit  23:12 R/W  PACKET_RAM_WR_DIS_INT_I |
 | | bit  11:0  R/W  PACKET_RAM_RD_DIS_INT_I |
 | +-----------------------------------------+
 | 
 | void                 etime_field_PACKET_RAM_WR_DIS_INT_I_set_to_clear       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_PACKET_RAM_WR_DIS_INT_I_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void                 etime_field_range_PACKET_RAM_WR_DIS_INT_I_set_to_clear ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etime_field_range_PACKET_RAM_WR_DIS_INT_I_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_PACKET_RAM_WR_DIS_INT_I_poll         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_PACKET_RAM_WR_DIS_INT_I_poll               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_PACKET_RAM_RD_DIS_INT_I_set_to_clear       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_PACKET_RAM_RD_DIS_INT_I_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void                 etime_field_range_PACKET_RAM_RD_DIS_INT_I_set_to_clear ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etime_field_range_PACKET_RAM_RD_DIS_INT_I_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_PACKET_RAM_RD_DIS_INT_I_poll         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_PACKET_RAM_RD_DIS_INT_I_poll               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x00000020 PACKET_RAM_ERR_INT_REG |
 | +--------------------------------------------+
 | | bit  11:0 R/W  PACKET_RAM_OSIZE_INT_I      |
 | +--------------------------------------------+
 | 
 | void                 etime_field_PACKET_RAM_OSIZE_INT_I_set_to_clear       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_PACKET_RAM_OSIZE_INT_I_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void                 etime_field_range_PACKET_RAM_OSIZE_INT_I_set_to_clear ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etime_field_range_PACKET_RAM_OSIZE_INT_I_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_PACKET_RAM_OSIZE_INT_I_poll         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_PACKET_RAM_OSIZE_INT_I_poll               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x00000024 EXT_DESYNC_ERR_INT_REG |
 | +--------------------------------------------+
 | | bit  11:0 R/W  EXT_DESYNC_INT_I            |
 | +--------------------------------------------+
 | 
 | void                 etime_field_EXT_DESYNC_INT_I_set_to_clear       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_EXT_DESYNC_INT_I_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void                 etime_field_range_EXT_DESYNC_INT_I_set_to_clear ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etime_field_range_EXT_DESYNC_INT_I_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_EXT_DESYNC_INT_I_poll         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_EXT_DESYNC_INT_I_poll               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------+
 | | Register 0x00000028 GP_INT_REG              |
 | +---------------------------------------------+
 | | bit  27 R/W  ILLEGAL_1STEP_INT_I            |
 | | bit  26 R/W  LINE_REF_CPTR_EVNT_I           |
 | | bit  25 R/W  IRIGO_TS_CPTR_EVNT_I           |
 | | bit  24 R/W  IRIGO_ONE_PPSO_REALIGN_I       |
 | | bit  23 R/W  IRIGO_TX_DAT_VAL_I             |
 | | bit  22 R/W  PTP_DCSU_TSTMPR_CPTR_EVNT_I    |
 | | bit  21 R/W  IRIGI_TS_CPTR_EVNT_I           |
 | | bit  20 R/W  IRIGI_RX_DAT_VAL_I             |
 | | bit  19 R/W  IRIGI_REALIGN_I                |
 | | bit  18 R/W  IRIGI_P0PR_MISSING_I           |
 | | bit  17 R/W  IRIGI_IDLE_I                   |
 | | bit  16 R/W  IRIGI_ONE_PPSO_REALIGN_I       |
 | | bit  15 R/W  IRIGI_PWM_DEC_ERR_I            |
 | | bit  14 R/W  TOD_CFC_OFFS_TS_CNTR_UPDATED_I |
 | | bit  13 R/W  TS_INTERPOLATE_CNTR_ERR_I      |
 | | bit  12 R/W  TS_TOD_SIGN_OVRFL_I            |
 | | bit  11 R/W  CFC_OVRFL_I                    |
 | | bit  10 R/W  TOD_TS_OVRFL_I                 |
 | | bit  9  R/W  FREE_RUN_CFC_OVRFL_I           |
 | | bit  8  R/W  FREE_RUN_TS_OVRFL_I            |
 | | bit  7  R/W  TOD_INT_1_I                    |
 | | bit  6  R/W  TOD_INT_2_I                    |
 | | bit  5  R/W  FREE_RUN_INT_1_I               |
 | | bit  4  R/W  FREE_RUN_INT_2_I               |
 | | bit  3  R/W  CC_INT_1_I                     |
 | | bit  2  R/W  CC_INT_2_I                     |
 | | bit  1  R/W  CC_INT_3_I                     |
 | | bit  0  R/W  CC_INT_4_I                     |
 | +---------------------------------------------+
 | 
 | void                 etime_field_ILLEGAL_1STEP_INT_I_set_to_clear            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_ILLEGAL_1STEP_INT_I_get                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_ILLEGAL_1STEP_INT_I_poll                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_LINE_REF_CPTR_EVNT_I_set_to_clear           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_LINE_REF_CPTR_EVNT_I_get                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_LINE_REF_CPTR_EVNT_I_poll                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_IRIGO_TS_CPTR_EVNT_I_set_to_clear           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_IRIGO_TS_CPTR_EVNT_I_get                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGO_TS_CPTR_EVNT_I_poll                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_IRIGO_ONE_PPSO_REALIGN_I_set_to_clear       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_IRIGO_ONE_PPSO_REALIGN_I_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGO_ONE_PPSO_REALIGN_I_poll               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_IRIGO_TX_DAT_VAL_I_set_to_clear             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_IRIGO_TX_DAT_VAL_I_get                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGO_TX_DAT_VAL_I_poll                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_PTP_DCSU_TSTMPR_CPTR_EVNT_I_set_to_clear    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_PTP_DCSU_TSTMPR_CPTR_EVNT_I_get             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_PTP_DCSU_TSTMPR_CPTR_EVNT_I_poll            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_IRIGI_TS_CPTR_EVNT_I_set_to_clear           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_IRIGI_TS_CPTR_EVNT_I_get                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGI_TS_CPTR_EVNT_I_poll                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_IRIGI_RX_DAT_VAL_I_set_to_clear             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_IRIGI_RX_DAT_VAL_I_get                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGI_RX_DAT_VAL_I_poll                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_IRIGI_REALIGN_I_set_to_clear                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_IRIGI_REALIGN_I_get                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGI_REALIGN_I_poll                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_IRIGI_P0PR_MISSING_I_set_to_clear           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_IRIGI_P0PR_MISSING_I_get                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGI_P0PR_MISSING_I_poll                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_IRIGI_IDLE_I_set_to_clear                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_IRIGI_IDLE_I_get                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGI_IDLE_I_poll                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_IRIGI_ONE_PPSO_REALIGN_I_set_to_clear       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_IRIGI_ONE_PPSO_REALIGN_I_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGI_ONE_PPSO_REALIGN_I_poll               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_IRIGI_PWM_DEC_ERR_I_set_to_clear            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_IRIGI_PWM_DEC_ERR_I_get                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGI_PWM_DEC_ERR_I_poll                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_TOD_CFC_OFFS_TS_CNTR_UPDATED_I_set_to_clear ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_TOD_CFC_OFFS_TS_CNTR_UPDATED_I_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_TOD_CFC_OFFS_TS_CNTR_UPDATED_I_poll         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_TS_INTERPOLATE_CNTR_ERR_I_set_to_clear      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_TS_INTERPOLATE_CNTR_ERR_I_get               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_TS_INTERPOLATE_CNTR_ERR_I_poll              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_TS_TOD_SIGN_OVRFL_I_set_to_clear            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_TS_TOD_SIGN_OVRFL_I_get                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_TS_TOD_SIGN_OVRFL_I_poll                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_CFC_OVRFL_I_set_to_clear                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_CFC_OVRFL_I_get                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_CFC_OVRFL_I_poll                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_TOD_TS_OVRFL_I_set_to_clear                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_TOD_TS_OVRFL_I_get                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_TOD_TS_OVRFL_I_poll                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_FREE_RUN_CFC_OVRFL_I_set_to_clear           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_FREE_RUN_CFC_OVRFL_I_get                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_FREE_RUN_CFC_OVRFL_I_poll                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_FREE_RUN_TS_OVRFL_I_set_to_clear            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_FREE_RUN_TS_OVRFL_I_get                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_FREE_RUN_TS_OVRFL_I_poll                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_TOD_INT_1_I_set_to_clear                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_TOD_INT_1_I_get                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_TOD_INT_1_I_poll                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_TOD_INT_2_I_set_to_clear                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_TOD_INT_2_I_get                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_TOD_INT_2_I_poll                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_FREE_RUN_INT_1_I_set_to_clear               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_FREE_RUN_INT_1_I_get                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_FREE_RUN_INT_1_I_poll                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_FREE_RUN_INT_2_I_set_to_clear               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_FREE_RUN_INT_2_I_get                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_FREE_RUN_INT_2_I_poll                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_CC_INT_1_I_set_to_clear                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_CC_INT_1_I_get                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_CC_INT_1_I_poll                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_CC_INT_2_I_set_to_clear                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_CC_INT_2_I_get                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_CC_INT_2_I_poll                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_CC_INT_3_I_set_to_clear                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_CC_INT_3_I_get                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_CC_INT_3_I_poll                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etime_field_CC_INT_4_I_set_to_clear                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32               etime_field_CC_INT_4_I_get                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_CC_INT_4_I_poll                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x0000002c SB_ECC_ERR_INT_EN_REG |
 | +-------------------------------------------+
 | | bit  16:0 R/W  RAM_BIT_SB_ECC_INT_E       |
 | +-------------------------------------------+
 | 
 | void   etime_field_RAM_BIT_SB_ECC_INT_E_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_RAM_BIT_SB_ECC_INT_E_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_RAM_BIT_SB_ECC_INT_E_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_RAM_BIT_SB_ECC_INT_E_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000030 MB_ECC_ERR_INT_EN_REG |
 | +-------------------------------------------+
 | | bit  16:0 R/W  RAM_BIT_MB_ECC_INT_E       |
 | +-------------------------------------------+
 | 
 | void   etime_field_RAM_BIT_MB_ECC_INT_E_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_RAM_BIT_MB_ECC_INT_E_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_RAM_BIT_MB_ECC_INT_E_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_RAM_BIT_MB_ECC_INT_E_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000034 REFL_OFLOW_ERR_INT_EN_REG |
 | +-----------------------------------------------+
 | | bit  11:0 R/W  EXT_RAM_OFLOW_INT_E            |
 | +-----------------------------------------------+
 | 
 | void   etime_field_EXT_RAM_OFLOW_INT_E_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_RAM_OFLOW_INT_E_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_RAM_OFLOW_INT_E_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_RAM_OFLOW_INT_E_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000038 TS_DIS_INT_EN_REG |
 | +---------------------------------------+
 | | bit  23:12 R/W  TS_RAM_WR_DIS_INT_E   |
 | | bit  11:0  R/W  TS_RAM_RD_DIS_INT_E   |
 | +---------------------------------------+
 | 
 | void   etime_field_TS_RAM_WR_DIS_INT_E_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TS_RAM_WR_DIS_INT_E_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TS_RAM_WR_DIS_INT_E_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TS_RAM_WR_DIS_INT_E_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_TS_RAM_RD_DIS_INT_E_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TS_RAM_RD_DIS_INT_E_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TS_RAM_RD_DIS_INT_E_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TS_RAM_RD_DIS_INT_E_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0000003c PACKET_DIS_INT_EN_REG |
 | +-------------------------------------------+
 | | bit  23:12 R/W  PACKET_RAM_WR_DIS_INT_E   |
 | | bit  11:0  R/W  PACKET_RAM_RD_DIS_INT_E   |
 | +-------------------------------------------+
 | 
 | void   etime_field_PACKET_RAM_WR_DIS_INT_E_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_PACKET_RAM_WR_DIS_INT_E_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_PACKET_RAM_WR_DIS_INT_E_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_PACKET_RAM_WR_DIS_INT_E_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_PACKET_RAM_RD_DIS_INT_E_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_PACKET_RAM_RD_DIS_INT_E_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_PACKET_RAM_RD_DIS_INT_E_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_PACKET_RAM_RD_DIS_INT_E_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000040 PACKET_RAM_ERR_INT_EN_REG |
 | +-----------------------------------------------+
 | | bit  11:0 R/W  PACKET_RAM_OSIZE_INT_E         |
 | +-----------------------------------------------+
 | 
 | void   etime_field_PACKET_RAM_OSIZE_INT_E_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_PACKET_RAM_OSIZE_INT_E_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_PACKET_RAM_OSIZE_INT_E_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_PACKET_RAM_OSIZE_INT_E_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000044 EXT_DESYNC_ERR_INT_EN_REG |
 | +-----------------------------------------------+
 | | bit  11:0 R/W  EXT_DESYNC_INT_E               |
 | +-----------------------------------------------+
 | 
 | void   etime_field_EXT_DESYNC_INT_E_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_DESYNC_INT_E_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_DESYNC_INT_E_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_DESYNC_INT_E_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000048 GP_INT_EN_REG           |
 | +---------------------------------------------+
 | | bit  27 R/W  ILLEGAL_1STEP_INT_E            |
 | | bit  26 R/W  LINE_REF_CPTR_EVNT_E           |
 | | bit  25 R/W  IRIGO_TS_CPTR_EVNT_E           |
 | | bit  24 R/W  IRIGO_ONE_PPSO_REALIGN_E       |
 | | bit  23 R/W  IRIGO_TX_DAT_VAL_E             |
 | | bit  22 R/W  PTP_DCSU_TSTMPR_CPTR_EVNT_E    |
 | | bit  21 R/W  IRIGI_TS_CPTR_EVNT_E           |
 | | bit  20 R/W  IRIGI_RX_DAT_VAL_E             |
 | | bit  19 R/W  IRIGI_REALIGN_E                |
 | | bit  18 R/W  IRIGI_P0PR_MISSING_E           |
 | | bit  17 R/W  IRIGI_IDLE_E                   |
 | | bit  16 R/W  IRIGI_ONE_PPSO_REALIGN_E       |
 | | bit  15 R/W  IRIGI_PWM_DEC_ERR_E            |
 | | bit  14 R/W  TOD_CFC_OFFS_TS_CNTR_UPDATED_E |
 | | bit  13 R/W  TS_INTERPOLATE_CNTR_ERR_E      |
 | | bit  12 R/W  TS_TOD_SIGN_OVRFL_E            |
 | | bit  11 R/W  CFC_OVRFL_E                    |
 | | bit  10 R/W  TOD_TS_OVRFL_E                 |
 | | bit  9  R/W  FREE_RUN_CFC_OVRFL_E           |
 | | bit  8  R/W  FREE_RUN_TS_OVRFL_E            |
 | | bit  7  R/W  TOD_INT_1_E                    |
 | | bit  6  R/W  TOD_INT_2_E                    |
 | | bit  5  R/W  FREE_RUN_INT_1_E               |
 | | bit  4  R/W  FREE_RUN_INT_2_E               |
 | | bit  3  R/W  CC_INT_1_E                     |
 | | bit  2  R/W  CC_INT_2_E                     |
 | | bit  1  R/W  CC_INT_3_E                     |
 | | bit  0  R/W  CC_INT_4_E                     |
 | +---------------------------------------------+
 | 
 | void   etime_field_ILLEGAL_1STEP_INT_E_set            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_ILLEGAL_1STEP_INT_E_get            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_LINE_REF_CPTR_EVNT_E_set           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_LINE_REF_CPTR_EVNT_E_get           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_IRIGO_TS_CPTR_EVNT_E_set           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGO_TS_CPTR_EVNT_E_get           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_IRIGO_ONE_PPSO_REALIGN_E_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGO_ONE_PPSO_REALIGN_E_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_IRIGO_TX_DAT_VAL_E_set             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGO_TX_DAT_VAL_E_get             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_PTP_DCSU_TSTMPR_CPTR_EVNT_E_set    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_PTP_DCSU_TSTMPR_CPTR_EVNT_E_get    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_IRIGI_TS_CPTR_EVNT_E_set           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_TS_CPTR_EVNT_E_get           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_IRIGI_RX_DAT_VAL_E_set             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_RX_DAT_VAL_E_get             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_IRIGI_REALIGN_E_set                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_REALIGN_E_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_IRIGI_P0PR_MISSING_E_set           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_P0PR_MISSING_E_get           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_IRIGI_IDLE_E_set                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_IDLE_E_get                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_IRIGI_ONE_PPSO_REALIGN_E_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_ONE_PPSO_REALIGN_E_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_IRIGI_PWM_DEC_ERR_E_set            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_PWM_DEC_ERR_E_get            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TOD_CFC_OFFS_TS_CNTR_UPDATED_E_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_CFC_OFFS_TS_CNTR_UPDATED_E_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TS_INTERPOLATE_CNTR_ERR_E_set      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TS_INTERPOLATE_CNTR_ERR_E_get      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TS_TOD_SIGN_OVRFL_E_set            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TS_TOD_SIGN_OVRFL_E_get            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_CFC_OVRFL_E_set                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_OVRFL_E_get                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TOD_TS_OVRFL_E_set                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_TS_OVRFL_E_get                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_FREE_RUN_CFC_OVRFL_E_set           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_FREE_RUN_CFC_OVRFL_E_get           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_FREE_RUN_TS_OVRFL_E_set            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_FREE_RUN_TS_OVRFL_E_get            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TOD_INT_1_E_set                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_INT_1_E_get                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TOD_INT_2_E_set                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_INT_2_E_get                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_FREE_RUN_INT_1_E_set               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_FREE_RUN_INT_1_E_get               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_FREE_RUN_INT_2_E_set               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_FREE_RUN_INT_2_E_get               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_CC_INT_1_E_set                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_INT_1_E_get                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_CC_INT_2_E_set                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_INT_2_E_get                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_CC_INT_3_E_set                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_INT_3_E_get                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_CC_INT_4_E_set                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_INT_4_E_get                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +--------------------------------------------+
 | | Register 0x0000004c SB_ECC_ERR_INT_VAL_REG |
 | +--------------------------------------------+
 | | bit  16:0 R  RAM_BIT_SB_ECC_INT_V          |
 | +--------------------------------------------+
 | 
 | UINT32               etime_field_RAM_BIT_SB_ECC_INT_V_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32               etime_field_range_RAM_BIT_SB_ECC_INT_V_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_RAM_BIT_SB_ECC_INT_V_poll ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_RAM_BIT_SB_ECC_INT_V_poll       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x00000050 MB_ECC_ERR_INT_VAL_REG |
 | +--------------------------------------------+
 | | bit  16:0 R  RAM_BIT_MB_ECC_INT_V          |
 | +--------------------------------------------+
 | 
 | UINT32               etime_field_RAM_BIT_MB_ECC_INT_V_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32               etime_field_range_RAM_BIT_MB_ECC_INT_V_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_RAM_BIT_MB_ECC_INT_V_poll ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_RAM_BIT_MB_ECC_INT_V_poll       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------------+
 | | Register 0x00000054 REFL_OFLOW_ERR_INT_VAL_REG |
 | +------------------------------------------------+
 | | bit  11:0 R  EXT_RAM_OFLOW_INT_V               |
 | +------------------------------------------------+
 | 
 | UINT32               etime_field_EXT_RAM_OFLOW_INT_V_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32               etime_field_range_EXT_RAM_OFLOW_INT_V_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_EXT_RAM_OFLOW_INT_V_poll ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_EXT_RAM_OFLOW_INT_V_poll       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x00000058 TS_DIS_INT_VAL_REG |
 | +----------------------------------------+
 | | bit  23:12 R  TS_RAM_WR_DIS_INT_V      |
 | | bit  11:0  R  TS_RAM_RD_DIS_INT_V      |
 | +----------------------------------------+
 | 
 | UINT32               etime_field_TS_RAM_WR_DIS_INT_V_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32               etime_field_range_TS_RAM_WR_DIS_INT_V_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_TS_RAM_WR_DIS_INT_V_poll ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_TS_RAM_WR_DIS_INT_V_poll       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_TS_RAM_RD_DIS_INT_V_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32               etime_field_range_TS_RAM_RD_DIS_INT_V_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_TS_RAM_RD_DIS_INT_V_poll ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_TS_RAM_RD_DIS_INT_V_poll       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x0000005c PACKET_DIS_INT_VAL_REG |
 | +--------------------------------------------+
 | | bit  23:12 R  PACKET_RAM_WR_DIS_INT_V      |
 | | bit  11:0  R  PACKET_RAM_RD_DIS_INT_V      |
 | +--------------------------------------------+
 | 
 | UINT32               etime_field_PACKET_RAM_WR_DIS_INT_V_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32               etime_field_range_PACKET_RAM_WR_DIS_INT_V_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_PACKET_RAM_WR_DIS_INT_V_poll ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_PACKET_RAM_WR_DIS_INT_V_poll       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_PACKET_RAM_RD_DIS_INT_V_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32               etime_field_range_PACKET_RAM_RD_DIS_INT_V_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_PACKET_RAM_RD_DIS_INT_V_poll ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_PACKET_RAM_RD_DIS_INT_V_poll       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------------+
 | | Register 0x00000060 PACKET_RAM_ERR_INT_VAL_REG |
 | +------------------------------------------------+
 | | bit  11:0 R  PACKET_RAM_OSIZE_INT_V            |
 | +------------------------------------------------+
 | 
 | UINT32               etime_field_PACKET_RAM_OSIZE_INT_V_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32               etime_field_range_PACKET_RAM_OSIZE_INT_V_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_PACKET_RAM_OSIZE_INT_V_poll ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_PACKET_RAM_OSIZE_INT_V_poll       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------------+
 | | Register 0x00000064 EXT_DESYNC_ERR_INT_VAL_REG |
 | +------------------------------------------------+
 | | bit  11:0 R  EXT_DESYNC_INT_V                  |
 | +------------------------------------------------+
 | 
 | UINT32               etime_field_EXT_DESYNC_INT_V_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32               etime_field_range_EXT_DESYNC_INT_V_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etime_field_range_EXT_DESYNC_INT_V_poll ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etime_field_EXT_DESYNC_INT_V_poll       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x00000068 GP_INT_VAL_REG        |
 | +-------------------------------------------+
 | | bit  27 R  ILLEGAL_1STEP_INT_V            |
 | | bit  26 R  LINE_REF_CPTR_EVNT_V           |
 | | bit  25 R  IRIGO_TS_CPTR_EVNT_V           |
 | | bit  24 R  IRIGO_ONE_PPSO_REALIGN_V       |
 | | bit  23 R  IRIGO_TX_DAT_VAL_V             |
 | | bit  22 R  PTP_DCSU_TSTMPR_CPTR_EVNT_V    |
 | | bit  21 R  IRIGI_TS_CPTR_EVNT_V           |
 | | bit  20 R  IRIGI_RX_DAT_VAL_V             |
 | | bit  19 R  IRIGI_REALIGN_V                |
 | | bit  18 R  IRIGI_P0PR_MISSING_V           |
 | | bit  17 R  IRIGI_IDLE_V                   |
 | | bit  16 R  IRIGI_ONE_PPSO_REALIGN_V       |
 | | bit  15 R  IRIGI_PWM_DEC_ERR_V            |
 | | bit  14 R  TOD_CFC_OFFS_TS_CNTR_UPDATED_V |
 | | bit  13 R  TS_INTERPOLATE_CNTR_ERR_V      |
 | | bit  12 R  TS_TOD_SIGN_OVRFL_V            |
 | | bit  11 R  CFC_OVRFL_V                    |
 | | bit  10 R  TOD_TS_OVRFL_V                 |
 | | bit  9  R  FREE_RUN_CFC_OVRFL_V           |
 | | bit  8  R  FREE_RUN_TS_OVRFL_V            |
 | | bit  7  R  TOD_INT_1_V                    |
 | | bit  6  R  TOD_INT_2_V                    |
 | | bit  5  R  FREE_RUN_INT_1_V               |
 | | bit  4  R  FREE_RUN_INT_2_V               |
 | | bit  3  R  CC_INT_1_V                     |
 | | bit  2  R  CC_INT_2_V                     |
 | | bit  1  R  CC_INT_3_V                     |
 | | bit  0  R  CC_INT_4_V                     |
 | +-------------------------------------------+
 | 
 | UINT32               etime_field_ILLEGAL_1STEP_INT_V_get             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_ILLEGAL_1STEP_INT_V_poll            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_LINE_REF_CPTR_EVNT_V_get            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_LINE_REF_CPTR_EVNT_V_poll           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_IRIGO_TS_CPTR_EVNT_V_get            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGO_TS_CPTR_EVNT_V_poll           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_IRIGO_ONE_PPSO_REALIGN_V_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGO_ONE_PPSO_REALIGN_V_poll       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_IRIGO_TX_DAT_VAL_V_get              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGO_TX_DAT_VAL_V_poll             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_PTP_DCSU_TSTMPR_CPTR_EVNT_V_get     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_PTP_DCSU_TSTMPR_CPTR_EVNT_V_poll    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_IRIGI_TS_CPTR_EVNT_V_get            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGI_TS_CPTR_EVNT_V_poll           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_IRIGI_RX_DAT_VAL_V_get              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGI_RX_DAT_VAL_V_poll             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_IRIGI_REALIGN_V_get                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGI_REALIGN_V_poll                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_IRIGI_P0PR_MISSING_V_get            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGI_P0PR_MISSING_V_poll           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_IRIGI_IDLE_V_get                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGI_IDLE_V_poll                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_IRIGI_ONE_PPSO_REALIGN_V_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGI_ONE_PPSO_REALIGN_V_poll       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_IRIGI_PWM_DEC_ERR_V_get             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_IRIGI_PWM_DEC_ERR_V_poll            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_TOD_CFC_OFFS_TS_CNTR_UPDATED_V_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_TOD_CFC_OFFS_TS_CNTR_UPDATED_V_poll ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_TS_INTERPOLATE_CNTR_ERR_V_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_TS_INTERPOLATE_CNTR_ERR_V_poll      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_TS_TOD_SIGN_OVRFL_V_get             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_TS_TOD_SIGN_OVRFL_V_poll            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_CFC_OVRFL_V_get                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_CFC_OVRFL_V_poll                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_TOD_TS_OVRFL_V_get                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_TOD_TS_OVRFL_V_poll                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_FREE_RUN_CFC_OVRFL_V_get            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_FREE_RUN_CFC_OVRFL_V_poll           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_FREE_RUN_TS_OVRFL_V_get             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_FREE_RUN_TS_OVRFL_V_poll            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_TOD_INT_1_V_get                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_TOD_INT_1_V_poll                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_TOD_INT_2_V_get                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_TOD_INT_2_V_poll                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_FREE_RUN_INT_1_V_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_FREE_RUN_INT_1_V_poll               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_FREE_RUN_INT_2_V_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_FREE_RUN_INT_2_V_poll               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_CC_INT_1_V_get                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_CC_INT_1_V_poll                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_CC_INT_2_V_get                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_CC_INT_2_V_poll                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_CC_INT_3_V_get                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_CC_INT_3_V_poll                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etime_field_CC_INT_4_V_get                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_field_CC_INT_4_V_poll                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------+
 | | Register 0x0000006c ING_47B_M |
 | +-------------------------------+
 | | bit  11 R/W  ING_47B_M_CH11   |
 | | bit  10 R/W  ING_47B_M_CH10   |
 | | bit  9  R/W  ING_47B_M_CH9    |
 | | bit  8  R/W  ING_47B_M_CH8    |
 | | bit  7  R/W  ING_47B_M_CH7    |
 | | bit  6  R/W  ING_47B_M_CH6    |
 | | bit  5  R/W  ING_47B_M_CH5    |
 | | bit  4  R/W  ING_47B_M_CH4    |
 | | bit  3  R/W  ING_47B_M_CH3    |
 | | bit  2  R/W  ING_47B_M_CH2    |
 | | bit  1  R/W  ING_47B_M_CH1    |
 | | bit  0  R/W  ING_47B_M_CH0    |
 | +-------------------------------+
 | 
 | void   etime_field_ING_47B_M_CH11_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_ING_47B_M_CH11_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_ING_47B_M_CH10_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_ING_47B_M_CH10_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_ING_47B_M_CH9_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_ING_47B_M_CH9_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_ING_47B_M_CH8_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_ING_47B_M_CH8_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_ING_47B_M_CH7_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_ING_47B_M_CH7_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_ING_47B_M_CH6_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_ING_47B_M_CH6_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_ING_47B_M_CH5_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_ING_47B_M_CH5_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_ING_47B_M_CH4_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_ING_47B_M_CH4_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_ING_47B_M_CH3_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_ING_47B_M_CH3_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_ING_47B_M_CH2_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_ING_47B_M_CH2_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_ING_47B_M_CH1_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_ING_47B_M_CH1_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_ING_47B_M_CH0_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_ING_47B_M_CH0_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x00000070 EGR_47B_M |
 | +-------------------------------+
 | | bit  11 R/W  EGR_47B_M_CH11   |
 | | bit  10 R/W  EGR_47B_M_CH10   |
 | | bit  9  R/W  EGR_47B_M_CH9    |
 | | bit  8  R/W  EGR_47B_M_CH8    |
 | | bit  7  R/W  EGR_47B_M_CH7    |
 | | bit  6  R/W  EGR_47B_M_CH6    |
 | | bit  5  R/W  EGR_47B_M_CH5    |
 | | bit  4  R/W  EGR_47B_M_CH4    |
 | | bit  3  R/W  EGR_47B_M_CH3    |
 | | bit  2  R/W  EGR_47B_M_CH2    |
 | | bit  1  R/W  EGR_47B_M_CH1    |
 | | bit  0  R/W  EGR_47B_M_CH0    |
 | +-------------------------------+
 | 
 | void   etime_field_EGR_47B_M_CH11_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EGR_47B_M_CH11_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_EGR_47B_M_CH10_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EGR_47B_M_CH10_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_EGR_47B_M_CH9_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EGR_47B_M_CH9_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_EGR_47B_M_CH8_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EGR_47B_M_CH8_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_EGR_47B_M_CH7_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EGR_47B_M_CH7_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_EGR_47B_M_CH6_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EGR_47B_M_CH6_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_EGR_47B_M_CH5_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EGR_47B_M_CH5_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_EGR_47B_M_CH4_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EGR_47B_M_CH4_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_EGR_47B_M_CH3_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EGR_47B_M_CH3_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_EGR_47B_M_CH2_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EGR_47B_M_CH2_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_EGR_47B_M_CH1_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EGR_47B_M_CH1_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_EGR_47B_M_CH0_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EGR_47B_M_CH0_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000074 ING_TS_ENA_REG |
 | +------------------------------------+
 | | bit  0 R/W  RX_TS_ENA              |
 | +------------------------------------+
 | 
 | void   etime_field_RX_TS_ENA_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_RX_TS_ENA_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x00000078 REFL_DEST |
 | +-------------------------------+
 | | bit  11 R/W  REFL_DEST_CH11   |
 | | bit  10 R/W  REFL_DEST_CH10   |
 | | bit  9  R/W  REFL_DEST_CH9    |
 | | bit  8  R/W  REFL_DEST_CH8    |
 | | bit  7  R/W  REFL_DEST_CH7    |
 | | bit  6  R/W  REFL_DEST_CH6    |
 | | bit  5  R/W  REFL_DEST_CH5    |
 | | bit  4  R/W  REFL_DEST_CH4    |
 | | bit  3  R/W  REFL_DEST_CH3    |
 | | bit  2  R/W  REFL_DEST_CH2    |
 | | bit  1  R/W  REFL_DEST_CH1    |
 | | bit  0  R/W  REFL_DEST_CH0    |
 | +-------------------------------+
 | 
 | void   etime_field_REFL_DEST_CH11_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DEST_CH11_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DEST_CH10_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DEST_CH10_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DEST_CH9_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DEST_CH9_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DEST_CH8_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DEST_CH8_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DEST_CH7_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DEST_CH7_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DEST_CH6_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DEST_CH6_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DEST_CH5_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DEST_CH5_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DEST_CH4_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DEST_CH4_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DEST_CH3_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DEST_CH3_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DEST_CH2_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DEST_CH2_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DEST_CH1_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DEST_CH1_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DEST_CH0_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DEST_CH0_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x0000007c EGR_TS_ENA_REG |
 | +------------------------------------+
 | | bit  11 R/W  TX_TS_ENA_LINK11      |
 | | bit  10 R/W  TX_TS_ENA_LINK10      |
 | | bit  9  R/W  TX_TS_ENA_LINK9       |
 | | bit  8  R/W  TX_TS_ENA_LINK8       |
 | | bit  7  R/W  TX_TS_ENA_LINK7       |
 | | bit  6  R/W  TX_TS_ENA_LINK6       |
 | | bit  5  R/W  TX_TS_ENA_LINK5       |
 | | bit  4  R/W  TX_TS_ENA_LINK4       |
 | | bit  3  R/W  TX_TS_ENA_LINK3       |
 | | bit  2  R/W  TX_TS_ENA_LINK2       |
 | | bit  1  R/W  TX_TS_ENA_LINK1       |
 | | bit  0  R/W  TX_TS_ENA_LINK0       |
 | +------------------------------------+
 | 
 | void   etime_field_TX_TS_ENA_LINK11_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_TS_ENA_LINK11_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TX_TS_ENA_LINK10_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_TS_ENA_LINK10_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TX_TS_ENA_LINK9_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_TS_ENA_LINK9_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TX_TS_ENA_LINK8_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_TS_ENA_LINK8_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TX_TS_ENA_LINK7_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_TS_ENA_LINK7_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TX_TS_ENA_LINK6_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_TS_ENA_LINK6_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TX_TS_ENA_LINK5_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_TS_ENA_LINK5_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TX_TS_ENA_LINK4_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_TS_ENA_LINK4_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TX_TS_ENA_LINK3_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_TS_ENA_LINK3_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TX_TS_ENA_LINK2_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_TS_ENA_LINK2_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TX_TS_ENA_LINK1_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_TS_ENA_LINK1_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TX_TS_ENA_LINK0_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_TS_ENA_LINK0_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x00000080 TOD_OFFS_SEC_HI |
 | +-------------------------------------+
 | | bit  18:16 R/W  OFFS_TS_PAGE        |
 | | bit  15:0  R/W  TOD_OFFS_SEC_HI     |
 | +-------------------------------------+
 | 
 | void   etime_field_OFFS_TS_PAGE_set          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_OFFS_TS_PAGE_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_OFFS_TS_PAGE_set    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_OFFS_TS_PAGE_get    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_TOD_OFFS_SEC_HI_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_OFFS_SEC_HI_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_OFFS_SEC_HI_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_OFFS_SEC_HI_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000084 TOD_OFFS_SEC_LO |
 | +-------------------------------------+
 | | bit  31:0 R/W  TOD_OFFS_SEC_LO      |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_OFFS_SEC_LO_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_OFFS_SEC_LO_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_OFFS_SEC_LO_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_OFFS_SEC_LO_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000088 TOD_OFFS_NSEC |
 | +-----------------------------------+
 | | bit  31:0 R/W  TOD_OFFS_NSEC      |
 | +-----------------------------------+
 | 
 | void   etime_field_TOD_OFFS_NSEC_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_OFFS_NSEC_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_OFFS_NSEC_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_OFFS_NSEC_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x0000008c CFC_OFFS_NSEC_HI |
 | +--------------------------------------+
 | | bit  15:0 R/W  CFC_OFFS_NSEC_HI      |
 | +--------------------------------------+
 | 
 | void   etime_field_CFC_OFFS_NSEC_HI_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_OFFS_NSEC_HI_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_OFFS_NSEC_HI_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_OFFS_NSEC_HI_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00000090 CFC_OFFS_NSEC_LO |
 | +--------------------------------------+
 | | bit  31:0 R/W  CFC_OFFS_NSEC_LO      |
 | +--------------------------------------+
 | 
 | void   etime_field_CFC_OFFS_NSEC_LO_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_OFFS_NSEC_LO_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_OFFS_NSEC_LO_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_OFFS_NSEC_LO_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000094 TOD_ING_DLY_CH0 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_ING_DLY_0        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_ING_DLY_0_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_ING_DLY_0_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_ING_DLY_0_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_ING_DLY_0_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000098 TOD_ING_DLY_CH1 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_ING_DLY_1        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_ING_DLY_1_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_ING_DLY_1_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_ING_DLY_1_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_ING_DLY_1_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x0000009c TOD_ING_DLY_CH2 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_ING_DLY_2        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_ING_DLY_2_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_ING_DLY_2_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_ING_DLY_2_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_ING_DLY_2_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000a0 TOD_ING_DLY_CH3 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_ING_DLY_3        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_ING_DLY_3_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_ING_DLY_3_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_ING_DLY_3_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_ING_DLY_3_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000a4 TOD_ING_DLY_CH4 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_ING_DLY_4        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_ING_DLY_4_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_ING_DLY_4_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_ING_DLY_4_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_ING_DLY_4_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000a8 TOD_ING_DLY_CH5 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_ING_DLY_5        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_ING_DLY_5_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_ING_DLY_5_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_ING_DLY_5_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_ING_DLY_5_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000ac TOD_ING_DLY_CH6 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_ING_DLY_6        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_ING_DLY_6_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_ING_DLY_6_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_ING_DLY_6_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_ING_DLY_6_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000b0 TOD_ING_DLY_CH7 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_ING_DLY_7        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_ING_DLY_7_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_ING_DLY_7_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_ING_DLY_7_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_ING_DLY_7_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000b4 TOD_ING_DLY_CH8 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_ING_DLY_8        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_ING_DLY_8_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_ING_DLY_8_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_ING_DLY_8_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_ING_DLY_8_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000b8 TOD_ING_DLY_CH9 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_ING_DLY_9        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_ING_DLY_9_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_ING_DLY_9_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_ING_DLY_9_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_ING_DLY_9_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x000000bc TOD_ING_DLY_CH10 |
 | +--------------------------------------+
 | | bit  15:0 R/W  TOD_ING_DLY_10        |
 | +--------------------------------------+
 | 
 | void   etime_field_TOD_ING_DLY_10_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_ING_DLY_10_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_ING_DLY_10_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_ING_DLY_10_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x000000c0 TOD_ING_DLY_CH11 |
 | +--------------------------------------+
 | | bit  15:0 R/W  TOD_ING_DLY_11        |
 | +--------------------------------------+
 | 
 | void   etime_field_TOD_ING_DLY_11_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_ING_DLY_11_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_ING_DLY_11_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_ING_DLY_11_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000c4 CFC_ING_DLY_CH0 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_ING_DLY_0        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_ING_DLY_0_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_ING_DLY_0_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_ING_DLY_0_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_ING_DLY_0_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000c8 CFC_ING_DLY_CH1 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_ING_DLY_1        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_ING_DLY_1_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_ING_DLY_1_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_ING_DLY_1_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_ING_DLY_1_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000cc CFC_ING_DLY_CH2 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_ING_DLY_2        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_ING_DLY_2_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_ING_DLY_2_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_ING_DLY_2_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_ING_DLY_2_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000d0 CFC_ING_DLY_CH3 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_ING_DLY_3        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_ING_DLY_3_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_ING_DLY_3_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_ING_DLY_3_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_ING_DLY_3_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000d4 CFC_ING_DLY_CH4 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_ING_DLY_4        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_ING_DLY_4_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_ING_DLY_4_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_ING_DLY_4_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_ING_DLY_4_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000d8 CFC_ING_DLY_CH5 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_ING_DLY_5        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_ING_DLY_5_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_ING_DLY_5_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_ING_DLY_5_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_ING_DLY_5_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000dc CFC_ING_DLY_CH6 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_ING_DLY_6        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_ING_DLY_6_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_ING_DLY_6_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_ING_DLY_6_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_ING_DLY_6_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000e0 CFC_ING_DLY_CH7 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_ING_DLY_7        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_ING_DLY_7_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_ING_DLY_7_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_ING_DLY_7_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_ING_DLY_7_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000e4 CFC_ING_DLY_CH8 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_ING_DLY_8        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_ING_DLY_8_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_ING_DLY_8_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_ING_DLY_8_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_ING_DLY_8_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000e8 CFC_ING_DLY_CH9 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_ING_DLY_9        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_ING_DLY_9_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_ING_DLY_9_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_ING_DLY_9_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_ING_DLY_9_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x000000ec CFC_ING_DLY_CH10 |
 | +--------------------------------------+
 | | bit  15:0 R/W  CFC_ING_DLY_10        |
 | +--------------------------------------+
 | 
 | void   etime_field_CFC_ING_DLY_10_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_ING_DLY_10_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_ING_DLY_10_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_ING_DLY_10_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x000000f0 CFC_ING_DLY_CH11 |
 | +--------------------------------------+
 | | bit  15:0 R/W  CFC_ING_DLY_11        |
 | +--------------------------------------+
 | 
 | void   etime_field_CFC_ING_DLY_11_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_ING_DLY_11_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_ING_DLY_11_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_ING_DLY_11_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000f4 TOD_EGR_DLY_CH0 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_EGR_DLY_0        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_EGR_DLY_0_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_EGR_DLY_0_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_EGR_DLY_0_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_EGR_DLY_0_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000f8 TOD_EGR_DLY_CH1 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_EGR_DLY_1        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_EGR_DLY_1_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_EGR_DLY_1_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_EGR_DLY_1_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_EGR_DLY_1_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000000fc TOD_EGR_DLY_CH2 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_EGR_DLY_2        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_EGR_DLY_2_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_EGR_DLY_2_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_EGR_DLY_2_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_EGR_DLY_2_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000100 TOD_EGR_DLY_CH3 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_EGR_DLY_3        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_EGR_DLY_3_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_EGR_DLY_3_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_EGR_DLY_3_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_EGR_DLY_3_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000104 TOD_EGR_DLY_CH4 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_EGR_DLY_4        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_EGR_DLY_4_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_EGR_DLY_4_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_EGR_DLY_4_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_EGR_DLY_4_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000108 TOD_EGR_DLY_CH5 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_EGR_DLY_5        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_EGR_DLY_5_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_EGR_DLY_5_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_EGR_DLY_5_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_EGR_DLY_5_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x0000010c TOD_EGR_DLY_CH6 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_EGR_DLY_6        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_EGR_DLY_6_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_EGR_DLY_6_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_EGR_DLY_6_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_EGR_DLY_6_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000110 TOD_EGR_DLY_CH7 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_EGR_DLY_7        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_EGR_DLY_7_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_EGR_DLY_7_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_EGR_DLY_7_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_EGR_DLY_7_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000114 TOD_EGR_DLY_CH8 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_EGR_DLY_8        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_EGR_DLY_8_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_EGR_DLY_8_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_EGR_DLY_8_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_EGR_DLY_8_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000118 TOD_EGR_DLY_CH9 |
 | +-------------------------------------+
 | | bit  15:0 R/W  TOD_EGR_DLY_9        |
 | +-------------------------------------+
 | 
 | void   etime_field_TOD_EGR_DLY_9_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_EGR_DLY_9_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_EGR_DLY_9_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_EGR_DLY_9_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x0000011c TOD_EGR_DLY_CH10 |
 | +--------------------------------------+
 | | bit  15:0 R/W  TOD_EGR_DLY_10        |
 | +--------------------------------------+
 | 
 | void   etime_field_TOD_EGR_DLY_10_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_EGR_DLY_10_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_EGR_DLY_10_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_EGR_DLY_10_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00000120 TOD_EGR_DLY_CH11 |
 | +--------------------------------------+
 | | bit  15:0 R/W  TOD_EGR_DLY_11        |
 | +--------------------------------------+
 | 
 | void   etime_field_TOD_EGR_DLY_11_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_EGR_DLY_11_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_EGR_DLY_11_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_EGR_DLY_11_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000124 CFC_EGR_DLY_CH0 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_EGR_DLY_0        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_EGR_DLY_0_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_EGR_DLY_0_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_EGR_DLY_0_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_EGR_DLY_0_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000128 CFC_EGR_DLY_CH1 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_EGR_DLY_1        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_EGR_DLY_1_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_EGR_DLY_1_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_EGR_DLY_1_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_EGR_DLY_1_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x0000012c CFC_EGR_DLY_CH2 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_EGR_DLY_2        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_EGR_DLY_2_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_EGR_DLY_2_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_EGR_DLY_2_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_EGR_DLY_2_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000130 CFC_EGR_DLY_CH3 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_EGR_DLY_3        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_EGR_DLY_3_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_EGR_DLY_3_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_EGR_DLY_3_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_EGR_DLY_3_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000134 CFC_EGR_DLY_CH4 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_EGR_DLY_4        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_EGR_DLY_4_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_EGR_DLY_4_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_EGR_DLY_4_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_EGR_DLY_4_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000138 CFC_EGR_DLY_CH5 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_EGR_DLY_5        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_EGR_DLY_5_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_EGR_DLY_5_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_EGR_DLY_5_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_EGR_DLY_5_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x0000013c CFC_EGR_DLY_CH6 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_EGR_DLY_6        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_EGR_DLY_6_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_EGR_DLY_6_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_EGR_DLY_6_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_EGR_DLY_6_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000140 CFC_EGR_DLY_CH7 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_EGR_DLY_7        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_EGR_DLY_7_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_EGR_DLY_7_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_EGR_DLY_7_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_EGR_DLY_7_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000144 CFC_EGR_DLY_CH8 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_EGR_DLY_8        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_EGR_DLY_8_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_EGR_DLY_8_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_EGR_DLY_8_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_EGR_DLY_8_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000148 CFC_EGR_DLY_CH9 |
 | +-------------------------------------+
 | | bit  15:0 R/W  CFC_EGR_DLY_9        |
 | +-------------------------------------+
 | 
 | void   etime_field_CFC_EGR_DLY_9_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_EGR_DLY_9_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_EGR_DLY_9_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_EGR_DLY_9_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x0000014c CFC_EGR_DLY_CH10 |
 | +--------------------------------------+
 | | bit  15:0 R/W  CFC_EGR_DLY_10        |
 | +--------------------------------------+
 | 
 | void   etime_field_CFC_EGR_DLY_10_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_EGR_DLY_10_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_EGR_DLY_10_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_EGR_DLY_10_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00000150 CFC_EGR_DLY_CH11 |
 | +--------------------------------------+
 | | bit  15:0 R/W  CFC_EGR_DLY_11        |
 | +--------------------------------------+
 | 
 | void   etime_field_CFC_EGR_DLY_11_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CFC_EGR_DLY_11_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CFC_EGR_DLY_11_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CFC_EGR_DLY_11_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000154 TS_CNTR_CFG     |
 | +-------------------------------------+
 | | bit  10  R/W  TS_ZERO_STRAP         |
 | | bit  9   R/W  B2B_SUPPRESS_ENA      |
 | | bit  8:7 R/W  MAC_SYNC_DLY          |
 | | bit  6   R/W  IRIGO_CAPTURED_TS_SEL |
 | | bit  5   R/W  IRIGI_CAPTURED_TS_SEL |
 | | bit  4   R/W  CAPTURED_TS_SEL       |
 | | bit  3:1 R/W  TS_ADV                |
 | | bit  0   R/W  TS_CNTR_EN            |
 | +-------------------------------------+
 | 
 | void   etime_field_TS_ZERO_STRAP_set         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TS_ZERO_STRAP_get         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_B2B_SUPPRESS_ENA_set      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_B2B_SUPPRESS_ENA_get      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_MAC_SYNC_DLY_set          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_MAC_SYNC_DLY_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_MAC_SYNC_DLY_set    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_MAC_SYNC_DLY_get    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_IRIGO_CAPTURED_TS_SEL_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGO_CAPTURED_TS_SEL_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_IRIGI_CAPTURED_TS_SEL_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_CAPTURED_TS_SEL_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_CAPTURED_TS_SEL_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CAPTURED_TS_SEL_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TS_ADV_set                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TS_ADV_get                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TS_ADV_set          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TS_ADV_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_TS_CNTR_EN_set            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TS_CNTR_EN_get            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00000158 OFFS_UPD_TIME_SEC_HI_REG |
 | +----------------------------------------------+
 | | bit  15:0 R/W  OFFS_UPD_TIME_SEC_HI          |
 | +----------------------------------------------+
 | 
 | void   etime_field_OFFS_UPD_TIME_SEC_HI_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_OFFS_UPD_TIME_SEC_HI_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_OFFS_UPD_TIME_SEC_HI_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_OFFS_UPD_TIME_SEC_HI_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x0000015c OFFS_UPD_TIME_SEC_LO_REG |
 | +----------------------------------------------+
 | | bit  31:0 R/W  OFFS_UPD_TIME_SEC_LO          |
 | +----------------------------------------------+
 | 
 | void   etime_field_OFFS_UPD_TIME_SEC_LO_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_OFFS_UPD_TIME_SEC_LO_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_OFFS_UPD_TIME_SEC_LO_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_OFFS_UPD_TIME_SEC_LO_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000160 OFFS_UPD_TIME_NSEC_REG |
 | +--------------------------------------------+
 | | bit  31:0 R/W  OFFS_UPD_TIME_NSEC          |
 | +--------------------------------------------+
 | 
 | void   etime_field_OFFS_UPD_TIME_NSEC_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_OFFS_UPD_TIME_NSEC_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_OFFS_UPD_TIME_NSEC_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_OFFS_UPD_TIME_NSEC_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000164 PTP_CLK_PER_INT_HI_REG |
 | +--------------------------------------------+
 | | bit  31:0 R/W  PTP_CLK_PER_INT_HI          |
 | +--------------------------------------------+
 | 
 | void   etime_field_PTP_CLK_PER_INT_HI_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_PTP_CLK_PER_INT_HI_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_PTP_CLK_PER_INT_HI_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_PTP_CLK_PER_INT_HI_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000168 PTP_CLK_PER_INT_LO_REG |
 | +--------------------------------------------+
 | | bit  31:0 R/W  PTP_CLK_PER_INT_LO          |
 | +--------------------------------------------+
 | 
 | void   etime_field_PTP_CLK_PER_INT_LO_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_PTP_CLK_PER_INT_LO_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_PTP_CLK_PER_INT_LO_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_PTP_CLK_PER_INT_LO_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0000016c PTP_CLK_PER_NUM_REG |
 | +-----------------------------------------+
 | | bit  31:0 R/W  PTP_CLK_PER_NUM          |
 | +-----------------------------------------+
 | 
 | void   etime_field_PTP_CLK_PER_NUM_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_PTP_CLK_PER_NUM_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_PTP_CLK_PER_NUM_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_PTP_CLK_PER_NUM_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000170 PTP_CLK_PER_DEN_REG |
 | +-----------------------------------------+
 | | bit  31:0 R/W  PTP_CLK_PER_DEN          |
 | +-----------------------------------------+
 | 
 | void   etime_field_PTP_CLK_PER_DEN_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_PTP_CLK_PER_DEN_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_PTP_CLK_PER_DEN_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_PTP_CLK_PER_DEN_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000174 OFFSET_UPD_QUANTA_REG |
 | +-------------------------------------------+
 | | bit  29:0 R/W  OFFSET_UPD_QUANTA          |
 | +-------------------------------------------+
 | 
 | void   etime_field_OFFSET_UPD_QUANTA_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_OFFSET_UPD_QUANTA_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_OFFSET_UPD_QUANTA_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_OFFSET_UPD_QUANTA_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000178 LINE_CLK_PER_INT_HI_REG |
 | +---------------------------------------------+
 | | bit  31:0 R/W  LINE_CLK_PER_INT_HI          |
 | +---------------------------------------------+
 | 
 | void   etime_field_LINE_CLK_PER_INT_HI_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_LINE_CLK_PER_INT_HI_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_LINE_CLK_PER_INT_HI_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_LINE_CLK_PER_INT_HI_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x0000017c LINE_CLK_PER_INT_LO_REG |
 | +---------------------------------------------+
 | | bit  31:0 R/W  LINE_CLK_PER_INT_LO          |
 | +---------------------------------------------+
 | 
 | void   etime_field_LINE_CLK_PER_INT_LO_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_LINE_CLK_PER_INT_LO_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_LINE_CLK_PER_INT_LO_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_LINE_CLK_PER_INT_LO_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000180 LINE_CLK_PER_NUM_REG |
 | +------------------------------------------+
 | | bit  31:0 R/W  LINE_CLK_PER_NUM          |
 | +------------------------------------------+
 | 
 | void   etime_field_LINE_CLK_PER_NUM_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_LINE_CLK_PER_NUM_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_LINE_CLK_PER_NUM_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_LINE_CLK_PER_NUM_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000184 LINE_CLK_PER_DEN_REG |
 | +------------------------------------------+
 | | bit  31:0 R/W  LINE_CLK_PER_DEN          |
 | +------------------------------------------+
 | 
 | void   etime_field_LINE_CLK_PER_DEN_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_LINE_CLK_PER_DEN_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_LINE_CLK_PER_DEN_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_LINE_CLK_PER_DEN_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000188 PTP_CNT              |
 | +------------------------------------------+
 | | bit  31:30 R  LINE_REF_PTP_SEC_CNTR_FRC  |
 | | bit  29:0  R  LINE_REF_PTP_NSEC_CNTR_FRC |
 | +------------------------------------------+
 | 
 | UINT32 etime_field_LINE_REF_PTP_SEC_CNTR_FRC_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_LINE_REF_PTP_SEC_CNTR_FRC_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 etime_field_LINE_REF_PTP_NSEC_CNTR_FRC_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_LINE_REF_PTP_NSEC_CNTR_FRC_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x0000018c LINE_REF_CNT     |
 | +--------------------------------------+
 | | bit  31:30 R  LINE_REF_SEC_CNTR_FRC  |
 | | bit  29:0  R  LINE_REF_NSEC_CNTR_FRC |
 | +--------------------------------------+
 | 
 | UINT32 etime_field_LINE_REF_SEC_CNTR_FRC_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_LINE_REF_SEC_CNTR_FRC_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 etime_field_LINE_REF_NSEC_CNTR_FRC_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_LINE_REF_NSEC_CNTR_FRC_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000190 TOD_TS_OUT_EXT_SEC_HI_REG |
 | +-----------------------------------------------+
 | | bit  18:16 R  TOD_TS_OUT_EXT_TS_PAGE          |
 | | bit  15:0  R  TOD_TS_OUT_EXT_SEC_HI           |
 | +-----------------------------------------------+
 | 
 | UINT32 etime_field_TOD_TS_OUT_EXT_TS_PAGE_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_TOD_TS_OUT_EXT_TS_PAGE_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 etime_field_TOD_TS_OUT_EXT_SEC_HI_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_TOD_TS_OUT_EXT_SEC_HI_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000194 TOD_TS_OUT_EXT_SEC_LO_REG |
 | +-----------------------------------------------+
 | | bit  31:0 R  TOD_TS_OUT_EXT_SEC_LO            |
 | +-----------------------------------------------+
 | 
 | UINT32 etime_field_TOD_TS_OUT_EXT_SEC_LO_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_TOD_TS_OUT_EXT_SEC_LO_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000198 TOD_TS_OUT_EXT_NSEC_REG |
 | +---------------------------------------------+
 | | bit  31:0 R  TOD_TS_OUT_EXT_NSEC            |
 | +---------------------------------------------+
 | 
 | UINT32 etime_field_TOD_TS_OUT_EXT_NSEC_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_TOD_TS_OUT_EXT_NSEC_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0000019c CFC_TS_OUT_EXT_HI_REG |
 | +-------------------------------------------+
 | | bit  15:0 R  CFC_TS_OUT_EXT_HI            |
 | +-------------------------------------------+
 | 
 | UINT32 etime_field_CFC_TS_OUT_EXT_HI_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_CFC_TS_OUT_EXT_HI_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x000001a0 CFC_TS_OUT_EXT_LO_REG |
 | +-------------------------------------------+
 | | bit  31:0 R  CFC_TS_OUT_EXT_LO            |
 | +-------------------------------------------+
 | 
 | UINT32 etime_field_CFC_TS_OUT_EXT_LO_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_CFC_TS_OUT_EXT_LO_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000001a4 TOD_TS_OUT_IRIGI_SEC_HI_REG |
 | +-------------------------------------------------+
 | | bit  18:16 R  TS_OUT_IRIGI_TS_PAGE              |
 | | bit  15:0  R  TOD_TS_OUT_IRIGI_SEC_HI           |
 | +-------------------------------------------------+
 | 
 | UINT32 etime_field_TS_OUT_IRIGI_TS_PAGE_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_TS_OUT_IRIGI_TS_PAGE_get    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 etime_field_TOD_TS_OUT_IRIGI_SEC_HI_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_TOD_TS_OUT_IRIGI_SEC_HI_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000001a8 TOD_TS_OUT_IRIGI_SEC_LO_REG |
 | +-------------------------------------------------+
 | | bit  31:0 R  TOD_TS_OUT_IRIGI_SEC_LO            |
 | +-------------------------------------------------+
 | 
 | UINT32 etime_field_TOD_TS_OUT_IRIGI_SEC_LO_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_TOD_TS_OUT_IRIGI_SEC_LO_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x000001ac TOD_TS_OUT_IRIGI_NSEC_REG |
 | +-----------------------------------------------+
 | | bit  31:0 R  TOD_TS_OUT_IRIGI_NSEC            |
 | +-----------------------------------------------+
 | 
 | UINT32 etime_field_TOD_TS_OUT_IRIGI_NSEC_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_TOD_TS_OUT_IRIGI_NSEC_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x000001b0 CFC_TS_OUT_IRIGI_HI_REG |
 | +---------------------------------------------+
 | | bit  15:0 R  CFC_TS_OUT_IRIGI_HI            |
 | +---------------------------------------------+
 | 
 | UINT32 etime_field_CFC_TS_OUT_IRIGI_HI_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_CFC_TS_OUT_IRIGI_HI_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x000001b4 CFC_TS_OUT_IRIGI_LO_REG |
 | +---------------------------------------------+
 | | bit  31:0 R  CFC_TS_OUT_IRIGI_LO            |
 | +---------------------------------------------+
 | 
 | UINT32 etime_field_CFC_TS_OUT_IRIGI_LO_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_CFC_TS_OUT_IRIGI_LO_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000001b8 TOD_TS_OUT_IRIGO_SEC_HI_REG |
 | +-------------------------------------------------+
 | | bit  18:16 R  TS_OUT_IRIGO_TS_PAGE              |
 | | bit  15:0  R  TOD_TS_OUT_IRIGO_SEC_HI           |
 | +-------------------------------------------------+
 | 
 | UINT32 etime_field_TS_OUT_IRIGO_TS_PAGE_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_TS_OUT_IRIGO_TS_PAGE_get    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 etime_field_TOD_TS_OUT_IRIGO_SEC_HI_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_TOD_TS_OUT_IRIGO_SEC_HI_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000001bc TOD_TS_OUT_IRIGO_SEC_LO_REG |
 | +-------------------------------------------------+
 | | bit  31:0 R  TOD_TS_OUT_IRIGO_SEC_LO            |
 | +-------------------------------------------------+
 | 
 | UINT32 etime_field_TOD_TS_OUT_IRIGO_SEC_LO_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_TOD_TS_OUT_IRIGO_SEC_LO_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x000001c0 TOD_TS_OUT_IRIGO_NSEC_REG |
 | +-----------------------------------------------+
 | | bit  31:0 R  TOD_TS_OUT_IRIGO_NSEC            |
 | +-----------------------------------------------+
 | 
 | UINT32 etime_field_TOD_TS_OUT_IRIGO_NSEC_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_TOD_TS_OUT_IRIGO_NSEC_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x000001c4 CFC_TS_OUT_IRIGO_HI_REG |
 | +---------------------------------------------+
 | | bit  15:0 R  CFC_TS_OUT_IRIGO_HI            |
 | +---------------------------------------------+
 | 
 | UINT32 etime_field_CFC_TS_OUT_IRIGO_HI_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_CFC_TS_OUT_IRIGO_HI_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x000001c8 CFC_TS_OUT_IRIGO_LO_REG |
 | +---------------------------------------------+
 | | bit  31:0 R  CFC_TS_OUT_IRIGO_LO            |
 | +---------------------------------------------+
 | 
 | UINT32 etime_field_CFC_TS_OUT_IRIGO_LO_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_CFC_TS_OUT_IRIGO_LO_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000001cc TX_VEC_IRIGO_4_REG |
 | +----------------------------------------+
 | | bit  31:0 R/W  TX_VEC_IRIGO_4          |
 | +----------------------------------------+
 | 
 | void   etime_field_TX_VEC_IRIGO_4_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_VEC_IRIGO_4_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TX_VEC_IRIGO_4_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TX_VEC_IRIGO_4_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000001d0 TX_VEC_IRIGO_3_REG |
 | +----------------------------------------+
 | | bit  31:0 R/W  TX_VEC_IRIGO_3          |
 | +----------------------------------------+
 | 
 | void   etime_field_TX_VEC_IRIGO_3_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_VEC_IRIGO_3_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TX_VEC_IRIGO_3_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TX_VEC_IRIGO_3_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000001d4 TX_VEC_IRIGO_2_REG |
 | +----------------------------------------+
 | | bit  31:0 R/W  TX_VEC_IRIGO_2          |
 | +----------------------------------------+
 | 
 | void   etime_field_TX_VEC_IRIGO_2_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_VEC_IRIGO_2_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TX_VEC_IRIGO_2_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TX_VEC_IRIGO_2_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000001d8 TX_VEC_IRIGO_1_REG |
 | +----------------------------------------+
 | | bit  3:0 R/W  TX_VEC_IRIGO_1           |
 | +----------------------------------------+
 | 
 | void   etime_field_TX_VEC_IRIGO_1_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TX_VEC_IRIGO_1_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TX_VEC_IRIGO_1_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TX_VEC_IRIGO_1_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000001dc RX_VEC_IRIGI_4_REG |
 | +----------------------------------------+
 | | bit  31:0 R  RX_VEC_IRIGI_4            |
 | +----------------------------------------+
 | 
 | UINT32 etime_field_RX_VEC_IRIGI_4_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_RX_VEC_IRIGI_4_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000001e0 RX_VEC_IRIGI_3_REG |
 | +----------------------------------------+
 | | bit  31:0 R  RX_VEC_IRIGI_3            |
 | +----------------------------------------+
 | 
 | UINT32 etime_field_RX_VEC_IRIGI_3_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_RX_VEC_IRIGI_3_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000001e4 RX_VEC_IRIGI_2_REG |
 | +----------------------------------------+
 | | bit  31:0 R  RX_VEC_IRIGI_2            |
 | +----------------------------------------+
 | 
 | UINT32 etime_field_RX_VEC_IRIGI_2_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_RX_VEC_IRIGI_2_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000001e8 RX_VEC_IRIGI_1_REG |
 | +----------------------------------------+
 | | bit  3:0 R  RX_VEC_IRIGI_1             |
 | +----------------------------------------+
 | 
 | UINT32 etime_field_RX_VEC_IRIGI_1_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_RX_VEC_IRIGI_1_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x000001ec IRIGI_CFG     |
 | +-----------------------------------+
 | | bit  3   R/W  IRIGI_1PPS_DET_MODE |
 | | bit  2:1 R/W  IRIGI_MODE          |
 | | bit  0   R/W  IRIGI_ENA           |
 | +-----------------------------------+
 | 
 | void   etime_field_IRIGI_1PPS_DET_MODE_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_1PPS_DET_MODE_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_IRIGI_MODE_set          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_MODE_get          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGI_MODE_set    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGI_MODE_get    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_IRIGI_ENA_set           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_ENA_get           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x000001f0 IRIGI_PWM_ZERO_REG  |
 | +-----------------------------------------+
 | | bit  21:0 R/W  IRIGI_PWM_WIDTH_ZERO_CFG |
 | +-----------------------------------------+
 | 
 | void   etime_field_IRIGI_PWM_WIDTH_ZERO_CFG_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_PWM_WIDTH_ZERO_CFG_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGI_PWM_WIDTH_ZERO_CFG_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGI_PWM_WIDTH_ZERO_CFG_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000001f4 IRIGI_PWM_ONE_REG  |
 | +----------------------------------------+
 | | bit  21:0 R/W  IRIGI_PWM_WIDTH_ONE_CFG |
 | +----------------------------------------+
 | 
 | void   etime_field_IRIGI_PWM_WIDTH_ONE_CFG_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_PWM_WIDTH_ONE_CFG_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGI_PWM_WIDTH_ONE_CFG_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGI_PWM_WIDTH_ONE_CFG_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x000001f8 IRIGI_PWM_WIDTH_PX_REG |
 | +--------------------------------------------+
 | | bit  21:0 R/W  IRIGI_PWM_WIDTH_PX_CFG      |
 | +--------------------------------------------+
 | 
 | void   etime_field_IRIGI_PWM_WIDTH_PX_CFG_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_PWM_WIDTH_PX_CFG_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGI_PWM_WIDTH_PX_CFG_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGI_PWM_WIDTH_PX_CFG_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x000001fc IRIGI_PWM_MIN_DIFF_CFG_REG |
 | +------------------------------------------------+
 | | bit  20:0 R/W  IRIGI_PWM_MIN_DIFF_CFG          |
 | +------------------------------------------------+
 | 
 | void   etime_field_IRIGI_PWM_MIN_DIFF_CFG_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_PWM_MIN_DIFF_CFG_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGI_PWM_MIN_DIFF_CFG_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGI_PWM_MIN_DIFF_CFG_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000200 IRIGI_PWM_MAX_DIFF_CFG_REG |
 | +------------------------------------------------+
 | | bit  20:0 R/W  IRIGI_PWM_MAX_DIFF_CFG          |
 | +------------------------------------------------+
 | 
 | void   etime_field_IRIGI_PWM_MAX_DIFF_CFG_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_PWM_MAX_DIFF_CFG_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGI_PWM_MAX_DIFF_CFG_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGI_PWM_MAX_DIFF_CFG_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x00000204 IRIGI_DATI_IDLE_DURATION |
 | +----------------------------------------------+
 | | bit  27:0 R/W  IRIGI_DATI_IDLE_DURATION      |
 | +----------------------------------------------+
 | 
 | void   etime_field_IRIGI_DATI_IDLE_DURATION_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGI_DATI_IDLE_DURATION_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGI_DATI_IDLE_DURATION_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGI_DATI_IDLE_DURATION_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x00000208 IRIGO_CFG |
 | +-------------------------------+
 | | bit  3   R/W  IRIGO_P0PR_ONLY |
 | | bit  2:1 R/W  IRIGO_MODE      |
 | | bit  0   R/W  IRIGO_ENA       |
 | +-------------------------------+
 | 
 | void   etime_field_IRIGO_P0PR_ONLY_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGO_P0PR_ONLY_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_IRIGO_MODE_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGO_MODE_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGO_MODE_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGO_MODE_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_IRIGO_ENA_set        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGO_ENA_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +-----------------------------------------------+
 | | Register 0x0000020c IRIGO_PWM_NOM_PER_CFG_REG |
 | +-----------------------------------------------+
 | | bit  21:0 R/W  IRIGO_PWM_NOM_PER_CFG          |
 | +-----------------------------------------------+
 | 
 | void   etime_field_IRIGO_PWM_NOM_PER_CFG_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGO_PWM_NOM_PER_CFG_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGO_PWM_NOM_PER_CFG_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGO_PWM_NOM_PER_CFG_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000210 IRIGO_PWM_ONE_REG  |
 | +----------------------------------------+
 | | bit  21:0 R/W  IRIGO_PWM_WIDTH_ONE_CFG |
 | +----------------------------------------+
 | 
 | void   etime_field_IRIGO_PWM_WIDTH_ONE_CFG_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGO_PWM_WIDTH_ONE_CFG_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGO_PWM_WIDTH_ONE_CFG_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGO_PWM_WIDTH_ONE_CFG_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000214 IRIGO_PWM_ZERO_REG  |
 | +-----------------------------------------+
 | | bit  21:0 R/W  IRIGO_PWM_WIDTH_ZERO_CFG |
 | +-----------------------------------------+
 | 
 | void   etime_field_IRIGO_PWM_WIDTH_ZERO_CFG_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGO_PWM_WIDTH_ZERO_CFG_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGO_PWM_WIDTH_ZERO_CFG_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGO_PWM_WIDTH_ZERO_CFG_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000218 IRIGO_PWM_WIDTH_PX_REG |
 | +--------------------------------------------+
 | | bit  21:0 R/W  IRIGO_PWM_WIDTH_PX_CFG      |
 | +--------------------------------------------+
 | 
 | void   etime_field_IRIGO_PWM_WIDTH_PX_CFG_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGO_PWM_WIDTH_PX_CFG_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGO_PWM_WIDTH_PX_CFG_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGO_PWM_WIDTH_PX_CFG_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x0000021c IRIGO_PWM_1PPS_WIDTH_REG |
 | +----------------------------------------------+
 | | bit  24:0 R/W  IRIGO_PWM_1PPS_WIDTH_CFG      |
 | +----------------------------------------------+
 | 
 | void   etime_field_IRIGO_PWM_1PPS_WIDTH_CFG_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGO_PWM_1PPS_WIDTH_CFG_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGO_PWM_1PPS_WIDTH_CFG_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGO_PWM_1PPS_WIDTH_CFG_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000220 INT_GEN_CFG |
 | +---------------------------------+
 | | bit  11 R/W  CC_INT_4_RAND_EN   |
 | | bit  10 R/W  CC_INT_3_RAND_EN   |
 | | bit  9  R/W  CC_INT_2_RAND_EN   |
 | | bit  8  R/W  CC_INT_1_RAND_EN   |
 | | bit  7  R/W  CC_INT_4_EN        |
 | | bit  6  R/W  CC_INT_3_EN        |
 | | bit  5  R/W  CC_INT_2_EN        |
 | | bit  4  R/W  CC_INT_1_EN        |
 | | bit  3  R/W  TOD_INT_2_EN       |
 | | bit  2  R/W  TOD_INT_1_EN       |
 | | bit  1  R/W  FREE_RUN_INT_2_EN  |
 | | bit  0  R/W  FREE_RUN_INT_1_EN  |
 | +---------------------------------+
 | 
 | void   etime_field_CC_INT_4_RAND_EN_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_INT_4_RAND_EN_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_CC_INT_3_RAND_EN_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_INT_3_RAND_EN_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_CC_INT_2_RAND_EN_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_INT_2_RAND_EN_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_CC_INT_1_RAND_EN_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_INT_1_RAND_EN_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_CC_INT_4_EN_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_INT_4_EN_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_CC_INT_3_EN_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_INT_3_EN_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_CC_INT_2_EN_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_INT_2_EN_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_CC_INT_1_EN_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_INT_1_EN_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TOD_INT_2_EN_set      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_INT_2_EN_get      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_TOD_INT_1_EN_set      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_INT_1_EN_get      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_FREE_RUN_INT_2_EN_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_FREE_RUN_INT_2_EN_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_FREE_RUN_INT_1_EN_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_FREE_RUN_INT_1_EN_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000224 INT_PER_CFG    |
 | +------------------------------------+
 | | bit  31:24 R/W  TOD_INT_PER_2      |
 | | bit  23:16 R/W  TOD_INT_PER_1      |
 | | bit  15:8  R/W  FREE_RUN_INT_PER_2 |
 | | bit  7:0   R/W  FREE_RUN_INT_PER_1 |
 | +------------------------------------+
 | 
 | void   etime_field_TOD_INT_PER_2_set            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_INT_PER_2_get            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_INT_PER_2_set      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_INT_PER_2_get      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_TOD_INT_PER_1_set            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_TOD_INT_PER_1_get            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_TOD_INT_PER_1_set      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_TOD_INT_PER_1_get      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_FREE_RUN_INT_PER_2_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_FREE_RUN_INT_PER_2_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_FREE_RUN_INT_PER_2_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_FREE_RUN_INT_PER_2_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_FREE_RUN_INT_PER_1_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_FREE_RUN_INT_PER_1_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_FREE_RUN_INT_PER_1_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_FREE_RUN_INT_PER_1_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000228 CC_INT_PER_HI_4_CFG |
 | +-----------------------------------------+
 | | bit  3:0 R/W  CC_MAX_RAND_INT_PER_HI_4  |
 | +-----------------------------------------+
 | 
 | void   etime_field_CC_MAX_RAND_INT_PER_HI_4_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_MAX_RAND_INT_PER_HI_4_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CC_MAX_RAND_INT_PER_HI_4_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CC_MAX_RAND_INT_PER_HI_4_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0000022c CC_INT_PER_HI_3_CFG |
 | +-----------------------------------------+
 | | bit  3:0 R/W  CC_MAX_RAND_INT_PER_HI_3  |
 | +-----------------------------------------+
 | 
 | void   etime_field_CC_MAX_RAND_INT_PER_HI_3_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_MAX_RAND_INT_PER_HI_3_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CC_MAX_RAND_INT_PER_HI_3_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CC_MAX_RAND_INT_PER_HI_3_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000230 CC_INT_PER_HI_2_CFG |
 | +-----------------------------------------+
 | | bit  3:0 R/W  CC_MAX_RAND_INT_PER_HI_2  |
 | +-----------------------------------------+
 | 
 | void   etime_field_CC_MAX_RAND_INT_PER_HI_2_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_MAX_RAND_INT_PER_HI_2_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CC_MAX_RAND_INT_PER_HI_2_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CC_MAX_RAND_INT_PER_HI_2_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000234 CC_INT_PER_HI_1_CFG |
 | +-----------------------------------------+
 | | bit  3:0 R/W  CC_MAX_RAND_INT_PER_HI_1  |
 | +-----------------------------------------+
 | 
 | void   etime_field_CC_MAX_RAND_INT_PER_HI_1_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_MAX_RAND_INT_PER_HI_1_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CC_MAX_RAND_INT_PER_HI_1_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CC_MAX_RAND_INT_PER_HI_1_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000238 CC_INT_PER_4_CFG    |
 | +-----------------------------------------+
 | | bit  31:0 R/W  CC_MAX_RAND_INT_PER_LO_4 |
 | +-----------------------------------------+
 | 
 | void   etime_field_CC_MAX_RAND_INT_PER_LO_4_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_MAX_RAND_INT_PER_LO_4_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CC_MAX_RAND_INT_PER_LO_4_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CC_MAX_RAND_INT_PER_LO_4_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0000023c CC_INT_PER_3_CFG    |
 | +-----------------------------------------+
 | | bit  31:0 R/W  CC_MAX_RAND_INT_PER_LO_3 |
 | +-----------------------------------------+
 | 
 | void   etime_field_CC_MAX_RAND_INT_PER_LO_3_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_MAX_RAND_INT_PER_LO_3_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CC_MAX_RAND_INT_PER_LO_3_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CC_MAX_RAND_INT_PER_LO_3_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000240 CC_INT_PER_2_CFG    |
 | +-----------------------------------------+
 | | bit  31:0 R/W  CC_MAX_RAND_INT_PER_LO_2 |
 | +-----------------------------------------+
 | 
 | void   etime_field_CC_MAX_RAND_INT_PER_LO_2_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_MAX_RAND_INT_PER_LO_2_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CC_MAX_RAND_INT_PER_LO_2_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CC_MAX_RAND_INT_PER_LO_2_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000244 CC_INT_PER_1_CFG    |
 | +-----------------------------------------+
 | | bit  31:0 R/W  CC_MAX_RAND_INT_PER_LO_1 |
 | +-----------------------------------------+
 | 
 | void   etime_field_CC_MAX_RAND_INT_PER_LO_1_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CC_MAX_RAND_INT_PER_LO_1_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CC_MAX_RAND_INT_PER_LO_1_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CC_MAX_RAND_INT_PER_LO_1_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x0000024c REFL_DLY_CFG      |
 | +---------------------------------------+
 | | bit  29:20 R/W  REFL_TIMEOUT_100G_DLY |
 | | bit  19:10 R/W  REFL_TIMEOUT_40G_DLY  |
 | | bit  9:0   R/W  REFL_TIMEOUT_10G_DLY  |
 | +---------------------------------------+
 | 
 | void   etime_field_REFL_TIMEOUT_100G_DLY_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_TIMEOUT_100G_DLY_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_REFL_TIMEOUT_100G_DLY_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_REFL_TIMEOUT_100G_DLY_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_REFL_TIMEOUT_40G_DLY_set        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_TIMEOUT_40G_DLY_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_REFL_TIMEOUT_40G_DLY_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_REFL_TIMEOUT_40G_DLY_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_REFL_TIMEOUT_10G_DLY_set        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_TIMEOUT_10G_DLY_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_REFL_TIMEOUT_10G_DLY_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_REFL_TIMEOUT_10G_DLY_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00000250 INTERPOL_CFG |
 | +----------------------------------+
 | | bit  31:28 R/W  INTERPOL_VAL7    |
 | | bit  27:24 R/W  INTERPOL_VAL6    |
 | | bit  23:20 R/W  INTERPOL_VAL5    |
 | | bit  19:16 R/W  INTERPOL_VAL4    |
 | | bit  15:12 R/W  INTERPOL_VAL3    |
 | | bit  11:8  R/W  INTERPOL_VAL2    |
 | | bit  7:4   R/W  INTERPOL_VAL1    |
 | | bit  3:0   R/W  INTERPOL_VAL0    |
 | +----------------------------------+
 | 
 | void   etime_field_INTERPOL_VAL7_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_INTERPOL_VAL7_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_INTERPOL_VAL7_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_INTERPOL_VAL7_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_INTERPOL_VAL6_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_INTERPOL_VAL6_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_INTERPOL_VAL6_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_INTERPOL_VAL6_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_INTERPOL_VAL5_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_INTERPOL_VAL5_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_INTERPOL_VAL5_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_INTERPOL_VAL5_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_INTERPOL_VAL4_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_INTERPOL_VAL4_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_INTERPOL_VAL4_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_INTERPOL_VAL4_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_INTERPOL_VAL3_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_INTERPOL_VAL3_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_INTERPOL_VAL3_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_INTERPOL_VAL3_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_INTERPOL_VAL2_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_INTERPOL_VAL2_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_INTERPOL_VAL2_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_INTERPOL_VAL2_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_INTERPOL_VAL1_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_INTERPOL_VAL1_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_INTERPOL_VAL1_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_INTERPOL_VAL1_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_INTERPOL_VAL0_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_INTERPOL_VAL0_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_INTERPOL_VAL0_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_INTERPOL_VAL0_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000254 CAPT_OFFS   |
 | +---------------------------------+
 | | bit  29:20 R/W  CAPT_OFFS_EXT   |
 | | bit  19:10 R/W  CAPT_OFFS_IRIGI |
 | | bit  9:0   R/W  CAPT_OFFS_IRIGO |
 | +---------------------------------+
 | 
 | void   etime_field_CAPT_OFFS_EXT_set         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CAPT_OFFS_EXT_get         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CAPT_OFFS_EXT_set   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CAPT_OFFS_EXT_get   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_CAPT_OFFS_IRIGI_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CAPT_OFFS_IRIGI_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CAPT_OFFS_IRIGI_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CAPT_OFFS_IRIGI_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_CAPT_OFFS_IRIGO_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_CAPT_OFFS_IRIGO_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_CAPT_OFFS_IRIGO_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_CAPT_OFFS_IRIGO_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00000288 EXT_CPB_LSC_LOW  |
 | +--------------------------------------+
 | | bit  31:28 R/W  EXT_CPB_LINK_SCH_TS7 |
 | | bit  27:24 R/W  EXT_CPB_LINK_SCH_TS6 |
 | | bit  23:20 R/W  EXT_CPB_LINK_SCH_TS5 |
 | | bit  19:16 R/W  EXT_CPB_LINK_SCH_TS4 |
 | | bit  15:12 R/W  EXT_CPB_LINK_SCH_TS3 |
 | | bit  11:8  R/W  EXT_CPB_LINK_SCH_TS2 |
 | | bit  7:4   R/W  EXT_CPB_LINK_SCH_TS1 |
 | | bit  3:0   R/W  EXT_CPB_LINK_SCH_TS0 |
 | +--------------------------------------+
 | 
 | void   etime_field_EXT_CPB_LINK_SCH_TS7_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_TS7_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_CPB_LINK_SCH_TS7_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_CPB_LINK_SCH_TS7_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_EXT_CPB_LINK_SCH_TS6_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_TS6_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_CPB_LINK_SCH_TS6_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_CPB_LINK_SCH_TS6_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_EXT_CPB_LINK_SCH_TS5_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_TS5_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_CPB_LINK_SCH_TS5_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_CPB_LINK_SCH_TS5_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_EXT_CPB_LINK_SCH_TS4_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_TS4_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_CPB_LINK_SCH_TS4_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_CPB_LINK_SCH_TS4_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_EXT_CPB_LINK_SCH_TS3_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_TS3_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_CPB_LINK_SCH_TS3_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_CPB_LINK_SCH_TS3_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_EXT_CPB_LINK_SCH_TS2_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_TS2_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_CPB_LINK_SCH_TS2_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_CPB_LINK_SCH_TS2_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_EXT_CPB_LINK_SCH_TS1_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_TS1_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_CPB_LINK_SCH_TS1_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_CPB_LINK_SCH_TS1_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_EXT_CPB_LINK_SCH_TS0_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_TS0_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_CPB_LINK_SCH_TS0_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_CPB_LINK_SCH_TS0_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x0000028c EXT_CPB_LSC_HIGH  |
 | +---------------------------------------+
 | | bit  15:12 R/W  EXT_CPB_LINK_SCH_TS11 |
 | | bit  11:8  R/W  EXT_CPB_LINK_SCH_TS10 |
 | | bit  7:4   R/W  EXT_CPB_LINK_SCH_TS9  |
 | | bit  3:0   R/W  EXT_CPB_LINK_SCH_TS8  |
 | +---------------------------------------+
 | 
 | void   etime_field_EXT_CPB_LINK_SCH_TS11_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_TS11_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_CPB_LINK_SCH_TS11_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_CPB_LINK_SCH_TS11_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_EXT_CPB_LINK_SCH_TS10_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_TS10_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_CPB_LINK_SCH_TS10_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_CPB_LINK_SCH_TS10_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_EXT_CPB_LINK_SCH_TS9_set        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_TS9_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_CPB_LINK_SCH_TS9_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_CPB_LINK_SCH_TS9_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_EXT_CPB_LINK_SCH_TS8_set        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_TS8_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_CPB_LINK_SCH_TS8_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_CPB_LINK_SCH_TS8_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x00000290 EXT_CPB_CFG              |
 | +----------------------------------------------+
 | | bit  8   R/W  PMON_CNT_CLR                   |
 | | bit  7:4 R/W  EXT_CPB_LINK_SCH_LAST_TIMESLOT |
 | | bit  3:0 R/W  EXT_CPB_LINK_SCH_OFFSET        |
 | +----------------------------------------------+
 | 
 | void   etime_field_EXT_CPB_LINK_SCH_LAST_TIMESLOT_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_LAST_TIMESLOT_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_CPB_LINK_SCH_LAST_TIMESLOT_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_CPB_LINK_SCH_LAST_TIMESLOT_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_EXT_CPB_LINK_SCH_OFFSET_set              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_OFFSET_get              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_CPB_LINK_SCH_OFFSET_set        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_CPB_LINK_SCH_OFFSET_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_PMON_CNT_CLR_set                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_PMON_CNT_CLR_get                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x00000294 EXT_LNK_SCH_UPD |
 | +-------------------------------------+
 | | bit  0 R/W  EXT_CPB_LINK_SCH_UPD    |
 | +-------------------------------------+
 | 
 | void   etime_field_EXT_CPB_LINK_SCH_UPD_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_CPB_LINK_SCH_UPD_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +--------------------------------------------+
 | | Register 0x00000298 EXT_NUM_SLOTS_OCCUPIED |
 | +--------------------------------------------+
 | | bit  20:14 R/W  EXT_SLOTS_OCCUPIED_100G    |
 | | bit  13:7  R/W  EXT_SLOTS_OCCUPIED_40G     |
 | | bit  6:0   R/W  EXT_SLOTS_OCCUPIED_10G     |
 | +--------------------------------------------+
 | 
 | void   etime_field_EXT_SLOTS_OCCUPIED_100G_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_SLOTS_OCCUPIED_100G_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_SLOTS_OCCUPIED_100G_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_SLOTS_OCCUPIED_100G_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_EXT_SLOTS_OCCUPIED_40G_set        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_SLOTS_OCCUPIED_40G_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_SLOTS_OCCUPIED_40G_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_SLOTS_OCCUPIED_40G_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etime_field_EXT_SLOTS_OCCUPIED_10G_set        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_EXT_SLOTS_OCCUPIED_10G_get        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_EXT_SLOTS_OCCUPIED_10G_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_EXT_SLOTS_OCCUPIED_10G_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------+
 | | Register 0x0000029c ENA_REG |
 | +-----------------------------+
 | | bit  1 R/W  LINE_REF_ENA    |
 | | bit  0 R/W  REFL_PATH_ENA   |
 | +-----------------------------+
 | 
 | void   etime_field_LINE_REF_ENA_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_LINE_REF_ENA_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_PATH_ENA_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_PATH_ENA_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x000002a0 REFL_DROP_ENA_CFG_REG |
 | +-------------------------------------------+
 | | bit  11 R/W  REFL_DROP_ENA_CH11           |
 | | bit  10 R/W  REFL_DROP_ENA_CH10           |
 | | bit  9  R/W  REFL_DROP_ENA_CH9            |
 | | bit  8  R/W  REFL_DROP_ENA_CH8            |
 | | bit  7  R/W  REFL_DROP_ENA_CH7            |
 | | bit  6  R/W  REFL_DROP_ENA_CH6            |
 | | bit  5  R/W  REFL_DROP_ENA_CH5            |
 | | bit  4  R/W  REFL_DROP_ENA_CH4            |
 | | bit  3  R/W  REFL_DROP_ENA_CH3            |
 | | bit  2  R/W  REFL_DROP_ENA_CH2            |
 | | bit  1  R/W  REFL_DROP_ENA_CH1            |
 | | bit  0  R/W  REFL_DROP_ENA_CH0            |
 | +-------------------------------------------+
 | 
 | void   etime_field_REFL_DROP_ENA_CH11_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DROP_ENA_CH11_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DROP_ENA_CH10_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DROP_ENA_CH10_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DROP_ENA_CH9_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DROP_ENA_CH9_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DROP_ENA_CH8_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DROP_ENA_CH8_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DROP_ENA_CH7_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DROP_ENA_CH7_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DROP_ENA_CH6_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DROP_ENA_CH6_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DROP_ENA_CH5_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DROP_ENA_CH5_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DROP_ENA_CH4_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DROP_ENA_CH4_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DROP_ENA_CH3_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DROP_ENA_CH3_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DROP_ENA_CH2_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DROP_ENA_CH2_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DROP_ENA_CH1_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DROP_ENA_CH1_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_REFL_DROP_ENA_CH0_set  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_REFL_DROP_ENA_CH0_get  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x000002a8 LINE_REF_CNT_DLY_REG |
 | +------------------------------------------+
 | | bit  31:0 R/W  LINE_REF_CNT_DLY          |
 | +------------------------------------------+
 | 
 | void   etime_field_LINE_REF_CNT_DLY_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_LINE_REF_CNT_DLY_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_LINE_REF_CNT_DLY_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_LINE_REF_CNT_DLY_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x000002ac EXT_PMON_LINK0 |
 | +------------------------------------+
 | | bit  11:0 R  REFL_PMON_0           |
 | +------------------------------------+
 | 
 | UINT32 etime_field_REFL_PMON_0_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_REFL_PMON_0_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x000002b0 EXT_PMON_LINK1 |
 | +------------------------------------+
 | | bit  11:0 R  REFL_PMON_1           |
 | +------------------------------------+
 | 
 | UINT32 etime_field_REFL_PMON_1_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_REFL_PMON_1_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x000002b4 EXT_PMON_LINK2 |
 | +------------------------------------+
 | | bit  11:0 R  REFL_PMON_2           |
 | +------------------------------------+
 | 
 | UINT32 etime_field_REFL_PMON_2_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_REFL_PMON_2_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x000002b8 EXT_PMON_LINK3 |
 | +------------------------------------+
 | | bit  11:0 R  REFL_PMON_3           |
 | +------------------------------------+
 | 
 | UINT32 etime_field_REFL_PMON_3_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_REFL_PMON_3_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x000002bc EXT_PMON_LINK4 |
 | +------------------------------------+
 | | bit  11:0 R  REFL_PMON_4           |
 | +------------------------------------+
 | 
 | UINT32 etime_field_REFL_PMON_4_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_REFL_PMON_4_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x000002c0 EXT_PMON_LINK5 |
 | +------------------------------------+
 | | bit  11:0 R  REFL_PMON_5           |
 | +------------------------------------+
 | 
 | UINT32 etime_field_REFL_PMON_5_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_REFL_PMON_5_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x000002c4 EXT_PMON_LINK6 |
 | +------------------------------------+
 | | bit  11:0 R  REFL_PMON_6           |
 | +------------------------------------+
 | 
 | UINT32 etime_field_REFL_PMON_6_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_REFL_PMON_6_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x000002c8 EXT_PMON_LINK7 |
 | +------------------------------------+
 | | bit  11:0 R  REFL_PMON_7           |
 | +------------------------------------+
 | 
 | UINT32 etime_field_REFL_PMON_7_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_REFL_PMON_7_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x000002cc EXT_PMON_LINK8 |
 | +------------------------------------+
 | | bit  11:0 R  REFL_PMON_8           |
 | +------------------------------------+
 | 
 | UINT32 etime_field_REFL_PMON_8_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_REFL_PMON_8_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x000002d0 EXT_PMON_LINK9 |
 | +------------------------------------+
 | | bit  11:0 R  REFL_PMON_9           |
 | +------------------------------------+
 | 
 | UINT32 etime_field_REFL_PMON_9_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_REFL_PMON_9_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000002d4 EXT_PMON_LINK10 |
 | +-------------------------------------+
 | | bit  11:0 R  REFL_PMON_10           |
 | +-------------------------------------+
 | 
 | UINT32 etime_field_REFL_PMON_10_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_REFL_PMON_10_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000002d8 EXT_PMON_LINK11 |
 | +-------------------------------------+
 | | bit  11:0 R  REFL_PMON_11           |
 | +-------------------------------------+
 | 
 | UINT32 etime_field_REFL_PMON_11_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | UINT32 etime_field_range_REFL_PMON_11_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x000002dc IRIGO_OFFS_S_REG |
 | +--------------------------------------+
 | | bit  15:0 R/W  IRIGO_OFFS_S          |
 | +--------------------------------------+
 | 
 | void   etime_field_IRIGO_OFFS_S_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGO_OFFS_S_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGO_OFFS_S_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGO_OFFS_S_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x000002e0 IRIGO_OFFS_NS_REG |
 | +---------------------------------------+
 | | bit  30:0 R/W  IRIGO_OFFS_NS          |
 | +---------------------------------------+
 | 
 | void   etime_field_IRIGO_OFFS_NS_set       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | UINT32 etime_field_IRIGO_OFFS_NS_get       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void   etime_field_range_IRIGO_OFFS_NS_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etime_field_range_IRIGO_OFFS_NS_get ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ETIME_IO_H
#define _ETIME_IO_H

#include "etime_loc.h"
#include "etime_regs.h"
#include "etime_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 etime_buffer_init               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | void                 etime_buffer_flush              ( etime_buffer_t *b_ptr )
 | UINT32               etime_reg_read                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 etime_reg_write                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 etime_field_set                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 etime_action_on_write_field_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 etime_burst_read                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 etime_burst_write               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE etime_poll                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 etime_reg_INT_BEHAV_CTRL_write                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_INT_BEHAV_CTRL_field_set                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_INT_BEHAV_CTRL_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_RAM_BIST_CTRL_write                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_RAM_BIST_CTRL_field_set                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_RAM_BIST_CTRL_read                               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_ING_47B_M_write                                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_ING_47B_M_field_set                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_ING_47B_M_read                                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_EGR_47B_M_write                                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_EGR_47B_M_field_set                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_EGR_47B_M_read                                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_ING_TS_ENA_REG_write                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_ING_TS_ENA_REG_field_set                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_ING_TS_ENA_REG_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_REFL_DEST_write                                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_REFL_DEST_field_set                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_REFL_DEST_read                                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_EGR_TS_ENA_REG_write                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_EGR_TS_ENA_REG_field_set                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_EGR_TS_ENA_REG_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_OFFS_SEC_HI_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_OFFS_SEC_HI_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_OFFS_SEC_HI_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_OFFS_SEC_LO_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_OFFS_SEC_LO_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_OFFS_SEC_LO_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_OFFS_NSEC_write                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_OFFS_NSEC_field_set                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_OFFS_NSEC_read                               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_OFFS_NSEC_HI_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_OFFS_NSEC_HI_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_OFFS_NSEC_HI_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_OFFS_NSEC_LO_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_OFFS_NSEC_LO_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_OFFS_NSEC_LO_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_ING_DLY_CH0_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_ING_DLY_CH0_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_ING_DLY_CH0_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_ING_DLY_CH1_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_ING_DLY_CH1_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_ING_DLY_CH1_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_ING_DLY_CH2_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_ING_DLY_CH2_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_ING_DLY_CH2_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_ING_DLY_CH3_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_ING_DLY_CH3_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_ING_DLY_CH3_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_ING_DLY_CH4_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_ING_DLY_CH4_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_ING_DLY_CH4_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_ING_DLY_CH5_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_ING_DLY_CH5_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_ING_DLY_CH5_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_ING_DLY_CH6_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_ING_DLY_CH6_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_ING_DLY_CH6_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_ING_DLY_CH7_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_ING_DLY_CH7_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_ING_DLY_CH7_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_ING_DLY_CH8_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_ING_DLY_CH8_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_ING_DLY_CH8_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_ING_DLY_CH9_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_ING_DLY_CH9_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_ING_DLY_CH9_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_ING_DLY_CH10_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_ING_DLY_CH10_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_ING_DLY_CH10_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_ING_DLY_CH11_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_ING_DLY_CH11_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_ING_DLY_CH11_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_ING_DLY_CH0_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_ING_DLY_CH0_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_ING_DLY_CH0_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_ING_DLY_CH1_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_ING_DLY_CH1_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_ING_DLY_CH1_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_ING_DLY_CH2_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_ING_DLY_CH2_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_ING_DLY_CH2_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_ING_DLY_CH3_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_ING_DLY_CH3_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_ING_DLY_CH3_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_ING_DLY_CH4_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_ING_DLY_CH4_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_ING_DLY_CH4_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_ING_DLY_CH5_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_ING_DLY_CH5_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_ING_DLY_CH5_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_ING_DLY_CH6_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_ING_DLY_CH6_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_ING_DLY_CH6_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_ING_DLY_CH7_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_ING_DLY_CH7_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_ING_DLY_CH7_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_ING_DLY_CH8_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_ING_DLY_CH8_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_ING_DLY_CH8_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_ING_DLY_CH9_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_ING_DLY_CH9_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_ING_DLY_CH9_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_ING_DLY_CH10_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_ING_DLY_CH10_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_ING_DLY_CH10_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_ING_DLY_CH11_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_ING_DLY_CH11_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_ING_DLY_CH11_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_EGR_DLY_CH0_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_EGR_DLY_CH0_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_EGR_DLY_CH0_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_EGR_DLY_CH1_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_EGR_DLY_CH1_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_EGR_DLY_CH1_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_EGR_DLY_CH2_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_EGR_DLY_CH2_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_EGR_DLY_CH2_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_EGR_DLY_CH3_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_EGR_DLY_CH3_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_EGR_DLY_CH3_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_EGR_DLY_CH4_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_EGR_DLY_CH4_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_EGR_DLY_CH4_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_EGR_DLY_CH5_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_EGR_DLY_CH5_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_EGR_DLY_CH5_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_EGR_DLY_CH6_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_EGR_DLY_CH6_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_EGR_DLY_CH6_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_EGR_DLY_CH7_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_EGR_DLY_CH7_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_EGR_DLY_CH7_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_EGR_DLY_CH8_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_EGR_DLY_CH8_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_EGR_DLY_CH8_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_EGR_DLY_CH9_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_EGR_DLY_CH9_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_EGR_DLY_CH9_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_EGR_DLY_CH10_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_EGR_DLY_CH10_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_EGR_DLY_CH10_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TOD_EGR_DLY_CH11_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TOD_EGR_DLY_CH11_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TOD_EGR_DLY_CH11_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_EGR_DLY_CH0_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_EGR_DLY_CH0_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_EGR_DLY_CH0_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_EGR_DLY_CH1_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_EGR_DLY_CH1_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_EGR_DLY_CH1_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_EGR_DLY_CH2_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_EGR_DLY_CH2_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_EGR_DLY_CH2_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_EGR_DLY_CH3_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_EGR_DLY_CH3_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_EGR_DLY_CH3_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_EGR_DLY_CH4_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_EGR_DLY_CH4_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_EGR_DLY_CH4_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_EGR_DLY_CH5_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_EGR_DLY_CH5_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_EGR_DLY_CH5_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_EGR_DLY_CH6_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_EGR_DLY_CH6_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_EGR_DLY_CH6_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_EGR_DLY_CH7_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_EGR_DLY_CH7_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_EGR_DLY_CH7_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_EGR_DLY_CH8_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_EGR_DLY_CH8_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_EGR_DLY_CH8_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_EGR_DLY_CH9_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_EGR_DLY_CH9_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_EGR_DLY_CH9_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_EGR_DLY_CH10_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_EGR_DLY_CH10_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_EGR_DLY_CH10_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CFC_EGR_DLY_CH11_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CFC_EGR_DLY_CH11_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CFC_EGR_DLY_CH11_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TS_CNTR_CFG_write                                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TS_CNTR_CFG_field_set                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TS_CNTR_CFG_read                                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_OFFS_UPD_TIME_SEC_HI_REG_write                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_OFFS_UPD_TIME_SEC_HI_REG_field_set               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_OFFS_UPD_TIME_SEC_HI_REG_read                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_OFFS_UPD_TIME_SEC_LO_REG_write                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_OFFS_UPD_TIME_SEC_LO_REG_field_set               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_OFFS_UPD_TIME_SEC_LO_REG_read                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_OFFS_UPD_TIME_NSEC_REG_write                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_OFFS_UPD_TIME_NSEC_REG_field_set                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_OFFS_UPD_TIME_NSEC_REG_read                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_PTP_CLK_PER_INT_HI_REG_write                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_PTP_CLK_PER_INT_HI_REG_field_set                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_PTP_CLK_PER_INT_HI_REG_read                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_PTP_CLK_PER_INT_LO_REG_write                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_PTP_CLK_PER_INT_LO_REG_field_set                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_PTP_CLK_PER_INT_LO_REG_read                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_PTP_CLK_PER_NUM_REG_write                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_PTP_CLK_PER_NUM_REG_field_set                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_PTP_CLK_PER_NUM_REG_read                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_PTP_CLK_PER_DEN_REG_write                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_PTP_CLK_PER_DEN_REG_field_set                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_PTP_CLK_PER_DEN_REG_read                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_OFFSET_UPD_QUANTA_REG_write                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_OFFSET_UPD_QUANTA_REG_field_set                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_OFFSET_UPD_QUANTA_REG_read                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_LINE_CLK_PER_INT_HI_REG_write                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_LINE_CLK_PER_INT_HI_REG_field_set                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_LINE_CLK_PER_INT_HI_REG_read                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_LINE_CLK_PER_INT_LO_REG_write                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_LINE_CLK_PER_INT_LO_REG_field_set                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_LINE_CLK_PER_INT_LO_REG_read                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_LINE_CLK_PER_NUM_REG_write                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_LINE_CLK_PER_NUM_REG_field_set                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_LINE_CLK_PER_NUM_REG_read                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_LINE_CLK_PER_DEN_REG_write                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_LINE_CLK_PER_DEN_REG_field_set                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_LINE_CLK_PER_DEN_REG_read                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TX_VEC_IRIGO_4_REG_write                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TX_VEC_IRIGO_4_REG_field_set                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TX_VEC_IRIGO_4_REG_read                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TX_VEC_IRIGO_3_REG_write                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TX_VEC_IRIGO_3_REG_field_set                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TX_VEC_IRIGO_3_REG_read                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TX_VEC_IRIGO_2_REG_write                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TX_VEC_IRIGO_2_REG_field_set                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TX_VEC_IRIGO_2_REG_read                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TX_VEC_IRIGO_1_REG_write                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TX_VEC_IRIGO_1_REG_field_set                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TX_VEC_IRIGO_1_REG_read                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGI_CFG_write                                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGI_CFG_field_set                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGI_CFG_read                                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGI_PWM_ZERO_REG_write                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGI_PWM_ZERO_REG_field_set                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGI_PWM_ZERO_REG_read                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGI_PWM_ONE_REG_write                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGI_PWM_ONE_REG_field_set                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGI_PWM_ONE_REG_read                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGI_PWM_WIDTH_PX_REG_write                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGI_PWM_WIDTH_PX_REG_field_set                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGI_PWM_WIDTH_PX_REG_read                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGI_PWM_MIN_DIFF_CFG_REG_write                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGI_PWM_MIN_DIFF_CFG_REG_field_set             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGI_PWM_MIN_DIFF_CFG_REG_read                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGI_PWM_MAX_DIFF_CFG_REG_write                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGI_PWM_MAX_DIFF_CFG_REG_field_set             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGI_PWM_MAX_DIFF_CFG_REG_read                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGI_DATI_IDLE_DURATION_write                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGI_DATI_IDLE_DURATION_field_set               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGI_DATI_IDLE_DURATION_read                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGO_CFG_write                                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGO_CFG_field_set                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGO_CFG_read                                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGO_PWM_NOM_PER_CFG_REG_write                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGO_PWM_NOM_PER_CFG_REG_field_set              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGO_PWM_NOM_PER_CFG_REG_read                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGO_PWM_ONE_REG_write                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGO_PWM_ONE_REG_field_set                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGO_PWM_ONE_REG_read                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGO_PWM_ZERO_REG_write                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGO_PWM_ZERO_REG_field_set                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGO_PWM_ZERO_REG_read                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGO_PWM_WIDTH_PX_REG_write                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGO_PWM_WIDTH_PX_REG_field_set                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGO_PWM_WIDTH_PX_REG_read                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGO_PWM_1PPS_WIDTH_REG_write                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGO_PWM_1PPS_WIDTH_REG_field_set               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGO_PWM_1PPS_WIDTH_REG_read                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_INT_GEN_CFG_write                                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_INT_GEN_CFG_field_set                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_INT_GEN_CFG_read                                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_INT_PER_CFG_write                                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_INT_PER_CFG_field_set                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_INT_PER_CFG_read                                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CC_INT_PER_HI_4_CFG_write                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CC_INT_PER_HI_4_CFG_field_set                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CC_INT_PER_HI_4_CFG_read                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CC_INT_PER_HI_3_CFG_write                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CC_INT_PER_HI_3_CFG_field_set                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CC_INT_PER_HI_3_CFG_read                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CC_INT_PER_HI_2_CFG_write                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CC_INT_PER_HI_2_CFG_field_set                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CC_INT_PER_HI_2_CFG_read                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CC_INT_PER_HI_1_CFG_write                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CC_INT_PER_HI_1_CFG_field_set                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CC_INT_PER_HI_1_CFG_read                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CC_INT_PER_4_CFG_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CC_INT_PER_4_CFG_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CC_INT_PER_4_CFG_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CC_INT_PER_3_CFG_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CC_INT_PER_3_CFG_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CC_INT_PER_3_CFG_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CC_INT_PER_2_CFG_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CC_INT_PER_2_CFG_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CC_INT_PER_2_CFG_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CC_INT_PER_1_CFG_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CC_INT_PER_1_CFG_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CC_INT_PER_1_CFG_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_REFL_DLY_CFG_write                               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_REFL_DLY_CFG_field_set                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_REFL_DLY_CFG_read                                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_INTERPOL_CFG_write                               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_INTERPOL_CFG_field_set                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_INTERPOL_CFG_read                                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_CAPT_OFFS_write                                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_CAPT_OFFS_field_set                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_CAPT_OFFS_read                                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_EXT_CPB_LSC_LOW_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_EXT_CPB_LSC_LOW_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_EXT_CPB_LSC_LOW_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_EXT_CPB_LSC_HIGH_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_EXT_CPB_LSC_HIGH_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_EXT_CPB_LSC_HIGH_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_EXT_CPB_CFG_write                                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_EXT_CPB_CFG_field_set                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_EXT_CPB_CFG_read                                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_EXT_NUM_SLOTS_OCCUPIED_write                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_EXT_NUM_SLOTS_OCCUPIED_field_set                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_EXT_NUM_SLOTS_OCCUPIED_read                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_ENA_REG_write                                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_ENA_REG_field_set                                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_ENA_REG_read                                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_REFL_DROP_ENA_CFG_REG_write                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_REFL_DROP_ENA_CFG_REG_field_set                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_REFL_DROP_ENA_CFG_REG_read                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_LINE_REF_CNT_DLY_REG_write                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_LINE_REF_CNT_DLY_REG_field_set                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_LINE_REF_CNT_DLY_REG_read                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGO_OFFS_S_REG_write                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGO_OFFS_S_REG_field_set                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGO_OFFS_S_REG_read                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_IRIGO_OFFS_NS_REG_write                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_IRIGO_OFFS_NS_REG_field_set                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_IRIGO_OFFS_NS_REG_read                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_REFLLRR_write                                    ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_REFLLRR_field_set                                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_REFLLRR_read                                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_EXT_LNK_SCH_UPD_write                            ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_EXT_LNK_SCH_UPD_field_set                        ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_EXT_LNK_SCH_UPD_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_SB_ECC_ERR_INT_EN_REG_write                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_SB_ECC_ERR_INT_EN_REG_field_set                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_SB_ECC_ERR_INT_EN_REG_read                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_MB_ECC_ERR_INT_EN_REG_write                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_MB_ECC_ERR_INT_EN_REG_field_set                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_MB_ECC_ERR_INT_EN_REG_read                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_REFL_OFLOW_ERR_INT_EN_REG_write                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_REFL_OFLOW_ERR_INT_EN_REG_field_set              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_REFL_OFLOW_ERR_INT_EN_REG_read                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_TS_DIS_INT_EN_REG_write                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TS_DIS_INT_EN_REG_field_set                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TS_DIS_INT_EN_REG_read                           ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_PACKET_DIS_INT_EN_REG_write                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_PACKET_DIS_INT_EN_REG_field_set                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_PACKET_DIS_INT_EN_REG_read                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_PACKET_RAM_ERR_INT_EN_REG_write                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_PACKET_RAM_ERR_INT_EN_REG_field_set              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_PACKET_RAM_ERR_INT_EN_REG_read                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_EXT_DESYNC_ERR_INT_EN_REG_write                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_EXT_DESYNC_ERR_INT_EN_REG_field_set              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_EXT_DESYNC_ERR_INT_EN_REG_read                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_GP_INT_EN_REG_write                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_GP_INT_EN_REG_field_set                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_GP_INT_EN_REG_read                               ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | void                 etime_reg_SB_ECC_ERR_INT_REG_write                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_SB_ECC_ERR_INT_REG_action_on_write_field_set     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_SB_ECC_ERR_INT_REG_read                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_SB_ECC_ERR_INT_REG_poll                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 etime_reg_MB_ECC_ERR_INT_REG_write                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_MB_ECC_ERR_INT_REG_action_on_write_field_set     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_MB_ECC_ERR_INT_REG_read                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_MB_ECC_ERR_INT_REG_poll                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 etime_reg_REFL_OFLOW_ERR_INT_REG_write                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_REFL_OFLOW_ERR_INT_REG_action_on_write_field_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_REFL_OFLOW_ERR_INT_REG_read                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_REFL_OFLOW_ERR_INT_REG_poll                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 etime_reg_TS_DIS_INT_REG_write                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_TS_DIS_INT_REG_action_on_write_field_set         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_TS_DIS_INT_REG_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_TS_DIS_INT_REG_poll                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 etime_reg_PACKET_DIS_INT_REG_write                         ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_PACKET_DIS_INT_REG_action_on_write_field_set     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_PACKET_DIS_INT_REG_read                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_PACKET_DIS_INT_REG_poll                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 etime_reg_PACKET_RAM_ERR_INT_REG_write                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_PACKET_RAM_ERR_INT_REG_action_on_write_field_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_PACKET_RAM_ERR_INT_REG_read                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_PACKET_RAM_ERR_INT_REG_poll                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 etime_reg_EXT_DESYNC_ERR_INT_REG_write                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_EXT_DESYNC_ERR_INT_REG_action_on_write_field_set ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_EXT_DESYNC_ERR_INT_REG_read                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_EXT_DESYNC_ERR_INT_REG_poll                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 etime_reg_GP_INT_REG_write                                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 value )
 | void                 etime_reg_GP_INT_REG_action_on_write_field_set             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etime_reg_GP_INT_REG_read                                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_GP_INT_REG_poll                                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               etime_reg_SB_ECC_ERR_INT_VAL_REG_read                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_SB_ECC_ERR_INT_VAL_REG_poll                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               etime_reg_MB_ECC_ERR_INT_VAL_REG_read                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_MB_ECC_ERR_INT_VAL_REG_poll                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               etime_reg_REFL_OFLOW_ERR_INT_VAL_REG_read                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_REFL_OFLOW_ERR_INT_VAL_REG_poll                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               etime_reg_TS_DIS_INT_VAL_REG_read                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_TS_DIS_INT_VAL_REG_poll                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               etime_reg_PACKET_DIS_INT_VAL_REG_read                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_PACKET_DIS_INT_VAL_REG_poll                      ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               etime_reg_PACKET_RAM_ERR_INT_VAL_REG_read                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_PACKET_RAM_ERR_INT_VAL_REG_poll                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               etime_reg_EXT_DESYNC_ERR_INT_VAL_REG_read                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_EXT_DESYNC_ERR_INT_VAL_REG_poll                  ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               etime_reg_GP_INT_VAL_REG_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etime_reg_GP_INT_VAL_REG_poll                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               etime_reg_PTP_CNT_read                                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_LINE_REF_CNT_read                                ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_TOD_TS_OUT_EXT_SEC_HI_REG_read                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_TOD_TS_OUT_EXT_SEC_LO_REG_read                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_TOD_TS_OUT_EXT_NSEC_REG_read                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_CFC_TS_OUT_EXT_HI_REG_read                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_CFC_TS_OUT_EXT_LO_REG_read                       ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_TOD_TS_OUT_IRIGI_SEC_HI_REG_read                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_TOD_TS_OUT_IRIGI_SEC_LO_REG_read                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_TOD_TS_OUT_IRIGI_NSEC_REG_read                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_CFC_TS_OUT_IRIGI_HI_REG_read                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_CFC_TS_OUT_IRIGI_LO_REG_read                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_TOD_TS_OUT_IRIGO_SEC_HI_REG_read                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_TOD_TS_OUT_IRIGO_SEC_LO_REG_read                 ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_TOD_TS_OUT_IRIGO_NSEC_REG_read                   ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_CFC_TS_OUT_IRIGO_HI_REG_read                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_CFC_TS_OUT_IRIGO_LO_REG_read                     ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_RX_VEC_IRIGI_4_REG_read                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_RX_VEC_IRIGI_3_REG_read                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_RX_VEC_IRIGI_2_REG_read                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_RX_VEC_IRIGI_1_REG_read                          ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_EXT_PMON_LINK0_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_EXT_PMON_LINK1_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_EXT_PMON_LINK2_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_EXT_PMON_LINK3_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_EXT_PMON_LINK4_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_EXT_PMON_LINK5_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_EXT_PMON_LINK6_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_EXT_PMON_LINK7_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_EXT_PMON_LINK8_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_EXT_PMON_LINK9_read                              ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_EXT_PMON_LINK10_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 | UINT32               etime_reg_EXT_PMON_LINK11_read                             ( etime_buffer_t *b_ptr, etime_handle_t *h_ptr )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ETIME_IO_H */
