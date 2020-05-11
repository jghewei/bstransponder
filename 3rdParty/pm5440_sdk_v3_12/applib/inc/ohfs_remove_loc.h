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
#ifndef _OHFS_REMOVE_LOC_H
#define _OHFS_REMOVE_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"
#include "ohfs_remove.h"
#include "ohfs_remove_io.h"
#include "ohfs_remove_io_inline.h"


/*
** Enumerated Types
*/

/*
** Constants
*/
#define OHFS_REMOVE_MAX_CHNL_NUM        96
#define OHFS_REMOVE_CHNL_ID_NIL         0x7F

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

PUBLIC PMC_ERROR ohfs_remove_accept_cnt_cfg(ohfs_remove_handle_t *ohfs_remove_handle, 
                                            UINT32 chnl, 
                                            UINT32 pt_accept_cnt, 
                                            UINT32 msi_accept_cnt); 
 
                                                                                                                                                                                      

#ifdef __cplusplus
}
#endif

#endif /* _OHFS_REMOVE_LOC_H */

/* 
** end of file 
*/

