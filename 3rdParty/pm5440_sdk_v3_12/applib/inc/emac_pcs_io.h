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
 *     and register accessor functions for the emac_pcs block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing emac_pcs io functions is:
 | {
 |     emac_pcs_buffer_t b_ptr[1];
 |     emac_pcs_buffer_init( b_ptr, h_ptr, A );
 |     value = emac_pcs_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     emac_pcs_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     emac_pcs_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = emac_pcs_field_<FIELD>_get( NULL, h_ptr, A );
 | emac_pcs_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------+
 | | Register 0x0000c000 CTRL_AND_STAT |
 | +-----------------------------------+
 | | bit  15  R/W  RESET               |
 | | bit  14  R/W  LOOPBACK            |
 | | bit  5:2 R    SPEED_SEL           |
 | +-----------------------------------+
 | 
 | void   emac_pcs_field_RESET_set           ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_pcs_field_RESET_get           ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_pcs_field_LOOPBACK_set        ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_pcs_field_LOOPBACK_get        ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_SPEED_SEL_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_SPEED_SEL_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x0000c004 STAT_1_2_ |
 | +-------------------------------+
 | | bit  2 R  LINK_STATUS_L       |
 | +-------------------------------+
 | 
 | UINT32 emac_pcs_field_LINK_STATUS_L_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------+
 | | Register 0x0000c008 DEVICE_ID_1_2_ |
 | +------------------------------------+
 | | bit  15:0 R  DEVID_LSB             |
 | +------------------------------------+
 | 
 | UINT32 emac_pcs_field_DEVID_LSB_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_DEVID_LSB_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x0000c00c DEVICE_ID_2_2_ |
 | +------------------------------------+
 | | bit  15:0 R  DEVID_MSB             |
 | +------------------------------------+
 | 
 | UINT32 emac_pcs_field_DEVID_MSB_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_DEVID_MSB_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x0000c01c CTRL_2_2_ |
 | +-------------------------------+
 | | bit  2:0 R  PCS_SEL           |
 | +-------------------------------+
 | 
 | UINT32 emac_pcs_field_PCS_SEL_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_PCS_SEL_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x0000c020 STAT_2_2_ |
 | +-------------------------------+
 | | bit  5:0 R  PCS_CFG_MODE      |
 | +-------------------------------+
 | 
 | UINT32 emac_pcs_field_PCS_CFG_MODE_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_PCS_CFG_MODE_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x0000c038 PACKAGE_ID_1_2_ |
 | +-------------------------------------+
 | | bit  15:0 R  PKGID_LSB              |
 | +-------------------------------------+
 | 
 | UINT32 emac_pcs_field_PKGID_LSB_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_PKGID_LSB_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x0000c03c PACKAGE_ID_2_2_ |
 | +-------------------------------------+
 | | bit  15:0 R  PKGID_MSB              |
 | +-------------------------------------+
 | 
 | UINT32 emac_pcs_field_PKGID_MSB_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_PKGID_MSB_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x0000c080 BASE_R_STAT_1_2_ |
 | +--------------------------------------+
 | | bit  12 R  LINK_STATUS               |
 | | bit  1  R  HIGH_BER                  |
 | | bit  0  R  BLOCK_LOCK                |
 | +--------------------------------------+
 | 
 | UINT32 emac_pcs_field_LINK_STATUS_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_HIGH_BER_get    ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_BLOCK_LOCK_get  ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------+
 | | Register 0x0000c084 BASE_R_STAT_2_2_ |
 | +--------------------------------------+
 | | bit  15   R  BLOCK_LOCK_L            |
 | | bit  14   R  HIGH_BER_L              |
 | | bit  13:8 R  BER_CNT_LSB             |
 | | bit  7:0  R  BLOCK_ERR_CNT_LSB       |
 | +--------------------------------------+
 | 
 | UINT32 emac_pcs_field_BLOCK_LOCK_L_get            ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_HIGH_BER_L_get              ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_BER_CNT_LSB_get             ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_BER_CNT_LSB_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 emac_pcs_field_BLOCK_ERR_CNT_LSB_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_BLOCK_ERR_CNT_LSB_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0000c088 BASE_R_SEED_A_1_4_ |
 | +----------------------------------------+
 | | bit  15:0 R/W  TEST_SEED_A_15_0        |
 | +----------------------------------------+
 | 
 | void   emac_pcs_field_TEST_SEED_A_15_0_set       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_pcs_field_TEST_SEED_A_15_0_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_pcs_field_range_TEST_SEED_A_15_0_set ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_pcs_field_range_TEST_SEED_A_15_0_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0000c08c BASE_R_SEED_A_2_4_ |
 | +----------------------------------------+
 | | bit  15:0 R/W  TEST_SEED_A_31_16       |
 | +----------------------------------------+
 | 
 | void   emac_pcs_field_TEST_SEED_A_31_16_set       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_pcs_field_TEST_SEED_A_31_16_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_pcs_field_range_TEST_SEED_A_31_16_set ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_pcs_field_range_TEST_SEED_A_31_16_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0000c090 BASE_R_SEED_A_3_4_ |
 | +----------------------------------------+
 | | bit  15:0 R/W  TEST_SEED_A_47_32       |
 | +----------------------------------------+
 | 
 | void   emac_pcs_field_TEST_SEED_A_47_32_set       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_pcs_field_TEST_SEED_A_47_32_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_pcs_field_range_TEST_SEED_A_47_32_set ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_pcs_field_range_TEST_SEED_A_47_32_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0000c094 BASE_R_SEED_A_4_4_ |
 | +----------------------------------------+
 | | bit  9:0 R/W  TEST_SEED_A_57_48        |
 | +----------------------------------------+
 | 
 | void   emac_pcs_field_TEST_SEED_A_57_48_set       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_pcs_field_TEST_SEED_A_57_48_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_pcs_field_range_TEST_SEED_A_57_48_set ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_pcs_field_range_TEST_SEED_A_57_48_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0000c098 BASE_R_SEED_B_1_4_ |
 | +----------------------------------------+
 | | bit  15:0 R/W  TEST_SEED_B_15_0        |
 | +----------------------------------------+
 | 
 | void   emac_pcs_field_TEST_SEED_B_15_0_set       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_pcs_field_TEST_SEED_B_15_0_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_pcs_field_range_TEST_SEED_B_15_0_set ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_pcs_field_range_TEST_SEED_B_15_0_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0000c09c BASE_R_SEED_B_2_4_ |
 | +----------------------------------------+
 | | bit  15:0 R/W  TEST_SEED_B_31_16       |
 | +----------------------------------------+
 | 
 | void   emac_pcs_field_TEST_SEED_B_31_16_set       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_pcs_field_TEST_SEED_B_31_16_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_pcs_field_range_TEST_SEED_B_31_16_set ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_pcs_field_range_TEST_SEED_B_31_16_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0000c0a0 BASE_R_SEED_B_3_4_ |
 | +----------------------------------------+
 | | bit  15:0 R/W  TEST_SEED_B_47_32       |
 | +----------------------------------------+
 | 
 | void   emac_pcs_field_TEST_SEED_B_47_32_set       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_pcs_field_TEST_SEED_B_47_32_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_pcs_field_range_TEST_SEED_B_47_32_set ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_pcs_field_range_TEST_SEED_B_47_32_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0000c0a4 BASE_R_SEED_B_4_4_ |
 | +----------------------------------------+
 | | bit  9:0 R/W  TEST_SEED_B_57_48        |
 | +----------------------------------------+
 | 
 | void   emac_pcs_field_TEST_SEED_B_57_48_set       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_pcs_field_TEST_SEED_B_57_48_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_pcs_field_range_TEST_SEED_B_57_48_set ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 emac_pcs_field_range_TEST_SEED_B_57_48_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x0000c0a8 BASE_R_TEST_CTRL |
 | +--------------------------------------+
 | | bit  3 R/W  TX_TEST_EN               |
 | | bit  2 R/W  RX_TEST_EN               |
 | | bit  1 R/W  TEST_PATTERN             |
 | | bit  0 R/W  DATA_PATTERN             |
 | +--------------------------------------+
 | 
 | void   emac_pcs_field_TX_TEST_EN_set   ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_pcs_field_TX_TEST_EN_get   ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_pcs_field_RX_TEST_EN_set   ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_pcs_field_RX_TEST_EN_get   ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_pcs_field_TEST_PATTERN_set ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_pcs_field_TEST_PATTERN_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void   emac_pcs_field_DATA_PATTERN_set ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 emac_pcs_field_DATA_PATTERN_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------+
 | | Register 0x0000c0ac BASE_R_TEST_ERR_CNT |
 | +-----------------------------------------+
 | | bit  15:0 R  TEST_ERR_CNT               |
 | +-----------------------------------------+
 | 
 | UINT32 emac_pcs_field_TEST_ERR_CNT_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_TEST_ERR_CNT_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x0000c0b0 BER_CNT_MSB_ |
 | +----------------------------------+
 | | bit  15:0 R  BER_CNT_MSB         |
 | +----------------------------------+
 | 
 | UINT32 emac_pcs_field_BER_CNT_MSB_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_BER_CNT_MSB_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0000c0b4 BLOCK_ERR_CNT_MSB_ |
 | +----------------------------------------+
 | | bit  13:0 R  BLOCK_ERR_CNT_MSB         |
 | +----------------------------------------+
 | 
 | UINT32 emac_pcs_field_BLOCK_ERR_CNT_MSB_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_BLOCK_ERR_CNT_MSB_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x0000c0b8 PMON_BLOCK_ERR_CNT_1_3_ |
 | +---------------------------------------------+
 | | bit  15:0 R  PMON_BLOCK_ERR_CNT_LSB         |
 | +---------------------------------------------+
 | 
 | UINT32 emac_pcs_field_PMON_BLOCK_ERR_CNT_LSB_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_PMON_BLOCK_ERR_CNT_LSB_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x0000c0bc PMON_BLOCK_ERR_CNT_2_3_ |
 | +---------------------------------------------+
 | | bit  15:0 R  PMON_BLOCK_ERR_CNT_MID         |
 | +---------------------------------------------+
 | 
 | UINT32 emac_pcs_field_PMON_BLOCK_ERR_CNT_MID_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_PMON_BLOCK_ERR_CNT_MID_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x0000c0c0 PMON_BLOCK_ERR_CNT_3_3_ |
 | +---------------------------------------------+
 | | bit  7:0 R  PMON_BLOCK_ERR_CNT_MSB          |
 | +---------------------------------------------+
 | 
 | UINT32 emac_pcs_field_PMON_BLOCK_ERR_CNT_MSB_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_PMON_BLOCK_ERR_CNT_MSB_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000c0c8 MULTI_LANE_ALIGNMENT_STAT_1_4_ |
 | +----------------------------------------------------+
 | | bit  12  R  ALL_LANES_LOCK                         |
 | | bit  7:0 R  LANE_7_0_BLOCK_LOCK                    |
 | +----------------------------------------------------+
 | 
 | UINT32 emac_pcs_field_ALL_LANES_LOCK_get            ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_LANE_7_0_BLOCK_LOCK_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_LANE_7_0_BLOCK_LOCK_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000c0cc MULTI_LANE_ALIGNMENT_STAT_2_4_ |
 | +----------------------------------------------------+
 | | bit  11:0 R  LANE_19_12_BLOCK_LOCK                 |
 | +----------------------------------------------------+
 | 
 | UINT32 emac_pcs_field_LANE_19_12_BLOCK_LOCK_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_LANE_19_12_BLOCK_LOCK_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000c0d0 MULTI_LANE_ALIGNMENT_STAT_3_4_ |
 | +----------------------------------------------------+
 | | bit  7:0 R  LANE_ALIGN_LOCK_7_0                    |
 | +----------------------------------------------------+
 | 
 | UINT32 emac_pcs_field_LANE_ALIGN_LOCK_7_0_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_LANE_ALIGN_LOCK_7_0_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x0000c0d4 MULTI_LANE_ALIGNMENT_STAT_4_4_ |
 | +----------------------------------------------------+
 | | bit  11:0 R  LANE_ALIGN_LOCK_19_12                 |
 | +----------------------------------------------------+
 | 
 | UINT32 emac_pcs_field_LANE_ALIGN_LOCK_19_12_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | UINT32 emac_pcs_field_range_LANE_ALIGN_LOCK_19_12_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x0000c168 + (N) * 4) LANE_BIP_ERR_CNT |
 | |     N[0..19]                                     |
 | +--------------------------------------------------+
 | | bit  15:0 R  LANE_BIP_ERR_CNT                    |
 | +--------------------------------------------------+
 | 
 | UINT32 emac_pcs_field_LANE_BIP_ERR_CNT_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 emac_pcs_field_range_LANE_BIP_ERR_CNT_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register (0x0000c1b8 + (N) * 4) LANE_MAPPING |
 | |     N[0..19]                                 |
 | +----------------------------------------------+
 | | bit  4:0 R  LANE_MAP                         |
 | +----------------------------------------------+
 | 
 | UINT32 emac_pcs_field_LANE_MAP_get       ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 emac_pcs_field_range_LANE_MAP_get ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _EMAC_PCS_IO_H
