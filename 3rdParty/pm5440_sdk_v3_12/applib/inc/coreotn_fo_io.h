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
 *     and register accessor functions for the coreotn_fo block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing coreotn_fo io functions is:
 | {
 |     coreotn_fo_buffer_t b_ptr[1];
 |     coreotn_fo_buffer_init( b_ptr, h_ptr, A );
 |     value = coreotn_fo_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     coreotn_fo_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     coreotn_fo_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = coreotn_fo_field_<FIELD>_get( NULL, h_ptr, A );
 | coreotn_fo_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------+
 | | Register 0x0023c000 SW_RESET_REG |
 | +----------------------------------+
 | | bit  6 R/W  OHFS_REMOVE_RST      |
 | | bit  5 R/W  FRM_RCP_RST          |
 | | bit  4 R/W  PRBS_RST             |
 | | bit  3 R/W  MPMO_RST             |
 | | bit  2 R/W  MPMA_RST             |
 | | bit  1 R/W  TFRM_RST             |
 | | bit  0 R/W  RFRM_RST             |
 | +----------------------------------+
 | 
 | void   coreotn_fo_field_OHFS_REMOVE_RST_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_OHFS_REMOVE_RST_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_FRM_RCP_RST_set     ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_FRM_RCP_RST_get     ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_PRBS_RST_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_PRBS_RST_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_MPMO_RST_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_MPMO_RST_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_MPMA_RST_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_MPMA_RST_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_TFRM_RST_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_TFRM_RST_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_RFRM_RST_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_RFRM_RST_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------+
 | | Register 0x0023c004 CLK_GATING_REG |
 | +------------------------------------+
 | | bit  6 R/W  OHFS_REMOVE_LOWPWR     |
 | | bit  5 R/W  FRM_RCP_LOWPWR         |
 | | bit  4 R/W  PRBS_LOWPWR            |
 | | bit  3 R/W  MPMO_LOWPWR            |
 | | bit  2 R/W  MPMA_LOWPWR            |
 | | bit  1 R/W  TFRM_LOWPWR            |
 | | bit  0 R/W  RFRM_LOWPWR            |
 | +------------------------------------+
 | 
 | void   coreotn_fo_field_OHFS_REMOVE_LOWPWR_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_OHFS_REMOVE_LOWPWR_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_FRM_RCP_LOWPWR_set     ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_FRM_RCP_LOWPWR_get     ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_PRBS_LOWPWR_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_PRBS_LOWPWR_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_MPMO_LOWPWR_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_MPMO_LOWPWR_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_MPMA_LOWPWR_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_MPMA_LOWPWR_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_TFRM_LOWPWR_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_TFRM_LOWPWR_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_RFRM_LOWPWR_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_RFRM_LOWPWR_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------+
 | | Register 0x0023c008 RAM_LOW_POWER_REG |
 | +---------------------------------------+
 | | bit  6 R/W  OHFS_REMOVE_RAM_LOWPWR    |
 | | bit  4 R/W  PRBS_RAM_LOWPWR           |
 | | bit  3 R/W  MPMO_RAM_LOWPWR           |
 | | bit  2 R/W  MPMA_RAM_LOWPWR           |
 | | bit  1 R/W  TFRM_RAM_LOWPWR           |
 | | bit  0 R/W  RFRM_RAM_LOWPWR           |
 | +---------------------------------------+
 | 
 | void   coreotn_fo_field_OHFS_REMOVE_RAM_LOWPWR_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_OHFS_REMOVE_RAM_LOWPWR_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_PRBS_RAM_LOWPWR_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_PRBS_RAM_LOWPWR_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_MPMO_RAM_LOWPWR_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_MPMO_RAM_LOWPWR_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_MPMA_RAM_LOWPWR_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_MPMA_RAM_LOWPWR_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_TFRM_RAM_LOWPWR_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_TFRM_RAM_LOWPWR_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_RFRM_RAM_LOWPWR_set        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_RFRM_RAM_LOWPWR_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------------+
 | | Register 0x0023c014 FRAMER_STAGE_CONFIG_REG |
 | +---------------------------------------------+
 | | bit  0 R/W  BWR_IND_SEL                     |
 | +---------------------------------------------+
 | 
 | void   coreotn_fo_field_BWR_IND_SEL_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_BWR_IND_SEL_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | +------------------------------------------+
 | | Register 0x0023c018 FRM_RCP_CONFIG_REG_1 |
 | +------------------------------------------+
 | | bit  31:0 R/W  REMOTE_RCP_CFG            |
 | +------------------------------------------+
 | 
 | void   coreotn_fo_lfield_REMOTE_RCP_CFG_set       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3] )
 | void   coreotn_fo_lfield_REMOTE_RCP_CFG_get       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3] )
 | void   coreotn_fo_lfield_range_REMOTE_RCP_CFG_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fo_lfield_range_REMOTE_RCP_CFG_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0023c024 FRM_RCP_CONFIG_REG_4 |
 | +------------------------------------------+
 | | bit  31:0 R/W  CFC_FIFO_CLRB_CFG         |
 | +------------------------------------------+
 | 
 | void   coreotn_fo_lfield_CFC_FIFO_CLRB_CFG_set       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3] )
 | void   coreotn_fo_lfield_CFC_FIFO_CLRB_CFG_get       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3] )
 | void   coreotn_fo_lfield_range_CFC_FIFO_CLRB_CFG_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fo_lfield_range_CFC_FIFO_CLRB_CFG_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------+
 | | Register 0x0023c070 GPO_REG |
 | +-----------------------------+
 | | bit  31:0 R/W  SPARE_GPO    |
 | +-----------------------------+
 | 
 | void   coreotn_fo_field_SPARE_GPO_set       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_SPARE_GPO_get       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_range_SPARE_GPO_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fo_field_range_SPARE_GPO_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x0023c078 + (N) * 0x4) PRBS_CHANNEL_CFG_REG |
 | |     N[0..11]                                           |
 | +--------------------------------------------------------+
 | | bit  14:8 R/W  PRBS_CHID                               |
 | | bit  3    R/W  PRBS_SEL_INV_NONINV                     |
 | | bit  2    R/W  PRBS_SEL_31_11                          |
 | | bit  1    R/W  PRBS_MON_EN                             |
 | | bit  0    R/W  PRBS_GEN_EN                             |
 | +--------------------------------------------------------+
 | 
 | void   coreotn_fo_field_PRBS_CHID_set           ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 coreotn_fo_field_PRBS_CHID_get           ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   coreotn_fo_field_range_PRBS_CHID_set     ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fo_field_range_PRBS_CHID_get     ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   coreotn_fo_field_PRBS_SEL_INV_NONINV_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 coreotn_fo_field_PRBS_SEL_INV_NONINV_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   coreotn_fo_field_PRBS_SEL_31_11_set      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 coreotn_fo_field_PRBS_SEL_31_11_get      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   coreotn_fo_field_PRBS_MON_EN_set         ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 coreotn_fo_field_PRBS_MON_EN_get         ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   coreotn_fo_field_PRBS_GEN_EN_set         ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 coreotn_fo_field_PRBS_GEN_EN_get         ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +-------------------------------------------------------+
 | | Register (0x0023c0a8 + (N) * 0x4) PRBS_DEFECT_CFG_REG |
 | |     N[0..11]                                          |
 | +-------------------------------------------------------+
 | | bit  0 R/W  PRBS_INS_DEFECT                           |
 | +-------------------------------------------------------+
 | 
 | void   coreotn_fo_field_PRBS_INS_DEFECT_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 coreotn_fo_field_PRBS_INS_DEFECT_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +-----------------------------------------------------+
 | | Register (0x0023c0d8 + (N) * 0x4) PRBS_TSE_STAT_REG |
 | |     N[0..11]                                        |
 | +-----------------------------------------------------+
 | | bit  31:0 R  PRBS_TSE_CHAN_REG                      |
 | +-----------------------------------------------------+
 | 
 | UINT32 coreotn_fo_field_PRBS_TSE_CHAN_REG_get       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 coreotn_fo_field_range_PRBS_TSE_CHAN_REG_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x0023c108 PRBS_BER_TH_LOWER_CFG_REG |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  PRBS_BER_THRES                 |
 | +-----------------------------------------------+
 | 
 | void   coreotn_fo_lfield_PRBS_BER_THRES_set       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | void   coreotn_fo_lfield_PRBS_BER_THRES_get       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | void   coreotn_fo_lfield_range_PRBS_BER_THRES_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fo_lfield_range_PRBS_BER_THRES_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x0023c110 PRBS_RESEED_REG  |
 | +--------------------------------------+
 | | bit  31:16 R/W  PRBS_RESEED_THR      |
 | | bit  15:0  R/W  PRBS_RESEED_INTERVAL |
 | +--------------------------------------+
 | 
 | void   coreotn_fo_field_PRBS_RESEED_THR_set            ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_PRBS_RESEED_THR_get            ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_range_PRBS_RESEED_THR_set      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fo_field_range_PRBS_RESEED_THR_get      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   coreotn_fo_field_PRBS_RESEED_INTERVAL_set       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_PRBS_RESEED_INTERVAL_get       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_range_PRBS_RESEED_INTERVAL_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fo_field_range_PRBS_RESEED_INTERVAL_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x0023c11c PRBS_TSE_ROLLOVER_STAT_REG |
 | +------------------------------------------------+
 | | bit  11:0 R  PRBS_TSE_ROVER                    |
 | +------------------------------------------------+
 | 
 | UINT32 coreotn_fo_field_PRBS_TSE_ROVER_get       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | UINT32 coreotn_fo_field_range_PRBS_TSE_ROVER_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0023c120 PRBS_DLSS_INT_EN_REG |
 | +------------------------------------------+
 | | bit  11:0 R/W  PRBS_DLSS_E               |
 | +------------------------------------------+
 | 
 | void   coreotn_fo_field_PRBS_DLSS_E_set       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 coreotn_fo_field_PRBS_DLSS_E_get       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void   coreotn_fo_field_range_PRBS_DLSS_E_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fo_field_range_PRBS_DLSS_E_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0023c124 PRBS_DLSS_INT_STAT_REG |
 | +--------------------------------------------+
 | | bit  11:0 R/W  PRBS_DLSS_I                 |
 | +--------------------------------------------+
 | 
 | void                 coreotn_fo_field_PRBS_DLSS_I_set_to_clear       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               coreotn_fo_field_PRBS_DLSS_I_get                ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void                 coreotn_fo_field_range_PRBS_DLSS_I_set_to_clear ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               coreotn_fo_field_range_PRBS_DLSS_I_get          ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE coreotn_fo_field_range_PRBS_DLSS_I_poll         ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE coreotn_fo_field_PRBS_DLSS_I_poll               ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x0023c128 PRBS_DLSS_INT_VAL_REG |
 | +-------------------------------------------+
 | | bit  11:0 R  PRBS_DLSS_V                  |
 | +-------------------------------------------+
 | 
 | UINT32               coreotn_fo_field_PRBS_DLSS_V_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | UINT32               coreotn_fo_field_range_PRBS_DLSS_V_get  ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE coreotn_fo_field_range_PRBS_DLSS_V_poll ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE coreotn_fo_field_PRBS_DLSS_V_poll       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register 0x0023c14c OPU_DCSF_INT_EN_REG_1 |
 | +-------------------------------------------+
 | | bit  31:0 R/W  OPU_DCSF_E                 |
 | +-------------------------------------------+
 | 
 | void   coreotn_fo_lfield_OPU_DCSF_E_set       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3] )
 | void   coreotn_fo_lfield_OPU_DCSF_E_get       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3] )
 | void   coreotn_fo_lfield_range_OPU_DCSF_E_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 coreotn_fo_lfield_range_OPU_DCSF_E_get ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x0023c158 OPU_DCSF_INT_STAT_REG_1 |
 | +---------------------------------------------+
 | | bit  31:0 R/W  OPU_DCSF_I                   |
 | +---------------------------------------------+
 | 
 | void                 coreotn_fo_lfield_OPU_DCSF_I_set_to_clear       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3] )
 | void                 coreotn_fo_lfield_OPU_DCSF_I_get                ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3] )
 | void                 coreotn_fo_lfield_range_OPU_DCSF_I_set_to_clear ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               coreotn_fo_lfield_range_OPU_DCSF_I_get          ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE coreotn_fo_lfield_range_OPU_DCSF_I_poll         ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE coreotn_fo_lfield_OPU_DCSF_I_poll               ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x0023c164 OPU_DCSF_INT_VAL_REG_1 |
 | +--------------------------------------------+
 | | bit  31:0 R  OPU_DCSF_V                    |
 | +--------------------------------------------+
 | 
 | void                 coreotn_fo_lfield_OPU_DCSF_V_get        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3] )
 | UINT32               coreotn_fo_lfield_range_OPU_DCSF_V_get  ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE coreotn_fo_lfield_range_OPU_DCSF_V_poll ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE coreotn_fo_lfield_OPU_DCSF_V_poll       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _COREOTN_FO_IO_H
