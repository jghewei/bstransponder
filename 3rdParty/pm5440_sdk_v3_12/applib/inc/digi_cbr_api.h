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
*     This file contains the function prototypes for DIGI CBR run time
*     functionality
*
*   NOTES:
*
*******************************************************************************/

#ifndef _DIGI_CBR_API_H
#define _DIGI_CBR_API_H

#ifdef __cplusplus
extern "C" {
#endif


/*
** Include Files
*/
#include "cbrc_api.h"
#include "sdh_pmg_api.h"
#include "cbr_fsgm_api.h"
#include "util_global_api.h"


/*
** Enumerated Types
*/


/*
** Global variables
*/


/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: digi_cbr_int_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Channel table for CBR interrupts.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    cbrc_int_chnl_t         cbrc;
    cpb_int_chnl_t          cpb;
} digi_cbr_int_chnl_t;


/*******************************************************************************
* STRUCTURE: digi_cbr_int_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Aggregation table for CBR interrupts.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    digi_cbr_client_chnl_t  *handle[DIGI_CBRC_CHNL_MAX];
    digi_cbr_int_chnl_t      cbrc_chnl[DIGI_CBRC_CHNL_MAX];
} digi_cbr_int_t;


/*******************************************************************************
* ENUM: digi_cbr_int_summary_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to retrieve in digi_cbr_int_summary_t.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    DIGI_CBR_INT_SUMMARY_MASK_ACTIVE_CHNL_NUM    = 0x01,
    DIGI_CBR_INT_SUMMARY_MASK_ACTIVE_CHNL        = 0x02,
} digi_cbr_int_summary_mask_t;


/*******************************************************************************
* STRUCTURE: digi_cbr_int_summary_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Interrupt summary information for CBR channels
*
* ELEMENTS:
*   mask                    - digi_cbr_int_summary_mask_t bitmask to
*                             indicate which fields have been returned
*
*   active_chnl_num         - Number of channels in CBRC that have an active
*                             enabled interrupt
*
*   active_chnl             - Array of CBR client channel handles that have an
*                             have an active enabled interrupt. This allows the user
*                             to iteratively call digi_cbr_int_chnl_filt_retrieve()
*                             with the provided channel handles.
*
*******************************************************************************/
typedef struct {
    UINT32                   mask;

    UINT32                   active_chnl_num;
    digi_cbr_client_chnl_t  *active_chnl[DIGI_CBRC_CHNL_MAX];
} digi_cbr_int_summary_t;






/*******************************************************************************
* STRUCTURE: digi_cbr_pmon_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Channel table for CBR PMON
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    digi_pmon_cbrc_sdh_pmg_t    sdh_pmg;
    digi_pmon_cbrc_fc1200_t     fc1200;
    digi_pmon_cbrc_fc_cbr_t     fc_cbr;
} digi_cbr_pmon_chnl_t;


/*******************************************************************************
* STRUCTURE: digi_cbr_pmon_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Aggregation table for CBR PMON
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    digi_cbr_client_chnl_t        *handle[DIGI_CBRC_CHNL_MAX];
    digi_cbr_pmon_chnl_t           cbr_chnl[DIGI_CBRC_CHNL_MAX];
} digi_cbr_pmon_t;

/*
** Function Prototypes
*/

/* Alarm Insertion */
PUBLIC PMC_ERROR digi_cbr_sdh_pn11_ins_set(digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           cbrc_port_dir_t           dir,
                                           sdh_pmg_pn11_insert_t     lof_los,
                                           BOOL                      ins_enable);

PUBLIC PMC_ERROR digi_cbr_sdh_pn11_ins_set_get(digi_handle_t            *digi_handle,
                                               digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                               cbrc_port_dir_t           dir,
                                               sdh_pmg_pn11_insert_t     lof_los,
                                               BOOL                     *ins_enable_ptr);

PUBLIC PMC_ERROR digi_cbr_sdh_oof_force(digi_handle_t            *digi_handle,
                                        digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                        cbrc_port_dir_t           dir);



/* RRMP */
PUBLIC PMC_ERROR digi_cbr_sdh_rrmp_scramble_cfg(digi_handle_t                *digi_handle,
                                                digi_cbr_client_chnl_t       *chnl_ctxt_ptr,
                                                cbrc_port_dir_t               dir,
                                                sdh_pmg_rrmp_scramble_cfg_t  *scramble_cfg);

