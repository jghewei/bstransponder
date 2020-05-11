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

#ifndef _EMAC_H
#define _EMAC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "emac_api.h"
#include "etime.h"
#include "etime_api.h"
#include "util_global.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: emac_data_rate_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Ethernet line rates.  The
*   element terms have a syntax:
*      EMAC_LINK_<rate>
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum 
{
    EMAC_LINK_10G = 0x0,
    EMAC_LINK_40G = 0x1,
    EMAC_LINK_100G = 0x2,
    LAST_EMAC_DATA_RATE
} emac_data_rate_t;

/*******************************************************************************
* ENUM: emac_data_dir_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Data direction
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum 
{
    EMAC_DIR_BOTH = 0x0,
    EMAC_DIR_ING  = 0x1,
    EMAC_DIR_EGR  = 0x2
} emac_data_dir_t;

/*******************************************************************************
* ENUM: emac_rx_data_src_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Data source
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum
{
    EMAC_RX_SRC_RX_SERDES = 0x0,
    EMAC_RX_SRC_TX_EMAC_PMA = 0x1,
    EMAC_RX_SRC_TX_EPMM = 0x2,
    EMAC_RX_SRC_RX_ETRANS = 0x3,
    EMAC_RX_SRC_UNKNOWN
} emac_rx_data_src_t;


/*******************************************************************************
* ENUM: emac_tx_data_dst_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for TX destination
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum
{
    EMAC_TX_DST_TX_PMA = 0x0,
    EMAC_TX_DST_TX_ETRANS = 0x1,
    EMAC_TX_DST_TX_EPMM = 0x2,
    EMAC_TX_DST_RESERVED_1 = 0x3,
    EMAC_TX_DST_UNKNOWN
} emac_tx_data_dst_t;


/*******************************************************************************
* ENUM: emac_pause_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Pause mode
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum 
{
    EMAC_LEGACY_PAUSE = 0x0,
    EMAC_PFC_PAUSE = 0x1
} emac_pause_mode_t;

/*******************************************************************************
* ENUM: emac_pcs_seed_sel_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for PCS Test Seed
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum 
{
    EMAC_PCS_TEST_SEED_A = 0x0,
    EMAC_PCS_TEST_SEED_B = 0x1
} emac_pcs_seed_sel_t;


/*******************************************************************************
* ENUM: emac_prbs_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for PRBS Pattern
*
* ELEMENTS:
*
*******************************************************************************/
/* PRBS Pattern */
typedef enum 
{
    EMAC_PRBS9 = 0x0,
    EMAC_PRBS31 = 0x1,
    LAST_EMAC_PRBS_TYPE
} emac_prbs_type_t;

/*******************************************************************************
* ENUM: emac_intr_status_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Interrupt Status type
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum
{
  EMAC_NO_INTR      = 0x0,
  EMAC_PHY_LOS      = 0x1,
  EMAC_RX_REM_FAULT = 0x2,
  EMAC_RX_LOC_FAULT = 0x3,
  EMAC_HI_BER       = 0x4,
  EMAC_BLOCK_LOCK   = 0x5,
  EMAC_ALIGN_STATUS = 0x6,
  EMAC_TX_OVR       = 0x7,
  EMAC_ING_AGB_OVR  = 0x8,
  EMAC_EGR_AGB_OVR  = 0x9,
  EMAC_SB_ECC_ERR   = 0xA,
  EMAC_MB_ECC_ERR   = 0xB,
  EMAC_REG_TIMEOUT  = 0xC
} emac_intr_status_t;


/*******************************************************************************
* ENUM: emac_intr_value_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure for enumerated type for Interrupt Value type
*
* ELEMENTS:
*
*******************************************************************************/
typedef enum
{
  EMAC_NO_VALUE  = 0x0,
  EMAC_INTR_RISE = 0x1,
  EMAC_INTR_FALL = 0x2
} emac_intr_value_t;


/*******************************************************************************
* ENUM: emac_loopback_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Enumerated type to specify the emac loopback type
*
* ELEMENTS:
*   EMAC_DISABLE_LOOPBACK     - Disable all loopbacks
*   EMAC_MLD_LOOPBACK         - EMAC loopback at MLD
*           
*
*******************************************************************************/ 
typedef enum
{
  EMAC_DISABLE_LOOPBACK  = 0x0,
  EMAC_MLD_LOOPBACK
} emac_loopback_cfg_t;

