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
#ifndef _MP_MGEN_LOC_H
#define _MP_MGEN_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "mp_mgen.h"
#include "mp_mgen_io.h"
#include "mp_mgen_io_inline.h"
#include "util_global.h"

/*
** Enumerated Types
*/

/*
** Constants
*/
#define MP_MGEN_MAX_CHNL_NUM        96
#define MP_MGEN_CHNL_ID_NIL         0x7F

#define NUM_ODTU13_COL              (238)
#define NUM_ODTU13_TS_COL           (119)

/* ODTU/OPU Column size */
#define ODU1_ODU0_COL_SIZE          (1904)
#define ODU2_ODU1_COL_SIZE          (952)
#define ODU2_10G_SONET_COL_SIZE     (3792)
#define ODU3_ODU1_COL_SIZE          (238)
#define ODU3_ODU2_COL_SIZE          (952)
#define ODU3_40G_SONET_COL_SIZE     (3776)
#define ODU3E1_ODU2E_COL_SIZE       (952)

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

#endif /* _MP_MGEN_LOC_H */

/* 
** end of file 
*/

