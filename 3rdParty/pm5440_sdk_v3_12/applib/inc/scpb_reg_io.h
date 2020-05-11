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
 *     and register accessor functions for the scpb_reg block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing scpb_reg io functions is:
 | {
 |     scpb_reg_buffer_t b_ptr[1];
 |     scpb_reg_buffer_init( b_ptr, h_ptr );
 |     value = scpb_reg_field_<FIELD>_get( b_ptr, h_ptr );
 |     scpb_reg_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     scpb_reg_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = scpb_reg_field_<FIELD>_get( NULL, h_ptr );
 | scpb_reg_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------------------------------+
 | | Register (0x00100000 + (N) * 0x100 + (M) * 0x4) BLOCK_TABLE_REG |
 | |     N[0..149]                                                   |
 | |     M[0..63]                                                    |
 | +-----------------------------------------------------------------+
 | | bit  31    R/W  IPT_BLOCK_VALID                                 |
 | | bit  30    R    IPT_BLOCK_ACTIVE                                |
 | | bit  23:16 R/W  IPT_WQID                                        |
 | | bit  0:10  R/W  IPT_LBA                                         |
 | | bit  0     R/W  WPTR_RST_PUSH_P0                                |
 | +-----------------------------------------------------------------+
 | 
 | void   scpb_reg_field_IPT_BLOCK_VALID_set  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 scpb_reg_field_IPT_BLOCK_VALID_get  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   scpb_reg_field_IPT_WQID_set         ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 scpb_reg_field_IPT_WQID_get         ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   scpb_reg_field_range_IPT_WQID_set   ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_IPT_WQID_get   ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | void   scpb_reg_field_IPT_LBA_set          ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 scpb_reg_field_IPT_LBA_get          ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   scpb_reg_field_range_IPT_LBA_set    ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_IPT_LBA_get    ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | void   scpb_reg_field_WPTR_RST_PUSH_P0_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 scpb_reg_field_WPTR_RST_PUSH_P0_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M )
 | UINT32 scpb_reg_field_IPT_BLOCK_ACTIVE_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00100400 + (N) * 0x4) PUSH_PORT_P0_RST_VAL |
 | |     N[0..7]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R  WPTR_RST_PUSH_P0_VAL                      |
 | +--------------------------------------------------------+
 | 
 | UINT32 scpb_reg_field_WPTR_RST_PUSH_P0_VAL_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | UINT32 scpb_reg_field_range_WPTR_RST_PUSH_P0_VAL_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00100800 + (N) * 0x4) PUSH_PORT_P1_RST |
 | |     N[0..149]                                      |
 | +----------------------------------------------------+
 | | bit  0 R/W  WPTR_RST_PUSH_P1                       |
 | +----------------------------------------------------+
 | 
 | void   scpb_reg_field_WPTR_RST_PUSH_P1_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 scpb_reg_field_WPTR_RST_PUSH_P1_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00100c00 + (N) * 0x4) PUSH_PORT_P1_RST_VAL |
 | |     N[0..7]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R  WPTR_RST_PUSH_P1_VAL                      |
 | +--------------------------------------------------------+
 | 
 | UINT32 scpb_reg_field_WPTR_RST_PUSH_P1_VAL_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | UINT32 scpb_reg_field_range_WPTR_RST_PUSH_P1_VAL_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00101000 + (N) * 0x4) PUSH_PORT_P2_RST |
 | |     N[0..119]                                      |
 | +----------------------------------------------------+
 | | bit  0 R/W  WPTR_RST_PUSH_P2                       |
 | +----------------------------------------------------+
 | 
 | void   scpb_reg_field_WPTR_RST_PUSH_P2_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 scpb_reg_field_WPTR_RST_PUSH_P2_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00101400 + (N) * 0x4) PUSH_PORT_P2_RST_VAL |
 | |     N[0..7]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R  WPTR_RST_PUSH_P2_VAL                      |
 | +--------------------------------------------------------+
 | 
 | UINT32 scpb_reg_field_WPTR_RST_PUSH_P2_VAL_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | UINT32 scpb_reg_field_range_WPTR_RST_PUSH_P2_VAL_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00101800 + (N) * 0x4) PUSH_PORT_P3_RST |
 | |     N[0..107]                                      |
 | +----------------------------------------------------+
 | | bit  0 R/W  WPTR_RST_PUSH_P3                       |
 | +----------------------------------------------------+
 | 
 | void   scpb_reg_field_WPTR_RST_PUSH_P3_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 scpb_reg_field_WPTR_RST_PUSH_P3_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00101c00 + (N) * 0x4) PUSH_PORT_P3_RST_VAL |
 | |     N[0..7]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R  WPTR_RST_PUSH_P3_VAL                      |
 | +--------------------------------------------------------+
 | 
 | UINT32 scpb_reg_field_WPTR_RST_PUSH_P3_VAL_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | UINT32 scpb_reg_field_range_WPTR_RST_PUSH_P3_VAL_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00102000 + (N) * 0x4) POP_PORT_RST |
 | |     N[0..95]                                   |
 | +------------------------------------------------+
 | | bit  0 R/W  RPTR_RST_POP                       |
 | +------------------------------------------------+
 | 
 | void   scpb_reg_field_RPTR_RST_POP_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 scpb_reg_field_RPTR_RST_POP_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00102400 + (N) * 0x4) RPTR_RST_POP_VAL_REG |
 | |     N[0..7]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R  RPTR_RST_POP_VAL                          |
 | +--------------------------------------------------------+
 | 
 | UINT32 scpb_reg_field_RPTR_RST_POP_VAL_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | UINT32 scpb_reg_field_range_RPTR_RST_POP_VAL_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00102600 + (N) * 0x4) FIFO_PTR_EMPTY_REG |
 | |     N[0..7]                                          |
 | +------------------------------------------------------+
 | | bit  31:0 R  FIFO_PKT_EMPTY                          |
 | +------------------------------------------------------+
 | 
 | UINT32               scpb_reg_field_FIFO_PKT_EMPTY_get        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | UINT32               scpb_reg_field_range_FIFO_PKT_EMPTY_get  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_range_FIFO_PKT_EMPTY_poll ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_FIFO_PKT_EMPTY_poll       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------+
 | | Register 0x00102800 FIFO_INT_REG |
 | +----------------------------------+
 | | bit  8   R/W  FIFO_UDR_I         |
 | | bit  3:0 R/W  FIFO_OVR_PUSH_I    |
 | +----------------------------------+
 | 
 | void                 scpb_reg_field_FIFO_UDR_I_set_to_clear            ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32               scpb_reg_field_FIFO_UDR_I_get                     ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_FIFO_UDR_I_poll                    ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 scpb_reg_field_FIFO_OVR_PUSH_I_set_to_clear       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32               scpb_reg_field_FIFO_OVR_PUSH_I_get                ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void                 scpb_reg_field_range_FIFO_OVR_PUSH_I_set_to_clear ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               scpb_reg_field_range_FIFO_OVR_PUSH_I_get          ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_range_FIFO_OVR_PUSH_I_poll         ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_FIFO_OVR_PUSH_I_poll               ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00102804 FIFO_INT_EN_REG |
 | +-------------------------------------+
 | | bit  8   R/W  FIFO_UDR_E            |
 | | bit  3:0 R/W  FIFO_OVR_PUSH_E       |
 | +-------------------------------------+
 | 
 | void   scpb_reg_field_FIFO_UDR_E_set            ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_FIFO_UDR_E_get            ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_FIFO_OVR_PUSH_E_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_FIFO_OVR_PUSH_E_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_FIFO_OVR_PUSH_E_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_FIFO_OVR_PUSH_E_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00103000 + (N) * 0x4) FIFO_UDR_STAT_REG_0 |
 | |     N[0..7]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R  FIFO_UDR_STATUS                          |
 | +-------------------------------------------------------+
 | 
 | UINT32 scpb_reg_field_FIFO_UDR_STATUS_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | UINT32 scpb_reg_field_range_FIFO_UDR_STATUS_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00103100 + (N) * 0x4) P0_FIFO_OVR_STAT_REG |
 | |     N[0..7]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R  P0_FIFO_OVR_STATUS                        |
 | +--------------------------------------------------------+
 | 
 | UINT32 scpb_reg_field_P0_FIFO_OVR_STATUS_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | UINT32 scpb_reg_field_range_P0_FIFO_OVR_STATUS_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00103200 + (N) * 0x4) P1_FIFO_OVR_STAT_REG |
 | |     N[0..7]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R  P1_FIFO_OVR_STATUS                        |
 | +--------------------------------------------------------+
 | 
 | UINT32 scpb_reg_field_P1_FIFO_OVR_STATUS_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | UINT32 scpb_reg_field_range_P1_FIFO_OVR_STATUS_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00103400 + (N) * 0x4) P2_FIFO_OVR_STAT_REG |
 | |     N[0..7]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R  P2_FIFO_OVR_STATUS                        |
 | +--------------------------------------------------------+
 | 
 | UINT32 scpb_reg_field_P2_FIFO_OVR_STATUS_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | UINT32 scpb_reg_field_range_P2_FIFO_OVR_STATUS_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00103500 + (N) * 0x4) P3_FIFO_OVR_STAT_REG |
 | |     N[0..7]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R  P3_FIFO_OVR_STATUS                        |
 | +--------------------------------------------------------+
 | 
 | UINT32 scpb_reg_field_P3_FIFO_OVR_STATUS_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | UINT32 scpb_reg_field_range_P3_FIFO_OVR_STATUS_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00103808 FIFO_IPT_BYPASS_REG |
 | +-----------------------------------------+
 | | bit  0 R/W  IPT_BYPASS                  |
 | +-----------------------------------------+
 | 
 | void   scpb_reg_field_IPT_BYPASS_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_IPT_BYPASS_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | +----------------------------------------+
 | | Register 0x00103d00 FIFO_CFG           |
 | +----------------------------------------+
 | | bit  28   R/W  FIFO_END_ADDR_CHG_ABORT |
 | | bit  24   R/W  FIFO_END_ADDR_CHG_REQ   |
 | | bit  16:0 R/W  FIFO_END_ADDR           |
 | +----------------------------------------+
 | 
 | void                 scpb_reg_field_FIFO_END_ADDR_set            ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32               scpb_reg_field_FIFO_END_ADDR_get            ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void                 scpb_reg_field_range_FIFO_END_ADDR_set      ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               scpb_reg_field_range_FIFO_END_ADDR_get      ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void                 scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_set  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32               scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_get  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_FIFO_END_ADDR_CHG_ABORT_poll ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 scpb_reg_field_FIFO_END_ADDR_CHG_REQ_set    ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32               scpb_reg_field_FIFO_END_ADDR_CHG_REQ_get    ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_FIFO_END_ADDR_CHG_REQ_poll   ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00103d04 FIFO_CFG_RID    |
 | +-------------------------------------+
 | | bit  7:0 R/W  FIFO_END_ADDR_CHG_RID |
 | +-------------------------------------+
 | 
 | void   scpb_reg_field_FIFO_END_ADDR_CHG_RID_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_FIFO_END_ADDR_CHG_RID_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_FIFO_END_ADDR_CHG_RID_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00103d08 FIFO_CFG_WID    |
 | +-------------------------------------+
 | | bit  9:0 R/W  FIFO_END_ADDR_CHG_WID |
 | +-------------------------------------+
 | 
 | void   scpb_reg_field_FIFO_END_ADDR_CHG_WID_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_FIFO_END_ADDR_CHG_WID_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_FIFO_END_ADDR_CHG_WID_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00103d0c FIFO_CFG_STAT   |
 | +-------------------------------------+
 | | bit  0 R  FIFO_END_ADDR_CHG_SUCCESS |
 | +-------------------------------------+
 | 
 | UINT32               scpb_reg_field_FIFO_END_ADDR_CHG_SUCCESS_get  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_FIFO_END_ADDR_CHG_SUCCESS_poll ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------+
 | | Register 0x00103e00 DBG_CTRL |
 | +------------------------------+
 | | bit  16   R/W  DBG_TRIG      |
 | | bit  10:0 R/W  DBG_RADDR     |
 | +------------------------------+
 | 
 | void   scpb_reg_field_DBG_RADDR_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_DBG_RADDR_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_DBG_RADDR_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_DBG_RADDR_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   scpb_reg_field_DBG_TRIG_set        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_DBG_TRIG_get        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x00103e04 DBG_RDATA |
 | +-------------------------------+
 | | bit  31:0 R  DBG_RDATA        |
 | +-------------------------------+
 | 
 | UINT32 scpb_reg_field_DBG_RDATA_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | UINT32 scpb_reg_field_range_DBG_RDATA_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00104000 + (N) * 0x4) PAGE_TABLE_REG |
 | |     N[0..31]                                     |
 | +--------------------------------------------------+
 | | bit  24    R/W  PAGE_VALID                       |
 | | bit  17:16 R/W  IPT_QS_ID                        |
 | | bit  9:8   R/W  IPT_WP_ID                        |
 | +--------------------------------------------------+
 | 
 | void   scpb_reg_field_PAGE_VALID_set      ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 scpb_reg_field_PAGE_VALID_get      ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | void   scpb_reg_field_IPT_QS_ID_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 scpb_reg_field_IPT_QS_ID_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | void   scpb_reg_field_range_IPT_QS_ID_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_IPT_QS_ID_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   scpb_reg_field_IPT_WP_ID_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 scpb_reg_field_IPT_WP_ID_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | void   scpb_reg_field_range_IPT_WP_ID_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_IPT_WP_ID_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00104100 PAGE_IPT_ERR_INT_REG |
 | +------------------------------------------+
 | | bit  0 R/W  PAGE_IPT_ERR_I               |
 | +------------------------------------------+
 | 
 | void                 scpb_reg_field_PAGE_IPT_ERR_I_set_to_clear ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32               scpb_reg_field_PAGE_IPT_ERR_I_get          ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_PAGE_IPT_ERR_I_poll         ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x00104104 BLOCK_IPT_ERR_INT_REG |
 | +-------------------------------------------+
 | | bit  31:0 R/W  BLOCK_IPT_ERR_I            |
 | +-------------------------------------------+
 | 
 | void                 scpb_reg_field_BLOCK_IPT_ERR_I_set_to_clear       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32               scpb_reg_field_BLOCK_IPT_ERR_I_get                ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void                 scpb_reg_field_range_BLOCK_IPT_ERR_I_set_to_clear ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               scpb_reg_field_range_BLOCK_IPT_ERR_I_get          ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_range_BLOCK_IPT_ERR_I_poll         ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_BLOCK_IPT_ERR_I_poll               ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x00104108 PREFETCH_FIFO_INT_REG |
 | +-------------------------------------------+
 | | bit  19:16 R/W  HOQ_DLY_OVERFLOW_I        |
 | | bit  11:8  R/W  HOQ_CFC_UNDERFLOW_I       |
 | | bit  3:0   R/W  HOQ_CFC_OVERFLOW_I        |
 | +-------------------------------------------+
 | 
 | void                 scpb_reg_field_HOQ_DLY_OVERFLOW_I_set_to_clear        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32               scpb_reg_field_HOQ_DLY_OVERFLOW_I_get                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void                 scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_set_to_clear  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_get           ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_range_HOQ_DLY_OVERFLOW_I_poll          ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_HOQ_DLY_OVERFLOW_I_poll                ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 scpb_reg_field_HOQ_CFC_UNDERFLOW_I_set_to_clear       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32               scpb_reg_field_HOQ_CFC_UNDERFLOW_I_get                ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void                 scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_set_to_clear ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_get          ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_range_HOQ_CFC_UNDERFLOW_I_poll         ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_HOQ_CFC_UNDERFLOW_I_poll               ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 scpb_reg_field_HOQ_CFC_OVERFLOW_I_set_to_clear        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32               scpb_reg_field_HOQ_CFC_OVERFLOW_I_get                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void                 scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_set_to_clear  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_get           ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_range_HOQ_CFC_OVERFLOW_I_poll          ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_HOQ_CFC_OVERFLOW_I_poll                ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------+
 | | Register 0x00104110 PAGE_IPT_ERR_INT_EN_REG |
 | +---------------------------------------------+
 | | bit  0 R/W  PAGE_IPT_ERR_E                  |
 | +---------------------------------------------+
 | 
 | void   scpb_reg_field_PAGE_IPT_ERR_E_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_PAGE_IPT_ERR_E_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00104114 BLOCK_IPT_ERR_INT_EN_REG |
 | +----------------------------------------------+
 | | bit  31:0 R/W  BLOCK_IPT_ERR_E               |
 | +----------------------------------------------+
 | 
 | void   scpb_reg_field_BLOCK_IPT_ERR_E_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_BLOCK_IPT_ERR_E_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_BLOCK_IPT_ERR_E_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_BLOCK_IPT_ERR_E_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x00104118 PREFETCH_FIFO_INT_EN_REG |
 | +----------------------------------------------+
 | | bit  19:16 R/W  HOQ_DLY_OVERFLOW_E           |
 | | bit  11:8  R/W  HOQ_CFC_UNDERFLOW_E          |
 | | bit  3:0   R/W  HOQ_CFC_OVERFLOW_E           |
 | +----------------------------------------------+
 | 
 | void   scpb_reg_field_HOQ_DLY_OVERFLOW_E_set        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_HOQ_DLY_OVERFLOW_E_get        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_set  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_HOQ_DLY_OVERFLOW_E_get  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   scpb_reg_field_HOQ_CFC_UNDERFLOW_E_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_HOQ_CFC_UNDERFLOW_E_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_HOQ_CFC_UNDERFLOW_E_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   scpb_reg_field_HOQ_CFC_OVERFLOW_E_set        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_HOQ_CFC_OVERFLOW_E_get        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_set  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_HOQ_CFC_OVERFLOW_E_get  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register (0x00104120 + (N) * 0x4) GPI |
 | |     N[0..3]                           |
 | +---------------------------------------+
 | | bit  31:0 R  GPI                      |
 | +---------------------------------------+
 | 
 | void   scpb_reg_lfield_GPI_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value[4] )
 | UINT32 scpb_reg_lfield_range_GPI_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register (0x00104140 + (N) * 0x4) GPO |
 | |     N[0..3]                           |
 | +---------------------------------------+
 | | bit  31:0 R/W  GPO                    |
 | +---------------------------------------+
 | 
 | void   scpb_reg_lfield_GPO_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value[4] )
 | void   scpb_reg_lfield_GPO_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value[4] )
 | void   scpb_reg_lfield_range_GPO_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_lfield_range_GPO_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x00104200 REMAP_REQ |
 | +-------------------------------+
 | | bit  28 R/W  REMAP_ABORT      |
 | | bit  24 R/W  REMAP_REQ        |
 | +-------------------------------+
 | 
 | void                 scpb_reg_field_REMAP_ABORT_set  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32               scpb_reg_field_REMAP_ABORT_get  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_REMAP_ABORT_poll ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 scpb_reg_field_REMAP_REQ_set    ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32               scpb_reg_field_REMAP_REQ_get    ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE scpb_reg_field_REMAP_REQ_poll   ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00104204 REMAP_OLD_ADDR  |
 | +-------------------------------------+
 | | bit  15:0 R/W  REMAP_OLD_BLOCK_ADDR |
 | +-------------------------------------+
 | 
 | void   scpb_reg_field_REMAP_OLD_BLOCK_ADDR_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_REMAP_OLD_BLOCK_ADDR_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_REMAP_OLD_BLOCK_ADDR_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00104208 REMAP_NEW_ADDR  |
 | +-------------------------------------+
 | | bit  15:0 R/W  REMAP_NEW_BLOCK_ADDR |
 | +-------------------------------------+
 | 
 | void   scpb_reg_field_REMAP_NEW_BLOCK_ADDR_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_REMAP_NEW_BLOCK_ADDR_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_REMAP_NEW_BLOCK_ADDR_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x0010420c REMAP_STAT |
 | +--------------------------------+
 | | bit  0 R  REMAP_SUCCESS        |
 | +--------------------------------+
 | 
 | UINT32 scpb_reg_field_REMAP_SUCCESS_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | +----------------------------------+
 | | Register 0x00104210 REMAP_MARGIN |
 | +----------------------------------+
 | | bit  7:0 R/W  REMAP_WPTR_MARGIN  |
 | +----------------------------------+
 | 
 | void   scpb_reg_field_REMAP_WPTR_MARGIN_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_REMAP_WPTR_MARGIN_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_REMAP_WPTR_MARGIN_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_REMAP_WPTR_MARGIN_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00104214 REMAP_OLD_BLK_CFG |
 | +---------------------------------------+
 | | bit  23:16 R/W  REMAP_WQID            |
 | | bit  8:0   R/W  REMAP_LBA             |
 | +---------------------------------------+
 | 
 | void   scpb_reg_field_REMAP_WQID_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_REMAP_WQID_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_REMAP_WQID_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_REMAP_WQID_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   scpb_reg_field_REMAP_LBA_set        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_REMAP_LBA_get        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_REMAP_LBA_set  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_REMAP_LBA_get  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00104218 REMAP_OLD_PAGE_CFG |
 | +----------------------------------------+
 | | bit  17:16 R/W  REMAP_QS_ID            |
 | | bit  9:8   R/W  REMAP_WP_ID            |
 | +----------------------------------------+
 | 
 | void   scpb_reg_field_REMAP_QS_ID_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_REMAP_QS_ID_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_REMAP_QS_ID_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_REMAP_QS_ID_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   scpb_reg_field_REMAP_WP_ID_set       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | UINT32 scpb_reg_field_REMAP_WP_ID_get       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void   scpb_reg_field_range_REMAP_WP_ID_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 scpb_reg_field_range_REMAP_WP_ID_get ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SCPB_REG_IO_H