#define _EMAC_PCS_IO_H

#include "emac_loc.h"
#include "emac_pcs_regs.h"
#include "emac_pcs_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 emac_pcs_buffer_init               ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | void                 emac_pcs_buffer_flush              ( emac_pcs_buffer_t *b_ptr )
 | UINT32               emac_pcs_reg_read                  ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 emac_pcs_reg_write                 ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 emac_pcs_field_set                 ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 emac_pcs_action_on_write_field_set ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 emac_pcs_burst_read                ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 emac_pcs_burst_write               ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE emac_pcs_poll                      ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   emac_pcs_reg_CTRL_AND_STAT_write                 ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_pcs_reg_CTRL_AND_STAT_field_set             ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_pcs_reg_CTRL_AND_STAT_read                  ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_pcs_reg_BASE_R_SEED_A_1_4__write            ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_pcs_reg_BASE_R_SEED_A_1_4__field_set        ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_pcs_reg_BASE_R_SEED_A_1_4__read             ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_pcs_reg_BASE_R_SEED_A_2_4__write            ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_pcs_reg_BASE_R_SEED_A_2_4__field_set        ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_pcs_reg_BASE_R_SEED_A_2_4__read             ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_pcs_reg_BASE_R_SEED_A_3_4__write            ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_pcs_reg_BASE_R_SEED_A_3_4__field_set        ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_pcs_reg_BASE_R_SEED_A_3_4__read             ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_pcs_reg_BASE_R_SEED_A_4_4__write            ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_pcs_reg_BASE_R_SEED_A_4_4__field_set        ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_pcs_reg_BASE_R_SEED_A_4_4__read             ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_pcs_reg_BASE_R_SEED_B_1_4__write            ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_pcs_reg_BASE_R_SEED_B_1_4__field_set        ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_pcs_reg_BASE_R_SEED_B_1_4__read             ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_pcs_reg_BASE_R_SEED_B_2_4__write            ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_pcs_reg_BASE_R_SEED_B_2_4__field_set        ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_pcs_reg_BASE_R_SEED_B_2_4__read             ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_pcs_reg_BASE_R_SEED_B_3_4__write            ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_pcs_reg_BASE_R_SEED_B_3_4__field_set        ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_pcs_reg_BASE_R_SEED_B_3_4__read             ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_pcs_reg_BASE_R_SEED_B_4_4__write            ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_pcs_reg_BASE_R_SEED_B_4_4__field_set        ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_pcs_reg_BASE_R_SEED_B_4_4__read             ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | void   emac_pcs_reg_BASE_R_TEST_CTRL_write              ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   emac_pcs_reg_BASE_R_TEST_CTRL_field_set          ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 emac_pcs_reg_BASE_R_TEST_CTRL_read               ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_STAT_1_2__read                      ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_DEVICE_ID_1_2__read                 ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_DEVICE_ID_2_2__read                 ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_CTRL_2_2__read                      ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_STAT_2_2__read                      ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_PACKAGE_ID_1_2__read                ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_PACKAGE_ID_2_2__read                ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_BASE_R_STAT_1_2__read               ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_BASE_R_STAT_2_2__read               ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_BASE_R_TEST_ERR_CNT_read            ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_BER_CNT_MSB__read                   ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_BLOCK_ERR_CNT_MSB__read             ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_PMON_BLOCK_ERR_CNT_1_3__read        ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_PMON_BLOCK_ERR_CNT_2_3__read        ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_PMON_BLOCK_ERR_CNT_3_3__read        ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_1_4__read ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_2_4__read ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_3_4__read ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_MULTI_LANE_ALIGNMENT_STAT_4_4__read ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 emac_pcs_reg_LANE_BIP_ERR_CNT_array_read         ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32 emac_pcs_reg_LANE_MAPPING_array_read             ( emac_pcs_buffer_t *b_ptr, emac_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _EMAC_PCS_IO_H */
