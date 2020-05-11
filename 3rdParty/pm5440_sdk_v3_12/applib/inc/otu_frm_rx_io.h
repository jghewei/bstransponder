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
 *     and register accessor functions for the otu_frm_rx block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing otu_frm_rx io functions is:
 | {
 |     otu_frm_rx_buffer_t b_ptr[1];
 |     otu_frm_rx_buffer_init( b_ptr, h_ptr );
 |     value = otu_frm_rx_field_<FIELD>_get( b_ptr, h_ptr );
 |     otu_frm_rx_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     otu_frm_rx_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = otu_frm_rx_field_<FIELD>_get( NULL, h_ptr );
 | otu_frm_rx_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------------------------------------------+
 | | Register (0x00002000 + (N) * 0x100 + (M) * 0x04) OTU_SK_SM_EXP_TTI |
 | |     N[0..11]                                                       |
 | |     M[0..15]                                                       |
 | +--------------------------------------------------------------------+
 | | bit  31:0 R/W  OTU_SK_SM_EXP_TTI                                   |
 | +--------------------------------------------------------------------+
 | 
 | void   otu_frm_rx_lfield_OTU_SK_SM_EXP_TTI_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value[16] )
 | void   otu_frm_rx_lfield_OTU_SK_SM_EXP_TTI_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value[16] )
 | void   otu_frm_rx_lfield_range_OTU_SK_SM_EXP_TTI_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_lfield_range_OTU_SK_SM_EXP_TTI_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00002040 + (N) * 0x100) OTU_SK_SM_DEG_THR |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  23:20 R/W  OTU_SK_SM_DEG_M                       |
 | | bit  19:0  R/W  OTU_SK_SM_DEG_THR                     |
 | +-------------------------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_SM_DEG_M_set         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_SM_DEG_M_get         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_range_OTU_SK_SM_DEG_M_set   ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_SM_DEG_M_get   ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_rx_field_OTU_SK_SM_DEG_THR_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_SM_DEG_THR_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_range_OTU_SK_SM_DEG_THR_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_SM_DEG_THR_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x00002044 + (N) * 0x100) OTU_SK_SM_CTL |
 | |     N[0..11]                                      |
 | +---------------------------------------------------+
 | | bit  22  R/W  OTU_SK_BDI_DIS_FROM_TTI             |
 | | bit  21  R/W  OTU_SK_BDI_DIS_FROM_SSF             |
 | | bit  20  R/W  OTU_SK_BEI_DIS                      |
 | | bit  19  R/W  OTU_SK_BIAE_DIS                     |
 | | bit  18  R/W  OTU_SK_TSD_DIS                      |
 | | bit  17  R/W  OTU_SK_TSF_DIS_FROM_SSF             |
 | | bit  16  R/W  OTU_SK_TIM_ACT_DIS                  |
 | | bit  14  R/W  OTU_SK_ADAPT_SSD_DIS                |
 | | bit  13  R/W  OTU_SK_ADAPT_SSF_DIS                |
 | | bit  12  R/W  OTU_SK_ADAPT_AIS_DIS                |
 | | bit  8   R/W  OTU_SK_MI_LCK                       |
 | | bit  7   R/W  OTU_SK_SSF_MI_DIS                   |
 | | bit  6   R/W  OTU_SK_AIS_MI_DIS                   |
 | | bit  5   R/W  OTU_SK_SSD_MI_DIS                   |
 | | bit  3:2 R/W  OTU_SK_TIM_DET_MO                   |
 | | bit  1:0 R/W  OTU_SK_MAINT_SIG                    |
 | +---------------------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_TTI_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_SSF_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_BDI_DIS_FROM_SSF_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_OTU_SK_BEI_DIS_set          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_BEI_DIS_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_OTU_SK_BIAE_DIS_set         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_BIAE_DIS_get         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_OTU_SK_TSD_DIS_set          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_TSD_DIS_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_OTU_SK_TSF_DIS_FROM_SSF_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_TSF_DIS_FROM_SSF_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_OTU_SK_TIM_ACT_DIS_set      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_TIM_ACT_DIS_get      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_OTU_SK_ADAPT_SSD_DIS_set    ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_ADAPT_SSD_DIS_get    ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_OTU_SK_ADAPT_SSF_DIS_set    ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_ADAPT_SSF_DIS_get    ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_OTU_SK_ADAPT_AIS_DIS_set    ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_ADAPT_AIS_DIS_get    ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_OTU_SK_MI_LCK_set           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_MI_LCK_get           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_OTU_SK_SSF_MI_DIS_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_SSF_MI_DIS_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_OTU_SK_AIS_MI_DIS_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_AIS_MI_DIS_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_OTU_SK_SSD_MI_DIS_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_SSD_MI_DIS_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_OTU_SK_TIM_DET_MO_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_TIM_DET_MO_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_range_OTU_SK_TIM_DET_MO_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_TIM_DET_MO_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_rx_field_OTU_SK_MAINT_SIG_set        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_MAINT_SIG_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_rx_field_range_OTU_SK_MAINT_SIG_set  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_MAINT_SIG_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00002048 + (N) * 0x100) OTU_SK_PASSTHRU |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  0 R/W  OTU_SK_PASSTHRU                         |
 | +-----------------------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_PASSTHRU_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_PASSTHRU_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00002080 + (N) * 0x100 + (M) * 0x04) OTU_SK_SM_AC_TTI |
 | |     N[0..11]                                                      |
 | |     M[0..15]                                                      |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R  OTU_SK_SM_AC_TTI                                     |
 | +-------------------------------------------------------------------+
 | 
 | void   otu_frm_rx_lfield_OTU_SK_SM_AC_TTI_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value[16] )
 | UINT32 otu_frm_rx_lfield_range_OTU_SK_SM_AC_TTI_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x000020c0 + (N) * 0x100) OTU_SK_SM_PMON |
 | |     N[0..11]                                       |
 | +----------------------------------------------------+
 | | bit  25   R  OTU_SK_SM_P_IAE                       |
 | | bit  24   R  OTU_SK_SM_P_N_DS                      |
 | | bit  23:0 R  OTU_SK_SM_P_N_EBC                     |
 | +----------------------------------------------------+
 | 
 | UINT32 otu_frm_rx_field_OTU_SK_SM_P_IAE_get         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | UINT32 otu_frm_rx_field_OTU_SK_SM_P_N_DS_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | UINT32 otu_frm_rx_field_OTU_SK_SM_P_N_EBC_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | UINT32 otu_frm_rx_field_range_OTU_SK_SM_P_N_EBC_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x000020c4 + (N) * 0x100) OTU_SK_SM_FE_PMON |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  25   R  OTU_SK_SM_P_BIAE                         |
 | | bit  24   R  OTU_SK_SM_P_F_DS                         |
 | | bit  23:0 R  OTU_SK_SM_P_F_EBC                        |
 | +-------------------------------------------------------+
 | 
 | UINT32 otu_frm_rx_field_OTU_SK_SM_P_BIAE_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | UINT32 otu_frm_rx_field_OTU_SK_SM_P_F_DS_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | UINT32 otu_frm_rx_field_OTU_SK_SM_P_F_EBC_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | UINT32 otu_frm_rx_field_range_OTU_SK_SM_P_F_EBC_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x000020c8 + (N) * 0x100) OTU_SK_SM_OH |
 | |     N[0..11]                                     |
 | +--------------------------------------------------+
 | | bit  23:16 R  OTU_SK_SM_TTI                      |
 | | bit  15:8  R  OTU_SK_SM_BIP8                     |
 | | bit  7:4   R  OTU_SK_SM_BEI_BIAE                 |
 | | bit  3     R  OTU_SK_SM_BDI                      |
 | | bit  2     R  OTU_SK_SM_IAE                      |
 | | bit  1:0   R  OTU_SK_SM_RES                      |
 | +--------------------------------------------------+
 | 
 | UINT32 otu_frm_rx_field_OTU_SK_SM_TTI_get            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | UINT32 otu_frm_rx_field_range_OTU_SK_SM_TTI_get      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 otu_frm_rx_field_OTU_SK_SM_BIP8_get           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | UINT32 otu_frm_rx_field_range_OTU_SK_SM_BIP8_get     ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 otu_frm_rx_field_OTU_SK_SM_BEI_BIAE_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | UINT32 otu_frm_rx_field_range_OTU_SK_SM_BEI_BIAE_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 otu_frm_rx_field_OTU_SK_SM_BDI_get            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | UINT32 otu_frm_rx_field_OTU_SK_SM_IAE_get            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | UINT32 otu_frm_rx_field_OTU_SK_SM_RES_get            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | UINT32 otu_frm_rx_field_range_OTU_SK_SM_RES_get      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x000020cc + (N) * 0x100) OTU_SK_OH2 |
 | |     N[0..11]                                   |
 | +------------------------------------------------+
 | | bit  31:16 R  OTU_SK_GCC0                      |
 | | bit  15:0  R  OTU_SK_RES                       |
 | +------------------------------------------------+
 | 
 | UINT32 otu_frm_rx_field_OTU_SK_GCC0_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | UINT32 otu_frm_rx_field_range_OTU_SK_GCC0_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 otu_frm_rx_field_OTU_SK_RES_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | UINT32 otu_frm_rx_field_range_OTU_SK_RES_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00002f00 OTU_SK_CFG  |
 | +---------------------------------+
 | | bit  2 R/W  OTU_SK_AC_TTI_FIELD |
 | | bit  1 R/W  OTU_SK_TTI_LOCK     |
 | | bit  0 R/W  OTU_SK_LOCK         |
 | +---------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_AC_TTI_set   ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_AC_TTI_get   ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_OTU_SK_TTI_LOCK_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_TTI_LOCK_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_OTU_SK_LOCK_set     ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_LOCK_get     ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00002f40 OTU_SK_A_AIS_E |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_AIS_E      |
 | +------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_A_AIS_E_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_A_AIS_E_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_range_OTU_SK_A_AIS_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_A_AIS_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00002f44 OTU_SK_A_SSD_E |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_SSD_E      |
 | +------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_A_SSD_E_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_A_SSD_E_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_range_OTU_SK_A_SSD_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_A_SSD_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00002f48 OTU_SK_A_SSF_E |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_SSF_E      |
 | +------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_A_SSF_E_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_A_SSF_E_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_range_OTU_SK_A_SSF_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_A_SSF_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00002f4c OTU_SK_A_TSF_E |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_TSF_E      |
 | +------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_A_TSF_E_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_A_TSF_E_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_range_OTU_SK_A_TSF_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_A_TSF_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00002f50 OTU_SK_A_TSD_E |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_TSD_E      |
 | +------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_A_TSD_E_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_A_TSD_E_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_range_OTU_SK_A_TSD_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_A_TSD_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00002f54 OTU_SK_A_BIAE_E |
 | +-------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_BIAE_E      |
 | +-------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_A_BIAE_E_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_A_BIAE_E_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_range_OTU_SK_A_BIAE_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_A_BIAE_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00002f58 OTU_SK_A_BEI_E |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_BEI_E      |
 | +------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_A_BEI_E_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_A_BEI_E_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_range_OTU_SK_A_BEI_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_A_BEI_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00002f5c OTU_SK_A_BDI_E |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_BDI_E      |
 | +------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_A_BDI_E_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_A_BDI_E_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_range_OTU_SK_A_BDI_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_A_BDI_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00002f60 OTU_SK_D_IAE_E |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_D_IAE_E      |
 | +------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_D_IAE_E_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_D_IAE_E_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_range_OTU_SK_D_IAE_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_D_IAE_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00002f64 OTU_SK_D_TIM_E |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_D_TIM_E      |
 | +------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_D_TIM_E_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_D_TIM_E_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_range_OTU_SK_D_TIM_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_D_TIM_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00002f68 OTU_SK_D_DEG_E |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_D_DEG_E      |
 | +------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_D_DEG_E_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_D_DEG_E_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_range_OTU_SK_D_DEG_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_D_DEG_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00002f6c OTU_SK_D_BDI_E |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_D_BDI_E      |
 | +------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_D_BDI_E_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_D_BDI_E_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_range_OTU_SK_D_BDI_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_D_BDI_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00002f70 OTU_SK_D_BIAE_E |
 | +-------------------------------------+
 | | bit  11:0 R/W  OTU_SK_D_BIAE_E      |
 | +-------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_D_BIAE_E_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_D_BIAE_E_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_range_OTU_SK_D_BIAE_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_D_BIAE_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00002f74 OTU_SK_CI_SSF_E |
 | +-------------------------------------+
 | | bit  11:0 R/W  OTU_SK_CI_SSF_E      |
 | +-------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_CI_SSF_E_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_CI_SSF_E_get       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_rx_field_range_OTU_SK_CI_SSF_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_rx_field_range_OTU_SK_CI_SSF_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00002f78 OTU_SK_SHADOW_XFER_E |
 | +------------------------------------------+
 | | bit  0 R/W  OTU_SK_COUNT_DONE_E          |
 | +------------------------------------------+
 | 
 | void   otu_frm_rx_field_OTU_SK_COUNT_DONE_E_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_rx_field_OTU_SK_COUNT_DONE_E_get ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00002f80 OTU_SK_A_AIS_I |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_AIS_I      |
 | +------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_A_AIS_I_set_to_clear       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_A_AIS_I_get                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_field_range_OTU_SK_A_AIS_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_AIS_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_AIS_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_AIS_I_poll               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002f84 OTU_SK_A_SSD_I |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_SSD_I      |
 | +------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_A_SSD_I_set_to_clear       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_A_SSD_I_get                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_field_range_OTU_SK_A_SSD_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_SSD_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_SSD_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_SSD_I_poll               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002f88 OTU_SK_A_SSF_I |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_SSF_I      |
 | +------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_A_SSF_I_set_to_clear       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_A_SSF_I_get                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_field_range_OTU_SK_A_SSF_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_SSF_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_SSF_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_SSF_I_poll               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002f8c OTU_SK_A_TSF_I |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_TSF_I      |
 | +------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_A_TSF_I_set_to_clear       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_A_TSF_I_get                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_field_range_OTU_SK_A_TSF_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_TSF_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_TSF_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_TSF_I_poll               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002f90 OTU_SK_A_TSD_I |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_TSD_I      |
 | +------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_A_TSD_I_set_to_clear       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_A_TSD_I_get                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_field_range_OTU_SK_A_TSD_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_TSD_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_TSD_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_TSD_I_poll               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00002f94 OTU_SK_A_BIAE_I |
 | +-------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_BIAE_I      |
 | +-------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_A_BIAE_I_set_to_clear       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_A_BIAE_I_get                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_field_range_OTU_SK_A_BIAE_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_BIAE_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BIAE_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BIAE_I_poll               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002f98 OTU_SK_A_BEI_I |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_BEI_I      |
 | +------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_A_BEI_I_set_to_clear       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_A_BEI_I_get                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_field_range_OTU_SK_A_BEI_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_BEI_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BEI_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BEI_I_poll               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002f9c OTU_SK_A_BDI_I |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_A_BDI_I      |
 | +------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_A_BDI_I_set_to_clear       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_A_BDI_I_get                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_field_range_OTU_SK_A_BDI_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_BDI_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BDI_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BDI_I_poll               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fa0 OTU_SK_D_IAE_I |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_D_IAE_I      |
 | +------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_D_IAE_I_set_to_clear       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_D_IAE_I_get                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_field_range_OTU_SK_D_IAE_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_rx_field_range_OTU_SK_D_IAE_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_IAE_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_IAE_I_poll               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fa4 OTU_SK_D_TIM_I |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_D_TIM_I      |
 | +------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_D_TIM_I_set_to_clear       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_D_TIM_I_get                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_field_range_OTU_SK_D_TIM_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_rx_field_range_OTU_SK_D_TIM_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_TIM_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_TIM_I_poll               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fa8 OTU_SK_D_DEG_I |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_D_DEG_I      |
 | +------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_D_DEG_I_set_to_clear       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_D_DEG_I_get                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_field_range_OTU_SK_D_DEG_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_rx_field_range_OTU_SK_D_DEG_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_DEG_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_DEG_I_poll               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fac OTU_SK_D_BDI_I |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SK_D_BDI_I      |
 | +------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_D_BDI_I_set_to_clear       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_D_BDI_I_get                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_field_range_OTU_SK_D_BDI_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_rx_field_range_OTU_SK_D_BDI_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_BDI_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_BDI_I_poll               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00002fb0 OTU_SK_D_BIAE_I |
 | +-------------------------------------+
 | | bit  11:0 R/W  OTU_SK_D_BIAE_I      |
 | +-------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_D_BIAE_I_set_to_clear       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_D_BIAE_I_get                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_field_range_OTU_SK_D_BIAE_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_rx_field_range_OTU_SK_D_BIAE_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_BIAE_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_BIAE_I_poll               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00002fb4 OTU_SK_CI_SSF_I |
 | +-------------------------------------+
 | | bit  11:0 R/W  OTU_SK_CI_SSF_I      |
 | +-------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_CI_SSF_I_set_to_clear       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_CI_SSF_I_get                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_field_range_OTU_SK_CI_SSF_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_rx_field_range_OTU_SK_CI_SSF_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_CI_SSF_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_CI_SSF_I_poll               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x00002fb8 OTU_SK_SHADOW_XFER_I |
 | +------------------------------------------+
 | | bit  0 R/W  OTU_SK_COUNT_DONE_I          |
 | +------------------------------------------+
 | 
 | void                 otu_frm_rx_field_OTU_SK_COUNT_DONE_I_set_to_clear ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_rx_field_OTU_SK_COUNT_DONE_I_get          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_COUNT_DONE_I_poll         ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fc0 OTU_SK_A_AIS_V |
 | +------------------------------------+
 | | bit  11:0 R  OTU_SK_A_AIS_V        |
 | +------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_A_AIS_V_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_AIS_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_AIS_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_AIS_V_poll       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fc4 OTU_SK_A_SSD_V |
 | +------------------------------------+
 | | bit  11:0 R  OTU_SK_A_SSD_V        |
 | +------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_A_SSD_V_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_SSD_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_SSD_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_SSD_V_poll       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fc8 OTU_SK_A_SSF_V |
 | +------------------------------------+
 | | bit  11:0 R  OTU_SK_A_SSF_V        |
 | +------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_A_SSF_V_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_SSF_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_SSF_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_SSF_V_poll       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fcc OTU_SK_A_TSF_V |
 | +------------------------------------+
 | | bit  11:0 R  OTU_SK_A_TSF_V        |
 | +------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_A_TSF_V_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_TSF_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_TSF_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_TSF_V_poll       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fd0 OTU_SK_A_TSD_V |
 | +------------------------------------+
 | | bit  11:0 R  OTU_SK_A_TSD_V        |
 | +------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_A_TSD_V_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_TSD_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_TSD_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_TSD_V_poll       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00002fd4 OTU_SK_A_BIAE_V |
 | +-------------------------------------+
 | | bit  11:0 R  OTU_SK_A_BIAE_V        |
 | +-------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_A_BIAE_V_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_BIAE_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BIAE_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BIAE_V_poll       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fd8 OTU_SK_A_BEI_V |
 | +------------------------------------+
 | | bit  11:0 R  OTU_SK_A_BEI_V        |
 | +------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_A_BEI_V_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_BEI_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BEI_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BEI_V_poll       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fdc OTU_SK_A_BDI_V |
 | +------------------------------------+
 | | bit  11:0 R  OTU_SK_A_BDI_V        |
 | +------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_A_BDI_V_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_rx_field_range_OTU_SK_A_BDI_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_A_BDI_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_A_BDI_V_poll       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fe0 OTU_SK_D_IAE_V |
 | +------------------------------------+
 | | bit  11:0 R  OTU_SK_D_IAE_V        |
 | +------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_D_IAE_V_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_rx_field_range_OTU_SK_D_IAE_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_IAE_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_IAE_V_poll       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fe4 OTU_SK_D_TIM_V |
 | +------------------------------------+
 | | bit  11:0 R  OTU_SK_D_TIM_V        |
 | +------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_D_TIM_V_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_rx_field_range_OTU_SK_D_TIM_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_TIM_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_TIM_V_poll       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fe8 OTU_SK_D_DEG_V |
 | +------------------------------------+
 | | bit  11:0 R  OTU_SK_D_DEG_V        |
 | +------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_D_DEG_V_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_rx_field_range_OTU_SK_D_DEG_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_DEG_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_DEG_V_poll       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00002fec OTU_SK_D_BDI_V |
 | +------------------------------------+
 | | bit  11:0 R  OTU_SK_D_BDI_V        |
 | +------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_D_BDI_V_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_rx_field_range_OTU_SK_D_BDI_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_BDI_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_BDI_V_poll       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00002ff0 OTU_SK_D_BIAE_V |
 | +-------------------------------------+
 | | bit  11:0 R  OTU_SK_D_BIAE_V        |
 | +-------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_D_BIAE_V_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_rx_field_range_OTU_SK_D_BIAE_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_D_BIAE_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_D_BIAE_V_poll       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00002ff4 OTU_SK_CI_SSF_V |
 | +-------------------------------------+
 | | bit  11:0 R  OTU_SK_CI_SSF_V        |
 | +-------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_CI_SSF_V_get        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_rx_field_range_OTU_SK_CI_SSF_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_range_OTU_SK_CI_SSF_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_CI_SSF_V_poll       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x00002ff8 OTU_SK_SHADOW_XFER_V |
 | +------------------------------------------+
 | | bit  0 R  OTU_SK_COUNT_DONE_V            |
 | +------------------------------------------+
 | 
 | UINT32               otu_frm_rx_field_OTU_SK_COUNT_DONE_V_get  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_field_OTU_SK_COUNT_DONE_V_poll ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _OTU_FRM_RX_IO_H
