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

#ifndef _ODU_TFRM_H
#define _ODU_TFRM_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_log.h"
#include "odu_tfrm_api.h"
#include "util_global.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
#define ODU_TFRM_UNUSED_POOL_ID 0xFFFF
#define ODU_TFRM_MAX_CHAN_NUM   96

/*
** Macro Definitions
*/
/* ODU_TFRM_INT_CHNL(handle, tsb_name, tsb_field, struct_field) */
#define ODU_TFRM_INT_TABLE_DEFINE()                                  \
    ODU_TFRM_INT_SINGLE(odu_tfrm_handle, odu_tfrm, DM_FIFO_UDR, dm_fifo_udr_i); \
    ODU_TFRM_INT_SINGLE(odu_tfrm_handle, odu_tfrm, DM_FIFO_OVR, dm_fifo_ovr_i); \

/* ODU_TFRM_INT_CHNL(handle, tsb_name, tsb_field, struct_field) */
#define ODU_TFRM_INT_CHNL_TABLE_DEFINE()                                  \
    ODU_TFRM_INT_CHNL_RANGE(odu_tfrm_handle, odu_tfrm, IAE, iae_i); \


/* ODU_TFRM_INT_CHNL(handle, tsb_name, tsb_field, struct_field) */
#define ODU_TFRM_STATUS_CHNL_TABLE_DEFINE()                             \
    ODU_TFRM_STATUS_CHNL_RANGE(odu_tfrm_handle, odu_tfrm, IAE_V, iae_v);     \
    

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: odu_tfrm_tcm_pool_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to illustrate the TCM POOL ID context table.
*
* ELEMENTS:
*   assign_state    - state of the TCM POOL ID
*                     0 - free (TCM is not operational)
*                     1 - assigned (TCM is operational)
*   chnl_id         - channel ID that the TCM belongs to
*   tcm_num         - TCM field instance. See odu_tfrm_pm_tcm_id_t.
*   child_tcm_num   - TCM instance of the child. See odu_tfrm_pm_tcm_id_t.
*   child_pool_id   - pool ID of the child layer
*   odukt_maint_sig - 0 -- no maintenance signal forced
*                     1 -- Force ODUK_AIS
*                     2 -- Force ODUK_LCK
*                     3 -- Force ODUK_OCI    
*******************************************************************************/
typedef struct
{
    BOOL8 assign_state;
    UINT32 chnl_id;
    odu_tfrm_pm_tcm_id_t tcm_num;
    odu_tfrm_pm_tcm_id_t child_tcm_num; 
    UINT16 child_pool_id;

    UINT8 odukt_maint_sig;
} odu_tfrm_tcm_pool_t;

/*******************************************************************************
* STRUCTURE: odu_tfrm_pm_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to illustrate the PM context.
*
* ELEMENTS:
*   assign_state    - state of the PM layer
*                     0 - free (PM is not operational)
*                     1 - assigned (PM is operational)
*   child_tcm_num   - TCM instance of the child. See odu_tfrm_pm_tcm_id_t.
*   child_pool_id   - pool ID of the child layer
*******************************************************************************/
typedef struct
{
    BOOL8 assigned_state;
    odu_tfrm_pm_tcm_id_t child_tcm_num;
    UINT16 child_pool_id;
} odu_tfrm_pm_ctxt_t;

/*******************************************************************************
* STRUCTURE: odu_tfrm_layer_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to illustrate the current layer hierarchy in odu_tfrm.
*
* ELEMENTS:
*   pool_id     - pool ID of TCM layer
*   pm_tcm_num  - TCM field instance. See odu_tfrm_pm_tcm_id_t.
*******************************************************************************/
typedef struct
{

    UINT32 pool_id;
    odu_tfrm_pm_tcm_id_t pm_tcm_num;

} odu_tfrm_layer_struct_t;

