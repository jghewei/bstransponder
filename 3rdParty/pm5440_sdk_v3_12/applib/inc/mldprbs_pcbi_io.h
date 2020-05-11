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
 *     and register accessor functions for the mldprbs_pcbi block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing mldprbs_pcbi io functions is:
 | {
 |     mldprbs_pcbi_buffer_t b_ptr[1];
 |     mldprbs_pcbi_buffer_init( b_ptr, h_ptr );
 |     value = mldprbs_pcbi_field_<FIELD>_get( b_ptr, h_ptr );
 |     mldprbs_pcbi_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     mldprbs_pcbi_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = mldprbs_pcbi_field_<FIELD>_get( NULL, h_ptr );
 | mldprbs_pcbi_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------------------------------------+
 | | Register (0x00000c00 + (N) * 0x4) RX_MLDPRBS_MULTIPLEXER_REG |
 | |     N[0..11]                                                 |
 | +--------------------------------------------------------------+
 | | bit  2:1 R/W  MLDPRBS_RX_MUX_SEL                             |
 | | bit  0   R/W  INGRESS_RX_MUX_SEL                             |
 | +--------------------------------------------------------------+
 | 
 | void   mldprbs_pcbi_field_MLDPRBS_RX_MUX_SEL_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mldprbs_pcbi_field_MLDPRBS_RX_MUX_SEL_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   mldprbs_pcbi_field_range_MLDPRBS_RX_MUX_SEL_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_MLDPRBS_RX_MUX_SEL_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_INGRESS_RX_MUX_SEL_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mldprbs_pcbi_field_INGRESS_RX_MUX_SEL_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x00000c30 + (N) * 0x4) TX_MLDPRBS_MULTIPLEXER_REG |
 | |     N[0..11]                                                 |
 | +--------------------------------------------------------------+
 | | bit  5:4 R/W  XOFF_TX_MUX_SEL                                |
 | | bit  3   R/W  LPBK_TX_MUX_SEL                                |
 | | bit  2:1 R/W  EGRESS_TX_MUX_SEL                              |
 | | bit  0   R/W  MLDPRBS_TX_MUX_SEL                             |
 | +--------------------------------------------------------------+
 | 
 | void   mldprbs_pcbi_field_XOFF_TX_MUX_SEL_set         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mldprbs_pcbi_field_XOFF_TX_MUX_SEL_get         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   mldprbs_pcbi_field_range_XOFF_TX_MUX_SEL_set   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_XOFF_TX_MUX_SEL_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_LPBK_TX_MUX_SEL_set         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mldprbs_pcbi_field_LPBK_TX_MUX_SEL_get         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   mldprbs_pcbi_field_EGRESS_TX_MUX_SEL_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mldprbs_pcbi_field_EGRESS_TX_MUX_SEL_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   mldprbs_pcbi_field_range_EGRESS_TX_MUX_SEL_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_EGRESS_TX_MUX_SEL_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_MLDPRBS_TX_MUX_SEL_set      ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mldprbs_pcbi_field_MLDPRBS_TX_MUX_SEL_get      ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000c60 + (N) * 0x4) RX_MLDPRBS_CONFIG_REG |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  0 R/W  RX_FIFO_SYNC_RST                            |
 | +---------------------------------------------------------+
 | 
 | void   mldprbs_pcbi_field_RX_FIFO_SYNC_RST_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mldprbs_pcbi_field_RX_FIFO_SYNC_RST_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000c90 + (N) * 0x4) TX_MLDPRBS_CONFIG_REG |
 | |     N[0..11]                                            |
 | +---------------------------------------------------------+
 | | bit  8:4 R/W  TX_FIFO_THRSHD_VAL                        |
 | | bit  3:2 R/W  TX_FIFO_RAM_ERR_INJECT                    |
 | | bit  1   R/W  TX_FIFO_RAM_SD                            |
 | | bit  0   R/W  TX_FIFO_SYNC_RST                          |
 | +---------------------------------------------------------+
 | 
 | void   mldprbs_pcbi_field_TX_FIFO_THRSHD_VAL_set           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_FIFO_THRSHD_VAL_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   mldprbs_pcbi_field_range_TX_FIFO_THRSHD_VAL_set     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_TX_FIFO_THRSHD_VAL_get     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_TX_FIFO_RAM_ERR_INJECT_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_FIFO_RAM_ERR_INJECT_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   mldprbs_pcbi_field_range_TX_FIFO_RAM_ERR_INJECT_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_TX_FIFO_RAM_ERR_INJECT_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_TX_FIFO_RAM_SD_set               ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_FIFO_RAM_SD_get               ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | void   mldprbs_pcbi_field_TX_FIFO_SYNC_RST_set             ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_FIFO_SYNC_RST_get             ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------+
 | | Register 0x00000cc0 MLD_CONFIG_REGISTER |
 | +-----------------------------------------+
 | | bit  26:24 R/W  STL256_RAM_SD           |
 | | bit  18:16 R/W  STL256_SYNC_RST         |
 | | bit  6:4   R/W  STL256_CLK_EN           |
 | | bit  3:2   R/W  SFIS_CLK_EN             |
 | | bit  1:0   R/W  SFI51_CLK_EN            |
 | +-----------------------------------------+
 | 
 | void   mldprbs_pcbi_field_STL256_RAM_SD_set         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_STL256_RAM_SD_get         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_STL256_RAM_SD_set   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_STL256_RAM_SD_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_STL256_SYNC_RST_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_STL256_SYNC_RST_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_STL256_SYNC_RST_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_STL256_SYNC_RST_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_STL256_CLK_EN_set         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_STL256_CLK_EN_get         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_STL256_CLK_EN_set   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_STL256_CLK_EN_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_SFIS_CLK_EN_set           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SFIS_CLK_EN_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_SFIS_CLK_EN_set     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_SFIS_CLK_EN_get     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_SFI51_CLK_EN_set          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SFI51_CLK_EN_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_SFI51_CLK_EN_set    ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_SFI51_CLK_EN_get    ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000cf0 MLDPRBS_INT_ENABLE_REG0 |
 | +---------------------------------------------+
 | | bit  20    R/W  MLDPRBSPCBI_IRQ_EN0         |
 | | bit  19    R/W  FCDLVR_IRQ_EN0              |
 | | bit  18    R/W  FCILVR_IRQ_EN0              |
 | | bit  17    R/W  SDHPMG1_IRQ_EN0             |
 | | bit  16    R/W  SDHPMG0_IRQ_EN0             |
 | | bit  15    R/W  PRBS_IRQ_EN0                |
 | | bit  14:12 R/W  CBRFSGM_IRQ_EN0             |
 | | bit  10:8  R/W  STL_IRQ_EN0                 |
 | | bit  7:4   R/W  SFIS_IRQ_EN0                |
 | | bit  1:0   R/W  SFI51_IRQ_EN0               |
 | +---------------------------------------------+
 | 
 | void   mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN0_set   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN0_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_FCDLVR_IRQ_EN0_set        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_FCDLVR_IRQ_EN0_get        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_FCILVR_IRQ_EN0_set        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_FCILVR_IRQ_EN0_get        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_SDHPMG1_IRQ_EN0_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SDHPMG1_IRQ_EN0_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_SDHPMG0_IRQ_EN0_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SDHPMG0_IRQ_EN0_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_PRBS_IRQ_EN0_set          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_PRBS_IRQ_EN0_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_CBRFSGM_IRQ_EN0_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_CBRFSGM_IRQ_EN0_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN0_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN0_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_STL_IRQ_EN0_set           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_STL_IRQ_EN0_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_STL_IRQ_EN0_set     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_STL_IRQ_EN0_get     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_SFIS_IRQ_EN0_set          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SFIS_IRQ_EN0_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_SFIS_IRQ_EN0_set    ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_SFIS_IRQ_EN0_get    ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_SFI51_IRQ_EN0_set         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SFI51_IRQ_EN0_get         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_SFI51_IRQ_EN0_set   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_SFI51_IRQ_EN0_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000cf4 MLDPRBS_INT_ENABLE_REG1 |
 | +---------------------------------------------+
 | | bit  20    R/W  MLDPRBSPCBI_IRQ_EN1         |
 | | bit  19    R/W  FCDLVR_IRQ_EN1              |
 | | bit  18    R/W  FCILVR_IRQ_EN1              |
 | | bit  17    R/W  SDHPMG1_IRQ_EN1             |
 | | bit  16    R/W  SDHPMG0_IRQ_EN1             |
 | | bit  15    R/W  PRBS_IRQ_EN1                |
 | | bit  14:12 R/W  CBRFSGM_IRQ_EN1             |
 | | bit  10:8  R/W  STL_IRQ_EN1                 |
 | | bit  7:4   R/W  SFIS_IRQ_EN1                |
 | | bit  1:0   R/W  SFI51_IRQ_EN1               |
 | +---------------------------------------------+
 | 
 | void   mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN1_set   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN1_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_FCDLVR_IRQ_EN1_set        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_FCDLVR_IRQ_EN1_get        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_FCILVR_IRQ_EN1_set        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_FCILVR_IRQ_EN1_get        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_SDHPMG1_IRQ_EN1_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SDHPMG1_IRQ_EN1_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_SDHPMG0_IRQ_EN1_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SDHPMG0_IRQ_EN1_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_PRBS_IRQ_EN1_set          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_PRBS_IRQ_EN1_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_CBRFSGM_IRQ_EN1_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_CBRFSGM_IRQ_EN1_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN1_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN1_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_STL_IRQ_EN1_set           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_STL_IRQ_EN1_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_STL_IRQ_EN1_set     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_STL_IRQ_EN1_get     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_SFIS_IRQ_EN1_set          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SFIS_IRQ_EN1_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_SFIS_IRQ_EN1_set    ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_SFIS_IRQ_EN1_get    ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_SFI51_IRQ_EN1_set         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SFI51_IRQ_EN1_get         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_SFI51_IRQ_EN1_set   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_SFI51_IRQ_EN1_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000cf8 MLDPRBS_INT_ENABLE_REG2 |
 | +---------------------------------------------+
 | | bit  20    R/W  MLDPRBSPCBI_IRQ_EN2         |
 | | bit  19    R/W  FCDLVR_IRQ_EN2              |
 | | bit  18    R/W  FCILVR_IRQ_EN2              |
 | | bit  17    R/W  SDHPMG1_IRQ_EN2             |
 | | bit  16    R/W  SDHPMG0_IRQ_EN2             |
 | | bit  15    R/W  PRBS_IRQ_EN2                |
 | | bit  14:12 R/W  CBRFSGM_IRQ_EN2             |
 | | bit  10:8  R/W  STL_IRQ_EN2                 |
 | | bit  7:4   R/W  SFIS_IRQ_EN2                |
 | | bit  1:0   R/W  SFI51_IRQ_EN2               |
 | +---------------------------------------------+
 | 
 | void   mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN2_set   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN2_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_FCDLVR_IRQ_EN2_set        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_FCDLVR_IRQ_EN2_get        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_FCILVR_IRQ_EN2_set        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_FCILVR_IRQ_EN2_get        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_SDHPMG1_IRQ_EN2_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SDHPMG1_IRQ_EN2_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_SDHPMG0_IRQ_EN2_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SDHPMG0_IRQ_EN2_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_PRBS_IRQ_EN2_set          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_PRBS_IRQ_EN2_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_CBRFSGM_IRQ_EN2_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_CBRFSGM_IRQ_EN2_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN2_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN2_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_STL_IRQ_EN2_set           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_STL_IRQ_EN2_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_STL_IRQ_EN2_set     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_STL_IRQ_EN2_get     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_SFIS_IRQ_EN2_set          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SFIS_IRQ_EN2_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_SFIS_IRQ_EN2_set    ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_SFIS_IRQ_EN2_get    ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_SFI51_IRQ_EN2_set         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SFI51_IRQ_EN2_get         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_SFI51_IRQ_EN2_set   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_SFI51_IRQ_EN2_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000cfc MLDPRBS_INT_ENABLE_REG3 |
 | +---------------------------------------------+
 | | bit  20    R/W  MLDPRBSPCBI_IRQ_EN3         |
 | | bit  19    R/W  FCDLVR_IRQ_EN3              |
 | | bit  18    R/W  FCILVR_IRQ_EN3              |
 | | bit  17    R/W  SDHPMG1_IRQ_EN3             |
 | | bit  16    R/W  SDHPMG0_IRQ_EN3             |
 | | bit  15    R/W  PRBS_IRQ_EN3                |
 | | bit  14:12 R/W  CBRFSGM_IRQ_EN3             |
 | | bit  10:8  R/W  STL_IRQ_EN3                 |
 | | bit  7:4   R/W  SFIS_IRQ_EN3                |
 | | bit  1:0   R/W  SFI51_IRQ_EN3               |
 | +---------------------------------------------+
 | 
 | void   mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN3_set   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_MLDPRBSPCBI_IRQ_EN3_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_FCDLVR_IRQ_EN3_set        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_FCDLVR_IRQ_EN3_get        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_FCILVR_IRQ_EN3_set        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_FCILVR_IRQ_EN3_get        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_SDHPMG1_IRQ_EN3_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SDHPMG1_IRQ_EN3_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_SDHPMG0_IRQ_EN3_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SDHPMG0_IRQ_EN3_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_PRBS_IRQ_EN3_set          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_PRBS_IRQ_EN3_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_CBRFSGM_IRQ_EN3_set       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_CBRFSGM_IRQ_EN3_get       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN3_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_CBRFSGM_IRQ_EN3_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_STL_IRQ_EN3_set           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_STL_IRQ_EN3_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_STL_IRQ_EN3_set     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_STL_IRQ_EN3_get     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_SFIS_IRQ_EN3_set          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SFIS_IRQ_EN3_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_SFIS_IRQ_EN3_set    ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_SFIS_IRQ_EN3_get    ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mldprbs_pcbi_field_SFI51_IRQ_EN3_set         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_SFI51_IRQ_EN3_get         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_range_SFI51_IRQ_EN3_set   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mldprbs_pcbi_field_range_SFI51_IRQ_EN3_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000d00 MLDPRBS_GLOBAL_CONFIG_REG |
 | +-----------------------------------------------+
 | | bit  0 R/W  MLDPRBS_SYNC_RST                  |
 | +-----------------------------------------------+
 | 
 | void   mldprbs_pcbi_field_MLDPRBS_SYNC_RST_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_MLDPRBS_SYNC_RST_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00000d10 TX_XFI_FIFO_UNFL_INT |
 | +------------------------------------------+
 | | bit  11 R/W  TX_XFI_FIFO_11_UNFL_INT_I   |
 | | bit  10 R/W  TX_XFI_FIFO_10_UNFL_INT_I   |
 | | bit  9  R/W  TX_XFI_FIFO_9_UNFL_INT_I    |
 | | bit  8  R/W  TX_XFI_FIFO_8_UNFL_INT_I    |
 | | bit  7  R/W  TX_XFI_FIFO_7_UNFL_INT_I    |
 | | bit  6  R/W  TX_XFI_FIFO_6_UNFL_INT_I    |
 | | bit  5  R/W  TX_XFI_FIFO_5_UNFL_INT_I    |
 | | bit  4  R/W  TX_XFI_FIFO_4_UNFL_INT_I    |
 | | bit  3  R/W  TX_XFI_FIFO_3_UNFL_INT_I    |
 | | bit  2  R/W  TX_XFI_FIFO_2_UNFL_INT_I    |
 | | bit  1  R/W  TX_XFI_FIFO_1_UNFL_INT_I    |
 | | bit  0  R/W  TX_XFI_FIFO_0_UNFL_INT_I    |
 | +------------------------------------------+
 | 
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_I_set_to_clear ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_I_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_I_poll         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_I_set_to_clear ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_I_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_I_poll         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------+
 | | Register 0x00000d14 TX_XFI_FIFO_UNFL_INT_EN |
 | +---------------------------------------------+
 | | bit  11 R/W  TX_XFI_FIFO_11_UNFL_INT_E      |
 | | bit  10 R/W  TX_XFI_FIFO_10_UNFL_INT_E      |
 | | bit  9  R/W  TX_XFI_FIFO_9_UNFL_INT_E       |
 | | bit  8  R/W  TX_XFI_FIFO_8_UNFL_INT_E       |
 | | bit  7  R/W  TX_XFI_FIFO_7_UNFL_INT_E       |
 | | bit  6  R/W  TX_XFI_FIFO_6_UNFL_INT_E       |
 | | bit  5  R/W  TX_XFI_FIFO_5_UNFL_INT_E       |
 | | bit  4  R/W  TX_XFI_FIFO_4_UNFL_INT_E       |
 | | bit  3  R/W  TX_XFI_FIFO_3_UNFL_INT_E       |
 | | bit  2  R/W  TX_XFI_FIFO_2_UNFL_INT_E       |
 | | bit  1  R/W  TX_XFI_FIFO_1_UNFL_INT_E       |
 | | bit  0  R/W  TX_XFI_FIFO_0_UNFL_INT_E       |
 | +---------------------------------------------+
 | 
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_E_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_E_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_E_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_E_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00000d18 TX_XFI_FIFO_UNFL_INT_VAL |
 | +----------------------------------------------+
 | | bit  11 R  TX_XFI_FIFO_11_UNFL_INT_V         |
 | | bit  10 R  TX_XFI_FIFO_10_UNFL_INT_V         |
 | | bit  9  R  TX_XFI_FIFO_9_UNFL_INT_V          |
 | | bit  8  R  TX_XFI_FIFO_8_UNFL_INT_V          |
 | | bit  7  R  TX_XFI_FIFO_7_UNFL_INT_V          |
 | | bit  6  R  TX_XFI_FIFO_6_UNFL_INT_V          |
 | | bit  5  R  TX_XFI_FIFO_5_UNFL_INT_V          |
 | | bit  4  R  TX_XFI_FIFO_4_UNFL_INT_V          |
 | | bit  3  R  TX_XFI_FIFO_3_UNFL_INT_V          |
 | | bit  2  R  TX_XFI_FIFO_2_UNFL_INT_V          |
 | | bit  1  R  TX_XFI_FIFO_1_UNFL_INT_V          |
 | | bit  0  R  TX_XFI_FIFO_0_UNFL_INT_V          |
 | +----------------------------------------------+
 | 
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_V_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_11_UNFL_INT_V_poll ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_V_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_10_UNFL_INT_V_poll ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_9_UNFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_8_UNFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_7_UNFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_6_UNFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_5_UNFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_4_UNFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_3_UNFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_2_UNFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_1_UNFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_0_UNFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x00000d1c TX_XFI_FIFO_OVFL_INT |
 | +------------------------------------------+
 | | bit  11 R/W  TX_XFI_FIFO_11_OVFL_INT_I   |
 | | bit  10 R/W  TX_XFI_FIFO_10_OVFL_INT_I   |
 | | bit  9  R/W  TX_XFI_FIFO_9_OVFL_INT_I    |
 | | bit  8  R/W  TX_XFI_FIFO_8_OVFL_INT_I    |
 | | bit  7  R/W  TX_XFI_FIFO_7_OVFL_INT_I    |
 | | bit  6  R/W  TX_XFI_FIFO_6_OVFL_INT_I    |
 | | bit  5  R/W  TX_XFI_FIFO_5_OVFL_INT_I    |
 | | bit  4  R/W  TX_XFI_FIFO_4_OVFL_INT_I    |
 | | bit  3  R/W  TX_XFI_FIFO_3_OVFL_INT_I    |
 | | bit  2  R/W  TX_XFI_FIFO_2_OVFL_INT_I    |
 | | bit  1  R/W  TX_XFI_FIFO_1_OVFL_INT_I    |
 | | bit  0  R/W  TX_XFI_FIFO_0_OVFL_INT_I    |
 | +------------------------------------------+
 | 
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_I_set_to_clear ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_I_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_I_poll         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_I_set_to_clear ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_I_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_I_poll         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_I_set_to_clear  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_I_get           ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_I_poll          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------+
 | | Register 0x00000d20 TX_XFI_FIFO_OVFL_INT_EN |
 | +---------------------------------------------+
 | | bit  11 R/W  TX_XFI_FIFO_11_OVFL_INT_E      |
 | | bit  10 R/W  TX_XFI_FIFO_10_OVFL_INT_E      |
 | | bit  9  R/W  TX_XFI_FIFO_9_OVFL_INT_E       |
 | | bit  8  R/W  TX_XFI_FIFO_8_OVFL_INT_E       |
 | | bit  7  R/W  TX_XFI_FIFO_7_OVFL_INT_E       |
 | | bit  6  R/W  TX_XFI_FIFO_6_OVFL_INT_E       |
 | | bit  5  R/W  TX_XFI_FIFO_5_OVFL_INT_E       |
 | | bit  4  R/W  TX_XFI_FIFO_4_OVFL_INT_E       |
 | | bit  3  R/W  TX_XFI_FIFO_3_OVFL_INT_E       |
 | | bit  2  R/W  TX_XFI_FIFO_2_OVFL_INT_E       |
 | | bit  1  R/W  TX_XFI_FIFO_1_OVFL_INT_E       |
 | | bit  0  R/W  TX_XFI_FIFO_0_OVFL_INT_E       |
 | +---------------------------------------------+
 | 
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_E_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_E_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_E_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_E_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_E_set  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_E_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x00000d24 TX_XFI_FIFO_OVFL_INT_VAL |
 | +----------------------------------------------+
 | | bit  11 R  TX_XFI_FIFO_11_OVFL_INT_V         |
 | | bit  10 R  TX_XFI_FIFO_10_OVFL_INT_V         |
 | | bit  9  R  TX_XFI_FIFO_9_OVFL_INT_V          |
 | | bit  8  R  TX_XFI_FIFO_8_OVFL_INT_V          |
 | | bit  7  R  TX_XFI_FIFO_7_OVFL_INT_V          |
 | | bit  6  R  TX_XFI_FIFO_6_OVFL_INT_V          |
 | | bit  5  R  TX_XFI_FIFO_5_OVFL_INT_V          |
 | | bit  4  R  TX_XFI_FIFO_4_OVFL_INT_V          |
 | | bit  3  R  TX_XFI_FIFO_3_OVFL_INT_V          |
 | | bit  2  R  TX_XFI_FIFO_2_OVFL_INT_V          |
 | | bit  1  R  TX_XFI_FIFO_1_OVFL_INT_V          |
 | | bit  0  R  TX_XFI_FIFO_0_OVFL_INT_V          |
 | +----------------------------------------------+
 | 
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_V_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_11_OVFL_INT_V_poll ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_V_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_10_OVFL_INT_V_poll ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_9_OVFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_8_OVFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_7_OVFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_6_OVFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_5_OVFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_4_OVFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_3_OVFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_2_OVFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_1_OVFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_V_get   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TX_XFI_FIFO_0_OVFL_INT_V_poll  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x00000d28 CBRC_TIP_EDGE_INT |
 | +---------------------------------------+
 | | bit  1 R/W  TIP_NEG_EDGE_INT_I        |
 | | bit  0 R/W  TIP_POS_EDGE_INT_I        |
 | +---------------------------------------+
 | 
 | void                 mldprbs_pcbi_field_TIP_NEG_EDGE_INT_I_set_to_clear ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TIP_NEG_EDGE_INT_I_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TIP_NEG_EDGE_INT_I_poll         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mldprbs_pcbi_field_TIP_POS_EDGE_INT_I_set_to_clear ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32               mldprbs_pcbi_field_TIP_POS_EDGE_INT_I_get          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TIP_POS_EDGE_INT_I_poll         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x00000d2c CBRC_TIP_EDGE_INT_E |
 | +-----------------------------------------+
 | | bit  1 R/W  TIP_NEG_EDGE_INT_E          |
 | | bit  0 R/W  TIP_POS_EDGE_INT_E          |
 | +-----------------------------------------+
 | 
 | void   mldprbs_pcbi_field_TIP_NEG_EDGE_INT_E_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TIP_NEG_EDGE_INT_E_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void   mldprbs_pcbi_field_TIP_POS_EDGE_INT_E_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | UINT32 mldprbs_pcbi_field_TIP_POS_EDGE_INT_E_get ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00000d30 CBRC_TIP_EDGE_INT_VAL |
 | +-------------------------------------------+
 | | bit  1 R  TIP_NEG_EDGE_INT_V              |
 | | bit  0 R  TIP_POS_EDGE_INT_V              |
 | +-------------------------------------------+
 | 
 | UINT32               mldprbs_pcbi_field_TIP_NEG_EDGE_INT_V_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TIP_NEG_EDGE_INT_V_poll ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               mldprbs_pcbi_field_TIP_POS_EDGE_INT_V_get  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_field_TIP_POS_EDGE_INT_V_poll ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MLDPRBS_PCBI_IO_H
