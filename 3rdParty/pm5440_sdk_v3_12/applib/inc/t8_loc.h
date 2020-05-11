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
*
*   NOTES:
*
*******************************************************************************/
#ifndef _T8_LOC_H
#define _T8_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_sys.h"
#include "pmc_types.h"
#include "t8.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: t8_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying T8 serdes pattern generators/monitors mode
*
* ELEMENTS:
*   T8_PATT_MODE_PRBS_5   - PRBS mode 5
*   T8_PATT_MODE_PRBS_7   - PRBS mode 7
*   T8_PATT_MODE_PRBS_11  - PRBS mode 11
*   T8_PATT_MODE_PRBS_15  - PRBS mode 15
*   T8_PATT_MODE_PRBS_23  - PRBS mode 23
*   T8_PATT_MODE_PRBS_31  - PRBS mode 31
*   T8_PATT_MODE_CJAT     - CJPAT
*   T8_PATT_MODE_USER_DEF - User defined Pattern Mode selected.
*
*******************************************************************************/
typedef enum t8_patt_mode_t {
    T8_PATT_MODE_PRBS_5   = 0,
    T8_PATT_MODE_PRBS_7   = 1,
    T8_PATT_MODE_PRBS_11  = 2,
    T8_PATT_MODE_PRBS_15  = 3,
    T8_PATT_MODE_PRBS_23  = 5,
    T8_PATT_MODE_PRBS_31  = 6,
    T8_PATT_MODE_CJAT     = 8,
    T8_PATT_MODE_USER_DEF = 15
} t8_patt_mode_t;

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

#endif /* _T8_LOC_H */

/*
** end of file 
*/

