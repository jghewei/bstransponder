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
 *     and register accessor functions for the opsm23 block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing opsm23 io functions is:
 | {
 |     opsm23_buffer_t b_ptr[1];
 |     opsm23_buffer_init( b_ptr, h_ptr, A );
 |     value = opsm23_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     opsm23_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     opsm23_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = opsm23_field_<FIELD>_get( NULL, h_ptr, A );
 | opsm23_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------------+
 | | Register 0x00011004 TX_GLOBAL_CTRL_REG |
 | +----------------------------------------+
 | | bit  20    R/W  TX_DATA_4X10G_MODE     |
 | | bit  19    R/W  TX_OTL_3_4_ENABLE      |
 | | bit  18:15 R/W  TX_SCRAMBLE_ENABLE     |
 | | bit  14    R/W  TX_HARDWARE_RESET      |
 | | bit  13:10 R/W  TX_SYS_LOOPBACK        |
 | | bit  7:4   R/W  TX_DATA_IN_SCRAMBLED   |
 | | bit  3:0   R/W  TX_LINE_SOFT_RESET     |
 | +----------------------------------------+
 | 
 | void   opsm23_field_TX_DATA_4X10G_MODE_set         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_DATA_4X10G_MODE_get         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_TX_OTL_3_4_ENABLE_set          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_OTL_3_4_ENABLE_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_TX_SCRAMBLE_ENABLE_set         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_SCRAMBLE_ENABLE_get         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_TX_SCRAMBLE_ENABLE_set   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_TX_SCRAMBLE_ENABLE_get   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   opsm23_field_TX_HARDWARE_RESET_set          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_HARDWARE_RESET_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_TX_SYS_LOOPBACK_set            ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_SYS_LOOPBACK_get            ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_TX_SYS_LOOPBACK_set      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_TX_SYS_LOOPBACK_get      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   opsm23_field_TX_DATA_IN_SCRAMBLED_set       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_DATA_IN_SCRAMBLED_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_TX_DATA_IN_SCRAMBLED_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_TX_DATA_IN_SCRAMBLED_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   opsm23_field_TX_LINE_SOFT_RESET_set         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_LINE_SOFT_RESET_get         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_TX_LINE_SOFT_RESET_set   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_TX_LINE_SOFT_RESET_get   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x0001100c TX_INT_REG               |
 | +----------------------------------------------+
 | | bit  25    R/W  TX_SFI_FIFO_OVERFLOW_INT_I   |
 | | bit  24    R/W  TX_SFI_FIFO_UNDERFLOW_INT_I  |
 | | bit  23:20 R/W  TX_SYS_FIFO_OVERFLOW_INT_I   |
 | | bit  19:16 R/W  TX_SYS_FIFO_UNDERFLOW_INT_I  |
 | | bit  7:4   R/W  TX_LANE_FIFO_OVERFLOW_INT_I  |
 | | bit  3:0   R/W  TX_LANE_FIFO_UNDERFLOW_INT_I |
 | +----------------------------------------------+
 | 
 | void                 opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_get                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_I_poll                 ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_get                 ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_I_poll                ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_get                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void                 opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_get            ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_I_poll           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_I_poll                 ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_get                 ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void                 opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_get           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_I_poll          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_I_poll                ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_get                 ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void                 opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_get           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_poll          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_I_poll                ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_get                ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void                 opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_poll         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_I_poll               ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------+
 | | Register 0x00011010 TX_INT_EN_REG            |
 | +----------------------------------------------+
 | | bit  25    R/W  TX_SFI_FIFO_OVERFLOW_INT_E   |
 | | bit  24    R/W  TX_SFI_FIFO_UNDERFLOW_INT_E  |
 | | bit  23:20 R/W  TX_SYS_FIFO_OVERFLOW_INT_E   |
 | | bit  19:16 R/W  TX_SYS_FIFO_UNDERFLOW_INT_E  |
 | | bit  7:4   R/W  TX_LANE_FIFO_OVERFLOW_INT_E  |
 | | bit  3:0   R/W  TX_LANE_FIFO_UNDERFLOW_INT_E |
 | +----------------------------------------------+
 | 
 | void   opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_E_set         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_E_get         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_E_set        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_E_get        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_E_set         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_E_get         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_E_set   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_E_get   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_E_set        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_E_get        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_E_set  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_E_get  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_E_set        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_E_get        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_set  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_get  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_E_set       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_E_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00011014 TX_INT_VAL_REG         |
 | +--------------------------------------------+
 | | bit  25    R  TX_SFI_FIFO_OVERFLOW_INT_V   |
 | | bit  24    R  TX_SFI_FIFO_UNDERFLOW_INT_V  |
 | | bit  23:20 R  TX_SYS_FIFO_OVERFLOW_INT_V   |
 | | bit  19:16 R  TX_SYS_FIFO_UNDERFLOW_INT_V  |
 | | bit  7:4   R  TX_LANE_FIFO_OVERFLOW_INT_V  |
 | | bit  3:0   R  TX_LANE_FIFO_UNDERFLOW_INT_V |
 | +--------------------------------------------+
 | 
 | UINT32               opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_V_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_TX_SFI_FIFO_OVERFLOW_INT_V_poll         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_V_get         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_TX_SFI_FIFO_UNDERFLOW_INT_V_poll        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_V_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32               opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_V_get    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE opsm23_field_range_TX_SYS_FIFO_OVERFLOW_INT_V_poll   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE opsm23_field_TX_SYS_FIFO_OVERFLOW_INT_V_poll         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_V_get         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32               opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_V_get   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE opsm23_field_range_TX_SYS_FIFO_UNDERFLOW_INT_V_poll  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE opsm23_field_TX_SYS_FIFO_UNDERFLOW_INT_V_poll        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_V_get         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32               opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_get   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE opsm23_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_poll  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE opsm23_field_TX_LANE_FIFO_OVERFLOW_INT_V_poll        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_V_get        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32               opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_get  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE opsm23_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_poll ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE opsm23_field_TX_LANE_FIFO_UNDERFLOW_INT_V_poll       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------+
 | | Register 0x0001101c TX_L2P_LANE_MAPPING_REG |
 | +---------------------------------------------+
 | | bit  7:6 R/W  TX_PHYSICAL_LANE_3_SELECT     |
 | | bit  5:4 R/W  TX_PHYSICAL_LANE_2_SELECT     |
 | | bit  3:2 R/W  TX_PHYSICAL_LANE_1_SELECT     |
 | | bit  1:0 R/W  TX_PHYSICAL_LANE_0_SELECT     |
 | +---------------------------------------------+
 | 
 | void   opsm23_field_TX_PHYSICAL_LANE_3_SELECT_set       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_PHYSICAL_LANE_3_SELECT_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_TX_PHYSICAL_LANE_3_SELECT_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_TX_PHYSICAL_LANE_3_SELECT_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   opsm23_field_TX_PHYSICAL_LANE_2_SELECT_set       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_PHYSICAL_LANE_2_SELECT_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_TX_PHYSICAL_LANE_2_SELECT_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_TX_PHYSICAL_LANE_2_SELECT_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   opsm23_field_TX_PHYSICAL_LANE_1_SELECT_set       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_PHYSICAL_LANE_1_SELECT_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_TX_PHYSICAL_LANE_1_SELECT_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_TX_PHYSICAL_LANE_1_SELECT_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   opsm23_field_TX_PHYSICAL_LANE_0_SELECT_set       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_PHYSICAL_LANE_0_SELECT_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_TX_PHYSICAL_LANE_0_SELECT_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_TX_PHYSICAL_LANE_0_SELECT_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00011020 TX_SYNC_FIFO_XOFF_TH       |
 | +------------------------------------------------+
 | | bit  15:8 R/W  TX_SYS_FIFO_XOFF_SET_FILL_LEVEL |
 | +------------------------------------------------+
 | 
 | void   opsm23_field_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_set       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_TX_SYS_FIFO_XOFF_SET_FILL_LEVEL_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x0001102c TX_FRAME_PULSE_GEN_CONFIG_REG |
 | +---------------------------------------------------+
 | | bit  19:16 R/W  TX_FP_MFAS_INVERT                 |
 | +---------------------------------------------------+
 | 
 | void   opsm23_field_TX_FP_MFAS_INVERT_set       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_TX_FP_MFAS_INVERT_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_TX_FP_MFAS_INVERT_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_TX_FP_MFAS_INVERT_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00011080 RX_GLOBAL_CTRL_REG   |
 | +------------------------------------------+
 | | bit  20    R/W  RX_DATA_4X10G_MODE       |
 | | bit  19    R/W  RX_OTL_3_4_ENABLE        |
 | | bit  18:15 R/W  RX_SCRAMBLE_ENABLE       |
 | | bit  14    R/W  RX_HARDWARE_RESET        |
 | | bit  13:10 R/W  RX_SYS_LOOPBACK          |
 | | bit  9:6   R/W  RX_DATA_IN_NOT_SCRAMBLED |
 | | bit  3:0   R/W  RX_LINE_SOFT_RESET       |
 | +------------------------------------------+
 | 
 | void   opsm23_field_RX_DATA_4X10G_MODE_set             ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_DATA_4X10G_MODE_get             ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_RX_OTL_3_4_ENABLE_set              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_OTL_3_4_ENABLE_get              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_RX_SCRAMBLE_ENABLE_set             ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_SCRAMBLE_ENABLE_get             ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_RX_SCRAMBLE_ENABLE_set       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_RX_SCRAMBLE_ENABLE_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   opsm23_field_RX_HARDWARE_RESET_set              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_HARDWARE_RESET_get              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_RX_SYS_LOOPBACK_set                ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_SYS_LOOPBACK_get                ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_RX_SYS_LOOPBACK_set          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_RX_SYS_LOOPBACK_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   opsm23_field_RX_DATA_IN_NOT_SCRAMBLED_set       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_DATA_IN_NOT_SCRAMBLED_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_RX_DATA_IN_NOT_SCRAMBLED_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   opsm23_field_RX_LINE_SOFT_RESET_set             ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_LINE_SOFT_RESET_get             ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_RX_LINE_SOFT_RESET_set       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_RX_LINE_SOFT_RESET_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00011084 RX_OTU3_CTRL_REG      |
 | +-------------------------------------------+
 | | bit  7:0 R/W  RX_LOF_TRANSITION_OUT_COUNT |
 | +-------------------------------------------+
 | 
 | void   opsm23_field_RX_LOF_TRANSITION_OUT_COUNT_set       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_LOF_TRANSITION_OUT_COUNT_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_RX_LOF_TRANSITION_OUT_COUNT_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_RX_LOF_TRANSITION_OUT_COUNT_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00011088 RX_OTU3_INT_REG        |
 | +--------------------------------------------+
 | | bit  15 R/W  RX_SFI_LOS_INT_I              |
 | | bit  14 R/W  RX_SFI_OOF_INT_I              |
 | | bit  13 R/W  RX_SFI_LOF_INT_I              |
 | | bit  12 R/W  RX_OTU_OOF_INT_I              |
 | | bit  11 R/W  RX_OTU_LOF_INT_I              |
 | | bit  9  R/W  RX_SFI_AIS_INT_I              |
 | | bit  8  R/W  RX_LOL_INT_I                  |
 | | bit  7  R/W  RX_SKEW_RANGE_VIOLATION_INT_I |
 | | bit  6  R/W  RX_ALIGN_CHANGE_INT_I         |
 | | bit  5  R/W  RX_DESKEW_CHANGE_INT_I        |
 | | bit  4  R/W  RX_SFI_FIFO_OVERFLOW_INT_I    |
 | | bit  3  R/W  RX_SFI_FIFO_UNDERFLOW_INT_I   |
 | +--------------------------------------------+
 | 
 | void                 opsm23_field_RX_SFI_LOS_INT_I_set_to_clear              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_RX_SFI_LOS_INT_I_get                       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_LOS_INT_I_poll                      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_SFI_OOF_INT_I_set_to_clear              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_RX_SFI_OOF_INT_I_get                       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_OOF_INT_I_poll                      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_SFI_LOF_INT_I_set_to_clear              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_RX_SFI_LOF_INT_I_get                       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_LOF_INT_I_poll                      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_OTU_OOF_INT_I_set_to_clear              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_RX_OTU_OOF_INT_I_get                       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_OTU_OOF_INT_I_poll                      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_OTU_LOF_INT_I_set_to_clear              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_RX_OTU_LOF_INT_I_get                       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_OTU_LOF_INT_I_poll                      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_SFI_AIS_INT_I_set_to_clear              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_RX_SFI_AIS_INT_I_get                       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_AIS_INT_I_poll                      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_LOL_INT_I_set_to_clear                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_RX_LOL_INT_I_get                           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LOL_INT_I_poll                          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_set_to_clear ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_I_poll         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_ALIGN_CHANGE_INT_I_set_to_clear         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_RX_ALIGN_CHANGE_INT_I_get                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_ALIGN_CHANGE_INT_I_poll                 ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_DESKEW_CHANGE_INT_I_set_to_clear        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_RX_DESKEW_CHANGE_INT_I_get                 ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_DESKEW_CHANGE_INT_I_poll                ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_get             ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_I_poll            ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_get            ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_I_poll           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x0001108c RX_OTU3_INT_EN_REG     |
 | +--------------------------------------------+
 | | bit  15 R/W  RX_SFI_LOS_INT_E              |
 | | bit  14 R/W  RX_SFI_OOF_INT_E              |
 | | bit  13 R/W  RX_SFI_LOF_INT_E              |
 | | bit  12 R/W  RX_OTU_OOF_INT_E              |
 | | bit  11 R/W  RX_OTU_LOF_INT_E              |
 | | bit  9  R/W  RX_SFI_AIS_INT_E              |
 | | bit  8  R/W  RX_LOL_INT_E                  |
 | | bit  7  R/W  RX_SKEW_RANGE_VIOLATION_INT_E |
 | | bit  6  R/W  RX_ALIGN_CHANGE_INT_E         |
 | | bit  5  R/W  RX_DESKEW_CHANGE_INT_E        |
 | | bit  4  R/W  RX_SFI_FIFO_OVERFLOW_INT_E    |
 | | bit  3  R/W  RX_SFI_FIFO_UNDERFLOW_INT_E   |
 | +--------------------------------------------+
 | 
 | void   opsm23_field_RX_SFI_LOS_INT_E_set              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_SFI_LOS_INT_E_get              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_RX_SFI_OOF_INT_E_set              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_SFI_OOF_INT_E_get              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_RX_SFI_LOF_INT_E_set              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_SFI_LOF_INT_E_get              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_RX_OTU_OOF_INT_E_set              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_OTU_OOF_INT_E_get              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_RX_OTU_LOF_INT_E_set              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_OTU_LOF_INT_E_get              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_RX_SFI_AIS_INT_E_set              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_SFI_AIS_INT_E_get              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_RX_LOL_INT_E_set                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_LOL_INT_E_get                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_E_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_E_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_RX_ALIGN_CHANGE_INT_E_set         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_ALIGN_CHANGE_INT_E_get         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_RX_DESKEW_CHANGE_INT_E_set        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_DESKEW_CHANGE_INT_E_get        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_E_set    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_E_get    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_E_set   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_E_get   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------------+
 | | Register 0x00011090 RX_OTU3_INT_VAL_REG  |
 | +------------------------------------------+
 | | bit  15 R  RX_SFI_LOS_INT_V              |
 | | bit  14 R  RX_SFI_OOF_INT_V              |
 | | bit  13 R  RX_SFI_LOF_INT_V              |
 | | bit  12 R  RX_OTU_OOF_INT_V              |
 | | bit  11 R  RX_OTU_LOF_INT_V              |
 | | bit  9  R  RX_SFI_AIS_INT_V              |
 | | bit  8  R  RX_LOL_INT_V                  |
 | | bit  7  R  RX_SKEW_RANGE_VIOLATION_INT_V |
 | | bit  6  R  RX_ALIGN_CHANGE_INT_V         |
 | | bit  5  R  RX_DESKEW_CHANGE_INT_V        |
 | | bit  4  R  RX_SFI_FIFO_OVERFLOW_INT_V    |
 | | bit  3  R  RX_SFI_FIFO_UNDERFLOW_INT_V   |
 | +------------------------------------------+
 | 
 | UINT32               opsm23_field_RX_SFI_LOS_INT_V_get               ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_LOS_INT_V_poll              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_SFI_OOF_INT_V_get               ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_OOF_INT_V_poll              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_SFI_LOF_INT_V_get               ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_LOF_INT_V_poll              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_OTU_OOF_INT_V_get               ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_OTU_OOF_INT_V_poll              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_OTU_LOF_INT_V_get               ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_OTU_LOF_INT_V_poll              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_SFI_AIS_INT_V_get               ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_AIS_INT_V_poll              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_LOL_INT_V_get                   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LOL_INT_V_poll                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_V_get  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_SKEW_RANGE_VIOLATION_INT_V_poll ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_ALIGN_CHANGE_INT_V_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_ALIGN_CHANGE_INT_V_poll         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_DESKEW_CHANGE_INT_V_get         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_DESKEW_CHANGE_INT_V_poll        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_V_get     ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_FIFO_OVERFLOW_INT_V_poll    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_V_get    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_SFI_FIFO_UNDERFLOW_INT_V_poll   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------------------------+
 | | Register (0x00011094 + (N) * 0x4) RX_LANE_SSF_FAIL_OR_CTRL_REG |
 | |     N[0..3]                                                    |
 | +----------------------------------------------------------------+
 | | bit  15 R/W  RX_SSF_LANE_MI_ACTIVE                             |
 | | bit  14 R/W  RX_SSF_LANE_LOS                                   |
 | | bit  13 R/W  RX_SSF_LANE_LOF_LOFLANE                           |
 | | bit  11 R/W  RX_SSF_LANE_OTUK_AIS                              |
 | | bit  10 R/W  RX_SSF_LANE_OTL_AIS                               |
 | | bit  9  R/W  RX_SSF_LOL                                        |
 | | bit  8  R/W  RX_SSF_OTU3_LOF                                   |
 | | bit  7  R/W  RX_FEC_LANE_MI_ACTIVE                             |
 | | bit  6  R/W  RX_FAIL_LANE_LOS                                  |
 | | bit  5  R/W  RX_FAIL_LANE_LOF_LOFLANE                          |
 | | bit  3  R/W  RX_FAIL_LANE_OTUK_AIS                             |
 | | bit  2  R/W  RX_FAIL_LANE_OTL_AIS                              |
 | | bit  1  R/W  RX_FAIL_LOL                                       |
 | | bit  0  R/W  RX_FAIL_OTU3_LOF                                  |
 | +----------------------------------------------------------------+
 | 
 | void   opsm23_field_RX_SSF_LANE_MI_ACTIVE_set    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_SSF_LANE_MI_ACTIVE_get    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_SSF_LANE_LOS_set          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_SSF_LANE_LOS_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_SSF_LANE_LOF_LOFLANE_set  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_SSF_LANE_LOF_LOFLANE_get  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_SSF_LANE_OTUK_AIS_set     ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_SSF_LANE_OTUK_AIS_get     ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_SSF_LANE_OTL_AIS_set      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_SSF_LANE_OTL_AIS_get      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_SSF_LOL_set               ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_SSF_LOL_get               ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_SSF_OTU3_LOF_set          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_SSF_OTU3_LOF_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_FEC_LANE_MI_ACTIVE_set    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_FEC_LANE_MI_ACTIVE_get    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_FAIL_LANE_LOS_set         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_FAIL_LANE_LOS_get         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_FAIL_LANE_LOF_LOFLANE_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_FAIL_LANE_OTUK_AIS_set    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_FAIL_LANE_OTUK_AIS_get    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_FAIL_LANE_OTL_AIS_set     ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_FAIL_LANE_OTL_AIS_get     ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_FAIL_LOL_set              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_FAIL_LOL_get              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_FAIL_OTU3_LOF_set         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_FAIL_OTU3_LOF_get         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +----------------------------------------------------------+
 | | Register (0x000110a4 + (N) * 0x4) RX_LANE_ID_OR_SKEW_REG |
 | |     N[0..3]                                              |
 | +----------------------------------------------------------+
 | | bit  1:0 R  RX_LOGICAL_LANE_IDENTIFIER                   |
 | +----------------------------------------------------------+
 | 
 | UINT32 opsm23_field_RX_LOGICAL_LANE_IDENTIFIER_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 opsm23_field_range_RX_LOGICAL_LANE_IDENTIFIER_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x000110b4 + (N) * 0x4) RX_LANE_INT_REG |
 | |     N[0..3]                                       |
 | +---------------------------------------------------+
 | | bit  15 R/W  RX_LANE_LOS_INT_I                    |
 | | bit  14 R/W  RX_LANE_OOF_INT_I                    |
 | | bit  13 R/W  RX_LANE_LOF_LOFLANE_INT_I            |
 | | bit  10 R/W  RX_LANE_OTUK_AIS_INT_I               |
 | | bit  9  R/W  RX_LANE_OTL_AIS_INT_I                |
 | | bit  7  R/W  RX_LANE_ALIGN_CHANGE_INT_I           |
 | | bit  5  R/W  RX_LANE_FIFO_ERROR_INT_I             |
 | | bit  2  R/W  RX_LANE_OOR_INT_I                    |
 | | bit  1  R/W  RX_LANE_LOR_INT_I                    |
 | +---------------------------------------------------+
 | 
 | void                 opsm23_field_RX_LANE_LOS_INT_I_set_to_clear          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               opsm23_field_RX_LANE_LOS_INT_I_get                   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOS_INT_I_poll                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_LANE_OOF_INT_I_set_to_clear          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               opsm23_field_RX_LANE_OOF_INT_I_get                   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OOF_INT_I_poll                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_set_to_clear  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_get           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOF_LOFLANE_INT_I_poll          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_LANE_OTUK_AIS_INT_I_set_to_clear     ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               opsm23_field_RX_LANE_OTUK_AIS_INT_I_get              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OTUK_AIS_INT_I_poll             ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_LANE_OTL_AIS_INT_I_set_to_clear      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               opsm23_field_RX_LANE_OTL_AIS_INT_I_get               ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OTL_AIS_INT_I_poll              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_set_to_clear ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_ALIGN_CHANGE_INT_I_poll         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_LANE_FIFO_ERROR_INT_I_set_to_clear   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               opsm23_field_RX_LANE_FIFO_ERROR_INT_I_get            ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_FIFO_ERROR_INT_I_poll           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_LANE_OOR_INT_I_set_to_clear          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               opsm23_field_RX_LANE_OOR_INT_I_get                   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OOR_INT_I_poll                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 opsm23_field_RX_LANE_LOR_INT_I_set_to_clear          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32               opsm23_field_RX_LANE_LOR_INT_I_get                   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOR_INT_I_poll                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------------------+
 | | Register (0x000110c4 + (N) * 0x4) RX_LANE_INT_EN_REG |
 | |     N[0..3]                                          |
 | +------------------------------------------------------+
 | | bit  15 R/W  RX_LANE_LOS_INT_E                       |
 | | bit  14 R/W  RX_LANE_OOF_INT_E                       |
 | | bit  13 R/W  RX_LANE_LOF_LOFLANE_INT_E               |
 | | bit  10 R/W  RX_LANE_OTUK_AIS_INT_E                  |
 | | bit  9  R/W  RX_LANE_OTL_AIS_INT_E                   |
 | | bit  7  R/W  RX_LANE_ALIGN_CHANGE_INT_E              |
 | | bit  5  R/W  RX_LANE_FIFO_ERROR_INT_E                |
 | | bit  2  R/W  RX_LANE_OOR_INT_E                       |
 | | bit  1  R/W  RX_LANE_LOR_INT_E                       |
 | +------------------------------------------------------+
 | 
 | void   opsm23_field_RX_LANE_LOS_INT_E_set          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_LANE_LOS_INT_E_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_LANE_OOF_INT_E_set          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_LANE_OOF_INT_E_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_LANE_LOF_LOFLANE_INT_E_set  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_LANE_LOF_LOFLANE_INT_E_get  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_LANE_OTUK_AIS_INT_E_set     ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_LANE_OTUK_AIS_INT_E_get     ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_LANE_OTL_AIS_INT_E_set      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_LANE_OTL_AIS_INT_E_get      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_LANE_ALIGN_CHANGE_INT_E_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_LANE_ALIGN_CHANGE_INT_E_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_LANE_FIFO_ERROR_INT_E_set   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_LANE_FIFO_ERROR_INT_E_get   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_LANE_OOR_INT_E_set          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_LANE_OOR_INT_E_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   opsm23_field_RX_LANE_LOR_INT_E_set          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 opsm23_field_RX_LANE_LOR_INT_E_get          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +-------------------------------------------------------+
 | | Register (0x000110d4 + (N) * 0x4) RX_LANE_INT_VAL_REG |
 | |     N[0..3]                                           |
 | +-------------------------------------------------------+
 | | bit  15 R  RX_LANE_LOS_INT_V                          |
 | | bit  14 R  RX_LANE_OOF_INT_V                          |
 | | bit  13 R  RX_LANE_LOF_LOFLANE_INT_V                  |
 | | bit  10 R  RX_LANE_OTUK_AIS_INT_V                     |
 | | bit  9  R  RX_LANE_OTL_AIS_INT_V                      |
 | | bit  7  R  RX_LANE_ALIGN_CHANGE_INT_V                 |
 | | bit  5  R  RX_LANE_FIFO_ERROR_INT_V                   |
 | | bit  2  R  RX_LANE_OOR_INT_V                          |
 | | bit  1  R  RX_LANE_LOR_INT_V                          |
 | +-------------------------------------------------------+
 | 
 | UINT32               opsm23_field_RX_LANE_LOS_INT_V_get           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOS_INT_V_poll          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_LANE_OOF_INT_V_get           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OOF_INT_V_poll          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_LANE_LOF_LOFLANE_INT_V_get   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOF_LOFLANE_INT_V_poll  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_LANE_OTUK_AIS_INT_V_get      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OTUK_AIS_INT_V_poll     ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_LANE_OTL_AIS_INT_V_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OTL_AIS_INT_V_poll      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_LANE_ALIGN_CHANGE_INT_V_get  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_ALIGN_CHANGE_INT_V_poll ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_LANE_FIFO_ERROR_INT_V_get    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_FIFO_ERROR_INT_V_poll   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_LANE_OOR_INT_V_get           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_OOR_INT_V_poll          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               opsm23_field_RX_LANE_LOR_INT_V_get           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_field_RX_LANE_LOR_INT_V_poll          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------+
 | | Register 0x000110e4 RX_FP_CONFIG_REG |
 | +--------------------------------------+
 | | bit  12:9 R/W  RX_FP_MFAS_GEN_INVERT |
 | | bit  8:5  R/W  RX_FP_MFAS_INVERT     |
 | +--------------------------------------+
 | 
 | void   opsm23_field_RX_FP_MFAS_GEN_INVERT_set       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_FP_MFAS_GEN_INVERT_get       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_RX_FP_MFAS_GEN_INVERT_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_RX_FP_MFAS_GEN_INVERT_get ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   opsm23_field_RX_FP_MFAS_INVERT_set           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 opsm23_field_RX_FP_MFAS_INVERT_get           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   opsm23_field_range_RX_FP_MFAS_INVERT_set     ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 opsm23_field_range_RX_FP_MFAS_INVERT_get     ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00011118 OPSM23_INT_SUMMARY |
 | +----------------------------------------+
 | | bit  7:0 R  INTERRUPT_SUMMARY          |
 | +----------------------------------------+
 | 
 | UINT32               opsm23_field_INTERRUPT_SUMMARY_get        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32               opsm23_field_range_INTERRUPT_SUMMARY_get  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE opsm23_field_range_INTERRUPT_SUMMARY_poll ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE opsm23_field_INTERRUPT_SUMMARY_poll       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _OPSM23_IO_H
