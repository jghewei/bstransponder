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
 *     and register accessor functions for the ilkn block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing ilkn io functions is:
 | {
 |     ilkn_buffer_t b_ptr[1];
 |     ilkn_buffer_init( b_ptr, h_ptr, A );
 |     value = ilkn_field_<FIELD>_get( b_ptr, h_ptr, A );
 |     ilkn_field_<FIELD>_set( b_ptr, h_ptr, A , value );
 | ...
 |     ilkn_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = ilkn_field_<FIELD>_get( NULL, h_ptr, A );
 | ilkn_field_<FIELD>_set( NULL, h_ptr, A , value );
 | 
 +-------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +--------------------------------------------------+
 | | Register (0x00005408 + (N) * 0x200) LANE_DISABLE |
 | |     N[0..1]                                      |
 | +--------------------------------------------------+
 | | bit  23:0 R/W  DISABLE                           |
 | +--------------------------------------------------+
 | 
 | void   ilkn_field_DISABLE_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_DISABLE_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_DISABLE_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_DISABLE_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register (0x00005410 + (N) * 0x200) CFG |
 | |     N[0..1]                             |
 | +-----------------------------------------+
 | | bit  15 R/W  FC_RX_TO_TX_LB             |
 | | bit  14 R/W  DATA_RX_TO_TX_LB           |
 | | bit  13 R/W  FC_TX_TO_RX_LB             |
 | | bit  12 R/W  DATA_TX_TO_RX_LB           |
 | | bit  7  R/W  OUTBAND                    |
 | | bit  6  R/W  INBAND                     |
 | | bit  3  R/W  MULT_FC                    |
 | +-----------------------------------------+
 | 
 | void   ilkn_field_FC_RX_TO_TX_LB_set   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_FC_RX_TO_TX_LB_get   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_DATA_RX_TO_TX_LB_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_DATA_RX_TO_TX_LB_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_FC_TX_TO_RX_LB_set   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_FC_TX_TO_RX_LB_get   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_DATA_TX_TO_RX_LB_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_DATA_TX_TO_RX_LB_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_OUTBAND_set          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_OUTBAND_get          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_INBAND_set           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_INBAND_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_MULT_FC_set          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_MULT_FC_get          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +----------------------------------+
 | | Register 0x00005418 TX_CTRL      |
 | +----------------------------------+
 | | bit  10 R/W  TX_STATUS_MSG_EN    |
 | | bit  7  R/W  TX_DATA_OUT_KILL    |
 | | bit  6  R/W  TX_FC_OUT_KILL      |
 | | bit  1  R/W  IB_UNALIGN_XOFF_DIS |
 | +----------------------------------+
 | 
 | void   ilkn_field_TX_STATUS_MSG_EN_set    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_TX_STATUS_MSG_EN_get    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_TX_DATA_OUT_KILL_set    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_TX_DATA_OUT_KILL_get    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_TX_FC_OUT_KILL_set      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_TX_FC_OUT_KILL_get      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_IB_UNALIGN_XOFF_DIS_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_IB_UNALIGN_XOFF_DIS_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------+
 | | Register 0x00005420 TX_FIFO_CFG |
 | +---------------------------------+
 | | bit  11:8 R/W  WM_HIGH          |
 | | bit  3:0  R/W  WM_LOW           |
 | +---------------------------------+
 | 
 | void   ilkn_field_WM_HIGH_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_WM_HIGH_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_range_WM_HIGH_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_WM_HIGH_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_WM_LOW_set        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_WM_LOW_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_range_WM_LOW_set  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_WM_LOW_get  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register (0x00005430 + (N) * 0x200) BURST_CFG |
 | |     N[0..1]                                   |
 | +-----------------------------------------------+
 | | bit  26:24 R/W  BURSTMAX                      |
 | | bit  18:16 R/W  BURSTMIN                      |
 | | bit  10:8  R/W  BURSTSHORT                    |
 | | bit  2     R/W  MAX_DIS                       |
 | | bit  1     R/W  MIN_DIS                       |
 | +-----------------------------------------------+
 | 
 | void   ilkn_field_BURSTMAX_set         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BURSTMAX_get         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_BURSTMAX_set   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_BURSTMAX_get   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_BURSTMIN_set         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BURSTMIN_get         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_BURSTMIN_set   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_BURSTMIN_get   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_BURSTSHORT_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BURSTSHORT_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_BURSTSHORT_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_BURSTSHORT_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_MAX_DIS_set          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_MAX_DIS_get          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_MIN_DIS_set          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_MIN_DIS_get          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +----------------------------------------------------+
 | | Register (0x00005434 + (N) * 0x200) METAFRAME_SYNC |
 | |     N[0..1]                                        |
 | +----------------------------------------------------+
 | | bit  15:0 R/W  PERIOD                              |
 | +----------------------------------------------------+
 | 
 | void   ilkn_field_PERIOD_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_PERIOD_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_PERIOD_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_PERIOD_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00005438 TX_RATE_LIMITER_0 |
 | +---------------------------------------+
 | | bit  31:16 R/W  CREDIT_MAX            |
 | | bit  10:8  R/W  BYTE_GRAN             |
 | | bit  0     R/W  RL_ENABLE             |
 | +---------------------------------------+
 | 
 | void   ilkn_field_CREDIT_MAX_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_CREDIT_MAX_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_range_CREDIT_MAX_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_CREDIT_MAX_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_BYTE_GRAN_set        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_BYTE_GRAN_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_range_BYTE_GRAN_set  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_BYTE_GRAN_get  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_RL_ENABLE_set        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_RL_ENABLE_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | +---------------------------------------+
 | | Register 0x0000543c TX_RATE_LIMITER_1 |
 | +---------------------------------------+
 | | bit  31:16 R/W  RL_TIME               |
 | | bit  15:0  R/W  RL_DELTA              |
 | +---------------------------------------+
 | 
 | void   ilkn_field_RL_TIME_set        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_RL_TIME_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_range_RL_TIME_set  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_RL_TIME_get  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_RL_DELTA_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_RL_DELTA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_range_RL_DELTA_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_RL_DELTA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register (0x00005474 + (N) * 0x200) CNT_ACCESS |
 | |     N[0..1]                                    |
 | +------------------------------------------------+
 | | bit  26:24 R/W  CMD                            |
 | | bit  19:16 R/W  ACC_TYPE                       |
 | | bit  7:0   R/W  ACC_ADDR                       |
 | +------------------------------------------------+
 | 
 | void   ilkn_field_CMD_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CMD_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_CMD_set      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_CMD_get      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_ACC_TYPE_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_ACC_TYPE_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_ACC_TYPE_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_ACC_TYPE_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_ACC_ADDR_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_ACC_ADDR_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_ACC_ADDR_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_ACC_ADDR_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register (0x00005478 + (N) * 0x200) CNT_FILL |
 | |     N[0..1]                                  |
 | +----------------------------------------------+
 | | bit  31 R  BOVER                             |
 | | bit  30 R  POVER                             |
 | | bit  29 R  EOVER                             |
 | | bit  28 R  BHALF                             |
 | | bit  27 R  PHALF                             |
 | | bit  26 R  EHALF                             |
 | +----------------------------------------------+
 | 
 | UINT32 ilkn_field_BOVER_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_POVER_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_EOVER_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_BHALF_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_PHALF_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_EHALF_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +---------------------------------------------------------+
 | | Register (0x00005480 + (N) * 0x200) BYTE_CNT_WRITE_HIGH |
 | |     N[0..1]                                             |
 | +---------------------------------------------------------+
 | | bit  23:0 R/W  MSB_BC_WDATA                             |
 | +---------------------------------------------------------+
 | 
 | void   ilkn_field_MSB_BC_WDATA_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_MSB_BC_WDATA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_MSB_BC_WDATA_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_MSB_BC_WDATA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x00005484 + (N) * 0x200) BYTE_CNT_WRITE_LOW |
 | |     N[0..1]                                            |
 | +--------------------------------------------------------+
 | | bit  31:0 R/W  LSB_BC_WDATA                            |
 | +--------------------------------------------------------+
 | 
 | void   ilkn_field_LSB_BC_WDATA_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_LSB_BC_WDATA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_LSB_BC_WDATA_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_LSB_BC_WDATA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x00005488 + (N) * 0x200) MSB_PACKET_CNT_WRITE |
 | |     N[0..1]                                              |
 | +----------------------------------------------------------+
 | | bit  15:0 R/W  MSB_PC_WDATA                              |
 | +----------------------------------------------------------+
 | 
 | void   ilkn_field_MSB_PC_WDATA_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_MSB_PC_WDATA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_MSB_PC_WDATA_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_MSB_PC_WDATA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x0000548c + (N) * 0x200) LSB_PACKET_CNT_WRITE |
 | |     N[0..1]                                              |
 | +----------------------------------------------------------+
 | | bit  31:0 R/W  LSB_PC_WDATA                              |
 | +----------------------------------------------------------+
 | 
 | void   ilkn_field_LSB_PC_WDATA_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_LSB_PC_WDATA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_LSB_PC_WDATA_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_LSB_PC_WDATA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------------+
 | | Register (0x00005490 + (N) * 0x200) MSB_ERROR_COUNTER_WRITE |
 | |     N[0..1]                                                 |
 | +-------------------------------------------------------------+
 | | bit  15:0 R/W  MSB_EC_WDATA                                 |
 | +-------------------------------------------------------------+
 | 
 | void   ilkn_field_MSB_EC_WDATA_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_MSB_EC_WDATA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_MSB_EC_WDATA_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_MSB_EC_WDATA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x00005494 + (N) * 0x200) LSB_ERR_CNT_WRITE |
 | |     N[0..1]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R/W  LSB_EC_WDATA                           |
 | +-------------------------------------------------------+
 | 
 | void   ilkn_field_LSB_EC_WDATA_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_LSB_EC_WDATA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_LSB_EC_WDATA_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_LSB_EC_WDATA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------------+
 | | Register (0x000054a0 + (N) * 0x200) BYTE_CNT_HIGH_READ |
 | |     N[0..1]                                            |
 | +--------------------------------------------------------+
 | | bit  31   R  RD_BOVER                                  |
 | | bit  30   R  RD_BHALF                                  |
 | | bit  29   R  RD_BUNCORR                                |
 | | bit  28   R  RD_BCORR                                  |
 | | bit  15:0 R  MSB_BC_RDATA                              |
 | +--------------------------------------------------------+
 | 
 | UINT32 ilkn_field_RD_BOVER_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_RD_BHALF_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_RD_BUNCORR_get         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_RD_BCORR_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_MSB_BC_RDATA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_range_MSB_BC_RDATA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x000054a4 + (N) * 0x200) BYTE_CNT_LOW_READ |
 | |     N[0..1]                                           |
 | +-------------------------------------------------------+
 | | bit  31:0 R  LSB_BC_RDATA                             |
 | +-------------------------------------------------------+
 | 
 | UINT32 ilkn_field_LSB_BC_RDATA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_range_LSB_BC_RDATA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------+
 | | Register (0x000054a8 + (N) * 0x200) PACKET_CNT_HIGH_READ |
 | |     N[0..1]                                              |
 | +----------------------------------------------------------+
 | | bit  31   R  RD_POVER                                    |
 | | bit  30   R  RD_PHALF                                    |
 | | bit  29   R  RD_PUNCORR                                  |
 | | bit  28   R  RD_PCORR                                    |
 | | bit  15:0 R  MSB_PC_RDATA                                |
 | +----------------------------------------------------------+
 | 
 | UINT32 ilkn_field_RD_POVER_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_RD_PHALF_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_RD_PUNCORR_get         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_RD_PCORR_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_MSB_PC_RDATA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_range_MSB_PC_RDATA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------+
 | | Register (0x000054ac + (N) * 0x200) PACKET_CNT_LOW_READ |
 | |     N[0..1]                                             |
 | +---------------------------------------------------------+
 | | bit  31:0 R  LSB_PC_RDATA                               |
 | +---------------------------------------------------------+
 | 
 | UINT32 ilkn_field_LSB_PC_RDATA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_range_LSB_PC_RDATA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register (0x000054b0 + (N) * 0x200) ERR_CNT_HIGH_READ |
 | |     N[0..1]                                           |
 | +-------------------------------------------------------+
 | | bit  31   R  RD_EOVER                                 |
 | | bit  30   R  RD_EHALF                                 |
 | | bit  29   R  RD_EUNCORR                               |
 | | bit  28   R  RD_ECORR                                 |
 | | bit  15:0 R  MSB_EC_RDATA                             |
 | +-------------------------------------------------------+
 | 
 | UINT32 ilkn_field_RD_EOVER_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_RD_EHALF_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_RD_EUNCORR_get         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_RD_ECORR_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_MSB_EC_RDATA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_range_MSB_EC_RDATA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x000054b4 + (N) * 0x200) ERR_CNT_LOW_READ |
 | |     N[0..1]                                          |
 | +------------------------------------------------------+
 | | bit  31:0 R  LSB_EC_RDATA                            |
 | +------------------------------------------------------+
 | 
 | UINT32 ilkn_field_LSB_EC_RDATA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_range_LSB_EC_RDATA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register (0x000054c0 + (N) * 0x200) CALENDAR_EN |
 | |     N[0..1]                                     |
 | +-------------------------------------------------+
 | | bit  18:8 R/W  LAST                             |
 | | bit  0    R/W  PROG                             |
 | +-------------------------------------------------+
 | 
 | void   ilkn_field_LAST_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_LAST_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_LAST_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_LAST_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_PROG_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_PROG_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +------------------------------------------------------------------+
 | | Register (0x000054c8 + (N) * 0x200) PROGRAMMABLE_CALENDAR_ACCESS |
 | |     N[0..1]                                                      |
 | +------------------------------------------------------------------+
 | | bit  25:24 R/W  CAL_ACC_CMD                                      |
 | | bit  10:0  R/W  CAL_ACC_ADDR                                     |
 | +------------------------------------------------------------------+
 | 
 | void   ilkn_field_CAL_ACC_CMD_set        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CAL_ACC_CMD_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_CAL_ACC_CMD_set  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_CAL_ACC_CMD_get  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_CAL_ACC_ADDR_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CAL_ACC_ADDR_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_CAL_ACC_ADDR_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_CAL_ACC_ADDR_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------------------------+
 | | Register (0x000054d0 + (N) * 0x200) PROGRAMMABLE_CALENDAR_WRITE_DATA |
 | |     N[0..1]                                                          |
 | +----------------------------------------------------------------------+
 | | bit  8:0 R/W  CAL_WDATA                                              |
 | +----------------------------------------------------------------------+
 | 
 | void   ilkn_field_CAL_WDATA_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CAL_WDATA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_CAL_WDATA_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_CAL_WDATA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------------------------+
 | | Register (0x000054d8 + (N) * 0x200) PROGRAMMABLE_CALENDAR_READ_DATA |
 | |     N[0..1]                                                         |
 | +---------------------------------------------------------------------+
 | | bit  7:0 R  CAL_RDATA                                               |
 | +---------------------------------------------------------------------+
 | 
 | UINT32 ilkn_field_CAL_RDATA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | UINT32 ilkn_field_range_CAL_RDATA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00005520 + (N) * 0x200) SERDES_TEST_CTRL |
 | |     N[0..1]                                          |
 | +------------------------------------------------------+
 | | bit  31:24 R/W  REPC                                 |
 | | bit  23:16 R/W  REPB                                 |
 | | bit  15:8  R/W  REPA                                 |
 | | bit  1     R/W  PROG_EN                              |
 | | bit  0     R/W  PRBS_EN                              |
 | +------------------------------------------------------+
 | 
 | void   ilkn_field_REPC_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REPC_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REPC_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REPC_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REPB_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REPB_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REPB_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REPB_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REPA_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REPA_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REPA_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REPA_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_PROG_EN_set    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_PROG_EN_get    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_PRBS_EN_set    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_PRBS_EN_get    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x00005524 + (N) * 0x200) SERDES_TEST_PATTERN_A |
 | |     N[0..1]                                               |
 | +-----------------------------------------------------------+
 | | bit  31:0 R/W  TST_PTRN_A_VALUE                           |
 | +-----------------------------------------------------------+
 | 
 | void   ilkn_field_TST_PTRN_A_VALUE_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_TST_PTRN_A_VALUE_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_TST_PTRN_A_VALUE_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_TST_PTRN_A_VALUE_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x00005528 + (N) * 0x200) SERDES_TEST_PATTERN_B |
 | |     N[0..1]                                               |
 | +-----------------------------------------------------------+
 | | bit  31:0 R/W  TST_PTRN_B_VALUE                           |
 | +-----------------------------------------------------------+
 | 
 | void   ilkn_field_TST_PTRN_B_VALUE_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_TST_PTRN_B_VALUE_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_TST_PTRN_B_VALUE_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_TST_PTRN_B_VALUE_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------------+
 | | Register (0x0000552c + (N) * 0x200) SERDES_TEST_PATTERN_C |
 | |     N[0..1]                                               |
 | +-----------------------------------------------------------+
 | | bit  31:0 R/W  TST_PTRN_C_VALUE                           |
 | +-----------------------------------------------------------+
 | 
 | void   ilkn_field_TST_PTRN_C_VALUE_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_TST_PTRN_C_VALUE_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_TST_PTRN_C_VALUE_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_TST_PTRN_C_VALUE_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------+
 | | Register 0x00005540 TX_STAT_MESSAGE_RXD |
 | +-----------------------------------------+
 | | bit  31   R  CURRENT                    |
 | | bit  30   R  VALID                      |
 | | bit  26   R  SYNC                       |
 | | bit  24   R  SIF                        |
 | | bit  23:0 R  TX_SL                      |
 | +-----------------------------------------+
 | 
 | UINT32 ilkn_field_CURRENT_get     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_VALID_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_SYNC_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_SIF_get         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_TX_SL_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_range_TX_SL_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register (0x00005550 + (N) * 0x200) REMAP_LANE_5_0 |
 | |     N[0..1]                                        |
 | +----------------------------------------------------+
 | | bit  29:25 R/W  REMAP_LANE5                        |
 | | bit  24:20 R/W  REMAP_LANE4                        |
 | | bit  19:15 R/W  REMAP_LANE3                        |
 | | bit  14:10 R/W  REMAP_LANE2                        |
 | | bit  9:5   R/W  REMAP_LANE1                        |
 | | bit  4:0   R/W  REMAP_LANE0                        |
 | +----------------------------------------------------+
 | 
 | void   ilkn_field_REMAP_LANE5_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE5_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE5_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE5_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE4_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE4_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE4_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE4_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE3_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE3_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE3_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE3_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE2_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE2_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE2_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE2_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE1_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE1_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE1_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE1_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE0_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE0_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE0_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE0_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------------+
 | | Register (0x00005554 + (N) * 0x200) REMAP_LANE_11_6 |
 | |     N[0..1]                                         |
 | +-----------------------------------------------------+
 | | bit  29:25 R/W  REMAP_LANE11                        |
 | | bit  24:20 R/W  REMAP_LANE10                        |
 | | bit  19:15 R/W  REMAP_LANE9                         |
 | | bit  14:10 R/W  REMAP_LANE8                         |
 | | bit  9:5   R/W  REMAP_LANE7                         |
 | | bit  4:0   R/W  REMAP_LANE6                         |
 | +-----------------------------------------------------+
 | 
 | void   ilkn_field_REMAP_LANE11_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE11_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE11_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE11_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE10_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE10_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE10_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE10_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE9_set        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE9_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE9_set  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE9_get  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE8_set        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE8_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE8_set  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE8_get  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE7_set        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE7_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE7_set  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE7_get  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE6_set        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE6_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE6_set  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE6_get  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00005558 + (N) * 0x200) REMAP_LANE_17_12 |
 | |     N[0..1]                                          |
 | +------------------------------------------------------+
 | | bit  29:25 R/W  REMAP_LANE17                         |
 | | bit  24:20 R/W  REMAP_LANE16                         |
 | | bit  19:15 R/W  REMAP_LANE15                         |
 | | bit  14:10 R/W  REMAP_LANE14                         |
 | | bit  9:5   R/W  REMAP_LANE13                         |
 | | bit  4:0   R/W  REMAP_LANE12                         |
 | +------------------------------------------------------+
 | 
 | void   ilkn_field_REMAP_LANE17_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE17_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE17_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE17_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE16_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE16_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE16_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE16_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE15_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE15_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE15_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE15_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE14_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE14_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE14_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE14_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE13_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE13_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE13_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE13_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE12_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE12_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE12_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE12_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x0000555c + (N) * 0x200) REMAP_LANE_23_18 |
 | |     N[0..1]                                          |
 | +------------------------------------------------------+
 | | bit  29:25 R/W  REMAP_LANE23                         |
 | | bit  24:20 R/W  REMAP_LANE22                         |
 | | bit  19:15 R/W  REMAP_LANE21                         |
 | | bit  14:10 R/W  REMAP_LANE20                         |
 | | bit  9:5   R/W  REMAP_LANE19                         |
 | | bit  4:0   R/W  REMAP_LANE18                         |
 | +------------------------------------------------------+
 | 
 | void   ilkn_field_REMAP_LANE23_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE23_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE23_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE23_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE22_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE22_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE22_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE22_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE21_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE21_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE21_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE21_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE20_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE20_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE20_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE20_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE19_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE19_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE19_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE19_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_REMAP_LANE18_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_REMAP_LANE18_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_range_REMAP_LANE18_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_REMAP_LANE18_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register (0x00005580 + (N) * 0x8) TX_INT |
 | |     N[0..1]                              |
 | +------------------------------------------+
 | | bit  29 R/W  FIFO_OVER_I                 |
 | | bit  28 R/W  FIFO_UNDER_I                |
 | | bit  17 R/W  OOB_FC_ERR_I                |
 | | bit  16 R/W  TX_STATUS_MSG_CHANGE_I      |
 | | bit  1  R/W  TX_FC_CNT_FILL_I            |
 | | bit  0  R/W  TX_CHAN_CNT_FILL_I          |
 | +------------------------------------------+
 | 
 | void   ilkn_field_FIFO_OVER_I_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_FIFO_OVER_I_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_FIFO_UNDER_I_set           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_FIFO_UNDER_I_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_OOB_FC_ERR_I_set           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_OOB_FC_ERR_I_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_TX_STATUS_MSG_CHANGE_I_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_TX_STATUS_MSG_CHANGE_I_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_TX_FC_CNT_FILL_I_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_TX_FC_CNT_FILL_I_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_TX_CHAN_CNT_FILL_I_set     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_TX_CHAN_CNT_FILL_I_get     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +-----------------------------------------------+
 | | Register (0x00005584 + (N) * 0x8) TX_INT_MASK |
 | |     N[0..1]                                   |
 | +-----------------------------------------------+
 | | bit  29 R/W  FIFO_OVER_M                      |
 | | bit  28 R/W  FIFO_UNDER_M                     |
 | | bit  17 R/W  OOB_FC_ERR_M                     |
 | | bit  16 R/W  TX_STATUS_MSG_CHANGE_M           |
 | | bit  1  R/W  TX_FC_CNT_FILL_M                 |
 | | bit  0  R/W  TX_CHAN_CNT_FILL_M               |
 | +-----------------------------------------------+
 | 
 | void   ilkn_field_FIFO_OVER_M_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_FIFO_OVER_M_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_FIFO_UNDER_M_set           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_FIFO_UNDER_M_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_OOB_FC_ERR_M_set           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_OOB_FC_ERR_M_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_TX_STATUS_MSG_CHANGE_M_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_TX_STATUS_MSG_CHANGE_M_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_TX_FC_CNT_FILL_M_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_TX_FC_CNT_FILL_M_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_TX_CHAN_CNT_FILL_M_set     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_TX_CHAN_CNT_FILL_M_get     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +----------------------------------+
 | | Register 0x00005618 RX_CTRL      |
 | +----------------------------------+
 | | bit  11 R/W  RX_STATUS_MSG_CONT  |
 | | bit  10 R/W  RX_STATUS_MSG_EN    |
 | | bit  7  R/W  RX_DATA_OUT_KILL    |
 | | bit  6  R/W  RX_FC_OUT_KILL      |
 | | bit  1  R/W  OB_UNALIGN_XOFF_DIS |
 | +----------------------------------+
 | 
 | void   ilkn_field_RX_STATUS_MSG_CONT_set  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_RX_STATUS_MSG_CONT_get  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_RX_STATUS_MSG_EN_set    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_RX_STATUS_MSG_EN_get    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_RX_DATA_OUT_KILL_set    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_RX_DATA_OUT_KILL_get    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_RX_FC_OUT_KILL_set      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_RX_FC_OUT_KILL_get      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_OB_UNALIGN_XOFF_DIS_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_OB_UNALIGN_XOFF_DIS_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | +----------------------------------------------+
 | | Register 0x00005628 RX_SERDES_FIFO_WATERMARK |
 | +----------------------------------------------+
 | | bit  27:24 R/W  FC_HIGH                      |
 | | bit  19:16 R/W  FC_LOW                       |
 | | bit  11:8  R/W  RX_DATA_HIGH                 |
 | | bit  3:0   R/W  RX_DATA_LOW                  |
 | +----------------------------------------------+
 | 
 | void   ilkn_field_FC_HIGH_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_FC_HIGH_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_range_FC_HIGH_set      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_FC_HIGH_get      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_FC_LOW_set             ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_FC_LOW_get             ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_range_FC_LOW_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_FC_LOW_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_RX_DATA_HIGH_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_RX_DATA_HIGH_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_range_RX_DATA_HIGH_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_RX_DATA_HIGH_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | void   ilkn_field_RX_DATA_LOW_set        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | UINT32 ilkn_field_RX_DATA_LOW_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void   ilkn_field_range_RX_DATA_LOW_set  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 ilkn_field_range_RX_DATA_LOW_get  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register (0x00005780 + (N) * 0x8) RX_INT |
 | |     N[0..1]                              |
 | +------------------------------------------+
 | | bit  31 R/W  CTLBIT_ERR_I                |
 | | bit  29 R/W  MF_ERR_I                    |
 | | bit  28 R/W  BADCTL_ERR_I                |
 | | bit  27 R/W  SCRAM_ERR_I                 |
 | | bit  26 R/W  CRC32_ERR_I                 |
 | | bit  25 R/W  CDR_ERR_I                   |
 | | bit  24 R/W  WRD_SYNC_CHANGE_I           |
 | | bit  23 R/W  ALIGN_CHANGE_I              |
 | | bit  22 R/W  ALIGN_ERR_I                 |
 | | bit  21 R/W  BURST_ERR_I                 |
 | | bit  20 R/W  BMIN_ERR_I                  |
 | | bit  19 R/W  BMAX_ERR_I                  |
 | | bit  18 R/W  CRC24_ERR_I                 |
 | | bit  17 R/W  IB_FC_ERR_I                 |
 | | bit  16 R/W  RX_STATUS_MSG_CHANGE_I      |
 | | bit  13 R/W  BADCTL_CNT_FILL_I           |
 | | bit  10 R/W  CRC32_CNT_FILL_I            |
 | | bit  9  R/W  CDR_CNT_FILL_I              |
 | | bit  8  R/W  WRD_SYNC_CNT_FILL_I         |
 | | bit  7  R/W  ALIGN_FAIL_CNT_FILL_I       |
 | | bit  6  R/W  ALIGN_CNT_FILL_I            |
 | | bit  5  R/W  BURST_CNT_FILL_I            |
 | | bit  4  R/W  BMIN_CNT_FILL_I             |
 | | bit  3  R/W  BMAX_CNT_FILL_I             |
 | | bit  2  R/W  CRC24_CNT_FILL_I            |
 | | bit  1  R/W  RX_FC_CNT_FILL_I            |
 | | bit  0  R/W  RX_CHAN_CNT_FILL_I          |
 | +------------------------------------------+
 | 
 | void   ilkn_field_CTLBIT_ERR_I_set           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CTLBIT_ERR_I_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_MF_ERR_I_set               ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_MF_ERR_I_get               ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BADCTL_ERR_I_set           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BADCTL_ERR_I_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_SCRAM_ERR_I_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_SCRAM_ERR_I_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_CRC32_ERR_I_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CRC32_ERR_I_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_CDR_ERR_I_set              ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CDR_ERR_I_get              ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_WRD_SYNC_CHANGE_I_set      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_WRD_SYNC_CHANGE_I_get      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_ALIGN_CHANGE_I_set         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_ALIGN_CHANGE_I_get         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_ALIGN_ERR_I_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_ALIGN_ERR_I_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BURST_ERR_I_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BURST_ERR_I_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BMIN_ERR_I_set             ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BMIN_ERR_I_get             ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BMAX_ERR_I_set             ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BMAX_ERR_I_get             ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_CRC24_ERR_I_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CRC24_ERR_I_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_IB_FC_ERR_I_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_IB_FC_ERR_I_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_RX_STATUS_MSG_CHANGE_I_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_RX_STATUS_MSG_CHANGE_I_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BADCTL_CNT_FILL_I_set      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BADCTL_CNT_FILL_I_get      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_CRC32_CNT_FILL_I_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CRC32_CNT_FILL_I_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_CDR_CNT_FILL_I_set         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CDR_CNT_FILL_I_get         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_WRD_SYNC_CNT_FILL_I_set    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_WRD_SYNC_CNT_FILL_I_get    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_ALIGN_FAIL_CNT_FILL_I_set  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_ALIGN_FAIL_CNT_FILL_I_get  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_ALIGN_CNT_FILL_I_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_ALIGN_CNT_FILL_I_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BURST_CNT_FILL_I_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BURST_CNT_FILL_I_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BMIN_CNT_FILL_I_set        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BMIN_CNT_FILL_I_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BMAX_CNT_FILL_I_set        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BMAX_CNT_FILL_I_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_CRC24_CNT_FILL_I_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CRC24_CNT_FILL_I_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_RX_FC_CNT_FILL_I_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_RX_FC_CNT_FILL_I_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_RX_CHAN_CNT_FILL_I_set     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_RX_CHAN_CNT_FILL_I_get     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +-----------------------------------------------+
 | | Register (0x00005784 + (N) * 0x8) RX_INT_MASK |
 | |     N[0..1]                                   |
 | +-----------------------------------------------+
 | | bit  31 R/W  CTLBIT_ERR_M                     |
 | | bit  29 R/W  MF_ERR_M                         |
 | | bit  28 R/W  BADCTL_ERR_M                     |
 | | bit  27 R/W  SCRAM_ERR_M                      |
 | | bit  26 R/W  CRC32_ERR_M                      |
 | | bit  25 R/W  CDR_ERR_M                        |
 | | bit  24 R/W  WRD_SYNC_CHANGE_M                |
 | | bit  23 R/W  ALIGN_CHANGE_M                   |
 | | bit  22 R/W  ALIGN_ERR_M                      |
 | | bit  21 R/W  BURST_ERR_M                      |
 | | bit  20 R/W  BMIN_ERR_M                       |
 | | bit  19 R/W  BMAX_ERR_M                       |
 | | bit  18 R/W  CRC24_ERR_M                      |
 | | bit  17 R/W  IB_FC_ERR_M                      |
 | | bit  16 R/W  RX_STATUS_MSG_CHANGE_M           |
 | | bit  13 R/W  BADCTL_CNT_FILL_M                |
 | | bit  10 R/W  CRC32_CNT_FILL_M                 |
 | | bit  9  R/W  CDR_CNT_FILL_M                   |
 | | bit  8  R/W  WRD_SYNC_CNT_FILL_M              |
 | | bit  7  R/W  ALIGN_FAIL_CNT_FILL_M            |
 | | bit  6  R/W  ALIGN_CNT_FILL_M                 |
 | | bit  5  R/W  BURST_CNT_FILL_M                 |
 | | bit  4  R/W  BMIN_CNT_FILL_M                  |
 | | bit  3  R/W  BMAX_CNT_FILL_M                  |
 | | bit  2  R/W  CRC24_CNT_FILL_M                 |
 | | bit  1  R/W  RX_FC_CNT_FILL_M                 |
 | | bit  0  R/W  RX_CHAN_CNT_FILL_M               |
 | +-----------------------------------------------+
 | 
 | void   ilkn_field_CTLBIT_ERR_M_set           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CTLBIT_ERR_M_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_MF_ERR_M_set               ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_MF_ERR_M_get               ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BADCTL_ERR_M_set           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BADCTL_ERR_M_get           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_SCRAM_ERR_M_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_SCRAM_ERR_M_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_CRC32_ERR_M_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CRC32_ERR_M_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_CDR_ERR_M_set              ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CDR_ERR_M_get              ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_WRD_SYNC_CHANGE_M_set      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_WRD_SYNC_CHANGE_M_get      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_ALIGN_CHANGE_M_set         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_ALIGN_CHANGE_M_get         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_ALIGN_ERR_M_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_ALIGN_ERR_M_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BURST_ERR_M_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BURST_ERR_M_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BMIN_ERR_M_set             ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BMIN_ERR_M_get             ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BMAX_ERR_M_set             ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BMAX_ERR_M_get             ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_CRC24_ERR_M_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CRC24_ERR_M_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_IB_FC_ERR_M_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_IB_FC_ERR_M_get            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_RX_STATUS_MSG_CHANGE_M_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_RX_STATUS_MSG_CHANGE_M_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BADCTL_CNT_FILL_M_set      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BADCTL_CNT_FILL_M_get      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_CRC32_CNT_FILL_M_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CRC32_CNT_FILL_M_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_CDR_CNT_FILL_M_set         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CDR_CNT_FILL_M_get         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_WRD_SYNC_CNT_FILL_M_set    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_WRD_SYNC_CNT_FILL_M_get    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_ALIGN_FAIL_CNT_FILL_M_set  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_ALIGN_FAIL_CNT_FILL_M_get  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_ALIGN_CNT_FILL_M_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_ALIGN_CNT_FILL_M_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BURST_CNT_FILL_M_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BURST_CNT_FILL_M_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BMIN_CNT_FILL_M_set        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BMIN_CNT_FILL_M_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_BMAX_CNT_FILL_M_set        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_BMAX_CNT_FILL_M_get        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_CRC24_CNT_FILL_M_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_CRC24_CNT_FILL_M_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_RX_FC_CNT_FILL_M_set       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_RX_FC_CNT_FILL_M_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | void   ilkn_field_RX_CHAN_CNT_FILL_M_set     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | UINT32 ilkn_field_RX_CHAN_CNT_FILL_M_get     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | +--------------------------------------+
 | | Register 0x000057ac RX_METAFRAME_ERR |
 | +--------------------------------------+
 | | bit  23:0 R  RX_MF_ERR               |
 | +--------------------------------------+
 | 
 | UINT32 ilkn_field_RX_MF_ERR_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_range_RX_MF_ERR_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x000057b0 RX_BAD_CTRL_WORD_ERR |
 | +------------------------------------------+
 | | bit  23:0 R  RX_BADCTL_ERR               |
 | +------------------------------------------+
 | 
 | UINT32 ilkn_field_RX_BADCTL_ERR_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_range_RX_BADCTL_ERR_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x000057b4 RX_SCRAMBLER_STATE_ERR |
 | +--------------------------------------------+
 | | bit  23:0 R  RX_SCRAM_ERR                  |
 | +--------------------------------------------+
 | 
 | UINT32 ilkn_field_RX_SCRAM_ERR_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_range_RX_SCRAM_ERR_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x000057b8 RX_CRC32_ERR |
 | +----------------------------------+
 | | bit  23:0 R  RX_CRC32_ERR        |
 | +----------------------------------+
 | 
 | UINT32 ilkn_field_RX_CRC32_ERR_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_range_RX_CRC32_ERR_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x000057bc RX_CDR_ERR |
 | +--------------------------------+
 | | bit  23:0 R  RX_CDR_ERR        |
 | +--------------------------------+
 | 
 | UINT32 ilkn_field_RX_CDR_ERR_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_range_RX_CDR_ERR_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x000057c0 RX_WORD_SYNC_CHANGED |
 | +------------------------------------------+
 | | bit  23:0 R  RX_WRD_SYNC_CHG             |
 | +------------------------------------------+
 | 
 | UINT32 ilkn_field_RX_WRD_SYNC_CHG_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_range_RX_WRD_SYNC_CHG_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x000057c4 RX_ALIGN_STAT |
 | +-----------------------------------+
 | | bit  23 R  RX_ALIGN_GOOD          |
 | | bit  22 R  RX_ALIGN_ERR           |
 | +-----------------------------------+
 | 
 | UINT32 ilkn_field_RX_ALIGN_GOOD_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_RX_ALIGN_ERR_get  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | +--------------------------------------+
 | | Register 0x000057c8 STAT_MESSAGE_RXD |
 | +--------------------------------------+
 | | bit  31   R  RX_STATUS_MSG_VALID_ALL |
 | | bit  30   R  RX_STATUS_MSG_VALID_ANY |
 | | bit  26   R  RX_STATUS_MSG_SYNC      |
 | | bit  25   R  SIF_ALL                 |
 | | bit  24   R  SIF_ANY                 |
 | | bit  23:0 R  RX_SL                   |
 | +--------------------------------------+
 | 
 | UINT32 ilkn_field_RX_STATUS_MSG_VALID_ALL_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_RX_STATUS_MSG_VALID_ANY_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_RX_STATUS_MSG_SYNC_get      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_SIF_ALL_get                 ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_SIF_ANY_get                 ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_RX_SL_get                   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_range_RX_SL_get             ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------+
 | | Register 0x000057cc RX_WORD_SYNC |
 | +----------------------------------+
 | | bit  23:0 R  RX_WRD_SYNC_LANE    |
 | +----------------------------------+
 | 
 | UINT32 ilkn_field_RX_WRD_SYNC_LANE_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_range_RX_WRD_SYNC_LANE_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------+
 | | Register 0x000057d8 CRC32_CNT_FILL  |
 | +-------------------------------------+
 | | bit  23:0 R  RX_CRC32_CNT_FILL_LANE |
 | +-------------------------------------+
 | 
 | UINT32 ilkn_field_RX_CRC32_CNT_FILL_LANE_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_range_RX_CRC32_CNT_FILL_LANE_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------+
 | | Register 0x000057dc RX_CDR_LOST_CNT_FILL |
 | +------------------------------------------+
 | | bit  23:0 R  RX_CRD_CNT_FILL_LANE        |
 | +------------------------------------------+
 | 
 | UINT32 ilkn_field_RX_CRD_CNT_FILL_LANE_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_range_RX_CRD_CNT_FILL_LANE_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------+
 | | Register 0x000057e0 RX_WORD_SYNC_CNT_FILL |
 | +-------------------------------------------+
 | | bit  23:0 R  RX_WRD_SYNC_CNT_FILL_LANE    |
 | +-------------------------------------------+
 | 
 | UINT32 ilkn_field_RX_WRD_SYNC_CNT_FILL_LANE_get       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | UINT32 ilkn_field_range_RX_WRD_SYNC_CNT_FILL_LANE_get ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 start_bit, UINT32 stop_bit )
 +------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _ILKN_IO_H
