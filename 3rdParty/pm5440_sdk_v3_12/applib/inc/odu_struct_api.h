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
*   DESCRIPTION : This files contains definitions for C functions for
*   configuring using an ODU struct.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _ODU_STRUCT_API_H
#define _ODU_STRUCT_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "coreotn_api.h"
#include "util_global_api.h"
#include "util_global_api.h"
#include "odu_rcp_api.h"

/*
** Enumerated Types
*/
#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* ENUM: odu_struct_target_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type to specify the target of configuration
*
* ELEMENTS:
*   ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT         - configuration is performed
*                                                 against odu_struct
*   ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE  - configuration is performed
*                                                 against the odu_struct and the
*                                                 device
*
*******************************************************************************/
typedef enum
{
    ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT        = 0,
    ODU_STRUCT_CONFIG_TARGET_ODU_STRUCT_DEVICE = 1,

    LAST_CONFIG_TARGET                                      /* this is for out of range values error handling */
} odu_struct_target_t;
#endif /* DOXYGEN_PUBLIC_ONLY */
#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* ENUM: odu_struct_ext_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type to specify the extraction mode of
*    configuration field
*
* ELEMENTS:
*   ODU_STRUCT_EXTRACT_FROM_ODU_STRUCT   - extraction is performed against the
*                                          odu_struct \n
*   ODU_STRUCT_EXTRACT_FROM_DEVICE       - extraction is performed against the
*                                          the device \n
*
*******************************************************************************/
typedef enum
{
    ODU_STRUCT_EXTRACT_FROM_ODU_STRUCT  = 0,
    ODU_STRUCT_EXTRACT_FROM_DEVICE      = 1,

    LAST_ODU_STRUCT_EXTRACT                                 /* this is for out of range values error handling */
} odu_struct_ext_mode_t;
#endif /* DOXYGEN_PUBLIC_ONLY */
/*******************************************************************************
* ENUM: odu_struct_pm_tcm_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type to specify the operating mode of PM/TCMi
*    layer.
*
* ELEMENTS:
*           ODU_STRUCT_PM_TCM_START                  - start mode
*           ODU_STRUCT_PM_TCM_TT_OPERATIONAL         - operational mode
*           ODU_STRUCT_PM_TCM_TT_TRANSPARENT         - transparent mode
*           ODU_STRUCT_PM_TCM_TT_MONITOR             - monitor mode only for ODU_RFRM
*           ODU_STRUCT_PM_TCM_NON_INTRUSIVE_MONITOR  - nim mode only for ODU_RFRM
*           ODU_STRUCT_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED - operational, but TCM OH
*                                                               is not zeroed out (this
*                                                               mode is required for interop
*                                                               with some legacy equipment.
*                                                               Mode only for RFRM)
*           ODU_STRUCT_PM_TCM_TT_ERASE - Zeros out the TCM overhead (G.798
*                                        MI_TCM_Extension - erase).  Note, a pool 
*                                        ID is consumed for this mode. Mode only for RFRM.
*
*******************************************************************************/
typedef enum
{
    ODU_STRUCT_PM_TCM_START                   = 0,
    ODU_STRUCT_PM_TCM_TT_OPERATIONAL,
    ODU_STRUCT_PM_TCM_TT_TRANSPARENT,
    ODU_STRUCT_PM_TCM_TT_MONITOR,
    ODU_STRUCT_PM_TCM_NON_INTRUSIVE_MONITOR,
    ODU_STRUCT_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED,
    ODU_STRUCT_PM_TCM_TT_ERASE,

    LAST_ODU_STRUCT_PM_TCM_MODE                              /* this is for out of range values error handling */
} odu_struct_pm_tcm_mode_t;
#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* ENUM: odu_struct_odu_oh_ins_src_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the source of ODU overhead
*    bytes insertion
*
* ELEMENTS:
*   ODU_STRUCT_ODU_OH_INS_INCOMING          - data from incoming ODUk frame
*   ODU_STRUCT_ODU_OH_INS_REGISTER_FORCE    - data from register forcing
*   ODU_STRUCT_ODU_OH_INS_ODU_RCP           - data from FE/GE port
*   ODU_STRUCT_ODU_OH_INS_ODU_OHP           - data from external overhead port
*
*******************************************************************************/
typedef enum
{
    ODU_STRUCT_ODU_OH_INS_INCOMING         = 0,
    ODU_STRUCT_ODU_OH_INS_REGISTER_FORCE,
    ODU_STRUCT_ODU_OH_INS_ODU_RCP,
    ODU_STRUCT_ODU_OH_INS_ODU_OHP,
  
    LAST_ODU_STRUCT_OH_INS_SRC                            /* this is for out of range values error handling */
} odu_struct_odu_oh_ins_src_t;
#endif /* DOXYGEN_PUBLIC_ONLY */
#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* ENUM: odu_struct_odu_level_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying the level of ODU struct
*
* ELEMENTS:
*     ODU_STRUCT_LEVEL_HO_ODU - ODU struct for HO ODU
*    ODU_STRUCT_LEVEL_MO_ODU - ODU struct for MO ODU
*    ODU_STRUCT_LEVEL_LO_ODU - ODU struct for LO ODU
*    ODU_STRUCT_LEVEL_3B_ODU - ODU struct for ODU 3B stage  
*    ODU_STRUCT_LEVEL_4_ODU  - ODU struct for ODU 4 stage 
*    ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC - ODU struct for SYSOTN application
*******************************************************************************/
typedef enum
{
    ODU_STRUCT_LEVEL_HO_ODU  = 0,
    ODU_STRUCT_LEVEL_MO_ODU,
    ODU_STRUCT_LEVEL_LO_ODU,
    ODU_STRUCT_LEVEL_3B_ODU,
    ODU_STRUCT_LEVEL_4_ODU,
    ODU_STRUCT_LEVEL_SYSOTN_TCM_MON_LINE_TO_FABRIC,
    LAST_ODU_STRUCT_LEVEL                    /* this is for out of range values error handling */
} odu_struct_odu_level_t;
#endif /* DOXYGEN_PUBLIC_ONLY */
#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* ENUM: odu_struct_otn_mux_stages_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type to specify number of stages involved in
*    multiplexing the payload data.
*
* ELEMENTS:
*    ODU_STRUCT_OTN_MUX_STAGES_NONE    - No muxing; unchannelized payload
*    ODU_STRUCT_OTN_MUX_STAGES_ONE     - One stage muxing
*   ODU_STRUCT_OTN_MUX_STAGES_TWO     - Two stage muxing
*
*
*******************************************************************************/
typedef enum
{
    ODU_STRUCT_OTN_MUX_STAGES_NONE = 0,
    ODU_STRUCT_OTN_MUX_STAGES_ONE,
    ODU_STRUCT_OTN_MUX_STAGES_TWO,

    LAST_ODU_STRUCT_OTN_MUX_STAGES        /* this is for out of range values error handling */
} odu_struct_otn_mux_stages_t;
#endif /* DOXYGEN_PUBLIC_ONLY */
/*******************************************************************************
* ENUM: odu_struct_odu_frm_pm_tcm_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for monitored connection layer type that is pool id based.
*
* ELEMENTS:
*   ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM           - PM message
*   ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM1         - TCM1 message
*   ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM2         - TCM2 message
*   ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM3         - TCM3 message
*   ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM4         - TCM4 message
*   ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM5         - TCM5 message
*   ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM6         - TCM6 message
*   ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_FLOATING_TCM - FLOATING TCM message
*                                                  (only valid for sink)
*   ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM_NIM       - PM NIM message
*                                                  (only valid for sink)
*   ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_DCI          - PARENT is DCI
*   ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_DONT_CARE    - default layer ID
*******************************************************************************/
typedef enum
{
    ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM              = 0,
    ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM1            = 1,
    ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM2            = 2,
    ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM3            = 3,
    ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM4            = 4,
    ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM5            = 5,
    ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_TCM6            = 6,
    ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_FLOATING_TCM    = 7,
    ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_PM_NIM          = 8,
    ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_DCI             = 9,
    ODU_STRUCT_ODU_FRM_MESSAGE_TYPE_DONT_CARE       = 10,

    LAST_ODU_STRUCT_ODU_FRM_MESSAGE_TYPES,    /* this is for out of range values error handling */
} odu_struct_odu_frm_pm_tcm_id_t;

