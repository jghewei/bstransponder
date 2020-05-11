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

#ifndef _ODTU_DMX_H
#define _ODTU_DMX_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "odtu_dmx_api.h"
#include "sts_sw.h"
#include "mp_mgen.h"
#include "rxjc_proc.h"
#include "cpact_obuf.h"
#include "ohfs_remove.h"  
#include "util_gen_db.h"
#include "util_gen_db_line_core_otn.h"
#include "rgmp.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: odtu_dmx_chnl_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for specifying the state of a channel
*
* ELEMENTS:
*   ODTU_DMX_CHNL_START       - Channel have not been provisioned
*   ODTU_DMX_CHNL_EQUIPPED    - Channel has been provisioned, but no activated
*   ODTU_DMX_CHNL_OPERATIONAL - Channel is active
*   ODTU_DMX_CHNL_PASSTHRU    - Channel is in passthru mode (and is active)
*
*******************************************************************************/
typedef enum 
{
    ODTU_DMX_CHNL_START        = 0,
    ODTU_DMX_CHNL_EQUIPPED     = 1,
    ODTU_DMX_CHNL_OPERATIONAL  = 2,
    ODTU_DMX_CHNL_PASSTHRU     = 3,
} odtu_dmx_chnl_state_t;

/*******************************************************************************
* ENUM: odtu_dmx_chnl_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for specifying the mode of the HO channel
*
* ELEMENTS:
*   ODTU_DMX_CHNL_UNCHANNELIZED   - HO channel passes through block
*   ODTU_DMX_CHNL_CHANNELIZED     - HO channel is terminated, LO channels are
*                                   demultiplexed
*   ODTU_DMX_CHNL_NON_INTRUSIVE   - Same as ODTU_DMX_CHNL_CHANNELIZED but
*                                   without the consequential actions
*   ODTU_DMX_CHNL_NULL_FRAME_MODE - Null frame mode
*
*******************************************************************************/
typedef enum
{
    ODTU_DMX_CHNL_UNCHANNELIZED     = 0,
    ODTU_DMX_CHNL_CHANNELIZED       = 1,
    ODTU_DMX_CHNL_NON_INTRUSIVE     = 2,
    ODTU_DMX_CHNL_NULL_FRAME_MODE   = 3,  
} odtu_dmx_chnl_mode_t;

/*******************************************************************************
* ENUM: odtu_dmx_stage_instance_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for specifying the demux instance
*
* ELEMENTS:
*   ODTU_DMX_STG_NONE   - Demux operating in passthru mode
*   ODTU_DMX_STG_ONE    - 1st DMX stage (FMF1 for standard
*                         application, FMF2 for SYSOTN application)
*   ODTU_DMX_STG_TWO    - 2nd DMX stage (FMF1 for standard
*                         application, FMF2 for SYSOTN application)
*
*******************************************************************************/
typedef enum
{
    ODTU_DMX_STG_NONE  = 0,
    ODTU_DMX_STG_ONE   = 1,
    ODTU_DMX_STG_TWO   = 2,
} odtu_dmx_stage_instance_t;

/*******************************************************************************
* ENUM: ghao_lcr_ctrl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for control word in RCOH:
*
* ELEMENTS:
*   GHAO_LCR_CTRL_IDLE   - control word sent after norm signalling
*   GHAO_LCR_CTRL_ADD    - Control word sent to add a tribslot
*   GHAO_LCR_CTRL_REMOVE - Control word sent to remove a triblsot
*   GHAO_LCR_CTRL_NORM   - Control word sent for NORM signalling (knife edge)
*   
*******************************************************************************/
typedef enum
{
    GHAO_LCR_CTRL_IDLE      = 0,
    GHAO_LCR_CTRL_ADD       = 1,
    GHAO_LCR_CTRL_REMOVE    = 2,
    GHAO_LCR_CTRL_NORM      = 3,
   
} ghao_lcr_ctrl_t;

/*******************************************************************************
* ENUM: ghao_lcr_ctrl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for control word in RCOH:
*
* ELEMENTS:
*   GHAO_LCR_CTRL_IDLE   - control word sent after norm signalling
*   GHAO_LCR_CTRL_ADD    - Control word sent to add a tribslot
*   GHAO_LCR_CTRL_REMOVE - Control word sent to remove a triblsot
*   GHAO_LCR_CTRL_NORM   - Control word sent for NORM signalling (knife edge)
*   
*******************************************************************************/
typedef enum
{
    GHAO_LCR_STATE_IDLE           = 0,
    GHAO_LCR_STATE_ADD_NACK       = 1,
    GHAO_LCR_STATE_ADD_ACK        = 2,
    GHAO_LCR_STATE_REMOVE_NACK    = 3,
    GHAO_LCR_STATE_REMOVE_ACK     = 4,
    GHAO_LCR_STATE_NORM           = 5,
   
} ghao_lcr_state_t;

/*******************************************************************************
* ENUM: ghao_bwr_state_t
*_______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for the BWR state
*
* ELEMENTS:
*   GHAO_BWR_STATE_IDLE - rp = 0
*   GHAO_BWR_STATE_LCR  - rp = 1, tscc = 0
*   GHAO_BWR_STATE_RAMP - rp = 1, tscc = 1
*   
*******************************************************************************/
typedef enum
{
    GHAO_BWR_STATE_IDLE         = 0,
    GHAO_BWR_STATE_LCR          = 1,
    GHAO_BWR_STATE_RAMP         = 2,
} ghao_bwr_state_t;

