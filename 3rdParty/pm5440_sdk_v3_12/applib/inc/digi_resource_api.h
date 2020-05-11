/*******************************************************************************
*  T (C) 2011 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*   DESCRIPTION : This file contains definitions for C functions for
*   configuring SIFD features.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _DIGI_RESOURCES_API_H
#define _DIGI_RESOURCES_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "digi_api.h"

/*
** Constants
*/
#define DIGI_XFI_PIN_NUM                               12
#define DIGI_SFI51_INTF_NUM                             2

#define DIGI_SYS_ODU_MO_FRAMER_NUM                     12

#define DIGI_ILKN_INTF_NUM                              2
#define DIGI_ILKN_CHNL_NUM                            254

#define DIGI_ODUKSW_PORT_NUM                            4
#define DIGI_CPB_PORT_NUM                               6
/*
 * Number of CPB switch channels is based on the defintion of 6 DCPB DPI
 * port capacities defined as:
 *    Port 0:     150  ILLKN 1
 *    Port 1:     150  ILKN 2
 *    Port 2:      12  CBRC
 *    Port 3:      96  MAPOTN
 *    Port 4:      12  ENET_LINE
 *    Port 5:      12  ENET_SYS
 */
#define DIGI_CPB_SWITCH_TOTAL_CHANNELS                432

#define DIGI_CPB_SWITCH_PORT0_OFFSET                    0  /*ILKN 1 150 */
#define DIGI_CPB_SWITCH_PORT1_OFFSET                  150  /*ILKN 2 150 */
#define DIGI_CPB_SWITCH_PORT2_OFFSET                  300  /*CBRC   12  */
#define DIGI_CPB_SWITCH_PORT3_OFFSET                  312  /*MAPOTN 96  */
#define DIGI_CPB_SWITCH_PORT4_OFFSET                  408  /*E-LINE 12  */
#define DIGI_CPB_SWITCH_PORT5_OFFSET                  420  /*E-SYS  12  */

  /*
 * Number of ODU switch channels is based on the defintion of 4 OCPB DPI
 * port capacities defined as:
 *    Port 0:     104  ILKN 1
 *    Port 1:     104  ILKN 2
 *    Port 2:      96  COREOTN
 *    Port 3:      96  MAPOTN
 */
#define DIGI_ODUK_SWITCH_TOTAL_CHANNELS                400

#define DIGI_ODU_SWITCH_PORT0_OFFSET                    0  /*ILKN 1 104 */
#define DIGI_ODU_SWITCH_PORT1_OFFSET                  104  /*ILKN 2 104 */
#define DIGI_ODU_SWITCH_PORT2_OFFSET                  208  /*COREOTN 96 */
#define DIGI_ODU_SWITCH_PORT3_OFFSET                  304  /*MAPOTN  96 */

#define DIGI_RESOURCE_MULTI_DEST_MAX                  12

#define INDEX_UNDEFINED                               0xFFFF

/*! Define a void type that can be used as a pointer */
typedef void digi_chnl_hndl_t; 

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: digi_handle_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining configured and provisioned
*   resource handle type.\n\n
*
*   The element terms have a syntax:\n
*      DIGI_CHANNEL_HANDLE_ID_<handle_type>
*
* ELEMENTS:
*    DIGI_CHANNEL_HANDLE_ID_LINEOTN_T     - LINEOTN channel, (digi_otn_server_chnl_t)
*    DIGI_CHANNEL_HANDLE_ID_SYSOTN_T      - SYSOTN channel, (digi_otn_server_chnl_t)
*    DIGI_CHANNEL_HANDLE_ID_LINE_SERDES_T - Line serdes handle used for CBRC,
*                                           ENET Line or ENET Sys.
*    DIGI_CHANNEL_HANDLE_ID_SYS_SERDES_T  - System serdes handle used for 
*                                           CBRC, ENET Line or ENET Sys.
*    DIGI_CHANNEL_HANDLE_ID_SFI51_SERDES_T - SFI 51 serdes handled used for
*                                            CBRC, ENET Line or ENET Sys.
*    DIGI_CHANNEL_HANDLE_ID_HO_ODU_T      - HO ODU framer, (digi_otn_odu_chnl_t)
*    DIGI_CHANNEL_HANDLE_ID_MO_ODU_T      - MO ODU frame, (digi_otn_odu_chnl_t)
*    DIGI_CHANNEL_HANDLE_ID_LO_ODU_T      - LO ODU frame, (digi_otn_odu_chnl_t)
*    DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T  - CBRC channel, (digi_cbr_client_chnl_t)
*    DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T   - ENET_LINE link, (digi_enet_client_chnl_t)
*    DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T    - ENET_SYS link, (digi_enet_client_chnl_t)
*    DIGI_CHANNEL_HANDLE_ID_MAPPER_T      - MAPPER channel, (digi_mapper_chnl_t)
*    DIGI_CHANNEL_HANDLE_ID_ILKN_CHNL_T   - ILKN channel, (digi_ilkn_chnl_t)
*    DIGI_CHANNEL_HANDLE_ID_ILKN_INTF_T   - ILKN interface, (digi_ilkn_intf_t)
*
*******************************************************************************/

