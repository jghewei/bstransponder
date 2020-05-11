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
*     This file contains functions to support DIGI Timing and Frequency 
*     Synchronization: PTP and SyncE run time configuration.
*
*   NOTES:
*
*******************************************************************************/

/*
** Include Files
*/
#include "digi_loc.h"
#include "hostmsg_loc.h"
#include "digi_timesync_api.h"


/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/* Length of PTP host messages */
#define HOSTMSG_CMD_CFG_PTP_SET_IRIG_CONFIG_LEN                  (0x04)
#define HOSTMSG_CMD_CFG_PTP_SET_PTP_IPV4_PARAM_LEN               (0x05)
#define HOSTMSG_CMD_CFG_PTP_SET_PTP_IPV6_PARAM_LEN               (0x0C)
#define HOSTMSG_CMD_REQ_PTP_GET_PORT_RX_MSG_CNTS_LEN             (0x03)
#define HOSTMSG_RSP_REQ_PTP_GET_PORT_RX_MSG_CNTS_LEN             (0x0B)
#define HOSTMSG_CMD_REQ_PTP_GET_PATH_DELAY_LEN                   (0x03)
#define HOSTMSG_RSP_REQ_PTP_GET_PATH_DELAY_LEN                   (0x04)
#define HOSTMSG_CMD_CFG_PTP_SET_PORT_ASYMMETRY_LEN               (0x06)
#define HOSTMSG_CMD_CFG_PTP_SET_HWIRED_PKT_FILTER_LEN            (0x10)
#define HOSTMSG_CMD_CFG_PTP_SET_GENERIC_PKT_FILTER_LEN           (0x23)
#define HOSTMSG_CMD_CFG_PTP_SET_DEFAULT_PROPERTIES_LEN           (0x04)
#define HOSTMSG_CMD_CFG_PTP_SET_TIME_PROPERTIES_LEN              (0x03)
#define HOSTMSG_CMD_CFG_PTP_SET_ACCEPTABLE_MASTER_CLOCKS_LEN(N)  (0x03 + (N)*3)
#define HOSTMSG_CMD_CFG_PTP_SET_START_INITIALIZE_LEN             (0x04)
#define HOSTMSG_CMD_REQ_PTP_GET_MASTER_CLOCK_DATABASE_STATUS_LEN (0x03)
#define HOSTMSG_RSP_REQ_PTP_GET_MASTER_CLOCK_DATABASE_STATUS_LEN (0x03)
#define HOSTMSG_CMD_REQ_PTP_GET_MASTER_CLOCK_DATABASE_LEN        (0x03)
#define HOSTMSG_RSP_REQ_PTP_GET_MASTER_CLOCK_DATABASE_LEN        (0x04)
#define HOSTMSG_CMD_CFG_PTP_SET_PARENT_DATA_LEN                  (0x0C)
#define HOSTMSG_CMD_CFG_PTP_SET_PORT_DATA_LEN                    (0x05)
#define HOSTMSG_CMD_CFG_PTP_SET_PORT_STATE_LEN                   (0x14)
#define HOSTMSG_CMD_CFG_PTP_SET_PORT_EVENT_LEN                   (0x03)
#define HOSTMSG_CMD_REQ_PTP_GET_PORT_STATE_LEN                   (0x03)
#define HOSTMSG_RSP_REQ_PTP_GET_PORT_STATE_LEN                   (0x14)
#define HOSTMSG_CMD_CFG_PTP_SET_PASSIVE_PORT_PARENT_LEN          (0x06)
#define HOSTMSG_CMD_CFG_PTP_SET_UNICAST_MASTER_PARAM_LEN         (0x06)
#define HOSTMSG_CMD_CFG_PTP_SET_UNICAST_REQUEST_LEN              (0x07)
#define HOSTMSG_CMD_CFG_PTP_SET_UNICAST_CANCEL_LEN               (0x1E)
#define HOSTMSG_CMD_REQ_PTP_GET_UNICAST_SLAVE_STATE_LEN          (0x03)
#define HOSTMSG_RSP_REQ_PTP_GET_UNICAST_SLAVE_STATE_LEN          (0x07)
#define HOSTMSG_CMD_CFG_PTP_SET_PASS_THRU_GENERAL_PARAM_LEN      (0x08)
#define HOSTMSG_CMD_CFG_PTP_SET_PASS_THRU_PORT_PARAM_LEN         (0x0F)
#define HOSTMSG_CMD_CFG_PTP_SET_TC_PARAM_LEN                     (0x06)
#define HOSTMSG_CMD_CFG_PTP_SET_TC_PORT_PARAM_LEN                (0x03)
#define HOSTMSG_CMD_REQ_PTP_GET_STABILITY_STATUS_LEN             (0x03)
#define HOSTMSG_RSP_REQ_PTP_GET_STABILITY_STATUS_LEN             (0x03)

/* For host message CMD_CFG_PTP_Set_Acceptable_Master_Clocks,
   acceptable master clockIdentity data length (in word) per entry */
#define PTP_ACCEPTABLE_MASTER_CLKID_DATA_PER_ENTRY_LEN    (0x3)

/* Length of SyncE host message */
#define SYNCE_CONFIG_BODY_INFO_PER_CH_LEN                 (0x4)
#define SYNCE_TX_SSM_BODY_INFO_PER_CH_LEN                 (0x4)
#define SYNCE_RX_SSM_BODY_INFO_PER_CH_LEN                 (0x3)
#define HOSTMSG_CMD_CFG_SYNCE_CONFIG_CH_LEN(CH_CNT)       (0x3 + SYNCE_CONFIG_BODY_INFO_PER_CH_LEN * (CH_CNT))
#define HOSTMSG_CMD_CFG_SYNCE_CTRL_CH_LEN                 (0x4)
#define HOSTMSG_CMD_REQ_SYNCE_GET_SSM_LEN                 (0x3)
#define HOSTMSG_RSP_REQ_SYNCE_GET_SSM_LEN                 (0x27)    
#define HOSTMSG_CMD_CFG_SYNCE_TX_SSM_LEN(CH_CNT)          (0x3 + SYNCE_TX_SSM_BODY_INFO_PER_CH_LEN * (CH_CNT))

/* Mask for extracting response SSM status host message */
#define SYNCE_RSP_MSG_CH_CNT_MSK    0xFF000000
#define SYNCE_RSP_MSG_ENET_CH_MSK   0xFF000000
#define SYNCE_RSP_MSG_QL_SSM_MSK    0x000000FF
#define SYNCE_RSP_MSG_EVENT_MSK     0xFF000000
#define SYNCE_RSP_MSG_MODE_MSK      0x00FF0000
#define SYNCE_RSP_MSG_LOST_CON_MSK  0x0000FF00
#define SYNCE_RSP_MSG_CH_EN_MSK     0x000000FF

/* Offset for extracting response SSM status host message */
#define SYNCE_RSP_MSG_CH_CNT_OFF    24
#define SYNCE_RSP_MSG_CH_OFF        24
#define SYNCE_RSP_MSG_QL_SSM_OFF    0
#define SYNCE_RSP_MSG_EVENT_OFF     24
#define SYNCE_RSP_MSG_MODE_OFF      16
#define SYNCE_RSP_MSG_LOST_CON_OFF  8
#define SYNCE_RSP_MSG_CH_EN_OFF     0

#define DIGI_IEEE_803_2_ADDR_SIZE   6
#define DIGI_IPV4_ADDR_SIZE         4
#define DIGI_IPV6_ADDR_SIZE         16
#define DIGI_PKT_ENET_FILTER_MAX 8

/*
** Local Macro Definitions
*/

/*
** Local Structures and Unions
*/
#ifndef DOXYGEN_PUBLIC_ONLY
/***************************************************************************
* STRUCTURE: enet_set_pkt_filter_msg_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   ENET packet filter config message 
*
* ELEMENTS:
*   clear           - BOOLEAN flag to clear (0) packet filter configuration
*   filter_idx - the filter index. Used when clear == 0. Max is 8.
*   protocol        - transport protocol for PTP message
*   ingress_egress  - DIGI_ENET_INGRESS: configure ingress packet filtering
*                     DIGI_ENET_EGRESS: configure egress packet filtering
*   value           - byte array that represents that the value (address) to filter 
*                     this array is sized for the protocol with biggest 
*                     address. Valid bytes per protocol are:\n
*                        PTP_OVER_UDP_OVER_IPV4 : 4
*                        PTP_OVER_UDP_OVER_IPV6 : 16
*                        PTP_OVER_802_3         : 6
*   mask            - mask bytes are associated to addr bytes. Mask bytes \n
*                     allows to wild card any bits in the specified address.
*                     A true value (1) means that the associated addr bit \n
*                     will be user by the filter and a false value (0).
*   channel_bitmask - ENET channel bitmask: 1 (0) - 2048(11).
****************************************************************************/
typedef struct enet_set_pkt_filter_msg_t {
    UINT32 clear:8;   
    UINT32 filter_idx:8;
    ptp_transport_protocol_enum protocol:8;
    UINT32 ingress_egress:8;
    UINT32 value[4];
    UINT32 mask[4];
    UINT32 channel_bitmask;
} enet_set_pkt_filter_msg_t;

#endif /* DOXYGEN_PUBLIC_ONLY */
/*
** Local Variables
*/

/*
** Private Functions
*/
#ifndef DOXYGEN_PUBLIC_ONLY
PRIVATE UINT32 digi_timesync_ptp_pass_thru_port_create_general_cfg_field (ptp_pass_thru_general_ingress_cfg *ingress_cfg_ptr);
PRIVATE UINT32 digi_timesync_ptp_pass_thru_port_create_event_cfg_field (ptp_pass_thru_event_ingress_cfg *ingress_cfg_ptr,
                                                                        ptp_pass_thru_event_egress_cfg *egress_cfg_ptr);
PRIVATE UINT32 digi_timesync_ptp_pass_thru_port_create_event_cfg_field (ptp_pass_thru_event_ingress_cfg *ingress_cfg_ptr,
                                                                        ptp_pass_thru_event_egress_cfg *egress_cfg_ptr);
#endif /* DOXYGEN_PUBLIC_ONLY */

/*
** Public Functions
*/

/*******************************************************************************
* digi_timesync_enet_ptp_config()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure the PTP mode of operation as Boundary
*   Clock or Transparent Clock, and related PTP parameters. The parameters
*   configured by this message correlate to the members of the defaultDS data
*   set described in IEEE 1588v2 standard.
*
* INPUTS:
*  *digi_handle                 - pointer to DIGI handle instance
*  *ptp_cfg_ptr                 - pointer to the PTP configuration structure
*  is_enet_line                 - If true, configure PTP for line enet use, otherwise
*                                 configure for sys enet. 
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Config
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_enet_ptp_config (digi_handle_t *digi_handle,
                                                ptp_config_t  *ptp_cfg_ptr,
                                                BOOL is_enet_line)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[(sizeof(ptp_config_t)/sizeof(UINT32))];
    UINT32 client_source_data[1];

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != ptp_cfg_ptr, DIGI_ERR_NULL_HANDLE, 0, 0); 

    client_source_data[0] = (UINT8)is_enet_line & 0x1;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_ENET,HOSTMSG_CMD_CFG_ENET_SET_ENET_CLIENT_SOURCE_ST,client_source_data,1,stat);
    }

    if (PMC_SUCCESS == result) {
        digi_handle->var.digi_pmon_mode_profile->fw_enet_client_source.valid = TRUE;
        digi_handle->var.digi_pmon_mode_profile->fw_enet_client_source.is_enet_line = is_enet_line;
    }

    /* CMD_CFG_PTP_Config has the following format,
     * Word      bits 24..31             bits 16..23         bits 8..15     bits 0..7
     *  0    |      dev_type           |   twoStepFlag   |            Reserved              |
     *  1    |                                 clockIdentity                                |
     *  2    |                                 clockIdentity                                |
     *  3    |      domainNumber       |                   Reserved                         |
     *  4    |transportProtocolBitmask | multicastEnable | operatingMode |  vlanTagPresent  |
     *  5    |             cfcModeMask                   | frq_synt_rate |  slave_mode      |
     *  6    |     mstProtocol[3]      | mstProtocol[2]  | mstProtocol[1]|  mstProtocol[0]  |
     *  7    |     mstProtocol[7]      | mstProtocol[6]  | mstProtocol[5]|  mstProtocol[4]  |
     *  8    |     mstProtocol[11]     | mstProtocol[10] | mstProtocol[9]|  mstProtocol[8]  |
     */
    if (PMC_SUCCESS == result) {
        data[0] = (((ptp_cfg_ptr->dev_type) << 24) | 
                     ((ptp_cfg_ptr->twoStepFlag) << 16));
#if defined(PMC_BIG_ENDIAN)
            ptp_cfg_ptr->clockIdentity = PMC_BE_TO_CPU_64(ptp_cfg_ptr->clockIdentity);
            data[1] = (UINT32)(ptp_cfg_ptr->clockIdentity >> 32);
            data[2] = (UINT32)(ptp_cfg_ptr->clockIdentity);
#else
           ptp_cfg_ptr->clockIdentity = PMC_LE_TO_CPU_64(ptp_cfg_ptr->clockIdentity);
           data[1] = (UINT32)ptp_cfg_ptr->clockIdentity;
           data[2] = (UINT32)(ptp_cfg_ptr->clockIdentity >> 32);
#endif

        data[3] = ((ptp_cfg_ptr->domainNumber) << 24);

        data[4] = (((ptp_cfg_ptr->transportProtocolBitmask) << 24) |
                         ((ptp_cfg_ptr->multicastEnable) << 16) |
                         ((ptp_cfg_ptr->optMode) << 8) |
                         (ptp_cfg_ptr->vlanTagPresent));
        data[5] = (((ptp_cfg_ptr->cfcModeMask) << 16) |
                         ((ptp_cfg_ptr->frq_synt_rate) << 8)|
                          ptp_cfg_ptr->slave_mode);
        memcpy(&data[6], &ptp_cfg_ptr->mstProtocol, sizeof(ptp_cfg_ptr->mstProtocol));
        {
            HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
            result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_CONFIG_ST,data,(sizeof(ptp_config_t)/sizeof(UINT32)),stat);
        }  
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_timesync_enet_ptp_config */