/*
** Constants
*/
#define ODTU_DMX_NUM_CHANL           96
#define ODTU_DMX_DB_ENTRY_UNKNOWN    0xFFFFFFFF
#define ODTU_DMX_GHAO_NUM_CTXT       MAX_NUM_GHAO_SEGMENTS

#define PSI_ODTU01      (0)
#define PSI_ODTU12      (0)
#define PSI_ODTU13      (0)
#define PSI_ODTU23      (1)
#define PSI_ODTU2_TS    (2)
#define PSI_ODTU3_TS    (2)
#define PSI_ODTU4_TS    (1)
    
/*
** Macro Definitions
*/
/* ODTU_DMX_INT(handle, tsb_name, tsb_field, struct_field) */
/* Interrupt/Status unchannalized */
#define ODTU_DMX_INT_TABLE_DEFINE() \
    ODTU_DMX_INT_SINGLE(odtu_dmx_handle, odtu_dmx, DLOOMFI   , dloomfi_i ); \

#define ODTU_DMX_STATUS_TABLE_DEFINE() \
    ODTU_DMX_STATUS_SINGLE(odtu_dmx_handle, odtu_dmx, DLOOMFI_V, dloomfi_v    ); \

/* ODTU_DMX_INT(handle, tsb_name, tsb_field, struct_field) */
/* Interrupt/Status to apply to HO/MO ODUs */
#define ODTU_DMX_INT_TABLE_HO_DEFINE() \
    ODTU_DMX_INT(odtu_dmx_handle, odtu_dmx, lfield_range, DPLM             , dplm_i             ); \
    ODTU_DMX_INT(odtu_dmx_handle, odtu_dmx, lfield_range, SSF_SF_RCVD      , ssf_sf_rcvd_i      ); \

#define ODTU_DMX_STATUS_TABLE_HO_DEFINE() \
    ODTU_DMX_STATUS(odtu_dmx_handle, odtu_dmx, lfield_range, DPLM_V          , dplm_v       ); \
    ODTU_DMX_STATUS(odtu_dmx_handle, odtu_dmx, lfield_range, SSF_SF_RCVD_V   , ssf_sf_rcvd_v); \



/* Interrupt/Status to apply to MO/LO ODUs */
#define ODTU_DMX_INT_TABLE_LO_DEFINE() \
    ODTU_DMX_INT(odtu_dmx_handle, odtu_dmx, lfield_range, CND_MAX          , cnd_max_i          ); \
    ODTU_DMX_INT(odtu_dmx_handle, odtu_dmx, lfield_range, CM_OUT_LIMIT     , cm_out_limit_i     ); \
    ODTU_DMX_INT(odtu_dmx_handle, odtu_dmx, lfield_range, JC_NOMTCH_LOSSYNC, jc_nomtch_lossync_i); \
    ODTU_DMX_INT(odtu_dmx_handle, odtu_dmx, lfield_range, DMSIM            , dmsim_i            ); \
    ODTU_DMX_INT(odtu_dmx_handle, odtu_dmx, lfield_range, CM_CND_CRC_ERR   , cm_cnd_crc_err_i   ); \
    ODTU_DMX_INT(odtu_dmx_handle, odtu_dmx, lfield_range, ERLY_PSERV_EXPRY , erly_pserv_expry_i ); \

#define ODTU_DMX_STATUS_TABLE_LO_DEFINE() \
    ODTU_DMX_STATUS(odtu_dmx_handle, odtu_dmx, lfield_range, DMSIM_V       , dmsim_v      ); \

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: odtu_dmx_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for describing static configuration
*
* ELEMENTS:
*   max_chnl_num  - maximum number of channels
*   rgmp_instance - Enum type to distinguish between RGMP/DMX
*   db_id         - Database ID for the DMX
*   db_dev_addr   - DMX handle address for database registration
*   db_handle     - database handle
*
*******************************************************************************/
typedef struct {
    
    /* To add static ODTU_DMX config context */
    UINT32 max_chnl_num;
    rgmp_instance_type_t rgmp_instance;
    util_gen_db_id_t db_id;
    UINT32   db_handle;
   
} odtu_dmx_cfg_t;

/*******************************************************************************
* STRUCTURE: ghao_dmx_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for describing G.HAO state 
*
* ELEMENTS:
*   lcr_state      - State variable during LCR
*   bwr_state      - State variable during BWR
*   discovery_mode - TRUE : DMX operates in G.HAO discovery mode
*                    FALSE : DMX does not operate in dicovery mode
*   channel_id     - Channel ID
*   rp             - TRUE : RP being received in RCOH is 1
*                    FALSE : RP being received in RCOH is 0
*   tscc           - TRUE : TSCC being received in RCOH is 1
*                    FALSE : TSCC being received in RCOH is 0
*   tsgs           - TRUE : TSGS being received in RCOH is 1
*                           TSGS bein received in TCOH is 0
*   ctrl           - RCOH CTRL word being received
*   same_rcoh      - TRUE : all capture RCOH on the different ts 
*                           have the same info
*                    FALSE : all capture RCOH on the different ts 
*                            do not have the same info
*
*******************************************************************************/
typedef struct {
    
    /* To add in var ODTU_DMX config context */
    ghao_lcr_state_t     lcr_state;
    ghao_bwr_state_t     bwr_state;
    BOOL8                discovery_mode;
    UINT8                channel_id;
    BOOL8                rp;
    BOOL8                tscc;
    BOOL8                tsgs;
    ghao_lcr_ctrl_t      ctrl;
    UINT8                tpid;
    BOOL8                same_rcoh;
 
} ghao_dmx_var_t;

