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

#ifndef _ODTU_MUX_H
#define _ODTU_MUX_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "odtu_mux_api.h"
#include "sts_sw.h"
#include "mp_mgen.h"
#include "txjc_proc.h"
#include "expand_ibuf.h"
#include "ohfs_insert.h"  
#include "tgmp.h"

/*
** Macro Definitions
*/
/* ODTU_MUX_INT_CHNL(handle, tsb_name, tsb_field, struct_field) */
#define ODTU_MUX_STATUS_CHNL_TABLE_DEFINE()                                  \
    ODTU_MUX_STATUS_CHNL_RANGE(odtu_mux_handle, odtu_mux, TXJC_RES_OVERFLOW_V, txjc_res_ovrflw_v); \

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: odtu_mux_chnl_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for specifying the state of a channel
*
* ELEMENTS:
*   ODTU_MUX_CHNL_START       - Channel have not been provisioned
*   ODTU_MUX_CHNL_EQUIPPED    - Channel has been provisioned, but no activated
*   ODTU_MUX_CHNL_OPERATIONAL - Channel is active
*   ODTU_MUX_CHNL_PASSTHRU    - Channel is in passthru mode (and is active)
*
*******************************************************************************/
typedef enum 
{
    ODTU_MUX_CHNL_START        = 0,
    ODTU_MUX_CHNL_EQUIPPED     = 1,
    ODTU_MUX_CHNL_OPERATIONAL  = 2,
    ODTU_MUX_CHNL_PASSTHRU     = 3,
    
    LAST_ODTU_MUX_CHNL_STATE,
} odtu_mux_chnl_state_t;


/*******************************************************************************
* ENUM: odtu_mux_chnl_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for specifying the mode of the HO channel
*
* ELEMENTS:
*   ODTU_MUX_CHNL_UNCHANNELIZED   - HO channel passes through block
*   ODTU_MUX_CHNL_CHANNELIZED     - HO channel is terminated, LO channels are
*                                   demultiplexed
*   ODTU_MUX_CHNL_NULL_FRAME_MODE - Null frame mode
*
*******************************************************************************/
typedef enum
{
    ODTU_MUX_CHNL_UNCHANNELIZED = 0,
    ODTU_MUX_CHNL_CHANNELIZED   = 1,
    ODTU_MUX_NULL_FRAME_MODE    = 2, 
} odtu_mux_chnl_mode_t;

/*******************************************************************************
* ENUM: odtu_mux_stage_instance_t
*______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for specifying the demux instance
*
* ELEMENTS:
*   ODTU_MUX_STG_NONE   - Demux operating in passthru mode
*   ODTU_MUX_STG_ONE    - 1st DMX stage (FMF1 for standard
*                         application, FMF2 for SYSOTN application)
*   ODTU_MUX_STG_TWO    - 2nd DMX stage (FMF1 for standard
*                         application, FMF2 for SYSOTN application)
*
*******************************************************************************/
typedef enum
{
    ODTU_MUX_STG_NONE  = 0,
    ODTU_MUX_STG_ONE   = 1,
    ODTU_MUX_STG_TWO   = 2,
} odtu_mux_stage_instance_t;

/*
** Constants
*/
#define ODTU_MUX_NUM_CHANL           96
#define ODTU_MUX_NUM_TS_IN_CHNL      80
#define ODTU_MUX_GHAO_NUM_CTXT       MAX_NUM_GHAO_SEGMENTS
#define ODTU_MUX_DB_ENTRY_UNKNOWN    0xFFFFFFFF
#define ODTU_MUX_STG1_MAX_NUM_HO_CHNL 12
/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: odtu_mux_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for describing static configuration
*
* ELEMENTS:
*   max_chnl_num  - maximum number of channels
*   db_id         - Database ID for the DMX
*   db_dev_addr   - DMX handle address for database registration
*   db_handle     - database handle
*
*******************************************************************************/
typedef struct {
    
    UINT32 max_chnl_num; 
    util_gen_db_id_t db_id;
    UINT32   db_handle;
    
} odtu_mux_cfg_t;

/*******************************************************************************
* STRUCTURE: odtu_mux_dummy_schd_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for storing the dummy schedule context (for MO ODU3e1/ODU3e2)
*
* ELEMENTS:
*   dummy_cal_entry - Dummy calendar entry
*   dummy_mo_ch_id  - Dummy MO channel id
*
*******************************************************************************/
typedef struct {
    UINT8 dummy_cal_entry;
    UINT8 dummy_mo_ch_id;
} odtu_mux_dummy_schd_t;    