/*******************************************************************************
* digi_timesync_ptp_config()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function is used to configure the PTP mode of operation as Boundary
*   Clock or Transparent Clock, and related PTP parameters. The parameters
*   configured by this message correlate to the members of the defaultDS data
*   set described in IEEE 1588v2 standard.
*
* INPUTS:
*  *digi_handle                 - pointer to DIGI handle instance
*  *ptp_cfg_ptr                 - pointer to the PTP configuration structure
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Config
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_config (digi_handle_t *digi_handle,
                                           ptp_config_t  *ptp_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[(sizeof(ptp_config_t)/sizeof(UINT32))];

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != ptp_cfg_ptr, DIGI_ERR_NULL_HANDLE, 0, 0); 

    /* CMD_CFG_PTP_Config has the following format,
     * Word      bits 24..31             bits 16..23         bits 8..15     bits 0..7
     *  0    |      dev_type           |   twoStepFlag   |            Reserved              |
     *  1    |                                 clockIdentity                                |
     *  2    |                                 clockIdentity                                |
     *  3    |      domainNumber       |                   Reserved                         |
     *  4    |transportProtocolBitmask | multicastEnable | operatingMode |  vlanTagPresent  |
     *  5    |             cfcModeMask                   | frq_synt_rate |  slave_mode      |
     *  6    |     mstProtocol[3]      | mstProtocol[2]  | mstProtocol[1]|  mstProtocol[0]  |
     *  7    |     mstProtocol[7]      | mstProtocol[6]  | mstProtocol[5]|  mstProtocol[4]  |
     *  8    |     mstProtocol[11]     | mstProtocol[10] | mstProtocol[9]|  mstProtocol[8]  |
     */
    data[0] = (((ptp_cfg_ptr->dev_type) << 24) | 
                 ((ptp_cfg_ptr->twoStepFlag) << 16));
#if defined(PMC_BIG_ENDIAN)
        ptp_cfg_ptr->clockIdentity = PMC_BE_TO_CPU_64(ptp_cfg_ptr->clockIdentity);
        data[1] = (UINT32)(ptp_cfg_ptr->clockIdentity >> 32);
        data[2] = (UINT32)(ptp_cfg_ptr->clockIdentity);
#else
       ptp_cfg_ptr->clockIdentity = PMC_LE_TO_CPU_64(ptp_cfg_ptr->clockIdentity);
       data[1] = (UINT32)ptp_cfg_ptr->clockIdentity;
       data[2] = (UINT32)(ptp_cfg_ptr->clockIdentity >> 32);
#endif

    data[3] = ((ptp_cfg_ptr->domainNumber) << 24);

    data[4] = (((ptp_cfg_ptr->transportProtocolBitmask) << 24) |
                     ((ptp_cfg_ptr->multicastEnable) << 16) |
                     ((ptp_cfg_ptr->optMode) << 8) |
                     (ptp_cfg_ptr->vlanTagPresent));
    data[5] = (((ptp_cfg_ptr->cfcModeMask) << 16) |
                     ((ptp_cfg_ptr->frq_synt_rate) << 8)|
                      ptp_cfg_ptr->slave_mode);
    memcpy(&data[6], &ptp_cfg_ptr->mstProtocol, sizeof(ptp_cfg_ptr->mstProtocol));
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_CONFIG_ST,data,(sizeof(ptp_config_t)/sizeof(UINT32)),stat);
    }  

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_timesync_ptp_config */



/*******************************************************************************
* digi_timesync_ptp_irig_config()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to configure IRIG port.
* 
* INPUTS:
*  *digi_handle                 - pointer to DIGI handle instance
*  *ptp_irig_cfg_ptr            - pointer to the IRIG configuration structure
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_IRIG_Config
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_irig_config (digi_handle_t     *digi_handle,
                                                ptp_irig_config_t *ptp_irig_cfg_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[2];

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != ptp_irig_cfg_ptr, DIGI_ERR_NULL_HANDLE, 0, 0); 

    /* The third word of CMD_CFG_PTP_Set_IRIG_Config has the following format,
     * Word |  bits 24..31 | bits 16..23  |  bits 8..15  |   bits 0..7 |
     *   0  |   IRIG_Port  |  EnableFlag  |   IRIG_Mode  |   Adj_Mode  |
     */
    data[0] = (((ptp_irig_cfg_ptr->port) << 24) | 
                 ((ptp_irig_cfg_ptr->enable) << 16) |
                 ((ptp_irig_cfg_ptr->irig_mode) << 8) |
                 (ptp_irig_cfg_ptr->adj_mode));
    data[1] = (((ptp_irig_cfg_ptr->frq_synt_prd) << 24) | 
                 ((ptp_irig_cfg_ptr->frq_synt_rate) << 16));

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_IRIG_CONFIG_ST,data,2,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_timesync_ptp_irig_config */

/*******************************************************************************
* digi_timesync_ptp_ipv4_param_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to set PTP over UDP over IPv4 parameters.
*
* INPUTS:
*  *digi_handle                 - pointer to DIGI handle instance
*  *ptp_ipv4_param_ptr          - pointer to the PTP over UDP over IPv4 parameters structure
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_PTP_IPv4_Param
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_ipv4_param_set (digi_handle_t     *digi_handle,
                                                   ptp_ipv4_param_t  *ptp_ipv4_param_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 data[3];

    PMC_ATOMIC_ENTRY(digi_handle);
 
    PMC_ASSERT(NULL != ptp_ipv4_param_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* The third word of CMD_CFG_PTP_Set_PTP_IPv4_Param has the following format,
     * Word |  bits 24..31  | bits 16..23  |  bits 8..15  |  bits 0..7 |
     *   0  | typeOfService |  timeToLive  |   Reserved              |
     *   1  | sourceAddress                                          |                                                            |
     *   2  | mst_destAddress                                        |                                                                                                                        |
     */
    data[0] = (((ptp_ipv4_param_ptr->typeOfService) << 24) | 
                 ((ptp_ipv4_param_ptr->timeToLive) << 16));

    data[1] = ptp_ipv4_param_ptr->sourceAddress;

    data[2] = ptp_ipv4_param_ptr->mst_destAddress;

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_PTP_IPV4_PARAM_ST,data,3,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_timesync_ptp_ipv4_param_set */
                               
/*******************************************************************************
* digi_timesync_ptp_ipv6_param_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to set PTP over UDP over IPv6 parameters.
*
* INPUTS:
*  *digi_handle                 - pointer to DIGI handle instance
*  *ptp_ipv6_param_ptr          - pointer to the PTP over UDP over IPv6 parameters structure
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_PTP_IPv6_Param
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_ipv6_param_set (digi_handle_t     *digi_handle,
                                                   ptp_ipv6_param_t  *ptp_ipv6_param_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[10];

    PMC_ATOMIC_ENTRY(digi_handle);
     
    PMC_ASSERT(NULL != ptp_ipv6_param_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* The third word of CMD_CFG_PTP_Set_PTP_IPv6_Param has the following format,
     * Word |  bits 24..31  | bits 16..23  |  bits 8..15  |  bits 0..7 |
     *   0  | trafficClass | flowLabel                              |
     *   1  | hopLimit     | daScopeValue |   Reserved              |
     *   2  | sourceAddress (127:96)                                |
     *   3  | sourceAddress (95:64)                                 |                                                                                                                                                                 |
     *   4  | sourceAddress (63:32)                                 |                                                                                                                                                                                                                              |
     *   5  | sourceAddress (31:0)                                  |                                                                                                                                                                                                                                                                                           |
     *   6  | sourceAddress (127:96)                                |                                                                                                                                                                                                                                                                                                                                                        |
     *   7  | sourceAddress (95:64)                                 |                                                                                                                                                                                                                                                                                                                                                                                                                     |
     *   8  | sourceAddress (63:32)                                 |                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  |
     *   9  | sourceAddress (31:0)                                  |                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                |
     */
    data[0] = (((ptp_ipv6_param_ptr->trafficClass) << 24) | 
                 ((ptp_ipv6_param_ptr->flowLabel[0]) << 16) |
                 ((ptp_ipv6_param_ptr->flowLabel[1]) << 8) |
                 (ptp_ipv6_param_ptr->flowLabel[2]));

    data[1] = (((ptp_ipv6_param_ptr->hopLimit) << 24) | 
                     ((ptp_ipv6_param_ptr->daScopeValue) << 16));

    data[2] = ptp_ipv6_param_ptr->sourceAddress[0];
    data[3] = ptp_ipv6_param_ptr->sourceAddress[1];
    data[4] = ptp_ipv6_param_ptr->sourceAddress[2];
    data[5] = ptp_ipv6_param_ptr->sourceAddress[3];

    data[6] = ptp_ipv6_param_ptr->mst_destAddress[0];
    data[7] = ptp_ipv6_param_ptr->mst_destAddress[1];
    data[8] = ptp_ipv6_param_ptr->mst_destAddress[2];
    data[9] = ptp_ipv6_param_ptr->mst_destAddress[3];

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_PTP_IPV6_PARAM_ST,data,10,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_ipv6_param_set */                               

/*******************************************************************************
* digi_timesync_ptp_port_asymmetry_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to configure port's asymmetry and latency delays.
*
* INPUTS:
*  *digi_handle                 - pointer to DIGI handle instance
*  *ptp_port_asymmetry_ptr      - pointer to the PTP port asymmetry parameters
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Port_Asymmetry
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_port_asymmetry_set (digi_handle_t     *digi_handle,
                                                       ptp_port_asymmetry_t *ptp_port_asymmetry_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[4];

    enet_handle_t *enet_handle;
    UINT32        link; 

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != ptp_port_asymmetry_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);


    /* CMD_CFG_PTP_Set_Port_Asymmetry has the following format,
     * Word |  bits 24..31  | bits 16..23 | bits 8..15 |  bits 0..7  |
     *   0  |  enetChannel  |    Reserved                            |
     *   1  |  delayAsymmetry                                        |
     *   2  |  cfcIngressLatencyDelay     |   cfcEgressLatencyDelay  |
     *   3  |  todIngressLatencyDelay     |   todEgressLatencyDelay  |                                                                                                                     |
     */

    /* dereference ptp_port_asymmetry_ptr->chnl_handle to extract the channel number */
    rv = digi_enet_validate_parms(digi_handle, ptp_port_asymmetry_ptr->chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rv)
    {
        data[0] = link << 24;
    
        data[1] = ptp_port_asymmetry_ptr->delayAsymmetry;
        data[2] = (((ptp_port_asymmetry_ptr->cfc_ingress_latency_delay) << 16) |
                         (ptp_port_asymmetry_ptr->cfc_egress_latency_delay));
        data[3] = (((ptp_port_asymmetry_ptr->tod_ingress_latency_delay) << 16) |
                         (ptp_port_asymmetry_ptr->tod_egress_latency_delay));
    }

    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_PORT_ASYMMETRY_ST,data,4,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_port_asymmetry_set */ 
 
/*******************************************************************************
* digi_timesync_ptp_port_rx_msg_cnts_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to obtain the received message counter values.
*
* INPUTS:
*  *digi_handle                 - pointer to DIGI handle instance
*  *ptp_rx_msg_cnts_ptr         - pointer to the received message counters structure
* OUTPUTS:
*  *ptp_rx_msg_cnts_ptr         - pointer to the received message counter values
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_REQ_PTP_Get_Port_Rx_Msg_Cnts
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_port_rx_msg_cnts_get (digi_handle_t     *digi_handle,
                                                         ptp_rx_msg_cnts_t *ptp_rx_msg_cnts_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[9];
    enet_handle_t *enet_handle = NULL;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);
      
    PMC_ASSERT(NULL != ptp_rx_msg_cnts_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* CMD_REQ_PTP_Get_Port_Rx_Msg_Cnts has the following format,
     * Word |  bits 24..31  | bits 16..23 | bits 8..15 |  bits 0..7  |
     *   0  |  enetChannel |    Reserved                            |
     */
    /* dereference the channel handle pointer to extrace enet channel ID */
    rv = digi_enet_validate_parms(digi_handle, ptp_rx_msg_cnts_ptr->chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rv)
    {
        data[0] = (link << 24);
    }

    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        rv = hostmsg_perform_variable_length_query((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_REQ_PTP_GET_PORT_RX_MSG_CNTS_ST,data,1,response,9,NULL,stat);
    }

    /* RSP_REQ_PTP_Get_Port_Rx_Msg_Cnts has the following format,
     * Word   |  bits 24..31   | bits 16..23  | bits 8..15 | bits 0..7 |
     *   0    | enetChannel    |  Reserved    |   Reserved             | 
     *   1    | syncCnt                                                |     
     *   2    | delayReqCnt                                            |         
     *   3    | pdelayReqCnt                                           |        
     *   4    | pdelayRespCnt                                          |       
     *   5    | followUpCnt                                            |     
     *   6    | delayRespCnt                                           |      
     *   7    | pdelayRespFollowUpCnt                                  |
     *   8    | announceCnt                                            |
     */

    if (PMC_SUCCESS == rv)
    {
        /* Return requested data. */    
        ((digi_enet_client_chnl_def_t*)ptp_rx_msg_cnts_ptr->chnl_handle)->switch_data.cpb_data.channel = (UINT8)(response[0] >> 24);
        ptp_rx_msg_cnts_ptr->syncCnt               = response[1];
        ptp_rx_msg_cnts_ptr->delayReqCnt           = response[2];
        ptp_rx_msg_cnts_ptr->pdelayReqCnt          = response[3];
        ptp_rx_msg_cnts_ptr->pdelayRespCnt         = response[4];
        ptp_rx_msg_cnts_ptr->followUpCnt           = response[5];
        ptp_rx_msg_cnts_ptr->delayRespCnt          = response[6];
        ptp_rx_msg_cnts_ptr->pdelayRespFollowUpCnt = response[7];
        ptp_rx_msg_cnts_ptr->announceCnt           = response[8];
    }

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_port_rx_msg_cnts_get */ 