/*******************************************************************************
* STRUCTURE: odtu_dmx_dummy_schd_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for storing the dummy schedule context
*
* ELEMENTS:
*   dummy_ch_id - Dummy channel ID
*   dummy_cycle - The calendar cycle that the dummy schedule occupies
*   dummy_state - TRUE : dummy schdedule is provisioned
*                 FALSE : dummy schedule is not provisioned
*
*******************************************************************************/
typedef struct {
    UINT8 dummy_ch_id;
    UINT8 dummy_cycle;
    BOOL8 dummy_state;
} odtu_dmx_dummy_schd_ctxt_t;

/*******************************************************************************
* STRUCTURE: odtu_dmx_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for storing dynamic context
*
* ELEMENTS:
*   ho_chnl_cfg       - context for HO channels
*   lo_chnl_cfg       - context for LO channels
*   mp_mpmgen_avl_ipt - array of available MP_MGEN IPT entries
*   ppm_offset        - configurabble +/- ppm tolerance for
*                       programmed CM_MIN/CM_MAX
*   dummy_schd_ctxt   - Dummy schedule context
*   isr_mode          - TRUE : G.HAO progresses through ISR
*                       FALSE : G.HAO progress through change polling
*                               summary bits
*   prov_mode         - TRUE : super prov enabled
*                       FALSE : super prov disabled
*
*******************************************************************************/
typedef struct {
    
    /* To add dynamic ODTU_DMX config context */
    util_global_odu_ho_chnl_cfg_t ho_chnl_cfg[ODTU_DMX_NUM_CHANL];
    util_global_odu_lo_chnl_cfg_t lo_chnl_cfg[ODTU_DMX_NUM_CHANL];
    UINT32 mp_mpmgen_avl_ipt[ODTU_DMX_NUM_CHANL];

    /* used to set a configurable ppm max collar for
    programming cm_min/cm_max */
    UINT8 ppm_offset[ODTU_DMX_NUM_CHANL];
    
    odtu_dmx_dummy_schd_ctxt_t dummy_schd_ctxt[UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD];
    BOOL8 isr_mode;
    BOOL8 prov_mode;
} odtu_dmx_var_t;

/*******************************************************************************
* STRUCTURE: odtu_dmx_lo_chnl_extra_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for storing additional LO channel context
*
* ELEMENTS:
*   cal_array        - array of calendar entries
*   ts_array         - array of triblsot
*   rx_tribslot_mask - tribslot mask on LO channel
*
*******************************************************************************/
typedef struct {
    UINT8 cal_array[UTIL_GLOBAL_MAX_NUM_TS];
    UINT8 ts_array[UTIL_GLOBAL_MAX_NUM_TS];
    UINT32 rx_tribslot_mask[3];
} odtu_dmx_lo_chnl_extra_ctxt_t;

/*******************************************************************************
* STRUCTURE: odtu_dmx_recover_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for storing recovered variables during restart
*
* ELEMENTS:
*   var           - see odtu_dmx_var_t
*   dmx_stage     - see odtu_dmx_stage_instance_t
*   lo_chnl_extra - see odtu_dmx_lo_chnl_extra_ctxt_t
*
*******************************************************************************/
typedef struct {
    odtu_dmx_var_t var;    
    odtu_dmx_stage_instance_t dmx_stage;
    ohfs_remove_recover_var_t ohfs_remove_recover_var;
    odtu_dmx_lo_chnl_extra_ctxt_t lo_chnl_extra[ODTU_DMX_NUM_CHANL];
} odtu_dmx_recover_var_t;


/*******************************************************************************
* STRUCTURE: odtu_dmx_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for ODTU_DMX handle
*
* ELEMENTS:
*   base           - see pmc_handle_t
*   rgmp_handle    - Pointer to RGMP handle
*   sts_sw_handle  - pointer to STS_SW handle
*   cfg            - Pointer to static configuration
*   var            - Pointer to dynamic configuration
*   ghao_dmx_state - Point to G.HAO context
*
*******************************************************************************/
typedef struct odtu_dmx_handle_struct_t {
    pmc_handle_t base;
    
    rgmp_handle_t *rgmp_handle;
    sts_sw_handle_t *sts_sw_handle;
    
    odtu_dmx_cfg_t cfg;
    odtu_dmx_var_t var;

    ghao_dmx_var_t *ghao_dmx_state;
 
} odtu_dmx_handle_struct_t;


/*******************************************************************************
* STRUCTURE: odtu_dmx_int_chnl_info_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for describing a channel id and its associated calendar slots and
*   tribslot ids for ODTU DMX interrupt operations
*
* ELEMENTS:
*   chnl_id         - Channel to access
*   higher_order    - TRUE for higher order ODUs (HO/MO).
*                     FALSE for lower order ODUs (MO/LO).
*   calendar_id     - Calendar entries to access
*   tribslot_id     - Tribslots associated with calendar entries
*   num_cal_entries - Number of calendar/tribslot entries in array
*
*******************************************************************************/
typedef struct {
    UINT32    chnl_id;
    BOOL8     higher_order;
    UINT32    calendar_id[ODTU_DMX_NUM_TS_IN_CHNL];
    UINT32    num_cal_entries;
} odtu_dmx_int_chnl_info_t;



/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC odtu_dmx_handle_t *odtu_dmx_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void odtu_dmx_ctxt_destroy(odtu_dmx_handle_t *odtu_dmx_handle);
PUBLIC void odtu_dmx_handle_init(odtu_dmx_handle_t *odtu_dmx_handle);
PUBLIC UINT32 odtu_dmx_base_address_get(odtu_dmx_handle_t *odtu_dmx_handle);

