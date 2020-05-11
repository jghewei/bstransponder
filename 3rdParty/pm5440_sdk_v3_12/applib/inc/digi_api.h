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
*   This files contains definitions for usage in systems
*   using the SW driver.
*
*   NOTES:
*
*******************************************************************************/
#ifndef _DIGI_API_H
#define _DIGI_API_H

#ifdef __cplusplus
extern "C" {
#endif


/*
** Include Files
*/
#include "pmc_types.h"
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "pmc_sys.h"

#include "coreotn_api.h"
#include "odu_struct_api.h"
#include "cbrc_api.h"
#include "cpb_api.h"
#include "enet_api.h"
#include "mgmt_fege_api.h"
#include "mapotn_api.h"
#include "lineotn_api.h"
#include "util_global_api.h"
#include "util_opsa_api.h"
#include "sifd_api.h"
#include "s16_api.h"
#include "t8_api.h"
#include "c8_api.h"
#include "d8_tx_2x_api.h"
#include "d8_rx_2x_api.h"
#include "sfi51_d8_rx_18x_api.h"
#include "sfi51_d8_tx_18x_api.h"
#include "cpuif_api.h"


/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: digi_mapping_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for otn payload mapping modes.  The element
*   terms have a syntax: \n
*      DIGI_MAP_[odu container]_[client_data_type]_[mapping_mode] \n\n
*
*   In the case of 10GE, 40GE and 100GE applications, there are cases where the
*   term PKT and CBR and ENET are used within the adaptation functions.
*   In these cases *   there terms are used to denote:\n\n
*
*   CBR:  A transportation application whereby the signal is transferred in
*   cbr format through CPB.\n
*   PKT:  A termination application whereby the ethernet signal is terminated
*   and packet data is transferred to CPB.\n
*   ENET: The opposite endpoint of the datapath will be either ENET_SYS or
*   ENET_LINE.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum digi_mapping_mode_t
{
    DIGI_MAP_ODUKP_PACKET_GFP         = 0,
    DIGI_MAP_ODU4P_100_GE_GFP,
    DIGI_MAP_ODU4P_100_GE_GFP_ENET,
    DIGI_MAP_ODU3P_40_GE_GFP,
    DIGI_MAP_ODU3P_40_GE_GFP_ENET,
    DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP,
    DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET,
    DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP,
    DIGI_MAP_ODU2P_10_GE_6_2_GFP,
    DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET,
    DIGI_MAP_ODU2EP_FC_1200_GFP,
    DIGI_MAP_ODU2P_FC_800_GMP,
    DIGI_MAP_ODU2EP_10G_GDPS_GMP,
    DIGI_MAP_ODU2P_5G_GDPS_GMP,
    DIGI_MAP_ODU2P_CPRI_5_GMP,
    DIGI_MAP_ODU2P_CPRI_10_GMP,
    DIGI_MAP_ODU2P_CPRI_6_GMP,
    DIGI_MAP_ODU3P_CBR_40_GE_GMP,
    DIGI_MAP_ODU3P_PKT_40_GE_GMP,
    DIGI_MAP_ODU4P_CBR_100_GE_GMP,
    DIGI_MAP_ODU4P_PKT_100_GE_GMP,
    DIGI_MAP_ODUFLEX_5G_GDPS_BMP,
    DIGI_MAP_ODUFLEX_10G_GDPS_BMP,
    DIGI_MAP_ODUFLEX_CPRI_5_BMP,
    DIGI_MAP_ODUFLEX_CPRI_10_BMP,
    DIGI_MAP_ODUFLEX_CPRI_6_BMP,
    DIGI_MAP_ODUFLEX_FC_800_BMP,
    DIGI_MAP_ODU2FP_FC_1200_BMP,
    DIGI_MAP_ODU1FP_FC_1200_BMP,
    DIGI_MAP_ODU1EP_CBR_10G3_7_2_BMP,
    DIGI_MAP_ODU2EP_CBR_10G3_7_1_BMP,
    DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP,
    DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP,
    DIGI_MAP_ODU3P_OC_768_CBR_BMP,
    DIGI_MAP_ODU2P_OC_192_CBR_BMP,
    DIGI_MAP_ODU3P_OC_768_CBR_AMP,
    DIGI_MAP_ODU2P_OC_192_CBR_AMP,

    LAST_DIGI_MAP       /* this is for out of range values error handling */
} digi_mapping_mode_t;

/*******************************************************************************
* ENUM: digi_oduk_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for otn payload mapping modes.  The element
*   terms have a syntax:
*      DIGI_ODUK_[odu container]
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum {
    DIGI_ODUK_ODU0          = 0,
    DIGI_ODUK_ODU1,
    DIGI_ODUK_ODU1E,
    DIGI_ODUK_ODU1F,
    DIGI_ODUK_ODU2,
    DIGI_ODUK_ODU2E,
    DIGI_ODUK_ODU2F,
    DIGI_ODUK_ODU3,
    DIGI_ODUK_ODU3E1,
    DIGI_ODUK_ODU3E2,
    DIGI_ODUK_ODU4,
    DIGI_ODUK_ODUFLEX,

    LAST_DIGI_ODUK      /* this is for out of range values error handling */
} digi_oduk_t;

/*******************************************************************************
* ENUM: digi_otn_msi_cfg_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OPU MSI field configuration modes.
*
* ELEMENTS:
*   DIGI_OTN_MSI_CFG_AUTO   - SW driver configures MSI field as part of datapath
*                             provisioning
*   DIGI_OTN_MSI_CFG_MANUAL - SW driver does not configure MSI field as part
*                             of datapath provisioning. User must call separate
*                             APIs to set MSI field once datapath has been
*                             established.
*                              -- To set transmit path MSI field,
*                                 call digi_otn_opu_tx_msi_set().
*                              -- To set receive path MSI field,
*                                 call digi_otn_opu_rx_expected_msi_set().
*
*******************************************************************************/
typedef enum {
    DIGI_OTN_MSI_CFG_AUTO   = 0,
    DIGI_OTN_MSI_CFG_MANUAL,
    LAST_DIGI_OTN_MSI_CFG,
} digi_otn_msi_cfg_mode_t;

/*******************************************************************************
* ENUM: digi_otn_server_otuk_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for otn server OTUk type.  The element
*   terms have a syntax:
*      DIGI_OTN_SERVER_[server_type]
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum {
    DIGI_OTN_SERVER_OTU4          = 0,

    DIGI_OTN_SERVER_OTU3E2,
    DIGI_OTN_SERVER_OTU3E1,
    DIGI_OTN_SERVER_OTU3,
    DIGI_OTN_SERVER_OTU2F,
    DIGI_OTN_SERVER_OTU1F,
    DIGI_OTN_SERVER_OTU2E,
    DIGI_OTN_SERVER_OTU1E,
    DIGI_OTN_SERVER_OTU2,

    LAST_DIGI_OTN_SERVER        /* this is for out of range values error handling */
} digi_otn_server_otuk_t;

/*******************************************************************************
* ENUM: digi_otn_server_fec_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for otn server fec type.  The element
*   terms have a syntax:\n
*      DIGI_OTN_SERVER_FEC_[FEC data]
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    DIGI_OTN_SERVER_FEC_NONE         = (LINE_OTN_FEC_NONE),
    DIGI_OTN_SERVER_FEC_I4,
    DIGI_OTN_SERVER_FEC_I7,
    DIGI_OTN_SERVER_FEC_G709,
    DIGI_OTN_SERVER_FEC_SWIZZLE_100G,
    DIGI_OTN_SERVER_FEC_SWIZZLE_40G,
    LAST_DIGI_OTN_SERVER_FEC    /* this is for out of range values error handling */
} digi_otn_server_fec_t;

/*******************************************************************************
* ENUM: digi_enet_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for ENET_LINE major processing modes.  The
*   element terms have a syntax:\n
*      DIGI_ENET_[line interface]_[application]
*
* ELEMENTS:
*   DIGI_ENET_SERDES_100GE_MAC                            - Serdes 100GE MAC Terminated
*   DIGI_ENET_SERDES_100GE_TRANSPARENT                    - Serdes 100GE CBR
*   DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON            - Serdes 100GE CBR with Rx MAC providing inbound signal PMON
*   DIGI_ENET_SERDES_40GE_MAC                             - Serdes 40GE MAC Terminated
*   DIGI_ENET_SERDES_40GE_TRANSPARENT                     - Serdes 40GE CBR
*   DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON             - Serdes 40GE CBR with Rx MAC providing inbound signal PMON
*   DIGI_ENET_SERDES_10GE_MAC                             - Serdes 10GE MAC Terminated
*   DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT          - Serdes 10GE Gsup43 7.3
*   DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON  - Serdes 10GE Gsup43 7.3 with Rx MAC providing inbound signal PMON
*   DIGI_ENET_SERDES_10GE_TRANSPARENT                     - Serdes 10GE CBR
*   DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON             - Serdes 10GE CBR with Rx MAC providing inbound signal PMON
*
*******************************************************************************/
typedef enum
{
    DIGI_ENET_SERDES_100GE_MAC = 0,
    DIGI_ENET_SERDES_100GE_TRANSPARENT,
    DIGI_ENET_SERDES_100GE_TRANSPARENT_RX_PMON,
    DIGI_ENET_SERDES_40GE_MAC,
    DIGI_ENET_SERDES_40GE_TRANSPARENT,
    DIGI_ENET_SERDES_40GE_TRANSPARENT_RX_PMON,
    DIGI_ENET_SERDES_10GE_MAC,
    DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT,
    DIGI_ENET_SERDES_10GE_GSUP43_7_3_TRANSPARENT_RX_PMON,
    DIGI_ENET_SERDES_10GE_TRANSPARENT,
    DIGI_ENET_SERDES_10GE_TRANSPARENT_RX_PMON,

    LAST_DIGI_ENET      /* this is for out of range values error handling */
} digi_enet_mode_t;


/*******************************************************************************
* ENUM: digi_cpb_stream_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the client stream passing through
*   CPB.\n\n
*
*   The stream is specified based on the destination rate requirements.\n\n
*
*   SIFD and MAPOTN datapaths support all rates.  For MAPOTN the rate must match
*   the ODUk that the signal is being mapped into.\n\n
*
*   ENET_LINE, ENET_SYS and CBRC* are limited to the following rates:\n
*   DIGI_CPB_STREAM_10G for all 10G and sub 10G clients\n
*   DIGI_CPB_STREAM_40G for all 40G clients (40GE/STS768/STM256)\n
*   DIGI_CPB_STREAM_100G\n
*   *Note:  CBRC does not support CPB_STREAM_100G rate as no applications of this
*           rate supported. \n\n
*
*   The element terms have a syntax:\n
*      DIGI_CPB_STREAM_[target_container_rate]
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    DIGI_CPB_STREAM_1G25 = 0,
    DIGI_CPB_STREAM_2G5,
    DIGI_CPB_STREAM_FLEX,
    DIGI_CPB_STREAM_10G,
    DIGI_CPB_STREAM_40G,
    DIGI_CPB_STREAM_100G,

    LAST_DIGI_CPB_STREAM        /* this is for out of range values error handling */
} digi_cpb_stream_t;

/*******************************************************************************
* ENUM: digi_oduksw_stream_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the client stream passing through
*   ODUk Switch.
*   The element terms have a syntax:
*      DIGI_ODUKSW_STREAM_CBR_[application]
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    DIGI_ODUKSW_STREAM_CBR_ODU0 = 0,
    DIGI_ODUKSW_STREAM_CBR_ODU1,
    DIGI_ODUKSW_STREAM_CBR_ODU2,
    DIGI_ODUKSW_STREAM_CBR_ODU2E,
    DIGI_ODUKSW_STREAM_CBR_ODU2F,
    DIGI_ODUKSW_STREAM_CBR_ODU1E,
    DIGI_ODUKSW_STREAM_CBR_ODU1F,
    DIGI_ODUKSW_STREAM_CBR_ODU3,
    DIGI_ODUKSW_STREAM_CBR_ODU3E2,
    DIGI_ODUKSW_STREAM_CBR_ODU3E1,
    DIGI_ODUKSW_STREAM_CBR_ODU4,
    DIGI_ODUKSW_STREAM_CBR_ODUFLEX,
    DIGI_ODUKSW_STREAM_PKT_GFPF_ODUFLEX,

    LAST_DIGI_ODUKSW_STREAM        /* this is for out of range values error handling */
} digi_oduksw_stream_t;

/*******************************************************************************
* ENUM: digi_cbr_client_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the cbr client stream being
*   processed through CBRC subsystem
*   The element terms have a syntax:
*      DIGI_CBR_CLIENT_[client_rate]_[type]
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    DIGI_CBR_CLIENT_40G_STS768  = 0,
    DIGI_CBR_CLIENT_40G_STM256,
    DIGI_CBR_CLIENT_10G_STS192,
    DIGI_CBR_CLIENT_10G_STM64,
    DIGI_CBR_CLIENT_10G_FC1200_ODU1F,
    DIGI_CBR_CLIENT_10G_FC1200_ODU2F,
    DIGI_CBR_CLIENT_10G_FC1200_GFP_ODU2E,
    DIGI_CBR_CLIENT_10G_FC800,
    DIGI_CBR_CLIENT_10G_CPRI_5G,
    DIGI_CBR_CLIENT_10G_CPRI_6G,
    DIGI_CBR_CLIENT_10G_CPRI_10G,
    DIGI_CBR_CLIENT_10G_GDPS_5G,
    DIGI_CBR_CLIENT_10G_GDPS_10G,
    DIGI_CBR_CLIENT_10G_STS192_PRBS,
    DIGI_CBR_CLIENT_10G_STM64_PRBS,
    DIGI_CBR_CLIENT_10G_8B_10B_IDLE,
    DIGI_CBR_CLIENT_10G_64B_66B_IDLE,
    DIGI_CBR_CLIENT_10G_8B_10B_ARB,
    LAST_DIGI_CBR_CLIENT        /* this is for out of range values error handling */
} digi_cbr_client_mode_t;

/*******************************************************************************
* ENUM: digi_serdes_intf_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the serdes interface to be
*   configured for the registration of serdes ports to interface pins.  The valid
*   interfaces are Line Serdes, System Serdes and SFI 5.1 Serdes.
*
*
* ELEMENTS:
*   DIGI_SERDES_LINE_INTF   - Serdes Port Associated with Line Serdes
*   DIGI_SERDES_SYSTEM_INTF - Serdes Port Associated with System Serdes
*   DIGI_SERDES_SFI_51_INTF - Serdes Port Associated with SFI_51_INTF
*   DIGI_SERDES_BACKPLANE_LR_INTF - Serdes Port associated with backplane LR interface
*   DIGI_SERDES_BACKPLANE_SR_INTF - Serdes Port associated with backplane SR interface
*   DIGI_SERDES_EMGMT_INTF    - Serdes Port associated with ENET management interface
*   DIGI_SERDES_OTN_OH_INTF       - Serdes Port associated with OTN OH interface
*   DIGI_SERDES_RCP_INTF          - Serdes Port associated with RCP interface
*
*******************************************************************************/
typedef enum {

    DIGI_SERDES_LINE_INTF = 0,
    DIGI_SERDES_SYSTEM_INTF,
    DIGI_SERDES_SFI_51_INTF,
    DIGI_SERDES_BACKPLANE_LR_INTF,
    DIGI_SERDES_BACKPLANE_SR_INTF,
    DIGI_SERDES_EMGMT_INTF,
    DIGI_SERDES_OTN_OH_INTF,
    DIGI_SERDES_RCP_INTF,

    LAST_DIGI_SERDES_INTF      /* this is for out of range values error handling */

} digi_serdes_intf_t;

/*******************************************************************************
* ENUM: digi_serdes_prov_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the provisioned state of
*   resources against a Post MLD DSI Lane.
*
*
* ELEMENTS:
*   DIGI_SERDES_PROV_STATE_NO_SERDES      - No Serdes initialized against the Post
*                                           MLD DSI resource.
*   DIGI_SERDES_PROV_STATE_UNASSIGNED     - No subsystem processing provisioned
*                                           against the Post MLD DSI resource.
*   DIGI_SERDES_PROV_STATE_LINEOTN        - LINEOTN SS processing provisioned
*                                           against the Post MLD DSI resource.
*   DIGI_SERDES_PROV_STATE_ENET_LINE      - ENET_LINE SS processing provisioned
*                                           against the Post MLD DSI resource.
*   DIGI_SERDES_PROV_STATE_CBRC           - CBRC SS processing provisioned
*                                           against the Post MLD DSI resource.
*   DIGI_SERDES_PROV_STATE_ENET_SYS       - ENET_SYS SS processing provisioned
*                                           against the Post MLD DSI resource.
*
*******************************************************************************/
typedef enum {
    DIGI_SERDES_PROV_STATE_NO_SERDES = 0,
    DIGI_SERDES_PROV_STATE_UNASSIGNED,
    DIGI_SERDES_PROV_STATE_LINEOTN,
    DIGI_SERDES_PROV_STATE_ENET_LINE,
    DIGI_SERDES_PROV_STATE_CBRC,
    DIGI_SERDES_PROV_STATE_ENET_SYS,
    DIGI_SERDES_PROV_STATE_SYSOTN,

    LAST_DIGI_SERDES_PROV_STATE      /* this is for out of range values error handling */
} digi_serdes_prov_state_t;


/*******************************************************************************
* ENUM: digi_mapper_primary_group_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the internal resource group
*   MAPOTN allocates channel bandwidth against within the 120G processing
*   capacity of MAPOTN subsystem, or the datapath between ODUKSW and CPB
*   blocks.
*
*   that are reconfigurable between 1 x 40G and 4 x 10G operation of the
*   ENET_LINE subsystem. \n\n
*
*   A secondary group defines a 10G block of processing resources managed by
*   the MAPOTN subsystem.  It is defined relative to the 40G primary group.
*
*   Managing MAPOTN applications against secondary groups allows for systems to
*   deterministically allocate bandwidth to allow flexibilty in reconfiguring
*   ENET_LINE for usage with or without MAPOTN as part of the datapath.  Systems
*   will be able to know which active connections may be disrupted by performing
*   a configuration change to ENET_LINE.\n\n
*
*   If the MAPOTN application bandwidth exceeds the capacity of the primary
*   group, the primary group must be specified as
*   DIGI_MAPPER_PRIMARY_GROUP_DONT_CARE \n\n
*
*   The DONT_CARE option allows for cases where the channel rate to be scheduled
*   exceeds the bandwidth already allocated against the primary group 40G
*   container size.\n\n
*
*   For example if there are no resources provisioned against any group, and
*   a 100G application is to be configured, the primary group can be specified
*   as DONT_CARE.
*
*   For example if bandwidth is allocated against all primary
*   groups and an ODUFLEX of 12 x 0DU0 would need bandwidth from multiple
*   groups, this primary group would be DONT_CARE.\n\n
*
*   The DONT_CARE option can unconditionally be used in systems that:\n
*   - Do not use the ENET_LINE subsystem. \n
*   - Do not use modes that use the ENET_LINE and MAPOTN in conjunction. \n
*
*
* ELEMENTS:
*   DIGI_MAPPER_PRIMARY_GROUP_0           - Allocates MAPOTN bandwidth to the
*                                           first 40G primary group.
*   DIGI_MAPPER_PRIMARY_GROUP_1           - Allocates MAPOTN bandwidth to the
*                                           secondary 40G primary group.
*   DIGI_MAPPER_PRIMARY_GROUP_2           - Allocates MAPOTN bandwidth to the
*                                           third 40G primary group.
*   DIGI_MAPPER_PRIMARY_GROUP_DONT_CARE   - Allocate MAPOTN bandwidth to one or
*                                           more primary groups.
*
*******************************************************************************/
typedef enum {
    DIGI_MAPPER_PRIMARY_GROUP_0 = 0,
    DIGI_MAPPER_PRIMARY_GROUP_1,
    DIGI_MAPPER_PRIMARY_GROUP_2,
    DIGI_MAPPER_PRIMARY_GROUP_DONT_CARE,

    LAST_DIGI_MAPPER_PRIMARY_GROUP      /* this is for out of range values error handling */
} digi_mapper_primary_group_t;


/*******************************************************************************
* ENUM: digi_mapper_secondary_group_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the internal resource group
*   MAPOTN allocates channel bandwidth against within the 40G processing blocks
*   that are reconfigurable between 1 x 40G and 4 x 10G operation of the
*   ENET_LINE subsystem. \n\n
*
*   A secondary group defines a 10G block of processing resources managed by
*   the MAPOTN subsystem.  It is defined relative to the 40G primary group.
*
*   Managing MAPOTN applications against secondary groups allows for systems to
*   deterministically allocate bandwidth to allow flexibilty in reconfiguring
*   ENET_LINE for usage with or without MAPOTN as part of the datapath.  Systems
*   will be able to know which active connections may be disrupted by performing
*   a configuration change to ENET_LINE.\n\n
*
*   If the MAPOTN application bandwidth exceeds the capacity of the secondary
*   group, the secondary group must be specified as
*   DIGI_MAPPER_SECONDARY_GROUP_DONT_CARE \n\n
*
*   The DONT_CARE option allows for cases where the channel rate to be scheduled
*   exceeds the bandwidth already allocated against the secondary group 10G
*   container size.\n\n
*
*   The DONT_CARE option can unconditionally be used in systems that:\n
*   - Do not use the ENET_LINE subsystem. \n
*   - Do not use modes that use the ENET_LINE and MAPOTN in conjunction. \n
*
*
* ELEMENTS:
*   DIGI_MAPPER_SECONDARY_GROUP_0           - Allocates MAPOTN bandwidth to the
*                                             first 10G secondary group.
*   DIGI_MAPPER_SECONDARY_GROUP_1           - Allocates MAPOTN bandwidth to the
*                                             second 10G secondary group.
*   DIGI_MAPPER_SECONDARY_GROUP_2           - Allocates MAPOTN bandwidth to the
*                                             third 10G secondary group.
*   DIGI_MAPPER_SECONDARY_GROUP_3           - Allocates MAPOTN bandwidth to the
*                                             fourth 10G secondary group.
*   DIGI_MAPPER_SECONDARY_GROUP_DONT_CARE   - Allocate MAPOTN bandwidth to one or
*                                             more secondary groups within the
*                                             primary group specified by
*                                             digi_mapper_primary_group_t
*
*******************************************************************************/
typedef enum {
    DIGI_MAPPER_SECONDARY_GROUP_0 = 0,
    DIGI_MAPPER_SECONDARY_GROUP_1,
    DIGI_MAPPER_SECONDARY_GROUP_2,
    DIGI_MAPPER_SECONDARY_GROUP_3,
    DIGI_MAPPER_SECONDARY_GROUP_DONT_CARE,

    LAST_DIGI_MAPPER_SECONDARY_GROUP      /* this is for out of range values error handling */
} digi_mapper_secondary_group_t;


/*******************************************************************************
* ENUM: digi_mapper_alloc_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the concurrent modes that may
*   be supported by a primary and secondary group of the MAPPER/ENET_LINE
*   processing resources.
*
*   When operating in DIGI_MAPPER_ALLOC_ENET_LINE and
*   DIGI_MAPPER_ALLOC_MAPPER_WITHOUT_ENET_LINE there it is possible to
*   configure an additional datapath within the resource to create an
*   allocation of DIGI_MAPPER_ALLOC_MAPPER_AND_ENET_LINE.
*
*
* ELEMENTS:
*   DIGI_MAPPER_ALLOC_UNUSED     - No resources allocated to data paths
*   DIGI_MAPPER_ALLOC_ENET_LINE  - Resources allocated to path connecting
*                                  LINE SERDES, ENET_LINE and CPB
*   DIGI_MAPPER_ALLOC_MAPPER_WITHOUT_ENET_LINE - Resources allocated to path
*                                  connecting ODUKSW, MAPPER and CPB excluding
*                                  ENET_LINE resources
*   DIGI_MAPPER_ALLOC_MAPPER_WITH_ENET_LINE - Resources allocated to path
*                                  connecting ODUKSW, MAPPER, ENET_LINE and CPB
*   DIGI_MAPPER_ALLOC_MAPPER_AND_ENET_LINE - Resources allocated to two unique
*                                  paths.  Connection between LINE_SERDES,
*                                  ENET_LINE and CPB and connection between
*                                  ODUKSW, MAPPER and CPB
*
*
*
*******************************************************************************/
typedef enum {
    DIGI_MAPPER_ALLOC_UNUSED = 0,
    DIGI_MAPPER_ALLOC_ENET_LINE,
    DIGI_MAPPER_ALLOC_MAPPER_WITHOUT_ENET_LINE,
    DIGI_MAPPER_ALLOC_MAPPER_WITH_ENET_LINE,
    DIGI_MAPPER_ALLOC_MAPPER_AND_ENET_LINE,

    LAST_DIGI_MAPPER_ALLOC      /* this is for out of range values error handling */
} digi_mapper_alloc_t;

/*******************************************************************************
* ENUM: digi_map_oduk_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the ODUk frame type.  This is
*   used to provide additional context to modes defined in digi_map_mode_t.
*
*   The element terms have a syntax:
*      DIGI_MAP_ODUK_TYPE_[oduk type]
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    DIGI_MAP_ODUK_TYPE_ODU0 = 0,
    DIGI_MAP_ODUK_TYPE_ODU1,
    DIGI_MAP_ODUK_TYPE_ODU1E,
    DIGI_MAP_ODUK_TYPE_ODU1F,
    DIGI_MAP_ODUK_TYPE_ODU2,
    DIGI_MAP_ODUK_TYPE_ODU2E,
    DIGI_MAP_ODUK_TYPE_ODU2F,
    DIGI_MAP_ODUK_TYPE_ODU3,
    DIGI_MAP_ODUK_TYPE_ODU3E1,
    DIGI_MAP_ODUK_TYPE_ODU3E2,
    DIGI_MAP_ODUK_TYPE_ODU4,
    DIGI_MAP_ODUK_TYPE_ODUFLEX,

    LAST_DIGI_MAP_ODUK_TYPE     /* this is for out of range values error handling */
} digi_map_oduk_type_t;

/*******************************************************************************
* ENUM: digi_prod_app_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumeration of the major product application architectures
*   supported by the DIGI device.
*
* ELEMENTS:
*   DIGI_PROD_APP_LINE_CARD - DIGI without system serdes interface ports
*   DIGI_PROD_APP_TRANSMUXPONDER_CARD - DIGI with system serdes interface ports
*   DIGI_PROD_APP_PORTLESS_CARD - DIGI without line and system serdes ports
*   DIGI_PROD_APP_SYSOTN_CARD   - DIGI using SYSOTN subsystem
*   DIGI_PROD_APP_100G_3_X_40G_CFP_CARD - DIGI using SIFD to LIFD Mux path
*   DIGI60_PROD_APP_LINE_CARD   - DIGI 60G without system serdes interface ports
*   DIGI60_PROD_APP_TRANSMUXPONDER_CARD - DIGI using 120G of LINE serdes independently
*                                         assignable to server or client usage
*   DIGI60_PROD_APP_TRANSMUXPONDER_SYS_CLIENT_CARD - DIGI using 60G of server 
*                                                    LINE serdes ports with 60G
*                                                    of client SYSTEM serdes ports
*
*
*******************************************************************************/
typedef enum
{
    DIGI_PROD_APP_LINE_CARD = 0,
    DIGI_PROD_APP_TRANSMUXPONDER_CARD,
    DIGI_PROD_APP_PORTLESS_CARD,
    DIGI_PROD_APP_SYSOTN_CARD,
    DIGI_PROD_APP_100G_3_X_40G_CFP_CARD,
    DIGI60_PROD_APP_LINE_CARD, 
    DIGI60_PROD_APP_TRANSMUXPONDER_CARD,
    DIGI60_PROD_APP_TRANSMUXPONDER_SYS_CLIENT_CARD,

    LAST_DIGI_PROD_APP       /* this is for out of range values error handling                                   */

} digi_prod_app_t;


/*******************************************************************************
* ENUM: digi_sifd_xfer_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the Interlaken transfer format
*
* ELEMENTS:
*    DIGI_SIFD_XFER_MODE_SEGMENT - Channel interleaved Segment transfers
*    DIGI_SIFD_XFER_MODE_PACKET  - Channel continuous Segment transfers forming
*                                  complete packets
*
*******************************************************************************/
typedef enum
{
    DIGI_SIFD_XFER_MODE_SEGMENT = 0,
    DIGI_SIFD_XFER_MODE_PACKET,
    LAST_DIGI_SIFD_XFER_MODE /* this is for out of range values error handling */
} digi_sifd_xfer_mode_t;

/*******************************************************************************
* ENUM: digi_sifd_burst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the Interlaken burst size
*
* ELEMENTS:
*    DIGI_SIFD_BURST_128 - 128 byte burst size
*    DIGI_SIFD_BURST_192 - 192 byte burst size
*    DIGI_SIFD_BURST_256 - 256 byte burst size
*
*******************************************************************************/
typedef enum
{
    DIGI_SIFD_BURST_128 = 0,
    DIGI_SIFD_BURST_192,
    DIGI_SIFD_BURST_256,
    LAST_DIGI_SIFD_BURST /* this is for out of range values error handling */
} digi_sifd_burst_t;

/*******************************************************************************
* ENUM: digi_sifd_burst_short_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the Interlaken short burst
*    size
*
* ELEMENTS:
*    DIGI_SIFD_BURST_SHORT_32 - 32 byte burst size
*    DIGI_SIFD_BURST_SHORT_64 - 64 byte burst size
*
*******************************************************************************/
typedef enum
{
    DIGI_SIFD_BURST_SHORT_32 = 0,
    DIGI_SIFD_BURST_SHORT_64,
    LAST_DIGI_SIFD_BURST_SHORT /* this is for out of range values error handling */
} digi_sifd_burst_short_t;

/*******************************************************************************
* ENUM: digi_sifd_burst_min_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the Interlaken minimum burst
*    size. If enabled, RX bursts will trigger interrupt if less than burst
*    min.
*
* ELEMENTS:
*    DIGI_SIFD_BURST_MIN_DISABLE - No minimum burst size expected
*    DIGI_SIFD_BURST_MIN_32      - 32 byte burst size
*    DIGI_SIFD_BURST_MIN_64      - 64 byte burst size
*
*******************************************************************************/
typedef enum
{
    DIGI_SIFD_BURST_MIN_DISABLE = 0,
    DIGI_SIFD_BURST_MIN_32,
    DIGI_SIFD_BURST_MIN_64,
    LAST_DIGI_SIFD_BURST_MIN /* this is for out of range values error handling */
} digi_sifd_burst_min_t;

/*******************************************************************************
* ENUM: digi_sifd_ilkn_mf_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the Interlaken Meta Frame
*    Length
*
* ELEMENTS:
*    DIGI_SIFD_ILKN_MF_1024 - meta frame length 1024 byte
*    DIGI_SIFD_ILKN_MF_2048 - meta frame length 2048 byte
*    DIGI_SIFD_ILKN_MF_4096 - meta frame length 4096 byte
*
*******************************************************************************/
typedef enum
{
    DIGI_SIFD_ILKN_MF_1024 = 1024,
    DIGI_SIFD_ILKN_MF_2048 = 2048,
    DIGI_SIFD_ILKN_MF_4096 = 4096,
    LAST_DIGI_SIFD_ILKN_MF /* this is for out of range values error handling */
} digi_sifd_ilkn_mf_t;