/*******************************************************************************
* STRUCTURE: odu_tfrm_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to illustrate the dynamically changing context.
*
* ELEMENTS:
*   tcm_pool_ptr       - TCM POOL ID context table
*   pm_ctxt_ptr        - Structure for PM context 
*   num_pool_free      - Number of unassigned pool ids that are available in the
*                        TCM POOL ID context table
*   optional_oh_ctxt   - bit mask to represent 5 optional overhead fields to be
*                        disabled on maintenance signal insertion.
*                        Bit 0-1: FTFL_INS
*                        Bit   2: TCM_ACT_INS
*                        Bit   3: PM_TCM_RES_INS
*                        Bit   4: RES1_INS
*                        Bit   5: RES2_INS
*   dci_pm_tcm_child   - The child TCM/PM of DCI
*   odukp_maint_sig    - ODUkP/ODU[i][j] Mux adaptation 
*                        (after all layers, before muxing)
*                        0 -- no maintenance signal forced
*                        1 -- Force ODUK_AIS
*                        2 -- Force ODUK_LCK
*                        3 -- Force ODUK_OCI
*  odukp_pck_maint_sig - ODUkP/Pck packet adaptation
*                        (over PM layer only, no TCMS)
*                        0 -- no maintenance signal forced
*                        1 -- Force ODUK_AIS
*                        2 -- Force ODUK_LCK
*                        3 -- Force ODUK_OCI
*******************************************************************************/
typedef struct
{
    odu_tfrm_tcm_pool_t *tcm_pool_ptr;
    odu_tfrm_pm_ctxt_t  *pm_ctxt_ptr;
    UINT32 num_pool_free;
    UINT8 *optional_oh_ctxt; 
    odu_tfrm_pm_tcm_id_t *dci_pm_tcm_child;
    UINT8 *odukp_mux_maint_sig;
    UINT8 *odukp_pck_maint_sig;

} odu_tfrm_var_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
                                        
PUBLIC odu_tfrm_handle_t *odu_tfrm_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void odu_tfrm_ctxt_destroy(odu_tfrm_handle_t *odu_tfrm_handle);
PUBLIC void odu_tfrm_handle_init(odu_tfrm_handle_t *odu_tfrm_handle,
                                 odu_tfrm_inst_t tfrm_instance);
PUBLIC PMC_ERROR odu_tfrm_handle_restart_init(odu_tfrm_handle_t *odu_tfrm_handle,
                                              util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                              pmc_energy_state_t energy_state);
PUBLIC UINT32 odu_tfrm_base_address_get(odu_tfrm_handle_t *odu_tfrm_handle);

PUBLIC PMC_ERROR odu_tfrm_init(odu_tfrm_handle_t *odu_tfrm_handle);
PUBLIC PMC_ERROR odu_tfrm_chnl_init(odu_tfrm_handle_t *odu_tfrm_handle,
                                    UINT32 chnl_id, UINT32 enbl);
PUBLIC PMC_ERROR odu_tfrm_pool_id_assign(odu_tfrm_handle_t *odu_tfrm_handle,
                                         UINT32 chnl_id, odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                         UINT32 assign_mode, UINT32 pool_id,
                                         UINT16 *assigned_pool_id);
PUBLIC PMC_ERROR odu_tfrm_pool_id_return(odu_tfrm_handle_t *odu_tfrm_handle,
                                         UINT32 chnl_id, odu_tfrm_pm_tcm_id_t tcm_num,
                                         UINT16 *pool_id);
PUBLIC PMC_ERROR odu_tfrm_tcmi_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                   UINT32 chnl_id, 
                                   odu_tfrm_pm_tcm_id_t tcm_num,
                                   odu_tfrm_pm_tcm_id_t parent_tcm_num,
                                   odu_tfrm_pm_tcm_mode_t mode, BOOL8 enbl);
PUBLIC PMC_ERROR odu_tfrm_tti_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                  UINT32 chnl_id, odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                  odu_tfrm_tti_oh_ins_t oh_ins_field,
                                  UINT8 sapi[16], UINT8 dapi[16],
                                  UINT8 op_spec[32]);
PUBLIC PMC_ERROR odu_tfrm_tti_ext(odu_tfrm_handle_t *odu_tfrm_handle,
                                  UINT32 chnl_id, odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                  UINT8 *sapi_ptr, UINT8 *dapi_ptr, UINT8 *op_spec_ptr);                                   
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_bdi_bei_biae_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                                   UINT32 chnl_id, 
                                                   odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                                   UINT32 oh_field, 
                                                   UINT32 force_val);
