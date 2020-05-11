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
 *     and register accessor functions for the otu_frm_tx block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing otu_frm_tx io functions is:
 | {
 |     otu_frm_tx_buffer_t b_ptr[1];
 |     otu_frm_tx_buffer_init( b_ptr, h_ptr );
 |     value = otu_frm_tx_field_<FIELD>_get( b_ptr, h_ptr );
 |     otu_frm_tx_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     otu_frm_tx_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = otu_frm_tx_field_<FIELD>_get( NULL, h_ptr );
 | otu_frm_tx_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------------------------------------+
 | | Register (0x00003000 + (N) * 0x100 + (M) * 0x04) OTU_SO_SM_TTI |
 | |     N[0..11]                                                   |
 | |     M[0..15]                                                   |
 | +----------------------------------------------------------------+
 | | bit  31:0 R/W  OTU_SO_SM_TTI                                   |
 | +----------------------------------------------------------------+
 | 
 | void   otu_frm_tx_lfield_OTU_SO_SM_TTI_set       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value[16] )
 | void   otu_frm_tx_lfield_OTU_SO_SM_TTI_get       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value[16] )
 | void   otu_frm_tx_lfield_range_OTU_SO_SM_TTI_set ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_lfield_range_OTU_SO_SM_TTI_get ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00003040 + (N) * 0x100) OTU_SO_MFAS_SM |
 | |     N[0..11]                                       |
 | +----------------------------------------------------+
 | | bit  23:16 R/W  OTU_SO_MFAS                        |
 | | bit  15:8  R/W  OTU_SO_SM_BIP8                     |
 | | bit  7:4   R/W  OTU_SO_SM_BEI_BIAE                 |
 | | bit  3     R/W  OTU_SO_SM_BDI                      |
 | | bit  2     R/W  OTU_SO_SM_IAE                      |
 | | bit  1     R/W  OTU_SO_SM_RES7                     |
 | | bit  0     R/W  OTU_SO_SM_RES8                     |
 | +----------------------------------------------------+
 | 
 | void   otu_frm_tx_field_OTU_SO_MFAS_set              ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_MFAS_get              ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_range_OTU_SO_MFAS_set        ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_MFAS_get        ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_tx_field_OTU_SO_SM_BIP8_set           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_BIP8_get           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_range_OTU_SO_SM_BIP8_set     ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_SM_BIP8_get     ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_set       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_get       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_set ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_get ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_tx_field_OTU_SO_SM_BDI_set            ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_BDI_get            ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_OTU_SO_SM_IAE_set            ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_IAE_get            ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_OTU_SO_SM_RES7_set           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_RES7_get           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_OTU_SO_SM_RES8_set           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_RES8_get           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00003044 + (N) * 0x100) OTU_SO_GCC0_RES |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  31:16 R/W  OTU_SO_GCC0                         |
 | | bit  15:8  R/W  OTU_SO_RES13                        |
 | | bit  7:0   R/W  OTU_SO_RES14                        |
 | +-----------------------------------------------------+
 | 
 | void   otu_frm_tx_field_OTU_SO_GCC0_set        ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_GCC0_get        ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_range_OTU_SO_GCC0_set  ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_GCC0_get  ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_tx_field_OTU_SO_RES13_set       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_RES13_get       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_range_OTU_SO_RES13_set ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_RES13_get ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_tx_field_OTU_SO_RES14_set       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_RES14_get       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_range_OTU_SO_RES14_set ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_RES14_get ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00003048 + (N) * 0x100) OTU_SO_SM_INS_CTRL |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  19    R/W  OTU_SO_FAS_INS_CTRL                    |
 | | bit  18:17 R/W  OTU_SO_MFAS_INS_CTRL                   |
 | | bit  16    R/W  OTU_SO_SM_TTI_SAPI_INS_CTRL            |
 | | bit  15    R/W  OTU_SO_SM_TTI_DAPI_INS_CTRL            |
 | | bit  14    R/W  OTU_SO_SM_TTI_OP_INS_CTRL              |
 | | bit  13:11 R/W  OTU_SO_SM_BIP8_INS_CTRL                |
 | | bit  10:9  R/W  OTU_SO_SM_BEI_BIAE_INS_CTRL            |
 | | bit  8:7   R/W  OTU_SO_SM_BDI_INS_CTRL                 |
 | | bit  6:5   R/W  OTU_SO_SM_IAE_INS_CTRL                 |
 | | bit  4     R/W  OTU_SO_SM_RES7_INS_CTRL                |
 | | bit  3     R/W  OTU_SO_SM_RES8_INS_CTRL                |
 | | bit  2     R/W  OTU_SO_GCC0_INS_CTRL                   |
 | | bit  1     R/W  OTU_SO_RES13_INS_CTRL                  |
 | | bit  0     R/W  OTU_SO_RES14_INS_CTRL                  |
 | +--------------------------------------------------------+
 | 
 | void   otu_frm_tx_field_OTU_SO_FAS_INS_CTRL_set               ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_FAS_INS_CTRL_get               ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_OTU_SO_MFAS_INS_CTRL_set              ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_MFAS_INS_CTRL_get              ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_range_OTU_SO_MFAS_INS_CTRL_set        ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_MFAS_INS_CTRL_get        ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_tx_field_OTU_SO_SM_TTI_SAPI_INS_CTRL_set       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_TTI_SAPI_INS_CTRL_get       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_OTU_SO_SM_TTI_DAPI_INS_CTRL_set       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_TTI_DAPI_INS_CTRL_get       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_OTU_SO_SM_TTI_OP_INS_CTRL_set         ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_TTI_OP_INS_CTRL_get         ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_OTU_SO_SM_BIP8_INS_CTRL_set           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_BIP8_INS_CTRL_get           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_range_OTU_SO_SM_BIP8_INS_CTRL_set     ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_SM_BIP8_INS_CTRL_get     ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_INS_CTRL_set       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_BEI_BIAE_INS_CTRL_get       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_INS_CTRL_set ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_SM_BEI_BIAE_INS_CTRL_get ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_tx_field_OTU_SO_SM_BDI_INS_CTRL_set            ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_BDI_INS_CTRL_get            ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_range_OTU_SO_SM_BDI_INS_CTRL_set      ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_SM_BDI_INS_CTRL_get      ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_tx_field_OTU_SO_SM_IAE_INS_CTRL_set            ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_IAE_INS_CTRL_get            ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_range_OTU_SO_SM_IAE_INS_CTRL_set      ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_SM_IAE_INS_CTRL_get      ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_tx_field_OTU_SO_SM_RES7_INS_CTRL_set           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_RES7_INS_CTRL_get           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_OTU_SO_SM_RES8_INS_CTRL_set           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_SM_RES8_INS_CTRL_get           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_OTU_SO_GCC0_INS_CTRL_set              ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_GCC0_INS_CTRL_get              ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_OTU_SO_RES13_INS_CTRL_set             ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_RES13_INS_CTRL_get             ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_OTU_SO_RES14_INS_CTRL_set             ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_RES14_INS_CTRL_get             ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------+
 | | Register (0x0000304c + (N) * 0x100) OTU_SO_CTRL |
 | |     N[0..11]                                    |
 | +-------------------------------------------------+
 | | bit  2:1 R/W  OTU_SO_MAINT_SIG                  |
 | | bit  0   R/W  OTU_SO_MI_ADMINSTATE              |
 | +-------------------------------------------------+
 | 
 | void   otu_frm_tx_field_OTU_SO_MAINT_SIG_set       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_MAINT_SIG_get       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_tx_field_range_OTU_SO_MAINT_SIG_set ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_MAINT_SIG_get ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_tx_field_OTU_SO_MI_ADMINSTATE_set   ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_MI_ADMINSTATE_get   ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00003050 + (N) * 0x100) OTU_SO_MASTER_CTRL |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  0 R/W  OTU_SO_PASSTHRU                            |
 | +--------------------------------------------------------+
 | 
 | void   otu_frm_tx_field_OTU_SO_PASSTHRU_set ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_PASSTHRU_get ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------+
 | | Register 0x00003f04 OTU_SO_FAS_F6 |
 | +-----------------------------------+
 | | bit  23:0 R/W  OTU_SO_FAS_F6      |
 | +-----------------------------------+
 | 
 | void   otu_frm_tx_field_OTU_SO_FAS_F6_set       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_FAS_F6_get       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_tx_field_range_OTU_SO_FAS_F6_set ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_FAS_F6_get ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00003f08 OTU_SO_FAS_28 |
 | +-----------------------------------+
 | | bit  23:0 R/W  OTU_SO_FAS_28      |
 | +-----------------------------------+
 | 
 | void   otu_frm_tx_field_OTU_SO_FAS_28_set       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_FAS_28_get       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_tx_field_range_OTU_SO_FAS_28_set ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_FAS_28_get ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00003f40 OTU_SO_A_IAE_E |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SO_A_IAE_E      |
 | +------------------------------------+
 | 
 | void   otu_frm_tx_field_OTU_SO_A_IAE_E_set       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_tx_field_OTU_SO_A_IAE_E_get       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_tx_field_range_OTU_SO_A_IAE_E_set ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_tx_field_range_OTU_SO_A_IAE_E_get ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00003f80 OTU_SO_A_IAE_I |
 | +------------------------------------+
 | | bit  11:0 R/W  OTU_SO_A_IAE_I      |
 | +------------------------------------+
 | 
 | void                 otu_frm_tx_field_OTU_SO_A_IAE_I_set_to_clear       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_tx_field_OTU_SO_A_IAE_I_get                ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_tx_field_range_OTU_SO_A_IAE_I_set_to_clear ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_tx_field_range_OTU_SO_A_IAE_I_get          ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_tx_field_range_OTU_SO_A_IAE_I_poll         ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_tx_field_OTU_SO_A_IAE_I_poll               ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00003fc0 OTU_SO_A_IAE_V |
 | +------------------------------------+
 | | bit  11:0 R  OTU_SO_A_IAE_V        |
 | +------------------------------------+
 | 
 | UINT32               otu_frm_tx_field_OTU_SO_A_IAE_V_get        ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_tx_field_range_OTU_SO_A_IAE_V_get  ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_tx_field_range_OTU_SO_A_IAE_V_poll ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_tx_field_OTU_SO_A_IAE_V_poll       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _OTU_FRM_TX_IO_H