PUBLIC PMC_ERROR digi_cbr_sdh_rrmp_framing_cfg(digi_handle_t               *digi_handle,
                                               digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                               cbrc_port_dir_t              dir,
                                               sdh_pmg_rrmp_framing_cfg_t  *framing_cfg);

PUBLIC PMC_ERROR digi_cbr_sdh_rrmp_alarm_cfg(digi_handle_t               *digi_handle,
                                             digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                             cbrc_port_dir_t              dir,
                                             sdh_pmg_rrmp_alarm_cfg_t    *framing_cfg);

PUBLIC PMC_ERROR digi_cbr_sdh_rrmp_scramble_cfg_get(digi_handle_t                *digi_handle,
                                                    digi_cbr_client_chnl_t       *chnl_ctxt_ptr,
                                                    cbrc_port_dir_t               dir,
                                                    sdh_pmg_rrmp_scramble_cfg_t  *scramble_cfg);

PUBLIC PMC_ERROR digi_cbr_sdh_rrmp_framing_cfg_get(digi_handle_t               *digi_handle,
                                                   digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                                   cbrc_port_dir_t              dir,
                                                   sdh_pmg_rrmp_framing_cfg_t  *framing_cfg);

PUBLIC PMC_ERROR digi_cbr_sdh_rrmp_alarm_cfg_get(digi_handle_t               *digi_handle,
                                                 digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                                 cbrc_port_dir_t              dir,
                                                 sdh_pmg_rrmp_alarm_cfg_t    *framing_cfg);

PUBLIC PMC_ERROR digi_cbr_sdh_rrmp_byte_get(digi_handle_t            *digi_handle,
                                            digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                            cbrc_port_dir_t           dir,
                                            sdh_pmg_rrmp_byte_type_t  type,
                                            UINT8                    *byte_ptr);

/*TRMP*/
PUBLIC PMC_ERROR digi_cbr_sdh_trmp_alarm_cfg(digi_handle_t               *digi_handle,
                                               digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                               sdh_pmg_trmp_alarm_cfg_t    *alarm_cfg);
PUBLIC PMC_ERROR digi_cbr_sdh_trmp_alarm_cfg_get(digi_handle_t               *digi_handle,
                                                   digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                                   sdh_pmg_trmp_alarm_cfg_t    *alarm_cfg);


/* SDH SBER */
PUBLIC PMC_ERROR digi_cbr_sdh_sber_byte_cfg(digi_handle_t                *digi_handle,
                                            sdh_pmg_sber_byte_t           byte);

PUBLIC PMC_ERROR digi_cbr_sdh_sber_byte_get(digi_handle_t                *digi_handle,
                                            sdh_pmg_sber_byte_t          *byte_ptr);

PUBLIC PMC_ERROR digi_cbr_sdh_sber_sf_enable(digi_handle_t            *digi_handle,
                                             digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                             cbrc_port_dir_t           dir,
                                             sdh_pmg_sber_ber_t        ber);

PUBLIC PMC_ERROR digi_cbr_sdh_sber_sd_enable(digi_handle_t            *digi_handle,
                                             digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                             cbrc_port_dir_t           dir,
                                             sdh_pmg_sber_ber_t        ber);

PUBLIC PMC_ERROR digi_cbr_sdh_sber_sd_ber_get(digi_handle_t            *digi_handle,
                                              digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                              cbrc_port_dir_t           dir,
                                              sdh_pmg_sber_ber_t       *ber_ptr);

PUBLIC PMC_ERROR digi_cbr_sdh_sber_sf_ber_get(digi_handle_t            *digi_handle,
                                              digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                              cbrc_port_dir_t           dir,
                                              sdh_pmg_sber_ber_t       *ber_ptr);


/* SDH RTTP */
PUBLIC PMC_ERROR digi_cbr_sdh_rttp_len_cfg(digi_handle_t               *digi_handle,
                                           digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                           cbrc_port_dir_t              dir,
                                           sdh_pmg_rttp_length_t        length,
                                           sdh_pmg_rttp_algo_t          algo);

