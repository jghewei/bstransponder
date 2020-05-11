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
 *     and register accessor functions for the c8 block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing c8 io functions is:
 | {
 |     c8_buffer_t b_ptr[1];
 |     c8_buffer_init( b_ptr, h_ptr, A );
 |     value = c8_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     c8_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     c8_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = c8_field_<FIELD>_get( NULL, h_ptr, A );
 | c8_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-------------------------------------+
 | | Register 0x00000000 DIGITAL_RST_REG |
 | +-------------------------------------+
 | | bit  31 R/W  GLBL_DRSTB             |
 | | bit  16 R/W  PMM_WRAP_RSTB          |
 | | bit  15 R/W  PLL_LOCK_DET_SRST      |
 | | bit  14 R/W  PATT_MON_RSTB          |
 | | bit  13 R/W  PATT_GEN_RSTB          |
 | | bit  12 R/W  SIPO_BIST_RSTB         |
 | | bit  11 R/W  FIFO_HW_RESET          |
 | | bit  10 R/W  DSKW_FIFO_RSTB         |
 | | bit  9  R/W  DSIPO_RSTB             |
 | | bit  8  R/W  DPISO_RSTB             |
 | | bit  7  R/W  CTRL_RSTB              |
 | | bit  6  R/W  MDSP_RSTB              |
 | | bit  5  R/W  ADPT_RSTB              |
 | | bit  4  R/W  DLOS_RSTB              |
 | | bit  3  R/W  DIAG_RSTB              |
 | | bit  2  R/W  SSI_RSTB               |
 | | bit  1  R/W  GCOC_RSTB              |
 | | bit  0  R/W  LOCK_DET_RSTB          |
 | +-------------------------------------+
 | 
 | void   c8_field_GLBL_DRSTB_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_GLBL_DRSTB_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PMM_WRAP_RSTB_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PMM_WRAP_RSTB_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PLL_LOCK_DET_SRST_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PLL_LOCK_DET_SRST_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PATT_MON_RSTB_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PATT_MON_RSTB_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PATT_GEN_RSTB_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PATT_GEN_RSTB_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_SIPO_BIST_RSTB_set    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_SIPO_BIST_RSTB_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_FIFO_HW_RESET_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_FIFO_HW_RESET_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_DSKW_FIFO_RSTB_set    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DSKW_FIFO_RSTB_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_DSIPO_RSTB_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DSIPO_RSTB_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_DPISO_RSTB_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DPISO_RSTB_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_CTRL_RSTB_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_CTRL_RSTB_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_MDSP_RSTB_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_MDSP_RSTB_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ADPT_RSTB_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADPT_RSTB_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_DLOS_RSTB_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DLOS_RSTB_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_DIAG_RSTB_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DIAG_RSTB_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_SSI_RSTB_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_SSI_RSTB_get          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_GCOC_RSTB_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_GCOC_RSTB_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_LOCK_DET_RSTB_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_LOCK_DET_RSTB_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------+
 | | Register 0x00000004 MABC_RST_CNFG_REG |
 | +---------------------------------------+
 | | bit  4 R/W  RXCPGEN_ARSTB             |
 | | bit  3 R/W  RXCPGEN_RSTB              |
 | | bit  2 R/W  PISO_RSTB                 |
 | | bit  1 R/W  ADC_RSTB                  |
 | | bit  0 R/W  ADC_DAC_RSTB              |
 | +---------------------------------------+
 | 
 | void   c8_field_RXCPGEN_ARSTB_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_ARSTB_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXCPGEN_RSTB_set  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_RSTB_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PISO_RSTB_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_RSTB_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ADC_RSTB_set      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADC_RSTB_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ADC_DAC_RSTB_set  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADC_DAC_RSTB_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------+
 | | Register 0x00000008 CLK_DGLCH_CNFG_REG |
 | +----------------------------------------+
 | | bit  3 R/W  DGLT_PCLK_EN               |
 | | bit  2 R/W  DGLT_TPCOUT_EN             |
 | | bit  1 R/W  DGLT_BADJ_EN               |
 | | bit  0 R/W  DGLCH_ADC_CLK_EN           |
 | +----------------------------------------+
 | 
 | void   c8_field_DGLT_PCLK_EN_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DGLT_PCLK_EN_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_DGLT_TPCOUT_EN_set   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DGLT_TPCOUT_EN_get   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_DGLT_BADJ_EN_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DGLT_BADJ_EN_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_DGLCH_ADC_CLK_EN_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DGLCH_ADC_CLK_EN_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x00000010 PLL_CNFG_REG1 |
 | +-----------------------------------+
 | | bit  8 R/W  LOAD_CLK_CNTRS        |
 | | bit  4 R/W  RESET_HISTORY         |
 | | bit  0 R/W  PPM_OFF_RD_REQ        |
 | +-----------------------------------+
 | 
 | void   c8_field_LOAD_CLK_CNTRS_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_LOAD_CLK_CNTRS_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RESET_HISTORY_set  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RESET_HISTORY_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PPM_OFF_RD_REQ_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PPM_OFF_RD_REQ_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x00000014 PLL_CNFG_REG2 |
 | +-----------------------------------+
 | | bit  3:0 R/W  LOCK_PRECISION      |
 | +-----------------------------------+
 | 
 | void   c8_field_LOCK_PRECISION_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_LOCK_PRECISION_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_LOCK_PRECISION_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_LOCK_PRECISION_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000018 PLL_CNFG_REG3 |
 | +-----------------------------------+
 | | bit  19:0 R/W  CLK_CNT_MAX        |
 | +-----------------------------------+
 | 
 | void   c8_field_CLK_CNT_MAX_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_CLK_CNT_MAX_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_CLK_CNT_MAX_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_CLK_CNT_MAX_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x0000001c PLL_CNFG_REG4 |
 | +-----------------------------------+
 | | bit  31:16 R/W  FAIL_CNT_MAX      |
 | | bit  15:0  R/W  PASS_CNT_MAX      |
 | +-----------------------------------+
 | 
 | void   c8_field_FAIL_CNT_MAX_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_FAIL_CNT_MAX_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_FAIL_CNT_MAX_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_FAIL_CNT_MAX_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_PASS_CNT_MAX_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PASS_CNT_MAX_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_PASS_CNT_MAX_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_PASS_CNT_MAX_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000020 PLL_CNFG_REG5 |
 | +-----------------------------------+
 | | bit  23:16 R/W  LOCK_CNT_MAX      |
 | +-----------------------------------+
 | 
 | void   c8_field_LOCK_CNT_MAX_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_LOCK_CNT_MAX_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_LOCK_CNT_MAX_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_LOCK_CNT_MAX_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000024 PLL_STS_REG |
 | +---------------------------------+
 | | bit  31   R  PPM_OFF_RD_SIGN    |
 | | bit  20:0 R  PPM_OFF_RD_VAL     |
 | +---------------------------------+
 | 
 | UINT32 c8_field_PPM_OFF_RD_SIGN_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_field_PPM_OFF_RD_VAL_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_field_range_PPM_OFF_RD_VAL_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x0000002c GCOC_CNFG_REG1 |
 | +------------------------------------+
 | | bit  4 R/W  MABC_INT_OVR_EN        |
 | | bit  3 R/W  OFFSET_EN              |
 | | bit  2 R/W  CALIB_PROG_EN          |
 | | bit  1 R/W  ADC_WR_OVR             |
 | | bit  0 R/W  CALIB                  |
 | +------------------------------------+
 | 
 | void   c8_field_MABC_INT_OVR_EN_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_MABC_INT_OVR_EN_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_OFFSET_EN_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_OFFSET_EN_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_CALIB_PROG_EN_set   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_CALIB_PROG_EN_get   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ADC_WR_OVR_set      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADC_WR_OVR_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_CALIB_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_CALIB_get           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------+
 | | Register 0x00000030 GCOC_CNFG_REG2 |
 | +------------------------------------+
 | | bit  13:10 R/W  ACC_DELAY          |
 | | bit  9:0   R/W  DAC_DELAY          |
 | +------------------------------------+
 | 
 | void   c8_field_ACC_DELAY_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ACC_DELAY_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_ACC_DELAY_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_ACC_DELAY_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_DAC_DELAY_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DAC_DELAY_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_DAC_DELAY_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_DAC_DELAY_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00000034 GCOC_CNFG_REG3 |
 | +------------------------------------+
 | | bit  31:0 R/W  THR_MAP             |
 | +------------------------------------+
 | 
 | void   c8_field_THR_MAP_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_THR_MAP_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_THR_MAP_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_THR_MAP_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00000038 GCOC_CNFG_REG4 |
 | +------------------------------------+
 | | bit  31:24 R/W  ADC_A_OVR          |
 | | bit  23:16 R/W  ADC_D_OVR          |
 | | bit  13:0  R/W  N_ACCUM_CYCLES     |
 | +------------------------------------+
 | 
 | void   c8_field_ADC_A_OVR_set            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADC_A_OVR_get            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_ADC_A_OVR_set      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_ADC_A_OVR_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_ADC_D_OVR_set            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADC_D_OVR_get            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_ADC_D_OVR_set      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_ADC_D_OVR_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_N_ACCUM_CYCLES_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_N_ACCUM_CYCLES_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_N_ACCUM_CYCLES_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_N_ACCUM_CYCLES_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000040 MDSP_CNFG_REG |
 | +-----------------------------------+
 | | bit  1:0 R/W  DECIM_FACT          |
 | +-----------------------------------+
 | 
 | void   c8_field_DECIM_FACT_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DECIM_FACT_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_DECIM_FACT_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_DECIM_FACT_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000044 CTRL_CNFG_REG |
 | +-----------------------------------+
 | | bit  0 R/W  RX_ADAPT_START        |
 | +-----------------------------------+
 | 
 | void   c8_field_RX_ADAPT_START_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RX_ADAPT_START_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------+
 | | Register 0x00000048 DLPBK_CNFG_REG |
 | +------------------------------------+
 | | bit  0 R/W  DLPBK_EN               |
 | +------------------------------------+
 | 
 | void   c8_field_DLPBK_EN_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DLPBK_EN_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x00000050 SIPO_CNFG_REG |
 | +-----------------------------------+
 | | bit  2:1 R/W  SIPO_DIV_RATIO      |
 | | bit  0   R/W  SIPO_BUS_WIDTH      |
 | +-----------------------------------+
 | 
 | void   c8_field_SIPO_DIV_RATIO_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_SIPO_DIV_RATIO_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_SIPO_DIV_RATIO_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_SIPO_DIV_RATIO_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_SIPO_BUS_WIDTH_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_SIPO_BUS_WIDTH_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x00000058 PISO_CNFG_REG |
 | +-----------------------------------+
 | | bit  2:1 R/W  PISO_DIV_RATIO      |
 | | bit  0   R/W  PISO_BUS_WIDTH      |
 | +-----------------------------------+
 | 
 | void   c8_field_PISO_DIV_RATIO_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_DIV_RATIO_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_PISO_DIV_RATIO_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_PISO_DIV_RATIO_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_PISO_BUS_WIDTH_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_BUS_WIDTH_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------+
 | | Register 0x00000060 EFUSE_CNFG_REG |
 | +------------------------------------+
 | | bit  1:0 R/W  RAM_EXT_INT_SEL      |
 | +------------------------------------+
 | 
 | void   c8_field_RAM_EXT_INT_SEL_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RAM_EXT_INT_SEL_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_RAM_EXT_INT_SEL_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_RAM_EXT_INT_SEL_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0000006c DESKEW_FIFO_CNFG_REG |
 | +------------------------------------------+
 | | bit  3:1 R/W  RD_PTR_SYNC_VAL            |
 | | bit  0   R/W  SYNC                       |
 | +------------------------------------------+
 | 
 | void   c8_field_RD_PTR_SYNC_VAL_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RD_PTR_SYNC_VAL_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_RD_PTR_SYNC_VAL_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_RD_PTR_SYNC_VAL_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_SYNC_set                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_SYNC_get                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------+
 | | Register 0x00000074 SSI_CNFG_REG1    |
 | +--------------------------------------+
 | | bit  31:30 R/W  REL_AMP              |
 | | bit  27:24 R/W  SSI_ACC_CNT          |
 | | bit  23:10 R/W  SSI_LIMIT            |
 | | bit  9     R/W  SSI_OVR_EN           |
 | | bit  8     R/W  SSI_OVR              |
 | | bit  7:3   R/W  THRESHOLD_OVR        |
 | | bit  2     R/W  THRESHOLD_OVR_EN     |
 | | bit  1     R/W  ADJ_THR_RELEASES_SSI |
 | | bit  0     R/W  ADJUST_THRESHOLD     |
 | +--------------------------------------+
 | 
 | void   c8_field_REL_AMP_set              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_REL_AMP_get              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_REL_AMP_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_REL_AMP_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_SSI_ACC_CNT_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_SSI_ACC_CNT_get          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_SSI_ACC_CNT_set    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_SSI_ACC_CNT_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_SSI_LIMIT_set            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_SSI_LIMIT_get            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_SSI_LIMIT_set      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_SSI_LIMIT_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_SSI_OVR_EN_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_SSI_OVR_EN_get           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_SSI_OVR_set              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_SSI_OVR_get              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_THRESHOLD_OVR_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_THRESHOLD_OVR_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_THRESHOLD_OVR_set  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_THRESHOLD_OVR_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_THRESHOLD_OVR_EN_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_THRESHOLD_OVR_EN_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ADJ_THR_RELEASES_SSI_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADJ_THR_RELEASES_SSI_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ADJUST_THRESHOLD_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADJUST_THRESHOLD_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x00000078 SSI_CNFG_REG2 |
 | +-----------------------------------+
 | | bit  4:0 R/W  MIN_THRESHOLD       |
 | +-----------------------------------+
 | 
 | void   c8_field_MIN_THRESHOLD_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_MIN_THRESHOLD_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_MIN_THRESHOLD_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_MIN_THRESHOLD_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x0000007c SSI_STATUS_REG |
 | +------------------------------------+
 | | bit  4:0 R  CURR_THRESHOLD         |
 | +------------------------------------+
 | 
 | UINT32 c8_field_CURR_THRESHOLD_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_field_range_CURR_THRESHOLD_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000080 DLOS_CNFG_REG |
 | +-----------------------------------+
 | | bit  18:11 R/W  LOS_LIMIT         |
 | | bit  10:7  R/W  LOS_ACC_CNT       |
 | | bit  5:3   R/W  AMP_THR           |
 | | bit  2     R/W  DLOS_OVR_EN       |
 | | bit  1     R/W  DLOS_OVR          |
 | | bit  0     R/W  ONE_COMP_EN       |
 | +-----------------------------------+
 | 
 | void   c8_field_LOS_LIMIT_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_LOS_LIMIT_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_LOS_LIMIT_set   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_LOS_LIMIT_get   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_LOS_ACC_CNT_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_LOS_ACC_CNT_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_LOS_ACC_CNT_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_LOS_ACC_CNT_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_AMP_THR_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_AMP_THR_get           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_AMP_THR_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_AMP_THR_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_DLOS_OVR_EN_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DLOS_OVR_EN_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_DLOS_OVR_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DLOS_OVR_get          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ONE_COMP_EN_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ONE_COMP_EN_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------+
 | | Register 0x0000008c MABC_ADC_STS_REG |
 | +--------------------------------------+
 | | bit  7:0 R  ADC_Q                    |
 | +--------------------------------------+
 | 
 | UINT32 c8_field_ADC_Q_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_field_range_ADC_Q_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000090 MABC_ADC_CNFG_REG |
 | +---------------------------------------+
 | | bit  27:20 R/W  ADC_MODE              |
 | | bit  12:10 R/W  ADC_ENB               |
 | | bit  4     R/W  ADC_RD                |
 | | bit  3     R/W  ADC_DLB_EN            |
 | | bit  2     R/W  ADC_DAC_ENB           |
 | | bit  1     R/W  ADC_BIST              |
 | | bit  0     R/W  ADC_ATMSB             |
 | +---------------------------------------+
 | 
 | void   c8_field_ADC_MODE_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADC_MODE_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_ADC_MODE_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_ADC_MODE_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_ADC_ENB_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADC_ENB_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_ADC_ENB_set  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_ADC_ENB_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_ADC_RD_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADC_RD_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ADC_DLB_EN_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADC_DLB_EN_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ADC_DAC_ENB_set    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADC_DAC_ENB_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ADC_BIST_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADC_BIST_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ADC_ATMSB_set      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADC_ATMSB_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x00000094 MABC_CNFG_REG |
 | +-----------------------------------+
 | | bit  0 R/W  BADJ_ENB              |
 | +-----------------------------------+
 | 
 | void   c8_field_BADJ_ENB_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_BADJ_ENB_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------+
 | | Register 0x00000098 MABC_PISO_CNFG_REG1 |
 | +-----------------------------------------+
 | | bit  4 R/W  PISO_PBUS_WIDTH             |
 | | bit  3 R/W  PISO_ENB                    |
 | | bit  2 R/W  PISO_DLB_EN                 |
 | | bit  1 R/W  PISO_DIV_ENABLE             |
 | | bit  0 R/W  PISO_ALTPAT_EN              |
 | +-----------------------------------------+
 | 
 | void   c8_field_PISO_PBUS_WIDTH_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_PBUS_WIDTH_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PISO_ENB_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_ENB_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PISO_DLB_EN_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_DLB_EN_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PISO_DIV_ENABLE_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_DIV_ENABLE_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PISO_ALTPAT_EN_set  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_ALTPAT_EN_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------+
 | | Register 0x0000009c MABC_PISO_CNFG_REG2 |
 | +-----------------------------------------+
 | | bit  7:5 R/W  PISO_CTRL_7_5             |
 | | bit  4:3 R/W  PISO_CTRL_4_3             |
 | | bit  2   R/W  PISO_CTRL_2               |
 | | bit  1:0 R/W  PISO_CTRL_1_0             |
 | +-----------------------------------------+
 | 
 | void   c8_field_PISO_CTRL_7_5_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_CTRL_7_5_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_PISO_CTRL_7_5_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_PISO_CTRL_7_5_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_PISO_CTRL_4_3_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_CTRL_4_3_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_PISO_CTRL_4_3_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_PISO_CTRL_4_3_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_PISO_CTRL_2_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_CTRL_2_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PISO_CTRL_1_0_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_CTRL_1_0_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_PISO_CTRL_1_0_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_PISO_CTRL_1_0_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000100 MABC_RXCPGEN_CNFG_REG |
 | +-------------------------------------------+
 | | bit  31    R/W  RXCM_ENB                  |
 | | bit  30    R/W  RXCPGEN_ENB               |
 | | bit  29:28 R/W  RXCPGEN_MODE_9_8          |
 | | bit  27    R/W  RXCPGEN_MODE_7            |
 | | bit  26:25 R/W  RXCPGEN_MODE_6_5          |
 | | bit  24:23 R/W  RXCPGEN_MODE_4_3          |
 | | bit  22:20 R/W  RXCPGEN_MODE_2_0          |
 | | bit  15    R/W  RXCPGEN_CTRL_15           |
 | | bit  14    R/W  RXCPGEN_CTRL_14           |
 | | bit  13    R/W  RXCPGEN_CTRL_13           |
 | | bit  12    R/W  RXCPGEN_CTRL_12           |
 | | bit  11    R/W  RXCPGEN_CTRL_11           |
 | | bit  10    R/W  RXCPGEN_CTRL_10           |
 | | bit  9:8   R/W  RXCPGEN_CTRL_9_8          |
 | | bit  7     R/W  RXCPGEN_CTRL_7            |
 | | bit  6     R/W  RXCPGEN_CTRL_6            |
 | | bit  5:4   R/W  RXCPGEN_CTRL_5_4          |
 | | bit  3:1   R/W  RXCPGEN_CTRL_3_1          |
 | | bit  0     R/W  RXCPGEN_CTRL_0            |
 | +-------------------------------------------+
 | 
 | void   c8_field_RXCM_ENB_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCM_ENB_get               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXCPGEN_ENB_set            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_ENB_get            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXCPGEN_MODE_9_8_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_MODE_9_8_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_RXCPGEN_MODE_9_8_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_RXCPGEN_MODE_9_8_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_RXCPGEN_MODE_7_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_MODE_7_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXCPGEN_MODE_6_5_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_MODE_6_5_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_RXCPGEN_MODE_6_5_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_RXCPGEN_MODE_6_5_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_RXCPGEN_MODE_4_3_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_MODE_4_3_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_RXCPGEN_MODE_4_3_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_RXCPGEN_MODE_4_3_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_RXCPGEN_MODE_2_0_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_MODE_2_0_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_RXCPGEN_MODE_2_0_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_RXCPGEN_MODE_2_0_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_RXCPGEN_CTRL_15_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_CTRL_15_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXCPGEN_CTRL_14_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_CTRL_14_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXCPGEN_CTRL_13_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_CTRL_13_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXCPGEN_CTRL_12_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_CTRL_12_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXCPGEN_CTRL_11_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_CTRL_11_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXCPGEN_CTRL_10_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_CTRL_10_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXCPGEN_CTRL_9_8_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_CTRL_9_8_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_RXCPGEN_CTRL_9_8_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_RXCPGEN_CTRL_9_8_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_RXCPGEN_CTRL_7_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_CTRL_7_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXCPGEN_CTRL_6_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_CTRL_6_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXCPGEN_CTRL_5_4_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_CTRL_5_4_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_RXCPGEN_CTRL_5_4_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_RXCPGEN_CTRL_5_4_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_RXCPGEN_CTRL_3_1_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_CTRL_3_1_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_RXCPGEN_CTRL_3_1_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_RXCPGEN_CTRL_3_1_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_RXCPGEN_CTRL_0_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_CTRL_0_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------+
 | | Register 0x00000104 TXRX_CNFG_REG |
 | +-----------------------------------+
 | | bit  31    R/W  TX_TEST_SEL       |
 | | bit  30    R/W  TX_SDATA_EN       |
 | | bit  21:20 R/W  TXRXCS_CLKSEL     |
 | | bit  13    R/W  TXRXCS_CTRL_3     |
 | | bit  12:10 R/W  TXRXCS_CTRL_2_0   |
 | | bit  3     R/W  TXRXCS_MODE_3     |
 | | bit  2     R/W  TXRXCS_MODE_2     |
 | | bit  1:0   R/W  TXRXCS_MODE_1_0   |
 | +-----------------------------------+
 | 
 | void   c8_field_TX_TEST_SEL_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_TEST_SEL_get           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TX_SDATA_EN_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_SDATA_EN_get           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TXRXCS_CLKSEL_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TXRXCS_CLKSEL_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_TXRXCS_CLKSEL_set   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_TXRXCS_CLKSEL_get   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_TXRXCS_CTRL_3_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TXRXCS_CTRL_3_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TXRXCS_CTRL_2_0_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TXRXCS_CTRL_2_0_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_TXRXCS_CTRL_2_0_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_TXRXCS_CTRL_2_0_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_TXRXCS_MODE_3_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TXRXCS_MODE_3_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TXRXCS_MODE_2_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TXRXCS_MODE_2_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TXRXCS_MODE_1_0_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TXRXCS_MODE_1_0_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_TXRXCS_MODE_1_0_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_TXRXCS_MODE_1_0_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000108 MABC_TEST_CNFG_REG |
 | +----------------------------------------+
 | | bit  11  R/W  VDDBIAS_ATMSB            |
 | | bit  10  R/W  TX_ATMSB                 |
 | | bit  9   R/W  TXRXCS_ATMSB             |
 | | bit  8   R/W  RX_ATMSB                 |
 | | bit  7   R/W  RXDET_ATMSB              |
 | | bit  6   R/W  RXCPGEN_ATMSB            |
 | | bit  5   R/W  PISO_ATMSB               |
 | | bit  4:0 R/W  ATIN                     |
 | +----------------------------------------+
 | 
 | void   c8_field_VDDBIAS_ATMSB_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_VDDBIAS_ATMSB_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TX_ATMSB_set      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_ATMSB_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TXRXCS_ATMSB_set  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TXRXCS_ATMSB_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RX_ATMSB_set      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RX_ATMSB_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXDET_ATMSB_set   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXDET_ATMSB_get   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXCPGEN_ATMSB_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXCPGEN_ATMSB_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PISO_ATMSB_set    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_ATMSB_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ATIN_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ATIN_get          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_ATIN_set    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_ATIN_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00000110 DIV_CNFG_REG |
 | +----------------------------------+
 | | bit  15:8 R/W  CM_CLK_DIVFACT    |
 | | bit  7:4  R/W  HRCLKDIV4_DIVFACT |
 | | bit  3:0  R/W  ADC_CLK_DIVFACT   |
 | +----------------------------------+
 | 
 | void   c8_field_CM_CLK_DIVFACT_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_CM_CLK_DIVFACT_get          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_CM_CLK_DIVFACT_set    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_CM_CLK_DIVFACT_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_HRCLKDIV4_DIVFACT_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_HRCLKDIV4_DIVFACT_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_HRCLKDIV4_DIVFACT_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_HRCLKDIV4_DIVFACT_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_ADC_CLK_DIVFACT_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ADC_CLK_DIVFACT_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_ADC_CLK_DIVFACT_set   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_ADC_CLK_DIVFACT_get   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000114 INT_REG       |
 | +-----------------------------------+
 | | bit  17 R/W  PATT_MON_ERR_I17     |
 | | bit  16 R/W  DIVCLK_DEAD_I16      |
 | | bit  15 R/W  SIPO_BIST_ERR_I15    |
 | | bit  14 R/W  RX_ADAPT_DONE_I14    |
 | | bit  13 R/W  ECC_2BIT_BERR_I13    |
 | | bit  12 R/W  ECC_1BIT_BERR_I12    |
 | | bit  11 R/W  ECC_2BIT_AERR_I11    |
 | | bit  10 R/W  ECC_1BIT_AERR_I10    |
 | | bit  9  R/W  ERROR_BIT_I9         |
 | | bit  8  R/W  DLOSB_I8             |
 | | bit  7  R/W  PROG_REF_DONE_I7     |
 | | bit  6  R/W  CALIB_DONE_I6        |
 | | bit  5  R/W  SSI_ADJUST_DONE_I5   |
 | | bit  4  R/W  SSI_I4               |
 | | bit  3  R/W  LOCK_LOSS_I3         |
 | | bit  2  R/W  LOCK_GAIN_I2         |
 | | bit  1  R/W  IN_RANGE_N_FAILS_I1  |
 | | bit  0  R/W  IN_RANGE_N_PASSES_I0 |
 | +-----------------------------------+
 | 
 | void                 c8_field_PATT_MON_ERR_I17_set_to_clear     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_PATT_MON_ERR_I17_get              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_PATT_MON_ERR_I17_poll             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_DIVCLK_DEAD_I16_set_to_clear      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_DIVCLK_DEAD_I16_get               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_DIVCLK_DEAD_I16_poll              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_SIPO_BIST_ERR_I15_set_to_clear    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_SIPO_BIST_ERR_I15_get             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_SIPO_BIST_ERR_I15_poll            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_RX_ADAPT_DONE_I14_set_to_clear    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_RX_ADAPT_DONE_I14_get             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_RX_ADAPT_DONE_I14_poll            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_ECC_2BIT_BERR_I13_set_to_clear    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_ECC_2BIT_BERR_I13_get             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_ECC_2BIT_BERR_I13_poll            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_ECC_1BIT_BERR_I12_set_to_clear    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_ECC_1BIT_BERR_I12_get             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_ECC_1BIT_BERR_I12_poll            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_ECC_2BIT_AERR_I11_set_to_clear    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_ECC_2BIT_AERR_I11_get             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_ECC_2BIT_AERR_I11_poll            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_ECC_1BIT_AERR_I10_set_to_clear    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_ECC_1BIT_AERR_I10_get             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_ECC_1BIT_AERR_I10_poll            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_ERROR_BIT_I9_set_to_clear         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_ERROR_BIT_I9_get                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_ERROR_BIT_I9_poll                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_DLOSB_I8_set_to_clear             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_DLOSB_I8_get                      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_DLOSB_I8_poll                     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_PROG_REF_DONE_I7_set_to_clear     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_PROG_REF_DONE_I7_get              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_PROG_REF_DONE_I7_poll             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_CALIB_DONE_I6_set_to_clear        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_CALIB_DONE_I6_get                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_CALIB_DONE_I6_poll                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_SSI_ADJUST_DONE_I5_set_to_clear   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_SSI_ADJUST_DONE_I5_get            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_SSI_ADJUST_DONE_I5_poll           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_SSI_I4_set_to_clear               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_SSI_I4_get                        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_SSI_I4_poll                       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_LOCK_LOSS_I3_set_to_clear         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_LOCK_LOSS_I3_get                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_LOCK_LOSS_I3_poll                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_LOCK_GAIN_I2_set_to_clear         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_LOCK_GAIN_I2_get                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_LOCK_GAIN_I2_poll                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_IN_RANGE_N_FAILS_I1_set_to_clear  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_IN_RANGE_N_FAILS_I1_get           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_IN_RANGE_N_FAILS_I1_poll          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 c8_field_IN_RANGE_N_PASSES_I0_set_to_clear ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               c8_field_IN_RANGE_N_PASSES_I0_get          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_IN_RANGE_N_PASSES_I0_poll         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00000118 INT_ENB_REG   |
 | +-----------------------------------+
 | | bit  17 R/W  PATT_MON_ERR_E17     |
 | | bit  16 R/W  DIVCLK_DEAD_E16      |
 | | bit  15 R/W  SIPO_BIST_ERR_E15    |
 | | bit  14 R/W  RX_ADAPT_DONE_E14    |
 | | bit  13 R/W  ECC_2BIT_BERR_E13    |
 | | bit  12 R/W  ECC_1BIT_BERR_E12    |
 | | bit  11 R/W  ECC_2BIT_AERR_E11    |
 | | bit  10 R/W  ECC_1BIT_AERR_E10    |
 | | bit  9  R/W  ERROR_BIT_E9         |
 | | bit  8  R/W  DLOSB_E8             |
 | | bit  7  R/W  PROG_REF_DONE_E7     |
 | | bit  6  R/W  CALIB_DONE_E6        |
 | | bit  5  R/W  SSI_ADJUST_DONE_E5   |
 | | bit  4  R/W  SSI_E4               |
 | | bit  3  R/W  LOCK_LOSS_E3         |
 | | bit  2  R/W  LOCK_GAIN_E2         |
 | | bit  1  R/W  IN_RANGE_N_FAILS_E1  |
 | | bit  0  R/W  IN_RANGE_N_PASSES_E0 |
 | +-----------------------------------+
 | 
 | void   c8_field_PATT_MON_ERR_E17_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PATT_MON_ERR_E17_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_DIVCLK_DEAD_E16_set      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DIVCLK_DEAD_E16_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_SIPO_BIST_ERR_E15_set    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_SIPO_BIST_ERR_E15_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RX_ADAPT_DONE_E14_set    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RX_ADAPT_DONE_E14_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ECC_2BIT_BERR_E13_set    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ECC_2BIT_BERR_E13_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ECC_1BIT_BERR_E12_set    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ECC_1BIT_BERR_E12_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ECC_2BIT_AERR_E11_set    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ECC_2BIT_AERR_E11_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ECC_1BIT_AERR_E10_set    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ECC_1BIT_AERR_E10_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_ERROR_BIT_E9_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ERROR_BIT_E9_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_DLOSB_E8_set             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DLOSB_E8_get             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PROG_REF_DONE_E7_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PROG_REF_DONE_E7_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_CALIB_DONE_E6_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_CALIB_DONE_E6_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_SSI_ADJUST_DONE_E5_set   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_SSI_ADJUST_DONE_E5_get   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_SSI_E4_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_SSI_E4_get               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_LOCK_LOSS_E3_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_LOCK_LOSS_E3_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_LOCK_GAIN_E2_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_LOCK_GAIN_E2_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_IN_RANGE_N_FAILS_E1_set  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_IN_RANGE_N_FAILS_E1_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_IN_RANGE_N_PASSES_E0_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_IN_RANGE_N_PASSES_E0_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------+
 | | Register 0x0000011c INT_VAL_REG |
 | +---------------------------------+
 | | bit  17 R  PATT_MON_ERR_V17     |
 | | bit  16 R  DIVCLK_DEAD_V16      |
 | | bit  15 R  SIPO_BIST_ERR_V15    |
 | | bit  14 R  RX_ADAPT_DONE_V14    |
 | | bit  13 R  ECC_2BIT_BERR_V13    |
 | | bit  12 R  ECC_1BIT_BERR_V12    |
 | | bit  11 R  ECC_2BIT_AERR_V11    |
 | | bit  10 R  ECC_1BIT_AERR_V10    |
 | | bit  9  R  ERROR_BIT_V9         |
 | | bit  8  R  DLOSB_V8             |
 | | bit  7  R  PROG_REF_DONE_V7     |
 | | bit  6  R  CALIB_DONE_V6        |
 | | bit  5  R  SSI_ADJUST_DONE_V5   |
 | | bit  4  R  SSI_V4               |
 | | bit  3  R  LOCK_LOSS_V3         |
 | | bit  2  R  LOCK_GAIN_V2         |
 | | bit  1  R  IN_RANGE_N_FAILS_V1  |
 | | bit  0  R  IN_RANGE_N_PASSES_V0 |
 | +---------------------------------+
 | 
 | UINT32               c8_field_PATT_MON_ERR_V17_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_PATT_MON_ERR_V17_poll     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_DIVCLK_DEAD_V16_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_DIVCLK_DEAD_V16_poll      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_SIPO_BIST_ERR_V15_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_SIPO_BIST_ERR_V15_poll    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_RX_ADAPT_DONE_V14_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_RX_ADAPT_DONE_V14_poll    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_ECC_2BIT_BERR_V13_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_ECC_2BIT_BERR_V13_poll    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_ECC_1BIT_BERR_V12_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_ECC_1BIT_BERR_V12_poll    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_ECC_2BIT_AERR_V11_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_ECC_2BIT_AERR_V11_poll    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_ECC_1BIT_AERR_V10_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_ECC_1BIT_AERR_V10_poll    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_ERROR_BIT_V9_get          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_ERROR_BIT_V9_poll         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_DLOSB_V8_get              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_DLOSB_V8_poll             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_PROG_REF_DONE_V7_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_PROG_REF_DONE_V7_poll     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_CALIB_DONE_V6_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_CALIB_DONE_V6_poll        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_SSI_ADJUST_DONE_V5_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_SSI_ADJUST_DONE_V5_poll   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_SSI_V4_get                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_SSI_V4_poll               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_LOCK_LOSS_V3_get          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_LOCK_LOSS_V3_poll         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_LOCK_GAIN_V2_get          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_LOCK_GAIN_V2_poll         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_IN_RANGE_N_FAILS_V1_get   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_IN_RANGE_N_FAILS_V1_poll  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               c8_field_IN_RANGE_N_PASSES_V0_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_field_IN_RANGE_N_PASSES_V0_poll ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------+
 | | Register 0x00000120 OFF_RAM_CNFG_REG |
 | +--------------------------------------+
 | | bit  5   R/W  RAM_SHUT_DOWN          |
 | | bit  4   R/W  RAM_DEEP_SLEEP         |
 | | bit  3   R/W  RAM_LIGHT_SLEEP        |
 | | bit  2:1 R/W  RAM_ERR_INJECT         |
 | | bit  0   R/W  ECC_BYPASS             |
 | +--------------------------------------+
 | 
 | void   c8_field_RAM_SHUT_DOWN_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RAM_SHUT_DOWN_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RAM_DEEP_SLEEP_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RAM_DEEP_SLEEP_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RAM_LIGHT_SLEEP_set      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RAM_LIGHT_SLEEP_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RAM_ERR_INJECT_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RAM_ERR_INJECT_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_RAM_ERR_INJECT_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_RAM_ERR_INJECT_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_ECC_BYPASS_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_ECC_BYPASS_get           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------+
 | | Register 0x00000124 MTSB_CTRL_CNFG_REG |
 | +----------------------------------------+
 | | bit  1:0 R/W  RX_RATE                  |
 | +----------------------------------------+
 | 
 | void   c8_field_RX_RATE_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RX_RATE_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_RX_RATE_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_RX_RATE_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000128 DIAG_CNFG_REG1        |
 | +-------------------------------------------+
 | | bit  31:24 R/W  VHIST_CNT_TIM_LIMIT_39_32 |
 | | bit  2     R/W  VHIST_PERBIN_VALUE_RD_ENB |
 | | bit  1     R/W  VHIST_CAPT_START          |
 | | bit  0     R/W  VHIST_CAPT_ENB            |
 | +-------------------------------------------+
 | 
 | void   c8_field_VHIST_CNT_TIM_LIMIT_39_32_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_VHIST_CNT_TIM_LIMIT_39_32_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_VHIST_CNT_TIM_LIMIT_39_32_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_VHIST_CNT_TIM_LIMIT_39_32_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_VHIST_PERBIN_VALUE_RD_ENB_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_VHIST_PERBIN_VALUE_RD_ENB_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_VHIST_CAPT_START_set                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_VHIST_CAPT_START_get                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_VHIST_CAPT_ENB_set                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_VHIST_CAPT_ENB_get                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------+
 | | Register 0x0000012c DIAG_CNFG_REG2      |
 | +-----------------------------------------+
 | | bit  31:0 R/W  VHIST_CNT_TIM_LIMIT_31_0 |
 | +-----------------------------------------+
 | 
 | void   c8_field_VHIST_CNT_TIM_LIMIT_31_0_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_VHIST_CNT_TIM_LIMIT_31_0_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_VHIST_CNT_TIM_LIMIT_31_0_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_VHIST_CNT_TIM_LIMIT_31_0_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000130 DIAG_STATUS_REG1   |
 | +----------------------------------------+
 | | bit  31:24 R  VHIST_PERBIN_VALUE_39_32 |
 | | bit  15:11 R  VHIST_READ_BACK_ADDR     |
 | | bit  7:5   R  VHIST_FSM_STATE          |
 | | bit  1     R  VHIST_PERBIN_VALUE_OVFL  |
 | | bit  0     R  VHIST_CAPT_RUNNING       |
 | +----------------------------------------+
 | 
 | UINT32 c8_field_VHIST_PERBIN_VALUE_39_32_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_field_range_VHIST_PERBIN_VALUE_39_32_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 c8_field_VHIST_READ_BACK_ADDR_get           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_field_range_VHIST_READ_BACK_ADDR_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 c8_field_VHIST_FSM_STATE_get                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_field_range_VHIST_FSM_STATE_get          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 c8_field_VHIST_PERBIN_VALUE_OVFL_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_field_VHIST_CAPT_RUNNING_get             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------+
 | | Register 0x00000134 DIAG_STATUS_REG2 |
 | +--------------------------------------+
 | | bit  31:0 R  VHIST_PERBIN_VALUE_31_0 |
 | +--------------------------------------+
 | 
 | UINT32 c8_field_VHIST_PERBIN_VALUE_31_0_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_field_range_VHIST_PERBIN_VALUE_31_0_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000140 SIPO_BIST_CNFG_REG |
 | +----------------------------------------+
 | | bit  8   R/W  TIME_REVERSE             |
 | | bit  6:4 R/W  SIPO_BIST_SEL            |
 | | bit  0   R/W  SIPO_BIST_EN             |
 | +----------------------------------------+
 | 
 | void   c8_field_TIME_REVERSE_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TIME_REVERSE_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_SIPO_BIST_SEL_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_SIPO_BIST_SEL_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_SIPO_BIST_SEL_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_SIPO_BIST_SEL_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_SIPO_BIST_EN_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_SIPO_BIST_EN_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------------+
 | | Register 0x00000144 SIPO_BIST_STATUS_REG |
 | +------------------------------------------+
 | | bit  1 R  SIPO_BIST_SYNC                 |
 | | bit  0 R  SIPO_BIST_ERR                  |
 | +------------------------------------------+
 | 
 | UINT32 c8_field_SIPO_BIST_SYNC_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_field_SIPO_BIST_ERR_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------+
 | | Register 0x00000150 PATT_GEN_CNFG_REG1 |
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
 | void   c8_field_GEN_USR_PAT_39_32_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_GEN_USR_PAT_39_32_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_GEN_USR_PAT_39_32_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_GEN_USR_PAT_39_32_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_PATT_BUS_WIDTH_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PATT_BUS_WIDTH_get          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_PATT_BUS_WIDTH_set    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_PATT_BUS_WIDTH_get    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_GEN_ERR_INJ_set             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_GEN_ERR_INJ_get             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_GEN_INV_set                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_GEN_INV_get                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_GEN_MODE_set                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_GEN_MODE_get                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_GEN_MODE_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_GEN_MODE_get          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_INV_TX_DATA_set             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_INV_TX_DATA_get             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PATT_GEN_SEL_set            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PATT_GEN_SEL_get            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_GEN_EN_set                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_GEN_EN_get                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------+
 | | Register 0x00000154 PATT_GEN_CNFG_REG2 |
 | +----------------------------------------+
 | | bit  31:0 R/W  GEN_USR_PAT_31_0        |
 | +----------------------------------------+
 | 
 | void   c8_field_GEN_USR_PAT_31_0_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_GEN_USR_PAT_31_0_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_GEN_USR_PAT_31_0_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_GEN_USR_PAT_31_0_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000158 PATT_CHK_CNFG_REG1 |
 | +----------------------------------------+
 | | bit  31:24 R/W  CHK_USR_PAT_39_32      |
 | | bit  8     R/W  CHK_READ               |
 | | bit  7     R/W  CHK_INV                |
 | | bit  6:3   R/W  CHK_MODE               |
 | | bit  0     R/W  CHK_EN                 |
 | +----------------------------------------+
 | 
 | void   c8_field_CHK_USR_PAT_39_32_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_CHK_USR_PAT_39_32_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_CHK_USR_PAT_39_32_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_CHK_USR_PAT_39_32_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_CHK_READ_set                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_CHK_READ_get                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_CHK_INV_set                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_CHK_INV_get                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_CHK_MODE_set                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_CHK_MODE_get                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_CHK_MODE_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_CHK_MODE_get          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_CHK_EN_set                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_CHK_EN_get                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------+
 | | Register 0x0000015c PATT_CHK_CNFG_REG2 |
 | +----------------------------------------+
 | | bit  31:0 R/W  CHK_USR_PAT_31_0        |
 | +----------------------------------------+
 | 
 | void   c8_field_CHK_USR_PAT_31_0_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_CHK_USR_PAT_31_0_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_CHK_USR_PAT_31_0_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_CHK_USR_PAT_31_0_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000160 PATT_CHK_STATUS_REG1 |
 | +------------------------------------------+
 | | bit  31:0 R  CHK_ERR_CNT                 |
 | +------------------------------------------+
 | 
 | UINT32 c8_field_CHK_ERR_CNT_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_field_range_CHK_ERR_CNT_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000170 TX_CTRL_CNFG_REG1 |
 | +---------------------------------------+
 | | bit  16:0 R/W  TX_PSELI               |
 | +---------------------------------------+
 | 
 | void   c8_field_TX_PSELI_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_PSELI_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_TX_PSELI_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_TX_PSELI_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000174 TX_CTRL_CNFG_REG2 |
 | +---------------------------------------+
 | | bit  16:0 R/W  TX_DENI                |
 | +---------------------------------------+
 | 
 | void   c8_field_TX_DENI_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_DENI_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_TX_DENI_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_TX_DENI_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000178 TX_CTRL_CNFG_REG3 |
 | +---------------------------------------+
 | | bit  31:30 R/W  PISO_PRE2_MODE        |
 | | bit  25    R/W  PISO_PRE2_ENABLE      |
 | | bit  24    R/W  PISO_PRE1_ENABLE      |
 | | bit  21:16 R/W  TX_DPENI2             |
 | | bit  13:8  R/W  TX_DPENI1             |
 | | bit  4:0   R/W  TX_DER                |
 | +---------------------------------------+
 | 
 | void   c8_field_PISO_PRE2_MODE_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_PRE2_MODE_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_PISO_PRE2_MODE_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_PISO_PRE2_MODE_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_PISO_PRE2_ENABLE_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_PRE2_ENABLE_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PISO_PRE1_ENABLE_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PISO_PRE1_ENABLE_get     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TX_DPENI2_set            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_DPENI2_get            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_TX_DPENI2_set      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_TX_DPENI2_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_TX_DPENI1_set            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_DPENI1_get            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_TX_DPENI1_set      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_TX_DPENI1_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_TX_DER_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_DER_get               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_TX_DER_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_TX_DER_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x0000017c TX_CTRL_CNFG_REG4 |
 | +---------------------------------------+
 | | bit  31    R/W  RXDET_CANCEL_OVR      |
 | | bit  30:25 R/W  RXDET_CTRL_PCBI       |
 | | bit  24:19 R/W  RXDET_CTRL_CORE       |
 | | bit  18    R/W  TX_CAL_EN             |
 | | bit  17    R/W  TX_CM_ENB             |
 | | bit  16    R/W  TX_EDGE_DELAY_SEL     |
 | | bit  15    R/W  TX_CM_MODN_B          |
 | | bit  14    R/W  TX_CM_MODP_B          |
 | | bit  13    R/W  RXDET_ENB             |
 | | bit  12    R/W  RXDET_HI_SET          |
 | | bit  11    R/W  TX_WIZARD_EN          |
 | | bit  10    R/W  TX_AMP_EMPH_EN        |
 | | bit  9     R/W  TX_CTRL_ENB           |
 | | bit  8     R/W  TX_CMD_EN             |
 | | bit  2:0   R/W  RXDET_IDLE_DELAY      |
 | +---------------------------------------+
 | 
 | void   c8_field_RXDET_CANCEL_OVR_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXDET_CANCEL_OVR_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXDET_CTRL_PCBI_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXDET_CTRL_PCBI_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_RXDET_CTRL_PCBI_set  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_RXDET_CTRL_PCBI_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_RXDET_CTRL_CORE_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXDET_CTRL_CORE_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_RXDET_CTRL_CORE_set  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_RXDET_CTRL_CORE_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_TX_CAL_EN_set              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_CAL_EN_get              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TX_CM_ENB_set              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_CM_ENB_get              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TX_EDGE_DELAY_SEL_set      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_EDGE_DELAY_SEL_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TX_CM_MODN_B_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_CM_MODN_B_get           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TX_CM_MODP_B_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_CM_MODP_B_get           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXDET_ENB_set              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXDET_ENB_get              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXDET_HI_SET_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXDET_HI_SET_get           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TX_WIZARD_EN_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_WIZARD_EN_get           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TX_AMP_EMPH_EN_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_AMP_EMPH_EN_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TX_CTRL_ENB_set            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_CTRL_ENB_get            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_TX_CMD_EN_set              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_CMD_EN_get              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_RXDET_IDLE_DELAY_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_RXDET_IDLE_DELAY_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_RXDET_IDLE_DELAY_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_RXDET_IDLE_DELAY_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000180 TX_CTRL_CNFG_REG5 |
 | +---------------------------------------+
 | | bit  31:16 R/W  TX_CTRL_EDGE          |
 | | bit  15:0  R/W  TX_CM_COUNTER         |
 | +---------------------------------------+
 | 
 | void   c8_field_TX_CTRL_EDGE_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_CTRL_EDGE_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_TX_CTRL_EDGE_set  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_TX_CTRL_EDGE_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_TX_CM_COUNTER_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_CM_COUNTER_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_TX_CM_COUNTER_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_TX_CM_COUNTER_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000184 TX_CTRL_CNFG_REG6 |
 | +---------------------------------------+
 | | bit  17:0 R/W  TX_CTRL_PRE1           |
 | +---------------------------------------+
 | 
 | void   c8_field_TX_CTRL_PRE1_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_CTRL_PRE1_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_TX_CTRL_PRE1_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_TX_CTRL_PRE1_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000188 TX_CTRL_CNFG_REG7 |
 | +---------------------------------------+
 | | bit  17:0 R/W  TX_CTRL_PRE2           |
 | +---------------------------------------+
 | 
 | void   c8_field_TX_CTRL_PRE2_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_TX_CTRL_PRE2_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_TX_CTRL_PRE2_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_TX_CTRL_PRE2_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x0000018c TX_CTRL_STS_REG1 |
 | +--------------------------------------+
 | | bit  31:16 R  TX_CODE_OUT            |
 | | bit  15:0  R  TX_SUM_OUT             |
 | +--------------------------------------+
 | 
 | UINT32 c8_field_TX_CODE_OUT_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_field_range_TX_CODE_OUT_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 c8_field_TX_SUM_OUT_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | UINT32 c8_field_range_TX_SUM_OUT_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000190 PMM_WRAP_CNFG_REG1 |
 | +----------------------------------------+
 | | bit  29:16 R/W  DSM_DENOM              |
 | | bit  1:0   R/W  DSM_TYPE               |
 | +----------------------------------------+
 | 
 | void   c8_field_DSM_DENOM_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DSM_DENOM_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_DSM_DENOM_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_DSM_DENOM_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_DSM_TYPE_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_DSM_TYPE_get        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_DSM_TYPE_set  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_DSM_TYPE_get  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000194 PMM_WRAP_CNFG_REG2 |
 | +----------------------------------------+
 | | bit  31    R/W  PMM_WRAP_EN            |
 | | bit  24:16 R/W  PKT_SIZE               |
 | | bit  15:0  R/W  PKT_PERIOD             |
 | +----------------------------------------+
 | 
 | void   c8_field_PMM_WRAP_EN_set      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PMM_WRAP_EN_get      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_PKT_SIZE_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PKT_SIZE_get         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_PKT_SIZE_set   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_PKT_SIZE_get   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   c8_field_PKT_PERIOD_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 c8_field_PKT_PERIOD_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void   c8_field_range_PKT_PERIOD_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_field_range_PKT_PERIOD_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000300 + (N) * 0x4) GCOC_OFFSET_RAM_REG |
 | |     N[0..26]                                          |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  OFFSET                                 |
 | +-------------------------------------------------------+
 | 
 | void   c8_lfield_OFFSET_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value[27] )
 | void   c8_lfield_OFFSET_get       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value[27] )
 | void   c8_lfield_range_OFFSET_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 c8_lfield_range_OFFSET_get ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _C8_IO_H