/*
** Constants
*/
#define BASE_ADDR_EMAC_ETIME                            0x00004000

#define EMAC_NUM_LINKS                                  12

#define EMAC_CORE_TDM_CREDIT_VAL                        2

#define MAC_REG_CTRL_AND_CFG_BIT_WAN_MODE_DFLT          0
#define MAC_REG_CTRL_AND_CFG_BIT_PROMIS_EN_DFLT         1
#define MAC_REG_CTRL_AND_CFG_BIT_TX_ADDR_INS_DFLT       0
#define MAC_REG_CTRL_AND_CFG_BIT_TX_PAD_EN_DFLT         1
#define MAC_REG_CTRL_AND_CFG_BIT_CMD_FRAME_EN_DFLT      1
#define MAC_REG_CTRL_AND_CFG_BIT_NO_LGTH_CHECK_DFLT     1

#define EMAC_DEFAULT_PAUSE_MODE                         EMAC_LEGACY_PAUSE
#define EMAC_DEFAULT_PAUSE_PFC_COMP                     TRUE 
#define EMAC_DEFAULT_PAUSE_FWD                          FALSE
#define EMAC_DEFAULT_PAUSE_IGNORE                       FALSE

#define EMAC_MODE_CTL_100G_NB_CFG_LINKS                 EMAC_NUM_LINKS
#define EMAC_MODE_CTL_40G_NB_CFG_LINKS                  4
#define EMAC_MODE_CTL_10G_NB_CFG_LINKS                  4

#define EMAC_100G_NB_PMA_LANES                          10
#define EMAC_40G_NB_PMA_LANES                           4
#define EMAC_10G_NB_PMA_LANES                           1

#define EMAC_FRM_LENGTH_DEFAULT                         10240

#define EMAC_RX_CREDIT_10G                              8
#define EMAC_RX_CREDIT_40G                              7
#define EMAC_RX_CREDIT_100G                             2
       
/* Valid links */
#define EMAC_40G_LINK_0                                 0
#define EMAC_40G_LINK_4                                 4
#define EMAC_40G_LINK_8                                 8
#define EMAC_100G_LINK_0                                0

/* PMON Stats */
#define EMAC_MAC_STATS_SIZE                             106
#define EMAC_PCS_STATS_SIZE                             26

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: emac_prbs_ctxt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is to illustrate the EMAC PRBS state
*
*******************************************************************************/
typedef struct emac_prbs_ctxt_t
{
    BOOL8              enable;
} emac_prbs_ctxt_t;

/*******************************************************************************
* STRUCTURE: emac_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This is the config struct for initial values.
*
*******************************************************************************/
typedef struct {
/*    
    UINT32 num_emac_chans;
*/    
} emac_cfg_t;

/*******************************************************************************
* STRUCTURE: emac_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is to illustrate the dynamically changing context.
*
*******************************************************************************/
typedef struct {   
    BOOL8 rx_enable[EMAC_NUM_LINKS];   
    BOOL8 tx_enable[EMAC_NUM_LINKS];   
    emac_data_rate_t data_rate[EMAC_NUM_LINKS];
    emac_rx_data_src_t rx_data_src[EMAC_NUM_LINKS];
    emac_loopback_cfg_t loopback[EMAC_NUM_LINKS];
    emac_prbs_ctxt_t **prbs_ctxt;   
 
} emac_var_t;

/*******************************************************************************
* STRUCTURE: emac_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This struct is the etime context 
*
* ELEMENTS:
*   base - base PMC handle 
*   cfg  - static config
*   var  - dynamic context 
*   etime_handle - ETIME handle 
*******************************************************************************/
typedef struct emac_handle_struct_t {
    pmc_handle_t base;
    
    emac_cfg_t cfg;
    emac_var_t var;

    etime_handle_t *etime_handle;

} emac_handle_struct_t;

typedef struct emac_handle_struct_t emac_handle_t; /*!< Declare emac handle */

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC emac_handle_t *emac_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void emac_ctxt_destroy(emac_handle_t *emac_handle);
PUBLIC void emac_handle_init(emac_handle_t *emac_handle);
PUBLIC PMC_ERROR emac_handle_restart_init(emac_handle_t *emac_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                          pmc_energy_state_t energy_state);
PUBLIC PMC_ERROR emac_init(emac_handle_t *hndl, pmc_energy_state_t energy_state);
PUBLIC BOOL8 emac_start_state_test(emac_handle_t *hndl);
PUBLIC UINT32 emac_base_address_get(emac_handle_t *emac_handle);
PUBLIC PMC_ERROR emac_mac_cfg(emac_handle_t *hndl, UINT32 link, emac_data_rate_t data_rate,
                               emac_rx_data_src_t rx_data_src,
                               emac_tx_data_dst_t tx_data_dst);
