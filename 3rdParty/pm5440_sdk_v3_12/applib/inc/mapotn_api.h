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
*   DESCRIPTION : This file contains definitions for user funtions.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _MAPOTN_API_H
#define _MAPOTN_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "rgmp_api.h"
#include "ohfs_insert_api.h"
#include "tgmp_api.h"
#include "cfc_api.h"
#include "oduksc_api.h"
#include "mpma_api.h"
#include "mpmo_api.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: mapotn_rgfpf_cmf_dest_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify destination for CMF UPI to be filtered.
*
* ELEMENTS:
*   MAPOTN_CPU                - CPU
*   MAPOTN_CSF_FIFO           - CSF FIFO
*           
*
*******************************************************************************/
typedef enum {
    MAPOTN_CPU      = 0,
    MAPOTN_CSF_FIFO = 1,
} mapotn_rgfpf_cmf_dest_t;


/*******************************************************************************
* ENUM: mapotn_rgfpf_cmf_upi_filt_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify CMF UPI filtering Mode.
*
* ELEMENTS:
*   MAPOTN_FORWARD_ALL_UPI           - Forward all UPI values to CPU FW port
*   MAPOTN_FORWARD_ONE_UPI_RANGE     - One range of UPI values to CPU FW port, other 
*                                      UPI values dropped
*   MAPOTN_FORWARD_TWO_UPI_RANGE     - Two range of UPI values to CPU FW port, other
*                                      UPI values dropped
*           
*
*******************************************************************************/
typedef enum { 
    MAPOTN_FORWARD_ALL_UPI       = 0, 
    MAPOTN_FORWARD_ONE_UPI_RANGE = 1, 
    MAPOTN_FORWARD_TWO_UPI_RANGE = 2,
} mapotn_rgfpf_cmf_upi_filt_mode_t;


/*******************************************************************************
* ENUM: mapotn_rgfpf_comparison_op_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify possible values for comparison.
*
* ELEMENTS:
*   MAPOTN_FLD_EQ            - Field equal
*   MAPOTN_FLD_NQ            - Field not equal
*   MAPOTN_FLD_GT            - Field greater than
*   MAPOTN_FLD_LT            - Field less than
*           
*
*******************************************************************************/
typedef enum {
    MAPOTN_FLD_EQ = 0,
    MAPOTN_FLD_NQ = 1,     
    MAPOTN_FLD_GT = 2,
    MAPOTN_FLD_LT = 3,
} mapotn_rgfpf_comparison_op_t;


/*******************************************************************************
* ENUM: mapotn_rgfpf_classify_actions_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify actions that can be taken towards RGFPF filter.
*
* ELEMENTS:
*   MAPOTN_ACTION_DROP        - Drop the frame
*   MAPOTN_ACTION_EGRESS      - Send frame to Egress
*   MAPOTN_ACTION_CPU         - Send frame to CPU
*   MAPOTN_ACTION_CSF         - Send frame to CSF
*   MAPOTN_ACTION_CPU_NOCSF   - Send frane to CPU do not route CSF       
*
*******************************************************************************/
typedef enum {
    MAPOTN_ACTION_DROP   = 0,
    MAPOTN_ACTION_EGRESS = 1,     
    MAPOTN_ACTION_CPU    = 2,
    MAPOTN_ACTION_CSF    = 3,
    MAPOTN_ACTION_CPU_NOCSF = 4,
    LAST_MAPOTN_ACTION
} mapotn_rgfpf_classify_actions_t;

/*******************************************************************************
* ENUM: mapotn_rgfpf_proc_seq_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify classification operation performed relative to 
*    the basic filter classification operation.
*
* ELEMENTS:
*   MAPOTN_PRE_CLASSIFY       - Pre basic filter classification operation
*   MAPOTN_POST_CLASSIFY      - Post basic filter classification operation
*           
*
*******************************************************************************/
typedef enum {
    MAPOTN_PRE_CLASSIFY  = 0,
    MAPOTN_POST_CLASSIFY = 1,     
} mapotn_rgfpf_proc_seq_t;



/*******************************************************************************
* ENUM: mapotn_tgfpf_pause_frm_memory_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify which of 4 pause frame templates is used to 
*    insert Pause frames.
*
* ELEMENTS:
*   MAPOTN_PAUSE_FRM_MEMORY0    - Pause frame Memory0 will be used for insertion
*   MAPOTN_PAUSE_FRM_MEMORY1    - Pause frame Memory1 will be used for insertion
*   MAPOTN_PAUSE_FRM_MEMORY2    - Pause frame Memory2 will be used for insertion
*   MAPOTN_PAUSE_FRM_MEMORY3    - Pause frame Memory3 will be used for insertion
*
*
*******************************************************************************/
typedef enum {
    MAPOTN_PAUSE_FRM_MEMORY0  = 0,
    MAPOTN_PAUSE_FRM_MEMORY1  = 1, 
    MAPOTN_PAUSE_FRM_MEMORY2  = 2,
    MAPOTN_PAUSE_FRM_MEMORY3  = 3,    
} mapotn_tgfpf_pause_frm_memory_t;


