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
 *     and register accessor functions for the mapotn_rgfpf block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing mapotn_rgfpf io functions is:
 | {
 |     mapotn_rgfpf_buffer_t b_ptr[1];
 |     mapotn_rgfpf_buffer_init( b_ptr, h_ptr );
 |     value = mapotn_rgfpf_field_<FIELD>_get( b_ptr, h_ptr );
 |     mapotn_rgfpf_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     mapotn_rgfpf_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = mapotn_rgfpf_field_<FIELD>_get( NULL, h_ptr );
 | mapotn_rgfpf_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-------------------------------------------+
 | | Register 0x00030000 GENERAL_CONFIGURATION |
 | +-------------------------------------------+
 | | bit  24   R/W  UNPACK_AFIFO_CLR           |
 | | bit  16   R/W  CSF_FIFO_CLR               |
 | | bit  14:8 R/W  CSF_ST_I_TRSHLD            |
 | +-------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_UNPACK_AFIFO_CLR_set      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_UNPACK_AFIFO_CLR_get      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rgfpf_field_CSF_FIFO_CLR_set          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_CSF_FIFO_CLR_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rgfpf_field_CSF_ST_I_TRSHLD_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_CSF_ST_I_TRSHLD_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rgfpf_field_range_CSF_ST_I_TRSHLD_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_CSF_ST_I_TRSHLD_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00030004 + (N) * 0x4) CHANNEL_RESET |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  0 R/W  CHN_RST                             |
 | +-------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_CHN_RST_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_CHN_RST_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | +-------------------------------------------------+
 | | Register 0x00030184 MAC_DESTINATION_ADDRESS_LSB |
 | +-------------------------------------------------+
 | | bit  31:0 R/W  MAC_DA_LSB                       |
 | +-------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_MAC_DA_LSB_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_MAC_DA_LSB_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rgfpf_field_range_MAC_DA_LSB_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_MAC_DA_LSB_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00030188 MAC_DESTINATION_ADDRESS_MSB |
 | +-------------------------------------------------+
 | | bit  15:0 R/W  MAC_DA_MSB                       |
 | +-------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_MAC_DA_MSB_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_MAC_DA_MSB_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rgfpf_field_range_MAC_DA_MSB_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_MAC_DA_MSB_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x0003018c ORDERED_SET_UPI |
 | +-------------------------------------+
 | | bit  7:0 R/W  ORD_SET_UPI           |
 | +-------------------------------------+
 | 
 | void   mapotn_rgfpf_field_ORD_SET_UPI_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_ORD_SET_UPI_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rgfpf_field_range_ORD_SET_UPI_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_ORD_SET_UPI_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x00030190 PMON_LATCH_CONTROL_REGISTER |
 | +-------------------------------------------------+
 | | bit  0 R/W  PMON_LATCH_TRIG                     |
 | +-------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_PMON_LATCH_TRIG_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PMON_LATCH_TRIG_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +---------------------------------------------+
 | | Register 0x00030194 TRIG_PAUSE_CAP_REGISTER |
 | +---------------------------------------------+
 | | bit  0 R/W  PAUSE_CAP_TRIG                  |
 | +---------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_PAUSE_CAP_TRIG_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PAUSE_CAP_TRIG_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x00030198 + (N) * 0x4) AUTO_FLOW_CTRL_REGISTER |
 | |     N[0..2]                                               |
 | +-----------------------------------------------------------+
 | | bit  31:0 R/W  AUTO_FLOW_CTRL_EN                          |
 | +-----------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_AUTO_FLOW_CTRL_EN_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_AUTO_FLOW_CTRL_EN_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_AUTO_FLOW_CTRL_EN_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_AUTO_FLOW_CTRL_EN_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x000301a4 + (N) * 0x4) CPB_ZONE_THRESHOLD |
 | |     N[0..5]                                          |
 | +------------------------------------------------------+
 | | bit  31:0 R/W  CPB_ZONE_THRLD                        |
 | +------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CPB_ZONE_THRLD_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[6] )
 | void   mapotn_rgfpf_lfield_CPB_ZONE_THRLD_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[6] )
 | void   mapotn_rgfpf_lfield_range_CPB_ZONE_THRLD_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CPB_ZONE_THRLD_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x000301bc EXTENSION_HDR_MIS_FAIL_STATUS |
 | +---------------------------------------------------+
 | | bit  31:16 R    DEXM_TYPEHDR                      |
 | | bit  15:8  R    DEXM_CID                          |
 | | bit  7:1   R    DEXM_CHID                         |
 | | bit  0     R/W  DEXM_STATUS_REG_FREE              |
 | +---------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_DEXM_STATUS_REG_FREE_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DEXM_STATUS_REG_FREE_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rgfpf_field_DEXM_TYPEHDR_get         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rgfpf_field_range_DEXM_TYPEHDR_get   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mapotn_rgfpf_field_DEXM_CID_get             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rgfpf_field_range_DEXM_CID_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mapotn_rgfpf_field_DEXM_CHID_get            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rgfpf_field_range_DEXM_CHID_get      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x000301c0 PTI_UPI_MIS_STATUS |
 | +----------------------------------------+
 | | bit  23:8 R    DUPM_TYPEHDR            |
 | | bit  7:1  R    DUPM_CHID               |
 | | bit  0    R/W  DUPM_STATUS_REG_FREE    |
 | +----------------------------------------+
 | 
 | void   mapotn_rgfpf_field_DUPM_STATUS_REG_FREE_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DUPM_STATUS_REG_FREE_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rgfpf_field_DUPM_TYPEHDR_get         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rgfpf_field_range_DUPM_TYPEHDR_get   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mapotn_rgfpf_field_DUPM_CHID_get            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rgfpf_field_range_DUPM_CHID_get      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x000301c4 CHID_MISMATCH_CAPTURE |
 | +-------------------------------------------+
 | | bit  14:8 R/W  DUPM_CAP_CHID              |
 | | bit  6:0  R/W  DEXM_CAP_CHID              |
 | +-------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_DUPM_CAP_CHID_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DUPM_CAP_CHID_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rgfpf_field_range_DUPM_CAP_CHID_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_DUPM_CAP_CHID_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_DEXM_CAP_CHID_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DEXM_CAP_CHID_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rgfpf_field_range_DEXM_CAP_CHID_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_DEXM_CAP_CHID_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x000301c8 CSF_STATUS |
 | +--------------------------------+
 | | bit  15:9 R  CSF_ST_CHID       |
 | | bit  8:1  R  CSF_ST_UPI        |
 | | bit  0    R  CSF_ST_VALID      |
 | +--------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_CSF_ST_CHID_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rgfpf_field_range_CSF_ST_CHID_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mapotn_rgfpf_field_CSF_ST_UPI_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | UINT32 mapotn_rgfpf_field_range_CSF_ST_UPI_get  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mapotn_rgfpf_field_CSF_ST_VALID_get      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +----------------------------------+
 | | Register 0x000301cc CPU_FIFO_RPT |
 | +----------------------------------+
 | | bit  24:16 R/W  WR_RPT_THRLD     |
 | | bit  12:8  R/W  RD_RPT_THRLD     |
 | | bit  1     R/W  WR_RPT_FIFOCLR   |
 | | bit  0     R/W  RD_RPT_FIFOCLR   |
 | +----------------------------------+
 | 
 | void   mapotn_rgfpf_field_WR_RPT_THRLD_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_WR_RPT_THRLD_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rgfpf_field_range_WR_RPT_THRLD_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_WR_RPT_THRLD_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_RD_RPT_THRLD_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_RD_RPT_THRLD_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rgfpf_field_range_RD_RPT_THRLD_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_RD_RPT_THRLD_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_WR_RPT_FIFOCLR_set     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_WR_RPT_FIFOCLR_get     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rgfpf_field_RD_RPT_FIFOCLR_set     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_RD_RPT_FIFOCLR_get     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +--------------------------------------------------------+
 | | Register (0x000301e8 + (N) * 0x4) IN_SFAIL_INTR_ENABLE |
 | |     N[0..2]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R/W  IN_SFAIL_E                              |
 | +--------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_IN_SFAIL_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_IN_SFAIL_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_IN_SFAIL_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_IN_SFAIL_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------------+
 | | Register (0x000301f4 + (N) * 0x4) LOSS_FRME_DELINEATION_INTR_ENABLE |
 | |     N[0..2]                                                         |
 | +---------------------------------------------------------------------+
 | | bit  31:0 R/W  LOFD_E                                               |
 | +---------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_LOFD_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_LOFD_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_LOFD_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_LOFD_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x00030200 + (N) * 0x4) GFP_IDLE_FRAME_INTR_ENABLE |
 | |     N[0..2]                                                  |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  IDLE_E                                        |
 | +--------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_IDLE_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_IDLE_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_IDLE_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_IDLE_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------------+
 | | Register (0x0003020c + (N) * 0x4) GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE |
 | |     N[0..2]                                                              |
 | +--------------------------------------------------------------------------+
 | | bit  31:0 R/W  CTRLFRM_E                                                 |
 | +--------------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CTRLFRM_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_CTRLFRM_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_CTRLFRM_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CTRLFRM_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x00030218 + (N) * 0x4) UNPACK_DROP_INTR_ENABLE |
 | |     N[0..2]                                               |
 | +-----------------------------------------------------------+
 | | bit  31:0 R/W  UNPACK_FIFO_DROP_E                         |
 | +-----------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_UNPACK_FIFO_DROP_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_UNPACK_FIFO_DROP_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_UNPACK_FIFO_DROP_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_UNPACK_FIFO_DROP_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------------------+
 | | Register (0x00030224 + (N) * 0x4) GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE |
 | |     N[0..2]                                                                |
 | +----------------------------------------------------------------------------+
 | | bit  31:0 R/W  UCHEC_E                                                     |
 | +----------------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_UCHEC_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_UCHEC_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_UCHEC_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_UCHEC_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00030230 + (N) * 0x4) GFP_CCHEC_ERROR_INTR_ENABLE |
 | |     N[0..2]                                                   |
 | +---------------------------------------------------------------+
 | | bit  31:0 R/W  CCHEC_E                                        |
 | +---------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CCHEC_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_CCHEC_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_CCHEC_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CCHEC_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x0003023c + (N) * 0x4) GFP_UTHEC_ERROR_INTR_ENABLE |
 | |     N[0..2]                                                   |
 | +---------------------------------------------------------------+
 | | bit  31:0 R/W  UTHEC_E                                        |
 | +---------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_UTHEC_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_UTHEC_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_UTHEC_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_UTHEC_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00030248 + (N) * 0x4) GFP_CTHEC_ERROR_INTR_ENABLE |
 | |     N[0..2]                                                   |
 | +---------------------------------------------------------------+
 | | bit  31:0 R/W  CTHEC_E                                        |
 | +---------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CTHEC_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_CTHEC_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_CTHEC_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CTHEC_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------------------+
 | | Register (0x00030254 + (N) * 0x4) GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE |
 | |     N[0..2]                                                                |
 | +----------------------------------------------------------------------------+
 | | bit  31:0 R/W  UEHEC_E                                                     |
 | +----------------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_UEHEC_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_UEHEC_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_UEHEC_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_UEHEC_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00030260 + (N) * 0x4) GFP_CEHEC_ERROR_INTR_ENABLE |
 | |     N[0..2]                                                   |
 | +---------------------------------------------------------------+
 | | bit  31:0 R/W  CEHEC_E                                        |
 | +---------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CEHEC_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_CEHEC_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_CEHEC_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CEHEC_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------------+
 | | Register (0x0003026c + (N) * 0x4) GFP_EXI_MISMATCH_ERROR_INTR_ENABLE |
 | |     N[0..2]                                                          |
 | +----------------------------------------------------------------------+
 | | bit  31:0 R/W  EXI_E                                                 |
 | +----------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_EXI_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_EXI_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_EXI_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_EXI_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00030284 + (N) * 0x4) GFP_PTI_UPI_MSM_ERR_INTR_ENABLE |
 | |     N[0..2]                                                       |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  PTI_UPI_MSMCH_E                                    |
 | +-------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_PTI_UPI_MSMCH_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_PTI_UPI_MSMCH_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_PTI_UPI_MSMCH_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_PTI_UPI_MSMCH_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------------------+
 | | Register (0x00030290 + (N) * 0x4) GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE |
 | |     N[0..2]                                                              |
 | +--------------------------------------------------------------------------+
 | | bit  31:0 R/W  GENERIC_E                                                 |
 | +--------------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_GENERIC_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_GENERIC_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_GENERIC_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_GENERIC_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x0003029c + (N) * 0x4) GFP_CSF_INTR_ENABLE |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  CSF_E                                  |
 | +-------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CSF_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_CSF_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_CSF_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CSF_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x000302a8 CSF_STATUS_AV_INTR_ENABLE |
 | +-----------------------------------------------+
 | | bit  0 R/W  CSF_ST_AV_E                       |
 | +-----------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_CSF_ST_AV_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_CSF_ST_AV_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +----------------------------------------------------+
 | | Register 0x000302ac CSF_STATUS_FIFO_OF_INTR_ENABLE |
 | +----------------------------------------------------+
 | | bit  0 R/W  CSF_ST_FIFO_OF_E                       |
 | +----------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_CSF_ST_FIFO_OF_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_CSF_ST_FIFO_OF_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +-----------------------------------------------------------------+
 | | Register (0x000302b0 + (N) * 0x4) GFP_DROPPED_FRAME_INTR_ENABLE |
 | |     N[0..2]                                                     |
 | +-----------------------------------------------------------------+
 | | bit  31:0 R/W  DROP_E                                           |
 | +-----------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_DROP_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_DROP_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_DROP_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_DROP_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------------+
 | | Register (0x000302bc + (N) * 0x4) GFP_CLIENT_DATA_FRAME_INTR_ENABLE |
 | |     N[0..2]                                                         |
 | +---------------------------------------------------------------------+
 | | bit  31:0 R/W  DATAFRM_E                                            |
 | +---------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_DATAFRM_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_DATAFRM_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_DATAFRM_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_DATAFRM_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x000302c8 + (N) * 0x4) PAUSE_INTR_ENABLE |
 | |     N[0..2]                                         |
 | +-----------------------------------------------------+
 | | bit  31:0 R/W  PAUSE_E                              |
 | +-----------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_PAUSE_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_PAUSE_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_PAUSE_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_PAUSE_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register (0x000302d4 + (N) * 0x4) MAX_FL_VIOALT_INTR_ENABLE |
 | |     N[0..2]                                                 |
 | +-------------------------------------------------------------+
 | | bit  31:0 R/W  MAX_FRM_LEN_E                                |
 | +-------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_MAX_FRM_LEN_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_MAX_FRM_LEN_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_MAX_FRM_LEN_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_MAX_FRM_LEN_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register (0x000302e0 + (N) * 0x4) MIN_FL_VIOALT_INTR_ENABLE |
 | |     N[0..2]                                                 |
 | +-------------------------------------------------------------+
 | | bit  31:0 R/W  MIN_FRM_LEN_E                                |
 | +-------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_MIN_FRM_LEN_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_MIN_FRM_LEN_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_MIN_FRM_LEN_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_MIN_FRM_LEN_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x000302ec + (N) * 0x4 + (M) * 0xc) |
 | | FRM_LEN_STAT_INTR_ENABLE                      |
 | |     N[0..2]                                   |
 | |     M[0..6]                                   |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  FRM_LEN_STAT_INTR_E            |
 | +-----------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_FRM_LEN_STAT_INTR_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_FRM_LEN_STAT_INTR_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_FRM_LEN_STAT_INTR_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_FRM_LEN_STAT_INTR_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x00030340 + (N) * 0x4) PAYLOAD_FCS_INTR_ENABLE |
 | |     N[0..2]                                               |
 | +-----------------------------------------------------------+
 | | bit  31:0 R/W  PFCSE_E                                    |
 | +-----------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_PFCSE_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_PFCSE_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_PFCSE_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_PFCSE_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------------+
 | | Register (0x0003034c + (N) * 0x4) DOWNSTRM_FRAME_INTR_ENABLE |
 | |     N[0..2]                                                  |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  DWNSTRM_FRM_E                                 |
 | +--------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_DWNSTRM_FRM_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_DWNSTRM_FRM_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_DWNSTRM_FRM_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_DWNSTRM_FRM_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------+
 | | Register (0x00030358 + (N) * 0x4) CPU_FIFO_DATA_AV_INTR_ENABLE |
 | |     N[0..2]                                                    |
 | +----------------------------------------------------------------+
 | | bit  31:0 R/W  CPU_FIFO_DATA_AV_E                              |
 | +----------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CPU_FIFO_DATA_AV_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_CPU_FIFO_DATA_AV_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_CPU_FIFO_DATA_AV_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CPU_FIFO_DATA_AV_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------------+
 | | Register (0x00030364 + (N) * 0x4) CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE |
 | |     N[0..2]                                                          |
 | +----------------------------------------------------------------------+
 | | bit  31:0 R/W  CPU_FIFO_OVF_E                                        |
 | +----------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CPU_FIFO_OVF_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_CPU_FIFO_OVF_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_CPU_FIFO_OVF_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CPU_FIFO_OVF_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x00030370 PMON_TRANSFER_INTR_ENABLE |
 | +-----------------------------------------------+
 | | bit  1 R/W  PMON_TRFR_END_E                   |
 | | bit  0 R/W  PMON_TRFR_START_E                 |
 | +-----------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_PMON_TRFR_END_E_set   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PMON_TRFR_END_E_get   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rgfpf_field_PMON_TRFR_START_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PMON_TRFR_START_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +------------------------------------------------+
 | | Register 0x00030374 ASYNC_FIFO_OVF_INTR_ENABLE |
 | +------------------------------------------------+
 | | bit  2 R/W  CPU_WRRPT_AFIFO_OVF_E              |
 | | bit  1 R/W  CPU_RDRPT_AFIFO_OVF_E              |
 | | bit  0 R/W  UNPACK_AFIFO_OVF_E                 |
 | +------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_CPU_WRRPT_AFIFO_OVF_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_CPU_WRRPT_AFIFO_OVF_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rgfpf_field_CPU_RDRPT_AFIFO_OVF_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_CPU_RDRPT_AFIFO_OVF_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void   mapotn_rgfpf_field_UNPACK_AFIFO_OVF_E_set    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_UNPACK_AFIFO_OVF_E_get    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +-------------------------------------------------+
 | | Register (0x00030384 + (N) * 0x4) SSF_SF_INT_EN |
 | |     N[0..2]                                     |
 | +-------------------------------------------------+
 | | bit  31:0 R/W  SSF_SF_RCVD_E                    |
 | +-------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_SSF_SF_RCVD_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_SSF_SF_RCVD_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_SSF_SF_RCVD_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_SSF_SF_RCVD_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register (0x00030390 + (N) * 0x4) HAO_STAT_INT_EN |
 | |     N[0..2]                                       |
 | +---------------------------------------------------+
 | | bit  31:0 R/W  HAO_STATUS_AVL_E                   |
 | +---------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_HAO_STATUS_AVL_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_HAO_STATUS_AVL_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_HAO_STATUS_AVL_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_HAO_STATUS_AVL_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x0003039c + (N) * 0x4) DPLM_INT_EN |
 | |     N[0..2]                                   |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  DPLM_E                         |
 | +-----------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_DPLM_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_DPLM_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_DPLM_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_DPLM_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x000303a8 + (N) * 0x4) OPU_CSF_INT_EN |
 | |     N[0..2]                                      |
 | +--------------------------------------------------+
 | | bit  31:0 R/W  OPU_CSF_E                         |
 | +--------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_OPU_CSF_E_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_OPU_CSF_E_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_OPU_CSF_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_OPU_CSF_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x000303b8 RCOH_STAT_TRANSFR_DONE_EN |
 | +-----------------------------------------------+
 | | bit  0 R/W  RCOH_TRNSFR_SHDW_DNE_E            |
 | +-----------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_RCOH_TRNSFR_SHDW_DNE_E_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_RCOH_TRNSFR_SHDW_DNE_E_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +-------------------------------------------------+
 | | Register (0x000303bc + (N) * 0x4) IN_SFAIL_INTR |
 | |     N[0..2]                                     |
 | +-------------------------------------------------+
 | | bit  31:0 R/W  IN_SFAIL_I                       |
 | +-------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_IN_SFAIL_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_IN_SFAIL_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_IN_SFAIL_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_IN_SFAIL_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_IN_SFAIL_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_IN_SFAIL_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_IN_SFAIL_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_IN_SFAIL_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_IN_SFAIL_INTR_array_poll( b_ptr,
 |                                                       h_ptr,
 |                                                       word_number,
 |                                                       mask,
 |                                                       ( value << field_ofs),
 |                                                       cmp,
 |                                                       max_count,
 |                                                       num_failed_polls,
 |                                                       delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +--------------------------------------------------------------+
 | | Register (0x000303c8 + (N) * 0x4) LOSS_FRME_DELINEATION_INTR |
 | |     N[0..2]                                                  |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  LOFD_I                                        |
 | +--------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_LOFD_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_LOFD_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_LOFD_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_LOFD_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_LOFD_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_LOFD_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_LOFD_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_LOFD_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_LOSS_FRME_DELINEATION_INTR_array_poll( b_ptr,
 |                                                                    h_ptr,
 |                                                                    word_number,
 |                                                                    mask,
 |                                                                    ( value << field_ofs),
 |                                                                    cmp,
 |                                                                    max_count,
 |                                                                    num_failed_polls,
 |                                                                    delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-------------------------------------------------------+
 | | Register (0x000303d4 + (N) * 0x4) GFP_IDLE_FRAME_INTR |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  IDLE_I                                 |
 | +-------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_IDLE_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_IDLE_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_IDLE_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_IDLE_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_IDLE_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_IDLE_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_IDLE_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_IDLE_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_IDLE_FRAME_INTR_array_poll( b_ptr,
 |                                                             h_ptr,
 |                                                             word_number,
 |                                                             mask,
 |                                                             ( value << field_ofs),
 |                                                             cmp,
 |                                                             max_count,
 |                                                             num_failed_polls,
 |                                                             delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x000303e0 + (N) * 0x4) GFP_NON_IDLE_CONTROL_FRAME_INTR |
 | |     N[0..2]                                                       |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  CTRLFRM_I                                          |
 | +-------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CTRLFRM_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_CTRLFRM_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_CTRLFRM_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CTRLFRM_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CTRLFRM_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CTRLFRM_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_CTRLFRM_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_CTRLFRM_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_NON_IDLE_CONTROL_FRAME_INTR_array_poll( b_ptr,
 |                                                                         h_ptr,
 |                                                                         word_number,
 |                                                                         mask,
 |                                                                         ( value << field_ofs),
 |                                                                         cmp,
 |                                                                         max_count,
 |                                                                         num_failed_polls,
 |                                                                         delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +----------------------------------------------------+
 | | Register (0x000303ec + (N) * 0x4) UNPACK_DROP_INTR |
 | |     N[0..2]                                        |
 | +----------------------------------------------------+
 | | bit  31:0 R/W  UNPACK_FIFO_DROP_I                  |
 | +----------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_UNPACK_FIFO_DROP_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_UNPACK_FIFO_DROP_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_UNPACK_FIFO_DROP_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_UNPACK_FIFO_DROP_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_UNPACK_FIFO_DROP_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_UNPACK_FIFO_DROP_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_UNPACK_FIFO_DROP_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_UNPACK_FIFO_DROP_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_UNPACK_DROP_INTR_array_poll( b_ptr,
 |                                                          h_ptr,
 |                                                          word_number,
 |                                                          mask,
 |                                                          ( value << field_ofs),
 |                                                          cmp,
 |                                                          max_count,
 |                                                          num_failed_polls,
 |                                                          delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +--------------------------------------------------------+
 | | Register (0x000303f8 + (N) * 0x4) GFP_UCHEC_ERROR_INTR |
 | |     N[0..2]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R/W  UCHEC_I                                 |
 | +--------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_UCHEC_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_UCHEC_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_UCHEC_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_UCHEC_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_UCHEC_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_UCHEC_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_UCHEC_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_UCHEC_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_UCHEC_ERROR_INTR_array_poll( b_ptr,
 |                                                              h_ptr,
 |                                                              word_number,
 |                                                              mask,
 |                                                              ( value << field_ofs),
 |                                                              cmp,
 |                                                              max_count,
 |                                                              num_failed_polls,
 |                                                              delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +--------------------------------------------------------------+
 | | Register (0x00030404 + (N) * 0x4) GFP_CCHEC_ERROR_ERROR_INTR |
 | |     N[0..2]                                                  |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  CCHEC_I                                       |
 | +--------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CCHEC_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_CCHEC_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_CCHEC_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CCHEC_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CCHEC_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CCHEC_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_CCHEC_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_CCHEC_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_CCHEC_ERROR_ERROR_INTR_array_poll( b_ptr,
 |                                                                    h_ptr,
 |                                                                    word_number,
 |                                                                    mask,
 |                                                                    ( value << field_ofs),
 |                                                                    cmp,
 |                                                                    max_count,
 |                                                                    num_failed_polls,
 |                                                                    delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +---------------------------------------------------------------------+
 | | Register (0x00030410 + (N) * 0x4) GFP_UNCORRECTABLE_THEC_ERROR_INTR |
 | |     N[0..2]                                                         |
 | +---------------------------------------------------------------------+
 | | bit  31:0 R/W  UTHEC_I                                              |
 | +---------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_UTHEC_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_UTHEC_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_UTHEC_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_UTHEC_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_UTHEC_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_UTHEC_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_UTHEC_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_UTHEC_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_UNCORRECTABLE_THEC_ERROR_INTR_array_poll( b_ptr,
 |                                                                           h_ptr,
 |                                                                           word_number,
 |                                                                           mask,
 |                                                                           ( value << field_ofs),
 |                                                                           cmp,
 |                                                                           max_count,
 |                                                                           num_failed_polls,
 |                                                                           delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +--------------------------------------------------------+
 | | Register (0x0003041c + (N) * 0x4) GFP_CTHEC_ERROR_INTR |
 | |     N[0..2]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R/W  CTHEC_I                                 |
 | +--------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CTHEC_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_CTHEC_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_CTHEC_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CTHEC_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CTHEC_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CTHEC_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_CTHEC_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_CTHEC_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_CTHEC_ERROR_INTR_array_poll( b_ptr,
 |                                                              h_ptr,
 |                                                              word_number,
 |                                                              mask,
 |                                                              ( value << field_ofs),
 |                                                              cmp,
 |                                                              max_count,
 |                                                              num_failed_polls,
 |                                                              delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +---------------------------------------------------------------------+
 | | Register (0x00030428 + (N) * 0x4) GFP_UNCORRECTABLE_EHEC_ERROR_INTR |
 | |     N[0..2]                                                         |
 | +---------------------------------------------------------------------+
 | | bit  31:0 R/W  UEHEC_I                                              |
 | +---------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_UEHEC_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_UEHEC_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_UEHEC_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_UEHEC_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_UEHEC_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_UEHEC_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_UEHEC_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_UEHEC_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_array_poll( b_ptr,
 |                                                                           h_ptr,
 |                                                                           word_number,
 |                                                                           mask,
 |                                                                           ( value << field_ofs),
 |                                                                           cmp,
 |                                                                           max_count,
 |                                                                           num_failed_polls,
 |                                                                           delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +--------------------------------------------------------------+
 | | Register (0x00030434 + (N) * 0x4) GFP_CEHEC_ERROR_ERROR_INTR |
 | |     N[0..2]                                                  |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  CEHEC_I                                       |
 | +--------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CEHEC_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_CEHEC_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_CEHEC_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CEHEC_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CEHEC_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CEHEC_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_CEHEC_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_CEHEC_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_CEHEC_ERROR_ERROR_INTR_array_poll( b_ptr,
 |                                                                    h_ptr,
 |                                                                    word_number,
 |                                                                    mask,
 |                                                                    ( value << field_ofs),
 |                                                                    cmp,
 |                                                                    max_count,
 |                                                                    num_failed_polls,
 |                                                                    delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00030440 + (N) * 0x4) GFP_EXI_MISMATCH_ERROR_INTR |
 | |     N[0..2]                                                   |
 | +---------------------------------------------------------------+
 | | bit  31:0 R/W  EXI_I                                          |
 | +---------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_EXI_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_EXI_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_EXI_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_EXI_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_EXI_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_EXI_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_EXI_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_EXI_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_EXI_MISMATCH_ERROR_INTR_array_poll( b_ptr,
 |                                                                     h_ptr,
 |                                                                     word_number,
 |                                                                     mask,
 |                                                                     ( value << field_ofs),
 |                                                                     cmp,
 |                                                                     max_count,
 |                                                                     num_failed_polls,
 |                                                                     delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +------------------------------------------------------------+
 | | Register (0x00030458 + (N) * 0x4) GFP_PTI_UPI_MSM_ERR_INTR |
 | |     N[0..2]                                                |
 | +------------------------------------------------------------+
 | | bit  31:0 R/W  PTI_UPI_MSMCH_I                             |
 | +------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_PTI_UPI_MSMCH_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_PTI_UPI_MSMCH_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_PTI_UPI_MSMCH_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_PTI_UPI_MSMCH_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_PTI_UPI_MSMCH_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_PTI_UPI_MSMCH_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_PTI_UPI_MSMCH_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_PTI_UPI_MSMCH_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_PTI_UPI_MSM_ERR_INTR_array_poll( b_ptr,
 |                                                                  h_ptr,
 |                                                                  word_number,
 |                                                                  mask,
 |                                                                  ( value << field_ofs),
 |                                                                  cmp,
 |                                                                  max_count,
 |                                                                  num_failed_polls,
 |                                                                  delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x00030464 + (N) * 0x4) GFP_GENERIC_MISMATCH_ERROR_INTR |
 | |     N[0..2]                                                       |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R/W  GENERIC_I                                          |
 | +-------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_GENERIC_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_GENERIC_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_GENERIC_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_GENERIC_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_GENERIC_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_GENERIC_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_GENERIC_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_GENERIC_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_GENERIC_MISMATCH_ERROR_INTR_array_poll( b_ptr,
 |                                                                         h_ptr,
 |                                                                         word_number,
 |                                                                         mask,
 |                                                                         ( value << field_ofs),
 |                                                                         cmp,
 |                                                                         max_count,
 |                                                                         num_failed_polls,
 |                                                                         delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +------------------------------------------------+
 | | Register (0x00030470 + (N) * 0x4) GFP_CSF_INTR |
 | |     N[0..2]                                    |
 | +------------------------------------------------+
 | | bit  31:0 R/W  CSF_I                           |
 | +------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CSF_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_CSF_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_CSF_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CSF_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CSF_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CSF_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_CSF_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_CSF_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_CSF_INTR_array_poll( b_ptr,
 |                                                      h_ptr,
 |                                                      word_number,
 |                                                      mask,
 |                                                      ( value << field_ofs),
 |                                                      cmp,
 |                                                      max_count,
 |                                                      num_failed_polls,
 |                                                      delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +------------------------------------+
 | | Register 0x0003047c CSF_ST_AV_INTR |
 | +------------------------------------+
 | | bit  0 R/W  CSF_ST_AV_I            |
 | +------------------------------------+
 | 
 | void                 mapotn_rgfpf_field_CSF_ST_AV_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32               mapotn_rgfpf_field_CSF_ST_AV_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_field_CSF_ST_AV_I_poll         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x00030480 CSF_ST_FIFO_OF_INTR |
 | +-----------------------------------------+
 | | bit  0 R/W  CSF_ST_FIFO_OF_I            |
 | +-----------------------------------------+
 | 
 | void                 mapotn_rgfpf_field_CSF_ST_FIFO_OF_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32               mapotn_rgfpf_field_CSF_ST_FIFO_OF_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_field_CSF_ST_FIFO_OF_I_poll         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00030484 + (N) * 0x4) GFP_DROPPED_FRAME_INTR |
 | |     N[0..2]                                              |
 | +----------------------------------------------------------+
 | | bit  31:0 R/W  DROP_I                                    |
 | +----------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_DROP_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_DROP_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_DROP_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_DROP_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_DROP_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_DROP_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_DROP_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_DROP_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_DROPPED_FRAME_INTR_array_poll( b_ptr,
 |                                                                h_ptr,
 |                                                                word_number,
 |                                                                mask,
 |                                                                ( value << field_ofs),
 |                                                                cmp,
 |                                                                max_count,
 |                                                                num_failed_polls,
 |                                                                delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +--------------------------------------------------------------+
 | | Register (0x00030490 + (N) * 0x4) GFP_CLIENT_DATA_FRAME_INTR |
 | |     N[0..2]                                                  |
 | +--------------------------------------------------------------+
 | | bit  31:0 R/W  DATAFRM_I                                     |
 | +--------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_DATAFRM_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_DATAFRM_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_DATAFRM_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_DATAFRM_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_DATAFRM_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_DATAFRM_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_DATAFRM_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_DATAFRM_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_CLIENT_DATA_FRAME_INTR_array_poll( b_ptr,
 |                                                                    h_ptr,
 |                                                                    word_number,
 |                                                                    mask,
 |                                                                    ( value << field_ofs),
 |                                                                    cmp,
 |                                                                    max_count,
 |                                                                    num_failed_polls,
 |                                                                    delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +----------------------------------------------+
 | | Register (0x0003049c + (N) * 0x4) PAUSE_INTR |
 | |     N[0..2]                                  |
 | +----------------------------------------------+
 | | bit  31:0 R/W  PAUSE_I                       |
 | +----------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_PAUSE_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_PAUSE_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_PAUSE_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_PAUSE_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_PAUSE_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_PAUSE_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_PAUSE_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_PAUSE_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_PAUSE_INTR_array_poll( b_ptr,
 |                                                    h_ptr,
 |                                                    word_number,
 |                                                    mask,
 |                                                    ( value << field_ofs),
 |                                                    cmp,
 |                                                    max_count,
 |                                                    num_failed_polls,
 |                                                    delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-----------------------------------------------------------------------+
 | | Register (0x000304a8 + (N) * 0x4) MAXIMUM_FRAME_LENGTH_VIOLATION_INTR |
 | |     N[0..2]                                                           |
 | +-----------------------------------------------------------------------+
 | | bit  31:0 R/W  MAX_FRM_LEN_I                                          |
 | +-----------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_MAX_FRM_LEN_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_MAX_FRM_LEN_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_MAX_FRM_LEN_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_MAX_FRM_LEN_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_MAX_FRM_LEN_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_MAX_FRM_LEN_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_MAX_FRM_LEN_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_MAX_FRM_LEN_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR_array_poll( b_ptr,
 |                                                                             h_ptr,
 |                                                                             word_number,
 |                                                                             mask,
 |                                                                             ( value << field_ofs),
 |                                                                             cmp,
 |                                                                             max_count,
 |                                                                             num_failed_polls,
 |                                                                             delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-----------------------------------------------------------------------+
 | | Register (0x000304b4 + (N) * 0x4) MINIMUM_FRAME_LENGTH_VIOLATION_INTR |
 | |     N[0..2]                                                           |
 | +-----------------------------------------------------------------------+
 | | bit  31:0 R/W  MIN_FRM_LEN_I                                          |
 | +-----------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_MIN_FRM_LEN_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_MIN_FRM_LEN_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_MIN_FRM_LEN_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_MIN_FRM_LEN_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_MIN_FRM_LEN_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_MIN_FRM_LEN_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_MIN_FRM_LEN_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_MIN_FRM_LEN_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_MINIMUM_FRAME_LENGTH_VIOLATION_INTR_array_poll( b_ptr,
 |                                                                             h_ptr,
 |                                                                             word_number,
 |                                                                             mask,
 |                                                                             ( value << field_ofs),
 |                                                                             cmp,
 |                                                                             max_count,
 |                                                                             num_failed_polls,
 |                                                                             delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-----------------------------------------------------------------+
 | | Register (0x000304c0 + (N) * 0x4 + (M) * 0xc) FRM_LEN_STAT_INTR |
 | |     N[0..2]                                                     |
 | |     M[0..6]                                                     |
 | +-----------------------------------------------------------------+
 | | bit  31:0 R/W  FRM_LEN_STAT_INTR_I                              |
 | +-----------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_FRM_LEN_STAT_INTR_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_FRM_LEN_STAT_INTR_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_FRM_LEN_STAT_INTR_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_FRM_LEN_STAT_INTR_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_FRM_LEN_STAT_INTR_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32  M,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_FRM_LEN_STAT_INTR_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32  M,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     if (M > 6)
 |         IO_RANGE_CHECK("%s M is %d but max is %d", "mapotn_rgfpf_lfield_range_FRM_LEN_STAT_INTR_I_poll", M, 6);
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_FRM_LEN_STAT_INTR_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> M=%d start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_FRM_LEN_STAT_INTR_I_set_to_clear", M, start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_FRM_LEN_STAT_INTR_array_poll( b_ptr,
 |                                                           h_ptr,
 |                                                           M,
 |                                                           word_number,
 |                                                           mask,
 |                                                           ( value << field_ofs),
 |                                                           cmp,
 |                                                           max_count,
 |                                                           num_failed_polls,
 |                                                           delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +----------------------------------------------------+
 | | Register (0x00030514 + (N) * 0x4) PAYLOAD_FCS_INTR |
 | |     N[0..2]                                        |
 | +----------------------------------------------------+
 | | bit  31:0 R/W  PFCSE_I                             |
 | +----------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_PFCSE_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_PFCSE_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_PFCSE_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_PFCSE_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_PFCSE_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_PFCSE_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_PFCSE_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_PFCSE_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_PAYLOAD_FCS_INTR_array_poll( b_ptr,
 |                                                          h_ptr,
 |                                                          word_number,
 |                                                          mask,
 |                                                          ( value << field_ofs),
 |                                                          cmp,
 |                                                          max_count,
 |                                                          num_failed_polls,
 |                                                          delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +---------------------------------------------------------+
 | | Register (0x00030520 + (N) * 0x4) DOWNSTREAM_FRAME_INTR |
 | |     N[0..2]                                             |
 | +---------------------------------------------------------+
 | | bit  31:0 R/W  DWNSTRM_FRM_I                            |
 | +---------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_DWNSTRM_FRM_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_DWNSTRM_FRM_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_DWNSTRM_FRM_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_DWNSTRM_FRM_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_DWNSTRM_FRM_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_DWNSTRM_FRM_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_DWNSTRM_FRM_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_DWNSTRM_FRM_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_DOWNSTREAM_FRAME_INTR_array_poll( b_ptr,
 |                                                               h_ptr,
 |                                                               word_number,
 |                                                               mask,
 |                                                               ( value << field_ofs),
 |                                                               cmp,
 |                                                               max_count,
 |                                                               num_failed_polls,
 |                                                               delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +---------------------------------------------------------+
 | | Register (0x0003052c + (N) * 0x4) CPU_FIFO_DATA_AV_INTR |
 | |     N[0..2]                                             |
 | +---------------------------------------------------------+
 | | bit  31:0 R/W  CPU_FIFO_DATA_AV_I                       |
 | +---------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CPU_FIFO_DATA_AV_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_CPU_FIFO_DATA_AV_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_CPU_FIFO_DATA_AV_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CPU_FIFO_DATA_AV_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CPU_FIFO_DATA_AV_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CPU_FIFO_DATA_AV_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_CPU_FIFO_DATA_AV_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_CPU_FIFO_DATA_AV_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_CPU_FIFO_DATA_AV_INTR_array_poll( b_ptr,
 |                                                               h_ptr,
 |                                                               word_number,
 |                                                               mask,
 |                                                               ( value << field_ofs),
 |                                                               cmp,
 |                                                               max_count,
 |                                                               num_failed_polls,
 |                                                               delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00030538 + (N) * 0x4) CPU_DATA_FIFO_OVERFLOW_INTR |
 | |     N[0..2]                                                   |
 | +---------------------------------------------------------------+
 | | bit  31:0 R/W  CPU_FIFO_OVF_I                                 |
 | +---------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CPU_FIFO_OVF_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_CPU_FIFO_OVF_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_CPU_FIFO_OVF_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_CPU_FIFO_OVF_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CPU_FIFO_OVF_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CPU_FIFO_OVF_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_CPU_FIFO_OVF_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_CPU_FIFO_OVF_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_CPU_DATA_FIFO_OVERFLOW_INTR_array_poll( b_ptr,
 |                                                                     h_ptr,
 |                                                                     word_number,
 |                                                                     mask,
 |                                                                     ( value << field_ofs),
 |                                                                     cmp,
 |                                                                     max_count,
 |                                                                     num_failed_polls,
 |                                                                     delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +----------------------------------------+
 | | Register 0x00030544 PMON_TRANSFER_INTR |
 | +----------------------------------------+
 | | bit  1 R/W  PMON_TRFR_END_I            |
 | | bit  0 R/W  PMON_TRFR_START_I          |
 | +----------------------------------------+
 | 
 | void                 mapotn_rgfpf_field_PMON_TRFR_END_I_set_to_clear   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32               mapotn_rgfpf_field_PMON_TRFR_END_I_get            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_field_PMON_TRFR_END_I_poll           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mapotn_rgfpf_field_PMON_TRFR_START_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32               mapotn_rgfpf_field_PMON_TRFR_START_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_field_PMON_TRFR_START_I_poll         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x00030548 ASYNC_FIFO_OVF_INTR |
 | +-----------------------------------------+
 | | bit  2 R/W  CPU_WRRPT_AFIFO_OVF_I       |
 | | bit  1 R/W  CPU_RDRPT_AFIFO_OVF_I       |
 | | bit  0 R/W  UNPACK_AFIFO_OVF_I          |
 | +-----------------------------------------+
 | 
 | void                 mapotn_rgfpf_field_CPU_WRRPT_AFIFO_OVF_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32               mapotn_rgfpf_field_CPU_WRRPT_AFIFO_OVF_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_field_CPU_WRRPT_AFIFO_OVF_I_poll         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mapotn_rgfpf_field_CPU_RDRPT_AFIFO_OVF_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32               mapotn_rgfpf_field_CPU_RDRPT_AFIFO_OVF_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_field_CPU_RDRPT_AFIFO_OVF_I_poll         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mapotn_rgfpf_field_UNPACK_AFIFO_OVF_I_set_to_clear    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32               mapotn_rgfpf_field_UNPACK_AFIFO_OVF_I_get             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_field_UNPACK_AFIFO_OVF_I_poll            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------------+
 | | Register (0x00030558 + (N) * 0x4) SSF_SF_RCVDINT |
 | |     N[0..2]                                      |
 | +--------------------------------------------------+
 | | bit  31:0 R/W  SSF_SF_RCVD_I                     |
 | +--------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_SSF_SF_RCVD_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_SSF_SF_RCVD_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_SSF_SF_RCVD_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_SSF_SF_RCVD_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_SSF_SF_RCVD_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_SSF_SF_RCVD_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_SSF_SF_RCVD_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_SSF_SF_RCVD_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_SSF_SF_RCVDINT_array_poll( b_ptr,
 |                                                        h_ptr,
 |                                                        word_number,
 |                                                        mask,
 |                                                        ( value << field_ofs),
 |                                                        cmp,
 |                                                        max_count,
 |                                                        num_failed_polls,
 |                                                        delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +------------------------------------------------+
 | | Register (0x00030564 + (N) * 0x4) HAO_STAT_INT |
 | |     N[0..2]                                    |
 | +------------------------------------------------+
 | | bit  31:0 R/W  HAO_STATUS_AVL_I                |
 | +------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_HAO_STATUS_AVL_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_HAO_STATUS_AVL_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_HAO_STATUS_AVL_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_HAO_STATUS_AVL_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_HAO_STATUS_AVL_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_HAO_STATUS_AVL_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_HAO_STATUS_AVL_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_HAO_STATUS_AVL_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_HAO_STAT_INT_array_poll( b_ptr,
 |                                                      h_ptr,
 |                                                      word_number,
 |                                                      mask,
 |                                                      ( value << field_ofs),
 |                                                      cmp,
 |                                                      max_count,
 |                                                      num_failed_polls,
 |                                                      delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +--------------------------------------------+
 | | Register (0x00030570 + (N) * 0x4) DPLM_INT |
 | |     N[0..2]                                |
 | +--------------------------------------------+
 | | bit  31:0 R/W  DPLM_I                      |
 | +--------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_DPLM_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_DPLM_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_DPLM_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_DPLM_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_DPLM_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_DPLM_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_DPLM_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_DPLM_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_DPLM_INT_array_poll( b_ptr,
 |                                                  h_ptr,
 |                                                  word_number,
 |                                                  mask,
 |                                                  ( value << field_ofs),
 |                                                  cmp,
 |                                                  max_count,
 |                                                  num_failed_polls,
 |                                                  delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-----------------------------------------------+
 | | Register (0x0003057c + (N) * 0x4) OPU_CSF_INT |
 | |     N[0..2]                                   |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  OPU_CSF_I                      |
 | +-----------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_OPU_CSF_I_set_to_clear       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_OPU_CSF_I_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_OPU_CSF_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_OPU_CSF_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_OPU_CSF_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_OPU_CSF_I_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_OPU_CSF_I_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_OPU_CSF_I_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_OPU_CSF_INT_array_poll( b_ptr,
 |                                                     h_ptr,
 |                                                     word_number,
 |                                                     mask,
 |                                                     ( value << field_ofs),
 |                                                     cmp,
 |                                                     max_count,
 |                                                     num_failed_polls,
 |                                                     delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-------------------------------------------------+
 | | Register 0x0003058c RCOH_STAT_TRANSFER_DONE_INT |
 | +-------------------------------------------------+
 | | bit  0 R/W  RCOH_TRNSFR_SHDW_DNE_I              |
 | +-------------------------------------------------+
 | 
 | void                 mapotn_rgfpf_field_RCOH_TRNSFR_SHDW_DNE_I_set_to_clear ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32               mapotn_rgfpf_field_RCOH_TRNSFR_SHDW_DNE_I_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_field_RCOH_TRNSFR_SHDW_DNE_I_poll         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------------+
 | | Register (0x00030590 + (N) * 0x4)         |
 | | LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE |
 | |     N[0..2]                               |
 | +-------------------------------------------+
 | | bit  31:0 R  LOFD_V                       |
 | +-------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_LOFD_V_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 mapotn_rgfpf_lfield_range_LOFD_V_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_LOFD_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_LOFD_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_LOFD_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_LOFD_V_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE_array_poll( b_ptr,
 |                                                                                   h_ptr,
 |                                                                                   word_number,
 |                                                                                   mask,
 |                                                                                   ( value << field_ofs),
 |                                                                                   cmp,
 |                                                                                   max_count,
 |                                                                                   num_failed_polls,
 |                                                                                   delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-------------------------------------------------------------------+
 | | Register (0x0003059c + (N) * 0x4) GFP_FRAMER_IDLE_INTERRUPT_VALUE |
 | |     N[0..2]                                                       |
 | +-------------------------------------------------------------------+
 | | bit  31:0 R  IDLE_V                                               |
 | +-------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_IDLE_V_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 mapotn_rgfpf_lfield_range_IDLE_V_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_IDLE_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_IDLE_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_IDLE_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_IDLE_V_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_FRAMER_IDLE_INTERRUPT_VALUE_array_poll( b_ptr,
 |                                                                         h_ptr,
 |                                                                         word_number,
 |                                                                         mask,
 |                                                                         ( value << field_ofs),
 |                                                                         cmp,
 |                                                                         max_count,
 |                                                                         num_failed_polls,
 |                                                                         delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-----------------------------------------------------------+
 | | Register (0x000305a8 + (N) * 0x4) GFP_CSF_INTERRUPT_VALUE |
 | |     N[0..2]                                               |
 | +-----------------------------------------------------------+
 | | bit  31:0 R  CSF_V                                        |
 | +-----------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_CSF_V_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 mapotn_rgfpf_lfield_range_CSF_V_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CSF_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_CSF_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_CSF_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_CSF_V_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_CSF_INTERRUPT_VALUE_array_poll( b_ptr,
 |                                                                 h_ptr,
 |                                                                 word_number,
 |                                                                 mask,
 |                                                                 ( value << field_ofs),
 |                                                                 cmp,
 |                                                                 max_count,
 |                                                                 num_failed_polls,
 |                                                                 delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +---------------------------------------------------------------------+
 | | Register (0x000305b4 + (N) * 0x4) GFP_EXI_MISMATCH_ERROR_INTR_VALUE |
 | |     N[0..2]                                                         |
 | +---------------------------------------------------------------------+
 | | bit  31:0 R  EXI_V                                                  |
 | +---------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_EXI_V_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 mapotn_rgfpf_lfield_range_EXI_V_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_EXI_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_EXI_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_EXI_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_EXI_V_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_EXI_MISMATCH_ERROR_INTR_VALUE_array_poll( b_ptr,
 |                                                                           h_ptr,
 |                                                                           word_number,
 |                                                                           mask,
 |                                                                           ( value << field_ofs),
 |                                                                           cmp,
 |                                                                           max_count,
 |                                                                           num_failed_polls,
 |                                                                           delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +------------------------------------------------------------------+
 | | Register (0x000305c0 + (N) * 0x4) GFP_PTI_UPI_MSM_ERR_INTR_VALUE |
 | |     N[0..2]                                                      |
 | +------------------------------------------------------------------+
 | | bit  31:0 R  PTI_UPI_MSMCH_V                                     |
 | +------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_PTI_UPI_MSMCH_V_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 mapotn_rgfpf_lfield_range_PTI_UPI_MSMCH_V_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_PTI_UPI_MSMCH_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_PTI_UPI_MSMCH_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_PTI_UPI_MSMCH_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_PTI_UPI_MSMCH_V_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_GFP_PTI_UPI_MSM_ERR_INTR_VALUE_array_poll( b_ptr,
 |                                                                        h_ptr,
 |                                                                        word_number,
 |                                                                        mask,
 |                                                                        ( value << field_ofs),
 |                                                                        cmp,
 |                                                                        max_count,
 |                                                                        num_failed_polls,
 |                                                                        delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +----------------------------------------------------+
 | | Register (0x000305cc + (N) * 0x4) IN_SFAIL_INT_VAL |
 | |     N[0..2]                                        |
 | +----------------------------------------------------+
 | | bit  31:0 R  IN_SFAIL_V                            |
 | +----------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_IN_SFAIL_V_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 mapotn_rgfpf_lfield_range_IN_SFAIL_V_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_IN_SFAIL_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_IN_SFAIL_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_IN_SFAIL_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_IN_SFAIL_V_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_IN_SFAIL_INT_VAL_array_poll( b_ptr,
 |                                                          h_ptr,
 |                                                          word_number,
 |                                                          mask,
 |                                                          ( value << field_ofs),
 |                                                          cmp,
 |                                                          max_count,
 |                                                          num_failed_polls,
 |                                                          delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +-------------------------------------------------------+
 | | Register (0x000305d8 + (N) * 0x4) SSF_SF_RCVD_INT_VAL |
 | |     N[0..2]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R  SSF_SF_RCVD_V                            |
 | +-------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_SSF_SF_RCVD_V_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 mapotn_rgfpf_lfield_range_SSF_SF_RCVD_V_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_SSF_SF_RCVD_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_SSF_SF_RCVD_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_SSF_SF_RCVD_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_SSF_SF_RCVD_V_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_SSF_SF_RCVD_INT_VAL_array_poll( b_ptr,
 |                                                             h_ptr,
 |                                                             word_number,
 |                                                             mask,
 |                                                             ( value << field_ofs),
 |                                                             cmp,
 |                                                             max_count,
 |                                                             num_failed_polls,
 |                                                             delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +------------------------------------------------+
 | | Register (0x000305e4 + (N) * 0x4) DPLM_INT_VAL |
 | |     N[0..2]                                    |
 | +------------------------------------------------+
 | | bit  31:0 R  DPLM_V                            |
 | +------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_DPLM_V_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 mapotn_rgfpf_lfield_range_DPLM_V_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_DPLM_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_DPLM_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_DPLM_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_DPLM_V_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_DPLM_INT_VAL_array_poll( b_ptr,
 |                                                      h_ptr,
 |                                                      word_number,
 |                                                      mask,
 |                                                      ( value << field_ofs),
 |                                                      cmp,
 |                                                      max_count,
 |                                                      num_failed_polls,
 |                                                      delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +---------------------------------------------------+
 | | Register (0x000305f0 + (N) * 0x4) OPU_CSF_INT_VAL |
 | |     N[0..2]                                       |
 | +---------------------------------------------------+
 | | bit  31:0 R  OPU_CSF_V                            |
 | +---------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_OPU_CSF_V_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | UINT32 mapotn_rgfpf_lfield_range_OPU_CSF_V_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_OPU_CSF_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds ) ALWAYS_INLINE;
 | static INLINE PMC_POLL_RETURN_TYPE mapotn_rgfpf_lfield_range_OPU_CSF_V_poll( mapotn_rgfpf_buffer_t *b_ptr,
 | mapotn_handle_t *h_ptr,
 | UINT32 start_bit,
 | UINT32 stop_bit,
 | UINT32 value,
 | PMC_POLL_COMPARISON_TYPE cmp,
 | UINT32 max_count,
 | UINT32 *num_failed_polls,
 | UINT32 delay_between_polls_in_microseconds )
 | {
 |     UINT32 word_number;
 |     UINT32 field_ofs;
 |     UINT32 num_bits;
 |     UINT32 mask;
 | 
 |     word_number = start_bit / 32;
 |     field_ofs = start_bit % 32;
 |     num_bits = stop_bit - start_bit + 1;
 |     mask = (0xffffffff >> (32-num_bits)) << field_ofs;
 |     if (stop_bit / 32 != word_number) {
 |         IOLOG( "BUG: %s does not support ranges that span multiple registers -- fix code generator - start_bit=%d, stop_bit=%d", "mapotn_rgfpf_lfield_range_OPU_CSF_V_poll", start_bit, stop_bit );
 |     }
 |     IOLOG( "%s -> start_bit=%d stop_bit=%d", "mapotn_rgfpf_lfield_range_OPU_CSF_V_set_to_clear", start_bit, stop_bit);
 |     return mapotn_rgfpf_reg_OPU_CSF_INT_VAL_array_poll( b_ptr,
 |                                                         h_ptr,
 |                                                         word_number,
 |                                                         mask,
 |                                                         ( value << field_ofs),
 |                                                         cmp,
 |                                                         max_count,
 |                                                         num_failed_polls,
 |                                                         delay_between_polls_in_microseconds);
 | }
 | 
 | 
 | +----------------------------------------------------------+
 | | Register (0x00030600 + (N) * 0x4) DCSF_DEASSERT_REGISTER |
 | |     N[0..2]                                              |
 | +----------------------------------------------------------+
 | | bit  31:0 R/W  DCSF_DEASSERT                             |
 | +----------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_DCSF_DEASSERT_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_DCSF_DEASSERT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value[3] )
 | void   mapotn_rgfpf_lfield_range_DCSF_DEASSERT_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_lfield_range_DCSF_DEASSERT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00030700 MUTEX_CPU_DATA_FIFO |
 | +-----------------------------------------+
 | | bit  0 R/W  MTX_CPU_DATA_FIFO           |
 | +-----------------------------------------+
 | 
 | void   mapotn_rgfpf_field_MTX_CPU_DATA_FIFO_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_MTX_CPU_DATA_FIFO_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +-------------------------------------------+
 | | Register 0x00030880 MUTEX_CSF_STATUS_FIFO |
 | +-------------------------------------------+
 | | bit  0 R/W  MTX_CSF_STATUS_FIFO           |
 | +-------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_MTX_CSF_STATUS_FIFO_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | UINT32 mapotn_rgfpf_field_MTX_CSF_STATUS_FIFO_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | +-----------------------------------------------------------------+
 | | Register (0x00031000 + (N) * 0x100) GFP_CHANNEL_CONFIGURATION_0 |
 | |     N[0..95]                                                    |
 | +-----------------------------------------------------------------+
 | | bit  30    R/W  SHUT_DATA                                       |
 | | bit  29    R/W  DATA_REV_IBIT                                   |
 | | bit  28    R/W  DC_BALANCE                                      |
 | | bit  27    R/W  CHEC_ECC                                        |
 | | bit  26:25 R/W  DELTA                                           |
 | | bit  23:16 R/W  UNPACK_DROP_THRLD2_MAX                          |
 | | bit  15:8  R/W  UNPACK_DROP_THRLD2_MIN                          |
 | | bit  7:0   R/W  UNPACK_DROP_THRLD1                              |
 | +-----------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_SHUT_DATA_set                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_SHUT_DATA_get                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_DATA_REV_IBIT_set                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DATA_REV_IBIT_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_DC_BALANCE_set                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DC_BALANCE_get                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_CHEC_ECC_set                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_CHEC_ECC_get                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_DELTA_set                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DELTA_get                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_DELTA_set                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_DELTA_get                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_UNPACK_DROP_THRLD2_MAX_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_UNPACK_DROP_THRLD2_MAX_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_UNPACK_DROP_THRLD2_MAX_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_UNPACK_DROP_THRLD2_MAX_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_UNPACK_DROP_THRLD2_MIN_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_UNPACK_DROP_THRLD2_MIN_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_UNPACK_DROP_THRLD2_MIN_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_UNPACK_DROP_THRLD2_MIN_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_UNPACK_DROP_THRLD1_set           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_UNPACK_DROP_THRLD1_get           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_UNPACK_DROP_THRLD1_set     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_UNPACK_DROP_THRLD1_get     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------------+
 | | Register (0x00031004 + (N) * 0x100) GFP_CHANNEL_CONFIGURATION_1 |
 | |     N[0..95]                                                    |
 | +-----------------------------------------------------------------+
 | | bit  30    R/W  PAUSE_MODE                                      |
 | | bit  29    R/W  PAUSE_FILTER                                    |
 | | bit  28    R/W  PAUSE_ENABLE                                    |
 | | bit  27:26 R/W  PREAMBLE_SFD_MODE                               |
 | | bit  25:20 R/W  EXI_LENGTH2                                     |
 | | bit  19:16 R/W  EXI_VALUE2                                      |
 | | bit  15:10 R/W  EXI_LENGTH1                                     |
 | | bit  9:6   R/W  EXI_VALUE1                                      |
 | | bit  5     R/W  EHEC_ECC                                        |
 | | bit  4     R/W  EHEC_CHK                                        |
 | | bit  3     R/W  THEC_ECC                                        |
 | | bit  2     R/W  THEC_CHK                                        |
 | | bit  1     R/W  DESCRMBL                                        |
 | +-----------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_PAUSE_MODE_set              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PAUSE_MODE_get              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_PAUSE_FILTER_set            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PAUSE_FILTER_get            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_PAUSE_ENABLE_set            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PAUSE_ENABLE_get            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_PREAMBLE_SFD_MODE_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PREAMBLE_SFD_MODE_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_PREAMBLE_SFD_MODE_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_PREAMBLE_SFD_MODE_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_EXI_LENGTH2_set             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_EXI_LENGTH2_get             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_EXI_LENGTH2_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_EXI_LENGTH2_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_EXI_VALUE2_set              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_EXI_VALUE2_get              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_EXI_VALUE2_set        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_EXI_VALUE2_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_EXI_LENGTH1_set             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_EXI_LENGTH1_get             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_EXI_LENGTH1_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_EXI_LENGTH1_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_EXI_VALUE1_set              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_EXI_VALUE1_get              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_EXI_VALUE1_set        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_EXI_VALUE1_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_EHEC_ECC_set                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_EHEC_ECC_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_EHEC_CHK_set                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_EHEC_CHK_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_THEC_ECC_set                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_THEC_ECC_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_THEC_CHK_set                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_THEC_CHK_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_DESCRMBL_set                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DESCRMBL_get                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------------------------------+
 | | Register (0x00031008 + (N) * 0x100) GFP_CHANNEL_CONFIGURATION_2 |
 | |     N[0..95]                                                    |
 | +-----------------------------------------------------------------+
 | | bit  30    R/W  CPU_ONLY1BUF                                    |
 | | bit  29    R/W  DATA_REV_OBIT                                   |
 | | bit  28    R/W  EGRESS_FCS_DISCARD                              |
 | | bit  27    R/W  FC1200_TRAFFIC                                  |
 | | bit  26:25 R/W  EGRESS_FORMAT                                   |
 | | bit  24:23 R/W  CPU_FIFO_FORMAT                                 |
 | | bit  20    R/W  EGRESS_EFCS_STRIP                               |
 | | bit  19    R/W  EGRESS_PFCS_STRIP                               |
 | | bit  18    R/W  CNT_UDF_FCS                                     |
 | | bit  17:16 R/W  GFP_FCS_CHK_MODE                                |
 | | bit  15    R/W  PFCS_REV_FCSBYTE                                |
 | | bit  14    R/W  PFCS_REV_FCSBIT                                 |
 | | bit  13    R/W  PFCS_REV_PLDBIT                                 |
 | | bit  12    R/W  PFCS_INV                                        |
 | | bit  11    R/W  PFCS_INIT                                       |
 | | bit  10:3  R/W  PAUSE_UPI                                       |
 | | bit  2:0   R/W  PAUSE_PTI                                       |
 | +-----------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_CPU_ONLY1BUF_set           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_CPU_ONLY1BUF_get           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_DATA_REV_OBIT_set          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DATA_REV_OBIT_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_EGRESS_FCS_DISCARD_set     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_EGRESS_FCS_DISCARD_get     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_FC1200_TRAFFIC_set         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_FC1200_TRAFFIC_get         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_EGRESS_FORMAT_set          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_EGRESS_FORMAT_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_EGRESS_FORMAT_set    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_EGRESS_FORMAT_get    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_CPU_FIFO_FORMAT_set        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_CPU_FIFO_FORMAT_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_CPU_FIFO_FORMAT_set  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_CPU_FIFO_FORMAT_get  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_EGRESS_EFCS_STRIP_set      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_EGRESS_EFCS_STRIP_get      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_EGRESS_PFCS_STRIP_set      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_EGRESS_PFCS_STRIP_get      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_CNT_UDF_FCS_set            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_CNT_UDF_FCS_get            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_GFP_FCS_CHK_MODE_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_GFP_FCS_CHK_MODE_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_GFP_FCS_CHK_MODE_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_GFP_FCS_CHK_MODE_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_PFCS_REV_FCSBYTE_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PFCS_REV_FCSBYTE_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_PFCS_REV_FCSBIT_set        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PFCS_REV_FCSBIT_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_PFCS_REV_PLDBIT_set        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PFCS_REV_PLDBIT_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_PFCS_INV_set               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PFCS_INV_get               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_PFCS_INIT_set              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PFCS_INIT_get              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_PAUSE_UPI_set              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PAUSE_UPI_get              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_PAUSE_UPI_set        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_PAUSE_UPI_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_PAUSE_PTI_set              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PAUSE_PTI_get              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_PAUSE_PTI_set        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_PAUSE_PTI_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------------+
 | | Register (0x0003100c + (N) * 0x100) GFP_CHANNEL_CONFIGURATION_3 |
 | |     N[0..95]                                                    |
 | +-----------------------------------------------------------------+
 | | bit  8 R/W  I_SFAIL_PROPAGATE                                   |
 | | bit  7 R/W  DLFD_SFAIL                                          |
 | | bit  4 R/W  DEXM_SFAIL                                          |
 | | bit  3 R/W  DLFD_EN                                             |
 | | bit  2 R/W  DCSF_EN                                             |
 | | bit  1 R/W  DUPM_EN                                             |
 | | bit  0 R/W  DEXM_EN                                             |
 | +-----------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_I_SFAIL_PROPAGATE_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_I_SFAIL_PROPAGATE_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_DLFD_SFAIL_set        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DLFD_SFAIL_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_DEXM_SFAIL_set        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DEXM_SFAIL_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_DLFD_EN_set           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DLFD_EN_get           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_DCSF_EN_set           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DCSF_EN_get           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_DUPM_EN_set           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DUPM_EN_get           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_DEXM_EN_set           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_DEXM_EN_get           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | +-----------------------------------------------------------------+
 | | Register (0x00031010 + (N) * 0x100) GFP_CHANNEL_CONFIGURATION_4 |
 | |     N[0..95]                                                    |
 | +-----------------------------------------------------------------+
 | | bit  24:16 R/W  UNPACK_XOFF_THRLD2                              |
 | | bit  15:8  R/W  EGRESS_XOFF_THRLD                               |
 | | bit  7:0   R/W  UNPACK_XOFF_THRLD                               |
 | +-----------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_UNPACK_XOFF_THRLD2_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_UNPACK_XOFF_THRLD2_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_UNPACK_XOFF_THRLD2_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_UNPACK_XOFF_THRLD2_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_EGRESS_XOFF_THRLD_set        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_EGRESS_XOFF_THRLD_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_EGRESS_XOFF_THRLD_set  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_EGRESS_XOFF_THRLD_get  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_UNPACK_XOFF_THRLD_set        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_UNPACK_XOFF_THRLD_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_UNPACK_XOFF_THRLD_set  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_UNPACK_XOFF_THRLD_get  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00031020 + (N) * 0x4 + (M) * 0x100) |
 | | FRM_LEN_STAT_COUNTER_CONFIG                     |
 | |     N[0..6]                                     |
 | |     M[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  31:16 R/W  MINFL_TR                        |
 | | bit  15:0  R/W  MAXFL_TR                        |
 | +-------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_MINFL_TR_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 mapotn_rgfpf_field_MINFL_TR_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   mapotn_rgfpf_field_range_MINFL_TR_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_MINFL_TR_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_MAXFL_TR_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 mapotn_rgfpf_field_MAXFL_TR_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   mapotn_rgfpf_field_range_MAXFL_TR_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_MAXFL_TR_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------------+
 | | Register (0x0003103c + (N) * 0x100) FRAME_LENGTH_CHECK_CONFIGURATION |
 | |     N[0..95]                                                         |
 | +----------------------------------------------------------------------+
 | | bit  31:16 R/W  MAXFL                                                |
 | | bit  15:0  R/W  MINFL                                                |
 | +----------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_MAXFL_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_MAXFL_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_MAXFL_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_MAXFL_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_MINFL_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_MINFL_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_MINFL_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_MINFL_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------+
 | | Register (0x00031040 + (N) * 0x100) CPU_DATA_FIFO_STATUS_RESET |
 | |     N[0..95]                                                   |
 | +----------------------------------------------------------------+
 | | bit  31   R/W  CPU_RST                                         |
 | | bit  15:8 R    CPU_NUMBYTES                                    |
 | | bit  3    R    CPU_ERR_AV                                      |
 | | bit  2    R    CPU_EOF_AV                                      |
 | | bit  1    R    CPU_SOF_AV                                      |
 | | bit  0    R    CPU_DATA_AV                                     |
 | +----------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_CPU_RST_set            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_CPU_RST_get            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_CPU_NUMBYTES_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_CPU_NUMBYTES_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mapotn_rgfpf_field_CPU_ERR_AV_get         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_CPU_EOF_AV_get         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_CPU_SOF_AV_get         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_CPU_DATA_AV_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00031044 + (N) * 0x100) CPU_DATA_FIFO_DATA |
 | |     N[0..95]                                           |
 | +--------------------------------------------------------+
 | | bit  31:24 R  CPU_BYTE3                                |
 | | bit  23:16 R  CPU_BYTE2                                |
 | | bit  15:8  R  CPU_BYTE1                                |
 | | bit  7:0   R  CPU_BYTE0                                |
 | +--------------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_CPU_BYTE3_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_CPU_BYTE3_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mapotn_rgfpf_field_CPU_BYTE2_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_CPU_BYTE2_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mapotn_rgfpf_field_CPU_BYTE1_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_CPU_BYTE1_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mapotn_rgfpf_field_CPU_BYTE0_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_CPU_BYTE0_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00031048 + (N) * 0x100) PAUSE_TIME_REG |
 | |     N[0..95]                                       |
 | +----------------------------------------------------+
 | | bit  15:0 R  PAUSE_TIME                            |
 | +----------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_PAUSE_TIME_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_PAUSE_TIME_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register (0x0003104c + (N) * 0x100) GENERIC_FILTER_CONFIG_0 |
 | |     N[0..95]                                                |
 | +-------------------------------------------------------------+
 | | bit  6:0 R/W  GENERIC_OFFSET                                |
 | +-------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_GENERIC_OFFSET_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_GENERIC_OFFSET_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_GENERIC_OFFSET_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_GENERIC_OFFSET_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register (0x00031050 + (N) * 0x100) GENERIC_FILTER_CONFIG_1 |
 | |     N[0..95]                                                |
 | +-------------------------------------------------------------+
 | | bit  31:0 R/W  GENERIC_MASK                                 |
 | +-------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_GENERIC_MASK_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_GENERIC_MASK_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_GENERIC_MASK_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_GENERIC_MASK_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register (0x00031054 + (N) * 0x100) GENERIC_FILTER_CONFIG_2 |
 | |     N[0..95]                                                |
 | +-------------------------------------------------------------+
 | | bit  31:0 R/W  GENERIC_VALUE                                |
 | +-------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_GENERIC_VALUE_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mapotn_rgfpf_field_GENERIC_VALUE_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | void   mapotn_rgfpf_field_range_GENERIC_VALUE_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_GENERIC_VALUE_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00031060 + (N) * 0x8 + (M) * 0x100) |
 | | FILTER_CONFIGURATION_0                          |
 | |     N[0..9]                                     |
 | |     M[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  31:24 R/W  UPI_VALUE                       |
 | | bit  22:20 R/W  PTI_VALUE                       |
 | | bit  19:16 R/W  EXI_VALUE                       |
 | | bit  15:8  R/W  UPI_MASK                        |
 | | bit  7:5   R/W  PTI_MASK                        |
 | | bit  4:1   R/W  EXI_MASK                        |
 | | bit  0     R/W  SELECT_GENERIC                  |
 | +-------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_UPI_VALUE_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 mapotn_rgfpf_field_UPI_VALUE_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   mapotn_rgfpf_field_range_UPI_VALUE_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_UPI_VALUE_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_PTI_VALUE_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PTI_VALUE_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   mapotn_rgfpf_field_range_PTI_VALUE_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_PTI_VALUE_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_EXI_VALUE_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 mapotn_rgfpf_field_EXI_VALUE_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   mapotn_rgfpf_field_range_EXI_VALUE_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_EXI_VALUE_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_UPI_MASK_set        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 mapotn_rgfpf_field_UPI_MASK_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   mapotn_rgfpf_field_range_UPI_MASK_set  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_UPI_MASK_get  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_PTI_MASK_set        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 mapotn_rgfpf_field_PTI_MASK_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   mapotn_rgfpf_field_range_PTI_MASK_set  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_PTI_MASK_get  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_EXI_MASK_set        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 mapotn_rgfpf_field_EXI_MASK_get        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   mapotn_rgfpf_field_range_EXI_MASK_set  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_EXI_MASK_get  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_SELECT_GENERIC_set  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 mapotn_rgfpf_field_SELECT_GENERIC_get  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | 
 | +-------------------------------------------------+
 | | Register (0x00031064 + (N) * 0x8 + (M) * 0x100) |
 | | FILTER_CONFIGURATION_1                          |
 | |     N[0..9]                                     |
 | |     M[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  16:13 R/W  MATCH_INDICATOR                 |
 | | bit  12:9  R/W  NEXT_FILTER_OFFSET              |
 | | bit  8     R/W  CONTINUATION_OPERATOR           |
 | | bit  7:6   R/W  ACTION                          |
 | | bit  5:0   R/W  COMPARE_OPERATOR                |
 | +-------------------------------------------------+
 | 
 | void   mapotn_rgfpf_field_MATCH_INDICATOR_set          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 mapotn_rgfpf_field_MATCH_INDICATOR_get          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   mapotn_rgfpf_field_range_MATCH_INDICATOR_set    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_MATCH_INDICATOR_get    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_NEXT_FILTER_OFFSET_set       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 mapotn_rgfpf_field_NEXT_FILTER_OFFSET_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   mapotn_rgfpf_field_range_NEXT_FILTER_OFFSET_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_NEXT_FILTER_OFFSET_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_CONTINUATION_OPERATOR_set    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 mapotn_rgfpf_field_CONTINUATION_OPERATOR_get    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   mapotn_rgfpf_field_ACTION_set                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 mapotn_rgfpf_field_ACTION_get                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   mapotn_rgfpf_field_range_ACTION_set             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_ACTION_get             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | void   mapotn_rgfpf_field_COMPARE_OPERATOR_set         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | UINT32 mapotn_rgfpf_field_COMPARE_OPERATOR_get         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | void   mapotn_rgfpf_field_range_COMPARE_OPERATOR_set   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mapotn_rgfpf_field_range_COMPARE_OPERATOR_get   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00037000 + (N) * 0x80) RX_BYTE_CNT_PMON_LSB |
 | |     N[0..95]                                            |
 | +---------------------------------------------------------+
 | | bit  31:0 R  RX_BYTE_CNT_LSB                            |
 | +---------------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_RX_BYTE_CNT_LSB_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_RX_BYTE_CNT_LSB_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00037004 + (N) * 0x80) RX_BYTE_CNT_PMON_MSB |
 | |     N[0..95]                                            |
 | +---------------------------------------------------------+
 | | bit  7:0 R  RX_BYTE_CNT_MSB                             |
 | +---------------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_RX_BYTE_CNT_MSB_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_RX_BYTE_CNT_MSB_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00037008 + (N) * 0x80) RX_FRM_CNT |
 | |     N[0..95]                                  |
 | +-----------------------------------------------+
 | | bit  31:0 R  RX_FRM_CNT                       |
 | +-----------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_RX_FRM_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_RX_FRM_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x0003700c + (N) * 0x80) IDLE_FRM_CNT_PMON |
 | |     N[0..95]                                         |
 | +------------------------------------------------------+
 | | bit  31:0 R  IDLE_FRM_CNT                            |
 | +------------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_IDLE_FRM_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_IDLE_FRM_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00037010 + (N) * 0x80) CTRL_FRM_CNT_PMON |
 | |     N[0..95]                                         |
 | +------------------------------------------------------+
 | | bit  31:0 R  CTRL_FRM_CNT                            |
 | +------------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_CTRL_FRM_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_CTRL_FRM_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00037014 + (N) * 0x80) ERR_CHEC_CNT |
 | |     N[0..95]                                    |
 | +-------------------------------------------------+
 | | bit  31:0 R  ERR_CHEC_CNT                       |
 | +-------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_ERR_CHEC_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_ERR_CHEC_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------------+
 | | Register (0x00037018 + (N) * 0x80) SINGL_ERR_CORR_CHEC_CNT_PMON |
 | |     N[0..95]                                                    |
 | +-----------------------------------------------------------------+
 | | bit  31:0 R  SINGL_ERR_CORR_CHEC_CNT                            |
 | +-----------------------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_SINGL_ERR_CORR_CHEC_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_SINGL_ERR_CORR_CHEC_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register (0x0003701c + (N) * 0x80) UNPACK_DROP_FRM_CNT_PMON |
 | |     N[0..95]                                                |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  UNPACK_DROP_FRM_CNT                            |
 | +-------------------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_UNPACK_DROP_FRM_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_UNPACK_DROP_FRM_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00037020 + (N) * 0x80) ERR_THEC_CNT_PMON |
 | |     N[0..95]                                         |
 | +------------------------------------------------------+
 | | bit  31:0 R  ERR_THEC_CNT                            |
 | +------------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_ERR_THEC_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_ERR_THEC_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------+
 | | Register (0x00037024 + (N) * 0x80) SINGL_ERR_CORR_TE_CNT_PMON |
 | |     N[0..95]                                                  |
 | +---------------------------------------------------------------+
 | | bit  31:0 R  SINGL_ERR_CORR_TE_CNT                            |
 | +---------------------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_SINGL_ERR_CORR_TE_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_SINGL_ERR_CORR_TE_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register (0x00037028 + (N) * 0x80) FILTER_DROP_FRM_CNT_PMON |
 | |     N[0..95]                                                |
 | +-------------------------------------------------------------+
 | | bit  31:0 R  FILTER_DROP_FRM_CNT                            |
 | +-------------------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_FILTER_DROP_FRM_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_FILTER_DROP_FRM_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x0003702c + (N) * 0x80) DEXM_EHEC_CNT_PMON |
 | |     N[0..95]                                          |
 | +-------------------------------------------------------+
 | | bit  31:0 R  DEXM_EHEC_CNT                            |
 | +-------------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_DEXM_EHEC_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_DEXM_EHEC_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00037030 + (N) * 0x80) DUPM_CNT_PMON |
 | |     N[0..95]                                     |
 | +--------------------------------------------------+
 | | bit  31:0 R  DUPM_FRM_CNT                        |
 | +--------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_DUPM_FRM_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_DUPM_FRM_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00037034 + (N) * 0x80) DGEM_CNT_PMON |
 | |     N[0..95]                                     |
 | +--------------------------------------------------+
 | | bit  31:0 R  DGEM_FRM_CNT                        |
 | +--------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_DGEM_FRM_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_DGEM_FRM_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00037038 + (N) * 0x80) CSF_FRM_CNT_PMON |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  7:0 R  CSF_FRM_CNT                             |
 | +-----------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_CSF_FRM_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_CSF_FRM_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x0003703c + (N) * 0x80) ERR_FCS_CNT_PMON |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  31:0 R  ERR_FCS_CNT                            |
 | +-----------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_ERR_FCS_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_ERR_FCS_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00037040 + (N) * 0x80) PAUSE_FRM_CNT_PMON |
 | |     N[0..95]                                          |
 | +-------------------------------------------------------+
 | | bit  31:0 R  PAUSE_FRM_CNT                            |
 | +-------------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_PAUSE_FRM_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_PAUSE_FRM_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00037048 + (N) * 0x80) CPU_OVF_CNT_PMON |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  31:0 R  CPU_OVF_CNT                            |
 | +-----------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_CPU_OVF_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_CPU_OVF_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x0003704c + (N) * 0x80) EGRESS_CNT_PMON |
 | |     N[0..95]                                       |
 | +----------------------------------------------------+
 | | bit  31:0 R  EGRESS_FRM_CNT                        |
 | +----------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_EGRESS_FRM_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_EGRESS_FRM_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00037050 + (N) * 0x80) UNPACK_CNT_PMON |
 | |     N[0..95]                                       |
 | +----------------------------------------------------+
 | | bit  31:0 R  UNPACK_FRM_CNT                        |
 | +----------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_UNPACK_FRM_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_UNPACK_FRM_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00037054 + (N) * 0x80) MINFL_CHK_CNT_PMON |
 | |     N[0..95]                                          |
 | +-------------------------------------------------------+
 | | bit  31:0 R  MINFL_CHK_CNT                            |
 | +-------------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_MINFL_CHK_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_MINFL_CHK_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00037058 + (N) * 0x80) MAXFL_CHK_CNT_PMON |
 | |     N[0..95]                                          |
 | +-------------------------------------------------------+
 | | bit  31:0 R  MAXFL_CHK_CNT                            |
 | +-------------------------------------------------------+
 | 
 | UINT32 mapotn_rgfpf_field_MAXFL_CHK_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | UINT32 mapotn_rgfpf_field_range_MAXFL_CHK_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------------+
 | | Register (0x00037060 + (N) * 0x4 + (M) * 0x80) FRM_LEN_STAT_CNT_PMON |
 | |     N[0..6]                                                          |
 | |     M[0..95]                                                         |
 | +----------------------------------------------------------------------+
 | | bit  31:0 R  FL_CHK_STAT_CNT                                         |
 | +----------------------------------------------------------------------+
 | 
 | void   mapotn_rgfpf_lfield_FL_CHK_STAT_CNT_get       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 value[7] )
 | UINT32 mapotn_rgfpf_lfield_range_FL_CHK_STAT_CNT_get ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 start_bit, UINT32 stop_bit )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MAPOTN_RGFPF_IO_H
#define _MAPOTN_RGFPF_IO_H

#include "mapotn_loc.h"
#include "mapotn_rgfpf_regs.h"
#include "mapotn_rgfpf_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mapotn_rgfpf_buffer_init               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | void                 mapotn_rgfpf_buffer_flush              ( mapotn_rgfpf_buffer_t *b_ptr )
 | UINT32               mapotn_rgfpf_reg_read                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 mapotn_rgfpf_reg_write                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 mapotn_rgfpf_field_set                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_action_on_write_field_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_burst_read                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_burst_write               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_poll                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mapotn_rgfpf_reg_GENERAL_CONFIGURATION_write                                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_GENERAL_CONFIGURATION_field_set                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_GENERAL_CONFIGURATION_read                                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_MAC_DESTINATION_ADDRESS_LSB_write                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_MAC_DESTINATION_ADDRESS_LSB_field_set                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_MAC_DESTINATION_ADDRESS_LSB_read                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_MAC_DESTINATION_ADDRESS_MSB_write                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_MAC_DESTINATION_ADDRESS_MSB_field_set                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_MAC_DESTINATION_ADDRESS_MSB_read                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_ORDERED_SET_UPI_write                                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_ORDERED_SET_UPI_field_set                                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_ORDERED_SET_UPI_read                                                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_CHID_MISMATCH_CAPTURE_write                                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_CHID_MISMATCH_CAPTURE_field_set                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_CHID_MISMATCH_CAPTURE_read                                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_CPU_FIFO_RPT_write                                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_CPU_FIFO_RPT_field_set                                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_CPU_FIFO_RPT_read                                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_AUTO_FLOW_CTRL_REGISTER_array_burst_write                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_AUTO_FLOW_CTRL_REGISTER_array_field_set                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_AUTO_FLOW_CTRL_REGISTER_array_burst_read                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_AUTO_FLOW_CTRL_REGISTER_array_read                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_CPB_ZONE_THRESHOLD_array_burst_write                                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_CPB_ZONE_THRESHOLD_array_field_set                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_CPB_ZONE_THRESHOLD_array_burst_read                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_CPB_ZONE_THRESHOLD_array_read                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_CHANNEL_RESET_array_write                                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_rgfpf_reg_CHANNEL_RESET_array_field_set                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_CHANNEL_RESET_array_read                                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_4_array_write                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_4_array_field_set                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_4_array_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_rgfpf_reg_PMON_LATCH_CONTROL_REGISTER_write                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_PMON_LATCH_CONTROL_REGISTER_field_set                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_PMON_LATCH_CONTROL_REGISTER_read                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_TRIG_PAUSE_CAP_REGISTER_write                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_TRIG_PAUSE_CAP_REGISTER_field_set                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_TRIG_PAUSE_CAP_REGISTER_read                                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_EXTENSION_HDR_MIS_FAIL_STATUS_write                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_EXTENSION_HDR_MIS_FAIL_STATUS_field_set                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_EXTENSION_HDR_MIS_FAIL_STATUS_read                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_PTI_UPI_MIS_STATUS_write                                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_PTI_UPI_MIS_STATUS_field_set                                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_PTI_UPI_MIS_STATUS_read                                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_DCSF_DEASSERT_REGISTER_array_burst_write                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_DCSF_DEASSERT_REGISTER_array_field_set                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_DCSF_DEASSERT_REGISTER_array_burst_read                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_DCSF_DEASSERT_REGISTER_array_read                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_CSF_STATUS_AV_INTR_ENABLE_write                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_CSF_STATUS_AV_INTR_ENABLE_field_set                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_CSF_STATUS_AV_INTR_ENABLE_read                                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_CSF_STATUS_FIFO_OF_INTR_ENABLE_write                                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_CSF_STATUS_FIFO_OF_INTR_ENABLE_field_set                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_CSF_STATUS_FIFO_OF_INTR_ENABLE_read                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_PMON_TRANSFER_INTR_ENABLE_write                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_PMON_TRANSFER_INTR_ENABLE_field_set                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_PMON_TRANSFER_INTR_ENABLE_read                                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_ASYNC_FIFO_OVF_INTR_ENABLE_write                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_ASYNC_FIFO_OVF_INTR_ENABLE_field_set                                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_ASYNC_FIFO_OVF_INTR_ENABLE_read                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_RCOH_STAT_TRANSFR_DONE_EN_write                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_RCOH_STAT_TRANSFR_DONE_EN_field_set                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_RCOH_STAT_TRANSFR_DONE_EN_read                                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_FRM_LEN_STAT_INTR_ENABLE_array_burst_write                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_FRM_LEN_STAT_INTR_ENABLE_array_field_set                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_FRM_LEN_STAT_INTR_ENABLE_array_burst_read                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_FRM_LEN_STAT_INTR_ENABLE_array_read                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_IN_SFAIL_INTR_ENABLE_array_burst_write                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_IN_SFAIL_INTR_ENABLE_array_field_set                                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_IN_SFAIL_INTR_ENABLE_array_burst_read                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_IN_SFAIL_INTR_ENABLE_array_read                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_LOSS_FRME_DELINEATION_INTR_ENABLE_array_burst_write                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_LOSS_FRME_DELINEATION_INTR_ENABLE_array_field_set                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_LOSS_FRME_DELINEATION_INTR_ENABLE_array_burst_read                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_LOSS_FRME_DELINEATION_INTR_ENABLE_array_read                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_IDLE_FRAME_INTR_ENABLE_array_burst_write                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_IDLE_FRAME_INTR_ENABLE_array_field_set                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_IDLE_FRAME_INTR_ENABLE_array_burst_read                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_IDLE_FRAME_INTR_ENABLE_array_read                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE_array_burst_write            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE_array_field_set              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE_array_burst_read             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_NON_IDLE_CONTROL_FRAME_INTR_ENABLE_array_read                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_UNPACK_DROP_INTR_ENABLE_array_burst_write                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_UNPACK_DROP_INTR_ENABLE_array_field_set                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_UNPACK_DROP_INTR_ENABLE_array_burst_read                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_UNPACK_DROP_INTR_ENABLE_array_read                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE_array_burst_write          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE_array_field_set            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE_array_burst_read           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_UNCORRECTABLE_CHEC_ERROR_INTR_ENABLE_array_read                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CCHEC_ERROR_INTR_ENABLE_array_burst_write                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_CCHEC_ERROR_INTR_ENABLE_array_field_set                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CCHEC_ERROR_INTR_ENABLE_array_burst_read                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_CCHEC_ERROR_INTR_ENABLE_array_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_UTHEC_ERROR_INTR_ENABLE_array_burst_write                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_UTHEC_ERROR_INTR_ENABLE_array_field_set                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_UTHEC_ERROR_INTR_ENABLE_array_burst_read                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_UTHEC_ERROR_INTR_ENABLE_array_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CTHEC_ERROR_INTR_ENABLE_array_burst_write                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_CTHEC_ERROR_INTR_ENABLE_array_field_set                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CTHEC_ERROR_INTR_ENABLE_array_burst_read                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_CTHEC_ERROR_INTR_ENABLE_array_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE_array_burst_write          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE_array_field_set            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE_array_burst_read           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_ENABLE_array_read                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CEHEC_ERROR_INTR_ENABLE_array_burst_write                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_CEHEC_ERROR_INTR_ENABLE_array_field_set                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CEHEC_ERROR_INTR_ENABLE_array_burst_read                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_CEHEC_ERROR_INTR_ENABLE_array_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_EXI_MISMATCH_ERROR_INTR_ENABLE_array_burst_write                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_EXI_MISMATCH_ERROR_INTR_ENABLE_array_field_set                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_EXI_MISMATCH_ERROR_INTR_ENABLE_array_burst_read                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_EXI_MISMATCH_ERROR_INTR_ENABLE_array_read                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_PTI_UPI_MSM_ERR_INTR_ENABLE_array_burst_write                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_PTI_UPI_MSM_ERR_INTR_ENABLE_array_field_set                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_PTI_UPI_MSM_ERR_INTR_ENABLE_array_burst_read                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_PTI_UPI_MSM_ERR_INTR_ENABLE_array_read                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE_array_burst_write            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE_array_field_set              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE_array_burst_read             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_GENERIC_MISMATCH_ERROR_INTR_ENABLE_array_read                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CSF_INTR_ENABLE_array_burst_write                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_CSF_INTR_ENABLE_array_field_set                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CSF_INTR_ENABLE_array_burst_read                                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_CSF_INTR_ENABLE_array_read                                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_DROPPED_FRAME_INTR_ENABLE_array_burst_write                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_DROPPED_FRAME_INTR_ENABLE_array_field_set                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_DROPPED_FRAME_INTR_ENABLE_array_burst_read                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_DROPPED_FRAME_INTR_ENABLE_array_read                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CLIENT_DATA_FRAME_INTR_ENABLE_array_burst_write                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_CLIENT_DATA_FRAME_INTR_ENABLE_array_field_set                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CLIENT_DATA_FRAME_INTR_ENABLE_array_burst_read                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_CLIENT_DATA_FRAME_INTR_ENABLE_array_read                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_PAUSE_INTR_ENABLE_array_burst_write                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_PAUSE_INTR_ENABLE_array_field_set                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_PAUSE_INTR_ENABLE_array_burst_read                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_PAUSE_INTR_ENABLE_array_read                                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_MAX_FL_VIOALT_INTR_ENABLE_array_burst_write                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_MAX_FL_VIOALT_INTR_ENABLE_array_field_set                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_MAX_FL_VIOALT_INTR_ENABLE_array_burst_read                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_MAX_FL_VIOALT_INTR_ENABLE_array_read                                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_MIN_FL_VIOALT_INTR_ENABLE_array_burst_write                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_MIN_FL_VIOALT_INTR_ENABLE_array_field_set                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_MIN_FL_VIOALT_INTR_ENABLE_array_burst_read                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_MIN_FL_VIOALT_INTR_ENABLE_array_read                                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_PAYLOAD_FCS_INTR_ENABLE_array_burst_write                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_PAYLOAD_FCS_INTR_ENABLE_array_field_set                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_PAYLOAD_FCS_INTR_ENABLE_array_burst_read                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_PAYLOAD_FCS_INTR_ENABLE_array_read                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_DOWNSTRM_FRAME_INTR_ENABLE_array_burst_write                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_DOWNSTRM_FRAME_INTR_ENABLE_array_field_set                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_DOWNSTRM_FRAME_INTR_ENABLE_array_burst_read                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_DOWNSTRM_FRAME_INTR_ENABLE_array_read                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_CPU_FIFO_DATA_AV_INTR_ENABLE_array_burst_write                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_CPU_FIFO_DATA_AV_INTR_ENABLE_array_field_set                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_CPU_FIFO_DATA_AV_INTR_ENABLE_array_burst_read                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_CPU_FIFO_DATA_AV_INTR_ENABLE_array_read                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE_array_burst_write                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE_array_field_set                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE_array_burst_read                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_CPU_DATA_FIFO_OVERFLOW_INTR_ENABLE_array_read                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_SSF_SF_INT_EN_array_burst_write                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_SSF_SF_INT_EN_array_field_set                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_SSF_SF_INT_EN_array_burst_read                                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_SSF_SF_INT_EN_array_read                                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_HAO_STAT_INT_EN_array_burst_write                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_HAO_STAT_INT_EN_array_field_set                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_HAO_STAT_INT_EN_array_burst_read                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_HAO_STAT_INT_EN_array_read                                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_DPLM_INT_EN_array_burst_write                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_DPLM_INT_EN_array_field_set                                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_DPLM_INT_EN_array_burst_read                                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_DPLM_INT_EN_array_read                                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_OPU_CSF_INT_EN_array_burst_write                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_OPU_CSF_INT_EN_array_field_set                                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_OPU_CSF_INT_EN_array_burst_read                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_OPU_CSF_INT_EN_array_read                                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_CSF_ST_AV_INTR_write                                                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_CSF_ST_AV_INTR_action_on_write_field_set                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_CSF_ST_AV_INTR_read                                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_CSF_ST_AV_INTR_poll                                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_CSF_ST_FIFO_OF_INTR_write                                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_CSF_ST_FIFO_OF_INTR_action_on_write_field_set                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_CSF_ST_FIFO_OF_INTR_read                                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_CSF_ST_FIFO_OF_INTR_poll                                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_PMON_TRANSFER_INTR_write                                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_PMON_TRANSFER_INTR_action_on_write_field_set                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_PMON_TRANSFER_INTR_read                                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_PMON_TRANSFER_INTR_poll                                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_ASYNC_FIFO_OVF_INTR_write                                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_ASYNC_FIFO_OVF_INTR_action_on_write_field_set                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_ASYNC_FIFO_OVF_INTR_read                                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_ASYNC_FIFO_OVF_INTR_poll                                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_RCOH_STAT_TRANSFER_DONE_INT_write                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_RCOH_STAT_TRANSFER_DONE_INT_action_on_write_field_set               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_RCOH_STAT_TRANSFER_DONE_INT_read                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_RCOH_STAT_TRANSFER_DONE_INT_poll                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_FRM_LEN_STAT_INTR_array_burst_write                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_FRM_LEN_STAT_INTR_array_action_on_write_field_set                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_FRM_LEN_STAT_INTR_array_burst_read                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_FRM_LEN_STAT_INTR_array_read                                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_FRM_LEN_STAT_INTR_array_poll                                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_IN_SFAIL_INTR_array_burst_write                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_IN_SFAIL_INTR_array_action_on_write_field_set                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_IN_SFAIL_INTR_array_burst_read                                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_IN_SFAIL_INTR_array_read                                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_IN_SFAIL_INTR_array_poll                                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_LOSS_FRME_DELINEATION_INTR_array_burst_write                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_LOSS_FRME_DELINEATION_INTR_array_action_on_write_field_set          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_LOSS_FRME_DELINEATION_INTR_array_burst_read                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_LOSS_FRME_DELINEATION_INTR_array_read                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_LOSS_FRME_DELINEATION_INTR_array_poll                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_IDLE_FRAME_INTR_array_burst_write                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_IDLE_FRAME_INTR_array_action_on_write_field_set                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_IDLE_FRAME_INTR_array_burst_read                                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_IDLE_FRAME_INTR_array_read                                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_IDLE_FRAME_INTR_array_poll                                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_NON_IDLE_CONTROL_FRAME_INTR_array_burst_write                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_NON_IDLE_CONTROL_FRAME_INTR_array_action_on_write_field_set     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_NON_IDLE_CONTROL_FRAME_INTR_array_burst_read                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_NON_IDLE_CONTROL_FRAME_INTR_array_read                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_NON_IDLE_CONTROL_FRAME_INTR_array_poll                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_UNPACK_DROP_INTR_array_burst_write                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_UNPACK_DROP_INTR_array_action_on_write_field_set                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_UNPACK_DROP_INTR_array_burst_read                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_UNPACK_DROP_INTR_array_read                                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_UNPACK_DROP_INTR_array_poll                                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_UCHEC_ERROR_INTR_array_burst_write                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_UCHEC_ERROR_INTR_array_action_on_write_field_set                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_UCHEC_ERROR_INTR_array_burst_read                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_UCHEC_ERROR_INTR_array_read                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_UCHEC_ERROR_INTR_array_poll                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CCHEC_ERROR_ERROR_INTR_array_burst_write                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_CCHEC_ERROR_ERROR_INTR_array_action_on_write_field_set          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CCHEC_ERROR_ERROR_INTR_array_burst_read                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_CCHEC_ERROR_ERROR_INTR_array_read                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_CCHEC_ERROR_ERROR_INTR_array_poll                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_UNCORRECTABLE_THEC_ERROR_INTR_array_burst_write                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_UNCORRECTABLE_THEC_ERROR_INTR_array_action_on_write_field_set   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_UNCORRECTABLE_THEC_ERROR_INTR_array_burst_read                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_UNCORRECTABLE_THEC_ERROR_INTR_array_read                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_UNCORRECTABLE_THEC_ERROR_INTR_array_poll                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CTHEC_ERROR_INTR_array_burst_write                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_CTHEC_ERROR_INTR_array_action_on_write_field_set                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CTHEC_ERROR_INTR_array_burst_read                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_CTHEC_ERROR_INTR_array_read                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_CTHEC_ERROR_INTR_array_poll                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_array_burst_write                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_array_action_on_write_field_set   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_array_burst_read                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_array_read                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_UNCORRECTABLE_EHEC_ERROR_INTR_array_poll                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CEHEC_ERROR_ERROR_INTR_array_burst_write                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_CEHEC_ERROR_ERROR_INTR_array_action_on_write_field_set          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CEHEC_ERROR_ERROR_INTR_array_burst_read                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_CEHEC_ERROR_ERROR_INTR_array_read                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_CEHEC_ERROR_ERROR_INTR_array_poll                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_EXI_MISMATCH_ERROR_INTR_array_burst_write                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_EXI_MISMATCH_ERROR_INTR_array_action_on_write_field_set         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_EXI_MISMATCH_ERROR_INTR_array_burst_read                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_EXI_MISMATCH_ERROR_INTR_array_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_EXI_MISMATCH_ERROR_INTR_array_poll                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_PTI_UPI_MSM_ERR_INTR_array_burst_write                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_PTI_UPI_MSM_ERR_INTR_array_action_on_write_field_set            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_PTI_UPI_MSM_ERR_INTR_array_burst_read                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_PTI_UPI_MSM_ERR_INTR_array_read                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_PTI_UPI_MSM_ERR_INTR_array_poll                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_GENERIC_MISMATCH_ERROR_INTR_array_burst_write                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_GENERIC_MISMATCH_ERROR_INTR_array_action_on_write_field_set     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_GENERIC_MISMATCH_ERROR_INTR_array_burst_read                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_GENERIC_MISMATCH_ERROR_INTR_array_read                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_GENERIC_MISMATCH_ERROR_INTR_array_poll                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CSF_INTR_array_burst_write                                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_CSF_INTR_array_action_on_write_field_set                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CSF_INTR_array_burst_read                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_CSF_INTR_array_read                                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_CSF_INTR_array_poll                                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_DROPPED_FRAME_INTR_array_burst_write                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_DROPPED_FRAME_INTR_array_action_on_write_field_set              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_DROPPED_FRAME_INTR_array_burst_read                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_DROPPED_FRAME_INTR_array_read                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_DROPPED_FRAME_INTR_array_poll                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CLIENT_DATA_FRAME_INTR_array_burst_write                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_GFP_CLIENT_DATA_FRAME_INTR_array_action_on_write_field_set          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CLIENT_DATA_FRAME_INTR_array_burst_read                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_CLIENT_DATA_FRAME_INTR_array_read                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_CLIENT_DATA_FRAME_INTR_array_poll                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_PAUSE_INTR_array_burst_write                                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_PAUSE_INTR_array_action_on_write_field_set                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_PAUSE_INTR_array_burst_read                                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_PAUSE_INTR_array_read                                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_PAUSE_INTR_array_poll                                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR_array_burst_write               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR_array_action_on_write_field_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR_array_burst_read                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR_array_read                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_MAXIMUM_FRAME_LENGTH_VIOLATION_INTR_array_poll                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_MINIMUM_FRAME_LENGTH_VIOLATION_INTR_array_burst_write               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_MINIMUM_FRAME_LENGTH_VIOLATION_INTR_array_action_on_write_field_set ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_MINIMUM_FRAME_LENGTH_VIOLATION_INTR_array_burst_read                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_MINIMUM_FRAME_LENGTH_VIOLATION_INTR_array_read                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_MINIMUM_FRAME_LENGTH_VIOLATION_INTR_array_poll                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_PAYLOAD_FCS_INTR_array_burst_write                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_PAYLOAD_FCS_INTR_array_action_on_write_field_set                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_PAYLOAD_FCS_INTR_array_burst_read                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_PAYLOAD_FCS_INTR_array_read                                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_PAYLOAD_FCS_INTR_array_poll                                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_DOWNSTREAM_FRAME_INTR_array_burst_write                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_DOWNSTREAM_FRAME_INTR_array_action_on_write_field_set               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_DOWNSTREAM_FRAME_INTR_array_burst_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_DOWNSTREAM_FRAME_INTR_array_read                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_DOWNSTREAM_FRAME_INTR_array_poll                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_CPU_FIFO_DATA_AV_INTR_array_burst_write                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_CPU_FIFO_DATA_AV_INTR_array_action_on_write_field_set               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_CPU_FIFO_DATA_AV_INTR_array_burst_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_CPU_FIFO_DATA_AV_INTR_array_read                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_CPU_FIFO_DATA_AV_INTR_array_poll                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_CPU_DATA_FIFO_OVERFLOW_INTR_array_burst_write                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_CPU_DATA_FIFO_OVERFLOW_INTR_array_action_on_write_field_set         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_CPU_DATA_FIFO_OVERFLOW_INTR_array_burst_read                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_CPU_DATA_FIFO_OVERFLOW_INTR_array_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_CPU_DATA_FIFO_OVERFLOW_INTR_array_poll                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_SSF_SF_RCVDINT_array_burst_write                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_SSF_SF_RCVDINT_array_action_on_write_field_set                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_SSF_SF_RCVDINT_array_burst_read                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_SSF_SF_RCVDINT_array_read                                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_SSF_SF_RCVDINT_array_poll                                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_HAO_STAT_INT_array_burst_write                                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_HAO_STAT_INT_array_action_on_write_field_set                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_HAO_STAT_INT_array_burst_read                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_HAO_STAT_INT_array_read                                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_HAO_STAT_INT_array_poll                                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_DPLM_INT_array_burst_write                                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_DPLM_INT_array_action_on_write_field_set                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_DPLM_INT_array_burst_read                                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_DPLM_INT_array_read                                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_DPLM_INT_array_poll                                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_OPU_CSF_INT_array_burst_write                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | void                 mapotn_rgfpf_reg_OPU_CSF_INT_array_action_on_write_field_set                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mapotn_rgfpf_reg_OPU_CSF_INT_array_burst_read                                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_OPU_CSF_INT_array_read                                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_OPU_CSF_INT_array_poll                                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE_array_burst_read          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE_array_read                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_LOSS_OF_FRAME_DELINEATION_INTERRUPT_VALUE_array_poll                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_FRAMER_IDLE_INTERRUPT_VALUE_array_burst_read                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_FRAMER_IDLE_INTERRUPT_VALUE_array_read                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_FRAMER_IDLE_INTERRUPT_VALUE_array_poll                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CSF_INTERRUPT_VALUE_array_burst_read                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_CSF_INTERRUPT_VALUE_array_read                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_CSF_INTERRUPT_VALUE_array_poll                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_EXI_MISMATCH_ERROR_INTR_VALUE_array_burst_read                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_EXI_MISMATCH_ERROR_INTR_VALUE_array_read                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_EXI_MISMATCH_ERROR_INTR_VALUE_array_poll                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_GFP_PTI_UPI_MSM_ERR_INTR_VALUE_array_burst_read                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_GFP_PTI_UPI_MSM_ERR_INTR_VALUE_array_read                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_GFP_PTI_UPI_MSM_ERR_INTR_VALUE_array_poll                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_IN_SFAIL_INT_VAL_array_burst_read                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_IN_SFAIL_INT_VAL_array_read                                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_IN_SFAIL_INT_VAL_array_poll                                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_SSF_SF_RCVD_INT_VAL_array_burst_read                                ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_SSF_SF_RCVD_INT_VAL_array_read                                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_SSF_SF_RCVD_INT_VAL_array_poll                                      ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_DPLM_INT_VAL_array_burst_read                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_DPLM_INT_VAL_array_read                                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_DPLM_INT_VAL_array_poll                                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_OPU_CSF_INT_VAL_array_burst_read                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_OPU_CSF_INT_VAL_array_read                                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N )
 | PMC_POLL_RETURN_TYPE mapotn_rgfpf_reg_OPU_CSF_INT_VAL_array_poll                                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mapotn_rgfpf_reg_FRM_LEN_STAT_CNT_PMON_array_burst_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 ofs, UINT32 len, UINT32 *value )
 | UINT32               mapotn_rgfpf_reg_FRM_LEN_STAT_CNT_PMON_array_read                                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  M, UINT32 N )
 | 
 | void                 mapotn_rgfpf_reg_FILTER_CONFIGURATION_0_array_write                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | void                 mapotn_rgfpf_reg_FILTER_CONFIGURATION_0_array_field_set                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_FILTER_CONFIGURATION_0_array_read                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | 
 | void                 mapotn_rgfpf_reg_FILTER_CONFIGURATION_1_array_write                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | void                 mapotn_rgfpf_reg_FILTER_CONFIGURATION_1_array_field_set                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_FILTER_CONFIGURATION_1_array_read                                   ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | 
 | void                 mapotn_rgfpf_reg_FRM_LEN_STAT_COUNTER_CONFIG_array_write                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 value )
 | void                 mapotn_rgfpf_reg_FRM_LEN_STAT_COUNTER_CONFIG_array_field_set                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_FRM_LEN_STAT_COUNTER_CONFIG_array_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32  M )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_0_array_write                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_0_array_field_set                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_0_array_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_1_array_write                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_1_array_field_set                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_1_array_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_2_array_write                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_2_array_field_set                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_2_array_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_3_array_write                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_3_array_field_set                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_3_array_read                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_rgfpf_reg_FRAME_LENGTH_CHECK_CONFIGURATION_array_write                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_rgfpf_reg_FRAME_LENGTH_CHECK_CONFIGURATION_array_field_set                    ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_FRAME_LENGTH_CHECK_CONFIGURATION_array_read                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_rgfpf_reg_CPU_DATA_FIFO_STATUS_RESET_array_write                              ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_rgfpf_reg_CPU_DATA_FIFO_STATUS_RESET_array_field_set                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_CPU_DATA_FIFO_STATUS_RESET_array_read                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_CPU_DATA_FIFO_DATA_array_read                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_PAUSE_TIME_REG_array_read                                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_0_array_write                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_0_array_field_set                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_0_array_read                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_1_array_write                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_1_array_field_set                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_1_array_read                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_2_array_write                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_2_array_field_set                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_2_array_read                                  ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_RX_BYTE_CNT_PMON_LSB_array_read                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_RX_BYTE_CNT_PMON_MSB_array_read                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_RX_FRM_CNT_array_read                                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_IDLE_FRM_CNT_PMON_array_read                                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_CTRL_FRM_CNT_PMON_array_read                                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_ERR_CHEC_CNT_array_read                                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_SINGL_ERR_CORR_CHEC_CNT_PMON_array_read                             ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_UNPACK_DROP_FRM_CNT_PMON_array_read                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_ERR_THEC_CNT_PMON_array_read                                        ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_SINGL_ERR_CORR_TE_CNT_PMON_array_read                               ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_FILTER_DROP_FRM_CNT_PMON_array_read                                 ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_DEXM_EHEC_CNT_PMON_array_read                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_DUPM_CNT_PMON_array_read                                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_DGEM_CNT_PMON_array_read                                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_CSF_FRM_CNT_PMON_array_read                                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_ERR_FCS_CNT_PMON_array_read                                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_PAUSE_FRM_CNT_PMON_array_read                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_CPU_OVF_CNT_PMON_array_read                                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_EGRESS_CNT_PMON_array_read                                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_UNPACK_CNT_PMON_array_read                                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_MINFL_CHK_CNT_PMON_array_read                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_MAXFL_CHK_CNT_PMON_array_read                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mapotn_rgfpf_reg_CSF_STATUS_read                                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_MUTEX_CPU_DATA_FIFO_write                                           ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_MUTEX_CPU_DATA_FIFO_field_set                                       ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_MUTEX_CPU_DATA_FIFO_read                                            ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 | void                 mapotn_rgfpf_reg_MUTEX_CSF_STATUS_FIFO_write                                         ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 value )
 | void                 mapotn_rgfpf_reg_MUTEX_CSF_STATUS_FIFO_field_set                                     ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mapotn_rgfpf_reg_MUTEX_CSF_STATUS_FIFO_read                                          ( mapotn_rgfpf_buffer_t *b_ptr, mapotn_handle_t *h_ptr )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MAPOTN_RGFPF_IO_H */
