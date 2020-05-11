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

#ifndef _MPMO_H
#define _MPMO_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "mpmo_api.h"
#include "scbs3.h"
#include "scbs3_api.h"
#include "cfc.h"
#include "cfc_api.h"
#include "util_global.h"
#include "util_opsa.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: mpmo_type_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying block instance type.
*
* ELEMENTS:
*    MPMO_COREOTN_MUX_FRAMER   - Sourced from: ODUk; Pushing to: Tx Framer/Mux
*    MPMO_COREOTN_TO_MAPOTN    - Sourced from: ODUk; Pushing to: MAPOTN Rx Demapper
*    MPMO_COREOTN_CTL          - Sourced from: ODUk; Pushing to: COREOTN_CTL
*    MPMO_MAPOTN               - Sourced from: CPB;  Pushing to: Tx Mapper
*    MPMO_ENET                 - Sourced from: CPB;  Pushing to: ENET
*    MPMO_CBRC                 - Sourced from: CPB;  Pushing to: CBRC in Local Mode
*    MPMO_CBRC_DB              - Sourced from: CPB;  Pushing to: CBRC in Database Mode
*
*******************************************************************************/
typedef enum {
    MPMO_COREOTN_MUX_FRAMER = 0x00,
    MPMO_COREOTN_TO_MAPOTN  = 0x01,
    MPMO_COREOTN_CTL        = 0x02,
    MPMO_MAPOTN             = 0x03,
    MPMO_ENET               = 0x04,
    MPMO_CBRC               = 0x05,
    MPMO_CBRC_DB            = 0x06,
    MPMO_NUM_TYPES
} mpmo_type_id_t;

/*******************************************************************************
* ENUM: mpmo_pacer_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for Pacing mode.
*
* ELEMENTS:
*    MPMO_PACER_DEFAULT      - No special datapath flow-control/pacing
*    MPMO_PACER_CONTINUOUS   - The continuous timestamp hold-off mode
*    MPMO_PACER_AUTOMATIC    - The automatic timestamp hold-off mode
*    MPMO_PACER_ZONE_HOLDOFF - The automatic zone hold-off mode
*
*******************************************************************************/
typedef enum {
    MPMO_PACER_DEFAULT    = 0x0,
    MPMO_PACER_CONTINUOUS = 0x1,
    MPMO_PACER_AUTOMATIC  = 0x2,
    MPMO_PACER_ZONE_HOLDOFF  = 0x3
} mpmo_pacer_mode_t;

/*******************************************************************************
* ENUM: mpmo_chnl_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for Channel State Definition.
*
* ELEMENTS:
*    MPMO_CHNL_START                  - Start state
*    MPMO_CHNL_MAINT_SIG_EQUIPPED     - Maint Signal Equipped state
*    MPMO_CHNL_MAINT_SIG_OPERATIONAL  - Maint Signal Operational state
*    MPMO_CHNL_EQUIPPED               - Equipped state
*    MPMO_CHNL_OPERATIONAL            - Operational state
*
*******************************************************************************/
typedef enum {
    MPMO_CHNL_START                   = 0,
    MPMO_CHNL_MAINT_SIG_EQUIPPED      = 1,
    MPMO_CHNL_MAINT_SIG_OPERATIONAL   = 2,
    MPMO_CHNL_EQUIPPED                = 3,
    MPMO_CHNL_OPERATIONAL             = 4,
} mpmo_chnl_state_t;

/*******************************************************************************
* ENUM: mpmo_resize_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for Bandwidth Resize Type.
*
* ELEMENTS:
*    MPMO_CHNL_INCREASE_BW     - Increase bandwidth
*    MPMO_CHNL_DECREASE_BW     - Decrease bandwidth
*
*******************************************************************************/
typedef enum {
    MPMO_CHNL_INCREASE_BW   = 0,
    MPMO_CHNL_DECREASE_BW   = 1
} mpmo_resize_type_t;


