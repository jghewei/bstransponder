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
 *     Contains all register offset and register bit definitions for the
 *     cpup34k block
 *****************************************************************************/
#ifndef _CPUP34K_REGS_H
#define _CPUP34K_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_CPUP34K_REG_SOFT_RESET_BITS                   0x00000000
#define PMC_CPUP34K_REG_SOFT_RESET_EN                     0x00000004
#define PMC_CPUP34K_REG_BOOT_CTRL                         0x00000054
#define PMC_CPUP34K_REG_BOOT_ADR                          0x00000058
#define PMC_CPUP34K_REG_BOOT_STAT                         0x0000005c
#define PMC_CPUP34K_REG_BOOT_INFO                         0x00000060
#define PMC_CPUP34K_REG_BOOT_CAUSE_REG                    0x00000068
#define PMC_CPUP34K_REG_DEVICE_ID                         0x00000180
#define PMC_CPUP34K_REG_APP_SCRATCH_0                     0x00000184
#define PMC_CPUP34K_REG_APP_SCRATCH_1                     0x00000188
#define PMC_CPUP34K_REG_APP_SCRATCH_2                     0x0000018c
#define PMC_CPUP34K_REG_APP_SCRATCH_3                     0x00000190
#define PMC_CPUP34K_REG_APP_SCRATCH_4                     0x00000194
#define PMC_CPUP34K_REG_APP_SCRATCH_5                     0x00000198
#define PMC_CPUP34K_REG_APP_SCRATCH_6                     0x0000019c
#define PMC_CPUP34K_REG_APP_SCRATCH_7                     0x000001a0
#define PMC_CPUP34K_REG_APP_USER_0                        0x000001a4
#define PMC_CPUP34K_REG_APP_USER_1                        0x000001a8
#define PMC_CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG      0x000001ac
#define PMC_CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG     0x000001b0
#define PMC_CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG          0x000001b4
#define PMC_CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG         0x000001b8
#define PMC_CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG  0x000001bc
#define PMC_CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG 0x000001c0
#define PMC_CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG      0x000001c4
#define PMC_CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG     0x000001c8
#define PMC_CPUP34K_REG_DIAG_ID                           0x000001d0

/*-------------------------------------.
 | Register 0x00000000 SOFT_RESET_BITS |
 +-------------------------------------+
 | bit  30 R/W  MIPS_SW_RST            |
 | bit  18 R/W  NIC_SW_RST             |
 | bit  0  R/W  MIPS_SW_RST_CLEAR      |
 +------------------------------------*/
#define PMC_CPUP34K_REG_SOFT_RESET_BITS_UNUSED_MASK           0x000007fe
#define CPUP34K_REG_SOFT_RESET_BITS_BIT_MIPS_SW_RST_MSK       0x40000000
#define CPUP34K_REG_SOFT_RESET_BITS_BIT_MIPS_SW_RST_OFF       30
#define CPUP34K_REG_SOFT_RESET_BITS_BIT_NIC_SW_RST_MSK        0x00040000
#define CPUP34K_REG_SOFT_RESET_BITS_BIT_NIC_SW_RST_OFF        18
#define CPUP34K_REG_SOFT_RESET_BITS_BIT_MIPS_SW_RST_CLEAR_MSK 0x00000001
#define CPUP34K_REG_SOFT_RESET_BITS_BIT_MIPS_SW_RST_CLEAR_OFF 0

/*---------------------------------------.
 | Register 0x00000004 SOFT_RESET_EN     |
 +---------------------------------------+
 | bit  30 R/W  MIPS_SW_RESET_EN         |
 | bit  18 R/W  NIC_SW_RST_EN            |
 | bit  0  R/W  MIPS_SW_RST_AUTOCLEAR_EN |
 +--------------------------------------*/