PUBLIC PMC_ERROR odtu_dmx_init(odtu_dmx_handle_t *odtu_dmx_handle, UINT32 enbl);
PUBLIC PMC_ERROR odtu_dmx_handle_restart_init(odtu_dmx_handle_t *odtu_dmx_handle,
                                              util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                              odtu_dmx_stage_instance_t dmx_state,
                                              pmc_energy_state_t top_energy_state_reg,
                                              odtu_dmx_recover_var_t *prev_stg_dmx_var_reg,
                                              odtu_dmx_var_t *odtu_dmx_var_clean,
                                              odtu_dmx_recover_var_t *odtu_dmx_var_reg);
PUBLIC void odtu_dmx_lo_chnl_get(odtu_dmx_handle_t *odtu_dmx_handle,
                                      UINT32 ho_chnl_id,
                                      UINT32 *ch_ptr,
                                      UINT32 *num_ch);
PUBLIC PMC_ERROR odtu_dmx_ctxt_cleanup(odtu_dmx_handle_t *odtu_dmx_handle,
                                       odtu_dmx_recover_var_t *odtu_dmx_var_reg,
                                       odtu_dmx_var_t *odtu_dmx_var_clean);
PUBLIC PMC_ERROR odtu_dmx_ho_chnl_reset_cfg(odtu_dmx_handle_t *odtu_dmx_handle,
                                            UINT32 chnl_id, UINT32 chnl_state);

PUBLIC PMC_ERROR odtu_dmx_lo_chnl_reset_cfg(odtu_dmx_handle_t *odtu_dmx_handle,
                                            UINT32 chnl_id, UINT32 chnl_state);

PUBLIC PMC_ERROR odtu_dmx_chnl_init(odtu_dmx_handle_t *odtu_dmx_handle,
                                    UINT32 chnl_id, 
                                    odtu_dmx_chnl_mode_t mode);
                                     
PUBLIC PMC_ERROR odtu_dmx_ho_chnl_prov(odtu_dmx_handle_t *odtu_dmx_handle,
                                       odtu_dmx_stage_instance_t stage_inst,
                                       UINT32 chnl_id,
                                       util_global_odukp_type_t ho_odu_type,
                                       util_global_odu_line_payload_t ts_type);
                                       
PUBLIC PMC_ERROR odtu_dmx_ho_chnl_activate(odtu_dmx_handle_t *odtu_dmx_handle, 
                                           UINT32 chnl_id);

PUBLIC PMC_ERROR odtu_dmx_ho_chnl_deactivate(odtu_dmx_handle_t *odtu_dmx_handle, 
                                             UINT32 chnl_id);   

PUBLIC PMC_ERROR odtu_dmx_lo_chnl_activate(odtu_dmx_handle_t *odtu_dmx_handle, 
                                           UINT32 chnl_id, UINT32 ho_chnl_id,
                                           odtu_dmx_stage_instance_t mux_stage);

PUBLIC PMC_ERROR odtu_dmx_lo_chnl_deactivate(odtu_dmx_handle_t *odtu_dmx_handle, 
                                             UINT32 chnl_id, UINT32 ho_chnl_id);

PUBLIC PMC_ERROR odtu_dmx_lo_chnl_prov(odtu_dmx_handle_t *odtu_dmx_handle,
                                       UINT32 chnl_id,
                                       odtu_dmx_stage_instance_t stage_inst,
                                       util_global_odukp_type_t lo_odu_type,
                                       UINT32 odu_flex_data,
                                       UINT32 cbr_client_rate,
                                       util_global_odukp_type_t ho_odu_type,
                                       util_global_mapping_mode_t mapping_mode,
                                       UINT32 trib_slot_mask[3],
                                       UINT32 msi_data,
                                       util_global_odu_line_payload_t lo_ts_type,
                                       UINT32 ho_chnl_id, BOOL8 offline_update,
                                       BOOL8 ghao_reprov);

PUBLIC PMC_ERROR odtu_dmx_tribslot_cfg(odtu_dmx_handle_t *odtu_dmx_handle,
                                       odtu_dmx_stage_instance_t stage_inst,
                                       UINT32 chnl_id, UINT32 ts_id,
                                       UINT32 ts_port, UINT32 ho_chnl_id,
                                       UINT32 client_type,
                                       util_global_odukp_type_t server_type,
                                       util_global_odu_line_payload_t ts_type,
                                       util_global_mapping_mode_t mapping_mode,
                                       UINT32 first_ts, UINT32 last_ts,
                                       UINT32 frm_num, BOOL8 offline_update,
                                       BOOL8 ghao_reprov);

PUBLIC PMC_ERROR odtu_dmx_ho_chnl_deprov(odtu_dmx_handle_t *odtu_dmx_handle,
                                         odtu_dmx_stage_instance_t mux_stage,
                                         UINT32 chnl_id,
                                         BOOL8 offline_update);

PUBLIC PMC_ERROR odtu_dmx_ho_chnl_switch_cfg(odtu_dmx_handle_t *odtu_dmx_handle,
                                             UINT32 chnl_id,
                                             BOOL8 cfg_all);