/*
** Constants
*/
#define MPMO_MAX_CKCTL_PORT             26
#define MPMO_PKT_SIZE_MIN                1
#define MPMO_PKT_SIZE_MAX              511
#define MPMO_CSI_NORMAL_DEFAULT        0x0C

/* Number of frame period counters supported in the MPMO DSP. */
#define MPMO_NUM_FRM_PERIOD_CNT          8



#define MPMO_GHAO_COREOTN_AGE_PROP_GAIN { \
   /* Index is number of ODU0's in ODUFlex container */  \
   /*1,  2,  3,  4,  5,  6,  7,  8,  9, 10, */  \
    20, 21, 22, 22, 22, 23, 23, 23, 23, 23, /* oduflex 1 - 10  */ \
    24, 24, 24, 24, 24, 24, 24, 24, 24, 24, /* oduflex 11 - 20 */ \
    24, 25, 25, 25, 25, 25, 25, 25, 25, 25, /* oduflex 21 - 30 */ \
    25, 25, 25, 25, 25, 25, 25, 25, 25, 25, /* oduflex 31 - 40 */ \
    25, 25, 26, 26, 26, 26, 26, 26, 26, 26, /* oduflex 41 - 50 */ \
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, /* oduflex 51 - 60 */ \
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26, /* oduflex 61 - 70 */ \
    26, 26, 26, 26, 26, 26, 26, 26, 26, 26  /* oduflex 71 - 80 */ \
} /* MPMO_GHAO_COREOTN_AGE_PROP_GAIN */ 

#define MPMO_GHAO_COREOTN_AGE_INT_GAIN { \
   /* Index is number of ODU0's in ODUFlex container */  \
   /*1,  2,  3,  4,  5,  6,  7,  8,  9, 10, */  \
    14, 15, 16, 16, 16, 17, 17, 17, 17, 17, /* oduflex 1 - 10  */ \
    19, 18, 18, 18, 18, 18, 18, 18, 18, 18, /* oduflex 11 - 20 */ \
    18, 20, 19, 19, 19, 19, 19, 19, 19, 19, /* oduflex 21 - 30 */ \
    19, 19, 19, 19, 19, 19, 19, 19, 19, 19, /* oduflex 31 - 40 */ \
    19, 19, 21, 21, 21, 20, 20, 20, 20, 20, /* oduflex 41 - 50 */ \
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, /* oduflex 51 - 60 */ \
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20, /* oduflex 61 - 70 */ \
    20, 20, 20, 20, 20, 20, 20, 20, 20, 20  /* oduflex 71 - 80 */ \
} /* MPMO_GHAO_COREOTN_AGE_INT_GAIN */ 






/*
** Macro Definitions
*/
#define MPMO_INT_TABLE_PKT_DROP_0_DEFINE()                               \
    MPMO_INT_RANGE(mpmo_handle, mpmo, lfield_range, PKT_DROP , pkt_drop_replace_i     ); \

#define MPMO_INT_TABLE_PKT_DROP_1_DEFINE()                               \
    MPMO_INT_RANGE(mpmo_handle, mpmo, lfield_range, PKT_DROP , pkt_drop_no_replace_i      ); \
   

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: mpmo_frm_period_cnt_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Frame period configuration parameters
*
* ELEMENTS:
*   frm_period_type      - see util_opsa_cn_frame_period_t
*   frm_period_n         - The frame generation period numerator
*   frm_period_d         - The frame generation period denominator
*
*       
*******************************************************************************/
typedef struct mpmo_frm_period_cnt_t {   
    util_opsa_cn_frame_period_t frm_period_type;
    UINT32                      frm_period_n;
    UINT32                      frm_period_d;
} mpmo_frm_period_cnt_t;