#define PMC_CPUP34K_REG_SOFT_RESET_EN_UNUSED_MASK                  0x000000fe
#define CPUP34K_REG_SOFT_RESET_EN_BIT_MIPS_SW_RESET_EN_MSK         0x40000000
#define CPUP34K_REG_SOFT_RESET_EN_BIT_MIPS_SW_RESET_EN_OFF         30
#define CPUP34K_REG_SOFT_RESET_EN_BIT_NIC_SW_RST_EN_MSK            0x00040000
#define CPUP34K_REG_SOFT_RESET_EN_BIT_NIC_SW_RST_EN_OFF            18
#define CPUP34K_REG_SOFT_RESET_EN_BIT_MIPS_SW_RST_AUTOCLEAR_EN_MSK 0x00000001
#define CPUP34K_REG_SOFT_RESET_EN_BIT_MIPS_SW_RST_AUTOCLEAR_EN_OFF 0

/*-------------------------------.
 | Register 0x00000054 BOOT_CTRL |
 +-------------------------------+
 | bit  0 R/W  BOOT_ENABLE       |
 +------------------------------*/
#define PMC_CPUP34K_REG_BOOT_CTRL_UNUSED_MASK     0xfffffffe
#define CPUP34K_REG_BOOT_CTRL_BIT_BOOT_ENABLE_MSK 0x00000001
#define CPUP34K_REG_BOOT_CTRL_BIT_BOOT_ENABLE_OFF 0

/*------------------------------.
 | Register 0x00000058 BOOT_ADR |
 +------------------------------+
 | bit  31:0 R/W  BOOT_JMP_ADDR |
 +-----------------------------*/
#define PMC_CPUP34K_REG_BOOT_ADR_UNUSED_MASK       0x00000000
#define CPUP34K_REG_BOOT_ADR_BIT_BOOT_JMP_ADDR_MSK 0xffffffff
#define CPUP34K_REG_BOOT_ADR_BIT_BOOT_JMP_ADDR_OFF 0

/*-------------------------------.
 | Register 0x0000005c BOOT_STAT |
 +-------------------------------+
 | bit  1 R/W  BOOT_FAIL         |
 | bit  0 R/W  BOOT_DONE         |
 +------------------------------*/
#define PMC_CPUP34K_REG_BOOT_STAT_UNUSED_MASK   0xfffffffc
#define CPUP34K_REG_BOOT_STAT_BIT_BOOT_FAIL_MSK 0x00000002
#define CPUP34K_REG_BOOT_STAT_BIT_BOOT_FAIL_OFF 1
#define CPUP34K_REG_BOOT_STAT_BIT_BOOT_DONE_MSK 0x00000001
#define CPUP34K_REG_BOOT_STAT_BIT_BOOT_DONE_OFF 0

/*-------------------------------.
 | Register 0x00000060 BOOT_INFO |
 +-------------------------------+
 | bit  15:0 R/W  BOOT_INFO      |
 +------------------------------*/
#define PMC_CPUP34K_REG_BOOT_INFO_UNUSED_MASK   0xffff0000
#define CPUP34K_REG_BOOT_INFO_BIT_BOOT_INFO_MSK 0x0000ffff
#define CPUP34K_REG_BOOT_INFO_BIT_BOOT_INFO_OFF 0

/*------------------------------------.
 | Register 0x00000068 BOOT_CAUSE_REG |
 +------------------------------------+
 | bit  3 R  CAUSE_MIPS_SELF_RST      |
 | bit  2 R  CAUSE_WDOG0_TIMEOUT      |
 | bit  1 R  CAUSE_WDOG1_TIMEOUT      |
 | bit  0 R  CAUSE_MIPS_SW_RST        |
 +-----------------------------------*/
#define PMC_CPUP34K_REG_BOOT_CAUSE_REG_UNUSED_MASK             0x7ffffff0
#define CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_MIPS_SELF_RST_MSK 0x00000008
#define CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_MIPS_SELF_RST_OFF 3
#define CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_WDOG0_TIMEOUT_MSK 0x00000004
#define CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_WDOG0_TIMEOUT_OFF 2
#define CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_WDOG1_TIMEOUT_MSK 0x00000002
#define CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_WDOG1_TIMEOUT_OFF 1
#define CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_MIPS_SW_RST_MSK   0x00000001
#define CPUP34K_REG_BOOT_CAUSE_REG_BIT_CAUSE_MIPS_SW_RST_OFF   0

