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

#ifndef _GSUP43_C73_H
#define _GSUP43_C73_H

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
#include "gsup43_c73_api.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: gsup43_c73_int_type_enum_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  GSUP43 C73 interrupts
*
* ELEMENTS:
*  See typedef 
*
*******************************************************************************/
typedef enum 
{
    /* Rx Interrupts */
    GSUP43_C73_LOS                 = 0x00,
    GSUP43_C73_LOS_SYNC            = 0x01, 
    GSUP43_C73_HI_BER              = 0x02,
    GSUP43_C73_LINK_FAIL           = 0x03,
    GSUP43_C73_RX_FAULT            = 0x04,
    GSUP43_C73_BLK_TYPE_ERR        = 0x05, 
    GSUP43_C73_RX_PRBS31_ERR       = 0x06, 
    GSUP43_C73_ALL_ONES_OR_ZEROS   = 0x07,
    GSUP43_C73_RX_RF               = 0x0E,
    GSUP43_C73_RX_LF               = 0x0F,
    /* Tx Interrupts */
    GSUP43_C73_FIFO_UNDRUN         = 0x08,
    GSUP43_C73_FIFO_OVRUN          = 0x09,
    GSUP43_C73_SEND_RF             = 0x0A,
    GSUP43_C73_SEND_LF             = 0x0B,
    GSUP43_C73_SEND_IDLE           = 0x0C,
    GSUP43_C73_TYPE_ERR            = 0x0D
} gsup43_c73_int_type_enum_t;


/*
** Constants
*/
#define GSUP43_C73_NUM_LINKS 1

/*
** Macro Definitions
*/
/* GSUP43_INT_SINGLE(handle, tsb_name, tsb_field, struct_field) */
#define GSUP43_INT_TABLE_DEFINE() \
    GSUP43_INT_SINGLE_IC(gsup43_c73_handle, gsup43_c73, LOS                , los_i                ); \
    GSUP43_INT_SINGLE_IC(gsup43_c73_handle, gsup43_c73, LOS_SYNC           , los_sync_i           ); \
    GSUP43_INT_SINGLE_IC(gsup43_c73_handle, gsup43_c73, HI_BER             , hi_ber_i             ); \
    GSUP43_INT_SINGLE_IC(gsup43_c73_handle, gsup43_c73, LINK_FAIL          , link_fail_i          ); \
    GSUP43_INT_SINGLE_IC(gsup43_c73_handle, gsup43_c73, RX_FAULT           , rx_fault_i           ); \
    GSUP43_INT_SINGLE   (gsup43_c73_handle, gsup43_c73, BLK_TYPE_ERR       , blk_type_err_i       ); \
    GSUP43_INT_SINGLE   (gsup43_c73_handle, gsup43_c73, RX_PRBS31_ERR      , rx_prbs31_err_i      ); \
    GSUP43_INT_SINGLE_IC(gsup43_c73_handle, gsup43_c73, ALL_ONES_OR_ZEROS  , all_ones_or_zeros_i  ); \
    GSUP43_INT_SINGLE_IC(gsup43_c73_handle, gsup43_c73, RX_RF              , rx_rf_i              ); \
    GSUP43_INT_SINGLE_IC(gsup43_c73_handle, gsup43_c73, RX_LF              , rx_lf_i              ); \
    GSUP43_INT_SINGLE   (gsup43_c73_handle, gsup43_c73, FIFO_UNDRUN        , fifo_undrun_i        ); \
    GSUP43_INT_SINGLE   (gsup43_c73_handle, gsup43_c73, FIFO_OVRUN         , fifo_ovrun_i         ); \
    GSUP43_INT_SINGLE   (gsup43_c73_handle, gsup43_c73, SEND_RF            , send_rf_i            ); \
    GSUP43_INT_SINGLE   (gsup43_c73_handle, gsup43_c73, SEND_LF            , send_lf_i            ); \
    GSUP43_INT_SINGLE   (gsup43_c73_handle, gsup43_c73, SEND_IDLE          , send_idle_i          ); \
    GSUP43_INT_SINGLE   (gsup43_c73_handle, gsup43_c73, TYPE_ERR           , type_err_i           ); \