PUBLIC PMC_ERROR odu_tfrm_ftfl_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT8 source,
                                       UINT8 force_field);
PUBLIC PMC_ERROR odu_tfrm_ftfl_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                   UINT32 chnl_id, UINT8 fwd_id,
                                   UINT8 fwd_op_id[9], UINT8 bwd_id,
                                   UINT8 bwd_op_id[9]);
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_stat_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                               UINT32 chnl_id, 
                                               odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                               UINT32 force_val);
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_bdi_bei_biae_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                                       UINT32 chnl_id, 
                                                       UINT32 source);
PUBLIC PMC_ERROR odu_tfrm_pm_tcm_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                         UINT32 chnl_id, 
                                         odu_tfrm_oh_ins_src_t source,
                                         UINT32 force_val);

PUBLIC PMC_ERROR odu_tfrm_tcm_act_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                          UINT32 chnl_id, 
                                          odu_tfrm_oh_ins_src_t tcm_act_src,
                                          UINT32 force_val);
                                      
PUBLIC PMC_ERROR odu_tfrm_res1_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, 
                                       odu_tfrm_oh_ins_src_t res1_source,
                                       UINT16 res1_val);
                                                                       
PUBLIC PMC_ERROR odu_tfrm_res2_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, 
                                       odu_tfrm_oh_ins_src_t res2_source,
                                       UINT8 res2_val[6]);
                                   
PUBLIC PMC_ERROR odu_tfrm_exp_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                      UINT32 chnl_id, 
                                      odu_tfrm_oh_ins_src_t exp_source,
                                      UINT32 exp_val);

PUBLIC PMC_ERROR odu_tfrm_gcc1_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, 
                                       odu_tfrm_oh_ins_src_t gcc1_source,
                                       UINT16 gcc1_val);
                                      
PUBLIC PMC_ERROR odu_tfrm_gcc2_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, 
                                       odu_tfrm_oh_ins_src_t gcc2_source,
                                       UINT16 gcc2_val);
                                   
PUBLIC PMC_ERROR odu_tfrm_aps_pcc_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                          UINT32 chnl_id, 
                                          UINT8 mon_level,
                                          odu_tfrm_oh_ins_src_t source,
                                          UINT32 aps_pcc_val);                                          

PUBLIC PMC_ERROR odu_tfrm_odukt_maint_sig_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                              UINT32 chnl_id, 
                                              odu_tfrm_pm_tcm_id_t tcm_num,
                                              UINT32 maint_sig);

PUBLIC PMC_ERROR odu_tfrm_odukp_mux_maint_sig_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                                  UINT32 chnl_id, 
                                                  UINT32 maint_sig);

PUBLIC PMC_ERROR odu_tfrm_odukp_pck_maint_sig_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                                  UINT32 chnl_id, 
                                                  UINT32 maint_sig);

PUBLIC void odu_tfrm_odukp_pck_maint_sig_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                             UINT32 chnl_id, 
                                             UINT32 *maint_sig);

PUBLIC PMC_ERROR odu_tfrm_sf_sd_proc_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                         UINT32 chnl_id, UINT32 sf_sd_proc);
PUBLIC PMC_ERROR odu_tfrm_ssf_proc_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT32 ssf_proc);
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_delay_measure_get_count(odu_tfrm_handle_t *odu_tfrm_handle,
                                                          UINT32 chnl_id,
                                                          odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                                          UINT32 block_mode,
                                                          UINT32 *dm_count);
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_delay_measure_init(odu_tfrm_handle_t *odu_tfrm_handle,
                                                     UINT32 chnl_id,
                                                     odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                                     UINT32 enbl,
                                                     UINT32 dm_val);

PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_delay_measure_mode_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                                         UINT32 chnl_id,
                                                         odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                                         UINT32 *enbl,
                                                         UINT32 *dm_val);
                                                                           
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_delay_measure_go(odu_tfrm_handle_t *odu_tfrm_handle,
                                                          UINT32 chnl_id,
                                                          odu_tfrm_pm_tcm_id_t pm_tcm_num);

PUBLIC PMC_ERROR odu_tfrm_iae_stat_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT32 *status);
                                             
