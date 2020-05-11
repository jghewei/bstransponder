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

#ifndef _CFC_H
#define _CFC_H

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
#include "cfc_api.h"

/*
** Enumerated Types
*/

/*
** Constants
*/

/*
** Macro Definitions
*/

#define CFC_INT_TABLE_DEFINE() \
    CFC_INT_RANGE(cfc_handle, cfc, lfield_range, FIFO_OVR , fifo_ovr_i); \
    CFC_INT_RANGE(cfc_handle, cfc, lfield_range, FIFO_UDR , fifo_udr_i); \
    CFC_INT_RANGE(cfc_handle, cfc, lfield_range, IPT_ERR , ipt_err_i); \

/*
** Structures and Unions
*/
typedef struct cfc_handle_struct_t cfc_handle_t; /*!< Opaque reference to module handle see cfc_handle_struct_t */
typedef void cfc_ext_handle_t; /*!< Module handle required for register access function pointers */

/* cfg function pointers definition*/
typedef void (*cfc_fifo_void_fct_ptr1_t) (cfc_handle_t *);        /*!< function pointer for register space accessor */
typedef void (*cfc_fifo_void_fct_ptr2_t) (cfc_handle_t *,UINT32); /*!< function pointer for register space accessor */
typedef void (*cfc_fifo_void_fct_ptr3_t) (cfc_handle_t *,UINT32,UINT32); /*!< function pointer for register space accessor */
typedef void (*cfc_fifo_void_fct_ptr4_t) (cfc_handle_t *,UINT32,UINT32 *); /*!< function pointer for register space accessor */
typedef void (*cfc_fifo_void_fct_ptr5_t) (cfc_handle_t *,UINT32,UINT32 *,UINT32 *); /*!< function pointer for register space accessor */
typedef UINT32 (*cfc_fifo_uint32_fct_ptr1_t) (cfc_handle_t *); /*!< function pointer for register space accessor */
typedef PMC_ERROR (*cfc_fifo_pmc_error_fct_ptr1_t) (cfc_handle_t *,UINT32); /*!< function pointer for register space accessor */
typedef PMC_ERROR (*cfc_fifo_pmc_error_fct_ptr2_t) (cfc_handle_t *,UINT32,UINT32,UINT32,UINT32); /*!< function pointer for register space accessor */
typedef PMC_ERROR (*cfc_fifo_pmc_error_fct_ptr3_t) (cfc_handle_t *,UINT32,UINT32); /*!< function pointer for register space accessor */
typedef PMC_ERROR (*cfc_fifo_pmc_error_fct_ptr4_t) (cfc_handle_t *,UINT32,cfc_int_chnl_t*,cfc_int_chnl_t*,BOOL8); /*!< function pointer for register space accessor */
typedef PMC_ERROR (*cfc_fifo_pmc_error_fct_ptr5_t) (cfc_handle_t *,UINT32,cfc_int_chnl_t*,cfc_int_chnl_t*); /*!< function pointer for register space accessor */
typedef PMC_ERROR (*cfc_fifo_pmc_error_fct_ptr6_t) (cfc_handle_t *,UINT32,cfc_int_chnl_t*,BOOL8*); /*!< function pointer for register space accessor */

