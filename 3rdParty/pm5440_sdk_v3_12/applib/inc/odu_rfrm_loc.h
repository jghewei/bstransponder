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
#ifndef _ODU_RFRM_LOC_H
#define _ODU_RFRM_LOC_H

#ifdef __cplusplus
extern "C" {
#endif

/*
** Include Files
*/


#include "pmc_sys.h"
#include "pmc_types.h"
#include "pmc_log.h"
#include "pmc_bitarray.h"
#include "odu_rfrm.h"
#include "odu_rfrm_io.h"
#include "odu_rfrm_io_inline.h"



/*
** Constants
*/
#define ODU_RFRM_CHAN_NUM_1   (1)
#define ODU_RFRM_CHAN_NUM_12 (12)
#define ODU_RFRM_CHAN_NUM_96 (96)
#define NUM_RFRM_LAYERS     (8)

/*
** Macro Definitions
*/
#define ODU_RFRM_NUM_CHNL_GET(odu_rfrm_handle)      (odu_rfrm_handle->cfg.num_chans)
#define ODU_RFRM_NUM_PID_CHNL_GET(odu_rfrm_handle)  (odu_rfrm_handle->cfg.tcm_pool_size)



/*
** Enumerated Types
*/



/*
** Structures and Unions
*/

/*******************************************************************************
* STRUCTURE: odu_rfrm_cfg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to configure initial values
*
* ELEMENTS:
*   odu_rfrm_inst       - See odu_rfrm_inst_t
*   num_chans           - total number of channels supported in the ODU_RFRM
*                         12 or 96
*   tcm_pool_size       - size of TCM POOL ID table (72 or 256)
*******************************************************************************/
typedef struct
{
    /* To add static ohfsr config context */
    odu_rfrm_inst_t odu_rfrm_inst;
    UINT32 num_chans;
    UINT32 tcm_pool_size;

} odu_rfrm_cfg_t;

/*******************************************************************************
* STRUCTURE: odu_rfrm_var_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to illustrate the dynamically changing context.
*
* ELEMENTS:
*   tcm_pool_ptr     - pointer to TCM context
*   pm_provisioned   - TRUE  : PM layer is provisioned for this channel
*                      FALSE :  PM layer is not provisioned for this channel
*   pm_mode          - See odu_rfrm_pm_tcm_mode_t
*   dci_pm_tcm_child - require for looking up which TCM connects to DCI
*   tcm_pool         - TCM POOL ID context table
*   num_pool_free    - Number of unassigned pool ids that are available in the
*                      TCM POOL ID context table
*   int_chnl_en      - Channel enabled interrupts (CH ID based)
*   int_pid_en       - Pool ID enabled interrupts (pool ID based)
*   int_comb_en      - Comb enabled intrrupts
*
*******************************************************************************/
typedef struct
{
    odu_rfrm_tcm_pool_t     *tcm_pool_ptr;
    BOOL8                   *pm_provisioned;
    odu_rfrm_pm_tcm_mode_t  *pm_mode;
    odu_rfrm_pm_tcm_id_t    *dci_pm_tcm_child;
    UINT8                   *maint_sig;
    UINT32                   num_pool_free;
    /* Enabled channels, pool ids, and defects */
    pmc_bitarray_t           int_chnl_en[PMC_BITARRAY_SIZE(ODU_RFRM_CHAN_NUM_96)];
    pmc_bitarray_t           int_pid_en[PMC_BITARRAY_SIZE(TCM_POOL_SIZE_96)];
    pmc_bitarray_t           int_comb_en[PMC_BITARRAY_SIZE(LAST_ODU_RFRM_INT_DEFECT)];
    BOOL8                    int_comb_valid;
    UINT32                   int_comb;

} odu_rfrm_var_t;

/*******************************************************************************
* STRUCTURE: odu_rfrm_handle_struct_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Structure to illustrate the handle struct for ODU_RFRM.
*
* ELEMENTS:
*   base    - See pmc_handle_t
*   cfg     - structure to store initial values.
*   var     - structure to store dynamically changing context information.
*******************************************************************************/
typedef struct odu_rfrm_handle_struct_t
{
    pmc_handle_t base;

    odu_rfrm_cfg_t cfg;
    odu_rfrm_var_t var;

} odu_rfrm_handle_struct_t;

/*
** Global variables
*/

/*
** Function Prototypes
*/

PUBLIC PMC_ERROR odu_rfrm_tcm_pool_id_ctxt_audit(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id, odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                 UINT32 pool_id);


#ifdef __cplusplus
}
#endif

#endif /* _ODU_RFRM_LOC_H */

/*
** end of file
*/

