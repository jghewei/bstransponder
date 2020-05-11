/*******************************************************************************
*   COPYRIGHT (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
* --------------------------------------------------------------------------
*   DESCRIPTION : This header file contains internal, system wide host message
*                 definitions. This header file can not be exposed to the
*                 customers.
*
*   NOTES:
*
*******************************************************************************/
#ifndef _HOSTMSG_API_H
#define _HOSTMSG_API_H

#include "util_global.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Number of words in header message.  */
#define HOSTMSG_NUM_HEADER_WORDS                                (2)

/* Length of response ACK host message*/
#define HOSTMSG_RSP_CFG_ACK_LEN                                 (2)

/* Responses when HOSTMSG_INFO_REQ_GET_STATUS requested. */
#define HOSTMSG_CMD_REQ_INFO_GET_STATUS_FIRMWARE_ALIVE          (0xABCD)

/* Responses when HOSTMSG_INFO_REQ_FW_VERS_CHECK requested. */
#define HOSTMSG_CMD_REQ_INFO_GET_STATUS_FIRMWARE_SUPPORTED      (0)
#define HOSTMSG_CMD_REQ_INFO_GET_STATUS_FIRMWARE_NOT_SUPPORTED  (1)

/* SERDES Host Message Lengths */
#define HOSTMSG_CMD_CFG_SERDES_C8_LEN                     (0x03)
#define HOSTMSG_CMD_CFG_SERDES_T8_LEN                     (0x04)
#define HOSTMSG_CMD_CFG_SERDES_S16_LEN                    (0x04)
#define HOSTMSG_CMD_CFG_SERDES_SINGLE_S16_LEN             (0x04)
#define HOSTMSG_CMD_REQ_SERDES_GET_SINGLE_S16_LEN         (0x03)
#define HOSTMSG_CMD_CFG_SERDES_SFI51_LEN                  (0x03)
#define HOSTMSG_CMD_CFG_SERDES_D8_LEN                     (0x03)
#define HOSTMSG_CMD_CFG_SERDES_POSTED_D8_LEN              (0x03)
#define HOSTMSG_CMD_REQ_SERDES_GET_POSTED_SFI51_LEN       (0x03)
#define HOSTMSG_CMD_CFG_SERDES_POSTED_SFI51_LEN           (0x03)
#define HOSTMSG_CMD_REQ_SERDES_GET_POSTED_D8_LEN          (0x03)
#define HOSTMSG_CMD_CFG_SERDES_START_ADAPT_LEN            (0x04)
#define HOSTMSG_CMD_CFG_SERDES_POSTED_START_ADAPT_LEN     (0x03)
#define HOSTMSG_CMD_REQ_SERDES_GET_POSTED_START_ADAPT_LEN (0x04)
#define HOSTMSG_CMD_CFG_SERDES_C8_WRITE_ADAPT_LEN         (0x06)
#define HOSTMSG_CMD_CFG_SERDES_C8_READ_ADAPT_LEN          (0x03)
#define HOSTMSG_CMD_CFG_SERDES_T8_WRITE_ADAPT_LEN         (0x12)
#define HOSTMSG_CMD_CFG_SERDES_T8_READ_ADAPT_LEN          (0x03)
#define HOSTMSG_CMD_CFG_SERDES_S16_WRITE_ADAPT_LEN        (0x0F)
#define HOSTMSG_CMD_CFG_SERDES_S16_READ_ADAPT_LEN         (0x03)
#define HOSTMSG_CMD_REQ_SERDES_GET_ADAPT_STAT_LEN         (0x03)
#define HOSTMSG_CMD_CFG_SERDES_DEBUG_LEN                  (0x03)
#define HOSTMSG_CMD_REQ_SERDES_GET_DEBUG_STAT_LEN         (0x03)

/* LINEOTN Host Message Lengths */
#define HOSTMSG_CMD_CFG_LINEOTN_INIT_FEC_LEN                            (0x03)
#define HOSTMSG_CMD_CFG_LINEOTN_CONFIG_SWIZZLE_FEC_FRAME_SLIP_MON_LEN   (0x04)
#define HOSTMSG_CMD_CFG_LINEOTN_CONFIG_OTU4_CLOCK_LEN                   (0x05)
#define HOSTMSG_CMD_CFG_LINEOTN_CONFIG_LOF_MON_LEN                      (0x05)

/* PMON RCP Message length */
#define HOSTMSG_CMD_CFG_PMON_RCP_LEN (0x3)

/* ENET set client source Message length */
#define HOSTMSG_CMD_CFG_ENET_SET_ENET_CLIENT_SOURCE_LEN (0x3)

/* ENET get client source Message length */
#define HOSTMSG_CMD_CFG_ENET_GET_ENET_CLIENT_SOURCE_LEN (0x3)

/* ENET ECLASS Init Message length */
#define HOSTMSG_CMD_CFG_ENET_ECLASS_INIT_LEN (0x3)

/* ECLASS config hwired filter Message length */
#define HOSTMSG_CMD_CFG_ENET_ECLASS_CONFIG_HWIRED_FILTER_LEN (0x5)

/* ECLASS config generic filter Message length */
#define HOSTMSG_CMD_CFG_ENET_ECLASS_CONFIG_GENERIC_FILTER_LEN (0xF)

/* ENET Interrupt Monitor Config Message length */
#define HOSTMSG_CMD_CFG_ENET_INTR_MONITOR_CFG_LEN (0x3)

/* ENET Interrupt Monitor Status Get Message length */
#define HOSTMSG_CMD_CFG_ENET_INTR_MONITOR_STATUS_GET_LEN (0x3)

/* OPSA Configuration Message Lengths */
#define HOSTMSG_CMD_CFG_OPSA_MPMA_PORT_ADD_LEN       (0x1E)
#define HOSTMSG_CMD_CFG_OPSA_MPMA_PORT_DROP_LEN      (0x06)
#define HOSTMSG_CMD_CFG_OPSA_MPMO_PORT_ADD_LEN       (0x40)
#define HOSTMSG_CMD_CFG_OPSA_MPMO_PORT_DROP_LEN      (0x06)
#define HOSTMSG_CMD_CFG_OPSA_TXJC_CFC_FIFO_CLRB_LEN  (0x06)
#define HOSTMSG_CMD_CFG_OPSA_FIFO_CENT_TRIGGER_LEN   (0x05)
#define HOSTMSG_CMD_CFG_OPSA_TAU_VALUE_LEN           (0x04)
#define HOSTMSG_CMD_REQ_OPSA_GET_DEBUG_STATUS_LEN    (0x03)