PUBLIC PMC_ERROR odu_tfrm_pm_tcm_res_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                         UINT32 chnl_id, UINT8 *pm_tcm_res_val);

PUBLIC PMC_ERROR odu_tfrm_chnl_prov(odu_tfrm_handle_t *odu_tfrm_handle,
                                    UINT32 chnl_id,
                                    odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                    odu_tfrm_pm_tcm_id_t parent_tcm_num,
                                    odu_tfrm_pm_tcm_mode_t mode,
                                    BOOL8 enbl);  

PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_bdi_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                          UINT32 chnl_id, 
                                          odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                          UINT8 *bdi_val);    

PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_bei_biae_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                               UINT32 chnl_id, 
                                               odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                               UINT8 *bei_biae_val);     

PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_stat_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                           UINT32 chnl_id,
                                           odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                           UINT8 *stat_val);
                                           
PUBLIC PMC_ERROR odu_tfrm_tcm_act_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                      UINT32 chnl_id, UINT8 *tcm_act_val); 

PUBLIC PMC_ERROR odu_tfrm_res1_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                   UINT32 chnl_id, UINT16 *res1_val);
                                   
PUBLIC PMC_ERROR odu_tfrm_res2_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                   UINT32 chnl_id, UINT8 *res2_val);

PUBLIC PMC_ERROR odu_tfrm_res2_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT8 *res2_src);                                   
                                   
PUBLIC PMC_ERROR odu_tfrm_exp_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                      UINT32 chnl_id, UINT8 *exp_src);
                                      
PUBLIC PMC_ERROR odu_tfrm_exp_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                  UINT32 chnl_id, UINT16 *exp_val);

PUBLIC PMC_ERROR odu_tfrm_ftfl_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT8 *source);                                                                                                           
                                   
PUBLIC PMC_ERROR odu_tfrm_tcm_act_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                          UINT32 chnl_id, UINT8 *tcm_act_src);

PUBLIC PMC_ERROR odu_tfrm_pm_tcm_res_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                             UINT32 chnl_id, UINT8 *pm_tcm_res_src);

PUBLIC PMC_ERROR odu_tfrm_res1_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT8 *res1_src);
                                                                         
PUBLIC PMC_ERROR odu_tfrm_aps_pcc_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                          UINT32 chnl_id, 
                                          UINT32 mon_level,
                                          UINT8 *aps_pcc_src);

PUBLIC PMC_ERROR odu_tfrm_aps_pcc_ext(odu_tfrm_handle_t *odu_tfrm_handle,
                                      UINT32 chnl_id, 
                                      UINT32 mon_level,
                                      UINT32 *aps_pcc_val);                                                                              
                                          
PUBLIC PMC_ERROR odu_tfrm_gcc2_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT8 *gcc2_src);

PUBLIC PMC_ERROR odu_tfrm_gcc1_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT8 *gcc1_src);
                                       
PUBLIC PMC_ERROR odu_tfrm_gcc1_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                   UINT32 chnl_id, UINT16 *gcc1_val);

PUBLIC PMC_ERROR odu_tfrm_gcc2_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                   UINT32 chnl_id, UINT16 *gcc2_val);                                                                                                                   
PUBLIC PMC_ERROR odu_tfrm_chnl_oh_ins_disable(odu_tfrm_handle_t *odu_tfrm_handle,
                                             UINT32 chnl_id);   
                                             
PUBLIC PMC_ERROR odu_tfrm_chnl_oh_ins_ctxt_set(odu_tfrm_handle_t *odu_tfrm_handle,
                                               UINT32 chnl_id);

PUBLIC PMC_ERROR odu_tfrm_chnl_oh_ins_ctxt_restore(odu_tfrm_handle_t *odu_tfrm_handle,
                                                   UINT32 chnl_id);  

PUBLIC PMC_ERROR odu_tfrm_chnl_maint_sig_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                             UINT32 chnl_id, UINT8 *maint_sig);

PUBLIC PMC_ERROR odu_tfrm_odukt_maint_sig_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                              UINT32 chnl_id, 
                                              odu_tfrm_pm_tcm_id_t tcm_num,
                                              UINT8 *maint_sig);

PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_mode_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                           UINT32 chnl_id,
                                           odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                           odu_tfrm_pm_tcm_mode_t *mode);  

PUBLIC PMC_ERROR odu_tfrm_ftfl_ext(odu_tfrm_handle_t *odu_tfrm_handle,
                                   UINT32 chnl_id, UINT8 *fwd_id,
                                   UINT8 *fwd_op_id, UINT8 *bwd_id,
                                   UINT8 *bwd_op_id);

PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_bdi_bei_biae_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                                       UINT32 chnl_id, 
                                                       UINT32 *rcp_en);  
                                                                                        
PUBLIC BOOL8 odu_tfrm_pm_prov_state_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id);  

PUBLIC PMC_ERROR odu_tfrm_dci_child_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                         UINT32 chnl_id,
                                         UINT32 *child_pool_id,
                                         odu_tfrm_pm_tcm_id_t *child_tcm_pm_id);

PUBLIC PMC_ERROR odu_tfrm_layer_hierarchy_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                              UINT32 chnl_id,
                                              odu_tfrm_layer_struct_t tcm_layers[ODU_SOURCE_MAX_PM_TCM_LAYERS],
                                              UINT32 *num_layers);                                           
PUBLIC void odu_tfrm_passthru_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                  UINT32 passthru[3]);

PUBLIC void odu_tfrm_chnl_clean(odu_tfrm_handle_t *odu_tfrm_handle,                                UINT32 *chnl,
                                UINT32 num_chnl);

PUBLIC PMC_ERROR odu_tfrm_int_enable(odu_tfrm_handle_t     *odu_tfrm_handle,
                                     odu_tfrm_int_t        *int_table_ptr,
                                     odu_tfrm_int_t        *int_en_table_ptr,
                                     BOOL8                  enable);

PUBLIC PMC_ERROR odu_tfrm_int_clear(odu_tfrm_handle_t      *odu_tfrm_handle,
                                    odu_tfrm_int_t         *int_table_ptr,
                                    odu_tfrm_int_t         *int_en_table_ptr);

PUBLIC PMC_ERROR odu_tfrm_int_retrieve(odu_tfrm_handle_t      *odu_tfrm_handle,
                                       odu_tfrm_int_t         *filt_table_ptr,
                                       odu_tfrm_int_t         *int_table_ptr);

PUBLIC PMC_ERROR odu_tfrm_int_enabled_check(odu_tfrm_handle_t    *odu_tfrm_handle,
                                            odu_tfrm_int_t       *int_en_table_ptr,
                                            BOOL8                *int_found_ptr);

PUBLIC PMC_ERROR odu_tfrm_int_chnl_enable(odu_tfrm_handle_t     *odu_tfrm_handle,
                                          UINT32                 chnl_id,
                                          odu_tfrm_int_chnl_t   *int_table_ptr,
                                          odu_tfrm_int_chnl_t   *int_en_table_ptr,
                                          BOOL8                  enable);

PUBLIC PMC_ERROR odu_tfrm_int_chnl_clear(odu_tfrm_handle_t      *odu_tfrm_handle,
                                         UINT32                  chnl_id,
                                         odu_tfrm_int_chnl_t    *int_table_ptr,
                                         odu_tfrm_int_chnl_t    *int_en_table_ptr);

PUBLIC PMC_ERROR odu_tfrm_int_chnl_retrieve(odu_tfrm_handle_t      *odu_tfrm_handle,
                                            UINT32                  chnl_id,
                                            odu_tfrm_int_chnl_t    *filt_table_ptr,
                                            odu_tfrm_int_chnl_t    *int_table_ptr);

PUBLIC PMC_ERROR odu_tfrm_int_chnl_enabled_check(odu_tfrm_handle_t    *odu_tfrm_handle,
                                                 UINT32                chnl_id,
                                                 odu_tfrm_int_chnl_t  *int_en_table_ptr,
                                                 BOOL8                *int_found_ptr);

PUBLIC PMC_ERROR odu_trfm_all_tcm_iae_force(odu_tfrm_handle_t    *odu_tfrm_handle,
                                            UINT32                chnl_id);

#ifdef __cplusplus
}
#endif

#endif /* _ODU_TFRM_H */

/*
** end of file
*/
