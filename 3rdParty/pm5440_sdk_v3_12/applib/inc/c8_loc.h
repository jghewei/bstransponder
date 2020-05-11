/*******************************************************************************
*   COPYRIGHT (C) 2012 PMC-SIERRA, INC. ALL RIGHTS RESERVED.
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
*     C8 serdes local include file.
*   NOTES:
*
*******************************************************************************/
#ifndef _C8_LOC_H
#define _C8_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_sys.h"
#include "pmc_types.h"
#include "c8.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: c8_patt_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying C8 block pattern generator/monitor mode
*
* ELEMENTS:
*   C8_PATT_MODE_PRBS_5   - PRBS mode 5
*   C8_PATT_MODE_PRBS_7   - PRBS mode 7
*   C8_PATT_MODE_PRBS_11  - PRBS mode 11
*   C8_PATT_MODE_PRBS_15  - PRBS mode 15
*   C8_PATT_MODE_PRBS_23  - PRBS mode 23
*   C8_PATT_MODE_PRBS_31  - PRBS mode 31
*   C8_PATT_MODE_CJAT     - CJPAT
*   C8_PATT_MODE_USER_DEF - User defined Pattern Mode selected.
*******************************************************************************/
typedef enum c8_patt_mode_t 
{
    C8_PATT_MODE_PRBS_5   = 0,
    C8_PATT_MODE_PRBS_7   = 1,
    C8_PATT_MODE_PRBS_11  = 2,
    C8_PATT_MODE_PRBS_15  = 3,
    C8_PATT_MODE_PRBS_23  = 5,
    C8_PATT_MODE_PRBS_31  = 6,
    C8_PATT_MODE_CJAT     = 8
} c8_patt_mode_t;

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
#ifdef __cplusplus
}
#endif

#endif /* _C8_LOC_H */

/* 
** end of file 
*/

