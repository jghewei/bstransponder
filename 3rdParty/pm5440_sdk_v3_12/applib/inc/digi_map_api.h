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
*   DESCRIPTION : This file contains definitions for C functions for
*   configuring MAPOTN features.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _DIGI_MAP_API_H
#define _DIGI_MAP_API_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

/*#include "digi_api.h"*/


/*
** Enumerated Types
*/

/*
** Constants
*/

/* The number of GFP demapper frame length statistics buckets */
#define DIGI_MAPPER_GFP_STATS_MAX 7

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: digi_mapper_pmon_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Channel table for MAPOTN PMON
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    digi_pmon_mapotn_counts_t           counts;

    digi_enet_pmon_mode_t               enet_mode;
    digi_enet_pmon_chnl_t               enet;
} digi_mapper_pmon_chnl_t;



/*******************************************************************************
* STRUCTURE: digi_mapper_pmon_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Aggregation table for MAPOTN PMON
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    digi_mapper_chnl_t           *handle[DIGI_MAPOTN_CHNL_MAX];
    digi_mapper_pmon_chnl_t       mapper_chnl[DIGI_MAPOTN_CHNL_MAX];
} digi_mapper_pmon_t;



/*******************************************************************************
* STRUCTURE: digi_mapper_pmon_fo2_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Channel table for MAPOTN FO2 PMON
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef digi_otn_odu_pmon_chnl_t digi_mapper_pmon_fo2_chnl_t;


/*******************************************************************************
* STRUCTURE: digi_mapper_pmon_fo2_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Aggregation table for MAPOTN FO2 PMON
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    digi_mapper_chnl_t           *handle[DIGI_MAPOTN_CHNL_MAX];
    digi_mapper_pmon_fo2_chnl_t   mapper_fo2_chnl[DIGI_MAPOTN_CHNL_MAX];
} digi_mapper_pmon_fo2_t;


/*******************************************************************************
* STRUCTURE: digi_mapper_int_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Channel table for MAPOTN interrupts
*
* ELEMENTS:
*   status              - Mapotn SS interrupts table.
*   fo2                 - FO 2 interrupts table.
*   enet_etrans_valid   - when TRUE, enet_etrans field can be used.
*                         when FALSE, ignore content of field enet_etrans.
*   enet_etrans         - ENET ETRANS channel interrupts table.
*   enet_pmon_valid     - when TRUE, enet_pmon field can be used.
*                         when FALSE, ignore content of field enet_pmon
*   enet_pmon           - ENET enhanced Pmon channel interrupts table.
*   cpb                 - CPB channel interrupts table.
*******************************************************************************/
typedef struct {
    mapotn_int_chnl_t           status;
    digi_otn_odu_int_chnl_t     fo2;
    BOOL8                       enet_etrans_valid;
    enet_int_chnl_t             enet_etrans;
    BOOL8                       enet_pmon_valid;
    enet_int_chnl_t             enet_pmon;
    cpb_int_chnl_t              cpb;
} digi_mapper_int_chnl_t;



/*******************************************************************************
* STRUCTURE: digi_mapper_int_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Aggregation table for MAPOTN interrupts.
*
* ELEMENTS:
*   See typedef.
*******************************************************************************/
typedef struct {
    mapotn_int_t                mapotn;
    coreotn_int_framer_t        fo2;
    digi_mapper_chnl_t         *handle[DIGI_MAPOTN_CHNL_MAX];
    digi_mapper_int_chnl_t      mapper_chnl[DIGI_MAPOTN_CHNL_MAX];
} digi_mapper_int_t;



/*******************************************************************************
* ENUM: digi_mapper_int_summary_mask_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bitmask to indicate which fields to retrieve in digi_mapper_int_summary_t.
*
* ELEMENTS:
*   See typedef.
*
*******************************************************************************/
typedef enum {
    DIGI_MAPPER_INT_SUMMARY_MASK_ACTIVE_CHNL_NUM    = 0x01,
    DIGI_MAPPER_INT_SUMMARY_MASK_ACTIVE_CHNL        = 0x02,
} digi_mapper_int_summary_mask_t;


