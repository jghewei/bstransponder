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

#ifndef _MPMA_H
#define _MPMA_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "mpma_api.h"
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
* ENUM: mpma_type_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying block instance type.
*
* ELEMENTS:
*    MPMA_COREOTN              - Sourced from: Rx Framer/Demux;  Pushing to: ODUk
*    MPMA_COREOTN_FROM_MAPOTN  - Sourced from: MAPOTN Tx Framer; Pushing to: ODUk
*    MPMA_COREOTN_CTL          - Sourced from: COREOTN_CTL;      Pushing to: ODUk
*    MPMA_MAPOTN               - Sourced from: Rx Demapper;      Pushing to: CPB
*    MPMA_ENET                 - Sourced from: ENET;             Pushing to: CPB
*    MPMA_CBRC                 - Sourced from: CBRC;             Pushing to: CPB
*    MPMA_ENET_LINE_DB         - Sourced from: ENET_LINE in DB mode; Pushing to: CPB
*    MPMA_ENET_SYS_DB          - Sourced from: ENET_SYS in DB mode;  Pushing to: CPB
*
*******************************************************************************/
typedef enum {
    MPMA_COREOTN              = 0x00,
    MPMA_COREOTN_FROM_MAPOTN  = 0x01,
    MPMA_COREOTN_CTL          = 0x02,
    MPMA_MAPOTN               = 0x03,
    MPMA_ENET                 = 0x04, 
    MPMA_CBRC                 = 0x05,
    MPMA_ENET_LINE_DB         = 0x06,
    MPMA_ENET_SYS_DB          = 0x07,
    MPMA_NUM_TYPES
} mpma_type_id_t;


/*******************************************************************************
* ENUM: mpma_csi_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for client status indication.
*
* ELEMENTS:
*    MPMA_CSI_NORMAL     - Normal client status indication
*    MPMA_CSI_SSF        - SSF client status indication
*    MPMA_CSI_SF         - SF client status indication
*    MPMA_CSI_SD         - SD client status indication
*    MPMA_CSI_FORCE_TO   - Force TO client status indication
*    MPMA_CSI_FORCE_AWAY - Force Away client status indication
*
*******************************************************************************/
typedef enum {
    MPMA_CSI_NORMAL     = 0x00,
    MPMA_CSI_SSF        = 0x01,
    MPMA_CSI_SF         = 0x02,
    MPMA_CSI_SD         = 0x03,
    MPMA_CSI_FORCE_TO   = 0x04,
    MPMA_CSI_FORCE_AWAY = 0x05
} mpma_csi_type_t;

/*******************************************************************************
* ENUM: mpma_chnl_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for Channel State Definition.
*
* ELEMENTS:
*    MPMA_CHNL_START        - Start state
*    MPMA_CHNL_EQUIPPED     - Equipped state
*    MPMA_CHNL_OPERATIONAL  - Operational state
*
*******************************************************************************/
typedef enum {
    MPMA_CHNL_START         = 0,
    MPMA_CHNL_EQUIPPED      = 1,
    MPMA_CHNL_OPERATIONAL   = 2
} mpma_chnl_state_t;


/*******************************************************************************
* ENUM: mpma_resize_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for Bandwidth Resize Type.
*
* ELEMENTS:
*    MPMA_CHNL_INCREASE_BW     - Increase bandwidth
*    MPMA_CHNL_DECREASE_BW     - Decrease bandwidth
*
*******************************************************************************/
typedef enum {
    MPMA_CHNL_INCREASE_BW   = 0,
    MPMA_CHNL_DECREASE_BW   = 1
} mpma_resize_type_t;


/* 
 * DSP_MODE Definition:
 * Used by MPMA_MAPOTN:
 * - MPMA_CN_MODE_B_C - B + C mode: source node reference clock timing 
 * - MPMA_CN_MODE_C   - C mode: local timing 
 * Used by MPMA_COREOTN:
 * - MPMA_CND_MODE    - DSP uses the client_CnD input
 */