PUBLIC PMC_ERROR emac_mac_uncfg(emac_handle_t *hndl, UINT32 link, emac_data_rate_t data_rate);
PUBLIC PMC_ERROR emac_src_dst_cfg(emac_handle_t *hndl, UINT32 link, emac_data_rate_t data_rate,
                                  emac_rx_data_src_t rx_data_src,
                                  emac_tx_data_dst_t tx_data_dst);
PUBLIC PMC_ERROR emac_tx_enable(emac_handle_t *hndl, UINT32 link, BOOL8 enable);
PUBLIC BOOL8 emac_tx_is_enabled(emac_handle_t *hndl, UINT32 link);
PUBLIC PMC_ERROR emac_rx_enable(emac_handle_t *hndl, UINT32 link, BOOL8 enable);

PUBLIC PMC_ERROR emac_link_lowpwr_cfg(emac_handle_t *hndl, UINT32 link, emac_data_rate_t data_rate, emac_data_dir_t data_direction, BOOL8 lowpwr);
PUBLIC PMC_ERROR emac_link_reset_cfg(emac_handle_t *hndl, UINT32 link, emac_data_dir_t data_direction, BOOL8 reset);
PUBLIC PMC_ERROR emac_lf_rf_cfg(emac_handle_t *hndl, UINT32 link, BOOL8 auto_fault_handling, 
                                BOOL8 send_lf, BOOL8 send_rf, BOOL8 halt_traffic_lf_rf);
PUBLIC PMC_ERROR emac_fault_mode_cfg(emac_handle_t *hndl, 
                                     UINT32 link, 
                                     BOOL8 flt_hdl_dis, 
                                     BOOL8 send_lf, 
                                     BOOL8 send_rf,
                                     BOOL8 etrans_fault_on);
PUBLIC PMC_ERROR emac_fault_mode_cfg_get(emac_handle_t *hndl, 
                                         UINT32 link, 
                                         BOOL8 *flt_hdl_dis_ptr, 
                                         BOOL8 *send_lf_ptr, 
                                         BOOL8 *send_rf_ptr);
PUBLIC PMC_ERROR emac_auto_lf_rf_cfg_get(emac_handle_t *hndl, 
                                         UINT32 link, 
                                         BOOL8 *auto_fault_handling);
PUBLIC PMC_ERROR emac_auto_lf_rf_cfg(emac_handle_t *hndl, 
                                     UINT32 link, 
                                     BOOL8 auto_fault_handling);
PUBLIC PMC_ERROR emac_idle_cfg(emac_handle_t *hndl, UINT32 link, BOOL8 send_idle);
PUBLIC BOOL8 emac_crc_fwd_is_enabled(emac_handle_t *hndl, UINT32 link);
PUBLIC PMC_ERROR emac_crc_fwd_en(emac_handle_t *hndl, UINT32 link, BOOL8 enable);
PUBLIC void emac_no_length_check_en(emac_handle_t *hndl, 
                                 UINT32 link, 
                                 BOOL8 enable);
PUBLIC PMC_ERROR emac_flow_ctrl_mode_cfg(emac_handle_t *hndl, UINT32 link, emac_pause_mode_t pause_mode,
                                         BOOL8 pause_pfc_comp, BOOL8 pause_fwd, BOOL8 pause_ignore);
PUBLIC PMC_ERROR emac_flow_ctrl_mode_get(emac_handle_t *hndl, UINT32 link, BOOL8 *enable);
PUBLIC PMC_ERROR emac_pause_quanta_cfg(emac_handle_t *hndl, UINT32 link, UINT32 prio_class, 
                                       UINT32 quanta_val, UINT32 quanta_thres);
PUBLIC PMC_ERROR emac_pause_quanta_get(emac_handle_t *hndl, UINT32 link, UINT32 prio_class, 
                                       UINT32 *quanta_val, UINT32 *quanta_thres);