/* COREOTN Configuration Message Lengths*/
#define HOSTMSG_CMD_CFG_COREOTN_SET_STS_SW_CONFIG_LEN               (0x2C)
#define HOSTMSG_CMD_CFG_COREOTN_SET_STS_SW_NULL_CONFIG_LEN          (0x04)
#define HOSTMSG_CMD_CFG_COREOTN_CONFIG_DLOOMFI_MON_LEN              (0x03)
#define HOSTMSG_CMD_CFG_COREOTN_SET_STS_SW_INIT_LEN                 (0x03)
#define HOSTMSG_CMD_CFG_COREOTN_SET_STS_SW_HO_CAL_NULL_CONFIG_LEN   (0x18)

/* CBRC COnfiguration Lengths */
#define HOSTMSG_CMD_CFG_CBRC_INTR_MONITOR_CFG_LEN (0x3)
#define HOSTMSG_CMD_CFG_CBRC_INTR_MONITOR_STATUS_GET_LEN (0x3)

/* LED Configuration Message Lengths*/
#define HOSTMSG_CMD_CFG_LED_SET_SPI_DEV_ADDR_LEN      (0x03)
#define HOSTMSG_CMD_CFG_LED_SET_MON_CTRL_LEN          (0x03)

/* Debug data Message Lengths */
#define DBG_LOG_HOSTMSG_STYPE_GET_DBG_DATA_ADDR_CMD_LEN     (0x03)
#define DBG_LOG_HOSTMSG_STYPE_GET_DBG_DATA_ADDR_RSP_LEN     (0x04)

/* STATS message subtype Message Lengths */
#define BENCH_HOSTMSG_STYPE_STATS_GET_CMD_LEN     (0x03)
#define BENCH_HOSTMSG_STYPE_STATS_GET_RSP_LEN     (0x04)
#define BENCH_HOSTMSG_STYPE_STATS_RESET_CMD_LEN     (0x03)

/* Serdes mask size */
#define HOSTMSG_C8_MASK_SIZE 8
#define HOSTMSG_T8_MASK_SIZE 40
#define HOSTMSG_SYS_S16_MASK_SIZE 16

/* Class member sizes. */
#define HOSTMSG_CLASS_MEMBER_SIZE_BYTE(type, member) sizeof(((type *)0)->member)
#define HOSTMSG_CLASS_MEMBER_SIZE_WORD(type, member) ((sizeof(((type *)0)->member))/4)
#define HOSTMSG_CLASS_MEMBER_SIZE(type, member) \
        HOSTMSG_CLASS_MEMBER_SIZE_WORD(type, member)




/*******************************************************************************
 * enums and types                                                             *
*******************************************************************************/

/*******************************************************************************
* ENUM: hostmsg_modid_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Host message module ids. They are located in first byte of first host message
*   word. For more information please look at the DIGI Firmware User Manual.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_modid
{
    MODID_PTP       = 0x01,
    MODID_SYNC_E    = 0x02,
    MODID_LINKOAM   = 0x03,
    MODID_SERDES    = 0x04,
    MODID_CMF       = 0x05,
    MODID_COREOTN   = 0x06,
    MODID_OPSA      = 0x07,
    MODID_LED       = 0x08,
    MODID_ENET      = 0x09,
    MODID_CBRC      = 0x0A,
    MODID_PMON      = 0x0B, /* PMON related messages. */
    MODID_LINEOTN   = 0x0C,
    MODID_PERFORMANCE = 0x0D,
    MODID_SIFD      = 0x11,
    MODID_INFO_DBG  = 0x80,
    MODID_DBG_LOG   = 0x81,
    MODID_BENCH     = 0x82,
    MODID_STATS     = 0x83,
    MODID_UNIT_TEST = 0x90,
    MODID_TEST      = 0xFF,

    MAX_MODID       = 0xFF  /* Change this parameter as needed */
} hostmsg_modid_t;