/*******************************************************************************
* STRUCTURE: odtu_mux_dummy_schd_ctxt_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for storing the dummy schedule context (for ODU3)
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
} odtu_mux_dummy_schd_ctxt_t;

/*******************************************************************************
* STRUCTURE: ghao_mux_var_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for describing G.HAO state 
*
* ELEMENTS:
*   lcr_state      - State variable during LCR
*   bwr_state      - State variable during BWR
*   ho_id          - HO channel ID
*   lo_id          - LO channel ID
*   tpid           - tribport ID
*   ts             - array of ts representing the changing tribslots
*                    (calendar based)
*   old_ts         -  array of ts representing the old tribslots
*                    (calendar based)
*   new_ts         -  array of ts representing the new tribslots
*                    (calendar based)
*   mux_stage      - See odtu_mux_stage_instance_t
*   old_last_cs    - The old last calendar slot
*
*******************************************************************************/
typedef struct {
    util_global_ghao_lcr_state_t     lcr_state;
    util_global_ghao_bwr_state_t     bwr_state;
    UINT8                            ho_id;
    UINT8                            lo_id;
    UINT8                            tpid;
    BOOL8                            ts[ODTU_MUX_NUM_CHANL];  
    BOOL8                            old_ts[ODTU_MUX_NUM_CHANL];  
    BOOL8                            new_ts[ODTU_MUX_NUM_CHANL];  
    odtu_mux_stage_instance_t        mux_stage;
    UINT8                            old_last_cs; 
} ghao_mux_var_t;

/*******************************************************************************
* STRUCTURE: ghao_mux_var_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for dynamic context 
*
* ELEMENTS:
*   ho_chnl_cfg          - HO channel context
*   ho_chnl_cfg          - LO channel context
*   dummy_schd_entry     - Dummy schedule entry (for MO ODU3e2/ODU3e1)
*   dummy_schd_ctxt      - Dummy schdule context (for MO ODU3)
*   exp_first_cycle      - EXPAND IBUF first cycle
*   prov_mode            - TRUE : super prov mode
*                          FALSE : not super prov mode
*   offline_last_lo_chnl - used to keep track of last LO channel in super prov
*
*******************************************************************************/
typedef struct {
   
    util_global_odu_ho_chnl_cfg_t ho_chnl_cfg[ODTU_MUX_NUM_CHANL];
    util_global_odu_lo_chnl_cfg_t lo_chnl_cfg[ODTU_MUX_NUM_CHANL];
    /* this is for ODU3e2/ODU3e1 context */
    odtu_mux_dummy_schd_t dummy_schd_entry[UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD];
    /* this is for ODU3 context */
    odtu_mux_dummy_schd_ctxt_t dummy_schd_ctxt[UTIL_GLOBAL_MAX_DMX_DUMMY_SCHD];
    BOOL8 exp_first_cycle[UTIL_GLOBAL_MAX_NUM_CAL_ENTRY];
    BOOL8 prov_mode;
    BOOL8 offline_last_lo_chnl[ODTU_MUX_NUM_CHANL];
} odtu_mux_var_t;

/*******************************************************************************
* STRUCTURE: odtu_mux_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for ODTU_MUX handle
*
* ELEMENTS:
*   base           - see pmc_handle_t
*   tgmp_handle    - Pointer to tGMP handle
*   sts_sw_handle  - pointer to STS_SW handle
*   cfg            - Pointer to static configuration
*   var            - Pointer to dynamic configuration
*   ghao_mux_state - Point to G.HAO context
*
*******************************************************************************/
typedef struct odtu_mux_handle_struct_t {
    pmc_handle_t base;
    
    tgmp_handle_t *tgmp_handle;
    sts_sw_handle_t *sts_sw_handle;
    
    odtu_mux_cfg_t cfg;
    odtu_mux_var_t var;
    ghao_mux_var_t   *ghao_mux_state;
} odtu_mux_handle_struct_t;

