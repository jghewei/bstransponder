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

#ifndef _ODUKSC_H
#define _ODUKSC_H

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
#include "oduksc_api.h"
#include "scbs3.h"
#include "scbs3_api.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: oduksc_type_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying block instance type.
*
* ELEMENTS:
*    ODUKSC_COREOTN_ODUKSC1  - COREOTN ODUKSC1
*    ODUKSC_COREOTN_ODUKSC2  - COREOTN ODUKSC2
*    ODUKSC_MAPOTN           - MAPOTN
*    ODUKSC_LINEOTN          - LINEOTN
*    ODUKSC_COREOTN_ODUJAT1  - COREOTN ODUJAT1
*    ODUKSC_COREOTN_ODUJAT2  - COREOTN ODUJAT2
*
*******************************************************************************/
typedef enum {
    ODUKSC_COREOTN_ODUKSC1    = 0x00,
    ODUKSC_COREOTN_ODUKSC2    = 0x01,  
    ODUKSC_MAPOTN             = 0x02,  
    ODUKSC_LINEOTN            = 0x03,
    ODUKSC_COREOTN_ODUJAT1    = 0x04,
    ODUKSC_COREOTN_ODUJAT2    = 0x05,
} oduksc_type_id_t;

/*******************************************************************************
* ENUM: oduksc_chnl_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for Channel State Definition.
*
* ELEMENTS:
*    ODUKSC_CHNL_START                 - Start state
*    ODUKSC_CHNL_PROVISIONED           - Provisioned state
*    ODUKSC_CHNL_PROVISION_PENDING     - Provision pending state
*    ODUKSC_CHNL_DEPROVISION_PENDING   - Deprovision pending state
*    ODUKSC_CHNL_ACTIVATED             - Activated state
*    ODUKSC_CHNL_REPROV_PENDING        - Reprovisioned pending state
*    ODUKSC_CHNL_REPROV_NO_RST_PENDING - Reprovisioned with no reset peding state
*
*******************************************************************************/
typedef enum
{
    ODUKSC_CHNL_START                = 0,
    ODUKSC_CHNL_PROVISIONED          = 1,
    ODUKSC_CHNL_PROVISION_PENDING    = 2,
    ODUKSC_CHNL_DEPROVISION_PENDING  = 3,
    ODUKSC_CHNL_ACTIVATED            = 4, 
    ODUKSC_CHNL_REPROV_PENDING       = 5,
    ODUKSC_CHNL_REPROV_NO_RST_PENDING = 6
} oduksc_chnl_state_t;

/*******************************************************************************
* ENUM: oduksc_ri_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for Ramp interval state.
*
* ELEMENTS:
*    RI_AVAILABLE    - Ramp interval available 
*    RI_UNAVAILABLE  - Ramp interval unavailable 
*
*******************************************************************************/
typedef enum
{
    RI_AVAILABLE,
    RI_UNAVAILABLE,
} oduksc_ri_state_t;


/*
** Constants
*/

#define ODUKSC_RI_NUM                   6

/*
** Macro Definitions
*/
/* ODUKSC_INT(handle, tsb_name, tsb_field, struct_field) */
#define ODUKSC_INT_TABLE_DEFINE()                                  \
    ODUKSC_INT(oduksc_handle, oduksc, ACC_UNDF, acc_undf_i);       \
    ODUKSC_INT(oduksc_handle, oduksc, ACC_OVF, acc_ovf_i);         \
    
/* ODUKSC_INT_CHNL(handle, tsb_name, tsb_field, struct_field) */
#define ODUKSC_INT_CHNL_TABLE_DEFINE()                                  \
    ODUKSC_INT_CHNL_RANGE(oduksc_handle, oduksc, SRC_ZONE, src_zone_i); \
    ODUKSC_INT_CHNL_RANGE(oduksc_handle, oduksc, RAMP_DONE, ramp_done_i); \
    
/* ODUKSC_INT_CHNL(handle, tsb_name, tsb_field, struct_field) */
#define ODUKSC_STATUS_CHNL_TABLE_DEFINE()                               \
    ODUKSC_STATUS_CHNL_RANGE(oduksc_handle, oduksc, SRC_ZONE_V, src_zone_v); \
    
