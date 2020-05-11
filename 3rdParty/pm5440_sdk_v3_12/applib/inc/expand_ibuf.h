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

#ifndef _EXPAND_IBUF_H
#define _EXPAND_IBUF_H

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
#include "expand_ibuf_api.h"
#include "cfc.h"
#include "util_gen_db.h"


/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: expand_ibuf_instance_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying block instance type.
*
* ELEMENTS:
*    EXPAND_IBUF_INSTANCE_TGMP - TGMP instance
*    EXPAND_IBUF_INSTANCE_MUX  - MUX instance
*
*******************************************************************************/
typedef enum {
    EXPAND_IBUF_INSTANCE_TGMP   = 0,
    EXPAND_IBUF_INSTANCE_MUX    = 1,
} expand_ibuf_instance_type_t;

/*
** Constants
*/
/* Number of supported channels */
#define EXPAND_IBUF_NUM_CHANL        96 


/*
** Macro Definitions
*/

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: expand_ibuf_cfc_cfg_struct_t
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
typedef struct expand_ibuf_cfc_cfg_struct_t {   
    UINT32 max_fifo_num;
    UINT32 page_size;
}  expand_ibuf_cfc_cfg_struct_t;


/*******************************************************************************
* STRUCTURE: expand_ibuf_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Static EXPAND_IBUF module instance data
*
* ELEMENTS:
*   expand_ibuf_cfc_cfg_struct_t      - CFC configuration parameters
*   expand_ibuf_instance_type_t       - block instance type
*
*       
*******************************************************************************/
typedef struct expand_ibuf_cfg_t {
   expand_ibuf_cfc_cfg_struct_t       cfc_cfg;
   expand_ibuf_instance_type_t        instance; 
} expand_ibuf_cfg_t;


/* Channel context*/
/*******************************************************************************
* STRUCTURE: expand_ibuf_chnl_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   EXPAND_IBUF module dynamically changing channel context data
*
* ELEMENTS:
*   server_size      - the size of the incoming channels container
*   eomf_cycle       - end of multi-frame calendar cycle
*   M                - M value of the server container
*
*       
*******************************************************************************/
typedef struct expand_ibuf_chnl_struct_t {  
    UINT32 server_size;
    UINT32 eomf_cycle;
    UINT32 M; 
} expand_ibuf_chnl_t;


/*******************************************************************************
* STRUCTURE: expand_ibuf_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   EXPAND_IBUF Module Instance dynamic data
*
* ELEMENTS:
*   expand_ibuf_chnl_ctxt     - see expand_ibuf_chnl_t
*
*       
*******************************************************************************/
typedef struct expand_ibuf_var_t{
    expand_ibuf_chnl_t expand_ibuf_chnl_ctxt[EXPAND_IBUF_NUM_CHANL];     
} expand_ibuf_var_t;


/*******************************************************************************
* STRUCTURE: expand_ibuf_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   EXPAND_IBUF Module Instance Handle
*
* ELEMENTS:
*       base                  - See pmc_handle_t
*       cfg                   - See expand_ibuf_cfg_t
*       var                   - See expand_ibuf_var_t
*       *cfc_handle           - see cfc_handle_t
*       
*******************************************************************************/
typedef struct expand_ibuf_handle_struct_t {
    pmc_handle_t base;
    expand_ibuf_cfg_t cfg;
    expand_ibuf_var_t var;
    cfc_handle_t     *cfc_handle;
} expand_ibuf_handle_struct_t;



/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC expand_ibuf_handle_t *expand_ibuf_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void expand_ibuf_ctxt_destroy(expand_ibuf_handle_t *expand_ibuf_handle);
PUBLIC void expand_ibuf_handle_init(expand_ibuf_handle_t *expand_ibuf_handle,
                                    expand_ibuf_instance_type_t expand_ibuf_instance);
PUBLIC void expand_ibuf_alloc_init(expand_ibuf_handle_t *expand_ibuf_handle);
PUBLIC void expand_ibuf_handle_restart_init(expand_ibuf_handle_t *expand_ibuf_handle,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                            pmc_energy_state_t top_energy_state);

