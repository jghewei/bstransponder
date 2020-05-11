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
*   DESCRIPTION : This file contains the definitions of register bit access
*   functions and computational operation functions for each subsystem.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _DIGI_OPSA_LOC_H
#define _DIGI_OPSA_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include File
*/


/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: digi_opsa_mpmo_target_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the timing target for MPMO.
*
* ELEMENTS:
*    DIGI_OPSA_MPMO_TARGET_ACB_LINE      - 0x1: Tx Serdes ACB LINE_S16      
*    DIGI_OPSA_MPMO_TARGET_ACB_SYS       - 0x2: Tx Serdes ACB SYS_S16       
*    DIGI_OPSA_MPMO_TARGET_ACB_SFI51     - 0x3: Tx Serdes ACB SFI5.1        
*    DIGI_OPSA_MPMO_TARGET_TS_STAGE_1    - 0x4: Stage 1 Mux Tributary Slots 
*    DIGI_OPSA_MPMO_TARGET_TS_STAGE_2    - 0x5: Stage 2 Mux Tributary Slots 
*    DIGI_OPSA_MPMO_TARGET_MAPPER_TX     - 0x6: Mapper Tx Path    
*
*******************************************************************************/
typedef enum
{
    DIGI_OPSA_MPMO_TARGET_ACB_LINE   = 1,
    DIGI_OPSA_MPMO_TARGET_ACB_SYS    = 2,
    DIGI_OPSA_MPMO_TARGET_ACB_SFI51  = 3,
    DIGI_OPSA_MPMO_TARGET_TS_STAGE_1 = 4,
    DIGI_OPSA_MPMO_TARGET_TS_STAGE_2 = 5,
    DIGI_OPSA_MPMO_TARGET_MAPPER_TX  = 6,

    LAST_DIGI_OPSA_MPMO_TARGET        /* this is for out of range values error handling */
} digi_opsa_mpmo_target_t;



/*******************************************************************************
* ENUM: digi_opsa_cfg_action_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the FW configuration operation
*   to be performed
*
* ELEMENTS:
*    DIGI_OPSA_CFG_ACTION_ACTIVATE    - Activate MPMA/MPMO for datapath
*    DIGI_OPSA_CFG_ACTION_DEACTIVATE  - Deactivate MPMA/MPMO for datapath
*    DIGI_OPSA_CFG_ACTION_RELEASE     - Drop/Release MPMA/MPMO for datapath
*
*******************************************************************************/
typedef enum
{
    DIGI_OPSA_CFG_ACTION_ACTIVATE   = 0,
    DIGI_OPSA_CFG_ACTION_DEACTIVATE, 
    DIGI_OPSA_CFG_ACTION_RELEASE,

    LAST_DIGI_OPSA_CFG_ACTION        /* this is for out of range values error handling */
} digi_opsa_cfg_action_t;


/*******************************************************************************
* ENUM: digi_opsa_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for defining the scope of resources within
*   a channel handle to process.
*
* ELEMENTS:
*    DIGI_OPSA_DIR_OUTBOUND - Operate on resources leaving CPB/ODUKSW
*    DIGI_OPSA_DIR_INBOUND  - Operate on resources entering CPB/ODUKSW
*    DIGI_OPSA_DIR_BIDIR    - Operate on resources entering and leaving CPB/ODUKSW
*
*******************************************************************************/
typedef enum
{
    DIGI_OPSA_DIR_OUTBOUND   = 0,
    DIGI_OPSA_DIR_INBOUND,
    DIGI_OPSA_DIR_BIDIR,

    LAST_DIGI_OPSA_DIR        /* this is for out of range values error handling */
} digi_opsa_dir_t;

/*
** Constants
*/


/*
** Macro Definitions
*/


/*
** Structures and Unions
*/


/*
** Global variables
*/

/*
** Function Prototypes
*/


PUBLIC void digi_opsa_cfg_mpmo_one_port_add(digi_handle_t *digi_handle,
                                            hostmsg_opsa_subsystem_cfg_t subsystem,
                                            UINT32 chnl,
                                            digi_opsa_mpmo_target_t data_target,
                                            UINT8 acb_inst,
                                            UINT8 ckctl_port_addr,
                                            BOOL8 mpmo_proc_type,
                                            BOOL8 cbrc_monitor_type,
                                            UINT8 coreotn_fo2_channel,
                                            UINT8 coreotn_fo2_ckctl_port,
                                            BOOL8 is_config_only);

PUBLIC void digi_opsa_cfg_mpmo_one_port_drop(digi_handle_t *digi_handle,
                                             hostmsg_opsa_subsystem_cfg_t subsystem,
                                             UINT32 chnl);


