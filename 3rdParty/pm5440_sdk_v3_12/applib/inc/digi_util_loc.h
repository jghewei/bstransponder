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
#ifndef _DIGI_UTIL_LOC_H
#define _DIGI_UTIL_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*** Include Files*/
#include "pmc_sys.h"
#include "pmc_types.h"
#include "digi_resource_api.h"
#include "digi_util_api.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/

/*
** Global Variables
*/

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/*
** Private Functions
*/

/*
** Public Functions
*/

PUBLIC void digi_debug_dsi_alloc_config_print(digi_handle_t* digi_handle, const char *config_name);

PUBLIC void digi_debug_digi_handle_states_all_print(digi_handle_t* digi_handle);

PUBLIC void digi_debug_digi_resource_handle_print_all(digi_resource_handle_summary_t *digi_reource_handle);

PUBLIC void digi_debug_enet_line_handle_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot);
PUBLIC void digi_debug_enet_sys_handle_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot);
PUBLIC void digi_debug_ilkn1_handle_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot);
PUBLIC void digi_debug_ilkn2_handle_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot);
PUBLIC void digi_debug_mapper_handle_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot);
PUBLIC void digi_debug_ho_odu_struct_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot);
PUBLIC void digi_debug_mo_odu_struct_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot);
PUBLIC void digi_debug_lo_odu_struct_state_print(digi_handle_t* digi_handle, UINT32 index, BOOL print_head, BOOL print_foot);

PUBLIC void digi_debug_enet_line_handle_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_enet_sys_handle_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_ilkn1_handle_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_ilkn2_handle_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_mapper_handle_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_ho_odu_struct_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_mo_odu_struct_state_all_print(digi_handle_t* digi_handle);
PUBLIC void digi_debug_lo_odu_struct_state_all_print(digi_handle_t* digi_handle);

PUBLIC void digi_debug_mapotn_chnl_print(digi_handle_t* digi_handle);

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_UTIL_LOC_H */

/* 
** end of file 
*/