/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: oduksc_scbs3_cfg_struct_t                                         
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:                                                                  
*   SCBS3 configuration parameters for each instantiation                         
*                                                                               
* ELEMENTS:                                                                     
*   num_grp             - number of groups
*   max_chnl            - maximum number of channels in the context of a parent
*   max_rows            - maximum number of rows in the calendar and userbits    
*   total_rate          - total interface rate = total_rate * 10^(total_rate_units)      
*   total_rate_units    - 10^ exponent part of total_rate     
*   toc_sync_en         - TRUE: enable synchronization to the 'sch_toc_in' input port.
*                         FALSE: ignore the 'sch_toc_in' input port.
*   toc_sync            - Entry that corresponds to the top of calendar entry \n
*                        toc_sync[6:2] defines the calendar memory row\n
*                        toc_sync[1:0] defines the entry within the row    
*   page_swap_mode      - Page swap with global pin or local register bit see
*                        scbs3_page_swap_mode_t   
*   bonus_mode_enabled  - semi-work conserving mode enabled
*   enable_row_resizing - row resizing enabled                       
*                                                                                                                                                            
*******************************************************************************/
typedef struct oduksc_scbs3_cfg_struct_t {   
    UINT32                    num_grp;          
    UINT32                    max_chnl;
    UINT32                    max_rows;
    UINT32                    total_rate;
    util_schd_chnl_rate_exp_t total_rate_units;
    BOOL                      toc_sync_en;
    UINT32                    toc_sync;
    scbs3_page_swap_mode_t    page_swap_mode;
    BOOL                      bonus_mode_enabled; 
    BOOL                      enable_row_resizing;
} oduksc_scbs3_cfg_struct_t;

/*******************************************************************************
* STRUCTURE: oduksc_ch_ctxt_ghao_t                                         
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:                                                                  
*   ODUKSC G.HAO parameters specific to each channel                        
*                                                                               
* ELEMENTS:                                                                     
*   current_oduflex_gfp_rate   - Current ODUflex GFP rate
*   target_oduflex_gfp_rate    - Tranget ODUflex GFP rate
*   current_rate               - Current rate  
*   target_rate                - Target rate     
*   target_cgen_m              - Target credit generator numerator
*   target_cgen_n              - Target credit generator denominator                    
*                                                                                                                                                            
*******************************************************************************/
typedef struct oduksc_ch_ctxt_ghao_t {
    UINT32                        current_oduflex_gfp_rate;
    UINT32                        target_oduflex_gfp_rate;
    UINT32                        current_rate;
    UINT32                        target_rate;
    UINT32                        target_cgen_m;
    UINT32                        target_cgen_n;
} oduksc_ch_ctxt_ghao_t;


/*******************************************************************************
* STRUCTURE: oduksc_ch_ctxt_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODUKSC parameters specific to each channel
*
* ELEMENTS:
*   chnl_state         - Channel state see oduksc_chnl_state_t
*   scbs3_user_bits    - Side band user bits that are output by the SCBS3 next 
*                        to the channel being scheduled
*   cgen_u_d           - ramp credit generator up/down flag \n
*                        0 -- ramp down \n
*                        1 -- ramp up \n
*   cgen_s             - Ramp credit generator numerator increment (ramp step)
*   cgen_m2            - Ramp credit generator numerator
*   cgen_n2            - Ramp credit generator denominator
*   cgen_m             - Base credit generator numerator
*   cgen_n             - Base credit generator denominator
*   *ghao              - GHAO parameters see oduksc_ch_ctxt_ghao_t
*       
*******************************************************************************/
typedef struct oduksc_ch_ctxt_struct_t {
    oduksc_chnl_state_t           chnl_state;  
    UINT32                        scbs3_user_bits; 
    UINT32                        cgen_u_d;
    UINT32                        cgen_s;
    UINT32                        cgen_m2;
    UINT32                        cgen_n2;
    UINT32                        cgen_m;
    UINT32                        cgen_n;  
    oduksc_ch_ctxt_ghao_t        *ghao; 
} oduksc_ch_ctxt_struct_t;