/*******************************************************************************
* STRUCTURE: cfc_fifo_cfg_fct_table_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Table with CFC configuration function pointers for register space accessor
*
* ELEMENTS:
*   cfc_fifo_end_addr_chg_abort_cfg_ptr      - abort of a FIFO end address change request
*   cfc_fifo_end_addr_chg_success_get_ptr    - the outcome of the previous address change request
*   cfc_fifo_end_addr_req_get_ptr            - the fifo end address request status
*   cfc_fifo_end_addr_req_cfg_ptr            - create an end address change request
*   cfc_fifo_overflow_mon_cfg_ptr            - monitors CFC FIFO overflow interupt
*   cfc_ipt_entry_state_get_cfg_ptr          - the current state of an IPT entry whether
*                                              or not the entry is occupied
*   cfc_ipt_entry_get_cfg_ptr                - the current state of an IPT entry whether
*                                              or not the entry is occupied
*   cfc_ovr_int_enable_cfg_ptr               - the overflow interrupt for a FIFO ID
*   cfc_udr_int_enable_cfg_ptr               - the underflow interrupt for a FIFO ID
*   cfc_ipt_int_enable_cfg_ptr               - the IPT error interrupt for a FIFO ID
*   cfc_bypass_cfg_ptr                       - the IPT bypass mode
*   cfc_ipt_page_mask_cfg_ptr                - the IPT page mask
*   cfc_inverted_page_table_cfg_ptr          - the inverted page table for the channelized FIFO 
*   cfc_fifo_reset_cfg_ptr                   - FIFO reset configuration
*   cfc_int_chnl_enable_ptr                  - channel interrupt table enable
*   cfc_int_chnl_clear_ptr                   - clear channel interrupt table
*   cfc_int_chnl_retrieve_ptr                - retrieve channel interrupts
*   cfc_int_chnl_enabled_check_ptr           - active interrupt bits based on the enabled interrupt table
*   ext_handle                               - Module handle for register access function pointers
*   opaque                                   - Opaque reference to module handle
*       
*******************************************************************************/
typedef struct cfc_fifo_cfg_fct_table_t {
    cfc_fifo_void_fct_ptr1_t      cfc_fifo_end_addr_chg_abort_cfg_ptr;
    cfc_fifo_uint32_fct_ptr1_t    cfc_fifo_end_addr_chg_success_get_ptr;
    cfc_fifo_uint32_fct_ptr1_t    cfc_fifo_end_addr_req_get_ptr;
    cfc_fifo_void_fct_ptr3_t      cfc_fifo_end_addr_req_cfg_ptr;
    cfc_fifo_pmc_error_fct_ptr1_t cfc_fifo_overflow_mon_cfg_ptr;
    cfc_fifo_void_fct_ptr4_t      cfc_ipt_entry_state_get_cfg_ptr;
    cfc_fifo_void_fct_ptr5_t      cfc_ipt_entry_get_cfg_ptr;
    cfc_fifo_void_fct_ptr3_t      cfc_ovr_int_enable_cfg_ptr;
    cfc_fifo_void_fct_ptr3_t      cfc_udr_int_enable_cfg_ptr;
    cfc_fifo_void_fct_ptr3_t      cfc_ipt_int_enable_cfg_ptr;
    cfc_fifo_void_fct_ptr2_t      cfc_bypass_cfg_ptr;
    cfc_fifo_void_fct_ptr1_t      cfc_ipt_page_mask_cfg_ptr;
    cfc_fifo_pmc_error_fct_ptr2_t cfc_inverted_page_table_cfg_ptr;
    cfc_fifo_pmc_error_fct_ptr3_t cfc_fifo_reset_cfg_ptr;
    cfc_fifo_pmc_error_fct_ptr4_t cfc_int_chnl_enable_ptr;
    cfc_fifo_pmc_error_fct_ptr5_t cfc_int_chnl_clear_ptr;
    cfc_fifo_pmc_error_fct_ptr5_t cfc_int_chnl_retrieve_ptr;
    cfc_fifo_pmc_error_fct_ptr6_t cfc_int_chnl_enabled_check_ptr;
    cfc_ext_handle_t              *ext_handle;
    UINT32                        opaque;
} cfc_fifo_cfg_fct_table_t;

/*******************************************************************************
* STRUCTURE: cfc_cfg_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Static CFC module instance data
*
* ELEMENTS:
*   max_fifo_num        - Number of FIFOs in CFC
*   page_size           - Number of entries per page
*   fifo_null_entry     - Null entry of the FIFOs
*   bus_width           - Bus width on the egress DCI interface
*   cfg_fct_table       - See cfc_fifo_cfg_fct_table_t
*       
*******************************************************************************/
typedef struct cfc_cfg_struct_t {    
    UINT32 max_fifo_num;
    UINT32 page_size;
    UINT32 fifo_null_entry;
    UINT32 bus_width; 
    cfc_fifo_cfg_fct_table_t cfg_fct_table;
} cfc_cfg_t;

/*******************************************************************************
* STRUCTURE: inverted_page_table_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Inverted page table configuration parameters
*
* ELEMENTS:
*   ipt_valid      - Qualifies the inverted page table entry
*   ipt_id         - channel ID associated with the physical block
*   ipt_va         - the block number associated with the physical block
*       
*******************************************************************************/
typedef struct inverted_page_table_struct_t {
     BOOL   ipt_valid;
     UINT32 ipt_id; 
     UINT32 ipt_va;
} inverted_page_table_t;