PUBLIC PMC_ERROR emac_max_frm_len_cfg(emac_handle_t *hndl, UINT32 link, UINT32 max_frm_length);
PUBLIC PMC_ERROR emac_tx_ipg_cfg(emac_handle_t *hndl, UINT32 link, UINT32 tx_ipg_val);
PUBLIC void emac_ins_mac_addr_set(emac_handle_t *hndl,
                                  UINT32 link,
                                  BOOL8 tx_addr_ins_en,
                                  UINT32 mac_addr_msb,
                                  UINT32 mac_addr_lsb);
PUBLIC void emac_ins_mac_addr_get(emac_handle_t *hndl,
                                  UINT32 link,
                                  BOOL8 *tx_addr_ins_en,
                                  UINT32 *mac_addr_msb,
                                  UINT32 *mac_addr_lsb);
PUBLIC PMC_ERROR emac_pma_rx_to_tx_lpbk_en(emac_handle_t *hndl, UINT32 link, BOOL8 enable);
PUBLIC PMC_ERROR emac_pma_tx_to_rx_lpbk_en(emac_handle_t *hndl, UINT32 link, BOOL8 enable);
PUBLIC PMC_ERROR emac_pcs_lpbk_en(emac_handle_t *hndl, UINT32 link, BOOL8 enable);
PUBLIC PMC_ERROR emac_pcs_test_patt_cfg(emac_handle_t *hndl, UINT32 link, BOOL8 square_wave, BOOL8 zero_pattern,
                                        BOOL8 tx_en, BOOL8 rx_en);
PUBLIC PMC_ERROR emac_pcs_10g_test_patt_seed_cfg(emac_handle_t *hndl, UINT32 link, 
                                                 emac_pcs_seed_sel_t seed_sel, UINT32 seed_msb, UINT32 seed_lsb);
PUBLIC PMC_ERROR emac_mon_prbs_resync(emac_handle_t   *hndl, 
                                      UINT32           link);

PUBLIC PMC_ERROR emac_prbs_cfg(emac_handle_t     *hndl, 
                               UINT32             link, 
                               emac_data_dir_t    data_direction,
                               util_patt_mode_t   prbs_type, 
                               BOOL8               enable);

PUBLIC PMC_ERROR emac_prbs_get(emac_handle_t   *hndl, 
                               UINT32           link,  
                               emac_data_dir_t  data_direction,
                               util_patt_mode_t *prbs_type,
                               BOOL8            *enable);

PUBLIC PMC_ERROR emac_prbs_error_cnt_get(emac_handle_t         *hndl, 
                                         UINT32                 link, 
                                         util_patt_mode_t       prbs_type,
                                         UINT32                *err_cnt);

PUBLIC PMC_ERROR emac_int_chnl_enable(emac_handle_t    *emac_handle,
                                      UINT32            link,
                                      emac_int_chnl_t  *int_table_ptr,
                                      emac_int_chnl_t  *int_en_table_ptr,
                                      BOOL8             enable,
                                      BOOL8             is_multichannels);
PUBLIC PMC_ERROR emac_int_chnl_clear(emac_handle_t    *emac_handle,
                                     UINT32            link,
                                     emac_int_chnl_t  *int_table_ptr,
                                     emac_int_chnl_t  *int_en_table_ptr,
                                     BOOL8             is_multichannels);
PUBLIC PMC_ERROR emac_int_chnl_retrieve(emac_handle_t    *emac_handle,
                                        UINT32            link,
                                        emac_int_chnl_t  *filt_table_ptr,
                                        emac_int_chnl_t  *int_table_ptr);
PUBLIC PMC_ERROR emac_int_chnl_enabled_check(emac_handle_t     *emac_handle,
                                             UINT32             link,
                                             emac_int_chnl_t   *int_en_table_ptr,
                                             BOOL8             *int_found_ptr,
                                             BOOL8              is_multichannels);

/* Status */
PUBLIC PMC_ERROR emac_intr_enable_set(emac_handle_t *hndl, 
                                      UINT32 link, 
                                      emac_intr_status_t status_type, 
                                      BOOL enable);

PUBLIC PMC_ERROR emac_pause_stat_get(emac_handle_t *hndl, UINT32 link, UINT32 *pause_stat_ptr);
PUBLIC PMC_ERROR emac_tx_enable_get(emac_handle_t *hndl, UINT32 link, BOOL8 *enable_ptr);
PUBLIC PMC_ERROR emac_max_frm_len_get(emac_handle_t *hndl, UINT32 link, UINT32 *length_ptr);
PUBLIC PMC_ERROR emac_tx_ipg_get(emac_handle_t *hndl, UINT32 link, UINT32 *gap_ptr);


