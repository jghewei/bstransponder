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
*   DESCRIPTION :
*
*   NOTES:
*
*******************************************************************************/
#ifndef _ODU_RCP_API_H
#define _ODU_RCP_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "cfc_api.h"
#include "util_global_api.h"
#include "enet_fege_api.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: odu_rcp_cfc_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enumerated type to specifythe RI RCP CFC instance 
*
* ELEMENTS:
*   ODU_RCP_RX_CFC_STG1            - RX Framer stage 1
*   ODU_RCP_RX_CFC_STG2            - RX Framer stage 2
*   ODU_RCP_RX_CFC_STG3A           - RX Framer stage 3A
*   ODU_RCP_RX_CFC_STG3B           - RX Framer stage 3b
*   ODU_RCP_RX_CFC_STG4            - RX Framer stage 4
*   ODU_RCP_RX_CFC_STGN            - RX Framer stage N
*   ODU_RCP_TX_CFC_STG1            - TX Framer stage 1
*   ODU_RCP_TX_CFC_STG2            - TX Framer stage 2
*   ODU_RCP_TX_CFC_STG3A           - TX Framer stage 3A
*   ODU_RCP_TX_CFC_STG3B           - TX Framer stage 3b
*   ODU_RCP_TX_CFC_STG4            - TX Framer stage 4
*   ODU_RCP_TX_CFC_STGN            - TX Framer stage N
*       
*******************************************************************************/
typedef enum odu_rcp_cfc_id_t
{
    ODU_RCP_RX_CFC_STG1 = 0,
    ODU_RCP_RX_CFC_STG2 = 1,
    ODU_RCP_RX_CFC_STG3A = 2,
    ODU_RCP_RX_CFC_STG3B = 3,
    ODU_RCP_RX_CFC_STG4 = 4,
    ODU_RCP_RX_CFC_STGN = 5,
    ODU_RCP_TX_CFC_STG1 = 6,
    ODU_RCP_TX_CFC_STG2 = 7,
    ODU_RCP_TX_CFC_STG3A = 8,
    ODU_RCP_TX_CFC_STG3B = 9,
    ODU_RCP_TX_CFC_STG4 = 10,
    ODU_RCP_TX_CFC_STGN = 11,
    LAST_ODU_RCP_CFC_ID
} odu_rcp_cfc_id_t;

/*******************************************************************************
* ENUM: odu_rcp_frm_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enumerated type to specify ODU_RCP RI framer stage 
*
* ELEMENTS:
*   ODU_RCP_FRM_STG1            - Framer stage 1
*   ODU_RCP_FRM_STG2            - Framer stage 2
*   ODU_RCP_FRM_STG3A           - Framer stage 3A
*   ODU_RCP_FRM_STG3B           - Framer stage 3b
*   ODU_RCP_FRM_STG4            - Framer stage 4
*   ODU_RCP_FRM_STGN            - Framer stage N
*           
*******************************************************************************/
typedef enum odu_rcp_frm_id_t
{
    ODU_RCP_FRM_STG1 = 0,
    ODU_RCP_FRM_STG2 = 1,
    ODU_RCP_FRM_STG3A = 2,
    ODU_RCP_FRM_STG3B = 3,
    ODU_RCP_FRM_STG4 = 4,
    ODU_RCP_FRM_STGN = 5,
    LAST_ODU_RCP_FRM_ID
} odu_rcp_frm_id_t;


/*******************************************************************************
* ENUM: odu_ri_rcp_cfc_int_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODU RI RCP CFC interrupt type: FIFO overflow or FIFO underflow.
*
*
* ELEMENTS:
*   ODU_RI_RCP_CFC_TYPE_FIFO_OVERFLOW    - FIFO overflow interruppt is selected.
*   ODU_RI_RCP_CFC_TYPE_FIFO_UNDERFLOW   - FIFO overflow interruppt is selected.
*******************************************************************************/
typedef enum odu_ri_rcp_cfc_int_type_t
{
    ODU_RI_RCP_CFC_TYPE_FIFO_UNDERFLOW = 0,
    ODU_RI_RCP_CFC_TYPE_FIFO_OVERFLOW = 1,
    LAST_ODU_RI_RCP_CFC_TYPE
}odu_ri_rcp_cfc_int_type_t;

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: digi_di_rcp_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*     Structure defining Defects Information Ring Control Port properties
*
* ELEMENTS:
*   src_mac           - source MAC address
*   dest_mac          - destination MAC address
*   eth_etype         - The Ethertype field of the Ethernet frame. The value 
*                       must be higher than 0x0800 and not conflict with any of 
*                       the standardized Ethertype values. 
*   discard_empty_pdu - When the option is enabled, empty PDUs or PDU without 
*                       relevant information will be automatically discarded 
*                       within the DI_RCP port
*
*******************************************************************************/
typedef  struct odu_di_rcp_cfg_t
{   
   UINT8 src_mac[UTIL_GLOBAL_MAC_NUM_BYTE];
   UINT8 dest_mac[UTIL_GLOBAL_MAC_NUM_BYTE];
   UINT16 eth_etype;
   BOOL8 discard_empty_pdu;
} odu_di_rcp_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_otn_odu_ri_rcp_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*     Structure defining Remote Information Ring Control Port properties
*
* ELEMENTS:
*   eth_mac_oui     - Organizationally Unique  Identifier is inserted in the 3\n
*                     first bytes  of the source MAC.
*   eth_mac_digi_id - DIGI identifier is inserted in the fourth byte of the \n
*                     source MAC.
*   eth_etype       - The Ethertype field of the Ethernet frame. The value must\n
*                     be higher than 0x0800 and not conflict with any of the \n
*                     standardized Ethertype values. 
*
*******************************************************************************/
typedef struct digi_otn_odu_ri_rcp_cfg_t
{   
    UINT32 eth_mac_oui;
    UINT8  eth_mac_digi_id;
    UINT32 eth_etype;
} digi_otn_odu_ri_rcp_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_otn_odu_ri_rcp_dst_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure defining Remote Information Ring Control Port destination 
*  configuration
*
* ELEMENTS:
*   dst_digi    - Destination DIGI Identifier. User can specify unique number \n
*                 as per number  of DIGI's on board.
*   dst_frm     - Destination framer stage on destination DIGI
*   dst_channel - Destination channel on destination DIGI
*******************************************************************************/
typedef  struct digi_otn_odu_ri_rcp_dst_cfg_t
{   
    UINT8 dst_digi;
    UINT8 dst_frm;
    UINT8 dst_channel;
} digi_otn_odu_ri_rcp_dst_cfg_t;


