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
*   DESCRIPTION :  This file contains definitions for C functions for
*   configuring ENET features.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _DIGI_ENET_API_H
#define _DIGI_ENET_API_H

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
* ENUM: digi_enet_classification_direction_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Enet classification modes.
*
* ELEMENTS:
*       DIGI_ENET_INGRESS              - extract in ingress direction
*       DIGI_ENET_EGRESS               - extract in egress direction
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_EGRESS                  = 0,
    DIGI_ENET_INGRESS                 = 1,
    DIGI_ENET_MAX_NUM
} digi_enet_classification_direction_mode_t;

/*******************************************************************************
* ENUM: digi_enet_tx_timing_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Ethernet timing modes
*
* ELEMENTS:
*   DIGI_ENET_TX_LOOP_TIMING_MODE    - Loop timing is where each transmit data\n
*                                      stream is timed from its partner receive\n
*                                      data stream (see hardware spec. 9.10.1)
*   DIGI_ENET_TX_NOMINAL_TIMING_MODE - External timing is where the transmit data\n
*                                      stream is timed from an external source \n
*                                      (see hardware spec. sect. 9.10.3)
*   DIGI_ENET_TX_LINE_TIMING_MODE    - Line Timing is where a transmit data stream\n
*                                      is timed with the clock from any active \n
*                                      receive data stream in the system or the \n
*                                      PTP clock.(see hardware spec. 9.10.4)
*   DIGI_ENET_TX_THROUGH_TIMING_MODE - Through Timing is where a transparent \n
*                                      transmit data stream is timed with its \n
*                                      original clock (see hardware spec.  9.10.2)
*
*******************************************************************************/
typedef enum digi_enet_tx_timing_mode_t
{
   DIGI_ENET_TX_LOOP_TIMING_MODE = 0,  
   DIGI_ENET_TX_NOMINAL_TIMING_MODE,
   DIGI_ENET_TX_LINE_TIMING_MODE,
   DIGI_ENET_TX_THROUGH_TIMING_MODE,
   LAST_DIGI_ENET_TX_TIMING_MODE
} digi_enet_tx_timing_mode_t;

/*******************************************************************************
* ENUM: digi_enet_tx_mac_fault_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Transmit MAC fault insertion behavior.\n\n
*
*   The value of elements is encoded as bit 2 FLT_HDL_DIS setting, bit 1 SEND_RF
*   setting and bit 0 SEND_LF setting.\n\n
*
* ELEMENTS:
*   DIGI_ENET_TX_MAC_FAULT_AUTOMATIC      - Enable ingress fault egress consequential action
*   DIGI_ENET_TX_MAC_FAULT_ILLEGAL_1      - Do not use illegal setting
*   DIGI_ENET_TX_MAX_FAULT_ILLEGAL_2      - Do not use illegal setting
*   DIGI_ENET_TX_MAX_FAULT_ILLEGAL_3      - Do not use illegal setting
*   DIGI_ENET_TX_MAC_FAULT_DISABLED       - Disable ingress fault egress consequential action
*   DIGI_ENET_TX_MAC_FAULT_FORCE_LF       - Force LF condition
*   DIGI_ENET_TX_MAC_FAULT_FORCE_RF       - Force RF condition
*   DIGI_ENET_TX_MAC_FAULT_UNIDIRECTIONAL - Unidirectional ENET Clause 66 compliant
*
*******************************************************************************/
typedef enum digi_enet_tx_mac_fault_t
{
   DIGI_ENET_TX_MAC_FAULT_AUTOMATIC = 0,
   DIGI_ENET_TX_MAC_FAULT_ILLEGAL_1 = 1,
   DIGI_ENET_TX_MAC_FAULT_ILLEGAL_2 = 2,
   DIGI_ENET_TX_MAC_FAULT_ILLEGAL_3 = 3,
   DIGI_ENET_TX_MAC_FAULT_DISABLED  = 4,
   DIGI_ENET_TX_MAC_FAULT_FORCE_LF  = 5,
   DIGI_ENET_TX_MAC_FAULT_FORCE_RF  = 6,
   DIGI_ENET_TX_MAC_FAULT_UNIDIRECTIONAL = 7,
   LAST_DIGI_ENET_TX_MAC_FAULT
} digi_enet_tx_mac_fault_t;

/*******************************************************************************
* ENUM: digi_enet_classification_type_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Enet classification modes.
*
* ELEMENTS:
*       DIGI_ENET_ALL_PACKETS          - extract all packets
*       DIGI_ENET_PTP_PACKETS          - extract PTP packets
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_ALL_PACKETS             = 0,
    DIGI_ENET_PTP_PACKETS,

} digi_enet_classification_type_mode_t;

/*******************************************************************************
* ENUM: digi_enet_irig_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for IRIG modes.
*
* ELEMENTS:
*       DIGI_ENET_IRIG_TOD             - custom PMC Time-Of-Day mode
*       DIGI_ENET_IRIG_B00X            - IRIG-B B00x mode
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_IRIG_TOD                = 0,
    DIGI_ENET_IRIG_B00X               = 1
} digi_enet_irig_mode_t;

/*******************************************************************************
* ENUM: digi_enet_pmon_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for ENET PMON modes which are provided on
*   behalf of ILKN or MAPOTN data streams.
*
* ELEMENTS:
*    DIGI_ENET_PMON_MODE_NONE             - No PMON configured.
*
*    DIGI_ENET_PMON_MODE_ILKN             - ENET PMON of ILKN path (ILKN to 
*                                           CPB). PMON of datastream entering
*                                           device via ILKN, monitored after 
*                                           transmit MAC in ENET subsystem.
*                                           Only Rx ENET PMON counters are 
*                                           valid.
*
*    DIGI_ENET_PMON_MODE_MAPPER_RX_LOCAL  - ENET PMON of demapper path (ODUKSW
*                                           to CPB). Unidirectional PMON of 
*                                           datastream going from ODUKSW to 
*                                           CPB, monitoring the demapped signal 
*                                           at MapOTN. Only Rx ENET PMON 
*                                           counters are valid. 
*                                           LOCAL indicates that MAPOTN
*                                           broadcasts to the CPB and to 
*                                           ENET_LINE (ETRANS).
*
*    DIGI_ENET_PMON_MODE_MAPPER_RX_REMOTE - ENET PMON of demapper path (ODUKSW
*                                           to CPB). Unidirectional PMON of 
*                                           datastream going from ODUKSW to 
*                                           CPB, monitoring the demapped signal
*                                           at MapOTN. Only Rx ENET PMON 
*                                           counters are valid.  
*                                           REMOTE indicates that the the CPB 
*                                           broadcasts to the companion ENET
*                                           subsystem.
*
*    DIGI_ENET_PMON_MODE_MAPPER_BIDIR     - Bidirectional ENET PMON of mapper 
*                                           path. Both Tx and Rx ENET PMON 
*                                           counters are valid.
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_PMON_MODE_NONE = 0,
    DIGI_ENET_PMON_MODE_ILKN,
    DIGI_ENET_PMON_MODE_MAPPER_RX_LOCAL,
    DIGI_ENET_PMON_MODE_MAPPER_RX_REMOTE,
    DIGI_ENET_PMON_MODE_MAPPER_BIDIR,

    LAST_DIGI_ENET_PMON_MODE,
} digi_enet_pmon_mode_t;

/*******************************************************************************
* ENUM: digi_enet_eclass_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerating available filter config modes for enet eclass.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_ECLASS_MODE_NO_MATCH_ALL      = 0,
    DIGI_ENET_ECLASS_MODE_NO_MATCH_CONFIG   = 1,
/*#ifdef PMC_INTERNAL*/
    DIGI_ENET_ECLASS_MODE_LINK_OAM          = 2,
    DIGI_ENET_ECLASS_MODE_SYNCE_SSM         = 3,
    DIGI_ENET_ECLASS_MODE_LACP              = 4,
    DIGI_ENET_ECLASS_MODE_MARKER            = 5,
    DIGI_ENET_ECLASS_MODE_MGMT_VLAN         = 6,
    DIGI_ENET_ECLASS_MODE_PTP               = 7,
    DIGI_ENET_ECLASS_MODE_PTP_ETH           = 8,
    DIGI_ENET_ECLASS_MODE_PTP_IPV4          = 9,
    DIGI_ENET_ECLASS_MODE_PTP_IPV6          = 10,
    DIGI_ENET_ECLASS_MODE_PTP_EVENT         = 11,
    DIGI_ENET_ECLASS_MODE_PTP_GENERAL       = 12,
    DIGI_ENET_ECLASS_MODE_PTP_SYNC_DLY      = 13,
    DIGI_ENET_ECLASS_MODE_PTP_MCAST_DA      = 14,
    DIGI_ENET_ECLASS_MODE_PTP_UCAST_DA      = 15,
    DIGI_ENET_ECLASS_MODE_PTP_NUCAST_DA     = 16,
    DIGI_ENET_ECLASS_MODE_ARP_IPV4          = 17,
    DIGI_ENET_ECLASS_MODE_NDP_IPV6          = 18,
    DIGI_ENET_ECLASS_MODE_PRBS              = 19,
    DIGI_ENET_ECLASS_MODE_NPRBS             = 20,
    DIGI_ENET_ECLASS_MODE_SLOW_BRIDGE       = 21,
    DIGI_ENET_ECLASS_MODE_SLOW_RSVD         = 22,
    DIGI_ENET_ECLASS_MODE_SLOW_GARP         = 23,
    DIGI_ENET_ECLASS_MODE_SLOW_TYP          = 24,
    DIGI_ENET_ECLASS_MODE_RESET             = 25,
    DIGI_ENET_ECLASS_MODE_PTP_BASE          = 26,
    DIGI_ENET_ECLASS_MODE_PTP_2VLAN_EXTRA   = 27,
    DIGI_ENET_ECLASS_MODE_PTP_1VLAN_9200    = 28,