#define _ILKN_IO_H

#include "sifd_api.h"
#include "ilkn_regs.h"
#include "ilkn_io_inline.h"


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 ilkn_buffer_init               ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | void                 ilkn_buffer_flush              ( ilkn_buffer_t *b_ptr )
 | UINT32               ilkn_reg_read                  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg )
 | void                 ilkn_reg_write                 ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 ilkn_field_set                 ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 ilkn_action_on_write_field_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 ilkn_burst_read                ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 ilkn_burst_write               ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE ilkn_poll                      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   ilkn_reg_TX_CTRL_write                                    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   ilkn_reg_TX_CTRL_field_set                                ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_TX_CTRL_read                                     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void   ilkn_reg_TX_FIFO_CFG_write                                ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   ilkn_reg_TX_FIFO_CFG_field_set                            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_TX_FIFO_CFG_read                                 ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void   ilkn_reg_TX_RATE_LIMITER_0_write                          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   ilkn_reg_TX_RATE_LIMITER_0_field_set                      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_TX_RATE_LIMITER_0_read                           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void   ilkn_reg_TX_RATE_LIMITER_1_write                          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   ilkn_reg_TX_RATE_LIMITER_1_field_set                      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_TX_RATE_LIMITER_1_read                           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void   ilkn_reg_RX_CTRL_write                                    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   ilkn_reg_RX_CTRL_field_set                                ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_RX_CTRL_read                                     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void   ilkn_reg_RX_SERDES_FIFO_WATERMARK_write                   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 value )
 | void   ilkn_reg_RX_SERDES_FIFO_WATERMARK_field_set               ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_RX_SERDES_FIFO_WATERMARK_read                    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | void   ilkn_reg_LANE_DISABLE_array_write                         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_LANE_DISABLE_array_field_set                     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_LANE_DISABLE_array_read                          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_CFG_array_write                                  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_CFG_array_field_set                              ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_CFG_array_read                                   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_BURST_CFG_array_write                            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_BURST_CFG_array_field_set                        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_BURST_CFG_array_read                             ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_METAFRAME_SYNC_array_write                       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_METAFRAME_SYNC_array_field_set                   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_METAFRAME_SYNC_array_read                        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_CNT_ACCESS_array_write                           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_CNT_ACCESS_array_field_set                       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_CNT_ACCESS_array_read                            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_BYTE_CNT_WRITE_HIGH_array_write                  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_BYTE_CNT_WRITE_HIGH_array_field_set              ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_BYTE_CNT_WRITE_HIGH_array_read                   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_BYTE_CNT_WRITE_LOW_array_write                   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_BYTE_CNT_WRITE_LOW_array_field_set               ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_BYTE_CNT_WRITE_LOW_array_read                    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_MSB_PACKET_CNT_WRITE_array_write                 ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_MSB_PACKET_CNT_WRITE_array_field_set             ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_MSB_PACKET_CNT_WRITE_array_read                  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_LSB_PACKET_CNT_WRITE_array_write                 ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_LSB_PACKET_CNT_WRITE_array_field_set             ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_LSB_PACKET_CNT_WRITE_array_read                  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_MSB_ERROR_COUNTER_WRITE_array_write              ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_MSB_ERROR_COUNTER_WRITE_array_field_set          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_MSB_ERROR_COUNTER_WRITE_array_read               ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_LSB_ERR_CNT_WRITE_array_write                    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_LSB_ERR_CNT_WRITE_array_field_set                ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_LSB_ERR_CNT_WRITE_array_read                     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_CALENDAR_EN_array_write                          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_CALENDAR_EN_array_field_set                      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_CALENDAR_EN_array_read                           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_PROGRAMMABLE_CALENDAR_ACCESS_array_write         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_PROGRAMMABLE_CALENDAR_ACCESS_array_field_set     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_PROGRAMMABLE_CALENDAR_ACCESS_array_read          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_PROGRAMMABLE_CALENDAR_WRITE_DATA_array_write     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_PROGRAMMABLE_CALENDAR_WRITE_DATA_array_field_set ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_PROGRAMMABLE_CALENDAR_WRITE_DATA_array_read      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_SERDES_TEST_CTRL_array_write                     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_SERDES_TEST_CTRL_array_field_set                 ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_SERDES_TEST_CTRL_array_read                      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_SERDES_TEST_PATTERN_A_array_write                ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_SERDES_TEST_PATTERN_A_array_field_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_SERDES_TEST_PATTERN_A_array_read                 ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_SERDES_TEST_PATTERN_B_array_write                ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_SERDES_TEST_PATTERN_B_array_field_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_SERDES_TEST_PATTERN_B_array_read                 ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_SERDES_TEST_PATTERN_C_array_write                ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_SERDES_TEST_PATTERN_C_array_field_set            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_SERDES_TEST_PATTERN_C_array_read                 ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_REMAP_LANE_5_0_array_write                       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_REMAP_LANE_5_0_array_field_set                   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_REMAP_LANE_5_0_array_read                        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_REMAP_LANE_11_6_array_write                      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_REMAP_LANE_11_6_array_field_set                  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_REMAP_LANE_11_6_array_read                       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_REMAP_LANE_17_12_array_write                     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_REMAP_LANE_17_12_array_field_set                 ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_REMAP_LANE_17_12_array_read                      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_REMAP_LANE_23_18_array_write                     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_REMAP_LANE_23_18_array_field_set                 ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_REMAP_LANE_23_18_array_read                      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_TX_INT_array_write                               ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_TX_INT_array_field_set                           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_TX_INT_array_read                                ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_TX_INT_MASK_array_write                          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_TX_INT_MASK_array_field_set                      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_TX_INT_MASK_array_read                           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_RX_INT_array_write                               ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_RX_INT_array_field_set                           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_RX_INT_array_read                                ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | void   ilkn_reg_RX_INT_MASK_array_write                          ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 value )
 | void   ilkn_reg_RX_INT_MASK_array_field_set                      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 ilkn_reg_RX_INT_MASK_array_read                           ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32 ilkn_reg_TX_STAT_MESSAGE_RXD_read                         ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 ilkn_reg_RX_METAFRAME_ERR_read                            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 ilkn_reg_RX_BAD_CTRL_WORD_ERR_read                        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 ilkn_reg_RX_SCRAMBLER_STATE_ERR_read                      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 ilkn_reg_RX_CRC32_ERR_read                                ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 ilkn_reg_RX_CDR_ERR_read                                  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 ilkn_reg_RX_WORD_SYNC_CHANGED_read                        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 ilkn_reg_RX_ALIGN_STAT_read                               ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 ilkn_reg_STAT_MESSAGE_RXD_read                            ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 ilkn_reg_RX_WORD_SYNC_read                                ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 ilkn_reg_CRC32_CNT_FILL_read                              ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 ilkn_reg_RX_CDR_LOST_CNT_FILL_read                        ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 ilkn_reg_RX_WORD_SYNC_CNT_FILL_read                       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A )
 | 
 | UINT32 ilkn_reg_CNT_FILL_array_read                              ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32 ilkn_reg_BYTE_CNT_HIGH_READ_array_read                    ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32 ilkn_reg_BYTE_CNT_LOW_READ_array_read                     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32 ilkn_reg_PACKET_CNT_HIGH_READ_array_read                  ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32 ilkn_reg_PACKET_CNT_LOW_READ_array_read                   ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32 ilkn_reg_ERR_CNT_HIGH_READ_array_read                     ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32 ilkn_reg_ERR_CNT_LOW_READ_array_read                      ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 | UINT32 ilkn_reg_PROGRAMMABLE_CALENDAR_READ_DATA_array_read       ( ilkn_buffer_t *b_ptr, sifd_handle_t *h_ptr, UINT32 A, UINT32  N )
 | 
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _ILKN_IO_H */
