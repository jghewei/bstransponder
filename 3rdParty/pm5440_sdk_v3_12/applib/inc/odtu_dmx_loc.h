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
#ifndef _ODTU_DMX_LOC_H
#define _ODTU_DMX_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"
#include "odtu_dmx.h"
#include "odtu_dmx_mtsb_map.h"
#include "odtu_dmx_io.h"
#include "odtu_dmx_io_inline.h"
#include "sts_sw.h"
#include "mp_mgen.h"
#include "rxjc_proc.h"
#include "cpact_obuf.h"
#include "ohfs_remove.h"
#include "rgmp.h"
#include "rgmp_mtsb_map.h"
#include "rxjc_proc_io.h"
#include "cpact_obuf_io.h"


/*
** Enumerated Types
*/

/*
** Constants
*/
#define ODTU_DMX_MAX_CHNL_NUM   (96)

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
/*PRIVATE PMC_ERROR odtu_dmx_lo_id_disable(odtu_dmx_handle_t *odtu_dmx_handle,
                                        UINT32 chnl_id, UINT32 ho_chnl_id);*/

#ifdef __cplusplus
}
#endif

#endif /* _ODTU_DMX_LOC_H */

/* 
** end of file 
*/

