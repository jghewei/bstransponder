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
 *     and register accessor functions for the emac_mac block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing emac_mac io functions is:
 | {
 |     emac_mac_buffer_t b_ptr[1];
 |     emac_mac_buffer_init( b_ptr, h_ptr, A );
 |     value = emac_mac_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     emac_mac_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     emac_mac_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = emac_mac_field_<FIELD>_get( NULL, h_ptr, A );
 | emac_mac_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +------------------------------+
 | | Register 0x00008000 REVISION |
 | +------------------------------+
 | | bit  31:16 R  PRGM_REVISION  |
 | | bit  15:8  R  CORE_VERSION   |
 | | bit  7:0   R  CORE_REVISION  |
 | +------------------------------+
 | 
 | UINT32 emac_mac_field_PRGM_REVISION_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_PRGM_REVISION_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 emac_mac_field_CORE_VERSION_get        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_CORE_VERSION_get  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 emac_mac_field_CORE_REVISION_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_CORE_REVISION_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------+
 | | Register 0x00008004 SCRATCH |
 | +-----------------------------+
 | | bit  31:0 R/W  SCRATCH      |
 | +-----------------------------+
 | 
 | void   emac_mac_field_SCRATCH_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_SCRATCH_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_SCRATCH_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_SCRATCH_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00008008 CTRL_AND_CFG |
 | +----------------------------------+
 | | bit  29 R/W  SEND_RF             |
 | | bit  28 R/W  SEND_LF             |
 | | bit  27 R/W  FLT_HDL_DIS         |
 | | bit  22 R/W  TX_FLUSH            |
 | | bit  21 R/W  RX_SFD_ANY          |
 | | bit  20 R/W  PAUSE_PFC_COMP      |
 | | bit  19 R/W  PFC_MODE            |
 | | bit  18 R/W  RS_COL_CNT_EXT      |
 | | bit  17 R/W  NO_LGTH_CHECK       |
 | | bit  16 R/W  SEND_IDLE           |
 | | bit  13 R/W  CMD_FRAME_EN        |
 | | bit  12 R/W  SW_RESET            |
 | | bit  11 R/W  TX_PAD_EN           |
 | | bit  9  R/W  TX_ADDR_INS         |
 | | bit  8  R/W  PAUSE_IGNORE        |
 | | bit  7  R/W  PAUSE_FWD           |
 | | bit  6  R/W  CRC_FWD             |
 | | bit  4  R/W  PROMIS_EN           |
 | | bit  3  R/W  WAN_MODE            |
 | | bit  1  R/W  RX_ENA              |
 | | bit  0  R/W  TX_ENA              |
 | +----------------------------------+
 | 
 | void   emac_mac_field_SEND_RF_set        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_SEND_RF_get        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_SEND_LF_set        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_SEND_LF_get        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_FLT_HDL_DIS_set    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_FLT_HDL_DIS_get    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_TX_FLUSH_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_TX_FLUSH_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_RX_SFD_ANY_set     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_RX_SFD_ANY_get     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_PAUSE_PFC_COMP_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_PAUSE_PFC_COMP_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_PFC_MODE_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_PFC_MODE_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_RS_COL_CNT_EXT_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_RS_COL_CNT_EXT_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_NO_LGTH_CHECK_set  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_NO_LGTH_CHECK_get  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_SEND_IDLE_set      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_SEND_IDLE_get      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_CMD_FRAME_EN_set   ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CMD_FRAME_EN_get   ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_SW_RESET_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_SW_RESET_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_TX_PAD_EN_set      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_TX_PAD_EN_get      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_TX_ADDR_INS_set    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_TX_ADDR_INS_get    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_PAUSE_IGNORE_set   ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_PAUSE_IGNORE_get   ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_PAUSE_FWD_set      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_PAUSE_FWD_get      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_CRC_FWD_set        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CRC_FWD_get        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_PROMIS_EN_set      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_PROMIS_EN_get      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_WAN_MODE_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_WAN_MODE_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_RX_ENA_set         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_RX_ENA_get         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_TX_ENA_set         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_TX_ENA_get         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------+
 | | Register 0x0000800c MAC_ADR_LSB_ |
 | +----------------------------------+
 | | bit  31:0 R/W  MAC_ADDR_LSB      |
 | +----------------------------------+
 | 
 | void   emac_mac_field_MAC_ADDR_LSB_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_MAC_ADDR_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_MAC_ADDR_LSB_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_MAC_ADDR_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00008010 MAC_ADR_MSB_ |
 | +----------------------------------+
 | | bit  15:0 R/W  MAC_ADDR_MSB      |
 | +----------------------------------+
 | 
 | void   emac_mac_field_MAC_ADDR_MSB_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_MAC_ADDR_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_MAC_ADDR_MSB_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_MAC_ADDR_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00008014 FRAME_LENGTH |
 | +----------------------------------+
 | | bit  15:0 R/W  FRM_LENGTH        |
 | +----------------------------------+
 | 
 | void   emac_mac_field_FRM_LENGTH_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_FRM_LENGTH_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_FRM_LENGTH_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_FRM_LENGTH_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------+
 | | Register 0x00008040 STAT |
 | +--------------------------+
 | | bit  2 R  PHY_LOS        |
 | | bit  1 R  RX_REM_FAULT   |
 | | bit  0 R  RX_LOC_FAULT   |
 | +--------------------------+
 | 
 | UINT32 emac_mac_field_PHY_LOS_get      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_RX_REM_FAULT_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_RX_LOC_FAULT_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------------+
 | | Register 0x00008044 TX_INTER_PACKET_GAP_IPG_ |
 | +----------------------------------------------+
 | | bit  6:0 R/W  TX_IPG_LENGTH                  |
 | +----------------------------------------------+
 | 
 | void   emac_mac_field_TX_IPG_LENGTH_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_TX_IPG_LENGTH_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_TX_IPG_LENGTH_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_TX_IPG_LENGTH_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00008048 RX_APPLICATION_CREDIT_RESET |
 | +-------------------------------------------------+
 | | bit  0 R/W  CREDIT_RESET                        |
 | +-------------------------------------------------+
 | 
 | void   emac_mac_field_CREDIT_RESET_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CREDIT_RESET_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------------------------+
 | | Register 0x0000804c RX_APPLICATION_CREDIT_INITIALIZATION |
 | +----------------------------------------------------------+
 | | bit  7:0 R/W  CREDIT_INIT                                |
 | +----------------------------------------------------------+
 | 
 | void   emac_mac_field_CREDIT_INIT_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CREDIT_INIT_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CREDIT_INIT_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CREDIT_INIT_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00008054 CLASS_0_1_PAUSE_QUANTA |
 | +--------------------------------------------+
 | | bit  31:16 R/W  CL1_PAUSE_QUANTA           |
 | | bit  15:0  R/W  CL0_PAUSE_QUANTA           |
 | +--------------------------------------------+
 | 
 | void   emac_mac_field_CL1_PAUSE_QUANTA_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL1_PAUSE_QUANTA_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL1_PAUSE_QUANTA_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL1_PAUSE_QUANTA_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_mac_field_CL0_PAUSE_QUANTA_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL0_PAUSE_QUANTA_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL0_PAUSE_QUANTA_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL0_PAUSE_QUANTA_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00008058 CLASS_2_3_PAUSE_QUANTA |
 | +--------------------------------------------+
 | | bit  31:16 R/W  CL3_PAUSE_QUANTA           |
 | | bit  15:0  R/W  CL2_PAUSE_QUANTA           |
 | +--------------------------------------------+
 | 
 | void   emac_mac_field_CL3_PAUSE_QUANTA_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL3_PAUSE_QUANTA_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL3_PAUSE_QUANTA_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL3_PAUSE_QUANTA_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_mac_field_CL2_PAUSE_QUANTA_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL2_PAUSE_QUANTA_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL2_PAUSE_QUANTA_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL2_PAUSE_QUANTA_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0000805c CLASS_4_5_PAUSE_QUANTA |
 | +--------------------------------------------+
 | | bit  31:16 R/W  CL5_PAUSE_QUANTA           |
 | | bit  15:0  R/W  CL4_PAUSE_QUANTA           |
 | +--------------------------------------------+
 | 
 | void   emac_mac_field_CL5_PAUSE_QUANTA_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL5_PAUSE_QUANTA_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL5_PAUSE_QUANTA_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL5_PAUSE_QUANTA_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_mac_field_CL4_PAUSE_QUANTA_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL4_PAUSE_QUANTA_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL4_PAUSE_QUANTA_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL4_PAUSE_QUANTA_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00008060 CLASS_6_7_PAUSE_QUANTA |
 | +--------------------------------------------+
 | | bit  31:16 R/W  CL7_PAUSE_QUANTA           |
 | | bit  15:0  R/W  CL6_PAUSE_QUANTA           |
 | +--------------------------------------------+
 | 
 | void   emac_mac_field_CL7_PAUSE_QUANTA_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL7_PAUSE_QUANTA_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL7_PAUSE_QUANTA_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL7_PAUSE_QUANTA_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_mac_field_CL6_PAUSE_QUANTA_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL6_PAUSE_QUANTA_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL6_PAUSE_QUANTA_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL6_PAUSE_QUANTA_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00008064 CLASS_0_1_PAUSE_QUANTA_TH |
 | +-----------------------------------------------+
 | | bit  31:16 R/W  CL1_PAUSE_QUANTA_THR          |
 | | bit  15:0  R/W  CL0_PAUSE_QUANTA_THR          |
 | +-----------------------------------------------+
 | 
 | void   emac_mac_field_CL1_PAUSE_QUANTA_THR_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL1_PAUSE_QUANTA_THR_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL1_PAUSE_QUANTA_THR_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL1_PAUSE_QUANTA_THR_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_mac_field_CL0_PAUSE_QUANTA_THR_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL0_PAUSE_QUANTA_THR_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL0_PAUSE_QUANTA_THR_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL0_PAUSE_QUANTA_THR_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00008068 CLASS_2_3_PAUSE_QUANTA_TH |
 | +-----------------------------------------------+
 | | bit  31:16 R/W  CL3_PAUSE_QUANTA_THR          |
 | | bit  15:0  R/W  CL2_PAUSE_QUANTA_THR          |
 | +-----------------------------------------------+
 | 
 | void   emac_mac_field_CL3_PAUSE_QUANTA_THR_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL3_PAUSE_QUANTA_THR_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL3_PAUSE_QUANTA_THR_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL3_PAUSE_QUANTA_THR_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_mac_field_CL2_PAUSE_QUANTA_THR_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL2_PAUSE_QUANTA_THR_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL2_PAUSE_QUANTA_THR_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL2_PAUSE_QUANTA_THR_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x0000806c CLASS_4_5_PAUSE_QUANTA_TH |
 | +-----------------------------------------------+
 | | bit  31:16 R/W  CL5_PAUSE_QUANTA_THR          |
 | | bit  15:0  R/W  CL4_PAUSE_QUANTA_THR          |
 | +-----------------------------------------------+
 | 
 | void   emac_mac_field_CL5_PAUSE_QUANTA_THR_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL5_PAUSE_QUANTA_THR_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL5_PAUSE_QUANTA_THR_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL5_PAUSE_QUANTA_THR_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_mac_field_CL4_PAUSE_QUANTA_THR_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL4_PAUSE_QUANTA_THR_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL4_PAUSE_QUANTA_THR_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL4_PAUSE_QUANTA_THR_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00008070 CLASS_6_7_PAUSE_QUANTA_TH |
 | +-----------------------------------------------+
 | | bit  31:16 R/W  CL7_PAUSE_QUANTA_THR          |
 | | bit  15:0  R/W  CL6_PAUSE_QUANTA_THR          |
 | +-----------------------------------------------+
 | 
 | void   emac_mac_field_CL7_PAUSE_QUANTA_THR_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL7_PAUSE_QUANTA_THR_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL7_PAUSE_QUANTA_THR_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL7_PAUSE_QUANTA_THR_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   emac_mac_field_CL6_PAUSE_QUANTA_THR_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_CL6_PAUSE_QUANTA_THR_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_CL6_PAUSE_QUANTA_THR_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_CL6_PAUSE_QUANTA_THR_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00008074 RX_PAUSE_STAT |
 | +-----------------------------------+
 | | bit  7:0 R  PAUSE_STATUS          |
 | +-----------------------------------+
 | 
 | UINT32 emac_mac_field_PAUSE_STATUS_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_PAUSE_STATUS_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00008080 STATISTIC_CNT_AFRAMESTXOK_LSB_ |
 | +----------------------------------------------------+
 | | bit  31:0 R  AFRAMESTXOK_LSB                       |
 | +----------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AFRAMESTXOK_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AFRAMESTXOK_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00008084 STATISTIC_CNT_AFRAMESTXOK_MSB_ |
 | +----------------------------------------------------+
 | | bit  7:0 R  AFRAMESTXOK_MSB                        |
 | +----------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AFRAMESTXOK_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AFRAMESTXOK_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00008088 STATISTIC_CNT_AFRAMESRXOK_LSB_ |
 | +----------------------------------------------------+
 | | bit  31:0 R  AFRAMESRXOK_LSB                       |
 | +----------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AFRAMESRXOK_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AFRAMESRXOK_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000808c STATISTIC_CNT_AFRAMESRXOK_MSB_ |
 | +----------------------------------------------------+
 | | bit  7:0 R  AFRAMESRXOK_MSB                        |
 | +----------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AFRAMESRXOK_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AFRAMESRXOK_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register 0x00008090 STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB_ |
 | +--------------------------------------------------------------+
 | | bit  31:0 R  AFRAMECHECKSEQUENCEERRORS_LSB                   |
 | +--------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AFRAMECHECKSEQUENCEERRORS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AFRAMECHECKSEQUENCEERRORS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register 0x00008094 STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB_ |
 | +--------------------------------------------------------------+
 | | bit  7:0 R  AFRAMECHECKSEQUENCEERRORS_MSB                    |
 | +--------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AFRAMECHECKSEQUENCEERRORS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AFRAMECHECKSEQUENCEERRORS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register 0x00008098 STATISTIC_CNT_AALIGNMENTERRS_LSB_ |
 | +-------------------------------------------------------+
 | | bit  31:0 R  AALIGNMENTERRORS_LSB                     |
 | +-------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AALIGNMENTERRORS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AALIGNMENTERRORS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register 0x0000809c STATISTIC_CNT_AALIGNMENTERRS_MSB_ |
 | +-------------------------------------------------------+
 | | bit  7:0 R  AALIGNMENTERRORS_MSB                      |
 | +-------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AALIGNMENTERRORS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AALIGNMENTERRORS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register 0x000080a0 STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB_ |
 | +--------------------------------------------------------------+
 | | bit  31:0 R  APAUSEMACCTRLFRAMESTX_LSB                       |
 | +--------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_APAUSEMACCTRLFRAMESTX_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_APAUSEMACCTRLFRAMESTX_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register 0x000080a4 STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB_ |
 | +--------------------------------------------------------------+
 | | bit  7:0 R  APAUSEMACCTRLFRAMESTX_MSB                        |
 | +--------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_APAUSEMACCTRLFRAMESTX_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_APAUSEMACCTRLFRAMESTX_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register 0x000080a8 STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB_ |
 | +--------------------------------------------------------------+
 | | bit  31:0 R  APAUSEMACCTRLFRAMESRX_LSB                       |
 | +--------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_APAUSEMACCTRLFRAMESRX_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_APAUSEMACCTRLFRAMESRX_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register 0x000080ac STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB_ |
 | +--------------------------------------------------------------+
 | | bit  7:0 R  APAUSEMACCTRLFRAMESRX_MSB                        |
 | +--------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_APAUSEMACCTRLFRAMESRX_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_APAUSEMACCTRLFRAMESRX_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register 0x000080b0 STATISTIC_CNT_AFRAMETOOLONGERRS_LSB_ |
 | +----------------------------------------------------------+
 | | bit  31:0 R  AFRAMETOOLONGERRORS_LSB                     |
 | +----------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AFRAMETOOLONGERRORS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AFRAMETOOLONGERRORS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register 0x000080b4 STATISTIC_CNT_AFRAMETOOLONGERRS_MSB_ |
 | +----------------------------------------------------------+
 | | bit  7:0 R  AFRAMETOOLONGERRORS_MSB                      |
 | +----------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AFRAMETOOLONGERRORS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AFRAMETOOLONGERRORS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x000080b8 STATISTIC_CNT_AINRANGELENGTHERRS_LSB_ |
 | +-----------------------------------------------------------+
 | | bit  31:0 R  AINRANGELENGTHERRORS_LSB                     |
 | +-----------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AINRANGELENGTHERRORS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AINRANGELENGTHERRORS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x000080bc STATISTIC_CNT_AINRANGELENGTHERRS_MSB_ |
 | +-----------------------------------------------------------+
 | | bit  7:0 R  AINRANGELENGTHERRORS_MSB                      |
 | +-----------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AINRANGELENGTHERRORS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AINRANGELENGTHERRORS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000080c0 STATISTIC_CNT_VLANTXOK_LSB_ |
 | +-------------------------------------------------+
 | | bit  31:0 R  VLANTXOK_LSB                       |
 | +-------------------------------------------------+
 | 
 | UINT32 emac_mac_field_VLANTXOK_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_VLANTXOK_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000080c4 STATISTIC_CNT_VLANTXOK_MSB_ |
 | +-------------------------------------------------+
 | | bit  7:0 R  VLANTXOK_MSB                        |
 | +-------------------------------------------------+
 | 
 | UINT32 emac_mac_field_VLANTXOK_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_VLANTXOK_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000080c8 STATISTIC_CNT_VLANRXOK_LSB_ |
 | +-------------------------------------------------+
 | | bit  31:0 R  VLANRXOK_LSB                       |
 | +-------------------------------------------------+
 | 
 | UINT32 emac_mac_field_VLANRXOK_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_VLANRXOK_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000080cc STATISTIC_CNT_VLANRXOK_MSB_ |
 | +-------------------------------------------------+
 | | bit  7:0 R  VLANRXOK_MSB                        |
 | +-------------------------------------------------+
 | 
 | UINT32 emac_mac_field_VLANRXOK_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_VLANRXOK_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x000080d0 STATISTIC_CNT_IFOUTOCTS_LSB_ |
 | +--------------------------------------------------+
 | | bit  31:0 R  IFOUTOCTS_LSB                       |
 | +--------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFOUTOCTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFOUTOCTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x000080d4 STATISTIC_CNT_IFOUTOCTS_MSB_ |
 | +--------------------------------------------------+
 | | bit  7:0 R  IFOUTOCTS_MSB                        |
 | +--------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFOUTOCTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFOUTOCTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000080d8 STATISTIC_CNT_IFINOCTS_LSB_ |
 | +-------------------------------------------------+
 | | bit  31:0 R  IFINOCTS_LSB                       |
 | +-------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFINOCTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFINOCTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000080dc STATISTIC_CNT_IFINOCTS_MSB_ |
 | +-------------------------------------------------+
 | | bit  7:0 R  IFINOCTS_MSB                        |
 | +-------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFINOCTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFINOCTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x000080e0 STATISTIC_CNT_IFINUCASTPKTS_LSB_ |
 | +------------------------------------------------------+
 | | bit  31:0 R  IFINUCASTPKTS_LSB                       |
 | +------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFINUCASTPKTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFINUCASTPKTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x000080e4 STATISTIC_CNT_IFINUCASTPKTS_MSB_ |
 | +------------------------------------------------------+
 | | bit  7:0 R  IFINUCASTPKTS_MSB                        |
 | +------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFINUCASTPKTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFINUCASTPKTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register 0x000080e8 STATISTIC_CNT_IFINMULTICASTPKTS_LSB_ |
 | +----------------------------------------------------------+
 | | bit  31:0 R  IFINMULTICASTPKTS_LSB                       |
 | +----------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFINMULTICASTPKTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFINMULTICASTPKTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register 0x000080ec STATISTIC_CNT_IFINMULTICASTPKTS_MSB_ |
 | +----------------------------------------------------------+
 | | bit  7:0 R  IFINMULTICASTPKTS_MSB                        |
 | +----------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFINMULTICASTPKTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFINMULTICASTPKTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register 0x000080f0 STATISTIC_CNT_IFINBROADCASTPKTS_LSB_ |
 | +----------------------------------------------------------+
 | | bit  31:0 R  IFINBROADCASTPKTS_LSB                       |
 | +----------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFINBROADCASTPKTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFINBROADCASTPKTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register 0x000080f4 STATISTIC_CNT_IFINBROADCASTPKTS_MSB_ |
 | +----------------------------------------------------------+
 | | bit  7:0 R  IFINBROADCASTPKTS_MSB                        |
 | +----------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFINBROADCASTPKTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFINBROADCASTPKTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x000080f8 STATISTIC_CNT_IFOUTERRS_LSB_ |
 | +--------------------------------------------------+
 | | bit  31:0 R  IFOUTERRORS_LSB                     |
 | +--------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFOUTERRORS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFOUTERRORS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x000080fc STATISTIC_CNT_IFOUTERRS_MSB_ |
 | +--------------------------------------------------+
 | | bit  7:0 R  IFOUTERRORS_MSB                      |
 | +--------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFOUTERRORS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFOUTERRORS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register 0x00008108 STATISTIC_CNT_IFOUTUCASTPKTS_LSB_ |
 | +-------------------------------------------------------+
 | | bit  31:0 R  IFOUTUCASTPKTS_LSB                       |
 | +-------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFOUTUCASTPKTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFOUTUCASTPKTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register 0x0000810c STATISTIC_CNT_IFOUTUCASTPKTS_MSB_ |
 | +-------------------------------------------------------+
 | | bit  7:0 R  IFOUTUCASTPKTS_MSB                        |
 | +-------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFOUTUCASTPKTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFOUTUCASTPKTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00008110 STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB_ |
 | +-----------------------------------------------------------+
 | | bit  31:0 R  IFOUTMULTICASTPKTS_LSB                       |
 | +-----------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFOUTMULTICASTPKTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFOUTMULTICASTPKTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00008114 STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB_ |
 | +-----------------------------------------------------------+
 | | bit  7:0 R  IFOUTMULTICASTPKTS_MSB                        |
 | +-----------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFOUTMULTICASTPKTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFOUTMULTICASTPKTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x00008118 STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB_ |
 | +-----------------------------------------------------------+
 | | bit  31:0 R  IFOUTBROADCASTPKTS_LSB                       |
 | +-----------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFOUTBROADCASTPKTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFOUTBROADCASTPKTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register 0x0000811c STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB_ |
 | +-----------------------------------------------------------+
 | | bit  7:0 R  IFOUTBROADCASTPKTS_MSB                        |
 | +-----------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFOUTBROADCASTPKTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFOUTBROADCASTPKTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x00008120 STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ETHERSTATSDROPEVENTS_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSDROPEVENTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSDROPEVENTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x00008124 STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ETHERSTATSDROPEVENTS_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSDROPEVENTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSDROPEVENTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register 0x00008128 STATISTIC_CNT_ETHERSTATSOCTS_LSB_ |
 | +-------------------------------------------------------+
 | | bit  31:0 R  ETHERSTATSOCTS_LSB                       |
 | +-------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSOCTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSOCTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register 0x0000812c STATISTIC_CNT_ETHERSTATSOCTS_MSB_ |
 | +-------------------------------------------------------+
 | | bit  7:0 R  ETHERSTATSOCTS_MSB                        |
 | +-------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSOCTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSOCTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register 0x00008130 STATISTIC_CNT_ETHERSTATSPKTS_LSB_ |
 | +-------------------------------------------------------+
 | | bit  31:0 R  ETHERSTATSPKTS_LSB                       |
 | +-------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register 0x00008134 STATISTIC_CNT_ETHERSTATSPKTS_MSB_ |
 | +-------------------------------------------------------+
 | | bit  7:0 R  ETHERSTATSPKTS_MSB                        |
 | +-------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------+
 | | Register 0x00008138 STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB_ |
 | +----------------------------------------------------------------+
 | | bit  31:0 R  ETHERSTATSUNDERSIZEPKTS_LSB                       |
 | +----------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSUNDERSIZEPKTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSUNDERSIZEPKTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------+
 | | Register 0x0000813c STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB_ |
 | +----------------------------------------------------------------+
 | | bit  7:0 R  ETHERSTATSUNDERSIZEPKTS_MSB                        |
 | +----------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSUNDERSIZEPKTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSUNDERSIZEPKTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x00008140 STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ETHERSTATSPKTS64OCTS_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS64OCTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS64OCTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x00008144 STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ETHERSTATSPKTS64OCTS_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS64OCTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS64OCTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------+
 | | Register 0x00008148 STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB_ |
 | +------------------------------------------------------------------+
 | | bit  31:0 R  ETHERSTATSPKTS65TO127OCTS_LSB                       |
 | +------------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS65TO127OCTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS65TO127OCTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------------+
 | | Register 0x0000814c STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB_ |
 | +------------------------------------------------------------------+
 | | bit  7:0 R  ETHERSTATSPKTS65TO127OCTS_MSB                        |
 | +------------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS65TO127OCTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS65TO127OCTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x00008150 STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB_ |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R  ETHERSTATSPKTS128TO255OCTS_LSB                       |
 | +-------------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS128TO255OCTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS128TO255OCTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x00008154 STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB_ |
 | +-------------------------------------------------------------------+
 | | bit  7:0 R  ETHERSTATSPKTS128TO255OCTS_MSB                        |
 | +-------------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS128TO255OCTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS128TO255OCTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x00008158 STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB_ |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R  ETHERSTATSPKTS256TO511OCTS_LSB                       |
 | +-------------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS256TO511OCTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS256TO511OCTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register 0x0000815c STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB_ |
 | +-------------------------------------------------------------------+
 | | bit  7:0 R  ETHERSTATSPKTS256TO511OCTS_MSB                        |
 | +-------------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS256TO511OCTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS256TO511OCTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------+
 | | Register 0x00008160 STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB_ |
 | +--------------------------------------------------------------------+
 | | bit  31:0 R  ETHERSTATSPKTS512TO1023OCTS_LSB                       |
 | +--------------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS512TO1023OCTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS512TO1023OCTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------+
 | | Register 0x00008164 STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB_ |
 | +--------------------------------------------------------------------+
 | | bit  7:0 R  ETHERSTATSPKTS512TO1023OCTS_MSB                        |
 | +--------------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS512TO1023OCTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS512TO1023OCTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------------+
 | | Register 0x00008168 STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB_ |
 | +---------------------------------------------------------------------+
 | | bit  31:0 R  ETHERSTATSPKTS1024TO1518OCTS_LSB                       |
 | +---------------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS1024TO1518OCTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS1024TO1518OCTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------------+
 | | Register 0x0000816c STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB_ |
 | +---------------------------------------------------------------------+
 | | bit  7:0 R  ETHERSTATSPKTS1024TO1518OCTS_MSB                        |
 | +---------------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS1024TO1518OCTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS1024TO1518OCTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------+
 | | Register 0x00008170 STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB_ |
 | +--------------------------------------------------------------------+
 | | bit  31:0 R  ETHERSTATSPKTS1519TOMAXOCTS_LSB                       |
 | +--------------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS1519TOMAXOCTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS1519TOMAXOCTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------+
 | | Register 0x00008174 STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB_ |
 | +--------------------------------------------------------------------+
 | | bit  7:0 R  ETHERSTATSPKTS1519TOMAXOCTS_MSB                        |
 | +--------------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSPKTS1519TOMAXOCTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSPKTS1519TOMAXOCTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register 0x00008178 STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB_ |
 | +---------------------------------------------------------------+
 | | bit  31:0 R  ETHERSTATSOVERSIZEPKTS_LSB                       |
 | +---------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSOVERSIZEPKTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSOVERSIZEPKTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register 0x0000817c STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB_ |
 | +---------------------------------------------------------------+
 | | bit  7:0 R  ETHERSTATSOVERSIZEPKTS_MSB                        |
 | +---------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSOVERSIZEPKTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSOVERSIZEPKTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register 0x00008180 STATISTIC_CNT_ETHERSTATSJABBER_LSB_ |
 | +---------------------------------------------------------+
 | | bit  31:0 R  ETHERSTATSJABBER_LSB                       |
 | +---------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSJABBER_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSJABBER_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register 0x00008184 STATISTIC_CNT_ETHERSTATSJABBER_MSB_ |
 | +---------------------------------------------------------+
 | | bit  7:0 R  ETHERSTATSJABBER_MSB                        |
 | +---------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSJABBER_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSJABBER_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register 0x00008188 STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB_ |
 | +------------------------------------------------------------+
 | | bit  31:0 R  ETHERSTATSFRAGMENTS_LSB                       |
 | +------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSFRAGMENTS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSFRAGMENTS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register 0x0000818c STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB_ |
 | +------------------------------------------------------------+
 | | bit  7:0 R  ETHERSTATSFRAGMENTS_MSB                        |
 | +------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ETHERSTATSFRAGMENTS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ETHERSTATSFRAGMENTS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00008190 STATISTIC_CNT_IFINERRS_LSB_ |
 | +-------------------------------------------------+
 | | bit  31:0 R  IFINERRORS_LSB                     |
 | +-------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFINERRORS_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFINERRORS_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00008194 STATISTIC_CNT_IFINERRS_MSB_ |
 | +-------------------------------------------------+
 | | bit  7:0 R  IFINERRORS_MSB                      |
 | +-------------------------------------------------+
 | 
 | UINT32 emac_mac_field_IFINERRORS_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_IFINERRORS_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x00008198 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESTX_0_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_0_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_0_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x0000819c STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESTX_0_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_0_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_0_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081a0 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBCFPAUSEFRAMESTX_1_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBCFPAUSEFRAMESTX_1_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBCFPAUSEFRAMESTX_1_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081a4 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBCFPAUSEFRAMESTX_1_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBCFPAUSEFRAMESTX_1_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBCFPAUSEFRAMESTX_1_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081a8 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESTX_2_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_2_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_2_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081ac STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESTX_2_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_2_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_2_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081b0 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESTX_3_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_3_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_3_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081b4 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESTX_3_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_3_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_3_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081b8 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESTX_4_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_4_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_4_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081bc STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESTX_4_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_4_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_4_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081c0 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESTX_5_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_5_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_5_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081c4 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESTX_5_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_5_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_5_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081c8 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESTX_6_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_6_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_6_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081cc STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESTX_6_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_6_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_6_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081d0 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESTX_7_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_7_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_7_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081d4 STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESTX_7_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESTX_7_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESTX_7_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081d8 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESRX_0_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_0_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_0_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081dc STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESRX_0_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_0_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_0_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081e0 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESRX_1_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_1_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_1_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081e4 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESRX_1_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_1_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_1_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081e8 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESRX_2_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_2_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_2_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081ec STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESRX_2_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_2_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_2_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081f0 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESRX_3_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_3_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_3_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081f4 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESRX_3_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_3_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_3_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081f8 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESRX_4_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_4_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_4_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x000081fc STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESRX_4_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_4_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_4_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x00008200 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESRX_5_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_5_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_5_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x00008204 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESRX_5_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_5_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_5_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x00008208 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESRX_6_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_6_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_6_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x0000820c STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESRX_6_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_6_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_6_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x00008210 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB_ |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  ACBFCPAUSEFRAMESRX_7_LSB                       |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_7_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_7_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register 0x00008214 STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB_ |
 | +-------------------------------------------------------------+
 | | bit  7:0 R  ACBFCPAUSEFRAMESRX_7_MSB                        |
 | +-------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_ACBFCPAUSEFRAMESRX_7_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_ACBFCPAUSEFRAMESRX_7_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register 0x00008218 STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB_ |
 | +------------------------------------------------------------+
 | | bit  31:0 R  AMACCONTROLFRAMESTX_LSB                       |
 | +------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AMACCONTROLFRAMESTX_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AMACCONTROLFRAMESTX_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register 0x0000821c STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB_ |
 | +------------------------------------------------------------+
 | | bit  7:0 R  AMACCONTROLFRAMESTX_MSB                        |
 | +------------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AMACCONTROLFRAMESTX_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AMACCONTROLFRAMESTX_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register 0x00008220 STATISTIC_CNT_AMACCTRLFRAMESRX_LSB_ |
 | +---------------------------------------------------------+
 | | bit  31:0 R  AMACCONTROLFRAMESRX_LSB                    |
 | +---------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AMACCONTROLFRAMESRX_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AMACCONTROLFRAMESRX_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register 0x00008224 STATISTIC_CNT_AMACCTRLFRAMESRX_MSB_ |
 | +---------------------------------------------------------+
 | | bit  7:0 R  AMACCONTROLFRAMESRX_MSB                     |
 | +---------------------------------------------------------+
 | 
 | UINT32 emac_mac_field_AMACCONTROLFRAMESRX_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_mac_field_range_AMACCONTROLFRAMESRX_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00008280 TX_PREAMBLE_LSB_ |
 | +--------------------------------------+
 | | bit  31:0 R/W  TX_PREAMBLE_LSB       |
 | +--------------------------------------+
 | 
 | void   emac_mac_field_TX_PREAMBLE_LSB_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_TX_PREAMBLE_LSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_TX_PREAMBLE_LSB_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_TX_PREAMBLE_LSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00008284 TX_PREAMBLE_MSB_ |
 | +--------------------------------------+
 | | bit  23:0 R/W  TX_PREAMBLE_MSB       |
 | +--------------------------------------+
 | 
 | void   emac_mac_field_TX_PREAMBLE_MSB_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_mac_field_TX_PREAMBLE_MSB_get       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_mac_field_range_TX_PREAMBLE_MSB_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_mac_field_range_TX_PREAMBLE_MSB_get ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _EMAC_MAC_IO_H
#define _EMAC_MAC_IO_H

#include "emac_loc.h"
#include "emac_mac_regs.h"
#include "emac_mac_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 emac_mac_buffer_init               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void                 emac_mac_buffer_flush              ( emac_mac_buffer_t *b_ptr )
 | UINT32               emac_mac_reg_read                  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 emac_mac_reg_write                 ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 emac_mac_field_set                 ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 emac_mac_action_on_write_field_set ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 emac_mac_burst_read                ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 emac_mac_burst_write               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE emac_mac_poll                      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   emac_mac_reg_CTRL_AND_CFG_write                                   ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_CTRL_AND_CFG_field_set                               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_CTRL_AND_CFG_read                                    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_MAC_ADR_LSB__write                                   ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_MAC_ADR_LSB__field_set                               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_MAC_ADR_LSB__read                                    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_MAC_ADR_MSB__write                                   ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_MAC_ADR_MSB__field_set                               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_MAC_ADR_MSB__read                                    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_FRAME_LENGTH_write                                   ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_FRAME_LENGTH_field_set                               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_FRAME_LENGTH_read                                    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_TX_INTER_PACKET_GAP_IPG__write                       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_TX_INTER_PACKET_GAP_IPG__field_set                   ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_TX_INTER_PACKET_GAP_IPG__read                        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_RX_APPLICATION_CREDIT_RESET_write                    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_RX_APPLICATION_CREDIT_RESET_field_set                ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_RX_APPLICATION_CREDIT_RESET_read                     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_write           ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_field_set       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_RX_APPLICATION_CREDIT_INITIALIZATION_read            ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_write                         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_field_set                     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_read                          ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_write                         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_field_set                     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_read                          ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_write                         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_field_set                     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_read                          ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_write                         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_field_set                     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_read                          ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_write                      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_field_set                  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_CLASS_0_1_PAUSE_QUANTA_TH_read                       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_write                      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_field_set                  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_CLASS_2_3_PAUSE_QUANTA_TH_read                       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_write                      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_field_set                  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_CLASS_4_5_PAUSE_QUANTA_TH_read                       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_write                      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_field_set                  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_CLASS_6_7_PAUSE_QUANTA_TH_read                       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_TX_PREAMBLE_LSB__write                               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_TX_PREAMBLE_LSB__field_set                           ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_TX_PREAMBLE_LSB__read                                ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_TX_PREAMBLE_MSB__write                               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_TX_PREAMBLE_MSB__field_set                           ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_TX_PREAMBLE_MSB__read                                ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_mac_reg_SCRATCH_write                                        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_mac_reg_SCRATCH_field_set                                    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_mac_reg_SCRATCH_read                                         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_REVISION_read                                        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STAT_read                                            ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_RX_PAUSE_STAT_read                                   ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMESTXOK_LSB__read                  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMESTXOK_MSB__read                  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMESRXOK_LSB__read                  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMESRXOK_MSB__read                  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_LSB__read        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMECHKSEQUENCEERRS_MSB__read        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AALIGNMENTERRS_LSB__read               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AALIGNMENTERRS_MSB__read               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_LSB__read        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESTX_MSB__read        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_LSB__read        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_APAUSEMACCTRLFRAMESRX_MSB__read        ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMETOOLONGERRS_LSB__read            ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AFRAMETOOLONGERRS_MSB__read            ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AINRANGELENGTHERRS_LSB__read           ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AINRANGELENGTHERRS_MSB__read           ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_VLANTXOK_LSB__read                     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_VLANTXOK_MSB__read                     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_VLANRXOK_LSB__read                     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_VLANRXOK_MSB__read                     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTOCTS_LSB__read                    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTOCTS_MSB__read                    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFINOCTS_LSB__read                     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFINOCTS_MSB__read                     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFINUCASTPKTS_LSB__read                ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFINUCASTPKTS_MSB__read                ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFINMULTICASTPKTS_LSB__read            ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFINMULTICASTPKTS_MSB__read            ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFINBROADCASTPKTS_LSB__read            ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFINBROADCASTPKTS_MSB__read            ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTERRS_LSB__read                    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTERRS_MSB__read                    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTUCASTPKTS_LSB__read               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTUCASTPKTS_MSB__read               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTMULTICASTPKTS_LSB__read           ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTMULTICASTPKTS_MSB__read           ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTBROADCASTPKTS_LSB__read           ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFOUTBROADCASTPKTS_MSB__read           ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSDROPEVENTS_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSDROPEVENTS_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSOCTS_LSB__read               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSOCTS_MSB__read               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS_LSB__read               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS_MSB__read               ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_LSB__read      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSUNDERSIZEPKTS_MSB__read      ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS64OCTS_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_LSB__read    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS65TO127OCTS_MSB__read    ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_LSB__read   ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS128TO255OCTS_MSB__read   ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_LSB__read   ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS256TO511OCTS_MSB__read   ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_LSB__read  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS512TO1023OCTS_MSB__read  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_LSB__read ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1024TO1518OCTS_MSB__read ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_LSB__read  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSPKTS1519TOMAXOCTS_MSB__read  ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_LSB__read       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSOVERSIZEPKTS_MSB__read       ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSJABBER_LSB__read             ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSJABBER_MSB__read             ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSFRAGMENTS_LSB__read          ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ETHERSTATSFRAGMENTS_MSB__read          ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFINERRS_LSB__read                     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_IFINERRS_MSB__read                     ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_0_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_1_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_2_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_3_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_4_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_5_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_6_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESTX_7_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_0_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_1_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_2_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_3_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_4_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_5_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_6_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_LSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_ACBFCPAUSEFRAMESRX_7_MSB__read         ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESTXTED_LSB__read          ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESTXTED_MSB__read          ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESRX_LSB__read             ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_mac_reg_STATISTIC_CNT_AMACCTRLFRAMESRX_MSB__read             ( emac_mac_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _EMAC_MAC_IO_H */
