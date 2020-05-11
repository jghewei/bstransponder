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

#ifndef _ODU_RFRM_H
#define _ODU_RFRM_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_bitarray.h"
#include "pmc_handle.h"
#include "pmc_log.h"
#include "util_global.h"
#include "odu_rfrm_api.h"
#include "util_global_api.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
#define TCM_POOL_SIZE_7     (8)  /* STGN can support 12 CHNL, but only 1 channel will be used.
                                    A maximum of 8 pool ids will be used (6 TMCs + 1 floating TCM + PM-NIM)*/
#define TCM_POOL_SIZE_12    (72)
#define TCM_POOL_SIZE_96    (256)

/*
** Macro Definitions
*/
/* Unchannelized Interrupts */
/* ODU_RFRM_INT(handle, tsb_name, tsb_field, tsb_comb_field, struct_field) */
#define ODU_RFRM_INT_TABLE_DEFINE()                                        \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_DLCK,    comb_dlck_i);    \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_DOCI,    comb_doci_i);    \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_DAIS,    comb_dais_i);    \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_DTIM,    comb_dtim_i);    \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_DDEG,    comb_ddeg_i);    \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_DBDI,    comb_dbdi_i);    \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_DLTC,    comb_dltc_i);    \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_DIAE,    comb_diae_i);    \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_DBIAE,   comb_dbiae_i);   \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_DLOFLOM, comb_dloflom_i); \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_OOF,     comb_oof_i);     \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_ATSF,    comb_atsf_i);    \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_ATSF,    comb_atsd_i);    \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_AAIS,    comb_aais_i);    \
    ODU_RFRM_INT_SINGLE(odu_rfrm_handle, odu_rfrm, COMB_ABDI,    comb_abdi_i);    \

/* APS interrupts */
/* ODU_RFRM_INT(handle, tsb_name, tsb_field, tsb_comb_field, struct_field) */
#define ODU_RFRM_INT_CHNL_TABLE_DEFINE()                                             \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, lfield_range, AC_APS, ac_aps_i);         \

#define ODU_RFRM_INT_CHNL_TABLE_CLEAR_DEFINE()                            \
    ODU_RFRM_INT_CHNL_RANGE(odu_rfrm_handle, odu_rfrm, AC_APS, ac_aps_i); \


/* PM Interrupts */
/* ODU_RFRM_INT(handle, tsb_name, tsb_field, tsb_comb_field, struct_field) */
#define ODU_RFRM_INT_PM_TABLE_DEFINE()                                       \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DLCK_PM, COMB_DLCK,    dlck_pm_i); \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DOCI_PM, COMB_DOCI,    doci_pm_i); \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DAIS_PM, COMB_DAIS,    dais_pm_i); \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DTIM_PM, COMB_DTIM,    dtim_pm_i); \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DDEG_PM, COMB_DDEG,    ddeg_pm_i); \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DBDI_PM, COMB_DBDI,    dbdi_pm_i); \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, OOF,     COMB_OOF,     oof_i);     \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DLOFLOM, COMB_DLOFLOM, dloflom_i); \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, ATSF_PM, COMB_ATSF,    atsf_pm_i); \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, ATSD_PM, COMB_ATSD,    atsd_pm_i); \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, ABDI_PM, COMB_ABDI,    abdi_pm_i); \
    ODU_RFRM_INT_NOOP(odu_rfrm_handle, odu_rfrm, NULL, ddeg_enh_pm_i);         \
    ODU_RFRM_INT_NOOP(odu_rfrm_handle, odu_rfrm, NULL, ddeg_bursty_pm_i);           

