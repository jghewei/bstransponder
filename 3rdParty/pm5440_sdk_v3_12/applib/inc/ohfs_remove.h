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

#ifndef _OHFS_REMOVE_H
#define _OHFS_REMOVE_H

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
#include "ohfs_remove_api.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: ohfs_remove_instance_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying block instance type.
*
* ELEMENTS:
*    OHFS_REMOVE_INSTANCE_RGMP    - RGMP instance
*    OHFS_REMOVE_INSTANCE_DEMUX   - DEMUX instance
*    OHFS_REMOVE_INSTANCE_GFP     - GFP instance
*    OHFS_REMOVE_INSTANCE_COREOTN - COREOTN instance
*
*******************************************************************************/
typedef enum {
    OHFS_REMOVE_INSTANCE_RGMP    = 0,
    OHFS_REMOVE_INSTANCE_DEMUX   = 1,
    OHFS_REMOVE_INSTANCE_GFP     = 2,
    OHFS_REMOVE_INSTANCE_COREOTN = 3,
} ohfs_remove_instance_type_t;

/*******************************************************************************
* ENUM: ohfs_remove_map_ssf_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying signal fail handeling type
*
* ELEMENTS:
*    OHFS_REMOVE_MAP_SF_TO_SSF      - ingress SF indication passed to egress SSF
*    OHFS_REMOVE_MAP_DCSF_TO_SSF    - CSF indication passed to egress SSF
*    OHFS_REMOVE_MAP_DPLM_TO_SSF    - PT mismatch indication passed to egress SSF 
*    OHFS_REMOVE_MAP_DLOOMFI_TO_SSF - DLOOMFI defect indication passed to egress SSF
*
*******************************************************************************/
typedef enum {
    OHFS_REMOVE_MAP_SF_TO_SSF = 0,
    OHFS_REMOVE_MAP_DCSF_TO_SSF,
    OHFS_REMOVE_MAP_DPLM_TO_SSF,
    OHFS_REMOVE_MAP_DLOOMFI_TO_SSF,
    LAST_OHFS_REMOVE_MAP_SSF_TYPE
} ohfs_remove_map_ssf_type_t;

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/


/*******************************************************************************
* STRUCTURE: ohfs_remove_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Static OHFS_REMOVE module instance data
*
* ELEMENTS:
*   ohfs_remove_instance_type_t       - block instance type
*   util_global_otn_msi_cfg_mode_t    - MSI configuration mode for OTN payload
*
*       
*******************************************************************************/
typedef struct ohfs_remove_cfg_t{
    ohfs_remove_instance_type_t     instance;
    util_global_otn_msi_cfg_mode_t  msi_cfg_mode;
} ohfs_remove_cfg_t;


/*******************************************************************************
* STRUCTURE: ohfs_remove_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   OHFS_REMOVE Module Instance Handle
*
* ELEMENTS:
*       base                  - See pmc_handle_t
*       cfg                   - See ohfs_remove_cfg_t
*       
*******************************************************************************/
typedef struct ohfs_remove_handle_struct_t {
    pmc_handle_t base;
    ohfs_remove_cfg_t cfg;
} ohfs_remove_handle_struct_t;



/*******************************************************************************
* STRUCTURE: ohfs_remove_recover_cycle_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Recover dynamically changing context for cycle-based infromation from 
*   register
*
* ELEMENTS:
*   ho_cycle_valid      - valid calendar cycles for the HO channel
*   ho_chnl_id          - HO channel ID
*   ho_frm_num          - number of tributary slots in the HO channel
*   lo_ts_valid         - valid tributary slot for the LO channel
*   lo_map_type         - LO channel mapping type
*   lo_first_ts         - LO channel first tributary slot
*   lo_last_ts          - LO channel last tributary slot
*   lo_chnl_id          - LO channel ID
*
*       
*******************************************************************************/
typedef struct ohfs_remove_recover_cycle_var_struct_t {
    BOOL8 ho_cycle_valid;
    UINT8 ho_chnl_id;
    UINT8 ho_frm_num;
    BOOL8 lo_ts_valid;
    UINT8 lo_map_type;
    BOOL8 lo_first_ts;
    BOOL8 lo_last_ts; 
    BOOL8 lo_chnl_id; 
} ohfs_remove_recover_cycle_var_t;