/*******************************************************************************
* STRUCTURE: odu_struct_odu_frm_layer_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to illustrate the current layer hierarchy in odu_rfrm.
*
* ELEMENTS:
*   pool_id     - pool ID of TCM layer
*   pm_tcm_num  - TCM field instance. See odu_rfrm_pm_tcm_id_t.
*   tcm_mode    - Mode of the TCM layer. See odu_rfrm_pm_tcm_mode_t.
*******************************************************************************/
typedef struct
{

    UINT32 pool_id;
    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num;
    odu_struct_pm_tcm_mode_t tcm_mode;

} odu_struct_odu_frm_layer_struct_t;

/*******************************************************************************
* ENUM: odu_struct_dm_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for OTN function configuration operation mode.
*
* ELEMENTS:
*   ODU_STRUCT_DM_SOURCE        - DIGI acts as a delay measurement source (the
*                               source flips the DMp/DMtx bit and waits
*                               for the value to be received by the sink).                
*   ODU_STRUCT_DM_FEEDTHROUGH   - DIGI acts as a delay measurement feedthough
*                               (DMp/DMtx value is passed transparently
*                                from source to sink).
*******************************************************************************/
typedef enum
{
    ODU_STRUCT_DM_SOURCE         = 0,
    ODU_STRUCT_DM_FEEDTHROUGH    = 1,
} odu_struct_dm_mode_t;

/*******************************************************************************
* ENUM: odu_struct_optional_oh_ins_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for optionally inserted overhead fields in
*   ODU_TFRM.
*
* ELEMENTS:
*   ODU_STRUCT_ODU_OH_FTFL          - FTFL OH insertion
*   ODU_STRUCT_ODU_OH_TCM_ACT       - TCM_ACT OH insertion
*   ODU_STRUCT_ODU_OH_PM_TCM_RES    - PM&TCM Reserved OH insertion
*   ODU_STRUCT_ODU_OH_RES1          - RES1 OH insertion
*   ODU_STRUCT_ODU_OH_EXP           - EXP OH insertion
*   ODU_STRUCT_ODU_OH_RES2          - RES2 OH insertion
*   ODU_STRUCT_ODU_OH_GCC2          - GCC2 OH insertion
*   ODU_STRUCT_ODU_OH_GCC1          - GCC1 OH insertion
*
*******************************************************************************/
typedef enum
{
    ODU_STRUCT_ODU_OH_FTFL          = 0,       
    ODU_STRUCT_ODU_OH_TCM_ACT          ,                              
    ODU_STRUCT_ODU_OH_PM_TCM_RES       ,                              
    ODU_STRUCT_ODU_OH_RES1             ,                              
    ODU_STRUCT_ODU_OH_EXP              ,                              
    ODU_STRUCT_ODU_OH_RES2             ,                              
    ODU_STRUCT_ODU_OH_GCC2             ,                              
    ODU_STRUCT_ODU_OH_GCC1             ,                              
                                         
    LAST_ODU_STRUCT_ODU_OH_FIELD           /* this is for error checking */                             
} odu_struct_optional_oh_ins_t;

/*******************************************************************************
* ENUM: odu_struct_tti_oh_ins_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for specifying insertion of desired fields in a TTI message
*
* ELEMENTS:
*   ODU_STRUCT_TTI_SAPI_INS               - SAPI only
*   ODU_STRUCT_TTI_DAPI_INS               - DAPI only
*   ODU_STRUCT_TTI_OP_SPEC_INS            - Operator Specific only
*   ODU_STRUCT_TTI_SAPI_DAPI_INS          - both SAPI and DAPI
*   ODU_STRUCT_TTI_SAPI_OP_SPEC_INS       - both SAPI and Operator Specific
*   ODU_STRUCT_TTI_DAPI_OP_SPEC_INS       - both DAPI and Operator Specific
*   ODU_STRUCT_TTI_SAPI_DAPI_OP_SPEC_INS  - all SAPI, DAPI and Operator Specific
*******************************************************************************/
typedef enum
{
    ODU_STRUCT_TTI_SAPI_INS   = 0,
    ODU_STRUCT_TTI_DAPI_INS,
    ODU_STRUCT_TTI_OP_SPEC_INS,
    ODU_STRUCT_TTI_SAPI_DAPI_INS,
    ODU_STRUCT_TTI_SAPI_OP_SPEC_INS,
    ODU_STRUCT_TTI_DAPI_OP_SPEC_INS,
    ODU_STRUCT_TTI_SAPI_DAPI_OP_SPEC_INS,
    
    LAST_ODU_STRUCT_TTI_INS,
} odu_struct_tti_oh_ins_t;

/*******************************************************************************
* ENUM: odu_struct_stream_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for specifying the direction of the ODU stream
*
* ELEMENTS:
*   ODU_STRUCT_STREAM_RX               - Rx direction
*   ODU_STRUCT_STREAM_TX               - Tx direction
*   
*******************************************************************************/
typedef enum
{
    ODU_STRUCT_STREAM_RX   = 0,
    ODU_STRUCT_STREAM_TX,
    
    LAST_ODU_STRUCT_STREAM_DIR,
} odu_struct_stream_dir_t;


#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* ENUM: odu_struct_frm_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enumerated type to specify ODU RI framer stage 
*
* ELEMENTS:
*   ODU_STRUCT_FRM_STG1            - Framer stage 1
*   ODU_STRUCT_FRM_STG2            - Framer stage 2
*   ODU_STRUCT_FRM_STG3A           - Framer stage 3A
*   ODU_STRUCT_FRM_STG3B           - Framer stage 3B
*   ODU_STRUCT_FRM_STG4            - Framer stage 4
*   ODU_STRUCT_FRM_STGN            - Framer stage N
*           
*******************************************************************************/
typedef enum odu_struct_frm_id_t {
    ODU_STRUCT_FRM_STG1 = 0,
    ODU_STRUCT_FRM_STG2 = 1,
    ODU_STRUCT_FRM_STG3A = 2,
    ODU_STRUCT_FRM_STG3B = 3,
    ODU_STRUCT_FRM_STG4 = 4,
    ODU_STRUCT_FRM_STGN = 5,
    LAST_ODU_STRUCT_FRM_ID
} odu_struct_frm_id_t;

#endif /* DOXYGEN_PUBLIC_ONLY */

/*
** Constants
*/
#define ODU_STRUCT_UNASSIGNED_POOL_ID 0xFFFF
#define ODU_STRUCT_PIDS_PER_CHNL      COREOTN_PIDS_PER_CHNL


