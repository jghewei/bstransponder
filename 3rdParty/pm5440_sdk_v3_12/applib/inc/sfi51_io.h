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
 *     and register accessor functions for the sfi51 block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing sfi51 io functions is:
 | {
 |     sfi51_buffer_t b_ptr[1];
 |     sfi51_buffer_init( b_ptr, h_ptr, A );
 |     value = sfi51_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     sfi51_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     sfi51_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = sfi51_field_<FIELD>_get( NULL, h_ptr, A );
 | sfi51_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------------+
 | | Register 0x0001a000 RX_GLOBAL_CTRL_REG |
 | +----------------------------------------+
 | | bit  2 R/W  RX_LANE_SWAP               |
 | | bit  1 R/W  RX_DESKEW_DISABLE          |
 | | bit  0 R/W  RX_SYNC_RESET              |
 | +----------------------------------------+
 | 
 | void   sfi51_field_RX_LANE_SWAP_set      ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_RX_LANE_SWAP_get      ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   sfi51_field_RX_DESKEW_DISABLE_set ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_RX_DESKEW_DISABLE_get ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   sfi51_field_RX_SYNC_RESET_set     ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_RX_SYNC_RESET_get     ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------+
 | | Register 0x0001a004 TX_GLOBAL_CTRL_REG |
 | +----------------------------------------+
 | | bit  1 R/W  TX_LANE_SWAP               |
 | | bit  0 R/W  TX_SYNC_RESET              |
 | +----------------------------------------+
 | 
 | void   sfi51_field_TX_LANE_SWAP_set  ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_TX_LANE_SWAP_get  ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   sfi51_field_TX_SYNC_RESET_set ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_TX_SYNC_RESET_get ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x0001a008 RX_INT_REG    |
 | +-----------------------------------+
 | | bit  4 R/W  RX_DESKEW_FIFO_OVFL_I |
 | | bit  3 R/W  RX_DESKEW_FIFO_UNFL_I |
 | | bit  2 R/W  RX_DESKEW_LOF_I       |
 | | bit  1 R/W  RX_DESKEW_OOF_I       |
 | | bit  0 R/W  RX_DESKEW_LOS_I       |
 | +-----------------------------------+
 | 
 | void                 sfi51_field_RX_DESKEW_FIFO_OVFL_I_set_to_clear ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               sfi51_field_RX_DESKEW_FIFO_OVFL_I_get          ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_FIFO_OVFL_I_poll         ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sfi51_field_RX_DESKEW_FIFO_UNFL_I_set_to_clear ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               sfi51_field_RX_DESKEW_FIFO_UNFL_I_get          ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_FIFO_UNFL_I_poll         ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sfi51_field_RX_DESKEW_LOF_I_set_to_clear       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               sfi51_field_RX_DESKEW_LOF_I_get                ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_LOF_I_poll               ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sfi51_field_RX_DESKEW_OOF_I_set_to_clear       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               sfi51_field_RX_DESKEW_OOF_I_get                ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_OOF_I_poll               ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sfi51_field_RX_DESKEW_LOS_I_set_to_clear       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               sfi51_field_RX_DESKEW_LOS_I_get                ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_LOS_I_poll               ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x0001a00c RX_INT_EN     |
 | +-----------------------------------+
 | | bit  4 R/W  RX_DESKEW_FIFO_OVFL_E |
 | | bit  3 R/W  RX_DESKEW_FIFO_UNFL_E |
 | | bit  2 R/W  RX_DESKEW_LOF_E       |
 | | bit  1 R/W  RX_DESKEW_OOF_E       |
 | | bit  0 R/W  RX_DESKEW_LOS_E       |
 | +-----------------------------------+
 | 
 | void   sfi51_field_RX_DESKEW_FIFO_OVFL_E_set ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_RX_DESKEW_FIFO_OVFL_E_get ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   sfi51_field_RX_DESKEW_FIFO_UNFL_E_set ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_RX_DESKEW_FIFO_UNFL_E_get ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   sfi51_field_RX_DESKEW_LOF_E_set       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_RX_DESKEW_LOF_E_get       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   sfi51_field_RX_DESKEW_OOF_E_set       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_RX_DESKEW_OOF_E_get       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   sfi51_field_RX_DESKEW_LOS_E_set       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_RX_DESKEW_LOS_E_get       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------+
 | | Register 0x0001a010 RX_INT_STAT |
 | +---------------------------------+
 | | bit  4 R  RX_DESKEW_FIFO_OVFL_V |
 | | bit  3 R  RX_DESKEW_FIFO_UNFL_V |
 | | bit  2 R  RX_DESKEW_LOF_V       |
 | | bit  1 R  RX_DESKEW_OOF_V       |
 | | bit  0 R  RX_DESKEW_LOS_V       |
 | +---------------------------------+
 | 
 | UINT32               sfi51_field_RX_DESKEW_FIFO_OVFL_V_get  ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_FIFO_OVFL_V_poll ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sfi51_field_RX_DESKEW_FIFO_UNFL_V_get  ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_FIFO_UNFL_V_poll ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sfi51_field_RX_DESKEW_LOF_V_get        ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_LOF_V_poll       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sfi51_field_RX_DESKEW_OOF_V_get        ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_OOF_V_poll       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sfi51_field_RX_DESKEW_LOS_V_get        ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_LOS_V_poll       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x0001a014 RX_DESKEW_OOA_INT_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  RX_DESKEW_OOA_I            |
 | +-------------------------------------------+
 | 
 | void                 sfi51_field_RX_DESKEW_OOA_I_set_to_clear       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               sfi51_field_RX_DESKEW_OOA_I_get                ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void                 sfi51_field_range_RX_DESKEW_OOA_I_set_to_clear ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               sfi51_field_range_RX_DESKEW_OOA_I_get          ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sfi51_field_range_RX_DESKEW_OOA_I_poll         ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_OOA_I_poll               ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------+
 | | Register 0x0001a018 RX_DESKEW_OOA_INT_EN_REG |
 | +----------------------------------------------+
 | | bit  15:0 R/W  RX_DESKEW_OOA_E               |
 | +----------------------------------------------+
 | 
 | void   sfi51_field_RX_DESKEW_OOA_E_set       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_RX_DESKEW_OOA_E_get       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   sfi51_field_range_RX_DESKEW_OOA_E_set ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_field_range_RX_DESKEW_OOA_E_get ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x0001a01c RX_DESKEW_OOA_INT_STAT_REG |
 | +------------------------------------------------+
 | | bit  15:0 R  RX_DESKEW_OOA_V                   |
 | +------------------------------------------------+
 | 
 | UINT32               sfi51_field_RX_DESKEW_OOA_V_get        ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32               sfi51_field_range_RX_DESKEW_OOA_V_get  ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sfi51_field_range_RX_DESKEW_OOA_V_poll ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sfi51_field_RX_DESKEW_OOA_V_poll       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------------+
 | | Register 0x0001a020 RX_CHANNEL_BIT_ERR_INT_REG |
 | +------------------------------------------------+
 | | bit  15:0 R/W  RX_CHN_BIT_ERR_I                |
 | +------------------------------------------------+
 | 
 | void                 sfi51_field_RX_CHN_BIT_ERR_I_set_to_clear       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               sfi51_field_RX_CHN_BIT_ERR_I_get                ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void                 sfi51_field_range_RX_CHN_BIT_ERR_I_set_to_clear ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               sfi51_field_range_RX_CHN_BIT_ERR_I_get          ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sfi51_field_range_RX_CHN_BIT_ERR_I_poll         ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sfi51_field_RX_CHN_BIT_ERR_I_poll               ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------------+
 | | Register 0x0001a024 RX_CHANNEL_BIT_ERR_INT_EN_REG |
 | +---------------------------------------------------+
 | | bit  15:0 R/W  RX_CHN_BIT_ERR_E                   |
 | +---------------------------------------------------+
 | 
 | void   sfi51_field_RX_CHN_BIT_ERR_E_set       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_RX_CHN_BIT_ERR_E_get       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   sfi51_field_range_RX_CHN_BIT_ERR_E_set ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_field_range_RX_CHN_BIT_ERR_E_get ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0001a028 RX_CHANNEL_BIT_ERR_INT_VAL_REG |
 | +----------------------------------------------------+
 | | bit  15:0 R  RX_CHN_BIT_ERR_V                      |
 | +----------------------------------------------------+
 | 
 | UINT32               sfi51_field_RX_CHN_BIT_ERR_V_get        ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | UINT32               sfi51_field_range_RX_CHN_BIT_ERR_V_get  ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sfi51_field_range_RX_CHN_BIT_ERR_V_poll ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sfi51_field_RX_CHN_BIT_ERR_V_poll       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------------------+
 | | Register 0x0001a02c DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG |
 | +----------------------------------------------------------+
 | | bit  4:0 R/W  DSC_FIFO_HI_RD_THRSH                       |
 | +----------------------------------------------------------+
 | 
 | void   sfi51_field_DSC_FIFO_HI_RD_THRSH_set       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_DSC_FIFO_HI_RD_THRSH_get       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   sfi51_field_range_DSC_FIFO_HI_RD_THRSH_set ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_field_range_DSC_FIFO_HI_RD_THRSH_get ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x0001a030 TX_INT_REG |
 | +--------------------------------+
 | | bit  1 R/W  TX_FIFO_UNFL_I     |
 | | bit  0 R/W  TX_FIFO_OVFL_I     |
 | +--------------------------------+
 | 
 | void                 sfi51_field_TX_FIFO_UNFL_I_set_to_clear ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               sfi51_field_TX_FIFO_UNFL_I_get          ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_field_TX_FIFO_UNFL_I_poll         ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sfi51_field_TX_FIFO_OVFL_I_set_to_clear ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               sfi51_field_TX_FIFO_OVFL_I_get          ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_field_TX_FIFO_OVFL_I_poll         ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------+
 | | Register 0x0001a034 TX_INT_EN |
 | +-------------------------------+
 | | bit  1 R/W  TX_FIFO_UNFL_E    |
 | | bit  0 R/W  TX_FIFO_OVFL_E    |
 | +-------------------------------+
 | 
 | void   sfi51_field_TX_FIFO_UNFL_E_set ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_TX_FIFO_UNFL_E_get ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   sfi51_field_TX_FIFO_OVFL_E_set ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_TX_FIFO_OVFL_E_get ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------+
 | | Register 0x0001a038 TX_INT_STAT |
 | +---------------------------------+
 | | bit  1 R  TX_FIFO_UNFL_V        |
 | | bit  0 R  TX_FIFO_OVFL_V        |
 | +---------------------------------+
 | 
 | UINT32               sfi51_field_TX_FIFO_UNFL_V_get  ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_field_TX_FIFO_UNFL_V_poll ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               sfi51_field_TX_FIFO_OVFL_V_get  ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_field_TX_FIFO_OVFL_V_poll ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x0001a03c EXPANSION_HEADER_REG |
 | +------------------------------------------+
 | | bit  31:0 R/W  EXPN_HEADER               |
 | +------------------------------------------+
 | 
 | void   sfi51_field_EXPN_HEADER_set       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 sfi51_field_EXPN_HEADER_get       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void   sfi51_field_range_EXPN_HEADER_set ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sfi51_field_range_EXPN_HEADER_get ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SFI51_IO_H
#define _SFI51_IO_H

#include "lineotn_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "lineotn.h"
#include "sfi51_regs.h"
#include "sfi51_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sfi51_buffer_init               ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | void                 sfi51_buffer_flush              ( sfi51_buffer_t *b_ptr )
 | UINT32               sfi51_reg_read                  ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 sfi51_reg_write                 ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 sfi51_field_set                 ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 sfi51_action_on_write_field_set ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 sfi51_burst_read                ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 sfi51_burst_write               ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE sfi51_poll                      ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sfi51_reg_RX_GLOBAL_CTRL_REG_write                             ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 sfi51_reg_RX_GLOBAL_CTRL_REG_field_set                         ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_reg_RX_GLOBAL_CTRL_REG_read                              ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 sfi51_reg_TX_GLOBAL_CTRL_REG_write                             ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 sfi51_reg_TX_GLOBAL_CTRL_REG_field_set                         ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_reg_TX_GLOBAL_CTRL_REG_read                              ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_write           ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_field_set       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_reg_DESKEW_CHANNEL_FIFO_HIGH_READ_TH_REG_read            ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 sfi51_reg_EXPANSION_HEADER_REG_write                           ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 sfi51_reg_EXPANSION_HEADER_REG_field_set                       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_reg_EXPANSION_HEADER_REG_read                            ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 sfi51_reg_RX_INT_EN_write                                      ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 sfi51_reg_RX_INT_EN_field_set                                  ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_reg_RX_INT_EN_read                                       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_write                       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_field_set                   ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_reg_RX_DESKEW_OOA_INT_EN_REG_read                        ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_write                  ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_field_set              ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_reg_RX_CHANNEL_BIT_ERR_INT_EN_REG_read                   ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 sfi51_reg_TX_INT_EN_write                                      ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 sfi51_reg_TX_INT_EN_field_set                                  ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_reg_TX_INT_EN_read                                       ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 sfi51_reg_RX_INT_REG_write                                     ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 sfi51_reg_RX_INT_REG_action_on_write_field_set                 ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_reg_RX_INT_REG_read                                      ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_reg_RX_INT_REG_poll                                      ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sfi51_reg_RX_DESKEW_OOA_INT_REG_write                          ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 sfi51_reg_RX_DESKEW_OOA_INT_REG_action_on_write_field_set      ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_reg_RX_DESKEW_OOA_INT_REG_read                           ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_reg_RX_DESKEW_OOA_INT_REG_poll                           ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_write                     ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_action_on_write_field_set ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_read                      ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_reg_RX_CHANNEL_BIT_ERR_INT_REG_poll                      ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sfi51_reg_TX_INT_REG_write                                     ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 sfi51_reg_TX_INT_REG_action_on_write_field_set                 ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sfi51_reg_TX_INT_REG_read                                      ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_reg_TX_INT_REG_poll                                      ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               sfi51_reg_RX_INT_STAT_read                                     ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_reg_RX_INT_STAT_poll                                     ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               sfi51_reg_RX_DESKEW_OOA_INT_STAT_REG_read                      ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_reg_RX_DESKEW_OOA_INT_STAT_REG_poll                      ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               sfi51_reg_RX_CHANNEL_BIT_ERR_INT_VAL_REG_read                  ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_reg_RX_CHANNEL_BIT_ERR_INT_VAL_REG_poll                  ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               sfi51_reg_TX_INT_STAT_read                                     ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE sfi51_reg_TX_INT_STAT_poll                                     ( sfi51_buffer_t *b_ptr, lineotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SFI51_IO_H */