typedef enum digi_handle_id_t
{
    DIGI_CHANNEL_HANDLE_ID_LINEOTN_T = 0,
    DIGI_CHANNEL_HANDLE_ID_SYSOTN_T,
    DIGI_CHANNEL_HANDLE_ID_LINE_SERDES_T,
    DIGI_CHANNEL_HANDLE_ID_SYS_SERDES_T,
    DIGI_CHANNEL_HANDLE_ID_SFI51_SERDES_T,
    DIGI_CHANNEL_HANDLE_ID_HO_ODU_T,
    DIGI_CHANNEL_HANDLE_ID_MO_ODU_T,
    DIGI_CHANNEL_HANDLE_ID_LO_ODU_T,
    DIGI_CHANNEL_HANDLE_ID_CBRC_STREAM_T,
    DIGI_CHANNEL_HANDLE_ID_ENET_LINE_T,
    DIGI_CHANNEL_HANDLE_ID_ENET_SYS_T,
    DIGI_CHANNEL_HANDLE_ID_MAPPER_T,
    DIGI_CHANNEL_HANDLE_ID_ILKN_1_T,
    DIGI_CHANNEL_HANDLE_ID_ILKN_2_T,
    DIGI_CHANNEL_HANDLE_ID_ILKN_CHNL_T,
    DIGI_CHANNEL_HANDLE_ID_ILKN_INTF_T,

    LAST_DIGI_HANDLE_ID
} digi_handle_id_t;


/*******************************************************************************
* ENUM: digi_resource_next_handle_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type representing the next field in the 
*   digi_resource_handle_summary_t structure to be accessed to find
*   the next piece connected to the current element.
*
* ELEMENTS:
*
*    DIGI_LINE_SERDES_CFG            - Represents the line_serdes_cfg
*    DIGI_SYS_SERDES_CFG             - Represnets the sys_serdes_cfg
*    DIGI_SFI51_SERDES_CFG           - Represents the sfi_serdes_cfg
*    DIGI_LINE_OTN_HO_CONTAINER      - Represents line_otn_ho_container_cfg
*    DIGI_LINE_OTN_MO_CONTAINER      - Represents line_otn_mo_subcontainer_cfg
*    DIGI_LINE_OTN_LO_CONTAINER      - Represents line_otn_lo_subcontainer_cfg
*    DIGI_SYS_OTN_HO_CONTAINER      - Represents sys_otn_ho_container_cfg  
*    DIGI_SYS_OTN_MO_CONTAINER      - Represents sys_otn_stage_1_subcontainer_cfg
*    DIGI_ILKN_INTF_CONTEXT         - Represents ilkn_intf_cfg
*    DIGI_ILKN_CHNL_CONTEXT         - Represents ilkn_chnl_cfg
*    DIGI_OTN_MAPPER                - Represents mapper_cfg
*    DIGI_ODUK_SWITCH               - Represents oduksw_cfg
*    DIGI_CPB_SWITCH                - Represents cpb_cfg
*    DIGI_ILKN_1                    - ILKN 1
*    DIGI_ILKN 2                    - ILKN 2
*    DIGI_NEXT_UNCONNECTED          - Setting of the current structure is 
*                                     not mapped to another subsystem.
*
*******************************************************************************/
typedef enum digi_resource_next_handle_t
{
    DIGI_LINE_SERDES_CFG, 
    DIGI_SYS_SERDES_CFG, 
    DIGI_SFI51_SERDES_CFG, 
    DIGI_LINE_OTN_HO_CONTAINER,
    DIGI_LINE_OTN_MO_CONTAINER,
    DIGI_LINE_OTN_LO_CONTAINER,
    DIGI_SYS_OTN_HO_CONTAINER,
    DIGI_SYS_OTN_MO_CONTAINER,
    DIGI_ILKN_INTF_CONTEXT,
    DIGI_ILKN_CHNL_CONTEXT,
    DIGI_OTN_MAPPER,
    DIGI_ODUK_SWITCH,
    DIGI_CPB_SWITCH,
    DIGI_ILKN_1,
    DIGI_ILKN_2,

    LAST_DIGI_RESOURCE_HANDLE
} digi_resource_next_handle_t;

/*******************************************************************************
* ENUM: digi_otn_time_slot_occupied_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enumerated type representing the usage associated with tributary slots.
*
* ELEMENTS:
*  DIGI_OTN_TIME_SLOT_OCCUPIED_ALL_FREE - Tributary time slot is not
*                                         currently used by any OTN handle.
*  DIGI_OTN_TIME_SLOT_OCCUPIED_MIXED_USE - Time slot has mixed used. It may
*                                          be used by multiple OTN chanels or
*                                          the time slot may be partially in
*                                          use by a OTN channel.
*  DIGI_OTN_TIME_SLOT_OCCUPIED_ONE_CHANNEL - Time slot is in use by only one
*                                            OTN channel.
***************************************************************************/
typedef enum digi_otn_time_slot_occupied_t
{ 
    DIGI_OTN_TIME_SLOT_OCCUPIED_ALL_FREE = 0, 
    DIGI_OTN_TIME_SLOT_OCCUPIED_MIXED_USE, 
    DIGI_OTN_TIME_SLOT_OCCUPIED_ONE_CHANNEL, 

    LAST_DIGI_OTN_TIME_SLOT_OCCUPIED 
} digi_otn_time_slot_occupied_t;