/*#endif PMC_INTERNAL */
    DIGI_ENET_ECLASS_MODE_LLDP_DA           = 29,
    DIGI_ENET_ECLASS_MODE_LLDP_ET           = 30,
    LAST_DIGI_ENET_ECLASS_MODE
} digi_enet_eclass_mode_t;

/*******************************************************************************
* ENUM: digi_enet_eclass_target_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerating eclass filter routing destinations.
*
* ELEMENTS:
*    DIGI_ENET_ECLASS_TARGET_NA     - No routing info. Only used for some specific,
*                                     general eclass filter configs
*    DIGI_ENET_ECLASS_TARGET_DCPB   - Route packets to DCPB
*    DIGI_ENET_ECLASS_TARGET_CPUH   - Route packets to CPU
*    DIGI_ENET_ECLASS_TARGET_MGMTH  - Route packets to Management Port
*    DIGI_ENET_ECLASS_TARGET_TSTAMP - Route packets to
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_ECLASS_TARGET_NA = 0xFF,
    DIGI_ENET_ECLASS_TARGET_DCPB = 0,
    DIGI_ENET_ECLASS_TARGET_CPUH = 1,
    DIGI_ENET_ECLASS_TARGET_MGMTH = 2,
    DIGI_ENET_ECLASS_TARGET_TSTAMP = 5,
    LAST_DIGI_ENET_ECLASS_TARGET 
} digi_enet_eclass_target_t;


/*******************************************************************************
* ENUM: digi_enet_eclass_encap_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerating eclass encapsulation.
*
* ELEMENTS:
*   DIGI_ENET_ECLASS_ENCAP_NONE         - No encapsulation
*   DIGI_ENET_ECLASS_ENCAP_ETH          - Ethernet 
*   DIGI_ENET_ECLASS_ENCAP_UDP_IPV4_ETH - UDP over IPV4 over Ethernet
*   DIGI_ENET_ECLASS_ENCAP_UDP_IPV6_ETH - UDP over IPV6 over Ethernet
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_ECLASS_ENCAP_NONE = 0,
    DIGI_ENET_ECLASS_ENCAP_ETH = 1,
    DIGI_ENET_ECLASS_ENCAP_UDP_IPV4_ETH = 2,
    DIGI_ENET_ECLASS_ENCAP_UDP_IPV6_ETH = 4,
    LAST_DIGI_ENET_ECLASS_ENCAP
} digi_enet_eclass_encap_t;

/*******************************************************************************
* ENUM: digi_enet_eclass_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerating eclass datapath directions.
*
* ELEMENTS:
*   DIGI_ENET_ECLASS_DIR_EGRESS     - Apply filtering on Egress direction
*   DIGI_ENET_ECLASS_DIR_INGRESS    - Apply filtering on Ingress direction
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_ECLASS_DIR_EGRESS = 0,
    DIGI_ENET_ECLASS_DIR_INGRESS,
    LAST_DIGI_ENET_ECLASS_DIR
} digi_enet_eclass_dir_t;

/*******************************************************************************
* ENUM: digi_enet_pcs_alignment_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerating pcs alignment application.
*
* ELEMENTS:
*   DIGI_ENET_PCS_NO_ALIGN         - No alignment action taken
*   DIGI_ENET_PCS_ALIGN_OCCURED    - Alignment action taken
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_PCS_NO_ALIGN = 0,
    DIGI_ENET_PCS_ALIGN_OCCURED,
    LAST_DIGI_ENET_PCS
} digi_enet_pcs_alignment_status_t;

/*******************************************************************************
* ENUM: lldp_cbr_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for ECLASS's LLDP for c71/c73 CBR traffic.
*
* ELEMENTS:
*   DIGI_ENET_ECLASS_LLDP_CBR_NONE    - Link is not used
*   DIGI_ENET_ECLASS_LLDP_CBR_LINE    - LLDP packets are monitored in in ENET LINE
*   DIGI_ENET_ECLASS_LLDP_CBR_SYS     - LLDP packets are monitored in in ENET SYS
*******************************************************************************/
typedef enum digi_enet_lldp_cbr_cfg_t
{
    DIGI_ENET_ECLASS_LLDP_CBR_NONE   = 0,
    DIGI_ENET_ECLASS_LLDP_CBR_LINE   = 1,
    DIGI_ENET_ECLASS_LLDP_CBR_SYS    = 2,
    LAST_DIGI_ENET_ECLASS_LLDP_CBR
}digi_enet_lldp_cbr_cfg_t;

/*
** Constants
*/
#ifndef DOXYGEN_PUBLIC_ONLY 
/*******************************************************************************
* The path type determines which status & operations are available
* ______________________________________________________________________________
*
*                                                                Gsup43   Gsup43
*  Path type (digi_enet_mode_t)                            MAC    C6.2     C7.3
*  ----------------------------                            ---    ----     ----
*  SERDES 100GE MAC Terminated                              X
*  SERDES 100GE CBR                                                 X
*  SERDES 40GE MAC Terminated                               X
*  SERDES 40GE CBR                                                  X
*  SERDES 10GE MAC Terminated                               X
*  SERDES 10GE Gsup43 7.3                                                   X
*  SERDES 10GE CBR
*  MAPOTN 100GE GFP mapped Gsup 43 6.2 MAC Terminated       X       X
*  MAPOTN 100GE GMP mapped MAC Terminated                   X
*  MAPOTN 40GE GFP mapped Gsup 43 6.2 MAC Terminated        X       X
*  MAPOTN 40GE GMP mapped MAC Terminated                    X
*  MAPOTN 10GE Gsup43 7.3 GFP mapped MAC Terminated         X               X
*  MAPOTN 10GE Gsup43 6.2 GFP mapped MAC Terminated         X
*  MAPOTN 10GE Gsup43 7.1/7.2 BMP mapped MAC Terminated     X
*
*******************************************************************************/