PUBLIC PMC_ERROR odtu_dmx_lo_chnl_offline_update(odtu_dmx_handle_t *odtu_dmx_handle,
                                                 UINT32 chnl_id,
                                                 odtu_dmx_stage_instance_t stage_inst,
                                                 util_global_odukp_type_t lo_odu_type,
                                                 UINT32 odu_flex_data,
                                                 UINT32 cbr_client_data,
                                                 util_global_odukp_type_t ho_odu_type,
                                                 util_global_mapping_mode_t mapping_mode,
                                                 UINT32 trib_slot_mask[3],
                                                 UINT32 msi_data,
                                                 util_global_odu_line_payload_t ts_type,
                                                 UINT32 ho_chnl_id);

PUBLIC PMC_ERROR odtu_dmx_config_update_done_poll(odtu_dmx_handle_t *odtu_dmx_handle,
                                                   UINT32 chnl_id, UINT32 max_poll);
PUBLIC PMC_ERROR odtu_dmx_config_update_done_int_clear(odtu_dmx_handle_t *odtu_dmx_handle,
                                                       UINT32 chnl_id);
PUBLIC PMC_ERROR odtu_dmx_reconfig_mode_cfg(odtu_dmx_handle_t *odtu_dmx_handle,
                                             UINT32 chnl_id,
                                             UINT32 config_mode);
PUBLIC PMC_ERROR odtu_dmx_reconfig_status_update(odtu_dmx_handle_t *odtu_dmx_handle,
                                                  UINT32 chnl_id, UINT32 update);
       
PUBLIC PMC_ERROR odtu_dmx_page_config_update(odtu_dmx_handle_t *odtu_dmx_handle,
                                             UINT32 chnl_id, 
                                             UINT32 norm_cmd,
                                             UINT32 config_mode,
                                             UINT32 update); 

PUBLIC PMC_ERROR odtu_dmx_config_update_poll(odtu_dmx_handle_t *odtu_dmx_handle,
                                            UINT32 chnl_id, UINT32 max_poll);

PUBLIC PMC_ERROR odtu_dmx_ho_chnl_offline_update(odtu_dmx_handle_t *odtu_dmx_handle,
                                                 odtu_dmx_stage_instance_t stage_inst,
                                                 UINT32 chnl_id,
                                                 util_global_odukp_type_t ho_odu_type,
                                                 util_global_odu_line_payload_t ts_type);
                                         
PUBLIC PMC_ERROR odtu_dmx_lo_chnl_deprov(odtu_dmx_handle_t  *odtu_dmx_handle,
                                         odtu_dmx_stage_instance_t mux_stage,
                                         UINT32 chnl_id, UINT32 ho_chnl_id,
                                         BOOL8 offline_update,
                                         BOOL8 mo_odu3e2_odu3e1,
                                         UINT8 extra_cycle);                                          

PUBLIC PMC_ERROR odtu_dmx_lo_chnl_offline_deprov(odtu_dmx_handle_t  *odtu_dmx_handle,
                                                 odtu_dmx_stage_instance_t mux_stage,
                                                 UINT32 chnl_id, UINT32 ho_chnl_id,
                                                 BOOL8 mo_odu3e2_odu3e1,
                                                 UINT8 extra_cycle);                                                             

PUBLIC PMC_ERROR odtu_dmx_int_enable(odtu_dmx_handle_t    *odtu_dmx_handle,
                                     odtu_dmx_int_t       *int_table_ptr,
                                     odtu_dmx_int_t       *int_en_table_ptr,
                                     BOOL8                       enable);

PUBLIC PMC_ERROR odtu_dmx_int_clear(odtu_dmx_handle_t          *odtu_dmx_handle,
                                    odtu_dmx_int_t        *int_table_ptr);

PUBLIC PMC_ERROR odtu_dmx_int_retrieve(odtu_dmx_handle_t    *odtu_dmx_handle,
                                       odtu_dmx_int_t       *filt_table_ptr,
                                       odtu_dmx_int_t       *int_table_ptr);

PUBLIC PMC_ERROR odtu_dmx_int_enabled_check(odtu_dmx_handle_t     *odtu_dmx_handle,
                                            odtu_dmx_int_t        *int_en_table_ptr,
                                            BOOL8                       *int_found_ptr);

PUBLIC PMC_ERROR odtu_dmx_int_chnl_enable(odtu_dmx_handle_t         *odtu_dmx_handle,
                                          odtu_dmx_int_chnl_info_t  *chnl_info,
                                          odtu_dmx_int_chnl_t       *int_table_ptr,
                                          odtu_dmx_int_chnl_t       *int_en_table_ptr,
                                          BOOL8                       enable);

PUBLIC PMC_ERROR odtu_dmx_int_chnl_clear(odtu_dmx_handle_t          *odtu_dmx_handle,
                                         odtu_dmx_int_chnl_info_t   *chnl_info,
                                         odtu_dmx_int_chnl_t        *int_table_ptr);

PUBLIC PMC_ERROR odtu_dmx_int_chnl_retrieve(odtu_dmx_handle_t         *odtu_dmx_handle,
                                            odtu_dmx_int_chnl_info_t  *chnl_info,
                                            odtu_dmx_int_chnl_t       *filt_table_ptr,
                                            odtu_dmx_int_chnl_t       *int_table_ptr);

PUBLIC PMC_ERROR odtu_dmx_int_chnl_enabled_check(odtu_dmx_handle_t          *odtu_dmx_handle,
                                                 odtu_dmx_int_chnl_info_t   *chnl_info,
                                                 odtu_dmx_int_chnl_t        *int_en_table_ptr,
                                                 BOOL8                       *int_found_ptr);

PUBLIC void odtu_dmx_activated_chnl_get(odtu_dmx_handle_t *odtu_dmx_handle,
                                        BOOL8 *actv_ho_chnl,
                                        BOOL8 *actv_lo_chnl);