/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* STRUCTURE: odu_struct_tcm_pool_rsc_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to illustrate the TCM pool id based configurations within an
*   ODU struct. One odu_struct_tcm_pool_rsc_t struct respresents configuration
*   data for one TCM resource. 7 of these structs are included in the ODU struct
*   to be used for a total of 7 TCM pool id based resources: TCM1-6 and Floating
*   TCM.
*
* ELEMENTS:
*   tcm_instance        - TCM resource instance to be provisioned.
*   tcm_pool_id_tx      - pool id of TCM provisioned in the source direction.
*   tcm_pool_id_rx      - pool id of TCM provisioned in the sink direction.
*   mode_tx             - operational mode of TCM layer in source.
*   mode_rx             - operational mode of TCM layer in sink.
*   maint_sig_tx        - maintenance signal carried in the TCM layer in source.
*   maint_sig_rx        - maintennace signal carried in the TCM layer in sink.
*   tim_mon             - TTI message processing mode for the path layer
*                         (determines if comparison of SAPI or/and DAPI is
*                         used for TIM detection)
*   tim_act_dis         - determines if dTIM defect contributes to the aAIS
*                         consequential action.
*   ltc_to_ais          - determines if dLTC defect contributes to the aAIS and
*                         aTSF consequential action.
*   bdi_force_val       - BDI value to be inserted in TCM
*   bei_biae_force_val  - BEI/BIAE value to be inserted in TCM
*   stat_force_val      - STAT value to be inserted in TCM
*   dmt_src             - determines if delay measurement will be enabled
*                         for TCM layer.
*   dmt_val             - value of the delay measurement on TCM layer.
*   deg_m               - degraded defect consecutive one second monitoring
*                         interval for TCM layer.
*   deg_th              - degraded defect one second errored block count for
*                         TCM layer.
*   tti_index_tx        - index of source TTI message in the shared memory pool
*   tti_index_rx        - index of sink TTI message in the shared memory pool
*   rx_layer_num        - Where this TCM sits in the TCM layering heirarchy
*                         (1 is ultimate parent)
*   tx_layer_num        - Where this TCM sits in the TCM layering heirarchy
*                         (1 is ultimate parent)
*
*******************************************************************************/
typedef struct odu_struct_tcm_pool_rsc_t
{
    odu_struct_odu_frm_pm_tcm_id_t tcm_instance;
    UINT16 tcm_pool_id_tx; 
    UINT16 tcm_pool_id_rx; 
    UINT8 mode_tx;
    UINT8 mode_rx; 
    UINT8 maint_sig_tx;
    UINT8 maint_sig_rx; 
    UINT8 tim_mon;
    BOOL8 tim_act_dis;
    BOOL8 ltc_to_ais;
    BOOL8 bw_dis;
    UINT8 bdi_force_val; 
    UINT8 bei_biae_force_val; 
    UINT8 stat_force_val; 
    UINT8 deg_m; 
    UINT32 deg_th; 
    UINT16 tti_index_tx;
    UINT16 tti_index_rx;
    UINT8 dmt_src; 
    UINT8 dmt_val; 
    UINT8 rx_layer_num;
    UINT8 tx_layer_num;
} odu_struct_tcm_pool_rsc_t;
#endif /* DOXYGEN_PUBLIC_ONLY */

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* STRUCTURE: odu_trib_slot_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to illustrate the odu channel context for channelized resources.
*
* ELEMENTS:
*   trib_port               - tributary port number the ODTU payload is being
*                             transported in the tributary slots.
*                             The maximum index used must equal to
*                             (num_tribslots - 1).
*   sub_chnl_ids            - an array of channel ids for the sub containers.
*   num_sub_container       - number of sub structures in the current channel
*   index                   - index to the pre-calculated data in the shared
*                             memory
*******************************************************************************/
typedef struct odu_trib_slot_t
{
    UINT8 tx_trib_port[80]; 
    UINT8 rx_trib_port[80]; 
    UINT8 sub_chnl_ids_rx[80];
    UINT8 sub_chnl_ids_tx[80];
    /* sub-container context */
    UINT8 num_sub_container_rx;
    UINT8 num_sub_container_tx; 
    UINT16 index;
} odu_trib_slot_t;
#endif /* DOXYGEN_PUBLIC_ONLY */