/*******************************************************************************
* ENUM: mpma_dsp_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for DSP_MODE definition.
*
* ELEMENTS:
*    MPMA_CN_MODE_B_C     - B + C mode: source node reference clock timing
*    MPMA_CN_MODE_C       - C mode: local timing
*    MPMA_CND_MODE        - DSP uses the client_CnD input
*    MPMA_DSP_MODE_UNUSED - default mode
*
*******************************************************************************/
typedef enum {
    MPMA_CN_MODE_B_C     = 0,
    MPMA_CN_MODE_C       = 1,   /* never used */
    MPMA_CND_MODE        = 2,
    MPMA_DSP_MODE_UNUSED = 3    
} mpma_dsp_mode_t;

/*
** Constants
*/
#define MPMA_PKT_SIZE_MIN                1
#define MPMA_PKT_SIZE_MAX              511
#define MPMA_DEFAULT_PKT_SIZE          143

/*
** Macro Definitions
*/
#define MPMA_INT_TABLE_DEFINE() \
    MPMA_INT_RANGE(mpma_handle, mpma, lfield_range, PSD_FIFO_OVR , psd_fifo_ovr_i     ); \
    MPMA_INT_RANGE(mpma_handle, mpma, lfield_range, PSD_FIFO_UDR , psd_fifo_udr_i     ); \

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: mpma_default_param_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MPMA parameters specific to each instantiation
*
* ELEMENTS:
*   mpma_type_id                      - MPMA type ID see mpma_type_id_t
*   num_chnl                          - number of supported channels
*   bus_width                         - bus width on the ingress DCI interface
*   lowest_bw                         - lowest supported bandwidth
*   ts_enable                         - enable/disable timestamp insertion feature
*   mkr_mode                          - packet sizing mode 
*   cn_bit_mode                       - Cn bit mode 1: Cn in bits 0: Cn in bytes
*   schd_pacer_en                     - scheduling pacer enable
*   pkt_size_range                    - packet size range
*   cn_frame_period                   - see util_opsa_cn_frame_period_t
*   cn_mode                           - consumption mode of the packet maker in Cn mode
*   cn_enable                         - Enable/disable phase measurement from the CLIENT_CN input
*   cnd_enable                        - Enable/disable the packet maker to monitor and use the CLIENT_CND input
*   cal_entries_alloc_method          - Calendar entries distribution algorithm number
*   db_id                             - database ID
*   db_handle                         - database handle
*   bwr_ind_sel                       - bandwidth resizing indication
*       
*******************************************************************************/
typedef struct mpma_default_param_struct_t {  
    mpma_type_id_t    mpma_type_id;
    UINT32            num_chnl;              
    UINT32            bus_width;             
    DOUBLE            lowest_bw;                           
    BOOL              ts_enable;            
    UINT32            mkr_mode;        
    UINT32            cn_bit_mode;
    BOOL              schd_pacer_en;
    UINT32            pkt_size_range;
    util_opsa_cn_frame_period_t cn_frame_period;
    UINT32            cn_mode;   
    BOOL              cn_enable;
    BOOL              cnd_enable;                                                           
    UINT32            cal_entries_alloc_method;   
    util_gen_db_id_t  db_id;                    
    UINT32            db_handle;
    UINT32            bwr_ind_sel;
} mpma_default_param_struct_t;

/*******************************************************************************
* STRUCTURE: mpma_cfc_cfg_struct_t                                         
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
typedef struct mpma_cfc_cfg_struct_t {   
    UINT32 max_fifo_num;   
    UINT32 page_size;      
} mpma_cfc_cfg_struct_t;

/*******************************************************************************
* STRUCTURE: mpma_scbs3_cfg_struct_t                                         
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
*   toc_sync           - Entry that corresponds to the top of calendar entry \n
*                        toc_sync[6:2] defines the calendar memory row\n
*                        toc_sync[1:0] defines the entry within the row    
*   page_swap_mode     - Page swap with global pin or local register bit see
*                        scbs3_page_swap_mode_t                          
*                                                                                                                                                            
*******************************************************************************/
typedef struct mpma_scbs3_cfg_struct_t {   
    BOOL                      scbs3_present;    
    UINT32                    total_rate;       
    util_schd_chnl_rate_exp_t total_rate_units; 
    BOOL                      toc_sync_en;      
    UINT32                    toc_sync;        
    scbs3_page_swap_mode_t    page_swap_mode;   
} mpma_scbs3_cfg_struct_t;