PUBLIC PMC_ERROR expand_ibuf_init(expand_ibuf_handle_t *expand_ibuf_handle, 
                                  pmc_energy_state_t energy_state);

PUBLIC BOOL expand_ibuf_start_state_test(expand_ibuf_handle_t *expand_ibuf_handle);

PUBLIC PMC_ERROR expand_ibuf_lo_chnl_prov(expand_ibuf_handle_t *expand_ibuf_handle, 
                                          UINT32 chnl,
                                          util_global_odukp_type_t server_type,
                                          UINT32 client_type,
                                          DOUBLE client_rate,
                                          util_global_mapping_mode_t mapping_mode, 
                                          UINT32 num_tribslot,
                                          util_global_odu_line_payload_t ts_type, 
                                          UINT32 num_cal_entries,
                                          BOOL8 offline_update);

PUBLIC PMC_ERROR expand_ibuf_lo_chnl_deprov(expand_ibuf_handle_t *expand_ibuf_handle,
                                            UINT32 chnl);


PUBLIC PMC_ERROR expand_ibuf_ho_chnl_prov_all(expand_ibuf_handle_t *expand_ibuf_handle,
                                              UINT32 ho_cycle, UINT32 chnl,
                                              util_global_odukp_type_t odu_type,
                                              util_global_odu_line_payload_t ts_type,
                                              UINT32 first_cycle, UINT32 last_cycle,
                                              BOOL8 unset_ho_cal_cycle_tot);

PUBLIC PMC_ERROR expand_ibuf_ho_cycle_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                           UINT32 cycle,
                                           UINT32 first_cycle,
                                           UINT32 last_cycle, UINT32 chnl);

PUBLIC PMC_ERROR expand_ibuf_ho_cal_cycle_tot_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                                  UINT32 cycle, UINT32 chnl,
                                                  util_global_odukp_type_t ho_odu_type,
                                                  util_global_odu_line_payload_t ts_type,
                                                  UINT32 unset);

PUBLIC PMC_ERROR expand_ibuf_ho_mf_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                       UINT32 cycle,
                                       util_global_odukp_type_t server_type,
                                       util_global_odu_line_payload_t ts_type,
                                       UINT32 unset);

PUBLIC PMC_ERROR expand_ibuf_lo_cycle_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                          UINT32 cycle, UINT32 first_cycle); 

PUBLIC PMC_ERROR expand_ibuf_lo_cycle_cfg_all(expand_ibuf_handle_t *expand_ibuf_handle,
                                          UINT32 cycle, UINT32 first_cycle,
                                          UINT32 eomf_cycle); 

PUBLIC PMC_ERROR expand_ibuf_mem_ho_id_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                           UINT32 chnl);

PUBLIC PMC_ERROR expand_ibuf_lo_eomf_data_cycle_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                                    UINT32 eomf_cycle, UINT32 value);

PUBLIC PMC_ERROR expand_ibuf_ho_last_cycle_set(expand_ibuf_handle_t *expand_ibuf_handle,
                                               UINT32 cycle, UINT32 last_cycle);

PUBLIC PMC_ERROR expand_ibuf_cfc_xoff_level_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                                UINT32 chnl,
                                                UINT32 trib_slot);

PUBLIC PMC_ERROR expand_ibuf_cfc_fill_lvl_set(expand_ibuf_handle_t *expand_ibuf_handle,
                                              UINT32 chnl,
                                              UINT32 cfc_fill_lvl);

PUBLIC PMC_ERROR expand_ibuf_first_cycle_cfg(expand_ibuf_handle_t *expand_ibuf_handle,
                                             UINT32 chnl,
                                             UINT32 first_cycle);

PUBLIC UINT32 expand_ibuf_block_non_default_cfg(expand_ibuf_handle_t *expand_ibuf_handle);

PUBLIC PMC_ERROR expand_ibuf_ram_init(expand_ibuf_handle_t *expand_ibuf_handle);

