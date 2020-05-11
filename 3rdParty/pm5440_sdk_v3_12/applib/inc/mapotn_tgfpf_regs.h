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
 *     mapotn_tgfpf block
 *****************************************************************************/
#ifndef _MAPOTN_TGFPF_REGS_H
#define _MAPOTN_TGFPF_REGS_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*--------------------.
 | register addresses |
 +-------------------*/
#define PMC_TGFPF96_CORE_REG_COMMON_CFG                         0x000a0000
#define PMC_TGFPF96_CORE_REG_PMON_CTRL                          0x000a0004
#define PMC_TGFPF96_CORE_REG_PMON_STATUS                        0x000a0008
#define PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME( N )                (0x000a0010 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_TX_CHAN_RST( N )                   (0x000a0080 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_PROTOCOL_ERR_INT_EN( N )           (0x000a0200 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_E( N )        (0x000a0210 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EN( N )         (0x000a0220 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EN( N )          (0x000a0230 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EN( N )        (0x000a0240 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EN( N )       (0x000a0250 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_E( N )     (0x000a0260 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT( N )    (0x000a0290 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EVENT( N )      (0x000a02a0 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EVENT( N )       (0x000a02b0 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EVENT( N )     (0x000a02c0 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EVENT( N )    (0x000a02d0 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_EVENT( N ) (0x000a02e0 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_MISC_INT_E                         0x000a0318
#define PMC_TGFPF96_CORE_REG_MISC_INT_EVENT                     0x000a0328
#define PMC_TGFPF96_CORE_REG_CFG_MEM_MUTEX                      0x000a0380
#define PMC_TGFPF96_CORE_REG_MGMT_FRM_MUTEX                     0x000a03a0
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_REQUEST( N )           (0x000a0400 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_REQUEST( N )            (0x000a0410 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_CHAN_CFG( N )                      (0x000a0800 + (N) * 0x4)
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0( N )              (0x000a1000 + (N) * 0x20)
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1( N )              (0x000a1004 + (N) * 0x20)
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2( N )              (0x000a1008 + (N) * 0x20)
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG( N )              (0x000a100c + (N) * 0x20)
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM( N,M )             (0x000a1c00 + (N) * 0x4 + (M) * 0x80)
#define PMC_TGFPF96_CORE_REG_EXT_HDR_RAM( N,M )                 (0x000a2000 + (N) * 0x4 + (M) * 0x40)
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM( N,M )              (0x000a4000 + (N) * 0x4 + (M) * 0x80)
#define PMC_TGFPF96_CORE_REG_PMON_0( N )                        (0x000a8000 + (N) * 0x40)
#define PMC_TGFPF96_CORE_REG_PMON_1( N )                        (0x000a8004 + (N) * 0x40)
#define PMC_TGFPF96_CORE_REG_PMON_2( N )                        (0x000a8008 + (N) * 0x40)
#define PMC_TGFPF96_CORE_REG_PMON_3( N )                        (0x000a800c + (N) * 0x40)
#define PMC_TGFPF96_CORE_REG_PMON_4( N )                        (0x000a8010 + (N) * 0x40)
#define PMC_TGFPF96_CORE_REG_PMON_5( N )                        (0x000a8014 + (N) * 0x40)
#define PMC_TGFPF96_CORE_REG_PMON_6( N )                        (0x000a8018 + (N) * 0x40)
#define PMC_TGFPF96_CORE_REG_PMON_7( N )                        (0x000a801c + (N) * 0x40)
#define PMC_TGFPF96_CORE_REG_PMON_8( N )                        (0x000a8020 + (N) * 0x40)

/*--------------------------------.
 | Register 0x000a0000 COMMON_CFG |
 +--------------------------------+
 | bit  16   R/W  ORD_SET_EN      |
 | bit  15:8 R/W  ORD_SET_UPI     |
 | bit  0    R/W  GFPF_MAP_EN     |
 +-------------------------------*/
#define PMC_TGFPF96_CORE_REG_COMMON_CFG_UNUSED_MASK     0xfcfc00fe
#define TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_EN_MSK  0x00010000
#define TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_EN_OFF  16
#define TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_UPI_MSK 0x0000ff00
#define TGFPF96_CORE_REG_COMMON_CFG_BIT_ORD_SET_UPI_OFF 8
#define TGFPF96_CORE_REG_COMMON_CFG_BIT_GFPF_MAP_EN_MSK 0x00000001
#define TGFPF96_CORE_REG_COMMON_CFG_BIT_GFPF_MAP_EN_OFF 0

/*-------------------------------.
 | Register 0x000a0004 PMON_CTRL |
 +-------------------------------+
 | bit  0 R/W  PMON_UPDATE       |
 +------------------------------*/
#define PMC_TGFPF96_CORE_REG_PMON_CTRL_UNUSED_MASK     0x7ffffffe
#define TGFPF96_CORE_REG_PMON_CTRL_BIT_PMON_UPDATE_MSK 0x00000001
#define TGFPF96_CORE_REG_PMON_CTRL_BIT_PMON_UPDATE_OFF 0

/*---------------------------------.
 | Register 0x000a0008 PMON_STATUS |
 +---------------------------------+
 | bit  0 R  PMON_TIP              |
 +--------------------------------*/
#define PMC_TGFPF96_CORE_REG_PMON_STATUS_UNUSED_MASK  0xfffffffe
#define TGFPF96_CORE_REG_PMON_STATUS_BIT_PMON_TIP_MSK 0x00000001
#define TGFPF96_CORE_REG_PMON_STATUS_BIT_PMON_TIP_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME */
#define PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME_INDEX_N_MIN    0
#define PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME_INDEX_N_MAX    3
#define PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME_INDEX_N_SIZE   4
#define PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME_INDEX_N_OFFSET 0x4

/*--------------------------------------------------.
 | Register (0x000a0010 + (N) * 0x4) PAUSE_FRM_TIME |
 +--------------------------------------------------+
 | bit  15:0 R/W  PAUSE_TIME                        |
 +-------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PAUSE_FRM_TIME_UNUSED_MASK    0xffff0000
#define TGFPF96_CORE_REG_PAUSE_FRM_TIME_BIT_PAUSE_TIME_MSK 0x0000ffff
#define TGFPF96_CORE_REG_PAUSE_FRM_TIME_BIT_PAUSE_TIME_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_TX_CHAN_RST */
#define PMC_TGFPF96_CORE_REG_TX_CHAN_RST_INDEX_N_MIN       0
#define PMC_TGFPF96_CORE_REG_TX_CHAN_RST_INDEX_N_MAX       95
#define PMC_TGFPF96_CORE_REG_TX_CHAN_RST_INDEX_N_SIZE      96
#define PMC_TGFPF96_CORE_REG_TX_CHAN_RST_INDEX_N_OFFSET    0x4

/*-----------------------------------------------.
 | Register (0x000a0080 + (N) * 0x4) TX_CHAN_RST |
 +-----------------------------------------------+
 | bit  0 R/W  TX_CHAN_RESET                     |
 +----------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_TX_CHAN_RST_UNUSED_MASK       0xfffffffe
#define TGFPF96_CORE_REG_TX_CHAN_RST_BIT_TX_CHAN_RESET_MSK 0x00000001
#define TGFPF96_CORE_REG_TX_CHAN_RST_BIT_TX_CHAN_RESET_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PROTOCOL_ERR_INT_EN */
#define PMC_TGFPF96_CORE_REG_PROTOCOL_ERR_INT_EN_INDEX_N_MIN            0
#define PMC_TGFPF96_CORE_REG_PROTOCOL_ERR_INT_EN_INDEX_N_MAX            2
#define PMC_TGFPF96_CORE_REG_PROTOCOL_ERR_INT_EN_INDEX_N_SIZE           3
#define PMC_TGFPF96_CORE_REG_PROTOCOL_ERR_INT_EN_INDEX_N_OFFSET         0x4

/*-------------------------------------------------------.
 | Register (0x000a0200 + (N) * 0x4) PROTOCOL_ERR_INT_EN |
 +-------------------------------------------------------+
 | bit  31:0 R/W  PROTOCOL_ERR_INT_E                     |
 +------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PROTOCOL_ERR_INT_EN_UNUSED_MASK            0x00000000
#define TGFPF96_CORE_REG_PROTOCOL_ERR_INT_EN_BIT_PROTOCOL_ERR_INT_E_MSK 0xffffffff
#define TGFPF96_CORE_REG_PROTOCOL_ERR_INT_EN_BIT_PROTOCOL_ERR_INT_E_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_E */
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_E_INDEX_N_MIN       0
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_E_INDEX_N_MAX       2
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_E_INDEX_N_SIZE      3
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_E_INDEX_N_OFFSET    0x4

/*----------------------------------------------------------.
 | Register (0x000a0210 + (N) * 0x4) FRAME_LENGTH_ERR_INT_E |
 +----------------------------------------------------------+
 | bit  31:0 R/W  LEN_ERR_INT_E                             |
 +---------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_E_UNUSED_MASK       0x00000000
#define TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_E_BIT_LEN_ERR_INT_E_MSK 0xffffffff
#define TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_E_BIT_LEN_ERR_INT_E_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EN */
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EN_INDEX_N_MIN              0
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EN_INDEX_N_MAX              2
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EN_INDEX_N_SIZE             3
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EN_INDEX_N_OFFSET           0x4

/*---------------------------------------------------------.
 | Register (0x000a0220 + (N) * 0x4) PAUSE_FRAME_TX_INT_EN |
 +---------------------------------------------------------+
 | bit  31:0 R/W  PAUSE_FRAME_TX_INT_E                     |
 +--------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EN_UNUSED_MASK              0x00000000
#define TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EN_BIT_PAUSE_FRAME_TX_INT_E_MSK 0xffffffff
#define TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EN_BIT_PAUSE_FRAME_TX_INT_E_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EN */
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EN_INDEX_N_MIN             0
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EN_INDEX_N_MAX             2
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EN_INDEX_N_SIZE            3
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EN_INDEX_N_OFFSET          0x4

/*--------------------------------------------------------.
 | Register (0x000a0230 + (N) * 0x4) MGMT_FRAME_TX_INT_EN |
 +--------------------------------------------------------+
 | bit  31:0 R/W  MGMT_FRAME_TX_INT_E                     |
 +-------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EN_UNUSED_MASK             0x00000000
#define TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EN_BIT_MGMT_FRAME_TX_INT_E_MSK 0xffffffff
#define TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EN_BIT_MGMT_FRAME_TX_INT_E_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EN */
#define PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EN_INDEX_N_MIN               0
#define PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EN_INDEX_N_MAX               2
#define PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EN_INDEX_N_SIZE              3
#define PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EN_INDEX_N_OFFSET            0x4

/*----------------------------------------------------------.
 | Register (0x000a0240 + (N) * 0x4) OVERSIZE_FRM_RX_INT_EN |
 +----------------------------------------------------------+
 | bit  31:0 R/W  OVERSIZE_FRM_RX_INT_E                     |
 +---------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EN_UNUSED_MASK               0x00000000
#define TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EN_BIT_OVERSIZE_FRM_RX_INT_E_MSK 0xffffffff
#define TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EN_BIT_OVERSIZE_FRM_RX_INT_E_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EN */
#define PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EN_INDEX_N_MIN                0
#define PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EN_INDEX_N_MAX                2
#define PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EN_INDEX_N_SIZE               3
#define PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EN_INDEX_N_OFFSET             0x4

/*-----------------------------------------------------------.
 | Register (0x000a0250 + (N) * 0x4) UNDERSIZE_FRM_RX_INT_EN |
 +-----------------------------------------------------------+
 | bit  31:0 R/W  UNDERSIZE_FRM_RX_INT_E                     |
 +----------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EN_UNUSED_MASK                0x00000000
#define TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EN_BIT_UNDERSIZE_FRM_RX_INT_E_MSK 0xffffffff
#define TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EN_BIT_UNDERSIZE_FRM_RX_INT_E_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_E */
#define PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_E_INDEX_N_MIN               0
#define PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_E_INDEX_N_MAX               2
#define PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_E_INDEX_N_SIZE              3
#define PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_E_INDEX_N_OFFSET            0x4

/*-------------------------------------------------------------.
 | Register (0x000a0260 + (N) * 0x4) ORD_SET_FRM_LEN_ERR_INT_E |
 +-------------------------------------------------------------+
 | bit  31:0 R/W  ORD_SET_LEN_ERR_INT_E                        |
 +------------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_E_UNUSED_MASK               0x00000000
#define TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_E_BIT_ORD_SET_LEN_ERR_INT_E_MSK 0xffffffff
#define TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_E_BIT_ORD_SET_LEN_ERR_INT_E_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT */
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT_INDEX_N_MIN       0
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT_INDEX_N_MAX       2
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT_INDEX_N_SIZE      3
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT_INDEX_N_OFFSET    0x4

/*--------------------------------------------------------------.
 | Register (0x000a0290 + (N) * 0x4) FRAME_LENGTH_ERR_INT_EVENT |
 +--------------------------------------------------------------+
 | bit  31:0 R/W  LEN_ERR_INT_I                                 |
 +-------------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT_UNUSED_MASK       0x00000000
#define TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT_BIT_LEN_ERR_INT_I_MSK 0xffffffff
#define TGFPF96_CORE_REG_FRAME_LENGTH_ERR_INT_EVENT_BIT_LEN_ERR_INT_I_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EVENT */
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EVENT_INDEX_N_MIN              0
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EVENT_INDEX_N_MAX              2
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EVENT_INDEX_N_SIZE             3
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EVENT_INDEX_N_OFFSET           0x4

/*------------------------------------------------------------.
 | Register (0x000a02a0 + (N) * 0x4) PAUSE_FRAME_TX_INT_EVENT |
 +------------------------------------------------------------+
 | bit  31:0 R/W  PAUSE_FRAME_TX_INT_I                        |
 +-----------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EVENT_UNUSED_MASK              0x00000000
#define TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EVENT_BIT_PAUSE_FRAME_TX_INT_I_MSK 0xffffffff
#define TGFPF96_CORE_REG_PAUSE_FRAME_TX_INT_EVENT_BIT_PAUSE_FRAME_TX_INT_I_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EVENT */
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EVENT_INDEX_N_MIN             0
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EVENT_INDEX_N_MAX             2
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EVENT_INDEX_N_SIZE            3
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EVENT_INDEX_N_OFFSET          0x4

/*-----------------------------------------------------------.
 | Register (0x000a02b0 + (N) * 0x4) MGMT_FRAME_TX_INT_EVENT |
 +-----------------------------------------------------------+
 | bit  31:0 R/W  MGMT_FRAME_TX_INT_I                        |
 +----------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EVENT_UNUSED_MASK             0x00000000
#define TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EVENT_BIT_MGMT_FRAME_TX_INT_I_MSK 0xffffffff
#define TGFPF96_CORE_REG_MGMT_FRAME_TX_INT_EVENT_BIT_MGMT_FRAME_TX_INT_I_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EVENT */
#define PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EVENT_INDEX_N_MIN               0
#define PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EVENT_INDEX_N_MAX               2
#define PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EVENT_INDEX_N_SIZE              3
#define PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EVENT_INDEX_N_OFFSET            0x4

/*-------------------------------------------------------------.
 | Register (0x000a02c0 + (N) * 0x4) OVERSIZE_FRM_RX_INT_EVENT |
 +-------------------------------------------------------------+
 | bit  31:0 R/W  OVERSIZE_FRM_RX_INT_I                        |
 +------------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EVENT_UNUSED_MASK               0x00000000
#define TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EVENT_BIT_OVERSIZE_FRM_RX_INT_I_MSK 0xffffffff
#define TGFPF96_CORE_REG_OVERSIZE_FRM_RX_INT_EVENT_BIT_OVERSIZE_FRM_RX_INT_I_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EVENT */
#define PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EVENT_INDEX_N_MIN                0
#define PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EVENT_INDEX_N_MAX                2
#define PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EVENT_INDEX_N_SIZE               3
#define PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EVENT_INDEX_N_OFFSET             0x4

/*--------------------------------------------------------------.
 | Register (0x000a02d0 + (N) * 0x4) UNDERSIZE_FRM_RX_INT_EVENT |
 +--------------------------------------------------------------+
 | bit  31:0 R/W  UNDERSIZE_FRM_RX_INT_I                        |
 +-------------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EVENT_UNUSED_MASK                0x00000000
#define TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EVENT_BIT_UNDERSIZE_FRM_RX_INT_I_MSK 0xffffffff
#define TGFPF96_CORE_REG_UNDERSIZE_FRM_RX_INT_EVENT_BIT_UNDERSIZE_FRM_RX_INT_I_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_EVENT */
#define PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_EVENT_INDEX_N_MIN               0
#define PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_EVENT_INDEX_N_MAX               2
#define PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_EVENT_INDEX_N_SIZE              3
#define PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_EVENT_INDEX_N_OFFSET            0x4

/*-----------------------------------------------------------------.
 | Register (0x000a02e0 + (N) * 0x4) ORD_SET_FRM_LEN_ERR_INT_EVENT |
 +-----------------------------------------------------------------+
 | bit  31:0 R/W  ORD_SET_LEN_ERR_INT_I                            |
 +----------------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_EVENT_UNUSED_MASK               0x00000000
#define TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_EVENT_BIT_ORD_SET_LEN_ERR_INT_I_MSK 0xffffffff
#define TGFPF96_CORE_REG_ORD_SET_FRM_LEN_ERR_INT_EVENT_BIT_ORD_SET_LEN_ERR_INT_I_OFF 0

/*---------------------------------------.
 | Register 0x000a0318 MISC_INT_E        |
 +---------------------------------------+
 | bit  1 R/W  PMON_TRANSFER_END_INT_E   |
 | bit  0 R/W  PMON_TRANSFER_START_INT_E |
 +--------------------------------------*/
#define PMC_TGFPF96_CORE_REG_MISC_INT_E_UNUSED_MASK                   0xfffffffc
#define TGFPF96_CORE_REG_MISC_INT_E_BIT_PMON_TRANSFER_END_INT_E_MSK   0x00000002
#define TGFPF96_CORE_REG_MISC_INT_E_BIT_PMON_TRANSFER_END_INT_E_OFF   1
#define TGFPF96_CORE_REG_MISC_INT_E_BIT_PMON_TRANSFER_START_INT_E_MSK 0x00000001
#define TGFPF96_CORE_REG_MISC_INT_E_BIT_PMON_TRANSFER_START_INT_E_OFF 0

/*---------------------------------------.
 | Register 0x000a0328 MISC_INT_EVENT    |
 +---------------------------------------+
 | bit  1 R/W  PMON_TRANSFER_END_INT_I   |
 | bit  0 R/W  PMON_TRANSFER_START_INT_I |
 +--------------------------------------*/
#define PMC_TGFPF96_CORE_REG_MISC_INT_EVENT_UNUSED_MASK                   0xfffffffc
#define TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_END_INT_I_MSK   0x00000002
#define TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_END_INT_I_OFF   1
#define TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_START_INT_I_MSK 0x00000001
#define TGFPF96_CORE_REG_MISC_INT_EVENT_BIT_PMON_TRANSFER_START_INT_I_OFF 0

/*-----------------------------------.
 | Register 0x000a0380 CFG_MEM_MUTEX |
 +-----------------------------------+
 | bit  0 R/W  MTX_CONFIG_MEM        |
 +----------------------------------*/
#define PMC_TGFPF96_CORE_REG_CFG_MEM_MUTEX_UNUSED_MASK        0xfffffffe
#define TGFPF96_CORE_REG_CFG_MEM_MUTEX_BIT_MTX_CONFIG_MEM_MSK 0x00000001
#define TGFPF96_CORE_REG_CFG_MEM_MUTEX_BIT_MTX_CONFIG_MEM_OFF 0

/*------------------------------------.
 | Register 0x000a03a0 MGMT_FRM_MUTEX |
 +------------------------------------+
 | bit  0 R/W  MTX_MGMT_FRAME_MEM     |
 +-----------------------------------*/
#define PMC_TGFPF96_CORE_REG_MGMT_FRM_MUTEX_UNUSED_MASK            0xfffffffe
#define TGFPF96_CORE_REG_MGMT_FRM_MUTEX_BIT_MTX_MGMT_FRAME_MEM_MSK 0x00000001
#define TGFPF96_CORE_REG_MGMT_FRM_MUTEX_BIT_MTX_MGMT_FRAME_MEM_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PAUSE_FRAME_REQUEST */
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_REQUEST_INDEX_N_MIN         0
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_REQUEST_INDEX_N_MAX         2
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_REQUEST_INDEX_N_SIZE        3
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_REQUEST_INDEX_N_OFFSET      0x4

/*-------------------------------------------------------.
 | Register (0x000a0400 + (N) * 0x4) PAUSE_FRAME_REQUEST |
 +-------------------------------------------------------+
 | bit  31:0 R/W  PAUSE_FRAME_REQ                        |
 +------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_REQUEST_UNUSED_MASK         0x00000000
#define TGFPF96_CORE_REG_PAUSE_FRAME_REQUEST_BIT_PAUSE_FRAME_REQ_MSK 0xffffffff
#define TGFPF96_CORE_REG_PAUSE_FRAME_REQUEST_BIT_PAUSE_FRAME_REQ_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_MGMT_FRAME_REQUEST */
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_REQUEST_INDEX_N_MIN        0
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_REQUEST_INDEX_N_MAX        2
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_REQUEST_INDEX_N_SIZE       3
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_REQUEST_INDEX_N_OFFSET     0x4

/*------------------------------------------------------.
 | Register (0x000a0410 + (N) * 0x4) MGMT_FRAME_REQUEST |
 +------------------------------------------------------+
 | bit  31:0 R/W  MGMT_FRAME_REQ                        |
 +-----------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_REQUEST_UNUSED_MASK        0x00000000
#define TGFPF96_CORE_REG_MGMT_FRAME_REQUEST_BIT_MGMT_FRAME_REQ_MSK 0xffffffff
#define TGFPF96_CORE_REG_MGMT_FRAME_REQUEST_BIT_MGMT_FRAME_REQ_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_CHAN_CFG */
#define PMC_TGFPF96_CORE_REG_CHAN_CFG_INDEX_N_MIN                 0
#define PMC_TGFPF96_CORE_REG_CHAN_CFG_INDEX_N_MAX                 95
#define PMC_TGFPF96_CORE_REG_CHAN_CFG_INDEX_N_SIZE                96
#define PMC_TGFPF96_CORE_REG_CHAN_CFG_INDEX_N_OFFSET              0x4

/*--------------------------------------------.
 | Register (0x000a0800 + (N) * 0x4) CHAN_CFG |
 +--------------------------------------------+
 | bit  23:16 R/W  PREFETCH_FIFO_THRESH       |
 | bit  15    R/W  CPB_MAP                    |
 | bit  14    R/W  AUTO_FLOW_CTRL_EN          |
 | bit  13:12 R/W  PAUSE_FRM_TEMPLATE_XON     |
 | bit  11:10 R/W  PAUSE_FRM_TEMPLATE_XOFF    |
 | bit  9     R/W  INSERT_FRM_PRIORITY        |
 | bit  8     R/W  MGMT_FRM_PRIORITY          |
 | bit  7:0   R/W  MGMT_FRM_LEN               |
 +-------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_CHAN_CFG_UNUSED_MASK                 0xff000000
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_PREFETCH_FIFO_THRESH_MSK    0x00ff0000
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_PREFETCH_FIFO_THRESH_OFF    16
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_CPB_MAP_MSK                 0x00008000
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_CPB_MAP_OFF                 15
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_AUTO_FLOW_CTRL_EN_MSK       0x00004000
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_AUTO_FLOW_CTRL_EN_OFF       14
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XON_MSK  0x00003000
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XON_OFF  12
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XOFF_MSK 0x00000c00
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_PAUSE_FRM_TEMPLATE_XOFF_OFF 10
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_INSERT_FRM_PRIORITY_MSK     0x00000200
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_INSERT_FRM_PRIORITY_OFF     9
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_PRIORITY_MSK       0x00000100
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_PRIORITY_OFF       8
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_LEN_MSK            0x000000ff
#define TGFPF96_CORE_REG_CHAN_CFG_BIT_MGMT_FRM_LEN_OFF            0

/* index definitions for PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0 */
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_INDEX_N_MIN      0
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_INDEX_N_MAX      95
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_INDEX_N_SIZE     96
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_INDEX_N_OFFSET   0x20

/*-----------------------------------------------------.
 | Register (0x000a1000 + (N) * 0x20) TX_CHAN_CFG_RAM0 |
 +-----------------------------------------------------+
 | bit  31    R/W  EXT_HDR_EN                          |
 | bit  30:24 R/W  EXT_HDR_LEN                         |
 | bit  22:20 R/W  TYPE_HDR_PTI                        |
 | bit  19:16 R/W  TYPE_HDR_EXI                        |
 | bit  15:8  R/W  TYPE_HDR_UPI                        |
 | bit  7:6   R/W  FCS_INS                             |
 | bit  5:4   R/W  FCS_INS_POS                         |
 | bit  3     R/W  UPI_SEL                             |
 +----------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_UNUSED_MASK      0x00800007
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_EN_MSK   0x80000000
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_EN_OFF   31
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_LEN_MSK  0x7f000000
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_EXT_HDR_LEN_OFF  24
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_PTI_MSK 0x00700000
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_PTI_OFF 20
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_EXI_MSK 0x000f0000
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_EXI_OFF 16
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_UPI_MSK 0x0000ff00
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_TYPE_HDR_UPI_OFF 8
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_MSK      0x000000c0
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_OFF      6
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_POS_MSK  0x00000030
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_FCS_INS_POS_OFF  4
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_UPI_SEL_MSK      0x00000008
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM0_BIT_UPI_SEL_OFF      3

/* index definitions for PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1 */
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_INDEX_N_MIN        0
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_INDEX_N_MAX        95
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_INDEX_N_SIZE       96
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_INDEX_N_OFFSET     0x20

/*-----------------------------------------------------.
 | Register (0x000a1004 + (N) * 0x20) TX_CHAN_CFG_RAM1 |
 +-----------------------------------------------------+
 | bit  7 R/W  SCRMBL                                  |
 | bit  6 R/W  PFCS_REV_OBYTE                          |
 | bit  5 R/W  PFCS_REV_OBIT                           |
 | bit  4 R/W  PFCS_REV_IBIT                           |
 | bit  3 R/W  PFCS_CRPT_INV                           |
 | bit  2 R/W  PFCS_INV                                |
 | bit  1 R/W  PFCS_INIT                               |
 | bit  0 R/W  DC_BALANCE                              |
 +----------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_UNUSED_MASK        0xfffffe00
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_SCRMBL_MSK         0x00000080
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_SCRMBL_OFF         7
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_OBYTE_MSK 0x00000040
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_OBYTE_OFF 6
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_OBIT_MSK  0x00000020
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_OBIT_OFF  5
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_IBIT_MSK  0x00000010
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_REV_IBIT_OFF  4
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_CRPT_INV_MSK  0x00000008
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_CRPT_INV_OFF  3
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_INV_MSK       0x00000004
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_INV_OFF       2
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_INIT_MSK      0x00000002
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_PFCS_INIT_OFF      1
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_DC_BALANCE_MSK     0x00000001
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM1_BIT_DC_BALANCE_OFF     0

/* index definitions for PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2 */
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_INDEX_N_MIN                 0
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_INDEX_N_MAX                 95
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_INDEX_N_SIZE                96
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_INDEX_N_OFFSET              0x20

/*-----------------------------------------------------.
 | Register (0x000a1008 + (N) * 0x20) TX_CHAN_CFG_RAM2 |
 +-----------------------------------------------------+
 | bit  15:8 R/W  PACKER_FIFO_LOW_THRESH               |
 | bit  7:0  R/W  PACKER_FIFO_HIGH_THRESH              |
 +----------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_UNUSED_MASK                 0xffff0000
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_LOW_THRESH_MSK  0x0000ff00
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_LOW_THRESH_OFF  8
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_HIGH_THRESH_MSK 0x000000ff
#define TGFPF96_CORE_REG_TX_CHAN_CFG_RAM2_BIT_PACKER_FIFO_HIGH_THRESH_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG */
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG_INDEX_N_MIN        0
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG_INDEX_N_MAX        95
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG_INDEX_N_SIZE       96
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG_INDEX_N_OFFSET     0x20

/*-----------------------------------------------------.
 | Register (0x000a100c + (N) * 0x20) FRAME_LENGTH_CFG |
 +-----------------------------------------------------+
 | bit  31:16 R/W  MAX_FRM_LENGTH                      |
 | bit  15:0  R/W  MIN_FRM_LENGTH                      |
 +----------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_FRAME_LENGTH_CFG_UNUSED_MASK        0x00000000
#define TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MAX_FRM_LENGTH_MSK 0xffff0000
#define TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MAX_FRM_LENGTH_OFF 16
#define TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MIN_FRM_LENGTH_MSK 0x0000ffff
#define TGFPF96_CORE_REG_FRAME_LENGTH_CFG_BIT_MIN_FRM_LENGTH_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM */
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM_INDEX_N_MIN         0
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM_INDEX_N_MAX         17
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM_INDEX_N_SIZE        18
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM_INDEX_N_OFFSET      0x4
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM_INDEX_M_MIN         0
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM_INDEX_M_MAX         3
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM_INDEX_M_SIZE        4
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM_INDEX_M_OFFSET      0x80

/*----------------------------------------------------------------.
 | Register (0x000a1c00 + (N) * 0x4 + (M) * 0x80) PAUSE_FRAME_RAM |
 +----------------------------------------------------------------+
 | bit  31:0 R/W  PAUSE_FRAME_MEM                                 |
 +---------------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PAUSE_FRAME_RAM_UNUSED_MASK         0x00000000
#define TGFPF96_CORE_REG_PAUSE_FRAME_RAM_BIT_PAUSE_FRAME_MEM_MSK 0xffffffff
#define TGFPF96_CORE_REG_PAUSE_FRAME_RAM_BIT_PAUSE_FRAME_MEM_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_EXT_HDR_RAM */
#define PMC_TGFPF96_CORE_REG_EXT_HDR_RAM_INDEX_N_MIN     0
#define PMC_TGFPF96_CORE_REG_EXT_HDR_RAM_INDEX_N_MAX     15
#define PMC_TGFPF96_CORE_REG_EXT_HDR_RAM_INDEX_N_SIZE    16
#define PMC_TGFPF96_CORE_REG_EXT_HDR_RAM_INDEX_N_OFFSET  0x4
#define PMC_TGFPF96_CORE_REG_EXT_HDR_RAM_INDEX_M_MIN     0
#define PMC_TGFPF96_CORE_REG_EXT_HDR_RAM_INDEX_M_MAX     95
#define PMC_TGFPF96_CORE_REG_EXT_HDR_RAM_INDEX_M_SIZE    96
#define PMC_TGFPF96_CORE_REG_EXT_HDR_RAM_INDEX_M_OFFSET  0x40

/*------------------------------------------------------------.
 | Register (0x000a2000 + (N) * 0x4 + (M) * 0x40) EXT_HDR_RAM |
 +------------------------------------------------------------+
 | bit  31:0 R/W  EXT_HDR_MEM                                 |
 +-----------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_EXT_HDR_RAM_UNUSED_MASK     0x00000000
#define TGFPF96_CORE_REG_EXT_HDR_RAM_BIT_EXT_HDR_MEM_MSK 0xffffffff
#define TGFPF96_CORE_REG_EXT_HDR_RAM_BIT_EXT_HDR_MEM_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM */
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM_INDEX_N_MIN        0
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM_INDEX_N_MAX        31
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM_INDEX_N_SIZE       32
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM_INDEX_N_OFFSET     0x4
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM_INDEX_M_MIN        0
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM_INDEX_M_MAX        95
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM_INDEX_M_SIZE       96
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM_INDEX_M_OFFSET     0x80

/*---------------------------------------------------------------.
 | Register (0x000a4000 + (N) * 0x4 + (M) * 0x80) MGMT_FRAME_RAM |
 +---------------------------------------------------------------+
 | bit  31:0 R/W  MGMT_FRAME_MEM                                 |
 +--------------------------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_MGMT_FRAME_RAM_UNUSED_MASK        0x00000000
#define TGFPF96_CORE_REG_MGMT_FRAME_RAM_BIT_MGMT_FRAME_MEM_MSK 0xffffffff
#define TGFPF96_CORE_REG_MGMT_FRAME_RAM_BIT_MGMT_FRAME_MEM_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PMON_0 */
#define PMC_TGFPF96_CORE_REG_PMON_0_INDEX_N_MIN      0
#define PMC_TGFPF96_CORE_REG_PMON_0_INDEX_N_MAX      95
#define PMC_TGFPF96_CORE_REG_PMON_0_INDEX_N_SIZE     96
#define PMC_TGFPF96_CORE_REG_PMON_0_INDEX_N_OFFSET   0x40

/*-------------------------------------------.
 | Register (0x000a8000 + (N) * 0x40) PMON_0 |
 +-------------------------------------------+
 | bit  31:0 R  TX_BYTE_CNT0                 |
 +------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PMON_0_UNUSED_MASK      0x00000000
#define TGFPF96_CORE_REG_PMON_0_BIT_TX_BYTE_CNT0_MSK 0xffffffff
#define TGFPF96_CORE_REG_PMON_0_BIT_TX_BYTE_CNT0_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PMON_1 */
#define PMC_TGFPF96_CORE_REG_PMON_1_INDEX_N_MIN      0
#define PMC_TGFPF96_CORE_REG_PMON_1_INDEX_N_MAX      95
#define PMC_TGFPF96_CORE_REG_PMON_1_INDEX_N_SIZE     96
#define PMC_TGFPF96_CORE_REG_PMON_1_INDEX_N_OFFSET   0x40

/*-------------------------------------------.
 | Register (0x000a8004 + (N) * 0x40) PMON_1 |
 +-------------------------------------------+
 | bit  7:0 R  TX_BYTE_CNT1                  |
 +------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PMON_1_UNUSED_MASK      0xffffff00
#define TGFPF96_CORE_REG_PMON_1_BIT_TX_BYTE_CNT1_MSK 0x000000ff
#define TGFPF96_CORE_REG_PMON_1_BIT_TX_BYTE_CNT1_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PMON_2 */
#define PMC_TGFPF96_CORE_REG_PMON_2_INDEX_N_MIN      0
#define PMC_TGFPF96_CORE_REG_PMON_2_INDEX_N_MAX      95
#define PMC_TGFPF96_CORE_REG_PMON_2_INDEX_N_SIZE     96
#define PMC_TGFPF96_CORE_REG_PMON_2_INDEX_N_OFFSET   0x40

/*-------------------------------------------.
 | Register (0x000a8008 + (N) * 0x40) PMON_2 |
 +-------------------------------------------+
 | bit  31:0 R  MGMT_FRM_CNT                 |
 +------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PMON_2_UNUSED_MASK      0x00000000
#define TGFPF96_CORE_REG_PMON_2_BIT_MGMT_FRM_CNT_MSK 0xffffffff
#define TGFPF96_CORE_REG_PMON_2_BIT_MGMT_FRM_CNT_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PMON_3 */
#define PMC_TGFPF96_CORE_REG_PMON_3_INDEX_N_MIN    0
#define PMC_TGFPF96_CORE_REG_PMON_3_INDEX_N_MAX    95
#define PMC_TGFPF96_CORE_REG_PMON_3_INDEX_N_SIZE   96
#define PMC_TGFPF96_CORE_REG_PMON_3_INDEX_N_OFFSET 0x40

/*-------------------------------------------.
 | Register (0x000a800c + (N) * 0x40) PMON_3 |
 +-------------------------------------------+
 | bit  31:0 R  TX_FRM_CNT                   |
 +------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PMON_3_UNUSED_MASK    0x00000000
#define TGFPF96_CORE_REG_PMON_3_BIT_TX_FRM_CNT_MSK 0xffffffff
#define TGFPF96_CORE_REG_PMON_3_BIT_TX_FRM_CNT_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PMON_4 */
#define PMC_TGFPF96_CORE_REG_PMON_4_INDEX_N_MIN       0
#define PMC_TGFPF96_CORE_REG_PMON_4_INDEX_N_MAX       95
#define PMC_TGFPF96_CORE_REG_PMON_4_INDEX_N_SIZE      96
#define PMC_TGFPF96_CORE_REG_PMON_4_INDEX_N_OFFSET    0x40

/*-------------------------------------------.
 | Register (0x000a8010 + (N) * 0x40) PMON_4 |
 +-------------------------------------------+
 | bit  31:0 R  PAUSE_FRM_CNT                |
 +------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PMON_4_UNUSED_MASK       0x00000000
#define TGFPF96_CORE_REG_PMON_4_BIT_PAUSE_FRM_CNT_MSK 0xffffffff
#define TGFPF96_CORE_REG_PMON_4_BIT_PAUSE_FRM_CNT_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PMON_5 */
#define PMC_TGFPF96_CORE_REG_PMON_5_INDEX_N_MIN      0
#define PMC_TGFPF96_CORE_REG_PMON_5_INDEX_N_MAX      95
#define PMC_TGFPF96_CORE_REG_PMON_5_INDEX_N_SIZE     96
#define PMC_TGFPF96_CORE_REG_PMON_5_INDEX_N_OFFSET   0x40

/*-------------------------------------------.
 | Register (0x000a8014 + (N) * 0x40) PMON_5 |
 +-------------------------------------------+
 | bit  31:0 R  IDLE_FRM_CNT                 |
 +------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PMON_5_UNUSED_MASK      0x00000000
#define TGFPF96_CORE_REG_PMON_5_BIT_IDLE_FRM_CNT_MSK 0xffffffff
#define TGFPF96_CORE_REG_PMON_5_BIT_IDLE_FRM_CNT_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PMON_6 */
#define PMC_TGFPF96_CORE_REG_PMON_6_INDEX_N_MIN     0
#define PMC_TGFPF96_CORE_REG_PMON_6_INDEX_N_MAX     95
#define PMC_TGFPF96_CORE_REG_PMON_6_INDEX_N_SIZE    96
#define PMC_TGFPF96_CORE_REG_PMON_6_INDEX_N_OFFSET  0x40

/*-------------------------------------------.
 | Register (0x000a8018 + (N) * 0x40) PMON_6 |
 +-------------------------------------------+
 | bit  31:0 R  ERR_FRM_CNT                  |
 +------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PMON_6_UNUSED_MASK     0x00000000
#define TGFPF96_CORE_REG_PMON_6_BIT_ERR_FRM_CNT_MSK 0xffffffff
#define TGFPF96_CORE_REG_PMON_6_BIT_ERR_FRM_CNT_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PMON_7 */
#define PMC_TGFPF96_CORE_REG_PMON_7_INDEX_N_MIN              0
#define PMC_TGFPF96_CORE_REG_PMON_7_INDEX_N_MAX              95
#define PMC_TGFPF96_CORE_REG_PMON_7_INDEX_N_SIZE             96
#define PMC_TGFPF96_CORE_REG_PMON_7_INDEX_N_OFFSET           0x40

/*-------------------------------------------.
 | Register (0x000a801c + (N) * 0x40) PMON_7 |
 +-------------------------------------------+
 | bit  31:0 R  TX_FRM_CNT_UNDERSIZE         |
 +------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PMON_7_UNUSED_MASK              0x00000000
#define TGFPF96_CORE_REG_PMON_7_BIT_TX_FRM_CNT_UNDERSIZE_MSK 0xffffffff
#define TGFPF96_CORE_REG_PMON_7_BIT_TX_FRM_CNT_UNDERSIZE_OFF 0

/* index definitions for PMC_TGFPF96_CORE_REG_PMON_8 */
#define PMC_TGFPF96_CORE_REG_PMON_8_INDEX_N_MIN             0
#define PMC_TGFPF96_CORE_REG_PMON_8_INDEX_N_MAX             95
#define PMC_TGFPF96_CORE_REG_PMON_8_INDEX_N_SIZE            96
#define PMC_TGFPF96_CORE_REG_PMON_8_INDEX_N_OFFSET          0x40

/*-------------------------------------------.
 | Register (0x000a8020 + (N) * 0x40) PMON_8 |
 +-------------------------------------------+
 | bit  31:0 R  TX_FRM_CNT_OVERSIZE          |
 +------------------------------------------*/
#define PMC_TGFPF96_CORE_REG_PMON_8_UNUSED_MASK             0x00000000
#define TGFPF96_CORE_REG_PMON_8_BIT_TX_FRM_CNT_OVERSIZE_MSK 0xffffffff
#define TGFPF96_CORE_REG_PMON_8_BIT_TX_FRM_CNT_OVERSIZE_OFF 0

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _MAPOTN_TGFPF_REGS_H */
