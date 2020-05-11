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
 *     and register accessor functions for the mpmo block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing mpmo io functions is:
 | {
 |     mpmo_buffer_t b_ptr[1];
 |     mpmo_buffer_init( b_ptr, h_ptr );
 |     value = mpmo_field_<FIELD>_get( b_ptr, h_ptr );
 |     mpmo_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     mpmo_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = mpmo_field_<FIELD>_get( NULL, h_ptr );
 | mpmo_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------------+
 | | Register 0x00000000 GEN_CFG          |
 | +--------------------------------------+
 | | bit  30    R/W  RAMP_EXTEND_EN       |
 | | bit  28    R/W  ZONE_SEL             |
 | | bit  24    R/W  SEQ_CHK_EN           |
 | | bit  21:20 R/W  PACER_MODE           |
 | | bit  19    R/W  BWR_IND_MODE         |
 | | bit  18    R/W  RAMP_STOP_MODE       |
 | | bit  17    R/W  AGE_SQUELCH_EN       |
 | | bit  16    R/W  CN_SQUELCH_EN        |
 | | bit  14    R/W  AGE_FILTER_EN        |
 | | bit  13    R/W  CN_BIT_MODE          |
 | | bit  12    R/W  SSF_GEN_EN           |
 | | bit  10    R/W  SSF_THRES_EN         |
 | | bit  9:8   R/W  BUS_WIDTH            |
 | | bit  5     R/W  DPI_DYNAMIC_LS_EN    |
 | | bit  4     R/W  DCI_DYNAMIC_LS_EN    |
 | | bit  0     R/W  MPMO_EN              |
 | | bit  29    R/W  CN_TARGET_DECIMAL_EN |
 | +--------------------------------------+
 | 
 | void   mpmo_field_RAMP_EXTEND_EN_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_RAMP_EXTEND_EN_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_ZONE_SEL_set             ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_ZONE_SEL_get             ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_SEQ_CHK_EN_set           ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_SEQ_CHK_EN_get           ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_PACER_MODE_set           ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_PACER_MODE_get           ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_range_PACER_MODE_set     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_PACER_MODE_get     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_field_BWR_IND_MODE_set         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_BWR_IND_MODE_get         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_RAMP_STOP_MODE_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_RAMP_STOP_MODE_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_AGE_SQUELCH_EN_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_AGE_SQUELCH_EN_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_CN_SQUELCH_EN_set        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_CN_SQUELCH_EN_get        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_AGE_FILTER_EN_set        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_AGE_FILTER_EN_get        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_CN_BIT_MODE_set          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_CN_BIT_MODE_get          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_SSF_GEN_EN_set           ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_SSF_GEN_EN_get           ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_SSF_THRES_EN_set         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_SSF_THRES_EN_get         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_BUS_WIDTH_set            ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_BUS_WIDTH_get            ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_range_BUS_WIDTH_set      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_BUS_WIDTH_get      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_field_DPI_DYNAMIC_LS_EN_set    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_DPI_DYNAMIC_LS_EN_get    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_DCI_DYNAMIC_LS_EN_set    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_DCI_DYNAMIC_LS_EN_get    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_MPMO_EN_set              ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_MPMO_EN_get              ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_CN_TARGET_DECIMAL_EN_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_CN_TARGET_DECIMAL_EN_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | +-----------------------------+
 | | Register 0x00000004 CSI_CFG |
 | +-----------------------------+
 | | bit  31  R/W  CSI_MASK_EN   |
 | | bit  4:0 R/W  CSI_NORMAL    |
 | +-----------------------------+
 | 
 | void   mpmo_field_CSI_MASK_EN_set      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_CSI_MASK_EN_get      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_CSI_NORMAL_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_CSI_NORMAL_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_range_CSI_NORMAL_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_CSI_NORMAL_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x00000008 DATA_AVAIL_CTRL |
 | +-------------------------------------+
 | | bit  28  R/W  DATA_AVAIL_CONT_HO_EN |
 | | bit  24  R/W  DATA_AVAIL_SQUELCH_EN |
 | | bit  20  R/W  HOLDOFF_ENB           |
 | | bit  16  R/W  FORCE_DATA_AVAIL_EN   |
 | | bit  9:0 R/W  BURST_SIZE            |
 | +-------------------------------------+
 | 
 | void   mpmo_field_DATA_AVAIL_CONT_HO_EN_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_DATA_AVAIL_CONT_HO_EN_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_DATA_AVAIL_SQUELCH_EN_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_DATA_AVAIL_SQUELCH_EN_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_HOLDOFF_ENB_set           ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_HOLDOFF_ENB_get           ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_FORCE_DATA_AVAIL_EN_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_FORCE_DATA_AVAIL_EN_get   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_BURST_SIZE_set            ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_BURST_SIZE_get            ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_range_BURST_SIZE_set      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_BURST_SIZE_get      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x0000000c AGE_FILT_CTRL |
 | +-----------------------------------+
 | | bit  23:0 R/W  AGE_FILT_PERIOD    |
 | +-----------------------------------+
 | 
 | void   mpmo_field_AGE_FILT_PERIOD_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32 mpmo_field_AGE_FILT_PERIOD_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void   mpmo_field_range_AGE_FILT_PERIOD_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_AGE_FILT_PERIOD_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------+
 | | Register 0x00000028 DEBUG_CTRL       |
 | +--------------------------------------+
 | | bit  28    R/W  DEBUG_DATA_REQ_ABORT |
 | | bit  24    R/W  DEBUG_DATA_REQ       |
 | | bit  16:22 R/W  DEBUG_DATA_REQ_ID    |
 | +--------------------------------------+
 | 
 | void                 mpmo_field_DEBUG_DATA_REQ_ID_set        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32               mpmo_field_DEBUG_DATA_REQ_ID_get        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void                 mpmo_field_range_DEBUG_DATA_REQ_ID_set  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               mpmo_field_range_DEBUG_DATA_REQ_ID_get  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE mpmo_field_range_DEBUG_DATA_REQ_ID_poll ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE mpmo_field_DEBUG_DATA_REQ_ID_poll       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mpmo_field_DEBUG_DATA_REQ_ABORT_set     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32               mpmo_field_DEBUG_DATA_REQ_ABORT_get     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_field_DEBUG_DATA_REQ_ABORT_poll    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 mpmo_field_DEBUG_DATA_REQ_set           ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | UINT32               mpmo_field_DEBUG_DATA_REQ_get           ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_field_DEBUG_DATA_REQ_poll          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------+
 | | Register 0x0000002c DEBUG_DATA |
 | +--------------------------------+
 | | bit  16:15 R  RAMP_STATE       |
 | | bit  4:0   R  CURR_CSI         |
 | +--------------------------------+
 | 
 | UINT32 mpmo_field_RAMP_STATE_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | UINT32 mpmo_field_range_RAMP_STATE_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 mpmo_field_CURR_CSI_get         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | UINT32 mpmo_field_range_CURR_CSI_get   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x00000030 DEBUG_DATA_1 |
 | +----------------------------------+
 | | bit  15:0 R  PKT_AGE             |
 | +----------------------------------+
 | 
 | UINT32 mpmo_field_PKT_AGE_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | UINT32 mpmo_field_range_PKT_AGE_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register (0x00000040 + (N) * 0x4) CKCTL_CTRL_REG |
 | |     N[0..25]                                     |
 | +--------------------------------------------------+
 | | bit  16  R/W  CKCTL_EN                           |
 | | bit  0:6 R/W  CKCTL_PORT_ID                      |
 | +--------------------------------------------------+
 | 
 | void   mpmo_field_CKCTL_EN_set            ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_CKCTL_EN_get            ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_CKCTL_PORT_ID_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_CKCTL_PORT_ID_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_range_CKCTL_PORT_ID_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_CKCTL_PORT_ID_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00000100 + (N) * 0x10) MPMO_DP_CH_CFG_0 |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  0:11  R/W  XOFF_THRES                          |
 | | bit  28    R/W  DATA_AVAIL_CTRL_EN                  |
 | | bit  23:16 R/W  DATA_AVAIL_THRES                    |
 | +-----------------------------------------------------+
 | 
 | void   mpmo_field_XOFF_THRES_set             ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_XOFF_THRES_get             ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_range_XOFF_THRES_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_XOFF_THRES_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_field_DATA_AVAIL_CTRL_EN_set     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_DATA_AVAIL_CTRL_EN_get     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_DATA_AVAIL_THRES_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_DATA_AVAIL_THRES_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_range_DATA_AVAIL_THRES_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_DATA_AVAIL_THRES_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00000104 + (N) * 0x10) MPMO_DP_CH_CFG_1 |
 | |     N[0..95]                                        |
 | +-----------------------------------------------------+
 | | bit  28    R/W  HOLDOFF_TRIG                        |
 | | bit  25:16 R/W  PKT_SIZE                            |
 | | bit  12    R/W  SSF_CN_CENTER_EN                    |
 | | bit  8     R/W  TS_ENABLE                           |
 | | bit  4     R/W  RAMP_EN                             |
 | | bit  0     R/W  CH_EN                               |
 | | bit  2     R/W  RAMP_UP_DWN2                        |
 | +-----------------------------------------------------+
 | 
 | void   mpmo_field_PKT_SIZE_set         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_PKT_SIZE_get         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_range_PKT_SIZE_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_PKT_SIZE_get   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_field_SSF_CN_CENTER_EN_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_SSF_CN_CENTER_EN_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_TS_ENABLE_set        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_TS_ENABLE_get        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_RAMP_EN_set          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_RAMP_EN_get          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_CH_EN_set            ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_CH_EN_get            ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_RAMP_UP_DWN2_set     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_RAMP_UP_DWN2_get     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_HOLDOFF_TRIG_set     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_HOLDOFF_TRIG_get     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | +----------------------------------------------+
 | | Register (0x00001000 + (N) * 0x10) CFG_RAM_1 |
 | |     N[0..95]                                 |
 | +----------------------------------------------+
 | | bit  31:16 R/W  AGE_IDEAL                    |
 | | bit  15:0  R/W  AGE_OLD                      |
 | +----------------------------------------------+
 | 
 | void   mpmo_field_AGE_IDEAL_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_AGE_IDEAL_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_range_AGE_IDEAL_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_AGE_IDEAL_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_field_AGE_OLD_set         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_AGE_OLD_get         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_range_AGE_OLD_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_AGE_OLD_get   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register (0x00001004 + (N) * 0x10) CFG_RAM_2 |
 | |     N[0..95]                                 |
 | +----------------------------------------------+
 | | bit  31:16 R/W  AGE_THRES_3_2                |
 | | bit  15:0  R/W  AGE_THRES_1_0                |
 | +----------------------------------------------+
 | 
 | void   mpmo_field_AGE_THRES_3_2_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_AGE_THRES_3_2_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_range_AGE_THRES_3_2_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_AGE_THRES_3_2_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_field_AGE_THRES_1_0_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_AGE_THRES_1_0_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_range_AGE_THRES_1_0_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_AGE_THRES_1_0_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register (0x00001008 + (N) * 0x10) CFG_RAM_3 |
 | |     N[0..95]                                 |
 | +----------------------------------------------+
 | | bit  24:24 R/W  RAMP_UP_DWN                  |
 | | bit  0:23  R/W  CN_TARGET                    |
 | | bit  31:8  R/W  CN_TARGET2                   |
 | | bit  7:0   R/W  CN_TARGET_DECIMAL            |
 | +----------------------------------------------+
 | 
 | void   mpmo_field_RAMP_UP_DWN_set             ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_RAMP_UP_DWN_get             ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_CN_TARGET_set               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_CN_TARGET_get               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_range_CN_TARGET_set         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_CN_TARGET_get         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_field_CN_TARGET2_set              ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_CN_TARGET2_get              ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_range_CN_TARGET2_set        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_CN_TARGET2_get        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   mpmo_field_CN_TARGET_DECIMAL_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 mpmo_field_CN_TARGET_DECIMAL_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | void   mpmo_field_range_CN_TARGET_DECIMAL_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_field_range_CN_TARGET_DECIMAL_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00001600 CSI_CHG_INT_REG_0 |
 | +---------------------------------------+
 | | bit  0:31 R/W  CSI_CHG_I              |
 | +---------------------------------------+
 | 
 | void                 mpmo_lfield_CSI_CHG_I_set_to_clear       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpmo_lfield_CSI_CHG_I_get                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpmo_lfield_range_CSI_CHG_I_set_to_clear ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               mpmo_lfield_range_CSI_CHG_I_get          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_range_CSI_CHG_I_poll         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_CSI_CHG_I_poll               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x0000160c SSF_GEN_INT_REG_0 |
 | +---------------------------------------+
 | | bit  0:31 R/W  SSF_GEN_I              |
 | +---------------------------------------+
 | 
 | void                 mpmo_lfield_SSF_GEN_I_set_to_clear       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpmo_lfield_SSF_GEN_I_get                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpmo_lfield_range_SSF_GEN_I_set_to_clear ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               mpmo_lfield_range_SSF_GEN_I_get          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_range_SSF_GEN_I_poll         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_SSF_GEN_I_poll               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x00001618 PKT_DROP_INT_REG_0 |
 | +----------------------------------------+
 | | bit  0:31 R/W  PKT_DROP_I              |
 | +----------------------------------------+
 | 
 | void                 mpmo_lfield_PKT_DROP_I_set_to_clear       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[6] )
 | void                 mpmo_lfield_PKT_DROP_I_get                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[6] )
 | void                 mpmo_lfield_range_PKT_DROP_I_set_to_clear ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               mpmo_lfield_range_PKT_DROP_I_get          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_range_PKT_DROP_I_poll         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_PKT_DROP_I_poll               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[6], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x00001630 EOP_ERR_INT_REG_0 |
 | +---------------------------------------+
 | | bit  0:31 R/W  EOP_ERR_I              |
 | +---------------------------------------+
 | 
 | void                 mpmo_lfield_EOP_ERR_I_set_to_clear       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpmo_lfield_EOP_ERR_I_get                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpmo_lfield_range_EOP_ERR_I_set_to_clear ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               mpmo_lfield_range_EOP_ERR_I_get          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_range_EOP_ERR_I_poll         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_EOP_ERR_I_poll               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x0000163c SOP_ERR_INT_REG_0 |
 | +---------------------------------------+
 | | bit  0:31 R/W  SOP_ERR_I              |
 | +---------------------------------------+
 | 
 | void                 mpmo_lfield_SOP_ERR_I_set_to_clear       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpmo_lfield_SOP_ERR_I_get                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpmo_lfield_range_SOP_ERR_I_set_to_clear ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               mpmo_lfield_range_SOP_ERR_I_get          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_range_SOP_ERR_I_poll         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_SOP_ERR_I_poll               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------------------+
 | | Register 0x00001650 BWR_RAMP_INT_REG_0 |
 | +----------------------------------------+
 | | bit  0:31 R/W  BWR_RAMP_I              |
 | +----------------------------------------+
 | 
 | void                 mpmo_lfield_BWR_RAMP_I_set_to_clear       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpmo_lfield_BWR_RAMP_I_get                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpmo_lfield_range_BWR_RAMP_I_set_to_clear ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               mpmo_lfield_range_BWR_RAMP_I_get          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_range_BWR_RAMP_I_poll         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_BWR_RAMP_I_poll               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------+
 | | Register 0x00001664 CSI_CHG_INT_EN_REG_0 |
 | +------------------------------------------+
 | | bit  0:31 R/W  CSI_CHG_E                 |
 | +------------------------------------------+
 | 
 | void   mpmo_lfield_CSI_CHG_E_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void   mpmo_lfield_CSI_CHG_E_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void   mpmo_lfield_range_CSI_CHG_E_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_lfield_range_CSI_CHG_E_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00001688 SSF_GEN_INT_EN_REG_0 |
 | +------------------------------------------+
 | | bit  0:31 R/W  SSF_GEN_E                 |
 | +------------------------------------------+
 | 
 | void   mpmo_lfield_SSF_GEN_E_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void   mpmo_lfield_SSF_GEN_E_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void   mpmo_lfield_range_SSF_GEN_E_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_lfield_range_SSF_GEN_E_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x000016a4 PKT_DROP_INT_EN_REG_0 |
 | +-------------------------------------------+
 | | bit  0:31 R/W  PKT_DROP_E                 |
 | +-------------------------------------------+
 | 
 | void   mpmo_lfield_PKT_DROP_E_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[6] )
 | void   mpmo_lfield_PKT_DROP_E_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[6] )
 | void   mpmo_lfield_range_PKT_DROP_E_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_lfield_range_PKT_DROP_E_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x000016bc EOP_ERR_INT_EN_REG_0 |
 | +------------------------------------------+
 | | bit  0:31 R/W  EOP_ERR_E                 |
 | +------------------------------------------+
 | 
 | void   mpmo_lfield_EOP_ERR_E_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void   mpmo_lfield_EOP_ERR_E_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void   mpmo_lfield_range_EOP_ERR_E_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_lfield_range_EOP_ERR_E_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x000016c8 SOP_ERR_INT_EN_REG_0 |
 | +------------------------------------------+
 | | bit  0:31 R/W  SOP_ERR_E                 |
 | +------------------------------------------+
 | 
 | void   mpmo_lfield_SOP_ERR_E_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void   mpmo_lfield_SOP_ERR_E_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void   mpmo_lfield_range_SOP_ERR_E_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_lfield_range_SOP_ERR_E_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00001708 BWR_RAMP_INT_EN_REG_0 |
 | +-------------------------------------------+
 | | bit  0:31 R/W  BWR_RAMP_E                 |
 | +-------------------------------------------+
 | 
 | void   mpmo_lfield_BWR_RAMP_E_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void   mpmo_lfield_BWR_RAMP_E_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void   mpmo_lfield_range_BWR_RAMP_E_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_lfield_range_BWR_RAMP_E_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x00001720 BWR_RAMP_INT_V_REG_0 |
 | +------------------------------------------+
 | | bit  0:31 R  BWR_RAMP_V                  |
 | +------------------------------------------+
 | 
 | void                 mpmo_lfield_BWR_RAMP_V_get        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | UINT32               mpmo_lfield_range_BWR_RAMP_V_get  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_range_BWR_RAMP_V_poll ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_BWR_RAMP_V_poll       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------------------+
 | | Register 0x00001730 SSF_STATE_INT_REG_0 |
 | +-----------------------------------------+
 | | bit  31:0 R/W  SSF_STATE_I              |
 | +-----------------------------------------+
 | 
 | void                 mpmo_lfield_SSF_STATE_I_set_to_clear       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpmo_lfield_SSF_STATE_I_get                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void                 mpmo_lfield_range_SSF_STATE_I_set_to_clear ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               mpmo_lfield_range_SSF_STATE_I_get          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_range_SSF_STATE_I_poll         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_SSF_STATE_I_poll               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------+
 | | Register 0x0000173c SSF_STATE_INT_EN_REG_0 |
 | +--------------------------------------------+
 | | bit  31:0 R/W  SSF_STATE_E                 |
 | +--------------------------------------------+
 | 
 | void   mpmo_lfield_SSF_STATE_E_set       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void   mpmo_lfield_SSF_STATE_E_get       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | void   mpmo_lfield_range_SSF_STATE_E_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 mpmo_lfield_range_SSF_STATE_E_get ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x00001748 SSF_STATE_INT_V_REG_0 |
 | +-------------------------------------------+
 | | bit  31:0 R  SSF_STATE_V                  |
 | +-------------------------------------------+
 | 
 | void                 mpmo_lfield_SSF_STATE_V_get        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3] )
 | UINT32               mpmo_lfield_range_SSF_STATE_V_get  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_range_SSF_STATE_V_poll ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE mpmo_lfield_SSF_STATE_V_poll       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value[3], PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _MPMO_IO_H