/*******************************************************************************
* STRUCTURE: odtu_mux_lo_chnl_extra_ctxt_t
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
    UINT8 cal_array[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    UINT8 ts_array[UTIL_GLOBAL_MAX_NUM_TS];
    UINT32 tx_tribslot_mask[3];
} odtu_mux_lo_chnl_extra_ctxt_t;

/*******************************************************************************
* STRUCTURE: odtu_mux_recover_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for storing recovered variables during restart
*
* ELEMENTS:
*   var           - see odtu_mux_var_t
*   dmx_stage     - see odtu_mux_stage_instance_t
*   lo_chnl_extra - see odtu_mux_lo_chnl_extra_ctxt_t
*
*******************************************************************************/
typedef struct {
    odtu_mux_var_t var;    
    odtu_mux_stage_instance_t mux_stage;
    ohfs_insert_recover_var_t ohfs_insert_recover_var;
    odtu_mux_lo_chnl_extra_ctxt_t lo_chnl_extra[ODTU_MUX_NUM_CHANL];
} odtu_mux_recover_var_t;



/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC odtu_mux_handle_t *odtu_mux_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void odtu_mux_ctxt_destroy(odtu_mux_handle_t *odtu_mux_handle);
PUBLIC void odtu_mux_handle_init(odtu_mux_handle_t *odtu_mux_handle);
PUBLIC UINT32 odtu_mux_base_address_get(odtu_mux_handle_t *odtu_mux_handle);
PUBLIC PMC_ERROR odtu_mux_handle_restart_init(odtu_mux_handle_t *odtu_mux_handle,
                                              util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                              odtu_mux_stage_instance_t mux_state,
                                              pmc_energy_state_t top_energy_state_reg,                                           
                                              odtu_mux_recover_var_t *prev_stg_mux_var_reg,
                                              odtu_mux_recover_var_t *odtu_mux_var_reg);

PUBLIC PMC_ERROR odtu_mux_init(odtu_mux_handle_t *odtu_mux_handle, UINT32 enbl);

PUBLIC PMC_ERROR odtu_mux_chnl_init(odtu_mux_handle_t *odtu_mux_handle,
                                    UINT32 chnl_id,
                                    odtu_mux_chnl_mode_t mode);  


PUBLIC PMC_ERROR odtu_mux_ho_chnl_activate(odtu_mux_handle_t *odtu_mux_handle, 
                                           UINT32 chnl_id);

PUBLIC PMC_ERROR odtu_mux_ho_chnl_deactivate(odtu_mux_handle_t *odtu_mux_handle, 
                                             UINT32 chnl_id);

PUBLIC PMC_ERROR odtu_mux_lo_chnl_activate(odtu_mux_handle_t *odtu_mux_handle, 
                                           UINT32 chnl_id, UINT32 ho_chnl_id);
                                           
PUBLIC PMC_ERROR odtu_mux_lo_chnl_deactivate(odtu_mux_handle_t *odtu_mux_handle, 
                                             UINT32 chnl_id, UINT32 ho_chnl_id);

PUBLIC PMC_ERROR odtu_mux_ho_id_mem_cfg(odtu_mux_handle_t *odtu_mux_handle,
                                        UINT32 ho_chnl_id);
                                                                                                                                                                                                                   
PUBLIC PMC_ERROR odtu_mux_tribslot_cfg(odtu_mux_handle_t *odtu_mux_handle,
                                       odtu_mux_stage_instance_t stage_inst,
                                       UINT32 chnl_id, UINT32 cycle,
                                       UINT32 ts_port, UINT32 ho_chnl_id,
                                       UINT32 client_type,
                                       util_global_odukp_type_t server_type,
                                       util_global_odu_line_payload_t ts_type,
                                       util_global_mapping_mode_t mapping_mode,
                                       UINT32 first_ts, UINT32 last_ts,
                                       UINT32 frm_num, BOOL8 offline_update); 
                                       
PUBLIC PMC_ERROR odtu_mux_ho_chnl_prov(odtu_mux_handle_t *odtu_mux_handle,
                                       UINT32 mux_stage_id,
                                       UINT32 chnl_id, 
                                       util_global_odukp_type_t ho_odu_type,
                                       util_global_odu_line_payload_t ts_type,
                                       BOOL8 offline_update);

PUBLIC PMC_ERROR odtu_mux_lo_chnl_prov(odtu_mux_handle_t *odtu_mux_handle,
                                       UINT32 chnl_id,
                                       odtu_mux_stage_instance_t stage_inst,
                                       util_global_odukp_type_t lo_odu_type,
                                       UINT32 odu_flex_rate,
                                       util_global_mapping_mode_t mapping_mode,
                                       UINT32 trib_slot_mask[3],
                                       UINT32 msi_data,
                                       util_global_odu_line_payload_t lo_ts_type,
                                       util_global_odukp_type_t ho_odu_type,
                                       UINT32 ho_chnl_id,
                                       BOOL8 offline_update,
                                       UINT32 client_rate,
                                       BOOL ghao_reprov);
 