/*******************************************************************************
* digi_timesync_ptp_path_delay_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to obtain the path delay of the specified channel.
*
* INPUTS:
*  *digi_handle                 - pointer to DIGI handle instance
*  *ptp_path_delay_ptr          - pointer to the path delay data structure
* OUTPUTS:
*  *ptp_path_delay_ptr          - pointer to the path delay data structure
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_REQ_PTP_Get_Path_Delay
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_path_delay_get (digi_handle_t    *digi_handle,
                                                   ptp_path_delay_t *ptp_path_delay_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[2];

    enet_handle_t* enet_handle;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);   
    PMC_ASSERT(NULL != ptp_path_delay_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);


    /*  CMD_REQ_PTP_Get_Path_Delay has the following format,
     * Word |  bits 24..31  | bits 16..23 | bits 8..15 |  bits 0..7  |
     *   0  |  enetChannel  |    Reserved                            |
     */
    /* deference enet channel handle pointer to extract channel ID */
    rv = digi_enet_validate_parms(digi_handle, ptp_path_delay_ptr->chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rv)
    {
        data[0] = (UINT8)link << 24;
    }
    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        rv = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_REQ_PTP_GET_PATH_DELAY_ST,data,1,response,2,stat);
    }
    
    /* RSP_REQ_PTP_Get_Path_Delay has the following format,
     * Word |  bits 24..31  | bits 16..23 | bits 8..15 |  bits 0..7  |
     *   0  | path_delay (MSB)                                     |
     *   1  | path_delay (LSB)                                     |
     */
    if (PMC_SUCCESS == rv)
    {

        /* Return requested data. */
        ptp_path_delay_ptr->path_delay_msb = response[0];
        ptp_path_delay_ptr->path_delay_lsb = response[1];
    }

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_path_delay_get */ 
/*******************************************************************************
* digi_timesync_ptp_default_properties_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to set PTP Boundary Clock mode default properties.
*
* INPUTS:
*  *digi_handle            - pointer to DIGI handle instance
*  *defaultDS_ptr          - pointer to the BC mode default properties
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Default_Properties.
*        This function is used to configure PTP Boundary Clock mode.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_default_properties_set (digi_handle_t *digi_handle,
                                                           ptp_default_properties_t *defaultDS_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[2];

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != defaultDS_ptr, DIGI_ERR_NULL_HANDLE, 0, 0); 

    /* CMD_CFG_PTP_Set_Default_Properties has the following format,
     * Word |  bits 24..31  | bits 16..23 | bits 8..15 |  bits 0..7  |
     *   0  |  clockClass   | clockAccuracy |  offsetScaledLogVariance   |
     *   1  |  priority1    | priority2     |  slaveOnly   |   Reserved  |
     */
    data[0] = (((defaultDS_ptr->clockClass) << 24) | 
               ((defaultDS_ptr->clockAccuracy) << 16) |
               (defaultDS_ptr->offsetScaledLogVariance));

    data[1] = (((defaultDS_ptr->priority1) << 24) | 
               ((defaultDS_ptr->priority2) << 16) |
               ((defaultDS_ptr->slaveOnly) << 8));

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_DEFAULT_PROPERTIES_ST,data,2,stat);
    }


    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_default_properties_set */ 

/*******************************************************************************
* digi_timesync_ptp_time_properties_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to set PTP Boundary Clock mode time properties.
*
* INPUTS:
*  *digi_handle            - pointer to DIGI handle instance
*  *timePropertiesDS_ptr   - pointer to the BC mode time properties
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Time_Properties.
*        This function is used to configure PTP Boundary Clock mode
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_time_properties_set (digi_handle_t        *digi_handle,
                                                        ptp_set_time_param_t *timePropertiesDS_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[1];

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != timePropertiesDS_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* CMD_CFG_PTP_Set_Time_Properties has the following format,
     * Word |  bits 24..31  | bits 16..23 | bits 8..15         |  bits 0..7    |
     *   0  |   currentUtcOffset          |timePropertiesFlags |   timeSource  |
     */
    data[0] = (((timePropertiesDS_ptr->currentUtcOffset) << 16) | 
               ((timePropertiesDS_ptr->leap61) << 15) | 
               ((timePropertiesDS_ptr->leap59) << 14) | 
               ((timePropertiesDS_ptr->currentUtcOffsetValid) << 13) | 
               ((timePropertiesDS_ptr->ptpTimescale) << 12) | 
               ((timePropertiesDS_ptr->timeTraceable)<< 11) | 
               (timePropertiesDS_ptr->timeSource));

    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_TIME_PROPERTIES_ST,data,1,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_time_properties_set */ 

/*******************************************************************************
* digi_timesync_ptp_acceptable_master_table_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to configure PTP Boundary Clock mode acceptable master clocks.
*
* INPUTS:
*  *digi_handle                     - pointer to DIGI handle instance
*  *acceptMasterClockParam_ptr      - pointer to acceptable master clocks parameters
*  *acceptableMasterClockTable_ptr  - pointer to the acceptable master clocks table
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Acceptable_Master_Clocks.
*        This function is used to configure PTP Boundary Clock mode
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_acceptable_master_table_set (digi_handle_t *digi_handle,
                                                                ptp_acceptable_master_clocks_t *acceptMasterClockParam_ptr,
                                                                ptp_acceptable_master_clocks_data_t *acceptableMasterClockTable_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[62];
    UINT32 i;
    UINT8 total_entry_num;
    UINT32 length;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != acceptMasterClockParam_ptr, DIGI_ERR_NULL_HANDLE, 0, 0); 
    PMC_ASSERT(NULL != acceptableMasterClockTable_ptr, DIGI_ERR_NULL_HANDLE, 0, 0); 

    total_entry_num = acceptMasterClockParam_ptr->numEntries;

    PMC_ASSERT(total_entry_num <= 20, DIGI_ERR_NULL_HANDLE, 0, 0); 
    length = total_entry_num * 3 + 1;


    /* Build the rest of the message to the prevously created header.*/
    /* The third word of CMD_CFG_PTP_Set_Acceptable_Master_Clocks has the following format,
     * Word |  bits 24..31  | bits 16..23 | bits 8..15 |  bits 0..7  |
     *   0  | enableTable  |  clearTable  |     numEntries    |   Reserved  |
     *   1  | Acceptable master clockIdentity data                    |                                                             |
     *                                                                                                                              |
     *   Acceptable master clockIdentity data:
     * Word |  bits 24..31  | bits 16..23 | bits 8..15 |  bits 0..7  |
     *      | clockIdentity[0] | clockIdentity[1] | clockIdentity[2] | clockIdentity[3] |
     *      | clockIdentity[4] | clockIdentity[5] |    clockIdentity[6] | clockIdentity[7] |
     *      | portNumber[0]       | portNumber[1]      | Reserved                            |                                                                                             |
     */

    data[0] = (((acceptMasterClockParam_ptr->enableTable) << 24) | 
               ((acceptMasterClockParam_ptr->clearTable)  << 16) |
               ((acceptMasterClockParam_ptr->numEntries)  << 8));

    for (i =0; i < total_entry_num; i++) 
    {
#if defined(PMC_BIG_ENDIAN)
        acceptableMasterClockTable_ptr[i].clockIdentity = PMC_BE_TO_CPU_64(acceptableMasterClockTable_ptr[i].clockIdentity);
        data[1 + (i*PTP_ACCEPTABLE_MASTER_CLKID_DATA_PER_ENTRY_LEN)] = (UINT32)(acceptableMasterClockTable_ptr[i].clockIdentity >> 32);
        data[2 + (i*PTP_ACCEPTABLE_MASTER_CLKID_DATA_PER_ENTRY_LEN)] = (UINT32)(acceptableMasterClockTable_ptr[i].clockIdentity);	
#else
        acceptableMasterClockTable_ptr[i].clockIdentity = PMC_LE_TO_CPU_64(acceptableMasterClockTable_ptr[i].clockIdentity);
        data[1 + (i*PTP_ACCEPTABLE_MASTER_CLKID_DATA_PER_ENTRY_LEN)] = (UINT32)(acceptableMasterClockTable_ptr[i].clockIdentity);
        data[2 + (i*PTP_ACCEPTABLE_MASTER_CLKID_DATA_PER_ENTRY_LEN)] = (UINT32)(acceptableMasterClockTable_ptr[i].clockIdentity >> 32);			
#endif
        data[3 + (i*PTP_ACCEPTABLE_MASTER_CLKID_DATA_PER_ENTRY_LEN)] = ((acceptableMasterClockTable_ptr[i].portNumber) << 16);
    }


    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_ACCEPTABLE_MASTER_CLOCKS_ST,data,length,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_acceptable_master_table_set */ 
/*******************************************************************************
* digi_timesync_ptp_start_initialize_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This message triggers the INITIALIZE event in the PTP state machine for 
*   Boundary Clock mode of operation and also perform hardware configuration to 
*   extract PTP packets. For Pass-Thru mode of operation, this message will 
*   trigger hardware configuration to extract PTP packets. 
*
* INPUTS:
*  *digi_handle     - pointer to DIGI handle instance
*  *start_init_ptr  - pointer to the start initialize structure
*
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Start_Initialize
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_start_initialize_set (digi_handle_t *digi_handle,
                                                         ptp_start_init_param_t *start_init_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[2];

    UINT32 link;
    UINT16 chnl_bitmask = 0;
    enet_handle_t *enet_handle = NULL;
    UINT32 i;

    PMC_ATOMIC_ENTRY(digi_handle);

    /*  CMD_CFG_PTP_Set_Start_Initialize has the following format,
     * Word |    bits 24..31   |    bits 16..23 | bits 8..15           |  bits 0..7  |
     *   0  |             enetChannelMask       | resetEnetChannelMask |  Reserved   |
     *   1  |               outerVlanTag        |      innerVlanTag                  |
     */
    /* dereference chnl_handle pointer array and get channel IDs into chnl_bitmask */
    for (i = 0; i < DIGI_ENET_LINE_CHNL_MAX && PMC_SUCCESS == rv; i++)
    {
        if (start_init_ptr->chnl_handle[i] != NULL)
        {
            /* validate channel handle pointer */
            rv = digi_enet_validate_parms(digi_handle, start_init_ptr->chnl_handle[i], &enet_handle, &link);

            if (PMC_SUCCESS == rv)
            {
                chnl_bitmask |= 1 << (link % 16);
            }
        }
    }
    /* specify what ENET channels are used for PTP */
    chnl_bitmask &= start_init_ptr->ptpChnlBitmask;

    if (PMC_SUCCESS == rv)
    {
        data[0] = (chnl_bitmask << 16) | (start_init_ptr->resetEnetChannelMask << 8);
        data[1] = (((start_init_ptr->outerVlanTag) << 16) |
                     start_init_ptr->innerVlanTag);
    }
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_START_INITIALIZE_ST,data,2,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_start_initialize_set */ 