/*******************************************************************************
*
*   Bit definitions for MAC status.
*
*******************************************************************************/
#define DIGI_ENET_MAC_LOS_TRANSITION      0x000001    /* PHY LOS transition */
#define DIGI_ENET_MAC_LOS_DETECTED        0x000002    /* PHY LOS detected */
#define DIGI_ENET_MAC_LF_TRANSITION       0x000004    /* LF transition */
#define DIGI_ENET_MAC_LF_DETECTED         0x000008    /* LF detected */
#define DIGI_ENET_MAC_RF_TRANSITION       0x000010    /* RF transition */
#define DIGI_ENET_MAC_RF_DETECTED         0x000020    /* RF detected */
#define DIGI_ENET_MAC_HBER_TRANSITION     0x000040    /* high BER transition */
#define DIGI_ENET_MAC_HBER_DETECTED       0x000080    /* high BER detected */
#define DIGI_ENET_MAC_LOCK_TRANSITION     0x000100    /* PCS lock transition to received blocks */
#define DIGI_ENET_MAC_NOT_LOCK_DETECTED   0x000200    /* PCS not locked to received blocks */
#define DIGI_ENET_MAC_ALIGN_TRANSITION    0x000400    /* multi-lane alignment transition (40G & 100G only) */
#define DIGI_ENET_MAC_NOT_ALIGN_DETECTED  0x000800    /* loss of multi-lane alignment (40G & 100G only) */
#define DIGI_ENET_MAC_PAUSE_RECEIVED      0x001000    /* egress is paused */
#define DIGI_ENET_MAC_LINK_UP             0x002000    /* link is up */
#define DIGI_ENET_MAC_AGB_EGR_OVERFLOW    0x004000    /* AGB egress overflow occurred */
#define DIGI_ENET_MAC_AGB_ING_OVERFLOW    0x008000    /* AGB ingress overflow occurred */
#define DIGI_ENET_MAC_MAC_EGR_OVERFLOW    0x010000    /* MAC egress overflow occurred */

/*******************************************************************************
*
*   Bit definitions for Gsup43 C6.2 status (40G & 100G).
*
*******************************************************************************/
#define DIGI_ENET_C62_HBER_TRANSITION     0x000001    /* high BER transition */
#define DIGI_ENET_C62_HBER_DETECTED       0x000002    /* high BER detected */
#define DIGI_ENET_C62_ERROR_BLOCK         0x000004    /* PCS error block detected */
#define DIGI_ENET_C62_INVALID_BLOCK       0x000008    /* PCS invalid block detected */
#define DIGI_ENET_C62_SKEW_VIOLATION      0x000010    /* skew range violation detected */
#define DIGI_ENET_C62_IDLE_TEST_PATTERN   0x000020    /* idle test pattern removed */
#define DIGI_ENET_C62_REMOTE_FAULT        0x000040    /* RF detected */
#define DIGI_ENET_C62_LOCAL_FAULT         0x000080    /* LF detected */
#define DIGI_ENET_C62_LOS_TRANSITION      0x000100    /* LOS transition */
#define DIGI_ENET_C62_LOS_DETECTED        0x000200    /* LOS detected */

/*******************************************************************************
*
*   Bit definitions for Gsup43 C7.3 status (10G).
*
*******************************************************************************/
#define DIGI_ENET_C73_LOS_TRANS          0x0000001    /* LOS transition */
#define DIGI_ENET_C73_LOS_DET            0x0000002    /* LOS detected */
#define DIGI_ENET_C73_RX_SYNC_TRANS      0x0000004    /* RX synchronization transition */
#define DIGI_ENET_C73_RX_SYNC_DET        0x0000008    /* RX synchronization in LOS */
#define DIGI_ENET_C73_HBER_TRANS         0x0000010    /* high BER transition */
#define DIGI_ENET_C73_HBER_DET           0x0000020    /* high BER detected */
#define DIGI_ENET_C73_LINK_FAIL_TRANS    0x0000040    /* link fail transition */
#define DIGI_ENET_C73_LINK_FAIL_DET      0x0000080    /* link fail detected */
#define DIGI_ENET_C73_LINK_FAULT_TRANS   0x0000100    /* link fault transition */
#define DIGI_ENET_C73_LINK_FAULT_DET     0x0000200    /* link fault detected */
#define DIGI_ENET_C73_BLOCK_ERROR        0x0000400    /* 64B/66B type field error detected */
#define DIGI_ENET_C73_PRBS31_ERROR       0x0000800    /* PRBS31 error detected */
#define DIGI_ENET_C73_1S_OR_0S_TRANS     0x0001000    /* input pattern of all 1's or 0's transition */
#define DIGI_ENET_C73_1S_OR_0S_DET       0x0002000    /* input pattern of all 1's or 0's detected */
#define DIGI_ENET_C73_LF_DETECTED        0x0004000    /* LF detected */
#define DIGI_ENET_C73_RF_DETECTED        0x0008000    /* RF detected */
#define DIGI_ENET_C73_FIFO_UNDERRUN      0x0010000    /* FIFO underrun occurred */             
#define DIGI_ENET_C73_FIFO_OVERRUN       0x0020000    /* FIFO overrun occurred */             
#define DIGI_ENET_C73_UPI_ERROR          0x0040000    /* UPI type not TX_PKT_TYPE or TX_OS_TYPE */             
#define DIGI_ENET_C73_SEND_RF            0x0080000    /* alarm to send RF */             
#define DIGI_ENET_C73_SEND_LF            0x0100000    /* alarm to send LF */             
#define DIGI_ENET_C73_SEND_IDLE          0x0200000    /* alarm to send Idle control characters */             
             
#endif /* DOXYGEN_PUBLIC_ONLY */

/* The number of ENET EMPEI extract PMONs */
#define DIGI_ENET_EXT_PMON_MAX 64
/*
** Macro Definitions
*/


/*
** Structures and Unions
*/



/*******************************************************************************
* STRUCTURE: digi_enet_pmon_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Channel table for ENET PMON
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    digi_pmon_enet_mib_t                     enet_mib;   
    digi_pmon_enet_errors_t                  enet_errors;
    digi_pmon_enet_eclass_t                  enet_eclass;
} digi_enet_pmon_chnl_t;

/*******************************************************************************
* STRUCTURE: digi_enet_to_mgmt_fege_extract_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Individual connection data for ENET to MGMT_FEGE packet extraction path.
*   Each channel may have multiple unique packet types identified for transfer
*   on the ENET to MGMT_FEGE packet extraction path.
*
* ELEMENTS:
*   valid                   - TRUE: the other field data is valid, otherwise
*                             the other fields are invalid.
*   pkt_type                - See util_global_pkt_type_t for details.
*   is_vlan_tagged          - TRUE: the vlan_src_cfg element is valid, otherwise
*                             it is invalid
*   vlan_src_cfg            - VLAN configuration data
*******************************************************************************/
typedef struct {
    BOOL8                          valid;
    util_global_pkt_type_t         pkt_type;
    BOOL8                          is_vlan_tagged;
    util_global_vlan_cfg_t         vlan_src_cfg;
} digi_enet_to_mgmt_fege_extract_cfg_t;


/*******************************************************************************
* STRUCTURE: digi_enet_pmon_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Aggregation table for ENET PMON
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    digi_enet_client_chnl_t       *handle[DIGI_ENET_CHNL_MAX];
    digi_enet_pmon_chnl_t          enet_chnl[DIGI_ENET_CHNL_MAX];
} digi_enet_pmon_t;



/*******************************************************************************
* STRUCTURE: digi_enet_int_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Channel table for ENET LINE/SYS interrupts.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    enet_int_chnl_t     enet;
    cpb_int_chnl_t      cpb;
} digi_enet_int_chnl_t;




/*******************************************************************************
* STRUCTURE: digi_enet_int_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Aggregation table for ENET LINE/SYS interrupts.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    digi_enet_client_chnl_t  *handle[DIGI_ENET_CHNL_MAX];
    digi_enet_int_chnl_t      enet_chnl[DIGI_ENET_CHNL_MAX];
} digi_enet_int_t;


/*******************************************************************************
* ENUM: digi_enet_int_summary_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to retrieve in digi_enet_int_summary_t.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    DIGI_ENET_INT_SUMMARY_MASK_LINE_SYS           = 0x01,
    DIGI_ENET_INT_SUMMARY_MASK_ACTIVE_CHNL_NUM    = 0x02,
    DIGI_ENET_INT_SUMMARY_MASK_ACTIVE_CHNL        = 0x04,
} digi_enet_int_summary_mask_t;


/*******************************************************************************
* STRUCTURE: digi_enet_int_summary_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Interrupt summary information for ENET channels
*
* ELEMENTS:
*   mask                    - digi_enet_int_summary_mask_t bitmask to
*                             indicate which fields have been returned
*
*   active_chnl_num         - Number of channels in ENET that have an active
*                             enabled interrupt
*
*   active_chnl             - Array of ENET channel handles that have an
*                             have an active enabled interrupt. This allows the user
*                             to iteratively call digi_enet_[line|sys]_int_chnl_filt_retrieve()
*                             with the provided channel handles.
*
*******************************************************************************/
typedef struct {
    UINT32                   mask;

    UINT32                   active_chnl_num;
    digi_enet_client_chnl_t *active_chnl[DIGI_ENET_CHNL_MAX];
} digi_enet_int_summary_t;


