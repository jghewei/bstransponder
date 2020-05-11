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
 *     and register accessor functions for the etrans block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing etrans io functions is:
 | {
 |     etrans_buffer_t b_ptr[1];
 |     etrans_buffer_init( b_ptr, h_ptr );
 |     value = etrans_field_<FIELD>_get( b_ptr, h_ptr );
 |     etrans_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     etrans_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = etrans_field_<FIELD>_get( NULL, h_ptr );
 | etrans_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------------------------+
 | | Register (0x00000000 + (N) * 0x4) LINKID_MAPPING |
 | |     N[0..11]                                     |
 | +--------------------------------------------------+
 | | bit  6:0 R/W  RX_CHID                            |
 | +--------------------------------------------------+
 | 
 | void   etrans_field_RX_CHID_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 etrans_field_RX_CHID_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | void   etrans_field_range_RX_CHID_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_RX_CHID_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0000003c GLOBAL_SOFT_RST_CFG |
 | +-----------------------------------------+
 | | bit  2 R/W  SW_100G_RST                 |
 | | bit  1 R/W  SW_40G_RST                  |
 | | bit  0 R/W  SW_10G_RST                  |
 | +-----------------------------------------+
 | 
 | void   etrans_field_SW_100G_RST_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_SW_100G_RST_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_SW_40G_RST_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_SW_40G_RST_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_SW_10G_RST_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_SW_10G_RST_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000048 GLOBAL_LPBK_CFG   |
 | +---------------------------------------+
 | | bit  31   R/W  GBL_ETRANS_LB_MCAST_EN |
 | | bit  11:0 R/W  GBL_ETRANS_LB_EN       |
 | +---------------------------------------+
 | 
 | void   etrans_field_GBL_ETRANS_LB_MCAST_EN_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_GBL_ETRANS_LB_MCAST_EN_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_GBL_ETRANS_LB_EN_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_GBL_ETRANS_LB_EN_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_GBL_ETRANS_LB_EN_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_GBL_ETRANS_LB_EN_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000054 + (N) * 0x04) RX_10G_LINK_CFG_REG |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  3:2 R/W  RX_10G_LINK_DP_SEL                       |
 | | bit  1   R/W  RX_10G_LINK_EN                           |
 | | bit  0   R/W  RX_10G_LINK_RST                          |
 | +--------------------------------------------------------+
 | 
 | void   etrans_field_RX_10G_LINK_DP_SEL_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 etrans_field_RX_10G_LINK_DP_SEL_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | void   etrans_field_range_RX_10G_LINK_DP_SEL_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_RX_10G_LINK_DP_SEL_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_RX_10G_LINK_EN_set           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 etrans_field_RX_10G_LINK_EN_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | void   etrans_field_RX_10G_LINK_RST_set          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 etrans_field_RX_10G_LINK_RST_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000084 + (N) * 0x04) RX_40G_LINK_CFG_REG |
 | |     N[0..2]                                            |
 | +--------------------------------------------------------+
 | | bit  3:2 R/W  RX_40G_LINK_DP_SEL                       |
 | | bit  1   R/W  RX_40G_LINK_EN                           |
 | | bit  0   R/W  RX_40G_LINK_RST                          |
 | +--------------------------------------------------------+
 | 
 | void   etrans_field_RX_40G_LINK_DP_SEL_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 etrans_field_RX_40G_LINK_DP_SEL_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | void   etrans_field_range_RX_40G_LINK_DP_SEL_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_RX_40G_LINK_DP_SEL_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_RX_40G_LINK_EN_set           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 etrans_field_RX_40G_LINK_EN_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | void   etrans_field_RX_40G_LINK_RST_set          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 etrans_field_RX_40G_LINK_RST_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------+
 | | Register 0x00000090 RX_100G_LINK_CFG_REG |
 | +------------------------------------------+
 | | bit  3:2 R/W  RX_100G_LINK_DP_SEL        |
 | | bit  1   R/W  RX_100G_LINK_EN            |
 | | bit  0   R/W  RX_100G_LINK_RST           |
 | +------------------------------------------+
 | 
 | void   etrans_field_RX_100G_LINK_DP_SEL_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_RX_100G_LINK_DP_SEL_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_RX_100G_LINK_DP_SEL_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_RX_100G_LINK_DP_SEL_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_RX_100G_LINK_EN_set           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_RX_100G_LINK_EN_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_RX_100G_LINK_RST_set          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_RX_100G_LINK_RST_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | +--------------------------------------------+
 | | Register 0x00000094 RXFIFO_OVERFLOW_INT_EN |
 | +--------------------------------------------+
 | | bit  15    R/W  RX_100G_FIFO_OVF_E         |
 | | bit  14:12 R/W  RX_40G_FIFO_OVF_E          |
 | | bit  11:0  R/W  RX_10G_FIFO_OVF_E          |
 | +--------------------------------------------+
 | 
 | void   etrans_field_RX_100G_FIFO_OVF_E_set      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_RX_100G_FIFO_OVF_E_get      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_RX_40G_FIFO_OVF_E_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_RX_40G_FIFO_OVF_E_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_RX_40G_FIFO_OVF_E_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_RX_40G_FIFO_OVF_E_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_RX_10G_FIFO_OVF_E_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_RX_10G_FIFO_OVF_E_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_RX_10G_FIFO_OVF_E_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_RX_10G_FIFO_OVF_E_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000098 RXFIFO_UNDERRUN_INT_EN |
 | +--------------------------------------------+
 | | bit  15    R/W  RX_100G_FIFO_UDR_E         |
 | | bit  14:12 R/W  RX_40G_FIFO_UDR_E          |
 | | bit  11:0  R/W  RX_10G_FIFO_UDR_E          |
 | +--------------------------------------------+
 | 
 | void   etrans_field_RX_100G_FIFO_UDR_E_set      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_RX_100G_FIFO_UDR_E_get      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_RX_40G_FIFO_UDR_E_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_RX_40G_FIFO_UDR_E_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_RX_40G_FIFO_UDR_E_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_RX_40G_FIFO_UDR_E_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_RX_10G_FIFO_UDR_E_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_RX_10G_FIFO_UDR_E_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_RX_10G_FIFO_UDR_E_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_RX_10G_FIFO_UDR_E_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x000000a4 RXFIFO_OVERFLOW_INT_STAT |
 | +----------------------------------------------+
 | | bit  15    R/W  RX_100G_FIFO_OVF_I           |
 | | bit  14:12 R/W  RX_40G_FIFO_OVF_I            |
 | | bit  11:0  R/W  RX_10G_FIFO_OVF_I            |
 | +----------------------------------------------+
 | 
 | void                 etrans_field_RX_100G_FIFO_OVF_I_set_to_clear      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_RX_100G_FIFO_OVF_I_get               ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_RX_100G_FIFO_OVF_I_poll              ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_RX_40G_FIFO_OVF_I_set_to_clear       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_RX_40G_FIFO_OVF_I_get                ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void                 etrans_field_range_RX_40G_FIFO_OVF_I_set_to_clear ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etrans_field_range_RX_40G_FIFO_OVF_I_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etrans_field_range_RX_40G_FIFO_OVF_I_poll         ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etrans_field_RX_40G_FIFO_OVF_I_poll               ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_RX_10G_FIFO_OVF_I_set_to_clear       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_RX_10G_FIFO_OVF_I_get                ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void                 etrans_field_range_RX_10G_FIFO_OVF_I_set_to_clear ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etrans_field_range_RX_10G_FIFO_OVF_I_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etrans_field_range_RX_10G_FIFO_OVF_I_poll         ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etrans_field_RX_10G_FIFO_OVF_I_poll               ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------+
 | | Register 0x000000a8 RXFIFO_UNDERRUN_INT_STAT |
 | +----------------------------------------------+
 | | bit  15    R/W  RX_100G_FIFO_UDR_I           |
 | | bit  14:12 R/W  RX_40G_FIFO_UDR_I            |
 | | bit  11:0  R/W  RX_10G_FIFO_UDR_I            |
 | +----------------------------------------------+
 | 
 | void                 etrans_field_RX_100G_FIFO_UDR_I_set_to_clear      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_RX_100G_FIFO_UDR_I_get               ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_RX_100G_FIFO_UDR_I_poll              ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_RX_40G_FIFO_UDR_I_set_to_clear       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_RX_40G_FIFO_UDR_I_get                ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void                 etrans_field_range_RX_40G_FIFO_UDR_I_set_to_clear ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etrans_field_range_RX_40G_FIFO_UDR_I_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etrans_field_range_RX_40G_FIFO_UDR_I_poll         ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etrans_field_RX_40G_FIFO_UDR_I_poll               ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_RX_10G_FIFO_UDR_I_set_to_clear       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_RX_10G_FIFO_UDR_I_get                ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void                 etrans_field_range_RX_10G_FIFO_UDR_I_set_to_clear ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etrans_field_range_RX_10G_FIFO_UDR_I_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etrans_field_range_RX_10G_FIFO_UDR_I_poll         ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etrans_field_RX_10G_FIFO_UDR_I_poll               ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------+
 | | Register 0x000000b4 RXFIFO_OVERFLOW_INT_VAL |
 | +---------------------------------------------+
 | | bit  15    R  RX_100G_FIFO_OVF_V            |
 | | bit  14:12 R  RX_40G_FIFO_OVF_V             |
 | | bit  11:0  R  RX_10G_FIFO_OVF_V             |
 | +---------------------------------------------+
 | 
 | UINT32               etrans_field_RX_100G_FIFO_OVF_V_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_RX_100G_FIFO_OVF_V_poll      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_RX_40G_FIFO_OVF_V_get        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | UINT32               etrans_field_range_RX_40G_FIFO_OVF_V_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etrans_field_range_RX_40G_FIFO_OVF_V_poll ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etrans_field_RX_40G_FIFO_OVF_V_poll       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_RX_10G_FIFO_OVF_V_get        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | UINT32               etrans_field_range_RX_10G_FIFO_OVF_V_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etrans_field_range_RX_10G_FIFO_OVF_V_poll ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etrans_field_RX_10G_FIFO_OVF_V_poll       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------+
 | | Register 0x000000b8 RXFIFO_UNDERRUN_INT_VAL |
 | +---------------------------------------------+
 | | bit  15    R  RX_100G_FIFO_UDR_V            |
 | | bit  14:12 R  RX_40G_FIFO_UDR_V             |
 | | bit  11:0  R  RX_10G_FIFO_UDR_V             |
 | +---------------------------------------------+
 | 
 | UINT32               etrans_field_RX_100G_FIFO_UDR_V_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_RX_100G_FIFO_UDR_V_poll      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_RX_40G_FIFO_UDR_V_get        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | UINT32               etrans_field_range_RX_40G_FIFO_UDR_V_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etrans_field_range_RX_40G_FIFO_UDR_V_poll ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etrans_field_RX_40G_FIFO_UDR_V_poll       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_RX_10G_FIFO_UDR_V_get        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | UINT32               etrans_field_range_RX_10G_FIFO_UDR_V_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etrans_field_range_RX_10G_FIFO_UDR_V_poll ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etrans_field_RX_10G_FIFO_UDR_V_poll       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------------+
 | | Register (0x000000c4 + (N) * 0x04) TX_10G_LINK_CFG |
 | |     N[0..11]                                       |
 | +----------------------------------------------------+
 | | bit  3:2 R/W  TX_10G_LINK_DP_SEL                   |
 | | bit  1   R/W  TX_10G_LINK_EN                       |
 | | bit  0   R/W  TX_10G_LINK_RST                      |
 | +----------------------------------------------------+
 | 
 | void   etrans_field_TX_10G_LINK_DP_SEL_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 etrans_field_TX_10G_LINK_DP_SEL_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | void   etrans_field_range_TX_10G_LINK_DP_SEL_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_10G_LINK_DP_SEL_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_10G_LINK_EN_set           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 etrans_field_TX_10G_LINK_EN_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | void   etrans_field_TX_10G_LINK_RST_set          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 etrans_field_TX_10G_LINK_RST_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------------------+
 | | Register (0x000000f4 + (N) * 0x04) TX_40G_LINK_CFG |
 | |     N[0..2]                                        |
 | +----------------------------------------------------+
 | | bit  3:2 R/W  TX_40G_LINK_DP_SEL                   |
 | | bit  1   R/W  TX_40G_LINK_EN                       |
 | | bit  0   R/W  TX_40G_LINK_RST                      |
 | +----------------------------------------------------+
 | 
 | void   etrans_field_TX_40G_LINK_DP_SEL_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 etrans_field_TX_40G_LINK_DP_SEL_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | void   etrans_field_range_TX_40G_LINK_DP_SEL_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_40G_LINK_DP_SEL_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_40G_LINK_EN_set           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 etrans_field_TX_40G_LINK_EN_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | void   etrans_field_TX_40G_LINK_RST_set          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 etrans_field_TX_40G_LINK_RST_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------+
 | | Register 0x00000100 TX_100G_LINK_CFG |
 | +--------------------------------------+
 | | bit  3:2 R/W  TX_100G_LINK_DP_SEL    |
 | | bit  1   R/W  TX_100G_LINK_EN        |
 | | bit  0   R/W  TX_100G_LINK_RST       |
 | +--------------------------------------+
 | 
 | void   etrans_field_TX_100G_LINK_DP_SEL_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_100G_LINK_DP_SEL_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_100G_LINK_DP_SEL_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_100G_LINK_DP_SEL_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_100G_LINK_EN_set           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_100G_LINK_EN_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_TX_100G_LINK_RST_set          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_100G_LINK_RST_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00000104 TX_FIFO_OVF_INT_EN   |
 | +------------------------------------------+
 | | bit  15    R/W  TX_100G_FIFO_OVF_E       |
 | | bit  14:12 R/W  TX_40G_FIFO_OVF_E        |
 | | bit  11    R/W  TX_LINK11_10G_FIFO_OVF_E |
 | | bit  10    R/W  TX_LINK10_10G_FIFO_OVF_E |
 | | bit  9     R/W  TX_LINK9_10G_FIFO_OVF_E  |
 | | bit  8     R/W  TX_LINK8_10G_FIFO_OVF_E  |
 | | bit  7     R/W  TX_LINK7_10G_FIFO_OVF_E  |
 | | bit  6     R/W  TX_LINK6_10G_FIFO_OVF_E  |
 | | bit  5     R/W  TX_LINK5_10G_FIFO_OVF_E  |
 | | bit  4     R/W  TX_LINK4_10G_FIFO_OVF_E  |
 | | bit  3     R/W  TX_LINK3_10G_FIFO_OVF_E  |
 | | bit  2     R/W  TX_LINK2_10G_FIFO_OVF_E  |
 | | bit  1     R/W  TX_LINK1_10G_FIFO_OVF_E  |
 | | bit  0     R/W  TX_LINK0_10G_FIFO_OVF_E  |
 | +------------------------------------------+
 | 
 | void   etrans_field_TX_100G_FIFO_OVF_E_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_100G_FIFO_OVF_E_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_TX_40G_FIFO_OVF_E_set        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_40G_FIFO_OVF_E_get        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_40G_FIFO_OVF_E_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_40G_FIFO_OVF_E_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_LINK11_10G_FIFO_OVF_E_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK11_10G_FIFO_OVF_E_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_TX_LINK10_10G_FIFO_OVF_E_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK10_10G_FIFO_OVF_E_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_TX_LINK9_10G_FIFO_OVF_E_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK9_10G_FIFO_OVF_E_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_TX_LINK8_10G_FIFO_OVF_E_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK8_10G_FIFO_OVF_E_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_TX_LINK7_10G_FIFO_OVF_E_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK7_10G_FIFO_OVF_E_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_TX_LINK6_10G_FIFO_OVF_E_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK6_10G_FIFO_OVF_E_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_TX_LINK5_10G_FIFO_OVF_E_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK5_10G_FIFO_OVF_E_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_TX_LINK4_10G_FIFO_OVF_E_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK4_10G_FIFO_OVF_E_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_TX_LINK3_10G_FIFO_OVF_E_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK3_10G_FIFO_OVF_E_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_TX_LINK2_10G_FIFO_OVF_E_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK2_10G_FIFO_OVF_E_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_TX_LINK1_10G_FIFO_OVF_E_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK1_10G_FIFO_OVF_E_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_TX_LINK0_10G_FIFO_OVF_E_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK0_10G_FIFO_OVF_E_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00000108 TX_FIFO_UNDERRUN_INT_EN |
 | +---------------------------------------------+
 | | bit  15    R/W  TX_100G_FIFO_UDR_E          |
 | | bit  14:12 R/W  TX_40G_FIFO_UDR_E           |
 | | bit  11:0  R/W  TX_10G_FIFO_UDR_E           |
 | +---------------------------------------------+
 | 
 | void   etrans_field_TX_100G_FIFO_UDR_E_set      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_100G_FIFO_UDR_E_get      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_TX_40G_FIFO_UDR_E_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_40G_FIFO_UDR_E_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_40G_FIFO_UDR_E_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_40G_FIFO_UDR_E_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_10G_FIFO_UDR_E_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_10G_FIFO_UDR_E_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_10G_FIFO_UDR_E_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_10G_FIFO_UDR_E_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000114 TX_FIFO_OVF_INT_STAT |
 | +------------------------------------------+
 | | bit  15    R/W  TX_100G_FIFO_OVF_I       |
 | | bit  14:12 R/W  TX_40G_FIFO_OVF_I        |
 | | bit  11    R/W  TX_LINK11_10G_FIFO_OVF_I |
 | | bit  10    R/W  TX_LINK10_10G_FIFO_OVF_I |
 | | bit  9     R/W  TX_LINK9_10G_FIFO_OVF_I  |
 | | bit  8     R/W  TX_LINK8_10G_FIFO_OVF_I  |
 | | bit  7     R/W  TX_LINK7_10G_FIFO_OVF_I  |
 | | bit  6     R/W  TX_LINK6_10G_FIFO_OVF_I  |
 | | bit  5     R/W  TX_LINK5_10G_FIFO_OVF_I  |
 | | bit  4     R/W  TX_LINK4_10G_FIFO_OVF_I  |
 | | bit  3     R/W  TX_LINK3_10G_FIFO_OVF_I  |
 | | bit  2     R/W  TX_LINK2_10G_FIFO_OVF_I  |
 | | bit  1     R/W  TX_LINK1_10G_FIFO_OVF_I  |
 | | bit  0     R/W  TX_LINK0_10G_FIFO_OVF_I  |
 | +------------------------------------------+
 | 
 | void                 etrans_field_TX_100G_FIFO_OVF_I_set_to_clear       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_100G_FIFO_OVF_I_get                ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_100G_FIFO_OVF_I_poll               ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_40G_FIFO_OVF_I_set_to_clear        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_40G_FIFO_OVF_I_get                 ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void                 etrans_field_range_TX_40G_FIFO_OVF_I_set_to_clear  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etrans_field_range_TX_40G_FIFO_OVF_I_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etrans_field_range_TX_40G_FIFO_OVF_I_poll          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_40G_FIFO_OVF_I_poll                ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_LINK11_10G_FIFO_OVF_I_set_to_clear ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_LINK11_10G_FIFO_OVF_I_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK11_10G_FIFO_OVF_I_poll         ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_LINK10_10G_FIFO_OVF_I_set_to_clear ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_LINK10_10G_FIFO_OVF_I_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK10_10G_FIFO_OVF_I_poll         ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_LINK9_10G_FIFO_OVF_I_set_to_clear  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_LINK9_10G_FIFO_OVF_I_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK9_10G_FIFO_OVF_I_poll          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_LINK8_10G_FIFO_OVF_I_set_to_clear  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_LINK8_10G_FIFO_OVF_I_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK8_10G_FIFO_OVF_I_poll          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_LINK7_10G_FIFO_OVF_I_set_to_clear  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_LINK7_10G_FIFO_OVF_I_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK7_10G_FIFO_OVF_I_poll          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_LINK6_10G_FIFO_OVF_I_set_to_clear  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_LINK6_10G_FIFO_OVF_I_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK6_10G_FIFO_OVF_I_poll          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_LINK5_10G_FIFO_OVF_I_set_to_clear  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_LINK5_10G_FIFO_OVF_I_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK5_10G_FIFO_OVF_I_poll          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_LINK4_10G_FIFO_OVF_I_set_to_clear  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_LINK4_10G_FIFO_OVF_I_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK4_10G_FIFO_OVF_I_poll          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_LINK3_10G_FIFO_OVF_I_set_to_clear  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_LINK3_10G_FIFO_OVF_I_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK3_10G_FIFO_OVF_I_poll          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_LINK2_10G_FIFO_OVF_I_set_to_clear  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_LINK2_10G_FIFO_OVF_I_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK2_10G_FIFO_OVF_I_poll          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_LINK1_10G_FIFO_OVF_I_set_to_clear  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_LINK1_10G_FIFO_OVF_I_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK1_10G_FIFO_OVF_I_poll          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_LINK0_10G_FIFO_OVF_I_set_to_clear  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_LINK0_10G_FIFO_OVF_I_get           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK0_10G_FIFO_OVF_I_poll          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000118 TX_FIFO_UNDERRUN_INT_STAT |
 | +-----------------------------------------------+
 | | bit  15    R/W  TX_100G_FIFO_UDR_I            |
 | | bit  14:12 R/W  TX_40G_FIFO_UDR_I             |
 | | bit  11:0  R/W  TX_10G_FIFO_UDR_I             |
 | +-----------------------------------------------+
 | 
 | void                 etrans_field_TX_100G_FIFO_UDR_I_set_to_clear      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_100G_FIFO_UDR_I_get               ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_100G_FIFO_UDR_I_poll              ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_40G_FIFO_UDR_I_set_to_clear       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_40G_FIFO_UDR_I_get                ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void                 etrans_field_range_TX_40G_FIFO_UDR_I_set_to_clear ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etrans_field_range_TX_40G_FIFO_UDR_I_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etrans_field_range_TX_40G_FIFO_UDR_I_poll         ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_40G_FIFO_UDR_I_poll               ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 etrans_field_TX_10G_FIFO_UDR_I_set_to_clear       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32               etrans_field_TX_10G_FIFO_UDR_I_get                ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void                 etrans_field_range_TX_10G_FIFO_UDR_I_set_to_clear ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               etrans_field_range_TX_10G_FIFO_UDR_I_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etrans_field_range_TX_10G_FIFO_UDR_I_poll         ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_10G_FIFO_UDR_I_poll               ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x00000124 TX_FIFO_OVF_INT_VAL |
 | +-----------------------------------------+
 | | bit  15    R  TX_100G_FIFO_OVF_V        |
 | | bit  14:12 R  TX_40G_FIFO_OVF_V         |
 | | bit  11    R  TX_LINK11_10G_FIFO_OVF_V  |
 | | bit  10    R  TX_LINK10_10G_FIFO_OVF_V  |
 | | bit  9     R  TX_LINK9_10G_FIFO_OVF_V   |
 | | bit  8     R  TX_LINK8_10G_FIFO_OVF_V   |
 | | bit  7     R  TX_LINK7_10G_FIFO_OVF_V   |
 | | bit  6     R  TX_LINK6_10G_FIFO_OVF_V   |
 | | bit  5     R  TX_LINK5_10G_FIFO_OVF_V   |
 | | bit  4     R  TX_LINK4_10G_FIFO_OVF_V   |
 | | bit  3     R  TX_LINK3_10G_FIFO_OVF_V   |
 | | bit  2     R  TX_LINK2_10G_FIFO_OVF_V   |
 | | bit  1     R  TX_LINK1_10G_FIFO_OVF_V   |
 | | bit  0     R  TX_LINK0_10G_FIFO_OVF_V   |
 | +-----------------------------------------+
 | 
 | UINT32               etrans_field_TX_100G_FIFO_OVF_V_get        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_100G_FIFO_OVF_V_poll       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_40G_FIFO_OVF_V_get         ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | UINT32               etrans_field_range_TX_40G_FIFO_OVF_V_get   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etrans_field_range_TX_40G_FIFO_OVF_V_poll  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_40G_FIFO_OVF_V_poll        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_LINK11_10G_FIFO_OVF_V_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK11_10G_FIFO_OVF_V_poll ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_LINK10_10G_FIFO_OVF_V_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK10_10G_FIFO_OVF_V_poll ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_LINK9_10G_FIFO_OVF_V_get   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK9_10G_FIFO_OVF_V_poll  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_LINK8_10G_FIFO_OVF_V_get   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK8_10G_FIFO_OVF_V_poll  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_LINK7_10G_FIFO_OVF_V_get   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK7_10G_FIFO_OVF_V_poll  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_LINK6_10G_FIFO_OVF_V_get   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK6_10G_FIFO_OVF_V_poll  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_LINK5_10G_FIFO_OVF_V_get   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK5_10G_FIFO_OVF_V_poll  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_LINK4_10G_FIFO_OVF_V_get   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK4_10G_FIFO_OVF_V_poll  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_LINK3_10G_FIFO_OVF_V_get   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK3_10G_FIFO_OVF_V_poll  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_LINK2_10G_FIFO_OVF_V_get   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK2_10G_FIFO_OVF_V_poll  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_LINK1_10G_FIFO_OVF_V_get   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK1_10G_FIFO_OVF_V_poll  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_LINK0_10G_FIFO_OVF_V_get   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_LINK0_10G_FIFO_OVF_V_poll  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------+
 | | Register 0x00000128 TX_FIFO_UNDERRUN_INT_VAL |
 | +----------------------------------------------+
 | | bit  15    R  TX_100G_FIFO_UDR_V             |
 | | bit  14:12 R  TX_40G_FIFO_UDR_V              |
 | | bit  11:0  R  TX_10G_FIFO_UDR_V              |
 | +----------------------------------------------+
 | 
 | UINT32               etrans_field_TX_100G_FIFO_UDR_V_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_100G_FIFO_UDR_V_poll      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_40G_FIFO_UDR_V_get        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | UINT32               etrans_field_range_TX_40G_FIFO_UDR_V_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etrans_field_range_TX_40G_FIFO_UDR_V_poll ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_40G_FIFO_UDR_V_poll       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               etrans_field_TX_10G_FIFO_UDR_V_get        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | UINT32               etrans_field_range_TX_10G_FIFO_UDR_V_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE etrans_field_range_TX_10G_FIFO_UDR_V_poll ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE etrans_field_TX_10G_FIFO_UDR_V_poll       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------+
 | | Register 0x00000134 TX_CHID_LOOKUP_CFG_REG0 |
 | +---------------------------------------------+
 | | bit  30:24 R/W  TX_CHID_LINK3               |
 | | bit  22:16 R/W  TX_CHID_LINK2               |
 | | bit  14:8  R/W  TX_CHID_LINK1               |
 | | bit  6:0   R/W  TX_CHID_LINK0               |
 | +---------------------------------------------+
 | 
 | void   etrans_field_TX_CHID_LINK3_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_CHID_LINK3_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_CHID_LINK3_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_CHID_LINK3_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_CHID_LINK2_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_CHID_LINK2_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_CHID_LINK2_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_CHID_LINK2_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_CHID_LINK1_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_CHID_LINK1_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_CHID_LINK1_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_CHID_LINK1_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_CHID_LINK0_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_CHID_LINK0_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_CHID_LINK0_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_CHID_LINK0_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000138 TX_CHID_LOOKUP_CFG_REG1 |
 | +---------------------------------------------+
 | | bit  30:24 R/W  TX_CHID_LINK7               |
 | | bit  22:16 R/W  TX_CHID_LINK6               |
 | | bit  14:8  R/W  TX_CHID_LINK5               |
 | | bit  6:0   R/W  TX_CHID_LINK4               |
 | +---------------------------------------------+
 | 
 | void   etrans_field_TX_CHID_LINK7_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_CHID_LINK7_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_CHID_LINK7_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_CHID_LINK7_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_CHID_LINK6_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_CHID_LINK6_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_CHID_LINK6_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_CHID_LINK6_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_CHID_LINK5_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_CHID_LINK5_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_CHID_LINK5_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_CHID_LINK5_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_CHID_LINK4_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_CHID_LINK4_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_CHID_LINK4_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_CHID_LINK4_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x0000013c TX_CHID_LOOKUP_CFG_REG2 |
 | +---------------------------------------------+
 | | bit  30:24 R/W  TX_CHID_LINK11              |
 | | bit  22:16 R/W  TX_CHID_LINK10              |
 | | bit  14:8  R/W  TX_CHID_LINK9               |
 | | bit  6:0   R/W  TX_CHID_LINK8               |
 | +---------------------------------------------+
 | 
 | void   etrans_field_TX_CHID_LINK11_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_CHID_LINK11_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_CHID_LINK11_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_CHID_LINK11_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_CHID_LINK10_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_CHID_LINK10_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_CHID_LINK10_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_CHID_LINK10_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_CHID_LINK9_set        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_CHID_LINK9_get        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_CHID_LINK9_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_CHID_LINK9_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_CHID_LINK8_set        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_CHID_LINK8_get        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_CHID_LINK8_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_CHID_LINK8_get  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000140 TX_OP_SCH_CAL_CFG0 |
 | +----------------------------------------+
 | | bit  31:28 R/W  TX_SLOT_7              |
 | | bit  27:24 R/W  TX_SLOT_6              |
 | | bit  23:20 R/W  TX_SLOT_5              |
 | | bit  19:16 R/W  TX_SLOT_4              |
 | | bit  15:12 R/W  TX_SLOT_3              |
 | | bit  11:8  R/W  TX_SLOT_2              |
 | | bit  7:4   R/W  TX_SLOT_1              |
 | | bit  3:0   R/W  TX_SLOT_0              |
 | +----------------------------------------+
 | 
 | void   etrans_field_TX_SLOT_7_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_SLOT_7_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_SLOT_7_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_SLOT_7_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_SLOT_6_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_SLOT_6_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_SLOT_6_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_SLOT_6_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_SLOT_5_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_SLOT_5_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_SLOT_5_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_SLOT_5_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_SLOT_4_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_SLOT_4_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_SLOT_4_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_SLOT_4_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_SLOT_3_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_SLOT_3_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_SLOT_3_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_SLOT_3_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_SLOT_2_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_SLOT_2_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_SLOT_2_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_SLOT_2_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_SLOT_1_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_SLOT_1_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_SLOT_1_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_SLOT_1_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_SLOT_0_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_SLOT_0_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_SLOT_0_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_SLOT_0_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000144 TX_OP_SCH_CAL_CFG1 |
 | +----------------------------------------+
 | | bit  31    R/W  TX_LINK_SCH_UPDATE     |
 | | bit  27:24 R/W  TX_LINK_SCH_OFFSET     |
 | | bit  23:20 R/W  TX_LINK_SCH_LAST_SLOT  |
 | | bit  15:12 R/W  TX_SLOT_11             |
 | | bit  11:8  R/W  TX_SLOT_10             |
 | | bit  7:4   R/W  TX_SLOT_9              |
 | | bit  3:0   R/W  TX_SLOT_8              |
 | +----------------------------------------+
 | 
 | void   etrans_field_TX_LINK_SCH_OFFSET_set          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK_SCH_OFFSET_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_LINK_SCH_OFFSET_set    ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_LINK_SCH_OFFSET_get    ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_LINK_SCH_LAST_SLOT_set       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK_SCH_LAST_SLOT_get       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_LINK_SCH_LAST_SLOT_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_LINK_SCH_LAST_SLOT_get ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_SLOT_11_set                  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_SLOT_11_get                  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_SLOT_11_set            ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_SLOT_11_get            ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_SLOT_10_set                  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_SLOT_10_get                  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_SLOT_10_set            ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_SLOT_10_get            ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_SLOT_9_set                   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_SLOT_9_get                   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_SLOT_9_set             ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_SLOT_9_get             ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_SLOT_8_set                   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_SLOT_8_get                   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void   etrans_field_range_TX_SLOT_8_set             ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 etrans_field_range_TX_SLOT_8_get             ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   etrans_field_TX_LINK_SCH_UPDATE_set          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | UINT32 etrans_field_TX_LINK_SCH_UPDATE_get          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ETRANS_IO_H
