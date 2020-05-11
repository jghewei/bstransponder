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

#ifndef _DIGI_H
#define _DIGI_H

#ifdef __cplusplus
extern "C" {
#endif
/*
** Include File
*/
#include "pmc_types.h"
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "pmc_sys.h"
#include "cbrc.h"
#include "digi_api.h"
#include "enet.h"
#include "cpb.h"
#include "coreotn.h"
#include "lineotn.h"
#include "mapotn.h"
#include "sifd.h"
#include "lifd.h"
#include "s16.h"
#include "sfi51_d8_tx_18x.h"
#include "sfi51_d8_rx_18x.h"
#include "t8.h"
#include "c8.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
#define DIGI_RESET                                  0x1
#define DIGI_ACTIVE                                 0x0

#define DIGI_ENABLE                                 0x1
#define DIGI_DISABLE                                0x0

/* IPI BITS value to signal SW that FW is done writing to shared mem */
#define DIGI_PMON_IPI_BITS_FW_DONE                          0x1

/* IPI BITS value to signal FW that SW is done reading from shared mem */
#define DIGI_PMON_IPI_BITS_SW_DONE                          0x0

/*
** Macro Definitions
*/
#define IS_DIGI_INT_CHNL_TYPE(int_type)     (int_type >= DIGI_INT_CHNL_TYPE_START)
#define IS_DIGI_INT_ALL_CHNLS(chnl_handle)  (DIGI_INT_ALL_CHNLS == chnl_handle)
#define IS_DIGI_INT_ALL_INTS(int_table_ptr) (DIGI_INT_ALL_INTS  == int_table_ptr)
#define DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_handle) \
    PMC_ASSERT(((util_global_chnl_header_t*)chnl_handle)->data_key == UTIL_GLOBAL_SWITCH_DATA_MAGIC_ID, \
               DIGI_ERR_INVALID_DATA_KEY,                                                               \
               ((util_global_chnl_header_t*)chnl_handle)->data_key,                                     \
               UTIL_GLOBAL_SWITCH_DATA_MAGIC_ID); 

#define DIGI_MAPPER_CHNL_HANDLE_MAGIC_CHECK(digi_handle, chnl_handle)    \
    PMC_ASSERT(((util_global_chnl_header_t*)chnl_handle)->data_key == UTIL_GLOBAL_SWITCH_DATA_MAGIC_ID, \
               DIGI_ERR_INVALID_DATA_KEY,                                                               \
               ((util_global_chnl_header_t*)chnl_handle)->data_key,                                     \
               UTIL_GLOBAL_SWITCH_DATA_MAGIC_ID); 

#define DIGI_PROD_APP_GET(digi_handle)                    (digi_handle->var.prod_app)
#define DIGI_PROD_APP_IS_TRANSMUXPONDER_CARD(digi_handle) (DIGI_PROD_APP_TRANSMUXPONDER_CARD   == DIGI_PROD_APP_GET(digi_handle))
#define DIGI_PROD_APP_IS_LINE_CARD(digi_handle)           (DIGI_PROD_APP_LINE_CARD             == DIGI_PROD_APP_GET(digi_handle))
#define DIGI_PROD_APP_IS_PORTLESS_CARD(digi_handle)       (DIGI_PROD_APP_PORTLESS_CARD         == DIGI_PROD_APP_GET(digi_handle))
#define DIGI_PROD_APP_IS_SYSOTN_CARD(digi_handle)         (DIGI_PROD_APP_SYSOTN_CARD           == DIGI_PROD_APP_GET(digi_handle))
#define DIGI_PROD_APP_IS_100G_3X40G_CFP_CARD(digi_handle) (DIGI_PROD_APP_100G_3_X_40G_CFP_CARD == DIGI_PROD_APP_GET(digi_handle))
#define DIGI60_PROD_APP_IS_LINE_CARD(digi_handle) (DIGI60_PROD_APP_LINE_CARD == DIGI_PROD_APP_GET(digi_handle))
#define DIGI60_PROD_APP_IS_TRANSMUXPONDER_CARD(digi_handle) (DIGI60_PROD_APP_TRANSMUXPONDER_CARD == DIGI_PROD_APP_GET(digi_handle))
#define DIGI60_PROD_APP_IS_TRANSMUXPONDER_SYS_CLIENT_CARD(digi_handle) (DIGI60_PROD_APP_TRANSMUXPONDER_SYS_CLIENT_CARD == DIGI_PROD_APP_GET(digi_handle))

#define IS_DIGI(digi_handle)    (digi_handle->base.common_info->device_id == 0x5440)
#define IS_DIGI60(digi_handle)  (digi_handle->base.common_info->device_id == 0x5441)

#define DIGI_OTN_ODU_LEVEL_GET(chnl_handle)               (((odu_struct_t *)chnl_handle)->mem_ptr->odu_level)
#define DIGI_OTN_ODU_LEVEL_IS_HO(chnl_handle)             (ODU_STRUCT_LEVEL_HO_ODU == ((odu_struct_t *)chnl_handle)->mem_ptr->odu_level)

#define DIGI_ODUK_PORT_TYPE_GET(chnl_handle) \
    ((((util_global_switch_data_def_t *)(chnl_handle))->oduk_data.port_type))

#define DIGI_ODUK_CHANNEL_GET(chnl_handle) \
    ((((util_global_switch_data_def_t *)(chnl_handle))->oduk_data.channel))

#define DIGI_ODUK_DATA_PTR_GET(chnl_handle) \
    ((((util_global_switch_data_def_t *)(chnl_handle))->oduk_data))


/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: digi_int_info_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure define element in interrupt info table.
*   Stores user cb, user token for each interrupt type (aggregation and channel).
*
* ELEMENTS:
* 
*   user_cb         - user callback saved for interrupt type
*   token           - user token that is passed with cb registration
*   token_size      - size of token in bytes
*******************************************************************************/
typedef struct digi_int_info 
{
    digi_int_cb_fcn     user_cb;
    
    void*               token;
    UINT32              token_size;
    
} digi_int_info_t;


/*******************************************************************************
* ENUM:  digi_pmon_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   There is 1-1 mapping between SUBID elements and digi pmon data collection
*   structures.
*
* ELEMENTS:
*       DIGI_PMON_ID_ENET_MIB - corresponds to digi_pmon_enet_mib_t
*       DIGI_PMON_LINEOTN_FEC_COUNTERS - corresponds to digi_pmon_lineotn_fec_counters_t
*       DIGI_PMON_LINEOTN_OTU_FRM_COUNTERS - corresponds to digi_pmon_lineotn_otu_frm_counters_t
*       DIGI_PMON_LINEOTN_INTLV_FIFO_ERRORS - corresponds to digi_pmon_lineotn_intlv_fifo_errors_t
*
*******************************************************************************/
typedef enum
{
    DIGI_PMON_ID_ENET_MIB = 0,
    DIGI_PMON_LINEOTN_FEC_COUNTERS,
    DIGI_PMON_LINEOTN_OTU_FRM_COUNTERS,
    DIGI_PMON_LINEOTN_INTLV_FIFO_ERRORS,
    DIGI_PMON_LINEOTN_DINTLV_FIFO_ERRORS,
    DIGI_PMON_ID_MAX            /* Must be always the last. */

} digi_pmon_id_t;

/*       DIGI_PMON_LINEOTN_DINTLV_FIFO_ERRORS - corresponds to digi_pmon_lineotn_dintlv_fifo_errors_t*/
/*******************************************************************************
* ENUM: digi_pmon_wo_action_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Indicates to the receiver, which action to take once this work order is
*   received.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum
{
    DIGI_PMON_WO_ACTION_START = 0,
    DIGI_PMON_WO_ACTION_STOP,
    DIGI_PMON_WO_ACTION_MAX     /* Must be always the last. */

} digi_pmon_wo_action_t;




