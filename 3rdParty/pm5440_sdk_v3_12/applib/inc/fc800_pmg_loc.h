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
#ifndef _FC800_PMG_LOC_H
#define _FC800_PMG_LOC_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"
#include "fc800_pmg.h"    
#include "util_global.h"

#include "fc_cbr_io.h"



/*
** Enumerated Types
*/

/*
** Constants
*/
#define FC800_PMG_MAX_INDEX                                 12


/*
** Macro Definitions
*/
#define FC800_PMG_MODE_IS_FC800(mode)  (FC800_PMG_MODE_10G_FC800 == mode)

/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: fc800_pmg_cfg_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configuration structure for inital values.
*
* ELEMENTS:
*   None.
*
*******************************************************************************/
typedef struct {
    
} fc800_pmg_cfg_t;



/*******************************************************************************
* STRUCTURE: fc800_pmg_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining dynamically changing context.
*
* ELEMENTS:
*   mode  - Configured mode for the link (e.g. OC-192, CPRI, etc).
*
*******************************************************************************/
typedef struct {
    fc800_pmg_mode_t    mode[FC800_PMG_MAX_INDEX];
} fc800_pmg_var_t;



/*******************************************************************************
* STRUCTURE: fc800_pmg_handle_struct_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining FC800 PMG handle information.
*
* ELEMENTS: 
*   base                - Handle base information
*   cfg                 - Static config structure
*   var                 - Variable config structure
*
*******************************************************************************/    
struct fc800_pmg_handle_struct_t {
    pmc_handle_t base;
    
    fc800_pmg_cfg_t cfg;
    fc800_pmg_var_t var;
       
};

/*
** Global variables
*/

/*
** Function Prototypes
*/
/*PUBLIC UINT32 fc800_pmg_unit_test_make_test(fc800_pmg_handle_t *fc800_pmg_handle, UINT32 val);*/

#ifdef __cplusplus
}
#endif

#endif /* _FC800_PMG_LOC_H */

/* 
** end of file 
*/