PUBLIC PMC_ERROR emac_status_get(emac_handle_t  *hndl, 
                                 UINT32          link, 
                                 BOOL            clear_int, 
                                 UINT32         *status_ptr);

PUBLIC PMC_ERROR emac_prbs_status_get(emac_handle_t         *hndl, 
                                      UINT32                 link,
                                      util_patt_status_t    *status);
/* Counters */
PUBLIC PMC_ERROR emac_count_get(emac_handle_t   *hndl, 
                                UINT32           link, 
                                UINT32           count_type,
                                UINT32          *count_val_ptr);

PUBLIC PMC_ERROR emac_ber_bip_err_cnt_get(emac_handle_t  *hndl, 
                                          UINT32          link, 
                                          UINT32         *ber_cnt_ptr, 
                                          UINT32         *lane_bip_err_cnt_ptr);

PUBLIC PMC_ERROR emac_block_err_cnt_get(emac_handle_t *hndl, 
                                        UINT32         link, 
                                        UINT32        *block_err_cnt_msb_ptr, 
                                        UINT32        *block_err_cnt_lsb_ptr);

PUBLIC PMC_ERROR emac_pcs_test_cnt_get(emac_handle_t *hndl, UINT32 link, UINT32 *test_err_cnt);

PUBLIC PMC_ERROR emac_pver_vl_intvl_cfg(emac_handle_t *hndl, BOOL8 enable);

PUBLIC PMC_ERROR emac_pcs_stats_cnt_get      (emac_handle_t    *hndl,
                                              UINT32           link,
                                              emac_data_rate_t data_rate,
                                              BOOL8             pmon_enable, 
                                              UINT32           count_val_ptr[EMAC_PCS_STATS_SIZE]);

PUBLIC PMC_ERROR emac_mac_stats_cnt_get      (emac_handle_t    *hndl,
                                              UINT32           link,
                                              emac_data_rate_t data_rate,
                                              UINT32           count_val_ptr[EMAC_MAC_STATS_SIZE]);
/* Interrupt Enable */
PUBLIC PMC_ERROR emac_global_intr_enable_set (emac_handle_t    *hndl, 
                                              BOOL8             enable);

PUBLIC PMC_ERROR emac_ecc_intr_enable_set    (emac_handle_t    *hndl,
                                              BOOL8             enable);

PUBLIC PMC_ERROR emac_link_intr_enable_set   (emac_handle_t    *hndl, 
                                              UINT32            link, 
                                              emac_data_rate_t  data_rate,
                                              BOOL8             enable);

PUBLIC PMC_ERROR emac_egr_data_squelching_cfg (emac_handle_t     *hndl, 
                                               UINT32             link, 
                                               emac_data_rate_t   data_rate, 
                                               BOOL8              enable,
                                               UINT32             pattern);

PUBLIC PMC_ERROR emac_ingr_data_sink_cfg (emac_handle_t    *hndl, 
                                          UINT32            link, 
                                          emac_data_rate_t  data_rate, 
                                          BOOL8             enable);

PUBLIC PMC_ERROR emac_ts_mask_cfg (emac_handle_t    *hndl, 
                                   UINT32            link, 
                                   emac_data_rate_t  data_rate,
                                   UINT32            mask_start,
                                   UINT32            mask_end,
                                   BOOL8             enable);

PUBLIC PMC_ERROR emac_ingr_dpi_err_mask_get(emac_handle_t    *hndl, 
                                            UINT32           link, 
                                            emac_data_rate_t data_rate,
                                            BOOL8            *fcs_mask_ptr,
                                            BOOL8            *len_mask_ptr);

PUBLIC void emac_errored_frames_drop_cfg(emac_handle_t       *hndl,
                                         emac_error_t         error_type,
                                         BOOL8                enable);

PUBLIC void emac_errored_frames_dpi_error_cfg(emac_handle_t *hndl,
                                              emac_error_t error_type,
                                              BOOL8 enable);

PUBLIC PMC_ERROR emac_egr_mac_link_reset (emac_handle_t    *hndl, 
                                          UINT32            link,
                                          emac_data_rate_t  data_rate);

PUBLIC PMC_ERROR emac_etime_ts_lowpwr_set (emac_handle_t *hndl);

PUBLIC PMC_ERROR emac_sw_reset_core_set   (emac_handle_t *hndl);

PUBLIC PMC_ERROR etime_sw_reset_core_set  (emac_handle_t *hndl);