/*******************************************************************************
* STRUCTURE: cfc_var_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CFC Module Instance dynamic data
*
* ELEMENTS:
*   inverted_page_table      - Inverted page table context see inverted_page_table_t
*       
*******************************************************************************/
typedef struct cfc_var_struct_t {
    inverted_page_table_t *inverted_page_table;
    
} cfc_var_t;


/*******************************************************************************
* STRUCTURE: cfc_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CFC Module Instance Handle
*
* ELEMENTS:
*       base               - See pmc_handle_t
*       cfg                - See cfc_cfg_t
*       var                - See cfc_var_t
*       
*******************************************************************************/
typedef struct cfc_handle_struct_t {
    pmc_handle_t base; 
    cfc_cfg_t cfg;
    cfc_var_t var;
} cfc_handle_struct_t ;



/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC cfc_handle_t *cfc_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void cfc_ctxt_destroy(cfc_handle_t *cfc_handle);
PUBLIC void cfc_handle_init(cfc_handle_t *cfc_handle, 
                            cfc_cfg_t *cfc_cfg_ptr);
PUBLIC void cfc_alloc_init(cfc_handle_t *cfc_handle);
PUBLIC void cfc_handle_restart_init(cfc_handle_t *cfc_handle, 
                                    cfc_cfg_t *cfc_cfg_ptr,
                                    util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                    pmc_energy_state_t top_energy_state);
PUBLIC void cfc_handle_init_ext_cfg(cfc_handle_t *cfc_handle, 
                                    cfc_cfg_t *cfc_cfg_ptr,
                                    cfc_fifo_cfg_fct_table_t *cfg_fct_table_ptr);
PUBLIC void cfc_handle_restart_init_ext_cfg(cfc_handle_t *cfc_handle, 
                                            cfc_cfg_t *cfc_cfg_ptr,
                                            cfc_fifo_cfg_fct_table_t *cfg_fct_table_ptr,
                                            util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                            pmc_energy_state_t top_energy_state);
PUBLIC PMC_ERROR cfc_init(cfc_handle_t *cfc_handle, pmc_energy_state_t energy_state);
PUBLIC BOOL cfc_start_state_test(cfc_handle_t *cfc_handle);
 

PUBLIC PMC_ERROR cfc_fifo_build_end_addr(cfc_handle_t *cfc_handle, 
                                         UINT32 fifo_id, 
                                         UINT32 num_pages,
                                         UINT32 end_addr);
                                         
PUBLIC PMC_ERROR cfc_fifo_build(cfc_handle_t *cfc_handle, 
                                UINT32 fifo_id, 
                                UINT32 num_pages);
                                
PUBLIC PMC_ERROR cfc_fifo_destroy(cfc_handle_t *cfc_handle, 
                                  UINT32 fifo_id);
PUBLIC void cfc_fifo_clean(cfc_handle_t *cfc_handle, 
                           UINT32 *fifo_id,
                           UINT32 num_fifo_id);   
PUBLIC PMC_ERROR cfc_fifo_resize(cfc_handle_t *cfc_handle, 
                                 UINT32 fifo_id, 
                                 UINT32 num_pages); 

PUBLIC PMC_ERROR cfc_fifo_resize_ext(cfc_handle_t *cfc_handle, 
                                     UINT32 fifo_id, 
                                     UINT32 num_pages,
                                     UINT32 poll_interval_us,
                                     UINT32 poll_iterations);
                                                             
PUBLIC PMC_ERROR cfc_fifo_resize_one_ts_step1(cfc_handle_t *cfc_handle,
                                              UINT32 fifo_id,
                                              UINT32 num_pages);

PUBLIC PMC_ERROR cfc_fifo_resize_one_ts_step2(cfc_handle_t *cfc_handle,
                                              UINT32 fifo_id,
                                              UINT32 num_pages,
                                              UINT32 num_current_pages);

PUBLIC PMC_ERROR cfc_inverted_page_table_init(cfc_handle_t *cfc_handle);
                                                                       
PUBLIC PMC_ERROR cfc_bypass(cfc_handle_t *cfc_handle, UINT32 bypass);

