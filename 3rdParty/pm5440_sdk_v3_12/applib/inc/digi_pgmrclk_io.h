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
 *     and register accessor functions for the digi_pgmrclk block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing digi_pgmrclk io functions is:
 | {
 |     digi_pgmrclk_buffer_t b_ptr[1];
 |     digi_pgmrclk_buffer_init( b_ptr, h_ptr );
 |     value = digi_pgmrclk_field_<FIELD>_get( b_ptr, h_ptr );
 |     digi_pgmrclk_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     digi_pgmrclk_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = digi_pgmrclk_field_<FIELD>_get( NULL, h_ptr );
 | digi_pgmrclk_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +------------------------------------------+
 | | Register 0x00015000 LCLK_GENERATOR_0_CFG |
 | +------------------------------------------+
 | | bit  26:24 R/W  MISC_LCLK_0              |
 | | bit  23    R/W  AUTO_LCLKOUT_EN_0        |
 | | bit  22    R/W  LCLK_PULS_GEN_0          |
 | | bit  21:8  R/W  SYNC_INTERVAL_0          |
 | | bit  7     R/W  SYNC_DISABLE_0           |
 | | bit  6:5   R/W  PMON_SYNC_I_SEL_0        |
 | | bit  4     R/W  CNTR_ENABLE_0            |
 | | bit  3:0   R/W  MASK_WINDOW_0            |
 | +------------------------------------------+
 | 
 | void   digi_pgmrclk_field_MISC_LCLK_0_set             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MISC_LCLK_0_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MISC_LCLK_0_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MISC_LCLK_0_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_AUTO_LCLKOUT_EN_0_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_AUTO_LCLKOUT_EN_0_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_PULS_GEN_0_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_PULS_GEN_0_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_SYNC_INTERVAL_0_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_INTERVAL_0_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_SYNC_INTERVAL_0_set   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_SYNC_INTERVAL_0_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_SYNC_DISABLE_0_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_DISABLE_0_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_PMON_SYNC_I_SEL_0_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_PMON_SYNC_I_SEL_0_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_PMON_SYNC_I_SEL_0_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_PMON_SYNC_I_SEL_0_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_CNTR_ENABLE_0_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_CNTR_ENABLE_0_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_MASK_WINDOW_0_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MASK_WINDOW_0_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MASK_WINDOW_0_set     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MASK_WINDOW_0_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00015004 LCLK_GENERATOR_1_CFG |
 | +------------------------------------------+
 | | bit  26:24 R/W  MISC_LCLK_1              |
 | | bit  23    R/W  AUTO_LCLKOUT_EN_1        |
 | | bit  22    R/W  LCLK_PULS_GEN_1          |
 | | bit  21:8  R/W  SYNC_INTERVAL_1          |
 | | bit  7     R/W  SYNC_DISABLE_1           |
 | | bit  6:5   R/W  PMON_SYNC_I_SEL_1        |
 | | bit  4     R/W  CNTR_ENABLE_1            |
 | | bit  3:0   R/W  MASK_WINDOW_1            |
 | +------------------------------------------+
 | 
 | void   digi_pgmrclk_field_MISC_LCLK_1_set             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MISC_LCLK_1_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MISC_LCLK_1_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MISC_LCLK_1_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_AUTO_LCLKOUT_EN_1_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_AUTO_LCLKOUT_EN_1_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_PULS_GEN_1_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_PULS_GEN_1_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_SYNC_INTERVAL_1_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_INTERVAL_1_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_SYNC_INTERVAL_1_set   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_SYNC_INTERVAL_1_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_SYNC_DISABLE_1_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_DISABLE_1_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_PMON_SYNC_I_SEL_1_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_PMON_SYNC_I_SEL_1_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_PMON_SYNC_I_SEL_1_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_PMON_SYNC_I_SEL_1_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_CNTR_ENABLE_1_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_CNTR_ENABLE_1_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_MASK_WINDOW_1_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MASK_WINDOW_1_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MASK_WINDOW_1_set     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MASK_WINDOW_1_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00015008 LCLK_GENERATOR_2_CFG |
 | +------------------------------------------+
 | | bit  26:24 R/W  MISC_LCLK_2              |
 | | bit  23    R/W  AUTO_LCLKOUT_EN_2        |
 | | bit  22    R/W  LCLK_PULS_GEN_2          |
 | | bit  21:8  R/W  SYNC_INTERVAL_2          |
 | | bit  7     R/W  SYNC_DISABLE_2           |
 | | bit  6:5   R/W  PMON_SYNC_I_SEL_2        |
 | | bit  4     R/W  CNTR_ENABLE_2            |
 | | bit  3:0   R/W  MASK_WINDOW_2            |
 | +------------------------------------------+
 | 
 | void   digi_pgmrclk_field_MISC_LCLK_2_set             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MISC_LCLK_2_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MISC_LCLK_2_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MISC_LCLK_2_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_AUTO_LCLKOUT_EN_2_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_AUTO_LCLKOUT_EN_2_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_PULS_GEN_2_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_PULS_GEN_2_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_SYNC_INTERVAL_2_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_INTERVAL_2_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_SYNC_INTERVAL_2_set   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_SYNC_INTERVAL_2_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_SYNC_DISABLE_2_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_DISABLE_2_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_PMON_SYNC_I_SEL_2_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_PMON_SYNC_I_SEL_2_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_PMON_SYNC_I_SEL_2_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_PMON_SYNC_I_SEL_2_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_CNTR_ENABLE_2_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_CNTR_ENABLE_2_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_MASK_WINDOW_2_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MASK_WINDOW_2_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MASK_WINDOW_2_set     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MASK_WINDOW_2_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0001500c LCLK_GENERATOR_3_CFG |
 | +------------------------------------------+
 | | bit  26:24 R/W  MISC_LCLK_3              |
 | | bit  23    R/W  AUTO_LCLKOUT_EN_3        |
 | | bit  22    R/W  LCLK_PULS_GEN_3          |
 | | bit  21:8  R/W  SYNC_INTERVAL_3          |
 | | bit  7     R/W  SYNC_DISABLE_3           |
 | | bit  6:5   R/W  PMON_SYNC_I_SEL_3        |
 | | bit  4     R/W  CNTR_ENABLE_3            |
 | | bit  3:0   R/W  MASK_WINDOW_3            |
 | +------------------------------------------+
 | 
 | void   digi_pgmrclk_field_MISC_LCLK_3_set             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MISC_LCLK_3_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MISC_LCLK_3_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MISC_LCLK_3_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_AUTO_LCLKOUT_EN_3_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_AUTO_LCLKOUT_EN_3_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_PULS_GEN_3_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_PULS_GEN_3_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_SYNC_INTERVAL_3_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_INTERVAL_3_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_SYNC_INTERVAL_3_set   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_SYNC_INTERVAL_3_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_SYNC_DISABLE_3_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_DISABLE_3_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_PMON_SYNC_I_SEL_3_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_PMON_SYNC_I_SEL_3_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_PMON_SYNC_I_SEL_3_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_PMON_SYNC_I_SEL_3_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_CNTR_ENABLE_3_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_CNTR_ENABLE_3_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_MASK_WINDOW_3_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MASK_WINDOW_3_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MASK_WINDOW_3_set     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MASK_WINDOW_3_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00015010 LCLK_GENERATOR_4_CFG |
 | +------------------------------------------+
 | | bit  26:24 R/W  MISC_LCLK_4              |
 | | bit  23    R/W  AUTO_LCLKOUT_EN_4        |
 | | bit  22    R/W  LCLK_PULS_GEN_4          |
 | | bit  21:8  R/W  SYNC_INTERVAL_4          |
 | | bit  7     R/W  SYNC_DISABLE_4           |
 | | bit  6:5   R/W  PMON_SYNC_I_SEL_4        |
 | | bit  4     R/W  CNTR_ENABLE_4            |
 | | bit  3:0   R/W  MASK_WINDOW_4            |
 | +------------------------------------------+
 | 
 | void   digi_pgmrclk_field_MISC_LCLK_4_set             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MISC_LCLK_4_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MISC_LCLK_4_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MISC_LCLK_4_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_AUTO_LCLKOUT_EN_4_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_AUTO_LCLKOUT_EN_4_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_PULS_GEN_4_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_PULS_GEN_4_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_SYNC_INTERVAL_4_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_INTERVAL_4_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_SYNC_INTERVAL_4_set   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_SYNC_INTERVAL_4_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_SYNC_DISABLE_4_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_DISABLE_4_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_PMON_SYNC_I_SEL_4_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_PMON_SYNC_I_SEL_4_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_PMON_SYNC_I_SEL_4_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_PMON_SYNC_I_SEL_4_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_CNTR_ENABLE_4_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_CNTR_ENABLE_4_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_MASK_WINDOW_4_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MASK_WINDOW_4_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MASK_WINDOW_4_set     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MASK_WINDOW_4_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00015014 LCLK_GENERATOR_5_CFG |
 | +------------------------------------------+
 | | bit  26:24 R/W  MISC_LCLK_5              |
 | | bit  23    R/W  AUTO_LCLKOUT_EN_5        |
 | | bit  22    R/W  LCLK_PULS_GEN_5          |
 | | bit  21:8  R/W  SYNC_INTERVAL_5          |
 | | bit  7     R/W  SYNC_DISABLE_5           |
 | | bit  6:5   R/W  PMON_SYNC_I_SEL_5        |
 | | bit  4     R/W  CNTR_ENABLE_5            |
 | | bit  3:0   R/W  MASK_WINDOW_5            |
 | +------------------------------------------+
 | 
 | void   digi_pgmrclk_field_MISC_LCLK_5_set             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MISC_LCLK_5_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MISC_LCLK_5_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MISC_LCLK_5_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_AUTO_LCLKOUT_EN_5_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_AUTO_LCLKOUT_EN_5_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_PULS_GEN_5_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_PULS_GEN_5_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_SYNC_INTERVAL_5_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_INTERVAL_5_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_SYNC_INTERVAL_5_set   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_SYNC_INTERVAL_5_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_SYNC_DISABLE_5_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_DISABLE_5_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_PMON_SYNC_I_SEL_5_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_PMON_SYNC_I_SEL_5_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_PMON_SYNC_I_SEL_5_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_PMON_SYNC_I_SEL_5_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_CNTR_ENABLE_5_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_CNTR_ENABLE_5_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_MASK_WINDOW_5_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MASK_WINDOW_5_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MASK_WINDOW_5_set     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MASK_WINDOW_5_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00015018 LCLK_GENERATOR_6_CFG |
 | +------------------------------------------+
 | | bit  26:24 R/W  MISC_LCLK_6              |
 | | bit  23    R/W  AUTO_LCLKOUT_EN_6        |
 | | bit  22    R/W  LCLK_PULS_GEN_6          |
 | | bit  21:8  R/W  SYNC_INTERVAL_6          |
 | | bit  7     R/W  SYNC_DISABLE_6           |
 | | bit  6:5   R/W  PMON_SYNC_I_SEL_6        |
 | | bit  4     R/W  CNTR_ENABLE_6            |
 | | bit  3:0   R/W  MASK_WINDOW_6            |
 | +------------------------------------------+
 | 
 | void   digi_pgmrclk_field_MISC_LCLK_6_set             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MISC_LCLK_6_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MISC_LCLK_6_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MISC_LCLK_6_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_AUTO_LCLKOUT_EN_6_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_AUTO_LCLKOUT_EN_6_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_PULS_GEN_6_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_PULS_GEN_6_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_SYNC_INTERVAL_6_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_INTERVAL_6_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_SYNC_INTERVAL_6_set   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_SYNC_INTERVAL_6_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_SYNC_DISABLE_6_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_DISABLE_6_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_PMON_SYNC_I_SEL_6_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_PMON_SYNC_I_SEL_6_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_PMON_SYNC_I_SEL_6_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_PMON_SYNC_I_SEL_6_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_CNTR_ENABLE_6_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_CNTR_ENABLE_6_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_MASK_WINDOW_6_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MASK_WINDOW_6_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MASK_WINDOW_6_set     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MASK_WINDOW_6_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0001501c LCLK_GENERATOR_7_CFG |
 | +------------------------------------------+
 | | bit  26:24 R/W  MISC_LCLK_7              |
 | | bit  23    R/W  AUTO_LCLKOUT_EN_7        |
 | | bit  22    R/W  LCLK_PULS_GEN_7          |
 | | bit  21:8  R/W  SYNC_INTERVAL_7          |
 | | bit  7     R/W  SYNC_DISABLE_7           |
 | | bit  6:5   R/W  PMON_SYNC_I_SEL_7        |
 | | bit  4     R/W  CNTR_ENABLE_7            |
 | | bit  3:0   R/W  MASK_WINDOW_7            |
 | +------------------------------------------+
 | 
 | void   digi_pgmrclk_field_MISC_LCLK_7_set             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MISC_LCLK_7_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MISC_LCLK_7_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MISC_LCLK_7_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_AUTO_LCLKOUT_EN_7_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_AUTO_LCLKOUT_EN_7_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_PULS_GEN_7_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_PULS_GEN_7_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_SYNC_INTERVAL_7_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_INTERVAL_7_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_SYNC_INTERVAL_7_set   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_SYNC_INTERVAL_7_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_SYNC_DISABLE_7_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_DISABLE_7_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_PMON_SYNC_I_SEL_7_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_PMON_SYNC_I_SEL_7_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_PMON_SYNC_I_SEL_7_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_PMON_SYNC_I_SEL_7_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_CNTR_ENABLE_7_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_CNTR_ENABLE_7_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_MASK_WINDOW_7_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MASK_WINDOW_7_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MASK_WINDOW_7_set     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MASK_WINDOW_7_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00015020 LCLK_GENERATOR_RESET |
 | +------------------------------------------+
 | | bit  7 R/W  INTERVAL_RST_0               |
 | | bit  6 R/W  INTERVAL_RST_1               |
 | | bit  5 R/W  INTERVAL_RST_2               |
 | | bit  4 R/W  INTERVAL_RST_3               |
 | | bit  3 R/W  INTERVAL_RST_4               |
 | | bit  2 R/W  INTERVAL_RST_5               |
 | | bit  1 R/W  INTERVAL_RST_6               |
 | | bit  0 R/W  INTERVAL_RST_7               |
 | +------------------------------------------+
 | 
 | void   digi_pgmrclk_field_INTERVAL_RST_0_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_INTERVAL_RST_0_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_INTERVAL_RST_1_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_INTERVAL_RST_1_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_INTERVAL_RST_2_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_INTERVAL_RST_2_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_INTERVAL_RST_3_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_INTERVAL_RST_3_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_INTERVAL_RST_4_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_INTERVAL_RST_4_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_INTERVAL_RST_5_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_INTERVAL_RST_5_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_INTERVAL_RST_6_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_INTERVAL_RST_6_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_INTERVAL_RST_7_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_INTERVAL_RST_7_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +------------------------------------------------+
 | | Register 0x00015024 LCLK_GENERATOR_SELECTION_0 |
 | +------------------------------------------------+
 | | bit  23:21 R/W  LCLK_OUT_SEL_17                |
 | | bit  20:18 R/W  LCLK_OUT_SEL_16                |
 | | bit  17:15 R/W  LCLK_OUT_SEL_15                |
 | | bit  14:12 R/W  LCLK_OUT_SEL_14                |
 | | bit  11:9  R/W  LCLK_OUT_SEL_13                |
 | | bit  8:6   R/W  LCLK_OUT_SEL_12                |
 | | bit  5:3   R/W  LCLK_OUT_SEL_11                |
 | | bit  2:0   R/W  LCLK_OUT_SEL_10                |
 | +------------------------------------------------+
 | 
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_17_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_17_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_17_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_17_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_16_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_16_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_16_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_16_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_15_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_15_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_15_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_15_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_14_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_14_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_14_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_14_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_13_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_13_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_13_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_13_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_12_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_12_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_12_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_12_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_11_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_11_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_11_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_11_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_10_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_10_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_10_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_10_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00015028 LCLK_GENERATOR_SELECTION_1 |
 | +------------------------------------------------+
 | | bit  29:27 R/W  LCLK_OUT_SEL_9                 |
 | | bit  26:24 R/W  LCLK_OUT_SEL_8                 |
 | | bit  23:21 R/W  LCLK_OUT_SEL_7                 |
 | | bit  20:18 R/W  LCLK_OUT_SEL_6                 |
 | | bit  17:15 R/W  LCLK_OUT_SEL_5                 |
 | | bit  14:12 R/W  LCLK_OUT_SEL_4                 |
 | | bit  11:9  R/W  LCLK_OUT_SEL_3                 |
 | | bit  8:6   R/W  LCLK_OUT_SEL_2                 |
 | | bit  5:3   R/W  LCLK_OUT_SEL_1                 |
 | | bit  2:0   R/W  LCLK_OUT_SEL_0                 |
 | +------------------------------------------------+
 | 
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_9_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_9_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_9_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_9_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_8_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_8_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_8_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_8_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_7_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_7_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_7_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_7_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_6_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_6_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_6_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_6_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_5_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_5_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_5_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_5_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_4_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_4_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_4_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_4_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_3_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_3_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_3_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_3_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_2_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_2_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_2_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_2_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_1_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_1_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_1_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_1_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_LCLK_OUT_SEL_0_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_SEL_0_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_SEL_0_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_SEL_0_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0001502c LCLK_MANUAL_PULSE_ENS |
 | +-------------------------------------------+
 | | bit  19:0 R/W  LCLK_OUT_MAN_PULS_EN       |
 | +-------------------------------------------+
 | 
 | void   digi_pgmrclk_field_LCLK_OUT_MAN_PULS_EN_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_MAN_PULS_EN_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_MAN_PULS_EN_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_MAN_PULS_EN_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00015030 LCLK_MANUAL_PULSE |
 | +---------------------------------------+
 | | bit  19:0 R/W  LCLK_OUT_MAN_PULS      |
 | +---------------------------------------+
 | 
 | void   digi_pgmrclk_field_LCLK_OUT_MAN_PULS_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_MAN_PULS_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_MAN_PULS_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_MAN_PULS_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------+
 | | Register 0x00015034 LCLK_ENS |
 | +------------------------------+
 | | bit  19:0 R/W  LCLK_OUT_EN   |
 | +------------------------------+
 | 
 | void   digi_pgmrclk_field_LCLK_OUT_EN_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_OUT_EN_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_OUT_EN_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_OUT_EN_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00015038 LCLK_GENERATOR_BYPASS |
 | +-------------------------------------------+
 | | bit  7:0 R/W  LCLK_GEN_BYPASS             |
 | +-------------------------------------------+
 | 
 | void   digi_pgmrclk_field_LCLK_GEN_BYPASS_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_BYPASS_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_LCLK_GEN_BYPASS_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_LCLK_GEN_BYPASS_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00015044 FPI_GENERATOR_CFG |
 | +---------------------------------------+
 | | bit  29:27 R/W  MISC_FPI_8            |
 | | bit  26    R/W  AUTO_LCLKOUT_EN_8     |
 | | bit  25    R/W  FPI_PULS_GEN_8        |
 | | bit  24    R/W  INTERVAL_RST_8        |
 | | bit  23:10 R/W  SYNC_INTERVAL_8       |
 | | bit  9     R/W  SYNC_DISABLE_8        |
 | | bit  8     R/W  CNTR_ENABLE_8         |
 | | bit  7:4   R/W  MASK_WINDOW_8         |
 | | bit  3     R/W  FPI_GEN_BYPASS        |
 | | bit  2     R/W  FPI_OUT_MAN_PULS_EN   |
 | | bit  1     R/W  FPI_OUT_MAN_PULS      |
 | | bit  0     R/W  FPI_OUT_EN            |
 | +---------------------------------------+
 | 
 | void   digi_pgmrclk_field_MISC_FPI_8_set            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MISC_FPI_8_get            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MISC_FPI_8_set      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MISC_FPI_8_get      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_AUTO_LCLKOUT_EN_8_set     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_AUTO_LCLKOUT_EN_8_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_FPI_PULS_GEN_8_set        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FPI_PULS_GEN_8_get        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_INTERVAL_RST_8_set        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_INTERVAL_RST_8_get        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_SYNC_INTERVAL_8_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_INTERVAL_8_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_SYNC_INTERVAL_8_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_SYNC_INTERVAL_8_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_SYNC_DISABLE_8_set        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_SYNC_DISABLE_8_get        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_CNTR_ENABLE_8_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_CNTR_ENABLE_8_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_MASK_WINDOW_8_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_MASK_WINDOW_8_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_MASK_WINDOW_8_set   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_MASK_WINDOW_8_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_FPI_GEN_BYPASS_set        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FPI_GEN_BYPASS_get        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_FPI_OUT_MAN_PULS_EN_set   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FPI_OUT_MAN_PULS_EN_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_FPI_OUT_MAN_PULS_set      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FPI_OUT_MAN_PULS_get      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_FPI_OUT_EN_set            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FPI_OUT_EN_get            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00015048 TIP_COMPLETE_STAT |
 | +---------------------------------------+
 | | bit  7:0 R  SS_TIP_COMPLETE           |
 | +---------------------------------------+
 | 
 | UINT32 digi_pgmrclk_field_SS_TIP_COMPLETE_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | UINT32 digi_pgmrclk_field_range_SS_TIP_COMPLETE_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x0001504c TIP_MASK_0 |
 | +--------------------------------+
 | | bit  17:0 R/W  TIP_MASK_0      |
 | +--------------------------------+
 | 
 | void   digi_pgmrclk_field_TIP_MASK_0_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_MASK_0_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_TIP_MASK_0_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_TIP_MASK_0_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x00015050 TIP_MASK_1 |
 | +--------------------------------+
 | | bit  17:0 R/W  TIP_MASK_1      |
 | +--------------------------------+
 | 
 | void   digi_pgmrclk_field_TIP_MASK_1_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_MASK_1_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_TIP_MASK_1_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_TIP_MASK_1_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x00015054 TIP_MASK_2 |
 | +--------------------------------+
 | | bit  17:0 R/W  TIP_MASK_2      |
 | +--------------------------------+
 | 
 | void   digi_pgmrclk_field_TIP_MASK_2_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_MASK_2_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_TIP_MASK_2_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_TIP_MASK_2_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x00015058 TIP_MASK_3 |
 | +--------------------------------+
 | | bit  17:0 R/W  TIP_MASK_3      |
 | +--------------------------------+
 | 
 | void   digi_pgmrclk_field_TIP_MASK_3_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_MASK_3_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_TIP_MASK_3_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_TIP_MASK_3_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x0001505c TIP_MASK_4 |
 | +--------------------------------+
 | | bit  17:0 R/W  TIP_MASK_4      |
 | +--------------------------------+
 | 
 | void   digi_pgmrclk_field_TIP_MASK_4_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_MASK_4_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_TIP_MASK_4_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_TIP_MASK_4_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x00015060 TIP_MASK_5 |
 | +--------------------------------+
 | | bit  17:0 R/W  TIP_MASK_5      |
 | +--------------------------------+
 | 
 | void   digi_pgmrclk_field_TIP_MASK_5_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_MASK_5_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_TIP_MASK_5_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_TIP_MASK_5_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x00015064 TIP_MASK_6 |
 | +--------------------------------+
 | | bit  17:0 R/W  TIP_MASK_6      |
 | +--------------------------------+
 | 
 | void   digi_pgmrclk_field_TIP_MASK_6_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_MASK_6_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_TIP_MASK_6_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_TIP_MASK_6_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x00015068 TIP_MASK_7 |
 | +--------------------------------+
 | | bit  17:0 R/W  TIP_MASK_7      |
 | +--------------------------------+
 | 
 | void   digi_pgmrclk_field_TIP_MASK_7_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_MASK_7_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_TIP_MASK_7_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_TIP_MASK_7_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x0001506c INT_REG_0 |
 | +-------------------------------+
 | | bit  27 R/W  TIP_FALLING_7_I  |
 | | bit  26 R/W  TIP_FALLING_6_I  |
 | | bit  25 R/W  TIP_FALLING_5_I  |
 | | bit  24 R/W  TIP_FALLING_4_I  |
 | | bit  23 R/W  TIP_FALLING_3_I  |
 | | bit  22 R/W  TIP_FALLING_2_I  |
 | | bit  21 R/W  TIP_FALLING_1_I  |
 | | bit  20 R/W  TIP_FALLING_0_I  |
 | | bit  19 R/W  TIP_RISING_7_I   |
 | | bit  18 R/W  TIP_RISING_6_I   |
 | | bit  17 R/W  TIP_RISING_5_I   |
 | | bit  16 R/W  TIP_RISING_4_I   |
 | | bit  15 R/W  TIP_RISING_3_I   |
 | | bit  14 R/W  TIP_RISING_2_I   |
 | | bit  13 R/W  TIP_RISING_1_I   |
 | | bit  12 R/W  TIP_RISING_0_I   |
 | | bit  11 R/W  FPI_OUT_I        |
 | | bit  10 R/W  FPI_IN_I         |
 | | bit  9  R/W  ALU_BKP_SYNC_I   |
 | | bit  8  R/W  PMON_SYNC_I      |
 | | bit  7  R/W  LCLK_GEN_7_I     |
 | | bit  6  R/W  LCLK_GEN_6_I     |
 | | bit  5  R/W  LCLK_GEN_5_I     |
 | | bit  4  R/W  LCLK_GEN_4_I     |
 | | bit  3  R/W  LCLK_GEN_3_I     |
 | | bit  2  R/W  LCLK_GEN_2_I     |
 | | bit  1  R/W  LCLK_GEN_1_I     |
 | | bit  0  R/W  LCLK_GEN_0_I     |
 | +-------------------------------+
 | 
 | void                 digi_pgmrclk_field_TIP_FALLING_7_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_7_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_7_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_FALLING_6_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_6_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_6_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_FALLING_5_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_5_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_5_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_FALLING_4_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_4_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_4_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_FALLING_3_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_3_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_3_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_FALLING_2_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_2_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_2_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_FALLING_1_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_1_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_1_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_FALLING_0_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_0_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_0_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_RISING_7_I_set_to_clear  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_RISING_7_I_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_7_I_poll          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_RISING_6_I_set_to_clear  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_RISING_6_I_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_6_I_poll          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_RISING_5_I_set_to_clear  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_RISING_5_I_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_5_I_poll          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_RISING_4_I_set_to_clear  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_RISING_4_I_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_4_I_poll          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_RISING_3_I_set_to_clear  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_RISING_3_I_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_3_I_poll          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_RISING_2_I_set_to_clear  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_RISING_2_I_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_2_I_poll          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_RISING_1_I_set_to_clear  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_RISING_1_I_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_1_I_poll          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_TIP_RISING_0_I_set_to_clear  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_TIP_RISING_0_I_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_0_I_poll          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_FPI_OUT_I_set_to_clear       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_FPI_OUT_I_get                ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_FPI_OUT_I_poll               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_FPI_IN_I_set_to_clear        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_FPI_IN_I_get                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_FPI_IN_I_poll                ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_ALU_BKP_SYNC_I_set_to_clear  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_ALU_BKP_SYNC_I_get           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_ALU_BKP_SYNC_I_poll          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_PMON_SYNC_I_set_to_clear     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_PMON_SYNC_I_get              ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_PMON_SYNC_I_poll             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_7_I_set_to_clear    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_7_I_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_7_I_poll            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_6_I_set_to_clear    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_6_I_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_6_I_poll            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_5_I_set_to_clear    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_5_I_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_5_I_poll            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_4_I_set_to_clear    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_4_I_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_4_I_poll            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_3_I_set_to_clear    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_3_I_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_3_I_poll            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_2_I_set_to_clear    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_2_I_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_2_I_poll            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_1_I_set_to_clear    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_1_I_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_1_I_poll            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_0_I_set_to_clear    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_0_I_get             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_0_I_poll            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------+
 | | Register 0x00015070 INT_REG_1 |
 | +-------------------------------+
 | | bit  8 R/W  FPI_SYNC_I        |
 | | bit  7 R/W  LCLK_GEN_SYNC_7_I |
 | | bit  6 R/W  LCLK_GEN_SYNC_6_I |
 | | bit  5 R/W  LCLK_GEN_SYNC_5_I |
 | | bit  4 R/W  LCLK_GEN_SYNC_4_I |
 | | bit  3 R/W  LCLK_GEN_SYNC_3_I |
 | | bit  2 R/W  LCLK_GEN_SYNC_2_I |
 | | bit  1 R/W  LCLK_GEN_SYNC_1_I |
 | | bit  0 R/W  LCLK_GEN_SYNC_0_I |
 | +-------------------------------+
 | 
 | void                 digi_pgmrclk_field_FPI_SYNC_I_set_to_clear        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_FPI_SYNC_I_get                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_FPI_SYNC_I_poll                ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_SYNC_7_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_7_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_7_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_SYNC_6_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_6_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_6_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_SYNC_5_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_5_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_5_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_SYNC_4_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_4_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_4_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_SYNC_3_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_3_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_3_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_SYNC_2_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_2_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_2_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_SYNC_1_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_1_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_1_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 digi_pgmrclk_field_LCLK_GEN_SYNC_0_I_set_to_clear ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_0_I_get          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_0_I_poll         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------+
 | | Register 0x00015074 INT_EN_REG_0 |
 | +----------------------------------+
 | | bit  27 R/W  TIP_FALLING_7_E     |
 | | bit  26 R/W  TIP_FALLING_6_E     |
 | | bit  25 R/W  TIP_FALLING_5_E     |
 | | bit  24 R/W  TIP_FALLING_4_E     |
 | | bit  23 R/W  TIP_FALLING_3_E     |
 | | bit  22 R/W  TIP_FALLING_2_E     |
 | | bit  21 R/W  TIP_FALLING_1_E     |
 | | bit  20 R/W  TIP_FALLING_0_E     |
 | | bit  19 R/W  TIP_RISING_7_E      |
 | | bit  18 R/W  TIP_RISING_6_E      |
 | | bit  17 R/W  TIP_RISING_5_E      |
 | | bit  16 R/W  TIP_RISING_4_E      |
 | | bit  15 R/W  TIP_RISING_3_E      |
 | | bit  14 R/W  TIP_RISING_2_E      |
 | | bit  13 R/W  TIP_RISING_1_E      |
 | | bit  12 R/W  TIP_RISING_0_E      |
 | | bit  11 R/W  FPI_OUT_E           |
 | | bit  10 R/W  FPI_IN_E            |
 | | bit  9  R/W  ALU_BKP_SYNC_E      |
 | | bit  8  R/W  PMON_SYNC_E         |
 | | bit  7  R/W  LCLK_GEN_7_E        |
 | | bit  6  R/W  LCLK_GEN_6_E        |
 | | bit  5  R/W  LCLK_GEN_5_E        |
 | | bit  4  R/W  LCLK_GEN_4_E        |
 | | bit  3  R/W  LCLK_GEN_3_E        |
 | | bit  2  R/W  LCLK_GEN_2_E        |
 | | bit  1  R/W  LCLK_GEN_1_E        |
 | | bit  0  R/W  LCLK_GEN_0_E        |
 | +----------------------------------+
 | 
 | void   digi_pgmrclk_field_TIP_FALLING_7_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_FALLING_7_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_FALLING_6_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_FALLING_6_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_FALLING_5_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_FALLING_5_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_FALLING_4_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_FALLING_4_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_FALLING_3_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_FALLING_3_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_FALLING_2_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_FALLING_2_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_FALLING_1_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_FALLING_1_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_FALLING_0_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_FALLING_0_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_RISING_7_E_set  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_RISING_7_E_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_RISING_6_E_set  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_RISING_6_E_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_RISING_5_E_set  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_RISING_5_E_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_RISING_4_E_set  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_RISING_4_E_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_RISING_3_E_set  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_RISING_3_E_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_RISING_2_E_set  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_RISING_2_E_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_RISING_1_E_set  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_RISING_1_E_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_TIP_RISING_0_E_set  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_TIP_RISING_0_E_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_FPI_OUT_E_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FPI_OUT_E_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_FPI_IN_E_set        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FPI_IN_E_get        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_ALU_BKP_SYNC_E_set  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_ALU_BKP_SYNC_E_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_PMON_SYNC_E_set     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_PMON_SYNC_E_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_7_E_set    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_7_E_get    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_6_E_set    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_6_E_get    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_5_E_set    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_5_E_get    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_4_E_set    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_4_E_get    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_3_E_set    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_3_E_get    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_2_E_set    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_2_E_get    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_1_E_set    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_1_E_get    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_0_E_set    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_0_E_get    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +----------------------------------+
 | | Register 0x00015078 INT_EN_REG_1 |
 | +----------------------------------+
 | | bit  8 R/W  FPI_SYNC_E           |
 | | bit  7 R/W  LCLK_GEN_SYNC_7_E    |
 | | bit  6 R/W  LCLK_GEN_SYNC_6_E    |
 | | bit  5 R/W  LCLK_GEN_SYNC_5_E    |
 | | bit  4 R/W  LCLK_GEN_SYNC_4_E    |
 | | bit  3 R/W  LCLK_GEN_SYNC_3_E    |
 | | bit  2 R/W  LCLK_GEN_SYNC_2_E    |
 | | bit  1 R/W  LCLK_GEN_SYNC_1_E    |
 | | bit  0 R/W  LCLK_GEN_SYNC_0_E    |
 | +----------------------------------+
 | 
 | void   digi_pgmrclk_field_FPI_SYNC_E_set        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FPI_SYNC_E_get        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_SYNC_7_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_SYNC_7_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_SYNC_6_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_SYNC_6_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_SYNC_5_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_SYNC_5_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_SYNC_4_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_SYNC_4_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_SYNC_3_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_SYNC_3_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_SYNC_2_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_SYNC_2_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_SYNC_1_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_SYNC_1_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_LCLK_GEN_SYNC_0_E_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_LCLK_GEN_SYNC_0_E_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x0001507c INT_STAT_REG_0 |
 | +------------------------------------+
 | | bit  27 R  TIP_FALLING_7_V         |
 | | bit  26 R  TIP_FALLING_6_V         |
 | | bit  25 R  TIP_FALLING_5_V         |
 | | bit  24 R  TIP_FALLING_4_V         |
 | | bit  23 R  TIP_FALLING_3_V         |
 | | bit  22 R  TIP_FALLING_2_V         |
 | | bit  21 R  TIP_FALLING_1_V         |
 | | bit  20 R  TIP_FALLING_0_V         |
 | | bit  19 R  TIP_RISING_7_V          |
 | | bit  18 R  TIP_RISING_6_V          |
 | | bit  17 R  TIP_RISING_5_V          |
 | | bit  16 R  TIP_RISING_4_V          |
 | | bit  15 R  TIP_RISING_3_V          |
 | | bit  14 R  TIP_RISING_2_V          |
 | | bit  13 R  TIP_RISING_1_V          |
 | | bit  12 R  TIP_RISING_0_V          |
 | | bit  11 R  FPI_OUT_V               |
 | | bit  10 R  FPI_IN_V                |
 | | bit  9  R  ALU_BKP_SYNC_V          |
 | | bit  8  R  PMON_SYNC_V             |
 | | bit  7  R  LCLK_GEN_7_V            |
 | | bit  6  R  LCLK_GEN_6_V            |
 | | bit  5  R  LCLK_GEN_5_V            |
 | | bit  4  R  LCLK_GEN_4_V            |
 | | bit  3  R  LCLK_GEN_3_V            |
 | | bit  2  R  LCLK_GEN_2_V            |
 | | bit  1  R  LCLK_GEN_1_V            |
 | | bit  0  R  LCLK_GEN_0_V            |
 | +------------------------------------+
 | 
 | UINT32               digi_pgmrclk_field_TIP_FALLING_7_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_7_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_6_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_6_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_5_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_5_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_4_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_4_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_3_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_3_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_2_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_2_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_1_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_1_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_FALLING_0_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_FALLING_0_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_RISING_7_V_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_7_V_poll  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_RISING_6_V_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_6_V_poll  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_RISING_5_V_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_5_V_poll  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_RISING_4_V_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_4_V_poll  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_RISING_3_V_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_3_V_poll  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_RISING_2_V_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_2_V_poll  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_RISING_1_V_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_1_V_poll  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_TIP_RISING_0_V_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_TIP_RISING_0_V_poll  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_FPI_OUT_V_get        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_FPI_OUT_V_poll       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_FPI_IN_V_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_FPI_IN_V_poll        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_ALU_BKP_SYNC_V_get   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_ALU_BKP_SYNC_V_poll  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_PMON_SYNC_V_get      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_PMON_SYNC_V_poll     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_7_V_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_7_V_poll    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_6_V_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_6_V_poll    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_5_V_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_5_V_poll    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_4_V_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_4_V_poll    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_3_V_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_3_V_poll    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_2_V_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_2_V_poll    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_1_V_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_1_V_poll    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_0_V_get     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_0_V_poll    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00015080 INT_STAT_REG_1 |
 | +------------------------------------+
 | | bit  8 R  FPI_SYNC_V               |
 | | bit  7 R  LCLK_GEN_SYNC_7_V        |
 | | bit  6 R  LCLK_GEN_SYNC_6_V        |
 | | bit  5 R  LCLK_GEN_SYNC_5_V        |
 | | bit  4 R  LCLK_GEN_SYNC_4_V        |
 | | bit  3 R  LCLK_GEN_SYNC_3_V        |
 | | bit  2 R  LCLK_GEN_SYNC_2_V        |
 | | bit  1 R  LCLK_GEN_SYNC_1_V        |
 | | bit  0 R  LCLK_GEN_SYNC_0_V        |
 | +------------------------------------+
 | 
 | UINT32               digi_pgmrclk_field_FPI_SYNC_V_get         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_FPI_SYNC_V_poll        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_7_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_7_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_6_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_6_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_5_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_5_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_4_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_4_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_3_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_3_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_2_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_2_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_1_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_1_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               digi_pgmrclk_field_LCLK_GEN_SYNC_0_V_get  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_field_LCLK_GEN_SYNC_0_V_poll ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x00015084 FDIV_0_CLK_SELECT |
 | +---------------------------------------+
 | | bit  8:0 R/W  FDIV_CLK_MUX_SEL_0      |
 | +---------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_CLK_MUX_SEL_0_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_CLK_MUX_SEL_0_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_CLK_MUX_SEL_0_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_CLK_MUX_SEL_0_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00015088 FDIV_1_CLK_SELECT |
 | +---------------------------------------+
 | | bit  8:0 R/W  FDIV_CLK_MUX_SEL_1      |
 | +---------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_CLK_MUX_SEL_1_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_CLK_MUX_SEL_1_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_CLK_MUX_SEL_1_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_CLK_MUX_SEL_1_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x0001508c FDIV_2_CLK_SELECT |
 | +---------------------------------------+
 | | bit  8:0 R/W  FDIV_CLK_MUX_SEL_2      |
 | +---------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_CLK_MUX_SEL_2_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_CLK_MUX_SEL_2_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_CLK_MUX_SEL_2_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_CLK_MUX_SEL_2_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00015090 FDIV_3_CLK_SELECT |
 | +---------------------------------------+
 | | bit  8:0 R/W  FDIV_CLK_MUX_SEL_3      |
 | +---------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_CLK_MUX_SEL_3_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_CLK_MUX_SEL_3_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_CLK_MUX_SEL_3_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_CLK_MUX_SEL_3_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00015094 FDIV_0_INTEGER |
 | +------------------------------------+
 | | bit  31:0 R/W  FDIV_INTEGER_0      |
 | +------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_INTEGER_0_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_INTEGER_0_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_INTEGER_0_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_INTEGER_0_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00015098 FDIV_1_INTEGER |
 | +------------------------------------+
 | | bit  31:0 R/W  FDIV_INTEGER_1      |
 | +------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_INTEGER_1_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_INTEGER_1_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_INTEGER_1_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_INTEGER_1_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x0001509c FDIV_2_INTEGER |
 | +------------------------------------+
 | | bit  31:0 R/W  FDIV_INTEGER_2      |
 | +------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_INTEGER_2_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_INTEGER_2_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_INTEGER_2_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_INTEGER_2_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x000150a0 FDIV_3_INTEGER |
 | +------------------------------------+
 | | bit  31:0 R/W  FDIV_INTEGER_3      |
 | +------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_INTEGER_3_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_INTEGER_3_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_INTEGER_3_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_INTEGER_3_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x000150a4 FDIV_0_NUMERATOR |
 | +--------------------------------------+
 | | bit  31:0 R/W  FDIV_NUMERATOR_0      |
 | +--------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_NUMERATOR_0_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_NUMERATOR_0_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_NUMERATOR_0_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_NUMERATOR_0_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x000150a8 FDIV_1_NUMERATOR |
 | +--------------------------------------+
 | | bit  31:0 R/W  FDIV_NUMERATOR_1      |
 | +--------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_NUMERATOR_1_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_NUMERATOR_1_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_NUMERATOR_1_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_NUMERATOR_1_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x000150ac FDIV_2_NUMERATOR |
 | +--------------------------------------+
 | | bit  31:0 R/W  FDIV_NUMERATOR_2      |
 | +--------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_NUMERATOR_2_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_NUMERATOR_2_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_NUMERATOR_2_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_NUMERATOR_2_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x000150b0 FDIV_3_NUMERATOR |
 | +--------------------------------------+
 | | bit  31:0 R/W  FDIV_NUMERATOR_3      |
 | +--------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_NUMERATOR_3_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_NUMERATOR_3_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_NUMERATOR_3_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_NUMERATOR_3_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000150b4 FDIV_0_DENOMINATOR |
 | +----------------------------------------+
 | | bit  31:0 R/W  FDIV_DENOMINATOR_0      |
 | +----------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_DENOMINATOR_0_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_DENOMINATOR_0_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_DENOMINATOR_0_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_DENOMINATOR_0_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000150b8 FDIV_1_DENOMINATOR |
 | +----------------------------------------+
 | | bit  31:0 R/W  FDIV_DENOMINATOR_1      |
 | +----------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_DENOMINATOR_1_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_DENOMINATOR_1_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_DENOMINATOR_1_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_DENOMINATOR_1_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000150bc FDIV_2_DENOMINATOR |
 | +----------------------------------------+
 | | bit  31:0 R/W  FDIV_DENOMINATOR_2      |
 | +----------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_DENOMINATOR_2_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_DENOMINATOR_2_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_DENOMINATOR_2_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_DENOMINATOR_2_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000150c0 FDIV_3_DENOMINATOR |
 | +----------------------------------------+
 | | bit  31:0 R/W  FDIV_DENOMINATOR_3      |
 | +----------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_DENOMINATOR_3_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_DENOMINATOR_3_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_DENOMINATOR_3_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_DENOMINATOR_3_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x000150c8 FDIV_EN_AND_RESET |
 | +---------------------------------------+
 | | bit  7:4 R/W  FDIV_ENABLE             |
 | | bit  3:0 R/W  FDIV_SW_RST             |
 | +---------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_ENABLE_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_ENABLE_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_ENABLE_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_ENABLE_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_pgmrclk_field_FDIV_SW_RST_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_SW_RST_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_SW_RST_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_SW_RST_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x000150cc FDIV_OUTPUT_EN |
 | +------------------------------------+
 | | bit  3:0 R/W  FDIV_OUT_ENABLE      |
 | +------------------------------------+
 | 
 | void   digi_pgmrclk_field_FDIV_OUT_ENABLE_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_FDIV_OUT_ENABLE_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_FDIV_OUT_ENABLE_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_FDIV_OUT_ENABLE_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------+
 | | Register 0x000150d0 IDIV_EN |
 | +-----------------------------+
 | | bit  1:0 R/W  IDIV_ENABLE   |
 | +-----------------------------+
 | 
 | void   digi_pgmrclk_field_IDIV_ENABLE_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_pgmrclk_field_IDIV_ENABLE_get       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_pgmrclk_field_range_IDIV_ENABLE_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_pgmrclk_field_range_IDIV_ENABLE_get ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _DIGI_PGMRCLK_IO_H
#define _DIGI_PGMRCLK_IO_H

#include "digi_api.h"
#include "digi_pgmrclk_regs.h"
#include "digi_pgmrclk_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 digi_pgmrclk_buffer_init               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void                 digi_pgmrclk_buffer_flush              ( digi_pgmrclk_buffer_t *b_ptr )
 | UINT32               digi_pgmrclk_reg_read                  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 digi_pgmrclk_reg_write                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 digi_pgmrclk_field_set                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 digi_pgmrclk_action_on_write_field_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 digi_pgmrclk_burst_read                ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 digi_pgmrclk_burst_write               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_poll                      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_0_CFG_write           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_0_CFG_field_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_GENERATOR_0_CFG_read            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_1_CFG_write           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_1_CFG_field_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_GENERATOR_1_CFG_read            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_2_CFG_write           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_2_CFG_field_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_GENERATOR_2_CFG_read            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_3_CFG_write           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_3_CFG_field_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_GENERATOR_3_CFG_read            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_4_CFG_write           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_4_CFG_field_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_GENERATOR_4_CFG_read            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_5_CFG_write           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_5_CFG_field_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_GENERATOR_5_CFG_read            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_6_CFG_write           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_6_CFG_field_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_GENERATOR_6_CFG_read            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_7_CFG_write           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_7_CFG_field_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_GENERATOR_7_CFG_read            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_RESET_write           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_RESET_field_set       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_GENERATOR_RESET_read            ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_SELECTION_0_write     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_SELECTION_0_field_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_GENERATOR_SELECTION_0_read      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_SELECTION_1_write     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_SELECTION_1_field_set ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_GENERATOR_SELECTION_1_read      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_LCLK_MANUAL_PULSE_ENS_write          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_MANUAL_PULSE_ENS_field_set      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_MANUAL_PULSE_ENS_read           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_LCLK_MANUAL_PULSE_write              ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_MANUAL_PULSE_field_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_MANUAL_PULSE_read               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_LCLK_ENS_write                       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_ENS_field_set                   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_ENS_read                        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_BYPASS_write          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_LCLK_GENERATOR_BYPASS_field_set      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_LCLK_GENERATOR_BYPASS_read           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FPI_GENERATOR_CFG_write              ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FPI_GENERATOR_CFG_field_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FPI_GENERATOR_CFG_read               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_TIP_MASK_0_write                     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_TIP_MASK_0_field_set                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_TIP_MASK_0_read                      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_TIP_MASK_1_write                     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_TIP_MASK_1_field_set                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_TIP_MASK_1_read                      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_TIP_MASK_2_write                     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_TIP_MASK_2_field_set                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_TIP_MASK_2_read                      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_TIP_MASK_3_write                     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_TIP_MASK_3_field_set                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_TIP_MASK_3_read                      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_TIP_MASK_4_write                     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_TIP_MASK_4_field_set                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_TIP_MASK_4_read                      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_TIP_MASK_5_write                     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_TIP_MASK_5_field_set                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_TIP_MASK_5_read                      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_TIP_MASK_6_write                     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_TIP_MASK_6_field_set                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_TIP_MASK_6_read                      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_TIP_MASK_7_write                     ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_TIP_MASK_7_field_set                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_TIP_MASK_7_read                      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_0_CLK_SELECT_write              ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_0_CLK_SELECT_field_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_0_CLK_SELECT_read               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_1_CLK_SELECT_write              ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_1_CLK_SELECT_field_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_1_CLK_SELECT_read               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_2_CLK_SELECT_write              ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_2_CLK_SELECT_field_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_2_CLK_SELECT_read               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_3_CLK_SELECT_write              ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_3_CLK_SELECT_field_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_3_CLK_SELECT_read               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_0_INTEGER_write                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_0_INTEGER_field_set             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_0_INTEGER_read                  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_1_INTEGER_write                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_1_INTEGER_field_set             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_1_INTEGER_read                  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_2_INTEGER_write                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_2_INTEGER_field_set             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_2_INTEGER_read                  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_3_INTEGER_write                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_3_INTEGER_field_set             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_3_INTEGER_read                  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_0_NUMERATOR_write               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_0_NUMERATOR_field_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_0_NUMERATOR_read                ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_1_NUMERATOR_write               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_1_NUMERATOR_field_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_1_NUMERATOR_read                ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_2_NUMERATOR_write               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_2_NUMERATOR_field_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_2_NUMERATOR_read                ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_3_NUMERATOR_write               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_3_NUMERATOR_field_set           ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_3_NUMERATOR_read                ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_0_DENOMINATOR_write             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_0_DENOMINATOR_field_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_0_DENOMINATOR_read              ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_1_DENOMINATOR_write             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_1_DENOMINATOR_field_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_1_DENOMINATOR_read              ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_2_DENOMINATOR_write             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_2_DENOMINATOR_field_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_2_DENOMINATOR_read              ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_3_DENOMINATOR_write             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_3_DENOMINATOR_field_set         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_3_DENOMINATOR_read              ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_EN_AND_RESET_write              ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_EN_AND_RESET_field_set          ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_EN_AND_RESET_read               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_FDIV_OUTPUT_EN_write                 ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_FDIV_OUTPUT_EN_field_set             ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_FDIV_OUTPUT_EN_read                  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_IDIV_EN_write                        ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_IDIV_EN_field_set                    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_IDIV_EN_read                         ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_INT_EN_REG_0_write                   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_INT_EN_REG_0_field_set               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_INT_EN_REG_0_read                    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_INT_EN_REG_1_write                   ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_INT_EN_REG_1_field_set               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_INT_EN_REG_1_read                    ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void                 digi_pgmrclk_reg_INT_REG_0_write                      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_INT_REG_0_action_on_write_field_set  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_INT_REG_0_read                       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_reg_INT_REG_0_poll                       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 digi_pgmrclk_reg_INT_REG_1_write                      ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void                 digi_pgmrclk_reg_INT_REG_1_action_on_write_field_set  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               digi_pgmrclk_reg_INT_REG_1_read                       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_reg_INT_REG_1_poll                       ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               digi_pgmrclk_reg_INT_STAT_REG_0_read                  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_reg_INT_STAT_REG_0_poll                  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               digi_pgmrclk_reg_INT_STAT_REG_1_read                  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE digi_pgmrclk_reg_INT_STAT_REG_1_poll                  ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               digi_pgmrclk_reg_TIP_COMPLETE_STAT_read               ( digi_pgmrclk_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _DIGI_PGMRCLK_IO_H */
