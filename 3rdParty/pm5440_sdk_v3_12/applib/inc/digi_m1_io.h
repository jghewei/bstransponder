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
 *     and register accessor functions for the digi_m1 block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing digi_m1 io functions is:
 | {
 |     digi_m1_buffer_t b_ptr[1];
 |     digi_m1_buffer_init( b_ptr, h_ptr );
 |     value = digi_m1_field_<FIELD>_get( b_ptr, h_ptr );
 |     digi_m1_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     digi_m1_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = digi_m1_field_<FIELD>_get( NULL, h_ptr );
 | digi_m1_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------+
 | | Register 0x00016000 SUBSYSTEM_SW_RESETS |
 | +-----------------------------------------+
 | | bit  7 R/W  FEGE_SW_RST                 |
 | | bit  6 R/W  PGMRCLK_SW_RST              |
 | | bit  5 R/W  TOC_CAL_349_SW_RST          |
 | | bit  4 R/W  TOC_CAL_500_SW_RST          |
 | | bit  3 R/W  MISC_SW_RST                 |
 | +-----------------------------------------+
 | 
 | void   digi_m1_field_FEGE_SW_RST_set        ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_FEGE_SW_RST_get        ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_PGMRCLK_SW_RST_set     ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_PGMRCLK_SW_RST_get     ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_TOC_CAL_349_SW_RST_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_TOC_CAL_349_SW_RST_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_TOC_CAL_500_SW_RST_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_TOC_CAL_500_SW_RST_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_MISC_SW_RST_set        ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_MISC_SW_RST_get        ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +-----------------------------------------+
 | | Register 0x00016004 PLL_SUPPORT_MUX_CFG |
 | +-----------------------------------------+
 | | bit  8   R/W  LINE_TIMING_REF_SEL       |
 | | bit  6:4 R/W  LINE_REF_CLK_SEL          |
 | | bit  2:0 R/W  PTP_DCSU_REF_SEL          |
 | +-----------------------------------------+
 | 
 | void   digi_m1_field_LINE_TIMING_REF_SEL_set    ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_LINE_TIMING_REF_SEL_get    ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_LINE_REF_CLK_SEL_set       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_LINE_REF_CLK_SEL_get       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_LINE_REF_CLK_SEL_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_LINE_REF_CLK_SEL_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_m1_field_PTP_DCSU_REF_SEL_set       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_PTP_DCSU_REF_SEL_get       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_PTP_DCSU_REF_SEL_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_PTP_DCSU_REF_SEL_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00016008 PLL_SUPPORT_PMM_CFG_0 |
 | +-------------------------------------------+
 | | bit  29:16 R/W  PMM_DSM_DENOM             |
 | | bit  15:0  R/W  PMM_PKT_PERIOD            |
 | +-------------------------------------------+
 | 
 | void   digi_m1_field_PMM_DSM_DENOM_set        ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_PMM_DSM_DENOM_get        ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_PMM_DSM_DENOM_set  ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_PMM_DSM_DENOM_get  ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_m1_field_PMM_PKT_PERIOD_set       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_PMM_PKT_PERIOD_get       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_PMM_PKT_PERIOD_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_PMM_PKT_PERIOD_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x0001600c PLL_SUPPORT_PMM_CFG_1 |
 | +-------------------------------------------+
 | | bit  24:16 R/W  PMM_PKT_SIZE              |
 | | bit  2:1   R/W  PMM_DSM_TYPE              |
 | | bit  0     R/W  PMM_ENABLE                |
 | +-------------------------------------------+
 | 
 | void   digi_m1_field_PMM_PKT_SIZE_set       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_PMM_PKT_SIZE_get       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_PMM_PKT_SIZE_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_PMM_PKT_SIZE_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_m1_field_PMM_DSM_TYPE_set       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_PMM_DSM_TYPE_get       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_PMM_DSM_TYPE_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_PMM_DSM_TYPE_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_m1_field_PMM_ENABLE_set         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_PMM_ENABLE_get         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +---------------------------------+
 | | Register 0x00016010 IRIG_IF_MUX |
 | +---------------------------------+
 | | bit  2 R/W  IRIG_CLKO_SEL       |
 | | bit  1 R/W  IRIG_DATO_SEL       |
 | | bit  0 R/W  ONE_PPSO_SEL        |
 | +---------------------------------+
 | 
 | void   digi_m1_field_IRIG_CLKO_SEL_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_IRIG_CLKO_SEL_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_IRIG_DATO_SEL_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_IRIG_DATO_SEL_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_ONE_PPSO_SEL_set  ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_ONE_PPSO_SEL_get  ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x00016014 CAL_349_CFG_0 |
 | +-----------------------------------+
 | | bit  21    R/W  CNT_349_EN        |
 | | bit  20:14 R/W  CNT_349_ROLLOVER  |
 | | bit  13:7  R/W  SS_CAL_349_DLY    |
 | | bit  6:0   R/W  SS_TOC_349_DLY    |
 | +-----------------------------------+
 | 
 | void   digi_m1_field_CNT_349_EN_set             ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_CNT_349_EN_get             ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_CNT_349_ROLLOVER_set       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_CNT_349_ROLLOVER_get       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_CNT_349_ROLLOVER_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_CNT_349_ROLLOVER_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_m1_field_SS_CAL_349_DLY_set         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_SS_CAL_349_DLY_get         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_SS_CAL_349_DLY_set   ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_SS_CAL_349_DLY_get   ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_m1_field_SS_TOC_349_DLY_set         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_SS_TOC_349_DLY_get         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_SS_TOC_349_DLY_set   ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_SS_TOC_349_DLY_get   ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00016018 CAL_349_CFG_1 |
 | +-----------------------------------+
 | | bit  9:0 R/W  SS_TOC_349_EN       |
 | +-----------------------------------+
 | 
 | void   digi_m1_field_SS_TOC_349_EN_set       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_SS_TOC_349_EN_get       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_SS_TOC_349_EN_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_SS_TOC_349_EN_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x0001601c CAL_349_CFG_2 |
 | +-----------------------------------+
 | | bit  24:0 R/W  SS_CAL_349_EN      |
 | +-----------------------------------+
 | 
 | void   digi_m1_field_SS_CAL_349_EN_set       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_SS_CAL_349_EN_get       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_SS_CAL_349_EN_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_SS_CAL_349_EN_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00016020 CAL_349_REQ |
 | +---------------------------------+
 | | bit  0 R/W  REQ_CAL_PG_349      |
 | +---------------------------------+
 | 
 | void   digi_m1_field_REQ_CAL_PG_349_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_REQ_CAL_PG_349_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x0001603c CAL_500_CFG_0 |
 | +-----------------------------------+
 | | bit  21    R/W  CNT_500_EN        |
 | | bit  20:14 R/W  CNT_500_ROLLOVER  |
 | | bit  13:7  R/W  SS_CAL_500_DLY    |
 | | bit  6:0   R/W  SS_TOC_500_DLY    |
 | +-----------------------------------+
 | 
 | void   digi_m1_field_CNT_500_EN_set             ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_CNT_500_EN_get             ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_CNT_500_ROLLOVER_set       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_CNT_500_ROLLOVER_get       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_CNT_500_ROLLOVER_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_CNT_500_ROLLOVER_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_m1_field_SS_CAL_500_DLY_set         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_SS_CAL_500_DLY_get         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_SS_CAL_500_DLY_set   ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_SS_CAL_500_DLY_get   ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_m1_field_SS_TOC_500_DLY_set         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_SS_TOC_500_DLY_get         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_SS_TOC_500_DLY_set   ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_SS_TOC_500_DLY_get   ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00016040 CAL_500_CFG_1 |
 | +-----------------------------------+
 | | bit  9:0 R/W  SS_TOC_500_EN       |
 | +-----------------------------------+
 | 
 | void   digi_m1_field_SS_TOC_500_EN_set       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_SS_TOC_500_EN_get       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_SS_TOC_500_EN_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_SS_TOC_500_EN_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00016044 CAL_500_CFG_2 |
 | +-----------------------------------+
 | | bit  25:0 R/W  SS_CAL_500_EN      |
 | +-----------------------------------+
 | 
 | void   digi_m1_field_SS_CAL_500_EN_set       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_SS_CAL_500_EN_get       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_SS_CAL_500_EN_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_SS_CAL_500_EN_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------+
 | | Register 0x00016048 CAL_500_REQ |
 | +---------------------------------+
 | | bit  0 R/W  REQ_CAL_PG_500      |
 | +---------------------------------+
 | 
 | void   digi_m1_field_REQ_CAL_PG_500_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_REQ_CAL_PG_500_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +----------------------------------------+
 | | Register 0x00016064 IRIG_IF_OUTPUT_ENS |
 | +----------------------------------------+
 | | bit  2 R/W  IRIG_CLKO_OEB              |
 | | bit  1 R/W  IRIG_DATO_OEB              |
 | | bit  0 R/W  ONE_PPSO_OEB               |
 | +----------------------------------------+
 | 
 | void   digi_m1_field_IRIG_CLKO_OEB_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_IRIG_CLKO_OEB_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_IRIG_DATO_OEB_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_IRIG_DATO_OEB_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_ONE_PPSO_OEB_set  ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_ONE_PPSO_OEB_get  ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x00016068 OTU_MFPO_SELECT |
 | +-------------------------------------+
 | | bit  27:16 R/W  OTU_RX_MFPO_SEL     |
 | | bit  11:0  R/W  OTU_TX_MFPO_SEL     |
 | +-------------------------------------+
 | 
 | void   digi_m1_field_OTU_RX_MFPO_SEL_set       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_OTU_RX_MFPO_SEL_get       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_OTU_RX_MFPO_SEL_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_OTU_RX_MFPO_SEL_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   digi_m1_field_OTU_TX_MFPO_SEL_set       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | UINT32 digi_m1_field_OTU_TX_MFPO_SEL_get       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void   digi_m1_field_range_OTU_TX_MFPO_SEL_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 digi_m1_field_range_OTU_TX_MFPO_SEL_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register (0x000160a4 + (N) * 4) EFUSE_STAT |
 | |     N[0..9]                                |
 | +--------------------------------------------+
 | | bit  31:0 R  EFUSE_DATA                    |
 | +--------------------------------------------+
 | 
 | void   digi_m1_lfield_EFUSE_DATA_get       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value[10] )
 | UINT32 digi_m1_lfield_range_EFUSE_DATA_get ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +---------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _DIGI_M1_IO_H
