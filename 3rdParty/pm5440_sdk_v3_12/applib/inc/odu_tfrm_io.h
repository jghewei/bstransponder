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
 *     and register accessor functions for the odu_tfrm block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing odu_tfrm io functions is:
 | {
 |     odu_tfrm_buffer_t b_ptr[1];
 |     odu_tfrm_buffer_init( b_ptr, h_ptr );
 |     value = odu_tfrm_field_<FIELD>_get( b_ptr, h_ptr );
 |     odu_tfrm_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     odu_tfrm_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = odu_tfrm_field_<FIELD>_get( NULL, h_ptr );
 | odu_tfrm_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------------------+
 | | Register (0x00000000 + (N) * 0x04) TTI_PID |
 | |     N[0..4095]                             |
 | +--------------------------------------------+
 | | bit  31:0 R/W  TTI                         |
 | +--------------------------------------------+
 | 
 | void   odu_tfrm_lfield_TTI_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[4096] )
 | void   odu_tfrm_lfield_TTI_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[4096] )
 | void   odu_tfrm_lfield_range_TTI_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_lfield_range_TTI_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register (0x00004000 + (N) * 0x04) TTI_PM |
 | |     N[0..1535]                            |
 | +-------------------------------------------+
 | | bit  31:0 R/W  TTI_PM                     |
 | +-------------------------------------------+
 | 
 | void   odu_tfrm_lfield_TTI_PM_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[1536] )
 | void   odu_tfrm_lfield_TTI_PM_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[1536] )
 | void   odu_tfrm_lfield_range_TTI_PM_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_lfield_range_TTI_PM_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register (0x00006000 + (N) * 0x04) FTFL |
 | |     N[0..767]                           |
 | +-----------------------------------------+
 | | bit  31:0 R/W  FTFL                     |
 | +-----------------------------------------+
 | 
 | void   odu_tfrm_lfield_FTFL_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[768] )
 | void   odu_tfrm_lfield_FTFL_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[768] )
 | void   odu_tfrm_lfield_range_FTFL_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_lfield_range_FTFL_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x00007000 + (N) * 0x04) APS_PCC |
 | |     N[0..767]                              |
 | +--------------------------------------------+
 | | bit  31:0 R/W  APS                         |
 | +--------------------------------------------+
 | 
 | void   odu_tfrm_lfield_APS_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[768] )
 | void   odu_tfrm_lfield_APS_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[768] )
 | void   odu_tfrm_lfield_range_APS_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_lfield_range_APS_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register (0x00007c00 + (N) * 0x40) DM_PID |
 | |     N[0..255]                             |
 | +-------------------------------------------+
 | | bit  26:24 R/W  DM_STATE                  |
 | | bit  19:0  R/W  DM_COUNT                  |
 | +-------------------------------------------+
 | 
 | void   odu_tfrm_field_DM_STATE_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_DM_STATE_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_DM_STATE_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_DM_STATE_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_tfrm_field_DM_COUNT_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_DM_COUNT_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_DM_COUNT_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_DM_COUNT_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register (0x00007c04 + (N) * 0x40) TCM_CTRL1 |
 | |     N[0..255]                                |
 | +----------------------------------------------+
 | | bit  2:0 R/W  STAT                           |
 | +----------------------------------------------+
 | 
 | void   odu_tfrm_field_STAT_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_STAT_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_STAT_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_STAT_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register (0x00007c08 + (N) * 0x40) TCM_CTRL2 |
 | |     N[0..255]                                |
 | +----------------------------------------------+
 | | bit  1 R/W  DMT_SRC                          |
 | | bit  0 R/W  DMT_VAL                          |
 | +----------------------------------------------+
 | 
 | void   odu_tfrm_field_DMT_SRC_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_DMT_SRC_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_DMT_VAL_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_DMT_VAL_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------------+
 | | Register (0x00007c0c + (N) * 0x40) TCM_CTRL3 |
 | |     N[0..255]                                |
 | +----------------------------------------------+
 | | bit  7:4 R/W  BEI_BIAE                       |
 | | bit  3   R/W  BDI                            |
 | | bit  2   R/W  TTI_OS_INS                     |
 | | bit  1   R/W  TTI_DAPI_INS                   |
 | | bit  0   R/W  TTI_SAPI_INS                   |
 | +----------------------------------------------+
 | 
 | void   odu_tfrm_field_BEI_BIAE_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_BEI_BIAE_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_BEI_BIAE_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_BEI_BIAE_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_tfrm_field_BDI_set            ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_BDI_get            ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_TTI_OS_INS_set     ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TTI_OS_INS_get     ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_TTI_DAPI_INS_set   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TTI_DAPI_INS_get   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_TTI_SAPI_INS_set   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TTI_SAPI_INS_get   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | +------------------------------------------+
 | | Register (0x0000bc00 + (N) * 0x40) DM_PM |
 | |     N[0..95]                             |
 | +------------------------------------------+
 | | bit  26:24 R/W  DM_STATE_PM              |
 | | bit  19:0  R/W  DM_COUNT_PM              |
 | +------------------------------------------+
 | 
 | void   odu_tfrm_field_DM_STATE_PM_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_DM_STATE_PM_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_DM_STATE_PM_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_DM_STATE_PM_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_tfrm_field_DM_COUNT_PM_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_DM_COUNT_PM_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_DM_COUNT_PM_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_DM_COUNT_PM_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register (0x0000bc04 + (N) * 0x40) PM_CTRL1 |
 | |     N[0..95]                                |
 | +---------------------------------------------+
 | | bit  2:0 R/W  STAT_PM                       |
 | +---------------------------------------------+
 | 
 | void   odu_tfrm_field_STAT_PM_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_STAT_PM_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_STAT_PM_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_STAT_PM_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x0000bc08 + (N) * 0x40) ROW2_OH |
 | |     N[0..95]                               |
 | +--------------------------------------------+
 | | bit  24    R/W  PM_TCM_RES                 |
 | | bit  23:16 R/W  TCM_ACT                    |
 | | bit  15:0  R/W  RES1                       |
 | +--------------------------------------------+
 | 
 | void   odu_tfrm_field_PM_TCM_RES_set    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_PM_TCM_RES_get    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_TCM_ACT_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TCM_ACT_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_TCM_ACT_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_TCM_ACT_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_tfrm_field_RES1_set          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_RES1_get          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_RES1_set    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_RES1_get    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register (0x0000bc0c + (N) * 0x40) ROW3_4_OH |
 | |     N[0..95]                                 |
 | +----------------------------------------------+
 | | bit  31:16 R/W  RES2                         |
 | | bit  15:0  R/W  EXP                          |
 | +----------------------------------------------+
 | 
 | void   odu_tfrm_field_RES2_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_RES2_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_RES2_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_RES2_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_tfrm_field_EXP_set        ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_EXP_get        ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_EXP_set  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_EXP_get  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register (0x0000bc10 + (N) * 0x40) ROW4_OH0 |
 | |     N[0..95]                                |
 | +---------------------------------------------+
 | | bit  31:0 R/W  RES3                         |
 | +---------------------------------------------+
 | 
 | void   odu_tfrm_field_RES3_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_RES3_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_RES3_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_RES3_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register (0x0000bc14 + (N) * 0x40) ROW4_OH1 |
 | |     N[0..95]                                |
 | +---------------------------------------------+
 | | bit  31:16 R/W  GCC2                        |
 | | bit  15:0  R/W  GCC1                        |
 | +---------------------------------------------+
 | 
 | void   odu_tfrm_field_GCC2_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_GCC2_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_GCC2_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_GCC2_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_tfrm_field_GCC1_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_GCC1_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_GCC1_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_GCC1_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x0000bc18 + (N) * 0x40) TCM1_POOL_ID |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  7:0 R/W  TCM1_POOL_ID                      |
 | +-------------------------------------------------+
 | 
 | void   odu_tfrm_field_TCM1_POOL_ID_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TCM1_POOL_ID_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_TCM1_POOL_ID_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_TCM1_POOL_ID_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x0000bc1c + (N) * 0x40) TCM2_POOL_ID |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  7:0 R/W  TCM2_POOL_ID                      |
 | +-------------------------------------------------+
 | 
 | void   odu_tfrm_field_TCM2_POOL_ID_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TCM2_POOL_ID_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_TCM2_POOL_ID_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_TCM2_POOL_ID_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x0000bc20 + (N) * 0x40) TCM3_POOL_ID |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  7:0 R/W  TCM3_POOL_ID                      |
 | +-------------------------------------------------+
 | 
 | void   odu_tfrm_field_TCM3_POOL_ID_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TCM3_POOL_ID_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_TCM3_POOL_ID_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_TCM3_POOL_ID_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x0000bc24 + (N) * 0x40) TCM4_POOL_ID |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  7:0 R/W  TCM4_POOL_ID                      |
 | +-------------------------------------------------+
 | 
 | void   odu_tfrm_field_TCM4_POOL_ID_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TCM4_POOL_ID_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_TCM4_POOL_ID_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_TCM4_POOL_ID_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x0000bc28 + (N) * 0x40) TCM5_POOL_ID |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  7:0 R/W  TCM5_POOL_ID                      |
 | +-------------------------------------------------+
 | 
 | void   odu_tfrm_field_TCM5_POOL_ID_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TCM5_POOL_ID_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_TCM5_POOL_ID_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_TCM5_POOL_ID_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x0000bc2c + (N) * 0x40) TCM6_POOL_ID |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  7:0 R/W  TCM6_POOL_ID                      |
 | +-------------------------------------------------+
 | 
 | void   odu_tfrm_field_TCM6_POOL_ID_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TCM6_POOL_ID_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_TCM6_POOL_ID_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_TCM6_POOL_ID_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register (0x0000bc30 + (N) * 0x40) OH_INSERT |
 | |     N[0..95]                                 |
 | +----------------------------------------------+
 | | bit  18   R/W  RCP_EN                        |
 | | bit  17   R/W  GCC1_INS                      |
 | | bit  16   R/W  GCC2_INS                      |
 | | bit  15:8 R/W  APS_PCC_INS                   |
 | | bit  6    R/W  RES2_INS                      |
 | | bit  5    R/W  EXP_INS                       |
 | | bit  4    R/W  RES1_INS                      |
 | | bit  3    R/W  PM_TCM_RES_INS                |
 | | bit  2    R/W  TCM_ACT_INS                   |
 | | bit  1:0  R/W  FTFL_INS                      |
 | +----------------------------------------------+
 | 
 | void   odu_tfrm_field_RCP_EN_set            ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_RCP_EN_get            ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_GCC1_INS_set          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_GCC1_INS_get          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_GCC2_INS_set          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_GCC2_INS_get          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_APS_PCC_INS_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_APS_PCC_INS_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_APS_PCC_INS_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_APS_PCC_INS_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_tfrm_field_RES2_INS_set          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_RES2_INS_get          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_EXP_INS_set           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_EXP_INS_get           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_RES1_INS_set          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_RES1_INS_get          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_PM_TCM_RES_INS_set    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_PM_TCM_RES_INS_get    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_TCM_ACT_INS_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TCM_ACT_INS_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_FTFL_INS_set          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_FTFL_INS_get          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_FTFL_INS_set    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_FTFL_INS_get    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x0000bc34 + (N) * 0x40) CHANNEL_CTRL |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  28    R/W  IAE_INS                         |
 | | bit  26    R/W  TTI_OS_INS_PM                   |
 | | bit  25    R/W  TTI_DAPI_INS_PM                 |
 | | bit  24    R/W  TTI_SAPI_INS_PM                 |
 | | bit  23:20 R/W  BEI_PM                          |
 | | bit  19    R/W  BDI_PM                          |
 | | bit  17    R/W  SF_SD_PROP                      |
 | | bit  16    R/W  SSF_PROP                        |
 | | bit  13    R/W  TCM6_TRANSPARENT                |
 | | bit  12    R/W  TCM5_TRANSPARENT                |
 | | bit  11    R/W  TCM4_TRANSPARENT                |
 | | bit  10    R/W  TCM3_TRANSPARENT                |
 | | bit  9     R/W  TCM2_TRANSPARENT                |
 | | bit  8     R/W  TCM1_TRANSPARENT                |
 | | bit  7     R/W  PM_TRANSPARENT                  |
 | | bit  5:4   R/W  MAINT_SIG                       |
 | | bit  1     R/W  DMP_SRC                         |
 | | bit  0     R/W  DMP_VAL                         |
 | +-------------------------------------------------+
 | 
 | void   odu_tfrm_field_IAE_INS_set          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_IAE_INS_get          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_TTI_OS_INS_PM_set    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TTI_OS_INS_PM_get    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_TTI_DAPI_INS_PM_set  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TTI_DAPI_INS_PM_get  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_TTI_SAPI_INS_PM_set  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TTI_SAPI_INS_PM_get  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_BEI_PM_set           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_BEI_PM_get           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_BEI_PM_set     ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_BEI_PM_get     ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_tfrm_field_BDI_PM_set           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_BDI_PM_get           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_SF_SD_PROP_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_SF_SD_PROP_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_SSF_PROP_set         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_SSF_PROP_get         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_TCM6_TRANSPARENT_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TCM6_TRANSPARENT_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_TCM5_TRANSPARENT_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TCM5_TRANSPARENT_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_TCM4_TRANSPARENT_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TCM4_TRANSPARENT_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_TCM3_TRANSPARENT_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TCM3_TRANSPARENT_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_TCM2_TRANSPARENT_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TCM2_TRANSPARENT_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_TCM1_TRANSPARENT_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_TCM1_TRANSPARENT_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_PM_TRANSPARENT_set   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_PM_TRANSPARENT_get   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_MAINT_SIG_set        ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_MAINT_SIG_get        ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_range_MAINT_SIG_set  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_field_range_MAINT_SIG_get  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_tfrm_field_DMP_SRC_set          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_DMP_SRC_get          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_tfrm_field_DMP_VAL_set          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_tfrm_field_DMP_VAL_get          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------+
 | | Register 0x0000d500 CHANNEL_PASSTHRU0 |
 | +---------------------------------------+
 | | bit  31:0 R/W  PASSTHRU               |
 | +---------------------------------------+
 | 
 | void   odu_tfrm_lfield_PASSTHRU_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_tfrm_lfield_PASSTHRU_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_tfrm_lfield_range_PASSTHRU_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_lfield_range_PASSTHRU_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x0000d50c CHANNEL_INT_STAT0_IAE_I |
 | +---------------------------------------------+
 | | bit  31:0 R/W  IAE_I                        |
 | +---------------------------------------------+
 | 
 | void                 odu_tfrm_lfield_IAE_I_set_to_clear       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_tfrm_lfield_IAE_I_get                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_tfrm_lfield_range_IAE_I_set_to_clear ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               odu_tfrm_lfield_range_IAE_I_get          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE odu_tfrm_lfield_range_IAE_I_poll         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE odu_tfrm_lfield_IAE_I_poll               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------------+
 | | Register 0x0000d518 CHANNEL_INT_STAT3_DM_PM_I |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  DM_PM_I                        |
 | +-----------------------------------------------+
 | 
 | void                 odu_tfrm_lfield_DM_PM_I_set_to_clear       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_tfrm_lfield_DM_PM_I_get                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_tfrm_lfield_range_DM_PM_I_set_to_clear ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               odu_tfrm_lfield_range_DM_PM_I_get          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE odu_tfrm_lfield_range_DM_PM_I_poll         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE odu_tfrm_lfield_DM_PM_I_poll               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------------+
 | | Register 0x0000d524 POOL_INT_STAT0_DM_PID_I |
 | +---------------------------------------------+
 | | bit  31:0 R/W  DM_PID_I                     |
 | +---------------------------------------------+
 | 
 | void                 odu_tfrm_lfield_DM_PID_I_set_to_clear       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[8] )
 | void                 odu_tfrm_lfield_DM_PID_I_get                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[8] )
 | void                 odu_tfrm_lfield_range_DM_PID_I_set_to_clear ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               odu_tfrm_lfield_range_DM_PID_I_get          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE odu_tfrm_lfield_range_DM_PID_I_poll         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE odu_tfrm_lfield_DM_PID_I_poll               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[8], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------------------+
 | | Register 0x0000d54c FIFO_INT_STAT_DM_FIFO_OVR_UDR_I |
 | +-----------------------------------------------------+
 | | bit  1 R/W  DM_FIFO_UDR_I                           |
 | | bit  0 R/W  DM_FIFO_OVR_I                           |
 | +-----------------------------------------------------+
 | 
 | void                 odu_tfrm_field_DM_FIFO_UDR_I_set_to_clear ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_tfrm_field_DM_FIFO_UDR_I_get          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_field_DM_FIFO_UDR_I_poll         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_tfrm_field_DM_FIFO_OVR_I_set_to_clear ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_tfrm_field_DM_FIFO_OVR_I_get          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_field_DM_FIFO_OVR_I_poll         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x0000d64c CHANNEL_INT_VAL0_IAE_V |
 | +--------------------------------------------+
 | | bit  31:0 R  IAE_V                         |
 | +--------------------------------------------+
 | 
 | void                 odu_tfrm_lfield_IAE_V_get        ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[3] )
 | UINT32               odu_tfrm_lfield_range_IAE_V_get  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE odu_tfrm_lfield_range_IAE_V_poll ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE odu_tfrm_lfield_IAE_V_poll       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x0000d658 CHANNEL_INT_EN0_IAE_E |
 | +-------------------------------------------+
 | | bit  31:0 R/W  IAE_E                      |
 | +-------------------------------------------+
 | 
 | void   odu_tfrm_lfield_IAE_E_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_tfrm_lfield_IAE_E_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_tfrm_lfield_range_IAE_E_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_lfield_range_IAE_E_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x0000d664 CHANNEL_INT_EN3_DM_PM_E |
 | +---------------------------------------------+
 | | bit  31:0 R/W  DM_PM_E                      |
 | +---------------------------------------------+
 | 
 | void   odu_tfrm_lfield_DM_PM_E_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_tfrm_lfield_DM_PM_E_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_tfrm_lfield_range_DM_PM_E_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_lfield_range_DM_PM_E_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0000d670 POOL_INT_EN0_DM_PID_E |
 | +-------------------------------------------+
 | | bit  31:0 R/W  DM_PID_E                   |
 | +-------------------------------------------+
 | 
 | void   odu_tfrm_lfield_DM_PID_E_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[8] )
 | void   odu_tfrm_lfield_DM_PID_E_get       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value[8] )
 | void   odu_tfrm_lfield_range_DM_PID_E_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_tfrm_lfield_range_DM_PID_E_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x0000d698 FIFO_INT_EN_DM_FIFO_OVR_UDR_E |
 | +---------------------------------------------------+
 | | bit  1 R/W  DM_FIFO_UDR_E                         |
 | | bit  0 R/W  DM_FIFO_OVR_E                         |
 | +---------------------------------------------------+
 | 
 | void   odu_tfrm_field_DM_FIFO_UDR_E_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_tfrm_field_DM_FIFO_UDR_E_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | void   odu_tfrm_field_DM_FIFO_OVR_E_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_tfrm_field_DM_FIFO_OVR_E_get ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ODU_TFRM_IO_H
