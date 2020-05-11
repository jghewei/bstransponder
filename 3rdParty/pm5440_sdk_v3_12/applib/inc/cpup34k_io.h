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
 *     and register accessor functions for the cpup34k block
 *****************************************************************************/

/*--------------------------------------------------------------------------------.
 | The pattern for accessing cpup34k io functions is:
 | {
 |     cpup34k_buffer_t b_ptr[1];
 |     cpup34k_buffer_init( b_ptr, h_ptr );
 |     value = cpup34k_field_<FIELD>_get( b_ptr, h_ptr );
 |     cpup34k_field_<FIELD>_set( b_ptr, h_ptr , value );
 | ...
 |     cpup34k_buffer_flush( b_ptr );
 | }
 | 
 | alternatively, if register coalescing is not needed then omit the buffer like:
 | 
 | value = cpup34k_field_<FIELD>_get( NULL, h_ptr );
 | cpup34k_field_<FIELD>_set( NULL, h_ptr , value );
 | 
 +-------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | 
 | +-------------------------------------+
 | | Register 0x00000000 SOFT_RESET_BITS |
 | +-------------------------------------+
 | | bit  30 R/W  MIPS_SW_RST            |
 | | bit  18 R/W  NIC_SW_RST             |
 | | bit  0  R/W  MIPS_SW_RST_CLEAR      |
 | +-------------------------------------+
 | 
 | void   cpup34k_field_MIPS_SW_RST_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_MIPS_SW_RST_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_NIC_SW_RST_set        ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_NIC_SW_RST_get        ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_MIPS_SW_RST_CLEAR_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_MIPS_SW_RST_CLEAR_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | +---------------------------------------+
 | | Register 0x00000004 SOFT_RESET_EN     |
 | +---------------------------------------+
 | | bit  30 R/W  MIPS_SW_RESET_EN         |
 | | bit  18 R/W  NIC_SW_RST_EN            |
 | | bit  0  R/W  MIPS_SW_RST_AUTOCLEAR_EN |
 | +---------------------------------------+
 | 
 | void   cpup34k_field_MIPS_SW_RESET_EN_set         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_MIPS_SW_RESET_EN_get         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_NIC_SW_RST_EN_set            ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_NIC_SW_RST_EN_get            ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_MIPS_SW_RST_AUTOCLEAR_EN_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_MIPS_SW_RST_AUTOCLEAR_EN_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x00000054 BOOT_CTRL |
 | +-------------------------------+
 | | bit  0 R/W  BOOT_ENABLE       |
 | +-------------------------------+
 | 
 | void   cpup34k_field_BOOT_ENABLE_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_BOOT_ENABLE_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | +------------------------------+
 | | Register 0x00000058 BOOT_ADR |
 | +------------------------------+
 | | bit  31:0 R/W  BOOT_JMP_ADDR |
 | +------------------------------+
 | 
 | void   cpup34k_field_BOOT_JMP_ADDR_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_BOOT_JMP_ADDR_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_BOOT_JMP_ADDR_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_BOOT_JMP_ADDR_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------+
 | | Register 0x0000005c BOOT_STAT |
 | +-------------------------------+
 | | bit  1 R/W  BOOT_FAIL         |
 | | bit  0 R/W  BOOT_DONE         |
 | +-------------------------------+
 | 
 | void   cpup34k_field_BOOT_FAIL_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_BOOT_FAIL_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_BOOT_DONE_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_BOOT_DONE_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x00000060 BOOT_INFO |
 | +-------------------------------+
 | | bit  15:0 R/W  BOOT_INFO      |
 | +-------------------------------+
 | 
 | void   cpup34k_field_BOOT_INFO_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_BOOT_INFO_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_BOOT_INFO_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_BOOT_INFO_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------+
 | | Register 0x00000068 BOOT_CAUSE_REG |
 | +------------------------------------+
 | | bit  3 R  CAUSE_MIPS_SELF_RST      |
 | | bit  2 R  CAUSE_WDOG0_TIMEOUT      |
 | | bit  1 R  CAUSE_WDOG1_TIMEOUT      |
 | | bit  0 R  CAUSE_MIPS_SW_RST        |
 | +------------------------------------+
 | 
 | UINT32 cpup34k_field_CAUSE_MIPS_SELF_RST_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 cpup34k_field_CAUSE_WDOG0_TIMEOUT_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 cpup34k_field_CAUSE_WDOG1_TIMEOUT_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 cpup34k_field_CAUSE_MIPS_SW_RST_get   ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | +-------------------------------+
 | | Register 0x00000180 DEVICE_ID |
 | +-------------------------------+
 | | bit  31:28 R  REVISION_CODE   |
 | | bit  27:12 R  DEVICE_ID       |
 | | bit  11:0  R  PMC_ID          |
 | +-------------------------------+
 | 
 | UINT32 cpup34k_field_REVISION_CODE_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 cpup34k_field_range_REVISION_CODE_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 cpup34k_field_DEVICE_ID_get           ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 cpup34k_field_range_DEVICE_ID_get     ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | UINT32 cpup34k_field_PMC_ID_get              ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 cpup34k_field_range_PMC_ID_get        ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000184 APP_SCRATCH_0 |
 | +-----------------------------------+
 | | bit  31:0 R/W  APP_SCRATCH0       |
 | +-----------------------------------+
 | 
 | void   cpup34k_field_APP_SCRATCH0_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_APP_SCRATCH0_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_APP_SCRATCH0_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_APP_SCRATCH0_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000188 APP_SCRATCH_1 |
 | +-----------------------------------+
 | | bit  31:0 R/W  APP_SCRATCH1       |
 | +-----------------------------------+
 | 
 | void   cpup34k_field_APP_SCRATCH1_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_APP_SCRATCH1_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_APP_SCRATCH1_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_APP_SCRATCH1_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x0000018c APP_SCRATCH_2 |
 | +-----------------------------------+
 | | bit  31:0 R/W  APP_SCRATCH2       |
 | +-----------------------------------+
 | 
 | void   cpup34k_field_APP_SCRATCH2_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_APP_SCRATCH2_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_APP_SCRATCH2_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_APP_SCRATCH2_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000190 APP_SCRATCH_3 |
 | +-----------------------------------+
 | | bit  31:0 R/W  APP_SCRATCH3       |
 | +-----------------------------------+
 | 
 | void   cpup34k_field_APP_SCRATCH3_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_APP_SCRATCH3_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_APP_SCRATCH3_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_APP_SCRATCH3_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000194 APP_SCRATCH_4 |
 | +-----------------------------------+
 | | bit  31:0 R/W  APP_SCRATCH4       |
 | +-----------------------------------+
 | 
 | void   cpup34k_field_APP_SCRATCH4_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_APP_SCRATCH4_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_APP_SCRATCH4_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_APP_SCRATCH4_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x00000198 APP_SCRATCH_5 |
 | +-----------------------------------+
 | | bit  31:0 R/W  APP_SCRATCH5       |
 | +-----------------------------------+
 | 
 | void   cpup34k_field_APP_SCRATCH5_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_APP_SCRATCH5_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_APP_SCRATCH5_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_APP_SCRATCH5_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x0000019c APP_SCRATCH_6 |
 | +-----------------------------------+
 | | bit  31:0 R/W  APP_SCRATCH6       |
 | +-----------------------------------+
 | 
 | void   cpup34k_field_APP_SCRATCH6_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_APP_SCRATCH6_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_APP_SCRATCH6_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_APP_SCRATCH6_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------+
 | | Register 0x000001a0 APP_SCRATCH_7 |
 | +-----------------------------------+
 | | bit  31:0 R/W  APP_SCRATCH7       |
 | +-----------------------------------+
 | 
 | void   cpup34k_field_APP_SCRATCH7_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_APP_SCRATCH7_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_APP_SCRATCH7_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_APP_SCRATCH7_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x000001a4 APP_USER_0 |
 | +--------------------------------+
 | | bit  31:0 R/W  APP_USER0       |
 | +--------------------------------+
 | 
 | void   cpup34k_field_APP_USER0_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_APP_USER0_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_APP_USER0_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_APP_USER0_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------+
 | | Register 0x000001a8 APP_USER_1 |
 | +--------------------------------+
 | | bit  31:0 R/W  APP_USER1       |
 | +--------------------------------+
 | 
 | void   cpup34k_field_APP_USER1_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_APP_USER1_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_APP_USER1_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_APP_USER1_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x000001ac BASE_ADD_INBOUND_MAILBOX_REG |
 | +--------------------------------------------------+
 | | bit  31:0 R/W  BASE_ADD_INBOUND_MAILBOX          |
 | +--------------------------------------------------+
 | 
 | void   cpup34k_field_BASE_ADD_INBOUND_MAILBOX_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_BASE_ADD_INBOUND_MAILBOX_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_BASE_ADD_INBOUND_MAILBOX_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_BASE_ADD_INBOUND_MAILBOX_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x000001b0 BASE_ADD_OUTBOUND_MAILBOX_REG |
 | +---------------------------------------------------+
 | | bit  31:0 R/W  BASE_ADD_OUTBOUND_MAILBOX          |
 | +---------------------------------------------------+
 | 
 | void   cpup34k_field_BASE_ADD_OUTBOUND_MAILBOX_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_BASE_ADD_OUTBOUND_MAILBOX_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_BASE_ADD_OUTBOUND_MAILBOX_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_BASE_ADD_OUTBOUND_MAILBOX_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +----------------------------------------------+
 | | Register 0x000001b4 SIZE_INBOUND_MAILBOX_REG |
 | +----------------------------------------------+
 | | bit  15:0 R/W  SIZE_INBOUND_MAILBOX          |
 | +----------------------------------------------+
 | 
 | void   cpup34k_field_SIZE_INBOUND_MAILBOX_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_SIZE_INBOUND_MAILBOX_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_SIZE_INBOUND_MAILBOX_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_SIZE_INBOUND_MAILBOX_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------------------------+
 | | Register 0x000001b8 SIZE_OUTBOUND_MAILBOX_REG |
 | +-----------------------------------------------+
 | | bit  15:0 R/W  SIZE_OUTBOUND_MAILBOX          |
 | +-----------------------------------------------+
 | 
 | void   cpup34k_field_SIZE_OUTBOUND_MAILBOX_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_SIZE_OUTBOUND_MAILBOX_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_SIZE_OUTBOUND_MAILBOX_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_SIZE_OUTBOUND_MAILBOX_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +------------------------------------------------------+
 | | Register 0x000001bc MAX_MESSAGES_INBOUND_MAILBOX_REG |
 | +------------------------------------------------------+
 | | bit  15:0 R/W  MAX_MESSAGES_INBOUND_MAILBOX          |
 | +------------------------------------------------------+
 | 
 | void   cpup34k_field_MAX_MESSAGES_INBOUND_MAILBOX_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_MAX_MESSAGES_INBOUND_MAILBOX_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_MAX_MESSAGES_INBOUND_MAILBOX_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_MAX_MESSAGES_INBOUND_MAILBOX_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-------------------------------------------------------+
 | | Register 0x000001c0 MAX_MESSAGES_OUTBOUND_MAILBOX_REG |
 | +-------------------------------------------------------+
 | | bit  15:0 R/W  MAX_MESSAGES_OUTBOUND_MAILBOX          |
 | +-------------------------------------------------------+
 | 
 | void   cpup34k_field_MAX_MESSAGES_OUTBOUND_MAILBOX_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_MAX_MESSAGES_OUTBOUND_MAILBOX_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_MAX_MESSAGES_OUTBOUND_MAILBOX_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_MAX_MESSAGES_OUTBOUND_MAILBOX_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +--------------------------------------------------+
 | | Register 0x000001c4 MAX_SIZE_INBOUND_MESSAGE_REG |
 | +--------------------------------------------------+
 | | bit  15:0 R/W  MAX_SIZE_INBOUND_MESSAGE          |
 | +--------------------------------------------------+
 | 
 | void   cpup34k_field_MAX_SIZE_INBOUND_MESSAGE_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_MAX_SIZE_INBOUND_MESSAGE_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_MAX_SIZE_INBOUND_MESSAGE_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_MAX_SIZE_INBOUND_MESSAGE_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +---------------------------------------------------+
 | | Register 0x000001c8 MAX_SIZE_OUTBOUND_MESSAGE_REG |
 | +---------------------------------------------------+
 | | bit  15:0 R/W  MAX_SIZE_OUTBOUND_MESSAGE          |
 | +---------------------------------------------------+
 | 
 | void   cpup34k_field_MAX_SIZE_OUTBOUND_MESSAGE_set       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | UINT32 cpup34k_field_MAX_SIZE_OUTBOUND_MESSAGE_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void   cpup34k_field_range_MAX_SIZE_OUTBOUND_MESSAGE_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit, UINT32 value )
 | UINT32 cpup34k_field_range_MAX_SIZE_OUTBOUND_MESSAGE_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 | 
 | +-----------------------------+
 | | Register 0x000001d0 DIAG_ID |
 | +-----------------------------+
 | | bit  31:0 R  DIAG_ID        |
 | +-----------------------------+
 | 
 | UINT32 cpup34k_field_DIAG_ID_get       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | UINT32 cpup34k_field_range_DIAG_ID_get ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 start_bit, UINT32 stop_bit )
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------*/
#ifndef _CPUP34K_IO_H
#define _CPUP34K_IO_H

