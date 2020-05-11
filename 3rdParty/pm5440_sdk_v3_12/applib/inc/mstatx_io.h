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
 *     and register accessor functions for the mstatx block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing mstatx io functions is:
 | {
 |     mstatx_buffer_t b_ptr[1];
 |     mstatx_buffer_init( b_ptr, h_ptr );
 |     value = mstatx_field_<FIELD>_get( b_ptr, h_ptr );
 |     mstatx_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     mstatx_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = mstatx_field_<FIELD>_get( NULL, h_ptr );
 | mstatx_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------------+
 | | Register 0x00001000 INT_1                     |
 | +-----------------------------------------------+
 | | bit  15 R/W  RX_LONG_LENGTH_ERROR_I           |
 | | bit  14 R/W  RX_IN_RANGE_LENGTH_ERROR_I       |
 | | bit  13 R/W  RX_LAYER1_ERROR_I                |
 | | bit  12 R/W  RX_SYMBOL_ERROR_I                |
 | | bit  10 R/W  RX_FCS_ERROR_I                   |
 | | bit  9  R/W  RX_MAC_CONTROL_FRAMES_OK_I       |
 | | bit  8  R/W  RX_MAC_PAUSE_CONTROL_FRAMES_OK_I |
 | | bit  7  R/W  RX_TAGGED_FRAMES_OK_I            |
 | | bit  6  R/W  RX_MULTICAST_FRAMES_OK_I         |
 | | bit  5  R/W  RX_BROADCAST_FRAMES_OK_I         |
 | | bit  4  R/W  RX_UNICAST_FRAMES_OK_I           |
 | | bit  3  R/W  RX_OCTETS_I                      |
 | | bit  2  R/W  RX_FRAMES_I                      |
 | | bit  1  R/W  RX_OCTETS_OK_I                   |
 | | bit  0  R/W  RX_FRAMES_OK_I                   |
 | +-----------------------------------------------+
 | 
 | void   mstatx_field_RX_LONG_LENGTH_ERROR_I_set           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_LONG_LENGTH_ERROR_I_get           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_IN_RANGE_LENGTH_ERROR_I_set       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_IN_RANGE_LENGTH_ERROR_I_get       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_LAYER1_ERROR_I_set                ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_LAYER1_ERROR_I_get                ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_SYMBOL_ERROR_I_set                ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_SYMBOL_ERROR_I_get                ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FCS_ERROR_I_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FCS_ERROR_I_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_MAC_CONTROL_FRAMES_OK_I_set       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_MAC_CONTROL_FRAMES_OK_I_get       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_I_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_I_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_TAGGED_FRAMES_OK_I_set            ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_TAGGED_FRAMES_OK_I_get            ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_MULTICAST_FRAMES_OK_I_set         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_MULTICAST_FRAMES_OK_I_get         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_BROADCAST_FRAMES_OK_I_set         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_BROADCAST_FRAMES_OK_I_get         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_UNICAST_FRAMES_OK_I_set           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_UNICAST_FRAMES_OK_I_get           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_OCTETS_I_set                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_OCTETS_I_get                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_I_set                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_I_get                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_OCTETS_OK_I_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_OCTETS_OK_I_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_OK_I_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_OK_I_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00001004 INT_2                    |
 | +----------------------------------------------+
 | | bit  11 R/W  RX_VARIABLE_I                   |
 | | bit  10 R/W  RX_STATION_ADDRESS_FILTERED_I   |
 | | bit  9  R/W  RX_FRAMES_1519_TO_MAX_SIZE_I    |
 | | bit  8  R/W  RX_FRAMES_1024_TO_1518_OCTETS_I |
 | | bit  7  R/W  RX_FRAMES_512_TO_1023_OCTETS_I  |
 | | bit  6  R/W  RX_FRAMES_256_TO_511_OCTETS_I   |
 | | bit  5  R/W  RX_FRAMES_128_TO_255_OCTETS_I   |
 | | bit  4  R/W  RX_FRAMES_65_TO_127_OCTETS_I    |
 | | bit  3  R/W  RX_FRAMES_64_OCTETS_I           |
 | | bit  2  R/W  RX_SHORT_LENGTH_CRC_ERROR_I     |
 | | bit  1  R/W  RX_SHORT_LENGTH_ERROR_I         |
 | | bit  0  R/W  RX_LONG_LENGTH_CRC_ERROR_I      |
 | +----------------------------------------------+
 | 
 | void   mstatx_field_RX_VARIABLE_I_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_VARIABLE_I_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_STATION_ADDRESS_FILTERED_I_set   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_STATION_ADDRESS_FILTERED_I_get   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_I_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_I_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_I_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_I_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_I_set  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_I_get  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_256_TO_511_OCTETS_I_set   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_256_TO_511_OCTETS_I_get   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_128_TO_255_OCTETS_I_set   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_128_TO_255_OCTETS_I_get   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_65_TO_127_OCTETS_I_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_65_TO_127_OCTETS_I_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_64_OCTETS_I_set           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_64_OCTETS_I_get           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_I_set     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_I_get     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_SHORT_LENGTH_ERROR_I_set         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_SHORT_LENGTH_ERROR_I_get         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_LONG_LENGTH_CRC_ERROR_I_set      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_LONG_LENGTH_CRC_ERROR_I_get      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00001008 INT_3                   |
 | +---------------------------------------------+
 | | bit  4 R/W  RX_FRAMES_2501_TO_MAX_SIZE_I    |
 | | bit  3 R/W  RX_FRAMES_2001_TO_2500_OCTETS_I |
 | | bit  2 R/W  RX_FRAMES_1549_TO_2000_OCTETS_I |
 | | bit  1 R/W  RX_FRAMES_1523_TO_1548_OCTETS_I |
 | | bit  0 R/W  RX_FRAMES_1519_TO_1522_OCTETS_I |
 | +---------------------------------------------+
 | 
 | void   mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_I_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_I_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_I_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_I_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_I_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_I_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_I_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_I_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_I_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_I_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +-----------------------------------------------+
 | | Register 0x0000100c INT_4                     |
 | +-----------------------------------------------+
 | | bit  15 R/W  TX_UNICAST_FRAMES_ATTEMPTED_I    |
 | | bit  14 R/W  TX_MULTICAST_FRAMES_OK_I         |
 | | bit  13 R/W  TX_BROADCAST_FRAMES_OK_I         |
 | | bit  12 R/W  TX_UNICAST_FRAMES_OK_I           |
 | | bit  11 R/W  TX_MAC_CONTROL_FRAMES_OK_I       |
 | | bit  10 R/W  TX_FRAMES_1523_TO_MAX_SIZE_I     |
 | | bit  9  R/W  TX_MAC_ERROR_I                   |
 | | bit  8  R/W  TX_SYSTEM_ERROR_I                |
 | | bit  7  R/W  TX_LONG_LENGTH_ERROR_I           |
 | | bit  6  R/W  TX_SHORT_LENGTH_ERROR_I          |
 | | bit  5  R/W  TX_FCS_ERROR_I                   |
 | | bit  4  R/W  TX_MAC_PAUSE_CONTROL_FRAMES_OK_I |
 | | bit  3  R/W  TX_TAGGED_FRAMES_OK_I            |
 | | bit  2  R/W  TX_OCTETS_I                      |
 | | bit  1  R/W  TX_OCTETS_OK_I                   |
 | | bit  0  R/W  TX_FRAMES_OK_I                   |
 | +-----------------------------------------------+
 | 
 | void   mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_I_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_I_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_MULTICAST_FRAMES_OK_I_set         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_MULTICAST_FRAMES_OK_I_get         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_BROADCAST_FRAMES_OK_I_set         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_BROADCAST_FRAMES_OK_I_get         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_UNICAST_FRAMES_OK_I_set           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_UNICAST_FRAMES_OK_I_get           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_MAC_CONTROL_FRAMES_OK_I_set       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_MAC_CONTROL_FRAMES_OK_I_get       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_I_set     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_I_get     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_MAC_ERROR_I_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_MAC_ERROR_I_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_SYSTEM_ERROR_I_set                ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_SYSTEM_ERROR_I_get                ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_LONG_LENGTH_ERROR_I_set           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_LONG_LENGTH_ERROR_I_get           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_SHORT_LENGTH_ERROR_I_set          ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_SHORT_LENGTH_ERROR_I_get          ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FCS_ERROR_I_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FCS_ERROR_I_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_I_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_I_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_TAGGED_FRAMES_OK_I_set            ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_TAGGED_FRAMES_OK_I_get            ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_OCTETS_I_set                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_OCTETS_I_get                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_OCTETS_OK_I_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_OCTETS_OK_I_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_OK_I_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_OK_I_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00001010 INT_5                    |
 | +----------------------------------------------+
 | | bit  15 R/W  TX_FRAMES_2501_TO_MAX_SIZE_I    |
 | | bit  14 R/W  TX_FRAMES_2001_TO_2500_OCTETS_I |
 | | bit  13 R/W  TX_FRAMES_1549_TO_2000_OCTETS_I |
 | | bit  12 R/W  TX_FRAMES_1523_TO_1548_OCTETS_I |
 | | bit  11 R/W  TX_FRAMES_1519_TO_1522_OCTETS_I |
 | | bit  10 R/W  RX_FRAMES_1523_TO_MAX_SIZE_I    |
 | | bit  9  R/W  TX_VARIABLE_I                   |
 | | bit  8  R/W  TX_FRAMES_1519_TO_MAX_SIZE_I    |
 | | bit  7  R/W  TX_FRAMES_1024_TO_1518_OCTETS_I |
 | | bit  6  R/W  TX_FRAMES_512_TO_1023_OCTETS_I  |
 | | bit  5  R/W  TX_FRAMES_256_TO_511_OCTETS_I   |
 | | bit  4  R/W  TX_FRAMES_128_TO_255_OCTETS_I   |
 | | bit  3  R/W  TX_FRAMES_65_TO_127_OCTETS_I    |
 | | bit  2  R/W  TX_FRAMES_64_OCTETS_I           |
 | | bit  1  R/W  TX_MULTICAST_FRAMES_ATTEMPTED_I |
 | | bit  0  R/W  TX_BROADCAST_FRAMES_ATTEMPTED_I |
 | +----------------------------------------------+
 | 
 | void   mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_I_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_I_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_I_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_I_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_I_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_I_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_I_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_I_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_I_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_I_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_I_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_I_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_VARIABLE_I_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_VARIABLE_I_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_I_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_I_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_I_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_I_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_I_set  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_I_get  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_256_TO_511_OCTETS_I_set   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_256_TO_511_OCTETS_I_get   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_128_TO_255_OCTETS_I_set   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_128_TO_255_OCTETS_I_get   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_65_TO_127_OCTETS_I_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_65_TO_127_OCTETS_I_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_64_OCTETS_I_set           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_64_OCTETS_I_get           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_I_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_I_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_I_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_I_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +-----------------------------------------------+
 | | Register 0x00001014 INT_EN_1                  |
 | +-----------------------------------------------+
 | | bit  15 R/W  RX_LONG_LENGTH_ERROR_E           |
 | | bit  14 R/W  RX_IN_RANGE_LENGTH_ERROR_E       |
 | | bit  13 R/W  RX_LAYER1_ERROR_E                |
 | | bit  12 R/W  RX_SYMBOL_ERROR_E                |
 | | bit  10 R/W  RX_FCS_ERROR_E                   |
 | | bit  9  R/W  RX_MAC_CONTROL_FRAMES_OK_E       |
 | | bit  8  R/W  RX_MAC_PAUSE_CONTROL_FRAMES_OK_E |
 | | bit  7  R/W  RX_TAGGED_FRAMES_OK_E            |
 | | bit  6  R/W  RX_MULTICAST_FRAMES_OK_E         |
 | | bit  5  R/W  RX_BROADCAST_FRAMES_OK_E         |
 | | bit  4  R/W  RX_UNICAST_FRAMES_OK_E           |
 | | bit  3  R/W  RX_OCTETS_E                      |
 | | bit  2  R/W  RX_FRAMES_E                      |
 | | bit  1  R/W  RX_OCTETS_OK_E                   |
 | | bit  0  R/W  RX_FRAMES_OK_E                   |
 | +-----------------------------------------------+
 | 
 | void   mstatx_field_RX_LONG_LENGTH_ERROR_E_set           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_LONG_LENGTH_ERROR_E_get           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_IN_RANGE_LENGTH_ERROR_E_set       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_IN_RANGE_LENGTH_ERROR_E_get       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_LAYER1_ERROR_E_set                ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_LAYER1_ERROR_E_get                ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_SYMBOL_ERROR_E_set                ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_SYMBOL_ERROR_E_get                ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FCS_ERROR_E_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FCS_ERROR_E_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_MAC_CONTROL_FRAMES_OK_E_set       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_MAC_CONTROL_FRAMES_OK_E_get       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_E_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_MAC_PAUSE_CONTROL_FRAMES_OK_E_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_TAGGED_FRAMES_OK_E_set            ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_TAGGED_FRAMES_OK_E_get            ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_MULTICAST_FRAMES_OK_E_set         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_MULTICAST_FRAMES_OK_E_get         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_BROADCAST_FRAMES_OK_E_set         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_BROADCAST_FRAMES_OK_E_get         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_UNICAST_FRAMES_OK_E_set           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_UNICAST_FRAMES_OK_E_get           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_OCTETS_E_set                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_OCTETS_E_get                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_E_set                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_E_get                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_OCTETS_OK_E_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_OCTETS_OK_E_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_OK_E_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_OK_E_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00001018 INT_EN_2                 |
 | +----------------------------------------------+
 | | bit  11 R/W  RX_VARIABLE_E                   |
 | | bit  10 R/W  RX_STATION_ADDRESS_FILTERED_E   |
 | | bit  9  R/W  RX_FRAMES_1519_TO_MAX_SIZE_E    |
 | | bit  8  R/W  RX_FRAMES_1024_TO_1518_OCTETS_E |
 | | bit  7  R/W  RX_FRAMES_512_TO_1023_OCTETS_E  |
 | | bit  6  R/W  RX_FRAMES_256_TO_511_OCTETS_E   |
 | | bit  5  R/W  RX_FRAMES_128_TO_255_OCTETS_E   |
 | | bit  4  R/W  RX_FRAMES_65_TO_127_OCTETS_E    |
 | | bit  3  R/W  RX_FRAMES_64_OCTETS_E           |
 | | bit  2  R/W  RX_SHORT_LENGTH_CRC_ERROR_E     |
 | | bit  1  R/W  RX_SHORT_LENGTH_ERROR_E         |
 | | bit  0  R/W  RX_LONG_LENGTH_CRC_ERROR_E      |
 | +----------------------------------------------+
 | 
 | void   mstatx_field_RX_VARIABLE_E_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_VARIABLE_E_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_STATION_ADDRESS_FILTERED_E_set   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_STATION_ADDRESS_FILTERED_E_get   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_E_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_1519_TO_MAX_SIZE_E_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_E_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_1024_TO_1518_OCTETS_E_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_E_set  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_512_TO_1023_OCTETS_E_get  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_256_TO_511_OCTETS_E_set   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_256_TO_511_OCTETS_E_get   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_128_TO_255_OCTETS_E_set   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_128_TO_255_OCTETS_E_get   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_65_TO_127_OCTETS_E_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_65_TO_127_OCTETS_E_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_64_OCTETS_E_set           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_64_OCTETS_E_get           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_E_set     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_SHORT_LENGTH_CRC_ERROR_E_get     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_SHORT_LENGTH_ERROR_E_set         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_SHORT_LENGTH_ERROR_E_get         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_LONG_LENGTH_CRC_ERROR_E_set      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_LONG_LENGTH_CRC_ERROR_E_get      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x0000101c INT_EN_3                |
 | +---------------------------------------------+
 | | bit  4 R/W  RX_FRAMES_2501_TO_MAX_SIZE_E    |
 | | bit  3 R/W  RX_FRAMES_2001_TO_2500_OCTETS_E |
 | | bit  2 R/W  RX_FRAMES_1549_TO_2000_OCTETS_E |
 | | bit  1 R/W  RX_FRAMES_1523_TO_1548_OCTETS_E |
 | | bit  0 R/W  RX_FRAMES_1519_TO_1522_OCTETS_E |
 | +---------------------------------------------+
 | 
 | void   mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_E_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_2501_TO_MAX_SIZE_E_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_E_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_2001_TO_2500_OCTETS_E_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_E_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_1549_TO_2000_OCTETS_E_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_E_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_1523_TO_1548_OCTETS_E_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_E_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_1519_TO_1522_OCTETS_E_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +-----------------------------------------------+
 | | Register 0x00001020 INT_EN_4                  |
 | +-----------------------------------------------+
 | | bit  15 R/W  TX_UNICAST_FRAMES_ATTEMPTED_E    |
 | | bit  14 R/W  TX_MULTICAST_FRAMES_OK_E         |
 | | bit  13 R/W  TX_BROADCAST_FRAMES_OK_E         |
 | | bit  12 R/W  TX_UNICAST_FRAMES_OK_E           |
 | | bit  11 R/W  TX_MAC_CONTROL_FRAMES_OK_E       |
 | | bit  10 R/W  TX_FRAMES_1523_TO_MAX_SIZE_E     |
 | | bit  9  R/W  TX_MAC_ERROR_E                   |
 | | bit  8  R/W  TX_SYSTEM_ERROR_E                |
 | | bit  7  R/W  TX_LONG_LENGTH_ERROR_E           |
 | | bit  6  R/W  TX_SHORT_LENGTH_ERROR_E          |
 | | bit  5  R/W  TX_FCS_ERROR_E                   |
 | | bit  4  R/W  TX_MAC_PAUSE_CONTROL_FRAMES_OK_E |
 | | bit  3  R/W  TX_TAGGED_FRAMES_OK_E            |
 | | bit  2  R/W  TX_OCTETS_E                      |
 | | bit  1  R/W  TX_OCTETS_OK_E                   |
 | | bit  0  R/W  TX_FRAMES_OK_E                   |
 | +-----------------------------------------------+
 | 
 | void   mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_E_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_UNICAST_FRAMES_ATTEMPTED_E_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_MULTICAST_FRAMES_OK_E_set         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_MULTICAST_FRAMES_OK_E_get         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_BROADCAST_FRAMES_OK_E_set         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_BROADCAST_FRAMES_OK_E_get         ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_UNICAST_FRAMES_OK_E_set           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_UNICAST_FRAMES_OK_E_get           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_MAC_CONTROL_FRAMES_OK_E_set       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_MAC_CONTROL_FRAMES_OK_E_get       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_E_set     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_1523_TO_MAX_SIZE_E_get     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_MAC_ERROR_E_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_MAC_ERROR_E_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_SYSTEM_ERROR_E_set                ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_SYSTEM_ERROR_E_get                ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_LONG_LENGTH_ERROR_E_set           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_LONG_LENGTH_ERROR_E_get           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_SHORT_LENGTH_ERROR_E_set          ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_SHORT_LENGTH_ERROR_E_get          ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FCS_ERROR_E_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FCS_ERROR_E_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_E_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_MAC_PAUSE_CONTROL_FRAMES_OK_E_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_TAGGED_FRAMES_OK_E_set            ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_TAGGED_FRAMES_OK_E_get            ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_OCTETS_E_set                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_OCTETS_E_get                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_OCTETS_OK_E_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_OCTETS_OK_E_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_OK_E_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_OK_E_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00001024 INT_EN_5                 |
 | +----------------------------------------------+
 | | bit  15 R/W  TX_FRAMES_2501_TO_MAX_SIZE_E    |
 | | bit  14 R/W  TX_FRAMES_2001_TO_2500_OCTETS_E |
 | | bit  13 R/W  TX_FRAMES_1549_TO_2000_OCTETS_E |
 | | bit  12 R/W  TX_FRAMES_1523_TO_1548_OCTETS_E |
 | | bit  11 R/W  TX_FRAMES_1519_TO_1522_OCTETS_E |
 | | bit  10 R/W  RX_FRAMES_1523_TO_MAX_SIZE_E    |
 | | bit  9  R/W  TX_VARIABLE_E                   |
 | | bit  8  R/W  TX_FRAMES_1519_TO_MAX_SIZE_E    |
 | | bit  7  R/W  TX_FRAMES_1024_TO_1518_OCTETS_E |
 | | bit  6  R/W  TX_FRAMES_512_TO_1023_OCTETS_E  |
 | | bit  5  R/W  TX_FRAMES_256_TO_511_OCTETS_E   |
 | | bit  4  R/W  TX_FRAMES_128_TO_255_OCTETS_E   |
 | | bit  3  R/W  TX_FRAMES_65_TO_127_OCTETS_E    |
 | | bit  2  R/W  TX_FRAMES_64_OCTETS_E           |
 | | bit  1  R/W  TX_MULTICAST_FRAMES_ATTEMPTED_E |
 | | bit  0  R/W  TX_BROADCAST_FRAMES_ATTEMPTED_E |
 | +----------------------------------------------+
 | 
 | void   mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_E_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_2501_TO_MAX_SIZE_E_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_E_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_2001_TO_2500_OCTETS_E_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_E_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_1549_TO_2000_OCTETS_E_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_E_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_1523_TO_1548_OCTETS_E_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_E_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_1519_TO_1522_OCTETS_E_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_E_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_FRAMES_1523_TO_MAX_SIZE_E_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_VARIABLE_E_set                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_VARIABLE_E_get                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_E_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_1519_TO_MAX_SIZE_E_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_E_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_1024_TO_1518_OCTETS_E_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_E_set  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_512_TO_1023_OCTETS_E_get  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_256_TO_511_OCTETS_E_set   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_256_TO_511_OCTETS_E_get   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_128_TO_255_OCTETS_E_set   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_128_TO_255_OCTETS_E_get   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_65_TO_127_OCTETS_E_set    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_65_TO_127_OCTETS_E_get    ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_FRAMES_64_OCTETS_E_set           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_FRAMES_64_OCTETS_E_get           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_E_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_MULTICAST_FRAMES_ATTEMPTED_E_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_E_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_BROADCAST_FRAMES_ATTEMPTED_E_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +----------------------------+
 | | Register 0x00001028 CTRL   |
 | +----------------------------+
 | | bit  15 R/W  LCLK_MODE     |
 | | bit  6  R/W  TX_LOAD       |
 | | bit  5  R/W  TX_SNAP_CLEAR |
 | | bit  4  R/W  TX_SNAP_POST  |
 | | bit  2  R/W  RX_LOAD       |
 | | bit  1  R/W  RX_SNAP_CLEAR |
 | | bit  0  R/W  RX_SNAP_POST  |
 | +----------------------------+
 | 
 | void   mstatx_field_LCLK_MODE_set     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_LCLK_MODE_get     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_LOAD_set       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_LOAD_get       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_SNAP_CLEAR_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_SNAP_CLEAR_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_TX_SNAP_POST_set  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_SNAP_POST_get  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_LOAD_set       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_LOAD_get       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_SNAP_CLEAR_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_SNAP_CLEAR_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_RX_SNAP_POST_set  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_SNAP_POST_get  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x0000102c VARIABLE_SELECT |
 | +-------------------------------------+
 | | bit  12:8 R/W  TX_INDEX             |
 | | bit  4:0  R/W  RX_INDEX             |
 | +-------------------------------------+
 | 
 | void   mstatx_field_TX_INDEX_set       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_TX_INDEX_get       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_range_TX_INDEX_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mstatx_field_range_TX_INDEX_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mstatx_field_RX_INDEX_set       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | UINT32 mstatx_field_RX_INDEX_get       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void   mstatx_field_range_RX_INDEX_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mstatx_field_range_RX_INDEX_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00001040 + (N) * 0x10) STATISTICAL_CNT_LOW |
 | |     N[0..64]                                           |
 | +--------------------------------------------------------+
 | | bit  15:0 R  COUNTL                                    |
 | +--------------------------------------------------------+
 | 
 | UINT32 mstatx_field_COUNTL_get       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32  N )
 | UINT32 mstatx_field_range_COUNTL_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00001044 + (N) * 0x10) STATISTICAL_CNT_MID |
 | |     N[0..64]                                           |
 | +--------------------------------------------------------+
 | | bit  15:0 R  COUNTM                                    |
 | +--------------------------------------------------------+
 | 
 | UINT32 mstatx_field_COUNTM_get       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32  N )
 | UINT32 mstatx_field_range_COUNTM_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00001048 + (N) * 0x10) STATISTICAL_CNT_HIGH |
 | |     N[0..64]                                            |
 | +---------------------------------------------------------+
 | | bit  7:0 R  COUNTH                                      |
 | +---------------------------------------------------------+
 | 
 | UINT32 mstatx_field_COUNTH_get       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32  N )
 | UINT32 mstatx_field_range_COUNTH_get ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MSTATX_IO_H