/*******************************************************************************
* STRUCTURE: mpmo_default_param_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MPMO parameters specific to each instantiation
*
* ELEMENTS:
*   mpmo_type_id                      - MPMO type ID see mpmo_type_id_t
*   num_chnl                          - number of supported channels
*   lowest_bw                         - lowest supported bandwidth
*   dsp_present                       - DSP instantiation 
*   mpmo_frm_period_flex_alloc_method - frame period allocation method
*   frm_period_cnt_ptr                - frame period configuration pointer
*   frm_period_cnt                    - see mpmo_frm_period_cnt_t
*   zone_sel                          - zone information to output on the CKCTL[4:3] 
*                                       port and to the MPMO_DSP\n
*                                       0: Zone information from the zone interface is output\n
*                                       1: Zone information calculated from the relative age of received packets is output
*   seq_chk_en                        - packet insertion logic
*   pacer_mode                        - pacing mode employed see mpmo_pacer_mode_t
*   ssf_gen_en                        - SSF generation enable/disable
*   ssf_thres_en                      - SSF threshold enable/disable
*   bus_width                         - bus width on the egress DCI interface
*   ts_enable                         - enable/disable timestamp insertion feature
*   cn_req_mode                       - Cn output request mode
*   cn_bit_mode                       - Cn bit mode 1: Cn in bits 0: Cn in bytes
*   cal_entries_alloc_method          - Calendar entries distribution algorithm number
*   db_id                             - database ID
*   db_handle                         - database handle
*   age_filter_en                     - enable/disable to filter the calculated packet 
*                                       age from the ingress packets
*   ramp_stop_mode                    - the stop condition for the ODUflex hitless 
*                                       resizing ramp
*   ramp_extend_en                    - enables software to end the ODUflex hitless 
*                                       resizing ramp
*   bwr_ind_mode                      - enables/disables the AGE_DELTA squelching 
*                                       during LOS
*       
*******************************************************************************/
typedef struct mpmo_default_param_struct_t { 
    mpmo_type_id_t mpmo_type_id;    
    UINT32 num_chnl;
    DOUBLE lowest_bw;               
    BOOL dsp_present;
    BOOL8 mpmo_frm_period_flex_alloc_method;
    const mpmo_frm_period_cnt_t *frm_period_cnt_ptr; /*!< see title block for description */
    mpmo_frm_period_cnt_t frm_period_cnt[8];
    UINT32 zone_sel;
    BOOL seq_chk_en;
    mpmo_pacer_mode_t pacer_mode;
    BOOL ssf_gen_en;
    BOOL ssf_thres_en;
    UINT32 bus_width;
    BOOL ts_enable;
    UINT32 cn_req_mode;
    UINT32 cn_bit_mode;
    UINT32 cal_entries_alloc_method;
    util_gen_db_id_t db_id;
    UINT32 db_handle;
    UINT32 age_filter_en;
    UINT32 ramp_stop_mode;
    UINT32 ramp_extend_en;
    UINT32 bwr_ind_mode;

} mpmo_default_param_struct_t;


/*******************************************************************************
* STRUCTURE: mpmo_cfc_cfg_struct_t                                         
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:                                                                  
*   CFC configuration parameters for each instantiation                         
*                                                                               
* ELEMENTS:                                                                     
*   max_fifo_num      - Number of FIFOs in CFC. Equivalent to channel count     
*   page_size         - Number of entries per page                              
*                                                                               
*                                                                               
*******************************************************************************/
typedef struct mpmo_cfc_cfg_struct_t {   
    UINT32 max_fifo_num;   
    UINT32 page_size;      
} mpmo_cfc_cfg_struct_t;