/*******************************************************************************
* STRUCTURE: oduksc_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Static ODUKSC module instance data
*
* ELEMENTS:
*   max_chnl                  - maximum number of channels
*   total_rate                - total interface rate = total_rate * 10^(total_rate_units) 
*   total_rate_units          - 10^ exponent part of total_rate      
*   bus_width                 - bus width on the egress DCI interface
*   scbs3_present             - TRUE: SCBS3 present. FALSE: No SCBS3 present  
*   scbs3_cfg                 - SCBS3 configuration parameters see oduksc_scbs3_cfg_struct_t
*   userbits_del              - the number of clock cycles by which the delayed 
*                               versions of the user-programmable output signals 
*                               lag the undelayed versions
*   cal_entries_alloc_method  - Calendar entries distribution algorithm number
*   db_id                     - Database ID for the instance to access
*   db_handle                 - Database handle  
*   db_en                     - Database access enable
*   ramp_fast_mode_en         - Ramp acceleration mode enable
*   ramp_fast_mode_factor     - Ramp acceleration factor
*   num_mult                  - Multiplier for the frame period numerator for 
*                               changing the DCI reference clock
*   demon_mult                - Multiplier for the frame period denomenator for 
*                               changing the DCI reference clock
*       
*******************************************************************************/
typedef struct oduksc_cfg_t{
    UINT32                        max_chnl;
    UINT32                        total_rate;
    util_schd_chnl_rate_exp_t     total_rate_units;
    UINT32                        bus_width;
    BOOL                          scbs3_present;
    oduksc_scbs3_cfg_struct_t     scbs3_cfg;
    UINT32                        userbits_del;                                                                                                        
    UINT32                        cal_entries_alloc_method; 
    util_gen_db_id_t              db_id;
    UINT32                        db_handle;
    BOOL                          db_en;
    BOOL                          ramp_fast_mode_en;
    UINT32                        ramp_fast_mode_factor;
    UINT32                        num_mult;
    UINT32                        demon_mult;
} oduksc_cfg_t;

/*******************************************************************************
* STRUCTURE: oduksc_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODUKSC Module Instance dynamic data
*
* ELEMENTS:
*   ch_ctxt_ptr      - Channel context pointer see oduksc_ch_ctxt_struct_t
*   ri_state_array   - Ramp interval state see oduksc_ri_state_t
*       
*******************************************************************************/
typedef struct oduksc_var_t {
    oduksc_ch_ctxt_struct_t   *ch_ctxt_ptr;
    oduksc_ri_state_t          ri_state_array[ODUKSC_RI_NUM];   
} oduksc_var_t;

/*******************************************************************************
* STRUCTURE: oduksc_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODUKSC Module Instance Handle
*
* ELEMENTS:
*       base              - See pmc_handle_t
*       cfg               - See oduksc_cfg_t
*       var               - See oduksc_var_t
*       *scbs3_handle     - See scbs3_handle_t
*       
*******************************************************************************/
typedef struct oduksc_handle_struct_t {
    pmc_handle_t base;
    oduksc_cfg_t cfg;
    oduksc_var_t var;
    scbs3_handle_t   *scbs3_handle;       
} oduksc_handle_struct_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC oduksc_handle_t *oduksc_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void oduksc_ctxt_destroy(oduksc_handle_t *oduksc_handle);
PUBLIC void oduksc_handle_init(oduksc_handle_t *oduksc_handle,
                               oduksc_type_id_t oduksc_type_id,
                               UINT32 userbits_del);   

PUBLIC PMC_ERROR oduksc_init(oduksc_handle_t *oduksc_handle,
                             pmc_energy_state_t energy_state);
PUBLIC BOOL oduksc_start_state_test(oduksc_handle_t *oduksc_handle);                              
PUBLIC PMC_ERROR oduksc_reset_cfg(oduksc_handle_t *oduksc_handle, UINT32 reset);