/*******************************************************************************
* ENUM: digi_mapotn_conn_use_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the interface for data retrieval
*   for a MAPOTN channel handle.\n\n
*
*
* ELEMENTS:
*    DIGI_MAPOTN_CONN_USE_NOT_MAPOTN    - Channel handle is not MAPOTN
*    DIGI_MAPOTN_CONN_USE_ODUKSW_INTF   - ODUKSW interface data for MAPOTN
*    DIGI_MAPOTN_CONN_USE_CPB_INTF      - CPB interface data for MAPOTN
*
*******************************************************************************/

typedef enum digi_mapotn_conn_use_t
{
    DIGI_MAPOTN_CONN_USE_NOT_MAPOTN = 0,
    DIGI_MAPOTN_CONN_USE_ODUKSW_INTF,
    DIGI_MAPOTN_CONN_USE_CPB_INTF,   

    LAST_DIGI_MAPOTN_CONN_USE
} digi_mapotn_conn_use_t;


/*******************************************************************************
* ENUM: digi_direction_check_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enumerated type representing the direction of data path is to be checked
*  with respect to its operational level/
*
*
* ELEMENTS:
*    DIGI_DIRECTION_TX_ONLY  - Operational check is in the transmit direction
*                              and the Rx is not currently active
*    DIGI_DIRECTION_RX_ONLY  - Operational check is in the receive diection
*                              and the TX is not currently configured
*    DIGI_DIRECTION_TX_RX    - Opeational check is done in both Rx and Tx 
*                              direction. Flag is used at higher levels to
*                              indicate both directions require checking.
*
*******************************************************************************/

typedef enum digi_direction_check_t
{
    DIGI_DIRECTION_TX_ONLY,
    DIGI_DIRECTION_RX_ONLY,
    DIGI_DIRECTION_TX_RX,

    LAST_DIGI_DIRECTION_CHECK
} digi_direction_check_t;


/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE:  digi_serdes_port_intf_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining configured or provisioned resources against an Line or
*   System side interface XFI pin or an SFI5.1 Port.
*
* ELEMENTS:
*  port_ctxt_port        - Pointer to the serdes port handle.
*  port_uid              - Physical port identifier associated with the
*                          port handle and used when creating the handle
*  chnl_handle_type      - Type of connection this serdes port handle is
*                          connected to. That is LINEOTN, ENET_LINE, CBRC 
*                          on the line side.
*  chnl_handle_ptr       - Pointer to the actual element this serdes port
*                          handle is connected to. For example if the
*                          handle_connection_type is LINEOTN then this
*                          points to 
*                          digi_handle->handle_pool.otn_server_chnl element
*  next_resource_handle  - The structure to go to next in the
*                          digi_resource_handle_summary_t to find the 
*                          handle/subsystem this is mapped to.
*  next_handle_index     - Array index into the structure pointed to by
*                          next_resource_handle 
*  tx_path_is_valid      - Flag indicating the operational validity of this
*                          serdes element in the TX direction. Set in all 
*                          cases although used specifically for ENET 
*                          LINE/SYS and CBRC Serdes connected resources.
*  rx_path_is_valid      - Flag indicating the operational validity of this
*                          serdes element in the RX direction. Set in all 
*                          cases although used specifically for ENET 
*                          LINE/SYS and CBRC Serdes connected resources.
*  path_verified         - Flag indicating that this serdes port handle 
*                          has been vderfiied as a result of checking from
*                          another handle. For example a Line serdes 
*                          connected to a System serdes handle.
*
*******************************************************************************/
typedef struct digi_serdes_port_intf_ctxt_t
{
    digi_serdes_port_t *port_ctxt_ptr;
    UINT32 port_uid;
    digi_handle_id_t chnl_handle_type;
    void *chnl_handle_ptr;
    digi_resource_next_handle_t next_resource_handle;
    UINT16 next_handle_index;
    BOOL8 tx_path_is_valid;
    BOOL8 rx_path_is_valid;
    BOOL8 path_verified;
} digi_serdes_port_intf_ctxt_t;