#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* STRUCTURE: odu_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to illustrate the odu struct.
*
* ELEMENTS:
*   chnl_id                 - channel ID of the odu_struct.
*   odu_type                - odu container type. See odukp_type_enum.
*   odu_level               - level of ODU struct.
*                             HO - stage 1 framer
*                             MO - stage 2 framer
*                             LO - stage 3a framer
*   mux_stage               - number of multiplexing stages.
*                             Used to determine if FMF1 or FMF2 needs to be
*                             configured.
*   payload_format          - ODU payload format in the HO odu_struct. Once
*                             determined at HO ODU struct, this information is
*                             consistent throughout all its sub containers.
*                             TS type. (i.e. unchannelized, 1.25G TS, 2.5G TS)
*   calendar_entry_mask     - 3 x 32 bit mask value to specify the calendar slot
*                             assignment of the ODU channel. 
*   calendar_entry_mask     - 3 x 32 bit mask value to specify the calendar slot
*                             assignment of the ODU channel. 
*   mapping_mode            - mapping mode of client odu (in case of LO ODU)
*   payload_type            - payload type of this odu channel.
*                             Maps to OPUk payload type (PT) byte in PSI[0]
*                             of the OPUk overhead.
*   num_tribslots           - number of tributary slots that this channel
*                             occupies.
*   mode                    - operational mode of this odu channel.
*                             PASSTHRU vs. Normal operation
*   client_bitate           - ODU client rate for CBR ODUFLEX.  For all 
*                             Lo order ODU types 0 should be assigned.
*   msi_mode                - msi assignment mode.
*                             0 - Fixed
*                             1 - Flexible
*   sf_sd_prop              - determines if server signal or server degrade
*                             status will be propagated.
*   ssf_prop                - determines if server signal fail status will be
*                             propagated.
*   defect_act_ctrl         - 1 byte mask that controls if a defect contributes
*                             to the aAIS consequential action.
*                             Bit 0 - dLTC_ACT_DIS
*                             Bit 1 - dAIS_ACT_DIS
*                             Bit 2 - dOCI_ACT_DIS
*                             Bit 3 - dLCK_ACT_DIS
*   lof_lom_act_dis         - controls whether or not dLOFLOM defect causes
*                             nominal ODUk-AIS to be inserted in the channel.
*   dplm_to_ssf             - controls whether or not dPML defect causes
*                             SSF to be inserted in the channel.
*   dloomfi_to_ssf          - controls whether or not dLOOMFI defect causes
*                             SSF to be inserted in the channel.
*   dmsim_to_ssf            - controls whether or not dMSIM defect causes
*                             SSF to be inserted in the channel.                             
*   pm_mode                 - operational mode of odu path layer.
*                             Corresponds to 'mode' field in
*                             LAYER_CONTROL1_PID of ODU_RFRM.
*                             Pooled PM resource can be configured as NIM,
*                             Transparent, Monitor, or Operational.
*   tim_mon_pm              - TTI message processing mode for the path layer
*                             (determines if comparison of SAPI or/and DAPI is
*                              used for TIM detection)
*   tim_act_dis_pm          - determines if dTIM defect contributes to the aAIS
*                             consequential action.
*   bw_dis                  - controls whether or not consequential actions
*                             based on backward defects are enabled or not 
*   deg_m_pm                - degraded defect consecutive one second monitoring
*                             interval for the path layer
*   deg_th_pm               - degraded defect one second errored block count for
*                             the path layer.
*   dmp_src                 - determines if delay measurement will be enabled
*                             for the path layer.
*   dmp_val                 - value of the delay measurement on the path layer.
*   bdi_force_val_pm        - value of BDI overhead field to be inserted for the
*                             path layer.
*   bei_biae_force_val_pm   - value of BEI/BIAE overhead field to be inserted
*                             for the path layer.
*   stat_force_val_pm       - value of STAT overhead field to be inserted.
*   pm_tcm_res_src          - source of PM&TCM reserved bit.
*   tcm_act_src             - source of TCM ACT field.
*   ftfl_src                - source of FTFL
*   aps_pcc_src             - source of APS/PCC field for all 8 layers in the
*                             odu channel
*                             Bit 0 - ODUk Path
*                             Bit 1 - ODUk TCM1
*                             Bit 2 - ODUk TCM2
*                             Bit 3 - ODUk TCM3
*                             Bit 4 - ODUk TCM4
*                             Bit 5 - ODUk TCM5
*                             Bit 6 - ODUk TCM6
*                             Bit 7 - ODUk server layer trail
*   res1_src                - source of RES1 overhead field.
*   res2_src                - source of RES2 overhead field.
*   exp_src                 - source of EXP overhead field.
*   gcc1_src                - source of GCC1 overhead field.
*   gcc2_src                - source of GCC2 overhead field.
*   bdi_src                 - source of BDI overhead field for the path layer.
*   bei_biae_src            - source of BEI/BIAE overhead field for the path
*                             layer.
*   stat_src                - source of STAT overhead field.
*   pm_tcm_res_force_val    - value of PM&TCM reserved bit to be inserted.
*   tcm_act_force_val       - value of TCM ACT field to be inserted.
*   res1_force_val          - value of RES1 overhead field to be inserted.
*   res2_force_val          - value of RES2 overhead field to be inserted.
*   exp_force_val           - value of EXP overhead field to be inserted.
*   gcc1_force_val          - value of GCC1 overhead field to be inserted.
*   gcc2_force_val          - value of GCC2 overhead field to be inserted.
*   ftfl                    - 20 bytes of FTFL value of backward and forward
*                             fault type
*   aps_pcc_force_val       - 8 x 4 byte value of APS/PCC overhead field to be
*                             inserted. A single 4 byte value represents for one
*                             of PM or TCM layer
*   tcm_pool_rsc            - an array of 8 structs to illustrate configuration 
*                             data of TCM pool id based resource. The resource 
*                             represented in this struct could be any one from 
*                             TCM1-6, Floating TCM_NIM, and PM_NIM.
*   num_layers_rx           - number of PM/TCMi resources provisioned on the
*                             receive direction of the channel
*   num_layers_tx           - number of PM/TCMi resources provisioned on the
*                             transmit direction of the channel
*   maint_sig_rx            - maintenance signal inserted on receive path 
*   tx_trib_loc_mask        - 3 x 32 bit mask value to specify the tributary
*                             slot allocation in the transmit direction of the
*                             ODU channel.
*   rx_trib_loc_mask        - 3 x 32 bit mask value to specify the tributary
*                             slot allocation in the receive direction of the
*                             ODU channel. 
*   trib_slot_ctxt           - structure to describe the tributary slot context
*                              of the ODU channel
*   ilkn_fc_cal              - array to describe the flow control calendar usage
*   bcw_chnl                 - bcw channel ID associated with this ODU channel
* 
*******************************************************************************/
typedef struct odu_chnl_t
{
    UINT32 chnl_id;
    util_global_odukp_type_t odu_type;
    odu_struct_odu_level_t odu_level;
    odu_struct_otn_mux_stages_t mux_stage;
    util_global_odu_line_payload_t  payload_format;
    UINT32 calendar_entry_mask_rx[3];
    UINT32 calendar_entry_mask_tx[3];

    util_global_mapping_mode_t mapping_mode;

    UINT8 payload_type;
    UINT8 num_tribslots;

    UINT8 mode;
    DOUBLE client_bitrate;
    UINT8 msi_mode;

    /* defect actions CTRL */
    UINT8 sf_sd_prop;
    UINT8 ssf_prop;
    UINT8 defect_act_ctrl;
    BOOL8 lof_lom_act_dis;
    BOOL8 dplm_to_ssf;
    BOOL8 dloomfi_to_ssf;
    BOOL8 dmsim_to_ssf;

    /* PM configuration */
    UINT8 pm_mode;
    UINT8 tim_mon_pm;
    BOOL8 tim_act_dis_pm;
    BOOL8 bw_dis;

    UINT8 deg_m_pm;
    UINT32 deg_th_pm;
    UINT8 dmp_src;
    UINT8 dmp_val;

    UINT8 bdi_force_val_pm;
    UINT8 bei_biae_force_val_pm;
    UINT8 stat_force_val_pm;

    /* ODU Framer OH context */
    UINT8 pm_tcm_res_src;
    UINT8 tcm_act_src;
    UINT8 ftfl_src;
    UINT8 aps_pcc_src;
    UINT8 res1_src;
    UINT8 res2_src;
    UINT8 exp_src;
    UINT8 gcc1_src;
    UINT8 gcc2_src;
    UINT8 bdi_src;
    UINT8 bei_biae_src;
    UINT8 stat_src;

    UINT8 pm_tcm_res_force_val;
    UINT8 tcm_act_force_val;

    UINT16 res1_force_val;
    UINT8 res2_force_val[6];
    UINT16 exp_force_val;
    UINT16 gcc1_force_val;
    UINT16 gcc2_force_val;
    UINT8 ftfl[20];
    UINT32 aps_pcc_force_val[8];

    /* TCM pool id based configuration
    ** where A-G can be TCM 1-6, TCM_NIM or PM_NIM
    */
    odu_struct_tcm_pool_rsc_t tcm_pool_rsc[8];
    
    /* total number of tcm_pool_rcs */
    UINT8 num_layers_rx;
    UINT8 num_layers_tx;
    UINT8 maint_sig_rx;
    
    /* ODU payload context for multiplexing */
    UINT32 tx_trib_loc_mask[3];
    UINT32 rx_trib_loc_mask[3];   

    /* tribslot context for a channelized ODU */
    odu_trib_slot_t trib_slot_ctxt;
    
    UINT32 ilkn_fc_cal[UTIL_GLOBAL_32BIT_ILKN_FC_ENTRIES];   
    UINT8 bcw_chnl;                                          

} odu_chnl_t;
#endif /* DOXYGEN_PUBLIC_ONLY */

#ifndef DOXYGEN_PUBLIC_ONLY

