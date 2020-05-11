/*******************************************************************************
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
* --------------------------------------------------------------------------
*   DESCRIPTION :
*     This file contains definitions for C functions for DIGI Timing and Frequency 
*     Synchronization: PTP and SyncE run time configuration.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _DIGI_TIMESYNC_API_H
#define _DIGI_TIMESYNC_API_H

#ifdef __cplusplus
extern "C" {
#endif
/*
** Include Files
*/
#include "stdint.h"
/*
** Enumerated Types
*/

/*
** Constants
*/
/* MAC Address Size, bytes */
#define ENET_MAC_ADDR_SIZE            6
/* PTP Master Clock Database, bytes */
#define PTP_MASTER_CLOCK_DB_SIZE      8

/* SyncE constants */
#define SYNCE_CHNL_MAX      DIGI_ENET_LINE_CHNL_MAX
#define SYNCE_QL_TLV_LEN    0x4
#define SYNCE_QL_TLV_TYPE   0x1
/* PTP */
#define PTP_PKT_FILTER_BYTE_SIZE 16

 /* PTP TS debug array size */
#define PTP_TS_DBG_ARRAY_SIZE                        30
/* PTP TS debug array maximum number */
#define PTP_TS_DBG_ARRAY_MAX_NUM                    (PTP_TS_DBG_ARRAY_SIZE - 1)

/*******************************************************************************
* ENUM: ptp_stability_status_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*   PTP algortihm stability status
*
* ELEMENTS:
*   PTP_STATUS_NOT_STABLE     - PTP algorithm is not stable
*   PTP_STATUS_STABLE         - PTP algorithm is stable
*
*******************************************************************************/
typedef enum ptp_stability_status_enum 
{
    PTP_STATUS_NOT_STABLE     = 0,
    PTP_STATUS_STABLE  = 1,
    LAST_PTP_STATUS
} ptp_stability_status_enum;

/*******************************************************************************
* ENUM: ptp_irig_adj_mode_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*   IRIG Adjustment Mode  
*
* ELEMENTS:
*   PTP_IRIG_ADJ_MODE_FREQ_AND_PHASE - Both frequency and phase are adjusted using IRIG
*   PTP_IRIG_ADJ_MODE_FREQ_ONLY      - Only frequency is adjusted using IRIG
*   PTP_IRIG_ADJ_MODE_PHASE_ONLY     - Only phase is adjusted using IRIG
*******************************************************************************/
typedef enum ptp_irig_adj_mode_enum
{
    PTP_IRIG_ADJ_MODE_FREQ_AND_PHASE = 0x0,
    PTP_IRIG_ADJ_MODE_FREQ_ONLY      = 0x1,
    PTP_IRIG_ADJ_MODE_PHASE_ONLY     = 0x2,
    LAST_PTP_IRIG_ADJ_MODE
} ptp_irig_adj_mode_enum;

/*******************************************************************************
* ENUM: ptp_irig_mode_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*   IRIG Adjustment Mode  
*
* ELEMENTS:
*   PTP_IRIG_MODE_1PPS_TOD - Clock, data and 1PPS TOD interface
*   PTP_IRIG_MODE_B_B00X   - IRIG-B B00x (PWM) format
*   
*******************************************************************************/
typedef enum ptp_irig_mode_enum
{
    PTP_IRIG_MODE_1PPS_TOD  = 0x0,
    PTP_IRIG_MODE_B_B00X    = 0x1,
    LAST_PTP_IRIG_MODE
} ptp_irig_mode_enum;

/*******************************************************************************
* ENUM: ptp_irig_port_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*   IRIG port (IRIGI or IRIGO)  
*
* ELEMENTS:
*   PTP_IRIG_PORT_IRIGI - Configure IRIGI port
*   PTP_IRIG_PORT_IRIGO - Configure IRIGO port
*   
*******************************************************************************/
typedef enum ptp_irig_port_enum
{
    PTP_IRIG_PORT_IRIGI  = 0x0,
    PTP_IRIG_PORT_IRIGO  = 0x1,
    PTP_IRIG_PORT_MAX_NUM
} ptp_irig_port_enum;

/*******************************************************************************
* ENUM: ptp_opt_mode_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*   PTP Operating Mode 
*   This type is used only for the Boundary Clock and Transparent clock operation.
*
* ELEMENTS:
*   PTP_PROCESS_OPT_MODE   - Device is configured to process PTP messsages
*                            and synchronize to the master.
*                            This mode is used in the Boundary Clock or
*                            Transparent Clock operation.
*   PTP_PASS_THRU_OPT_MODE - Device is configured to process timestamps and
*                            formward PTP messages to the external timing card to
*                            process PTP messages and synchronize to the master.*                            
*******************************************************************************/
typedef enum ptp_opt_mode_enum 
{
    PTP_PROCESS_OPT_MODE   = 0x00, 
    PTP_PASS_THRU_OPT_MODE = 0x01,
    LAST_PTP_OPT_MODE
} ptp_opt_mode_enum;

/*******************************************************************************
* ENUM: ptp_device_type_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*   PTP Device Type.
*   This type is used only for the Boundary Clock and Transparent clock operation.
*
* ELEMENTS:
*   PTP_DISABLED       - Device is not configured for PTP. Default value. 
*   PTP_BOUNDARY_CLOCK - Device is configured for the Boundary Clock operation
*   PTP_E2E_TC         - Device is configured for the End-to-end Transparent Clock 
*   PTP_P2P_TC         - Device is configured for thPeer-to-peer Transparent Clock 
*******************************************************************************/
typedef enum ptp_device_type_enum
{
    PTP_DISABLED       = 0,  
    PTP_BOUNDARY_CLOCK = 1,  
    PTP_E2E_TC         = 2,
    PTP_P2P_TC         = 3, 
    LAST_PTP_DEVICE_TYPE
} ptp_device_type_enum;

/*******************************************************************************
* ENUM: ptp_transport_protocol_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*   PTP Transport Protocol
*
* ELEMENTS:
*   PTP_OVER_UDP_OVER_IPV4      - transport PTP over UDP over IPv4 
*   PTP_OVER_UDP_OVER_IPV6      - transport PTP over UDP over IPv6
*   PTP_OVER_802_3              - transport PTP over IEEE 802.3/Ethernet
*******************************************************************************/
typedef enum ptp_transport_protocol_enum
{
    PTP_OVER_UDP_OVER_IPV4 = 0,
    PTP_OVER_UDP_OVER_IPV6 = 1,
    PTP_OVER_802_3         = 2,
    LAST_PTP_TRANSPORT_PROTOCOL_TYPE
} ptp_transport_protocol_enum;

/*******************************************************************************
* ENUM: ptp_vlan_tag_type_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*   PTP VLAN tag type.
*
* ELEMENTS:
*   PTP_VLAN_SINGLE_TAG      - Single VLAN tag 0x8100(IEEE_8021P)
*   PTP_VLAN_DOUBLE_TAG      - Double BLAN tag 0x88A8(IEEE_8021AD)
*******************************************************************************/
typedef enum ptp_vlan_tag_type_enum
{
    PTP_VLAN_NO_TAG     = 0,
    PTP_VLAN_SINGLE_TAG = 1,
    PTP_VLAN_DOUBLE_TAG = 2,
    LAST_PTP_VLAN_TAG_TYPE
} ptp_vlan_tag_type_enum;

