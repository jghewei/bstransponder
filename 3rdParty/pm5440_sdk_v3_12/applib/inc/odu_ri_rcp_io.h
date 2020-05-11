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
 *     and register accessor functions for the odu_ri_rcp block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing odu_ri_rcp io functions is:
 | {
 |     odu_ri_rcp_buffer_t b_ptr[1];
 |     odu_ri_rcp_buffer_init( b_ptr, h_ptr );
 |     value = odu_ri_rcp_field_<FIELD>_get( b_ptr, h_ptr );
 |     odu_ri_rcp_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     odu_ri_rcp_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = odu_ri_rcp_field_<FIELD>_get( NULL, h_ptr );
 | odu_ri_rcp_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------+
 | | Register 0x001c0000 REG_SW_RESET |
 | +----------------------------------+
 | | bit  1 R/W  REG_SOFT_SW_RESET    |
 | | bit  0 R/W  REG_SW_RESET         |
 | +----------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_SOFT_SW_RESET_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_SOFT_SW_RESET_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_REG_SW_RESET_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_SW_RESET_get      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +--------------------------------+
 | | Register 0x001c0004 REG_LOWPWR |
 | +--------------------------------+
 | | bit  0 R/W  REG_LOWPWR         |
 | +--------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_LOWPWR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_LOWPWR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x001c000c REG_RAM_PWR_MGMT |
 | +--------------------------------------+
 | | bit  0 R/W  REG_RAM_LOWPWR           |
 | +--------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_RAM_LOWPWR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_RAM_LOWPWR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x001c0010 IRQ_SUMMARY |
 | +---------------------------------+
 | | bit  12:0 R  INTERRUPT_SUMMARY  |
 | +---------------------------------+
 | 
 | UINT32               odu_ri_rcp_field_INTERRUPT_SUMMARY_get        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | UINT32               odu_ri_rcp_field_range_INTERRUPT_SUMMARY_get  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_field_range_INTERRUPT_SUMMARY_poll ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_field_INTERRUPT_SUMMARY_poll       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x001c001c REG_GOJF               |
 | +--------------------------------------------+
 | | bit  21:16 R/W  REG_ING_FIFO_MIN_SLOT_FREE |
 | | bit  13:8  R/W  REG_ING_FIFO_MIN_LVL       |
 | | bit  4     R/W  REG_DROP_PDU_WO_DFEC       |
 | | bit  3     R/W  REG_ETH_RX_10B_SWIZZLE     |
 | | bit  2     R/W  REG_ETH_TX_10B_SWIZZLE     |
 | | bit  1     R/W  REG_ETH_RX_8B_SWIZZLE      |
 | | bit  0     R/W  REG_ETH_TX_8B_SWIZZLE      |
 | +--------------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_ING_FIFO_MIN_SLOT_FREE_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_ING_FIFO_MIN_SLOT_FREE_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_REG_ING_FIFO_MIN_SLOT_FREE_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_ING_FIFO_MIN_SLOT_FREE_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_REG_ING_FIFO_MIN_LVL_set             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_ING_FIFO_MIN_LVL_get             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_REG_ING_FIFO_MIN_LVL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_ING_FIFO_MIN_LVL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_REG_DROP_PDU_WO_DFEC_set             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_DROP_PDU_WO_DFEC_get             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_REG_ETH_RX_10B_SWIZZLE_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_ETH_RX_10B_SWIZZLE_get           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_REG_ETH_TX_10B_SWIZZLE_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_ETH_TX_10B_SWIZZLE_get           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_REG_ETH_RX_8B_SWIZZLE_set            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_ETH_RX_8B_SWIZZLE_get            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_REG_ETH_TX_8B_SWIZZLE_set            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_ETH_TX_8B_SWIZZLE_get            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x001c0020 GEN_IRQ_I |
 | +-------------------------------+
 | | bit  0 R/W  FEGE_TIP_I        |
 | +-------------------------------+
 | 
 | void                 odu_ri_rcp_field_FEGE_TIP_I_set_to_clear ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_ri_rcp_field_FEGE_TIP_I_get          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_field_FEGE_TIP_I_poll         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------+
 | | Register 0x001c0040 GEN_IRQ_E |
 | +-------------------------------+
 | | bit  0 R/W  FEGE_TIP_E        |
 | +-------------------------------+
 | 
 | void   odu_ri_rcp_field_FEGE_TIP_E_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_FEGE_TIP_E_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x001c0060 REG_ETH_TX_EN |
 | +-----------------------------------+
 | | bit  0 R/W  REG_ETH_TX_EN         |
 | +-----------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_ETH_TX_EN_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_ETH_TX_EN_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x001c0068 REG_ETH_MAC_OUI |
 | +-------------------------------------+
 | | bit  23:0 R/W  REG_ETH_MAC_OUI      |
 | +-------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_ETH_MAC_OUI_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_ETH_MAC_OUI_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_REG_ETH_MAC_OUI_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_ETH_MAC_OUI_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x001c006c REG_ETH_MAC_DIGI_ID |
 | +-----------------------------------------+
 | | bit  7:0 R/W  REG_ETH_MAC_DIGI_ID       |
 | +-----------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_ETH_MAC_DIGI_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_ETH_MAC_DIGI_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_REG_ETH_MAC_DIGI_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_ETH_MAC_DIGI_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x001c0070 REG_ETH_ETYPE |
 | +-----------------------------------+
 | | bit  15:0 R/W  REG_ETH_ETYPE      |
 | +-----------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_ETH_ETYPE_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_ETH_ETYPE_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_REG_ETH_ETYPE_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_ETH_ETYPE_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x001c0080 + (N) * 4) HO_ODU_MAC_DA_CFG |
 | |     N[0..31]                                      |
 | +---------------------------------------------------+
 | | bit  23:16 R/W  HO_ODU_DST_DIGI                   |
 | | bit  10:8  R/W  HO_ODU_DST_FRM                    |
 | | bit  6:0   R/W  HO_ODU_DST_CHANNEL                |
 | +---------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_HO_ODU_DST_DIGI_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_HO_ODU_DST_DIGI_get          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_HO_ODU_DST_DIGI_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_HO_ODU_DST_DIGI_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_HO_ODU_DST_FRM_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_HO_ODU_DST_FRM_get           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_HO_ODU_DST_FRM_set     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_HO_ODU_DST_FRM_get     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_HO_ODU_DST_CHANNEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_HO_ODU_DST_CHANNEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_HO_ODU_DST_CHANNEL_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_HO_ODU_DST_CHANNEL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x001c0100 + (N) * 4) LO_ODU_MAC_DA_CFG |
 | |     N[0..511]                                     |
 | +---------------------------------------------------+
 | | bit  23:16 R/W  LO_ODU_DST_DIGI                   |
 | | bit  10:8  R/W  LO_ODU_DST_FRM                    |
 | | bit  6:0   R/W  LO_ODU_DST_CHANNEL                |
 | +---------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_LO_ODU_DST_DIGI_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_LO_ODU_DST_DIGI_get          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_LO_ODU_DST_DIGI_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_LO_ODU_DST_DIGI_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_LO_ODU_DST_FRM_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_LO_ODU_DST_FRM_get           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_LO_ODU_DST_FRM_set     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_LO_ODU_DST_FRM_get     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_LO_ODU_DST_CHANNEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_LO_ODU_DST_CHANNEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_LO_ODU_DST_CHANNEL_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_LO_ODU_DST_CHANNEL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x001c0b00 REG_TX_STG_1_RCP_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  11:0 R/W  REG_TX_STG_1_RCP_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_TX_STG_1_RCP_INS_EN_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_TX_STG_1_RCP_INS_EN_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_REG_TX_STG_1_RCP_INS_EN_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_TX_STG_1_RCP_INS_EN_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x001c0b10 REG_TX_STG_2_RCP_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_2_RCP_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_REG_TX_STG_2_RCP_INS_EN_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_REG_TX_STG_2_RCP_INS_EN_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_REG_TX_STG_2_RCP_INS_EN_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_REG_TX_STG_2_RCP_INS_EN_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x001c0b20 REG_TX_STG_3A_RCP_INS_EN_0 |
 | +------------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_3A_RCP_INS_EN        |
 | +------------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_REG_TX_STG_3A_RCP_INS_EN_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_REG_TX_STG_3A_RCP_INS_EN_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_REG_TX_STG_3A_RCP_INS_EN_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_REG_TX_STG_3A_RCP_INS_EN_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x001c0b30 REG_TX_STG_3B_RCP_INS_EN_0 |
 | +------------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_3B_RCP_INS_EN        |
 | +------------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_REG_TX_STG_3B_RCP_INS_EN_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_REG_TX_STG_3B_RCP_INS_EN_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_REG_TX_STG_3B_RCP_INS_EN_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_REG_TX_STG_3B_RCP_INS_EN_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x001c0b40 REG_TX_STG_4_RCP_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_4_RCP_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_REG_TX_STG_4_RCP_INS_EN_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_REG_TX_STG_4_RCP_INS_EN_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_REG_TX_STG_4_RCP_INS_EN_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_REG_TX_STG_4_RCP_INS_EN_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x001c0b50 REG_TX_STG_N_RCP_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  11:0 R/W  REG_TX_STG_N_RCP_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_TX_STG_N_RCP_INS_EN_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_TX_STG_N_RCP_INS_EN_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_REG_TX_STG_N_RCP_INS_EN_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_TX_STG_N_RCP_INS_EN_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x001c0b80 REG_TX_STG_1_APS_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  11:0 R/W  REG_TX_STG_1_APS_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_TX_STG_1_APS_INS_EN_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_TX_STG_1_APS_INS_EN_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_REG_TX_STG_1_APS_INS_EN_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_TX_STG_1_APS_INS_EN_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x001c0b90 REG_TX_STG_2_APS_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_2_APS_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_REG_TX_STG_2_APS_INS_EN_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_REG_TX_STG_2_APS_INS_EN_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_REG_TX_STG_2_APS_INS_EN_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_REG_TX_STG_2_APS_INS_EN_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x001c0ba0 REG_TX_STG_3A_APS_INS_EN_0 |
 | +------------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_3A_APS_INS_EN        |
 | +------------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_REG_TX_STG_3A_APS_INS_EN_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_REG_TX_STG_3A_APS_INS_EN_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_REG_TX_STG_3A_APS_INS_EN_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_REG_TX_STG_3A_APS_INS_EN_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x001c0bb0 REG_TX_STG_3B_APS_INS_EN_0 |
 | +------------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_3B_APS_INS_EN        |
 | +------------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_REG_TX_STG_3B_APS_INS_EN_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_REG_TX_STG_3B_APS_INS_EN_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_REG_TX_STG_3B_APS_INS_EN_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_REG_TX_STG_3B_APS_INS_EN_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x001c0bc0 REG_TX_STG_4_APS_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_4_APS_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_REG_TX_STG_4_APS_INS_EN_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_REG_TX_STG_4_APS_INS_EN_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_REG_TX_STG_4_APS_INS_EN_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_REG_TX_STG_4_APS_INS_EN_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x001c0bd0 REG_TX_STG_N_APS_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  11:0 R/W  REG_TX_STG_N_APS_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_TX_STG_N_APS_INS_EN_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_TX_STG_N_APS_INS_EN_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_REG_TX_STG_N_APS_INS_EN_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_TX_STG_N_APS_INS_EN_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x001c0c00 + (N) * 0x04) RX_STG_1_APS_LEVEL |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  2:0 R/W  REG_RX_STG_1_APS_LEVEL                  |
 | +-------------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_RX_STG_1_APS_LEVEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_RX_STG_1_APS_LEVEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_REG_RX_STG_1_APS_LEVEL_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_RX_STG_1_APS_LEVEL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x001c0c80 + (N) * 0x04) RX_STG_2_APS_LEVEL |
 | |     N[0..95]                                          |
 | +-------------------------------------------------------+
 | | bit  2:0 R/W  REG_RX_STG_2_APS_LEVEL                  |
 | +-------------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_RX_STG_2_APS_LEVEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_RX_STG_2_APS_LEVEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_REG_RX_STG_2_APS_LEVEL_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_RX_STG_2_APS_LEVEL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x001c0e00 + (N) * 0x04) RX_STG_3A_APS_LEVEL |
 | |     N[0..95]                                           |
 | +--------------------------------------------------------+
 | | bit  2:0 R/W  REG_RX_STG_3A_APS_LEVEL                  |
 | +--------------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_RX_STG_3A_APS_LEVEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_RX_STG_3A_APS_LEVEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_REG_RX_STG_3A_APS_LEVEL_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_RX_STG_3A_APS_LEVEL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x001c0f80 + (N) * 0x04) RX_STG_3B_APS_LEVEL |
 | |     N[0..95]                                           |
 | +--------------------------------------------------------+
 | | bit  2:0 R/W  REG_RX_STG_3B_APS_LEVEL                  |
 | +--------------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_RX_STG_3B_APS_LEVEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_RX_STG_3B_APS_LEVEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_REG_RX_STG_3B_APS_LEVEL_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_RX_STG_3B_APS_LEVEL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x001c1100 + (N) * 0x04) RX_STG_4_APS_LEVEL |
 | |     N[0..95]                                          |
 | +-------------------------------------------------------+
 | | bit  2:0 R/W  REG_RX_STG_4_APS_LEVEL                  |
 | +-------------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_RX_STG_4_APS_LEVEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_RX_STG_4_APS_LEVEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_REG_RX_STG_4_APS_LEVEL_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_RX_STG_4_APS_LEVEL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x001c1280 + (N) * 0x04) RX_STG_N_APS_LEVEL |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  2:0 R/W  REG_RX_STG_N_APS_LEVEL                  |
 | +-------------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_RX_STG_N_APS_LEVEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_RX_STG_N_APS_LEVEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_REG_RX_STG_N_APS_LEVEL_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_RX_STG_N_APS_LEVEL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x001c1300 CFC_DBG_SEL |
 | +---------------------------------+
 | | bit  4   R/W  CFC_DBG_SRC_SEL   |
 | | bit  2:0 R/W  CFC_DBG_STG_SEL   |
 | +---------------------------------+
 | 
 | void   odu_ri_rcp_field_CFC_DBG_SRC_SEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_CFC_DBG_SRC_SEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_CFC_DBG_STG_SEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_CFC_DBG_STG_SEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_CFC_DBG_STG_SEL_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_CFC_DBG_STG_SEL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------+
 | | Register 0x001c1304 DBG_CTRL |
 | +------------------------------+
 | | bit  16   R/W  DBG_TRIG      |
 | | bit  10:0 R/W  DBG_RADDR     |
 | +------------------------------+
 | 
 | void   odu_ri_rcp_field_DBG_RADDR_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_DBG_RADDR_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_DBG_RADDR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_DBG_RADDR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_DBG_TRIG_set        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_DBG_TRIG_get        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x001c1308 DBG_RDATA |
 | +-------------------------------+
 | | bit  31:0 R  DBG_RDATA        |
 | +-------------------------------+
 | 
 | UINT32 odu_ri_rcp_field_DBG_RDATA_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | UINT32 odu_ri_rcp_field_range_DBG_RDATA_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x001c1320 CFC_IRQ_SEL   |
 | +-----------------------------------+
 | | bit  4   R/W  REG_CFC_IRQ_SRC_SEL |
 | | bit  2:0 R/W  REG_CFC_IRQ_STG_SEL |
 | +-----------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_CFC_IRQ_SRC_SEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_CFC_IRQ_SRC_SEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_REG_CFC_IRQ_STG_SEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_CFC_IRQ_STG_SEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_REG_CFC_IRQ_STG_SEL_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_CFC_IRQ_STG_SEL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x001c1324 CFC_IRQ_TYPE_SEL_0 |
 | +----------------------------------------+
 | | bit  31:0 R/W  REG_CFC_IRQ_TYPE_SEL    |
 | +----------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_REG_CFC_IRQ_TYPE_SEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_REG_CFC_IRQ_TYPE_SEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_REG_CFC_IRQ_TYPE_SEL_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_REG_CFC_IRQ_TYPE_SEL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x001c1330 CFC_FIFO_IRQ_0 |
 | +------------------------------------+
 | | bit  31:0 R/W  CFC_FIFO_IRQ_I      |
 | +------------------------------------+
 | 
 | void                 odu_ri_rcp_lfield_CFC_FIFO_IRQ_I_set_to_clear       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_ri_rcp_lfield_CFC_FIFO_IRQ_I_get                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_ri_rcp_lfield_range_CFC_FIFO_IRQ_I_set_to_clear ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               odu_ri_rcp_lfield_range_CFC_FIFO_IRQ_I_get          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_lfield_range_CFC_FIFO_IRQ_I_poll         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_lfield_CFC_FIFO_IRQ_I_poll               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x001c1340 CFC_IPT_ERR_SEL   |
 | +---------------------------------------+
 | | bit  4   R/W  REG_CFC_IPT_ERR_SRC_SEL |
 | | bit  2:0 R/W  REG_CFC_IPT_ERR_STG_SEL |
 | +---------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_CFC_IPT_ERR_SRC_SEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_CFC_IPT_ERR_SRC_SEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_REG_CFC_IPT_ERR_STG_SEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_CFC_IPT_ERR_STG_SEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_REG_CFC_IPT_ERR_STG_SEL_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_CFC_IPT_ERR_STG_SEL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x001c1344 CFC_IPT_ERR_0 |
 | +-----------------------------------+
 | | bit  31:0 R/W  CFC_IPT_ERR_I      |
 | +-----------------------------------+
 | 
 | void                 odu_ri_rcp_lfield_CFC_IPT_ERR_I_set_to_clear       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_ri_rcp_lfield_CFC_IPT_ERR_I_get                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_ri_rcp_lfield_range_CFC_IPT_ERR_I_set_to_clear ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               odu_ri_rcp_lfield_range_CFC_IPT_ERR_I_get          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_lfield_range_CFC_IPT_ERR_I_poll         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_lfield_CFC_IPT_ERR_I_poll               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x001c1360 CFC_FIFO_IRQ_EN_0 |
 | +---------------------------------------+
 | | bit  31:0 R/W  CFC_FIFO_IRQ_E         |
 | +---------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_CFC_FIFO_IRQ_E_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_CFC_FIFO_IRQ_E_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_CFC_FIFO_IRQ_E_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_CFC_FIFO_IRQ_E_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x001c1380 CFC_IPT_ERR_EN_0 |
 | +--------------------------------------+
 | | bit  31:0 R/W  CFC_IPT_ERR_E         |
 | +--------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_CFC_IPT_ERR_E_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_CFC_IPT_ERR_E_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_CFC_IPT_ERR_E_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_CFC_IPT_ERR_E_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x001c13c0 CFC_STATUS_SEL   |
 | +--------------------------------------+
 | | bit  4   R/W  REG_CFC_STATUS_SRC_SEL |
 | | bit  2:0 R/W  REG_CFC_STATUS_STG_SEL |
 | +--------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_CFC_STATUS_SRC_SEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_CFC_STATUS_SRC_SEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_REG_CFC_STATUS_STG_SEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_CFC_STATUS_STG_SEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_REG_CFC_STATUS_STG_SEL_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_CFC_STATUS_STG_SEL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x001c13c4 CFC_EMPTY_0 |
 | +---------------------------------+
 | | bit  31:0 R  CFC_EMPTY          |
 | +---------------------------------+
 | 
 | void   odu_ri_rcp_lfield_CFC_EMPTY_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 odu_ri_rcp_lfield_range_CFC_EMPTY_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x001c13d0 CFC_FULL_0 |
 | +--------------------------------+
 | | bit  31:0 R  CFC_FULL          |
 | +--------------------------------+
 | 
 | void   odu_ri_rcp_lfield_CFC_FULL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 odu_ri_rcp_lfield_range_CFC_FULL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x001c1800 RX_STG_1_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  11:0 R/W  RX_STG_1_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_1_FIFO_CLRB_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_1_FIFO_CLRB_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_RX_STG_1_FIFO_CLRB_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_1_FIFO_CLRB_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x001c1820 RX_STG_1_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  RX_STG_1_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  RX_STG_1_CFC_END_ADDR_CHG_REQ   |
 | | bit  19:16 R/W  RX_STG_1_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_RX_STG_1_CFC_END_ADDR_CHG_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_1_CFC_END_ADDR_CHG_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_ABORT_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_ABORT_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_REQ_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_REQ_get      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x001c1824 RX_STG_1_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  RX_STG_1_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 odu_ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_SUCCESS_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x001c1828 RX_STG_1_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  4:0 R/W  RX_STG_1_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_1_CFC_END_ADDR_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_1_CFC_END_ADDR_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_RX_STG_1_CFC_END_ADDR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_1_CFC_END_ADDR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x001c1840 + (N) * 0x04) RX_STG_1_CFC_IPT |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  RX_STG_1_CFC_IPT_VALID              |
 | | bit  19:16 R/W  RX_STG_1_CFC_IPT_ID                 |
 | | bit  3:0   R/W  RX_STG_1_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_1_CFC_IPT_VALID_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_1_CFC_IPT_VALID_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_RX_STG_1_CFC_IPT_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_1_CFC_IPT_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_RX_STG_1_CFC_IPT_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_1_CFC_IPT_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_RX_STG_1_CFC_IPT_VA_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_1_CFC_IPT_VA_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_RX_STG_1_CFC_IPT_VA_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_1_CFC_IPT_VA_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x001c1a00 RX_STG_2_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  31:0 R/W  RX_STG_2_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_RX_STG_2_FIFO_CLRB_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_RX_STG_2_FIFO_CLRB_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_RX_STG_2_FIFO_CLRB_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_RX_STG_2_FIFO_CLRB_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x001c1a20 RX_STG_2_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  RX_STG_2_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  RX_STG_2_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  RX_STG_2_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_RX_STG_2_CFC_END_ADDR_CHG_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_2_CFC_END_ADDR_CHG_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_ABORT_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_ABORT_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_REQ_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_REQ_get      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x001c1a24 RX_STG_2_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  RX_STG_2_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 odu_ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_SUCCESS_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x001c1a28 RX_STG_2_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  7:0 R/W  RX_STG_2_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_2_CFC_END_ADDR_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_2_CFC_END_ADDR_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_RX_STG_2_CFC_END_ADDR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_2_CFC_END_ADDR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x001c1a40 + (N) * 0x04) RX_STG_2_CFC_IPT |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  RX_STG_2_CFC_IPT_VALID              |
 | | bit  22:16 R/W  RX_STG_2_CFC_IPT_ID                 |
 | | bit  6:0   R/W  RX_STG_2_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_2_CFC_IPT_VALID_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_2_CFC_IPT_VALID_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_RX_STG_2_CFC_IPT_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_2_CFC_IPT_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_RX_STG_2_CFC_IPT_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_2_CFC_IPT_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_RX_STG_2_CFC_IPT_VA_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_2_CFC_IPT_VA_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_RX_STG_2_CFC_IPT_VA_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_2_CFC_IPT_VA_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x001c1c00 RX_STG_3A_FIFO_CLRB_0 |
 | +-------------------------------------------+
 | | bit  31:0 R/W  RX_STG_3A_FIFO_CLRB        |
 | +-------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_RX_STG_3A_FIFO_CLRB_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_RX_STG_3A_FIFO_CLRB_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_RX_STG_3A_FIFO_CLRB_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_RX_STG_3A_FIFO_CLRB_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x001c1c20 RX_STG_3A_CFC_CFG            |
 | +--------------------------------------------------+
 | | bit  28    R/W  RX_STG_3A_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  RX_STG_3A_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  RX_STG_3A_CFC_END_ADDR_CHG_ID    |
 | +--------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_RX_STG_3A_CFC_END_ADDR_CHG_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_3A_CFC_END_ADDR_CHG_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_ABORT_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_ABORT_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_REQ_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_REQ_get      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x001c1c24 RX_STG_3A_CFC_CFG_STAT   |
 | +----------------------------------------------+
 | | bit  0 R  RX_STG_3A_CFC_END_ADDR_CHG_SUCCESS |
 | +----------------------------------------------+
 | 
 | UINT32 odu_ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_SUCCESS_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x001c1c28 RX_STG_3A_CFC_CFG_VAL |
 | +-------------------------------------------+
 | | bit  7:0 R/W  RX_STG_3A_CFC_END_ADDR      |
 | +-------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_3A_CFC_END_ADDR_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_3A_CFC_END_ADDR_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_RX_STG_3A_CFC_END_ADDR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_3A_CFC_END_ADDR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x001c1c40 + (N) * 0x04) RX_STG_3A_CFC_IPT |
 | |     N[0..95]                                         |
 | +------------------------------------------------------+
 | | bit  24    R/W  RX_STG_3A_CFC_IPT_VALID              |
 | | bit  22:16 R/W  RX_STG_3A_CFC_IPT_ID                 |
 | | bit  6:0   R/W  RX_STG_3A_CFC_IPT_VA                 |
 | +------------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_3A_CFC_IPT_VALID_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_3A_CFC_IPT_VALID_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_RX_STG_3A_CFC_IPT_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_3A_CFC_IPT_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_RX_STG_3A_CFC_IPT_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_3A_CFC_IPT_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_RX_STG_3A_CFC_IPT_VA_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_3A_CFC_IPT_VA_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_RX_STG_3A_CFC_IPT_VA_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_3A_CFC_IPT_VA_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x001c1e00 RX_STG_3B_FIFO_CLRB_0 |
 | +-------------------------------------------+
 | | bit  31:0 R/W  RX_STG_3B_FIFO_CLRB        |
 | +-------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_RX_STG_3B_FIFO_CLRB_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_RX_STG_3B_FIFO_CLRB_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_RX_STG_3B_FIFO_CLRB_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_RX_STG_3B_FIFO_CLRB_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x001c1e20 RX_STG_3B_CFC_CFG            |
 | +--------------------------------------------------+
 | | bit  28    R/W  RX_STG_3B_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  RX_STG_3B_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  RX_STG_3B_CFC_END_ADDR_CHG_ID    |
 | +--------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_RX_STG_3B_CFC_END_ADDR_CHG_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_3B_CFC_END_ADDR_CHG_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_ABORT_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_ABORT_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_REQ_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_REQ_get      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x001c1e24 RX_STG_3B_CFC_CFG_STAT   |
 | +----------------------------------------------+
 | | bit  0 R  RX_STG_3B_CFC_END_ADDR_CHG_SUCCESS |
 | +----------------------------------------------+
 | 
 | UINT32 odu_ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_SUCCESS_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x001c1e28 RX_STG_3B_CFC_CFG_VAL |
 | +-------------------------------------------+
 | | bit  7:0 R/W  RX_STG_3B_CFC_END_ADDR      |
 | +-------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_3B_CFC_END_ADDR_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_3B_CFC_END_ADDR_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_RX_STG_3B_CFC_END_ADDR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_3B_CFC_END_ADDR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x001c1e40 + (N) * 0x04) RX_STG_3B_CFC_IPT |
 | |     N[0..95]                                         |
 | +------------------------------------------------------+
 | | bit  24    R/W  RX_STG_3B_CFC_IPT_VALID              |
 | | bit  22:16 R/W  RX_STG_3B_CFC_IPT_ID                 |
 | | bit  6:0   R/W  RX_STG_3B_CFC_IPT_VA                 |
 | +------------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_3B_CFC_IPT_VALID_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_3B_CFC_IPT_VALID_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_RX_STG_3B_CFC_IPT_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_3B_CFC_IPT_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_RX_STG_3B_CFC_IPT_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_3B_CFC_IPT_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_RX_STG_3B_CFC_IPT_VA_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_3B_CFC_IPT_VA_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_RX_STG_3B_CFC_IPT_VA_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_3B_CFC_IPT_VA_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x001c2000 RX_STG_4_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  31:0 R/W  RX_STG_4_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_RX_STG_4_FIFO_CLRB_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_RX_STG_4_FIFO_CLRB_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_RX_STG_4_FIFO_CLRB_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_RX_STG_4_FIFO_CLRB_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x001c2020 RX_STG_4_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  RX_STG_4_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  RX_STG_4_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  RX_STG_4_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_RX_STG_4_CFC_END_ADDR_CHG_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_4_CFC_END_ADDR_CHG_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_ABORT_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_ABORT_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_REQ_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_REQ_get      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x001c2024 RX_STG_4_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  RX_STG_4_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 odu_ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_SUCCESS_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x001c2028 RX_STG_4_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  7:0 R/W  RX_STG_4_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_4_CFC_END_ADDR_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_4_CFC_END_ADDR_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_RX_STG_4_CFC_END_ADDR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_4_CFC_END_ADDR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x001c2040 + (N) * 0x04) RX_STG_4_CFC_IPT |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  RX_STG_4_CFC_IPT_VALID              |
 | | bit  22:16 R/W  RX_STG_4_CFC_IPT_ID                 |
 | | bit  6:0   R/W  RX_STG_4_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_4_CFC_IPT_VALID_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_4_CFC_IPT_VALID_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_RX_STG_4_CFC_IPT_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_4_CFC_IPT_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_RX_STG_4_CFC_IPT_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_4_CFC_IPT_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_RX_STG_4_CFC_IPT_VA_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_4_CFC_IPT_VA_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_RX_STG_4_CFC_IPT_VA_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_4_CFC_IPT_VA_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x001c2200 RX_STG_N_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  11:0 R/W  RX_STG_N_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_N_FIFO_CLRB_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_N_FIFO_CLRB_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_RX_STG_N_FIFO_CLRB_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_N_FIFO_CLRB_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x001c2220 RX_STG_N_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  RX_STG_N_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  RX_STG_N_CFC_END_ADDR_CHG_REQ   |
 | | bit  19:16 R/W  RX_STG_N_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_RX_STG_N_CFC_END_ADDR_CHG_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_N_CFC_END_ADDR_CHG_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_ABORT_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_ABORT_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_REQ_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_REQ_get      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x001c2224 RX_STG_N_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  RX_STG_N_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 odu_ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_SUCCESS_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x001c2228 RX_STG_N_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  4:0 R/W  RX_STG_N_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_N_CFC_END_ADDR_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_N_CFC_END_ADDR_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_RX_STG_N_CFC_END_ADDR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_N_CFC_END_ADDR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x001c2240 + (N) * 0x04) RX_STG_N_CFC_IPT |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  RX_STG_N_CFC_IPT_VALID              |
 | | bit  19:16 R/W  RX_STG_N_CFC_IPT_ID                 |
 | | bit  3:0   R/W  RX_STG_N_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_RX_STG_N_CFC_IPT_VALID_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_N_CFC_IPT_VALID_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_RX_STG_N_CFC_IPT_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_N_CFC_IPT_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_RX_STG_N_CFC_IPT_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_N_CFC_IPT_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_RX_STG_N_CFC_IPT_VA_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_RX_STG_N_CFC_IPT_VA_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_RX_STG_N_CFC_IPT_VA_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_RX_STG_N_CFC_IPT_VA_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x001c2400 TX_STG_1_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  11:0 R/W  TX_STG_1_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_1_FIFO_CLRB_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_1_FIFO_CLRB_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_TX_STG_1_FIFO_CLRB_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_1_FIFO_CLRB_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x001c2420 TX_STG_1_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  TX_STG_1_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  TX_STG_1_CFC_END_ADDR_CHG_REQ   |
 | | bit  19:16 R/W  TX_STG_1_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_TX_STG_1_CFC_END_ADDR_CHG_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_1_CFC_END_ADDR_CHG_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_ABORT_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_ABORT_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_REQ_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_REQ_get      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x001c2424 TX_STG_1_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  TX_STG_1_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 odu_ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_SUCCESS_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x001c2428 TX_STG_1_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  4:0 R/W  TX_STG_1_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_1_CFC_END_ADDR_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_1_CFC_END_ADDR_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_TX_STG_1_CFC_END_ADDR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_1_CFC_END_ADDR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x001c2440 + (N) * 0x04) TX_STG_1_CFC_IPT |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  TX_STG_1_CFC_IPT_VALID              |
 | | bit  19:16 R/W  TX_STG_1_CFC_IPT_ID                 |
 | | bit  3:0   R/W  TX_STG_1_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_1_CFC_IPT_VALID_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_1_CFC_IPT_VALID_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_TX_STG_1_CFC_IPT_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_1_CFC_IPT_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_TX_STG_1_CFC_IPT_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_1_CFC_IPT_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_TX_STG_1_CFC_IPT_VA_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_1_CFC_IPT_VA_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_TX_STG_1_CFC_IPT_VA_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_1_CFC_IPT_VA_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x001c2600 TX_STG_2_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  31:0 R/W  TX_STG_2_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_TX_STG_2_FIFO_CLRB_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_TX_STG_2_FIFO_CLRB_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_TX_STG_2_FIFO_CLRB_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_TX_STG_2_FIFO_CLRB_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x001c2620 TX_STG_2_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  TX_STG_2_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  TX_STG_2_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  TX_STG_2_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_TX_STG_2_CFC_END_ADDR_CHG_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_2_CFC_END_ADDR_CHG_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_ABORT_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_ABORT_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_REQ_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_REQ_get      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x001c2624 TX_STG_2_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  TX_STG_2_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 odu_ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_SUCCESS_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x001c2628 TX_STG_2_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  7:0 R/W  TX_STG_2_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_2_CFC_END_ADDR_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_2_CFC_END_ADDR_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_TX_STG_2_CFC_END_ADDR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_2_CFC_END_ADDR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x001c2640 + (N) * 0x04) TX_STG_2_CFC_IPT |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  TX_STG_2_CFC_IPT_VALID              |
 | | bit  22:16 R/W  TX_STG_2_CFC_IPT_ID                 |
 | | bit  6:0   R/W  TX_STG_2_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_2_CFC_IPT_VALID_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_2_CFC_IPT_VALID_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_TX_STG_2_CFC_IPT_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_2_CFC_IPT_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_TX_STG_2_CFC_IPT_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_2_CFC_IPT_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_TX_STG_2_CFC_IPT_VA_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_2_CFC_IPT_VA_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_TX_STG_2_CFC_IPT_VA_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_2_CFC_IPT_VA_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x001c2800 TX_STG_3A_FIFO_CLRB_0 |
 | +-------------------------------------------+
 | | bit  31:0 R/W  TX_STG_3A_FIFO_CLRB        |
 | +-------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_TX_STG_3A_FIFO_CLRB_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_TX_STG_3A_FIFO_CLRB_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_TX_STG_3A_FIFO_CLRB_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_TX_STG_3A_FIFO_CLRB_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x001c2820 TX_STG_3A_CFC_CFG            |
 | +--------------------------------------------------+
 | | bit  28    R/W  TX_STG_3A_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  TX_STG_3A_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  TX_STG_3A_CFC_END_ADDR_CHG_ID    |
 | +--------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_TX_STG_3A_CFC_END_ADDR_CHG_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_3A_CFC_END_ADDR_CHG_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_ABORT_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_ABORT_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_REQ_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_REQ_get      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x001c2824 TX_STG_3A_CFC_CFG_STAT   |
 | +----------------------------------------------+
 | | bit  0 R  TX_STG_3A_CFC_END_ADDR_CHG_SUCCESS |
 | +----------------------------------------------+
 | 
 | UINT32 odu_ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_SUCCESS_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x001c2828 TX_STG_3A_CFC_CFG_VAL |
 | +-------------------------------------------+
 | | bit  7:0 R/W  TX_STG_3A_CFC_END_ADDR      |
 | +-------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_3A_CFC_END_ADDR_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_3A_CFC_END_ADDR_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_TX_STG_3A_CFC_END_ADDR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_3A_CFC_END_ADDR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x001c2840 + (N) * 0x04) TX_STG_3A_CFC_IPT |
 | |     N[0..95]                                         |
 | +------------------------------------------------------+
 | | bit  24    R/W  TX_STG_3A_CFC_IPT_VALID              |
 | | bit  22:16 R/W  TX_STG_3A_CFC_IPT_ID                 |
 | | bit  6:0   R/W  TX_STG_3A_CFC_IPT_VA                 |
 | +------------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_3A_CFC_IPT_VALID_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_3A_CFC_IPT_VALID_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_TX_STG_3A_CFC_IPT_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_3A_CFC_IPT_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_TX_STG_3A_CFC_IPT_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_3A_CFC_IPT_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_TX_STG_3A_CFC_IPT_VA_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_3A_CFC_IPT_VA_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_TX_STG_3A_CFC_IPT_VA_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_3A_CFC_IPT_VA_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x001c2a00 TX_STG_3B_FIFO_CLRB_0 |
 | +-------------------------------------------+
 | | bit  31:0 R/W  TX_STG_3B_FIFO_CLRB        |
 | +-------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_TX_STG_3B_FIFO_CLRB_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_TX_STG_3B_FIFO_CLRB_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_TX_STG_3B_FIFO_CLRB_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_TX_STG_3B_FIFO_CLRB_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x001c2a20 TX_STG_3B_CFC_CFG            |
 | +--------------------------------------------------+
 | | bit  28    R/W  TX_STG_3B_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  TX_STG_3B_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  TX_STG_3B_CFC_END_ADDR_CHG_ID    |
 | +--------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_TX_STG_3B_CFC_END_ADDR_CHG_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_3B_CFC_END_ADDR_CHG_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_ABORT_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_ABORT_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_REQ_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_REQ_get      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x001c2a24 TX_STG_3B_CFC_CFG_STAT   |
 | +----------------------------------------------+
 | | bit  0 R  TX_STG_3B_CFC_END_ADDR_CHG_SUCCESS |
 | +----------------------------------------------+
 | 
 | UINT32 odu_ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_SUCCESS_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x001c2a28 TX_STG_3B_CFC_CFG_VAL |
 | +-------------------------------------------+
 | | bit  7:0 R/W  TX_STG_3B_CFC_END_ADDR      |
 | +-------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_3B_CFC_END_ADDR_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_3B_CFC_END_ADDR_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_TX_STG_3B_CFC_END_ADDR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_3B_CFC_END_ADDR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x001c2a40 + (N) * 0x04) TX_STG_3B_CFC_IPT |
 | |     N[0..95]                                         |
 | +------------------------------------------------------+
 | | bit  24    R/W  TX_STG_3B_CFC_IPT_VALID              |
 | | bit  22:16 R/W  TX_STG_3B_CFC_IPT_ID                 |
 | | bit  6:0   R/W  TX_STG_3B_CFC_IPT_VA                 |
 | +------------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_3B_CFC_IPT_VALID_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_3B_CFC_IPT_VALID_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_TX_STG_3B_CFC_IPT_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_3B_CFC_IPT_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_TX_STG_3B_CFC_IPT_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_3B_CFC_IPT_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_TX_STG_3B_CFC_IPT_VA_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_3B_CFC_IPT_VA_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_TX_STG_3B_CFC_IPT_VA_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_3B_CFC_IPT_VA_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x001c2c00 TX_STG_4_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  31:0 R/W  TX_STG_4_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_lfield_TX_STG_4_FIFO_CLRB_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_TX_STG_4_FIFO_CLRB_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ri_rcp_lfield_range_TX_STG_4_FIFO_CLRB_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_lfield_range_TX_STG_4_FIFO_CLRB_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x001c2c20 TX_STG_4_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  TX_STG_4_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  TX_STG_4_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  TX_STG_4_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_TX_STG_4_CFC_END_ADDR_CHG_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_4_CFC_END_ADDR_CHG_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_ABORT_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_ABORT_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_REQ_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_REQ_get      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x001c2c24 TX_STG_4_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  TX_STG_4_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 odu_ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_SUCCESS_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x001c2c28 TX_STG_4_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  7:0 R/W  TX_STG_4_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_4_CFC_END_ADDR_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_4_CFC_END_ADDR_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_TX_STG_4_CFC_END_ADDR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_4_CFC_END_ADDR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x001c2c40 + (N) * 0x04) TX_STG_4_CFC_IPT |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  TX_STG_4_CFC_IPT_VALID              |
 | | bit  22:16 R/W  TX_STG_4_CFC_IPT_ID                 |
 | | bit  6:0   R/W  TX_STG_4_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_4_CFC_IPT_VALID_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_4_CFC_IPT_VALID_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_TX_STG_4_CFC_IPT_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_4_CFC_IPT_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_TX_STG_4_CFC_IPT_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_4_CFC_IPT_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_TX_STG_4_CFC_IPT_VA_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_4_CFC_IPT_VA_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_TX_STG_4_CFC_IPT_VA_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_4_CFC_IPT_VA_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x001c2e00 TX_STG_N_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  11:0 R/W  TX_STG_N_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_N_FIFO_CLRB_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_N_FIFO_CLRB_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_TX_STG_N_FIFO_CLRB_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_N_FIFO_CLRB_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x001c2e20 TX_STG_N_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  TX_STG_N_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  TX_STG_N_CFC_END_ADDR_CHG_REQ   |
 | | bit  19:16 R/W  TX_STG_N_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_TX_STG_N_CFC_END_ADDR_CHG_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_N_CFC_END_ADDR_CHG_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_ABORT_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_ABORT_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_REQ_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_REQ_get      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x001c2e24 TX_STG_N_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  TX_STG_N_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 odu_ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_SUCCESS_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x001c2e28 TX_STG_N_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  4:0 R/W  TX_STG_N_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_N_CFC_END_ADDR_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_N_CFC_END_ADDR_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_TX_STG_N_CFC_END_ADDR_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_N_CFC_END_ADDR_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x001c2e40 + (N) * 0x04) TX_STG_N_CFC_IPT |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  TX_STG_N_CFC_IPT_VALID              |
 | | bit  19:16 R/W  TX_STG_N_CFC_IPT_ID                 |
 | | bit  3:0   R/W  TX_STG_N_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_TX_STG_N_CFC_IPT_VALID_set    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_N_CFC_IPT_VALID_get    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_TX_STG_N_CFC_IPT_ID_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_N_CFC_IPT_ID_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_TX_STG_N_CFC_IPT_ID_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_N_CFC_IPT_ID_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_TX_STG_N_CFC_IPT_VA_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_TX_STG_N_CFC_IPT_VA_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_TX_STG_N_CFC_IPT_VA_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_TX_STG_N_CFC_IPT_VA_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x001c3000 REG_SCH_CAL_ENABLE |
 | +----------------------------------------+
 | | bit  5:0 R/W  REG_SCH_CAL_EN           |
 | +----------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_SCH_CAL_EN_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_SCH_CAL_EN_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_REG_SCH_CAL_EN_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_SCH_CAL_EN_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x001c3004 REG_SCH_CAL_SEL |
 | +-------------------------------------+
 | | bit  5:0 R/W  REG_SCH_CAL_SEL       |
 | +-------------------------------------+
 | 
 | void   odu_ri_rcp_field_REG_SCH_CAL_SEL_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ri_rcp_field_REG_SCH_CAL_SEL_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   odu_ri_rcp_field_range_REG_SCH_CAL_SEL_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_REG_SCH_CAL_SEL_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x001c3080 + (N) * 0x04) SCH_STG_1_ACT_CFG |
 | |     N[0..11]                                         |
 | +------------------------------------------------------+
 | | bit  3:0 R/W  SCH_STG_1_ACT_CFG                      |
 | +------------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_SCH_STG_1_ACT_CFG_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_1_ACT_CFG_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_1_ACT_CFG_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_1_ACT_CFG_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x001c30c0 + (N) * 0x04) SCH_STG_1_STBY_CFG |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  3:0 R/W  SCH_STG_1_STBY_CFG                      |
 | +-------------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_SCH_STG_1_STBY_CFG_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_1_STBY_CFG_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_1_STBY_CFG_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_1_STBY_CFG_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x001c3100 + (N) * 4) SCH_STG_2_ACT_CFG |
 | |     N[0..48]                                      |
 | +---------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_2_ACT_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_2_ACT_CH_N                 |
 | +---------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_SCH_STG_2_ACT_CH_NP1_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_2_ACT_CH_NP1_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_2_ACT_CH_NP1_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_2_ACT_CH_NP1_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_SCH_STG_2_ACT_CH_N_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_2_ACT_CH_N_get         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_2_ACT_CH_N_set   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_2_ACT_CH_N_get   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x001c3200 + (N) * 4) SCH_STG_2_STBY_CFG |
 | |     N[0..48]                                       |
 | +----------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_2_STBY_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_2_STBY_CH_N                 |
 | +----------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_SCH_STG_2_STBY_CH_NP1_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_2_STBY_CH_NP1_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_2_STBY_CH_NP1_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_2_STBY_CH_NP1_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_SCH_STG_2_STBY_CH_N_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_2_STBY_CH_N_get         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_2_STBY_CH_N_set   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_2_STBY_CH_N_get   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x001c3300 + (N) * 4) SCH_STG_3A_ACT_CFG |
 | |     N[0..48]                                       |
 | +----------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_3A_ACT_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_3A_ACT_CH_N                 |
 | +----------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_SCH_STG_3A_ACT_CH_NP1_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_3A_ACT_CH_NP1_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_3A_ACT_CH_NP1_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_3A_ACT_CH_NP1_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_SCH_STG_3A_ACT_CH_N_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_3A_ACT_CH_N_get         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_3A_ACT_CH_N_set   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_3A_ACT_CH_N_get   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x001c3400 + (N) * 4) SCH_STG_3A_STBY_CFG |
 | |     N[0..48]                                        |
 | +-----------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_3A_STBY_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_3A_STBY_CH_N                 |
 | +-----------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_SCH_STG_3A_STBY_CH_NP1_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_3A_STBY_CH_NP1_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_3A_STBY_CH_NP1_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_3A_STBY_CH_NP1_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_SCH_STG_3A_STBY_CH_N_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_3A_STBY_CH_N_get         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_3A_STBY_CH_N_set   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_3A_STBY_CH_N_get   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x001c3500 + (N) * 4) SCH_STG_3B_ACT_CFG |
 | |     N[0..48]                                       |
 | +----------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_3B_ACT_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_3B_ACT_CH_N                 |
 | +----------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_SCH_STG_3B_ACT_CH_NP1_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_3B_ACT_CH_NP1_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_3B_ACT_CH_NP1_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_3B_ACT_CH_NP1_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_SCH_STG_3B_ACT_CH_N_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_3B_ACT_CH_N_get         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_3B_ACT_CH_N_set   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_3B_ACT_CH_N_get   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x001c3600 + (N) * 4) SCH_STG_3B_STBY_CFG |
 | |     N[0..48]                                        |
 | +-----------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_3B_STBY_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_3B_STBY_CH_N                 |
 | +-----------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_SCH_STG_3B_STBY_CH_NP1_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_3B_STBY_CH_NP1_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_3B_STBY_CH_NP1_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_3B_STBY_CH_NP1_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_SCH_STG_3B_STBY_CH_N_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_3B_STBY_CH_N_get         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_3B_STBY_CH_N_set   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_3B_STBY_CH_N_get   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x001c3700 + (N) * 4) SCH_STG_4_ACT_CFG |
 | |     N[0..48]                                      |
 | +---------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_4_ACT_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_4_ACT_CH_N                 |
 | +---------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_SCH_STG_4_ACT_CH_NP1_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_4_ACT_CH_NP1_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_4_ACT_CH_NP1_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_4_ACT_CH_NP1_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_SCH_STG_4_ACT_CH_N_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_4_ACT_CH_N_get         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_4_ACT_CH_N_set   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_4_ACT_CH_N_get   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x001c3800 + (N) * 4) SCH_STG_4_STBY_CFG |
 | |     N[0..48]                                       |
 | +----------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_4_STBY_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_4_STBY_CH_N                 |
 | +----------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_SCH_STG_4_STBY_CH_NP1_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_4_STBY_CH_NP1_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_4_STBY_CH_NP1_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_4_STBY_CH_NP1_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_ri_rcp_field_SCH_STG_4_STBY_CH_N_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_4_STBY_CH_N_get         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_4_STBY_CH_N_set   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_4_STBY_CH_N_get   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x001c3900 + (N) * 0x04) SCH_STG_N_ACT_CFG |
 | |     N[0..11]                                         |
 | +------------------------------------------------------+
 | | bit  3:0 R/W  SCH_STG_N_ACT_CFG                      |
 | +------------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_SCH_STG_N_ACT_CFG_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_N_ACT_CFG_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_N_ACT_CFG_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_N_ACT_CFG_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x001c3940 + (N) * 0x04) SCH_STG_N_STBY_CFG |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  3:0 R/W  SCH_STG_N_STBY_CFG                      |
 | +-------------------------------------------------------+
 | 
 | void   odu_ri_rcp_field_SCH_STG_N_STBY_CFG_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ri_rcp_field_SCH_STG_N_STBY_CFG_get       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | void   odu_ri_rcp_field_range_SCH_STG_N_STBY_CFG_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ri_rcp_field_range_SCH_STG_N_STBY_CFG_get ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ODU_RI_RCP_IO_H