/*******************************************************************************
* digi_timesync_ptp_master_clock_db_status_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to obtain status of the master clock database state.
*
* INPUTS:
*  *digi_handle               - pointer to DIGI handle instance
* OUTPUTS:
*  *status_ptr                - pointer to the master clock database status
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_REQ_PTP_Get_Master_Clock_Database_Status
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_master_clock_db_status_get (digi_handle_t *digi_handle,
                                                               ptp_master_clock_db_status_t *status_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[1];

    PMC_ATOMIC_ENTRY(digi_handle);
     
    PMC_ASSERT(NULL != status_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    /*  CMD_REQ_PTP_Get_Master_Clock_Database_Status has the following format,
     * Word |  bits 24..31  | bits 16..23 | bits 8..15 |  bits 0..7  |
     *   0  |   Reserved                                            |
     */
    data[0] = 0;

    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        rv = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_REQ_PTP_GET_MASTER_CLOCK_DATABASE_STATUS_ST,data,1,response,1,stat);
    }


    /* RSP_REQ_PTP_Get_Master_Clock_Database_Status has the following format,
     * Word |  bits 24..31  | bits 16..23 | bits 8..15 |  bits 0..7  |
     *   0  |   status   |    numEntries   |   Reserved              |
     */
    if (PMC_SUCCESS == rv)
    {

        /* Return requested data. */
        status_ptr->status = (UINT8)((response[0] & 0xFF000000) >> 24);
        status_ptr->num_entries = (UINT8)((response[0] & 0xFF0000) >> 16);
    }

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_master_clock_db_status_get */ 
/*******************************************************************************
* digi_timesync_ptp_master_clock_db_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to obtain the master clock database data.
*
* INPUTS:
*  *digi_handle               - pointer to DIGI handle instance
* OUTPUTS:
*  *master_clock_db_ptr       - pointer to the master clock database data
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_REQ_PTP_Get_Master_Clock_Database
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_master_clock_db_get (digi_handle_t *digi_handle,
                                                        ptp_master_clock_db_t     *master_clock_db_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 spram_var;
    UINT32 spram_addr_offset;
    hostmsg_handle_t *cpup34k_handle = NULL;
    UINT32 data[1];
    UINT32 response[2];

    PMC_ATOMIC_ENTRY(digi_handle);

    cpup34k_handle = (hostmsg_handle_t *)(digi_handle->base.common_info->hostmsg_handle);

    PMC_ASSERT((NULL != cpup34k_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);   
    PMC_ASSERT(NULL != master_clock_db_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* CMD_REQ_PTP_Get_Master_Clock_Database has the following format,
     * Word |  bits 24..31  | bits 16..23 | bits 8..15 |  bits 0..7  |
     *   2  |   Reserved                                             |
     */
    data[0] = 0;

    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        rv = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_REQ_PTP_GET_MASTER_CLOCK_DATABASE_ST,data,1,response,2,stat);
    }

    /* RSP_REQ_PTP_Get_Master_Clock_Database has the following format,
     * Word |  bits 24..31  | bits 16..23 | bits 8..15 |  bits 0..7  |
     *   0    | responseSize           |  Reserved           |
     *   1    | responseAddress                            |
     *
     * 'responseAddress' points to the address of extended message, which has the following format,
     * Word   |  bits 24..31            | bits 16..23            | bits 8..15             | bits 0..7                |
     *   0    | domainNumber            | Reserved               | stepsRemoved                                      |
     *   1    | grandmasterPriority1    | grandmasterPriority2   | grandmasterClockClass  | grandmasterClockAccuracy |
     *   2    | grandmasterOffsetScaledLogVariance               | Reserved                                          |
     *   3    | Reserved                                                                                             |
     *   4    | grandmasterIdentity[0]  | grandmasterIdentity[1] | grandmasterIdentity[2] | grandmasterIdentity[3]   |
     *   5    | grandmasterIdentity[4]  | grandmasterIdentity[5] | grandmasterIdentity[6] | grandmasterIdentity[7]   |
     *   6    | clockIdentity[0]        | clockIdentity[1]       | clockIdentity[2]       | clockIdentity[3]         |
     *   7    | clockIdentity[4]        | clockIdentity[5]       | clockIdentity[6]       | clockIdentity[7]         |
     *   8    | portNumber[0]           | portNumber[1]          | Reserved                                          |
     *   9    | Reserved                                                                                             |
     *   10   | timePropertiesFlag      | timeSource             | Reserved                                          |
     *   11   | Reserved                                                                                             |                             
     */
    if (PMC_SUCCESS == rv)
    {

        /* Return requested data. */
        master_clock_db_ptr->responseSize = (UINT16)(response[0] & 0xFFFF0000) >> 16;
        master_clock_db_ptr->responseAddress = response[1];

        /* Get the address offset from spram base address*/
        spram_addr_offset = master_clock_db_ptr->responseAddress & 0x7FFFFF;

        /* Read from SPRAM and return extended data */
        spram_var = cpup34k_reg_read(NULL, (pmc_handle_t *)cpup34k_handle, MEM_TYPE_CONFIG, spram_addr_offset);
        master_clock_db_ptr->domainNumber = (UINT8)((spram_var & 0xFF000000) >> 24);
        master_clock_db_ptr->stepsRemoved = (UINT16)(spram_var & 0xFFFF);

        spram_var = cpup34k_reg_read(NULL, (pmc_handle_t *)cpup34k_handle, MEM_TYPE_CONFIG, (spram_addr_offset+0x4));
        master_clock_db_ptr->grandmasterPriority1    = (UINT8)((spram_var & 0xFF000000) >> 24);
        master_clock_db_ptr->grandmasterPriority2    = (UINT8)((spram_var & 0x00FF0000) >> 16);
        master_clock_db_ptr->grandmasterClockClass   = (UINT8)((spram_var & 0x0000FF00) >> 8);
        master_clock_db_ptr->grandmasterClockAccuracy= (UINT8)(spram_var & 0x000000FF);

        spram_var = cpup34k_reg_read(NULL, (pmc_handle_t *)cpup34k_handle, MEM_TYPE_CONFIG, (spram_addr_offset+0x8));
        master_clock_db_ptr->grandmasterOffsetScaledLogVariance = (UINT16)((spram_var & 0xFFFF0000) >> 16);

        spram_var = cpup34k_reg_read(NULL, (pmc_handle_t *)cpup34k_handle, MEM_TYPE_CONFIG, (spram_addr_offset+0x10));
        master_clock_db_ptr->grandMasterIndentity[0] = (UINT8)((spram_var & 0xFF000000) >> 24);
        master_clock_db_ptr->grandMasterIndentity[1] = (UINT8)((spram_var & 0x00FF0000) >> 16);
        master_clock_db_ptr->grandMasterIndentity[2] = (UINT8)((spram_var & 0x0000FF00) >> 8);
        master_clock_db_ptr->grandMasterIndentity[3] = (UINT8)(spram_var & 0xFF);

        spram_var = cpup34k_reg_read(NULL, (pmc_handle_t *)cpup34k_handle, MEM_TYPE_CONFIG, (spram_addr_offset+0x14));
        master_clock_db_ptr->grandMasterIndentity[4] = (UINT8)((spram_var & 0xFF000000) >> 24);
        master_clock_db_ptr->grandMasterIndentity[5] = (UINT8)((spram_var & 0x00FF0000) >> 16);
        master_clock_db_ptr->grandMasterIndentity[6] = (UINT8)((spram_var & 0x0000FF00) >> 8); 
        master_clock_db_ptr->grandMasterIndentity[7] = (UINT8)(spram_var & 0xFF);

        spram_var = cpup34k_reg_read(NULL, (pmc_handle_t *)cpup34k_handle, MEM_TYPE_CONFIG, (spram_addr_offset+0x18));
        master_clock_db_ptr->sourcePortClockIdentity[0] = (UINT8)((spram_var & 0xFF000000) >> 24);
        master_clock_db_ptr->sourcePortClockIdentity[0] = (UINT8)((spram_var & 0x00FF0000) >> 16);
        master_clock_db_ptr->sourcePortClockIdentity[0] = (UINT8)((spram_var & 0x0000FF00) >> 8);
        master_clock_db_ptr->sourcePortClockIdentity[0] = (UINT8)(spram_var & 0xFF);

        spram_var = cpup34k_reg_read(NULL, (pmc_handle_t *)cpup34k_handle, MEM_TYPE_CONFIG, (spram_addr_offset+0x1c));
        master_clock_db_ptr->sourcePortClockIdentity[0] = (UINT8)((spram_var & 0xFF000000) >> 24);
        master_clock_db_ptr->sourcePortClockIdentity[0] = (UINT8)((spram_var & 0x00FF0000) >> 16);
        master_clock_db_ptr->sourcePortClockIdentity[0] = (UINT8)((spram_var & 0x0000FF00) >> 8); 
        master_clock_db_ptr->sourcePortClockIdentity[0] = (UINT8)(spram_var & 0xFF);

        spram_var = cpup34k_reg_read(NULL, (pmc_handle_t *)cpup34k_handle, MEM_TYPE_CONFIG, (spram_addr_offset+0x20));
        master_clock_db_ptr->srcPortNumber[0] = (UINT8)((spram_var & 0xFF000000) >> 24);
        master_clock_db_ptr->srcPortNumber[1] = (UINT8)((spram_var & 0x00FF0000) >> 16);

        spram_var = cpup34k_reg_read(NULL, (pmc_handle_t *)cpup34k_handle, MEM_TYPE_CONFIG, (spram_addr_offset+0x28));
        master_clock_db_ptr->timeProperties = (UINT8)((spram_var & 0xFF000000) >> 24);
        master_clock_db_ptr->timeSource = (UINT8)((spram_var & 0x00FF0000) >> 16);
    } 

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_master_clock_db_get */ 
/*******************************************************************************
* digi_timesync_ptp_parent_data_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to set Boundary Clock mode parent data properties.
*
* INPUTS:
*  *digi_handle      - pointer to DIGI handle instance
*  *parent_data_ptr  - pointer to the parent data properties
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Parent_Data
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_parent_data_set (digi_handle_t *digi_handle,
                                                    ptp_parent_data_t *parent_data_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[10];

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != parent_data_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);      

    /*  CMD_CFG_PTP_Set_Parent_Data has the following format,
     * Word   |  bits 24..31           | bits 16..23             | bits 8..15            |  bits 0..7  |
     *   0    | stepsRemoved                                     | Reserved                                       |
     *   1    | parentClockIdentity[0] | parentClockIdentity[1]  | parentClockIdentity[2]| parentClockIdentity[3] |
     *   2    | parentClockIdentity[4] | parentClockIdentity[5]  | parentClockIdentity[6]| parentClockIdentity[7] |
     *   3    | parentPortNumber[0]    | parentPortNumber[1]     | Reserved                                       |
     *   4    | grandmasterPriority1   | grandmasterPriority2    | Reserved                                       |
     *   5    | grandmasterClockClass  | grandmasterClockAccuracy| grandmasterOffsetScaledLogVariance             |
     *   6    | grandmasterIdentity[0] | grandmasterIdentity[1]  | grandmasterIdentity[2] | grandmasterIdentity[3]|
     *   7    | grandmasterIdentity[4] | grandmasterIdentity[5]  | grandmasterIdentity[6] | grandmasterIdentity[7]|
     *   8    | unicastDA[0]           | unicastDA[1]            | unicastDA[2]           | unicastDA[3]          |
     *   9    | unicastDA[4]           | unicastDA[5]            | Reserved                                       |
     */
    data[0] = ((parent_data_ptr->stepsRemoved) << 16);

#if defined(PMC_BIG_ENDIAN)
    parent_data_ptr->parentClockIdentity = PMC_BE_TO_CPU_64(parent_data_ptr->parentClockIdentity);
    data[1] = (UINT32)(parent_data_ptr->parentClockIdentity >> 32);
    data[2] = (UINT32)parent_data_ptr->parentClockIdentity;
#else
    parent_data_ptr->parentClockIdentity= PMC_LE_TO_CPU_64(parent_data_ptr->parentClockIdentity);
    data[1] = (UINT32)parent_data_ptr->parentClockIdentity;
    data[2] = (UINT32)(parent_data_ptr->parentClockIdentity>>32);	
#endif


    data[3] = ((parent_data_ptr->parentPortNumber) << 16);

    data[4] = (((parent_data_ptr->grandmasterPriority1) << 24) | 
               ((parent_data_ptr->grandmasterPriority2) << 16));

    data[5] = (((parent_data_ptr->grandmasterClockClass) << 24) | 
               ((parent_data_ptr->grandmasterClockAccuracy) << 16) |
               (parent_data_ptr->grandmasterOffsetScaledLogVariance));

#if defined(PMC_BIG_ENDIAN)
    parent_data_ptr->grandmasterIdentity = PMC_BE_TO_CPU_64(parent_data_ptr->grandmasterIdentity);	
    data[6] = (UINT32)(parent_data_ptr->grandmasterIdentity >> 32);
    data[7] = (UINT32)parent_data_ptr->grandmasterIdentity;
#else
    parent_data_ptr->grandmasterIdentity= PMC_LE_TO_CPU_64(parent_data_ptr->grandmasterIdentity);
    data[6] = (UINT32)parent_data_ptr->grandmasterIdentity;
    data[7] = (UINT32)(parent_data_ptr->grandmasterIdentity>>32);
#endif

    data[8] = (((parent_data_ptr->unicastDA[0]) << 24) | 
               ((parent_data_ptr->unicastDA[1]) << 16) |
               ((parent_data_ptr->unicastDA[2]) << 8)  | 
               (parent_data_ptr->unicastDA[3]));

    data[9] = (((parent_data_ptr->unicastDA[4]) << 24) | 
               ((parent_data_ptr->unicastDA[5]) << 16));

    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_PARENT_DATA_ST,data,10,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_parent_data_set */ 
/*******************************************************************************
* digi_timesync_ptp_port_data_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to set Boundary Clock mode port data properties.
*
* INPUTS:
*  *digi_handle    - pointer to DIGI handle instance
*  *port_data_ptr  - pointer to the BC mode port data properties
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Port_Data.
*        All ports are set using the values specified in this function call
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_port_data_set (digi_handle_t *digi_handle,
                                                  ptp_port_data_t *port_data_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[3];

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != port_data_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);      


    /*  CMD_CFG_PTP_Set_Port_Data has the following format,
     * Word   |  bits 24..31          | bits 16..23             | bits 8..15            |  bits 0..7        |
     *   0    |logMinDelayReqInterval | logAnnounceInterval     | announceReceiptTimeout| logSyncInterval   |
     *   1    |delayMechanism         | logMinPdelayReqInterval | qualificationTimeoutN | syncReceiptTimeout|
     *   2    |delayRespReceiptTimeout| Reserved                                                            |
     */
    data[0] = (((port_data_ptr->logMinDelayReqInterval)<< 24) | 
               ((port_data_ptr->logAnnounceInterval)   << 16) |
               ((port_data_ptr->announceReceiptTimeout)<< 8)  | 
               (port_data_ptr->logSyncInterval));

    data[1] = (((port_data_ptr->delayMechanism)<< 24) | 
               ((port_data_ptr->logMinPdelayReqInterval) << 16) |
               ((port_data_ptr->qualificationTimeoutN) << 8)  | 
               (port_data_ptr->syncReceiptTimeout));

    data[2] = ((port_data_ptr->delayRespReceiptTimeout) << 24);

    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_PORT_DATA_ST,data,3,stat);
    } 

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_port_data_set */ 
/*******************************************************************************
* digi_timesync_ptp_port_state_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to set states of all Boundary Clock mode ports.
*
* INPUTS:
*  *digi_handle    - pointer to DIGI handle instance
*  *port_state_ptr - pointer to the mode ports' states
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Port_State
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_port_state_set (digi_handle_t *digi_handle,
                                                   ptp_port_state_t *port_state_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    UINT32 data[DIGI_ENET_CHNL_MAX];
    UINT32 i;

    PMC_ATOMIC_ENTRY(digi_handle);
     
    PMC_ASSERT(NULL != port_state_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);      

    PMC_MEMSET((void *)data, 0, sizeof(data) );
    /* set PTP port state for the specified and provisioned channels */
    for (i = 0; i < DIGI_ENET_CHNL_MAX && PMC_SUCCESS == rv; i++)
    {
        if (port_state_ptr->chnl_handle[i] != NULL)
        {
            /* validate channel handle pointer */
            rv = digi_enet_validate_parms(digi_handle, port_state_ptr->chnl_handle[i], &enet_handle, &link);
            /*  CMD_CFG_PTP_Set_Port_State has the following format,
             * Word |  bits 24..31       | bits 16..23       | bits 8..15 |  bits 0..7  |
             *   0  |  chnl0 port_state  | chnl0 ptsfStatus  |   Reserved              |
             *   1  |  chnl1 port_state  | chnl1 ptsfStatus  |   Reserved              |                                 
             * 2-10 |   ...                                                            |
             *  11  |  chnl11 port_state | chnl11 ptsfStatus |   Reserved              |  
            */
            if (PMC_SUCCESS == rv && link < DIGI_ENET_CHNL_MAX) {
                data[link] = ((port_state_ptr->state[i].portState << 24) |
                              (port_state_ptr->state[i].ptsfStatus << 16));
            }
        }
    }
    
    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_PORT_STATE_ST,data,DIGI_ENET_LINE_CHNL_MAX,stat);
    }   

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_port_state_set */ 
/*******************************************************************************
* digi_timesync_ptp_port_event_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to set the DESIGNATED_ENABLED and DESIGNATED_DISABLED
*   event on the port of the PTP Boundary Clock mode.
*
* INPUTS:
*  *digi_handle    - pointer to DIGI handle instance
*  *event_ptr      - pointer to the port event data
*  
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Port_Event.
*        Only one port of the whole device may be configured as a slave (including IRIGI).
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_port_event_set (digi_handle_t *digi_handle,
                                                   ptp_port_event_t *event_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[1];
    enet_handle_t* enet_handle;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != event_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);   

    /*  CMD_CFG_PTP_Set_Port_Event has the following format,
     * Word |  bits 24..31 | bits 16..23 | bits 8..15 |  bits 0..7  |
     *   0  |  enetChannel |  Reserved   | enableFlag |  Reserved   |
     */
    /* dereference enet channel handle pointer to extract channel ID */
    rv = digi_enet_validate_parms(digi_handle, event_ptr->chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rv)
    {
        data[0] = ((link << 24) | (event_ptr->enableFlag << 8));
    }
    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_PORT_EVENT_ST,data,1,stat);
    }

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_port_event_set */ 

