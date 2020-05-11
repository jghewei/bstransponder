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

#ifndef _CBR_FSGM_H
#define _CBR_FSGM_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "cbr_fsgm_api.h"

/*
** Enumerated Types
*/

/*
** Constants
*/


#define CBR_FSGM_MAX_INDEX                  4
#define CBR_FSGM_RESET                      0x1
#define CBR_FSGM_ACTIVE                     0x0
#define CBR_FSGM_ENABLE                     0x1
#define CBR_FSGM_DISABLE                    0x0
#define CBR_FSGM_GAP_NUM_FC800              0x40D99
#define CBR_FSGM_GAP_NUM_FC1200             0x50407
#define CBR_FSGM_GAP_NUM_CPRI5              0x25800
#define CBR_FSGM_GAP_NUM_CPRI6              0x2EE00
#define CBR_FSGM_GAP_NUM_CPRI10             0x4B000
#define CBR_FSGM_GAP_NUM_GDPS5              0x2625A
#define CBR_FSGM_GAP_NUM_GDPS10             0x4C4B4
#define CBR_FSGM_GAP_NUM_STS192             0x4BF00
#define CBR_FSGM_GAP_NUM_10GE               10312500
#define CBR_FSGM_GAP_NUM_STS768             0x4BF00
#define CBR_FSGM_GAP_DENOM                  0x556E0
#define CBR_FSGM_GAP_DENOM_10GE             11197440

#define CBR_FSGM_PN11_PATT_MODE             0x0
#define CBR_FSGM_MSAIS_PATT_MODE            0x1
#define CBR_FSGM_8B10B_PATT_MODE            0x2
#define CBR_FSGM_64B66B_PATT_MODE           0x3

/*
** Macro Definitions
*/
/* CBR_FSGM_INT(handle, tsb_name, tsb_field, struct_field) */
#define CBR_FSGM_INT_TABLE_DEFINE() \
    CBR_FSGM_INT(cbr_fsgm_handle, fc_cbr_patt_ins, UNDERRUN                   , underrun_i                   ); \
    CBR_FSGM_INT(cbr_fsgm_handle, fc_cbr_patt_ins, TX_PN11_SYNC               , tx_pn11_sync_i               ); \
    CBR_FSGM_INT(cbr_fsgm_handle, fc_cbr_patt_ins, RX_PN11_SYNC               , rx_pn11_sync_i               ); \
    CBR_FSGM_INT(cbr_fsgm_handle, fc_cbr_patt_ins, TX_PN11_ERR                , tx_pn11_err_i                ); \
    CBR_FSGM_INT(cbr_fsgm_handle, fc_cbr_patt_ins, RX_PN11_ERR                , rx_pn11_err_i                ); \
    CBR_FSGM_INT(cbr_fsgm_handle, fc_cbr_patt_ins, RX_PRBS31_ERR              , rx_prbs31_err_i              ); \
    CBR_FSGM_INT(cbr_fsgm_handle, fc_cbr_patt_ins, RX_PRBS31_ALL_ONES_OR_ZEROS, rx_prbs31_all_ones_or_zeros_i); \
    CBR_FSGM_INT_FIELD(cbr_fsgm_handle,   cbr_fsgm    , CH,    PN11_DET       , pn11_det_i                   ); \

#define CBR_FSGM_STATUS_TABLE_DEFINE() \
    CBR_FSGM_STATUS(cbr_fsgm_handle, fc_cbr_patt_ins, UNDERRUN_V                   , underrun_v                   ); \
    CBR_FSGM_STATUS(cbr_fsgm_handle, fc_cbr_patt_ins, TX_PN11_SYNC_V               , tx_pn11_sync_v               ); \
    CBR_FSGM_STATUS(cbr_fsgm_handle, fc_cbr_patt_ins, RX_PN11_SYNC_V               , rx_pn11_sync_v               ); \
    CBR_FSGM_STATUS(cbr_fsgm_handle, fc_cbr_patt_ins, RX_PRBS31_ALL_ONES_OR_ZEROS_V, rx_prbs31_all_ones_or_zeros_v); \
    CBR_FSGM_STATUS_FIELD(cbr_fsgm_handle,   cbr_fsgm    , CH,    PN11_DET_V       , pn11_det_v                   ); \

