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

#ifndef _OHFS_INSERT_H
#define _OHFS_INSERT_H

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
#include "ohfs_insert_api.h"
#include "cfc.h"


/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: ohfs_insert_instance_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying block instance type.
*
* ELEMENTS:
*    OHFS_INSERT_INSTANCE_TGMP - TGMP instance
*    OHFS_INSERT_INSTANCE_MUX  - MUX instance
*
*******************************************************************************/
typedef enum {
    OHFS_INSERT_INSTANCE_TGMP   = 0,
    OHFS_INSERT_INSTANCE_MUX    = 1,
} ohfs_insert_instance_type_t;


/*
** Constants
*/


#define OHFS_INSERT_CFC_MAX_FIFO_NUM  96        /*!<CFC maximum number of FIFO*/
#define OHFS_INSERT_CFC_PAGE_SIZE     4         /*!<CFC page size*/

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: ohfs_insert_cfc_cfg_struct_t
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
typedef struct ohfs_insert_cfc_cfg_struct_t {   
    UINT32 max_fifo_num;  
    UINT32 page_size;      
}  ohfs_insert_cfc_cfg_struct_t;


/*******************************************************************************
* STRUCTURE: ohfs_insert_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Static OHFS_INSERT module instance data
*
* ELEMENTS:
*   ohfs_insert_cfc_cfg_struct_t      - CFC configuration parameters
*   ohfs_insert_instance_type_t       - block instance type
*   util_global_otn_msi_cfg_mode_t    - MSI configuration mode for OTN payload
*
*       
*******************************************************************************/
typedef struct ohfs_insert_cfg_t {
   ohfs_insert_cfc_cfg_struct_t       cfc_cfg;
   ohfs_insert_instance_type_t        instance;
   util_global_otn_msi_cfg_mode_t     msi_cfg_mode;
} ohfs_insert_cfg_t;