PUBLIC PMC_ERROR expand_ibuf_chnl_activate(expand_ibuf_handle_t *expand_ibuf_handle, 
                                           UINT32 chnl);

PUBLIC PMC_ERROR expand_ibuf_chnl_deactivate(expand_ibuf_handle_t *expand_ibuf_handle, 
                                             UINT32 chnl); 
                                                                                                                                           
PUBLIC void expand_ibuf_chnl_clean(expand_ibuf_handle_t *expand_ibuf_handle,
                                   UINT32 *chnl,
                                   UINT32 num_chnl); 

PUBLIC PMC_ERROR expand_ibuf_mux_lo_chnl_deprov(expand_ibuf_handle_t *expand_ibuf_handle,
                                                UINT32 chnl, UINT32 offline_update);

PUBLIC void expand_ibuf_ts_type_get(expand_ibuf_handle_t *expand_ibuf_handle, 
                                    UINT32 chnl,
                                    UINT32 cycle, 
                                    util_global_odu_line_payload_t *ts_type);

PUBLIC void expand_ibuf_param_cfg_get(expand_ibuf_handle_t *expand_ibuf_handle, 
                                      UINT32 chnl, 
                                      UINT32 ho_num_tribslot,
                                      UINT32 num_tribslot,
                                      util_global_mapping_mode_t mapping_mode,
                                      util_global_odu_line_payload_t ts_type,
                                      UINT32 cn_n, UINT32 cn_d,
                                      UINT32 golden_cn, UINT32 pjo_cnt, 
                                      UINT32 njo_cnt,
                                      util_global_odukp_type_t *server_type,
                                      util_global_odukp_type_t *client_type);
PUBLIC void expand_ibuf_chnl_cycle_clean(expand_ibuf_handle_t *expand_ibuf_handle, 
                                         UINT32 *ho_chnl,
                                         UINT32 num_ho_chnl,
                                         UINT32 *lo_chnl,
                                         UINT32 num_lo_chnl);
PUBLIC void expand_ibuf_lo_cycle_clean(expand_ibuf_handle_t *expand_ibuf_handle, 
                                       UINT32 *lo_chnl,
                                       UINT32 num_lo_chnl,
                                       UINT32 *cycle,       
                                       UINT32 num_cycle);
    
PUBLIC PMC_ERROR expand_ibuf_int_chnl_enable(expand_ibuf_handle_t *expand_ibuf_handle,
                                             UINT32 chnl_id,
                                             expand_ibuf_int_chnl_t *int_table_ptr,
                                             expand_ibuf_int_chnl_t *int_en_table_ptr,
                                             BOOL8 enable);

PUBLIC PMC_ERROR expand_ibuf_int_chnl_clear(expand_ibuf_handle_t *expand_ibuf_handle,
                                            UINT32  chnl_id,
                                            expand_ibuf_int_chnl_t *int_table_ptr,
                                            expand_ibuf_int_chnl_t *int_en_table_ptr);
    
PUBLIC PMC_ERROR expand_ibuf_int_chnl_retrieve(expand_ibuf_handle_t *expand_ibuf_handle,
                                               UINT32 chnl_id,
                                               expand_ibuf_int_chnl_t *filt_table_ptr,
                                               expand_ibuf_int_chnl_t *int_table_ptr);
    
PUBLIC PMC_ERROR expand_ibuf_int_chnl_enabled_check(expand_ibuf_handle_t *expand_ibuf_handle,
                                                    UINT32 chnl_id,
                                                    expand_ibuf_int_chnl_t *int_en_table_ptr,
                                                    BOOL8 *int_found_ptr);  

PUBLIC PMC_ERROR expxand_cfc_fifo_ipt_id_get(expand_ibuf_handle_t *expand_ibuf_handle,
                                             UINT32 fifo_id,
                                             UINT32 frm_num,
                                             UINT32 *ipt_id);

#ifdef __cplusplus
}
#endif

#endif /* _EXPAND_IBUF_H */

/*
** end of file
*/