/*******************************************************************************
* STRUCTURE:  digi_otn_container_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure to the particular COREOTN high order/mid order/low order 
*  subsystem for either the line or system side. Handles connections from
*  LINEOTN or SYSOTN.
*
* ELEMENTS:
*   chnl_id            - Channel identifer for this odu resource handle. Is 
*                        the index into the OTN subcontainer for this ho/mo/lo
*                        subsystem.
*   handle_type        - Type of digi_handle_id_t of this resource handle.
*   chnl_handle_ptr    - Reference to the ho/lo/mo order channel pointer  
*                        this resource handle points to.
*   payload_mode       - Payload mode 
*   tx_trib_slot_mask  - Transmit path tributary slots the low order container 
*                        uses in the higher order payload.
*   rx_trib_slot_mask  - Receive path tributary slots the low order container 
*                        uses in the higher order payload.
*   lo_chnl_entries    - Number of lower order channels this odu struct is
*                        connected to. If 0 then next_resource_handle 
*                        should be the ODUK Switch and the payload_mode is
*                        UNCHANNELIZED.
*   next_resource_handle - Next structure in the digi_resource_handle_summary_t
*                          this container is connected to.
*   next_handle_index    - Array index into the structure pointed to by
*                          next_resource_handle 
*   next_associate_handle - Whether there is another handle at this ODU level
*                           in the chain. For Mid or Low levels the next
*                           associate handle is the next companion at this
*                           level sharing the same higher order (parent's) 
*                           channel. At  the High Order level the next 
*                           associate handle is not related but is provided 
*                           to facilitate searching for and determinning 
*                           activation of high order channels.
*   next_associate_index - The indice of the next ODU level at this level 
*                          in the chain. At the Mid and Low order ODU level
*                          the number of lo_chnl_entries of the parent is the
*                          number of associated handles in the chain.
*   tx_path_is_valid      - Flag that indicates that this level of the OTN
*                          container the path is valid for the transmit
*                          direction. This may be an end to end fully 
*                          operational path  or a channelized  High or Mid 
*                          order resource.
*   rx_path_is_valid     - Flag that indicates that this level of the OTN
*                          container the path is valid for the receive
*                          direction. This may be an end to end fully 
*                          operational path  or a channelized  High or Mid 
*                          order resource.
*******************************************************************************/
typedef struct digi_otn_container_cfg_t
{
    digi_resource_next_handle_t parent_handle_type;
    UINT16 parent_handle_index;

    digi_handle_id_t handle_type;
    digi_otn_odu_chnl_t *chnl_handle_ptr; 
    digi_odu_line_payload_t payload_mode;
    UINT32 tx_trib_slot_mask[3];
    UINT32 rx_trib_slot_mask[3];
    UINT32 lo_chnl_entries;

    digi_resource_next_handle_t next_resource_handle;
    UINT16 next_handle_index;

    digi_resource_next_handle_t next_associate_handle;
    UINT16 next_associate_index;

    BOOL8 tx_path_is_valid;
    BOOL8 rx_path_is_valid;
} digi_otn_container_cfg_t;

/*******************************************************************************
* STRUCTURE:  digi_otn_mapper_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Configguration structure for the MAPOTN channel handles. Holds the actual
*  channels handles and the ODUK/CPB switch indices this mapotn channel may
*  be connected to. A value of 0 for either in the switch indices indicates
*  no mapper channel is connect to that switch.
*
* ELEMENTS:
*   mapper_handle_ptr   - Reference to the mapper channel handle stored in 
*                         the digi_handle.
*   oduk_switch_index   - Indice into the ODUK switch if this mapper channel
*                         streams is routed through the ODUK switch.
*   cpb_switch_index    - Indice into the CPB switch if this mapper channel
*                         stream is routed through the CPB switch. 
*
*******************************************************************************/
typedef struct digi_otn_mapper_cfg_t
{
    digi_mapper_chnl_t *mapper_handle_ptr;
    UINT16 oduk_switch_index;
    UINT16 cpb_switch_index;
} digi_otn_mapper_cfg_t;

/*******************************************************************************
* STRUCTURE:  digi_ilkn_chnl_desc_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Channel descriptor for the ILKN channel set when it was configured via
*  digi_ilkn_chnl_cfg
*
* ELEMENTS:
*   target           - Subsystem to source/sink data for the channel.
*                      See digi_ilkn_chnl_target_t for more information.
*   tx_bcw_chnl      - Defines the burst control word channel that will be
*                      associated with the outbound channel. Used for 
*                      output_port ILKN1/ILKN2 only.
*   tx_2b_header     - Defines the 2B header for source ODUKSW channels.
*   tx_4b_header     - Defines the 4B header for source ODUKSW channels.
*   rx_bcw_chnl      - Defines the burst control word channel that will be
*                      associated with the inbound channel
*   rx_2b_header     - Defines the 2B header for sink ODUKSW channels.
*   rx_4b_header     - Defines the expected 4B header for sink ODUKSW 
*                      channels.
*
*******************************************************************************/
typedef struct digi_ilkn_chnl_desc_t
{
    digi_ilkn_chnl_target_t  target;
    UINT32  tx_bcw_chnl;
    UINT32  tx_2b_header;
    UINT32  tx_4b_header;
    UINT32  rx_bcw_chnl;
    UINT32  rx_2b_header;
    UINT32  rx_4b_header;
} digi_ilkn_chnl_desc_t;