/*******************************************************************************
* STRUCTURE: ohfs_insert_recover_cycle_var_t
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
typedef struct ohfs_insert_recover_cycle_var_struct_t {
    BOOL8 ho_cycle_valid;
    UINT8 ho_chnl_id;
    UINT8 ho_frm_num;
    BOOL8 lo_ts_valid;
    UINT8 lo_map_type;
    BOOL8 lo_first_ts;
    BOOL8 lo_last_ts; 
    BOOL8 lo_chnl_id; 
} ohfs_insert_recover_cycle_var_t;


/*******************************************************************************
* STRUCTURE: ohfs_insert_recover_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Recover dynamically changing context from register
*
* ELEMENTS:
*   max_slot_num      - Max slot number
*   has_fixed_stuff   - Fixed stuff indication
*   cycle             - see ohfs_insert_recover_cycle_var_t
*
*       
*******************************************************************************/
typedef struct ohfs_insert_recover_var_struct_t {
    UINT8 max_slot_num[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    BOOL has_fixed_stuff[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];
    ohfs_insert_recover_cycle_var_t cycle[UTIL_GLOBAL_MAX_NUM_ODU_CHNL];            
} ohfs_insert_recover_var_t;


/*******************************************************************************
* STRUCTURE: ohfs_insert_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   OHFS_INSERT Module Instance dynamic data
*
* ELEMENTS:
*   pt     - PT value
*
*       
*******************************************************************************/
typedef struct ohfs_insert_var_t {
   UINT8 pt[UTIL_GLOBAL_MAX_NUM_ODU_CHNL]; 
} ohfs_insert_var_t;


/*******************************************************************************
* STRUCTURE: ohfs_insert_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   OHFS_INSERT Module Instance Handle
*
* ELEMENTS:
*       base                  - See pmc_handle_t
*       cfg                   - See ohfs_insert_cfg_t
*       var                   - See ohfs_insert_var_t
*       *cfc_handle           - see cfc_handle_t
*       
*******************************************************************************/
typedef struct ohfs_insert_handle_struct_t {
    pmc_handle_t base;
    ohfs_insert_cfg_t cfg;
    ohfs_insert_var_t var;
    cfc_handle_t *cfc_handle;
} ohfs_insert_handle_struct_t;



/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC ohfs_insert_handle_t *ohfs_insert_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void ohfs_insert_ctxt_destroy(ohfs_insert_handle_t *ohfs_insert_handle);
PUBLIC void ohfs_insert_handle_init(ohfs_insert_handle_t *ohfs_insert_handle,
                                    ohfs_insert_instance_type_t ohfs_insert_instance);
PUBLIC void ohfs_insert_alloc_init(ohfs_insert_handle_t *ohfs_insert_handle);
PUBLIC void ohfs_insert_handle_restart_init(ohfs_insert_handle_t *ohfs_insert_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                            pmc_energy_state_t top_energy_state);

PUBLIC PMC_ERROR ohfs_insert_init(ohfs_insert_handle_t *ohfs_insert_handle, 
                                  pmc_energy_state_t energy_state);
PUBLIC BOOL ohfs_insert_start_state_test(ohfs_insert_handle_t *ohfs_insert_handle);                                   
PUBLIC PMC_ERROR ohfs_insert_block_non_default_cfg(ohfs_insert_handle_t *ohfs_insert_handle);

PUBLIC PMC_ERROR ohfs_insert_chnl_default_init(ohfs_insert_handle_t *ohfs_insert_handle,
                                               UINT32 chnl,
                                               pmc_block_reg_access_state_enum state);

PUBLIC PMC_ERROR ohfs_insert_ho_chnl_prov(ohfs_insert_handle_t *ohfs_insert_handle, 
                                          UINT32 chnl,
                                          util_global_odukp_type_t server_type,
                                          util_global_odu_line_payload_t ts_type, 
                                          UINT32 num_cal_entries, BOOL8 offline_update);
                                          
PUBLIC PMC_ERROR ohfs_insert_chnl_prov(ohfs_insert_handle_t *ohfs_insert_handle, 
                                       UINT32 chnl,
                                       util_global_odukp_type_t server_type,
                                       UINT32 client_type,
                                       DOUBLE client_rate,
                                       util_global_mapping_mode_t mapping_mode,
                                       util_global_odu_line_payload_t ts_type, 
                                       UINT32 num_cal_entries);
                                       
PUBLIC PMC_ERROR ohfs_insert_chnl_cfc_reprov(ohfs_insert_handle_t *ohfs_insert_handle,
                                             UINT32 chnl,
                                             UINT32 num_cal_entries);

PUBLIC PMC_ERROR ohfs_insert_chnl_thresh_reprov(ohfs_insert_handle_t *ohfs_insert_handle,
                                                UINT32 chnl,
                                                util_global_mapping_mode_t mapping_mode,
                                                UINT32 num_cal_entries);

PUBLIC PMC_ERROR ohfs_insert_chnl_unprov(ohfs_insert_handle_t *ohfs_insert_handle, 
                                         UINT32 chnl);                                       
PUBLIC void      ohfs_insert_chnl_clean(ohfs_insert_handle_t *ohfs_insert_handle, 
                                        UINT32 *chnl,   
                                        UINT32 num_chnl);    
PUBLIC PMC_ERROR ohfs_insert_lo_chnl_id_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                            UINT32 cycle, UINT32 chnl);

PUBLIC PMC_ERROR ohfs_insert_ho_ipt_cfg(ohfs_insert_handle_t *ohfs_insert_handle,
                                        UINT32 cycle,
                                        UINT32 chnl,
                                        UINT32 frm_num,
                                        UINT32 valid);
                                                                                   
PUBLIC PMC_ERROR ohfs_insert_lo_ipt_cfg(ohfs_insert_handle_t *ohfs_insert_handle,
                                        UINT32 cycle,
                                        UINT32 chnl,
                                        UINT32 frm_num,
                                        UINT32 valid); 

PUBLIC PMC_ERROR ohfs_insert_ts_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                    UINT32 cycle,
                                    UINT32 last_ts,
                                    UINT32 valid);
                                    
PUBLIC PMC_ERROR ohfs_insert_chnl_mapping_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                              UINT32 chnl_id,
                                              UINT32 mapping_mode);  
                                               
PUBLIC UINT32 ohfs_insert_cfc_fifo_end_addr_only_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                                     UINT32 chnl_id, UINT32 num_cal_entries);   
                                                     
PUBLIC PMC_ERROR ohfs_insert_cfc_ipt_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                         UINT32 index, 
                                         UINT32 ipt_valid, 
                                         UINT32 ipt_id, 
                                         UINT32 ipt_va);                                                                                                                                   