#define _C8_IO_H

#include "c8.h"
#include "c8_regs.h"
#include "c8_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 c8_buffer_init               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | void                 c8_buffer_flush              ( c8_buffer_t *b_ptr )
 | UINT32               c8_reg_read                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 c8_reg_write                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 c8_field_set                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 c8_action_on_write_field_set ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 c8_burst_read                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 c8_burst_write               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE c8_poll                      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 c8_reg_DIGITAL_RST_REG_write                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_DIGITAL_RST_REG_field_set             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_DIGITAL_RST_REG_read                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_MABC_RST_CNFG_REG_write               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_MABC_RST_CNFG_REG_field_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_MABC_RST_CNFG_REG_read                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_CLK_DGLCH_CNFG_REG_write              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_CLK_DGLCH_CNFG_REG_field_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_CLK_DGLCH_CNFG_REG_read               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_PLL_CNFG_REG1_write                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_PLL_CNFG_REG1_field_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_PLL_CNFG_REG1_read                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_PLL_CNFG_REG2_write                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_PLL_CNFG_REG2_field_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_PLL_CNFG_REG2_read                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_PLL_CNFG_REG3_write                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_PLL_CNFG_REG3_field_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_PLL_CNFG_REG3_read                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_PLL_CNFG_REG4_write                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_PLL_CNFG_REG4_field_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_PLL_CNFG_REG4_read                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_PLL_CNFG_REG5_write                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_PLL_CNFG_REG5_field_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_PLL_CNFG_REG5_read                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_GCOC_CNFG_REG1_write                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_GCOC_CNFG_REG1_field_set              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_GCOC_CNFG_REG1_read                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_GCOC_CNFG_REG2_write                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_GCOC_CNFG_REG2_field_set              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_GCOC_CNFG_REG2_read                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_GCOC_CNFG_REG3_write                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_GCOC_CNFG_REG3_field_set              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_GCOC_CNFG_REG3_read                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_GCOC_CNFG_REG4_write                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_GCOC_CNFG_REG4_field_set              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_GCOC_CNFG_REG4_read                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_MDSP_CNFG_REG_write                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_MDSP_CNFG_REG_field_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_MDSP_CNFG_REG_read                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_CTRL_CNFG_REG_write                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_CTRL_CNFG_REG_field_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_CTRL_CNFG_REG_read                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_DLPBK_CNFG_REG_write                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_DLPBK_CNFG_REG_field_set              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_DLPBK_CNFG_REG_read                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_SIPO_CNFG_REG_write                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_SIPO_CNFG_REG_field_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_SIPO_CNFG_REG_read                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_PISO_CNFG_REG_write                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_PISO_CNFG_REG_field_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_PISO_CNFG_REG_read                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_EFUSE_CNFG_REG_write                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_EFUSE_CNFG_REG_field_set              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_EFUSE_CNFG_REG_read                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_DESKEW_FIFO_CNFG_REG_write            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_DESKEW_FIFO_CNFG_REG_field_set        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_DESKEW_FIFO_CNFG_REG_read             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_SSI_CNFG_REG1_write                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_SSI_CNFG_REG1_field_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_SSI_CNFG_REG1_read                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_SSI_CNFG_REG2_write                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_SSI_CNFG_REG2_field_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_SSI_CNFG_REG2_read                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_DLOS_CNFG_REG_write                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_DLOS_CNFG_REG_field_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_DLOS_CNFG_REG_read                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_MABC_ADC_CNFG_REG_write               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_MABC_ADC_CNFG_REG_field_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_MABC_ADC_CNFG_REG_read                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_MABC_CNFG_REG_write                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_MABC_CNFG_REG_field_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_MABC_CNFG_REG_read                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_MABC_PISO_CNFG_REG1_write             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_MABC_PISO_CNFG_REG1_field_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_MABC_PISO_CNFG_REG1_read              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_MABC_PISO_CNFG_REG2_write             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_MABC_PISO_CNFG_REG2_field_set         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_MABC_PISO_CNFG_REG2_read              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_MABC_RXCPGEN_CNFG_REG_write           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_MABC_RXCPGEN_CNFG_REG_field_set       ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_MABC_RXCPGEN_CNFG_REG_read            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_TXRX_CNFG_REG_write                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_TXRX_CNFG_REG_field_set               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_TXRX_CNFG_REG_read                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_MABC_TEST_CNFG_REG_write              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_MABC_TEST_CNFG_REG_field_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_MABC_TEST_CNFG_REG_read               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_DIV_CNFG_REG_write                    ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_DIV_CNFG_REG_field_set                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_DIV_CNFG_REG_read                     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_OFF_RAM_CNFG_REG_write                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_OFF_RAM_CNFG_REG_field_set            ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_OFF_RAM_CNFG_REG_read                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_MTSB_CTRL_CNFG_REG_write              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_MTSB_CTRL_CNFG_REG_field_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_MTSB_CTRL_CNFG_REG_read               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_DIAG_CNFG_REG1_write                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_DIAG_CNFG_REG1_field_set              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_DIAG_CNFG_REG1_read                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_DIAG_CNFG_REG2_write                  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_DIAG_CNFG_REG2_field_set              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_DIAG_CNFG_REG2_read                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_SIPO_BIST_CNFG_REG_write              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_SIPO_BIST_CNFG_REG_field_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_SIPO_BIST_CNFG_REG_read               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_PATT_GEN_CNFG_REG1_write              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_PATT_GEN_CNFG_REG1_field_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_PATT_GEN_CNFG_REG1_read               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_PATT_GEN_CNFG_REG2_write              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_PATT_GEN_CNFG_REG2_field_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_PATT_GEN_CNFG_REG2_read               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_PATT_CHK_CNFG_REG1_write              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_PATT_CHK_CNFG_REG1_field_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_PATT_CHK_CNFG_REG1_read               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_PATT_CHK_CNFG_REG2_write              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_PATT_CHK_CNFG_REG2_field_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_PATT_CHK_CNFG_REG2_read               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_TX_CTRL_CNFG_REG1_write               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_TX_CTRL_CNFG_REG1_field_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_TX_CTRL_CNFG_REG1_read                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_TX_CTRL_CNFG_REG2_write               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_TX_CTRL_CNFG_REG2_field_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_TX_CTRL_CNFG_REG2_read                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_TX_CTRL_CNFG_REG3_write               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_TX_CTRL_CNFG_REG3_field_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_TX_CTRL_CNFG_REG3_read                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_TX_CTRL_CNFG_REG4_write               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_TX_CTRL_CNFG_REG4_field_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_TX_CTRL_CNFG_REG4_read                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_TX_CTRL_CNFG_REG5_write               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_TX_CTRL_CNFG_REG5_field_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_TX_CTRL_CNFG_REG5_read                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_TX_CTRL_CNFG_REG6_write               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_TX_CTRL_CNFG_REG6_field_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_TX_CTRL_CNFG_REG6_read                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_TX_CTRL_CNFG_REG7_write               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_TX_CTRL_CNFG_REG7_field_set           ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_TX_CTRL_CNFG_REG7_read                ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_PMM_WRAP_CNFG_REG1_write              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_PMM_WRAP_CNFG_REG1_field_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_PMM_WRAP_CNFG_REG1_read               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_PMM_WRAP_CNFG_REG2_write              ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_PMM_WRAP_CNFG_REG2_field_set          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_PMM_WRAP_CNFG_REG2_read               ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | void                 c8_reg_INT_REG_write                         ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_INT_REG_action_on_write_field_set     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_INT_REG_read                          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_reg_INT_REG_poll                          ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 c8_reg_INT_ENB_REG_write                     ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 c8_reg_INT_ENB_REG_field_set                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               c8_reg_INT_ENB_REG_read                      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               c8_reg_INT_VAL_REG_read                      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE c8_reg_INT_VAL_REG_poll                      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 c8_reg_GCOC_OFFSET_RAM_REG_array_burst_write ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 c8_reg_GCOC_OFFSET_RAM_REG_array_field_set   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 c8_reg_GCOC_OFFSET_RAM_REG_array_burst_read  ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               c8_reg_GCOC_OFFSET_RAM_REG_array_read        ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A, UINT32 N )
 | 
 | UINT32               c8_reg_PLL_STS_REG_read                      ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               c8_reg_SSI_STATUS_REG_read                   ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               c8_reg_MABC_ADC_STS_REG_read                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               c8_reg_DIAG_STATUS_REG1_read                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               c8_reg_DIAG_STATUS_REG2_read                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               c8_reg_SIPO_BIST_STATUS_REG_read             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               c8_reg_PATT_CHK_STATUS_REG1_read             ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               c8_reg_TX_CTRL_STS_REG1_read                 ( c8_buffer_t *b_ptr, c8_handle_t *h_ptr, UINT32 A )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _C8_IO_H */
