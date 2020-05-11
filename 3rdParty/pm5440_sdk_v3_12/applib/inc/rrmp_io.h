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
 *     and register accessor functions for the rrmp block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing rrmp io functions is:
 | {
 |     rrmp_buffer_t b_ptr[1];
 |     rrmp_buffer_init( b_ptr, h_ptr, A );
 |     value = rrmp_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     rrmp_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     rrmp_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = rrmp_field_<FIELD>_get( NULL, h_ptr, A );
 | rrmp_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +---------------------------+
 | | Register 0x00000100 CFG   |
 | +---------------------------+
 | | bit  15  R    TIP         |
 | | bit  14  R/W  EXTAPSDIS   |
 | | bit  12  R/W  LREIACCBLK  |
 | | bit  11  R/W  LBIPEREIBLK |
 | | bit  9   R/W  LBIPEACCBLK |
 | | bit  7   R/W  SBIPEACCBLK |
 | | bit  6   R/W  LBIPEREISAT |
 | | bit  5:4 R/W  APSUNST     |
 | | bit  3   R/W  LRDI3       |
 | | bit  2   R/W  LAIS3       |
 | | bit  1   R/W  ALGO2       |
 | | bit  0   R/W  FOOF        |
 | +---------------------------+
 | 
 | void                 rrmp_field_EXTAPSDIS_set     ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               rrmp_field_EXTAPSDIS_get     ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rrmp_field_LREIACCBLK_set    ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               rrmp_field_LREIACCBLK_get    ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rrmp_field_LBIPEREIBLK_set   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               rrmp_field_LBIPEREIBLK_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rrmp_field_LBIPEACCBLK_set   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               rrmp_field_LBIPEACCBLK_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rrmp_field_SBIPEACCBLK_set   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               rrmp_field_SBIPEACCBLK_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rrmp_field_LBIPEREISAT_set   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               rrmp_field_LBIPEREISAT_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rrmp_field_APSUNST_set       ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               rrmp_field_APSUNST_get       ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rrmp_field_range_APSUNST_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               rrmp_field_range_APSUNST_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void                 rrmp_field_LRDI3_set         ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               rrmp_field_LRDI3_get         ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rrmp_field_LAIS3_set         ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               rrmp_field_LAIS3_get         ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rrmp_field_ALGO2_set         ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               rrmp_field_ALGO2_get         ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rrmp_field_FOOF_set          ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               rrmp_field_FOOF_get          ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32               rrmp_field_TIP_get           ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE rrmp_field_TIP_poll          ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------+
 | | Register 0x00000104 CFG_2   |
 | +-----------------------------+
 | | bit  15 R/W  DESCRAMA1A2SEL |
 | | bit  14 R/W  DESCRAMJ0SEL   |
 | | bit  13 R/W  FRM768SEL      |
 | | bit  12 R/W  THREE_FRMSSM   |
 | | bit  5  R/W  LOSSPEN        |
 | | bit  3  R/W  B2MASKEN       |
 | | bit  1  R/W  B1MASKEN       |
 | | bit  0  R/W  M0EN           |
 | +-----------------------------+
 | 
 | void   rrmp_field_DESCRAMA1A2SEL_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_DESCRAMA1A2SEL_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_DESCRAMJ0SEL_set   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_DESCRAMJ0SEL_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_FRM768SEL_set      ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_FRM768SEL_get      ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_THREE_FRMSSM_set   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_THREE_FRMSSM_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_LOSSPEN_set        ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_LOSSPEN_get        ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_B2MASKEN_set       ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_B2MASKEN_get       ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_B1MASKEN_set       ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_B1MASKEN_get       ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_M0EN_set           ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_M0EN_get           ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------+
 | | Register 0x00000108 STAT |
 | +--------------------------+
 | | bit  5 R  APSBF_V        |
 | | bit  4 R  LRDI_V         |
 | | bit  3 R  LAIS_V         |
 | | bit  2 R  LOS_V          |
 | | bit  1 R  LOF_V          |
 | | bit  0 R  OOF_V          |
 | +--------------------------+
 | 
 | UINT32 rrmp_field_APSBF_V_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 rrmp_field_LRDI_V_get  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 rrmp_field_LAIS_V_get  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 rrmp_field_LOS_V_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 rrmp_field_LOF_V_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 rrmp_field_OOF_V_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------+
 | | Register 0x0000010c INT_EN |
 | +----------------------------+
 | | bit  11 R/W  COK2_E        |
 | | bit  10 R/W  LREIE_E       |
 | | bit  9  R/W  LBIPE_E       |
 | | bit  8  R/W  SBIPE_E       |
 | | bit  7  R/W  COSSM_E       |
 | | bit  6  R/W  COAPS_E       |
 | | bit  5  R/W  APSBF_E       |
 | | bit  4  R/W  LRDI_E        |
 | | bit  3  R/W  LAIS_E        |
 | | bit  2  R/W  LOS_E         |
 | | bit  1  R/W  LOF_E         |
 | | bit  0  R/W  OOF_E         |
 | +----------------------------+
 | 
 | void   rrmp_field_COK2_E_set  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_COK2_E_get  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_LREIE_E_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_LREIE_E_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_LBIPE_E_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_LBIPE_E_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_SBIPE_E_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_SBIPE_E_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_COSSM_E_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_COSSM_E_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_COAPS_E_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_COAPS_E_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_APSBF_E_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_APSBF_E_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_LRDI_E_set  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_LRDI_E_get  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_LAIS_E_set  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_LAIS_E_get  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_LOS_E_set   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_LOS_E_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_LOF_E_set   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_LOF_E_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_OOF_E_set   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_OOF_E_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------+
 | | Register 0x00000110 INT_STAT |
 | +------------------------------+
 | | bit  11 R/W  COK2_I          |
 | | bit  10 R/W  LREIE_I         |
 | | bit  9  R/W  LBIPE_I         |
 | | bit  8  R/W  SBIPE_I         |
 | | bit  7  R/W  COSSM_I         |
 | | bit  6  R/W  COAPS_I         |
 | | bit  5  R/W  APSBF_I         |
 | | bit  4  R/W  LRDI_I          |
 | | bit  3  R/W  LAIS_I          |
 | | bit  2  R/W  LOS_I           |
 | | bit  1  R/W  LOF_I           |
 | | bit  0  R/W  OOF_I           |
 | +------------------------------+
 | 
 | void   rrmp_field_COK2_I_set  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_COK2_I_get  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_LREIE_I_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_LREIE_I_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_LBIPE_I_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_LBIPE_I_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_SBIPE_I_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_SBIPE_I_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_COSSM_I_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_COSSM_I_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_COAPS_I_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_COAPS_I_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_APSBF_I_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_APSBF_I_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_LRDI_I_set  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_LRDI_I_get  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_LAIS_I_set  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_LAIS_I_get  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_LOS_I_set   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_LOS_I_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_LOF_I_set   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_LOF_I_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_OOF_I_set   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_OOF_I_get   ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------+
 | | Register 0x00000114 RX_APS |
 | +----------------------------+
 | | bit  15:8 R  K1V           |
 | | bit  7:0  R  K2V           |
 | +----------------------------+
 | 
 | UINT32 rrmp_field_K1V_get       ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 rrmp_field_range_K1V_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 rrmp_field_K2V_get       ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 rrmp_field_range_K2V_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------+
 | | Register 0x00000118 RXD_SSM |
 | +-----------------------------+
 | | bit  15  R/W  BYTESSM       |
 | | bit  14  R/W  FLTRSSM       |
 | | bit  7:0 R    SSMV          |
 | +-----------------------------+
 | 
 | void   rrmp_field_BYTESSM_set    ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_BYTESSM_get    ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void   rrmp_field_FLTRSSM_set    ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 rrmp_field_FLTRSSM_get    ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 rrmp_field_SSMV_get       ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 rrmp_field_range_SSMV_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00000120 SECTION_BIP_ERR_CNT |
 | +-----------------------------------------+
 | | bit  15:0 R  SBIPE                      |
 | +-----------------------------------------+
 | 
 | UINT32 rrmp_field_SBIPE_get       ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 rrmp_field_range_SBIPE_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000124 LINE_BIP_ERR_CNT_LSB |
 | +------------------------------------------+
 | | bit  15:0 R  LBIPE                       |
 | +------------------------------------------+
 | 
 | UINT32 rrmp_field_LBIPE_get       ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 rrmp_field_range_LBIPE_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0000012c LINE_REI_ERR_CNT_LSB |
 | +------------------------------------------+
 | | bit  15:0 R  LREIE                       |
 | +------------------------------------------+
 | 
 | UINT32 rrmp_field_LREIE_get       ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 rrmp_field_range_LREIE_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00000134 RXD_J0_BYTE |
 | +---------------------------------+
 | | bit  7:0 R  J0V                 |
 | +---------------------------------+
 | 
 | UINT32 rrmp_field_J0V_get       ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | UINT32 rrmp_field_range_J0V_get ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _RRMP_IO_H
#define _RRMP_IO_H

#include "sdh_pmg_api.h"
#include "rrmp_regs.h"
#include "rrmp_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 rrmp_buffer_init               ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | void                 rrmp_buffer_flush              ( rrmp_buffer_t *b_ptr )
 | UINT32               rrmp_reg_read                  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 rrmp_reg_write                 ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 rrmp_field_set                 ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 rrmp_action_on_write_field_set ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 rrmp_burst_read                ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 rrmp_burst_write               ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE rrmp_poll                      ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 rrmp_reg_CFG_write                 ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 rrmp_reg_CFG_field_set             ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               rrmp_reg_CFG_read                  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE rrmp_reg_CFG_poll                  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 rrmp_reg_CFG_2_write               ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 rrmp_reg_CFG_2_field_set           ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               rrmp_reg_CFG_2_read                ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | void                 rrmp_reg_INT_EN_write              ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 rrmp_reg_INT_EN_field_set          ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               rrmp_reg_INT_EN_read               ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | void                 rrmp_reg_RXD_SSM_write             ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 rrmp_reg_RXD_SSM_field_set         ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               rrmp_reg_RXD_SSM_read              ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               rrmp_reg_STAT_read                 ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | void                 rrmp_reg_INT_STAT_write            ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 rrmp_reg_INT_STAT_field_set        ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               rrmp_reg_INT_STAT_read             ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               rrmp_reg_RX_APS_read               ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               rrmp_reg_SECTION_BIP_ERR_CNT_read  ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               rrmp_reg_LINE_BIP_ERR_CNT_LSB_read ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               rrmp_reg_LINE_BIP_ERR_CNT_MSB_read ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               rrmp_reg_LINE_REI_ERR_CNT_LSB_read ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               rrmp_reg_LINE_REI_ERR_CNT_MSB_read ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               rrmp_reg_RXD_J0_BYTE_read          ( rrmp_buffer_t *b_ptr, sdh_pmg_handle_t *h_ptr, UINT32 A )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _RRMP_IO_H */