PUBLIC PMC_ERROR oduksc_chnl_prov(oduksc_handle_t *oduksc_handle, 
                                  UINT32 chnl, 
                                  util_global_odukp_type_t server_type,
                                  util_global_odukp_type_t client_type,  
                                  UINT32 client_rate, 
                                  util_global_mapping_mode_t mapping_mode,
                                  UINT32 oduflex_gfp_rate);
                                  
PUBLIC PMC_ERROR oduksc_chnl_deprov(oduksc_handle_t *oduksc_handle, 
                                    UINT32 chnl);

PUBLIC PMC_ERROR oduksc_chnl_activate(oduksc_handle_t *oduksc_handle, 
                                      UINT32 chnl);                                                                                                          

PUBLIC PMC_ERROR oduksc_chnl_deactivate(oduksc_handle_t *oduksc_handle, 
                                        UINT32 chnl);

PUBLIC void oduksc_chnl_clean(oduksc_handle_t *oduksc_handle, 
                              UINT32 *chnl,
                              UINT32 num_chnl);

PUBLIC PMC_ERROR oduksc_chnl_update(oduksc_handle_t *oduksc_handle,
                                    UINT32 chnl,
                                    BOOL update_scbs3);

PUBLIC PMC_ERROR oduksc_core_cgen_chnl_update(oduksc_handle_t *oduksc_handle, UINT32 chnl);

PUBLIC PMC_ERROR oduksc_zone_pushback_cfg(oduksc_handle_t *oduksc_handle, 
                                          UINT32 chnl, 
                                          UINT32 zone_pb_en);     
PUBLIC UINT32 oduksc_zone_pushback_cfg_get(oduksc_handle_t *oduksc_handle, 
                                           UINT32 chnl);     
PUBLIC PMC_ERROR oduksc_bmp_mode_cgen_chnl_cfg_get(oduksc_handle_t *oduksc_handle,
                                                   UINT32 chnl,
                                                   DOUBLE *client_rate);
PUBLIC PMC_ERROR oduksc_odujat_chnl_prov(oduksc_handle_t *oduksc_handle,
                                         UINT32 chnl,
                                         util_global_odukp_type_t server_type,
                                         util_global_odukp_type_t client_type,
                                         UINT32 client_rate,
                                         UINT32 odu_flex_data,
                                         UINT32 mc_numer);   

PUBLIC PMC_ERROR oduksc_adjust_dci_clk(oduksc_handle_t *oduksc_handle,
                                       UINT32 num_mult,
                                       UINT32 denom_mult);
PUBLIC scbs3_handle_t* oduksc_scbs3_handle_get(oduksc_handle_t *hndl);

PUBLIC void oduksc_cgen_chnl_cfg_get(oduksc_handle_t *oduksc_handle,
                                     UINT32 chnl,
                                     util_global_odukp_type_t *server_type,
                                     util_global_odukp_type_t *client_type,
                                     UINT32 *client_rate,
                                     UINT32 *rate_ramp);

PUBLIC PMC_ERROR oduksc_chnl_cleanup(oduksc_handle_t *oduksc_handle,
                                     UINT32 chnl);
PUBLIC void oduksc_alloc_init(oduksc_handle_t *oduksc_handle);
PUBLIC PMC_ERROR oduksc_handle_restart_init(oduksc_handle_t *oduksc_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr);

PUBLIC PMC_ERROR oduksc_chnl_scbs3_cgen_reprov(oduksc_handle_t *oduksc_handle,
                                               UINT32 chnl,
                                               util_global_odukp_type_t client_type,
                                               UINT32 oduflex_gfp_rate);

PUBLIC PMC_ERROR oduksc_chnl_cgen_ramp_prov(oduksc_handle_t *oduksc_handle,
                                            UINT32 chnl,
                                            util_global_odukp_type_t client_type,
                                            UINT32 oduflex_gfp_rate_final,
                                            BOOL end_for_bwr_ind_clear);

PUBLIC PMC_ERROR oduksc_chnl_cgen_ramp_start(oduksc_handle_t *oduksc_handle,
                                             UINT32 chnl,
                                             util_global_odukp_type_t client_type);