PUBLIC odtu_dmx_chnl_mode_t odtu_dmx_chnl_mode_get(odtu_dmx_handle_t *odtu_dmx_handle,
                                                   UINT32 chnl_id);

PUBLIC PMC_ERROR odtu_dmx_dummy_schd_prov_num_get(odtu_dmx_handle_t *odtu_dmx_handle,
                                                  UINT32 *num_dummy_schd);

PUBLIC PMC_ERROR odtu_dmx_ho_null_frame_cfg(odtu_dmx_handle_t *odtu_dmx_handle,
                                            UINT32 chnl_id, UINT32 null_frame_mode);

PUBLIC PMC_ERROR odtu_dmx_dummy_schd_get(odtu_dmx_handle_t *odtu_dmx_handle,
                                         UINT32 dmx_schd_idx,
                                         UINT8 *mo_chnl_id,
                                         UINT8 *calendar_entry);

PUBLIC PMC_ERROR odtu_dmx_first_valid_dummy_find(odtu_dmx_handle_t *odtu_dmx_handle,
                                                  UINT32 first_cycle, 
                                                  UINT32 *valid_null_cycle,
                                                  UINT32 *dummy_idx);

PUBLIC PMC_ERROR odtu_dmx_dummy_schd_entry_reset(odtu_dmx_handle_t *odtu_dmx_handle,
                                                 UINT8 chnl_id);                                                  

PUBLIC PMC_ERROR odtu_dmx_ho_config_addr_st_id_set(odtu_dmx_handle_t *odtu_dmx_handle,
                                                   UINT32 ho_ch_id);
                                                   
PUBLIC void odtu_dmx_ho_ch_id_get(odtu_dmx_handle_t *odtu_dmx_handle,
                                  UINT32 lo_chnl_id,
                                  UINT32 *ho_chnl_id);   
                                  
PUBLIC void odtu_dmx_prov_mode_set(odtu_dmx_handle_t *odtu_dmx_handle,
                                   BOOL8 prov_mode);                                                                               

PUBLIC void odtu_dmx_dump_var_context(odtu_dmx_handle_t *hndl, UINT8 ho_chnl_id, UINT8 lo_chnl_id, BOOL8 all_chnl);

PUBLIC PMC_ERROR odtu_dmx_temp_mo_chnl_prov(odtu_dmx_handle_t *odtu_dmx_handle,
                                            odtu_dmx_stage_instance_t stage_inst,
                                            UINT32 chnl_id,
                                            util_global_odukp_type_t ho_odu_type,
                                            util_global_odu_line_payload_t ts_type);

PUBLIC void odtu_dmx_ghao_enable(odtu_dmx_handle_t *odtu_dmx_handle,
                                      UINT8 chnl_id,
                                      BOOL8 ho_rcoh_enable,
                                      BOOL8 flex_rcoh_enable);

PUBLIC void odtu_dmx_ghao_isr_enable(odtu_dmx_handle_t *odtu_dmx_handle,
                                          BOOL8 enable);

PUBLIC PMC_ERROR odtu_dmx_ghao_wait_event(odtu_dmx_handle_t *odtu_dmx_handle,
                                          UINT8 *ho_chnl_id,
                                          UINT8 *lo_chnl_id,
                                          BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                          BOOL8 *ref_rp,
                                          ghao_lcr_ctrl_t *ref_ctrl,
                                          UINT8 *ref_tpid,
                                          BOOL8 *ref_tsgs,
                                          BOOL8 *ref_tscc,
                                          UINT8 *first_ts,
                                          BOOL8 *same_rcoh);

PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_add_nack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                 UINT8 *ho_chnl_id,
                                                 UINT8 *lo_chnl_id,
                                                 UINT8 *tpid,
                                                 BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);
PUBLIC PMC_ERROR odtu_dmx_ghao_update_stdby_config(odtu_dmx_handle_t *odtu_dmx_handle,
                                                   UINT8 *ho_chnl_id,
                                                   UINT8 *lo_chnl_id,
                                                   BOOL8 ghao_trib_slot[ODTU_DMX_NUM_CHANL],
                                                   odtu_dmx_stage_instance_t stage_inst,
                                                   BOOL8 add,
                                                   UINT32 old_trib_slot_mask[3],
                                                   UINT8 msi_data);

PUBLIC PMC_ERROR odtu_dmx_ghao_update_stdby_config_after_knife_edge(odtu_dmx_handle_t *odtu_dmx_handle,
                                                                    odtu_dmx_stage_instance_t stage_inst,
                                                                    UINT8 ho_chnl_id,
                                                                    UINT8 lo_chnl_id,
                                                                    UINT32 num_ts,
                                                                    UINT32 new_tribslot_mask[3],
                                                                    UINT8 msi_data,
                                                                    BOOL8 add,
                                                                    UINT32 cal_to_add_remove_num,
                                                                    UINT32 cal_slots_add_rem[79]);

PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_add_ack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                UINT8 *ho_chnl_id,
                                                UINT8 *lo_chnl_id,
                                                UINT8 *tpid,
                                                BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);
PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_norm_ack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                 UINT8 *ho_chnl_id,
                                                 UINT8 *lo_chnl_id,
                                                 UINT8 *tpid,
                                                 BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);
PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_idle_nack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                  UINT8 *ho_chnl_id,
                                                  UINT8 *lo_chnl_id,
                                                  UINT8 *tpid,
                                                  BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);
PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_tscc(odtu_dmx_handle_t *odtu_dmx_handle,
                                             UINT8 *ho_chnl_id,
                                             UINT8 *lo_chnl_id,
                                             UINT8 *tpid,
                                             BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                             BOOL8 *tscc);
PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_rp(odtu_dmx_handle_t *odtu_dmx_handle,
                                           UINT8 *ho_chnl_id,
                                           UINT8 *lo_chnl_id,
                                           UINT8 *tpid,
                                           BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);
PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_remove_nack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                    UINT8 *ho_chnl_id,
                                                    UINT8 *lo_chnl_id,
                                                    UINT8 *tpid,
                                                    BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);

PUBLIC PMC_ERROR odtu_dmx_ghao_wait_for_remove_ack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                   UINT8 *ho_chnl_id,
                                                   UINT8 *lo_chnl_id,
                                                   UINT8 *tpid,
                                                   BOOL8 trib_slot[ODTU_DMX_NUM_CHANL]);

PUBLIC PMC_ERROR odtu_dmx_transfer_rcoh_status(odtu_dmx_handle_t *odtu_dmx_handle);

PUBLIC PMC_ERROR odtu_dmx_chk_summary_ghao_interrupt(odtu_dmx_handle_t *odtu_dmx_handle,
                                                     BOOL8 *cfg_dne_0_31,
                                                     BOOL8 *cfg_dne_32_63,
                                                     BOOL8 *cfg_dne_64_95,                                                     
                                                     BOOL8 *hao_st_avl_0_31,
                                                     BOOL8 *hao_st_avl_32_63,
                                                     BOOL8 *hao_st_avl_64_95,
                                                     BOOL8 *rcoh_st_trsnfr_dne);

PUBLIC PMC_ERROR odtu_dmx_get_hao_ts(odtu_dmx_handle_t *odtu_dmx_handle,
                                     BOOL8 hao_st_avl_0_31,
                                     BOOL8 hao_st_avl_32_63,
                                     BOOL8 hao_st_avl_64_95,
                                     UINT8 *ho_chnl_id,
                                     BOOL8 l_hao_cs[ODTU_DMX_NUM_CHANL],
                                     BOOL8 l_hao_ts[ODTU_DMX_NUM_CHANL]);

PUBLIC PMC_ERROR odtu_dmx_get_rcoh(odtu_dmx_handle_t *odtu_dmx_handle,
                                   UINT8 hao_ts,
                                   BOOL8 *rp,
                                   ghao_lcr_ctrl_t *ctrl,
                                   UINT8 *tpid,
                                   BOOL8 *tsgs,
                                   BOOL8 *tscc);

PUBLIC void odtu_dmx_ghao_get_state(odtu_dmx_handle_t *odtu_dmx_handle,
                                         ghao_lcr_state_t *lcr_state,
                                         ghao_bwr_state_t *bwr_state,
                                         BOOL8 *discovery_mode,
                                         UINT8 *channel_id,
                                         UINT8 *tpid,
                                         BOOL8 ts[ODTU_DMX_NUM_CHANL],
                                         BOOL8 *rp,
                                         BOOL8 *tscc,
                                         BOOL8 *tsgs,
                                         ghao_lcr_ctrl_t *ctrl,
                                         BOOL8 *same_rcoh);

PUBLIC void odtu_dmx_ghao_update_state(odtu_dmx_handle_t *odtu_dmx_handle,
                                            ghao_lcr_state_t lcr_state,
                                            ghao_bwr_state_t bwr_state,
                                            BOOL8 discovery_mode,
                                            UINT8 channel_id,
                                            UINT8 tpid,
                                            BOOL8 ts[ODTU_DMX_NUM_CHANL],
                                            BOOL8 rp,
                                            BOOL8 tscc,
                                            BOOL8 tsgs,
                                            ghao_lcr_ctrl_t ctrl,
                                            BOOL8 same_rcoh);

PUBLIC PMC_ERROR odtu_dmx_ghao_chk_tpid(odtu_dmx_handle_t *odtu_dmx_handle,
                                        UINT8 exp_tpid,
                                        UINT8 new_tpid,
                                        UINT8 ts);

PUBLIC PMC_ERROR odtu_dmx_ghao_chk_ctrl(odtu_dmx_handle_t *odtu_dmx_handle,
                                        ghao_lcr_ctrl_t exp_ctrl,
                                        ghao_lcr_ctrl_t new_ctrl,
                                        UINT8 ts);

PUBLIC PMC_ERROR odtu_dmx_ghao_chk_tsgs(odtu_dmx_handle_t *odtu_dmx_handle,
                                        BOOL8 exp_tsgs,
                                        BOOL8 new_tsgs,
                                        UINT8 ts);

PUBLIC PMC_ERROR odtu_dmx_ghao_chk_tscc(odtu_dmx_handle_t *odtu_dmx_handle,
                                        BOOL8 exp_tscc,
                                        BOOL8 new_tscc,
                                        UINT8 ts);
PUBLIC PMC_ERROR odtu_dmx_ghao_chk_rp(odtu_dmx_handle_t *odtu_dmx_handle,
                                      BOOL8 exp_rp,
                                      BOOL8 new_rp,
                                      UINT8 ts);

PUBLIC PMC_ERROR odtu_dmx_ghao_lo_chnl_deprov(odtu_dmx_handle_t  *odtu_dmx_handle,
                                              odtu_dmx_stage_instance_t mux_stage,
                                              UINT32 lo_chnl_id, UINT32 ho_chnl_id );

