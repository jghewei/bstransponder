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
 *     and register accessor functions for the coreotn_ctl block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing coreotn_ctl io functions is:
 | {
 |     coreotn_ctl_buffer_t b_ptr[1];
 |     coreotn_ctl_buffer_init( b_ptr, h_ptr );
 |     value = coreotn_ctl_field_<FIELD>_get( b_ptr, h_ptr );
 |     coreotn_ctl_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     coreotn_ctl_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = coreotn_ctl_field_<FIELD>_get( NULL, h_ptr );
 | coreotn_ctl_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------------+
 | | Register 0x002ac000 SW_RESET_REG     |
 | +--------------------------------------+
 | | bit  7 R/W  ECC_FRM_MICROPROC_RESETN |
 | | bit  6 R/W  ECC_FRM_RSTB             |
 | | bit  5 R/W  ECC_FRM_S_RSTB           |
 | | bit  4 R/W  MPMO_RST                 |
 | | bit  3 R/W  MPMA_RST                 |
 | | bit  2 R/W  OHP3_RST                 |
 | | bit  1 R/W  OHP2_RST                 |
 | | bit  0 R/W  OHP1_RST                 |
 | +--------------------------------------+
 | 
 | void   coreotn_ctl_field_ECC_FRM_MICROPROC_RESETN_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_ECC_FRM_MICROPROC_RESETN_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_ECC_FRM_RSTB_set             ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_ECC_FRM_RSTB_get             ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_ECC_FRM_S_RSTB_set           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_ECC_FRM_S_RSTB_get           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_MPMO_RST_set                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_MPMO_RST_get                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_MPMA_RST_set                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_MPMA_RST_get                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_OHP3_RST_set                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_OHP3_RST_get                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_OHP2_RST_set                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_OHP2_RST_get                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_OHP1_RST_set                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_OHP1_RST_get                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x002ac004 CLK_GATING_REG |
 | +------------------------------------+
 | | bit  5 R/W  ECC_FRM_LOWPWR         |
 | | bit  4 R/W  MPMO_LOWPWR            |
 | | bit  3 R/W  MPMA_LOWPWR            |
 | | bit  2 R/W  OHP3_LOWPWR            |
 | | bit  1 R/W  OHP2_LOWPWR            |
 | | bit  0 R/W  OHP1_LOWPWR            |
 | +------------------------------------+
 | 
 | void   coreotn_ctl_field_ECC_FRM_LOWPWR_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_ECC_FRM_LOWPWR_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_MPMO_LOWPWR_set    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_MPMO_LOWPWR_get    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_MPMA_LOWPWR_set    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_MPMA_LOWPWR_get    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_OHP3_LOWPWR_set    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_OHP3_LOWPWR_get    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_OHP2_LOWPWR_set    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_OHP2_LOWPWR_get    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_OHP1_LOWPWR_set    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_OHP1_LOWPWR_get    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x002ac008 RAM_LOW_POWER_REG |
 | +---------------------------------------+
 | | bit  5 R/W  ECC_FRM_RAM_LOWPWR        |
 | | bit  4 R/W  MPMO_RAM_LOWPWR           |
 | | bit  3 R/W  MPMA_RAM_LOWPWR           |
 | | bit  2 R/W  OHP3_RAM_LOWPWR           |
 | | bit  1 R/W  OHP2_RAM_LOWPWR           |
 | | bit  0 R/W  OHP1_RAM_LOWPWR           |
 | +---------------------------------------+
 | 
 | void   coreotn_ctl_field_ECC_FRM_RAM_LOWPWR_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_ECC_FRM_RAM_LOWPWR_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_MPMO_RAM_LOWPWR_set    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_MPMO_RAM_LOWPWR_get    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_MPMA_RAM_LOWPWR_set    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_MPMA_RAM_LOWPWR_get    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_OHP3_RAM_LOWPWR_set    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_OHP3_RAM_LOWPWR_get    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_OHP2_RAM_LOWPWR_set    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_OHP2_RAM_LOWPWR_get    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_OHP1_RAM_LOWPWR_set    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_OHP1_RAM_LOWPWR_get    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x002ac020 INT_STAT_REG_1 |
 | +------------------------------------+
 | | bit  31:0 R  INT_STAT              |
 | +------------------------------------+
 | 
 | void   coreotn_ctl_lfield_INT_STAT_get       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[2] )
 | UINT32 coreotn_ctl_lfield_range_INT_STAT_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x002ac028 LOWER_INT_EN_REG_1 |
 | +----------------------------------------+
 | | bit  31:0 R/W  INT_EN_1                |
 | +----------------------------------------+
 | 
 | void   coreotn_ctl_lfield_INT_EN_1_set       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[2] )
 | void   coreotn_ctl_lfield_INT_EN_1_get       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[2] )
 | void   coreotn_ctl_lfield_range_INT_EN_1_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_ctl_lfield_range_INT_EN_1_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x002ac030 LOWER_INT_EN_REG_2 |
 | +----------------------------------------+
 | | bit  31:0 R/W  INT_EN_2                |
 | +----------------------------------------+
 | 
 | void   coreotn_ctl_lfield_INT_EN_2_set       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[2] )
 | void   coreotn_ctl_lfield_INT_EN_2_get       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[2] )
 | void   coreotn_ctl_lfield_range_INT_EN_2_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_ctl_lfield_range_INT_EN_2_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x002ac038 LOWER_INT_EN_REG_3 |
 | +----------------------------------------+
 | | bit  31:0 R/W  INT_EN_3                |
 | +----------------------------------------+
 | 
 | void   coreotn_ctl_lfield_INT_EN_3_set       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[2] )
 | void   coreotn_ctl_lfield_INT_EN_3_get       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[2] )
 | void   coreotn_ctl_lfield_range_INT_EN_3_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_ctl_lfield_range_INT_EN_3_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x002ac040 LOWER_INT_EN_REG_4 |
 | +----------------------------------------+
 | | bit  31:0 R/W  INT_EN_4                |
 | +----------------------------------------+
 | 
 | void   coreotn_ctl_lfield_INT_EN_4_set       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[2] )
 | void   coreotn_ctl_lfield_INT_EN_4_get       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value[2] )
 | void   coreotn_ctl_lfield_range_INT_EN_4_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_ctl_lfield_range_INT_EN_4_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x002ac048 DPI_BUS_CONFIG_REG |
 | +----------------------------------------+
 | | bit  4 R/W  SIFD1_SEL_CTRL             |
 | | bit  0 R/W  SIFD1_MUX_CTRL             |
 | +----------------------------------------+
 | 
 | void   coreotn_ctl_field_SIFD1_SEL_CTRL_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_SIFD1_SEL_CTRL_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_SIFD1_MUX_CTRL_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_SIFD1_MUX_CTRL_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x002ac04c CKCTL_CONFIG_REG |
 | +--------------------------------------+
 | | bit  0 R/W  CKCTL_CTRL               |
 | +--------------------------------------+
 | 
 | void   coreotn_ctl_field_CKCTL_CTRL_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_CKCTL_CTRL_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x002ac050 OHP_CONFIG_REG |
 | +------------------------------------+
 | | bit  31:24 R/W  OHP3_SEL_B         |
 | | bit  23:16 R/W  OHP3_SEL_A         |
 | | bit  15:8  R/W  OHP2_SEL_B         |
 | | bit  7:0   R/W  OHP2_SEL_A         |
 | +------------------------------------+
 | 
 | void   coreotn_ctl_field_OHP3_SEL_B_set       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_OHP3_SEL_B_get       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_range_OHP3_SEL_B_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_ctl_field_range_OHP3_SEL_B_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   coreotn_ctl_field_OHP3_SEL_A_set       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_OHP3_SEL_A_get       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_range_OHP3_SEL_A_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_ctl_field_range_OHP3_SEL_A_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   coreotn_ctl_field_OHP2_SEL_B_set       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_OHP2_SEL_B_get       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_range_OHP2_SEL_B_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_ctl_field_range_OHP2_SEL_B_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   coreotn_ctl_field_OHP2_SEL_A_set       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_OHP2_SEL_A_get       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_range_OHP2_SEL_A_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_ctl_field_range_OHP2_SEL_A_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x002ac054 ECC_FRM_PROCESSOR_CONFIG_REG |
 | +--------------------------------------------------+
 | | bit  22:16 R/W  SLV_OP_MAX_CNT                   |
 | | bit  14:0  R/W  MP_BLOCK_MAX_CNT                 |
 | +--------------------------------------------------+
 | 
 | void   coreotn_ctl_field_SLV_OP_MAX_CNT_set         ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_SLV_OP_MAX_CNT_get         ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_range_SLV_OP_MAX_CNT_set   ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_ctl_field_range_SLV_OP_MAX_CNT_get   ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   coreotn_ctl_field_MP_BLOCK_MAX_CNT_set       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_MP_BLOCK_MAX_CNT_get       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_range_MP_BLOCK_MAX_CNT_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_ctl_field_range_MP_BLOCK_MAX_CNT_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x002ac058 ECC_FRM_GCC_CONFIG_REG |
 | +--------------------------------------------+
 | | bit  12 R/W  TX_GCC_BYTE_SWAP              |
 | | bit  8  R/W  TX_GCC_BIT_SWAP               |
 | | bit  4  R/W  RX_GCC_BYTE_SWAP              |
 | | bit  0  R/W  RX_GCC_BIT_SWAP               |
 | +--------------------------------------------+
 | 
 | void   coreotn_ctl_field_TX_GCC_BYTE_SWAP_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_TX_GCC_BYTE_SWAP_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_TX_GCC_BIT_SWAP_set  ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_TX_GCC_BIT_SWAP_get  ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_RX_GCC_BYTE_SWAP_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_RX_GCC_BYTE_SWAP_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_RX_GCC_BIT_SWAP_set  ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_RX_GCC_BIT_SWAP_get  ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +--------------------------------------------+
 | | Register 0x002ac05c ECC_FRM_FIFO_RESET_REG |
 | +--------------------------------------------+
 | | bit  4 R/W  ECC_FRM_FIFO_TX_RSTB           |
 | | bit  0 R/W  ECC_FRM_FIFO_RX_RSTB           |
 | +--------------------------------------------+
 | 
 | void   coreotn_ctl_field_ECC_FRM_FIFO_TX_RSTB_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_RSTB_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void   coreotn_ctl_field_ECC_FRM_FIFO_RX_RSTB_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_RSTB_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | +----------------------------------------------+
 | | Register 0x002ac060 ECC_FRM_FIFO_RX_STAT_REG |
 | +----------------------------------------------+
 | | bit  29:24 R  ECC_FRM_FIFO_RX_EMPTY          |
 | | bit  21:16 R  ECC_FRM_FIFO_RX_FULL           |
 | | bit  13:8  R  ECC_FRM_FIFO_RX_UNDERFLOW      |
 | | bit  5:0   R  ECC_FRM_FIFO_RX_OVERFLOW       |
 | +----------------------------------------------+
 | 
 | UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_EMPTY_get           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_RX_EMPTY_get     ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_FULL_get            ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_RX_FULL_get      ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_UNDERFLOW_get       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_RX_UNDERFLOW_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 coreotn_ctl_field_ECC_FRM_FIFO_RX_OVERFLOW_get        ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_RX_OVERFLOW_get  ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x002ac064 ECC_FRM_FIFO_TX_STAT_REG |
 | +----------------------------------------------+
 | | bit  29:24 R  ECC_FRM_FIFO_TX_EMPTY          |
 | | bit  21:16 R  ECC_FRM_FIFO_TX_FULL           |
 | | bit  13:8  R  ECC_FRM_FIFO_TX_UNDERFLOW      |
 | | bit  5:0   R  ECC_FRM_FIFO_TX_OVERFLOW       |
 | +----------------------------------------------+
 | 
 | UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_EMPTY_get           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_TX_EMPTY_get     ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_FULL_get            ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_TX_FULL_get      ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_UNDERFLOW_get       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_TX_UNDERFLOW_get ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 coreotn_ctl_field_ECC_FRM_FIFO_TX_OVERFLOW_get        ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | UINT32 coreotn_ctl_field_range_ECC_FRM_FIFO_TX_OVERFLOW_get  ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _COREOTN_CTL_IO_H
