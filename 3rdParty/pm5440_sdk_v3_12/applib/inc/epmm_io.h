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
 *     and register accessor functions for the epmm block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing epmm io functions is:
 | {
 |     epmm_buffer_t b_ptr[1];
 |     epmm_buffer_init( b_ptr, h_ptr );
 |     value = epmm_field_<FIELD>_get( b_ptr, h_ptr );
 |     epmm_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     epmm_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = epmm_field_<FIELD>_get( NULL, h_ptr );
 | epmm_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------------------------+
 | | Register 0x00000000 GLOBAL_SOFT_RESET_CONFIG_REG |
 | +--------------------------------------------------+
 | | bit  2 R/W  SW_RESET_100G                        |
 | | bit  1 R/W  SW_RESET_40G                         |
 | | bit  0 R/W  SW_RESET_10G                         |
 | +--------------------------------------------------+
 | 
 | void   epmm_field_SW_RESET_100G_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_SW_RESET_100G_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_SW_RESET_40G_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_SW_RESET_40G_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_SW_RESET_10G_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_SW_RESET_10G_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | +------------------------------------------------------+
 | | Register 0x00000014 GSUP43_PATH_UPI_TYPE_VAL_CFG_REG |
 | +------------------------------------------------------+
 | | bit  15:8 R/W  UPI_OS_TYPE                           |
 | | bit  7:0  R/W  UPI_PKT_TYPE                          |
 | +------------------------------------------------------+
 | 
 | void   epmm_field_UPI_OS_TYPE_set        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_UPI_OS_TYPE_get        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_range_UPI_OS_TYPE_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 epmm_field_range_UPI_OS_TYPE_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   epmm_field_UPI_PKT_TYPE_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_UPI_PKT_TYPE_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_range_UPI_PKT_TYPE_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 epmm_field_range_UPI_PKT_TYPE_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000024 + (N) * 0x04) ING_10G_LINK_CFG_REG |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  3:2 R/W  ING_10G_LINK_DP_SEL                       |
 | | bit  1   R/W  ING_10G_LINK_ENABLE                       |
 | | bit  0   R/W  ING_10G_LINK_RESET                        |
 | +---------------------------------------------------------+
 | 
 | void   epmm_field_ING_10G_LINK_DP_SEL_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_ING_10G_LINK_DP_SEL_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | void   epmm_field_range_ING_10G_LINK_DP_SEL_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 epmm_field_range_ING_10G_LINK_DP_SEL_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   epmm_field_ING_10G_LINK_ENABLE_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_ING_10G_LINK_ENABLE_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | void   epmm_field_ING_10G_LINK_RESET_set        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_ING_10G_LINK_RESET_get        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000054 + (N) * 0x04) ING_40G_LINK_CFG_REG |
 | |     N[0..2]                                             |
 | +---------------------------------------------------------+
 | | bit  1 R/W  ING_40G_LINK_ENABLE                         |
 | | bit  0 R/W  ING_40G_LINK_RESET                          |
 | +---------------------------------------------------------+
 | 
 | void   epmm_field_ING_40G_LINK_ENABLE_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_ING_40G_LINK_ENABLE_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | void   epmm_field_ING_40G_LINK_RESET_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_ING_40G_LINK_RESET_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------+
 | | Register 0x00000060 ING_100G_LINK_CFG_REG |
 | +-------------------------------------------+
 | | bit  1 R/W  ING_100G_LINK_ENABLE          |
 | | bit  0 R/W  ING_100G_LINK_RESET           |
 | +-------------------------------------------+
 | 
 | void   epmm_field_ING_100G_LINK_ENABLE_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_100G_LINK_ENABLE_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_100G_LINK_RESET_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_100G_LINK_RESET_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x00000064 + (N) * 0x4) ING_10G_LF_RF_OS_EN_REG |
 | |     N[0..11]                                              |
 | +-----------------------------------------------------------+
 | | bit  4 R/W  ING_CBR10G_OS_FORCE_EN                        |
 | | bit  3 R/W  ING_GSUP10G_FORCE_RF                          |
 | | bit  2 R/W  ING_GSUP10G_FORCE_LF                          |
 | | bit  1 R/W  ING_CBR10G_FORCE_RF                           |
 | | bit  0 R/W  ING_CBR10G_FORCE_LF                           |
 | +-----------------------------------------------------------+
 | 
 | void   epmm_field_ING_CBR10G_OS_FORCE_EN_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_ING_CBR10G_OS_FORCE_EN_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | void   epmm_field_ING_GSUP10G_FORCE_RF_set   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_ING_GSUP10G_FORCE_RF_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | void   epmm_field_ING_GSUP10G_FORCE_LF_set   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_ING_GSUP10G_FORCE_LF_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | void   epmm_field_ING_CBR10G_FORCE_RF_set    ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_ING_CBR10G_FORCE_RF_get    ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | void   epmm_field_ING_CBR10G_FORCE_LF_set    ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_ING_CBR10G_FORCE_LF_get    ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------+
 | | Register 0x00000094 ING_GSUP_OVR_INT_EN |
 | +-----------------------------------------+
 | | bit  11 R/W  ING_10G_GSUP11_FIFO_OVR_E  |
 | | bit  10 R/W  ING_10G_GSUP10_FIFO_OVR_E  |
 | | bit  9  R/W  ING_10G_GSUP9_FIFO_OVR_E   |
 | | bit  8  R/W  ING_10G_GSUP8_FIFO_OVR_E   |
 | | bit  7  R/W  ING_10G_GSUP7_FIFO_OVR_E   |
 | | bit  6  R/W  ING_10G_GSUP6_FIFO_OVR_E   |
 | | bit  5  R/W  ING_10G_GSUP5_FIFO_OVR_E   |
 | | bit  4  R/W  ING_10G_GSUP4_FIFO_OVR_E   |
 | | bit  3  R/W  ING_10G_GSUP3_FIFO_OVR_E   |
 | | bit  2  R/W  ING_10G_GSUP2_FIFO_OVR_E   |
 | | bit  1  R/W  ING_10G_GSUP1_FIFO_OVR_E   |
 | | bit  0  R/W  ING_10G_GSUP0_FIFO_OVR_E   |
 | +-----------------------------------------+
 | 
 | void   epmm_field_ING_10G_GSUP11_FIFO_OVR_E_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_GSUP11_FIFO_OVR_E_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_GSUP10_FIFO_OVR_E_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_GSUP10_FIFO_OVR_E_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_GSUP9_FIFO_OVR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_GSUP9_FIFO_OVR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_GSUP8_FIFO_OVR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_GSUP8_FIFO_OVR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_GSUP7_FIFO_OVR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_GSUP7_FIFO_OVR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_GSUP6_FIFO_OVR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_GSUP6_FIFO_OVR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_GSUP5_FIFO_OVR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_GSUP5_FIFO_OVR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_GSUP4_FIFO_OVR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_GSUP4_FIFO_OVR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_GSUP3_FIFO_OVR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_GSUP3_FIFO_OVR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_GSUP2_FIFO_OVR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_GSUP2_FIFO_OVR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_GSUP1_FIFO_OVR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_GSUP1_FIFO_OVR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_GSUP0_FIFO_OVR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_GSUP0_FIFO_OVR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x00000098 ING_GSUP_UDR_INT_EN |
 | +-----------------------------------------+
 | | bit  11:0 R/W  ING_10G_GSUP_FIFO_UDR_E  |
 | +-----------------------------------------+
 | 
 | void   epmm_field_ING_10G_GSUP_FIFO_UDR_E_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_GSUP_FIFO_UDR_E_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_range_ING_10G_GSUP_FIFO_UDR_E_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 epmm_field_range_ING_10G_GSUP_FIFO_UDR_E_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x000000a4 ING_GSUP_OVR_INT_STAT |
 | +-------------------------------------------+
 | | bit  11 R/W  ING_10G_GSUP11_FIFO_OVR_I    |
 | | bit  10 R/W  ING_10G_GSUP10_FIFO_OVR_I    |
 | | bit  9  R/W  ING_10G_GSUP9_FIFO_OVR_I     |
 | | bit  8  R/W  ING_10G_GSUP8_FIFO_OVR_I     |
 | | bit  7  R/W  ING_10G_GSUP7_FIFO_OVR_I     |
 | | bit  6  R/W  ING_10G_GSUP6_FIFO_OVR_I     |
 | | bit  5  R/W  ING_10G_GSUP5_FIFO_OVR_I     |
 | | bit  4  R/W  ING_10G_GSUP4_FIFO_OVR_I     |
 | | bit  3  R/W  ING_10G_GSUP3_FIFO_OVR_I     |
 | | bit  2  R/W  ING_10G_GSUP2_FIFO_OVR_I     |
 | | bit  1  R/W  ING_10G_GSUP1_FIFO_OVR_I     |
 | | bit  0  R/W  ING_10G_GSUP0_FIFO_OVR_I     |
 | +-------------------------------------------+
 | 
 | void                 epmm_field_ING_10G_GSUP11_FIFO_OVR_I_set_to_clear ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_GSUP11_FIFO_OVR_I_get          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP11_FIFO_OVR_I_poll         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_GSUP10_FIFO_OVR_I_set_to_clear ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_GSUP10_FIFO_OVR_I_get          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP10_FIFO_OVR_I_poll         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_GSUP9_FIFO_OVR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_GSUP9_FIFO_OVR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP9_FIFO_OVR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_GSUP8_FIFO_OVR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_GSUP8_FIFO_OVR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP8_FIFO_OVR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_GSUP7_FIFO_OVR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_GSUP7_FIFO_OVR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP7_FIFO_OVR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_GSUP6_FIFO_OVR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_GSUP6_FIFO_OVR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP6_FIFO_OVR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_GSUP5_FIFO_OVR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_GSUP5_FIFO_OVR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP5_FIFO_OVR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_GSUP4_FIFO_OVR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_GSUP4_FIFO_OVR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP4_FIFO_OVR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_GSUP3_FIFO_OVR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_GSUP3_FIFO_OVR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP3_FIFO_OVR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_GSUP2_FIFO_OVR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_GSUP2_FIFO_OVR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP2_FIFO_OVR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_GSUP1_FIFO_OVR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_GSUP1_FIFO_OVR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP1_FIFO_OVR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_GSUP0_FIFO_OVR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_GSUP0_FIFO_OVR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP0_FIFO_OVR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x000000a8 ING_GSUP_UDR_INT_STAT |
 | +-------------------------------------------+
 | | bit  11:0 R/W  ING_10G_GSUP_FIFO_UDR_I    |
 | +-------------------------------------------+
 | 
 | void                 epmm_field_ING_10G_GSUP_FIFO_UDR_I_set_to_clear       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_GSUP_FIFO_UDR_I_get                ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void                 epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_set_to_clear ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_get          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_ING_10G_GSUP_FIFO_UDR_I_poll         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP_FIFO_UDR_I_poll               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x000000b4 ING_GSUP_OVR_INT_VAL |
 | +------------------------------------------+
 | | bit  11 R  ING_10G_GSUP11_FIFO_OVR_V     |
 | | bit  10 R  ING_10G_GSUP10_FIFO_OVR_V     |
 | | bit  9  R  ING_10G_GSUP9_FIFO_OVR_V      |
 | | bit  8  R  ING_10G_GSUP8_FIFO_OVR_V      |
 | | bit  7  R  ING_10G_GSUP7_FIFO_OVR_V      |
 | | bit  6  R  ING_10G_GSUP6_FIFO_OVR_V      |
 | | bit  5  R  ING_10G_GSUP5_FIFO_OVR_V      |
 | | bit  4  R  ING_10G_GSUP4_FIFO_OVR_V      |
 | | bit  3  R  ING_10G_GSUP3_FIFO_OVR_V      |
 | | bit  2  R  ING_10G_GSUP2_FIFO_OVR_V      |
 | | bit  1  R  ING_10G_GSUP1_FIFO_OVR_V      |
 | | bit  0  R  ING_10G_GSUP0_FIFO_OVR_V      |
 | +------------------------------------------+
 | 
 | UINT32               epmm_field_ING_10G_GSUP11_FIFO_OVR_V_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP11_FIFO_OVR_V_poll ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_GSUP10_FIFO_OVR_V_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP10_FIFO_OVR_V_poll ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_GSUP9_FIFO_OVR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP9_FIFO_OVR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_GSUP8_FIFO_OVR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP8_FIFO_OVR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_GSUP7_FIFO_OVR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP7_FIFO_OVR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_GSUP6_FIFO_OVR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP6_FIFO_OVR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_GSUP5_FIFO_OVR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP5_FIFO_OVR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_GSUP4_FIFO_OVR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP4_FIFO_OVR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_GSUP3_FIFO_OVR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP3_FIFO_OVR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_GSUP2_FIFO_OVR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP2_FIFO_OVR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_GSUP1_FIFO_OVR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP1_FIFO_OVR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_GSUP0_FIFO_OVR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP0_FIFO_OVR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x000000b8 ING_GSUP_UDR_INT_VAL |
 | +------------------------------------------+
 | | bit  11:0 R  ING_10G_GSUP_FIFO_UDR_V     |
 | +------------------------------------------+
 | 
 | UINT32               epmm_field_ING_10G_GSUP_FIFO_UDR_V_get        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | UINT32               epmm_field_range_ING_10G_GSUP_FIFO_UDR_V_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_ING_10G_GSUP_FIFO_UDR_V_poll ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_GSUP_FIFO_UDR_V_poll       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x000000c4 ING_CBR_OVR_INT_EN   |
 | +------------------------------------------+
 | | bit  15    R/W  ING_100G_CBR_FIFO_OVR_E  |
 | | bit  14:12 R/W  ING_40G_CBR_FIFO_OVR_E   |
 | | bit  11    R/W  ING_10G_CBR11_FIFO_OVR_E |
 | | bit  10    R/W  ING_10G_CBR10_FIFO_OVR_E |
 | | bit  9     R/W  ING_10G_CBR9_FIFO_OVR_E  |
 | | bit  8     R/W  ING_10G_CBR8_FIFO_OVR_E  |
 | | bit  7     R/W  ING_10G_CBR7_FIFO_OVR_E  |
 | | bit  6     R/W  ING_10G_CBR6_FIFO_OVR_E  |
 | | bit  5     R/W  ING_10G_CBR5_FIFO_OVR_E  |
 | | bit  4     R/W  ING_10G_CBR4_FIFO_OVR_E  |
 | | bit  3     R/W  ING_10G_CBR3_FIFO_OVR_E  |
 | | bit  2     R/W  ING_10G_CBR2_FIFO_OVR_E  |
 | | bit  1     R/W  ING_10G_CBR1_FIFO_OVR_E  |
 | | bit  0     R/W  ING_10G_CBR0_FIFO_OVR_E  |
 | +------------------------------------------+
 | 
 | void   epmm_field_ING_100G_CBR_FIFO_OVR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_100G_CBR_FIFO_OVR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_40G_CBR_FIFO_OVR_E_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_40G_CBR_FIFO_OVR_E_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_range_ING_40G_CBR_FIFO_OVR_E_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 epmm_field_range_ING_40G_CBR_FIFO_OVR_E_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   epmm_field_ING_10G_CBR11_FIFO_OVR_E_set     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_CBR11_FIFO_OVR_E_get     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_CBR10_FIFO_OVR_E_set     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_CBR10_FIFO_OVR_E_get     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_CBR9_FIFO_OVR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_CBR9_FIFO_OVR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_CBR8_FIFO_OVR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_CBR8_FIFO_OVR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_CBR7_FIFO_OVR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_CBR7_FIFO_OVR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_CBR6_FIFO_OVR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_CBR6_FIFO_OVR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_CBR5_FIFO_OVR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_CBR5_FIFO_OVR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_CBR4_FIFO_OVR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_CBR4_FIFO_OVR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_CBR3_FIFO_OVR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_CBR3_FIFO_OVR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_CBR2_FIFO_OVR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_CBR2_FIFO_OVR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_CBR1_FIFO_OVR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_CBR1_FIFO_OVR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_10G_CBR0_FIFO_OVR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_CBR0_FIFO_OVR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x000000c8 ING_CBR_UDR_INT_EN  |
 | +-----------------------------------------+
 | | bit  15    R/W  ING_100G_CBR_FIFO_UDR_E |
 | | bit  14:12 R/W  ING_40G_CBR_FIFO_UDR_E  |
 | | bit  11:0  R/W  ING_10G_CBR_FIFO_UDR_E  |
 | +-----------------------------------------+
 | 
 | void   epmm_field_ING_100G_CBR_FIFO_UDR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_100G_CBR_FIFO_UDR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_ING_40G_CBR_FIFO_UDR_E_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_40G_CBR_FIFO_UDR_E_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_range_ING_40G_CBR_FIFO_UDR_E_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 epmm_field_range_ING_40G_CBR_FIFO_UDR_E_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   epmm_field_ING_10G_CBR_FIFO_UDR_E_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_ING_10G_CBR_FIFO_UDR_E_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_range_ING_10G_CBR_FIFO_UDR_E_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 epmm_field_range_ING_10G_CBR_FIFO_UDR_E_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x000000d4 ING_CBR_OVR_INT_STAT |
 | +------------------------------------------+
 | | bit  15    R/W  ING_100G_CBR_FIFO_OVR_I  |
 | | bit  14:12 R/W  ING_40G_CBR_FIFO_OVR_I   |
 | | bit  11    R/W  ING_10G_CBR11_FIFO_OVR_I |
 | | bit  10    R/W  ING_10G_CBR10_FIFO_OVR_I |
 | | bit  9     R/W  ING_10G_CBR9_FIFO_OVR_I  |
 | | bit  8     R/W  ING_10G_CBR8_FIFO_OVR_I  |
 | | bit  7     R/W  ING_10G_CBR7_FIFO_OVR_I  |
 | | bit  6     R/W  ING_10G_CBR6_FIFO_OVR_I  |
 | | bit  5     R/W  ING_10G_CBR5_FIFO_OVR_I  |
 | | bit  4     R/W  ING_10G_CBR4_FIFO_OVR_I  |
 | | bit  3     R/W  ING_10G_CBR3_FIFO_OVR_I  |
 | | bit  2     R/W  ING_10G_CBR2_FIFO_OVR_I  |
 | | bit  1     R/W  ING_10G_CBR1_FIFO_OVR_I  |
 | | bit  0     R/W  ING_10G_CBR0_FIFO_OVR_I  |
 | +------------------------------------------+
 | 
 | void                 epmm_field_ING_100G_CBR_FIFO_OVR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_100G_CBR_FIFO_OVR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_100G_CBR_FIFO_OVR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_40G_CBR_FIFO_OVR_I_set_to_clear       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_40G_CBR_FIFO_OVR_I_get                ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void                 epmm_field_range_ING_40G_CBR_FIFO_OVR_I_set_to_clear ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               epmm_field_range_ING_40G_CBR_FIFO_OVR_I_get          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_ING_40G_CBR_FIFO_OVR_I_poll         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_40G_CBR_FIFO_OVR_I_poll               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_CBR11_FIFO_OVR_I_set_to_clear     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_CBR11_FIFO_OVR_I_get              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR11_FIFO_OVR_I_poll             ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_CBR10_FIFO_OVR_I_set_to_clear     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_CBR10_FIFO_OVR_I_get              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR10_FIFO_OVR_I_poll             ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_CBR9_FIFO_OVR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_CBR9_FIFO_OVR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR9_FIFO_OVR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_CBR8_FIFO_OVR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_CBR8_FIFO_OVR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR8_FIFO_OVR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_CBR7_FIFO_OVR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_CBR7_FIFO_OVR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR7_FIFO_OVR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_CBR6_FIFO_OVR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_CBR6_FIFO_OVR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR6_FIFO_OVR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_CBR5_FIFO_OVR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_CBR5_FIFO_OVR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR5_FIFO_OVR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_CBR4_FIFO_OVR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_CBR4_FIFO_OVR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR4_FIFO_OVR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_CBR3_FIFO_OVR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_CBR3_FIFO_OVR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR3_FIFO_OVR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_CBR2_FIFO_OVR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_CBR2_FIFO_OVR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR2_FIFO_OVR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_CBR1_FIFO_OVR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_CBR1_FIFO_OVR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR1_FIFO_OVR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_CBR0_FIFO_OVR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_CBR0_FIFO_OVR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR0_FIFO_OVR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x000000d8 ING_CBR_UDR_INT_STAT |
 | +------------------------------------------+
 | | bit  15    R/W  ING_100G_CBR_FIFO_UDR_I  |
 | | bit  14:12 R/W  ING_40G_CBR_FIFO_UDR_I   |
 | | bit  11:0  R/W  ING_10G_CBR_FIFO_UDR_I   |
 | +------------------------------------------+
 | 
 | void                 epmm_field_ING_100G_CBR_FIFO_UDR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_100G_CBR_FIFO_UDR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_100G_CBR_FIFO_UDR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_40G_CBR_FIFO_UDR_I_set_to_clear       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_40G_CBR_FIFO_UDR_I_get                ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void                 epmm_field_range_ING_40G_CBR_FIFO_UDR_I_set_to_clear ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               epmm_field_range_ING_40G_CBR_FIFO_UDR_I_get          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_ING_40G_CBR_FIFO_UDR_I_poll         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_40G_CBR_FIFO_UDR_I_poll               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_ING_10G_CBR_FIFO_UDR_I_set_to_clear       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_ING_10G_CBR_FIFO_UDR_I_get                ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void                 epmm_field_range_ING_10G_CBR_FIFO_UDR_I_set_to_clear ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               epmm_field_range_ING_10G_CBR_FIFO_UDR_I_get          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_ING_10G_CBR_FIFO_UDR_I_poll         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR_FIFO_UDR_I_poll               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x000000e4 ING_CBR_OVR_INT_VAL |
 | +-----------------------------------------+
 | | bit  15    R  ING_100G_CBR_FIFO_OVR_V   |
 | | bit  14:12 R  ING_40G_CBR_FIFO_OVR_V    |
 | | bit  11    R  ING_10G_CBR11_FIFO_OVR_V  |
 | | bit  10    R  ING_10G_CBR10_FIFO_OVR_V  |
 | | bit  9     R  ING_10G_CBR9_FIFO_OVR_V   |
 | | bit  8     R  ING_10G_CBR8_FIFO_OVR_V   |
 | | bit  7     R  ING_10G_CBR7_FIFO_OVR_V   |
 | | bit  6     R  ING_10G_CBR6_FIFO_OVR_V   |
 | | bit  5     R  ING_10G_CBR5_FIFO_OVR_V   |
 | | bit  4     R  ING_10G_CBR4_FIFO_OVR_V   |
 | | bit  3     R  ING_10G_CBR3_FIFO_OVR_V   |
 | | bit  2     R  ING_10G_CBR2_FIFO_OVR_V   |
 | | bit  1     R  ING_10G_CBR1_FIFO_OVR_V   |
 | | bit  0     R  ING_10G_CBR0_FIFO_OVR_V   |
 | +-----------------------------------------+
 | 
 | UINT32               epmm_field_ING_100G_CBR_FIFO_OVR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_100G_CBR_FIFO_OVR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_40G_CBR_FIFO_OVR_V_get        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | UINT32               epmm_field_range_ING_40G_CBR_FIFO_OVR_V_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_ING_40G_CBR_FIFO_OVR_V_poll ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_40G_CBR_FIFO_OVR_V_poll       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_CBR11_FIFO_OVR_V_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR11_FIFO_OVR_V_poll     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_CBR10_FIFO_OVR_V_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR10_FIFO_OVR_V_poll     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_CBR9_FIFO_OVR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR9_FIFO_OVR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_CBR8_FIFO_OVR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR8_FIFO_OVR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_CBR7_FIFO_OVR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR7_FIFO_OVR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_CBR6_FIFO_OVR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR6_FIFO_OVR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_CBR5_FIFO_OVR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR5_FIFO_OVR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_CBR4_FIFO_OVR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR4_FIFO_OVR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_CBR3_FIFO_OVR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR3_FIFO_OVR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_CBR2_FIFO_OVR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR2_FIFO_OVR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_CBR1_FIFO_OVR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR1_FIFO_OVR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_CBR0_FIFO_OVR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR0_FIFO_OVR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x000000e8 ING_CBR_UDR_INT_VAL |
 | +-----------------------------------------+
 | | bit  15    R  ING_100G_CBR_FIFO_UDR_V   |
 | | bit  14:12 R  ING_40G_CBR_FIFO_UDR_V    |
 | | bit  11:0  R  ING_10G_CBR_FIFO_UDR_V    |
 | +-----------------------------------------+
 | 
 | UINT32               epmm_field_ING_100G_CBR_FIFO_UDR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_100G_CBR_FIFO_UDR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_40G_CBR_FIFO_UDR_V_get        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | UINT32               epmm_field_range_ING_40G_CBR_FIFO_UDR_V_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_ING_40G_CBR_FIFO_UDR_V_poll ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_40G_CBR_FIFO_UDR_V_poll       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_ING_10G_CBR_FIFO_UDR_V_get        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | UINT32               epmm_field_range_ING_10G_CBR_FIFO_UDR_V_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_ING_10G_CBR_FIFO_UDR_V_poll ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_ING_10G_CBR_FIFO_UDR_V_poll       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000000f4 + (N) * 0x04) EGR_10G_LINK_CFG_REG |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  3:2 R/W  EGR_10G_LINK_DP_SEL                       |
 | | bit  1   R/W  EGR_10G_LINK_ENABLE                       |
 | | bit  0   R/W  EGR_10G_LINK_RESET                        |
 | +---------------------------------------------------------+
 | 
 | void   epmm_field_EGR_10G_LINK_DP_SEL_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_EGR_10G_LINK_DP_SEL_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | void   epmm_field_range_EGR_10G_LINK_DP_SEL_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 epmm_field_range_EGR_10G_LINK_DP_SEL_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   epmm_field_EGR_10G_LINK_ENABLE_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_EGR_10G_LINK_ENABLE_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | void   epmm_field_EGR_10G_LINK_RESET_set        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_EGR_10G_LINK_RESET_get        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000124 + (N) * 0x04) EGR_40G_LINK_CFG_REG |
 | |     N[0..2]                                             |
 | +---------------------------------------------------------+
 | | bit  1 R/W  EGR_40G_LINK_ENABLE                         |
 | | bit  0 R/W  EGR_40G_LINK_RESET                          |
 | +---------------------------------------------------------+
 | 
 | void   epmm_field_EGR_40G_LINK_ENABLE_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_EGR_40G_LINK_ENABLE_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | void   epmm_field_EGR_40G_LINK_RESET_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_EGR_40G_LINK_RESET_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------+
 | | Register 0x00000130 EGR_100G_LINK_CFG_REG |
 | +-------------------------------------------+
 | | bit  1 R/W  EGR_100G_LINK_ENABLE          |
 | | bit  0 R/W  EGR_100G_LINK_RESET           |
 | +-------------------------------------------+
 | 
 | void   epmm_field_EGR_100G_LINK_ENABLE_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_100G_LINK_ENABLE_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_100G_LINK_RESET_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_100G_LINK_RESET_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000134 + (N) * 0x4) EGR_LF_RF_OS_EN_REG |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  5   R/W  EGR_CBR10G_OS_FORCE_EN                  |
 | | bit  4:3 R/W  EGR_GSUP_TX_SW_FORCE                    |
 | | bit  2   R/W  EGR_GSUP_CNSQ_ACT_EN                    |
 | | bit  1   R/W  EGR_CBR10G_FORCE_RF                     |
 | | bit  0   R/W  EGR_CBR10G_FORCE_LF                     |
 | +-------------------------------------------------------+
 | 
 | void   epmm_field_EGR_CBR10G_OS_FORCE_EN_set     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_EGR_CBR10G_OS_FORCE_EN_get     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | void   epmm_field_EGR_GSUP_TX_SW_FORCE_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_EGR_GSUP_TX_SW_FORCE_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | void   epmm_field_range_EGR_GSUP_TX_SW_FORCE_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 epmm_field_range_EGR_GSUP_TX_SW_FORCE_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   epmm_field_EGR_GSUP_CNSQ_ACT_EN_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_EGR_GSUP_CNSQ_ACT_EN_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | void   epmm_field_EGR_CBR10G_FORCE_RF_set        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_EGR_CBR10G_FORCE_RF_get        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | void   epmm_field_EGR_CBR10G_FORCE_LF_set        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 epmm_field_EGR_CBR10G_FORCE_LF_get        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------+
 | | Register 0x00000164 EGR_GSUP_OVR_INT_EN |
 | +-----------------------------------------+
 | | bit  11:0 R/W  EGR_10G_GSUP_FIFO_OVR_E  |
 | +-----------------------------------------+
 | 
 | void   epmm_field_EGR_10G_GSUP_FIFO_OVR_E_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_GSUP_FIFO_OVR_E_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_range_EGR_10G_GSUP_FIFO_OVR_E_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 epmm_field_range_EGR_10G_GSUP_FIFO_OVR_E_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000168 EGR_GSUP_UDR_INT_EN |
 | +-----------------------------------------+
 | | bit  11 R/W  EGR_10G_GSUP11_FIFO_UDR_E  |
 | | bit  10 R/W  EGR_10G_GSUP10_FIFO_UDR_E  |
 | | bit  9  R/W  EGR_10G_GSUP9_FIFO_UDR_E   |
 | | bit  8  R/W  EGR_10G_GSUP8_FIFO_UDR_E   |
 | | bit  7  R/W  EGR_10G_GSUP7_FIFO_UDR_E   |
 | | bit  6  R/W  EGR_10G_GSUP6_FIFO_UDR_E   |
 | | bit  5  R/W  EGR_10G_GSUP5_FIFO_UDR_E   |
 | | bit  4  R/W  EGR_10G_GSUP4_FIFO_UDR_E   |
 | | bit  3  R/W  EGR_10G_GSUP3_FIFO_UDR_E   |
 | | bit  2  R/W  EGR_10G_GSUP2_FIFO_UDR_E   |
 | | bit  1  R/W  EGR_10G_GSUP1_FIFO_UDR_E   |
 | | bit  0  R/W  EGR_10G_GSUP0_FIFO_UDR_E   |
 | +-----------------------------------------+
 | 
 | void   epmm_field_EGR_10G_GSUP11_FIFO_UDR_E_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_GSUP11_FIFO_UDR_E_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_GSUP10_FIFO_UDR_E_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_GSUP10_FIFO_UDR_E_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_GSUP9_FIFO_UDR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_GSUP9_FIFO_UDR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_GSUP8_FIFO_UDR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_GSUP8_FIFO_UDR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_GSUP7_FIFO_UDR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_GSUP7_FIFO_UDR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_GSUP6_FIFO_UDR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_GSUP6_FIFO_UDR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_GSUP5_FIFO_UDR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_GSUP5_FIFO_UDR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_GSUP4_FIFO_UDR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_GSUP4_FIFO_UDR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_GSUP3_FIFO_UDR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_GSUP3_FIFO_UDR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_GSUP2_FIFO_UDR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_GSUP2_FIFO_UDR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_GSUP1_FIFO_UDR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_GSUP1_FIFO_UDR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_GSUP0_FIFO_UDR_E_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_GSUP0_FIFO_UDR_E_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00000174 EGR_GSUP_OVR_INT_STAT |
 | +-------------------------------------------+
 | | bit  11:0 R/W  EGR_10G_GSUP_FIFO_OVR_I    |
 | +-------------------------------------------+
 | 
 | void                 epmm_field_EGR_10G_GSUP_FIFO_OVR_I_set_to_clear       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_GSUP_FIFO_OVR_I_get                ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void                 epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_set_to_clear ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_get          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_EGR_10G_GSUP_FIFO_OVR_I_poll         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP_FIFO_OVR_I_poll               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x00000178 EGR_GSUP_UDR_INT_STAT |
 | +-------------------------------------------+
 | | bit  11 R/W  EGR_10G_GSUP11_FIFO_UDR_I    |
 | | bit  10 R/W  EGR_10G_GSUP10_FIFO_UDR_I    |
 | | bit  9  R/W  EGR_10G_GSUP9_FIFO_UDR_I     |
 | | bit  8  R/W  EGR_10G_GSUP8_FIFO_UDR_I     |
 | | bit  7  R/W  EGR_10G_GSUP7_FIFO_UDR_I     |
 | | bit  6  R/W  EGR_10G_GSUP6_FIFO_UDR_I     |
 | | bit  5  R/W  EGR_10G_GSUP5_FIFO_UDR_I     |
 | | bit  4  R/W  EGR_10G_GSUP4_FIFO_UDR_I     |
 | | bit  3  R/W  EGR_10G_GSUP3_FIFO_UDR_I     |
 | | bit  2  R/W  EGR_10G_GSUP2_FIFO_UDR_I     |
 | | bit  1  R/W  EGR_10G_GSUP1_FIFO_UDR_I     |
 | | bit  0  R/W  EGR_10G_GSUP0_FIFO_UDR_I     |
 | +-------------------------------------------+
 | 
 | void                 epmm_field_EGR_10G_GSUP11_FIFO_UDR_I_set_to_clear ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_GSUP11_FIFO_UDR_I_get          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP11_FIFO_UDR_I_poll         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_GSUP10_FIFO_UDR_I_set_to_clear ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_GSUP10_FIFO_UDR_I_get          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP10_FIFO_UDR_I_poll         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_GSUP9_FIFO_UDR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_GSUP9_FIFO_UDR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP9_FIFO_UDR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_GSUP8_FIFO_UDR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_GSUP8_FIFO_UDR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP8_FIFO_UDR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_GSUP7_FIFO_UDR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_GSUP7_FIFO_UDR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP7_FIFO_UDR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_GSUP6_FIFO_UDR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_GSUP6_FIFO_UDR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP6_FIFO_UDR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_GSUP5_FIFO_UDR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_GSUP5_FIFO_UDR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP5_FIFO_UDR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_GSUP4_FIFO_UDR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_GSUP4_FIFO_UDR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP4_FIFO_UDR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_GSUP3_FIFO_UDR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_GSUP3_FIFO_UDR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP3_FIFO_UDR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_GSUP2_FIFO_UDR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_GSUP2_FIFO_UDR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP2_FIFO_UDR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_GSUP1_FIFO_UDR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_GSUP1_FIFO_UDR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP1_FIFO_UDR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_GSUP0_FIFO_UDR_I_set_to_clear  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_GSUP0_FIFO_UDR_I_get           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP0_FIFO_UDR_I_poll          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x00000184 EGR_GSUP_OVR_INT_VAL |
 | +------------------------------------------+
 | | bit  11:0 R  EGR_10G_GSUP_FIFO_OVR_V     |
 | +------------------------------------------+
 | 
 | UINT32               epmm_field_EGR_10G_GSUP_FIFO_OVR_V_get        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | UINT32               epmm_field_range_EGR_10G_GSUP_FIFO_OVR_V_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_EGR_10G_GSUP_FIFO_OVR_V_poll ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP_FIFO_OVR_V_poll       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x00000188 EGR_GSUP_UDR_INT_VAL |
 | +------------------------------------------+
 | | bit  11 R  EGR_10G_GSUP11_FIFO_UDR_V     |
 | | bit  10 R  EGR_10G_GSUP10_FIFO_UDR_V     |
 | | bit  9  R  EGR_10G_GSUP9_FIFO_UDR_V      |
 | | bit  8  R  EGR_10G_GSUP8_FIFO_UDR_V      |
 | | bit  7  R  EGR_10G_GSUP7_FIFO_UDR_V      |
 | | bit  6  R  EGR_10G_GSUP6_FIFO_UDR_V      |
 | | bit  5  R  EGR_10G_GSUP5_FIFO_UDR_V      |
 | | bit  4  R  EGR_10G_GSUP4_FIFO_UDR_V      |
 | | bit  3  R  EGR_10G_GSUP3_FIFO_UDR_V      |
 | | bit  2  R  EGR_10G_GSUP2_FIFO_UDR_V      |
 | | bit  1  R  EGR_10G_GSUP1_FIFO_UDR_V      |
 | | bit  0  R  EGR_10G_GSUP0_FIFO_UDR_V      |
 | +------------------------------------------+
 | 
 | UINT32               epmm_field_EGR_10G_GSUP11_FIFO_UDR_V_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP11_FIFO_UDR_V_poll ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_GSUP10_FIFO_UDR_V_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP10_FIFO_UDR_V_poll ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_GSUP9_FIFO_UDR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP9_FIFO_UDR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_GSUP8_FIFO_UDR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP8_FIFO_UDR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_GSUP7_FIFO_UDR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP7_FIFO_UDR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_GSUP6_FIFO_UDR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP6_FIFO_UDR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_GSUP5_FIFO_UDR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP5_FIFO_UDR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_GSUP4_FIFO_UDR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP4_FIFO_UDR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_GSUP3_FIFO_UDR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP3_FIFO_UDR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_GSUP2_FIFO_UDR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP2_FIFO_UDR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_GSUP1_FIFO_UDR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP1_FIFO_UDR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_GSUP0_FIFO_UDR_V_get   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_GSUP0_FIFO_UDR_V_poll  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x00000194 EGR_CBR_OVR_INT_EN  |
 | +-----------------------------------------+
 | | bit  15    R/W  EGR_100G_CBR_FIFO_OVR_E |
 | | bit  14:12 R/W  EGR_40G_CBR_FIFO_OVR_E  |
 | | bit  11:0  R/W  EGR_10G_CBR_FIFO_OVR_E  |
 | +-----------------------------------------+
 | 
 | void   epmm_field_EGR_100G_CBR_FIFO_OVR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_100G_CBR_FIFO_OVR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_40G_CBR_FIFO_OVR_E_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_40G_CBR_FIFO_OVR_E_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_range_EGR_40G_CBR_FIFO_OVR_E_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 epmm_field_range_EGR_40G_CBR_FIFO_OVR_E_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   epmm_field_EGR_10G_CBR_FIFO_OVR_E_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_CBR_FIFO_OVR_E_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_range_EGR_10G_CBR_FIFO_OVR_E_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 epmm_field_range_EGR_10G_CBR_FIFO_OVR_E_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000198 EGR_CBR_UDR_INT_EN   |
 | +------------------------------------------+
 | | bit  15    R/W  EGR_100G_CBR_FIFO_UDR_E  |
 | | bit  14:12 R/W  EGR_40G_CBR_FIFO_UDR_E   |
 | | bit  11    R/W  EGR_10G_CBR11_FIFO_UDR_E |
 | | bit  10    R/W  EGR_10G_CBR10_FIFO_UDR_E |
 | | bit  9     R/W  EGR_10G_CBR9_FIFO_UDR_E  |
 | | bit  8     R/W  EGR_10G_CBR8_FIFO_UDR_E  |
 | | bit  7     R/W  EGR_10G_CBR7_FIFO_UDR_E  |
 | | bit  6     R/W  EGR_10G_CBR6_FIFO_UDR_E  |
 | | bit  5     R/W  EGR_10G_CBR5_FIFO_UDR_E  |
 | | bit  4     R/W  EGR_10G_CBR4_FIFO_UDR_E  |
 | | bit  3     R/W  EGR_10G_CBR3_FIFO_UDR_E  |
 | | bit  2     R/W  EGR_10G_CBR2_FIFO_UDR_E  |
 | | bit  1     R/W  EGR_10G_CBR1_FIFO_UDR_E  |
 | | bit  0     R/W  EGR_10G_CBR0_FIFO_UDR_E  |
 | +------------------------------------------+
 | 
 | void   epmm_field_EGR_100G_CBR_FIFO_UDR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_100G_CBR_FIFO_UDR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_40G_CBR_FIFO_UDR_E_set       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_40G_CBR_FIFO_UDR_E_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_range_EGR_40G_CBR_FIFO_UDR_E_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 epmm_field_range_EGR_40G_CBR_FIFO_UDR_E_get ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   epmm_field_EGR_10G_CBR11_FIFO_UDR_E_set     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_CBR11_FIFO_UDR_E_get     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_CBR10_FIFO_UDR_E_set     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_CBR10_FIFO_UDR_E_get     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_CBR9_FIFO_UDR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_CBR9_FIFO_UDR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_CBR8_FIFO_UDR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_CBR8_FIFO_UDR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_CBR7_FIFO_UDR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_CBR7_FIFO_UDR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_CBR6_FIFO_UDR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_CBR6_FIFO_UDR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_CBR5_FIFO_UDR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_CBR5_FIFO_UDR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_CBR4_FIFO_UDR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_CBR4_FIFO_UDR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_CBR3_FIFO_UDR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_CBR3_FIFO_UDR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_CBR2_FIFO_UDR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_CBR2_FIFO_UDR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_CBR1_FIFO_UDR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_CBR1_FIFO_UDR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void   epmm_field_EGR_10G_CBR0_FIFO_UDR_E_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32 epmm_field_EGR_10G_CBR0_FIFO_UDR_E_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x000001a4 EGR_CBR_OVR_INT_STAT |
 | +------------------------------------------+
 | | bit  15    R/W  EGR_100G_CBR_FIFO_OVR_I  |
 | | bit  14:12 R/W  EGR_40G_CBR_FIFO_OVR_I   |
 | | bit  11:0  R/W  EGR_10G_CBR_FIFO_OVR_I   |
 | +------------------------------------------+
 | 
 | void                 epmm_field_EGR_100G_CBR_FIFO_OVR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_100G_CBR_FIFO_OVR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_100G_CBR_FIFO_OVR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_40G_CBR_FIFO_OVR_I_set_to_clear       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_40G_CBR_FIFO_OVR_I_get                ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void                 epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_set_to_clear ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_get          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_EGR_40G_CBR_FIFO_OVR_I_poll         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_40G_CBR_FIFO_OVR_I_poll               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_CBR_FIFO_OVR_I_set_to_clear       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_CBR_FIFO_OVR_I_get                ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void                 epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_set_to_clear ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_get          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_EGR_10G_CBR_FIFO_OVR_I_poll         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR_FIFO_OVR_I_poll               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x000001a8 EGR_CBR_UDR_INT_STAT |
 | +------------------------------------------+
 | | bit  15    R/W  EGR_100G_CBR_FIFO_UDR_I  |
 | | bit  14:12 R/W  EGR_40G_CBR_FIFO_UDR_I   |
 | | bit  11    R/W  EGR_10G_CBR11_FIFO_UDR_I |
 | | bit  10    R/W  EGR_10G_CBR10_FIFO_UDR_I |
 | | bit  9     R/W  EGR_10G_CBR9_FIFO_UDR_I  |
 | | bit  8     R/W  EGR_10G_CBR8_FIFO_UDR_I  |
 | | bit  7     R/W  EGR_10G_CBR7_FIFO_UDR_I  |
 | | bit  6     R/W  EGR_10G_CBR6_FIFO_UDR_I  |
 | | bit  5     R/W  EGR_10G_CBR5_FIFO_UDR_I  |
 | | bit  4     R/W  EGR_10G_CBR4_FIFO_UDR_I  |
 | | bit  3     R/W  EGR_10G_CBR3_FIFO_UDR_I  |
 | | bit  2     R/W  EGR_10G_CBR2_FIFO_UDR_I  |
 | | bit  1     R/W  EGR_10G_CBR1_FIFO_UDR_I  |
 | | bit  0     R/W  EGR_10G_CBR0_FIFO_UDR_I  |
 | +------------------------------------------+
 | 
 | void                 epmm_field_EGR_100G_CBR_FIFO_UDR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_100G_CBR_FIFO_UDR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_100G_CBR_FIFO_UDR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_40G_CBR_FIFO_UDR_I_set_to_clear       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_40G_CBR_FIFO_UDR_I_get                ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void                 epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_set_to_clear ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_get          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_EGR_40G_CBR_FIFO_UDR_I_poll         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_40G_CBR_FIFO_UDR_I_poll               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_CBR11_FIFO_UDR_I_set_to_clear     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_CBR11_FIFO_UDR_I_get              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR11_FIFO_UDR_I_poll             ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_CBR10_FIFO_UDR_I_set_to_clear     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_CBR10_FIFO_UDR_I_get              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR10_FIFO_UDR_I_poll             ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_CBR9_FIFO_UDR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_CBR9_FIFO_UDR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR9_FIFO_UDR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_CBR8_FIFO_UDR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_CBR8_FIFO_UDR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR8_FIFO_UDR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_CBR7_FIFO_UDR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_CBR7_FIFO_UDR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR7_FIFO_UDR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_CBR6_FIFO_UDR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_CBR6_FIFO_UDR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR6_FIFO_UDR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_CBR5_FIFO_UDR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_CBR5_FIFO_UDR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR5_FIFO_UDR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_CBR4_FIFO_UDR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_CBR4_FIFO_UDR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR4_FIFO_UDR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_CBR3_FIFO_UDR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_CBR3_FIFO_UDR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR3_FIFO_UDR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_CBR2_FIFO_UDR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_CBR2_FIFO_UDR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR2_FIFO_UDR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_CBR1_FIFO_UDR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_CBR1_FIFO_UDR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR1_FIFO_UDR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 epmm_field_EGR_10G_CBR0_FIFO_UDR_I_set_to_clear      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | UINT32               epmm_field_EGR_10G_CBR0_FIFO_UDR_I_get               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR0_FIFO_UDR_I_poll              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x000001b4 EGR_CBR_OVR_INT_VAL |
 | +-----------------------------------------+
 | | bit  15    R  EGR_100G_CBR_FIFO_OVR_V   |
 | | bit  14:12 R  EGR_40G_CBR_FIFO_OVR_V    |
 | | bit  11:0  R  EGR_10G_CBR_FIFO_OVR_V    |
 | +-----------------------------------------+
 | 
 | UINT32               epmm_field_EGR_100G_CBR_FIFO_OVR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_100G_CBR_FIFO_OVR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_40G_CBR_FIFO_OVR_V_get        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | UINT32               epmm_field_range_EGR_40G_CBR_FIFO_OVR_V_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_EGR_40G_CBR_FIFO_OVR_V_poll ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_40G_CBR_FIFO_OVR_V_poll       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_CBR_FIFO_OVR_V_get        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | UINT32               epmm_field_range_EGR_10G_CBR_FIFO_OVR_V_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_EGR_10G_CBR_FIFO_OVR_V_poll ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR_FIFO_OVR_V_poll       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x000001b8 EGR_CBR_UDR_INT_VAL |
 | +-----------------------------------------+
 | | bit  15    R  EGR_100G_CBR_FIFO_UDR_V   |
 | | bit  14:12 R  EGR_40G_CBR_FIFO_UDR_V    |
 | | bit  11    R  EGR_10G_CBR11_FIFO_UDR_V  |
 | | bit  10    R  EGR_10G_CBR10_FIFO_UDR_V  |
 | | bit  9     R  EGR_10G_CBR9_FIFO_UDR_V   |
 | | bit  8     R  EGR_10G_CBR8_FIFO_UDR_V   |
 | | bit  7     R  EGR_10G_CBR7_FIFO_UDR_V   |
 | | bit  6     R  EGR_10G_CBR6_FIFO_UDR_V   |
 | | bit  5     R  EGR_10G_CBR5_FIFO_UDR_V   |
 | | bit  4     R  EGR_10G_CBR4_FIFO_UDR_V   |
 | | bit  3     R  EGR_10G_CBR3_FIFO_UDR_V   |
 | | bit  2     R  EGR_10G_CBR2_FIFO_UDR_V   |
 | | bit  1     R  EGR_10G_CBR1_FIFO_UDR_V   |
 | | bit  0     R  EGR_10G_CBR0_FIFO_UDR_V   |
 | +-----------------------------------------+
 | 
 | UINT32               epmm_field_EGR_100G_CBR_FIFO_UDR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_100G_CBR_FIFO_UDR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_40G_CBR_FIFO_UDR_V_get        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | UINT32               epmm_field_range_EGR_40G_CBR_FIFO_UDR_V_get  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE epmm_field_range_EGR_40G_CBR_FIFO_UDR_V_poll ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_40G_CBR_FIFO_UDR_V_poll       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_CBR11_FIFO_UDR_V_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR11_FIFO_UDR_V_poll     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_CBR10_FIFO_UDR_V_get      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR10_FIFO_UDR_V_poll     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_CBR9_FIFO_UDR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR9_FIFO_UDR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_CBR8_FIFO_UDR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR8_FIFO_UDR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_CBR7_FIFO_UDR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR7_FIFO_UDR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_CBR6_FIFO_UDR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR6_FIFO_UDR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_CBR5_FIFO_UDR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR5_FIFO_UDR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_CBR4_FIFO_UDR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR4_FIFO_UDR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_CBR3_FIFO_UDR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR3_FIFO_UDR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_CBR2_FIFO_UDR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR2_FIFO_UDR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_CBR1_FIFO_UDR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR1_FIFO_UDR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               epmm_field_EGR_10G_CBR0_FIFO_UDR_V_get       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_field_EGR_10G_CBR0_FIFO_UDR_V_poll      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _EPMM_IO_H
