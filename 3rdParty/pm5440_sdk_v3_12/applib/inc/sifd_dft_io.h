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
 *     and register accessor functions for the sifd_dft block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing sifd_dft io functions is:
 | {
 |     sifd_dft_buffer_t b_ptr[1];
 |     sifd_dft_buffer_init( b_ptr, h_ptr );
 |     value = sifd_dft_field_<FIELD>_get( b_ptr, h_ptr );
 |     sifd_dft_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     sifd_dft_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = sifd_dft_field_<FIELD>_get( NULL, h_ptr );
 | sifd_dft_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------+
 | | Register 0x00000000 SOFT_RESET  |
 | +---------------------------------+
 | | bit  4   R/W  FIC_SOFT_RST      |
 | | bit  3   R/W  DSIS_SOFT_RST     |
 | | bit  2:1 R/W  COPI_SOFT_RST     |
 | | bit  0   R/W  SIFD_DFT_SOFT_RST |
 | +---------------------------------+
 | 
 | void   sifd_dft_field_FIC_SOFT_RST_set        ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_FIC_SOFT_RST_get        ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_DSIS_SOFT_RST_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_DSIS_SOFT_RST_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_COPI_SOFT_RST_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_COPI_SOFT_RST_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_range_COPI_SOFT_RST_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sifd_dft_field_range_COPI_SOFT_RST_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sifd_dft_field_SIFD_DFT_SOFT_RST_set   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_SIFD_DFT_SOFT_RST_get   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x00000004 LOW_POWER_CFG |
 | +-----------------------------------+
 | | bit  4   R/W  FIC_LOWPWR_CFG      |
 | | bit  3   R/W  DSIS_LOWPWR_CFG     |
 | | bit  2:1 R/W  COPI_LOWPWR_CFG     |
 | | bit  0   R/W  SIFD_DFT_LOWPWR_CFG |
 | +-----------------------------------+
 | 
 | void   sifd_dft_field_FIC_LOWPWR_CFG_set        ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_FIC_LOWPWR_CFG_get        ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_DSIS_LOWPWR_CFG_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_DSIS_LOWPWR_CFG_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_COPI_LOWPWR_CFG_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_COPI_LOWPWR_CFG_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_range_COPI_LOWPWR_CFG_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sifd_dft_field_range_COPI_LOWPWR_CFG_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   sifd_dft_field_SIFD_DFT_LOWPWR_CFG_set   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_SIFD_DFT_LOWPWR_CFG_get   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x0000000c RAM_LOW_POWER_CFG |
 | +---------------------------------------+
 | | bit  2   R/W  FIC_RAM_LOWPWR_CFG      |
 | | bit  1:0 R/W  COPI_RAM_LOWPWR_CFG     |
 | +---------------------------------------+
 | 
 | void   sifd_dft_field_FIC_RAM_LOWPWR_CFG_set        ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_FIC_RAM_LOWPWR_CFG_get        ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_COPI_RAM_LOWPWR_CFG_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_COPI_RAM_LOWPWR_CFG_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_range_COPI_RAM_LOWPWR_CFG_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sifd_dft_field_range_COPI_RAM_LOWPWR_CFG_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000018 SIFD_SUBSYSTEM_INT_STAT |
 | +---------------------------------------------+
 | | bit  14 R  MPMA_IRQ                         |
 | | bit  13 R  MPMO_IRQ                         |
 | | bit  12 R  FIC_WRAP_IRQ                     |
 | | bit  11 R  FIC_IP_IRQ                       |
 | | bit  10 R  DSIS_IRQ                         |
 | | bit  9  R  IL2_RX_IRQ_2ND                   |
 | | bit  8  R  IL2_RX_IRQ                       |
 | | bit  7  R  IL2_TX_IRQ_2ND                   |
 | | bit  6  R  IL2_TX_IRQ                       |
 | | bit  5  R  COPI2_IRQ                        |
 | | bit  4  R  IL1_RX_IRQ_2ND                   |
 | | bit  3  R  IL1_RX_IRQ                       |
 | | bit  2  R  IL1_TX_IRQ_2ND                   |
 | | bit  1  R  IL1_TX_IRQ                       |
 | | bit  0  R  COPI1_IRQ                        |
 | +---------------------------------------------+
 | 
 | UINT32 sifd_dft_field_MPMA_IRQ_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32 sifd_dft_field_MPMO_IRQ_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32 sifd_dft_field_FIC_WRAP_IRQ_get   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32 sifd_dft_field_FIC_IP_IRQ_get     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32 sifd_dft_field_DSIS_IRQ_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32 sifd_dft_field_IL2_RX_IRQ_2ND_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32 sifd_dft_field_IL2_RX_IRQ_get     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32 sifd_dft_field_IL2_TX_IRQ_2ND_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32 sifd_dft_field_IL2_TX_IRQ_get     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32 sifd_dft_field_COPI2_IRQ_get      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32 sifd_dft_field_IL1_RX_IRQ_2ND_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32 sifd_dft_field_IL1_RX_IRQ_get     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32 sifd_dft_field_IL1_TX_IRQ_2ND_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32 sifd_dft_field_IL1_TX_IRQ_get     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | UINT32 sifd_dft_field_COPI1_IRQ_get      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +-----------------------------------------------+
 | | Register (0x0000001c + (N) * 0x4) SIFD_INT_EN |
 | |     N[0..3]                                   |
 | +-----------------------------------------------+
 | | bit  14 R/W  MPMA_IRQ_EN                      |
 | | bit  13 R/W  MPMO_IRQ_EN                      |
 | | bit  12 R/W  FIC_WRAP_IRQ_EN                  |
 | | bit  11 R/W  FIC_IP_IRQ_EN                    |
 | | bit  10 R/W  DSIS_IRQ_EN                      |
 | | bit  9  R/W  IL2_RX_IRQ_2ND_EN                |
 | | bit  8  R/W  IL2_RX_IRQ_EN                    |
 | | bit  7  R/W  IL2_TX_IRQ_2ND_EN                |
 | | bit  6  R/W  IL2_TX_IRQ_EN                    |
 | | bit  5  R/W  COPI2_IRQ_EN                     |
 | | bit  4  R/W  IL1_RX_IRQ_2ND_EN                |
 | | bit  3  R/W  IL1_RX_IRQ_EN                    |
 | | bit  2  R/W  IL1_TX_IRQ_2ND_EN                |
 | | bit  1  R/W  IL1_TX_IRQ_EN                    |
 | | bit  0  R/W  COPI1_IRQ_EN                     |
 | +-----------------------------------------------+
 | 
 | void   sifd_dft_field_MPMA_IRQ_EN_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_MPMA_IRQ_EN_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_MPMO_IRQ_EN_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_MPMO_IRQ_EN_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_FIC_WRAP_IRQ_EN_set   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_FIC_WRAP_IRQ_EN_get   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_FIC_IP_IRQ_EN_set     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_FIC_IP_IRQ_EN_get     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_DSIS_IRQ_EN_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_DSIS_IRQ_EN_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL2_RX_IRQ_2ND_EN_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL2_RX_IRQ_2ND_EN_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL2_RX_IRQ_EN_set     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL2_RX_IRQ_EN_get     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL2_TX_IRQ_2ND_EN_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL2_TX_IRQ_2ND_EN_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL2_TX_IRQ_EN_set     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL2_TX_IRQ_EN_get     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_COPI2_IRQ_EN_set      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_COPI2_IRQ_EN_get      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL1_RX_IRQ_2ND_EN_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL1_RX_IRQ_2ND_EN_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL1_RX_IRQ_EN_set     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL1_RX_IRQ_EN_get     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL1_TX_IRQ_2ND_EN_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL1_TX_IRQ_2ND_EN_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL1_TX_IRQ_EN_set     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL1_TX_IRQ_EN_get     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_COPI1_IRQ_EN_set      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_COPI1_IRQ_EN_get      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------+
 | | Register 0x0000002c FIC_SERDES_LANE_SELECT |
 | +--------------------------------------------+
 | | bit  7 R/W  FIC_P_TX_SERDES_SEL_19         |
 | | bit  6 R/W  FIC_P_RX_SERDES_SEL_19         |
 | | bit  5 R/W  FIC_P_TX_SERDES_SEL_18         |
 | | bit  4 R/W  FIC_P_RX_SERDES_SEL_18         |
 | | bit  3 R/W  FIC_W_TX_SERDES_SEL_19         |
 | | bit  2 R/W  FIC_W_RX_SERDES_SEL_19         |
 | | bit  1 R/W  FIC_W_TX_SERDES_SEL_18         |
 | | bit  0 R/W  FIC_W_RX_SERDES_SEL_18         |
 | +--------------------------------------------+
 | 
 | void   sifd_dft_field_FIC_P_TX_SERDES_SEL_19_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_FIC_P_TX_SERDES_SEL_19_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_FIC_P_RX_SERDES_SEL_19_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_FIC_P_RX_SERDES_SEL_19_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_FIC_P_TX_SERDES_SEL_18_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_FIC_P_TX_SERDES_SEL_18_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_FIC_P_RX_SERDES_SEL_18_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_FIC_P_RX_SERDES_SEL_18_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_FIC_W_TX_SERDES_SEL_19_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_FIC_W_TX_SERDES_SEL_19_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_FIC_W_RX_SERDES_SEL_19_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_FIC_W_RX_SERDES_SEL_19_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_FIC_W_TX_SERDES_SEL_18_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_FIC_W_TX_SERDES_SEL_18_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_FIC_W_RX_SERDES_SEL_18_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_FIC_W_RX_SERDES_SEL_18_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000030 COPI_FIC_DPI_SEL |
 | +--------------------------------------+
 | | bit  0 R/W  COPI_FIC_DPI_SEL         |
 | +--------------------------------------+
 | 
 | void   sifd_dft_field_COPI_FIC_DPI_SEL_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_COPI_FIC_DPI_SEL_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000034 SYSTEM_N1_SERDES_ACB_MUX_SEL |
 | +--------------------------------------------------+
 | | bit  31:24 R/W  N1_SERD_ACB_SEL                  |
 | +--------------------------------------------------+
 | 
 | void   sifd_dft_field_N1_SERD_ACB_SEL_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | UINT32 sifd_dft_field_N1_SERD_ACB_SEL_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void   sifd_dft_field_range_N1_SERD_ACB_SEL_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sifd_dft_field_range_N1_SERD_ACB_SEL_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00000040 + (N) * 0x4) INTERLAKEN_1_CFGS |
 | |     N[0..23]                                        |
 | +-----------------------------------------------------+
 | | bit  5 R/W  IL1_TX_SERDES_SEL                       |
 | | bit  4 R/W  IL1_RX_SERDES_SEL                       |
 | | bit  3 R/W  IL1_DLOS_INV                            |
 | | bit  2 R/W  IL1_DLOS_EN                             |
 | | bit  1 R/W  IL1_TX_LANE_DIS                         |
 | | bit  0 R/W  IL1_RX_LANE_DIS                         |
 | +-----------------------------------------------------+
 | 
 | void   sifd_dft_field_IL1_TX_SERDES_SEL_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL1_TX_SERDES_SEL_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL1_RX_SERDES_SEL_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL1_RX_SERDES_SEL_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL1_DLOS_INV_set      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL1_DLOS_INV_get      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL1_DLOS_EN_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL1_DLOS_EN_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL1_TX_LANE_DIS_set   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL1_TX_LANE_DIS_get   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL1_RX_LANE_DIS_set   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL1_RX_LANE_DIS_get   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------------------+
 | | Register (0x000000a0 + (N) * 0x4) INTERLAKEN_2_CFGS |
 | |     N[0..23]                                        |
 | +-----------------------------------------------------+
 | | bit  5 R/W  IL2_TX_SERDES_SEL                       |
 | | bit  4 R/W  IL2_RX_SERDES_SEL                       |
 | | bit  3 R/W  IL2_DLOS_INV                            |
 | | bit  2 R/W  IL2_DLOS_EN                             |
 | | bit  1 R/W  IL2_TX_LANE_DIS                         |
 | | bit  0 R/W  IL2_RX_LANE_DIS                         |
 | +-----------------------------------------------------+
 | 
 | void   sifd_dft_field_IL2_TX_SERDES_SEL_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL2_TX_SERDES_SEL_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL2_RX_SERDES_SEL_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL2_RX_SERDES_SEL_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL2_DLOS_INV_set      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL2_DLOS_INV_get      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL2_DLOS_EN_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL2_DLOS_EN_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL2_TX_LANE_DIS_set   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL2_TX_LANE_DIS_get   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_IL2_RX_LANE_DIS_set   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_IL2_RX_LANE_DIS_get   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------+
 | | Register (0x00000100 + (N) * 0x4) DSIS_CFGS |
 | |     N[0..11]                                |
 | +---------------------------------------------+
 | | bit  6 R/W  DSIS_LOS_POL_INV                |
 | | bit  5 R/W  DSIS_DLOS_INV                   |
 | | bit  4 R/W  DSIS_DLOS_EN                    |
 | | bit  3 R/W  DSIS_PIN_LOS_INV                |
 | | bit  2 R/W  DSIS_PIN_LOS_EN                 |
 | | bit  1 R/W  DSIS_TX_LANE_DIS                |
 | | bit  0 R/W  DSIS_RX_LANE_DIS                |
 | +---------------------------------------------+
 | 
 | void   sifd_dft_field_DSIS_LOS_POL_INV_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_DSIS_LOS_POL_INV_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_DSIS_DLOS_INV_set    ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_DSIS_DLOS_INV_get    ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_DSIS_DLOS_EN_set     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_DSIS_DLOS_EN_get     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_DSIS_PIN_LOS_INV_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_DSIS_PIN_LOS_INV_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_DSIS_PIN_LOS_EN_set  ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_DSIS_PIN_LOS_EN_get  ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_DSIS_TX_LANE_DIS_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_DSIS_TX_LANE_DIS_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_DSIS_RX_LANE_DIS_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_DSIS_RX_LANE_DIS_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------+
 | | Register (0x00000130 + (N) * 0x4) LINE_OTN_CFGS |
 | |     N[0..7]                                     |
 | +-------------------------------------------------+
 | | bit  1 R/W  LOTN_TX_LANE_DIS                    |
 | | bit  0 R/W  LOTN_RX_LANE_DIS                    |
 | +-------------------------------------------------+
 | 
 | void   sifd_dft_field_LOTN_TX_LANE_DIS_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_LOTN_TX_LANE_DIS_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_LOTN_RX_LANE_DIS_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_LOTN_RX_LANE_DIS_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------+
 | | Register (0x00000150 + (N) * 0x4) FIC_WORK_CFGS |
 | |     N[0..26]                                    |
 | +-------------------------------------------------+
 | | bit  3 R/W  FIC_W_DLOS_INV                      |
 | | bit  2 R/W  FIC_W_DLOS_EN                       |
 | | bit  1 R/W  FIC_W_TX_LANE_DIS                   |
 | | bit  0 R/W  FIC_W_RX_LANE_DIS                   |
 | +-------------------------------------------------+
 | 
 | void   sifd_dft_field_FIC_W_DLOS_INV_set    ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_FIC_W_DLOS_INV_get    ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_FIC_W_DLOS_EN_set     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_FIC_W_DLOS_EN_get     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_FIC_W_TX_LANE_DIS_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_FIC_W_TX_LANE_DIS_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_FIC_W_RX_LANE_DIS_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_FIC_W_RX_LANE_DIS_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------------------+
 | | Register (0x000001bc + (N) * 0x4) FIC_PROTECT_CFGS |
 | |     N[0..26]                                       |
 | +----------------------------------------------------+
 | | bit  3 R/W  FIC_P_DLOS_INV                         |
 | | bit  2 R/W  FIC_P_DLOS_EN                          |
 | | bit  1 R/W  FIC_P_TX_LANE_DIS                      |
 | | bit  0 R/W  FIC_P_RX_LANE_DIS                      |
 | +----------------------------------------------------+
 | 
 | void   sifd_dft_field_FIC_P_DLOS_INV_set    ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_FIC_P_DLOS_INV_get    ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_FIC_P_DLOS_EN_set     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_FIC_P_DLOS_EN_get     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_FIC_P_TX_LANE_DIS_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_FIC_P_TX_LANE_DIS_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_FIC_P_RX_LANE_DIS_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_FIC_P_RX_LANE_DIS_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00000228 + (N) * 0x4) BACKPLANE_LR_E1_SERDES_CFGS |
 | |     N[0..39]                                                  |
 | +---------------------------------------------------------------+
 | | bit  25:24 R/W  E1_TX_LANE_SEL                                |
 | | bit  20    R/W  E1_TX_DESKEW_CONST_DELAY_EN                   |
 | | bit  19:16 R/W  E1_TX_NUM_DESKEW_STAGES                       |
 | | bit  12    R/W  E1_RX_DESKEW_CONST_DELAY_EN                   |
 | | bit  11:8  R/W  E1_RX_NUM_DESKEW_STAGES                       |
 | +---------------------------------------------------------------+
 | 
 | void   sifd_dft_field_E1_TX_LANE_SEL_set                ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_E1_TX_LANE_SEL_get                ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_range_E1_TX_LANE_SEL_set          ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sifd_dft_field_range_E1_TX_LANE_SEL_get          ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sifd_dft_field_E1_TX_DESKEW_CONST_DELAY_EN_set   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_E1_TX_DESKEW_CONST_DELAY_EN_get   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_E1_TX_NUM_DESKEW_STAGES_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_E1_TX_NUM_DESKEW_STAGES_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_range_E1_TX_NUM_DESKEW_STAGES_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sifd_dft_field_range_E1_TX_NUM_DESKEW_STAGES_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sifd_dft_field_E1_RX_DESKEW_CONST_DELAY_EN_set   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_E1_RX_DESKEW_CONST_DELAY_EN_get   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_E1_RX_NUM_DESKEW_STAGES_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_E1_RX_NUM_DESKEW_STAGES_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_range_E1_RX_NUM_DESKEW_STAGES_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sifd_dft_field_range_E1_RX_NUM_DESKEW_STAGES_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000002c8 + (N) * 0x4) SYSTEM_N1_SERDES_CFGS |
 | |     N[0..15]                                            |
 | +---------------------------------------------------------+
 | | bit  25:24 R/W  N1_TX_LANE_SEL                          |
 | | bit  18    R/W  N1_TX_DESKEW_CONST_DELAY_EN             |
 | | bit  17:16 R/W  N1_TX_NUM_DESKEW_STAGES                 |
 | | bit  9:8   R/W  N1_RX_NUM_DESKEW_STAGES                 |
 | +---------------------------------------------------------+
 | 
 | void   sifd_dft_field_N1_TX_LANE_SEL_set                ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_N1_TX_LANE_SEL_get                ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_range_N1_TX_LANE_SEL_set          ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sifd_dft_field_range_N1_TX_LANE_SEL_get          ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sifd_dft_field_N1_TX_DESKEW_CONST_DELAY_EN_set   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_N1_TX_DESKEW_CONST_DELAY_EN_get   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_N1_TX_NUM_DESKEW_STAGES_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_N1_TX_NUM_DESKEW_STAGES_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_range_N1_TX_NUM_DESKEW_STAGES_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sifd_dft_field_range_N1_TX_NUM_DESKEW_STAGES_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sifd_dft_field_N1_RX_NUM_DESKEW_STAGES_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_N1_RX_NUM_DESKEW_STAGES_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_range_N1_RX_NUM_DESKEW_STAGES_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sifd_dft_field_range_N1_RX_NUM_DESKEW_STAGES_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00000308 + (N) * 0x4) BACKPLANE_SR_N2_SERDES_CFGS |
 | |     N[0..7]                                                   |
 | +---------------------------------------------------------------+
 | | bit  25:24 R/W  N2_TX_LANE_SEL                                |
 | | bit  19    R/W  N2_TX_DESKEW_CONST_DELAY_EN                   |
 | | bit  18:16 R/W  N2_TX_NUM_DESKEW_STAGES                       |
 | | bit  11:8  R/W  N2_RX_NUM_DESKEW_STAGES                       |
 | +---------------------------------------------------------------+
 | 
 | void   sifd_dft_field_N2_TX_LANE_SEL_set                ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_N2_TX_LANE_SEL_get                ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_range_N2_TX_LANE_SEL_set          ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sifd_dft_field_range_N2_TX_LANE_SEL_get          ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sifd_dft_field_N2_TX_DESKEW_CONST_DELAY_EN_set   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_N2_TX_DESKEW_CONST_DELAY_EN_get   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_N2_TX_NUM_DESKEW_STAGES_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_N2_TX_NUM_DESKEW_STAGES_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_range_N2_TX_NUM_DESKEW_STAGES_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sifd_dft_field_range_N2_TX_NUM_DESKEW_STAGES_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   sifd_dft_field_N2_RX_NUM_DESKEW_STAGES_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 sifd_dft_field_N2_RX_NUM_DESKEW_STAGES_get       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | void   sifd_dft_field_range_N2_RX_NUM_DESKEW_STAGES_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 sifd_dft_field_range_N2_RX_NUM_DESKEW_STAGES_get ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _SIFD_DFT_IO_H