/*******************************************************************************
* STRUCTURE: mpmo_scbs3_cfg_struct_t                                         
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:                                                                  
*   SCBS3 configuration parameters for each instantiation                         
*                                                                               
* ELEMENTS:                                                                     
*   scbs3_present      - TRUE: SCBS3 present. FALSE: No SCBS3 present     
*   total_rate         - total interface rate = total_rate * 10^(total_rate_units)      
*   total_rate_units   - 10^ exponent part of total_rate     
*   toc_sync_en        - TRUE: enable synchronization to the 'sch_toc_in' input port.
*                        FALSE: ignore the 'sch_toc_in' input port.
*   toc_sync           - Entry that corresponds to the top of calendar entry\n
*                        toc_sync[6:2] defines the calendar memory row\n
*                        toc_sync[1:0] defines the entry within the row      
*   page_swap_mode     - Page swap with global pin or local register bit see
*                        scbs3_page_swap_mode_t                          
*                                                                                                                                                            
*******************************************************************************/
typedef struct mpmo_scbs3_cfg_struct_t {   
    BOOL scbs3_present;  
    UINT32 total_rate; 
    util_schd_chnl_rate_exp_t total_rate_units; 
    BOOL toc_sync_en;
    UINT32 toc_sync;
    scbs3_page_swap_mode_t page_swap_mode;   
} mpmo_scbs3_cfg_struct_t;


/*******************************************************************************
* STRUCTURE: mpmo_ch_cfg_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MPMO parameters specific to each channel
*
* ELEMENTS:
*   state                        - Channel state
*   client_bitrate               - Client rate in bits per second
*   pkt_period_int               - Integer packets per period
*   pkt_period_n                 - Decimal (numerator) pkt per period
*   pkt_period_d                 - Decimal (denomenator) pkt per period
*   pkt_size                     - Packet size
*   cn_base_int                  - Integer of Cn base
*   cn_base_n                    - Decimal (numerator) of cn base
*   cn_base_d                    - Decimal (denomenator) of cn base
*   frm_period_idx               - The frame counter select configuration index
*   frm_period_n                 - The frame generation period numerator
*   frm_period_d                 - The frame generation period denomenator
*   lpf_gain                     - The Low Pass Filter (LPF) gain
*   zone_cent_en                 - Zone based FIFO centering
*   golden_cn_cal_en             - Golden cn calculation
*   cn_generate_en               - Enable/disable DSP to generate Cn justification
*   xoff_thres                   - FIFO back-pressure threshold
*   cfc_num_pages                - Number of pages to allocate for the channel in the CFC
*   ghao_xoff_thresh_target      - GHAO traget FIFO back-pressure threshold
*   ghao_xoff_thresh_change_step - GHAO FIFO back-pressure threshold change step
*       
*******************************************************************************/
typedef struct mpmo_ch_cfg_struct_t {   
    mpmo_chnl_state_t           state;
    DOUBLE                      client_bitrate;
    UINT32                      pkt_period_int;
    UINT32                      pkt_period_n;
    UINT32                      pkt_period_d;
    UINT32                      pkt_size;
    UINT32                      cn_base_int;
    UINT32                      cn_base_n;
    UINT32                      cn_base_d;
    UINT32                      frm_period_idx;
    UINT32                      frm_period_n;
    UINT32                      frm_period_d;
    UINT32                      lpf_gain;
    BOOL                        zone_cent_en;
    BOOL                        golden_cn_cal_en;
    BOOL                        cn_generate_en;                                                          
    UINT32                      xoff_thres; 
    UINT32                      cfc_num_pages; 
    UINT32                      ghao_xoff_thresh_target;
    UINT32                      ghao_xoff_thresh_change_step;
} mpmo_ch_cfg_struct_t;


/*******************************************************************************
* STRUCTURE: mpmo_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Static MPMO module instance data
*
* ELEMENTS:
*   default_param   - MPMO default parameter see mpmo_default_param_struct_t
*   cfc_cfg         - CFC configuration parameters see mpmo_cfc_cfg_struct_t
*   scbs3_cfg       - SCBS3 configuration parameters see mpmo_scbs3_cfg_struct_t
*   num_mult        - Multiplier for the frame period numerator for changing the 
*                     DCI reference clock
*   denom_mult      - Multiplier for the frame period denomenator for changing the 
*                     DCI reference clock 
*
*       
*******************************************************************************/
typedef struct mpmo_cfg_t{
    mpmo_default_param_struct_t default_param;
    mpmo_cfc_cfg_struct_t       cfc_cfg;
    mpmo_scbs3_cfg_struct_t     scbs3_cfg;
    UINT32                      num_mult;
    UINT32                      denom_mult;
} mpmo_cfg_t;

