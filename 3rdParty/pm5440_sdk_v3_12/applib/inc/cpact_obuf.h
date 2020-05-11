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

#ifndef _CPACT_OBUF_H
#define _CPACT_OBUF_H

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
#include "cpact_obuf_api.h"

/*
** Enumerated Types
*/
/*******************************************************************************
* ENUM: cpact_obuf_instance_type_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Structure for enumerated type for specifying block instance type.
*
* ELEMENTS:
*    CPACT_OBUF_INSTANCE_RGMP   - RGMP instance
*    CPACT_OBUF_INSTANCE_DEMUX  - DEMUX instance
*
*******************************************************************************/
typedef enum {
    CPACT_OBUF_INSTANCE_RGMP    = 0,
    CPACT_OBUF_INSTANCE_DEMUX   = 1,
} cpact_obuf_instance_type_t;

/*
** Constants
*/

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/


/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC cpact_obuf_handle_t *cpact_obuf_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name);
PUBLIC void cpact_obuf_ctxt_destroy(cpact_obuf_handle_t *cpact_obuf_handle);
PUBLIC void cpact_obuf_handle_init(cpact_obuf_handle_t *cpact_obuf_handle, cpact_obuf_instance_type_t instance);
PUBLIC PMC_ERROR cpact_obuf_handle_restart_init(cpact_obuf_handle_t *cpact_obuf_handle,
                                                util_global_restart_init_cfg_t *restart_init_cfg_ptr,
                                                pmc_energy_state_t energy_state);

PUBLIC UINT32 cpact_obuf_ram_init(cpact_obuf_handle_t *cpact_obuf_handle);


PUBLIC PMC_ERROR cpact_obuf_chnl_cfg(cpact_obuf_handle_t *cpact_obuf_handle, 
                                     UINT32 chnl_id, 
                                     util_global_odukp_type_t server_type,
                                     UINT32 client_type,
                                     util_global_mapping_mode_t mapping_mode,
                                     UINT32 num_tribslot, 
                                     util_global_odu_line_payload_t ts_type, 
                                     BOOL8 offline_update); 

PUBLIC PMC_ERROR cpact_obuf_lo_chnl_deprov(cpact_obuf_handle_t *cpact_obuf_handle, 
                                           UINT32 chnl_id);
                                                                                                                         



#ifdef __cplusplus
}
#endif

#endif /* _CPACT_OBUF_H */

/*
** end of file
*/
