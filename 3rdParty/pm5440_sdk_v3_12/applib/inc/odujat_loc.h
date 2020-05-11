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
#include "odujat.h"
#include "odujat_io.h"
#include "odujat_io_inline.h"
#include "odujat_mtsb_map.h"


/*
** Enumerated Types
*/

/*
** Constants
*/

/* Number of supported channels */
#define ODUJAT_NUM_CHNL                 96
#define ODUJAT_TOTAL_RATE               134369280
#define ODUJAT_BUS_WIDTH                384

/* CFC configuration */
#define ODUJAT_CFC_MAX_FIFO_NUM         96

/* updated as latest configuration */
#define ODUJAT_CFC_PAGE_SIZE            8



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

#endif /* _ODUJAT_LOC_H */

/* 
** end of file 
*/

