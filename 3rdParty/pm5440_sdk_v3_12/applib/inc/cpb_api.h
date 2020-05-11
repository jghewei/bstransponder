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

#ifndef _CPB_API_H
#define _CPB_API_H

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
* ENUM: cpb_port_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the DCPB dpi port type.
*
* ELEMENTS:
*    CPB_PORT_SIFD1     - SIFD 1 DPI Port
*    CPB_PORT_SIFD2     - SIFD 2 DPI Port
*    CPB_PORT_CBRC      - CBRC DPI Port
*    CPB_PORT_MAPOTN    - MAPOTN DPI Port
*    CPB_PORT_ENET_LINE - ENET_LINE DPI Port
*    CPB_PORT_ENET_SYS  - ENET_SYS DPI Port
*
*******************************************************************************/
typedef enum {
    CPB_PORT_SIFD1 = 0,
    CPB_PORT_SIFD2 = 1,
    CPB_PORT_CBRC = 2,
    CPB_PORT_MAPOTN = 3,
    CPB_PORT_ENET_LINE = 4,
    CPB_PORT_ENET_SYS = 5
} cpb_port_t;

/*******************************************************************************
* ENUM: cpb_pmon_mode
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Defines various performance monitor modes in the CPB.
*
* ELEMENTS:
*    CPB_PMON_MODE_PACKET  - PMON mode packet
*    CPB_PMON_MODE_BYTE    - PMON mode byte
*    CPB_PMON_MODE_DISABLE - PMON mode disabled
*
*******************************************************************************/
typedef enum {
    CPB_PMON_MODE_PACKET = 0,
    CPB_PMON_MODE_BYTE = 1,
    CPB_PMON_MODE_DISABLE = 2
} cpb_pmon_mode;

/*******************************************************************************
* ENUM: cpb_dcpb_dpi_port_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the DCPB dpi port type.
*
* ELEMENTS:
*    CPB_DCPB_DPI_PORT_SIFD1     - SIFD 1 DPI Port
*    CPB_DCPB_DPI_PORT_SIFD2     - SIFD 2 DPI Port
*    CPB_DCPB_DPI_PORT_CBRC      - CBRC DPI Port
*    CPB_DCPB_DPI_PORT_MAPOTN    - MAPOTN DPI Port
*    CPB_DCPB_DPI_PORT_ENET_LINE - ENET_LINE DPI Port
*    CPB_DCPB_DPI_PORT_ENET_SYS  - ENET_SYS DPI Port
*
*******************************************************************************/
typedef enum
{
    /* Enum values map to physical device resources */
    CPB_DCPB_DPI_PORT_SIFD1      = 0,
    CPB_DCPB_DPI_PORT_SIFD2      = 1,
    CPB_DCPB_DPI_PORT_CBRC       = 2, 
    CPB_DCPB_DPI_PORT_MAPOTN     = 3,
    CPB_DCPB_DPI_PORT_ENET_LINE  = 4,
    CPB_DCPB_DPI_PORT_ENET_SYS   = 5,
    
    LAST_CPB_DCPB_INGRESS_PORT
}cpb_dcpb_dpi_port_type_t;

/*******************************************************************************
* ENUM: cpb_ocpb_dpi_port_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the OCPB dpi port type.
*
* ELEMENTS:
*   CPB_OCPB_DPI_PORT_SIFD1     - OCPB SIFD 1 DPI Port
*   CPB_OCPB_DPI_PORT_SIFD2     - OCPB SIFD 2 DPI Port
*   CPB_OCPB_DPI_PORT_ODUK_LINE - OCPB LINE (FO1) DPI Port
*   CPB_OCPB_DPI_PORT_ODUK_SYS  - OCPB LINE (FO2) DPI Port
*
*******************************************************************************/
typedef enum
{
    /* Enum values map to physical device resources */
    CPB_OCPB_DPI_PORT_SIFD1         = 0,
    CPB_OCPB_DPI_PORT_SIFD2         = 1,
    CPB_OCPB_DPI_PORT_ODUK_LINE     = 2, 
    CPB_OCPB_DPI_PORT_ODUK_SYS      = 3,
    
    LAST_CPB_OCPB_INGRESS_PORT
}cpb_ocpb_dpi_port_type_t;

/*******************************************************************************
* ENUM: cpb_oduksw_port0_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This enumerated type indicates where ODUKSW port0 is muxed 
*
* ELEMENTS:
*    CPB_ODUKSW_PORT0_COREOTN_CTRL - Port 0 usage in SYSOTN mode of operation
*    CPB_ODUKSW_PORT0_ILKN         - Port 0 usage in non-SYSOTN mode (SIFD 1)
*
*******************************************************************************/
typedef enum {
    CPB_ODUKSW_PORT0_COREOTN_CTRL = 0,
    CPB_ODUKSW_PORT0_ILKN = 2,
    LAST_CPB_ODUKSW_PORT0 
    } cpb_oduksw_port0_t;


/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

typedef struct cpb_handle_struct_t cpb_handle_t; /*!< Opaque reference to module handle */