/*******************************************************************************
*  STRUCTURE: digi_pmon_lineotn_fec_counters_fw_t
*  ______________________________________________________________________________
* 
*  DESCRIPTION:
*    Structure defining LINEOTN FEC error counters for a single channel.
*    This structure is used for FW transfer. Customer facing structure is
*    digi_pmon_lineotn_fec_counters_t.
*    The description of each counter is the register field name.
* 
* 
*  ELEMENTS:
*     fec_total_corr_errs                                 - TOTAL_CORR_ERRS[31:0]
* 
*     40G Swizzle FEC:
*     fec_otuk_swz_lpa_stats_fp                           - LPA_STATS_FP[31:0]
*     fec_otuk_swz_lpa_stats_uncorr_cwds                  - LPA_STATS_UNCORR_CWDS [31:0]
*     fec_otuk_swz_lpa_stats_corr_1s                      - LPA_STATS_CORR_1S [31:0]
*     fec_otuk_swz_lpa_stats_corr_0s                      - LPA_STATS_CORR_0S [31:0]
*     fec_otuk_swz_llsa_stats_uncorr_cwds                 - LLSA_STATS_UNCORR_CWDS [31:0]
*     fec_otuk_swz_llsa_stats_lane_corr_1s[20]            - LLSA_STATS_LANE_CORR_1S [31:0]
*     fec_otuk_swz_llsa_stats_lane_corr_0s[20]            - LLSA_STATS_LANE_CORR_0S [31:0]
* 
*     GFEC (10G,40G,100G):
*     fec_otuk_gfec_lpa_stats_fp                          - LPA_STATS_FP [31:0]
*     fec_otuk_gfec_lpa_stats_uncorr_cwds                 - LPA_STATS_UNCORR_CWDS [31:0]
*     fec_otuk_gfec_lpa_stats_corr_1s                     - LPA_STATS_CORR_1S [31:0]
*     fec_otuk_gfec_lpa_stats_corr_0s                     - LPA_STATS_CORR_0S [31:0]
* 
*     I4 FEC (10G,40G)
*     fec_otuk_i4_fec_corr_0s_count                       - CORRECTED_ZEROES_X_COUNT [31:0]   (X=3..0)
*     fec_otuk_i4_fec_corr_1s_count                       - CORRECTED_ONES_X_COUNT [31:0]     (X=3..0)
*     fec_otuk_i4_fec_uncorr_error_count                  - UNCORR_ERROR_X_COUNT [31:0]       (X=3..0)
*     fec_otuk_i4_fec_rx_frm_pulse_count                  - RX_FRAME_PULSE_X_COUNT [31:0]     (X=3..0)
* 
*     I7 FEC (10G,40G)
*     fec_otuk_i7_fec_corr_0s_count                       - CORRECTED_ZEROES_X_COUNT [31:0]   (X=3..0)
*     fec_otuk_i7_fec_corr_1s_count                       - CORRECTED_ONES_X_COUNT [31:0]     (X=3..0)
*     fec_otuk_i7_fec_uncorr_error_count                  - UNCORR_ERROR_X_COUNT [31:0]       (X=3..0)
*     fec_otuk_i7_fec_rx_frm_pulse_count                  - RX_FRAME_PULSE_X_COUNT [31:0]     (X=3..0)
* 
* ******************************************************************************/
  typedef struct digi_pmon_lineotn_fec_counters_fw_t
 {
    UINT32 fec_total_corr_errs;

    UINT32 fec_otuk_swz_lpa_stats_fp;
    UINT32 fec_otuk_swz_lpa_stats_uncorr_cwds;
    UINT32 fec_otuk_swz_lpa_stats_corr_1s;
    UINT32 fec_otuk_swz_lpa_stats_corr_0s;
    UINT32 fec_otuk_swz_llsa_stats_uncorr_cwds;
    UINT32 fec_otuk_swz_llsa_stats_lane_corr_1s[20];
    UINT32 fec_otuk_swz_llsa_stats_lane_corr_0s[20];

    UINT32 fec_otuk_gfec_lpa_stats_fp;
    UINT32 fec_otuk_gfec_lpa_stats_uncorr_cwds;
    UINT32 fec_otuk_gfec_lpa_stats_corr_1s;
    UINT32 fec_otuk_gfec_lpa_stats_corr_0s;

    UINT32 fec_otuk_i4_fec_corr_0s_count;
    UINT32 fec_otuk_i4_fec_corr_1s_count;
    UINT32 fec_otuk_i4_fec_uncorr_error_count;
    UINT32 fec_otuk_i4_fec_rx_frm_pulse_count;

    UINT32 fec_otuk_i7_fec_corr_0s_count;
    UINT32 fec_otuk_i7_fec_corr_1s_count;
    UINT32 fec_otuk_i7_fec_uncorr_error_count;
    UINT32 fec_otuk_i7_fec_rx_frm_pulse_count;

    UINT32 fec_unused_params1[7];


    UINT32 fec_unused_params2[7];

 } digi_pmon_lineotn_fec_counters_fw_t;