/*******************************************************************************
* ENUM: ptp_pass_thru_msg_interface_enum
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Message Interface for Pass-Thru PTP
*
* ELEMENTS:
*   PTP_PASS_THRU_MSG_INTF_MGMT    - MGMT interface is message destination
*   PTP_PASS_THRU_MSG_INTF_CPB     - CPB is message destination
*   PTP_PASS_THRU_MSG_INTF_MAX_NUM - Last/max element in enum
*******************************************************************************/
typedef enum ptp_pass_thru_msg_interface_enum 
{
    PTP_PASS_THRU_MSG_INTF_MGMT = 0x0,
    PTP_PASS_THRU_MSG_INTF_CPB  = 0x1,
    PTP_PASS_THRU_MSG_INTF_MAX_NUM
} ptp_pass_thru_msg_interface_enum;

/** @brief PTP Slave mode: PTP Slave or IRIG Slave
 *  
  */
typedef enum 
{
    PTP_SLAVE_MODE_DISABLED    = 0x0,
    PTP_SLAVE_MODE_PTP         = 0x1,
    PTP_SLAVE_MODE_IRIG        = 0x2,
    PTP_SLAVE_MAX_NUM
} ptp_slave_mode_enum;
/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
#pragma pack(4)
/***************************************************************************
* STRUCTURE: ptp_config_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP configuration structure
*
* ELEMENTS:
*   reserved          - padding for the 32-bit alignment\n
*   twoStepFlag       - defaultDS.twoStepFlag\n
*                       This field is only used for Boundary Clock and 
*                       Transparent Clock operation.
*   dev_type          - Device Type\n
*                       1: Boundary clock\n
*                       2: End-to-end transparent clock\n
*                       3: Peer-to-peer transparent clock\n
*                       This field is only used for Boundary Clock and 
*                       Transparent Clock operation.
*   clockIdentity     - defaultDS.clockIdentity\n
*                       This field is only used for Boundary Clock and 
*                       Transparent Clock operation.
*   reserved1         - padding for the 32-bit alignment\n 
*   domainNumber      - defaultDS.domainNumber\n
*                       This field is only used for Boundary Clock and 
*                       Transparent Clock operation.
*   vlanTagPresent    - VLAN tagging of the PTP packets\n
*                       0: No VLAN tagging\n
*                       1: PTP packets tagged with the single VLAN tag\n
*                       2: PTP packets tagged with the double VLAN tag
*   optMode           - PTP Operating Mode\n
*                       0: Processing mode\n
*                       1: Pass-thru mode
*   multicastEnable   - TRUE: Transmit and listen to multicast PTP messages\n
*                       FALSE: Do not transmit or listen to multicast PTP messages\n
*                       This field is only used for Boundary Clock operation. 
*   transportProtocolBitmask - Transport Protocl for PTP message\n
*                       bit 0: Enable (1) or disable (0) PTP over UDP over IPv4\n
*                       bit 1: Enable (1) or disable (0) PTP over UDP over IPv6\n
*                       bit 2: Enable (1) or disable (0) PTP over IEEE 802.3 /Ethernet
*   slave_mode        - PTP Slave mode: 1, IRIG Slave mode - 2
*   frq_synt_rate     - Frequency syntonization rate:  0 - 2.9 ppm/sec G.253 requirement 
*   cfcModeMask       - Bitmask for CorrectionField Counter mode to use. 
*                       The following bits are used for each link rate:
*                       10G rate: bits 0 to 11\n
*                       40G rate: bits 0, 4, 8\n
*                       100G rate: bit 0\n
*                       Bits 0 to 11 correspond to channels 0 to 11 and 
*                       each bit value configures the hardware as follows:\n
*                       0: 48-bit nanosecond count.\n
*                       1: 47-bit nanosecond count. \n
*                       This field is only used for Transparent Clock and 
*                       Pass-Thru mode operation.
*   mstProtocol       - PTP BC Master Transport Protocol\n
*                       0: PTP over UDP over IPv4\n
*                       1: PTP over UDP over IPv6\n
*                       2: PTP over IEEE 802.3 /Ethernet  
****************************************************************************/
typedef struct ptp_config_t
{
#if defined(PMC_BIG_ENDIAN)
    UINT8  dev_type;
    UINT8  twoStepFlag;
    UINT8  reserved[2];
    UINT64 clockIdentity;
    UINT8  domainNumber;
    UINT8  reserved1[3];
    UINT8  transportProtocolBitmask;
    BOOL8  multicastEnable;
    UINT8  optMode;
    UINT8  vlanTagPresent;
    UINT16 cfcModeMask;
    UINT8 frq_synt_rate;
    UINT8 slave_mode;
    UINT8 mstProtocol[DIGI_ENET_LINE_CHNL_MAX];
#else
    UINT8  reserved[2];
    UINT8  twoStepFlag;
    UINT8  dev_type;
    UINT64 clockIdentity;
    UINT8  reserved1[3];
    UINT8  domainNumber; 
    UINT8  vlanTagPresent;
    UINT8  optMode; 
    BOOL8  multicastEnable;
    UINT8  transportProtocolBitmask;
    UINT8  slave_mode;
    UINT8  frq_synt_rate;
    UINT16 cfcModeMask;
    UINT8 mstProtocol[DIGI_ENET_LINE_CHNL_MAX];
#endif
} ptp_config_t;
#pragma pack()
/***************************************************************************
* STRUCTURE: ptp_irig_config_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP IRIG configuration structure
*
* ELEMENTS:
*   port  - IRIGI or IRIGO port
*   enable - Enable or disable IRIG port.
*   irig_mode - TOD or IRIG-B B00x mode.
*   adj_mode  - Adjust frequency, phase or both.
*   frq_synt_prd  - Frequency syntonization period, seconds.
*   frq_synt_rate - Frequency syntonization rate:  0 - 2.9 ppm/sec G.253 requirement
****************************************************************************/
typedef struct ptp_irig_config_t
{
      UINT8 port;
      BOOL8 enable;
      UINT8 irig_mode;
      UINT8 adj_mode;
      UINT8 frq_synt_prd;
      UINT8 frq_synt_rate;
} ptp_irig_config_t;

/***************************************************************************
* STRUCTURE: ptp_pkt_filtering_param_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP packet filtering parameters structure
*
* ELEMENTS:
*   protocol        - transport protocol for PTP message
*   channel_bitmask - bitmask of the channels that are used for the filtering
*   addr            - byte array that represents that the address to filter 
*                     this array is sized for the protocol with biggest 
*                     address. Valid bytes per protocol are:\n
*                        PTP_OVER_UDP_OVER_IPV4 : 4
*                        PTP_OVER_UDP_OVER_IPV6 : 16
*                        PTP_OVER_802_3         : 6
*   mask            - mask bytes are associated to addr bytes. Mask bytes \n
*                     allows to wild card any bits in the specified address.
*                     A true value (1) means that the associated addr bit \n
*                     will be user by the filter and a false value (0) .
****************************************************************************/
typedef struct ptp_pkt_filtering_param_t
{
    ptp_transport_protocol_enum protocol;
    UINT32               channel_bitmask;
    UINT8 addr[PTP_PKT_FILTER_BYTE_SIZE];
    UINT8 mask[PTP_PKT_FILTER_BYTE_SIZE];
} ptp_pkt_filtering_param_t;