/*******************************************************************************
* ENUM: digi_sifd_oduksw_ilkn_header_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the format of headers
*    transferred between Interlaken interface and COREOTN ODUKSW.  All headers
*    will contain the Packet Maker Header, in addition 'user specified overhead
*    "2 bytes"' and 'user specified overhead "4 bytes"' may be included.
*
* ELEMENTS:
*    DIGI_SIFD_ODUKSW_ILKN_HEADER_NONE          - Optional Header 2B/4B not present
*    DIGI_SIFD_ODUKSW_ILKN_HEADER_2B            - Optional Header 2B present
*    DIGI_SIFD_ODUKSW_ILKN_HEADER_4B            - Optional Header 4B present
*    DIGI_SIFD_ODUKSW_ILKN_HEADER_2B_AND_4B     - Both Header 2B and 4B present
*
*******************************************************************************/
typedef enum
{
    DIGI_SIFD_ODUKSW_ILKN_HEADER_NONE = 0,
    DIGI_SIFD_ODUKSW_ILKN_HEADER_2B,
    DIGI_SIFD_ODUKSW_ILKN_HEADER_4B,
    DIGI_SIFD_ODUKSW_ILKN_HEADER_2B_AND_4B,
    LAST_DIGI_SIFD_ODUKSW_ILKN_HEADER /* this is for out of range values error handling */
} digi_sifd_oduksw_ilkn_header_t;

/*******************************************************************************
* ENUM: digi_ilkn_serdes_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the Interlaken interface
*    Serdes rate.
*
* ELEMENTS:
*    DIGI_ILKN_SERDES_RATE_3G125     - 3.125 Gbps
*    DIGI_ILKN_SERDES_RATE_6G25      - 6.25 Gbps
*    DIGI_ILKN_SERDES_RATE_10G3125   - 10.3125 Gbps
*    DIGI_ILKN_SERDES_RATE_11G5      - 11.5 Gbps
*    DIGI_ILKN_SERDES_RATE_12G5      - 12.5 Gpbs
*******************************************************************************/
typedef enum
{
    DIGI_ILKN_SERDES_RATE_3G125 = 0,
    DIGI_ILKN_SERDES_RATE_6G25,
    DIGI_ILKN_SERDES_RATE_10G3125,
    DIGI_ILKN_SERDES_RATE_11G5,
    DIGI_ILKN_SERDES_RATE_12G5,
    LAST_DIGI_ILKN_SERDES_RATE /* this is for out of range values error handling */

} digi_ilkn_serdes_rate_t;

/*******************************************************************************
* ENUM: digi_sifd_fc_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the flow control mode for
*    an Interlaken interface.
*
* ELEMENTS:
*    DIGI_SIFD_FC_MODE_NONE        - No Flow Control Implemented
*    DIGI_SIFD_FC_MODE_IN_BAND_16  - In-band Flow Control 16 indications per ICW/BCW
*    DIGI_SIFD_FC_MODE_IN_BAND_24  - In-band Flow Control 24 indications per ICW/BCW
*    DIGI_SIFD_FC_MODE_OUT_OF_BAND - Out-of-band Flow Control
*
*******************************************************************************/
typedef enum
{
    DIGI_SIFD_FC_MODE_NONE = 0,
    DIGI_SIFD_FC_MODE_IN_BAND_16,
    DIGI_SIFD_FC_MODE_IN_BAND_24,
    DIGI_SIFD_FC_MODE_OUT_OF_BAND,
    LAST_DIGI_SIFD_FC_MODE /* this is for out of range values error handling */
} digi_sifd_fc_mode_t;


/*******************************************************************************
* ENUM: digi_odu_struct_odu_level_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the level of ODU struct
*
* ELEMENTS:
*    DIGI_ODU_STRUCT_LEVEL_HO_ODU - ODU struct for HO ODU
*    DIGI_ODU_STRUCT_LEVEL_MO_ODU - ODU struct for MO ODU
*    DIGI_ODU_STRUCT_LEVEL_LO_ODU - ODU struct for LO ODU
*
*******************************************************************************/
typedef enum
{
    DIGI_ODU_STRUCT_LEVEL_HO_ODU  = 0,
    DIGI_ODU_STRUCT_LEVEL_MO_ODU  = 1,
    DIGI_ODU_STRUCT_LEVEL_LO_ODU  = 2,

    LAST_DIGI_ODU_STRUCT_LEVEL                    /* this is for out of range values error handling */
} digi_odu_struct_odu_level_t;

/*******************************************************************************
* ENUM: digi_otn_mux_stages_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type to specify number of stages involved in
*    multiplexing the payload data.
*
* ELEMENTS:
*   DIGI_OTN_MUX_STAGES_NONE    - No muxing; unchannelized payload
*   DIGI_OTN_MUX_STAGES_ONE     - One stage muxing
*   DIGI_OTN_MUX_STAGES_TWO     - Two stage muxing
*
*
*******************************************************************************/
typedef enum
{
    DIGI_OTN_MUX_STAGES_NONE = 0,
    DIGI_OTN_MUX_STAGES_ONE,
    DIGI_OTN_MUX_STAGES_TWO,

    LAST_DIGI_OTN_MUX_STAGES        /* this is for out of range values error handling  */
} digi_otn_mux_stages_t;

/*******************************************************************************
* ENUM: digi_odu_line_payload_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to store HO Tributary slot types.
*
* ELEMENTS:
*   DIGI_ODU_UNCHANNELIZED     - unchannelized ODU channel
*   DIGI_ODU_TS_1G25           - 1.25G tribtuary slot payload
*   DIGI_ODU_TS_2G5            - 2.5G tributary slot payload
*   DIGI_ODU_NULL              - To configure HO or MO channel with NULL payload.
*                                This NULL payload channel gets terminated at HO
*                                or MO.
*
*******************************************************************************/
typedef enum
{
    DIGI_ODU_UNCHANNELIZED  = 0,
    DIGI_ODU_TS_1G25        = 1,
    DIGI_ODU_TS_2G5         = 2,
    DIGI_ODU_NULL           = 3,

    LAST_DIGI_ODU_PAYLOAD_FORMAT
} digi_odu_line_payload_t;

/*******************************************************************************
* ENUM: digi_otn_mapping_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to store mapping mode supported in COREOTN
*   or MAPOTN subsystem.
*
* ELEMENTS:
*   DIGI_OTN_MAP_AMP        - Asynchronous mapping procedure
*   DIGI_OTN_MAP_GMP        - Generic mapping procedure
*   DIGI_OTN_MAP_RESERVED   - Reserved mapping mode for testing
*
*******************************************************************************/
typedef enum {
    DIGI_OTN_MAP_AMP        = 0,
    DIGI_OTN_MAP_GMP        = 1,
    DIGI_OTN_MAP_RESERVED   = 2,

    LAST_DIGI_OTN_MAP,
} digi_otn_mapping_type_t;

/*******************************************************************************
* ENUM: digi_otn_cfg_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN function configuration operation mode.
*
* ELEMENTS:
*   DIGI_OTN_DIRECT_DEVICE_CONFIG       - ODU configuration data is directly
*                                         applied to the device.
*   DIGI_OTN_PENDING_CONFIG             - ODU configuration data is stored in
*                                         ODU structs for pending application.
*******************************************************************************/
typedef enum
{
    DIGI_OTN_DIRECT_DEVICE_CONFIG = 0,
    DIGI_OTN_PENDING_CONFIG       = 1,
} digi_otn_cfg_mode_t;

/*******************************************************************************
* ENUM: digi_otn_odu_chnl_prov_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the provisioning mode of
*    LO ODU channels in the OTN server
*
* ELEMENTS:
*   DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT - default setting. This will provision
*                                         one LO ODU channel into a HO at a time
*   DIGI_OTN_ODU_CHNL_PROV_ALL          - This will provision all LO ODU channels
*                                         into a HO at once. This mode will
*                                         reduce provisioning configuration time.
*   DIGI_OTN_ODU_CHNL_PROV_ODUKSW_ONLY_BULK
*                                       - Should be only used in 1+1 SNCP case,
*                                         where switching happens inside oduksw.
*
*******************************************************************************/
typedef enum
{
    DIGI_OTN_ODU_CHNL_PROV_IN_INCREMENT = 0,
    DIGI_OTN_ODU_CHNL_PROV_ALL,
    DIGI_OTN_ODU_CHNL_PROV_ODUKSW_ONLY_BULK,

    LAST_DIGI_OTN_CHNL_PROV_MODE,
} digi_otn_odu_chnl_prov_mode_t;

/*******************************************************************************
* ENUM: digi_odu_frm_pm_tcm_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for monitored connection layer type that is pool id based.
*
* ELEMENTS:
*   DIGI_ODU_FRM_MESSAGE_TYPE_PM           - PM message
*   DIGI_ODU_FRM_MESSAGE_TYPE_TCM1         - TCM1 message
*   DIGI_ODU_FRM_MESSAGE_TYPE_TCM2         - TCM2 message
*   DIGI_ODU_FRM_MESSAGE_TYPE_TCM3         - TCM3 message
*   DIGI_ODU_FRM_MESSAGE_TYPE_TCM4         - TCM4 message
*   DIGI_ODU_FRM_MESSAGE_TYPE_TCM5         - TCM5 message
*   DIGI_ODU_FRM_MESSAGE_TYPE_TCM6         - TCM6 message
*   DIGI_ODU_FRM_MESSAGE_TYPE_FLOATING_TCM - FLOATING TCM message
*   DIGI_ODU_FRM_MESSAGE_TYPE_PM_NIM       - PM NIM message
*   DIGI_ODU_FRM_MESSAGE_TYPE_DCI          - PARENT is DCI
*******************************************************************************/
typedef enum
{
    DIGI_ODU_FRM_MESSAGE_TYPE_PM              = 0,    /* PM Message */
    DIGI_ODU_FRM_MESSAGE_TYPE_TCM1            = 1,    /* TCM1 Message */
    DIGI_ODU_FRM_MESSAGE_TYPE_TCM2            = 2,    /* TCM2 Message */
    DIGI_ODU_FRM_MESSAGE_TYPE_TCM3            = 3,    /* TCM3 Message */
    DIGI_ODU_FRM_MESSAGE_TYPE_TCM4            = 4,    /* TCM4 Message */
    DIGI_ODU_FRM_MESSAGE_TYPE_TCM5            = 5,    /* TCM5 Message */
    DIGI_ODU_FRM_MESSAGE_TYPE_TCM6            = 6,    /* TCM6 Message */
    DIGI_ODU_FRM_MESSAGE_TYPE_FLOATING_TCM    = 7,    /* Floating TCM Message */
    DIGI_ODU_FRM_MESSAGE_TYPE_PM_NIM          = 8,    /* PM NIM Message */
    DIGI_ODU_FRM_MESSAGE_TYPE_DCI             = 9,    /* No parent - Parent is DCI */

    LAST_DIGI_ODU_FRM_MESSAGE_TYPES                = 11,   /* this is for out of range values error handling */
} digi_odu_frm_pm_tcm_id_t;

/*******************************************************************************
* ENUM: digi_post_mux_sfi51_bus_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type defining the post mux bus.
*
* ELEMENTS:
*   DIGI_POST_MUX_BUS_0                    - The first post mux bus.
*   DIGI_POST_MUX_BUS_1                    - The second post mux bus.
*   DIGI_POST_MUX_BUS_UNUSED               - The bus is not in use.
*******************************************************************************/
typedef enum
{
    DIGI_POST_MUX_BUS_0                       = 0,
    DIGI_POST_MUX_BUS_1                       = 1,
    DIGI_POST_MUX_BUS_UNUSED                  = 0xFF,

    LAST_DIGI_POST_MUX_BUS, /* this is for out of range values error handling */
} digi_post_mux_sfi51_bus_t;


/******************************************************************************
* ENUM: digi_deskew_type_t
*______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated deskew configuration type.
*   Some configurations can have either a HW resources routing via SFIS or OTL
*   HW modules. In the case of SFIS routing user need to configure the type of
*   dsi lane deskew required.
*   for OTL3.4/OTU2 configurations routing no deskew is required.
*
* ELEMENTS:
*   DIGI_OTN_SERVER_NO_DESKEW            - no deskew required, OTL3.4/OTU2 configurations
*   DIGI_OTN_SERVER_OIF_SFIS_DESKEW      - OIF SFI-S deskew
*   DIGI_OTN_SERVER_ENHANCED_SFIS_DESKEW - OTL over SFI deskew mode
*   LAST_OTN_SERVER_DIGI_DESKEW_TYPE     - this is for out of range values error handling
*******************************************************************************/
typedef enum
{
    DIGI_OTN_SERVER_NO_DESKEW = (LINE_OTN_NO_DESKEW),
    DIGI_OTN_SERVER_OIF_SFIS_DESKEW ,
    DIGI_OTN_SERVER_ENHANCED_SFIS_DESKEW ,
    LAST_DIGI_OTN_SERVER_DESKEW_TYPE   /* this is for out of range values error handling           */

}digi_sfis_deskew_type_t;

/*******************************************************************************
* ENUM: digi_serdes_sfi51_swap_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the SFI 5.1 serdes interface lane
*   ordering (logical pin to physical pin mapping).
*
*
* ELEMENTS:
*   DIGI_SERDES_SFI51_SWAP_OFF   - Lane order 1:1 with physical pin index
*   DIGI_SERDES_SFI51_SWAP_ON    - Lane order inverted from physical pin index
*
*******************************************************************************/
typedef enum {

    DIGI_SERDES_SFI51_SWAP_OFF = 0,
    DIGI_SERDES_SFI51_SWAP_ON,

    LAST_DIGI_SERDES_SFI51_SWAP      /* this is for out of range values error handling */

} digi_serdes_sfi51_swap_t;


/*******************************************************************************
* ENUM: digi_fw_info_request_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for requesting information regarding the Firmware state.
*
* ELEMENTS:
*   DIGI_FW_INFO_REQ_FW_STATUS   - Request the status of the firmware.
*   DIGI_FW_INFO_REQ_FW_VERSION  - Request the version of firmware be checked.
*   LAST_DIGI_FW_INFO_REQ        - This is for out of range values error handling
*
*******************************************************************************/
typedef enum {

    DIGI_FW_INFO_REQ_FW_STATUS = 1,
    DIGI_FW_INFO_REQ_FW_VERSION = 2,

    LAST_DIGI_FW_INFO_REQ   /* this is for out of range values error handling */

} digi_fw_info_request_t;


/*******************************************************************************
* ENUM: digi_int_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration for defining aggregation interrupt tables.
*   Aggregation interrupt tables provide interrupt and status information at
*   the device and subsystem level. For each subsystem, all the channels
*   are available in an array.
*
* ELEMENTS:
*    DIGI_INT_TYPE_DEVICE               - Aggregation int table defined in digi_device_int_t
*    DIGI_INT_TYPE_ENET_LINE            - Aggregation int table defined in digi_enet_int_t
*    DIGI_INT_TYPE_ENET_SYS             - Aggregation int table defined in digi_enet_int_t
*    DIGI_INT_TYPE_CBR                  - Aggregation int table defined in digi_cbr_int_t
*    DIGI_INT_TYPE_OTN_MAPPER           - Aggregation int table defined in digi_otn_mapper_int_t
*    DIGI_INT_TYPE_OTN                  - Aggregation int table defined in digi_otn_int_t
*    DIGI_INT_TYPE_CPB                  - Aggregation int table defined in digi_cpb_int_t
*    DIGI_INT_TYPE_PMON                 - Aggregation int table defined in digi_pmon_int_t
*
*    DIGI_INT_CHNL_TYPE_ENET_LINE       - Channel int table defined in digi_enet_int_chnl_t
*    DIGI_INT_CHNL_TYPE_ENET_SYS        - Channel int table defined in digi_enet_int_chnl_t
*    DIGI_INT_CHNL_TYPE_CBR             - Channel int table defined in digi_cbr_int_chnl_t
*    DIGI_INT_CHNL_TYPE_OTN_MAPPER      - Channel int table defined in digi_mapper_int_chnl_t
*    DIGI_INT_CHNL_TYPE_OTN_MAPPER_FO2  - Channel FO2 int table defined in digi_mapper_int_chnl_t
*    DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET_ETRANS - Channel ENET/ETRANS int table defined in digi_mapper_int_chnl_t
*    DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET_PMON   - Channel ENET PMON int table defined in digi_mapper_int_chnl_t
*    DIGI_INT_CHNL_TYPE_OTN_SERVER      - Channel int table defined in lineotn_int_chnl_t
*    DIGI_INT_CHNL_TYPE_OTN_SERVER_SYS  - Channel int table defined in lineotn_int_chnl_t
*    DIGI_INT_CHNL_TYPE_OTN_ODU         - Channel int table defined in digi_otn_odu_int_chnl_t
*    DIGI_INT_CHNL_TYPE_CPB             - Channel int table defined in cpb_int_chnl_t
*    DIGI_INT_LINK_TYPE_SIFD            - Link int table defined in sifd_int_link_t
*    DIGI_INT_CHNL_TYPE_SIFD_ENET_PMON  - Channel ENET PMON int table defined in digi_sifd_enet_pmon_int_chnl_t
*
*    LAST_DIGI_INT_TYPE                 - Out of range handling.
*******************************************************************************/
typedef enum {
    DIGI_INT_TYPE_DEVICE = 0,
    DIGI_INT_TYPE_ENET_LINE,
    DIGI_INT_TYPE_ENET_SYS,
    DIGI_INT_TYPE_CBR,
    DIGI_INT_TYPE_OTN_MAPPER,
    DIGI_INT_TYPE_OTN,
    DIGI_INT_TYPE_CPB,
    DIGI_INT_TYPE_SIFD,
    DIGI_INT_TYPE_PMON,

    DIGI_INT_CHNL_TYPE_START,
    DIGI_INT_CHNL_TYPE_ENET_LINE,
    DIGI_INT_CHNL_TYPE_ENET_SYS,
    DIGI_INT_CHNL_TYPE_CBR,
    DIGI_INT_CHNL_TYPE_OTN_MAPPER,
    DIGI_INT_CHNL_TYPE_OTN_MAPPER_FO2,
    DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET_ETRANS,
    DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET_PMON,
    DIGI_INT_CHNL_TYPE_OTN_SERVER,
    DIGI_INT_CHNL_TYPE_OTN_SERVER_SYS,
    DIGI_INT_CHNL_TYPE_OTN_ODU,
    DIGI_INT_CHNL_TYPE_CPB,
    DIGI_INT_LINK_TYPE_SIFD,
    DIGI_INT_CHNL_TYPE_SIFD_ENET_PMON,

    LAST_DIGI_INT_TYPE,
} digi_int_type_t;

/*******************************************************************************
* ENUM: digi_ilkn_chnl_target_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying the destination target of an ILKN channel
*
* ELEMENTS:
*   DIGI_ILKN_CHNL_TARGET_CPB    - Target is the CPB (PIF Interface)
*   DIGI_ILKN_CHNL_TARGET_ODUK   - Target is the ODUk Switch (CPRI Interface)
*   LAST_DIGI_ILKN_CHNL_TARGET   - This is for out of range values error handling
*
*******************************************************************************/
typedef enum {

    DIGI_ILKN_CHNL_TARGET_CPB = 0,
    DIGI_ILKN_CHNL_TARGET_ODUK = 1,
    LAST_DIGI_ILKN_CHNL_TARGET
} digi_ilkn_chnl_target_t;

/*******************************************************************************
* ENUM: digi_serdes_port_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*
*   Structure for enumerated type for defining serdes port major modes of
*   operation. \n\n
*
*   Terms take on the following scheme: \n
*   DIGI_SERDES_PORT_MODE_[serdes_type]_[mode] \n
*   Where:\n
*   [serdes_type]: specifies the serdes instatiation type\n
*   [mode]:    specifies the Serdes type and [mode] specifies the desired rate
*   of operation or the unused lowest power consumption state of a serdes port.\n\n
*
*
* ELEMENTS:
*   DIGI_SERDES_PORT_MODE_ANY_LOWPWR       - Any Serdes port type in unused lowest
*                                            power consumption state.
*   DIGI_SERDES_PORT_MODE_S16_100_GE       - System/Line 100GE
*   DIGI_SERDES_PORT_MODE_S16_40_GE        - System/Line 40GE
*   DIGI_SERDES_PORT_MODE_S16_10_GE        - System/Line 10GE
*   DIGI_SERDES_PORT_MODE_S16_DIC          - System/Line for ENET MAC datapath with Deficit Idle Count (DIC) compensation
*   DIGI_SERDES_PORT_MODE_S16_FC_1200      - System/Line FC1200
*   DIGI_SERDES_PORT_MODE_S16_FC_800       - System/Line FC800
*   DIGI_SERDES_PORT_MODE_S16_10G_GDPS     - System/Line 10G GDPS
*   DIGI_SERDES_PORT_MODE_S16_5G_GDPS      - System/Line 5G GDPS
*   DIGI_SERDES_PORT_MODE_S16_CPRI_10G     - System/Line CPRI 10G
*   DIGI_SERDES_PORT_MODE_S16_CPRI_6G      - System/Line CPRI 6G
*   DIGI_SERDES_PORT_MODE_S16_CPRI_5G      - System/Line CPRI 5G
*   DIGI_SERDES_PORT_MODE_S16_OC_768       - System/Line OC-768
*   DIGI_SERDES_PORT_MODE_S16_OC_192       - System/Line OC-192
*
*   DIGI_SERDES_PORT_MODE_S16_OTU4         - System/Line OTU4
*   DIGI_SERDES_PORT_MODE_S16_OTU3         - System/Line OTU3
*   DIGI_SERDES_PORT_MODE_S16_OTU3E2       - System/Line OTU3E2
*   DIGI_SERDES_PORT_MODE_S16_OTU3E1       - System/Line OTU3E1
*   DIGI_SERDES_PORT_MODE_S16_OTU2         - System/Line OTU2
*   DIGI_SERDES_PORT_MODE_S16_OTU2E        - System/Line OTU2E
*   DIGI_SERDES_PORT_MODE_S16_OTU1E        - System/Line OTU1E
*   DIGI_SERDES_PORT_MODE_S16_OTU2F        - System/Line OTU2F
*   DIGI_SERDES_PORT_MODE_S16_OTU1F        - System/Line OTU1F
*
*   DIGI_SERDES_PORT_MODE_D8_OC_768        - SFI-5.1 OC-768
*   DIGI_SERDES_PORT_MODE_D8_OTU3          - SFI-5.1 OTU3
*   DIGI_SERDES_PORT_MODE_D8_OTU3E2        - SFI-5.1 OTU3E2
*   DIGI_SERDES_PORT_MODE_D8_OTU3E1        - SFI-5.1 OTU3E1
*
*   DIGI_SERDES_PORT_MODE_D8_GE            -  Ethernet Management ports
*
*   DIGI_SERDES_PORT_MODE_T8_3G125         -  ILKN LR 3.125G
*   DIGI_SERDES_PORT_MODE_T8_6G25          -  ILKN LR 6.25G
*   DIGI_SERDES_PORT_MODE_C8_3G125         -  ILKN SR 3.125G
*   DIGI_SERDES_PORT_MODE_C8_6G25          -  ILKN SR 6.25G
*   DIGI_SERDES_PORT_MODE_S16_3G125        -  ILKN System 3.125G
*   DIGI_SERDES_PORT_MODE_S16_6G25         -  ILKN System 6.25G
*   DIGI_SERDES_PORT_MODE_S16_10G3125      -  ILKN System 10.3125G
*   DIGI_SERDES_PORT_MODE_S16_11G5         -  ILKN System 11.5G
*   DIGI_SERDES_PORT_MODE_S16_12G5         -  ILKN System 12.5G
*
*******************************************************************************/

typedef enum {

    DIGI_SERDES_PORT_MODE_ANY_LOWPWR = 0,
    DIGI_SERDES_PORT_MODE_S16_100_GE,
    DIGI_SERDES_PORT_MODE_S16_40_GE,
    DIGI_SERDES_PORT_MODE_S16_10_GE,
    DIGI_SERDES_PORT_MODE_S16_DIC,
    DIGI_SERDES_PORT_MODE_S16_FC_1200,
    DIGI_SERDES_PORT_MODE_S16_FC_800,
    DIGI_SERDES_PORT_MODE_S16_10G_GDPS,
    DIGI_SERDES_PORT_MODE_S16_5G_GDPS,
    DIGI_SERDES_PORT_MODE_S16_CPRI_10G,
    DIGI_SERDES_PORT_MODE_S16_CPRI_6G,
    DIGI_SERDES_PORT_MODE_S16_CPRI_5G,
    DIGI_SERDES_PORT_MODE_S16_OC_768,
    DIGI_SERDES_PORT_MODE_S16_OC_192,

    DIGI_SERDES_PORT_MODE_S16_OTU4,
    DIGI_SERDES_PORT_MODE_S16_OTU3,
    DIGI_SERDES_PORT_MODE_S16_OTU3E2,
    DIGI_SERDES_PORT_MODE_S16_OTU3E1,
    DIGI_SERDES_PORT_MODE_S16_OTU2,
    DIGI_SERDES_PORT_MODE_S16_OTU2E,
    DIGI_SERDES_PORT_MODE_S16_OTU1E,
    DIGI_SERDES_PORT_MODE_S16_OTU2F,
    DIGI_SERDES_PORT_MODE_S16_OTU1F,

    DIGI_SERDES_PORT_MODE_D8_OC_768,
    DIGI_SERDES_PORT_MODE_D8_OTU3,
    DIGI_SERDES_PORT_MODE_D8_OTU3E2,
    DIGI_SERDES_PORT_MODE_D8_OTU3E1,
    DIGI_SERDES_PORT_MODE_D8_GE,
    DIGI_SERDES_PORT_MODE_T8_3G125,
    DIGI_SERDES_PORT_MODE_T8_6G25,
    DIGI_SERDES_PORT_MODE_C8_3G125,
    DIGI_SERDES_PORT_MODE_C8_6G25,
    DIGI_SERDES_PORT_MODE_S16_3G125,
    DIGI_SERDES_PORT_MODE_S16_6G25,
    DIGI_SERDES_PORT_MODE_S16_10G3125,
    DIGI_SERDES_PORT_MODE_S16_11G5,
    DIGI_SERDES_PORT_MODE_S16_12G5,

    LAST_DIGI_SERDES_PORT_MODE      /* this is for out of range values error handling */

} digi_serdes_port_mode_t;


/*******************************************************************************
* ENUM: digi_all_serdes_intf_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Structure for enumerated type for defining all serdes interfaces of
*   device. \n\n
*
*   Terms take on the following scheme: \n
*   DIGI_ALL_SERDES_INTF_[serdes_type]_[intf_name] \n
*   Where:\n
*   [serdes_type]: specifies the serdes instatiation type\n
*   [intf_name]:   specifies the interface name as per device HW specification
*
*
*
*
* ELEMENTS:
*    DIGI_ALL_SERDES_INTF_D8_SFI_51     - SFI-5.1 Interface (D8)
*    DIGI_ALL_SERDES_INTF_S16_LINE      - Line Interface (S16)
*    DIGI_ALL_SERDES_INTF_S16_SYSTEM    - System Interface (S16)
*    DIGI_ALL_SERDES_INTF_D8_RCP        - RCP Interface (D8)
*    DIGI_ALL_SERDES_INTF_D8_OTN_OH     - OTN OH Interface (D8)
*    DIGI_ALL_SERDES_INTF_D8_EMGMT  - ENET MGMT Interface (D8)
*    DIGI_ALL_SERDES_INTF_T8_BP_LR      - Backplane LR Interface (T8)
*    DIGI_ALL_SERDES_INTF_C8_BP_SR      - Backplane SR Interface (C8)
*    DIGI_ALL_SERDES_INTF_C8_PCIE       - PCIE Interface (C8)
*
*
*******************************************************************************/
typedef enum {
    DIGI_ALL_SERDES_INTF_D8_SFI_51 = 0,
    DIGI_ALL_SERDES_INTF_S16_LINE,
    DIGI_ALL_SERDES_INTF_S16_SYSTEM,
    DIGI_ALL_SERDES_INTF_D8_RCP,
    DIGI_ALL_SERDES_INTF_D8_OTN_OH,
    DIGI_ALL_SERDES_INTF_D8_EMGMT,
    DIGI_ALL_SERDES_INTF_T8_BP_LR,
    DIGI_ALL_SERDES_INTF_C8_BP_SR,
    DIGI_ALL_SERDES_INTF_C8_PCIE,

    LAST_DIGI_ALL_SERDES_INTF      /* this is for out of range values error handling */

} digi_all_serdes_intf_t;

/*******************************************************************************
* ENUM: digi_ilkn_intf_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Structure for enumerated type for defining ILKN interfaces of
*   device. \n\n
*
* ELEMENTS:
*    DIGI_ILKN1                         - Interlaken Interface 1
*    DIGI_ILKN2                         - Interlaken Interface 2
*
*******************************************************************************/


typedef enum {
    DIGI_ILKN1 = 0,
    DIGI_ILKN2,
    LAST_DIGI_ILKN_INTF      /* this is for out of range values error handling */

} digi_ilkn_intf_t;

/*******************************************************************************
* ENUM: hostmsg_adapt_intf_type_t
* ______________________________________________________________________________
*
* DESCRIPTION: SERDES Interfaces for Adaptation
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    HOSTMSG_ADAPT_INTF_S16_LINE = 0,
    HOSTMSG_ADAPT_INTF_S16_SYS  = 1,
    HOSTMSG_ADAPT_INTF_C8       = 2,
    HOSTMSG_ADAPT_INTF_T8       = 3,
    HOSTMSG_ADAPT_INTF_MAX_NUM
} hostmsg_adapt_intf_type_t;

/*******************************************************************************
* ENUM: hostmsg_s16_intf_type_t
* ______________________________________________________________________________
*
* DESCRIPTION: S16 SERDES Interfaces
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum
{
    HOSTMSG_S16_SERDES_INTF_LINE = 0,
    HOSTMSG_S16_SERDES_INTF_SYS  = 1,
    HOSTMSG_S16_SERDES_INTF_MAX_NUM
} hostmsg_s16_intf_type_t;

/*******************************************************************************
* ENUM: hostmsg_otn_server_src_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for source of the OTN server for configuring host message
*   CMD_CFG_CoreOTN_Config_dLOOMFI_Monitor. For more information please look
*   at the DIGI Firmware User Manual.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    HOSTMSG_OTN_SERVER_SRC_LINE = 0,
    HOSTMSG_OTN_SERVER_SRC_SYS,
    HOSTMSG_OTN_SERVER_SRC_MAX_NUM,
} hostmsg_otn_server_src_t;



/*******************************************************************************
* ENUM: digi_reset_reason_t
* ______________________________________________________________________________
*
* DESCRIPTION: This enumeration lists the different types of firmware restarts.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    NOT_DETERMINED = 0,
    COLD_RESTART   = 1,
    WARM_RESTART   = 2,
    CRASH_RESTART  = 3,
} digi_reset_reason_t;

/*******************************************************************************
* ENUM: digi_pgmrclk_div_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying the PGMR clock dividers identifier
*
* ELEMENTS:
*    See typedef.
*
*******************************************************************************/
typedef enum {
    DIGI_PGMRCLK_DIV_0 = 0,
    DIGI_PGMRCLK_DIV_1,
    DIGI_PGMRCLK_DIV_2,
    DIGI_PGMRCLK_DIV_3,
    LAST_DIGI_PGMRCLK_DIV
}digi_pgmrclk_div_id_t;

