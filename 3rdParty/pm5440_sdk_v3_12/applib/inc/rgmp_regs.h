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
 *     rgmp block
 *****************************************************************************/
#ifndef _RGMP_REGS_H
#define _RGMP_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_RGMP96_CORE_REG_CHANNEL_RESET( N )                 (0x00000000 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_DPLM_INT_EN( N )                   (0x00000300 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_CSF_INT_EN( N )                    (0x0000030c + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_CM_OUT_LIMIT_INT_EN( N )           (0x00000318 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_CND_MAX_INT_EN( N )                (0x00000324 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_EN( N )        (0x00000330 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_EN( N )         (0x0000033c + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_PSERV_EARLY_EXP_INT_EN( N )        (0x00000348 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_PSERV_LATE_EXP_INT_EN( N )         (0x00000354 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_EN( N )            (0x00000360 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_DPLM_INT( N )                      (0x00000400 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_CSF_INT( N )                       (0x0000040c + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_CM_OUT_OF_LIMIT_VIOLATION_INT( N ) (0x00000418 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_CND_MAX_INT( N )                   (0x00000424 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT( N )           (0x00000430 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT( N )            (0x0000043c + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_PSERV_EARLY_EXPIRY_INT( N )        (0x00000448 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_PSERV_LATE_EXPIRY_INT( N )         (0x00000454 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT( N )               (0x00000460 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_DPLM_INT_VAL( N )                  (0x00000500 + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_CSF_INT_VAL( N )                   (0x0000050c + (N) * 0x4)
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_VAL( N )           (0x00000518 + (N) * 0x4)

/* index definitions for PMC_RGMP96_CORE_REG_CHANNEL_RESET */
#define PMC_RGMP96_CORE_REG_CHANNEL_RESET_INDEX_N_MIN    0
#define PMC_RGMP96_CORE_REG_CHANNEL_RESET_INDEX_N_MAX    95
#define PMC_RGMP96_CORE_REG_CHANNEL_RESET_INDEX_N_SIZE   96
#define PMC_RGMP96_CORE_REG_CHANNEL_RESET_INDEX_N_OFFSET 0x4

/*-------------------------------------------------.
 | Register (0x00000000 + (N) * 0x4) CHANNEL_RESET |
 +-------------------------------------------------+
 | bit  0 R/W  CHAN_RSTB                           |
 +------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_CHANNEL_RESET_UNUSED_MASK    0xfffffffe
#define RGMP96_CORE_REG_CHANNEL_RESET_BIT_CHAN_RSTB_MSK  0x00000001
#define RGMP96_CORE_REG_CHANNEL_RESET_BIT_CHAN_RSTB_OFF  0

/* index definitions for PMC_RGMP96_CORE_REG_DPLM_INT_EN */
#define PMC_RGMP96_CORE_REG_DPLM_INT_EN_INDEX_N_MIN    0
#define PMC_RGMP96_CORE_REG_DPLM_INT_EN_INDEX_N_MAX    2
#define PMC_RGMP96_CORE_REG_DPLM_INT_EN_INDEX_N_SIZE   3
#define PMC_RGMP96_CORE_REG_DPLM_INT_EN_INDEX_N_OFFSET 0x4

/*-----------------------------------------------.
 | Register (0x00000300 + (N) * 0x4) DPLM_INT_EN |
 +-----------------------------------------------+
 | bit  31:0 R/W  DPLM_E                         |
 +----------------------------------------------*/
#define PMC_RGMP96_CORE_REG_DPLM_INT_EN_UNUSED_MASK    0x00000000
#define RGMP96_CORE_REG_DPLM_INT_EN_BIT_DPLM_E_MSK     0xffffffff
#define RGMP96_CORE_REG_DPLM_INT_EN_BIT_DPLM_E_OFF     0

/* index definitions for PMC_RGMP96_CORE_REG_CSF_INT_EN */
#define PMC_RGMP96_CORE_REG_CSF_INT_EN_INDEX_N_MIN    0
#define PMC_RGMP96_CORE_REG_CSF_INT_EN_INDEX_N_MAX    2
#define PMC_RGMP96_CORE_REG_CSF_INT_EN_INDEX_N_SIZE   3
#define PMC_RGMP96_CORE_REG_CSF_INT_EN_INDEX_N_OFFSET 0x4

