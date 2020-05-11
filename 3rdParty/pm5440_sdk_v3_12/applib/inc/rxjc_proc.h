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

#ifndef _RXJC_PROC_H
#define _RXJC_PROC_H

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
#include "rxjc_proc_api.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: rxjc_proc_instance_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying block instance type.
*
* ELEMENTS:
*    RXJC_PROC_INSTANCE_RGMP   - RGMP instance
*    RXJC_PROC_INSTANCE_DEMUX  - DEMUX instance
*
*******************************************************************************/
typedef enum {
    RXJC_PROC_INSTANCE_RGMP    = 0,
    RXJC_PROC_INSTANCE_DEMUX   = 1,
} rxjc_proc_instance_type_t;

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
* STRUCTURE: expand_ibuf_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Static RXJC_PROC module instance data
*
* ELEMENTS:
*   rxjc_proc_instance_type_t       - block instance type
*
*       
*******************************************************************************/
typedef struct rxjc_proc_cfg_t{
    rxjc_proc_instance_type_t instance;  
} rxjc_proc_cfg_t;






/*******************************************************************************
* STRUCTURE: rxjc_proc_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   RXJC_PROC Module Instance Handle
*
* ELEMENTS:
*       base                  - See pmc_handle_t
*       cfg                   - See rxjc_proc_cfg_t
*       
*******************************************************************************/
typedef struct rxjc_proc_handle_struct_t {
    pmc_handle_t base;
    rxjc_proc_cfg_t cfg;
} rxjc_proc_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC rxjc_proc_handle_t *rxjc_proc_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void rxjc_proc_ctxt_destroy(rxjc_proc_handle_t *rxjc_proc_handle);
PUBLIC void rxjc_proc_handle_init(rxjc_proc_handle_t *rxjc_proc_handle,
                                  rxjc_proc_instance_type_t rxjc_proc_instance);
PUBLIC PMC_ERROR rxjc_proc_handle_restart_init(rxjc_proc_handle_t *rxjc_proc_handle,
                                               util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                               pmc_energy_state_t energy_state);

PUBLIC UINT32 rxjc_proc_ram_init(rxjc_proc_handle_t *rxjc_proc_handle);                                    

PUBLIC PMC_ERROR rxjc_proc_amp_cfg(rxjc_proc_handle_t *rxjc_proc_handle, 
                                   UINT32 chnl,
                                   util_global_odukp_type_t server_type,
                                   util_global_odukp_type_t client_type,
                                   UINT32 ts_type);
                                                                             
                                      
PUBLIC PMC_ERROR rxjc_proc_gmp_param_cfg(rxjc_proc_handle_t *rxjc_proc_handle, 
                                         UINT32 chnl, 
                                         util_global_odukp_type_t server_type,
                                         util_global_odukp_type_t client_type, 
                                         DOUBLE client_rate,
                                         UINT32 num_tribslot,
                                         BOOL use_custom_ppm,
                                         UINT32 ppm_offset);                                                                                                                                                 

PUBLIC PMC_ERROR rxjc_proc_bmp_cfg(rxjc_proc_handle_t *rxjc_proc_handle, 
                                   UINT32 chnl,
                                   util_global_odukp_type_t server_type,
                                   UINT32 ts_type);

PUBLIC PMC_ERROR rxjc_proc_cm_cfg(rxjc_proc_handle_t *rxjc_proc_handle, 
                                  UINT32 chnl,
                                  UINT32 cm_max,
                                  UINT32 cm_min);

PUBLIC PMC_ERROR rxjc_proc_lo_chnl_deprov(rxjc_proc_handle_t *rxjc_proc_handle, 
                                          UINT32 chnl);        

                                                                     
PUBLIC void rxjc_proc_param_cfg_get(rxjc_proc_handle_t *rxjc_proc_handle, 
                                    UINT32 chnl, 
                                    UINT32 ho_num_tribslot,
                                    UINT32 num_tribslot,
                                    util_global_mapping_mode_t mapping_mode,
                                    BOOL use_custom_ppm,
                                    UINT32 ppm_offset,
                                    util_global_odu_line_payload_t *ts_type,
                                    util_global_odukp_type_t *server_type,
                                    util_global_odukp_type_t *client_type);
                                                                

PUBLIC void rxjc_proc_cm_status_get(rxjc_proc_handle_t *rxjc_proc_handle, 
                                    UINT32 chnl, 
                                    UINT32 *value);
#ifdef __cplusplus
}
#endif

#endif /* _RXJC_PROC_H */

/*
** end of file
*/
