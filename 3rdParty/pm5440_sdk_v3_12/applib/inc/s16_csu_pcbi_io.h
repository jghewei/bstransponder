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
 *     and register accessor functions for the s16_csu_pcbi block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing s16_csu_pcbi io functions is:
 | {
 |     s16_csu_pcbi_buffer_t b_ptr[1];
 |     s16_csu_pcbi_buffer_init( b_ptr, h_ptr, A );
 |     value = s16_csu_pcbi_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     s16_csu_pcbi_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     s16_csu_pcbi_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = s16_csu_pcbi_field_<FIELD>_get( NULL, h_ptr, A );
 | s16_csu_pcbi_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-------------------------------------------+
 | | Register 0x00000500 CSU0_PLL_OOL_STAT_REG |
 | +-------------------------------------------+
 | | bit  20:0 R  CSU0_PPM_OFF_RD_VAL          |
 | +-------------------------------------------+
 | 
 | UINT32 s16_csu_pcbi_field_CSU0_PPM_OFF_RD_VAL_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_csu_pcbi_field_range_CSU0_PPM_OFF_RD_VAL_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000508 CSU0_VCOC_STAT_REG |
 | +----------------------------------------+
 | | bit  15   R  VCOC_MODE_IN_RANGE        |
 | | bit  14:7 R  CSU0_VCOC_BIST_MODE_COUNT |
 | | bit  6:1  R  CSU0_VCOC_MODE_OUT        |
 | +----------------------------------------+
 | 
 | UINT32 s16_csu_pcbi_field_VCOC_MODE_IN_RANGE_get              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_csu_pcbi_field_CSU0_VCOC_BIST_MODE_COUNT_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_csu_pcbi_field_range_CSU0_VCOC_BIST_MODE_COUNT_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 s16_csu_pcbi_field_CSU0_VCOC_MODE_OUT_get              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_csu_pcbi_field_range_CSU0_VCOC_MODE_OUT_get        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x0000050c CSU0_EADSM_LOCK_DET_STAT_REG |
 | +--------------------------------------------------+
 | | bit  17:0 R  CSU0_EADSM_SYSCLK_CNT               |
 | +--------------------------------------------------+
 | 
 | UINT32 s16_csu_pcbi_field_CSU0_EADSM_SYSCLK_CNT_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_csu_pcbi_field_range_CSU0_EADSM_SYSCLK_CNT_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x00000510 CSU0_PLL_OOL_CONFIG_REG1 |
 | +----------------------------------------------+
 | | bit  23   R/W  CSU0_SW_RESET                 |
 | | bit  22   R/W  CSU0_PLL_RD                   |
 | | bit  20   R/W  CSU0_RESET_HISTORY            |
 | | bit  19:0 R/W  CSU0_CLK_CNT_MAX              |
 | +----------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_SW_RESET_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_SW_RESET_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_RESET_HISTORY_set     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_RESET_HISTORY_get     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_CLK_CNT_MAX_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CLK_CNT_MAX_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_CLK_CNT_MAX_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_CLK_CNT_MAX_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_PLL_RD_set            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_PLL_RD_get            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------------+
 | | Register 0x00000514 CSU0_PLL_OOL_CONFIG_REG2 |
 | +----------------------------------------------+
 | | bit  31:16 R/W  CSU0_PASS_CNT_MAX            |
 | | bit  15:0  R/W  CSU0_FAIL_CNT_MAX            |
 | +----------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_PASS_CNT_MAX_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_PASS_CNT_MAX_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_PASS_CNT_MAX_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_PASS_CNT_MAX_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_FAIL_CNT_MAX_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_FAIL_CNT_MAX_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_FAIL_CNT_MAX_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_FAIL_CNT_MAX_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x00000518 CSU0_PLL_OOL_CONFIG_REG3 |
 | +----------------------------------------------+
 | | bit  31:28 R/W  CSU0_LOCK_PRECISION          |
 | | bit  27:20 R/W  CSU0_LOCK_CNT_MAX            |
 | +----------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_LOCK_PRECISION_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_LOCK_PRECISION_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_LOCK_PRECISION_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_LOCK_PRECISION_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_LOCK_CNT_MAX_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_LOCK_CNT_MAX_get         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_LOCK_CNT_MAX_set   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_LOCK_CNT_MAX_get   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x0000051c CSU0_MABC_TEST_CONFIG_REG |
 | +-----------------------------------------------+
 | | bit  17    R/W  VBIAS_CSU_ATMSB               |
 | | bit  16    R/W  CSU_ENB                       |
 | | bit  15    R/W  CSU_CTRL_15                   |
 | | bit  14:13 R/W  CSU_CTRL_14_13                |
 | | bit  12    R/W  CSU_CTRL_12                   |
 | | bit  11    R/W  CSU_CTRL_11                   |
 | | bit  10    R/W  CSU_CTRL_10                   |
 | | bit  9:6   R/W  CSU_CTRL_9_6                  |
 | | bit  5     R/W  CSU_CTRL_5                    |
 | | bit  4     R/W  CSU_CTRL_4                    |
 | | bit  3     R/W  CSU_CTRL_3                    |
 | | bit  2     R/W  CSU_CTRL_2                    |
 | | bit  1:0   R/W  CSU_CTRL_1_0                  |
 | +-----------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_VBIAS_CSU_ATMSB_set      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_VBIAS_CSU_ATMSB_get      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU_ENB_set              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU_ENB_get              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU_CTRL_15_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU_CTRL_15_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU_CTRL_14_13_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU_CTRL_14_13_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU_CTRL_14_13_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU_CTRL_14_13_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU_CTRL_12_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU_CTRL_12_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU_CTRL_11_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU_CTRL_11_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU_CTRL_10_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU_CTRL_10_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU_CTRL_9_6_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU_CTRL_9_6_get         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU_CTRL_9_6_set   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU_CTRL_9_6_get   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU_CTRL_5_set           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU_CTRL_5_get           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU_CTRL_4_set           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU_CTRL_4_get           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU_CTRL_3_set           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU_CTRL_3_get           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU_CTRL_2_set           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU_CTRL_2_get           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU_CTRL_1_0_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU_CTRL_1_0_get         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU_CTRL_1_0_set   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU_CTRL_1_0_get   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00000520 CSU0_CONFIG_REG2 |
 | +--------------------------------------+
 | | bit  29    R/W  CSU0_CSU_MODE_29     |
 | | bit  28    R/W  CSU0_CSU_MODE_28     |
 | | bit  27    R/W  CSU0_CSU_MODE_27     |
 | | bit  26:25 R/W  CSU0_CSU_MODE_26_25  |
 | | bit  24:23 R/W  CSU0_CSU_MODE_24_23  |
 | | bit  22    R/W  CSU0_CSU_MODE_22     |
 | | bit  21    R/W  CSU0_CSU_MODE_21     |
 | | bit  20    R/W  CSU0_CSU_MODE_20     |
 | | bit  19    R/W  CSU0_CSU_MODE_19     |
 | | bit  17:16 R/W  CSU0_CSU_MODE_17_16  |
 | | bit  15:10 R/W  CSU0_CSU_MODE_15_10  |
 | | bit  9:8   R/W  CSU0_CSU_MODE_9_8    |
 | | bit  7:6   R/W  CSU0_CSU_MODE_7_6    |
 | | bit  5:3   R/W  CSU0_CSU_MODE_5_3    |
 | | bit  2:0   R/W  CSU0_CSU_MODE_2_0    |
 | +--------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_29_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_29_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_28_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_28_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_27_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_27_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_26_25_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_26_25_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_CSU_MODE_26_25_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_26_25_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_24_23_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_24_23_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_CSU_MODE_24_23_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_24_23_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_22_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_22_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_21_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_21_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_20_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_20_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_19_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_19_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_17_16_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_17_16_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_CSU_MODE_17_16_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_17_16_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_15_10_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_15_10_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_CSU_MODE_15_10_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_15_10_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_9_8_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_9_8_get         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_CSU_MODE_9_8_set   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_9_8_get   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_7_6_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_7_6_get         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_CSU_MODE_7_6_set   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_7_6_get   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_5_3_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_5_3_get         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_CSU_MODE_5_3_set   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_5_3_get   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_CSU_MODE_2_0_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_MODE_2_0_get         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_CSU_MODE_2_0_set   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_CSU_MODE_2_0_get   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000524 CSU0_CONFIG_REG3  |
 | +---------------------------------------+
 | | bit  4   R/W  CSU0_CSU_FB_CTRL_OVR_EN |
 | | bit  3:0 R/W  CSU0_CSU_FB_CTRL        |
 | +---------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_OVR_EN_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_OVR_EN_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_set        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CSU_FB_CTRL_get        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_CSU_FB_CTRL_set  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_CSU_FB_CTRL_get  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000528 CSU0_MABC_TEST_CONFIG_REG3 |
 | +------------------------------------------------+
 | | bit  8:4 R/W  CSU0_VC_LPF_CODE                 |
 | | bit  3:0 R/W  CSU0_VC_LPF_MODE                 |
 | +------------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_VC_LPF_CODE_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_VC_LPF_CODE_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_VC_LPF_CODE_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_VC_LPF_CODE_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_VC_LPF_MODE_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_VC_LPF_MODE_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_VC_LPF_MODE_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_VC_LPF_MODE_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0000052c CSU0_VCOC_CONFIG_REG1 |
 | +-------------------------------------------+
 | | bit  17:12 R/W  CSU0_MAX_THRESHOLD        |
 | | bit  11:6  R/W  CSU0_MIN_THRESHOLD        |
 | | bit  5     R/W  CSU0_VCOC_START           |
 | | bit  4:1   R/W  CSU0_VCOC_LOOP            |
 | | bit  0     R/W  CSU0_VCOC_EN              |
 | +-------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_MAX_THRESHOLD_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_MAX_THRESHOLD_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_MAX_THRESHOLD_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_MAX_THRESHOLD_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_MIN_THRESHOLD_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_MIN_THRESHOLD_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_MIN_THRESHOLD_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_MIN_THRESHOLD_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_VCOC_START_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_VCOC_START_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_VCOC_LOOP_set           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_VCOC_LOOP_get           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_VCOC_LOOP_set     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_VCOC_LOOP_get     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_VCOC_EN_set             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_VCOC_EN_get             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------------------+
 | | Register 0x00000530 CSU0_VCOC_CONFIG_REG2 |
 | +-------------------------------------------+
 | | bit  31:24 R/W  CSU0_VCOC_SETTLING_TIME   |
 | | bit  23:18 R/W  CSU0_MIN_MODE             |
 | | bit  17:12 R/W  CSU0_MAX_MODE             |
 | | bit  11    R/W  CSU0_VCOC_BIST_EN         |
 | | bit  10:5  R/W  CSU0_OFF_EQ_WINDOW        |
 | | bit  4     R/W  CSU0_SWEEP_DIR            |
 | +-------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_VCOC_SETTLING_TIME_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_VCOC_SETTLING_TIME_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_VCOC_SETTLING_TIME_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_VCOC_SETTLING_TIME_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_MIN_MODE_set                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_MIN_MODE_get                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_MIN_MODE_set           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_MIN_MODE_get           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_MAX_MODE_set                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_MAX_MODE_get                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_MAX_MODE_set           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_MAX_MODE_get           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_VCOC_BIST_EN_set             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_VCOC_BIST_EN_get             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_OFF_EQ_WINDOW_set            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_OFF_EQ_WINDOW_get            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_OFF_EQ_WINDOW_set      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_OFF_EQ_WINDOW_get      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_SWEEP_DIR_set                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_SWEEP_DIR_get                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------------------+
 | | Register 0x00000534 CSU0_RESET_CONFIG_REG |
 | +-------------------------------------------+
 | | bit  4 R/W  CSU0_ADSM_SFT_RSTB            |
 | | bit  3 R/W  CSU0_ADSM_HRD_RSTB            |
 | | bit  2 R/W  CSU0_PLL_RSTB                 |
 | | bit  1 R/W  CSU_ARSTB                     |
 | | bit  0 R/W  CSU0_VCOC_RSTB                |
 | +-------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_ADSM_SFT_RSTB_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_ADSM_SFT_RSTB_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_ADSM_HRD_RSTB_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_ADSM_HRD_RSTB_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_PLL_RSTB_set      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_PLL_RSTB_get      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU_ARSTB_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU_ARSTB_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_VCOC_RSTB_set     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_VCOC_RSTB_get     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------+
 | | Register 0x00000538 CSU_TEST_CONFIG_REG |
 | +-----------------------------------------+
 | | bit  0 R/W  CSU_ATMSB                   |
 | +-----------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU_ATMSB_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU_ATMSB_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------------+
 | | Register 0x0000053c CLK_DEGLITCHER_CONFIG_REG |
 | +-----------------------------------------------+
 | | bit  1 R/W  SEL_DIVCLK_16_OR_64               |
 | | bit  0 R/W  CLK_DEGLITCHER_EN                 |
 | +-----------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_SEL_DIVCLK_16_OR_64_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_SEL_DIVCLK_16_OR_64_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CLK_DEGLITCHER_EN_set   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CLK_DEGLITCHER_EN_get   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------------+
 | | Register 0x00000540 GLOBAL_RESET_CONFIG_REG |
 | +---------------------------------------------+
 | | bit  0 R/W  GLBL_RSTB                       |
 | +---------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_GLBL_RSTB_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_GLBL_RSTB_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000548 CSU0_FRAC_DIV_CONFIG_REG1 |
 | +-----------------------------------------------+
 | | bit  26 R/W  CSU0_FRAC_DIV_MOD_EXT_EN         |
 | | bit  25 R/W  CSU0_FRAC_DIV_MOD_OVR_EN         |
 | +-----------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_EXT_EN_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_EN_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_EN_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------------+
 | | Register 0x0000054c CSU0_FRAC_DIV_CONFIG_REG2 |
 | +-----------------------------------------------+
 | | bit  15:0 R/W  CSU0_FRAC_DIV_MOD_OVR_31_16    |
 | +-----------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_31_16_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_31_16_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_31_16_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_31_16_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000550 CSU0_FRAC_DIV_CONFIG_REG3 |
 | +-----------------------------------------------+
 | | bit  8:0 R/W  CSU0_FRAC_DIV_MOD_OVR_40_32     |
 | +-----------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_40_32_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_MOD_OVR_40_32_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_40_32_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_FRAC_DIV_MOD_OVR_40_32_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x00000554 CSU0_EADSM_LOCK_DET_CONFIG_REG |
 | +----------------------------------------------------+
 | | bit  23:6 R/W  CSU0_EADSM_CLK_CNT_MAX              |
 | | bit  5:0  R/W  CSU0_EADSM_LOCK_CNT                 |
 | +----------------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_EADSM_CLK_CNT_MAX_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_EADSM_CLK_CNT_MAX_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_EADSM_CLK_CNT_MAX_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_EADSM_CLK_CNT_MAX_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_EADSM_LOCK_CNT_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCK_CNT_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_EADSM_LOCK_CNT_set    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_EADSM_LOCK_CNT_get    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000558 CSU0_EADSM_CONFIG_REG1 |
 | +--------------------------------------------+
 | | bit  4:3 R/W  CSU0_DITHER_SEL              |
 | | bit  2   R/W  CSU0_MULT2                   |
 | | bit  1   R/W  CSU0_SEED                    |
 | | bit  0   R/W  CSU0_CLKO_POLARITY           |
 | +--------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_DITHER_SEL_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_DITHER_SEL_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_DITHER_SEL_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_DITHER_SEL_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_CSU0_MULT2_set            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_MULT2_get            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_SEED_set             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_SEED_get             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_CLKO_POLARITY_set    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_CLKO_POLARITY_get    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------------+
 | | Register 0x0000055c CSU0_EADSM_CONFIG_REG2 |
 | +--------------------------------------------+
 | | bit  9:0 R/W  CSU0_INT_DIV                 |
 | +--------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_INT_DIV_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_INT_DIV_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_INT_DIV_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_INT_DIV_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000560 CSU0_EADSM_CONFIG_REG3 |
 | +--------------------------------------------+
 | | bit  31:0 R/W  CSU0_DENOMIN_31_0           |
 | +--------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_DENOMIN_31_0_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_DENOMIN_31_0_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_DENOMIN_31_0_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_DENOMIN_31_0_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000564 CSU0_EADSM_CONFIG_REG4 |
 | +--------------------------------------------+
 | | bit  7:0 R/W  CSU0_DENOMIN_39_32           |
 | +--------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_DENOMIN_39_32_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_DENOMIN_39_32_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_DENOMIN_39_32_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_DENOMIN_39_32_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000568 CSU0_EADSM_CONFIG_REG5 |
 | +--------------------------------------------+
 | | bit  31:0 R/W  CSU0_FRAC_DIV_31_0          |
 | +--------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_FRAC_DIV_31_0_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_31_0_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_FRAC_DIV_31_0_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_FRAC_DIV_31_0_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0000056c CSU0_EADSM_CONFIG_REG6 |
 | +--------------------------------------------+
 | | bit  7:0 R/W  CSU0_FRAC_DIV_39_32          |
 | +--------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_CSU0_FRAC_DIV_39_32_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_FRAC_DIV_39_32_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_CSU0_FRAC_DIV_39_32_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_CSU0_FRAC_DIV_39_32_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000570 INT_EN_REG                      |
 | +-----------------------------------------------------+
 | | bit  14 R/W  UPDATE_RATE_OVERFLOW_E                 |
 | | bit  13 R/W  ACB_DESKEW_OVERFLOW_E                  |
 | | bit  12 R/W  ACB_DESKEW_UNDERFLOW_E                 |
 | | bit  11 R/W  VC_LPF_OUT1_E                          |
 | | bit  10 R/W  VC_LPF_OUT0_E                          |
 | | bit  9  R/W  CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E |
 | | bit  8  R/W  CSU0_EADSM_LOCKDET_LOCK_E              |
 | | bit  7  R/W  CSU0_PLL_LOCKDET_PPM_OFF_UPD_E         |
 | | bit  6  R/W  CSU0_VCOC_BIST_ERR_E                   |
 | | bit  5  R/W  CSU0_DIVCLK_DEAD_E                     |
 | | bit  4  R/W  CSU0_IN_RANGE_N_FAILS_E                |
 | | bit  3  R/W  CSU0_IN_RANGE_N_PASSES_E               |
 | | bit  2  R/W  CSU0_LOCK_LOSS_E                       |
 | | bit  1  R/W  CSU0_LOCK_GAIN_E                       |
 | | bit  0  R/W  CSU0_VCOC_DONE_E                       |
 | +-----------------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_E_set                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_E_get                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_E_set                  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_E_get                  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_E_set                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_E_get                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_VC_LPF_OUT1_E_set                          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_VC_LPF_OUT1_E_get                          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_VC_LPF_OUT0_E_set                          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_VC_LPF_OUT0_E_get                          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_E_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_E_set              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_E_get              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_E_get         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_E_set                   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_E_get                   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_E_set                     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_E_get                     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_E_set                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_E_get                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_E_set               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_E_get               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_LOCK_LOSS_E_set                       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_LOCK_LOSS_E_get                       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_LOCK_GAIN_E_set                       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_LOCK_GAIN_E_get                       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_CSU0_VCOC_DONE_E_set                       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_CSU0_VCOC_DONE_E_get                       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000574 INT_REG                         |
 | +-----------------------------------------------------+
 | | bit  14 R/W  UPDATE_RATE_OVERFLOW_I                 |
 | | bit  13 R/W  ACB_DESKEW_OVERFLOW_I                  |
 | | bit  12 R/W  ACB_DESKEW_UNDERFLOW_I                 |
 | | bit  11 R/W  VC_LPF_OUT1_I                          |
 | | bit  10 R/W  VC_LPF_OUT0_I                          |
 | | bit  9  R/W  CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I |
 | | bit  8  R/W  CSU0_EADSM_LOCKDET_LOCK_I              |
 | | bit  7  R/W  CSU0_PLL_LOCKDET_PPM_OFF_UPD_I         |
 | | bit  6  R/W  CSU0_VCOC_BIST_ERR_I                   |
 | | bit  5  R/W  CSU0_DIVCLK_DEAD_I                     |
 | | bit  4  R/W  CSU0_IN_RANGE_N_FAILS_I                |
 | | bit  3  R/W  CSU0_IN_RANGE_N_PASSES_I               |
 | | bit  2  R/W  CSU0_LOCK_LOSS_I                       |
 | | bit  1  R/W  CSU0_LOCK_GAIN_I                       |
 | | bit  0  R/W  CSU0_VCOC_DONE_I                       |
 | +-----------------------------------------------------+
 | 
 | void                 s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_set_to_clear                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_get                          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_I_poll                         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_set_to_clear                  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_get                           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_I_poll                          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_set_to_clear                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_get                          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_I_poll                         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_csu_pcbi_field_VC_LPF_OUT1_I_set_to_clear                          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_VC_LPF_OUT1_I_get                                   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_VC_LPF_OUT1_I_poll                                  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_csu_pcbi_field_VC_LPF_OUT0_I_set_to_clear                          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_VC_LPF_OUT0_I_get                                   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_VC_LPF_OUT0_I_poll                                  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_set_to_clear ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_I_poll         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_set_to_clear              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_get                       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_I_poll                      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_set_to_clear         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_get                  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_I_poll                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_set_to_clear                   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_get                            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_I_poll                           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_set_to_clear                     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_get                              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_I_poll                             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_set_to_clear                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_get                         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_I_poll                        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_set_to_clear               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_get                        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_I_poll                       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_set_to_clear                       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_get                                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_LOCK_LOSS_I_poll                               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_set_to_clear                       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_get                                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_LOCK_GAIN_I_poll                               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 s16_csu_pcbi_field_CSU0_VCOC_DONE_I_set_to_clear                       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               s16_csu_pcbi_field_CSU0_VCOC_DONE_I_get                                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_VCOC_DONE_I_poll                               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------------+
 | | Register 0x00000578 INT_VAL_REG                   |
 | +---------------------------------------------------+
 | | bit  14 R  UPDATE_RATE_OVERFLOW_V                 |
 | | bit  13 R  ACB_DESKEW_OVERFLOW_V                  |
 | | bit  12 R  ACB_DESKEW_UNDERFLOW_V                 |
 | | bit  11 R  VC_LPF_OUT1_V                          |
 | | bit  10 R  VC_LPF_OUT0_V                          |
 | | bit  9  R  CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V |
 | | bit  8  R  CSU0_EADSM_LOCKDET_LOCK_V              |
 | | bit  7  R  CSU0_PLL_LOCKDET_PPM_OFF_UPD_V         |
 | | bit  6  R  CSU0_VCOC_BIST_ERR_V                   |
 | | bit  5  R  CSU0_DIVCLK_DEAD_V                     |
 | | bit  4  R  CSU0_IN_RANGE_N_FAILS_V                |
 | | bit  3  R  CSU0_IN_RANGE_N_PASSES_V               |
 | | bit  2  R  CSU0_LOCK_LOSS_V                       |
 | | bit  1  R  CSU0_LOCK_GAIN_V                       |
 | | bit  0  R  CSU0_VCOC_DONE_V                       |
 | +---------------------------------------------------+
 | 
 | UINT32               s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_V_get                  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_UPDATE_RATE_OVERFLOW_V_poll                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_V_get                   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_ACB_DESKEW_OVERFLOW_V_poll                  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_V_get                  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_ACB_DESKEW_UNDERFLOW_V_poll                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_csu_pcbi_field_VC_LPF_OUT1_V_get                           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_VC_LPF_OUT1_V_poll                          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_csu_pcbi_field_VC_LPF_OUT0_V_get                           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_VC_LPF_OUT0_V_poll                          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_get  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_IN_RANGE_N_PASSES_V_poll ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_V_get               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_EADSM_LOCKDET_LOCK_V_poll              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_PLL_LOCKDET_PPM_OFF_UPD_V_poll         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_V_get                    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_VCOC_BIST_ERR_V_poll                   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_V_get                      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_DIVCLK_DEAD_V_poll                     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_V_get                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_IN_RANGE_N_FAILS_V_poll                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_V_get                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_IN_RANGE_N_PASSES_V_poll               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_csu_pcbi_field_CSU0_LOCK_LOSS_V_get                        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_LOCK_LOSS_V_poll                       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_csu_pcbi_field_CSU0_LOCK_GAIN_V_get                        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_LOCK_GAIN_V_poll                       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               s16_csu_pcbi_field_CSU0_VCOC_DONE_V_get                        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_field_CSU0_VCOC_DONE_V_poll                       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x0000057c CSU0_GLOBAL_CFG_REG1 |
 | +------------------------------------------+
 | | bit  23:22 R/W  TRCS_MODE_23_22          |
 | | bit  14    R/W  TRCS_MODE_14             |
 | | bit  10    R/W  TRCS_MODE_10             |
 | +------------------------------------------+
 | 
 | void   s16_csu_pcbi_field_TRCS_MODE_23_22_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_TRCS_MODE_23_22_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_TRCS_MODE_23_22_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_TRCS_MODE_23_22_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_TRCS_MODE_14_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_TRCS_MODE_14_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_TRCS_MODE_10_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_TRCS_MODE_10_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------+
 | | Register 0x00000580 ACB_FT_CONFIG_REG1 |
 | +----------------------------------------+
 | | bit  31    R/W  ACB_RSTB               |
 | | bit  27    R/W  PDM_PHS_DET_DEL_SEL    |
 | | bit  26    R/W  PDM_ACCUM_BYPASS       |
 | | bit  25:21 R/W  PDM_TCLK2_DLY_SEL      |
 | | bit  20:16 R/W  PDM_TCLK1_DLY_SEL      |
 | | bit  15    R/W  PDM_EN                 |
 | | bit  13:10 R/W  PDM_TCLK_SEL           |
 | | bit  9:6   R/W  FRAC_DIV_MOD_EXT_SEL   |
 | | bit  1     R/W  ACB_HOLDOVER           |
 | | bit  0     R/W  ACB_FORCE_NOMINAL      |
 | +----------------------------------------+
 | 
 | void   s16_csu_pcbi_field_ACB_RSTB_set                   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_ACB_RSTB_get                   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_PDM_PHS_DET_DEL_SEL_set        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PDM_PHS_DET_DEL_SEL_get        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_PDM_ACCUM_BYPASS_set           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PDM_ACCUM_BYPASS_get           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_PDM_TCLK2_DLY_SEL_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PDM_TCLK2_DLY_SEL_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_PDM_TCLK2_DLY_SEL_set    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_PDM_TCLK2_DLY_SEL_get    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_PDM_TCLK1_DLY_SEL_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PDM_TCLK1_DLY_SEL_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_PDM_TCLK1_DLY_SEL_set    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_PDM_TCLK1_DLY_SEL_get    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_PDM_EN_set                     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PDM_EN_get                     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_PDM_TCLK_SEL_set               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PDM_TCLK_SEL_get               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_PDM_TCLK_SEL_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_PDM_TCLK_SEL_get         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_FRAC_DIV_MOD_EXT_SEL_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_FRAC_DIV_MOD_EXT_SEL_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_FRAC_DIV_MOD_EXT_SEL_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_FRAC_DIV_MOD_EXT_SEL_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_ACB_HOLDOVER_set               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_ACB_HOLDOVER_get               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_ACB_FORCE_NOMINAL_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_ACB_FORCE_NOMINAL_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------------+
 | | Register 0x00000584 PMM_CONFIG_REG1 |
 | +-------------------------------------+
 | | bit  29:16 R/W  PMM_DSM_DENOM       |
 | | bit  15:0  R/W  PMM_PKT_PERIOD      |
 | +-------------------------------------+
 | 
 | void   s16_csu_pcbi_field_PMM_DSM_DENOM_set        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PMM_DSM_DENOM_get        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_PMM_DSM_DENOM_set  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_PMM_DSM_DENOM_get  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_PMM_PKT_PERIOD_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PMM_PKT_PERIOD_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_PMM_PKT_PERIOD_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_PMM_PKT_PERIOD_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000588 PMM_CONFIG_REG2 |
 | +-------------------------------------+
 | | bit  19    R/W  ACB_DESKEW_CLRB     |
 | | bit  18:12 R/W  PDM_UPDATE_RATE     |
 | | bit  11    R/W  PMM_ENABLE          |
 | | bit  10:2  R/W  PMM_PKT_SIZE        |
 | | bit  1:0   R/W  PMM_DSM_TYPE        |
 | +-------------------------------------+
 | 
 | void   s16_csu_pcbi_field_ACB_DESKEW_CLRB_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_ACB_DESKEW_CLRB_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_PDM_UPDATE_RATE_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PDM_UPDATE_RATE_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_PDM_UPDATE_RATE_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_PDM_UPDATE_RATE_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_PMM_ENABLE_set            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PMM_ENABLE_get            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_PMM_PKT_SIZE_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PMM_PKT_SIZE_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_PMM_PKT_SIZE_set    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_PMM_PKT_SIZE_get    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_PMM_DSM_TYPE_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PMM_DSM_TYPE_get          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_PMM_DSM_TYPE_set    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_PMM_DSM_TYPE_get    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x0000058c PMM_CONFIG_REG3 |
 | +-------------------------------------+
 | | bit  31    R/W  PDM_OVERRIDE_EN     |
 | | bit  28:17 R/W  PDM_OVERRIDE_VAL    |
 | | bit  16:6  R/W  PDM_ACCUM_CYCLES    |
 | | bit  5:3   R/W  PDM_GAIN            |
 | | bit  2:0   R/W  PDM_ATTEN           |
 | +-------------------------------------+
 | 
 | void   s16_csu_pcbi_field_PDM_OVERRIDE_EN_set        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PDM_OVERRIDE_EN_get        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_PDM_OVERRIDE_VAL_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PDM_OVERRIDE_VAL_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_PDM_OVERRIDE_VAL_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_PDM_OVERRIDE_VAL_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_PDM_ACCUM_CYCLES_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PDM_ACCUM_CYCLES_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_PDM_ACCUM_CYCLES_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_PDM_ACCUM_CYCLES_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_PDM_GAIN_set               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PDM_GAIN_get               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_PDM_GAIN_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_PDM_GAIN_get         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_csu_pcbi_field_PDM_ATTEN_set              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_csu_pcbi_field_PDM_ATTEN_get              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_csu_pcbi_field_range_PDM_ATTEN_set        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_csu_pcbi_field_range_PDM_ATTEN_get        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000590 PMM_STATUS_REG1 |
 | +-------------------------------------+
 | | bit  28:17 R  PDM_ACCUM_ATTEN       |
 | | bit  11:0  R  PDM_ACCUM             |
 | +-------------------------------------+
 | 
 | UINT32 s16_csu_pcbi_field_PDM_ACCUM_ATTEN_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_csu_pcbi_field_range_PDM_ACCUM_ATTEN_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 s16_csu_pcbi_field_PDM_ACCUM_get             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_csu_pcbi_field_range_PDM_ACCUM_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000594 PMM_STATUS_REG2 |
 | +-------------------------------------+
 | | bit  28:17 R  PDM_ALIGN_OUT         |
 | | bit  11:0  R  PDM_ACCUM_GAIN        |
 | +-------------------------------------+
 | 
 | UINT32 s16_csu_pcbi_field_PDM_ALIGN_OUT_get        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_csu_pcbi_field_range_PDM_ALIGN_OUT_get  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 s16_csu_pcbi_field_PDM_ACCUM_GAIN_get       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_csu_pcbi_field_range_PDM_ACCUM_GAIN_get ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _S16_CSU_PCBI_IO_H
#define _S16_CSU_PCBI_IO_H

#include "s16_loc.h"
#include "s16_csu_pcbi_regs.h"
#include "s16_csu_pcbi_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 s16_csu_pcbi_buffer_init               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void                 s16_csu_pcbi_buffer_flush              ( s16_csu_pcbi_buffer_t *b_ptr )
 | UINT32               s16_csu_pcbi_reg_read                  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 s16_csu_pcbi_reg_write                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 s16_csu_pcbi_field_set                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 s16_csu_pcbi_action_on_write_field_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 s16_csu_pcbi_burst_read                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 s16_csu_pcbi_burst_write               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_poll                      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_write           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_field_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG1_read            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_write           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_field_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG2_read            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_write           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_field_set       ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_PLL_OOL_CONFIG_REG3_read            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_write          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_field_set      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG_read           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_CONFIG_REG2_write                   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_CONFIG_REG2_field_set               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_CONFIG_REG2_read                    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_CONFIG_REG3_write                   ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_CONFIG_REG3_field_set               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_CONFIG_REG3_read                    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_write         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_field_set     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_MABC_TEST_CONFIG_REG3_read          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_write              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_field_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG1_read               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_write              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_field_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_VCOC_CONFIG_REG2_read               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_write              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_field_set          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_RESET_CONFIG_REG_read               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU_TEST_CONFIG_REG_write                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU_TEST_CONFIG_REG_field_set            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU_TEST_CONFIG_REG_read                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_write          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_field_set      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CLK_DEGLITCHER_CONFIG_REG_read           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_GLOBAL_RESET_CONFIG_REG_write            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_GLOBAL_RESET_CONFIG_REG_field_set        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_GLOBAL_RESET_CONFIG_REG_read             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_write          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_field_set      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG1_read           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_write          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_field_set      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG2_read           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_write          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_field_set      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_FRAC_DIV_CONFIG_REG3_read           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_write     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_field_set ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_CONFIG_REG_read      ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_write             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_field_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG1_read              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_write             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_field_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG2_read              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_write             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_field_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG3_read              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_write             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_field_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG4_read              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_write             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_field_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG5_read              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_write             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_field_set         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_EADSM_CONFIG_REG6_read              ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_write               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_field_set           ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_CSU0_GLOBAL_CFG_REG1_read                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_write                 ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_field_set             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_ACB_FT_CONFIG_REG1_read                  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_PMM_CONFIG_REG1_write                    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_PMM_CONFIG_REG1_field_set                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_PMM_CONFIG_REG1_read                     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_PMM_CONFIG_REG2_write                    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_PMM_CONFIG_REG2_field_set                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_PMM_CONFIG_REG2_read                     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_PMM_CONFIG_REG3_write                    ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_PMM_CONFIG_REG3_field_set                ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_PMM_CONFIG_REG3_read                     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void                 s16_csu_pcbi_reg_INT_REG_write                            ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_INT_REG_action_on_write_field_set        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_INT_REG_read                             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_reg_INT_REG_poll                             ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 s16_csu_pcbi_reg_INT_EN_REG_write                         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 s16_csu_pcbi_reg_INT_EN_REG_field_set                     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               s16_csu_pcbi_reg_INT_EN_REG_read                          ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_csu_pcbi_reg_INT_VAL_REG_read                         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE s16_csu_pcbi_reg_INT_VAL_REG_poll                         ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               s16_csu_pcbi_reg_CSU0_PLL_OOL_STAT_REG_read               ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_csu_pcbi_reg_CSU0_VCOC_STAT_REG_read                  ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_csu_pcbi_reg_CSU0_EADSM_LOCK_DET_STAT_REG_read        ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_csu_pcbi_reg_PMM_STATUS_REG1_read                     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               s16_csu_pcbi_reg_PMM_STATUS_REG2_read                     ( s16_csu_pcbi_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _S16_CSU_PCBI_IO_H */