#define _ETRANS_IO_H

#include "etrans_loc.h"
#include "etrans_regs.h"
#include "etrans_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 etrans_buffer_init               ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | void                 etrans_buffer_flush              ( etrans_buffer_t *b_ptr )
 | UINT32               etrans_reg_read                  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 etrans_reg_write                 ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 etrans_field_set                 ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 etrans_action_on_write_field_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 etrans_burst_read                ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 etrans_burst_write               ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE etrans_poll                      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 etrans_reg_GLOBAL_SOFT_RST_CFG_write                           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_GLOBAL_SOFT_RST_CFG_field_set                       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_GLOBAL_SOFT_RST_CFG_read                            ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | void                 etrans_reg_GLOBAL_LPBK_CFG_write                               ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_GLOBAL_LPBK_CFG_field_set                           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_GLOBAL_LPBK_CFG_read                                ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | void                 etrans_reg_RX_100G_LINK_CFG_REG_write                          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_RX_100G_LINK_CFG_REG_field_set                      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_RX_100G_LINK_CFG_REG_read                           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | void                 etrans_reg_TX_100G_LINK_CFG_write                              ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_TX_100G_LINK_CFG_field_set                          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_TX_100G_LINK_CFG_read                               ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | void                 etrans_reg_TX_CHID_LOOKUP_CFG_REG0_write                       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_TX_CHID_LOOKUP_CFG_REG0_field_set                   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_TX_CHID_LOOKUP_CFG_REG0_read                        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | void                 etrans_reg_TX_CHID_LOOKUP_CFG_REG1_write                       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_TX_CHID_LOOKUP_CFG_REG1_field_set                   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_TX_CHID_LOOKUP_CFG_REG1_read                        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | void                 etrans_reg_TX_CHID_LOOKUP_CFG_REG2_write                       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_TX_CHID_LOOKUP_CFG_REG2_field_set                   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_TX_CHID_LOOKUP_CFG_REG2_read                        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | void                 etrans_reg_TX_OP_SCH_CAL_CFG0_write                            ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_TX_OP_SCH_CAL_CFG0_field_set                        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_TX_OP_SCH_CAL_CFG0_read                             ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | void                 etrans_reg_TX_OP_SCH_CAL_CFG1_write                            ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_TX_OP_SCH_CAL_CFG1_field_set                        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_TX_OP_SCH_CAL_CFG1_read                             ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | void                 etrans_reg_LINKID_MAPPING_array_write                          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 etrans_reg_LINKID_MAPPING_array_field_set                      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_LINKID_MAPPING_array_read                           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | 
 | void                 etrans_reg_RX_10G_LINK_CFG_REG_array_write                     ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 etrans_reg_RX_10G_LINK_CFG_REG_array_field_set                 ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_RX_10G_LINK_CFG_REG_array_read                      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | 
 | void                 etrans_reg_TX_10G_LINK_CFG_array_write                         ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 etrans_reg_TX_10G_LINK_CFG_array_field_set                     ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_TX_10G_LINK_CFG_array_read                          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | 
 | void                 etrans_reg_RX_40G_LINK_CFG_REG_array_write                     ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 etrans_reg_RX_40G_LINK_CFG_REG_array_field_set                 ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_RX_40G_LINK_CFG_REG_array_read                      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | 
 | void                 etrans_reg_TX_40G_LINK_CFG_array_write                         ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 etrans_reg_TX_40G_LINK_CFG_array_field_set                     ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_TX_40G_LINK_CFG_array_read                          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32  N )
 | 
 | void                 etrans_reg_RXFIFO_OVERFLOW_INT_EN_write                        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_RXFIFO_OVERFLOW_INT_EN_field_set                    ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_RXFIFO_OVERFLOW_INT_EN_read                         ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | void                 etrans_reg_RXFIFO_UNDERRUN_INT_EN_write                        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_RXFIFO_UNDERRUN_INT_EN_field_set                    ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_RXFIFO_UNDERRUN_INT_EN_read                         ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | void                 etrans_reg_TX_FIFO_OVF_INT_EN_write                            ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_TX_FIFO_OVF_INT_EN_field_set                        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_TX_FIFO_OVF_INT_EN_read                             ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | void                 etrans_reg_TX_FIFO_UNDERRUN_INT_EN_write                       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_TX_FIFO_UNDERRUN_INT_EN_field_set                   ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_TX_FIFO_UNDERRUN_INT_EN_read                        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | 
 | void                 etrans_reg_RXFIFO_OVERFLOW_INT_STAT_write                      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_RXFIFO_OVERFLOW_INT_STAT_action_on_write_field_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_RXFIFO_OVERFLOW_INT_STAT_read                       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_reg_RXFIFO_OVERFLOW_INT_STAT_poll                       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 etrans_reg_RXFIFO_UNDERRUN_INT_STAT_write                      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_RXFIFO_UNDERRUN_INT_STAT_action_on_write_field_set  ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_RXFIFO_UNDERRUN_INT_STAT_read                       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_reg_RXFIFO_UNDERRUN_INT_STAT_poll                       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 etrans_reg_TX_FIFO_OVF_INT_STAT_write                          ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_TX_FIFO_OVF_INT_STAT_action_on_write_field_set      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_TX_FIFO_OVF_INT_STAT_read                           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_reg_TX_FIFO_OVF_INT_STAT_poll                           ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_write                     ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 value )
 | void                 etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_action_on_write_field_set ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_read                      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_reg_TX_FIFO_UNDERRUN_INT_STAT_poll                      ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               etrans_reg_RXFIFO_OVERFLOW_INT_VAL_read                        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_reg_RXFIFO_OVERFLOW_INT_VAL_poll                        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               etrans_reg_RXFIFO_UNDERRUN_INT_VAL_read                        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_reg_RXFIFO_UNDERRUN_INT_VAL_poll                        ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               etrans_reg_TX_FIFO_OVF_INT_VAL_read                            ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_reg_TX_FIFO_OVF_INT_VAL_poll                            ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_read                       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE etrans_reg_TX_FIFO_UNDERRUN_INT_VAL_poll                       ( etrans_buffer_t *b_ptr, etrans_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ETRANS_IO_H */
