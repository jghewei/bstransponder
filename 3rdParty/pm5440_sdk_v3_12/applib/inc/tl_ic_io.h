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
 *     and register accessor functions for the tl_ic block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing tl_ic io functions is:
 | {
 |     tl_ic_buffer_t b_ptr[1];
 |     tl_ic_buffer_init( b_ptr, h_ptr );
 |     value = tl_ic_field_<FIELD>_get( b_ptr, h_ptr );
 |     tl_ic_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     tl_ic_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = tl_ic_field_<FIELD>_get( NULL, h_ptr );
 | tl_ic_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------+
 | | Register 0x00000000 LP_CTRL       |
 | +-----------------------------------+
 | | bit  13 R/W  PGMRCLK_LOW_PWR      |
 | | bit  12 R/W  LINE_DSIS_LOW_PWR    |
 | | bit  11 R/W  COREOTN_LOW_PWR      |
 | | bit  10 R/W  SYSOTN_LOW_PWR       |
 | | bit  9  R/W  SIFD_LOW_PWR         |
 | | bit  8  R/W  MAPOTN_LOW_PWR       |
 | | bit  7  R/W  ENET120_LINE_LOW_PWR |
 | | bit  6  R/W  ENET120_SYS_LOW_PWR  |
 | | bit  5  R/W  MCPB_LOW_PWR         |
 | | bit  4  R/W  DCPB_LOW_PWR         |
 | | bit  3  R/W  CBRC_LOW_PWR         |
 | | bit  2  R/W  LIFD_MUX_LOW_PWR     |
 | | bit  1  R/W  LINEOTN_LOW_PWR      |
 | | bit  0  R/W  DIGI_M1_LOW_PWR      |
 | +-----------------------------------+
 | 
 | void   tl_ic_field_PGMRCLK_LOW_PWR_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PGMRCLK_LOW_PWR_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_LINE_DSIS_LOW_PWR_set    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LINE_DSIS_LOW_PWR_get    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_COREOTN_LOW_PWR_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_COREOTN_LOW_PWR_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SYSOTN_LOW_PWR_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SYSOTN_LOW_PWR_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SIFD_LOW_PWR_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SIFD_LOW_PWR_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_MAPOTN_LOW_PWR_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_MAPOTN_LOW_PWR_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET120_LINE_LOW_PWR_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET120_LINE_LOW_PWR_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET120_SYS_LOW_PWR_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET120_SYS_LOW_PWR_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_MCPB_LOW_PWR_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_MCPB_LOW_PWR_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_DCPB_LOW_PWR_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_DCPB_LOW_PWR_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_CBRC_LOW_PWR_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_CBRC_LOW_PWR_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_LIFD_MUX_LOW_PWR_set     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LIFD_MUX_LOW_PWR_get     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_LINEOTN_LOW_PWR_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LINEOTN_LOW_PWR_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_DIGI_M1_LOW_PWR_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_DIGI_M1_LOW_PWR_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000004 RAM_LP_CTRL       |
 | +---------------------------------------+
 | | bit  11 R/W  COREOTN_RAM_LOW_PWR      |
 | | bit  10 R/W  SYSOTN_RAM_LOW_PWR       |
 | | bit  9  R/W  SIFD_RAM_LOW_PWR         |
 | | bit  8  R/W  MAPOTN_RAM_LOW_PWR       |
 | | bit  7  R/W  ENET120_LINE_RAM_LOW_PWR |
 | | bit  6  R/W  ENET120_SYS_RAM_LOW_PWR  |
 | | bit  5  R/W  MCPB_RAM_LOW_PWR         |
 | | bit  4  R/W  DCPB_RAM_LOW_PWR         |
 | | bit  3  R/W  CBRC_RAM_LOW_PWR         |
 | | bit  1  R/W  LINEOTN_RAM_LOW_PWR      |
 | | bit  0  R/W  DIGI_M1_RAM_LOW_PWR      |
 | +---------------------------------------+
 | 
 | void   tl_ic_field_COREOTN_RAM_LOW_PWR_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_COREOTN_RAM_LOW_PWR_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SYSOTN_RAM_LOW_PWR_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SYSOTN_RAM_LOW_PWR_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SIFD_RAM_LOW_PWR_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SIFD_RAM_LOW_PWR_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_MAPOTN_RAM_LOW_PWR_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_MAPOTN_RAM_LOW_PWR_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET120_LINE_RAM_LOW_PWR_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET120_LINE_RAM_LOW_PWR_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET120_SYS_RAM_LOW_PWR_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET120_SYS_RAM_LOW_PWR_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_MCPB_RAM_LOW_PWR_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_MCPB_RAM_LOW_PWR_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_DCPB_RAM_LOW_PWR_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_DCPB_RAM_LOW_PWR_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_CBRC_RAM_LOW_PWR_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_CBRC_RAM_LOW_PWR_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_LINEOTN_RAM_LOW_PWR_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LINEOTN_RAM_LOW_PWR_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_DIGI_M1_RAM_LOW_PWR_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_DIGI_M1_RAM_LOW_PWR_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000010 LINE_SERDES_IDDQ |
 | +--------------------------------------+
 | | bit  13   R/W  SFI2_IDDQ             |
 | | bit  12   R/W  SFI1_IDDQ             |
 | | bit  11:0 R/W  LINE_SERDES_IDDQ      |
 | +--------------------------------------+
 | 
 | void   tl_ic_field_SFI2_IDDQ_set              ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI2_IDDQ_get              ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SFI1_IDDQ_set              ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI1_IDDQ_get              ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_LINE_SERDES_IDDQ_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LINE_SERDES_IDDQ_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_LINE_SERDES_IDDQ_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_LINE_SERDES_IDDQ_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000014 SYS_SERDES_IDDQ_1 |
 | +---------------------------------------+
 | | bit  21:20 R/W  LR_CSU_IDDQ           |
 | | bit  19:0  R/W  LR_SERDES_IDDQ        |
 | +---------------------------------------+
 | 
 | void   tl_ic_field_LR_CSU_IDDQ_set          ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LR_CSU_IDDQ_get          ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_LR_CSU_IDDQ_set    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_LR_CSU_IDDQ_get    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_LR_SERDES_IDDQ_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LR_SERDES_IDDQ_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_LR_SERDES_IDDQ_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_LR_SERDES_IDDQ_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000018 SYS_SERDES_IDDQ_2 |
 | +---------------------------------------+
 | | bit  24    R/W  SR_CSU_IDDQ           |
 | | bit  23:16 R/W  SR_SERDES_IDDQ        |
 | | bit  15:0  R/W  SYS_SERDES_IDDQ       |
 | +---------------------------------------+
 | 
 | void   tl_ic_field_SR_CSU_IDDQ_set           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SR_CSU_IDDQ_get           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SR_SERDES_IDDQ_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SR_SERDES_IDDQ_get        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_SR_SERDES_IDDQ_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_SR_SERDES_IDDQ_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_SYS_SERDES_IDDQ_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SYS_SERDES_IDDQ_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_SYS_SERDES_IDDQ_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_SYS_SERDES_IDDQ_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0000001c PCIE_OH_SERDES_IDDQ |
 | +-----------------------------------------+
 | | bit  15    R/W  OH_CSU_IDDQ             |
 | | bit  14:12 R/W  OH_TX_SERDES_IDDQ       |
 | | bit  10:8  R/W  OH_RX_SERDES_IDDQ       |
 | | bit  4     R/W  PCIE_CSU_IDDQ           |
 | | bit  3:0   R/W  PCIE_SERDES_IDDQ        |
 | +-----------------------------------------+
 | 
 | void   tl_ic_field_OH_CSU_IDDQ_set             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OH_CSU_IDDQ_get             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OH_TX_SERDES_IDDQ_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OH_TX_SERDES_IDDQ_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_OH_TX_SERDES_IDDQ_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_OH_TX_SERDES_IDDQ_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_OH_RX_SERDES_IDDQ_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OH_RX_SERDES_IDDQ_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_OH_RX_SERDES_IDDQ_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_OH_RX_SERDES_IDDQ_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_PCIE_CSU_IDDQ_set           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PCIE_CSU_IDDQ_get           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_PCIE_SERDES_IDDQ_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PCIE_SERDES_IDDQ_get        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_PCIE_SERDES_IDDQ_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_PCIE_SERDES_IDDQ_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x00000020 DCSU_IDDQ |
 | +-------------------------------+
 | | bit  8 R/W  FIC2_DCSU_IDDQ    |
 | | bit  7 R/W  ENET3_DCSU_IDDQ   |
 | | bit  6 R/W  PTP_DCSU_IDDQ     |
 | | bit  5 R/W  ENET2_DCSU_IDDQ   |
 | | bit  4 R/W  ENET1_DCSU_IDDQ   |
 | | bit  1 R/W  OTN2_DCSU_IDDQ    |
 | | bit  0 R/W  OTN1_DCSU_IDDQ    |
 | +-------------------------------+
 | 
 | void   tl_ic_field_FIC2_DCSU_IDDQ_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_FIC2_DCSU_IDDQ_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET3_DCSU_IDDQ_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET3_DCSU_IDDQ_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_PTP_DCSU_IDDQ_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PTP_DCSU_IDDQ_get   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET2_DCSU_IDDQ_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET2_DCSU_IDDQ_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET1_DCSU_IDDQ_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET1_DCSU_IDDQ_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OTN2_DCSU_IDDQ_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OTN2_DCSU_IDDQ_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OTN1_DCSU_IDDQ_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OTN1_DCSU_IDDQ_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00000030 RESET_CTRL0 |
 | +---------------------------------+
 | | bit  26 R/W  LINE_DSIS_RESET    |
 | | bit  25 R/W  PGMRCLK_RESET      |
 | | bit  24 R/W  PTP_DCSU_RESET     |
 | | bit  23 R/W  ENET2_DCSU_RESET   |
 | | bit  22 R/W  ENET1_DCSU_RESET   |
 | | bit  19 R/W  FIC2_DCSU_RESET    |
 | | bit  18 R/W  ENET3_DCSU_RESET   |
 | | bit  17 R/W  OTN2_DCSU_RESET    |
 | | bit  16 R/W  OTN1_DCSU_RESET    |
 | | bit  11 R/W  COREOTN_RESET      |
 | | bit  10 R/W  SYSOTN_RESET       |
 | | bit  9  R/W  SIFD_RESET         |
 | | bit  8  R/W  MAPOTN_RESET       |
 | | bit  7  R/W  ENET120_LINE_RESET |
 | | bit  6  R/W  ENET120_SYS_RESET  |
 | | bit  5  R/W  MCPB_RESET         |
 | | bit  4  R/W  DCPB_RESET         |
 | | bit  3  R/W  CBRC_RESET         |
 | | bit  2  R/W  LIFD_MUX_RESET     |
 | | bit  1  R/W  LINEOTN_RESET      |
 | | bit  0  R/W  DIGI_M1_RESET      |
 | +---------------------------------+
 | 
 | void   tl_ic_field_LINE_DSIS_RESET_set    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LINE_DSIS_RESET_get    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_PGMRCLK_RESET_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PGMRCLK_RESET_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_PTP_DCSU_RESET_set     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PTP_DCSU_RESET_get     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET2_DCSU_RESET_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET2_DCSU_RESET_get   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET1_DCSU_RESET_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET1_DCSU_RESET_get   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_FIC2_DCSU_RESET_set    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_FIC2_DCSU_RESET_get    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET3_DCSU_RESET_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET3_DCSU_RESET_get   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OTN2_DCSU_RESET_set    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OTN2_DCSU_RESET_get    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OTN1_DCSU_RESET_set    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OTN1_DCSU_RESET_get    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_COREOTN_RESET_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_COREOTN_RESET_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SYSOTN_RESET_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SYSOTN_RESET_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SIFD_RESET_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SIFD_RESET_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_MAPOTN_RESET_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_MAPOTN_RESET_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET120_LINE_RESET_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET120_LINE_RESET_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET120_SYS_RESET_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET120_SYS_RESET_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_MCPB_RESET_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_MCPB_RESET_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_DCPB_RESET_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_DCPB_RESET_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_CBRC_RESET_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_CBRC_RESET_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_LIFD_MUX_RESET_set     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LIFD_MUX_RESET_get     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_LINEOTN_RESET_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LINEOTN_RESET_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_DIGI_M1_RESET_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_DIGI_M1_RESET_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00000034 RESET_CTRL1 |
 | +---------------------------------+
 | | bit  24    R/W  C8_CSU_RESET    |
 | | bit  23:16 R/W  C8_RESET        |
 | | bit  15:0  R/W  S16_SYS_RESET   |
 | +---------------------------------+
 | 
 | void   tl_ic_field_C8_CSU_RESET_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_C8_CSU_RESET_get        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_C8_RESET_set            ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_C8_RESET_get            ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_C8_RESET_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_C8_RESET_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_S16_SYS_RESET_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_S16_SYS_RESET_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_S16_SYS_RESET_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_S16_SYS_RESET_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000038 RESET_CTRL2 |
 | +---------------------------------+
 | | bit  21:20 R/W  TITAN_CSU_RESET |
 | | bit  19:0  R/W  TITAN_RESET     |
 | +---------------------------------+
 | 
 | void   tl_ic_field_TITAN_CSU_RESET_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_TITAN_CSU_RESET_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_TITAN_CSU_RESET_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_TITAN_CSU_RESET_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_TITAN_RESET_set           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_TITAN_RESET_get           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_TITAN_RESET_set     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_TITAN_RESET_get     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x0000003c RESET_CTRL3    |
 | +------------------------------------+
 | | bit  31    R/W  D8_CSU_RESET       |
 | | bit  30:28 R/W  OH_SERDES_TX_RESET |
 | | bit  27:25 R/W  OH_SERDES_RX_RESET |
 | | bit  24    R/W  C8_PCIE_CSU_RESET  |
 | | bit  23:20 R/W  C8_PCIE_RESET      |
 | | bit  19:18 R/W  SFI51_TX_RESET     |
 | | bit  17:16 R/W  SFI51_RX_RESET     |
 | | bit  11:0  R/W  S16_LINE_RESET     |
 | +------------------------------------+
 | 
 | void   tl_ic_field_D8_CSU_RESET_set             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_D8_CSU_RESET_get             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OH_SERDES_TX_RESET_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OH_SERDES_TX_RESET_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_OH_SERDES_TX_RESET_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_OH_SERDES_TX_RESET_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_OH_SERDES_RX_RESET_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OH_SERDES_RX_RESET_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_OH_SERDES_RX_RESET_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_OH_SERDES_RX_RESET_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_C8_PCIE_CSU_RESET_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_C8_PCIE_CSU_RESET_get        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_C8_PCIE_RESET_set            ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_C8_PCIE_RESET_get            ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_C8_PCIE_RESET_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_C8_PCIE_RESET_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_SFI51_TX_RESET_set           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI51_TX_RESET_get           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_SFI51_TX_RESET_set     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_SFI51_TX_RESET_get     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_SFI51_RX_RESET_set           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI51_RX_RESET_get           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_SFI51_RX_RESET_set     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_SFI51_RX_RESET_get     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_S16_LINE_RESET_set           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_S16_LINE_RESET_get           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_S16_LINE_RESET_set     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_S16_LINE_RESET_get     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x00000070 IRIG_CTRL |
 | +-------------------------------+
 | | bit  3 R/W  IRIG_ONE_PPS_O_EN |
 | | bit  2 R/W  IRIG_DATO_EN      |
 | | bit  1 R/W  IRIG_CLK0_EN      |
 | | bit  0 R/W  IRIG_ENET_SOURCE  |
 | +-------------------------------+
 | 
 | void   tl_ic_field_IRIG_ONE_PPS_O_EN_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_IRIG_ONE_PPS_O_EN_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_IRIG_DATO_EN_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_IRIG_DATO_EN_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_IRIG_CLK0_EN_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_IRIG_CLK0_EN_get      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_IRIG_ENET_SOURCE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_IRIG_ENET_SOURCE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00000080 LINE_SERDES_INTR_SUM |
 | +------------------------------------------+
 | | bit  15   R  SFI2_TX_INTR                |
 | | bit  14   R  SFI2_RX_INTR                |
 | | bit  13   R  SFI1_TX_INTR                |
 | | bit  12   R  SFI1_RX_INTR                |
 | | bit  11:0 R  LINE_SERDES_INTR            |
 | +------------------------------------------+
 | 
 | UINT32 tl_ic_field_SFI2_TX_INTR_get           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_SFI2_RX_INTR_get           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_SFI1_TX_INTR_get           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_SFI1_RX_INTR_get           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_LINE_SERDES_INTR_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_range_LINE_SERDES_INTR_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000084 SYS_SERDES_INTR_SUM_1 |
 | +-------------------------------------------+
 | | bit  21:20 R  LR_CSU_INTR                 |
 | | bit  19:0  R  LR_SERDES_INTR              |
 | +-------------------------------------------+
 | 
 | UINT32 tl_ic_field_LR_CSU_INTR_get          ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_range_LR_CSU_INTR_get    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 tl_ic_field_LR_SERDES_INTR_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_range_LR_SERDES_INTR_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000088 SYS_SERDES_INTR_SUM_2 |
 | +-------------------------------------------+
 | | bit  24    R  SR_CSU_INTR                 |
 | | bit  23:16 R  SR_SERDES_INTR              |
 | | bit  15:0  R  SYS_SERDES_INTR             |
 | +-------------------------------------------+
 | 
 | UINT32 tl_ic_field_SR_CSU_INTR_get           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_SR_SERDES_INTR_get        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_range_SR_SERDES_INTR_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 tl_ic_field_SYS_SERDES_INTR_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_range_SYS_SERDES_INTR_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x0000008c PCIE_OH_SERDES_INTR_SUM |
 | +---------------------------------------------+
 | | bit  16    R  MGMT_FEGE_INTR                |
 | | bit  15    R  OH_CSU_INTR                   |
 | | bit  14:12 R  OH_TX_SERDES_INTR             |
 | | bit  10:8  R  OH_RX_SERDES_INTR             |
 | | bit  4     R  PCIE_CSU_INTR                 |
 | | bit  3:0   R  PCIE_SERDES_INTR              |
 | +---------------------------------------------+
 | 
 | UINT32 tl_ic_field_MGMT_FEGE_INTR_get          ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_OH_CSU_INTR_get             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_OH_TX_SERDES_INTR_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_range_OH_TX_SERDES_INTR_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 tl_ic_field_OH_RX_SERDES_INTR_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_range_OH_RX_SERDES_INTR_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 tl_ic_field_PCIE_CSU_INTR_get           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_PCIE_SERDES_INTR_get        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_range_PCIE_SERDES_INTR_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000090 DCSU_INTR_SUM |
 | +-----------------------------------+
 | | bit  8 R  FIC2_DCSU_INTR          |
 | | bit  7 R  FIC1_DCSU_INTR          |
 | | bit  6 R  PTP_DCSU_INTR           |
 | | bit  5 R  ENET2_DCSU_INTR         |
 | | bit  4 R  ENET1_DCSU_INTR         |
 | | bit  3 R  SYS2_DCSU_INTR          |
 | | bit  2 R  SYS_DCSU_INTR           |
 | | bit  1 R  OTN2_DCSU_INTR          |
 | | bit  0 R  OTN1_DCSU_INTR          |
 | +-----------------------------------+
 | 
 | UINT32 tl_ic_field_FIC2_DCSU_INTR_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_FIC1_DCSU_INTR_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_PTP_DCSU_INTR_get   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_ENET2_DCSU_INTR_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_ENET1_DCSU_INTR_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_SYS2_DCSU_INTR_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_SYS_DCSU_INTR_get   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_OTN2_DCSU_INTR_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 tl_ic_field_OTN1_DCSU_INTR_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00000100 LINE_SERDES_INTR_EN_1 |
 | +-------------------------------------------+
 | | bit  15 R/W  SFI2_TX_INTR_EN_MIPS         |
 | | bit  14 R/W  SFI2_RX_INTR_EN_MIPS         |
 | | bit  13 R/W  SFI1_TX_INTR_EN_MIPS         |
 | | bit  12 R/W  SFI1_RX_INTR_EN_MIPS         |
 | +-------------------------------------------+
 | 
 | void   tl_ic_field_SFI2_TX_INTR_EN_MIPS_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI2_TX_INTR_EN_MIPS_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SFI2_RX_INTR_EN_MIPS_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI2_RX_INTR_EN_MIPS_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SFI1_TX_INTR_EN_MIPS_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI1_TX_INTR_EN_MIPS_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SFI1_RX_INTR_EN_MIPS_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI1_RX_INTR_EN_MIPS_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00000108 SYS_SERDES_INTR_EN_1B |
 | +-------------------------------------------+
 | | bit  24 R/W  SR_CSU_INTR_EN_MIPS          |
 | +-------------------------------------------+
 | 
 | void   tl_ic_field_SR_CSU_INTR_EN_MIPS_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SR_CSU_INTR_EN_MIPS_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x0000010c OH_SERDES_INTR_EN_1 |
 | +-----------------------------------------+
 | | bit  16 R/W  MGMT_FEGE_INTR_EN_MIPS     |
 | | bit  15 R/W  OH_CSU_INTR_EN_MIPS        |
 | | bit  4  R/W  PCIE_CSU_INTR_EN_MIPS      |
 | +-----------------------------------------+
 | 
 | void   tl_ic_field_MGMT_FEGE_INTR_EN_MIPS_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_MGMT_FEGE_INTR_EN_MIPS_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OH_CSU_INTR_EN_MIPS_set    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OH_CSU_INTR_EN_MIPS_get    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_PCIE_CSU_INTR_EN_MIPS_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PCIE_CSU_INTR_EN_MIPS_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x00000110 DCSU_INTR_EN_1  |
 | +-------------------------------------+
 | | bit  8 R/W  FIC2_DCSU_INTR_EN_MIPS  |
 | | bit  7 R/W  FIC1_DCSU_INTR_EN_MIPS  |
 | | bit  6 R/W  PTP_DCSU_INTR_EN_MIPS   |
 | | bit  5 R/W  ENET2_DCSU_INTR_EN_MIPS |
 | | bit  4 R/W  ENET1_DCSU_INTR_EN_MIPS |
 | | bit  3 R/W  SYS2_DCSU_INTR_EN_MIPS  |
 | | bit  2 R/W  SYS_DCSU_INTR_EN_MIPS   |
 | | bit  1 R/W  OTN2_DCSU_INTR_EN_MIPS  |
 | | bit  0 R/W  OTN1_DCSU_INTR_EN_MIPS  |
 | +-------------------------------------+
 | 
 | void   tl_ic_field_FIC2_DCSU_INTR_EN_MIPS_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_FIC2_DCSU_INTR_EN_MIPS_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_FIC1_DCSU_INTR_EN_MIPS_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_FIC1_DCSU_INTR_EN_MIPS_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_PTP_DCSU_INTR_EN_MIPS_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PTP_DCSU_INTR_EN_MIPS_get   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET2_DCSU_INTR_EN_MIPS_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET2_DCSU_INTR_EN_MIPS_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET1_DCSU_INTR_EN_MIPS_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET1_DCSU_INTR_EN_MIPS_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SYS2_DCSU_INTR_EN_MIPS_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SYS2_DCSU_INTR_EN_MIPS_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SYS_DCSU_INTR_EN_MIPS_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SYS_DCSU_INTR_EN_MIPS_get   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OTN2_DCSU_INTR_EN_MIPS_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OTN2_DCSU_INTR_EN_MIPS_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OTN1_DCSU_INTR_EN_MIPS_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OTN1_DCSU_INTR_EN_MIPS_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00000140 LINE_SERDES_INTR_EN_2 |
 | +-------------------------------------------+
 | | bit  15   R/W  SFI2_TX_INTR_EN_PCIE       |
 | | bit  14   R/W  SFI2_RX_INTR_EN_PCIE       |
 | | bit  13   R/W  SFI1_TX_INTR_EN_PCIE       |
 | | bit  12   R/W  SFI1_RX_INTR_EN_PCIE       |
 | | bit  11:0 R/W  LINE_SERDES_INTR_EN_DEVICE |
 | +-------------------------------------------+
 | 
 | void   tl_ic_field_SFI2_TX_INTR_EN_PCIE_set             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI2_TX_INTR_EN_PCIE_get             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SFI2_RX_INTR_EN_PCIE_set             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI2_RX_INTR_EN_PCIE_get             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SFI1_TX_INTR_EN_PCIE_set             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI1_TX_INTR_EN_PCIE_get             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SFI1_RX_INTR_EN_PCIE_set             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI1_RX_INTR_EN_PCIE_get             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_LINE_SERDES_INTR_EN_DEVICE_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LINE_SERDES_INTR_EN_DEVICE_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_LINE_SERDES_INTR_EN_DEVICE_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_LINE_SERDES_INTR_EN_DEVICE_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000144 SYS_SERDES_INTR_EN_2A |
 | +-------------------------------------------+
 | | bit  21:20 R/W  LR_CSU_INTR_EN_DEVICE     |
 | | bit  19:0  R/W  LR_SERDES_INTR_EN_DEVICE  |
 | +-------------------------------------------+
 | 
 | void   tl_ic_field_LR_CSU_INTR_EN_DEVICE_set          ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LR_CSU_INTR_EN_DEVICE_get          ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_LR_CSU_INTR_EN_DEVICE_set    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_LR_CSU_INTR_EN_DEVICE_get    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_LR_SERDES_INTR_EN_DEVICE_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LR_SERDES_INTR_EN_DEVICE_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_LR_SERDES_INTR_EN_DEVICE_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_LR_SERDES_INTR_EN_DEVICE_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000148 SYS_SERDES_INTR_EN_2B |
 | +-------------------------------------------+
 | | bit  24    R/W  SR_CSU_INTR_EN_PCIE       |
 | | bit  23:16 R/W  SR_SERDES_INTR_EN_DEVICE  |
 | | bit  15:0  R/W  SYS_SERDES_INTR_EN_DEVICE |
 | +-------------------------------------------+
 | 
 | void   tl_ic_field_SR_CSU_INTR_EN_PCIE_set             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SR_CSU_INTR_EN_PCIE_get             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SR_SERDES_INTR_EN_DEVICE_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SR_SERDES_INTR_EN_DEVICE_get        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_SR_SERDES_INTR_EN_DEVICE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_SR_SERDES_INTR_EN_DEVICE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_SYS_SERDES_INTR_EN_DEVICE_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SYS_SERDES_INTR_EN_DEVICE_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_SYS_SERDES_INTR_EN_DEVICE_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_SYS_SERDES_INTR_EN_DEVICE_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x0000014c OH_SERDES_INTR_EN_2     |
 | +---------------------------------------------+
 | | bit  16    R/W  MGMT_FEGE_INTR_EN_PCIE      |
 | | bit  15    R/W  OH_CSU_INTR_EN_PCIE         |
 | | bit  14:12 R/W  OH_TX_SERDES_INTR_EN_DEVICE |
 | | bit  10:8  R/W  OH_RX_SERDES_INTR_EN_DEVICE |
 | | bit  4     R/W  PCIE_CSU_INTR_EN_PCIE       |
 | | bit  3:0   R/W  PCIE_SERDES_INTR_EN_DEVICE  |
 | +---------------------------------------------+
 | 
 | void   tl_ic_field_MGMT_FEGE_INTR_EN_PCIE_set            ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_MGMT_FEGE_INTR_EN_PCIE_get            ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OH_CSU_INTR_EN_PCIE_set               ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OH_CSU_INTR_EN_PCIE_get               ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OH_TX_SERDES_INTR_EN_DEVICE_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OH_TX_SERDES_INTR_EN_DEVICE_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_OH_TX_SERDES_INTR_EN_DEVICE_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_OH_TX_SERDES_INTR_EN_DEVICE_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_OH_RX_SERDES_INTR_EN_DEVICE_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OH_RX_SERDES_INTR_EN_DEVICE_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_OH_RX_SERDES_INTR_EN_DEVICE_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_OH_RX_SERDES_INTR_EN_DEVICE_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_PCIE_CSU_INTR_EN_PCIE_set             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PCIE_CSU_INTR_EN_PCIE_get             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_PCIE_SERDES_INTR_EN_DEVICE_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PCIE_SERDES_INTR_EN_DEVICE_get        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_PCIE_SERDES_INTR_EN_DEVICE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_PCIE_SERDES_INTR_EN_DEVICE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000150 DCSU_INTR_EN_2  |
 | +-------------------------------------+
 | | bit  8 R/W  FIC2_DCSU_INTR_EN_PCIE  |
 | | bit  7 R/W  FIC1_DCSU_INTR_EN_PCIE  |
 | | bit  6 R/W  PTP_DCSU_INTR_EN_PCIE   |
 | | bit  5 R/W  ENET2_DCSU_INTR_EN_PCIE |
 | | bit  4 R/W  ENET1_DCSU_INTR_EN_PCIE |
 | | bit  3 R/W  SYS2_DCSU_INTR_EN_PCIE  |
 | | bit  2 R/W  SYS_DCSU_INTR_EN_PCIE   |
 | | bit  1 R/W  OTN2_DCSU_INTR_EN_PCIE  |
 | | bit  0 R/W  OTN1_DCSU_INTR_EN_PCIE  |
 | +-------------------------------------+
 | 
 | void   tl_ic_field_FIC2_DCSU_INTR_EN_PCIE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_FIC2_DCSU_INTR_EN_PCIE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_FIC1_DCSU_INTR_EN_PCIE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_FIC1_DCSU_INTR_EN_PCIE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_PTP_DCSU_INTR_EN_PCIE_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PTP_DCSU_INTR_EN_PCIE_get   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET2_DCSU_INTR_EN_PCIE_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET2_DCSU_INTR_EN_PCIE_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET1_DCSU_INTR_EN_PCIE_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET1_DCSU_INTR_EN_PCIE_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SYS2_DCSU_INTR_EN_PCIE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SYS2_DCSU_INTR_EN_PCIE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SYS_DCSU_INTR_EN_PCIE_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SYS_DCSU_INTR_EN_PCIE_get   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OTN2_DCSU_INTR_EN_PCIE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OTN2_DCSU_INTR_EN_PCIE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OTN1_DCSU_INTR_EN_PCIE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OTN1_DCSU_INTR_EN_PCIE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00000180 LINE_SERDES_INTR_EN_3 |
 | +-------------------------------------------+
 | | bit  15   R/W  SFI2_TX_INTR_EN_DEVICE     |
 | | bit  14   R/W  SFI2_RX_INTR_EN_DEVICE     |
 | | bit  13   R/W  SFI1_TX_INTR_EN_DEVICE     |
 | | bit  12   R/W  SFI1_RX_INTR_EN_DEVICE     |
 | | bit  11:0 R/W  LINE_SERDES_INTR_EN_PCIE   |
 | +-------------------------------------------+
 | 
 | void   tl_ic_field_SFI2_TX_INTR_EN_DEVICE_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI2_TX_INTR_EN_DEVICE_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SFI2_RX_INTR_EN_DEVICE_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI2_RX_INTR_EN_DEVICE_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SFI1_TX_INTR_EN_DEVICE_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI1_TX_INTR_EN_DEVICE_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SFI1_RX_INTR_EN_DEVICE_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SFI1_RX_INTR_EN_DEVICE_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_LINE_SERDES_INTR_EN_PCIE_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LINE_SERDES_INTR_EN_PCIE_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_LINE_SERDES_INTR_EN_PCIE_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_LINE_SERDES_INTR_EN_PCIE_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000184 SYS_SERDES_INTR_EN_3A |
 | +-------------------------------------------+
 | | bit  21:20 R/W  LR_CSU_INTR_EN_PCIE       |
 | | bit  19:0  R/W  LR_SERDES_INTR_EN_PCIE    |
 | +-------------------------------------------+
 | 
 | void   tl_ic_field_LR_CSU_INTR_EN_PCIE_set          ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LR_CSU_INTR_EN_PCIE_get          ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_LR_CSU_INTR_EN_PCIE_set    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_LR_CSU_INTR_EN_PCIE_get    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_LR_SERDES_INTR_EN_PCIE_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LR_SERDES_INTR_EN_PCIE_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_LR_SERDES_INTR_EN_PCIE_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_LR_SERDES_INTR_EN_PCIE_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000188 SYS_SERDES_INTR_EN_3B |
 | +-------------------------------------------+
 | | bit  24    R/W  SR_CSU_INTR_EN_DEVICE     |
 | | bit  23:16 R/W  SR_SERDES_INTR_EN_PCIE    |
 | | bit  15:0  R/W  SYS_SERDES_INTR_EN_PCIE   |
 | +-------------------------------------------+
 | 
 | void   tl_ic_field_SR_CSU_INTR_EN_DEVICE_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SR_CSU_INTR_EN_DEVICE_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SR_SERDES_INTR_EN_PCIE_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SR_SERDES_INTR_EN_PCIE_get        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_SR_SERDES_INTR_EN_PCIE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_SR_SERDES_INTR_EN_PCIE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_SYS_SERDES_INTR_EN_PCIE_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SYS_SERDES_INTR_EN_PCIE_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_SYS_SERDES_INTR_EN_PCIE_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_SYS_SERDES_INTR_EN_PCIE_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0000018c OH_SERDES_INTR_EN_3   |
 | +-------------------------------------------+
 | | bit  16    R/W  MGMT_FEGE_INTR_EN_DEVICE  |
 | | bit  15    R/W  OH_CSU_INTR_EN_DEVICE     |
 | | bit  14:12 R/W  OH_TX_SERDES_INTR_EN_PCIE |
 | | bit  10:8  R/W  OH_RX_SERDES_INTR_EN_PCIE |
 | | bit  4     R/W  PCIE_CSU_INTR_EN_DEVICE   |
 | | bit  3:0   R/W  PCIE_SERDES_INTR_EN_PCIE  |
 | +-------------------------------------------+
 | 
 | void   tl_ic_field_MGMT_FEGE_INTR_EN_DEVICE_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_MGMT_FEGE_INTR_EN_DEVICE_get        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OH_CSU_INTR_EN_DEVICE_set           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OH_CSU_INTR_EN_DEVICE_get           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OH_TX_SERDES_INTR_EN_PCIE_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OH_TX_SERDES_INTR_EN_PCIE_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_OH_TX_SERDES_INTR_EN_PCIE_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_OH_TX_SERDES_INTR_EN_PCIE_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_OH_RX_SERDES_INTR_EN_PCIE_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OH_RX_SERDES_INTR_EN_PCIE_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_OH_RX_SERDES_INTR_EN_PCIE_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_OH_RX_SERDES_INTR_EN_PCIE_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_PCIE_CSU_INTR_EN_DEVICE_set         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PCIE_CSU_INTR_EN_DEVICE_get         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_PCIE_SERDES_INTR_EN_PCIE_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PCIE_SERDES_INTR_EN_PCIE_get        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_PCIE_SERDES_INTR_EN_PCIE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_PCIE_SERDES_INTR_EN_PCIE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000190 DCSU_INTR_EN_3    |
 | +---------------------------------------+
 | | bit  8 R/W  FIC2_DCSU_INTR_EN_DEVICE  |
 | | bit  7 R/W  FIC1_DCSU_INTR_EN_DEVICE  |
 | | bit  6 R/W  PTP_DCSU_INTR_EN_DEVICE   |
 | | bit  5 R/W  ENET2_DCSU_INTR_EN_DEVICE |
 | | bit  4 R/W  ENET1_DCSU_INTR_EN_DEVICE |
 | | bit  3 R/W  SYS2_DCSU_INTR_EN_DEVICE  |
 | | bit  2 R/W  SYS_DCSU_INTR_EN_DEVICE   |
 | | bit  1 R/W  OTN2_DCSU_INTR_EN_DEVICE  |
 | | bit  0 R/W  OTN1_DCSU_INTR_EN_DEVICE  |
 | +---------------------------------------+
 | 
 | void   tl_ic_field_FIC2_DCSU_INTR_EN_DEVICE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_FIC2_DCSU_INTR_EN_DEVICE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_FIC1_DCSU_INTR_EN_DEVICE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_FIC1_DCSU_INTR_EN_DEVICE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_PTP_DCSU_INTR_EN_DEVICE_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PTP_DCSU_INTR_EN_DEVICE_get   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET2_DCSU_INTR_EN_DEVICE_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET2_DCSU_INTR_EN_DEVICE_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_ENET1_DCSU_INTR_EN_DEVICE_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_ENET1_DCSU_INTR_EN_DEVICE_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SYS2_DCSU_INTR_EN_DEVICE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SYS2_DCSU_INTR_EN_DEVICE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_SYS_DCSU_INTR_EN_DEVICE_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SYS_DCSU_INTR_EN_DEVICE_get   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OTN2_DCSU_INTR_EN_DEVICE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OTN2_DCSU_INTR_EN_DEVICE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_OTN1_DCSU_INTR_EN_DEVICE_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OTN1_DCSU_INTR_EN_DEVICE_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x000001c0 LINE_SERDES_INTR_EN_4 |
 | +-------------------------------------------+
 | | bit  11:0 R/W  LINE_SERDES_INTR_EN_MIPS   |
 | +-------------------------------------------+
 | 
 | void   tl_ic_field_LINE_SERDES_INTR_EN_MIPS_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LINE_SERDES_INTR_EN_MIPS_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_LINE_SERDES_INTR_EN_MIPS_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_LINE_SERDES_INTR_EN_MIPS_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x000001c4 SYS_SERDES_INTR_EN_4A |
 | +-------------------------------------------+
 | | bit  21:20 R/W  LR_CSU_INTR_EN_MIPS       |
 | | bit  19:0  R/W  LR_SERDES_INTR_EN_MIPS    |
 | +-------------------------------------------+
 | 
 | void   tl_ic_field_LR_CSU_INTR_EN_MIPS_set          ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LR_CSU_INTR_EN_MIPS_get          ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_LR_CSU_INTR_EN_MIPS_set    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_LR_CSU_INTR_EN_MIPS_get    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_LR_SERDES_INTR_EN_MIPS_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LR_SERDES_INTR_EN_MIPS_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_LR_SERDES_INTR_EN_MIPS_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_LR_SERDES_INTR_EN_MIPS_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x000001c8 SYS_SERDES_INTR_EN_4B |
 | +-------------------------------------------+
 | | bit  23:16 R/W  SR_SERDES_INTR_EN_MIPS    |
 | | bit  15:0  R/W  SYS_SERDES_INTR_EN_MIPS   |
 | +-------------------------------------------+
 | 
 | void   tl_ic_field_SR_SERDES_INTR_EN_MIPS_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SR_SERDES_INTR_EN_MIPS_get        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_SR_SERDES_INTR_EN_MIPS_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_SR_SERDES_INTR_EN_MIPS_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_SYS_SERDES_INTR_EN_MIPS_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SYS_SERDES_INTR_EN_MIPS_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_SYS_SERDES_INTR_EN_MIPS_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_SYS_SERDES_INTR_EN_MIPS_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x000001cc OH_SERDES_INTR_EN_4   |
 | +-------------------------------------------+
 | | bit  14:12 R/W  OH_TX_SERDES_INTR_EN_MIPS |
 | | bit  10:8  R/W  OH_RX_SERDES_INTR_EN_MIPS |
 | | bit  3:0   R/W  PCIE_SERDES_INTR_EN_MIPS  |
 | +-------------------------------------------+
 | 
 | void   tl_ic_field_OH_TX_SERDES_INTR_EN_MIPS_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OH_TX_SERDES_INTR_EN_MIPS_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_OH_TX_SERDES_INTR_EN_MIPS_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_OH_TX_SERDES_INTR_EN_MIPS_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_OH_RX_SERDES_INTR_EN_MIPS_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_OH_RX_SERDES_INTR_EN_MIPS_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_OH_RX_SERDES_INTR_EN_MIPS_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_OH_RX_SERDES_INTR_EN_MIPS_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_PCIE_SERDES_INTR_EN_MIPS_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_PCIE_SERDES_INTR_EN_MIPS_get        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_PCIE_SERDES_INTR_EN_MIPS_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_PCIE_SERDES_INTR_EN_MIPS_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000240 SERDES_CSU_RESET_SYNC |
 | +-------------------------------------------+
 | | bit  31:16 R/W  SYS_RST_SYNC              |
 | | bit  11:0  R/W  LINE_RST_SYNC             |
 | +-------------------------------------------+
 | 
 | void   tl_ic_field_SYS_RST_SYNC_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_SYS_RST_SYNC_get        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_SYS_RST_SYNC_set  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_SYS_RST_SYNC_get  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   tl_ic_field_LINE_RST_SYNC_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_LINE_RST_SYNC_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_range_LINE_RST_SYNC_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 tl_ic_field_range_LINE_RST_SYNC_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x0000024c APRAM_CFG |
 | +-------------------------------+
 | | bit  2 R/W  APRAM_RAM_LOW_PWR |
 | | bit  1 R/W  APRAM_LOW_PWR     |
 | | bit  0 R/W  APRAM_RESET       |
 | +-------------------------------+
 | 
 | void   tl_ic_field_APRAM_RAM_LOW_PWR_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_APRAM_RAM_LOW_PWR_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_APRAM_LOW_PWR_set     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_APRAM_LOW_PWR_get     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   tl_ic_field_APRAM_RESET_set       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 tl_ic_field_APRAM_RESET_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register (0x00000250 + (N) * 4) GPCFG |
 | |     N[0..11]                          |
 | +---------------------------------------+
 | | bit  31:0 R  GP_CONFIG                |
 | +---------------------------------------+
 | 
 | void   tl_ic_lfield_GP_CONFIG_get       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value[12] )
 | UINT32 tl_ic_lfield_range_GP_CONFIG_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x00000284 AVS_STATUS |
 | +--------------------------------+
 | | bit  0 R  AVS_ENB              |
 | +--------------------------------+
 | 
 | UINT32 tl_ic_field_AVS_ENB_get ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _TL_IC_IO_H