PUBLIC PMC_ERROR digi_cbr_sdh_rttp_len_get(digi_handle_t               *digi_handle,
                                           digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                           cbrc_port_dir_t              dir,
                                           sdh_pmg_rttp_length_t       *length_ptr);

PUBLIC PMC_ERROR digi_cbr_sdh_rttp_algo_get(digi_handle_t               *digi_handle,
                                           digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                           cbrc_port_dir_t              dir,
                                           sdh_pmg_rttp_algo_t         *algo_ptr);

PUBLIC PMC_ERROR digi_cbr_sdh_rttp_expect_trace_set(digi_handle_t              *digi_handle,
                                                    digi_cbr_client_chnl_t     *chnl_ctxt_ptr,
                                                    cbrc_port_dir_t             dir,
                                                    sdh_pmg_rttp_trace_t       *expect_trace);

PUBLIC PMC_ERROR digi_cbr_sdh_rttp_cap_trace_get(digi_handle_t               *digi_handle,
                                                 digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                                 cbrc_port_dir_t              dir,
                                                 sdh_pmg_rttp_trace_t        *cap_trace);

PUBLIC PMC_ERROR digi_cbr_sdh_rttp_accept_trace_get(digi_handle_t               *digi_handle,
                                                    digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                                    cbrc_port_dir_t              dir,
                                                    sdh_pmg_rttp_trace_t        *accept_trace);

PUBLIC PMC_ERROR digi_cbr_sdh_rttp_expect_trace_get(digi_handle_t              *digi_handle,
                                                    digi_cbr_client_chnl_t     *chnl_ctxt_ptr,
                                                    cbrc_port_dir_t             dir,
                                                    sdh_pmg_rttp_trace_t       *expect_trace);


/* SDH PRBS */
PUBLIC PMC_ERROR digi_cbr_sdh_prbs_mon_set(digi_handle_t           *digi_handle,
                                           digi_cbr_client_chnl_t  *chnl_ctxt_ptr,
                                           sdh_pmg_prbs_cfg_t      *cfg_ptr);

PUBLIC PMC_ERROR digi_cbr_sdh_prbs_mon_resync(digi_handle_t           *digi_handle,
                                              digi_cbr_client_chnl_t  *chnl_ctxt_ptr);

PUBLIC PMC_ERROR digi_cbr_sdh_prbs_mon_get(digi_handle_t           *digi_handle,
                                           digi_cbr_client_chnl_t  *chnl_ctxt_ptr, 
                                           sdh_pmg_prbs_cfg_t      *cfg_ptr);

PUBLIC PMC_ERROR digi_cbr_sdh_prbs_mon_sync_status_get(digi_handle_t           *digi_handle,
                                                       digi_cbr_client_chnl_t  *chnl_ctxt_ptr, 
                                                       util_patt_status_t      *status,
                                                       UINT32                  *err_cnt);

PUBLIC PMC_ERROR digi_cbr_sdh_prbs_mon_lfsr_state_get(digi_handle_t            *digi_handle,
                                                      digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                      UINT32                   *status);

PUBLIC PMC_ERROR digi_cbr_sdh_prbs_gen_set(digi_handle_t           *digi_handle,
                                           digi_cbr_client_chnl_t  *chnl_ctxt_ptr,
                                           sdh_pmg_prbs_cfg_t      *cfg_ptr);

PUBLIC PMC_ERROR digi_cbr_sdh_prbs_gen_get(digi_handle_t           *digi_handle,
                                           digi_cbr_client_chnl_t  *chnl_ctxt_ptr, 
                                           sdh_pmg_prbs_cfg_t      *cfg_ptr);

PUBLIC PMC_ERROR digi_cbr_sdh_prbs_error_ins(digi_handle_t           *digi_handle,
                                             digi_cbr_client_chnl_t  *chnl_ctxt_ptr);

PUBLIC PMC_ERROR digi_cbr_sdh_prbs_oh_ins_set(digi_handle_t           *digi_handle,
                                              digi_cbr_client_chnl_t  *chnl_ctxt_ptr, 
                                              sdh_pmg_prbs_oh_ins_t    ins_type,
                                              char                    *ins_value_ptr,
                                              BOOL                     ins_enable);