/*******************************************************************************
* STRUCTURE: mpmo_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MPMO Module Instance dynamic data
*
* ELEMENTS:
*   ch_cfg_ptr   - pointer to channel specific configuration parameters
*       
*******************************************************************************/
typedef struct mpmo_var_t{
    mpmo_ch_cfg_struct_t   *ch_cfg_ptr;
} mpmo_var_t;


/*******************************************************************************
* STRUCTURE: mpmo_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MPMO Module Instance Handle
*
* ELEMENTS:
*       base                  - See pmc_handle_t
*       cfg                   - See mpmo_cfg_t
*       var                   - See mpmo_var_t
*       *scbs3_handle         - See scbs3_handle_t
*       *cfc_handle           - See cfc_handle_t
*       
*******************************************************************************/
typedef struct mpmo_handle_struct_t {
    pmc_handle_t base;
    mpmo_cfg_t cfg;
    mpmo_var_t var;
    scbs3_handle_t   *scbs3_handle;
    cfc_handle_t     *cfc_handle;
       
} mpmo_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC mpmo_handle_t *mpmo_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void mpmo_ctxt_destroy(mpmo_handle_t *mpmo_handle);
PUBLIC void mpmo_handle_init(mpmo_handle_t *mpmo_handle, 
                             mpmo_type_id_t mpmo_type_id,
                             UINT32 scbs3_toc_sync);
PUBLIC PMC_ERROR mpmo_init(mpmo_handle_t *hndl, 
                           pmc_energy_state_t energy_state);
PUBLIC BOOL mpmo_start_state_test(mpmo_handle_t *hndl); 
PUBLIC PMC_ERROR mpmo_ckctl_cfg(mpmo_handle_t *hndl, UINT32 ckctl_port, UINT32 chnl,
                                BOOL enable);
PUBLIC PMC_ERROR mpmo_ckctl_en_set(mpmo_handle_t *hndl, UINT32 ckctl_port, BOOL enable);
PUBLIC PMC_ERROR mpmo_enable_cfg(mpmo_handle_t *hndl, BOOL enable);
PUBLIC BOOL8 mpmo_enable_get(mpmo_handle_t *hndl);
PUBLIC PMC_ERROR mpmo_csi_normal_cfg(mpmo_handle_t *hndl, UINT32 csi_normal,
                                     BOOL csi_mask_en);                                     
PUBLIC PMC_ERROR mpmo_ch_prov(mpmo_handle_t *hndl, UINT32 chnl, DOUBLE client_bitrate, 
                              UINT32 pkt_size, UINT32 pkt_per_int, UINT32 pkt_per_n, UINT32 pkt_per_d,
                              util_opsa_cn_frame_period_t cn_frame_period, 
                              BOOL golden_cn_en);
PUBLIC PMC_ERROR mpmo_mapotn_ch_prov(mpmo_handle_t *hndl, UINT32 chnl,
                              DOUBLE client_bitrate,
                              UINT32 pkt_size, UINT32 pkt_per_int,
                              UINT32 pkt_per_n, UINT32 pkt_per_d,
                              util_opsa_cn_frame_period_t cn_frame_period,
                              BOOL golden_cn_en,
                              BOOL8 bmp_map_mode,
                              BOOL8 is_sonet);
PUBLIC PMC_ERROR mpmo_ch_deprov(mpmo_handle_t *hndl, UINT32 chnl);
PUBLIC PMC_ERROR mpmo_ch_activate(mpmo_handle_t *hndl, UINT32 chnl);
PUBLIC PMC_ERROR mpmo_ch_deactivate(mpmo_handle_t *hndl, UINT32 chnl);
PUBLIC PMC_ERROR mpmo_ch_enable_cfg(mpmo_handle_t *hndl, UINT32 chnl, BOOL enable);
PUBLIC void mpmo_ch_clean(mpmo_handle_t *hndl, UINT32 *chnl, UINT32 num_chnl);
PUBLIC void mpmo_ch_restore_clean(mpmo_handle_t *hndl, UINT32 *clean_chnl, UINT32 num_clean_chnl, UINT32 *restore_chnl, UINT32 num_restore_chnl);