/*******************************************************************************
* ENUM: pgmrclk_input_source_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying the PGMR clock input sources
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    DIGI_PGMRCLK_SFI_1_TX = 0x0,
    DIGI_PGMRCLK_SFI_1_DSC_TX = 0x10,
    DIGI_PGMRCLK_SFI_1_RX = 0x11,
    DIGI_PGMRCLK_SFI_1_DSC_RX = 0x21,
    DIGI_PGMRCLK_SFI_2_TX = 0x22,
    DIGI_PGMRCLK_SFI_2_DSC_TX = 0x32,
    DIGI_PGMRCLK_SFI_2_RX = 0x33,
    DIGI_PGMRCLK_SFI_2_DSC_RX = 0x43,
    DIGI_PGMRCLK_LINE_IF_TX = 0x44,
    DIGI_PGMRCLK_LINE_IF_RX = 0x50,
    DIGI_PGMRCLK_SYS_IF_RX = 0x5C,
    DIGI_PGMRCLK_SYS_IF_TX = 0x6C,
    DIGI_PGMRCLK_MGMT_IF_TX = 0x7C,
    DIGI_PGMRCLK_MGMT_IF_RX = 0x7D,
    DIGI_PGMRCLK_PCIE_IF_TX = 0x7E,
    DIGI_PGMRCLK_PCIE_IF_RX = 0x82,
    DIGI_PGMRCLK_SR_IF_RX = 0x86,
    DIGI_PGMRCLK_SR_IF_TX = 0x8F,
    DIGI_PGMRCLK_LR_IF_RX = 0x96,
    DIGI_PGMRCLK_LR_IF_TX = 0xBE,
    DIGI_PGMRCLK_SYS1_DCSU_PCLK = 0xE6,
    DIGI_PGMRCLK_SYS1_DCSU_REF300_CLK = 0xE7,
    DIGI_PGMRCLK_SYS1_DCSU_REF600_CLK = 0xE8,
    DIGI_PGMRCLK_SYS2_DCSU_REF500_CLK = 0xE9,
    DIGI_PGMRCLK_ENET1_DCSU_REF653_CLK = 0xEA,
    DIGI_PGMRCLK_ENET2_DCSU_REF515_CLK = 0xEB,
    DIGI_PGMRCLK_OTN1_DCSU_REF349_CLK = 0xEC,
    DIGI_PGMRCLK_OTN1_DCSU_REF311_CLK = 0xED,
    DIGI_PGMRCLK_OTN1_DCSU_REF155_CLK = 0xEE,
    DIGI_PGMRCLK_OTN2_DCSU_REF355_CLK = 0xF0,
    DIGI_PGMRCLK_PTP_CLK = 0xF1,
    DIGI_PGMRCLK_RCP_IF_RX = 0xF2,
    DIGI_PGMRCLK_RCP_IF_TX = 0xF4,
    DIGI_PGMRCLK_OTN_OH_IF_RX = 0xF6,
    DIGI_PGMRCLK_OTN_OH_IF_TX = 0xF9,
    DIGI_PGMRCLK_DEMAP_ENET= 0xFC,
    DIGI_PGMRCLK_DEV_IF_REFCLK = 0X108,
    DIGI_PGMRCLK_ENET3_DCSU_REF518_CLK = 0x109,
    DIGI_PGMRCLK_ENET3_DCSU_REF388_CLK = 0x10A,
    DIGI_PGMRCLK_INTERNAL_CLK = 0x10C,
    DIGI_PGMRCLK_IRIG_IF_IRIG_CLKI = 0x11D,
    DIGI_PGMRCLK_IRIG_IF_IRIG_CLKO = 0x11E,
    DIGI_PGMRCLK_SPI_IF_SPI_SCLK = 0x11F,
    DIGI_PGMRCLK_SYS2_DCSU_DIV10_REF50_CLK = 0x120
} digi_pgmrclk_input_source_t ;


/*******************************************************************************
* ENUM: digi_s16_clk_divider_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying the clock divider of a S16 used as a clock
*
* ELEMENTS:
*   DIGI_S16_CLK_DIVIDER_16 - Divide S16 clock by 16
*   DIGI_S16_CLK_DIVIDER_64 - Divide S16 clock by 64
*
*******************************************************************************/
typedef enum digi_s16_clk_divider_t
{
    DIGI_S16_CLK_DIVIDER_16 = 0,
    DIGI_S16_CLK_DIVIDER_64 = 1,
} digi_s16_clk_divider_t;

/*******************************************************************************
* ENUM: digi_pgmrclk_fpi_cfg_action_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying the configuration action for FPI Generator.
*
* ELEMENTS:
*   DIGI_PGMRCLK_FPI_CFG_ACTION_DISABLE_RESYNC - Perform the one-time synchronization to the FPI pulse, then disable synchronization.
*   DIGI_PGMRCLK_FPI_CFG_ACTION_ENABLE_RESYNC  - Enable synchronization with the current settings.
*   DIGI_PGMRCLK_FPI_CFG_ACTION_RESIZE_MASK_WINDOW - Configure the mask window and enable synchronization.
*
*******************************************************************************/
typedef enum digi_pgmrclk_fpi_cfg_action_t
{
    DIGI_PGMRCLK_FPI_CFG_ACTION_DISABLE_RESYNC          = 0,
    DIGI_PGMRCLK_FPI_CFG_ACTION_ENABLE_RESYNC           = 1,
    DIGI_PGMRCLK_FPI_CFG_ACTION_RESIZE_MASK_WINDOW   = 2,
} digi_pgmrclk_fpi_cfg_action_t;

/*******************************************************************************
* ENUM: digi_ge_serdes_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying the GE serdes ports identifier.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum digi_ge_serdes_id_t
{
    DIGI_SERDES_INTF_OHP_0 = 0x0,
    DIGI_SERDES_INTF_OHP_1 = 0x1,
    DIGI_SERDES_INTF_OHP_2 = 0x2,
    DIGI_SERDES_INTF_EMGMT = 0x3,
    DIGI_SERDES_INTF_RI_RCP = 0x4,
    DIGI_SERDES_INTF_DI_RCP = 0x5,
    LAST_DIGI_GE_SERDES_ID
} digi_ge_serdes_id_t;


/*******************************************************************************
* ENUM: digi_loopback_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumeration of the different types of loopbacks. The first two loopbacks,
*    DIGI_SERDES_DIAGNOSTIC_LOOPBACK and DIGI_SERDES_FACILITY_LOOOPBACK, are
*    used by the SERDES loopback functions (digi_loopback_serdes*). The
*    remaining loopback types are used by the digi_loopback_internal_cfg
*    function.
*
* ELEMENTS:
*    DIGI_SERDES_DIAGNOSTIC_LOOPBACK                - Serdes transmitted data is looped back to the rx
*    DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK        - Serdes data is transmitted to the rx and tx interface
*    DIGI_SERDES_FACILITY_LOOPBACK                  - Serdes received data is looped back to the tx
*    DIGI_ENET_MAC_MLD_LOOPBACK                     - ENET (sys or line) MAC MLD to CPB
*    DIGI_ENET_MAPOTN_G709_LOOPBACK                 - ENET (line) G.709 Transcoder to MAPOTN (40GE)
*    DIGI_ENET_MAPOTN_GMP_LOOPBACK                  - ENET (line) GMP to MAPOTN (10/100GE)
*    DIGI_LINEOTN_MLD_DIAGNOSTIC_LOOPBACK           - LINEOTN loopback of ODU2/3/4 back to ODUk switch
*    DIGI_SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK      - SIFD COPI #1 or #2 to CPB at Interlaken
*    DIGI_SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK  - SIFD COPI #1 or #2 to COREOTN at Interlaken
*    DIGI_SIFD_COPI_FACILITY_LOOPBACK               - SIFD COPI (1 or 2) receive to transmit
*    DIGI_SYSOTN_MLD_DIAGNOSTIC_LOOPBACK            - SYSOTN MLD OTU Framer to COREOTN (ODU4)
*    DIGI_CBRC_DIAG_LOOPBACK                        - CBRC transmit to receive loopback
*    LAST_DIGI_LOOPBACK                             - this is for out of range checking
*******************************************************************************/
typedef enum digi_loopback_t
{
    DIGI_SERDES_DIAGNOSTIC_LOOPBACK = 0,
    DIGI_SERDES_DIAGNOSTIC_WITH_TX_LOOPBACK,
    DIGI_SERDES_FACILITY_LOOPBACK,
    DIGI_ENET_MAC_MLD_LOOPBACK,
    DIGI_ENET_MAPOTN_G709_LOOPBACK,
    DIGI_ENET_MAPOTN_GMP_LOOPBACK,
    DIGI_LINEOTN_MLD_DIAGNOSTIC_LOOPBACK,
    DIGI_SIFD_COPI_ILKN_CPB_INTERNAL_LOOPBACK,
    DIGI_SIFD_COPI_ILKN_COREOTN_INTERNAL_LOOPBACK,
    DIGI_SIFD_COPI_FACILITY_LOOPBACK,
    DIGI_SYSOTN_MLD_DIAGNOSTIC_LOOPBACK,
    DIGI_CBRC_DIAG_LOOPBACK,
    LAST_DIGI_LOOPBACK      /* this is for out of range values error handling */
} digi_loopback_t;


/*******************************************************************************
* ENUM: digi_enet_flow_control_buffer_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type to define how flow control is driven
*
* ELEMENTS:
*   DIGI_ENET_FLOW_CONTROL_BUFFER_LOCAL    - Flow control is driven by local\n
*                                            ressource  (DCPB)
*   DIGI_ENET_FLOW_CONTROL_BUFFER_EXTERNAL - Flow control is driven by external\n
*                                            ressource (via ILKN interface)
*
*******************************************************************************/
typedef enum digi_enet_flow_control_buffer_mode_t
{
    DIGI_ENET_FLOW_CONTROL_BUFFER_LOCAL= 0,
    DIGI_ENET_FLOW_CONTROL_BUFFER_EXTERNAL,
    LAST_DIGI_ENET_FLOW_CONTROL_BUFFER_MODE
} digi_enet_flow_control_buffer_mode_t;

/*******************************************************************************
* ENUM: digi_m1_pmm_lite_clk_ref_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for DIGI_M1 PMM LITE clock reference
*
* ELEMENTS:
*       DIGI_ENET_LINE_TIMING_PGMRCLK0  - PGMRCLK0 as the source
*       DIGI_ENET_LINE_TIMING_PGMRCLK1  - PGMRCLK1 as the source
*       DIGI_ENET_LINE_TIMING_PGMRCLK2  - PGMRCLK2 as the source
*       DIGI_ENET_LINE_TIMING_PGMRCLK3  - PGMRCLK3 as the source
*       DIGI_ENET_LINE_TIMING_IRIG_CLKI - IRIG_CLKI as the source
*       DIGI_ENET_LINE_TIMING_IRIG_DATI - IRIG_DATI as the source
*       DIGI_ENET_LINE_TIMING_PTP       - PTP clock as the source
*
*******************************************************************************/
typedef enum digi_m1_pmm_lite_clk_ref_t
{
   DIGI_M1_PMM_LITE_CLK_REF_PGMRCLK0 = 0,
   DIGI_M1_PMM_LITE_CLK_REF_PGMRCLK1,
   DIGI_M1_PMM_LITE_CLK_REF_PGMRCLK2,
   DIGI_M1_PMM_LITE_CLK_REF_PGMRCLK3,
   DIGI_M1_PMM_LITE_CLK_REF_IRIG_CLKI,
   DIGI_M1_PMM_LITE_CLK_REF_IRIG_DATI,
   DIGI_M1_PMM_LITE_CLK_REF_PTP,
   LAST_DIGI_M1_PMM_LITE_CLK_REF_CLK
} digi_m1_pmm_lite_clk_ref_t;

/*******************************************************************************
* ENUM: digi_cmf_sw_control_action_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This enumeration type will be used for various forcing action APIs.
*    The purpose is to allow SW control over the specific action so it does
*    not interfere with FWs consequential actions. If the consequential actions
*    are not being used, user does not need to 'release' control of actions.
*    If the consequential actions are being used and the user want to
*    force/force stop actions then by calling the force APIS, the FW
*    will be superceded. SW will have full control over that action until
*    SW chooses to release control.
*
* ELEMENTS:
*
*            DIGI_SW_STOP_FORCING_ACTION ~ Stop the force insertion of an action          ~ 0\n
*            DIGI_SW_START_FORCING_ACTION ~ Force insert action                           ~ 1\n
*            DIGI_SW_RELEASE_ACTION_CONTROL ~ Release SW control for the specified action ~ 2
*
*******************************************************************************/
typedef enum
{
    DIGI_SW_STOP_FORCING_ACTION = 0,
    DIGI_SW_START_FORCING_ACTION = 1,
    DIGI_SW_RELEASE_ACTION_CONTROL = 2

} digi_cmf_sw_control_action_t;

/*******************************************************************************
* ENUM: digi_client2trunk_actions_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the action for the client
*    to trunk direction.
*
* ELEMENTS:
*
*            Nothing                                ~ 0\n
*            Insert GFP-F CMF-CSF (loss client)     ~ 1\n
*            Insert GFP-F CMF-CSF (loss char)       ~ 2\n
*            Insert GFP-F CMF-DCI                   ~ 3\n
*            Insert GFP-F CMF-FDI                   ~ 4\n
*            Insert GFP-F CMF-RDI                   ~ 5\n
*            Insert OPU-CSF                         ~ 6\n
*            Insert CBRC ingress replacement (NOS)  ~ 7\n
*            CPRI(BMP), GDPS(BMP), FC800(BMP), FC1200(BMP)\n
*            Insert enet epmm ingress local fault (10G)   ~ 9\n
*            Insert epmm gfp ordered set                ~ 10\n
*            Insert enet epmm ingress local fault for legacy mode ~ 11\n
*        Insert enet epmm ingress local fault with high freq ordered sets ~ 12\n
*        Insert GFP-F CMF-CSF (loss client) with packet squelching for GSUP6.2 ~ 13\n
*        Insert GFP-F CMF-CSF (loss client) with packet squelching for GSUP7.3 ~ 14\n
*        Insert GFP-F CMF-FCI with packet squelching for GSUP6.2 ~ 15\n
*        Insert enet epmm ingress local fault (40G) ~ 16\n
*        Insert enet epmm ingress local fault (100G) ~ 17\n
*
*******************************************************************************/
typedef enum
{
    NO_C2T_ACTION = 0,
    INSERT_GFP_CMF_CSF_LOSS_CLIENT_SIGNAL = 1,
    INSERT_GFP_CMF_CSF_LOSS_CHAR_SYNC = 2,
    INSERT_GFP_CMF_DCI = 3,
    INSERT_GFP_CMF_FDI = 4,
    INSERT_GFP_CMF_RDI = 5,
    INSERT_OPU_CSF = 6,
    INSERT_CBRC_FORCE_FAULT = 7,
    /* Please be aware that INSERT_SONET_FORCE_FAULT was deprecated as INSERT_CBRC_FORCE_FAULT should be used instead */
    INSERT_ENET_ING_GSUP43_C73_LF = 9,
    INSERT_ENET_EPMM_GFP_ORDERED_SET = 10,
    INSERT_ENET_ING_GSUP43_C73_LF_LEGACY_MODE = 11,
    INSERT_ENET_ING_GSUP43_C73_LF_HIGH_FREQ_MODE = 12,
  INSERT_GFP_CMF_CSF_LOSS_CLIENT_SIGNAL_WITH_PACKET_SQUELCH = 13,
  INSERT_GFP_CMF_CSF_LOSS_CLIENT_SIGNAL_WITH_PACKET_SQUELCH_GSUP73 = 14,
  INSERT_GFP_CMF_FDI_WITH_PACKET_SQUELCH = 15,
  INSERT_ENET_EPMM_40G_LF = 16,
  INSERT_ENET_EPMM_100G_LF = 17,
} digi_client2trunk_actions_t;

/*******************************************************************************
* ENUM: digi_trunk2client_actions_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the action for the trunk
*    to client direction.
*
* ELEMENTS:
*           Do Nothing                                                          ~ 0  \n
*           Force GE LF (Gsup43 6.2, G7041 7.1) - into the client               ~ 1  \n
*           Force GE LF (Gsup43 7.3) - into the client                          ~ 2  \n
*           Force GE LF (Gsup43 7.1, Gsup43 7.2) - into the client              ~ 3  \n
*           Force GE 40G GMP LF  (G709-17.7.4) - into the client                ~ 4  \n
*           Force GE 100G GMP LF  (G709-17.4.5) - into the client               ~ 5  \n
*           Force GE RF (Gsup43 6.2, G7041 7.1) - into the client               ~ 6  \n
*           Force GE RF (GSup43 7.3) - into the client                        ~ 7  \n
*           Force GE RF (Gsup 43 7.1, Gsup 43 7.2) - into the client                    ~ 8  \n
*           Force 40G GMP RF (G709-17.7.4) - into the client                            ~ 9  \n
*           Force 100G GMP RF (G709-17.7.5)  - into the client                          ~ 10 \n
*           Force GE Egress Idle (GSup43 6.2, G7041 7.1) - into the client              ~ 11 \n
*           Insert GFP RDI - into the trunk                                         ~ 12 \n
*           Force GE Egress Idle (GSup43 7.3) - into the client                         ~ 13 \n
*           Squelch demap packets (to ETRANS) - into the client                         ~ 14 \n
*           Insert CBRC force fault  - into the client                                  ~ 16 \n
*           Squelch demap packets (to DCPB) - into the trunk                            ~ 17 \n
*           Insert SONET force fault pattern for sfis mode  - into the client           ~ 18 \n
*           FORCE GE LF NO FAULT AUTO CLEAR (Gsup43 6.2, G7041 7.1) - into the client   ~ 19 \n
*           FORCE GE LF on ENET SYS (Gsup43 6.2, G7041 7.1) - into the client           ~ 20 \n
*           Insert an OPU CSF - into the trunk                                ~ 21 \n
*           Insert GFP CMF CSF Loss of Signal - into the trunk                    ~ 22 \n
*           Force GE LF (Gsup43 6.2, G7041 7.1)
*           and suppress LOFD and PTI/UPI mm alarms - into the client                   ~ 23 \n
*           Force GE LF (Gsup43 7.3)
*           and suppress LOFD and PTI/UPI mm alarms - into the client                   ~ 24  \n
* 
*
*******************************************************************************/
typedef enum
{
    NO_T2C_ACTION = 0,
    FORCE_C62_LF = 1,
    FORCE_C73_LF = 2,
    FORCE_C71_C72_LF = 3,
    FORCE_40G_LF = 4,
    FORCE_100G_LF = 5,
    FORCE_C62_RF = 6,
    FORCE_C73_RF = 7,
    FORCE_C71_C72_RF = 8,
    FORCE_40G_RF = 9,
    FORCE_100G_RF = 10,
    FORCE_EGRESS_IDLE_C62_GFP = 11,
    INSERT_T2T_GFP_CMF_RDI = 12,
    FORCE_EGRESS_IDLE_C73 = 13,
    SQUELCH_PACKETS_ETRANS = 14,
    FORCE_CBRC_FORCE_FAULT = 16,
    FORCE_DIRECT_ILKN_PIF_SQUELCH = 17,
    INSERT_SONET_EGRESS_FORCE_FAULT = 18,
    /* The following action is similar to FORCE_C62_LF except upon seeing 
    valid traffic, it will not immediately clear the fault until the next FW polling
    period. */
    FORCE_C62_LF_NO_FAULT_AUTO_CLEAR = 19,
    FORCE_C62_LF_ENET_SYS = 20,
    FORCE_T2T_OPU_CSF = 21,
    FORCE_T2T_GFP_CMF_CSF_LOS = 22,
    FORCE_C73_LF_WITH_NULL_FRAME = 23,
    FORCE_C62_LF_WITH_NULL_FRAME = 24,
    INSERT_T2T_GFP_CMF_RDI_NO_SQUELCH = 25,
    INSERT_T2T_GFP_CMF_RDI_TXENA = 26,
    INSERT_T2T_RX_EMAC_SEND_RF = 27,
} digi_trunk2client_actions_t;


/*******************************************************************************
* ENUM: digi_led_cbrc_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the CBRC type for LED status
*    monitoring configuration. This is used for monitoring CBRC channels only.
*
* ELEMENTS:
*         DIGI_LED_CBRC_FC800  - CBRC link activity is monitored by CBRC FC800 block
*         DIGI_LED_CBRC_FC1200 - CBRC link activity is monitored by CBRC FC1200 block
*
*******************************************************************************/
typedef enum
{
    DIGI_LED_CBRC_FC800  = 0,
    DIGI_LED_CBRC_FC1200 = 1
} digi_led_cbrc_type_t;

/*******************************************************************************
* ENUM: digi_led_interface_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the interface for LED status
*    monitoring configuration.
*
* ELEMENTS:
*           DIGI_LED_10G_ENET_LINE      - 10G ENET Line
*           DIGI_LED_10G_ENET_SYSTEM    - 10G ENET System
*           DIGI_LED_10G_CBRC_LINE      - 10G CBRC Line
*           DIGI_LED_10G_CBRC_SYSTEM    - 10G CBRC System
*           DIGI_LED_40G_ENET_LINE      - 40G ENET Line
*           DIGI_LED_40G_ENET_SYSTEM    - 40G ENET System
*           DIGI_LED_100G_ENET_LINE     - 100G ENET Line
*           DIGI_LED_100G_ENET_SYSTEM   - 100G ENET System
*           DIGI_LED_ENET_MGMT          - ENET MGMT
*           DIGI_LED_RI_RCP             - RI_RCP
*           DIGI_LED_DI_RCP             - DI_RCP
*           DIGI_LED_OTN_OH1            - OTN_OH1
*           DIGI_LED_OTN_OH2            - OTN_OH2
*           DIGI_LED_OTN_OH3            - OTN_OH3
*
*******************************************************************************/
typedef enum
{
    DIGI_LED_10G_ENET_LINE      = 0,
    DIGI_LED_10G_ENET_SYSTEM    = 1,
    DIGI_LED_10G_CBRC_LINE      = 2,
    DIGI_LED_10G_CBRC_SYSTEM    = 3,
    DIGI_LED_40G_ENET_LINE      = 4,
    DIGI_LED_40G_ENET_SYSTEM    = 5,
    DIGI_LED_100G_ENET_LINE     = 6,
    DIGI_LED_100G_ENET_SYSTEM   = 7,
    DIGI_LED_ENET_MGMT          = 8,
    DIGI_LED_RI_RCP             = 9,
    DIGI_LED_DI_RCP             = 10,
    DIGI_LED_OTN_OH1            = 11,
    DIGI_LED_OTN_OH2            = 12,
    DIGI_LED_OTN_OH3            = 13
} digi_led_interface_t;

/*******************************************************************************
* ENUM: digi_dcsu_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This enumeration is used to identify DCSU modules that are configured by
*   SW driver.
*
* ELEMENTS:
*
*   DIGI_DCSU_INST_OTN1  - OTN1 DCSU
*   DIGI_DCSU_INST_OTN2  - OTN2 DCSU
*   DIGI_DCSU_INST_ENET3 - ENET3 DCSU
*   DIGI_DCSU_INST_ENET1 - ENET1 DCSU
*   DIGI_DCSU_INST_ENET2 - ENET2 DCSU
*   DIGI_DCSU_INST_PTP   - PTP DCSU
*   DIGI_DCSU_INST_SYS1  - SYS1 DCSU
*   DIGI_DCSU_INST_SYS2  - SYS2 DCSU
*******************************************************************************/
typedef enum {

    DIGI_DCSU_INST_OTN1  = 0,
    DIGI_DCSU_INST_OTN2,
    DIGI_DCSU_INST_ENET3,
    DIGI_DCSU_INST_ENET1,
    DIGI_DCSU_INST_ENET2,
    DIGI_DCSU_INST_PTP,
    DIGI_DCSU_INST_SYS1,
    DIGI_DCSU_INST_SYS2,

    LAST_DIGI_DCSU_INST
} digi_dcsu_inst_t;



/*******************************************************************************
* ENUM: digi_opsa_inst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for for specifying the instance of a MPMA and
*    MPMO.\n\n
*
*    For product applications which are not DIGI_PROD_APP_SYSOTN_CARD, the FO1
*    is associated with path between ODUKSW and LINEOTN, and the CTL is not
*    used.\n\n
*
*    For product application DIGI_PROD_APP_SYSOTN_CARD, FO1 is associated with
*    path between ODUKSW and SYSOTN and CTL is associated with path between
*    ODUKSW and LINEOTN.\n\n
*
*    FO2 is always associated with Mapper and Demapper path between ODUKSW and
*    DCPB.
*
* ELEMENTS:
*           DIGI_OPSA_COREOTN_FO1    - COREOTN FO1 instance ODUK <-> MUX/DEMUX/TX/RX framer
*           DIGI_OPSA_COREOTN_FO2    - COREOTN FO2 instance ODUK <-> MAPOTN
*           DIGI_OPSA_COREOTN_CTL    - COREOTN CTL instance ODUK <-> COREOTN_CTL
*
*******************************************************************************/
typedef enum
{
    DIGI_OPSA_COREOTN_FO1      = 0,
    DIGI_OPSA_COREOTN_FO2      = 1,
    DIGI_OPSA_COREOTN_CTL      = 2,
} digi_opsa_inst_t;



/*******************************************************************************
* ENUM: digi_opsa_csi_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for client status indication.
*
* ELEMENTS:
*    DIGI_OPSA_CSI_NORMAL     - Normal client status indication
*    DIGI_OPSA_CSI_SSF        - SSF client status indication
*    DIGI_OPSA_CSI_SF         - SF client status indication
*    DIGI_OPSA_CSI_SD         - SD client status indication
*    DIGI_OPSA_CSI_FORCE_TO   - Force TO client status indication
*    DIGI_OPSA_CSI_FORCE_AWAY - Force Away client status indication
*
*******************************************************************************/
typedef enum {
    DIGI_OPSA_CSI_NORMAL     = 0x00,
    DIGI_OPSA_CSI_SSF        = 0x01,
    DIGI_OPSA_CSI_SF         = 0x02,
    DIGI_OPSA_CSI_SD         = 0x03,
    DIGI_OPSA_CSI_FORCE_TO   = 0x04,
    DIGI_OPSA_CSI_FORCE_AWAY = 0x05
} digi_opsa_csi_type_t;


/*******************************************************************************
* ENUM: digi_fec_rx_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type Rx FEC processing mode.
*
* ELEMENTS:
*    DIGI_FEC_RX_ENABLED              - Rx FEC Processing
*    DIGI_FEC_RX_CORRECTION_DISABLED  - No Rx FEC Processing, maintaining FEC 
*                                       datapath latency (NO FEC correction)
*    DIGI_FEC_RX_BYPASSED             - No Rx FEC Processing, FEC bypassed, 
*                                       lowest latency
*
*******************************************************************************/
typedef enum {
    DIGI_FEC_RX_ENABLED              = 0,
    DIGI_FEC_RX_CORRECTION_DISABLED  = 1,
    DIGI_FEC_RX_BYPASSED             = 2,
    LAST_DIGI_FEC_RX

} digi_fec_rx_mode_t;

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* ENUM: digi_fw_log_prov_deprov_test_type
* ______________________________________________________________________________
*
* DESCRIPTION:
*   enumeration for various types of benchmarks
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    PROV_DEPROV_NO_QUEUE = 0,
    PROV_DEPROV_RECORD_TO_FILE,
    PROV_DEPROV_RECORD_TO_MEMORY,
    PROV_DEPROV_WITHOUT_ATOMIC,
    PROV_DEPROV_WITH_ATOMIC,
    PROV_DEPROV_PLAYBACK_FILL,
    PROV_DEPROV_PLAYBACK_WAIT,
    PROV_DEPROV_PLAYBACK,
    PROV_DEPROV_TIMED_PLAYBACK,
    PROV_DEPROV_BENCH_PLAYBACK,
    PROV_DEPROV_NUM
} digi_fw_log_prov_deprov_test_type;

#endif /* DOXYGEN_PUBLIC_ONLY */

/*
** Constants
*/


/* defines the Digi software version
* 0x0mmn_nbbb
* m - major
* n - minor
* b - build
*/
#ifndef DIGI_SW_VERSION
/* These 3 values are auto-generated during formal-build process */
#define DIGI_SW_VERSION_MM 3
#define DIGI_SW_VERSION_NN 12
#define DIGI_SW_BUILD_NUMBER 1961

#define DIGI_SW_VERSION (((DIGI_SW_VERSION_MM & 0xFF) << 20) + ((DIGI_SW_VERSION_NN & 0xFF) << 12) + ((DIGI_SW_BUILD_NUMBER & 0xFFF) << 0))
#endif

/* Hardcoded values for DIGI base addresses. */
#define DIGI0_BASE_ADDRESS          0x0
#define DIGI1_BASE_ADDRESS          0xDEADBEEF

#define DIGI_ILKN_LANES_MAX                  24

/* The maximum number of SERDES lanes used by the ENET Subsystems */
#define DIGI_SERDES_DSI_LANES_MAX            12
#define DIGI_SERDES_XFI_LANES_MAX            12
#define DIGI_SERDES_SYS_LANES_MAX            12
#define DIGI_SERDES_PIN_NOT_USED             0xFF
#define DIGI_ODU_CHNLS_MAX                   96
#define DIGI_ODU_PIDS_MAX                    256
#define DIGI_DCI_CHANNEL_UNASSIGNED          0xFF
#define DIGI_TRIBSLOT_UNUSED                 0xFF
#define DIGI_TRIBPORT_UNUSED                 0xFF
#define DIGI_CHANNEL_UNASSIGNED_FOR_TRIBPORT 0xFFFFFFFF
#define DIGI_SERDES_UNASSIGNED               0xFFFFFFFF
/* SFI 51 number of bus */
#define DIGI_NUM_SFI51_PORTS_MAX             2

/* Number of GE supporting interfaces */
#define DIGI_NUM_GE_INTF                     6