#include "pmc_handle.h"
#include "cpup34k_regs.h"
#include "cpup34k_io_inline.h"


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------.
 | TSB access functions
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions do not need to be accessed directly,... just included here for completeness.
 +---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void                 cpup34k_buffer_init               ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | void                 cpup34k_buffer_flush              ( cpup34k_buffer_t *b_ptr )
 | UINT32               cpup34k_reg_read                  ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg )
 | void                 cpup34k_reg_write                 ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 value )
 | void                 cpup34k_field_set                 ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 unused_mask, UINT32 ofs, UINT32 value )
 | void                 cpup34k_action_on_write_field_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 ofs, UINT32 value )
 | void                 cpup34k_burst_read                ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | void                 cpup34k_burst_write               ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 len, UINT32 *value )
 | PMC_POLL_RETURN_TYPE cpup34k_poll                      ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mem_type, UINT32 reg, UINT32 mask, UINT32 value, PMC_POLL_COMPARISON_TYPE cmp, UINT32 max_count, UINT32 *num_failed_polls, UINT32 delay_between_polls_in_microseconds )
 +--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------------------------------------------.
 | Functions to Access Registers
 +--------------------------------------------------------------------------------------------------------------------------------------------------------+
 | These functions can be used if for some reason the field getters & setters are not appropriate.
 +--------------------------------------------------------------------------------------------------------------------------------------------------------+
 | void   cpup34k_reg_SOFT_RESET_EN_write                         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_SOFT_RESET_EN_field_set                     ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_SOFT_RESET_EN_read                          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_BOOT_CTRL_write                             ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_BOOT_CTRL_field_set                         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_BOOT_CTRL_read                              ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_BOOT_ADR_write                              ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_BOOT_ADR_field_set                          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_BOOT_ADR_read                               ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_BOOT_STAT_write                             ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_BOOT_STAT_field_set                         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_BOOT_STAT_read                              ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_BOOT_INFO_write                             ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_BOOT_INFO_field_set                         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_BOOT_INFO_read                              ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_APP_USER_0_write                            ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_APP_USER_0_field_set                        ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_APP_USER_0_read                             ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_APP_USER_1_write                            ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_APP_USER_1_field_set                        ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_APP_USER_1_read                             ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_write          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_field_set      ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_BASE_ADD_INBOUND_MAILBOX_REG_read           ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_write         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_field_set     ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_BASE_ADD_OUTBOUND_MAILBOX_REG_read          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_write              ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_field_set          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_SIZE_INBOUND_MAILBOX_REG_read               ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_write             ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_field_set         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_SIZE_OUTBOUND_MAILBOX_REG_read              ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_write      ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_field_set  ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_MAX_MESSAGES_INBOUND_MAILBOX_REG_read       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_write     ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_field_set ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_read      ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_write          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_field_set      ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_MAX_SIZE_INBOUND_MESSAGE_REG_read           ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_write         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_field_set     ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_MAX_SIZE_OUTBOUND_MESSAGE_REG_read          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_SOFT_RESET_BITS_write                       ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_SOFT_RESET_BITS_field_set                   ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_SOFT_RESET_BITS_read                        ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_APP_SCRATCH_0_write                         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_APP_SCRATCH_0_field_set                     ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_APP_SCRATCH_0_read                          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_APP_SCRATCH_1_write                         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_APP_SCRATCH_1_field_set                     ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_APP_SCRATCH_1_read                          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_APP_SCRATCH_2_write                         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_APP_SCRATCH_2_field_set                     ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_APP_SCRATCH_2_read                          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_APP_SCRATCH_3_write                         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_APP_SCRATCH_3_field_set                     ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_APP_SCRATCH_3_read                          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_APP_SCRATCH_4_write                         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_APP_SCRATCH_4_field_set                     ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_APP_SCRATCH_4_read                          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_APP_SCRATCH_5_write                         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_APP_SCRATCH_5_field_set                     ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_APP_SCRATCH_5_read                          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_APP_SCRATCH_6_write                         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_APP_SCRATCH_6_field_set                     ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_APP_SCRATCH_6_read                          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | void   cpup34k_reg_APP_SCRATCH_7_write                         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 value )
 | void   cpup34k_reg_APP_SCRATCH_7_field_set                     ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr, UINT32 mask, UINT32 ofs, UINT32 value )
 | UINT32 cpup34k_reg_APP_SCRATCH_7_read                          ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32 cpup34k_reg_BOOT_CAUSE_REG_read                         ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32 cpup34k_reg_DEVICE_ID_read                              ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 | UINT32 cpup34k_reg_DIAG_ID_read                                ( cpup34k_buffer_t *b_ptr, pmc_handle_t *h_ptr )
 | 
 +-------------------------------------------------------------------------------------------------------------------------------------------------------*/
#endif /* _CPUP34K_IO_H */
