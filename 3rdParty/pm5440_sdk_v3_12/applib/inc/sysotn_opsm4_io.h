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
 *     and register accessor functions for the sysotn_opsm4 block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing sysotn_opsm4 io functions is:
 | {
 |     sysotn_opsm4_buffer_t b_ptr[1];
 |     sysotn_opsm4_buffer_init( b_ptr, h_ptr );
 |     value = sysotn_opsm4_field_<FIELD>_get( b_ptr, h_ptr );
 |     sysotn_opsm4_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     sysotn_opsm4_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = sysotn_opsm4_field_<FIELD>_get( NULL, h_ptr );
 | sysotn_opsm4_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------------+
 | | Register 0x00002004 TX_GLOBAL_CTRL_REG |
 | +----------------------------------------+
 | | bit  30  R/W  TX_SCRAMBLE_ENABLE       |
 | | bit  29  R/W  TX_HARDWARE_RESET        |
 | | bit  27  R/W  TX_FIFO_RESET            |
 | | bit  25  R/W  TX_DATA_IN_SCRAMBLED     |
 | | bit  9:0 R/W  TX_LINE_SOFT_RESET       |
 | +----------------------------------------+
 | 
 | void   sysotn_opsm4_field_TX_SCRAMBLE_ENABLE_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_SCRAMBLE_ENABLE_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_TX_HARDWARE_RESET_set        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_HARDWARE_RESET_get        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_TX_FIFO_RESET_set            ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_FIFO_RESET_get            ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_TX_DATA_IN_SCRAMBLED_set     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_DATA_IN_SCRAMBLED_get     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_TX_LINE_SOFT_RESET_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_LINE_SOFT_RESET_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_LINE_SOFT_RESET_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_LINE_SOFT_RESET_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00002008 TX_OTUK_AIS_CTRL_REG |
 | +------------------------------------------+
 | | bit  12 R/W  TX_FORCE_LANE_OTUK_AIS      |
 | +------------------------------------------+
 | 
 | void   sysotn_opsm4_field_TX_FORCE_LANE_OTUK_AIS_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_FORCE_LANE_OTUK_AIS_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x0000200c TX_INT_REG               |
 | +----------------------------------------------+
 | | bit  26    R/W  TX_SFI_FIFO_OVERFLOW_INT_I   |
 | | bit  25    R/W  TX_SFI_FIFO_UNDERFLOW_INT_I  |
 | | bit  24    R/W  TX_SYS_FIFO_OVERFLOW_INT_I   |
 | | bit  23    R/W  TX_SYS_FIFO_UNDERFLOW_INT_I  |
 | | bit  22    R/W  TX_LANE_FAS_IAE_INT_I        |
 | | bit  21    R/W  TX_LANE_MFAS_IAE_INT_I       |
 | | bit  20:11 R/W  TX_LANE_FIFO_OVERFLOW_INT_I  |
 | | bit  10:1  R/W  TX_LANE_FIFO_UNDERFLOW_INT_I |
 | | bit  0     R/W  TX_OTUK_AIS_INT_I            |
 | +----------------------------------------------+
 | 
 | void                 sysotn_opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_I_get                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_I_poll                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_I_get                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_I_poll                ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_I_set_to_clear         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_I_get                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_I_poll                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_I_set_to_clear        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_I_get                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_I_poll                ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_TX_LANE_FAS_IAE_INT_I_set_to_clear              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_TX_LANE_FAS_IAE_INT_I_get                       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_LANE_FAS_IAE_INT_I_poll                      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_TX_LANE_MFAS_IAE_INT_I_set_to_clear             ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_TX_LANE_MFAS_IAE_INT_I_get                      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_LANE_MFAS_IAE_INT_I_poll                     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_I_get                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 sysotn_opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_set_to_clear  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               sysotn_opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_get           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_I_poll          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_I_poll                ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_I_get                ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 sysotn_opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_set_to_clear ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               sysotn_opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_get          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_I_poll         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_I_poll               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_TX_OTUK_AIS_INT_I_set_to_clear                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_TX_OTUK_AIS_INT_I_get                           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_OTUK_AIS_INT_I_poll                          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------+
 | | Register 0x00002010 TX_INT_EN_REG            |
 | +----------------------------------------------+
 | | bit  26    R/W  TX_SFI_FIFO_OVERFLOW_INT_E   |
 | | bit  25    R/W  TX_SFI_FIFO_UNDERFLOW_INT_E  |
 | | bit  24    R/W  TX_SYS_FIFO_OVERFLOW_INT_E   |
 | | bit  23    R/W  TX_SYS_FIFO_UNDERFLOW_INT_E  |
 | | bit  22    R/W  TX_LANE_FAS_IAE_INT_E        |
 | | bit  21    R/W  TX_LANE_MFAS_IAE_INT_E       |
 | | bit  20:11 R/W  TX_LANE_FIFO_OVERFLOW_INT_E  |
 | | bit  10:1  R/W  TX_LANE_FIFO_UNDERFLOW_INT_E |
 | | bit  0     R/W  TX_OTUK_AIS_INT_E            |
 | +----------------------------------------------+
 | 
 | void   sysotn_opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_E_set         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_E_get         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_E_set        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_E_get        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_E_set         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_E_get         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_E_set        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_E_get        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_TX_LANE_FAS_IAE_INT_E_set              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_LANE_FAS_IAE_INT_E_get              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_TX_LANE_MFAS_IAE_INT_E_set             ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_LANE_MFAS_IAE_INT_E_get             ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_E_set        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_E_get        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_set  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_E_get  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_E_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_E_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_E_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_OTUK_AIS_INT_E_set                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_OTUK_AIS_INT_E_get                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +--------------------------------------------+
 | | Register 0x00002014 TX_INT_VAL_REG         |
 | +--------------------------------------------+
 | | bit  26    R  TX_SFI_FIFO_OVERFLOW_INT_V   |
 | | bit  25    R  TX_SFI_FIFO_UNDERFLOW_INT_V  |
 | | bit  24    R  TX_SYS_FIFO_OVERFLOW_INT_V   |
 | | bit  23    R  TX_SYS_FIFO_UNDERFLOW_INT_V  |
 | | bit  22    R  TX_LANE_FAS_IAE_INT_V        |
 | | bit  21    R  TX_LANE_MFAS_IAE_INT_V       |
 | | bit  20:11 R  TX_LANE_FIFO_OVERFLOW_INT_V  |
 | | bit  10:1  R  TX_LANE_FIFO_UNDERFLOW_INT_V |
 | | bit  0     R  TX_OTUK_AIS_INT_V            |
 | +--------------------------------------------+
 | 
 | UINT32               sysotn_opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_V_get          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_SFI_FIFO_OVERFLOW_INT_V_poll         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_V_get         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_SFI_FIFO_UNDERFLOW_INT_V_poll        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_V_get          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_SYS_FIFO_OVERFLOW_INT_V_poll         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_V_get         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_SYS_FIFO_UNDERFLOW_INT_V_poll        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_TX_LANE_FAS_IAE_INT_V_get               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_LANE_FAS_IAE_INT_V_poll              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_TX_LANE_MFAS_IAE_INT_V_get              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_LANE_MFAS_IAE_INT_V_poll             ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_V_get         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32               sysotn_opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_get   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_range_TX_LANE_FIFO_OVERFLOW_INT_V_poll  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_LANE_FIFO_OVERFLOW_INT_V_poll        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_V_get        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | UINT32               sysotn_opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_get  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_range_TX_LANE_FIFO_UNDERFLOW_INT_V_poll ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_LANE_FIFO_UNDERFLOW_INT_V_poll       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_TX_OTUK_AIS_INT_V_get                   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_TX_OTUK_AIS_INT_V_poll                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------+
 | | Register 0x0000201c TX_L2P_LANE_MAPPING0_REG |
 | +----------------------------------------------+
 | | bit  29:25 R/W  TX_PHYSICAL_LANE_5_SELECT    |
 | | bit  24:20 R/W  TX_PHYSICAL_LANE_4_SELECT    |
 | | bit  19:15 R/W  TX_PHYSICAL_LANE_3_SELECT    |
 | | bit  14:10 R/W  TX_PHYSICAL_LANE_2_SELECT    |
 | | bit  9:5   R/W  TX_PHYSICAL_LANE_1_SELECT    |
 | | bit  4:0   R/W  TX_PHYSICAL_LANE_0_SELECT    |
 | +----------------------------------------------+
 | 
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_5_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_5_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_5_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_5_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_4_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_4_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_4_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_4_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_3_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_3_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_3_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_3_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_2_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_2_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_2_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_2_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_1_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_1_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_1_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_1_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_0_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_0_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_0_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_0_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x00002020 TX_L2P_LANE_MAPPING1_REG |
 | +----------------------------------------------+
 | | bit  29:25 R/W  TX_PHYSICAL_LANE_11_SELECT   |
 | | bit  24:20 R/W  TX_PHYSICAL_LANE_10_SELECT   |
 | | bit  19:15 R/W  TX_PHYSICAL_LANE_9_SELECT    |
 | | bit  14:10 R/W  TX_PHYSICAL_LANE_8_SELECT    |
 | | bit  9:5   R/W  TX_PHYSICAL_LANE_7_SELECT    |
 | | bit  4:0   R/W  TX_PHYSICAL_LANE_6_SELECT    |
 | +----------------------------------------------+
 | 
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_11_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_11_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_11_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_11_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_10_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_10_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_10_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_10_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_9_SELECT_set        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_9_SELECT_get        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_9_SELECT_set  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_9_SELECT_get  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_8_SELECT_set        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_8_SELECT_get        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_8_SELECT_set  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_8_SELECT_get  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_7_SELECT_set        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_7_SELECT_get        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_7_SELECT_set  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_7_SELECT_get  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_6_SELECT_set        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_6_SELECT_get        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_6_SELECT_set  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_6_SELECT_get  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x00002024 TX_L2P_LANE_MAPPING2_REG |
 | +----------------------------------------------+
 | | bit  29:25 R/W  TX_PHYSICAL_LANE_17_SELECT   |
 | | bit  24:20 R/W  TX_PHYSICAL_LANE_16_SELECT   |
 | | bit  19:15 R/W  TX_PHYSICAL_LANE_15_SELECT   |
 | | bit  14:10 R/W  TX_PHYSICAL_LANE_14_SELECT   |
 | | bit  9:5   R/W  TX_PHYSICAL_LANE_13_SELECT   |
 | | bit  4:0   R/W  TX_PHYSICAL_LANE_12_SELECT   |
 | +----------------------------------------------+
 | 
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_17_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_17_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_17_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_17_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_16_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_16_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_16_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_16_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_15_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_15_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_15_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_15_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_14_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_14_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_14_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_14_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_13_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_13_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_13_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_13_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_12_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_12_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_12_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_12_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x00002028 TX_L2P_LANE_MAPPING3_REG |
 | +----------------------------------------------+
 | | bit  9:5 R/W  TX_PHYSICAL_LANE_19_SELECT     |
 | | bit  4:0 R/W  TX_PHYSICAL_LANE_18_SELECT     |
 | +----------------------------------------------+
 | 
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_19_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_19_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_19_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_19_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_PHYSICAL_LANE_18_SELECT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_PHYSICAL_LANE_18_SELECT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_PHYSICAL_LANE_18_SELECT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_PHYSICAL_LANE_18_SELECT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00002030 TX_ASYNC_FIFO_SETTING_REG |
 | +-----------------------------------------------+
 | | bit  14:10 R/W  TX_LINE_FIFO_XOFF_FILL_LEVEL  |
 | | bit  9:6   R/W  TX_LINE_DEQUEUE_EN            |
 | +-----------------------------------------------+
 | 
 | void   sysotn_opsm4_field_TX_LINE_FIFO_XOFF_FILL_LEVEL_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_LINE_FIFO_XOFF_FILL_LEVEL_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_LINE_FIFO_XOFF_FILL_LEVEL_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_LINE_FIFO_XOFF_FILL_LEVEL_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sysotn_opsm4_field_TX_LINE_DEQUEUE_EN_set                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_LINE_DEQUEUE_EN_get                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_TX_LINE_DEQUEUE_EN_set           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_TX_LINE_DEQUEUE_EN_get           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00002034 TX_CONFIG_REG1 |
 | +------------------------------------+
 | | bit  13 R/W  TX_FP_MFAS_INVERT     |
 | +------------------------------------+
 | 
 | void   sysotn_opsm4_field_TX_FP_MFAS_INVERT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_FP_MFAS_INVERT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +----------------------------------------+
 | | Register 0x00002074 RX_GLOBAL_CTRL_REG |
 | +----------------------------------------+
 | | bit  13 R/W  RX_OTL_4_10_ENABLE        |
 | | bit  12 R/W  RX_SCRAMBLE_ENABLE        |
 | | bit  11 R/W  RX_HARDWARE_RESET         |
 | | bit  9  R/W  RX_FIFO_RESET             |
 | | bit  6  R/W  RX_DATA_IN_NOT_SCRAMBLED  |
 | +----------------------------------------+
 | 
 | void   sysotn_opsm4_field_RX_OTL_4_10_ENABLE_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_OTL_4_10_ENABLE_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_SCRAMBLE_ENABLE_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SCRAMBLE_ENABLE_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_HARDWARE_RESET_set        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_HARDWARE_RESET_get        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_FIFO_RESET_set            ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_FIFO_RESET_get            ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_DATA_IN_NOT_SCRAMBLED_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00002078 RX_OTU4_CTRL_REG      |
 | +-------------------------------------------+
 | | bit  7:0 R/W  RX_LOF_TRANSITION_OUT_COUNT |
 | +-------------------------------------------+
 | 
 | void   sysotn_opsm4_field_RX_LOF_TRANSITION_OUT_COUNT_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_LOF_TRANSITION_OUT_COUNT_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_RX_LOF_TRANSITION_OUT_COUNT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_RX_LOF_TRANSITION_OUT_COUNT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0000207c RX_OTU4_INT_REG        |
 | +--------------------------------------------+
 | | bit  15 R/W  RX_SFI_LOS_INT_I              |
 | | bit  14 R/W  RX_SFI_OOF_INT_I              |
 | | bit  13 R/W  RX_SFI_LOF_INT_I              |
 | | bit  12 R/W  RX_OTU_OOF_INT_I              |
 | | bit  11 R/W  RX_OTU_LOF_INT_I              |
 | | bit  9  R/W  RX_SFI_AIS_INT_I              |
 | | bit  8  R/W  RX_LOL_INT_I                  |
 | | bit  7  R/W  RX_SKEW_RANGE_VIOLATION_INT_I |
 | | bit  6  R/W  RX_SFI_ALIGN_CHANGE_INT_I     |
 | | bit  5  R/W  RX_DESKEW_CHANGE_INT_I        |
 | | bit  4  R/W  RX_SFI_FIFO_OVERFLOW_INT_I    |
 | | bit  3  R/W  RX_SFI_FIFO_UNDERFLOW_INT_I   |
 | +--------------------------------------------+
 | 
 | void                 sysotn_opsm4_field_RX_SFI_LOS_INT_I_set_to_clear              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_SFI_LOS_INT_I_get                       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SFI_LOS_INT_I_poll                      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_SFI_OOF_INT_I_set_to_clear              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_SFI_OOF_INT_I_get                       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SFI_OOF_INT_I_poll                      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_SFI_LOF_INT_I_set_to_clear              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_SFI_LOF_INT_I_get                       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SFI_LOF_INT_I_poll                      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_OTU_OOF_INT_I_set_to_clear              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_OTU_OOF_INT_I_get                       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_OTU_OOF_INT_I_poll                      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_OTU_LOF_INT_I_set_to_clear              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_OTU_LOF_INT_I_get                       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_OTU_LOF_INT_I_poll                      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_SFI_AIS_INT_I_set_to_clear              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_SFI_AIS_INT_I_get                       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SFI_AIS_INT_I_poll                      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_LOL_INT_I_set_to_clear                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_LOL_INT_I_get                           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LOL_INT_I_poll                          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_I_set_to_clear ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_I_get          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_I_poll         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_SFI_ALIGN_CHANGE_INT_I_set_to_clear     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_SFI_ALIGN_CHANGE_INT_I_get              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SFI_ALIGN_CHANGE_INT_I_poll             ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_DESKEW_CHANGE_INT_I_set_to_clear        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_DESKEW_CHANGE_INT_I_get                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_DESKEW_CHANGE_INT_I_poll                ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_I_set_to_clear    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_I_get             ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_I_poll            ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_I_set_to_clear   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_I_get            ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_I_poll           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x00002080 RX_OTU4_INT_EN_REG     |
 | +--------------------------------------------+
 | | bit  15 R/W  RX_SFI_LOS_INT_E              |
 | | bit  14 R/W  RX_SFI_OOF_INT_E              |
 | | bit  13 R/W  RX_SFI_LOF_INT_E              |
 | | bit  12 R/W  RX_OTU_OOF_INT_E              |
 | | bit  11 R/W  RX_OTU_LOF_INT_E              |
 | | bit  9  R/W  RX_SFI_AIS_INT_E              |
 | | bit  8  R/W  RX_LOL_INT_E                  |
 | | bit  7  R/W  RX_SKEW_RANGE_VIOLATION_INT_E |
 | | bit  6  R/W  RX_SFI_ALIGN_CHANGE_INT_E     |
 | | bit  5  R/W  RX_DESKEW_CHANGE_INT_E        |
 | | bit  4  R/W  RX_SFI_FIFO_OVERFLOW_INT_E    |
 | | bit  3  R/W  RX_SFI_FIFO_UNDERFLOW_INT_E   |
 | +--------------------------------------------+
 | 
 | void   sysotn_opsm4_field_RX_SFI_LOS_INT_E_set              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SFI_LOS_INT_E_get              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_SFI_OOF_INT_E_set              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SFI_OOF_INT_E_get              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_SFI_LOF_INT_E_set              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SFI_LOF_INT_E_get              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_OTU_OOF_INT_E_set              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_OTU_OOF_INT_E_get              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_OTU_LOF_INT_E_set              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_OTU_LOF_INT_E_get              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_SFI_AIS_INT_E_set              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SFI_AIS_INT_E_get              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_LOL_INT_E_set                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_LOL_INT_E_get                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_E_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_E_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_SFI_ALIGN_CHANGE_INT_E_set     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SFI_ALIGN_CHANGE_INT_E_get     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_DESKEW_CHANGE_INT_E_set        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_DESKEW_CHANGE_INT_E_get        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_E_set    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_E_get    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_E_set   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_E_get   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00002084 RX_OTU4_INT_VAL_REG  |
 | +------------------------------------------+
 | | bit  15 R  RX_SFI_LOS_INT_V              |
 | | bit  14 R  RX_SFI_OOF_INT_V              |
 | | bit  13 R  RX_SFI_LOF_INT_V              |
 | | bit  12 R  RX_OTU_OOF_INT_V              |
 | | bit  11 R  RX_OTU_LOF_INT_V              |
 | | bit  9  R  RX_SFI_AIS_INT_V              |
 | | bit  8  R  RX_LOL_INT_V                  |
 | | bit  7  R  RX_SKEW_RANGE_VIOLATION_INT_V |
 | | bit  6  R  RX_SFI_ALIGN_CHANGE_INT_V     |
 | | bit  5  R  RX_DESKEW_CHANGE_INT_V        |
 | | bit  4  R  RX_SFI_FIFO_OVERFLOW_INT_V    |
 | | bit  3  R  RX_SFI_FIFO_UNDERFLOW_INT_V   |
 | +------------------------------------------+
 | 
 | UINT32               sysotn_opsm4_field_RX_SFI_LOS_INT_V_get               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SFI_LOS_INT_V_poll              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_SFI_OOF_INT_V_get               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SFI_OOF_INT_V_poll              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_SFI_LOF_INT_V_get               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SFI_LOF_INT_V_poll              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_OTU_OOF_INT_V_get               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_OTU_OOF_INT_V_poll              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_OTU_LOF_INT_V_get               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_OTU_LOF_INT_V_poll              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_SFI_AIS_INT_V_get               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SFI_AIS_INT_V_poll              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_LOL_INT_V_get                   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LOL_INT_V_poll                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_V_get  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SKEW_RANGE_VIOLATION_INT_V_poll ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_SFI_ALIGN_CHANGE_INT_V_get      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SFI_ALIGN_CHANGE_INT_V_poll     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_DESKEW_CHANGE_INT_V_get         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_DESKEW_CHANGE_INT_V_poll        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_V_get     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SFI_FIFO_OVERFLOW_INT_V_poll    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_V_get    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_SFI_FIFO_UNDERFLOW_INT_V_poll   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------------------------+
 | | Register (0x000020ec + (N) * 0x4) RX_LANE_SSF_FAIL_OR_CTRL_REG |
 | |     N[0..19]                                                   |
 | +----------------------------------------------------------------+
 | | bit  15 R/W  RX_SSF_LANE_MI_ACTIVE                             |
 | | bit  14 R/W  RX_SSF_LANE_LOS                                   |
 | | bit  13 R/W  RX_SSF_LANE_LOF_LOFLANE                           |
 | | bit  11 R/W  RX_SSF_LANE_OTUK_AIS                              |
 | | bit  10 R/W  RX_SSF_LANE_OTL_AIS                               |
 | | bit  9  R/W  RX_SSF_LOL                                        |
 | | bit  8  R/W  RX_SSF_OTU4_LOF                                   |
 | | bit  7  R/W  RX_FEC_LANE_MI_ACTIVE                             |
 | | bit  6  R/W  RX_FAIL_LANE_LOS                                  |
 | | bit  5  R/W  RX_FAIL_LANE_LOF_LOFLANE                          |
 | | bit  3  R/W  RX_FAIL_LANE_OTUK_AIS                             |
 | | bit  2  R/W  RX_FAIL_LANE_OTL_AIS                              |
 | | bit  1  R/W  RX_FAIL_LOL                                       |
 | | bit  0  R/W  RX_FAIL_OTU4_LOF                                  |
 | +----------------------------------------------------------------+
 | 
 | void   sysotn_opsm4_field_RX_SSF_LANE_MI_ACTIVE_set    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SSF_LANE_MI_ACTIVE_get    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_SSF_LANE_LOS_set          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SSF_LANE_LOS_get          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_SSF_LANE_LOF_LOFLANE_set  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SSF_LANE_LOF_LOFLANE_get  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_SSF_LANE_OTUK_AIS_set     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SSF_LANE_OTUK_AIS_get     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_SSF_LANE_OTL_AIS_set      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SSF_LANE_OTL_AIS_get      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_SSF_LOL_set               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SSF_LOL_get               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_SSF_OTU4_LOF_set          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SSF_OTU4_LOF_get          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_FEC_LANE_MI_ACTIVE_set    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_FEC_LANE_MI_ACTIVE_get    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_FAIL_LANE_LOS_set         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_FAIL_LANE_LOS_get         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_FAIL_LANE_LOF_LOFLANE_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_FAIL_LANE_OTUK_AIS_set    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_FAIL_LANE_OTUK_AIS_get    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_FAIL_LANE_OTL_AIS_set     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_FAIL_LANE_OTL_AIS_get     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_FAIL_LOL_set              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_FAIL_LOL_get              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_FAIL_OTU4_LOF_set         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_FAIL_OTU4_LOF_get         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------------------------+
 | | Register (0x0000213c + (N) * 0x4) RX_LANE_ID_OR_SKEW_REG |
 | |     N[0..19]                                             |
 | +----------------------------------------------------------+
 | | bit  12:5 R  RX_LANE_SKEW_FIFO_LEVEL                     |
 | | bit  4:0  R  RX_LOGICAL_LANE_IDENTIFIER                  |
 | +----------------------------------------------------------+
 | 
 | UINT32 sysotn_opsm4_field_RX_LANE_SKEW_FIFO_LEVEL_get          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | UINT32 sysotn_opsm4_field_range_RX_LANE_SKEW_FIFO_LEVEL_get    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 sysotn_opsm4_field_RX_LOGICAL_LANE_IDENTIFIER_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | UINT32 sysotn_opsm4_field_range_RX_LOGICAL_LANE_IDENTIFIER_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x0000218c + (N) * 0x4) RX_LANE_INT_REG |
 | |     N[0..19]                                      |
 | +---------------------------------------------------+
 | | bit  15 R/W  RX_LANE_LOS_INT_I                    |
 | | bit  14 R/W  RX_LANE_OOF_INT_I                    |
 | | bit  13 R/W  RX_LANE_LOF_LOFLANE_INT_I            |
 | | bit  12 R/W  RX_LANE_OOM_INT_I                    |
 | | bit  10 R/W  RX_LANE_OTUK_AIS_INT_I               |
 | | bit  9  R/W  RX_LANE_OTL_AIS_INT_I                |
 | | bit  7  R/W  RX_LANE_ALIGN_CHANGE_INT_I           |
 | | bit  5  R/W  RX_LANE_FIFO_ERROR_INT_I             |
 | | bit  2  R/W  RX_LANE_OOR_INT_I                    |
 | | bit  1  R/W  RX_LANE_LOR_INT_I                    |
 | +---------------------------------------------------+
 | 
 | void                 sysotn_opsm4_field_RX_LANE_LOS_INT_I_set_to_clear          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_LANE_LOS_INT_I_get                   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_LOS_INT_I_poll                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_LANE_OOF_INT_I_set_to_clear          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_LANE_OOF_INT_I_get                   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_OOF_INT_I_poll                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_set_to_clear  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_get           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_LOF_LOFLANE_INT_I_poll          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_LANE_OOM_INT_I_set_to_clear          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_LANE_OOM_INT_I_get                   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_OOM_INT_I_poll                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_LANE_OTUK_AIS_INT_I_set_to_clear     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_LANE_OTUK_AIS_INT_I_get              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_OTUK_AIS_INT_I_poll             ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_LANE_OTL_AIS_INT_I_set_to_clear      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_LANE_OTL_AIS_INT_I_get               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_OTL_AIS_INT_I_poll              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_set_to_clear ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_get          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_ALIGN_CHANGE_INT_I_poll         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_LANE_FIFO_ERROR_INT_I_set_to_clear   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_LANE_FIFO_ERROR_INT_I_get            ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_FIFO_ERROR_INT_I_poll           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_LANE_OOR_INT_I_set_to_clear          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_LANE_OOR_INT_I_get                   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_OOR_INT_I_poll                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sysotn_opsm4_field_RX_LANE_LOR_INT_I_set_to_clear          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               sysotn_opsm4_field_RX_LANE_LOR_INT_I_get                   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_LOR_INT_I_poll                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------------------+
 | | Register (0x000021dc + (N) * 0x4) RX_LANE_INT_EN_REG |
 | |     N[0..19]                                         |
 | +------------------------------------------------------+
 | | bit  15 R/W  RX_LANE_LOS_INT_E                       |
 | | bit  14 R/W  RX_LANE_OOF_INT_E                       |
 | | bit  13 R/W  RX_LANE_LOF_LOFLANE_INT_E               |
 | | bit  12 R/W  RX_LANE_OOM_INT_E                       |
 | | bit  10 R/W  RX_LANE_OTUK_AIS_INT_E                  |
 | | bit  9  R/W  RX_LANE_OTL_AIS_INT_E                   |
 | | bit  7  R/W  RX_LANE_ALIGN_CHANGE_INT_E              |
 | | bit  5  R/W  RX_LANE_FIFO_ERROR_INT_E                |
 | | bit  2  R/W  RX_LANE_OOR_INT_E                       |
 | | bit  1  R/W  RX_LANE_LOR_INT_E                       |
 | +------------------------------------------------------+
 | 
 | void   sysotn_opsm4_field_RX_LANE_LOS_INT_E_set          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_LANE_LOS_INT_E_get          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_LANE_OOF_INT_E_set          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_LANE_OOF_INT_E_get          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_LANE_LOF_LOFLANE_INT_E_set  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_LANE_LOF_LOFLANE_INT_E_get  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_LANE_OOM_INT_E_set          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_LANE_OOM_INT_E_get          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_LANE_OTUK_AIS_INT_E_set     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_LANE_OTUK_AIS_INT_E_get     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_LANE_OTL_AIS_INT_E_set      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_LANE_OTL_AIS_INT_E_get      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_LANE_ALIGN_CHANGE_INT_E_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_LANE_ALIGN_CHANGE_INT_E_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_LANE_FIFO_ERROR_INT_E_set   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_LANE_FIFO_ERROR_INT_E_get   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_LANE_OOR_INT_E_set          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_LANE_OOR_INT_E_get          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | void   sysotn_opsm4_field_RX_LANE_LOR_INT_E_set          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_LANE_LOR_INT_E_get          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------------+
 | | Register (0x0000222c + (N) * 0x4) RX_LANE_INT_VAL_REG |
 | |     N[0..19]                                          |
 | +-------------------------------------------------------+
 | | bit  15 R  RX_LANE_LOS_INT_V                          |
 | | bit  14 R  RX_LANE_OOF_INT_V                          |
 | | bit  13 R  RX_LANE_LOF_LOFLANE_INT_V                  |
 | | bit  12 R  RX_LANE_OOM_INT_V                          |
 | | bit  10 R  RX_LANE_OTUK_AIS_INT_V                     |
 | | bit  9  R  RX_LANE_OTL_AIS_INT_V                      |
 | | bit  7  R  RX_LANE_ALIGN_CHANGE_INT_V                 |
 | | bit  5  R  RX_LANE_FIFO_ERROR_INT_V                   |
 | | bit  2  R  RX_LANE_OOR_INT_V                          |
 | | bit  1  R  RX_LANE_LOR_INT_V                          |
 | +-------------------------------------------------------+
 | 
 | UINT32               sysotn_opsm4_field_RX_LANE_LOS_INT_V_get           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_LOS_INT_V_poll          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_LANE_OOF_INT_V_get           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_OOF_INT_V_poll          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_LANE_LOF_LOFLANE_INT_V_get   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_LOF_LOFLANE_INT_V_poll  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_LANE_OOM_INT_V_get           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_OOM_INT_V_poll          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_LANE_OTUK_AIS_INT_V_get      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_OTUK_AIS_INT_V_poll     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_LANE_OTL_AIS_INT_V_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_OTL_AIS_INT_V_poll      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_LANE_ALIGN_CHANGE_INT_V_get  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_ALIGN_CHANGE_INT_V_poll ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_LANE_FIFO_ERROR_INT_V_get    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_FIFO_ERROR_INT_V_poll   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_LANE_OOR_INT_V_get           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_OOR_INT_V_poll          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sysotn_opsm4_field_RX_LANE_LOR_INT_V_get           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_field_RX_LANE_LOR_INT_V_poll          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002320 RX_PTP_CONFIG  |
 | +------------------------------------+
 | | bit  30 R/W  RX_FP_MFAS_GEN_INVERT |
 | | bit  29 R/W  RX_FP_MFAS_INVERT     |
 | +------------------------------------+
 | 
 | void   sysotn_opsm4_field_RX_FP_MFAS_GEN_INVERT_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_FP_MFAS_GEN_INVERT_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_FP_MFAS_INVERT_set     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_FP_MFAS_INVERT_get     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x00002324 LPBK_CTRL |
 | +-------------------------------+
 | | bit  1 R/W  RX_SYS_LOOPBACK   |
 | | bit  0 R/W  TX_SYS_LOOPBACK   |
 | +-------------------------------+
 | 
 | void   sysotn_opsm4_field_RX_SYS_LOOPBACK_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_SYS_LOOPBACK_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_TX_SYS_LOOPBACK_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_SYS_LOOPBACK_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00002328 TX_SYS_CTRL           |
 | +-------------------------------------------+
 | | bit  15  R/W  TX_SYS_DEQUEUE_EN           |
 | | bit  14  R/W  SFI_TX_DEQUEUE_EN           |
 | | bit  4:0 R/W  SFI_TX_FIFO_XOFF_FILL_LEVEL |
 | +-------------------------------------------+
 | 
 | void   sysotn_opsm4_field_TX_SYS_DEQUEUE_EN_set                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_TX_SYS_DEQUEUE_EN_get                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_SFI_TX_DEQUEUE_EN_set                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_SFI_TX_DEQUEUE_EN_get                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_SFI_TX_FIFO_XOFF_FILL_LEVEL_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_SFI_TX_FIFO_XOFF_FILL_LEVEL_get       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_range_SFI_TX_FIFO_XOFF_FILL_LEVEL_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sysotn_opsm4_field_range_SFI_TX_FIFO_XOFF_FILL_LEVEL_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x0000232c RX_CTRL    |
 | +--------------------------------+
 | | bit  17 R/W  SFI_RX_DEQUEUE_EN |
 | | bit  16 R/W  RX_LINE_EN        |
 | +--------------------------------+
 | 
 | void   sysotn_opsm4_field_SFI_RX_DEQUEUE_EN_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_SFI_RX_DEQUEUE_EN_get ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void   sysotn_opsm4_field_RX_LINE_EN_set        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | UINT32 sysotn_opsm4_field_RX_LINE_EN_get        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00002364 OPSM4_INT_SUMMARY |
 | +---------------------------------------+
 | | bit  31:0 R  INTERRUPT_SUMMARY        |
 | +---------------------------------------+
 | 
 | void                 sysotn_opsm4_lfield_INTERRUPT_SUMMARY_get        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value[2] )
 | UINT32               sysotn_opsm4_lfield_range_INTERRUPT_SUMMARY_get  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_lfield_range_INTERRUPT_SUMMARY_poll ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_lfield_INTERRUPT_SUMMARY_poll       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value[2], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SYSOTN_OPSM4_IO_H