/*******************************************************************************
* ENUM: mapotn_fcs_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify the payload FCS field type is inserted in a GFP
*    frame.
*
* ELEMENTS:
*   MAPOTN_NO_FCS             - Do not insert any FCS
*   MAPOTN_PFCS               - Insert pFCS for GFP Frame
*   MAPOTN_EFCS_4_BYTES       - Calculate eFCS for special Ethernet traffic 
*                               received from the CPB excluding preamble bytes 
*                               after skipping 4-bytes of the packet
*   MAPOTN_EFCS_8_BYTES       - Calculate eFCS for special Ethernet traffic 
*                               received from the CPB excluding preamble bytes 
*                               after skipping 8-bytes of the packet
*   MAPOTN_EFCS_12_BYTES      - Calculate eFCS for special Ethernet traffic 
*                               received from the CPB excluding preamble bytes 
*                               after skipping 12-bytes of the packet
*   MAPOTN_EFCS_16_BYTES      - Calculate eFCS for special Ethernet traffic 
*                               received from the CPB excluding preamble bytes 
*                               after skipping 16-bytes of the packet
*   MAPOTN_EFCS               - Calculate eFCS for special Ethernet traffic 
*                               received from the CPB including the preamble 
*                               byte
*
*
*******************************************************************************/
typedef enum {
    MAPOTN_NO_FCS        = 0,
    MAPOTN_PFCS          = 1,
    MAPOTN_EFCS_4_BYTES  = 2,
    MAPOTN_EFCS_8_BYTES  = 3,
    MAPOTN_EFCS_12_BYTES = 4,
    MAPOTN_EFCS_16_BYTES = 5,
    MAPOTN_EFCS          = 6,
} mapotn_fcs_mode_t;



/*
** Constants
*/
#define MAPOTN_TGFPF_PAUSE_FRAME_TEMPLATE_CNT       4
#define MAPOTN_TGFPF_PAUSE_FRAME_MEM_TOTAL_REG_CNT  72
#define MAPOTN_TGFPF_EXTENSION_HEADER_REG_CNT       16
#define MAPOTN_TGFPF_MANAGEMENT_FRAME_REG_CNT       32

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

typedef struct mapotn_handle_struct_t  mapotn_handle_t; /*!< Opaque reference to module handle see mapotn_handle_struct_t*/
typedef struct client_hndl_struct_t    client_hndl_t;   /*!< Opaque reference to client handle see client_hndl_struct_t*/

