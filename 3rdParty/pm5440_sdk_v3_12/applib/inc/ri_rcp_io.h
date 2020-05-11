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
 *     and register accessor functions for the ri_rcp block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing ri_rcp io functions is:
 | {
 |     ri_rcp_buffer_t b_ptr[1];
 |     ri_rcp_buffer_init( b_ptr, h_ptr );
 |     value = ri_rcp_field_<FIELD>_get( b_ptr, h_ptr );
 |     ri_rcp_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     ri_rcp_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = ri_rcp_field_<FIELD>_get( NULL, h_ptr );
 | ri_rcp_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------+
 | | Register 0x00000000 REG_SW_RESET |
 | +----------------------------------+
 | | bit  1 R/W  REG_SOFT_SW_RESET    |
 | | bit  0 R/W  REG_SW_RESET         |
 | +----------------------------------+
 | 
 | void   ri_rcp_field_REG_SOFT_SW_RESET_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_SOFT_SW_RESET_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_REG_SW_RESET_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_SW_RESET_get      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +--------------------------------+
 | | Register 0x00000004 REG_LOWPWR |
 | +--------------------------------+
 | | bit  0 R/W  REG_LOWPWR         |
 | +--------------------------------+
 | 
 | void   ri_rcp_field_REG_LOWPWR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_LOWPWR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x0000000c REG_RAM_PWR_MGMT |
 | +--------------------------------------+
 | | bit  0 R/W  REG_RAM_LOWPWR           |
 | +--------------------------------------+
 | 
 | void   ri_rcp_field_REG_RAM_LOWPWR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_RAM_LOWPWR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00000010 IRQ_SUMMARY |
 | +---------------------------------+
 | | bit  12:0 R  INTERRUPT_SUMMARY  |
 | +---------------------------------+
 | 
 | UINT32               ri_rcp_field_INTERRUPT_SUMMARY_get        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | UINT32               ri_rcp_field_range_INTERRUPT_SUMMARY_get  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE ri_rcp_field_range_INTERRUPT_SUMMARY_poll ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE ri_rcp_field_INTERRUPT_SUMMARY_poll       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x0000001c REG_GOJF               |
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
 | void   ri_rcp_field_REG_ING_FIFO_MIN_SLOT_FREE_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_ING_FIFO_MIN_SLOT_FREE_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_REG_ING_FIFO_MIN_SLOT_FREE_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_ING_FIFO_MIN_SLOT_FREE_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_REG_ING_FIFO_MIN_LVL_set             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_ING_FIFO_MIN_LVL_get             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_REG_ING_FIFO_MIN_LVL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_ING_FIFO_MIN_LVL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_REG_DROP_PDU_WO_DFEC_set             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_DROP_PDU_WO_DFEC_get             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_REG_ETH_RX_10B_SWIZZLE_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_ETH_RX_10B_SWIZZLE_get           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_REG_ETH_TX_10B_SWIZZLE_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_ETH_TX_10B_SWIZZLE_get           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_REG_ETH_RX_8B_SWIZZLE_set            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_ETH_RX_8B_SWIZZLE_get            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_REG_ETH_TX_8B_SWIZZLE_set            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_ETH_TX_8B_SWIZZLE_get            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x00000020 GEN_IRQ_I |
 | +-------------------------------+
 | | bit  0 R/W  FEGE_TIP_I        |
 | +-------------------------------+
 | 
 | void                 ri_rcp_field_FEGE_TIP_I_set_to_clear ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32               ri_rcp_field_FEGE_TIP_I_get          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ri_rcp_field_FEGE_TIP_I_poll         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------+
 | | Register 0x00000040 GEN_IRQ_E |
 | +-------------------------------+
 | | bit  0 R/W  FEGE_TIP_E        |
 | +-------------------------------+
 | 
 | void   ri_rcp_field_FEGE_TIP_E_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_FEGE_TIP_E_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x00000060 REG_ETH_TX_EN |
 | +-----------------------------------+
 | | bit  0 R/W  REG_ETH_TX_EN         |
 | +-----------------------------------+
 | 
 | void   ri_rcp_field_REG_ETH_TX_EN_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_ETH_TX_EN_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x00000068 REG_ETH_MAC_OUI |
 | +-------------------------------------+
 | | bit  23:0 R/W  REG_ETH_MAC_OUI      |
 | +-------------------------------------+
 | 
 | void   ri_rcp_field_REG_ETH_MAC_OUI_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_ETH_MAC_OUI_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_REG_ETH_MAC_OUI_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_ETH_MAC_OUI_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0000006c REG_ETH_MAC_DIGI_ID |
 | +-----------------------------------------+
 | | bit  7:0 R/W  REG_ETH_MAC_DIGI_ID       |
 | +-----------------------------------------+
 | 
 | void   ri_rcp_field_REG_ETH_MAC_DIGI_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_ETH_MAC_DIGI_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_REG_ETH_MAC_DIGI_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_ETH_MAC_DIGI_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000070 REG_ETH_ETYPE |
 | +-----------------------------------+
 | | bit  15:0 R/W  REG_ETH_ETYPE      |
 | +-----------------------------------+
 | 
 | void   ri_rcp_field_REG_ETH_ETYPE_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_ETH_ETYPE_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_REG_ETH_ETYPE_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_ETH_ETYPE_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x00000080 + (N) * 4) HO_ODU_MAC_DA_CFG |
 | |     N[0..31]                                      |
 | +---------------------------------------------------+
 | | bit  23:16 R/W  HO_ODU_DST_DIGI                   |
 | | bit  10:8  R/W  HO_ODU_DST_FRM                    |
 | | bit  6:0   R/W  HO_ODU_DST_CHANNEL                |
 | +---------------------------------------------------+
 | 
 | void   ri_rcp_field_HO_ODU_DST_DIGI_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_HO_ODU_DST_DIGI_get          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_HO_ODU_DST_DIGI_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_HO_ODU_DST_DIGI_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_HO_ODU_DST_FRM_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_HO_ODU_DST_FRM_get           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_HO_ODU_DST_FRM_set     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_HO_ODU_DST_FRM_get     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_HO_ODU_DST_CHANNEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_HO_ODU_DST_CHANNEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_HO_ODU_DST_CHANNEL_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_HO_ODU_DST_CHANNEL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x00000100 + (N) * 4) LO_ODU_MAC_DA_CFG |
 | |     N[0..511]                                     |
 | +---------------------------------------------------+
 | | bit  23:16 R/W  LO_ODU_DST_DIGI                   |
 | | bit  10:8  R/W  LO_ODU_DST_FRM                    |
 | | bit  6:0   R/W  LO_ODU_DST_CHANNEL                |
 | +---------------------------------------------------+
 | 
 | void   ri_rcp_field_LO_ODU_DST_DIGI_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_LO_ODU_DST_DIGI_get          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_LO_ODU_DST_DIGI_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_LO_ODU_DST_DIGI_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_LO_ODU_DST_FRM_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_LO_ODU_DST_FRM_get           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_LO_ODU_DST_FRM_set     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_LO_ODU_DST_FRM_get     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_LO_ODU_DST_CHANNEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_LO_ODU_DST_CHANNEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_LO_ODU_DST_CHANNEL_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_LO_ODU_DST_CHANNEL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000b00 REG_TX_STG_1_RCP_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  11:0 R/W  REG_TX_STG_1_RCP_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   ri_rcp_field_REG_TX_STG_1_RCP_INS_EN_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_TX_STG_1_RCP_INS_EN_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_REG_TX_STG_1_RCP_INS_EN_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_TX_STG_1_RCP_INS_EN_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000b10 REG_TX_STG_2_RCP_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_2_RCP_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   ri_rcp_lfield_REG_TX_STG_2_RCP_INS_EN_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_REG_TX_STG_2_RCP_INS_EN_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_REG_TX_STG_2_RCP_INS_EN_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_REG_TX_STG_2_RCP_INS_EN_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000b20 REG_TX_STG_3A_RCP_INS_EN_0 |
 | +------------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_3A_RCP_INS_EN        |
 | +------------------------------------------------+
 | 
 | void   ri_rcp_lfield_REG_TX_STG_3A_RCP_INS_EN_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_REG_TX_STG_3A_RCP_INS_EN_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_REG_TX_STG_3A_RCP_INS_EN_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_REG_TX_STG_3A_RCP_INS_EN_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000b30 REG_TX_STG_3B_RCP_INS_EN_0 |
 | +------------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_3B_RCP_INS_EN        |
 | +------------------------------------------------+
 | 
 | void   ri_rcp_lfield_REG_TX_STG_3B_RCP_INS_EN_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_REG_TX_STG_3B_RCP_INS_EN_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_REG_TX_STG_3B_RCP_INS_EN_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_REG_TX_STG_3B_RCP_INS_EN_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000b40 REG_TX_STG_4_RCP_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_4_RCP_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   ri_rcp_lfield_REG_TX_STG_4_RCP_INS_EN_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_REG_TX_STG_4_RCP_INS_EN_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_REG_TX_STG_4_RCP_INS_EN_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_REG_TX_STG_4_RCP_INS_EN_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000b50 REG_TX_STG_N_RCP_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  11:0 R/W  REG_TX_STG_N_RCP_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   ri_rcp_field_REG_TX_STG_N_RCP_INS_EN_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_TX_STG_N_RCP_INS_EN_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_REG_TX_STG_N_RCP_INS_EN_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_TX_STG_N_RCP_INS_EN_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000b80 REG_TX_STG_1_APS_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  11:0 R/W  REG_TX_STG_1_APS_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   ri_rcp_field_REG_TX_STG_1_APS_INS_EN_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_TX_STG_1_APS_INS_EN_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_REG_TX_STG_1_APS_INS_EN_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_TX_STG_1_APS_INS_EN_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000b90 REG_TX_STG_2_APS_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_2_APS_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   ri_rcp_lfield_REG_TX_STG_2_APS_INS_EN_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_REG_TX_STG_2_APS_INS_EN_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_REG_TX_STG_2_APS_INS_EN_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_REG_TX_STG_2_APS_INS_EN_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000ba0 REG_TX_STG_3A_APS_INS_EN_0 |
 | +------------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_3A_APS_INS_EN        |
 | +------------------------------------------------+
 | 
 | void   ri_rcp_lfield_REG_TX_STG_3A_APS_INS_EN_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_REG_TX_STG_3A_APS_INS_EN_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_REG_TX_STG_3A_APS_INS_EN_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_REG_TX_STG_3A_APS_INS_EN_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000bb0 REG_TX_STG_3B_APS_INS_EN_0 |
 | +------------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_3B_APS_INS_EN        |
 | +------------------------------------------------+
 | 
 | void   ri_rcp_lfield_REG_TX_STG_3B_APS_INS_EN_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_REG_TX_STG_3B_APS_INS_EN_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_REG_TX_STG_3B_APS_INS_EN_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_REG_TX_STG_3B_APS_INS_EN_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000bc0 REG_TX_STG_4_APS_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  REG_TX_STG_4_APS_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   ri_rcp_lfield_REG_TX_STG_4_APS_INS_EN_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_REG_TX_STG_4_APS_INS_EN_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_REG_TX_STG_4_APS_INS_EN_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_REG_TX_STG_4_APS_INS_EN_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000bd0 REG_TX_STG_N_APS_INS_EN_0 |
 | +-----------------------------------------------+
 | | bit  11:0 R/W  REG_TX_STG_N_APS_INS_EN        |
 | +-----------------------------------------------+
 | 
 | void   ri_rcp_field_REG_TX_STG_N_APS_INS_EN_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_TX_STG_N_APS_INS_EN_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_REG_TX_STG_N_APS_INS_EN_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_TX_STG_N_APS_INS_EN_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000c00 + (N) * 0x04) RX_STG_1_APS_LEVEL |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  2:0 R/W  REG_RX_STG_1_APS_LEVEL                  |
 | +-------------------------------------------------------+
 | 
 | void   ri_rcp_field_REG_RX_STG_1_APS_LEVEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_REG_RX_STG_1_APS_LEVEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_REG_RX_STG_1_APS_LEVEL_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_RX_STG_1_APS_LEVEL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000c80 + (N) * 0x04) RX_STG_2_APS_LEVEL |
 | |     N[0..95]                                          |
 | +-------------------------------------------------------+
 | | bit  2:0 R/W  REG_RX_STG_2_APS_LEVEL                  |
 | +-------------------------------------------------------+
 | 
 | void   ri_rcp_field_REG_RX_STG_2_APS_LEVEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_REG_RX_STG_2_APS_LEVEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_REG_RX_STG_2_APS_LEVEL_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_RX_STG_2_APS_LEVEL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000e00 + (N) * 0x04) RX_STG_3A_APS_LEVEL |
 | |     N[0..95]                                           |
 | +--------------------------------------------------------+
 | | bit  2:0 R/W  REG_RX_STG_3A_APS_LEVEL                  |
 | +--------------------------------------------------------+
 | 
 | void   ri_rcp_field_REG_RX_STG_3A_APS_LEVEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_REG_RX_STG_3A_APS_LEVEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_REG_RX_STG_3A_APS_LEVEL_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_RX_STG_3A_APS_LEVEL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00000f80 + (N) * 0x04) RX_STG_3B_APS_LEVEL |
 | |     N[0..95]                                           |
 | +--------------------------------------------------------+
 | | bit  2:0 R/W  REG_RX_STG_3B_APS_LEVEL                  |
 | +--------------------------------------------------------+
 | 
 | void   ri_rcp_field_REG_RX_STG_3B_APS_LEVEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_REG_RX_STG_3B_APS_LEVEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_REG_RX_STG_3B_APS_LEVEL_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_RX_STG_3B_APS_LEVEL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00001100 + (N) * 0x04) RX_STG_4_APS_LEVEL |
 | |     N[0..95]                                          |
 | +-------------------------------------------------------+
 | | bit  2:0 R/W  REG_RX_STG_4_APS_LEVEL                  |
 | +-------------------------------------------------------+
 | 
 | void   ri_rcp_field_REG_RX_STG_4_APS_LEVEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_REG_RX_STG_4_APS_LEVEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_REG_RX_STG_4_APS_LEVEL_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_RX_STG_4_APS_LEVEL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00001280 + (N) * 0x04) RX_STG_N_APS_LEVEL |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  2:0 R/W  REG_RX_STG_N_APS_LEVEL                  |
 | +-------------------------------------------------------+
 | 
 | void   ri_rcp_field_REG_RX_STG_N_APS_LEVEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_REG_RX_STG_N_APS_LEVEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_REG_RX_STG_N_APS_LEVEL_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_RX_STG_N_APS_LEVEL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00001300 CFC_DBG_SEL |
 | +---------------------------------+
 | | bit  4   R/W  CFC_DBG_SRC_SEL   |
 | | bit  2:0 R/W  CFC_DBG_STG_SEL   |
 | +---------------------------------+
 | 
 | void   ri_rcp_field_CFC_DBG_SRC_SEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_CFC_DBG_SRC_SEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_CFC_DBG_STG_SEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_CFC_DBG_STG_SEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_CFC_DBG_STG_SEL_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_CFC_DBG_STG_SEL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------+
 | | Register 0x00001304 DBG_CTRL |
 | +------------------------------+
 | | bit  16   R/W  DBG_TRIG      |
 | | bit  10:0 R/W  DBG_RADDR     |
 | +------------------------------+
 | 
 | void   ri_rcp_field_DBG_RADDR_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_DBG_RADDR_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_DBG_RADDR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_DBG_RADDR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_DBG_TRIG_set        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_DBG_TRIG_get        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x00001308 DBG_RDATA |
 | +-------------------------------+
 | | bit  31:0 R  DBG_RDATA        |
 | +-------------------------------+
 | 
 | UINT32 ri_rcp_field_DBG_RDATA_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | UINT32 ri_rcp_field_range_DBG_RDATA_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00001320 CFC_IRQ_SEL   |
 | +-----------------------------------+
 | | bit  4   R/W  REG_CFC_IRQ_SRC_SEL |
 | | bit  2:0 R/W  REG_CFC_IRQ_STG_SEL |
 | +-----------------------------------+
 | 
 | void   ri_rcp_field_REG_CFC_IRQ_SRC_SEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_CFC_IRQ_SRC_SEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_REG_CFC_IRQ_STG_SEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_CFC_IRQ_STG_SEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_REG_CFC_IRQ_STG_SEL_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_CFC_IRQ_STG_SEL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00001324 CFC_IRQ_TYPE_SEL_0 |
 | +----------------------------------------+
 | | bit  31:0 R/W  REG_CFC_IRQ_TYPE_SEL    |
 | +----------------------------------------+
 | 
 | void   ri_rcp_lfield_REG_CFC_IRQ_TYPE_SEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_REG_CFC_IRQ_TYPE_SEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_REG_CFC_IRQ_TYPE_SEL_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_REG_CFC_IRQ_TYPE_SEL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00001330 CFC_FIFO_IRQ_0 |
 | +------------------------------------+
 | | bit  31:0 R/W  CFC_FIFO_IRQ_I      |
 | +------------------------------------+
 | 
 | void                 ri_rcp_lfield_CFC_FIFO_IRQ_I_set_to_clear       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void                 ri_rcp_lfield_CFC_FIFO_IRQ_I_get                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void                 ri_rcp_lfield_range_CFC_FIFO_IRQ_I_set_to_clear ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               ri_rcp_lfield_range_CFC_FIFO_IRQ_I_get          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE ri_rcp_lfield_range_CFC_FIFO_IRQ_I_poll         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE ri_rcp_lfield_CFC_FIFO_IRQ_I_poll               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x00001340 CFC_IPT_ERR_SEL   |
 | +---------------------------------------+
 | | bit  4   R/W  REG_CFC_IPT_ERR_SRC_SEL |
 | | bit  2:0 R/W  REG_CFC_IPT_ERR_STG_SEL |
 | +---------------------------------------+
 | 
 | void   ri_rcp_field_REG_CFC_IPT_ERR_SRC_SEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_CFC_IPT_ERR_SRC_SEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_REG_CFC_IPT_ERR_STG_SEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_CFC_IPT_ERR_STG_SEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_REG_CFC_IPT_ERR_STG_SEL_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_CFC_IPT_ERR_STG_SEL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00001344 CFC_IPT_ERR_0 |
 | +-----------------------------------+
 | | bit  31:0 R/W  CFC_IPT_ERR_I      |
 | +-----------------------------------+
 | 
 | void                 ri_rcp_lfield_CFC_IPT_ERR_I_set_to_clear       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void                 ri_rcp_lfield_CFC_IPT_ERR_I_get                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void                 ri_rcp_lfield_range_CFC_IPT_ERR_I_set_to_clear ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               ri_rcp_lfield_range_CFC_IPT_ERR_I_get          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE ri_rcp_lfield_range_CFC_IPT_ERR_I_poll         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE ri_rcp_lfield_CFC_IPT_ERR_I_poll               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x00001360 CFC_FIFO_IRQ_EN_0 |
 | +---------------------------------------+
 | | bit  31:0 R/W  CFC_FIFO_IRQ_E         |
 | +---------------------------------------+
 | 
 | void   ri_rcp_lfield_CFC_FIFO_IRQ_E_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_CFC_FIFO_IRQ_E_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_CFC_FIFO_IRQ_E_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_CFC_FIFO_IRQ_E_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00001380 CFC_IPT_ERR_EN_0 |
 | +--------------------------------------+
 | | bit  31:0 R/W  CFC_IPT_ERR_E         |
 | +--------------------------------------+
 | 
 | void   ri_rcp_lfield_CFC_IPT_ERR_E_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_CFC_IPT_ERR_E_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_CFC_IPT_ERR_E_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_CFC_IPT_ERR_E_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x000013c0 CFC_STATUS_SEL   |
 | +--------------------------------------+
 | | bit  4   R/W  REG_CFC_STATUS_SRC_SEL |
 | | bit  2:0 R/W  REG_CFC_STATUS_STG_SEL |
 | +--------------------------------------+
 | 
 | void   ri_rcp_field_REG_CFC_STATUS_SRC_SEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_CFC_STATUS_SRC_SEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_REG_CFC_STATUS_STG_SEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_CFC_STATUS_STG_SEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_REG_CFC_STATUS_STG_SEL_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_CFC_STATUS_STG_SEL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x000013c4 CFC_EMPTY_0 |
 | +---------------------------------+
 | | bit  31:0 R  CFC_EMPTY          |
 | +---------------------------------+
 | 
 | void   ri_rcp_lfield_CFC_EMPTY_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 ri_rcp_lfield_range_CFC_EMPTY_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x000013d0 CFC_FULL_0 |
 | +--------------------------------+
 | | bit  31:0 R  CFC_FULL          |
 | +--------------------------------+
 | 
 | void   ri_rcp_lfield_CFC_FULL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 ri_rcp_lfield_range_CFC_FULL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00001800 RX_STG_1_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  11:0 R/W  RX_STG_1_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_1_FIFO_CLRB_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_1_FIFO_CLRB_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_RX_STG_1_FIFO_CLRB_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_1_FIFO_CLRB_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00001820 RX_STG_1_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  RX_STG_1_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  RX_STG_1_CFC_END_ADDR_CHG_REQ   |
 | | bit  19:16 R/W  RX_STG_1_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_RX_STG_1_CFC_END_ADDR_CHG_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_1_CFC_END_ADDR_CHG_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_ABORT_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_ABORT_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_REQ_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_REQ_get      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00001824 RX_STG_1_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  RX_STG_1_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 ri_rcp_field_RX_STG_1_CFC_END_ADDR_CHG_SUCCESS_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00001828 RX_STG_1_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  4:0 R/W  RX_STG_1_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_1_CFC_END_ADDR_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_1_CFC_END_ADDR_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_RX_STG_1_CFC_END_ADDR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_1_CFC_END_ADDR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00001840 + (N) * 0x04) RX_STG_1_CFC_IPT |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  RX_STG_1_CFC_IPT_VALID              |
 | | bit  19:16 R/W  RX_STG_1_CFC_IPT_ID                 |
 | | bit  3:0   R/W  RX_STG_1_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_1_CFC_IPT_VALID_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_1_CFC_IPT_VALID_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_RX_STG_1_CFC_IPT_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_1_CFC_IPT_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_RX_STG_1_CFC_IPT_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_1_CFC_IPT_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_RX_STG_1_CFC_IPT_VA_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_1_CFC_IPT_VA_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_RX_STG_1_CFC_IPT_VA_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_1_CFC_IPT_VA_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00001a00 RX_STG_2_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  31:0 R/W  RX_STG_2_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   ri_rcp_lfield_RX_STG_2_FIFO_CLRB_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_RX_STG_2_FIFO_CLRB_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_RX_STG_2_FIFO_CLRB_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_RX_STG_2_FIFO_CLRB_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00001a20 RX_STG_2_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  RX_STG_2_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  RX_STG_2_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  RX_STG_2_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_RX_STG_2_CFC_END_ADDR_CHG_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_2_CFC_END_ADDR_CHG_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_ABORT_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_ABORT_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_REQ_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_REQ_get      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00001a24 RX_STG_2_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  RX_STG_2_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 ri_rcp_field_RX_STG_2_CFC_END_ADDR_CHG_SUCCESS_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00001a28 RX_STG_2_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  7:0 R/W  RX_STG_2_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_2_CFC_END_ADDR_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_2_CFC_END_ADDR_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_RX_STG_2_CFC_END_ADDR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_2_CFC_END_ADDR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00001a40 + (N) * 0x04) RX_STG_2_CFC_IPT |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  RX_STG_2_CFC_IPT_VALID              |
 | | bit  22:16 R/W  RX_STG_2_CFC_IPT_ID                 |
 | | bit  6:0   R/W  RX_STG_2_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_2_CFC_IPT_VALID_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_2_CFC_IPT_VALID_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_RX_STG_2_CFC_IPT_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_2_CFC_IPT_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_RX_STG_2_CFC_IPT_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_2_CFC_IPT_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_RX_STG_2_CFC_IPT_VA_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_2_CFC_IPT_VA_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_RX_STG_2_CFC_IPT_VA_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_2_CFC_IPT_VA_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00001c00 RX_STG_3A_FIFO_CLRB_0 |
 | +-------------------------------------------+
 | | bit  31:0 R/W  RX_STG_3A_FIFO_CLRB        |
 | +-------------------------------------------+
 | 
 | void   ri_rcp_lfield_RX_STG_3A_FIFO_CLRB_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_RX_STG_3A_FIFO_CLRB_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_RX_STG_3A_FIFO_CLRB_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_RX_STG_3A_FIFO_CLRB_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00001c20 RX_STG_3A_CFC_CFG            |
 | +--------------------------------------------------+
 | | bit  28    R/W  RX_STG_3A_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  RX_STG_3A_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  RX_STG_3A_CFC_END_ADDR_CHG_ID    |
 | +--------------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_RX_STG_3A_CFC_END_ADDR_CHG_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_3A_CFC_END_ADDR_CHG_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_ABORT_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_ABORT_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_REQ_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_REQ_get      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00001c24 RX_STG_3A_CFC_CFG_STAT   |
 | +----------------------------------------------+
 | | bit  0 R  RX_STG_3A_CFC_END_ADDR_CHG_SUCCESS |
 | +----------------------------------------------+
 | 
 | UINT32 ri_rcp_field_RX_STG_3A_CFC_END_ADDR_CHG_SUCCESS_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00001c28 RX_STG_3A_CFC_CFG_VAL |
 | +-------------------------------------------+
 | | bit  7:0 R/W  RX_STG_3A_CFC_END_ADDR      |
 | +-------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_3A_CFC_END_ADDR_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_3A_CFC_END_ADDR_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_RX_STG_3A_CFC_END_ADDR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_3A_CFC_END_ADDR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00001c40 + (N) * 0x04) RX_STG_3A_CFC_IPT |
 | |     N[0..95]                                         |
 | +------------------------------------------------------+
 | | bit  24    R/W  RX_STG_3A_CFC_IPT_VALID              |
 | | bit  22:16 R/W  RX_STG_3A_CFC_IPT_ID                 |
 | | bit  6:0   R/W  RX_STG_3A_CFC_IPT_VA                 |
 | +------------------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_3A_CFC_IPT_VALID_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_3A_CFC_IPT_VALID_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_RX_STG_3A_CFC_IPT_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_3A_CFC_IPT_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_RX_STG_3A_CFC_IPT_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_3A_CFC_IPT_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_RX_STG_3A_CFC_IPT_VA_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_3A_CFC_IPT_VA_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_RX_STG_3A_CFC_IPT_VA_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_3A_CFC_IPT_VA_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00001e00 RX_STG_3B_FIFO_CLRB_0 |
 | +-------------------------------------------+
 | | bit  31:0 R/W  RX_STG_3B_FIFO_CLRB        |
 | +-------------------------------------------+
 | 
 | void   ri_rcp_lfield_RX_STG_3B_FIFO_CLRB_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_RX_STG_3B_FIFO_CLRB_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_RX_STG_3B_FIFO_CLRB_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_RX_STG_3B_FIFO_CLRB_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00001e20 RX_STG_3B_CFC_CFG            |
 | +--------------------------------------------------+
 | | bit  28    R/W  RX_STG_3B_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  RX_STG_3B_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  RX_STG_3B_CFC_END_ADDR_CHG_ID    |
 | +--------------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_RX_STG_3B_CFC_END_ADDR_CHG_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_3B_CFC_END_ADDR_CHG_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_ABORT_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_ABORT_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_REQ_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_REQ_get      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00001e24 RX_STG_3B_CFC_CFG_STAT   |
 | +----------------------------------------------+
 | | bit  0 R  RX_STG_3B_CFC_END_ADDR_CHG_SUCCESS |
 | +----------------------------------------------+
 | 
 | UINT32 ri_rcp_field_RX_STG_3B_CFC_END_ADDR_CHG_SUCCESS_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00001e28 RX_STG_3B_CFC_CFG_VAL |
 | +-------------------------------------------+
 | | bit  7:0 R/W  RX_STG_3B_CFC_END_ADDR      |
 | +-------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_3B_CFC_END_ADDR_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_3B_CFC_END_ADDR_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_RX_STG_3B_CFC_END_ADDR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_3B_CFC_END_ADDR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00001e40 + (N) * 0x04) RX_STG_3B_CFC_IPT |
 | |     N[0..95]                                         |
 | +------------------------------------------------------+
 | | bit  24    R/W  RX_STG_3B_CFC_IPT_VALID              |
 | | bit  22:16 R/W  RX_STG_3B_CFC_IPT_ID                 |
 | | bit  6:0   R/W  RX_STG_3B_CFC_IPT_VA                 |
 | +------------------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_3B_CFC_IPT_VALID_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_3B_CFC_IPT_VALID_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_RX_STG_3B_CFC_IPT_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_3B_CFC_IPT_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_RX_STG_3B_CFC_IPT_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_3B_CFC_IPT_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_RX_STG_3B_CFC_IPT_VA_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_3B_CFC_IPT_VA_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_RX_STG_3B_CFC_IPT_VA_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_3B_CFC_IPT_VA_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00002000 RX_STG_4_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  31:0 R/W  RX_STG_4_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   ri_rcp_lfield_RX_STG_4_FIFO_CLRB_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_RX_STG_4_FIFO_CLRB_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_RX_STG_4_FIFO_CLRB_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_RX_STG_4_FIFO_CLRB_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00002020 RX_STG_4_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  RX_STG_4_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  RX_STG_4_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  RX_STG_4_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_RX_STG_4_CFC_END_ADDR_CHG_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_4_CFC_END_ADDR_CHG_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_ABORT_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_ABORT_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_REQ_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_REQ_get      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00002024 RX_STG_4_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  RX_STG_4_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 ri_rcp_field_RX_STG_4_CFC_END_ADDR_CHG_SUCCESS_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00002028 RX_STG_4_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  7:0 R/W  RX_STG_4_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_4_CFC_END_ADDR_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_4_CFC_END_ADDR_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_RX_STG_4_CFC_END_ADDR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_4_CFC_END_ADDR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00002040 + (N) * 0x04) RX_STG_4_CFC_IPT |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  RX_STG_4_CFC_IPT_VALID              |
 | | bit  22:16 R/W  RX_STG_4_CFC_IPT_ID                 |
 | | bit  6:0   R/W  RX_STG_4_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_4_CFC_IPT_VALID_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_4_CFC_IPT_VALID_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_RX_STG_4_CFC_IPT_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_4_CFC_IPT_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_RX_STG_4_CFC_IPT_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_4_CFC_IPT_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_RX_STG_4_CFC_IPT_VA_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_4_CFC_IPT_VA_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_RX_STG_4_CFC_IPT_VA_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_4_CFC_IPT_VA_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00002200 RX_STG_N_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  11:0 R/W  RX_STG_N_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_N_FIFO_CLRB_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_N_FIFO_CLRB_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_RX_STG_N_FIFO_CLRB_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_N_FIFO_CLRB_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00002220 RX_STG_N_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  RX_STG_N_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  RX_STG_N_CFC_END_ADDR_CHG_REQ   |
 | | bit  19:16 R/W  RX_STG_N_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_RX_STG_N_CFC_END_ADDR_CHG_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_N_CFC_END_ADDR_CHG_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_ABORT_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_ABORT_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_REQ_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_REQ_get      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00002224 RX_STG_N_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  RX_STG_N_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 ri_rcp_field_RX_STG_N_CFC_END_ADDR_CHG_SUCCESS_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00002228 RX_STG_N_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  4:0 R/W  RX_STG_N_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_N_CFC_END_ADDR_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_N_CFC_END_ADDR_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_RX_STG_N_CFC_END_ADDR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_N_CFC_END_ADDR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00002240 + (N) * 0x04) RX_STG_N_CFC_IPT |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  RX_STG_N_CFC_IPT_VALID              |
 | | bit  19:16 R/W  RX_STG_N_CFC_IPT_ID                 |
 | | bit  3:0   R/W  RX_STG_N_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   ri_rcp_field_RX_STG_N_CFC_IPT_VALID_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_N_CFC_IPT_VALID_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_RX_STG_N_CFC_IPT_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_N_CFC_IPT_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_RX_STG_N_CFC_IPT_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_N_CFC_IPT_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_RX_STG_N_CFC_IPT_VA_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_RX_STG_N_CFC_IPT_VA_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_RX_STG_N_CFC_IPT_VA_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_RX_STG_N_CFC_IPT_VA_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00002400 TX_STG_1_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  11:0 R/W  TX_STG_1_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_1_FIFO_CLRB_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_1_FIFO_CLRB_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_TX_STG_1_FIFO_CLRB_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_1_FIFO_CLRB_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00002420 TX_STG_1_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  TX_STG_1_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  TX_STG_1_CFC_END_ADDR_CHG_REQ   |
 | | bit  19:16 R/W  TX_STG_1_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_TX_STG_1_CFC_END_ADDR_CHG_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_1_CFC_END_ADDR_CHG_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_ABORT_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_ABORT_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_REQ_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_REQ_get      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00002424 TX_STG_1_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  TX_STG_1_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 ri_rcp_field_TX_STG_1_CFC_END_ADDR_CHG_SUCCESS_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00002428 TX_STG_1_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  4:0 R/W  TX_STG_1_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_1_CFC_END_ADDR_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_1_CFC_END_ADDR_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_TX_STG_1_CFC_END_ADDR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_1_CFC_END_ADDR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00002440 + (N) * 0x04) TX_STG_1_CFC_IPT |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  TX_STG_1_CFC_IPT_VALID              |
 | | bit  19:16 R/W  TX_STG_1_CFC_IPT_ID                 |
 | | bit  3:0   R/W  TX_STG_1_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_1_CFC_IPT_VALID_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_1_CFC_IPT_VALID_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_TX_STG_1_CFC_IPT_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_1_CFC_IPT_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_TX_STG_1_CFC_IPT_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_1_CFC_IPT_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_TX_STG_1_CFC_IPT_VA_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_1_CFC_IPT_VA_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_TX_STG_1_CFC_IPT_VA_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_1_CFC_IPT_VA_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00002600 TX_STG_2_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  31:0 R/W  TX_STG_2_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   ri_rcp_lfield_TX_STG_2_FIFO_CLRB_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_TX_STG_2_FIFO_CLRB_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_TX_STG_2_FIFO_CLRB_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_TX_STG_2_FIFO_CLRB_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00002620 TX_STG_2_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  TX_STG_2_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  TX_STG_2_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  TX_STG_2_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_TX_STG_2_CFC_END_ADDR_CHG_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_2_CFC_END_ADDR_CHG_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_ABORT_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_ABORT_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_REQ_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_REQ_get      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00002624 TX_STG_2_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  TX_STG_2_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 ri_rcp_field_TX_STG_2_CFC_END_ADDR_CHG_SUCCESS_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00002628 TX_STG_2_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  7:0 R/W  TX_STG_2_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_2_CFC_END_ADDR_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_2_CFC_END_ADDR_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_TX_STG_2_CFC_END_ADDR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_2_CFC_END_ADDR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00002640 + (N) * 0x04) TX_STG_2_CFC_IPT |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  TX_STG_2_CFC_IPT_VALID              |
 | | bit  22:16 R/W  TX_STG_2_CFC_IPT_ID                 |
 | | bit  6:0   R/W  TX_STG_2_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_2_CFC_IPT_VALID_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_2_CFC_IPT_VALID_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_TX_STG_2_CFC_IPT_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_2_CFC_IPT_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_TX_STG_2_CFC_IPT_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_2_CFC_IPT_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_TX_STG_2_CFC_IPT_VA_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_2_CFC_IPT_VA_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_TX_STG_2_CFC_IPT_VA_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_2_CFC_IPT_VA_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00002800 TX_STG_3A_FIFO_CLRB_0 |
 | +-------------------------------------------+
 | | bit  31:0 R/W  TX_STG_3A_FIFO_CLRB        |
 | +-------------------------------------------+
 | 
 | void   ri_rcp_lfield_TX_STG_3A_FIFO_CLRB_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_TX_STG_3A_FIFO_CLRB_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_TX_STG_3A_FIFO_CLRB_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_TX_STG_3A_FIFO_CLRB_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00002820 TX_STG_3A_CFC_CFG            |
 | +--------------------------------------------------+
 | | bit  28    R/W  TX_STG_3A_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  TX_STG_3A_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  TX_STG_3A_CFC_END_ADDR_CHG_ID    |
 | +--------------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_TX_STG_3A_CFC_END_ADDR_CHG_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_3A_CFC_END_ADDR_CHG_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_ABORT_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_ABORT_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_REQ_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_REQ_get      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00002824 TX_STG_3A_CFC_CFG_STAT   |
 | +----------------------------------------------+
 | | bit  0 R  TX_STG_3A_CFC_END_ADDR_CHG_SUCCESS |
 | +----------------------------------------------+
 | 
 | UINT32 ri_rcp_field_TX_STG_3A_CFC_END_ADDR_CHG_SUCCESS_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00002828 TX_STG_3A_CFC_CFG_VAL |
 | +-------------------------------------------+
 | | bit  7:0 R/W  TX_STG_3A_CFC_END_ADDR      |
 | +-------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_3A_CFC_END_ADDR_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_3A_CFC_END_ADDR_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_TX_STG_3A_CFC_END_ADDR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_3A_CFC_END_ADDR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00002840 + (N) * 0x04) TX_STG_3A_CFC_IPT |
 | |     N[0..95]                                         |
 | +------------------------------------------------------+
 | | bit  24    R/W  TX_STG_3A_CFC_IPT_VALID              |
 | | bit  22:16 R/W  TX_STG_3A_CFC_IPT_ID                 |
 | | bit  6:0   R/W  TX_STG_3A_CFC_IPT_VA                 |
 | +------------------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_3A_CFC_IPT_VALID_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_3A_CFC_IPT_VALID_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_TX_STG_3A_CFC_IPT_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_3A_CFC_IPT_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_TX_STG_3A_CFC_IPT_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_3A_CFC_IPT_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_TX_STG_3A_CFC_IPT_VA_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_3A_CFC_IPT_VA_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_TX_STG_3A_CFC_IPT_VA_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_3A_CFC_IPT_VA_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00002a00 TX_STG_3B_FIFO_CLRB_0 |
 | +-------------------------------------------+
 | | bit  31:0 R/W  TX_STG_3B_FIFO_CLRB        |
 | +-------------------------------------------+
 | 
 | void   ri_rcp_lfield_TX_STG_3B_FIFO_CLRB_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_TX_STG_3B_FIFO_CLRB_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_TX_STG_3B_FIFO_CLRB_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_TX_STG_3B_FIFO_CLRB_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00002a20 TX_STG_3B_CFC_CFG            |
 | +--------------------------------------------------+
 | | bit  28    R/W  TX_STG_3B_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  TX_STG_3B_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  TX_STG_3B_CFC_END_ADDR_CHG_ID    |
 | +--------------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_TX_STG_3B_CFC_END_ADDR_CHG_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_3B_CFC_END_ADDR_CHG_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_ABORT_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_ABORT_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_REQ_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_REQ_get      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00002a24 TX_STG_3B_CFC_CFG_STAT   |
 | +----------------------------------------------+
 | | bit  0 R  TX_STG_3B_CFC_END_ADDR_CHG_SUCCESS |
 | +----------------------------------------------+
 | 
 | UINT32 ri_rcp_field_TX_STG_3B_CFC_END_ADDR_CHG_SUCCESS_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00002a28 TX_STG_3B_CFC_CFG_VAL |
 | +-------------------------------------------+
 | | bit  7:0 R/W  TX_STG_3B_CFC_END_ADDR      |
 | +-------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_3B_CFC_END_ADDR_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_3B_CFC_END_ADDR_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_TX_STG_3B_CFC_END_ADDR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_3B_CFC_END_ADDR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00002a40 + (N) * 0x04) TX_STG_3B_CFC_IPT |
 | |     N[0..95]                                         |
 | +------------------------------------------------------+
 | | bit  24    R/W  TX_STG_3B_CFC_IPT_VALID              |
 | | bit  22:16 R/W  TX_STG_3B_CFC_IPT_ID                 |
 | | bit  6:0   R/W  TX_STG_3B_CFC_IPT_VA                 |
 | +------------------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_3B_CFC_IPT_VALID_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_3B_CFC_IPT_VALID_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_TX_STG_3B_CFC_IPT_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_3B_CFC_IPT_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_TX_STG_3B_CFC_IPT_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_3B_CFC_IPT_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_TX_STG_3B_CFC_IPT_VA_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_3B_CFC_IPT_VA_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_TX_STG_3B_CFC_IPT_VA_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_3B_CFC_IPT_VA_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00002c00 TX_STG_4_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  31:0 R/W  TX_STG_4_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   ri_rcp_lfield_TX_STG_4_FIFO_CLRB_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_TX_STG_4_FIFO_CLRB_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[3] )
 | void   ri_rcp_lfield_range_TX_STG_4_FIFO_CLRB_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_TX_STG_4_FIFO_CLRB_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00002c20 TX_STG_4_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  TX_STG_4_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  TX_STG_4_CFC_END_ADDR_CHG_REQ   |
 | | bit  22:16 R/W  TX_STG_4_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_TX_STG_4_CFC_END_ADDR_CHG_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_4_CFC_END_ADDR_CHG_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_ABORT_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_ABORT_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_REQ_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_REQ_get      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00002c24 TX_STG_4_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  TX_STG_4_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 ri_rcp_field_TX_STG_4_CFC_END_ADDR_CHG_SUCCESS_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00002c28 TX_STG_4_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  7:0 R/W  TX_STG_4_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_4_CFC_END_ADDR_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_4_CFC_END_ADDR_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_TX_STG_4_CFC_END_ADDR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_4_CFC_END_ADDR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00002c40 + (N) * 0x04) TX_STG_4_CFC_IPT |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  TX_STG_4_CFC_IPT_VALID              |
 | | bit  22:16 R/W  TX_STG_4_CFC_IPT_ID                 |
 | | bit  6:0   R/W  TX_STG_4_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_4_CFC_IPT_VALID_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_4_CFC_IPT_VALID_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_TX_STG_4_CFC_IPT_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_4_CFC_IPT_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_TX_STG_4_CFC_IPT_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_4_CFC_IPT_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_TX_STG_4_CFC_IPT_VA_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_4_CFC_IPT_VA_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_TX_STG_4_CFC_IPT_VA_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_4_CFC_IPT_VA_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00002e00 TX_STG_N_FIFO_CLRB_0 |
 | +------------------------------------------+
 | | bit  11:0 R/W  TX_STG_N_FIFO_CLRB        |
 | +------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_N_FIFO_CLRB_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_N_FIFO_CLRB_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_TX_STG_N_FIFO_CLRB_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_N_FIFO_CLRB_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00002e20 TX_STG_N_CFC_CFG            |
 | +-------------------------------------------------+
 | | bit  28    R/W  TX_STG_N_CFC_END_ADDR_CHG_ABORT |
 | | bit  24    R/W  TX_STG_N_CFC_END_ADDR_CHG_REQ   |
 | | bit  19:16 R/W  TX_STG_N_CFC_END_ADDR_CHG_ID    |
 | +-------------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_TX_STG_N_CFC_END_ADDR_CHG_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_N_CFC_END_ADDR_CHG_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_ABORT_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_ABORT_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_REQ_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_REQ_get      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00002e24 TX_STG_N_CFC_CFG_STAT   |
 | +---------------------------------------------+
 | | bit  0 R  TX_STG_N_CFC_END_ADDR_CHG_SUCCESS |
 | +---------------------------------------------+
 | 
 | UINT32 ri_rcp_field_TX_STG_N_CFC_END_ADDR_CHG_SUCCESS_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00002e28 TX_STG_N_CFC_CFG_VAL |
 | +------------------------------------------+
 | | bit  4:0 R/W  TX_STG_N_CFC_END_ADDR      |
 | +------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_N_CFC_END_ADDR_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_N_CFC_END_ADDR_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_TX_STG_N_CFC_END_ADDR_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_N_CFC_END_ADDR_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00002e40 + (N) * 0x04) TX_STG_N_CFC_IPT |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  24    R/W  TX_STG_N_CFC_IPT_VALID              |
 | | bit  19:16 R/W  TX_STG_N_CFC_IPT_ID                 |
 | | bit  3:0   R/W  TX_STG_N_CFC_IPT_VA                 |
 | +-----------------------------------------------------+
 | 
 | void   ri_rcp_field_TX_STG_N_CFC_IPT_VALID_set    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_N_CFC_IPT_VALID_get    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_TX_STG_N_CFC_IPT_ID_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_N_CFC_IPT_ID_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_TX_STG_N_CFC_IPT_ID_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_N_CFC_IPT_ID_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_TX_STG_N_CFC_IPT_VA_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_TX_STG_N_CFC_IPT_VA_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_TX_STG_N_CFC_IPT_VA_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_TX_STG_N_CFC_IPT_VA_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00003000 REG_SCH_CAL_ENABLE |
 | +----------------------------------------+
 | | bit  5:0 R/W  REG_SCH_CAL_EN           |
 | +----------------------------------------+
 | 
 | void   ri_rcp_field_REG_SCH_CAL_EN_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_SCH_CAL_EN_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_REG_SCH_CAL_EN_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_SCH_CAL_EN_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00003004 REG_SCH_CAL_SEL |
 | +-------------------------------------+
 | | bit  5:0 R/W  REG_SCH_CAL_SEL       |
 | +-------------------------------------+
 | 
 | void   ri_rcp_field_REG_SCH_CAL_SEL_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | UINT32 ri_rcp_field_REG_SCH_CAL_SEL_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void   ri_rcp_field_range_REG_SCH_CAL_SEL_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_REG_SCH_CAL_SEL_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00003080 + (N) * 0x04) SCH_STG_1_ACT_CFG |
 | |     N[0..11]                                         |
 | +------------------------------------------------------+
 | | bit  3:0 R/W  SCH_STG_1_ACT_CFG                      |
 | +------------------------------------------------------+
 | 
 | void   ri_rcp_lfield_SCH_STG_1_ACT_CFG_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[12] )
 | void   ri_rcp_lfield_SCH_STG_1_ACT_CFG_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[12] )
 | void   ri_rcp_lfield_range_SCH_STG_1_ACT_CFG_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_SCH_STG_1_ACT_CFG_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x000030c0 + (N) * 0x04) SCH_STG_1_STBY_CFG |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  3:0 R/W  SCH_STG_1_STBY_CFG                      |
 | +-------------------------------------------------------+
 | 
 | void   ri_rcp_lfield_SCH_STG_1_STBY_CFG_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[12] )
 | void   ri_rcp_lfield_SCH_STG_1_STBY_CFG_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[12] )
 | void   ri_rcp_lfield_range_SCH_STG_1_STBY_CFG_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_SCH_STG_1_STBY_CFG_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x00003100 + (N) * 4) SCH_STG_2_ACT_CFG |
 | |     N[0..48]                                      |
 | +---------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_2_ACT_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_2_ACT_CH_N                 |
 | +---------------------------------------------------+
 | 
 | void   ri_rcp_field_SCH_STG_2_ACT_CH_NP1_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_2_ACT_CH_NP1_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_2_ACT_CH_NP1_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_2_ACT_CH_NP1_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_SCH_STG_2_ACT_CH_N_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_2_ACT_CH_N_get         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_2_ACT_CH_N_set   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_2_ACT_CH_N_get   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00003200 + (N) * 4) SCH_STG_2_STBY_CFG |
 | |     N[0..48]                                       |
 | +----------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_2_STBY_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_2_STBY_CH_N                 |
 | +----------------------------------------------------+
 | 
 | void   ri_rcp_field_SCH_STG_2_STBY_CH_NP1_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_2_STBY_CH_NP1_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_2_STBY_CH_NP1_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_2_STBY_CH_NP1_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_SCH_STG_2_STBY_CH_N_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_2_STBY_CH_N_get         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_2_STBY_CH_N_set   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_2_STBY_CH_N_get   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00003300 + (N) * 4) SCH_STG_3A_ACT_CFG |
 | |     N[0..48]                                       |
 | +----------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_3A_ACT_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_3A_ACT_CH_N                 |
 | +----------------------------------------------------+
 | 
 | void   ri_rcp_field_SCH_STG_3A_ACT_CH_NP1_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_3A_ACT_CH_NP1_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_3A_ACT_CH_NP1_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_3A_ACT_CH_NP1_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_SCH_STG_3A_ACT_CH_N_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_3A_ACT_CH_N_get         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_3A_ACT_CH_N_set   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_3A_ACT_CH_N_get   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00003400 + (N) * 4) SCH_STG_3A_STBY_CFG |
 | |     N[0..48]                                        |
 | +-----------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_3A_STBY_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_3A_STBY_CH_N                 |
 | +-----------------------------------------------------+
 | 
 | void   ri_rcp_field_SCH_STG_3A_STBY_CH_NP1_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_3A_STBY_CH_NP1_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_3A_STBY_CH_NP1_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_3A_STBY_CH_NP1_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_SCH_STG_3A_STBY_CH_N_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_3A_STBY_CH_N_get         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_3A_STBY_CH_N_set   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_3A_STBY_CH_N_get   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00003500 + (N) * 4) SCH_STG_3B_ACT_CFG |
 | |     N[0..48]                                       |
 | +----------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_3B_ACT_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_3B_ACT_CH_N                 |
 | +----------------------------------------------------+
 | 
 | void   ri_rcp_field_SCH_STG_3B_ACT_CH_NP1_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_3B_ACT_CH_NP1_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_3B_ACT_CH_NP1_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_3B_ACT_CH_NP1_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_SCH_STG_3B_ACT_CH_N_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_3B_ACT_CH_N_get         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_3B_ACT_CH_N_set   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_3B_ACT_CH_N_get   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00003600 + (N) * 4) SCH_STG_3B_STBY_CFG |
 | |     N[0..48]                                        |
 | +-----------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_3B_STBY_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_3B_STBY_CH_N                 |
 | +-----------------------------------------------------+
 | 
 | void   ri_rcp_field_SCH_STG_3B_STBY_CH_NP1_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_3B_STBY_CH_NP1_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_3B_STBY_CH_NP1_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_3B_STBY_CH_NP1_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_SCH_STG_3B_STBY_CH_N_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_3B_STBY_CH_N_get         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_3B_STBY_CH_N_set   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_3B_STBY_CH_N_get   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x00003700 + (N) * 4) SCH_STG_4_ACT_CFG |
 | |     N[0..48]                                      |
 | +---------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_4_ACT_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_4_ACT_CH_N                 |
 | +---------------------------------------------------+
 | 
 | void   ri_rcp_field_SCH_STG_4_ACT_CH_NP1_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_4_ACT_CH_NP1_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_4_ACT_CH_NP1_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_4_ACT_CH_NP1_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_SCH_STG_4_ACT_CH_N_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_4_ACT_CH_N_get         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_4_ACT_CH_N_set   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_4_ACT_CH_N_get   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00003800 + (N) * 4) SCH_STG_4_STBY_CFG |
 | |     N[0..48]                                       |
 | +----------------------------------------------------+
 | | bit  14:8 R/W  SCH_STG_4_STBY_CH_NP1               |
 | | bit  6:0  R/W  SCH_STG_4_STBY_CH_N                 |
 | +----------------------------------------------------+
 | 
 | void   ri_rcp_field_SCH_STG_4_STBY_CH_NP1_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_4_STBY_CH_NP1_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_4_STBY_CH_NP1_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_4_STBY_CH_NP1_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ri_rcp_field_SCH_STG_4_STBY_CH_N_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 ri_rcp_field_SCH_STG_4_STBY_CH_N_get         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | void   ri_rcp_field_range_SCH_STG_4_STBY_CH_N_set   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_field_range_SCH_STG_4_STBY_CH_N_get   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00003900 + (N) * 0x04) SCH_STG_N_ACT_CFG |
 | |     N[0..11]                                         |
 | +------------------------------------------------------+
 | | bit  3:0 R/W  SCH_STG_N_ACT_CFG                      |
 | +------------------------------------------------------+
 | 
 | void   ri_rcp_lfield_SCH_STG_N_ACT_CFG_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[12] )
 | void   ri_rcp_lfield_SCH_STG_N_ACT_CFG_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[12] )
 | void   ri_rcp_lfield_range_SCH_STG_N_ACT_CFG_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_SCH_STG_N_ACT_CFG_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00003940 + (N) * 0x04) SCH_STG_N_STBY_CFG |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  3:0 R/W  SCH_STG_N_STBY_CFG                      |
 | +-------------------------------------------------------+
 | 
 | void   ri_rcp_lfield_SCH_STG_N_STBY_CFG_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[12] )
 | void   ri_rcp_lfield_SCH_STG_N_STBY_CFG_get       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value[12] )
 | void   ri_rcp_lfield_range_SCH_STG_N_STBY_CFG_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ri_rcp_lfield_range_SCH_STG_N_STBY_CFG_get ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _RI_RCP_IO_H