/***************************************************************************
* STRUCTURE: ptp_ipv4_param_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP IPv4 parameters structure
*
* ELEMENTS:
*   typeOfService   - Type of Service field of the IPv4 header.\n
*                     Bits 7:5 map to the PRECEDENCE bits, and
*                     bits 4:2 map to the D, T and R bits of the field.
*   timeToLive      - Time to Live field of the IPv4 header.
*   sourceAddress   - 32-bit IPv4 Source Address.
*   mst_destAddress - 32-bit IPv4 Destination Address of the master.
*
****************************************************************************/
typedef struct ptp_ipv4_param_t
{
      UINT8  typeOfService;
      UINT8  timeToLive;
      UINT32 sourceAddress;
      UINT32 mst_destAddress;
} ptp_ipv4_param_t;

/***************************************************************************
* STRUCTURE: ptp_ipv6_param_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP IPv6 parameters structure
*
* ELEMENTS:
*   trafficClass  - Traffic Class field of the IPv6 header.
*   flowLabel     - 20-bit Flow Label field of the IPv6 header.
*                   Bits 19:0 map to the Flow Label field. Bits 23:20 are unused.
*   hopLimit      - Hop Limit field of the IPv6 header.
*   daScopeValue  - When using PTP over UDP over IPv6, the IPv6 Destination Address has the format
*                   FF0X:0:0:0:0:0:0:181. This field defines the 4-bit value for X. 
*                   See Table E.1 in IEEE 1588 - 2008.
*                   Only bits 3:0 are used. Bits 7:4 in this field are unused.
*   sourceAddress   - 128-bit IPv6 Source Address.
*   mst_destAddress - 128-bit IPv6 Destination Address of the master.
* 
* NOTES:
*   Convert flowLabel to little/big endian before using it
****************************************************************************/
typedef struct ptp_ipv6_param_t
{
      UINT8  trafficClass;
      UINT8  flowLabel[3];
      UINT8  hopLimit;
      UINT8  daScopeValue;
      UINT32 sourceAddress[4];
      UINT32 mst_destAddress[4];
} ptp_ipv6_param_t;

/***************************************************************************
* STRUCTURE: ptp_port_asymmetry_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP port asymmetry structure
*
* ELEMENTS:
*   chnl_handle         - pointer to an Ethernet channel handle
*   delayAsymmetry      - delayAsymmetry value for this port in ns. See section 7.4.2 of the std.
*   cfc_ingress_latency_delay - CFC Ingress latency delay in ns.
*   cfc_egress_latency_delay  - CFC Egress latency delay in ns.
*   tod_ingress_latency_delay - TOD Ingress latency delay in ns.
*   tod_egress_latency_delay  - TOD Egress latency delay in ns.
*
****************************************************************************/
typedef struct ptp_port_asymmetry_t
{
    util_global_switch_data_t *chnl_handle;
    INT32  delayAsymmetry;
    UINT16 cfc_ingress_latency_delay;
    UINT16 cfc_egress_latency_delay;
    UINT16 tod_ingress_latency_delay;
    UINT16 tod_egress_latency_delay;
} ptp_port_asymmetry_t;

/***************************************************************************
* STRUCTURE: ptp_rx_msg_cnts_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   Boundary Clock counters used for receipt PTP message
*
* ELEMENTS:
*   chnl_handle           - pointer to an Ethernet channel handle
*   syncCnt               - counter used for receipt Sync message
*   delayReqCnt           - counter used for receipt Delay_Req message
*   pdelayReqCnt          - counter used for receipt Pdelay_Req message
*   pdelayRespCnt         - counter used for receipt Pdelay_Resp message
*   followUpCnt           - counter used for receipt Follow_Up message
*   delayRespCnt          - counter used for receipt Delay_Resp message
*   pdelayRespFollowUpCnt - counter used for receipt Pdelay_Resp_Follow_Up message
*   announceCnt           - counter used for receipt Announce message
****************************************************************************/
typedef struct ptp_rx_msg_cnts_t
{
    util_global_switch_data_t *chnl_handle;
    UINT32 syncCnt;                               
    UINT32 delayReqCnt;                           
    UINT32 pdelayReqCnt;                          
    UINT32 pdelayRespCnt;                         
    UINT32 followUpCnt;                           
    UINT32 delayRespCnt;                          
    UINT32 pdelayRespFollowUpCnt;                 
    UINT32 announceCnt;                           
} ptp_rx_msg_cnts_t;

/***************************************************************************
* STRUCTURE: ptp_path_delay_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   Path delay of the specified chanel
*
* ELEMENTS:
*   chnl_handle          - Pointer to Ethernet channel handle
*   path_delay_msb       - 32 MSbits of the path delay
*   path_delay_lsb       - 32 LSbits of the path delay 
****************************************************************************/
typedef struct ptp_path_delay_t
{
    util_global_switch_data_t *chnl_handle;
    UINT32 path_delay_msb;
    UINT32 path_delay_lsb;
} ptp_path_delay_t;

/***************************************************************************
* STRUCTURE: ptp_default_properties_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP Boundary Clock mode default properties structure
*
* ELEMENTS:
*   clockClass     - defaultDS.clockQuality.clockClass
*   clockAccuracy  - defaultDS. clockQuality.clockAccuracy
*   offsetScaledLogVariance  - defaultDS. clockQuality.offsetScaledLogVariance
*   priority1      - defaultDS.priority1
*   priority2      - defaultDS.priority2
*   slaveOnly      - defaultDS.slaveOnly
****************************************************************************/
typedef struct ptp_default_properties_t
{
      UINT8  clockClass;
      UINT8  clockAccuracy;
      UINT16 offsetScaledLogVariance;
      UINT8  priority1;
      UINT8  priority2;
      UINT8  slaveOnly;
} ptp_default_properties_t;

/***************************************************************************
* STRUCTURE: ptp_set_time_param_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP time parameters structure
*
* ELEMENTS:
*   currentUtcOffset      - timePropertiesDS.currentUtcOffset:\n
*                             UINT8:
*                                   Bit 0:leap61\n
*                                   Bit 1:leap59\n
*                                   Bit 2:currentUtcOffsetValid\n
*                                   Bit 3:ptpTimescale\n
*                                   Bit 4:timeTraceable\n
*                                   Bit 5:frequencyTraceable\n
*                                   Bits 6 and 7 are unused.\n
*   timeSource            - timePropertiesDS.timeSource
****************************************************************************/
typedef struct ptp_set_time_param_t
{
    UINT16  currentUtcOffset;
#ifdef PMC_BITFIELD_MSB_FIRST
    UINT8   reserved:2;
    UINT8   frequencyTraceable:1;
    UINT8   timeTraceable:1;
    UINT8   ptpTimescale:1;
    UINT8   currentUtcOffsetValid:1;
    UINT8   leap59:1;
    UINT8   leap61:1;
#else
    UINT8   leap61:1;
    UINT8   leap59:1;
    UINT8   currentUtcOffsetValid:1;
    UINT8   ptpTimescale:1;
    UINT8   timeTraceable:1;
    UINT8   frequencyTraceable:1;
    UINT8   reserved:2;
#endif
    UINT8   timeSource;
} ptp_set_time_param_t;