/*******************************************************************************
* ENUM: hostmsg_category_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Host message category ids. They are located in second byte of first host
*   message word. For more information please look at the DIGI Firmware User
*   Manual.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum hostmsg_category
{
    CATEGORY_CMD_CFG  = 0x01,
    CATEGORY_CMD_REQ  = 0x02,
    CATEGORY_RSP_AUTO = 0xF0,
    CATEGORY_RSP_CFG  = 0xF1,
    CATEGORY_RSP_REQ  = 0xF2,
    CATEGORY_RSP_NACK = 0xFF
} hostmsg_category_t;


/*******************************************************************************
* ENUM: hostmsg_nack_error_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Further information from host message NACK error codes. They are located
*   in return message of NACK host messages. 
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum hostmsg_nack_error
{

     PMCFW_ERR_HOSTMSG_RX_INVALID_MSG_TYPE      =  (0x00000100), /*RX message header contains invalid type*/
     PMCFW_ERR_HOSTMSG_RX_INVALID_MSG_SUBTYPE   =  (0x00000101), /*RX message header contains invalid subtype*/
     PMCFW_ERR_HOSTMSG_RX_INVALID_MSG_LEN       =  (0x00000102), /*RX message header contains invalid length*/
     PMCFW_ERR_HOSTMSG_RX_INVALID_MSG_CTRL      =  (0x00000103), /*RX message header contains invalid control*/
     PMCFW_ERR_HOSTMSG_RX_INVALID_MSG_MODID     =  (0x00000104), /*RX message header contains invalid module ID*/
     PMCFW_ERR_HOSTMSG_RX_INVALID_MSG_PARAMETERS = (0x00000105), /*RX message contains invalid parameters*/
     PMCFW_ERR_HOSTMSG_RX_INVALID_PRECONFIG     =  (0x00000106), /*Failed to receive host message due to prerequisite not satisfied*/
     PMCFW_ERR_HOSTMSG_RX_HARDWARE_UNAVAILABLE  =  (0x00000107), /*FW cannot access HW */
     PMCFW_ERR_HOSTMSG_OUTBOX_BUSY              =  (0x00000108), /*Outbound doorbell is set when send message to outbox*/
     PMCFW_ERR_HOSTMSG_TX_CONCAT_FAIL           =  (0x00000109), /*Failed to transmit concatenated message*/
     PMCFW_ERR_HOSTMSG_RX_CONCAT_FAIL           =  (0x0000010a), /*Failed to receive concatenated message*/
     PMCFW_ERR_HOSTMSG_NULL_PTR                 =  (0x0000010b), /*NULL pointer detected during processing host message*/
     PMCFW_ERR_HOSTMSG_HARDWARE_CFG_FAILURE     =  (0x0000010c), /*Failure while configuring hardware*/
     PMCFW_ERR_HOSTMSG_UNKNOWN_ERR_CODE         =  (0x000001ff), /*Unknown hostmsg error*/
     PMCFW_ERR_UNKNOWN_ERROR                    =  (0x00000fff) /*Unknown common error*/

} hostmsg_nack_error_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_info_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMD_REQ_INFO and CMD_CFG_INFO message subtypes. They are located in first
*   byte of second host message word. For more information please look at the
*   DIGI Firmware User Manual.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum hostmsg_cmd_info_subtype
{
    HOSTMSG_CMD_INFO_GET_STATUS_ST                 = 0x01,
    HOSTMSG_CMD_INFO_GET_VERSION_ST                = 0x02,
    HOSTMSG_CMD_INFO_FW_SHUTDOWN_ST                = 0x03,
    HOSTMSG_CMD_INFO_SPRAM_PCIE_WRITE_EN_ST        = 0x04,
    HOSTMSG_CMD_INFO_GET_SPRAM_SW_RESERVED_ADDR_ST = 0x05,
} hostmsg_cmd_info_subtype_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_pmon_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMD_REQ_PMON and CMD_CFG_PMON message subtypes. They are located in first
*   byte of second host message word. For more information please look at the
*   DIGI Firmware User Manual.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum hostmsg_cmd_pmon_subtype
{
    HOSTMSG_CMD_PMON_CFG_COLLECTION_MODE_ST      = 0x01,
    HOSTMSG_CMD_PMON_CFG_WORK_ORDER_ST           = 0x02,
    HOSTMSG_CMD_PMON_CFG_RCP_ST                  = 0x03,
    HOSTMSG_CMD_PMON_CFG_DDEG_COREOTN_SEG_ST     = 0x04,
    HOSTMSG_CMD_PMON_CFG_DDEG_PARAM_ST           = 0x05,
    HOSTMSG_CMD_PMON_CFG_DDEG_ENABLE_ST          = 0x06,
    HOSTMSG_CMD_PMON_CFG_DDEG_INT_ENABLE_ST      = 0x07,
    HOSTMSG_CMD_PMON_REQ_DDEG_INT_ST             = 0x08,
    HOSTMSG_CMD_PMON_REQ_DDEG_PARAM_ST           = 0x09,
    HOSTMSG_CMD_PMON_REQ_DDEG_ENABLE_ST          = 0x0A,
    HOSTMSG_CMD_PMON_CFG_FW_FEC_ST               = 0x0B,
    HOSTMSG_CMD_PMON_CFG_HW_DDEG_LCLK_ST         = 0x0C,
    HOSTMSG_CMD_PMON_CFG_MANUAL_TRIGGER_ST       = 0x10,
    HOSTMSG_CMD_PMON_CFG_FW_MANUAL_FEC_UPD_ST    = 0x11,
    HOSTMSG_CMD_PMON_CFG_FW_SET_RCP_LOS_ST       = 0x12,
    HOSTMSG_CMD_PMON_REQ_FW_SM_GET_STATUS_ST     = 0x20,
    HOSTMSG_CMD_PMON_REQ_DDEG_STATE_ST           = 0x21,
    HOSTMSG_CMD_PMON_CFG_PRBS_COLLECTION_MODE_ST = 0x22,
} hostmsg_cmd_pmon_subtype_t;

