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
 *     and register accessor functions for the int_agr block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing int_agr io functions is:
 | {
 |     int_agr_buffer_t b_ptr[1];
 |     int_agr_buffer_init( b_ptr, h_ptr );
 |     value = int_agr_field_<FIELD>_get( b_ptr, h_ptr );
 |     int_agr_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     int_agr_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = int_agr_field_<FIELD>_get( NULL, h_ptr );
 | int_agr_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-----------------------------------------------+
 | | Register 0x000010c8 INT_AGGREGATOR_DEVICE_SET |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  DEVICE_SET                     |
 | +-----------------------------------------------+
 | 
 | void   int_agr_field_DEVICE_SET_set       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 int_agr_field_DEVICE_SET_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   int_agr_field_range_DEVICE_SET_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 int_agr_field_range_DEVICE_SET_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------+
 | | Register 0x000010cc INT_AGGREGATOR_DEVICE_CLEAR |
 | +-------------------------------------------------+
 | | bit  31:0 R/W  DEVICE_CLR                       |
 | +-------------------------------------------------+
 | 
 | void   int_agr_field_DEVICE_CLR_set       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 int_agr_field_DEVICE_CLR_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   int_agr_field_range_DEVICE_CLR_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 int_agr_field_range_DEVICE_CLR_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x000010d0 INT_AGGREGATOR_DEVICE_EN |
 | +----------------------------------------------+
 | | bit  31:0 R  DEVICE_EN                       |
 | +----------------------------------------------+
 | 
 | UINT32 int_agr_field_DEVICE_EN_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 int_agr_field_range_DEVICE_EN_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------+
 | | Register 0x000010d4 INT_AGGREGATOR_DEVICE_STAT |
 | +------------------------------------------------+
 | | bit  31:0 R  DEVICE_STAT                       |
 | +------------------------------------------------+
 | 
 | UINT32 int_agr_field_DEVICE_STAT_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 int_agr_field_range_DEVICE_STAT_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------------+
 | | Register 0x000010d8 INT_AGGREGATOR_DEVICE_RAW_STAT |
 | +----------------------------------------------------+
 | | bit  31:0 R  DEVICE_RSTAT                          |
 | +----------------------------------------------------+
 | 
 | UINT32 int_agr_field_DEVICE_RSTAT_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 int_agr_field_range_DEVICE_RSTAT_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------+
 | | Register 0x000010dc INT_AGGREGATOR_MISC_SET |
 | +---------------------------------------------+
 | | bit  31:0 R/W  MISC_SET                     |
 | +---------------------------------------------+
 | 
 | void   int_agr_field_MISC_SET_set       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 int_agr_field_MISC_SET_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   int_agr_field_range_MISC_SET_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 int_agr_field_range_MISC_SET_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x000010e0 INT_AGGREGATOR_MISC_CLEAR |
 | +-----------------------------------------------+
 | | bit  31:0 R/W  MISC_CLR                       |
 | +-----------------------------------------------+
 | 
 | void   int_agr_field_MISC_CLR_set       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 int_agr_field_MISC_CLR_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   int_agr_field_range_MISC_CLR_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 int_agr_field_range_MISC_CLR_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------+
 | | Register 0x000010e4 INT_AGGREGATOR_MISC_EN |
 | +--------------------------------------------+
 | | bit  31:0 R  MISC_EN                       |
 | +--------------------------------------------+
 | 
 | UINT32 int_agr_field_MISC_EN_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 int_agr_field_range_MISC_EN_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x000010e8 INT_AGGREGATOR_MISC_STAT |
 | +----------------------------------------------+
 | | bit  31:0 R  MISC_STAT                       |
 | +----------------------------------------------+
 | 
 | UINT32 int_agr_field_MISC_STAT_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 int_agr_field_range_MISC_STAT_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x000010ec INT_AGGREGATOR_MISC_RAW_STAT |
 | +--------------------------------------------------+
 | | bit  31:0 R  MISC_RSTAT                          |
 | +--------------------------------------------------+
 | 
 | UINT32 int_agr_field_MISC_RSTAT_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 int_agr_field_range_MISC_RSTAT_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register (0x00001230 + (N) * 4) INTER_PROCESSOR_INTS |
 | |     N[0..63]                                         |
 | +------------------------------------------------------+
 | | bit  0 R/W  IPI_BITS                                 |
 | +------------------------------------------------------+
 | 
 | void                 int_agr_field_IPI_BITS_set  ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | UINT32               int_agr_field_IPI_BITS_get  ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE int_agr_field_IPI_BITS_poll ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32  N, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00001330 IPI2PCIE_0_INT |
 | +------------------------------------+
 | | bit  15:0 R/W  IPI2PCIE_0_I0       |
 | +------------------------------------+
 | 
 | void                 int_agr_field_IPI2PCIE_0_I0_set_to_clear       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32               int_agr_field_IPI2PCIE_0_I0_get                ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 int_agr_field_range_IPI2PCIE_0_I0_set_to_clear ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               int_agr_field_range_IPI2PCIE_0_I0_get          ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2PCIE_0_I0_poll         ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE int_agr_field_IPI2PCIE_0_I0_poll               ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00001334 IPI2PCIE_1_INT |
 | +------------------------------------+
 | | bit  15:0 R/W  IPI2PCIE_1_I1       |
 | +------------------------------------+
 | 
 | void                 int_agr_field_IPI2PCIE_1_I1_set_to_clear       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32               int_agr_field_IPI2PCIE_1_I1_get                ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 int_agr_field_range_IPI2PCIE_1_I1_set_to_clear ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               int_agr_field_range_IPI2PCIE_1_I1_get          ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2PCIE_1_I1_poll         ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE int_agr_field_IPI2PCIE_1_I1_poll               ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00001338 IPI2PCIE_2_INT |
 | +------------------------------------+
 | | bit  15:0 R/W  IPI2PCIE_2_I2       |
 | +------------------------------------+
 | 
 | void                 int_agr_field_IPI2PCIE_2_I2_set_to_clear       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32               int_agr_field_IPI2PCIE_2_I2_get                ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 int_agr_field_range_IPI2PCIE_2_I2_set_to_clear ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               int_agr_field_range_IPI2PCIE_2_I2_get          ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2PCIE_2_I2_poll         ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE int_agr_field_IPI2PCIE_2_I2_poll               ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x0000133c IPI2PCIE_3_INT |
 | +------------------------------------+
 | | bit  15:0 R/W  IPI2PCIE_3_I3       |
 | +------------------------------------+
 | 
 | void                 int_agr_field_IPI2PCIE_3_I3_set_to_clear       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32               int_agr_field_IPI2PCIE_3_I3_get                ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 int_agr_field_range_IPI2PCIE_3_I3_set_to_clear ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               int_agr_field_range_IPI2PCIE_3_I3_get          ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2PCIE_3_I3_poll         ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE int_agr_field_IPI2PCIE_3_I3_poll               ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x00001340 IPI2PCIE_0_INT_EN |
 | +---------------------------------------+
 | | bit  15:0 R/W  IPI2PCIE_0_E0          |
 | +---------------------------------------+
 | 
 | void   int_agr_field_IPI2PCIE_0_E0_set       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 int_agr_field_IPI2PCIE_0_E0_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   int_agr_field_range_IPI2PCIE_0_E0_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 int_agr_field_range_IPI2PCIE_0_E0_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00001344 IPI2PCIE_1_INT_EN |
 | +---------------------------------------+
 | | bit  15:0 R/W  IPI2PCIE_1_E1          |
 | +---------------------------------------+
 | 
 | void   int_agr_field_IPI2PCIE_1_E1_set       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 int_agr_field_IPI2PCIE_1_E1_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   int_agr_field_range_IPI2PCIE_1_E1_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 int_agr_field_range_IPI2PCIE_1_E1_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00001348 IPI2PCIE_2_INT_EN |
 | +---------------------------------------+
 | | bit  15:0 R/W  IPI2PCIE_2_E2          |
 | +---------------------------------------+
 | 
 | void   int_agr_field_IPI2PCIE_2_E2_set       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 int_agr_field_IPI2PCIE_2_E2_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   int_agr_field_range_IPI2PCIE_2_E2_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 int_agr_field_range_IPI2PCIE_2_E2_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x0000134c IPI2PCIE_3_INT_EN |
 | +---------------------------------------+
 | | bit  15:0 R/W  IPI2PCIE_3_E3          |
 | +---------------------------------------+
 | 
 | void   int_agr_field_IPI2PCIE_3_E3_set       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 int_agr_field_IPI2PCIE_3_E3_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   int_agr_field_range_IPI2PCIE_3_E3_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 int_agr_field_range_IPI2PCIE_3_E3_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00001350 IPI2MIPS_0_INT |
 | +------------------------------------+
 | | bit  15:0 R/W  IPI2MIPS_0_I4       |
 | +------------------------------------+
 | 
 | void                 int_agr_field_IPI2MIPS_0_I4_set_to_clear       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32               int_agr_field_IPI2MIPS_0_I4_get                ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 int_agr_field_range_IPI2MIPS_0_I4_set_to_clear ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               int_agr_field_range_IPI2MIPS_0_I4_get          ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2MIPS_0_I4_poll         ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE int_agr_field_IPI2MIPS_0_I4_poll               ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00001354 IPI2MIPS_1_INT |
 | +------------------------------------+
 | | bit  15:0 R/W  IPI2MIPS_1_I5       |
 | +------------------------------------+
 | 
 | void                 int_agr_field_IPI2MIPS_1_I5_set_to_clear       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32               int_agr_field_IPI2MIPS_1_I5_get                ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 int_agr_field_range_IPI2MIPS_1_I5_set_to_clear ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               int_agr_field_range_IPI2MIPS_1_I5_get          ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2MIPS_1_I5_poll         ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE int_agr_field_IPI2MIPS_1_I5_poll               ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x00001358 IPI2MIPS_2_INT |
 | +------------------------------------+
 | | bit  15:0 R/W  IPI2MIPS_2_I6       |
 | +------------------------------------+
 | 
 | void                 int_agr_field_IPI2MIPS_2_I6_set_to_clear       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32               int_agr_field_IPI2MIPS_2_I6_get                ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 int_agr_field_range_IPI2MIPS_2_I6_set_to_clear ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               int_agr_field_range_IPI2MIPS_2_I6_get          ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2MIPS_2_I6_poll         ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE int_agr_field_IPI2MIPS_2_I6_poll               ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +------------------------------------+
 | | Register 0x0000135c IPI2MIPS_3_INT |
 | +------------------------------------+
 | | bit  15:0 R/W  IPI2MIPS_3_I7       |
 | +------------------------------------+
 | 
 | void                 int_agr_field_IPI2MIPS_3_I7_set_to_clear       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32               int_agr_field_IPI2MIPS_3_I7_get                ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 int_agr_field_range_IPI2MIPS_3_I7_set_to_clear ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32               int_agr_field_range_IPI2MIPS_3_I7_get          ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | PMC_POLL_RETURN_TYPE int_agr_field_range_IPI2MIPS_3_I7_poll         ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | PMC_POLL_RETURN_TYPE int_agr_field_IPI2MIPS_3_I7_poll               ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | +---------------------------------------+
 | | Register 0x00001360 IPI2MIPS_0_INT_EN |
 | +---------------------------------------+
 | | bit  15:0 R/W  IPI2MIPS_0_E4          |
 | +---------------------------------------+
 | 
 | void   int_agr_field_IPI2MIPS_0_E4_set       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 int_agr_field_IPI2MIPS_0_E4_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   int_agr_field_range_IPI2MIPS_0_E4_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 int_agr_field_range_IPI2MIPS_0_E4_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00001364 IPI2MIPS_1_INT_EN |
 | +---------------------------------------+
 | | bit  15:0 R/W  IPI2MIPS_1_E5          |
 | +---------------------------------------+
 | 
 | void   int_agr_field_IPI2MIPS_1_E5_set       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 int_agr_field_IPI2MIPS_1_E5_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   int_agr_field_range_IPI2MIPS_1_E5_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 int_agr_field_range_IPI2MIPS_1_E5_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x00001368 IPI2MIPS_2_INT_EN |
 | +---------------------------------------+
 | | bit  15:0 R/W  IPI2MIPS_2_E6          |
 | +---------------------------------------+
 | 
 | void   int_agr_field_IPI2MIPS_2_E6_set       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 int_agr_field_IPI2MIPS_2_E6_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   int_agr_field_range_IPI2MIPS_2_E6_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 int_agr_field_range_IPI2MIPS_2_E6_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------+
 | | Register 0x0000136c IPI2MIPS_3_INT_EN |
 | +---------------------------------------+
 | | bit  15:0 R/W  IPI2MIPS_3_E7          |
 | +---------------------------------------+
 | 
 | void   int_agr_field_IPI2MIPS_3_E7_set       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 int_agr_field_IPI2MIPS_3_E7_get       ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   int_agr_field_range_IPI2MIPS_3_E7_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 int_agr_field_range_IPI2MIPS_3_E7_get ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _INT_AGR_IO_H
#define _INT_AGR_IO_H

#include "pmc_handle.h"
#include "int_agr_regs.h"
#include "int_agr_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 int_agr_buffer_init               ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 int_agr_buffer_flush              ( int_agr_buffer_t *b_ptr )
 | UINT32               int_agr_reg_read                  ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 int_agr_reg_write                 ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 int_agr_field_set                 ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 int_agr_action_on_write_field_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 int_agr_burst_read                ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 int_agr_burst_write               ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE int_agr_poll                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 int_agr_reg_INTER_PROCESSOR_INTS_array_write         ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32  N, UINT32 value )
 | void                 int_agr_reg_INTER_PROCESSOR_INTS_array_field_set     ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_INTER_PROCESSOR_INTS_array_read          ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32  N )
 | PMC_POLL_RETURN_TYPE int_agr_reg_INTER_PROCESSOR_INTS_array_poll          ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32  N, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 int_agr_reg_INT_AGGREGATOR_DEVICE_SET_write          ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_INT_AGGREGATOR_DEVICE_SET_field_set      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_INT_AGGREGATOR_DEVICE_SET_read           ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_write        ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_field_set    ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_INT_AGGREGATOR_DEVICE_CLEAR_read         ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 int_agr_reg_INT_AGGREGATOR_MISC_SET_write            ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_INT_AGGREGATOR_MISC_SET_field_set        ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_INT_AGGREGATOR_MISC_SET_read             ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_write          ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_field_set      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_INT_AGGREGATOR_MISC_CLEAR_read           ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 int_agr_reg_IPI2PCIE_0_INT_write                     ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2PCIE_0_INT_action_on_write_field_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2PCIE_0_INT_read                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE int_agr_reg_IPI2PCIE_0_INT_poll                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 int_agr_reg_IPI2PCIE_1_INT_write                     ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2PCIE_1_INT_action_on_write_field_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2PCIE_1_INT_read                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE int_agr_reg_IPI2PCIE_1_INT_poll                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 int_agr_reg_IPI2PCIE_2_INT_write                     ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2PCIE_2_INT_action_on_write_field_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2PCIE_2_INT_read                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE int_agr_reg_IPI2PCIE_2_INT_poll                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 int_agr_reg_IPI2PCIE_3_INT_write                     ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2PCIE_3_INT_action_on_write_field_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2PCIE_3_INT_read                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE int_agr_reg_IPI2PCIE_3_INT_poll                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 int_agr_reg_IPI2MIPS_0_INT_write                     ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2MIPS_0_INT_action_on_write_field_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2MIPS_0_INT_read                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE int_agr_reg_IPI2MIPS_0_INT_poll                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 int_agr_reg_IPI2MIPS_1_INT_write                     ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2MIPS_1_INT_action_on_write_field_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2MIPS_1_INT_read                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE int_agr_reg_IPI2MIPS_1_INT_poll                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 int_agr_reg_IPI2MIPS_2_INT_write                     ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2MIPS_2_INT_action_on_write_field_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2MIPS_2_INT_read                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE int_agr_reg_IPI2MIPS_2_INT_poll                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 int_agr_reg_IPI2MIPS_3_INT_write                     ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2MIPS_3_INT_action_on_write_field_set ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2MIPS_3_INT_read                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | PMC_POLL_RETURN_TYPE int_agr_reg_IPI2MIPS_3_INT_poll                      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 | 
 | void                 int_agr_reg_IPI2PCIE_0_INT_EN_write                  ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2PCIE_0_INT_EN_field_set              ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2PCIE_0_INT_EN_read                   ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 int_agr_reg_IPI2PCIE_1_INT_EN_write                  ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2PCIE_1_INT_EN_field_set              ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2PCIE_1_INT_EN_read                   ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 int_agr_reg_IPI2PCIE_2_INT_EN_write                  ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2PCIE_2_INT_EN_field_set              ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2PCIE_2_INT_EN_read                   ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 int_agr_reg_IPI2PCIE_3_INT_EN_write                  ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2PCIE_3_INT_EN_field_set              ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2PCIE_3_INT_EN_read                   ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 int_agr_reg_IPI2MIPS_0_INT_EN_write                  ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2MIPS_0_INT_EN_field_set              ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2MIPS_0_INT_EN_read                   ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 int_agr_reg_IPI2MIPS_1_INT_EN_write                  ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2MIPS_1_INT_EN_field_set              ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2MIPS_1_INT_EN_read                   ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 int_agr_reg_IPI2MIPS_2_INT_EN_write                  ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2MIPS_2_INT_EN_field_set              ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2MIPS_2_INT_EN_read                   ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void                 int_agr_reg_IPI2MIPS_3_INT_EN_write                  ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void                 int_agr_reg_IPI2MIPS_3_INT_EN_field_set              ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32               int_agr_reg_IPI2MIPS_3_INT_EN_read                   ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32               int_agr_reg_INT_AGGREGATOR_DEVICE_EN_read            ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32               int_agr_reg_INT_AGGREGATOR_DEVICE_STAT_read          ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32               int_agr_reg_INT_AGGREGATOR_DEVICE_RAW_STAT_read      ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32               int_agr_reg_INT_AGGREGATOR_MISC_EN_read              ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32               int_agr_reg_INT_AGGREGATOR_MISC_STAT_read            ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32               int_agr_reg_INT_AGGREGATOR_MISC_RAW_STAT_read        ( int_agr_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _INT_AGR_IO_H */