PUBLIC PMC_ERROR odtu_mux_ho_chnl_switch_cfg(odtu_mux_handle_t *odtu_mux_handle,
                                             UINT32 chnl_id,
                                             BOOL8 cfg_all); 
                                                                                                                       
PUBLIC PMC_ERROR odtu_mux_expand_ibuf_chnl_deactivate(odtu_mux_handle_t *odtu_mux_handle, 
                                                      UINT32 chnl);
                                         
PUBLIC PMC_ERROR odtu_mux_lo_chnl_deprov(odtu_mux_handle_t *odtu_mux_handle,
                                         odtu_mux_stage_instance_t stage_inst,
                                         UINT32 chnl_id, 
                                         UINT32 ho_chnl_id, BOOL8 offline_update,
                                         BOOL8 mo_odu3e2_odu3e1, 
                                         UINT8 extra_cycle);                                         
                                           
PUBLIC PMC_ERROR odtu_mux_ho_chnl_deprov(odtu_mux_handle_t *odtu_mux_handle,
                                         odtu_mux_stage_instance_t mux_stage,
                                         UINT32 chnl_id, BOOL8 offline_update);                                                                                                                                          

PUBLIC PMC_ERROR odtu_mux_lo_chnl_offline_update(odtu_mux_handle_t *odtu_mux_handle,
                                                 UINT32 chnl_id,
                                                 odtu_mux_stage_instance_t stage_inst,
                                                 util_global_odukp_type_t lo_odu_type,
                                                 UINT32 odu_flex_rate,
                                                 util_global_mapping_mode_t mapping_mode,
                                                 UINT32 trib_slot_mask[3],
                                                 UINT32 msi_data,
                                                 util_global_odu_line_payload_t ts_type,
                                                 util_global_odukp_type_t ho_odu_type,
                                                 UINT32 ho_chnl_id,
                                                 UINT32 client_rate);
                                                 
PUBLIC PMC_ERROR odtu_mux_ho_chnl_offline_update(odtu_mux_handle_t *odtu_mux_handle,
                                                 UINT32 mux_stage_id,
                                                 UINT32 chnl_id, util_global_odukp_type_t ho_odu_type,
                                                 util_global_odu_line_payload_t ts_type);     
                                                    
PUBLIC PMC_ERROR odtu_mux_config_update(odtu_mux_handle_t *odtu_mux_handle,
                                        UINT32 ho_chnl_id,
                                        UINT32 config_mode, UINT32 norm_cmd,
                                        UINT32 config_update);
                                        
PUBLIC PMC_ERROR odtu_mux_config_update_poll(odtu_mux_handle_t *odtu_mux_handle,
                                             UINT32 ho_chnl_id, UINT32 max_poll);
                                                 
PUBLIC PMC_ERROR odtu_mux_lo_chnl_offline_deprov(odtu_mux_handle_t *odtu_mux_handle,
                                                 odtu_mux_stage_instance_t stage_inst,
                                                 UINT32 chnl_id, 
                                                 UINT32 ho_chnl_id,
                                                 BOOL8 mo_odu3e2_odu3e1, 
                                                 UINT8 extra_cycle);
                                   
PUBLIC PMC_ERROR odtu_mux_dbg_mask_somf_sync_set(odtu_mux_handle_t *odtu_mux_handle, 
                                                 UINT32 chnl, UINT32 somf_sync);   

PUBLIC PMC_ERROR odtu_mux_config_update_set(odtu_mux_handle_t *odtu_mux_handle,
                                            UINT32 ho_chnl_id,
                                            UINT32 config_update); 

PUBLIC PMC_ERROR odtu_mux_null_chnl_cfg(odtu_mux_handle_t *odtu_mux_handle,
                                        UINT32 chnl_id,
                                        UINT32 null_frame_en);                                                                                                                                                                                                                                                             
PUBLIC void odtu_mux_activated_chnl_get(odtu_mux_handle_t *odtu_mux_handle,
                                        BOOL8 *actv_ho_chnl,
                                        BOOL8 *actv_lo_chnl);
                                        
