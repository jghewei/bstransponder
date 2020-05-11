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

#ifndef _EXCO4M_H
#define _EXCO4M_H

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
#include "exco4m_api.h"

/*
** Enumerated Types
*/
/*!< Declare exco4m datapath  */
typedef enum
{
    EXCO4M_GMP_709_100GE = 0,
    EXCO4M_GFP_GSUP43_C62_100GE = 1
} exco4m_datapath_t;


/*
** Constants
*/

/*
** Macro Definitions
*/
/* EXCO4M_INT(handle, tsb_name, tsb_field, struct_field) */
#define EXCO4M_INT_TABLE_DEFINE() \
    EXCO4M_INT      (exco4m_handle, exco4m, TX_LOS              , tx_los_i              ); \
    EXCO4M_INT      (exco4m_handle, exco4m, TX_LF_OS_DET        , tx_lf_os_det_i        ); \
    EXCO4M_INT      (exco4m_handle, exco4m, TX_RF_OS_DET        , tx_rf_os_det_i        ); \
    EXCO4M_INT      (exco4m_handle, exco4m, TX_IDLE_TEST_ERR_DET, tx_idle_test_err_det_i); \
    EXCO4M_INT      (exco4m_handle, exco4m, TX_SKEW_RANGE_VIOL  , tx_skew_range_viol_i  ); \
    EXCO4M_INT      (exco4m_handle, exco4m, TX_PCS_INV_BLK_DET  , tx_pcs_inv_blk_det_i  ); \
    EXCO4M_INT      (exco4m_handle, exco4m, TX_PCS_ERR_BLK_DET  , tx_pcs_err_blk_det_i  ); \
    EXCO4M_INT      (exco4m_handle, exco4m, TX_PCS_HBER         , tx_pcs_hber_i         ); \
    EXCO4M_INT      (exco4m_handle, exco4m, RX_TSF              , rx_tsf_i              ); \
    EXCO4M_INT      (exco4m_handle, exco4m, RX_LF_OS_DET        , rx_lf_os_det_i        ); \
    EXCO4M_INT      (exco4m_handle, exco4m, RX_RF_OS_DET        , rx_rf_os_det_i        ); \
    EXCO4M_INT      (exco4m_handle, exco4m, RX_IDLE_TEST_ERR_DET, rx_idle_test_err_det_i); \
    EXCO4M_INT      (exco4m_handle, exco4m, RX_PCS_INV_BLK_DET  , rx_pcs_inv_blk_det_i  ); \
    EXCO4M_INT      (exco4m_handle, exco4m, RX_PCS_ERR_BLK_DET  , rx_pcs_err_blk_det_i  ); \
    EXCO4M_INT      (exco4m_handle, exco4m, RX_PCS_HBER         , rx_pcs_hber_i         ); \
    EXCO4M_INT      (exco4m_handle, exco4m, RX_LANE_ALGN_MRK_ERR, rx_lane_algn_mrk_err_i); \
    EXCO4M_INT_ARRAY(exco4m_handle, exco4m, EXCO4M_N_LANES, TX_PCS_LOBL         , tx_pcs_lobl_i         ); \
    EXCO4M_INT_ARRAY(exco4m_handle, exco4m, EXCO4M_N_LANES, TX_PCS_BER_DET      , tx_pcs_ber_det_i      ); \
    EXCO4M_INT_ARRAY(exco4m_handle, exco4m, EXCO4M_N_LANES, TX_PCS_BIP_ERR_DET  , tx_pcs_bip_err_det_i  ); \
    EXCO4M_INT_ARRAY(exco4m_handle, exco4m, EXCO4M_N_LANES, TX_LOAM             , tx_loam_i             ); \
    EXCO4M_INT_ARRAY(exco4m_handle, exco4m, EXCO4M_N_LANES, TX_MRK_INV_DET      , tx_mrk_inv_det_i      ); \
    EXCO4M_INT_ARRAY(exco4m_handle, exco4m, EXCO4M_N_LANES, RX_PCS_BER_DET      , rx_pcs_ber_det_i      ); \
    EXCO4M_INT_ARRAY(exco4m_handle, exco4m, EXCO4M_N_LANES, RX_PCS_BIP_ERR_DET  , rx_pcs_bip_err_det_i  ); \
    EXCO4M_INT_ARRAY(exco4m_handle, exco4m, EXCO4M_N_LANES, RX_LOAM             , rx_loam_i             ); \
    EXCO4M_INT_ARRAY(exco4m_handle, exco4m, EXCO4M_N_LANES, RX_PCS_MRK_INV_DET  , rx_pcs_mrk_inv_det_i  ); \

#define EXCO4M_STATUS_TABLE_DEFINE() \
    EXCO4M_STATUS      (exco4m_handle, exco4m, TX_LOS_V     , tx_los_v     ); \
    EXCO4M_STATUS      (exco4m_handle, exco4m, TX_PCS_HBER_V, tx_pcs_hber_v); \
    EXCO4M_STATUS      (exco4m_handle, exco4m, RX_TSF_V     , rx_tsf_v     ); \
    EXCO4M_STATUS      (exco4m_handle, exco4m, RX_PCS_HBER_V, rx_pcs_hber_v); \
    EXCO4M_STATUS_ARRAY(exco4m_handle, exco4m, EXCO4M_N_LANES, TX_PCS_LOBL_V, tx_pcs_lobl_v); \
    EXCO4M_STATUS_ARRAY(exco4m_handle, exco4m, EXCO4M_N_LANES, TX_LOAM_V    , tx_loam_v    ); \
    EXCO4M_STATUS_ARRAY(exco4m_handle, exco4m, EXCO4M_N_LANES, RX_LOBL_V    , rx_lobl_v    ); \
    EXCO4M_STATUS_ARRAY(exco4m_handle, exco4m, EXCO4M_N_LANES, RX_LOAM_V    , rx_loam_v    ); \

