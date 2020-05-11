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

#ifndef _ODUJAT_H
#define _ODUJAT_H

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
#include "odujat_api.h"
#include "oduksc.h"
#include "cfc.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: odujat_type_id_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying block instance type.
*
* ELEMENTS:
*    ODUJAT_FMF1   - FMF1
*    ODUJAT_FMF2   - FMF2
*
*******************************************************************************/
typedef enum {
    ODUJAT_FMF1    = 0,
    ODUJAT_FMF2    = 1,   
    LAST_ODUJAT,   
} odujat_type_id_t;

/*******************************************************************************
* ENUM: odujat_chnl_state_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for Channel State Definition.
*
* ELEMENTS:
*    ODUJAT_CHNL_START                - Start state
*    ODUJAT_CHNL_PROVISIONED          - Provisioned state
*    ODUJAT_CHNL_PROVISION_PENDING    - Provision pending state
*    ODUJAT_CHNL_DEPROVISION_PENDING  - Deprovision pending state
*    ODUJAT_CHNL_ACTIVATED            - Activated state
*    ODUJAT_CHNL_REPROV_PENDING       - Reprovisioned pending state
*
*******************************************************************************/
typedef enum
{
    ODUJAT_CHNL_START                = 0,
    ODUJAT_CHNL_PROVISIONED          = 1,
    ODUJAT_CHNL_PROVISION_PENDING    = 2,
    ODUJAT_CHNL_DEPROVISION_PENDING  = 3,
    ODUJAT_CHNL_ACTIVATED            = 4, 
    ODUJAT_CHNL_REPROV_PENDING       = 5
} odujat_chnl_state_t;


/*******************************************************************************
* ENUM: odujat_chnl_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for enable or disable JAT.
*
* ELEMENTS:
*    ODUJAT_CHNL_IN_BYPASS   - JAT in bypass mode
*    ODUJAT_CHNL_ENABLED     - JAT enabled
*
*******************************************************************************/
typedef enum
{
    ODUJAT_CHNL_IN_BYPASS       = 0,
    ODUJAT_CHNL_ENABLED         = 1,
} odujat_chnl_mode_t;

/*
** Constants
*/

/*
** Macro Definitions
*/
/* ODUJAT_INT(handle, tsb_name, tsb_field, struct_field) */
#define ODUJAT_INT_TABLE_DEFINE()                                  \
    ODUJAT_INT(odujat_handle, odujat, DQ_LIMIT, dq_limit_i);       \
    ODUJAT_INT(odujat_handle, odujat, GAIN_UNDF, gain_undf_i );    \
    ODUJAT_INT(odujat_handle, odujat, GAIN_OVF, gain_ovf_i);         \
    
/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: odujat_ch_ctxt_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODUJAT parameters specific to each channel
*
* ELEMENTS:
*   chnl_state        - Channel state see odujat_chnl_state_t
*   chnl_mode         - Channel mode see odujat_chnl_mode_t
*       
*******************************************************************************/
typedef struct odujat_ch_ctxt_struct_t{
    odujat_chnl_state_t     chnl_state;
    odujat_chnl_mode_t      chnl_mode;
} odujat_ch_ctxt_struct_t;

/*******************************************************************************
* STRUCTURE: odujat_cfc_cfg_struct_t                                         
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
typedef struct odujat_cfc_cfg_struct_t {   
    UINT32 max_fifo_num;
    UINT32 page_size;
}  odujat_cfc_cfg_struct_t;


/*******************************************************************************
* STRUCTURE: odujat_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Static ODUJAT module instance data
*
* ELEMENTS:
*   odujat_type_id            - ODUJAT type ID see odujat_type_id
*   max_chnl                  - maximum number of channels
*   cfc_cfg                   - CFC configuration parameters see odujat_cfc_cfg_struct_t
*   cal_entries_alloc_method  - Calendar entries distribution algorithm number
*   db_id                     - Database ID for the instance to access
*   db_handle                 - Database handle  
*       
*******************************************************************************/
typedef struct odujat_cfg_t{
    odujat_type_id_t              odujat_type_id;
    UINT32                        max_chnl;
    odujat_cfc_cfg_struct_t       cfc_cfg;                                                                                                         
    UINT32                        cal_entries_alloc_method; 
    util_gen_db_id_t              db_id;
    UINT32                        db_handle;
} odujat_cfg_t;

/*******************************************************************************
* STRUCTURE: odujat_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODUJAT Module Instance dynamic data
*
* ELEMENTS:
*   ch_ctxt_ptr      - Channel context pointer see odujat_ch_ctxt_struct_t
*       
*******************************************************************************/
typedef struct odujat_var_t{
    odujat_ch_ctxt_struct_t   *ch_ctxt_ptr;  
} odujat_var_t;


/*******************************************************************************
* STRUCTURE: odujat_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   ODUJAT Module Instance Handle
*
* ELEMENTS:
*       base               - See pmc_handle_t
*       cfg                - See odujat_cfg_t
*       var                - See odujat_var_t
*       *oduksc_handle     - See oduksc_handle_t
*       *cfc_handle        - See cfc_handle_t
*       
*******************************************************************************/
typedef struct odujat_handle_struct_t {
    pmc_handle_t base;
    odujat_cfg_t cfg;
    odujat_var_t var;
    oduksc_handle_t   *oduksc_handle;
    cfc_handle_t    *cfc_handle;     
} odujat_handle_struct_t;


/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC odujat_handle_t *odujat_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void odujat_ctxt_destroy(odujat_handle_t *odujat_handle);

