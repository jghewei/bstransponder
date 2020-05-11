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
 *     and register accessor functions for the odtu_dmx block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing odtu_dmx io functions is:
 | {
 |     odtu_dmx_buffer_t b_ptr[1];
 |     odtu_dmx_buffer_init( b_ptr, h_ptr );
 |     value = odtu_dmx_field_<FIELD>_get( b_ptr, h_ptr );
 |     odtu_dmx_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     odtu_dmx_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = odtu_dmx_field_<FIELD>_get( NULL, h_ptr );
 | odtu_dmx_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------------------+
 | | Register (0x00000000 + (N) * 0x10) HO_CHANNEL_RESET |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  0 R/W  HO_CHNL_RSTB                            |
 | +-----------------------------------------------------+
 | 
 | void   odtu_dmx_field_HO_CHNL_RSTB_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odtu_dmx_field_HO_CHNL_RSTB_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000004 + (N) * 0x10) PASS_THROUGH_CFG_REG |
 | |     N[0..95]                                            |
 | +---------------------------------------------------------+
 | | bit  0 R/W  PASSTHRU                                    |
 | +---------------------------------------------------------+
 | 
 | void   odtu_dmx_field_PASSTHRU_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odtu_dmx_field_PASSTHRU_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000008 + (N) * 0x10) NON_INTRUSIVE_MODE |
 | |     N[0..95]                                          |
 | +-------------------------------------------------------+
 | | bit  0 R/W  NON_INTRUSVE_MODE                         |
 | +-------------------------------------------------------+
 | 
 | void   odtu_dmx_field_NON_INTRUSVE_MODE_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odtu_dmx_field_NON_INTRUSVE_MODE_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------------------+
 | | Register (0x00000600 + (N) * 0x4) LO_CHANNEL_RESET |
 | |     N[0..95]                                       |
 | +----------------------------------------------------+
 | | bit  0 R/W  LO_CHNL_RSTB                           |
 | +----------------------------------------------------+
 | 
 | void   odtu_dmx_field_LO_CHNL_RSTB_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odtu_dmx_field_LO_CHNL_RSTB_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00000780 + (N) * 0x4) CONFIG_UPDATE_REG |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  1 R/W  CONFIG_MODE                             |
 | | bit  0 R/W  CONFIG_UPDATE                           |
 | +-----------------------------------------------------+
 | 
 | void                 odtu_dmx_field_CONFIG_MODE_set    ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               odtu_dmx_field_CONFIG_MODE_get    ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_field_CONFIG_MODE_poll   ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odtu_dmx_field_CONFIG_UPDATE_set  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               odtu_dmx_field_CONFIG_UPDATE_get  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_field_CONFIG_UPDATE_poll ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------+
 | | Register 0x00000900 FORCE_CONFIG_REG |
 | +--------------------------------------+
 | | bit  1 R/W  FORCE_CONFIG_ADDR        |
 | | bit  0 R/W  FORCE_CONFIG             |
 | +--------------------------------------+
 | 
 | void   odtu_dmx_field_FORCE_CONFIG_ADDR_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | UINT32 odtu_dmx_field_FORCE_CONFIG_ADDR_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | void   odtu_dmx_field_FORCE_CONFIG_set      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | UINT32 odtu_dmx_field_FORCE_CONFIG_get      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000904 + (N) * 0x4) CONFIG_UPDATE_DNE_EN |
 | |     N[0..2]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R/W  CONFIG_UPDATE_DNE_E                     |
 | +--------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_CONFIG_UPDATE_DNE_E_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_CONFIG_UPDATE_DNE_E_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_E_set", start_bit, stop_bit);
 |     return odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_poll( b_ptr,
 |                                                          h_ptr,
 |                                                          word_number,
 |                                                          mask,
 |                                                          ( value << field_ofs),
 |                                                          cmp,
 |                                                          max_count,
 |                                                          num_failed_polls,
 |                                                          delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +---------------------------------------------------+
 | | Register (0x00000910 + (N) * 0x4) HAO_STAT_INT_EN |
 | |     N[0..2]                                       |
 | +---------------------------------------------------+
 | | bit  31:0 R/W  HAO_STATUS_AVL_E                   |
 | +---------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_HAO_STATUS_AVL_E_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_HAO_STATUS_AVL_E_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_HAO_STATUS_AVL_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_HAO_STATUS_AVL_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x0000091c + (N) * 0x4) DPLM_INT_EN |
 | |     N[0..2]                                   |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  DPLM_E                         |
 | +-----------------------------------------------+
 | 
 | void   odtu_dmx_lfield_DPLM_E_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_DPLM_E_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_DPLM_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_DPLM_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00000928 + (N) * 0x4) DMSIM_INT_EN |
 | |     N[0..2]                                    |
 | +------------------------------------------------+
 | | bit  31:0 R/W  DMSIM_E                         |
 | +------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_DMSIM_E_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_DMSIM_E_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_DMSIM_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_DMSIM_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000934 + (N) * 0x4) STS_SW_OVFLW_INT_EN |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  STS_SW_OVFLW_E                         |
 | +-------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_STS_SW_OVFLW_E_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_STS_SW_OVFLW_E_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_STS_SW_OVFLW_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_STS_SW_OVFLW_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00000940 DLOOMFI_INT_EN |
 | +------------------------------------+
 | | bit  0 R/W  DLOOMFI_E              |
 | +------------------------------------+
 | 
 | void   odtu_dmx_field_DLOOMFI_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | UINT32 odtu_dmx_field_DLOOMFI_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x00000944 OOM_OM_INT_EN |
 | +-----------------------------------+
 | | bit  0 R/W  OOM_IM_E              |
 | +-----------------------------------+
 | 
 | void   odtu_dmx_field_OOM_IM_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | UINT32 odtu_dmx_field_OOM_IM_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | 
 | +------------------------------------------------------+
 | | Register (0x0000094c + (N) * 0x4) SSF_SF_RCVD_INT_EN |
 | |     N[0..2]                                          |
 | +------------------------------------------------------+
 | | bit  31:0 R/W  SSF_SF_RCVD_E                         |
 | +------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_SSF_SF_RCVD_E_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_SSF_SF_RCVD_E_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_SSF_SF_RCVD_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_SSF_SF_RCVD_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000958 RCOH_STAT_TRANSFR_DONE_EN |
 | +-----------------------------------------------+
 | | bit  0 R/W  RCOH_TRNSFR_SHDW_DNE_E            |
 | +-----------------------------------------------+
 | 
 | void   odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | UINT32 odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | 
 | +------------------------------------------------------+
 | | Register (0x0000095c + (N) * 0x4) CM_OUT_OF_LIMIT_EN |
 | |     N[0..2]                                          |
 | +------------------------------------------------------+
 | | bit  31:0 R/W  CM_OUT_LIMIT_E                        |
 | +------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_CM_OUT_LIMIT_E_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_CM_OUT_LIMIT_E_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_CM_OUT_LIMIT_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_CM_OUT_LIMIT_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00000968 + (N) * 0x4) CND_MAX_INT_EN |
 | |     N[0..2]                                      |
 | +--------------------------------------------------+
 | | bit  31:0 R/W  CND_MAX_E                         |
 | +--------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_CND_MAX_E_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_CND_MAX_E_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_CND_MAX_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_CND_MAX_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000974 + (N) * 0x4) JC_NOMTCH_LOSSYNC_E |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  JC_NOMTCH_LOSSYNC_E                    |
 | +-------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_E_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_E_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00000980 + (N) * 0x4) CM_CND_CRC_ERR_EN |
 | |     N[0..2]                                         |
 | +-----------------------------------------------------+
 | | bit  31:0 R/W  CM_CND_CRC_ERR_E                     |
 | +-----------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_CM_CND_CRC_ERR_E_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_CM_CND_CRC_ERR_E_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_CM_CND_CRC_ERR_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_CM_CND_CRC_ERR_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x0000098c + (N) * 0x4) ERLY_PSERV_EXPRY_EN |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  ERLY_PSERV_EXPRY_E                     |
 | +-------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_ERLY_PSERV_EXPRY_E_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_ERLY_PSERV_EXPRY_E_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000998 + (N) * 0x4) LATE_PSERV_EXPRY_EN |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  LATE_PSERV_EXPRY_E                     |
 | +-------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_LATE_PSERV_EXPRY_E_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_LATE_PSERV_EXPRY_E_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_LATE_PSERV_EXPRY_E_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_LATE_PSERV_EXPRY_E_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000009a4 + (N) * 0x4) CONFIG_UPDATE_DNE_INT |
 | |     N[0..2]                                             |
 | +---------------------------------------------------------+
 | | bit  31:0 R/W  CONFIG_UPDATE_DNE_I                      |
 | +---------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_CONFIG_UPDATE_DNE_I_set_to_clear       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_CONFIG_UPDATE_DNE_I_get                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_CONFIG_UPDATE_DNE_I_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_poll( b_ptr,
 |                                                           h_ptr,
 |                                                           word_number,
 |                                                           mask,
 |                                                           ( value << field_ofs),
 |                                                           cmp,
 |                                                           max_count,
 |                                                           num_failed_polls,
 |                                                           delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +------------------------------------------------+
 | | Register (0x000009b0 + (N) * 0x4) HAO_STAT_INT |
 | |     N[0..2]                                    |
 | +------------------------------------------------+
 | | bit  31:0 R/W  HAO_STATUS_AVL_I                |
 | +------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_HAO_STATUS_AVL_I_set_to_clear       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_HAO_STATUS_AVL_I_get                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_HAO_STATUS_AVL_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_HAO_STATUS_AVL_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_HAO_STATUS_AVL_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_HAO_STATUS_AVL_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_HAO_STATUS_AVL_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_HAO_STATUS_AVL_I_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_HAO_STAT_INT_array_poll( b_ptr,
 |                                                  h_ptr,
 |                                                  word_number,
 |                                                  mask,
 |                                                  ( value << field_ofs),
 |                                                  cmp,
 |                                                  max_count,
 |                                                  num_failed_polls,
 |                                                  delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +--------------------------------------------+
 | | Register (0x000009bc + (N) * 0x4) DPLM_INT |
 | |     N[0..2]                                |
 | +--------------------------------------------+
 | | bit  31:0 R/W  DPLM_I                      |
 | +--------------------------------------------+
 | 
 | void   odtu_dmx_lfield_DPLM_I_set_to_clear       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_DPLM_I_get                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_DPLM_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_DPLM_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DPLM_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DPLM_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DPLM_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_DPLM_I_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_DPLM_INT_array_poll( b_ptr,
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
 | +---------------------------------------------+
 | | Register (0x000009c8 + (N) * 0x4) DMSIM_INT |
 | |     N[0..2]                                 |
 | +---------------------------------------------+
 | | bit  31:0 R/W  DMSIM_I                      |
 | +---------------------------------------------+
 | 
 | void   odtu_dmx_lfield_DMSIM_I_set_to_clear       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_DMSIM_I_get                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_DMSIM_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_DMSIM_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DMSIM_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DMSIM_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DMSIM_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_DMSIM_I_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_DMSIM_INT_array_poll( b_ptr,
 |                                               h_ptr,
 |                                               word_number,
 |                                               mask,
 |                                               ( value << field_ofs),
 |                                               cmp,
 |                                               max_count,
 |                                               num_failed_polls,
 |                                               delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +----------------------------------------------------+
 | | Register (0x000009d4 + (N) * 0x4) STS_SW_OVFLW_INT |
 | |     N[0..2]                                        |
 | +----------------------------------------------------+
 | | bit  31:0 R/W  STS_SW_OVFLW_I                      |
 | +----------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_STS_SW_OVFLW_I_set_to_clear       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_STS_SW_OVFLW_I_get                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_STS_SW_OVFLW_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_STS_SW_OVFLW_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_STS_SW_OVFLW_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_STS_SW_OVFLW_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_STS_SW_OVFLW_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_STS_SW_OVFLW_I_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_STS_SW_OVFLW_INT_array_poll( b_ptr,
 |                                                      h_ptr,
 |                                                      word_number,
 |                                                      mask,
 |                                                      ( value << field_ofs),
 |                                                      cmp,
 |                                                      max_count,
 |                                                      num_failed_polls,
 |                                                      delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +---------------------------------+
 | | Register 0x000009e0 DLOOMFI_INT |
 | +---------------------------------+
 | | bit  0 R/W  DLOOMFI_I           |
 | +---------------------------------+
 | 
 | void                 odtu_dmx_field_DLOOMFI_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | UINT32               odtu_dmx_field_DLOOMFI_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odtu_dmx_field_DLOOMFI_I_poll         ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------+
 | | Register 0x000009e4 OOM_IM_INT |
 | +--------------------------------+
 | | bit  0 R/W  OOM_IM_I           |
 | +--------------------------------+
 | 
 | void                 odtu_dmx_field_OOM_IM_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | UINT32               odtu_dmx_field_OOM_IM_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odtu_dmx_field_OOM_IM_I_poll         ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------------+
 | | Register (0x000009ec + (N) * 0x4) SSF_SF_RCVD_INT |
 | |     N[0..2]                                       |
 | +---------------------------------------------------+
 | | bit  31:0 R/W  SSF_SF_RCVD_I                      |
 | +---------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_SSF_SF_RCVD_I_set_to_clear       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_SSF_SF_RCVD_I_get                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_SSF_SF_RCVD_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_SSF_SF_RCVD_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_SSF_SF_RCVD_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_SSF_SF_RCVD_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_SSF_SF_RCVD_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_SSF_SF_RCVD_I_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_SSF_SF_RCVD_INT_array_poll( b_ptr,
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
 | +-------------------------------------------------+
 | | Register 0x000009f8 RCOH_STAT_TRANSFER_DONE_INT |
 | +-------------------------------------------------+
 | | bit  0 R/W  RCOH_TRNSFR_SHDW_DNE_I              |
 | +-------------------------------------------------+
 | 
 | void                 odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | UINT32               odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odtu_dmx_field_RCOH_TRNSFR_SHDW_DNE_I_poll         ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------------------+
 | | Register (0x000009fc + (N) * 0x4) CM_OUT_OF_LIMIT_INT |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  CM_OUT_LIMIT_I                         |
 | +-------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_CM_OUT_LIMIT_I_set_to_clear       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_CM_OUT_LIMIT_I_get                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_CM_OUT_LIMIT_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_CM_OUT_LIMIT_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CM_OUT_LIMIT_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CM_OUT_LIMIT_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CM_OUT_LIMIT_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_CM_OUT_LIMIT_I_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_poll( b_ptr,
 |                                                         h_ptr,
 |                                                         word_number,
 |                                                         mask,
 |                                                         ( value << field_ofs),
 |                                                         cmp,
 |                                                         max_count,
 |                                                         num_failed_polls,
 |                                                         delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-------------------------------------------------------------+
 | | Register (0x00000a08 + (N) * 0x4) CND_MAX_VAL_VIOLATION_INT |
 | |     N[0..2]                                                 |
 | +-------------------------------------------------------------+
 | | bit  31:0 R/W  CND_MAX_I                                    |
 | +-------------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_CND_MAX_I_set_to_clear       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_CND_MAX_I_get                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_CND_MAX_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_CND_MAX_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CND_MAX_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CND_MAX_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CND_MAX_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_CND_MAX_I_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_poll( b_ptr,
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
 | +----------------------------------------------------------+
 | | Register (0x00000a14 + (N) * 0x4) JC_NOMATCH_LOSSYNC_INT |
 | |     N[0..2]                                              |
 | +----------------------------------------------------------+
 | | bit  31:0 R/W  JC_NOMTCH_LOSSYNC_I                       |
 | +----------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_I_set_to_clear       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_JC_NOMTCH_LOSSYNC_I_get                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_JC_NOMTCH_LOSSYNC_I_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_poll( b_ptr,
 |                                                            h_ptr,
 |                                                            word_number,
 |                                                            mask,
 |                                                            ( value << field_ofs),
 |                                                            cmp,
 |                                                            max_count,
 |                                                            num_failed_polls,
 |                                                            delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +------------------------------------------------------+
 | | Register (0x00000a20 + (N) * 0x4) CM_CND_CRC_ERR_INT |
 | |     N[0..2]                                          |
 | +------------------------------------------------------+
 | | bit  31:0 R/W  CM_CND_CRC_ERR_I                      |
 | +------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_CM_CND_CRC_ERR_I_set_to_clear       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_CM_CND_CRC_ERR_I_get                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_CM_CND_CRC_ERR_I_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_poll( b_ptr,
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
 | +--------------------------------------------------------+
 | | Register (0x00000a2c + (N) * 0x4) ERLY_PSERV_EXPRY_INT |
 | |     N[0..2]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R/W  ERLY_PSERV_EXPRY_I                      |
 | +--------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_ERLY_PSERV_EXPRY_I_set_to_clear       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_ERLY_PSERV_EXPRY_I_get                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_ERLY_PSERV_EXPRY_I_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_poll( b_ptr,
 |                                                          h_ptr,
 |                                                          word_number,
 |                                                          mask,
 |                                                          ( value << field_ofs),
 |                                                          cmp,
 |                                                          max_count,
 |                                                          num_failed_polls,
 |                                                          delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000a38 + (N) * 0x4) LATE_PSERV_EXPRY_INT |
 | |     N[0..2]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R/W  LATE_PSERV_EXPRY_I                      |
 | +--------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_LATE_PSERV_EXPRY_I_set_to_clear       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_LATE_PSERV_EXPRY_I_get                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | void   odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_set_to_clear ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_LATE_PSERV_EXPRY_I_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_poll( b_ptr,
 |                                                          h_ptr,
 |                                                          word_number,
 |                                                          mask,
 |                                                          ( value << field_ofs),
 |                                                          cmp,
 |                                                          max_count,
 |                                                          num_failed_polls,
 |                                                          delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +------------------------------------------------+
 | | Register (0x00000a44 + (N) * 0x4) DPLM_INT_VAL |
 | |     N[0..2]                                    |
 | +------------------------------------------------+
 | | bit  31:0 R  DPLM_V                            |
 | +------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_DPLM_V_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 odtu_dmx_lfield_range_DPLM_V_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DPLM_V_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DPLM_V_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DPLM_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_DPLM_V_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_DPLM_INT_VAL_array_poll( b_ptr,
 |                                                  h_ptr,
 |                                                  word_number,
 |                                                  mask,
 |                                                  ( value << field_ofs),
 |                                                  cmp,
 |                                                  max_count,
 |                                                  num_failed_polls,
 |                                                  delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-------------------------------------------------+
 | | Register (0x00000a50 + (N) * 0x4) DMSIM_INT_VAL |
 | |     N[0..2]                                     |
 | +-------------------------------------------------+
 | | bit  31:0 R  DMSIM_V                            |
 | +-------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_DMSIM_V_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 odtu_dmx_lfield_range_DMSIM_V_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DMSIM_V_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_DMSIM_V_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_DMSIM_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_DMSIM_V_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_DMSIM_INT_VAL_array_poll( b_ptr,
 |                                                   h_ptr,
 |                                                   word_number,
 |                                                   mask,
 |                                                   ( value << field_ofs),
 |                                                   cmp,
 |                                                   max_count,
 |                                                   num_failed_polls,
 |                                                   delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-------------------------------------+
 | | Register 0x00000a68 DLOOMFI_INT_VAL |
 | +-------------------------------------+
 | | bit  0 R  DLOOMFI_V                 |
 | +-------------------------------------+
 | 
 | UINT32               odtu_dmx_field_DLOOMFI_V_get  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odtu_dmx_field_DLOOMFI_V_poll ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00000a6c OOM_IM_INT_VAL |
 | +------------------------------------+
 | | bit  0 R  OOM_IM_V                 |
 | +------------------------------------+
 | 
 | UINT32               odtu_dmx_field_OOM_IM_V_get  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odtu_dmx_field_OOM_IM_V_poll ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000a74 + (N) * 0x4) SSF_SF_RCVD_INT_VAL |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R  SSF_SF_RCVD_V                            |
 | +-------------------------------------------------------+
 | 
 | void   odtu_dmx_lfield_SSF_SF_RCVD_V_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 odtu_dmx_lfield_range_SSF_SF_RCVD_V_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_SSF_SF_RCVD_V_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE odtu_dmx_lfield_range_SSF_SF_RCVD_V_poll( odtu_dmx_buffer_t *b_ptr,
 | odtu_dmx_handle_t *h_ptr,
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
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "odtu_dmx_lfield_range_SSF_SF_RCVD_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "odtu_dmx_lfield_range_SSF_SF_RCVD_V_set_to_clear", start_bit, stop_bit);
 |     return odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_poll( b_ptr,
 |                                                         h_ptr,
 |                                                         word_number,
 |                                                         mask,
 |                                                         ( value << field_ofs),
 |                                                         cmp,
 |                                                         max_count,
 |                                                         num_failed_polls,
 |                                                         delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-----------------------------------------+
 | | Register 0x00000a80 INTERRUPT_SUMMARY_1 |
 | +-----------------------------------------+
 | | bit  31:0 R  INTR_SUMMARY_1             |
 | +-----------------------------------------+
 | 
 | UINT32               odtu_dmx_field_INTR_SUMMARY_1_get        ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | UINT32               odtu_dmx_field_range_INTR_SUMMARY_1_get  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE odtu_dmx_field_range_INTR_SUMMARY_1_poll ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE odtu_dmx_field_INTR_SUMMARY_1_poll       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x00000a84 INTERRUPT_SUMMARY_2 |
 | +-----------------------------------------+
 | | bit  6:0 R  INTR_SUMMARY_2              |
 | +-----------------------------------------+
 | 
 | UINT32               odtu_dmx_field_INTR_SUMMARY_2_get        ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | UINT32               odtu_dmx_field_range_INTR_SUMMARY_2_get  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE odtu_dmx_field_range_INTR_SUMMARY_2_poll ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE odtu_dmx_field_INTR_SUMMARY_2_poll       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------+
 | | Register 0x00000a88 CONFIG_ADDR_ST_CTRL_REG |
 | +---------------------------------------------+
 | | bit  14:8 R/W  LO_CONFIG_ADDR_ST_ID         |
 | | bit  6:0  R/W  HO_CONFIG_ADDR_ST_ID         |
 | +---------------------------------------------+
 | 
 | void   odtu_dmx_field_LO_CONFIG_ADDR_ST_ID_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | UINT32 odtu_dmx_field_LO_CONFIG_ADDR_ST_ID_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | void   odtu_dmx_field_range_LO_CONFIG_ADDR_ST_ID_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_field_range_LO_CONFIG_ADDR_ST_ID_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | UINT32 odtu_dmx_field_HO_CONFIG_ADDR_ST_ID_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | void   odtu_dmx_field_range_HO_CONFIG_ADDR_ST_ID_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_field_range_HO_CONFIG_ADDR_ST_ID_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000a8c CONFIG_ADDR_ST_REG |
 | +----------------------------------------+
 | | bit  8 R  LO_CONFIG_ADDR_ST            |
 | | bit  0 R  HO_CONFIG_ADDR_ST            |
 | +----------------------------------------+
 | 
 | UINT32 odtu_dmx_field_LO_CONFIG_ADDR_ST_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | UINT32 odtu_dmx_field_HO_CONFIG_ADDR_ST_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000a94 + (N) * 0x4) DUMMY_SCHDL_CTRL_REG |
 | |     N[0..3]                                            |
 | +--------------------------------------------------------+
 | | bit  22:16 R/W  DUMMY_SCHDL_ID                         |
 | | bit  14:8  R/W  DUMMY_SCHDL_CYCLE                      |
 | | bit  0     R/W  DUMMY_SCHDL_EN                         |
 | +--------------------------------------------------------+
 | 
 | void   odtu_dmx_field_DUMMY_SCHDL_ID_set          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odtu_dmx_field_DUMMY_SCHDL_ID_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 | void   odtu_dmx_field_range_DUMMY_SCHDL_ID_set    ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_field_range_DUMMY_SCHDL_ID_get    ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odtu_dmx_field_DUMMY_SCHDL_CYCLE_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odtu_dmx_field_DUMMY_SCHDL_CYCLE_get       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 | void   odtu_dmx_field_range_DUMMY_SCHDL_CYCLE_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odtu_dmx_field_range_DUMMY_SCHDL_CYCLE_get ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odtu_dmx_field_DUMMY_SCHDL_EN_set          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odtu_dmx_field_DUMMY_SCHDL_EN_get          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ODTU_DMX_IO_H
#define _ODTU_DMX_IO_H

#include "odtu_dmx_loc.h"
#include "odtu_dmx_regs.h"
#include "odtu_dmx_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 odtu_dmx_buffer_init               ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | void                 odtu_dmx_buffer_flush              ( odtu_dmx_buffer_t *b_ptr )
 | UINT32               odtu_dmx_reg_read                  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 odtu_dmx_reg_write                 ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 odtu_dmx_field_set                 ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_action_on_write_field_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_burst_read                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_burst_write               ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE odtu_dmx_poll                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 odtu_dmx_reg_FORCE_CONFIG_REG_write                                    ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | void                 odtu_dmx_reg_FORCE_CONFIG_REG_field_set                                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_dmx_reg_FORCE_CONFIG_REG_read                                     ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | 
 | void                 odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_write                             ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | void                 odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_field_set                         ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_dmx_reg_CONFIG_ADDR_ST_CTRL_REG_read                              ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | 
 | void                 odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_write                          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_field_set                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_dmx_reg_DUMMY_SCHDL_CTRL_REG_array_read                           ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odtu_dmx_reg_HO_CHANNEL_RESET_array_write                              ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odtu_dmx_reg_HO_CHANNEL_RESET_array_field_set                          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_dmx_reg_HO_CHANNEL_RESET_array_read                               ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odtu_dmx_reg_PASS_THROUGH_CFG_REG_array_write                          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odtu_dmx_reg_PASS_THROUGH_CFG_REG_array_field_set                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_dmx_reg_PASS_THROUGH_CFG_REG_array_read                           ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odtu_dmx_reg_NON_INTRUSIVE_MODE_array_write                            ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odtu_dmx_reg_NON_INTRUSIVE_MODE_array_field_set                        ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_dmx_reg_NON_INTRUSIVE_MODE_array_read                             ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odtu_dmx_reg_LO_CHANNEL_RESET_array_write                              ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odtu_dmx_reg_LO_CHANNEL_RESET_array_field_set                          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_dmx_reg_LO_CHANNEL_RESET_array_read                               ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odtu_dmx_reg_CONFIG_UPDATE_REG_array_write                             ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odtu_dmx_reg_CONFIG_UPDATE_REG_array_field_set                         ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_dmx_reg_CONFIG_UPDATE_REG_array_read                              ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_CONFIG_UPDATE_REG_array_poll                              ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_DLOOMFI_INT_EN_write                                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | void                 odtu_dmx_reg_DLOOMFI_INT_EN_field_set                                  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_dmx_reg_DLOOMFI_INT_EN_read                                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | 
 | void                 odtu_dmx_reg_OOM_OM_INT_EN_write                                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | void                 odtu_dmx_reg_OOM_OM_INT_EN_field_set                                   ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_dmx_reg_OOM_OM_INT_EN_read                                        ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | 
 | void                 odtu_dmx_reg_RCOH_STAT_TRANSFR_DONE_EN_write                           ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | void                 odtu_dmx_reg_RCOH_STAT_TRANSFR_DONE_EN_field_set                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_dmx_reg_RCOH_STAT_TRANSFR_DONE_EN_read                            ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | 
 | void                 odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_burst_write                    ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_field_set                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_burst_read                     ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_read                           ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_CONFIG_UPDATE_DNE_EN_array_poll                           ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_HAO_STAT_INT_EN_array_burst_write                         ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_HAO_STAT_INT_EN_array_field_set                           ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_HAO_STAT_INT_EN_array_burst_read                          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_HAO_STAT_INT_EN_array_read                                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odtu_dmx_reg_DPLM_INT_EN_array_burst_write                             ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_DPLM_INT_EN_array_field_set                               ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_DPLM_INT_EN_array_burst_read                              ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_DPLM_INT_EN_array_read                                    ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odtu_dmx_reg_DMSIM_INT_EN_array_burst_write                            ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_DMSIM_INT_EN_array_field_set                              ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_DMSIM_INT_EN_array_burst_read                             ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_DMSIM_INT_EN_array_read                                   ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_burst_write                     ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_field_set                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_burst_read                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_STS_SW_OVFLW_INT_EN_array_read                            ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_burst_write                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_field_set                        ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_burst_read                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_SSF_SF_RCVD_INT_EN_array_read                             ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_burst_write                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_field_set                        ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_burst_read                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_CM_OUT_OF_LIMIT_EN_array_read                             ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odtu_dmx_reg_CND_MAX_INT_EN_array_burst_write                          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_CND_MAX_INT_EN_array_field_set                            ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_CND_MAX_INT_EN_array_burst_read                           ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_CND_MAX_INT_EN_array_read                                 ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_burst_write                     ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_field_set                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_burst_read                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_JC_NOMTCH_LOSSYNC_E_array_read                            ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_burst_write                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_field_set                         ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_burst_read                        ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_CM_CND_CRC_ERR_EN_array_read                              ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_burst_write                     ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_field_set                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_burst_read                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_ERLY_PSERV_EXPRY_EN_array_read                            ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_burst_write                     ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_field_set                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_burst_read                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_LATE_PSERV_EXPRY_EN_array_read                            ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | 
 | UINT32               odtu_dmx_reg_INTERRUPT_SUMMARY_1_read                                  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_INTERRUPT_SUMMARY_1_poll                                  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               odtu_dmx_reg_INTERRUPT_SUMMARY_2_read                                  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_INTERRUPT_SUMMARY_2_poll                                  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_DLOOMFI_INT_write                                         ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | void                 odtu_dmx_reg_DLOOMFI_INT_action_on_write_field_set                     ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_dmx_reg_DLOOMFI_INT_read                                          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_DLOOMFI_INT_poll                                          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_OOM_IM_INT_write                                          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | void                 odtu_dmx_reg_OOM_IM_INT_action_on_write_field_set                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_dmx_reg_OOM_IM_INT_read                                           ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_OOM_IM_INT_poll                                           ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_write                         ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 value )
 | void                 odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_action_on_write_field_set     ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_read                          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_RCOH_STAT_TRANSFER_DONE_INT_poll                          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_burst_write                   ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_action_on_write_field_set     ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_burst_read                    ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_read                          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_CONFIG_UPDATE_DNE_INT_array_poll                          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_HAO_STAT_INT_array_burst_write                            ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_HAO_STAT_INT_array_action_on_write_field_set              ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_HAO_STAT_INT_array_burst_read                             ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_HAO_STAT_INT_array_read                                   ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_HAO_STAT_INT_array_poll                                   ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_DPLM_INT_array_burst_write                                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_DPLM_INT_array_action_on_write_field_set                  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_DPLM_INT_array_burst_read                                 ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_DPLM_INT_array_read                                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_DPLM_INT_array_poll                                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_DMSIM_INT_array_burst_write                               ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_DMSIM_INT_array_action_on_write_field_set                 ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_DMSIM_INT_array_burst_read                                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_DMSIM_INT_array_read                                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_DMSIM_INT_array_poll                                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_STS_SW_OVFLW_INT_array_burst_write                        ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_STS_SW_OVFLW_INT_array_action_on_write_field_set          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_STS_SW_OVFLW_INT_array_burst_read                         ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_STS_SW_OVFLW_INT_array_read                               ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_STS_SW_OVFLW_INT_array_poll                               ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_SSF_SF_RCVD_INT_array_burst_write                         ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_SSF_SF_RCVD_INT_array_action_on_write_field_set           ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_SSF_SF_RCVD_INT_array_burst_read                          ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_SSF_SF_RCVD_INT_array_read                                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_SSF_SF_RCVD_INT_array_poll                                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_burst_write                     ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_action_on_write_field_set       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_burst_read                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_read                            ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_CM_OUT_OF_LIMIT_INT_array_poll                            ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_burst_write               ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_action_on_write_field_set ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_burst_read                ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_read                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_CND_MAX_VAL_VIOLATION_INT_array_poll                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_burst_write                  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_action_on_write_field_set    ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_burst_read                   ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_read                         ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_JC_NOMATCH_LOSSYNC_INT_array_poll                         ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_burst_write                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_action_on_write_field_set        ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_burst_read                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_read                             ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_CM_CND_CRC_ERR_INT_array_poll                             ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_burst_write                    ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_action_on_write_field_set      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_burst_read                     ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_read                           ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_ERLY_PSERV_EXPRY_INT_array_poll                           ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_burst_write                    ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_action_on_write_field_set      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_burst_read                     ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_read                           ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_LATE_PSERV_EXPRY_INT_array_poll                           ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               odtu_dmx_reg_DLOOMFI_INT_VAL_read                                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_DLOOMFI_INT_VAL_poll                                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               odtu_dmx_reg_OOM_IM_INT_VAL_read                                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_OOM_IM_INT_VAL_poll                                       ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_DPLM_INT_VAL_array_burst_read                             ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_DPLM_INT_VAL_array_read                                   ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_DPLM_INT_VAL_array_poll                                   ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_DMSIM_INT_VAL_array_burst_read                            ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_DMSIM_INT_VAL_array_read                                  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_DMSIM_INT_VAL_array_poll                                  ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_burst_read                      ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_read                            ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE odtu_dmx_reg_SSF_SF_RCVD_INT_VAL_array_poll                            ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               odtu_dmx_reg_CONFIG_ADDR_ST_REG_read                                   ( odtu_dmx_buffer_t *b_ptr, odtu_dmx_handle_t *h_ptr )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ODTU_DMX_IO_H */
