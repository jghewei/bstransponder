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
#ifndef _ODTU_MUX_LOC_H
#define _ODTU_MUX_LOC_H
/*GB: deterimine if we need extern C tags for *.h files in the src directory or only inc directory*/
#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"
#include "odtu_mux.h"
#include "mux_mtsb_map.h"
#include "odtu_mux_regs.h"
#include "odtu_mux_io.h"
#include "odtu_mux_io_inline.h"
#include "sts_sw.h"
#include "mp_mgen.h"
#include "txjc_proc.h"
#include "txjc_proc_io.h"
#include "expand_ibuf.h"
#include "expand_ibuf_io.h"
#include "ohfs_insert.h"
#include "tgmp.h"
#include "tgmp_mtsb_map.h"


/*
** Enumerated Types
*/

/*
** Constants
*/

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

PUBLIC void odtu_mux_lo_chnl_var_update(odtu_mux_handle_t *odtu_mux_handle, 
                                        UINT32 lo_chnl_id, UINT32 ho_chnl_id,
                                        odtu_mux_chnl_state_t chnl_state,
                                        util_global_odukp_type_t lo_oduk_type,
                                        util_global_odukp_type_t ho_oduk_type,
                                        UINT32 num_ts, 
                                        util_global_odu_line_payload_t ts_type,
                                        util_global_mapping_mode_t mapping_mode);

#ifdef __cplusplus
}
#endif

#endif /* _ODTU_MUX_LOC_H */

/* 
** end of file 
*/