/*******************************************************************************
* STRUCTURE: digi_pmon_t
*______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining PMON counts for DIGI.
*
* ELEMENTS: 
*    
*    lineotn_otn_hdr          - LINEOTN OTN PMON header
*    lineotn_otn              - LINEOTN OTN PMON counts
*    sysotn_otn_hdr           - SYSOTN OTN PMON header
*    sysotn_otn               - SYSOTN OTN PMON counts
*    coreotn_stg1_hdr         - COREOTN Stage 1 OTN PMON header
*    coreotn_stg1_tcm         - COREOTN Stage 1 TCM OTN PMON counts
*    coreotn_stg1_pm          - COREOTN Stage 1 PM OTN PMON counts
*    coreotn_stg1_prbs        - COREOTN Stage 1 PRBS PMON counts
*    coreotn_stg2_hdr         - COREOTN Stage 2 OTN PMON header
*    coreotn_stg2_tcm         - COREOTN Stage 2 TCM OTN PMON counts
*    coreotn_stg2_pm          - COREOTN Stage 2 PM OTN PMON counts
*    coreotn_stgn_hdr         - COREOTN Stage N OTN PMON header
*    coreotn_stgn_tcm         - COREOTN Stage N TCM OTN PMON counts
*    coreotn_stgn_pm          - COREOTN Stage N PM OTN PMON counts
*    coreotn_stgn_prbs        - COREOTN Stage N PRBS PMON counts
*    coreotn_stg3a_hdr        - COREOTN Stage 3A OTN PMON header
*    coreotn_stg3a_tcm        - COREOTN Stage 3A TCM OTN PMON counts
*    coreotn_stg3a_pm         - COREOTN Stage 3A PM OTN PMON counts
*    coreotn_stg3b_hdr        - COREOTN Stage 3B OTN PMON header
*    coreotn_stg3b_tcm        - COREOTN Stage 3B TCM OTN PMON counts
*    coreotn_stg3b_pm         - COREOTN Stage 3B PM OTN PMON counts
*    coreotn_stg3b_prbs       - COREOTN Stage 3B PRBS PMON counts
*    coreotn_stg4_hdr         - COREOTN Stage 4 OTN PMON header
*    coreotn_stg4_tcm         - COREOTN Stage 4 TCM OTN PMON counts
*    coreotn_stg4_pm          - COREOTN Stage 4 PM OTN PMON counts
*    coreotn_stg4_prbs        - COREOTN Stage 4 PRBS PMON counts
*
*    The following PMON counters are unused:
*    coreotn_ohp1, coreotn_ohp2, coreotn_ohp3, coreotn_rcp_ri, coreotn_rcp_di 
*    coreotn_ohp1_hdr         - COREOTN OHP1 PMON header
*    coreotn_ohp1             - COREOTN OHP1 PMON counts
*    coreotn_ohp2_hdr         - COREOTN OHP2 PMON header
*    coreotn_ohp2             - COREOTN OHP2 PMON counts
*    coreotn_ohp3_hdr         - COREOTN OHP3 PMON header
*    coreotn_ohp3             - COREOTN OHP3 PMON counts
*    coreotn_rcp_ri_hdr       - COREOTN RCP_RI PMON header
*    coreotn_rcp_ri           - COREOTN RCP_RI PMON counts
*    coreotn_rcp_di_hdr       - COREOTN RCP_DI PMON header
*    coreotn_rcp_di           - COREOTN RCP_DI PMON counts
*
*    lineotn_fec_hdr          - LINEOTN FEC PMON header
*    lineotn_fec              - LINEOTN FEC PMON counts
*    sysotn_fec_hdr           - SYSOTN FEC PMON header
*    sysotn_fec               - SYSOTN FEC PMON counts
*    cbrc_hdr                 - CBRC PMON header
*    cbrc_sdh_pmg_prbs        - CBRC SDH PMG PRBS counts
*    cbrc_sdh_pmg             - CBRC SDH PMG counts
*    cbrc_fc1200              - CBRC FC1200 counts
*    cbrc_fc_cbr              - CBRC FC CBR counts
*    mgmt_hdr                 - MGMT_PORT PMON header
*    mgmt_pmon                - MGMT_PORT PMON counts
*    enet_line_hdr            - ENET_LINE PMON header
*    enet_line_mib            - ENET_LINE MIB PMON counts
*    enet_line_errors         - ENET_LINE Errors PMON counts
*    enet_sys_hdr             - ENET_SYS PMON header
*    enet_sys_mib             - ENET_SYS MIB PMON counts
*    enet_sys_errors          - ENET_SYS Errors PMON counts
*    mapotn_hdr               - MAPOTN PMON header
*    mapotn_counts            - MAPOTN PMON counts
*    mapotn_gmp_sync_loss     - MAPOTN GMP SYNC LOSS counts
*    mcpb_hdr                 - MCPB PMON header
*    mcpb_pmon                - MCPB PMON counts
*    sifd_hdr                 - SIFD PMON header
*    sifd_pmon                - SIFD PMON counts
*******************************************************************************/
typedef struct digi_pmon_t
 {
    pmon_dma_hdr_t                           lineotn_otn_hdr;
    digi_pmon_lineotn_otu_frm_counters_t     lineotn_otn[DIGI_OTN_SERVER_CHNL_MAX];
    pmon_dma_hdr_t                           sysotn_otn_hdr;
    digi_pmon_lineotn_otu_frm_counters_t     sysotn_otn;

    pmon_dma_hdr_t                           coreotn_stg1_hdr;
    digi_pmon_coreotn_odukt_counters_t       coreotn_stg1_tcm[COREOTN_STG1_TCM_CH_MAX_NUM];
    digi_pmon_coreotn_odukp_counters_t       coreotn_stg1_pm[DIGI_OTN_SERVER_HO_CHNL_MAX];
    digi_pmon_coreotn_odukp_prbs_counter_t   coreotn_stg1_prbs[DIGI_OTN_SERVER_HO_CHNL_MAX];
    pmon_dma_hdr_t                           coreotn_stg2_hdr;
    digi_pmon_coreotn_odukt_counters_t       coreotn_stg2_tcm[COREOTN_STG2_TCM_CH_MAX_NUM];
    digi_pmon_coreotn_odukp_counters_t       coreotn_stg2_pm[DIGI_OTN_SERVER_MO_CHNL_MAX];
    pmon_dma_hdr_t                           coreotn_stgn_hdr;
    digi_pmon_coreotn_odukt_counters_t       coreotn_stgn_tcm[COREOTN_STGN_TCM_CH_MAX_NUM];
    digi_pmon_coreotn_odukp_counters_t       coreotn_stgn_pm[COREOTN_STGN_PM_CH_MAX_NUM];
    digi_pmon_coreotn_odukp_prbs_counter_t   coreotn_stgn_prbs[COREOTN_STGN_TSE_CH_MAX_NUM];
    pmon_dma_hdr_t                           coreotn_stg3a_hdr;
    digi_pmon_coreotn_odukt_counters_t       coreotn_stg3a_tcm[COREOTN_STG3A_TCM_CH_MAX_NUM];
    digi_pmon_coreotn_odukp_counters_t       coreotn_stg3a_pm[DIGI_OTN_SERVER_LO_CHNL_MAX];
    pmon_dma_hdr_t                           coreotn_stg3b_hdr;
    digi_pmon_coreotn_odukt_counters_t       coreotn_stg3b_tcm[COREOTN_STG3B_TCM_CH_MAX_NUM];
    digi_pmon_coreotn_odukp_counters_t       coreotn_stg3b_pm[DIGI_OTN_SERVER_LO_CHNL_MAX];
    digi_pmon_coreotn_odukp_prbs_counter_t   coreotn_stg3b_prbs[COREOTN_STG3B_TSE_CH_MAX_NUM];
    pmon_dma_hdr_t                           coreotn_stg4_hdr;
    digi_pmon_coreotn_odukt_counters_t       coreotn_stg4_tcm[COREOTN_STG4_TCM_CH_MAX_NUM];
    digi_pmon_coreotn_odukp_counters_t       coreotn_stg4_pm[DIGI_OTN_SERVER_LO_CHNL_MAX];
    digi_pmon_coreotn_odukp_prbs_counter_t   coreotn_stg4_prbs[COREOTN_STG4_TSE_CH_MAX_NUM];
    UINT32                                   coreotn_oduksw_ch_drop_cnt[DIGI_OTN_SERVER_LO_CHNL_MAX];

    /* 
     * The following PMON counters are unused:
     * coreotn_ohp1, coreotn_ohp2, coreotn_ohp3, coreotn_rcp_ri, coreotn_rcp_di 
     */
    pmon_dma_hdr_t                           coreotn_ohp1_hdr;
    digi_pmon_coreotn_fege_mstat_t           coreotn_ohp1;
    pmon_dma_hdr_t                           coreotn_ohp2_hdr;
    digi_pmon_coreotn_fege_mstat_t           coreotn_ohp2;
    pmon_dma_hdr_t                           coreotn_ohp3_hdr;
    digi_pmon_coreotn_fege_mstat_t           coreotn_ohp3;
    pmon_dma_hdr_t                           coreotn_rcp_ri_hdr;
    digi_pmon_coreotn_fege_mstat_t           coreotn_rcp_ri;
    pmon_dma_hdr_t                           coreotn_rcp_di_hdr;
    digi_pmon_coreotn_fege_mstat_t           coreotn_rcp_di;

    pmon_dma_hdr_t                           lineotn_fec_hdr;
    digi_pmon_lineotn_fec_counters_fw_t      lineotn_fec[DIGI_OTN_SERVER_CHNL_MAX];
    pmon_dma_hdr_t                           sysotn_fec_hdr;
    digi_pmon_lineotn_fec_counters_fw_t      sysotn_fec;

    pmon_dma_hdr_t                           cbrc_hdr;
    UINT32                                   cbrc_unused;
    digi_pmon_cbrc_sdh_pmg_t                 cbrc_sdh_pmg[DIGI_CBRC_CHNL_MAX];
    digi_pmon_cbrc_fc_cbr_t                  cbrc_fc_cbr[DIGI_CBRC_CHNL_MAX];
    digi_pmon_cbrc_fc1200_t                  cbrc_fc1200[DIGI_CBRC_CHNL_MAX];

    pmon_dma_hdr_t                           mgmt_hdr;
    digi_pmon_coreotn_fege_mstat_t           mgmt_pmon;

    pmon_dma_hdr_t                           enet_line_hdr;   
    digi_pmon_enet_mib_t                     enet_line_mib[DIGI_ENET_LINE_CHNL_MAX];   
    digi_pmon_enet_errors_t                  enet_line_errors[DIGI_ENET_LINE_CHNL_MAX];
    digi_pmon_enet_eclass_t                  enet_line_eclass[DIGI_ENET_LINE_CHNL_MAX];

    pmon_dma_hdr_t                           enet_sys_hdr;    
    digi_pmon_enet_mib_t                     enet_sys_mib[DIGI_ENET_SYS_CHNL_MAX];    
    digi_pmon_enet_errors_t                  enet_sys_errors[DIGI_ENET_SYS_CHNL_MAX]; 
    digi_pmon_enet_eclass_t                  enet_sys_eclass[DIGI_ENET_LINE_CHNL_MAX];

    pmon_dma_hdr_t                           mapotn_hdr; 
    digi_pmon_mapotn_counts_t                mapotn_counts[DIGI_MAPOTN_CHNL_MAX]; 
    digi_pmon_mapotn_gmp_sync_loss_t         mapotn_gmp_sync_loss; 

    pmon_dma_hdr_t                           mcpb_hdr; 
    digi_pmon_cpb_t                          mcpb_pmon[DIGI_DPI_PORT_MAX];

    pmon_dma_hdr_t                           sifd_hdr; 
    digi_pmon_sifd_t                         sifd_pmon[DIGI_SIFD_ILKN_NUM];

} digi_pmon_t;