#define _SCPB_REG_IO_H

#include "cpb_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "scpb_reg_regs.h"
#include "scpb_reg_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 scpb_reg_buffer_init               ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | void                 scpb_reg_buffer_flush              ( scpb_reg_buffer_t *b_ptr )
 | UINT32               scpb_reg_reg_read                  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 scpb_reg_reg_write                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 scpb_reg_field_set                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 scpb_reg_action_on_write_field_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 scpb_reg_burst_read                ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 scpb_reg_burst_write               ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE scpb_reg_poll                      ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 scpb_reg_reg_REMAP_OLD_ADDR_write                            ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_REMAP_OLD_ADDR_field_set                        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_REMAP_OLD_ADDR_read                             ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void                 scpb_reg_reg_REMAP_NEW_ADDR_write                            ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_REMAP_NEW_ADDR_field_set                        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_REMAP_NEW_ADDR_read                             ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void                 scpb_reg_reg_REMAP_MARGIN_write                              ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_REMAP_MARGIN_field_set                          ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_REMAP_MARGIN_read                               ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void                 scpb_reg_reg_REMAP_OLD_BLK_CFG_write                         ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_REMAP_OLD_BLK_CFG_field_set                     ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_REMAP_OLD_BLK_CFG_read                          ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void                 scpb_reg_reg_REMAP_OLD_PAGE_CFG_write                        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_REMAP_OLD_PAGE_CFG_field_set                    ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_REMAP_OLD_PAGE_CFG_read                         ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void                 scpb_reg_reg_FIFO_IPT_BYPASS_REG_write                       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_FIFO_IPT_BYPASS_REG_field_set                   ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_FIFO_IPT_BYPASS_REG_read                        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void                 scpb_reg_reg_FIFO_CFG_write                                  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_FIFO_CFG_field_set                              ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_FIFO_CFG_read                                   ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE scpb_reg_reg_FIFO_CFG_poll                                   ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 scpb_reg_reg_FIFO_CFG_RID_write                              ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_FIFO_CFG_RID_field_set                          ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_FIFO_CFG_RID_read                               ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void                 scpb_reg_reg_FIFO_CFG_WID_write                              ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_FIFO_CFG_WID_field_set                          ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_FIFO_CFG_WID_read                               ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void                 scpb_reg_reg_DBG_CTRL_write                                  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_DBG_CTRL_field_set                              ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_DBG_CTRL_read                                   ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void                 scpb_reg_reg_GPO_array_burst_write                           ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 scpb_reg_reg_GPO_array_field_set                             ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 scpb_reg_reg_GPO_array_burst_read                            ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               scpb_reg_reg_GPO_array_read                                  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 N )
 | 
 | void                 scpb_reg_reg_BLOCK_TABLE_REG_array_write                     ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | void                 scpb_reg_reg_BLOCK_TABLE_REG_array_field_set                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_BLOCK_TABLE_REG_array_read                      ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32  M )
 | 
 | void                 scpb_reg_reg_PAGE_TABLE_REG_array_write                      ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 scpb_reg_reg_PAGE_TABLE_REG_array_field_set                  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_PAGE_TABLE_REG_array_read                       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | void                 scpb_reg_reg_REMAP_REQ_write                                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_REMAP_REQ_field_set                             ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_REMAP_REQ_read                                  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE scpb_reg_reg_REMAP_REQ_poll                                  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 scpb_reg_reg_PUSH_PORT_P3_RST_array_write                    ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 scpb_reg_reg_PUSH_PORT_P3_RST_array_field_set                ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_PUSH_PORT_P3_RST_array_read                     ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | void                 scpb_reg_reg_PUSH_PORT_P2_RST_array_write                    ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 scpb_reg_reg_PUSH_PORT_P2_RST_array_field_set                ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_PUSH_PORT_P2_RST_array_read                     ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | void                 scpb_reg_reg_PUSH_PORT_P1_RST_array_write                    ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 scpb_reg_reg_PUSH_PORT_P1_RST_array_field_set                ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_PUSH_PORT_P1_RST_array_read                     ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | void                 scpb_reg_reg_POP_PORT_RST_array_write                        ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 scpb_reg_reg_POP_PORT_RST_array_field_set                    ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_POP_PORT_RST_array_read                         ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | void                 scpb_reg_reg_PAGE_IPT_ERR_INT_REG_write                      ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_PAGE_IPT_ERR_INT_REG_action_on_write_field_set  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_PAGE_IPT_ERR_INT_REG_read                       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE scpb_reg_reg_PAGE_IPT_ERR_INT_REG_poll                       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_write                     ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_action_on_write_field_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_read                      ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE scpb_reg_reg_BLOCK_IPT_ERR_INT_REG_poll                      ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 scpb_reg_reg_PREFETCH_FIFO_INT_REG_write                     ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_PREFETCH_FIFO_INT_REG_action_on_write_field_set ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_PREFETCH_FIFO_INT_REG_read                      ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE scpb_reg_reg_PREFETCH_FIFO_INT_REG_poll                      ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 scpb_reg_reg_FIFO_INT_REG_write                              ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_FIFO_INT_REG_action_on_write_field_set          ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_FIFO_INT_REG_read                               ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE scpb_reg_reg_FIFO_INT_REG_poll                               ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_write                   ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_field_set               ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_PAGE_IPT_ERR_INT_EN_REG_read                    ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void                 scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_write                  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_field_set              ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_BLOCK_IPT_ERR_INT_EN_REG_read                   ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void                 scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_write                  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_field_set              ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_PREFETCH_FIFO_INT_EN_REG_read                   ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void                 scpb_reg_reg_FIFO_INT_EN_REG_write                           ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 value )
 | void                 scpb_reg_reg_FIFO_INT_EN_REG_field_set                       ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               scpb_reg_reg_FIFO_INT_EN_REG_read                            ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | UINT32               scpb_reg_reg_REMAP_STAT_read                                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | UINT32               scpb_reg_reg_FIFO_CFG_STAT_read                              ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE scpb_reg_reg_FIFO_CFG_STAT_poll                              ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               scpb_reg_reg_DBG_RDATA_read                                  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr )
 | 
 | void                 scpb_reg_reg_GPI_array_burst_read                            ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               scpb_reg_reg_GPI_array_read                                  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32 N )
 | 
 | UINT32               scpb_reg_reg_PUSH_PORT_P0_RST_VAL_array_read                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               scpb_reg_reg_PUSH_PORT_P1_RST_VAL_array_read                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               scpb_reg_reg_PUSH_PORT_P2_RST_VAL_array_read                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               scpb_reg_reg_PUSH_PORT_P3_RST_VAL_array_read                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               scpb_reg_reg_RPTR_RST_POP_VAL_REG_array_read                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_read                   ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE scpb_reg_reg_FIFO_PTR_EMPTY_REG_array_poll                   ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               scpb_reg_reg_FIFO_UDR_STAT_REG_0_array_read                  ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               scpb_reg_reg_P0_FIFO_OVR_STAT_REG_array_read                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               scpb_reg_reg_P1_FIFO_OVR_STAT_REG_array_read                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               scpb_reg_reg_P2_FIFO_OVR_STAT_REG_array_read                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               scpb_reg_reg_P3_FIFO_OVR_STAT_REG_array_read                 ( scpb_reg_buffer_t *b_ptr, cpb_handle_t *h_ptr, UINT32  N )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SCPB_REG_IO_H */