/*******************************************************************************
* STRUCTURE: mapotn_int_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MAPOTN level table for unchannelized interrupts and status.
*
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    oduksc_int_t  oduksc;

    BOOL8 enet_fifo_ovfl_i;
    BOOL8 dci_dpi_async_fifo_full_i;
    BOOL8 unpack_afifo_ovf_i;
    BOOL8 cpu_rdrpt_afifo_ovf_i;
    BOOL8 cpu_wrrpt_afifo_ovf_i;
} mapotn_int_t;


/*******************************************************************************
* STRUCTURE: mapotn_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MAPOTN level channel table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8                   tgmp_valid;
    tgmp_int_chnl_t         tgmp;
    ohfs_insert_int_chnl_t  ohfs_insert;
    oduksc_int_chnl_t       oduksc;
    cfc_int_chnl_t          cfc_unpack;
    cfc_int_chnl_t          cfc_egress;
    cfc_int_chnl_t          cfc_packing;
    cfc_int_chnl_t          cfc_prefetch;
    BOOL8                   mpmo_valid;
    mpmo_int_chnl_t         mpmo;
    mpma_int_chnl_t         mpma;
    BOOL8                   rgmp_valid;
    rgmp_int_chnl_t         rgmp;

    BOOL8               tgfpf_valid;
    BOOL8               rgfpf_valid;

    /* RGFPF */
    BOOL8               lofd_i;
    BOOL8               idle_i;
    BOOL8               ctrlfrm_i;
    BOOL8               unpack_fifo_drop_i;
    BOOL8               uchec_i;
    BOOL8               cchec_i;
    BOOL8               uthec_i;
    BOOL8               cthec_i;
    BOOL8               uehec_i;
    BOOL8               cehec_i;
    BOOL8               exi_i;
    BOOL8               pti_upi_msmch_i;
    BOOL8               generic_i;
    BOOL8               csf_i;
    BOOL8               drop_i;
    BOOL8               datafrm_i;
    BOOL8               pause_i;
    BOOL8               max_frm_len_i;
    BOOL8               min_frm_len_i;
    BOOL8               frm_len_stat_intr_i[7];
    BOOL8               pfcse_i;
    BOOL8               dwnstrm_frm_i;
    BOOL8               ssf_sf_rcvd_i;
    BOOL8               hao_status_avl_i;
    BOOL8               dplm_i;
    BOOL8               opu_csf_i;
    BOOL8               in_sfail_i;

    /* TGFPF */
    BOOL8               len_err_int_i;
    BOOL8               pause_frame_tx_int_i;
    BOOL8               mgmt_frame_tx_int_i;
    BOOL8               oversize_frm_rx_int_i;
    BOOL8               undersize_frm_rx_int_i;
    BOOL8               ord_set_len_err_int_i;
    
    /* RGFPF status */
    BOOL8               lofd_v;
    BOOL8               idle_v;
    BOOL8               csf_v;
    BOOL8               exi_v;
    BOOL8               pti_upi_msmch_v;
    BOOL8               ssf_sf_rcvd_v;
    BOOL8               dplm_v;
    BOOL8               opu_csf_v;
    BOOL8               in_sfail_v;
} mapotn_int_chnl_t;



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
#define MAPOTN_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_LOG_CODE_DEBUG,                           "MAPOTN: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_ERR_CODE_ASSERT,                          "MAPOTN: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_LOG_CODE_LOG,                             "MAPOTN: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_ERR_INVALID_PARAMETERS,                   "MAPOTN: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_ERR_INVALID_MAPPING_MODE,                 "MAPOTN: Invalid mapping mode" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_CHNL_STATE_NOT_START,                     "MAPOTN: Channel is not in start state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_CHNL_STATE_NOT_EQUIPPED,                  "MAPOTN: Channel is not in provisioned state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_CHNL_STATE_NOT_OPERATIONAL,               "MAPOTN: Channel is not in activated state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_BLOCK_NOT_OPERATIONAL,                    "MAPOTN: Block is not in operational state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_RGFPF_ERR_NO_FILTER_AVAILABLE,            "MAPOTN: RGFPF no filter available" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_RGFPF_ERR_CLIENT_TYPE_NOT_VALID,          "MAPOTN: RGFPF client type not valid" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_RGFPF_ERR_FILTER_IN_USE,                  "MAPOTN: RGFPF filter in use" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_RGFPF_ERR_CLIENT_TO_PROV_ALREADY_EXISTS,  "MAPOTN: RGFPF client to provision already exists" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_RGFPF_ERR_CLIENT_NOT_FOUND,                "MAPOTN: RGFPF client not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_RGFPF_ERR_CLIENT_FILTERS_EXCEED_CURRENT_FILTERS, "MAPOTN: RGFPF client filters needed exceed current filters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_RGFPF_ERR_FILTER_TO_UNPROV_NOT_IN_USE,    "MAPOTN: RGFPF filter to unprove not is use" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_RGFPF_ERR_INVALID_PARAMETERS,             "MAPOTN: RGFPF invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_TGFPF_ERR_INVALID_PARAMETERS,             "MAPOTN: TGFPF invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_ERR_NO_SW_CONTENT,                        "MAPOTN: Configuration Not Coded" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_ERR_DB_RECS_UNAVAILABLE,                  "MAPOTN: DB empty records unavailable" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_ERR_DB_EXT_ACCESS,                        "MAPOTN: DB access error externally" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_ERR_DB_ENTRY_NOT_FOUND,                   "MAPOTN: DB entry not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_ERR_UNSUPPORTED_DB_MODE,                  "MAPONT: Unsupported DB mode" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_CHNL_NOT_AVAILABLE,                       "MAPOTN: Channel is not available" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_TGFPF_ERR_MGMT_FRAME_TX_IN_PROGRESS,      "MAPOTN: MGMT frame transmit in progress" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_TGFPF_ERR_MGMT_FRAME_TX_NOT_COMPLETED,    "MAPOTN: MGMT frame transmit not completed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_TGFPF_ERR_PAUSE_FRAME_TX_IN_PROGRESS,     "MAPOTN: Pause frame transmit in progress" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_TGFPF_ERR_PAUSE_FRAME_TX_NOT_COMPLETED,   "MAPOTN: Pause frame transmit not completed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_ERR_CONTEXT_INCOHERENT,                   "MAPOTN: Context and Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_ERR_CONTEXT_REG_INCOHERENT,               "MAPOTN: Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_ERR_CONTEXT_CLEANUP,                      "MAPOTN: Context cleanup is required" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_ERR_CONTEXT_COMMUN_RES_ENERGY_INCOHERENT, "MAPOTN: Communal ressource energy states incoherent" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_ERR_CLIENT_CLASSIFY_FILTERS_ALREADY_PROV, "MAPOTN: RGFPF client classify filters are already provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(MAPOTN_RGFPF_ERR_UNEXPECTED_FILTER_CFG,          "MAPOTN: RGFPF filter get operation doesn't support generic filters" ) \


/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum MAPOTN_LOG_ERR_INDEX
{
    MAPOTN_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_MAPOTN << 16),
    MAPOTN_LOG_ERR_TABLE
    MAPOTN_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char MAPOTN_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char MAPOTN_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */
                        
#endif /* DOXYGEN_PUBLIC_ONLY */

#ifdef __cplusplus
}
#endif

#endif /* _MAPOTN_API_H */

/* 
** end of file 
*/