/*******************************************************************************
* STRUCTURE: odu_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This structure contains the definition for a single ODU layer. This
*   structure is located in local memory as a reference to the odu_chnl_t
*   portion to be located in local or shared memory.
*
* ELEMENTS:
*   switch_data  - ODUK Switch port and channel data
*   pool_id_array- array of pool ids that are used by TCM or PM layer provisioned
*                  on this ODU channel   
*   *mem_ptr     - pointer to odu_struct in local/shared memory. For shared
*                  memory, this would be the mem_pool address (needs to be
*                  converted to physical PCIe address when creating a TRQ to
*                  pass to FW).
*   active       - TRUE  -- odu_struct is active and coherent with device. \n
*                  FALSE -- odu_struct is pending and independent of device state.
*******************************************************************************/
typedef struct odu_struct_t {
    util_global_switch_data_def_t switch_data;
    UINT8 pool_id_array[ODU_STRUCT_PIDS_PER_CHNL];
    odu_chnl_t *mem_ptr;
    BOOL8 active;
} odu_struct_t;

#endif /* DOXYGEN_PUBLIC_ONLY */
/*
** Global variables
*/

/*
** Function Prototypes
*/
#ifndef DOXYGEN_PUBLIC_ONLY
PUBLIC odu_struct_t *odu_struct_ctxt_create(pmc_handle_t *parent);
PUBLIC void odu_struct_ctxt_destroy(odu_struct_t *odu_struct);

PUBLIC void odu_struct_default_set(odu_struct_t *odu_struct);

PUBLIC PMC_ERROR odu_struct_base_build(odu_struct_t *odu_struct,
                                       UINT32 chnl,
                                       util_global_odukp_type_t oduk_type,
                                       odu_struct_odu_level_t odu_level,
                                       util_global_odu_line_payload_t payload_format,
                                       odu_struct_otn_mux_stages_t mux_stage);