/*******************************************************************************
* STRUCTURE: digi_mapper_int_summary_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Interrupt summary information for mapper channels
*
* ELEMENTS:
*   mask                    - digi_mapper_int_summary_mask_t bitmask to
*                             indicate which fields have been returned
*
*   active_chnl_num         - Number of channels in mapper that have an active
*                             enabled interrupt
*
*   active_chnl             - Array of mapper channel handles that have an
*                             have an active enabled interrupt. This allows the user
*                             to iteratively call digi_mapper_int_chnl_filt_retrieve()
*                             with the provided channel handles.
*
*******************************************************************************/
typedef struct {
    UINT32               mask;

    UINT32               active_chnl_num;
    digi_mapper_chnl_t  *active_chnl[DIGI_MAPOTN_CHNL_MAX];
} digi_mapper_int_summary_t;


/*******************************************************************************
* STRUCTURE: digi_mapotn_gfp_enet_pause_filter_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the properties of Ethernet pause filter in the GFP demapper 
* 
*   Once pause filtering is enabled. The GFP demappper inspects incoming GFP\n
*   frames to determine whether it is a pause frame. An incoming frame is \n
*   classified as a PAUSE frame when the following conditions are satisfied:\n
*      1) PTI is equal to the configured PAUSE_PTI value\n
*      2) UPI is equal to the configured PAUSE_UPI value\n
*      3) Destination MAC address matches the configured Ethernet MAC destination \n
*         address.
*
*   Note that pause filtering is configured per channel with the exception of 
*   the Ethernet MAC destination address. A unique MAC address can is configured 
*   with the digi_mapper_gfp_pause_frm_mac_cfg API.
*
* ELEMENTS:
*   
*   pause_mode          - set if spd and preamble bytes exist\n
*                          1: Pause frame has a preamble. The length of the\n
*                             preamble is determined by the PREAMBLE_SFD_MODE \n
*                              register field.
*                          0: Pause frame does not have a preamble.
*   pause_upi           - Pause frame UPI value
*   pause_pti           - Pause frame PTI value
*   pause_filter        - defines whether to drop or forward pause frames to 
*                         egress interface\n
*                         0 -- Frames qualified as pause frames are forwarded to 
*                             egress interface\n
*                         1 -- Frames qualified as pause frames are not forwarded
*                             to egress interface\n
*
*******************************************************************************/
typedef struct digi_mapotn_gfp_enet_pause_filter_cfg_t {
    BOOL8 pause_filter;
    BOOL8 pause_mode;
    UINT8 pause_upi;
    UINT8 pause_pti;
} digi_mapotn_gfp_enet_pause_filter_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_mapotn_wan_pause_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the properties of ENET WAN flow control.
*   GFP mapper allows the configuration of four pause templates which should \n
*   at least define an XON pause frame and can additionally contains three OFF\n
*   pause frames with different PAUSE quanta field. These templates are shared\n
*   between channels and shall be initially configured with \n
*   digi_mapper_gfp_pause_frm_mem_cfg API before attempting the configuration\n
*   of WAN flow control.
*
* ELEMENTS:
*   
* xon_template_id  - Pause template id for XON frame
* xoff_template_id - Pause template id for XOFF frame
* buffer_mode   - Define if flow control is controlled from internal or \n
*                 external resource 
*  xon_xoff_threshold - xon/xoff threshold that triggers the generation of\n
*                       XOFF/XON pause frames. This value is only used when\n
*                       buffer_mode is set to DIGI_ENET_FLOW_CONTROL_BUFFER_LOCAL
*                       The DCPB is organized into blocks of 12288 bytes each\n
*                       (256 bursts of 48 bytes), with a maximum of 512 blocks.\n
*                       This threshold is provided in term of burst of 48 bytes \n
*                       and it shall represent a level that is less than the \n
*                       size of the allocated buffer in DCPB.\n
*******************************************************************************/
typedef struct digi_mapotn_wan_pause_cfg_t {
    mapotn_tgfpf_pause_frm_memory_t xon_template_id;
    mapotn_tgfpf_pause_frm_memory_t xoff_template_id;
    digi_enet_flow_control_buffer_mode_t buffer_mode;
    UINT32 xon_xoff_threshold;
} digi_mapotn_wan_pause_cfg_t;

