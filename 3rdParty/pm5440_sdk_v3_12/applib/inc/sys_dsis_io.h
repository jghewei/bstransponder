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
 *     and register accessor functions for the sys_dsis block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing sys_dsis io functions is:
 | {
 |     sys_dsis_buffer_t b_ptr[1];
 |     sys_dsis_buffer_init( b_ptr, h_ptr );
 |     value = sys_dsis_field_<FIELD>_get( b_ptr, h_ptr );
 |     sys_dsis_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     sys_dsis_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = sys_dsis_field_<FIELD>_get( NULL, h_ptr );
 | sys_dsis_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------------------------------------+
 | | Register (0x00001000 + (N) * 0x4) RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 |
 | |     N[0..11]                                                          |
 | +-----------------------------------------------------------------------+
 | | bit  10:8 R/W  RX_EN                                                  |
 | | bit  7:4  R/W  RX_LSEL                                                |
 | | bit  3:0  R/W  RX_SEL                                                 |
 | +-----------------------------------------------------------------------+
 | 
 | void   sys_dsis_field_RX_EN_set         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sys_dsis_field_RX_EN_get         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sys_dsis_field_range_RX_EN_set   ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_RX_EN_get   ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sys_dsis_field_RX_LSEL_set       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sys_dsis_field_RX_LSEL_get       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sys_dsis_field_range_RX_LSEL_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_RX_LSEL_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sys_dsis_field_RX_SEL_set        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sys_dsis_field_RX_SEL_get        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sys_dsis_field_range_RX_SEL_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_RX_SEL_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------------+
 | | Register (0x00001030 + (N) * 0x4) RX_FIRST_LEVEL_MUX_FOR_LANES_0_11 |
 | |     N[0..11]                                                        |
 | +---------------------------------------------------------------------+
 | | bit  5:4 R/W  RX_I_LSEL                                             |
 | | bit  1:0 R/W  RX_I_SEL                                              |
 | +---------------------------------------------------------------------+
 | 
 | void   sys_dsis_field_RX_I_LSEL_set       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sys_dsis_field_RX_I_LSEL_get       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sys_dsis_field_range_RX_I_LSEL_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_RX_I_LSEL_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sys_dsis_field_RX_I_SEL_set        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sys_dsis_field_RX_I_SEL_get        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sys_dsis_field_range_RX_I_SEL_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_RX_I_SEL_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------+
 | | Register (0x00001060 + (N) * 0x4) TX_CLK_SEL_AND_EN_FOR_LANES_0_11 |
 | |     N[0..11]                                                       |
 | +--------------------------------------------------------------------+
 | | bit  6:4 R/W  TX_CLK_EN                                            |
 | | bit  3:0 R/W  TX_CLK_SEL                                           |
 | +--------------------------------------------------------------------+
 | 
 | void   sys_dsis_field_TX_CLK_EN_set        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sys_dsis_field_TX_CLK_EN_get        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sys_dsis_field_range_TX_CLK_EN_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_TX_CLK_EN_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sys_dsis_field_TX_CLK_SEL_set       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sys_dsis_field_TX_CLK_SEL_get       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sys_dsis_field_range_TX_CLK_SEL_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_TX_CLK_SEL_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------+
 | | Register (0x00001090 + (N) * 0x4) TX_CLK_PREMUX_SEL_FOR_LANES_0_11 |
 | |     N[0..11]                                                       |
 | +--------------------------------------------------------------------+
 | | bit  1:0 R/W  TX_I_CLK_SEL                                         |
 | +--------------------------------------------------------------------+
 | 
 | void   sys_dsis_field_TX_I_CLK_SEL_set       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sys_dsis_field_TX_I_CLK_SEL_get       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sys_dsis_field_range_TX_I_CLK_SEL_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_TX_I_CLK_SEL_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000010c0 ENET_TX_CLK_SELECT |
 | +----------------------------------------+
 | | bit  11:0 R/W  ENET_TX_CLK_SEL         |
 | +----------------------------------------+
 | 
 | void   sys_dsis_field_ENET_TX_CLK_SEL_set       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_ENET_TX_CLK_SEL_get       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_range_ENET_TX_CLK_SEL_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_ENET_TX_CLK_SEL_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------+
 | | Register (0x000010c4 + (N) * 0x4) TX_AND_ACB_SS_SEL_FOR_LANES_0_11 |
 | |     N[0..11]                                                       |
 | +--------------------------------------------------------------------+
 | | bit  6:4 R/W  ACB_IF_SEL                                           |
 | | bit  1:0 R/W  TX_IF_SEL                                            |
 | +--------------------------------------------------------------------+
 | 
 | void   sys_dsis_field_ACB_IF_SEL_set       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sys_dsis_field_ACB_IF_SEL_get       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sys_dsis_field_range_ACB_IF_SEL_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_ACB_IF_SEL_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sys_dsis_field_TX_IF_SEL_set        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sys_dsis_field_TX_IF_SEL_get        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sys_dsis_field_range_TX_IF_SEL_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_TX_IF_SEL_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------+
 | | Register (0x000010f4 + (N) * 0x4) TX_SEL_FOR_SERDES_LANES_0_11 |
 | |     N[0..11]                                                   |
 | +----------------------------------------------------------------+
 | | bit  3:0 R/W  TX_SEL                                           |
 | +----------------------------------------------------------------+
 | 
 | void   sys_dsis_field_TX_SEL_set       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sys_dsis_field_TX_SEL_get       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sys_dsis_field_range_TX_SEL_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_TX_SEL_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00001144 SERDES_TX_LANE_EN |
 | +---------------------------------------+
 | | bit  19:0 R/W  TX_LANE_ENABLE         |
 | +---------------------------------------+
 | 
 | void   sys_dsis_field_TX_LANE_ENABLE_set       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_TX_LANE_ENABLE_get       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_range_TX_LANE_ENABLE_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_TX_LANE_ENABLE_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x00001148 + (N) * 0x4) TX_FIFO_WRITE_CLK_SELECT |
 | |     N[0..11]                                               |
 | +------------------------------------------------------------+
 | | bit  4:0 R/W  TX_FIFO_WRITE_CLK_SEL                        |
 | +------------------------------------------------------------+
 | 
 | void   sys_dsis_field_TX_FIFO_WRITE_CLK_SEL_set       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sys_dsis_field_TX_FIFO_WRITE_CLK_SEL_get       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sys_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00001198 TX_FIFO_RESET |
 | +-----------------------------------+
 | | bit  19:0 R/W  TX_FIFO_RESET      |
 | +-----------------------------------+
 | 
 | void   sys_dsis_field_TX_FIFO_RESET_set       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_TX_FIFO_RESET_get       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_range_TX_FIFO_RESET_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_TX_FIFO_RESET_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0000119c TX_FIFO_MODE_SELECT |
 | +-----------------------------------------+
 | | bit  19:0 R/W  TX_FIFO_MODE_SEL         |
 | +-----------------------------------------+
 | 
 | void   sys_dsis_field_TX_FIFO_MODE_SEL_set       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_TX_FIFO_MODE_SEL_get       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_range_TX_FIFO_MODE_SEL_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_TX_FIFO_MODE_SEL_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x000011a0 TX_FIFO_CENTERING_MODE |
 | +--------------------------------------------+
 | | bit  19:0 R/W  TX_FIFO_AUTO_CENTER         |
 | +--------------------------------------------+
 | 
 | void   sys_dsis_field_TX_FIFO_AUTO_CENTER_set       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_TX_FIFO_AUTO_CENTER_get       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_range_TX_FIFO_AUTO_CENTER_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_TX_FIFO_AUTO_CENTER_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x000011a4 TX_FIFO_CENTERING_REQUEST |
 | +-----------------------------------------------+
 | | bit  19:0 R/W  TX_FIFO_CENTER_REQUEST         |
 | +-----------------------------------------------+
 | 
 | void   sys_dsis_field_TX_FIFO_CENTER_REQUEST_set       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_TX_FIFO_CENTER_REQUEST_get       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_range_TX_FIFO_CENTER_REQUEST_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_TX_FIFO_CENTER_REQUEST_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x000011a8 + (N) * 0x4) TX_FIFO_READ_POINTER_FOR_TXFIFO |
 | |     N[0..11]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  2:0 R/W  TX_FIFO_READ_PTR                                    |
 | +-------------------------------------------------------------------+
 | 
 | void   sys_dsis_field_TX_FIFO_READ_PTR_set       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sys_dsis_field_TX_FIFO_READ_PTR_get       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sys_dsis_field_range_TX_FIFO_READ_PTR_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sys_dsis_field_range_TX_FIFO_READ_PTR_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x000011f8 TX_FIFO_0_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_0_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_0_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_0_I      |
 | | bit  1 R/W  RP_BELOW_EXP_0_I      |
 | | bit  0 R/W  RP_IN_SYNC_0_I        |
 | +-----------------------------------+
 | 
 | void                 sys_dsis_field_RP_NOT_CENTERED_0_I_set_to_clear ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_NOT_CENTERED_0_I_get          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_0_I_poll         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_OUT_OF_SYNC_0_I_set_to_clear  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_OUT_OF_SYNC_0_I_get           ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_0_I_poll          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_ABOVE_EXP_0_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_ABOVE_EXP_0_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_0_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_BELOW_EXP_0_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_BELOW_EXP_0_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_0_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_IN_SYNC_0_I_set_to_clear      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_IN_SYNC_0_I_get               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_0_I_poll              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x000011fc TX_FIFO_1_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_1_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_1_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_1_I      |
 | | bit  1 R/W  RP_BELOW_EXP_1_I      |
 | | bit  0 R/W  RP_IN_SYNC_1_I        |
 | +-----------------------------------+
 | 
 | void                 sys_dsis_field_RP_NOT_CENTERED_1_I_set_to_clear ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_NOT_CENTERED_1_I_get          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_1_I_poll         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_OUT_OF_SYNC_1_I_set_to_clear  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_OUT_OF_SYNC_1_I_get           ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_1_I_poll          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_ABOVE_EXP_1_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_ABOVE_EXP_1_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_1_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_BELOW_EXP_1_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_BELOW_EXP_1_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_1_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_IN_SYNC_1_I_set_to_clear      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_IN_SYNC_1_I_get               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_1_I_poll              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00001200 TX_FIFO_2_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_2_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_2_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_2_I      |
 | | bit  1 R/W  RP_BELOW_EXP_2_I      |
 | | bit  0 R/W  RP_IN_SYNC_2_I        |
 | +-----------------------------------+
 | 
 | void                 sys_dsis_field_RP_NOT_CENTERED_2_I_set_to_clear ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_NOT_CENTERED_2_I_get          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_2_I_poll         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_OUT_OF_SYNC_2_I_set_to_clear  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_OUT_OF_SYNC_2_I_get           ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_2_I_poll          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_ABOVE_EXP_2_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_ABOVE_EXP_2_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_2_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_BELOW_EXP_2_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_BELOW_EXP_2_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_2_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_IN_SYNC_2_I_set_to_clear      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_IN_SYNC_2_I_get               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_2_I_poll              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00001204 TX_FIFO_3_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_3_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_3_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_3_I      |
 | | bit  1 R/W  RP_BELOW_EXP_3_I      |
 | | bit  0 R/W  RP_IN_SYNC_3_I        |
 | +-----------------------------------+
 | 
 | void                 sys_dsis_field_RP_NOT_CENTERED_3_I_set_to_clear ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_NOT_CENTERED_3_I_get          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_3_I_poll         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_OUT_OF_SYNC_3_I_set_to_clear  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_OUT_OF_SYNC_3_I_get           ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_3_I_poll          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_ABOVE_EXP_3_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_ABOVE_EXP_3_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_3_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_BELOW_EXP_3_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_BELOW_EXP_3_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_3_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_IN_SYNC_3_I_set_to_clear      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_IN_SYNC_3_I_get               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_3_I_poll              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00001208 TX_FIFO_4_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_4_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_4_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_4_I      |
 | | bit  1 R/W  RP_BELOW_EXP_4_I      |
 | | bit  0 R/W  RP_IN_SYNC_4_I        |
 | +-----------------------------------+
 | 
 | void                 sys_dsis_field_RP_NOT_CENTERED_4_I_set_to_clear ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_NOT_CENTERED_4_I_get          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_4_I_poll         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_OUT_OF_SYNC_4_I_set_to_clear  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_OUT_OF_SYNC_4_I_get           ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_4_I_poll          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_ABOVE_EXP_4_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_ABOVE_EXP_4_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_4_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_BELOW_EXP_4_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_BELOW_EXP_4_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_4_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_IN_SYNC_4_I_set_to_clear      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_IN_SYNC_4_I_get               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_4_I_poll              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x0000120c TX_FIFO_5_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_5_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_5_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_5_I      |
 | | bit  1 R/W  RP_BELOW_EXP_5_I      |
 | | bit  0 R/W  RP_IN_SYNC_5_I        |
 | +-----------------------------------+
 | 
 | void                 sys_dsis_field_RP_NOT_CENTERED_5_I_set_to_clear ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_NOT_CENTERED_5_I_get          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_5_I_poll         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_OUT_OF_SYNC_5_I_set_to_clear  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_OUT_OF_SYNC_5_I_get           ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_5_I_poll          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_ABOVE_EXP_5_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_ABOVE_EXP_5_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_5_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_BELOW_EXP_5_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_BELOW_EXP_5_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_5_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_IN_SYNC_5_I_set_to_clear      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_IN_SYNC_5_I_get               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_5_I_poll              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00001210 TX_FIFO_6_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_6_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_6_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_6_I      |
 | | bit  1 R/W  RP_BELOW_EXP_6_I      |
 | | bit  0 R/W  RP_IN_SYNC_6_I        |
 | +-----------------------------------+
 | 
 | void                 sys_dsis_field_RP_NOT_CENTERED_6_I_set_to_clear ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_NOT_CENTERED_6_I_get          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_6_I_poll         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_OUT_OF_SYNC_6_I_set_to_clear  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_OUT_OF_SYNC_6_I_get           ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_6_I_poll          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_ABOVE_EXP_6_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_ABOVE_EXP_6_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_6_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_BELOW_EXP_6_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_BELOW_EXP_6_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_6_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_IN_SYNC_6_I_set_to_clear      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_IN_SYNC_6_I_get               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_6_I_poll              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00001214 TX_FIFO_7_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_7_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_7_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_7_I      |
 | | bit  1 R/W  RP_BELOW_EXP_7_I      |
 | | bit  0 R/W  RP_IN_SYNC_7_I        |
 | +-----------------------------------+
 | 
 | void                 sys_dsis_field_RP_NOT_CENTERED_7_I_set_to_clear ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_NOT_CENTERED_7_I_get          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_7_I_poll         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_OUT_OF_SYNC_7_I_set_to_clear  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_OUT_OF_SYNC_7_I_get           ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_7_I_poll          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_ABOVE_EXP_7_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_ABOVE_EXP_7_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_7_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_BELOW_EXP_7_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_BELOW_EXP_7_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_7_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_IN_SYNC_7_I_set_to_clear      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_IN_SYNC_7_I_get               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_7_I_poll              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00001218 TX_FIFO_8_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_8_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_8_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_8_I      |
 | | bit  1 R/W  RP_BELOW_EXP_8_I      |
 | | bit  0 R/W  RP_IN_SYNC_8_I        |
 | +-----------------------------------+
 | 
 | void                 sys_dsis_field_RP_NOT_CENTERED_8_I_set_to_clear ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_NOT_CENTERED_8_I_get          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_8_I_poll         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_OUT_OF_SYNC_8_I_set_to_clear  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_OUT_OF_SYNC_8_I_get           ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_8_I_poll          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_ABOVE_EXP_8_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_ABOVE_EXP_8_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_8_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_BELOW_EXP_8_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_BELOW_EXP_8_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_8_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_IN_SYNC_8_I_set_to_clear      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_IN_SYNC_8_I_get               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_8_I_poll              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x0000121c TX_FIFO_9_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_9_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_9_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_9_I      |
 | | bit  1 R/W  RP_BELOW_EXP_9_I      |
 | | bit  0 R/W  RP_IN_SYNC_9_I        |
 | +-----------------------------------+
 | 
 | void                 sys_dsis_field_RP_NOT_CENTERED_9_I_set_to_clear ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_NOT_CENTERED_9_I_get          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_9_I_poll         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_OUT_OF_SYNC_9_I_set_to_clear  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_OUT_OF_SYNC_9_I_get           ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_9_I_poll          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_ABOVE_EXP_9_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_ABOVE_EXP_9_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_9_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_BELOW_EXP_9_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_BELOW_EXP_9_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_9_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_IN_SYNC_9_I_set_to_clear      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_IN_SYNC_9_I_get               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_9_I_poll              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00001220 TX_FIFO_10_INT |
 | +------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_10_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_10_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_10_I      |
 | | bit  1 R/W  RP_BELOW_EXP_10_I      |
 | | bit  0 R/W  RP_IN_SYNC_10_I        |
 | +------------------------------------+
 | 
 | void                 sys_dsis_field_RP_NOT_CENTERED_10_I_set_to_clear ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_NOT_CENTERED_10_I_get          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_10_I_poll         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_OUT_OF_SYNC_10_I_set_to_clear  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_OUT_OF_SYNC_10_I_get           ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_10_I_poll          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_ABOVE_EXP_10_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_ABOVE_EXP_10_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_10_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_BELOW_EXP_10_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_BELOW_EXP_10_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_10_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_IN_SYNC_10_I_set_to_clear      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_IN_SYNC_10_I_get               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_10_I_poll              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00001224 TX_FIFO_11_INT |
 | +------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_11_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_11_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_11_I      |
 | | bit  1 R/W  RP_BELOW_EXP_11_I      |
 | | bit  0 R/W  RP_IN_SYNC_11_I        |
 | +------------------------------------+
 | 
 | void                 sys_dsis_field_RP_NOT_CENTERED_11_I_set_to_clear ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_NOT_CENTERED_11_I_get          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_NOT_CENTERED_11_I_poll         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_OUT_OF_SYNC_11_I_set_to_clear  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_OUT_OF_SYNC_11_I_get           ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_OUT_OF_SYNC_11_I_poll          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_ABOVE_EXP_11_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_ABOVE_EXP_11_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_ABOVE_EXP_11_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_BELOW_EXP_11_I_set_to_clear    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_BELOW_EXP_11_I_get             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_BELOW_EXP_11_I_poll            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 sys_dsis_field_RP_IN_SYNC_11_I_set_to_clear      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32               sys_dsis_field_RP_IN_SYNC_11_I_get               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_RP_IN_SYNC_11_I_poll              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------+
 | | Register 0x00001248 TX_FIFO_0_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_0_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_0_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_0_E         |
 | | bit  1 R/W  RP_BELOW_EXP_0_E         |
 | | bit  0 R/W  RP_IN_SYNC_0_E           |
 | +--------------------------------------+
 | 
 | void   sys_dsis_field_RP_NOT_CENTERED_0_E_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_NOT_CENTERED_0_E_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_OUT_OF_SYNC_0_E_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_OUT_OF_SYNC_0_E_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_ABOVE_EXP_0_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_ABOVE_EXP_0_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_BELOW_EXP_0_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_BELOW_EXP_0_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_IN_SYNC_0_E_set      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_IN_SYNC_0_E_get      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x0000124c TX_FIFO_1_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_1_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_1_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_1_E         |
 | | bit  1 R/W  RP_BELOW_EXP_1_E         |
 | | bit  0 R/W  RP_IN_SYNC_1_E           |
 | +--------------------------------------+
 | 
 | void   sys_dsis_field_RP_NOT_CENTERED_1_E_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_NOT_CENTERED_1_E_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_OUT_OF_SYNC_1_E_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_OUT_OF_SYNC_1_E_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_ABOVE_EXP_1_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_ABOVE_EXP_1_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_BELOW_EXP_1_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_BELOW_EXP_1_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_IN_SYNC_1_E_set      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_IN_SYNC_1_E_get      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00001250 TX_FIFO_2_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_2_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_2_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_2_E         |
 | | bit  1 R/W  RP_BELOW_EXP_2_E         |
 | | bit  0 R/W  RP_IN_SYNC_2_E           |
 | +--------------------------------------+
 | 
 | void   sys_dsis_field_RP_NOT_CENTERED_2_E_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_NOT_CENTERED_2_E_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_OUT_OF_SYNC_2_E_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_OUT_OF_SYNC_2_E_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_ABOVE_EXP_2_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_ABOVE_EXP_2_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_BELOW_EXP_2_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_BELOW_EXP_2_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_IN_SYNC_2_E_set      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_IN_SYNC_2_E_get      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00001254 TX_FIFO_3_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_3_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_3_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_3_E         |
 | | bit  1 R/W  RP_BELOW_EXP_3_E         |
 | | bit  0 R/W  RP_IN_SYNC_3_E           |
 | +--------------------------------------+
 | 
 | void   sys_dsis_field_RP_NOT_CENTERED_3_E_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_NOT_CENTERED_3_E_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_OUT_OF_SYNC_3_E_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_OUT_OF_SYNC_3_E_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_ABOVE_EXP_3_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_ABOVE_EXP_3_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_BELOW_EXP_3_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_BELOW_EXP_3_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_IN_SYNC_3_E_set      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_IN_SYNC_3_E_get      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00001258 TX_FIFO_4_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_4_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_4_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_4_E         |
 | | bit  1 R/W  RP_BELOW_EXP_4_E         |
 | | bit  0 R/W  RP_IN_SYNC_4_E           |
 | +--------------------------------------+
 | 
 | void   sys_dsis_field_RP_NOT_CENTERED_4_E_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_NOT_CENTERED_4_E_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_OUT_OF_SYNC_4_E_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_OUT_OF_SYNC_4_E_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_ABOVE_EXP_4_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_ABOVE_EXP_4_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_BELOW_EXP_4_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_BELOW_EXP_4_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_IN_SYNC_4_E_set      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_IN_SYNC_4_E_get      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x0000125c TX_FIFO_5_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_5_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_5_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_5_E         |
 | | bit  1 R/W  RP_BELOW_EXP_5_E         |
 | | bit  0 R/W  RP_IN_SYNC_5_E           |
 | +--------------------------------------+
 | 
 | void   sys_dsis_field_RP_NOT_CENTERED_5_E_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_NOT_CENTERED_5_E_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_OUT_OF_SYNC_5_E_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_OUT_OF_SYNC_5_E_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_ABOVE_EXP_5_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_ABOVE_EXP_5_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_BELOW_EXP_5_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_BELOW_EXP_5_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_IN_SYNC_5_E_set      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_IN_SYNC_5_E_get      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00001260 TX_FIFO_6_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_6_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_6_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_6_E         |
 | | bit  1 R/W  RP_BELOW_EXP_6_E         |
 | | bit  0 R/W  RP_IN_SYNC_6_E           |
 | +--------------------------------------+
 | 
 | void   sys_dsis_field_RP_NOT_CENTERED_6_E_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_NOT_CENTERED_6_E_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_OUT_OF_SYNC_6_E_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_OUT_OF_SYNC_6_E_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_ABOVE_EXP_6_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_ABOVE_EXP_6_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_BELOW_EXP_6_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_BELOW_EXP_6_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_IN_SYNC_6_E_set      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_IN_SYNC_6_E_get      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00001264 TX_FIFO_7_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_7_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_7_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_7_E         |
 | | bit  1 R/W  RP_BELOW_EXP_7_E         |
 | | bit  0 R/W  RP_IN_SYNC_7_E           |
 | +--------------------------------------+
 | 
 | void   sys_dsis_field_RP_NOT_CENTERED_7_E_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_NOT_CENTERED_7_E_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_OUT_OF_SYNC_7_E_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_OUT_OF_SYNC_7_E_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_ABOVE_EXP_7_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_ABOVE_EXP_7_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_BELOW_EXP_7_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_BELOW_EXP_7_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_IN_SYNC_7_E_set      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_IN_SYNC_7_E_get      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00001268 TX_FIFO_8_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_8_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_8_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_8_E         |
 | | bit  1 R/W  RP_BELOW_EXP_8_E         |
 | | bit  0 R/W  RP_IN_SYNC_8_E           |
 | +--------------------------------------+
 | 
 | void   sys_dsis_field_RP_NOT_CENTERED_8_E_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_NOT_CENTERED_8_E_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_OUT_OF_SYNC_8_E_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_OUT_OF_SYNC_8_E_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_ABOVE_EXP_8_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_ABOVE_EXP_8_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_BELOW_EXP_8_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_BELOW_EXP_8_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_IN_SYNC_8_E_set      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_IN_SYNC_8_E_get      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x0000126c TX_FIFO_9_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_9_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_9_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_9_E         |
 | | bit  1 R/W  RP_BELOW_EXP_9_E         |
 | | bit  0 R/W  RP_IN_SYNC_9_E           |
 | +--------------------------------------+
 | 
 | void   sys_dsis_field_RP_NOT_CENTERED_9_E_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_NOT_CENTERED_9_E_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_OUT_OF_SYNC_9_E_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_OUT_OF_SYNC_9_E_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_ABOVE_EXP_9_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_ABOVE_EXP_9_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_BELOW_EXP_9_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_BELOW_EXP_9_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_IN_SYNC_9_E_set      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_IN_SYNC_9_E_get      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00001270 TX_FIFO_10_INT_EN |
 | +---------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_10_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_10_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_10_E         |
 | | bit  1 R/W  RP_BELOW_EXP_10_E         |
 | | bit  0 R/W  RP_IN_SYNC_10_E           |
 | +---------------------------------------+
 | 
 | void   sys_dsis_field_RP_NOT_CENTERED_10_E_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_NOT_CENTERED_10_E_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_OUT_OF_SYNC_10_E_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_OUT_OF_SYNC_10_E_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_ABOVE_EXP_10_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_ABOVE_EXP_10_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_BELOW_EXP_10_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_BELOW_EXP_10_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_IN_SYNC_10_E_set      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_IN_SYNC_10_E_get      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00001274 TX_FIFO_11_INT_EN |
 | +---------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_11_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_11_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_11_E         |
 | | bit  1 R/W  RP_BELOW_EXP_11_E         |
 | | bit  0 R/W  RP_IN_SYNC_11_E           |
 | +---------------------------------------+
 | 
 | void   sys_dsis_field_RP_NOT_CENTERED_11_E_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_NOT_CENTERED_11_E_get ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_OUT_OF_SYNC_11_E_set  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_OUT_OF_SYNC_11_E_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_ABOVE_EXP_11_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_ABOVE_EXP_11_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_BELOW_EXP_11_E_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_BELOW_EXP_11_E_get    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sys_dsis_field_RP_IN_SYNC_11_E_set      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sys_dsis_field_RP_IN_SYNC_11_E_get      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00001298 INT_SUMMARY |
 | +---------------------------------+
 | | bit  19:0 R  TX_FIFO_INT        |
 | +---------------------------------+
 | 
 | UINT32               sys_dsis_field_TX_FIFO_INT_get        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32               sys_dsis_field_range_TX_FIFO_INT_get  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_range_TX_FIFO_INT_poll ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE sys_dsis_field_TX_FIFO_INT_poll       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SYS_DSIS_IO_H