#define _DIGI_M1_IO_H

#include "digi_api.h"
#include "digi_m1_regs.h"
#include "digi_m1_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 digi_m1_buffer_init               ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | void                 digi_m1_buffer_flush              ( digi_m1_buffer_t *b_ptr )
 | UINT32               digi_m1_reg_read                  ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 digi_m1_reg_write                 ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 digi_m1_field_set                 ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 digi_m1_action_on_write_field_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 digi_m1_burst_read                ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 digi_m1_burst_write               ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE digi_m1_poll                      ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +---------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +---------------------------------------------------------------------------------------------------------------------------------------------+
 | void   digi_m1_reg_SUBSYSTEM_SW_RESETS_write       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_SUBSYSTEM_SW_RESETS_field_set   ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_SUBSYSTEM_SW_RESETS_read        ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_PLL_SUPPORT_MUX_CFG_write       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_PLL_SUPPORT_MUX_CFG_field_set   ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_PLL_SUPPORT_MUX_CFG_read        ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_write     ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_field_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_PLL_SUPPORT_PMM_CFG_0_read      ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_write     ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_field_set ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_PLL_SUPPORT_PMM_CFG_1_read      ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_IRIG_IF_MUX_write               ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_IRIG_IF_MUX_field_set           ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_IRIG_IF_MUX_read                ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_CAL_349_CFG_0_write             ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_CAL_349_CFG_0_field_set         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_CAL_349_CFG_0_read              ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_CAL_349_CFG_1_write             ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_CAL_349_CFG_1_field_set         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_CAL_349_CFG_1_read              ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_CAL_349_CFG_2_write             ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_CAL_349_CFG_2_field_set         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_CAL_349_CFG_2_read              ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_CAL_500_CFG_0_write             ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_CAL_500_CFG_0_field_set         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_CAL_500_CFG_0_read              ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_CAL_500_CFG_1_write             ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_CAL_500_CFG_1_field_set         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_CAL_500_CFG_1_read              ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_CAL_500_CFG_2_write             ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_CAL_500_CFG_2_field_set         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_CAL_500_CFG_2_read              ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_IRIG_IF_OUTPUT_ENS_write        ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_IRIG_IF_OUTPUT_ENS_field_set    ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_IRIG_IF_OUTPUT_ENS_read         ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_OTU_MFPO_SELECT_write           ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_OTU_MFPO_SELECT_field_set       ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_OTU_MFPO_SELECT_read            ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_CAL_349_REQ_write               ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_CAL_349_REQ_field_set           ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_CAL_349_REQ_read                ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_CAL_500_REQ_write               ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 value )
 | void   digi_m1_reg_CAL_500_REQ_field_set           ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 digi_m1_reg_CAL_500_REQ_read                ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr )
 | 
 | void   digi_m1_reg_EFUSE_STAT_array_burst_read     ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32 digi_m1_reg_EFUSE_STAT_array_read           ( digi_m1_buffer_t *b_ptr, digi_handle_t *h_ptr, UINT32 N )
 | 
 +--------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _DIGI_M1_IO_H */