/*
** Structures and Unions
*/
/*******************************************************************************
* ENUM: cbr_fsgm_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration to describe the client traffic modes that can be 
*   used sent through the CBR_FSGM.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    CBR_FSGM_MODE_NONE = 0,

    CBR_FSGM_MODE_40G_STS768,
    CBR_FSGM_MODE_40G_STM256,
   
    CBR_FSGM_MODE_10G_STS192,
    CBR_FSGM_MODE_10G_STM64,
    CBR_FSGM_MODE_10G_FC1200_ODU1F,
    CBR_FSGM_MODE_10G_FC1200_ODU2F,
    CBR_FSGM_MODE_10G_FC1200_GFP_ODU2E,
    CBR_FSGM_MODE_10G_FC800,
    CBR_FSGM_MODE_10G_CPRI_5G,
    CBR_FSGM_MODE_10G_CPRI_6G,
    CBR_FSGM_MODE_10G_CPRI_10G,
    CBR_FSGM_MODE_10G_GDPS_5G,
    CBR_FSGM_MODE_10G_GDPS_10G,
    CBR_FSGM_MODE_10G_STS192_PRBS,
    CBR_FSGM_MODE_10G_STM64_PRBS,
    CBR_FSGM_MODE_10G_10GE_LF,
    CBR_FSGM_MODE_10G_10GE_RF,
    CBR_FSGM_MODE_10G_8B_10B_IDLE,
    CBR_FSGM_MODE_10G_64B_66B_IDLE,
    CBR_FSGM_MODE_10G_8B_10B_ARB,
    LAST_CBR_FSGM_MODE,
} cbr_fsgm_mode_t;



/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC cbr_fsgm_handle_t *cbr_fsgm_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void cbr_fsgm_ctxt_destroy(cbr_fsgm_handle_t *cbr_fsgm_handle);
PUBLIC void cbr_fsgm_handle_init(cbr_fsgm_handle_t *cbr_fsgm_handle);


PUBLIC PMC_ERROR cbr_fsgm_init(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                               pmc_energy_state_t    energy_state);
PUBLIC BOOL cbr_fsgm_start_state_test(cbr_fsgm_handle_t *cbr_fsgm_handle);
PUBLIC PMC_ERROR cbr_fsgm_reset(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                BOOL                  reset);
PUBLIC BOOL8 cbr_fsgm_patt_ins_reset_status_get(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                                UINT32                index);

PUBLIC PMC_ERROR cbr_fsgm_patt_ins_reset(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                         UINT32                index,
                                         BOOL                  reset);
PUBLIC PMC_ERROR cbr_fsgm_ch_en(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                UINT32                index,
                                BOOL                  enable);
PUBLIC PMC_ERROR cbr_fsgm_gap_val_set(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                      UINT32                index,
                                      cbr_fsgm_mode_t       gap_mode);
PUBLIC PMC_ERROR cbr_fsgm_cfg(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                              UINT32                index,
                              UINT32                fault_ins_en);
PUBLIC PMC_ERROR cbr_fsgm_provision(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                    UINT32                index,
                                    cbr_fsgm_mode_t       mode);
PUBLIC PMC_ERROR cbr_fsgm_unprovision(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                      UINT32                index);
PUBLIC PMC_ERROR cbr_fsgm_activate(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                   UINT32                index);
PUBLIC PMC_ERROR cbr_fsgm_deactivate(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                     UINT32                index);
PUBLIC PMC_ERROR cbr_fsgm_pattern_cfg(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                      UINT32                index,
                                      cbr_fsgm_pattern_t    pattern);
PUBLIC PMC_ERROR cbr_fsgm_pattern_cfg_get(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                          UINT32                index,
                                          cbr_fsgm_pattern_t   *pattern_ptr);
PUBLIC PMC_ERROR cbr_fsgm_fault_force(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                      UINT32                index,
                                      BOOL                  enable);
PUBLIC PMC_ERROR cbr_fsgm_fault_force_get(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                          UINT32                index,
                                          BOOL                 *enable_ptr);
PUBLIC PMC_ERROR cbr_fsgm_pn11_mon_enable(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                          UINT32                index,
                                          BOOL                  enable);
PUBLIC PMC_ERROR cbr_fsgm_pn11_mon_enable_get(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                              UINT32                index,
                                              BOOL                 *enable_ptr);
PUBLIC PMC_ERROR cbr_fsgm_insert_cfg(cbr_fsgm_handle_t        *cbr_fsgm_handle,
                                     UINT32                    index,
                                     cbr_fsgm_pattern_cfg_t   *cfg_ptr);
PUBLIC PMC_ERROR cbr_fsgm_insert_cfg_get(cbr_fsgm_handle_t        *cbr_fsgm_handle,
                                         UINT32                    index,
                                         cbr_fsgm_pattern_cfg_t   *cfg_ptr);
PUBLIC PMC_ERROR cbr_fsgm_patt_ins_fault_force(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                               UINT32                index,
                                               BOOL                  force);
PUBLIC PMC_ERROR cbr_fsgm_patt_ins_fault_force_get(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                                   UINT32                index,
                                                   BOOL                 *force_ptr);
PUBLIC PMC_ERROR cbr_fsgm_patt_ins_enable(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                          UINT32                index,
                                          BOOL                  enable);
PUBLIC PMC_ERROR cbr_fsgm_patt_ins_enable_get(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                              UINT32                index,
                                              BOOL                 *enable_ptr);
PUBLIC PMC_ERROR cbr_fsgm_bus_width_set(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                        UINT8                 index);
PUBLIC PMC_ERROR cbr_fsgm_fault_size_set(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                         UINT8                 index);
PUBLIC PMC_ERROR cbr_fsgm_fault_mode_set(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                         UINT8                 index);
PUBLIC PMC_ERROR cbr_fsgm_user_fault_pattern_set(cbr_fsgm_handle_t              *cbr_fsgm_handle,
                                                 UINT8                           index,
                                                 cbr_fsgm_user_fault_pattern_t   user_pattern);
PUBLIC PMC_ERROR cbr_fsgm_user_fault_pattern_get(cbr_fsgm_handle_t              *cbr_fsgm_handle,
                                                 UINT8                           index,
                                                 cbr_fsgm_user_fault_pattern_t  *user_pattern_ptr);
PUBLIC PMC_ERROR cbr_fsgm_int_chnl_enable(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                          UINT32                index,
                                          cbr_fsgm_int_chnl_t  *int_table_ptr,
                                          cbr_fsgm_int_chnl_t  *int_en_table_ptr,
                                          BOOL                  enable);
PUBLIC PMC_ERROR cbr_fsgm_int_chnl_clear(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                         UINT32                index,
                                         cbr_fsgm_int_chnl_t  *int_table_ptr,
                                         cbr_fsgm_int_chnl_t  *int_en_table_ptr);
PUBLIC PMC_ERROR cbr_fsgm_int_chnl_retrieve(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                            UINT32                index,
                                            cbr_fsgm_int_chnl_t  *filt_table_ptr,
                                            cbr_fsgm_int_chnl_t  *int_table_ptr);
PUBLIC PMC_ERROR cbr_fsgm_int_chnl_enabled_check(cbr_fsgm_handle_t     *cbr_fsgm_handle,
                                                 UINT32                 index,
                                                 cbr_fsgm_int_chnl_t   *int_en_table_ptr,
                                                 BOOL                  *int_found_ptr);


PUBLIC void cbr_fsgm_cleanup(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                             UINT32                slice_start_index,
                             UINT32                num_link_to_clean,
                             UINT32               *links);

#ifdef __cplusplus
}
#endif

#endif /* _CBR_FSGM_H */

/*
** end of file
*/