PUBLIC PMC_ERROR digi_cbr_sdh_prbs_oh_ins_get(digi_handle_t               *digi_handle,
                                              digi_cbr_client_chnl_t      *chnl_ctxt_ptr, 
                                              sdh_pmg_prbs_oh_ins_t        ins_type,
                                              char                        *ins_value_ptr,
                                              BOOL                        *ins_enable_ptr);

    
/* CBR FSGM */
PUBLIC PMC_ERROR digi_cbr_fsgm_fault_force(digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           cbrc_port_dir_t           dir,
                                           BOOL                      enable);

PUBLIC PMC_ERROR digi_cbr_fsgm_fault_force_get(digi_handle_t            *digi_handle,
                                               digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                               cbrc_port_dir_t           dir,
                                               BOOL                     *enable_ptr);

PUBLIC PMC_ERROR digi_cbr_fsgm_pattern_cfg(digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           cbrc_port_dir_t           dir,
                                           cbr_fsgm_pattern_t        pattern);

PUBLIC PMC_ERROR digi_cbr_fsgm_pattern_cfg_get(
                                           digi_handle_t            *digi_handle,
                                           cbrc_port_dir_t           dir,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           cbr_fsgm_pattern_t       *pattern_ptr);

PUBLIC PMC_ERROR digi_cbr_fsgm_user_fault_pattern_set(digi_handle_t                 *digi_handle,
                                                      digi_cbr_client_chnl_t        *chnl_ctxt_ptr,
                                                      cbrc_port_dir_t                dir,
                                                      cbr_fsgm_user_fault_pattern_t  user_pattern);

PUBLIC PMC_ERROR digi_cbr_fsgm_user_fault_pattern_get(digi_handle_t                 *digi_handle,
                                                      digi_cbr_client_chnl_t        *chnl_ctxt_ptr,
                                                      cbrc_port_dir_t                dir,
                                                      cbr_fsgm_user_fault_pattern_t *user_pattern_ptr);

PUBLIC PMC_ERROR digi_cbr_fsgm_pn11_mon_enable(digi_handle_t            *digi_handle,
                                               digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                               cbrc_port_dir_t           dir,
                                               BOOL                      enable);

PUBLIC PMC_ERROR digi_cbr_fsgm_pn11_mon_enable_get(digi_handle_t            *digi_handle,
                                                   digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                   cbrc_port_dir_t           dir,
                                                   BOOL                     *enable_ptr);

PUBLIC PMC_ERROR digi_cbr_patt_ins_fault_force(digi_handle_t            *digi_handle,
                                               digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                               cbrc_port_dir_t           dir,
                                               BOOL                      force);

PUBLIC PMC_ERROR digi_cbr_patt_ins_fault_force_get(digi_handle_t            *digi_handle,
                                                   digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                   cbrc_port_dir_t           dir,
                                                   BOOL                     *force_ptr);

PUBLIC PMC_ERROR digi_cbr_patt_ins_enable(digi_handle_t            *digi_handle,
                                          digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                          cbrc_port_dir_t           dir,
                                          BOOL                      enable);

PUBLIC PMC_ERROR digi_cbr_patt_ins_enable_get(digi_handle_t            *digi_handle,
                                              digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                              cbrc_port_dir_t           dir,
                                              BOOL                     *enable_ptr);

PUBLIC PMC_ERROR digi_cbr_fsgm_insert_cfg(digi_handle_t                *digi_handle,
                                          digi_cbr_client_chnl_t       *chnl_ctxt_ptr,
                                          cbrc_port_dir_t               dir,
                                          cbr_fsgm_pattern_cfg_t       *cfg_ptr);

PUBLIC PMC_ERROR digi_cbr_fsgm_insert_cfg_get(digi_handle_t                *digi_handle,
                                              digi_cbr_client_chnl_t       *chnl_ctxt_ptr,
                                              cbrc_port_dir_t               dir,
                                              cbr_fsgm_pattern_cfg_t       *cfg_ptr);



PUBLIC PMC_ERROR digi_cbr_fc800_dlolb_gen_set(digi_handle_t          *digi_handle,
                                              digi_cbr_client_chnl_t *chnl_ctxt_ptr,
                                              cbrc_port_dir_t         dir,
                                              fc800_pmg_dlolb_gen_t  *dlolb_gen_cfg_ptr);