#define _SYSOTN_OPSM4_IO_H

#include "lineotn_loc.h"
#include "sysotn_opsm4_regs.h"
#include "sysotn_opsm4_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sysotn_opsm4_buffer_init               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | void                 sysotn_opsm4_buffer_flush              ( sysotn_opsm4_buffer_t *b_ptr )
 | UINT32               sysotn_opsm4_reg_read                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 sysotn_opsm4_reg_write                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 sysotn_opsm4_field_set                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 sysotn_opsm4_action_on_write_field_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 sysotn_opsm4_burst_read                ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 sysotn_opsm4_burst_write               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_poll                      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sysotn_opsm4_reg_TX_GLOBAL_CTRL_REG_write                        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_TX_GLOBAL_CTRL_REG_field_set                    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_TX_GLOBAL_CTRL_REG_read                         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_TX_OTUK_AIS_CTRL_REG_write                      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_TX_OTUK_AIS_CTRL_REG_field_set                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_TX_OTUK_AIS_CTRL_REG_read                       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_TX_L2P_LANE_MAPPING0_REG_write                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_TX_L2P_LANE_MAPPING0_REG_field_set              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_TX_L2P_LANE_MAPPING0_REG_read                   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_TX_L2P_LANE_MAPPING1_REG_write                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_TX_L2P_LANE_MAPPING1_REG_field_set              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_TX_L2P_LANE_MAPPING1_REG_read                   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_TX_L2P_LANE_MAPPING2_REG_write                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_TX_L2P_LANE_MAPPING2_REG_field_set              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_TX_L2P_LANE_MAPPING2_REG_read                   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_TX_L2P_LANE_MAPPING3_REG_write                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_TX_L2P_LANE_MAPPING3_REG_field_set              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_TX_L2P_LANE_MAPPING3_REG_read                   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_write                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_field_set             ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_TX_ASYNC_FIFO_SETTING_REG_read                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_TX_CONFIG_REG1_write                            ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_TX_CONFIG_REG1_field_set                        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_TX_CONFIG_REG1_read                             ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_RX_GLOBAL_CTRL_REG_write                        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_RX_GLOBAL_CTRL_REG_field_set                    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_RX_GLOBAL_CTRL_REG_read                         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_RX_OTU4_CTRL_REG_write                          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_RX_OTU4_CTRL_REG_field_set                      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_RX_OTU4_CTRL_REG_read                           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_RX_PTP_CONFIG_write                             ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_RX_PTP_CONFIG_field_set                         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_RX_PTP_CONFIG_read                              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_LPBK_CTRL_write                                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_LPBK_CTRL_field_set                             ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_LPBK_CTRL_read                                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_TX_SYS_CTRL_write                               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_TX_SYS_CTRL_field_set                           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_TX_SYS_CTRL_read                                ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_RX_CTRL_write                                   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_RX_CTRL_field_set                               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_RX_CTRL_read                                    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_write        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 sysotn_opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_field_set    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_RX_LANE_SSF_FAIL_OR_CTRL_REG_array_read         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 sysotn_opsm4_reg_TX_INT_EN_REG_write                             ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_TX_INT_EN_REG_field_set                         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_TX_INT_EN_REG_read                              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_RX_OTU4_INT_EN_REG_write                        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_RX_OTU4_INT_EN_REG_field_set                    ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_RX_OTU4_INT_EN_REG_read                         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | 
 | void                 sysotn_opsm4_reg_RX_LANE_INT_EN_REG_array_write                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 sysotn_opsm4_reg_RX_LANE_INT_EN_REG_array_field_set              ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_RX_LANE_INT_EN_REG_array_read                   ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               sysotn_opsm4_reg_OPSM4_INT_SUMMARY_read                          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_reg_OPSM4_INT_SUMMARY_poll                          ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               sysotn_opsm4_reg_OPSM4_INT_SUMMARY1_read                         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_reg_OPSM4_INT_SUMMARY1_poll                         ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sysotn_opsm4_reg_TX_INT_REG_write                                ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_TX_INT_REG_action_on_write_field_set            ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_TX_INT_REG_read                                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_reg_TX_INT_REG_poll                                 ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sysotn_opsm4_reg_RX_OTU4_INT_REG_write                           ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 value )
 | void                 sysotn_opsm4_reg_RX_OTU4_INT_REG_action_on_write_field_set       ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_RX_OTU4_INT_REG_read                            ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_reg_RX_OTU4_INT_REG_poll                            ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sysotn_opsm4_reg_RX_LANE_INT_REG_array_write                     ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 sysotn_opsm4_reg_RX_LANE_INT_REG_array_action_on_write_field_set ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sysotn_opsm4_reg_RX_LANE_INT_REG_array_read                      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_reg_RX_LANE_INT_REG_array_poll                      ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               sysotn_opsm4_reg_TX_INT_VAL_REG_read                             ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_reg_TX_INT_VAL_REG_poll                             ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               sysotn_opsm4_reg_RX_OTU4_INT_VAL_REG_read                        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_reg_RX_OTU4_INT_VAL_REG_poll                        ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               sysotn_opsm4_reg_RX_LANE_INT_VAL_REG_array_read                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE sysotn_opsm4_reg_RX_LANE_INT_VAL_REG_array_poll                  ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               sysotn_opsm4_reg_RX_LANE_ID_OR_SKEW_REG_array_read               ( sysotn_opsm4_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32  N )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SYSOTN_OPSM4_IO_H */