#define ODU_RFRM_STATUS_PM_TABLE_DEFINE()                                    \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DLCK_PM,   COMB_DLCK,    dlck_pm_v); \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DOCI_PM,   COMB_DOCI,    doci_pm_v); \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DAIS_PM,   COMB_DAIS,    dais_pm_v); \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DTIM_PM,   COMB_DTIM,    dtim_pm_v); \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DDEG_PM,   COMB_DDEG,    ddeg_pm_v); \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DBDI_PM,   COMB_DBDI,    dbdi_pm_v); \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, OOF,       COMB_OOF,     oof_v);     \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DLOFLOM,   COMB_DLOFLOM, dloflom_v); \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, ATSF_PM,   COMB_ATSF,    atsf_pm_v); \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, ATSD_PM,   COMB_ATSD,    atsd_pm_v); \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, ABDI_PM,   COMB_ABDI,    abdi_pm_v); \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, CI_SSF_PM, NULL,         ci_ssf_pm_v); \
    ODU_RFRM_STATUS_NOOP(odu_rfrm_handle, odu_rfrm, NULL, ddeg_enh_pm_v);           \
    ODU_RFRM_STATUS_NOOP(odu_rfrm_handle, odu_rfrm, NULL, ddeg_bursty_pm_v);           




/* PID Interrupts */
/* ODU_RFRM_INT(handle, tsb_name, tsb_field, tsb_comb_field, struct_field) */
#define ODU_RFRM_INT_PID_TABLE_DEFINE()                                      \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DLCK,  COMB_DLCK,  dlck_i);      \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DOCI,  COMB_DOCI,  doci_i);      \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DAIS,  COMB_DAIS,  dais_i);      \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DTIM,  COMB_DTIM,  dtim_i);      \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DDEG,  COMB_DDEG,  ddeg_i);      \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DBDI,  COMB_DBDI,  dbdi_i);      \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DBIAE, COMB_DBIAE, dbiae_i);     \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DLTC,  COMB_DLTC,  dltc_i);      \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, DIAE,  COMB_DIAE,  diae_i);      \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, ATSF,  COMB_ATSF,  atsf_i);      \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, ATSD,  COMB_ATSD,  atsd_i);      \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, AAIS,  COMB_AAIS,  aais_i);      \
    ODU_RFRM_INT(odu_rfrm_handle, odu_rfrm, ABDI,  COMB_ABDI,  abdi_i);      \
    ODU_RFRM_INT_NOOP(odu_rfrm_handle, odu_rfrm, NULL, ddeg_enh_i);          \
    ODU_RFRM_INT_NOOP(odu_rfrm_handle, odu_rfrm, NULL, ddeg_bursty_i);           

#define ODU_RFRM_STATUS_PID_TABLE_DEFINE()                                   \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DLCK,      COMB_DLCK,  dlck_v);  \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DOCI,      COMB_DOCI,  doci_v);  \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DAIS,      COMB_DAIS,  dais_v);  \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DTIM,      COMB_DTIM,  dtim_v);  \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DDEG,      COMB_DDEG,  ddeg_v);  \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DBDI,      COMB_DBDI,  dbdi_v);  \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DBIAE,     COMB_DBIAE, dbiae_v); \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DLTC,      COMB_DLTC,  dltc_v);  \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, DIAE,      COMB_DIAE,  diae_v);  \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, ATSF,      COMB_ATSF,  atsf_v);  \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, ATSD,      COMB_ATSD,  atsd_v);  \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, AAIS,      COMB_AAIS,  aais_v);  \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, ABDI,      COMB_ABDI,  abdi_v);  \
    ODU_RFRM_STATUS(odu_rfrm_handle, odu_rfrm, CI_SSF,    NULL,       ci_ssf_v);\
    ODU_RFRM_STATUS_NOOP(odu_rfrm_handle, odu_rfrm, NULL, ddeg_enh_v);          \
    ODU_RFRM_STATUS_NOOP(odu_rfrm_handle, odu_rfrm, NULL, ddeg_bursty_v);           




/* COMB_INT management: superset of the above two tables (PM and PID) 
 *  This table correspondes to odu_rfrm_int_comb_t */
/* ODU_RFRM_INT(handle, tsb_comb_field) */
#define ODU_RFRM_COMB_INT_TABLE_DEFINE()                \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_DLCK);           \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_DOCI);           \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_DAIS);           \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_DTIM);           \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_DDEG);           \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_DBDI);           \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_OOF);            \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_DLOFLOM);        \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_DBIAE);          \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_DLTC);           \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_DIAE);           \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_ATSF);           \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_ATSD);           \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_AAIS);           \
    ODU_RFRM_INT(odu_rfrm_handle, COMB_ABDI);           \