/*-------------------------------.
 | Register 0x00000180 DEVICE_ID |
 +-------------------------------+
 | bit  31:28 R  REVISION_CODE   |
 | bit  27:12 R  DEVICE_ID       |
 | bit  11:0  R  PMC_ID          |
 +------------------------------*/
#define PMC_CPUP34K_REG_DEVICE_ID_UNUSED_MASK       0x00000000
#define CPUP34K_REG_DEVICE_ID_BIT_REVISION_CODE_MSK 0xf0000000
#define CPUP34K_REG_DEVICE_ID_BIT_REVISION_CODE_OFF 28
#define CPUP34K_REG_DEVICE_ID_BIT_DEVICE_ID_MSK     0x0ffff000
#define CPUP34K_REG_DEVICE_ID_BIT_DEVICE_ID_OFF     12
#define CPUP34K_REG_DEVICE_ID_BIT_PMC_ID_MSK        0x00000fff
#define CPUP34K_REG_DEVICE_ID_BIT_PMC_ID_OFF        0

/*-----------------------------------.
 | Register 0x00000184 APP_SCRATCH_0 |
 +-----------------------------------+
 | bit  31:0 R/W  APP_SCRATCH0       |
 +----------------------------------*/
#define PMC_CPUP34K_REG_APP_SCRATCH_0_UNUSED_MASK      0x00000000
#define CPUP34K_REG_APP_SCRATCH_0_BIT_APP_SCRATCH0_MSK 0xffffffff
#define CPUP34K_REG_APP_SCRATCH_0_BIT_APP_SCRATCH0_OFF 0

/*-----------------------------------.
 | Register 0x00000188 APP_SCRATCH_1 |
 +-----------------------------------+
 | bit  31:0 R/W  APP_SCRATCH1       |
 +----------------------------------*/
#define PMC_CPUP34K_REG_APP_SCRATCH_1_UNUSED_MASK      0x00000000
#define CPUP34K_REG_APP_SCRATCH_1_BIT_APP_SCRATCH1_MSK 0xffffffff
#define CPUP34K_REG_APP_SCRATCH_1_BIT_APP_SCRATCH1_OFF 0

/*-----------------------------------.
 | Register 0x0000018c APP_SCRATCH_2 |
 +-----------------------------------+
 | bit  31:0 R/W  APP_SCRATCH2       |
 +----------------------------------*/
#define PMC_CPUP34K_REG_APP_SCRATCH_2_UNUSED_MASK      0x00000000
#define CPUP34K_REG_APP_SCRATCH_2_BIT_APP_SCRATCH2_MSK 0xffffffff
#define CPUP34K_REG_APP_SCRATCH_2_BIT_APP_SCRATCH2_OFF 0

/*-----------------------------------.
 | Register 0x00000190 APP_SCRATCH_3 |
 +-----------------------------------+
 | bit  31:0 R/W  APP_SCRATCH3       |
 +----------------------------------*/
#define PMC_CPUP34K_REG_APP_SCRATCH_3_UNUSED_MASK      0x00000000
#define CPUP34K_REG_APP_SCRATCH_3_BIT_APP_SCRATCH3_MSK 0xffffffff
#define CPUP34K_REG_APP_SCRATCH_3_BIT_APP_SCRATCH3_OFF 0

/*-----------------------------------.
 | Register 0x00000194 APP_SCRATCH_4 |
 +-----------------------------------+
 | bit  31:0 R/W  APP_SCRATCH4       |
 +----------------------------------*/
#define PMC_CPUP34K_REG_APP_SCRATCH_4_UNUSED_MASK      0x00000000
#define CPUP34K_REG_APP_SCRATCH_4_BIT_APP_SCRATCH4_MSK 0xffffffff
#define CPUP34K_REG_APP_SCRATCH_4_BIT_APP_SCRATCH4_OFF 0