PUBLIC PMC_ERROR odtu_mux_lo_chnl_activate_only(odtu_mux_handle_t *odtu_mux_handle, 
                                                UINT32 chnl_id);         
                                                  
PUBLIC PMC_ERROR odtu_mux_ohfs_insert_cfc_fifo_activate(odtu_mux_handle_t *odtu_mux_handle,
                                                        UINT32 chnl_id);    
                                                        
PUBLIC PMC_ERROR odtu_mux_ohfs_insert_cfc_fifo_deactivate(odtu_mux_handle_t *odtu_mux_handle,
                                                          UINT32 chnl_id);
                                                          
PUBLIC PMC_ERROR odtu_mux_ho_chnl_reset_set(odtu_mux_handle_t *odtu_mux_handle,
                                            UINT32 chnl_id, UINT32 chnl_state);
                                            
PUBLIC PMC_ERROR odtu_mux_lo_chnl_deactivate_only(odtu_mux_handle_t *odtu_mux_handle, 
                                                  UINT32 chnl_id, 
                                                  BOOL8 *dummy_ch);                                                                                                                                                                                                                                                                                        

PUBLIC PMC_ERROR odtu_mux_lo_chnl_deactivate_ch_state_chk_only(odtu_mux_handle_t *odtu_mux_handle, 
                                                               UINT32 chnl_id, UINT32 ho_chnl_id);
                                                          
PUBLIC void odtu_mux_ho_ch_id_get(odtu_mux_handle_t *odtu_mux_handle,
                                  UINT32 lo_chnl_id,
                                  UINT32 *ho_chnl_id);  
                                  
PUBLIC void odtu_mux_prov_mode_set(odtu_mux_handle_t *odtu_mux_handle,
                                   BOOL8 prov_mode);
                                   
PUBLIC PMC_ERROR odtu_mux_null_chan_mode_get(odtu_mux_handle_t *odtu_mux_handle,
                                             UINT32 chnl_id,
                                             BOOL8 *null_frame_en);                                   
                                 