#define _MSTATX_IO_H

#include "enet_fege_loc.h"
#include "mstatx_regs.h"
#include "mstatx_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mstatx_buffer_init               ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | void                 mstatx_buffer_flush              ( mstatx_buffer_t *b_ptr )
 | UINT32               mstatx_reg_read                  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 mstatx_reg_write                 ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 mstatx_field_set                 ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 mstatx_action_on_write_field_set ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mstatx_burst_read                ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 mstatx_burst_write               ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE mstatx_poll                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   mstatx_reg_INT_1_write                     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   mstatx_reg_INT_1_field_set                 ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mstatx_reg_INT_1_read                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   mstatx_reg_INT_2_write                     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   mstatx_reg_INT_2_field_set                 ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mstatx_reg_INT_2_read                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   mstatx_reg_INT_3_write                     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   mstatx_reg_INT_3_field_set                 ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mstatx_reg_INT_3_read                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   mstatx_reg_INT_4_write                     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   mstatx_reg_INT_4_field_set                 ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mstatx_reg_INT_4_read                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   mstatx_reg_INT_5_write                     ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   mstatx_reg_INT_5_field_set                 ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mstatx_reg_INT_5_read                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   mstatx_reg_INT_EN_1_write                  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   mstatx_reg_INT_EN_1_field_set              ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mstatx_reg_INT_EN_1_read                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   mstatx_reg_INT_EN_2_write                  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   mstatx_reg_INT_EN_2_field_set              ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mstatx_reg_INT_EN_2_read                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   mstatx_reg_INT_EN_3_write                  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   mstatx_reg_INT_EN_3_field_set              ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mstatx_reg_INT_EN_3_read                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   mstatx_reg_INT_EN_4_write                  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   mstatx_reg_INT_EN_4_field_set              ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mstatx_reg_INT_EN_4_read                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   mstatx_reg_INT_EN_5_write                  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   mstatx_reg_INT_EN_5_field_set              ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mstatx_reg_INT_EN_5_read                   ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   mstatx_reg_CTRL_write                      ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   mstatx_reg_CTRL_field_set                  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mstatx_reg_CTRL_read                       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | void   mstatx_reg_VARIABLE_SELECT_write           ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 value )
 | void   mstatx_reg_VARIABLE_SELECT_field_set       ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 mstatx_reg_VARIABLE_SELECT_read            ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr )
 | 
 | UINT32 mstatx_reg_STATISTICAL_CNT_LOW_array_read  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32 mstatx_reg_STATISTICAL_CNT_MID_array_read  ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32 mstatx_reg_STATISTICAL_CNT_HIGH_array_read ( mstatx_buffer_t *b_ptr, enet_fege_handle_t *h_ptr, UINT32  N )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MSTATX_IO_H */