/***************************************************************************
* STRUCTURE: ptp_acceptable_master_clocks_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP acceptable master clock structure
*
* ELEMENTS:
*   enableTable  - Enable Acceptable Master Table
*   clearTable   - Clear Acceptable Master Table
*   numEntries   - Number of Entries
****************************************************************************/
typedef struct ptp_acceptable_master_clocks_t
{
      UINT8   enableTable;
      UINT8   clearTable;
      UINT8   numEntries;
} ptp_acceptable_master_clocks_t;

/***************************************************************************
* STRUCTURE: ptp_acceptable_master_clocks_data_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP acceptable master clocks data structure
*
* ELEMENTS:
*   clockIdentity  - acceptable master clockIdentity
*   portNumber     - acceptable master portNumber
****************************************************************************/
typedef struct ptp_acceptable_master_clocks_data_t
{
      UINT64  clockIdentity;
      UINT16  portNumber;
} ptp_acceptable_master_clocks_data_t;

/***************************************************************************
* STRUCTURE: ptp_parent_data_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP Boundary clock mode parent data structure
*
* ELEMENTS:
*   stepsRemoved        - currentDS.stepsRemoved
*   parentClockIdentity - parentDS.parentPortIdentity.clockIdentity
*   parentPortNumber    - parentDS.parentPortIdentity.portNumber
*   grandmasterPriority1  - parentDS.grandmasterPriority1
*   grandmasterPriority2  - parentDS.grandmasterPriority2
*   grandmasterClockClass - parentDS.grandmasterClockQuality.clockClass
*   grandmasterClockAccuracy - parentDS.grandmasterClockQuality.clockAccuracy
*   grandmasterOffsetScaledLogVariance - parentDS.grandmasterClockQuality.offsetScaledLogVariance
*   grandmasterIdentity                - parentDS.grandmasterIdentity
*   unicastDA             - unicast destination MAC address
****************************************************************************/
typedef struct ptp_parent_data_t
{
      UINT16  stepsRemoved;
      UINT64  parentClockIdentity;
      UINT16  parentPortNumber;
      UINT8   grandmasterPriority1;
      UINT8   grandmasterPriority2;
      UINT8   grandmasterClockClass;
      UINT8   grandmasterClockAccuracy;
      UINT16  grandmasterOffsetScaledLogVariance;
      UINT64  grandmasterIdentity;
      UINT8   unicastDA[ENET_MAC_ADDR_SIZE];
} ptp_parent_data_t;


/***************************************************************************
* STRUCTURE: ptp_port_data_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP Boundary Clock mode port data structure
*
* ELEMENTS:
*   logMinDelayReqInterval  - portDS.logMinDelayReqInterval
*   logAnnounceInterval     - portDS.logAnnounceInterval
*   announceReceiptTimeout  - portDS.announceReceiptTimeout
*   logSyncInterval         - portDS.logSyncInterval
*   delayMechanism          - portDS.delayMechanism
*   logMinPdelayReqInterval - portDS.logMinPdelayReqInterval
*   qualificationTimeoutN   - qualificationTimeoutN
*   syncReceiptTimeout      - syncReceiptTimeout
*   delayRespReceiptTimeout - delayRespReceiptTimeout
****************************************************************************/
typedef struct ptp_port_data_t
{
      UINT8   logMinDelayReqInterval;
      UINT8   logAnnounceInterval;
      UINT8   announceReceiptTimeout;
      UINT8   logSyncInterval;
      UINT8   delayMechanism;
      UINT8   logMinPdelayReqInterval;
      UINT8   qualificationTimeoutN;
      UINT8   syncReceiptTimeout;
      UINT8   delayRespReceiptTimeout;
} ptp_port_data_t;

/***************************************************************************
* STRUCTURE: port_state_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP Boundary Clock mode port state structure
*
* ELEMENTS:
*   port_state     - Port State
*   ptsfStatus     - PSTF status of the requested port\n
*                    Bit [0]:'1' indicates that PTSF-lossSync is raised because of lacking of reception of PTP timing messages (Sync) from a master.\n
*                    Bit [1]:'1' indicates that PTSF-lossSync is raised because of lacking of reception of PTP timing messages (Delay_Resp) from a master. \n
*                    Bit [2]:'1' indicates that PTSF- lossAnnounce is raised because of lacking of reception of PTP Announce messages (Announce) from a master.\n
*                    Bit [3]:'1' indicates that PTSF- unusable is raised. It's currently unused.
*   announceReceiptTimeoutFlag  - Announce Receipt Timeout Flag. A '1' indicates
*                                 that the Announce Receipt Timeout has expired.
*                                 See section 9.2.6.11 of reference [6].
****************************************************************************/
typedef struct port_state_t
{
      UINT8   portState;
      UINT8   ptsfStatus; 
      UINT8   announceReceiptTimeoutFlag;
} port_state_t;

/***************************************************************************
* STRUCTURE: ptp_port_state_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   Struct that contains array of the PTP Boundary Clock mode port state structures
*
* ELEMENTS:
*   chnl_handle - Pointer to an array of Ethernet channel handles                      
*   state       - port_state_t structures that contain port_state and ptsfStatus
****************************************************************************/
typedef struct ptp_port_state_t
{
    util_global_switch_data_t *chnl_handle[DIGI_ENET_LINE_CHNL_MAX];
    port_state_t   state[DIGI_ENET_CHNL_MAX];
} ptp_port_state_t;

/***************************************************************************
* STRUCTURE: ptp_port_event_t
* __________________________________________________________________________
*
* DESCRIPTION:
*  PTP Boundary Clock mode port event structure
*
* ELEMENTS:
*   chnl_handle         - Pointer to Ethernet channel handle. 
*   enableFlag          - Enable Flag
****************************************************************************/
typedef struct ptp_port_event_t
{
      util_global_switch_data_t *chnl_handle;
      UINT8   enableFlag;
} ptp_port_event_t;

/***************************************************************************
* STRUCTURE: ptp_passive_port_parent_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP passive port parent structure
*
* ELEMENTS:
*   chnl_handle    - Pointer to Ethernet channel handle
*   clockIdentity  - sourcePortIdentity.clockIdentity
*   portNumber     - sourcePortIdentity.portNumber
****************************************************************************/
typedef struct ptp_passive_port_parent_t
{
      util_global_switch_data_t *chnl_handle;
      UINT64  clockIdentity;
      UINT16  portNumber;
} ptp_passive_port_parent_t;

/***************************************************************************
* STRUCTURE: ptp_rsp_master_clock_db_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP master clock database status structure
*
* ELEMENTS:
*   status        - Master Clock database status
*   num_entries   - Master Clock database number of entries.
****************************************************************************/
typedef struct ptp_master_clock_db_status_t
{
      UINT8   status;
      UINT8   num_entries;
} ptp_master_clock_db_status_t;