/*******************************************************************************
* digi_timesync_ptp_port_state_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to get states of all Boundary Clock mode ports.
*
* INPUTS:
*  *digi_handle    - pointer to DIGI handle instance
*  *port_state_ptr - pointer to the PTP ports' states
* OUTPUTS:
*  *port_state_ptr - pointer to the PTP ports' states
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_REQ_PTP_Get_Port_State. 
*        User application may use this function to get the port state periodically
*        and take recover actions if the port is in the FAULTY state
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_port_state_get (digi_handle_t *digi_handle,
                                                   ptp_port_state_t *port_state_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    enet_handle_t *enet_handle = NULL;
    UINT32 link;
    UINT32 i;
    UINT32 data[1] = {0};
    UINT32 response[DIGI_ENET_CHNL_MAX];

    PMC_ATOMIC_ENTRY(digi_handle);
     
    PMC_ASSERT(NULL != port_state_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        rv = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_REQ_PTP_GET_PORT_STATE_ST,data,1,response,18,stat);
    }
    /* get PTP port states for the specified and provisioned channels */
    for (i = 0; i < DIGI_ENET_CHNL_MAX && PMC_SUCCESS == rv; i++)
    {
        if (port_state_ptr->chnl_handle[i] != NULL)
        {
            /* validate channel handle pointer */
            rv = digi_enet_validate_parms(digi_handle, port_state_ptr->chnl_handle[i], &enet_handle, &link);
            /* RSP_REQ_PTP_Get_Port_State has the following format,
             * Word |  bits 24..31       | bits 16..23               | bits 8..15  | bits 0..7 |
             *   0  |  chnl0 port_state  | announceReceiptTimeoutFlag|  ptsfStatus | Reserved  |
             *   1  |  chnl1 port_state  | announceReceiptTimeoutFlag|  ptsfStatus | Reserved  |                             
             * 2-10 |   ...                                                                    |
             *  11  |  chnl11 port_state | announceReceiptTimeoutFlag|  ptsfStatus | Reserved  |   
             */
            if (PMC_SUCCESS == rv && link < DIGI_ENET_CHNL_MAX) {
                port_state_ptr->state[i].portState  = (UINT8)((response[link] & 0xFF000000) >> 24);
                port_state_ptr->state[i].announceReceiptTimeoutFlag = (UINT8)((response[link] & 0xFF0000) >> 16);
                port_state_ptr->state[i].ptsfStatus = (UINT8)((response[link] & 0xFF00) >> 8);
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_port_state_get */ 
/*******************************************************************************
* digi_timesync_ptp_passive_port_parent_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to specify the port identity of the PTP master
*   that passive port should listen to.
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  *passive_port_ptr  - pointer to the Boundary Clock mode passive port
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Passive_Port_Parent.
*        The passive port will restart its Announce Receipt Timer
*        when an Announce message is received from the master
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_passive_port_parent_set (digi_handle_t *digi_handle,
                                                            ptp_passive_port_parent_t *passive_port_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[4];

    enet_handle_t *enet_handle;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != passive_port_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);      

    /*  CMD_CFG_PTP_Set_Passive_Port_Parent has the following format,
     * Word   |  bits 24..31     | bits 16..23     | bits 8..15       |  bits 0..7  |
     *   0    | enetChannel      | Reserved        | Reserved                       |
     *   1    | clockIdentity 32..64                                                |
     *   2    | clockIdentity 0..31                                                 |
     *   3    | portNumber                         | Reserved                       |
     */
    /* retrieve enet channel ID */
    rv = digi_enet_validate_parms(digi_handle, passive_port_ptr->chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rv)
    {
        data[0] = (link << 24);
   
#if defined(PMC_BIG_ENDIAN)
        passive_port_ptr->clockIdentity = PMC_BE_TO_CPU_64(passive_port_ptr->clockIdentity);		
        data[1] = (UINT32)(passive_port_ptr->clockIdentity >> 32);
        data[2] = (UINT32)(passive_port_ptr->clockIdentity);
#else
        passive_port_ptr->clockIdentity = PMC_LE_TO_CPU_64(passive_port_ptr->clockIdentity);
        data[1] = (UINT32)passive_port_ptr->clockIdentity;
        data[2] = (UINT32)(passive_port_ptr->clockIdentity >> 32);
#endif
        data[3]= (passive_port_ptr->portNumber << 16);
    }
    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_PASSIVE_PORT_PARENT_ST,data,1,stat);
    }


    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_passive_port_parent_set */
/*******************************************************************************
* digi_timesync_ptp_unicast_master_param_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to specify the parameters for the unicast message negotiation.
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  *master_unicast_param_ptr  - pointer to the unicast parameters
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Unicast_Master_Param
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_unicast_master_param_set (digi_handle_t *digi_handle,
                                                             ptp_master_unicast_param_t *master_unicast_param_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[4];
    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != master_unicast_param_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);      

    /*  CMD_CFG_PTP_Set_Unicast_Master_Param has the following format,
     * Word  |  bits 24..31           | bits 16..23                | bits 8..15                       | bits 0..7 |
     *   0    | logInterMessagePeriod | logInterMessagePeriod(Sync)| logInterMessagePeriod(Delay_Resp)| Reserved  |
     *   1    | durationField (Announce)                                                                          |
     *   2    | durationField (Sync)                                                                              |
     *   3    | durationField (Delay_Resp)                                                                        |
     */
    data[0] = ((master_unicast_param_ptr->logInterMessagePeriod_announce << 24) |
                 (master_unicast_param_ptr->logInterMessagePeriod_sync << 16) |
                 (master_unicast_param_ptr->logInterMessagePeriod_delay_resp << 8));
    data[1] = master_unicast_param_ptr->durationField_announce;
    data[2] = master_unicast_param_ptr->durationField_sync;
    data[3] = master_unicast_param_ptr->durationField_delay_resp;

    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_UNICAST_MASTER_PARAM_ST,data,4,stat);
    } 

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_unicast_master_param_set */

/*******************************************************************************
* digi_timesync_ptp_unicast_request_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to start the transmission of the REQUEST_UNICAST_TRANSMISSION TLV
*   for a port in the LISTENING, PASSIVE, UNCALIBRATED or SLAVE state.
*   A port will resend the REQUEST_UNICAST_TRANSMISSION TLV two more times at one second interval
*   if a GRANT_UNICST_TRANSMISSION TLV is not received.
*
* INPUTS:
*  *digi_handle               - pointer to DIGI handle instance
*  *unicast_request_param_ptr - pointer to the unicast slave parameters
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Unicast_Request
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_unicast_request_set (digi_handle_t *digi_handle,
                                                        ptp_unicast_request_t *unicast_request_param_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[5];

    enet_handle_t *enet_handle;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);
     
    PMC_ASSERT(NULL != unicast_request_param_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);      

    /*  CMD_CFG_PTP_Set_Unicast_Request has the following format,
     * Word |  bits 24..31 | bits 16..23                  | bits 8..15                 |  bits 0..7                      |
     *   0  | messageType  | logInterMessagePeriod(Annoc) |logInterMessagePeriod(Sync) |logInterMessagePeriod(Delay_Resp)|
     *   1  | durationField (Announce)                                                                                   |
     *   2  | durationField (Sync)                                                                                       |
     *   3  | durationField (Delay_Resp)                                                                                 |
     *   4  | enetChannel  | reserved                     | continuous                  | individual_tlv                 |
     */

    /* retrieve enet channel ID */
    rv = digi_enet_validate_parms(digi_handle, unicast_request_param_ptr->chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rv)
    {
        data[0] = ((unicast_request_param_ptr->messageType << 24) |
                     (unicast_request_param_ptr->logInterMessagePeriod_announce << 16) |
                     (unicast_request_param_ptr->logInterMessagePeriod_sync << 8) |
                     (unicast_request_param_ptr->logInterMessagePeriod_delay_resp));

        data[1] = unicast_request_param_ptr->durationField_announce;
        data[2] = unicast_request_param_ptr->durationField_sync;
        data[3] = unicast_request_param_ptr->durationField_delay_resp;
        data[4] = ((link << 24) | 
                         (unicast_request_param_ptr->continuous << 8) |
                         (unicast_request_param_ptr->individual_tlv));
    }

    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_UNICAST_REQUEST_ST,data,4,stat);
    } 

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_unicast_request_set */
/*******************************************************************************
* digi_timesync_ptp_unicast_cancel_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to trigger master or slave to transmit the CANCEL_UNICAST_TRANSMISSION TLV.
*
* INPUTS:
*  *digi_handle              - pointer to DIGI handle instance
*  *unicast_cancel_param_ptr - pointer to unicast cancel parameters
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Unicast_Cancel
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_unicast_cancel_set (digi_handle_t *digi_handle,
                                                       ptp_unicast_cancel_t *unicast_cancel_param_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[28];
    enet_handle_t* enet_handle;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);


    /*  CMD_CFG_PTP_Set_Unicast_Cancel has the following format,
     * Word |  bits 24..31 | bits 16..23  | bits 8..15    | bits 0..7  |
     *   0  | messageType  |  enetChannel |     Reserved  |  Reserved  |
     */

    rv = digi_enet_validate_parms(digi_handle, unicast_cancel_param_ptr->chnl_handle, &enet_handle, &link);

    PMC_MEMSET(data,0,sizeof(data));
    if (PMC_SUCCESS == rv)
    {
        data[0] = ((unicast_cancel_param_ptr->messageType << 24) |
                     (link << 16));
    }

    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_UNICAST_CANCEL_ST,data,28,stat);
    } 

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_unicast_cancel_set */
/*******************************************************************************
* digi_timesync_ptp_unicast_slave_state_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to get the unicast slave state of the PTP Boundary Clock mode.
*
* INPUTS:
*  *digi_handle              - pointer to DIGI handle instance
*  *slave_unicast_param_ptr  - pointer to the unicast slave parameters
* OUTPUTS:
*  *slave_unicast_param_ptr  - pointer to the unicast slave parameters
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_REQ_PTP_Get_Unicast_Slave_State
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_unicast_slave_state_get (digi_handle_t *digi_handle,
                                                            ptp_slave_unicast_param_t *slave_unicast_param_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[1] = {0};
    UINT32 response[5];

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != slave_unicast_param_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        rv = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_REQ_PTP_GET_UNICAST_SLAVE_STATE_ST,data,1,response,5,stat);
    }

    /* The third word of RSP_REQ_PTP_Get_Unicast_Slave_State has the following format,
     * Word   |  bits 24..31  | bits 16..23            | bits 8..15             | bits 0..7                    |
     *   0    | Reserved      |logInterMsgPeriod(Annou)| logInterMsgPeriod(Sync)| logInterMsgPeriod(Delay_Resp)|
     *   1    | durationField (Announce)                                                                       |
     *   2    | durationField (Sync)                                                                           |
     *   3    | durationField (Delay_Resp)                                                                     |
     *   4    | enetChannel   | Reserved               | Reserved                                              |
     */

    if (PMC_SUCCESS == rv)
    {

        /* Return requested data. */
        slave_unicast_param_ptr->logInterMessagePeriod_announce = (UINT8)((response[0] & 0xFF0000) >> 16);
        slave_unicast_param_ptr->logInterMessagePeriod_sync = (UINT8)((response[0] & 0xFF00) >> 8);
        slave_unicast_param_ptr->logInterMessagePeriod_delay_resp = (UINT8)(response[0] & 0xFF);
        slave_unicast_param_ptr->durationField_announce = response[1];
        slave_unicast_param_ptr->durationField_sync = response[2];
        slave_unicast_param_ptr->durationField_delay_resp = response[3];
        ((digi_enet_client_chnl_def_t *)slave_unicast_param_ptr->chnl_handle)->switch_data.cpb_data.channel = (UINT8)((response[4] & 0xFF000000) >> 24);
    }

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_unicast_slave_state_get */

/*******************************************************************************
* digi_timesync_ptp_pass_thru_general_param_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to set the general PTP pass-thru parameters.
*
* INPUTS:
*  *digi_handle              - pointer to DIGI handle instance
*  *ptp_pass_thru_param_ptr  - pointer to the PTP pass-thru parameters
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Pass_Thru_General_Param
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_pass_thru_general_param_set (digi_handle_t *digi_handle,
                                                                ptp_pass_thru_general_param_t *ptp_pass_thru_param_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[6];

    PMC_ATOMIC_ENTRY(digi_handle);
           
    PMC_ASSERT(NULL != ptp_pass_thru_param_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* Build the rest of the message to the prevously created header.*/
    data[0] = (ptp_pass_thru_param_ptr->vlanTci << 16);

    data[1] = ((ptp_pass_thru_param_ptr->externalHostMacAddr[0] << 24) | 
                     (ptp_pass_thru_param_ptr->externalHostMacAddr[1] << 16) |
                     (ptp_pass_thru_param_ptr->externalHostMacAddr[2] << 8)  | 
                      ptp_pass_thru_param_ptr->externalHostMacAddr[3]);
    data[2] = ((ptp_pass_thru_param_ptr->externalHostMacAddr[4] << 24) | 
                     (ptp_pass_thru_param_ptr->externalHostMacAddr[5] << 16));

    data[3] = ((ptp_pass_thru_param_ptr->mipsMacAddr[0] << 24) | 
                     (ptp_pass_thru_param_ptr->mipsMacAddr[1] << 16) |
                     (ptp_pass_thru_param_ptr->mipsMacAddr[2] << 8)  | 
                      ptp_pass_thru_param_ptr->mipsMacAddr[3]);
    data[4] = ((ptp_pass_thru_param_ptr->mipsMacAddr[4] << 24) | 
                     (ptp_pass_thru_param_ptr->mipsMacAddr[5] << 16));

    data[5] = (ptp_pass_thru_param_ptr->vlanPresent << 16);


    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_PASS_THRU_GENERAL_PARAM_ST,data,6,stat);
    } 
    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_pass_thru_general_param_set */

