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

#ifndef _EMAC_API_H
#define _EMAC_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: emac_error_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated EMAC packet error types.
*
* ELEMENTS:
*   EMAC_PKT_ERROR_PKT_LENGTH    - Packet length error (on a Per link basis).
*   EMAC_ERROR_FCS               - FCS error (on a Per link basis).
*   EMAC_ERROR_RX_CODE           - Wrong / Unexpected received code.
*   EMAC_ERROR_FIFO_OVR          - FIFO overflow.
*   EMAC_ERROR_RX_SEQ            - Received sequence /ERROR/ (local or remote).
*   EMAC_ERROR_STACKED_VLAN_TAG  - Current frame implements a Stacked VLAN Tag.
*   EMAC_ERROR_CTRL_CHAR         - Received an Error contol character on the XL/CGMII.
*   EMAC_ERROR_VLAN_TAG          - Current frame implements a VLAN Tag.
*
*******************************************************************************/
typedef enum emac_error_t
{
    EMAC_ERROR_PKT_LENGTH       = 0,
    EMAC_ERROR_FCS              = 1,
    EMAC_ERROR_RX_CODE          = 2,
    EMAC_ERROR_FIFO_OVR         = 3,
    EMAC_ERROR_RX_SEQ           = 4,
    EMAC_ERROR_STACKED_VLAN_TAG = 5,
    EMAC_ERROR_CTRL_CHAR        = 6,
    EMAC_ERROR_VLAN_TAG         = 7,

    LAST_EMAC_ERROR
} emac_error_t;

/*******************************************************************************
* ENUM: emac_dpi_err_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated EMAC packet error types.
*
* ELEMENTS:
*   EMAC_DPI_ERR_FCS_ENABLE      - FCS error may contribute to DPI error/drop
*   EMAC_DPI_ERR_FCS_DISABLE     - FCS error will not contribute to DPI error/drop
*   EMAC_DPI_ERR_LEN_ENABLE      - Length error may contribute to DPI error/drop
*   EMAC_DPI_ERR_lEN_DISABLE     - Length error will not contribute to DPI error/drop
*
*******************************************************************************/
typedef enum emac_dpi_err_mask_t
{
    EMAC_INGR_DPI_ERR_FCS_ENABLE  = 0,
    EMAC_INGR_DPI_ERR_FCS_DISABLE = 1,
    EMAC_INGR_DPI_ERR_LEN_ENABLE  = 2,
    EMAC_INGR_DPI_ERR_LEN_DISABLE = 3,
    EMAC_INGR_DPI_ERR_LAST
} emac_dpi_err_cfg_t;


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
* STRUCTURE: emac_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   EMAC level channel table for interrupt and status.
*
*   Note that if channel has phy_los_v/i TRUE, then all locking/alignment status
*   should be ignored.
*
*   Note that block_lock_v/i is valid only for 10GE mode
*   and is invalid for 40GE and 100GE modes.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   phy_los_i;
    BOOL8   rx_loc_fault_i;
    BOOL8   rx_rem_fault_i;
    BOOL8   hi_ber_i;
    BOOL8   block_lock_i;
    BOOL8   align_status_i;
    BOOL8   emac_tx_overflow_i;
    BOOL8   emac_tx_underflow_i;
    
    BOOL8   phy_los_v;
    BOOL8   rx_loc_fault_v;
    BOOL8   rx_rem_fault_v;
    BOOL8   hi_ber_v;
    BOOL8   block_lock_v;
    BOOL8   align_status_v;
    BOOL8   link_status;
    BOOL8   all_lanes_lock;
    UINT32  lane_19_12_block_lock;
    UINT32  lane_7_0_block_lock;  
    UINT32  lane_align_lock_19_12;
    UINT32  lane_align_lock_7_0;  
} emac_int_chnl_t;



/*
** Global variables
*/

/*
** Function Prototypes
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
#define EMAC_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMAC_LOG_ERR_NULL_HANDLE,    "EMAC: NULL Handle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMAC_LOG_ERR_INVALID_LINK_ID,    "EMAC: Invalid link ID" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMAC_LOG_ERR_INVALID_RATE,    "EMAC: Invalid Rate" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMAC_LOG_ERR_INVALID_TX_IPG_LEN,    "EMAC: Invalid TX IPG Length" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMAC_LOG_ERR_INVALID_PARAMETERS,    "EMAC: Invalid parameters" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMAC_LOG_ERR_LOOPBACK_ENABLED,    "EMAC: Loopback not disabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMAC_LOG_ERR_ALLOC_FAILED,    "EMAC: memory allocation failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMAC_LOG_ERR_PRBS_STILL_ENABLED,    "EMAC: PRBS still enabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMAC_LOG_ERR_PRBS_NOT_ENABLED,    "EMAC: PRBS already disabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMAC_LOG_ERR_LINK_NOT_PROV,    "EMAC: Link not provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMAC_LOG_ERR_PRBS_TYPE,        "EMAC: PRBS type not supported. Only PRBS 9 and 31 are supported." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMAC_LOG_ERR_UNSUPPORTED_REV_A_FEATURE,  "EMAC: This feature is not supported in revision A")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(EMAC_LOG_ERR_PRBS_ALREADY_ENABLED,    "EMAC: PRBS already enabled" ) \


/* 
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above  
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum EMAC_LOG_ERR_INDEX
{
    EMAC_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_EMAC << 16),
    EMAC_LOG_ERR_TABLE
    EMAC_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char EMAC_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char EMAC_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _EMAC_API_H */

/*
** end of file
*/