#define GSUP43_STATUS_TABLE_DEFINE() \
    GSUP43_STATUS_SINGLE(gsup43_c73_handle, gsup43_c73, LOS_V                , los_v                ); \
    GSUP43_STATUS_SINGLE(gsup43_c73_handle, gsup43_c73, LOS_SYNC_V           , los_sync_v           ); \
    GSUP43_STATUS_SINGLE(gsup43_c73_handle, gsup43_c73, HI_BER_V             , hi_ber_v             ); \
    GSUP43_STATUS_SINGLE(gsup43_c73_handle, gsup43_c73, LINK_FAIL_V          , link_fail_v          ); \
    GSUP43_STATUS_SINGLE(gsup43_c73_handle, gsup43_c73, RX_FAULT_V           , rx_fault_v           ); \
    GSUP43_STATUS_SINGLE(gsup43_c73_handle, gsup43_c73, ALL_ONES_OR_ZEROS_V  , all_ones_or_zeros_v  ); \
    GSUP43_STATUS_SINGLE(gsup43_c73_handle, gsup43_c73, RX_RF_V              , rx_rf_v              ); \
    GSUP43_STATUS_SINGLE(gsup43_c73_handle, gsup43_c73, RX_LF_V              , rx_lf_v              ); \
    GSUP43_STATUS_SINGLE(gsup43_c73_handle, gsup43_c73, LF_FAULT             , lf_fault_v           ); \
    GSUP43_STATUS_SINGLE(gsup43_c73_handle, gsup43_c73, RF_FAULT             , rf_fault_v           ); \
    GSUP43_STATUS_SINGLE(gsup43_c73_handle, gsup43_c73, SEND_RF_V            , send_rf_v            ); \
    GSUP43_STATUS_SINGLE(gsup43_c73_handle, gsup43_c73, SEND_LF_V            , send_lf_v            ); \
    GSUP43_STATUS_SINGLE(gsup43_c73_handle, gsup43_c73, SEND_IDLE_V          , send_idle_v          ); \


/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: gsup43_c73_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This is the config struct for initial values.
*
* ELEMENTS:
*  num_gsup43_c73_chans    - number of channel
*
*******************************************************************************/
typedef struct {    
    UINT32 num_gsup43_c73_chans;    
} gsup43_c73_cfg_t;


/*******************************************************************************
* STRUCTURE: gsup43_c73_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is to illustrate the dynamically changing context.
*
* ELEMENTS:
*   tx_enable - TX enabled
*   rx_enable - RX enabled
*
*******************************************************************************/
typedef struct {
    BOOL tx_enable;
    BOOL rx_enable;     
} gsup43_c73_var_t;

/*******************************************************************************
* STRUCTURE: gsup43_c73_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is the gsup43_c73 context 
*
* ELEMENTS:
*   base - base PMC handle 
*   cfg  - static config
*   var  - dynamic context 
*
*******************************************************************************/
typedef struct gsup43_c73_handle_struct_t {
    pmc_handle_t base;
    
    gsup43_c73_cfg_t cfg;
    gsup43_c73_var_t var;

} gsup43_c73_handle_struct_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC gsup43_c73_handle_t *gsup43_c73_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void gsup43_c73_ctxt_destroy(gsup43_c73_handle_t *gsup43_c73_handle);
PUBLIC void gsup43_c73_handle_init(gsup43_c73_handle_t *gsup43_c73_handle);
PUBLIC PMC_ERROR gsup43_c73_handle_restart_init(gsup43_c73_handle_t *gsup43_c73_handle,
                                                util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                pmc_energy_state_t energy_state);
PUBLIC UINT32 gsup43_c73_base_address_get(gsup43_c73_handle_t *gsup43_c73_handle);

PUBLIC PMC_ERROR gsup43_c73_init(gsup43_c73_handle_t *hndl, pmc_energy_state_t energy_state);
PUBLIC void gsup43_c73_var_default_init(gsup43_c73_handle_t *hndl);
PUBLIC BOOL gsup43_c73_start_state_test(gsup43_c73_handle_t *hndl);
PUBLIC void gsup43_c73_energy_state_reg_set( gsup43_c73_handle_t *hndl, pmc_energy_state_t energy_state, BOOL rx, BOOL tx);

PUBLIC PMC_ERROR gsup43_c73_rx_en(gsup43_c73_handle_t *hndl, BOOL enable);
PUBLIC PMC_ERROR gsup43_c73_tx_en(gsup43_c73_handle_t *hndl, BOOL enable);
PUBLIC PMC_ERROR gsup43_c73_tx_prbs_en(gsup43_c73_handle_t *hndl, BOOL enable);

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */ 
/* LCOV_EXCL_START */
PUBLIC PMC_ERROR gsup43_c73_intr_enable_set(gsup43_c73_handle_t *hndl, 
                                         gsup43_c73_int_type_enum_t int_type, BOOL enable);