#define DIGI_MAPPER_NUM_SECONDARY_GROUPS     12
#define DIGI_NUM_ODU0S_PER_LANE_MAX          8
#define DIGI_NUM_CAL_ENTRIES_IN_10G          8
#define DIGI_NUM_CAL_ENTRIES_IN_20G          16
#define DIGI_NUM_CAL_ENTRIES_IN_30G          24
#define DIGI_NUM_CAL_ENTRIES_IN_40G          32
#define DIGI_NUM_CAL_ENTRIES_IN_50G          40
#define DIGI_NUM_CAL_ENTRIES_IN_60G          48
#define DIGI_NUM_CAL_ENTRIES_IN_70G          56
#define DIGI_NUM_CAL_ENTRIES_IN_80G          64
#define DIGI_NUM_CAL_ENTRIES_IN_90G          72
#define DIGI_NUM_CAL_ENTRIES_IN_100G         80

#define DIGI_NUM_DSI_LANES_IN_10G            1
#define DIGI_NUM_DSI_LANES_IN_20G            2
#define DIGI_NUM_DSI_LANES_IN_30G            3
#define DIGI_NUM_DSI_LANES_IN_40G            4
#define DIGI_NUM_DSI_LANES_IN_40G_SFIS       5
#define DIGI_NUM_DSI_LANES_IN_50G            5
#define DIGI_NUM_DSI_LANES_IN_60G            6
#define DIGI_NUM_DSI_LANES_IN_70G            7
#define DIGI_NUM_DSI_LANES_IN_80G            8
#define DIGI_NUM_DSI_LANES_IN_90G            9
#define DIGI_NUM_DSI_LANES_IN_100G           10

#define DIGI_SIFD_ILKN_MAX_CAL_ENTRIES       300
#define DIGI_SIFD_ILKN_NULL_CAL_ENTRY        255
#define DIGI_SIFD_ILKN_LINK_FLOW_CONTROL     254

#define DIGI_DPI_PORT_MAX                    8
#define DIGI_ENET_CHNL_MAX                   12
#define DIGI_ENET_LINE_CHNL_MAX              DIGI_ENET_CHNL_MAX
#define DIGI_ENET_SYS_CHNL_MAX               DIGI_ENET_CHNL_MAX
#define DIGI_CBRC_CHNL_MAX                   12

#define DIGI_MAPOTN_CHNL_MAX 96
#define DIGI_OTN_SERVER_CHNL_MAX             12
#define DIGI_OTN_SERVER_SYS_CHNL_MAX         1
#define DIGI_OTN_SERVER_HO_CHNL_MAX          12
#define DIGI_OTN_SERVER_MO_CHNL_MAX          96
#define DIGI_OTN_SERVER_LO_CHNL_MAX          96
#define DIGI_OTN_SERVER_MAX_TRIBSLOT         80
#define DIGI_OTN_SERVER_PIDS_PER_CHNL        ODU_STRUCT_PIDS_PER_CHNL
#define DIGI_FW_VERSION_SUPPORTED            0x0000
#define DIGI_FW_VERSION_NOT_SUPPORTED        0x0001
#define DIGI_FW_STATUS_ALIVE                 0xABCD


/* The number of SIFD/ILKN  instances */
#define DIGI_SIFD_ILKN_NUM                   2
#define DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX     UTIL_GLOBAL_ILKN_CPB_DPI_CHNLS_MAX
#define DIGI_SIFD_ILKN_ODUK_DPI_CHNLS_MAX    UTIL_GLOBAL_ILKN_ODUK_DPI_CHNLS_MAX
#define DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX     (DIGI_SIFD_ILKN_CPB_DPI_CHNLS_MAX + DIGI_SIFD_ILKN_ODUK_DPI_CHNLS_MAX)
#define DIGI_SIFD_BCW_CHANNEL_MAX            253
#define DIGI_SIFD_BCW_CHANNEL_LINK           254
#define DIGI_SIFD_BCW_CHANNEL_NULL           255

/* Number of clock fivider in PGMRCLK block */
#define DIGI_PGMRCLK_NUM_CLK_DIV             4

/* PGMRCLK number of clocks per indexed source clock */
#define DIGI_PGMRCLK_SFI_1_TX_NUM_CLKS                    16
#define DIGI_PGMRCLK_SFI_1_RX_NUM_CLKS                    16
#define DIGI_PGMRCLK_SFI_2_TX_NUM_CLKS                    16
#define DIGI_PGMRCLK_SFI_2_RX_NUM_CLKS                    16
#define DIGI_PGMRCLK_LINE_IF_TX_NUM_CLKS                  12
#define DIGI_PGMRCLK_LINE_IF_RX_NUM_CLKS                   12
#define DIGI_PGMRCLK_SYS_IF_TX_NUM_CLKS                   16
#define DIGI_PGMRCLK_SYS_IF_RX_NUM_CLKS                   16
#define DIGI_PGMRCLK_PCIE_IF_TX_NUM_CLKS                  4
#define DIGI_PGMRCLK_PCIE_IF_RX_NUM_CLKS                  4
#define DIGI_PGMRCLK_SR_IF_TX_NUM_CLKS                    8
#define DIGI_PGMRCLK_SR_IF_RX_NUM_CLKS                    8
#define DIGI_PGMRCLK_LR_IF_TX_NUM_CLKS                    40
#define DIGI_PGMRCLK_LR_IF_RX_NUM_CLKS                    40
#define DIGI_PGMRCLK_OTN1_DCSU_NUM_CLKS                   4
#define DIGI_PGMRCLK_OTN2_DCSU_NUM_CLKS                   2
#define DIGI_PGMRCLK_PTP_DCSU_NUM_CLKS                    1
#define DIGI_PGMRCLK_RCP_IF_RX_NUM_CLKS                   2
#define DIGI_PGMRCLK_RCP_IF_TX_NUM_CLKS                   2
#define DIGI_PGMRCLK_OTN_OH_IF_RX_NUM_CLKS                3
#define DIGI_PGMRCLK_OTN_OH_IF_TX_NUM_CLKS                3
#define DIGI_PGMRCLK_DEMAP_ENE_NUM_CLKS                   12
#define DIGI_PGMRCLK_INTERNAL_CLK_NUM_CLKS                17

#define DIGI_DPI_BUS_WIDTH                                48

#define DIGI_INT_ALL_CHNLS                                NULL
#define DIGI_INT_ALL_INTS                                 UTIL_GLOBAL_ALL_INTS

/* Number of serdes in BP interface */
#define DIGI_SERDES_MAX_T8_E1_IN_BP_LR                    UTIL_GLOBAL_MAX_T8_E1_IN_BP_LR 
#define DIGI_SERDES_MAX_C8_IN_BP_SR                       8
#define DIGI_SERDES_MAX_S16_IN_BP                         16

/* Maximum number of SFI51 TX and Rx D8 */
#define DIGI_SERDES_MAX_SFI51_D8_TXRX_18X   2
#define DIGI_SERDES_MAX_T8_2X               20

/* Number of rates associated to each serdes type */


/* Number of rates default values */
#if !defined(DIGI_SERDES_S16_NUM_RATES_MAX)
#define DIGI_SERDES_S16_NUM_RATES_MAX                     27
#endif/* !defined(DIGI_SERDES_S16_NUM_RATES_MAX) */

#if !defined(DIGI_SERDES_T8_NUM_RATES_MAX)
#define DIGI_SERDES_T8_NUM_RATES_MAX                      2
#endif/* !defined(DIGI_SERDES_T8_NUM_RATES_MAX) */

#if !defined(DIGI_SERDES_C8_NUM_RATES_MAX)
#define DIGI_SERDES_C8_NUM_RATES_MAX                      2
#endif/* !defined(DIGI_SERDES_C8_NUM_RATES_MAX) */

#if !defined(DIGI_SERDES_SFI51_D8_NUM_RATES_MAX)
#define DIGI_SERDES_SFI51_D8_NUM_RATES_MAX                4
#endif /*DIGI_SERDES_SFI51_D8_NUM_RATES_MAX*/

/* Maximum number of adapation paramaters */
#define DIGI_SERDES_ADAPT_PARAMS_MAX (DIGI_SERDES_MAX_S16_IN_BP * DIGI_SERDES_S16_NUM_RATES_MAX + \
    DIGI_SERDES_XFI_LANES_MAX * DIGI_SERDES_S16_NUM_RATES_MAX +    \
    DIGI_SERDES_MAX_C8_IN_BP_SR * DIGI_SERDES_C8_NUM_RATES_MAX +        \
    DIGI_SERDES_MAX_T8_E1_IN_BP_LR * DIGI_SERDES_T8_NUM_RATES_MAX)

#define DIGI_SERDES_PORT_DLOS_LOS_SOURCE                  0xFE
#define DIGI_SERDES_PORT_NO_LOS_SOURCE                    0xFD  /* LOS DISABLED */

/* Maximum number of DCSU instances */
#define DIGI_DCSU_MAX_INST                                LAST_DIGI_DCSU_INST
/*
 * Defined a constant rather than an enumerated type based on the fact that
 * integer and not emumerated types are passed and that the values are 
 * specific and not arbitrary as would be the case with an enumerated type.
 */
#define  DIGI_CMF_GFP_FRAME_CSF_LOS 1		/* Client signal fail (loss of client signal) */
#define  DIGI_CMF_GFP_FRAME_CSF_LOCS 2		/* Client signal fail (loss of character synchronization) */
#define  DIGI_CMF_GFP_FRAME_DCI 3
#define  DIGI_CMF_GFP_FRAME_FDI 4
#define  DIGI_CMF_GFP_FRAME_RDI 5
/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: digi_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Opaque structure cookie.  Used for the identification of a DIGI device
*   instance.
*
*******************************************************************************/
typedef struct digi_handle_struct_t digi_handle_t;

/*******************************************************************************
* STRUCTURE: digi_ilkn_lane_remap_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining SERDES lane remapping
*
* ELEMENTS:
*       lane - array containing remapped lane numbers
*
*******************************************************************************/
typedef struct digi_ilkn_lane_remap_cfg_t
{
    UINT8 lane[DIGI_ILKN_LANES_MAX];
} digi_ilkn_lane_remap_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_sifd_ilkn_rl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining Interlaken TX rate limit
*
* ELEMENTS:
*       rate_limit               - rate limit in 1 Gb/s increments (from 0 to 100)\n
*                                    0 -- disable TX rate limit\n
*                                    1 to 100 -- enable TX rate limit to value in Gb/s
*       burst_limit              - maxiumum quantity of data that may be burst across
*                                  the Interlaken interface before invoking the rate
*                                  limiting logic (in bytes)
*       byte_granularity         - number of credits that are to be subtracted from
*                                  the token bucket when an EOP is encountered.\n
*                                    0 -- 1 byte\n
*                                    1 -- 2 bytes\n
*                                    2 -- 4 bytes\n
*                                    3 -- 8 bytes\n
*                                    4 -- 16 bytes\n
*                                    5 -- 32 bytes
*
*******************************************************************************/
typedef struct digi_sifd_ilkn_rl_t 
{
    UINT32 rate_limit;
    UINT32 burst_limit;
    UINT32 byte_granularity;
} digi_sifd_ilkn_rl_t;

/*******************************************************************************
* STRUCTURE: digi_sifd_ilkn_calendar_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the Interlaken flow control calendar.  There is a calendar
*   for each direction of Interlaken transmission.  Each calendar can contain up
*   to 300 entries (defined by fc_cal_length).  Each entry corresponds to a flow
*   control bit that is passed either in-band or out-of-band on the Interlaken
*   interface.
*
*   Each entry can be one of:
*
*        DIGI_SIFD_ILKN_NULL_CAL_ENTRY      -- calendar entry is unused
*        DIGI_SIFD_ILKN_LINK_FLOW_CONTROL   -- calendar entry is passing flow control
*                                              for the link
*        channel number                     -- calendar entry is passing flow control
*                                              for a specific BCW channel ID (from 0
*                                              to 253)
*
*   Channels can represent arbitrary bandwidth from 1.25G to 100G.  The speed of flow
*   control response (i.e reacting to either XOFF or XON) is controlled by how the
*   calendar entries are populated.  Specifically, how many calendar entries have the
*   same channel number and the distribution of these entries over the complete calendar.
*   For example, if you were using in-band flow control with 16 flow control bits
*   per ICW/BCW and 300 calendar entries (which requires 18 ICW/BCW to transfer):
*
*        link flow control -- use 1 entry every ICW/BCW
*        100G channel      -- use 1 entry every ICW/BCW  (consumes all the bandwidth)
*        40G channel       -- use 1 entry every 3 ICW/BCW  (consumes 1/3 of the bandwidth)
*        10G channel       -- use 1 entry every 12 ICW/BCW  (consumes 1/12 of the bandwidth)
*        1.25G (ODU0) chan -- use 1 entry every 18 ICW/BCW  (consumes 1/96 of the bandwidth)
*
*   These are example usages.  In a real system LMAX delay would provide further criteria
*   for populating the calendar (e.g. how frequently the channel is reported).
*
*   There are two other modes of flow control:
*
*        -- in-band with 24 flow control bits per ICW/BCW (300 calendar entries require
*           13 ICW/BCW)
*        -- out-of-band flow control is sent as a repetitive frame (length fc_cal_length)
*           using a 100Mhz clock (300 calendar entries would be sent every 3.01 useconds)
*
*   For in-band flow control, if the calendar length is not a modulo of 16 or 24,
*   then the excess flow control bits (to complete the modulo) are unused (set to XOFF).
*
*   When the calendar length (fc_cal_length) is less than 300, the unused entries should
*   be populated with DIGI_SIFD_ILKN_NULL_CAL_ENTRY (set to XOFF).
*
* ELEMENTS:
*       entry[DIGI_SIFD_ILKN_MAX_CAL_ENTRIES] - array of 300 calendar entries
*
*******************************************************************************/
typedef struct digi_sifd_ilkn_calendar_t
{
    UINT8 entry[DIGI_SIFD_ILKN_MAX_CAL_ENTRIES];
} digi_sifd_ilkn_calendar_t;

/*******************************************************************************
* STRUCTURE: digi_sifd_ilkn_calendar32_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This calendar is similar to digi_sifd_ilkn_calendar_t except that the entries
*   are 32 bits wide instead of 8. This is required because it needs to hold 2 byte
*   header values instead of BCW channels.
*
* ELEMENTS:
*   entry - array of 300 calendar entries
*
*******************************************************************************/
typedef struct digi_sifd_ilkn_calendar32_t
{
    UINT32 entry[DIGI_SIFD_ILKN_MAX_CAL_ENTRIES];
} digi_sifd_ilkn_calendar32_t;

/*******************************************************************************
* STRUCTURE: digi_sifd_ilkn_cfg_t
* _____________________________________________________________________
*
* DESCRIPTION:
*   Structure defining common Interlaken port properties.
*
*   Note the following relationship between the land mask and the lane order.
*   The ILKN has 24 physical serdes lanes they can be remaped to 24 logical lanes.
*   The lane mask specifies the enabled lanes for BOTH the physical lanes and
*   the logical lanes.
*  
*   For example:
*   When lane_mask = 0x00_FF_00, the lane_order[8..15] must be programmed with any
*   value between 8 and 15 such that each entry is unique. lane_order[8] = 15
*   means that logical lane 8 maps to physical serdes lane 15. Disabled lanes
*   must have a value of 0xFF in the lane_order array.
* 
* ELEMENTS:
*       rate                    - enum, Interlaken Serdes rate
*       lane_mask               - definition of the set of serdes lanes used at the
*                               backplane interface of the interlaken block.  24 bits,
*                               with the index representing the set of 24 possible lanes
*                               connected to the ILKN block.  Set to 1 for a valid lane,
*                               set to 0 for an invalid lane.
*       lane_tx_order           - sequence of set of TX lanes specified by lane_mask
*                               used for ILKN link processing
*       lane_rx_order           - sequence of set of RX lanes specified by lane_mask
*                               used for ILKN link processing
*       burst                   - enum, Interlaken burst length
*       burst_min               - enum, Interlaken burst minimal length
*       burst_short             - enum, Interlaken burst short length
*       metaframe               - enum, metaframe period length
*       tx_rate_limit           - structure for Interlaken TX rate limit
*
*       oduksw_intf_en          - BOOL8, Interlaken interconnection with ODUKSW
*                               TRUE: Interlaken to ODUKSW enabled
*                               FALSE: Interlaken to ODUKSW disabled
*       oduksw_tx_xfer_mode     - enum, ODUKSW traffic packet vs segment mode in the
*                               tx direction (SIFD to system)
*       oduksw_header           - enum, optional ODUKSW Interlaken transfer headers
*       oduksw_2b_header_mode   - BOOL8, ODUKSW Rx 2B header mode\n
*                               TRUE:  Mode 1, header compared with expected value
*                               FALSE: Mode 0, header used to identify ODUKSW DPI
*                               channel ID.
*       cpb_intf_en             - BOOL8, Interlaken interconnection with CPB
*                               TRUE: Interlaken to CPB enabled
*                               FALSE: Interlaken to CPB disabled
*       cpb_tx_xfer_mode        - enum, CPB traffic packet vs segment mode in the
*                               tx direction (SIFD to system)
*       rx_xfer_mode            - enum, CPB and ODUKSW traffic packet vs. segment
*                               mode in the rx direction (system to SIFD)
*       fc_mode                 - enum defining the flow control options
*       fc_cal_length           - number of entries in the flow control calendar
*                               1 to 300
*
*******************************************************************************/
typedef struct digi_sifd_ilkn_cfg_t
{
    digi_ilkn_serdes_rate_t rate;
    UINT32 lane_mask;
    digi_ilkn_lane_remap_cfg_t lane_tx_order;
    digi_ilkn_lane_remap_cfg_t lane_rx_order;

    digi_sifd_burst_t       burst;
    digi_sifd_burst_min_t   burst_min;
    digi_sifd_burst_short_t burst_short;

    digi_sifd_ilkn_mf_t    metaframe;
    digi_sifd_ilkn_rl_t    tx_rate_limit;

    BOOL8 oduksw_intf_en;
    digi_sifd_xfer_mode_t  oduksw_tx_xfer_mode;
    digi_sifd_oduksw_ilkn_header_t oduksw_header;
    BOOL8 oduksw_2b_header_mode;
    BOOL8 cpb_intf_en;
    digi_sifd_xfer_mode_t  cpb_tx_xfer_mode;
    digi_sifd_xfer_mode_t  rx_xfer_mode;

    digi_sifd_fc_mode_t fc_mode;
    UINT32 fc_cal_length;

} digi_sifd_ilkn_cfg_t;


/*******************************************************************************
* STRUCTURE: digi_sifd_ilkn_1_lane_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the set of serdes ports associated with the 24 lanes
*   of ILKN1.
*
*   The valid Serdes that ILKN1 may connect to are:\n
* ______________________________________________________________________________\n
*
*   The E1 Serdes, Lanes[39:20]\n
*   The N1 Serdes, Lanes[15:00]\n
*   The N2 Serdes, Lanes[07:00]\n
*
* ILKN1 Lanes can be mapped to the following serdes lanes
*
*  ILKN1 Lanes 23 through 16:
*
*  \code
* ILKN1 --------------------------------[23][22][21][20][19][18][17][16]
*    E1 ------------------------------------------------[20][21][22][23]
*    N1 ----------------------------------------------------------------
*    N2 --------------------------------[ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]\endcode
*
*
*  ILKN1 Lanes 15 through 0:
*
*  \code
* ILKN1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]
*    E1 [24][25][26][27][28][29][30][31][32][33][34][35][36][37][38][39]
*    N1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]
*    N2 ----------------------------------------------------------------\endcode
*
* ELEMENTS:
*       blk_to_E1_enable_mask - 24 bits defining if each ILKN1 lane associated
*                               with E1 Serdes or not.
*       blk_to_N1_enable_mask - 24 bits defining if each ILKN1 lane associated
*                               with N1 Serdes or not.
*       blk_to_N2_enable_mask - 24 bits defining if each ILKN1 lane associated
*                               with N2 Serdes or not.
*
* NOTE:
*       The offsets of E1, N1 and N2 masks must be mutually exclusively set as
*       only one serdes port may be associated with an ILKN1 lane
*
*******************************************************************************/
typedef struct digi_sifd_ilkn_1_lane_cfg_t
{
    UINT32 blk_to_E1_enable_mask;
    UINT32 blk_to_N1_enable_mask;
    UINT32 blk_to_N2_enable_mask;

} digi_sifd_ilkn_1_lane_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_sifd_ilkn_2_lane_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the set of serdes ports associated with the 24 lanes
*   of ILKN2.
*
*   The valid Serdes that ILKN2 may connect to are:\n
* ______________________________________________________________________________\n
*
*   The E1 Serdes, Lanes[23:00]\n
*   The N1 Serdes, Lanes[15:08]\n
*   The N2 Serdes, Lanes[07:00]\n
*
*  ILKN2 Lanes can be mapped to the following serdes lanes
*
*  ILKN2 Lanes 23 through 16:
*
*  \code
* ILKN2 --------------------------------[23][22][21][20][19][18][17][16]
*    E1 --------------------------------[23][22][21][20][19][18][17][16]
*    N1 ----------------------------------------------------------------
*    N2 ----------------------------------------------------------------\endcode
*
*  ILKN2 Lanes 15 through 0:
*
*  \code
* ILKN2 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]
*    E1 [15][14][13][12][11][10][ 9][ 8][ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]
*    N1 --------------------------------[15][14][13][12][11][10][ 9][ 8]
*    N2 [ 7][ 6][ 5][ 4][ 3][ 2][ 1][ 0]--------------------------------\endcode
*
* ELEMENTS:
*       blk_to_E1_enable_mask - 24 bits defining if each ILKN1 lane associated
*                               with E1 Serdes or not.
*       blk_to_N1_enable_mask - 24 bits defining if each ILKN1 lane associated
*                               with N1 Serdes or not.
*       blk_to_N2_enable_mask - 24 bits defining if each ILKN1 lane associated
*                               with N2 Serdes or not.
*
* NOTE:
*       The offsets of E1, N1 and N2 masks must be mutually exclusively set as
*       only one serdes port may be associated with an ILKN2 lane
*
*******************************************************************************/
typedef struct digi_sifd_ilkn_2_lane_cfg_t
{
    UINT32 blk_to_E1_enable_mask;
    UINT32 blk_to_N1_enable_mask;
    UINT32 blk_to_N2_enable_mask;

} digi_sifd_ilkn_2_lane_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_sifd_ilkn_1_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining Interlaken 1 port properties.
*
* ELEMENTS:
*       ilkn            - sturcture defining common Interlaken properties
*       lane            - structure defining Interlaken 1 lane specific
*                         properties
*
*
*******************************************************************************/
typedef struct digi_sifd_ilkn_1_cfg_t
{
    /* SIFD block config */
    digi_sifd_ilkn_cfg_t ilkn;

    /* ILKN1 specific configuration */
    digi_sifd_ilkn_1_lane_cfg_t lane;

} digi_sifd_ilkn_1_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_sifd_ilkn_2_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining Interlaken 2 port properties.
*
* ELEMENTS:
*       ilkn            - sturcture defining common Interlaken properties
*       lane            - structure defining Interlaken 1 lane specific
*                         properties
*
*
*******************************************************************************/
typedef struct digi_sifd_ilkn_2_cfg_t
{
    /* SIFD block config */
    digi_sifd_ilkn_cfg_t ilkn;

    /* SIFD2 specific configuration */
    digi_sifd_ilkn_2_lane_cfg_t lane;

} digi_sifd_ilkn_2_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_enet_client_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Opaque structure cookie.  Used for the identification of ENET_LINE or
*   ENET_SYS subsystems processing a single logical channel.
*
*******************************************************************************/
typedef void digi_enet_client_chnl_t;

/*******************************************************************************
* STRUCTURE: digi_cbr_client_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Opaque structure cookie.  Used for the identification of CBRC Subsystem
*   processing resource operating on a single logical connection.
*
*******************************************************************************/
typedef void digi_cbr_client_chnl_t;

/*******************************************************************************
* STRUCTURE: digi_ilkn_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Opaque structure cookie.  Used for the identification of a Interlaken link
*   single logical channel connecting to CPB subsystem.
*
*******************************************************************************/
typedef void digi_ilkn_chnl_t;

/*******************************************************************************
* STRUCTURE: digi_otn_server_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Opaque structure cookie.  Used for the identification of a OTU framer channel
*   processing resource.
*
*******************************************************************************/
typedef void digi_otn_server_chnl_t;

/*******************************************************************************
* STRUCTURE: digi_otn_odu_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Opaque structure cookie.  Used for the identification of a ODU framer channel
*   processing resource.
*
*
*******************************************************************************/
typedef void digi_otn_odu_chnl_t;

/*******************************************************************************
* STRUCTURE: digi_mapper_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Opaque structure cookie.  Used for the identification of a mapping path for
*   a single channel.  The mapping path composes physical device resources in
*   COREOTN FO2 block, MAPOTN and optionally ENET_LINE subsystems.
*
*******************************************************************************/
typedef void digi_mapper_chnl_t;

/*******************************************************************************
* STRUCTURE: digi_ilkn_link_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Opaque structure cookie.  Used for the identification of an Interlaken
*   backplane instance.
*
*******************************************************************************/
typedef void digi_ilkn_link_t;

/*******************************************************************************
* STRUCTURE: digi_serdes_port_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Opaque structure cookie.  Used for the identification of a serdes port
*   providing a single logical connection.
*
*******************************************************************************/
typedef void digi_serdes_port_t;

/*******************************************************************************
* STRUCTURE: digi_sw_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*       Structure that constains the software status elements
*
* ELEMENTS:
*       sw_version           - the software version
*       build_device_id      - the digi device ID the SW was compiled with
*       device_id            - the digi device ID as read from the device
*       device_revision_code - the digi device revision as read from the device
*       last_reset_reason    - the reason for the last reset
*       read_write_status    - status of the digi read/write test. TRUE if
*                              successful read/write, FALSE otherwise.
*
*******************************************************************************/
typedef struct digi_sw_status_t
{
    UINT32 sw_version;
    UINT16 build_device_id;
    UINT16 device_id;
    /* Device revision code is only a 4 bit register.
     * [7:4] = 0, [3:0] = revision_code */
    UINT8  device_revision_code;
    digi_reset_reason_t last_reset_reason;
    UINT32 read_write_status;
} digi_sw_status_t;

/*******************************************************************************
* STRUCTURE: digi_fw_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*       Structure that constains the firmware status elements
*
* ELEMENTS:
*       bootloader_version          - The Digi FW bootloader version number
*                                     read from the device
*       bootloader_build            - The Digi FW bootloader build number read
*                                     the device
*       fw_version                  - the Digi application FW version read from
*                                     the device.
*       fw_build                    - the Digi application FW build number read
*                                     from the device.
*       fw_is_alive                 - TRUE if the device 'is alive',
*                                     FALSE otherwise.
*       fw_version_not_supported    - TRUE if the current loaded FW version is
*                                     NOT supported on the device revision,
*                                     FALSE otherwise(FW version is supported).
*       gen_excp_detected           - TRUE if a general exception was detected,
*                                     FALSE otherwise.
*       sw_rst_detected             - TRUE if the fw was reset by the sw host,
*                                     FALSE otherwise.
*       wdog_rst_detected           - TRUE if the device was reset by the
*                                     watchdog timer, FALSE otherwise.
*       fw_is_ready                 - TRUE if the FW is operational and can
*                                     support host messages, FALSE otherwise.
*       enabled_fw_module_bitfield  - a bit-field to indicate which FW module
*                                     is enabled.
*       disabled_fw_module_bitfield - a bit-field to indicate which FW module
*                                     is disabled.
*
*******************************************************************************/
typedef struct digi_fw_status_t
{
    /* bootloader status */
    UINT32 bootloader_version;
    UINT32 bootloader_build;
    /* application FW status */
    UINT32 fw_version;
    UINT32 fw_build;

    BOOL   fw_is_alive;
    /* from APP_USER1 register */
    BOOL   fw_version_not_supported;
    BOOL   gen_excp_detected;
    BOOL   sw_rst_detected;
    BOOL   wdog_rst_detected;
    BOOL   fw_is_ready;
    UINT32 enabled_fw_module_bitfield;
    UINT32 disabled_fw_module_bitfield;

} digi_fw_status_t;

/*******************************************************************************
* STRUCTURE: digi_port_usage_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure providing the usage for one serdes port.
*   This includes a pointer to the port, the subsystem it is provisioned
*   against, if it has been mapped using the mapper, and if so the number of
*   ODU0s that have been mapped against it.
*
* ELEMENTS:
*       port_ctxt_ptr   - a pointer to port context
*       prov_state      - the provisioned state of the port.
*       grp_alloc       - the mapper state of the port.
*       num_odu0        - the number of odu0s provisioned against this port.
*
*******************************************************************************/
typedef struct digi_port_usage_struct_t 
{

    digi_serdes_port_t* port_ctxt_ptr;
    digi_serdes_prov_state_t prov_state;
    digi_mapper_alloc_t grp_alloc;
    UINT8 num_odu0;
} digi_port_usage_struct_t;

/*******************************************************************************
* STRUCTURE: digi_port_usage_grp_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure providing the usage for a group of SERDES ports.
*   The Array index is (primary_group number * 4) + secondary_group number. \n
*   The array size is 12 elements long.
*
* ELEMENTS:
*       port_usage      - the port usage structure containing the port
*                         information. See the structure definition for more
*                         information.
*
*******************************************************************************/
typedef struct digi_port_usage_grp_struct_t 
{
    digi_port_usage_struct_t port_usage[DIGI_MAPPER_NUM_SECONDARY_GROUPS];
} digi_port_usage_grp_struct_t;

/*******************************************************************************
* STRUCTURE: serdes_c8_adapt_objects_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   C8 SERDES Adaptation Objects
*
* ELEMENTS:
*   valid            - adaptation objects status.
*   port_mode        - serdes rate associated to adaptation objects.
*   dp_ffe_a         - See Serdes reg doc.
*   target_amp       - See Serdes reg doc.
*   adc_map_scale    - See Serdes reg doc.
*******************************************************************************/
typedef struct serdes_c8_adapt_objects_t
{
    BOOL8  valid;
    digi_serdes_port_mode_t port_mode;
    UINT32 dp_ffe_a;
    UINT32 target_amp;
    UINT32 adc_map_scale;
} serdes_c8_adapt_objects_t;

/*******************************************************************************
* STRUCTURE: serdes_t8_adapt_objects_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   T8 SERDES Adaptation Objects
*
* ELEMENTS:
*   valid            - adaptation objects status.
*   port_mode        - serdes rate associated to adaptation objects.
*   dp_ffe_a         - See Serdes reg doc.
*   dp_ffe_b         - See Serdes reg doc.
*   dp_ffe_c         - See Serdes reg doc.
*   tr_ffe_a         - See Serdes reg doc.
*   tr_ffe_b         - See Serdes reg doc.
*   dfe_coef0        - See Serdes reg doc.
*   dfe_coef1        - See Serdes reg doc.
*   dfe_coef2        - See Serdes reg doc.
*   dfe_coef3        - See Serdes reg doc.
*   dfe_coef4        - See Serdes reg doc.
*   dfe_coef5        - See Serdes reg doc.
*   pga_gain         - See Serdes reg doc.
*   pga_offset       - See Serdes reg doc.
*   target_amp       - See Serdes reg doc.
*   adc_map_scale    - See Serdes reg doc.
*******************************************************************************/
typedef struct serdes_t8_adapt_objects_t
{
    BOOL8  valid;
    digi_serdes_port_mode_t port_mode;
    UINT32 dp_ffe_a;
    UINT32 dp_ffe_b;
    UINT32 dp_ffe_c;
    UINT32 tr_ffe_a;
    UINT32 tr_ffe_b;
    UINT32 dfe_coef0;
    UINT32 dfe_coef1;
    UINT32 dfe_coef2;
    UINT32 dfe_coef3;
    UINT32 dfe_coef4;
    UINT32 dfe_coef5;
    UINT32 pga_gain;
    UINT32 pga_offset;
    UINT32 target_amp;
    UINT32 adc_map_scale;
} serdes_t8_adapt_objects_t;