#define _OTU_FRM_TX_IO_H

#include "pmc_sys.h"
#include "pmc_types.h"
#include "otu_frm_tx_regs.h"
#include "otu_frm_tx_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 otu_frm_tx_buffer_init               ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_tx_buffer_flush              ( otu_frm_tx_buffer_t *b_ptr )
 | UINT32               otu_frm_tx_reg_read                  ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 otu_frm_tx_reg_write                 ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 otu_frm_tx_field_set                 ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 otu_frm_tx_action_on_write_field_set ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 otu_frm_tx_burst_read                ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 otu_frm_tx_burst_write               ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE otu_frm_tx_poll                      ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 otu_frm_tx_reg_OTU_SO_FAS_F6_write                      ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_tx_reg_OTU_SO_FAS_F6_field_set                  ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_tx_reg_OTU_SO_FAS_F6_read                       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_tx_reg_OTU_SO_FAS_28_write                      ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_tx_reg_OTU_SO_FAS_28_field_set                  ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_tx_reg_OTU_SO_FAS_28_read                       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_tx_reg_OTU_SO_CTRL_array_write                  ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otu_frm_tx_reg_OTU_SO_CTRL_array_field_set              ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_tx_reg_OTU_SO_CTRL_array_read                   ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otu_frm_tx_reg_OTU_SO_MASTER_CTRL_array_write           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otu_frm_tx_reg_OTU_SO_MASTER_CTRL_array_field_set       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_tx_reg_OTU_SO_MASTER_CTRL_array_read            ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otu_frm_tx_reg_OTU_SO_A_IAE_E_write                     ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_tx_reg_OTU_SO_A_IAE_E_field_set                 ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_tx_reg_OTU_SO_A_IAE_E_read                      ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_tx_reg_OTU_SO_A_IAE_I_write                     ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_tx_reg_OTU_SO_A_IAE_I_action_on_write_field_set ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_tx_reg_OTU_SO_A_IAE_I_read                      ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_tx_reg_OTU_SO_A_IAE_I_poll                      ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_tx_reg_OTU_SO_A_IAE_V_read                      ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_tx_reg_OTU_SO_A_IAE_V_poll                      ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_tx_reg_OTU_SO_MFAS_SM_array_write               ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otu_frm_tx_reg_OTU_SO_MFAS_SM_array_field_set           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_tx_reg_OTU_SO_MFAS_SM_array_read                ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otu_frm_tx_reg_OTU_SO_GCC0_RES_array_write              ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otu_frm_tx_reg_OTU_SO_GCC0_RES_array_field_set          ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_tx_reg_OTU_SO_GCC0_RES_array_read               ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_write           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_field_set       ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_tx_reg_OTU_SO_SM_INS_CTRL_array_read            ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otu_frm_tx_reg_OTU_SO_SM_TTI_array_burst_write          ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 otu_frm_tx_reg_OTU_SO_SM_TTI_array_field_set            ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 M, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 otu_frm_tx_reg_OTU_SO_SM_TTI_array_burst_read           ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               otu_frm_tx_reg_OTU_SO_SM_TTI_array_read                 ( otu_frm_tx_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 M )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _OTU_FRM_TX_IO_H */