/*******************************************************************************
* STRUCTURE: digi_mapper_gfp_stats_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining the GFP demapper frame length statistics counter 
*   configuration. There are 7 frame length ranges: \n
*       range 0: min_fl[0]  > length > max_fl[0]    \n
*       range 1: min_fl[1]  > length > max_fl[1]    \n
*       ...                                         \n
*       range 6: min_fl[6]  > length > max_fl[6]    \n
*   
*   Where 'length' is the size of the client frame payload area. \n
*
* ELEMENTS:
*   
* min_fl[DIGI_MAPPER_GFP_STATS_MAX] - minimum client frame length bucket \n
* max_fl[DIGI_MAPPER_GFP_STATS_MAX] - maximum client frame length bucket \n
*******************************************************************************/
typedef struct digi_mapper_gfp_stats_cfg_t {
    UINT16 min_fl[DIGI_MAPPER_GFP_STATS_MAX]; 
    UINT16 max_fl[DIGI_MAPPER_GFP_STATS_MAX]; 
} digi_mapper_gfp_stats_cfg_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC PMC_ERROR digi_mapper_gfp_tx_pti_set(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            UINT32 pti_value);                                           

PUBLIC PMC_ERROR digi_mapper_gfp_tx_upi_set(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            UINT32 upi_value); 

PUBLIC PMC_ERROR digi_mapper_ilkn_gfp_tx_upi_sel_set(digi_handle_t      *digi_handle,
                                                     digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                     BOOL8               ilkn_upi_sel);

PUBLIC PMC_ERROR digi_mapper_gfp_tx_exi_set(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            UINT32 exi_value);                                            

PUBLIC PMC_ERROR digi_mapper_gfp_tx_ext_header_set(digi_handle_t *digi_handle,
                                                   digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                   UINT32 ext_hdr_enbl,
                                                   UINT32 ext_hdr_len, 
                                                   UINT32 ext_hdr_mem[MAPOTN_TGFPF_EXTENSION_HEADER_REG_CNT]);

PUBLIC PMC_ERROR digi_mapper_gfp_rx_ext_header_set(digi_handle_t *digi_handle,
                                                   digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                   UINT32 ehec_chk,
                                                   UINT32 ehec_ecc,
                                                   UINT32 exi_val1, 
                                                   UINT32 exi_len1,    
                                                   UINT32 exi_val2, 
                                                   UINT32 exi_len2);                                                   

PUBLIC PMC_ERROR digi_mapper_gfp_tx_fcs_mode_set(digi_handle_t *digi_handle,
                                                 digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                 mapotn_fcs_mode_t fcs_mode);     

PUBLIC PMC_ERROR digi_mapper_gfp_rx_fcs_chk_mode_set(digi_handle_t *digi_handle,
                                                     digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                     UINT32 cnt_udf_fcs,
                                                     UINT32 fcs_chk_mode, 
                                                     UINT32 preamble_size);

PUBLIC PMC_ERROR digi_mapper_opu_tx_pt_set(digi_handle_t *digi_handle,
                                           digi_mapper_chnl_t *chnl_ctxt_ptr,
                                           UINT8 pt_val);  

PUBLIC PMC_ERROR digi_mapper_opu_rx_pt_set(digi_handle_t *digi_handle,
                                           digi_mapper_chnl_t *chnl_ctxt_ptr,
                                           UINT8 pt_val);                                                                                             
                                              
PUBLIC PMC_ERROR digi_mapper_gfp_client_classify_prov(digi_handle_t *digi_handle,
                                                      digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                      mapotn_rgfpf_cmf_dest_t cmf_dest,
                                                      mapotn_rgfpf_cmf_upi_filt_mode_t cmf_upi_filt_mode,
                                                      UINT32 upi_range_1_start, UINT32 upi_range_1_end, 
                                                      UINT32 upi_range_2_start, UINT32 upi_range_2_end);
                                                        