PUBLIC PMC_ERROR mpmo_ch_latency_cfg(mpmo_handle_t *hndl, UINT32 chnl,
                                     DOUBLE client_bitrate, UINT32 pkt_size,
                                     UINT32 fifo_depth, BOOL hys_buffer_en, 
                                     UINT32 hys_buffer,
                                     UINT32 latency_s,
                                     UINT32 s_old_ratio,
                                     BOOL   odu_flex_client,
                                     UINT32 num_odu_flex);
                                    
PUBLIC PMC_ERROR mpmo_cn_frame_period_cfg(mpmo_handle_t *hndl, UINT32 chnl,
                                          util_opsa_cn_frame_period_t cn_frame_period);
PUBLIC PMC_ERROR mpmo_cn_frame_period_assign(mpmo_handle_t *hndl); 

PUBLIC PMC_ERROR mpmo_cfc_fifo_overflow_mon(mpmo_handle_t *hndl, UINT32 chnl);                                                                              
PUBLIC PMC_ERROR mpmo_ch_bw_cfg(mpmo_handle_t *hndl, UINT32 chnl);


PUBLIC PMC_ERROR mpmo_pkt_period_var_get(mpmo_handle_t *hndl, UINT32 chnl, 
                                         UINT32 *pkt_size_ptr,
                                         UINT32 *pkt_per_int_ptr,
                                         UINT32 *pkt_per_n_ptr, 
                                         UINT32 *pkt_per_d_ptr); 

PUBLIC PMC_ERROR mpmo_ch_threshold_cfg(mpmo_handle_t *hndl, UINT32 chnl);
PUBLIC UINT32 mpmo_xoff_thres_calc(mpmo_handle_t *hndl, UINT32 cfc_num_pages,
                                   BOOL8 bmp_map_mode); 

PUBLIC PMC_ERROR mpmo_adjust_dci_clk(mpmo_handle_t *hndl,
                                     UINT32 numer_mult,
                                     UINT32 denom_mult);
PUBLIC scbs3_handle_t* mpmo_scbs3_handle_get(mpmo_handle_t *hndl);
PUBLIC BOOL mpmo_ch_ssf_cn_center_get(mpmo_handle_t *hndl, UINT32 chnl);
PUBLIC PMC_ERROR mpmo_ch_pkt_cfg_get(mpmo_handle_t *hndl, 
                                     UINT32 chnl,
                                     UINT32 *pkt_size_ptr,
                                     UINT32 *cn_base_int_ptr, 
                                     UINT32 *cn_base_n_ptr, 
                                     UINT32 *cn_base_d_ptr,
                                     UINT32 *cn_frm_per_n_ptr, 
                                     UINT32 *cn_frm_per_d_ptr);

PUBLIC BOOL8 mpmo_ch_enable_get(mpmo_handle_t *hndl, UINT32 chnl); 
                                     
PUBLIC PMC_ERROR mpmo_ch_zone_cent_gain_cfg(mpmo_handle_t *hndl, UINT32 chnl,
                                            UINT32 ppm_offset);  
                                            
PUBLIC PMC_ERROR mpmo_coreotn_fo1_scbs3_chnl_deactivate(mpmo_handle_t *hndl, UINT32 chnl);   
PUBLIC PMC_ERROR mpmo_ch_activate_super_prov(mpmo_handle_t *hndl, UINT32 chnl); 

PUBLIC PMC_ERROR mpmo_ch_prov_for_maint_sig_insertion(mpmo_handle_t *hndl, UINT32 chnl,
                                                      DOUBLE client_bitrate,
                                                      UINT32 pkt_size, UINT32 pkt_per_int,
                                                      UINT32 pkt_per_n, UINT32 pkt_per_d,
                                                      util_opsa_cn_frame_period_t cn_frame_period,
                                                      BOOL golden_cn_en);