PUBLIC PMC_ERROR ohfs_insert_msi_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                     UINT32 cycle, 
                                     util_global_odukp_type_t server_type, 
                                     UINT32 client_type, 
                                     util_global_odu_line_payload_t ts_type, UINT32 ts_port);
                                     
PUBLIC PMC_ERROR ohfs_insert_omfi_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                      UINT32 chnl, 
                                      UINT32 omfi_insrt);

PUBLIC PMC_ERROR ohfs_insert_null_frame_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                            UINT32 chnl,
                                            UINT32 null_frm_mode);                                                                                                                                                        

PUBLIC PMC_ERROR ohfs_insert_ncs_en_set(ohfs_insert_handle_t *ohfs_insert_handle,
                                        UINT32 chnl,
                                        UINT32 ncs_en);

PUBLIC PMC_ERROR ohfs_insert_rcoh_set(ohfs_insert_handle_t *ohfs_insert_handle,
                                      UINT32 chnl,
                                      UINT32 rcoh);

PUBLIC PMC_ERROR ohfs_insert_rp_set(ohfs_insert_handle_t *ohfs_insert_handle,
                                    UINT32 chnl,
                                    BOOL8 rp);  
PUBLIC PMC_ERROR ohfs_insert_xoff_wm_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                         UINT32 chnl, 
                                         util_global_mapping_mode_t mapping_mode,
                                         UINT32 num_ts);                                               
PUBLIC PMC_ERROR ohfs_insert_pt_cfg(ohfs_insert_handle_t *ohfs_insert_handle, 
                                     UINT32 chnl,
                                     UINT8 pt);     
                                     
PUBLIC PMC_ERROR ohfs_insert_pt_get(ohfs_insert_handle_t *ohfs_insert_handle, 
                                    UINT32 chnl,
                                    UINT8 *pt);      
                                    
PUBLIC PMC_ERROR ohfs_insert_chnl_activate(ohfs_insert_handle_t *ohfs_insert_handle, 
                                           UINT32 chnl);
                                           
PUBLIC PMC_ERROR ohfs_insert_chnl_deactivate(ohfs_insert_handle_t *ohfs_insert_handle, 
                                             UINT32 chnl);

PUBLIC PMC_ERROR ohfs_insert_ram_init(ohfs_insert_handle_t *ohfs_insert_handle);   

PUBLIC PMC_ERROR ohfs_insert_lo_calendar_cfg_deprov(ohfs_insert_handle_t *ohfs_insert_handle, 
                                                    UINT32 cal_entry); 
                                                    
PUBLIC void ohfs_insert_msi_mode_set(ohfs_insert_handle_t *ohfs_insert_handle,
                                     util_global_otn_msi_cfg_mode_t msi_mode);
                                     
PUBLIC void ohfs_insert_msi_mode_get(ohfs_insert_handle_t *ohfs_insert_handle,
                                     util_global_otn_msi_cfg_mode_t *msi_mode);                                     
                                     
PUBLIC PMC_ERROR ohfs_insert_ho_chnl_deprov(ohfs_insert_handle_t *ohfs_insert_handle, 
                                            UINT32 chnl);

PUBLIC PMC_ERROR ohfs_insert_ho_calendar_cfg_deprov(ohfs_insert_handle_t *ohfs_insert_handle, 
                                                    UINT32 cal_entry);   

PUBLIC PMC_ERROR ohfs_insert_dbg_mask_somf_sync_set(ohfs_insert_handle_t *ohfs_insert_handle, 
                                                    UINT32 chnl, UINT32 somf_sync);
PUBLIC PMC_ERROR ohfs_insert_csf_insert_get(ohfs_insert_handle_t *ohfs_insert_handle,
                                            UINT32 chnl,UINT32 *csf_insert);
                     

PUBLIC PMC_ERROR ohfs_insert_msi_set(ohfs_insert_handle_t *ohfs_insert_handle, 
                                     UINT32 cycle, 
                                     UINT8 msi_val);

PUBLIC PMC_ERROR ohfs_insert_msi_get(ohfs_insert_handle_t *ohfs_insert_handle, 
                                     UINT32 cycle, 
                                     UINT8 *msi_val);                                     