/*******************************************************************************
* STRUCTURE: serdes_s16_adapt_objects_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   S16 SERDES Adaptation Objects
*
* ELEMENTS:
*   valid            - adaptation objects status.
*   port_mode        - serdes rate associated to adaptation objects.
*   dp_ffe_a         - See Serdes reg doc.
*   dp_ffe_b         - See Serdes reg doc.
*   dp_ffe_c         - See Serdes reg doc.
*   tr_ffe_a         - See Serdes reg doc.
*   tr_ffe_b         - See Serdes reg doc.
*   dfe_coef0        - See Serdes reg doc.
*   dfe_coef1        - See Serdes reg doc.
*   dfe_coef2        - See Serdes reg doc.
*   dfe_coef3        - See Serdes reg doc.
*   dfe_coef4        - See Serdes reg doc.
*   dfe_coef5        - See Serdes reg doc.
*   pga_gain         - See Serdes reg doc.
*   pga_offset       - See Serdes reg doc.
*   target_amp       - See Serdes reg doc.
*   adc_map_scale    - See Serdes reg doc.
*******************************************************************************/
typedef struct serdes_s16_adapt_objects_t
{
    BOOL8  valid;
    digi_serdes_port_mode_t port_mode;
    UINT32 dp_ffe_a;
    UINT32 dp_ffe_b;
    UINT32 dp_ffe_c;
    UINT32 tr_ffe_a;
    UINT32 tr_ffe_b;
    UINT32 dfe_coef0;
    UINT32 dfe_coef1;
    UINT32 dfe_coef2;
    UINT32 pga_gain;
    UINT32 pga_offset;
    UINT32 target_amp;
    UINT32 adc_map_scale;
} serdes_s16_adapt_objects_t;

/*******************************************************************************
* STRUCTURE: digi_serdes_c8_tx_swing_levels_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   C8 SERDES TX swing and de-emphasis levels.
*   This structure will be valid only when
*   "digi_serdes_c8_tx_swing_emphasis_levels_in_ctxt_set"
*   function was called for the specific SerDes lane
*
* ELEMENTS:
*   valid            - adaptation objects status.
*   port_mode        - serdes rate associated to adaptation objects.
*   levels           - tx swing and de-emphasis levels.
*******************************************************************************/
typedef struct digi_serdes_c8_tx_swing_levels_t
{
    BOOL8                      valid;
    digi_serdes_port_mode_t    port_mode;
    c8_tx_swing_levels_t       levels;
} digi_serdes_c8_tx_swing_levels_t;

/*******************************************************************************
* STRUCTURE: digi_serdes_t8_tx_swing_levels_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   T8 SERDES TX swing and de-emphasis levels.
*   This structure will be valid only when "digi_serdes_t8_tx_swing_emphasis_levels_in_ctxt_set"
*   function was called for the specific SerDes lane.
*
* ELEMENTS:
*   valid            - adaptation objects status.
*   port_mode        - serdes rate associated to adaptation objects.
*   levels           - tx swing and de-emphasis levels.
*******************************************************************************/
typedef struct digi_serdes_t8_tx_swing_levels_t
{
    BOOL8                      valid;
    digi_serdes_port_mode_t    port_mode;
    t8_tx_swing_levels_t       levels;
} digi_serdes_t8_tx_swing_levels_t;

/*******************************************************************************
* STRUCTURE: digi_serdes_s16_tx_swing_levels_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   S16 SERDES TX swing and de-emphasis levels.
*   This structure will be valid only when "digi_serdes_s16_tx_swing_emphasis_set"
*   function was called for the specific SerDes lane.
*
* ELEMENTS:
*   valid            - adaptation objects status.
*   port_mode        - serdes rate associated to adaptation objects.
*   levels           - tx swing and de-emphasis levels.
*******************************************************************************/
typedef struct digi_serdes_s16_tx_swing_levels_t
{
    BOOL8                       valid;
    digi_serdes_port_mode_t     port_mode;
    s16_tx_swing_levels_t       levels;
} digi_serdes_s16_tx_swing_levels_t;

/*******************************************************************************
* STRUCTURE: digi_serdes_port_s16_line_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure for defining S16 Line serdes configuration.
*
* ELEMENTS:
*  s16_line_adapt_params       - S16 adaptation parameters for each rate
*  s16_line_tx_swing_levels    - TX swing and de-emphasis levels per rate
*  dc_coupling_enabled         - Serdes coupling mode: DC when true, AC when FALSE
*  polarity_inversed           - Serdes inversed polarity in the folowing directions:
*                                 * RX (UTIL_GLOBAL_DIR_RX)
*                                 * TX (UTIL_GLOBAL_DIR_RX)
*                                 * BOTH (UTIL_GLOBAL_DIR_BOTH)
*                                 * NONE (LAST_UTIL_GLOBAL_DIR)
*******************************************************************************/
typedef struct digi_serdes_port_s16_line_cfg_t
{
    serdes_s16_adapt_objects_t          s16_line_adapt_params[DIGI_SERDES_S16_NUM_RATES_MAX];
    digi_serdes_s16_tx_swing_levels_t   s16_line_tx_swing_levels[DIGI_SERDES_S16_NUM_RATES_MAX];
    BOOL8                               dc_coupling_enabled;
    util_global_direction_t             polarity_inversed;
}digi_serdes_port_s16_line_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_serdes_port_s16_sys_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure for defining S16 system serdes configuration.
*
* ELEMENTS:
*  s16_sys_adapt_params        - S16 adaptation parameters for each rate
*  s16_sys_tx_swing_levels     - TX swing and de-emphasis levels per rate
*  dc_coupling_enabled         - Serdes coupling mode: DC when true, AC when FALSE
*  polarity_inversed           - Serdes inversed polarity in the folowing directions:
*                                 * RX (UTIL_GLOBAL_DIR_RX)
*                                 * TX (UTIL_GLOBAL_DIR_RX)
*                                 * BOTH (UTIL_GLOBAL_DIR_BOTH)
*                                 * NONE (LAST_UTIL_GLOBAL_DIR)
*******************************************************************************/
typedef struct digi_serdes_port_s16_sys_cfg_t
{
    serdes_s16_adapt_objects_t          s16_sys_adapt_params[DIGI_SERDES_S16_NUM_RATES_MAX];
    digi_serdes_s16_tx_swing_levels_t   s16_sys_tx_swing_levels[DIGI_SERDES_S16_NUM_RATES_MAX];
    BOOL8                               dc_coupling_enabled;
    util_global_direction_t             polarity_inversed;
}digi_serdes_port_s16_sys_cfg_t;


/*******************************************************************************
* STRUCTURE: digi_serdes_port_t8_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure for defining T8 serdes configuration.
*
* ELEMENTS:
*  t8_adapt_params        - T8 adaptation parameters for each rate
*  t8_tx_swing_levels     - TX swing and de-emphasis levels per rate
*  dc_coupling_enabled    - Serdes coupling mode: DC when true, AC when FALSE
*  polarity_inversed      - Serdes inversed polarity in the folowing directions:
*                                 * RX (UTIL_GLOBAL_DIR_RX)
*                                 * TX (UTIL_GLOBAL_DIR_RX)
*                                 * BOTH (UTIL_GLOBAL_DIR_BOTH)
*                                 * NONE (LAST_UTIL_GLOBAL_DIR)
*******************************************************************************/
typedef struct digi_serdes_port_t8_cfg_t
{
    serdes_t8_adapt_objects_t          t8_adapt_params[DIGI_SERDES_T8_NUM_RATES_MAX];
    digi_serdes_t8_tx_swing_levels_t   t8_tx_swing_levels[DIGI_SERDES_T8_NUM_RATES_MAX];
    BOOL8                              dc_coupling_enabled;
    util_global_direction_t            polarity_inversed;
}digi_serdes_port_t8_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_serdes_port_c8_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure for defining  serdes configuration.
*
* ELEMENTS:
*  c8_adapt_params        -  adaptation parameters for each rate
*  c8_tx_swing_levels     -  TX swing and de-emphasis levels per rate
*  dc_coupling_enabled    -  Serdes coupling mode: DC when true, AC when FALSE
*  polarity_inversed      - Serdes inversed polarity in the folowing directions:
*                                 * RX (UTIL_GLOBAL_DIR_RX)
*                                 * TX (UTIL_GLOBAL_DIR_RX)
*                                 * BOTH (UTIL_GLOBAL_DIR_BOTH)
*                                 * NONE (LAST_UTIL_GLOBAL_DIR)
*******************************************************************************/
typedef struct digi_serdes_port_c8_cfg_t
{
    serdes_c8_adapt_objects_t          c8_adapt_params[DIGI_SERDES_C8_NUM_RATES_MAX];
    digi_serdes_c8_tx_swing_levels_t   c8_tx_swing_levels[DIGI_SERDES_C8_NUM_RATES_MAX];
    BOOL8                              dc_coupling_enabled;
    util_global_direction_t            polarity_inversed;
} digi_serdes_port_c8_cfg_t;


/*******************************************************************************
* STRUCTURE: digi_serdes_d8_tx_swing_level_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure for defining D8 serdes tx swing level configuration.
*  This structure will be valid only when either
*  "digi_serdes_ge_tx_swing_set" or "digi_serdes_sfi51_tx_swing_set" functions
*  were called for the specific SerDes lane.
*
* ELEMENTS:
*   valid            - parameter status.
*   port_mode        - serdes rate associated to parameter value.
*   tx_swing_level   - TX swing level
*******************************************************************************/
typedef struct digi_d8_tx_swing_level_cfg_t
{
    BOOL8                       valid;
    digi_serdes_port_mode_t     port_mode;
    d8_tx_2x_tx_swing_level_t   tx_swing_level;
}digi_serdes_d8_tx_swing_level_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_serdes_port_d8_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure for defining D8 serdes configuration.
*
* ELEMENTS:
*  d8_tx_swing_level      - D8 serdes tx swing level configuration
*
*******************************************************************************/
typedef struct digi_serdes_port_d8_cfg_t
{
    digi_serdes_d8_tx_swing_level_cfg_t         d8_tx_swing_level;
}digi_serdes_port_d8_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_serdes_port_sfi51_d8_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure for defining SFI51 D8 serdes configuration.
*
* ELEMENTS:
*   d8_cfg             - D8 serdes configuration
*  polarity_inversed   - Serdes inversed polarity in the folowing directions:
*                                 * RX (UTIL_GLOBAL_DIR_RX)
*                                 * TX (UTIL_GLOBAL_DIR_RX)
*                                 * BOTH (UTIL_GLOBAL_DIR_BOTH)
*                                 * NONE (LAST_UTIL_GLOBAL_DIR)
*******************************************************************************/
typedef struct digi_serdes_port_sfi51_d8_cfg_t
{
     digi_serdes_port_d8_cfg_t       d8_cfg[DIGI_SERDES_SFI51_D8_NUM_RATES_MAX];
    util_global_direction_t          polarity_inversed;
}digi_serdes_port_sfi51_d8_cfg_t;


/*******************************************************************************
* STRUCTURE: digi_serdes_port_sfi51_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure for defining SFI51 bus configuration.
*
* ELEMENTS:
*  sfi51_d8_cfg           - SFI51 bus D8 serdes configuration
*******************************************************************************/
typedef struct digi_serdes_port_sfi51_cfg_t
{
    digi_serdes_port_sfi51_d8_cfg_t    sfi51_d8_cfg[SFI51_D8_TX_MAX_NUM_SERDES];
}digi_serdes_port_sfi51_cfg_t;


/*******************************************************************************
* STRUCTURE: digi_serdes_port_ge_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure for defining supporting interface GE serdes configuration.
*
* ELEMENTS:
*   d8_cfg             - D8 serdes configuration
*  polarity_inversed   - Serdes inversed polarity in the folowing directions:
*                                 * RX (UTIL_GLOBAL_DIR_RX)
*                                 * TX (UTIL_GLOBAL_DIR_RX)
*                                 * BOTH (UTIL_GLOBAL_DIR_BOTH)
*                                 * NONE (LAST_UTIL_GLOBAL_DIR)
*******************************************************************************/
typedef struct digi_serdes_port_ge_cfg_t
{
    digi_serdes_port_d8_cfg_t    d8_cfg;
    util_global_direction_t      polarity_inversed;
}digi_serdes_port_ge_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_serdes_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure for defining configuration of S16 lINE and SYS, T8 and C8 serdes.
*
* ELEMENTS:
*   s16_line_cfg    - S16 line serdes configuration per lane.
*   s16_sys_cfg     - S16 system serdes configuration per lane.
*   t8_cfg          - T8 serdes configuration per lane.
*   c8_cfg          - C8 serdes configuration per lane.
*   sfi51_cfg       - SFI51 bus serdes configuration
*   ge_cfg          - GE interface serdes configuration
*******************************************************************************/
typedef struct digi_serdes_cfg_t 
{
    digi_serdes_port_s16_line_cfg_t  s16_line_cfg[DIGI_SERDES_XFI_LANES_MAX];
    digi_serdes_port_s16_sys_cfg_t   s16_sys_cfg[DIGI_SERDES_MAX_S16_IN_BP];
    digi_serdes_port_t8_cfg_t        t8_cfg[ DIGI_SERDES_MAX_T8_E1_IN_BP_LR];
    digi_serdes_port_c8_cfg_t        c8_cfg[DIGI_SERDES_MAX_C8_IN_BP_SR];
    digi_serdes_port_sfi51_cfg_t     sfi51_cfg[DIGI_NUM_SFI51_PORTS_MAX];
    digi_serdes_port_ge_cfg_t        ge_cfg[DIGI_NUM_GE_INTF];
} digi_serdes_cfg_t ;

/*******************************************************************************
* STRUCTURE: digi_ctxt_build_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure for defining saved context fields that may be returned by digi_ctxt_status_get function
*
* ELEMENTS:
*   build_version    - SW version
*   ctxt_size        - size of the saved context
*******************************************************************************/
typedef struct digi_ctxt_build_t
{
    UINT32 build_version;
    UINT32 ctxt_size;
} digi_ctxt_build_t;
 



/*******************************************************************************
* STRUCTURE: serdes_adapt_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   SERDES Adaptation Status
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct serdes_adapt_status_t
{
    hostmsg_adapt_intf_type_t adapt_intf_type;
    UINT32 cfg_lane_msk_19_0;
    UINT32 cfg_lane_msk_39_20;
    UINT32 success_lane_msk_19_0;
    UINT32 success_lane_msk_39_20;
    UINT32 fail_lane_msk_19_0;
    UINT32 fail_lane_msk_39_20;
} serdes_adapt_status_t;

/*******************************************************************************
* STRUCTURE: serdes_debug_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   SERDES debug variables
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct serdes_debug_t
{
    /* Record last lane, configuration step and return value for each SERDES */
    UINT16 c8_cur_lane;
    UINT16 c8_cur_step;
    INT32  c8_config_rv;
    UINT8  t8_cur_slice;
    UINT8  t8_cur_subch;
    UINT16 t8_cur_step;
    INT32  t8_config_rv;
    UINT16 s16_cur_chnl;
    UINT16 s16_cur_step;
    INT32  s16_config_rv;
    UINT8  sfi51_cur_chnl;
    UINT8  sfi51_cur_inst;
    UINT8  sfi51_cur_side;
    UINT8  sfi51_cur_step;
    INT32  sfi51_config_rv;
    UINT32 d8_cur_step;
    INT32  d8_config_rv;
} serdes_debug_t;



/*! Function pointer for aggregated interrupt table processing callback */
typedef PMC_ERROR (*digi_int_cb_fcn) (digi_handle_t     *digi_handle,
                                      void              *token,
                                      void              *digi_client_chnl_p,
                                      digi_int_type_t    int_type);

/*******************************************************************************
* STRUCTURE: digi_pgmrclk_div_cfg_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining configuration for one DIGI PGMRCLK divider.
*
* ELEMENTS:
*   div_id                    - divider ID between 0 and 3
*   clk_source                - divider clock source
*   clk_source_idx            - divider clock source index
*   input_clk_freq            - source clock frequency
*   output_clk_freq           - output clock desired frequency
*
* NOTE
*   For example to select source clock SFI5.1 TX[2] of clock divider 3:
*   div_id = 3, clk_source =  DIGI_PGMRCLK_SFI_1_TX and clk_source_idx = 2
*******************************************************************************/
typedef struct digi_pgmrclk_div_cfg_t
{
    digi_pgmrclk_div_id_t div_id;
    digi_pgmrclk_input_source_t clk_source;
    UINT32 clk_source_idx;
    DOUBLE input_clk_freq;
    DOUBLE output_clk_freq;
} digi_pgmrclk_div_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_pgmrclk_fpi_cfg_t
* ___________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining configuration for FPI Generator.
*
* ELEMENTS:
*   cfg_action       - Configuration action:
*                      -DIGI_PGMRCLK_FPI_CFG_ACTION_DISABLE_RESYNC     = 0
*                      -DIGI_PGMRCLK_FPI_CFG_ACTION_ENABLE_RESYNC      = 1
*                      -DIGI_PGMRCLK_FPI_CFG_ACTION_RESIZE_MASK_WINDOW = 2
*   misc_fpi_8_bit_0 - Maps to mask_622 input of FPI LCLK Generator 8,
*                      which adjusts the mask window sizes.
*                      Valid range: 0-1
*   mask_window_8    - The width of the mask window centered on the generated FPI pulse.
*                      Valid range: 0-0xF.
*
* NOTE:
*   Parameters 'misc_fpi_8_bit_0' and 'mask_window_8' are only valid
*   for the action DIGI_PGMRCLK_FPI_CFG_ACTION_RESIZE_MASK_WINDOW.
*   See PGMRCLK MISC_FPI_8[0] and MASK_WINDOW_8 register field descriptions
*   for reshaping window sizes derived from these parameters.
*
*******************************************************************************/
typedef struct digi_pgmrclk_fpi_cfg_t
{
    digi_pgmrclk_fpi_cfg_action_t   cfg_action;
    UINT8                           misc_fpi_8_bit_0;
    UINT8                           mask_window_8;
} digi_pgmrclk_fpi_cfg_t;

/*******************************************************************************
* STRUCTURE:  digi_s16_clk_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining S16 clock properties.
*
* ELEMENTS:
*   pin_index    - The pin index on which a clock is generated. Valid values are values within
*                  the TX module lanes specified for digi_serdes_port_init function
*   divider      - A division factor applied on the clock.
*
*******************************************************************************/
typedef struct digi_s16_clk_cfg_t  
{
    UINT8 pin_index;
    digi_s16_clk_divider_t divider;
} digi_s16_clk_cfg_t;


/*******************************************************************************
* STRUCTURE: digi_client2trunk_port_action_table_cmf_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Client to trunk port action table parameters. This table will hold the parameters
*   to be to be used in conjunction with the consequential action table.
*   Each consequential action table will have an associated port action
*   table.
*
* ELEMENTS:
*   enable_monitoring - 1 ~ Indicating defect monitoring should be enabled for the defects
*                       specifed in the consequential action table. \n
*                       0 ~ The defects specifed in the consequential action table (digi_client2trunk_defect_action_t)
*                       will not be monitored. \n
*   is_send_dci - 1 ~ Send a DCI when a consequential action is deasserted \n
*                 0 ~ Do not send a DCI when a consequential action is deasserted \n
*   is_gen_keep_alive_cmf - 1 ~ Generate a keep alive CMF every 100ms on this mapotn channel \n
*                           0 ~ Do not generate a keep alive CMF \n
*   upi_for_keep_alive_cmf - 0 ~ 255 ~ value of UPI in CMF to be inserted in both keep alive CMF
*                            and consequential action CMF if applicable
*   is_gen_payload_fcs - 1 ~ Generate FCS for the both keep alive CMF and consequential action CMF if applicable \n
*                        0 ~ Do not generate FCS \n
*   is_use_payload_csf - 1 ~ Insert payload to both keep alive CMF and consequential action CMF if applicable \n
*                        0 ~ Do not use payload CSF \n
*   cmf_payload - The payload of the inserted CMF
*
*******************************************************************************/
typedef struct digi_client2trunk_port_action_table_cmf_t
{
    UINT32 enable_monitoring;
    UINT32 is_send_dci;
    UINT32 is_gen_keep_alive_cmf;
    UINT32 upi_for_keep_alive_cmf;
    UINT32 is_gen_payload_fcs;
    UINT32 is_use_payload_csf;
    UINT8 cmf_payload[8];
} digi_client2trunk_port_action_table_cmf_t;

/***************************************************************************
* STRUCTURE: digi_client2trunk_defect_action_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   Client to trunk defect action table. This will give the user the ability
*   to specify the actions associated with each defect and the priority of the
*   given defect. The user may also disable the defect to not monitor the
*   defect altogether - doing so will not allow the actions to be executed
*   since the defects will never be asserted.
*
* ELEMENTS:
*   monitor_defect - enable this defect to be monitored
*   action1 - action1 is the first action to take upon this defect being active
*   action2 - action2 is the second action to take upon this defect being active
*   priority_idx - priority index of this defect. 1 is highest priority, the number of
*                  defects (7) is the lowest priority. Do Not duplicate priorities.
****************************************************************************/
typedef struct digi_client2trunk_defect_action_t
{
    BOOL monitor_defect;
    digi_client2trunk_actions_t action1;
    digi_client2trunk_actions_t action2;
    UINT8 priority_idx;
} digi_client2trunk_defect_action_t;

/***************************************************************************
* STRUCTURE: digi_client2trunk_conseq_action_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   The defects each have their own structures for actions and priorites.
*   Each defect available to be monitored will be listed here.
*
* ELEMENTS:
*   enet_emac_phy_los_mon - structure that holds ENET Link Down (PHY LOS) defect actions and priorities\n
*   enet_emac_block_lock_mon - structure that holds ENET PCS loss of block sync defect actions and priorities\n
*   enet_emac_hi_ber_mon - structure that holds ENET Excessive error ratio (HiBER) defect actions and priorities\n
*   enet_emac_rx_loc_fault_mon - structure that holds ENET ingress LF defect actions and priorities\n
*   enet_emac_rx_rem_fault_mon - structure that holds ENET ingress RF defect actions and priorities\n
*   cbrc_8b10b_loss_sync_mon - structure that holds CBRC FC ILVR FC800 RX_SYNC defect actions and priorities\n
*   fc1200_loss_sync_mon - structure that holds CBRC FC1200 LOSS_SYNC defect actions and priorities\n
*   sonet_loss_signal_mon - structure that holds SONET LOS_S defect actions and priorities\n
*   sonet_loss_frame_mon - structure that holds SONET LOS_F defect actions and priorities\n
*   cbrc_8b10b_loss_signal_mon - structure that holds CBRC 8B10B LOS defect actions and priorities\n
*                                This can also be used for the SONET client Port\n
*   fc1200_loss_signal_mon - structure that holds fc1200 loss of signal actions and priorities\n
*   enet_epmm_loss_signal_mon - structure that holds enet epmm Gsup43 loss of signal actions and priorities\n
*   enet_epmm_local_fault_mon - structure that holds enet epmm Gsup43 local fault actions and priorities\n
*   enet_epmm_remote_fault_mon - structure that holds enet epmm Gsup43 remote fault actions and priorities\n
*   sw_force_manual_trigger_mon - structure that holds sw force manual trigger actions and priorities\n
*   enet_mac_40g_100g_loam_mon - structure that holds enet mac 40/100G LOAM actions and priorities\n
****************************************************************************/
typedef struct digi_client2trunk_conseq_action_t
{
    digi_client2trunk_defect_action_t enet_emac_phy_los_mon;
    digi_client2trunk_defect_action_t enet_emac_block_lock_mon;
    digi_client2trunk_defect_action_t enet_emac_hi_ber_mon;
    digi_client2trunk_defect_action_t enet_emac_rx_loc_fault_mon;
    digi_client2trunk_defect_action_t enet_emac_rx_rem_fault_mon;
    digi_client2trunk_defect_action_t cbrc_8b10b_loss_sync_mon;
    digi_client2trunk_defect_action_t fc1200_loss_sync_mon;
    digi_client2trunk_defect_action_t sonet_loss_signal_mon;
    digi_client2trunk_defect_action_t sonet_loss_frame_mon;
    digi_client2trunk_defect_action_t cbrc_8b10b_loss_signal_mon;
    digi_client2trunk_defect_action_t fc1200_loss_signal_mon;
    digi_client2trunk_defect_action_t enet_epmm_loss_signal_mon;
    digi_client2trunk_defect_action_t enet_epmm_local_fault_mon;
    digi_client2trunk_defect_action_t enet_epmm_remote_fault_mon;
    digi_client2trunk_defect_action_t sw_force_manual_trigger_mon;
    digi_client2trunk_defect_action_t enet_mac_40g_100g_loam_mon;

} digi_client2trunk_conseq_action_t;

/*******************************************************************************
* STRUCTURE: digi_client2trunk_tables_cmf_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Client to trunk tables - port action table parameters and the consequential action parameters.
*
* ELEMENTS:
*  port_action_cmf  - port action table structure\n
*  conseq_action_cmf - consequential action table structure\n
*******************************************************************************/
typedef struct digi_client2trunk_tables_cmf_t
{
    digi_client2trunk_port_action_table_cmf_t port_action_cmf;
    digi_client2trunk_conseq_action_t conseq_action_cmf;
} digi_client2trunk_tables_cmf_t;

/***************************************************************************
* STRUCTURE: cmf_trunk2client_defect_action_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   Trunk to client defect action table. This will be used within the
*   trunk to client Consequential action tables. This will give the user the ability
*   to specify the action associated with each defect and the priority of the
*   given defect. The user may also disable the defect to not monitor the
*   defect altogether - doing so will not allow the actions to be executed
*   since the defects will never be asserted.
*
* ELEMENTS:
*   monitor_defect - enable this defect to be monitored\n
*   action1 - action1 is the action to take upon this defect being active\n
*   action2 - action2 is the second action to take upon this defect being active.\n
*             Note that this action is only valid if second_action_holdoff_time is set.\n
*   second_action_holdoff_time - time in ms to wait before turning off action1 and turning\n
*                                on action2. This must be in multiples of 100ms (0,100,200..1000 ms).\n
*   priority_idx - priority index of this defect. 1 is highest priority, the number of\n
*                  defects (10) is the lowest priority. Do Not duplicate priorities.
*
*
****************************************************************************/
typedef struct cmf_trunk2client_defect_action_t
{
    BOOL monitor_defect;
    digi_trunk2client_actions_t action1;
    digi_trunk2client_actions_t action2;
    UINT16 second_action_holdoff_time;
    UINT8 priority_idx;
} cmf_trunk2client_defect_action_t;

/***************************************************************************
* STRUCTURE: cmf_trunk2client_conseqAction_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   The defects each have their own structures for actions and priorites.
*   Each defect available to be monitored will be listed here.
*
* ELEMENTS:
*   ssf_sf_rcvd_mon     - structure that holds ssf_sf_rcvd_mon defect actions and priorities
*   dplm_mon    - structure that holds dplm_mon defect actions and priorities
*   opu_csf_mon - structure that holds opu_csf_mon defect actions and priorities
*   gfp_lofd_mon    - structure that holds gfp_lofd_mon defect actions and priorities
*   gfp_pti_upi_mismatch_mon - structure that holds gfp_pti_upi_mismatch_mon defect actions and priorities
*   gfp_cmf_csf_loss_client_signal_mon - structure that holds gfp_cmf_csf_loss_client_signal_mon defect actions and priorities
*   gfp_cmf_csf_loss_char_sync_mon - structure that holds gfp_cmf_csf_loss_char_sync_mon defect actions and priorities
*   gfp_cmf_fdi_mon - structure that holds gfp_cmf_fdi_mon defect actions and priorities
*   gfp_cmf_rdi_mon - structure that holds gfp_cmf_rdi_mon defect actions and priorities
*   sonet_sfis_lof_mon - structure that holds sonet_sfis_los_mon defect actions and priorities
*   gfp_dexm_mon - structure that holds gfp_dexm_mon defect actions and priorities
****************************************************************************/
typedef struct cmf_trunk2client_conseqAction_t
{
    cmf_trunk2client_defect_action_t ssf_sf_rcvd_mon;
    cmf_trunk2client_defect_action_t dplm_mon;
    cmf_trunk2client_defect_action_t opu_csf_mon;
    cmf_trunk2client_defect_action_t gfp_lofd_mon;
    cmf_trunk2client_defect_action_t gfp_pti_upi_mismatch_mon;
    cmf_trunk2client_defect_action_t gfp_cmf_csf_loss_client_signal_mon;
    cmf_trunk2client_defect_action_t gfp_cmf_csf_loss_char_sync_mon;
    cmf_trunk2client_defect_action_t gfp_cmf_fdi_mon;
    cmf_trunk2client_defect_action_t gfp_cmf_rdi_mon;
    cmf_trunk2client_defect_action_t sonet_sfis_lof_mon;
    cmf_trunk2client_defect_action_t gfp_dexm_mon;
    cmf_trunk2client_defect_action_t emac_lf_and_phy_los_mon;

} cmf_trunk2client_conseqAction_t;

/***************************************************************************
* STRUCTURE: cmf_trunk2client_portMapping_t
* __________________________________________________________________________
*
* DESCRIPTION:
*   Trunk to client port mapping table parameters. This table will hold the parameters
*   to be to be used in conjunction with the consequential action table.
*   Each consequential action table will have an associated port mapping
*   table.
*
* ELEMENTS:
*    opu_csf_persistance_time   - variable to set for how long the opu_csf must be
*                               persistant before it can be be considered a defect
*                               (3 frames) (ODU4 ~ 900us (~1ms), ODU0 ~ 76ms).
*                               This is given in increments of 1ms (Minimum 1ms).
****************************************************************************/
typedef struct cmf_trunk2client_portMapping_t
{
    UINT8 opu_csf_persistance_time;
} cmf_trunk2client_portMapping_t;

/*******************************************************************************
* STRUCTURE: digi_trunk2client_tables_cmf_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Trunk to client tables - port mapping table parameters and the consequential action parameters.
*
* ELEMENTS:
*  port_mapping_cmf  - port action table structure
*  conseq_action_cmf - consequential action table structure
*******************************************************************************/
typedef struct digi_trunk2client_tables_cmf_t
{
    cmf_trunk2client_portMapping_t port_mapping_cmf;
    cmf_trunk2client_conseqAction_t conseq_action_cmf;
} digi_trunk2client_tables_cmf_t;