/*******************************************************************************
* STRUCTURE: digi_enet_test_packet_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining test packet format.
*
* ELEMENTS:
*       packet_size     - length in bytes
*       interval        - 12 cycles is minimum transmission opportunity at 10G
*       dest_addr_hi    - high-order 16 bits
*       dest_addr_lo    - low-order 32 bits
*       src_addr_hi     - high-order 16 bits
*       src_addr_lo     - low-order 32 bits
*       ether_type      - 16 bits for etherType
*       outer_vlan      - outer VLAN (0 indicates no VLAN)
*       inner_vlan      - inner VLAN (0 indicates no VLAN)
*       payload         - payload repeating data pattern
*
*******************************************************************************/
typedef struct digi_enet_test_packet_t
{
    UINT16 packet_size;
    UINT16 interval;
    UINT16 dest_addr_hi;
    UINT32 dest_addr_lo;
    UINT16 src_addr_hi;
    UINT32 src_addr_lo;
    UINT16 ether_type;
    UINT32 outer_vlan;
    UINT32 inner_vlan;
    UINT8 payload;
}digi_enet_test_packet_t;

/*******************************************************************************
* STRUCTURE: digi_enet_link_pause_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the properties of ENET link flow control 
*
* ELEMENTS:
*  pause_quanta - The pause quanta value sent in the PAUSE quanta field of a \n
*                 XOFF pause frame. A pause quanta is the time required to \n
*                 transmit 512 bits at the rate of the provisionned channel.
*  pause_quanta_thresh - pause quanta threshold for TX flow control \n
*                        When the quanta timer counts down and reaches this value,\n
*                        a refresh pause frame is sent with the programmed full \n
*                        quanta value if the pause condition persists. \n
*                        This value shall be set to more than the half of the\n
*                        provided pause quanta value
*  buffer_mode  - Define if flow control is controlled from internal or \n
*                 external resource 
*  xon_xoff_threshold - xon/xoff threshold that triggers the generation of\n
*                       XOFF/XON pause frames. This value is only used when\n
*                       buffer_mode is set to DIGI_ENET_FLOW_CONTROL_BUFFER_LOCAL
*                       The DCPB is organized into blocks of 12288 bytes each\n
*                       (256 bursts of 48 bytes), with a maximum of 512 blocks.\n
*                       This threshold is provided in term of burst of 48 bytes \n
*                       and it shall represent a level that is less than the \n
*                       size of the allocated buffer in DCPB..\n
*
*******************************************************************************/
typedef struct digi_enet_link_pause_cfg_t {
    UINT16 pause_quanta;
    UINT16 pause_quanta_thresh;
    digi_enet_flow_control_buffer_mode_t buffer_mode;
    UINT32 xon_xoff_threshold;
} digi_enet_link_pause_cfg_t;


/*******************************************************************************
* ENUM: enet_vlan_tag_type_enum 
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerating enet vlan tag.
*
* ELEMENTS:
*    See typedef
*
*******************************************************************************/
typedef enum enet_vlan_tag_type_enum
{
    ENET_VLAN_NO_TAG     = 0,
    ENET_VLAN_SINGLE_TAG = 1,
    ENET_VLAN_DOUBLE_TAG = 2,
    ENET_PTP_VLAN_TAG_TYPE
} enet_vlan_tag_type_enum;


/*******************************************************************************
* STRUCTURE: digi_enet_eclass_filter_config_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for eclass filter configuration options.
*
* ELEMENTS:
*    mode     - Filter mode to apply.
*    target   - Destination where filtered packets are routed.
*    encap    - Ethernet encapsulation to filter.
*    direction  - Ingress or Egress eclass to conifgure.
*    param_1 - Optional parameter for some config modes.
*    param_2 - Optional parameter for some config modes.
*
*******************************************************************************/
typedef struct digi_enet_eclass_filter_config_t {
    digi_enet_eclass_mode_t mode;
    digi_enet_eclass_target_t target;
    digi_enet_eclass_encap_t encap;
    digi_enet_eclass_dir_t direction;
    UINT32 param_1;
    UINT32 param_2;
    BOOL8 enable;
    enet_vlan_tag_type_enum vlan_tag_cfg;
    UINT16 vlan_tag_inner;
    UINT16 vlan_tag_outer;
    /*lldp_cbr_cfg_t lldp_cbr_cfg;*/
} digi_enet_eclass_filter_config_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR digi_enet_mru_get(digi_handle_t *digi_handle,
                                   util_global_switch_data_t *chnl_handle,
                                   UINT32 *length_ptr);
PUBLIC PMC_ERROR digi_enet_ipg_get(digi_handle_t *digi_handle,
                                   util_global_switch_data_t *chnl_handle,
                                   UINT32 *gap_ptr);
PUBLIC PMC_ERROR digi_enet_phy_status_get(digi_handle_t *digi_handle,
                                          util_global_switch_data_t *chnl_handle,
                                          BOOL *phy_enabled_ptr);
PUBLIC PMC_ERROR digi_enet_crc_forwarding_set(digi_handle_t *digi_handle,
                                              util_global_switch_data_t *chnl_handle,
                                              BOOL enable);
PUBLIC PMC_ERROR digi_enet_crc_forwarding_get(digi_handle_t *digi_handle,
                                              util_global_switch_data_t *chnl_handle,
                                              BOOL *enable);
PUBLIC PMC_ERROR digi_enet_insert_crc_set(digi_handle_t *digi_handle,
                                          util_global_switch_data_t *chnl_handle,
                                          BOOL enable);
PUBLIC PMC_ERROR digi_enet_insert_crc_get(digi_handle_t *digi_handle,
                                          util_global_switch_data_t *chnl_handle,
                                          BOOL *enable);
PUBLIC PMC_ERROR digi_enet_no_length_check_set(digi_handle_t *digi_handle,
                                              util_global_switch_data_t *chnl_handle,
                                              BOOL8 enable);
PUBLIC PMC_ERROR digi_enet_flow_control_status_get(digi_handle_t *digi_handle,
                                                   util_global_switch_data_t *chnl_handle,
                                                   BOOL *pause_received_ptr);

PUBLIC PMC_ERROR digi_enet_mru_set(digi_handle_t *digi_handle,
                                   util_global_switch_data_t *chnl_handle,
                                   UINT32 length);
PUBLIC PMC_ERROR digi_enet_ipg_set(digi_handle_t *digi_handle,
                                   util_global_switch_data_t *chnl_handle,
                                   UINT32 gap);
PUBLIC PMC_ERROR digi_enet_ins_mac_addr_set(digi_handle_t *digi_handle,
                                            util_global_switch_data_t *chnl_handle,
                                            BOOL8 tx_addr_ins_en,
                                            UINT32 mac_addr_msb,
                                            UINT32 mac_addr_lsb);
PUBLIC PMC_ERROR digi_enet_ins_mac_addr_get(digi_handle_t *digi_handle,
                                            util_global_switch_data_t *chnl_handle,
                                            BOOL8 *tx_addr_ins_en,
                                            UINT32 *mac_addr_msb,
                                            UINT32 *mac_addr_lsb);
PUBLIC PMC_ERROR digi_enet_loopback_set(digi_handle_t *digi_handle,
                                        util_global_switch_data_t *chnl_handle,
                                        BOOL enable);
