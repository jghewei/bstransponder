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
 *     and register accessor functions for the odu_rfrm block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing odu_rfrm io functions is:
 | {
 |     odu_rfrm_buffer_t b_ptr[1];
 |     odu_rfrm_buffer_init( b_ptr, h_ptr );
 |     value = odu_rfrm_field_<FIELD>_get( b_ptr, h_ptr );
 |     odu_rfrm_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     odu_rfrm_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = odu_rfrm_field_<FIELD>_get( NULL, h_ptr );
 | odu_rfrm_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------------+
 | | Register (0x00000000 + (N) * 0x04) AC_TTI_PID |
 | |     N[0..4095]                                |
 | +-----------------------------------------------+
 | | bit  31:0 R  AC_TTI                           |
 | +-----------------------------------------------+
 | 
 | void   odu_rfrm_lfield_AC_TTI_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[4096] )
 | UINT32 odu_rfrm_lfield_range_AC_TTI_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register (0x00004000 + (N) * 0x04) AC_TTI_PM |
 | |     N[0..1535]                               |
 | +----------------------------------------------+
 | | bit  31:0 R  AC_TTI_PM                       |
 | +----------------------------------------------+
 | 
 | void   odu_rfrm_lfield_AC_TTI_PM_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[1536] )
 | UINT32 odu_rfrm_lfield_range_AC_TTI_PM_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00008000 + (N) * 0x04) EXP_TTI_PID |
 | |     N[0..2047]                                 |
 | +------------------------------------------------+
 | | bit  31:0 R/W  EX_TTI                          |
 | +------------------------------------------------+
 | 
 | void   odu_rfrm_lfield_EX_TTI_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[2048] )
 | void   odu_rfrm_lfield_EX_TTI_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[2048] )
 | void   odu_rfrm_lfield_range_EX_TTI_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_lfield_range_EX_TTI_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x0000a000 + (N) * 0x04) EXP_TTI_PM |
 | |     N[0..767]                                 |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  EX_TTI_PM                      |
 | +-----------------------------------------------+
 | 
 | void   odu_rfrm_lfield_EX_TTI_PM_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[768] )
 | void   odu_rfrm_lfield_EX_TTI_PM_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[768] )
 | void   odu_rfrm_lfield_range_EX_TTI_PM_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_lfield_range_EX_TTI_PM_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register (0x0000b000 + (N) * 0x04) FTFL |
 | |     N[0..767]                           |
 | +-----------------------------------------+
 | | bit  31:0 R  FTFL                       |
 | +-----------------------------------------+
 | 
 | void   odu_rfrm_lfield_FTFL_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[768] )
 | UINT32 odu_rfrm_lfield_range_FTFL_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x0000c000 + (N) * 0x04) AC_APS_PCC |
 | |     N[0..767]                                 |
 | +-----------------------------------------------+
 | | bit  31:0 R  AC_APS                           |
 | +-----------------------------------------------+
 | 
 | void   odu_rfrm_lfield_AC_APS_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[768] )
 | UINT32 odu_rfrm_lfield_range_AC_APS_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x0000cc00 + (N) * 0x40) LAYER_CTRL0_PID |
 | |     N[0..255]                                      |
 | +----------------------------------------------------+
 | | bit  27    R/W  LTC_ACT_DIS                        |
 | | bit  26    R/W  TIM_ACT_DIS                        |
 | | bit  25:24 R/W  TIM_DET_MO                         |
 | | bit  23:20 R/W  DEG_M                              |
 | | bit  19:0  R/W  DEG_THR                            |
 | +----------------------------------------------------+
 | 
 | void   odu_rfrm_field_LTC_ACT_DIS_set      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_LTC_ACT_DIS_get      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TIM_ACT_DIS_set      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TIM_ACT_DIS_get      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TIM_DET_MO_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TIM_DET_MO_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_TIM_DET_MO_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_TIM_DET_MO_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_rfrm_field_DEG_M_set            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_DEG_M_get            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_DEG_M_set      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_DEG_M_get      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_rfrm_field_DEG_THR_set          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_DEG_THR_get          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_DEG_THR_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_DEG_THR_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x0000cc04 + (N) * 0x40) LAYER_CTRL1_PID |
 | |     N[0..255]                                      |
 | +----------------------------------------------------+
 | | bit  20    R/W  INT_EN                             |
 | | bit  19    R/W  AIS_DIS                            |
 | | bit  18    R/W  BW_DIS                             |
 | | bit  17    R/W  TSD_DIS                            |
 | | bit  16    R/W  TSF_DIS                            |
 | | bit  15:14 R/W  MODE                               |
 | | bit  13:12 R/W  TCM_MAINT_SIG                      |
 | | bit  8     R/W  PARENT_IS_DCI                      |
 | | bit  7:0   R/W  PARENT_POOL_ID                     |
 | +----------------------------------------------------+
 | 
 | void   odu_rfrm_field_INT_EN_set               ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_INT_EN_get               ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_AIS_DIS_set              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_AIS_DIS_get              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_BW_DIS_set               ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_BW_DIS_get               ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TSD_DIS_set              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TSD_DIS_get              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TSF_DIS_set              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TSF_DIS_get              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_MODE_set                 ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_MODE_get                 ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_MODE_set           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_MODE_get           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_rfrm_field_TCM_MAINT_SIG_set        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM_MAINT_SIG_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_TCM_MAINT_SIG_set  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_TCM_MAINT_SIG_get  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_rfrm_field_PARENT_IS_DCI_set        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_PARENT_IS_DCI_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_PARENT_POOL_ID_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_PARENT_POOL_ID_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_PARENT_POOL_ID_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_PARENT_POOL_ID_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x0000cc20 + (N) * 0x40) NEAR_END_PMON_PID |
 | |     N[0..255]                                        |
 | +------------------------------------------------------+
 | | bit  21   R  PIAE                                    |
 | | bit  20   R  PN_DS                                   |
 | | bit  19:0 R  PN_EBC                                  |
 | +------------------------------------------------------+
 | 
 | UINT32 odu_rfrm_field_PIAE_get         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_PN_DS_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_PN_EBC_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_PN_EBC_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x0000cc24 + (N) * 0x40) FAR_END_PMON_PID |
 | |     N[0..255]                                       |
 | +-----------------------------------------------------+
 | | bit  21   R  PBIAE                                  |
 | | bit  20   R  PF_DS                                  |
 | | bit  19:0 R  PF_EBC                                 |
 | +-----------------------------------------------------+
 | 
 | UINT32 odu_rfrm_field_PBIAE_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_PF_DS_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_PF_EBC_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_PF_EBC_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x0000cc28 + (N) * 0x40) DEFECTS_ACTIONS_PID |
 | |     N[0..255]                                          |
 | +--------------------------------------------------------+
 | | bit  31 R  ABDI_V                                      |
 | | bit  30 R  AAIS_V                                      |
 | | bit  29 R  ATSD_V                                      |
 | | bit  28 R  ATSF_V                                      |
 | | bit  25 R  CI_SSF_V                                    |
 | | bit  24 R  DIAE_V                                      |
 | | bit  23 R  DLTC_V                                      |
 | | bit  22 R  DBIAE_V                                     |
 | | bit  21 R  DBDI_V                                      |
 | | bit  20 R  DDEG_V                                      |
 | | bit  19 R  DTIM_V                                      |
 | | bit  18 R  DAIS_V                                      |
 | | bit  17 R  DOCI_V                                      |
 | | bit  16 R  DLCK_V                                      |
 | | bit  15 R  ABDI_I                                      |
 | | bit  14 R  AAIS_I                                      |
 | | bit  13 R  ATSD_I                                      |
 | | bit  12 R  ATSF_I                                      |
 | | bit  8  R  DIAE_I                                      |
 | | bit  7  R  DLTC_I                                      |
 | | bit  6  R  DBIAE_I                                     |
 | | bit  5  R  DBDI_I                                      |
 | | bit  4  R  DDEG_I                                      |
 | | bit  3  R  DTIM_I                                      |
 | | bit  2  R  DAIS_I                                      |
 | | bit  1  R  DOCI_I                                      |
 | | bit  0  R  DLCK_I                                      |
 | +--------------------------------------------------------+
 | 
 | UINT32 odu_rfrm_field_ABDI_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_AAIS_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_ATSD_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_ATSF_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_CI_SSF_V_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DIAE_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DLTC_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DBIAE_V_get  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DBDI_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DDEG_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DTIM_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DAIS_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DOCI_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DLCK_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_ABDI_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_AAIS_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_ATSD_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_ATSF_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DIAE_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DLTC_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DBIAE_I_get  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DBDI_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DDEG_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DTIM_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DAIS_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DOCI_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DLCK_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------+
 | | Register (0x0000cc2c + (N) * 0x40) NE_FE_EBC_PID |
 | |     N[0..255]                                    |
 | +--------------------------------------------------+
 | | bit  28:16 R  FE_EBC                             |
 | | bit  12:0  R  NE_EBC                             |
 | +--------------------------------------------------+
 | 
 | UINT32 odu_rfrm_field_FE_EBC_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_FE_EBC_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 odu_rfrm_field_NE_EBC_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_NE_EBC_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x00010c00 + (N) * 0x40) LAYER_CTRL0_PM |
 | |     N[0..95]                                      |
 | +---------------------------------------------------+
 | | bit  26    R/W  TIM_ACT_DIS_PM                    |
 | | bit  25:24 R/W  TIM_DET_MO_PM                     |
 | | bit  23:20 R/W  DEG_M_PM                          |
 | | bit  19:0  R/W  DEG_THR_PM                        |
 | +---------------------------------------------------+
 | 
 | void   odu_rfrm_field_TIM_ACT_DIS_PM_set      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TIM_ACT_DIS_PM_get      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TIM_DET_MO_PM_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TIM_DET_MO_PM_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_TIM_DET_MO_PM_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_TIM_DET_MO_PM_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_rfrm_field_DEG_M_PM_set            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_DEG_M_PM_get            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_DEG_M_PM_set      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_DEG_M_PM_get      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_rfrm_field_DEG_THR_PM_set          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_DEG_THR_PM_get          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_DEG_THR_PM_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_DEG_THR_PM_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x00010c04 + (N) * 0x40) LAYER_CTRL1_PM |
 | |     N[0..95]                                      |
 | +---------------------------------------------------+
 | | bit  20    R/W  INT_EN_PM                         |
 | | bit  18    R/W  BW_DIS_PM                         |
 | | bit  17    R/W  TSD_DIS_PM                        |
 | | bit  16    R/W  TSF_DIS_PM                        |
 | | bit  15:14 R/W  MODE_PM                           |
 | | bit  8     R/W  PARENT_IS_DCI_PM                  |
 | | bit  7:0   R/W  PARENT_POOL_ID_PM                 |
 | +---------------------------------------------------+
 | 
 | void   odu_rfrm_field_INT_EN_PM_set               ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_INT_EN_PM_get               ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_BW_DIS_PM_set               ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_BW_DIS_PM_get               ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TSD_DIS_PM_set              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TSD_DIS_PM_get              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TSF_DIS_PM_set              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TSF_DIS_PM_get              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_MODE_PM_set                 ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_MODE_PM_get                 ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_MODE_PM_set           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_MODE_PM_get           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_rfrm_field_PARENT_IS_DCI_PM_set        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_PARENT_IS_DCI_PM_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_PARENT_POOL_ID_PM_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_PARENT_POOL_ID_PM_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_PARENT_POOL_ID_PM_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_PARENT_POOL_ID_PM_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00010c08 + (N) * 0x40) TCM1_2_3_PID |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  30    R/W  TCM3_WR_EN                      |
 | | bit  29    R/W  TCM2_WR_EN                      |
 | | bit  28    R/W  TCM1_WR_EN                      |
 | | bit  23:16 R/W  TCM3_POOL_ID                    |
 | | bit  15:8  R/W  TCM2_POOL_ID                    |
 | | bit  7:0   R/W  TCM1_POOL_ID                    |
 | +-------------------------------------------------+
 | 
 | void   odu_rfrm_field_TCM3_WR_EN_set         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM3_WR_EN_get         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM2_WR_EN_set         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM2_WR_EN_get         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM1_WR_EN_set         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM1_WR_EN_get         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM3_POOL_ID_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM3_POOL_ID_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_TCM3_POOL_ID_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_TCM3_POOL_ID_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_rfrm_field_TCM2_POOL_ID_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM2_POOL_ID_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_TCM2_POOL_ID_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_TCM2_POOL_ID_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_rfrm_field_TCM1_POOL_ID_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM1_POOL_ID_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_TCM1_POOL_ID_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_TCM1_POOL_ID_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00010c0c + (N) * 0x40) TCM4_5_6_PID |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  30    R/W  TCM6_WR_EN                      |
 | | bit  29    R/W  TCM5_WR_EN                      |
 | | bit  28    R/W  TCM4_WR_EN                      |
 | | bit  23:16 R/W  TCM6_POOL_ID                    |
 | | bit  15:8  R/W  TCM5_POOL_ID                    |
 | | bit  7:0   R/W  TCM4_POOL_ID                    |
 | +-------------------------------------------------+
 | 
 | void   odu_rfrm_field_TCM6_WR_EN_set         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM6_WR_EN_get         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM5_WR_EN_set         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM5_WR_EN_get         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM4_WR_EN_set         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM4_WR_EN_get         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM6_POOL_ID_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM6_POOL_ID_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_TCM6_POOL_ID_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_TCM6_POOL_ID_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_rfrm_field_TCM5_POOL_ID_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM5_POOL_ID_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_TCM5_POOL_ID_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_TCM5_POOL_ID_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_rfrm_field_TCM4_POOL_ID_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM4_POOL_ID_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_TCM4_POOL_ID_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_TCM4_POOL_ID_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00010c10 + (N) * 0x40) FLOATING_PM_NIM_PID |
 | |     N[0..95]                                           |
 | +--------------------------------------------------------+
 | | bit  29   R/W  FLOATING_TCM_WR_EN                      |
 | | bit  28   R/W  PM_NIM_WR_EN                            |
 | | bit  15:8 R/W  FLOATING_TCM_POOL_ID                    |
 | | bit  7:0  R/W  PM_NIM_POOL_ID                          |
 | +--------------------------------------------------------+
 | 
 | void   odu_rfrm_field_FLOATING_TCM_WR_EN_set         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_FLOATING_TCM_WR_EN_get         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_PM_NIM_WR_EN_set               ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_PM_NIM_WR_EN_get               ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_FLOATING_TCM_POOL_ID_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_FLOATING_TCM_POOL_ID_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_FLOATING_TCM_POOL_ID_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_FLOATING_TCM_POOL_ID_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_rfrm_field_PM_NIM_POOL_ID_set             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_PM_NIM_POOL_ID_get             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_PM_NIM_POOL_ID_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_PM_NIM_POOL_ID_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00010c14 + (N) * 0x40) CHANNEL_CTRL0 |
 | |     N[0..95]                                     |
 | +--------------------------------------------------+
 | | bit  25   R/W  TCM6_REMOVE_OH                    |
 | | bit  24   R/W  TCM5_REMOVE_OH                    |
 | | bit  23   R/W  TCM4_REMOVE_OH                    |
 | | bit  22   R/W  TCM3_REMOVE_OH                    |
 | | bit  21   R/W  TCM2_REMOVE_OH                    |
 | | bit  20   R/W  TCM1_REMOVE_OH                    |
 | | bit  18   R/W  PM_NIM_TRANSPARENT                |
 | | bit  17   R/W  TCM6_TRANSPARENT                  |
 | | bit  16   R/W  TCM5_TRANSPARENT                  |
 | | bit  15   R/W  TCM4_TRANSPARENT                  |
 | | bit  14   R/W  TCM3_TRANSPARENT                  |
 | | bit  13   R/W  TCM2_TRANSPARENT                  |
 | | bit  12   R/W  TCM1_TRANSPARENT                  |
 | | bit  10:8 R/W  FLOATING_TCM_ID                   |
 | | bit  3    R/W  LAST_IS_DCI                       |
 | | bit  2:0  R/W  LAST_LAYER                        |
 | +--------------------------------------------------+
 | 
 | void   odu_rfrm_field_TCM6_REMOVE_OH_set        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM6_REMOVE_OH_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM5_REMOVE_OH_set        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM5_REMOVE_OH_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM4_REMOVE_OH_set        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM4_REMOVE_OH_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM3_REMOVE_OH_set        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM3_REMOVE_OH_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM2_REMOVE_OH_set        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM2_REMOVE_OH_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM1_REMOVE_OH_set        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM1_REMOVE_OH_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_PM_NIM_TRANSPARENT_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_PM_NIM_TRANSPARENT_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM6_TRANSPARENT_set      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM6_TRANSPARENT_get      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM5_TRANSPARENT_set      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM5_TRANSPARENT_get      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM4_TRANSPARENT_set      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM4_TRANSPARENT_get      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM3_TRANSPARENT_set      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM3_TRANSPARENT_get      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM2_TRANSPARENT_set      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM2_TRANSPARENT_get      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_TCM1_TRANSPARENT_set      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_TCM1_TRANSPARENT_get      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_FLOATING_TCM_ID_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_FLOATING_TCM_ID_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_FLOATING_TCM_ID_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_FLOATING_TCM_ID_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_rfrm_field_LAST_IS_DCI_set           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_LAST_IS_DCI_get           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_LAST_LAYER_set            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_LAST_LAYER_get            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_LAST_LAYER_set      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_LAST_LAYER_get      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00010c18 + (N) * 0x40) CHANNEL_CTRL1 |
 | |     N[0..95]                                     |
 | +--------------------------------------------------+
 | | bit  4   R/W  LOFLOM_ACT_DIS                     |
 | | bit  3   R/W  FAU_PASSTHRU                       |
 | | bit  2:1 R/W  MAINT_SIG                          |
 | | bit  0   R/W  PASSTHRU                           |
 | +--------------------------------------------------+
 | 
 | void   odu_rfrm_field_LOFLOM_ACT_DIS_set  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_LOFLOM_ACT_DIS_get  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_FAU_PASSTHRU_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_FAU_PASSTHRU_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_MAINT_SIG_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_MAINT_SIG_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | void   odu_rfrm_field_range_MAINT_SIG_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_field_range_MAINT_SIG_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   odu_rfrm_field_PASSTHRU_set        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 odu_rfrm_field_PASSTHRU_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------+
 | | Register (0x00010c1c + (N) * 0x40) ROW4_OH1 |
 | |     N[0..95]                                |
 | +---------------------------------------------+
 | | bit  31:16 R  GCC2                          |
 | | bit  15:0  R  GCC1                          |
 | +---------------------------------------------+
 | 
 | UINT32 odu_rfrm_field_GCC2_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_GCC2_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 odu_rfrm_field_GCC1_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_GCC1_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00010c20 + (N) * 0x40) NEAR_END_PMON_PM |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  20   R  PN_DS_PM                               |
 | | bit  19:0 R  PN_EBC_PM                              |
 | +-----------------------------------------------------+
 | 
 | UINT32 odu_rfrm_field_PN_DS_PM_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_PN_EBC_PM_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_PN_EBC_PM_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00010c24 + (N) * 0x40) FAR_END_PMON_PM |
 | |     N[0..95]                                       |
 | +----------------------------------------------------+
 | | bit  20   R  PF_DS_PM                              |
 | | bit  19:0 R  PF_EBC_PM                             |
 | +----------------------------------------------------+
 | 
 | UINT32 odu_rfrm_field_PF_DS_PM_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_PF_EBC_PM_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_PF_EBC_PM_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00010c28 + (N) * 0x40) DEFECTS_ACTIONS_PM |
 | |     N[0..95]                                          |
 | +-------------------------------------------------------+
 | | bit  31 R  ABDI_PM_V                                  |
 | | bit  29 R  ATSD_PM_V                                  |
 | | bit  28 R  ATSF_PM_V                                  |
 | | bit  25 R  CI_SSF_PM_V                                |
 | | bit  24 R  DLOFLOM_V                                  |
 | | bit  23 R  OOF_V                                      |
 | | bit  21 R  DBDI_PM_V                                  |
 | | bit  20 R  DDEG_PM_V                                  |
 | | bit  19 R  DTIM_PM_V                                  |
 | | bit  18 R  DAIS_PM_V                                  |
 | | bit  17 R  DOCI_PM_V                                  |
 | | bit  16 R  DLCK_PM_V                                  |
 | | bit  15 R  ABDI_PM_I                                  |
 | | bit  13 R  ATSD_PM_I                                  |
 | | bit  12 R  ATSF_PM_I                                  |
 | | bit  8  R  DLOFLOM_I                                  |
 | | bit  7  R  OOF_I                                      |
 | | bit  5  R  DBDI_PM_I                                  |
 | | bit  4  R  DDEG_PM_I                                  |
 | | bit  3  R  DTIM_PM_I                                  |
 | | bit  2  R  DAIS_PM_I                                  |
 | | bit  1  R  DOCI_PM_I                                  |
 | | bit  0  R  DLCK_PM_I                                  |
 | +-------------------------------------------------------+
 | 
 | UINT32 odu_rfrm_field_ABDI_PM_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_ATSD_PM_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_ATSF_PM_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_CI_SSF_PM_V_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DLOFLOM_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_OOF_V_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DBDI_PM_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DDEG_PM_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DTIM_PM_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DAIS_PM_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DOCI_PM_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DLCK_PM_V_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_ABDI_PM_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_ATSD_PM_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_ATSF_PM_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DLOFLOM_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_OOF_I_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DBDI_PM_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DDEG_PM_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DTIM_PM_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DAIS_PM_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DOCI_PM_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_DLCK_PM_I_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------+
 | | Register (0x00010c2c + (N) * 0x40) NE_FE_EBC_PM |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  28:16 R  FE_EBC_PM                         |
 | | bit  12:0  R  NE_EBC_PM                         |
 | +-------------------------------------------------+
 | 
 | UINT32 odu_rfrm_field_FE_EBC_PM_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_FE_EBC_PM_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 odu_rfrm_field_NE_EBC_PM_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_NE_EBC_PM_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x00010c30 + (N) * 0x40) AC_STAT |
 | |     N[0..95]                               |
 | +--------------------------------------------+
 | | bit  26:24 R  AC_TCM6                      |
 | | bit  22:20 R  AC_TCM5                      |
 | | bit  18:16 R  AC_TCM4                      |
 | | bit  14:12 R  AC_TCM3                      |
 | | bit  10:8  R  AC_TCM2                      |
 | | bit  6:4   R  AC_TCM1                      |
 | | bit  2:0   R  AC_PM                        |
 | +--------------------------------------------+
 | 
 | UINT32 odu_rfrm_field_AC_TCM6_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_AC_TCM6_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 odu_rfrm_field_AC_TCM5_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_AC_TCM5_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 odu_rfrm_field_AC_TCM4_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_AC_TCM4_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 odu_rfrm_field_AC_TCM3_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_AC_TCM3_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 odu_rfrm_field_AC_TCM2_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_AC_TCM2_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 odu_rfrm_field_AC_TCM1_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_AC_TCM1_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 odu_rfrm_field_AC_PM_get         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_AC_PM_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x00010c34 + (N) * 0x40) ROW2_OH |
 | |     N[0..95]                               |
 | +--------------------------------------------+
 | | bit  31:24 R  PM_TCM                       |
 | | bit  23:16 R  TCM_ACT                      |
 | | bit  15:0  R  RES1                         |
 | +--------------------------------------------+
 | 
 | UINT32 odu_rfrm_field_PM_TCM_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_PM_TCM_get  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 odu_rfrm_field_TCM_ACT_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_TCM_ACT_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 odu_rfrm_field_RES1_get          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_RES1_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register (0x00010c38 + (N) * 0x40) ROW3_4_OH |
 | |     N[0..95]                                 |
 | +----------------------------------------------+
 | | bit  31:16 R  RES2                           |
 | | bit  15:0  R  EXP                            |
 | +----------------------------------------------+
 | 
 | UINT32 odu_rfrm_field_RES2_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_RES2_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 odu_rfrm_field_EXP_get        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_EXP_get  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register (0x00010c3c + (N) * 0x40) ROW4_OH0 |
 | |     N[0..95]                                |
 | +---------------------------------------------+
 | | bit  31:0 R  RES3                           |
 | +---------------------------------------------+
 | 
 | UINT32 odu_rfrm_field_RES3_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | UINT32 odu_rfrm_field_range_RES3_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00012400 DEFECT_INT_STAT |
 | +-------------------------------------+
 | | bit  15 R/W  COMB_ABDI_I            |
 | | bit  14 R/W  COMB_AAIS_I            |
 | | bit  13 R/W  COMB_ATSD_I            |
 | | bit  12 R/W  COMB_ATSF_I            |
 | | bit  10 R/W  COMB_OOF_I             |
 | | bit  9  R/W  COMB_DLOFLOM_I         |
 | | bit  8  R/W  COMB_DBIAE_I           |
 | | bit  7  R/W  COMB_DIAE_I            |
 | | bit  6  R/W  COMB_DLTC_I            |
 | | bit  5  R/W  COMB_DBDI_I            |
 | | bit  4  R/W  COMB_DDEG_I            |
 | | bit  3  R/W  COMB_DTIM_I            |
 | | bit  2  R/W  COMB_DAIS_I            |
 | | bit  1  R/W  COMB_DOCI_I            |
 | | bit  0  R/W  COMB_DLCK_I            |
 | +-------------------------------------+
 | 
 | void                 odu_rfrm_field_COMB_ABDI_I_set_to_clear    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_ABDI_I_get             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_ABDI_I_poll            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_COMB_AAIS_I_set_to_clear    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_AAIS_I_get             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_AAIS_I_poll            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_COMB_ATSD_I_set_to_clear    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_ATSD_I_get             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_ATSD_I_poll            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_COMB_ATSF_I_set_to_clear    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_ATSF_I_get             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_ATSF_I_poll            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_COMB_OOF_I_set_to_clear     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_OOF_I_get              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_OOF_I_poll             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_COMB_DLOFLOM_I_set_to_clear ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_DLOFLOM_I_get          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DLOFLOM_I_poll         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_COMB_DBIAE_I_set_to_clear   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_DBIAE_I_get            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DBIAE_I_poll           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_COMB_DIAE_I_set_to_clear    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_DIAE_I_get             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DIAE_I_poll            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_COMB_DLTC_I_set_to_clear    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_DLTC_I_get             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DLTC_I_poll            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_COMB_DBDI_I_set_to_clear    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_DBDI_I_get             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DBDI_I_poll            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_COMB_DDEG_I_set_to_clear    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_DDEG_I_get             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DDEG_I_poll            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_COMB_DTIM_I_set_to_clear    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_DTIM_I_get             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DTIM_I_poll            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_COMB_DAIS_I_set_to_clear    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_DAIS_I_get             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DAIS_I_poll            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_COMB_DOCI_I_set_to_clear    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_DOCI_I_get             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DOCI_I_poll            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_COMB_DLCK_I_set_to_clear    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_COMB_DLCK_I_get             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_COMB_DLCK_I_poll            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------------+
 | | Register 0x00012404 CHANNEL_DEFECT_INT_SUMMARY0 |
 | +-------------------------------------------------+
 | | bit  31:0 R  PM_DEF_I                           |
 | +-------------------------------------------------+
 | 
 | void   odu_rfrm_lfield_PM_DEF_I_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 odu_rfrm_lfield_range_PM_DEF_I_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x00012410 PID_DEFECT_INT_SUMMARY0 |
 | +---------------------------------------------+
 | | bit  31:0 R  PID_DEF_I                      |
 | +---------------------------------------------+
 | 
 | void   odu_rfrm_lfield_PID_DEF_I_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[8] )
 | UINT32 odu_rfrm_lfield_range_PID_DEF_I_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x00012430 SHADOW_TRANSFER_INT_STAT |
 | +----------------------------------------------+
 | | bit  5 R/W  PMON_DONE_I                      |
 | | bit  4 R/W  DEF_ACT_DONE_I                   |
 | | bit  1 R/W  PMON_START_I                     |
 | | bit  0 R/W  DEF_ACT_START_I                  |
 | +----------------------------------------------+
 | 
 | void                 odu_rfrm_field_PMON_DONE_I_set_to_clear     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_PMON_DONE_I_get              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_PMON_DONE_I_poll             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_DEF_ACT_DONE_I_set_to_clear  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_DEF_ACT_DONE_I_get           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_DEF_ACT_DONE_I_poll          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_PMON_START_I_set_to_clear    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_PMON_START_I_get             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_PMON_START_I_poll            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 odu_rfrm_field_DEF_ACT_START_I_set_to_clear ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32               odu_rfrm_field_DEF_ACT_START_I_get          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_field_DEF_ACT_START_I_poll         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------------+
 | | Register 0x0001243c CHANNEL_AC_APS_INT_STATUS0 |
 | +------------------------------------------------+
 | | bit  31:0 R/W  AC_APS_I                        |
 | +------------------------------------------------+
 | 
 | void                 odu_rfrm_lfield_AC_APS_I_set_to_clear       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_rfrm_lfield_AC_APS_I_get                ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[3] )
 | void                 odu_rfrm_lfield_range_AC_APS_I_set_to_clear ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               odu_rfrm_lfield_range_AC_APS_I_get          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE odu_rfrm_lfield_range_AC_APS_I_poll         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE odu_rfrm_lfield_AC_APS_I_poll               ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x00012448 CHANNEL_AC_APS_INT_EN0 |
 | +--------------------------------------------+
 | | bit  31:0 R/W  AC_APS_E                    |
 | +--------------------------------------------+
 | 
 | void   odu_rfrm_lfield_AC_APS_E_set       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_rfrm_lfield_AC_APS_E_get       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value[3] )
 | void   odu_rfrm_lfield_range_AC_APS_E_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 odu_rfrm_lfield_range_AC_APS_E_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00012454 DEFECT_INT_EN |
 | +-----------------------------------+
 | | bit  15 R/W  COMB_ABDI_E          |
 | | bit  14 R/W  COMB_AAIS_E          |
 | | bit  13 R/W  COMB_ATSD_E          |
 | | bit  12 R/W  COMB_ATSF_E          |
 | | bit  10 R/W  COMB_OOF_E           |
 | | bit  9  R/W  COMB_DLOFLOM_E       |
 | | bit  8  R/W  COMB_DBIAE_E         |
 | | bit  7  R/W  COMB_DIAE_E          |
 | | bit  6  R/W  COMB_DLTC_E          |
 | | bit  5  R/W  COMB_DBDI_E          |
 | | bit  4  R/W  COMB_DDEG_E          |
 | | bit  3  R/W  COMB_DTIM_E          |
 | | bit  2  R/W  COMB_DAIS_E          |
 | | bit  1  R/W  COMB_DOCI_E          |
 | | bit  0  R/W  COMB_DLCK_E          |
 | +-----------------------------------+
 | 
 | void   odu_rfrm_field_COMB_ABDI_E_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_ABDI_E_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_COMB_AAIS_E_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_AAIS_E_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_COMB_ATSD_E_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_ATSD_E_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_COMB_ATSF_E_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_ATSF_E_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_COMB_OOF_E_set     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_OOF_E_get     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_COMB_DLOFLOM_E_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_DLOFLOM_E_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_COMB_DBIAE_E_set   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_DBIAE_E_get   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_COMB_DIAE_E_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_DIAE_E_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_COMB_DLTC_E_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_DLTC_E_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_COMB_DBDI_E_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_DBDI_E_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_COMB_DDEG_E_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_DDEG_E_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_COMB_DTIM_E_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_DTIM_E_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_COMB_DAIS_E_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_DAIS_E_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_COMB_DOCI_E_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_DOCI_E_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_COMB_DLCK_E_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_COMB_DLCK_E_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | +--------------------------------------------+
 | | Register 0x00012458 SHADOW_TRANSFER_INT_EN |
 | +--------------------------------------------+
 | | bit  5 R/W  PMON_DONE_E                    |
 | | bit  4 R/W  DEF_ACT_DONE_E                 |
 | | bit  1 R/W  PMON_START_E                   |
 | | bit  0 R/W  DEF_ACT_START_E                |
 | +--------------------------------------------+
 | 
 | void   odu_rfrm_field_PMON_DONE_E_set     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_PMON_DONE_E_get     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_DEF_ACT_DONE_E_set  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_DEF_ACT_DONE_E_get  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_PMON_START_E_set    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_PMON_START_E_get    ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_DEF_ACT_START_E_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_DEF_ACT_START_E_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x00012464 GLOBAL_CONFIG |
 | +-----------------------------------+
 | | bit  15 R/W  DLTC_ACT_DIS         |
 | | bit  14 R/W  DAIS_ACT_DIS         |
 | | bit  13 R/W  DOCI_ACT_DIS         |
 | | bit  12 R/W  DLCK_ACT_DIS         |
 | | bit  10 R/W  AC_TTI_FIELD         |
 | | bit  9  R/W  LOCK_TTI             |
 | | bit  8  R/W  LOCK                 |
 | +-----------------------------------+
 | 
 | void   odu_rfrm_field_DLTC_ACT_DIS_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_DLTC_ACT_DIS_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_DAIS_ACT_DIS_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_DAIS_ACT_DIS_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_DOCI_ACT_DIS_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_DOCI_ACT_DIS_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_DLCK_ACT_DIS_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_DLCK_ACT_DIS_get ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_LOCK_TTI_set     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_LOCK_TTI_get     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void   odu_rfrm_field_LOCK_set         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | UINT32 odu_rfrm_field_LOCK_get         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ODU_RFRM_IO_H