/*******************************************************************************
* STRUCTURE:  digi_ilkn_chnl_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* FIELDS:
*   ilkn_chnl_handle_ptr - Reference the ILKN channel handle.
*   next_resource_handle - Next structure in the 
*                          digi_resource_handle_summary_t this container is
*                          connected.
*   next_handle_index    - Array index into the structure pointed to by
*                          next_resource_handle 
*******************************************************************************/
typedef struct digi_ilkn_chnl_ctxt_t
{
    digi_ilkn_chnl_t *ilkn_chnl_handle_ptr;
    digi_ilkn_chnl_desc_t ilkn_chnl_desc;
    digi_resource_next_handle_t next_resource_handle;
    UINT16 next_handle_index;
} digi_ilkn_chnl_ctxt_t;


/*******************************************************************************
* STRUCTURE:  digi_switch_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Data element representing the connection and activation state of a 
*   provisioned resource at the ODUKSW or CPB switches. All data is relative
*   to the subsystem defined for the provisioned_handle_type. \n\n
*
*   Each index used for these structures is derived based on the 
*   port/channel of the switch that each of the subsystem uses with respect
*   the the system it is sending data to or receiving data from.  There is
*   one defined in the digi_handle_resource_summary_t for CPB and ODUKSW
*
*   The provisioned handle represents the subsystem being examined. The 
*   source is the subsystem providing data to this subsystem and the
*   destination is where this subsystem is sending data. There can be 
*   multiple destinations but only one source.
*
*   When num_dest is 2 or greater, the subsystem is performing a combination
*   of broadcast and multicast replication of the signal within the switch.
*
*   To identify the set of destinations for a provisioned_handle_type/index,
*   the array of digi_switch_ctxt_t would be searched for cases where 
*   source_handle_type/index matches provisioned_handle_type/index. The 
*   activation state of these secondary connections can be identifier from
*   the source_activated field.
*
*   Note these elements are applied to two seperate arrays for ODUKSW and CPB.
*   No direct connection may exist between ODUKSW and CPB.  Searching for 
*   multiple destinations must be performed with the array of the switch
*   type of interest.\n\n
*
*   In cases where a provisioned resource is not mapped to a source or 
*   destination, the handle type for these will be specified as
*   LAST_DIGI_HANDLE_ID.

*
* FIELDS:
*   provisioned_handle_type  - The provisioned subsystem using the switch
*                              port channel.
*   provisioned_handle_index - Indice into the digi_resource_handle_summary_t
*                              structure for this subsystem. The 
*                              provisioned_handle_type defines the field in
*                              that structure and this field defines the 
*                              index.
*   source_handle_type       - Subsystem providing data to the provisioned
*                              handle type. If this value has a valid handle
*                              type, ie not LAST_DIGI_RESOURCE_HANDLE, then
*                              it is implicitly mapped.
*   source_handle_index      - Indice into the digi_resource_handle_summary_t
*                              representing the data associated with this 
*                              source subsystem.
*   source_activated         - Whether the provision handle is the source of
*                              input to another resource, namely the 
*                              destionation resource.
*   primary_dest_handle_type - Subsystem that is receiving data from this
*                              provisioned subsystem.  In a one to one 
*                              mapping this will be the same as the 
*                              source_handle_type.
*   primary_dest_index       - Indice into the digi_resource_handle_summary_t
*                              structure.
*   dest_activated           - Whether the provisioned handle type has been
*                              activated and is receiving a signal from the
*                              source handle type.
*   associate_dest_handle_type - Set if broadcast/multicast is enabled and
*                                represents the next destination associated
*                                with this set of destinations channels.
*   associate_dest_index     - Index of the associate destination handle.
*   num_dest                 - Number of destinations this provisioned 
*                              handled type and index will appear as the 
*                              source handle/index for multiple subsystems
*                              throughout this switch array
*******************************************************************************/
typedef struct digi_switch_ctxt_t
{
    digi_resource_next_handle_t provisioned_handle_type;
    UINT16 provisioned_handle_index;

    digi_resource_next_handle_t source_handle_type;
    UINT16 source_handle_index;
    BOOL8 source_activated;

    digi_resource_next_handle_t primary_dest_handle_type; 
    UINT16 primary_dest_index;
    BOOL8 dest_activated;

    digi_resource_next_handle_t associate_dest_handle_type; 
    UINT16 associate_dest_index;

    UINT16 num_dest;

} digi_switch_ctxt_t;


/*******************************************************************************
* STRUCTURE:  digi_chnl_hndl_conn_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Structure to define the data source or destination for a connection at the
*  ODUKSW or CPB subsystem.
*
* ELEMENTS:
*   chnl_hndl_type  - enumerated type defining the type of channel hande.
*   *chnl_hndl_ptr  - NULL if unused, otherwise pointer to DIGI context 
*                     channel handle
*   chnl_primary    - Indicator if first mapped source or destination.  TRUE
*                     if first otherwise FALSE.
*   chnl_activate   - Indicator of the channel activation state.  TRUE if
*                     activated otherwise FALSE.
*
*
*
*******************************************************************************/
typedef struct digi_chnl_hndl_conn_t
{
    digi_handle_id_t chnl_hndl_type;
    digi_chnl_hndl_t *chnl_hndl_ptr;
    BOOL8            chnl_primary;
    BOOL8            chnl_activated;
} digi_chnl_hndl_conn_t;