/*-----------------------------------.
 | Register 0x00000198 APP_SCRATCH_5 |
 +-----------------------------------+
 | bit  31:0 R/W  APP_SCRATCH5       |
 +----------------------------------*/
#define PMC_CPUP34K_REG_APP_SCRATCH_5_UNUSED_MASK      0x00000000
#define CPUP34K_REG_APP_SCRATCH_5_BIT_APP_SCRATCH5_MSK 0xffffffff
#define CPUP34K_REG_APP_SCRATCH_5_BIT_APP_SCRATCH5_OFF 0

/*-----------------------------------.
 | Register 0x0000019c APP_SCRATCH_6 |
 +-----------------------------------+
 | bit  31:0 R/W  APP_SCRATCH6       |
 +----------------------------------*/
#define PMC_CPUP34K_REG_APP_SCRATCH_6_UNUSED_MASK      0x00000000
#define CPUP34K_REG_APP_SCRATCH_6_BIT_APP_SCRATCH6_MSK 0xffffffff
#define CPUP34K_REG_APP_SCRATCH_6_BIT_APP_SCRATCH6_OFF 0

/*-----------------------------------.
 | Register 0x000001a0 APP_SCRATCH_7 |
 +-----------------------------------+
 | bit  31:0 R/W  APP_SCRATCH7       |
 +----------------------------------*/
#define PMC_CPUP34K_REG_APP_SCRATCH_7_UNUSED_MASK      0x00000000
#define CPUP34K_REG_APP_SCRATCH_7_BIT_APP_SCRATCH7_MSK 0xffffffff
#define CPUP34K_REG_APP_SCRATCH_7_BIT_APP_SCRATCH7_OFF 0

/*--------------------------------.
 | Register 0x000001a4 APP_USER_0 |
 +--------------------------------+
 | bit  31:0 R/W  APP_USER0       |
 +-------------------------------*/
#define PMC_CPUP34K_REG_APP_USER_0_UNUSED_MASK   0x00000000
#define CPUP34K_REG_APP_USER_0_BIT_APP_USER0_MSK 0xffffffff
#define CPUP34K_REG_APP_USER_0_BIT_APP_USER0_OFF 0

/*--------------------------------.
 | Register 0x000001a8 APP_USER_1 |
 +--------------------------------+
 | bit  31:0 R/W  APP_USER1       |
 +-------------------------------*/
#define PMC_CPUP34K_REG_APP_USER_1_UNUSED_MASK   0x00000000
#define CPUP34K_REG_APP_USER_1_BIT_APP_USER1_MSK 0xffffffff
#define CPUP34K_REG_APP_USER_1_BIT_APP_USER1_OFF 0

/*--------------------------------------------------.
 | Register 0x000001ac BASE_ADD_INBOUND_MAILBOX_REG |
 +--------------------------------------------------+
 | bit  31:0 R/W  BASE_ADD_INBOUND_MAILBOX          |
 +-------------------------------------------------*/
#define PMC_CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG_UNUSED_MASK                  0x00000000
#define CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG_BIT_BASE_ADD_INBOUND_MAILBOX_MSK 0xffffffff
#define CPUP34K_REG_BASE_ADD_INBOUND_MAILBOX_REG_BIT_BASE_ADD_INBOUND_MAILBOX_OFF 0

/*---------------------------------------------------.
 | Register 0x000001b0 BASE_ADD_OUTBOUND_MAILBOX_REG |
 +---------------------------------------------------+
 | bit  31:0 R/W  BASE_ADD_OUTBOUND_MAILBOX          |
 +--------------------------------------------------*/
#define PMC_CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG_UNUSED_MASK                   0x00000000
#define CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG_BIT_BASE_ADD_OUTBOUND_MAILBOX_MSK 0xffffffff
#define CPUP34K_REG_BASE_ADD_OUTBOUND_MAILBOX_REG_BIT_BASE_ADD_OUTBOUND_MAILBOX_OFF 0

/*----------------------------------------------.
 | Register 0x000001b4 SIZE_INBOUND_MAILBOX_REG |
 +----------------------------------------------+
 | bit  15:0 R/W  SIZE_INBOUND_MAILBOX          |
 +---------------------------------------------*/