PUBLIC PMC_ERROR digi_mapper_gfp_classify_unprov(digi_handle_t *digi_handle,
                                                 digi_mapper_chnl_t *chnl_ctxt_ptr); 
                                                   
PUBLIC PMC_ERROR digi_mapper_gfp_user_client_classify_prov(digi_handle_t *digi_handle,
                                                           digi_mapper_chnl_t *chnl_ctxt_ptr, 
                                                           UINT32 frame_field_mask, 
                                                           mapotn_rgfpf_comparison_op_t pti_comp, 
                                                           UINT32 pti_value,
                                                           mapotn_rgfpf_comparison_op_t upi_comp, 
                                                           UINT32 upi_value,
                                                           mapotn_rgfpf_comparison_op_t exi_comp, 
                                                           UINT32 exi_value,
                                                           mapotn_rgfpf_comparison_op_t pfi_comp, 
                                                           UINT32 pfi_value,
                                                           mapotn_rgfpf_proc_seq_t proc_seq, 
                                                           mapotn_rgfpf_classify_actions_t action);
                                                           
PUBLIC PMC_ERROR digi_mapper_gfp_classify_cfg(digi_handle_t *digi_handle,
                                              digi_mapper_chnl_t *chnl_ctxt_ptr, 
                                              UINT32 filter_no,
                                              UINT32 frame_field_mask, 
                                              mapotn_rgfpf_comparison_op_t pti_comp, 
                                              UINT32 pti_value,
                                              mapotn_rgfpf_comparison_op_t upi_comp, 
                                              UINT32 upi_value,
                                              mapotn_rgfpf_comparison_op_t exi_comp, 
                                              UINT32 exi_value,
                                              mapotn_rgfpf_classify_actions_t action);

PUBLIC PMC_ERROR digi_mapper_gfp_classify_get(digi_handle_t *digi_handle,
                                              digi_mapper_chnl_t *chnl_ctxt_ptr, 
                                              UINT32 filter_no,
                                              UINT32 *frame_field_mask_ptr, 
                                              mapotn_rgfpf_comparison_op_t *pti_comp_ptr, 
                                              UINT32 *pti_value_ptr,
                                              mapotn_rgfpf_comparison_op_t *upi_comp_ptr, 
                                              UINT32 *upi_value_ptr,
                                              mapotn_rgfpf_comparison_op_t *exi_comp_ptr, 
                                              UINT32 *exi_value_ptr,
                                              mapotn_rgfpf_classify_actions_t *action_ptr);

PUBLIC PMC_ERROR digi_mapper_cpu_fifo_enable(digi_handle_t *digi_handle,
                                             digi_mapper_chnl_t *chnl_ctxt_ptr);                                                           

PUBLIC PMC_ERROR digi_mapper_gfp_xoff_pause_frm_insrt(digi_handle_t *digi_handle,
                                                      digi_mapper_chnl_t *chnl_ctxt_ptr);

PUBLIC PMC_ERROR digi_mapper_gfp_pause_frm_mem_cfg(digi_handle_t *digi_handle,
                                                   UINT32 pause_time[MAPOTN_TGFPF_PAUSE_FRAME_TEMPLATE_CNT],
                                                   UINT32 pause_frame_mem[MAPOTN_TGFPF_PAUSE_FRAME_MEM_TOTAL_REG_CNT]);                                                   

PUBLIC PMC_ERROR digi_mapper_gfp_pause_frm_mac_cfg(digi_handle_t *digi_handle,
                                                   UINT32 mac_da_lsb,
                                                   UINT32 mac_da_msb);
                                           
PUBLIC PMC_ERROR digi_mapper_gfp_frm_len_stat_cfg(digi_handle_t              *digi_handle,
                                                  digi_mapper_chnl_t          *chnl_handle,
                                                  digi_mapper_gfp_stats_cfg_t *gfp_stats_cfg_ptr);
                                                 
PUBLIC PMC_ERROR digi_mapper_null_frame_cfg(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            BOOL null_frame_en);

/*
** MAPOTN Getter methods
*/
PUBLIC PMC_ERROR digi_mapper_gfp_csf_insert_get(digi_handle_t *digi_handle,
                                                  digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                UINT32 *csf_insert);