/**/
/*******************************************************************************
* ENUM: hostmsg_msg_ctrl_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Host message control types. Large Msg Control:basic, extend or concatenated
*   message. They are located in second byte of second host message word.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_msg_ctrl_type
{
    BASIC_MSG             = 0x00,
    CONCATENATED_1ST_MSG  = 0x01,
    CONCATENATED_MID_MSG  = 0x03,
    CONCATENATED_LST_MSG  = 0x02,
    EXTEND_MSG            = 0xF0
} hostmsg_msg_ctrl_type_t;

/*******************************************************************************
* ENUM: hostmsg_info_req_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This enum is for the REQ_INFO_Get_Status input "info_req" field.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_info_req
{
    HOSTMSG_INFO_REQ_FIRST = 0,
    HOSTMSG_INFO_REQ_GET_STATINFO = 1,
    HOSTMSG_INFO_REQ_CHECK_FWINFO = 2,
    HOSTMSG_INFO_REQ_LAST,
} hostmsg_info_req_t;

/*******************************************************************************
* ENUM: hostmsg_info_req_ver_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This enum is for the REQ_INFO_Get_Version_Info input "info_req" field.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_info_req_ver
{
    HOSTMSG_INFO_REQ_VER_FIRST = 0,
    HOSTMSG_INFO_REQ_VER_GET_REV_NUM_ONLY = 1,
    HOSTMSG_INFO_REQ_VER_GET_REV_AND_BUILD_NUM = 2,
    HOSTMSG_INFO_REQ_VER_LAST,
} hostmsg_info_req_ver_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_lineotn_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMD_CFG_LINEOTN message subtypes. They are located in first
*   byte of second host message word. For more information please look at the
*   DIGI Firmware User Manual.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_cmd_lineotn_subtype_t
{
    HOSTMSG_CMD_CFG_LINEOTN_INIT_FEC_ST                          = 0x01,
    HOSTMSG_CMD_CFG_LINEOTN_CONFIG_OTU4_CLOCK_ST                 = 0x02,
    HOSTMSG_CMD_CFG_LINEOTN_CONFIG_SWIZZLE_FEC_FRAME_SLIP_MON_ST = 0x03,
    HOSTMSG_CMD_CFG_LINEOTN_CONFIG_LOF_MON_ST                    = 0x04,
} hostmsg_cmd_lineotn_subtype_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_serdes_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMD_CFG_SERDES message subtypes. They are located in first
*   byte of second host message word. For more information please look at the
*   DIGI Firmware User Manual.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_cmd_serdes_subtype_t
{
    HOSTMSG_CMD_CFG_SERDES_C8_ST                = 0x01,
    HOSTMSG_CMD_CFG_SERDES_T8_ST                = 0x02,
    HOSTMSG_CMD_CFG_SERDES_S16_ST               = 0x03,
    HOSTMSG_CMD_CFG_SERDES_SFI51_ST             = 0x04,
    HOSTMSG_CMD_CFG_SERDES_D8_ST                = 0x05,
    HOSTMSG_CMD_CFG_SERDES_START_ADAPT_ST       = 0x06,
    HOSTMSG_CMD_CFG_SERDES_C8_WRITE_ADAPT_ST    = 0x07,
    HOSTMSG_CMD_CFG_SERDES_C8_READ_ADAPT_ST     = 0x08,
    HOSTMSG_CMD_CFG_SERDES_T8_WRITE_ADAPT_ST    = 0x09,
    HOSTMSG_CMD_CFG_SERDES_T8_READ_ADAPT_ST     = 0x0A,
    HOSTMSG_CMD_CFG_SERDES_S16_WRITE_ADAPT_ST   = 0x0B,
    HOSTMSG_CMD_CFG_SERDES_S16_READ_ADAPT_ST    = 0x0C,
    HOSTMSG_CMD_REQ_SERDES_GET_ADAPT_STAT_ST    = 0x0D,
    HOSTMSG_CMD_CFG_SERDES_SINGLE_S16_ST        = 0x0E,
    HOSTMSG_CMD_REQ_SERDES_SINGLE_S16_STATUS_ST = 0x0F,
    HOSTMSG_CMD_CFG_SERDES_POSTED_D8_ST         = 0x10,
    HOSTMSG_CMD_REQ_SERDES_POSTED_D8_STATUS_ST  = 0x11,
    HOSTMSG_CMD_CFG_SERDES_POSTED_SFI51_ST         = 0x12,
    HOSTMSG_CMD_REQ_SERDES_POSTED_SFI51_STATUS_ST  = 0x13,
    HOSTMSG_CMD_CFG_SERDES_POSTED_START_ADAPT_ST         = 0x14,
    HOSTMSG_CMD_REQ_SERDES_POSTED_START_ADAPT_STATUS_ST  = 0x15,
    HOSTMSG_CMD_CFG_SERDES_DEBUG_ST             = 0x90,
    HOSTMSG_CMD_REQ_SERDES_GET_DEBUG_STAT_ST    = 0x91,
} hostmsg_cmd_serdes_subtype_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_enet_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMD_CFG_ENET message subtypes. They are located in first
*   byte of second host message word. For more information please look at the
*   DIGI Firmware User Manual.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_cmd_enet_subtype_t
{
    HOSTMSG_CMD_CFG_ENET_SET_ENET_CLIENT_SOURCE_ST              = 0x01,
    HOSTMSG_CMD_CFG_ENET_GET_ENET_CLIENT_SOURCE_ST              = 0x02,
    HOSTMSG_CMD_CFG_ENET_ECLASS_INIT_ST                         = 0x03,
    HOSTMSG_CMD_CFG_ENET_ECLASS_CONFIG_HWIRED_FILTER_ST         = 0x04,
    HOSTMSG_CMD_CFG_ENET_ECLASS_CONFIG_GENERIC_FILTER_ST        = 0x05,
    HOSTMSG_CMD_CFG_ENET_INTR_MONITOR_CFG_ST                    = 0x06,
    HOSTMSG_CMD_CFG_ENET_INTR_MONITOR_STATUS_GET_ST             = 0x07,
    HOSTMSG_CMD_CFG_ENET_SET_PKT_FILTER_ST                      = 0x08,
    HOSTMSG_CMD_CFG_ENET_ECLASS_FILTER_CFG_ST                   = 0x09,
    HOSTMSG_CMD_CFG_ENET_LDP_PKT_FETCH_ST                       = 0x0A,
    HOSTMSG_CMD_CFG_ENET_LDP_RELEASE_ST                         = 0x0B,
    HOSTMSG_CMD_REQ_ENET_LLDP_PKT_CNTS_ST                       = 0x0C,
} hostmsg_cmd_enet_subtype_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_opsa_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMD_CFG_OPSA message subtypes. They are located in first
*   byte of second host message word. For more information please look at the
*   DIGI Firmware User Manual.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_cmd_opsa_subtype_t
{
    HOSTMSG_CMD_CFG_OPSA_MPMA_PORT_ADD_ST              = 0x01,
    HOSTMSG_CMD_CFG_OPSA_MPMA_PORT_DROP_ST             = 0x02,
    HOSTMSG_CMD_CFG_OPSA_MPMO_PORT_ADD_ST              = 0x03,
    HOSTMSG_CMD_CFG_OPSA_MPMO_PORT_DROP_ST             = 0x04,
    HOSTMSG_CMD_CFG_OPSA_TXJC_CFC_FIFO_CLRB_ST         = 0x05,
    HOSTMSG_CMD_CFG_OPSA_FIFO_CENT_TRIGGER_ST          = 0x06,
    HOSTMSG_CMD_CFG_OPSA_TAU_VALUE_ST                  = 0x07,
    HOSTMSG_CMD_CFG_OPSA_MPMO_TIMES_ST                 = 0x08,
    HOSTMSG_CMD_CFG_OPSA_MPMO_FIFO_TRIGGER_ENABLES_ST  = 0x09,
    HOSTMSG_CMD_CFG_OPSA_MPMO_CTXT_CFG                 = 0x12,
    HOSTMSG_CMD_CFG_OPSA_MPMA_CTXT_CFG                 = 0x13,
    HOSTMSG_CMD_REQ_OPSA_GET_DEBUG_STATUS_ST           = 0x91
} hostmsg_cmd_opsa_subtype_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_dbg_log_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   dbg_log message subtypes. They are located in first
*   byte of second host message word.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_cmd_dbg_log_subtype_t
{
    DBG_LOG_HOSTMSG_STYPE_SET_APP_FILTERS      = 0x01,
    DBG_LOG_HOSTMSG_STYPE_GET_APP_FILTER_INFO  = 0x02,
    DBG_LOG_HOSTMSG_STYPE_CLEAR_APP_LOG        = 0x03,
    DBG_LOG_HOSTMSG_STYPE_GET_LOG_ADDR         = 0x04,
    DBG_LOG_HOSTMSG_STYPE_GET_DBG_DATA_ADDR    = 0x05,
    DBG_LOG_HOSTMSG_STYPE_START_DBG_DATA_CAPT  = 0x06
} hostmsg_cmd_dbg_log_subtype_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_dbg_log_module_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   dbg_log message module ids.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_cmd_dbg_log_module_id_t
{
    HOSTMSG_CMD_CFG_DBG_LOG_DBG_DATA_PKT_CACHE           = 0x00,
    HOSTMSG_CMD_CFG_DBG_LOG_DBG_DATA_HOSTMSG             = 0x01,
    HOSTMSG_CMD_CFG_DBG_LOG_DBG_DATA_OPSA_MPMO           = 0x02,
    HOSTMSG_CMD_CFG_DBG_LOG_DBG_DATA_PTP                 = 0x03,
    HOSTMSG_CMD_CFG_DBG_LOG_DBG_DATA_ENET                = 0x04,
    HOSTMSG_CMD_CFG_DBG_LOG_DBG_DATA_PTP_TS_RX           = 0x05,
    HOSTMSG_CMD_CFG_DBG_LOG_DBG_DATA_PTP_TS_TX           = 0x06
} hostmsg_cmd_dbg_log_module_id_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_synce_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMD_CFG_SYNCE, CMD_REQ_SYNCE, and RSP_REQ_SYNCE message subtypes. They are
*   located in first byte of second host message word. For more information please
*   look at the DIGI Firmware User Manual.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_cmd_synce_subtype_t
{
    HOSTMSG_CMD_CFG_SYNCE_CONFIG_CH_ST  = 0x00,
    HOSTMSG_CMD_CFG_SYNCE_CTRL_CH_ST    = 0x01,
    HOSTMSG_CMD_REQ_SYNCE_GET_SSM_ST    = 0x02,
    HOSTMSG_RSP_REQ_SYNCE_GET_SSM_ST    = 0x02,
    HOSTMSG_CMD_CFG_SYNCE_TX_SSM_ST     = 0x03
} hostmsg_cmd_synce_subtype_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_coreotn_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMD_CFG_COREOTN message subtypes. They are located in first byte of second
*   host message word. For more information please look at the DIGI Firmware User
*   Manual.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_cmd_coreotn_subtype_t
{
    HOSTMSG_CMD_CFG_COREOTN_SET_STS_SW_CONFIG_ST                = 0x01,
    HOSTMSG_CMD_CFG_COREOTN_SET_STS_SW_NULL_CONFIG_ST           = 0x02,
    HOSTMSG_CMD_CFG_COREOTN_CONFIG_DLOOMFI_MON_ST               = 0x03,
    HOSTMSG_CMD_CFG_COREOTN_SET_STS_SW_INIT_ST                  = 0x04,
    HOSTMSG_CMD_CFG_COREOTN_SET_STS_SW_HO_CAL_NULL_CONFIG_ST    = 0x05,
    HOSTMSG_CMD_CFG_COREOTN_SET_STS_SW_CONFIG_ALL_ST            = 0x06,
    HOSTMSG_CMD_CFG_COREOTN_HOSTMSG_STYPE_CFG_OHP_EN_ST         = 0x07
} hostmsg_cmd_coreotn_subtype_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_cmf_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMD_REQ_CMF and CMD_CFG_CMF message subtypes. They are located in first
*   byte of second host message word. For more information please look at the
*   DIGI Firmware User Manual.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_cmd_cmf_subtype_t
{
    HOSTMSG_CMD_CMF_CFG_SEND_ONE_SHOT_CMF_ST      = 0x01,
    HOSTMSG_CMD_CMF_REQ_EXTRACT_CMF_ST            = 0x02,
    HOSTMSG_CMD_CMF_CFG_TRIGGER_OPU_CSF_ST        = 0x03,
    HOSTMSG_CMD_CMF_CFG_CLIENTTOTRUNK_CONSEQ_ACTION_TABLE_ST = 0x04,
    HOSTMSG_CMD_CMF_CFG_CLIENTTOTRUNK_PORT_ACTION_TABLE_ST = 0x05,
    HOSTMSG_CMD_CMF_CFG_CLIENTTOTRUNK_UPDATE_TABLES_ST = 0x06,
    HOSTMSG_CMD_CMF_CFG_TRUNKTOCLIENT_CONSEQ_ACTION_TABLE_ST = 0x08,
    HOSTMSG_CMD_CMF_CFG_TRUNKTOCLIENT_PORT_MAPPING_TABLE_ST = 0x09,
    HOSTMSG_CMD_CMF_CFG_TRUNKTOCLIENT_UPDATE_TABLES_ST = 0x0A,
    HOSTMSG_CMD_CMF_CFG_TRIGGER_LF_ST = 0x0B,
    HOSTMSG_CMD_CMF_CFG_TRIGGER_RF_ST = 0x0C,
    HOSTMSG_CMD_CMF_GFP_ENET_EMAC_LF_RF_ST = 0x0D,
    HOSTMSG_CMD_CMF_REQ_OPU_CSF_STAT_ST = 0x0E,
    HOSTMSG_CMD_CMF_REQ_TRUNKTOCLIENT_CONSEQ_ACTION_TABLE_ST = 0x0F,
    HOSTMSG_CMD_CMF_REQ_CLIENTTOTRUNK_CONSEQ_ACTION_TABLE_ST = 0x10,
    HOSTMSG_CMD_CMF_CFG_CLIENTTOTRUNK_SW_FORCE_ST = 0x11,
    HOSTMSG_CMD_CMF_CFG_MAC_MAC_LF_RF_FORWARDING_ST = 0x12,
    HOSTMSG_CMD_CMF_REQ_CLIENTTOTRUNK_PORT_ACTION_TABLE_ST = 0x13,
    HOSTMSG_CMD_CMF_REQ_TRUNKTOCLIENT_PORT_MAPPING_TABLE_ST = 0x14,
    HOSTMSG_CMD_CMF_CFG_FDI_INS_MAPOTN_CHNLS = 0x15,
    HOSTMSG_CMD_CMF_CFG_CBRC_FORCE_FAULT = 0x16,
    HOSTMSG_CMD_CMF_CFG_NON_STD_UPI_VALUES_ST = 0x17,
    HOSTMSG_MAX_ST_NUM
    

} hostmsg_cmd_cmf_subtype_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_ptp_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMD_REQ_PTP and CMD_CFG_PTP message subtypes. They are located in first
*   byte of second host message word. For more information please look at the
*   DIGI Firmware User Manual.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_cmd_ptp_subtype_t
{
    HOSTMSG_CMD_CFG_PTP_CONFIG_ST                           = 0x01,
    HOSTMSG_CMD_CFG_PTP_SET_IRIG_CONFIG_ST                  = 0x02,
    HOSTMSG_CMD_CFG_PTP_SET_PTP_IPV4_PARAM_ST               = 0x03, 
    HOSTMSG_CMD_CFG_PTP_SET_PTP_IPV6_PARAM_ST               = 0x04,
    HOSTMSG_CMD_REQ_PTP_GET_PORT_RX_MSG_CNTS_ST             = 0x05,
    HOSTMSG_RSP_REQ_PTP_GET_PORT_RX_MSG_CNTS_ST             = 0x05,
    HOSTMSG_CMD_REQ_PTP_GET_PATH_DELAY_ST                   = 0x06,
    HOSTMSG_RSP_REQ_PTP_GET_PATH_DELAY_ST                   = 0x06,
    HOSTMSG_CMD_CFG_PTP_SET_PORT_ASYMMETRY_ST               = 0x07, 
    HOSTMSG_CMD_CFG_PTP_SET_DEFAULT_PROPERTIES_ST           = 0x10,
    HOSTMSG_CMD_CFG_PTP_SET_TIME_PROPERTIES_ST              = 0x11,
    HOSTMSG_CMD_CFG_PTP_SET_ACCEPTABLE_MASTER_CLOCKS_ST     = 0x12,
    HOSTMSG_CMD_CFG_PTP_SET_START_INITIALIZE_ST             = 0x13,
    HOSTMSG_CMD_REQ_PTP_GET_MASTER_CLOCK_DATABASE_STATUS_ST = 0x14,
    HOSTMSG_RSP_REQ_PTP_GET_MASTER_CLOCK_DATABASE_STATUS_ST = 0x14,
    HOSTMSG_CMD_REQ_PTP_GET_MASTER_CLOCK_DATABASE_ST        = 0x15,
    HOSTMSG_RSP_REQ_PTP_GET_MASTER_CLOCK_DATABASE_ST        = 0x15,
    HOSTMSG_CMD_CFG_PTP_SET_PARENT_DATA_ST                  = 0x16,
    HOSTMSG_CMD_CFG_PTP_SET_PORT_DATA_ST                    = 0x17,
    HOSTMSG_CMD_CFG_PTP_SET_PORT_STATE_ST                   = 0x18,
    HOSTMSG_CMD_CFG_PTP_SET_PORT_EVENT_ST                   = 0x19,
    HOSTMSG_CMD_REQ_PTP_GET_PORT_STATE_ST                   = 0x1A,
    HOSTMSG_RSP_REQ_PTP_GET_PORT_STATE_ST                   = 0x1A,
    HOSTMSG_CMD_CFG_PTP_SET_PASSIVE_PORT_PARENT_ST          = 0x1B,           
    HOSTMSG_CMD_CFG_PTP_SET_UNICAST_MASTER_PARAM_ST         = 0x1C,
    HOSTMSG_CMD_CFG_PTP_SET_UNICAST_REQUEST_ST              = 0x1D,
    HOSTMSG_CMD_CFG_PTP_SET_UNICAST_CANCEL_ST               = 0x1E,
    HOSTMSG_CMD_REQ_PTP_GET_UNICAST_SLAVE_STATE_ST          = 0x1F,
    HOSTMSG_RSP_REQ_PTP_GET_UNICAST_SLAVE_STATE_ST          = 0x1F,
    HOSTMSG_CMD_CFG_PTP_SET_PASS_THRU_GENERAL_PARAM_ST      = 0x30,
    HOSTMSG_CMD_CFG_PTP_SET_PASS_THRU_PORT_PARAM_ST         = 0x31,
    HOSTMSG_CMD_CFG_PTP_SET_TC_PARAM_ST                     = 0x40,
    HOSTMSG_CMD_CFG_PTP_SET_TC_PORT_PARAM_ST                = 0x41,
    HOSTMSG_CMD_REQ_PTP_GET_STABILITY_STATUS_ST             = 0x50,
    HOSTMSG_RSP_REQ_PTP_GET_STABILITY_STATUS_ST             = 0x50,
    HOSTMSG_CMD_REQ_PTP_SET_PTP_HOSTMSG_DCSU_AJUST_ST       = 0x51,
} hostmsg_cmd_ptp_subtype_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_cbrc_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMD_CFG_CBRC message subtypes. They are located in first
*   byte of second host message word. For more information please look at the
*   DIGI Firmware User Manual.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_cmd_cbrc_subtype_t
{
    HOSTMSG_CMD_CFG_CBRC_INTR_MONITOR_CFG_ST                        = 0x01,
    HOSTMSG_CMD_CFG_CBRC_INTR_MONITOR_STATUS_GET_ST                 = 0x02,
    HOSTMSG_CMD_CFG_CBRC_INTR_CHANNELIZED_MONITOR_STATUS_GET_ST     = 0x03,
    HOSTMSG_CMD_CFG_CBRC_INTR_CLEAR_MONITOR_STATUS                  = 0x04,
    HOSTMSG_CMD_CFG_CBRC_CHANNELIZED_INTR_CLEAR_MONITOR_STATUS      = 0x05

} hostmsg_cmd_cbrc_subtype_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_led_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMD_CFG_LED message subtypes. They are located in first byte of second
*   host message word. For more information please look at the DIGI Firmware User
*   Manual.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_cmd_led_subtype_t
{
    HOSTMSG_CMD_CFG_LED_SET_SPI_DEV_ADDR_ST           = 0x01,
    HOSTMSG_CMD_CFG_LED_SET_MON_CTRL_ST               = 0x02,
} hostmsg_cmd_led_subtype_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_performance_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMD_CFG_PERFORMANCE message subtypes. They are located in first byte of second
*   host message word. For more information please look at the DIGI Firmware User
*   Manual.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_cmd_performance_subtype_t
{
    HOSTMSG_CMD_CFG_INIT_FW_PERFORMANCE_QUEUE_GET_SPRAM_ADDR_ST  = 0x01,
    HOSTMSG_CMD_CFG_INIT_FW_PERFORMANCE_HOST_QUEUE_ST            = 0x02,
} hostmsg_cmd_performance_subtype_t;

/*******************************************************************************
* ENUM: hostmsg_c8_serdes_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   C8 SERDES Rates
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_c8_serdes_rate_t
{
    HOSTMSG_C8_SERDES_RATE_6G25    = 1,
    HOSTMSG_C8_SERDES_RATE_3G125   = 2,
    HOSTMSG_C8_SERDES_RATE_MAX_NUM
} hostmsg_c8_serdes_rate_t;

/*******************************************************************************
* ENUM: hostmsg_t8_serdes_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   T8 SERDES Rates
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_t8_serdes_rate_t
{
    HOSTMSG_T8_SERDES_RATE_3G125     = 7,
    HOSTMSG_T8_SERDES_RATE_6G25      = 8,
    HOSTMSG_T8_SERDES_RATE_MAX_NUM
} hostmsg_t8_serdes_rate_t;

/*******************************************************************************
* ENUM: hostmsg_s16_serdes_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   S16 SERDES Rates
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_s16_serdes_rate_t
{
    HOSTMSG_S16_SERDES_RATE_2G488    = 1,
    HOSTMSG_S16_SERDES_RATE_3G125    = 2,
    HOSTMSG_S16_SERDES_RATE_4G250    = 3,
    HOSTMSG_S16_SERDES_RATE_4G915    = 4,
    HOSTMSG_S16_SERDES_RATE_5G000    = 5,
    HOSTMSG_S16_SERDES_RATE_6G144    = 6,
    HOSTMSG_S16_SERDES_RATE_6G250    = 7,
    HOSTMSG_S16_SERDES_RATE_8G500    = 8,
    HOSTMSG_S16_SERDES_RATE_9G830    = 9,
    HOSTMSG_S16_SERDES_RATE_9G953    = 10,

    HOSTMSG_S16_SERDES_RATE_10G000   = 11,
    HOSTMSG_S16_SERDES_RATE_10G037   = 12,
    HOSTMSG_S16_SERDES_RATE_10G312   = 13,
    HOSTMSG_S16_SERDES_RATE_10G312_DIC = 14,
    HOSTMSG_S16_SERDES_RATE_10G355   = 15,
    HOSTMSG_S16_SERDES_RATE_10G399   = 16,
    HOSTMSG_S16_SERDES_RATE_10G518   = 17,
    HOSTMSG_S16_SERDES_RATE_10G562   = 18,
    HOSTMSG_S16_SERDES_RATE_10G607   = 19,
    HOSTMSG_S16_SERDES_RATE_10G709   = 20,
    HOSTMSG_S16_SERDES_RATE_10G754   = 21,
    HOSTMSG_S16_SERDES_RATE_10G925   = 22,

    HOSTMSG_S16_SERDES_RATE_11G049   = 23,
    HOSTMSG_S16_SERDES_RATE_11G095   = 24,
    HOSTMSG_S16_SERDES_RATE_11G142   = 25,
    HOSTMSG_S16_SERDES_RATE_11G145   = 26,
    HOSTMSG_S16_SERDES_RATE_11G180   = 27,
    HOSTMSG_S16_SERDES_RATE_11G182   = 28,
    HOSTMSG_S16_SERDES_RATE_11G200   = 29,
    HOSTMSG_S16_SERDES_RATE_11G270   = 30,
    HOSTMSG_S16_SERDES_RATE_11G317   = 31,
    HOSTMSG_S16_SERDES_RATE_11G319   = 32,
    HOSTMSG_S16_SERDES_RATE_11G320   = 33,
    HOSTMSG_S16_SERDES_RATE_11G32252 = 34,
    HOSTMSG_S16_SERDES_RATE_11G32259 = 35,
    HOSTMSG_S16_SERDES_RATE_11G32275 = 36,
    HOSTMSG_S16_SERDES_RATE_11G500   = 37,

    HOSTMSG_S16_SERDES_RATE_12G500   = 38,
    HOSTMSG_S16_SERDES_RATE_MAX_NUM
} hostmsg_s16_serdes_rate_t;

/*******************************************************************************
* ENUM: hostmsg_sfi51_serdes_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   SFI5.1 SERDES Rates
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_sfi51_serdes_rate_t
{
    HOSTMSG_SFI51_SERDES_RATE_OC768_STM256     = 1,
    HOSTMSG_SFI51_SERDES_RATE_OTU3             = 2,
    HOSTMSG_SFI51_SERDES_RATE_OTU3_64_63       = 3,
    HOSTMSG_SFI51_SERDES_RATE_OTU3_64_63x57_55 = 4,
    HOSTMSG_SFI51_SERDES_RATE_OTU3E1           = 5,
    HOSTMSG_SFI51_SERDES_RATE_OTU3E1_64_63     = 6,
    HOSTMSG_SFI51_SERDES_RATE_OTU3E2           = 7,
    HOSTMSG_SFI51_SERDES_RATE_OTU3E2_64_63     = 8,
    HOSTMSG_SFI51_SERDES_RATE_MAX_NUM
} hostmsg_sfi51_serdes_rate_t;

/*******************************************************************************
* ENUM: hostmsg_d8_intf_type_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*   D8 SERDES Rates
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_d8_intf_type_enum
{
    HOSTMSG_D8_SERDES_INTF_OHP_0     = 0,
    HOSTMSG_D8_SERDES_INTF_OHP_1     = 1,
    HOSTMSG_D8_SERDES_INTF_OHP_2     = 2,
    HOSTMSG_D8_SERDES_INTF_ENET_MGMT = 3,
    HOSTMSG_D8_SERDES_INTF_RI_RCP    = 4,
    HOSTMSG_D8_SERDES_INTF_DI_RCP    = 5,
    HOSTMSG_D8_SERDES_INTF_MAX_NUM
} hostmsg_d8_intf_type_enum;

/*******************************************************************************
* ENUM: hostmsg_cmf_enet_mapotn_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMF ENET modes
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_cmf_enet_mapotn_mode_t
{
    HOSTMSG_CMF_ENET_MAPOTN_GFP_MAC                     = 0,
    HOSTMSG_CMF_ENET_MAPOTN_GFP_GSUP43_7_3_10GE_MAC     = 1,
    HOSTMSG_CMF_ENET_MAPOTN_BMP_GSUP43_10GE_MAC         = 2,
    HOSTMSG_CMF_ENET_MAPOTN_GMP_G709_40GE_MAC           = 3,
    HOSTMSG_CMF_ENET_MAPOTN_GMP_G709_100GE_MAC          = 4,
    HOSTMSG_CMF_ENET_MAPOTN_MAX_NUM
} hostmsg_cmf_enet_mapotn_mode_t;

/*******************************************************************************
* ENUM: hostmsg_opsa_fifo_clbr_target_ss_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   OPSA CFC FIFO_CLRB subsystem target
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum hostmsg_opsa_fifo_clbr_target_ss_t
{    
    HOSTMSG_OPSA_FIFO_CLRB_TARGET_SS_COREOTN_FMF1     = 0,
    HOSTMSG_OPSA_FIFO_CLRB_TARGET_SS_MAPOTN           = 1,
    HOSTMSG_OPSA_FIFO_CLRB_TARGET_SS_MAX_NUM
} hostmsg_opsa_fifo_clbr_target_ss_t;

/*******************************************************************************
* ENUM: hostmsg_cmd_stats_subtype_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   stat message subtypes. They are located in first
*   byte of second host message word. For more information please look at the
*   DIGI Firmware User Manual.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum hostmsg_cmd_stats_subtype_t
{
    STATS_HOSTMSG_STYPE_GET = 0x01,
    STATS_HOSTMSG_STYPE_RESET = 0x02,
} hostmsg_cmd_stats_subtype_t;


/******************************************************************************* 
 *                                                                             *
 * Methods                                                                     *
 *                                                                             *
*******************************************************************************/
PUBLIC PMC_ERROR  hostmsg_handle_init(pmc_handle_t *pmc_handle);
PUBLIC PMC_ERROR hostmsg_handle_restart_init(pmc_handle_t *pmc_handle,
                                             util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                             pmc_energy_state_t energy_state);
