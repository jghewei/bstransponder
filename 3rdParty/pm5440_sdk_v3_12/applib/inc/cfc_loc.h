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
#ifndef _CFC_LOC_H
#define _CFC_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/


#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"
#include "cfc.h"
#include "cfc_io.h"
#include "cfc_io_inline.h"


/*
** Enumerated Types
*/

/*
** Constants
*/
#ifndef PMC_SW_SIMULATION
#define CFC_NUM_USEC_ITERATIONS 250
#else
#define CFC_NUM_USEC_ITERATIONS 1
#endif

#define CFC_PHYSICAL_PAGE_NIL   0x7F
#define CFC_PHYSICAL_PAGE_EMPTY 0
#define CFC_BUS_WIDTH           384
#define CFC_IPT_DEPTH           96


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

                                          

#ifdef __cplusplus
}
#endif

#endif /* _CFC_LOC_H */

/* 
** end of file 
*/