#define _OTU_FRM_RX_IO_H

#include "pmc_sys.h"
#include "pmc_types.h"
#include "otu_frm_rx_regs.h"
#include "otu_frm_rx_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 otu_frm_rx_buffer_init               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_rx_buffer_flush              ( otu_frm_rx_buffer_t *b_ptr )
 | UINT32               otu_frm_rx_reg_read                  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 otu_frm_rx_reg_write                 ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 otu_frm_rx_field_set                 ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 otu_frm_rx_action_on_write_field_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 otu_frm_rx_burst_read                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 otu_frm_rx_burst_write               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_poll                      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 otu_frm_rx_reg_OTU_SK_CFG_write                               ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_CFG_field_set                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_CFG_read                                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_write                  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_field_set              ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_SM_DEG_THR_array_read                   ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_SM_CTL_array_write                      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_SM_CTL_array_field_set                  ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_SM_CTL_array_read                       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_PASSTHRU_array_write                    ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_PASSTHRU_array_field_set                ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_PASSTHRU_array_read                     ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_AIS_E_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_AIS_E_field_set                       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_AIS_E_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_SSD_E_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_SSD_E_field_set                       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_SSD_E_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_SSF_E_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_SSF_E_field_set                       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_SSF_E_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_TSF_E_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_TSF_E_field_set                       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_TSF_E_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_TSD_E_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_TSD_E_field_set                       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_TSD_E_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_BIAE_E_write                          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_BIAE_E_field_set                      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_BIAE_E_read                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_BEI_E_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_BEI_E_field_set                       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_BEI_E_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_BDI_E_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_BDI_E_field_set                       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_BDI_E_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_D_IAE_E_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_D_IAE_E_field_set                       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_D_IAE_E_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_D_TIM_E_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_D_TIM_E_field_set                       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_D_TIM_E_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_D_DEG_E_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_D_DEG_E_field_set                       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_D_DEG_E_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_D_BDI_E_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_D_BDI_E_field_set                       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_D_BDI_E_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_D_BIAE_E_write                          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_D_BIAE_E_field_set                      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_D_BIAE_E_read                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_CI_SSF_E_write                          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_CI_SSF_E_field_set                      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_CI_SSF_E_read                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_SHADOW_XFER_E_write                     ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_SHADOW_XFER_E_field_set                 ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_SHADOW_XFER_E_read                      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_AIS_I_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_AIS_I_action_on_write_field_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_AIS_I_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_AIS_I_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_SSD_I_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_SSD_I_action_on_write_field_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_SSD_I_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_SSD_I_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_SSF_I_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_SSF_I_action_on_write_field_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_SSF_I_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_SSF_I_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_TSF_I_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_TSF_I_action_on_write_field_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_TSF_I_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_TSF_I_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_TSD_I_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_TSD_I_action_on_write_field_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_TSD_I_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_TSD_I_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_BIAE_I_write                          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_BIAE_I_action_on_write_field_set      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_BIAE_I_read                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BIAE_I_poll                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_BEI_I_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_BEI_I_action_on_write_field_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_BEI_I_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BEI_I_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_A_BDI_I_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_A_BDI_I_action_on_write_field_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_A_BDI_I_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BDI_I_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_D_IAE_I_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_D_IAE_I_action_on_write_field_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_D_IAE_I_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_IAE_I_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_D_TIM_I_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_D_TIM_I_action_on_write_field_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_D_TIM_I_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_TIM_I_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_D_DEG_I_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_D_DEG_I_action_on_write_field_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_D_DEG_I_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_DEG_I_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_D_BDI_I_write                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_D_BDI_I_action_on_write_field_set       ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_D_BDI_I_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_BDI_I_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_D_BIAE_I_write                          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_D_BIAE_I_action_on_write_field_set      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_D_BIAE_I_read                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_BIAE_I_poll                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_CI_SSF_I_write                          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_CI_SSF_I_action_on_write_field_set      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_CI_SSF_I_read                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_CI_SSF_I_poll                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_write                     ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_action_on_write_field_set ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_read                      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_SHADOW_XFER_I_poll                      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_A_AIS_V_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_AIS_V_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_A_SSD_V_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_SSD_V_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_A_SSF_V_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_SSF_V_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_A_TSF_V_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_TSF_V_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_A_TSD_V_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_TSD_V_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_A_BIAE_V_read                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BIAE_V_poll                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_A_BEI_V_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BEI_V_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_A_BDI_V_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_A_BDI_V_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_D_IAE_V_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_IAE_V_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_D_TIM_V_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_TIM_V_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_D_DEG_V_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_DEG_V_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_D_BDI_V_read                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_BDI_V_poll                            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_D_BIAE_V_read                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_D_BIAE_V_poll                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_CI_SSF_V_read                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_CI_SSF_V_poll                           ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_SHADOW_XFER_V_read                      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_rx_reg_OTU_SK_SHADOW_XFER_V_poll                      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_SM_OH_array_read                        ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_OH2_array_read                          ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_burst_write            ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_field_set              ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 M, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_burst_read             ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               otu_frm_rx_reg_OTU_SK_SM_EXP_TTI_array_read                   ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 M )
 | 
 | void                 otu_frm_rx_reg_OTU_SK_SM_AC_TTI_array_burst_read              ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               otu_frm_rx_reg_OTU_SK_SM_AC_TTI_array_read                    ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 M )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_SM_PMON_array_read                      ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               otu_frm_rx_reg_OTU_SK_SM_FE_PMON_array_read                   ( otu_frm_rx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _OTU_FRM_RX_IO_H */