/*******************************************************************************
* STRUCTURE: digi_cmf_extract_cmf_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMF extract parameters retrieved by firmware.
*
* ELEMENTS:
*  mapton_ch   - mapotn channel where CMF was extracted
*  cmf_length - length of extracted CMF in bytes
*  retrieved_cmf_valid - Indicator of whether last retrieved CMF from internal FW
*                        memory is valid or has been cleared at time of retrieval.
*                        Indicates whether the following conditions have been met:
*                         1) no CSF/FDI (RDI) CMF frame is received in N*1000 ms (a value of 3 is suggested for N); or
*                         2) upon receiving a valid GFP client data frame; or
*                         3) upon receiving a DCI.
*                        0 : This CMF is no longer valid
*                        1 : This CMF is valid
*                        NOTE: you must set up monitoring for these defects in the trunk
*                        to client consequential action tables to obtain this information.
*                        This is only valid if you retrieve CMF from internal memory.
*  cmf_typeheader - The type header of the extracted CMF
*  cmf_payload - The payload of the extracted CMF
*******************************************************************************/
typedef struct digi_cmf_extract_cmf_t
{
    UINT8 mapotn_ch;
    UINT8 cmf_length;
    UINT8 retrieved_cmf_valid;
    UINT8 cmf_typeheader[4];
    UINT8 cmf_payload[8];
} digi_cmf_extract_cmf_t;

/*******************************************************************************
* STRUCTURE: digi_cmf_one_shot_cmf_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CMF insert one shot CMF parameters.
*
* ELEMENTS:
*  cmf_payload - The payload of the inserted CMF
*******************************************************************************/
typedef struct digi_cmf_one_shot_cmf_t
{
    UINT8 cmf_payload[8];
} digi_cmf_one_shot_cmf_t;

/*******************************************************************************
* STRUCTURE: digi_cmf_upi_values_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Alternate values (non-standard) for the CMF frames being inserted by FW.
*
* ELEMENTS:
*  los_upi - loss of client signal upi value (max 0xFF)     
*  loss_sync_upi - loss of client character synchronization upi value (max 0xFF)
*  dci_upi - defect clear indication upi value (max 0xFF)     
*  fdi_upi - forward defect indication upi value (max 0xFF)
*  rdi_upi - reverse defect indication upi value (max 0xFF)     
* 
*******************************************************************************/
typedef struct digi_cmf_upi_values_t
{
    UINT32 los_upi;
    UINT32 loss_sync_upi;
    UINT32 dci_upi;
    UINT32 fdi_upi;
    UINT32 rdi_upi;

} digi_cmf_upi_values_t;

#include "digi_pmon_api.h"
#include "digi_cbr_api.h"
#include "digi_enet_api.h"
#include "digi_otn_api.h"
#include "digi_map_api.h"
#include "digi_sifd_api.h"
#include "digi_resource_api.h"
#include "digi_ghao_api.h"
#include "digi_timesync_api.h"

/*******************************************************************************
* STRUCTURE: digi_dcsu_int_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DIGI DCSU table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct digi_dcsu_int_t
{
    /* Interrupts */
    BOOL8 err_fast_i;
    BOOL8 err_slow_i;
    BOOL8 lock_i;

    /* Status */
    BOOL8 err_fast_v;
    BOOL8 err_slow_v;
    BOOL8 lock_v;
} digi_dcsu_int_t;

/*******************************************************************************
* STRUCTURE: digi_device_int_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DIGI device level table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct digi_device_int_t
{
    s16_int_pin_t               s16_line[DIGI_SERDES_XFI_LANES_MAX];
    s16_int_pin_t               s16_sys[S16_MAX_NUM_SERDES];

    mgmt_fege_int_t             mgmt_fege;

    digi_dcsu_int_t             dcsu[DIGI_DCSU_MAX_INST];
} digi_device_int_t;

/*******************************************************************************
* STRUCTURE: digi_int_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DIGI level aggregation table for interrupt and status.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct digi_int_t 
{
    digi_device_int_t   device;
    digi_otn_int_t      otn;
    digi_mapper_int_t   mapper;
    digi_cbr_int_t      cbr;
    digi_pmon_int_t     pmon;
    digi_enet_int_t     enet_line;
    digi_enet_int_t     enet_sys;
    digi_sifd_int_t     sifd;
} digi_int_t;
/*******************************************************************************
* ENUM: digi_int_irq_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration to select subsystems that will propagate interrupts outside
*   the device.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum digi_int_irq_mask_t
{
    DIGI_INT_IRQ_MASK_PMON          = (0x1<<12),
    DIGI_INT_IRQ_MASK_TOP_SYSOTN    = (0x1<<9),
    DIGI_INT_IRQ_MASK_MCPB          = (0x1<<8),
    DIGI_INT_IRQ_MASK_DCPB          = (0x1<<7),
    DIGI_INT_IRQ_MASK_SIFD          = (0x1<<6),
    DIGI_INT_IRQ_MASK_CBRC          = (0x1<<5),
    DIGI_INT_IRQ_MASK_ENET_SYS      = (0x1<<4),
    DIGI_INT_IRQ_MASK_ENET_LINE     = (0x1<<3),
    DIGI_INT_IRQ_MASK_MAPOTN        = (0x1<<2),
    DIGI_INT_IRQ_MASK_LINEOTN       = (0x1<<1),
    DIGI_INT_IRQ_MASK_COREOTN       = (0x1<<0),
} digi_int_irq_mask_t;

/*******************************************************************************
* STRUCTURE: digi_event_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DIGI Event configuration structure - same as pmc_sys_event_cfg_t.
*******************************************************************************/
typedef pmc_sys_event_cfg_t digi_event_cfg_t;


#ifndef DOXYGEN_PUBLIC_ONLY
/*! generic function that takes one void * argument */
typedef PMC_ERROR (*digi_fw_log_prov_deprov_callback_t)(void*);

/*******************************************************************************
* STRUCTURE: digi_fw_log_prov_deprov_stats_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   structure for storing provision/deprovision benchmark statistics.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    digi_handle_t *digi_handle;
    char name[100];

    pmc_sys_stat_t prov_stats[PROV_DEPROV_NUM];
    pmc_sys_stat_t deprov_stats[PROV_DEPROV_NUM];
    
    l5sys_queue_message_t *provision_messages;
    l5sys_queue_message_t *deprovision_messages;
    digi_fw_log_prov_deprov_callback_t prov_function;
    digi_fw_log_prov_deprov_callback_t deprov_function;
    void *test_parameters;

    PMC_ERROR result;
} digi_fw_log_prov_deprov_stats_t;
#endif /* DOXYGEN_PUBLIC_ONLY */
/*
** Global variables
*/

/*
** Function Prototypes
*/
#ifndef DOXYGEN_PUBLIC_ONLY
PUBLIC digi_handle_t *digi_ctxt_create(pmc_handle_t *parent,
                                       UINT32 base_address,
                                       pmc_sys_handle_t *sys_handle,
                                       const char *tsb_name);

PUBLIC digi_handle_t *digi_ctxt_restart_create(void *data_ptr,
                                               pmc_sys_handle_t *sys_handle);

PUBLIC void digi_ctxt_destroy(digi_handle_t **digi_handle);





PUBLIC PMC_ERROR digi_ctxt_status_get(void  *ctxt_ptr, digi_ctxt_build_t *ctxt_data_ptr);


PUBLIC PMC_ERROR digi_handle_init(digi_handle_t *digi_handle,
                             void *warm_restart_ctxt);

PUBLIC void digi_handle_init_uses_fw_q(BOOL8 initialize_fw_q );

PUBLIC PMC_ERROR digi_handle_restart_init(digi_handle_t *digi_handle,
                                          BOOL8 is_warm_restart);

PUBLIC PMC_ERROR digi_ctxt_v3_xx_to_v3_yy_migrate(void *final_ptr,
                                                  void *original_ptr,
                                                  void *temp_ptr);

PUBLIC PMC_ERROR digi_ctxt_v3_05_to_v3_06_migrate(void *final_ptr,
                                                  void *original_ptr,
                                                  void *temp_ptr);
                          
PUBLIC PMC_ERROR digi_ctxt_v3_06_to_v3_07_migrate(void *final_ptr,
                                                  void *original_ptr,
                                                  void *temp_ptr);

PUBLIC PMC_ERROR digi_ctxt_v3_07_to_v3_08_migrate(void *final_ptr,
                                                  void *original_ptr,
                                                  void *temp_ptr);

PUBLIC PMC_ERROR digi_ctxt_v3_08_to_v3_09_migrate(void *final_ptr,
                                                  void *original_ptr,
                                                  void *temp_ptr);
PUBLIC PMC_ERROR digi_ctxt_v3_09_to_v3_10_migrate(void *final_ptr,
                                                  void *original_ptr,
                                                  void *temp_ptr);
PUBLIC PMC_ERROR digi_ctxt_v3_10_to_v3_11_migrate(void *final_ptr,
                                                  void *original_ptr,
                                                  void *temp_ptr);
PUBLIC PMC_ERROR digi_ctxt_v3_11_to_v3_12_migrate(void *final_ptr,
                                                  void *original_ptr,
                                                  void *temp_ptr);

PUBLIC UINT32 digi_ctxt_v3_xx_size_get();
PUBLIC UINT32 digi_ctxt_v3_yy_size_get();
PUBLIC UINT32 digi_ctxt_v3_05_size_get();
PUBLIC UINT32 digi_ctxt_v3_06_size_get();
PUBLIC UINT32 digi_ctxt_v3_07_size_get();
PUBLIC UINT32 digi_ctxt_v3_08_size_get();
PUBLIC UINT32 digi_ctxt_v3_09_size_get();
PUBLIC UINT32 digi_ctxt_v3_10_size_get();
PUBLIC UINT32 digi_ctxt_v3_11_size_get();
PUBLIC UINT32 digi_ctxt_v3_12_size_get();


