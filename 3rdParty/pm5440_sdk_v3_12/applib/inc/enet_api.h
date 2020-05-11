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
*     This file contains function prototypes, enumerations, and structures
*     for the ENET subsystem.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _ENET_API_H
#define _ENET_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "emac_api.h"
#include "empei_e_api.h"
#include "empei_i_api.h"
#include "epmm_api.h"
#include "etrans_api.h"


/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: enet_data_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Ethernet line rates.  The
*   element terms have a syntax:
*      ENET_LINK_<rate>
*
* ELEMENTS:
*   ENET_LINK_10G  - Ethernet link is 10G
*   ENET_LINK_40G  - Ethernet link is 40G
*   ENET_LINK_100G - Ethernet link is 100G
*
*******************************************************************************/
typedef enum 
{
    ENET_LINK_10G = 0x0,
    ENET_LINK_40G = 0x1,
    ENET_LINK_100G = 0x2,
    LAST_ENET_LINK_DATA_RATE
} enet_data_rate_t;

/*******************************************************************************
* ENUM: enet_pmon_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for the direction of Ethernet PMON monitoring.  
*   The element terms have a syntax:
*      ENET_<dir>_CBR
*
* ELEMENTS:
*   ENET_TX_CBR  - PMON monitoring for Tx CBR traffic (from EPMM)
*   ENET_RX_CBR  - PMON monitoring for Rx CBR traffic (from SERDES)
*
*******************************************************************************/
typedef enum 
{
    ENET_TX_CBR = 0x0,
    ENET_RX_CBR = 0x1
} enet_pmon_dir_t;

/*******************************************************************************
* ENUM: enet_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for the direction of Ethernet.  
*   The element terms have a syntax:
*      ENET_<dir>
*
* ELEMENTS:
*   ENET_EGRESS   - egress direction
*   ENET_INGRESS  - ingress direction
*
*******************************************************************************/
typedef enum 
{
    ENET_EGRESS = 0x0,    
    ENET_INGRESS = 0x1,
    LAST_ENET_DIR
} enet_dir_t;

/*******************************************************************************
* ENUM: enet_extract_pkt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for extracted packet type
*   The element terms have a syntax:
*      ENET_EXTRACT_<type>_PKTS
*
* ELEMENTS:
*   ENET_EXTRACT_NO_PKTS        - no packet extraction
*   ENET_EXTRACT_ALL_PKTS_MGMT  - extract all packets to Mgmt
*   ENET_EXTRACT_ALL_PKTS_CPU   - extract all packets to CPU
*   ENET_EXTRACT_PTP_PKTS       - extract PTP packets
*
*******************************************************************************/
typedef enum 
{
    ENET_EXTRACT_NO_PKTS = 0x0,
    ENET_EXTRACT_ALL_PKTS_MGMT = 0x1,
    ENET_EXTRACT_ALL_PKTS_CPU = 0x2,
    ENET_EXTRACT_PTP_PKTS = 0x3
} enet_extract_pkt_t;


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
* STRUCTURE: enet_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ENET level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    etrans_int_chnl_t   etrans;
    emac_int_chnl_t     emac;
    epmm_int_chnl_t     epmm;
    empei_e_int_chnl_t  empei_e;
    empei_i_int_chnl_t  empei_i;
} enet_int_chnl_t;


typedef struct enet_handle_struct_t enet_handle_t; /*!< Declare enet handle */

/*
** Global variables
*/

/*
** Function Prototypes
*/

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
#define ENET_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_LOG_ERR_NULL_HANDLE, "ENET: NULL Handle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_LOG_ERR_INVALID_ARG, "ENET: Invalid Argument" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_LOG_ERR_NULL_PARAMETERS, "ENET: NULL Parameter" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_LOG_ERR_PAUSA_QUANTA_THRESH_LT_TWO, "ENET: Pause quanta threshold shall be superior or equal to 2" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_LOG_ERR_INVALID_PARAMETERS, "ENET: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_LOG_ERR_ALREADY_PROVISIONED, "ENET: Link already provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_ERR_CODE_ASSERT,   "ENET: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EPMM_INVALID_DATAPATH,  "ENET: Invalid data path provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_ERR_NO_SW_CONTENT, "ENET: Err No software content" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_ERR_UNSUPPORTED_SCHD_DB_MODE, "ENET: Scheduler Mode Does not Support Database" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_ERR_DB_ATTACH_FAIL, "ENET: DB Attachment Operation Failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_LOG_CODE_LOG,      "ENET: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_ERR_DB_ENTRY_NOT_FOUND, "ENET: DB entry not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_ERR_INVALID_MAPPING_MODE, "ENET: Invalid mapping mode" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_LNK_STATE_NOT_START, "ENET: Link is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_LNK_STATE_NOT_EQUIPPED, "ENET: Link is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_LNK_STATE_NOT_OPERATIONAL, "ENET: Link is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_BLOCK_NOT_OPERATIONAL, "ENET: Block is not in operational state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_ERR_ETRANS_NOT_LOCKED, "ENET: Etrans did not locked after its activation") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_ERR_CONTEXT_INCOHERENT,                   "ENET: Context and Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_ERR_CONTEXT_REG_INCOHERENT,               "ENET: Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_ERR_CONTEXT_CLEANUP,                      "ENET: Context cleanup is required" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT, "ENET: Communal ressource energy states incoherent" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(ENET_ERR_INVALID_DATAPATH, "ENET: Operation not suppported with configured datapath" ) \
    

/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum ENET_LOG_ERR_INDEX
{
    ENET_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_ENET << 16),
    ENET_LOG_ERR_TABLE
    ENET_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char ENET_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char ENET_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#endif /* DOXYGEN_PUBLIC_ONLY */

#ifdef __cplusplus
}
#endif

#endif /* _ENET_API_H */

/* 
** end of file 
*/