#define PMC_CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG_UNUSED_MASK              0xffff0000
#define CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG_BIT_SIZE_INBOUND_MAILBOX_MSK 0x0000ffff
#define CPUP34K_REG_SIZE_INBOUND_MAILBOX_REG_BIT_SIZE_INBOUND_MAILBOX_OFF 0

/*-----------------------------------------------.
 | Register 0x000001b8 SIZE_OUTBOUND_MAILBOX_REG |
 +-----------------------------------------------+
 | bit  15:0 R/W  SIZE_OUTBOUND_MAILBOX          |
 +----------------------------------------------*/
#define PMC_CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG_UNUSED_MASK               0xffff0000
#define CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG_BIT_SIZE_OUTBOUND_MAILBOX_MSK 0x0000ffff
#define CPUP34K_REG_SIZE_OUTBOUND_MAILBOX_REG_BIT_SIZE_OUTBOUND_MAILBOX_OFF 0

/*------------------------------------------------------.
 | Register 0x000001bc MAX_MESSAGES_INBOUND_MAILBOX_REG |
 +------------------------------------------------------+
 | bit  15:0 R/W  MAX_MESSAGES_INBOUND_MAILBOX          |
 +-----------------------------------------------------*/
#define PMC_CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG_UNUSED_MASK                      0xffff0000
#define CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_INBOUND_MAILBOX_MSK 0x0000ffff
#define CPUP34K_REG_MAX_MESSAGES_INBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_INBOUND_MAILBOX_OFF 0

/*-------------------------------------------------------.
 | Register 0x000001c0 MAX_MESSAGES_OUTBOUND_MAILBOX_REG |
 +-------------------------------------------------------+
 | bit  15:0 R/W  MAX_MESSAGES_OUTBOUND_MAILBOX          |
 +------------------------------------------------------*/
#define PMC_CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_UNUSED_MASK                       0xffff0000
#define CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_OUTBOUND_MAILBOX_MSK 0x0000ffff
#define CPUP34K_REG_MAX_MESSAGES_OUTBOUND_MAILBOX_REG_BIT_MAX_MESSAGES_OUTBOUND_MAILBOX_OFF 0

/*--------------------------------------------------.
 | Register 0x000001c4 MAX_SIZE_INBOUND_MESSAGE_REG |
 +--------------------------------------------------+
 | bit  15:0 R/W  MAX_SIZE_INBOUND_MESSAGE          |
 +-------------------------------------------------*/
#define PMC_CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG_UNUSED_MASK                  0xffff0000
#define CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG_BIT_MAX_SIZE_INBOUND_MESSAGE_MSK 0x0000ffff
#define CPUP34K_REG_MAX_SIZE_INBOUND_MESSAGE_REG_BIT_MAX_SIZE_INBOUND_MESSAGE_OFF 0

/*---------------------------------------------------.
 | Register 0x000001c8 MAX_SIZE_OUTBOUND_MESSAGE_REG |
 +---------------------------------------------------+
 | bit  15:0 R/W  MAX_SIZE_OUTBOUND_MESSAGE          |
 +--------------------------------------------------*/
#define PMC_CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG_UNUSED_MASK                   0xffff0000
#define CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG_BIT_MAX_SIZE_OUTBOUND_MESSAGE_MSK 0x0000ffff
#define CPUP34K_REG_MAX_SIZE_OUTBOUND_MESSAGE_REG_BIT_MAX_SIZE_OUTBOUND_MESSAGE_OFF 0

/*-----------------------------.
 | Register 0x000001d0 DIAG_ID |
 +-----------------------------+
 | bit  31:0 R  DIAG_ID        |
 +----------------------------*/
#define PMC_CPUP34K_REG_DIAG_ID_UNUSED_MASK 0x00000000
#define CPUP34K_REG_DIAG_ID_BIT_DIAG_ID_MSK 0xffffffff
#define CPUP34K_REG_DIAG_ID_BIT_DIAG_ID_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CPUP34K_REGS_H */