PUBLIC PMC_ERROR digi_mapper_gfp_tx_pti_get(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            UINT32 *pti_value);                                       

PUBLIC PMC_ERROR digi_mapper_gfp_tx_upi_get(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            UINT32 *upi_value); 

PUBLIC PMC_ERROR digi_mapper_ilkn_gfp_tx_upi_sel_get(digi_handle_t      *digi_handle,
                                                     digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                     BOOL8              *ilkn_upi_sel);

PUBLIC PMC_ERROR digi_mapper_gfp_tx_exi_get(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            UINT32 *exi_value);                                             
                                            
PUBLIC PMC_ERROR digi_mapper_gfp_tx_ext_header_get(digi_handle_t *digi_handle,
                                                   digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                   UINT32 *ext_hdr_enbl,
                                                   UINT32 *ext_hdr_len); 

PUBLIC PMC_ERROR digi_mapper_gfp_rx_ext_header_get(digi_handle_t *digi_handle,
                                                   digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                   UINT32 *ehec_chk,
                                                   UINT32 *ehec_ecc,
                                                   UINT32 *exi_val1, 
                                                   UINT32 *exi_len1, 
                                                   UINT32 *exi_val2, 
                                                   UINT32 *exi_len2);                                                   
                                                
PUBLIC PMC_ERROR digi_mapper_gfp_tx_fcs_mode_get(digi_handle_t *digi_handle,
                                                 digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                 mapotn_fcs_mode_t *fcs_mode); 

PUBLIC PMC_ERROR digi_mapper_gfp_rx_fcs_chk_mode_get(digi_handle_t *digi_handle,
                                                     digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                     UINT32 *fcs_chk_mode);

PUBLIC PMC_ERROR digi_mapper_opu_tx_pt_get(digi_handle_t *digi_handle,
                                           digi_mapper_chnl_t *chnl_ctxt_ptr,
                                           UINT8 *pt); 

PUBLIC PMC_ERROR digi_mapper_opu_rx_accpt_pt_get(digi_handle_t *digi_handle,
                                                 digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                 UINT8 *accpt_pt_val);
                                                                                                                                                                                                                                                     
PUBLIC PMC_ERROR digi_mapper_opu_rx_exp_pt_get(digi_handle_t *digi_handle,
                                               digi_mapper_chnl_t *chnl_ctxt_ptr,
                                               UINT8 *exp_pt_val);
                                             
PUBLIC PMC_ERROR digi_mapper_tx_flow_control_set(digi_handle_t                      *digi_handle,
                                                 digi_mapper_chnl_t                 *digi_otn_mapper_prov,
                                                 digi_mapotn_wan_pause_cfg_t        *digi_mapotn_wan_pause_cfg,
                                                 BOOL8                               enable);
    
PUBLIC PMC_ERROR digi_mapper_tx_flow_control_get(digi_handle_t                      *digi_handle,
                                                 digi_mapper_chnl_t                 *digi_otn_mapper_prov,
                                                 digi_mapotn_wan_pause_cfg_t        *digi_mapotn_wan_pause_cfg,
                                                 BOOL8                              *enable);

PUBLIC PMC_ERROR digi_mapper_rx_flow_control_set(digi_handle_t                            *digi_handle,
                                                 digi_mapper_chnl_t                      *digi_otn_mapper_prov,
                                                 digi_mapotn_gfp_enet_pause_filter_cfg_t *digi_mapotn_gfp_enet_pause_filter_cfg,
                                                 BOOL8                                    enable);

PUBLIC PMC_ERROR digi_mapper_rx_flow_control_get(digi_handle_t                           *digi_handle,
                                                 digi_mapper_chnl_t                      *digi_otn_mapper_prov,
                                                 digi_mapotn_gfp_enet_pause_filter_cfg_t *digi_mapotn_gfp_enet_pause_filter_cfg,
                                                 BOOL8                                   *enable);

PUBLIC PMC_ERROR digi_mapper_pmon_retrieve(digi_handle_t       *digi_handle,
                                           digi_mapper_pmon_t  *pmon_table_ptr);