/*******************************************************************************
* STRUCTURE: mpma_ch_cfg_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MPMA parameters specific to each channel
*
* ELEMENTS:
*   state                        - Channel state
*   client_bitrate               - Client rate in bits per second
*   t_frame_n                    - Numerator of the frame period
*   t_frame_d                    - Denominator of the frame period
*   t_frame                      - frame period
*   pkt_period_int               - Integer packets per period
*   pkt_period_n                 - Decimal (numerator) pkt per period
*   pkt_period_d                 - Decimal (denomenator) pkt per period
*   pkt_size                     - Packet size
*   pkts_per_cn                  - the packets per Cn input
*   cn_base                      - the base value of the Cn 
*   mpma_gain                    - the gain of the packet maker
*   epsilon                      - minimum epsilon
*   n_ratio                      - ratio of t_frame / packet period
*   pkt_period_calc              - see util_opsa_pkt_period_calc_t
*   xoff_thres                   - FIFO back-pressure threshold
*   cfc_num_pages                - Number of pages to allocate for the channel in the CFC
*   sch_only                     - TRUE: channel uses the light version of MPMA with scheduler only. 
*                                  FALSE: full MPMA version
*       
*******************************************************************************/
typedef struct mpma_ch_cfg_struct_t {   
    mpma_chnl_state_t           state;        
    DOUBLE                      client_bitrate;
    UINT32                      t_frame_n;
    UINT32                      t_frame_d;
    DOUBLE                      t_frame;
    UINT32                      pkt_period_int;
    UINT32                      pkt_period_n;
    UINT32                      pkt_period_d;
    UINT32                      pkt_size;
    UINT32                      pkts_per_cn;
    UINT32                      cn_base;
    UINT32                      mpma_gain;
    DOUBLE                      epsilon;
    DOUBLE                      n_ratio;
    util_opsa_pkt_period_calc_t pkt_period_calc;
    UINT32                      xoff_thres;      
    UINT32                      cfc_num_pages;
    BOOL                        sch_only; 
} mpma_ch_cfg_struct_t;

/*******************************************************************************
* STRUCTURE: mpma_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Static MPMA module instance data
*
* ELEMENTS:
*   default_param   - MPMA default parameter see mpma_default_param_struct_t
*   cfc_cfg         - CFC configuration parameters see mpma_cfc_cfg_struct_t
*   scbs3_cfg       - SCBS3 configuration parameters see mpma_scbs3_cfg_struct_t
*       
*******************************************************************************/
typedef struct mpma_cfg_t{ 
    mpma_default_param_struct_t default_param;
    mpma_cfc_cfg_struct_t       cfc_cfg;
    mpma_scbs3_cfg_struct_t     scbs3_cfg;
} mpma_cfg_t;

/*******************************************************************************
* STRUCTURE: mpma_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MPMA Module Instance dynamic data
*
* ELEMENTS:
*   ch_cfg_ptr   - pointer to channel specific configuration parameters
*       
*******************************************************************************/
typedef struct mpma_var_t{
    mpma_ch_cfg_struct_t   *ch_cfg_ptr;
} mpma_var_t;


/*******************************************************************************
* STRUCTURE: mpma_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MPMA Module Instance Handle
*
* ELEMENTS:
*       base                  - See pmc_handle_t
*       cfg                   - See mpma_cfg_t
*       var                   - See mpma_var_t
*       *scbs3_handle         - See scbs3_handle_t
*       *cfc_handle           - See cfc_handle_t
*       
*******************************************************************************/
typedef struct mpma_handle_struct_t {
    pmc_handle_t base;
    mpma_cfg_t cfg;
    mpma_var_t var;
    scbs3_handle_t   *scbs3_handle;
    cfc_handle_t     *cfc_handle;
} mpma_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC mpma_handle_t *mpma_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void mpma_ctxt_destroy(mpma_handle_t *mpma_handle);
PUBLIC void mpma_handle_init(mpma_handle_t *mpma_handle, 
                             mpma_type_id_t mpma_type_id, UINT32 scbs3_toc_sync);
PUBLIC PMC_ERROR mpma_init(mpma_handle_t *hndl, 
                           pmc_energy_state_t energy_state);
PUBLIC BOOL mpma_start_state_test(mpma_handle_t *hndl); 
PUBLIC PMC_ERROR mpma_enable_cfg(mpma_handle_t *hndl, BOOL enable);
PUBLIC BOOL8 mpma_enable_get(mpma_handle_t *hndl);

