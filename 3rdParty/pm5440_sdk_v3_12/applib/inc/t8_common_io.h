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
 *     and register accessor functions for the t8_common block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing t8_common io functions is:
 | {
 |     t8_common_buffer_t b_ptr[1];
 |     t8_common_buffer_init( b_ptr, h_ptr );
 |     value = t8_common_field_<FIELD>_get( b_ptr, h_ptr );
 |     t8_common_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     t8_common_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = t8_common_field_<FIELD>_get( NULL, h_ptr );
 | t8_common_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------+
 | | Register 0x0000060c RAM_CAPT_CONFIG_REG |
 | +-----------------------------------------+
 | | bit  9 R/W  RAMCAPT_STOP_EVENT          |
 | | bit  8 R/W  RAMCAPT_DATA_OUT_RD_EN      |
 | | bit  4 R/W  LS_RAMCAPT                  |
 | | bit  3 R/W  RAMCAPT_RST_RD_ADDR         |
 | | bit  2 R/W  RAMCAPT_CAPT_MODE           |
 | | bit  1 R/W  RAMCAPT_CAPT_START          |
 | | bit  0 R/W  RAMCAPT_ENB                 |
 | +-----------------------------------------+
 | 
 | void   t8_common_field_RAMCAPT_STOP_EVENT_set     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_RAMCAPT_STOP_EVENT_get     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_LS_RAMCAPT_set             ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_LS_RAMCAPT_get             ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_RAMCAPT_RST_RD_ADDR_set    ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_RAMCAPT_RST_RD_ADDR_get    ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_RAMCAPT_CAPT_MODE_set      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_RAMCAPT_CAPT_MODE_get      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_RAMCAPT_CAPT_START_set     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_RAMCAPT_CAPT_START_get     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_RAMCAPT_ENB_set            ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_RAMCAPT_ENB_get            ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_RAMCAPT_DATA_OUT_RD_EN_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_RAMCAPT_DATA_OUT_RD_EN_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | +----------------------------------------+
 | | Register 0x00000610 RAM_CAP_STATUS_REG |
 | +----------------------------------------+
 | | bit  31:0 R  RAMCAPT_DATA_OUT          |
 | +----------------------------------------+
 | 
 | UINT32 t8_common_field_RAMCAPT_DATA_OUT_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | UINT32 t8_common_field_range_RAMCAPT_DATA_OUT_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000614 RST_REG       |
 | +-----------------------------------+
 | | bit  13 R/W  PCBI_RXCPGEN_ARSTB   |
 | | bit  12 R/W  RXCPGEN_ARSTB_OVR_EN |
 | | bit  10 R/W  PCBI_RXCPGEN_RSTB    |
 | | bit  9  R/W  RXCPGEN_RSTB_OVR_EN  |
 | | bit  8  R/W  PCBI_PISO_RSTB       |
 | | bit  7  R/W  PISO_RSTB_OVR_EN     |
 | | bit  5  R/W  PCBI_ADC_RSTB        |
 | | bit  4  R/W  ADC_RSTB_OVR_EN      |
 | | bit  3  R/W  PCBI_ADC_DAC_RSTB    |
 | | bit  2  R/W  ADC_DAC_RSTB_OVR_EN  |
 | +-----------------------------------+
 | 
 | void   t8_common_field_PCBI_RXCPGEN_ARSTB_set   ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_PCBI_RXCPGEN_ARSTB_get   ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_RXCPGEN_ARSTB_OVR_EN_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_RXCPGEN_ARSTB_OVR_EN_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_PCBI_RXCPGEN_RSTB_set    ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_PCBI_RXCPGEN_RSTB_get    ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_RXCPGEN_RSTB_OVR_EN_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_RXCPGEN_RSTB_OVR_EN_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_PCBI_PISO_RSTB_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_PCBI_PISO_RSTB_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_PISO_RSTB_OVR_EN_set     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_PISO_RSTB_OVR_EN_get     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_PCBI_ADC_RSTB_set        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_PCBI_ADC_RSTB_get        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_ADC_RSTB_OVR_EN_set      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_ADC_RSTB_OVR_EN_get      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_PCBI_ADC_DAC_RSTB_set    ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_PCBI_ADC_DAC_RSTB_get    ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_ADC_DAC_RSTB_OVR_EN_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_ADC_DAC_RSTB_OVR_EN_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x00000618 PCIE3_CFG |
 | +-------------------------------+
 | | bit  21:17 R/W  ATIN          |
 | | bit  16    R/W  RXCPGEN_ENB   |
 | +-------------------------------+
 | 
 | void   t8_common_field_ATIN_set        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_ATIN_get        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_ATIN_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_ATIN_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_RXCPGEN_ENB_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_RXCPGEN_ENB_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x0000061c MABC_CONFIG_REG_1 |
 | +---------------------------------------+
 | | bit  3 R/W  CS_RXCPGEN_DLY_RSTB       |
 | | bit  2 R/W  CS_PISO_DLY_RSTB          |
 | | bit  1 R/W  CS_ADC_DLY_RSTB           |
 | | bit  0 R/W  CS_RXCPGEN_DLY_ARSTB      |
 | +---------------------------------------+
 | 
 | void   t8_common_field_CS_RXCPGEN_DLY_RSTB_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_CS_RXCPGEN_DLY_RSTB_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_CS_PISO_DLY_RSTB_set     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_CS_PISO_DLY_RSTB_get     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_CS_ADC_DLY_RSTB_set      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_CS_ADC_DLY_RSTB_get      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_CS_RXCPGEN_DLY_ARSTB_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_CS_RXCPGEN_DLY_ARSTB_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000620 MABC_CONFIG_REG_2 |
 | +---------------------------------------+
 | | bit  15:0 R/W  MABC_RXCPGEN_CTRL      |
 | +---------------------------------------+
 | 
 | void   t8_common_field_MABC_RXCPGEN_CTRL_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_MABC_RXCPGEN_CTRL_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_MABC_RXCPGEN_CTRL_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_MABC_RXCPGEN_CTRL_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000624 MABC_CONFIG_REG_3 |
 | +---------------------------------------+
 | | bit  31:24 R/W  PISO_CTRL_B           |
 | | bit  7:0   R/W  PISO_CTRL_A           |
 | +---------------------------------------+
 | 
 | void   t8_common_field_PISO_CTRL_B_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_PISO_CTRL_B_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_PISO_CTRL_B_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_PISO_CTRL_B_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_PISO_CTRL_A_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_PISO_CTRL_A_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_PISO_CTRL_A_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_PISO_CTRL_A_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000628 MABC_CONFIG_REG_4 |
 | +---------------------------------------+
 | | bit  31  R/W  MABC_PISO_PBUS_WIDTH_A  |
 | | bit  30  R/W  MABC_PISO_PBUS_WIDTH_B  |
 | | bit  9:0 R/W  MABC_RXCPGEN_MODE       |
 | +---------------------------------------+
 | 
 | void   t8_common_field_MABC_PISO_PBUS_WIDTH_A_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_MABC_PISO_PBUS_WIDTH_A_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_MABC_PISO_PBUS_WIDTH_B_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_MABC_PISO_PBUS_WIDTH_B_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_MABC_RXCPGEN_MODE_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_MABC_RXCPGEN_MODE_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_MABC_RXCPGEN_MODE_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_MABC_RXCPGEN_MODE_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000630 MTSB_CTRLR_CONFIG_REG_4 |
 | +---------------------------------------------+
 | | bit  7   R/W  RX_ADAPT_START_1              |
 | | bit  6   R/W  RX_ADAPT_START_0              |
 | | bit  5   R/W  ADC_CALIB_START_1             |
 | | bit  4   R/W  ADC_CALIB_START_0             |
 | | bit  3:2 R/W  ADAPT_MODE_1                  |
 | | bit  1:0 R/W  ADAPT_MODE_0                  |
 | +---------------------------------------------+
 | 
 | void   t8_common_field_RX_ADAPT_START_1_set   ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_RX_ADAPT_START_1_get   ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_RX_ADAPT_START_0_set   ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_RX_ADAPT_START_0_get   ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_ADC_CALIB_START_1_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_ADC_CALIB_START_1_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_ADC_CALIB_START_0_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_ADC_CALIB_START_0_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_ADAPT_MODE_1_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_ADAPT_MODE_1_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_ADAPT_MODE_1_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_ADAPT_MODE_1_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_ADAPT_MODE_0_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_ADAPT_MODE_0_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_ADAPT_MODE_0_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_ADAPT_MODE_0_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000634 MTSB_CTRLR_STATUS_REG1 |
 | +--------------------------------------------+
 | | bit  3 R  ADC_CALIB_DONE_OUT_1             |
 | | bit  2 R  ADC_CALIB_DONE_OUT_0             |
 | | bit  1 R  RX_ADAPT_DONE_1                  |
 | | bit  0 R  RX_ADAPT_DONE_0                  |
 | +--------------------------------------------+
 | 
 | UINT32 t8_common_field_ADC_CALIB_DONE_OUT_1_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | UINT32 t8_common_field_ADC_CALIB_DONE_OUT_0_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | UINT32 t8_common_field_RX_ADAPT_DONE_1_get      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | UINT32 t8_common_field_RX_ADAPT_DONE_0_get      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | +------------------------------------------------+
 | | Register 0x0000064c AMP_THR_TABLE_CONFIG_REG_1 |
 | +------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_5                |
 | | bit  24:20 R/W  AMP_THR_TABLE_4                |
 | | bit  19:15 R/W  AMP_THR_TABLE_3                |
 | | bit  14:10 R/W  AMP_THR_TABLE_2                |
 | | bit  9:5   R/W  AMP_THR_TABLE_1                |
 | | bit  4:0   R/W  AMP_THR_TABLE_0                |
 | +------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_5_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_5_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_5_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_5_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_4_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_4_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_4_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_4_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_3_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_3_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_3_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_3_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_2_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_2_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_2_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_2_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_1_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_1_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_1_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_1_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_0_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_0_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_0_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_0_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000650 AMP_THR_TABLE_CONFIG_REG_2 |
 | +------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_11               |
 | | bit  24:20 R/W  AMP_THR_TABLE_10               |
 | | bit  19:15 R/W  AMP_THR_TABLE_9                |
 | | bit  14:10 R/W  AMP_THR_TABLE_8                |
 | | bit  9:5   R/W  AMP_THR_TABLE_7                |
 | | bit  4:0   R/W  AMP_THR_TABLE_6                |
 | +------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_11_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_11_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_11_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_11_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_10_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_10_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_10_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_10_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_9_set        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_9_get        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_9_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_9_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_8_set        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_8_get        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_8_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_8_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_7_set        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_7_get        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_7_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_7_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_6_set        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_6_get        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_6_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_6_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000654 AMP_THR_TABLE_CONFIG_REG_3 |
 | +------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_17               |
 | | bit  24:20 R/W  AMP_THR_TABLE_16               |
 | | bit  19:15 R/W  AMP_THR_TABLE_15               |
 | | bit  14:10 R/W  AMP_THR_TABLE_14               |
 | | bit  9:5   R/W  AMP_THR_TABLE_13               |
 | | bit  4:0   R/W  AMP_THR_TABLE_12               |
 | +------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_17_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_17_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_17_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_17_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_16_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_16_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_16_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_16_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_15_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_15_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_15_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_15_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_14_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_14_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_14_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_14_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_13_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_13_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_13_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_13_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_12_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_12_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_12_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_12_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000658 AMP_THR_TABLE_CONFIG_REG_4 |
 | +------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_23               |
 | | bit  24:20 R/W  AMP_THR_TABLE_22               |
 | | bit  19:15 R/W  AMP_THR_TABLE_21               |
 | | bit  14:10 R/W  AMP_THR_TABLE_20               |
 | | bit  9:5   R/W  AMP_THR_TABLE_19               |
 | | bit  4:0   R/W  AMP_THR_TABLE_18               |
 | +------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_23_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_23_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_23_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_23_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_22_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_22_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_22_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_22_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_21_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_21_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_21_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_21_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_20_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_20_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_20_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_20_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_19_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_19_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_19_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_19_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_18_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_18_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_18_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_18_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x0000065c AMP_THR_TABLE_CONFIG_REG_5 |
 | +------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_29               |
 | | bit  24:20 R/W  AMP_THR_TABLE_28               |
 | | bit  19:15 R/W  AMP_THR_TABLE_27               |
 | | bit  14:10 R/W  AMP_THR_TABLE_26               |
 | | bit  9:5   R/W  AMP_THR_TABLE_25               |
 | | bit  4:0   R/W  AMP_THR_TABLE_24               |
 | +------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_29_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_29_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_29_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_29_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_28_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_28_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_28_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_28_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_27_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_27_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_27_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_27_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_26_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_26_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_26_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_26_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_25_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_25_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_25_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_25_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_24_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_24_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_24_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_24_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000660 AMP_THR_TABLE_CONFIG_REG_6 |
 | +------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_35               |
 | | bit  24:20 R/W  AMP_THR_TABLE_34               |
 | | bit  19:15 R/W  AMP_THR_TABLE_33               |
 | | bit  14:10 R/W  AMP_THR_TABLE_32               |
 | | bit  9:5   R/W  AMP_THR_TABLE_31               |
 | | bit  4:0   R/W  AMP_THR_TABLE_30               |
 | +------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_35_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_35_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_35_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_35_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_34_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_34_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_34_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_34_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_33_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_33_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_33_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_33_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_32_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_32_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_32_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_32_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_31_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_31_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_31_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_31_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_30_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_30_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_30_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_30_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000664 AMP_THR_TABLE_CONFIG_REG_7 |
 | +------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_41               |
 | | bit  24:20 R/W  AMP_THR_TABLE_40               |
 | | bit  19:15 R/W  AMP_THR_TABLE_39               |
 | | bit  14:10 R/W  AMP_THR_TABLE_38               |
 | | bit  9:5   R/W  AMP_THR_TABLE_37               |
 | | bit  4:0   R/W  AMP_THR_TABLE_36               |
 | +------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_41_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_41_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_41_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_41_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_40_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_40_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_40_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_40_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_39_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_39_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_39_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_39_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_38_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_38_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_38_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_38_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_37_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_37_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_37_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_37_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_36_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_36_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_36_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_36_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000668 AMP_THR_TABLE_CONFIG_REG_8 |
 | +------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_47               |
 | | bit  24:20 R/W  AMP_THR_TABLE_46               |
 | | bit  19:15 R/W  AMP_THR_TABLE_45               |
 | | bit  14:10 R/W  AMP_THR_TABLE_44               |
 | | bit  9:5   R/W  AMP_THR_TABLE_43               |
 | | bit  4:0   R/W  AMP_THR_TABLE_42               |
 | +------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_47_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_47_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_47_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_47_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_46_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_46_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_46_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_46_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_45_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_45_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_45_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_45_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_44_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_44_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_44_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_44_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_43_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_43_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_43_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_43_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_42_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_42_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_42_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_42_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x0000066c AMP_THR_TABLE_CONFIG_REG_9 |
 | +------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_53               |
 | | bit  24:20 R/W  AMP_THR_TABLE_52               |
 | | bit  19:15 R/W  AMP_THR_TABLE_51               |
 | | bit  14:10 R/W  AMP_THR_TABLE_50               |
 | | bit  9:5   R/W  AMP_THR_TABLE_49               |
 | | bit  4:0   R/W  AMP_THR_TABLE_48               |
 | +------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_53_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_53_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_53_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_53_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_52_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_52_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_52_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_52_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_51_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_51_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_51_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_51_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_50_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_50_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_50_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_50_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_49_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_49_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_49_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_49_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_48_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_48_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_48_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_48_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00000670 AMP_THR_TABLE_CONFIG_REG_10 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_59                |
 | | bit  24:20 R/W  AMP_THR_TABLE_58                |
 | | bit  19:15 R/W  AMP_THR_TABLE_57                |
 | | bit  14:10 R/W  AMP_THR_TABLE_56                |
 | | bit  9:5   R/W  AMP_THR_TABLE_55                |
 | | bit  4:0   R/W  AMP_THR_TABLE_54                |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_59_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_59_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_59_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_59_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_58_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_58_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_58_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_58_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_57_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_57_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_57_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_57_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_56_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_56_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_56_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_56_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_55_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_55_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_55_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_55_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_54_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_54_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_54_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_54_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00000674 AMP_THR_TABLE_CONFIG_REG_11 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_65                |
 | | bit  24:20 R/W  AMP_THR_TABLE_64                |
 | | bit  19:15 R/W  AMP_THR_TABLE_63                |
 | | bit  14:10 R/W  AMP_THR_TABLE_62                |
 | | bit  9:5   R/W  AMP_THR_TABLE_61                |
 | | bit  4:0   R/W  AMP_THR_TABLE_60                |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_65_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_65_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_65_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_65_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_64_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_64_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_64_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_64_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_63_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_63_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_63_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_63_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_62_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_62_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_62_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_62_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_61_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_61_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_61_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_61_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_60_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_60_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_60_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_60_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00000678 AMP_THR_TABLE_CONFIG_REG_12 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_71                |
 | | bit  24:20 R/W  AMP_THR_TABLE_70                |
 | | bit  19:15 R/W  AMP_THR_TABLE_69                |
 | | bit  14:10 R/W  AMP_THR_TABLE_68                |
 | | bit  9:5   R/W  AMP_THR_TABLE_67                |
 | | bit  4:0   R/W  AMP_THR_TABLE_66                |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_71_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_71_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_71_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_71_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_70_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_70_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_70_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_70_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_69_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_69_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_69_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_69_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_68_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_68_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_68_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_68_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_67_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_67_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_67_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_67_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_66_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_66_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_66_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_66_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x0000067c AMP_THR_TABLE_CONFIG_REG_13 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_77                |
 | | bit  24:20 R/W  AMP_THR_TABLE_76                |
 | | bit  19:15 R/W  AMP_THR_TABLE_75                |
 | | bit  14:10 R/W  AMP_THR_TABLE_74                |
 | | bit  9:5   R/W  AMP_THR_TABLE_73                |
 | | bit  4:0   R/W  AMP_THR_TABLE_72                |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_77_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_77_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_77_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_77_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_76_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_76_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_76_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_76_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_75_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_75_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_75_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_75_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_74_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_74_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_74_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_74_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_73_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_73_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_73_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_73_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_72_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_72_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_72_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_72_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00000680 AMP_THR_TABLE_CONFIG_REG_14 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_83                |
 | | bit  24:20 R/W  AMP_THR_TABLE_82                |
 | | bit  19:15 R/W  AMP_THR_TABLE_81                |
 | | bit  14:10 R/W  AMP_THR_TABLE_80                |
 | | bit  9:5   R/W  AMP_THR_TABLE_79                |
 | | bit  4:0   R/W  AMP_THR_TABLE_78                |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_83_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_83_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_83_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_83_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_82_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_82_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_82_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_82_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_81_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_81_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_81_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_81_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_80_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_80_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_80_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_80_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_79_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_79_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_79_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_79_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_78_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_78_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_78_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_78_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00000684 AMP_THR_TABLE_CONFIG_REG_15 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_89                |
 | | bit  24:20 R/W  AMP_THR_TABLE_88                |
 | | bit  19:15 R/W  AMP_THR_TABLE_87                |
 | | bit  14:10 R/W  AMP_THR_TABLE_86                |
 | | bit  9:5   R/W  AMP_THR_TABLE_85                |
 | | bit  4:0   R/W  AMP_THR_TABLE_84                |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_89_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_89_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_89_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_89_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_88_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_88_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_88_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_88_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_87_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_87_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_87_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_87_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_86_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_86_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_86_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_86_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_85_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_85_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_85_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_85_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_84_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_84_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_84_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_84_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00000688 AMP_THR_TABLE_CONFIG_REG_16 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_95                |
 | | bit  24:20 R/W  AMP_THR_TABLE_94                |
 | | bit  19:15 R/W  AMP_THR_TABLE_93                |
 | | bit  14:10 R/W  AMP_THR_TABLE_92                |
 | | bit  9:5   R/W  AMP_THR_TABLE_91                |
 | | bit  4:0   R/W  AMP_THR_TABLE_90                |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_95_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_95_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_95_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_95_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_94_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_94_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_94_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_94_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_93_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_93_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_93_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_93_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_92_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_92_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_92_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_92_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_91_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_91_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_91_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_91_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_90_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_90_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_90_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_90_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x0000068c AMP_THR_TABLE_CONFIG_REG_17 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_101               |
 | | bit  24:20 R/W  AMP_THR_TABLE_100               |
 | | bit  19:15 R/W  AMP_THR_TABLE_99                |
 | | bit  14:10 R/W  AMP_THR_TABLE_98                |
 | | bit  9:5   R/W  AMP_THR_TABLE_97                |
 | | bit  4:0   R/W  AMP_THR_TABLE_96                |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_101_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_101_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_101_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_101_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_100_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_100_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_100_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_100_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_99_set        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_99_get        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_99_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_99_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_98_set        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_98_get        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_98_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_98_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_97_set        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_97_get        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_97_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_97_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_96_set        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_96_get        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_96_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_96_get  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00000690 AMP_THR_TABLE_CONFIG_REG_18 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_107               |
 | | bit  24:20 R/W  AMP_THR_TABLE_106               |
 | | bit  19:15 R/W  AMP_THR_TABLE_105               |
 | | bit  14:10 R/W  AMP_THR_TABLE_104               |
 | | bit  9:5   R/W  AMP_THR_TABLE_103               |
 | | bit  4:0   R/W  AMP_THR_TABLE_102               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_107_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_107_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_107_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_107_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_106_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_106_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_106_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_106_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_105_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_105_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_105_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_105_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_104_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_104_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_104_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_104_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_103_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_103_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_103_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_103_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_102_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_102_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_102_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_102_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00000694 AMP_THR_TABLE_CONFIG_REG_19 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_113               |
 | | bit  24:20 R/W  AMP_THR_TABLE_112               |
 | | bit  19:15 R/W  AMP_THR_TABLE_111               |
 | | bit  14:10 R/W  AMP_THR_TABLE_110               |
 | | bit  9:5   R/W  AMP_THR_TABLE_109               |
 | | bit  4:0   R/W  AMP_THR_TABLE_108               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_113_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_113_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_113_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_113_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_112_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_112_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_112_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_112_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_111_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_111_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_111_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_111_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_110_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_110_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_110_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_110_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_109_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_109_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_109_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_109_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_108_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_108_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_108_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_108_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00000698 AMP_THR_TABLE_CONFIG_REG_20 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_119               |
 | | bit  24:20 R/W  AMP_THR_TABLE_118               |
 | | bit  19:15 R/W  AMP_THR_TABLE_117               |
 | | bit  14:10 R/W  AMP_THR_TABLE_116               |
 | | bit  9:5   R/W  AMP_THR_TABLE_115               |
 | | bit  4:0   R/W  AMP_THR_TABLE_114               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_119_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_119_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_119_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_119_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_118_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_118_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_118_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_118_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_117_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_117_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_117_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_117_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_116_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_116_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_116_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_116_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_115_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_115_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_115_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_115_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_114_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_114_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_114_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_114_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x0000069c AMP_THR_TABLE_CONFIG_REG_21 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_125               |
 | | bit  24:20 R/W  AMP_THR_TABLE_124               |
 | | bit  19:15 R/W  AMP_THR_TABLE_123               |
 | | bit  14:10 R/W  AMP_THR_TABLE_122               |
 | | bit  9:5   R/W  AMP_THR_TABLE_121               |
 | | bit  4:0   R/W  AMP_THR_TABLE_120               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_125_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_125_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_125_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_125_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_124_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_124_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_124_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_124_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_123_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_123_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_123_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_123_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_122_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_122_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_122_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_122_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_121_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_121_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_121_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_121_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_120_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_120_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_120_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_120_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000006a0 AMP_THR_TABLE_CONFIG_REG_22 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_131               |
 | | bit  24:20 R/W  AMP_THR_TABLE_130               |
 | | bit  19:15 R/W  AMP_THR_TABLE_129               |
 | | bit  14:10 R/W  AMP_THR_TABLE_128               |
 | | bit  9:5   R/W  AMP_THR_TABLE_127               |
 | | bit  4:0   R/W  AMP_THR_TABLE_126               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_131_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_131_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_131_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_131_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_130_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_130_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_130_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_130_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_129_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_129_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_129_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_129_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_128_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_128_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_128_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_128_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_127_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_127_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_127_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_127_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_126_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_126_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_126_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_126_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000006a4 AMP_THR_TABLE_CONFIG_REG_23 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_137               |
 | | bit  24:20 R/W  AMP_THR_TABLE_136               |
 | | bit  19:15 R/W  AMP_THR_TABLE_135               |
 | | bit  14:10 R/W  AMP_THR_TABLE_134               |
 | | bit  9:5   R/W  AMP_THR_TABLE_133               |
 | | bit  4:0   R/W  AMP_THR_TABLE_132               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_137_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_137_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_137_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_137_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_136_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_136_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_136_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_136_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_135_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_135_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_135_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_135_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_134_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_134_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_134_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_134_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_133_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_133_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_133_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_133_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_132_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_132_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_132_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_132_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000006a8 AMP_THR_TABLE_CONFIG_REG_24 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_143               |
 | | bit  24:20 R/W  AMP_THR_TABLE_142               |
 | | bit  19:15 R/W  AMP_THR_TABLE_141               |
 | | bit  14:10 R/W  AMP_THR_TABLE_140               |
 | | bit  9:5   R/W  AMP_THR_TABLE_139               |
 | | bit  4:0   R/W  AMP_THR_TABLE_138               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_143_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_143_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_143_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_143_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_142_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_142_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_142_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_142_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_141_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_141_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_141_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_141_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_140_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_140_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_140_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_140_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_139_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_139_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_139_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_139_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_138_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_138_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_138_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_138_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000006ac AMP_THR_TABLE_CONFIG_REG_25 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_149               |
 | | bit  24:20 R/W  AMP_THR_TABLE_148               |
 | | bit  19:15 R/W  AMP_THR_TABLE_147               |
 | | bit  14:10 R/W  AMP_THR_TABLE_146               |
 | | bit  9:5   R/W  AMP_THR_TABLE_145               |
 | | bit  4:0   R/W  AMP_THR_TABLE_144               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_149_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_149_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_149_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_149_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_148_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_148_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_148_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_148_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_147_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_147_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_147_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_147_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_146_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_146_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_146_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_146_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_145_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_145_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_145_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_145_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_144_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_144_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_144_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_144_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000006b0 AMP_THR_TABLE_CONFIG_REG_26 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_155               |
 | | bit  24:20 R/W  AMP_THR_TABLE_154               |
 | | bit  19:15 R/W  AMP_THR_TABLE_153               |
 | | bit  14:10 R/W  AMP_THR_TABLE_152               |
 | | bit  9:5   R/W  AMP_THR_TABLE_151               |
 | | bit  4:0   R/W  AMP_THR_TABLE_150               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_155_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_155_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_155_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_155_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_154_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_154_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_154_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_154_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_153_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_153_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_153_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_153_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_152_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_152_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_152_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_152_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_151_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_151_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_151_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_151_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_150_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_150_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_150_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_150_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000006b4 AMP_THR_TABLE_CONFIG_REG_27 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_161               |
 | | bit  24:20 R/W  AMP_THR_TABLE_160               |
 | | bit  19:15 R/W  AMP_THR_TABLE_159               |
 | | bit  14:10 R/W  AMP_THR_TABLE_158               |
 | | bit  9:5   R/W  AMP_THR_TABLE_157               |
 | | bit  4:0   R/W  AMP_THR_TABLE_156               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_161_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_161_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_161_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_161_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_160_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_160_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_160_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_160_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_159_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_159_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_159_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_159_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_158_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_158_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_158_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_158_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_157_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_157_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_157_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_157_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_156_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_156_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_156_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_156_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000006b8 AMP_THR_TABLE_CONFIG_REG_28 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_167               |
 | | bit  24:20 R/W  AMP_THR_TABLE_166               |
 | | bit  19:15 R/W  AMP_THR_TABLE_165               |
 | | bit  14:10 R/W  AMP_THR_TABLE_164               |
 | | bit  9:5   R/W  AMP_THR_TABLE_163               |
 | | bit  4:0   R/W  AMP_THR_TABLE_162               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_167_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_167_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_167_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_167_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_166_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_166_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_166_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_166_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_165_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_165_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_165_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_165_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_164_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_164_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_164_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_164_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_163_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_163_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_163_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_163_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_162_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_162_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_162_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_162_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000006bc AMP_THR_TABLE_CONFIG_REG_29 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_173               |
 | | bit  24:20 R/W  AMP_THR_TABLE_172               |
 | | bit  19:15 R/W  AMP_THR_TABLE_171               |
 | | bit  14:10 R/W  AMP_THR_TABLE_170               |
 | | bit  9:5   R/W  AMP_THR_TABLE_169               |
 | | bit  4:0   R/W  AMP_THR_TABLE_168               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_173_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_173_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_173_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_173_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_172_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_172_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_172_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_172_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_171_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_171_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_171_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_171_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_170_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_170_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_170_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_170_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_169_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_169_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_169_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_169_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_168_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_168_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_168_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_168_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000006c0 AMP_THR_TABLE_CONFIG_REG_30 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_179               |
 | | bit  24:20 R/W  AMP_THR_TABLE_178               |
 | | bit  19:15 R/W  AMP_THR_TABLE_177               |
 | | bit  14:10 R/W  AMP_THR_TABLE_176               |
 | | bit  9:5   R/W  AMP_THR_TABLE_175               |
 | | bit  4:0   R/W  AMP_THR_TABLE_174               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_179_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_179_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_179_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_179_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_178_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_178_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_178_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_178_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_177_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_177_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_177_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_177_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_176_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_176_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_176_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_176_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_175_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_175_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_175_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_175_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_174_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_174_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_174_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_174_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000006c4 AMP_THR_TABLE_CONFIG_REG_31 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_185               |
 | | bit  24:20 R/W  AMP_THR_TABLE_184               |
 | | bit  19:15 R/W  AMP_THR_TABLE_183               |
 | | bit  14:10 R/W  AMP_THR_TABLE_182               |
 | | bit  9:5   R/W  AMP_THR_TABLE_181               |
 | | bit  4:0   R/W  AMP_THR_TABLE_180               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_185_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_185_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_185_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_185_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_184_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_184_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_184_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_184_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_183_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_183_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_183_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_183_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_182_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_182_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_182_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_182_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_181_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_181_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_181_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_181_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_180_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_180_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_180_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_180_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000006c8 AMP_THR_TABLE_CONFIG_REG_32 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_191               |
 | | bit  24:20 R/W  AMP_THR_TABLE_190               |
 | | bit  19:15 R/W  AMP_THR_TABLE_189               |
 | | bit  14:10 R/W  AMP_THR_TABLE_188               |
 | | bit  9:5   R/W  AMP_THR_TABLE_187               |
 | | bit  4:0   R/W  AMP_THR_TABLE_186               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_191_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_191_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_191_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_191_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_190_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_190_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_190_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_190_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_189_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_189_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_189_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_189_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_188_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_188_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_188_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_188_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_187_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_187_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_187_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_187_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_186_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_186_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_186_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_186_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000006cc AMP_THR_TABLE_CONFIG_REG_33 |
 | +-------------------------------------------------+
 | | bit  29:25 R/W  AMP_THR_TABLE_197               |
 | | bit  24:20 R/W  AMP_THR_TABLE_196               |
 | | bit  19:15 R/W  AMP_THR_TABLE_195               |
 | | bit  14:10 R/W  AMP_THR_TABLE_194               |
 | | bit  9:5   R/W  AMP_THR_TABLE_193               |
 | | bit  4:0   R/W  AMP_THR_TABLE_192               |
 | +-------------------------------------------------+
 | 
 | void   t8_common_field_AMP_THR_TABLE_197_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_197_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_197_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_197_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_196_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_196_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_196_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_196_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_195_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_195_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_195_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_195_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_194_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_194_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_194_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_194_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_193_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_193_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_193_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_193_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   t8_common_field_AMP_THR_TABLE_192_set       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | UINT32 t8_common_field_AMP_THR_TABLE_192_get       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void   t8_common_field_range_AMP_THR_TABLE_192_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 t8_common_field_range_AMP_THR_TABLE_192_get ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _T8_COMMON_IO_H
#define _T8_COMMON_IO_H

#include "t8.h"
#include "t8_common_regs.h"
#include "t8_common_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 t8_common_buffer_init               ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | void                 t8_common_buffer_flush              ( t8_common_buffer_t *b_ptr )
 | UINT32               t8_common_reg_read                  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 t8_common_reg_write                 ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 t8_common_field_set                 ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 t8_common_action_on_write_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 t8_common_burst_read                ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 t8_common_burst_write               ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE t8_common_poll                      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   t8_common_reg_RAM_CAPT_CONFIG_REG_write             ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_RAM_CAPT_CONFIG_REG_field_set         ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_RAM_CAPT_CONFIG_REG_read              ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_RST_REG_write                         ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_RST_REG_field_set                     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_RST_REG_read                          ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_PCIE3_CFG_write                       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_PCIE3_CFG_field_set                   ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_PCIE3_CFG_read                        ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_MABC_CONFIG_REG_1_write               ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_MABC_CONFIG_REG_1_field_set           ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_MABC_CONFIG_REG_1_read                ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_MABC_CONFIG_REG_2_write               ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_MABC_CONFIG_REG_2_field_set           ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_MABC_CONFIG_REG_2_read                ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_MABC_CONFIG_REG_3_write               ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_MABC_CONFIG_REG_3_field_set           ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_MABC_CONFIG_REG_3_read                ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_MABC_CONFIG_REG_4_write               ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_MABC_CONFIG_REG_4_field_set           ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_MABC_CONFIG_REG_4_read                ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_MTSB_CTRLR_CONFIG_REG_4_write         ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_MTSB_CTRLR_CONFIG_REG_4_field_set     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_MTSB_CTRLR_CONFIG_REG_4_read          ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_1_write      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_1_field_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_1_read       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_2_write      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_2_field_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_2_read       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_3_write      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_3_field_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_3_read       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_4_write      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_4_field_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_4_read       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_5_write      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_5_field_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_5_read       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_6_write      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_6_field_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_6_read       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_7_write      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_7_field_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_7_read       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_8_write      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_8_field_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_8_read       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_9_write      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_9_field_set  ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_9_read       ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_10_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_10_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_10_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_11_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_11_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_11_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_12_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_12_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_12_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_13_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_13_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_13_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_14_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_14_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_14_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_15_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_15_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_15_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_16_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_16_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_16_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_17_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_17_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_17_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_18_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_18_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_18_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_19_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_19_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_19_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_20_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_20_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_20_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_21_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_21_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_21_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_22_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_22_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_22_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_23_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_23_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_23_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_24_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_24_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_24_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_25_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_25_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_25_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_26_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_26_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_26_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_27_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_27_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_27_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_28_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_28_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_28_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_29_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_29_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_29_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_30_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_30_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_30_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_31_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_31_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_31_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_32_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_32_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_32_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_33_write     ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 value )
 | void   t8_common_reg_AMP_THR_TABLE_CONFIG_REG_33_field_set ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 t8_common_reg_AMP_THR_TABLE_CONFIG_REG_33_read      ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | UINT32 t8_common_reg_RAM_CAP_STATUS_REG_read               ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 | UINT32 t8_common_reg_MTSB_CTRLR_STATUS_REG1_read           ( t8_common_buffer_t *b_ptr, t8_handle_t *h_ptr )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _T8_COMMON_IO_H */