#define ODU_RFRM_TCM_PID_IS_VALID(pid)  (ODU_RFRM_UNASSIGNED_POOL_ID != pid)

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: odu_rfrm_tcm_pool_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to illustrate the TCM POOL ID context table.
*
* ELEMENTS:
*   assign_state     - state of the TCM POOL ID
*                      0 - free
*                      1 - assigned
*   chnl_id          - channel ID that the TCM belongs to
*   tcm_num          - TCM field instance. See odu_rfrm_pm_tcm_id_t.
*   tcm_mode         - Mode of the TCM layer. See odu_rfrm_tcm_state_t.
*   child_pool_id    - Child pool id of this TCM layer
*   child_pm_tcm_num - Child TMC/PM type connect to this layer
*                      only required to keep track of PM layer
*   maint_sig        - To keep track of maintance signals forced
*                      at a TCM layer
*   
*
*******************************************************************************/
typedef struct
{
    BOOL8 assign_state;
    UINT32 chnl_id;
    odu_rfrm_pm_tcm_id_t tcm_num;
    odu_rfrm_pm_tcm_mode_t tcm_mode;
    UINT32 child_pool_id;
    odu_rfrm_pm_tcm_id_t child_pm_tcm_num;
    UINT8 maint_sig;
} odu_rfrm_tcm_pool_t;

/*******************************************************************************
* STRUCTURE: odu_rfrm_layer_struct_t
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
    odu_rfrm_pm_tcm_id_t pm_tcm_num;
    odu_rfrm_pm_tcm_mode_t tcm_mode;
    UINT8 maint_sig;

} odu_rfrm_layer_struct_t;



/*******************************************************************************
* ENUM: odu_rfrm_int_summary_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Type of interrupt summary
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef enum 
{
    ODU_RFRM_INT_SUMMARY_TYPE_ACTIVE_PM = 0,
    ODU_RFRM_INT_SUMMARY_TYPE_ACTIVE_PID,
    ODU_RFRM_INT_SUMMARY_TYPE_ACTIVE_INT,

    LAST_ODU_RFRM_INT_SUMMARY_TYPE  /* this is for out of range values error handling */
} odu_rfrm_int_summary_type_t;



/*******************************************************************************
* STRUCTURE: odu_rfrm_int_summary_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Interrupt summary information for OTN ODU channels
*
* ELEMENTS:
*   pool_id                 - TCM PID(s) to access
*
*   active_chnl_bitmask     - Bitmask of channels in ODU framer that have an
*                             active interrupt
*
*   active_pid_bitmask      - Bitmask of PID TCMs in ODU framer that have an active
*                             interrupt
*
*   active_int_bitmask      - Bitmask of active interrupts in ODU framer.
*                             Interrupt accessor macros: 
*                             DIGI_OTN_ODU_INT_SUMMARY_*_GET(bitmask)
*
*******************************************************************************/
typedef struct {
    UINT32                  active_chnl_bitmask[PMC_BITARRAY_SIZE(96)];
    UINT32                  active_pid_bitmask[PMC_BITARRAY_SIZE(256)];                   
    UINT32                  active_int_bitmask[1];
    UINT32                  pool_id[ODU_RFRM_PIDS_PER_CHNL];
} odu_rfrm_int_summary_t;



/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC odu_rfrm_handle_t *odu_rfrm_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void odu_rfrm_ctxt_destroy(odu_rfrm_handle_t *odu_rfrm_handle);
PUBLIC void odu_rfrm_handle_init(odu_rfrm_handle_t *odu_rfrm_handle,
                                 odu_rfrm_inst_t odu_rfrm_instance);
PUBLIC PMC_ERROR odu_rfrm_handle_restart_init(odu_rfrm_handle_t *odu_rfrm_handle,
                                              util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                              pmc_energy_state_t energy_state);
