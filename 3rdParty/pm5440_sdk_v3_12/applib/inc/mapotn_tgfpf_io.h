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
 *     and register accessor functions for the mapotn_tgfpf block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing mapotn_tgfpf io functions is:
 | {
 |     mapotn_tgfpf_buffer_t b_ptr[1];
 |     mapotn_tgfpf_buffer_init( b_ptr, h_ptr );
 |     value = mapotn_tgfpf_field_<FIELD>_get( b_ptr, h_ptr );
 |     mapotn_tgfpf_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     mapotn_tgfpf_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = mapotn_tgfpf_field_<FIELD>_get( NULL, h_ptr );
 | mapotn_tgfpf_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------+
 | | Register 0x000a0000 COMMON_CFG |
 | +--------------------------------+
 | | bit  16   R/W  ORD_SET_EN      |
 | | bit  15:8 R/W  ORD_SET_UPI     |
 | | bit  0    R/W  GFPF_MAP_EN     |
 | +--------------------------------+
 | 
 | void   mapotn_tgfpf_field_ORD_SET_EN_set        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tgfpf_field_ORD_SET_EN_get        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_tgfpf_field_ORD_SET_UPI_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tgfpf_field_ORD_SET_UPI_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_tgfpf_field_range_ORD_SET_UPI_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_ORD_SET_UPI_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_tgfpf_field_GFPF_MAP_EN_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tgfpf_field_GFPF_MAP_EN_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x000a0004 PMON_CTRL |
 | +-------------------------------+
 | | bit  0 R/W  PMON_UPDATE       |
 | +-------------------------------+
 | 
 | void   mapotn_tgfpf_field_PMON_UPDATE_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PMON_UPDATE_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x000a0008 PMON_STATUS |
 | +---------------------------------+
 | | bit  0 R  PMON_TIP              |
 | +---------------------------------+
 | 
 | UINT32               mapotn_tgfpf_field_PMON_TIP_get  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_tgfpf_field_PMON_TIP_poll ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------------+
 | | Register (0x000a0010 + (N) * 0x4) PAUSE_FRM_TIME |
 | |     N[0..3]                                      |
 | +--------------------------------------------------+
 | | bit  15:0 R/W  PAUSE_TIME                        |
 | +--------------------------------------------------+
 | 
 | void   mapotn_tgfpf_field_PAUSE_TIME_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PAUSE_TIME_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_PAUSE_TIME_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_PAUSE_TIME_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x000a0080 + (N) * 0x4) TX_CHAN_RST |
 | |     N[0..95]                                  |
 | +-----------------------------------------------+
 | | bit  0 R/W  TX_CHAN_RESET                     |
 | +-----------------------------------------------+
 | 
 | void   mapotn_tgfpf_field_TX_CHAN_RESET_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_TX_CHAN_RESET_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------------+
 | | Register (0x000a0200 + (N) * 0x4) PROTOCOL_ERR_INT_EN |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  PROTOCOL_ERR_INT_E                     |
 | +-------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_PROTOCOL_ERR_INT_E_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_PROTOCOL_ERR_INT_E_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_PROTOCOL_ERR_INT_E_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_PROTOCOL_ERR_INT_E_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x000a0210 + (N) * 0x4) FRAME_LENGTH_ERR_INT_E |
 | |     N[0..2]                                              |
 | +----------------------------------------------------------+
 | | bit  31:0 R/W  LEN_ERR_INT_E                             |
 | +----------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_LEN_ERR_INT_E_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_LEN_ERR_INT_E_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_LEN_ERR_INT_E_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_LEN_ERR_INT_E_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000a0220 + (N) * 0x4) PAUSE_FRAME_TX_INT_EN |
 | |     N[0..2]                                             |
 | +---------------------------------------------------------+
 | | bit  31:0 R/W  PAUSE_FRAME_TX_INT_E                     |
 | +---------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_E_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_E_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_E_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_E_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x000a0230 + (N) * 0x4) MGMT_FRAME_TX_INT_EN |
 | |     N[0..2]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R/W  MGMT_FRAME_TX_INT_E                     |
 | +--------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_E_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_E_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_E_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_E_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x000a0240 + (N) * 0x4) OVERSIZE_FRM_RX_INT_EN |
 | |     N[0..2]                                              |
 | +----------------------------------------------------------+
 | | bit  31:0 R/W  OVERSIZE_FRM_RX_INT_E                     |
 | +----------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_E_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_E_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_E_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_E_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x000a0250 + (N) * 0x4) UNDERSIZE_FRM_RX_INT_EN |
 | |     N[0..2]                                               |
 | +-----------------------------------------------------------+
 | | bit  31:0 R/W  UNDERSIZE_FRM_RX_INT_E                     |
 | +-----------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_E_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_E_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_E_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_E_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register (0x000a0260 + (N) * 0x4) ORD_SET_FRM_LEN_ERR_INT_E |
 | |     N[0..2]                                                 |
 | +-------------------------------------------------------------+
 | | bit  31:0 R/W  ORD_SET_LEN_ERR_INT_E                        |
 | +-------------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_E_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_E_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_E_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_E_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x000a0290 + (N) * 0x4) FRAME_LENGTH_ERR_INT_EVENT |
 | |     N[0..2]                                                  |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  LEN_ERR_INT_I                                 |
 | +--------------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_LEN_ERR_INT_I_set_to_clear       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_LEN_ERR_INT_I_get                ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_set_to_clear ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_get          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_tgfpf_lfield_range_LEN_ERR_INT_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_poll( b_ptr,
 |                                                                    h_ptr,
 |                                                                    word_number,
 |                                                                    mask,
 |                                                                    ( value << field_ofs),
 |                                                                    cmp,
 |                                                                    max_count,
 |                                                                    num_failed_polls,
 |                                                                    delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +------------------------------------------------------------+
 | | Register (0x000a02a0 + (N) * 0x4) PAUSE_FRAME_TX_INT_EVENT |
 | |     N[0..2]                                                |
 | +------------------------------------------------------------+
 | | bit  31:0 R/W  PAUSE_FRAME_TX_INT_I                        |
 | +------------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_I_set_to_clear       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_PAUSE_FRAME_TX_INT_I_get                ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_set_to_clear ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_get          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_tgfpf_lfield_range_PAUSE_FRAME_TX_INT_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_poll( b_ptr,
 |                                                                  h_ptr,
 |                                                                  word_number,
 |                                                                  mask,
 |                                                                  ( value << field_ofs),
 |                                                                  cmp,
 |                                                                  max_count,
 |                                                                  num_failed_polls,
 |                                                                  delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-----------------------------------------------------------+
 | | Register (0x000a02b0 + (N) * 0x4) MGMT_FRAME_TX_INT_EVENT |
 | |     N[0..2]                                               |
 | +-----------------------------------------------------------+
 | | bit  31:0 R/W  MGMT_FRAME_TX_INT_I                        |
 | +-----------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_I_set_to_clear       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_MGMT_FRAME_TX_INT_I_get                ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_set_to_clear ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_get          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_tgfpf_lfield_range_MGMT_FRAME_TX_INT_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_poll( b_ptr,
 |                                                                 h_ptr,
 |                                                                 word_number,
 |                                                                 mask,
 |                                                                 ( value << field_ofs),
 |                                                                 cmp,
 |                                                                 max_count,
 |                                                                 num_failed_polls,
 |                                                                 delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-------------------------------------------------------------+
 | | Register (0x000a02c0 + (N) * 0x4) OVERSIZE_FRM_RX_INT_EVENT |
 | |     N[0..2]                                                 |
 | +-------------------------------------------------------------+
 | | bit  31:0 R/W  OVERSIZE_FRM_RX_INT_I                        |
 | +-------------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_I_set_to_clear       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_OVERSIZE_FRM_RX_INT_I_get                ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_set_to_clear ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_get          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_tgfpf_lfield_range_OVERSIZE_FRM_RX_INT_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_poll( b_ptr,
 |                                                                   h_ptr,
 |                                                                   word_number,
 |                                                                   mask,
 |                                                                   ( value << field_ofs),
 |                                                                   cmp,
 |                                                                   max_count,
 |                                                                   num_failed_polls,
 |                                                                   delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +--------------------------------------------------------------+
 | | Register (0x000a02d0 + (N) * 0x4) UNDERSIZE_FRM_RX_INT_EVENT |
 | |     N[0..2]                                                  |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  UNDERSIZE_FRM_RX_INT_I                        |
 | +--------------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_I_set_to_clear       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_UNDERSIZE_FRM_RX_INT_I_get                ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_set_to_clear ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_get          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_tgfpf_lfield_range_UNDERSIZE_FRM_RX_INT_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_poll( b_ptr,
 |                                                                    h_ptr,
 |                                                                    word_number,
 |                                                                    mask,
 |                                                                    ( value << field_ofs),
 |                                                                    cmp,
 |                                                                    max_count,
 |                                                                    num_failed_polls,
 |                                                                    delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-----------------------------------------------------------------+
 | | Register (0x000a02e0 + (N) * 0x4) ORD_SET_FRM_LEN_ERR_INT_EVENT |
 | |     N[0..2]                                                     |
 | +-----------------------------------------------------------------+
 | | bit  31:0 R/W  ORD_SET_LEN_ERR_INT_I                            |
 | +-----------------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_I_set_to_clear       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_ORD_SET_LEN_ERR_INT_I_get                ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_set_to_clear ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_get          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_poll( mapotn_tgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_tgfpf_lfield_range_ORD_SET_LEN_ERR_INT_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_poll( b_ptr,
 |                                                                       h_ptr,
 |                                                                       word_number,
 |                                                                       mask,
 |                                                                       ( value << field_ofs),
 |                                                                       cmp,
 |                                                                       max_count,
 |                                                                       num_failed_polls,
 |                                                                       delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +---------------------------------------+
 | | Register 0x000a0318 MISC_INT_E        |
 | +---------------------------------------+
 | | bit  1 R/W  PMON_TRANSFER_END_INT_E   |
 | | bit  0 R/W  PMON_TRANSFER_START_INT_E |
 | +---------------------------------------+
 | 
 | void   mapotn_tgfpf_field_PMON_TRANSFER_END_INT_E_set   ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PMON_TRANSFER_END_INT_E_get   ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_tgfpf_field_PMON_TRANSFER_START_INT_E_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PMON_TRANSFER_START_INT_E_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x000a0328 MISC_INT_EVENT    |
 | +---------------------------------------+
 | | bit  1 R/W  PMON_TRANSFER_END_INT_I   |
 | | bit  0 R/W  PMON_TRANSFER_START_INT_I |
 | +---------------------------------------+
 | 
 | void                 mapotn_tgfpf_field_PMON_TRANSFER_END_INT_I_set_to_clear   ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32               mapotn_tgfpf_field_PMON_TRANSFER_END_INT_I_get            ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_tgfpf_field_PMON_TRANSFER_END_INT_I_poll           ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mapotn_tgfpf_field_PMON_TRANSFER_START_INT_I_set_to_clear ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32               mapotn_tgfpf_field_PMON_TRANSFER_START_INT_I_get          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_tgfpf_field_PMON_TRANSFER_START_INT_I_poll         ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x000a0380 CFG_MEM_MUTEX |
 | +-----------------------------------+
 | | bit  0 R/W  MTX_CONFIG_MEM        |
 | +-----------------------------------+
 | 
 | void   mapotn_tgfpf_field_MTX_CONFIG_MEM_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tgfpf_field_MTX_CONFIG_MEM_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x000a03a0 MGMT_FRM_MUTEX |
 | +------------------------------------+
 | | bit  0 R/W  MTX_MGMT_FRAME_MEM     |
 | +------------------------------------+
 | 
 | void   mapotn_tgfpf_field_MTX_MGMT_FRAME_MEM_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_tgfpf_field_MTX_MGMT_FRAME_MEM_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +-------------------------------------------------------+
 | | Register (0x000a0400 + (N) * 0x4) PAUSE_FRAME_REQUEST |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  PAUSE_FRAME_REQ                        |
 | +-------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_PAUSE_FRAME_REQ_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_PAUSE_FRAME_REQ_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_PAUSE_FRAME_REQ_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_PAUSE_FRAME_REQ_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x000a0410 + (N) * 0x4) MGMT_FRAME_REQUEST |
 | |     N[0..2]                                          |
 | +------------------------------------------------------+
 | | bit  31:0 R/W  MGMT_FRAME_REQ                        |
 | +------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_MGMT_FRAME_REQ_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_MGMT_FRAME_REQ_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_tgfpf_lfield_range_MGMT_FRAME_REQ_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_MGMT_FRAME_REQ_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x000a0800 + (N) * 0x4) CHAN_CFG |
 | |     N[0..95]                               |
 | +--------------------------------------------+
 | | bit  23:16 R/W  PREFETCH_FIFO_THRESH       |
 | | bit  15    R/W  CPB_MAP                    |
 | | bit  14    R/W  AUTO_FLOW_CTRL_EN          |
 | | bit  13:12 R/W  PAUSE_FRM_TEMPLATE_XON     |
 | | bit  11:10 R/W  PAUSE_FRM_TEMPLATE_XOFF    |
 | | bit  9     R/W  INSERT_FRM_PRIORITY        |
 | | bit  8     R/W  MGMT_FRM_PRIORITY          |
 | | bit  7:0   R/W  MGMT_FRM_LEN               |
 | +--------------------------------------------+
 | 
 | void   mapotn_tgfpf_field_PREFETCH_FIFO_THRESH_set          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PREFETCH_FIFO_THRESH_get          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_PREFETCH_FIFO_THRESH_set    ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_PREFETCH_FIFO_THRESH_get    ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_tgfpf_field_CPB_MAP_set                       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_CPB_MAP_get                       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_AUTO_FLOW_CTRL_EN_set             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_AUTO_FLOW_CTRL_EN_get             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XON_set        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XON_get        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XON_set  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XON_get  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XOFF_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XOFF_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XOFF_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_PAUSE_FRM_TEMPLATE_XOFF_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_tgfpf_field_INSERT_FRM_PRIORITY_set           ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_INSERT_FRM_PRIORITY_get           ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_MGMT_FRM_PRIORITY_set             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_MGMT_FRM_PRIORITY_get             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_MGMT_FRM_LEN_set                  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_MGMT_FRM_LEN_get                  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_MGMT_FRM_LEN_set            ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_MGMT_FRM_LEN_get            ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x000a1000 + (N) * 0x20) TX_CHAN_CFG_RAM0 |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  31    R/W  EXT_HDR_EN                          |
 | | bit  30:24 R/W  EXT_HDR_LEN                         |
 | | bit  22:20 R/W  TYPE_HDR_PTI                        |
 | | bit  19:16 R/W  TYPE_HDR_EXI                        |
 | | bit  15:8  R/W  TYPE_HDR_UPI                        |
 | | bit  7:6   R/W  FCS_INS                             |
 | | bit  5:4   R/W  FCS_INS_POS                         |
 | | bit  3     R/W  UPI_SEL                             |
 | +-----------------------------------------------------+
 | 
 | void   mapotn_tgfpf_field_EXT_HDR_EN_set         ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_EXT_HDR_EN_get         ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_EXT_HDR_LEN_set        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_EXT_HDR_LEN_get        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_EXT_HDR_LEN_set  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_EXT_HDR_LEN_get  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_tgfpf_field_TYPE_HDR_PTI_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_TYPE_HDR_PTI_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_TYPE_HDR_PTI_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_TYPE_HDR_PTI_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_tgfpf_field_TYPE_HDR_EXI_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_TYPE_HDR_EXI_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_TYPE_HDR_EXI_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_TYPE_HDR_EXI_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_tgfpf_field_TYPE_HDR_UPI_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_TYPE_HDR_UPI_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_TYPE_HDR_UPI_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_TYPE_HDR_UPI_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_tgfpf_field_FCS_INS_set            ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_FCS_INS_get            ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_FCS_INS_set      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_FCS_INS_get      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_tgfpf_field_FCS_INS_POS_set        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_FCS_INS_POS_get        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_FCS_INS_POS_set  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_FCS_INS_POS_get  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_tgfpf_field_UPI_SEL_set            ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_UPI_SEL_get            ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------------------+
 | | Register (0x000a1004 + (N) * 0x20) TX_CHAN_CFG_RAM1 |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  7 R/W  SCRMBL                                  |
 | | bit  6 R/W  PFCS_REV_OBYTE                          |
 | | bit  5 R/W  PFCS_REV_OBIT                           |
 | | bit  4 R/W  PFCS_REV_IBIT                           |
 | | bit  3 R/W  PFCS_CRPT_INV                           |
 | | bit  2 R/W  PFCS_INV                                |
 | | bit  1 R/W  PFCS_INIT                               |
 | | bit  0 R/W  DC_BALANCE                              |
 | +-----------------------------------------------------+
 | 
 | void   mapotn_tgfpf_field_SCRMBL_set         ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_SCRMBL_get         ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_PFCS_REV_OBYTE_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PFCS_REV_OBYTE_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_PFCS_REV_OBIT_set  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PFCS_REV_OBIT_get  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_PFCS_REV_IBIT_set  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PFCS_REV_IBIT_get  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_PFCS_CRPT_INV_set  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PFCS_CRPT_INV_get  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_PFCS_INV_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PFCS_INV_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_PFCS_INIT_set      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PFCS_INIT_get      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_DC_BALANCE_set     ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_DC_BALANCE_get     ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------------------+
 | | Register (0x000a1008 + (N) * 0x20) TX_CHAN_CFG_RAM2 |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  15:8 R/W  PACKER_FIFO_LOW_THRESH               |
 | | bit  7:0  R/W  PACKER_FIFO_HIGH_THRESH              |
 | +-----------------------------------------------------+
 | 
 | void   mapotn_tgfpf_field_PACKER_FIFO_LOW_THRESH_set        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PACKER_FIFO_LOW_THRESH_get        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_PACKER_FIFO_LOW_THRESH_set  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_PACKER_FIFO_LOW_THRESH_get  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_tgfpf_field_PACKER_FIFO_HIGH_THRESH_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_PACKER_FIFO_HIGH_THRESH_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_PACKER_FIFO_HIGH_THRESH_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_PACKER_FIFO_HIGH_THRESH_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x000a100c + (N) * 0x20) FRAME_LENGTH_CFG |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  31:16 R/W  MAX_FRM_LENGTH                      |
 | | bit  15:0  R/W  MIN_FRM_LENGTH                      |
 | +-----------------------------------------------------+
 | 
 | void   mapotn_tgfpf_field_MAX_FRM_LENGTH_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_MAX_FRM_LENGTH_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_MAX_FRM_LENGTH_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_MAX_FRM_LENGTH_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_tgfpf_field_MIN_FRM_LENGTH_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_tgfpf_field_MIN_FRM_LENGTH_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_tgfpf_field_range_MIN_FRM_LENGTH_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_field_range_MIN_FRM_LENGTH_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------+
 | | Register (0x000a1c00 + (N) * 0x4 + (M) * 0x80) PAUSE_FRAME_RAM |
 | |     N[0..17]                                                   |
 | |     M[0..3]                                                    |
 | +----------------------------------------------------------------+
 | | bit  31:0 R/W  PAUSE_FRAME_MEM                                 |
 | +----------------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_PAUSE_FRAME_MEM_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 value[18] )
 | void   mapotn_tgfpf_lfield_PAUSE_FRAME_MEM_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 value[18] )
 | void   mapotn_tgfpf_lfield_range_PAUSE_FRAME_MEM_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_PAUSE_FRAME_MEM_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register (0x000a2000 + (N) * 0x4 + (M) * 0x40) EXT_HDR_RAM |
 | |     N[0..15]                                               |
 | |     M[0..95]                                               |
 | +------------------------------------------------------------+
 | | bit  31:0 R/W  EXT_HDR_MEM                                 |
 | +------------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_EXT_HDR_MEM_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 value[16] )
 | void   mapotn_tgfpf_lfield_EXT_HDR_MEM_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 value[16] )
 | void   mapotn_tgfpf_lfield_range_EXT_HDR_MEM_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_EXT_HDR_MEM_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x000a4000 + (N) * 0x4 + (M) * 0x80) MGMT_FRAME_RAM |
 | |     N[0..31]                                                  |
 | |     M[0..95]                                                  |
 | +---------------------------------------------------------------+
 | | bit  31:0 R/W  MGMT_FRAME_MEM                                 |
 | +---------------------------------------------------------------+
 | 
 | void   mapotn_tgfpf_lfield_MGMT_FRAME_MEM_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 value[32] )
 | void   mapotn_tgfpf_lfield_MGMT_FRAME_MEM_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 value[32] )
 | void   mapotn_tgfpf_lfield_range_MGMT_FRAME_MEM_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_tgfpf_lfield_range_MGMT_FRAME_MEM_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register (0x000a8000 + (N) * 0x40) PMON_0 |
 | |     N[0..95]                              |
 | +-------------------------------------------+
 | | bit  31:0 R  TX_BYTE_CNT0                 |
 | +-------------------------------------------+
 | 
 | UINT32 mapotn_tgfpf_field_TX_BYTE_CNT0_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_tgfpf_field_range_TX_BYTE_CNT0_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register (0x000a8004 + (N) * 0x40) PMON_1 |
 | |     N[0..95]                              |
 | +-------------------------------------------+
 | | bit  7:0 R  TX_BYTE_CNT1                  |
 | +-------------------------------------------+
 | 
 | UINT32 mapotn_tgfpf_field_TX_BYTE_CNT1_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_tgfpf_field_range_TX_BYTE_CNT1_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register (0x000a8008 + (N) * 0x40) PMON_2 |
 | |     N[0..95]                              |
 | +-------------------------------------------+
 | | bit  31:0 R  MGMT_FRM_CNT                 |
 | +-------------------------------------------+
 | 
 | UINT32 mapotn_tgfpf_field_MGMT_FRM_CNT_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_tgfpf_field_range_MGMT_FRM_CNT_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register (0x000a800c + (N) * 0x40) PMON_3 |
 | |     N[0..95]                              |
 | +-------------------------------------------+
 | | bit  31:0 R  TX_FRM_CNT                   |
 | +-------------------------------------------+
 | 
 | UINT32 mapotn_tgfpf_field_TX_FRM_CNT_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_tgfpf_field_range_TX_FRM_CNT_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register (0x000a8010 + (N) * 0x40) PMON_4 |
 | |     N[0..95]                              |
 | +-------------------------------------------+
 | | bit  31:0 R  PAUSE_FRM_CNT                |
 | +-------------------------------------------+
 | 
 | UINT32 mapotn_tgfpf_field_PAUSE_FRM_CNT_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_tgfpf_field_range_PAUSE_FRM_CNT_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register (0x000a8014 + (N) * 0x40) PMON_5 |
 | |     N[0..95]                              |
 | +-------------------------------------------+
 | | bit  31:0 R  IDLE_FRM_CNT                 |
 | +-------------------------------------------+
 | 
 | UINT32 mapotn_tgfpf_field_IDLE_FRM_CNT_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_tgfpf_field_range_IDLE_FRM_CNT_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register (0x000a8018 + (N) * 0x40) PMON_6 |
 | |     N[0..95]                              |
 | +-------------------------------------------+
 | | bit  31:0 R  ERR_FRM_CNT                  |
 | +-------------------------------------------+
 | 
 | UINT32 mapotn_tgfpf_field_ERR_FRM_CNT_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_tgfpf_field_range_ERR_FRM_CNT_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register (0x000a801c + (N) * 0x40) PMON_7 |
 | |     N[0..95]                              |
 | +-------------------------------------------+
 | | bit  31:0 R  TX_FRM_CNT_UNDERSIZE         |
 | +-------------------------------------------+
 | 
 | UINT32 mapotn_tgfpf_field_TX_FRM_CNT_UNDERSIZE_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_tgfpf_field_range_TX_FRM_CNT_UNDERSIZE_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register (0x000a8020 + (N) * 0x40) PMON_8 |
 | |     N[0..95]                              |
 | +-------------------------------------------+
 | | bit  31:0 R  TX_FRM_CNT_OVERSIZE          |
 | +-------------------------------------------+
 | 
 | UINT32 mapotn_tgfpf_field_TX_FRM_CNT_OVERSIZE_get       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_tgfpf_field_range_TX_FRM_CNT_OVERSIZE_get ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MAPOTN_TGFPF_IO_H