/*----------------------------------------------.
 | Register (0x0000030c + (N) * 0x4) CSF_INT_EN |
 +----------------------------------------------+
 | bit  31:0 R/W  CSF_E                         |
 +---------------------------------------------*/
#define PMC_RGMP96_CORE_REG_CSF_INT_EN_UNUSED_MASK    0x00000000
#define RGMP96_CORE_REG_CSF_INT_EN_BIT_CSF_E_MSK      0xffffffff
#define RGMP96_CORE_REG_CSF_INT_EN_BIT_CSF_E_OFF      0

/* index definitions for PMC_RGMP96_CORE_REG_CM_OUT_LIMIT_INT_EN */
#define PMC_RGMP96_CORE_REG_CM_OUT_LIMIT_INT_EN_INDEX_N_MIN        0
#define PMC_RGMP96_CORE_REG_CM_OUT_LIMIT_INT_EN_INDEX_N_MAX        2
#define PMC_RGMP96_CORE_REG_CM_OUT_LIMIT_INT_EN_INDEX_N_SIZE       3
#define PMC_RGMP96_CORE_REG_CM_OUT_LIMIT_INT_EN_INDEX_N_OFFSET     0x4

/*-------------------------------------------------------.
 | Register (0x00000318 + (N) * 0x4) CM_OUT_LIMIT_INT_EN |
 +-------------------------------------------------------+
 | bit  31:0 R/W  CM_OUT_LIMIT_E                         |
 +------------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_CM_OUT_LIMIT_INT_EN_UNUSED_MASK        0x00000000
#define RGMP96_CORE_REG_CM_OUT_LIMIT_INT_EN_BIT_CM_OUT_LIMIT_E_MSK 0xffffffff
#define RGMP96_CORE_REG_CM_OUT_LIMIT_INT_EN_BIT_CM_OUT_LIMIT_E_OFF 0

/* index definitions for PMC_RGMP96_CORE_REG_CND_MAX_INT_EN */
#define PMC_RGMP96_CORE_REG_CND_MAX_INT_EN_INDEX_N_MIN    0
#define PMC_RGMP96_CORE_REG_CND_MAX_INT_EN_INDEX_N_MAX    2
#define PMC_RGMP96_CORE_REG_CND_MAX_INT_EN_INDEX_N_SIZE   3
#define PMC_RGMP96_CORE_REG_CND_MAX_INT_EN_INDEX_N_OFFSET 0x4

/*--------------------------------------------------.
 | Register (0x00000324 + (N) * 0x4) CND_MAX_INT_EN |
 +--------------------------------------------------+
 | bit  31:0 R/W  CND_MAX_E                         |
 +-------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_CND_MAX_INT_EN_UNUSED_MASK    0x00000000
#define RGMP96_CORE_REG_CND_MAX_INT_EN_BIT_CND_MAX_E_MSK  0xffffffff
#define RGMP96_CORE_REG_CND_MAX_INT_EN_BIT_CND_MAX_E_OFF  0

/* index definitions for PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_EN */
#define PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_EN_INDEX_N_MIN           0
#define PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_EN_INDEX_N_MAX           2
#define PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_EN_INDEX_N_SIZE          3
#define PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_EN_INDEX_N_OFFSET        0x4

/*----------------------------------------------------------.
 | Register (0x00000330 + (N) * 0x4) JC_ERR_SYNCLOSS_INT_EN |
 +----------------------------------------------------------+
 | bit  31:0 R/W  JC_ERR_SYNCLOSS_E                         |
 +---------------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_EN_UNUSED_MASK           0x00000000
#define RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_EN_BIT_JC_ERR_SYNCLOSS_E_MSK 0xffffffff
#define RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_EN_BIT_JC_ERR_SYNCLOSS_E_OFF 0

/* index definitions for PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_EN */
#define PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_EN_INDEX_N_MIN          0
#define PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_EN_INDEX_N_MAX          2
#define PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_EN_INDEX_N_SIZE         3
#define PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_EN_INDEX_N_OFFSET       0x4