PUBLIC UINT32 odu_rfrm_base_address_get(odu_rfrm_handle_t *odu_rfrm_handle);

PUBLIC PMC_ERROR odu_rfrm_init(odu_rfrm_handle_t *odu_rfrm_handle,
                                                             BOOL8 stg2_sysotn_mode);
PUBLIC PMC_ERROR odu_rfrm_chnl_init(odu_rfrm_handle_t *odu_rfrm_handle,
                                    UINT32 chnl_id, UINT32 enbl, UINT32 fau_enbl);

PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_defect_ctrl_mode_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                                       UINT32 chnl_id,
                                                       odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                       odu_rfrm_pm_tcm_mode_t mode,
                                                       UINT32 tcm_pool_id);

PUBLIC PMC_ERROR odu_rfrm_layer_hierarchy_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                              UINT32 chnl_id,
                                              odu_rfrm_layer_struct_t (*tcm_layers)[ODU_SINK_MAX_PM_TCM_LAYERS],
                                              UINT32 *num_layers);

PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                      UINT32 chnl_id,
                                      odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                      UINT32 float_tcmi_src,
                                      odu_rfrm_pm_tcm_id_t parent_id,
                                      odu_rfrm_pm_tcm_mode_t mode,
                                      BOOL8 prov_action);

PUBLIC PMC_ERROR odu_rfrm_tcmi_mode_update(odu_rfrm_handle_t *odu_rfrm_handle,
                                           UINT32 chnl_id,
                                           odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                           odu_rfrm_pm_tcm_mode_t mode);

PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_parent_update(odu_rfrm_handle_t *odu_rfrm_handle,
                                                UINT32 chnl_id,
                                                odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                odu_rfrm_pm_tcm_id_t new_parent_tcm_num,
                                                odu_rfrm_layer_struct_t odu_rfrm_layers[ODU_SINK_MAX_PM_TCM_LAYERS],
                                                UINT32 num_layers);

PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_tim_det_mode_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                                   UINT32 chnl_id,
                                                   odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                   UINT32 tim_det_mode);
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_tim_det_mode_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                                   UINT32 chnl_id,
                                                   odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                   UINT8 *tim_det_mode);                                                   
PUBLIC PMC_ERROR odu_rfrm_tcm_maint_sig_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                            UINT32 chnl_id, odu_rfrm_pm_tcm_id_t tcm_num,
                                            UINT32 force_alarm_sig);
PUBLIC PMC_ERROR odu_rfrm_acpt_tti_lock(odu_rfrm_handle_t *odu_rfrm_handle, BOOL8 set);
PUBLIC PMC_ERROR odu_rfrm_acpt_tti_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                       UINT32 chnl_id, odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                       UINT8 *sapi_ptr, UINT8 *dapi_ptr,
                                       UINT8 *opspec_ptr);
PUBLIC PMC_ERROR odu_rfrm_ftfl_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                   UINT32 chnl_id, UINT8 *fwd_id,
                                   UINT8 *fwd_op_id, UINT8 *bwd_id,
                                   UINT8 *bwd_op_id);
PUBLIC PMC_ERROR odu_rfrm_acpt_aps_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                       UINT32 chnl_id, UINT32 layer_num,
                                       UINT32 *aps_val);
PUBLIC PMC_ERROR odu_rfrm_exp_tti_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                          UINT32 chnl_id, odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                          UINT8 sapi[16], UINT8 dapi[16]);
PUBLIC PMC_ERROR odu_rfrm_exp_tti_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                          UINT32 chnl_id, odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                          UINT8 *sapi_ptr, UINT8 *dapi_ptr);
PUBLIC PMC_ERROR odu_rfrm_pm_ebc_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                     UINT32 chnl_id,
                                     odu_rfrm_odukp_counters_t *ebc);
PUBLIC PMC_ERROR odu_rfrm_acpt_gcc_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                       UINT32 chnl_id, UINT8 oh_field, 
                                       UINT16 *gcc_val);
PUBLIC PMC_ERROR odu_rfrm_acpt_stat_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                        UINT32 chnl_id, 
                                        odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                        UINT8 *stat);