PUBLIC PMC_ERROR digi_enet_send_test_pattern_set(digi_handle_t              *digi_handle,
                                                 util_global_switch_data_t  *chnl_handle,
                                                 BOOL                        tx_enable, 
                                                 BOOL                        rx_enable,
                                                 BOOL                        use_test_seed_b,
                                                 UINT32                      seed_msb, 
                                                 UINT32                      seed_lsb,
                                                 BOOL                        use_10g_square_wave,
                                                 BOOL                        use_10g_zeroes_data);

PUBLIC PMC_ERROR digi_enet_mon_prbs_set(digi_handle_t             *digi_handle,
                                        util_global_switch_data_t *chnl_handle,
                                        BOOL                       enable);

PUBLIC PMC_ERROR digi_enet_mon_prbs_get(digi_handle_t             *digi_handle,
                                        util_global_switch_data_t *chnl_handle,
                                        BOOL8                     *enable);

PUBLIC PMC_ERROR digi_enet_gen_prbs_set(digi_handle_t             *digi_handle,
                                        util_global_switch_data_t *chnl_handle,
                                        BOOL                       enable,
                                        util_patt_mode_t           prbs_type);

PUBLIC PMC_ERROR digi_enet_gen_prbs_get(digi_handle_t             *digi_handle,
                                        util_global_switch_data_t *chnl_handle,
                                        BOOL8                     *enable,
                                        util_patt_mode_t          *prbs_type);

PUBLIC PMC_ERROR digi_enet_mon_prbs_resync(digi_handle_t             *digi_handle,
                                           util_global_switch_data_t *chnl_handle);

PUBLIC PMC_ERROR digi_enet_prbs_error_cnt_get(digi_handle_t              *digi_handle,
                                              util_global_switch_data_t  *chnl_handle,
                                              util_patt_mode_t            prbs_type,
                                              UINT32                     *err_cnt);

PUBLIC PMC_ERROR digi_enet_prbs_status_get(digi_handle_t              *digi_handle,
                                           util_global_switch_data_t  *chnl_handle,
                                           util_patt_status_t         *status);

PUBLIC PMC_ERROR digi_enet_send_packets_set(digi_handle_t                *digi_handle,
                                            util_global_switch_data_t    *chnl_handle,
                                            BOOL                          enable,
                                            digi_enet_test_packet_t      *test_pkt_ptr);

PUBLIC PMC_ERROR digi_enet_send_packets_get(digi_handle_t *digi_handle,
                                            util_global_switch_data_t *chnl_handle,
                                            BOOL *enable,
                                            digi_enet_test_packet_t *test_pkt_ptr);

PUBLIC PMC_ERROR digi_enet_send_packets_status_get(digi_handle_t *digi_handle,
                                            util_global_switch_data_t *chnl_handle,
                                            BOOL *enabled_ptr);

PUBLIC PMC_ERROR digi_enet_phy_set(digi_handle_t *digi_handle,
                                   util_global_switch_data_t *chnl_handle,
                                   BOOL enable);
                                   
PUBLIC PMC_ERROR digi_enet_rx_emac_set(digi_handle_t *digi_handle,
                                      util_global_switch_data_t *chnl_handle,
                                      BOOL8 enable);
                                   
PUBLIC PMC_ERROR digi_enet_tx_mac_fault_insert_ctrl(digi_handle_t *digi_handle,
                                                    util_global_switch_data_t *chnl_handle,
                                                    digi_enet_tx_mac_fault_t mode);
PUBLIC PMC_ERROR digi_enet_tx_mac_fault_insert_ctrl_get(digi_handle_t *digi_handle,
                                                        util_global_switch_data_t *chnl_handle,
                                                        digi_enet_tx_mac_fault_t *mode_ptr);
PUBLIC PMC_ERROR digi_enet_idle_insert_set(digi_handle_t *digi_handle,
                                           util_global_switch_data_t *chnl_handle,
                                           BOOL idle_enable);
PUBLIC PMC_ERROR digi_enet_egress_squelch_set(digi_handle_t *digi_handle,
                                              util_global_switch_data_t *chnl_handle,
                                              BOOL enable, UINT16 pattern);
PUBLIC PMC_ERROR digi_enet_ingress_data_sink_set(digi_handle_t *digi_handle,
                                                 util_global_switch_data_t *chnl_handle,
                                                 BOOL enable);
PUBLIC PMC_ERROR digi_enet_irig_configuration_set(digi_handle_t *digi_handle,
                                                  util_global_switch_data_t *chnl_handle,
                                                  BOOL ingress_enable,
                                                  digi_enet_irig_mode_t ingress_mode,
                                                  BOOL egress_enable,
                                                  digi_enet_irig_mode_t egress_mode);

PUBLIC PMC_ERROR digi_enet_line_pmon_retrieve(digi_handle_t      *digi_handle,
                                              digi_enet_pmon_t   *pmon_table_ptr);

PUBLIC PMC_ERROR digi_enet_sys_pmon_retrieve(digi_handle_t      *digi_handle,
                                             digi_enet_pmon_t   *pmon_table_ptr);

PUBLIC PMC_ERROR digi_enet_pmon_chnl_get(digi_handle_t                *digi_handle,
                                         digi_enet_client_chnl_t      *chnl_handle,
                                         digi_enet_pmon_t             *pmon_table_ptr,
                                         digi_enet_pmon_chnl_t       **pmon_chnl_table_pptr);

PUBLIC PMC_ERROR digi_enet_enhanced_pmon_chnl_retrieve(digi_handle_t                  *digi_handle,
                                                       util_global_switch_data_t      *chnl_handle,
                                                       digi_enet_pmon_chnl_t          *pmon_table_ptr);

PUBLIC PMC_ERROR digi_enet_mgmt_fege_pmon_retrieve(digi_handle_t       *digi_handle,
                                                   digi_pmon_coreotn_fege_mstat_t    *pmon_table_ptr);

PUBLIC PMC_ERROR digi_enet_line_int_register_cb(digi_handle_t      *digi_handle,
                                                void               *token,
                                                UINT32              token_size,
                                                digi_int_cb_fcn     user_cb);

PUBLIC PMC_ERROR digi_enet_line_int_chnl_register_cb(digi_handle_t      *digi_handle,
                                                     void               *token,
                                                     UINT32              token_size,
                                                     digi_int_cb_fcn     user_cb);

