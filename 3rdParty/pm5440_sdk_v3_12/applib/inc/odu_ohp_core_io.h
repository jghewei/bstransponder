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
 *     and register accessor functions for the odu_ohp_core block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing odu_ohp_core io functions is:
 | {
 |     odu_ohp_core_buffer_t b_ptr[1];
 |     odu_ohp_core_buffer_init( b_ptr, h_ptr );
 |     value = odu_ohp_core_field_<FIELD>_get( b_ptr, h_ptr );
 |     odu_ohp_core_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     odu_ohp_core_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = odu_ohp_core_field_<FIELD>_get( NULL, h_ptr );
 | odu_ohp_core_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------+
 | | Register 0x00000000 OHP_CFG_CTRL |
 | +----------------------------------+
 | | bit  8 R/W  STGB_GCC_EN          |
 | | bit  4 R/W  STGA_GCC_EN          |
 | | bit  0 R/W  OHP_EN               |
 | +----------------------------------+
 | 
 | void   odu_ohp_core_field_STGB_GCC_EN_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_STGB_GCC_EN_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_STGA_GCC_EN_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_STGA_GCC_EN_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_OHP_EN_set      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_OHP_EN_get      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | +----------------------------------+
 | | Register 0x00000004 SCH_RXFA_CFG |
 | +----------------------------------+
 | | bit  16 R/W  RXFA_SCH_FORCE      |
 | | bit  8  R/W  RXFA_SCH_REQ        |
 | | bit  0  R/W  RXFA_SCH_EN         |
 | +----------------------------------+
 | 
 | void   odu_ohp_core_field_RXFA_SCH_FORCE_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_RXFA_SCH_FORCE_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_RXFA_SCH_REQ_set   ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_RXFA_SCH_REQ_get   ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_RXFA_SCH_EN_set    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_RXFA_SCH_EN_get    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | +----------------------------------+
 | | Register 0x00000008 SCH_RXFB_CFG |
 | +----------------------------------+
 | | bit  16 R/W  RXFB_SCH_FORCE      |
 | | bit  8  R/W  RXFB_SCH_REQ        |
 | | bit  0  R/W  RXFB_SCH_EN         |
 | +----------------------------------+
 | 
 | void   odu_ohp_core_field_RXFB_SCH_FORCE_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_RXFB_SCH_FORCE_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_RXFB_SCH_REQ_set   ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_RXFB_SCH_REQ_get   ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_RXFB_SCH_EN_set    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_RXFB_SCH_EN_get    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x0000000c STGA_SA_UID |
 | +---------------------------------+
 | | bit  31:0 R/W  STGA_SADDR_UID   |
 | +---------------------------------+
 | 
 | void   odu_ohp_core_field_STGA_SADDR_UID_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_STGA_SADDR_UID_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_range_STGA_SADDR_UID_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_field_range_STGA_SADDR_UID_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000010 STGB_SA_UID |
 | +---------------------------------+
 | | bit  31:0 R/W  STGB_SADDR_UID   |
 | +---------------------------------+
 | 
 | void   odu_ohp_core_field_STGB_SADDR_UID_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_STGB_SADDR_UID_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_range_STGB_SADDR_UID_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_field_range_STGB_SADDR_UID_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000014 STGA_DA_UID |
 | +---------------------------------+
 | | bit  31:0 R/W  STGA_DADDR_UID   |
 | +---------------------------------+
 | 
 | void   odu_ohp_core_field_STGA_DADDR_UID_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_STGA_DADDR_UID_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_range_STGA_DADDR_UID_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_field_range_STGA_DADDR_UID_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000018 STGB_DA_UID |
 | +---------------------------------+
 | | bit  31:0 R/W  STGB_DADDR_UID   |
 | +---------------------------------+
 | 
 | void   odu_ohp_core_field_STGB_DADDR_UID_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_STGB_DADDR_UID_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_range_STGB_DADDR_UID_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_field_range_STGB_DADDR_UID_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x0000001c RXFA_PWR_CTRL |
 | +-----------------------------------+
 | | bit  5:0 R/W  RXFA_SD_CONFIG      |
 | +-----------------------------------+
 | 
 | void   odu_ohp_core_field_RXFA_SD_CONFIG_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_RXFA_SD_CONFIG_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_range_RXFA_SD_CONFIG_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_field_range_RXFA_SD_CONFIG_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000020 RXFB_PWR_CTRL |
 | +-----------------------------------+
 | | bit  5:0 R/W  RXFB_SD_CONFIG      |
 | +-----------------------------------+
 | 
 | void   odu_ohp_core_field_RXFB_SD_CONFIG_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_RXFB_SD_CONFIG_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_range_RXFB_SD_CONFIG_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_field_range_RXFB_SD_CONFIG_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000024 TXFA_PWR_CTRL |
 | +-----------------------------------+
 | | bit  5:0 R/W  TXFA_SD_CONFIG      |
 | +-----------------------------------+
 | 
 | void   odu_ohp_core_field_TXFA_SD_CONFIG_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_TXFA_SD_CONFIG_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_range_TXFA_SD_CONFIG_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_field_range_TXFA_SD_CONFIG_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000028 TXFB_PWR_CTRL |
 | +-----------------------------------+
 | | bit  5:0 R/W  TXFB_SD_CONFIG      |
 | +-----------------------------------+
 | 
 | void   odu_ohp_core_field_TXFB_SD_CONFIG_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_TXFB_SD_CONFIG_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_range_TXFB_SD_CONFIG_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_field_range_TXFB_SD_CONFIG_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x0000002c + (N) * 4) STGA_CH_EN |
 | |     N[0..95]                               |
 | +--------------------------------------------+
 | | bit  0 R/W  STGA_CH_EN_CTRL                |
 | +--------------------------------------------+
 | 
 | void   odu_ohp_core_lfield_STGA_CH_EN_CTRL_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[96] )
 | void   odu_ohp_core_lfield_STGA_CH_EN_CTRL_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[96] )
 | void   odu_ohp_core_lfield_range_STGA_CH_EN_CTRL_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_STGA_CH_EN_CTRL_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x000001ac + (N) * 4) STGB_CH_EN |
 | |     N[0..95]                               |
 | +--------------------------------------------+
 | | bit  0 R/W  STGB_CH_EN_CTRL                |
 | +--------------------------------------------+
 | 
 | void   odu_ohp_core_lfield_STGB_CH_EN_CTRL_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[96] )
 | void   odu_ohp_core_lfield_STGB_CH_EN_CTRL_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[96] )
 | void   odu_ohp_core_lfield_range_STGB_CH_EN_CTRL_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_STGB_CH_EN_CTRL_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register (0x0000032c + (N) * 4) TXFA_OFFSET |
 | |     N[0..95]                                |
 | +---------------------------------------------+
 | | bit  8:0 R/W  TXFA_INS_OFFSET               |
 | +---------------------------------------------+
 | 
 | void   odu_ohp_core_lfield_TXFA_INS_OFFSET_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[96] )
 | void   odu_ohp_core_lfield_TXFA_INS_OFFSET_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[96] )
 | void   odu_ohp_core_lfield_range_TXFA_INS_OFFSET_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_TXFA_INS_OFFSET_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register (0x000004ac + (N) * 4) TXFB_OFFSET |
 | |     N[0..95]                                |
 | +---------------------------------------------+
 | | bit  8:0 R/W  TXFB_INS_OFFSET               |
 | +---------------------------------------------+
 | 
 | void   odu_ohp_core_lfield_TXFB_INS_OFFSET_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[96] )
 | void   odu_ohp_core_lfield_TXFB_INS_OFFSET_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[96] )
 | void   odu_ohp_core_lfield_range_TXFB_INS_OFFSET_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_TXFB_INS_OFFSET_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x0000062c + (N) * 4) TXFA_GCC0_MASK |
 | |     N[0..2]                                    |
 | +------------------------------------------------+
 | | bit  31:0 R/W  TXFA_GCC0_INS_MASK              |
 | +------------------------------------------------+
 | 
 | void   odu_ohp_core_lfield_TXFA_GCC0_INS_MASK_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_TXFA_GCC0_INS_MASK_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_range_TXFA_GCC0_INS_MASK_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_TXFA_GCC0_INS_MASK_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00000638 + (N) * 4) TXFA_GCC1_MASK |
 | |     N[0..2]                                    |
 | +------------------------------------------------+
 | | bit  31:0 R/W  TXFA_GCC1_INS_MASK              |
 | +------------------------------------------------+
 | 
 | void   odu_ohp_core_lfield_TXFA_GCC1_INS_MASK_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_TXFA_GCC1_INS_MASK_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_range_TXFA_GCC1_INS_MASK_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_TXFA_GCC1_INS_MASK_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00000644 + (N) * 4) TXFA_GCC2_MASK |
 | |     N[0..2]                                    |
 | +------------------------------------------------+
 | | bit  31:0 R/W  TXFA_GCC2_INS_MASK              |
 | +------------------------------------------------+
 | 
 | void   odu_ohp_core_lfield_TXFA_GCC2_INS_MASK_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_TXFA_GCC2_INS_MASK_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_range_TXFA_GCC2_INS_MASK_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_TXFA_GCC2_INS_MASK_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00000650 + (N) * 4) TXFB_GCC0_MASK |
 | |     N[0..2]                                    |
 | +------------------------------------------------+
 | | bit  31:0 R/W  TXFB_GCC0_INS_MASK              |
 | +------------------------------------------------+
 | 
 | void   odu_ohp_core_lfield_TXFB_GCC0_INS_MASK_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_TXFB_GCC0_INS_MASK_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_range_TXFB_GCC0_INS_MASK_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_TXFB_GCC0_INS_MASK_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x0000065c + (N) * 4) TXFB_GCC1_MASK |
 | |     N[0..2]                                    |
 | +------------------------------------------------+
 | | bit  31:0 R/W  TXFB_GCC1_INS_MASK              |
 | +------------------------------------------------+
 | 
 | void   odu_ohp_core_lfield_TXFB_GCC1_INS_MASK_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_TXFB_GCC1_INS_MASK_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_range_TXFB_GCC1_INS_MASK_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_TXFB_GCC1_INS_MASK_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00000668 + (N) * 4) TXFB_GCC2_MASK |
 | |     N[0..2]                                    |
 | +------------------------------------------------+
 | | bit  31:0 R/W  TXFB_GCC2_INS_MASK              |
 | +------------------------------------------------+
 | 
 | void   odu_ohp_core_lfield_TXFB_GCC2_INS_MASK_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_TXFB_GCC2_INS_MASK_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_range_TXFB_GCC2_INS_MASK_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_TXFB_GCC2_INS_MASK_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000674 RXFA_CFC_FULL_INT_EN0 |
 | +-------------------------------------------+
 | | bit  31:0 R/W  RXFA_CFC_FULL_E            |
 | +-------------------------------------------+
 | 
 | void   odu_ohp_core_lfield_RXFA_CFC_FULL_E_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_RXFA_CFC_FULL_E_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_range_RXFA_CFC_FULL_E_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_RXFA_CFC_FULL_E_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000680 RXFA_CFC_FULL_INT0 |
 | +----------------------------------------+
 | | bit  31:0 R/W  RXFA_CFC_FULL_I         |
 | +----------------------------------------+
 | 
 | void                 odu_ohp_core_lfield_RXFA_CFC_FULL_I_set_to_clear       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_ohp_core_lfield_RXFA_CFC_FULL_I_get                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_set_to_clear ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_get          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_range_RXFA_CFC_FULL_I_poll         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_RXFA_CFC_FULL_I_poll               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x0000068c RXFB_CFC_FULL_INT_EN0 |
 | +-------------------------------------------+
 | | bit  31:0 R/W  RXFB_CFC_FULL_E            |
 | +-------------------------------------------+
 | 
 | void   odu_ohp_core_lfield_RXFB_CFC_FULL_E_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_RXFB_CFC_FULL_E_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_range_RXFB_CFC_FULL_E_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_RXFB_CFC_FULL_E_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000698 RXFB_CFC_FULL_INT0 |
 | +----------------------------------------+
 | | bit  31:0 R/W  RXFB_CFC_FULL_I         |
 | +----------------------------------------+
 | 
 | void                 odu_ohp_core_lfield_RXFB_CFC_FULL_I_set_to_clear       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_ohp_core_lfield_RXFB_CFC_FULL_I_get                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_set_to_clear ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_get          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_range_RXFB_CFC_FULL_I_poll         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_RXFB_CFC_FULL_I_poll               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x000006a4 RXFA_TXDLY_INT_EN0 |
 | +----------------------------------------+
 | | bit  31:0 R/W  RXFA_TXDLY_E            |
 | +----------------------------------------+
 | 
 | void   odu_ohp_core_lfield_RXFA_TXDLY_E_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_RXFA_TXDLY_E_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_range_RXFA_TXDLY_E_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_RXFA_TXDLY_E_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000006b0 RXFA_TXDLY_INT0 |
 | +-------------------------------------+
 | | bit  31:0 R/W  RXFA_TXDLY_I         |
 | +-------------------------------------+
 | 
 | void                 odu_ohp_core_lfield_RXFA_TXDLY_I_set_to_clear       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_ohp_core_lfield_RXFA_TXDLY_I_get                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_ohp_core_lfield_range_RXFA_TXDLY_I_set_to_clear ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               odu_ohp_core_lfield_range_RXFA_TXDLY_I_get          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_range_RXFA_TXDLY_I_poll         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_RXFA_TXDLY_I_poll               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x000006bc RXFB_TXDLY_INT_EN0 |
 | +----------------------------------------+
 | | bit  31:0 R/W  RXFB_TXDLY_E            |
 | +----------------------------------------+
 | 
 | void   odu_ohp_core_lfield_RXFB_TXDLY_E_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_RXFB_TXDLY_E_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_ohp_core_lfield_range_RXFB_TXDLY_E_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_RXFB_TXDLY_E_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000006c8 RXFB_TXDLY_INT0 |
 | +-------------------------------------+
 | | bit  31:0 R/W  RXFB_TXDLY_I         |
 | +-------------------------------------+
 | 
 | void                 odu_ohp_core_lfield_RXFB_TXDLY_I_set_to_clear       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_ohp_core_lfield_RXFB_TXDLY_I_get                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_ohp_core_lfield_range_RXFB_TXDLY_I_set_to_clear ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               odu_ohp_core_lfield_range_RXFB_TXDLY_I_get          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_range_RXFB_TXDLY_I_poll         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_lfield_RXFB_TXDLY_I_poll               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x000006d4 RXMAC_UID_FAIL_EN |
 | +---------------------------------------+
 | | bit  0 R/W  RXMAC_UID_FAIL_E          |
 | +---------------------------------------+
 | 
 | void   odu_ohp_core_field_RXMAC_UID_FAIL_E_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_RXMAC_UID_FAIL_E_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x000006d8 RXMAC_FRMRID_FAIL_EN |
 | +------------------------------------------+
 | | bit  0 R/W  RXMAC_FRMRID_FAIL_E          |
 | +------------------------------------------+
 | 
 | void   odu_ohp_core_field_RXMAC_FRMRID_FAIL_E_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_RXMAC_FRMRID_FAIL_E_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | +----------------------------------------+
 | | Register 0x000006dc RXMAC_CHID_FAIL_EN |
 | +----------------------------------------+
 | | bit  0 R/W  RXMAC_CHID_FAIL_E          |
 | +----------------------------------------+
 | 
 | void   odu_ohp_core_field_RXMAC_CHID_FAIL_E_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_RXMAC_CHID_FAIL_E_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x000006e0 RXMAC_PKTERR_INT_EN |
 | +-----------------------------------------+
 | | bit  0 R/W  RXMAC_PKTERR_E              |
 | +-----------------------------------------+
 | 
 | void   odu_ohp_core_field_RXMAC_PKTERR_E_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_RXMAC_PKTERR_E_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x000006e4 RXMAC_RXFIFO_ERR_EN |
 | +-----------------------------------------+
 | | bit  0 R/W  RXMAC_FIFO_ERR_E            |
 | +-----------------------------------------+
 | 
 | void   odu_ohp_core_field_RXMAC_FIFO_ERR_E_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_RXMAC_FIFO_ERR_E_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | +----------------------------------------+
 | | Register 0x000006e8 RXMAC_UID_FAIL_INT |
 | +----------------------------------------+
 | | bit  0 R/W  RXMAC_UID_FAIL_I           |
 | +----------------------------------------+
 | 
 | void                 odu_ohp_core_field_RXMAC_UID_FAIL_I_set_to_clear ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_ohp_core_field_RXMAC_UID_FAIL_I_get          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_UID_FAIL_I_poll         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x000006ec RXMAC_FRMRID_FAIL_INT |
 | +-------------------------------------------+
 | | bit  0 R/W  RXMAC_FRMRID_FAIL_I           |
 | +-------------------------------------------+
 | 
 | void                 odu_ohp_core_field_RXMAC_FRMRID_FAIL_I_set_to_clear ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_ohp_core_field_RXMAC_FRMRID_FAIL_I_get          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_FRMRID_FAIL_I_poll         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x000006f0 RXMAC_CHID_FAIL_INT |
 | +-----------------------------------------+
 | | bit  0 R/W  RXMAC_CHID_FAIL_I           |
 | +-----------------------------------------+
 | 
 | void                 odu_ohp_core_field_RXMAC_CHID_FAIL_I_set_to_clear ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_ohp_core_field_RXMAC_CHID_FAIL_I_get          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_CHID_FAIL_I_poll         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------+
 | | Register 0x000006f4 RXMAC_PKTERR_INT |
 | +--------------------------------------+
 | | bit  0 R/W  RXMAC_PKTERR_I           |
 | +--------------------------------------+
 | 
 | void                 odu_ohp_core_field_RXMAC_PKTERR_I_set_to_clear ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_ohp_core_field_RXMAC_PKTERR_I_get          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_PKTERR_I_poll         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x000006f8 RXMAC_RXFIFO_ERR_INT |
 | +------------------------------------------+
 | | bit  0 R/W  RXMAC_FIFO_ERR_I             |
 | +------------------------------------------+
 | 
 | void                 odu_ohp_core_field_RXMAC_FIFO_ERR_I_set_to_clear ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_ohp_core_field_RXMAC_FIFO_ERR_I_get          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_field_RXMAC_FIFO_ERR_I_poll         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------------+
 | | Register (0x00000800 + (N) * 4) RXFA_EXT_MASK |
 | |     N[0..95]                                  |
 | +-----------------------------------------------+
 | | bit  15:0 R/W  RXFA_EXT_MASK                  |
 | +-----------------------------------------------+
 | 
 | void   odu_ohp_core_field_RXFA_EXT_MASK_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ohp_core_field_RXFA_EXT_MASK_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N )
 | void   odu_ohp_core_field_range_RXFA_EXT_MASK_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_field_range_RXFA_EXT_MASK_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00000a00 + (N) * 4) RXFB_EXT_MASK |
 | |     N[0..95]                                  |
 | +-----------------------------------------------+
 | | bit  15:0 R/W  RXFB_EXT_MASK                  |
 | +-----------------------------------------------+
 | 
 | void   odu_ohp_core_field_RXFB_EXT_MASK_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ohp_core_field_RXFB_EXT_MASK_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N )
 | void   odu_ohp_core_field_range_RXFB_EXT_MASK_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_field_range_RXFB_EXT_MASK_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00000c00 + (N) * 4) TXFA_INS_MASK |
 | |     N[0..95]                                  |
 | +-----------------------------------------------+
 | | bit  15:0 R/W  TXFA_INS_MASK                  |
 | +-----------------------------------------------+
 | 
 | void   odu_ohp_core_field_TXFA_INS_MASK_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ohp_core_field_TXFA_INS_MASK_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N )
 | void   odu_ohp_core_field_range_TXFA_INS_MASK_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_field_range_TXFA_INS_MASK_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00000e00 + (N) * 4) TXFB_INS_MASK |
 | |     N[0..95]                                  |
 | +-----------------------------------------------+
 | | bit  15:0 R/W  TXFB_INS_MASK                  |
 | +-----------------------------------------------+
 | 
 | void   odu_ohp_core_field_TXFB_INS_MASK_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_ohp_core_field_TXFB_INS_MASK_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N )
 | void   odu_ohp_core_field_range_TXFB_INS_MASK_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_field_range_TXFB_INS_MASK_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00001000 + (N) * 4) RXFA_CH_SCHEDULE |
 | |     N[0..95]                                     |
 | +--------------------------------------------------+
 | | bit  7:0 R/W  SRV_CH_ID_A                        |
 | +--------------------------------------------------+
 | 
 | void   odu_ohp_core_lfield_SRV_CH_ID_A_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[96] )
 | void   odu_ohp_core_lfield_SRV_CH_ID_A_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[96] )
 | void   odu_ohp_core_lfield_range_SRV_CH_ID_A_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_SRV_CH_ID_A_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00001200 + (N) * 4) RXFB_CH_SCHEDULE |
 | |     N[0..95]                                     |
 | +--------------------------------------------------+
 | | bit  7:0 R/W  SRV_CH_ID_B                        |
 | +--------------------------------------------------+
 | 
 | void   odu_ohp_core_lfield_SRV_CH_ID_B_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[96] )
 | void   odu_ohp_core_lfield_SRV_CH_ID_B_get       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value[96] )
 | void   odu_ohp_core_lfield_range_SRV_CH_ID_B_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_ohp_core_lfield_range_SRV_CH_ID_B_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00001390 OHP_ENET_IO_CTRL |
 | +--------------------------------------+
 | | bit  24 R/W  FEGE_RX_FLIP            |
 | | bit  16 R/W  FEGE_TX_FLIP            |
 | | bit  8  R/W  SGMII_RX_FLIP           |
 | | bit  0  R/W  SGMII_TX_FLIP           |
 | +--------------------------------------+
 | 
 | void   odu_ohp_core_field_FEGE_RX_FLIP_set  ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_FEGE_RX_FLIP_get  ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_FEGE_TX_FLIP_set  ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_FEGE_TX_FLIP_get  ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_SGMII_RX_FLIP_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_SGMII_RX_FLIP_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void   odu_ohp_core_field_SGMII_TX_FLIP_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_ohp_core_field_SGMII_TX_FLIP_get ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ODU_OHP_CORE_IO_H
