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
 *     and register accessor functions for the rgmp block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing rgmp io functions is:
 | {
 |     rgmp_buffer_t b_ptr[1];
 |     rgmp_buffer_init( b_ptr, h_ptr );
 |     value = rgmp_field_<FIELD>_get( b_ptr, h_ptr );
 |     rgmp_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     rgmp_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = rgmp_field_<FIELD>_get( NULL, h_ptr );
 | rgmp_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-------------------------------------------------+
 | | Register (0x00000000 + (N) * 0x4) CHANNEL_RESET |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  0 R/W  CHAN_RSTB                           |
 | +-------------------------------------------------+
 | 
 | void   rgmp_field_CHAN_RSTB_set ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 rgmp_field_CHAN_RSTB_get ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------------+
 | | Register (0x00000300 + (N) * 0x4) DPLM_INT_EN |
 | |     N[0..2]                                   |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  DPLM_E                         |
 | +-----------------------------------------------+
 | 
 | void   rgmp_lfield_DPLM_E_set       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_DPLM_E_get       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_DPLM_E_set ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_DPLM_E_get ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register (0x0000030c + (N) * 0x4) CSF_INT_EN |
 | |     N[0..2]                                  |
 | +----------------------------------------------+
 | | bit  31:0 R/W  CSF_E                         |
 | +----------------------------------------------+
 | 
 | void   rgmp_lfield_CSF_E_set       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_CSF_E_get       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_CSF_E_set ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_CSF_E_get ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000318 + (N) * 0x4) CM_OUT_LIMIT_INT_EN |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  CM_OUT_LIMIT_E                         |
 | +-------------------------------------------------------+
 | 
 | void   rgmp_lfield_CM_OUT_LIMIT_E_set       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_CM_OUT_LIMIT_E_get       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_CM_OUT_LIMIT_E_set ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_CM_OUT_LIMIT_E_get ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00000324 + (N) * 0x4) CND_MAX_INT_EN |
 | |     N[0..2]                                      |
 | +--------------------------------------------------+
 | | bit  31:0 R/W  CND_MAX_E                         |
 | +--------------------------------------------------+
 | 
 | void   rgmp_lfield_CND_MAX_E_set       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_CND_MAX_E_get       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_CND_MAX_E_set ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_CND_MAX_E_get ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000330 + (N) * 0x4) JC_ERR_SYNCLOSS_INT_EN |
 | |     N[0..2]                                              |
 | +----------------------------------------------------------+
 | | bit  31:0 R/W  JC_ERR_SYNCLOSS_E                         |
 | +----------------------------------------------------------+
 | 
 | void   rgmp_lfield_JC_ERR_SYNCLOSS_E_set       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_JC_ERR_SYNCLOSS_E_get       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_JC_ERR_SYNCLOSS_E_set ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_JC_ERR_SYNCLOSS_E_get ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x0000033c + (N) * 0x4) CM_CND_CRC_ERR_INT_EN |
 | |     N[0..2]                                             |
 | +---------------------------------------------------------+
 | | bit  31:0 R/W  CM_CND_CRC_ERR_E                         |
 | +---------------------------------------------------------+
 | 
 | void   rgmp_lfield_CM_CND_CRC_ERR_E_set       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_CM_CND_CRC_ERR_E_get       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_CM_CND_CRC_ERR_E_set ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_CM_CND_CRC_ERR_E_get ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000348 + (N) * 0x4) PSERV_EARLY_EXP_INT_EN |
 | |     N[0..2]                                              |
 | +----------------------------------------------------------+
 | | bit  31:0 R/W  PSERV_EARLY_E                             |
 | +----------------------------------------------------------+
 | 
 | void   rgmp_lfield_PSERV_EARLY_E_set       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_PSERV_EARLY_E_get       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_PSERV_EARLY_E_set ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_PSERV_EARLY_E_get ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000354 + (N) * 0x4) PSERV_LATE_EXP_INT_EN |
 | |     N[0..2]                                             |
 | +---------------------------------------------------------+
 | | bit  31:0 R/W  PSERV_LATE_E                             |
 | +---------------------------------------------------------+
 | 
 | void   rgmp_lfield_PSERV_LATE_E_set       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_PSERV_LATE_E_get       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_PSERV_LATE_E_set ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_PSERV_LATE_E_get ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00000360 + (N) * 0x4) SSF_SF_RCVD_INT_EN |
 | |     N[0..2]                                          |
 | +------------------------------------------------------+
 | | bit  31:0 R/W  SSF_SF_RCVD_E                         |
 | +------------------------------------------------------+
 | 
 | void   rgmp_lfield_SSF_SF_RCVD_E_set       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_SSF_SF_RCVD_E_get       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_SSF_SF_RCVD_E_set ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_SSF_SF_RCVD_E_get ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x00000400 + (N) * 0x4) DPLM_INT |
 | |     N[0..2]                                |
 | +--------------------------------------------+
 | | bit  31:0 R/W  DPLM_I                      |
 | +--------------------------------------------+
 | 
 | void   rgmp_lfield_DPLM_I_set_to_clear       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_DPLM_I_get                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_DPLM_I_set_to_clear ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_DPLM_I_get          ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_DPLM_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_DPLM_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_DPLM_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_DPLM_I_set_to_clear", start_bit, stop_bit);
 |     return rgmp_reg_DPLM_INT_array_poll( b_ptr,
 |                                          h_ptr,
 |                                          word_number,
 |                                          mask,
 |                                          ( value << field_ofs),
 |                                          cmp,
 |                                          max_count,
 |                                          num_failed_polls,
 |                                          delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-------------------------------------------+
 | | Register (0x0000040c + (N) * 0x4) CSF_INT |
 | |     N[0..2]                               |
 | +-------------------------------------------+
 | | bit  31:0 R/W  CSF_I                      |
 | +-------------------------------------------+
 | 
 | void   rgmp_lfield_CSF_I_set_to_clear       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_CSF_I_get                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_CSF_I_set_to_clear ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_CSF_I_get          ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CSF_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CSF_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CSF_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_CSF_I_set_to_clear", start_bit, stop_bit);
 |     return rgmp_reg_CSF_INT_array_poll( b_ptr,
 |                                         h_ptr,
 |                                         word_number,
 |                                         mask,
 |                                         ( value << field_ofs),
 |                                         cmp,
 |                                         max_count,
 |                                         num_failed_polls,
 |                                         delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-----------------------------------------------------------------+
 | | Register (0x00000418 + (N) * 0x4) CM_OUT_OF_LIMIT_VIOLATION_INT |
 | |     N[0..2]                                                     |
 | +-----------------------------------------------------------------+
 | | bit  31:0 R/W  CM_OUT_LIMIT_I                                   |
 | +-----------------------------------------------------------------+
 | 
 | void   rgmp_lfield_CM_OUT_LIMIT_I_set_to_clear       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_CM_OUT_LIMIT_I_get                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_CM_OUT_LIMIT_I_set_to_clear ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_CM_OUT_LIMIT_I_get          ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CM_OUT_LIMIT_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CM_OUT_LIMIT_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CM_OUT_LIMIT_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_CM_OUT_LIMIT_I_set_to_clear", start_bit, stop_bit);
 |     return rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_poll( b_ptr,
 |                                                               h_ptr,
 |                                                               word_number,
 |                                                               mask,
 |                                                               ( value << field_ofs),
 |                                                               cmp,
 |                                                               max_count,
 |                                                               num_failed_polls,
 |                                                               delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-----------------------------------------------+
 | | Register (0x00000424 + (N) * 0x4) CND_MAX_INT |
 | |     N[0..2]                                   |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  CND_MAX_I                      |
 | +-----------------------------------------------+
 | 
 | void   rgmp_lfield_CND_MAX_I_set_to_clear       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_CND_MAX_I_get                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_CND_MAX_I_set_to_clear ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_CND_MAX_I_get          ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CND_MAX_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CND_MAX_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CND_MAX_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_CND_MAX_I_set_to_clear", start_bit, stop_bit);
 |     return rgmp_reg_CND_MAX_INT_array_poll( b_ptr,
 |                                             h_ptr,
 |                                             word_number,
 |                                             mask,
 |                                             ( value << field_ofs),
 |                                             cmp,
 |                                             max_count,
 |                                             num_failed_polls,
 |                                             delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000430 + (N) * 0x4) JC_ERR_SYNCLOSS_INT |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  JC_ERR_SYNCLOSS_I                      |
 | +-------------------------------------------------------+
 | 
 | void   rgmp_lfield_JC_ERR_SYNCLOSS_I_set_to_clear       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_JC_ERR_SYNCLOSS_I_get                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_JC_ERR_SYNCLOSS_I_set_to_clear ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_JC_ERR_SYNCLOSS_I_get          ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_JC_ERR_SYNCLOSS_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_JC_ERR_SYNCLOSS_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_JC_ERR_SYNCLOSS_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_JC_ERR_SYNCLOSS_I_set_to_clear", start_bit, stop_bit);
 |     return rgmp_reg_JC_ERR_SYNCLOSS_INT_array_poll( b_ptr,
 |                                                     h_ptr,
 |                                                     word_number,
 |                                                     mask,
 |                                                     ( value << field_ofs),
 |                                                     cmp,
 |                                                     max_count,
 |                                                     num_failed_polls,
 |                                                     delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +------------------------------------------------------+
 | | Register (0x0000043c + (N) * 0x4) CM_CND_CRC_ERR_INT |
 | |     N[0..2]                                          |
 | +------------------------------------------------------+
 | | bit  31:0 R/W  CM_CND_CRC_ERR_I                      |
 | +------------------------------------------------------+
 | 
 | void   rgmp_lfield_CM_CND_CRC_ERR_I_set_to_clear       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_CM_CND_CRC_ERR_I_get                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_CM_CND_CRC_ERR_I_set_to_clear ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_CM_CND_CRC_ERR_I_get          ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CM_CND_CRC_ERR_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CM_CND_CRC_ERR_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CM_CND_CRC_ERR_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_CM_CND_CRC_ERR_I_set_to_clear", start_bit, stop_bit);
 |     return rgmp_reg_CM_CND_CRC_ERR_INT_array_poll( b_ptr,
 |                                                    h_ptr,
 |                                                    word_number,
 |                                                    mask,
 |                                                    ( value << field_ofs),
 |                                                    cmp,
 |                                                    max_count,
 |                                                    num_failed_polls,
 |                                                    delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +----------------------------------------------------------+
 | | Register (0x00000448 + (N) * 0x4) PSERV_EARLY_EXPIRY_INT |
 | |     N[0..2]                                              |
 | +----------------------------------------------------------+
 | | bit  31:0 R/W  PSERV_EARLY_I                             |
 | +----------------------------------------------------------+
 | 
 | void   rgmp_lfield_PSERV_EARLY_I_set_to_clear       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_PSERV_EARLY_I_get                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_PSERV_EARLY_I_set_to_clear ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_PSERV_EARLY_I_get          ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_PSERV_EARLY_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_PSERV_EARLY_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_PSERV_EARLY_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_PSERV_EARLY_I_set_to_clear", start_bit, stop_bit);
 |     return rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_poll( b_ptr,
 |                                                        h_ptr,
 |                                                        word_number,
 |                                                        mask,
 |                                                        ( value << field_ofs),
 |                                                        cmp,
 |                                                        max_count,
 |                                                        num_failed_polls,
 |                                                        delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000454 + (N) * 0x4) PSERV_LATE_EXPIRY_INT |
 | |     N[0..2]                                             |
 | +---------------------------------------------------------+
 | | bit  31:0 R/W  PSERV_LATE_I                             |
 | +---------------------------------------------------------+
 | 
 | void   rgmp_lfield_PSERV_LATE_I_set_to_clear       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_PSERV_LATE_I_get                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_PSERV_LATE_I_set_to_clear ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_PSERV_LATE_I_get          ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_PSERV_LATE_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_PSERV_LATE_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_PSERV_LATE_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_PSERV_LATE_I_set_to_clear", start_bit, stop_bit);
 |     return rgmp_reg_PSERV_LATE_EXPIRY_INT_array_poll( b_ptr,
 |                                                       h_ptr,
 |                                                       word_number,
 |                                                       mask,
 |                                                       ( value << field_ofs),
 |                                                       cmp,
 |                                                       max_count,
 |                                                       num_failed_polls,
 |                                                       delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +---------------------------------------------------+
 | | Register (0x00000460 + (N) * 0x4) SSF_SF_RCVD_INT |
 | |     N[0..2]                                       |
 | +---------------------------------------------------+
 | | bit  31:0 R/W  SSF_SF_RCVD_I                      |
 | +---------------------------------------------------+
 | 
 | void   rgmp_lfield_SSF_SF_RCVD_I_set_to_clear       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_SSF_SF_RCVD_I_get                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | void   rgmp_lfield_range_SSF_SF_RCVD_I_set_to_clear ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 rgmp_lfield_range_SSF_SF_RCVD_I_get          ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_SSF_SF_RCVD_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_SSF_SF_RCVD_I_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_SSF_SF_RCVD_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_SSF_SF_RCVD_I_set_to_clear", start_bit, stop_bit);
 |     return rgmp_reg_SSF_SF_RCVD_INT_array_poll( b_ptr,
 |                                                 h_ptr,
 |                                                 word_number,
 |                                                 mask,
 |                                                 ( value << field_ofs),
 |                                                 cmp,
 |                                                 max_count,
 |                                                 num_failed_polls,
 |                                                 delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +------------------------------------------------+
 | | Register (0x00000500 + (N) * 0x4) DPLM_INT_VAL |
 | |     N[0..2]                                    |
 | +------------------------------------------------+
 | | bit  31:0 R  DPLM_V                            |
 | +------------------------------------------------+
 | 
 | void   rgmp_lfield_DPLM_V_get       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 rgmp_lfield_range_DPLM_V_get ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_DPLM_V_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_DPLM_V_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_DPLM_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_DPLM_V_set_to_clear", start_bit, stop_bit);
 |     return rgmp_reg_DPLM_INT_VAL_array_poll( b_ptr,
 |                                              h_ptr,
 |                                              word_number,
 |                                              mask,
 |                                              ( value << field_ofs),
 |                                              cmp,
 |                                              max_count,
 |                                              num_failed_polls,
 |                                              delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-----------------------------------------------+
 | | Register (0x0000050c + (N) * 0x4) CSF_INT_VAL |
 | |     N[0..2]                                   |
 | +-----------------------------------------------+
 | | bit  31:0 R  CSF_V                            |
 | +-----------------------------------------------+
 | 
 | void   rgmp_lfield_CSF_V_get       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 rgmp_lfield_range_CSF_V_get ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CSF_V_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_CSF_V_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_CSF_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_CSF_V_set_to_clear", start_bit, stop_bit);
 |     return rgmp_reg_CSF_INT_VAL_array_poll( b_ptr,
 |                                             h_ptr,
 |                                             word_number,
 |                                             mask,
 |                                             ( value << field_ofs),
 |                                             cmp,
 |                                             max_count,
 |                                             num_failed_polls,
 |                                             delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000518 + (N) * 0x4) SSF_SF_RCVD_INT_VAL |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R  SSF_SF_RCVD_V                            |
 | +-------------------------------------------------------+
 | 
 | void   rgmp_lfield_SSF_SF_RCVD_V_get       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 rgmp_lfield_range_SSF_SF_RCVD_V_get ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_SSF_SF_RCVD_V_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE rgmp_lfield_range_SSF_SF_RCVD_V_poll( rgmp_buffer_t *b_ptr,
 | rgmp_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "rgmp_lfield_range_SSF_SF_RCVD_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "rgmp_lfield_range_SSF_SF_RCVD_V_set_to_clear", start_bit, stop_bit);
 |     return rgmp_reg_SSF_SF_RCVD_INT_VAL_array_poll( b_ptr,
 |                                                     h_ptr,
 |                                                     word_number,
 |                                                     mask,
 |                                                     ( value << field_ofs),
 |                                                     cmp,
 |                                                     max_count,
 |                                                     num_failed_polls,
 |                                                     delay_between_polls_in_microseconds);
 | }
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _RGMP_IO_H
#define _RGMP_IO_H

#include "rgmp_loc.h"
#include "rgmp_regs.h"
#include "rgmp_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 rgmp_buffer_init               ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr )
 | void                 rgmp_buffer_flush              ( rgmp_buffer_t *b_ptr )
 | UINT32               rgmp_reg_read                  ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 rgmp_reg_write                 ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 rgmp_field_set                 ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_action_on_write_field_set ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_burst_read                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 rgmp_burst_write               ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE rgmp_poll                      ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 rgmp_reg_CHANNEL_RESET_array_write                                     ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 rgmp_reg_CHANNEL_RESET_array_field_set                                 ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               rgmp_reg_CHANNEL_RESET_array_read                                      ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 rgmp_reg_DPLM_INT_EN_array_burst_write                                 ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_DPLM_INT_EN_array_field_set                                   ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_DPLM_INT_EN_array_burst_read                                  ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_DPLM_INT_EN_array_read                                        ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 rgmp_reg_CSF_INT_EN_array_burst_write                                  ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_CSF_INT_EN_array_field_set                                    ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_CSF_INT_EN_array_burst_read                                   ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_CSF_INT_EN_array_read                                         ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 rgmp_reg_CM_OUT_LIMIT_INT_EN_array_burst_write                         ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_CM_OUT_LIMIT_INT_EN_array_field_set                           ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_CM_OUT_LIMIT_INT_EN_array_burst_read                          ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_CM_OUT_LIMIT_INT_EN_array_read                                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 rgmp_reg_CND_MAX_INT_EN_array_burst_write                              ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_CND_MAX_INT_EN_array_field_set                                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_CND_MAX_INT_EN_array_burst_read                               ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_CND_MAX_INT_EN_array_read                                     ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_burst_write                      ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_field_set                        ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_burst_read                       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_JC_ERR_SYNCLOSS_INT_EN_array_read                             ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_burst_write                       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_field_set                         ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_burst_read                        ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_CM_CND_CRC_ERR_INT_EN_array_read                              ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_burst_write                      ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_field_set                        ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_burst_read                       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_PSERV_EARLY_EXP_INT_EN_array_read                             ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 rgmp_reg_PSERV_LATE_EXP_INT_EN_array_burst_write                       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_PSERV_LATE_EXP_INT_EN_array_field_set                         ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_PSERV_LATE_EXP_INT_EN_array_burst_read                        ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_PSERV_LATE_EXP_INT_EN_array_read                              ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 rgmp_reg_SSF_SF_RCVD_INT_EN_array_burst_write                          ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_SSF_SF_RCVD_INT_EN_array_field_set                            ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_SSF_SF_RCVD_INT_EN_array_burst_read                           ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_SSF_SF_RCVD_INT_EN_array_read                                 ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 rgmp_reg_DPLM_INT_array_burst_write                                    ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_DPLM_INT_array_action_on_write_field_set                      ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_DPLM_INT_array_burst_read                                     ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_DPLM_INT_array_read                                           ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE rgmp_reg_DPLM_INT_array_poll                                           ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 rgmp_reg_CSF_INT_array_burst_write                                     ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_CSF_INT_array_action_on_write_field_set                       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_CSF_INT_array_burst_read                                      ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_CSF_INT_array_read                                            ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE rgmp_reg_CSF_INT_array_poll                                            ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_burst_write               ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_action_on_write_field_set ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_burst_read                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_read                      ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE rgmp_reg_CM_OUT_OF_LIMIT_VIOLATION_INT_array_poll                      ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 rgmp_reg_CND_MAX_INT_array_burst_write                                 ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_CND_MAX_INT_array_action_on_write_field_set                   ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_CND_MAX_INT_array_burst_read                                  ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_CND_MAX_INT_array_read                                        ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE rgmp_reg_CND_MAX_INT_array_poll                                        ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 rgmp_reg_JC_ERR_SYNCLOSS_INT_array_burst_write                         ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_JC_ERR_SYNCLOSS_INT_array_action_on_write_field_set           ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_JC_ERR_SYNCLOSS_INT_array_burst_read                          ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_JC_ERR_SYNCLOSS_INT_array_read                                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE rgmp_reg_JC_ERR_SYNCLOSS_INT_array_poll                                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 rgmp_reg_CM_CND_CRC_ERR_INT_array_burst_write                          ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_CM_CND_CRC_ERR_INT_array_action_on_write_field_set            ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_CM_CND_CRC_ERR_INT_array_burst_read                           ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_CM_CND_CRC_ERR_INT_array_read                                 ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE rgmp_reg_CM_CND_CRC_ERR_INT_array_poll                                 ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_burst_write                      ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_action_on_write_field_set        ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_burst_read                       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_read                             ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE rgmp_reg_PSERV_EARLY_EXPIRY_INT_array_poll                             ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 rgmp_reg_PSERV_LATE_EXPIRY_INT_array_burst_write                       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_PSERV_LATE_EXPIRY_INT_array_action_on_write_field_set         ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_PSERV_LATE_EXPIRY_INT_array_burst_read                        ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_PSERV_LATE_EXPIRY_INT_array_read                              ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE rgmp_reg_PSERV_LATE_EXPIRY_INT_array_poll                              ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 rgmp_reg_SSF_SF_RCVD_INT_array_burst_write                             ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 rgmp_reg_SSF_SF_RCVD_INT_array_action_on_write_field_set               ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rgmp_reg_SSF_SF_RCVD_INT_array_burst_read                              ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_SSF_SF_RCVD_INT_array_read                                    ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE rgmp_reg_SSF_SF_RCVD_INT_array_poll                                    ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 rgmp_reg_DPLM_INT_VAL_array_burst_read                                 ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_DPLM_INT_VAL_array_read                                       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE rgmp_reg_DPLM_INT_VAL_array_poll                                       ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 rgmp_reg_CSF_INT_VAL_array_burst_read                                  ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_CSF_INT_VAL_array_read                                        ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE rgmp_reg_CSF_INT_VAL_array_poll                                        ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 rgmp_reg_SSF_SF_RCVD_INT_VAL_array_burst_read                          ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               rgmp_reg_SSF_SF_RCVD_INT_VAL_array_read                                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE rgmp_reg_SSF_SF_RCVD_INT_VAL_array_poll                                ( rgmp_buffer_t *b_ptr, rgmp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _RGMP_IO_H */