PUBLIC void hostmsg_ctxt_create(pmc_handle_t *parent,
                                UINT32 base_address,
                                pmc_sys_handle_t *sys_handle,
                                const char *tsb_name);
PUBLIC void hostmsg_ctxt_destroy(pmc_handle_t *pmc_handle);

PUBLIC void hostmsg_clear_outbound_doorbell(pmc_handle_t *pmc_handle);

/*******************************************************************************
* HOSTMSG_DECLARE_CFG_MSG_STAT
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Declare a statistic variable suitable for passing to hostmsg_send_cfg_message
*  Note: this is the same as PMC_SYS_STAT_DECLARE, but you can't pass __FUNCTION__ as a macro argument.
*
* example:
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,mod_id,msg_subtype,data,length,stat);
    }
    
*
* INPUTS:
*   stat  - variable name
*
*******************************************************************************/
#define HOSTMSG_DECLARE_CFG_MSG_STAT(stat) PMC_SYS_STAT_DECLARE_WITH_FUNCTION(stat,"FW_CFG:")

PUBLIC PMC_ERROR hostmsg_send_cfg_message(pmc_handle_t *pmc_handle,
                                          UINT32 mod_id,
                                          UINT32 msg_subtype,
                                          UINT32 *cfg_msg,
                                          UINT32 len,
                                          pmc_sys_stat_t *stat);