/***************************************************************************
* STRUCTURE: ptp_master_clock_db_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP master clock database structure
*
* ELEMENTS:
*   responseSize             - PTP Master Clock Database size in 32-bit words
*   responseAddress          - PTP Master Clock Database address
*   grandMasterIndentity     - PTP Master Clock Database grand master identities
*   sourcePortClockIdentity  - PTP Master Clock Database source port clock identities
*   srcPortNumber            - PTP Master Clock Database source port numbers
*   timeProperties           - PTP Master Clock Database time properties
*   timeSource               - PTP Master Clock Database time source
*   domainNumber             - PTP Master Clock Database domain number
*   grandmasterOffsetScaledLogVariance - PTP Master Clock Database grand master offset scaled log variance
*   stepsRemoved             - PTP Master Clock Database steps removed 
*   grandmasterPriority1     - PTP Master Clock Database grand master Priority1
*   grandmasterPriority2     - PTP Master Clock Database grand master Priority2
*   grandmasterClockClass    - PTP Master Clock Database grand master Clock Class 
*   grandmasterClockAccuracy - PTP Master Clock Database grand master Clock Accuracy
****************************************************************************/
typedef struct ptp_master_clock_db_t
{
      UINT16  responseSize;
      UINT32  responseAddress;
      UINT8   grandMasterIndentity[PTP_MASTER_CLOCK_DB_SIZE];
      UINT8   sourcePortClockIdentity[PTP_MASTER_CLOCK_DB_SIZE];
      UINT8   srcPortNumber[2];
      UINT8   timeProperties;
      UINT8   timeSource;
      UINT8   domainNumber;
      UINT16  grandmasterOffsetScaledLogVariance;
      UINT16  stepsRemoved;
      UINT8   grandmasterPriority1;
      UINT8   grandmasterPriority2;
      UINT8   grandmasterClockClass;
      UINT8   grandmasterClockAccuracy;
} ptp_master_clock_db_t;

/***************************************************************************
* STRUCTURE: ptp_start_init_param_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP Start Initialization parameters structure
*
* ELEMENTS:
*   chnl_handle             - Pointer to an array of Ethernet channel handles                      
*   outerVlanTag            - Outer VLAN Tag 
*   innerVlanTag            - Inner VLAN Tag
*   ptpChnlBitmask          - PTP ENET channel bitmask\n
*                             10G rate: bits 0 to11\n
*                             40G rate: bits 0, 4, 8\n
*                             100G rate: bit 0
*   resetEnetChannelMask    - reset ENET channel mask flag
****************************************************************************/
typedef struct ptp_start_init_param_t 
{
      util_global_switch_data_t *chnl_handle[DIGI_ENET_LINE_CHNL_MAX];
      UINT16 outerVlanTag;
      UINT16 innerVlanTag;
      UINT16 ptpChnlBitmask;
      UINT8 resetEnetChannelMask;
} ptp_start_init_param_t;


/***************************************************************************
* STRUCTURE: ptp_master_unicast_param_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP master unicast parameters structure
*
* ELEMENTS:
*   logInterMessagePeriod_* - log base 2 of the maximum rate in seconds that can be granted for the specified messageType.
*   durationField_*         - Maximum number of seconds for which messageType will be transmitted.
*
****************************************************************************/
typedef struct ptp_master_unicast_param_t 
{
      UINT8  logInterMessagePeriod_announce;
      UINT8  logInterMessagePeriod_sync;
      UINT8  logInterMessagePeriod_delay_resp;
      UINT32 durationField_announce;
      UINT32 durationField_sync;
      UINT32 durationField_delay_resp;
} ptp_master_unicast_param_t;

/***************************************************************************
* STRUCTURE: ptp_unicast_request_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP unicast request parameters structure
*
* ELEMENTS:
*   messageType             - PTP message type to configure
*   logInterMessagePeriod_* - log base 2 of the message rate in seconds to be requested for the specified messageType.
*   durationField_*         - Number of seconds to request for the specified messageType.
*   chnl_handle             - Pointer to Ethernet channel handle.
*   continuous              - TRUE: Repeat the request continuously.\n
*                             FALSE: Transmit the request once.
*   individual_tlv          - TRUE: Send TLVs individually.\n
*                             FALSE: Send TLVs together in the same Signalling message.
*
****************************************************************************/
typedef struct ptp_unicast_request_t 
{
      UINT8  messageType;
      UINT8  logInterMessagePeriod_announce;
      UINT8  logInterMessagePeriod_sync;
      UINT8  logInterMessagePeriod_delay_resp;
      UINT32 durationField_announce;
      UINT32 durationField_sync;
      UINT32 durationField_delay_resp;
      util_global_switch_data_t *chnl_handle;
      UINT8  continuous;
      BOOL8  individual_tlv;
} ptp_unicast_request_t;

/***************************************************************************
* STRUCTURE: ptp_unicast_cancel_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP unicast slave cancel structure
*
* ELEMENTS:
*   messageType        - PTP message type to configure
*   chnl_handle        - Pointer to Ethernet channel handle.
*
****************************************************************************/
typedef struct ptp_unicast_cancel_t 
{
      UINT8  messageType;
      util_global_switch_data_t *chnl_handle;
} ptp_unicast_cancel_t;

/***************************************************************************
* STRUCTURE: ptp_get_unicast_slave_state_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP unicast slave parameters structure
*
* ELEMENTS:
*   logInterMessagePeriod_* - log base 2 of the message rate in seconds that is granted for the specified messageType.
*   durationField_*         - Number of seconds granted for the specified messageType.
*   chnl_handle             - Pointer to Ethernet channel handle.
****************************************************************************/
typedef struct ptp_slave_unicast_param_t 
{
      UINT8  logInterMessagePeriod_announce;
      UINT8  logInterMessagePeriod_sync;
      UINT8  logInterMessagePeriod_delay_resp;
      UINT32 durationField_announce;
      UINT32 durationField_sync;
      UINT32 durationField_delay_resp;
      util_global_switch_data_t *chnl_handle;
} ptp_slave_unicast_param_t;

/***************************************************************************
* STRUCTURE: ptp_tc_param_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP Transparent Clock mode parameters structure
*
* ELEMENTS:
*   masterClockIdentity     - clockIdentity of the master used to syntonize this clock 
*   masterPortNumber        - portNumber of the master used to syntonize this clock
*   enableSynt              - Enable/disable syntonization to the master 
*   chnl_handle             - Pointer to an array of Ethernet channel handles                      
*   ipSaEgressCtrlMask      - Bitmask to enable/disable packet filtering based
*                             on the MAC/IPvx source address in Egress
*   ipDaIngressCtrlMask     - Bitmask to enable/disable packet filtering based
*                             on the MAC/IPvx destination address in Ingress.
*   ptpChnlBitmask          - PTP ENET channel bitmask:\n
*                             The following bits are used for each link rate:\n
*                             10G rate: bits 0 to 11\n
*                             40G rate: bits 0, 4, 8\n
*                             100G rate: bit 0
*   resetEnetChannelMask    - reset ENET channel mask flag
****************************************************************************/
typedef struct ptp_tc_param_t 
{
      UINT64 masterClockIdentity;
      UINT16 masterPortNumber;
      BOOL8  enableSynt;
      util_global_switch_data_t *chnl_handle[DIGI_ENET_LINE_CHNL_MAX];
      UINT16  ipSaEgressCtrlMask;
      UINT16  ipDaIngressCtrlMask;
      UINT16  ptpChnlBitmask; 
      UINT8   resetEnetChannelMask;
} ptp_tc_param_t;