#define _COREOTN_FO_IO_H

#include "coreotn_loc.h"
#include "coreotn_fo_regs.h"
#include "coreotn_fo_io_inline.h"


/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 coreotn_fo_buffer_init               ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | void                 coreotn_fo_buffer_flush              ( coreotn_fo_buffer_t *b_ptr )
 | UINT32               coreotn_fo_reg_read                  ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 coreotn_fo_reg_write                 ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 coreotn_fo_field_set                 ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 coreotn_fo_action_on_write_field_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 coreotn_fo_burst_read                ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 coreotn_fo_burst_write               ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE coreotn_fo_poll                      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 coreotn_fo_reg_SW_RESET_REG_write                                ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_SW_RESET_REG_field_set                            ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_SW_RESET_REG_read                                 ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_CLK_GATING_REG_write                              ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_CLK_GATING_REG_field_set                          ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_CLK_GATING_REG_read                               ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_RAM_LOW_POWER_REG_write                           ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_RAM_LOW_POWER_REG_field_set                       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_RAM_LOW_POWER_REG_read                            ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_FRAMER_STAGE_CONFIG_REG_write                     ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_FRAMER_STAGE_CONFIG_REG_field_set                 ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_FRAMER_STAGE_CONFIG_REG_read                      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_FRM_RCP_CONFIG_REG_1_write                        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_FRM_RCP_CONFIG_REG_1_field_set                    ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_FRM_RCP_CONFIG_REG_1_read                         ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_FRM_RCP_CONFIG_REG_2_write                        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_FRM_RCP_CONFIG_REG_2_field_set                    ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_FRM_RCP_CONFIG_REG_2_read                         ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_FRM_RCP_CONFIG_REG_3_write                        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_FRM_RCP_CONFIG_REG_3_field_set                    ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_FRM_RCP_CONFIG_REG_3_read                         ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_FRM_RCP_CONFIG_REG_4_write                        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_FRM_RCP_CONFIG_REG_4_field_set                    ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_FRM_RCP_CONFIG_REG_4_read                         ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_FRM_RCP_CONFIG_REG_5_write                        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_FRM_RCP_CONFIG_REG_5_field_set                    ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_FRM_RCP_CONFIG_REG_5_read                         ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_FRM_RCP_CONFIG_REG_6_write                        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_FRM_RCP_CONFIG_REG_6_field_set                    ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_FRM_RCP_CONFIG_REG_6_read                         ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_GPO_REG_write                                     ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_GPO_REG_field_set                                 ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_GPO_REG_read                                      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_PRBS_BER_TH_LOWER_CFG_REG_write                   ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_PRBS_BER_TH_LOWER_CFG_REG_field_set               ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_PRBS_BER_TH_LOWER_CFG_REG_read                    ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_PRBS_BER_TH_UPPER_CFG_REG_write                   ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_PRBS_BER_TH_UPPER_CFG_REG_field_set               ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_PRBS_BER_TH_UPPER_CFG_REG_read                    ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_PRBS_RESEED_REG_write                             ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_PRBS_RESEED_REG_field_set                         ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_PRBS_RESEED_REG_read                              ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_PRBS_CHANNEL_CFG_REG_array_write                  ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 coreotn_fo_reg_PRBS_CHANNEL_CFG_REG_array_field_set              ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_PRBS_CHANNEL_CFG_REG_array_read                   ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 coreotn_fo_reg_PRBS_DEFECT_CFG_REG_array_write                   ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void                 coreotn_fo_reg_PRBS_DEFECT_CFG_REG_array_field_set               ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_PRBS_DEFECT_CFG_REG_array_read                    ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void                 coreotn_fo_reg_PRBS_DLSS_INT_EN_REG_write                        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_PRBS_DLSS_INT_EN_REG_field_set                    ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_PRBS_DLSS_INT_EN_REG_read                         ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_OPU_DCSF_INT_EN_REG_1_write                       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_OPU_DCSF_INT_EN_REG_1_field_set                   ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_OPU_DCSF_INT_EN_REG_1_read                        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_OPU_DCSF_INT_EN_REG_2_write                       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_OPU_DCSF_INT_EN_REG_2_field_set                   ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_OPU_DCSF_INT_EN_REG_2_read                        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_OPU_DCSF_INT_EN_REG_3_write                       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_OPU_DCSF_INT_EN_REG_3_field_set                   ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_OPU_DCSF_INT_EN_REG_3_read                        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | void                 coreotn_fo_reg_PRBS_DLSS_INT_STAT_REG_write                      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_PRBS_DLSS_INT_STAT_REG_action_on_write_field_set  ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_PRBS_DLSS_INT_STAT_REG_read                       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE coreotn_fo_reg_PRBS_DLSS_INT_STAT_REG_poll                       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 coreotn_fo_reg_OPU_DCSF_INT_STAT_REG_1_write                     ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_OPU_DCSF_INT_STAT_REG_1_action_on_write_field_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_OPU_DCSF_INT_STAT_REG_1_read                      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE coreotn_fo_reg_OPU_DCSF_INT_STAT_REG_1_poll                      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 coreotn_fo_reg_OPU_DCSF_INT_STAT_REG_2_write                     ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_OPU_DCSF_INT_STAT_REG_2_action_on_write_field_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_OPU_DCSF_INT_STAT_REG_2_read                      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE coreotn_fo_reg_OPU_DCSF_INT_STAT_REG_2_poll                      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 coreotn_fo_reg_OPU_DCSF_INT_STAT_REG_3_write                     ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 coreotn_fo_reg_OPU_DCSF_INT_STAT_REG_3_action_on_write_field_set ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               coreotn_fo_reg_OPU_DCSF_INT_STAT_REG_3_read                      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE coreotn_fo_reg_OPU_DCSF_INT_STAT_REG_3_poll                      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               coreotn_fo_reg_PRBS_DLSS_INT_VAL_REG_read                        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE coreotn_fo_reg_PRBS_DLSS_INT_VAL_REG_poll                        ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               coreotn_fo_reg_OPU_DCSF_INT_VAL_REG_1_read                       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE coreotn_fo_reg_OPU_DCSF_INT_VAL_REG_1_poll                       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               coreotn_fo_reg_OPU_DCSF_INT_VAL_REG_2_read                       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE coreotn_fo_reg_OPU_DCSF_INT_VAL_REG_2_poll                       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               coreotn_fo_reg_OPU_DCSF_INT_VAL_REG_3_read                       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE coreotn_fo_reg_OPU_DCSF_INT_VAL_REG_3_poll                       ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               coreotn_fo_reg_PRBS_TSE_ROLLOVER_STAT_REG_read                   ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               coreotn_fo_reg_PRBS_TSE_STAT_REG_array_read                      ( coreotn_fo_buffer_t *b_ptr, coreotn_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _COREOTN_FO_IO_H */