PUBLIC PMC_ERROR odu_rfrm_acpt_pm_tcm_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                          UINT32 chnl_id, UINT8 *pm_tcm);
PUBLIC PMC_ERROR odu_rfrm_acpt_tcm_act_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                           UINT32 chnl_id, UINT8 *tcm_act);
PUBLIC PMC_ERROR odu_rfrm_acpt_res1_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                        UINT32 chnl_id, UINT16 *res1);
PUBLIC PMC_ERROR odu_rfrm_acpt_res2_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                        UINT32 chnl_id, UINT8 *res2);
PUBLIC PMC_ERROR odu_rfrm_acpt_exp_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                       UINT32 chnl_id, UINT16 *exp);
PUBLIC PMC_ERROR odu_rfrm_chnl_maint_sig_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                          UINT32 chnl_id, UINT32 force_alarm_sig);
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_ais_disable(odu_rfrm_handle_t *odu_rfrm_handle,
                                              UINT32 chnl_id,
                                              odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                              UINT8 ais_dis,
                                              UINT32 tcm_pool_id);
PUBLIC PMC_ERROR odu_rfrm_tcmi_ltc_to_ais_tsf_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                                  UINT32 chnl_id,
                                                  odu_rfrm_pm_tcm_id_t tcm_num,
                                                  UINT32 dLTC_dis);
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_tim_dis_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                              UINT32 chnl_id,
                                              odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                              BOOL8 dTIM_dis);
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_tim_dis_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                              UINT32 chnl_id,
                                              odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                              BOOL8 *dTIM_dis);                                              
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_bw_act_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                             UINT32 chnl_id,
                                             odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                             UINT8 bw_dis,
                                             UINT32 tcm_pool_id);
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_bw_act_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                             UINT32 chnl_id,
                                             odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                             UINT8 *bw_dis);     
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                                   UINT32 chnl_id,
                                                   odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                   UINT32 tcm_pool_id,
                                                   UINT8 defect_ctrl,
                                                   UINT8 tsd_dis,
                                                   UINT8 tsf_dis);
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_tsd_tsf_dis_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                  UINT32 chnl_id,
                                                  odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                  UINT8 *tsd_dis,
                                                  UINT8 *tsf_dis);                                                   
PUBLIC PMC_ERROR odu_rfrm_chnl_ins_ais_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                           UINT32 chnl_id, BOOL8 dloflom_dis);

PUBLIC PMC_ERROR odu_rfrm_action_proc_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                          UINT32 dLTC_dis, UINT32 dAIS_dis,
                                          UINT32 dOCI_dis, UINT32 dLCK_dis,
                                          UINT8 wr_mask);

PUBLIC PMC_ERROR odu_rfrm_tcmi_ltc_to_ais_tsf_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                  UINT32 chnl_id,
                                                  odu_rfrm_pm_tcm_id_t tcm_num,
                                                  BOOL8 *dLTC_dis);
PUBLIC BOOL8 odu_rfrm_is_chnl_schd(odu_rfrm_handle_t *odu_rfrm_handle,
                                  UINT32 chnl_id);

PUBLIC PMC_ERROR odu_rfrm_chnl_prov(odu_rfrm_handle_t *odu_rfrm_handle,
                                    UINT32 chnl_id,
                                    BOOL8 provision_pm,
                                    UINT32 enbl, 
                                    UINT32 fau_enbl,
                                    BOOL deprov_configs);

PUBLIC PMC_ERROR odu_rfrm_pm_int_enbl(odu_rfrm_handle_t *odu_rfrm_handle,
                                      UINT32 chnl_id, UINT32 int_enbl);

PUBLIC PMC_ERROR odu_rfrm_tcm_maint_sig_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                            UINT32 chnl_id, 
                                            odu_rfrm_pm_tcm_id_t tcm_num,
                                            UINT8 *maint_sig);
PUBLIC void odu_rfrm_chnl_maint_sig_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                             UINT32 chnl_id, 
                                             UINT8 *maint_sig);     
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_ais_dis_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                              UINT32 chnl_id,
                                              odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                              UINT8 *ais_dis);   