PUBLIC PMC_ERROR odu_struct_chnl_payload_type_set(coreotn_handle_t *coreotn_handle,
                                                  odu_struct_t *odu_struct,
                                                  UINT8 pt,
                                                  odu_struct_stream_dir_t direction,
                                                  odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_defect_actions_ctrl_set(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    UINT8 defect_action_value,
                                                    UINT8 defect_action_wr_mask,
                                                    odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_tcmi_ltc_to_ais_tsf_set(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                    BOOL8 dLTC_dis,
                                                    odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_tim_act_dis_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                            BOOL8 tim_act_dis,
                                            odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_tcm_maint_sig_set(coreotn_handle_t *coreotn_handle,
                                              odu_struct_t *odu_struct,
                                              odu_struct_stream_dir_t direction,
                                              odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                              coreotn_maint_sig_type_t force_alarm_sig,
                                              odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_chnl_maint_sig_set(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               odu_struct_stream_dir_t direction,
                                               coreotn_maint_sig_type_t force_alarm_sig,
                                               odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_odukp_pck_maint_sig_set(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    coreotn_maint_sig_type_t force_alarm_sig,
                                                    odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_odukp_pck_maint_sig_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    coreotn_maint_sig_type_t *force_alarm_sig);
PUBLIC PMC_ERROR odu_struct_chnl_sd_sf_ssf_prop_set(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    UINT8 chnl_defect_action_msk,
                                                    odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_chnl_lof_lom_act_dis_set(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     BOOL8 lof_lom_act_dis,
                                                     odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_tx_pm_tcmi_set(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_struct,
                                           odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                           UINT8 pool_assign_mode,
                                           UINT8 tcm_pool_id, 
                                           odu_struct_odu_frm_pm_tcm_id_t parent_tcm_num,
                                           odu_struct_pm_tcm_mode_t source_mode,
                                           odu_struct_target_t target,
                                           UINT16 *assigned_pool_id);
PUBLIC PMC_ERROR odu_struct_rx_pm_tcmi_set(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_struct,
                                           odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                           UINT8 pool_assign_mode,
                                           UINT8 tcm_pool_id, UINT8 float_tcmi_src,
                                           odu_struct_odu_frm_pm_tcm_id_t parent_tcm_num,
                                           odu_struct_pm_tcm_mode_t sink_mode,
                                           odu_struct_target_t target,
                                           UINT16 *assigned_pool_id);                                          

PUBLIC PMC_ERROR odu_struct_rx_tcmi_mode_update(coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct,
                                                odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                odu_struct_pm_tcm_mode_t sink_mode);

PUBLIC PMC_ERROR odu_struct_rx_tcmi_resource_transition(coreotn_handle_t *coreotn_handle,
                                                        odu_struct_t *odu_struct,
                                                        UINT32 pool_id);

PUBLIC PMC_ERROR odu_struct_rx_pm_tcmi_parent_update(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                     odu_struct_odu_frm_pm_tcm_id_t new_parent_tcm_num);

PUBLIC PMC_ERROR odu_struct_pm_tcm_res_src_set(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               odu_struct_odu_oh_ins_src_t pm_tcm_res_src,
                                               UINT8 pm_tcm_res_force_val,
                                               odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_ftfl_src_set(coreotn_handle_t *coreotn_handle,
                                         odu_struct_t *odu_struct,
                                         odu_struct_odu_oh_ins_src_t ftfl_source,
                                         UINT8 ftfl_force_field,
                                         odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_tx_tti_set( coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                        odu_struct_tti_oh_ins_t tti_oh_ins,
                                        UINT8 sapi[16],
                                        UINT8 dapi[16],
                                        UINT8 op_spec[32] );
PUBLIC PMC_ERROR odu_struct_rx_exp_tti_set( coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                            UINT8 sapi[16],
                                            UINT8 dapi[16] ); 
PUBLIC PMC_ERROR odu_struct_tcm_act_src_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_oh_ins_src_t tcm_act_source,
                                            UINT8 tcm_act_force_val,
                                            odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_aps_pcc_src_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            UINT8 mon_level,
                                            odu_struct_odu_oh_ins_src_t aps_pcc_source,
                                            UINT32 aps_pcc_force_val,
                                            odu_struct_target_t target);                                        
PUBLIC PMC_ERROR odu_struct_tx_res1_src_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_oh_ins_src_t res1_source,
                                            UINT16 res1_force_val,
                                            odu_struct_target_t target); 
PUBLIC PMC_ERROR odu_struct_tx_res2_src_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_oh_ins_src_t res2_source,
                                            UINT8 res2_force_val[6],
                                            odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_tx_exp_src_set(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_struct,
                                           odu_struct_odu_oh_ins_src_t exp_source,
                                           UINT16 exp_force_val,
                                           odu_struct_target_t target);                                                                                
PUBLIC PMC_ERROR odu_struct_tx_gcc1_src_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_oh_ins_src_t gcc1_source,
                                            UINT16 gcc1_force_val,
                                            odu_struct_target_t target);

PUBLIC PMC_ERROR odu_struct_deg_th_set(coreotn_handle_t *coreotn_handle,
                                       odu_struct_t *odu_struct,
                                       odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                       UINT32 deg_th,
                                       odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_tim_det_mon_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                            UINT8 tim_det_mo,
                                            odu_struct_target_t target);

PUBLIC PMC_ERROR odu_struct_bdi_bei_biae_set(coreotn_handle_t *coreotn_handle,
                                             odu_struct_t *odu_struct,
                                             odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                             UINT8 oh_field,
                                             UINT8 bdi_force_val,
                                             UINT8 bei_biae_force_val,
                                             odu_struct_target_t target);
PUBLIC PMC_ERROR odu_struct_stat_set(coreotn_handle_t *coreotn_handle,
                                     odu_struct_t *odu_struct,
                                     odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                     UINT8 force_val,
                                     odu_struct_target_t target);
                                     
PUBLIC PMC_ERROR odu_struct_rx_pm_tcm_get(coreotn_handle_t *coreotn_handle,
                                          odu_struct_t *odu_struct,
                                          UINT8 *pm_tcm_val);                                     
                                     
PUBLIC PMC_ERROR odu_struct_delay_measure_init(coreotn_handle_t *coreotn_handle,
                                              odu_struct_t *odu_struct,
                                              odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                              odu_struct_dm_mode_t dm_mode,
                                              odu_struct_target_t target,
                                              UINT32 dm_val);

PUBLIC PMC_ERROR odu_struct_delay_measure_mode_get(coreotn_handle_t *coreotn_handle,
                                                   odu_struct_t *odu_struct,
                                                   odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                   odu_struct_dm_mode_t *dm_mode,
                                                   UINT32 *dm_val);
                                                   
PUBLIC PMC_ERROR odu_struct_all_tcm_iae_force(coreotn_handle_t *coreotn_handle,
                                              odu_struct_t *odu_struct);

PUBLIC PMC_ERROR odu_struct_pm_tcmi_delay_measure_go(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num);

PUBLIC PMC_ERROR odu_struct_pm_tcmi_get_delay_measure_count(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                     UINT32 block_mode,
                                                     UINT32 *dm_count);

                                                                                    
PUBLIC PMC_ERROR odu_struct_rx_expected_pt_set(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               UINT8 pt_val);    
PUBLIC PMC_ERROR odu_struct_tx_pt_set(coreotn_handle_t *coreotn_handle,
                                      odu_struct_t *odu_struct,
                                      UINT8 pt_val,
                                      odu_struct_target_t target);   
PUBLIC PMC_ERROR odu_struct_tx_msi_set(coreotn_handle_t *coreotn_handle,
                                       odu_struct_t *odu_struct,
                                       UINT8 ts_num,
                                       UINT8 msi_byte,
                                       odu_struct_target_t target);  
PUBLIC PMC_ERROR odu_struct_rx_expected_msi_set(coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct,
                                                UINT8 ts_port,
                                                UINT8 msi_byte,
                                                odu_struct_target_t target);                                                                                                                                                       
                                    
PUBLIC PMC_ERROR odu_struct_pm_tcmi_bw_act_set(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                               UINT8 bw_dis,
                                               odu_struct_target_t target);
                                               
PUBLIC PMC_ERROR odu_struct_tx_ftfl_set(coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        UINT8 fwd_id,
                                        UINT8 fwd_op_id[9],
                                        UINT8 bwd_id,
                                        UINT8 bwd_op_id[9],
                                        odu_struct_target_t target); 
PUBLIC PMC_ERROR odu_struct_tx_gcc2_src_set(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_oh_ins_src_t gcc2_source,
                                            UINT16 gcc2_force_val,
                                            odu_struct_target_t target);                                                 
PUBLIC PMC_ERROR odu_struct_ais_dis_set(coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                        UINT8 ais_dis,
                                        odu_struct_target_t target);    
PUBLIC PMC_ERROR odu_struct_pm_tcmi_tsd_tsf_dis_set(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                    UINT8 defect_ctrl,
                                                    UINT8 tsd_dis,
                                                    UINT8 tsf_dis,
                                                    odu_struct_target_t target);                                                                                                                                                          

PUBLIC PMC_ERROR odu_struct_otn_odu_dplm_to_ssf_set(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_target_t target,
                                                    BOOL8 enable);

PUBLIC PMC_ERROR odu_struct_otn_odu_dmsim_to_ssf_set(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_target_t target,
                                                     BOOL8 enable);

PUBLIC PMC_ERROR odu_struct_otn_odu_dloomfi_to_ssf_set(coreotn_handle_t *coreotn_handle,
                                                       odu_struct_t *odu_struct,
                                                       odu_struct_target_t target,
                                                       BOOL8 enable);


/*
** ODU Struct Getter methods
*/
PUBLIC PMC_ERROR odu_struct_tx_aps_pcc_get(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_struct,
                                           UINT8 mon_level,
                                           odu_struct_ext_mode_t ext_mode,
                                           UINT32 *aps_pcc_val);
PUBLIC PMC_ERROR odu_struct_rx_aps_pcc_get(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_struct,
                                           UINT8 mon_level,
                                           odu_struct_ext_mode_t ext_mode,
                                           UINT32 *aps_pcc_val);                                        
PUBLIC PMC_ERROR odu_struct_ais_dis_get(coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                        odu_struct_ext_mode_t ext_mode,
                                        UINT8 *ais_dis);
PUBLIC PMC_ERROR odu_struct_pm_tcmi_tsd_tsf_dis_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                    odu_struct_ext_mode_t target,
                                                    UINT8 *tsd_dis,
                                                    UINT8 *tsf_dis);
PUBLIC PMC_ERROR odu_struct_tx_msi_get(coreotn_handle_t *coreotn_handle,
                                       odu_struct_t *odu_struct,
                                       odu_struct_ext_mode_t ext_mode,
                                       UINT8 *msi_byte);
PUBLIC PMC_ERROR odu_struct_tx_msi_per_ts_get(coreotn_handle_t *coreotn_handle,
                                              odu_struct_t *odu_struct,
                                              odu_struct_ext_mode_t ext_mode,
                                              UINT8 ts_port,
                                              UINT8 *msi_byte);                                       
PUBLIC PMC_ERROR odu_struct_pm_tcmi_bw_act_get(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                               odu_struct_ext_mode_t ext_mode,
                                               UINT8 *bw_dis);
PUBLIC PMC_ERROR odu_struct_tx_pm_tcmi_mode_get(coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct,
                                                odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                odu_struct_ext_mode_t ext_mode,
                                                odu_struct_pm_tcm_mode_t *mode);
PUBLIC PMC_ERROR odu_struct_rx_pm_tcmi_mode_get(coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct,
                                                odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                odu_struct_ext_mode_t ext_mode,
                                                odu_struct_pm_tcm_mode_t *mode);                                                
PUBLIC PMC_ERROR odu_struct_rx_accepted_tti_lock(coreotn_handle_t *coreotn_handle,
                                                  BOOL8 set);
PUBLIC PMC_ERROR odu_struct_rx_accpt_tti_get(coreotn_handle_t *coreotn_handle,
                                             odu_struct_t *odu_struct,
                                             odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                             UINT8 *sapi_ptr,
                                             UINT8 *dapi_ptr,
                                             UINT8 *op_spec_ptr);
PUBLIC PMC_ERROR odu_struct_tx_tti_get(coreotn_handle_t *coreotn_handle,
                                       odu_struct_t *odu_struct,
                                       odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                       UINT8 *sapi_ptr,
                                       UINT8 *dapi_ptr,
                                       UINT8 *op_spec_ptr);
PUBLIC PMC_ERROR odu_struct_rx_exp_tti_get(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_struct,
                                           odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                           UINT8 *sapi_ptr,
                                           UINT8 *dapi_ptr);
PUBLIC PMC_ERROR odu_struct_rx_expected_msi_get(coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct,
                                                odu_struct_ext_mode_t ext_mode,
                                                UINT8 msi_byte[UTIL_GLOBAL_MAX_NUM_TS]);
PUBLIC PMC_ERROR odu_struct_rx_expected_msi_per_ts_get(coreotn_handle_t *coreotn_handle,
                                                       odu_struct_t *odu_struct,
                                                       odu_struct_ext_mode_t ext_mode,
                                                       UINT8 ts_port,
                                                       UINT8 *msi_byte);                                                    
PUBLIC PMC_ERROR odu_struct_tx_pt_get(coreotn_handle_t *coreotn_handle,
                                      odu_struct_t *odu_struct,
                                      odu_struct_ext_mode_t ext_mode,
                                      UINT8 *pt_val);
PUBLIC PMC_ERROR odu_struct_rx_tcm_maint_sig_get(coreotn_handle_t *coreotn_handle,
                                                 odu_struct_t *odu_struct,
                                                 odu_struct_odu_frm_pm_tcm_id_t tcm_num,
                                                 odu_struct_ext_mode_t ext_mode,
                                                 coreotn_maint_sig_type_t *maint_sig);
PUBLIC PMC_ERROR odu_struct_tcmi_ltc_to_ais_tsf_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                    odu_struct_ext_mode_t ext_mode,
                                                    BOOL8 *dLTC_dis);
PUBLIC PMC_ERROR odu_struct_rx_ftfl_get(coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        odu_struct_ext_mode_t ext_mode,
                                        UINT8 *ftfl);
PUBLIC PMC_ERROR odu_struct_tx_ftfl_get(coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        odu_struct_ext_mode_t ext_mode,
                                        UINT8 *ftfl);                                     
PUBLIC PMC_ERROR odu_struct_tcm_act_get(coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        odu_struct_stream_dir_t direction,
                                        odu_struct_ext_mode_t ext_mode,
                                        UINT8 *tcm_act_val);
PUBLIC PMC_ERROR odu_struct_res1_get(coreotn_handle_t *coreotn_handle,
                                     odu_struct_t *odu_struct,
                                     odu_struct_stream_dir_t direction,
                                     odu_struct_ext_mode_t ext_mode,
                                     UINT16 *res1_val);
PUBLIC PMC_ERROR odu_struct_res2_get(coreotn_handle_t *coreotn_handle,
                                     odu_struct_t *odu_struct,
                                     odu_struct_stream_dir_t direction,
                                     odu_struct_ext_mode_t ext_mode,
                                     UINT8 *res2_val);
PUBLIC PMC_ERROR odu_struct_exp_get(coreotn_handle_t *coreotn_handle,
                                    odu_struct_t *odu_struct,
                                    odu_struct_stream_dir_t direction,
                                    odu_struct_ext_mode_t ext_mode,
                                    UINT16 *exp_val);
PUBLIC PMC_ERROR odu_struct_gcc1_get(coreotn_handle_t *coreotn_handle,
                                     odu_struct_t *odu_struct,
                                     odu_struct_stream_dir_t direction,
                                     odu_struct_ext_mode_t ext_mode,
                                     UINT16 *gcc1_val);
PUBLIC PMC_ERROR odu_struct_gcc2_get(coreotn_handle_t *coreotn_handle,
                                     odu_struct_t *odu_struct,
                                     odu_struct_stream_dir_t direction,
                                     odu_struct_ext_mode_t ext_mode,
                                     UINT16 *gcc2_val);
PUBLIC PMC_ERROR odu_struct_bei_biae_get(coreotn_handle_t *coreotn_handle,
                                         odu_struct_t *odu_struct,
                                         odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                         odu_struct_ext_mode_t ext_mode,
                                         UINT8 *bei_biae_val);
PUBLIC PMC_ERROR odu_struct_bdi_get(coreotn_handle_t *coreotn_handle,
                                    odu_struct_t *odu_struct,
                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                    odu_struct_ext_mode_t ext_mode,
                                    UINT8 *bdi_val);
PUBLIC PMC_ERROR odu_struct_stat_get(coreotn_handle_t *coreotn_handle,
                                     odu_struct_t *odu_struct,
                                     odu_struct_stream_dir_t direction,
                                     odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                     odu_struct_ext_mode_t ext_mode,
                                     UINT8 *stat_val);

PUBLIC PMC_ERROR odu_struct_deg_th_get(coreotn_handle_t *coreotn_handle,
                                       odu_struct_t *odu_struct,
                                       odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                       odu_struct_ext_mode_t ext_mode,
                                       UINT32 *deg_th);
PUBLIC PMC_ERROR odu_struct_tim_det_mon_get(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                            odu_struct_ext_mode_t ext_mode,
                                            UINT8 *tim_det_mo);
PUBLIC PMC_ERROR odu_struct_tim_act_dis_get(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                            odu_struct_ext_mode_t ext_mode,
                                            BOOL8 *tim_act_dis);
PUBLIC PMC_ERROR odu_struct_chnl_lof_lom_act_dis_get(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_ext_mode_t ext_mode,
                                                     BOOL8 *lof_lom_act_dis);                                            
PUBLIC PMC_ERROR odu_struct_maint_sig_get(coreotn_handle_t *coreotn_handle,
                                          odu_struct_t *odu_struct,
                                          odu_struct_stream_dir_t direction,
                                          coreotn_maint_sig_type_t *maint_sig);
PUBLIC PMC_ERROR odu_struct_rx_expected_pt_get(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               UINT8 *exp_pt_val);
PUBLIC PMC_ERROR odu_struct_rx_accepted_pt_get(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               UINT8 *accpt_pt_val);
PUBLIC PMC_ERROR odu_struct_pm_tcm_res_get(coreotn_handle_t *coreotn_handle,
                                           odu_struct_t *odu_struct,
                                           odu_struct_ext_mode_t ext_mode,
                                           UINT8 *pm_tcm_res_val);
PUBLIC PMC_ERROR odu_struct_rx_partial_ho_accept_msi_init(coreotn_handle_t *coreotn_handle,
                                                          odu_struct_t *odu_struct,
                                                          BOOL init_msi);
PUBLIC PMC_ERROR odu_struct_rx_accpt_msi_get(coreotn_handle_t *coreotn_handle,
                                             odu_struct_t *odu_struct,
                                             odu_struct_ext_mode_t ext_mode,
                                             UINT8 *msi_byte);    
PUBLIC PMC_ERROR odu_struct_rx_accpt_msi_per_ts_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_ext_mode_t ext_mode,
                                                    UINT8 ts_port,
                                                    UINT8 *msi_byte);                                             
#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */ 
/* LCOV_EXCL_START */
PUBLIC PMC_ERROR odu_struct_prbs_dlss_status_get(coreotn_handle_t* coreotn_handle,
                                                 odu_struct_t *odu_struct,
                                                 UINT32 prbs_chnl, 
                                                 UINT8 *dlss_stat);   
/* LCOV_EXCL_STOP */
#endif/*#ifndef DOXYGEN_PUBLIC_ONLY*/
PUBLIC PMC_ERROR odu_struct_tx_oh_ins_src_get(coreotn_handle_t *coreotn_handle,
                                              odu_struct_t *odu_struct,
                                              odu_struct_optional_oh_ins_t oh_field,
                                              odu_struct_ext_mode_t ext_mode,
                                              UINT8 *oh_ins_src);                                                 
PUBLIC PMC_ERROR odu_struct_tx_aps_pcc_src_get(coreotn_handle_t *coreotn_handle,
                                               odu_struct_t *odu_struct,
                                               UINT8 mon_level,
                                               odu_struct_ext_mode_t ext_mode,
                                               UINT8 *aps_pcc_src);  
                                               
PUBLIC PMC_ERROR odu_struct_rx_tcmi_defect_stat_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                    UINT32 *defect_stat);
PUBLIC PMC_ERROR odu_struct_rx_pm_defect_stat_get(coreotn_handle_t *coreotn_handle,
                                                  odu_struct_t *odu_struct,
                                                  odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                                  UINT32 *defect_stat); 
PUBLIC PMC_ERROR odu_struct_tx_iae_stat_get(coreotn_handle_t *coreotn_handle,
                                            odu_struct_t *odu_struct,
                                            UINT32 *status);   
PUBLIC PMC_ERROR odu_struct_prbs_inst_get(coreotn_handle_t *coreotn_handle,
                                          odu_struct_t *odu_struct,
                                          coreotn_prbs_inst_t *prbs_inst);                                             
PUBLIC PMC_ERROR odu_struct_prbs_tfrm_cfg(coreotn_handle_t *coreotn_handle,
                                          odu_struct_t *odu_struct,
                                          BOOL8 enable);
PUBLIC PMC_ERROR odu_struct_otn_odu_dplm_to_ssf_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_ext_mode_t ext_mode,
                                                    BOOL8 *enable);

PUBLIC PMC_ERROR odu_struct_otn_odu_dmsim_to_ssf_get(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_ext_mode_t ext_mode,
                                                     BOOL8 *enable);

PUBLIC PMC_ERROR odu_struct_otn_odu_dloomfi_to_ssf_get(coreotn_handle_t *coreotn_handle,
                                                       odu_struct_t *odu_struct,
                                                       odu_struct_ext_mode_t ext_mode,
                                                       BOOL8 *enable);

PUBLIC PMC_ERROR odu_struct_defect_actions_ctrl_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    odu_struct_ext_mode_t ext_mode,
                                                    UINT8 *defect_action_value);

