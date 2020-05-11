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
#ifndef _CPACT_OBUF_LOC_H
#define _CPACT_OBUF_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"
#include "cpact_obuf.h"
#include "cpact_obuf_io.h"
#include "cpact_obuf_io_inline.h"


/*
** Enumerated Types
*/

/*
** Constants
*/

/*
** Macro Definitions
*/

#define CEILING_POS(X) ((X-(int)(X)) > 0 ? (int)(X+1) : (int)(X))
#define CEILING_NEG(X) ((X-(int)(X)) < 0 ? (int)(X-1) : (int)(X))
#define CEILING(X) ( ((X) > 0) ? CEILING_POS(X) : CEILING_NEG(X) )

/*
** Structures and Unions
*/


/*******************************************************************************
* STRUCTURE: cpact_obuf_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Static CPACT_OBUF module instance data
*
* ELEMENTS:
*   cpact_obuf_instance_type_t       - block instance type
*
*       
*******************************************************************************/
typedef struct cpact_obuf_cfg_t{
    cpact_obuf_instance_type_t instance;
} cpact_obuf_cfg_t;



/*******************************************************************************
* STRUCTURE: cpact_obuf_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   CPACT_OBUF Module Instance Handle
*
* ELEMENTS:
*       base                  - See pmc_handle_t
*       cfg                   - See cpact_obuf_cfg_t
*       
*******************************************************************************/
typedef struct cpact_obuf_handle_struct_t {
    pmc_handle_t base;
    cpact_obuf_cfg_t cfg;
} cpact_obuf_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/


PUBLIC PMC_ERROR cpact_obuf_chnl_mapping_cfg(cpact_obuf_handle_t *cpact_obuf_handle, 
                                             UINT32 chnl,
                                             UINT32 mapping_mode);
                                     
PUBLIC PMC_ERROR cpact_obuf_end_of_multi_frame_cfg(cpact_obuf_handle_t *cpact_obuf_handle, 
                                                   UINT32 chnl_id,
                                                   util_global_odukp_type_t server_type,
                                                   util_global_odukp_type_t client_type,
                                                   UINT32 mapping_type, 
                                                   UINT32 trib_slot);
                                                  



#ifdef __cplusplus
}
#endif

#endif /* _CPACT_OBUF_LOC_H */

/* 
** end of file 
*/