PUBLIC util_global_mapping_mode_t ohfs_insert_mapping_mode_get(ohfs_insert_handle_t *ohfs_insert_handle, 
                                                               UINT32 calendar_cycle);                                    
PUBLIC UINT32 ohfs_insert_ext_payload_get(ohfs_insert_handle_t *ohfs_insert_handle,
                                          UINT32 chnl);
PUBLIC BOOL8 ohfs_insert_fixed_stuff_enable_get(ohfs_insert_handle_t *ohfs_insert_handle,
                                                UINT32 chnl);
PUBLIC BOOL8 ohfs_insert_null_frame_mode_get(ohfs_insert_handle_t *ohfs_insert_handle, 
                                             UINT32 chnl);
PUBLIC void ohfs_insert_reg_ctxt_recover(ohfs_insert_handle_t *ohfs_insert_handle, 
                                         ohfs_insert_recover_var_t *ohfs_insert_recover_reg);
PUBLIC void ohfs_insert_reg_ctxt_ho_chnl_cfg_get(ohfs_insert_handle_t *ohfs_insert_handle, 
                                                 ohfs_insert_recover_var_t *ohfs_insert_recover_reg,
                                                 UINT32 chnl,
                                                 UINT8 *num_tribslot,
                                                 UINT8 cal_array[UTIL_GLOBAL_MAX_NUM_TS]);
PUBLIC void ohfs_insert_reg_ctxt_lo_chnl_cfg_get(ohfs_insert_handle_t *ohfs_insert_handle, 
                                                 ohfs_insert_recover_var_t *ohfs_insert_recover_reg,
                                                 UINT8 lo_chnl,
                                                 UINT8 *num_tribslot,
                                                 UINT8 *mapping_mode,
                                                 UINT8 *ho_chnl,
                                                 UINT8 cal_array[UTIL_GLOBAL_MAX_NUM_TS],
                                                 UINT8 ts_array[UTIL_GLOBAL_MAX_NUM_TS]);
PUBLIC void ohfs_insert_chnl_cycle_clean_get(ohfs_insert_handle_t *ohfs_insert_handle, 
                                             UINT32 *ho_chnl,
                                             UINT32 num_ho_chnl,
                                             UINT32 *lo_chnl,
                                             UINT32 num_lo_chnl,                                          
                                             UINT32 cleaned_cycle[],                                          
                                             UINT32 *num_cleaned_cycle);
PUBLIC void ohfs_insert_chnl_cycle_clean(ohfs_insert_handle_t *ohfs_insert_handle, 
                                         UINT32 *ho_chnl,
                                         UINT32 num_ho_chnl,
                                         UINT32 *lo_chnl,
                                         UINT32 num_lo_chnl);
PUBLIC void ohfs_insert_lo_cycle_clean(ohfs_insert_handle_t *ohfs_insert_handle, 
                                       UINT32 *ho_chnl,
                                       UINT32 num_ho_chnl, 
                                       UINT32 *cycle,
                                       UINT32 num_cycle);

PUBLIC PMC_ERROR ohfs_insert_int_chnl_enable(ohfs_insert_handle_t *ohfs_insert_handle,
                                             UINT32 chnl_id,
                                             ohfs_insert_int_chnl_t *int_table_ptr,
                                             ohfs_insert_int_chnl_t *int_en_table_ptr,
                                             BOOL8 enable);

PUBLIC PMC_ERROR ohfs_insert_int_chnl_clear(ohfs_insert_handle_t *ohfs_insert_handle,
                                            UINT32 chnl_id,
                                            ohfs_insert_int_chnl_t *int_table_ptr,
                                            ohfs_insert_int_chnl_t *int_en_table_ptr);

PUBLIC PMC_ERROR ohfs_insert_int_chnl_retrieve(ohfs_insert_handle_t *ohfs_insert_handle,
                                               UINT32 chnl_id,
                                               ohfs_insert_int_chnl_t *filt_table_ptr,
                                               ohfs_insert_int_chnl_t *int_table_ptr);

PUBLIC PMC_ERROR ohfs_insert_int_chnl_enabled_check(ohfs_insert_handle_t *ohfs_insert_handle,
                                                    UINT32 chnl_id,
                                                    ohfs_insert_int_chnl_t *int_en_table_ptr,
                                                    BOOL8 *int_found_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _OHFS_INSERT_H */

/*
** end of file
*/