PUBLIC PMC_ERROR digi_mapper_pmon_fo2_retrieve(digi_handle_t          *digi_handle,
                                              digi_mapper_pmon_fo2_t  *pmon_table_ptr);

PUBLIC PMC_ERROR digi_mapper_pmon_chnl_get(digi_handle_t                *digi_handle,
                                           digi_mapper_chnl_t           *chnl_handle,
                                           digi_mapper_pmon_t           *pmon_table_ptr,
                                           digi_mapper_pmon_chnl_t     **pmon_chnl_table_pptr);

PUBLIC PMC_ERROR digi_mapper_pmon_fo2_chnl_get(digi_handle_t                *digi_handle,
                                               digi_mapper_chnl_t           *chnl_handle,
                                               digi_mapper_pmon_fo2_t       *pmon_table_ptr,
                                               digi_mapper_pmon_fo2_chnl_t **pmon_chnl_table_pptr);

PUBLIC PMC_ERROR digi_mapper_int_register_cb(digi_handle_t      *digi_handle,
                                             void               *token,
                                             UINT32              token_size,
                                             digi_int_cb_fcn     user_cb);

PUBLIC PMC_ERROR digi_mapper_int_chnl_register_cb(digi_handle_t      *digi_handle,
                                                  void               *token,
                                                  UINT32              token_size,
                                                  digi_int_cb_fcn     user_cb);

PUBLIC PMC_ERROR digi_mapper_int_fo2_chnl_register_cb(digi_handle_t      *digi_handle,
                                                      void               *token,
                                                      UINT32              token_size,
                                                      digi_int_cb_fcn     user_cb);

PUBLIC PMC_ERROR digi_mapper_int_enet_etrans_chnl_register_cb(digi_handle_t      *digi_handle,
                                                              void               *token,
                                                              UINT32              token_size,
                                                              digi_int_cb_fcn     user_cb);
    
PUBLIC PMC_ERROR digi_mapper_int_enet_pmon_chnl_register_cb(digi_handle_t      *digi_handle,
                                                            void               *token,
                                                            UINT32              token_size,
                                                            digi_int_cb_fcn     user_cb);