PUBLIC PMC_ERROR digi_cbr_fc800_dlolb_gen_get(digi_handle_t             *digi_handle,
                                              digi_cbr_client_chnl_t    *chnl_ctxt_ptr,
                                              cbrc_port_dir_t            dir,
                                              fc800_pmg_dlolb_gen_t     *dlolb_gen_cfg_ptr);


/* FC1200 PMON */
PUBLIC PMC_ERROR digi_cbr_fc1200_pmon_dlolb_cfg(digi_handle_t                    *digi_handle,
                                                digi_cbr_client_chnl_t           *chnl_ctxt_ptr,
                                                cbrc_port_dir_t                   dir,
                                                fc1200_pmon_dlolb_cfg_t          *cfg);

PUBLIC PMC_ERROR digi_cbr_fc1200_pmon_dlolb_cfg_get(digi_handle_t                    *digi_handle,
                                                    digi_cbr_client_chnl_t           *chnl_ctxt_ptr,
                                                    cbrc_port_dir_t                   dir,
                                                    fc1200_pmon_dlolb_cfg_t          *cfg);

PUBLIC PMC_ERROR digi_cbr_fc1200_pmon_dlolb_force(digi_handle_t            *digi_handle,
                                                  digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                  cbrc_port_dir_t           dir,
                                                  BOOL                      enable);

