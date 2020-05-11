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
#ifndef _D8_RX_2X_LOC_H
#define _D8_RX_2X_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_sys.h"
#include "pmc_types.h"
#include "d8_rx_2x.h"
#include "util_global_api.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: d8_rx_2x_patt_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying D8_Rx_2X slice serdes pattern generators/monitors mode
*
* ELEMENTS:
*   D8_RX_2X_PATT_MODE_PRBS_5   - PRBS mode 5
*   D8_RX_2X_PATT_MODE_PRBS_7   - PRBS mode 7
*   D8_RX_2X_PATT_MODE_PRBS_11  - PRBS mode 11
*   D8_RX_2X_PATT_MODE_PRBS_15  - PRBS mode 15
*   D8_RX_2X_PATT_MODE_PRBS_20  - PRBS mode 20
*   D8_RX_2X_PATT_MODE_PRBS_23  - PRBS mode 23
*   D8_RX_2X_PATT_MODE_PRBS_31  - PRBS mode 31
*   D8_RX_2X_PATT_MODE_PRBS_9   - PRBS mode 9
*   D8_RX_2X_PATT_MODE_CJAT     - CJPAT
*   D8_RX_2X_PATT_MODE_USER_DEF - User defined Pattern Mode selected.
*
*******************************************************************************/
typedef enum d8_rx_2x_patt_mode_t {
    D8_RX_2X_PATT_MODE_PRBS_5   = 0,
    D8_RX_2X_PATT_MODE_PRBS_7   = 1,
    D8_RX_2X_PATT_MODE_PRBS_11  = 2,
    D8_RX_2X_PATT_MODE_PRBS_15  = 3,
    D8_RX_2X_PATT_MODE_PRBS_20  = 4,
    D8_RX_2X_PATT_MODE_PRBS_23  = 5,
    D8_RX_2X_PATT_MODE_PRBS_31  = 6,
    D8_RX_2X_PATT_MODE_PRBS_9   = 7,
    D8_RX_2X_PATT_MODE_CJAT     = 8,
    D8_RX_2X_PATT_MODE_USER_DEF = 15
} d8_rx_2x_patt_mode_t;
    

/*
** Constants
*/
/* D8_RX_2X_SERDES_A_IDX: Serdes A index */
#define D8_RX_2X_SERDES_A_IDX 0

/* D8_RX_2X_SERDES_B_IDX: Serdes B index */
#define D8_RX_2X_SERDES_B_IDX 1

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

#endif /* _D8_RX_2X_LOC_H */

/* 
** end of file 
*/