#define _ODU_TFRM_IO_H

#include "odu_tfrm_loc.h"
#include "odu_tfrm_regs.h"
#include "odu_tfrm_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 odu_tfrm_buffer_init               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | void                 odu_tfrm_buffer_flush              ( odu_tfrm_buffer_t *b_ptr )
 | UINT32               odu_tfrm_reg_read                  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 odu_tfrm_reg_write                 ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 odu_tfrm_field_set                 ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 odu_tfrm_action_on_write_field_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_tfrm_burst_read                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 odu_tfrm_burst_write               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE odu_tfrm_poll                      ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 odu_tfrm_reg_CHANNEL_PASSTHRU0_write                                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_PASSTHRU0_field_set                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_PASSTHRU0_read                                    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_CHANNEL_PASSTHRU1_write                                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_PASSTHRU1_field_set                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_PASSTHRU1_read                                    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_CHANNEL_PASSTHRU2_write                                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_PASSTHRU2_field_set                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_PASSTHRU2_read                                    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_TCM_CTRL2_array_write                                     ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_TCM_CTRL2_array_field_set                                 ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_TCM_CTRL2_array_read                                      ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_write                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_field_set                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_INT_EN0_IAE_E_read                                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_write                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_field_set                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_INT_EN1_IAE_E_read                                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_write                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_field_set                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_INT_EN2_IAE_E_read                                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_write                             ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_field_set                         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_INT_EN3_DM_PM_E_read                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_write                             ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_field_set                         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_INT_EN4_DM_PM_E_read                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_write                             ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_field_set                         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_INT_EN5_DM_PM_E_read                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_write                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_field_set                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_EN0_DM_PID_E_read                                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_write                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_field_set                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_EN1_DM_PID_E_read                                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_write                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_field_set                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_EN2_DM_PID_E_read                                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_write                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_field_set                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_EN3_DM_PID_E_read                                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_write                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_field_set                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_EN4_DM_PID_E_read                                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_write                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_field_set                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_EN5_DM_PID_E_read                                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_write                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_field_set                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_EN6_DM_PID_E_read                                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_write                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_field_set                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_EN7_DM_PID_E_read                                ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_write                       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_field_set                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_FIFO_INT_EN_DM_FIFO_OVR_UDR_E_read                        ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | 
 | void                 odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_write                             ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_action_on_write_field_set         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_read                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT0_IAE_I_poll                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_write                             ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_action_on_write_field_set         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_read                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT1_IAE_I_poll                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_write                             ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_action_on_write_field_set         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_read                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT2_IAE_I_poll                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_write                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_action_on_write_field_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_read                            ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT3_DM_PM_I_poll                            ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_write                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_action_on_write_field_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_read                            ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT4_DM_PM_I_poll                            ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_write                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_action_on_write_field_set       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_read                            ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_STAT5_DM_PM_I_poll                            ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_write                             ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_action_on_write_field_set         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_read                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT0_DM_PID_I_poll                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_write                             ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_action_on_write_field_set         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_read                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT1_DM_PID_I_poll                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_write                             ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_action_on_write_field_set         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_read                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT2_DM_PID_I_poll                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_write                             ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_action_on_write_field_set         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_read                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT3_DM_PID_I_poll                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_write                             ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_action_on_write_field_set         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_read                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT4_DM_PID_I_poll                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_write                             ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_action_on_write_field_set         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_read                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT5_DM_PID_I_poll                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_write                             ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_action_on_write_field_set         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_read                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT6_DM_PID_I_poll                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_write                             ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_action_on_write_field_set         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_read                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_POOL_INT_STAT7_DM_PID_I_poll                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_write                     ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_action_on_write_field_set ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_read                      ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_FIFO_INT_STAT_DM_FIFO_OVR_UDR_I_poll                      ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               odu_tfrm_reg_CHANNEL_INT_VAL0_IAE_V_read                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_VAL0_IAE_V_poll                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               odu_tfrm_reg_CHANNEL_INT_VAL1_IAE_V_read                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_VAL1_IAE_V_poll                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               odu_tfrm_reg_CHANNEL_INT_VAL2_IAE_V_read                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_tfrm_reg_CHANNEL_INT_VAL2_IAE_V_poll                               ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_tfrm_reg_TTI_PM_array_burst_write                                  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_tfrm_reg_TTI_PM_array_field_set                                    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_tfrm_reg_TTI_PM_array_burst_read                                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_tfrm_reg_TTI_PM_array_read                                         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_tfrm_reg_TTI_PID_array_burst_write                                 ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_tfrm_reg_TTI_PID_array_field_set                                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_tfrm_reg_TTI_PID_array_burst_read                                  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_tfrm_reg_TTI_PID_array_read                                        ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_tfrm_reg_FTFL_array_burst_write                                    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_tfrm_reg_FTFL_array_field_set                                      ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_tfrm_reg_FTFL_array_burst_read                                     ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_tfrm_reg_FTFL_array_read                                           ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_tfrm_reg_APS_PCC_array_burst_write                                 ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_tfrm_reg_APS_PCC_array_field_set                                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_tfrm_reg_APS_PCC_array_burst_read                                  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_tfrm_reg_APS_PCC_array_read                                        ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_tfrm_reg_DM_PID_array_write                                        ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_DM_PID_array_field_set                                    ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_DM_PID_array_read                                         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_TCM_CTRL1_array_write                                     ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_TCM_CTRL1_array_field_set                                 ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_TCM_CTRL1_array_read                                      ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_TCM_CTRL3_array_write                                     ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_TCM_CTRL3_array_field_set                                 ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_TCM_CTRL3_array_read                                      ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_DM_PM_array_write                                         ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_DM_PM_array_field_set                                     ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_DM_PM_array_read                                          ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_PM_CTRL1_array_write                                      ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_PM_CTRL1_array_field_set                                  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_PM_CTRL1_array_read                                       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_ROW2_OH_array_write                                       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_ROW2_OH_array_field_set                                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_ROW2_OH_array_read                                        ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_ROW3_4_OH_array_write                                     ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_ROW3_4_OH_array_field_set                                 ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_ROW3_4_OH_array_read                                      ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_ROW4_OH0_array_write                                      ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_ROW4_OH0_array_field_set                                  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_ROW4_OH0_array_read                                       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_ROW4_OH1_array_write                                      ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_ROW4_OH1_array_field_set                                  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_ROW4_OH1_array_read                                       ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_TCM1_POOL_ID_array_write                                  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_TCM1_POOL_ID_array_field_set                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_TCM1_POOL_ID_array_read                                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_TCM2_POOL_ID_array_write                                  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_TCM2_POOL_ID_array_field_set                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_TCM2_POOL_ID_array_read                                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_TCM3_POOL_ID_array_write                                  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_TCM3_POOL_ID_array_field_set                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_TCM3_POOL_ID_array_read                                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_TCM4_POOL_ID_array_write                                  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_TCM4_POOL_ID_array_field_set                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_TCM4_POOL_ID_array_read                                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_TCM5_POOL_ID_array_write                                  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_TCM5_POOL_ID_array_field_set                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_TCM5_POOL_ID_array_read                                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_TCM6_POOL_ID_array_write                                  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_TCM6_POOL_ID_array_field_set                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_TCM6_POOL_ID_array_read                                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_OH_INSERT_array_write                                     ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_OH_INSERT_array_field_set                                 ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_OH_INSERT_array_read                                      ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_tfrm_reg_CHANNEL_CTRL_array_write                                  ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_tfrm_reg_CHANNEL_CTRL_array_field_set                              ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_tfrm_reg_CHANNEL_CTRL_array_read                                   ( odu_tfrm_buffer_t *b_ptr, odu_tfrm_handle_t *h_ptr, UINT32  N )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ODU_TFRM_IO_H */
