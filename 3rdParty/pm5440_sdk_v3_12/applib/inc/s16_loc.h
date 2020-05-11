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
#ifndef _S16_LOC_H
#define _S16_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/
#include "pmc_sys.h"
#include "pmc_types.h"
#include "s16.h"
#include "s16_channel_pcbi_io.h"
#include "s16_titan_br2_mdsp_io.h"
#include "s16_adapt_dsp_16_io.h"
#include "s16_mtsb_ctrl_io.h"
#include "s16_csu_pcbi_io.h"
#include "s16_mtsb_map.h"
#include "util_global.h"

/*
** Enumerated Types
*/

/*******************************************************************************
* ENUM: s16_patt_mode_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumeration specifying the pattern generators/monitors mode
*
* ELEMENTS:
*   S16_PATT_MODE_PRBS_5   - PRBS mode 5
*   S16_PATT_MODE_PRBS_7   - PRBS mode 7
*   S16_PATT_MODE_PRBS_11  - PRBS mode 11
*   S16_PATT_MODE_PRBS_15  - PRBS mode 15
*   S16_PATT_MODE_PRBS_20  - PRBS mode 20
*   S16_PATT_MODE_PRBS_23  - PRBS mode 23
*   S16_PATT_MODE_PRBS_31  - PRBS mode 31
*   S16_PATT_MODE_PRBS_9   - PRBS mode 9
*   S16_PATT_MODE_CJAT     - CJPAT
*   S16_PATT_MODE_USER_DEF - User defined Pattern Mode selected.
*
*******************************************************************************/
typedef enum
{
    S16_PATT_MODE_PRBS_5   = 0,
    S16_PATT_MODE_PRBS_7   = 1,
    S16_PATT_MODE_PRBS_11  = 2,
    S16_PATT_MODE_PRBS_15  = 3,
    S16_PATT_MODE_PRBS_20  = 4,
    S16_PATT_MODE_PRBS_23  = 5,
    S16_PATT_MODE_PRBS_31  = 6,
    S16_PATT_MODE_PRBS_9   = 7,
    S16_PATT_MODE_CJAT     = 8,
    S16_PATT_MODE_USER_DEF = 15
} s16_patt_mode_t;

/*
** Constants
*/

#define S16_LINE_NUM_INSTANCE              12
#define S16_SYSTEM_NUM_INSTANCE            16
#define S16_INSTANCE_ADDR_SPACE            0x1000
#define S16_SLAVE_PDM_ACCUM_CYCLES         32
#define S16_SLAVE_PDM_GAIN                 5
#define S16_PDM_UPDATE_RATE                9
#define S16_PMM_DSM_TYPE                   1
#define S16_PMM_DSM_DENOM                  1650
#define S16_PMM_PACKET_PERIOD              100
#define S16_TX_SOURCE_FUNCTIONAL_MODE      0x0
#define S16_TX_SOURCE_PATTERN_GENERATOR    0x2
#define S16_TX_SOURCE_INTERNAL_LOOPBACK    0x3
#define S16_LANE_ALIGNER_OUT_OF_RESET_VAL 0x666

/*
** Macro Definitions
*/
#define round(x)   ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))

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

#endif /* _S16_LOC_H */

/* 
** end of file 
*/