#define _COREOTN_CTL_IO_H

#include "coreotn_loc.h"
#include "coreotn_ctl_regs.h"
#include "coreotn_ctl_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 coreotn_ctl_buffer_init               ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | void                 coreotn_ctl_buffer_flush              ( coreotn_ctl_buffer_t *b_ptr )
 | UINT32               coreotn_ctl_reg_read                  ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 coreotn_ctl_reg_write                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 coreotn_ctl_field_set                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 coreotn_ctl_action_on_write_field_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 coreotn_ctl_burst_read                ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 coreotn_ctl_burst_write               ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE coreotn_ctl_poll                      ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   coreotn_ctl_reg_SW_RESET_REG_write                     ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_SW_RESET_REG_field_set                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_SW_RESET_REG_read                      ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_CLK_GATING_REG_write                   ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_CLK_GATING_REG_field_set               ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_CLK_GATING_REG_read                    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_RAM_LOW_POWER_REG_write                ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_RAM_LOW_POWER_REG_field_set            ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_RAM_LOW_POWER_REG_read                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_LOWER_INT_EN_REG_1_write               ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_LOWER_INT_EN_REG_1_field_set           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_LOWER_INT_EN_REG_1_read                ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_UPPER_INT_EN_REG_1_write               ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_UPPER_INT_EN_REG_1_field_set           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_UPPER_INT_EN_REG_1_read                ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_LOWER_INT_EN_REG_2_write               ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_LOWER_INT_EN_REG_2_field_set           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_LOWER_INT_EN_REG_2_read                ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_UPPER_INT_EN_REG_2_write               ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_UPPER_INT_EN_REG_2_field_set           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_UPPER_INT_EN_REG_2_read                ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_LOWER_INT_EN_REG_3_write               ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_LOWER_INT_EN_REG_3_field_set           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_LOWER_INT_EN_REG_3_read                ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_UPPER_INT_EN_REG_3_write               ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_UPPER_INT_EN_REG_3_field_set           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_UPPER_INT_EN_REG_3_read                ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_LOWER_INT_EN_REG_4_write               ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_LOWER_INT_EN_REG_4_field_set           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_LOWER_INT_EN_REG_4_read                ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_UPPER_INT_EN_REG_4_write               ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_UPPER_INT_EN_REG_4_field_set           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_UPPER_INT_EN_REG_4_read                ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_DPI_BUS_CONFIG_REG_write               ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_DPI_BUS_CONFIG_REG_field_set           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_DPI_BUS_CONFIG_REG_read                ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_CKCTL_CONFIG_REG_write                 ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_CKCTL_CONFIG_REG_field_set             ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_CKCTL_CONFIG_REG_read                  ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_OHP_CONFIG_REG_write                   ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_OHP_CONFIG_REG_field_set               ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_OHP_CONFIG_REG_read                    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_write     ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_field_set ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_ECC_FRM_PROCESSOR_CONFIG_REG_read      ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_write           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_field_set       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_ECC_FRM_GCC_CONFIG_REG_read            ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | void   coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_write           ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 value )
 | void   coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_field_set       ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 coreotn_ctl_reg_ECC_FRM_FIFO_RESET_REG_read            ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32 coreotn_ctl_reg_INT_STAT_REG_1_read                    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32 coreotn_ctl_reg_INT_STAT_REG_2_read                    ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32 coreotn_ctl_reg_ECC_FRM_FIFO_RX_STAT_REG_read          ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 | UINT32 coreotn_ctl_reg_ECC_FRM_FIFO_TX_STAT_REG_read          ( coreotn_ctl_buffer_t *b_ptr, coreotn_handle_t *h_ptr )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _COREOTN_CTL_IO_H */