/* always define DIGI_FW_STATS, we can remove this if needed. */
#ifndef DIGI_FW_STATS
#define DIGI_FW_STATS
#endif

#ifdef DIGI_FW_STATS
/*******************************************************************************
* STRUCTURE: digi_fw_stats_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*       Structure defining firmware stats.
*
* ELEMENTS:
*       stats_number_of_stats_addr    - SPRAM address of number of stats
*       stats_list_of_statistics_addr - SPRAM address of list of statistics records
*       stats_command_addr            - SPRAM address of command location.
*       pmcfw_assert_info_addr        - SPRAM address of firmware assert information.
*       display_firmware_stats        - should firmware statistics be displayed?
*       
*       app_log_size                   - size (bytes) of DBG_LOG application log memory location 
*       app_log_ptr_addr               - SPRAM address of the start of the DBG_LOG application log memory
*       app_log_wr_idx_addr            - SPRAM address of Write index for the application log    
*       app_log_num_entries_max        - Maximum number of entries in the application log       
*       
*       cur_hostmsg_stat               - current host message in progress
*       prev_hostmsg_stat              - previous host message
*     
*       opsa_mpmo_debug_addr           - spram address of opsa mpmo debug variables
*       opsa_mpmo_debug_size           - size of opsa mpmo debug variables
*       last_mpmo_debug                - previous values of opsa mpmo debug variables.
*
*       stats_number_of_stats          - number of firmware stats
*       firmware_stats_buffer          - pmc_sys_fw_stats_t[stats_number_of_stats]
*******************************************************************************/
typedef struct {
    UINT32 stats_number_of_stats_addr;
    UINT32 stats_list_of_statistics_addr;
    UINT32 stats_command_addr;
    UINT32 pmcfw_assert_info_addr;

    UINT32 app_log_size;                
    UINT32 app_log_ptr_addr;            
    UINT32 app_log_wr_idx_addr;         
    UINT32 app_log_num_entries_max; 

    UINT32 app_log_last_sequence_number_displayed;
    UINT32 app_log_last_timestamp;    
     
    BOOL display_firmware_stats;

    pmc_sys_stat_t*  cur_hostmsg_stat;
    pmc_sys_stat_t*  prev_hostmsg_stat;

    UINT32 opsa_mpmo_debug_addr;
    UINT32 opsa_mpmo_debug_size;
    fw_opsa_mpmo_debug_t last_mpmo_debug;

    UINT32              stats_number_of_stats;
    pmc_sys_fw_stats_t *firmware_stats_buffer;
} digi_fw_stats_t;
#endif