PUBLIC PMC_ERROR mpma_csi_cfg(mpma_handle_t *hndl, mpma_csi_type_t csi_type, UINT32 csi_val);

PUBLIC PMC_ERROR mpma_ch_prov(mpma_handle_t *hndl, UINT32 chnl,
                              DOUBLE client_bitrate,
                              UINT32 pkt_size, 
                              util_opsa_cn_frame_period_t cn_frame_period,
                              util_opsa_pkt_period_calc_t pkt_period_calc, 
                              UINT32 scbs3_user_bits_3_0, BOOL sch_only, 
                              UINT32 pkt_per_int, UINT32 pkt_per_n,
                              UINT32 pkt_per_d, DOUBLE epsilon);
PUBLIC PMC_ERROR mpma_fo2_ch_prov(mpma_handle_t *hndl, UINT32 chnl,
                                  DOUBLE client_bitrate,
                                  UINT32 pkt_size, 
                                  util_opsa_cn_frame_period_t cn_frame_period,
                                  util_opsa_pkt_period_calc_t pkt_period_calc, 
                                  UINT32 scbs3_user_bits_3_0, BOOL sch_only, 
                                  UINT32 pkt_per_int, UINT32 pkt_per_n,
                                  UINT32 pkt_per_d, DOUBLE epsilon,
                                  BOOL8 bmp_map_mode);                                                          
PUBLIC PMC_ERROR mpma_ch_deprov(mpma_handle_t *hndl, UINT32 chnl);
PUBLIC PMC_ERROR mpma_ch_activate(mpma_handle_t *hndl, UINT32 chnl, BOOL dsp_datapath);
PUBLIC PMC_ERROR mpma_ch_activate_super_prov(mpma_handle_t *hndl, UINT32 chnl, BOOL dsp_datapath);
PUBLIC PMC_ERROR mpma_ch_deactivate(mpma_handle_t *hndl, UINT32 chnl);
PUBLIC PMC_ERROR mpma_ch_deactivate_super_prov(mpma_handle_t *hndl, UINT32 chnl);
PUBLIC PMC_ERROR mpma_scbs3_all_chnls_deactivate(mpma_handle_t *hndl);
PUBLIC void mpma_ch_clean(mpma_handle_t *hndl, UINT32 *chnl, UINT32 num_chnl);
PUBLIC PMC_ERROR mpma_schd_ch_reprov(mpma_handle_t *hndl, UINT32 chnl, 
                                     DOUBLE client_bitrate);
PUBLIC scbs3_handle_t* mpma_scbs3_handle_get(mpma_handle_t *hndl);

/* Packet size(B) and Packet period(T) calculators */
PUBLIC PMC_ERROR mpma_pkt_period_calc(mpma_handle_t *hndl, 
                                      DOUBLE client_bitrate, 
                                      UINT32 pkt_size, 
                                      UINT32 pkt_size_range,
                                      util_opsa_cn_frame_period_t cn_frame_period,
                                      util_opsa_pkt_period_calc_t pkt_period_calc, 
                                      UINT32 *pkt_size_ptr,
                                      UINT32 *pkt_per_int_ptr,
                                      UINT32 *pkt_per_n_ptr, 
                                      UINT32 *pkt_per_d_ptr,
                                      DOUBLE *epsilon_ptr); 
                                      
PUBLIC PMC_ERROR mpma_oif_pkt_period_calc(mpma_handle_t *hndl, 
                                          DOUBLE client_bitrate, 
                                          UINT32 pkt_size, 
                                          UINT32 header_size,
                                          util_opsa_pkt_period_calc_t pkt_period_calc, 
                                          UINT32 *pkt_size_ptr,
                                          UINT32 *pkt_per_int_ptr,
                                          UINT32 *pkt_per_n_ptr, 
                                          UINT32 *pkt_per_d_ptr,
                                          DOUBLE *epsilon_ptr);    

PUBLIC PMC_ERROR mpma_pkt_period_var_get(mpma_handle_t *hndl, UINT32 chnl, 
                                         UINT32 *pkt_size_ptr,
                                         UINT32 *pkt_per_int_ptr,
                                         UINT32 *pkt_per_n_ptr, 
                                         UINT32 *pkt_per_d_ptr,
                                         DOUBLE *epsilon_ptr); 

PUBLIC PMC_ERROR mpma_schd_pacer_enable_cfg(mpma_handle_t *hndl, BOOL8 enable);
                                                                                                                                                    