PUBLIC PMC_ERROR odu_rfrm_tcm_ebc_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                      UINT32 chnl_id,
                                      odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                      odu_rfrm_odukt_counters_t *ebc);
PUBLIC PMC_ERROR odu_rfrm_chnl_dloflom_act_dis_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                   UINT32 chnl_id, BOOL8 *dloflom_dis);                                                                                                                               
                                      
/* pool id operators */

PUBLIC PMC_ERROR odu_rfrm_pool_id_assign(odu_rfrm_handle_t *odu_rfrm_handle,
                                         UINT32 chnl_id, 
                                         odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                         UINT8 assign_mode, UINT8 pool_id,
                                         UINT16 *assigned_pool_id);
PUBLIC UINT32 odu_rfrm_pool_id_return(odu_rfrm_handle_t *odu_rfrm_handle,
                                      UINT32 chnl_id, odu_rfrm_pm_tcm_id_t tcm_num, UINT32 *pool_id);
PUBLIC PMC_ERROR odu_rfrm_tcm_pool_id_ctxt_audit(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id, odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                 UINT32 pool_id);
PUBLIC BOOL8 odu_rfrm_tcm_pool_id_check(odu_rfrm_handle_t *odu_rfrm_handle,
                                        UINT32 pool_id,
                                        UINT32 chnl_id,
                                        odu_rfrm_pm_tcm_id_t pm_tcm_num);


PUBLIC PMC_ERROR odu_rfrm_tcm_pool_id_search(odu_rfrm_handle_t *odu_rfrm_handle,
                                             UINT32 chnl_id, 
                                             odu_rfrm_pm_tcm_id_t tcm_num,
                                             UINT32 *pool_id);  

PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_mode_set(odu_rfrm_handle_t *odu_rfrm_handle,
                                           UINT32 chnl_id,
                                           odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                           UINT32 float_tcmi_src,  
                                           odu_rfrm_pm_tcm_mode_t mode,
                                           UINT32 tcm_pool_id);

PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_mode_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                           UINT32 chnl_id,
                                           odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                           odu_rfrm_pm_tcm_mode_t *tcm_mode);                                                      

PUBLIC PMC_ERROR odu_rfrm_comb_defect_intr_stat_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                    UINT32 *intr_stat);
PUBLIC PMC_ERROR odu_rfrm_comb_defect_intr_enable_set(odu_rfrm_handle_t *odu_rfrm_handle,
                                                      UINT32 comb_intr_enbl);
PUBLIC PMC_ERROR odu_rfrm_chnl_pm_defect_intr_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                  UINT32 chnl_id, UINT32 *pm_defect_stat);
PUBLIC PMC_ERROR odu_rfrm_tcmi_defect_stat_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                               UINT32 chnl_id, odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                               UINT32 *defect_stat);
PUBLIC PMC_ERROR odu_rfrm_pm_defect_stat_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                             UINT32 chnl_id, odu_rfrm_pm_tcm_id_t pm_num,
                                             UINT32 *defect_stat);
PUBLIC PMC_ERROR odu_rfrm_pm_near_end_pmon_ds_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                  UINT32 chnl_id,
                                                  UINT32 *near_end_ds);
PUBLIC PMC_ERROR odu_rfrm_pm_far_end_pmon_ds_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id,
                                                 UINT32 *far_end_ds);

PUBLIC PMC_ERROR odu_rfrm_pm_near_end_pmon_ebc_cnt_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                       UINT32 chnl_id,
                                                       UINT32 *near_end_ebc_cnt);
PUBLIC PMC_ERROR odu_rfrm_pm_far_end_pmon_ebc_cnt_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                      UINT32 chnl_id,
                                                      UINT32 *far_end_ebc_cnt);
PUBLIC PMC_ERROR odu_rfrm_pm_near_end_defect_ebc_cnt_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                         UINT32 chnl_id,
                                                         UINT32 *near_end_ebc_cnt);
