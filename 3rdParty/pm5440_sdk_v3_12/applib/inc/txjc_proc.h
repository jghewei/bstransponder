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

#ifndef _TXJC_PROC_H
#define _TXJC_PROC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "util_opsa.h"
#include "util_global.h"
#include "txjc_proc_api.h"
#include "cfc.h"


/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: txjc_proc_instance_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying block instance type.
*
* ELEMENTS:
*    TXJC_PROC_INSTANCE_TGMP   - TGMP instance
*    TXJC_PROC_INSTANCE_MUX    - MUX instance
*
*******************************************************************************/
typedef enum {
    TXJC_PROC_INSTANCE_TGMP   = 0,
    TXJC_PROC_INSTANCE_MUX    = 1,
} txjc_proc_instance_type_t;


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
* STRUCTURE: txjc_proc_cfc_cfg_struct_t
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
typedef struct txjc_proc_cfc_cfg_struct_t {   
    UINT32 max_fifo_num;
    UINT32 page_size;
}  txjc_proc_cfc_cfg_struct_t;


/*******************************************************************************
* STRUCTURE: txjc_proc_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Static TXJC_PROC module instance data
*
* ELEMENTS:
*   txjc_proc_cfc_cfg_struct_t      - CFC configuration parameters
*   txjc_proc_instance_type_t       - block instance type
*
*       
*******************************************************************************/
typedef struct txjc_proc_cfg_t{
   txjc_proc_cfc_cfg_struct_t       cfc_cfg;
   txjc_proc_instance_type_t        instance;
} txjc_proc_cfg_t;




/*******************************************************************************
* STRUCTURE: txjc_proc_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   TXJC_PROC Module Instance Handle
*
* ELEMENTS:
*       base                  - See pmc_handle_t
*       cfg                   - See ohfs_insert_cfg_t
*       *cfc_handle           - see cfc_handle_t
*       
*******************************************************************************/
typedef struct txjc_proc_handle_struct_t {
    pmc_handle_t base;
    txjc_proc_cfg_t cfg;
    cfc_handle_t     *cfc_handle;  
} txjc_proc_handle_struct_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC txjc_proc_handle_t *txjc_proc_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void txjc_proc_ctxt_destroy(txjc_proc_handle_t *txjc_proc_handle);
PUBLIC void txjc_proc_handle_init(txjc_proc_handle_t *txjc_proc_handle,
                                  txjc_proc_instance_type_t txjc_proc_instance);
PUBLIC void txjc_proc_alloc_init(txjc_proc_handle_t *txjc_proc_handle);
PUBLIC void txjc_proc_handle_restart_init(txjc_proc_handle_t *txjc_proc_handle,
                                          util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                          pmc_energy_state_t top_energy_state);

PUBLIC PMC_ERROR txjc_proc_init(txjc_proc_handle_t *txjc_proc_handle, 
                                pmc_energy_state_t energy_state);

PUBLIC PMC_ERROR txjc_proc_ram_init(txjc_proc_handle_t *txjc_proc_handle);
                                
PUBLIC BOOL txjc_proc_start_state_test(txjc_proc_handle_t *txjc_proc_handle);
 
PUBLIC PMC_ERROR txjc_proc_chnl_prov(txjc_proc_handle_t *txjc_proc_handle, 
                                     UINT32 chnl,
                                     util_global_odukp_type_t server_type,
                                     UINT32 client_type,
                                     DOUBLE client_rate,
                                     util_global_mapping_mode_t mapping_mode, 
                                     util_global_odu_line_payload_t ts_type,
                                     UINT32 m);

PUBLIC PMC_ERROR txjc_proc_chnl_deprov(txjc_proc_handle_t *txjc_proc_handle, 
                                       UINT32 chnl);       

PUBLIC PMC_ERROR txjc_proc_cn_select(txjc_proc_handle_t *txjc_proc_handle,
                                     UINT32 chnl, UINT32 cn_sel);    
                                     
PUBLIC PMC_ERROR txjc_proc_chnl_activate(txjc_proc_handle_t *txjc_proc_handle, 
                                         UINT32 chnl);
                                          
PUBLIC PMC_ERROR txjc_proc_chnl_deactivate(txjc_proc_handle_t *txjc_proc_handle, 
                                           UINT32 chnl); 
        
PUBLIC void txjc_proc_chnl_clean(txjc_proc_handle_t *txjc_proc_handle, 
                                 UINT32 *chnl,
                                 UINT32 num_chnl);

PUBLIC PMC_ERROR txjc_proc_mux_lo_chnl_deprov(txjc_proc_handle_t *txjc_proc_handle, 
                                              UINT32 chnl);      
PUBLIC void txjc_proc_cfg_get(txjc_proc_handle_t *txjc_proc_handle,
                              UINT32 chnl, UINT32 *cn_n, UINT32 *cn_d,
                              UINT32 *golden_cn, UINT32 *pjo_cnt, 
                              UINT32 *njo_cnt);
PUBLIC void txjc_proc_lo_chnl_clean(txjc_proc_handle_t *txjc_proc_handle, 
                                    UINT32 *lo_chnl,
                                    UINT32 num_lo_chnl );

PUBLIC PMC_ERROR txjc_proc_int_chnl_enable(txjc_proc_handle_t *txjc_proc_handle,
                                           UINT32 chnl_id,
                                           txjc_proc_int_chnl_t *int_table_ptr,
                                           txjc_proc_int_chnl_t *int_en_table_ptr,
                                           BOOL8 enable);

PUBLIC PMC_ERROR txjc_proc_int_chnl_clear(txjc_proc_handle_t *txjc_proc_handle,
                                          UINT32 chnl_id,
                                          txjc_proc_int_chnl_t *int_table_ptr,
                                          txjc_proc_int_chnl_t *int_en_table_ptr);
    
PUBLIC PMC_ERROR txjc_proc_int_chnl_retrieve(txjc_proc_handle_t *txjc_proc_handle,
                                             UINT32 chnl_id,
                                             txjc_proc_int_chnl_t *filt_table_ptr,
                                             txjc_proc_int_chnl_t *int_table_ptr);
    
PUBLIC PMC_ERROR txjc_proc_int_chnl_enabled_check(txjc_proc_handle_t *txjc_proc_handle,
                                                  UINT32 chnl_id,
                                                  txjc_proc_int_chnl_t  *int_en_table_ptr,
                                                  BOOL8 *int_found_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _TXJC_PROC_H */

/*
** end of file
*/
