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

#ifndef _MP_MGEN_H
#define _MP_MGEN_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_handle.h"
#include "pmc_err.h"
#include "pmc_log.h"
#include "mp_mgen_api.h"
#include "util_global.h"

/*
** Enumerated Types
*/

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
* STRUCTURE: mp_mgen_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   MP_MGEN Module Instance Handle
*
* ELEMENTS:
*       base                  - See pmc_handle_t
*       cfg                   - See mp_mgen_cfg_t
*       var                   - See mp_mgen_var_t
*       
*******************************************************************************/
typedef struct mp_mgen_handle_struct_t {
    pmc_handle_t base;
    mp_mgen_cfg_t cfg;
    mp_mgen_var_t var;
} mp_mgen_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC mp_mgen_handle_t *mp_mgen_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void mp_mgen_ctxt_destroy(mp_mgen_handle_t *mp_mgen_handle);
PUBLIC void mp_mgen_handle_init(mp_mgen_handle_t *mp_mgen_handle);
PUBLIC PMC_ERROR mp_mgen_handle_restart_init(mp_mgen_handle_t *mp_mgen_handle,
                                             util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                             pmc_energy_state_t energy_state);
PUBLIC PMC_ERROR mp_mgen_ipt_print(mp_mgen_handle_t *mp_mgen_handle);
PUBLIC PMC_ERROR mp_mgen_ipt_cfg(mp_mgen_handle_t *mp_mgen_handle, 
                                 UINT32 ts_loc, UINT32 chnl_id, 
                                 UINT32 occurrence, UINT32 valid);
PUBLIC void mp_mgen_chnl_clean(mp_mgen_handle_t *mp_mgen_handle, 
                               UINT32 *chnl,
                               UINT32 num_chnl);
PUBLIC void mp_mgen_cycle_clean(mp_mgen_handle_t *mp_mgen_handle, 
                                UINT32 *cycle,
                                UINT32 num_cycle);
PUBLIC PMC_ERROR mp_mgen_data_mask_gen_cfg(mp_mgen_handle_t *mp_mgen_handle, 
                                           UINT32 chnl_id, UINT32 num_tribslot);
PUBLIC PMC_ERROR mp_mgen_last_txr_cfg(mp_mgen_handle_t *mp_mgen_handle, 
                                      UINT32 chnl_id, 
                                      util_global_odukp_type_t server_type, 
                                      util_global_odukp_type_t client_type,
                                      util_global_mapping_mode_t mapping_mode,
                                      UINT32 ts_type,
                                      UINT32 trib_slot,
                                      UINT32 m);
PUBLIC PMC_ERROR mp_mgen_amp_cfg(mp_mgen_handle_t *mp_mgen_handle, 
                                 UINT32 cal_entry, UINT32 tribslot_pos, 
                                 util_global_odukp_type_t server_type, 
                                 util_global_odukp_type_t client_type, 
                                 util_global_odu_line_payload_t tribslot_type); 

PUBLIC PMC_ERROR mp_mgen_amp_uncfg(mp_mgen_handle_t *mp_mgen_handle, 
                                   UINT32 cal_entry);                                    

PUBLIC PMC_ERROR mp_mgen_gmp_uncfg(mp_mgen_handle_t *mp_mgen_handle, 
                                   UINT32 chnl_id);

PUBLIC PMC_ERROR mp_mgen_ram_init(mp_mgen_handle_t *mp_mgen_handle);                             

PUBLIC PMC_ERROR mp_mgen_ipt_va_get(mp_mgen_handle_t *mp_mgen_handle, 
                                    UINT32 cal_entry,
                                    UINT32 *ipt_va);

#ifdef __cplusplus
}
#endif

#endif /* _MP_MGEN_H */

/*
** end of file
*/
