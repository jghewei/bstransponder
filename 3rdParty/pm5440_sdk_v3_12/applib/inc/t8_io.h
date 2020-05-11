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
 *     and register accessor functions for the t8 block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing t8 io functions is:
 | {
 |     t8_buffer_t b_ptr[1];
 |     t8_buffer_init( b_ptr, h_ptr, id );
 |     value = t8_field_<FIELD>_get( b_ptr, h_ptr, id );
 |     t8_field_<FIELD>_set( b_ptr, h_ptr, id , value );
 | ...
 |     t8_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = t8_field_<FIELD>_get( NULL, h_ptr, id );
 | t8_field_<FIELD>_set( NULL, h_ptr, id , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------------+
 | | Register 0x00000000 PCIE3_CFG_1 |
 | +---------------------------------+
 | | bit  26    R/W  TXRXCS_ATMSB    |
 | | bit  25:22 R/W  TXRXCS_MODE     |
 | | bit  21:18 R/W  TXRXCS_CTRL     |
 | | bit  17    R/W  PISO_ATMSB      |
 | | bit  10    R/W  PISO_ALTPAT_EN  |
 | | bit  9     R/W  PISO_ENB        |
 | | bit  0     R/W  PISO_DLB_EN     |
 | +---------------------------------+
 | 
 | void   t8_field_TXRXCS_ATMSB_set      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TXRXCS_ATMSB_get      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_TXRXCS_MODE_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TXRXCS_MODE_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_TXRXCS_MODE_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_TXRXCS_MODE_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_TXRXCS_CTRL_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TXRXCS_CTRL_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_TXRXCS_CTRL_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_TXRXCS_CTRL_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_PISO_ATMSB_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PISO_ATMSB_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PISO_ALTPAT_EN_set    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PISO_ALTPAT_EN_get    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PISO_ENB_set          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PISO_ENB_get          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PISO_DLB_EN_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PISO_DLB_EN_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +---------------------------------+
 | | Register 0x00000004 PCIE3_CFG_2 |
 | +---------------------------------+
 | | bit  25    R/W  BADJ_ENB        |
 | | bit  24    R/W  TX_TEST_SEL     |
 | | bit  23    R/W  TX_TEST_EN      |
 | | bit  22:20 R/W  ADC_ENB         |
 | | bit  19:12 R/W  ADC_MODE        |
 | | bit  11    R/W  ADC_BIST        |
 | | bit  10    R/W  ADC_DLB_EN      |
 | | bit  9     R/W  ADC_DAC_ENB     |
 | | bit  8     R/W  RXCM_ENB        |
 | | bit  5     R/W  ADC_ATMSB       |
 | | bit  3     R/W  RX_ATMSB        |
 | +---------------------------------+
 | 
 | void   t8_field_BADJ_ENB_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_BADJ_ENB_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_TX_TEST_SEL_set    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TX_TEST_SEL_get    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_TX_TEST_EN_set     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TX_TEST_EN_get     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_ADC_ENB_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADC_ENB_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ADC_ENB_set  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ADC_ENB_get  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_ADC_MODE_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADC_MODE_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ADC_MODE_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ADC_MODE_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_ADC_BIST_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADC_BIST_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_ADC_DLB_EN_set     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADC_DLB_EN_get     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_ADC_DAC_ENB_set    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADC_DAC_ENB_get    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_RXCM_ENB_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_RXCM_ENB_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_ADC_ATMSB_set      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADC_ATMSB_get      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_RX_ATMSB_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_RX_ATMSB_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +-------------------------------------------------+
 | | Register 0x00000008 ANALOG_SIPO_BIST_STATUS_REG |
 | +-------------------------------------------------+
 | | bit  1 R  BIST_SYNC                             |
 | | bit  0 R  BIST_ERR                              |
 | +-------------------------------------------------+
 | 
 | UINT32 t8_field_BIST_SYNC_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_field_BIST_ERR_get  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +-----------------------------------+
 | | Register 0x0000000c TX_CNTR_CFG_1 |
 | +-----------------------------------+
 | | bit  26 R/W  CM_MODN_B            |
 | | bit  25 R/W  CM_MODP_B            |
 | | bit  24 R/W  TX_ATMSB             |
 | +-----------------------------------+
 | 
 | void   t8_field_CM_MODN_B_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CM_MODN_B_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_CM_MODP_B_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CM_MODP_B_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_TX_ATMSB_set  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TX_ATMSB_get  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +-----------------------------------+
 | | Register 0x00000010 TX_CNTR_CFG_2 |
 | +-----------------------------------+
 | | bit  31:30 R/W  PISO_PRE2_MODE    |
 | | bit  25    R/W  PISO_PRE2_ENABLE  |
 | | bit  24    R/W  PISO_PRE1_ENABLE  |
 | +-----------------------------------+
 | 
 | void   t8_field_PISO_PRE2_MODE_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PISO_PRE2_MODE_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PISO_PRE2_MODE_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PISO_PRE2_MODE_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_PISO_PRE2_ENABLE_set     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PISO_PRE2_ENABLE_get     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PISO_PRE1_ENABLE_set     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PISO_PRE1_ENABLE_get     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +-----------------------------------+
 | | Register 0x00000014 TX_CNTR_CFG_3 |
 | +-----------------------------------+
 | | bit  21:17 R/W  TX_DER_CORE       |
 | +-----------------------------------+
 | 
 | void   t8_field_TX_DER_CORE_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TX_DER_CORE_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_TX_DER_CORE_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_TX_DER_CORE_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000018 TX_CNTR_CFG_4 |
 | +-----------------------------------+
 | | bit  26 R/W  TX_AMP_EMPH_EN       |
 | +-----------------------------------+
 | 
 | void   t8_field_TX_AMP_EMPH_EN_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TX_AMP_EMPH_EN_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +---------------------------------------+
 | | Register 0x0000001c TX_CNTR_CFG_5     |
 | +---------------------------------------+
 | | bit  31   R/W  RXDET_CANCEL_OVERRIDE  |
 | | bit  17   R/W  TX_CTRL_CMD_EN         |
 | | bit  16   R/W  TX_EDGE_DELAY_SEL_CORE |
 | | bit  15:0 R/W  TX_CTRL_EDGE_CORE      |
 | +---------------------------------------+
 | 
 | void   t8_field_RXDET_CANCEL_OVERRIDE_set   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_RXDET_CANCEL_OVERRIDE_get   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_TX_CTRL_CMD_EN_set          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TX_CTRL_CMD_EN_get          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_TX_EDGE_DELAY_SEL_CORE_set  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TX_EDGE_DELAY_SEL_CORE_get  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_TX_CTRL_EDGE_CORE_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TX_CTRL_EDGE_CORE_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_TX_CTRL_EDGE_CORE_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_TX_CTRL_EDGE_CORE_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000020 TX_CNTR_CFG_6 |
 | +-----------------------------------+
 | | bit  15:0 R/W  TX_COUNTER         |
 | +-----------------------------------+
 | 
 | void   t8_field_TX_COUNTER_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TX_COUNTER_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_TX_COUNTER_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_TX_COUNTER_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000024 TX_CNTR_STA |
 | +---------------------------------+
 | | bit  15:0 R  TX_CODE_COUNT      |
 | +---------------------------------+
 | 
 | UINT32 t8_field_TX_CODE_COUNT_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_field_range_TX_CODE_COUNT_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00000028 DIG_LPBK_CFG |
 | +----------------------------------+
 | | bit  7 R/W  RX_TO_TX_LPBK_EN     |
 | | bit  3 R/W  TX_TO_RX_LPBK_EN     |
 | +----------------------------------+
 | 
 | void   t8_field_RX_TO_TX_LPBK_EN_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_RX_TO_TX_LPBK_EN_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_TX_TO_RX_LPBK_EN_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TX_TO_RX_LPBK_EN_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +----------------------------------------------+
 | | Register 0x0000002c ANALOG_SIPO_BIST_CFG_REG |
 | +----------------------------------------------+
 | | bit  6   R/W  TIME_REVERSE                   |
 | | bit  3:1 R/W  A_SIPO_BIST_SEL                |
 | | bit  0   R/W  A_SIPO_BIST_EN                 |
 | +----------------------------------------------+
 | 
 | void   t8_field_TIME_REVERSE_set          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TIME_REVERSE_get          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_A_SIPO_BIST_SEL_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_A_SIPO_BIST_SEL_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_A_SIPO_BIST_SEL_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_A_SIPO_BIST_SEL_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_A_SIPO_BIST_EN_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_A_SIPO_BIST_EN_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +------------------------------------+
 | | Register 0x00000030 OFFSET_RAM_CFG |
 | +------------------------------------+
 | | bit  6   R/W  GCOC_RAM_LS          |
 | | bit  4   R/W  GCOC_RAM_SD          |
 | | bit  3   R/W  GCOC_RAM_DS          |
 | | bit  2:1 R/W  GCOC_RAM_ERR_INJ     |
 | +------------------------------------+
 | 
 | void   t8_field_GCOC_RAM_LS_set            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_GCOC_RAM_LS_get            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_GCOC_RAM_SD_set            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_GCOC_RAM_SD_get            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_GCOC_RAM_DS_set            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_GCOC_RAM_DS_get            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_GCOC_RAM_ERR_INJ_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_GCOC_RAM_ERR_INJ_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_GCOC_RAM_ERR_INJ_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_GCOC_RAM_ERR_INJ_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000034 PLL_CNFG_REG1 |
 | +-----------------------------------+
 | | bit  15:12 R/W  LOCK_PRECISION    |
 | | bit  4     R/W  LOAD_CLK_CNTRS    |
 | | bit  2     R/W  RESET_HISTORY     |
 | | bit  0     R/W  PPM_OFF_RD_REQ    |
 | +-----------------------------------+
 | 
 | void   t8_field_LOCK_PRECISION_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_LOCK_PRECISION_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_LOCK_PRECISION_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_LOCK_PRECISION_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_LOAD_CLK_CNTRS_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_LOAD_CLK_CNTRS_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_RESET_HISTORY_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_RESET_HISTORY_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PPM_OFF_RD_REQ_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PPM_OFF_RD_REQ_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +--------------------------------------+
 | | Register 0x00000038 ADC_OC_CFG_1     |
 | +--------------------------------------+
 | | bit  31    R/W  ADCOC_BIST_EN        |
 | | bit  30    R/W  ADCOC_RD_OVR         |
 | | bit  29    R/W  ADCOC_OFFSET_EN      |
 | | bit  28    R/W  ADCOC_CALIB_PROG_EN  |
 | | bit  27:24 R/W  ADCOC_ACC_DELAY      |
 | | bit  23:14 R/W  ADCOC_DAC_DELAY      |
 | | bit  8:0   R/W  ADCOC_N_ACCUM_CYCLES |
 | +--------------------------------------+
 | 
 | void   t8_field_ADCOC_BIST_EN_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADCOC_BIST_EN_get              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_ADCOC_RD_OVR_set               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADCOC_RD_OVR_get               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_ADCOC_OFFSET_EN_set            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADCOC_OFFSET_EN_get            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_ADCOC_CALIB_PROG_EN_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADCOC_CALIB_PROG_EN_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_ADCOC_ACC_DELAY_set            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADCOC_ACC_DELAY_get            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ADCOC_ACC_DELAY_set      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ADCOC_ACC_DELAY_get      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_ADCOC_DAC_DELAY_set            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADCOC_DAC_DELAY_get            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ADCOC_DAC_DELAY_set      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ADCOC_DAC_DELAY_get      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_ADCOC_N_ACCUM_CYCLES_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADCOC_N_ACCUM_CYCLES_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ADCOC_N_ACCUM_CYCLES_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ADCOC_N_ACCUM_CYCLES_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x0000003c ADC_OC_CFG_2      |
 | +---------------------------------------+
 | | bit  31:28 R/W  ADCOC_LINEAR_STEP     |
 | | bit  27:18 R/W  ADCOC_CAL_DELAY       |
 | | bit  17    R/W  ADCOC_MABC_INT_OVR_EN |
 | | bit  16    R/W  ADCOC_ADC_WR_OVR      |
 | | bit  15:8  R/W  ADCOC_ADC_A_OVR       |
 | | bit  7:0   R/W  ADCOC_ADC_D_OVR       |
 | +---------------------------------------+
 | 
 | void   t8_field_ADCOC_LINEAR_STEP_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADCOC_LINEAR_STEP_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ADCOC_LINEAR_STEP_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ADCOC_LINEAR_STEP_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_ADCOC_CAL_DELAY_set         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADCOC_CAL_DELAY_get         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ADCOC_CAL_DELAY_set   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ADCOC_CAL_DELAY_get   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_ADCOC_MABC_INT_OVR_EN_set   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADCOC_MABC_INT_OVR_EN_get   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_ADCOC_ADC_WR_OVR_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADCOC_ADC_WR_OVR_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_ADCOC_ADC_A_OVR_set         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADCOC_ADC_A_OVR_get         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ADCOC_ADC_A_OVR_set   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ADCOC_ADC_A_OVR_get   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_ADCOC_ADC_D_OVR_set         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADCOC_ADC_D_OVR_get         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ADCOC_ADC_D_OVR_set   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ADCOC_ADC_D_OVR_get   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00000040 ADC_OC_REGISTER3 |
 | +--------------------------------------+
 | | bit  31:0 R/W  THR_MAP31_0           |
 | +--------------------------------------+
 | 
 | void   t8_field_THR_MAP31_0_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_THR_MAP31_0_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_THR_MAP31_0_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_THR_MAP31_0_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000044 ADC_OC_STS  |
 | +---------------------------------+
 | | bit  9:2 R  ADC_Q               |
 | | bit  1   R  ADCOC_PROG_REF_DONE |
 | | bit  0   R  ADCOC_CALIB_DONE    |
 | +---------------------------------+
 | 
 | UINT32 t8_field_ADC_Q_get               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_field_range_ADC_Q_get         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_field_ADCOC_PROG_REF_DONE_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_field_ADCOC_CALIB_DONE_get    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +----------------------------------------+
 | | Register 0x00000048 PMM_WRAP_CNFG_REG1 |
 | +----------------------------------------+
 | | bit  29:16 R/W  DSM_DENOM              |
 | | bit  1:0   R/W  DSM_TYPE               |
 | +----------------------------------------+
 | 
 | void   t8_field_DSM_DENOM_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_DSM_DENOM_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_DSM_DENOM_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_DSM_DENOM_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_DSM_TYPE_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_DSM_TYPE_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_DSM_TYPE_set  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_DSM_TYPE_get  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0000004c PMM_WRAP_CNFG_REG2 |
 | +----------------------------------------+
 | | bit  31    R/W  PMM_WRAP_EN            |
 | | bit  24:16 R/W  PKT_SIZE               |
 | | bit  15:0  R/W  PKT_PERIOD             |
 | +----------------------------------------+
 | 
 | void   t8_field_PMM_WRAP_EN_set      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PMM_WRAP_EN_get      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PKT_SIZE_set         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PKT_SIZE_get         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PKT_SIZE_set   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PKT_SIZE_get   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_PKT_PERIOD_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PKT_PERIOD_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PKT_PERIOD_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PKT_PERIOD_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000050 SIPO_CNFG_REG |
 | +-----------------------------------+
 | | bit  2:1 R/W  SIPO_DIV_RATIO      |
 | | bit  0   R/W  SIPO_BUS_WIDTH      |
 | +-----------------------------------+
 | 
 | void   t8_field_SIPO_DIV_RATIO_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_SIPO_DIV_RATIO_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_SIPO_DIV_RATIO_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_SIPO_DIV_RATIO_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_SIPO_BUS_WIDTH_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_SIPO_BUS_WIDTH_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +-----------------------------------+
 | | Register 0x00000054 PISO_CNFG_REG |
 | +-----------------------------------+
 | | bit  2:1 R/W  PISO_DIV_RATIO      |
 | | bit  0   R/W  PISO_BUS_WIDTH      |
 | +-----------------------------------+
 | 
 | void   t8_field_PISO_DIV_RATIO_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PISO_DIV_RATIO_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PISO_DIV_RATIO_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PISO_DIV_RATIO_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_PISO_BUS_WIDTH_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PISO_BUS_WIDTH_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +-----------------------------------------+
 | | Register 0x00000058 TX_CNTRL_OVERRIDE_1 |
 | +-----------------------------------------+
 | | bit  31:26 R/W  PCBI_TX_DPENI1          |
 | | bit  25:20 R/W  PCBI_TX_DPENI2          |
 | +-----------------------------------------+
 | 
 | void   t8_field_PCBI_TX_DPENI1_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PCBI_TX_DPENI1_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PCBI_TX_DPENI1_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PCBI_TX_DPENI1_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_PCBI_TX_DPENI2_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PCBI_TX_DPENI2_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PCBI_TX_DPENI2_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PCBI_TX_DPENI2_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000068 TX_CNTRL_OVERRIDE_REG_5 |
 | +---------------------------------------------+
 | | bit  19 R/W  TX_WIZARD_EN                   |
 | +---------------------------------------------+
 | 
 | void   t8_field_TX_WIZARD_EN_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TX_WIZARD_EN_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +----------------------------------+
 | | Register 0x0000006c CLK_DIV_FACT |
 | +----------------------------------+
 | | bit  31:25 R/W  CM_CLK_DIV_FACT  |
 | | bit  23:20 R/W  ADC_CLK_DIV_FACT |
 | | bit  19:16 R/W  CLK800_DIV_FACT  |
 | +----------------------------------+
 | 
 | void   t8_field_CM_CLK_DIV_FACT_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CM_CLK_DIV_FACT_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_CM_CLK_DIV_FACT_set  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_CM_CLK_DIV_FACT_get  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_ADC_CLK_DIV_FACT_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADC_CLK_DIV_FACT_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ADC_CLK_DIV_FACT_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ADC_CLK_DIV_FACT_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_CLK800_DIV_FACT_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CLK800_DIV_FACT_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_CLK800_DIV_FACT_set  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_CLK800_DIV_FACT_get  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00000070 DEGLITCHER_EN    |
 | +--------------------------------------+
 | | bit  31:15 R/W  PCBI_TX_DENI         |
 | | bit  2     R/W  TPCOUT_DEGLITCH_EN   |
 | | bit  1     R/W  BADJ_CLK_DEGLITCH_EN |
 | | bit  0     R/W  ADC_CLK_DEGLITCH_EN  |
 | +--------------------------------------+
 | 
 | void   t8_field_PCBI_TX_DENI_set         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PCBI_TX_DENI_get         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PCBI_TX_DENI_set   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PCBI_TX_DENI_get   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_TPCOUT_DEGLITCH_EN_set   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TPCOUT_DEGLITCH_EN_get   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_BADJ_CLK_DEGLITCH_EN_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_BADJ_CLK_DEGLITCH_EN_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_ADC_CLK_DEGLITCH_EN_set  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADC_CLK_DEGLITCH_EN_get  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +--------------------------------------+
 | | Register 0x00000074 DIGITAL_RSTB_REG |
 | +--------------------------------------+
 | | bit  31 R/W  GLBL_DIGITAL_RSTB       |
 | | bit  15 R/W  PCBI_FIFO_HW_RESET      |
 | | bit  12 R/W  MON_RSTB                |
 | | bit  11 R/W  GEN_RSTB                |
 | | bit  10 R/W  PISO_RSTB               |
 | | bit  9  R/W  SIPO_RSTB               |
 | | bit  8  R/W  DLOS_RSTB               |
 | | bit  7  R/W  PLL_LOCK_DET_RSTB       |
 | | bit  6  R/W  MDSP_RSTB               |
 | | bit  5  R/W  DIAGNOSTICS_RSTB        |
 | | bit  4  R/W  SIPO_BIST_RSTB          |
 | | bit  3  R/W  GCOC_RSTB               |
 | | bit  2  R/W  LPBK_DESKEW_FIFO_RSTB   |
 | | bit  1  R/W  DESKEW_FIFO_RSTB        |
 | | bit  0  R/W  PMM_WRAP_RSTB           |
 | +--------------------------------------+
 | 
 | void   t8_field_GLBL_DIGITAL_RSTB_set     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_GLBL_DIGITAL_RSTB_get     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PCBI_FIFO_HW_RESET_set    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PCBI_FIFO_HW_RESET_get    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_MON_RSTB_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_MON_RSTB_get              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_GEN_RSTB_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_GEN_RSTB_get              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PISO_RSTB_set             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PISO_RSTB_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_SIPO_RSTB_set             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_SIPO_RSTB_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_DLOS_RSTB_set             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_DLOS_RSTB_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PLL_LOCK_DET_RSTB_set     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PLL_LOCK_DET_RSTB_get     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_MDSP_RSTB_set             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_MDSP_RSTB_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_DIAGNOSTICS_RSTB_set      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_DIAGNOSTICS_RSTB_get      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_SIPO_BIST_RSTB_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_SIPO_BIST_RSTB_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_GCOC_RSTB_set             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_GCOC_RSTB_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_LPBK_DESKEW_FIFO_RSTB_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_LPBK_DESKEW_FIFO_RSTB_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_DESKEW_FIFO_RSTB_set      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_DESKEW_FIFO_RSTB_get      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PMM_WRAP_RSTB_set         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PMM_WRAP_RSTB_get         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +----------------------------------+
 | | Register 0x00000078 IE_REG       |
 | +----------------------------------+
 | | bit  19 R/W  CALIB_PASS_E19      |
 | | bit  18 R/W  PATT_MON_ERR_E18    |
 | | bit  17 R/W  ADC_BIST_ERR_E17    |
 | | bit  16 R/W  ADC_BIST_DONE_E16   |
 | | bit  15 R/W  ADCOC_FSM_ERR_E15   |
 | | bit  14 R/W  DLOS_E14            |
 | | bit  9  R/W  DIV_CLK_DEAD_E9     |
 | | bit  7  R/W  IN_RANGEN_FAILS_E7  |
 | | bit  6  R/W  IN_RANGEN_PASSES_E6 |
 | | bit  5  R/W  PLL_LOCK_GAIN_E5    |
 | | bit  4  R/W  PROG_REF_DONE_E4    |
 | | bit  3  R/W  CALIB_DONE_E3       |
 | | bit  0  R/W  PLL_LOCK_LOSS_E0    |
 | +----------------------------------+
 | 
 | void   t8_field_CALIB_PASS_E19_set      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CALIB_PASS_E19_get      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PATT_MON_ERR_E18_set    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PATT_MON_ERR_E18_get    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_ADC_BIST_ERR_E17_set    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADC_BIST_ERR_E17_get    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_ADC_BIST_DONE_E16_set   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADC_BIST_DONE_E16_get   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_ADCOC_FSM_ERR_E15_set   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADCOC_FSM_ERR_E15_get   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_DLOS_E14_set            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_DLOS_E14_get            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_DIV_CLK_DEAD_E9_set     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_DIV_CLK_DEAD_E9_get     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_IN_RANGEN_FAILS_E7_set  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_IN_RANGEN_FAILS_E7_get  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_IN_RANGEN_PASSES_E6_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_IN_RANGEN_PASSES_E6_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PLL_LOCK_GAIN_E5_set    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PLL_LOCK_GAIN_E5_get    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PROG_REF_DONE_E4_set    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PROG_REF_DONE_E4_get    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_CALIB_DONE_E3_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CALIB_DONE_E3_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PLL_LOCK_LOSS_E0_set    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PLL_LOCK_LOSS_E0_get    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +----------------------------------+
 | | Register 0x0000007c INT_REG      |
 | +----------------------------------+
 | | bit  19 R/W  CALIB_PASS_I19      |
 | | bit  18 R/W  PATT_MON_ERR_I18    |
 | | bit  17 R/W  ADC_BIST_ERR_I17    |
 | | bit  16 R/W  ADC_BIST_DONE_I16   |
 | | bit  15 R/W  ADCOC_FSM_ERR_I15   |
 | | bit  14 R/W  DLOS_I14            |
 | | bit  9  R/W  DIV_CLK_DEAD_I9     |
 | | bit  7  R/W  IN_RANGEN_FAILS_I7  |
 | | bit  6  R/W  IN_RANGEN_PASSES_I6 |
 | | bit  5  R/W  PLL_LOCK_GAIN_I5    |
 | | bit  4  R/W  PROG_REF_DONE_I4    |
 | | bit  3  R/W  CALIB_DONE_I3       |
 | | bit  0  R/W  PLL_LOCK_LOSS_I0    |
 | +----------------------------------+
 | 
 | void                 t8_field_CALIB_PASS_I19_set_to_clear      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_field_CALIB_PASS_I19_get               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_CALIB_PASS_I19_poll              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_field_PATT_MON_ERR_I18_set_to_clear    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_field_PATT_MON_ERR_I18_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_PATT_MON_ERR_I18_poll            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_field_ADC_BIST_ERR_I17_set_to_clear    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_field_ADC_BIST_ERR_I17_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_ADC_BIST_ERR_I17_poll            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_field_ADC_BIST_DONE_I16_set_to_clear   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_field_ADC_BIST_DONE_I16_get            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_ADC_BIST_DONE_I16_poll           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_field_ADCOC_FSM_ERR_I15_set_to_clear   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_field_ADCOC_FSM_ERR_I15_get            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_ADCOC_FSM_ERR_I15_poll           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_field_DLOS_I14_set_to_clear            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_field_DLOS_I14_get                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_DLOS_I14_poll                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_field_DIV_CLK_DEAD_I9_set_to_clear     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_field_DIV_CLK_DEAD_I9_get              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_DIV_CLK_DEAD_I9_poll             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_field_IN_RANGEN_FAILS_I7_set_to_clear  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_field_IN_RANGEN_FAILS_I7_get           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_IN_RANGEN_FAILS_I7_poll          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_field_IN_RANGEN_PASSES_I6_set_to_clear ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_field_IN_RANGEN_PASSES_I6_get          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_IN_RANGEN_PASSES_I6_poll         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_field_PLL_LOCK_GAIN_I5_set_to_clear    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_field_PLL_LOCK_GAIN_I5_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_PLL_LOCK_GAIN_I5_poll            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_field_PROG_REF_DONE_I4_set_to_clear    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_field_PROG_REF_DONE_I4_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_PROG_REF_DONE_I4_poll            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_field_CALIB_DONE_I3_set_to_clear       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_field_CALIB_DONE_I3_get                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_CALIB_DONE_I3_poll               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 t8_field_PLL_LOCK_LOSS_I0_set_to_clear    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32               t8_field_PLL_LOCK_LOSS_I0_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_PLL_LOCK_LOSS_I0_poll            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------+
 | | Register 0x00000080 IV_REG     |
 | +--------------------------------+
 | | bit  19 R  CALIB_PASS_V19      |
 | | bit  18 R  PATT_MON_ERR_V18    |
 | | bit  17 R  ADC_BIST_ERR_V17    |
 | | bit  16 R  ADC_BIST_DONE_V16   |
 | | bit  15 R  ADCOC_FSM_ERR_V15   |
 | | bit  14 R  DLOS_V14            |
 | | bit  9  R  DIV_CLK_DEAD_V9     |
 | | bit  7  R  IN_RANGEN_FAILS_V7  |
 | | bit  6  R  IN_RANGEN_PASSES_V6 |
 | | bit  5  R  PLL_LOCK_GAIN_V5    |
 | | bit  4  R  PROG_REF_DONE_V4    |
 | | bit  3  R  CALIB_DONE_V3       |
 | | bit  0  R  PLL_LOCK_LOSS_V0    |
 | +--------------------------------+
 | 
 | UINT32               t8_field_CALIB_PASS_V19_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_CALIB_PASS_V19_poll      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_field_PATT_MON_ERR_V18_get     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_PATT_MON_ERR_V18_poll    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_field_ADC_BIST_ERR_V17_get     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_ADC_BIST_ERR_V17_poll    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_field_ADC_BIST_DONE_V16_get    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_ADC_BIST_DONE_V16_poll   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_field_ADCOC_FSM_ERR_V15_get    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_ADCOC_FSM_ERR_V15_poll   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_field_DLOS_V14_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_DLOS_V14_poll            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_field_DIV_CLK_DEAD_V9_get      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_DIV_CLK_DEAD_V9_poll     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_field_IN_RANGEN_FAILS_V7_get   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_IN_RANGEN_FAILS_V7_poll  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_field_IN_RANGEN_PASSES_V6_get  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_IN_RANGEN_PASSES_V6_poll ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_field_PLL_LOCK_GAIN_V5_get     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_PLL_LOCK_GAIN_V5_poll    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_field_PROG_REF_DONE_V4_get     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_PROG_REF_DONE_V4_poll    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_field_CALIB_DONE_V3_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_CALIB_DONE_V3_poll       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               t8_field_PLL_LOCK_LOSS_V0_get     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_field_PLL_LOCK_LOSS_V0_poll    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x00000084 PRIM_LUT_INDEX_0_REG |
 | +------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_0          |
 | +------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_0_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_0_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_0_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_0_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000088 PRIM_LUT_INDEX_1_REG |
 | +------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_1          |
 | +------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_1_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_1_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_1_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_1_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0000008c PRIM_LUT_INDEX_2_REG |
 | +------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_2          |
 | +------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_2_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_2_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_2_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_2_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000090 PRIM_LUT_INDEX_3_REG |
 | +------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_3          |
 | +------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_3_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_3_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_3_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_3_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000094 PRIM_LUT_INDEX_4_REG |
 | +------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_4          |
 | +------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_4_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_4_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_4_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_4_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000098 PRIM_LUT_INDEX_5_REG |
 | +------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_5          |
 | +------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_5_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_5_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_5_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_5_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0000009c PRIM_LUT_INDEX_6_REG |
 | +------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_6          |
 | +------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_6_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_6_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_6_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_6_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x000000a0 TX_CTRL_STS_REG1 |
 | +--------------------------------------+
 | | bit  15:0 R  TX_SUM_OUT              |
 | +--------------------------------------+
 | 
 | UINT32 t8_field_TX_SUM_OUT_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_field_range_TX_SUM_OUT_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000100 PRIM_LUT_INDEX_7_REG |
 | +------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_7          |
 | +------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_7_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_7_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_7_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_7_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000104 PRIM_LUT_INDEX_8_REG |
 | +------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_8          |
 | +------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_8_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_8_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_8_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_8_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000108 PRIM_LUT_INDEX_9_REG |
 | +------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_9          |
 | +------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_9_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_9_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_9_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_9_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0000010c PRIM_LUT_INDEX_10_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_10          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_10_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_10_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_10_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_10_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000110 PRIM_LUT_INDEX_11_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_11          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_11_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_11_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_11_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_11_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000114 PRIM_LUT_INDEX_12_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_12          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_12_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_12_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_12_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_12_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000118 PRIM_LUT_INDEX_13_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_13          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_13_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_13_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_13_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_13_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0000011c PRIM_LUT_INDEX_14_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_14          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_14_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_14_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_14_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_14_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000120 PRIM_LUT_INDEX_15_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_15          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_15_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_15_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_15_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_15_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000124 PRIM_LUT_INDEX_16_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_16          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_16_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_16_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_16_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_16_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000128 PRIM_LUT_INDEX_17_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_17          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_17_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_17_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_17_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_17_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0000012c PRIM_LUT_INDEX_18_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_18          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_18_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_18_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_18_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_18_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000130 PRIM_LUT_INDEX_19_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_19          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_19_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_19_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_19_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_19_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000134 PRIM_LUT_INDEX_20_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_20          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_20_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_20_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_20_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_20_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000138 PRIM_LUT_INDEX_21_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_21          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_21_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_21_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_21_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_21_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0000013c PRIM_LUT_INDEX_22_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_22          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_22_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_22_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_22_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_22_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000140 PRIM_LUT_INDEX_23_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_23          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_23_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_23_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_23_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_23_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000144 PRIM_LUT_INDEX_24_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_24          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_24_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_24_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_24_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_24_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000148 PRIM_LUT_INDEX_25_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_25          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_25_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_25_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_25_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_25_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0000014c PRIM_LUT_INDEX_26_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_26          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_26_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_26_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_26_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_26_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000150 PRIM_LUT_INDEX_27_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_27          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_27_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_27_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_27_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_27_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000154 PRIM_LUT_INDEX_28_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_28          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_28_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_28_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_28_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_28_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000158 PRIM_LUT_INDEX_29_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_29          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_29_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_29_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_29_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_29_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0000015c PRIM_LUT_INDEX_30_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_30          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_30_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_30_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_30_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_30_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000160 PRIM_LUT_INDEX_31_REG |
 | +-------------------------------------------+
 | | bit  15:0 R/W  PRIM_LUT_INDEX_31          |
 | +-------------------------------------------+
 | 
 | void   t8_field_PRIM_LUT_INDEX_31_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PRIM_LUT_INDEX_31_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PRIM_LUT_INDEX_31_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PRIM_LUT_INDEX_31_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000164 ALT_LUT_INDEX_0_REG |
 | +-----------------------------------------+
 | | bit  15:0 R/W  ALT_LUT_INDEX_0          |
 | +-----------------------------------------+
 | 
 | void   t8_field_ALT_LUT_INDEX_0_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ALT_LUT_INDEX_0_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ALT_LUT_INDEX_0_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ALT_LUT_INDEX_0_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000168 ALT_LUT_INDEX_1_REG |
 | +-----------------------------------------+
 | | bit  15:0 R/W  ALT_LUT_INDEX_1          |
 | +-----------------------------------------+
 | 
 | void   t8_field_ALT_LUT_INDEX_1_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ALT_LUT_INDEX_1_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ALT_LUT_INDEX_1_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ALT_LUT_INDEX_1_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0000016c ALT_LUT_INDEX_2_REG |
 | +-----------------------------------------+
 | | bit  15:0 R/W  ALT_LUT_INDEX_2          |
 | +-----------------------------------------+
 | 
 | void   t8_field_ALT_LUT_INDEX_2_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ALT_LUT_INDEX_2_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ALT_LUT_INDEX_2_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ALT_LUT_INDEX_2_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000170 ALT_LUT_INDEX_3_REG |
 | +-----------------------------------------+
 | | bit  15:0 R/W  ALT_LUT_INDEX_3          |
 | +-----------------------------------------+
 | 
 | void   t8_field_ALT_LUT_INDEX_3_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ALT_LUT_INDEX_3_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ALT_LUT_INDEX_3_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ALT_LUT_INDEX_3_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000174 ALT_LUT_INDEX_4_REG |
 | +-----------------------------------------+
 | | bit  15:0 R/W  ALT_LUT_INDEX_4          |
 | +-----------------------------------------+
 | 
 | void   t8_field_ALT_LUT_INDEX_4_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ALT_LUT_INDEX_4_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ALT_LUT_INDEX_4_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ALT_LUT_INDEX_4_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000178 ALT_LUT_INDEX_5_REG |
 | +-----------------------------------------+
 | | bit  15:0 R/W  ALT_LUT_INDEX_5          |
 | +-----------------------------------------+
 | 
 | void   t8_field_ALT_LUT_INDEX_5_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ALT_LUT_INDEX_5_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ALT_LUT_INDEX_5_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ALT_LUT_INDEX_5_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x0000017c ALT_LUT_INDEX_6_REG |
 | +-----------------------------------------+
 | | bit  15:0 R/W  ALT_LUT_INDEX_6          |
 | +-----------------------------------------+
 | 
 | void   t8_field_ALT_LUT_INDEX_6_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ALT_LUT_INDEX_6_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ALT_LUT_INDEX_6_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ALT_LUT_INDEX_6_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000180 ALT_LUT_INDEX_7_REG |
 | +-----------------------------------------+
 | | bit  15:0 R/W  ALT_LUT_INDEX_7          |
 | +-----------------------------------------+
 | 
 | void   t8_field_ALT_LUT_INDEX_7_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ALT_LUT_INDEX_7_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ALT_LUT_INDEX_7_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ALT_LUT_INDEX_7_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00000184 PGA_LUT_CTRL_REG |
 | +--------------------------------------+
 | | bit  24    R/W  PLEN                 |
 | | bit  22:20 R/W  CG_S2_IN             |
 | | bit  18:16 R/W  CG_S1_IN             |
 | | bit  15:12 R/W  RG_S2_IN             |
 | | bit  11:8  R/W  RG_S1_IN             |
 | | bit  1:0   R/W  ATTENUATE_EN_IN      |
 | +--------------------------------------+
 | 
 | void   t8_field_PLEN_set                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PLEN_get                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_CG_S2_IN_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CG_S2_IN_get              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_CG_S2_IN_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_CG_S2_IN_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_CG_S1_IN_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CG_S1_IN_get              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_CG_S1_IN_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_CG_S1_IN_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_RG_S2_IN_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_RG_S2_IN_get              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_RG_S2_IN_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_RG_S2_IN_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_RG_S1_IN_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_RG_S1_IN_get              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_RG_S1_IN_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_RG_S1_IN_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_ATTENUATE_EN_IN_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ATTENUATE_EN_IN_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_ATTENUATE_EN_IN_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_ATTENUATE_EN_IN_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000188 PLL_CNFG_REG2 |
 | +-----------------------------------+
 | | bit  19:0 R/W  CLK_CNT_MAX        |
 | +-----------------------------------+
 | 
 | void   t8_field_CLK_CNT_MAX_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CLK_CNT_MAX_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_CLK_CNT_MAX_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_CLK_CNT_MAX_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x0000018c PLL_CNFG_REG4 |
 | +-----------------------------------+
 | | bit  31:16 R/W  FAIL_CNT_MAX      |
 | | bit  15:0  R/W  PASS_CNT_MAX      |
 | +-----------------------------------+
 | 
 | void   t8_field_FAIL_CNT_MAX_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_FAIL_CNT_MAX_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_FAIL_CNT_MAX_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_FAIL_CNT_MAX_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_PASS_CNT_MAX_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PASS_CNT_MAX_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PASS_CNT_MAX_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PASS_CNT_MAX_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000190 PLL_CNFG_REG6 |
 | +-----------------------------------+
 | | bit  7:0 R/W  LOCK_CNT_MAX        |
 | +-----------------------------------+
 | 
 | void   t8_field_LOCK_CNT_MAX_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_LOCK_CNT_MAX_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_LOCK_CNT_MAX_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_LOCK_CNT_MAX_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000194 PLL_STS_REG |
 | +---------------------------------+
 | | bit  31   R  PPM_OFF_RD_SIGN    |
 | | bit  20:0 R  PPM_OFF_RD_VAL     |
 | +---------------------------------+
 | 
 | UINT32 t8_field_PPM_OFF_RD_SIGN_get      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_field_PPM_OFF_RD_VAL_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_field_range_PPM_OFF_RD_VAL_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000198 MDSP_CFG_REG1 |
 | +-----------------------------------+
 | | bit  31  R/W  ADD_DROP_HOLD       |
 | | bit  4   R/W  TED_HOLD_OUTPUT_EN  |
 | | bit  3   R/W  DFE_BYPASS          |
 | | bit  2:1 R/W  TED_MODE            |
 | | bit  0   R/W  DLB_ENB             |
 | +-----------------------------------+
 | 
 | void   t8_field_ADD_DROP_HOLD_set      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ADD_DROP_HOLD_get      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_TED_HOLD_OUTPUT_EN_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TED_HOLD_OUTPUT_EN_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_DFE_BYPASS_set         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_DFE_BYPASS_get         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_TED_MODE_set           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_TED_MODE_get           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_TED_MODE_set     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_TED_MODE_get     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_DLB_ENB_set            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_DLB_ENB_get            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +-----------------------------------+
 | | Register 0x0000019c MDSP_CFG_REG2 |
 | +-----------------------------------+
 | | bit  15:8 R/W  LF_CLAMP_MIN       |
 | | bit  7:0  R/W  LF_CLAMP_MAX       |
 | +-----------------------------------+
 | 
 | void   t8_field_LF_CLAMP_MIN_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_LF_CLAMP_MIN_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_LF_CLAMP_MIN_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_LF_CLAMP_MIN_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_LF_CLAMP_MAX_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_LF_CLAMP_MAX_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_LF_CLAMP_MAX_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_LF_CLAMP_MAX_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000204 PGA_CFG_REG     |
 | +-------------------------------------+
 | | bit  7 R/W  PGA_ATMSB               |
 | | bit  6 R/W  PGA_CMFILT_ENB          |
 | | bit  1 R/W  PGA_ATTENUATE_FORCE_OFF |
 | +-------------------------------------+
 | 
 | void   t8_field_PGA_ATMSB_set               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PGA_ATMSB_get               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PGA_CMFILT_ENB_set          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PGA_CMFILT_ENB_get          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PGA_ATTENUATE_FORCE_OFF_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PGA_ATTENUATE_FORCE_OFF_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +----------------------------------------------+
 | | Register 0x00000208 DIAG_CNFG_REG1           |
 | +----------------------------------------------+
 | | bit  31:24 R/W  VHIST_CNT_TIM_LIMIT_39_32    |
 | | bit  3     R/W  VHIST_TITAN_TARGET_AMP_WR_EN |
 | | bit  2     R/W  VHIST_PERBIN_VALUE_RD_ENB    |
 | | bit  1     R/W  VHIST_CAPT_START             |
 | | bit  0     R/W  VHIST_CAPT_ENB               |
 | +----------------------------------------------+
 | 
 | void   t8_field_VHIST_CNT_TIM_LIMIT_39_32_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_VHIST_CNT_TIM_LIMIT_39_32_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_VHIST_CNT_TIM_LIMIT_39_32_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_VHIST_CNT_TIM_LIMIT_39_32_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_VHIST_TITAN_TARGET_AMP_WR_EN_set    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_VHIST_TITAN_TARGET_AMP_WR_EN_get    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_VHIST_PERBIN_VALUE_RD_ENB_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_VHIST_PERBIN_VALUE_RD_ENB_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_VHIST_CAPT_START_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_VHIST_CAPT_START_get                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_VHIST_CAPT_ENB_set                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_VHIST_CAPT_ENB_get                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +-----------------------------------------+
 | | Register 0x0000020c DIAG_CNFG_REG2      |
 | +-----------------------------------------+
 | | bit  31:0 R/W  VHIST_CNT_TIM_LIMIT_31_0 |
 | +-----------------------------------------+
 | 
 | void   t8_field_VHIST_CNT_TIM_LIMIT_31_0_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_VHIST_CNT_TIM_LIMIT_31_0_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_VHIST_CNT_TIM_LIMIT_31_0_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_VHIST_CNT_TIM_LIMIT_31_0_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000210 DIAG_STATUS_REG1   |
 | +----------------------------------------+
 | | bit  31:24 R  VHIST_PERBIN_VALUE_39_32 |
 | | bit  15:10 R  VHIST_READBACK_ADDR      |
 | | bit  7:5   R  VHIST_FSM_STATE          |
 | | bit  1     R  VHIST_PERBIN_VALUE_OVFL  |
 | | bit  0     R  VHIST_CAPT_RUNNING       |
 | +----------------------------------------+
 | 
 | UINT32 t8_field_VHIST_PERBIN_VALUE_39_32_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_field_range_VHIST_PERBIN_VALUE_39_32_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_field_VHIST_READBACK_ADDR_get            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_field_range_VHIST_READBACK_ADDR_get      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_field_VHIST_FSM_STATE_get                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_field_range_VHIST_FSM_STATE_get          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 t8_field_VHIST_PERBIN_VALUE_OVFL_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_field_VHIST_CAPT_RUNNING_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +--------------------------------------+
 | | Register 0x00000214 DIAG_STATUS_REG2 |
 | +--------------------------------------+
 | | bit  31:0 R  VHIST_PERBIN_VALUE_31_0 |
 | +--------------------------------------+
 | 
 | UINT32 t8_field_VHIST_PERBIN_VALUE_31_0_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_field_range_VHIST_PERBIN_VALUE_31_0_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000218 OFFSET_RAM_CNFG_REG |
 | +-----------------------------------------+
 | | bit  1:0 R/W  OFFSET_RAM_QUADRANT_SEL   |
 | +-----------------------------------------+
 | 
 | void   t8_field_OFFSET_RAM_QUADRANT_SEL_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_OFFSET_RAM_QUADRANT_SEL_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_OFFSET_RAM_QUADRANT_SEL_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_OFFSET_RAM_QUADRANT_SEL_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0000021c DIG_LPBK_CTRL_REG  |
 | +----------------------------------------+
 | | bit  31:29 R/W  RD_PTR_SYNC_VAL        |
 | | bit  28:26 R/W  DESKEW_RD_PTR_SYNC_VAL |
 | | bit  2     R/W  DESKEW_FIFO_SYNC       |
 | | bit  1     R/W  PLL_LOCK_DET_SW_RST    |
 | | bit  0     R/W  DIG_LPBK_SYNC          |
 | +----------------------------------------+
 | 
 | void   t8_field_RD_PTR_SYNC_VAL_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_RD_PTR_SYNC_VAL_get              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_RD_PTR_SYNC_VAL_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_RD_PTR_SYNC_VAL_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_DESKEW_RD_PTR_SYNC_VAL_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_DESKEW_RD_PTR_SYNC_VAL_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_DESKEW_RD_PTR_SYNC_VAL_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_DESKEW_RD_PTR_SYNC_VAL_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_DESKEW_FIFO_SYNC_set             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_DESKEW_FIFO_SYNC_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PLL_LOCK_DET_SW_RST_set          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PLL_LOCK_DET_SW_RST_get          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_DIG_LPBK_SYNC_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_DIG_LPBK_SYNC_get                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +--------------------------------------+
 | | Register 0x00000220 ADC_OC_REGISTER4 |
 | +--------------------------------------+
 | | bit  31:0 R/W  THR_MAP63_32          |
 | +--------------------------------------+
 | 
 | void   t8_field_THR_MAP63_32_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_THR_MAP63_32_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_THR_MAP63_32_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_THR_MAP63_32_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00000224 ADC_OC_REGISTER5 |
 | +--------------------------------------+
 | | bit  31:24 R/W  MAX_THRESHOLD        |
 | | bit  23:16 R/W  MIN_THRESHOLD        |
 | | bit  15:0  R/W  THR_MAP79_64         |
 | +--------------------------------------+
 | 
 | void   t8_field_MAX_THRESHOLD_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_MAX_THRESHOLD_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_MAX_THRESHOLD_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_MAX_THRESHOLD_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_MIN_THRESHOLD_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_MIN_THRESHOLD_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_MIN_THRESHOLD_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_MIN_THRESHOLD_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_THR_MAP79_64_set        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_THR_MAP79_64_get        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_THR_MAP79_64_set  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_THR_MAP79_64_get  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000228 GCOC_MABC_CFG_REG |
 | +---------------------------------------+
 | | bit  7:4 R/W  OFFSET                  |
 | +---------------------------------------+
 | 
 | void   t8_field_OFFSET_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_OFFSET_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_OFFSET_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_OFFSET_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------+
 | | Register 0x00000230 DLOS_CFG |
 | +------------------------------+
 | | bit  18    R/W  DLOS_OVR_EN  |
 | | bit  17    R/W  DLOS_OVR     |
 | | bit  16:13 R/W  LOS_ACC_CNT  |
 | | bit  12:5  R/W  LOS_LIMIT    |
 | | bit  0     R/W  ONE_COMP_EN  |
 | +------------------------------+
 | 
 | void   t8_field_DLOS_OVR_EN_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_DLOS_OVR_EN_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_DLOS_OVR_set          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_DLOS_OVR_get          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_LOS_ACC_CNT_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_LOS_ACC_CNT_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_LOS_ACC_CNT_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_LOS_ACC_CNT_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_LOS_LIMIT_set         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_LOS_LIMIT_get         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_LOS_LIMIT_set   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_LOS_LIMIT_get   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_ONE_COMP_EN_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_ONE_COMP_EN_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +----------------------------------------+
 | | Register 0x00000234 PATT_GEN_CNFG_REG1 |
 | +----------------------------------------+
 | | bit  31:24 R/W  GEN_USR_PAT_39_32      |
 | | bit  14:9  R/W  PATT_BUS_WIDTH         |
 | | bit  8     R/W  GEN_ERR_INJ            |
 | | bit  7     R/W  GEN_INV                |
 | | bit  6:3   R/W  GEN_MODE               |
 | | bit  2     R/W  INV_TX_DATA            |
 | | bit  1     R/W  PATT_GEN_SEL           |
 | | bit  0     R/W  GEN_EN                 |
 | +----------------------------------------+
 | 
 | void   t8_field_GEN_USR_PAT_39_32_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_GEN_USR_PAT_39_32_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_GEN_USR_PAT_39_32_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_GEN_USR_PAT_39_32_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_PATT_BUS_WIDTH_set          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PATT_BUS_WIDTH_get          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PATT_BUS_WIDTH_set    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PATT_BUS_WIDTH_get    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_GEN_ERR_INJ_set             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_GEN_ERR_INJ_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_GEN_INV_set                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_GEN_INV_get                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_GEN_MODE_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_GEN_MODE_get                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_GEN_MODE_set          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_GEN_MODE_get          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_INV_TX_DATA_set             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_INV_TX_DATA_get             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_PATT_GEN_SEL_set            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PATT_GEN_SEL_get            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_GEN_EN_set                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_GEN_EN_get                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +----------------------------------------+
 | | Register 0x00000238 PATT_GEN_CNFG_REG2 |
 | +----------------------------------------+
 | | bit  31:0 R/W  GEN_USR_PAT_31_0        |
 | +----------------------------------------+
 | 
 | void   t8_field_GEN_USR_PAT_31_0_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_GEN_USR_PAT_31_0_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_GEN_USR_PAT_31_0_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_GEN_USR_PAT_31_0_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0000023c PATT_CHK_CNFG_REG1 |
 | +----------------------------------------+
 | | bit  31:24 R/W  CHK_USR_PAT_39_32      |
 | | bit  8     R/W  CHK_READ               |
 | | bit  7     R/W  CHK_INV                |
 | | bit  6:3   R/W  CHK_MODE               |
 | | bit  0     R/W  CHK_EN                 |
 | +----------------------------------------+
 | 
 | void   t8_field_CHK_USR_PAT_39_32_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CHK_USR_PAT_39_32_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_CHK_USR_PAT_39_32_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_CHK_USR_PAT_39_32_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_CHK_READ_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CHK_READ_get                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_CHK_INV_set                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CHK_INV_get                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_CHK_MODE_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CHK_MODE_get                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_CHK_MODE_set          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_CHK_MODE_get          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_field_CHK_EN_set                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CHK_EN_get                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | +----------------------------------------+
 | | Register 0x00000240 PATT_CHK_CNFG_REG2 |
 | +----------------------------------------+
 | | bit  31:0 R/W  CHK_USR_PAT_31_0        |
 | +----------------------------------------+
 | 
 | void   t8_field_CHK_USR_PAT_31_0_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_CHK_USR_PAT_31_0_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_CHK_USR_PAT_31_0_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_CHK_USR_PAT_31_0_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000244 PATT_CHK_STATUS_REG1 |
 | +------------------------------------------+
 | | bit  31:0 R  CHK_ERR_CNT                 |
 | +------------------------------------------+
 | 
 | UINT32 t8_field_CHK_ERR_CNT_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | UINT32 t8_field_range_CHK_ERR_CNT_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000248 TX_CNTRL_OVERRIDE_5 |
 | +-----------------------------------------+
 | | bit  16:0 R/W  PCBI_TX_PSELI            |
 | +-----------------------------------------+
 | 
 | void   t8_field_PCBI_TX_PSELI_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | UINT32 t8_field_PCBI_TX_PSELI_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void   t8_field_range_PCBI_TX_PSELI_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_field_range_PCBI_TX_PSELI_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x0000024c + (N) * 0x4) PM20_82_24_OFFSET_RAM_REG0 |
 | |     N[0..95]                                                 |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  PM20_82_24_MTSB0_OFFSET_RAM_REG               |
 | +--------------------------------------------------------------+
 | 
 | void   t8_lfield_PM20_82_24_MTSB0_OFFSET_RAM_REG_set       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value[96] )
 | void   t8_lfield_PM20_82_24_MTSB0_OFFSET_RAM_REG_get       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value[96] )
 | void   t8_lfield_range_PM20_82_24_MTSB0_OFFSET_RAM_REG_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_lfield_range_PM20_82_24_MTSB0_OFFSET_RAM_REG_get ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 start_bit, UINT32 stop_bit )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _T8_IO_H
