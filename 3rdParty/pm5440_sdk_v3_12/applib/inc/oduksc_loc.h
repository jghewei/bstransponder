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
#ifndef _ODUKSC_LOC_H
#define _ODUKSC_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/


#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"
#include "oduksc.h"
#include "oduksc_io.h"
#include "oduksc_io_inline.h"
#include "oduksc_mtsb_map.h"
#include "util_opsa.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
#define ODUKSC_NUM_USEC_ITERATIONS      250
#define ODUKSC_CGEN_MAX_VALUE           67108863   /* 2^26-1 */

/* Number of supported channels */
#define ODUKSC_NUM_CHNL                 96
#define ODUKSC_TOTAL_RATE               134369280
#define ODUKSC_BUS_WIDTH                384
#define ODUKSC_CLOCK_SPEED              349920000  /* 349.92 MHz */

/* Ramp Interval PulseGen Settings */
#define ODUKSC_RAMP_INTERVAL_TM         2
#define ODUKSC_RAMP_INTERVAL_TN         10935

/* Ramp Credit Generator (ODUflex GFP) */
#define ODUKSC_RAMP_RATE                512000000  /* bits / s^2 */
#define ODUKSC_RAMP_RATE_PER_INTERVAL   8000  /* kbps */
#define ODUKSC_RAMP_N2_MAX              67108863   /* 2^26-1 */
#define ODUKSC_RAMP_NB_INTERVAL_250US   16

/* Ramp Credit Generator Denominator (CBR) */
#define ODUKSC_RAMP_CGEN_N2             55987200
#define ODUKSC_RAMP_STEP_INCRS          320

/* SCBS3 Parameters */
#define ODUKSC_SCBS3_NUM_GRP            1
#define ODUKSC_SCBS3_MAX_ROWS           24
#define ODUKSC_SCBS3_TOTAL_RATE         134369280 
#define ODUKSC_SCBS3_TOTAL_RATE_UNITS   UTIL_SCHD_1K_BIT_SEC
#define ODUKSC_SCBS3_TOC_SYNC_EN        0
#define ODUKSC_SCBS3_TOC_SYNC           0x0
#define ODUKSC_SCBS3_PAGE_SWAP_BIT      0x0
#define ODUKSC_SCBS3_BONUS_MODE         FALSE
#define ODUKSC_SCBS3_ROW_RESIZE         FALSE

/* Define TARGET_NUMERATOR value for ODUFlex clients */
#define ODUFLEX_TARGET_NUMERATOR { \
   /* Index is number of ODU0's in ODUFlex container */  \
   /* 1,      2,        3,        4,        5,        6,        7,        8,        9,        10, */  \
   {15206400, 3785694 , 5679497 , 7572662 , 9465828 , 11358993, 13252159, 15145325, 16966598, 18851775}, /* oduflex 1 - 10  */ \
   {20736953, 22622130, 24507308, 26392485, 28277663, 30162841, 32048018, 33933196, 35818373, 37703551}, /* oduflex 11 - 20 */ \
   {39588728, 41473906, 43359083, 45244261, 47129438, 49014616, 50899793, 52784971, 54670148, 56555326}, /* oduflex 21 - 30 */ \
   {58440504, 30162841, 29982193, 30890744, 31799296, 32707847, 33616398, 34524950, 35433501, 36342052}, /* oduflex 31 - 40 */ \
   {37250604, 38159155, 39067706, 39976257, 40884809, 41793361, 42701911, 43610463, 44519014, 45427565}, /* oduflex 41 - 50 */ \
   {46336117, 47244668, 48153219, 49061771, 49970322, 50878873, 51787424, 52695976, 53604527, 54513078}, /* oduflex 51 - 60 */ \
   {55421630, 56330181, 57238732, 29073642, 29527917, 29982193, 30436469, 30890744, 31345020, 31799296}, /* oduflex 61 - 70 */ \
   {32253571, 32707847, 33162123, 33616398, 34070674, 34524950, 34979225, 35433501, 35887777, 36342052}, /* oduflex 71 - 80 */ \
} /* ODUFlex TARGET_NUMERATOR sequence */ 

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

PUBLIC PMC_ERROR oduksc_cgen_chnl_cfg(oduksc_handle_t *oduksc_handle,
                                      UINT32 chnl,
                                      util_global_odukp_type_t server_type,
                                      util_global_odukp_type_t client_type,
                                      UINT32 client_rate,
                                      UINT32 oduflex_gfp_rate);

PUBLIC PMC_ERROR oduksc_oduflex_gfp_m_n_get(oduksc_handle_t *oduksc_handle,
                                            UINT32 tribslots,
                                            UINT32 *cgen_m,
                                            UINT32 *cgen_n);



#ifdef __cplusplus
}
#endif

#endif /* _ODUKSC_LOC_H */

/* 
** end of file 
*/