PUBLIC PMC_ERROR gsup43_c73_status_get(gsup43_c73_handle_t *hndl, UINT32 *rx_status_ptr, UINT32 *tx_status_ptr);
PUBLIC PMC_ERROR gsup43_c73_global_status_get(gsup43_c73_handle_t *hndl, BOOL clear_int, UINT32 *status_ptr);
/* LCOV_EXCL_STOP */
#endif/*#ifndef DOXYGEN_PUBLIC_ONLY*/
PUBLIC PMC_ERROR gsup43_c73_cnt_get(gsup43_c73_handle_t *hndl, BOOL update_cnt, UINT32 count_type,
                                    UINT32 *count_val_ptr);
PUBLIC PMC_ERROR gsup43_c73_rx_cfg(gsup43_c73_handle_t *hndl,
                                   UINT32 losInv, UINT32 dscrdZone, UINT32 sigEn,
                                   UINT32 seqEn, UINT32 allDscrd, UINT32 osPrtDis,
                                   UINT32 minOsPrt, UINT32 pktType, UINT32 osType,
                                   UINT32 rx_125us_cnt);

PUBLIC PMC_ERROR gsup43_c73_tx_cfg(gsup43_c73_handle_t *hndl,
                                   UINT32 errDrop, UINT32 pktType, UINT32 osType,
                                   UINT32 forceMinIpg, UINT32 unidirEn,
                                   UINT32 sendIdleEn, UINT32 sendLfEn,
                                   UINT32 sendRfEn, UINT32 fifoHmark);

PUBLIC PMC_POLL_RETURN_TYPE gsup43_c73_los_sync_poll(gsup43_c73_handle_t *hndl);


PUBLIC PMC_ERROR gsup43_c73_tx_upi_set(gsup43_c73_handle_t *hndl,                               
                                       UINT8                pkt_upi_val,
                                       UINT8                os_upi_val,
                                       BOOL8                enable_drop);

PUBLIC PMC_ERROR gsup43_c73_tx_upi_get(gsup43_c73_handle_t *hndl,                               
                                       UINT8               *pkt_upi_val_ptr,
                                       UINT8               *os_upi_val_ptr,
                                       BOOL8               *enable_drop_ptr);
PUBLIC PMC_ERROR gsup43_c73_rx_upi_set(gsup43_c73_handle_t *hndl,                               
                                       UINT8                pkt_upi_val,
                                       UINT8                os_upi_val);

PUBLIC PMC_ERROR gsup43_c73_rx_upi_get(gsup43_c73_handle_t *hndl,                               
                                       UINT8               *pkt_upi_val_ptr,
                                       UINT8               *os_upi_val_ptr);

PUBLIC PMC_ERROR gsup43_c73_int_chnl_enable(gsup43_c73_handle_t    *gsup43_c73_handle,
                                            UINT32                  link,
                                            gsup43_c73_int_chnl_t  *int_table_ptr,
                                            gsup43_c73_int_chnl_t  *int_en_table_ptr,
                                            BOOL8                   enable);
PUBLIC PMC_ERROR gsup43_c73_int_chnl_clear(gsup43_c73_handle_t    *gsup43_c73_handle,
                                           UINT32                  link,
                                           gsup43_c73_int_chnl_t  *int_table_ptr,
                                           gsup43_c73_int_chnl_t  *int_en_table_ptr);
PUBLIC PMC_ERROR gsup43_c73_int_chnl_retrieve(gsup43_c73_handle_t    *gsup43_c73_handle,
                                              UINT32                  link,
                                              gsup43_c73_int_chnl_t  *filt_table_ptr,
                                              gsup43_c73_int_chnl_t  *int_table_ptr);
PUBLIC PMC_ERROR gsup43_c73_int_chnl_enabled_check(gsup43_c73_handle_t     *gsup43_c73_handle,
                                                   UINT32                   link,
                                                   gsup43_c73_int_chnl_t   *int_en_table_ptr,
                                                   BOOL8                   *int_found_ptr);

PUBLIC void gsup43_c73_pcs_decoder_en(gsup43_c73_handle_t *hndl, BOOL enable);      
       
PUBLIC PMC_ERROR gsup43_c73_TX_SW_RST_set(gsup43_c73_handle_t *h_ptr,
										  UINT32 value );      
#ifdef __cplusplus
}
#endif

#endif /* _GSUP43_C73_H */

/*
** end of file
*/
