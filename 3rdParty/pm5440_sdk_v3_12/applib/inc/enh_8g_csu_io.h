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
 *     and register accessor functions for the enh_8g_csu block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing enh_8g_csu io functions is:
 | {
 |     enh_8g_csu_buffer_t b_ptr[1];
 |     enh_8g_csu_buffer_init( b_ptr, h_ptr );
 |     value = enh_8g_csu_field_<FIELD>_get( b_ptr, h_ptr );
 |     enh_8g_csu_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     enh_8g_csu_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = enh_8g_csu_field_<FIELD>_get( NULL, h_ptr );
 | enh_8g_csu_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------+
 | | Register 0x00000400 CNI_UNI_CSU_RST_REG |
 | +-----------------------------------------+
 | | bit  7 R/W  ACB_RSTB                    |
 | | bit  6 R/W  PLL_LOCK_DET_SW_RST         |
 | | bit  5 R/W  CNI_UNI_CSU_GLB_RSTB        |
 | | bit  4 R/W  GCOC_RSTB                   |
 | | bit  3 R/W  EADSM_RSTB                  |
 | | bit  2 R/W  OCC_TXRX_RSTB               |
 | | bit  1 R/W  VCO_CAL_RSTB                |
 | | bit  0 R/W  PLL_LOCK_DET_RSTB           |
 | +-----------------------------------------+
 | 
 | void   enh_8g_csu_field_ACB_RSTB_set             ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_ACB_RSTB_get             ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_PLL_LOCK_DET_SW_RST_set  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_PLL_LOCK_DET_SW_RST_get  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CNI_UNI_CSU_GLB_RSTB_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CNI_UNI_CSU_GLB_RSTB_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_GCOC_RSTB_set            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_GCOC_RSTB_get            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_EADSM_RSTB_set           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_EADSM_RSTB_get           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_OCC_TXRX_RSTB_set        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_OCC_TXRX_RSTB_get        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_VCO_CAL_RSTB_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_VCO_CAL_RSTB_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_PLL_LOCK_DET_RSTB_set    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_PLL_LOCK_DET_RSTB_get    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000404 MABC_RST_CNFG_REG |
 | +---------------------------------------+
 | | bit  1 R/W  ARSTB                     |
 | | bit  0 R/W  RST_SYNC                  |
 | +---------------------------------------+
 | 
 | void   enh_8g_csu_field_ARSTB_set    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_ARSTB_get    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_RST_SYNC_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_RST_SYNC_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +----------------------------------------+
 | | Register 0x00000408 CLK_DGLCH_CNFG_REG |
 | +----------------------------------------+
 | | bit  1 R/W  HRCLKDIV4OR5_EN            |
 | | bit  0 R/W  HRCLKDIV4_EN               |
 | +----------------------------------------+
 | 
 | void   enh_8g_csu_field_HRCLKDIV4OR5_EN_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_HRCLKDIV4OR5_EN_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_HRCLKDIV4_EN_set    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_HRCLKDIV4_EN_get    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x00000410 PLL_CNFG_REG1 |
 | +-----------------------------------+
 | | bit  5   R/W  PPM_OFF_RD_REQ      |
 | | bit  4   R/W  RESET_HISTORY       |
 | | bit  3:0 R/W  LOCK_PRECISION      |
 | +-----------------------------------+
 | 
 | void   enh_8g_csu_field_RESET_HISTORY_set        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_RESET_HISTORY_get        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_LOCK_PRECISION_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_LOCK_PRECISION_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_LOCK_PRECISION_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_LOCK_PRECISION_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_PPM_OFF_RD_REQ_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_PPM_OFF_RD_REQ_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000414 PLL_CLK_CNTR_REG |
 | +--------------------------------------+
 | | bit  27:20 R/W  LOCK_CNT_MAX         |
 | | bit  19:0  R/W  CLK_CNT_MAX          |
 | +--------------------------------------+
 | 
 | void   enh_8g_csu_field_LOCK_CNT_MAX_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_LOCK_CNT_MAX_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_LOCK_CNT_MAX_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_LOCK_CNT_MAX_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CLK_CNT_MAX_set        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CLK_CNT_MAX_get        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CLK_CNT_MAX_set  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CLK_CNT_MAX_get  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00000418 PLL_CNTR_REG |
 | +----------------------------------+
 | | bit  31:16 R/W  FAIL_CNT_MAX     |
 | | bit  15:0  R/W  PASS_CNT_MAX     |
 | +----------------------------------+
 | 
 | void   enh_8g_csu_field_FAIL_CNT_MAX_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_FAIL_CNT_MAX_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_FAIL_CNT_MAX_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_FAIL_CNT_MAX_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_PASS_CNT_MAX_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_PASS_CNT_MAX_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_PASS_CNT_MAX_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_PASS_CNT_MAX_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x0000041c PLL_STA_REG |
 | +---------------------------------+
 | | bit  1 R  LOCKB                 |
 | +---------------------------------+
 | 
 | UINT32 enh_8g_csu_field_LOCKB_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000420 PLL_LOCK_DET_PPM_OFF_STA_REG |
 | +--------------------------------------------------+
 | | bit  31   R  PPM_OFF_RD_SIGN                     |
 | | bit  20:0 R  PPM_OFF_RD_VAL                      |
 | +--------------------------------------------------+
 | 
 | UINT32 enh_8g_csu_field_PPM_OFF_RD_SIGN_get      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 enh_8g_csu_field_PPM_OFF_RD_VAL_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 enh_8g_csu_field_range_PPM_OFF_RD_VAL_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000428 VCO_CAL_CNFG_REG_1  |
 | +-----------------------------------------+
 | | bit  28:23 R/W  VCOC_MAX_THRESHOLD      |
 | | bit  22:17 R/W  VCOC_MIN_THRESHOLD      |
 | | bit  16    R/W  PPM_OFFSET_POLARITY_INV |
 | | bit  15    R/W  LOAD_COUNT_OVR_EN       |
 | | bit  14    R/W  LOAD_COUNT_OVR          |
 | | bit  13:8  R/W  MIN_MODE                |
 | | bit  7:2   R/W  MAX_MODE                |
 | | bit  1     R/W  VCOC_START              |
 | | bit  0     R/W  VCOC_EN                 |
 | +-----------------------------------------+
 | 
 | void   enh_8g_csu_field_VCOC_MAX_THRESHOLD_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_VCOC_MAX_THRESHOLD_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_VCOC_MAX_THRESHOLD_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_VCOC_MAX_THRESHOLD_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_VCOC_MIN_THRESHOLD_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_VCOC_MIN_THRESHOLD_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_VCOC_MIN_THRESHOLD_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_VCOC_MIN_THRESHOLD_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_PPM_OFFSET_POLARITY_INV_set  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_PPM_OFFSET_POLARITY_INV_get  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_LOAD_COUNT_OVR_EN_set        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_LOAD_COUNT_OVR_EN_get        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_LOAD_COUNT_OVR_set           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_LOAD_COUNT_OVR_get           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_MIN_MODE_set                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_MIN_MODE_get                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_MIN_MODE_set           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_MIN_MODE_get           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_MAX_MODE_set                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_MAX_MODE_get                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_MAX_MODE_set           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_MAX_MODE_get           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_VCOC_START_set               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_VCOC_START_get               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_VCOC_EN_set                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_VCOC_EN_get                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +----------------------------------------+
 | | Register 0x0000042c VCO_CAL_CNFG_REG_2 |
 | +----------------------------------------+
 | | bit  23:16 R/W  VCO_SETTLING_TIME      |
 | | bit  11:8  R/W  VCOC_LOOP              |
 | | bit  6:1   R/W  OFF_EQ_WINDOW          |
 | | bit  0     R/W  VCOC_BIST_EN           |
 | +----------------------------------------+
 | 
 | void   enh_8g_csu_field_VCO_SETTLING_TIME_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_VCO_SETTLING_TIME_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_VCO_SETTLING_TIME_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_VCO_SETTLING_TIME_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_VCOC_LOOP_set               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_VCOC_LOOP_get               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_VCOC_LOOP_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_VCOC_LOOP_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_OFF_EQ_WINDOW_set           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_OFF_EQ_WINDOW_get           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_OFF_EQ_WINDOW_set     ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_OFF_EQ_WINDOW_get     ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_VCOC_BIST_EN_set            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_VCOC_BIST_EN_get            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x00000430 VCO_CAL_STA_REG |
 | +-------------------------------------+
 | | bit  16   R  VCOC_MODE_IN_RANGE_S   |
 | | bit  15:8 R  VCOC_BIST_MODE_COUNT   |
 | | bit  5:0  R  VCOC_MODE_OUT          |
 | +-------------------------------------+
 | 
 | UINT32 enh_8g_csu_field_VCOC_MODE_IN_RANGE_S_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 enh_8g_csu_field_VCOC_BIST_MODE_COUNT_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 enh_8g_csu_field_range_VCOC_BIST_MODE_COUNT_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 enh_8g_csu_field_VCOC_MODE_OUT_get              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 enh_8g_csu_field_range_VCOC_MODE_OUT_get        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000438 OCC_TXRX_CNFG_REG_1 |
 | +-----------------------------------------+
 | | bit  21    R/W  OCC_START               |
 | | bit  20    R/W  OCC_CONT                |
 | | bit  19    R/W  CLR_OCDAC               |
 | | bit  18    R/W  OCC_ENABLE              |
 | | bit  17    R/W  OC_OVERRIDE_ENB         |
 | | bit  16    R/W  OC_INVERT               |
 | | bit  15:14 R/W  OCC_DONE_GEN_SEL        |
 | +-----------------------------------------+
 | 
 | void   enh_8g_csu_field_OCC_START_set              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_OCC_START_get              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_OCC_CONT_set               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_OCC_CONT_get               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CLR_OCDAC_set              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CLR_OCDAC_get              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_OCC_ENABLE_set             ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_OCC_ENABLE_get             ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_OC_OVERRIDE_ENB_set        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_OC_OVERRIDE_ENB_get        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_OC_INVERT_set              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_OC_INVERT_get              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_OCC_DONE_GEN_SEL_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_OCC_DONE_GEN_SEL_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_OCC_DONE_GEN_SEL_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_OCC_DONE_GEN_SEL_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000440 OCC_TXRX_CNFG_REG_2 |
 | +-----------------------------------------+
 | | bit  24:20 R/W  OC_OVERRIDE             |
 | | bit  14:0  R/W  OC_TOLERANCE            |
 | +-----------------------------------------+
 | 
 | void   enh_8g_csu_field_OC_OVERRIDE_set        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_OC_OVERRIDE_get        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_OC_OVERRIDE_set  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_OC_OVERRIDE_get  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_OC_TOLERANCE_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_OC_TOLERANCE_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_OC_TOLERANCE_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_OC_TOLERANCE_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000444 OCC_TXRX_CNFG_REG_3 |
 | +-----------------------------------------+
 | | bit  19:16 R/W  OC_THRESHOLD            |
 | | bit  15:0  R/W  OC_INTERVAL             |
 | +-----------------------------------------+
 | 
 | void   enh_8g_csu_field_OC_THRESHOLD_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_OC_THRESHOLD_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_OC_THRESHOLD_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_OC_THRESHOLD_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_OC_INTERVAL_set        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_OC_INTERVAL_get        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_OC_INTERVAL_set  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_OC_INTERVAL_get  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00000448 OCC_TXRX_STA_REG |
 | +--------------------------------------+
 | | bit  6   R  OC_THRESHOLD_CHECK_FAILB |
 | | bit  5:1 R  OCDAC_UNENC              |
 | | bit  0   R  OC_SENSE                 |
 | +--------------------------------------+
 | 
 | UINT32 enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 enh_8g_csu_field_OCDAC_UNENC_get              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 enh_8g_csu_field_range_OCDAC_UNENC_get        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 enh_8g_csu_field_OC_SENSE_get                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000454 CLK_DIV_CNFG_REG |
 | +--------------------------------------+
 | | bit  3:0 R/W  DIV_CLK_FACT           |
 | +--------------------------------------+
 | 
 | void   enh_8g_csu_field_DIV_CLK_FACT_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_DIV_CLK_FACT_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_DIV_CLK_FACT_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_DIV_CLK_FACT_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x0000045c MABC_TC_CNFG_REG |
 | +--------------------------------------+
 | | bit  16   R/W  TERM_OVR              |
 | | bit  11:8 R/W  TERMB0_ENB_DIG        |
 | | bit  3:0  R/W  TERMA0_ENB_DIG        |
 | +--------------------------------------+
 | 
 | void   enh_8g_csu_field_TERM_OVR_set             ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_TERM_OVR_get             ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_TERMB0_ENB_DIG_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_TERMB0_ENB_DIG_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_TERMB0_ENB_DIG_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_TERMB0_ENB_DIG_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_TERMA0_ENB_DIG_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_TERMA0_ENB_DIG_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_TERMA0_ENB_DIG_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_TERMA0_ENB_DIG_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000460 MABC_TEST_CON_REG |
 | +---------------------------------------+
 | | bit  6   R/W  CSU_ATMSB               |
 | | bit  5   R/W  CS_ATMSB                |
 | | bit  4:0 R/W  ATIN                    |
 | +---------------------------------------+
 | 
 | void   enh_8g_csu_field_CSU_ATMSB_set  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_ATMSB_get  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CS_ATMSB_set   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CS_ATMSB_get   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_ATIN_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_ATIN_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_ATIN_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_ATIN_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00000464 MABC_ENB_CON_REG |
 | +--------------------------------------+
 | | bit  0 R/W  ENB                      |
 | +--------------------------------------+
 | 
 | void   enh_8g_csu_field_ENB_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_ENB_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00000468 MABC_CS_MODE_CONFIG_REG |
 | +---------------------------------------------+
 | | bit  21:20 R/W  CS_DIVMODE                  |
 | | bit  17:13 R/W  CS_CTRL_7_3                 |
 | | bit  12    R/W  CS_CTRL_2                   |
 | | bit  11    R/W  CS_CTRL_1                   |
 | | bit  10    R/W  CS_CTRL_0                   |
 | | bit  7:6   R/W  CS_MODE_7_6                 |
 | | bit  5     R/W  CS_MODE_5                   |
 | | bit  4     R/W  CS_MODE_4                   |
 | | bit  3     R/W  CS_MODE_3                   |
 | | bit  2     R/W  CS_MODE_2                   |
 | | bit  1     R/W  CS_MODE_1                   |
 | | bit  0     R/W  CS_MODE_0                   |
 | +---------------------------------------------+
 | 
 | void   enh_8g_csu_field_CS_DIVMODE_set        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CS_DIVMODE_get        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CS_DIVMODE_set  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CS_DIVMODE_get  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CS_CTRL_7_3_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CS_CTRL_7_3_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CS_CTRL_7_3_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CS_CTRL_7_3_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CS_CTRL_2_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CS_CTRL_2_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CS_CTRL_1_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CS_CTRL_1_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CS_CTRL_0_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CS_CTRL_0_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CS_MODE_7_6_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CS_MODE_7_6_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CS_MODE_7_6_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CS_MODE_7_6_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CS_MODE_5_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CS_MODE_5_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CS_MODE_4_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CS_MODE_4_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CS_MODE_3_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CS_MODE_3_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CS_MODE_2_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CS_MODE_2_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CS_MODE_1_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CS_MODE_1_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CS_MODE_0_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CS_MODE_0_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +--------------------------------------------+
 | | Register 0x00000470 MABC_CSU_MODE_CNFG_REG |
 | +--------------------------------------------+
 | | bit  31:28 R/W  CSU_MODE_31_28             |
 | | bit  27    R/W  CSU_MODE_27                |
 | | bit  26:25 R/W  CSU_MODE_26_25             |
 | | bit  24:22 R/W  CSU_MODE_24_22             |
 | | bit  21    R/W  CSU_MODE_21                |
 | | bit  20    R/W  CSU_MODE_20                |
 | | bit  19    R/W  CSU_MODE_19                |
 | | bit  18:16 R/W  CSU_MODE_18_16             |
 | | bit  15:10 R/W  CSU_MODE_15_10             |
 | | bit  9:8   R/W  CSU_MODE_9_8               |
 | | bit  7:6   R/W  CSU_MODE_7_6               |
 | | bit  5:3   R/W  CSU_MODE_5_3               |
 | | bit  2:0   R/W  CSU_MODE_2_0               |
 | +--------------------------------------------+
 | 
 | void   enh_8g_csu_field_CSU_MODE_31_28_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_MODE_31_28_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CSU_MODE_31_28_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CSU_MODE_31_28_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CSU_MODE_27_set          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_MODE_27_get          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CSU_MODE_26_25_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_MODE_26_25_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CSU_MODE_26_25_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CSU_MODE_26_25_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CSU_MODE_24_22_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_MODE_24_22_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CSU_MODE_24_22_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CSU_MODE_24_22_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CSU_MODE_21_set          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_MODE_21_get          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CSU_MODE_20_set          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_MODE_20_get          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CSU_MODE_19_set          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_MODE_19_get          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CSU_MODE_18_16_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_MODE_18_16_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CSU_MODE_18_16_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CSU_MODE_18_16_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CSU_MODE_15_10_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_MODE_15_10_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CSU_MODE_15_10_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CSU_MODE_15_10_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CSU_MODE_9_8_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_MODE_9_8_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CSU_MODE_9_8_set   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CSU_MODE_9_8_get   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CSU_MODE_7_6_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_MODE_7_6_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CSU_MODE_7_6_set   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CSU_MODE_7_6_get   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CSU_MODE_5_3_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_MODE_5_3_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CSU_MODE_5_3_set   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CSU_MODE_5_3_get   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CSU_MODE_2_0_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_MODE_2_0_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CSU_MODE_2_0_set   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CSU_MODE_2_0_get   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000474 MABC_CNTL_REG |
 | +-----------------------------------+
 | | bit  19:17 R/W  CSU_CTRL_19_17    |
 | | bit  16    R/W  CSU_CTRL_16       |
 | | bit  15    R/W  CSU_CTRL_15       |
 | | bit  14:13 R/W  CSU_CTRL_14_13    |
 | | bit  12    R/W  CSU_CTRL_12       |
 | | bit  11    R/W  CSU_CTRL_11       |
 | | bit  10    R/W  CSU_CTRL_10       |
 | | bit  9:6   R/W  CSU_CTRL_9_6      |
 | | bit  5     R/W  CSU_CTRL_5        |
 | | bit  4     R/W  CSU_CTRL_4        |
 | | bit  3     R/W  CSU_CTRL_3        |
 | | bit  2     R/W  CSU_CTRL_2        |
 | | bit  1:0   R/W  CSU_CTRL_1_0      |
 | +-----------------------------------+
 | 
 | void   enh_8g_csu_field_CSU_CTRL_19_17_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_CTRL_19_17_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CSU_CTRL_19_17_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CSU_CTRL_19_17_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CSU_CTRL_16_set          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_CTRL_16_get          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CSU_CTRL_15_set          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_CTRL_15_get          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CSU_CTRL_14_13_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_CTRL_14_13_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CSU_CTRL_14_13_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CSU_CTRL_14_13_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CSU_CTRL_12_set          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_CTRL_12_get          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CSU_CTRL_11_set          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_CTRL_11_get          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CSU_CTRL_10_set          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_CTRL_10_get          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CSU_CTRL_9_6_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_CTRL_9_6_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CSU_CTRL_9_6_set   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CSU_CTRL_9_6_get   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CSU_CTRL_5_set           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_CTRL_5_get           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CSU_CTRL_4_set           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_CTRL_4_get           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CSU_CTRL_3_set           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_CTRL_3_get           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CSU_CTRL_2_set           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_CTRL_2_get           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CSU_CTRL_1_0_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CSU_CTRL_1_0_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_CSU_CTRL_1_0_set   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_CSU_CTRL_1_0_get   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000478 REF_DBLR_CALIB_STA_REG |
 | +--------------------------------------------+
 | | bit  6   R  REF_DCD_VAL                    |
 | | bit  5:0 R  DBL_CTRL_RD                    |
 | +--------------------------------------------+
 | 
 | UINT32 enh_8g_csu_field_REF_DCD_VAL_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 enh_8g_csu_field_DBL_CTRL_RD_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | UINT32 enh_8g_csu_field_range_DBL_CTRL_RD_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000480 E_ADSM_CNFG_REG_1 |
 | +---------------------------------------+
 | | bit  3 R/W  CLKO_POLARITY             |
 | | bit  2 R/W  DSM_RSTB                  |
 | | bit  1 R/W  MULT2                     |
 | | bit  0 R/W  SEED                      |
 | +---------------------------------------+
 | 
 | void   enh_8g_csu_field_CLKO_POLARITY_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CLKO_POLARITY_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_DSM_RSTB_set      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_DSM_RSTB_get      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_MULT2_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_MULT2_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_SEED_set          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_SEED_get          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000484 E_ADSM_CNFG_REG_2 |
 | +---------------------------------------+
 | | bit  31:0 R/W  FRAC_DIV_LOWER         |
 | +---------------------------------------+
 | 
 | void   enh_8g_csu_field_FRAC_DIV_LOWER_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_FRAC_DIV_LOWER_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_FRAC_DIV_LOWER_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_FRAC_DIV_LOWER_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000488 E_ADSM_CNFG_REG_3 |
 | +---------------------------------------+
 | | bit  7:0 R/W  FRAC_DIV_UPPER          |
 | +---------------------------------------+
 | 
 | void   enh_8g_csu_field_FRAC_DIV_UPPER_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_FRAC_DIV_UPPER_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_FRAC_DIV_UPPER_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_FRAC_DIV_UPPER_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x0000048c E_ADSM_DENOMIN_LOW_CNFG_REG |
 | +-------------------------------------------------+
 | | bit  31:0 R/W  DENOMIN_LOWER                    |
 | +-------------------------------------------------+
 | 
 | void   enh_8g_csu_field_DENOMIN_LOWER_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_DENOMIN_LOWER_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_DENOMIN_LOWER_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_DENOMIN_LOWER_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000490 E_ADSM_DENOMIN_UP_CNFG_REG |
 | +------------------------------------------------+
 | | bit  7:0 R/W  DENOMIN_UPPER                    |
 | +------------------------------------------------+
 | 
 | void   enh_8g_csu_field_DENOMIN_UPPER_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_DENOMIN_UPPER_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_DENOMIN_UPPER_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_DENOMIN_UPPER_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000494 E_ADSM_ID_DS_CNFG_REG |
 | +-------------------------------------------+
 | | bit  11:10 R/W  DITHER_SEL                |
 | | bit  9:0   R/W  INT_DIV                   |
 | +-------------------------------------------+
 | 
 | void   enh_8g_csu_field_DITHER_SEL_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_DITHER_SEL_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_DITHER_SEL_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_DITHER_SEL_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_INT_DIV_set          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_INT_DIV_get          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_INT_DIV_set    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_INT_DIV_get    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000498 EADSM_FRAC_DIV_MOD_OVR_REG |
 | +------------------------------------------------+
 | | bit  29   R/W  FRAC_DIV_MOD_OVR_ENB            |
 | | bit  24:0 R/W  FRAC_DIV_MOD_LOW_OVR_VAL        |
 | +------------------------------------------------+
 | 
 | void   enh_8g_csu_field_FRAC_DIV_MOD_OVR_ENB_set           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_FRAC_DIV_MOD_OVR_ENB_get           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_FRAC_DIV_MOD_LOW_OVR_VAL_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_FRAC_DIV_MOD_LOW_OVR_VAL_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_FRAC_DIV_MOD_LOW_OVR_VAL_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_FRAC_DIV_MOD_LOW_OVR_VAL_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x0000049c EADSM_FRAC_DIV_MOD_UP_OVR_REG |
 | +---------------------------------------------------+
 | | bit  15:0 R/W  FRAC_DIV_MOD_UP_OVR_VAL            |
 | +---------------------------------------------------+
 | 
 | void   enh_8g_csu_field_FRAC_DIV_MOD_UP_OVR_VAL_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_FRAC_DIV_MOD_UP_OVR_VAL_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_FRAC_DIV_MOD_UP_OVR_VAL_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_FRAC_DIV_MOD_UP_OVR_VAL_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x000004a0 CNI_UNI_CSU_INT_REG   |
 | +-------------------------------------------+
 | | bit  12 R/W  OC_THRESHOLD_CHECK_FAILB_I12 |
 | | bit  11 R/W  VCOC_MODE_IN_RANGE_I11       |
 | | bit  10 R/W  VCOC_BIST_ERR_I10            |
 | | bit  8  R/W  PPM_OFFSET_UPD_I8            |
 | | bit  7  R/W  DIVCLK_DEAD_I7               |
 | | bit  6  R/W  CALIB_DONE_I6                |
 | | bit  5  R/W  OCC_DONE_I5                  |
 | | bit  4  R/W  LOCK_LOSS_I4                 |
 | | bit  3  R/W  LOCK_GAIN_I3                 |
 | | bit  2  R/W  VCOC_DONE_I2                 |
 | | bit  1  R/W  IN_RANGE_N_FAILS_I1          |
 | | bit  0  R/W  IN_RANGE_N_PASSES_I0         |
 | +-------------------------------------------+
 | 
 | void                 enh_8g_csu_field_PPM_OFFSET_UPD_I8_set_to_clear            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32               enh_8g_csu_field_PPM_OFFSET_UPD_I8_get                     ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_PPM_OFFSET_UPD_I8_poll                    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_I12_set_to_clear ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32               enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_I12_get          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_I12_poll         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enh_8g_csu_field_VCOC_MODE_IN_RANGE_I11_set_to_clear       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32               enh_8g_csu_field_VCOC_MODE_IN_RANGE_I11_get                ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_MODE_IN_RANGE_I11_poll               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enh_8g_csu_field_VCOC_BIST_ERR_I10_set_to_clear            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32               enh_8g_csu_field_VCOC_BIST_ERR_I10_get                     ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_BIST_ERR_I10_poll                    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enh_8g_csu_field_DIVCLK_DEAD_I7_set_to_clear               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32               enh_8g_csu_field_DIVCLK_DEAD_I7_get                        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_DIVCLK_DEAD_I7_poll                       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enh_8g_csu_field_CALIB_DONE_I6_set_to_clear                ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32               enh_8g_csu_field_CALIB_DONE_I6_get                         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_CALIB_DONE_I6_poll                        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enh_8g_csu_field_OCC_DONE_I5_set_to_clear                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32               enh_8g_csu_field_OCC_DONE_I5_get                           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_OCC_DONE_I5_poll                          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enh_8g_csu_field_LOCK_LOSS_I4_set_to_clear                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32               enh_8g_csu_field_LOCK_LOSS_I4_get                          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_LOCK_LOSS_I4_poll                         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enh_8g_csu_field_LOCK_GAIN_I3_set_to_clear                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32               enh_8g_csu_field_LOCK_GAIN_I3_get                          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_LOCK_GAIN_I3_poll                         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enh_8g_csu_field_VCOC_DONE_I2_set_to_clear                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32               enh_8g_csu_field_VCOC_DONE_I2_get                          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_DONE_I2_poll                         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enh_8g_csu_field_IN_RANGE_N_FAILS_I1_set_to_clear          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32               enh_8g_csu_field_IN_RANGE_N_FAILS_I1_get                   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_IN_RANGE_N_FAILS_I1_poll                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 enh_8g_csu_field_IN_RANGE_N_PASSES_I0_set_to_clear         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32               enh_8g_csu_field_IN_RANGE_N_PASSES_I0_get                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_IN_RANGE_N_PASSES_I0_poll                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------+
 | | Register 0x000004a4 CNI_UNI_CSU_INT_ENB_REG |
 | +---------------------------------------------+
 | | bit  12 R/W  OC_THRESHOLD_CHECK_FAILB_E12   |
 | | bit  11 R/W  VCOC_MODE_IN_RANGE_E11         |
 | | bit  10 R/W  VCOC_BIST_ERR_E10              |
 | | bit  8  R/W  PPM_OFFSET_UPD_E8              |
 | | bit  7  R/W  DIVCLK_DEAD_E7                 |
 | | bit  6  R/W  CALIB_DONE_E6                  |
 | | bit  5  R/W  OCC_DONE_E5                    |
 | | bit  4  R/W  LOCK_LOSS_E4                   |
 | | bit  3  R/W  LOCK_GAIN_E3                   |
 | | bit  2  R/W  VCOC_DONE_E2                   |
 | | bit  1  R/W  IN_RANGE_N_FAILS_E1            |
 | | bit  0  R/W  IN_RANGE_N_PASSES_E0           |
 | +---------------------------------------------+
 | 
 | void   enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_E12_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_E12_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_VCOC_MODE_IN_RANGE_E11_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_VCOC_MODE_IN_RANGE_E11_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_VCOC_BIST_ERR_E10_set            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_VCOC_BIST_ERR_E10_get            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_PPM_OFFSET_UPD_E8_set            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_PPM_OFFSET_UPD_E8_get            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_DIVCLK_DEAD_E7_set               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_DIVCLK_DEAD_E7_get               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CALIB_DONE_E6_set                ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CALIB_DONE_E6_get                ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_OCC_DONE_E5_set                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_OCC_DONE_E5_get                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_LOCK_LOSS_E4_set                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_LOCK_LOSS_E4_get                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_LOCK_GAIN_E3_set                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_LOCK_GAIN_E3_get                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_VCOC_DONE_E2_set                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_VCOC_DONE_E2_get                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_IN_RANGE_N_FAILS_E1_set          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_IN_RANGE_N_FAILS_E1_get          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_IN_RANGE_N_PASSES_E0_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_IN_RANGE_N_PASSES_E0_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x000004a8 CNI_UNI_CSU_INT_VAL_REG |
 | +---------------------------------------------+
 | | bit  12 R  OC_THRESHOLD_CHECK_FAILB_V12     |
 | | bit  11 R  VCOC_MODE_IN_RANGE_V11           |
 | | bit  10 R  VCOC_BIST_ERR_V10                |
 | | bit  8  R  PPM_OFFSET_UPD_V8                |
 | | bit  7  R  DIVCLK_DEAD_V7                   |
 | | bit  6  R  CALIB_DONE_V6                    |
 | | bit  5  R  OCC_DONE_V5                      |
 | | bit  4  R  LOCK_LOSS_V4                     |
 | | bit  3  R  LOCK_GAIN_V3                     |
 | | bit  2  R  VCOC_DONE_V2                     |
 | | bit  1  R  IN_RANGE_N_FAILS_V1              |
 | | bit  0  R  IN_RANGE_N_PASSES_V0             |
 | +---------------------------------------------+
 | 
 | UINT32               enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_V12_get  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_OC_THRESHOLD_CHECK_FAILB_V12_poll ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               enh_8g_csu_field_VCOC_MODE_IN_RANGE_V11_get        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_MODE_IN_RANGE_V11_poll       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               enh_8g_csu_field_VCOC_BIST_ERR_V10_get             ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_BIST_ERR_V10_poll            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               enh_8g_csu_field_PPM_OFFSET_UPD_V8_get             ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_PPM_OFFSET_UPD_V8_poll            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               enh_8g_csu_field_DIVCLK_DEAD_V7_get                ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_DIVCLK_DEAD_V7_poll               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               enh_8g_csu_field_CALIB_DONE_V6_get                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_CALIB_DONE_V6_poll                ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               enh_8g_csu_field_OCC_DONE_V5_get                   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_OCC_DONE_V5_poll                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               enh_8g_csu_field_LOCK_LOSS_V4_get                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_LOCK_LOSS_V4_poll                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               enh_8g_csu_field_LOCK_GAIN_V3_get                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_LOCK_GAIN_V3_poll                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               enh_8g_csu_field_VCOC_DONE_V2_get                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_VCOC_DONE_V2_poll                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               enh_8g_csu_field_IN_RANGE_N_FAILS_V1_get           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_IN_RANGE_N_FAILS_V1_poll          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               enh_8g_csu_field_IN_RANGE_N_PASSES_V0_get          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_field_IN_RANGE_N_PASSES_V0_poll         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x000004ac REFCLK_DBLR_CNFG_REG_1 |
 | +--------------------------------------------+
 | | bit  29:18 R/W  DAC_DELAY                  |
 | | bit  17:10 R/W  ACC_DELAY                  |
 | | bit  9:2   R/W  N_ACCUM_CYCLES             |
 | | bit  1     R/W  CALIB_PROG_EN              |
 | | bit  0     R/W  CALIB                      |
 | +--------------------------------------------+
 | 
 | void   enh_8g_csu_field_DAC_DELAY_set            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_DAC_DELAY_get            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_DAC_DELAY_set      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_DAC_DELAY_get      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_ACC_DELAY_set            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_ACC_DELAY_get            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_ACC_DELAY_set      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_ACC_DELAY_get      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_N_ACCUM_CYCLES_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_N_ACCUM_CYCLES_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_N_ACCUM_CYCLES_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_N_ACCUM_CYCLES_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_CALIB_PROG_EN_set        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CALIB_PROG_EN_get        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_CALIB_set                ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_CALIB_get                ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +--------------------------------------------+
 | | Register 0x000004b0 REFCLK_DBLR_CNFG_REG_2 |
 | +--------------------------------------------+
 | | bit  8   R/W  REF_DCD_INV                  |
 | | bit  7   R/W  DBL_CTRL_BIT5_INV            |
 | | bit  6:1 R/W  DBL_CTRL_OVR                 |
 | | bit  0   R/W  DBL_CTRL_OVR_EN              |
 | +--------------------------------------------+
 | 
 | void   enh_8g_csu_field_REF_DCD_INV_set        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_REF_DCD_INV_get        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_DBL_CTRL_BIT5_INV_set  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_DBL_CTRL_BIT5_INV_get  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_DBL_CTRL_OVR_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_DBL_CTRL_OVR_get       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_DBL_CTRL_OVR_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_DBL_CTRL_OVR_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   enh_8g_csu_field_DBL_CTRL_OVR_EN_set    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_DBL_CTRL_OVR_EN_get    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | +--------------------------------------------+
 | | Register 0x000004b4 ACB_CNFG_REG_1         |
 | +--------------------------------------------+
 | | bit  23    R/W  PLL_S_OUT_FRAC_DIV_MOD_SEL |
 | | bit  22:19 R/W  ACB_CLK_FACT               |
 | +--------------------------------------------+
 | 
 | void   enh_8g_csu_field_PLL_S_OUT_FRAC_DIV_MOD_SEL_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_PLL_S_OUT_FRAC_DIV_MOD_SEL_get ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_ACB_CLK_FACT_set               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | UINT32 enh_8g_csu_field_ACB_CLK_FACT_get               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void   enh_8g_csu_field_range_ACB_CLK_FACT_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 enh_8g_csu_field_range_ACB_CLK_FACT_get         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ENH_8G_CSU_IO_H
#define _ENH_8G_CSU_IO_H

#include "sfi51_d8_tx_18x.h"
#include "enh_8g_csu_regs.h"
#include "enh_8g_csu_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 enh_8g_csu_buffer_init               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | void                 enh_8g_csu_buffer_flush              ( enh_8g_csu_buffer_t *b_ptr )
 | UINT32               enh_8g_csu_reg_read                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 enh_8g_csu_reg_write                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 enh_8g_csu_field_set                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 enh_8g_csu_action_on_write_field_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 enh_8g_csu_burst_read                ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 enh_8g_csu_burst_write               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_poll                      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_write                     ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_field_set                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_CNI_UNI_CSU_RST_REG_read                      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_MABC_RST_CNFG_REG_write                       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_MABC_RST_CNFG_REG_field_set                   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_MABC_RST_CNFG_REG_read                        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_write                      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_field_set                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_CLK_DGLCH_CNFG_REG_read                       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_PLL_CNFG_REG1_write                           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_PLL_CNFG_REG1_field_set                       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_PLL_CNFG_REG1_read                            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_PLL_CLK_CNTR_REG_write                        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_PLL_CLK_CNTR_REG_field_set                    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_PLL_CLK_CNTR_REG_read                         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_PLL_CNTR_REG_write                            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_PLL_CNTR_REG_field_set                        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_PLL_CNTR_REG_read                             ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_write                      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_field_set                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_VCO_CAL_CNFG_REG_1_read                       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_write                      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_field_set                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_VCO_CAL_CNFG_REG_2_read                       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_write                     ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_field_set                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_OCC_TXRX_CNFG_REG_1_read                      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_write                     ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_field_set                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_OCC_TXRX_CNFG_REG_2_read                      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_write                     ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_field_set                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_OCC_TXRX_CNFG_REG_3_read                      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_CLK_DIV_CNFG_REG_write                        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_CLK_DIV_CNFG_REG_field_set                    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_CLK_DIV_CNFG_REG_read                         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_MABC_TC_CNFG_REG_write                        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_MABC_TC_CNFG_REG_field_set                    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_MABC_TC_CNFG_REG_read                         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_MABC_TEST_CON_REG_write                       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_MABC_TEST_CON_REG_field_set                   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_MABC_TEST_CON_REG_read                        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_MABC_ENB_CON_REG_write                        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_MABC_ENB_CON_REG_field_set                    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_MABC_ENB_CON_REG_read                         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_write                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_field_set             ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_MABC_CS_MODE_CONFIG_REG_read                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_write                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_field_set              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_MABC_CSU_MODE_CNFG_REG_read                   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_MABC_CNTL_REG_write                           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_MABC_CNTL_REG_field_set                       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_MABC_CNTL_REG_read                            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_E_ADSM_CNFG_REG_1_write                       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_E_ADSM_CNFG_REG_1_field_set                   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_E_ADSM_CNFG_REG_1_read                        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_E_ADSM_CNFG_REG_2_write                       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_E_ADSM_CNFG_REG_2_field_set                   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_E_ADSM_CNFG_REG_2_read                        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_E_ADSM_CNFG_REG_3_write                       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_E_ADSM_CNFG_REG_3_field_set                   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_E_ADSM_CNFG_REG_3_read                        ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_write             ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_field_set         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_E_ADSM_DENOMIN_LOW_CNFG_REG_read              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_write              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_field_set          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_E_ADSM_DENOMIN_UP_CNFG_REG_read               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_write                   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_field_set               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_E_ADSM_ID_DS_CNFG_REG_read                    ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_write              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_field_set          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_OVR_REG_read               ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_write           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_field_set       ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_EADSM_FRAC_DIV_MOD_UP_OVR_REG_read            ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_write                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_field_set              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_1_read                   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_write                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_field_set              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_REFCLK_DBLR_CNFG_REG_2_read                   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_ACB_CNFG_REG_1_write                          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_ACB_CNFG_REG_1_field_set                      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_ACB_CNFG_REG_1_read                           ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | void                 enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_write                     ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_action_on_write_field_set ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_read                      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_reg_CNI_UNI_CSU_INT_REG_poll                      ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_write                 ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 value )
 | void                 enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_field_set             ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               enh_8g_csu_reg_CNI_UNI_CSU_INT_ENB_REG_read                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | UINT32               enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_read                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE enh_8g_csu_reg_CNI_UNI_CSU_INT_VAL_REG_poll                  ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               enh_8g_csu_reg_PLL_STA_REG_read                              ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | UINT32               enh_8g_csu_reg_PLL_LOCK_DET_PPM_OFF_STA_REG_read             ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | UINT32               enh_8g_csu_reg_VCO_CAL_STA_REG_read                          ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | UINT32               enh_8g_csu_reg_OCC_TXRX_STA_REG_read                         ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 | UINT32               enh_8g_csu_reg_REF_DBLR_CALIB_STA_REG_read                   ( enh_8g_csu_buffer_t *b_ptr, sfi51_d8_tx_18x_handle_t *h_ptr )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ENH_8G_CSU_IO_H */