PUBLIC PMC_ERROR digi_cbr_fc1200_pmon_dlolb_force_get(digi_handle_t            *digi_handle,
                                                      digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                      cbrc_port_dir_t           dir,
                                                      BOOL                     *enable_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_pmon_invalid_block_mode_set(
                                   digi_handle_t                    *digi_handle,
                                   digi_cbr_client_chnl_t           *chnl_ctxt_ptr,
                                   cbrc_port_dir_t                   dir,
                                   fc1200_pmon_invalid_block_mode_t  mode);

PUBLIC PMC_ERROR digi_cbr_fc1200_pmon_invalid_block_mode_get(
                                   digi_handle_t                    *digi_handle,
                                   digi_cbr_client_chnl_t           *chnl_ctxt_ptr,
                                   cbrc_port_dir_t                   dir,
                                   fc1200_pmon_invalid_block_mode_t *mode_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_prbs31_enable(digi_handle_t            *digi_handle,
                                               digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                               cbrc_port_dir_t           dir,
                                               BOOL                      enable);

PUBLIC PMC_ERROR digi_cbr_fc1200_prbs31_enable_get(digi_handle_t            *digi_handle,
                                                   digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                   cbrc_port_dir_t           dir,
                                                   BOOL                     *enable_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_lfos_force(digi_handle_t            *digi_handle,
                                            digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                            cbrc_port_dir_t           dir,
                                            BOOL                      enable);

PUBLIC PMC_ERROR digi_cbr_fc1200_lfos_force_get(digi_handle_t            *digi_handle,
                                                digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                cbrc_port_dir_t           dir,
                                                BOOL                     *enable_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_lfos_set(digi_handle_t            *digi_handle,
                                          digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                          cbrc_port_dir_t           dir,
                                          cbrc_fc1200_lfos_t       *lfos_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_lfos_get(digi_handle_t            *digi_handle,
                                          digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                          cbrc_port_dir_t           dir,
                                          cbrc_fc1200_lfos_t       *lfos_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_rx_lfos_cfg(digi_handle_t                 *digi_handle,
                                             digi_cbr_client_chnl_t        *chnl_ctxt_ptr,
                                             cbrc_fc1200_rx_lfos_cfg_t     *lfos_cfg_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_rx_lfos_cfg_get(digi_handle_t                 *digi_handle,
                                                 digi_cbr_client_chnl_t        *chnl_ctxt_ptr,
                                                 cbrc_fc1200_rx_lfos_cfg_t     *lfos_cfg_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_tx_lfos_cfg(digi_handle_t                 *digi_handle,
                                             digi_cbr_client_chnl_t        *chnl_ctxt_ptr,
                                             cbrc_fc1200_tx_lfos_cfg_t     *lfos_cfg_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_tx_lfos_cfg_get(digi_handle_t                 *digi_handle,
                                                 digi_cbr_client_chnl_t        *chnl_ctxt_ptr,
                                                 cbrc_fc1200_tx_lfos_cfg_t     *lfos_cfg_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_rx_restore_invalid_idle_enable(
                                           digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           BOOL                      enable);

PUBLIC PMC_ERROR digi_cbr_fc1200_rx_restore_invalid_idle_enable_get(
                                           digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           BOOL                     *enable_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_rx_descrambler_enable(digi_handle_t            *digi_handle,
                                                       digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                       BOOL                      enable);

PUBLIC PMC_ERROR digi_cbr_fc1200_rx_descrambler_enable_get(
                                        digi_handle_t            *digi_handle,
                                        digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                        BOOL                     *enable_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_rx_ecb_force(digi_handle_t            *digi_handle,
                                              digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                              BOOL                      enable);

PUBLIC PMC_ERROR digi_cbr_fc1200_rx_ecb_force_get(digi_handle_t            *digi_handle,
                                                  digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                  BOOL                     *enable_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_rx_ecb_set(digi_handle_t            *digi_handle,
                                            digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                            cbrc_fc1200_ecb_t        *ecb_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_rx_ecb_get(digi_handle_t            *digi_handle,
                                            digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                            cbrc_fc1200_ecb_t        *ecb_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_rx_gfp_payload_scramble_enable(
                                              digi_handle_t            *digi_handle,
                                              digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                              BOOL                      enable);

PUBLIC PMC_ERROR digi_cbr_fc1200_rx_gfp_payload_scramble_enable_get(
                                              digi_handle_t            *digi_handle,
                                              digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                              BOOL                     *enable_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_rx_gfp_set(digi_handle_t            *digi_handle,
                                            digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                            cbrc_fc1200_rx_gfp_t     *gfp_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_rx_gfp_get(digi_handle_t            *digi_handle,
                                            digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                            cbrc_fc1200_rx_gfp_t     *gfp_ptr);

PUBLIC PMC_ERROR digi_cbr_fc1200_tx_corrupt_blk_conv_enable(
                                           digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           BOOL                      enable);

PUBLIC PMC_ERROR digi_cbr_fc1200_tx_corrupt_blk_conv_enable_get(
                                           digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           BOOL                     *enable_ptr);

/* Interrupts and PMON */

PUBLIC PMC_ERROR digi_cbr_pmon_retrieve(digi_handle_t       *digi_handle,
                                        digi_cbr_pmon_t     *pmon_table_ptr);
                                        
PUBLIC PMC_ERROR digi_cbr_pmon_chnl_get(digi_handle_t                *digi_handle,
                                        digi_cbr_client_chnl_t       *chnl_handle,
                                        digi_cbr_pmon_t              *pmon_table_ptr,
                                        digi_cbr_pmon_chnl_t        **pmon_chnl_table_pptr);

PUBLIC PMC_ERROR digi_cbr_int_register_cb(digi_handle_t      *digi_handle,
                                          void               *token,
                                          UINT32              token_size,
                                          digi_int_cb_fcn     user_cb);
       
PUBLIC PMC_ERROR digi_cbr_int_chnl_register_cb(digi_handle_t           *digi_handle,
                                               void                    *token,
                                               UINT32                   token_size,
                                               digi_int_cb_fcn          user_cb);

PUBLIC PMC_ERROR digi_cbr_int_chnl_enable(digi_handle_t            *digi_handle,
                                          digi_cbr_client_chnl_t   *chnl_handle,   
                                          cbrc_int_chnl_t          *int_table_ptr);

PUBLIC PMC_ERROR digi_cbr_int_chnl_disable(digi_handle_t         *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_handle,   
                                           cbrc_int_chnl_t          *int_table_ptr);

PUBLIC PMC_ERROR digi_cbr_int_clear(digi_handle_t            *digi_handle,
                                    digi_cbr_int_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_cbr_int_chnl_clear(digi_handle_t            *digi_handle,
                                         digi_cbr_client_chnl_t   *chnl_handle,
                                         digi_cbr_int_chnl_t      *int_table_ptr);

PUBLIC PMC_ERROR digi_cbr_int_retrieve(digi_handle_t      *digi_handle,
                                       digi_cbr_int_t     *int_table_ptr);

PUBLIC PMC_ERROR digi_cbr_int_filt_retrieve(digi_handle_t        *digi_handle,
                                            digi_cbr_int_chnl_t  *filt_table_ptr,
                                            digi_cbr_int_t       *int_table_ptr);

PUBLIC PMC_ERROR digi_cbr_int_chnl_filt_retrieve(digi_handle_t          *digi_handle,
                                                 digi_cbr_client_chnl_t *chnl_handle,
                                                 digi_cbr_int_chnl_t    *filt_table_ptr,
                                                 digi_cbr_int_chnl_t    *int_table_ptr);

PUBLIC PMC_ERROR digi_cbr_int_chnl_get(digi_handle_t              *digi_handle,
                                       digi_cbr_client_chnl_t     *chnl_handle,
                                       digi_cbr_int_t             *int_table_ptr,
                                       digi_cbr_int_chnl_t       **int_chnl_table_pptr);

PUBLIC PMC_ERROR digi_cbr_int_enabled_get(digi_handle_t       *digi_handle,
                                          digi_cbr_int_t      *int_table_ptr);

PUBLIC PMC_ERROR digi_cbr_int_enabled_check(digi_handle_t   *digi_handle);

PUBLIC PMC_ERROR digi_cbr_int_chnl_enabled_check(digi_handle_t            *digi_handle,
                                                 digi_cbr_int_summary_t   *int_summary_ptr);

PUBLIC PMC_ERROR digi_cbr_int_summary_get(digi_handle_t              *digi_handle,
                                          UINT32                      unused,
                                          digi_cbr_int_summary_t     *int_summary_ptr);

PUBLIC PMC_ERROR digi_cbr_sfis_odd_parity_set(digi_handle_t             *digi_handle,
                                              digi_cbr_client_chnl_t    *chnl_ctxt_ptr,
                                              util_global_direction_t    dir,
                                              BOOL8                      inv);

PUBLIC PMC_ERROR digi_cbr_sfis_odd_parity_get(digi_handle_t             *digi_handle,
                                              digi_cbr_client_chnl_t    *chnl_ctxt_ptr,                                          
                                              util_global_direction_t    dir,
                                              BOOL8                     *inv_ptr);

/* FC800 PMG */
#ifdef PMC_DEPRECATED
PUBLIC PMC_ERROR digi_cbr_fc800_user_pattern_set(digi_handle_t             *digi_handle,
                                                 digi_cbr_client_chnl_t    *chnl_ctxt_ptr,
                                                 cbrc_port_dir_t            dir,
                                                 UINT32                     user_pattern);

PUBLIC PMC_ERROR digi_cbr_fc800_user_pattern_get(digi_handle_t             *digi_handle,
                                                 digi_cbr_client_chnl_t    *chnl_ctxt_ptr,
                                                 cbrc_port_dir_t            dir,
                                                 UINT32                    *user_pattern_ptr);

PUBLIC PMC_ERROR digi_cbr_fc800_pattern_ins_set(digi_handle_t                *digi_handle,
                                                digi_cbr_client_chnl_t       *chnl_ctxt_ptr,
                                                cbrc_port_dir_t               dir,
                                                fc800_pmg_pattern_ins_cfg_t   pattern_ins_cfg);

PUBLIC PMC_ERROR digi_cbr_fc800_pattern_ins_get(digi_handle_t                *digi_handle,
                                                digi_cbr_client_chnl_t       *chnl_ctxt_ptr,
                                                cbrc_port_dir_t               dir,
                                                fc800_pmg_pattern_ins_cfg_t  *pattern_ins_cfg_ptr);

PUBLIC PMC_ERROR digi_cbr_fc800_pattern_force(digi_handle_t          *digi_handle,
                                              digi_cbr_client_chnl_t *chnl_ctxt_ptr,
                                              cbrc_port_dir_t         dir,
                                              BOOL                    force);

PUBLIC PMC_ERROR digi_cbr_fc800_pattern_force_get(digi_handle_t             *digi_handle,
                                                  digi_cbr_client_chnl_t    *chnl_ctxt_ptr,
                                                  cbrc_port_dir_t            dir,
                                                  BOOL                      *force_ptr);
#endif /* PMC_DEPRECATED */

#ifdef __cplusplus
}
#endif


#endif /* _DIGI_CBR_API_H */

/*
** end of file
*/
