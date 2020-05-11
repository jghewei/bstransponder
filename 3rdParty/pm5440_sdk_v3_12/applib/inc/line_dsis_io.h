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
 *     and register accessor functions for the line_dsis block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing line_dsis io functions is:
 | {
 |     line_dsis_buffer_t b_ptr[1];
 |     line_dsis_buffer_init( b_ptr, h_ptr );
 |     value = line_dsis_field_<FIELD>_get( b_ptr, h_ptr );
 |     line_dsis_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     line_dsis_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = line_dsis_field_<FIELD>_get( NULL, h_ptr );
 | line_dsis_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------------------------------------+
 | | Register (0x00000000 + (N) * 0x4) RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11 |
 | |     N[0..11]                                                          |
 | +-----------------------------------------------------------------------+
 | | bit  10:8 R/W  RX_EN                                                  |
 | | bit  7:4  R/W  RX_LSEL                                                |
 | | bit  3:0  R/W  RX_SEL                                                 |
 | +-----------------------------------------------------------------------+
 | 
 | void   line_dsis_field_RX_EN_set         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_RX_EN_get         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_RX_EN_set   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_RX_EN_get   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   line_dsis_field_RX_LSEL_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_RX_LSEL_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_RX_LSEL_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_RX_LSEL_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   line_dsis_field_RX_SEL_set        ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_RX_SEL_get        ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_RX_SEL_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_RX_SEL_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------------+
 | | Register (0x00000030 + (N) * 0x4) RX_FIRST_LEVEL_MUX_FOR_LANES_0_11 |
 | |     N[0..11]                                                        |
 | +---------------------------------------------------------------------+
 | | bit  5:4 R/W  RX_I_LSEL                                             |
 | | bit  1:0 R/W  RX_I_SEL                                              |
 | +---------------------------------------------------------------------+
 | 
 | void   line_dsis_field_RX_I_LSEL_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_RX_I_LSEL_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_RX_I_LSEL_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_RX_I_LSEL_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   line_dsis_field_RX_I_SEL_set        ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_RX_I_SEL_get        ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_RX_I_SEL_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_RX_I_SEL_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------+
 | | Register (0x00000060 + (N) * 0x4) TX_CLK_SEL_AND_EN_FOR_LANES_0_11 |
 | |     N[0..11]                                                       |
 | +--------------------------------------------------------------------+
 | | bit  6:4 R/W  TX_CLK_EN                                            |
 | | bit  3:0 R/W  TX_CLK_SEL                                           |
 | +--------------------------------------------------------------------+
 | 
 | void   line_dsis_field_TX_CLK_EN_set        ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_TX_CLK_EN_get        ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_TX_CLK_EN_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_TX_CLK_EN_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   line_dsis_field_TX_CLK_SEL_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_TX_CLK_SEL_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_TX_CLK_SEL_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_TX_CLK_SEL_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------+
 | | Register (0x00000090 + (N) * 0x4) TX_CLK_PREMUX_SEL_FOR_LANES_0_11 |
 | |     N[0..11]                                                       |
 | +--------------------------------------------------------------------+
 | | bit  1:0 R/W  TX_I_CLK_SEL                                         |
 | +--------------------------------------------------------------------+
 | 
 | void   line_dsis_field_TX_I_CLK_SEL_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_TX_I_CLK_SEL_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_TX_I_CLK_SEL_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_TX_I_CLK_SEL_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000000c0 ENET_TX_CLK_SELECT |
 | +----------------------------------------+
 | | bit  11:0 R/W  ENET_TX_CLK_SEL         |
 | +----------------------------------------+
 | 
 | void   line_dsis_field_ENET_TX_CLK_SEL_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_ENET_TX_CLK_SEL_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_range_ENET_TX_CLK_SEL_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_ENET_TX_CLK_SEL_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------+
 | | Register (0x000000c4 + (N) * 0x4) TX_AND_ACB_SS_SEL_FOR_LANES_0_11 |
 | |     N[0..11]                                                       |
 | +--------------------------------------------------------------------+
 | | bit  6:4 R/W  ACB_IF_SEL                                           |
 | | bit  1:0 R/W  TX_IF_SEL                                            |
 | +--------------------------------------------------------------------+
 | 
 | void   line_dsis_field_ACB_IF_SEL_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_ACB_IF_SEL_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_ACB_IF_SEL_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_ACB_IF_SEL_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   line_dsis_field_TX_IF_SEL_set        ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_TX_IF_SEL_get        ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_TX_IF_SEL_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_TX_IF_SEL_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------+
 | | Register (0x000000f4 + (N) * 0x4) TX_SEL_FOR_SERDES_LANES_0_11 |
 | |     N[0..11]                                                   |
 | +----------------------------------------------------------------+
 | | bit  3:0 R/W  TX_SEL                                           |
 | +----------------------------------------------------------------+
 | 
 | void   line_dsis_field_TX_SEL_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_TX_SEL_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_TX_SEL_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_TX_SEL_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register (0x00000124 + (N) * 0x4)     |
 | | TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19 |
 | |     N[0..7]                           |
 | +---------------------------------------+
 | | bit  5:4 R/W  ACB_O_SEL               |
 | | bit  1:0 R/W  TX_O_SEL                |
 | +---------------------------------------+
 | 
 | void   line_dsis_field_ACB_O_SEL_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_ACB_O_SEL_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_ACB_O_SEL_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_ACB_O_SEL_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   line_dsis_field_TX_O_SEL_set        ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_TX_O_SEL_get        ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_TX_O_SEL_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_TX_O_SEL_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000144 SERDES_TX_LANE_EN |
 | +---------------------------------------+
 | | bit  19:0 R/W  TX_LANE_ENABLE         |
 | +---------------------------------------+
 | 
 | void   line_dsis_field_TX_LANE_ENABLE_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_TX_LANE_ENABLE_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_range_TX_LANE_ENABLE_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_TX_LANE_ENABLE_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x00000148 + (N) * 0x4) TX_FIFO_WRITE_CLK_SELECT |
 | |     N[0..19]                                               |
 | +------------------------------------------------------------+
 | | bit  4:0 R/W  TX_FIFO_WRITE_CLK_SEL                        |
 | +------------------------------------------------------------+
 | 
 | void   line_dsis_field_TX_FIFO_WRITE_CLK_SEL_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_TX_FIFO_WRITE_CLK_SEL_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_TX_FIFO_WRITE_CLK_SEL_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000198 TX_FIFO_RESET |
 | +-----------------------------------+
 | | bit  19:0 R/W  TX_FIFO_RESET      |
 | +-----------------------------------+
 | 
 | void   line_dsis_field_TX_FIFO_RESET_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_TX_FIFO_RESET_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_range_TX_FIFO_RESET_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_TX_FIFO_RESET_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0000019c TX_FIFO_MODE_SELECT |
 | +-----------------------------------------+
 | | bit  19:0 R/W  TX_FIFO_MODE_SEL         |
 | +-----------------------------------------+
 | 
 | void   line_dsis_field_TX_FIFO_MODE_SEL_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_TX_FIFO_MODE_SEL_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_range_TX_FIFO_MODE_SEL_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_TX_FIFO_MODE_SEL_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x000001a0 TX_FIFO_CENTERING_MODE |
 | +--------------------------------------------+
 | | bit  19:0 R/W  TX_FIFO_AUTO_CENTER         |
 | +--------------------------------------------+
 | 
 | void   line_dsis_field_TX_FIFO_AUTO_CENTER_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_TX_FIFO_AUTO_CENTER_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_range_TX_FIFO_AUTO_CENTER_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_TX_FIFO_AUTO_CENTER_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x000001a4 TX_FIFO_CENTERING_REQUEST |
 | +-----------------------------------------------+
 | | bit  19:0 R/W  TX_FIFO_CENTER_REQUEST         |
 | +-----------------------------------------------+
 | 
 | void   line_dsis_field_TX_FIFO_CENTER_REQUEST_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_TX_FIFO_CENTER_REQUEST_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_range_TX_FIFO_CENTER_REQUEST_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_TX_FIFO_CENTER_REQUEST_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------------+
 | | Register (0x000001a8 + (N) * 0x4) TX_FIFO_READ_POINTER_FOR_TXFIFO_0_19 |
 | |     N[0..19]                                                           |
 | +------------------------------------------------------------------------+
 | | bit  2:0 R/W  TX_FIFO_READ_PTR                                         |
 | +------------------------------------------------------------------------+
 | 
 | void   line_dsis_field_TX_FIFO_READ_PTR_set       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 line_dsis_field_TX_FIFO_READ_PTR_get       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | void   line_dsis_field_range_TX_FIFO_READ_PTR_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 line_dsis_field_range_TX_FIFO_READ_PTR_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x000001f8 TX_FIFO_0_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_0_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_0_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_0_I      |
 | | bit  1 R/W  RP_BELOW_EXP_0_I      |
 | | bit  0 R/W  RP_IN_SYNC_0_I        |
 | +-----------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_0_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_0_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_0_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_0_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_0_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_0_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_0_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_0_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_0_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_0_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_0_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_0_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_0_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_0_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_0_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x000001fc TX_FIFO_1_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_1_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_1_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_1_I      |
 | | bit  1 R/W  RP_BELOW_EXP_1_I      |
 | | bit  0 R/W  RP_IN_SYNC_1_I        |
 | +-----------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_1_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_1_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_1_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_1_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_1_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_1_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_1_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_1_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_1_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_1_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_1_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_1_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_1_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_1_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_1_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00000200 TX_FIFO_2_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_2_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_2_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_2_I      |
 | | bit  1 R/W  RP_BELOW_EXP_2_I      |
 | | bit  0 R/W  RP_IN_SYNC_2_I        |
 | +-----------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_2_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_2_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_2_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_2_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_2_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_2_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_2_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_2_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_2_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_2_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_2_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_2_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_2_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_2_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_2_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00000204 TX_FIFO_3_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_3_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_3_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_3_I      |
 | | bit  1 R/W  RP_BELOW_EXP_3_I      |
 | | bit  0 R/W  RP_IN_SYNC_3_I        |
 | +-----------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_3_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_3_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_3_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_3_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_3_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_3_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_3_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_3_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_3_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_3_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_3_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_3_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_3_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_3_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_3_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00000208 TX_FIFO_4_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_4_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_4_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_4_I      |
 | | bit  1 R/W  RP_BELOW_EXP_4_I      |
 | | bit  0 R/W  RP_IN_SYNC_4_I        |
 | +-----------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_4_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_4_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_4_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_4_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_4_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_4_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_4_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_4_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_4_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_4_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_4_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_4_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_4_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_4_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_4_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x0000020c TX_FIFO_5_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_5_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_5_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_5_I      |
 | | bit  1 R/W  RP_BELOW_EXP_5_I      |
 | | bit  0 R/W  RP_IN_SYNC_5_I        |
 | +-----------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_5_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_5_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_5_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_5_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_5_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_5_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_5_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_5_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_5_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_5_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_5_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_5_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_5_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_5_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_5_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00000210 TX_FIFO_6_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_6_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_6_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_6_I      |
 | | bit  1 R/W  RP_BELOW_EXP_6_I      |
 | | bit  0 R/W  RP_IN_SYNC_6_I        |
 | +-----------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_6_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_6_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_6_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_6_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_6_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_6_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_6_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_6_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_6_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_6_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_6_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_6_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_6_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_6_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_6_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00000214 TX_FIFO_7_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_7_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_7_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_7_I      |
 | | bit  1 R/W  RP_BELOW_EXP_7_I      |
 | | bit  0 R/W  RP_IN_SYNC_7_I        |
 | +-----------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_7_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_7_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_7_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_7_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_7_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_7_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_7_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_7_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_7_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_7_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_7_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_7_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_7_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_7_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_7_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00000218 TX_FIFO_8_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_8_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_8_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_8_I      |
 | | bit  1 R/W  RP_BELOW_EXP_8_I      |
 | | bit  0 R/W  RP_IN_SYNC_8_I        |
 | +-----------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_8_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_8_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_8_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_8_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_8_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_8_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_8_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_8_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_8_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_8_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_8_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_8_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_8_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_8_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_8_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x0000021c TX_FIFO_9_INT |
 | +-----------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_9_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_9_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_9_I      |
 | | bit  1 R/W  RP_BELOW_EXP_9_I      |
 | | bit  0 R/W  RP_IN_SYNC_9_I        |
 | +-----------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_9_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_9_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_9_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_9_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_9_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_9_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_9_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_9_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_9_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_9_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_9_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_9_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_9_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_9_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_9_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00000220 TX_FIFO_10_INT |
 | +------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_10_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_10_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_10_I      |
 | | bit  1 R/W  RP_BELOW_EXP_10_I      |
 | | bit  0 R/W  RP_IN_SYNC_10_I        |
 | +------------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_10_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_10_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_10_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_10_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_10_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_10_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_10_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_10_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_10_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_10_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_10_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_10_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_10_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_10_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_10_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00000224 TX_FIFO_11_INT |
 | +------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_11_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_11_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_11_I      |
 | | bit  1 R/W  RP_BELOW_EXP_11_I      |
 | | bit  0 R/W  RP_IN_SYNC_11_I        |
 | +------------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_11_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_11_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_11_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_11_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_11_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_11_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_11_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_11_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_11_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_11_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_11_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_11_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_11_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_11_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_11_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00000228 TX_FIFO_12_INT |
 | +------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_12_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_12_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_12_I      |
 | | bit  1 R/W  RP_BELOW_EXP_12_I      |
 | | bit  0 R/W  RP_IN_SYNC_12_I        |
 | +------------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_12_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_12_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_12_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_12_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_12_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_12_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_12_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_12_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_12_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_12_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_12_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_12_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_12_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_12_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_12_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x0000022c TX_FIFO_13_INT |
 | +------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_13_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_13_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_13_I      |
 | | bit  1 R/W  RP_BELOW_EXP_13_I      |
 | | bit  0 R/W  RP_IN_SYNC_13_I        |
 | +------------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_13_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_13_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_13_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_13_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_13_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_13_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_13_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_13_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_13_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_13_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_13_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_13_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_13_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_13_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_13_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00000230 TX_FIFO_14_INT |
 | +------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_14_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_14_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_14_I      |
 | | bit  1 R/W  RP_BELOW_EXP_14_I      |
 | | bit  0 R/W  RP_IN_SYNC_14_I        |
 | +------------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_14_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_14_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_14_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_14_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_14_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_14_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_14_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_14_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_14_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_14_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_14_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_14_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_14_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_14_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_14_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00000234 TX_FIFO_15_INT |
 | +------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_15_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_15_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_15_I      |
 | | bit  1 R/W  RP_BELOW_EXP_15_I      |
 | | bit  0 R/W  RP_IN_SYNC_15_I        |
 | +------------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_15_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_15_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_15_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_15_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_15_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_15_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_15_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_15_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_15_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_15_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_15_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_15_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_15_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_15_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_15_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00000238 TX_FIFO_16_INT |
 | +------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_16_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_16_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_16_I      |
 | | bit  1 R/W  RP_BELOW_EXP_16_I      |
 | | bit  0 R/W  RP_IN_SYNC_16_I        |
 | +------------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_16_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_16_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_16_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_16_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_16_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_16_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_16_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_16_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_16_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_16_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_16_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_16_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_16_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_16_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_16_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x0000023c TX_FIFO_17_INT |
 | +------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_17_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_17_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_17_I      |
 | | bit  1 R/W  RP_BELOW_EXP_17_I      |
 | | bit  0 R/W  RP_IN_SYNC_17_I        |
 | +------------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_17_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_17_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_17_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_17_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_17_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_17_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_17_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_17_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_17_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_17_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_17_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_17_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_17_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_17_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_17_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00000240 TX_FIFO_18_INT |
 | +------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_18_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_18_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_18_I      |
 | | bit  1 R/W  RP_BELOW_EXP_18_I      |
 | | bit  0 R/W  RP_IN_SYNC_18_I        |
 | +------------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_18_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_18_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_18_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_18_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_18_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_18_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_18_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_18_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_18_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_18_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_18_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_18_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_18_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_18_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_18_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00000244 TX_FIFO_19_INT |
 | +------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_19_I   |
 | | bit  3 R/W  RP_OUT_OF_SYNC_19_I    |
 | | bit  2 R/W  RP_ABOVE_EXP_19_I      |
 | | bit  1 R/W  RP_BELOW_EXP_19_I      |
 | | bit  0 R/W  RP_IN_SYNC_19_I        |
 | +------------------------------------+
 | 
 | void                 line_dsis_field_RP_NOT_CENTERED_19_I_set_to_clear ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_NOT_CENTERED_19_I_get          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_NOT_CENTERED_19_I_poll         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_OUT_OF_SYNC_19_I_set_to_clear  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_OUT_OF_SYNC_19_I_get           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_OUT_OF_SYNC_19_I_poll          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_ABOVE_EXP_19_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_ABOVE_EXP_19_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_ABOVE_EXP_19_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_BELOW_EXP_19_I_set_to_clear    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_BELOW_EXP_19_I_get             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_BELOW_EXP_19_I_poll            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 line_dsis_field_RP_IN_SYNC_19_I_set_to_clear      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32               line_dsis_field_RP_IN_SYNC_19_I_get               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_field_RP_IN_SYNC_19_I_poll              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------+
 | | Register 0x00000248 TX_FIFO_0_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_0_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_0_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_0_E         |
 | | bit  1 R/W  RP_BELOW_EXP_0_E         |
 | | bit  0 R/W  RP_IN_SYNC_0_E           |
 | +--------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_0_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_0_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_0_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_0_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_0_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_0_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_0_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_0_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_0_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_0_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x0000024c TX_FIFO_1_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_1_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_1_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_1_E         |
 | | bit  1 R/W  RP_BELOW_EXP_1_E         |
 | | bit  0 R/W  RP_IN_SYNC_1_E           |
 | +--------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_1_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_1_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_1_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_1_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_1_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_1_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_1_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_1_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_1_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_1_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000250 TX_FIFO_2_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_2_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_2_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_2_E         |
 | | bit  1 R/W  RP_BELOW_EXP_2_E         |
 | | bit  0 R/W  RP_IN_SYNC_2_E           |
 | +--------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_2_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_2_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_2_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_2_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_2_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_2_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_2_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_2_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_2_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_2_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000254 TX_FIFO_3_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_3_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_3_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_3_E         |
 | | bit  1 R/W  RP_BELOW_EXP_3_E         |
 | | bit  0 R/W  RP_IN_SYNC_3_E           |
 | +--------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_3_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_3_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_3_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_3_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_3_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_3_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_3_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_3_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_3_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_3_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000258 TX_FIFO_4_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_4_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_4_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_4_E         |
 | | bit  1 R/W  RP_BELOW_EXP_4_E         |
 | | bit  0 R/W  RP_IN_SYNC_4_E           |
 | +--------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_4_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_4_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_4_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_4_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_4_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_4_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_4_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_4_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_4_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_4_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x0000025c TX_FIFO_5_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_5_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_5_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_5_E         |
 | | bit  1 R/W  RP_BELOW_EXP_5_E         |
 | | bit  0 R/W  RP_IN_SYNC_5_E           |
 | +--------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_5_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_5_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_5_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_5_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_5_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_5_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_5_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_5_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_5_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_5_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000260 TX_FIFO_6_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_6_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_6_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_6_E         |
 | | bit  1 R/W  RP_BELOW_EXP_6_E         |
 | | bit  0 R/W  RP_IN_SYNC_6_E           |
 | +--------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_6_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_6_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_6_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_6_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_6_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_6_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_6_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_6_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_6_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_6_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000264 TX_FIFO_7_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_7_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_7_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_7_E         |
 | | bit  1 R/W  RP_BELOW_EXP_7_E         |
 | | bit  0 R/W  RP_IN_SYNC_7_E           |
 | +--------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_7_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_7_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_7_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_7_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_7_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_7_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_7_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_7_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_7_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_7_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000268 TX_FIFO_8_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_8_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_8_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_8_E         |
 | | bit  1 R/W  RP_BELOW_EXP_8_E         |
 | | bit  0 R/W  RP_IN_SYNC_8_E           |
 | +--------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_8_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_8_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_8_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_8_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_8_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_8_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_8_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_8_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_8_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_8_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x0000026c TX_FIFO_9_INT_EN |
 | +--------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_9_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_9_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_9_E         |
 | | bit  1 R/W  RP_BELOW_EXP_9_E         |
 | | bit  0 R/W  RP_IN_SYNC_9_E           |
 | +--------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_9_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_9_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_9_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_9_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_9_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_9_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_9_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_9_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_9_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_9_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000270 TX_FIFO_10_INT_EN |
 | +---------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_10_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_10_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_10_E         |
 | | bit  1 R/W  RP_BELOW_EXP_10_E         |
 | | bit  0 R/W  RP_IN_SYNC_10_E           |
 | +---------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_10_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_10_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_10_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_10_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_10_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_10_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_10_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_10_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_10_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_10_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000274 TX_FIFO_11_INT_EN |
 | +---------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_11_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_11_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_11_E         |
 | | bit  1 R/W  RP_BELOW_EXP_11_E         |
 | | bit  0 R/W  RP_IN_SYNC_11_E           |
 | +---------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_11_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_11_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_11_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_11_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_11_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_11_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_11_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_11_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_11_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_11_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000278 TX_FIFO_12_INT_EN |
 | +---------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_12_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_12_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_12_E         |
 | | bit  1 R/W  RP_BELOW_EXP_12_E         |
 | | bit  0 R/W  RP_IN_SYNC_12_E           |
 | +---------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_12_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_12_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_12_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_12_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_12_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_12_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_12_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_12_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_12_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_12_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x0000027c TX_FIFO_13_INT_EN |
 | +---------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_13_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_13_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_13_E         |
 | | bit  1 R/W  RP_BELOW_EXP_13_E         |
 | | bit  0 R/W  RP_IN_SYNC_13_E           |
 | +---------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_13_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_13_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_13_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_13_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_13_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_13_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_13_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_13_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_13_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_13_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000280 TX_FIFO_14_INT_EN |
 | +---------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_14_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_14_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_14_E         |
 | | bit  1 R/W  RP_BELOW_EXP_14_E         |
 | | bit  0 R/W  RP_IN_SYNC_14_E           |
 | +---------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_14_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_14_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_14_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_14_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_14_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_14_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_14_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_14_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_14_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_14_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000284 TX_FIFO_15_INT_EN |
 | +---------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_15_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_15_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_15_E         |
 | | bit  1 R/W  RP_BELOW_EXP_15_E         |
 | | bit  0 R/W  RP_IN_SYNC_15_E           |
 | +---------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_15_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_15_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_15_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_15_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_15_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_15_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_15_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_15_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_15_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_15_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000288 TX_FIFO_16_INT_EN |
 | +---------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_16_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_16_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_16_E         |
 | | bit  1 R/W  RP_BELOW_EXP_16_E         |
 | | bit  0 R/W  RP_IN_SYNC_16_E           |
 | +---------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_16_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_16_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_16_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_16_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_16_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_16_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_16_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_16_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_16_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_16_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x0000028c TX_FIFO_17_INT_EN |
 | +---------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_17_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_17_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_17_E         |
 | | bit  1 R/W  RP_BELOW_EXP_17_E         |
 | | bit  0 R/W  RP_IN_SYNC_17_E           |
 | +---------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_17_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_17_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_17_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_17_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_17_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_17_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_17_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_17_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_17_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_17_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000290 TX_FIFO_18_INT_EN |
 | +---------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_18_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_18_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_18_E         |
 | | bit  1 R/W  RP_BELOW_EXP_18_E         |
 | | bit  0 R/W  RP_IN_SYNC_18_E           |
 | +---------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_18_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_18_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_18_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_18_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_18_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_18_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_18_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_18_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_18_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_18_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000294 TX_FIFO_19_INT_EN |
 | +---------------------------------------+
 | | bit  4 R/W  RP_NOT_CENTERED_19_E      |
 | | bit  3 R/W  RP_OUT_OF_SYNC_19_E       |
 | | bit  2 R/W  RP_ABOVE_EXP_19_E         |
 | | bit  1 R/W  RP_BELOW_EXP_19_E         |
 | | bit  0 R/W  RP_IN_SYNC_19_E           |
 | +---------------------------------------+
 | 
 | void   line_dsis_field_RP_NOT_CENTERED_19_E_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_NOT_CENTERED_19_E_get ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_OUT_OF_SYNC_19_E_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_OUT_OF_SYNC_19_E_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_ABOVE_EXP_19_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_ABOVE_EXP_19_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_BELOW_EXP_19_E_set    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_BELOW_EXP_19_E_get    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void   line_dsis_field_RP_IN_SYNC_19_E_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | UINT32 line_dsis_field_RP_IN_SYNC_19_E_get      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00000298 INT_SUMMARY |
 | +---------------------------------+
 | | bit  19:0 R  TX_FIFO_INT        |
 | +---------------------------------+
 | 
 | UINT32               line_dsis_field_TX_FIFO_INT_get        ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | UINT32               line_dsis_field_range_TX_FIFO_INT_get  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE line_dsis_field_range_TX_FIFO_INT_poll ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE line_dsis_field_TX_FIFO_INT_poll       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _LINE_DSIS_IO_H