/*******************************************************************************
* STRUCTURE: ohfs_remove_recover_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Recover dynamically changing context from register
*
* ELEMENTS:
*   ts_num                - Max slot number
*   has_fixed_stuff       - Fixed stuff indication
*   cycle                 - see ohfs_remove_recover_cycle_var_t
*
*       
*******************************************************************************/
typedef struct ohfs_remove_recover_var_struct_t {
    UINT8 ts_num[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    BOOL has_fixed_stuff[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    ohfs_remove_recover_cycle_var_t cycle[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];           
} ohfs_remove_recover_var_t;



/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC ohfs_remove_handle_t *ohfs_remove_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void ohfs_remove_ctxt_destroy(ohfs_remove_handle_t *ohfs_remove_handle);
PUBLIC void ohfs_remove_handle_init(ohfs_remove_handle_t *ohfs_remove_handle, 
                                    ohfs_remove_instance_type_t ohfs_remove_instance);
PUBLIC PMC_ERROR ohfs_remove_handle_restart_init(ohfs_remove_handle_t *ohfs_remove_handle, 
                                                 util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                 pmc_energy_state_t energy_state);
PUBLIC PMC_ERROR ohfs_remove_init(ohfs_remove_handle_t *ohfs_remove_handle,
                                  pmc_energy_state_t energy_state);
PUBLIC UINT32 ohfs_remove_block_non_default_cfg(ohfs_remove_handle_t *ohfs_remove_handle);
PUBLIC PMC_ERROR ohfs_remove_chnl_prov(ohfs_remove_handle_t *ohfs_remove_handle, 
                                       UINT32 chnl,
                                       util_global_odukp_type_t server_type,
                                       UINT32 client_type,
                                       util_global_mapping_mode_t mapping_mode,
                                       util_global_odu_line_payload_t ts_type);

PUBLIC PMC_ERROR ohfs_remove_chnl_unprov(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         UINT32 cycle,
                                         UINT32 chnl); 
PUBLIC void      ohfs_remove_chnl_clean(ohfs_remove_handle_t *ohfs_remove_handle, 
                                        UINT32 *chnl,   
                                        UINT32 num_chnl);    
PUBLIC void ohfs_remove_chnl_cycle_clean(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         UINT32 *ho_chnl,
                                         UINT32 num_ho_chnl,
                                         UINT32 *lo_chnl,
                                         UINT32 num_lo_chnl);
PUBLIC void ohfs_remove_chnl_cycle_clean_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                             UINT32 *ho_chnl,
                                             UINT32 num_ho_chnl,
                                             UINT32 *lo_chnl,
                                             UINT32 num_lo_chnl,                                          
                                             UINT32 cleaned_cycle[],                                          
                                             UINT32 *num_cleaned_cycle);
PUBLIC PMC_ERROR ohfs_remove_trnsfr_rcoh_to_shadow_en(ohfs_remove_handle_t *ohfs_remove_handle);


PUBLIC PMC_ERROR ohfs_remove_func_mode_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                           UINT32 chnl,
                                           UINT32 func_mode);

PUBLIC PMC_ERROR ohfs_remove_psi_check_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                           UINT32 chnl,
                                           UINT32 pt_chk_en, 
                                           UINT32 csf_chk_en, 
                                           UINT32 msi_chk_en);

PUBLIC PMC_ERROR ohfs_remove_flex_rcoh_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                           UINT32 chnl,
                                           UINT32 enable);
                                           
PUBLIC PMC_ERROR ohfs_remove_ho_rcoh_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         UINT32 chnl,
                                         UINT32 enable);                                           

PUBLIC PMC_ERROR ohfs_remove_signal_fail_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                             UINT32 chnl,
                                             UINT32 dcsf_to_ssf,
                                             UINT32 dplm_to_ssf,
                                             UINT32 sf_to_ssf);

PUBLIC PMC_ERROR ohfs_remove_fs_stripping_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                              UINT32 chnl,
                                              util_global_odukp_type_t oduk_type,
                                              util_global_mapping_mode_t mapping_mode);                                             
                                                                               
PUBLIC PMC_ERROR ohfs_remove_ipt_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                     UINT32 cycle,
                                     UINT32 chnl,
                                     UINT32 trib_slot,
                                     UINT32 valid);

PUBLIC PMC_ERROR ohfs_remove_ts_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                    UINT32 cycle,
                                    util_global_mapping_mode_t mapping_mode,
                                    UINT32 last_ts,
                                    UINT32 first_ts,
                                    UINT32 valid);                                     
                                         
PUBLIC PMC_ERROR ohfs_remove_lo_chnl_id_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                            UINT32 cycle, UINT32 chnl);

PUBLIC PMC_ERROR ohfs_remove_msi_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                     UINT32 cycle, 
                                     util_global_odukp_type_t server_type, 
                                     UINT32 client_type, 
                                     util_global_odu_line_payload_t ts_type, 
                                     UINT32 ts_port); 
                                       
PUBLIC PMC_ERROR ohfs_remove_msi_lo_id_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                           UINT32 cycle, 
                                           util_global_odukp_type_t server_type, 
                                           UINT32 client_type, 
                                           util_global_odu_line_payload_t ts_type, 
                                           UINT32 ts_port,
                                           UINT32 chnl); 
                                       
PUBLIC PMC_ERROR ohfs_remove_fs_enbl(ohfs_remove_handle_t *ohfs_remove_handle, 
                                     UINT32 chnl,
                                     UINT32 enbl);   
PUBLIC PMC_ERROR ohfs_remove_ipt_print(ohfs_remove_handle_t *ohfs_remove_handle);   


PUBLIC PMC_ERROR ohfs_remove_accpt_pt_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                          UINT32 chnl,
                                          UINT8 *accpt_pt);
                                          
PUBLIC PMC_ERROR ohfs_remove_exp_pt_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                        UINT32 chnl,
                                        UINT8 *exp_pt);                                          
                                    