PUBLIC PMC_ERROR digi_mapper_int_mapotn_enable(digi_handle_t        *digi_handle,
                                               mapotn_int_t         *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_mapotn_disable(digi_handle_t        *digi_handle,
                                                mapotn_int_t         *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_fo2_enable(digi_handle_t           *digi_handle,
                                            coreotn_int_framer_t    *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_fo2_disable(digi_handle_t           *digi_handle,
                                             coreotn_int_framer_t    *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_enet_etrans_int_chnl_enable(digi_handle_t             *digi_handle,
                                                         digi_mapper_chnl_t        *chnl_handle,     
                                                         enet_int_chnl_t           *int_table_ptr);
    
PUBLIC PMC_ERROR digi_mapper_enet_etrans_int_chnl_disable(digi_handle_t             *digi_handle,
                                                          digi_mapper_chnl_t        *chnl_handle,
                                                          enet_int_chnl_t           *int_table_ptr);
    
PUBLIC PMC_ERROR digi_mapper_enet_etrans_int_chnl_clear(digi_handle_t             *digi_handle,
                                                        digi_mapper_chnl_t        *chnl_handle,   
                                                        enet_int_chnl_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_enet_pmon_int_chnl_enable(digi_handle_t             *digi_handle,
                                                       digi_mapper_chnl_t        *chnl_handle,   
                                                       enet_int_chnl_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_enet_pmon_int_chnl_disable(digi_handle_t             *digi_handle,
                                                        digi_mapper_chnl_t        *chnl_handle,     
                                                        enet_int_chnl_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_enet_pmon_int_chnl_clear(digi_handle_t             *digi_handle,
                                                      digi_mapper_chnl_t        *chnl_handle,
                                                      enet_int_chnl_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_mapotn_filt_retrieve(digi_handle_t     *digi_handle,
                                                      mapotn_int_t      *filt_table_ptr,
                                                      mapotn_int_t      *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_mapotn_clear(digi_handle_t      *digi_handle,
                                              mapotn_int_t       *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_fo2_clear(digi_handle_t           *digi_handle,
                                           coreotn_int_framer_t    *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_chnl_enable(digi_handle_t             *digi_handle,
                                             digi_mapper_chnl_t        *chnl_handle,   
                                             mapotn_int_chnl_t    *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_chnl_disable(digi_handle_t             *digi_handle,
                                              digi_mapper_chnl_t        *chnl_handle,   
                                              mapotn_int_chnl_t         *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_fo2_chnl_enable(digi_handle_t              *digi_handle,
                                                 digi_mapper_chnl_t         *chnl_handle,
                                                 coreotn_int_framer_chnl_t  *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_fo2_chnl_disable(digi_handle_t              *digi_handle,
                                                  digi_mapper_chnl_t         *chnl_handle,
                                                  coreotn_int_framer_chnl_t  *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_fo2_chnl_filt_retrieve(digi_handle_t              *digi_handle,
                                                        digi_mapper_chnl_t         *chnl_handle,
                                                        coreotn_int_framer_chnl_t  *framer_filt_table_ptr,
                                                        odu_rfrm_int_pid_chnl_t    *pid_filt_table_ptr,
                                                        digi_otn_odu_int_chnl_t    *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_retrieve(digi_handle_t       *digi_handle,
                                          digi_mapper_int_t   *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_filt_retrieve(digi_handle_t             *digi_handle,
                                               digi_mapper_int_chnl_t    *filt_table_ptr,   
                                               digi_mapper_int_t         *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_chnl_filt_retrieve(digi_handle_t           *digi_handle,
                                                    digi_mapper_chnl_t      *chnl_handle,
                                                    digi_mapper_int_chnl_t  *filt_table_ptr,
                                                    digi_mapper_int_chnl_t  *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_enet_etrans_int_chnl_filt_retrieve(digi_handle_t           *digi_handle,
                                                                digi_mapper_chnl_t      *chnl_handle,
                                                                enet_int_chnl_t         *filt_table_ptr,
                                                                enet_int_chnl_t         *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_enet_pmon_int_chnl_filt_retrieve(digi_handle_t           *digi_handle,
                                                              digi_mapper_chnl_t      *chnl_handle,
                                                              enet_int_chnl_t         *filt_table_ptr,
                                                              enet_int_chnl_t         *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_chnl_get(digi_handle_t              *digi_handle,
                                          digi_mapper_chnl_t         *chnl_handle,
                                          digi_mapper_int_t          *int_table_ptr,
                                          digi_mapper_int_chnl_t    **int_chnl_table_pptr);

PUBLIC PMC_ERROR digi_mapper_int_enabled_get(digi_handle_t       *digi_handle,
                                             digi_mapper_int_t   *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_clear(digi_handle_t             *digi_handle,
                                       digi_mapper_int_t         *int_table_ptr);


PUBLIC PMC_ERROR digi_mapper_int_chnl_clear(digi_handle_t          *digi_handle,
                                            digi_mapper_chnl_t     *chnl_handle,
                                            digi_mapper_int_chnl_t *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_enet_etrans_int_chnl_clear(digi_handle_t             *digi_handle,
                                                        digi_mapper_chnl_t        *chnl_handle,   
                                                        enet_int_chnl_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_enet_pmon_int_chnl_clear(digi_handle_t             *digi_handle,
                                                        digi_mapper_chnl_t        *chnl_handle,   
                                                        enet_int_chnl_t           *int_table_ptr);

PUBLIC PMC_ERROR digi_mapper_int_enabled_check(digi_handle_t      *digi_handle);

PUBLIC PMC_ERROR digi_mapper_int_chnl_enabled_check(digi_handle_t              *digi_handle,
                                                    digi_mapper_int_summary_t  *int_summary_ptr);

PUBLIC PMC_ERROR digi_mapper_int_fo2_chnl_enabled_check(digi_handle_t               *digi_handle,
                                                        digi_otn_odu_int_summary_t  *int_summary_ptr);

PUBLIC PMC_ERROR digi_mapper_enet_etrans_int_chnl_enabled_check(digi_handle_t              *digi_handle,
                                                                digi_mapper_int_summary_t  *int_summary_ptr);

PUBLIC PMC_ERROR digi_mapper_enet_pmon_int_chnl_enabled_check(digi_handle_t              *digi_handle,
                                                              digi_mapper_int_summary_t  *int_summary_ptr);

PUBLIC PMC_ERROR digi_mapper_int_summary_get(digi_handle_t              *digi_handle,
                                             UINT32                      summary_mask,
                                             digi_mapper_int_summary_t  *int_summary_ptr);
PUBLIC PMC_ERROR digi_mapper_dplm_to_ssf_set(digi_handle_t *digi_handle, 
                                             digi_mapper_chnl_t  *chnl_ctxt_ptr,
                                             BOOL8 enable);
PUBLIC PMC_ERROR digi_mapper_dplm_to_ssf_get(digi_handle_t *digi_handle, 
                                             digi_mapper_chnl_t  *chnl_ctxt_ptr,
                                             BOOL8 *enable);
PUBLIC PMC_ERROR digi_mapper_sf_to_ssf_set(digi_handle_t *digi_handle, 
                                           digi_mapper_chnl_t  *chnl_ctxt_ptr,
                                           BOOL8 enable);
PUBLIC PMC_ERROR digi_mapper_sf_to_ssf_get(digi_handle_t *digi_handle, 
                                           digi_mapper_chnl_t  *chnl_ctxt_ptr,
                                           BOOL8 *enable);
PUBLIC PMC_ERROR digi_mapper_dcsf_to_ssf_set(digi_handle_t *digi_handle, 
                                             digi_mapper_chnl_t  *chnl_ctxt_ptr,
                                             BOOL8 enable);
PUBLIC PMC_ERROR digi_mapper_dcsf_to_ssf_get(digi_handle_t *digi_handle, 
                                             digi_mapper_chnl_t  *chnl_ctxt_ptr,
                                             BOOL8 *enable);

PUBLIC PMC_ERROR digi_mapper_gfp_tx_fcs_corrupt_set(digi_handle_t       *digi_handle, 
                                                    digi_mapper_chnl_t  *chnl_ctxt_ptr,
                                                    BOOL8                enable);

PUBLIC BOOL8 digi_mapper_gfp_tx_fcs_corrupt_get(digi_handle_t       *digi_handle, 
                                                digi_mapper_chnl_t  *chnl_ctxt_ptr);

PUBLIC PMC_ERROR digi_mapper_gfp_frm_len_stat_cfg_get(digi_handle_t               *digi_handle,
                                                      digi_mapper_chnl_t          *chnl_handle,
                                                      digi_mapper_gfp_stats_cfg_t *gfp_stats_cfg_ptr);

PUBLIC PMC_ERROR digi_mapper_gfp_frm_len_cfg(digi_handle_t              *digi_handle,
                                             digi_mapper_chnl_t    *chnl_handle,
                                             UINT16                 max_frm_len,
                                             UINT16                 min_frm_len);

PUBLIC PMC_ERROR digi_mapper_null_frame_get(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            BOOL *null_frame_en);

PUBLIC PMC_ERROR digi_mapper_gfp_csf_status_get(digi_handle_t   *digi_handle,
                                                UINT32          *valid,
                                                UINT32          *chid,
                                                UINT32          *upi);



#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */ 

#ifdef PMC_DEPRECATED

/* LCOV_EXCL_START */
PUBLIC PMC_ERROR digi_mapper_ordered_set_upi_cfg(digi_handle_t  *digi_handle,
                                                 UINT8 tx_upi_val, UINT8 rx_upi_val);

PUBLIC PMC_ERROR digi_mapper_ordered_set_upi_get(digi_handle_t *digi_handle,
                                                 UINT8 *tx_upi,
                                                 UINT8 *rx_upi);
/* LCOV_EXCL_STOP */

#endif /* PMC_DEPRECATED */

#endif/*#ifndef DOXYGEN_PUBLIC_ONLY*/

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_MAP_API_H */

/*
** end of file
*/