PUBLIC PMC_ERROR odu_rfrm_pm_far_end_defect_ebc_cnt_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                        UINT32 chnl_id,
                                                        UINT32 *far_end_ebc_cnt);
PUBLIC PMC_ERROR odu_rfrm_pm_defect_intr_stat_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                  UINT32 chnl_id, odu_rfrm_pm_tcm_id_t pm_num,
                                                  UINT32 *defect_intr_stat);
PUBLIC PMC_ERROR odu_rfrm_pm_tcm_defect_intr_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id,
                                                 odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                          UINT32 *intr_status);
PUBLIC PMC_ERROR odu_rfrm_tcm_defect_intr_stat_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                   UINT32 chnl_id, odu_rfrm_pm_tcm_id_t tcm_num,
                                                   UINT32 *defect_intr_stat);


PUBLIC PMC_ERROR odu_rfrm_action_proc_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                          UINT32 *dLTC_dis, UINT32 *dAIS_dis,
                                          UINT32 *dOCI_dis, UINT32 *dLCK_dis);


PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_deg_m_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id,
                                                 odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                 UINT8 *deg_m);

PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_deg_m_set(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id,
                                                 odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                 UINT8 deg_m);


PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_deg_th_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id,
                                                 odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                 UINT32 *deg_thr);

PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_deg_th_set(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id,
                                                 odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                 UINT32 deg_thr);

PUBLIC PMC_ERROR odu_rfrm_validate_pm_tcm_params(odu_rfrm_handle_t *odu_rfrm_handle,
                                                  UINT32 chnl_id,
                                                  odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                  odu_rfrm_pm_tcm_id_t parent_id,
                                                  odu_rfrm_pm_tcm_mode_t mode,
                                                  BOOL8 prov_action,
                                                  UINT8 tcm_pool_id,
                                                  UINT8 assign_mode);
                                                  
PUBLIC PMC_ERROR odu_rfrm_last_layer_last_is_dci_update(odu_rfrm_handle_t *odu_rfrm_handle,
                                                        UINT32 chnl_id,
                                                        odu_rfrm_layer_struct_t tcm_layers[ODU_SINK_MAX_PM_TCM_LAYERS],
                                                        UINT32 num_layers);                                                  

PUBLIC PMC_ERROR odu_rfrm_parent_pid_parent_is_dci_update(odu_rfrm_handle_t *odu_rfrm_handle,
                                                          UINT32 chnl_id,
                                                          odu_rfrm_layer_struct_t tcm_layers[ODU_SINK_MAX_PM_TCM_LAYERS],
                                                          UINT32 num_layers);
                                                          
PUBLIC PMC_ERROR odu_rfrm_int_enable(odu_rfrm_handle_t          *odu_rfrm_handle,
                                             odu_rfrm_int_t     *int_table_ptr,
                                             odu_rfrm_int_t     *int_en_table_ptr,
                                             BOOL8               enable);
                                          
PUBLIC PMC_ERROR odu_rfrm_int_clear(odu_rfrm_handle_t      *odu_rfrm_handle,
                                    odu_rfrm_int_t         *int_table_ptr,
                                    odu_rfrm_int_t         *int_en_table_ptr);

PUBLIC PMC_ERROR odu_rfrm_int_retrieve(odu_rfrm_handle_t     *odu_rfrm_handle,
                                       odu_rfrm_int_t        *filt_table_ptr,
                                       odu_rfrm_int_t        *int_table_ptr);

PUBLIC PMC_ERROR odu_rfrm_int_enabled_check(odu_rfrm_handle_t   *odu_rfrm_handle,
                                            odu_rfrm_int_t      *int_en_table_ptr,
                                            BOOL8               *int_found_ptr);

PUBLIC PMC_ERROR odu_rfrm_int_chnl_enable(odu_rfrm_handle_t          *odu_rfrm_handle,
                                             UINT32                   chnl_id,
                                             odu_rfrm_int_chnl_t     *int_table_ptr,
                                             odu_rfrm_int_chnl_t     *int_en_table_ptr,
                                             BOOL8                     enable);
                                          
