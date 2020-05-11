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
 *     and register accessor functions for the otu_frm block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing otu_frm io functions is:
 | {
 |     otu_frm_buffer_t b_ptr[1];
 |     otu_frm_buffer_init( b_ptr, h_ptr );
 |     value = otu_frm_field_<FIELD>_get( b_ptr, h_ptr );
 |     otu_frm_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     otu_frm_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = otu_frm_field_<FIELD>_get( NULL, h_ptr );
 | otu_frm_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------------+
 | | Register 0x00000000 OTU_FRM_MASTER_CFG |
 | +----------------------------------------+
 | | bit  31 R/W  OTU_FRM_SW_RESET          |
 | | bit  1  R/W  OTU_FRM_SW_RAM_LOWPWR     |
 | | bit  0  R/W  OTU_FRM_SW_LOWPWR         |
 | +----------------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_SW_RESET_set      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_SW_RESET_get      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_SW_RAM_LOWPWR_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_SW_RAM_LOWPWR_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_SW_LOWPWR_set     ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_SW_LOWPWR_get     ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x00000004 OTU_FRM_SW_PMON |
 | +-------------------------------------+
 | | bit  1 R/W  OTU_FRM_LCLK_EN         |
 | | bit  0 R/W  OTU_FRM_PMON_UPDATE     |
 | +-------------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_LCLK_EN_set     ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_LCLK_EN_get     ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_PMON_UPDATE_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_PMON_UPDATE_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | +--------------------------------------------+
 | | Register 0x0000000c OTU_FRM_SW_RESET       |
 | +--------------------------------------------+
 | | bit  13   R/W  OTU_FRM_SW_SYSCLK_RESET     |
 | | bit  12   R/W  OTU_FRM_SW_CLK355M_RESET    |
 | | bit  11:0 R/W  OTU_FRM_SW_RX_LINECLK_RESET |
 | +--------------------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_SW_SYSCLK_RESET_set           ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_SW_SYSCLK_RESET_get           ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_SW_CLK355M_RESET_set          ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_SW_CLK355M_RESET_get          ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_SW_RX_LINECLK_RESET_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_SW_RX_LINECLK_RESET_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_range_OTU_FRM_SW_RX_LINECLK_RESET_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_field_range_OTU_FRM_SW_RX_LINECLK_RESET_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x00000018 OTU_FRM_SF_SQUELCH_CNT_LIMIT |
 | +--------------------------------------------------+
 | | bit  31:0 R/W  OTU_FRM_SF_CNT_LIMIT              |
 | +--------------------------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_SF_CNT_LIMIT_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_SF_CNT_LIMIT_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_range_OTU_FRM_SF_CNT_LIMIT_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_field_range_OTU_FRM_SF_CNT_LIMIT_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x0000001c OTU_FRM_RX_LINECLK_MUX_CFG |
 | +------------------------------------------------+
 | | bit  22  R/W  OTU_FRM_RX_LINECLK_11_SRC2_EN    |
 | | bit  21  R/W  OTU_FRM_RX_LINECLK_11_SRC1_EN    |
 | | bit  20  R/W  OTU_FRM_RX_LINECLK_11_SRC0_EN    |
 | | bit  19  R/W  OTU_FRM_RX_LINECLK_7_SRC1_EN     |
 | | bit  18  R/W  OTU_FRM_RX_LINECLK_7_SRC0_EN     |
 | | bit  17  R/W  OTU_FRM_RX_LINECLK_3_SRC1_EN     |
 | | bit  16  R/W  OTU_FRM_RX_LINECLK_3_SRC0_EN     |
 | | bit  3:2 R/W  OTU_FRM_RX_LINECLK_11_SEL        |
 | | bit  1   R/W  OTU_FRM_RX_LINECLK_7_SEL         |
 | | bit  0   R/W  OTU_FRM_RX_LINECLK_3_SEL         |
 | +------------------------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC2_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC2_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC1_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC1_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC0_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_11_SRC0_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC1_EN_set    ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC1_EN_get    ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC0_EN_set    ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_7_SRC0_EN_get    ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC1_EN_set    ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC1_EN_get    ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC0_EN_set    ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_3_SRC0_EN_get    ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_RX_LINECLK_11_SEL_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_11_SEL_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_range_OTU_FRM_RX_LINECLK_11_SEL_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_field_range_OTU_FRM_RX_LINECLK_11_SEL_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_field_OTU_FRM_RX_LINECLK_7_SEL_set        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_7_SEL_get        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_RX_LINECLK_3_SEL_set        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_RX_LINECLK_3_SEL_get        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000040 + (N) * 0x04) OTU_FRM_10G_ENABLE |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  10 R/W  OTU_FRM_10G_SCRAMBLING_EN                |
 | | bit  9  R/W  OTU_FRM_10G_FEC_COL_INS_EN               |
 | | bit  8  R/W  OTU_FRM_10G_DINTLV_EN                    |
 | | bit  6  R/W  OTU_FRM_10G_MI_ACTIVE                    |
 | | bit  5  R/W  OTU_FRM_10G_DLOM_TO_SF_EN                |
 | | bit  4  R/W  OTU_FRM_10G_DLOM_EN                      |
 | | bit  3  R/W  OTU_FRM_10G_SF_SQUELCH_EN                |
 | | bit  2  R/W  OTU_FRM_10G_DESCRAMBLING_EN              |
 | | bit  1  R/W  OTU_FRM_10G_FEC_COL_RM_EN                |
 | | bit  0  R/W  OTU_FRM_10G_INTLV_EN                     |
 | +-------------------------------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_10G_SCRAMBLING_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_10G_SCRAMBLING_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_10G_FEC_COL_INS_EN_set  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_10G_FEC_COL_INS_EN_get  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_10G_DINTLV_EN_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_10G_DINTLV_EN_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_10G_MI_ACTIVE_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_10G_MI_ACTIVE_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_10G_DLOM_TO_SF_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_10G_DLOM_EN_set         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_10G_DLOM_EN_get         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_10G_SF_SQUELCH_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_10G_SF_SQUELCH_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_10G_DESCRAMBLING_EN_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_10G_DESCRAMBLING_EN_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_10G_FEC_COL_RM_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_10G_FEC_COL_RM_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_10G_INTLV_EN_set        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_10G_INTLV_EN_get        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000070 + (N) * 0x04) OTU_FRM_40G_ENABLE |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  10 R/W  OTU_FRM_40G_SCRAMBLING_EN                |
 | | bit  9  R/W  OTU_FRM_40G_FEC_COL_INS_EN               |
 | | bit  8  R/W  OTU_FRM_40G_DINTLV_EN                    |
 | | bit  6  R/W  OTU_FRM_40G_MI_ACTIVE                    |
 | | bit  5  R/W  OTU_FRM_40G_DLOM_TO_SF_EN                |
 | | bit  4  R/W  OTU_FRM_40G_DLOM_EN                      |
 | | bit  3  R/W  OTU_FRM_40G_SF_SQUELCH_EN                |
 | | bit  2  R/W  OTU_FRM_40G_DESCRAMBLING_EN              |
 | | bit  1  R/W  OTU_FRM_40G_FEC_COL_RM_EN                |
 | | bit  0  R/W  OTU_FRM_40G_INTLV_EN                     |
 | +-------------------------------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_40G_SCRAMBLING_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_40G_SCRAMBLING_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_40G_FEC_COL_INS_EN_set  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_40G_FEC_COL_INS_EN_get  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_40G_DINTLV_EN_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_40G_DINTLV_EN_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_40G_MI_ACTIVE_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_40G_MI_ACTIVE_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_40G_DLOM_TO_SF_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_40G_DLOM_EN_set         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_40G_DLOM_EN_get         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_40G_SF_SQUELCH_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_40G_SF_SQUELCH_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_40G_DESCRAMBLING_EN_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_40G_DESCRAMBLING_EN_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_40G_FEC_COL_RM_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_40G_FEC_COL_RM_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_OTU_FRM_40G_INTLV_EN_set        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_40G_INTLV_EN_get        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------+
 | | Register 0x0000007c OTU_FRM_100G_ENABLE   |
 | +-------------------------------------------+
 | | bit  10 R/W  OTU_FRM_100G_SCRAMBLING_EN   |
 | | bit  9  R/W  OTU_FRM_100G_FEC_COL_INS_EN  |
 | | bit  8  R/W  OTU_FRM_100G_DINTLV_EN       |
 | | bit  6  R/W  OTU_FRM_100G_MI_ACTIVE       |
 | | bit  5  R/W  OTU_FRM_100G_DLOM_TO_SF_EN   |
 | | bit  4  R/W  OTU_FRM_100G_DLOM_EN         |
 | | bit  3  R/W  OTU_FRM_100G_SF_SQUELCH_EN   |
 | | bit  2  R/W  OTU_FRM_100G_DESCRAMBLING_EN |
 | | bit  1  R/W  OTU_FRM_100G_FEC_COL_RM_EN   |
 | | bit  0  R/W  OTU_FRM_100G_INTLV_EN        |
 | +-------------------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_100G_SCRAMBLING_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_100G_SCRAMBLING_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_100G_FEC_COL_INS_EN_set  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_100G_FEC_COL_INS_EN_get  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_100G_DINTLV_EN_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_100G_DINTLV_EN_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_100G_MI_ACTIVE_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_100G_MI_ACTIVE_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_100G_DLOM_TO_SF_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_100G_DLOM_TO_SF_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_100G_DLOM_EN_set         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_100G_DLOM_EN_get         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_100G_SF_SQUELCH_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_100G_SF_SQUELCH_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_100G_DESCRAMBLING_EN_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_100G_DESCRAMBLING_EN_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_100G_FEC_COL_RM_EN_set   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_100G_FEC_COL_RM_EN_get   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_OTU_FRM_100G_INTLV_EN_set        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_100G_INTLV_EN_get        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00000080 + (N) * 0x04) OTU_FRM_STREAM_CFG |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  24:16 R/W  OTU_FRM_DINTLV_XOFF_THRES             |
 | | bit  6:0   R/W  OTU_FRM_CH_ID                         |
 | +-------------------------------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_DINTLV_XOFF_THRES_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_DINTLV_XOFF_THRES_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_range_OTU_FRM_DINTLV_XOFF_THRES_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_field_range_OTU_FRM_DINTLV_XOFF_THRES_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   otu_frm_field_OTU_FRM_CH_ID_set                   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_CH_ID_get                   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | void   otu_frm_field_range_OTU_FRM_CH_ID_set             ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_field_range_OTU_FRM_CH_ID_get             ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x000000e8 OTU_FRM_SPARE |
 | +-----------------------------------+
 | | bit  31:0 R/W  OTU_FRM_SPARE      |
 | +-----------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_SPARE_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_SPARE_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_range_OTU_FRM_SPARE_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_field_range_OTU_FRM_SPARE_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x000000fc OTU_FRM_INT_SUMMARY |
 | +-----------------------------------------+
 | | bit  29:0 R  OTU_FRM_INT_SUMMARY        |
 | +-----------------------------------------+
 | 
 | UINT32 otu_frm_field_OTU_FRM_INT_SUMMARY_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32 otu_frm_field_range_OTU_FRM_INT_SUMMARY_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000100 OTU_FRM_INTLV_WOVR_E |
 | +------------------------------------------+
 | | bit  11:0 R/W  OTU_FRM_INTLV_WOVR_E      |
 | +------------------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_INTLV_WOVR_E_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_INTLV_WOVR_E_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_range_OTU_FRM_INTLV_WOVR_E_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_field_range_OTU_FRM_INTLV_WOVR_E_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000104 OTU_FRM_INTLV_RUDR_E |
 | +------------------------------------------+
 | | bit  11:0 R/W  OTU_FRM_INTLV_RUDR_E      |
 | +------------------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_INTLV_RUDR_E_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_INTLV_RUDR_E_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_range_OTU_FRM_INTLV_RUDR_E_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_field_range_OTU_FRM_INTLV_RUDR_E_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00000108 OTU_FRM_DINTLV_WOVR_E |
 | +-------------------------------------------+
 | | bit  11:0 R/W  OTU_FRM_DINTLV_WOVR_E      |
 | +-------------------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_DINTLV_WOVR_E_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_DINTLV_WOVR_E_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_range_OTU_FRM_DINTLV_WOVR_E_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_field_range_OTU_FRM_DINTLV_WOVR_E_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0000010c OTU_FRM_DINTLV_RUDR_E |
 | +-------------------------------------------+
 | | bit  11:0 R/W  OTU_FRM_DINTLV_RUDR_E      |
 | +-------------------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_DINTLV_RUDR_E_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_DINTLV_RUDR_E_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_range_OTU_FRM_DINTLV_RUDR_E_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_field_range_OTU_FRM_DINTLV_RUDR_E_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000110 OTU_FRM_D_LOM_E |
 | +-------------------------------------+
 | | bit  11:0 R/W  OTU_FRM_D_LOM_E      |
 | +-------------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_D_LOM_E_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_D_LOM_E_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_range_OTU_FRM_D_LOM_E_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_field_range_OTU_FRM_D_LOM_E_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000114 OTU_FRM_OOM_E |
 | +-----------------------------------+
 | | bit  11:0 R/W  OTU_FRM_OOM_E      |
 | +-----------------------------------+
 | 
 | void   otu_frm_field_OTU_FRM_OOM_E_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32 otu_frm_field_OTU_FRM_OOM_E_get       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void   otu_frm_field_range_OTU_FRM_OOM_E_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 otu_frm_field_range_OTU_FRM_OOM_E_get ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00000120 OTU_FRM_INTLV_WOVR_I |
 | +------------------------------------------+
 | | bit  11:0 R/W  OTU_FRM_INTLV_WOVR_I      |
 | +------------------------------------------+
 | 
 | void                 otu_frm_field_OTU_FRM_INTLV_WOVR_I_set_to_clear       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_field_OTU_FRM_INTLV_WOVR_I_get                ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_set_to_clear ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_get          ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_INTLV_WOVR_I_poll         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_INTLV_WOVR_I_poll               ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x00000124 OTU_FRM_INTLV_RUDR_I |
 | +------------------------------------------+
 | | bit  11:0 R/W  OTU_FRM_INTLV_RUDR_I      |
 | +------------------------------------------+
 | 
 | void                 otu_frm_field_OTU_FRM_INTLV_RUDR_I_set_to_clear       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_field_OTU_FRM_INTLV_RUDR_I_get                ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_set_to_clear ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_get          ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_INTLV_RUDR_I_poll         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_INTLV_RUDR_I_poll               ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x00000128 OTU_FRM_DINTLV_WOVR_I |
 | +-------------------------------------------+
 | | bit  11:0 R/W  OTU_FRM_DINTLV_WOVR_I      |
 | +-------------------------------------------+
 | 
 | void                 otu_frm_field_OTU_FRM_DINTLV_WOVR_I_set_to_clear       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_field_OTU_FRM_DINTLV_WOVR_I_get                ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_set_to_clear ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_get          ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_DINTLV_WOVR_I_poll         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_DINTLV_WOVR_I_poll               ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x0000012c OTU_FRM_DINTLV_RUDR_I |
 | +-------------------------------------------+
 | | bit  11:0 R/W  OTU_FRM_DINTLV_RUDR_I      |
 | +-------------------------------------------+
 | 
 | void                 otu_frm_field_OTU_FRM_DINTLV_RUDR_I_set_to_clear       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_field_OTU_FRM_DINTLV_RUDR_I_get                ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_set_to_clear ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_get          ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_DINTLV_RUDR_I_poll         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_DINTLV_RUDR_I_poll               ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00000130 OTU_FRM_D_LOM_I |
 | +-------------------------------------+
 | | bit  11:0 R/W  OTU_FRM_D_LOM_I      |
 | +-------------------------------------+
 | 
 | void                 otu_frm_field_OTU_FRM_D_LOM_I_set_to_clear       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_field_OTU_FRM_D_LOM_I_get                ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_field_range_OTU_FRM_D_LOM_I_set_to_clear ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_field_range_OTU_FRM_D_LOM_I_get          ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_D_LOM_I_poll         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_D_LOM_I_poll               ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00000134 OTU_FRM_OOM_I |
 | +-----------------------------------+
 | | bit  11:0 R/W  OTU_FRM_OOM_I      |
 | +-----------------------------------+
 | 
 | void                 otu_frm_field_OTU_FRM_OOM_I_set_to_clear       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | UINT32               otu_frm_field_OTU_FRM_OOM_I_get                ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_field_range_OTU_FRM_OOM_I_set_to_clear ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               otu_frm_field_range_OTU_FRM_OOM_I_get          ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_OOM_I_poll         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_OOM_I_poll               ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x00000160 OTU_FRM_INTLV_WOVR_V |
 | +------------------------------------------+
 | | bit  11:0 R  OTU_FRM_INTLV_WOVR_V        |
 | +------------------------------------------+
 | 
 | UINT32               otu_frm_field_OTU_FRM_INTLV_WOVR_V_get        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_field_range_OTU_FRM_INTLV_WOVR_V_get  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_INTLV_WOVR_V_poll ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_INTLV_WOVR_V_poll       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x00000164 OTU_FRM_INTLV_RUDR_V |
 | +------------------------------------------+
 | | bit  11:0 R  OTU_FRM_INTLV_RUDR_V        |
 | +------------------------------------------+
 | 
 | UINT32               otu_frm_field_OTU_FRM_INTLV_RUDR_V_get        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_field_range_OTU_FRM_INTLV_RUDR_V_get  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_INTLV_RUDR_V_poll ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_INTLV_RUDR_V_poll       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x00000168 OTU_FRM_DINTLV_WOVR_V |
 | +-------------------------------------------+
 | | bit  11:0 R  OTU_FRM_DINTLV_WOVR_V        |
 | +-------------------------------------------+
 | 
 | UINT32               otu_frm_field_OTU_FRM_DINTLV_WOVR_V_get        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_field_range_OTU_FRM_DINTLV_WOVR_V_get  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_DINTLV_WOVR_V_poll ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_DINTLV_WOVR_V_poll       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x0000016c OTU_FRM_DINTLV_RUDR_V |
 | +-------------------------------------------+
 | | bit  11:0 R  OTU_FRM_DINTLV_RUDR_V        |
 | +-------------------------------------------+
 | 
 | UINT32               otu_frm_field_OTU_FRM_DINTLV_RUDR_V_get        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_field_range_OTU_FRM_DINTLV_RUDR_V_get  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_DINTLV_RUDR_V_poll ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_DINTLV_RUDR_V_poll       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00000170 OTU_FRM_D_LOM_V |
 | +-------------------------------------+
 | | bit  11:0 R  OTU_FRM_D_LOM_V        |
 | +-------------------------------------+
 | 
 | UINT32               otu_frm_field_OTU_FRM_D_LOM_V_get        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_field_range_OTU_FRM_D_LOM_V_get  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_D_LOM_V_poll ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_D_LOM_V_poll       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------+
 | | Register 0x00000174 OTU_FRM_OOM_V |
 | +-----------------------------------+
 | | bit  11:0 R  OTU_FRM_OOM_V        |
 | +-----------------------------------+
 | 
 | UINT32               otu_frm_field_OTU_FRM_OOM_V_get        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | UINT32               otu_frm_field_range_OTU_FRM_OOM_V_get  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE otu_frm_field_range_OTU_FRM_OOM_V_poll ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE otu_frm_field_OTU_FRM_OOM_V_poll       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _OTU_FRM_IO_H
