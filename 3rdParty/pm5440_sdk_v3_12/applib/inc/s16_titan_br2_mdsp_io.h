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
 *     and register accessor functions for the s16_titan_br2_mdsp block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing s16_titan_br2_mdsp io functions is:
 | {
 |     s16_titan_br2_mdsp_buffer_t b_ptr[1];
 |     s16_titan_br2_mdsp_buffer_init( b_ptr, h_ptr, A );
 |     value = s16_titan_br2_mdsp_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     s16_titan_br2_mdsp_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     s16_titan_br2_mdsp_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = s16_titan_br2_mdsp_field_<FIELD>_get( NULL, h_ptr, A );
 | s16_titan_br2_mdsp_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------------------+
 | | Register 0x00000200 CONFIG_1_PM20_80_48_PP |
 | +--------------------------------------------+
 | | bit  29:24 R/W  ADC_D_MAP_4                |
 | | bit  23:18 R/W  ADC_D_MAP_3                |
 | | bit  17:12 R/W  ADC_D_MAP_2                |
 | | bit  11:6  R/W  ADC_D_MAP_1                |
 | | bit  5:0   R/W  ADC_D_MAP_0                |
 | +--------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_4_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_4_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_4_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_4_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_3_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_3_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_3_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_3_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_2_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_2_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_2_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_2_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_1_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_1_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_1_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_1_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_0_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_0_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_0_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_0_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000204 CONFIG_2_PM20_80_48_PP |
 | +--------------------------------------------+
 | | bit  29:24 R/W  ADC_D_MAP_9                |
 | | bit  23:18 R/W  ADC_D_MAP_8                |
 | | bit  17:12 R/W  ADC_D_MAP_7                |
 | | bit  11:6  R/W  ADC_D_MAP_6                |
 | | bit  5:0   R/W  ADC_D_MAP_5                |
 | +--------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_9_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_9_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_9_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_9_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_8_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_8_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_8_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_8_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_7_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_7_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_7_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_7_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_6_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_6_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_6_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_6_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_5_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_5_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_5_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_5_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000208 CONFIG_3_PM20_80_48_PP |
 | +--------------------------------------------+
 | | bit  29:24 R/W  ADC_D_MAP_14               |
 | | bit  23:18 R/W  ADC_D_MAP_13               |
 | | bit  17:12 R/W  ADC_D_MAP_12               |
 | | bit  11:6  R/W  ADC_D_MAP_11               |
 | | bit  5:0   R/W  ADC_D_MAP_10               |
 | +--------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_14_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_14_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_14_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_14_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_13_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_13_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_13_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_13_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_12_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_12_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_12_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_12_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_11_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_11_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_11_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_11_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_10_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_10_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_10_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_10_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0000020c CONFIG_4_PM20_80_48_PP |
 | +--------------------------------------------+
 | | bit  29:24 R/W  ADC_D_MAP_19               |
 | | bit  23:18 R/W  ADC_D_MAP_18               |
 | | bit  17:12 R/W  ADC_D_MAP_17               |
 | | bit  11:6  R/W  ADC_D_MAP_16               |
 | | bit  5:0   R/W  ADC_D_MAP_15               |
 | +--------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_19_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_19_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_19_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_19_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_18_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_18_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_18_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_18_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_17_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_17_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_17_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_17_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_16_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_16_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_16_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_16_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_15_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_15_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_15_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_15_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000210 CONFIG_5_PM20_80_48_PP |
 | +--------------------------------------------+
 | | bit  29:24 R/W  ADC_D_MAP_24               |
 | | bit  23:18 R/W  ADC_D_MAP_23               |
 | | bit  17:12 R/W  ADC_D_MAP_22               |
 | | bit  11:6  R/W  ADC_D_MAP_21               |
 | | bit  5:0   R/W  ADC_D_MAP_20               |
 | +--------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_24_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_24_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_24_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_24_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_23_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_23_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_23_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_23_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_22_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_22_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_22_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_22_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_21_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_21_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_21_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_21_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_20_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_20_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_20_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_20_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000214 CONFIG_6_PM20_80_48_PP |
 | +--------------------------------------------+
 | | bit  29:24 R/W  ADC_D_MAP_29               |
 | | bit  23:18 R/W  ADC_D_MAP_28               |
 | | bit  17:12 R/W  ADC_D_MAP_27               |
 | | bit  11:6  R/W  ADC_D_MAP_26               |
 | | bit  5:0   R/W  ADC_D_MAP_25               |
 | +--------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_29_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_29_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_29_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_29_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_28_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_28_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_28_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_28_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_27_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_27_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_27_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_27_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_26_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_26_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_26_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_26_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_25_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_25_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_25_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_25_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000218 CONFIG_7_PM20_80_48_PP |
 | +--------------------------------------------+
 | | bit  11:6 R/W  ADC_D_MAP_31                |
 | | bit  5:0  R/W  ADC_D_MAP_30                |
 | +--------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_31_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_31_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_31_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_31_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_D_MAP_30_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_D_MAP_30_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_D_MAP_30_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_D_MAP_30_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0000021c CONFIG_8_PM20_80_48_PP |
 | +--------------------------------------------+
 | | bit  29:24 R/W  ADC_T_MAP_4                |
 | | bit  23:18 R/W  ADC_T_MAP_3                |
 | | bit  17:12 R/W  ADC_T_MAP_2                |
 | | bit  11:6  R/W  ADC_T_MAP_1                |
 | | bit  5:0   R/W  ADC_T_MAP_0                |
 | +--------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_4_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_4_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_4_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_4_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_3_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_3_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_3_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_3_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_2_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_2_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_2_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_2_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_1_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_1_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_1_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_1_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_0_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_0_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_0_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_0_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x00000220 CONFIG_9_PM20_80_48_PP |
 | +--------------------------------------------+
 | | bit  29:24 R/W  ADC_T_MAP_9                |
 | | bit  23:18 R/W  ADC_T_MAP_8                |
 | | bit  17:12 R/W  ADC_T_MAP_7                |
 | | bit  11:6  R/W  ADC_T_MAP_6                |
 | | bit  5:0   R/W  ADC_T_MAP_5                |
 | +--------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_9_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_9_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_9_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_9_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_8_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_8_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_8_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_8_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_7_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_7_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_7_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_7_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_6_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_6_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_6_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_6_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_5_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_5_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_5_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_5_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000224 CONFIG_10_PM20_80_48_PP |
 | +---------------------------------------------+
 | | bit  29:24 R/W  ADC_T_MAP_14                |
 | | bit  23:18 R/W  ADC_T_MAP_13                |
 | | bit  17:12 R/W  ADC_T_MAP_12                |
 | | bit  11:6  R/W  ADC_T_MAP_11                |
 | | bit  5:0   R/W  ADC_T_MAP_10                |
 | +---------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_14_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_14_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_14_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_14_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_13_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_13_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_13_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_13_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_12_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_12_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_12_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_12_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_11_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_11_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_11_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_11_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_10_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_10_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_10_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_10_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00000228 CONFIG_11_PM20_80_48_PP |
 | +---------------------------------------------+
 | | bit  5:0 R/W  ADC_T_MAP_15                  |
 | +---------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_ADC_T_MAP_15_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ADC_T_MAP_15_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_ADC_T_MAP_15_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_ADC_T_MAP_15_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register 0x0000022c                                          |
 | | CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46 |
 | +--------------------------------------------------------------+
 | | bit  9:8 R/W  BLOCK_SIZE                                     |
 | | bit  6   R/W  CFG_FROM_PCBI_EN                               |
 | | bit  5   R/W  PCBI_PARAM_EN                                  |
 | | bit  2   R/W  INV_DFE_OUT                                    |
 | | bit  1   R/W  INV_DFE_ERR                                    |
 | | bit  0   R/W  INV_DP_FFE_IN                                  |
 | +--------------------------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_BLOCK_SIZE_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_BLOCK_SIZE_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_BLOCK_SIZE_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_BLOCK_SIZE_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_CFG_FROM_PCBI_EN_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_CFG_FROM_PCBI_EN_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_PCBI_PARAM_EN_set    ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_PCBI_PARAM_EN_get    ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_INV_DFE_OUT_set      ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_INV_DFE_OUT_get      ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_INV_DFE_ERR_set      ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_INV_DFE_ERR_get      ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_INV_DP_FFE_IN_set    ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_INV_DP_FFE_IN_get    ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------------------+
 | | Register 0x00000230 CONFIG_1_LW20_80_49_TR_BR2 |
 | +------------------------------------------------+
 | | bit  26    R/W  ENCODER_FLOP_BYPASS_EN         |
 | | bit  25    R/W  IMUXCODE_BRIDGE_EN             |
 | | bit  24    R/W  UNLOCK_EYE_EDGE_EN             |
 | | bit  23:21 R/W  LF_INTEG_LEFT_SHIFT            |
 | | bit  20:16 R/W  LF_INTEG_CTRL_GAIN             |
 | | bit  15:14 R/W  LF_PROP_LEFT_SHIFT             |
 | | bit  13:9  R/W  LF_PROP_CTRL_GAIN              |
 | | bit  5:0   R/W  LF_BIAS                        |
 | +------------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_ENCODER_FLOP_BYPASS_EN_set    ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_ENCODER_FLOP_BYPASS_EN_get    ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_IMUXCODE_BRIDGE_EN_set        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_IMUXCODE_BRIDGE_EN_get        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_UNLOCK_EYE_EDGE_EN_set        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_UNLOCK_EYE_EDGE_EN_get        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_LF_INTEG_LEFT_SHIFT_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_LF_INTEG_LEFT_SHIFT_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_LF_INTEG_LEFT_SHIFT_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_LF_INTEG_LEFT_SHIFT_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_LF_INTEG_CTRL_GAIN_set        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_LF_INTEG_CTRL_GAIN_get        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_LF_INTEG_CTRL_GAIN_set  ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_LF_INTEG_CTRL_GAIN_get  ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_LF_PROP_LEFT_SHIFT_set        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_LF_PROP_LEFT_SHIFT_get        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_LF_PROP_LEFT_SHIFT_set  ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_LF_PROP_LEFT_SHIFT_get  ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_LF_PROP_CTRL_GAIN_set         ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_LF_PROP_CTRL_GAIN_get         ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_LF_PROP_CTRL_GAIN_set   ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_LF_PROP_CTRL_GAIN_get   ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_LF_BIAS_set                   ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_LF_BIAS_get                   ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_LF_BIAS_set             ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_LF_BIAS_get             ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000234 CONFIG_2_LW20_80_49_TR_BR2 |
 | +------------------------------------------------+
 | | bit  26:21 R/W  INTERP_IDX_OVR                 |
 | | bit  20    R/W  INTERP_IDX_OVR_EN              |
 | | bit  19:18 R/W  LF_INTEG_OUT_MODE              |
 | | bit  11:0  R/W  LF_INTEG_OUT_OVR               |
 | +------------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_INTERP_IDX_OVR_set          ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_INTERP_IDX_OVR_get          ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_INTERP_IDX_OVR_set    ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_INTERP_IDX_OVR_get    ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_INTERP_IDX_OVR_EN_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_INTERP_IDX_OVR_EN_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_LF_INTEG_OUT_MODE_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_LF_INTEG_OUT_MODE_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_MODE_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_MODE_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_LF_INTEG_OUT_OVR_set        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_LF_INTEG_OUT_OVR_get        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_OVR_set  ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_LF_INTEG_OUT_OVR_get  ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000238 CONFIG_3_LW20_80_49_TR_BR2 |
 | +------------------------------------------------+
 | | bit  11:0 R/W  LF_CLAMP_MAX                    |
 | +------------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_LF_CLAMP_MAX_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_LF_CLAMP_MAX_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_LF_CLAMP_MAX_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_LF_CLAMP_MAX_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x0000023c CONFIG_4_LW20_80_49_TR_BR2 |
 | +------------------------------------------------+
 | | bit  17:12 R/W  EQ_TARGET_AMP                  |
 | | bit  11:0  R/W  LF_CLAMP_MIN                   |
 | +------------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_EQ_TARGET_AMP_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_EQ_TARGET_AMP_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_EQ_TARGET_AMP_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_EQ_TARGET_AMP_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_LF_CLAMP_MIN_set        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_LF_CLAMP_MIN_get        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_LF_CLAMP_MIN_set  ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_LF_CLAMP_MIN_get  ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000240 CONFIG_1_LW20_80_46_DP_T16 |
 | +------------------------------------------------+
 | | bit  20    R/W  PTW_SUM_OVR_EN                 |
 | | bit  17:12 R/W  PTW_SUM_SEL                    |
 | | bit  11:0  R/W  PTW_SUM_OVR                    |
 | +------------------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_PTW_SUM_OVR_EN_set    ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_PTW_SUM_OVR_EN_get    ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_PTW_SUM_SEL_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_PTW_SUM_SEL_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_PTW_SUM_SEL_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_PTW_SUM_SEL_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_PTW_SUM_OVR_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_PTW_SUM_OVR_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_PTW_SUM_OVR_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_PTW_SUM_OVR_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00000244 PRE_PROCESS_DATA_CAPTURED_0 |
 | +-------------------------------------------------+
 | | bit  17:12 R  INTERP_IDX_CAPT                   |
 | | bit  11:6  R  PP_T_DATA_CAPT                    |
 | | bit  5:0   R  PP_D_DATA_CAPT                    |
 | +-------------------------------------------------+
 | 
 | UINT32 s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 s16_titan_br2_mdsp_field_PP_T_DATA_CAPT_get        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_titan_br2_mdsp_field_range_PP_T_DATA_CAPT_get  ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 s16_titan_br2_mdsp_field_PP_D_DATA_CAPT_get        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_titan_br2_mdsp_field_range_PP_D_DATA_CAPT_get  ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x00000248 DFE_STATE |
 | +-------------------------------+
 | | bit  15:0 R  DFE_STATE_CAPT   |
 | +-------------------------------+
 | 
 | UINT32 s16_titan_br2_mdsp_field_DFE_STATE_CAPT_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x0000024c TCB_STATE_0 |
 | +---------------------------------+
 | | bit  17:0 R  INTEGRAL_ACC       |
 | +---------------------------------+
 | 
 | UINT32 s16_titan_br2_mdsp_field_INTEGRAL_ACC_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | UINT32 s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x00000250 DIAG_CONTROL       |
 | +----------------------------------------+
 | | bit  29:25 R/W  CAPT_RDBK_SEL          |
 | | bit  24:19 R/W  INTERP_IDX_CAPT_TIME   |
 | | bit  18:13 R/W  INTEGRAL_ACC_CAPT_TIME |
 | | bit  12:7  R/W  DFE_STATE_CAPT_TIME    |
 | | bit  6:1   R/W  SAMP_CAPT_TIME         |
 | | bit  0     R/W  CAPTURE_REQ            |
 | +----------------------------------------+
 | 
 | void   s16_titan_br2_mdsp_field_CAPT_RDBK_SEL_set                ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_CAPT_RDBK_SEL_get                ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_CAPT_RDBK_SEL_set          ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_CAPT_RDBK_SEL_get          ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_TIME_set         ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_INTERP_IDX_CAPT_TIME_get         ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_TIME_set   ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_INTERP_IDX_CAPT_TIME_get   ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_INTEGRAL_ACC_CAPT_TIME_set       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_INTEGRAL_ACC_CAPT_TIME_get       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_CAPT_TIME_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_INTEGRAL_ACC_CAPT_TIME_get ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_DFE_STATE_CAPT_TIME_set          ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_DFE_STATE_CAPT_TIME_get          ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_TIME_set    ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_DFE_STATE_CAPT_TIME_get    ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_SAMP_CAPT_TIME_set               ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_SAMP_CAPT_TIME_get               ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void   s16_titan_br2_mdsp_field_range_SAMP_CAPT_TIME_set         ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_range_SAMP_CAPT_TIME_get         ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   s16_titan_br2_mdsp_field_CAPTURE_REQ_set                  ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_field_CAPTURE_REQ_get                  ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _S16_TITAN_BR2_MDSP_IO_H
#define _S16_TITAN_BR2_MDSP_IO_H

#include "s16_loc.h"
#include "s16_titan_br2_mdsp_regs.h"
#include "s16_titan_br2_mdsp_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 s16_titan_br2_mdsp_buffer_init               ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | void                 s16_titan_br2_mdsp_buffer_flush              ( s16_titan_br2_mdsp_buffer_t *b_ptr )
 | UINT32               s16_titan_br2_mdsp_reg_read                  ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 s16_titan_br2_mdsp_reg_write                 ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 s16_titan_br2_mdsp_field_set                 ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 s16_titan_br2_mdsp_action_on_write_field_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 s16_titan_br2_mdsp_burst_read                ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 s16_titan_br2_mdsp_burst_write               ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE s16_titan_br2_mdsp_poll                      ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_write                                           ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_field_set                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_1_PM20_80_48_PP_read                                            ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_write                                           ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_field_set                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_2_PM20_80_48_PP_read                                            ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_write                                           ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_field_set                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_3_PM20_80_48_PP_read                                            ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_write                                           ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_field_set                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_4_PM20_80_48_PP_read                                            ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_write                                           ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_field_set                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_5_PM20_80_48_PP_read                                            ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_write                                           ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_field_set                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_6_PM20_80_48_PP_read                                            ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_write                                           ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_field_set                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_7_PM20_80_48_PP_read                                            ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_write                                           ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_field_set                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_8_PM20_80_48_PP_read                                            ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_write                                           ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_field_set                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_9_PM20_80_48_PP_read                                            ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_write                                          ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_field_set                                      ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_10_PM20_80_48_PP_read                                           ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_write                                          ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_field_set                                      ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_11_PM20_80_48_PP_read                                           ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_write     ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_field_set ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_LW20_80_16_PM20_80_48_PP_LW20_80_49_TR_BR2_LW20_80_46_read      ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_write                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_field_set                                   ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_49_TR_BR2_read                                        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_write                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_field_set                                   ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_2_LW20_80_49_TR_BR2_read                                        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_write                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_field_set                                   ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_3_LW20_80_49_TR_BR2_read                                        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_write                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_field_set                                   ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_4_LW20_80_49_TR_BR2_read                                        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_write                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_field_set                                   ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_CONFIG_1_LW20_80_46_DP_T16_read                                        ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | void   s16_titan_br2_mdsp_reg_DIAG_CONTROL_write                                                     ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   s16_titan_br2_mdsp_reg_DIAG_CONTROL_field_set                                                 ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 s16_titan_br2_mdsp_reg_DIAG_CONTROL_read                                                      ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 s16_titan_br2_mdsp_reg_PRE_PROCESS_DATA_CAPTURED_0_read                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 s16_titan_br2_mdsp_reg_DFE_STATE_read                                                         ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 s16_titan_br2_mdsp_reg_TCB_STATE_0_read                                                       ( s16_titan_br2_mdsp_buffer_t *b_ptr, s16_handle_t *h_ptr, UINT32 A )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _S16_TITAN_BR2_MDSP_IO_H */