PUBLIC PMC_ERROR digi_enet_line_int_chnl_enable(digi_handle_t             *digi_handle,
                                                digi_enet_client_chnl_t   *chnl_handle,   
                                                enet_int_chnl_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_line_int_chnl_disable(digi_handle_t             *digi_handle,
                                                 digi_enet_client_chnl_t   *chnl_handle,   
                                                 enet_int_chnl_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_line_int_clear(digi_handle_t             *digi_handle,
                                          digi_enet_int_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_line_int_chnl_clear(digi_handle_t             *digi_handle,
                                               digi_enet_client_chnl_t   *chnl_handle,
                                               digi_enet_int_chnl_t      *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_line_int_retrieve(digi_handle_t       *digi_handle,
                                             digi_enet_int_t     *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_line_int_filt_retrieve(digi_handle_t        *digi_handle,
                                                  digi_enet_int_chnl_t *filt_table_ptr,
                                                  digi_enet_int_t      *int_table_ptr);
  
PUBLIC PMC_ERROR digi_enet_int_chnl_filt_retrieve(digi_handle_t             *digi_handle,
                                                  digi_enet_client_chnl_t   *chnl_handle,   
                                                  digi_enet_int_chnl_t      *filt_table_ptr,
                                                  digi_enet_int_chnl_t      *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_int_chnl_get(digi_handle_t                *digi_handle,
                                        digi_enet_client_chnl_t      *chnl_handle,
                                        digi_enet_int_t              *int_table_ptr,
                                        digi_enet_int_chnl_t        **int_chnl_table_pptr);

PUBLIC PMC_ERROR digi_enet_line_int_enabled_get(digi_handle_t       *digi_handle,
                                                digi_enet_int_t     *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_line_int_enabled_check(digi_handle_t      *digi_handle);

PUBLIC PMC_ERROR digi_enet_line_int_chnl_enabled_check(digi_handle_t            *digi_handle,
                                                       digi_enet_int_summary_t  *int_summary_ptr);

PUBLIC PMC_ERROR digi_enet_line_int_summary_get(digi_handle_t              *digi_handle,
                                                UINT32                      summary_mask,
                                                digi_enet_int_summary_t    *int_summary_ptr);

PUBLIC PMC_ERROR digi_enet_sys_int_register_cb(digi_handle_t      *digi_handle,
                                               void               *token,
                                               UINT32              token_size,
                                               digi_int_cb_fcn     user_cb);

PUBLIC PMC_ERROR digi_enet_sys_int_chnl_register_cb(digi_handle_t      *digi_handle,
                                                    void               *token,
                                                    UINT32              token_size,
                                                    digi_int_cb_fcn     user_cb);

PUBLIC PMC_ERROR digi_enet_sys_int_chnl_enable(digi_handle_t             *digi_handle,
                                               digi_enet_client_chnl_t   *chnl_handle,   
                                               enet_int_chnl_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_sys_int_chnl_disable(digi_handle_t             *digi_handle,
                                                digi_enet_client_chnl_t   *chnl_handle,   
                                                enet_int_chnl_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_sys_int_clear(digi_handle_t             *digi_handle,
                                         digi_enet_int_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_sys_int_chnl_clear(digi_handle_t             *digi_handle,
                                              digi_enet_client_chnl_t   *chnl_handle,
                                              digi_enet_int_chnl_t      *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_sys_int_retrieve(digi_handle_t       *digi_handle,
                                            digi_enet_int_t     *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_sys_int_filt_retrieve(digi_handle_t        *digi_handle,
                                                 digi_enet_int_chnl_t *filt_table_ptr,
                                                 digi_enet_int_t      *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_sys_int_enabled_get(digi_handle_t       *digi_handle,
                                               digi_enet_int_t     *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_sys_int_enabled_check(digi_handle_t      *digi_handle);

PUBLIC PMC_ERROR digi_enet_sys_int_chnl_enabled_check(digi_handle_t            *digi_handle,
                                                      digi_enet_int_summary_t  *int_summary_ptr);

PUBLIC PMC_ERROR digi_enet_sys_int_summary_get(digi_handle_t              *digi_handle,
                                               UINT32                      summary_mask,
                                               digi_enet_int_summary_t    *int_summary_ptr);

PUBLIC PMC_ERROR digi_enet_rx_lane_info_get(digi_handle_t               *digi_handle,
                                            util_global_switch_data_t   *chnl_handle,
                                            UINT8 rxLaneId[20],
                                            UINT8 rxLaneSkewFifoAddr[20]);
                                                 
PUBLIC PMC_ERROR digi_enet_epmm_lf_os_insert_cfg(digi_handle_t *digi_handle,
                                                 util_global_switch_data_t *chnl_handle,
                                                 enet_dir_t direction,
                                                 BOOL8               enable);
PUBLIC PMC_ERROR digi_enet_epmm_lf_os_insert_cfg_get(digi_handle_t *digi_handle,
                                                 util_global_switch_data_t *chnl_handle,
                                                 enet_dir_t direction,
                                                 BOOL8               *enable);

PUBLIC PMC_ERROR digi_enet_epmm_lf_os_set_auto_insert_enable(digi_handle_t               *digi_handle,
                                                             util_global_switch_data_t   *chnl_handle,
                                                             enet_dir_t                   direction,
                                                             BOOL8                        enable);
PUBLIC PMC_ERROR digi_enet_epmm_lf_os_set_auto_insert_enable_get(digi_handle_t               *digi_handle,
                                                             util_global_switch_data_t   *chnl_handle,
                                                             enet_dir_t                   direction,
                                                             BOOL8                        *enable);
PUBLIC PMC_ERROR digi_enet_mgmt_pkt_ext_pmon_cfg(digi_handle_t *digi_handle,
                                                 util_global_switch_data_t *chnl_handle,        
                                                 digi_enet_classification_direction_mode_t direction,
                                                 UINT32 tag,
                                                 BOOL enable);

PUBLIC PMC_ERROR digi_enet_mgmt_pkt_ext_pmon_get(digi_handle_t *digi_handle, 
                                                 util_global_switch_data_t *chnl_handle,        
                                                 digi_enet_classification_direction_mode_t direction,
                                                 BOOL accumulate,
                                                 UINT32 **counters_pptr,
                                                 UINT8 **tags_pptr);

PUBLIC PMC_ERROR digi_enet_mgmt_pkt_ext_pmon_disable_all(digi_handle_t *digi_handle,
                                                 util_global_switch_data_t *chnl_handle,        
                                                 digi_enet_classification_direction_mode_t direction);

PUBLIC PMC_ERROR digi_enet_empei_pmon_latch(digi_handle_t *digi_handle,
                                                 util_global_switch_data_t *chnl_handle,        
                                                 digi_enet_classification_direction_mode_t direction);

PUBLIC PMC_ERROR digi_enet_mgmt_fege_init(digi_handle_t                 *digi_handle);

PUBLIC PMC_ERROR digi_enet_mgmt_fege_uninit(digi_handle_t                *digi_handle);

PUBLIC PMC_ERROR digi_enet_mgmt_fege_init_get(digi_handle_t *digi_handle,
                                              BOOL8 *enable);

PUBLIC PMC_ERROR digi_mgmt_fege_to_enet_pkt_fwd_cfg(digi_handle_t                 *digi_handle,
                                                    util_global_switch_data_t     *chnl_handle,
                                                    enet_dir_t                     enet_datapath,
                                                    util_global_vlan_cfg_t        *vlan_cfg_ptr,
                                                    BOOL8                          enable,
                                                    BOOL8                          overwrite_vlan_priority);

PUBLIC PMC_ERROR digi_enet_to_mgmt_fege_pkt_fwd_cfg(digi_handle_t                 *digi_handle,
                                                    util_global_switch_data_t     *chnl_handle,
                                                    enet_dir_t                     enet_datapath,
                                                    util_global_pkt_type_t         pkt_type,
                                                    BOOL8                          is_vlan_tagged,
                                                    util_global_vlan_cfg_t        *vlan_src_cfg_ptr,
                                                    BOOL8                          enable,
                                                    BOOL8                          vlan_id_repl_en,
                                                    UINT16                         vlan_id_repl);

PUBLIC PMC_ERROR digi_enet_to_mgmt_fege_pkt_fwd_cfg_single_get(digi_handle_t          *digi_handle,
                                                            util_global_switch_data_t *chnl_handle,
                                                            enet_dir_t                 enet_datapath,
                                                            util_global_pkt_type_t     pkt_type,
                                                            BOOL8                      is_vlan_tagged,
                                                            util_global_vlan_cfg_t    *vlan_src_cfg_ptr,
                                                            BOOL8                     *enable,
                                                            BOOL8                     *vlan_id_repl_en,
                                                            UINT16                    *vlan_id_repl);

PUBLIC PMC_ERROR digi_cpu_to_mgmt_fege_pkt_fwd_cfg(digi_handle_t                 *digi_handle,
                                                   util_global_vlan_cfg_t        *vlan_src_cfg_ptr,
                                                   BOOL8                          enable,
                                                   BOOL8                          vlan_id_repl_en,
                                                   UINT16                         vlan_id_repl);

PUBLIC PMC_ERROR digi_mgmt_fege_to_cpu_pkt_fwd_cfg(digi_handle_t *digi_handle,
                                                   util_global_vlan_cfg_t *vlan_cfg_ptr,
                                                   BOOL8 enable);

PUBLIC PMC_ERROR digi_mgmt_fege_to_cpu_pkt_fwd_cfg_get(digi_handle_t          *digi_handle,
                                                       util_global_vlan_cfg_t *vlan_cfg_ptr,
                                                       BOOL8                  *enable);

PUBLIC BOOL8 digi_enet_cpu_insert_pkt_cfg_get (digi_handle_t             *digi_handle,
                                               enet_dir_t                enet_datapath,
                                               util_global_switch_data_t *chnl_handle);

PUBLIC BOOL8 digi_enet_cpu_extract_pkt_cfg_get (digi_handle_t             *digi_handle,
                                                enet_dir_t                 enet_datapath,
                                                util_global_switch_data_t *chnl_handle,
                                                BOOL8                      is_reflect);

PUBLIC BOOL8 digi_cpu_to_mgmt_fege_pkt_fwd_cfg_get(digi_handle_t                 *digi_handle,
                                                   util_global_vlan_cfg_t        *vlan_src_cfg_ptr,
                                                   BOOL8                         *vlan_id_repl_en_ptr,
                                                   UINT16                        *vlan_id_repl_ptr);

PUBLIC BOOL8 digi_mgmt_fege_to_enet_pkt_fwd_cfg_get(digi_handle_t                 *digi_handle,
                                                    util_global_switch_data_t     *chnl_handle,
                                                    enet_dir_t                     enet_datapath,
                                                    util_global_vlan_cfg_t        *vlan_cfg_ptr,
                                                    BOOL8                         *overwrite_vlan_priority_ptr);

PUBLIC PMC_ERROR digi_mgmt_fege_to_10g_enet_pkt_fifo_size_cfg(digi_handle_t                 *digi_handle,
                                                              util_global_switch_data_t     *chnl_handle,
                                                              enet_dir_t                     enet_datapath,
                                                              BOOL8                          enable_9600b);

PUBLIC PMC_ERROR digi_10g_enet_pkt_to_mgmt_fege_fifo_size_cfg(digi_handle_t                 *digi_handle,
                                                              util_global_switch_data_t     *chnl_handle,
                                                              enet_dir_t                     enet_datapath,
                                                              BOOL8                          enable_9600b);

PUBLIC PMC_ERROR digi_mgmt_fege_to_enet_pkt_fwd_vlan_pri_cfg_get(digi_handle_t             *digi_handle,
                                                                 util_global_switch_data_t *chnl_handle,
                                                                 enet_dir_t                 enet_datapath,
                                                                 BOOL8                     *enable,
                                                                 util_global_vlan_cfg_t    *vlan_cfg_ptr,
                                                                 BOOL8                     *overwrite_vlan_priority_ptr);

PUBLIC BOOL8 digi_enet_to_mgmt_fege_pkt_fwd_cfg_get(digi_handle_t                 *digi_handle,
                                                    util_global_switch_data_t     *chnl_handle,
                                                    enet_dir_t                     enet_datapath,
                                                    digi_enet_to_mgmt_fege_extract_cfg_t  *extract_cfg_ptr,
                                                    BOOL8                         *vlan_id_repl_en_ptr,
                                                    UINT16                        *vlan_id_repl_ptr);

PUBLIC void digi_mgmt_fege_rx_flow_control_set(digi_handle_t          *digi_handle,
                                               BOOL8                   enable,
                                               BOOL8                   drop_pause_frm);

PUBLIC void digi_mgmt_fege_rx_flow_control_get(digi_handle_t          *digi_handle,
                                               BOOL8                  *enable,
                                               BOOL8                  *drop_pause_frm);

PUBLIC PMC_ERROR digi_mgmt_fege_frame_sizes_set(digi_handle_t          *digi_handle,
                                                UINT16                  txMaxFrameSize,
                                                UINT16                  txMinFrameSize,
                                                UINT16                  rxMaxFrameSize,
                                                UINT16                  rxMinFrameSize);

PUBLIC void digi_mgmt_fege_frame_sizes_get(digi_handle_t          *digi_handle,
                                           UINT16                 *txMaxFrameSize,
                                           UINT16                 *txMinFrameSize,
                                           UINT16                 *rxMaxFrameSize,
                                           UINT16                 *rxMinFrameSize);

PUBLIC PMC_ERROR digi_enet_mgmt_fege_int_enable(digi_handle_t             *digi_handle,  
                                                mgmt_fege_int_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_mgmt_fege_int_disable(digi_handle_t             *digi_handle, 
                                                 mgmt_fege_int_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_mgmt_fege_int_clear(digi_handle_t             *digi_handle,
                                               mgmt_fege_int_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_mgmt_fege_int_retrieve(digi_handle_t       *digi_handle,
                                                  mgmt_fege_int_t     *int_table_ptr);

PUBLIC PMC_ERROR digi_enet_tx_flow_control_set(digi_handle_t *digi_handle,
                                               util_global_switch_data_t *chnl_handle,
                                               digi_enet_link_pause_cfg_t *digi_enet_link_pause_cfg,
                                               BOOL8 enable);

PUBLIC PMC_ERROR digi_enet_rx_flow_control_set(digi_handle_t *digi_handle,
                                               util_global_switch_data_t *chnl_handle,
                                               BOOL8 enable);

PUBLIC PMC_ERROR digi_enet_client_source_cfg(digi_handle_t *digi_handle, util_global_switch_data_t *chnl_handle);


PUBLIC PMC_ERROR digi_enet_tx_flow_control_get(digi_handle_t *digi_handle,
                                               util_global_switch_data_t *digi_enet_client_chnl,                                                          
                                               digi_enet_link_pause_cfg_t *digi_enet_link_pause_cfg,
                                               BOOL8 *enable);

PUBLIC PMC_ERROR digi_enet_rx_flow_control_get(digi_handle_t *digi_handle,
                                               util_global_switch_data_t *digi_enet_client_chnl,                                   
                                               BOOL8 *enable);

PUBLIC PMC_ERROR digi_enet_cpu_insert_pkt_cfg (digi_handle_t             *digi_handle,
                                               enet_dir_t                enet_datapath,
                                               util_global_switch_data_t *chnl_handle,
                                               BOOL8                     enable);
PUBLIC PMC_ERROR digi_enet_cpu_extract_pkt_cfg (digi_handle_t             *digi_handle,
                                                enet_dir_t                enet_datapath,
                                                util_global_switch_data_t *chnl_handle,
                                                BOOL8                     enable,
                                                BOOL8                     is_reflect);

PUBLIC PMC_ERROR digi_enet_tx_timing_mode_set(digi_handle_t *digi_handle,
                                              digi_enet_client_chnl_t *digi_enet_client_chnl,
                                              digi_enet_tx_timing_mode_t timing_mode);
PUBLIC PMC_ERROR digi_enet_tx_timing_mode_get(digi_handle_t *digi_handle,
                                              digi_enet_client_chnl_t *digi_enet_client_chnl,
                                              digi_enet_tx_timing_mode_t *timing_mode);

PUBLIC PMC_ERROR digi_enet_emac_errored_frames_drop_cfg(digi_handle_t   *digi_handle,
                                                        BOOL8           enet_line,
                                                        emac_error_t    error_type,
                                                        BOOL8           enable);

PUBLIC PMC_ERROR digi_enet_emac_errored_frames_drop_get(digi_handle_t   *digi_handle,
                                                        BOOL8           enet_line,
                                                        emac_error_t    error_type,
                                                        BOOL8           *enabled);

PUBLIC PMC_ERROR digi_enet_emac_errored_frames_dpi_error_cfg(digi_handle_t   *digi_handle,
                                                             BOOL8           enet_line,
                                                             emac_error_t    error_type,
                                                             BOOL8           enable);

PUBLIC PMC_ERROR digi_enet_emac_errored_frames_dpi_error_get(digi_handle_t   *digi_handle,
                                                            BOOL8           enet_line,
                                                            emac_error_t    error_type,
                                                            BOOL8           *enabled);

PUBLIC PMC_ERROR digi_enet_emac_ingr_dpi_err_mask_cfg(digi_handle_t  *digi_handle,
                                                      util_global_switch_data_t *chnl_handle,
                                                      emac_dpi_err_cfg_t err_cfg);

PUBLIC PMC_ERROR digi_enet_emac_ingr_dpi_err_mask_get(digi_handle_t  *digi_handle,
                                                      util_global_switch_data_t *chnl_handle,
                                                      BOOL8   *fcs_mask_ptr,
                                                      BOOL8   *len_mask_ptr);

PUBLIC PMC_ERROR digi_enet_epmm_rf_os_insert_cfg(digi_handle_t              *digi_handle,
                                                 util_global_switch_data_t  *chnl_handle,
                                                 enet_dir_t                  direction,
                                                 BOOL8                       enable);
PUBLIC PMC_ERROR digi_enet_epmm_rf_os_insert_cfg_get(digi_handle_t              *digi_handle,
                                                 util_global_switch_data_t  *chnl_handle,
                                                 enet_dir_t                  direction,
                                                 BOOL8                      *enable);
PUBLIC PMC_ERROR digi_enet_epmm_rf_os_set_auto_insert_enable(digi_handle_t              *digi_handle,
                                                             util_global_switch_data_t  *chnl_handle,
                                                             enet_dir_t                  direction,
                                                             BOOL8                       enable);

PUBLIC PMC_ERROR digi_enet_tx_upi_set(digi_handle_t             *digi_handle,
                                      util_global_switch_data_t *chnl_handle,
                                      UINT8                      data_pkt_upi_val,
                                      UINT8                      os_pkt_upi_val,
                                      BOOL8                      enable_drop);

PUBLIC PMC_ERROR digi_enet_tx_upi_get(digi_handle_t             *digi_handle,
                                      util_global_switch_data_t *chnl_handle,
                                      UINT8                     *data_pkt_upi_val_ptr,
                                      UINT8                     *os_upi_val_ptr,
                                      BOOL8                     *enable_drop_ptr);
PUBLIC PMC_ERROR digi_enet_rx_upi_set(digi_handle_t             *digi_handle,
                                      util_global_switch_data_t *chnl_handle,
                                      UINT8                      data_pkt_upi_val,
                                      UINT8                      os_pkt_upi_val);

PUBLIC PMC_ERROR digi_enet_rx_upi_get(digi_handle_t             *digi_handle,
                                      util_global_switch_data_t *chnl_handle,
                                      UINT8                     *data_pkt_upi_val_ptr,
                                      UINT8                     *os_upi_val_ptr);

PUBLIC PMC_ERROR digi_enet_mac_tx_flush(digi_handle_t *digi_handle,
                                        util_global_switch_data_t *output_pc_ptr,
                                        BOOL8 action);
PUBLIC PMC_ERROR digi_enet_rx_pause_frame_fwd_get(digi_handle_t             *digi_handle,
                                                  util_global_switch_data_t *chnl_handle,
                                                  BOOL8                     *enabled_ptr);

PUBLIC PMC_ERROR digi_enet_rx_pause_frame_fwd_set(digi_handle_t             *digi_handle,
                                                  util_global_switch_data_t *chnl_handle,
                                                  BOOL8                      enable);

PUBLIC PMC_ERROR digi_enet_robust_dic_cfg(digi_handle_t             *digi_handle,
                                          util_global_switch_data_t *chnl_handle,
                                          BOOL8                      enable);

PUBLIC PMC_ERROR digi_enet_robust_dic_status_get(digi_handle_t             *digi_handle,
                                                 util_global_switch_data_t *chnl_handle,
                                                 BOOL8                      *enable_ptr);

PUBLIC PMC_ERROR digi_enet_inbound_fault_prov(digi_handle_t          *digi_handle,
                                              util_global_switch_data_t *chnl_handle,
                                              BOOL8 prov);

PUBLIC PMC_ERROR digi_enet_inbound_fault_force(digi_handle_t *digi_handle, util_global_switch_data_t *chnl_handle, BOOL force);

PUBLIC PMC_ERROR digi_enet_inbound_fault_type_cfg(digi_handle_t *digi_handle, util_global_switch_data_t *chnl_handle, BOOL fault_lf);

PUBLIC PMC_ERROR digi_enet_inbound_fault_type_pn_cfg(digi_handle_t *digi_handle, util_global_switch_data_t *chnl_handle, BOOL fault_pn11);
PUBLIC PMC_ERROR digi_enet_inbound_fault_state_get(digi_handle_t *digi_handle, 
                                                   util_global_switch_data_t *chnl_handle, 
                                                   BOOL8 *cfg_state_ptr,
                                                   BOOL8 *lf_type_ptr,
                                                   BOOL8 *active_state_ptr);

PUBLIC PMC_ERROR digi_enet_eclass_filter_config(digi_handle_t *digi_handle,
                                                util_global_switch_data_t *chnl_handle,
                                                digi_enet_eclass_filter_config_t cfg);

PUBLIC PMC_ERROR digi_enet_lpd_pkt_fetch(digi_handle_t *digi_handle,
                                         util_global_switch_data_t *chnl_handle,
                                         digi_enet_eclass_dir_t direct,
                                         UINT32 *pkt_addr);

PUBLIC PMC_ERROR digi_enet_lpd_release(digi_handle_t *digi_handle,
                                       util_global_switch_data_t *chnl_handle,
                                       digi_enet_eclass_dir_t direct);

PUBLIC PMC_ERROR digi_enet_lldp_cnts_get(digi_handle_t *digi_handle,
                                         util_global_switch_data_t *chnl_handle,
                                         digi_enet_eclass_dir_t direct,
                                         UINT32 *lldp_cnts_ptr,
                                         UINT32 *lldp_cnts_missed_ptr);

PUBLIC digi_enet_pcs_alignment_status_t digi_enet_pcs_block_alignment(digi_handle_t *digi_handle,
                                                                      digi_enet_pmon_t *enet_pmon_table,
                                                                      UINT64 pcs_threshold,
                                                                      UINT64 fcs_threshold);

PUBLIC PMC_ERROR digi_enet_lldp_config(digi_handle_t *digi_handle,
                                       util_global_switch_data_t * chnl_handle,
                                       digi_enet_eclass_encap_t packet_encap_type,
                                       digi_enet_eclass_mode_t lldp_filter_mode,
                                       UINT32 eth_type_field,
                                       digi_enet_eclass_dir_t direct,
                                       UINT32  enet_sys_tx_pmon);

PUBLIC PMC_ERROR digi_enet_transparent_lldp_set(digi_handle_t *digi_handle,
                                                util_global_switch_data_t * chnl_handle,
                                                BOOL8 enable);
PUBLIC util_global_switch_data_t*  digi_enet_lldp_companion_pmon_get(digi_handle_t *digi_handle,
                                                                    util_global_switch_data_t * chnl_handle,
                                                                    BOOL8 is_enhanced_pmon);
/*
** Deprecated functions
*/

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */ 

#ifdef PMC_DEPRECATED

/* LCOV_EXCL_START */

PUBLIC PMC_ERROR digi_enet_gsup43_c7point3_status_get(digi_handle_t *digi_handle,
                                                      util_global_switch_data_t *chnl_handle,
                                                      BOOL clear_interrupt,
                                                      UINT32 *status_ptr);
PUBLIC PMC_ERROR digi_enet_gsup43_c6point2_status_get(digi_handle_t *digi_handle,
                                                      util_global_switch_data_t *chnl_handle,
                                                      BOOL clear_interrupt,
                                                      UINT32 *status_ptr);

PUBLIC PMC_ERROR digi_enet_mac_status_get(digi_handle_t *digi_handle,
                                          util_global_switch_data_t *chnl_handle,
                                          BOOL clear_interrupt,
                                          UINT32 *status_ptr);

PUBLIC PMC_ERROR digi_enet_classification_set(digi_handle_t *digi_handle,
                                              util_global_switch_data_t *chnl_handle,
                                              digi_enet_classification_direction_mode_t direction,
                                              digi_enet_classification_type_mode_t type,
                                              BOOL enable);

PUBLIC PMC_ERROR digi_enet_egress_insert_rate_set(digi_handle_t *digi_handle,
                                                  util_global_switch_data_t *chnl_handle,
                                                  UINT16 insert_quantum, UINT16 system_quantum);

PUBLIC PMC_ERROR digi_enet_eclass_init(digi_handle_t *digi_handle,
                                       enet_dir_t    datapath);
/*
 * This function has been deprecated although not yet removed from the code 
 * space. Customer applications still call this function although its 
 * contents have been removed and it returns PMC_SUCCESS.
 */
PUBLIC PMC_ERROR digi_enet_client_source_cfg(digi_handle_t *digi_handle, util_global_switch_data_t *chnl_handle);
/* LCOV_EXCL_STOP */

#endif /* PMC_DEPRECATED */

#endif/*#ifndef DOXYGEN_PUBLIC_ONLY*/
    
#ifdef __cplusplus
}
#endif

#endif /* _DIGI_ENET_API_H */

/*
** end of file
*/