/*******************************************************************************
* STRUCTURE: cpb_int_chnl_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CPB level channel table for interrupt and status.
*   Note: CPB interrupts cannot be enabled for interrupt mode. CPB interrupts
*         are only available at interrupt/status retrieval time.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    BOOL8   ch_drop_ovrsize_i;
    BOOL8   ch_drop_client_i;
    BOOL8   ch_drop_full_i;
} cpb_int_chnl_t;


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
#define CPB_LOG_ERR_TABLE \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_LOG_CODE_DEBUG,    "CPB: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_CODE_ASSERT,   "CPB: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_NULL_HANDLE,      "CPB: NULL Handle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_LOG_CODE_LOG,      "CPB: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_NO_SW_CONTENT, "CPB: Function Not Coded" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_INVALID_DEVICE_STATE,   "CPB: Invalid device state read" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_INVALID_ARG,   "CPB: Err Invalid Argument" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_CTXT_NOT_INITIALIZED,   "CPB: Context not Initialized" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_INVALID_CPB_TYPE,   "CPB: Invalid CPB Instantiation Type Specified" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_NO_RD_QID,   "CPB: Read Queue ID Unavailable" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_DIVIDED_BY_ZERO,   "CPB: Division by 0 attempted" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_CHANNEL_NOT_FOUND,   "CPB: Channel data not found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_INVALID_VAR_T_RAM_DATA_SIZE, "CPB: backup memory var_t size is not expected size" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_PMPM_PAGE_UNAVAILABLE,   "CPB: Not Enough PMPM Pages" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_PMPM_PAGE_UNRELEASEABLE,   "CPB: Can't Release PMPM Pages" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_PMPM_BLOCK_UNAVAILABLE,   "CPB: Not Enough Blocks" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_QUEUE_NOT_BUILT,   "CPB: Queue Not Built" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_QUEUE_ALREADY_EXSITS,   "CPB: Queue Already Exist" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_END_ADDR_UPDATE_INCOMPLETE,   "CPB: End Address Update Incomplete" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_ADDR_CHG_ABORT,   "CPB: End Address Update Aborted" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_WRR_PAGE_SWAP,   "CPB: DCS WRR Page Swap Failure" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_MAPPING_DATA_EXTRACT_FAIL,   "CPB: Retrieval of CPB Mapping Context Failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_POLL_FAIL,   "CPB: Poll failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_NO_DCS_CH_FOUND,   "CPB: Couldn't find a matching channel in the DCS" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_DEACT_PRIMARY_WITH_SECONDARY,   "CPB: Primary Channel Not Able to Deactivate") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_CHANNEL_ALLOC_AGAINST_DCS,   "CPB: Channel Init State is Not Start") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_CHNL_ALREADY_EXISTS,   "CPB: Channel Already Exist" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_CHANNEL_NOT_OPERATIONAL,   "CPB: The Channel is NOT in Operational State" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_CHANNEL_NOT_EQUIPPED,   "CPB: The Channel is NOT in Equipped State" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_CHNL_RATE_INCORRECT,   "CPB: The Rate for the Requested Channel is not Correct" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_ZONE_SOURCE_NOT_FOUND,   "CPB: Zone Source Not Found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_MEM_POOL_NOT_AVAILABLE,   "CPB: Memory Pool is already in use. Cannot assign memory" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_MEM_POOL_AVAILABLE,   "CPB: Memory Pool not in use. No need to release memory" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_DB_ATTACH_FAIL,   "CPB: DB Attachment Operation Failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_UNSUPPORTED_SCHD_MODE,   "CPB: Scheduler Mode Does not Support Database" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_INVALID_EGR_MUX_PORT_MAPPING,   "CPB: Egress Mux Input/Output Port Combination Invalid" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_INVALID_IGR_MUX_PORT_MAPPING,   "CPB: Ingress Mux Input/Output Port Combination Invalid" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_CHNL_OUT_RANGE,   "CPB: Channel Id out of Range" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_DB_ENTRY_NOT_FOUND,  "CPB: DB entry not found" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_INVALID_DB_MODE,  "CPB: Invalid scheduler DB mode" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_CHANNEL_INVALID_STATE,  "CPB: Invalid channel state" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_THRESHOLD_EXCEED_BUFFER_SIZE,  "CPB: Threshold values exceed allocate buffer" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_CONTEXT_INCOHERENT,  "CPB: Context and Register Space Incoherent" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_NEEDS_RESET,  "CPB: Crash restart message - CPB should be reset" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_REMAP_ABORTED,  "CPB: REMAP_REQ did not complete, so ABORTED" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_LOG_IBN_LEN_SEQ_NUM,                       "CPB: info status get inbound_msg_len and inbound_seq_num" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_LOG_OBND_DATA,                             "CPB: outbound data" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(CPB_ERR_NO_BACKUP_SETUP,                       "CPB: fault replacement backup path not provisioned" ) \







/*
 * The following creates the ERR/LOG indexes from above by pulling out the enum
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum CPB_LOG_ERR_INDEX
{
    CPB_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI_CPB << 16),
    CPB_LOG_ERR_TABLE
    CPB_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#ifdef PMC_LOG_STRINGS
extern const char CPB_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else /* no strings */
extern const char CPB_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */

#ifdef __cplusplus
}
#endif

#endif /* _CPB_API_H */

/*
** end of file
*/