/*---------------------------------------------------------.
 | Register (0x0000033c + (N) * 0x4) CM_CND_CRC_ERR_INT_EN |
 +---------------------------------------------------------+
 | bit  31:0 R/W  CM_CND_CRC_ERR_E                         |
 +--------------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_EN_UNUSED_MASK          0x00000000
#define RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_EN_BIT_CM_CND_CRC_ERR_E_MSK 0xffffffff
#define RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_EN_BIT_CM_CND_CRC_ERR_E_OFF 0

/* index definitions for PMC_RGMP96_CORE_REG_PSERV_EARLY_EXP_INT_EN */
#define PMC_RGMP96_CORE_REG_PSERV_EARLY_EXP_INT_EN_INDEX_N_MIN       0
#define PMC_RGMP96_CORE_REG_PSERV_EARLY_EXP_INT_EN_INDEX_N_MAX       2
#define PMC_RGMP96_CORE_REG_PSERV_EARLY_EXP_INT_EN_INDEX_N_SIZE      3
#define PMC_RGMP96_CORE_REG_PSERV_EARLY_EXP_INT_EN_INDEX_N_OFFSET    0x4

/*----------------------------------------------------------.
 | Register (0x00000348 + (N) * 0x4) PSERV_EARLY_EXP_INT_EN |
 +----------------------------------------------------------+
 | bit  31:0 R/W  PSERV_EARLY_E                             |
 +---------------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_PSERV_EARLY_EXP_INT_EN_UNUSED_MASK       0x00000000
#define RGMP96_CORE_REG_PSERV_EARLY_EXP_INT_EN_BIT_PSERV_EARLY_E_MSK 0xffffffff
#define RGMP96_CORE_REG_PSERV_EARLY_EXP_INT_EN_BIT_PSERV_EARLY_E_OFF 0

/* index definitions for PMC_RGMP96_CORE_REG_PSERV_LATE_EXP_INT_EN */
#define PMC_RGMP96_CORE_REG_PSERV_LATE_EXP_INT_EN_INDEX_N_MIN      0
#define PMC_RGMP96_CORE_REG_PSERV_LATE_EXP_INT_EN_INDEX_N_MAX      2
#define PMC_RGMP96_CORE_REG_PSERV_LATE_EXP_INT_EN_INDEX_N_SIZE     3
#define PMC_RGMP96_CORE_REG_PSERV_LATE_EXP_INT_EN_INDEX_N_OFFSET   0x4

/*---------------------------------------------------------.
 | Register (0x00000354 + (N) * 0x4) PSERV_LATE_EXP_INT_EN |
 +---------------------------------------------------------+
 | bit  31:0 R/W  PSERV_LATE_E                             |
 +--------------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_PSERV_LATE_EXP_INT_EN_UNUSED_MASK      0x00000000
#define RGMP96_CORE_REG_PSERV_LATE_EXP_INT_EN_BIT_PSERV_LATE_E_MSK 0xffffffff
#define RGMP96_CORE_REG_PSERV_LATE_EXP_INT_EN_BIT_PSERV_LATE_E_OFF 0

/* index definitions for PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_EN */
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_EN_INDEX_N_MIN       0
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_EN_INDEX_N_MAX       2
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_EN_INDEX_N_SIZE      3
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_EN_INDEX_N_OFFSET    0x4

/*------------------------------------------------------.
 | Register (0x00000360 + (N) * 0x4) SSF_SF_RCVD_INT_EN |
 +------------------------------------------------------+
 | bit  31:0 R/W  SSF_SF_RCVD_E                         |
 +-----------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_EN_UNUSED_MASK       0x00000000
#define RGMP96_CORE_REG_SSF_SF_RCVD_INT_EN_BIT_SSF_SF_RCVD_E_MSK 0xffffffff
#define RGMP96_CORE_REG_SSF_SF_RCVD_INT_EN_BIT_SSF_SF_RCVD_E_OFF 0

/* index definitions for PMC_RGMP96_CORE_REG_DPLM_INT */
#define PMC_RGMP96_CORE_REG_DPLM_INT_INDEX_N_MIN    0
#define PMC_RGMP96_CORE_REG_DPLM_INT_INDEX_N_MAX    2
#define PMC_RGMP96_CORE_REG_DPLM_INT_INDEX_N_SIZE   3
#define PMC_RGMP96_CORE_REG_DPLM_INT_INDEX_N_OFFSET 0x4