/*
** Global variables
*/
/** should the firmware log be displayed after each firmware message
 * and should firmware statistics be displayed during digi_shutdown() */
extern BOOL8 digi_fw_firmware_logging_enabled;

/*
** Function Prototypes
*/

PUBLIC UINT32 digi_reg_read(digi_handle_t *digi_handle, UINT32 vaddr);
PUBLIC void digi_reg_write(digi_handle_t *digi_handle, UINT32 vaddr, UINT32 data);
PUBLIC void digi_mem_write(digi_handle_t *digi_handle, UINT32 vaddr, UINT32 len, UINT8 *data_ptr);

PUBLIC PMC_ERROR digi_otn_odu_handle_pool_get(digi_handle_t           *digi_handle,
                                              odu_struct_odu_level_t   odu_level,
                                              odu_struct_t           **odu_pool_pptr);

PUBLIC PMC_ERROR digi_int_param_check(digi_handle_t    *digi_handle,
                                      void             *chnl_handle,
                                      digi_int_type_t   int_type,
                                      void             *int_table_ptr);
PUBLIC PMC_ERROR digi_int_chnl_param_check(digi_handle_t    *digi_handle,
                                           void             *chnl_handle,
                                           digi_int_type_t   int_type,
                                           void             *int_table_ptr);