PUBLIC PMC_ERROR mpma_pkt_period_var_set(mpma_handle_t *hndl, UINT32 chnl, 
                                         DOUBLE client_bitrate, 
                                         util_opsa_cn_frame_period_t cn_frame_period,
                                         util_opsa_pkt_period_calc_t pkt_period_calc, 
                                         UINT32 pkt_size,
                                         UINT32 pkt_per_int,
                                         UINT32 pkt_per_n, 
                                         UINT32 pkt_per_d, DOUBLE epsilon,
                                         UINT32 corner_frequency);

PUBLIC PMC_ERROR mpma_ch_bw_cfg(mpma_handle_t *hndl, UINT32 chnl);

PUBLIC UINT32 mpma_cfc_num_pages_calc(mpma_handle_t *hndl, DOUBLE client_bitrate);

PUBLIC PMC_ERROR mpma_scbs3_all_chnls_activate(mpma_handle_t *hndl);


PUBLIC void mpma_ch_pkt_cfg_get(mpma_handle_t *hndl, 
                                UINT32 chnl,
                                UINT32 *pkt_size_ptr,
                                UINT32 *pkts_per_cn,
                                UINT32 *cn_base);
PUBLIC BOOL8 mpma_ch_enable_get(mpma_handle_t *hndl, UINT32 chnl);

PUBLIC void mpma_alloc_init(mpma_handle_t *mpma_handle);

PUBLIC PMC_ERROR mpma_handle_restart_init(mpma_handle_t *mpma_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                          pmc_energy_state_t top_energy_state,
                                          BOOL8 ptr_init_only,
                                          BOOL8 * mismatch_mpma_channels);

PUBLIC PMC_ERROR mpma_ch_enable_cfg(mpma_handle_t *hndl, UINT32 chnl, BOOL enable);

PUBLIC PMC_ERROR mpma_dsp_ch_enable_cfg(mpma_handle_t *hndl,
                                        UINT32 chnl,
                                        BOOL enable);

PUBLIC PMC_ERROR mpma_ghao_fifo_resize(mpma_handle_t *hndl,
                                       UINT32 chnl, 
                                       UINT32 oduflex_num_tribslots);


PUBLIC PMC_ERROR mpma_ghao_bwr_end_param_set(mpma_handle_t *hndl,
                                             UINT32 chnl,
                                             DOUBLE odu_bitrate,
                                             util_opsa_cn_frame_period_t cn_frame_period,
                                             UINT32 pkt_size,
                                             UINT32 pkt_per_int,
                                             UINT32 pkt_per_n,
                                             UINT32 pkt_per_d,
                                             DOUBLE eps_loc);

PUBLIC PMC_ERROR mpma_ghao_bwr_ind_set(mpma_handle_t *hndl,
                                       UINT32 chnl,
                                       UINT32 bwr_ind);

PUBLIC PMC_ERROR mpma_int_chnl_enable(mpma_handle_t *mpma_handle,
                                      UINT32 chnl_id,
                                      mpma_int_chnl_t *int_table_ptr,
                                      mpma_int_chnl_t *int_en_table_ptr,
                                      BOOL8 enable);

PUBLIC PMC_ERROR mpma_int_chnl_clear(mpma_handle_t *mpma_handle,
                                     UINT32 chnl_id,
                                     mpma_int_chnl_t *int_table_ptr,
                                     mpma_int_chnl_t *int_en_table_ptr);
    
PUBLIC PMC_ERROR mpma_int_chnl_retrieve(mpma_handle_t *mpma_handle,
                                        UINT32 chnl_id,    
                                        mpma_int_chnl_t *int_en_table_ptr,
                                        mpma_int_chnl_t *int_table_ptr);

PUBLIC void mpma_int_chnl_pack_registers(mpma_handle_t *mpma_handle,
                                         UINT32 sector,
                                         UINT32 reg_addrs[],
                                         UINT32 *index);


PUBLIC PMC_ERROR mpma_int_chnl_enabled_check(mpma_handle_t *mpma_handle,
                                             UINT32 chnl_id,
                                             mpma_int_chnl_t  *int_en_table_ptr,
                                             BOOL8 *int_found_ptr);        
    


#ifdef __cplusplus
}
#endif

#endif /* _MPMA_H */

/*
** end of file
*/
