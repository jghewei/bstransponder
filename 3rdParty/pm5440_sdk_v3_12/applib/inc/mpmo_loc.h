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
#ifndef _MPMO_LOC_H
#define _MPMO_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "mpmo.h"
#include "mpmo_io.h"
#include "mpmo_dsp_io.h"
#include "mpmo_mtsb_map.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
/* Number of supported channels */
#define MPMO_NUM_CHNL                 96
#define MPMO_NUM_CHNL_REDUCED         12
#define MPMO_NUM_CHNL_SIFD_FIC        80
/* Bus width on the egress DCI interface */
#define MPMO_BUS_WIDTH_32_BYTE       0x0
#define MPMO_BUS_WIDTH_40_BYTE       0x1
#define MPMO_BUS_WIDTH_48_BYTE       0x2

/* MPMO lowest supported bandwidth.
 * Use 1.4Gbps for ODU0 since the DCI bus is overspeed. 
 */
#define MPMO_LOWEST_BANDWIDTH          1400E6
#define MPMO_LOWEST_BANDWIDTH_REDUCED   MPMO_LOWEST_BANDWIDTH * 8

/* Page size per FIFO channel */
#define MPMO_COREOTN_CFC_PAGE_SIZE       4
#define MPMO_COREOTN_CTL_CFC_PAGE_SIZE  16
#define MPMO_MAPOTN_CFC_PAGE_SIZE        4
#define MPMO_ENET_CFC_PAGE_SIZE         16
#define MPMO_CBRC_CFC_PAGE_SIZE         16
#define MPMO_CFC_FIFO_WORD_BITS        384


/* Util Scheduler Allocation Method */
#define MPMO_DEFAULT_CAL_ENTRIES_ALLOC_METHOD    1
#define MPMO_DB_CAL_ENTRIES_ALLOC_METHOD         5
#define MPMA_MPMO_COREOTN_CTL_DB_CAL_ENTRIES_ALLOC_METHOD       9
#define MPMO_COREOTN_MUX_FRAMER_DB_CAL_ENTRIES_ALLOC_METHOD     10
#define MPMO_COREOTN_TO_MAPOTN_DB_CAL_ENTRIES_ALLOC_METHOD      11


/* SCBS3 Parameters */
/* 
 * Schedule total rate as 1.4Gbps * 96 ODU0 since the DCI bus is overspeed. 
 * Rate = 1.4 * 96 = 134.4 Gbps
 */
#define MPMO_SCBS3_TOTAL_RATE         (MPMO_LOWEST_BANDWIDTH/100000000 * MPMO_NUM_CHNL)
#define MPMO_SCBS3_TOTAL_RATE_UNITS   UTIL_SCHD_100M_BIT_SEC
#define MPMO_SCBS3_NUM_GRP               1
#define MPMO_SCBS3_MAX_ROWS             24
#define MPMO_SCBS3_READ_EN_USER_BITS   0x1
#define MPMO_SCBS3_BONUS_MODE        FALSE
#define MPMO_SCBS3_ROW_RESIZE        FALSE
#define MPMO_SCBS3_RATE_CONV_FACTOR    100

/* MPMO latency fifo threshold */
#define MPMO_UPPER_LOWER_FENCE  0.1875
#define MPMO_MIDDLE_FENCE       0.5

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

#endif /* _MPMO_LOC_H */

/* 
** end of file 
*/