PUBLIC PMC_ERROR ohfs_remove_accpt_msi_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                           UINT32 cycle,
                                           UINT8 *msi);

PUBLIC PMC_ERROR ohfs_remove_rcoh_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                      UINT32 chnl,
                                      UINT32 *rcoh);
                                      
PUBLIC PMC_ERROR ohfs_remove_pt_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                    UINT32 chnl, 
                                    UINT8 pt);
                                    
PUBLIC PMC_ERROR ohfs_remove_signal_fail_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                             UINT32 chnl,
                                             UINT32 dcsf_to_ssf,
                                             UINT32 dplm_to_ssf,
                                             UINT32 sf_to_ssf);    

PUBLIC UINT32 ohfs_remove_ram_init(ohfs_remove_handle_t *ohfs_remove_handle);

PUBLIC PMC_ERROR ohfs_remove_fs_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                    UINT32 chnl, UINT32 fs1_en, UINT32 fs1_16_8,
                                    UINT32 fs1_addr, UINT32 fs2_en, UINT32 fs2_16_8,
                                    UINT32 fs2_addr);

PUBLIC PMC_ERROR ohfs_remove_ho_chnl_deprov(ohfs_remove_handle_t *ohfs_remove_handle, 
                                            UINT32 chnl);

PUBLIC PMC_ERROR ohfs_remove_dmx_chan_mapping_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                                  UINT32 cycle,
                                                  UINT32 map_type);  

PUBLIC PMC_ERROR ohfs_remove_exp_msi_set(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         UINT32 cycle, 
                                         UINT8 msi_val);

PUBLIC PMC_ERROR ohfs_remove_exp_msi_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         UINT32 cycle, 
                                         UINT8 *msi_val);                                                                                                                                     
                                                        
PUBLIC PMC_ERROR ohfs_remove_map_ssf_set(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         UINT32 chnl,
                                         ohfs_remove_map_ssf_type_t map_ssf_type,
                                         UINT32 enable);
PUBLIC PMC_ERROR ohfs_remove_map_ssf_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         UINT32 chnl,
                                         ohfs_remove_map_ssf_type_t map_ssf_type,                       
                                         UINT32 *enable);
PUBLIC util_global_mapping_mode_t ohfs_remove_mapping_mode_get(ohfs_remove_handle_t *ohfs_remove_handle,
                                                               UINT32 calendar_cycle);
PUBLIC UINT32 ohfs_remove_ext_payload_get(ohfs_remove_handle_t *ohfs_remove_handle,
                                          UINT32 chnl);

PUBLIC BOOL8 ohfs_remove_fixed_stuff_enable_get(ohfs_remove_handle_t *ohfs_remove_handle,
                                                UINT32 chnl);
PUBLIC BOOL8 ohfs_remove_null_frame_mode_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                             UINT32 chnl);
PUBLIC void ohfs_remove_reg_ctxt_recover(ohfs_remove_handle_t *ohfs_remove_handle, 
                                         ohfs_remove_recover_var_t *ohfs_remove_recover_reg);
PUBLIC void ohfs_remove_reg_ctxt_ho_chnl_cfg_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                                 ohfs_remove_recover_var_t *ohfs_remove_recover_reg,
                                                 UINT32 chnl,
                                                 UINT8 *num_tribslot,
                                                 UINT8 cal_array[UTIL_GLOBAL_MAX_NUM_TS]);
PUBLIC void ohfs_remove_reg_ctxt_lo_chnl_cfg_get(ohfs_remove_handle_t *ohfs_remove_handle, 
                                                 ohfs_remove_recover_var_t *ohfs_remove_recover_reg,
                                                 UINT8 lo_chnl,
                                                 UINT8 *num_tribslot,
                                                 UINT8 *mapping_mode,
                                                 UINT8 *ho_chnl,
                                                 UINT8 cal_array[UTIL_GLOBAL_MAX_NUM_TS],
                                                 UINT8 ts_array[UTIL_GLOBAL_MAX_NUM_TS]);
PUBLIC void ohfs_remove_lo_cycle_clean(ohfs_remove_handle_t *ohfs_remove_handle, 
                                       UINT32 *cycle,
                                       UINT32 num_cycle);
                                        
PUBLIC void ohfs_remove_msi_mode_set(ohfs_remove_handle_t *ohfs_remove_handle,
                                     util_global_otn_msi_cfg_mode_t msi_mode);
                                     
PUBLIC void ohfs_remove_msi_mode_get(ohfs_remove_handle_t *ohfs_remove_handle,
                                     util_global_otn_msi_cfg_mode_t *msi_mode);                                        
                                     
PUBLIC PMC_ERROR ohfs_remove_ts_valid_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                          UINT32 cycle,
                                          BOOL8 valid);                             

PUBLIC BOOL8 ohfs_remove_ts_valid_get(ohfs_remove_handle_t *ohfs_remove_handle,                                           UINT32 cycle);
                                          
#ifdef __cplusplus
}
#endif

#endif /* _OHFS_REMOVE_H */

/*
** end of file
*/
