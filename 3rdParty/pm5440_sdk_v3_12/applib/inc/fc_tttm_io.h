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
 *     and register accessor functions for the fc_tttm block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing fc_tttm io functions is:
 | {
 |     fc_tttm_buffer_t b_ptr[1];
 |     fc_tttm_buffer_init( b_ptr, h_ptr, A );
 |     value = fc_tttm_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     fc_tttm_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     fc_tttm_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = fc_tttm_field_<FIELD>_get( NULL, h_ptr, A );
 | fc_tttm_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------+
 | | Register 0x0002f100 CFG     |
 | +-----------------------------+
 | | bit  26 R/W  DIS_SCRAM      |
 | | bit  25 R/W  FORCE_LF       |
 | | bit  24 R/W  FORCE_ERR      |
 | | bit  14 R/W  LFOS_HIBER     |
 | | bit  13 R/W  LFOS_SYNC_LOST |
 | | bit  12 R/W  LFOS_LINK_FAIL |
 | | bit  0  R/W  SW_RESET       |
 | +-----------------------------+
 | 
 | void   fc_tttm_field_DIS_SCRAM_set      ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_DIS_SCRAM_get      ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_FORCE_LF_set       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_FORCE_LF_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_FORCE_ERR_set      ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_FORCE_ERR_get      ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_LFOS_HIBER_set     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_LFOS_HIBER_get     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_LFOS_SYNC_LOST_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_LFOS_SYNC_LOST_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_LFOS_LINK_FAIL_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_LFOS_LINK_FAIL_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_SW_RESET_set       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_SW_RESET_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------+
 | | Register 0x0002f104 INT |
 | +-------------------------+
 | | bit  5 R/W  DPI_I       |
 | | bit  4 R/W  FOVR_I      |
 | | bit  3 R/W  BADBLK_I    |
 | | bit  2 R/W  LOSYNC_I    |
 | | bit  1 R/W  HIBER_I     |
 | | bit  0 R/W  LINK_FAIL_I |
 | +-------------------------+
 | 
 | void                 fc_tttm_field_DPI_I_set_to_clear       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_tttm_field_DPI_I_get                ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttm_field_DPI_I_poll               ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_tttm_field_FOVR_I_set_to_clear      ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_tttm_field_FOVR_I_get               ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttm_field_FOVR_I_poll              ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_tttm_field_BADBLK_I_set_to_clear    ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_tttm_field_BADBLK_I_get             ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttm_field_BADBLK_I_poll            ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_tttm_field_LOSYNC_I_set_to_clear    ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_tttm_field_LOSYNC_I_get             ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttm_field_LOSYNC_I_poll            ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_tttm_field_HIBER_I_set_to_clear     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_tttm_field_HIBER_I_get              ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttm_field_HIBER_I_poll             ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 fc_tttm_field_LINK_FAIL_I_set_to_clear ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_tttm_field_LINK_FAIL_I_get          ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttm_field_LINK_FAIL_I_poll         ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-----------------------------+
 | | Register 0x0002f108 INT_VAL |
 | +-----------------------------+
 | | bit  2 R  LOSYNC_V          |
 | | bit  1 R  HIBER_V           |
 | | bit  0 R  LINK_FAIL_V       |
 | +-----------------------------+
 | 
 | UINT32               fc_tttm_field_LOSYNC_V_get     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttm_field_LOSYNC_V_poll    ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc_tttm_field_HIBER_V_get      ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttm_field_HIBER_V_poll     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               fc_tttm_field_LINK_FAIL_V_get  ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttm_field_LINK_FAIL_V_poll ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +----------------------------+
 | | Register 0x0002f10c INT_EN |
 | +----------------------------+
 | | bit  5 R/W  DPI_E          |
 | | bit  4 R/W  FOVR_E         |
 | | bit  3 R/W  BADBLK_E       |
 | | bit  2 R/W  LOSYNC_E       |
 | | bit  1 R/W  HIBER_E        |
 | | bit  0 R/W  LINK_FAIL_E    |
 | +----------------------------+
 | 
 | void   fc_tttm_field_DPI_E_set       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_DPI_E_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_FOVR_E_set      ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_FOVR_E_get      ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_BADBLK_E_set    ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_BADBLK_E_get    ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_LOSYNC_E_set    ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_LOSYNC_E_get    ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_HIBER_E_set     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_HIBER_E_get     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_LINK_FAIL_E_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_LINK_FAIL_E_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------------+
 | | Register 0x0002f110 GFP_CORE_HEADER |
 | +-------------------------------------+
 | | bit  31:16 R/W  PLI                 |
 | | bit  15:0  R/W  CHEC                |
 | +-------------------------------------+
 | 
 | void   fc_tttm_field_PLI_set        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_PLI_get        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_range_PLI_set  ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttm_field_range_PLI_get  ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc_tttm_field_CHEC_set       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_CHEC_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_range_CHEC_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttm_field_range_CHEC_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0002f114 GFP_PAYLOAD_HEADER |
 | +----------------------------------------+
 | | bit  31:29 R/W  PTI                    |
 | | bit  28    R/W  PFI                    |
 | | bit  27:24 R/W  EXI                    |
 | | bit  23:16 R/W  UPI                    |
 | | bit  15:0  R/W  THEC                   |
 | +----------------------------------------+
 | 
 | void   fc_tttm_field_PTI_set        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_PTI_get        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_range_PTI_set  ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttm_field_range_PTI_get  ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc_tttm_field_PFI_set        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_PFI_get        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_EXI_set        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_EXI_get        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_range_EXI_set  ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttm_field_range_EXI_get  ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc_tttm_field_UPI_set        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_UPI_get        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_range_UPI_set  ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttm_field_range_UPI_get  ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   fc_tttm_field_THEC_set       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_THEC_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_range_THEC_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttm_field_range_THEC_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0002f118 GFP_RESERVED_BYTES_1_4 |
 | +--------------------------------------------+
 | | bit  31:0 R/W  GFP_RES1                    |
 | +--------------------------------------------+
 | 
 | void   fc_tttm_field_GFP_RES1_set       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_GFP_RES1_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_range_GFP_RES1_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttm_field_range_GFP_RES1_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x0002f11c GFP_RESERVED_BYTES_5_8 |
 | +--------------------------------------------+
 | | bit  31:0 R/W  GFP_RES2                    |
 | +--------------------------------------------+
 | 
 | void   fc_tttm_field_GFP_RES2_set       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_GFP_RES2_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_range_GFP_RES2_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttm_field_range_GFP_RES2_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x0002f120 GFP_RESERVED_BYTES_9_12 |
 | +---------------------------------------------+
 | | bit  31:0 R/W  GFP_RES3                     |
 | +---------------------------------------------+
 | 
 | void   fc_tttm_field_GFP_RES3_set       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_GFP_RES3_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_range_GFP_RES3_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttm_field_range_GFP_RES3_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x0002f124 GFP_RESERVED_BYTES_13_16 |
 | +----------------------------------------------+
 | | bit  31:0 R/W  GFP_RES4                      |
 | +----------------------------------------------+
 | 
 | void   fc_tttm_field_GFP_RES4_set       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_GFP_RES4_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_range_GFP_RES4_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttm_field_range_GFP_RES4_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x0002f128 LOCAL_FAULT_ORDERED_SET_LSB |
 | +-------------------------------------------------+
 | | bit  31:0 R/W  LFOS                             |
 | +-------------------------------------------------+
 | 
 | void   fc_tttm_lfield_LFOS_set       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | void   fc_tttm_lfield_LFOS_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | void   fc_tttm_lfield_range_LFOS_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttm_lfield_range_LFOS_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------+
 | | Register 0x0002f130 ERR_CTRL_BLOCK_LSB |
 | +----------------------------------------+
 | | bit  31:0 R/W  ECB                     |
 | +----------------------------------------+
 | 
 | void   fc_tttm_lfield_ECB_set       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | void   fc_tttm_lfield_ECB_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value[2] )
 | void   fc_tttm_lfield_range_ECB_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttm_lfield_range_ECB_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register 0x0002f13c ORDERED_SET_INSERTION_RATE_NUMERATOR |
 | +----------------------------------------------------------+
 | | bit  31:0 R/W  OSIR_NUM                                  |
 | +----------------------------------------------------------+
 | 
 | void   fc_tttm_field_OSIR_NUM_set       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_OSIR_NUM_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_range_OSIR_NUM_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttm_field_range_OSIR_NUM_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------------+
 | | Register 0x0002f140 ORDERED_SET_INSERTION_RATE_DENOMINATOR |
 | +------------------------------------------------------------+
 | | bit  31:0 R/W  OSIR_DENOM                                  |
 | +------------------------------------------------------------+
 | 
 | void   fc_tttm_field_OSIR_DENOM_set       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 fc_tttm_field_OSIR_DENOM_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   fc_tttm_field_range_OSIR_DENOM_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 fc_tttm_field_range_OSIR_DENOM_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x0002f144 GFP_FRAME_CNT |
 | +-----------------------------------+
 | | bit  23:0 R  GFPCNT               |
 | +-----------------------------------+
 | 
 | UINT32 fc_tttm_field_GFPCNT_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 fc_tttm_field_range_GFPCNT_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x0002f148 GFP_ERRED_FRAMES_CNT |
 | +------------------------------------------+
 | | bit  23:0 R  GFPERRCNT                   |
 | +------------------------------------------+
 | 
 | UINT32 fc_tttm_field_GFPERRCNT_get       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32 fc_tttm_field_range_GFPERRCNT_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x0002f14c CNT_UPDATE |
 | +--------------------------------+
 | | bit  31 R    TIP               |
 | | bit  0  R/W  CNTR_UPDATE       |
 | +--------------------------------+
 | 
 | void                 fc_tttm_field_CNTR_UPDATE_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               fc_tttm_field_CNTR_UPDATE_get ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | UINT32               fc_tttm_field_TIP_get         ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttm_field_TIP_poll        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _FC_TTTM_IO_H
