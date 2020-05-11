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

#ifndef _EXCO3M_H
#define _EXCO3M_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "util_global.h"
#include "exco3m_api.h"

/*
** Enumerated Types
*/
 /*!< Declare EXCO3M datapath */
typedef enum
{
    EXCO3M_GMP_709_40GE = 0,
    EXCO3M_GFP_GSUP43_C62_40GE = 1
} exco3m_datapath_t;
/*
** Constants
*/

/*
** Macro Definitions
*/
/* EXCO3M_INT(handle, tsb_name, tsb_field, struct_field) */
#define EXCO3M_INT_TABLE_DEFINE() \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 TX_LOS              , tx_los_i              ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 TX_LF_OS_DET        , tx_lf_os_det_i        ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 TX_RF_OS_DET        , tx_rf_os_det_i        ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 TX_IDLE_TEST_ERR_DET, tx_idle_test_err_det_i); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 TX_SKEW_RANGE_VIOL  , tx_skew_range_viol_i  ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 TX_PCS_INV_BLK_DET  , tx_pcs_inv_blk_det_i  ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 TX_PCS_ERR_BLK_DET  , tx_pcs_err_blk_det_i  ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 TX_PCS_HBER         , tx_pcs_hber_i         ); \
    EXCO3M_INT_ARRAY (exco3m_handle, exco3m, EXCO3M_N_LANES, TX_PCS_LOBL         , tx_pcs_lobl_i         ); \
    EXCO3M_INT_ARRAY (exco3m_handle, exco3m, EXCO3M_N_LANES, TX_PCS_BER_DET      , tx_pcs_ber_det_i      ); \
    EXCO3M_INT_ARRAY (exco3m_handle, exco3m, EXCO3M_N_LANES, TX_PCS_BIP_ERR_DET  , tx_pcs_bip_err_det_i  ); \
    EXCO3M_INT_ARRAY (exco3m_handle, exco3m, EXCO3M_N_LANES, TX_LOAM             , tx_loam_i             ); \
    EXCO3M_INT_ARRAY (exco3m_handle, exco3m, EXCO3M_N_LANES, TX_MRK_INV_DET      , tx_mrk_inv_det_i      ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 RX_TSF              , rx_tsf_i              ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 RX_LF_OS_DET        , rx_lf_os_det_i        ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 RX_RF_OS_DET        , rx_rf_os_det_i        ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 RX_IDLE_TEST_ERR_DET, rx_idle_test_err_det_i); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 RX_PCS_ERR_BLK_DET  , rx_pcs_err_blk_det_i  ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 RX_1027B_LOBL       , rx_1027b_lobl_i       ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 RX_1027B_HBER       , rx_1027b_hber_i       ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 RX_1027B_BER        , rx_1027b_ber_i        ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 RX_SKEW_RANGE_VIOL  , rx_skew_range_viol_i  ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 RX_513B_POS_ERR_DET , rx_513b_pos_err_det_i ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 RX_WELL_PKT_ERR     , rx_well_pkt_err_i     ); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 RX_LANE_ALGN_MRK_ERR, rx_lane_algn_mrk_err_i); \
    EXCO3M_INT_SINGLE(exco3m_handle, exco3m,                 RX_1027B_PAR_ERR    , rx_1027b_par_err_i    ); \
    EXCO3M_INT_ARRAY (exco3m_handle, exco3m, EXCO3M_N_LANES, RX_OTN_BIP_ERR_DET  , rx_otn_bip_err_det_i  ); \
    EXCO3M_INT_ARRAY (exco3m_handle, exco3m, EXCO3M_N_LANES, RX_LOAM             , rx_loam_i             ); \
    EXCO3M_INT_ARRAY (exco3m_handle, exco3m, EXCO3M_N_LANES, RX_PCS_MRK_INV_DET  , rx_pcs_mrk_inv_det_i  ); \

/*    EXCO3M_STATUS_ARRAY       (exco3m_handle, exco3m,                 EXCO3M_N_LANES, RX_LOAM_V      , rx_loam_v      ); \*/
#define EXCO3M_STATUS_TABLE_DEFINE() \
    EXCO3M_STATUS_SINGLE(exco3m_handle, exco3m,                 TX_LOS_V       , tx_los_v       ); \
    EXCO3M_STATUS_SINGLE(exco3m_handle, exco3m,                 TX_PCS_HBER_V  , tx_pcs_hber_v  ); \
    EXCO3M_STATUS_ARRAY (exco3m_handle, exco3m, EXCO3M_N_LANES, TX_PCS_LOBL_V  , tx_pcs_lobl_v  ); \
    EXCO3M_STATUS_ARRAY (exco3m_handle, exco3m, EXCO3M_N_LANES, TX_LOAM_V      , tx_loam_v      ); \
    EXCO3M_STATUS_SINGLE(exco3m_handle, exco3m,                 RX_TSF_V       , rx_tsf_v       ); \
    EXCO3M_STATUS_SINGLE(exco3m_handle, exco3m,                 RX_1027B_LOBL_V, rx_1027b_lobl_v); \
    EXCO3M_STATUS_SINGLE(exco3m_handle, exco3m,                 RX_1027B_HBER_V, rx_1027b_hber_v); \

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: exco3m_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is the exco3m context 
*
* ELEMENTS:
*   base - base PMC handle 
*
*******************************************************************************/
typedef struct exco3m_handle_struct_t {
    pmc_handle_t base;
    
} exco3m_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC exco3m_handle_t *exco3m_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void exco3m_ctxt_destroy(exco3m_handle_t *exco3m_handle);
PUBLIC void exco3m_handle_init(exco3m_handle_t *exco3m_handle);
PUBLIC PMC_ERROR exco3m_handle_restart_init(exco3m_handle_t *exco3m_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                            pmc_energy_state_t energy_state);
PUBLIC UINT32 exco3m_base_address_get(exco3m_handle_t *exco3m_handle);