/*******************************************************************************
* digi_timesync_ptp_pass_thru_port_param_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set the port PTP pass-thru parameters.
*
* INPUTS:
*  *digi_handle              - pointer to DIGI handle instance
*  *ptp_pass_thru_param_ptr  - pointer to the PTP port pass-thru parameters
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_Pass_Thru_Port_Param
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_pass_thru_port_param_set (digi_handle_t *digi_handle,
                                                             ptp_pass_thru_port_param_t *ptp_pass_thru_param_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[13];
    enet_handle_t *enet_handle;
    UINT32        link; 

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);        
    PMC_ASSERT(NULL != ptp_pass_thru_param_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    rv = digi_enet_validate_parms(digi_handle, ptp_pass_thru_param_ptr->chnl_handle,
                                  &enet_handle, &link);

    if (PMC_SUCCESS == rv)
    {
        /* Build the rest of the message to the prevously created header.*/
        data[0] = ((link << 24) | (ptp_pass_thru_param_ptr->msgInterface << 16));

        data[1] = (ptp_pass_thru_param_ptr->peerMeanPathDelay >> 32) & 0xffffffff;
        data[2] = ptp_pass_thru_param_ptr->peerMeanPathDelay & 0xffffffff;

        data[3] = digi_timesync_ptp_pass_thru_port_create_event_cfg_field(&(ptp_pass_thru_param_ptr->sync_ingress),
                                                                                &(ptp_pass_thru_param_ptr->sync_egress));
        data[4] = digi_timesync_ptp_pass_thru_port_create_event_cfg_field(&(ptp_pass_thru_param_ptr->delay_req_ingress),
                                                                                &(ptp_pass_thru_param_ptr->delay_req_egress));
        data[5] = digi_timesync_ptp_pass_thru_port_create_event_cfg_field(&(ptp_pass_thru_param_ptr->pdelay_req_ingress),
                                                                                &(ptp_pass_thru_param_ptr->pdelay_req_egress));
        data[6] = digi_timesync_ptp_pass_thru_port_create_event_cfg_field(&(ptp_pass_thru_param_ptr->pdelay_resp_ingress),
                                                                                &(ptp_pass_thru_param_ptr->pdelay_resp_egress));

        data[7] = digi_timesync_ptp_pass_thru_port_create_general_cfg_field(&(ptp_pass_thru_param_ptr->follow_up_ingress));
        data[8] = digi_timesync_ptp_pass_thru_port_create_general_cfg_field(&(ptp_pass_thru_param_ptr->delay_resp_ingress));
        data[9] = digi_timesync_ptp_pass_thru_port_create_general_cfg_field(&(ptp_pass_thru_param_ptr->pdelay_resp_follow_up_ingress));
        data[10] = digi_timesync_ptp_pass_thru_port_create_general_cfg_field(&(ptp_pass_thru_param_ptr->announce_ingress));
        data[11] = digi_timesync_ptp_pass_thru_port_create_general_cfg_field(&(ptp_pass_thru_param_ptr->signaling_ingress));
        data[12] = digi_timesync_ptp_pass_thru_port_create_general_cfg_field(&(ptp_pass_thru_param_ptr->management_ingress));
    }

    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_PASS_THRU_PORT_PARAM_ST,data,13,stat);
    } 

    PMC_ATOMIC_RETURN(digi_handle, rv);

} /* digi_timesync_ptp_pass_thru_port_param_set */

/*******************************************************************************
* digi_timesync_ptp_tc_param_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to set the PTP Transparent Clock parameters.
*
* INPUTS:
*  *digi_handle              - pointer to DIGI handle instance
*  *ptp_tc_param_ptr         - pointer to the PTP TC parameters
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_TC_Param
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_tc_param_set (digi_handle_t *digi_handle,
                                                 ptp_tc_param_t *ptp_tc_param_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT16 chnl_bitmask = 0;
    enet_handle_t *enet_handle;
    UINT32        link; 
    UINT32        i; 
    UINT32 data[5];

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);        
    PMC_ASSERT(NULL != ptp_tc_param_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* extract channel IDs from the channel handle pointer array and create a bit mask */
    for (i = 0; i < DIGI_ENET_LINE_CHNL_MAX && PMC_SUCCESS == rv; i++)
    {  
        if (ptp_tc_param_ptr->chnl_handle[i] != NULL)
        {      
            rv = digi_enet_validate_parms(digi_handle, ptp_tc_param_ptr->chnl_handle[i],
                                          &enet_handle, &link);
            if (PMC_SUCCESS == rv)
            {
                chnl_bitmask |= 1 << (link % 16);
            }
        }
    }

    /* specify what ENET channels are used for PTP */
    chnl_bitmask &= ptp_tc_param_ptr->ptpChnlBitmask;

    /* CMD_CFG_PTP_Set_TC_Param has the following format,
     * Word   |  bits 24..31          | bits 16..23           | bits 8..15            | bits 0..7               |
     *   0    | masterClockIdentity[0]| masterClockIdentity[1]| masterClockIdentity[2]| masterClockIdentity[3]  |
     *   1    | masterClockIdentity[4]| masterClockIdentity[5]| masterClockIdentity[6]| masterClockIdentity[7]  |
     *   2    | masterPortNumber[0]   | masterPortNumber[1]   | enableSynt            |  resetEnetChannelMask   |
     *   3    |                 enetChannelMask               |                ipSaEgressCtrlMask               |
     *   4    |               ipDaIngressCtrlMask             |                Reserved1                        |
     */

    if (PMC_SUCCESS == rv)
    {
#if defined(PMC_BIG_ENDIAN)
        ptp_tc_param_ptr->masterClockIdentity = PMC_BE_TO_CPU_64(ptp_tc_param_ptr->masterClockIdentity);
        data[0] = (UINT32)(ptp_tc_param_ptr->masterClockIdentity >> 32);
        data[1] = (UINT32)ptp_tc_param_ptr->masterClockIdentity;		
#else
        ptp_tc_param_ptr->masterClockIdentity = PMC_LE_TO_CPU_64(ptp_tc_param_ptr->masterClockIdentity);
        data[0] = (UINT32)ptp_tc_param_ptr->masterClockIdentity;
        data[1] = (UINT32)(ptp_tc_param_ptr->masterClockIdentity >> 32);
#endif
        data[2] = ((ptp_tc_param_ptr->masterPortNumber << 16) | 
                   (ptp_tc_param_ptr->enableSynt << 8)|
                   ptp_tc_param_ptr->resetEnetChannelMask);

        data[3] = ((chnl_bitmask << 16) |
                   ptp_tc_param_ptr->ipSaEgressCtrlMask);

        data[4] = (ptp_tc_param_ptr->ipDaIngressCtrlMask << 16);
    }

    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_TC_PARAM_ST,data,5,stat);
    } 
  

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_tc_param_set */
/*******************************************************************************
* digi_timesync_ptp_tc_port_param_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to configure parameters for each port of a Transparent Clock.
*
* INPUTS:
*  *digi_handle              - pointer to DIGI handle instance
*  *ptp_tc_port_param_ptr    - pointer to the PTP TC port parameters
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_CFG_PTP_Set_TC_Port_Param
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_tc_port_param_set (digi_handle_t *digi_handle,
                                                      ptp_tc_port_param_t *ptp_tc_port_param_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;

    UINT16 chnl_bitmask = 0;
    enet_handle_t *enet_handle;
    UINT32        link; 
    UINT32        i; 
    UINT32 data[1];

    PMC_ATOMIC_ENTRY(digi_handle);
           
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);  
    PMC_ASSERT(NULL != ptp_tc_port_param_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    /*  CMD_CFG_PTP_Set_TC_Port_Param has the following format,
     * Word |  bits 24..31 | bits 16..23  | bits 8..15              | bits 0..7  |
     *   0  | enetChannelMask             | logMinPdelayReqInterval |   Reserved |
     */
    /* extract channel IDs from the channel handle pointer array and create a bit mask */
    for (i = 0; i < DIGI_ENET_LINE_CHNL_MAX && PMC_SUCCESS == rv; i++)
    {  
        if (ptp_tc_port_param_ptr->chnl_handle[i] != NULL)
        {
            rv = digi_enet_validate_parms(digi_handle, ptp_tc_port_param_ptr->chnl_handle[i],
                                          &enet_handle, &link);
            if (PMC_SUCCESS == rv)
            {
                chnl_bitmask |= 1 << (link % 16);
            }
        }
    }

    if (PMC_SUCCESS == rv)
    {
        data[0] = ((chnl_bitmask << 16) |
                     (ptp_tc_port_param_ptr->logMinPdelayReqInterval << 8));
    }


    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        rv = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_CFG_PTP_SET_TC_PORT_PARAM_ST,data,1,stat);
    } 
  

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_tc_port_param_set */
/*******************************************************************************
* digi_timesync_ptp_stability_status_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to get the PTP algorithm statibility status.
*
* INPUTS:
*  *digi_handle           - pointer to DIGI handle instance
*  *stability_data_ptr    - pointer to PTP stability status structure
* OUTPUTS:
*  *stability_data_ptr    - pointer to PTP stability status structure 
* RETURNS:
*   PMC_SUCCESS
*
* NOTES: CMD_REQ_PTP_Get_Stability_Status
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_stability_status_get (digi_handle_t *digi_handle,
                                                         ptp_stability_status_t *stability_data_ptr)
{
    PMC_ERROR rv = PMC_SUCCESS;
    UINT32 data[1];
    UINT32 response[1];
    enet_handle_t *enet_handle;
    UINT32 link;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);        
    PMC_ASSERT(NULL != stability_data_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    /*  CMD_REQ_PTP_Get_Stability_Status has the following format,
     * Word |  bits 24..31 | bits 16..23             | bits 8..15    | bits 0..7  |
     *   0  | enetChannel  | phase_offset_threshold  |  interval     |  Reserved  |
     */

    /* retrieve enet channel ID */
    rv = digi_enet_validate_parms(digi_handle, stability_data_ptr->chnl_handle, &enet_handle, &link);

    if (PMC_SUCCESS == rv)
    {
        data[0] = ((link << 24) |
                  (stability_data_ptr->phase_offset_threshold << 16) |
                  (stability_data_ptr->interval << 8));
    }

    /* Send message*/
    if (PMC_SUCCESS == rv)
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        rv = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_PTP,HOSTMSG_CMD_REQ_PTP_GET_STABILITY_STATUS_ST,data,1,response,1,stat);
    }

    /*  RSP_REQ_PTP_Get_Stability_Status has the following format,
     * Word |  bits 24..31    | bits 16..23      | bits 8..15 | bits 0..7  |
     *   0  |stability_status |    phase_offset  |   Reserved              |
     */

    if (PMC_SUCCESS == rv)
    {
        UINT8 value;

        /* Return requested data. */
        value = (UINT8)((response[0]  & 0xFF000000) >> 24);

        if (0 == value)
        {
            stability_data_ptr->status = PTP_STATUS_NOT_STABLE;
        }
        else if (1 == value)
        {
            stability_data_ptr->status = PTP_STATUS_STABLE;
        }
        else
        {
            stability_data_ptr->status = LAST_PTP_STATUS;
        }
        stability_data_ptr->phase_offset = (UINT8)((response[0] & 0xFF0000) >> 16);
    } 

    PMC_ATOMIC_RETURN(digi_handle, rv);
} /* digi_timesync_ptp_stability_status_get */


/*******************************************************************************
*  digi_timesync_synce_cfg_ssm()
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This method sends host message to FW to configure the SyncE
*               mode and SSM quality level.
*
*  INPUTS:
*               digi_handle - System handle.
*               synce_ctrl_ptr - Pointer to the SyncE configurations. The
*                                configuration can contain mulitple channels.
*
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_SYNCE_Config
*
******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_synce_cfg_ssm(digi_handle_t *digi_handle,
                                             digi_synce_ctrl_t *synce_ctrl_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    enet_handle_t *enet_handle;
    UINT32 link;
    UINT32 i;
    UINT32 data[SYNCE_CHNL_MAX * SYNCE_CONFIG_BODY_INFO_PER_CH_LEN];


    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != synce_ctrl_ptr), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    /* initialize data array as no SYNCE channels will be used
    */
    PMC_MEMSET ((void *)&data, 0, sizeof(data));
    for (i = 0; i < SYNCE_CHNL_MAX; i++)
    {
        data[0 + (i*SYNCE_CONFIG_BODY_INFO_PER_CH_LEN)] = (0xFF << 24);
    }
    /* get the total number of channels and gather ENET channel IDs */
    for (i =0; i < SYNCE_CHNL_MAX && PMC_SUCCESS == result; i++) 
    {
        if (synce_ctrl_ptr->ch_cfg[i].chnl_handle != NULL)
        {
            result = digi_enet_validate_parms(digi_handle,
                                              synce_ctrl_ptr->ch_cfg[i].chnl_handle,
                                              &enet_handle, &link);
            /* if ENET parameters are not validated or if link is out of the range,
            *  assign it to the 0xFF */
            if (PMC_SUCCESS != result || link >= SYNCE_CHNL_MAX)
            {
                link = 0xFF;
            }
            /* CMD_CFG_SYNCE_Config has the following format,
            * word 1-4 can be repeated depends on the ch_cnt:
            * Word |  bits 24..31 | bits 16..23  | bits 8..15 | bits 0..7  |
            *   0  |   enet_ch    |   mode       |  reserved               |
            *   1  |   QL_type    |   QL_length               |   QL_SSM   |
            *   2  |   MAC_SA[0]  |   MAC_SA[1]  |  MAC_SA[2] |  MAC_SA[3] |               
            *   3  |   MAC_SA[4]  |   MAC_SA[5]  |  reserved               |
            */
            data[0 + (i*SYNCE_CONFIG_BODY_INFO_PER_CH_LEN)] = ((link << 24) | 
                                                                ((synce_ctrl_ptr->ch_cfg[i].mode) << 16));
            /* QL_type and QL_length are always 0x1 and 0x4.
               QL_ssm is lower 4bit of Byte3 in the word */
            data[1 + (i*SYNCE_CONFIG_BODY_INFO_PER_CH_LEN)] = ((SYNCE_QL_TLV_TYPE << 24) | 
                                                                (SYNCE_QL_TLV_LEN << 8) | 
                                                                ((synce_ctrl_ptr->ch_cfg[i].ql_ssm) & 0xf));
            /* Higher 32 bit of 48-bit MAC source address */
            data[2 + (i*SYNCE_CONFIG_BODY_INFO_PER_CH_LEN)] = ((synce_ctrl_ptr->ch_cfg[i].mac_sa[0] << 24) | 
                                                                (synce_ctrl_ptr->ch_cfg[i].mac_sa[1] << 16) |
                                                                (synce_ctrl_ptr->ch_cfg[i].mac_sa[2] << 8) |
                                                                (synce_ctrl_ptr->ch_cfg[i].mac_sa[3]));
            /* Lower 16 bit of 48-bit MAC source address */
            data[3 + (i*SYNCE_CONFIG_BODY_INFO_PER_CH_LEN)] = ((synce_ctrl_ptr->ch_cfg[i].mac_sa[4] << 24) |
                                                                (synce_ctrl_ptr->ch_cfg[i].mac_sa[5] << 16));
        }
    }

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_SYNC_E,HOSTMSG_CMD_CFG_SYNCE_CONFIG_CH_ST,data,(sizeof(data)/sizeof(UINT32)),stat);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_timesync_synce_cfg_ssm */