PUBLIC PMC_ERROR hostmsg_start_cfg_message(pmc_handle_t *pmc_handle,
                                          UINT32 mod_id,
                                          UINT32 msg_subtype,
                                          UINT32 *cfg_msg,
                                          UINT32 len,
                                          pmc_sys_stat_t *stat);
PUBLIC PMC_ERROR hostmsg_send_cfg_message_with_error_info(pmc_handle_t *pmc_handle,
                                                          UINT32 mod_id,
                                                          UINT32 msg_subtype,
                                                          UINT32 *cfg_msg,
                                                          UINT32 len,
                                                          UINT32 *msg_ctrl_ptr,
                                                          pmc_sys_stat_t *stat);


/*******************************************************************************
* HOSTMSG_DECLARE_QUERY_STAT
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Declare a statistic variable suitable for passing to hostmsg_perform_fixed/variable_length_query
*  Note: this is the same as PMC_SYS_STAT_DECLARE, but you can't pass __FUNCTION__ as a macro argument.
*
* example:
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,mod_id,msg_subtype,data,length,response,max_response_length,NULL,stat);
    }
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,mod_id,msg_subtype,data,length,response,max_response_length,stat);
    }
* INPUTS:
*   stat  - variable name
*
*******************************************************************************/
#define HOSTMSG_DECLARE_QUERY_STAT(stat) PMC_SYS_STAT_DECLARE_WITH_FUNCTION(stat,"FW_QUERY:")