#define _ODU_RFRM_IO_H

#include "odu_rfrm_loc.h"
#include "odu_rfrm_regs.h"
#include "odu_rfrm_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 odu_rfrm_buffer_init               ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | void                 odu_rfrm_buffer_flush              ( odu_rfrm_buffer_t *b_ptr )
 | UINT32               odu_rfrm_reg_read                  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 odu_rfrm_reg_write                 ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 odu_rfrm_field_set                 ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 odu_rfrm_action_on_write_field_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_rfrm_burst_read                ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 odu_rfrm_burst_write               ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE odu_rfrm_poll                      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 odu_rfrm_reg_GLOBAL_CONFIG_write                                  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_rfrm_reg_GLOBAL_CONFIG_field_set                              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_GLOBAL_CONFIG_read                                   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | void                 odu_rfrm_reg_CHANNEL_CTRL1_array_write                            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_rfrm_reg_CHANNEL_CTRL1_array_field_set                        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_CHANNEL_CTRL1_array_read                             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_write                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_field_set                     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_CHANNEL_AC_APS_INT_EN0_read                          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | void                 odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_write                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_field_set                     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_CHANNEL_AC_APS_INT_EN1_read                          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | void                 odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_write                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_field_set                     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_CHANNEL_AC_APS_INT_EN2_read                          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | void                 odu_rfrm_reg_DEFECT_INT_EN_write                                  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_rfrm_reg_DEFECT_INT_EN_field_set                              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_DEFECT_INT_EN_read                                   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | void                 odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_write                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_field_set                     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_SHADOW_TRANSFER_INT_EN_read                          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | void                 odu_rfrm_reg_DEFECT_INT_STAT_write                                ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_rfrm_reg_DEFECT_INT_STAT_action_on_write_field_set            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_DEFECT_INT_STAT_read                                 ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_reg_DEFECT_INT_STAT_poll                                 ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_write                       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_action_on_write_field_set   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_read                        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_reg_SHADOW_TRANSFER_INT_STAT_poll                        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_write                     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_action_on_write_field_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_read                      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS0_poll                      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_write                     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_action_on_write_field_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_read                      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS1_poll                      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_write                     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value )
 | void                 odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_action_on_write_field_set ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_read                      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE odu_rfrm_reg_CHANNEL_AC_APS_INT_STATUS2_poll                      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 odu_rfrm_reg_AC_TTI_PM_array_burst_read                           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_rfrm_reg_AC_TTI_PM_array_read                                 ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_rfrm_reg_EXP_TTI_PID_array_burst_write                        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_rfrm_reg_EXP_TTI_PID_array_field_set                          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_rfrm_reg_EXP_TTI_PID_array_burst_read                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_rfrm_reg_EXP_TTI_PID_array_read                               ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_rfrm_reg_AC_TTI_PID_array_burst_read                          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_rfrm_reg_AC_TTI_PID_array_read                                ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_rfrm_reg_EXP_TTI_PM_array_burst_write                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 odu_rfrm_reg_EXP_TTI_PM_array_field_set                           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 odu_rfrm_reg_EXP_TTI_PM_array_burst_read                          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_rfrm_reg_EXP_TTI_PM_array_read                                ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_rfrm_reg_FTFL_array_burst_read                                ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_rfrm_reg_FTFL_array_read                                      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_rfrm_reg_AC_APS_PCC_array_burst_read                          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               odu_rfrm_reg_AC_APS_PCC_array_read                                ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 N )
 | 
 | void                 odu_rfrm_reg_LAYER_CTRL0_PID_array_write                          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_rfrm_reg_LAYER_CTRL0_PID_array_field_set                      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_LAYER_CTRL0_PID_array_read                           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_rfrm_reg_LAYER_CTRL1_PID_array_write                          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_rfrm_reg_LAYER_CTRL1_PID_array_field_set                      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_LAYER_CTRL1_PID_array_read                           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_rfrm_reg_NEAR_END_PMON_PID_array_read                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_rfrm_reg_FAR_END_PMON_PID_array_read                          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_rfrm_reg_DEFECTS_ACTIONS_PID_array_read                       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_rfrm_reg_NE_FE_EBC_PID_array_read                             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_rfrm_reg_LAYER_CTRL0_PM_array_write                           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_rfrm_reg_LAYER_CTRL0_PM_array_field_set                       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_LAYER_CTRL0_PM_array_read                            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_rfrm_reg_LAYER_CTRL1_PM_array_write                           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_rfrm_reg_LAYER_CTRL1_PM_array_field_set                       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_LAYER_CTRL1_PM_array_read                            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_rfrm_reg_TCM1_2_3_PID_array_write                             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_rfrm_reg_TCM1_2_3_PID_array_field_set                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_TCM1_2_3_PID_array_read                              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_rfrm_reg_TCM4_5_6_PID_array_write                             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_rfrm_reg_TCM4_5_6_PID_array_field_set                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_TCM4_5_6_PID_array_read                              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_rfrm_reg_FLOATING_PM_NIM_PID_array_write                      ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_rfrm_reg_FLOATING_PM_NIM_PID_array_field_set                  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_FLOATING_PM_NIM_PID_array_read                       ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 odu_rfrm_reg_CHANNEL_CTRL0_array_write                            ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 odu_rfrm_reg_CHANNEL_CTRL0_array_field_set                        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               odu_rfrm_reg_CHANNEL_CTRL0_array_read                             ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_rfrm_reg_ROW4_OH1_array_read                                  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_rfrm_reg_NEAR_END_PMON_PM_array_read                          ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_rfrm_reg_FAR_END_PMON_PM_array_read                           ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_rfrm_reg_DEFECTS_ACTIONS_PM_array_read                        ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_rfrm_reg_NE_FE_EBC_PM_array_read                              ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_rfrm_reg_AC_STAT_array_read                                   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_rfrm_reg_ROW2_OH_array_read                                   ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_rfrm_reg_ROW3_4_OH_array_read                                 ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_rfrm_reg_ROW4_OH0_array_read                                  ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY0_read                     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | UINT32               odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY1_read                     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | UINT32               odu_rfrm_reg_CHANNEL_DEFECT_INT_SUMMARY2_read                     ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | UINT32               odu_rfrm_reg_PID_DEFECT_INT_SUMMARY0_read                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | UINT32               odu_rfrm_reg_PID_DEFECT_INT_SUMMARY1_read                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | UINT32               odu_rfrm_reg_PID_DEFECT_INT_SUMMARY2_read                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | UINT32               odu_rfrm_reg_PID_DEFECT_INT_SUMMARY3_read                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | UINT32               odu_rfrm_reg_PID_DEFECT_INT_SUMMARY4_read                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | UINT32               odu_rfrm_reg_PID_DEFECT_INT_SUMMARY5_read                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | UINT32               odu_rfrm_reg_PID_DEFECT_INT_SUMMARY6_read                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 | UINT32               odu_rfrm_reg_PID_DEFECT_INT_SUMMARY7_read                         ( odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ODU_RFRM_IO_H */