#define _MAPOTN_TGFPF_IO_H

#include "mapotn_loc.h"
#include "mapotn_tgfpf_regs.h"
#include "mapotn_tgfpf_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mapotn_tgfpf_buffer_init               ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void                 mapotn_tgfpf_buffer_flush              ( mapotn_tgfpf_buffer_t *b_ptr )
 | UINT32               mapotn_tgfpf_reg_read                  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 mapotn_tgfpf_reg_write                 ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 mapotn_tgfpf_field_set                 ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_action_on_write_field_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_burst_read                ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_burst_write               ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE mapotn_tgfpf_poll                      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mapotn_tgfpf_reg_COMMON_CFG_write                                              ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_tgfpf_reg_COMMON_CFG_field_set                                          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tgfpf_reg_COMMON_CFG_read                                               ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_burst_write                             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_field_set                               ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_burst_read                              ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_PAUSE_FRAME_RAM_array_read                                    ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 N )
 | 
 | void                 mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_write                                    ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_field_set                                ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tgfpf_reg_PAUSE_FRM_TIME_array_read                                     ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_tgfpf_reg_TX_CHAN_RST_array_write                                       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_tgfpf_reg_TX_CHAN_RST_array_field_set                                   ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tgfpf_reg_TX_CHAN_RST_array_read                                        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_tgfpf_reg_CHAN_CFG_array_write                                          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_tgfpf_reg_CHAN_CFG_array_field_set                                      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tgfpf_reg_CHAN_CFG_array_read                                           ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_write                                  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_field_set                              ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_read                                   ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_tgfpf_reg_PMON_CTRL_write                                               ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_tgfpf_reg_PMON_CTRL_field_set                                           ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tgfpf_reg_PMON_CTRL_read                                                ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_burst_write                         ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_field_set                           ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_burst_read                          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_PAUSE_FRAME_REQUEST_array_read                                ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_burst_write                          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_field_set                            ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_burst_read                           ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_MGMT_FRAME_REQUEST_array_read                                 ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_tgfpf_reg_MISC_INT_E_write                                              ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_tgfpf_reg_MISC_INT_E_field_set                                          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tgfpf_reg_MISC_INT_E_read                                               ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_burst_write                         ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_field_set                           ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_burst_read                          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_PROTOCOL_ERR_INT_EN_array_read                                ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_burst_write                      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_field_set                        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_burst_read                       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_E_array_read                             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_burst_write                       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_field_set                         ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_burst_read                        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EN_array_read                              ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_burst_write                        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_field_set                          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_burst_read                         ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EN_array_read                               ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_burst_write                      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_field_set                        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_burst_read                       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EN_array_read                             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_burst_write                     ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_field_set                       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_burst_read                      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EN_array_read                            ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_burst_write                   ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_field_set                     ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_burst_read                    ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_E_array_read                          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_tgfpf_reg_MISC_INT_EVENT_write                                          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_tgfpf_reg_MISC_INT_EVENT_action_on_write_field_set                      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tgfpf_reg_MISC_INT_EVENT_read                                           ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_MISC_INT_EVENT_poll                                           ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_burst_write                  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_action_on_write_field_set    ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_burst_read                   ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_read                         ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_FRAME_LENGTH_ERR_INT_EVENT_array_poll                         ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_burst_write                    ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_action_on_write_field_set      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_burst_read                     ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_read                           ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_PAUSE_FRAME_TX_INT_EVENT_array_poll                           ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_burst_write                     ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_action_on_write_field_set       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_burst_read                      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_read                            ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_MGMT_FRAME_TX_INT_EVENT_array_poll                            ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_burst_write                   ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_action_on_write_field_set     ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_burst_read                    ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_read                          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_OVERSIZE_FRM_RX_INT_EVENT_array_poll                          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_burst_write                  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_action_on_write_field_set    ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_burst_read                   ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_read                         ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_UNDERSIZE_FRM_RX_INT_EVENT_array_poll                         ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_burst_write               ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_action_on_write_field_set ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_burst_read                ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_read                      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_ORD_SET_FRM_LEN_ERR_INT_EVENT_array_poll                      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_tgfpf_reg_EXT_HDR_RAM_array_burst_write                                 ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_EXT_HDR_RAM_array_field_set                                   ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_EXT_HDR_RAM_array_burst_read                                  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_EXT_HDR_RAM_array_read                                        ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 N )
 | 
 | void                 mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_burst_write                              ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_field_set                                ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_burst_read                               ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_tgfpf_reg_MGMT_FRAME_RAM_array_read                                     ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 N )
 | 
 | void                 mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_write                                  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_field_set                              ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_read                                   ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_write                                  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_field_set                              ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_read                                   ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_write                                  ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_field_set                              ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_read                                   ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_tgfpf_reg_PMON_0_array_read                                             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_tgfpf_reg_PMON_1_array_read                                             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_tgfpf_reg_PMON_2_array_read                                             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_tgfpf_reg_PMON_3_array_read                                             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_tgfpf_reg_PMON_4_array_read                                             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_tgfpf_reg_PMON_5_array_read                                             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_tgfpf_reg_PMON_6_array_read                                             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_tgfpf_reg_PMON_7_array_read                                             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_tgfpf_reg_PMON_8_array_read                                             ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_tgfpf_reg_PMON_STATUS_read                                              ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_tgfpf_reg_PMON_STATUS_poll                                              ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_tgfpf_reg_CFG_MEM_MUTEX_write                                           ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_tgfpf_reg_CFG_MEM_MUTEX_field_set                                       ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tgfpf_reg_CFG_MEM_MUTEX_read                                            ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_tgfpf_reg_MGMT_FRM_MUTEX_write                                          ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_tgfpf_reg_MGMT_FRM_MUTEX_field_set                                      ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_tgfpf_reg_MGMT_FRM_MUTEX_read                                           ( mapotn_tgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MAPOTN_TGFPF_IO_H */