#define _ODU_RI_RCP_IO_H

#include "coreotn_loc.h"
#include "odu_ri_rcp_regs.h"
#include "odu_ri_rcp_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 odu_ri_rcp_buffer_init               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void                 odu_ri_rcp_buffer_flush              ( odu_ri_rcp_buffer_t *b_ptr )
 | UINT32               odu_ri_rcp_reg_read                  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 odu_ri_rcp_reg_write                 ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 odu_ri_rcp_field_set                 ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 odu_ri_rcp_action_on_write_field_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_ri_rcp_burst_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 odu_ri_rcp_burst_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_poll                      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 odu_ri_rcp_reg_REG_SW_RESET_write                       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_SW_RESET_field_set                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_SW_RESET_read                        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_LOWPWR_write                         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_LOWPWR_field_set                     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_LOWPWR_read                          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_RAM_PWR_MGMT_write                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_RAM_PWR_MGMT_field_set               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_RAM_PWR_MGMT_read                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_GOJF_write                           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_GOJF_field_set                       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_GOJF_read                            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_ETH_TX_EN_write                      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_ETH_TX_EN_field_set                  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_ETH_TX_EN_read                       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_ETH_MAC_OUI_write                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_ETH_MAC_OUI_field_set                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_ETH_MAC_OUI_read                     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_ETH_MAC_DIGI_ID_write                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_ETH_MAC_DIGI_ID_field_set            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_ETH_MAC_DIGI_ID_read                 ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_ETH_ETYPE_write                      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_ETH_ETYPE_field_set                  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_ETH_ETYPE_read                       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_1_RCP_INS_EN_0_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_1_RCP_INS_EN_0_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_1_RCP_INS_EN_0_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_0_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_0_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_0_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_1_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_1_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_1_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_2_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_2_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_2_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_0_write         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_0_field_set     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_0_read          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_1_write         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_1_field_set     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_1_read          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_2_write         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_2_field_set     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_2_read          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_0_write         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_0_field_set     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_0_read          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_1_write         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_1_field_set     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_1_read          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_2_write         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_2_field_set     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_2_read          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_0_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_0_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_0_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_1_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_1_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_1_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_2_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_2_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_2_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_N_RCP_INS_EN_0_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_N_RCP_INS_EN_0_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_N_RCP_INS_EN_0_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_1_APS_INS_EN_0_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_1_APS_INS_EN_0_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_1_APS_INS_EN_0_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_0_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_0_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_0_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_1_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_1_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_1_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_2_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_2_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_2_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_0_write         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_0_field_set     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_0_read          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_1_write         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_1_field_set     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_1_read          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_2_write         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_2_field_set     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_2_read          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_0_write         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_0_field_set     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_0_read          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_1_write         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_1_field_set     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_1_read          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_2_write         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_2_field_set     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_2_read          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_0_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_0_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_0_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_1_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_1_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_1_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_2_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_2_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_2_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_TX_STG_N_APS_INS_EN_0_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_TX_STG_N_APS_INS_EN_0_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_TX_STG_N_APS_INS_EN_0_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_CFC_DBG_SEL_write                        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_DBG_SEL_field_set                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_DBG_SEL_read                         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_DBG_CTRL_write                           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_DBG_CTRL_field_set                       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_DBG_CTRL_read                            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_CFC_IRQ_SEL_write                        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_IRQ_SEL_field_set                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_IRQ_SEL_read                         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_CFC_IRQ_TYPE_SEL_0_write                 ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_IRQ_TYPE_SEL_0_field_set             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_IRQ_TYPE_SEL_0_read                  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_CFC_IRQ_TYPE_SEL_1_write                 ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_IRQ_TYPE_SEL_1_field_set             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_IRQ_TYPE_SEL_1_read                  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_CFC_IRQ_TYPE_SEL_2_write                 ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_IRQ_TYPE_SEL_2_field_set             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_IRQ_TYPE_SEL_2_read                  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_CFC_IPT_ERR_SEL_write                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_IPT_ERR_SEL_field_set                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_IPT_ERR_SEL_read                     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_CFC_STATUS_SEL_write                     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_STATUS_SEL_field_set                 ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_STATUS_SEL_read                      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_1_FIFO_CLRB_0_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_1_FIFO_CLRB_0_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_1_FIFO_CLRB_0_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_1_CFC_CFG_write                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_1_CFC_CFG_field_set               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_1_CFC_CFG_read                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_1_CFC_CFG_VAL_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_1_CFC_CFG_VAL_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_1_CFC_CFG_VAL_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_2_FIFO_CLRB_0_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_2_FIFO_CLRB_0_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_2_FIFO_CLRB_0_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_2_FIFO_CLRB_1_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_2_FIFO_CLRB_1_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_2_FIFO_CLRB_1_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_2_FIFO_CLRB_2_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_2_FIFO_CLRB_2_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_2_FIFO_CLRB_2_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_2_CFC_CFG_write                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_2_CFC_CFG_field_set               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_2_CFC_CFG_read                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_2_CFC_CFG_VAL_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_2_CFC_CFG_VAL_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_2_CFC_CFG_VAL_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_3A_FIFO_CLRB_0_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_3A_FIFO_CLRB_0_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_3A_FIFO_CLRB_0_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_3A_FIFO_CLRB_1_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_3A_FIFO_CLRB_1_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_3A_FIFO_CLRB_1_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_3A_FIFO_CLRB_2_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_3A_FIFO_CLRB_2_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_3A_FIFO_CLRB_2_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_3A_CFC_CFG_write                  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_3A_CFC_CFG_field_set              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_3A_CFC_CFG_read                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_3A_CFC_CFG_VAL_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_3A_CFC_CFG_VAL_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_3A_CFC_CFG_VAL_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_3B_FIFO_CLRB_0_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_3B_FIFO_CLRB_0_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_3B_FIFO_CLRB_0_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_3B_FIFO_CLRB_1_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_3B_FIFO_CLRB_1_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_3B_FIFO_CLRB_1_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_3B_FIFO_CLRB_2_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_3B_FIFO_CLRB_2_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_3B_FIFO_CLRB_2_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_3B_CFC_CFG_write                  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_3B_CFC_CFG_field_set              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_3B_CFC_CFG_read                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_3B_CFC_CFG_VAL_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_3B_CFC_CFG_VAL_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_3B_CFC_CFG_VAL_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_4_FIFO_CLRB_0_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_4_FIFO_CLRB_0_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_4_FIFO_CLRB_0_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_4_FIFO_CLRB_1_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_4_FIFO_CLRB_1_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_4_FIFO_CLRB_1_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_4_FIFO_CLRB_2_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_4_FIFO_CLRB_2_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_4_FIFO_CLRB_2_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_4_CFC_CFG_write                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_4_CFC_CFG_field_set               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_4_CFC_CFG_read                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_4_CFC_CFG_VAL_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_4_CFC_CFG_VAL_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_4_CFC_CFG_VAL_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_N_FIFO_CLRB_0_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_N_FIFO_CLRB_0_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_N_FIFO_CLRB_0_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_N_CFC_CFG_write                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_N_CFC_CFG_field_set               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_N_CFC_CFG_read                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_N_CFC_CFG_VAL_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_N_CFC_CFG_VAL_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_N_CFC_CFG_VAL_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_1_FIFO_CLRB_0_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_1_FIFO_CLRB_0_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_1_FIFO_CLRB_0_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_1_CFC_CFG_write                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_1_CFC_CFG_field_set               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_1_CFC_CFG_read                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_1_CFC_CFG_VAL_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_1_CFC_CFG_VAL_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_1_CFC_CFG_VAL_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_2_FIFO_CLRB_0_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_2_FIFO_CLRB_0_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_2_FIFO_CLRB_0_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_2_FIFO_CLRB_1_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_2_FIFO_CLRB_1_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_2_FIFO_CLRB_1_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_2_FIFO_CLRB_2_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_2_FIFO_CLRB_2_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_2_FIFO_CLRB_2_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_2_CFC_CFG_write                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_2_CFC_CFG_field_set               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_2_CFC_CFG_read                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_2_CFC_CFG_VAL_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_2_CFC_CFG_VAL_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_2_CFC_CFG_VAL_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_3A_FIFO_CLRB_0_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_3A_FIFO_CLRB_0_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_3A_FIFO_CLRB_0_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_3A_FIFO_CLRB_1_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_3A_FIFO_CLRB_1_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_3A_FIFO_CLRB_1_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_3A_FIFO_CLRB_2_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_3A_FIFO_CLRB_2_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_3A_FIFO_CLRB_2_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_3A_CFC_CFG_write                  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_3A_CFC_CFG_field_set              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_3A_CFC_CFG_read                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_3A_CFC_CFG_VAL_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_3A_CFC_CFG_VAL_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_3A_CFC_CFG_VAL_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_3B_FIFO_CLRB_0_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_3B_FIFO_CLRB_0_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_3B_FIFO_CLRB_0_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_3B_FIFO_CLRB_1_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_3B_FIFO_CLRB_1_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_3B_FIFO_CLRB_1_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_3B_FIFO_CLRB_2_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_3B_FIFO_CLRB_2_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_3B_FIFO_CLRB_2_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_3B_CFC_CFG_write                  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_3B_CFC_CFG_field_set              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_3B_CFC_CFG_read                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_3B_CFC_CFG_VAL_write              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_3B_CFC_CFG_VAL_field_set          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_3B_CFC_CFG_VAL_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_4_FIFO_CLRB_0_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_4_FIFO_CLRB_0_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_4_FIFO_CLRB_0_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_4_FIFO_CLRB_1_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_4_FIFO_CLRB_1_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_4_FIFO_CLRB_1_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_4_FIFO_CLRB_2_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_4_FIFO_CLRB_2_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_4_FIFO_CLRB_2_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_4_CFC_CFG_write                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_4_CFC_CFG_field_set               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_4_CFC_CFG_read                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_4_CFC_CFG_VAL_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_4_CFC_CFG_VAL_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_4_CFC_CFG_VAL_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_N_FIFO_CLRB_0_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_N_FIFO_CLRB_0_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_N_FIFO_CLRB_0_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_N_CFC_CFG_write                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_N_CFC_CFG_field_set               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_N_CFC_CFG_read                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_N_CFC_CFG_VAL_write               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_N_CFC_CFG_VAL_field_set           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_N_CFC_CFG_VAL_read                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_SCH_CAL_ENABLE_write                 ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_SCH_CAL_ENABLE_field_set             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_SCH_CAL_ENABLE_read                  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_REG_SCH_CAL_SEL_write                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_REG_SCH_CAL_SEL_field_set                ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_REG_SCH_CAL_SEL_read                     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_1_APS_LEVEL_array_write           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_1_APS_LEVEL_array_field_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_1_APS_LEVEL_array_read            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_N_APS_LEVEL_array_write           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_N_APS_LEVEL_array_field_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_N_APS_LEVEL_array_read            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_1_CFC_IPT_array_write             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_1_CFC_IPT_array_field_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_1_CFC_IPT_array_read              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_N_CFC_IPT_array_write             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_N_CFC_IPT_array_field_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_N_CFC_IPT_array_read              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_1_CFC_IPT_array_write             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_1_CFC_IPT_array_field_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_1_CFC_IPT_array_read              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_N_CFC_IPT_array_write             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_N_CFC_IPT_array_field_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_N_CFC_IPT_array_read              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_SCH_STG_1_ACT_CFG_array_write            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_SCH_STG_1_ACT_CFG_array_field_set        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_SCH_STG_1_ACT_CFG_array_read             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_SCH_STG_1_STBY_CFG_array_write           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_SCH_STG_1_STBY_CFG_array_field_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_SCH_STG_1_STBY_CFG_array_read            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_SCH_STG_N_ACT_CFG_array_write            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_SCH_STG_N_ACT_CFG_array_field_set        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_SCH_STG_N_ACT_CFG_array_read             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_SCH_STG_N_STBY_CFG_array_write           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_SCH_STG_N_STBY_CFG_array_field_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_SCH_STG_N_STBY_CFG_array_read            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_2_APS_LEVEL_array_write           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_2_APS_LEVEL_array_field_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_2_APS_LEVEL_array_read            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_3A_APS_LEVEL_array_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_3A_APS_LEVEL_array_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_3A_APS_LEVEL_array_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_3B_APS_LEVEL_array_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_3B_APS_LEVEL_array_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_3B_APS_LEVEL_array_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_4_APS_LEVEL_array_write           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_4_APS_LEVEL_array_field_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_4_APS_LEVEL_array_read            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_2_CFC_IPT_array_write             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_2_CFC_IPT_array_field_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_2_CFC_IPT_array_read              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_3A_CFC_IPT_array_write            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_3A_CFC_IPT_array_field_set        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_3A_CFC_IPT_array_read             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_3B_CFC_IPT_array_write            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_3B_CFC_IPT_array_field_set        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_3B_CFC_IPT_array_read             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_RX_STG_4_CFC_IPT_array_write             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_RX_STG_4_CFC_IPT_array_field_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_RX_STG_4_CFC_IPT_array_read              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_2_CFC_IPT_array_write             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_2_CFC_IPT_array_field_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_2_CFC_IPT_array_read              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_3A_CFC_IPT_array_write            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_3A_CFC_IPT_array_field_set        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_3A_CFC_IPT_array_read             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_3B_CFC_IPT_array_write            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_3B_CFC_IPT_array_field_set        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_3B_CFC_IPT_array_read             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_TX_STG_4_CFC_IPT_array_write             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_TX_STG_4_CFC_IPT_array_field_set         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_TX_STG_4_CFC_IPT_array_read              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_GEN_IRQ_E_write                          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_GEN_IRQ_E_field_set                      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_GEN_IRQ_E_read                           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_CFC_FIFO_IRQ_EN_0_write                  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_FIFO_IRQ_EN_0_field_set              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_FIFO_IRQ_EN_0_read                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_CFC_FIFO_IRQ_EN_1_write                  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_FIFO_IRQ_EN_1_field_set              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_FIFO_IRQ_EN_1_read                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_CFC_FIFO_IRQ_EN_2_write                  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_FIFO_IRQ_EN_2_field_set              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_FIFO_IRQ_EN_2_read                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_CFC_IPT_ERR_EN_0_write                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_IPT_ERR_EN_0_field_set               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_IPT_ERR_EN_0_read                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_CFC_IPT_ERR_EN_1_write                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_IPT_ERR_EN_1_field_set               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_IPT_ERR_EN_1_read                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void                 odu_ri_rcp_reg_CFC_IPT_ERR_EN_2_write                   ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_IPT_ERR_EN_2_field_set               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_IPT_ERR_EN_2_read                    ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_IRQ_SUMMARY_read                         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_reg_IRQ_SUMMARY_poll                         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ri_rcp_reg_GEN_IRQ_I_write                          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_GEN_IRQ_I_action_on_write_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_GEN_IRQ_I_read                           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_reg_GEN_IRQ_I_poll                           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ri_rcp_reg_CFC_FIFO_IRQ_0_write                     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_FIFO_IRQ_0_action_on_write_field_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_FIFO_IRQ_0_read                      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_reg_CFC_FIFO_IRQ_0_poll                      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ri_rcp_reg_CFC_FIFO_IRQ_1_write                     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_FIFO_IRQ_1_action_on_write_field_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_FIFO_IRQ_1_read                      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_reg_CFC_FIFO_IRQ_1_poll                      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ri_rcp_reg_CFC_FIFO_IRQ_2_write                     ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_FIFO_IRQ_2_action_on_write_field_set ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_FIFO_IRQ_2_read                      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_reg_CFC_FIFO_IRQ_2_poll                      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ri_rcp_reg_CFC_IPT_ERR_0_write                      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_IPT_ERR_0_action_on_write_field_set  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_IPT_ERR_0_read                       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_reg_CFC_IPT_ERR_0_poll                       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ri_rcp_reg_CFC_IPT_ERR_1_write                      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_IPT_ERR_1_action_on_write_field_set  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_IPT_ERR_1_read                       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_reg_CFC_IPT_ERR_1_poll                       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ri_rcp_reg_CFC_IPT_ERR_2_write                      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void                 odu_ri_rcp_reg_CFC_IPT_ERR_2_action_on_write_field_set  ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_CFC_IPT_ERR_2_read                       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ri_rcp_reg_CFC_IPT_ERR_2_poll                       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ri_rcp_reg_HO_ODU_MAC_DA_CFG_array_write            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_HO_ODU_MAC_DA_CFG_array_field_set        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_HO_ODU_MAC_DA_CFG_array_read             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_SCH_STG_2_ACT_CFG_array_write            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_SCH_STG_2_ACT_CFG_array_field_set        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_SCH_STG_2_ACT_CFG_array_read             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_SCH_STG_2_STBY_CFG_array_write           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_SCH_STG_2_STBY_CFG_array_field_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_SCH_STG_2_STBY_CFG_array_read            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_SCH_STG_3A_ACT_CFG_array_write           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_SCH_STG_3A_ACT_CFG_array_field_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_SCH_STG_3A_ACT_CFG_array_read            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_SCH_STG_3A_STBY_CFG_array_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_SCH_STG_3A_STBY_CFG_array_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_SCH_STG_3A_STBY_CFG_array_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_SCH_STG_3B_ACT_CFG_array_write           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_SCH_STG_3B_ACT_CFG_array_field_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_SCH_STG_3B_ACT_CFG_array_read            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_SCH_STG_3B_STBY_CFG_array_write          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_SCH_STG_3B_STBY_CFG_array_field_set      ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_SCH_STG_3B_STBY_CFG_array_read           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_SCH_STG_4_ACT_CFG_array_write            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_SCH_STG_4_ACT_CFG_array_field_set        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_SCH_STG_4_ACT_CFG_array_read             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_SCH_STG_4_STBY_CFG_array_write           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_SCH_STG_4_STBY_CFG_array_field_set       ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_SCH_STG_4_STBY_CFG_array_read            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ri_rcp_reg_LO_ODU_MAC_DA_CFG_array_write            ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ri_rcp_reg_LO_ODU_MAC_DA_CFG_array_field_set        ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ri_rcp_reg_LO_ODU_MAC_DA_CFG_array_read             ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_ri_rcp_reg_DBG_RDATA_read                           ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_CFC_EMPTY_0_read                         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_CFC_EMPTY_1_read                         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_CFC_EMPTY_2_read                         ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_CFC_FULL_0_read                          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_CFC_FULL_1_read                          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_CFC_FULL_2_read                          ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_RX_STG_1_CFC_CFG_STAT_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_RX_STG_2_CFC_CFG_STAT_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_RX_STG_3A_CFC_CFG_STAT_read              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_RX_STG_3B_CFC_CFG_STAT_read              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_RX_STG_4_CFC_CFG_STAT_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_RX_STG_N_CFC_CFG_STAT_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_TX_STG_1_CFC_CFG_STAT_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_TX_STG_2_CFC_CFG_STAT_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_TX_STG_3A_CFC_CFG_STAT_read              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_TX_STG_3B_CFC_CFG_STAT_read              ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_TX_STG_4_CFC_CFG_STAT_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32               odu_ri_rcp_reg_TX_STG_N_CFC_CFG_STAT_read               ( odu_ri_rcp_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ODU_RI_RCP_IO_H */