#define _SIFD_DFT_IO_H

#include "sifd_api.h"
#include "sifd_dft_regs.h"
#include "sifd_dft_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 sifd_dft_buffer_init               ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | void                 sifd_dft_buffer_flush              ( sifd_dft_buffer_t *b_ptr )
 | UINT32               sifd_dft_reg_read                  ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 sifd_dft_reg_write                 ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 sifd_dft_field_set                 ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 sifd_dft_action_on_write_field_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 sifd_dft_burst_read                ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 sifd_dft_burst_write               ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE sifd_dft_poll                      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   sifd_dft_reg_SOFT_RESET_write                            ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void   sifd_dft_reg_SOFT_RESET_field_set                        ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_SOFT_RESET_read                             ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void   sifd_dft_reg_LOW_POWER_CFG_write                         ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void   sifd_dft_reg_LOW_POWER_CFG_field_set                     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_LOW_POWER_CFG_read                          ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void   sifd_dft_reg_RAM_LOW_POWER_CFG_write                     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void   sifd_dft_reg_RAM_LOW_POWER_CFG_field_set                 ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_RAM_LOW_POWER_CFG_read                      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void   sifd_dft_reg_FIC_SERDES_LANE_SELECT_write                ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void   sifd_dft_reg_FIC_SERDES_LANE_SELECT_field_set            ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_FIC_SERDES_LANE_SELECT_read                 ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void   sifd_dft_reg_COPI_FIC_DPI_SEL_write                      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void   sifd_dft_reg_COPI_FIC_DPI_SEL_field_set                  ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_COPI_FIC_DPI_SEL_read                       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void   sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_write          ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 value )
 | void   sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_field_set      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_SYSTEM_N1_SERDES_ACB_MUX_SEL_read           ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 | void   sifd_dft_reg_DSIS_CFGS_array_write                       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   sifd_dft_reg_DSIS_CFGS_array_field_set                   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_DSIS_CFGS_array_read                        ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void   sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_write           ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_field_set       ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_SYSTEM_N1_SERDES_CFGS_array_read            ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void   sifd_dft_reg_INTERLAKEN_1_CFGS_array_write               ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   sifd_dft_reg_INTERLAKEN_1_CFGS_array_field_set           ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_INTERLAKEN_1_CFGS_array_read                ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void   sifd_dft_reg_INTERLAKEN_2_CFGS_array_write               ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   sifd_dft_reg_INTERLAKEN_2_CFGS_array_field_set           ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_INTERLAKEN_2_CFGS_array_read                ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void   sifd_dft_reg_FIC_WORK_CFGS_array_write                   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   sifd_dft_reg_FIC_WORK_CFGS_array_field_set               ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_FIC_WORK_CFGS_array_read                    ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void   sifd_dft_reg_FIC_PROTECT_CFGS_array_write                ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   sifd_dft_reg_FIC_PROTECT_CFGS_array_field_set            ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_FIC_PROTECT_CFGS_array_read                 ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void   sifd_dft_reg_SIFD_INT_EN_array_write                     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   sifd_dft_reg_SIFD_INT_EN_array_field_set                 ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_SIFD_INT_EN_array_read                      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void   sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_write     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_field_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_BACKPLANE_LR_E1_SERDES_CFGS_array_read      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void   sifd_dft_reg_LINE_OTN_CFGS_array_write                   ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   sifd_dft_reg_LINE_OTN_CFGS_array_field_set               ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_LINE_OTN_CFGS_array_read                    ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | void   sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_write     ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void   sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_field_set ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 sifd_dft_reg_BACKPLANE_SR_N2_SERDES_CFGS_array_read      ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32 sifd_dft_reg_SIFD_SUBSYSTEM_INT_STAT_read                ( sifd_dft_buffer_t *b_ptr, sifd_handle_t *h_ptr )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _SIFD_DFT_IO_H */