/*******************************************************************************
*  digi_timesync_synce_ch_en()
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This method sends host message to FW to enable or disable the
*               SyncE SSM feature after configuration the channel's SSM.
*               It is important to ensure that digi_timesync_synce_cfg_ssm()
*               is called before calling this method to enable the SyncE SSM
*               feature. By design, the firmware only processes the Ethernet
*               packets from either ENET_LINE or ENET_SYS interface, but not
*               from both interfaces at the same time.
*               Note that all channel handle pointers provided in 'chnl_handle_ptr'
*               array will be applicable for the action chosen by 'enable'
*               argument.
*
*  INPUTS:
*           digi_handle     - System handle.
*           chnl_handle_ptr - Pointer to an array of ENET channel handles
*           enable          - Boolean to indicate if we are enabling SyncE channels
*                             or disabling SyncE channels.
*                             TRUE -- enable SyncE channels
*                             FALSE -- disable SyncE channels
*                             Note: The value of 'enable' action applies to all
*                                   channel handle pointers provided in the
*                                   '*chnl_handle_ptr' array.
*
*  OUTPUTS:
*           None.
* 
*  RETURNS:
*           PMC_SUCCESS if everything went OK, number of error codes
*           otherwise.
*
*  NOTES:
*           CMD_CFG_SYNCE_Control_Channel
*
******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_synce_ch_en(digi_handle_t *digi_handle,
                                           util_global_switch_data_t *chnl_handle_ptr[SYNCE_CHNL_MAX],
                                           BOOL8 enable)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT16 ch_enable_bit_msk = 0;
    UINT16 ch_disable_bit_msk = 0;

    enet_handle_t *enet_handle;
    enet_handle_t *enet_handle_prev = NULL;
    UINT32 link;
    UINT32 i;
    UINT32 data[2];
	cpb_mcpb_mux_mode_t mcpb_mux_type;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);

    /* CMD_CFG_SYNCE_Control_Channel has the following format,
     * Word |  bits 24..31 | bits 16..23  | bits 8..15 | bits 0..7  |
     *   0  |   reserved                   |   ch_enable_bit_msk   |
     *   1  |   reserved                   |   ch_disable_bit_msk  |              
     */

    /* process chnl_handle_ptr array and create a mask for FW */
    for (i = 0; i < SYNCE_CHNL_MAX; i++)
    {
        if (chnl_handle_ptr[i] != NULL)
        {
            
            result = digi_enet_validate_parms(digi_handle, chnl_handle_ptr[i], &enet_handle, &link);
            /* check that handles belongs to the same ENET SS */
            if (PMC_SUCCESS == result)
            {
                if (NULL != enet_handle_prev &&
                    enet_handle_prev != enet_handle)
                {
                    result = DIGI_ERR_INVALID_ARG;
                    break;
                }
            }    
          
            if (PMC_SUCCESS == result)
            {
                /* update FW enet client source */
                if (0 == ch_enable_bit_msk &&
                    0 == ch_disable_bit_msk)
                {
                    result = digi_enet_fw_enet_client_source_conditional_update(digi_handle, chnl_handle_ptr[i]);

					/* Make sure that the MCPB is set up for this ENET subsystem */
					mcpb_mux_type = cpb_mcpb_mux_mode_init_get(digi_handle->mcpb_handle);
					if (mcpb_mux_type == CPB_MCPB_MUX_MODE_ENET_LINE)
					{
						if ( ((digi_enet_client_chnl_def_t *)chnl_handle_ptr[i])->switch_data.cpb_data.port_type != UTIL_GLOBAL_CPB_PORT_ENET_LINE ) {
							result = DIGI_ERR_MCPB_INVALID_PORT;
						}
					} else if (mcpb_mux_type == CPB_MCPB_MUX_MODE_ENET_SYS) {
						if ( ((digi_enet_client_chnl_def_t *)chnl_handle_ptr[i])->switch_data.cpb_data.port_type != UTIL_GLOBAL_CPB_PORT_ENET_SYS ) {
							result = DIGI_ERR_MCPB_INVALID_PORT;
						}
					} else {
						/* CPB_MCPB_MUX_MODE_DISABLED */
						result = DIGI_ERR_MCPB_INVALID_PORT;
					}
                }

                if (PMC_SUCCESS == result)
                {
                    if (enable)
                    {
                        ch_enable_bit_msk |= 1 << (link % 16);
                    }
                    else {
                        ch_disable_bit_msk |= 1 << (link % 16);
                    }
                }
                enet_handle_prev = enet_handle;
            }
        }
    }

    
    /* Only lower 16 bit of ch_enable_bit_msk and ch_disable_bit_msk contains information */
   
    data[0] = ch_enable_bit_msk & 0xFFFF;
    data[1] = ch_disable_bit_msk & 0xFFFF;


    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_SYNC_E,HOSTMSG_CMD_CFG_SYNCE_CTRL_CH_ST,data,2,stat);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_timesync_synce_ch_en */


/*******************************************************************************
*  digi_timesync_synce_req_ssm()
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This method sends host message to FW to request SSM status
*               from all the Ethernet channels on the device. Once the SyncE
*               channels are enabled, this method should be called periodically
*               to determine if there is a change of the SSM code from the
*               remote network element. There is also an indicator provided 
*               by this function to indicate the lack of reception of an ESMC
*               information PDU within a five-second period. \n
*               Note that the Event Flag returned by this function goes to '1'
*               when an Event PDU is received, and the Event Flag stored in the
*               firmware is cleared after the host message is sent to firmware. \n
*
*  INPUTS:
*               digi_handle - System handle.
*
*  OUTPUTS:
*               synce_status_ptr - Pointer to the replied SyncE status
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_REQ_SYNCE_Get_SSM
*
******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_synce_req_ssm(digi_handle_t *digi_handle,
                                             digi_synce_info_t *synce_status_ptr)
{  
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    UINT32 rsp_word;
    UINT32 response[37];
    UINT32 data[1] = {0};

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != synce_status_ptr), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);


    /*  CMD_CFG_SYNCE_Get_SSM has the following format,
     * Word |  bits 24..31 | bits 16..23 | bits 8..15 | bits 0..7  |
     *   2  |   reserved                                           |           
     */
    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_QUERY_STAT(stat);
        result = hostmsg_perform_fixed_length_query((pmc_handle_t *)digi_handle,MODID_SYNC_E,HOSTMSG_CMD_REQ_SYNCE_GET_SSM_ST,data,1,response,37,stat);
    }

    /* The format of RSP_REQ_SYNCE_Get_SSM Word 1-3 are repeated.                  
     * Word |  bits 24..31 | bits 16..23  | bits 8..15                | bits 0..7 |
     *   0  |  ch_cnt      |  reserved                                            |
     *   1  |  enet_ch     |  reserved                                            | 
     *   2  |  QL_type     |  QL_length                               |  QL_SSM   |
     *   3  |  event_flag  |  mode        | rx_esmc_timeout_indicator |  enable   |
     */

    /* Get result*/
    if (PMC_SUCCESS == result)
    {
        /* skip data[0] */

        /* Get SSM status for the provisioned channels - up to SYNCE_CHNL_MAX */
        for (i =0; i < SYNCE_CHNL_MAX; i++) {
            if (synce_status_ptr->ch_status[i].chnl_handle != NULL) {
                /* Get ENET channel ID */
                rsp_word = response[ (i*SYNCE_RX_SSM_BODY_INFO_PER_CH_LEN)];
                ((digi_enet_client_chnl_def_t *) (synce_status_ptr->ch_status[i].chnl_handle))->switch_data.cpb_data.channel = (UINT8)((rsp_word & SYNCE_RSP_MSG_ENET_CH_MSK) >> 
                                                                                                                                        SYNCE_RSP_MSG_CH_OFF);

                /* Get QL_TLV*/
                rsp_word = response[ (i*SYNCE_RX_SSM_BODY_INFO_PER_CH_LEN) + 1];
                synce_status_ptr->ch_status[i].rx_ql_ssm= (UINT8)((rsp_word & SYNCE_RSP_MSG_QL_SSM_MSK) >> 
                                                                  SYNCE_RSP_MSG_QL_SSM_OFF);

                /* Get event flag*/
                rsp_word = response[ (i*SYNCE_RX_SSM_BODY_INFO_PER_CH_LEN) + 2];
                synce_status_ptr->ch_status[i].rx_event_flag= (UINT8)((rsp_word & SYNCE_RSP_MSG_EVENT_MSK) >> 
                                                                      SYNCE_RSP_MSG_EVENT_OFF);

                /* Get mode */
                synce_status_ptr->ch_status[i].mode= (UINT8)((rsp_word & SYNCE_RSP_MSG_MODE_MSK) >> 
                                                             SYNCE_RSP_MSG_MODE_OFF);

                /* Get rx_esmc_timeout_indicator */
                synce_status_ptr->ch_status[i].rx_esmc_timeout_indicator= (BOOL8)((rsp_word & SYNCE_RSP_MSG_LOST_CON_MSK) >> 
                                                                                  SYNCE_RSP_MSG_LOST_CON_OFF);

                /* Get channel enable indicator */
                synce_status_ptr->ch_status[i].ch_en= (BOOL8)((rsp_word & SYNCE_RSP_MSG_CH_EN_MSK) >> 
                                                              SYNCE_RSP_MSG_CH_EN_OFF);
            }
        }
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_timesync_synce_req_ssm */



/*******************************************************************************
*  digi_timesync_synce_tx_ssm()
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*               This method sends host message to FW to send an event ESMC PDU.
*               It is important to ensure digi_fw_enet_client_source_cfg() is
*               called before calling this method to send an Event ESMC PDU.
*               The digi_fw_enet_client_source_cfg() function on only needs to
*               be called once during the datapath provisioning. It is not
*               required to call digi_fw_enet_client_source_cfg() every time this
*               function is called to send Event ESMC PDU.
*
*  INPUTS:
*               digi_handle - System handle.
*               synce_ctrl_ptr - Pointer to the SyncE configurations
*
*  OUTPUTS:
*               None.
* 
*  RETURNS:
*               PMC_SUCCESS if everything went OK, number of error codes
*               otherwise.
*
*  NOTES:
*               CMD_CFG_SYNCE_Transmit_SSM
*
******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_synce_tx_ssm(digi_handle_t *digi_handle,
                                            digi_synce_ctrl_t *synce_ctrl_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    enet_handle_t *enet_handle;
    UINT32 link;
    UINT32 i;
    UINT32 data[SYNCE_CHNL_MAX * SYNCE_TX_SSM_BODY_INFO_PER_CH_LEN];

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT((NULL != synce_ctrl_ptr), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT((NULL != digi_handle), HOSTMSG_ERR_INVALID_PARAMETERS, 0, 0);
    /* initialize data array as no SYNCE channels will be used     
    */
    PMC_MEMSET ((void *)&data, 0, sizeof(data));
    for (i = 0; i < SYNCE_CHNL_MAX; i++)
    {
        data[0 + (i*SYNCE_TX_SSM_BODY_INFO_PER_CH_LEN)] = (0xFF << 24);
    }
    /* Count the total number of channels and gather their channel IDs */
    for (i =0; i < SYNCE_CHNL_MAX && PMC_SUCCESS == result; i++) 
    {
        if (synce_ctrl_ptr->ch_cfg[i].chnl_handle != NULL)
        {
            result = digi_enet_validate_parms(digi_handle, 
                                              synce_ctrl_ptr->ch_cfg[i].chnl_handle, 
                                              &enet_handle, &link);
            /* parameters are not validated or if link is out of the range,
             * assign it to 0xFF */
            if (PMC_SUCCESS != result || link >= SYNCE_CHNL_MAX)
            {
                link = 0xFF;
            }

            result =  digi_fw_enet_client_source_cfg(digi_handle, (ENET_LINE == enet_handle->cfg.enet_type_id) ? UTIL_GLOBAL_CPB_PORT_ENET_LINE : UTIL_GLOBAL_CPB_PORT_ENET_SYS);

            /* CMD_CFG_SYNCE_Transmit_SSM has the following format,
             * word 1-4 can be repeated depends on the ch_cnt:
             * Word |  bits 24..31 | bits 16..23   | bits 8..15 | bits 0..7  |
             *   0  |   enet_ch    |   event_flag  |  reserved               |
             *   1  |   QL_type    |   QL_length                |  QL_SSM    |
             *   2  |   MAC_SA[0]  |   MAC_SA[1]   |  MAC_SA[2] |  MAC_SA[3] |               
             *   3  |   MAC_SA[4]  |   MAC_SA[5]   |  reserved               |
             */
            /* Note that, data[0] is not used as enet_ch (link) value is used to configure SYNCE channels */
            data[0 + (i*SYNCE_TX_SSM_BODY_INFO_PER_CH_LEN)] = ((link << 24) | 
                                                               ((synce_ctrl_ptr->ch_cfg[i].event_flag) << 16));
            /* QL_type and QL_length are always 0x1 and 0x4.
               QL_ssm is the lower 4bit */
            data[1 + (i*SYNCE_TX_SSM_BODY_INFO_PER_CH_LEN)] = ((SYNCE_QL_TLV_TYPE << 24) | 
                                                               (SYNCE_QL_TLV_LEN << 8) | 
                                                               ((synce_ctrl_ptr->ch_cfg[i].ql_ssm) & 0xf));

            /* Higher 32 bit of 48-bit MAC source address */
            data[2 + (i*SYNCE_TX_SSM_BODY_INFO_PER_CH_LEN)] = ((synce_ctrl_ptr->ch_cfg[i].mac_sa[0] << 24) | 
                                                               (synce_ctrl_ptr->ch_cfg[i].mac_sa[1] << 16) |
                                                               (synce_ctrl_ptr->ch_cfg[i].mac_sa[2] << 8) |
                                                               (synce_ctrl_ptr->ch_cfg[i].mac_sa[3]));

            /* Lower 16 bit of 48-bit MAC source address */
            data[3 + (i*SYNCE_TX_SSM_BODY_INFO_PER_CH_LEN)] = ((synce_ctrl_ptr->ch_cfg[i].mac_sa[4] << 24) |
                                                               (synce_ctrl_ptr->ch_cfg[i].mac_sa[5] << 16));
        }
    }

    /* Send message*/
    if (PMC_SUCCESS == result)
    {
        HOSTMSG_DECLARE_CFG_MSG_STAT(stat);
        result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,MODID_SYNC_E,HOSTMSG_CMD_CFG_SYNCE_TX_SSM_ST,data,(sizeof(data)/sizeof(UINT32)),stat);
    } 


    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_timesync_synce_tx_ssm */

