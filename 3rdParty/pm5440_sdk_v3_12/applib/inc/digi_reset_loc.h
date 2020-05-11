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
*   DESCRIPTION : This file contains the definitions of register bit access
*   functions and computational operation functions for each subsystem.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _DIGI_RESET_LOC_H
#define _DIGI_RESET_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include File
*/


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
* STRUCTURE: digi_energy_state_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining DIGI energy state
*
* ELEMENTS: 
*   dcpb            - DCPB energy state
*   mcpb            - MCPB energy state
*   mapotn          - MAPOTN energy state
*   enet_line       - ENET LINE energy state
*   enet_sys        - ENET SYS energy state
*   lineotn         - LINEOTN energy state
*   sysotn          - SYSOTN energy state
*   cbrc            - CBRC energy state
*
*******************************************************************************/    
typedef struct digi_energy_state_struct_t { 
    pmc_energy_state_t dcpb;
    pmc_energy_state_t mcpb;
    pmc_energy_state_t mapotn;
    pmc_energy_state_t coreotn;
    pmc_energy_state_t enet_line;
    pmc_energy_state_t enet_sys;
    pmc_energy_state_t lineotn;
    pmc_energy_state_t sysotn;
    pmc_energy_state_t cbrc;
} digi_energy_state_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/


/*
** Computational Operation Functions
*/
PUBLIC void digi_dcpb_energy_state_set(digi_handle_t *digi_handle, 
                                       pmc_energy_state_request_t energy_state);

PUBLIC void digi_mcpb_energy_state_set(digi_handle_t *digi_handle, 
                                       pmc_energy_state_request_t energy_state);
                                       
PUBLIC void digi_lineotn_energy_state_set(digi_handle_t *digi_handle, 
                                          pmc_energy_state_request_t energy_state);
                                       
PUBLIC void digi_lifd_energy_state_set(digi_handle_t *digi_handle, 
                                       pmc_energy_state_request_t energy_state);
                                       
PUBLIC void digi_enet_sys_energy_state_set(digi_handle_t *digi_handle, 
                                           UINT32 link,
                                           enet_datapath_t datapath,
                                           pmc_energy_state_request_t energy_state);
                                       
PUBLIC void digi_enet_line_energy_state_set(digi_handle_t *digi_handle, 
                                            UINT32 link,
                                            enet_datapath_t datapath,
                                            pmc_energy_state_request_t energy_state);
                                       
PUBLIC void digi_mapotn_energy_state_set(digi_handle_t *digi_handle, 
                                         digi_mapping_mode_t mode,
                                         pmc_energy_state_request_t energy_state);
                                       
PUBLIC void digi_sifd_energy_state_set(digi_handle_t *digi_handle, 
                                       pmc_energy_state_request_t energy_state);

PUBLIC void digi_sysotn_energy_state_set(digi_handle_t *digi_handle, 
                                         pmc_energy_state_request_t energy_state);

PUBLIC void digi_coreotn_energy_state_set(digi_handle_t *digi_handle, 
                                          pmc_energy_state_request_t energy_state,
                                          coreotn_power_t coreotn_power);

PUBLIC void digi_cbrc_energy_state_set(digi_handle_t *digi_handle, 
                                       pmc_energy_state_request_t energy_state);

PUBLIC BOOL digi_lifd_system_required_test(digi_handle_t *digi_handle);

PUBLIC BOOL digi_sifd_system_required_test(digi_handle_t *digi_handle);

/*
** Register Access Operaion
*/

PUBLIC void digi_m1_energy_state_set(digi_handle_t *digi_handle, 
                                     pmc_energy_state_request_t energy_state);

PUBLIC void pgmrclk_energy_state_reg_toggle(digi_handle_t *digi_handle);

PUBLIC void digi_cpb_energy_state_reg_set(digi_handle_t *digi_handle, 
                                          pmc_energy_state_t energy_state);

PUBLIC void digi_mcpb_energy_state_reg_set(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t energy_state);

PUBLIC void digi_lineotn_energy_state_reg_set(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t energy_state);

PUBLIC void digi_lifd_energy_state_reg_set(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t energy_state);

PUBLIC void digi_enet_sys_energy_state_reg_set(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t energy_state);

PUBLIC void digi_enet_line_energy_state_reg_set(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t energy_state);

PUBLIC void digi_mapotn_energy_state_reg_set(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t energy_state);

PUBLIC void digi_sifd_energy_state_reg_set(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t energy_state);

PUBLIC void digi_sysotn_energy_state_reg_set(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t energy_state);

PUBLIC void digi_coreotn_energy_state_reg_set(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t energy_state);

PUBLIC void digi_cbrc_energy_state_reg_set(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t energy_state);

PUBLIC void digi_energy_state_reg_get(digi_handle_t *digi_handle, 
                                      digi_energy_state_t *energy_state);

/* 
** Deprecated Functions 
*/

#ifdef PMC_DEPRECATED

/* LCOV_EXCL_START */

PUBLIC void digi_cbrc_line_energy_state_set(digi_handle_t *digi_handle, 
                                            pmc_energy_state_request_t energy_state);
PUBLIC void digi_mcpb_energy_state_reg_get(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t *energy_state_ptr);
PUBLIC void digi_lineotn_energy_state_reg_get(digi_handle_t *digi_handle, 
                                              pmc_energy_state_t *energy_state_ptr);
PUBLIC void digi_sysotn_energy_state_reg_get(digi_handle_t *digi_handle, 
                                              pmc_energy_state_t *energy_state_ptr);
PUBLIC void digi_lifd_energy_state_reg_get(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t *energy_state_ptr);
PUBLIC void digi_cbrc_energy_state_reg_get(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t *energy_state_ptr);
PUBLIC void digi_enet_sys_energy_state_reg_get(digi_handle_t *digi_handle, 
                                               pmc_energy_state_t *energy_state_ptr);
PUBLIC void digi_enet_line_energy_state_reg_get(digi_handle_t *digi_handle, 
                                                pmc_energy_state_t *energy_state_ptr);
PUBLIC void digi_mapotn_energy_state_reg_get(digi_handle_t *digi_handle, 
                                             pmc_energy_state_t *energy_state_ptr);
PUBLIC void digi_sifd_energy_state_reg_get(digi_handle_t *digi_handle, 
                                           pmc_energy_state_t *energy_state_ptr);
PUBLIC void digi_coreotn_energy_state_reg_get(digi_handle_t *digi_handle, 
                                              pmc_energy_state_t *energy_state_ptr);
PUBLIC void digi_cbrc_line_energy_state_reg_set(digi_handle_t *digi_handle, 
                                                pmc_energy_state_t energy_state);

/* LCOV_EXCL_STOP */

#endif /* PMC_DEPRECATED */

#ifdef __cplusplus
}
#endif

#endif /* _DIGI_RESET_LOC_H */

/*
** end of file
*/
