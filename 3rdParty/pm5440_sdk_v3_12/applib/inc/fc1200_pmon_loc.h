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
#ifndef _FC1200_PMON_LOC_H
#define _FC1200_PMON_LOC_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"
#include "fc1200_pmon.h"    
#include "fc1200_pmon_io.h"
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
* STRUCTURE: fc1200_pmon_cfg_t 
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
    
} fc1200_pmon_cfg_t;



/*******************************************************************************
* STRUCTURE: fc1200_pmon_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining dynamically changing context.
*
* ELEMENTS:
*   None.
*
*******************************************************************************/
typedef struct {
    
} fc1200_pmon_var_t;



/*******************************************************************************
* STRUCTURE: fc1200_pmon_handle_struct_t 
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure defining FC1200 PMON handle information.
*
* ELEMENTS: 
*   base                - Handle base information
*   cfg                 - Static config structure
*   var                 - Variable config structure
*
*******************************************************************************/    
struct fc1200_pmon_handle_struct_t {
    pmc_handle_t base;
    
    fc1200_pmon_cfg_t cfg;
    fc1200_pmon_var_t var;
       
};

/*
** Global variables
*/

/*
** Function Prototypes
*/
/*PUBLIC UINT32 fc1200_pmon_unit_test_make_test(fc1200_pmon_handle_t *fc1200_pmon_handle, UINT32 val);*/

#ifdef __cplusplus
}
#endif

#endif /* _FC1200_PMON_LOC_H */

/* 
** end of file 
*/