/***************************************************************************
* STRUCTURE: ptp_tc_port_param_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   PTP Transparent Clock mode port parameters structure
*
* ELEMENTS:
*   chnl_handle             - Pointer to an array of Ethernet channel handles
*   logMinPdelayReqInterval - Log base 2 of the Pdelay_Req message transmission interval
*
****************************************************************************/
typedef struct ptp_tc_port_param_t 
{
      util_global_switch_data_t *chnl_handle[DIGI_ENET_LINE_CHNL_MAX];
      UINT8  logMinPdelayReqInterval;
} ptp_tc_port_param_t;

/***************************************************************************
* STRUCTURE: ptp_pass_thru_general_param_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   General parameters for pass-thru PTP operating mode
*
* ELEMENTS:
*   vlanTci               - VLAN TCI.
*   externalHostMacAddr   - External Host MAC Address 
*   mipsMacAddr           - The MAC address of MIPS of DIGI
*   vlanPresent           - VLAN tagging indication port for PTP packets. 
*                           Each bit position represents the corresponding port.\n
*                           0: PTP without VLAN\n
*                           1: PTP with VLAN
****************************************************************************/
typedef struct ptp_pass_thru_general_param_t 
{
    UINT16  vlanTci;
    UINT8   externalHostMacAddr[6];
    UINT8   mipsMacAddr[6];
    UINT16  vlanPresent;
} ptp_pass_thru_general_param_t;

/***************************************************************************
* STRUCTURE: ptp_pass_thru_event_ingress_cfg
* __________________________________________________________________________
*
* DESCRIPTION:
*   Ingress processing options for PTP Event messages
*
* ELEMENTS:
*   See typedef
****************************************************************************/
typedef struct ptp_pass_thru_event_ingress_cfg 
{
#ifdef PMC_BITFIELD_MSB_FIRST
    UINT8  reserved:3, /*!< force alignment */
           peerMeanPathDelay_update:1, /*!< See description below */
           asymmetry_corr:1, /*!< See description below */
           corrField_update:1, /*!< See description below */
           process_option:2; /*!< See description below */
#else
    UINT8  process_option:2,    /*!< Processing option:\n                         
                                     0: Forward with Ingress Timestamp (default)\n
                                     1: Forward ith Correction Field Counter\n    
                                     2: Discard */                                   
           corrField_update:1,  /*!<  CorrectionField update:\n                      
                                      0: No correctionField update (default)\n       
                                      1: Subtract current egress nanosecond timestamp
                                         from the correctionField of the PTP packet */
           asymmetry_corr:1,  /*!< Asymmetry Correction:\n                            
                                   0: Do not perform asymmetry correction (default)\n 
                                   1: Perform asymmetry correction. Valid for Sync    
                                      and Pdelay_Resp messages only.                  
                                      The delayAsymmetry value configured for the port
                                      (using CMD_CFG_PTP_Set_Port_Asymmetry)          
                                      will be added to the correctionField of the     
                                      PTP packet. */
           peerMeanPathDelay_update:1, /*!< PeerMeanPathDelay Update:\n                    
                                            0: Do not add peerMeanPathDelay value to the   
                                               correctionField of Sync messages (default)\n
                                            1: Add peerMeanPathDelay value to the          
                                               correctionField of Sync messages */
           reserved:3; /*!< force alignment */
#endif
} ptp_pass_thru_event_ingress_cfg;

/***************************************************************************
* STRUCTURE: ptp_pass_thru_event_egress_cfg
* __________________________________________________________________________
*
* DESCRIPTION:
*   Egress processing options for PTP Event messages
*
* ELEMENTS:
*   See typedef
****************************************************************************/
typedef struct ptp_pass_thru_event_egress_cfg 
{
#ifdef PMC_BITFIELD_MSB_FIRST
    UINT8  reserved:3, /*!< force alignment */
           asymmetry_corr:1, /*!< See description below */
           tstmp_reflect:2, /*!< See description below */
           tstmp_corrField_update:2; /*!< See description below */
#else
    UINT8  tstmp_corrField_update:2, /*!< Timestamp/CorrectionField update:\n            
                                          0: Insert timestamp into PTP packet. Valid     
                                             for Sync, Delay_Req and                     
                                             Pdelay_Req only (default)\n                 
                                          1: Add current egress nanosecond timestamp     
                                             to the correctionField of the PTP packet\n  
                                          2: No timestamp insertion or correctionField   
                                             update performed */
           tstmp_reflect:2, /*!< Timestamp Reflect:\n                              
                                 0: Do not return timestamp (default)\n            
                                 1: Return egress packet timestamp to MGMT Port\n  
                                 2: Return egress packet timestamp to CPB */          
           asymmetry_corr:1, /*!< Asymmetry Correction:\n                             
                                  0: Do not perform asymmetry correction (default)\n  
                                  1: Perform asymmetry correction. Valid for          
                                     Delay_Req and Pdelay_Req messages only.          
                                     The delayAsymmetry value configured for          
                                     the port (using CMD_CFG_PTP_Set_Port_Asymmetry)  
                                     will be subtracted from the correctionField      
                                     of the PTP packet. */
           reserved:3; /*!< force alignment */
#endif
} ptp_pass_thru_event_egress_cfg;

/***************************************************************************
* STRUCTURE: ptp_pass_thru_general_ingress_cfg
* __________________________________________________________________________
*
* DESCRIPTION:
*   Ingress processing options for PTP General messages
*
* ELEMENTS:
*   See typedef
****************************************************************************/
typedef struct ptp_pass_thru_general_ingress_cfg
{
#ifdef PMC_BITFIELD_MSB_FIRST
    UINT8  reserved:7, /*!< force alignment */
           process_option:1; /*!< See below */
#else
    UINT8  process_option:1, /*!< Processing option:\n                   
                                  0 -- Forward with Ingress Timestamp set
                                      to 0 (default)\n                   
                                  1 -- Discard message */
           reserved:7; /*!< force alignment */
#endif
} ptp_pass_thru_general_ingress_cfg;