#define _OPSM23_IO_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "opsm23_regs.h"
#include "opsm23_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 opsm23_buffer_init               ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void                 opsm23_buffer_flush              ( opsm23_buffer_t *b_ptr )
 | UINT32               opsm23_reg_read                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 opsm23_reg_write                 ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 opsm23_field_set                 ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 opsm23_action_on_write_field_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 opsm23_burst_read                ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 opsm23_burst_write               ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE opsm23_poll                      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 opsm23_reg_TX_GLOBAL_CTRL_REG_write                        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 opsm23_reg_TX_GLOBAL_CTRL_REG_field_set                    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               opsm23_reg_TX_GLOBAL_CTRL_REG_read                         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 opsm23_reg_TX_L2P_LANE_MAPPING_REG_write                   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 opsm23_reg_TX_L2P_LANE_MAPPING_REG_field_set               ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               opsm23_reg_TX_L2P_LANE_MAPPING_REG_read                    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 opsm23_reg_TX_SYNC_FIFO_XOFF_TH_write                      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 opsm23_reg_TX_SYNC_FIFO_XOFF_TH_field_set                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               opsm23_reg_TX_SYNC_FIFO_XOFF_TH_read                       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_write             ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_field_set         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               opsm23_reg_TX_FRAME_PULSE_GEN_CONFIG_REG_read              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 opsm23_reg_RX_GLOBAL_CTRL_REG_write                        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 opsm23_reg_RX_GLOBAL_CTRL_REG_field_set                    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               opsm23_reg_RX_GLOBAL_CTRL_REG_read                         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 opsm23_reg_RX_OTU3_CTRL_REG_write                          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 opsm23_reg_RX_OTU3_CTRL_REG_field_set                      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               opsm23_reg_RX_OTU3_CTRL_REG_read                           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 opsm23_reg_RX_FP_CONFIG_REG_write                          ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 opsm23_reg_RX_FP_CONFIG_REG_field_set                      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               opsm23_reg_RX_FP_CONFIG_REG_read                           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_write        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               opsm23_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 opsm23_reg_TX_INT_EN_REG_write                             ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 opsm23_reg_TX_INT_EN_REG_field_set                         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               opsm23_reg_TX_INT_EN_REG_read                              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 opsm23_reg_RX_OTU3_INT_EN_REG_write                        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 opsm23_reg_RX_OTU3_INT_EN_REG_field_set                    ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               opsm23_reg_RX_OTU3_INT_EN_REG_read                         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 opsm23_reg_RX_LANE_INT_EN_REG_array_write                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 opsm23_reg_RX_LANE_INT_EN_REG_array_field_set              ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               opsm23_reg_RX_LANE_INT_EN_REG_array_read                   ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32               opsm23_reg_OPSM23_INT_SUMMARY_read                         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_reg_OPSM23_INT_SUMMARY_poll                         ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 opsm23_reg_TX_INT_REG_write                                ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 opsm23_reg_TX_INT_REG_action_on_write_field_set            ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               opsm23_reg_TX_INT_REG_read                                 ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_reg_TX_INT_REG_poll                                 ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 opsm23_reg_RX_OTU3_INT_REG_write                           ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 opsm23_reg_RX_OTU3_INT_REG_action_on_write_field_set       ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               opsm23_reg_RX_OTU3_INT_REG_read                            ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_reg_RX_OTU3_INT_REG_poll                            ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 opsm23_reg_RX_LANE_INT_REG_array_write                     ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 opsm23_reg_RX_LANE_INT_REG_array_action_on_write_field_set ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               opsm23_reg_RX_LANE_INT_REG_array_read                      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_reg_RX_LANE_INT_REG_array_poll                      ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               opsm23_reg_TX_INT_VAL_REG_read                             ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_reg_TX_INT_VAL_REG_poll                             ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               opsm23_reg_RX_OTU3_INT_VAL_REG_read                        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE opsm23_reg_RX_OTU3_INT_VAL_REG_poll                        ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               opsm23_reg_RX_LANE_INT_VAL_REG_array_read                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | PMC_POLL_RETURN_TYPE opsm23_reg_RX_LANE_INT_VAL_REG_array_poll                  ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               opsm23_reg_RX_LANE_ID_OR_SKEW_REG_array_read               ( opsm23_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _OPSM23_IO_H */