/*******************************************************************************
* STRUCTURE:  digi_resource_handle_summary_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining resource handles of the configured and provisioned
*   resourcs within the DIGI device.
*
* FIELDS:
*   line_serdes_cfg  - Line sides serde port configuration.
*   sys_serdes_cfg   - System side serdes port configuration
*   sfi51_serdes_cfg - SFI 51 side serdes port configuration
*   mapper_cfg       - Channel handles within the MAPOTN subsystem
*   oduksw_cfg       - Array of 396 elements representings the 4 ODUK switch
*                      ports and underlying channels within those ports.
*   cpbsw_cfg        - Array 416 elements based on the 6 CPB swotch ports and
*                      channels associated with each port.
*   line_otn_ho_container_cfg - Data related to the High Order line side 
*                               coreotn subsystem.
*   line_otn_mo_container_cfg - Data related to the Mid Order line side 
*                               coreotn subsystem.
*   line_otn_lo_container_cfg - Data related to the Low Order line side 
*                               coreotn subsystem.
*   sys_otn_ho_container_cfg - Data related to the High Order System side 
*                              coreotn subsystem. 
*   sys_otn_mo_container_cfg - Data related to the Mid Order System side 
*                              coreotn subsystem. 
*   ilkn_inst_cfg            - The interlaken interface configuration.  
*                              Defined as 0 for ILKN1 and 1 for ILKN2. Value 
*                              of the array index can be used as the index
*                              into interface of digi_ilkn_chnl_ctxt_t.
*   digi_ilkn_chnl_ctxt_t    - ILKN 1 and 2 subsystem channel handles.
*   datapath_release         - Flag that tracks whether any aspect of a 
*                              datapath has been released. Used primarily
*                              across inter-switch data paths where one side
*                              is broadcast/multicast and the other is
*                              uni-directional but a valid data path.
*   force_release            - Override flag indicating that the particular
*                              datapath being operated on will be released.
*                              Set when a serdes port or provisioned handle
*                              has become invalid making the entire data path
*                              invalid.
*******************************************************************************/

typedef struct digi_resource_handle_summary_t
{
    digi_serdes_port_intf_ctxt_t line_serdes_cfg[DIGI_XFI_PIN_NUM];
    digi_serdes_port_intf_ctxt_t sys_serdes_cfg[DIGI_XFI_PIN_NUM];
    digi_serdes_port_intf_ctxt_t sfi51_serdes_cfg[DIGI_SFI51_INTF_NUM];

    digi_otn_mapper_cfg_t mapper_cfg[DIGI_MAPOTN_CHNL_MAX];
    digi_switch_ctxt_t oduksw_cfg[DIGI_ODUK_SWITCH_TOTAL_CHANNELS];
    digi_switch_ctxt_t cpbsw_cfg[DIGI_CPB_SWITCH_TOTAL_CHANNELS];

    digi_otn_container_cfg_t line_otn_ho_container_cfg[DIGI_OTN_SERVER_HO_CHNL_MAX];
    digi_otn_container_cfg_t line_otn_mo_container_cfg[DIGI_OTN_SERVER_MO_CHNL_MAX]; 
    digi_otn_container_cfg_t line_otn_lo_container_cfg[DIGI_OTN_SERVER_LO_CHNL_MAX]; 
    digi_otn_container_cfg_t sys_otn_ho_container_cfg[DIGI_OTN_SERVER_HO_CHNL_MAX];
    digi_otn_container_cfg_t sys_otn_mo_container_cfg[DIGI_OTN_SERVER_MO_CHNL_MAX];
    digi_ilkn_link_t *ilkn_inst_cfg[DIGI_ILKN_INTF_NUM];
    digi_ilkn_chnl_ctxt_t ilkn_chnl_cfg[DIGI_ILKN_INTF_NUM][DIGI_ILKN_CHNL_NUM];
    BOOL8 datapath_release;
    BOOL8 force_release;
} digi_resource_handle_summary_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC PMC_ERROR digi_resource_state_get(digi_handle_t *digi_handle,
                                         digi_resource_handle_summary_t *digi_resource_handle);


PUBLIC void digi_resource_handle_print(digi_handle_t *digi_handle,
                                       digi_resource_handle_summary_t *digi_resource_handle);

/*
 * ASK related operators. The first set of get operators return channel 
 * handles given an identifier for that handle. In the case of the serdes
 * port handle that is the port_uid for others it is the parent handle.  that
 * is the serdes port handle can be used to retrieve the lineotn/sysotn, 
 * enet line/sys or cbrc handle.
 */
PUBLIC PMC_ERROR digi_serdes_port_init_get(digi_handle_t *digi_handle,
                                           UINT32 port_uid,
                                           digi_serdes_port_t **port_ctxt_pptr);