#define _MLDPRBS_PCBI_IO_H

#include "cbrc_api.h"
#include "mldprbs_pcbi_regs.h"
#include "mldprbs_pcbi_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mldprbs_pcbi_buffer_init               ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | void                 mldprbs_pcbi_buffer_flush              ( mldprbs_pcbi_buffer_t *b_ptr )
 | UINT32               mldprbs_pcbi_reg_read                  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 mldprbs_pcbi_reg_write                 ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 mldprbs_pcbi_field_set                 ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 mldprbs_pcbi_action_on_write_field_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mldprbs_pcbi_burst_read                ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 mldprbs_pcbi_burst_write               ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_poll                      ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_write                      ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_field_set                  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_MLD_CONFIG_REGISTER_read                       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_write                  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_field_set              ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG0_read                   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_write                  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_field_set              ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG1_read                   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_write                  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_field_set              ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG2_read                   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_write                  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_field_set              ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_MLDPRBS_INT_ENABLE_REG3_read                   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 mldprbs_pcbi_reg_MLDPRBS_GLOBAL_CONFIG_REG_write                ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 mldprbs_pcbi_reg_MLDPRBS_GLOBAL_CONFIG_REG_field_set            ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_MLDPRBS_GLOBAL_CONFIG_REG_read                 ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_write         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_field_set     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_RX_MLDPRBS_MULTIPLEXER_REG_array_read          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_write         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_field_set     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_TX_MLDPRBS_MULTIPLEXER_REG_array_read          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mldprbs_pcbi_reg_RX_MLDPRBS_CONFIG_REG_array_write              ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mldprbs_pcbi_reg_RX_MLDPRBS_CONFIG_REG_array_field_set          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_RX_MLDPRBS_CONFIG_REG_array_read               ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_write              ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_field_set          ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_TX_MLDPRBS_CONFIG_REG_array_read               ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_write                        ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_action_on_write_field_set    ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_read                         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_poll                         ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_write                  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_field_set              ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_EN_read                   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_write                  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_field_set              ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_EN_read                   ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_write                      ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_field_set                  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_E_read                       ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | 
 | void                 mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_write                     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_action_on_write_field_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_read                      ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_poll                      ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_write                     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 value )
 | void                 mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_action_on_write_field_set ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_read                      ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_poll                      ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_read                  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_TX_XFI_FIFO_UNFL_INT_VAL_poll                  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_read                  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_TX_XFI_FIFO_OVFL_INT_VAL_poll                  ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_VAL_read                     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mldprbs_pcbi_reg_CBRC_TIP_EDGE_INT_VAL_poll                     ( mldprbs_pcbi_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MLDPRBS_PCBI_IO_H */
