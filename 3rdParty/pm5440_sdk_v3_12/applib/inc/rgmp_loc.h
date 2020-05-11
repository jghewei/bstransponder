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
#ifndef _RGMP_LOC_H
#define _RGMP_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "rgmp.h"
#include "rgmp_mtsb_map.h"
#include "rgmp_regs.h"
#include "rgmp_io.h"
#include "rgmp_io_inline.h"
#include "ohfs_remove.h"
#include "rxjc_proc.h"
#include "mp_mgen.h"
#include "cpact_obuf.h"



/*
** Enumerated Types
*/

/* Channel State Definition */
/*typedef enum {
    RGMP_CHNL_START        = 0,
    RGMP_CHNL_EQUIPPED     = 1,
    RGMP_CHNL_OPERATIONAL  = 2
} rgmp_chnl_state_t;
*/
/*
** Constants
*/
#define RGMP_PHYSICAL_PAGE_NIL   0x7F
#define RGMP_PHYSICAL_PAGE_EMPTY 0
#define RGMP_IPT_DEPTH           96

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

#endif /* _RGMP_LOC_H */

/* 
** end of file 
*/