/*--------------------------------------------.
 | Register (0x00000400 + (N) * 0x4) DPLM_INT |
 +--------------------------------------------+
 | bit  31:0 R/W  DPLM_I                      |
 +-------------------------------------------*/
#define PMC_RGMP96_CORE_REG_DPLM_INT_UNUSED_MASK    0x00000000
#define RGMP96_CORE_REG_DPLM_INT_BIT_DPLM_I_MSK     0xffffffff
#define RGMP96_CORE_REG_DPLM_INT_BIT_DPLM_I_OFF     0

/* index definitions for PMC_RGMP96_CORE_REG_CSF_INT */
#define PMC_RGMP96_CORE_REG_CSF_INT_INDEX_N_MIN    0
#define PMC_RGMP96_CORE_REG_CSF_INT_INDEX_N_MAX    2
#define PMC_RGMP96_CORE_REG_CSF_INT_INDEX_N_SIZE   3
#define PMC_RGMP96_CORE_REG_CSF_INT_INDEX_N_OFFSET 0x4

/*-------------------------------------------.
 | Register (0x0000040c + (N) * 0x4) CSF_INT |
 +-------------------------------------------+
 | bit  31:0 R/W  CSF_I                      |
 +------------------------------------------*/
#define PMC_RGMP96_CORE_REG_CSF_INT_UNUSED_MASK    0x00000000
#define RGMP96_CORE_REG_CSF_INT_BIT_CSF_I_MSK      0xffffffff
#define RGMP96_CORE_REG_CSF_INT_BIT_CSF_I_OFF      0

/* index definitions for PMC_RGMP96_CORE_REG_CM_OUT_OF_LIMIT_VIOLATION_INT */
#define PMC_RGMP96_CORE_REG_CM_OUT_OF_LIMIT_VIOLATION_INT_INDEX_N_MIN        0
#define PMC_RGMP96_CORE_REG_CM_OUT_OF_LIMIT_VIOLATION_INT_INDEX_N_MAX        2
#define PMC_RGMP96_CORE_REG_CM_OUT_OF_LIMIT_VIOLATION_INT_INDEX_N_SIZE       3
#define PMC_RGMP96_CORE_REG_CM_OUT_OF_LIMIT_VIOLATION_INT_INDEX_N_OFFSET     0x4

/*-----------------------------------------------------------------.
 | Register (0x00000418 + (N) * 0x4) CM_OUT_OF_LIMIT_VIOLATION_INT |
 +-----------------------------------------------------------------+
 | bit  31:0 R/W  CM_OUT_LIMIT_I                                   |
 +----------------------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_CM_OUT_OF_LIMIT_VIOLATION_INT_UNUSED_MASK        0x00000000
#define RGMP96_CORE_REG_CM_OUT_OF_LIMIT_VIOLATION_INT_BIT_CM_OUT_LIMIT_I_MSK 0xffffffff
#define RGMP96_CORE_REG_CM_OUT_OF_LIMIT_VIOLATION_INT_BIT_CM_OUT_LIMIT_I_OFF 0

/* index definitions for PMC_RGMP96_CORE_REG_CND_MAX_INT */
#define PMC_RGMP96_CORE_REG_CND_MAX_INT_INDEX_N_MIN    0
#define PMC_RGMP96_CORE_REG_CND_MAX_INT_INDEX_N_MAX    2
#define PMC_RGMP96_CORE_REG_CND_MAX_INT_INDEX_N_SIZE   3
#define PMC_RGMP96_CORE_REG_CND_MAX_INT_INDEX_N_OFFSET 0x4

/*-----------------------------------------------.
 | Register (0x00000424 + (N) * 0x4) CND_MAX_INT |
 +-----------------------------------------------+
 | bit  31:0 R/W  CND_MAX_I                      |
 +----------------------------------------------*/