PUBLIC BOOL digi_int_is_cb_registered(digi_handle_t    *digi_handle,
                                      digi_int_type_t   int_type);

PUBLIC PMC_ERROR digi_int_callback_set(digi_handle_t        *digi_handle,
                                       digi_int_type_t       int_type,
                                       void                 *token,
                                       UINT32                token_size,
                                       digi_int_cb_fcn       user_cb);

PUBLIC PMC_ERROR digi_int_info_get(digi_handle_t    *digi_handle,
                                   digi_int_type_t   int_type,
                                   digi_int_info_t **int_info_pptr);

PUBLIC PMC_ERROR digi_int_link_range_get(digi_handle_t      *digi_handle,
                                         void               *chnl_handle,
                                         digi_int_type_t     int_type,
                                         UINT32             *start_link,
                                         UINT32             *end_link);

PUBLIC PMC_ERROR digi_int_callback_run(digi_handle_t    *digi_handle,
                                       void*             chnl_handle,
                                       digi_int_type_t   int_type);

PUBLIC PMC_ERROR digi_int_top_enabled_check(digi_handle_t    *digi_handle,
                                            digi_int_type_t   int_type,
                                            BOOL             *int_found_ptr);

PUBLIC PMC_ERROR digi_pmon_shared_mem_get(digi_handle_t    *digi_handle,
                                          digi_pmon_t     **pmon_pptr);

PUBLIC PMC_ERROR digi_enet_enhanced_pmon_chnl_retrieve_internal(digi_handle_t               *digi_handle,
                                                                void                        *chnl_handle,
                                                                digi_enet_pmon_chnl_t       *pmon_table_ptr,
                                                                digi_enet_pmon_mode_t       *pmon_type_ptr);

PUBLIC PMC_ERROR digi_otn_tcm_pid_valid_set(digi_handle_t           *digi_handle,
                                            digi_otn_odu_chnl_t     *chnl_handle,
                                            UINT32                   tcm_pid_slot,
                                            BOOL                     valid);

PUBLIC BOOL digi_otn_is_tcm_pid_valid(digi_handle_t           *digi_handle,
                                      digi_otn_odu_chnl_t     *chnl_handle,
                                      UINT32                   tcm_pid_slot);

PUBLIC PMC_ERROR digi_otn_ddeg_init(digi_handle_t    *digi_handle);

PUBLIC PMC_ERROR digi_otn_odu_int_framer_chnl_ddeg_enable(digi_handle_t             *digi_handle,
                                                          odu_struct_odu_level_t     odu_level,
                                                          coreotn_int_source_t       source, 
                                                          UINT32                     start_chnl,
                                                          UINT32                     end_chnl,
                                                          coreotn_int_framer_chnl_t *int_table_ptr,
                                                          coreotn_int_framer_chnl_t *int_en_table_ptr,
                                                          BOOL                       enable);

