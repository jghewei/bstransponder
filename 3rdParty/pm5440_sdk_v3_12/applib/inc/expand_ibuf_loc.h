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
#ifndef _EXPAND_IBUF_LOC_H
#define _EXPAND_IBUF_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"
#include "expand_ibuf.h"
#include "exp_mtsb_map.h"
#include "expand_ibuf_io.h"
#include "expand_ibuf_io_inline.h"


/*
** Enumerated Types
*/

/*
** Constants
*/
#define EXPAND_IBUF_CFC_MAX_FIFO_NUM   96
/* the way CFC configures IPT_PAGE_MASK requires the PAGE_SIZE to be +1 of 
** the desired number
*/
#define EXPAND_IBUF_CFC_PAGE_SIZE      4 

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

#endif /* _EXPAND_IBUF_LOC_H */

/* 
** end of file 
*/