PUBLIC PMC_ERROR odu_struct_ais_dis_get(coreotn_handle_t *coreotn_handle,
                                        odu_struct_t *odu_struct,
                                        odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                        odu_struct_ext_mode_t ext_mode,
                                        UINT8 *ais_dis);
PUBLIC PMC_ERROR odu_struct_ebc_get(coreotn_handle_t *coreotn_handle,
                                    odu_struct_t *odu_struct,
                                    odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                    UINT32 ebc[2]);

PUBLIC PMC_ERROR odu_struct_deg_m_get(coreotn_handle_t *coreotn_handle,
                                              odu_struct_t *odu_struct,
                                              odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                              odu_struct_ext_mode_t ext_mode,
                                              UINT8 *deg_m);

PUBLIC PMC_ERROR odu_struct_deg_th_get(coreotn_handle_t *coreotn_handle,
                                              odu_struct_t *odu_struct,
                                              odu_struct_odu_frm_pm_tcm_id_t pm_tcm_num,
                                              odu_struct_ext_mode_t ext_mode,
                                              UINT32 *deg_th);

PUBLIC PMC_ERROR odu_struct_otn_opu_payload_get(coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct,
                                                util_global_odu_line_payload_t *payload_format);                                                       


PUBLIC PMC_ERROR odu_struct_tx_tcm_maint_sig_get(coreotn_handle_t *coreotn_handle,
                                                 odu_struct_t *odu_struct,
                                                 odu_struct_odu_frm_pm_tcm_id_t tcm_num,
                                                 odu_struct_ext_mode_t ext_mode,
                                                 coreotn_maint_sig_type_t *maint_sig);