PUBLIC PMC_ERROR mpmo_ch_activate_for_maint_sig_insertion(mpmo_handle_t *hndl, UINT32 chnl); 
 
PUBLIC PMC_ERROR mpmo_ch_deactivate_for_maint_sig_insertion(mpmo_handle_t *hndl,
                                                            UINT32 chnl);


PUBLIC PMC_ERROR mpmo_scbs3_all_chnls_activate(mpmo_handle_t *hndl);  

PUBLIC PMC_ERROR mpmo_scbs3_all_chnls_deactivate(mpmo_handle_t *hndl); 

PUBLIC PMC_ERROR mpmo_ch_super_prov(mpmo_handle_t *hndl, UINT32 chnl,
                                    DOUBLE client_bitrate,
                                    UINT32 pkt_size, UINT32 pkt_per_int,
                                    UINT32 pkt_per_n, UINT32 pkt_per_d,
                                    util_opsa_cn_frame_period_t cn_frame_period,
                                    BOOL golden_cn_en,
                                    util_global_odukp_type_t odu_type);                                                                   

    
PUBLIC PMC_ERROR mpmo_ch_deprov_for_maint_sig_insertion(mpmo_handle_t *hndl,
                                                        UINT32 chnl);

PUBLIC PMC_ERROR mpmo_ch_deprov_super_deprov(mpmo_handle_t *hndl, 
                                             UINT32 chnl);  
                                             
PUBLIC PMC_ERROR mpmo_ch_deactivate_super_prov(mpmo_handle_t *hndl,
                                               UINT32 chnl);                                                                                                           

PUBLIC void mpmo_alloc_init(mpmo_handle_t *mpmo_handle);

PUBLIC PMC_ERROR mpmo_handle_restart_init(mpmo_handle_t *mpmo_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                          pmc_energy_state_t top_energy_state,
                                          BOOL8 ptr_init_only,
                                          BOOL8 * mismatch_mpmo_channels);
PUBLIC PMC_ERROR mpmo_raw_latency_get(mpmo_handle_t *mpmo_handle,
                                      UINT32 chnl,
                                      UINT32 *minimum_latency,
                                      UINT32 *maximum_latency);

PUBLIC PMC_ERROR mpmo_debug_data_get(mpmo_handle_t *mpmo_handle,
                                     UINT32 chnl,
                                     UINT32 *debug_pkt_age_ptr,
                                     UINT32 *debug_ramp_state_ptr,
                                     UINT32 *debug_curr_csi_ptr);

PUBLIC PMC_ERROR mpmo_ghao_fifo_resize(mpmo_handle_t *hndl,
                                       UINT32 chnl, 
                                       UINT32 oduflex_num_tribslots);

PUBLIC PMC_ERROR mpmo_ghao_fifo_resize_no_thresh_update(mpmo_handle_t *hndl,
                                                        UINT32 chnl, 
                                                        UINT32 oduflex_num_tribslots);

PUBLIC PMC_ERROR mpmo_schd_ch_reprov(mpmo_handle_t *hndl,
                                     UINT32 chnl, 
                                     DOUBLE client_bitrate);

PUBLIC PMC_ERROR mpmo_ghao_age_gain_set(mpmo_handle_t *hndl, 
                                        UINT32 chnl,
                                        UINT32 num_entries);

PUBLIC PMC_ERROR mpmo_bwr_ind_get(mpmo_handle_t *hndl, 
                                  UINT32 bwr_ind[3]);


PUBLIC PMC_ERROR mpmo_xoff_thresh_change(mpmo_handle_t *hndl, 
                                         UINT32 chnl,
                                         BOOL8 increase);

PUBLIC PMC_ERROR mpmo_xoff_thresh_change_delta(mpmo_handle_t *hndl, 
                                               UINT32 chnl,
                                               INT32 threshold_delta);