/***************************************************************************
* STRUCTURE: ptp_pass_thru_port_param_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   Port parameters for pass-thru PTP operating mode
*
* ELEMENTS:
*   chnl_handle        - Pointer to an Ethernet channel handle
*   msgInterface       - PTP Packet Source/Destination.\n
*                        0: FE/GE Mgmt port\n
*                        1: CPB
*   peerMeanPathDelay  - Peer Path delay in scaledNanoseconds. 
*   sync_ingress        - Ingress processing option for PTP Event Messages
*   delay_req_ingress   - Ingress processing option for PTP Event Messages           
*   pdelay_req_ingress  - Ingress processing option for PTP Event Messages          
*   pdelay_resp_ingress - Ingress processing option for PTP Event Messages         
*   sync_egress         - Egress processing option for PTP Event Messages                 
*   delay_req_egress    - Egress processing option for PTP Event Messages            
*   pdelay_req_egress   - Egress processing option for PTP Event Messages           
*   pdelay_resp_egress  - Egress processing option for PTP Event Messages          
*   follow_up_ingress   - Ingress processing option for PTP Event Messages          
*   delay_resp_ingress  - Ingress processing option for PTP Event Messages          
*   pdelay_resp_follow_up_ingress - Ingress processing option for PTP Event Messages
*   announce_ingress    - Ingress processing option for PTP Event Messages            
*   signaling_ingress   - Ingress processing option for PTP Event Messages           
*   management_ingress  - Ingress processing option for PTP Event Messages          
****************************************************************************/
typedef struct ptp_pass_thru_port_param_t 
{
    util_global_switch_data_t         *chnl_handle;
    ptp_pass_thru_msg_interface_enum  msgInterface;
    INT64                             peerMeanPathDelay;
    ptp_pass_thru_event_ingress_cfg   sync_ingress;
    ptp_pass_thru_event_ingress_cfg   delay_req_ingress;
    ptp_pass_thru_event_ingress_cfg   pdelay_req_ingress;
    ptp_pass_thru_event_ingress_cfg   pdelay_resp_ingress;
    ptp_pass_thru_event_egress_cfg    sync_egress;
    ptp_pass_thru_event_egress_cfg    delay_req_egress;
    ptp_pass_thru_event_egress_cfg    pdelay_req_egress;
    ptp_pass_thru_event_egress_cfg    pdelay_resp_egress;

    ptp_pass_thru_general_ingress_cfg follow_up_ingress;
    ptp_pass_thru_general_ingress_cfg delay_resp_ingress;
    ptp_pass_thru_general_ingress_cfg pdelay_resp_follow_up_ingress;
    ptp_pass_thru_general_ingress_cfg announce_ingress;
    ptp_pass_thru_general_ingress_cfg signaling_ingress;
    ptp_pass_thru_general_ingress_cfg management_ingress;
} ptp_pass_thru_port_param_t;

/*******************************************************************************
* STRUCTURE: ptp_stability_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining PTP stability status.
*
* ELEMENTS:
*
*   chnl_handle            - Pointer to an Ethernet channel handle
*   phase_offset_threshold - phase offset threshold to get PTP stability status, radians
*   interval               - Interval to reach PTP algorithm stability, seconds
*   status                 - PTP stability status, output
*   phase_offset           - PTP phase offset, output
*******************************************************************************/
typedef struct ptp_stability_status_t
{                     
      util_global_switch_data_t *chnl_handle;
      UINT8  phase_offset_threshold;
      UINT32 interval;
      ptp_stability_status_enum status;
      UINT8  phase_offset;
} ptp_stability_status_t;

/*******************************************************************************
* STRUCTURE: digi_synce_ch_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   SyncE structure contains configuration information for one SyncE channel.
*
* ELEMENTS:
*   chnl_handle     - Pointer to an Ethernet channel handle 
*   event_flag      - Event flag to be used by the local network element for
*                     ESMC PDU generation. Refer to ITU-G.8264 Sec 11.3.1.1 \n
*                     0: Information PDU \n 
*                     1: Event PDU \n 
*   mode            - 0:slave \n 
*                     1: master \n 
*   ql_ssm          - SSM code to be used in the QL data for ESMC PDU generation
*                     from the local network element. , refer to ITU-G.8264 Sec 11.3.1.2
*   mac_sa          - MAC source address
*******************************************************************************/
typedef struct digi_synce_ch_cfg_t
{
    util_global_switch_data_t *chnl_handle;
    UINT8    event_flag;
    UINT8    mode;
    UINT8    ql_ssm;
    UINT8    mac_sa[ENET_MAC_ADDR_SIZE]; 
} digi_synce_ch_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_synce_ch_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   SyncE structure contains SyncE status information for one SyncE channel.
*
* ELEMENTS:
*   chnl_handle     - Pointer to an Ethernet channel handle 
*   ch_en           - enable or disable for the channel.
*   rx_event_flag   - Event flag received from the remote network element.
*                     Refer to ITU-G.8264 Sec 11.3.1.1 \n
*                     0: Information PDU \n 
*                     1: Event PDU \n 
*   mode            - 0:slave \n 
*                     1: master \n 
*   rx_ql_ssm       - SSM code received in the QL data of the ESMC PDU from the
*                     remote network element. Refer to ITU-G.8264 Sec 11.3.1.2
*   rx_esmc_timeout_indicator - Indicate the lack of reception of an ESMC
*                               information PDU within a five-second period.
*                               Refer to ITU-G.8264 Sec 11.3.2.2 -- QL Reception
*                               for more details.
*******************************************************************************/
typedef struct digi_synce_ch_status_t
{
    util_global_switch_data_t *chnl_handle;
    BOOL8    ch_en;
    UINT8    rx_event_flag;
    UINT8    mode;
    UINT8    rx_ql_ssm;
    BOOL8    rx_esmc_timeout_indicator;
} digi_synce_ch_status_t;

/*******************************************************************************
* STRUCTURE: digi_synce_ctrl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   SyncE structure contains configuration parameters for configuring SyncE channels.
*
* ELEMENTS:
*   ch_cfg       - configuration information for each SyncE channel
*******************************************************************************/
typedef struct digi_synce_ctrl_t
{
    digi_synce_ch_cfg_t ch_cfg[SYNCE_CHNL_MAX];
} digi_synce_ctrl_t;

/*******************************************************************************
* STRUCTURE: digi_synce_info_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   SyncE structure contains status information for all SyncE channels.
*
* ELEMENTS:
*   ch_status    - Status information each SyncE channel
*******************************************************************************/
typedef struct digi_synce_info_t
{
    digi_synce_ch_status_t ch_status[SYNCE_CHNL_MAX];
} digi_synce_info_t;