#define _EPMM_IO_H

#include "epmm_loc.h"
#include "epmm_regs.h"
#include "epmm_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 epmm_buffer_init               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | void                 epmm_buffer_flush              ( epmm_buffer_t *b_ptr )
 | UINT32               epmm_reg_read                  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 epmm_reg_write                 ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 epmm_field_set                 ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 epmm_action_on_write_field_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 epmm_burst_read                ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 epmm_burst_write               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE epmm_poll                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_write              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_field_set          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_GLOBAL_SOFT_RESET_CONFIG_REG_read               ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | void                 epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_write          ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_field_set      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_GSUP43_PATH_UPI_TYPE_VAL_CFG_REG_read           ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | void                 epmm_reg_ING_100G_LINK_CFG_REG_write                     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_ING_100G_LINK_CFG_REG_field_set                 ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_ING_100G_LINK_CFG_REG_read                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | void                 epmm_reg_EGR_100G_LINK_CFG_REG_write                     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_EGR_100G_LINK_CFG_REG_field_set                 ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_EGR_100G_LINK_CFG_REG_read                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | void                 epmm_reg_ING_10G_LINK_CFG_REG_array_write                ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 epmm_reg_ING_10G_LINK_CFG_REG_array_field_set            ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_ING_10G_LINK_CFG_REG_array_read                 ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_write             ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_field_set         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_ING_10G_LF_RF_OS_EN_REG_array_read              ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 epmm_reg_EGR_10G_LINK_CFG_REG_array_write                ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 epmm_reg_EGR_10G_LINK_CFG_REG_array_field_set            ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_EGR_10G_LINK_CFG_REG_array_read                 ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 epmm_reg_EGR_LF_RF_OS_EN_REG_array_write                 ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 epmm_reg_EGR_LF_RF_OS_EN_REG_array_field_set             ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_EGR_LF_RF_OS_EN_REG_array_read                  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 epmm_reg_ING_40G_LINK_CFG_REG_array_write                ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 epmm_reg_ING_40G_LINK_CFG_REG_array_field_set            ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_ING_40G_LINK_CFG_REG_array_read                 ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 epmm_reg_EGR_40G_LINK_CFG_REG_array_write                ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 epmm_reg_EGR_40G_LINK_CFG_REG_array_field_set            ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_EGR_40G_LINK_CFG_REG_array_read                 ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 epmm_reg_ING_GSUP_OVR_INT_EN_write                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_ING_GSUP_OVR_INT_EN_field_set                   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_ING_GSUP_OVR_INT_EN_read                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | void                 epmm_reg_ING_GSUP_UDR_INT_EN_write                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_ING_GSUP_UDR_INT_EN_field_set                   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_ING_GSUP_UDR_INT_EN_read                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | void                 epmm_reg_ING_CBR_OVR_INT_EN_write                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_ING_CBR_OVR_INT_EN_field_set                    ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_ING_CBR_OVR_INT_EN_read                         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | void                 epmm_reg_ING_CBR_UDR_INT_EN_write                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_ING_CBR_UDR_INT_EN_field_set                    ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_ING_CBR_UDR_INT_EN_read                         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | void                 epmm_reg_EGR_GSUP_OVR_INT_EN_write                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_EGR_GSUP_OVR_INT_EN_field_set                   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_EGR_GSUP_OVR_INT_EN_read                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | void                 epmm_reg_EGR_GSUP_UDR_INT_EN_write                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_EGR_GSUP_UDR_INT_EN_field_set                   ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_EGR_GSUP_UDR_INT_EN_read                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | void                 epmm_reg_EGR_CBR_OVR_INT_EN_write                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_EGR_CBR_OVR_INT_EN_field_set                    ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_EGR_CBR_OVR_INT_EN_read                         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | void                 epmm_reg_EGR_CBR_UDR_INT_EN_write                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_EGR_CBR_UDR_INT_EN_field_set                    ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_EGR_CBR_UDR_INT_EN_read                         ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | 
 | void                 epmm_reg_ING_GSUP_OVR_INT_STAT_write                     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_ING_GSUP_OVR_INT_STAT_action_on_write_field_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_ING_GSUP_OVR_INT_STAT_read                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_ING_GSUP_OVR_INT_STAT_poll                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 epmm_reg_ING_GSUP_UDR_INT_STAT_write                     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_ING_GSUP_UDR_INT_STAT_action_on_write_field_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_ING_GSUP_UDR_INT_STAT_read                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_ING_GSUP_UDR_INT_STAT_poll                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 epmm_reg_ING_CBR_OVR_INT_STAT_write                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_ING_CBR_OVR_INT_STAT_action_on_write_field_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_ING_CBR_OVR_INT_STAT_read                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_ING_CBR_OVR_INT_STAT_poll                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 epmm_reg_ING_CBR_UDR_INT_STAT_write                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_ING_CBR_UDR_INT_STAT_action_on_write_field_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_ING_CBR_UDR_INT_STAT_read                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_ING_CBR_UDR_INT_STAT_poll                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 epmm_reg_EGR_GSUP_OVR_INT_STAT_write                     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_EGR_GSUP_OVR_INT_STAT_action_on_write_field_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_EGR_GSUP_OVR_INT_STAT_read                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_EGR_GSUP_OVR_INT_STAT_poll                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 epmm_reg_EGR_GSUP_UDR_INT_STAT_write                     ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_EGR_GSUP_UDR_INT_STAT_action_on_write_field_set ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_EGR_GSUP_UDR_INT_STAT_read                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_EGR_GSUP_UDR_INT_STAT_poll                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 epmm_reg_EGR_CBR_OVR_INT_STAT_write                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_EGR_CBR_OVR_INT_STAT_action_on_write_field_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_EGR_CBR_OVR_INT_STAT_read                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_EGR_CBR_OVR_INT_STAT_poll                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 epmm_reg_EGR_CBR_UDR_INT_STAT_write                      ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 value )
 | void                 epmm_reg_EGR_CBR_UDR_INT_STAT_action_on_write_field_set  ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               epmm_reg_EGR_CBR_UDR_INT_STAT_read                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_EGR_CBR_UDR_INT_STAT_poll                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               epmm_reg_ING_GSUP_OVR_INT_VAL_read                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_ING_GSUP_OVR_INT_VAL_poll                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               epmm_reg_ING_GSUP_UDR_INT_VAL_read                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_ING_GSUP_UDR_INT_VAL_poll                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               epmm_reg_ING_CBR_OVR_INT_VAL_read                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_ING_CBR_OVR_INT_VAL_poll                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               epmm_reg_ING_CBR_UDR_INT_VAL_read                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_ING_CBR_UDR_INT_VAL_poll                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               epmm_reg_EGR_GSUP_OVR_INT_VAL_read                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_EGR_GSUP_OVR_INT_VAL_poll                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               epmm_reg_EGR_GSUP_UDR_INT_VAL_read                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_EGR_GSUP_UDR_INT_VAL_poll                       ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               epmm_reg_EGR_CBR_OVR_INT_VAL_read                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_EGR_CBR_OVR_INT_VAL_poll                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               epmm_reg_EGR_CBR_UDR_INT_VAL_read                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE epmm_reg_EGR_CBR_UDR_INT_VAL_poll                        ( epmm_buffer_t *b_ptr, epmm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _EPMM_IO_H */