PUBLIC PMC_ERROR odu_rfrm_int_pm_chnl_enable(odu_rfrm_handle_t       *odu_rfrm_handle,
                                              UINT32                  chnl_id,
                                              BOOL8                   enable);

PUBLIC PMC_ERROR odu_rfrm_int_pid_chnl_enable(odu_rfrm_handle_t       *odu_rfrm_handle,
                                              UINT32                   pool_id,
                                              BOOL8                    enable);

PUBLIC PMC_ERROR odu_rfrm_int_pid_chnl_tbl_enable(odu_rfrm_handle_t       *odu_rfrm_handle,
                                                  UINT32                   chnl_id,
                                                  odu_rfrm_int_pid_chnl_t *int_table_ptr,
                                                  odu_rfrm_int_pid_chnl_t *int_en_table_ptr,
                                                  BOOL8                     enable);

PUBLIC PMC_ERROR odu_rfrm_int_defect_enable(odu_rfrm_handle_t       *odu_rfrm_handle,
                                            odu_rfrm_int_defect_t    defect,
                                            BOOL8                    enable);

PUBLIC PMC_ERROR odu_rfrm_int_summary_get(odu_rfrm_handle_t             *odu_rfrm_handle,
                                          odu_rfrm_int_summary_type_t    summary_type,
                                          UINT32                        *bitmask_ptr);

PUBLIC PMC_ERROR odu_rfrm_int_chnl_retrieve(odu_rfrm_handle_t        *odu_rfrm_handle,
                                               UINT32                 chnl_id,
                                               odu_rfrm_int_chnl_t   *filt_table_ptr,
                                               odu_rfrm_int_chnl_t   *int_table_ptr);

PUBLIC PMC_ERROR odu_rfrm_int_chnl_clear(odu_rfrm_handle_t        *odu_rfrm_handle,
                                         UINT32                 chnl_id,
                                         odu_rfrm_int_chnl_t   *int_table_ptr);

PUBLIC PMC_ERROR odu_rfrm_int_pid_chnl_retrieve(odu_rfrm_handle_t         *odu_rfrm_handle,
                                                UINT32                     chnl_id,
                                                UINT32                    *pool_id_ptr,
                                                odu_rfrm_int_pid_chnl_t   *filt_table_ptr,
                                                UINT32                    *int_table_pid_ptr,
                                                odu_rfrm_int_pid_chnl_t   *int_table_ptr);

PUBLIC PMC_ERROR odu_rfrm_int_chnl_enabled_check(odu_rfrm_handle_t        *odu_rfrm_handle,
                                                 UINT32                    chnl_id,
                                                 odu_rfrm_int_summary_t   *int_summary_ptr,
                                                 odu_rfrm_int_chnl_t      *int_en_table_ptr,
                                                 BOOL8                    *int_found_ptr);

PUBLIC PMC_ERROR odu_rfrm_int_start(odu_rfrm_handle_t    *odu_rfrm_handle,
                                    BOOL                  start,
                                    BOOL8                 clear);

PUBLIC PMC_ERROR odu_rfrm_int_transfer_done_clr(odu_rfrm_handle_t    *odu_rfrm_handle);

PUBLIC PMC_ERROR odu_rfrm_int_transfer_done_poll(odu_rfrm_handle_t    *odu_rfrm_handle);
PUBLIC void odu_rfrm_passthru_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                  UINT32 passthru[3],
                                  UINT32 fau_passthru[3]);
PUBLIC void odu_rfrm_chnl_clean(odu_rfrm_handle_t *odu_rfrm_handle,
                                UINT32 *chnl,
                                UINT32 num_chnl);

PUBLIC PMC_ERROR odu_rfrm_tcmi_resource_transition(odu_rfrm_handle_t *odu_rfrm_handle,
                                                   UINT32 chnl_id,
                                                   UINT32 tcm_pool_id);

PUBLIC void odu_rfrm_pm_status_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                   util_global_otn_pm_status_t *pm_table_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _ODU_RFRM_H */

/*
** end of file
*/
