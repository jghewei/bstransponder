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
 *     and register accessor functions for the emac block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing emac io functions is:
 | {
 |     emac_buffer_t b_ptr[1];
 |     emac_buffer_init( b_ptr, h_ptr );
 |     value = emac_field_<FIELD>_get( b_ptr, h_ptr );
 |     emac_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     emac_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = emac_field_<FIELD>_get( NULL, h_ptr );
 | emac_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------+
 | | Register 0x00000000 SW_RESET     |
 | +----------------------------------+
 | | bit  3 R/W  ETIME_NREGS_SW_RESET |
 | | bit  2 R/W  ETIME_CORE_SW_RESET  |
 | | bit  1 R/W  EMAC_NREGS_SW_RESET  |
 | | bit  0 R/W  EMAC_CORE_SW_RESET   |
 | +----------------------------------+
 | 
 | void   emac_field_ETIME_NREGS_SW_RESET_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ETIME_NREGS_SW_RESET_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_ETIME_CORE_SW_RESET_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ETIME_CORE_SW_RESET_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_EMAC_NREGS_SW_RESET_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_NREGS_SW_RESET_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_EMAC_CORE_SW_RESET_set   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_CORE_SW_RESET_get   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | +--------------------------------+
 | | Register 0x00000004 LINK_RESET |
 | +--------------------------------+
 | | bit  23:12 R/W  ING_LINK_RESET |
 | | bit  11:0  R/W  EGR_LINK_RESET |
 | +--------------------------------+
 | 
 | void   emac_field_ING_LINK_RESET_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_LINK_RESET_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_LINK_RESET_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_LINK_RESET_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EGR_LINK_RESET_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_LINK_RESET_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_LINK_RESET_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_LINK_RESET_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000010 ETIME_LOW_POWER_CTRL |
 | +------------------------------------------+
 | | bit  0 R/W  ETIME_TS_LOWPWR              |
 | +------------------------------------------+
 | 
 | void   emac_field_ETIME_TS_LOWPWR_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ETIME_TS_LOWPWR_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000014 EMAC_PMA_IF_CLK_LOW_POWER_CTRL |
 | +----------------------------------------------------+
 | | bit  23:12 R/W  EMAC_TX_PMA_CLK_LOWPWR             |
 | | bit  11:0  R/W  EMAC_RX_PMA_CLK_LOWPWR             |
 | +----------------------------------------------------+
 | 
 | void   emac_field_EMAC_TX_PMA_CLK_LOWPWR_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_TX_PMA_CLK_LOWPWR_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_TX_PMA_CLK_LOWPWR_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_TX_PMA_CLK_LOWPWR_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EMAC_RX_PMA_CLK_LOWPWR_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_RX_PMA_CLK_LOWPWR_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_RX_PMA_CLK_LOWPWR_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_RX_PMA_CLK_LOWPWR_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000018 EMAC_MAC_PCS_CLK_LOW_POWER_CTRL |
 | +-----------------------------------------------------+
 | | bit  18:16 R/W  EMAC_LINK_40_100G_CLK_LOWPWR        |
 | | bit  11:0  R/W  EMAC_LINK_10G_CLK_LOWPWR            |
 | +-----------------------------------------------------+
 | 
 | void   emac_field_EMAC_LINK_40_100G_CLK_LOWPWR_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_LINK_40_100G_CLK_LOWPWR_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_LINK_40_100G_CLK_LOWPWR_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EMAC_LINK_10G_CLK_LOWPWR_set           ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_LINK_10G_CLK_LOWPWR_get           ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_LINK_10G_CLK_LOWPWR_set     ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_LINK_10G_CLK_LOWPWR_get     ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register 0x00000020 EMAC_STATISTICS_RAM_LOW_POWER_CTRL |
 | +--------------------------------------------------------+
 | | bit  11:0 R/W  EMAC_STAT_RAM_LOWPWR                    |
 | +--------------------------------------------------------+
 | 
 | void   emac_field_EMAC_STAT_RAM_LOWPWR_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_STAT_RAM_LOWPWR_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_STAT_RAM_LOWPWR_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_STAT_RAM_LOWPWR_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register 0x00000024 EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL |
 | +------------------------------------------------------------+
 | | bit  11:0 R/W  EGR_XOFF_OVERRIDE                           |
 | +------------------------------------------------------------+
 | 
 | void   emac_field_EGR_XOFF_OVERRIDE_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_XOFF_OVERRIDE_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_XOFF_OVERRIDE_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_XOFF_OVERRIDE_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x00000030 EMAC_INGRESS_PCS_INT_VAL_1_4_ |
 | +---------------------------------------------------+
 | | bit  11:0 R  PHY_LOS_V                            |
 | +---------------------------------------------------+
 | 
 | UINT32               emac_field_PHY_LOS_V_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32               emac_field_range_PHY_LOS_V_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_PHY_LOS_V_poll ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_PHY_LOS_V_poll       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------------+
 | | Register 0x00000034 EMAC_INGRESS_PCS_INT_VAL_2_4_ |
 | +---------------------------------------------------+
 | | bit  23:12 R  RX_REM_FAULT_V                      |
 | | bit  11:0  R  RX_LOC_FAULT_V                      |
 | +---------------------------------------------------+
 | 
 | UINT32               emac_field_RX_REM_FAULT_V_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32               emac_field_range_RX_REM_FAULT_V_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_RX_REM_FAULT_V_poll ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_RX_REM_FAULT_V_poll       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               emac_field_RX_LOC_FAULT_V_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32               emac_field_range_RX_LOC_FAULT_V_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_RX_LOC_FAULT_V_poll ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_RX_LOC_FAULT_V_poll       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------------+
 | | Register 0x00000038 EMAC_INGRESS_PCS_INT_VAL_3_4_ |
 | +---------------------------------------------------+
 | | bit  11:0 R  HI_BER_V                             |
 | +---------------------------------------------------+
 | 
 | UINT32               emac_field_HI_BER_V_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32               emac_field_range_HI_BER_V_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_HI_BER_V_poll ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_HI_BER_V_poll       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------------+
 | | Register 0x0000003c EMAC_INGRESS_PCS_INT_VAL_4_4_ |
 | +---------------------------------------------------+
 | | bit  18:16 R  ALIGN_STATUS_V                      |
 | | bit  11:0  R  BLOCK_LOCK_V                        |
 | +---------------------------------------------------+
 | 
 | UINT32               emac_field_ALIGN_STATUS_V_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32               emac_field_range_ALIGN_STATUS_V_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_ALIGN_STATUS_V_poll ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_ALIGN_STATUS_V_poll       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               emac_field_BLOCK_LOCK_V_get          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | UINT32               emac_field_range_BLOCK_LOCK_V_get    ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_BLOCK_LOCK_V_poll   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_BLOCK_LOCK_V_poll         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000040 EMAC_INGRESS_PCS_INT_STAT_1_4_ |
 | +----------------------------------------------------+
 | | bit  11:0 R/W  PHY_LOS_I                           |
 | +----------------------------------------------------+
 | 
 | void                 emac_field_PHY_LOS_I_set_to_clear       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32               emac_field_PHY_LOS_I_get                ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void                 emac_field_range_PHY_LOS_I_set_to_clear ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               emac_field_range_PHY_LOS_I_get          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_PHY_LOS_I_poll         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_PHY_LOS_I_poll               ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000044 EMAC_INGRESS_PCS_INT_STAT_2_4_ |
 | +----------------------------------------------------+
 | | bit  23:12 R/W  RX_REM_FAULT_I                     |
 | | bit  11:0  R/W  RX_LOC_FAULT_I                     |
 | +----------------------------------------------------+
 | 
 | void                 emac_field_RX_REM_FAULT_I_set_to_clear       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32               emac_field_RX_REM_FAULT_I_get                ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void                 emac_field_range_RX_REM_FAULT_I_set_to_clear ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               emac_field_range_RX_REM_FAULT_I_get          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_RX_REM_FAULT_I_poll         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_RX_REM_FAULT_I_poll               ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 emac_field_RX_LOC_FAULT_I_set_to_clear       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32               emac_field_RX_LOC_FAULT_I_get                ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void                 emac_field_range_RX_LOC_FAULT_I_set_to_clear ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               emac_field_range_RX_LOC_FAULT_I_get          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_RX_LOC_FAULT_I_poll         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_RX_LOC_FAULT_I_poll               ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000048 EMAC_INGRESS_PCS_INT_STAT_3_4_ |
 | +----------------------------------------------------+
 | | bit  11:0 R/W  HI_BER_I                            |
 | +----------------------------------------------------+
 | 
 | void                 emac_field_HI_BER_I_set_to_clear       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32               emac_field_HI_BER_I_get                ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void                 emac_field_range_HI_BER_I_set_to_clear ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               emac_field_range_HI_BER_I_get          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_HI_BER_I_poll         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_HI_BER_I_poll               ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000004c EMAC_INGRESS_PCS_INT_STAT_4_4_ |
 | +----------------------------------------------------+
 | | bit  18:16 R/W  ALIGN_STATUS_I                     |
 | | bit  11:0  R/W  BLOCK_LOCK_I                       |
 | +----------------------------------------------------+
 | 
 | void                 emac_field_ALIGN_STATUS_I_set_to_clear       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32               emac_field_ALIGN_STATUS_I_get                ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void                 emac_field_range_ALIGN_STATUS_I_set_to_clear ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               emac_field_range_ALIGN_STATUS_I_get          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_ALIGN_STATUS_I_poll         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_ALIGN_STATUS_I_poll               ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 emac_field_BLOCK_LOCK_I_set_to_clear         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32               emac_field_BLOCK_LOCK_I_get                  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void                 emac_field_range_BLOCK_LOCK_I_set_to_clear   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               emac_field_range_BLOCK_LOCK_I_get            ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_BLOCK_LOCK_I_poll           ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_BLOCK_LOCK_I_poll                 ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x00000050 AGB_OVERFLOW_INT_STAT |
 | +-------------------------------------------+
 | | bit  23:12 R/W  ING_AGB_OVERFLOW_I        |
 | | bit  11:0  R/W  EGR_AGB_OVERFLOW_I        |
 | +-------------------------------------------+
 | 
 | void                 emac_field_ING_AGB_OVERFLOW_I_set_to_clear       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32               emac_field_ING_AGB_OVERFLOW_I_get                ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void                 emac_field_range_ING_AGB_OVERFLOW_I_set_to_clear ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               emac_field_range_ING_AGB_OVERFLOW_I_get          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_ING_AGB_OVERFLOW_I_poll         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_ING_AGB_OVERFLOW_I_poll               ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 emac_field_EGR_AGB_OVERFLOW_I_set_to_clear       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32               emac_field_EGR_AGB_OVERFLOW_I_get                ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void                 emac_field_range_EGR_AGB_OVERFLOW_I_set_to_clear ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               emac_field_range_EGR_AGB_OVERFLOW_I_get          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_EGR_AGB_OVERFLOW_I_poll         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_EGR_AGB_OVERFLOW_I_poll               ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------------+
 | | Register 0x00000058 EMAC_EGRESS_OVERFLOW_INT_STAT |
 | +---------------------------------------------------+
 | | bit  23:12 R/W  EMAC_TX_UNDERFLOW_I               |
 | | bit  11:0  R/W  EMAC_TX_OVERFLOW_I                |
 | +---------------------------------------------------+
 | 
 | void                 emac_field_EMAC_TX_UNDERFLOW_I_set_to_clear       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32               emac_field_EMAC_TX_UNDERFLOW_I_get                ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void                 emac_field_range_EMAC_TX_UNDERFLOW_I_set_to_clear ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               emac_field_range_EMAC_TX_UNDERFLOW_I_get          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_EMAC_TX_UNDERFLOW_I_poll         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_EMAC_TX_UNDERFLOW_I_poll               ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 emac_field_EMAC_TX_OVERFLOW_I_set_to_clear        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32               emac_field_EMAC_TX_OVERFLOW_I_get                 ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void                 emac_field_range_EMAC_TX_OVERFLOW_I_set_to_clear  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               emac_field_range_EMAC_TX_OVERFLOW_I_get           ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE emac_field_range_EMAC_TX_OVERFLOW_I_poll          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE emac_field_EMAC_TX_OVERFLOW_I_poll                ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000060 EMAC_INGRESS_PCS_INT_EN_1_4_ |
 | +--------------------------------------------------+
 | | bit  23:12 R/W  RX_REM_FAULT_E                   |
 | | bit  11:0  R/W  RX_LOC_FAULT_E                   |
 | +--------------------------------------------------+
 | 
 | void   emac_field_RX_REM_FAULT_E_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_RX_REM_FAULT_E_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_RX_REM_FAULT_E_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_RX_REM_FAULT_E_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_RX_LOC_FAULT_E_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_RX_LOC_FAULT_E_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_RX_LOC_FAULT_E_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_RX_LOC_FAULT_E_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000064 EMAC_INGRESS_PCS_INT_EN_2_4_ |
 | +--------------------------------------------------+
 | | bit  11:0 R/W  PHY_LOS_E                         |
 | +--------------------------------------------------+
 | 
 | void   emac_field_PHY_LOS_E_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_PHY_LOS_E_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_PHY_LOS_E_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_PHY_LOS_E_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000068 EMAC_INGRESS_PCS_INT_EN_3_4_ |
 | +--------------------------------------------------+
 | | bit  11:0 R/W  HI_BER_E                          |
 | +--------------------------------------------------+
 | 
 | void   emac_field_HI_BER_E_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_HI_BER_E_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_HI_BER_E_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_HI_BER_E_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x0000006c EMAC_INGRESS_PCS_INT_EN_4_4_ |
 | +--------------------------------------------------+
 | | bit  18:16 R/W  ALIGN_STATUS_E                   |
 | | bit  11:0  R/W  BLOCK_LOCK_E                     |
 | +--------------------------------------------------+
 | 
 | void   emac_field_ALIGN_STATUS_E_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ALIGN_STATUS_E_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ALIGN_STATUS_E_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ALIGN_STATUS_E_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_BLOCK_LOCK_E_set         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_BLOCK_LOCK_E_get         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_BLOCK_LOCK_E_set   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_BLOCK_LOCK_E_get   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000070 AGB_OVERFLOW_INT_EN |
 | +-----------------------------------------+
 | | bit  23:12 R/W  ING_AGB_OVERFLOW_E      |
 | | bit  11:0  R/W  EGR_AGB_OVERFLOW_E      |
 | +-----------------------------------------+
 | 
 | void   emac_field_ING_AGB_OVERFLOW_E_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_AGB_OVERFLOW_E_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_AGB_OVERFLOW_E_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_AGB_OVERFLOW_E_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EGR_AGB_OVERFLOW_E_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_AGB_OVERFLOW_E_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_AGB_OVERFLOW_E_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_AGB_OVERFLOW_E_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00000078 EMAC_EGRESS_OVERFLOW_INT_EN |
 | +-------------------------------------------------+
 | | bit  23:12 R/W  EMAC_TX_UNDERFLOW_E             |
 | | bit  11:0  R/W  EMAC_TX_OVERFLOW_E              |
 | +-------------------------------------------------+
 | 
 | void   emac_field_EMAC_TX_UNDERFLOW_E_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_TX_UNDERFLOW_E_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_TX_UNDERFLOW_E_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_TX_UNDERFLOW_E_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EMAC_TX_OVERFLOW_E_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_TX_OVERFLOW_E_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_TX_OVERFLOW_E_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_TX_OVERFLOW_E_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00000080 INGRESS_DSI_IF |
 | +------------------------------------+
 | | bit  23:22 R/W  ING_DSI_CTRL_11    |
 | | bit  21:20 R/W  ING_DSI_CTRL_10    |
 | | bit  19:18 R/W  ING_DSI_CTRL_9     |
 | | bit  17:16 R/W  ING_DSI_CTRL_8     |
 | | bit  15:14 R/W  ING_DSI_CTRL_7     |
 | | bit  13:12 R/W  ING_DSI_CTRL_6     |
 | | bit  11:10 R/W  ING_DSI_CTRL_5     |
 | | bit  9:8   R/W  ING_DSI_CTRL_4     |
 | | bit  7:6   R/W  ING_DSI_CTRL_3     |
 | | bit  5:4   R/W  ING_DSI_CTRL_2     |
 | | bit  3:2   R/W  ING_DSI_CTRL_1     |
 | | bit  1:0   R/W  ING_DSI_CTRL_0     |
 | +------------------------------------+
 | 
 | void   emac_field_ING_DSI_CTRL_11_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_DSI_CTRL_11_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_DSI_CTRL_11_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_DSI_CTRL_11_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_ING_DSI_CTRL_10_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_DSI_CTRL_10_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_DSI_CTRL_10_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_DSI_CTRL_10_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_ING_DSI_CTRL_9_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_DSI_CTRL_9_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_DSI_CTRL_9_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_DSI_CTRL_9_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_ING_DSI_CTRL_8_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_DSI_CTRL_8_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_DSI_CTRL_8_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_DSI_CTRL_8_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_ING_DSI_CTRL_7_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_DSI_CTRL_7_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_DSI_CTRL_7_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_DSI_CTRL_7_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_ING_DSI_CTRL_6_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_DSI_CTRL_6_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_DSI_CTRL_6_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_DSI_CTRL_6_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_ING_DSI_CTRL_5_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_DSI_CTRL_5_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_DSI_CTRL_5_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_DSI_CTRL_5_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_ING_DSI_CTRL_4_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_DSI_CTRL_4_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_DSI_CTRL_4_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_DSI_CTRL_4_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_ING_DSI_CTRL_3_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_DSI_CTRL_3_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_DSI_CTRL_3_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_DSI_CTRL_3_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_ING_DSI_CTRL_2_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_DSI_CTRL_2_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_DSI_CTRL_2_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_DSI_CTRL_2_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_ING_DSI_CTRL_1_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_DSI_CTRL_1_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_DSI_CTRL_1_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_DSI_CTRL_1_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_ING_DSI_CTRL_0_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_DSI_CTRL_0_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_DSI_CTRL_0_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_DSI_CTRL_0_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000084 EGRESS_DSI_IF |
 | +-----------------------------------+
 | | bit  23:22 R/W  EGR_DSI_CTRL_11   |
 | | bit  21:20 R/W  EGR_DSI_CTRL_10   |
 | | bit  19:18 R/W  EGR_DSI_CTRL_9    |
 | | bit  17:16 R/W  EGR_DSI_CTRL_8    |
 | | bit  15:14 R/W  EGR_DSI_CTRL_7    |
 | | bit  13:12 R/W  EGR_DSI_CTRL_6    |
 | | bit  11:10 R/W  EGR_DSI_CTRL_5    |
 | | bit  9:8   R/W  EGR_DSI_CTRL_4    |
 | | bit  7:6   R/W  EGR_DSI_CTRL_3    |
 | | bit  5:4   R/W  EGR_DSI_CTRL_2    |
 | | bit  3:2   R/W  EGR_DSI_CTRL_1    |
 | | bit  1:0   R/W  EGR_DSI_CTRL_0    |
 | +-----------------------------------+
 | 
 | void   emac_field_EGR_DSI_CTRL_11_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_DSI_CTRL_11_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_DSI_CTRL_11_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_DSI_CTRL_11_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EGR_DSI_CTRL_10_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_DSI_CTRL_10_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_DSI_CTRL_10_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_DSI_CTRL_10_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EGR_DSI_CTRL_9_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_DSI_CTRL_9_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_DSI_CTRL_9_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_DSI_CTRL_9_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EGR_DSI_CTRL_8_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_DSI_CTRL_8_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_DSI_CTRL_8_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_DSI_CTRL_8_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EGR_DSI_CTRL_7_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_DSI_CTRL_7_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_DSI_CTRL_7_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_DSI_CTRL_7_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EGR_DSI_CTRL_6_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_DSI_CTRL_6_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_DSI_CTRL_6_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_DSI_CTRL_6_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EGR_DSI_CTRL_5_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_DSI_CTRL_5_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_DSI_CTRL_5_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_DSI_CTRL_5_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EGR_DSI_CTRL_4_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_DSI_CTRL_4_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_DSI_CTRL_4_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_DSI_CTRL_4_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EGR_DSI_CTRL_3_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_DSI_CTRL_3_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_DSI_CTRL_3_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_DSI_CTRL_3_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EGR_DSI_CTRL_2_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_DSI_CTRL_2_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_DSI_CTRL_2_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_DSI_CTRL_2_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EGR_DSI_CTRL_1_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_DSI_CTRL_1_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_DSI_CTRL_1_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_DSI_CTRL_1_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EGR_DSI_CTRL_0_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_DSI_CTRL_0_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_DSI_CTRL_0_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_DSI_CTRL_0_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x00000088 EGRESS_DSI_IF_DATA_SQUELCHING |
 | +---------------------------------------------------+
 | | bit  27:16 R/W  EGR_DSI_OVR_EN                    |
 | | bit  15:0  R/W  EGR_DSI_OVR_PAT                   |
 | +---------------------------------------------------+
 | 
 | void   emac_field_EGR_DSI_OVR_EN_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_DSI_OVR_EN_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_DSI_OVR_EN_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_DSI_OVR_EN_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EGR_DSI_OVR_PAT_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EGR_DSI_OVR_PAT_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EGR_DSI_OVR_PAT_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EGR_DSI_OVR_PAT_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x0000008c INGRESS_DATA_SINK |
 | +---------------------------------------+
 | | bit  11:0 R/W  ING_DATA_SINK          |
 | +---------------------------------------+
 | 
 | void   emac_field_ING_DATA_SINK_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_ING_DATA_SINK_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_ING_DATA_SINK_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_ING_DATA_SINK_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000090 EGRESS_TIMESTAMP_MASK_CTRL |
 | +------------------------------------------------+
 | | bit  30:16 R/W  TS_MASK_END                    |
 | | bit  14:0  R/W  TS_MASK_START                  |
 | +------------------------------------------------+
 | 
 | void   emac_field_TS_MASK_END_set         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_TS_MASK_END_get         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_TS_MASK_END_set   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_TS_MASK_END_get   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_TS_MASK_START_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_TS_MASK_START_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_TS_MASK_START_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_TS_MASK_START_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x00000094 EGRESS_TIMESTAMP_MASK_DISABLE |
 | +---------------------------------------------------+
 | | bit  2:0 R/W  TS_MASK_DISABLE                     |
 | +---------------------------------------------------+
 | 
 | void   emac_field_TS_MASK_DISABLE_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_TS_MASK_DISABLE_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_TS_MASK_DISABLE_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_TS_MASK_DISABLE_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000098 EMAC_INGRESS_DROP_ERR_MASK |
 | +------------------------------------------------+
 | | bit  15:8 R/W  EMAC_ING_DPI_DROP_MASK          |
 | | bit  7:0  R/W  EMAC_ING_DPI_ERR_MASK           |
 | +------------------------------------------------+
 | 
 | void   emac_field_EMAC_ING_DPI_DROP_MASK_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_ING_DPI_DROP_MASK_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_ING_DPI_DROP_MASK_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_ING_DPI_DROP_MASK_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EMAC_ING_DPI_ERR_MASK_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_ING_DPI_ERR_MASK_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_ING_DPI_ERR_MASK_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_ING_DPI_ERR_MASK_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------------+
 | | Register 0x0000009c EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK |
 | +-----------------------------------------------------------------+
 | | bit  23:12 R/W  EMAC_ING_DPI_FCS_MASK                           |
 | | bit  11:0  R/W  EMAC_ING_DPI_LEN_MASK                           |
 | +-----------------------------------------------------------------+
 | 
 | void   emac_field_EMAC_ING_DPI_FCS_MASK_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_ING_DPI_FCS_MASK_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_ING_DPI_FCS_MASK_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_ING_DPI_FCS_MASK_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EMAC_ING_DPI_LEN_MASK_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_ING_DPI_LEN_MASK_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_ING_DPI_LEN_MASK_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_ING_DPI_LEN_MASK_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x000000a0 EGRESS_CREDIT_CNT_CFG      |
 | +------------------------------------------------+
 | | bit  29:24 R/W  EMAC_TX_CREDIT_COUNT_MAX_100G  |
 | | bit  23:20 R/W  EMAC_TX_CREDIT_COUNT_MAX_40G   |
 | | bit  19:16 R/W  EMAC_TX_CREDIT_COUNT_MAX_10G   |
 | | bit  13:8  R/W  EMAC_TX_CREDIT_COUNT_INIT_100G |
 | | bit  7:4   R/W  EMAC_TX_CREDIT_COUNT_INIT_40G  |
 | | bit  3:0   R/W  EMAC_TX_CREDIT_COUNT_INIT_10G  |
 | +------------------------------------------------+
 | 
 | void   emac_field_EMAC_TX_CREDIT_COUNT_MAX_100G_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_TX_CREDIT_COUNT_MAX_100G_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_100G_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_100G_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EMAC_TX_CREDIT_COUNT_MAX_40G_set         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_TX_CREDIT_COUNT_MAX_40G_get         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_40G_set   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_40G_get   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EMAC_TX_CREDIT_COUNT_MAX_10G_set         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_TX_CREDIT_COUNT_MAX_10G_get         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_10G_set   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_MAX_10G_get   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EMAC_TX_CREDIT_COUNT_INIT_100G_set       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_TX_CREDIT_COUNT_INIT_100G_get       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_100G_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_100G_get ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EMAC_TX_CREDIT_COUNT_INIT_40G_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_TX_CREDIT_COUNT_INIT_40G_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_40G_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_40G_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_field_EMAC_TX_CREDIT_COUNT_INIT_10G_set        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | UINT32 emac_field_EMAC_TX_CREDIT_COUNT_INIT_10G_get        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void   emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_10G_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_field_range_EMAC_TX_CREDIT_COUNT_INIT_10G_get  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _EMAC_IO_H
#define _EMAC_IO_H

#include "emac_loc.h"
#include "emac_regs.h"
#include "emac_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 emac_buffer_init               ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | void                 emac_buffer_flush              ( emac_buffer_t *b_ptr )
 | UINT32               emac_reg_read                  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 emac_reg_write                 ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 emac_field_set                 ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 emac_action_on_write_field_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 emac_burst_read                ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 emac_burst_write               ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE emac_poll                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 emac_reg_SW_RESET_write                                           ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_SW_RESET_field_set                                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_SW_RESET_read                                            ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_ETIME_LOW_POWER_CTRL_write                               ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_ETIME_LOW_POWER_CTRL_field_set                           ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_ETIME_LOW_POWER_CTRL_read                                ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_write                     ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_field_set                 ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_PMA_IF_CLK_LOW_POWER_CTRL_read                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_write                    ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_field_set                ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_MAC_PCS_CLK_LOW_POWER_CTRL_read                     ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_write                 ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_field_set             ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_STATISTICS_RAM_LOW_POWER_CTRL_read                  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_write             ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_field_set         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_EGRESS_BACKPRESSURE_OVERRIDE_CTRL_read              ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_INGRESS_DSI_IF_write                                     ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_INGRESS_DSI_IF_field_set                                 ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_INGRESS_DSI_IF_read                                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EGRESS_DSI_IF_write                                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EGRESS_DSI_IF_field_set                                  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EGRESS_DSI_IF_read                                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_write                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_field_set                  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EGRESS_DSI_IF_DATA_SQUELCHING_read                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_INGRESS_DATA_SINK_write                                  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_INGRESS_DATA_SINK_field_set                              ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_INGRESS_DATA_SINK_read                                   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_write                         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_field_set                     ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EGRESS_TIMESTAMP_MASK_CTRL_read                          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_write                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_field_set                  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EGRESS_TIMESTAMP_MASK_DISABLE_read                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EMAC_INGRESS_DROP_ERR_MASK_write                         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_INGRESS_DROP_ERR_MASK_field_set                     ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_INGRESS_DROP_ERR_MASK_read                          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_write        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_field_set    ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_INGRESS_PACKET_FCS_AND_LENGTH_ERR_MASK_read         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EGRESS_CREDIT_CNT_CFG_write                              ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EGRESS_CREDIT_CNT_CFG_field_set                          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EGRESS_CREDIT_CNT_CFG_read                               ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_LINK_RESET_write                                         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_LINK_RESET_field_set                                     ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_LINK_RESET_read                                          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__write                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__field_set                   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_INGRESS_PCS_INT_EN_1_4__read                        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__write                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__field_set                   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_INGRESS_PCS_INT_EN_2_4__read                        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__write                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__field_set                   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_INGRESS_PCS_INT_EN_3_4__read                        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__write                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__field_set                   ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_INGRESS_PCS_INT_EN_4_4__read                        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_AGB_OVERFLOW_INT_EN_write                                ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_AGB_OVERFLOW_INT_EN_field_set                            ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_AGB_OVERFLOW_INT_EN_read                                 ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_write                        ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_field_set                    ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_EGRESS_OVERFLOW_INT_EN_read                         ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | 
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__write                     ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__action_on_write_field_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__read                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_STAT_1_4__poll                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__write                     ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__action_on_write_field_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__read                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_STAT_2_4__poll                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__write                     ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__action_on_write_field_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__read                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_STAT_3_4__poll                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__write                     ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__action_on_write_field_set ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__read                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_STAT_4_4__poll                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 emac_reg_AGB_OVERFLOW_INT_STAT_write                              ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_AGB_OVERFLOW_INT_STAT_action_on_write_field_set          ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_AGB_OVERFLOW_INT_STAT_read                               ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE emac_reg_AGB_OVERFLOW_INT_STAT_poll                               ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_write                      ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 value )
 | void                 emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_action_on_write_field_set  ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_read                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE emac_reg_EMAC_EGRESS_OVERFLOW_INT_STAT_poll                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               emac_reg_EMAC_INGRESS_PCS_INT_VAL_1_4__read                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_VAL_1_4__poll                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__read                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_VAL_2_4__poll                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               emac_reg_EMAC_INGRESS_PCS_INT_VAL_3_4__read                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_VAL_3_4__poll                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__read                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE emac_reg_EMAC_INGRESS_PCS_INT_VAL_4_4__poll                       ( emac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _EMAC_IO_H */