/*******************************************************************************
* digi_timesync_ptp_pkt_filter_cfg 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function setup PTP packet filterings in ingress and egress 
*   direction. User has to provide a set of ptp_pkt_filtering_param_t data 
*   structure  where it specifies the transport protocol, the packet address 
*   to filter and an associated bit-level mask to wild-card bits in the provided 
*   address. Supported transport procotols are PTP over Ethernet/IPv4 and Ipv6. 
*   It is assumed that addresses are source addresses in egress direction and 
*   destination addresses in ingress direction. 
*   
*   This function allows setting 8 different filters per direction and per 
*   transport protocol.
*   
*   A succesfull call to this function will override the previous configuration 
*   and it can be called with 0 filters to clear the configuration.  
*
*   This function setup filtering configuration that is actually applied when 
*   you call digi_timesync_ptp_tc_param_set. Filter config is applied when you set
*   ipSaEgressCtrl or ipDaIngressCtrl to filter respectively on egress or ingress 
*   direction.
*  
* INPUTS:
*  *digi_handle - pointer to the digi handle
*  direction - DIGI_ENET_INGRESS: configure ingress packet filtering
*              DIGI_ENET_EGRESS: configure egress packet filtering
*  ptp_pkt_filtering_param - an array of packet filters 
*  num_ptp_pkt_filter - Number of packet filters
* 
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS if everything went OK
*   DIGI_ERR_PTP_ECLASS_RESOURCE_EXCEEDED - when requested configuration cannot
*   fit in avaiable hardware ressource 
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_pkt_filter_cfg (digi_handle_t *digi_handle,
                                                   digi_enet_classification_direction_mode_t direction,
                                                   ptp_pkt_filtering_param_t ptp_pkt_filtering_param[],
                                                   UINT32 num_ptp_pkt_filter)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 filter_itr = 0, protocol_itr = 0, pfilter_itr = 0;
    enet_set_pkt_filter_msg_t enet_set_pkt_filter_msg;
    UINT32 filter_cnt_check[LAST_PTP_TRANSPORT_PROTOCOL_TYPE];
    HOSTMSG_DECLARE_CFG_MSG_STAT(stat);

    PMC_ATOMIC_ENTRY(digi_handle);

    /* first check if we are not exceeding the number of supported filters */
    filter_cnt_check[PTP_OVER_UDP_OVER_IPV4] = 0;
    filter_cnt_check[PTP_OVER_UDP_OVER_IPV6] = 0;
    filter_cnt_check[PTP_OVER_802_3] = 0;
    for (filter_itr=0;
         filter_itr< num_ptp_pkt_filter && PMC_SUCCESS == rc;
         filter_itr++)
    {
        PMC_ASSERT(ptp_pkt_filtering_param[filter_itr].protocol < LAST_PTP_TRANSPORT_PROTOCOL_TYPE, DIGI_ERR_INVALID_ARG,ptp_pkt_filtering_param[filter_itr].protocol,0);
        filter_cnt_check[ptp_pkt_filtering_param[filter_itr].protocol] += 1;
        if (filter_cnt_check[ptp_pkt_filtering_param[filter_itr].protocol] > DIGI_PKT_ENET_FILTER_MAX)
        {
            rc = DIGI_ERR_PTP_PKT_FILTER_RESOURCE_EXCEEDED;
        }
    }
        
    /* generate config commands */
    if (PMC_SUCCESS == rc)
    {
        /* 1 FW config command per protocol */
        for (protocol_itr = 0; protocol_itr < (UINT32) LAST_PTP_TRANSPORT_PROTOCOL_TYPE && PMC_SUCCESS == rc; protocol_itr++)
        {
            /* first clear previous filters */
            enet_set_pkt_filter_msg.clear = 0x1;
            enet_set_pkt_filter_msg.ingress_egress = (direction == DIGI_ENET_INGRESS)?1:0;
            enet_set_pkt_filter_msg.protocol = (ptp_transport_protocol_enum) protocol_itr;  
            enet_set_pkt_filter_msg.filter_idx = 0; /* DON't care */
            
            rc = hostmsg_send_cfg_message((pmc_handle_t*)digi_handle,
                                          MODID_ENET,
                                          HOSTMSG_CMD_CFG_ENET_SET_PKT_FILTER_ST,
                                          (UINT32*) &enet_set_pkt_filter_msg,
                                          sizeof(enet_set_pkt_filter_msg_t)/4,stat);


            if (0 != filter_cnt_check[protocol_itr] && PMC_SUCCESS == rc)
            {
                pfilter_itr = 0;
                /* we have some filters to configure */
                enet_set_pkt_filter_msg.clear = 0x0;
                enet_set_pkt_filter_msg.ingress_egress = (direction == DIGI_ENET_INGRESS)?1:0;
                enet_set_pkt_filter_msg.protocol = (ptp_transport_protocol_enum) protocol_itr;
                enet_set_pkt_filter_msg.filter_idx = 0;
                
                for (filter_itr = 0;filter_itr < num_ptp_pkt_filter && PMC_SUCCESS == rc; filter_itr++)
                {
                    if ((UINT32) ptp_pkt_filtering_param[filter_itr].protocol == protocol_itr)
                    {
                        enet_set_pkt_filter_msg.filter_idx =  pfilter_itr;
                        memcpy(&enet_set_pkt_filter_msg.value[0], 
                               &ptp_pkt_filtering_param[filter_itr].addr[0], sizeof(ptp_pkt_filtering_param[filter_itr].addr));
                        memcpy(&enet_set_pkt_filter_msg.mask[0], 
                               &ptp_pkt_filtering_param[filter_itr].mask[0], sizeof(ptp_pkt_filtering_param[filter_itr].mask));
                        /* specify ENET channel bitmask */
                        enet_set_pkt_filter_msg.channel_bitmask = ptp_pkt_filtering_param[filter_itr].channel_bitmask;
                        /* configure filter pfilter_itr */
                        rc = hostmsg_send_cfg_message((pmc_handle_t*)digi_handle,
                                          MODID_ENET,
                                          HOSTMSG_CMD_CFG_ENET_SET_PKT_FILTER_ST,
                                          (UINT32*) &enet_set_pkt_filter_msg,
                                          sizeof(enet_set_pkt_filter_msg_t)/4,stat);

                        pfilter_itr += 1;
                    }
                }                    
            }
        }
    }
    PMC_ATOMIC_RETURN(digi_handle,rc);

} /* digi_timesync_ptp_gen_pkt_filter_cfg */


/*******************************************************************************
* digi_timesync_ptp_capt_debug_data_start
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Starts capturing and retrieves debug data
*  
* INPUTS:
*  digi_handle - DIGI device handle.
*  
* OUTPUTS:
*  None
**  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_capt_debug_data_start (digi_handle_t *digi_handle)
{
    PMC_ERROR result;
    struct {
        UINT32 dbg_data_subsystem;
    } cfg_msg;

    HOSTMSG_DECLARE_QUERY_STAT(stat);

    PMC_ATOMIC_ENTRY(digi_handle);

    cfg_msg.dbg_data_subsystem = HOSTMSG_CMD_CFG_DBG_LOG_DBG_DATA_PTP_TS_RX; 
    result = hostmsg_send_cfg_message((pmc_handle_t *)digi_handle,
                                      MODID_DBG_LOG,
                                      DBG_LOG_HOSTMSG_STYPE_START_DBG_DATA_CAPT,
                                      (UINT32 *)&cfg_msg,
                                      (sizeof(cfg_msg)/sizeof(UINT32)),
                                      stat);

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_timesync_ptp_capt_debug_data_start */

/*******************************************************************************
* digi_timesync_ptp_capt_debug_data_get
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Get the captured debug data
*  
* INPUTS:
*  digi_handle - DIGI device handle.
*  
* OUTPUTS:
*  debug_addr - SPRAM address of debug variables.
*  debug_size - size of debug variables.
*
*  RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_timesync_ptp_capt_debug_data_get (digi_handle_t *digi_handle,
                                                        uintptr_t *debug_addr,
                                                        UINT32 *debug_size)
{
    struct {
        UINT32 dbg_data_subsystem;
    } cfg_msg;
    struct {
        UINT32 addr;
        UINT32 size;
    } response;
    PMC_ERROR result;
    HOSTMSG_DECLARE_QUERY_STAT(stat);

    PMC_ATOMIC_ENTRY(digi_handle);

    cfg_msg.dbg_data_subsystem = HOSTMSG_CMD_CFG_DBG_LOG_DBG_DATA_PTP_TS_RX; 
 
    result = hostmsg_perform_fixed_length_query((pmc_handle_t*)digi_handle,
                                                MODID_DBG_LOG,
                                                DBG_LOG_HOSTMSG_STYPE_GET_DBG_DATA_ADDR,
                                                (UINT32 *)&cfg_msg,
                                                1,
                                                (UINT32 *)&response,
                                                (sizeof(response)/sizeof(UINT32)),
                                                stat);

    /* note this function returns a spram address and the size of the structure in spram */
    if (PMC_SUCCESS == result) {
        /* if the firmware reports that the size is larger than the buffer we will be using,
           then limit the size to just the size fw_opsa_mpmo_debug_t */
        if (response.size > sizeof(ptp_dbg_pkt_t))
        {
            response.size = sizeof(ptp_dbg_pkt_t);
        }
        /* mask the firmware addresses with 0x1FFFFFF to convert from MIPS bus addresses to PCIE bus addresses */
        response.addr = CONVERT_MIPS_ADDR_TO_HOST_ADDR (response.addr);

        *debug_addr = response.addr;
        *debug_size = response.size;
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_timesync_ptp_capt_debug_data_get */

#ifndef DOXYGEN_PUBLIC_ONLY

/*******************************************************************************
* digi_timesync_ptp_pass_thru_port_create_event_cfg_field()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates the host message input field for PTP event messages
*
* INPUTS:
*  *ingress_cfg_ptr    - Pointer to ingress configuration struct
*  *egress_cfg_ptr     - Pointer to egress configuration struct
*
* OUTPUTS:
*   None
*
* RETURNS:
*   32-bit host message field
*
* NOTES: CMD_CFG_PTP_Set_Pass_Thru_Port_Param
*
*******************************************************************************/
PRIVATE UINT32 digi_timesync_ptp_pass_thru_port_create_event_cfg_field (ptp_pass_thru_event_ingress_cfg *ingress_cfg_ptr,
                                                                        ptp_pass_thru_event_egress_cfg *egress_cfg_ptr)
{
    UINT32 cfg_field = 0;

    PMC_ENTRY();

    /* Create ingress configuration field */
    cfg_field |= ((ingress_cfg_ptr->peerMeanPathDelay_update & 0x1) << 10) |
                 ((ingress_cfg_ptr->asymmetry_corr & 0x1) << 8) |
                 ((ingress_cfg_ptr->corrField_update & 0x1) << 4) |
                  (ingress_cfg_ptr->process_option & 0x3);
    cfg_field = cfg_field << 16;

    /* Create egress configuration field */
    cfg_field |= ((egress_cfg_ptr->asymmetry_corr & 0x1) << 8) |
                 ((egress_cfg_ptr->tstmp_reflect & 0x3) << 4) |
                  (egress_cfg_ptr->tstmp_corrField_update & 0x3);

    PMC_RETURN(cfg_field);

} /* digi_timesync_ptp_pass_thru_port_create_event_cfg_field */

/*******************************************************************************
* digi_timesync_ptp_pass_thru_port_create_general_cfg_field()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates the host message input field for PTP general messages
*
* INPUTS:
*  *ingress_cfg_ptr    - Pointer to ingress configuration struct
*
* OUTPUTS:
*   None
*
* RETURNS:
*   32-bit host message field
*
* NOTES: CMD_CFG_PTP_Set_Pass_Thru_Port_Param
*
*******************************************************************************/
PRIVATE UINT32 digi_timesync_ptp_pass_thru_port_create_general_cfg_field (ptp_pass_thru_general_ingress_cfg *ingress_cfg_ptr)
{
    UINT32 cfg_field = 0;

    PMC_ENTRY();

    /* Create ingress configuration field */
    cfg_field |= (ingress_cfg_ptr->process_option & 0x1);
    cfg_field = cfg_field << 16;

    PMC_RETURN(cfg_field);

} /* digi_timesync_ptp_pass_thru_port_create_general_cfg_field */
#endif /* DOXYGEN_PUBLIC_ONLY */

/*
** End of file
*/
