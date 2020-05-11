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
 *     and register accessor functions for the stl256 block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing stl256 io functions is:
 | {
 |     stl256_buffer_t b_ptr[1];
 |     stl256_buffer_init( b_ptr, h_ptr, A );
 |     value = stl256_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     stl256_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     stl256_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = stl256_field_<FIELD>_get( NULL, h_ptr, A );
 | stl256_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +----------------------------------------+
 | | Register 0x00000400 CTRL               |
 | +----------------------------------------+
 | | bit  8   R/W  FORCE_DESKEW_REALIGNMENT |
 | | bit  6   R/W  AIS_ACT_DIS              |
 | | bit  5   R/W  FORCE_AIS                |
 | | bit  4:0 R/W  AF_THRESH                |
 | +----------------------------------------+
 | 
 | void   stl256_field_FORCE_DESKEW_REALIGNMENT_set ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_FORCE_DESKEW_REALIGNMENT_get ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_AIS_ACT_DIS_set              ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_AIS_ACT_DIS_get              ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_FORCE_AIS_set                ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_FORCE_AIS_get                ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_AF_THRESH_set                ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_AF_THRESH_get                ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_range_AF_THRESH_set          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 stl256_field_range_AF_THRESH_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00000404 INT_STA           |
 | +---------------------------------------+
 | | bit  30 R/W  EXCESSIVE_SKEW_I         |
 | | bit  29 R/W  LANE_ID_NOT_CONSISTENT_I |
 | | bit  28 R/W  MARKERS_NOT_UNIQUE_I     |
 | | bit  27 R/W  LANE3_OOR_STL_I          |
 | | bit  26 R/W  LANE2_OOR_STL_I          |
 | | bit  25 R/W  LANE1_OOR_STL_I          |
 | | bit  24 R/W  LANE0_OOR_STL_I          |
 | | bit  23 R/W  LANE3_DLOS_I             |
 | | bit  22 R/W  LANE2_DLOS_I             |
 | | bit  21 R/W  LANE1_DLOS_I             |
 | | bit  20 R/W  LANE0_DLOS_I             |
 | | bit  18 R/W  ATSF_I                   |
 | | bit  17 R/W  DLOL_I                   |
 | | bit  16 R/W  DSTL_AIS_I               |
 | | bit  15 R/W  LANE3_DLOF_STL_I         |
 | | bit  14 R/W  LANE2_DLOF_STL_I         |
 | | bit  13 R/W  LANE1_DLOF_STL_I         |
 | | bit  12 R/W  LANE0_DLOF_STL_I         |
 | | bit  11 R/W  LANE3_OOF_STL_I          |
 | | bit  10 R/W  LANE2_OOF_STL_I          |
 | | bit  9  R/W  LANE1_OOF_STL_I          |
 | | bit  8  R/W  LANE0_OOF_STL_I          |
 | | bit  4  R/W  LANE_FIFO_OVR_I          |
 | | bit  1  R/W  OOF_I                    |
 | | bit  0  R/W  DAIS_I                   |
 | +---------------------------------------+
 | 
 | void                 stl256_field_EXCESSIVE_SKEW_I_set_to_clear         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_EXCESSIVE_SKEW_I_get                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_EXCESSIVE_SKEW_I_poll                 ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE_ID_NOT_CONSISTENT_I_set_to_clear ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE_ID_NOT_CONSISTENT_I_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE_ID_NOT_CONSISTENT_I_poll         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_MARKERS_NOT_UNIQUE_I_set_to_clear     ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_MARKERS_NOT_UNIQUE_I_get              ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_MARKERS_NOT_UNIQUE_I_poll             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE3_OOR_STL_I_set_to_clear          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE3_OOR_STL_I_get                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE3_OOR_STL_I_poll                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE2_OOR_STL_I_set_to_clear          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE2_OOR_STL_I_get                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE2_OOR_STL_I_poll                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE1_OOR_STL_I_set_to_clear          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE1_OOR_STL_I_get                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE1_OOR_STL_I_poll                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE0_OOR_STL_I_set_to_clear          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE0_OOR_STL_I_get                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE0_OOR_STL_I_poll                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE3_DLOS_I_set_to_clear             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE3_DLOS_I_get                      ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE3_DLOS_I_poll                     ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE2_DLOS_I_set_to_clear             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE2_DLOS_I_get                      ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE2_DLOS_I_poll                     ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE1_DLOS_I_set_to_clear             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE1_DLOS_I_get                      ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE1_DLOS_I_poll                     ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE0_DLOS_I_set_to_clear             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE0_DLOS_I_get                      ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE0_DLOS_I_poll                     ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_ATSF_I_set_to_clear                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_ATSF_I_get                            ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_ATSF_I_poll                           ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_DLOL_I_set_to_clear                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_DLOL_I_get                            ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_DLOL_I_poll                           ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_DSTL_AIS_I_set_to_clear               ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_DSTL_AIS_I_get                        ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_DSTL_AIS_I_poll                       ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE3_DLOF_STL_I_set_to_clear         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE3_DLOF_STL_I_get                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE3_DLOF_STL_I_poll                 ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE2_DLOF_STL_I_set_to_clear         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE2_DLOF_STL_I_get                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE2_DLOF_STL_I_poll                 ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE1_DLOF_STL_I_set_to_clear         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE1_DLOF_STL_I_get                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE1_DLOF_STL_I_poll                 ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE0_DLOF_STL_I_set_to_clear         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE0_DLOF_STL_I_get                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE0_DLOF_STL_I_poll                 ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE3_OOF_STL_I_set_to_clear          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE3_OOF_STL_I_get                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE3_OOF_STL_I_poll                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE2_OOF_STL_I_set_to_clear          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE2_OOF_STL_I_get                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE2_OOF_STL_I_poll                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE1_OOF_STL_I_set_to_clear          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE1_OOF_STL_I_get                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE1_OOF_STL_I_poll                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE0_OOF_STL_I_set_to_clear          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE0_OOF_STL_I_get                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE0_OOF_STL_I_poll                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_LANE_FIFO_OVR_I_set_to_clear          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_LANE_FIFO_OVR_I_get                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE_FIFO_OVR_I_poll                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_OOF_I_set_to_clear                    ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_OOF_I_get                             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_OOF_I_poll                            ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 stl256_field_DAIS_I_set_to_clear                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32               stl256_field_DAIS_I_get                            ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_DAIS_I_poll                           ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x0000040c INT_EN            |
 | +---------------------------------------+
 | | bit  30 R/W  EXCESSIVE_SKEW_E         |
 | | bit  29 R/W  LANE_ID_NOT_CONSISTENT_E |
 | | bit  28 R/W  MARKERS_NOT_UNIQUE_E     |
 | | bit  27 R/W  LANE3_OOR_STL_E          |
 | | bit  26 R/W  LANE2_OOR_STL_E          |
 | | bit  25 R/W  LANE1_OOR_STL_E          |
 | | bit  24 R/W  LANE0_OOR_STL_E          |
 | | bit  23 R/W  LANE3_DLOS_E             |
 | | bit  22 R/W  LANE2_DLOS_E             |
 | | bit  21 R/W  LANE1_DLOS_E             |
 | | bit  20 R/W  LANE0_DLOS_E             |
 | | bit  18 R/W  ATSF_E                   |
 | | bit  17 R/W  DLOL_E                   |
 | | bit  16 R/W  DSTL_AIS_E               |
 | | bit  15 R/W  LANE3_DLOF_STL_E         |
 | | bit  14 R/W  LANE2_DLOF_STL_E         |
 | | bit  13 R/W  LANE1_DLOF_STL_E         |
 | | bit  12 R/W  LANE0_DLOF_STL_E         |
 | | bit  11 R/W  LANE3_OOF_STL_E          |
 | | bit  10 R/W  LANE2_OOF_STL_E          |
 | | bit  9  R/W  LANE1_OOF_STL_E          |
 | | bit  8  R/W  LANE0_OOF_STL_E          |
 | | bit  4  R/W  LANE_FIFO_OVR_E          |
 | | bit  1  R/W  OOF_E                    |
 | | bit  0  R/W  DAIS_E                   |
 | +---------------------------------------+
 | 
 | void   stl256_field_EXCESSIVE_SKEW_E_set         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_EXCESSIVE_SKEW_E_get         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE_ID_NOT_CONSISTENT_E_set ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE_ID_NOT_CONSISTENT_E_get ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_MARKERS_NOT_UNIQUE_E_set     ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_MARKERS_NOT_UNIQUE_E_get     ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE3_OOR_STL_E_set          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE3_OOR_STL_E_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE2_OOR_STL_E_set          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE2_OOR_STL_E_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE1_OOR_STL_E_set          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE1_OOR_STL_E_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE0_OOR_STL_E_set          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE0_OOR_STL_E_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE3_DLOS_E_set             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE3_DLOS_E_get             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE2_DLOS_E_set             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE2_DLOS_E_get             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE1_DLOS_E_set             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE1_DLOS_E_get             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE0_DLOS_E_set             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE0_DLOS_E_get             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_ATSF_E_set                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_ATSF_E_get                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_DLOL_E_set                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_DLOL_E_get                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_DSTL_AIS_E_set               ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_DSTL_AIS_E_get               ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE3_DLOF_STL_E_set         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE3_DLOF_STL_E_get         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE2_DLOF_STL_E_set         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE2_DLOF_STL_E_get         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE1_DLOF_STL_E_set         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE1_DLOF_STL_E_get         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE0_DLOF_STL_E_set         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE0_DLOF_STL_E_get         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE3_OOF_STL_E_set          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE3_OOF_STL_E_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE2_OOF_STL_E_set          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE2_OOF_STL_E_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE1_OOF_STL_E_set          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE1_OOF_STL_E_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE0_OOF_STL_E_set          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE0_OOF_STL_E_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_LANE_FIFO_OVR_E_set          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_LANE_FIFO_OVR_E_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_OOF_E_set                    ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_OOF_E_get                    ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void   stl256_field_DAIS_E_set                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 stl256_field_DAIS_E_get                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | +-------------------------------------+
 | | Register 0x00000414 INT_VAL         |
 | +-------------------------------------+
 | | bit  30 R  EXCESSIVE_SKEW_V         |
 | | bit  29 R  LANE_ID_NOT_CONSISTENT_V |
 | | bit  28 R  MARKERS_NOT_UNIQUE_V     |
 | | bit  27 R  LANE3_OOR_STL_V          |
 | | bit  26 R  LANE2_OOR_STL_V          |
 | | bit  25 R  LANE1_OOR_STL_V          |
 | | bit  24 R  LANE0_OOR_STL_V          |
 | | bit  23 R  LANE3_DLOS_V             |
 | | bit  22 R  LANE2_DLOS_V             |
 | | bit  21 R  LANE1_DLOS_V             |
 | | bit  20 R  LANE0_DLOS_V             |
 | | bit  18 R  ATSF_V                   |
 | | bit  17 R  DLOL_V                   |
 | | bit  16 R  DSTL_AIS_V               |
 | | bit  15 R  LANE3_DLOF_STL_V         |
 | | bit  14 R  LANE2_DLOF_STL_V         |
 | | bit  13 R  LANE1_DLOF_STL_V         |
 | | bit  12 R  LANE0_DLOF_STL_V         |
 | | bit  11 R  LANE3_OOF_STL_V          |
 | | bit  10 R  LANE2_OOF_STL_V          |
 | | bit  9  R  LANE1_OOF_STL_V          |
 | | bit  8  R  LANE0_OOF_STL_V          |
 | | bit  1  R  OOF_V                    |
 | | bit  0  R  DAIS_V                   |
 | +-------------------------------------+
 | 
 | UINT32               stl256_field_EXCESSIVE_SKEW_V_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_EXCESSIVE_SKEW_V_poll         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE_ID_NOT_CONSISTENT_V_get  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE_ID_NOT_CONSISTENT_V_poll ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_MARKERS_NOT_UNIQUE_V_get      ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_MARKERS_NOT_UNIQUE_V_poll     ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE3_OOR_STL_V_get           ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE3_OOR_STL_V_poll          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE2_OOR_STL_V_get           ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE2_OOR_STL_V_poll          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE1_OOR_STL_V_get           ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE1_OOR_STL_V_poll          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE0_OOR_STL_V_get           ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE0_OOR_STL_V_poll          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE3_DLOS_V_get              ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE3_DLOS_V_poll             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE2_DLOS_V_get              ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE2_DLOS_V_poll             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE1_DLOS_V_get              ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE1_DLOS_V_poll             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE0_DLOS_V_get              ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE0_DLOS_V_poll             ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_ATSF_V_get                    ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_ATSF_V_poll                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_DLOL_V_get                    ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_DLOL_V_poll                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_DSTL_AIS_V_get                ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_DSTL_AIS_V_poll               ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE3_DLOF_STL_V_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE3_DLOF_STL_V_poll         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE2_DLOF_STL_V_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE2_DLOF_STL_V_poll         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE1_DLOF_STL_V_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE1_DLOF_STL_V_poll         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE0_DLOF_STL_V_get          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE0_DLOF_STL_V_poll         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE3_OOF_STL_V_get           ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE3_OOF_STL_V_poll          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE2_OOF_STL_V_get           ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE2_OOF_STL_V_poll          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE1_OOF_STL_V_get           ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE1_OOF_STL_V_poll          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_LANE0_OOF_STL_V_get           ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_LANE0_OOF_STL_V_poll          ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_OOF_V_get                     ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_OOF_V_poll                    ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               stl256_field_DAIS_V_get                    ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_field_DAIS_V_poll                   ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _STL256_IO_H
#define _STL256_IO_H

#include "cbrc_api.h"
#include "stl256_regs.h"
#include "stl256_io_inline.h"


/*------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 stl256_buffer_init               ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | void                 stl256_buffer_flush              ( stl256_buffer_t *b_ptr )
 | UINT32               stl256_reg_read                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 stl256_reg_write                 ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 stl256_field_set                 ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 stl256_action_on_write_field_set ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 stl256_burst_read                ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 stl256_burst_write               ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE stl256_poll                      ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 stl256_reg_CTRL_write                        ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 stl256_reg_CTRL_field_set                    ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               stl256_reg_CTRL_read                         ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 stl256_reg_INT_EN_write                      ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 stl256_reg_INT_EN_field_set                  ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               stl256_reg_INT_EN_read                       ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | 
 | void                 stl256_reg_INT_STA_write                     ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void                 stl256_reg_INT_STA_action_on_write_field_set ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               stl256_reg_INT_STA_read                      ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_reg_INT_STA_poll                      ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               stl256_reg_INT_VAL_read                      ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A )
 | PMC_POLL_RETURN_TYPE stl256_reg_INT_VAL_poll                      ( stl256_buffer_t *b_ptr, cbrc_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _STL256_IO_H */
