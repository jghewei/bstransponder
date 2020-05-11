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
 *     and register accessor functions for the exco3m block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing exco3m io functions is:
 | {
 |     exco3m_buffer_t b_ptr[1];
 |     exco3m_buffer_init( b_ptr, h_ptr );
 |     value = exco3m_field_<FIELD>_get( b_ptr, h_ptr );
 |     exco3m_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     exco3m_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = exco3m_field_<FIELD>_get( NULL, h_ptr );
 | exco3m_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------+
 | | Register 0x00000000 SW_RESET   |
 | +--------------------------------+
 | | bit  0 R/W  EXCO_CORE_SW_RESET |
 | +--------------------------------+
 | 
 | void   exco3m_field_EXCO_CORE_SW_RESET_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_EXCO_CORE_SW_RESET_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | +-------------------------------------+
 | | Register 0x00000010 RX_PUSH_CFG_REG |
 | +-------------------------------------+
 | | bit  0 R/W  RX_PCS_PUSH_ENA         |
 | +-------------------------------------+
 | 
 | void   exco3m_field_RX_PCS_PUSH_ENA_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_PCS_PUSH_ENA_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | +------------------------------------+
 | | Register 0x00000024 TX_GLOBAL_CTRL |
 | +------------------------------------+
 | | bit  14 R/W  TX_XLGMII_MODE        |
 | | bit  1  R/W  TX_PCS_ERR_INS_EN     |
 | +------------------------------------+
 | 
 | void   exco3m_field_TX_XLGMII_MODE_set    ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_XLGMII_MODE_get    ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_PCS_ERR_INS_EN_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_PCS_ERR_INS_EN_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | +--------------------------------------+
 | | Register 0x00000028 TX_FAULT_CTRL    |
 | +--------------------------------------+
 | | bit  15 R/W  TX_FORCE_LF_OS          |
 | | bit  14 R/W  TX_FORCE_RF_OS          |
 | | bit  13 R/W  TX_FORCE_PCS_ERR        |
 | | bit  12 R/W  TX_FORCE_IDLE_TEST      |
 | | bit  11 R/W  TX_FORCE_LF_OS_LOS      |
 | | bit  10 R/W  TX_FORCE_LF_OS_PCS_LOBL |
 | | bit  9  R/W  TX_FORCE_LF_OS_LOAM     |
 | | bit  8  R/W  TX_FORCE_LF_OS_SKEW     |
 | +--------------------------------------+
 | 
 | void   exco3m_field_TX_FORCE_LF_OS_set          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_FORCE_LF_OS_get          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_FORCE_RF_OS_set          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_FORCE_RF_OS_get          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_FORCE_PCS_ERR_set        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_FORCE_PCS_ERR_get        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_FORCE_IDLE_TEST_set      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_FORCE_IDLE_TEST_get      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_FORCE_LF_OS_LOS_set      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_FORCE_LF_OS_LOS_get      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_FORCE_LF_OS_PCS_LOBL_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_FORCE_LF_OS_PCS_LOBL_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_FORCE_LF_OS_LOAM_set     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_FORCE_LF_OS_LOAM_get     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_FORCE_LF_OS_SKEW_set     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_FORCE_LF_OS_SKEW_get     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | +--------------------------------+
 | | Register 0x00000030 TX_INT_VAL |
 | +--------------------------------+
 | | bit  15 R  TX_LOS_V            |
 | | bit  8  R  TX_PCS_HBER_V       |
 | +--------------------------------+
 | 
 | UINT32               exco3m_field_TX_LOS_V_get       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_LOS_V_poll      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               exco3m_field_TX_PCS_HBER_V_get  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_HBER_V_poll ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00000034 TX_INT_STAT     |
 | +-------------------------------------+
 | | bit  15 R/W  TX_LOS_I               |
 | | bit  14 R/W  TX_LF_OS_DET_I         |
 | | bit  13 R/W  TX_RF_OS_DET_I         |
 | | bit  12 R/W  TX_IDLE_TEST_ERR_DET_I |
 | | bit  11 R/W  TX_SKEW_RANGE_VIOL_I   |
 | | bit  10 R/W  TX_PCS_INV_BLK_DET_I   |
 | | bit  9  R/W  TX_PCS_ERR_BLK_DET_I   |
 | | bit  8  R/W  TX_PCS_HBER_I          |
 | +-------------------------------------+
 | 
 | void                 exco3m_field_TX_LOS_I_set_to_clear               ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_TX_LOS_I_get                        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_LOS_I_poll                       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_TX_LF_OS_DET_I_set_to_clear         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_TX_LF_OS_DET_I_get                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_LF_OS_DET_I_poll                 ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_TX_RF_OS_DET_I_set_to_clear         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_TX_RF_OS_DET_I_get                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_RF_OS_DET_I_poll                 ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_TX_IDLE_TEST_ERR_DET_I_set_to_clear ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_TX_IDLE_TEST_ERR_DET_I_get          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_IDLE_TEST_ERR_DET_I_poll         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_TX_SKEW_RANGE_VIOL_I_set_to_clear   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_TX_SKEW_RANGE_VIOL_I_get            ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_SKEW_RANGE_VIOL_I_poll           ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_TX_PCS_INV_BLK_DET_I_set_to_clear   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_TX_PCS_INV_BLK_DET_I_get            ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_INV_BLK_DET_I_poll           ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_TX_PCS_ERR_BLK_DET_I_set_to_clear   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_TX_PCS_ERR_BLK_DET_I_get            ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_ERR_BLK_DET_I_poll           ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_TX_PCS_HBER_I_set_to_clear          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_TX_PCS_HBER_I_get                   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_HBER_I_poll                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00000038 TX_INT_EN       |
 | +-------------------------------------+
 | | bit  15 R/W  TX_LOS_E               |
 | | bit  14 R/W  TX_LF_OS_DET_E         |
 | | bit  13 R/W  TX_RF_OS_DET_E         |
 | | bit  12 R/W  TX_IDLE_TEST_ERR_DET_E |
 | | bit  11 R/W  TX_SKEW_RANGE_VIOL_E   |
 | | bit  10 R/W  TX_PCS_INV_BLK_DET_E   |
 | | bit  9  R/W  TX_PCS_ERR_BLK_DET_E   |
 | | bit  8  R/W  TX_PCS_HBER_E          |
 | +-------------------------------------+
 | 
 | void   exco3m_field_TX_LOS_E_set               ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_LOS_E_get               ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_LF_OS_DET_E_set         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_LF_OS_DET_E_get         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_RF_OS_DET_E_set         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_RF_OS_DET_E_get         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_IDLE_TEST_ERR_DET_E_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_IDLE_TEST_ERR_DET_E_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_SKEW_RANGE_VIOL_E_set   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_SKEW_RANGE_VIOL_E_get   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_PCS_INV_BLK_DET_E_set   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_PCS_INV_BLK_DET_E_get   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_PCS_ERR_BLK_DET_E_set   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_PCS_ERR_BLK_DET_E_get   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_TX_PCS_HBER_E_set          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_TX_PCS_HBER_E_get          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | +------------------------------------------+
 | | Register 0x00000040 TX_PCS_BER_CNT_LOWER |
 | +------------------------------------------+
 | | bit  15:0 R  TX_PCS_BER_CTR              |
 | +------------------------------------------+
 | 
 | UINT32 exco3m_field_TX_PCS_BER_CTR_get       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | UINT32 exco3m_field_range_TX_PCS_BER_CTR_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000048 TX_PCS_BLOCK_ERR_CNT_LOWER |
 | +------------------------------------------------+
 | | bit  15:0 R  TX_PCS_BLK_ERR_CTR                |
 | +------------------------------------------------+
 | 
 | UINT32 exco3m_field_TX_PCS_BLK_ERR_CTR_get       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | UINT32 exco3m_field_range_TX_PCS_BLK_ERR_CTR_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register (0x00000080 + (N) * 4) TX_LANE_INFO |
 | |     N[0..3]                                  |
 | +----------------------------------------------+
 | | bit  15:11 R  TX_LANE_ID                     |
 | | bit  6:0   R  TX_LANE_SKEW_FIFO_ADDR         |
 | +----------------------------------------------+
 | 
 | UINT32 exco3m_field_TX_LANE_ID_get                   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | UINT32 exco3m_field_range_TX_LANE_ID_get             ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 exco3m_field_TX_LANE_SKEW_FIFO_ADDR_get       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | UINT32 exco3m_field_range_TX_LANE_SKEW_FIFO_ADDR_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00000100 + (N) * 4) TX_LANE_INT_VAL |
 | |     N[0..3]                                     |
 | +-------------------------------------------------+
 | | bit  15 R  TX_PCS_LOBL_V                        |
 | | bit  12 R  TX_LOAM_V                            |
 | +-------------------------------------------------+
 | 
 | UINT32               exco3m_field_TX_PCS_LOBL_V_get  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_LOBL_V_poll ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               exco3m_field_TX_LOAM_V_get      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_LOAM_V_poll     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------------+
 | | Register (0x00000180 + (N) * 4) TX_LANE_INT_STAT |
 | |     N[0..3]                                      |
 | +--------------------------------------------------+
 | | bit  15 R/W  TX_PCS_LOBL_I                       |
 | | bit  14 R/W  TX_PCS_BER_DET_I                    |
 | | bit  13 R/W  TX_PCS_BIP_ERR_DET_I                |
 | | bit  12 R/W  TX_LOAM_I                           |
 | | bit  11 R/W  TX_MRK_INV_DET_I                    |
 | +--------------------------------------------------+
 | 
 | void                 exco3m_field_TX_PCS_LOBL_I_set_to_clear        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               exco3m_field_TX_PCS_LOBL_I_get                 ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_LOBL_I_poll                ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_TX_PCS_BER_DET_I_set_to_clear     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               exco3m_field_TX_PCS_BER_DET_I_get              ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_BER_DET_I_poll             ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_TX_PCS_BIP_ERR_DET_I_set_to_clear ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               exco3m_field_TX_PCS_BIP_ERR_DET_I_get          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_PCS_BIP_ERR_DET_I_poll         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_TX_LOAM_I_set_to_clear            ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               exco3m_field_TX_LOAM_I_get                     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_LOAM_I_poll                    ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_TX_MRK_INV_DET_I_set_to_clear     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               exco3m_field_TX_MRK_INV_DET_I_get              ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_field_TX_MRK_INV_DET_I_poll             ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------------+
 | | Register (0x00000200 + (N) * 4) TX_LANE_INT_EN |
 | |     N[0..3]                                    |
 | +------------------------------------------------+
 | | bit  15 R/W  TX_PCS_LOBL_E                     |
 | | bit  14 R/W  TX_PCS_BER_DET_E                  |
 | | bit  13 R/W  TX_PCS_BIP_ERR_DET_E              |
 | | bit  12 R/W  TX_LOAM_E                         |
 | | bit  11 R/W  TX_MRK_INV_DET_E                  |
 | +------------------------------------------------+
 | 
 | void   exco3m_field_TX_PCS_LOBL_E_set        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 exco3m_field_TX_PCS_LOBL_E_get        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | void   exco3m_field_TX_PCS_BER_DET_E_set     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 exco3m_field_TX_PCS_BER_DET_E_get     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | void   exco3m_field_TX_PCS_BIP_ERR_DET_E_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 exco3m_field_TX_PCS_BIP_ERR_DET_E_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | void   exco3m_field_TX_LOAM_E_set            ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 exco3m_field_TX_LOAM_E_get            ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | void   exco3m_field_TX_MRK_INV_DET_E_set     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 exco3m_field_TX_MRK_INV_DET_E_get     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000300 + (N) * 4) TX_LANE_PCS_BIP_ERR_CNT |
 | |     N[0..3]                                             |
 | +---------------------------------------------------------+
 | | bit  15:0 R  TX_PCS_BIP_ERR_CTR                         |
 | +---------------------------------------------------------+
 | 
 | UINT32 exco3m_field_TX_PCS_BIP_ERR_CTR_get       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | UINT32 exco3m_field_range_TX_PCS_BIP_ERR_CTR_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00000424 RX_GLOBAL_CTRL |
 | +------------------------------------+
 | | bit  14 R/W  RX_XLGMII_MODE        |
 | | bit  1  R/W  RX_PCS_ERR_INS_EN     |
 | +------------------------------------+
 | 
 | void   exco3m_field_RX_XLGMII_MODE_set    ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_XLGMII_MODE_get    ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_PCS_ERR_INS_EN_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_PCS_ERR_INS_EN_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | +-----------------------------------+
 | | Register 0x00000428 RX_FAULT_CTRL |
 | +-----------------------------------+
 | | bit  15 R/W  RX_FORCE_LF_OS       |
 | | bit  14 R/W  RX_FORCE_RF_OS       |
 | | bit  13 R/W  RX_FORCE_PCS_ERR     |
 | | bit  12 R/W  RX_FORCE_IDLE_TEST   |
 | | bit  11 R/W  RX_FORCE_LF_OS_TSF   |
 | | bit  10 R/W  RX_FORCE_LF_OS_LOBL  |
 | | bit  9  R/W  RX_FORCE_LF_OS_LOAM  |
 | | bit  8  R/W  RX_FORCE_LF_OS_SKEW  |
 | +-----------------------------------+
 | 
 | void   exco3m_field_RX_FORCE_LF_OS_set      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_FORCE_LF_OS_get      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_FORCE_RF_OS_set      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_FORCE_RF_OS_get      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_FORCE_PCS_ERR_set    ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_FORCE_PCS_ERR_get    ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_FORCE_IDLE_TEST_set  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_FORCE_IDLE_TEST_get  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_FORCE_LF_OS_TSF_set  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_FORCE_LF_OS_TSF_get  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_FORCE_LF_OS_LOBL_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_FORCE_LF_OS_LOBL_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_FORCE_LF_OS_LOAM_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_FORCE_LF_OS_LOAM_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_FORCE_LF_OS_SKEW_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_FORCE_LF_OS_SKEW_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | +--------------------------------+
 | | Register 0x00000430 RX_INT_VAL |
 | +--------------------------------+
 | | bit  15 R  RX_TSF_V            |
 | | bit  10 R  RX_1027B_LOBL_V     |
 | | bit  9  R  RX_1027B_HBER_V     |
 | +--------------------------------+
 | 
 | UINT32               exco3m_field_RX_TSF_V_get         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_TSF_V_poll        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               exco3m_field_RX_1027B_LOBL_V_get  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_LOBL_V_poll ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | UINT32               exco3m_field_RX_1027B_HBER_V_get  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_HBER_V_poll ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00000434 RX_INT_STAT     |
 | +-------------------------------------+
 | | bit  15 R/W  RX_TSF_I               |
 | | bit  14 R/W  RX_LF_OS_DET_I         |
 | | bit  13 R/W  RX_RF_OS_DET_I         |
 | | bit  12 R/W  RX_IDLE_TEST_ERR_DET_I |
 | | bit  11 R/W  RX_PCS_ERR_BLK_DET_I   |
 | | bit  10 R/W  RX_1027B_LOBL_I        |
 | | bit  9  R/W  RX_1027B_HBER_I        |
 | | bit  8  R/W  RX_1027B_BER_I         |
 | | bit  7  R/W  RX_SKEW_RANGE_VIOL_I   |
 | | bit  5  R/W  RX_513B_POS_ERR_DET_I  |
 | | bit  4  R/W  RX_WELL_PKT_ERR_I      |
 | | bit  2  R/W  RX_LANE_ALGN_MRK_ERR_I |
 | | bit  1  R/W  RX_1027B_PAR_ERR_I     |
 | +-------------------------------------+
 | 
 | void                 exco3m_field_RX_TSF_I_set_to_clear               ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_RX_TSF_I_get                        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_TSF_I_poll                       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_RX_LF_OS_DET_I_set_to_clear         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_RX_LF_OS_DET_I_get                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_LF_OS_DET_I_poll                 ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_RX_RF_OS_DET_I_set_to_clear         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_RX_RF_OS_DET_I_get                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_RF_OS_DET_I_poll                 ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_RX_IDLE_TEST_ERR_DET_I_set_to_clear ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_RX_IDLE_TEST_ERR_DET_I_get          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_IDLE_TEST_ERR_DET_I_poll         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_RX_PCS_ERR_BLK_DET_I_set_to_clear   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_RX_PCS_ERR_BLK_DET_I_get            ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_PCS_ERR_BLK_DET_I_poll           ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_RX_1027B_LOBL_I_set_to_clear        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_RX_1027B_LOBL_I_get                 ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_LOBL_I_poll                ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_RX_1027B_HBER_I_set_to_clear        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_RX_1027B_HBER_I_get                 ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_HBER_I_poll                ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_RX_1027B_BER_I_set_to_clear         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_RX_1027B_BER_I_get                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_BER_I_poll                 ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_RX_SKEW_RANGE_VIOL_I_set_to_clear   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_RX_SKEW_RANGE_VIOL_I_get            ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_SKEW_RANGE_VIOL_I_poll           ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_RX_513B_POS_ERR_DET_I_set_to_clear  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_RX_513B_POS_ERR_DET_I_get           ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_513B_POS_ERR_DET_I_poll          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_RX_WELL_PKT_ERR_I_set_to_clear      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_RX_WELL_PKT_ERR_I_get               ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_WELL_PKT_ERR_I_poll              ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_RX_LANE_ALGN_MRK_ERR_I_set_to_clear ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_RX_LANE_ALGN_MRK_ERR_I_get          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_LANE_ALGN_MRK_ERR_I_poll         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_RX_1027B_PAR_ERR_I_set_to_clear     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32               exco3m_field_RX_1027B_PAR_ERR_I_get              ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_1027B_PAR_ERR_I_poll             ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +-------------------------------------+
 | | Register 0x00000438 RX_INT_EN       |
 | +-------------------------------------+
 | | bit  15 R/W  RX_TSF_E               |
 | | bit  14 R/W  RX_LF_OS_DET_E         |
 | | bit  13 R/W  RX_RF_OS_DET_E         |
 | | bit  12 R/W  RX_IDLE_TEST_ERR_DET_E |
 | | bit  11 R/W  RX_PCS_ERR_BLK_DET_E   |
 | | bit  10 R/W  RX_1027B_LOBL_E        |
 | | bit  9  R/W  RX_1027B_HBER_E        |
 | | bit  8  R/W  RX_1027B_BER_E         |
 | | bit  7  R/W  RX_SKEW_RANGE_VIOL_E   |
 | | bit  5  R/W  RX_513B_POS_ERR_DET_E  |
 | | bit  4  R/W  RX_WELL_PKT_ERR_E      |
 | | bit  2  R/W  RX_LANE_ALGN_MRK_ERR_E |
 | | bit  1  R/W  RX_1027B_PAR_ERR_E     |
 | +-------------------------------------+
 | 
 | void   exco3m_field_RX_TSF_E_set               ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_TSF_E_get               ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_LF_OS_DET_E_set         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_LF_OS_DET_E_get         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_RF_OS_DET_E_set         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_RF_OS_DET_E_get         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_IDLE_TEST_ERR_DET_E_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_IDLE_TEST_ERR_DET_E_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_PCS_ERR_BLK_DET_E_set   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_PCS_ERR_BLK_DET_E_get   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_1027B_LOBL_E_set        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_1027B_LOBL_E_get        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_1027B_HBER_E_set        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_1027B_HBER_E_get        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_1027B_BER_E_set         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_1027B_BER_E_get         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_SKEW_RANGE_VIOL_E_set   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_SKEW_RANGE_VIOL_E_get   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_513B_POS_ERR_DET_E_set  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_513B_POS_ERR_DET_E_get  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_WELL_PKT_ERR_E_set      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_WELL_PKT_ERR_E_get      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_LANE_ALGN_MRK_ERR_E_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_LANE_ALGN_MRK_ERR_E_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void   exco3m_field_RX_1027B_PAR_ERR_E_set     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | UINT32 exco3m_field_RX_1027B_PAR_ERR_E_get     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | +--------------------------------------------+
 | | Register 0x00000440 RX_1027B_BER_CNT_LOWER |
 | +--------------------------------------------+
 | | bit  15:0 R  RX_1027B_BER_CTR              |
 | +--------------------------------------------+
 | 
 | UINT32 exco3m_field_RX_1027B_BER_CTR_get       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | UINT32 exco3m_field_range_RX_1027B_BER_CTR_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x00000448 RX_PCS_BLOCK_ERR_CNT_LOWER |
 | +------------------------------------------------+
 | | bit  15:0 R  RX_PCS_BLK_ERR_CTR                |
 | +------------------------------------------------+
 | 
 | UINT32 exco3m_field_RX_PCS_BLK_ERR_CTR_get       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | UINT32 exco3m_field_range_RX_PCS_BLK_ERR_CTR_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x00000580 + (N) * 4) RX_LANE_INT_VAL |
 | |     N[0..3]                                     |
 | +-------------------------------------------------+
 | | bit  14 R  RX_LOAM_V                            |
 | +-------------------------------------------------+
 | 
 | UINT32               exco3m_field_RX_LOAM_V_get  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_LOAM_V_poll ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +--------------------------------------------------+
 | | Register (0x00000600 + (N) * 4) RX_LANE_INT_STAT |
 | |     N[0..3]                                      |
 | +--------------------------------------------------+
 | | bit  15 R/W  RX_OTN_BIP_ERR_DET_I                |
 | | bit  14 R/W  RX_LOAM_I                           |
 | | bit  13 R/W  RX_PCS_MRK_INV_DET_I                |
 | +--------------------------------------------------+
 | 
 | void                 exco3m_field_RX_OTN_BIP_ERR_DET_I_set_to_clear ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               exco3m_field_RX_OTN_BIP_ERR_DET_I_get          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_OTN_BIP_ERR_DET_I_poll         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_RX_LOAM_I_set_to_clear            ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               exco3m_field_RX_LOAM_I_get                     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_LOAM_I_poll                    ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | void                 exco3m_field_RX_PCS_MRK_INV_DET_I_set_to_clear ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               exco3m_field_RX_PCS_MRK_INV_DET_I_get          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_field_RX_PCS_MRK_INV_DET_I_poll         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------------------+
 | | Register (0x00000680 + (N) * 4) RX_LANE_INT_EN |
 | |     N[0..3]                                    |
 | +------------------------------------------------+
 | | bit  15 R/W  RX_OTN_BIP_ERR_DET_E              |
 | | bit  14 R/W  RX_LOAM_E                         |
 | | bit  13 R/W  RX_PCS_MRK_INV_DET_E              |
 | +------------------------------------------------+
 | 
 | void   exco3m_field_RX_OTN_BIP_ERR_DET_E_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 exco3m_field_RX_OTN_BIP_ERR_DET_E_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | void   exco3m_field_RX_LOAM_E_set            ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 exco3m_field_RX_LOAM_E_get            ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | void   exco3m_field_RX_PCS_MRK_INV_DET_E_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32 exco3m_field_RX_PCS_MRK_INV_DET_E_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00000700 + (N) * 4) RX_LANE_OTN_BIP_ERR_CNT |
 | |     N[0..3]                                             |
 | +---------------------------------------------------------+
 | | bit  15:0 R  RX_OTN_BIP_ERR_CTR                         |
 | +---------------------------------------------------------+
 | 
 | UINT32 exco3m_field_RX_OTN_BIP_ERR_CTR_get       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | UINT32 exco3m_field_range_RX_OTN_BIP_ERR_CTR_get ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _EXCO3M_IO_H