#define _MPMO_IO_H

#include "mpmo_loc.h"
#include "mpmo_regs.h"
#include "mpmo_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mpmo_buffer_init               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | void                 mpmo_buffer_flush              ( mpmo_buffer_t *b_ptr )
 | UINT32               mpmo_reg_read                  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 mpmo_reg_write                 ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 mpmo_field_set                 ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 mpmo_action_on_write_field_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 mpmo_burst_read                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 mpmo_burst_write               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE mpmo_poll                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 mpmo_reg_GEN_CFG_write                                 ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_GEN_CFG_field_set                             ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_GEN_CFG_read                                  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_CSI_CFG_write                                 ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_CSI_CFG_field_set                             ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_CSI_CFG_read                                  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_AGE_FILT_CTRL_write                           ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_AGE_FILT_CTRL_field_set                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_AGE_FILT_CTRL_read                            ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_DEBUG_CTRL_write                              ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_DEBUG_CTRL_field_set                          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_DEBUG_CTRL_read                               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_DEBUG_CTRL_poll                               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_DATA_AVAIL_CTRL_write                         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_DATA_AVAIL_CTRL_field_set                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_DATA_AVAIL_CTRL_read                          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_CKCTL_CTRL_REG_array_write                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mpmo_reg_CKCTL_CTRL_REG_array_field_set                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_CKCTL_CTRL_REG_array_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mpmo_reg_MPMO_DP_CH_CFG_0_array_write                  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mpmo_reg_MPMO_DP_CH_CFG_0_array_field_set              ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_MPMO_DP_CH_CFG_0_array_read                   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mpmo_reg_MPMO_DP_CH_CFG_1_array_write                  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mpmo_reg_MPMO_DP_CH_CFG_1_array_field_set              ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_MPMO_DP_CH_CFG_1_array_read                   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mpmo_reg_CSI_CHG_INT_EN_REG_0_write                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_CSI_CHG_INT_EN_REG_0_field_set                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_CSI_CHG_INT_EN_REG_0_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_CSI_CHG_INT_EN_REG_1_write                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_CSI_CHG_INT_EN_REG_1_field_set                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_CSI_CHG_INT_EN_REG_1_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_CSI_CHG_INT_EN_REG_2_write                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_CSI_CHG_INT_EN_REG_2_field_set                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_CSI_CHG_INT_EN_REG_2_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_SSF_GEN_INT_EN_REG_0_write                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SSF_GEN_INT_EN_REG_0_field_set                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SSF_GEN_INT_EN_REG_0_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_SSF_GEN_INT_EN_REG_1_write                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SSF_GEN_INT_EN_REG_1_field_set                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SSF_GEN_INT_EN_REG_1_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_SSF_GEN_INT_EN_REG_2_write                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SSF_GEN_INT_EN_REG_2_field_set                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SSF_GEN_INT_EN_REG_2_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_PKT_DROP_INT_EN_REG_0_write                   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_PKT_DROP_INT_EN_REG_0_field_set               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_PKT_DROP_INT_EN_REG_0_read                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_PKT_DROP_INT_EN_REG_1_write                   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_PKT_DROP_INT_EN_REG_1_field_set               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_PKT_DROP_INT_EN_REG_1_read                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_PKT_DROP_INT_EN_REG_2_write                   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_PKT_DROP_INT_EN_REG_2_field_set               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_PKT_DROP_INT_EN_REG_2_read                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_PKT_DROP_INT_EN_REG_3_write                   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_PKT_DROP_INT_EN_REG_3_field_set               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_PKT_DROP_INT_EN_REG_3_read                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_PKT_DROP_INT_EN_REG_4_write                   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_PKT_DROP_INT_EN_REG_4_field_set               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_PKT_DROP_INT_EN_REG_4_read                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_PKT_DROP_INT_EN_REG_5_write                   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_PKT_DROP_INT_EN_REG_5_field_set               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_PKT_DROP_INT_EN_REG_5_read                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_EOP_ERR_INT_EN_REG_0_write                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_EOP_ERR_INT_EN_REG_0_field_set                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_EOP_ERR_INT_EN_REG_0_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_EOP_ERR_INT_EN_REG_1_write                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_EOP_ERR_INT_EN_REG_1_field_set                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_EOP_ERR_INT_EN_REG_1_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_EOP_ERR_INT_EN_REG_2_write                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_EOP_ERR_INT_EN_REG_2_field_set                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_EOP_ERR_INT_EN_REG_2_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_SOP_ERR_INT_EN_REG_0_write                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SOP_ERR_INT_EN_REG_0_field_set                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SOP_ERR_INT_EN_REG_0_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_SOP_ERR_INT_EN_REG_1_write                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SOP_ERR_INT_EN_REG_1_field_set                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SOP_ERR_INT_EN_REG_1_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_SOP_ERR_INT_EN_REG_2_write                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SOP_ERR_INT_EN_REG_2_field_set                ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SOP_ERR_INT_EN_REG_2_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_BWR_RAMP_INT_EN_REG_0_write                   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_BWR_RAMP_INT_EN_REG_0_field_set               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_BWR_RAMP_INT_EN_REG_0_read                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_BWR_RAMP_INT_EN_REG_1_write                   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_BWR_RAMP_INT_EN_REG_1_field_set               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_BWR_RAMP_INT_EN_REG_1_read                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_BWR_RAMP_INT_EN_REG_2_write                   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_BWR_RAMP_INT_EN_REG_2_field_set               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_BWR_RAMP_INT_EN_REG_2_read                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_SSF_STATE_INT_EN_REG_0_write                  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SSF_STATE_INT_EN_REG_0_field_set              ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SSF_STATE_INT_EN_REG_0_read                   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_SSF_STATE_INT_EN_REG_1_write                  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SSF_STATE_INT_EN_REG_1_field_set              ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SSF_STATE_INT_EN_REG_1_read                   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_SSF_STATE_INT_EN_REG_2_write                  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SSF_STATE_INT_EN_REG_2_field_set              ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SSF_STATE_INT_EN_REG_2_read                   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | void                 mpmo_reg_CSI_CHG_INT_REG_0_write                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_CSI_CHG_INT_REG_0_action_on_write_field_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_CSI_CHG_INT_REG_0_read                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_CSI_CHG_INT_REG_0_poll                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_CSI_CHG_INT_REG_1_write                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_CSI_CHG_INT_REG_1_action_on_write_field_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_CSI_CHG_INT_REG_1_read                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_CSI_CHG_INT_REG_1_poll                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_CSI_CHG_INT_REG_2_write                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_CSI_CHG_INT_REG_2_action_on_write_field_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_CSI_CHG_INT_REG_2_read                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_CSI_CHG_INT_REG_2_poll                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_SSF_GEN_INT_REG_0_write                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SSF_GEN_INT_REG_0_action_on_write_field_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SSF_GEN_INT_REG_0_read                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_SSF_GEN_INT_REG_0_poll                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_SSF_GEN_INT_REG_1_write                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SSF_GEN_INT_REG_1_action_on_write_field_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SSF_GEN_INT_REG_1_read                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_SSF_GEN_INT_REG_1_poll                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_SSF_GEN_INT_REG_2_write                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SSF_GEN_INT_REG_2_action_on_write_field_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SSF_GEN_INT_REG_2_read                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_SSF_GEN_INT_REG_2_poll                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_PKT_DROP_INT_REG_0_write                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_PKT_DROP_INT_REG_0_action_on_write_field_set  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_PKT_DROP_INT_REG_0_read                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_0_poll                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_PKT_DROP_INT_REG_1_write                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_PKT_DROP_INT_REG_1_action_on_write_field_set  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_PKT_DROP_INT_REG_1_read                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_1_poll                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_PKT_DROP_INT_REG_2_write                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_PKT_DROP_INT_REG_2_action_on_write_field_set  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_PKT_DROP_INT_REG_2_read                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_2_poll                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_PKT_DROP_INT_REG_3_write                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_PKT_DROP_INT_REG_3_action_on_write_field_set  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_PKT_DROP_INT_REG_3_read                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_3_poll                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_PKT_DROP_INT_REG_4_write                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_PKT_DROP_INT_REG_4_action_on_write_field_set  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_PKT_DROP_INT_REG_4_read                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_4_poll                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_PKT_DROP_INT_REG_5_write                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_PKT_DROP_INT_REG_5_action_on_write_field_set  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_PKT_DROP_INT_REG_5_read                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_PKT_DROP_INT_REG_5_poll                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_EOP_ERR_INT_REG_0_write                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_EOP_ERR_INT_REG_0_action_on_write_field_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_EOP_ERR_INT_REG_0_read                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_EOP_ERR_INT_REG_0_poll                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_EOP_ERR_INT_REG_1_write                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_EOP_ERR_INT_REG_1_action_on_write_field_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_EOP_ERR_INT_REG_1_read                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_EOP_ERR_INT_REG_1_poll                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_EOP_ERR_INT_REG_2_write                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_EOP_ERR_INT_REG_2_action_on_write_field_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_EOP_ERR_INT_REG_2_read                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_EOP_ERR_INT_REG_2_poll                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_SOP_ERR_INT_REG_0_write                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SOP_ERR_INT_REG_0_action_on_write_field_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SOP_ERR_INT_REG_0_read                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_SOP_ERR_INT_REG_0_poll                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_SOP_ERR_INT_REG_1_write                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SOP_ERR_INT_REG_1_action_on_write_field_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SOP_ERR_INT_REG_1_read                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_SOP_ERR_INT_REG_1_poll                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_SOP_ERR_INT_REG_2_write                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SOP_ERR_INT_REG_2_action_on_write_field_set   ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SOP_ERR_INT_REG_2_read                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_SOP_ERR_INT_REG_2_poll                        ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_BWR_RAMP_INT_REG_0_write                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_BWR_RAMP_INT_REG_0_action_on_write_field_set  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_BWR_RAMP_INT_REG_0_read                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_REG_0_poll                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_BWR_RAMP_INT_REG_1_write                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_BWR_RAMP_INT_REG_1_action_on_write_field_set  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_BWR_RAMP_INT_REG_1_read                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_REG_1_poll                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_BWR_RAMP_INT_REG_2_write                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_BWR_RAMP_INT_REG_2_action_on_write_field_set  ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_BWR_RAMP_INT_REG_2_read                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_REG_2_poll                       ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_SSF_STATE_INT_REG_0_write                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SSF_STATE_INT_REG_0_action_on_write_field_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SSF_STATE_INT_REG_0_read                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_REG_0_poll                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_SSF_STATE_INT_REG_1_write                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SSF_STATE_INT_REG_1_action_on_write_field_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SSF_STATE_INT_REG_1_read                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_REG_1_poll                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_SSF_STATE_INT_REG_2_write                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 value )
 | void                 mpmo_reg_SSF_STATE_INT_REG_2_action_on_write_field_set ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_SSF_STATE_INT_REG_2_read                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_REG_2_poll                      ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               mpmo_reg_BWR_RAMP_INT_V_REG_0_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_V_REG_0_poll                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               mpmo_reg_BWR_RAMP_INT_V_REG_1_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_V_REG_1_poll                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               mpmo_reg_BWR_RAMP_INT_V_REG_2_read                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_BWR_RAMP_INT_V_REG_2_poll                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               mpmo_reg_SSF_STATE_INT_V_REG_0_read                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_V_REG_0_poll                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               mpmo_reg_SSF_STATE_INT_V_REG_1_read                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_V_REG_1_poll                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               mpmo_reg_SSF_STATE_INT_V_REG_2_read                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE mpmo_reg_SSF_STATE_INT_V_REG_2_poll                    ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 mpmo_reg_CFG_RAM_1_array_write                         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mpmo_reg_CFG_RAM_1_array_field_set                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_CFG_RAM_1_array_read                          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mpmo_reg_CFG_RAM_2_array_write                         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mpmo_reg_CFG_RAM_2_array_field_set                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_CFG_RAM_2_array_read                          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | void                 mpmo_reg_CFG_RAM_3_array_write                         ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 mpmo_reg_CFG_RAM_3_array_field_set                     ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               mpmo_reg_CFG_RAM_3_array_read                          ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               mpmo_reg_DEBUG_DATA_read                               ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 | UINT32               mpmo_reg_DEBUG_DATA_1_read                             ( mpmo_buffer_t *b_ptr, mpmo_handle_t *h_ptr )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _MPMO_IO_H */