#define _FC_TTTM_IO_H

#include "cbrc_api.h"
#include "fc_tttm_regs.h"
#include "fc_tttm_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fc_tttm_buffer_init               ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 fc_tttm_buffer_flush              ( fc_tttm_buffer_t *b_ptr )
 | UINT32               fc_tttm_reg_read                  ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 fc_tttm_reg_write                 ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 fc_tttm_field_set                 ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 fc_tttm_action_on_write_field_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 fc_tttm_burst_read                ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 fc_tttm_burst_write               ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE fc_tttm_poll                      ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 fc_tttm_reg_CFG_write                                        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_CFG_field_set                                    ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_CFG_read                                         ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttm_reg_GFP_CORE_HEADER_write                            ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_GFP_CORE_HEADER_field_set                        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_GFP_CORE_HEADER_read                             ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttm_reg_GFP_PAYLOAD_HEADER_write                         ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_GFP_PAYLOAD_HEADER_field_set                     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_GFP_PAYLOAD_HEADER_read                          ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttm_reg_GFP_RESERVED_BYTES_1_4_write                     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_GFP_RESERVED_BYTES_1_4_field_set                 ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_GFP_RESERVED_BYTES_1_4_read                      ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttm_reg_GFP_RESERVED_BYTES_5_8_write                     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_GFP_RESERVED_BYTES_5_8_field_set                 ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_GFP_RESERVED_BYTES_5_8_read                      ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttm_reg_GFP_RESERVED_BYTES_9_12_write                    ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_GFP_RESERVED_BYTES_9_12_field_set                ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_GFP_RESERVED_BYTES_9_12_read                     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttm_reg_GFP_RESERVED_BYTES_13_16_write                   ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_GFP_RESERVED_BYTES_13_16_field_set               ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_GFP_RESERVED_BYTES_13_16_read                    ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_write                ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_field_set            ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_LSB_read                 ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_write                ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_field_set            ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_LOCAL_FAULT_ORDERED_SET_MSB_read                 ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttm_reg_ERR_CTRL_BLOCK_LSB_write                         ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_ERR_CTRL_BLOCK_LSB_field_set                     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_ERR_CTRL_BLOCK_LSB_read                          ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttm_reg_ERR_CTRL_BLOCK_MSB_write                         ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_ERR_CTRL_BLOCK_MSB_field_set                     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_ERR_CTRL_BLOCK_MSB_read                          ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_write       ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_field_set   ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_ORDERED_SET_INSERTION_RATE_NUMERATOR_read        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_write     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_field_set ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_ORDERED_SET_INSERTION_RATE_DENOMINATOR_read      ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttm_reg_CNT_UPDATE_write                                 ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_CNT_UPDATE_field_set                             ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_CNT_UPDATE_read                                  ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttm_reg_CNT_UPDATE_poll                                  ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 fc_tttm_reg_INT_EN_write                                     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_INT_EN_field_set                                 ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_INT_EN_read                                      ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 fc_tttm_reg_INT_write                                        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 fc_tttm_reg_INT_action_on_write_field_set                    ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               fc_tttm_reg_INT_read                                         ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttm_reg_INT_poll                                         ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               fc_tttm_reg_INT_VAL_read                                     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE fc_tttm_reg_INT_VAL_poll                                     ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               fc_tttm_reg_GFP_FRAME_CNT_read                               ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32               fc_tttm_reg_GFP_ERRED_FRAMES_CNT_read                        ( fc_tttm_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _FC_TTTM_IO_H */