#define _LINE_DSIS_IO_H

#include "lifd_api.h"
#include "line_dsis_regs.h"
#include "line_dsis_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 line_dsis_buffer_init               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | void                 line_dsis_buffer_flush              ( line_dsis_buffer_t *b_ptr )
 | UINT32               line_dsis_reg_read                  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 line_dsis_reg_write                 ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 line_dsis_field_set                 ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 line_dsis_action_on_write_field_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 line_dsis_burst_read                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 line_dsis_burst_write               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE line_dsis_poll                      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 line_dsis_reg_ENET_TX_CLK_SELECT_write                              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_ENET_TX_CLK_SELECT_field_set                          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_ENET_TX_CLK_SELECT_read                               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_SERDES_TX_LANE_EN_write                               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_SERDES_TX_LANE_EN_field_set                           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_SERDES_TX_LANE_EN_read                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_RESET_write                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_RESET_field_set                               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_RESET_read                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_MODE_SELECT_write                             ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_MODE_SELECT_field_set                         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_MODE_SELECT_read                              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_CENTERING_MODE_write                          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_CENTERING_MODE_field_set                      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_CENTERING_MODE_read                           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_CENTERING_REQUEST_write                       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_CENTERING_REQUEST_field_set                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_CENTERING_REQUEST_read                        ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_write       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 line_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_field_set   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_RX_SEL_AND_CLK_EN_FOR_SS_LANES_0_11_array_read        ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 line_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_write         ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 line_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_field_set     ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_RX_FIRST_LEVEL_MUX_FOR_LANES_0_11_array_read          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 line_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_write          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 line_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_field_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_CLK_SEL_AND_EN_FOR_LANES_0_11_array_read           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 line_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_write          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 line_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_field_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_CLK_PREMUX_SEL_FOR_LANES_0_11_array_read           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 line_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_write          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 line_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_field_set      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_AND_ACB_SS_SEL_FOR_LANES_0_11_array_read           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 line_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_write              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 line_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_field_set          ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_SEL_FOR_SERDES_LANES_0_11_array_read               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 line_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_write                  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_field_set              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_WRITE_CLK_SELECT_array_read                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 line_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_0_19_array_write      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_0_19_array_field_set  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_READ_POINTER_FOR_TXFIFO_0_19_array_read       ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 line_dsis_reg_TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19_array_write     ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 line_dsis_reg_TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19_array_field_set ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_AND_ACB_SEL_FOR_SERDES_LANES_12_19_array_read      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32  N )
 | 
 | void                 line_dsis_reg_TX_FIFO_0_INT_EN_write                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_0_INT_EN_field_set                            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_0_INT_EN_read                                 ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_1_INT_EN_write                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_1_INT_EN_field_set                            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_1_INT_EN_read                                 ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_2_INT_EN_write                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_2_INT_EN_field_set                            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_2_INT_EN_read                                 ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_3_INT_EN_write                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_3_INT_EN_field_set                            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_3_INT_EN_read                                 ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_4_INT_EN_write                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_4_INT_EN_field_set                            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_4_INT_EN_read                                 ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_5_INT_EN_write                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_5_INT_EN_field_set                            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_5_INT_EN_read                                 ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_6_INT_EN_write                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_6_INT_EN_field_set                            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_6_INT_EN_read                                 ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_7_INT_EN_write                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_7_INT_EN_field_set                            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_7_INT_EN_read                                 ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_8_INT_EN_write                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_8_INT_EN_field_set                            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_8_INT_EN_read                                 ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_9_INT_EN_write                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_9_INT_EN_field_set                            ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_9_INT_EN_read                                 ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_10_INT_EN_write                               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_10_INT_EN_field_set                           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_10_INT_EN_read                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_11_INT_EN_write                               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_11_INT_EN_field_set                           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_11_INT_EN_read                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_12_INT_EN_write                               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_12_INT_EN_field_set                           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_12_INT_EN_read                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_13_INT_EN_write                               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_13_INT_EN_field_set                           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_13_INT_EN_read                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_14_INT_EN_write                               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_14_INT_EN_field_set                           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_14_INT_EN_read                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_15_INT_EN_write                               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_15_INT_EN_field_set                           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_15_INT_EN_read                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_16_INT_EN_write                               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_16_INT_EN_field_set                           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_16_INT_EN_read                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_17_INT_EN_write                               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_17_INT_EN_field_set                           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_17_INT_EN_read                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_18_INT_EN_write                               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_18_INT_EN_field_set                           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_18_INT_EN_read                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | void                 line_dsis_reg_TX_FIFO_19_INT_EN_write                               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_19_INT_EN_field_set                           ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_19_INT_EN_read                                ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | 
 | UINT32               line_dsis_reg_INT_SUMMARY_read                                      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_INT_SUMMARY_poll                                      ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_0_INT_write                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_0_INT_action_on_write_field_set               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_0_INT_read                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_0_INT_poll                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_1_INT_write                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_1_INT_action_on_write_field_set               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_1_INT_read                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_1_INT_poll                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_2_INT_write                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_2_INT_action_on_write_field_set               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_2_INT_read                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_2_INT_poll                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_3_INT_write                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_3_INT_action_on_write_field_set               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_3_INT_read                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_3_INT_poll                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_4_INT_write                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_4_INT_action_on_write_field_set               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_4_INT_read                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_4_INT_poll                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_5_INT_write                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_5_INT_action_on_write_field_set               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_5_INT_read                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_5_INT_poll                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_6_INT_write                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_6_INT_action_on_write_field_set               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_6_INT_read                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_6_INT_poll                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_7_INT_write                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_7_INT_action_on_write_field_set               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_7_INT_read                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_7_INT_poll                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_8_INT_write                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_8_INT_action_on_write_field_set               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_8_INT_read                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_8_INT_poll                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_9_INT_write                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_9_INT_action_on_write_field_set               ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_9_INT_read                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_9_INT_poll                                    ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_10_INT_write                                  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_10_INT_action_on_write_field_set              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_10_INT_read                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_10_INT_poll                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_11_INT_write                                  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_11_INT_action_on_write_field_set              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_11_INT_read                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_11_INT_poll                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_12_INT_write                                  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_12_INT_action_on_write_field_set              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_12_INT_read                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_12_INT_poll                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_13_INT_write                                  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_13_INT_action_on_write_field_set              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_13_INT_read                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_13_INT_poll                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_14_INT_write                                  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_14_INT_action_on_write_field_set              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_14_INT_read                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_14_INT_poll                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_15_INT_write                                  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_15_INT_action_on_write_field_set              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_15_INT_read                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_15_INT_poll                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_16_INT_write                                  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_16_INT_action_on_write_field_set              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_16_INT_read                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_16_INT_poll                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_17_INT_write                                  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_17_INT_action_on_write_field_set              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_17_INT_read                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_17_INT_poll                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_18_INT_write                                  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_18_INT_action_on_write_field_set              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_18_INT_read                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_18_INT_poll                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 line_dsis_reg_TX_FIFO_19_INT_write                                  ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 value )
 | void                 line_dsis_reg_TX_FIFO_19_INT_action_on_write_field_set              ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               line_dsis_reg_TX_FIFO_19_INT_read                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE line_dsis_reg_TX_FIFO_19_INT_poll                                   ( line_dsis_buffer_t *b_ptr, lifd_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _LINE_DSIS_IO_H */