#define _T8_IO_H

#include "t8.h"
#include "t8_regs.h"
#include "t8_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 t8_buffer_init               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | void                 t8_buffer_flush              ( t8_buffer_t *b_ptr )
 | UINT32               t8_reg_read                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mem_type, UINT32 reg )
 | void                 t8_reg_write                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 t8_field_set                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 t8_action_on_write_field_set ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 t8_burst_read                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 t8_burst_write               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE t8_poll                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 t8_reg_PCIE3_CFG_1_write                            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PCIE3_CFG_1_field_set                        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PCIE3_CFG_1_read                             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PCIE3_CFG_2_write                            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PCIE3_CFG_2_field_set                        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PCIE3_CFG_2_read                             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_TX_CNTR_CFG_1_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_TX_CNTR_CFG_1_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_TX_CNTR_CFG_1_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_TX_CNTR_CFG_2_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_TX_CNTR_CFG_2_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_TX_CNTR_CFG_2_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_TX_CNTR_CFG_3_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_TX_CNTR_CFG_3_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_TX_CNTR_CFG_3_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_TX_CNTR_CFG_4_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_TX_CNTR_CFG_4_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_TX_CNTR_CFG_4_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_TX_CNTR_CFG_5_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_TX_CNTR_CFG_5_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_TX_CNTR_CFG_5_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_TX_CNTR_CFG_6_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_TX_CNTR_CFG_6_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_TX_CNTR_CFG_6_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_DIG_LPBK_CFG_write                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_DIG_LPBK_CFG_field_set                       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_DIG_LPBK_CFG_read                            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_ANALOG_SIPO_BIST_CFG_REG_write               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_ANALOG_SIPO_BIST_CFG_REG_field_set           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_ANALOG_SIPO_BIST_CFG_REG_read                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_OFFSET_RAM_CFG_write                         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_OFFSET_RAM_CFG_field_set                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_OFFSET_RAM_CFG_read                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PLL_CNFG_REG1_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PLL_CNFG_REG1_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PLL_CNFG_REG1_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_ADC_OC_CFG_1_write                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_ADC_OC_CFG_1_field_set                       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_ADC_OC_CFG_1_read                            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_ADC_OC_CFG_2_write                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_ADC_OC_CFG_2_field_set                       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_ADC_OC_CFG_2_read                            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_ADC_OC_REGISTER3_write                       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_ADC_OC_REGISTER3_field_set                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_ADC_OC_REGISTER3_read                        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PMM_WRAP_CNFG_REG1_write                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PMM_WRAP_CNFG_REG1_field_set                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PMM_WRAP_CNFG_REG1_read                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PMM_WRAP_CNFG_REG2_write                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PMM_WRAP_CNFG_REG2_field_set                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PMM_WRAP_CNFG_REG2_read                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_SIPO_CNFG_REG_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_SIPO_CNFG_REG_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_SIPO_CNFG_REG_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PISO_CNFG_REG_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PISO_CNFG_REG_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PISO_CNFG_REG_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_TX_CNTRL_OVERRIDE_1_write                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_TX_CNTRL_OVERRIDE_1_field_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_TX_CNTRL_OVERRIDE_1_read                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_TX_CNTRL_OVERRIDE_REG_5_write                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_TX_CNTRL_OVERRIDE_REG_5_field_set            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_TX_CNTRL_OVERRIDE_REG_5_read                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_CLK_DIV_FACT_write                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_CLK_DIV_FACT_field_set                       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_CLK_DIV_FACT_read                            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_DEGLITCHER_EN_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_DEGLITCHER_EN_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_DEGLITCHER_EN_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_DIGITAL_RSTB_REG_write                       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_DIGITAL_RSTB_REG_field_set                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_DIGITAL_RSTB_REG_read                        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_0_REG_write                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_0_REG_field_set               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_0_REG_read                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_1_REG_write                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_1_REG_field_set               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_1_REG_read                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_2_REG_write                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_2_REG_field_set               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_2_REG_read                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_3_REG_write                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_3_REG_field_set               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_3_REG_read                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_4_REG_write                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_4_REG_field_set               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_4_REG_read                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_5_REG_write                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_5_REG_field_set               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_5_REG_read                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_6_REG_write                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_6_REG_field_set               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_6_REG_read                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_7_REG_write                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_7_REG_field_set               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_7_REG_read                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_8_REG_write                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_8_REG_field_set               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_8_REG_read                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_9_REG_write                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_9_REG_field_set               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_9_REG_read                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_10_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_10_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_10_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_11_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_11_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_11_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_12_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_12_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_12_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_13_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_13_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_13_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_14_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_14_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_14_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_15_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_15_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_15_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_16_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_16_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_16_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_17_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_17_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_17_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_18_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_18_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_18_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_19_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_19_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_19_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_20_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_20_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_20_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_21_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_21_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_21_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_22_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_22_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_22_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_23_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_23_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_23_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_24_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_24_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_24_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_25_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_25_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_25_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_26_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_26_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_26_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_27_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_27_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_27_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_28_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_28_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_28_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_29_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_29_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_29_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_30_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_30_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_30_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PRIM_LUT_INDEX_31_REG_write                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PRIM_LUT_INDEX_31_REG_field_set              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PRIM_LUT_INDEX_31_REG_read                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_ALT_LUT_INDEX_0_REG_write                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_ALT_LUT_INDEX_0_REG_field_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_ALT_LUT_INDEX_0_REG_read                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_ALT_LUT_INDEX_1_REG_write                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_ALT_LUT_INDEX_1_REG_field_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_ALT_LUT_INDEX_1_REG_read                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_ALT_LUT_INDEX_2_REG_write                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_ALT_LUT_INDEX_2_REG_field_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_ALT_LUT_INDEX_2_REG_read                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_ALT_LUT_INDEX_3_REG_write                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_ALT_LUT_INDEX_3_REG_field_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_ALT_LUT_INDEX_3_REG_read                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_ALT_LUT_INDEX_4_REG_write                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_ALT_LUT_INDEX_4_REG_field_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_ALT_LUT_INDEX_4_REG_read                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_ALT_LUT_INDEX_5_REG_write                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_ALT_LUT_INDEX_5_REG_field_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_ALT_LUT_INDEX_5_REG_read                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_ALT_LUT_INDEX_6_REG_write                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_ALT_LUT_INDEX_6_REG_field_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_ALT_LUT_INDEX_6_REG_read                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_ALT_LUT_INDEX_7_REG_write                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_ALT_LUT_INDEX_7_REG_field_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_ALT_LUT_INDEX_7_REG_read                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PGA_LUT_CTRL_REG_write                       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PGA_LUT_CTRL_REG_field_set                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PGA_LUT_CTRL_REG_read                        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PLL_CNFG_REG2_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PLL_CNFG_REG2_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PLL_CNFG_REG2_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PLL_CNFG_REG4_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PLL_CNFG_REG4_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PLL_CNFG_REG4_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PLL_CNFG_REG6_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PLL_CNFG_REG6_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PLL_CNFG_REG6_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_MDSP_CFG_REG1_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_MDSP_CFG_REG1_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_MDSP_CFG_REG1_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_MDSP_CFG_REG2_write                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_MDSP_CFG_REG2_field_set                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_MDSP_CFG_REG2_read                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PGA_CFG_REG_write                            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PGA_CFG_REG_field_set                        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PGA_CFG_REG_read                             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_DIAG_CNFG_REG1_write                         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_DIAG_CNFG_REG1_field_set                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_DIAG_CNFG_REG1_read                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_DIAG_CNFG_REG2_write                         ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_DIAG_CNFG_REG2_field_set                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_DIAG_CNFG_REG2_read                          ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_OFFSET_RAM_CNFG_REG_write                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_OFFSET_RAM_CNFG_REG_field_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_OFFSET_RAM_CNFG_REG_read                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_DIG_LPBK_CTRL_REG_write                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_DIG_LPBK_CTRL_REG_field_set                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_DIG_LPBK_CTRL_REG_read                       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_ADC_OC_REGISTER4_write                       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_ADC_OC_REGISTER4_field_set                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_ADC_OC_REGISTER4_read                        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_ADC_OC_REGISTER5_write                       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_ADC_OC_REGISTER5_field_set                   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_ADC_OC_REGISTER5_read                        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_GCOC_MABC_CFG_REG_write                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_GCOC_MABC_CFG_REG_field_set                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_GCOC_MABC_CFG_REG_read                       ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_DLOS_CFG_write                               ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_DLOS_CFG_field_set                           ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_DLOS_CFG_read                                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PATT_GEN_CNFG_REG1_write                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PATT_GEN_CNFG_REG1_field_set                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PATT_GEN_CNFG_REG1_read                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PATT_GEN_CNFG_REG2_write                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PATT_GEN_CNFG_REG2_field_set                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PATT_GEN_CNFG_REG2_read                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PATT_CHK_CNFG_REG1_write                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PATT_CHK_CNFG_REG1_field_set                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PATT_CHK_CNFG_REG1_read                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_PATT_CHK_CNFG_REG2_write                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_PATT_CHK_CNFG_REG2_field_set                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_PATT_CHK_CNFG_REG2_read                      ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_TX_CNTRL_OVERRIDE_5_write                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_TX_CNTRL_OVERRIDE_5_field_set                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_TX_CNTRL_OVERRIDE_5_read                     ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | void                 t8_reg_INT_REG_write                                ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_INT_REG_action_on_write_field_set            ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_INT_REG_read                                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_reg_INT_REG_poll                                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 t8_reg_IE_REG_write                                 ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 value )
 | void                 t8_reg_IE_REG_field_set                             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               t8_reg_IE_REG_read                                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | UINT32               t8_reg_IV_REG_read                                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | PMC_POLL_RETURN_TYPE t8_reg_IV_REG_poll                                  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 t8_reg_PM20_82_24_OFFSET_RAM_REG0_array_burst_write ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 t8_reg_PM20_82_24_OFFSET_RAM_REG0_array_field_set   ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 t8_reg_PM20_82_24_OFFSET_RAM_REG0_array_burst_read  ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               t8_reg_PM20_82_24_OFFSET_RAM_REG0_array_read        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id, UINT32 N )
 | 
 | UINT32               t8_reg_ANALOG_SIPO_BIST_STATUS_REG_read             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | UINT32               t8_reg_TX_CNTR_STA_read                             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | UINT32               t8_reg_ADC_OC_STS_read                              ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | UINT32               t8_reg_TX_CTRL_STS_REG1_read                        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | UINT32               t8_reg_PLL_STS_REG_read                             ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | UINT32               t8_reg_DIAG_STATUS_REG1_read                        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | UINT32               t8_reg_DIAG_STATUS_REG2_read                        ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 | UINT32               t8_reg_PATT_CHK_STATUS_REG1_read                    ( t8_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 id )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _T8_IO_H */
