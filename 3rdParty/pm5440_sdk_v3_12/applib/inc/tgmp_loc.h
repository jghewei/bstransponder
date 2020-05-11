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
#ifndef _TGMP_LOC_H
#define _TGMP_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "tgmp.h"
#include "tgmp_mtsb_map.h"
#include "tgmp_regs.h"
#include "tgmp_io.h"
#include "tgmp_io_inline.h"
#include "cfc.h"
#include "txjc_proc.h"
#include "ohfs_insert.h"
#include "mp_mgen.h"
#include "expand_ibuf.h"


/*
** Enumerated Types
*/



/*
** Constants
*/
#define TGMP_PHYSICAL_PAGE_NIL   0x7F
#define TGMP_PHYSICAL_PAGE_EMPTY 0
#define TGMP_IPT_DEPTH           96



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

#endif /* _TGMP_LOC_H */

/* 
** end of file 
*/