/*
** G.HAO APIs 
*/                                 
PUBLIC PMC_ERROR odtu_mux_ghao_init(odtu_mux_handle_t *odtu_mux_handle,
                                    UINT8 ho_chnl_id,
                                    UINT8 lo_chnl_id,
                                    UINT8 tpid,
                                    BOOL8 ts[ODTU_MUX_NUM_CHANL],
                                    odtu_mux_stage_instance_t mux_stage,
                                    UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_send_add_nack(odtu_mux_handle_t *odtu_mux_handle,
                                             UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_send_add_ack(odtu_mux_handle_t *odtu_mux_handle,
                                            UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_send_remove_nack(odtu_mux_handle_t *odtu_mux_handle,
                                                UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_send_remove_ack(odtu_mux_handle_t *odtu_mux_handle,
                                               UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_send_tscc(odtu_mux_handle_t *odtu_mux_handle,
                                         BOOL8 tscc,
                                         UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_send_rp(odtu_mux_handle_t *odtu_mux_handle,
                                       BOOL8 rp,
                                       UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_send_norm(odtu_mux_handle_t *odtu_mux_handle,
                                         UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_disable_norm(odtu_mux_handle_t *odtu_mux_handle,
                                            UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_send_idle_nack(odtu_mux_handle_t *odtu_mux_handle,
                                              UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_update_stdby_config(odtu_mux_handle_t *odtu_mux_handle,
                                                   UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_resize_fifo(odtu_mux_handle_t *odtu_mux_handle,
                                           UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_adjust_xoff_lvl(odtu_mux_handle_t *odtu_mux_handle,
                                               UINT8 xoff_lvl,
                                               UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_recfg_passthru(odtu_mux_handle_t *odtu_mux_handle,
                                              UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_recfg_exp_lo_calen(odtu_mux_handle_t *odtu_mux_handle,
                                                  UINT32 cycle,
                                                  UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_end(odtu_mux_handle_t *odtu_mux_handle,
                                   UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_set_rcoh(odtu_mux_handle_t *odtu_mux_handle,
                                        BOOL8 ts[ODTU_MUX_NUM_CHANL],
                                        BOOL8 rp_flag,
                                        BOOL8 rp,
                                        util_global_ghao_lcr_ctrl_t ctrl,
                                        UINT8 tpid,
                                        BOOL8 tsgs,
                                        BOOL8 tscc);

PUBLIC PMC_ERROR odtu_mux_ghao_get_state(odtu_mux_handle_t *odtu_mux_handle,
                                         util_global_ghao_lcr_state_t *lcr_state,
                                         util_global_ghao_bwr_state_t *bwr_state,
                                         UINT8 *ho_chnl_id,
                                         UINT8 *lo_chnl_id,
                                         UINT8 *tpid,
                                         BOOL8 ts[ODTU_MUX_NUM_CHANL],
                                         BOOL8 old_ts[ODTU_MUX_NUM_CHANL],
                                         BOOL8 new_ts[ODTU_MUX_NUM_CHANL],
                                         odtu_mux_stage_instance_t *mux_stage,
                                         UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_set_state(odtu_mux_handle_t *odtu_mux_handle,
                                         util_global_ghao_lcr_state_t lcr_state,
                                         util_global_ghao_bwr_state_t bwr_state,
                                         UINT8 ho_chnl_id,
                                         UINT8 lo_chnl_id,
                                         UINT8 tpid,
                                         BOOL8 ts[ODTU_MUX_NUM_CHANL],
                                         BOOL8 old_ts[ODTU_MUX_NUM_CHANL],
                                         BOOL8 new_ts[ODTU_MUX_NUM_CHANL],
                                         odtu_mux_stage_instance_t mux_stage,
                                         UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_get_ts_num(odtu_mux_handle_t *odtu_mux_handle,
                                          UINT8 *ts_num,
                                          UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_get_old_ts_num(odtu_mux_handle_t *odtu_mux_handle,
                                              UINT8 *old_ts_num,
                                              UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_get_new_ts_num(odtu_mux_handle_t *odtu_mux_handle,
                                              UINT8 *new_ts_num,
                                              UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_get_new_ts_last_cycle(odtu_mux_handle_t *odtu_mux_handle,
                                                     UINT8 *new_ts_last_idx,
                                                     UINT8 ghao_index);

PUBLIC PMC_ERROR odtu_mux_ghao_lo_chnl_deprov(odtu_mux_handle_t  *odtu_mux_handle,
                                              odtu_mux_stage_instance_t mux_stage,
                                              UINT32 lo_chnl_id, UINT32 ho_chnl_id,
                                              BOOL ghao_inc,
                                              UINT32 num_old_ts,
                                              UINT8 *old_highest_cs);

PUBLIC PMC_ERROR odtu_mux_ghao_adjust_exp_xoff_lvl(odtu_mux_handle_t *odtu_mux_handle,
                                                   UINT32 lo_chnl_id,
                                                   UINT8 xoff_lvl);

PUBLIC PMC_ERROR odtu_mux_ghao_passthru_cal_enable_diable(odtu_mux_handle_t *odtu_mux_handle,
                                                          UINT32 cycle,
                                                          BOOL8 enable);

PUBLIC PMC_ERROR odtu_mux_int_chnl_enable(odtu_mux_handle_t     *odtu_mux_handle,
                                          UINT32                 chnl_id,
                                          odtu_mux_int_chnl_t   *int_table_ptr,
                                          odtu_mux_int_chnl_t   *int_en_table_ptr,
                                          BOOL8                  enable);

PUBLIC PMC_ERROR odtu_mux_int_chnl_clear(odtu_mux_handle_t      *odtu_mux_handle,
                                         UINT32                  chnl_id,
                                         odtu_mux_int_chnl_t    *int_table_ptr,
                                         odtu_mux_int_chnl_t    *int_en_table_ptr);

PUBLIC PMC_ERROR odtu_mux_int_chnl_retrieve(odtu_mux_handle_t      *odtu_mux_handle,
                                            UINT32                  chnl_id,
                                            odtu_mux_int_chnl_t    *filt_table_ptr,
                                            odtu_mux_int_chnl_t    *int_table_ptr);

PUBLIC PMC_ERROR odtu_mux_int_chnl_enabled_check(odtu_mux_handle_t    *odtu_mux_handle,
                                                 UINT32                chnl_id,
                                                 odtu_mux_int_chnl_t  *int_en_table_ptr,
                                                 BOOL8                *int_found_ptr);

PUBLIC void odtu_mux_lo_chnl_get(odtu_mux_handle_t *odtu_mux_handle,
                                      UINT32 ho_chnl_id,
                                      UINT32 *ch_ptr,
                                      UINT32 *num_ch); 




#ifdef __cplusplus
}
#endif

#endif /* _ODTU_MUX_H */

/*
** end of file
*/