PUBLIC PMC_ERROR hostmsg_perform_fixed_length_query(pmc_handle_t *pmc_handle,
                                                     UINT32 mod_id,
                                                     UINT32 msg_subtype,
                                                     UINT32 *cfg_msg,
                                                     UINT32 cfg_len,
                                                     UINT32 *response,
                                                     UINT32 max_response_len,
                                                     pmc_sys_stat_t *stat);

PUBLIC PMC_ERROR hostmsg_perform_variable_length_query(pmc_handle_t *pmc_handle,
                                                     UINT32 mod_id,
                                                     UINT32 msg_subtype,
                                                     UINT32 *cfg_msg,
                                                     UINT32 cfg_len,
                                                     UINT32 *response,
                                                     UINT32 max_response_len,
                                                     UINT32 *actual_response_len,
                                                     pmc_sys_stat_t *stat );
#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */

/*
** Log Messaging
*/

/*
 * Add any new log codes in the following paired definitions
 * below. These will be defined as a (enum, string) pair if printing out
 * errors or if embedded (limited memory) then will just define the enum.
 * This method means that one table of log/error values and string pairs
 * can be used to generate just a list of enums or strings and enums
 */
#define HOSTMSG_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_LOG_CODE_DEBUG,  "HOSTMSG: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_ERR_CODE_ASSERT, "HOSTMSG: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_LOG_CODE_LOG, "HOSTMSG: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_ERR_INVALID_PARAMETERS, "HOSTMSG: Err Invalid Parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_ERR_INVALID_DATA, "HOSTMSG: Invalid Data" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_ERR_TIMEOUT, "HOSTMSG: Err Timeout" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_FAILURE, "HOSTMSG: Failure" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_ERR_RSP_NACK, "HOSTMSG: NACK Response" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_ERR_MEM, "HOSTMSG: Err memory alloc" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_ERR_MOD_ID_MISMATCH, "HOSTMSG: mod id not equal to expected mod id" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_ERR_MSG_CAT_MISMATCH, "HOSTMSG: msg cat not equal to expected msg cat" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_ERR_MSG_SUBTYPE_MISMATCH, "HOSTMSG: msg subtype not equal to expected msg subtype" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_ERR_SEQ_NUM_MISMATCH, "HOSTMSG: seq num not equal to expected seq num" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_ERR_RSP_MSG_LEN, "HOSTMSG: Length of response msg not equal to expected msg length" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_ERR_ENET_CLIENT_NOT_VALID, "HOSTMSG: ENET_LINE or ENET_SYS are not properly configured" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(HOSTMSG_ERR_INVALID_STATE, "HOSTMSG: invalid state" ) \

/*
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum HOSTMSG_LOG_ERR_INDEX
{
    HOSTMSG_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_HOSTMSG << 16),
    HOSTMSG_LOG_ERR_TABLE
    HOSTMSG_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char HOSTMSG_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char HOSTMSG_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */
#endif /* DOXYGEN_PUBLIC_ONLY */


#ifdef __cplusplus
}
#endif

#endif /* _HOSTMSG_API_H */