/*******************************************************************************
* STRUCTURE: odu_rcp_int_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODU RI and DI RCP table for unchannelized interrupts and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct odu_rcp_int_t
{
    enet_fege_int_t   enet_fege;
    BOOL8             pmon_tx_done_i;
    BOOL8             dfec_tx_done_i;
} odu_rcp_int_t;


/*******************************************************************************
* STRUCTURE: odu_ri_rcp_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODU RI RCP CFC channel table for interrupt and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct odu_ri_rcp_int_chnl_t
{
    cfc_int_chnl_t  cfc;
} odu_ri_rcp_int_chnl_t;



/*
** Global variables
*/
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
#define ODU_RCP_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_ERR_CODE_ASSERT,                 "ODU_RCP: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_LOG_CODE_LOG,                    "ODU_RCP: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_ERR_NULL_HANDLE,                 "ODU_RCP: NULL Handle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_ERR_INVALID_ARGS,                "ODU_RCP: Invalid arguments" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_ERR_DIV_BY_ZERO,                 "ODU_RCP: Division by zero" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_STATE_NOT_START,                 "ODU_RCP: Instance is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_STATE_NOT_PROVISIONED,           "ODU_RCP: Instance is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_STATE_NOT_OPERATIONNAL,             "ODU_RCP: Instance is not in operational state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_STATE_ALREADY_START,             "ODU_RCP: Instance is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_STATE_ALREADY_PROVISIONED,       "ODU_RCP: Instance is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_DI_PDU_NOT_ENABLED,                 "ODU_RCP: DI RCP PDU not enabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_STATE_UNKOWN,                    "ODU_RCP: Unkown state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_STATE_RI_EXTRACT_ALREADY_ENABLED, "ODU_RCP: RI extraction already enabled on this channel" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_STATE_RI_EXTRACT_NOT_ENABLED,     "ODU_RCP: RI extraction not enabled on this channel" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_STATE_RI_INSERT_BACKWARD_DEFECT_ALREADY_ENABLED, "ODU_RCP: RI backward defect insertion already enabled on this channel" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_STATE_RI_INSERT_BACKWARD_DEFECT_NOT_ENABLED,     "ODU_RCP: RI backward defect insertion not enabled on this channel" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_STATE_RI_INSERT_APS_ALREADY_ENABLED, "ODU_RCP: RI APS insertion already enabled on this channel" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_STATE_RI_INSERT_APS_NOT_ENABLED,     "ODU_RCP: RI APS insertion not enabled on this channel" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_STATE_RI_UNINIT_ERR,     "ODU_RCP: Trying to un-initialize while insertion or extraction is enabled on at least 1 channel" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_ERR_BAD_DB_ENTRY,           "ODU_RCP: Database entry is incorrect" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_RI_ERR_INVALID_ETH_MAC_OUI,           "ODU_RCP: RI Organizationally Unique Identifier is bad (it is 24 bits)" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_RI_ERR_INVALID_ETH_DIGI_ID,           "ODU_RCP: RI DIGI Identifier is bad (it is 8 bits)" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_RI_ERR_INVALID_ETH_ETYPE,           "ODU_RCP: RI Ethertype is bad (it is 16 bits)" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_RI_ERR_INVALID_DEST_FRM,           "ODU_RCP: RI Invalid destination framer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_RI_ERR_INVALID_DEST_CHNL,           "ODU_RCP: RI Invalid destination channel" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ODU_RCP_RI_ERR_INVALID_DEST_DIGI,           "ODU_RCP: RI Invalid destination DIGI id" ) \
/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum ODU_RCP_LOG_ERR_INDEX
{
    ODU_RCP_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_ODU_RCP << 16),
    ODU_RCP_LOG_ERR_TABLE
    ODU_RCP_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char ODU_RCP_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char ODU_RCP_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */


/*
** Function Prototypes
*/



#ifdef __cplusplus
}
#endif

#endif /* _ODU_RCP_API_H */

/*
** end of file
*/