#define _ODU_OHP_CORE_IO_H

#include "odu_ohp_loc.h"
#include "odu_ohp_core_regs.h"
#include "odu_ohp_core_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 odu_ohp_core_buffer_init               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | void                 odu_ohp_core_buffer_flush              ( odu_ohp_core_buffer_t *b_ptr )
 | UINT32               odu_ohp_core_reg_read                  ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 odu_ohp_core_reg_write                 ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 odu_ohp_core_field_set                 ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 odu_ohp_core_action_on_write_field_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_ohp_core_burst_read                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 odu_ohp_core_burst_write               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_poll                      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 odu_ohp_core_reg_OHP_CFG_CTRL_write                              ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_OHP_CFG_CTRL_field_set                          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_OHP_CFG_CTRL_read                               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_STGA_SA_UID_write                               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_STGA_SA_UID_field_set                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_STGA_SA_UID_read                                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_STGB_SA_UID_write                               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_STGB_SA_UID_field_set                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_STGB_SA_UID_read                                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_STGA_DA_UID_write                               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_STGA_DA_UID_field_set                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_STGA_DA_UID_read                                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_STGB_DA_UID_write                               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_STGB_DA_UID_field_set                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_STGB_DA_UID_read                                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFA_PWR_CTRL_write                             ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_PWR_CTRL_field_set                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFA_PWR_CTRL_read                              ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFB_PWR_CTRL_write                             ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_PWR_CTRL_field_set                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFB_PWR_CTRL_read                              ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_TXFA_PWR_CTRL_write                             ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_TXFA_PWR_CTRL_field_set                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_TXFA_PWR_CTRL_read                              ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_TXFB_PWR_CTRL_write                             ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_TXFB_PWR_CTRL_field_set                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_TXFB_PWR_CTRL_read                              ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_OHP_ENET_IO_CTRL_write                          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_OHP_ENET_IO_CTRL_field_set                      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_OHP_ENET_IO_CTRL_read                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_TXFA_GCC0_MASK_array_burst_write                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_ohp_core_reg_TXFA_GCC0_MASK_array_field_set                  ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_ohp_core_reg_TXFA_GCC0_MASK_array_burst_read                 ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_ohp_core_reg_TXFA_GCC0_MASK_array_read                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_ohp_core_reg_TXFA_GCC1_MASK_array_burst_write                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_ohp_core_reg_TXFA_GCC1_MASK_array_field_set                  ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_ohp_core_reg_TXFA_GCC1_MASK_array_burst_read                 ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_ohp_core_reg_TXFA_GCC1_MASK_array_read                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_ohp_core_reg_TXFA_GCC2_MASK_array_burst_write                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_ohp_core_reg_TXFA_GCC2_MASK_array_field_set                  ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_ohp_core_reg_TXFA_GCC2_MASK_array_burst_read                 ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_ohp_core_reg_TXFA_GCC2_MASK_array_read                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_ohp_core_reg_TXFB_GCC0_MASK_array_burst_write                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_ohp_core_reg_TXFB_GCC0_MASK_array_field_set                  ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_ohp_core_reg_TXFB_GCC0_MASK_array_burst_read                 ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_ohp_core_reg_TXFB_GCC0_MASK_array_read                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_ohp_core_reg_TXFB_GCC1_MASK_array_burst_write                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_ohp_core_reg_TXFB_GCC1_MASK_array_field_set                  ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_ohp_core_reg_TXFB_GCC1_MASK_array_burst_read                 ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_ohp_core_reg_TXFB_GCC1_MASK_array_read                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_ohp_core_reg_TXFB_GCC2_MASK_array_burst_write                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_ohp_core_reg_TXFB_GCC2_MASK_array_field_set                  ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_ohp_core_reg_TXFB_GCC2_MASK_array_burst_read                 ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_ohp_core_reg_TXFB_GCC2_MASK_array_read                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_ohp_core_reg_STGA_CH_EN_array_burst_write                    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_ohp_core_reg_STGA_CH_EN_array_field_set                      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_ohp_core_reg_STGA_CH_EN_array_burst_read                     ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_ohp_core_reg_STGA_CH_EN_array_read                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_ohp_core_reg_STGB_CH_EN_array_burst_write                    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_ohp_core_reg_STGB_CH_EN_array_field_set                      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_ohp_core_reg_STGB_CH_EN_array_burst_read                     ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_ohp_core_reg_STGB_CH_EN_array_read                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_ohp_core_reg_TXFA_OFFSET_array_burst_write                   ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_ohp_core_reg_TXFA_OFFSET_array_field_set                     ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_ohp_core_reg_TXFA_OFFSET_array_burst_read                    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_ohp_core_reg_TXFA_OFFSET_array_read                          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_ohp_core_reg_TXFB_OFFSET_array_burst_write                   ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_ohp_core_reg_TXFB_OFFSET_array_field_set                     ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_ohp_core_reg_TXFB_OFFSET_array_burst_read                    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_ohp_core_reg_TXFB_OFFSET_array_read                          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_ohp_core_reg_SCH_RXFA_CFG_write                              ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_SCH_RXFA_CFG_field_set                          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_SCH_RXFA_CFG_read                               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_SCH_RXFB_CFG_write                              ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_SCH_RXFB_CFG_field_set                          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_SCH_RXFB_CFG_read                               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_write                     ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_field_set                 ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN0_read                      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_write                     ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_field_set                 ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN1_read                      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_write                     ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_field_set                 ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFA_CFC_FULL_INT_EN2_read                      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_write                     ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_field_set                 ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN0_read                      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_write                     ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_field_set                 ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN1_read                      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_write                     ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_field_set                 ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFB_CFC_FULL_INT_EN2_read                      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_write                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_field_set                    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFA_TXDLY_INT_EN0_read                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_write                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_field_set                    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFA_TXDLY_INT_EN1_read                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_write                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_field_set                    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFA_TXDLY_INT_EN2_read                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_write                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_field_set                    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFB_TXDLY_INT_EN0_read                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_write                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_field_set                    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFB_TXDLY_INT_EN1_read                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_write                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_field_set                    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFB_TXDLY_INT_EN2_read                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXMAC_UID_FAIL_EN_write                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXMAC_UID_FAIL_EN_field_set                     ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXMAC_UID_FAIL_EN_read                          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXMAC_FRMRID_FAIL_EN_write                      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXMAC_FRMRID_FAIL_EN_field_set                  ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXMAC_FRMRID_FAIL_EN_read                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXMAC_CHID_FAIL_EN_write                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXMAC_CHID_FAIL_EN_field_set                    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXMAC_CHID_FAIL_EN_read                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXMAC_PKTERR_INT_EN_write                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXMAC_PKTERR_INT_EN_field_set                   ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXMAC_PKTERR_INT_EN_read                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXMAC_RXFIFO_ERR_EN_write                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXMAC_RXFIFO_ERR_EN_field_set                   ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXMAC_RXFIFO_ERR_EN_read                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | 
 | void                 odu_ohp_core_reg_RXFA_CFC_FULL_INT0_write                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_CFC_FULL_INT0_action_on_write_field_set    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFA_CFC_FULL_INT0_read                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_CFC_FULL_INT0_poll                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXFA_CFC_FULL_INT1_write                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_CFC_FULL_INT1_action_on_write_field_set    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFA_CFC_FULL_INT1_read                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_CFC_FULL_INT1_poll                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXFA_CFC_FULL_INT2_write                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_CFC_FULL_INT2_action_on_write_field_set    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFA_CFC_FULL_INT2_read                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_CFC_FULL_INT2_poll                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXFB_CFC_FULL_INT0_write                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_CFC_FULL_INT0_action_on_write_field_set    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFB_CFC_FULL_INT0_read                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_CFC_FULL_INT0_poll                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXFB_CFC_FULL_INT1_write                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_CFC_FULL_INT1_action_on_write_field_set    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFB_CFC_FULL_INT1_read                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_CFC_FULL_INT1_poll                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXFB_CFC_FULL_INT2_write                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_CFC_FULL_INT2_action_on_write_field_set    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFB_CFC_FULL_INT2_read                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_CFC_FULL_INT2_poll                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXFA_TXDLY_INT0_write                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_TXDLY_INT0_action_on_write_field_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFA_TXDLY_INT0_read                            ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_TXDLY_INT0_poll                            ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXFA_TXDLY_INT1_write                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_TXDLY_INT1_action_on_write_field_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFA_TXDLY_INT1_read                            ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_TXDLY_INT1_poll                            ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXFA_TXDLY_INT2_write                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_TXDLY_INT2_action_on_write_field_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFA_TXDLY_INT2_read                            ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFA_TXDLY_INT2_poll                            ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXFB_TXDLY_INT0_write                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_TXDLY_INT0_action_on_write_field_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFB_TXDLY_INT0_read                            ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_TXDLY_INT0_poll                            ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXFB_TXDLY_INT1_write                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_TXDLY_INT1_action_on_write_field_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFB_TXDLY_INT1_read                            ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_TXDLY_INT1_poll                            ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXFB_TXDLY_INT2_write                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_TXDLY_INT2_action_on_write_field_set       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFB_TXDLY_INT2_read                            ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXFB_TXDLY_INT2_poll                            ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXMAC_UID_FAIL_INT_write                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXMAC_UID_FAIL_INT_action_on_write_field_set    ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXMAC_UID_FAIL_INT_read                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_UID_FAIL_INT_poll                         ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_write                     ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_action_on_write_field_set ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_read                      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_FRMRID_FAIL_INT_poll                      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_write                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_action_on_write_field_set   ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_read                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_CHID_FAIL_INT_poll                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXMAC_PKTERR_INT_write                          ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXMAC_PKTERR_INT_action_on_write_field_set      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXMAC_PKTERR_INT_read                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_PKTERR_INT_poll                           ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_write                      ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 value )
 | void                 odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_action_on_write_field_set  ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_read                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_ohp_core_reg_RXMAC_RXFIFO_ERR_INT_poll                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_burst_write              ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_field_set                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_burst_read               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_ohp_core_reg_RXFA_CH_SCHEDULE_array_read                     ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_burst_write              ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_field_set                ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_burst_read               ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_ohp_core_reg_RXFB_CH_SCHEDULE_array_read                     ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_ohp_core_reg_RXFA_EXT_MASK_array_write                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ohp_core_reg_RXFA_EXT_MASK_array_field_set                   ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFA_EXT_MASK_array_read                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ohp_core_reg_RXFB_EXT_MASK_array_write                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ohp_core_reg_RXFB_EXT_MASK_array_field_set                   ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_RXFB_EXT_MASK_array_read                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ohp_core_reg_TXFA_INS_MASK_array_write                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ohp_core_reg_TXFA_INS_MASK_array_field_set                   ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_TXFA_INS_MASK_array_read                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_ohp_core_reg_TXFB_INS_MASK_array_write                       ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_ohp_core_reg_TXFB_INS_MASK_array_field_set                   ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_ohp_core_reg_TXFB_INS_MASK_array_read                        ( odu_ohp_core_buffer_t *b_ptr, odu_ohp_handle_t *h_ptr, UINT32  N )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ODU_OHP_CORE_IO_H */