#define _SYS_DSIS_IO_H

#include "sifd_api.h"
#include "sys_dsis_regs.h"
#include "sys_dsis_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sys_dsis_buffer_init               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void                 sys_dsis_buffer_flush              ( sys_dsis_buffer_t *b_ptr )
 | UINT32               sys_dsis_reg_read                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 sys_dsis_reg_write                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 sys_dsis_field_set                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 sys_dsis_action_on_write_field_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 sys_dsis_burst_read                ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 sys_dsis_burst_write               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE sys_dsis_poll                      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sys_dsis_reg_ENET_TX_CLK_SELECT_write                            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_ENET_TX_CLK_SELECT_field_set                        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_ENET_TX_CLK_SELECT_read                             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_SERDES_TX_LANE_EN_write                             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_SERDES_TX_LANE_EN_field_set                         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_SERDES_TX_LANE_EN_read                              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_RESET_write                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_RESET_field_set                             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_RESET_read                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_MODE_SELECT_write                           ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_MODE_SELECT_field_set                       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_MODE_SELECT_read                            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_CENTERING_MODE_write                        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_CENTERING_MODE_field_set                    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_CENTERING_MODE_read                         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_write                     ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_field_set                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_CENTERING_REQUEST_read                      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_write     ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_field_set ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_read      ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_write       ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_field_set   ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_read        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_write        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_field_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_read         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_write        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_field_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_read         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_write        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_field_set    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_read         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_write            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_field_set        ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_read             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_write                ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_field_set            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_read                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_write         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_field_set     ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_array_read          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 sys_dsis_reg_TX_FIFO_0_INT_EN_write                              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_0_INT_EN_field_set                          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_0_INT_EN_read                               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_1_INT_EN_write                              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_1_INT_EN_field_set                          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_1_INT_EN_read                               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_2_INT_EN_write                              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_2_INT_EN_field_set                          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_2_INT_EN_read                               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_3_INT_EN_write                              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_3_INT_EN_field_set                          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_3_INT_EN_read                               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_4_INT_EN_write                              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_4_INT_EN_field_set                          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_4_INT_EN_read                               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_5_INT_EN_write                              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_5_INT_EN_field_set                          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_5_INT_EN_read                               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_6_INT_EN_write                              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_6_INT_EN_field_set                          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_6_INT_EN_read                               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_7_INT_EN_write                              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_7_INT_EN_field_set                          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_7_INT_EN_read                               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_8_INT_EN_write                              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_8_INT_EN_field_set                          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_8_INT_EN_read                               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_9_INT_EN_write                              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_9_INT_EN_field_set                          ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_9_INT_EN_read                               ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_10_INT_EN_write                             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_10_INT_EN_field_set                         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_10_INT_EN_read                              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void                 sys_dsis_reg_TX_FIFO_11_INT_EN_write                             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_11_INT_EN_field_set                         ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_11_INT_EN_read                              ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | UINT32               sys_dsis_reg_INT_SUMMARY_read                                    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_reg_INT_SUMMARY_poll                                    ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sys_dsis_reg_TX_FIFO_0_INT_write                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_0_INT_action_on_write_field_set             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_0_INT_read                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_0_INT_poll                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sys_dsis_reg_TX_FIFO_1_INT_write                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_1_INT_action_on_write_field_set             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_1_INT_read                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_1_INT_poll                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sys_dsis_reg_TX_FIFO_2_INT_write                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_2_INT_action_on_write_field_set             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_2_INT_read                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_2_INT_poll                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sys_dsis_reg_TX_FIFO_3_INT_write                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_3_INT_action_on_write_field_set             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_3_INT_read                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_3_INT_poll                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sys_dsis_reg_TX_FIFO_4_INT_write                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_4_INT_action_on_write_field_set             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_4_INT_read                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_4_INT_poll                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sys_dsis_reg_TX_FIFO_5_INT_write                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_5_INT_action_on_write_field_set             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_5_INT_read                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_5_INT_poll                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sys_dsis_reg_TX_FIFO_6_INT_write                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_6_INT_action_on_write_field_set             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_6_INT_read                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_6_INT_poll                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sys_dsis_reg_TX_FIFO_7_INT_write                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_7_INT_action_on_write_field_set             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_7_INT_read                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_7_INT_poll                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sys_dsis_reg_TX_FIFO_8_INT_write                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_8_INT_action_on_write_field_set             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_8_INT_read                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_8_INT_poll                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sys_dsis_reg_TX_FIFO_9_INT_write                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_9_INT_action_on_write_field_set             ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_9_INT_read                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_9_INT_poll                                  ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sys_dsis_reg_TX_FIFO_10_INT_write                                ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_10_INT_action_on_write_field_set            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_10_INT_read                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_10_INT_poll                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 sys_dsis_reg_TX_FIFO_11_INT_write                                ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void                 sys_dsis_reg_TX_FIFO_11_INT_action_on_write_field_set            ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               sys_dsis_reg_TX_FIFO_11_INT_read                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE sys_dsis_reg_TX_FIFO_11_INT_poll                                 ( sys_dsis_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SYS_DSIS_IO_H */