PUBLIC PMC_ERROR mpmo_set_custom_userbit_mask(mpmo_handle_t *hndl,
                                              UINT32 chnl_id,
                                              BOOL enable,
                                              UINT32 set_mask[3],
                                              UINT32 userbit_mask[3],
                                              BOOL8 config_update);


PUBLIC PMC_ERROR mpmo_ghao_bwr_setup_param_set(mpmo_handle_t *hndl,
                                               UINT32 chnl,
                                               UINT32 target_ts,
                                               BOOL8 increase);


PUBLIC PMC_ERROR mpmo_ghao_bwr_end_param_set(mpmo_handle_t *hndl,
                                             UINT32 chnl,
                                             DOUBLE odu_bitrate,
                                             UINT32 pkt_size,
                                             UINT32 pkt_per_int,
                                             UINT32 pkt_per_n,
                                             UINT32 pkt_per_d);

PUBLIC PMC_ERROR mpmo_ghao_new_cn_set(mpmo_handle_t *hndl,
                                      UINT32 chnl,
                                      BOOL program_cn_n_first,
                                      UINT32 cn_base_int,
                                      UINT32 cn_base_n,
                                      UINT32 cn_base_d);


PUBLIC PMC_ERROR mpmo_ghao_calculate_new_cn_max_cn_d(mpmo_handle_t *hndl,
                                                     UINT32 chnl,
                                                     DOUBLE client_bitrate,
                                                     UINT32 pkt_size,
                                                     UINT32 pkt_per_int,
                                                     UINT32 pkt_per_n,
                                                     UINT32 pkt_per_d,
                                                     UINT32 *cn_base_int,
                                                     UINT32 *cn_base_n,
                                                     UINT32 *cn_base_d,
                                                     BOOL *program_cn_n_first);

PUBLIC PMC_ERROR mpmo_int_chnl_enable(mpmo_handle_t *mpmo_handle,
                                      UINT32 chnl_id,
                                      mpmo_int_chnl_t *int_table_ptr,
                                      mpmo_int_chnl_t *int_en_table_ptr,
                                      BOOL8 enable);

PUBLIC PMC_ERROR mpmo_int_chnl_clear(mpmo_handle_t *mpmo_handle,
                                     UINT32 chnl_id,
                                     mpmo_int_chnl_t *int_table_ptr,
                                     mpmo_int_chnl_t *int_en_table_ptr);
    
PUBLIC PMC_ERROR mpmo_int_chnl_retrieve(mpmo_handle_t *mpmo_handle,
                                        UINT32 chnl_id,    
                                        mpmo_int_chnl_t *int_en_table_ptr,
                                        mpmo_int_chnl_t *int_table_ptr);

PUBLIC PMC_ERROR mpmo_int_chnl_enabled_check(mpmo_handle_t *mpmo_handle,
                                             UINT32 chnl_id,
                                             mpmo_int_chnl_t *int_en_table_ptr,
                                             BOOL8 *int_found_ptr); 

PUBLIC PMC_ERROR mpmo_calendar_pinch_mode_set(mpmo_handle_t *hndl,
                                              util_global_calendar_pinch_t cal_pinch_type);


PUBLIC PMC_ERROR mpmo_reprov_pkt_size_configs(mpmo_handle_t *hndl,
                                              UINT32 chnl,
                                              UINT32 pkt_size,
                                              UINT32 pkt_per_int,
                                              UINT32 pkt_per_n,
                                              UINT32 pkt_per_d);

PUBLIC DOUBLE mpmo_client_bitrate_get(mpmo_handle_t *hndl,
                                      UINT32 chnl);

PUBLIC void mpmo_ch_fifo_clear_set(mpmo_handle_t *hndl,
                                   UINT32         chnl, 
                                   BOOL8          reset);

#ifdef __cplusplus
}
#endif

#endif /* _MPMO_H */

/*
** end of file
*/