PUBLIC void digi_device_init_get(digi_handle_t *digi_handle,
                                 digi_prod_app_t *prod_app_ptr,
                                 digi_otn_mux_stages_t *mux_stages_ptr,
                                 digi_ilkn_link_t **ilkn_1_ctxt_pptr,
                                 digi_ilkn_link_t **ilkn_2_ctxt_pptr);

PUBLIC PMC_ERROR digi_serdes_port_chnl_hndl_get(digi_handle_t *digi_handle,
                                                digi_serdes_port_t *port_ctxt_ptr,
                                                digi_handle_id_t *chnl_hndl_type_ptr,
                                                digi_chnl_hndl_t **chnl_hndl_pptr,
                                                digi_otn_odu_chnl_t **ho_chnl_hndl_pptr);

PUBLIC PMC_ERROR digi_ho_subcontainer_chnl_hndl_get(digi_handle_t *digi_handle,
                                                    digi_handle_id_t ho_chnl_hndl_type,
                                                    digi_otn_odu_chnl_t *ho_chnl_hndl_ptr,
                                                    digi_handle_id_t *lo_chnl_hndl_type_ptr,
                                                    digi_otn_odu_chnl_t *lo_chnl_hndl_ptr[DIGI_OTN_SERVER_MAX_TRIBSLOT]);

PUBLIC PMC_ERROR digi_ho_subcontainer_trib_slot_chnl_hndl_get(digi_handle_t *digi_handle,
                                                              digi_handle_id_t ho_chnl_hndl_type,
                                                              digi_otn_odu_chnl_t *ho_chnl_hndl_ptr,
                                                              UINT32 tx_trib_slot[3],
                                                              UINT32 rx_trib_slot[3],
                                                              digi_otn_time_slot_occupied_t *slot_use_ptr,
                                                              digi_handle_id_t *lo_chnl_hndl_type_ptr,
                                                              digi_otn_odu_chnl_t *lo_chnl_hndl_ptr[DIGI_OTN_SERVER_MAX_TRIBSLOT]);

PUBLIC PMC_ERROR digi_otn_mapper_prov_hndl_all_get(digi_handle_t *digi_handle,
                                                   digi_mapper_chnl_t *map_chnl_hndl_ptr[DIGI_MAPOTN_CHNL_MAX]);


PUBLIC PMC_ERROR digi_ilkn_chnl_prov_hndl_get(digi_handle_t *digi_handle,
                                              digi_handle_id_t ilkn_handle_id,
                                              UINT32 tx_ilkn_bcw_chnl,
                                              UINT32 rx_ilkn_bcw_chnl,
                                              digi_ilkn_chnl_t **ilkn_chnl_hndl_pptr);

PUBLIC PMC_ERROR digi_ilkn_chnl_prov_2b_header_hndl_get(digi_handle_t *digi_handle,
                                                        digi_handle_id_t ilkn_handle_id,
                                                        UINT32 tx_2b_header,
                                                        UINT32 rx_2b_header,
                                                        digi_ilkn_chnl_t **ilkn_chnl_hndl_pptr);

/*
 * ASK operators that given a channel handle will return all the 
 * configuration parameters used when the handle was provisioned.
 */
PUBLIC void digi_otn_server_prov_get(digi_handle_t *digi_handle,
                                     digi_otn_server_chnl_t *otn_server_ptr,
                                     digi_otn_server_otuk_t *otuk_mode_ptr,
                                     digi_otn_server_fec_t *fec_mode_ptr,
                                     digi_sfis_deskew_type_t *deskew_type_ptr);

PUBLIC void digi_otn_server_ho_prov_get(digi_handle_t *digi_handle,
                                        digi_otn_server_chnl_t *ho_chnl_ptr,
                                        digi_otn_cfg_mode_t *cfg_mode_ptr,
                                        digi_odu_line_payload_t *payload_mode_ptr);

PUBLIC void digi_otn_server_lo_prov_get(digi_handle_t *digi_handle,
                                        digi_otn_odu_chnl_t *lo_chnl_ptr,
                                        digi_otn_cfg_mode_t *cfg_mode_ptr,
                                        digi_oduk_t *oduk_type_ptr,
                                        DOUBLE *client_bitrate_ptr,
                                        digi_otn_mapping_type_t *map_mode_ptr,
                                        UINT32 trib_slot_mask[3],
                                        digi_odu_line_payload_t *payload_mode_ptr);

PUBLIC void digi_enet_prov_get(digi_handle_t *digi_handle,
                               digi_enet_client_chnl_t *enet_chnl_ptr,
                               digi_enet_mode_t *enet_mode_ptr);


PUBLIC void digi_cbr_client_prov_get(digi_handle_t *digi_handle,
                                     digi_cbr_client_chnl_t *cbrc_chnl_ptr,
                                     digi_cbr_client_mode_t *cbrc_mode_ptr);

