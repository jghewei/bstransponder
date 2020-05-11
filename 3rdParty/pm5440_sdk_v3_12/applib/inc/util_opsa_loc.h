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
#ifndef _UTIL_OPSA_LOC_H
#define _UTIL_OPSA_LOC_H
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_sys.h"
#include "pmc_types.h"
#include "util_opsa.h"    



/*
** Enumerated Types
*/

/*
** Constants
*/


/*
** Macro Definitions
*/
/* Rounding function for positive floating point numbers */
#define round_pos(x) (UINT32) (x + 0.5)

/*
** Structures and Unions
*/

/*
** Global variables
*/

/*
** Function Prototypes
*/

#ifdef __cplusplus
}
#endif

#endif /* _UTIL_OPSA_LOC_H */

/* 
** end of file 
*/

