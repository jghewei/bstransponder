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
#ifndef _ODU_TFRM_LOC_H
#define _ODU_TFRM_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/

#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"
#include "odu_tfrm.h"
#include "odu_tfrm_io.h"
#include "odu_tfrm_io_inline.h"


/*
** Enumerated Types
*/

/*
** Constants
*/
#define ODU_TFRM_CHAN_NUM_1  (1)
#define ODU_TFRM_CHAN_NUM_12 (12)
#define ODU_TFRM_CHAN_NUM_96 (96)
#define TCM_POOL_SIZE_6      (6)
#define TCM_POOL_SIZE_72     (72)
#define TCM_POOL_SIZE_256    (256)
#define TCM_BUFFER_SIZE      (256)
#define NUM_TFRM_LAYERS      (7)
#define ODU_TFRM_NUM_USEC_ITERATIONS    (500)

/*
** Macro Definitions
*/

/*
** Structures and Unions
*/
/*******************************************************************************
* STRUCTURE: odu_tfrm_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to configure initial values
*
* ELEMENTS:
*   num_odu_tfrm_chans  - total number of channels supported in the ODU_TFRM
*                         12 or 96
*   tcm_pool_size       - size of TCM POOL ID table (72 or 256)
*******************************************************************************/
typedef struct
{
    /* To add static ohfsr config context */
    odu_tfrm_inst_t tfrm_id;
    UINT32 num_chans;
    UINT32 tcm_pool_size;

} odu_tfrm_cfg_t;

/*******************************************************************************
* STRUCTURE: odu_tfrm_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to define ODU_TFRM handle type
*
* ELEMENTS:
*   base - See pmc_handle_t
*   cfg  - See odu_tfrm_cfg_t
*   var  - See odu_tfrm_var_t
*
*******************************************************************************/
typedef struct odu_tfrm_handle_struct_t {
    pmc_handle_t base;

    odu_tfrm_cfg_t cfg;
    odu_tfrm_var_t var;

} odu_tfrm_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/


#ifdef __cplusplus
}
#endif

#endif /* _ODU_TFRM_LOC_H */

/*
** end of file
*/