PUBLIC PMC_ERROR emac_sw_reset_nregs_set  (emac_handle_t *hndl);

PUBLIC PMC_ERROR etime_sw_reset_nregs_set (emac_handle_t *hndl);

PUBLIC void emac_energy_state_reg_set ( emac_handle_t *hndl, pmc_energy_state_t energy_state);

PUBLIC void emac_block_non_default_cfg(emac_handle_t *hndl);

PUBLIC void emac_var_default_init(emac_handle_t *hndl);

PUBLIC PMC_ERROR emac_loopback_cfg(emac_handle_t       *hndl, 
                                   UINT32               link, 
                                   emac_loopback_cfg_t  loopback); 

PUBLIC PMC_ERROR emac_loopback_status_get(emac_handle_t        *hndl,
                                          UINT32                link,
                                          emac_loopback_cfg_t  *loopback_ptr);

PUBLIC void emac_errored_frames_drop_cfg(emac_handle_t       *hndl,
                                         emac_error_t         error_type,
                                         BOOL8                enable);

PUBLIC BOOL8 emac_errored_frames_drop_get(emac_handle_t       *hndl,
                                          emac_error_t         error_type);

PUBLIC void emac_errored_frames_dpi_error_cfg(emac_handle_t *hndl,
                                              emac_error_t error_type,
                                              BOOL8 enable);

BOOL8 emac_errored_frames_dpi_error_get(emac_handle_t *hndl,
                                        emac_error_t error_type);

PUBLIC PMC_ERROR emac_ingr_dpi_err_mask_cfg(emac_handle_t    *hndl, 
                                            UINT32           link, 
                                            emac_data_rate_t data_rate,
                                            emac_dpi_err_cfg_t err_cfg);


PUBLIC PMC_ERROR emac_ingr_dpi_err_mask_get(emac_handle_t    *hndl, 
                                            UINT32           link, 
                                            emac_data_rate_t data_rate,
                                            BOOL8            *fcs_mask_ptr,
                                            BOOL8            *len_mask_ptr);

PUBLIC void emac_activated_link_get(emac_handle_t   *emac_handle,
                                    BOOL8           *rx_actv_link,
                                    BOOL8           *tx_actv_link);

PUBLIC void emac_link_rate_get(emac_handle_t       *hndl,
                               emac_data_rate_t    *data_rate);

PUBLIC void emac_mac_tx_flush_toggle(emac_handle_t       *hndl,
                                     UINT32              link);

PUBLIC BOOL8 emac_ingr_data_sink_get (emac_handle_t    *hndl, 
                                      UINT32           link);

PUBLIC PMC_ERROR emac_cleanup(emac_handle_t    *hndl,
                              UINT32            num_links, 
                              UINT32           *links,
                              BOOL8             ing_ts_en);

PUBLIC BOOL8 emac_link_lowpwr_get(emac_handle_t    *hndl, 
                                  UINT32            link,
                                  emac_data_dir_t   data_direction, 
                                  BOOL8             check_40G100G_lanes,
                                  emac_data_rate_t  data_rate);

PUBLIC emac_rx_data_src_t  emac_link_rx_data_src_get(emac_handle_t *hndl, 
                                                      UINT32         link);

PUBLIC emac_tx_data_dst_t  emac_link_tx_data_dst_get(emac_handle_t *hndl, 
                                                      UINT32         link);
PUBLIC PMC_ERROR emac_unidirectional_mode_get(emac_handle_t *hndl, 
                                              UINT32 link, 
                                              BOOL8 *enable);

PUBLIC void emac_pause_frame_fwd_cfg(emac_handle_t *hndl, 
                                     UINT32 link, 
                                     BOOL8 pause_fwd);

PUBLIC void emac_pause_frame_fwd_get(emac_handle_t *hndl, 
                                     UINT32         link, 
                                     BOOL8         *pause_fwd_ptr);

PUBLIC BOOL emac_100G_check_align_status_v_get(emac_handle_t *hndl);

PUBLIC void emac_100G_disable_align_status_loc_fault(emac_handle_t *hndl);

PUBLIC void emac_100G_enable_align_status_loc_fault(emac_handle_t *hndl);

PUBLIC void emac_100G_clear_align_status_loc_fault(emac_handle_t *hndl);

PUBLIC void emac_pcs_reset(emac_handle_t    *hndl,
                           UINT32           link);

#ifdef __cplusplus
}
#endif

#endif /* _EMAC_H */

/*
** end of file
*/
