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
*   DESCRIPTION:
*       This file provides the interface prototypes for the cpuif module. Note
*       that this module is primarily a set of register interfaces for the DIGI
*       CPU subsystem to inteface to firmware.  There are some additional CPU
*       interrupt aggregation functions.
*
*   NOTES:
*
*******************************************************************************/

#ifndef _CPUSS_H
#define _CPUSS_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
/* This module currently provides access to the register information
   for the DIGI internal CPU subsystem. It does not provide functional
   operations that encapsulate the operations and hence it exposes
   the /io files */
#include "pmc_err.h"
#include "pmc_handle.h"
#include "pmc_log.h"
#include "pmc_sys.h"
#include "cpuif_api.h"
#include "int_agr_io_inline.h"
#include "cpup34k_io_inline.h"

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



/*
** Global variables
*/

/*
** Function Prototypes
*/
PUBLIC void cpuif_int_cfg(pmc_handle_t *cpu_handle,
                          cpuif_int_t  int_type,
                          UINT32 int_agg_mask);
PUBLIC void cpuif_int_agg_enable(pmc_handle_t *cpu_handle,UINT32 int_no);
PUBLIC void cpuif_int_agg_disable(pmc_handle_t *cpu_handle,UINT32 int_no);
PUBLIC void cpuif_int_agg_clr(pmc_handle_t *cpu_handle,UINT32 int_no);
PUBLIC PMC_ERROR cpuif_int_agg_get(pmc_handle_t *cpu_handle,UINT32 reg_no);
PUBLIC UINT32 cpuif_int_agg_get(pmc_handle_t *cpu_handle,UINT32 reg_no);
PUBLIC void cpuif_int_device_aggregator_set(pmc_handle_t *cpu_handle,
                                            cpuif_int_t int_type,
                                            UINT32 int_enable_mask);
PUBLIC void cpuif_int_device_aggregator_clr(pmc_handle_t *cpu_handle,
                                            cpuif_int_t int_type,
                                            UINT32 int_disable_mask);
PUBLIC UINT32 cpuif_int_device_aggregator_get(pmc_handle_t *cpu_handle, 
                                              cpuif_int_t int_type);
#ifdef __cplusplus
}
#endif

#endif /* _CFC_H */

/*
** end of file
*/