/*******************************************************************************
* STRUCTURE: ptp_dbg_pkt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   PTP packet debug data.
*
* ELEMENTS:
*   start          - flag to start capturing timestamp data 
*   idx            - the captured packet index 
*   msg_type       - PTP message type 
*   cf_nsec_hi     - CorrectionField nanoseconds high  
*   cf_nsec_lo     - CorrectionField nanoseconds low
*   reserved       - RESERVED field
*   seq_id         - sequence ID
*   tod_sec_lo     - TOD seconds low  
*   tod_nsec       - TOD nanoseconds
*   arrive_sec_loc - Arrival TOD seconds low
*   arrive_nsec    - Arrival TOD nanoseconds
*   ing_tod_sec    - Ingress message handling seconds low
*   ing_tod_nsec   - Ingress message handling nanoseconds
*   depart_sec_loc - Departure TOD seconds low
*   depart_nsec    - DEparture TOD nanoseconds
*   egr_tod_sec    - Egress message handling seconds low
*   egr_tod_nsec   - Egress message handling nanoseconds
*******************************************************************************/
typedef struct ptp_dbg_pkt_t
{
    UINT32 start;                                
    UINT32 idx;                                  
    UINT32 msg_type[PTP_TS_DBG_ARRAY_SIZE];      
    UINT32 cf_nsec_hi[PTP_TS_DBG_ARRAY_SIZE];    
    UINT32 cf_nsec_lo[PTP_TS_DBG_ARRAY_SIZE];    
    UINT32 reserved[PTP_TS_DBG_ARRAY_SIZE];      
    UINT32 seq_id[PTP_TS_DBG_ARRAY_SIZE];        
    UINT32 tod_sec_lo[PTP_TS_DBG_ARRAY_SIZE];    
    UINT32 tod_nsec[PTP_TS_DBG_ARRAY_SIZE];      
    UINT32 arrive_sec_lo[PTP_TS_DBG_ARRAY_SIZE]; 
    UINT32 arrive_nsec[PTP_TS_DBG_ARRAY_SIZE];   
    UINT32 ing_tod_sec_lo[PTP_TS_DBG_ARRAY_SIZE];
    UINT32 ing_tod_nsec[PTP_TS_DBG_ARRAY_SIZE];  
    UINT32 depart_sec_lo[PTP_TS_DBG_ARRAY_SIZE]; 
    UINT32 depart_nsec[PTP_TS_DBG_ARRAY_SIZE];   
    UINT32 egr_tod_sec_lo[PTP_TS_DBG_ARRAY_SIZE];
    UINT32 egr_tod_nsec[PTP_TS_DBG_ARRAY_SIZE];  
} ptp_dbg_pkt_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR digi_timesync_enet_ptp_config (digi_handle_t *digi_handle,
                                               ptp_config_t *ptp_cfg_ptr,
                                               BOOL is_enet_line);
PUBLIC PMC_ERROR digi_timesync_ptp_config (digi_handle_t *digi_handle,
                                           ptp_config_t *ptp_cfg_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_irig_config (digi_handle_t *digi_handle,
                                                ptp_irig_config_t *ptp_irig_cfg_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_ipv4_param_set (digi_handle_t *digi_handle,
                                                   ptp_ipv4_param_t *ptp_ipv4_param_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_ipv6_param_set (digi_handle_t *digi_handle,
                                                   ptp_ipv6_param_t *ptp_ipv6_param_ptr);                              
PUBLIC PMC_ERROR digi_timesync_ptp_port_asymmetry_set (digi_handle_t *digi_handle,
                                                       ptp_port_asymmetry_t *ptp_port_asymmetry_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_port_rx_msg_cnts_get (digi_handle_t *digi_handle,
                                                         ptp_rx_msg_cnts_t *ptp_rx_msg_cnts_ptr); 
PUBLIC PMC_ERROR digi_timesync_ptp_path_delay_get (digi_handle_t *digi_handle,
                                                   ptp_path_delay_t *ptp_path_delay_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_default_properties_set (digi_handle_t *digi_handle,
                                                           ptp_default_properties_t *defaultDS_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_time_properties_set (digi_handle_t *digi_handle,
                                                        ptp_set_time_param_t *timePropertiesDS_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_acceptable_master_table_set (digi_handle_t *digi_handle,
                                                                ptp_acceptable_master_clocks_t *acceptMasterClockParam_ptr,
                                                                ptp_acceptable_master_clocks_data_t *acceptableMasterClockTable_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_start_initialize_set (digi_handle_t *digi_handle,
                                                         ptp_start_init_param_t *start_init_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_master_clock_db_status_get (digi_handle_t *digi_handle,
                                                               ptp_master_clock_db_status_t *status_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_master_clock_db_get (digi_handle_t *digi_handle,
                                                        ptp_master_clock_db_t *master_clock_db_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_parent_data_set (digi_handle_t *digi_handle,
                                                    ptp_parent_data_t *parentDS_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_port_data_set (digi_handle_t *digi_handle,
                                                  ptp_port_data_t *portDS_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_port_state_set (digi_handle_t *digi_handle,
                                                   ptp_port_state_t *port_state_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_port_event_set (digi_handle_t *digi_handle,
                                                   ptp_port_event_t *event_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_port_state_get (digi_handle_t *digi_handle,
                                                   ptp_port_state_t *port_state_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_passive_port_parent_set (digi_handle_t *digi_handle,
                                                            ptp_passive_port_parent_t *passive_port_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_unicast_master_param_set (digi_handle_t *digi_handle,
                                                             ptp_master_unicast_param_t *master_unicast_param_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_unicast_request_set (digi_handle_t *digi_handle,
                                                        ptp_unicast_request_t *unicast_request_param_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_unicast_cancel_set (digi_handle_t *digi_handle,
                                                       ptp_unicast_cancel_t *unicast_cancel_param_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_unicast_slave_state_get (digi_handle_t *digi_handle,
                                                            ptp_slave_unicast_param_t *slave_unicast_param_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_pass_thru_general_param_set (digi_handle_t *digi_handle,
                                                                ptp_pass_thru_general_param_t *ptp_pass_thru_param_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_pass_thru_port_param_set (digi_handle_t *digi_handle,
                                                             ptp_pass_thru_port_param_t *ptp_pass_thru_param_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_tc_param_set (digi_handle_t *digi_handle,
                                                 ptp_tc_param_t *ptp_tc_param_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_tc_port_param_set (digi_handle_t *digi_handle,
                                                      ptp_tc_port_param_t *ptp_tc_port_param_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_stability_status_get (digi_handle_t *digi_handle,
                                                         ptp_stability_status_t *status_ptr);

PUBLIC PMC_ERROR digi_timesync_synce_cfg_ssm(digi_handle_t *digi_handle,
                                             digi_synce_ctrl_t *synce_ctrl_ptr);
PUBLIC PMC_ERROR digi_timesync_synce_ch_en(digi_handle_t *digi_handle,
                                           util_global_switch_data_t *chnl_handle_ptr[SYNCE_CHNL_MAX],
                                           BOOL8 enable);
PUBLIC PMC_ERROR digi_timesync_synce_req_ssm(digi_handle_t *digi_handle,
                                             digi_synce_info_t *synce_ctrl_ptr);
PUBLIC PMC_ERROR digi_timesync_synce_tx_ssm(digi_handle_t *digi_handle,
                                            digi_synce_ctrl_t *synce_ctrl_ptr);
PUBLIC PMC_ERROR digi_timesync_ptp_pkt_filter_cfg (digi_handle_t *digi_handle,
                                                   digi_enet_classification_direction_mode_t direction,
                                                   ptp_pkt_filtering_param_t ptp_pkt_filtering_param[],
                                                   UINT32 num_ptp_pkt_filter);
PUBLIC PMC_ERROR digi_timesync_ptp_capt_debug_data_start (digi_handle_t *digi_handle);

PUBLIC PMC_ERROR digi_timesync_ptp_capt_debug_data_get (digi_handle_t *digi_handle,
                                                        uintptr_t *debug_addr,
                                                        UINT32 *debug_size);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_TIMESYNC_API_H */

/*
** End of file
*/