PUBLIC void digi_opsa_oduksw_activate_cfg(digi_handle_t *digi_handle,
                                          util_global_switch_data_t *input_pc_ptr,
                                          util_global_switch_data_t *output_pc_ptr,
                                          BOOL8 is_reader_primary,
                                          BOOL8 is_slave_zone_primary,
                                          digi_opsa_cfg_action_t cfg_action,
                                          BOOL activate_deactivate_mpmo,
                                          BOOL8 is_config_only);

PUBLIC void digi_opsa_cpb_activate_cfg(digi_handle_t *digi_handle,
                                       util_global_switch_data_t *input_pc_ptr, 
                                       util_global_switch_data_t *output_pc_ptr,
                                       BOOL8 is_primary_reader,
                                       BOOL8 is_slave_zone_primary,
                                       digi_opsa_cfg_action_t cfg_action,
                                       BOOL8 is_config_only);


PUBLIC void digi_opsa_cpb_port_release_cfg(digi_handle_t *digi_handle,
                                           util_global_cpb_port_t port_type,
                                           UINT32 *chnl_mask_ptr,
                                           digi_opsa_dir_t dir);

PUBLIC void digi_opsa_oduksw_port_release_cfg(digi_handle_t *digi_handle,
                                              util_global_oduk_port_t port_type,
                                              UINT32 *chnl_mask_ptr,
                                              digi_opsa_dir_t dir);

PUBLIC void digi_opsa_cfg_dcpb_mpma_chnl_backup_cfg(digi_handle_t *digi_handle,
                                                    hostmsg_opsa_subsystem_cfg_t subsystem,
                                                    UINT32 chnl,
                                                    BOOL8 action);

PUBLIC void digi_opsa_cfg_one_fmf1_txjc_cfc_fifo_clrb(digi_handle_t *digi_handle,
                                                      UINT32 chnl,
                                                      BOOL8 value);

PUBLIC void digi_opsa_chnl_to_chnl_msk(UINT32 chnl,
                                       UINT32 *chnl_msk_ptr);

PUBLIC void digi_opsa_cfg_one_tgmp_txjc_cfc_fifo_clrb(digi_handle_t *digi_handle,
                                                      UINT32 chnl,
                                                      BOOL8 value);

PUBLIC void digi_cpb_chnl_qs_ch_en_get(digi_handle_t *digi_handle,
                                       util_global_switch_data_t *input_pc_ptr,
                                       util_global_switch_data_t *output_pc_ptr,
                                       UINT8  *qs_ch_en_ptr);

PUBLIC void digi_opsa_chnl_hndl_for_port_chnl_get(digi_handle_t *digi_handle,
                                                  UINT32 port_id,
                                                  UINT32 chnl_id,
                                                  BOOL8 is_oduk_switch,
                                                  util_global_switch_data_t **chnl_hndl_pptr);

PUBLIC void digi_opsa_src_chnl_hndl_for_dest_get(digi_handle_t *digi_handle,
                                                 util_global_switch_data_t *dest_chnl_hndl_ptr,
                                                 BOOL8 is_oduk_switch,
                                                 util_global_switch_data_t **src_chnl_hndl_pptr);
                                                 
PUBLIC void digi_opsa_cfg_mpmo_all_chnl_add(digi_handle_t *digi_handle,
                                            hostmsg_opsa_subsystem_cfg_t subsystem,
                                            UINT32 output_chnl_list[96],
                                            digi_opsa_mpmo_target_t data_target[96],
                                            UINT8 acb_inst[96],
                                            UINT8 ckctl_port_addr[96],
                                            BOOL8 mpmo_proc_type,
                                            BOOL8 cbrc_monitor_type,
                                            UINT8 coreotn_fo2_channel,
                                            UINT8 coreotn_fo2_ckctl_port);   

PUBLIC void digi_opsa_cfg_mpmo_all_chnl_drop(digi_handle_t *digi_handle,
                                             hostmsg_opsa_subsystem_cfg_t subsystem,
                                             UINT32 chnl_list[96]);                                                                                          
                                                
PUBLIC void digi_opsa_oduksw_activate_cfg_all(digi_handle_t *digi_handle,
                                              util_global_switch_data_t **input_pc_ptr,
                                              util_global_switch_data_t **output_pc_ptr,
                                              BOOL *is_reader_primary,
                                              BOOL *is_slave_zone_primary,
                                              digi_opsa_cfg_action_t cfg_action);

PUBLIC PMC_ERROR digi_opsa_fw_update(digi_handle_t *digi_handle);


/*
** Computational Operation Functions
*/


/*
** Register Access Operaion
*/


/*
** Deprecated Functions
*/



#ifdef __cplusplus
}
#endif

#endif /* _DIGI_OPSA_LOC_H */

/*
** end of file
*/