#define _TL_IC_IO_H

#include "digi_api.h"
#include "tl_ic_regs.h"
#include "tl_ic_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 tl_ic_buffer_init               ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void                 tl_ic_buffer_flush              ( tl_ic_buffer_t *b_ptr )
 | UINT32               tl_ic_reg_read                  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 tl_ic_reg_write                 ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 tl_ic_field_set                 ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 tl_ic_action_on_write_field_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 tl_ic_burst_read                ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 tl_ic_burst_write               ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE tl_ic_poll                      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------+
 | void   tl_ic_reg_LP_CTRL_write                   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_LP_CTRL_field_set               ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_LP_CTRL_read                    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_RAM_LP_CTRL_write               ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_RAM_LP_CTRL_field_set           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_RAM_LP_CTRL_read                ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_LINE_SERDES_IDDQ_write          ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_LINE_SERDES_IDDQ_field_set      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_LINE_SERDES_IDDQ_read           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_SYS_SERDES_IDDQ_1_write         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_SYS_SERDES_IDDQ_1_field_set     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_SYS_SERDES_IDDQ_1_read          ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_SYS_SERDES_IDDQ_2_write         ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_SYS_SERDES_IDDQ_2_field_set     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_SYS_SERDES_IDDQ_2_read          ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_PCIE_OH_SERDES_IDDQ_write       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_PCIE_OH_SERDES_IDDQ_field_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_PCIE_OH_SERDES_IDDQ_read        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_DCSU_IDDQ_write                 ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_DCSU_IDDQ_field_set             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_DCSU_IDDQ_read                  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_RESET_CTRL0_write               ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_RESET_CTRL0_field_set           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_RESET_CTRL0_read                ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_RESET_CTRL1_write               ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_RESET_CTRL1_field_set           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_RESET_CTRL1_read                ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_RESET_CTRL2_write               ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_RESET_CTRL2_field_set           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_RESET_CTRL2_read                ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_RESET_CTRL3_write               ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_RESET_CTRL3_field_set           ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_RESET_CTRL3_read                ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_IRIG_CTRL_write                 ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_IRIG_CTRL_field_set             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_IRIG_CTRL_read                  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_LINE_SERDES_INTR_EN_1_write     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_LINE_SERDES_INTR_EN_1_field_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_LINE_SERDES_INTR_EN_1_read      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_SYS_SERDES_INTR_EN_1B_write     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_SYS_SERDES_INTR_EN_1B_field_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_1B_read      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_OH_SERDES_INTR_EN_1_write       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_OH_SERDES_INTR_EN_1_field_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_OH_SERDES_INTR_EN_1_read        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_DCSU_INTR_EN_1_write            ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_DCSU_INTR_EN_1_field_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_DCSU_INTR_EN_1_read             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_LINE_SERDES_INTR_EN_2_write     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_LINE_SERDES_INTR_EN_2_field_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_LINE_SERDES_INTR_EN_2_read      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_SYS_SERDES_INTR_EN_2A_write     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_SYS_SERDES_INTR_EN_2A_field_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_2A_read      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_SYS_SERDES_INTR_EN_2B_write     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_SYS_SERDES_INTR_EN_2B_field_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_2B_read      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_OH_SERDES_INTR_EN_2_write       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_OH_SERDES_INTR_EN_2_field_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_OH_SERDES_INTR_EN_2_read        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_DCSU_INTR_EN_2_write            ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_DCSU_INTR_EN_2_field_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_DCSU_INTR_EN_2_read             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_LINE_SERDES_INTR_EN_3_write     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_LINE_SERDES_INTR_EN_3_field_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_LINE_SERDES_INTR_EN_3_read      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_SYS_SERDES_INTR_EN_3A_write     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_SYS_SERDES_INTR_EN_3A_field_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_3A_read      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_SYS_SERDES_INTR_EN_3B_write     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_SYS_SERDES_INTR_EN_3B_field_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_3B_read      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_OH_SERDES_INTR_EN_3_write       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_OH_SERDES_INTR_EN_3_field_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_OH_SERDES_INTR_EN_3_read        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_DCSU_INTR_EN_3_write            ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_DCSU_INTR_EN_3_field_set        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_DCSU_INTR_EN_3_read             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_LINE_SERDES_INTR_EN_4_write     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_LINE_SERDES_INTR_EN_4_field_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_LINE_SERDES_INTR_EN_4_read      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_SYS_SERDES_INTR_EN_4A_write     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_SYS_SERDES_INTR_EN_4A_field_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_4A_read      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_SYS_SERDES_INTR_EN_4B_write     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_SYS_SERDES_INTR_EN_4B_field_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_SYS_SERDES_INTR_EN_4B_read      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_OH_SERDES_INTR_EN_4_write       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_OH_SERDES_INTR_EN_4_field_set   ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_OH_SERDES_INTR_EN_4_read        ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_SERDES_CSU_RESET_SYNC_write     ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_SERDES_CSU_RESET_SYNC_field_set ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_SERDES_CSU_RESET_SYNC_read      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_APRAM_CFG_write                 ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   tl_ic_reg_APRAM_CFG_field_set             ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 tl_ic_reg_APRAM_CFG_read                  ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | UINT32 tl_ic_reg_LINE_SERDES_INTR_SUM_read       ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | UINT32 tl_ic_reg_SYS_SERDES_INTR_SUM_1_read      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | UINT32 tl_ic_reg_SYS_SERDES_INTR_SUM_2_read      ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | UINT32 tl_ic_reg_PCIE_OH_SERDES_INTR_SUM_read    ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | UINT32 tl_ic_reg_DCSU_INTR_SUM_read              ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | UINT32 tl_ic_reg_AVS_STATUS_read                 ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   tl_ic_reg_GPCFG_array_burst_read          ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 tl_ic_reg_GPCFG_array_read                ( tl_ic_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 N )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _TL_IC_IO_H */