PUBLIC PMC_ERROR digi_atomic_keep_lock(digi_handle_t *digi_handle);
PUBLIC void digi_atomic_end(digi_handle_t *digi_handle);
PUBLIC void digi_device_shutdown(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_software_status_get(digi_handle_t *digi_handle,
                                          digi_sw_status_t *status_ptr);
PUBLIC PMC_ERROR digi_firmware_status_get(digi_handle_t *digi_handle,
                                          digi_fw_status_t *status_ptr);
PUBLIC PMC_ERROR digi_warm_restart_context_get(digi_handle_t *digi_handle,
                                               UINT32 *size_ptr,
                                               void **backup_ram_pptr);
PUBLIC PMC_ERROR digi_fw_spram_sw_reserved_addr_get(digi_handle_t *digi_handle,
                                                    UINT32 *sw_rsvd_addr_ptr,
                                                    UINT32 *sw_rsvd_size_ptr);
/* data path establishment APIs */
PUBLIC void digi_mapotn_etrans_scheduling_mode_set(digi_handle_t *digi_handle,
                                                   BOOL8 schd_mode);
PUBLIC PMC_ERROR digi_device_init(digi_handle_t *digi_handle,
                                  digi_sifd_ilkn_1_cfg_t *sifd_1_cfg_ptr,
                                  digi_sifd_ilkn_2_cfg_t *sifd_2_cfg_ptr,
                                  digi_prod_app_t prod_app,
                                  digi_otn_mux_stages_t mux_stages,
                                  digi_ilkn_link_t **ilkn_1_ctxt_pptr,
                                  digi_ilkn_link_t **ilkn_2_ctxt_pptr);
                                  
PUBLIC PMC_ERROR digi_mpmo_csi_cfg(digi_handle_t *digi_handle,
                                   digi_opsa_inst_t opsa_inst,
                                   BOOL csi_squelch_en);
                                   
PUBLIC PMC_ERROR digi_mpma_csi_cfg(digi_handle_t *digi_handle,
                                   digi_opsa_inst_t opsa_inst,
                                   digi_opsa_csi_type_t cfg_csi_type,
                                   UINT32 csi_val); 
                                                                                                       
PUBLIC PMC_ERROR digi_serdes_cage_cfg(digi_handle_t *digi_handle,
                                      digi_serdes_intf_t intf,
                                      UINT8 cage_id_per_pin[DIGI_SERDES_XFI_LANES_MAX]);
PUBLIC PMC_ERROR digi_serdes_port_init(digi_handle_t *digi_handle,
                                       UINT32 identifier,
                                       digi_serdes_intf_t intf,
                                       UINT32 module_lane_tx[DIGI_SERDES_XFI_LANES_MAX],
                                       UINT32 module_lane_rx[DIGI_SERDES_XFI_LANES_MAX],
                                       UINT8 los_pin,
                                       BOOL los_inv,
                                       digi_s16_clk_cfg_t *s16_clk_cfg_ptr,
                                       digi_serdes_port_t **port_ctxt_pptr);
PUBLIC PMC_ERROR digi_serdes_ge_port_prov(digi_handle_t *digi_handle,
                                          digi_ge_serdes_id_t ge_intf);
PUBLIC PMC_ERROR digi_serdes_ge_port_deprov(digi_handle_t *digi_handle,
                                            digi_ge_serdes_id_t ge_intf);
PUBLIC PMC_ERROR digi_otn_server_line_prov(digi_handle_t *digi_handle,
                                           digi_serdes_port_t *port_ctxt_ptr,
                                           digi_otn_server_otuk_t otuk_mode,
                                           digi_otn_server_fec_t fec_mode,
                                           digi_sfis_deskew_type_t deskew_type,
                                           digi_otn_server_chnl_t **chnl_ctxt_pptr);
PUBLIC PMC_ERROR digi_otn_server_line_deprov( digi_handle_t *digi_handle,
                                              digi_otn_server_chnl_t **otn_server_pptr );
PUBLIC PMC_ERROR digi_otn_server_sys_deprov(digi_handle_t *digi_handle,
                                            digi_otn_server_chnl_t **otn_server_pptr);

PUBLIC PMC_ERROR digi_otn_odu_chnl_prov_mode_set(digi_handle_t *digi_handle,
                                                 digi_otn_odu_chnl_prov_mode_t prov_mode);
PUBLIC PMC_ERROR digi_otn_server_lo_prov_num_chnl_set(digi_handle_t *digi_handle,
                                                      digi_otn_odu_chnl_t *in_chnl_ctxt_ptr,
                                                      UINT8 num_mo_chnls,
                                                      UINT8 num_lo_chnls);
PUBLIC PMC_ERROR digi_otn_server_lo_prov_end( digi_handle_t *digi_handle,
                                              digi_otn_odu_chnl_t *in_chnl_ctxt_ptr);
PUBLIC PMC_ERROR digi_otn_server_lo_deprov_end( digi_handle_t *digi_handle,
                                                digi_otn_odu_chnl_t **ho_odu_chnl_ctxt_pptr);


PUBLIC PMC_ERROR digi_otn_server_sys_prov(digi_handle_t *digi_handle,
                                          digi_serdes_port_t *port_ctxt_ptr,
                                          digi_otn_server_fec_t fec_mode,
                                          digi_sfis_deskew_type_t deskew_type,
                                          digi_otn_server_chnl_t **chnl_ctxt_pptr);


PUBLIC PMC_ERROR digi_otn_server_ho_prov(digi_handle_t *digi_handle,
                                         digi_otn_server_chnl_t *in_chnl_ctxt_ptr,
                                         digi_otn_cfg_mode_t cfg_mode,
                                         digi_odu_line_payload_t payload_mode,
                                         digi_otn_odu_chnl_t **out_chnl_ctxt_pptr);

PUBLIC PMC_ERROR digi_otn_server_lo_prov( digi_handle_t *digi_handle,
                                          digi_otn_odu_chnl_t *in_chnl_ctxt_ptr,
                                          digi_otn_cfg_mode_t cfg_mode,
                                          digi_oduk_t oduk_type,
                                          DOUBLE client_bitrate,
                                          digi_otn_mapping_type_t mapping_mode,
                                          UINT32 trib_slot_mask[3],
                                          digi_odu_line_payload_t payload_mode,
                                          digi_otn_odu_chnl_t **out_chnl_ctxt_pptr);

PUBLIC PMC_ERROR digi_otn_server_lo_prov_asym( digi_handle_t *digi_handle,
                                               digi_otn_odu_chnl_t *in_chnl_ctxt_ptr,
                                               digi_otn_cfg_mode_t cfg_mode,
                                               digi_oduk_t oduk_type,
                                               DOUBLE client_bitrate,
                                               digi_otn_mapping_type_t mapping_mode,
                                               UINT32 tx_trib_slot_mask[3],
                                               UINT32 rx_trib_slot_mask[3],
                                               digi_odu_line_payload_t payload_mode,
                                               digi_otn_odu_chnl_t **out_chnl_ctxt_pptr);
PUBLIC PMC_ERROR digi_otn_server_lo_deprov(digi_handle_t *digi_handle,
                                           digi_otn_odu_chnl_t **lo_odu_chnl_ctxt_pptr,
                                           digi_otn_odu_chnl_t **ho_odu_chnl_ctxt_pptr);
PUBLIC PMC_ERROR digi_otn_server_lo_deprov_asym(digi_handle_t *digi_handle,
                                                digi_otn_odu_chnl_t **lo_odu_chnl_ctxt_pptr,
                                                digi_otn_odu_chnl_t **ho_odu_chnl_ctxt_pptr,
                                                digi_otn_asym_deprov_t deprov_flavour);
PUBLIC PMC_ERROR digi_otn_server_ho_deprov(digi_handle_t *digi_handle,
                                           digi_otn_odu_chnl_t  **odu_chnl_ctxt_pptr,
                                           digi_otn_server_chnl_t **otn_server_pptr);
PUBLIC PMC_ERROR digi_mapper_group_usage_get(digi_handle_t *digi_handle,
                                             digi_port_usage_grp_struct_t* port_grp_usage);
PUBLIC PMC_ERROR digi_line_port_info_get(digi_handle_t *digi_handle,
                                         digi_port_usage_grp_struct_t* port_grp_usage);
PUBLIC PMC_ERROR digi_system_port_info_get(digi_handle_t *digi_handle,
                                           digi_port_usage_grp_struct_t* port_grp_usage);

PUBLIC PMC_ERROR digi_port_is_initialized(digi_serdes_port_t *port_ctxt_ptr,
                                          BOOL* is_init);

PUBLIC PMC_ERROR digi_port_uid_get(digi_serdes_port_t *port_ctxt_ptr,
                                   UINT32* port_uid);

PUBLIC PMC_ERROR digi_port_interface_type_get(digi_serdes_port_t *port_ctxt_ptr,
                                              digi_serdes_intf_t* interface_type);
PUBLIC PMC_ERROR digi_enhanced_pmon_link_get(digi_handle_t *digi_handle, 
                                           digi_serdes_port_t *port_ctxt_ptr,
                                           digi_mapper_primary_group_t *primary_group_ptr,
                                           digi_mapper_secondary_group_t *secondary_group_ptr);
PUBLIC PMC_ERROR digi_otn_odu_chnl_prov_mode_get(digi_handle_t *digi_handle,
                                                digi_otn_odu_chnl_prov_mode_t *prov_mode);
/*
 * ASK related functions as defined in Crash Restart
 */
PUBLIC PMC_ERROR digi_otn_server_prov_chnl_get(digi_handle_t *digi_handle,
                                               digi_serdes_port_t *port_ctxt_ptr,
                                               digi_otn_server_chnl_t **chnl_ctxt_pptr);

PUBLIC PMC_ERROR digi_otn_server_ho_prov_chnl_get(digi_handle_t *digi_handle,
                                                  digi_otn_server_chnl_t *in_chnl_ctxt_ptr,
                                                  digi_otn_odu_chnl_t **out_chnl_ctxt_pptr);

PUBLIC PMC_ERROR digi_otn_server_lo_prov_chnl_get(digi_handle_t *digi_handle,
                                                  digi_otn_odu_chnl_t *in_chnl_ctxt_ptr,
                                                  digi_otn_cfg_mode_t cfg_mode,
                                                  UINT32 trib_slot_mask[3],
                                                  digi_otn_odu_chnl_t **out_chnl_ctxt_pptr);

PUBLIC PMC_ERROR digi_enet_prov_chnl_get(digi_handle_t *digi_handle,
                                         digi_serdes_port_t *port_ctxt_ptr,
                                         digi_enet_client_chnl_t **chnl_ctxt_pptr);

PUBLIC PMC_ERROR digi_cbr_client_prov_chnl_get(digi_handle_t *digi_handle,
                                               digi_serdes_port_t *port_ctxt_ptr,
                                               digi_enet_client_chnl_t **chnl_ctxt_pptr);


PUBLIC PMC_ERROR digi_util_otn_mapper_prov(digi_handle_t *digi_handle,
                                           digi_mapper_primary_group_t primary_group,
                                           digi_mapper_secondary_group_t secondary_group,
                                           digi_mapping_mode_t mode,
                                           BOOL enet_pmon,
                                           digi_oduk_t odu_data,
                                           UINT32 odu_flex_data,
                                           UINT32 pv_dci_chnl_force,
                                           digi_mapper_chnl_t **chnl_ctxt_pptr);

PUBLIC PMC_ERROR digi_otn_mapper_prov(digi_handle_t *digi_handle,
                                      digi_mapper_primary_group_t primary_group,
                                      digi_mapper_secondary_group_t secondary_group,
                                      digi_mapping_mode_t mode,
                                      BOOL enet_pmon,
                                      digi_oduk_t odu_data,
                                      UINT32 odu_flex_data,
                                      digi_mapper_chnl_t **chnl_ctxt_pptr);

PUBLIC PMC_ERROR digi_otn_mapper_deprov(digi_handle_t *digi_handle,
                                        digi_mapper_chnl_t **chnl_ctxt_pptr);
PUBLIC PMC_ERROR digi_enet_line_prov(digi_handle_t *digi_handle,
                                     digi_serdes_port_t *port_ctxt_ptr,
                                     digi_enet_mode_t mode,
                                     digi_enet_client_chnl_t **chnl_ctxt_pptr);
PUBLIC PMC_ERROR digi_enet_line_mirror_prov(digi_handle_t *digi_handle,
                                     digi_serdes_port_t *port_ctxt_ptr,
                                     digi_enet_mode_t mode,
                                     digi_enet_client_chnl_t **chnl_ctxt_pptr);
PUBLIC PMC_ERROR digi_enet_sys_prov(digi_handle_t *digi_handle,
                                    digi_serdes_port_t *port_ctxt_ptr,
                                    digi_enet_mode_t mode,
                                    digi_enet_client_chnl_t **chnl_ctxt_pptr);
PUBLIC PMC_ERROR digi_enet_line_deprov(digi_handle_t *digi_handle,
                                       digi_enet_client_chnl_t **chnl_ctxt_pptr);
PUBLIC PMC_ERROR digi_enet_sys_deprov(digi_handle_t *digi_handle,
                                      digi_enet_client_chnl_t **chnl_ctxt_pptr);
PUBLIC PMC_ERROR digi_cbr_client_prov(digi_handle_t *digi_handle,
                                      digi_serdes_port_t *port_ctxt_ptr,
                                      digi_cbr_client_mode_t mode,
                                      digi_cbr_client_chnl_t **chnl_ctxt_pptr);
PUBLIC PMC_ERROR digi_cbr_client_deprov(digi_handle_t *digi_handle,
                                        digi_cbr_client_chnl_t **chnl_ctxt_pptr);

PUBLIC PMC_ERROR digi_ilkn_cpb_chnl_prov(digi_handle_t *digi_handle,
                                         digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                         UINT8 rate,
                                         BOOL enet_pmon,
                                         BOOL tx_type_header,
                                         BOOL rx_type_header,
                                         BOOL rx_fc_reach_extend,
                                         digi_sifd_ilkn_calendar32_t *tx_ilkn_fc_ptr,
                                         digi_sifd_ilkn_calendar32_t *rx_ilkn_fc_ptr);

PUBLIC PMC_ERROR digi_ilkn_cpb_rx_chnl_prov(digi_handle_t *digi_handle,
                                            digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                            BOOL enet_pmon,
                                            BOOL rx_type_header,
                                            digi_sifd_ilkn_calendar32_t *rx_ilkn_fc_ptr);

PUBLIC PMC_ERROR digi_ilkn_cpb_tx_chnl_prov(digi_handle_t *digi_handle,
                                            digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                            UINT8 rate,
                                            BOOL tx_type_header,
                                            BOOL rx_fc_reach_extend,
                                            digi_sifd_ilkn_calendar32_t *tx_ilkn_fc_ptr);

PUBLIC PMC_ERROR digi_ilkn_cpb_chnl_deprov(digi_handle_t *digi_handle,
                                           digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                           digi_sifd_ilkn_calendar32_t *tx_ilkn_fc_ptr,
                                           digi_sifd_ilkn_calendar32_t *rx_ilkn_fc_ptr);

PUBLIC PMC_ERROR digi_ilkn_cpb_rx_chnl_deprov(digi_handle_t *digi_handle,
                                              digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                              digi_sifd_ilkn_calendar32_t *rx_ilkn_fc_ptr);

PUBLIC PMC_ERROR digi_ilkn_cpb_tx_chnl_deprov(digi_handle_t *digi_handle,
                                              digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                              digi_sifd_ilkn_calendar32_t *tx_ilkn_fc_ptr);

PUBLIC PMC_ERROR digi_ilkn_oduk_chnl_prov(digi_handle_t *digi_handle,
                                          digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                          UINT8 rate,
                                          digi_sifd_ilkn_calendar32_t *tx_ilkn_fc_ptr,
                                          digi_sifd_ilkn_calendar32_t *rx_ilkn_fc_ptr);

PUBLIC PMC_ERROR digi_ilkn_oduk_rx_chnl_prov(digi_handle_t                *digi_handle,
                                             digi_ilkn_chnl_t             *chnl_ctxt_ptr,
                                             digi_sifd_ilkn_calendar32_t  *rx_ilkn_fc_ptr);

PUBLIC PMC_ERROR digi_ilkn_oduk_tx_chnl_prov(digi_handle_t               *digi_handle,
                                             digi_ilkn_chnl_t            *chnl_ctxt_ptr,
                                             UINT8                        rate,
                                             digi_sifd_ilkn_calendar32_t *tx_ilkn_fc_ptr);

PUBLIC PMC_ERROR digi_ilkn_oduk_chnl_deprov(digi_handle_t *digi_handle,
                                            digi_ilkn_chnl_t *chnl_ctxt_ptr,
                                            digi_sifd_ilkn_calendar32_t *tx_ilkn_fc_ptr,
                                            digi_sifd_ilkn_calendar32_t *rx_ilkn_fc_ptr);

PUBLIC PMC_ERROR digi_ilkn_oduk_rx_chnl_deprov(digi_handle_t               *digi_handle,
                                               digi_ilkn_chnl_t            *chnl_ctxt_ptr,
                                               digi_sifd_ilkn_calendar32_t *rx_ilkn_fc_ptr);

PUBLIC PMC_ERROR digi_ilkn_oduk_tx_chnl_deprov(digi_handle_t              *digi_handle,
                                               digi_ilkn_chnl_t            *chnl_ctxt_ptr,
                                               digi_sifd_ilkn_calendar32_t *tx_ilkn_fc_ptr);

PUBLIC PMC_ERROR digi_oif_pkt_period_calc(digi_handle_t *digi_handle,
                                          digi_oduksw_stream_t rate_data,
                                          UINT32 num_odu0,
                                          DOUBLE client_bitrate,
                                          UINT32 pkt_size,
                                          UINT32 header_size,
                                          UINT32 *pkt_size_ptr,
                                          UINT32 *pkt_per_int_ptr,
                                          UINT32 *pkt_per_n_ptr,
                                          UINT32 *pkt_per_d_ptr,
                                          DOUBLE *eps_ptr);


PUBLIC PMC_ERROR digi_oduksw_chnl_map(digi_handle_t *digi_handle,
                                      util_global_switch_data_t *input_pc_ptr,
                                      util_global_switch_data_t *output_pc_ptr,
                                      digi_oduksw_stream_t rate_data,
                                      UINT32 client_num_odu0,
                                      DOUBLE client_bitrate,
                                      UINT32 pkt_size,
                                      UINT32 pkt_per_int,
                                      UINT32 pkt_per_n,
                                      UINT32 pkt_per_d,
                                      UINT32 halt_buffer,
                                      UINT32 latency_s);
                                      
PUBLIC PMC_ERROR digi_oduksw_int_latency_chnl_map(digi_handle_t *digi_handle,
                                                  util_global_switch_data_t *input_pc_ptr,
                                                  util_global_switch_data_t *output_pc_ptr,
                                                  digi_oduksw_stream_t rate_data,
                                                  UINT32 client_num_odu0,
                                                  DOUBLE client_bitrate,
                                                  UINT32 pkt_size,
                                                  UINT32 pkt_per_int,
                                                  UINT32 pkt_per_n,
                                                  UINT32 pkt_per_d,
                                                  UINT32 hys_buffer,
                                                  UINT32 halt_buffer,
                                                  UINT32 latency_s);
                                                  
PUBLIC PMC_ERROR digi_default_hys_latency_get(digi_handle_t *digi_handle, 
                                              util_global_switch_data_t *output_pc_ptr,
                                              UINT32 *hys_latency_refclk);                                                  
                                                                                        
PUBLIC PMC_ERROR digi_oduksw_chnl_demap(digi_handle_t *digi_handle,
                                        util_global_switch_data_t *output_pc_ptr);
PUBLIC PMC_ERROR digi_oduksw_chnl_activate(digi_handle_t *digi_handle,
                                           util_global_switch_data_t *output_pc_ptr );
PUBLIC PMC_ERROR digi_oduksw_chnl_deactivate(digi_handle_t *digi_handle,
                                             util_global_switch_data_t *output_pc_ptr);

PUBLIC PMC_ERROR digi_cpb_chnl_map(digi_handle_t *digi_handle,
                                   util_global_switch_data_t *input_pc_ptr,
                                   util_global_switch_data_t *output_pc_ptr,
                                   digi_cpb_stream_t rate_data,
                                   UINT32 client_num_odu0,
                                   UINT32 halt_buffer);
PUBLIC PMC_ERROR digi_cpb_chnl_demap( digi_handle_t *digi_handle,
                                      util_global_switch_data_t *output_pc_ptr );
PUBLIC PMC_ERROR digi_cpb_chnl_activate(digi_handle_t *digi_handle,
                                        util_global_switch_data_t *output_pc_ptr);

PUBLIC PMC_ERROR digi_oduksw_opsa_update(digi_handle_t *digi_handle,
                                         util_global_switch_data_t *output_pc_ptr,
                                         UINT32 pkt_size,
                                         UINT32 pkt_per_int,
                                         UINT32 pkt_per_n,
                                         UINT32 pkt_per_d,
                                         BOOL   hys_buffer_en, 
                                         UINT32 hys_buffer,
                                         UINT32 latency_s);

PUBLIC PMC_ERROR digi_otn_payload_type_transition(digi_handle_t *digi_handle,
                                                  digi_otn_odu_chnl_t  *odu_chnl_ctxt_ptr,
                                                  digi_odu_line_payload_t new_payload_mode);

/* DIGI firmware related. */
PUBLIC void digi_fw_pathname_set(digi_handle_t  *digi_handle,
                                 char const     *pathname_ptr);
PUBLIC void digi_fw_filename_set(digi_handle_t *digi_handle,
                                 char const* filename);
PUBLIC PMC_ERROR digi_fw_reload(digi_handle_t *digi_handle);

PUBLIC PMC_ERROR digi_fw_shutdown(digi_handle_t *digi_handle);

PUBLIC PMC_ERROR digi_cpb_chnl_deactivate(digi_handle_t *digi_handle,
                                          util_global_switch_data_t *output_pc_ptr);
PUBLIC PMC_ERROR digi_serdes_port_uninit(digi_handle_t *digi_handle,
                                         digi_serdes_port_t **port_ctxt_pptr);
PUBLIC PMC_ERROR digi_serdes_debug_status_get(digi_handle_t *digi_handle,
                                              serdes_debug_t *serdes_debug_ptr,
                                              BOOL clear_val);
PUBLIC PMC_ERROR digi_loopback_serdes_port_cfg(digi_handle_t *digi_handle,
                                               digi_serdes_port_t *port_ctxt_ptr,
                                               digi_loopback_t loopback_mode,
                                               BOOL8 enable);
PUBLIC PMC_ERROR digi_loopback_serdes_port_status_get(digi_handle_t *digi_handle,
                                               digi_serdes_port_t *port_ctxt_ptr,
                                               digi_loopback_t loopback_mode,
                                               BOOL8 *enabled_ptr);
PUBLIC PMC_ERROR digi_loopback_serdes_ge_cfg(digi_handle_t *digi_handle,
                                             digi_ge_serdes_id_t ge_intf,
                                             BOOL8 enable);
PUBLIC PMC_ERROR digi_loopback_serdes_ge_status_get(digi_handle_t *digi_handle,
                                             digi_ge_serdes_id_t ge_intf,
                                             BOOL8 *enabled_ptr);
PUBLIC PMC_ERROR digi_loopback_serdes_bp_cfg(digi_handle_t *digi_handle,
                                             digi_ilkn_intf_t ilkn_intf,
                                             BOOL8 enable);
PUBLIC PMC_ERROR digi_loopback_serdes_bp_status_get(digi_handle_t *digi_handle,
                                             digi_ilkn_intf_t ilkn_intf,
                                             BOOL8 *enabled_ptr);
PUBLIC PMC_ERROR digi_loopback_internal_cfg(digi_handle_t *digi_handle,
                                            util_global_switch_data_t *input_pc_ptr,
                                            digi_loopback_t loopback_mode,
                                            BOOL8 enable);
PUBLIC PMC_ERROR digi_loopback_internal_status_get(digi_handle_t *digi_handle,
                                                   util_global_switch_data_t *input_pc_ptr,
                                                   digi_loopback_t loopback_mode,
                                                   BOOL8 *enabled_ptr);
PUBLIC PMC_ERROR digi_cmf_insert_one_shot_gfp_cmf_cfg(digi_handle_t *digi_handle,
                                                      digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                      UINT32 payload_length_type,
                                                      UINT32 upi_value,
                                                      UINT32 genpayloadFCS,
                                                      BOOL8 *insertion_result,
                                                      digi_cmf_one_shot_cmf_t *digi_cmf_one_shot_cmf_ptr);
PUBLIC PMC_ERROR digi_cmf_extract_gfp_cmf_get(digi_handle_t *digi_handle,
                                              digi_mapper_chnl_t *chnl_ctxt_pptr,
                                              UINT32 extract_mode,
                                              digi_cmf_extract_cmf_t *digi_cmf_extract_cmf_ptr);
PUBLIC PMC_ERROR digi_cmf_trigger_opu_csf_cfg(digi_handle_t *digi_handle,
                                              digi_mapper_chnl_t *chnl_ctxt_pptr,
                                              digi_cmf_sw_control_action_t trigger);
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_conseq_action_cfg(digi_handle_t *digi_handle,
                                                          digi_mapper_chnl_t *chnl_ctxt_pptr,
                                                          digi_client2trunk_tables_cmf_t  *client2trunk_tables_ptr);
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_tables_update(digi_handle_t *digi_handle,
                                                      digi_mapper_chnl_t *chnl_ctxt_pptr);
PUBLIC PMC_ERROR digi_cmf_trunktoclient_conseq_action_cfg(digi_handle_t *digi_handle,
                                                          digi_mapper_chnl_t *chnl_ctxt_pptr,
                                                          digi_trunk2client_tables_cmf_t  *trunk2client_tables_ptr);
PUBLIC PMC_ERROR digi_cmf_trunktoclient_tables_update(digi_handle_t *digi_handle,
                                                      digi_mapper_chnl_t *chnl_ctxt_pptr);

PUBLIC PMC_ERROR digi_cmf_trunktoclient_conseq_action_get(digi_handle_t *digi_handle,
                                                          digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                          cmf_trunk2client_conseqAction_t  *trunk2client_tables_ptr);
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_conseq_action_get(digi_handle_t *digi_handle,
                                                          digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                          digi_client2trunk_conseq_action_t  *client2trunk_tables_ptr);

PUBLIC PMC_ERROR digi_cmf_trigger_lf_cfg(digi_handle_t *digi_handle,
                                         digi_enet_client_chnl_t  *chnl_handle,
                                         digi_cmf_sw_control_action_t trigger);

PUBLIC PMC_ERROR digi_cmf_trigger_rf_cfg(digi_handle_t *digi_handle,
                                         digi_enet_client_chnl_t    *chnl_handle,
                                         digi_cmf_sw_control_action_t trigger);

PUBLIC PMC_ERROR digi_cmf_trigger_cbr_fault_cfg(digi_handle_t *digi_handle,
                                                digi_cbr_client_chnl_t    *chnl_handle, 
                                                digi_cmf_sw_control_action_t enable);

PUBLIC PMC_ERROR digi_cmf_fdi_insertion_mapotn_channels_cfg(digi_handle_t *digi_handle,
                                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                            BOOL enable,
                                                            UINT32 upi_mode);

PUBLIC PMC_ERROR digi_fw_opu_csf_int_rsp(digi_handle_t *digi_handle, UINT32 *bits96_64, UINT32 *bits64_32, UINT32 *bits32_0 );

PUBLIC PMC_ERROR digi_cmf_clienttotrunk_sw_manual_trigger(digi_handle_t *digi_handle,
                                                          digi_mapper_chnl_t *chnl_ctxt_ptr, 
                                                          BOOL8 trigger);

PUBLIC PMC_ERROR digi_mac_mac_lf_rf_forwarding(digi_handle_t *digi_handle,
                                               BOOL8 feature_en );
PUBLIC PMC_ERROR digi_cmf_clienttotrunk_port_action_get(digi_handle_t *digi_handle,
                                                        digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                        digi_client2trunk_port_action_table_cmf_t  *client2trunk_tables_ptr);

PUBLIC PMC_ERROR digi_cmf_trunktoclient_port_mapping_get(digi_handle_t *digi_handle,
                                                         digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                         cmf_trunk2client_portMapping_t  *trunk2client_tables_ptr);
PUBLIC PMC_ERROR digi_cmf_non_std_upi_cfg(digi_handle_t *digi_handle,
                                          digi_mapper_chnl_t *chnl_ctxt_ptr,
                                          digi_cmf_upi_values_t upi_values);  



PUBLIC PMC_ERROR digi_serdes_port_polarity_cfg(digi_handle_t           *digi_handle,
                                               digi_serdes_port_t      *port_ctxt_ptr,
                                               UINT32                   logical_lanes,
                                               util_global_direction_t  dir,
                                               BOOL8                    enable_inv_polarity);

PUBLIC PMC_ERROR digi_serdes_ge_polarity_cfg(digi_handle_t           *digi_handle,
                                             UINT8                    num_ge_serdes,
                                             digi_ge_serdes_id_t      ge_intfs[],
                                             util_global_direction_t  dir,
                                             BOOL8                    enable_inv_polarity);

PUBLIC PMC_ERROR digi_serdes_bp_polarity_cfg(digi_handle_t                     *digi_handle,
                                             digi_sifd_ilkn_1_lane_cfg_t       *sifd_1_lane_cfg_ptr,
                                             digi_sifd_ilkn_2_lane_cfg_t       *sifd_2_lane_cfg_ptr,
                                             util_global_direction_t            dir,
                                             BOOL8                              enable_inv_polarity);

#ifdef PMC_DEPRECATED

/* LCOV_EXCL_START */
/* DEPRECATED-Function has been marked as deprecated starting in the 3.02 release */

PUBLIC void digi_serdes_t8_dfe_error_invert(digi_handle_t *digi_handle,
                                            UINT8 pin_id);


PUBLIC PMC_ERROR digi_serdes_sfi51_tx_dclk_enable(digi_handle_t *digi_handle,
                                                  digi_serdes_port_t *port_ctxt_ptr,
                                                  BOOL8 enable);


/* LCOV_EXCL_STOP */


#endif /* PMC_DEPRECATED */



PUBLIC BOOL8 digi_serdes_t8_dfe_error_get(digi_handle_t *digi_handle,
                                          UINT8 pin_id);
PUBLIC void *digi_serdes_handle_get(digi_handle_t *digi_handle,
                                    digi_serdes_intf_t intf);
PUBLIC PMC_ERROR digi_serdes_port_init_status_get(digi_handle_t      *digi_handle,
                                                  digi_serdes_port_t *port_ctxt_ptr,
                                                  BOOL8              *init_status);

PUBLIC PMC_ERROR digi_serdes_port_rate_prov_status_get(digi_handle_t *digi_handle,
                                                       digi_serdes_port_t *port_ctxt_ptr,
                                                       BOOL8 *prov_status,
                                                       digi_serdes_port_mode_t *port_mode,
                                                       digi_serdes_prov_state_t *prov_state);
PUBLIC PMC_ERROR digi_serdes_port_rate_prov(digi_handle_t *digi_handle,
                                            digi_serdes_port_t *port_ctxt_ptr,
                                            digi_serdes_port_mode_t port_mode);
PUBLIC PMC_ERROR digi_serdes_port_rate_deprov(digi_handle_t *digi_handle,
                                              digi_serdes_port_t *port_ctxt_ptr);
PUBLIC PMC_ERROR digi_serdes_line_sys_patt_mon_prov(digi_handle_t *digi_handle,
                                                    digi_serdes_port_t *port_ctxt_ptr,
                                                    UINT8 logical_lane,
                                                    util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr);
PUBLIC PMC_ERROR digi_serdes_line_sys_patt_mon_deprov(digi_handle_t *digi_handle,
                                                      digi_serdes_port_t *port_ctxt_ptr,
                                                      UINT8 logical_lane);
PUBLIC PMC_ERROR digi_serdes_line_sys_patt_mon_resync(digi_handle_t *digi_handle,
                                                      digi_serdes_port_t *port_ctxt_ptr,
                                                      UINT8 logical_lane);
PUBLIC PMC_ERROR digi_serdes_line_sys_patt_mon_err_count_get(digi_handle_t *digi_handle,
                                                             digi_serdes_port_t *port_ctxt_ptr,
                                                             UINT8 logical_lane,
                                                             util_patt_status_t    *status,
                                                             UINT32 *err_count);
PUBLIC PMC_ERROR digi_serdes_line_sys_patt_gen_prov(digi_handle_t *digi_handle,
                                                    digi_serdes_port_t *port_ctxt_ptr,
                                                    UINT8 logical_lane,
                                                    util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);
PUBLIC PMC_ERROR digi_serdes_line_sys_patt_gen_deprov(digi_handle_t *digi_handle,
                                                      digi_serdes_port_t *port_ctxt_ptr,
                                                      UINT8 logical_lane);
PUBLIC PMC_ERROR digi_serdes_line_sys_patt_gen_err_insert_cfg(digi_handle_t *digi_handle,
                                                              digi_serdes_port_t *port_ctxt_ptr,
                                                              UINT8 logical_lane);
PUBLIC PMC_ERROR digi_serdes_bp_patt_mon_prov(digi_handle_t *digi_handle,
                                              digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                              digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr,
                                              util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr);
PUBLIC PMC_ERROR digi_serdes_bp_patt_mon_deprov(digi_handle_t *digi_handle,
                                                digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                                digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr);
PUBLIC PMC_ERROR digi_serdes_bp_patt_mon_resync(digi_handle_t *digi_handle,
                                                digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                                digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr);
PUBLIC PMC_ERROR digi_serdes_bp_patt_gen_prov(digi_handle_t *digi_handle,
                                              digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                              digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr,
                                              util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);
PUBLIC PMC_ERROR digi_serdes_bp_patt_gen_deprov(digi_handle_t *digi_handle,
                                                digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                                digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr);
PUBLIC PMC_ERROR digi_serdes_bp_patt_mon_err_count_get(digi_handle_t *digi_handle,
                                                       digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                                       digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr,
                                                       util_patt_status_t    *status,
                                                       UINT32 *err_cnt);
PUBLIC PMC_ERROR digi_serdes_bp_patt_gen_err_insert_cfg(digi_handle_t *digi_handle,
                                                        digi_sifd_ilkn_1_lane_cfg_t *sifd_1_lane_cfg_ptr,
                                                        digi_sifd_ilkn_2_lane_cfg_t *sifd_2_lane_cfg_ptr);

PUBLIC PMC_ERROR digi_serdes_ge_patt_mon_prov(digi_handle_t *digi_handle,
                                              digi_ge_serdes_id_t ge_intf,
                                              util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr);
PUBLIC PMC_ERROR digi_serdes_ge_patt_mon_get(digi_handle_t *digi_handle,
                                             digi_ge_serdes_id_t ge_intf,
                                             util_serdes_patt_mon_cfg_t *patt_mon_cfg_ptr);
PUBLIC PMC_ERROR digi_serdes_ge_patt_mon_deprov(digi_handle_t *digi_handle,
                                                digi_ge_serdes_id_t ge_intf);
PUBLIC PMC_ERROR digi_serdes_ge_patt_mon_resync(digi_handle_t *digi_handle,
                                                digi_ge_serdes_id_t ge_intf);
PUBLIC PMC_ERROR digi_serdes_ge_patt_mon_err_count_get(digi_handle_t *digi_handle,
                                                       digi_ge_serdes_id_t ge_intf,
                                                       util_patt_status_t    *status,
                                                       UINT32 *err_cnt);
PUBLIC PMC_ERROR digi_serdes_ge_patt_gen_prov(digi_handle_t *digi_handle,
                                              digi_ge_serdes_id_t ge_intf,
                                              util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);
PUBLIC PMC_ERROR digi_serdes_ge_patt_gen_get(digi_handle_t *digi_handle,
                                             digi_ge_serdes_id_t ge_intf,
                                             util_serdes_patt_gen_cfg_t *patt_gen_cfg_ptr);
PUBLIC PMC_ERROR digi_serdes_ge_patt_gen_deprov(digi_handle_t *digi_handle,
                                                digi_ge_serdes_id_t ge_intf);
PUBLIC PMC_ERROR digi_serdes_ge_patt_gen_err_insert_cfg(digi_handle_t *digi_handle,
                                                        digi_ge_serdes_id_t ge_intf);
PUBLIC PMC_ERROR digi_serdes_los_force(digi_handle_t *digi_handle,
                                                  digi_serdes_port_t *port_ptr,
                                                  BOOL8 dlos_en, BOOL8 los_pol_inv);
PUBLIC PMC_ERROR digi_serdes_los_force_get (digi_handle_t *digi_handle,
                                                  digi_serdes_port_t *port_ptr,
                                                  BOOL8 *dlos_en, BOOL8 *los_pol_inv);
/*
* Emulation Api Start
*/

#ifdef PMC_DEPRECATED

/* This API is for debugging purpose only. */
PUBLIC PMC_ERROR digi_coreotn_ho_db_cfg(digi_handle_t *digi_handle,
                                        UINT32 chnl_id,
                                        UINT32 cal_slot_mask[3]);
#endif /* PMC_DEPRECATED */

PUBLIC PMC_ERROR digi_cpb_chnl_resize(digi_handle_t *digi_handle,
                                      util_global_switch_data_t *output_pc_ptr,
                                      UINT32 num_blocks);

PUBLIC PMC_ERROR digi_ocpb_chnl_resize(digi_handle_t *digi_handle,
                                       util_global_switch_data_t *output_pc_ptr,
                                       UINT32 num_blocks);
PUBLIC PMC_ERROR digi_fw_performance_queue_init(digi_handle_t *digi_handle , BOOL8 enable );



PUBLIC PMC_ERROR digi_set_revision_code(digi_handle_t *digi_handle,
                                        UINT32 revision_code);

PUBLIC PMC_ERROR digi_ilkn_chnl_cfg(digi_handle_t *digi_handle,
                                    digi_ilkn_link_t *ilkn_ctxt_ptr,
                                    digi_ilkn_chnl_target_t target,
                                    UINT32 tx_ilkn_bcw_chnl,
                                    UINT16 tx_2b_header,
                                    UINT32 tx_4b_header,
                                    UINT32 rx_ilkn_bcw_chnl,
                                    UINT16 rx_2b_header,
                                    UINT32 rx_4b_header,
                                    digi_ilkn_chnl_t **chnl_ctxt_pptr);

PUBLIC PMC_ERROR digi_ilkn_chnl_uncfg(digi_handle_t          *digi_handle,
                                      digi_ilkn_chnl_t      **chnl_ctxt_ptr);

PUBLIC PMC_ERROR digi_ilkn_chnl_cfg_lock(digi_handle_t *digi_handle);

PUBLIC PMC_ERROR digi_ilkn_chnl_cfg_unlock(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_ilkn_chnl_cfg_rx_reset(digi_handle_t *digi_handle,
                                             digi_ilkn_link_t *ilkn_intf_handle,
                                             BOOL enable);


PUBLIC PMC_ERROR digi_pgmrclk_div_cfg(digi_handle_t *digi_handle,
                                      digi_pgmrclk_div_cfg_t *pgmrclk_cfg,
                                      BOOL8 enable);
PUBLIC PMC_ERROR digi_pgmrclk_div_cfg_get(digi_handle_t *digi_handle,
                                          digi_pgmrclk_div_id_t div_id,
                                          digi_pgmrclk_div_cfg_t *pgmrclk_cfg_ptr,
                                          BOOL8 *enable);
PUBLIC PMC_ERROR digi_pgmrclk_demapper_source_ctxt_get(digi_handle_t *digi_handle, 
                                                       util_global_switch_data_t *chnl_handle, 
                                                       digi_pgmrclk_input_source_t *clk_source_ptr, 
                                                       UINT32 *clk_source_idx_ptr);
PUBLIC PMC_ERROR digi_pgmrclk_fpi_cfg(digi_handle_t *digi_handle,
                                      digi_pgmrclk_fpi_cfg_t *fpi_cfg_ptr);

PUBLIC PMC_ERROR digi_pgmrclk_div_output_cfg(digi_handle_t *digi_handle,
                                             digi_pgmrclk_div_id_t div_id,
                                             BOOL8 enable);

PUBLIC PMC_ERROR digi_serdes_start_adaptation(digi_handle_t *digi_handle,
                                              hostmsg_adapt_intf_type_t interface,
                                              UINT32 lane_0_19_msk,
                                              UINT32 lane_20_39_msk);

PUBLIC PMC_ERROR digi_serdes_adapt_status_get(digi_handle_t *digi_handle,
                                              serdes_adapt_status_t *serdes_adapt_status_ptr);


PUBLIC PMC_ERROR digi_serdes_debug_cfg(digi_handle_t *digi_handle,
                                       BOOL skip_cfg);

PUBLIC PMC_ERROR digi_serdes_port_tx_refclk_div_cfg(digi_handle_t         *digi_handle,
                                                    digi_serdes_port_t    *port_ctxt_ptr,
                                                    UINT32                 logical_lanes_msk,
                                                    digi_s16_clk_divider_t clk_divider);

PUBLIC PMC_ERROR digi_serdes_cfg_set(digi_handle_t       *digi_handle,
                                     digi_serdes_cfg_t   *serdes_cfg_ptr);

PUBLIC PMC_ERROR digi_serdes_cfg_get(digi_handle_t       *digi_handle,
                                     digi_serdes_cfg_t   *serdes_cfg_ptr);

PUBLIC PMC_ERROR digi_serdes_cfg_file_parse(digi_handle_t        *digi_handle,
                                            char                 *filename,
                                            digi_serdes_cfg_t    *serdes_cfg_ptr);

PUBLIC void digi_serdes_cfg_file_save(digi_handle_t            *digi_handle,
                                      const char               *file_path_ptr);

PUBLIC PMC_ERROR digi_serdes_s16_tx_swing_emphasis_set(digi_handle_t       *digi_handle,
                                                       digi_serdes_intf_t   s16_intf,
                                                       UINT32               lane,
                                                       UINT8                amplitude,
                                                       UINT8                post_cursor,
                                                       UINT8                pre_cursor);

PUBLIC PMC_ERROR digi_serdes_t8_tx_swing_emphasis_set(digi_handle_t      *digi_handle,
                                                      UINT32              lane,
                                                      UINT8               tx_dpeni1,
                                                      UINT8               tx_dpeni2,
                                                      UINT32              den,
                                                      UINT32              tx_psel);

PUBLIC PMC_ERROR digi_serdes_c8_tx_swing_emphasis_set(digi_handle_t      *digi_handle,
                                                      UINT32              lane,
                                                      UINT8               tx_dpeni1,
                                                      UINT8               tx_dpeni2,
                                                      UINT32              den,
                                                      UINT32              tx_psel);

PUBLIC PMC_ERROR digi_serdes_sfi51_tx_swing_set(digi_handle_t             *digi_handle,
                                                UINT32                     bus_id,
                                                UINT32                     lane,
                                                d8_tx_2x_tx_swing_level_t  tx_swing_level);

PUBLIC PMC_ERROR digi_serdes_ge_tx_swing_set(digi_handle_t              *digi_handle,
                                             digi_ge_serdes_id_t         ge_intf,
                                             d8_tx_2x_tx_swing_level_t   tx_swing_level);

PUBLIC PMC_ERROR digi_serdes_dc_coupling_set(digi_handle_t       *digi_handle,
                                             digi_serdes_intf_t   interface,
                                             UINT32               lane_0_19_msk,
                                             UINT32               lane_20_39_msk,
                                             BOOL8                dc_coupling_enabled);

PUBLIC PMC_ERROR digi_int_init(digi_handle_t   *digi_handle,
                               cpuif_int_t      int_type,
                               BOOL             enable);

PUBLIC PMC_ERROR digi_int_enabled_check(digi_handle_t          *digi_handle);

PUBLIC PMC_ERROR digi_int_monitor_cfg(digi_handle_t *digi_handle,
                                  BOOL polling_mode,
                                  UINT32 polling_mode_wait_us,
                                  BOOL forever);

PUBLIC PMC_ERROR digi_int_monitor(digi_handle_t *digi_handle);

PUBLIC void digi_int_monitor_disable(digi_handle_t *digi_handle);

PUBLIC void digi_int_device_aggregator_enable(digi_handle_t *digi_handle,
                                              cpuif_int_t int_type,
                                              UINT32 int_enable);

PUBLIC void digi_int_device_aggregator_disable(digi_handle_t *digi_handle,
                                               cpuif_int_t int_type,
                                               UINT32 int_enable);

PUBLIC UINT32 digi_int_device_aggregator_get(digi_handle_t *digi_handle,
                                             cpuif_int_t int_type);

PUBLIC PMC_ERROR digi_device_int_register_cb(digi_handle_t      *digi_handle,
                                             void               *token,
                                             UINT32              token_size,
                                             digi_int_cb_fcn     user_cb);

PUBLIC PMC_ERROR digi_device_int_enable(digi_handle_t            *digi_handle,
                                        digi_device_int_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_device_int_disable(digi_handle_t            *digi_handle,
                                         digi_device_int_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_device_int_clear(digi_handle_t            *digi_handle,
                                       digi_device_int_t        *int_table_ptr);

PUBLIC PMC_ERROR digi_device_int_retrieve(digi_handle_t      *digi_handle,
                                         digi_device_int_t   *int_table_ptr);

PUBLIC PMC_ERROR digi_device_int_enabled_get(digi_handle_t       *digi_handle,
                                             digi_device_int_t   *int_table_ptr);

PUBLIC PMC_ERROR digi_device_int_enabled_check(digi_handle_t   *digi_handle);



PUBLIC PMC_ERROR digi_dcsu_int_enable(digi_handle_t             *digi_handle,
                                      digi_dcsu_inst_t           dcsu_inst,
                                      digi_dcsu_int_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_dcsu_int_disable(digi_handle_t             *digi_handle,
                                       digi_dcsu_inst_t           dcsu_inst,
                                       digi_dcsu_int_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_dcsu_int_retrieve(digi_handle_t              *digi_handle,
                                        digi_dcsu_inst_t            dcsu_inst,
                                        digi_dcsu_int_t            *int_table_ptr);

PUBLIC PMC_ERROR digi_dcsu_int_retrieve_act(digi_handle_t              *digi_handle,
                                        digi_dcsu_inst_t            dcsu_inst,
                                        digi_dcsu_int_t            *int_table_ptr);

PUBLIC PMC_ERROR digi_dcsu_int_clear(digi_handle_t       *digi_handle,
                                     digi_dcsu_inst_t     dcsu_inst,
                                     digi_dcsu_int_t     *int_table_ptr);

PUBLIC PMC_ERROR digi_m1_rate_monitor_cfg_set(digi_handle_t *digi_handle,
                                              digi_m1_pmm_lite_clk_ref_t ref_clk_src,
                                              DOUBLE ref_clk_freq,
                                              BOOL8 enable);
PUBLIC PMC_ERROR digi_m1_rate_monitor_cfg_get(digi_handle_t *digi_handle,
                                              digi_m1_pmm_lite_clk_ref_t *ref_clk_src,
                                              DOUBLE *ref_clk_freq,
                                              BOOL8 *enable);
PUBLIC PMC_ERROR digi_serdes_m1_rate_monitor_drop_prov(digi_handle_t *digi_handle, 
                                                       UINT32 identifier,
                                                       digi_serdes_intf_t intf, 
                                                       UINT8 pin, 
                                                       DOUBLE *output_clk_freq_ptr,
                                                       digi_serdes_port_t **port_ctxt_pptr);
PUBLIC PMC_ERROR digi_serdes_m1_rate_monitor_drop_deprov(digi_handle_t *digi_handle, 
                                                         digi_serdes_port_t **port_ctxt_pptr);
PUBLIC PMC_ERROR digi_opsa_fifo_cent_trigger(digi_handle_t *digi_handle,
                                             UINT8 num_chnl,
                                             util_global_switch_data_t **chnl_handle);

PUBLIC PMC_ERROR digi_serdes_sfi51_tx_refclk_source_set(digi_handle_t                     *digi_handle,
                                                        digi_serdes_port_t                *port_ctxt_ptr,
                                                        util_global_sfi51_refclk_source_t  refclk_src);

PUBLIC PMC_ERROR digi_fw_opsa_mpmo_debug_get(digi_handle_t *digi_handle,
                                             fw_opsa_mpmo_debug_t *fw_opsa_status_ptr);
PUBLIC PMC_ERROR digi_fw_opsa_debug_status_get(digi_handle_t *digi_handle,
                                               hostmsg_opsa_subsystem_cfg_t subsystem,
                                               UINT8 channel,
                                               fw_opsa_status_t *fw_opsa_status_ptr);




PUBLIC util_global_switch_data_t* digi_cpb_remap_clean(digi_handle_t * digi_handle);
PUBLIC util_global_switch_data_t* digi_oduksw_remap_clean(digi_handle_t * digi_handle);
PUBLIC void digi_debug_oduksw_page_block_alloc_print(digi_handle_t* digi_handle);
PUBLIC PMC_ERROR digi_dcsu_diag_init(digi_handle_t *digi_handle, BOOL8 enable);

PUBLIC PMC_ERROR  digi_serdes_vert_hist_get(digi_handle_t *digi_handle,
                                            UINT32                    s16_line_lane_0_11_msk,
                                            UINT32                    s16_sys_lane_0_15_msk,
                                            UINT32                    t8_lane_0_19_msk,
                                            UINT32                    t8_lane_20_39_msk,
                                            util_vhist_test_length_t  test_length,
                                            UINT64                    userdef_length,
                                            UINT64                    s16_line_bin_buffer[][64],
                                            UINT64                    s16_sys_bin_buffer[][64],
                                            UINT64                    t8_bin_buffer[][64]);
PUBLIC void digi_serdes_vert_eye_opening_calc(digi_handle_t *digi_handle,
                                            UINT32  s16_line_lane_0_11_msk,
                                            UINT32  s16_sys_lane_0_15_msk,
                                            UINT32  t8_lane_0_19_msk,
                                            UINT32  t8_lane_20_39_msk,
                                            UINT64  s16_line_bin_buffer[][64],
                                            UINT64  s16_sys_bin_buffer[][64],
                                            UINT64  t8_bin_buffer[][64],
                                            UINT32  s16_line_port_verteye_opening[12],
                                            UINT32  s16_sys_port_verteye_opening[16],
                                            UINT32  t8_port_verteye_opening[40]);

PUBLIC PMC_ERROR digi_gsup43_7_3_legacy_mode_cfg(digi_handle_t *digi_handle,
                                                 util_global_switch_data_t *channel_ptr,
                                                 UINT8 data_frame_upi,
                                                 UINT8 ordered_set_upi);

PUBLIC PMC_ERROR digi_gsup43_7_3_legacy_mode_get(digi_handle_t *digi_handle,
                                                 digi_mapper_chnl_t *channel_ptr,
                                                 UINT8 *data_frame_upi_ptr,
                                                 UINT8 *ordered_set_upi_ptr,
                                                 BOOL8 *enabled_ptr);

PUBLIC PMC_ERROR digi_gsup43_7_3_tgfpf_upi_set(digi_handle_t *digi_handle,
                                               digi_mapper_chnl_t *channel_ptr,
                                               UINT32 upi,
                                               UINT32 upi_sel);

PUBLIC PMC_ERROR digi_gsup43_7_3_increase_orderred_set_rate(digi_handle_t *digi_handle,
                                                            digi_mapper_chnl_t *channel_ptr,
                                                            BOOL8 enable);

PUBLIC PMC_ERROR digi_gsup43_7_3_increase_orderred_set_rate_get(digi_handle_t *digi_handle,
                                                                digi_mapper_chnl_t *channel_ptr,
                                                                BOOL8 *enabled_ptr);

PUBLIC void digi_fw_show_firmware_stats(digi_handle_t *digi_handle );

PUBLIC PMC_ERROR digi_cm_status_get(digi_handle_t *digi_handle,
                                    util_global_switch_data_t *chnl_hndl_ptr,
                                    UINT32 *value);

PUBLIC void digi_fw_log_prov_deprov_init( digi_fw_log_prov_deprov_stats_t *t, const char *name, digi_fw_log_prov_deprov_callback_t _prov_function, digi_fw_log_prov_deprov_callback_t _deprov_function, digi_handle_t *digi_handle, void *_test_parameters );

PUBLIC void digi_fw_log_prov_deprov_halt( digi_fw_log_prov_deprov_stats_t *t );

PUBLIC void digi_fw_log_prov_deprov_print( digi_fw_log_prov_deprov_stats_t *t );

PUBLIC void digi_fw_log_prov_deprov_test_all( digi_fw_log_prov_deprov_stats_t *t, double duration );

PUBLIC void digi_fw_log_prov_deprov_test( digi_fw_log_prov_deprov_stats_t *t, digi_fw_log_prov_deprov_test_type test_type, double duration );
#endif /* DOXYGEN_PUBLIC_ONLY */


/*
* Emulation Api End
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
#define DIGI_LOG_ERR_TABLE                                              \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_LOG_CODE_DEBUG,                            "DIGI: Log debug" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CODE_ASSERT,                           "DIGI: Err assert" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_LOG_CODE_LOG,                              "DIGI: Log logging code" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_LOG_IBN_LEN_SEQ_NUM,                       "DIGI: info status get inbound_msg_len and inbound_seq_num" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_LOG_LCLK_SETUP_IBN_LEN_SEQ_NUM,            "DIGI: subsystem LCLK setup inbound_msg_len and inbound_seq_num" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_LOG_OBND_DATA,                             "DIGI: outbound data" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_LOG_LCLK_SETUP,                            "DIGI: lclk setup data index and data" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_LOG_LCLK_CFG_ERR,                          "DIGI: digi_pmon_lck_cfg error" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_LOG_COLLECTION_MODE_CFG,                   "DIGI: config collection mode inbound_msg_len and inbound_seq_num" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_LOG_COLLECTION_MODE_DATA_MSG,              "DIGI: config collection mode setup data index and data" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_LOG_CONFIG_COLLECTION_MODE_ERR,            "DIGI: config collection mode error" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_LOG_COLLECTION_MODE_OBN_DATA,              "DIGI: config collection mode data index and data received" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_LOG_WORK_ORDER_SEND,                       "DIGI: work order send inbound len and sequence number" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_LOG_WORK_ORDER_SEND_IDX_DATA,              "DIGI: work order send data index and data" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_LOG_WORK_ORDER_SEND_ERR,                   "DIGI: work order send error" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_NULL_HANDLE,                           "DIGI: NULL device handle" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_ARG,                           "DIGI: Invalid argument" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_BCW_CHANNEL_INUSE,                     "DIGI: BCW Channel is already configured" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_UNKNOWN,                               "DIGI: Unknown error" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_CANNOT_UNINIT,                  "DIGI: Cannot uninitialize serdes port otherwise no datapath could be provisioned against it later." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_CAGE_ALREADY_PROV,              "DIGI: A cage with this id has already been provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_ALL_CAGE_ALREADY,               "DIGI: A cage has already been provisioned on this IF type." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_PORT_NOT_PROV,                  "DIGI: A SERDES port is not provisioned against this sub system" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_PORT_ALREADY_PROV,              "DIGI: A SERDES port with this id has already been provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_ALL_PORTS_PROV,                 "DIGI: All SERDES ports have already been provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_CAGE_STILL_PROV,                "DIGI: Serdes port is associated to provisioned cage. Cannot deprovision serdes rate." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_NO_SFI51_CAGE_PROV,             "DIGI: It is not necessary to provision a cage on the SFI 5.1 interface." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_INVALID_NUM_PINS_PER_PORT_PROV, "DIGI: The number of pins configured on this port is not valid" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_SYS_PIN_ALREADY_PROV,           "DIGI: The SERDES System pin has already been provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_SYS_NO_PIN_PROV,                "DIGI: Could not find any System pins provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_SFI51_BUS_ALREADY_PROV,         "DIGI: The SERDES SFI51 bus has already been provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_SFI51_NO_BUS_PROV,              "DIGI: Could not find the bus to provision." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_XFI_PIN_ALREADY_PROV,           "DIGI: The SERDES XFI pin has already been provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_XFI_NO_PIN_PROV,                "DIGI: Could not find any XFI pins provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_LOS_PIN_ALREADY_USED,           "DIGI: The specified LOS pin is already in use." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_PORT_UID_NOT_FOUND,             "DIGI: The specified port uid cannot be found." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_INVALID_S16_CLK_CFG,            "DIGI: The specified s16 clock configuratgion is not valid." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_100G_3_X_40G_CFP_CARD_CFG_ERR,         "DIGI: This card application requires 1x100G or 3x40G serdes defined." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_100G_3_X_40G_CFP_CARD_SERDES_UNINIT,   "DIGI: Cannot deprovision serdes port for 100G_3_X_40G_CFP_CARD application." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_NO_SYSTEM_SERDES_DEFINED,       "DIGI: This card application requires that system serdes are defined." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_SYSTEM_SERDES_DEFINED,          "DIGI: This card application requires that no system serdes are defined." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MAPPER_RESOURCE_NOT_AVAILABLE,         "DIGI: Insufficient mapper resources available." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MAPPER_FCS_INSERTION_NOT_ALLOWED,      "DIGI: FCS insertion is not allowed." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MAPPER_EXT_HDR_GEN_NOT_ALLOWED,        "DIGI: Extension header generation is not allowed." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MAPPER_CHNL_NOT_AVAIL,                 "DIGI: Mapper Channel is not available." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ILKN_IS_ACTIVE,                        "DIGI: ILKN is active. Cannot provision channel." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ILKN_CHNL_IN_USE,                      "DIGI: ILKN Channel is already in use." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ILKN_1_CHNL_NOT_AVAIL,                 "DIGI: ILKN 1 Channel is not available." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ILKN_2_CHNL_NOT_AVAIL,                 "DIGI: ILKN 2 Channel is not available." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ILKN_CHNL_UNCONFIG,                    "DIGI: ILKN Channel is not configured." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ILKN_CHNL_ALREADY_PROV,                "DIGI: ILKN Channel is already provisioned." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ILKN_CHNL_NOT_PROV,                    "DIGI: ILKN Channel is not provisioned." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ILKN_IN_WRONG_MAJ_MODE,                "DIGI: ILKN is set in the wrong major mode." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ILKN_CPB_INTF_NOT_EN,                  "DIGI: ILKN CPB interfaces is not enabled." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ILKN_ODUKSW_INTF_NOT_EN,               "DIGI: ILKN ODUKSW interfaces is not enabled." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ILKN_CPB_PROV,                         "DIGI: ILKN is provisionned on CPB interface." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ILKN_CPB_NOT_SUPPORT_ENET_PMON_DIGI60, "DIGI: ILKN CPB interface does not support ENET PMON on a DIGI60 device." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ILKN_IN_WRONG_HEADER_MODE,             "DIGI: ILKN is set in the wrong header mode." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ILKN_CHNL_PROV_INCOMPATIBLE_TARGET,    "DIGI: Incompatible target: Cannot provision channel on CPB and ODUKSW in the same time." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_LINE_RESOURCE_NOT_AVAILABLE,      "DIGI: Insufficient ENET LINE resources available." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_SYS_RESOURCE_NOT_AVAILABLE,       "DIGI: Insufficient ENET SYS resources available." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_COULD_NOT_PROV,                   "DIGI: Could not provision the requested ENET datapath" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_ONLY_TRANSPARENT_MODES_SUPPORTED, "DIGI: ENET Line only supports transparent modes in muxponder application.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_SYS_UNAVAILABLE_IN_CARD_MODE,     "DIGI: ENET SYS may not be provisioned in this card application.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PORT_PROV_RATE_FAIL,                   "DIGI: SERDES port rate does not match provisioned application" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PORT_PROV_CAGE_FAIL,                   "DIGI: SERDES port incompatible with cage size" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_NO_CAGE_PROV,                   "DIGI: No cage has been configured at this pin location" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_NO_DSI_ALLOC_PROV,                     "DIGI: NO DSI Lane allocation has occurred for this port" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_POST_MLD_DSI_LANE_NOT_FOUND,           "DIGI: Cannot find post-mld dsi lane" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CPB_CHNL_NOT_PROV,                     "DIGI: Cannot map at CPB as this resource is not provisioned." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CPB_NOT_MAPPED,                        "DIGI: Cannot activate as this resource is not mapped at CPB." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CPB_ALREADY_MAPPED,                    "DIGI: Cannot map at CPB as this resource has already been mapped." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CPB_ALREADY_ACTIVE,                    "DIGI: Cannot activate as this resource is already active at CPB." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CPB_STILL_ACTIVE,                      "DIGI: Cannot demap as this resource is still active at CPB." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CPB_STILL_MAPPED,                      "DIGI: Cannot deprovision as this resource is still mapped at CPB." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CPB_NOT_ACTIVE,                        "DIGI: Cannot proceed as this resource is not active at CPB." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CPB_INCOMPATIBLE_RATE,                 "DIGI: CPB Requested rate is incompatible for these input and output ports.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CPB_INCOMPATIBLE_MAPPING,              "DIGI: CPB Mapping is not supported for these input and output ports.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MAPPER_NOT_MAPPED,                     "DIGI: Cannot deprovision as this resource is not mapped." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MAPPER_STILL_MAPPED,                   "DIGI: Cannot deprovision as this resource is still mapped." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MAPPER_ALREADY_MAPPED,                 "DIGI: Cannot map as this resource has already been mapped." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MAPPER_ENET_LINE_RESOURCE_IN_USE,      "DIGI: Cannot map: required ENET resource is already in use." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MAPPER_CANNOT_MODIFY,                  "DIGI: This map modification is not supported in this mode." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ODUKSW_CHNL_NOT_PROV,                  "DIGI: Cannot map at ODUKSW as this resource is not provisioned." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ODUKSW_NOT_MAPPED,                     "DIGI: Cannot activate as this resource is not mapped at ODUK." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ODUKSW_ALREADY_MAPPED,                 "DIGI: Cannot map at ODUKSW as this resource has already been mapped." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ODUKSW_ALREADY_ACTIVE,                 "DIGI: Cannot activate at ODUKSW as this resource is already active." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ODUKSW_STILL_ACTIVE,                   "DIGI: Cannot demap at ODUKSW as this resource is still active." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ODUKSW_STILL_MAPPED,                   "DIGI: Cannot deprovision as this resource is still mapped at ODUKSW." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ODUKSW_NOT_ACTIVE,                     "DIGI: Cannot deactivate as this resource is not active at ODUKSW." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ODUKSW_INCOMPATIBLE_RATE,              "DIGI: ODUKSW Requested rate is incompatible for these input and output ports.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_OOM,                                   "DIGI: Out of memory error" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MAX_BWTH_PROV,                         "DIGI: The maximum bandwidth already provisioned for this device" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PRIMARY_UNABLE_TO_DEACTIVATE,          "DIGI: Deactivate Non-primary readers before the primary reader" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ODU0_INSUFFICIENT_RESOURCES_AVAILABLE, "DIGI: Insufficient ODU0 resources available" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SCHD_INSUFFICIENT_RESOURCES_AVAILABLE, "DIGI: Insufficient resources available in scheduler" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SCHD_RESOURCE_NOT_AVAILABLE,           "DIGI: Scheduler resource is not available" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_CTXT_STATE,                    "DIGI: Invalid device context state detected" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_ODU_CHNL_CTXT,                 "DIGI: Invalid ODU channel context state detected" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_ODU_CHNL_DEPROV,               "DIGI: ODU channel cannot be deprovisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_PTR,                           "DIGI: Invalid pointer" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_DATA_KEY,                      "DIGI: Invalid channel handle data key" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_FW_UNAVAILABLE,                        "DIGI: Firmware unavailable" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_FW_BOOT_PCIE_INIT_FAIL,                "DIGI: Firmware boot PCIE initialization failure" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_FW_BOOT_NOT_DONE,                      "DIGI: Firmware has not finished booting" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_FW_BOOT_FAIL,                          "DIGI: Firmware has failed to boot" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_FW_FAILED_TO_SHUTDOWN,                 "DIGI: Firmware failed to shutdown" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_FW_DOWNLOAD_FAIL,                      "DIGI: Firmware failed to download" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_FW_SPRAM_PCIE_WRITE_FAIL,              "DIGI: Firmware write to SPRAM from PCIe failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_FW_SPRAM_UNLOCK_FAIL,                  "DIGI: SPRAM Unlock Failed" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_FW_IS_NOT_RUNNING,                     "DIGI: Firmware is not running" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_PMON_RESOURCE_UNAVAILABLE,        "DIGI: The requested ENET PMON resource is unavailable" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_PMON_NOT_SUPPORTED,               "DIGI: ENET PMON is not supported in this mode" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_CANNOT_MODIFY,                    "DIGI: This ENET modification is not supported in this mode." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_PIN_ALLOC_COLLISION,            "DIGI: More than one resource allocated a Serdes pin" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_CSU_MULTIRATE_REQUEST,          "DIGI: More than one rate assigned to a common backplane CSU" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_COULD_NOT_PROV,                 "DIGI: Could not provision the SERDES" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_COULD_NOT_SHUTDOWN,             "DIGI: Could not shutdown the SERDES" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PGMRCLK_COULD_NOT_UNCONFIG,            "DIGI: Could not unconfigure the PGMCLK" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_NO_SW_CONTENT,                         "DIGI: Temporary return code during development" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PGMRCLK_DIV_BY_ZERO,                   "DIGI PGMRCLK: Division by 0" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_LOOPBACK_MODE,                 "DIGI: Invalid loopback mode" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_LOOPBACK_NOT_PROV,                     "DIGI: Loopback is not provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_LOOPBACK_ALREADY_PROV,                 "DIGI: Loopback already provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_LOOPBACK_STILL_ACTIVE,                 "DIGI: Cannot deprovision as this resource has an active loopback" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_LOOPBACK_INVALID_PORT_PROV,            "DIGI: Loopback invalid SERDES Port provisioning state" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_LOOPBACK_INVALID_PORT_UID,             "DIGI: Loopback invalid SERDES Port UID" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PRBS_ALREADY_PROV,                     "DIGI: PRBS already provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PRBS_NOT_PROV,                         "DIGI: PRBS NOT provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PRBS_MON_STILL_ACTIVE,                 "DIGI: Cannot deprovision as this resource has an active prbs monitor" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PRBS_GEN_STILL_ACTIVE,                 "DIGI: Cannot deprovision as this resource has an active prbs generator" )\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE,         "DIGI: Interrupt handling callback must be registered before enabling interrupts" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MORE_THAN_ONE_PIN_SELECTED,            "DIGI: Only one serdes pin shall be selected" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INT_ONLY_CHNL_TYPE_ALLOWED,            "DIGI: This interrupt function only supports channel interrupt types" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INT_ONLY_AGG_TYPE_ALLOWED,             "DIGI: This interrupt function only supports aggregate interrupt types" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INT_CHNL_NOT_PROV,                     "DIGI: Chnl must be prov before interrupts can be retrieved" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INT_TCM_NUM_NOT_PROV,                  "DIGI: TCM NUM must be prov before interrupts can be enabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INT_PRBS_NOT_PROV,                     "DIGI: PRBS chnl must be prov before interrupts can be enabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INT_FILTER_TABLE_IS_NULL,              "DIGI: Filter table must be provided to API" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INT_SUMMARY_ODU_LEVEL_NOT_AVAIL,       "DIGI: ODU Level was not provided in interrupt summary table." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INT_SUMMARY_ODU_LEVEL_MISMATCH,        "DIGI: ODU level provided in interrupt summary table does not match requested operation" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INT_REV_A_LATCH_ALL_ONLY,              "DIGI: Rev A must latch all RFRMs simultaneously. Must use COREOTN_LATCH_DEFECT_ALL arg.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_IPI_BIT_IS_NOT_FW_DONE,                "DIGI: IPI bit is not set to FW_DONE") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ACTIVE_INT_FOUND,                      "DIGI: Active interrupt has been found" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_STATUS_CHNL_NOT_PROV,                  "DIGI: Chnl must be prov before status can be retrieved" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PMON_CHNL_NOT_PROV,                    "DIGI: Chnl must be prov before PMON can be retrieved" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PMON_IPI_BIT_NOT_READY,                "DIGI: IPI BITS indicates shared mem is not available" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_RI_RCP_CHNL_NOT_PROV,                  "DIGI: Cannot activate Remote Information Ring Control Port function as this channel is not provisionned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_MAPPING_MODE,                  "DIGI: Invalid mapping mode" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_DI_RCP_PDU_NOT_PROCESSED,              "DIGI: DI RCP PDU cannot be processed") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CBR_INCORRECT_SERDES,                  "DIGI: CBR only supports LINE and SYS serdes") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CBR_INCORRECT_SERDES_FOR_PROD_APP,     "DIGI: Incorrect CBR serdes chosen for product application") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_PORT_TYPE,                     "DIGI: Invalid handle port type") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_VLAN_ID_OUT_OF_RANGE,                  "DIGI: VLAN ID value is out of range: [0..4095].") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_VLAN_TYPE_NOT_SUPPORTED,               "DIGI: VLAN type not supported.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_HANDLE_NOT_INIT,                       "DIGI: DIGI handle not initialized.")  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_DEVICE_NOT_INIT,                       "DIGI: DIGI device not initialized.")  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_DEVICE_INIT_UNCOMPLETE,                "DIGI: DIGI device initialization uncomplete.")  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MCPB_CHNL_STILL_PROV,                  "DIGI: MCPB channels still provisioned.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MGMT_FEGE_ALREADY_INIT,                "DIGI: MGMT_FEGE already initialized.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_OTN_ODU_PID_OUT_OF_RANGE,                  "DIGI: TCM PID out of range.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MGMT_FEGE_NOT_INIT,                    "DIGI: MGMT_FEGE not initialized.")  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_GE_SERDES_ALREADY_CFG,                 "DIGI: GE interface already in enabled or disabled.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_GE_SERDES_NOT_PROV,                    "DIGI: GE interface serdes not provisioned.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ODU_LEVEL_NOT_VALID,                   "DIGI: Chnl ODU level not valid for this operation.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ENET_RX_FLOW_CONTROL_ENABLED,              "DIGI: Cannot executed operation when ENET RX link flow control enabled.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_PAUSE_QUANTA_THRES,            "DIGI: ENET link flow control pause quanta threshold shall be set to more than the half of the provided pause quanta value.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_LINE_SYS_CHNL_MISMATCH,           "DIGI: Mismatch of ENET LINE/SYS API and ENET LINE/SYS channel") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INCORRECT_HANDLE_FOR_ENH_PMON,         "DIGI: Incorrect channel handle for enhanced PMON. Must be a mapper or ilkn handle.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_NO_ENH_PMON,                           "DIGI: Enhanced PMON was not found on this handle") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_SERDES_RATE,                   "DIGI: Invalid serdes rate.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_ADAPT_PARAMS_NOT_FOUND,         "DIGI: Serdes adaptation parameters not found.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_TX_SWING_LEVELS_NOT_FOUND,      "DIGI: Serdes adaptation parameters not found.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_ADAPT_ON_RESET,                 "DIGI: Trying to run adaptation on Serdes in reset.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_PARAMS_NOT_SAVED_IN_CTXT,       "DIGI: Serdes configuration parameters were not saved in context.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_ADAPT_FAILED,                   "DIGI: Serdes adaptation failed for one or multiple serdes.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_PRBS_FAIL,                      "DIGI: Serdes PRBS error") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_STILL_IN_USE,                   "DIGI: Serdes still in use by a sub system.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_CANNOT_UNINIT_SFI51,            "DIGI: SFI 5.1 Serdes may not be uninitialized.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_PKT_GEN_LINK,                     "DIGI: Enet Pkt Gen enabled on differnt link.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_D8_SERDES_CONFIG,                      "DIGI: D8 SERDES debug")  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_C8_SERDES_CONFIG,                      "DIGI: C8 SERDES debug")  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_T8_SERDES_CONFIG,                      "DIGI: T8 SERDES debug")  \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_S16_SERDES_CONFIG,                     "DIGI: S16 SERDES debug") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ATOMIC_KEEP_LOCK_MODE_ALREADY_SET,     "DIGI: digi_atomic_keep_lock has been previously called")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_LINE_TIMING_MODE_NOT_CONFIG,      "DIGI: Ethernet Line Timing ressources are not configured")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ACTIVE_ENET_LINE_TIMING_LINK,          "DIGI: They are active ethernet links using DIGI_M1 rate monitor")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_CANNOT_CHANGE_TIMING_MODE,        "DIGI: Cannot change tx timing mode on transparently mapped ENET datapath")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_CANNOT_SET_THROUGH_TIMING_MODE,   "DIGI: Through timing mode is not a valid mode for MAC terminated datapath")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_LINK_NOT_MAC,                     "DIGI: The specified ENET link is not MAC terminated.")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_LINK_NOT_TRANSPARENT,             "DIGI: The specified ENET link is not a transparent mode.")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_LINK_NOT_10G,                     "DIGI: The specified ENET link is not 10G.")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CONTEXT_INCOHERENT,                    "DIGI: Context and Register Space Incoherent")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_INVALID_DATAPATH,                 "DIGI: Invalid ENET datapath")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SYSOTN_UNSUPPORTED,                    "DIGI: API is not supported on SYSOTN")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MCPB_INVALID_PORT,                     "DIGI: Invalid MCPB port")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_OTN_NO_STG4_FRAMER,                    "DIGI: Channel does not have a stage 4 framer")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_OTN_NO_SCHEDULE,                       "DIGI: Channel does not have a schedule")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_NO_OPERATIONAL_DATAPATH,               "DIGI: No Operational Datapath in Crash Restart Scenario") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_NO_SWITCH_DATA,                        "DIGI: Specified resource is not connected to a switch")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PMON_SHARED_MEM_NOT_INIT,              "DIGI: PMON Shared Memory not initialized yet. See digi_pmon_collection_mode_cfg")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_COREOTN_DMX_INVALID_DUMMY_CTXT,        "DIGI: Invalid dummy scheduling context") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_TRIB_SLOT_ASYMMETRIC,                  "DIGI: Tributary slot locations do not match between TX and RX") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MAPPING_NOT_SUPPORTED,                 "DIGI: Mapping mode not supported for in datapath") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_LATENCY_S_EXCEEDED,                    "DIGI: Latency_s parameter setting exceeded") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_DDEG_NOT_CONFIGURED,                   "DIGI: Specified dDEG mode is not configured" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_HW_DDEG_ODU_RCP_DI_ONLY,               "DIGI: Only hardware dDEG mode is supported when ODU RCP DI port is enabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ODU_RCP_DI_HW_DDEG_ONLY,               "DIGI: ODU RCP DI port cannot be enabled when dDeg mode is not hardware" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENH_DDEG_NOT_ENABLED,                  "DIGI: Enhanced dDEG monitoring is not enabled" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_HW_DDEG_NOT_ENABLED,                   "DIGI: Hardware dDEG monitoring shall be properly setup at OTU and ODU level" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SIFD_INCOMPATIBLE_HEADER_TYPE,         "DIGI: rx_type_header must be set to true for this mapping mode") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CPB_MUST_NOT_BE_ACTIVE,                "DIGI: This function must be called before calling digi_cpb_chnl_activate") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_REVISION,                      "DIGI: Retrieved revision is neither A or B")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_UNSUPPORTED_REV_A_FEATURE,             "DIGI: This feature is not supported in revision A")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PROD_APP_NOT_SUPPORTED_CALL,           "DIGI: Action not supported with this product application")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_RGFP_EFCS_TO_COPI_ILLEGAL,             "DIGI: Cannot map MAPPER vs COPI when eFCS monitoring is enabled.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_RGFP_EFCS_TO_PKT_10GE_7_3_GFP_ILLEGAL, "DIGI: Cannot map MAPPER vs 10GE GSUP_7_3 ENET when eFCS monitoring is enabled (or vice versa).") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_ODUFLEX_CBR_CLIENT_BITRATE,    "DIGI: Invalid ODUFlex CBR client bitrate.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_HO_ODU_PAYLOAD_FORMAT,         "DIGI: Cannot add LO ODU to a unchannelized HO ODU") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_INVALID_MIRROR_MODE,              "DIGI: Enet line mirror port only supports 10GE and 40GE MAC modes") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_OHP_INVALID_EXTRACT_MASK,              "DIGI: Invalid OHP extract mask - exactly 10 bits must be set to one")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ETRANS_INVALID_CHNL_HANDLE,            "DIGI: Invalid handle - MAPOTN handle expected")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_INVALID_CHNL_HANDLE,              "DIGI: Invalid handle - ENET handle expected")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_ODU_PAYLOAD_FORMAT,            "DIGI: Invalid ODU payload format")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_HO_MO_CONN_TO_ODUKSW,                  "DIGI: HO or MO channel connected to ODUKSW.")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_USE_ASSOCIATED_ENET_MODE_RX_PMON,          "DIGI: Use associated ENET_RX_PMON mode to broadcast data to RX PMON")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MAPOTN_CHNL_HNDL_NO_ENET,              "DIGI: No ENET provisioned as part of MAPOTN channel handle.  Feature not supported.")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_OTN_OTU_MAINT_SIG,                     "DIGI: TX OTN OTU maint sig requires that a HO ODU segment has been provisioned first using digi_otn_server_ho_prov()") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_OTN_ODU_SUB_CHNL_CNT_MISMATCH,         "DIGI: Incorrect number of LO channels in the HO ODU channel") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_SDK_ERR_BUF_ADDR_ERROR,                    "DIGI: Shared memory buffer address error ") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_SDK_ERR_BUF_ADDR_NULL,                     "DIGI: Shared memory address is null ") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_SDK_ERR_BUF_INIT_ERROR,                    "DIGI: Shared memory initialization error ") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_LO_ODUK_TYPE,                  "DIGI: Invalid LO ODUk type.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_ENET_EPMM_MODE_ONLY,                   "DIGI: For ENET EMAC faults, use: digi_enet_tx_mac_fault_insert_ctrl ") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PKT_SIZE_CHANGE,                       "DIGI: No support for pkt size change across ODUKSW for demapped clients that broadcast/multicast across the DCPB ") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PGMRCLK_RATE_NOT_SUPPORTED,            "DIGI PGMRCLK: Rate not supported." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_HO_PAYLOAD_FORMAT,             "DIGI: Bulk LO provisioning is invalid because HO is unchannelized" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_PGMRCLK_ERR_CONTEXT_INCOHERENT,            "DIGI PGMRCLK: Context and Register Space Incoherent")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_PGMRCLK_ERR_CONTEXT_CLEANUP,               "DIGI PGMRCLK: Register Space needs to be cleaned")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_DCSU_ERR_CONTEXT_INCOHERENT,               "DIGI DCSU: Context and Register Space Incoherent")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_DCSU_ERR_CONTEXT_CLEANUP,                  "DIGI DCSU: Register Space needs to be cleaned")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INCORRECT_MAX_MUXING_LEVEL,            "DIGI: The maximum level of multiplexing is incorrectly initialized")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INCORRECT_ODU_PROV_MODE,               "DIGI: Incorrect ODU provisioning mode")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_STG3B_NOT_PRESENT,                     "DIGI: Stage 3B is not present on this channel. Use lowest order channel for stage 3b.")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PROV_ALL_NOT_AVAILABLE,                "DIGI: Bulk provisioning mode not available")\
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_GSUP43_7_3_INVALID_DATAPATH,           "DIGI: Invalid Mapping mode - expected a GSUP43 7.3 datapath")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_GSUP43_7_3_NOT_EPMM,                   "DIGI: Invalid Mapping mode - expected a GSUP43 7.3 transparent datapath")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_GSUP43_7_3_INVALID_UPI,                "DIGI: Invalid GSUP43 7.3 UPI value")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MCPB_PATH_STILL_CONNECTED,             "DIGI: MCPB Paths Still Provisioned Against ENET Subsystem")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CHNL_NOT_MAPPED,                       "DIGI: Channel must be mapped before calling this function")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_EPMM_INVALID_OP_LEGACY_MODE,           "DIGI: Invalid operation in GSUP43 7.3 legacy mode")   \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_SERDES_10G_PORT_CAGE_INVALID,          "DIGI: 10G Serdes port can only be mapped to 1- or 4-pin cage" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PTP_PKT_FILTER_RESOURCE_EXCEEDED,      "DIGI: Requested configuration exceeds resources available." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_HO_ODU_STILL_PROVISIONED,              "DIGI: HO ODU channel associated with LINEOTN channel is still provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_FW_RESTART_NOT_SUPPORTED,              "DIGI: Firmware cannot be reload on the current active firmware version" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_OTN_ODU_RX_MAINT_SIG,                  "DIGI: For RFRMs at the egress of the ODUKSW, it's illegal to provision a maintenance signal before an ODUKSW connection is made") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_NOT_PROVISONED,                        "DIGI: Runtime API cannot on this handle since it is not provisioned state.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_BACKUP_STILL_PROV,                     "DIGI: Inbound Fault Replacement Still Provisioned" ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_SW_FW_DEPENDANCY,                          "DIGI: Current SW version requires a more recent FW version") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_INVALID_PATTERN_CFG,                   "DIGI: Unexpected pattern type configuration") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MPMA_NOT_ACTIVATED,                    "DIGI: MPMA startup procedure not complete, feature configuration not available.") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_FAULT_ACTIVATED,                       "DIGI: ENET Fault insertion activated, need to turn off before deprovision") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_EMPTY_CONTEXT,                         "DIGI: Basic Context information is missing. Possible error reading in context") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_TX_BCW_CHANNEL_ID_AMBIGUOUS,           "DIGI: The Tx BCW Channel ID is used by more than one channel and is not supported in the flow control calendar") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_NO_LO_ODU_PROVISIONED,                 "DIGI: No provisioned LO ODU channels in the HO") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_UNUSED_TRIBSLOT,                       "DIGI: Provided tributary slots are not in use") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_CHANNEL_ID_UNASSIGNED,                 "DIGI: Channel ID unassigned for this tributary slot") \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_MCPB_10GE_LARGE_FIFO_ALLOCATED,        "DIGI:  Only one large FIFO may be specified per direction." ) \
    PMC_LOG_ERR_PAIR_ENTRY_CREATE(DIGI_ERR_PGMRCLK_FPI_NOT_SYNCHRONIZED,          "DIGI PGMRCLK: FPI Generator not synchronized to the reference pulse." ) \
    
/*! The following creates the ERR/LOG indexes from above by pulling out the enum
    portion of the LOG_ERR_TABLE above */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) enum,
enum DIGI_LOG_ERR_INDEX
{
    DIGI_LOG_ERR_TABLE_BASE = (PMC_MID_DIGI << 16),
    DIGI_LOG_ERR_TABLE
    DIGI_LOG_ERR_COUNT
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE


#ifdef PMC_LOG_STRINGS
/*! Errors with strings */
extern const char DIGI_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF];
#else 
/*! Errors without strings */
extern const char DIGI_LOG_ERR_STRINGS[1][1];
#endif /* PMC_SW_SIMULATION */
#ifdef __cplusplus
}
#endif

#endif /* _DIGI_API_H */

/*
** end of file
*/