#define _RI_RCP_IO_H

#include "odu_rcp_loc.h"
#include "ri_rcp_regs.h"
#include "ri_rcp_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 ri_rcp_buffer_init               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | void                 ri_rcp_buffer_flush              ( ri_rcp_buffer_t *b_ptr )
 | UINT32               ri_rcp_reg_read                  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 ri_rcp_reg_write                 ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 ri_rcp_field_set                 ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 ri_rcp_action_on_write_field_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 ri_rcp_burst_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 ri_rcp_burst_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE ri_rcp_poll                      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 ri_rcp_reg_REG_SW_RESET_write                       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_SW_RESET_field_set                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_SW_RESET_read                        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_LOWPWR_write                         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_LOWPWR_field_set                     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_LOWPWR_read                          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_RAM_PWR_MGMT_write                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_RAM_PWR_MGMT_field_set               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_RAM_PWR_MGMT_read                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_GOJF_write                           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_GOJF_field_set                       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_GOJF_read                            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_ETH_TX_EN_write                      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_ETH_TX_EN_field_set                  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_ETH_TX_EN_read                       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_ETH_MAC_OUI_write                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_ETH_MAC_OUI_field_set                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_ETH_MAC_OUI_read                     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_ETH_MAC_DIGI_ID_write                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_ETH_MAC_DIGI_ID_field_set            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_ETH_MAC_DIGI_ID_read                 ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_ETH_ETYPE_write                      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_ETH_ETYPE_field_set                  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_ETH_ETYPE_read                       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_1_RCP_INS_EN_0_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_1_RCP_INS_EN_0_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_1_RCP_INS_EN_0_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_0_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_0_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_0_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_1_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_1_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_1_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_2_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_2_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_2_RCP_INS_EN_2_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_0_write         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_0_field_set     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_0_read          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_1_write         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_1_field_set     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_1_read          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_2_write         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_2_field_set     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_3A_RCP_INS_EN_2_read          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_0_write         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_0_field_set     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_0_read          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_1_write         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_1_field_set     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_1_read          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_2_write         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_2_field_set     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_3B_RCP_INS_EN_2_read          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_0_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_0_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_0_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_1_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_1_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_1_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_2_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_2_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_4_RCP_INS_EN_2_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_N_RCP_INS_EN_0_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_N_RCP_INS_EN_0_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_N_RCP_INS_EN_0_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_1_APS_INS_EN_0_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_1_APS_INS_EN_0_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_1_APS_INS_EN_0_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_0_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_0_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_0_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_1_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_1_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_1_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_2_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_2_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_2_APS_INS_EN_2_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_0_write         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_0_field_set     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_0_read          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_1_write         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_1_field_set     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_1_read          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_2_write         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_2_field_set     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_3A_APS_INS_EN_2_read          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_0_write         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_0_field_set     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_0_read          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_1_write         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_1_field_set     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_1_read          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_2_write         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_2_field_set     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_3B_APS_INS_EN_2_read          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_0_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_0_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_0_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_1_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_1_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_1_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_2_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_2_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_4_APS_INS_EN_2_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_TX_STG_N_APS_INS_EN_0_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_TX_STG_N_APS_INS_EN_0_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_TX_STG_N_APS_INS_EN_0_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_CFC_DBG_SEL_write                        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_DBG_SEL_field_set                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_DBG_SEL_read                         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_DBG_CTRL_write                           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_DBG_CTRL_field_set                       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_DBG_CTRL_read                            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_CFC_IRQ_SEL_write                        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_IRQ_SEL_field_set                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_IRQ_SEL_read                         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_CFC_IRQ_TYPE_SEL_0_write                 ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_IRQ_TYPE_SEL_0_field_set             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_IRQ_TYPE_SEL_0_read                  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_CFC_IRQ_TYPE_SEL_1_write                 ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_IRQ_TYPE_SEL_1_field_set             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_IRQ_TYPE_SEL_1_read                  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_CFC_IRQ_TYPE_SEL_2_write                 ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_IRQ_TYPE_SEL_2_field_set             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_IRQ_TYPE_SEL_2_read                  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_CFC_IPT_ERR_SEL_write                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_IPT_ERR_SEL_field_set                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_IPT_ERR_SEL_read                     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_CFC_STATUS_SEL_write                     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_STATUS_SEL_field_set                 ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_STATUS_SEL_read                      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_1_FIFO_CLRB_0_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_1_FIFO_CLRB_0_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_1_FIFO_CLRB_0_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_1_CFC_CFG_write                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_1_CFC_CFG_field_set               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_1_CFC_CFG_read                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_1_CFC_CFG_VAL_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_1_CFC_CFG_VAL_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_1_CFC_CFG_VAL_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_2_FIFO_CLRB_0_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_2_FIFO_CLRB_0_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_2_FIFO_CLRB_0_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_2_FIFO_CLRB_1_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_2_FIFO_CLRB_1_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_2_FIFO_CLRB_1_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_2_FIFO_CLRB_2_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_2_FIFO_CLRB_2_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_2_FIFO_CLRB_2_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_2_CFC_CFG_write                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_2_CFC_CFG_field_set               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_2_CFC_CFG_read                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_2_CFC_CFG_VAL_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_2_CFC_CFG_VAL_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_2_CFC_CFG_VAL_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_3A_FIFO_CLRB_0_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_3A_FIFO_CLRB_0_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_3A_FIFO_CLRB_0_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_3A_FIFO_CLRB_1_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_3A_FIFO_CLRB_1_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_3A_FIFO_CLRB_1_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_3A_FIFO_CLRB_2_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_3A_FIFO_CLRB_2_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_3A_FIFO_CLRB_2_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_3A_CFC_CFG_write                  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_3A_CFC_CFG_field_set              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_3A_CFC_CFG_read                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_3A_CFC_CFG_VAL_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_3A_CFC_CFG_VAL_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_3A_CFC_CFG_VAL_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_3B_FIFO_CLRB_0_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_3B_FIFO_CLRB_0_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_3B_FIFO_CLRB_0_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_3B_FIFO_CLRB_1_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_3B_FIFO_CLRB_1_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_3B_FIFO_CLRB_1_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_3B_FIFO_CLRB_2_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_3B_FIFO_CLRB_2_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_3B_FIFO_CLRB_2_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_3B_CFC_CFG_write                  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_3B_CFC_CFG_field_set              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_3B_CFC_CFG_read                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_3B_CFC_CFG_VAL_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_3B_CFC_CFG_VAL_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_3B_CFC_CFG_VAL_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_4_FIFO_CLRB_0_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_4_FIFO_CLRB_0_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_4_FIFO_CLRB_0_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_4_FIFO_CLRB_1_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_4_FIFO_CLRB_1_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_4_FIFO_CLRB_1_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_4_FIFO_CLRB_2_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_4_FIFO_CLRB_2_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_4_FIFO_CLRB_2_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_4_CFC_CFG_write                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_4_CFC_CFG_field_set               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_4_CFC_CFG_read                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_4_CFC_CFG_VAL_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_4_CFC_CFG_VAL_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_4_CFC_CFG_VAL_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_N_FIFO_CLRB_0_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_N_FIFO_CLRB_0_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_N_FIFO_CLRB_0_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_N_CFC_CFG_write                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_N_CFC_CFG_field_set               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_N_CFC_CFG_read                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_RX_STG_N_CFC_CFG_VAL_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_N_CFC_CFG_VAL_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_N_CFC_CFG_VAL_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_1_FIFO_CLRB_0_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_1_FIFO_CLRB_0_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_1_FIFO_CLRB_0_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_1_CFC_CFG_write                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_1_CFC_CFG_field_set               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_1_CFC_CFG_read                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_1_CFC_CFG_VAL_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_1_CFC_CFG_VAL_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_1_CFC_CFG_VAL_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_2_FIFO_CLRB_0_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_2_FIFO_CLRB_0_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_2_FIFO_CLRB_0_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_2_FIFO_CLRB_1_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_2_FIFO_CLRB_1_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_2_FIFO_CLRB_1_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_2_FIFO_CLRB_2_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_2_FIFO_CLRB_2_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_2_FIFO_CLRB_2_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_2_CFC_CFG_write                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_2_CFC_CFG_field_set               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_2_CFC_CFG_read                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_2_CFC_CFG_VAL_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_2_CFC_CFG_VAL_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_2_CFC_CFG_VAL_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_3A_FIFO_CLRB_0_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_3A_FIFO_CLRB_0_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_3A_FIFO_CLRB_0_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_3A_FIFO_CLRB_1_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_3A_FIFO_CLRB_1_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_3A_FIFO_CLRB_1_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_3A_FIFO_CLRB_2_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_3A_FIFO_CLRB_2_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_3A_FIFO_CLRB_2_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_3A_CFC_CFG_write                  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_3A_CFC_CFG_field_set              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_3A_CFC_CFG_read                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_3A_CFC_CFG_VAL_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_3A_CFC_CFG_VAL_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_3A_CFC_CFG_VAL_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_3B_FIFO_CLRB_0_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_3B_FIFO_CLRB_0_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_3B_FIFO_CLRB_0_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_3B_FIFO_CLRB_1_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_3B_FIFO_CLRB_1_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_3B_FIFO_CLRB_1_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_3B_FIFO_CLRB_2_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_3B_FIFO_CLRB_2_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_3B_FIFO_CLRB_2_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_3B_CFC_CFG_write                  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_3B_CFC_CFG_field_set              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_3B_CFC_CFG_read                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_3B_CFC_CFG_VAL_write              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_3B_CFC_CFG_VAL_field_set          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_3B_CFC_CFG_VAL_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_4_FIFO_CLRB_0_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_4_FIFO_CLRB_0_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_4_FIFO_CLRB_0_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_4_FIFO_CLRB_1_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_4_FIFO_CLRB_1_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_4_FIFO_CLRB_1_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_4_FIFO_CLRB_2_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_4_FIFO_CLRB_2_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_4_FIFO_CLRB_2_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_4_CFC_CFG_write                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_4_CFC_CFG_field_set               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_4_CFC_CFG_read                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_4_CFC_CFG_VAL_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_4_CFC_CFG_VAL_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_4_CFC_CFG_VAL_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_N_FIFO_CLRB_0_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_N_FIFO_CLRB_0_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_N_FIFO_CLRB_0_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_N_CFC_CFG_write                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_N_CFC_CFG_field_set               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_N_CFC_CFG_read                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_TX_STG_N_CFC_CFG_VAL_write               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_N_CFC_CFG_VAL_field_set           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_N_CFC_CFG_VAL_read                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_SCH_CAL_ENABLE_write                 ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_SCH_CAL_ENABLE_field_set             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_SCH_CAL_ENABLE_read                  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_REG_SCH_CAL_SEL_write                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_REG_SCH_CAL_SEL_field_set                ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_REG_SCH_CAL_SEL_read                     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_SCH_STG_1_ACT_CFG_array_burst_write      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 ri_rcp_reg_SCH_STG_1_ACT_CFG_array_field_set        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 ri_rcp_reg_SCH_STG_1_ACT_CFG_array_burst_read       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               ri_rcp_reg_SCH_STG_1_ACT_CFG_array_read             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 ri_rcp_reg_SCH_STG_1_STBY_CFG_array_burst_write     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 ri_rcp_reg_SCH_STG_1_STBY_CFG_array_field_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 ri_rcp_reg_SCH_STG_1_STBY_CFG_array_burst_read      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               ri_rcp_reg_SCH_STG_1_STBY_CFG_array_read            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 ri_rcp_reg_SCH_STG_N_ACT_CFG_array_burst_write      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 ri_rcp_reg_SCH_STG_N_ACT_CFG_array_field_set        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 ri_rcp_reg_SCH_STG_N_ACT_CFG_array_burst_read       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               ri_rcp_reg_SCH_STG_N_ACT_CFG_array_read             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 ri_rcp_reg_SCH_STG_N_STBY_CFG_array_burst_write     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 ri_rcp_reg_SCH_STG_N_STBY_CFG_array_field_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 ri_rcp_reg_SCH_STG_N_STBY_CFG_array_burst_read      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               ri_rcp_reg_SCH_STG_N_STBY_CFG_array_read            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 ri_rcp_reg_RX_STG_1_APS_LEVEL_array_write           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_1_APS_LEVEL_array_field_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_1_APS_LEVEL_array_read            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_RX_STG_N_APS_LEVEL_array_write           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_N_APS_LEVEL_array_field_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_N_APS_LEVEL_array_read            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_RX_STG_1_CFC_IPT_array_write             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_1_CFC_IPT_array_field_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_1_CFC_IPT_array_read              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_RX_STG_N_CFC_IPT_array_write             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_N_CFC_IPT_array_field_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_N_CFC_IPT_array_read              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_TX_STG_1_CFC_IPT_array_write             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_1_CFC_IPT_array_field_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_1_CFC_IPT_array_read              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_TX_STG_N_CFC_IPT_array_write             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_N_CFC_IPT_array_field_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_N_CFC_IPT_array_read              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_RX_STG_2_APS_LEVEL_array_write           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_2_APS_LEVEL_array_field_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_2_APS_LEVEL_array_read            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_RX_STG_3A_APS_LEVEL_array_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_3A_APS_LEVEL_array_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_3A_APS_LEVEL_array_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_RX_STG_3B_APS_LEVEL_array_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_3B_APS_LEVEL_array_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_3B_APS_LEVEL_array_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_RX_STG_4_APS_LEVEL_array_write           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_4_APS_LEVEL_array_field_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_4_APS_LEVEL_array_read            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_RX_STG_2_CFC_IPT_array_write             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_2_CFC_IPT_array_field_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_2_CFC_IPT_array_read              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_RX_STG_3A_CFC_IPT_array_write            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_3A_CFC_IPT_array_field_set        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_3A_CFC_IPT_array_read             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_RX_STG_3B_CFC_IPT_array_write            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_3B_CFC_IPT_array_field_set        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_3B_CFC_IPT_array_read             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_RX_STG_4_CFC_IPT_array_write             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_RX_STG_4_CFC_IPT_array_field_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_RX_STG_4_CFC_IPT_array_read              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_TX_STG_2_CFC_IPT_array_write             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_2_CFC_IPT_array_field_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_2_CFC_IPT_array_read              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_TX_STG_3A_CFC_IPT_array_write            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_3A_CFC_IPT_array_field_set        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_3A_CFC_IPT_array_read             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_TX_STG_3B_CFC_IPT_array_write            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_3B_CFC_IPT_array_field_set        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_3B_CFC_IPT_array_read             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_TX_STG_4_CFC_IPT_array_write             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_TX_STG_4_CFC_IPT_array_field_set         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_TX_STG_4_CFC_IPT_array_read              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_GEN_IRQ_E_write                          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_GEN_IRQ_E_field_set                      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_GEN_IRQ_E_read                           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_CFC_FIFO_IRQ_EN_0_write                  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_FIFO_IRQ_EN_0_field_set              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_FIFO_IRQ_EN_0_read                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_CFC_FIFO_IRQ_EN_1_write                  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_FIFO_IRQ_EN_1_field_set              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_FIFO_IRQ_EN_1_read                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_CFC_FIFO_IRQ_EN_2_write                  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_FIFO_IRQ_EN_2_field_set              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_FIFO_IRQ_EN_2_read                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_CFC_IPT_ERR_EN_0_write                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_IPT_ERR_EN_0_field_set               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_IPT_ERR_EN_0_read                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_CFC_IPT_ERR_EN_1_write                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_IPT_ERR_EN_1_field_set               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_IPT_ERR_EN_1_read                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | void                 ri_rcp_reg_CFC_IPT_ERR_EN_2_write                   ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_IPT_ERR_EN_2_field_set               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_IPT_ERR_EN_2_read                    ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_IRQ_SUMMARY_read                         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ri_rcp_reg_IRQ_SUMMARY_poll                         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 ri_rcp_reg_GEN_IRQ_I_write                          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_GEN_IRQ_I_action_on_write_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_GEN_IRQ_I_read                           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ri_rcp_reg_GEN_IRQ_I_poll                           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 ri_rcp_reg_CFC_FIFO_IRQ_0_write                     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_FIFO_IRQ_0_action_on_write_field_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_FIFO_IRQ_0_read                      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ri_rcp_reg_CFC_FIFO_IRQ_0_poll                      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 ri_rcp_reg_CFC_FIFO_IRQ_1_write                     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_FIFO_IRQ_1_action_on_write_field_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_FIFO_IRQ_1_read                      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ri_rcp_reg_CFC_FIFO_IRQ_1_poll                      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 ri_rcp_reg_CFC_FIFO_IRQ_2_write                     ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_FIFO_IRQ_2_action_on_write_field_set ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_FIFO_IRQ_2_read                      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ri_rcp_reg_CFC_FIFO_IRQ_2_poll                      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 ri_rcp_reg_CFC_IPT_ERR_0_write                      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_IPT_ERR_0_action_on_write_field_set  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_IPT_ERR_0_read                       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ri_rcp_reg_CFC_IPT_ERR_0_poll                       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 ri_rcp_reg_CFC_IPT_ERR_1_write                      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_IPT_ERR_1_action_on_write_field_set  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_IPT_ERR_1_read                       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ri_rcp_reg_CFC_IPT_ERR_1_poll                       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 ri_rcp_reg_CFC_IPT_ERR_2_write                      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 value )
 | void                 ri_rcp_reg_CFC_IPT_ERR_2_action_on_write_field_set  ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_CFC_IPT_ERR_2_read                       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE ri_rcp_reg_CFC_IPT_ERR_2_poll                       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 ri_rcp_reg_HO_ODU_MAC_DA_CFG_array_write            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_HO_ODU_MAC_DA_CFG_array_field_set        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_HO_ODU_MAC_DA_CFG_array_read             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_SCH_STG_2_ACT_CFG_array_write            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_SCH_STG_2_ACT_CFG_array_field_set        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_SCH_STG_2_ACT_CFG_array_read             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_SCH_STG_2_STBY_CFG_array_write           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_SCH_STG_2_STBY_CFG_array_field_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_SCH_STG_2_STBY_CFG_array_read            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_SCH_STG_3A_ACT_CFG_array_write           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_SCH_STG_3A_ACT_CFG_array_field_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_SCH_STG_3A_ACT_CFG_array_read            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_SCH_STG_3A_STBY_CFG_array_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_SCH_STG_3A_STBY_CFG_array_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_SCH_STG_3A_STBY_CFG_array_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_SCH_STG_3B_ACT_CFG_array_write           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_SCH_STG_3B_ACT_CFG_array_field_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_SCH_STG_3B_ACT_CFG_array_read            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_SCH_STG_3B_STBY_CFG_array_write          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_SCH_STG_3B_STBY_CFG_array_field_set      ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_SCH_STG_3B_STBY_CFG_array_read           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_SCH_STG_4_ACT_CFG_array_write            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_SCH_STG_4_ACT_CFG_array_field_set        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_SCH_STG_4_ACT_CFG_array_read             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_SCH_STG_4_STBY_CFG_array_write           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_SCH_STG_4_STBY_CFG_array_field_set       ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_SCH_STG_4_STBY_CFG_array_read            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 ri_rcp_reg_LO_ODU_MAC_DA_CFG_array_write            ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 ri_rcp_reg_LO_ODU_MAC_DA_CFG_array_field_set        ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               ri_rcp_reg_LO_ODU_MAC_DA_CFG_array_read             ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               ri_rcp_reg_DBG_RDATA_read                           ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_CFC_EMPTY_0_read                         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_CFC_EMPTY_1_read                         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_CFC_EMPTY_2_read                         ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_CFC_FULL_0_read                          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_CFC_FULL_1_read                          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_CFC_FULL_2_read                          ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_RX_STG_1_CFC_CFG_STAT_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_RX_STG_2_CFC_CFG_STAT_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_RX_STG_3A_CFC_CFG_STAT_read              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_RX_STG_3B_CFC_CFG_STAT_read              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_RX_STG_4_CFC_CFG_STAT_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_RX_STG_N_CFC_CFG_STAT_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_TX_STG_1_CFC_CFG_STAT_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_TX_STG_2_CFC_CFG_STAT_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_TX_STG_3A_CFC_CFG_STAT_read              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_TX_STG_3B_CFC_CFG_STAT_read              ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_TX_STG_4_CFC_CFG_STAT_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 | UINT32               ri_rcp_reg_TX_STG_N_CFC_CFG_STAT_read               ( ri_rcp_buffer_t *b_ptr, odu_rcp_handle_t *h_ptr )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _RI_RCP_IO_H */