#define _EXCO3M_IO_H

#include "exco3m_loc.h"
#include "exco3m_regs.h"
#include "exco3m_io_inline.h"


/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 exco3m_buffer_init               ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | void                 exco3m_buffer_flush              ( exco3m_buffer_t *b_ptr )
 | UINT32               exco3m_reg_read                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 exco3m_reg_write                 ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 exco3m_field_set                 ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 exco3m_action_on_write_field_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 exco3m_burst_read                ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 exco3m_burst_write               ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE exco3m_poll                      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 exco3m_reg_SW_RESET_write                                   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | void                 exco3m_reg_SW_RESET_field_set                               ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               exco3m_reg_SW_RESET_read                                    ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | void                 exco3m_reg_RX_PUSH_CFG_REG_write                            ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | void                 exco3m_reg_RX_PUSH_CFG_REG_field_set                        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               exco3m_reg_RX_PUSH_CFG_REG_read                             ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | void                 exco3m_reg_TX_GLOBAL_CTRL_write                             ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | void                 exco3m_reg_TX_GLOBAL_CTRL_field_set                         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               exco3m_reg_TX_GLOBAL_CTRL_read                              ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | void                 exco3m_reg_TX_FAULT_CTRL_write                              ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | void                 exco3m_reg_TX_FAULT_CTRL_field_set                          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               exco3m_reg_TX_FAULT_CTRL_read                               ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | void                 exco3m_reg_RX_GLOBAL_CTRL_write                             ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | void                 exco3m_reg_RX_GLOBAL_CTRL_field_set                         ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               exco3m_reg_RX_GLOBAL_CTRL_read                              ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | void                 exco3m_reg_RX_FAULT_CTRL_write                              ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | void                 exco3m_reg_RX_FAULT_CTRL_field_set                          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               exco3m_reg_RX_FAULT_CTRL_read                               ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | void                 exco3m_reg_TX_INT_EN_write                                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | void                 exco3m_reg_TX_INT_EN_field_set                              ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               exco3m_reg_TX_INT_EN_read                                   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | void                 exco3m_reg_RX_INT_EN_write                                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | void                 exco3m_reg_RX_INT_EN_field_set                              ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               exco3m_reg_RX_INT_EN_read                                   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | void                 exco3m_reg_TX_LANE_INT_EN_array_write                       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 exco3m_reg_TX_LANE_INT_EN_array_field_set                   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               exco3m_reg_TX_LANE_INT_EN_array_read                        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | 
 | void                 exco3m_reg_RX_LANE_INT_EN_array_write                       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 exco3m_reg_RX_LANE_INT_EN_array_field_set                   ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               exco3m_reg_RX_LANE_INT_EN_array_read                        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | 
 | void                 exco3m_reg_TX_INT_STAT_write                                ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | void                 exco3m_reg_TX_INT_STAT_action_on_write_field_set            ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               exco3m_reg_TX_INT_STAT_read                                 ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_reg_TX_INT_STAT_poll                                 ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 exco3m_reg_RX_INT_STAT_write                                ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 value )
 | void                 exco3m_reg_RX_INT_STAT_action_on_write_field_set            ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               exco3m_reg_RX_INT_STAT_read                                 ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_reg_RX_INT_STAT_poll                                 ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 exco3m_reg_TX_LANE_INT_STAT_array_write                     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 exco3m_reg_TX_LANE_INT_STAT_array_action_on_write_field_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               exco3m_reg_TX_LANE_INT_STAT_array_read                      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_reg_TX_LANE_INT_STAT_array_poll                      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 exco3m_reg_RX_LANE_INT_STAT_array_write                     ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 exco3m_reg_RX_LANE_INT_STAT_array_action_on_write_field_set ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               exco3m_reg_RX_LANE_INT_STAT_array_read                      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_reg_RX_LANE_INT_STAT_array_poll                      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               exco3m_reg_TX_INT_VAL_read                                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_reg_TX_INT_VAL_poll                                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               exco3m_reg_RX_INT_VAL_read                                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE exco3m_reg_RX_INT_VAL_poll                                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               exco3m_reg_TX_LANE_INT_VAL_array_read                       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_reg_TX_LANE_INT_VAL_array_poll                       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               exco3m_reg_RX_LANE_INT_VAL_array_read                       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE exco3m_reg_RX_LANE_INT_VAL_array_poll                       ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | UINT32               exco3m_reg_TX_PCS_BER_CNT_LOWER_read                        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | UINT32               exco3m_reg_TX_PCS_BER_CNT_UPPER_read                        ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | UINT32               exco3m_reg_TX_PCS_BLOCK_ERR_CNT_LOWER_read                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | UINT32               exco3m_reg_TX_PCS_BLOCK_ERR_CNT_UPPER_read                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | UINT32               exco3m_reg_RX_1027B_BER_CNT_LOWER_read                      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | UINT32               exco3m_reg_RX_1027B_BER_CNT_UPPER_read                      ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | UINT32               exco3m_reg_RX_PCS_BLOCK_ERR_CNT_LOWER_read                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | UINT32               exco3m_reg_RX_PCS_BLOCK_ERR_CNT_UPPER_read                  ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr )
 | 
 | UINT32               exco3m_reg_TX_LANE_INFO_array_read                          ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               exco3m_reg_TX_LANE_PCS_BIP_ERR_CNT_array_read               ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | 
 | UINT32               exco3m_reg_RX_LANE_OTN_BIP_ERR_CNT_array_read               ( exco3m_buffer_t *b_ptr, exco3m_handle_t *h_ptr, UINT32  N )
 | 
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _EXCO3M_IO_H */
