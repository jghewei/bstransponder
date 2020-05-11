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
#ifndef _MPMA_LOC_H
#define _MPMA_LOC_H
/* GB: deterimine if we need extern C tags for *.h files in the src directory or only inc directory */
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "mpma.h"
#include "mpma_io.h"
#include "mpma_dsp_io.h"
#include "mpma_mtsb_map.h"
#include "cfc_io.h"


/*
** Enumerated Types
*/

/*
** Constants
*/
/* Number of supported channels */
#define MPMA_NUM_CHNL                 96
/* Number of supported channels */
#define MPMA_NUM_CHNL_REDUCED         12
/* Bus width on the ingress DCI interface */
#define MPMA_BUS_WIDTH_32_BYTE       0x0
#define MPMA_BUS_WIDTH_40_BYTE       0x1
#define MPMA_BUS_WIDTH_48_BYTE       0x2
/* Packet Maker mode - dither packet sizes */
#define MPMA_MKR_MODE                0x1
/* Packet Size Decision FIFO refill threshold */
#define MPMA_PSD_FIFO_REFILL_THRES     16
#define MPMA_PSD_FIFO_REFILL_THRES_CTL     8
/* Number of bytes per CLIENT_VALID. For 48 bytes, set CLIENT_WIDTH to 47. */
#define MPMA_CLIENT_WIDTH             47
/* Packet size variation is 1 byte. */
#define MPMA_PKT_SIZE_VAR              0
/* DSM type is MASH11 */
#define MPMA_DSM_TYPE                  0
/* Packet size range */
#define MPMA_MAPOTN_PKT_SIZE_RANGE      96
#define MPMA_CBRC_ENET_PKT_SIZE_RANGE   96
#define MPMA_DEFAULT_PKT_SIZE_RANGE     0

/* MPMA lowest supported bandwidth for 96 channel version.
 * Use 1.4Gbps for ODU0 since the DCI bus is overspeed. 
 */
#define MPMA_LOWEST_BANDWIDTH          1400E6 
/* MPMA lowest supported bandwidth for 12 channel version.
 * Use 11.2Gbps for ODU2 since the DCI bus is overspeed. 
 */
#define MPMA_LOWEST_BANDWIDTH_REDUCED   MPMA_LOWEST_BANDWIDTH * 8

/* Page size per FIFO channel */
#define MPMA_COREOTN_CFC_PAGE_SIZE       2
#define MPMA_COREOTN_CTL_CFC_PAGE_SIZE  16
#define MPMA_MAPOTN_CFC_PAGE_SIZE        2
#define MPMA_ENET_CFC_PAGE_SIZE         16
#define MPMA_CBRC_CFC_PAGE_SIZE         16



/* Util Scheduler Allocation Method */
#define MPMA_DEFAULT_CAL_ENTRIES_ALLOC_METHOD    1
#define MPMA_DB_CAL_ENTRIES_ALLOC_METHOD         4
#define MPMA_COREOTN_DB_CAL_ENTRIES_ALLOC_METHOD                7
#define MPMA_COREOTN_FROM_MAPOTN_DB_CAL_ENTRIES_ALLOC_METHOD    8
#define MPMA_MPMO_COREOTN_CTL_DB_CAL_ENTRIES_ALLOC_METHOD       9

/* SCBS3 Parameters */
/* 
 * Schedule total rate as 1.4Gbps * 96 ODU0 since the DCI bus is overspeed. 
 * Rate = 1.4 * 96 = 134.4 Gbps
 */
#define MPMA_SCBS3_TOTAL_RATE         (MPMA_LOWEST_BANDWIDTH/100000000 * MPMA_NUM_CHNL)
#define MPMA_SCBS3_TOTAL_RATE_UNITS   UTIL_SCHD_100M_BIT_SEC
#define MPMA_SCBS3_NUM_GRP               1
#define MPMA_SCBS3_MAX_ROWS             24
#define MPMA_SCBS3_READ_EN_USER_BITS   0x1
#define MPMA_SCBS3_USER_BITS_2_1_OFF     1
#define MPMA_SCBS3_BONUS_MODE        FALSE
#define MPMA_SCBS3_ROW_RESIZE        FALSE
#define MPMA_SCBS3_RATE_CONV_FACTOR    100

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

#endif /* _MPMA_LOC_H */

/* 
** end of file 
*/