PUBLIC void odujat_handle_init(odujat_handle_t *odujat_handle,
                               odujat_type_id_t odujat_id);

PUBLIC void odujat_alloc_init(odujat_handle_t *odujat_handle);

PUBLIC void odujat_handle_restart_init(odujat_handle_t *odujat_handle,
                                       util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                       pmc_energy_state_t top_energy_state);

PUBLIC PMC_ERROR odujat_init(odujat_handle_t *odujat_handle, UINT32 mode);

PUBLIC PMC_ERROR odujat_chnl_init(odujat_handle_t *odujat_handle,
                                  UINT32 chnl, odujat_chnl_mode_t mode);
                                  
PUBLIC PMC_ERROR odujat_chnl_mc_numer_cfg(odujat_handle_t *odujat_handle,
                                          UINT32 chnl,
                                          util_global_odukp_type_t odu_type,
                                          UINT32 odu_flex_data,
                                          UINT32 *mc_num);                                            
PUBLIC PMC_ERROR odujat_chnl_prov(odujat_handle_t *odujat_handle,
                                  UINT32 chnl,
                                  UINT32 mux_stage, UINT32 ho_chnl,
                                  util_global_odukp_type_t ho_odu_type,
                                  util_global_odukp_type_t lo_odu_type,
                                  UINT32 odu_flex_data,
                                  UINT32 client_rate,
                                  odujat_chnl_mode_t mode,
                                  BOOL8 super_prov);
                                                                    
PUBLIC PMC_ERROR odujat_chnl_activate(odujat_handle_t *odujat_handle,
                                      UINT32 chnl,
                                      BOOL8 super_prov);                                                                                   
                                                                                     
PUBLIC PMC_ERROR odujat_chnl_deprov(odujat_handle_t *odujat_handle, 
                                    UINT32 chnl,
                                    UINT32 mux_stage, 
                                    BOOL8 mo_odu3e2_3e1,
                                    BOOL8 super_deprov);                                    

PUBLIC PMC_ERROR odujat_chnl_deactivate(odujat_handle_t *odujat_handle,
                                        UINT32 chnl);

PUBLIC PMC_ERROR odujat_oduksc_page_swap(odujat_handle_t *odujat_handle,
                                         UINT32 *chnl_ids_ptr,
                                         UINT32 num_lo_chnls,
                                         BOOL update_scbs3);   
                                    
PUBLIC PMC_ERROR odujat_chnl_deactivate_super_deprov(odujat_handle_t *odujat_handle,
                                                     UINT32 chnl);     
                               
PUBLIC PMC_ERROR odujat_oduksc_chnl_state_set(odujat_handle_t *odujat_handle,
                                              UINT32 chnl,
                                              oduksc_chnl_state_t chnl_state);                               

PUBLIC PMC_ERROR odujat_fifo_limit_set(odujat_handle_t *odujat_handle,
                                       util_global_odukp_type_t lo_odu_type);
        
PUBLIC PMC_ERROR odujat_chnl_enable(odujat_handle_t *odujat_handle, 
                                    UINT32 chnl,
                                    BOOL8 mode);
        
PUBLIC void odujat_chnl_clean(odujat_handle_t *odujat_handle,
                              UINT32 *chnl,
                              UINT32 num_chnl);                                         

PUBLIC PMC_ERROR odujat_ghao_resize_fifo(odujat_handle_t *odujat_handle,
                                         UINT32 chnl,
                                         UINT32 oduflex_num_tribslots);
                                         
PUBLIC PMC_ERROR odujat_int_enable(odujat_handle_t *odujat_handle,
                                   odujat_int_t *int_table_ptr,
                                   odujat_int_t *int_en_table_ptr,
                                   BOOL8 enable);

PUBLIC PMC_ERROR odujat_int_clear(odujat_handle_t *odujat_handle,
                                  odujat_int_t *int_table_ptr,
                                  odujat_int_t *int_en_table_ptr);

PUBLIC PMC_ERROR odujat_int_retrieve(odujat_handle_t *odujat_handle,
                                     odujat_int_t *filt_table_ptr,
                                     odujat_int_t *int_table_ptr);

PUBLIC PMC_ERROR odujat_int_enabled_check(odujat_handle_t *odujat_handle,
                                          odujat_int_t *int_en_table_ptr,
                                          BOOL8 *int_found_ptr);

PUBLIC PMC_ERROR odujat_int_chnl_enable(odujat_handle_t *odujat_handle,
                                        UINT32 chnl_id,
                                        odujat_int_chnl_t *int_table_ptr,
                                        odujat_int_chnl_t *int_en_table_ptr,
                                        BOOL8 enable);

PUBLIC PMC_ERROR odujat_int_chnl_clear(odujat_handle_t *odujat_handle,
                                       UINT32 chnl_id,
                                       odujat_int_chnl_t *int_table_ptr,
                                       odujat_int_chnl_t *int_en_table_ptr);

PUBLIC PMC_ERROR odujat_int_chnl_retrieve(odujat_handle_t *odujat_handle,
                                          UINT32 chnl_id,
                                          odujat_int_chnl_t *filt_table_ptr,
                                          odujat_int_chnl_t *int_table_ptr);

PUBLIC PMC_ERROR odujat_int_chnl_enabled_check(odujat_handle_t *odujat_handle,
                                               UINT32 chnl_id,
                                               odujat_int_chnl_t *int_en_table_ptr,
                                               BOOL8 *int_found_ptr);

#ifdef __cplusplus
}
#endif

#endif /* _ODUJAT_H */

/*
** end of file
*/