PUBLIC void digi_otn_mapper_prov_get(digi_handle_t *digi_handle,
                                     digi_mapper_chnl_t *map_chnl_ptr,
                                     digi_mapper_primary_group_t *primary_group_ptr,
                                     digi_mapper_secondary_group_t *secondary_group_ptr,
                                     digi_mapping_mode_t *mapping_mode_ptr,
                                     BOOL8 *enet_pmon_ptr,
                                     digi_oduk_t *odu_data_ptr,
                                     UINT32 *odu_flex_data_ptr);

PUBLIC void digi_ilkn_chnl_prov_get(digi_handle_t *digi_handle,
                                    digi_ilkn_chnl_t *ilkn_chnl_ptr,
                                    BOOL8 *is_oduk_switch,
                                    UINT8 *rate_ptr,
                                    BOOL8 *enet_pmon,
                                    BOOL8 *tx_type_header,
                                    BOOL8 *rx_type_header,
                                    BOOL8 *rx_fc_reach_extend);



PUBLIC void digi_resource_flow_control_calendar_get(digi_handle_t *digi_handle,
                                                    digi_ilkn_intf_t ilkn_type,
                                                    digi_sifd_ilkn_calendar32_t *tx_ilkn_fc_ptr,
                                                    digi_sifd_ilkn_calendar32_t *rx_ilkn_fc_ptr);

PUBLIC PMC_ERROR digi_chnl_hndl_map_act_state_get(digi_handle_t *digi_handle,
                                                  digi_handle_id_t chnl_hndl_type,
                                                  digi_chnl_hndl_t *chnl_hndl_ptr,
                                                  digi_mapotn_conn_use_t mapotn_use,
                                                  digi_chnl_hndl_conn_t *source_ptr,
                                                  BOOL8 *companion_source_ptr,
                                                  digi_chnl_hndl_conn_t *primary_dest_ptr,
                                                  digi_chnl_hndl_conn_t mult_dest_ptr[DIGI_RESOURCE_MULTI_DEST_MAX]);

/*
 * ASK operators related to provisioning, activation and mapping states.
 */
PUBLIC BOOL8 digi_resource_is_channel_provisioned(digi_handle_t *digi_handle,
                                                  digi_handle_id_t chnl_hndl_type,
                                                  digi_chnl_hndl_t *chnl_hndl_ptr);

PUBLIC BOOL8 digi_resource_is_channel_activated(digi_handle_t *digi_handle,
                                                digi_resource_handle_summary_t *digi_resource_handle,
                                                void *chnl_handle_ptr,
                                                digi_handle_id_t chnl_handle_type,
                                                BOOL8 is_oduk_switch);

PUBLIC BOOL8 digi_resource_is_channel_mapped(digi_handle_t *digi_handle,
                                             digi_resource_handle_summary_t *digi_resource_handle,
                                             void *chnl_handle_ptr,
                                             digi_handle_id_t chnl_handle_type,
                                             BOOL8 is_oduk_switch);


/*
 * Release related operators.  The first set of release operators uses the
 * resource table to enable the releasing of these resources. The second
 * set do not and simply release either from a physical configuration value
 * such as a serdes port or release resources from a provided channel handle
 */

PUBLIC PMC_ERROR digi_serdes_port_release(digi_handle_t *digi_handle,
                                          digi_serdes_port_t *port_ctxt_ptr,
                                          BOOL8 switch_release,
                                          BOOL8 mapotn_release);

PUBLIC PMC_ERROR digi_bp_intf_chnl_release(digi_handle_t *digi_handle,
                                           digi_handle_id_t chnl_hndl_type,
                                           digi_chnl_hndl_t *chnl_hndl_ptr,
                                           BOOL8 switch_release,
                                           BOOL8 mapotn_release);

PUBLIC PMC_ERROR digi_trib_slot_chnl_release(digi_handle_t *digi_handle,
                                             digi_handle_id_t chnl_hndl_type,
                                             digi_chnl_hndl_t *chnl_hndl_ptr,
                                             UINT32 tx_trib_slot[3],
                                             UINT32 rx_trib_slot[3],
                                             BOOL8 switch_release,
                                             BOOL8 mapotn_release);

PUBLIC PMC_ERROR digi_subcontainer_chnl_release(digi_handle_t *digi_handle,
                                                digi_handle_id_t chnl_hndl_type,
                                                digi_chnl_hndl_t *chnl_hndl_ptr,
                                                BOOL8 switch_release,
                                                BOOL8 mapotn_release);



#ifdef PMC_DEPRECATED
PUBLIC PMC_ERROR digi_resource_serdes_port_release(digi_handle_t *digi_handle,
                                                   digi_resource_handle_summary_t *digi_resource_handle,
                                                   digi_serdes_intf_t intf,
                                                   UINT16 serdes_port);

PUBLIC PMC_ERROR digi_resource_ilkn_channel_release(digi_handle_t *digi_handle,
                                                    digi_resource_handle_summary_t *digi_resource_handle,
                                                    UINT32 tx_ilkn_bcw_chnl,
                                                    UINT32 rx_ilkn_bcw_chnl);
#endif /* PMC_DEPRECATED */

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_RESOURCES_API_H */

/*
** end of file
*/

