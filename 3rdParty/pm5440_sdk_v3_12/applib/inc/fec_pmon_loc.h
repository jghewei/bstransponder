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
#ifndef _FEC_PMON_LOC_H
#define _FEC_PMON_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "fec_pmon.h"
#include "fec_pmon_regs.h"
#include "fec_pmon_io.h"
#include "fec_pmon_io_inline.h"


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
* STRUCTURE: fec_pmon_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This is the config struct for initial values.
*
* ELEMENTS:
*    Currently none
*******************************************************************************/
typedef struct fec_pmon_cfg_t
{
    
} fec_pmon_cfg_t;

/*******************************************************************************
* STRUCTURE: fec_pmon_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    This struct is for dynamically changing context.
*
* ELEMENTS:
*    Currently none
*******************************************************************************/
typedef struct fec_pmon_var_t
{

} fec_pmon_var_t;


/*******************************************************************************
* STRUCTURE: fec_pmon_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Context structure for FEC PMON
*
* ELEMENTS:
*    base - base type for all block handles
*    cfg - fixed values from startup
*    var - dynamically changing values during run-time
*******************************************************************************/
typedef struct fec_pmon_handle_struct_t 
{
    pmc_handle_t base;
    
    fec_pmon_cfg_t cfg;
    fec_pmon_var_t var;
} fec_pmon_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/

#ifdef __cplusplus
}
#endif

#endif /* _FEC_PMON_LOC_H */

/* 
** end of file 
*/