PUBLIC PMC_ERROR odu_struct_rx_pm_tcmi_activate(coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct);                                                 
                                                 
/* ODU PM/TCMi layer hierarchy functions */
PUBLIC PMC_ERROR odu_struct_rx_odu_pm_tcmi_layer_get(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_odu_frm_layer_struct_t (*tcm_layers)[ODU_SINK_MAX_PM_TCM_LAYERS],
                                                     UINT32 *num_layers); 
PUBLIC PMC_ERROR odu_struct_tx_odu_pm_tcmi_layer_get(coreotn_handle_t *coreotn_handle,
                                                     odu_struct_t *odu_struct,
                                                     odu_struct_odu_frm_layer_struct_t (*tcm_layers)[ODU_SOURCE_MAX_PM_TCM_LAYERS],
                                                     UINT32 *num_layers); 
                                                     
PUBLIC PMC_ERROR odu_struct_tcm_pool_id_from_instance_get(coreotn_handle_t *coreotn_handle,
                                                          odu_struct_t *odu_struct,
                                                          odu_struct_stream_dir_t direction,
                                                          odu_struct_odu_frm_pm_tcm_id_t tcm_num,
                                                          UINT16 *tcm_pool_id);

/*
** Remote Information RCP functions
*/
PUBLIC PMC_ERROR odu_struct_ri_rcp_extract_cfg( coreotn_handle_t *coreotn_handle,
                                                odu_struct_t *odu_struct,
                                                digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr,
                                                UINT8 aps_pcc_mon_level,
                                                UINT32 *ch_entries,
                                                BOOL8 enable);
PUBLIC PMC_ERROR odu_struct_ri_rcp_extract_cfg_get( coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr,
                                                    UINT8 *aps_pcc_mon_level,
                                                    BOOL8 *enable);
PUBLIC PMC_ERROR odu_struct_ri_rcp_aps_pcc_mon_level_cfg( coreotn_handle_t *coreotn_handle,
                                                          odu_struct_t *odu_struct,
                                                          UINT8 aps_pcc_mon_level);
PUBLIC PMC_ERROR odu_struct_ri_rcp_dest_get( coreotn_handle_t *coreotn_handle,
                                             odu_struct_t *odu_struct,
                                             digi_otn_odu_ri_rcp_dst_cfg_t *ri_rcp_dst_cfg_ptr);
PUBLIC PMC_ERROR odu_struct_bdi_bei_biae_insert_cfg(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    UINT32 num_cal_entries,
                                                    BOOL8 source);
PUBLIC void odu_struct_bdi_bei_biae_status_get(coreotn_handle_t *coreotn_handle,
                                                    odu_struct_t *odu_struct,
                                                    UINT8 *source);
PUBLIC PMC_ERROR odu_struct_ri_rcp_aps_insert_cfg( coreotn_handle_t *coreotn_handle,
                                                   odu_struct_t *odu_struct, 
                                                   UINT32 num_cal_entries,                                
                                                   BOOL8 source);
PUBLIC PMC_ERROR odu_struct_ri_rcp_aps_insert_en_get( coreotn_handle_t *coreotn_handle,
                                                      odu_struct_t *odu_struct,                              
                                                      BOOL8 *source);

PUBLIC PMC_ERROR odu_struct_tx_maint_sig_dummy_rfrm_cfg(coreotn_handle_t *coreotn_handle,
                                                        odu_struct_odu_level_t odu_level,
                                                        UINT32 chnl_id,
                                                        BOOL8 enable);

PUBLIC PMC_ERROR odu_struct_otn_odu_cm_min_max_reprov(coreotn_handle_t *coreotn_handle,
                                                      odu_struct_t *odu_struct,
                                                      BOOL use_custom_ppm,
                                                      UINT32 ppm_offset);






#endif /* DOXYGEN_PUBLIC_ONLY */
#ifdef __cplusplus
}
#endif

#endif /* _ODU_STRUCT_API_H */

/*
** end of file
*/