#define _OTU_FRM_IO_H

#include "otu_frm_loc.h"
#include "pmc_sys.h"
#include "pmc_types.h"
#include "otu_frm.h"
#include "otu_frm_regs.h"
#include "otu_frm_io_inline.h"


/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 otu_frm_buffer_init               ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | void                 otu_frm_buffer_flush              ( otu_frm_buffer_t *b_ptr )
 | UINT32               otu_frm_reg_read                  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 otu_frm_reg_write                 ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 otu_frm_field_set                 ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 otu_frm_action_on_write_field_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 otu_frm_burst_read                ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 otu_frm_burst_write               ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE otu_frm_poll                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 otu_frm_reg_OTU_FRM_MASTER_CFG_write                        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_MASTER_CFG_field_set                    ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_MASTER_CFG_read                         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_reg_OTU_FRM_SW_PMON_write                           ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_SW_PMON_field_set                       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_SW_PMON_read                            ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_reg_OTU_FRM_SW_RESET_write                          ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_SW_RESET_field_set                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_SW_RESET_read                           ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_write              ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_field_set          ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_SF_SQUELCH_CNT_LIMIT_read               ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_write                ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_field_set            ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_RX_LINECLK_MUX_CFG_read                 ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_reg_OTU_FRM_100G_ENABLE_write                       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_100G_ENABLE_field_set                   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_100G_ENABLE_read                        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_reg_OTU_FRM_SPARE_write                             ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_SPARE_field_set                         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_SPARE_read                              ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_reg_OTU_FRM_10G_ENABLE_array_write                  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_10G_ENABLE_array_field_set              ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_10G_ENABLE_array_read                   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otu_frm_reg_OTU_FRM_STREAM_CFG_array_write                  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_STREAM_CFG_array_field_set              ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_STREAM_CFG_array_read                   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otu_frm_reg_OTU_FRM_40G_ENABLE_array_write                  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_40G_ENABLE_array_field_set              ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_40G_ENABLE_array_read                   ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32  N )
 | 
 | void                 otu_frm_reg_OTU_FRM_INTLV_WOVR_E_write                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_INTLV_WOVR_E_field_set                  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_INTLV_WOVR_E_read                       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_reg_OTU_FRM_INTLV_RUDR_E_write                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_INTLV_RUDR_E_field_set                  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_INTLV_RUDR_E_read                       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_write                     ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_field_set                 ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_DINTLV_WOVR_E_read                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_write                     ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_field_set                 ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_DINTLV_RUDR_E_read                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_reg_OTU_FRM_D_LOM_E_write                           ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_D_LOM_E_field_set                       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_D_LOM_E_read                            ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_reg_OTU_FRM_OOM_E_write                             ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_OOM_E_field_set                         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_OOM_E_read                              ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 | void                 otu_frm_reg_OTU_FRM_INTLV_WOVR_I_write                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_INTLV_WOVR_I_action_on_write_field_set  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_INTLV_WOVR_I_read                       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_INTLV_WOVR_I_poll                       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_reg_OTU_FRM_INTLV_RUDR_I_write                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_INTLV_RUDR_I_action_on_write_field_set  ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_INTLV_RUDR_I_read                       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_INTLV_RUDR_I_poll                       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_write                     ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_action_on_write_field_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_read                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_DINTLV_WOVR_I_poll                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_write                     ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_action_on_write_field_set ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_read                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_DINTLV_RUDR_I_poll                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_reg_OTU_FRM_D_LOM_I_write                           ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_D_LOM_I_action_on_write_field_set       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_D_LOM_I_read                            ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_D_LOM_I_poll                            ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 otu_frm_reg_OTU_FRM_OOM_I_write                             ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 value )
 | void                 otu_frm_reg_OTU_FRM_OOM_I_action_on_write_field_set         ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               otu_frm_reg_OTU_FRM_OOM_I_read                              ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_OOM_I_poll                              ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_reg_OTU_FRM_INTLV_WOVR_V_read                       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_INTLV_WOVR_V_poll                       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_reg_OTU_FRM_INTLV_RUDR_V_read                       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_INTLV_RUDR_V_poll                       ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_reg_OTU_FRM_DINTLV_WOVR_V_read                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_DINTLV_WOVR_V_poll                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_reg_OTU_FRM_DINTLV_RUDR_V_read                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_DINTLV_RUDR_V_poll                      ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_reg_OTU_FRM_D_LOM_V_read                            ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_D_LOM_V_poll                            ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_reg_OTU_FRM_OOM_V_read                              ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE otu_frm_reg_OTU_FRM_OOM_V_poll                              ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               otu_frm_reg_OTU_FRM_INT_SUMMARY_read                        ( otu_frm_buffer_t *b_ptr, otu_frm_handle_t *h_ptr )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _OTU_FRM_IO_H */