#define PMC_RGMP96_CORE_REG_CND_MAX_INT_UNUSED_MASK    0x00000000
#define RGMP96_CORE_REG_CND_MAX_INT_BIT_CND_MAX_I_MSK  0xffffffff
#define RGMP96_CORE_REG_CND_MAX_INT_BIT_CND_MAX_I_OFF  0

/* index definitions for PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT */
#define PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_INDEX_N_MIN           0
#define PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_INDEX_N_MAX           2
#define PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_INDEX_N_SIZE          3
#define PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_INDEX_N_OFFSET        0x4

/*-------------------------------------------------------.
 | Register (0x00000430 + (N) * 0x4) JC_ERR_SYNCLOSS_INT |
 +-------------------------------------------------------+
 | bit  31:0 R/W  JC_ERR_SYNCLOSS_I                      |
 +------------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_UNUSED_MASK           0x00000000
#define RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_BIT_JC_ERR_SYNCLOSS_I_MSK 0xffffffff
#define RGMP96_CORE_REG_JC_ERR_SYNCLOSS_INT_BIT_JC_ERR_SYNCLOSS_I_OFF 0

/* index definitions for PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT */
#define PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_INDEX_N_MIN          0
#define PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_INDEX_N_MAX          2
#define PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_INDEX_N_SIZE         3
#define PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_INDEX_N_OFFSET       0x4

/*------------------------------------------------------.
 | Register (0x0000043c + (N) * 0x4) CM_CND_CRC_ERR_INT |
 +------------------------------------------------------+
 | bit  31:0 R/W  CM_CND_CRC_ERR_I                      |
 +-----------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_UNUSED_MASK          0x00000000
#define RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_BIT_CM_CND_CRC_ERR_I_MSK 0xffffffff
#define RGMP96_CORE_REG_CM_CND_CRC_ERR_INT_BIT_CM_CND_CRC_ERR_I_OFF 0

/* index definitions for PMC_RGMP96_CORE_REG_PSERV_EARLY_EXPIRY_INT */
#define PMC_RGMP96_CORE_REG_PSERV_EARLY_EXPIRY_INT_INDEX_N_MIN       0
#define PMC_RGMP96_CORE_REG_PSERV_EARLY_EXPIRY_INT_INDEX_N_MAX       2
#define PMC_RGMP96_CORE_REG_PSERV_EARLY_EXPIRY_INT_INDEX_N_SIZE      3
#define PMC_RGMP96_CORE_REG_PSERV_EARLY_EXPIRY_INT_INDEX_N_OFFSET    0x4

/*----------------------------------------------------------.
 | Register (0x00000448 + (N) * 0x4) PSERV_EARLY_EXPIRY_INT |
 +----------------------------------------------------------+
 | bit  31:0 R/W  PSERV_EARLY_I                             |
 +---------------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_PSERV_EARLY_EXPIRY_INT_UNUSED_MASK       0x00000000
#define RGMP96_CORE_REG_PSERV_EARLY_EXPIRY_INT_BIT_PSERV_EARLY_I_MSK 0xffffffff
#define RGMP96_CORE_REG_PSERV_EARLY_EXPIRY_INT_BIT_PSERV_EARLY_I_OFF 0

/* index definitions for PMC_RGMP96_CORE_REG_PSERV_LATE_EXPIRY_INT */
#define PMC_RGMP96_CORE_REG_PSERV_LATE_EXPIRY_INT_INDEX_N_MIN      0
#define PMC_RGMP96_CORE_REG_PSERV_LATE_EXPIRY_INT_INDEX_N_MAX      2
#define PMC_RGMP96_CORE_REG_PSERV_LATE_EXPIRY_INT_INDEX_N_SIZE     3
#define PMC_RGMP96_CORE_REG_PSERV_LATE_EXPIRY_INT_INDEX_N_OFFSET   0x4

/*---------------------------------------------------------.
 | Register (0x00000454 + (N) * 0x4) PSERV_LATE_EXPIRY_INT |
 +---------------------------------------------------------+
 | bit  31:0 R/W  PSERV_LATE_I                             |
 +--------------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_PSERV_LATE_EXPIRY_INT_UNUSED_MASK      0x00000000
#define RGMP96_CORE_REG_PSERV_LATE_EXPIRY_INT_BIT_PSERV_LATE_I_MSK 0xffffffff
#define RGMP96_CORE_REG_PSERV_LATE_EXPIRY_INT_BIT_PSERV_LATE_I_OFF 0

/* index definitions for PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT */
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_INDEX_N_MIN       0
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_INDEX_N_MAX       2
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_INDEX_N_SIZE      3
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_INDEX_N_OFFSET    0x4

