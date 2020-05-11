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
#ifndef _STS_SW_LOC_H
#define _STS_SW_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"
#include "sts_sw.h"
#include "sts_sw_io.h"
#include "sts_sw_io_inline.h"
#include "util_global.h"
#include "hostmsg.h"


/*
** Enumerated Types
*/

/*
** Constants
*/
#define NUM_STS_SW_CHNL        (96)
#define NUM_STS_SW_CAL_ENTRY   (96)
#define NUM_BYTE_IN_CYCLE      (48)

#define NUM_ODTU01_96_BYTES_LEN  (192)
#define NUM_ODTU01_48_BYTES_LEN  (96)
#define HALF_OF_CALENDAR_CYCLE   (48)


#define ODU0_PAYLOAD_BYTES      15232
#define ODU1_PAYLOAD_BYTES      30464
#define ODU2_PAYLOAD_BYTES      121856
#define ODU2_2G5_PAYLOAD_BYTES  60928
#define ODU3_PAYLOAD_BYTES      487424
#define ODU3_2G5_PAYLOAD_BYTES  243712
#define ODU3E1_PAYLOAD_BYTES    243712
#define ODU3E2_PAYLOAD_BYTES    487424
#define ODU4_PAYLOAD_BYTES      1216000

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



PUBLIC PMC_ERROR sts_sw_cfg_by_fw(sts_sw_handle_t *sts_sw_handle, UINT32 chnl_id);
PUBLIC PMC_ERROR sts_sw_calendar_null_cfg_by_fw(sts_sw_handle_t *sts_sw_handle, UINT32 cal_entry);
PUBLIC PMC_ERROR sts_sw_calendar_init_by_fw(sts_sw_handle_t *sts_sw_handle);
PUBLIC PMC_ERROR sts_sw_ho_calendar_null_cfg_by_fw(sts_sw_handle_t *sts_sw_handle, 
                                                   UINT8 chnl_id,
                                                   UINT8 num_tribslot,
                                                   UINT32 cal_entry[UTIL_GLOBAL_MAX_NUM_CAL_ENTRY],
                                                   UINT8 target_reg_mask);
PUBLIC PMC_ERROR sts_sw_cfg_all_by_fw(sts_sw_handle_t *sts_sw_handle, UINT32 chnl_id);

#ifdef __cplusplus
}
#endif

#endif /* _STS_SW_LOC_H */

/* 
** end of file 
*/