PUBLIC PMC_ERROR cfc_fifo_end_addr_cfg(cfc_handle_t *cfc_handle, 
                                       UINT32 fifo_id,  
                                       UINT32 *end_addr_ptr,
                                       BOOL8 resize_mode);

PUBLIC PMC_ERROR cfc_fifo_end_addr_cfg_ext(cfc_handle_t *cfc_handle, 
                                           UINT32 fifo_id,  
                                           UINT32 end_address,
                                           UINT32 poll_interval_us,
                                           UINT32 poll_iterations, 
                                           BOOL8 resize_mode);

PUBLIC PMC_ERROR cfc_ipt_cfg(cfc_handle_t *cfc_handle, UINT32 index, 
                             UINT32 ipt_valid, UINT32 ipt_id, UINT32 ipt_va);
                             
PUBLIC PMC_ERROR cfc_fifo_activate(cfc_handle_t *cfc_handle, UINT32 fifo_id);

PUBLIC PMC_ERROR cfc_fifo_deactivate(cfc_handle_t *cfc_handle, 
                                     UINT32 fifo_id);

PUBLIC PMC_ERROR cfc_ipt_entry_state_get(cfc_handle_t *cfc_handle, UINT32 index,
                                         UINT32 *ipt_valid);
                             
PUBLIC PMC_ERROR cfc_fifo_overflow_mon(cfc_handle_t *cfc_handle, UINT32 fifo_id);

PUBLIC UINT32 cfc_fifo_end_addr_only_cfg(cfc_handle_t *cfc_handle, 
                                         UINT32 chnl_id, UINT32 num_cal_entries);

PUBLIC PMC_ERROR cfc_fifo_current_num_pages_get(cfc_handle_t *cfc_handle, 
                                                 UINT32 fifo_id, 
                                                 UINT32 *num_current_pages_ptr);                                               

PUBLIC PMC_ERROR cfc_fifo_excess_page_get(cfc_handle_t *cfc_handle, 
                                           UINT32 fifo_id,
                                           UINT32 num_pages, 
                                           UINT32 *excess_page_array);                                          

PUBLIC PMC_ERROR cfc_fifo_reset_cfg(cfc_handle_t *cfc_handle, 
                                     UINT32 fifo_id, 
                                     UINT32 reset_value);

                                                                        
PUBLIC PMC_ERROR cfc_fifo_current_num_pages_get(cfc_handle_t *cfc_handle, 
                                                 UINT32 fifo_id, 
                                                 UINT32 *num_current_pages_ptr); 
                                                

PUBLIC PMC_ERROR cfc_ipt_entry_get(cfc_handle_t *cfc_handle, 
                                   UINT32 index,
                                   UINT32 *ipt_valid,
                                   UINT32 *ipt_id);

PUBLIC PMC_ERROR cfc_int_chnl_enable(cfc_handle_t *cfc_handle,
                                     UINT32 fifo_id,
                                     cfc_int_chnl_t *int_table_ptr,
                                     cfc_int_chnl_t *int_en_table_ptr,
                                     BOOL8 enable);

PUBLIC PMC_ERROR cfc_int_chnl_clear(cfc_handle_t *cfc_handle,
                                    UINT32 fifo_id,
                                    cfc_int_chnl_t *int_table_ptr,
                                    cfc_int_chnl_t *int_en_table_ptr);
   
PUBLIC PMC_ERROR cfc_int_chnl_retrieve(cfc_handle_t *cfc_handle,
                                       UINT32 fifo_id,    
                                       cfc_int_chnl_t *int_en_table_ptr,
                                       cfc_int_chnl_t *int_table_ptr);

PUBLIC PMC_ERROR cfc_int_chnl_enabled_check(cfc_handle_t *cfc_handle,
                                            UINT32 fifo_id,
                                            cfc_int_chnl_t *int_en_table_ptr,
                                            BOOL8 *int_found_ptr); 
       
PUBLIC PMC_ERROR cfc_fifo_ipt_id_get(cfc_handle_t *cfc_handle,
                                     UINT32 fifo_id,
                                     UINT32 frm_num,
                                     UINT32 *ipt_id);       

#ifdef __cplusplus
}
#endif

#endif /* _CFC_H */

/*
** end of file
*/