/*---------------------------------------------------.
 | Register (0x00000460 + (N) * 0x4) SSF_SF_RCVD_INT |
 +---------------------------------------------------+
 | bit  31:0 R/W  SSF_SF_RCVD_I                      |
 +--------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_UNUSED_MASK       0x00000000
#define RGMP96_CORE_REG_SSF_SF_RCVD_INT_BIT_SSF_SF_RCVD_I_MSK 0xffffffff
#define RGMP96_CORE_REG_SSF_SF_RCVD_INT_BIT_SSF_SF_RCVD_I_OFF 0

/* index definitions for PMC_RGMP96_CORE_REG_DPLM_INT_VAL */
#define PMC_RGMP96_CORE_REG_DPLM_INT_VAL_INDEX_N_MIN    0
#define PMC_RGMP96_CORE_REG_DPLM_INT_VAL_INDEX_N_MAX    2
#define PMC_RGMP96_CORE_REG_DPLM_INT_VAL_INDEX_N_SIZE   3
#define PMC_RGMP96_CORE_REG_DPLM_INT_VAL_INDEX_N_OFFSET 0x4

/*------------------------------------------------.
 | Register (0x00000500 + (N) * 0x4) DPLM_INT_VAL |
 +------------------------------------------------+
 | bit  31:0 R  DPLM_V                            |
 +-----------------------------------------------*/
#define PMC_RGMP96_CORE_REG_DPLM_INT_VAL_UNUSED_MASK    0x00000000
#define RGMP96_CORE_REG_DPLM_INT_VAL_BIT_DPLM_V_MSK     0xffffffff
#define RGMP96_CORE_REG_DPLM_INT_VAL_BIT_DPLM_V_OFF     0

/* index definitions for PMC_RGMP96_CORE_REG_CSF_INT_VAL */
#define PMC_RGMP96_CORE_REG_CSF_INT_VAL_INDEX_N_MIN    0
#define PMC_RGMP96_CORE_REG_CSF_INT_VAL_INDEX_N_MAX    2
#define PMC_RGMP96_CORE_REG_CSF_INT_VAL_INDEX_N_SIZE   3
#define PMC_RGMP96_CORE_REG_CSF_INT_VAL_INDEX_N_OFFSET 0x4

/*-----------------------------------------------.
 | Register (0x0000050c + (N) * 0x4) CSF_INT_VAL |
 +-----------------------------------------------+
 | bit  31:0 R  CSF_V                            |
 +----------------------------------------------*/
#define PMC_RGMP96_CORE_REG_CSF_INT_VAL_UNUSED_MASK    0x00000000
#define RGMP96_CORE_REG_CSF_INT_VAL_BIT_CSF_V_MSK      0xffffffff
#define RGMP96_CORE_REG_CSF_INT_VAL_BIT_CSF_V_OFF      0

/* index definitions for PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_VAL */
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_VAL_INDEX_N_MIN       0
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_VAL_INDEX_N_MAX       2
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_VAL_INDEX_N_SIZE      3
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_VAL_INDEX_N_OFFSET    0x4

/*-------------------------------------------------------.
 | Register (0x00000518 + (N) * 0x4) SSF_SF_RCVD_INT_VAL |
 +-------------------------------------------------------+
 | bit  31:0 R  SSF_SF_RCVD_V                            |
 +------------------------------------------------------*/
#define PMC_RGMP96_CORE_REG_SSF_SF_RCVD_INT_VAL_UNUSED_MASK       0x00000000
#define RGMP96_CORE_REG_SSF_SF_RCVD_INT_VAL_BIT_SSF_SF_RCVD_V_MSK 0xffffffff
#define RGMP96_CORE_REG_SSF_SF_RCVD_INT_VAL_BIT_SSF_SF_RCVD_V_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _RGMP_REGS_H */
