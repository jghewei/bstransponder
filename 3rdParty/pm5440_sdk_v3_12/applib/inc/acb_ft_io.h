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
 *     and register accessor functions for the acb_ft block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing acb_ft io functions is:
 | {
 |     acb_ft_buffer_t b_ptr[1];
 |     acb_ft_buffer_init( b_ptr, h_ptr );
 |     value = acb_ft_field_<FIELD>_get( b_ptr, h_ptr );
 |     acb_ft_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     acb_ft_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = acb_ft_field_<FIELD>_get( NULL, h_ptr );
 | acb_ft_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------+
 | | Register 0x00000000 SW_RESET_REG |
 | +----------------------------------+
 | | bit  0 R/W  SOFTWARE_RESET       |
 | +----------------------------------+
 | 
 | void   acb_ft_field_SOFTWARE_RESET_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_SOFTWARE_RESET_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x00000004 ACB_FT_EN_REG |
 | +-----------------------------------+
 | | bit  7 R/W  FIFO_CENT_ACCEL       |
 | | bit  6 R/W  B_CKCTL_MODE          |
 | | bit  5 R/W  C_CKCTL_MODE          |
 | | bit  4 R/W  FIFO_CENT_TRIGGER     |
 | | bit  3 R/W  FIFO_CENT_ENABLE      |
 | | bit  2 R/W  B_INTERFACE_ENABLE    |
 | | bit  1 R/W  FIFO_ADAPT_ENABLE     |
 | | bit  0 R/W  PMM_ADAPT_ENABLE      |
 | +-----------------------------------+
 | 
 | void   acb_ft_field_FIFO_CENT_ACCEL_set    ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_FIFO_CENT_ACCEL_get    ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_B_CKCTL_MODE_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_B_CKCTL_MODE_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_C_CKCTL_MODE_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_C_CKCTL_MODE_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_FIFO_CENT_TRIGGER_set  ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_FIFO_CENT_TRIGGER_get  ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_FIFO_CENT_ENABLE_set   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_FIFO_CENT_ENABLE_get   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_B_INTERFACE_ENABLE_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_B_INTERFACE_ENABLE_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_FIFO_ADAPT_ENABLE_set  ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_FIFO_ADAPT_ENABLE_get  ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_PMM_ADAPT_ENABLE_set   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_PMM_ADAPT_ENABLE_get   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x00000008 ACB_FT_HOLDOVER_REG |
 | +-----------------------------------------+
 | | bit  1 R/W  HOLDOVER                    |
 | | bit  0 R/W  FORCE_NOMINAL               |
 | +-----------------------------------------+
 | 
 | void   acb_ft_field_HOLDOVER_set      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_HOLDOVER_get      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_FORCE_NOMINAL_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_FORCE_NOMINAL_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x0000000c ACB_FT_GAIN_REG_1 |
 | +---------------------------------------+
 | | bit  31:28 R/W  PLL_S_MIN             |
 | | bit  27:24 R/W  DWELL_MAX             |
 | | bit  23:20 R/W  FREQ_GAIN             |
 | | bit  19:18 R/W  DITHER_SEL            |
 | | bit  16    R/W  SEED                  |
 | | bit  11:8  R/W  FINAL_SHIFT           |
 | | bit  7:4   R/W  INT_GAIN              |
 | | bit  3:0   R/W  FILT_GAIN             |
 | +---------------------------------------+
 | 
 | void   acb_ft_field_PLL_S_MIN_set         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_PLL_S_MIN_get         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_PLL_S_MIN_set   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_PLL_S_MIN_get   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   acb_ft_field_DWELL_MAX_set         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_DWELL_MAX_get         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_DWELL_MAX_set   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_DWELL_MAX_get   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   acb_ft_field_FREQ_GAIN_set         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_FREQ_GAIN_get         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_FREQ_GAIN_set   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_FREQ_GAIN_get   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   acb_ft_field_DITHER_SEL_set        ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_DITHER_SEL_get        ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_DITHER_SEL_set  ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_DITHER_SEL_get  ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   acb_ft_field_SEED_set              ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_SEED_get              ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_FINAL_SHIFT_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_FINAL_SHIFT_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_FINAL_SHIFT_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_FINAL_SHIFT_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   acb_ft_field_INT_GAIN_set          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_INT_GAIN_get          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_INT_GAIN_set    ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_INT_GAIN_get    ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   acb_ft_field_FILT_GAIN_set         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_FILT_GAIN_get         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_FILT_GAIN_set   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_FILT_GAIN_get   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000010 ACB_FT_GAIN_REG_2 |
 | +---------------------------------------+
 | | bit  17   R/W  OUTPUT_DSM_ENABLE      |
 | | bit  16   R/W  PHASE_FILT_ENABLE      |
 | | bit  15:8 R/W  OUTPUT_UPDATE_RATE     |
 | | bit  4:0  R/W  PLL_S_MAX              |
 | +---------------------------------------+
 | 
 | void   acb_ft_field_OUTPUT_DSM_ENABLE_set        ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_OUTPUT_DSM_ENABLE_get        ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_PHASE_FILT_ENABLE_set        ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_PHASE_FILT_ENABLE_get        ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_OUTPUT_UPDATE_RATE_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_OUTPUT_UPDATE_RATE_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_OUTPUT_UPDATE_RATE_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_OUTPUT_UPDATE_RATE_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   acb_ft_field_PLL_S_MAX_set                ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_PLL_S_MAX_get                ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_PLL_S_MAX_set          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_PLL_S_MAX_get          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x00000014 PACKET_PERIOD_INTEGER_CFG_REG |
 | +---------------------------------------------------+
 | | bit  15:0 R/W  PKT_PERIOD_INT                     |
 | +---------------------------------------------------+
 | 
 | void   acb_ft_field_PKT_PERIOD_INT_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_PKT_PERIOD_INT_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_PKT_PERIOD_INT_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_PKT_PERIOD_INT_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x00000018 PACKET_PERIOD_NUMERATOR_MSB_REG |
 | +-----------------------------------------------------+
 | | bit  8:0 R/W  PKT_PERIOD_N_MSB                      |
 | +-----------------------------------------------------+
 | 
 | void   acb_ft_field_PKT_PERIOD_N_MSB_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_PKT_PERIOD_N_MSB_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_PKT_PERIOD_N_MSB_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_PKT_PERIOD_N_MSB_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register 0x0000001c PACKET_PERIOD_NUMERATOR_LSB_REG |
 | +-----------------------------------------------------+
 | | bit  31:0 R/W  PKT_PERIOD_N_LSB                     |
 | +-----------------------------------------------------+
 | 
 | void   acb_ft_field_PKT_PERIOD_N_LSB_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_PKT_PERIOD_N_LSB_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_PKT_PERIOD_N_LSB_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_PKT_PERIOD_N_LSB_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register 0x00000020 PACKET_PERIOD_DENOMINATOR_MSB_REG |
 | +-------------------------------------------------------+
 | | bit  7:0 R/W  PKT_PERIOD_D_MSB                        |
 | +-------------------------------------------------------+
 | 
 | void   acb_ft_field_PKT_PERIOD_D_MSB_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_PKT_PERIOD_D_MSB_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_PKT_PERIOD_D_MSB_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_PKT_PERIOD_D_MSB_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register 0x00000024 PACKET_PERIOD_DENOMINATOR_LSB_REG |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  PKT_PERIOD_D_LSB                       |
 | +-------------------------------------------------------+
 | 
 | void   acb_ft_field_PKT_PERIOD_D_LSB_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_PKT_PERIOD_D_LSB_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_PKT_PERIOD_D_LSB_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_PKT_PERIOD_D_LSB_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000028 ACB_FT_PACKET_SIZE_CFG_REG |
 | +------------------------------------------------+
 | | bit  29:24 R/W  FB_WORDSIZE                    |
 | | bit  16    R/W  PKT_SIZE_VAR                   |
 | | bit  8:0   R/W  PKT_SIZE                       |
 | +------------------------------------------------+
 | 
 | void   acb_ft_field_FB_WORDSIZE_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_FB_WORDSIZE_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_FB_WORDSIZE_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_FB_WORDSIZE_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   acb_ft_field_PKT_SIZE_VAR_set      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_PKT_SIZE_VAR_get      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_PKT_SIZE_set          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_PKT_SIZE_get          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_PKT_SIZE_set    ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_PKT_SIZE_get    ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x0000002c ACB_FT_THERMOMETER_CFG_REG_1 |
 | +--------------------------------------------------+
 | | bit  3:0 R/W  THERMOMETER_INT                    |
 | +--------------------------------------------------+
 | 
 | void   acb_ft_field_THERMOMETER_INT_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_THERMOMETER_INT_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_THERMOMETER_INT_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_THERMOMETER_INT_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000030 ACB_FT_THERMOMETER_CFG_REG_2 |
 | +--------------------------------------------------+
 | | bit  31:0 R/W  THERMOMETER_N                     |
 | +--------------------------------------------------+
 | 
 | void   acb_ft_field_THERMOMETER_N_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_THERMOMETER_N_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_THERMOMETER_N_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_THERMOMETER_N_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000034 ACB_FT_THERMOMETER_CFG_REG_3 |
 | +--------------------------------------------------+
 | | bit  31:0 R/W  THERMOMETER_D                     |
 | +--------------------------------------------------+
 | 
 | void   acb_ft_field_THERMOMETER_D_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_THERMOMETER_D_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_THERMOMETER_D_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_THERMOMETER_D_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x00000038 ACB_FT_FEEDBACK_CNT_CFG_REG_1 |
 | +---------------------------------------------------+
 | | bit  31:0 R/W  CNT_N                              |
 | +---------------------------------------------------+
 | 
 | void   acb_ft_field_CNT_N_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_CNT_N_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_CNT_N_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_CNT_N_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x0000003c ACB_FT_FEEDBACK_CNT_CFG_REG_2 |
 | +---------------------------------------------------+
 | | bit  31:0 R/W  CNT_D                              |
 | +---------------------------------------------------+
 | 
 | void   acb_ft_field_CNT_D_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_CNT_D_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_CNT_D_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_CNT_D_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00000044 ACB_FT_OUTPUT_MON_CFG_REG |
 | +-----------------------------------------------+
 | | bit  3 R/W  OVERRIDE_FILT                     |
 | | bit  2 R/W  UPDATE_FILT                       |
 | | bit  1 R/W  UPDATE_INT                        |
 | | bit  0 R/W  RESET_MINMAX                      |
 | +-----------------------------------------------+
 | 
 | void   acb_ft_field_OVERRIDE_FILT_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_OVERRIDE_FILT_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_UPDATE_FILT_set   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_UPDATE_FILT_get   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_UPDATE_INT_set    ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_UPDATE_INT_get    ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_RESET_MINMAX_set  ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_RESET_MINMAX_get  ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000048 ACB_FT_OUTPUT_MON_STAT_REG_1 |
 | +--------------------------------------------------+
 | | bit  24:0 R  PLL_S_OUT_MIN                       |
 | +--------------------------------------------------+
 | 
 | UINT32 acb_ft_field_PLL_S_OUT_MIN_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | UINT32 acb_ft_field_range_PLL_S_OUT_MIN_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x0000004c ACB_FT_OUTPUT_MON_STAT_REG_2 |
 | +--------------------------------------------------+
 | | bit  24:0 R  PLL_S_OUT_MAX                       |
 | +--------------------------------------------------+
 | 
 | UINT32 acb_ft_field_PLL_S_OUT_MAX_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | UINT32 acb_ft_field_range_PLL_S_OUT_MAX_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000050 ACB_FT_OUTPUT_MON_STAT_REG_3 |
 | +--------------------------------------------------+
 | | bit  24:0 R  INTEGRATOR_VALUE                    |
 | +--------------------------------------------------+
 | 
 | UINT32 acb_ft_field_INTEGRATOR_VALUE_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | UINT32 acb_ft_field_range_INTEGRATOR_VALUE_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000054 ACB_FT_OUTPUT_MON_STAT_REG_4 |
 | +--------------------------------------------------+
 | | bit  24:0 R  FILTER_INT_VALUE                    |
 | +--------------------------------------------------+
 | 
 | UINT32 acb_ft_field_FILTER_INT_VALUE_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | UINT32 acb_ft_field_range_FILTER_INT_VALUE_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000058 ACB_FT_OUTPUT_MON_STAT_REG_5 |
 | +--------------------------------------------------+
 | | bit  15:0 R  FILTER_FRAC_VALUE                   |
 | +--------------------------------------------------+
 | 
 | UINT32 acb_ft_field_FILTER_FRAC_VALUE_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | UINT32 acb_ft_field_range_FILTER_FRAC_VALUE_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x0000005c ACB_FT_OUTPUT_MON_STAT_REG_6 |
 | +--------------------------------------------------+
 | | bit  24:0 R/W  FILTER_INT_OVERRIDE               |
 | +--------------------------------------------------+
 | 
 | void   acb_ft_field_FILTER_INT_OVERRIDE_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_FILTER_INT_OVERRIDE_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_FILTER_INT_OVERRIDE_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_FILTER_INT_OVERRIDE_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000060 ACB_FT_OUTPUT_MON_STAT_REG_7 |
 | +--------------------------------------------------+
 | | bit  15:0 R/W  FILTER_FRAC_OVERRIDE              |
 | +--------------------------------------------------+
 | 
 | void   acb_ft_field_FILTER_FRAC_OVERRIDE_set       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_FILTER_FRAC_OVERRIDE_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void   acb_ft_field_range_FILTER_FRAC_OVERRIDE_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 acb_ft_field_range_FILTER_FRAC_OVERRIDE_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000064 ACB_FT_OUTPUT_MON_STAT_REG_8 |
 | +--------------------------------------------------+
 | | bit  31:0 R  B_CNT                               |
 | +--------------------------------------------------+
 | 
 | UINT32 acb_ft_field_B_CNT_get       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | UINT32 acb_ft_field_range_B_CNT_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00000068 ACB_FT_INT_REG |
 | +------------------------------------+
 | | bit  0 R/W  ACB_FAIL_I             |
 | +------------------------------------+
 | 
 | void                 acb_ft_field_ACB_FAIL_I_set_to_clear ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32               acb_ft_field_ACB_FAIL_I_get          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE acb_ft_field_ACB_FAIL_I_poll         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x0000006c ACB_FT_INT_EN_REG |
 | +---------------------------------------+
 | | bit  0 R/W  ACB_FAIL_E                |
 | +---------------------------------------+
 | 
 | void   acb_ft_field_ACB_FAIL_E_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | UINT32 acb_ft_field_ACB_FAIL_E_get ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x00000070 ACB_FT_INT_STAT_REG |
 | +-----------------------------------------+
 | | bit  0 R  ACB_FAIL_V                    |
 | +-----------------------------------------+
 | 
 | UINT32               acb_ft_field_ACB_FAIL_V_get  ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE acb_ft_field_ACB_FAIL_V_poll ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ACB_FT_IO_H
#define _ACB_FT_IO_H

#include "acb_ft.h"
#include "acb_ft_regs.h"
#include "acb_ft_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 acb_ft_buffer_init               ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | void                 acb_ft_buffer_flush              ( acb_ft_buffer_t *b_ptr )
 | UINT32               acb_ft_reg_read                  ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 acb_ft_reg_write                 ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 acb_ft_field_set                 ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 acb_ft_action_on_write_field_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 acb_ft_burst_read                ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 acb_ft_burst_write               ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE acb_ft_poll                      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 acb_ft_reg_ACB_FT_EN_REG_write                         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_EN_REG_field_set                     ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_EN_REG_read                          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_ACB_FT_HOLDOVER_REG_write                   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_HOLDOVER_REG_field_set               ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_HOLDOVER_REG_read                    ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_ACB_FT_GAIN_REG_1_write                     ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_GAIN_REG_1_field_set                 ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_GAIN_REG_1_read                      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_ACB_FT_GAIN_REG_2_write                     ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_GAIN_REG_2_field_set                 ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_GAIN_REG_2_read                      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_write         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_field_set     ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_PACKET_PERIOD_INTEGER_CFG_REG_read          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_write       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_field_set   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_PACKET_PERIOD_NUMERATOR_MSB_REG_read        ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_write       ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_field_set   ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_PACKET_PERIOD_NUMERATOR_LSB_REG_read        ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_write     ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_field_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_PACKET_PERIOD_DENOMINATOR_MSB_REG_read      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_write     ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_field_set ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_PACKET_PERIOD_DENOMINATOR_LSB_REG_read      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_write            ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_field_set        ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_PACKET_SIZE_CFG_REG_read             ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_write          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_field_set      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_1_read           ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_write          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_field_set      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_2_read           ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_write          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_field_set      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_THERMOMETER_CFG_REG_3_read           ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_write         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_field_set     ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_1_read          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_write         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_field_set     ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_FEEDBACK_CNT_CFG_REG_2_read          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_write             ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_field_set         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_OUTPUT_MON_CFG_REG_read              ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_write          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_field_set      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_6_read           ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_write          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_field_set      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_7_read           ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_ACB_FT_INT_REG_write                        ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_INT_REG_action_on_write_field_set    ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_INT_REG_read                         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE acb_ft_reg_ACB_FT_INT_REG_poll                         ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 acb_ft_reg_ACB_FT_INT_EN_REG_write                     ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_ACB_FT_INT_EN_REG_field_set                 ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_ACB_FT_INT_EN_REG_read                      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | UINT32               acb_ft_reg_ACB_FT_INT_STAT_REG_read                    ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE acb_ft_reg_ACB_FT_INT_STAT_REG_poll                    ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_1_read           ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | UINT32               acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_2_read           ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | UINT32               acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_3_read           ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | UINT32               acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_4_read           ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | UINT32               acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_5_read           ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | UINT32               acb_ft_reg_ACB_FT_OUTPUT_MON_STAT_REG_8_read           ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 | void                 acb_ft_reg_SW_RESET_REG_write                          ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 value )
 | void                 acb_ft_reg_SW_RESET_REG_field_set                      ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               acb_ft_reg_SW_RESET_REG_read                           ( acb_ft_buffer_t *b_ptr, acb_ft_handle_t *h_ptr )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ACB_FT_IO_H */