PUBLIC PMC_ERROR odtu_dmx_chk_cfg_dne( odtu_dmx_handle_t  *odtu_dmx_handle,
                                       UINT8 ho_chnl_id);


PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_event(odtu_dmx_handle_t *odtu_dmx_handle,
                                                UINT8 *ho_chnl_id,
                                                UINT8 *lo_chnl_id,
                                                BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                BOOL8 *ref_rp,
                                                ghao_lcr_ctrl_t *ref_ctrl,
                                                UINT8 *ref_tpid,
                                                BOOL8 *ref_tsgs,
                                                BOOL8 *ref_tscc,
                                                UINT8 *first_ts,
                                                BOOL8 *same_rcoh,
                                                UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_add_nack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                       UINT8 *ho_chnl_id,
                                                       UINT8 *lo_chnl_id,
                                                       UINT8 *tpid,
                                                       BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                       BOOL8 *same_rcoh,
                                                       UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_dmx_multi_ghao_update_stdby_config(odtu_dmx_handle_t *odtu_dmx_handle,
                                                         UINT8 *ho_chnl_id,
                                                         UINT8 *lo_chnl_id,
                                                         BOOL8 ghao_trib_slot[ODTU_DMX_NUM_CHANL],
                                                         odtu_dmx_stage_instance_t stage_inst,
                                                         BOOL8 add,
                                                         UINT32 old_trib_slot_mask[3],
                                                         UINT8 msi_data,
                                                         UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_add_ack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                      UINT8 *ho_chnl_id,
                                                      UINT8 *lo_chnl_id,
                                                      UINT8 *tpid,
                                                      BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                      BOOL8 *same_rcoh,
                                                      UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_norm_ack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                       UINT8 *ho_chnl_id,
                                                       UINT8 *lo_chnl_id,
                                                       UINT8 *tpid,
                                                       BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                       BOOL8 *same_rcoh,
                                                       UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_idle_nack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                        UINT8 *ho_chnl_id,
                                                        UINT8 *lo_chnl_id,
                                                        UINT8 *tpid,
                                                        BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                        BOOL8 *same_rcoh,
                                                        UINT8 ghao_index );

PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_tscc(odtu_dmx_handle_t *odtu_dmx_handle,
                                                   UINT8 *ho_chnl_id,
                                                   UINT8 *lo_chnl_id,
                                                   UINT8 *tpid,
                                                   BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                   BOOL8 *tscc,
                                                   BOOL8 *same_rcoh,
                                                   UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_rp(odtu_dmx_handle_t *odtu_dmx_handle,
                                                 UINT8 *ho_chnl_id,
                                                 UINT8 *lo_chnl_id,
                                                 UINT8 *tpid,
                                                 BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                 BOOL8 *same_rcoh,
                                                 UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_remove_nack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                          UINT8 *ho_chnl_id,
                                                          UINT8 *lo_chnl_id,
                                                          UINT8 *tpid,
                                                          BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                          BOOL8 *same_rcoh,
                                                          UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_dmx_multi_ghao_wait_for_remove_ack(odtu_dmx_handle_t *odtu_dmx_handle,
                                                         UINT8 *ho_chnl_id,
                                                         UINT8 *lo_chnl_id,
                                                         UINT8 *tpid,
                                                         BOOL8 trib_slot[ODTU_DMX_NUM_CHANL],
                                                         BOOL8 *same_rcoh,
                                                         UINT8 ghao_index);    
 
PUBLIC void odtu_dmx_multi_ghao_get_state(odtu_dmx_handle_t *odtu_dmx_handle,
                                               ghao_lcr_state_t *lcr_state,
                                               ghao_bwr_state_t *bwr_state,
                                               BOOL8 *discovery_mode,
                                               UINT8 *channel_id,
                                               UINT8 *tpid,
                                               BOOL8 ts[ODTU_DMX_NUM_CHANL],
                                               BOOL8 *rp,
                                               BOOL8 *tscc,
                                               BOOL8 *tsgs,
                                               ghao_lcr_ctrl_t *ctrl,
                                               BOOL8 *same_rcoh,
                                               UINT8 ghao_index);

PUBLIC void odtu_dmx_multi_ghao_update_state(odtu_dmx_handle_t *odtu_dmx_handle,
                                                  ghao_lcr_state_t lcr_state,
                                                  ghao_bwr_state_t bwr_state,
                                                  BOOL8 discovery_mode,
                                                  UINT8 channel_id,
                                                  UINT8 tpid,
                                                  BOOL8 ts[ODTU_DMX_NUM_CHANL],
                                                  BOOL8 rp,
                                                  BOOL8 tscc,
                                                  BOOL8 tsgs,
                                                  ghao_lcr_ctrl_t ctrl,
                                                  BOOL8 same_rcoh,
                                                  UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_dmx_cm_min_max_reprov(odtu_dmx_handle_t *odtu_dmx_handle,
                                            UINT32 chnl_id,
                                            util_global_odukp_type_t lo_odu_type,
                                            UINT32 cbr_client_rate,
                                            util_global_odukp_type_t ho_odu_type,
                                            util_global_mapping_mode_t mapping_mode,
                                            UINT32 num_ts,
                                            BOOL use_custom_ppm,
                                            UINT32 ppm_offset);


PUBLIC void odtu_dmx_cm_status_get(odtu_dmx_handle_t *odtu_dmx_handle,
                                   UINT32 chnl_id,
                                   UINT32 *value);




#ifdef __cplusplus
}
#endif

#endif /* _ODTU_DMX_H */

/*
** end of file
*/