PUBLIC PMC_ERROR oduksc_chnl_cgen_ramp_end_prov(oduksc_handle_t *oduksc_handle,
                                                UINT32 chnl,
                                                util_global_odukp_type_t client_type);

PUBLIC PMC_ERROR oduksc_ramp_done_isr(oduksc_handle_t *oduksc_handle, 
                                      UINT32 chnl);       

PUBLIC PMC_ERROR oduksc_chnl_ramp_done_get(oduksc_handle_t *oduksc_handle,
                                           UINT32 chnl,
                                           UINT32 *ramp_done);
                                           
PUBLIC PMC_ERROR oduksc_chnl_ramp_done_poll(oduksc_handle_t *oduksc_handle,
                                            UINT32 chnl,
                                            UINT32 poll_delay_us);                                       

PUBLIC PMC_ERROR oduksc_chnl_activate_super_prov(oduksc_handle_t *oduksc_handle,
                                                 UINT32 chnl);   

PUBLIC PMC_ERROR oduksc_chnl_deactivate_super_deprov(oduksc_handle_t *oduksc_handle,
                                                     UINT32 chnl);
                                                     
PUBLIC PMC_ERROR oduksc_chnl_state_set(oduksc_handle_t *oduksc_handle,
                                       UINT32 chnl,
                                       oduksc_chnl_state_t chnl_state);                                         
                                                       
PUBLIC PMC_ERROR oduksc_page_swap(oduksc_handle_t *oduksc_handle,
                                  BOOL update_scbs3);                                                                                                                                                                                                                                                                

PUBLIC void oduksc_chnl_base_gen_en_cfg(oduksc_handle_t *oduksc_handle,
                                        UINT32 chnl,
                                        BOOL8 value);

PUBLIC PMC_ERROR oduksc_all_chnls_update(oduksc_handle_t *oduksc_handle,
                                         UINT32 *chnl_ids,
                                         UINT32 num_chnls,
                                         BOOL update_scbs3);



PUBLIC PMC_ERROR oduksc_int_enable(oduksc_handle_t *oduksc_handle,
                                   oduksc_int_t *int_table_ptr,
                                   oduksc_int_t *int_en_table_ptr,
                                   BOOL8 enable);

PUBLIC PMC_ERROR oduksc_int_clear(oduksc_handle_t *oduksc_handle,
                                  oduksc_int_t *int_table_ptr,
                                  oduksc_int_t *int_en_table_ptr);

PUBLIC PMC_ERROR oduksc_int_retrieve(oduksc_handle_t *oduksc_handle,
                                     oduksc_int_t *filt_table_ptr,
                                     oduksc_int_t *int_table_ptr);

PUBLIC PMC_ERROR oduksc_int_enabled_check(oduksc_handle_t *oduksc_handle,
                                          oduksc_int_t *int_en_table_ptr,
                                          BOOL8 *int_found_ptr);

PUBLIC PMC_ERROR oduksc_int_chnl_enable(oduksc_handle_t *oduksc_handle,
                                        UINT32 chnl_id,
                                        oduksc_int_chnl_t *int_table_ptr,
                                        oduksc_int_chnl_t *int_en_table_ptr,
                                        BOOL8 enable);

PUBLIC PMC_ERROR oduksc_int_chnl_clear(oduksc_handle_t *oduksc_handle,
                                       UINT32 chnl_id,
                                       oduksc_int_chnl_t *int_table_ptr,
                                       oduksc_int_chnl_t *int_en_table_ptr);

PUBLIC PMC_ERROR oduksc_int_chnl_retrieve(oduksc_handle_t *oduksc_handle,
                                          UINT32 chnl_id,
                                          oduksc_int_chnl_t *filt_table_ptr,
                                          oduksc_int_chnl_t *int_table_ptr);

PUBLIC PMC_ERROR oduksc_int_chnl_enabled_check(oduksc_handle_t *oduksc_handle,
                                               UINT32 chnl_id,
                                               oduksc_int_chnl_t *int_en_table_ptr,
                                               BOOL8 *int_found_ptr);
#ifdef __cplusplus
}
#endif

#endif /* _ODUKSC_H */

/*
** end of file
*/