#ifndef DOXYGEN_PUBLIC_ONLY
PUBLIC PMC_ERROR exco3m_cfg       (exco3m_handle_t *hndl, UINT32 link, exco3m_datapath_t datapath, BOOL8 enable, BOOL8 set_rx_pcs_push_ena);
PUBLIC PMC_ERROR exco3m_activate  (exco3m_handle_t *hndl, UINT32 link);
PUBLIC PMC_ERROR exco3m_deactivate(exco3m_handle_t *hndl, UINT32 link);
PUBLIC PMC_ERROR exco3m_lowpwr    (exco3m_handle_t *hndl, BOOL8 value);
#endif

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */ 
/* LCOV_EXCL_START */
PUBLIC PMC_ERROR exco3m_status_get(exco3m_handle_t *hndl, BOOL8 clear_int, UINT32 *tx_status_ptr);
/* LCOV_EXCL_STOP */
#endif/*#ifndef DOXYGEN_PUBLIC_ONLY*/

PUBLIC PMC_ERROR exco3m_init(exco3m_handle_t *hndl, pmc_energy_state_t energy_state);
PUBLIC BOOL8 exco3m_start_state_test(exco3m_handle_t *hndl);
PUBLIC void exco3m_tx_lane_info_get(exco3m_handle_t *hndl,
                                    UINT8 txLaneId[4],
                                    UINT8 txLaneSkewFifoAddr[4]);
PUBLIC void exco3m_rx_lf_os_cfg(exco3m_handle_t *hndl,
                                BOOL8            enable);
PUBLIC void exco3m_tx_lf_os_cfg(exco3m_handle_t *hndl,
                                BOOL8            enable);

PUBLIC void exco3m_rx_rf_os_cfg(exco3m_handle_t *hndl,
                                BOOL8            enable);
PUBLIC void exco3m_tx_rf_os_cfg(exco3m_handle_t *hndl,
                                BOOL8            enable);

PUBLIC void exco3m_rx_lf_os_cfg_get(exco3m_handle_t *hndl,
                                    BOOL8 *enable);
PUBLIC void exco3m_tx_lf_os_cfg_get(exco3m_handle_t *hndl,
                                    BOOL8 *enable);

PUBLIC void exco3m_rx_rf_os_cfg_get(exco3m_handle_t *hndl,
                                    BOOL8 *enable);
PUBLIC void exco3m_tx_rf_os_cfg_get(exco3m_handle_t *hndl,
                                    BOOL8 *enable);


PUBLIC PMC_POLL_RETURN_TYPE exco3m_tx_align_poll(exco3m_handle_t *hndl);

PUBLIC PMC_ERROR exco3m_int_chnl_enable(exco3m_handle_t    *exco3m_handle,
                                        UINT32              link,
                                        exco3m_int_chnl_t  *int_table_ptr,
                                        exco3m_int_chnl_t  *int_en_table_ptr,
                                        BOOL8                enable);
PUBLIC PMC_ERROR exco3m_int_chnl_clear(exco3m_handle_t    *exco3m_handle,
                                       UINT32              link,
                                       exco3m_int_chnl_t  *int_table_ptr,
                                       exco3m_int_chnl_t  *int_en_table_ptr);
PUBLIC PMC_ERROR exco3m_int_chnl_retrieve(exco3m_handle_t    *exco3m_handle,
                                          UINT32              link,
                                          exco3m_int_chnl_t  *filt_table_ptr,
                                          exco3m_int_chnl_t  *int_table_ptr);
PUBLIC PMC_ERROR exco3m_int_chnl_enabled_check(exco3m_handle_t     *exco3m_handle,
                                               UINT32               link,
                                               exco3m_int_chnl_t   *int_en_table_ptr,
                                               BOOL8                *int_found_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _EXCO3M_H */

/*
** end of file
*/