PUBLIC PMC_ERROR digi_otn_odu_int_ddeg_stage_chnl_retrieve(digi_handle_t            *digi_handle,
                                                           digi_otn_odu_chnl_t      *chnl_handle,
                                                           coreotn_ddeg_mon_seg_t    mon_seg,
                                                           coreotn_int_chnl_t       *int_en_table_ptr,
                                                           coreotn_int_chnl_t       *int_table_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_ddeg_scope_get(digi_handle_t             *digi_handle,
                                                 coreotn_ddeg_mon_seg_t     mon_seg,
                                                 BOOL                      *enh_valid_ptr,
                                                 BOOL                      *bursty_valid_ptr,
                                                 BOOL                      *valid_ptr);


PUBLIC PMC_ERROR digi_otn_odu_int_ddeg_clear(digi_handle_t            *digi_handle);

PUBLIC PMC_ERROR digi_otn_odu_int_ddeg_enabled_check(digi_handle_t             *digi_handle,
                                                     coreotn_ddeg_mon_seg_t     mon_seg,  
                                                     BOOL                      *int_found_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_active_chnl_get(digi_handle_t                *digi_handle,
                                                  odu_struct_odu_level_t        odu_level,
                                                  BOOL                          pack_chnls,
                                                  digi_otn_odu_int_summary_t   *int_summary_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_enabled_check_summary_get(digi_handle_t              *digi_handle,
                                                            odu_struct_odu_level_t      odu_level,
                                                            digi_otn_odu_int_summary_t *int_summary_ptr);

PUBLIC PMC_ERROR digi_otn_odu_int_chnl_stage_enabled_check(digi_handle_t               *digi_handle,
                                                           digi_otn_odu_chnl_t         *chnl_handle,
                                                           odu_struct_odu_level_t       odu_level,
                                                           digi_otn_odu_int_summary_t  *int_summary_ptr,
                                                           BOOL                        *int_found_ptr);

PUBLIC PMC_ERROR digi_pmon_ipi_int_enable(digi_handle_t         *digi_handle,
                                          digi_pmon_ipi_type_t   ipi_type,
                                          BOOL                   enable);

PUBLIC PMC_ERROR digi_pmon_ipi_int_clear(digi_handle_t          *digi_handle,
                                         digi_pmon_ipi_type_t    ipi_type);
                                                                              
PUBLIC BOOL digi_pmon_ipi_int_retrieve(digi_handle_t          *digi_handle,
                                       digi_pmon_ipi_type_t    ipi_type);

PUBLIC UINT32 digi_pmon_ipi_bit_get(digi_handle_t          *digi_handle,
                                    digi_pmon_ipi_type_t    ipi_type);

PUBLIC PMC_ERROR digi_pmon_ipi_bit_set(digi_handle_t          *digi_handle,
                                       digi_pmon_ipi_type_t    ipi_type,
                                       BOOL                    enable);
PUBLIC PMC_ERROR digi_enet_line_prov_internal(digi_handle_t *digi_handle,
                                              digi_serdes_port_t *port_ctxt_ptr,
                                              digi_enet_mode_t mode,
                                              BOOL mirror_port,
                                              digi_enet_client_chnl_t **chnl_ctxt_pptr);
PUBLIC PMC_ERROR digi_cpb_port_chnl_get(digi_handle_t               *digi_handle,
                                        util_global_switch_data_t   *chnl_handle,
                                        UINT32                      *port_ptr,
                                        UINT32                      *chnl_ptr);
PUBLIC PMC_ERROR digi_cpb_rate_get (digi_cpb_stream_t rate_data,
                                    UINT32 num_odu0,
                                    UINT32 *rate,
                                    util_schd_chnl_rate_exp_t *rate_unit,
                                    UINT32 *cpb_data);
PUBLIC PMC_ERROR digi_mapper_gfp_tx_fcs_mode_internal_set(digi_handle_t *digi_handle,
                                                          digi_mapper_chnl_t *chnl_ctxt_pptr,
                                                          mapotn_fcs_mode_t fcs_mode);

/* OPSA Configuration */
PUBLIC PMC_ERROR digi_fw_opsa_cfg_mpma_port_add(digi_handle_t *digi_handle,
                                                hostmsg_opsa_subsystem_cfg_t subsystem,
                                                UINT32 *chnl_add_msk_ptr,
                                                UINT8  *qs_ch_en_ptr,
                                                BOOL8 is_config_only);
PUBLIC PMC_ERROR digi_fw_opsa_cfg_mpma_port_drop(digi_handle_t *digi_handle,
                                                hostmsg_opsa_subsystem_cfg_t subsystem,
                                                UINT32 *chnl_drop_msk_ptr);
PUBLIC PMC_ERROR digi_fw_opsa_cfg_mpmo_port_add(digi_handle_t *digi_handle,
                                                hostmsg_opsa_subsystem_cfg_t subsystem,
                                                UINT32 chnl_add_msk_ptr[3],
                                                UINT8  ch_data_target_ptr[96],
                                                UINT8  ch_ckctl_port_ptr[96],
                                                UINT8  coreotn_fo2_channel_ptr[24],
                                                UINT8  coreotn_fo2_ckctl_port_ptr[24],
                                                UINT8   num_coreotn_fo2_channels,
                                                BOOL8 is_config_only);
PUBLIC PMC_ERROR digi_fw_opsa_cfg_mpmo_port_drop(digi_handle_t *digi_handle,
                                                hostmsg_opsa_subsystem_cfg_t subsystem,
                                                UINT32 chnl_drop_msk_ptr[3]);
PUBLIC PMC_ERROR digi_fw_opsa_cfg_fmf1_txjc_cfc_fifo_clrb(digi_handle_t *digi_handle,
                                                          UINT8 value, UINT32 *chnl_msk_ptr);

PUBLIC PMC_ERROR digi_fw_opsa_cfg_tgmp_txjc_cfc_fifo_clrb(digi_handle_t *digi_handle,
                                                          UINT8 value, UINT32 *chnl_msk_ptr);

PUBLIC PMC_ERROR digi_fw_opsa_fifo_cent_trigger(digi_handle_t *digi_handle, UINT32 *chnl_msk_ptr);
PUBLIC PMC_ERROR digi_fw_opsa_cfg_tau_value(digi_handle_t *digi_handle,
                                            hostmsg_opsa_subsystem_cfg_t subsystem,
                                            BOOL cfg_mpma, UINT16 tau_value_us);
PUBLIC PMC_ERROR digi_fw_opsa_cfg_mpmo_times(digi_handle_t *digi_handle,
                                            digi_fw_opsa_mpmo_times_t *opsa_mpmo_times);
PUBLIC PMC_ERROR digi_fw_opsa_cfg_mpmo_fifo_trigger_enables(digi_handle_t *digi_handle,
                                            digi_fw_opsa_mpmo_fifo_trigger_enables_t *digi_fw_opsa_mpmo_fifo_trigger_enables);
PUBLIC PMC_ERROR digi_fw_opsa_mpmo_debug_addr_get(digi_handle_t *digi_handle,
                                                  UINT32 *opsa_mpmo_debug_addr,
                                                  UINT32 *opsa_mpmo_debug_size);
/* COREOTN dLOOMFI defect monitor configuration */
PUBLIC PMC_ERROR digi_fw_coreotn_cfg_dloomfi_mon(digi_handle_t *digi_handle,
                                                 BOOL8 is_enable,
                                                 hostmsg_otn_server_src_t defect_src);
PUBLIC PMC_ERROR digi_fw_coreotn_ohp_en(digi_handle_t *digi_handle,
                                                 UINT8 ohp_port,
                                                 BOOL8 ohp_en,
                                                 BOOL8 portA_en,
                                                 BOOL8 portB_en);

/* LED Monitoring configuration */
PUBLIC PMC_ERROR digi_fw_led_set_spi_dev_addr(digi_handle_t *digi_handle,
                                              UINT32 spi_reg_offset);
PUBLIC PMC_ERROR digi_fw_led_set_mon_ctrl(digi_handle_t *digi_handle,
                                          BOOL8 is_enable,
                                          UINT8 chnl_num,
                                          digi_led_interface_t led_interface,
                                          digi_led_cbrc_type_t led_cbrc_type);
PUBLIC PMC_ERROR digi_otn_server_stream_is_10g(digi_handle_t           *digi_handle,
                                               digi_otn_server_chnl_t  *chnl_handle);
PUBLIC PMC_ERROR digi_otn_server_stream_is_40g(digi_handle_t           *digi_handle,
                                               digi_otn_server_chnl_t  *chnl_handle);
PUBLIC PMC_ERROR digi_otn_server_stream_is_100g(digi_handle_t           *digi_handle,
                                                digi_otn_server_chnl_t  *chnl_handle);
#ifdef DIGI_FW_STATS
PUBLIC PMC_ERROR digi_fw_stats_reset(digi_handle_t *digi_handle);
PUBLIC PMC_ERROR digi_fw_stats_get(digi_handle_t *digi_handle, digi_fw_stats_t *wdigi_fw_stats);
#endif
PUBLIC void digi_fw_just_show_firmware_stats(digi_handle_t *digi_handle);
PUBLIC void digi_fw_show_firmware_errors(digi_handle_t *digi_handle );
PUBLIC void digi_fw_enable_firmware_stats(digi_handle_t *digi_handle, BOOL enable );
PUBLIC void digi_fw_show_debug_log(digi_handle_t *digi_handle);
PUBLIC void digi_fw_show_mpmo_counts(digi_handle_t *digi_handle);
PUBLIC void digi_fw_logging_enable(BOOL8 enable);
PUBLIC void digi_fw_log_init(digi_handle_t *digi_handle);
PUBLIC void digi_fw_show_queue_stall(digi_handle_t *digi_handle,BOOL8 full);
PUBLIC void digi_fw_poll_debug_log(digi_handle_t *digi_handle);
PUBLIC void digi_fw_wait_for_restart(digi_handle_t *digi_handle);

PUBLIC void digi_otn_server_oduksw_prov_deprov_end(digi_handle_t *digi_handle,
                                                   BOOL8 is_prov);

PUBLIC void digi_otn_fc_calendar_bulk_update(digi_handle_t *digi_handle);


#ifdef __cplusplus
}
#endif

#endif /* _DIGI_H */

/*
** end of file
*/