#ifdef DIGI_FW_INTR_MON
/* Interrupts are in FW */
#else
/* EXCO4M_INT(handle, tsb_name, tsb_field, struct_field) */
#define EXCO4M_INT_TABLE_DEFINE_FW() \
    EXCO4M_INT_ARRAY(exco4m_handle, exco4m, EXCO4M_N_LANES, RX_LOBL             , rx_lobl_i             ); \

#endif

/*
** Structures and Unions
*/


/*******************************************************************************
* STRUCTURE: exco4m_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is the exco4m context 
*
* ELEMENTS:
*   base - base PMC handle 
*
*******************************************************************************/
typedef struct exco4m_handle_struct_t {
    pmc_handle_t base;
    
} exco4m_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC exco4m_handle_t *exco4m_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void exco4m_ctxt_destroy(exco4m_handle_t *exco4m_handle);
PUBLIC void exco4m_handle_init(exco4m_handle_t *exco4m_handle);
PUBLIC PMC_ERROR exco4m_handle_restart_init(exco4m_handle_t *exco4m_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                            pmc_energy_state_t energy_state);
PUBLIC UINT32 exco4m_base_address_get(exco4m_handle_t *exco4m_handle);

#ifndef DOXYGEN_PUBLIC_ONLY
PUBLIC PMC_ERROR exco4m_cfg(exco4m_handle_t *hndl, UINT32 link, exco4m_datapath_t datapath, BOOL8 enable, BOOL8 set_rx_pcs_push_ena);
PUBLIC PMC_ERROR exco4m_activate(exco4m_handle_t *hndl, UINT32 link);
PUBLIC PMC_ERROR exco4m_deactivate(exco4m_handle_t *hndl, UINT32 link);
PUBLIC PMC_ERROR exco4m_lowpwr    (exco4m_handle_t *hndl, BOOL8 value);
#endif /* DOXYGEN_PUBLIC_ONLY */

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */ 
/* LCOV_EXCL_START */
PUBLIC PMC_ERROR exco4m_status_get(exco4m_handle_t *hndl, BOOL8 clear_int, UINT32 *tx_status_ptr);
/* LCOV_EXCL_STOP */
#endif/*#ifndef DOXYGEN_PUBLIC_ONLY*/

PUBLIC PMC_ERROR exco4m_init(exco4m_handle_t *hndl, pmc_energy_state_t energy_state);
PUBLIC BOOL8 exco4m_start_state_test(exco4m_handle_t *hndl);
PUBLIC void exco4m_tx_lane_info_get(exco4m_handle_t *hndl,
                                    UINT8 txLaneId[20],
                                    UINT8 txLaneSkewFifoAddr[20]);
PUBLIC void exco4m_rx_lf_os_cfg(exco4m_handle_t *hndl,
                                BOOL8            enable);
PUBLIC void exco4m_tx_lf_os_cfg(exco4m_handle_t *hndl,
                                BOOL8            enable);
PUBLIC void exco4m_rx_rf_os_cfg(exco4m_handle_t *hndl,
                                BOOL8            enable);
PUBLIC void exco4m_tx_rf_os_cfg(exco4m_handle_t *hndl,
                                BOOL8            enable);
PUBLIC void exco4m_rx_lf_os_cfg_get(exco4m_handle_t *hndl,
                                    BOOL8 *enable);
PUBLIC void exco4m_tx_lf_os_cfg_get(exco4m_handle_t *hndl,
                                    BOOL8 *enable);
PUBLIC void exco4m_rx_rf_os_cfg_get(exco4m_handle_t *hndl,
                                    BOOL8 *enable);
PUBLIC void exco4m_tx_rf_os_cfg_get(exco4m_handle_t *hndl,
                                    BOOL8 *enable);


PUBLIC PMC_POLL_RETURN_TYPE exco4m_tx_align_poll(exco4m_handle_t *hndl);

PUBLIC PMC_ERROR exco4m_int_chnl_enable(exco4m_handle_t    *exco4m_handle,
                                        UINT32              link,
                                        exco4m_int_chnl_t  *int_table_ptr,
                                        exco4m_int_chnl_t  *int_en_table_ptr,
                                        BOOL8                enable);
PUBLIC PMC_ERROR exco4m_int_chnl_clear(exco4m_handle_t    *exco4m_handle,
                                       UINT32              link,
                                       exco4m_int_chnl_t  *int_table_ptr,
                                       exco4m_int_chnl_t  *int_en_table_ptr);
PUBLIC PMC_ERROR exco4m_int_chnl_retrieve(exco4m_handle_t    *exco4m_handle,
                                          UINT32              link,
                                          exco4m_int_chnl_t  *filt_table_ptr,
                                          exco4m_int_chnl_t  *int_table_ptr);
PUBLIC PMC_ERROR exco4m_int_chnl_enabled_check(exco4m_handle_t     *exco4m_handle,
                                               UINT32               link,
                                               exco4m_int_chnl_t   *int_en_table_ptr,
                                               BOOL8                *int_found_ptr);


#ifdef __cplusplus
}
#endif

#endif /* _EXCO4M_H */

/*
** end of file
*/
