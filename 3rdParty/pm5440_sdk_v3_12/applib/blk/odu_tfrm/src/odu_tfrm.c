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
*   DESCRIPTION : This file contains C functions for transmit direction ODU
*   overhead processing methods.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/
#include "odu_tfrm_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*TFRM must support up to a maximum of ~103 seconds in delay measurement
  (ODU0 frame period - 98.354*(1^20) = 103.12 seconds */
#define OTU_TFRM_POLL_ITERATIONS        105000
#define OTU_TFRM_USEC_DELAY_TICKS         1000

/*
** Local Macro Definitions
*/

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/*
** Log Messaging
*/
/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/*
 * The following creates the LOG/ERR strings from above by pulling out the text
 * portion of the LOG_ERR_TABLE above
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char ODU_TFRM_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    ODU_TFRM_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char ODU_TFRM_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/
PRIVATE void odu_tfrm_internal_chnl_clean(odu_tfrm_handle_t *odu_tfrm_handle,
                                          UINT32 chnl);
PRIVATE void odu_tfrm_var_default_init(odu_tfrm_handle_t *hndl);
PRIVATE PMC_ERROR odu_tfrm_tcm_pool_update(odu_tfrm_handle_t *odu_tfrm_handle,
                                           UINT32 chnl_id,
                                           odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                           UINT16 pool_id, 
                                           BOOL8 assign_state,
                                           odu_tfrm_pm_tcm_mode_t tcm_mode);
PRIVATE UINT32 odu_tfrm_tcm_pool_id_check(odu_tfrm_handle_t *odu_tfrm_handle,
                                          UINT32 pool_id);
PRIVATE PMC_ERROR odu_tfrm_update_pm_tcm_ctxt(odu_tfrm_handle_t *odu_tfrm_handle,
                                             UINT32 chnl_id,
                                             odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                             odu_tfrm_pm_tcm_id_t parent_id,
                                             odu_tfrm_pm_tcm_mode_t mode,
                                             BOOL8 prov_action,
                                             UINT32 tcm_pool_id);                                                             

PRIVATE PMC_ERROR odu_tfrm_pm_tcmi_mode_set(odu_tfrm_handle_t *odu_tfrm_handle,
                                            UINT32 chnl_id,
                                            odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                            BOOL transparent_mode);

PRIVATE PMC_ERROR odu_tfrm_int_validate(odu_tfrm_handle_t    *odu_trfm_handle,
                                        UINT32                chnl_id);

/*
** Public Functions
*/

/*******************************************************************************
* odu_tfrm_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an ODU_TFRM block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the RGFPF subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   odu_tfrm_handle_t       - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC odu_tfrm_handle_t *odu_tfrm_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    odu_tfrm_handle_t *odu_tfrm_handle;

    PMC_ENTRY();
    
    odu_tfrm_handle = (odu_tfrm_handle_t*)PMC_CTXT_CALLOC(sizeof(odu_tfrm_handle_t), parent);

    pmc_handle_init(parent, odu_tfrm_handle, sys_handle, PMC_MID_DIGI_ODU_TFRM, tsb_name, base_address);
    
    /* Register log strings. */
    pmc_log_block_strings_register(ODU_TFRM_LOG_ERR_STRINGS[0], ODU_TFRM_LOG_ERR_TABLE_BASE, ODU_TFRM_LOG_ERR_COUNT);

    PMC_RETURN(odu_tfrm_handle);
} /* odu_tfrm_ctxt_create */


/*******************************************************************************
* odu_tfrm_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a ODU_TFRM block instance.
*
*
* INPUTS:
*   *odu_tfrm_handle        - pointer to ODU_TFRM handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odu_tfrm_ctxt_destroy(odu_tfrm_handle_t *odu_tfrm_handle)
{
    
    PMC_ENTRY();
    
    PMC_CTXT_FREE(&(odu_tfrm_handle->var.tcm_pool_ptr), odu_tfrm_handle);
    PMC_CTXT_FREE(&(odu_tfrm_handle->var.optional_oh_ctxt), odu_tfrm_handle);
    PMC_CTXT_FREE(&(odu_tfrm_handle->var.dci_pm_tcm_child), odu_tfrm_handle);
    PMC_CTXT_FREE(&(odu_tfrm_handle->var.pm_ctxt_ptr), odu_tfrm_handle);
    PMC_CTXT_FREE(&(odu_tfrm_handle->var.odukp_mux_maint_sig), odu_tfrm_handle);
    PMC_CTXT_FREE(&(odu_tfrm_handle->var.odukp_pck_maint_sig), odu_tfrm_handle);

    PMC_LOG_TRACE("odu_tfrm_handle->cfg.num_chans = %u\n", odu_tfrm_handle->cfg.num_chans);

    PMC_CTXT_FREE(&odu_tfrm_handle, odu_tfrm_handle);
    
    PMC_RETURN();
} /* odu_tfrm_ctxt_destroy */

 

/*******************************************************************************
* odu_tfrm_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an ODU_TFRM block handle instance. Based on the TFRM instance,
*   the maximum number of channels and the maximum pool size supported in the
*   TFRM instance will be determined and cfg_t data updated with these data.
*
* INPUTS:
*   *odu_tfrm_handle        - pointer to ODU_TFRM handle instance
*   tfrm_instance           - Enum. See odu_tfrm_inst_t.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odu_tfrm_handle_init(odu_tfrm_handle_t *odu_tfrm_handle,
                                 odu_tfrm_inst_t tfrm_instance)
{
    /* variable declaration */
    UINT32 size;
    UINT32 num_chnl = 0;
    UINT32 pool_size = 0;
    UINT32 chnl_size;
    UINT32 chnl_ptr_size;
    UINT32 sub_layer_size;
    
    PMC_ENTRY();
    
    /* Check for valid channel number */
    if (tfrm_instance > ODU_TFRM_STGN)
    {
        PMC_LOG_TRACE("ODU_TFRM  = %u is not supported ...\n", tfrm_instance);
        PMC_DEBUG(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS, tfrm_instance, 0);
        PMC_RETURN();
    }

    /* Update static configurations */
    switch (tfrm_instance)
    {
        case ODU_TFRM_STG1:
            /* number of channels */
            num_chnl = ODU_TFRM_CHAN_NUM_12;
            /* maximum pool size */
            pool_size = TCM_POOL_SIZE_72;
            break;
        case ODU_TFRM_STGN:
            /* number of channels */
            num_chnl = ODU_TFRM_CHAN_NUM_1;
            /* maximum pool size */
            pool_size = TCM_POOL_SIZE_6;
            break;
        case ODU_TFRM_STG2:
        case ODU_TFRM_STG3A:
        case ODU_TFRM_STG3B:
        case ODU_TFRM_STG4:
            /* number of channels */
            num_chnl = ODU_TFRM_CHAN_NUM_96;
            /* maximum pool size */
            pool_size = TCM_POOL_SIZE_256;
            break;
    }

    odu_tfrm_handle->cfg.num_chans      = num_chnl;
    odu_tfrm_handle->cfg.tcm_pool_size  = pool_size;
    odu_tfrm_handle->cfg.tfrm_id        = tfrm_instance;

    size = sizeof(odu_tfrm_tcm_pool_t) * odu_tfrm_handle->cfg.tcm_pool_size;
    chnl_size = sizeof(UINT8) * odu_tfrm_handle->cfg.num_chans;
    
    chnl_ptr_size = sizeof(UINT8 *) * odu_tfrm_handle->cfg.num_chans;
    sub_layer_size = sizeof(UINT8) * ODU_SOURCE_MAX_PM_TCM_LAYERS;
    
    PMC_LOG_TRACE("%s, %s, %d, odu_tfrm_handle->cfg.tcm_pool_size = %u...\n", __FILE__, __FUNCTION__, __LINE__, odu_tfrm_handle->cfg.tcm_pool_size);

    odu_tfrm_handle->var.tcm_pool_ptr = (odu_tfrm_tcm_pool_t*)PMC_CTXT_CALLOC(size, odu_tfrm_handle);
    PMC_CTXT_REGISTER_PTR(&odu_tfrm_handle->var.tcm_pool_ptr, odu_tfrm_handle);
    odu_tfrm_handle->var.optional_oh_ctxt = (UINT8 *)PMC_CTXT_CALLOC(chnl_size, odu_tfrm_handle);
    PMC_CTXT_REGISTER_PTR(&odu_tfrm_handle->var.optional_oh_ctxt, odu_tfrm_handle);

    odu_tfrm_handle->var.dci_pm_tcm_child = (odu_tfrm_pm_tcm_id_t*)PMC_CTXT_CALLOC(sizeof(odu_tfrm_pm_tcm_id_t)*odu_tfrm_handle->cfg.num_chans, odu_tfrm_handle);
    PMC_CTXT_REGISTER_PTR(&odu_tfrm_handle->var.dci_pm_tcm_child, odu_tfrm_handle);
       
    odu_tfrm_handle->var.pm_ctxt_ptr = (odu_tfrm_pm_ctxt_t*)PMC_CTXT_CALLOC(sizeof(odu_tfrm_pm_ctxt_t)*odu_tfrm_handle->cfg.num_chans, odu_tfrm_handle);
    PMC_CTXT_REGISTER_PTR(&odu_tfrm_handle->var.pm_ctxt_ptr, odu_tfrm_handle);
       
    odu_tfrm_handle->var.odukp_mux_maint_sig = (UINT8*)PMC_CTXT_CALLOC(sizeof(UINT32)*odu_tfrm_handle->cfg.num_chans, odu_tfrm_handle);
    PMC_CTXT_REGISTER_PTR(&odu_tfrm_handle->var.odukp_mux_maint_sig, odu_tfrm_handle);

    odu_tfrm_handle->var.odukp_pck_maint_sig = (UINT8*)PMC_CTXT_CALLOC(sizeof(UINT32)*odu_tfrm_handle->cfg.num_chans, odu_tfrm_handle);
    PMC_CTXT_REGISTER_PTR(&odu_tfrm_handle->var.odukp_pck_maint_sig, odu_tfrm_handle);
    
    odu_tfrm_var_default_init(odu_tfrm_handle);
    
    PMC_RETURN();
} /* odu_tfrm_handle_init */

/*******************************************************************************
* odu_tfrm_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on odu_tfrm.
*
*
* INPUTS:
*   *odu_tfrm_handle      - pointer to ODU_TFRM handle instance
*   *restart_init_cfg_ptr - pointer to restart init config
*   top_energy_state_reg  - The expected energy state 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS : 
*     WARM : register/context are coherent
*     CRASH: everything is coherent (a cleanup may has been executed)
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_handle_restart_init(odu_tfrm_handle_t *odu_tfrm_handle,
                                              util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                              pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(ODU_TFRM_LOG_ERR_STRINGS[0], ODU_TFRM_LOG_ERR_TABLE_BASE, ODU_TFRM_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);
} /* odu_tfrm_handle_restart_init */

/*******************************************************************************
* odu_tfrm_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of an ODU_TFRM block instance.
*
*
* INPUTS:
*   *odu_tfrm_handle        - pointer to ODU_TFRM handle instance.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 odu_tfrm_base_address_get(odu_tfrm_handle_t *odu_tfrm_handle)
{
    PMC_ENTRY();
    
    PMC_RETURN(odu_tfrm_handle->base.base_address);
} /* odu_tfrm_base_address_get */

/*******************************************************************************
* odu_tfrm_chnl_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures an ODUk framer in ODU_TFRM to be operational.
*
*   When an ODUk channel is not operational, no overhead insertion
*   from registers or the overhead port is performed. Note that FAS/MFAS is
*   still inserted when the framer channel is not operational.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   enbl                - Enables or disables the ODUk channel \n
*                         0 -- Disabled; ODUk is passed through transparently.
*                         1 -- Enabled; the ODUk channel is operational. \n
*                              Overhead insertion is provisioned as required.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_chnl_init(odu_tfrm_handle_t *odu_tfrm_handle,
                                    UINT32 chnl_id, UINT32 enbl)
{
    /* variable declaration */
    PMC_ENTRY();

    /* Set FAU_PASSTHRU bit */
    odu_tfrm_lfield_range_PASSTHRU_set(NULL, odu_tfrm_handle, chnl_id, chnl_id,
                                       1-enbl);                          

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_chnl_init */

/*******************************************************************************
* odu_tfrm_passthru_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the PASSTHRU status for all channels
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*
* OUTPUTS:
*   passthru            - the passthru vector
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odu_tfrm_passthru_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                  UINT32 passthru[3])
{
    /* variable declaration */
    PMC_ENTRY();
    odu_tfrm_lfield_PASSTHRU_get(NULL,odu_tfrm_handle,passthru);

    PMC_RETURN();
} /* odu_tfrm_passthru_get */

/*******************************************************************************
* odu_tfrm_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes ODU_TFRM block to a known start state.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_init(odu_tfrm_handle_t *odu_tfrm_handle)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    odu_tfrm_buffer_t b[1];
    UINT32 i;
    
    PMC_ENTRY();

    odu_tfrm_buffer_init(b,odu_tfrm_handle);
    
    /* initialize STAT = 1 for normal operation */
    for (i = 0; i < odu_tfrm_handle->cfg.tcm_pool_size; i++)
    {
        odu_tfrm_field_STAT_set(NULL, odu_tfrm_handle, i, 1);
        
        /*ensure that DM state machine starts in inacctive state*/
        odu_tfrm_field_DMT_SRC_set(NULL, odu_tfrm_handle, i, 0);       
        PMC_ATOMIC_YIELD(odu_tfrm_handle->base.parent_handle->parent_handle,0);     
    }        
    
    /* initialize MAINT_SIG ram based register field to 0 */
    for (i = 0; i < odu_tfrm_handle->cfg.num_chans; i++)
    { 
        if (odu_tfrm_handle->cfg.tfrm_id == ODU_TFRM_STG3B)
        {
            /* SSF and SD_SF status is propagated transparently */
            odu_tfrm_field_SSF_PROP_set(b, odu_tfrm_handle, i, 1);
            odu_tfrm_field_SF_SD_PROP_set(b, odu_tfrm_handle, i, 1);
        }
        else
        {
            /* SSF and SD_SF status is propagated transparently */
            odu_tfrm_field_SSF_PROP_set(b, odu_tfrm_handle, i, 0);
            odu_tfrm_field_SF_SD_PROP_set(b, odu_tfrm_handle, i, 0);
        }
        odu_tfrm_field_TCM1_TRANSPARENT_set(b, odu_tfrm_handle, i, 1);
        odu_tfrm_field_TCM2_TRANSPARENT_set(b, odu_tfrm_handle, i, 1);
        odu_tfrm_field_TCM3_TRANSPARENT_set(b, odu_tfrm_handle, i, 1);
        odu_tfrm_field_TCM4_TRANSPARENT_set(b, odu_tfrm_handle, i, 1);
        odu_tfrm_field_TCM5_TRANSPARENT_set(b, odu_tfrm_handle, i, 1);
        odu_tfrm_field_TCM6_TRANSPARENT_set(b, odu_tfrm_handle, i, 1);
        odu_tfrm_field_PM_TRANSPARENT_set(b, odu_tfrm_handle, i, 1);
                    
        
        odu_tfrm_field_MAINT_SIG_set(b, odu_tfrm_handle, i, 0);
        
        /*ensure that DM state machine starts in inacctive state*/
        odu_tfrm_field_DMP_SRC_set(b, odu_tfrm_handle, i, 0);        

        odu_tfrm_buffer_flush(b);

        odu_tfrm_field_GCC1_INS_set(b, odu_tfrm_handle, i, 0);
        odu_tfrm_field_GCC2_INS_set(b, odu_tfrm_handle, i, 0);
        odu_tfrm_field_APS_PCC_INS_set(b, odu_tfrm_handle, i, 0);
        odu_tfrm_field_RES2_INS_set(b, odu_tfrm_handle, i, 0);
        odu_tfrm_field_EXP_INS_set(b, odu_tfrm_handle, i, 0);
        odu_tfrm_field_RES1_INS_set(b, odu_tfrm_handle, i, 0);
        odu_tfrm_field_PM_TCM_RES_INS_set(b, odu_tfrm_handle, i, 0);
        odu_tfrm_field_TCM_ACT_INS_set(b, odu_tfrm_handle, i, 0);
        odu_tfrm_field_FTFL_INS_set(b, odu_tfrm_handle, i, 0);
        /*by default, enable the RCP interface */
        odu_tfrm_field_RCP_EN_set(b, odu_tfrm_handle, i, 1);

        odu_tfrm_buffer_flush(b);

        odu_tfrm_field_STAT_PM_set(NULL, odu_tfrm_handle, i, 1);
        PMC_ATOMIC_YIELD(odu_tfrm_handle->base.parent_handle->parent_handle,0);

    }
    
    PMC_RETURN(result);
} /* odu_tfrm_init */

/*******************************************************************************
* odu_tfrm_pool_id_assign
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function either internally manages tcmi pool IDs or allows the calling
*   system to specify explicit IDs for use with a resource.
*
*   When user assigned pool IDs are used, if the pool ID is already allocated
*   to a different resource an error will be raised.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed. 
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM NIM or TCMi field instance: \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM \n
*                         8         -- PM NIM (non-intrusive monitoring)
*   assign_mode         - TCMi pool ID assignment mode \n
*                         0 -- SW Driver assigns Pool ID for specified
*                              PM NIM/TCMi \n
*                         1 -- pool_id specifies the Pool ID used for specified
*                              PM NIM/TCMi.
*   pool_id             - Pool ID assigned to PM NIM/TCMi message.  Only valid
*                         if assign_mode set to 1.   Legal range 0 to 255.
*
* OUTPUTS:
*   *assigned_pool_id   - assigned pool id to be retured to the user
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pool_id_assign(odu_tfrm_handle_t *odu_tfrm_handle,
                                         UINT32 chnl_id, odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                         UINT32 assign_mode, UINT32 pool_id,
                                         UINT16 *assigned_pool_id)
{
    /* variable declaration */
    UINT32 i;
    UINT32 valid;
    UINT32 tcm_pool_id = ODU_TFRM_UNUSED_POOL_ID;
    BOOL8 state = 1;
    UINT32 cnt = 0;

    PMC_ENTRY();

    /* if user assigns the pool_id, check to see if this pool_id is free */
    if (assign_mode == 1)
    {
        valid = odu_tfrm_tcm_pool_id_check(odu_tfrm_handle, pool_id);
        if (valid)
        {
            PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_ENTRY_UNAVAIL);
        }
        else
        {
            tcm_pool_id = pool_id;
        }
    }
    else
    {
        /* assign pool_id internally */
        /* Search for a free POOL ID and assign a unique TCM_POOL_ID */
        for (i = 0; i <  odu_tfrm_handle->cfg.tcm_pool_size; i++)
        {
            valid = odu_tfrm_tcm_pool_id_check(odu_tfrm_handle, i);
            PMC_LOG_TRACE("%s, %s, %d, Valid:State of TCM pool id[%u] = %u\n", __FILE__, __FUNCTION__, __LINE__, i, valid);
            if (valid == 0)
            {
                tcm_pool_id = i;
                PMC_LOG_TRACE("%s, %s, %d, tcm_pool_id assigned = %u...\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);

                break;
            }
            else
            {
                cnt++;
            }

        }

        if (cnt == odu_tfrm_handle->cfg.tcm_pool_size)
        {
            PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_FULL);
        }
    }

    /* update the tcm pool pool id table */
    if (tcm_pool_id < odu_tfrm_handle->cfg.tcm_pool_size)
    {
        odu_tfrm_tcm_pool_update(odu_tfrm_handle, chnl_id, pm_tcm_num, tcm_pool_id, 
                                 state, ODU_TFRM_PM_TCM_START);
    }
    else
    {
        PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_ENTRY_OUT_OF_RANGE);
    }

    
    *assigned_pool_id = tcm_pool_id;


    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_pool_id_assign */

/*******************************************************************************
*  odu_tfrm_pool_id_return
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function looks up the pool id from the internal pool id table
*   and returns the pool id associated with the TCM/Floating TCM/PM_NIM field
*   instance of a channel.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - channel ID
*   tcm_num             - TCM layer number for the channel \n
*                         1 to 6 -- TCM1 to TCM6 \n
*                         7      -- Floating TCM \n
*                         8      -- PM NIM
*
* OUTPUTS:
*   *pool_id            - pool id
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pool_id_return(odu_tfrm_handle_t *odu_tfrm_handle,
                                         UINT32 chnl_id, odu_tfrm_pm_tcm_id_t tcm_num,
                                         UINT16 *pool_id)
{
    /* variable declaration */
    UINT32 i, count = 0;

    PMC_ENTRY();

    PMC_LOG_TRACE("%s, %s, %d, cfg_ptr->tcm_pool_size = %u\n", __FILE__, __FUNCTION__, __LINE__, odu_tfrm_handle->cfg.tcm_pool_size);
    PMC_LOG_TRACE("%s, %s, %d, tcm pool size = %u\n", __FILE__, __FUNCTION__, __LINE__, odu_tfrm_handle->cfg.tcm_pool_size);

    for (i = 0; i < odu_tfrm_handle->cfg.tcm_pool_size; i++)
    {
        if ((odu_tfrm_handle->var.tcm_pool_ptr[i].chnl_id == chnl_id) && (odu_tfrm_handle->var.tcm_pool_ptr[i].tcm_num == tcm_num))
        {
            /* found the entry */
            PMC_LOG_TRACE("%s, %s, %d, Found the entry: (var_ptr->tcm_pool[i])->chnl_id[%u] = %u, (var_ptr->tcm_pool[i])->tcm_num[%u] = %u\n", __FILE__, __FUNCTION__, __LINE__, i, (odu_tfrm_handle->var.tcm_pool_ptr[i]).chnl_id, i, (odu_tfrm_handle->var.tcm_pool_ptr[i]).tcm_num);
            *pool_id = i;
            count++;
        }
    }

    PMC_LOG_TRACE("%s, %s, %d, pool id duplicate count = %u\n", __FILE__, __FUNCTION__, __LINE__, count);

    /* check for duplicates */
    if (count > 1)
    {
        PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_ID_DUPLICATE);
    }

    if (count == 0)
    {
        PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_ID_NOT_FOUND);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_pool_id_return */

/*******************************************************************************
* odu_tfrm_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function provisions an ODU channl with OH insertion function.
*
*   Prior to calling this API, the layer must have been configured with
*   odu_tfrm_tcmi_cfg() API. 
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         See odu_tfrm_pm_tcm_id_t.
*   parent_tcm_num      - PM or TCMi field instance of the parent layer: \n
*                         See odu_tfrm_pm_tcm_id_t
*   mode                - functional mode of layer source function. See
*                         odu_tfrm_pm_tcm_mode_t for valid options. \n
*                         0 -- Transparent \n
*                         1 -- Operational
*   enbl                - Enables or disables the ODUk channel \n
*                         0 -- Disabled; ODUk is passed through transparently.
*                         1 -- Enabled; the ODUk channel is operational. \n
*                              Overhead insertion is provisioned as required.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_chnl_prov(odu_tfrm_handle_t *odu_tfrm_handle,
                                    UINT32 chnl_id,
                                    odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                    odu_tfrm_pm_tcm_id_t parent_tcm_num,
                                    odu_tfrm_pm_tcm_mode_t mode,
                                    BOOL8 enbl)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    if (1 == enbl)
    {
        result = odu_tfrm_tcmi_cfg(odu_tfrm_handle, chnl_id, pm_tcm_num, 
                                   parent_tcm_num, mode, enbl);
        
        
        if (PMC_SUCCESS == result)
        {   
            /* put the channel in passthru prior to configuring RCP_EN */
            result = odu_tfrm_chnl_init(odu_tfrm_handle, chnl_id, 0);
            
            /* enable RCP for this channel */
            odu_tfrm_field_RCP_EN_set(NULL, odu_tfrm_handle, chnl_id, 1);
            
        }  
    } else
    {
        odu_tfrm_internal_chnl_clean(odu_tfrm_handle, chnl_id);
    }
                                                                                   
    PMC_RETURN(result);
    
} /* odu_tfrm_chnl_prov */    





/*******************************************************************************
* odu_tfrm_gen_and_activate_tcm_pm_layers
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function generates the TCM layering heirarchy and sets layers
*   to be transparent or non-transparent based on the odukp and odukt
*   maintenance signals applied.
*
*   This should be called:
*      - every time a TCM is provisioned or deprovisioned
*      - every time an odukt or odukp maintenance signal is applied
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_gen_and_activate_tcm_pm_layers(odu_tfrm_handle_t *odu_tfrm_handle,
                                                         UINT32 chnl_id)
{

    PMC_ERROR result = PMC_SUCCESS;

    odu_tfrm_layer_struct_t tcm_layers[ODU_SOURCE_MAX_PM_TCM_LAYERS];
    UINT32 num_layers = 0;
    UINT32 i;
    UINT32 pool_id;
    odu_tfrm_pm_tcm_id_t pm_tcm_num;
    BOOL maint_sig_found = FALSE;
    BOOL transparent_mode;
    BOOL tcm_lck;

    PMC_ENTRY();

    PMC_ASSERT(odu_tfrm_handle != NULL, ODU_TFRM_ERR_INVALID_PTR, 0, 0);  
    
    /*get the existing rfrm layer_hierachy */
    /* returns TCMs/PM-NIM/and PM*/
    result = odu_tfrm_layer_hierarchy_get(odu_tfrm_handle,
                                          chnl_id,
                                          tcm_layers,
                                          &num_layers);




    /* if odukp mux, all layers must be transparent */
    if(odu_tfrm_handle->var.odukp_mux_maint_sig[chnl_id] != 0)
    {
        /*set context for TRANPARENT setting below*/
        maint_sig_found = TRUE;

        /* set MAINT_SIG bit to desired value */
        odu_tfrm_field_MAINT_SIG_set(NULL, odu_tfrm_handle, chnl_id, odu_tfrm_handle->var.odukp_mux_maint_sig[chnl_id]);
    }
    else
    {
        tcm_lck = FALSE;
        for(i=0;i<num_layers;i++)
        {
            if(tcm_layers[i].pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
            {
                if(odu_tfrm_handle->var.tcm_pool_ptr[tcm_layers[i].pool_id].odukt_maint_sig != 0)
                {
                    tcm_lck = TRUE;
                }
            }
        }

        
        if(tcm_lck)
        {
            /*LCK is being forced at a TCM level*/
            odu_tfrm_field_MAINT_SIG_set(NULL, odu_tfrm_handle, chnl_id, 2);
        }
        else if(odu_tfrm_handle->var.odukp_pck_maint_sig[chnl_id] != 0)
        {
            /*maintenance signal is being applied over PM layer only*/
            odu_tfrm_field_MAINT_SIG_set(NULL, odu_tfrm_handle, chnl_id, odu_tfrm_handle->var.odukp_pck_maint_sig[chnl_id]);
        }
        else
        {
            /*no maintenance signal being applied*/
            odu_tfrm_field_MAINT_SIG_set(NULL, odu_tfrm_handle, chnl_id, 0);
        }
    }



    for(i=(num_layers);i>0;i--)
    {

        pm_tcm_num = tcm_layers[i-1].pm_tcm_num;
        pool_id = tcm_layers[i-1].pool_id;

        
        /* For ODUkP packet adaptation, only the PM layer
        needs to be made transparent */
        if(pm_tcm_num == ODU_TFRM_MESSAGE_TYPE_PM &&
           odu_tfrm_handle->var.odukp_pck_maint_sig[chnl_id] != 0)
        {
            maint_sig_found = TRUE;
        }


        if(maint_sig_found)
        {
            transparent_mode = TRUE;
        }
        else
        {
            transparent_mode = FALSE;
        }

        result = odu_tfrm_pm_tcmi_mode_set(odu_tfrm_handle,
                                           chnl_id,
                                           pm_tcm_num,
                                           transparent_mode);
        


        /* if odukt, sub layers need to be made tranparent */

        if(pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
        {
            if(odu_tfrm_handle->var.tcm_pool_ptr[pool_id].odukt_maint_sig != 0)
            {
                maint_sig_found = TRUE;
            }
        }

    } 

     PMC_RETURN(result);
} /*  odu_tfrm_gen_and_activate_tcm_pm_layers */


/*******************************************************************************
* FUNCTION: odu_tfrm_int_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the interrupt table.
*   
*
* INPUTS:
*   odu_tfrm_handle   - Pointer to subsystem handle
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_int_enable(odu_tfrm_handle_t     *odu_tfrm_handle,
                                     odu_tfrm_int_t        *int_table_ptr,
                                     odu_tfrm_int_t        *int_en_table_ptr,
                                     BOOL8                  enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();    

    PMC_ASSERT(NULL != odu_tfrm_handle, ODU_TFRM_ERR_INVALID_PTR, 0, 0);
    
#ifdef ODU_TFRM_INT_SINGLE
#undef ODU_TFRM_INT_SINGLE
#endif
#define ODU_TFRM_INT_SINGLE    UTIL_GLOBAL_INT_ENABLE_SINGLE
        
    ODU_TFRM_INT_TABLE_DEFINE();    

    PMC_RETURN(result);
} /* odu_tfrm_int_enable */

/*******************************************************************************
* FUNCTION: odu_tfrm_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the interrupt table.
*   
*
* INPUTS:
*   odu_tfrm_handle   - Pointer to subsystem handle
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_int_clear(odu_tfrm_handle_t      *odu_tfrm_handle,
                                    odu_tfrm_int_t    *int_table_ptr,
                                    odu_tfrm_int_t    *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != odu_tfrm_handle, ODU_TFRM_ERR_INVALID_PTR, 0, 0);
    
#ifdef ODU_TFRM_INT_SINGLE
#undef ODU_TFRM_INT_SINGLE
#endif
#define ODU_TFRM_INT_SINGLE UTIL_GLOBAL_INT_CLEAR_SINGLE
    
    ODU_TFRM_INT_TABLE_DEFINE();
    
    PMC_RETURN(result);
} /* odu_tfrm_int_clear */

/*******************************************************************************
* FUNCTION: odu_tfrm_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the interrupt table.
*   
*
* INPUTS:
*   odu_tfrm_handle   - Pointer to subsystem handle
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_int_retrieve(odu_tfrm_handle_t      *odu_tfrm_handle,
                                       odu_tfrm_int_t    *filt_table_ptr,
                                       odu_tfrm_int_t    *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != odu_tfrm_handle, ODU_TFRM_ERR_INVALID_PTR, 0, 0);

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odu_tfrm_handle, odu_tfrm, UTIL_GLOBAL_INT_OPTIMIZE);
    
#ifdef ODU_TFRM_INT_SINGLE
#undef ODU_TFRM_INT_SINGLE
#endif
#define ODU_TFRM_INT_SINGLE UTIL_GLOBAL_INT_RETRIEVE_SINGLE
    
    ODU_TFRM_INT_TABLE_DEFINE();

    UTIL_GLOBAL_INT_BUFFER_FLUSH(odu_tfrm_handle, odu_tfrm);

    PMC_RETURN(result);
} /* odu_tfrm_int_retrieve */

/*******************************************************************************
* FUNCTION: odu_tfrm_int_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   odu_tfrm_handle   - Pointer to subsystem handle
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_int_enabled_check(odu_tfrm_handle_t    *odu_tfrm_handle,
                                            odu_tfrm_int_t  *int_en_table_ptr,
                                            BOOL8                *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != odu_tfrm_handle, ODU_TFRM_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(int_found_ptr != NULL, ODU_TFRM_ERR_INVALID_PTR, 0, 0);

#ifdef ODU_TFRM_INT_SINGLE
#undef ODU_TFRM_INT_SINGLE
#endif
#define ODU_TFRM_INT_SINGLE UTIL_GLOBAL_INT_CHECK_SINGLE
    
    ODU_TFRM_INT_TABLE_DEFINE();
    
    PMC_RETURN(result);
} /* odu_tfrm_int_enabled_check */


/*******************************************************************************
* FUNCTION: odu_tfrm_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   odu_tfrm_handle   - Pointer to subsystem handle
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_int_chnl_enable(odu_tfrm_handle_t     *odu_tfrm_handle,
                                          UINT32                 chnl_id,
                                          odu_tfrm_int_chnl_t   *int_table_ptr,
                                          odu_tfrm_int_chnl_t   *int_en_table_ptr,
                                          BOOL8                  enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();    

    result = odu_tfrm_int_validate(odu_tfrm_handle, chnl_id);
    
#ifdef ODU_TFRM_INT_CHNL_RANGE
#undef ODU_TFRM_INT_CHNL_RANGE
#endif
#define ODU_TFRM_INT_CHNL_RANGE    UTIL_GLOBAL_INT_LFIELD_ENABLE_SINGLE
        
    ODU_TFRM_INT_CHNL_TABLE_DEFINE();    

    PMC_RETURN(result);
} /* odu_tfrm_int_chnl_enable */

/*******************************************************************************
* FUNCTION: odu_tfrm_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   odu_tfrm_handle   - Pointer to subsystem handle
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_int_chnl_clear(odu_tfrm_handle_t      *odu_tfrm_handle,
                                         UINT32                  chnl_id,
                                         odu_tfrm_int_chnl_t    *int_table_ptr,
                                         odu_tfrm_int_chnl_t    *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = odu_tfrm_int_validate(odu_tfrm_handle, chnl_id);
    
#ifdef ODU_TFRM_INT_CHNL_RANGE
#undef ODU_TFRM_INT_CHNL_RANGE
#endif
#define ODU_TFRM_INT_CHNL_RANGE UTIL_GLOBAL_INT_LFIELD_CLEAR_SINGLE
    
    ODU_TFRM_INT_CHNL_TABLE_DEFINE();
    
    PMC_RETURN(result);
} /* odu_tfrm_int_chnl_clear */

/*******************************************************************************
* FUNCTION: odu_tfrm_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   odu_tfrm_handle   - Pointer to subsystem handle
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
*
* OUTPUTS:
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_int_chnl_retrieve(odu_tfrm_handle_t      *odu_tfrm_handle,
                                            UINT32                  chnl_id,
                                            odu_tfrm_int_chnl_t    *filt_table_ptr,
                                            odu_tfrm_int_chnl_t    *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    result = odu_tfrm_int_validate(odu_tfrm_handle, chnl_id);

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odu_tfrm_handle, odu_tfrm, UTIL_GLOBAL_INT_OPTIMIZE);
    
#ifdef ODU_TFRM_INT_CHNL_RANGE
#undef ODU_TFRM_INT_CHNL_RANGE
#endif
#define ODU_TFRM_INT_CHNL_RANGE UTIL_GLOBAL_INT_LFIELD_RETRIEVE_SINGLE
    
    ODU_TFRM_INT_CHNL_TABLE_DEFINE();

#ifdef ODU_TFRM_STATUS_CHNL_RANGE
#undef ODU_TFRM_STATUS_CHNL_RANGE
#endif
#define ODU_TFRM_STATUS_CHNL_RANGE UTIL_GLOBAL_STATUS_LFIELD_RETRIEVE_SINGLE
    
    ODU_TFRM_STATUS_CHNL_TABLE_DEFINE();

    UTIL_GLOBAL_INT_BUFFER_FLUSH(odu_tfrm_handle, odu_tfrm);

    PMC_RETURN(result);
} /* odu_tfrm_int_chnl_retrieve */

/*******************************************************************************
* FUNCTION: odu_tfrm_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   odu_tfrm_handle   - Pointer to subsystem handle
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_int_chnl_enabled_check(odu_tfrm_handle_t    *odu_tfrm_handle,
                                                 UINT32                chnl_id,
                                                 odu_tfrm_int_chnl_t  *int_en_table_ptr,
                                                 BOOL8                *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    result = odu_tfrm_int_validate(odu_tfrm_handle, chnl_id);
    
#ifdef ODU_TFRM_INT_CHNL_RANGE
#undef ODU_TFRM_INT_CHNL_RANGE
#endif
#define ODU_TFRM_INT_CHNL_RANGE UTIL_GLOBAL_INT_LFIELD_CHECK_SINGLE
    
    ODU_TFRM_INT_CHNL_TABLE_DEFINE();
    
    PMC_RETURN(result);
} /* odu_tfrm_int_chnl_enabled_check */

#ifdef PMC_INTENRAL
/* TFRM DM_PID interrupt should not be exposed to customer. DM_PID is already used
intenrally to monitor delay monitor measurement. */
/*******************************************************************************
* FUNCTION: odu_tfrm_int_pid_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   odu_tfrm_handle     - Pointer to subsystem handle
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_int_pid_chnl_enable(odu_tfrm_handle_t     *odu_tfrm_handle,
                                              UINT32                 chnl_id,
                                              odu_tfrm_int_pid_chnl_t   *int_table_ptr,
                                              odu_tfrm_int_pid_chnl_t   *int_en_table_ptr,
                                              BOOL8                  enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();    

    result = odu_tfrm_int_pid_validate(odu_tfrm_handle, chnl_id, (void *) int_table_ptr);
    
#ifdef ODU_TFRM_INT_PID_CHNL_RANGE
#undef ODU_TFRM_INT_PID_CHNL_RANGE
#endif
#define ODU_TFRM_INT_PID_CHNL_RANGE    UTIL_GLOBAL_INT_LFIELD_ENABLE_SINGLE
        
    ODU_TFRM_INT_PID_CHNL_TABLE_DEFINE();    

    PMC_RETURN(result);
} /* odu_tfrm_int_pid_chnl_enable */

/*******************************************************************************
* FUNCTION: odu_tfrm_int_pid_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   odu_tfrm_handle   - Pointer to subsystem handle
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   pool_id_ptr       - list of tcm pool ids
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_int_pid_chnl_clear(odu_tfrm_handle_t          *odu_tfrm_handle,
                                             UINT32                      chnl_id,
                                             UINT32                     *pool_id_ptr,
                                             odu_tfrm_int_pid_chnl_t    *int_table_ptr,
                                             odu_tfrm_int_pid_chnl_t    *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32 int_ar[ODU_TFRM_PIDS_PER_CHNL];
    UINT32 itr;
    
    
    PMC_ENTRY();
    
    result = odu_tfrm_int_pid_validate(odu_tfrm_handle, chnl_id, (void *)int_table_ptr);
    
    if (PMC_SUCCESS == result)
    {
        odu_tfrm_lfield_DM_PID_I_get(NULL, odu_tfrm_handle, int_ar);
        
        PMC_MEMSET(int_ar, 0x0, ODU_TFRM_PIDS_PER_CHNL * sizeof(UINT32));
        
        for (itr = 0; itr < ODU_TFRM_NUM_PID_CHNL_GET(odu_tfrm_handle); 
             itr += 1)
        {
            if ((pool_id_ptr[itr] != ODU_TFRM_UNASSIGNED_POOL_ID) &&
                (NULL == int_table_ptr || 0 != int_table_ptr[itr].dm_pid_i) && 
                (NULL == int_en_table_ptr || int_en_table_ptr[itr].dm_pid_i))
            {
                if (ODU_TFRM_PIDS_PER_CHNL > (pool_id_ptr[itr] / 32))
                {
                    int_ar[pool_id_ptr[itr] / 32] |= (1 << ((pool_id_ptr[itr] % 32)));
                }
            }
        }
        
        odu_tfrm_lfield_DM_PID_I_set_to_clear(NULL, odu_tfrm_handle, int_ar);
    }
    
    PMC_RETURN(result);
} /* odu_tfrm_int_pid_chnl_clear */

/*******************************************************************************
* FUNCTION: odu_tfrm_int_pid_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   odu_tfrm_handle   - Pointer to subsystem handle
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   pool_id_ptr       - list of tcm pool ids
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
*   filt_table_ptr    - pointer to channel enabled interrupt table
*
* OUTPUTS:
*   int_table_pid_ptr - pointer to the channel interrupt table to fill with 
*                       TCM PIDs
*   int_table_ptr     - pointer to channel interrupt table to fill with 
*                       retrieved data
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_int_pid_chnl_retrieve(odu_tfrm_handle_t          *odu_tfrm_handle,
                                                UINT32                      chnl_id,
                                                UINT32                     *pool_id_ptr,
                                                odu_tfrm_int_pid_chnl_t    *filt_table_ptr,
                                                UINT32                     *int_table_pid_ptr,
                                                odu_tfrm_int_pid_chnl_t    *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          pool_id;
    UINT32          i;

    PMC_ENTRY();

    result = odu_tfrm_int_pid_validate(odu_tfrm_handle, chnl_id, (void*) int_table_ptr);
    
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odu_tfrm_handle, odu_tfrm, UTIL_GLOBAL_INT_OPTIMIZE);

    /* Get TCM pool id's for given TFRM chnl */
    for (i = 0; i < ODU_TFRM_PIDS_PER_CHNL; i++)
    {
        pool_id = pool_id_ptr[i];     

        /* Store TCM pool id in interrupt table */
        int_table_pid_ptr[i] = pool_id;   

        /* Retrieve pool id interrupts if PID is valid*/
        if (ODU_TFRM_TCM_PID_IS_VALID(pool_id))
        {
            chnl_id = pool_id;
            int_table_ptr[i].dm_pid_i = odu_tfrm_lfield_range_DM_PID_I_get(NULL, odu_tfrm_handle, chnl_id, chnl_id);
        }
    } /* end for */
    
    UTIL_GLOBAL_INT_BUFFER_FLUSH(odu_tfrm_handle, odu_tfrm);

    PMC_RETURN(result);
} /* odu_tfrm_int_pid_chnl_retrieve */

/*******************************************************************************
* FUNCTION: odu_tfrm_int_pid_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   odu_tfrm_handle   - Pointer to subsystem handle
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   pool_id_ptr       - list of tcm pool ids
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_int_pid_chnl_enabled_check(odu_tfrm_handle_t        *odu_tfrm_handle,
                                                     UINT32                    chnl_id,
                                                     UINT32                   *pool_id_ptr,
                                                     odu_tfrm_int_pid_chnl_t  *int_en_table_ptr,
                                                     BOOL8                    *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32 int_ar[ODU_TFRM_PIDS_PER_CHNL];
    UINT32 itr;
    
    PMC_ENTRY();
    
    result = odu_tfrm_int_pid_validate(odu_tfrm_handle, chnl_id, (void *) int_found_ptr);
    

    odu_tfrm_lfield_DM_PID_I_get(NULL, odu_tfrm_handle, int_ar);

    for (itr = 0; itr < ODU_TFRM_NUM_PID_CHNL_GET(odu_tfrm_handle) &&
             FALSE == *int_found_ptr; 
         itr++)
    {
        if ((NULL == int_en_table_ptr || TRUE == int_en_table_ptr[itr].dm_pid_i) && 
            ODU_TFRM_UNASSIGNED_POOL_ID != pool_id_ptr[itr] &&
            (ODU_TFRM_PIDS_PER_CHNL > (pool_id_ptr[itr] / 32)) &&
            0 != ((int_ar[pool_id_ptr[itr] / 32] >> (pool_id_ptr[itr] % 32)) & 0x1))
        {
            *int_found_ptr = TRUE;
        }
    }
    
    PMC_RETURN(result);
} /* odu_tfrm_int_pid_chnl_enabled_check */
#endif /*PMC_INTERNAL*/

/*******************************************************************************
* odu_tfrm_pm_tcmi_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function sets the operational mode of provisioned PM or TCMi layer.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*   transparent_mode    - TRUE : set mode to transparent
*                         FALSE : set mode non-tranparent (operational)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_tfrm_pm_tcmi_mode_set(odu_tfrm_handle_t *odu_tfrm_handle,
                                            UINT32 chnl_id,
                                            odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                            BOOL transparent_mode)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(odu_tfrm_handle != NULL, ODU_TFRM_ERR_INVALID_PTR, 0, 0);


    /* Enable/disable desired layer of the channel */
    switch (pm_tcm_num)
    {
        case ODU_TFRM_MESSAGE_TYPE_PM: 
            odu_tfrm_field_PM_TRANSPARENT_set(NULL, odu_tfrm_handle, chnl_id, transparent_mode);
            break;
        case ODU_TFRM_MESSAGE_TYPE_TCM1:
            odu_tfrm_field_TCM1_TRANSPARENT_set(NULL, odu_tfrm_handle, chnl_id, transparent_mode);
            break;
        case ODU_TFRM_MESSAGE_TYPE_TCM2:
            odu_tfrm_field_TCM2_TRANSPARENT_set(NULL, odu_tfrm_handle, chnl_id, transparent_mode);
            break;
        case ODU_TFRM_MESSAGE_TYPE_TCM3:
            odu_tfrm_field_TCM3_TRANSPARENT_set(NULL, odu_tfrm_handle, chnl_id, transparent_mode);
            break;
        case ODU_TFRM_MESSAGE_TYPE_TCM4:
            odu_tfrm_field_TCM4_TRANSPARENT_set(NULL, odu_tfrm_handle, chnl_id, transparent_mode);
            break;
        case ODU_TFRM_MESSAGE_TYPE_TCM5:
            odu_tfrm_field_TCM5_TRANSPARENT_set(NULL, odu_tfrm_handle, chnl_id, transparent_mode);
            break;
        case ODU_TFRM_MESSAGE_TYPE_TCM6:
            odu_tfrm_field_TCM6_TRANSPARENT_set(NULL, odu_tfrm_handle, chnl_id, transparent_mode);
            break;
        default:
            PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
    }


    PMC_RETURN(result);

} /* odu_tfrm_pm_tcmi_mode_set */




/*******************************************************************************
* odu_tfrm_pm_tcmi_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retreives the operational mode of provisioned PM or TCMi layer.
*
*   Note, this API extracts the mode from the TFRM context not from the
*   TCMx_TRANPARENT or PM_TRANSPARENT registers as these registers are
*   modified during maintenance signal application.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*
* OUTPUTS:
*   *mode               - functional mode of the selected PM or TCMi layer
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_mode_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                           UINT32 chnl_id,
                                           odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                           odu_tfrm_pm_tcm_mode_t *mode)
{
    /* variable declaration */
    odu_tfrm_layer_struct_t tcm_layers[ODU_SOURCE_MAX_PM_TCM_LAYERS];
    UINT32 num_layers = 0;
    UINT32 i;
    BOOL pm_tcm_found = FALSE;

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(odu_tfrm_handle != NULL, ODU_TFRM_ERR_INVALID_PTR, 0, 0);

    /*get the existing rfrm layer_hierachy */
    /* returns TCMs/PM-NIM/and PM*/
    result = odu_tfrm_layer_hierarchy_get(odu_tfrm_handle,
                                          chnl_id,
                                          tcm_layers,
                                          &num_layers);

    /*if the TCM is present in the layering heiracrchy, then
    the mode is operational.  For maintenance signal applications the
    TCMx_TRANSPARENT or PM_TRANSPARENT bit may be set to 1, even though
    the layer is operational */
    for(i=0;i<num_layers;i++)
    {
        if(tcm_layers[i].pm_tcm_num == pm_tcm_num)
        {
            pm_tcm_found = TRUE;
        }
    }

    /* configure for each operating mode */
    if(pm_tcm_found)
    {
        *mode = ODU_TFRM_PM_TCM_TT_OPERATIONAL;
    }
    else
    {
        *mode = ODU_TFRM_PM_TCM_TT_TRANSPARENT;
    }
    
    PMC_RETURN(result);
} /* odu_tfrm_pm_tcmi_mode_get */

/*******************************************************************************
* odu_tfrm_tcmi_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures TCMi layer per channel in ODU_TFRM block.
*
*   When this function is called with 'mode' = 1, the layer specified by
*   'pm_tcm_num' argument will be provisioned for the channel. When a layer is
*   provisioned, it is made non-transparent. Prior to calling this API, a unique 
*   TCM_POOL_ID must have been assigned with odu_tfrm_pool_id_assign().
*
*   When this function is called with 'mode' = 0, the layer specified by
*   'pm_tcm_num' argument will be unprovisioned. Note that when called with
*   'mode' set to 0, the layer must have been previously provisioned for the
*   same channel with this function. If the layer is checked to be transparent
*   at the time deactivation is requested, the function will return an error.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed. \n 
*                         Valid range: 0 - 95
*   tcm_num             - TCMi field instance:\n 
*                         1 to 6    -- TCM1 to TCM6
*   parent_tcm_num      - TCMI field instance of the parent layer. \n
*                         See odu_tfrm_pm_tcm_id_t.
*                         1 to 6    -- TCM1 to TCM6 or DCI if this is the first layer
*   mode                - Provisions or unprovisions the TCM layer
*                         connection monitoring for the ODUk channel \n
*                         1 -- Provisioned; TCM layer is non-transparent. \n
*                         0 -- Unprovisioned; TCM layer is transparent.
*   enbl                - Indicates if the layer is being provisioned or unprovisioned \n
*                         TRUE -- provision a layer
*                         FALSE -- unprovision a layer
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_tcmi_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                   UINT32 chnl_id, 
                                   odu_tfrm_pm_tcm_id_t tcm_num,
                                   odu_tfrm_pm_tcm_id_t parent_tcm_num,
                                   odu_tfrm_pm_tcm_mode_t mode, BOOL8 enbl)
{
    /* variable declaration */
    UINT16 tcm_pool_id = ODU_TFRM_UNUSED_POOL_ID;
    UINT16 parent_pool_id = ODU_TFRM_UNUSED_POOL_ID;
    BOOL8 assign_state = FALSE;
    odu_tfrm_pm_tcm_id_t first_layer;
    UINT32 first_pool_id;

    PMC_ERROR ret_code = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* get pool id if it is a pooled resource */
    /* If provisioning for a TCM layer, find the pool id */
    if (tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
    {
        
        /* make sure that parent provided is not DCI if the PM layer is already provisioned */
        if(odu_tfrm_pm_prov_state_get(odu_tfrm_handle, chnl_id) == TRUE && enbl == 1)
        {
            if(parent_tcm_num == ODU_TFRM_MESSAGE_TYPE_DCI)
            {
                PMC_RETURN((ODU_TFRM_ERR_DCI_PARENT));
            }
        }


        ret_code = odu_tfrm_pool_id_return(odu_tfrm_handle, chnl_id, tcm_num, &tcm_pool_id);
        
        PMC_LOG_TRACE("%s, %s, %d, PM/TCMi layer is to be provisioned...tcm_pool_id assigned = %u...\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);

        if (PMC_SUCCESS == ret_code)
        {
            if (tcm_pool_id > odu_tfrm_handle->cfg.tcm_pool_size)
            {
                PMC_LOG_TRACE("%s, %s, %d, POOL ID unassigned: pool_id = %u\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);
                PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_ID_UNASSIGNED);
            } 
        } 
        
        if (PMC_SUCCESS == ret_code)
        {
            if (parent_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM && parent_tcm_num != ODU_TFRM_MESSAGE_TYPE_DCI && enbl == 1)
            {
                /* find the parent pool id */
                ret_code = odu_tfrm_pool_id_return(odu_tfrm_handle, chnl_id, parent_tcm_num, &parent_pool_id);  
            }
            else {
                /*make sure the PM layer is provisioned if trying to connect to it */
                parent_pool_id = ODU_TFRM_UNUSED_POOL_ID;

                if(parent_tcm_num == ODU_TFRM_MESSAGE_TYPE_PM && enbl == 1)
                {
                    if(odu_tfrm_pm_prov_state_get(odu_tfrm_handle, chnl_id) == FALSE)
                    {
                          PMC_RETURN(ODU_TFRM_ERR_PM_LAYER_NOT_PROVISIONED);
                    }
                }
            }                   
        }                  
    }
    else
    {

        if(odu_tfrm_pm_prov_state_get(odu_tfrm_handle, chnl_id) == TRUE && enbl == 1)
        {
              PMC_RETURN(ODU_TFRM_ERR_PM_LAYER_PROVISIONED);
        }

    }
    
    if (PMC_SUCCESS == ret_code)
    {
        assign_state = (enbl == 1)? TRUE:FALSE;

        /* set pool_id */
        if (mode == ODU_TFRM_PM_TCM_TT_OPERATIONAL)
        {
            /*tcm_mode = ODU_TFRM_PM_TCM_TT_OPERATIONAL;*/
        
            /* Configure TCMx_POOL_ID register */
            switch (tcm_num)
            {
                case ODU_TFRM_MESSAGE_TYPE_PM: /* PM layer */
                    break;
                case ODU_TFRM_MESSAGE_TYPE_TCM1: /* TCM1 */
                    /* assign unique POOL ID */
                    odu_tfrm_field_TCM1_POOL_ID_set(NULL, odu_tfrm_handle, chnl_id, tcm_pool_id);
                    break;
                case ODU_TFRM_MESSAGE_TYPE_TCM2: /* TCM2 */
                    /* assign unique POOL ID */
                    odu_tfrm_field_TCM2_POOL_ID_set(NULL, odu_tfrm_handle, chnl_id, tcm_pool_id);
                    break;
                case ODU_TFRM_MESSAGE_TYPE_TCM3: /* TCM3 */
                    /* assign unique POOL ID */
                    odu_tfrm_field_TCM3_POOL_ID_set(NULL, odu_tfrm_handle, chnl_id, tcm_pool_id);
                    break;
                case ODU_TFRM_MESSAGE_TYPE_TCM4: /* TCM4 */
                    /* assign unique POOL ID */
                    odu_tfrm_field_TCM4_POOL_ID_set(NULL, odu_tfrm_handle, chnl_id, tcm_pool_id);
                    break;
                case ODU_TFRM_MESSAGE_TYPE_TCM5: /* TCM5 */
                    /* assign unique POOL ID */
                    odu_tfrm_field_TCM5_POOL_ID_set(NULL, odu_tfrm_handle, chnl_id, tcm_pool_id);
                    break;
                case ODU_TFRM_MESSAGE_TYPE_TCM6: /* TCM6 */
                    /* assign unique POOL ID */
                    odu_tfrm_field_TCM6_POOL_ID_set(NULL, odu_tfrm_handle, chnl_id, tcm_pool_id);
                    break;
                default:
                    PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
                    break;
            }
        }
    }

    /* if PM is provisioned or first TCM is provisioned, 
    ** then terminate SSF and SF_SD status.
    */
    ret_code = odu_tfrm_dci_child_get(odu_tfrm_handle,
                                        chnl_id,
                                        &first_pool_id,
                                        &first_layer);

    /* perform once only for the first layer */                                  
    if (first_layer == ODU_TFRM_MESSAGE_DONT_CARE)
    {
        /* this is the first layer */
        if (PMC_SUCCESS == ret_code)
        {
            ret_code = odu_tfrm_sf_sd_proc_cfg(odu_tfrm_handle, chnl_id, 0);
        }
        
        if (PMC_SUCCESS == ret_code)
        {
            ret_code = odu_tfrm_ssf_proc_cfg(odu_tfrm_handle, chnl_id, 0);
        }   
    }
   


    if(PMC_SUCCESS == ret_code)
    {

        ret_code = odu_tfrm_update_pm_tcm_ctxt(odu_tfrm_handle,
                                                 chnl_id,
                                                 tcm_num,
                                                 parent_tcm_num,
                                                 mode,
                                                 enbl,
                                                 tcm_pool_id);
    }


    if(PMC_SUCCESS == ret_code)
    {

        ret_code = odu_tfrm_gen_and_activate_tcm_pm_layers(odu_tfrm_handle,
                                                           chnl_id);
    }
    
    PMC_LOG_TRACE("%s, %s, %d, Configuring ODU_tfrm for channel ID = %u...\n", __FILE__, __FUNCTION__, __LINE__, chnl_id);

    PMC_RETURN(ret_code);
} /* odu_tfrm_tcmi_cfg */


/*******************************************************************************
* Run-time Configurations
*******************************************************************************/
/*******************************************************************************
* odu_tfrm_tti_msg_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the expected transmit PM or TCMi TTI message.
*
*   The function takes in the 16 byte Source Access Point Identifier (SAPI),
*   the 16 byte Destination Access Point Identifier (DAPI)as arguments to fill
*   the SAPI and DAPI fields of the 64 byte TTI.
*
*   The range of a SAPI or DAPI characters 15 to 1 are 0 to 127.
*   SAPI and DAPI character 0 must be set to 0x00. Function will return
*   ODU_TFRM_LOG_ERR_INVALID_PARAMETERS if these rules are not adhered.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6 \n
*   oh_ins_field        - selects which field within TTI message to configure.
*                         See odu_tfrm_tti_oh_ins_t.
*                         0 -- SAPI only
*                         1 -- DAPI only
*                         2 -- Operator Specific only
*                         3 -- both SAPI and DAPI
*                         4 -- both SAPI and Operator Specific
*                         5 -- both DAPI and Operator Specific
*                         6 -- all SAPI, DAPI and Operator Specific
*   sapi                - SAPI characters (16 bytes)
*   dapi                - DAPI characters (16 bytes)
*   op_spec             - OPERATOR SPECIFIC characters (32 bytes)
*
* OUTPUTS:
*
* RETURNS:
*   ODU_TFRM_LOG_ERR_INVALID_PARAMETERS - invalid argument
*   0                                   - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_tti_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                  UINT32 chnl_id, odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                  odu_tfrm_tti_oh_ins_t oh_ins_field,
                                  UINT8 sapi[16], UINT8 dapi[16],
                                  UINT8 op_spec[32])
{
    /* variable declaration */
    PMC_ERROR ret_code;

    UINT16 tcm_pool_id;
    UINT32 valid;
    UINT32 i;
    UINT32 j;
    UINT32 start_idx = 0, dapi_idx = 0, opspec_idx = 0;
    UINT32 tti_to_write[16];
    UINT32 byte_val;
    odu_tfrm_cfg_t cfg_ptr;
    odu_tfrm_buffer_t b[1];

    PMC_ENTRY();

    odu_tfrm_buffer_init(b,odu_tfrm_handle);
    /* retreive TCM buffer pool */
    cfg_ptr     = odu_tfrm_handle->cfg;

    /* SAPI */
    j =0;
    for (i = 0; i < 4; i++)
    {
        tti_to_write[i] = 0;
        byte_val = sapi[j++];
        tti_to_write[i] |= byte_val << 24;
        byte_val = sapi[j++];
        tti_to_write[i] |= byte_val << 16;
        byte_val = sapi[j++];
        tti_to_write[i] |= byte_val << 8;
        byte_val = sapi[j++];
        tti_to_write[i] |= byte_val;
    }

    /* DAPI */
    j =0;
    for (i = 4; i < 8; i++)
    {
        tti_to_write[i] = 0;
        byte_val = dapi[j++];
        tti_to_write[i] |= byte_val << 24;
        byte_val = dapi[j++];
        tti_to_write[i] |= byte_val << 16;
        byte_val = dapi[j++];
        tti_to_write[i] |= byte_val << 8;
        byte_val = dapi[j++];
        tti_to_write[i] |= byte_val;
    }

    /* OP_SPEC */
    j =0;
    for (i = 8; i < 16; i++)
    {
        tti_to_write[i] = 0;
        byte_val = op_spec[j++];
        tti_to_write[i] |= byte_val << 24;
        byte_val = op_spec[j++];
        tti_to_write[i] |= byte_val << 16;
        byte_val = op_spec[j++];
        tti_to_write[i] |= byte_val << 8;
        byte_val = op_spec[j++];
        tti_to_write[i] |= byte_val;
    }


    if (pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
    {
        ret_code = odu_tfrm_pool_id_return(odu_tfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);
        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }

        PMC_LOG_TRACE("%s, %s, %d, PM/TCMi layer is to be provisioned...tcm_pool_id assigned = %u...\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);

        /* Check whether or not the TCM POOL ID is valid */
        valid = odu_tfrm_tcm_pool_id_check(odu_tfrm_handle, tcm_pool_id);
        if (valid == 0)
        {
            PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
        }
        else
        {
            start_idx = tcm_pool_id * 16;
        }

        /* find end_index for this PM/TCM layer */
        dapi_idx = start_idx + 4;
        opspec_idx = dapi_idx + 4;

        PMC_LOG_TRACE("%s, %s, %d, dapi_idx = %u...\n", __FILE__, __FUNCTION__, __LINE__, dapi_idx);

        /* Insert SAPI */
        if (oh_ins_field == ODU_TFRM_TTI_SAPI_INS 
            || oh_ins_field == ODU_TFRM_TTI_SAPI_DAPI_INS
            || oh_ins_field == ODU_TFRM_TTI_SAPI_OP_SPEC_INS
            || oh_ins_field == ODU_TFRM_TTI_SAPI_DAPI_OP_SPEC_INS)
        {            
            /* Write SAPI[16] message */
            odu_tfrm_reg_TTI_PID_array_burst_write(NULL, odu_tfrm_handle, start_idx, 4, &tti_to_write[0]);
            odu_tfrm_field_TTI_SAPI_INS_set(b, odu_tfrm_handle, tcm_pool_id, 1);     
        }
        else {
            odu_tfrm_field_TTI_SAPI_INS_set(b, odu_tfrm_handle, tcm_pool_id, 0);
        }

        /* Insert DAPI */
        if (oh_ins_field == ODU_TFRM_TTI_DAPI_INS 
            || oh_ins_field == ODU_TFRM_TTI_SAPI_DAPI_INS
            || oh_ins_field == ODU_TFRM_TTI_DAPI_OP_SPEC_INS
            || oh_ins_field == ODU_TFRM_TTI_SAPI_DAPI_OP_SPEC_INS)
        {
            /* Write DAPI[16] message */
            odu_tfrm_reg_TTI_PID_array_burst_write(NULL, odu_tfrm_handle, dapi_idx, 4, &tti_to_write[4]);
            odu_tfrm_field_TTI_DAPI_INS_set(b, odu_tfrm_handle, tcm_pool_id, 1);
        } 
        else {
            odu_tfrm_field_TTI_DAPI_INS_set(b, odu_tfrm_handle, tcm_pool_id, 0);
        }
        
        /* Insert Op_Spec */                                               
        if (oh_ins_field == ODU_TFRM_TTI_OP_SPEC_INS 
            || oh_ins_field == ODU_TFRM_TTI_SAPI_OP_SPEC_INS
            || oh_ins_field == ODU_TFRM_TTI_DAPI_OP_SPEC_INS
            || oh_ins_field == ODU_TFRM_TTI_SAPI_DAPI_OP_SPEC_INS)
        {
            /* Write OP_SPEC[32] message */
            odu_tfrm_reg_TTI_PID_array_burst_write(NULL, odu_tfrm_handle, opspec_idx, 8, &tti_to_write[8]);
            odu_tfrm_field_TTI_OS_INS_set(b, odu_tfrm_handle, tcm_pool_id, 1);
        }
        else {
            odu_tfrm_field_TTI_OS_INS_set(b, odu_tfrm_handle, tcm_pool_id, 0);
        }         
    }
    else
    {
        start_idx = chnl_id * 16;
        /* find end_index for this PM/TCM layer */
        dapi_idx = start_idx + 4;
        opspec_idx = dapi_idx + 4;

        PMC_LOG_TRACE("%s, %s, %d, dapi_idx = %u...\n", __FILE__, __FUNCTION__, __LINE__, dapi_idx);

        /* Insert SAPI */
        if (oh_ins_field == ODU_TFRM_TTI_SAPI_INS 
            || oh_ins_field == ODU_TFRM_TTI_SAPI_DAPI_INS
            || oh_ins_field == ODU_TFRM_TTI_SAPI_OP_SPEC_INS
            || oh_ins_field == ODU_TFRM_TTI_SAPI_DAPI_OP_SPEC_INS)
        {             
            /* Write SAPI[16] message */
            odu_tfrm_reg_TTI_PM_array_burst_write(NULL, odu_tfrm_handle, start_idx, 4, &tti_to_write[0]);
            odu_tfrm_field_TTI_SAPI_INS_PM_set(b, odu_tfrm_handle, chnl_id, 1);
        }
        else {
            odu_tfrm_field_TTI_SAPI_INS_PM_set(b, odu_tfrm_handle, chnl_id, 0);
        }  

        if (oh_ins_field == ODU_TFRM_TTI_DAPI_INS 
            || oh_ins_field == ODU_TFRM_TTI_SAPI_DAPI_INS
            || oh_ins_field == ODU_TFRM_TTI_DAPI_OP_SPEC_INS
            || oh_ins_field == ODU_TFRM_TTI_SAPI_DAPI_OP_SPEC_INS)
        { 
            /* Write DAPI[16] message */
            odu_tfrm_reg_TTI_PM_array_burst_write(NULL, odu_tfrm_handle, dapi_idx, 4, &tti_to_write[4]);
            odu_tfrm_field_TTI_DAPI_INS_PM_set(b, odu_tfrm_handle, chnl_id, 1);
        } 
        else {
            odu_tfrm_field_TTI_DAPI_INS_PM_set(b, odu_tfrm_handle, chnl_id, 0);
        }

        /* Insert Op_Spec */                                               
        if (oh_ins_field == ODU_TFRM_TTI_OP_SPEC_INS 
            || oh_ins_field == ODU_TFRM_TTI_SAPI_OP_SPEC_INS
            || oh_ins_field == ODU_TFRM_TTI_DAPI_OP_SPEC_INS
            || oh_ins_field == ODU_TFRM_TTI_SAPI_DAPI_OP_SPEC_INS)
        {
            /* Write OP_SPEC[32] message */
            odu_tfrm_reg_TTI_PM_array_burst_write(NULL, odu_tfrm_handle, opspec_idx, 8, &tti_to_write[8]);
            odu_tfrm_field_TTI_OS_INS_PM_set(b, odu_tfrm_handle, chnl_id, 1);
        }
        else {
            odu_tfrm_field_TTI_OS_INS_PM_set(b, odu_tfrm_handle, chnl_id, 0);
        } 
    }
    odu_tfrm_buffer_flush(b);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_tti_cfg */

/*******************************************************************************
* odu_tfrm_tti_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the transmit PM or TCMi TTI message.
*
*   The function extracts the 64 byte TTI relative to the 16 byte Source Access
*   Point Identifier (SAPI), the 16 byte Destination Access Point Identifier
*   (DAPI), and the 32 byte Operator Specific fields.
*
*   Accepted TTI should not be read until a complete accepted meesage has been
*   written.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance:\n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*
*
* OUTPUTS:
*   *sapi_ptr           - pointer to storage of SAPI characters (16 bytes)
*   *dapi_ptr           - pointer to storage of DAPI characters (16 bytes)
*   *op_spec_ptr        - pointer to storage of Operator Specific  (32 bytes)
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_tti_ext(odu_tfrm_handle_t *odu_tfrm_handle,
                                  UINT32 chnl_id, odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                  UINT8 *sapi_ptr, UINT8 *dapi_ptr, UINT8 *op_spec_ptr)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 sapi_val;
    UINT32 dapi_val;
    UINT32 op_spec_val;
    UINT32 start_idx;
    UINT32 dapi_idx;
    UINT32 op_spec_idx;
    UINT32 end_idx;
    UINT32 i; 
    UINT16 tcm_pool_id;
    UINT32 row;
    
    PMC_ENTRY();
    
    if (pm_tcm_num == ODU_TFRM_MESSAGE_TYPE_PM)
    {
        /* read from TTI_PM */
        start_idx = chnl_id * 16;
        
        dapi_idx = start_idx + 4;
        op_spec_idx = dapi_idx + 4;
        end_idx = op_spec_idx + 8;
        
        row = 0;
        /* SAPI[16] */
        for (i = start_idx; i < dapi_idx; i++)
        {
            sapi_val = odu_tfrm_reg_TTI_PM_array_read(NULL, odu_tfrm_handle, i);
            
            /* extract SAPI[0] - [15] */
            sapi_ptr[row*4 + 3] = sapi_val & 0xFF;
            sapi_ptr[row*4 + 2] = (sapi_val >> 8) & 0xFF;
            sapi_ptr[row*4 + 1] = (sapi_val >> 16) & 0xFF;
            sapi_ptr[row*4] = (sapi_val >> 24) & 0xFF;
            
            row++;
        }            
        
        row = 0;
        /* DAPI[16] */
        for (i = dapi_idx; i < op_spec_idx; i++)
        {
            dapi_val = odu_tfrm_reg_TTI_PM_array_read(NULL, odu_tfrm_handle, i);
            
            /* extract DAPI[0] - [15] */
            dapi_ptr[row*4 + 3] = dapi_val & 0xFF;
            dapi_ptr[row*4 + 2] = (dapi_val >> 8) & 0xFF;
            dapi_ptr[row*4 + 1] = (dapi_val >> 16) & 0xFF;
            dapi_ptr[row*4] = (dapi_val >> 24) & 0xFF;
            
            row++;
        }  
        
        row = 0;
        /* OP_SPEC[32] */
        for (i = op_spec_idx; i < end_idx; i++)
        {
            op_spec_val = odu_tfrm_reg_TTI_PM_array_read(NULL, odu_tfrm_handle, i);
            
            op_spec_ptr[row*4 + 3] = op_spec_val & 0xFF;
            op_spec_ptr[row*4 + 2] = (op_spec_val >> 8) & 0xFF;
            op_spec_ptr[row*4 + 1] = (op_spec_val >> 16) & 0xFF;
            op_spec_ptr[row*4] = (op_spec_val >> 24) & 0xFF;
            
            row++;
        }  
    }
    else {
        result = odu_tfrm_pool_id_return(odu_tfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);
        
        if (PMC_SUCCESS == result && (tcm_pool_id > odu_tfrm_handle->cfg.tcm_pool_size))
        {
            result = ODU_TFRM_LOG_CODE_POOL_ID_UNASSIGNED;
        } 
        
        start_idx = tcm_pool_id * 16;
        
        dapi_idx = start_idx + 4;
        op_spec_idx = dapi_idx + 4;
        end_idx = op_spec_idx + 8;
        
        row = 0;
        /* SAPI[16] */
        for (i = start_idx; i < dapi_idx; i++)
        {
            sapi_val = odu_tfrm_reg_TTI_PID_array_read(NULL, odu_tfrm_handle, i);
            
            /* extract SAPI[0] - [15] */
            sapi_ptr[row*4 + 3] = sapi_val & 0xFF;
            sapi_ptr[row*4 + 2] = (sapi_val >> 8) & 0xFF;
            sapi_ptr[row*4 + 1] = (sapi_val >> 16) & 0xFF;
            sapi_ptr[row*4] = (sapi_val >> 24) & 0xFF;
            
            row++;
        }            
        
        row = 0;
        /* DAPI[16] */
        for (i = dapi_idx; i < op_spec_idx; i++)
        {
            dapi_val = odu_tfrm_reg_TTI_PID_array_read(NULL, odu_tfrm_handle, i);
            
            /* extract DAPI[0] - [15] */
            dapi_ptr[row*4 + 3] = dapi_val & 0xFF;
            dapi_ptr[row*4 + 2] = (dapi_val >> 8) & 0xFF;
            dapi_ptr[row*4 + 1] = (dapi_val >> 16) & 0xFF;
            dapi_ptr[row*4] = (dapi_val >> 24) & 0xFF;
            
            row++;
        }  
        
        row = 0;
        /* OP_SPEC[32] */
        for (i = op_spec_idx; i < end_idx; i++)
        {
            op_spec_val = odu_tfrm_reg_TTI_PID_array_read(NULL, odu_tfrm_handle, i);
            
            op_spec_ptr[row*4 + 3] = op_spec_val & 0xFF;
            op_spec_ptr[row*4 + 2] = (op_spec_val >> 8) & 0xFF;
            op_spec_ptr[row*4 + 1] = (op_spec_val >> 16) & 0xFF;
            op_spec_ptr[row*4] = (op_spec_val >> 24) & 0xFF;
            
            row++;
        }       
    }
    
    
    PMC_RETURN(result);
} /* odu_tfrm_tti_ext */

/*******************************************************************************
* odu_tfrm_ftfl_src_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the source of FTFL message for an ODU channel.
*
*   When configured for 'Incoming FTFL value' the value transferred on
*   FTFL byte will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*   When configured for 'Force FTFL value', odu_tfrm_ftfl_msg_cfg() must be
*   called to configure the forced FTFL values.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   source              - PM/TCMi FTFL source \n
*                         0 -- Incoming FTFL value \n
*                         1 -- Force FTFL value
*   force_field         - Only valid when source = 1. \n
*                         0 -- force both backward and forward field \n
*                         1 -- force backward field \n
*                         2 -- force forward field
*
* OUTPUTS:
*
* RETURNS:
*   ODU_TFRM_LOG_ERR_INVALID_PARAMETERS - invalid arg
*   0                                   - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_ftfl_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT8 source,
                                       UINT8 force_field)
{
    /* if source == 0, set FTFL_INS = 0
       if source == 1, set FTFL_INS = 1 */
    /* variable declaration */
    UINT32 ftfl_ins;

    PMC_ENTRY();

    if (source == 0)
    {
        odu_tfrm_field_FTFL_INS_set(NULL, odu_tfrm_handle, chnl_id, 0);
    }
    else
    {
        switch (force_field)
        {
            case 0: /* force both */
                ftfl_ins = 3;
                break;
            case 1: /* force backward */
                ftfl_ins = 1;
                break;
            case 2: /* force forward */
                ftfl_ins = 2;
                break;
            default:
                PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
        }
        odu_tfrm_field_FTFL_INS_set(NULL, odu_tfrm_handle, chnl_id, ftfl_ins);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_ftfl_src_cfg */

/*******************************************************************************
* odu_tfrm_ftfl_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the source of FTFL message for an ODU channel.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *source             - source of FTFL insertion
*
* RETURNS:
*   ODU_TFRM_LOG_ERR_INVALID_PARAMETERS - invalid arg
*   0                                   - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_ftfl_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT8 *source)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    *source = odu_tfrm_field_FTFL_INS_get(NULL, odu_tfrm_handle, chnl_id);

    PMC_RETURN(result);
} /* odu_tfrm_ftfl_src_get */

/*******************************************************************************
* odu_tfrm_ftfl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the accepted FTFL message for an ODU channel.
*
*   The function extracts the 20 byte FTFL message relative to the following
*   fields: \n
*       -- one byte Forward Indication Field \n
*       -- 9 byte Forward Operator Identifier Field \n
*       -- one byte Backward Indication Field \n
*       -- 9 byte Backward Operator Identifier Field
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   fwd_id              - Forward Indication Field. \n
*                         Byte 0 of the FTFL message
*   fwd_op_id           - Forward Operator Identifier Field \n
*                         Byte 9:1 of the FTFL message 
*   bwd_id              - Backward Indication Field. \n
*                         Byte 128 of the FTFL message
*   bwd_op_id           - Backward Operator Identifier Field. \n
*                         Byte 137:129 of the FTFL message
*
* OUTPUTS:
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_ftfl_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                   UINT32 chnl_id, UINT8 fwd_id,
                                   UINT8 fwd_op_id[9], UINT8 bwd_id,
                                   UINT8 bwd_op_id[9])
{
    /* variable declaration */
    UINT32 mask;
    UINT32 i;
    UINT32 start_idx = 0, boif_idx = 0;
    UINT32 row1_val, row2_val, row3_val;

    PMC_ENTRY();
    
    /* find the register for given channel_id */
    start_idx = chnl_id * 8;
    boif_idx = start_idx + 4;
    mask = 0xFFFFFFFF;

    PMC_LOG_TRACE("%s, %s, %d, start_idx = %u...\n", __FILE__, __FUNCTION__, __LINE__, start_idx);
    PMC_LOG_TRACE("%s, %s, %d, boif_idx = %u...\n", __FILE__, __FUNCTION__, __LINE__, boif_idx);

    /* fwd_id and fwd_op_id[9] */
    for (i = start_idx; i < start_idx + 3; i++)
    {
        /* Configure FFIF and fwd_op_id[0] - fwd_op_id[2] */
        if (i == start_idx)
        {
            /* Configure FFIF */
            row1_val = (fwd_id << 24) + (fwd_op_id[0] << 16) + (fwd_op_id[1] << 8) + (fwd_op_id[2]);
            
            PMC_LOG_TRACE("%s, %s, %d,row1_val = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, row1_val);

            /* end of first row */
            odu_tfrm_reg_FTFL_array_field_set(NULL, odu_tfrm_handle, i, mask, 0, row1_val);
        }
        else if (i == start_idx + 1)
        {   /* second row */
            /* Configure fwd_op_id[3] - fwd_op_id[6] */
            row2_val = (fwd_op_id[3] << 24) + (fwd_op_id[4] << 16) + (fwd_op_id[5] << 8) + (fwd_op_id[6]);
            PMC_LOG_TRACE("%s, %s, %d, row2_val value = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, row2_val);

            /* end of second row */
            odu_tfrm_reg_FTFL_array_field_set(NULL, odu_tfrm_handle, i, mask, 0, row2_val);
        }
        else
        {
            /* Configure fwd_op_id[7] - fwd_op_id[8] */
            row3_val = (fwd_op_id[7] << 24) + (fwd_op_id[8] << 16); 
            PMC_LOG_TRACE("%s, %s, %d, row3_val value = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, row3_val);

            /* end of third row */
            odu_tfrm_reg_FTFL_array_field_set(NULL, odu_tfrm_handle, i, mask, 0, row3_val);
        }
    }

    /* bwd_id and bwd_op_id[9] */
    for (i = boif_idx; i < boif_idx + 3; i++)
    {
        /* Extract bwd_op_id[0] - bwd_op_id[2] */
        if (i == boif_idx)
        {
            /* Extract bwd_id */
            row1_val = (bwd_id << 24) + (bwd_op_id[0] << 16) + (bwd_op_id[1] << 8) + (bwd_op_id[2]);

            PMC_LOG_TRACE("%s, %s, %d, bwd_id row1_val = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, row1_val);

            /* end of first row */
            odu_tfrm_reg_FTFL_array_field_set(NULL, odu_tfrm_handle, i, mask, 0, row1_val);
        }
        else if (i == boif_idx + 1)
        {
            /* Extract bwd_op_id[3] - bwd_op_id[6] */
            row2_val = (bwd_op_id[3] << 24) + (bwd_op_id[4] << 16) + (bwd_op_id[5] << 8) + (bwd_op_id[6]);
            PMC_LOG_TRACE("%s, %s, %d, bwd_id row2_val = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, row2_val);

            /* end of second row */
            odu_tfrm_reg_FTFL_array_field_set(NULL, odu_tfrm_handle, i, mask, 0, row2_val);
        }
        else
        {
            /* Extract bwd_op_id[7] - bwd_op_id[8] */
            row3_val = (bwd_op_id[7] << 24) + (bwd_op_id[8] << 16);
            PMC_LOG_TRACE("%s, %s, %d, bwd_id row3_val = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, row3_val);

            /* end of third row */
            odu_tfrm_reg_FTFL_array_field_set(NULL, odu_tfrm_handle, i, mask, 0, row3_val);
        }
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_ftfl_cfg */

/*******************************************************************************
* odu_tfrm_ftfl_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the accepted FTFL message in an ODU framer.
*
*   The function extracts the 20 byte FTFL message relative to the following
*   fields: \n
*       -- one byte Forward Indication Field \n
*       -- 9 byte Forward Operator Identifier Field \n
*       -- one byte Backward Indication Field \n
*       -- 9 byte Backward Operator Identifier Field
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *fwd_id             - Forward Indication Field. 
*                         Byte 0 of the FTFL message
*   *fwd_op_id          - Forward Operator Identifier Field.
*                         Byte 9:1 of the FTFL message
*   *bwd_id             - Backward Indication Field.
*                         Byte 128 of the FTFL message
*   *bwd_op_id          - Backward Operator Identifier Field.
*                         Byte 137:129 of the FTFL message
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_ftfl_ext(odu_tfrm_handle_t *odu_tfrm_handle,
                                   UINT32 chnl_id, UINT8 *fwd_id,
                                   UINT8 *fwd_op_id, UINT8 *bwd_id,
                                   UINT8 *bwd_op_id)
{
    /* variable declaration */
    UINT32 value, mask;
    UINT32 i, j;
    UINT32 start_idx = 0, boif_idx = 0;
    UINT8 tmp_fwd_id[1], tmp_fwd_op_id[9], tmp_bwd_id[1], tmp_bwd_op_id[9];

    /* Initialize to remove compiler warnings */
    tmp_fwd_id[0] = 0;

    tmp_bwd_id[0] = 0;

    PMC_ENTRY();

    PMC_LOG_TRACE("%s, %s, %d, calling odu_tfrm_ftfl_ext...\n", __FILE__, __FUNCTION__, __LINE__);

    /* find the register for given channel_id */
    start_idx = chnl_id * 8;
    boif_idx = start_idx + 4;
    mask = 0xFF;

    PMC_LOG_TRACE("%s, %s, %d, start_idx = %u...\n", __FILE__, __FUNCTION__, __LINE__, start_idx);
    PMC_LOG_TRACE("%s, %s, %d, boif_idx = %u...\n", __FILE__, __FUNCTION__, __LINE__, boif_idx);

    /* fwd_id and fwd_op_id[9] */
    for (i = start_idx; i < start_idx + 3; i++)
    {
        value = odu_tfrm_reg_FTFL_array_read(NULL, odu_tfrm_handle, i);

        /* Extract fwd_op_id[0] - fwd_op_id[2] */
        if (i == start_idx)
        {
            /* Extract fwd_id */
            tmp_fwd_id[0] = (value >> 24) & mask;
            PMC_LOG_TRACE("%s, %s, %d, tmp_fwd_id = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, tmp_fwd_id[0]);
        
            for (j = 3; j > 0 ; j--)
            {
                tmp_fwd_op_id[j-1] = value & 0xFF;
                value = value >> 8;
                PMC_LOG_TRACE("%s, %s, %d, %u: value = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, j, value);
                PMC_LOG_TRACE("%s, %s, %d, tmp_fwd_op_id[%u] value = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, j-1, tmp_fwd_op_id[j-1]);
            }
        }
        else if (i == start_idx + 1)
        {
            /* Extract fwd_op_id[3] - fwd_op_id[6] */
            for (j = 7; j > 3; j--)
            {
                tmp_fwd_op_id[j-1] = value & 0xFF;
                value = value >> 8;
                PMC_LOG_TRACE("%s, %s, %d, %u: value = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, j, value);
                PMC_LOG_TRACE("%s, %s, %d, tmp_fwd_op_id[%u] value = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, j-1, tmp_fwd_op_id[j-1]);
            }
        }
        else
        {
            /* Extract fwd_op_id[7] - fwd_op_id[8] */
            value = value >> 16;
            for (j = 9; j > 7; j--)
            {
                tmp_fwd_op_id[j-1] = value & 0xFF;
                value = value >> 8;
                PMC_LOG_TRACE("%s, %s, %d, %u: value = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, j, value);
                PMC_LOG_TRACE("%s, %s, %d, tmp_fwd_op_id[%u] value = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, j-1, tmp_fwd_op_id[j-1]);

            }
        }
    }

    /* bwd_id and bwd_op_id[9] */
    for (i = boif_idx; i < boif_idx + 3; i++)
    {
        value = odu_tfrm_reg_FTFL_array_read(NULL, odu_tfrm_handle, i);

        /* Extract bwd_op_id[0] - bwd_op_id[2] */
        if (i == boif_idx)
        {
            /* Extract bwd_id */
            tmp_bwd_id[0] = (value >> 24) & 0xFF;
        
            for (j = 3; j > 0 ; j--)
            {
                tmp_bwd_op_id[j-1] = value & 0xFF;
                value = value >> 8;
                PMC_LOG_TRACE("%s, %s, %d, %u: value = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, j, value);
                PMC_LOG_TRACE("%s, %s, %d, bwd_op_id[%u] value = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, j-1, tmp_bwd_op_id[j-1]);
            }
        }
        else if (i == boif_idx + 1)
        {
            /* Extract bwd_op_id[3] - bwd_op_id[6] */
            for (j = 7; j > 3; j--)
            {
                tmp_bwd_op_id[j-1] = value & 0xFF;
                value = value >> 8;
                PMC_LOG_TRACE("%s, %s, %d, %u: value = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, j, value);
                PMC_LOG_TRACE("%s, %s, %d, bwd_op_id[%u] value = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, j-1, tmp_bwd_op_id[j-1]);
            }
        }
        else
        {
            /* Extract bwd_op_id[7] - bwd_op_id[8] */
            value = value >> 16;
            for (j = 9; j > 7; j--)
            {
                tmp_bwd_op_id[j-1] = value & 0xFF;
                value = value >> 8;
                PMC_LOG_TRACE("%s, %s, %d, %u: value = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, j, value);
                PMC_LOG_TRACE("%s, %s, %d, 3: bwd_op_id[%u] value = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, j-1, tmp_bwd_op_id[j-1]);
            }
        }
    }

    /* print fwd_id and fwd_op_id */
    PMC_LOG_TRACE("%s, %s, %d, fwd_id = %u...\n", __FILE__, __FUNCTION__, __LINE__, tmp_fwd_id[0]);
    *fwd_id = tmp_fwd_id[0];

    for (i = 0; i < 9; i++)
    {
        PMC_LOG_TRACE("%s, %s, %d, fwd_op_id[%u] = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, i, tmp_fwd_op_id[i]);
        *fwd_op_id = tmp_fwd_op_id[i];
        PMC_LOG_TRACE("%s, %s, %d, *fwd_op_id = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, *fwd_op_id);
        fwd_op_id++;
    }


    /* print bwd_id and bwd_op_id */
    PMC_LOG_TRACE("%s, %s, %d, bwd_id = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, tmp_bwd_id[0]);
    *bwd_id = tmp_bwd_id[0];

    for (i = 0; i < 9; i++)
    {
        PMC_LOG_TRACE("%s, %s, %d, bwd_op_id[%u] = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, i, tmp_bwd_op_id[i]);
        *bwd_op_id = tmp_bwd_op_id[i];
        PMC_LOG_TRACE("%s, %s, %d, *bwd_op_id = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, *bwd_op_id);
        bwd_op_id++;
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_ftfl_ext */

/*******************************************************************************
* odu_tfrm_pm_tcmi_bdi_bei_biae_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the value of PM BDI/BEI or TCMi BDI/BIAE field of 
*   an ODU transmit data stream when register forcing is the insertion source.
*   Prior to calling this API, RC_EN must be set to 0 by 
*   calling odu_tfrm_pm_tcmi_bdi_bei_biae_src_cfg().
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi layer number \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*   oh_field            - OH field to configure
*                         0 -- BDI field
*                         1 -- BEI/BIAE field
*   force_val           - The value to be inserted into BDI or BEI/BIAE field of
*                         PM or TCMi layer. \n
*                         Valid range: \n
*                         If oh_field == 0 for BDI insertion, 0 -- 0xFF \n
*                         If oh_field == 1 for BEI/BIAE insertion, 0 - 0xFFFF 
*
* OUTPUTS:
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_bdi_bei_biae_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                                   UINT32 chnl_id, 
                                                   odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                                   UINT32 oh_field, 
                                                   UINT32 force_val)
{
    /* variable declaration */
    PMC_ERROR ret_code;

    UINT16 tcm_pool_id;
    
    PMC_ENTRY();
    
    if (chnl_id > 95 || pm_tcm_num > LAST_ODU_TFRM_MESSAGE_TYPE || oh_field > 1
        || (oh_field == 0 && force_val > 0xFF) || (oh_field == 1 && force_val > 0xFFFF))
    {
        PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
    }
    
    if (pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
    {
        ret_code = odu_tfrm_pool_id_return(odu_tfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);
        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }

        if (tcm_pool_id > odu_tfrm_handle->cfg.tcm_pool_size)
        {
            PMC_LOG_TRACE("%s, %s, %d, POOL ID unassigned: pool_id = %u\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);
            PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_ID_UNASSIGNED);
        }
        
        if (oh_field == 0)
        {
            odu_tfrm_field_BDI_set(NULL, odu_tfrm_handle, tcm_pool_id, force_val);
        }
        else {
            odu_tfrm_field_BEI_BIAE_set(NULL, odu_tfrm_handle, tcm_pool_id, force_val);              
        }
    }
    else {   
        if (oh_field == 0)
        {
            odu_tfrm_field_BDI_PM_set(NULL, odu_tfrm_handle, chnl_id, force_val);
        }
        else {
            odu_tfrm_field_BEI_PM_set(NULL, odu_tfrm_handle, chnl_id, force_val);
        }
    }
    
    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_pm_tcmi_bdi_bei_biae_cfg */


/*******************************************************************************
* odu_tfrm_pm_tcmi_stat_src_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the source of PM STAT or TCMi STAT field of an
*   ODU transmit data stream.
*
*   When configured for 'Incoming STAT value' the value transferred on
*   STAT byte will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*   Note that when configuring STAT field, the corresponding TCMx_TRANSPARENT or
*   PM_TRANSPARENT bit must be set to 1.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi layer number \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*   force_val           - For source == 1 (otherwise set to 0) \n
*                         The value to be inserted into STAT field of
*                         PM or TCMi layer \n
*                         Valid range: 0x0 - 0x7
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_stat_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                               UINT32 chnl_id,
                                               odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                               UINT32 force_val)
{
    /* variable declaration */
    PMC_ERROR ret_code;
    UINT16 tcm_pool_id;

    PMC_ENTRY();
    
    /* find pool id */
    if (pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
    {
        ret_code = odu_tfrm_pool_id_return(odu_tfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);
        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }
    
        if (tcm_pool_id > odu_tfrm_handle->cfg.tcm_pool_size)
        {
            PMC_LOG_TRACE("%s, %s, %d, POOL ID unassigned: pool_id = %u\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);
            PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_ID_UNASSIGNED);
        }
    
        
        odu_tfrm_field_STAT_set(NULL, odu_tfrm_handle, tcm_pool_id, force_val);
    }
    else {
        odu_tfrm_field_STAT_PM_set(NULL, odu_tfrm_handle, chnl_id, force_val);
    }
    
    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_pm_tcmi_stat_src_cfg */

/*******************************************************************************
* odu_tfrm_pm_tcmi_stat_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retreives the value of PM STAT or TCMi STAT field from register.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi layer number \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*
* OUTPUTS:
*   *stat_val           - retreived STAT field 
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_stat_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                           UINT32 chnl_id,
                                           odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                           UINT8 *stat_val)
{
    /* variable declaration */
    PMC_ERROR ret_code;
    UINT16 tcm_pool_id;

    PMC_ENTRY();
    
    /* find pool id */
    if (pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
    {
        ret_code = odu_tfrm_pool_id_return(odu_tfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);
        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }
    
        if (tcm_pool_id > odu_tfrm_handle->cfg.tcm_pool_size)
        {
            PMC_LOG_TRACE("%s, %s, %d, POOL ID unassigned: pool_id = %u\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);
            PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_ID_UNASSIGNED);
        }
    
        *stat_val = odu_tfrm_field_STAT_get(NULL, odu_tfrm_handle, tcm_pool_id);
    }
    else {
        *stat_val = odu_tfrm_field_STAT_PM_get(NULL, odu_tfrm_handle, chnl_id);
    }
    
    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_pm_tcmi_stat_get */

/*******************************************************************************
* odu_tfrm_pm_tcmi_bdi_bei_biae_src_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the source of PM BDI or TCMi BDI field of an
*   ODU transmit data stream.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   source              - PM/TCMi STAT source \n
*                         0 -- Inserted from RCP \n
*                         1 -- Force BDI value from register
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_bdi_bei_biae_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                                       UINT32 chnl_id, 
                                                       UINT32 source)
{
    PMC_ERROR ret_code = PMC_SUCCESS;
    UINT32 rcp_en;

    PMC_ENTRY();
    
    rcp_en = (source == 1)? 0:1;

    odu_tfrm_field_RCP_EN_set(NULL, odu_tfrm_handle, chnl_id, rcp_en);

    PMC_RETURN(ret_code);
} /* odu_tfrm_pm_tcmi_bdi_src_cfg */


/*******************************************************************************
* odu_tfrm_pm_tcmi_bdi_bei_biae_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets the source of the BDI/BEI on a per channel basis
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   rcp_en              - PM/TCMi STAT source \n
*                         0 -- RCP_EN = 0
*                         1 -- RCP_EN = 1
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_bdi_bei_biae_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                                       UINT32 chnl_id, 
                                                       UINT32 *rcp_en)
{
    PMC_ERROR ret_code = PMC_SUCCESS;

    PMC_ENTRY();
    
    *rcp_en = odu_tfrm_field_RCP_EN_get(NULL, odu_tfrm_handle, chnl_id);

    PMC_RETURN(ret_code);
} /* odu_tfrm_pm_tcmi_bdi_bei_biae_src_get */


/*******************************************************************************
* odu_tfrm_pm_tcmi_bdi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the BDI value of PM or TCMi of an ODU transmit data 
*   stream from the register.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi layer number \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*
* OUTPUTS:
*   *bdi_val            - BDI value retreived from register
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_bdi_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                          UINT32 chnl_id, 
                                          odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                          UINT8 *bdi_val)
{
    PMC_ERROR ret_code = PMC_SUCCESS;
    UINT16 tcm_pool_id = 0;
    
    PMC_ENTRY();
    
    if (pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
    {
        ret_code = odu_tfrm_pool_id_return(odu_tfrm_handle, chnl_id, 
                                           pm_tcm_num, &tcm_pool_id);
                                           
        if (PMC_SUCCESS == ret_code)
        {
            if (tcm_pool_id > odu_tfrm_handle->cfg.tcm_pool_size)
            {
                PMC_LOG_TRACE("%s, %s, %d, POOL ID unassigned: pool_id = %u\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);
                PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_ID_UNASSIGNED);
            }

            *bdi_val = odu_tfrm_field_BDI_get(NULL, odu_tfrm_handle, tcm_pool_id);
        }            
    }
    else {
        *bdi_val = odu_tfrm_field_BDI_PM_get(NULL, odu_tfrm_handle, chnl_id);
    }

    PMC_RETURN(ret_code);
} /* odu_tfrm_pm_tcmi_bdi_get */

/*******************************************************************************
* odu_tfrm_pm_tcmi_bei_biae_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the BEI/BIAE value of PM or TCMi of an ODU transmit 
*   data  stream from the register.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi layer number \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*
* OUTPUTS:
*   *bei_biae_val       - BEI/BIAE value retreived from register
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_bei_biae_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                               UINT32 chnl_id, 
                                               odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                               UINT8 *bei_biae_val)
{
    PMC_ERROR ret_code = PMC_SUCCESS;
    UINT16 tcm_pool_id = 0;
    
    PMC_ENTRY();
    
    if (pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
    {
        ret_code = odu_tfrm_pool_id_return(odu_tfrm_handle, chnl_id, 
                                           pm_tcm_num, &tcm_pool_id);
                                           
        if (PMC_SUCCESS == ret_code)
        {
            if (tcm_pool_id > odu_tfrm_handle->cfg.tcm_pool_size)
            {
                PMC_LOG_TRACE("%s, %s, %d, POOL ID unassigned: pool_id = %u\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);
                PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_ID_UNASSIGNED);
            }

            *bei_biae_val = odu_tfrm_field_BEI_BIAE_get(NULL, odu_tfrm_handle, 
                                                        tcm_pool_id);
        }            
    }
    else {
        *bei_biae_val = odu_tfrm_field_BEI_PM_get(NULL, odu_tfrm_handle, 
                                                  chnl_id);
    }

    PMC_RETURN(ret_code);
} /* odu_tfrm_pm_tcmi_bei_biae_get */

/*******************************************************************************
* odu_tfrm_pm_tcm_src_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the source and the value of PM&TCM reserved field 
*   of an ODU transmit data stream.
*
*   When configured for 'Incoming PM&TCM value' the value transferred on
*   PM&TCM byte will be from highest to lowest priority: \n
*       - Data from the External Overhead Port (if enabled) \n
*       - Register forcing \n
*       - Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert PM&TCM byte, the value 
*   inserted from register using this API will be overwritten and have no effect. 
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   source              - enum for PM&TCM insertion source
*                         See odu_tfrm_oh_ins_src_t. ODU_TFRM_OH_INS_RCP is
*                         invalid as PM&TCM insertion source.
*   force_val           - For 'source' == ODU_TFRM_OH_INS_REGISTER (otherwise set to 0) \n
*                         The value to be inserted into PM&TCM field of
*                         PM or TCMi layer \n
*                         Valid range: 0 -- 1
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcm_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                         UINT32 chnl_id, 
                                         odu_tfrm_oh_ins_src_t source,
                                         UINT32 force_val)
{
    PMC_ENTRY();
    
    if (chnl_id > 95 || force_val > 1 
        || source > LAST_ODU_TFRM_OH_INS_SRC || source == ODU_TFRM_OH_INS_RCP)
    {
        PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
    }
    
    if (source == ODU_TFRM_OH_INS_REGISTER)
    {
        /* Configure PM_TCM_RES */
        odu_tfrm_field_PM_TCM_RES_set(NULL, odu_tfrm_handle, chnl_id, force_val);
        
        /* Set PM_TCM_RES_INS */
        odu_tfrm_field_PM_TCM_RES_INS_set(NULL, odu_tfrm_handle, chnl_id, 1);
    }
    else {
        /* Clear PM_TCM_RES_INS */
        odu_tfrm_field_PM_TCM_RES_INS_set(NULL, odu_tfrm_handle, chnl_id, 0);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_pm_tcm_src_cfg */

/*******************************************************************************
* odu_tfrm_pm_tcm_res_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retreives the value of PM&TCM reserved field of 
*   an ODU transmit data stream.
*
*   The value retreived with this API represents what was inserted using
*   register forcing.   
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*  *pm_tcm_res_val      - retreived PM&TCM field
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcm_res_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                         UINT32 chnl_id, UINT8 *pm_tcm_res_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    *pm_tcm_res_val = odu_tfrm_field_PM_TCM_RES_get(NULL, odu_tfrm_handle, chnl_id);
    
    PMC_RETURN(result);
} /* odu_tfrm_pm_tcm_res_get */

/*******************************************************************************
* odu_tfrm_pm_tcm_res_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the source of PM&TCM reserved field of an
*   ODU transmit data stream.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*  *pm_tcm_res_src      - extracted insertion source of PM&TCM field
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcm_res_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                             UINT32 chnl_id, UINT8 *pm_tcm_res_src)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    *pm_tcm_res_src = odu_tfrm_field_PM_TCM_RES_INS_get(NULL, odu_tfrm_handle, chnl_id);
    
    PMC_RETURN(result);
} /* odu_tfrm_pm_tcm_res_src_get */

/*******************************************************************************
* odu_tfrm_tcm_act_src_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the source and the value of TCM_ACT field of an ODU 
*   transmit data stream.
*
*   When configured for 'Incoming TCM_ACT value' the value transferred on
*   TCM_ACT byte will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert TCM_ACT bytes, the value 
*   inserted from register using this API will be overwritten and have no effect. 
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   tcm_act_src         - enum for TCM_ACT insertion source
*                         See odu_tfrm_oh_ins_src_t. ODU_TFRM_OH_INS_RCP is
*                         invalid as TCM_ACT insertion source.
*   force_val           - For source == ODU_TFRM_OH_INS_REGISTER (otherwise set to 0) \n
*                         The value to be inserted into TCM_ACT field of
*                         PM or TCMi layer \n
*                         Valid range: 0 - 0xFF
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_tcm_act_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                          UINT32 chnl_id, 
                                          odu_tfrm_oh_ins_src_t tcm_act_src,
                                          UINT32 force_val)
{
    PMC_ENTRY();
    
    PMC_ASSERT(odu_tfrm_handle != NULL, ODU_TFRM_ERR_INVALID_PTR, 0, 0);

    if (chnl_id > 95 || force_val > 0xFF 
        || tcm_act_src > LAST_ODU_TFRM_OH_INS_SRC || tcm_act_src == ODU_TFRM_OH_INS_RCP)
    {
        PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
    }
    
    if (tcm_act_src == ODU_TFRM_OH_INS_REGISTER)
    {
        /* Configure TCM_ACT */
        odu_tfrm_field_TCM_ACT_set(NULL, odu_tfrm_handle, chnl_id, force_val);
        
        /* Set TCM_ACT_INS */
        odu_tfrm_field_TCM_ACT_INS_set(NULL, odu_tfrm_handle, chnl_id, 1);        
    }
    else {
        /* Clear TCM_ACT_INS */
        odu_tfrm_field_TCM_ACT_INS_set(NULL, odu_tfrm_handle, chnl_id, 0);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_tcm_act_src_cfg */

/*******************************************************************************
* odu_tfrm_tcm_act_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retreives TCM_ACT value of an ODU transmit data stream
*   from register.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*  *tcm_act_val         - retreived TCM_ACT field
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_tcm_act_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                      UINT32 chnl_id, UINT8 *tcm_act_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    *tcm_act_val = odu_tfrm_field_TCM_ACT_get(NULL, odu_tfrm_handle, chnl_id);
    
    PMC_RETURN(result);
} /* odu_tfrm_tcm_act_get */

/*******************************************************************************
* odu_tfrm_tcm_act_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the insertion source of TCM_ACT overhead field.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*  *tcm_act_src         - extracted TCM_ACT insertion source
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_tcm_act_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                          UINT32 chnl_id, UINT8 *tcm_act_src)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    *tcm_act_src = odu_tfrm_field_TCM_ACT_INS_get(NULL, odu_tfrm_handle, chnl_id);
    
    PMC_RETURN(result);
} /* odu_tfrm_tcm_act_get */

/*******************************************************************************
* odu_tfrm_res1_src_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the source of row 2 RES1 overhead field of an ODU
*   transmit data stream.
*
*   When configured for 'Incoming RES1 value' the value transferred on
*   RES1 byte will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert RES1 bytes, the value 
*   inserted from register using this API will be overwritten and have no effect. 
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   res1_source         - enum for RES1 insertion source
*                         See odu_tfrm_oh_ins_src_t. ODU_TFRM_OH_INS_RCP is
*                         invalid as RES1 insertion source.
*   res1_val            - For 'res1_source' = ODU_TFRM_OH_INS_REGISTER. 
*                         For all other sources, set to '0'.
*                         The value to be inserted into RES1 field \n
*                         Valid range: 0x0 - 0xFFFF
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_res1_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, 
                                       odu_tfrm_oh_ins_src_t res1_source,
                                       UINT16 res1_val)
{
    PMC_ENTRY();
    
    if (res1_source > LAST_ODU_TFRM_OH_INS_SRC || res1_source == ODU_TFRM_OH_INS_RCP)
    {
        PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
    }
    
    if (res1_source == ODU_TFRM_OH_INS_REGISTER)
    {
        /* Configure RES1 value */
        odu_tfrm_field_RES1_set(NULL, odu_tfrm_handle, chnl_id, res1_val);
        
        /* Set RES1_INS */
        odu_tfrm_field_RES1_INS_set(NULL, odu_tfrm_handle, chnl_id, 1);   
    }
    else {
        /* Clear RES1_INS */
        odu_tfrm_field_RES1_INS_set(NULL, odu_tfrm_handle, chnl_id, 0);
    }
        
    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_res1_src_cfg */

/*******************************************************************************
* odu_tfrm_res1_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the inserted row 2 RES1 overhead field from an ODU
*   transmit data stream.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   res1_val            - extracted RES1 value
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_res1_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                   UINT32 chnl_id, UINT16 *res1_val)
{
    PMC_ENTRY();

    *res1_val = odu_tfrm_field_RES1_get(NULL, odu_tfrm_handle, chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_res1_get */

/*******************************************************************************
* odu_tfrm_res1_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the insertion source of row 2 RES1 overhead field.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *res1_src           - extracted RES1 insertion source
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_res1_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT8 *res1_src)
{
    PMC_ENTRY();

    *res1_src = odu_tfrm_field_RES1_INS_get(NULL, odu_tfrm_handle, chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_res1_src_get */


/*******************************************************************************
* odu_tfrm_res2_src_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the source of row 4 RES2 overhead field of an ODU
*   transmit data stream.
*
*   When configured for 'Incoming RES2 value' the value transferred on
*   RES2 byte will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert RES2 bytes, the value 
*   inserted from register using this API will be overwritten and have no effect.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   res2_source         - Enum for RES2 insertion source. See odu_tfrm_oh_ins_src_t.
*                         ODU_TFRM_OH_INS_RCP cannot be used as RES2 insertion source.
*   res2_val            - For 'res2_source' = ODU_TFRM_OH_INS_REGISTER only.
*                         For all other sources, set to '0'.
*                         The value to be inserted into Row 4 Col 9 to 14 of 
*                         RES byte. \n 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_res2_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, 
                                       odu_tfrm_oh_ins_src_t res2_source,
                                       UINT8 res2_val[6])
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 res2;
    UINT32 res3;
     
    PMC_ENTRY();
    
    if (res2_source > LAST_ODU_TFRM_OH_INS_SRC || res2_source == ODU_TFRM_OH_INS_RCP)
    {
        PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
    }

    /* set the insertion source */
    if (res2_source == ODU_TFRM_OH_INS_REGISTER)
    {
        res2 = res2_val[1] + (res2_val[0] << 8);
        res3 = res2_val[5] + (res2_val[4] << 8) + (res2_val[3] << 16) + (res2_val[2] << 24);
     
        /* Configure RES2 value */
        odu_tfrm_field_RES2_set(NULL, odu_tfrm_handle, chnl_id, res2);
        odu_tfrm_field_RES3_set(NULL, odu_tfrm_handle, chnl_id, res3);
        
        /* Set RES2_INS */
        odu_tfrm_field_RES2_INS_set(NULL, odu_tfrm_handle, chnl_id, 1);
    }
    else {
        /* Clear RES2_INS */
        odu_tfrm_field_RES2_INS_set(NULL, odu_tfrm_handle, chnl_id, 0);      
    }
    
    PMC_RETURN(result);
} /* odu_tfrm_res2_src_cfg */


/*******************************************************************************
* odu_tfrm_res2_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the inserted row 4 RES overhead field from an ODU
*   transmit data stream.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   res2_val            - extracted RES2 value
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_res2_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                   UINT32 chnl_id, UINT8 *res2_val)
{
    /* col 11- 14 */
    UINT32 res3;
    /* col 9-10 */
    UINT16 res2;
    UINT32 i;
    
    PMC_ENTRY();

    res2 = odu_tfrm_field_RES2_get(NULL, odu_tfrm_handle, chnl_id); 
    res3 = odu_tfrm_field_RES3_get(NULL, odu_tfrm_handle, chnl_id);
  
    res2_val[1] = res2 & 0xFF;
    res2 = res2 >> 8;
    res2_val[0] = res2 & 0xFF;
   
    for (i = 5; i > 1; i--)
    {
        res2_val[i] = res3 & 0xFF;
        res3 = res3 >> 8;
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_res2_get */

/*******************************************************************************
* odu_tfrm_res2_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the insertion source of row 4 RES overhead field.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *res2_src           - extracted RES2 insertion source
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_res2_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT8 *res2_src)
{
    PMC_ENTRY();

    *res2_src = odu_tfrm_field_RES2_INS_get(NULL, odu_tfrm_handle, chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_res2_src_get */

/*******************************************************************************
* odu_tfrm_exp_src_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the source of row 4 EXP overhead field of an ODU
*   transmit data stream. 
*
*   When configured for 'Incoming EXP value' the value transferred on
*   EXP byte will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert RES2 bytes, the value 
*   inserted from register using this API will be overwritten and have no effect.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   exp_source          - Enum, EXP source. See odu_tfrm_oh_ins_src_t. \n
*                         Note: ODU_TFRM_OH_INS_RCP is an invalid source for 
*                               EXP insertion. \n
*   exp_val             - For source == 1 (otherwise set to 0) \n
*                         The value to be inserted into EXP bytes \n
*                         Valid range: 0x0 - 0xFFFF
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_exp_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                      UINT32 chnl_id, 
                                      odu_tfrm_oh_ins_src_t exp_source,
                                      UINT32 exp_val)
{
    PMC_ENTRY();
    
    if (exp_source > LAST_ODU_TFRM_OH_INS_SRC || exp_source == ODU_TFRM_OH_INS_RCP)
    {
        PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
    }
    
    if (exp_source == ODU_TFRM_OH_INS_REGISTER)
    {
        odu_tfrm_field_EXP_set(NULL, odu_tfrm_handle, chnl_id, exp_val);
        
        odu_tfrm_field_EXP_INS_set(NULL, odu_tfrm_handle, chnl_id, 1);
    }
    else {
        odu_tfrm_field_EXP_INS_set(NULL, odu_tfrm_handle, chnl_id, 0);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_exp_src_cfg */

/*******************************************************************************
* odu_tfrm_exp_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the source of insertion for EXP field.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *exp_src            - extracted EXP insertion source
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_exp_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                      UINT32 chnl_id, UINT8 *exp_src)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    *exp_src = odu_tfrm_field_EXP_INS_get(NULL, odu_tfrm_handle, chnl_id);
    
    PMC_RETURN(result);
} /* odu_tfrm_exp_src_get */

/*******************************************************************************
* odu_tfrm_exp_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the value of EXP field.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *exp_val            - extracted EXP value
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_exp_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                  UINT32 chnl_id, UINT16 *exp_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    *exp_val = odu_tfrm_field_EXP_get(NULL, odu_tfrm_handle, chnl_id);
    
    PMC_RETURN(result);
} /* odu_tfrm_exp_get */

/*******************************************************************************
* odu_tfrm_gcc1_src_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the source of row 4 GCC1 overhead field of an ODU
*   transmit data stream.
*
*   When configured for 'Incoming GCC1 value' the value transferred on
*   GCC1 byte will be from highest to lowest priority: \n
*       - Data from the External Overhead Port (if enabled) \n
*       - Register forcing \n
*       - Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert GCC1 bytes, the value 
*   inserted from register using this API will be overwritten and have no effect. 
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   gcc1_source         - Enum for GCC1 source. See odu_tfrm_oh_ins_src_t.
*                         ODU_TFRM_OH_INS_RCP is invalid for GCC1 insertion.
*   gcc1_val            - For 'gcc1_source' == ODU_TFRM_OH_INS_REGISTER 
*                         The value to be inserted into row 4 GCC1 field. \n
*                         Valid range: 0x0 - 0xFFFFFFFF
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_gcc1_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, 
                                       odu_tfrm_oh_ins_src_t gcc1_source,
                                       UINT16 gcc1_val)
{
    PMC_ENTRY();

    if (gcc1_source > LAST_ODU_TFRM_OH_INS_SRC || gcc1_source == ODU_TFRM_OH_INS_RCP)
    {
        PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
    }
    
    if (gcc1_source == ODU_TFRM_OH_INS_REGISTER)
    {
        /* Configure GCC1 */
        odu_tfrm_field_GCC1_set(NULL, odu_tfrm_handle, chnl_id, gcc1_val);
        
        /* Set GCC1_INS */
        odu_tfrm_field_GCC1_INS_set(NULL, odu_tfrm_handle, chnl_id, 1);
    }
    else {
        /* Clear GCC1_INS */
        odu_tfrm_field_GCC1_INS_set(NULL, odu_tfrm_handle, chnl_id, 0);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_gcc1_src_cfg */

/*******************************************************************************
* odu_tfrm_gcc1_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the insertion source of GCC1.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *gcc1_src           - extracted insertion source of GCC1
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_gcc1_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT8 *gcc1_src)
{
    PMC_ENTRY();

    *gcc1_src = odu_tfrm_field_GCC1_INS_get(NULL, odu_tfrm_handle, chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_gcc1_src_get */

/*******************************************************************************
* odu_tfrm_gcc1_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the value of GCC1.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *gcc1_val           - extracted the value of GCC1
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_gcc1_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                   UINT32 chnl_id, UINT16 *gcc1_val)
{
    PMC_ENTRY();

    *gcc1_val = odu_tfrm_field_GCC1_get(NULL, odu_tfrm_handle, chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_gcc1_get */


/*******************************************************************************
* odu_tfrm_gcc2_src_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the source of row 4 GCC2 overhead field of an ODU
*   transmit data stream.
*
*   When configured for 'Incoming GCC2 value' the value transferred on
*   GCC2 byte will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*   If external overhead port is enabled to insert GCC2 bytes, the value 
*   inserted from register using this API will be overwritten and have no effect. 
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   gcc2_source         - Enum for gcc2_source. See odu_tfrm_oh_ins_src_t.
*                         ODU_TFRM_OH_INS_RCP is invalid for GCC2 insertion.
*   gcc2_val            - The value to be inserted into row 4 GCC2 field. \n
*                         Valid range: 0x0 - 0xFFFFFFFF
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_gcc2_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, 
                                       odu_tfrm_oh_ins_src_t gcc2_source,
                                       UINT16 gcc2_val)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    if (gcc2_source > LAST_ODU_TFRM_OH_INS_SRC || gcc2_source == ODU_TFRM_OH_INS_RCP)
    {
        PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
    }

    if (gcc2_source == ODU_TFRM_OH_INS_REGISTER)
    {
        /* Configure GCC2 */
        odu_tfrm_field_GCC2_set(NULL, odu_tfrm_handle, chnl_id, gcc2_val);
        
        /* Set GCC2_INS */
        odu_tfrm_field_GCC2_INS_set(NULL, odu_tfrm_handle, chnl_id, 1);
    }
    else {
        /* Clear GCC2_INS */
        odu_tfrm_field_GCC2_INS_set(NULL, odu_tfrm_handle, chnl_id, 0);
    }    

    PMC_RETURN(result);
} /* odu_tfrm_gcc2_src_cfg */

/*******************************************************************************
* odu_tfrm_gcc2_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the insertion source of GCC2.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *gcc2_src           - extracted insertion source of GCC2
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_gcc2_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT8 *gcc2_src)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    *gcc2_src = odu_tfrm_field_GCC2_INS_get(NULL, odu_tfrm_handle, chnl_id);

    PMC_RETURN(result);
} /* odu_tfrm_gcc2_src_get */

/*******************************************************************************
* odu_tfrm_gcc2_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the value of GCC2.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *gcc2_val           - extracted the value of GCC2
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_gcc2_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                   UINT32 chnl_id, UINT16 *gcc2_val)
{
    PMC_ENTRY();

    *gcc2_val = odu_tfrm_field_GCC2_get(NULL, odu_tfrm_handle, chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_gcc2_get */

/*******************************************************************************
* odu_tfrm_aps_pcc_src_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the source of APS/PCC overhead field of PM or TCMi
*   connection of an ODU transmit data stream.
*
*   When configured for 'Incoming APS/PCC value' the value transferred on
*   APS/PCC byte will be from highest to lowest priority: \n
*       -- Data from the External Overhead Port (if enabled) \n
*       -- Register forcing \n
*       -- Data from incoming ODUk frame
*
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   mon_level           - PM or TCMi layer number \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6 \n
*                         7      -- ODUk server layer trail
*   source              - Enum, APS/PCC source. See odu_tfrm_oh_ins_src_t.
*                         
*   aps_pcc_val         - For source == 1 (otherwise set to 0) \n
*                         The value to be inserted into APS/PCC field. \n
*                         Valid range: 0x0 - 0xFFFFFFFF
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_aps_pcc_src_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                          UINT32 chnl_id, 
                                          UINT8 mon_level,
                                          odu_tfrm_oh_ins_src_t source,
                                          UINT32 aps_pcc_val)
{
    /* variable declaration */
    UINT32 reg_offset;
    
    PMC_ENTRY();
    
    if (source > LAST_ODU_TFRM_OH_INS_SRC || source == ODU_TFRM_OH_INS_RCP)
    {
        PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
    }

    if (source == ODU_TFRM_OH_INS_REGISTER)
    {
        reg_offset = (chnl_id * 8) + mon_level;

        odu_tfrm_reg_APS_PCC_array_field_set(NULL, odu_tfrm_handle, reg_offset, 0xFFFFFFFF, 0, aps_pcc_val);
        odu_tfrm_field_range_APS_PCC_INS_set(NULL, odu_tfrm_handle, chnl_id, mon_level, mon_level, 1);
    }
    else {
        odu_tfrm_field_range_APS_PCC_INS_set(NULL, odu_tfrm_handle, chnl_id, mon_level, mon_level, 0);
    }        

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_aps_pcc_src_cfg */

/*******************************************************************************
* odu_tfrm_aps_pcc_src_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the insertion source of APS/PCC overhead field of 
*   an ODU transmit data stream.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   mon_level           - connection monitoring level
*                         0 -- ODUk Path
*                         1 -- 6: TCM1 - 6
*                         7 -- ODUk server layer trail
*
* OUTPUTS:
*   *aps_pcc_src        - extracted insertion source of APS/PCC
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_aps_pcc_src_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                          UINT32 chnl_id, 
                                          UINT32 mon_level,
                                          UINT8 *aps_pcc_src)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    if (chnl_id > 95 || mon_level > 7)
    {
        PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
    }
    
    *aps_pcc_src = odu_tfrm_field_range_APS_PCC_INS_get(NULL, odu_tfrm_handle, chnl_id, mon_level, mon_level);
    
    PMC_RETURN(result);
} /* odu_tfrm_aps_pcc_src_get */

/*******************************************************************************
* odu_tfrm_aps_pcc_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the value of APS/PCC overhead field of 
*   an ODU transmit data stream.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   mon_level           - connection monitoring level
*                         0 -- ODUk Path
*                         1 -- 6: TCM1 - 6
*                         7 -- ODUk server layer trail
*
* OUTPUTS:
*   *aps_pcc_val        - extracted value of APS/PCC
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_aps_pcc_ext(odu_tfrm_handle_t *odu_tfrm_handle,
                                      UINT32 chnl_id, 
                                      UINT32 mon_level,
                                      UINT32 *aps_pcc_val)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 reg_offset; 

    PMC_ENTRY();
    
    if (chnl_id > 95 || mon_level > 7)
    {
        PMC_RETURN(ODU_TFRM_LOG_ERR_INVALID_PARAMETERS);
    }
    
    reg_offset = (chnl_id * 8) + mon_level;  
       
    *aps_pcc_val = odu_tfrm_reg_APS_PCC_array_read(NULL, odu_tfrm_handle, reg_offset);
    
    PMC_RETURN(result);
} /* odu_tfrm_aps_pcc_ext */


/*******************************************************************************
* odu_tfrm_odukp_mux_maint_sig_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This functions controls the forced insertion of ODUk maintenance signal
*   at the ODUkP/ODU[i][j] Mux adaptation layer (over all PM/TCM layers).
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   maint_sig           - ODUk maintenance signal control \n
*                         0 -- no maintenance signal forced \n
*                         1 -- Force ODUK_AIS \n
*                         2 -- Force ODUK_LCK \n
*                         3 -- Force ODUK_OCI
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_odukp_mux_maint_sig_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                                  UINT32 chnl_id, 
                                              UINT32 maint_sig)
{

    PMC_ERROR result;
    /* variable declartion */
    PMC_ENTRY();

    
    odu_tfrm_handle->var.odukp_mux_maint_sig[chnl_id] = maint_sig;
 
    result = odu_tfrm_gen_and_activate_tcm_pm_layers(odu_tfrm_handle,
                                                     chnl_id);


    PMC_RETURN(result);
} /* odu_tfrm_odukp_mux_maint_sig_cfg */


/*******************************************************************************
* odu_tfrm_odukp_pck_maint_sig_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This functions controls the forced insertion of ODUk maintenance signal
*   at the  ODUkP/Pck packet adaptation (PM layer only, no TCMS)
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   maint_sig           - ODUk maintenance signal control \n
*                         0 -- no maintenance signal forced \n
*                         1 -- Force ODUK_AIS \n
*                         2 -- Force ODUK_LCK \n
*                         3 -- Force ODUK_OCI
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_odukp_pck_maint_sig_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                                  UINT32 chnl_id, 
                                              UINT32 maint_sig)
{

    PMC_ERROR result;
    /* variable declartion */
    PMC_ENTRY();

    
    odu_tfrm_handle->var.odukp_pck_maint_sig[chnl_id] = maint_sig;

    result = odu_tfrm_gen_and_activate_tcm_pm_layers(odu_tfrm_handle,
                                                     chnl_id);


    PMC_RETURN(result);
} /* odu_tfrm_odukp_mux_maint_sig_cfg */


/*******************************************************************************
* odu_tfrm_odukp_pck_maint_sig_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the current maintenance signal configuration status 
*   at the ODUkP/Pck packet adapataion for the channel.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95

*
* OUTPUTS: 
*   *maint_sig          - ODUk maintenance signal control \n
*                         0 -- no maintenance signal forced \n
*                         1 -- Force ODUK_AIS \n
*                         2 -- Force ODUK_LCK \n
*                         3 -- Force ODUK_OCI 
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odu_tfrm_odukp_pck_maint_sig_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                             UINT32 chnl_id, 
                                             UINT32 *maint_sig)
{

    /* variable declartion */
    PMC_ENTRY();
    
    *maint_sig = odu_tfrm_handle->var.odukp_pck_maint_sig[chnl_id];

    PMC_RETURN();
} /* odu_tfrm_odukp_mux_maint_sig_cfg */


/*******************************************************************************
* odu_tfrm_odukt_maint_sig_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This functions controls the forced insertion of ODUk maintenance signal
*   at the ODUkt adaptation layer (forced at a TCM, but before this TCM's
*   overhead processing).
*
*   Only valid to force or removed LCK.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   tcm_num             - The TCM type
*   maint_sig           - ODUk maintenance signal control \n
*                         0 -- no maintenance signal forced \n
*                         3 -- Force ODUK_OCI
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_odukt_maint_sig_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                              UINT32 chnl_id, 
                                              odu_tfrm_pm_tcm_id_t tcm_num,
                                              UINT32 maint_sig)
{

    PMC_ERROR result;
    UINT16 tcm_pool_id;

    /* variable declartion */
    PMC_ENTRY();


    result = odu_tfrm_pool_id_return(odu_tfrm_handle,
                                     chnl_id,
                                     tcm_num,
                                     &tcm_pool_id);
    
    if(result == PMC_SUCCESS)
    {
        odu_tfrm_handle->var.tcm_pool_ptr[tcm_pool_id].odukt_maint_sig = maint_sig;

        result = odu_tfrm_gen_and_activate_tcm_pm_layers(odu_tfrm_handle,
                                                         chnl_id);
    }


    PMC_RETURN(result);
} /* odu_tfrm_odukt_maint_sig_cfg */


/*******************************************************************************
* odu_tfrm_chnl_maint_sig_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This functions controls the forced insertion of ODUk maintenance signal.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *maint_sig          - extracted maintenance signal being inserted on the
*                         channel
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_chnl_maint_sig_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                             UINT32 chnl_id, UINT8 *maint_sig)
{
    /* variable declartion */
    PMC_ENTRY();

    /* set MAINT_SIG bit to desired value */
    *maint_sig = odu_tfrm_field_MAINT_SIG_get(NULL, odu_tfrm_handle, chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_chnl_maint_sig_get */


/*******************************************************************************
* odu_tfrm_odukt_maint_sig_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the forced insertion of ODUk maintenance signal
*   at the ODUkt adaptation layer (forced at a TCM, but before this TCM's
*   overhead processing).
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   tcm_num             - The TCM type
*
* OUTPUTS:
*   *maint_sig          - ODUk maintenance signal control \n
*                         0 -- no maintenance signal forced \n
*                         1 -- Force ODUK_AIS \n
*                         2 -- Force ODUK_LCK \n
*                         3 -- Force ODUK_OCI
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_odukt_maint_sig_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                              UINT32 chnl_id, 
                                              odu_tfrm_pm_tcm_id_t tcm_num,
                                              UINT8 *maint_sig)
{

    PMC_ERROR result;
    UINT16 tcm_pool_id;

    /* variable declartion */
    PMC_ENTRY();


    result = odu_tfrm_pool_id_return(odu_tfrm_handle,
                                     chnl_id,
                                     tcm_num,
                                     &tcm_pool_id);
    
    if(result == PMC_SUCCESS)
    {
        *maint_sig = odu_tfrm_handle->var.tcm_pool_ptr[tcm_pool_id].odukt_maint_sig;
    }


    PMC_RETURN(result);
} /* odu_tfrm_odukt_maint_sig_get */


/*******************************************************************************
* odu_tfrm_chnl_oh_ins_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This functions disables the optional overhead field insertion while forced 
*   insertion of ODUk maintenance signal on the channel.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_chnl_oh_ins_disable(odu_tfrm_handle_t *odu_tfrm_handle,
                                             UINT32 chnl_id)
{
    /* variable declartion */
    PMC_ERROR result = PMC_SUCCESS;
    PMC_ENTRY();

    /* set MAINT_SIG bit to desired value */
    odu_tfrm_field_RES1_INS_set(NULL, odu_tfrm_handle, chnl_id, 0);
    odu_tfrm_field_RES2_INS_set(NULL, odu_tfrm_handle, chnl_id, 0);
    odu_tfrm_field_PM_TCM_RES_INS_set(NULL, odu_tfrm_handle, chnl_id, 0);
    odu_tfrm_field_TCM_ACT_INS_set(NULL, odu_tfrm_handle, chnl_id, 0);
    odu_tfrm_field_FTFL_INS_set(NULL, odu_tfrm_handle, chnl_id, 0);

    PMC_RETURN(result);
} /* odu_tfrm_chnl_oh_ins_disable */

/*******************************************************************************
* odu_tfrm_chnl_oh_ins_ctxt_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function stores the current overhead insertion controls in the device
*   prior to inserting a maintenance signal over an ODU channel.
*   This context will be used to restore the overhead insertion controls after
*   the maintenance signal is removed from the channel.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_chnl_oh_ins_ctxt_set(odu_tfrm_handle_t *odu_tfrm_handle,
                                               UINT32 chnl_id)
{
    /* variable declartion */
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 ftfl_ins;
    UINT8 res1_ins;
    UINT8 res2_ins;
    UINT8 tcm_act_ins;
    UINT8 pm_tcm_ins;
    UINT8 oh_ins_ctxt;
    
    PMC_ENTRY();
    
    /*
                       Bit 0-1: FTFL_INS
*                      Bit   2: TCM_ACT_INS
*                      Bit   3: PM_TCM_RES_INS
*                      Bit   4: RES1_INS
*                      Bit   5: RES2_INS */

    /* set MAINT_SIG bit to desired value */
    ftfl_ins = odu_tfrm_field_FTFL_INS_get(NULL, odu_tfrm_handle, chnl_id);
    res1_ins = odu_tfrm_field_RES1_INS_get(NULL, odu_tfrm_handle, chnl_id);
    res2_ins = odu_tfrm_field_RES2_INS_get(NULL, odu_tfrm_handle, chnl_id);
    tcm_act_ins = odu_tfrm_field_TCM_ACT_INS_get(NULL, odu_tfrm_handle, chnl_id);
    pm_tcm_ins = odu_tfrm_field_PM_TCM_RES_INS_get(NULL, odu_tfrm_handle, chnl_id);
    
    oh_ins_ctxt = (ftfl_ins & 0x3) + ((tcm_act_ins & 0x1) << 2) + ((pm_tcm_ins & 0x1) << 3)
                  + ((res1_ins & 0x1) << 4) + ((res2_ins & 0x1) << 5); 
    
    odu_tfrm_handle->var.optional_oh_ctxt[chnl_id] = oh_ins_ctxt;

    PMC_RETURN(result);
} /* odu_tfrm_chnl_oh_ins_ctxt_set */

/*******************************************************************************
* odu_tfrm_chnl_oh_ins_ctxt_restore
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function stores the current overhead insertion controls in the device
*   prior to inserting a maintenance signal over an ODU channel.
*   This context will be used to restore the overhead insertion controls after
*   the maintenance signal is removed from the channel.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_chnl_oh_ins_ctxt_restore(odu_tfrm_handle_t *odu_tfrm_handle,
                                                   UINT32 chnl_id)
{
    /* variable declartion */
    PMC_ERROR result = PMC_SUCCESS;
    UINT8 oh_ins_ctxt;
    
    PMC_ENTRY();
    
    /*
                       Bit 0-1: FTFL_INS
*                      Bit   2: TCM_ACT_INS
*                      Bit   3: PM_TCM_RES_INS
*                      Bit   4: RES1_INS
*                      Bit   5: RES2_INS */

    oh_ins_ctxt = odu_tfrm_handle->var.optional_oh_ctxt[chnl_id];
    
    /* set MAINT_SIG bit to desired value */
    odu_tfrm_field_RES2_INS_set(NULL, odu_tfrm_handle, chnl_id, ((oh_ins_ctxt >> 5) & 0x1));
    odu_tfrm_field_RES1_INS_set(NULL, odu_tfrm_handle, chnl_id, ((oh_ins_ctxt >> 4) & 0x1));
    odu_tfrm_field_PM_TCM_RES_INS_set(NULL, odu_tfrm_handle, chnl_id, ((oh_ins_ctxt >> 3) & 0x1));
    odu_tfrm_field_TCM_ACT_INS_set(NULL, odu_tfrm_handle, chnl_id, ((oh_ins_ctxt >> 2) & 0x1));
    odu_tfrm_field_FTFL_INS_set(NULL, odu_tfrm_handle, chnl_id, (oh_ins_ctxt & 0x3));

    PMC_RETURN(result);
} /* odu_tfrm_chnl_oh_ins_ctxt_restore */


/*******************************************************************************
*  odu_tfrm_pm_prov_state_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the provisioned state of the PM layer
*   
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - channel ID to request TCM layer information on
*                         Valid range: 0 - 95
*
* OUTPUTS:
*
* RETURNS:
*    TRUE - PM layer is provisioned
*    FALSE - PM layer is not provisioned
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 odu_tfrm_pm_prov_state_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id)
{
    BOOL8 result = TRUE;
    PMC_ENTRY();
    if(odu_tfrm_handle->var.pm_ctxt_ptr[chnl_id].assigned_state == FALSE)
    {
        result = FALSE;
    }
    PMC_RETURN(result);
} /* odu_tfrm_pm_prov_state_get  */


/*******************************************************************************
* odu_tfrm_sf_sd_proc_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures how signal fail and signal degrade status is
*   processed through ODU_TFRM.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   sf_sd_proc          - Signal Fail/Signal Degrade processing mode \n
*                         0 -- SF/SD status is terminated \n
*                         1 -- SF/SD status is propagated transparently
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_sf_sd_proc_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                        UINT32 chnl_id, UINT32 sf_sd_proc)
{
    /* variable declartion */
    PMC_ENTRY();

    /* set SF_SD_PROP bit to desired value */
    odu_tfrm_field_SF_SD_PROP_set(NULL, odu_tfrm_handle, chnl_id, sf_sd_proc);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_sf_sd_proc_cfg */

/*******************************************************************************
* odu_tfrm_ssf_proc_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the processing mode of server signal fail status
*   through ODU_TFRM.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   ssf_proc            - Server Signal processing mode \n
*                         0 -- SSF status is terminated \n
*                         1 -- SSF status is propagated transparently
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_ssf_proc_cfg(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT32 ssf_proc)
{
    /* variable declartion */
    PMC_ENTRY();

    /* set SSF_PROP bit to desired value */
    odu_tfrm_field_SSF_PROP_set(NULL, odu_tfrm_handle, chnl_id, ssf_proc);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_ssf_proc_cfg */


/*******************************************************************************
* odu_tfrm_pm_tcmi_delay_measure_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes or unprovisions delay measurement source on the
*   provisioned PM or TCMi connection. This function clears DM_STATE and
*   DM_COUNT and resets delay measurement state machine when it is initialized.
*   This function should be called once before performing multiple measurements.
*
*   When this function is called with 'enbl' = 0, it disables the delay
*   measurement state machine.
*
*   After calling this API, call odu_tfrm_pm_tcmi_delay_measure_get_count() API
*   to collect the counter value.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi layer on which delay measurement will be
*                         performed on \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*   enbl                - enables or disables the delay measurement state
*                         machine \n
*                         0 -- disable \n
*                         1 -- enable
*   dm_val              - if dm_val = 1, this is the initial value to force in
*                         dmp_val/dmt_val
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_delay_measure_init(odu_tfrm_handle_t *odu_tfrm_handle,
                                                     UINT32 chnl_id,
                                                     odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                                     UINT32 enbl,
                                                     UINT32 dm_val)
{
    /* variable declaration */
    PMC_ERROR ret_code;
    UINT16 tcm_pool_id;

    PMC_ENTRY();

    /* argument checking */

    /* find pool_id */
    if (pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
   {
        ret_code = odu_tfrm_pool_id_return(odu_tfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);
        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }

        if (tcm_pool_id > odu_tfrm_handle->cfg.tcm_pool_size)
        {
            PMC_LOG_TRACE("%s, %s, %d, POOL ID unassigned: pool_id = %u\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);
            PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_ID_UNASSIGNED);
        }

        /* disable DM state machine */
        odu_tfrm_field_DMT_SRC_set(NULL, odu_tfrm_handle, tcm_pool_id, 0);
        if (enbl)
        {
            odu_tfrm_field_DMT_VAL_set(NULL, odu_tfrm_handle, tcm_pool_id, dm_val);

            /* initialize DM context */
            odu_tfrm_field_DM_COUNT_set(NULL, odu_tfrm_handle, tcm_pool_id, 0);
            odu_tfrm_field_DM_STATE_set(NULL, odu_tfrm_handle, tcm_pool_id, dm_val);

            /* enable DM state machine */
            odu_tfrm_field_DMT_SRC_set(NULL, odu_tfrm_handle, tcm_pool_id, 1);

            /* clear DM interrupt bit */
            odu_tfrm_lfield_range_DM_PID_I_set_to_clear(NULL, odu_tfrm_handle, tcm_pool_id, tcm_pool_id, 1);
        }

        /* clear DM interrupt bit */
        odu_tfrm_lfield_range_DM_PID_I_set_to_clear(NULL, odu_tfrm_handle, tcm_pool_id, tcm_pool_id, 1);
                
    }
    else
    {
        /* disable DM state machine */
        odu_tfrm_field_DMP_SRC_set(NULL, odu_tfrm_handle, chnl_id, 0);
        if (enbl)
        {
            odu_tfrm_field_DMP_VAL_set(NULL, odu_tfrm_handle, chnl_id, dm_val);

            /* initialize DM context */
            odu_tfrm_field_DM_COUNT_PM_set(NULL, odu_tfrm_handle, chnl_id, 0);
            odu_tfrm_field_DM_STATE_PM_set(NULL, odu_tfrm_handle, chnl_id, dm_val);

            /* enable DM state machine */
            odu_tfrm_field_DMP_SRC_set(NULL, odu_tfrm_handle, chnl_id, 1);
        }

        /* clear DM interrupt bit */
        odu_tfrm_lfield_range_DM_PM_I_set_to_clear (NULL, odu_tfrm_handle, chnl_id, chnl_id, 1);
           
    }
    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_pm_tcmi_delay_measure_init */

/*******************************************************************************
* odu_tfrm_pm_tcmi_delay_measure_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets delay measurement mode and its initial value.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi layer on which delay measurement will be
*                         performed on \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*
* OUTPUTS:
*   *enbl               - enabling of the delay measurement state
*   *dm_val             - the initial value to force in dmp_val/dmt_val
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_delay_measure_mode_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                                         UINT32 chnl_id,
                                                         odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                                         UINT32 *enbl,
                                                         UINT32 *dm_val)
{
    /* variable declaration */
    PMC_ERROR ret_code;
    UINT16 tcm_pool_id;

    PMC_ENTRY();

    if (pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
    {
        ret_code = odu_tfrm_pool_id_return(odu_tfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);
        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }

        if (tcm_pool_id > odu_tfrm_handle->cfg.tcm_pool_size)
        {
            PMC_LOG_TRACE("%s, %s, %d, POOL ID unassigned: pool_id = %u\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);
            PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_ID_UNASSIGNED);
        }

        *enbl = odu_tfrm_field_DMT_SRC_get(NULL, odu_tfrm_handle, tcm_pool_id);
        if(*enbl)
        {
            *dm_val = odu_tfrm_field_DMT_VAL_get(NULL, odu_tfrm_handle, tcm_pool_id);
        }
        else
        {
            *dm_val = 0;
        }
    }
    else
    {
        *enbl = odu_tfrm_field_DMP_SRC_get(NULL, odu_tfrm_handle, chnl_id);
        if(*enbl)
        {
            *dm_val = odu_tfrm_field_DMP_VAL_get(NULL, odu_tfrm_handle, chnl_id);
        }
        else
        {
            *dm_val = 0;
        }
    }
    
    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_pm_tcmi_delay_measure_mode_get */

/*******************************************************************************
* odu_tfrm_pm_tcmi_delay_measure_get_count
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the delay measurement result for PM or TCMi layer.
*
*   Delay measurement values are valid when a corresponding PM or TCMi layer is
*   provisioned. If reading the delay measurement count on the layer for the 
*   first time, odu_tfrm_pm_tcmi_delay_measure_init() must be called prior to
*   calling this API to ensure accurate measurements.   
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi layer on which delay measurement will be
*                         performed on \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*   block_mode          - blocking mode
*                         0 -- no-blocking
*                         1 -- blocking
*
* OUTPUTS:
*   *dm_count           - delay measurement result in ODUk frame periods
*                         When measure counter is not ready, *dm_count is set 
*                         to 0xFFFF_FFFF and it value should be ignored.
*                         If delay measurement is not started ahead, *dm_count
*                         is set to 0xFFFF_FFFE.
*                           
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_delay_measure_get_count(odu_tfrm_handle_t *odu_tfrm_handle,
                                                          UINT32 chnl_id,
                                                          odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                                          UINT32 block_mode,
                                                          UINT32 *dm_count)
{
    /* variable declaration */
    PMC_ERROR ret_code;
    PMC_POLL_RETURN_TYPE poll_rc;
    UINT32 num_failed_polls;
    UINT16 tcm_pool_id;
    UINT32 rc;
    UINT32 dm_val, dm_state;
    
    PMC_ENTRY();

    /* find pool_id */
    if (pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
    {
        ret_code = odu_tfrm_pool_id_return(odu_tfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);
        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }

        if (tcm_pool_id > odu_tfrm_handle->cfg.tcm_pool_size)
        {
            PMC_LOG_TRACE("%s, %s, %d, POOL ID unassigned: pool_id = %u\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);
            PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_ID_UNASSIGNED);
        }

        dm_val = odu_tfrm_field_DMT_VAL_get(NULL, odu_tfrm_handle, tcm_pool_id);
        dm_state = odu_tfrm_field_DM_STATE_get(NULL, odu_tfrm_handle, tcm_pool_id);

        if (dm_val == dm_state)
        {   /* measure complete or has not been started */
            rc = odu_tfrm_lfield_range_DM_PID_I_get(NULL, 
                                                      odu_tfrm_handle,
                                                      tcm_pool_id,
                                                      tcm_pool_id);
            if (rc == 0)
            {
                *dm_count = 0xFFFFFFFE; /* delay measure is not started. */
            }
            else
            {
                /* read counter value */
                *dm_count = odu_tfrm_field_DM_COUNT_get(NULL, odu_tfrm_handle, tcm_pool_id);
                
                /* clear DM interrupt bit */
                odu_tfrm_lfield_range_DM_PID_I_set_to_clear(NULL, odu_tfrm_handle, tcm_pool_id, tcm_pool_id, 1);
            }

            PMC_RETURN(PMC_SUCCESS);
        }

        /* dm_val != dm_state, means result not ready */
        if (!block_mode)
        {
            *dm_count = 0xFFFFFFFF; /* result not ready */
            
            PMC_RETURN(PMC_SUCCESS);
        }
        else
        {   /* blocking mode */
            poll_rc = odu_tfrm_lfield_range_DM_PID_I_poll(NULL,
                                                          odu_tfrm_handle,
                                                          tcm_pool_id,
                                                          tcm_pool_id,  
                                                          1,
                                                          PMC_POLL_EQUAL,
                                                          OTU_TFRM_POLL_ITERATIONS,
                                                          &num_failed_polls, 
                                                          OTU_TFRM_USEC_DELAY_TICKS);
            if (0 != poll_rc)
            {
                *dm_count = 0xFFFFFFFF;
            }
            else
            {
                /* read counter value */
                *dm_count = odu_tfrm_field_DM_COUNT_get(NULL, odu_tfrm_handle, tcm_pool_id);
                
                /* clear DM interrupt bit */
                odu_tfrm_lfield_range_DM_PID_I_set_to_clear(NULL, odu_tfrm_handle, tcm_pool_id, tcm_pool_id, 1);
            }
        }
    }
    else
    {
        dm_val = odu_tfrm_field_DMP_VAL_get(NULL, odu_tfrm_handle, chnl_id);
        dm_state = odu_tfrm_field_DM_STATE_PM_get(NULL, odu_tfrm_handle, chnl_id);
        if (dm_val == dm_state)
        {   /* measure complete or has not been started */
            rc = odu_tfrm_lfield_range_DM_PM_I_get(NULL, 
                                                      odu_tfrm_handle,
                                                      chnl_id,
                                                      chnl_id);

            if (rc == 0)
            {
                *dm_count = 0xFFFFFFFE; /* delay measure is not started. */
            }
            else 
            {
                /* read counter value */
                *dm_count = odu_tfrm_field_DM_COUNT_PM_get(NULL, odu_tfrm_handle, chnl_id);

                /* clear DM interrupt bit */
                odu_tfrm_lfield_range_DM_PM_I_set_to_clear (NULL, odu_tfrm_handle, chnl_id, chnl_id, 1);
            }

            PMC_RETURN(PMC_SUCCESS);
        }

        /* dm_val != dm_state, means result not ready */
        if (!block_mode)
        {
            *dm_count = 0xFFFFFFFF; /* result not ready */
            
            PMC_RETURN(PMC_SUCCESS);
        }
        else
        {
            poll_rc = odu_tfrm_lfield_range_DM_PM_I_poll(NULL,
                                                         odu_tfrm_handle,
                                                         chnl_id,
                                                         chnl_id,  
                                                         1,
                                                         PMC_POLL_EQUAL,
                                                         OTU_TFRM_POLL_ITERATIONS,
                                                         &num_failed_polls, 
                                                         OTU_TFRM_USEC_DELAY_TICKS);
            
            if (0 != poll_rc)
            {
                *dm_count = 0xFFFFFFFF;
            }
            else
            {
                /* read counter value */
                *dm_count = odu_tfrm_field_DM_COUNT_PM_get(NULL, odu_tfrm_handle, chnl_id);

                /* clear DM interrupt bit */
                odu_tfrm_lfield_range_DM_PM_I_set_to_clear (NULL, odu_tfrm_handle, chnl_id, chnl_id, 1);
            }
        }
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_pm_tcmi_delay_measure_get_count */

/*******************************************************************************
* odu_tfrm_pm_tcmi_delay_measure_go
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initiates the delay measurement.   
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - The PM/TCM to start the delay measurement for
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_pm_tcmi_delay_measure_go(odu_tfrm_handle_t *odu_tfrm_handle,
                                                   UINT32 chnl_id,
                                                   odu_tfrm_pm_tcm_id_t pm_tcm_num)
{
    /* variable declaration */
    PMC_ERROR ret_code = PMC_SUCCESS;

    UINT16 tcm_pool_id;
    UINT32 dm_val;
    UINT32 dm_inv;

    PMC_ENTRY();

    /* find pool_id */
    if (pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
    {
        ret_code = odu_tfrm_pool_id_return(odu_tfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);
        if (ret_code != PMC_SUCCESS)
        {
            PMC_RETURN(ret_code);
        }

        if (tcm_pool_id > odu_tfrm_handle->cfg.tcm_pool_size)
        {
            PMC_LOG_TRACE("%s, %s, %d, POOL ID unassigned: pool_id = %u\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);
            PMC_RETURN(ODU_TFRM_LOG_CODE_POOL_ID_UNASSIGNED);
        }

        /* start the delay measurement */
        dm_val = odu_tfrm_field_DMT_VAL_get(NULL, odu_tfrm_handle, tcm_pool_id);
        dm_inv = (~dm_val & 0x1);
        odu_tfrm_field_DMT_VAL_set(NULL, odu_tfrm_handle, tcm_pool_id, dm_inv & 0x1);
    
    }
    else
    {
        dm_val = odu_tfrm_field_DMP_VAL_get(NULL, odu_tfrm_handle, chnl_id);
        dm_inv = (~dm_val & 0x1);
        odu_tfrm_field_DMP_VAL_set(NULL, odu_tfrm_handle, chnl_id, dm_inv);

    }

    PMC_RETURN(ret_code);

} /* odu_tfrm_pm_tcmi_delay_measure_go */



/*******************************************************************************
* odu_tfrm_iae_stat_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the status of incoming alignment error in the 
*   corresponding channel.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *status             - pointer to storage for IAE status bits\n
*                         Bit 95 -- IAE status on channel 95 \n
*                         Bit 94 -- IAE status on channel 94 \n
*                         Bit 93 -- IAE status on channel 93 \n
*                               ... \n
*                         Bit 1  -- IAE status on channel 1 \n
*                         Bit 0  -- IAE status on channel 0
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_iae_stat_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                       UINT32 chnl_id, UINT32 *status)
{
    PMC_ENTRY();
    
    /*
    read IAE_V
    */
    *status = odu_tfrm_lfield_range_IAE_V_get(NULL, odu_tfrm_handle, chnl_id, chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_iae_stat_get */

/*
** Private Functions
*/
/*******************************************************************************
*  odu_tfrm_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initialize ODU_TFRM var_t variables. Elements in TCM POOL ID
*   context structure are also initialized in this function.
*
* INPUTS:
*   hndl     - handle to the ODU_TFRM instance to be operated on
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE void odu_tfrm_var_default_init(odu_tfrm_handle_t *hndl)
{
    /* variable declaration */
    UINT32 i;

    PMC_ENTRY();

    hndl->var.num_pool_free = hndl->cfg.tcm_pool_size;

    /* initialize TCM POOL ID context table */
    for (i = 0; i < hndl->cfg.tcm_pool_size; i++)
    {
        hndl->var.tcm_pool_ptr[i].assign_state  = 0;
        hndl->var.tcm_pool_ptr[i].chnl_id       = 0xFF;
        hndl->var.tcm_pool_ptr[i].tcm_num       = ODU_TFRM_MESSAGE_DONT_CARE;
        hndl->var.tcm_pool_ptr[i].child_tcm_num     = ODU_TFRM_MESSAGE_DONT_CARE;
        hndl->var.tcm_pool_ptr[i].child_pool_id     = ODU_TFRM_UNUSED_POOL_ID;
        hndl->var.tcm_pool_ptr[i].odukt_maint_sig = 0;
    }
    
    for (i = 0; i < hndl->cfg.num_chans; i++)
    {
        hndl->var.pm_ctxt_ptr[i].assigned_state = FALSE; 
        hndl->var.pm_ctxt_ptr[i].child_tcm_num = ODU_TFRM_MESSAGE_DONT_CARE;
        hndl->var.pm_ctxt_ptr[i].child_pool_id = ODU_TFRM_UNUSED_POOL_ID;

        hndl->var.dci_pm_tcm_child[i] = ODU_TFRM_MESSAGE_DONT_CARE;
    }
    
    for (i = 0; i < hndl->cfg.num_chans; i++)
    {
        hndl->var.optional_oh_ctxt[i] = 0;
        hndl->var.odukp_mux_maint_sig[i] = 0;
        hndl->var.odukp_pck_maint_sig[i] = 0;
    }
    
    PMC_RETURN();
} /* odu_tfrm_var_default_init */

/*******************************************************************************
*  odu_tfrm_tcm_pool_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function updates the pool entry state when a TCM layer is provisioned 
*   or unprovisioned. When a new layer is added, the state of the pool entry is 
*   updated to 'Assigned'. When a layer is removed, the state of the pool entry 
*   is updated to 'Unassigned'.
*
*   The total number of free entries available in the pool will be updated
*   accordingly in this function.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - Channel ID
*                         Legal range: 0 - 95
*   pm_tcm_num          - PM or TCMi layer \n 
*                         See odu_tfrm_pm_tcm_id_t \n
*   pool_id             - TCM pool POOL ID
*   assign_state        - pool entry state \n
*                         1 -- Assigned \n
*                         0 -- Unassigned (Free)
*   tcm_mode            - Operating mode of the pool resource
*                         See odu_tfrm_pm_tcm_mode_t.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_tfrm_tcm_pool_update(odu_tfrm_handle_t *odu_tfrm_handle,
                                           UINT32 chnl_id,
                                           odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                           UINT16 pool_id, 
                                           BOOL8 assign_state,
                                           odu_tfrm_pm_tcm_mode_t tcm_mode)
{
    PMC_ENTRY();

    PMC_LOG_TRACE("%s, %s, %d, chnl_id = %u, pm_tcm_num = %u, pool_id = %u, state = %u\n", __FILE__, __FUNCTION__, __LINE__, chnl_id, pm_tcm_num, pool_id, assign_state);

    /* Update the pool entry state */
    odu_tfrm_handle->var.tcm_pool_ptr[pool_id].assign_state = assign_state;

    if (assign_state == 0)
    {
        /* unprovision a TCM layer */
        odu_tfrm_handle->var.tcm_pool_ptr[pool_id].tcm_num = ODU_TFRM_MESSAGE_DONT_CARE;
        odu_tfrm_handle->var.tcm_pool_ptr[pool_id].chnl_id = 0xFF;

        /* Update the number of free entries in the pool */
        odu_tfrm_handle->var.num_pool_free =+ 1;
    }
    else
    {
        /* provision a TCM layer */
        odu_tfrm_handle->var.tcm_pool_ptr[pool_id].tcm_num = pm_tcm_num;
        odu_tfrm_handle->var.tcm_pool_ptr[pool_id].chnl_id = chnl_id;
        
        /* Update the number of free entries in the pool */
        odu_tfrm_handle->var.num_pool_free =- 1;
    }

    PMC_LOG_TRACE("%s, %s, %d, state of tcm_pool_ptr[%u] = %u....\n", __FILE__, __FUNCTION__, __LINE__, pool_id, odu_tfrm_handle->var.tcm_pool_ptr[pool_id].assign_state);
    PMC_LOG_TRACE("%s, %s, %d, TCM number of tcm_pool_ptr[%u] = %u....\n", __FILE__, __FUNCTION__, __LINE__, pool_id, odu_tfrm_handle->var.tcm_pool_ptr[pool_id].tcm_num);
    PMC_LOG_TRACE("%s, %s, %d, Channel ID of tcm_pool_ptr[%u] = %u....\n", __FILE__, __FUNCTION__, __LINE__, pool_id, odu_tfrm_handle->var.tcm_pool_ptr[pool_id].chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_tfrm_tcm_pool_update */

/*******************************************************************************
* odu_tfrm_internal_chnl_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function clean a  channels
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   *chnl               - the channel to clean 
*
* OUTPUTS:
*    None
*
* RETURNS:
*    None
*
* NOTES:
*
*******************************************************************************/
PRIVATE void odu_tfrm_internal_chnl_clean(odu_tfrm_handle_t *odu_tfrm_handle,
                                          UINT32 chnl)
{
    UINT32 pool_itr;
    odu_tfrm_buffer_t b[1];
    
    PMC_ENTRY();
    odu_tfrm_buffer_init(b,odu_tfrm_handle);

    odu_tfrm_lfield_range_PASSTHRU_set(b, odu_tfrm_handle, chnl, chnl,TRUE);   
    if (odu_tfrm_handle->cfg.tfrm_id == ODU_TFRM_STG3B)
    {
        /* SSF and SD_SF status is propagated transparently */
        odu_tfrm_field_SSF_PROP_set(b, odu_tfrm_handle, chnl, 1);
        odu_tfrm_field_SF_SD_PROP_set(b, odu_tfrm_handle, chnl, 1);
    } else 
    {
        /* SSF and SD_SF status is propagated transparently */
        odu_tfrm_field_SSF_PROP_set(b, odu_tfrm_handle, chnl, 0);
        odu_tfrm_field_SF_SD_PROP_set(b, odu_tfrm_handle, chnl, 0);
    }
    
    /* initialize MAINT_SIG ram based register field to 0 */
    odu_tfrm_field_TCM1_TRANSPARENT_set(b, odu_tfrm_handle, chnl, 1);
    odu_tfrm_field_TCM2_TRANSPARENT_set(b, odu_tfrm_handle, chnl, 1);
    odu_tfrm_field_TCM3_TRANSPARENT_set(b, odu_tfrm_handle, chnl, 1);
    odu_tfrm_field_TCM4_TRANSPARENT_set(b, odu_tfrm_handle, chnl, 1);
    odu_tfrm_field_TCM5_TRANSPARENT_set(b, odu_tfrm_handle, chnl, 1);
    odu_tfrm_field_TCM6_TRANSPARENT_set(b, odu_tfrm_handle, chnl, 1);
    odu_tfrm_field_PM_TRANSPARENT_set(b, odu_tfrm_handle, chnl, 1);
    
    odu_tfrm_field_STAT_PM_set(b, odu_tfrm_handle, chnl, 1);
    
    odu_tfrm_field_MAINT_SIG_set(b, odu_tfrm_handle, chnl, 0);
    
    odu_tfrm_field_GCC1_INS_set(b, odu_tfrm_handle, chnl, 0);
    odu_tfrm_field_GCC2_INS_set(b, odu_tfrm_handle, chnl, 0);
    odu_tfrm_field_APS_PCC_INS_set(b, odu_tfrm_handle, chnl, 0);
    odu_tfrm_field_RES2_INS_set(b, odu_tfrm_handle, chnl, 0);
    odu_tfrm_field_EXP_INS_set(b, odu_tfrm_handle, chnl, 0);
    odu_tfrm_field_RES1_INS_set(b, odu_tfrm_handle, chnl, 0);
    odu_tfrm_field_PM_TCM_RES_INS_set(b, odu_tfrm_handle, chnl, 0);
    odu_tfrm_field_TCM_ACT_INS_set(b, odu_tfrm_handle, chnl, 0);
    odu_tfrm_field_FTFL_INS_set(b, odu_tfrm_handle, chnl, 0);
    
    /*by default, enable the RCP interface */
    odu_tfrm_field_RCP_EN_set(b, odu_tfrm_handle, chnl, 1); 
    
    odu_tfrm_handle->var.pm_ctxt_ptr[chnl].assigned_state = FALSE;
    odu_tfrm_handle->var.pm_ctxt_ptr[chnl].child_tcm_num = ODU_TFRM_MESSAGE_DONT_CARE;
    odu_tfrm_handle->var.pm_ctxt_ptr[chnl].child_pool_id = ODU_TFRM_UNUSED_POOL_ID;
    
    odu_tfrm_handle->var.optional_oh_ctxt[chnl] = 0;

    odu_tfrm_handle->var.odukp_mux_maint_sig[chnl] = 0;
    odu_tfrm_handle->var.odukp_pck_maint_sig[chnl] = 0;
   
    odu_tfrm_handle->var.dci_pm_tcm_child[chnl] = ODU_TFRM_MESSAGE_DONT_CARE;


    /* initialize TCM POOL ID context table */
    for (pool_itr = 0; pool_itr < odu_tfrm_handle->cfg.tcm_pool_size; pool_itr++)
    {
        if (odu_tfrm_handle->var.tcm_pool_ptr[pool_itr].chnl_id ==  chnl)
        {
            odu_tfrm_handle->var.tcm_pool_ptr[pool_itr].assign_state  = 0;
            odu_tfrm_handle->var.tcm_pool_ptr[pool_itr].chnl_id       = 0xFF;
            odu_tfrm_handle->var.tcm_pool_ptr[pool_itr].tcm_num       = ODU_TFRM_MESSAGE_DONT_CARE;
            odu_tfrm_handle->var.tcm_pool_ptr[pool_itr].child_tcm_num     = ODU_TFRM_MESSAGE_DONT_CARE;
            odu_tfrm_handle->var.tcm_pool_ptr[pool_itr].child_pool_id     = ODU_TFRM_UNUSED_POOL_ID; 
            odu_tfrm_handle->var.tcm_pool_ptr[pool_itr].odukt_maint_sig = 0;
        }
    }

    odu_tfrm_buffer_flush(b);

    PMC_RETURN();
} /* odu_tfrm_chnl_clean */

/*******************************************************************************
* odu_tfrm_chnl_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function clean a set of channels
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   *chnl               - set of Channel that needs to be cleaned
*   num_chnl            - the number of channel that needs to be cleaned
*
* OUTPUTS:
*    None
*
* RETURNS:
*    None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odu_tfrm_chnl_clean(odu_tfrm_handle_t *odu_tfrm_handle,
                                UINT32 *chnl,
                                UINT32 num_chnl)
{
    UINT32 chnl_itr;
    
    PMC_ENTRY();

    for(chnl_itr = 0; chnl_itr < num_chnl; chnl_itr++)
    {
        odu_tfrm_internal_chnl_clean(odu_tfrm_handle,chnl[chnl_itr]);
    }

    PMC_RETURN();
} /* odu_tfrm_chnl_clean */

/*******************************************************************************
*  odu_tfrm_tcm_pool_id_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function checks if the retrieved pool id is a valid ID.
*   If the retrieved pool ID is not assigned previously, the pool ID
*   is not valid.
*
* INPUTS:
*   odu_tfrm_handle     - pointer to ODU_TFRM handle instance to be operated on
*   pool_id             - TCM POOL ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 odu_tfrm_tcm_pool_id_check(odu_tfrm_handle_t *odu_tfrm_handle,
                                          UINT32 pool_id)
{
    /* variable declaration */
    UINT32 valid;
    UINT32 free = 0, assigned = 1;
    
    PMC_ENTRY();

    if ((odu_tfrm_handle->var.tcm_pool_ptr[pool_id].assign_state) == 0)
    {
        PMC_LOG_TRACE("tcm_pool_ptr[%u] = %u... is unassigned.\n", (unsigned int)pool_id, odu_tfrm_handle->var.tcm_pool_ptr[pool_id].assign_state);
        valid = free;
    }
    else
    {
        valid = assigned;
    }

    PMC_RETURN(valid);
} /* odu_tfrm_tcm_pool_id_check */

/*******************************************************************************
*  odu_tfrm_dci_child_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the first layer provisioned on the given ODU channel.
*
* INPUTS:
*   odu_tfrm_handle     - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - channel ID to request TCM layer information on
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   child_tcm_pm_id     - The TCM/PM/PM-NIM type of the child connected to DCI
*                         See odu_tfrm_pm_tcm_id_t (if no child connected,
*                         returns ODU_TFRM_MESSAGE_DONT_CARE).
*   child_pool_id       - The pool id of the child connected to DCI.
*                         If no child connect, or if the child is the
*                         PM layer, returns ODU_TFRM_UNUSED_POOL_ID
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_dci_child_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                         UINT32 chnl_id,
                                         UINT32 *child_pool_id,
                                         odu_tfrm_pm_tcm_id_t *child_tcm_pm_id)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    UINT32 count = 0;
    
    PMC_ENTRY();
   
    *child_tcm_pm_id = odu_tfrm_handle->var.dci_pm_tcm_child[chnl_id];


    for (i = 0; i < odu_tfrm_handle->cfg.tcm_pool_size; i++)
    {
        if(odu_tfrm_handle->var.tcm_pool_ptr[i].chnl_id == chnl_id)
        {

            if(odu_tfrm_handle->var.tcm_pool_ptr[i].tcm_num == *child_tcm_pm_id){
                *child_pool_id = i;
                *child_tcm_pm_id = odu_tfrm_handle->var.tcm_pool_ptr[i].tcm_num;
                count++;
            }


        }
 
    }        
    
    if(count == 0){
        if(odu_tfrm_pm_prov_state_get(odu_tfrm_handle, chnl_id))
        {
            *child_tcm_pm_id = ODU_TFRM_MESSAGE_TYPE_PM;
        }
    }
    if(count > 1){
        PMC_RETURN(ODU_TFRM_ERR_DCI_CHILD);
    }


    PMC_RETURN(result);
} /* odu_tfrm_dci_child_get */



/*******************************************************************************
*  odu_tfrm_layer_hierarchy_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retreives all non-transparent sub-layers in the layer 
*   hierarchy on a given ODU channel.
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - channel ID to request TCM layer information on
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   tcm_layers          - array of 9 odu_tfrm_layer_struct_t elements
*   num_layers          - an array of TCM instance number and its associated
*                         pool id.
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_tfrm_layer_hierarchy_get(odu_tfrm_handle_t *odu_tfrm_handle,
                                              UINT32 chnl_id,
                                              odu_tfrm_layer_struct_t tcm_layers[ODU_SOURCE_MAX_PM_TCM_LAYERS],
                                              UINT32 *num_layers)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
   
    UINT32 i;
    UINT32 dci_child_pool_id = ODU_TFRM_UNUSED_POOL_ID;
    odu_tfrm_pm_tcm_id_t dci_child_pm_tcm_id;
    BOOL8 layer_list_complete = FALSE;
    
    PMC_ENTRY();

    /* initialize output variables */
    for(i=0;i<ODU_SOURCE_MAX_PM_TCM_LAYERS;i++){
        tcm_layers[i].pool_id = ODU_TFRM_UNUSED_POOL_ID;
        tcm_layers[i].pm_tcm_num = ODU_TFRM_MESSAGE_DONT_CARE;
    }

    *num_layers = 0;


    result = odu_tfrm_dci_child_get(odu_tfrm_handle,
                                    chnl_id,
                                    &dci_child_pool_id,
                                    &dci_child_pm_tcm_id);


    if(result == PMC_SUCCESS && dci_child_pm_tcm_id != ODU_TFRM_MESSAGE_DONT_CARE) {


            while(layer_list_complete == FALSE)
            {
                if( (*num_layers) == 0){
                    tcm_layers[*num_layers].pool_id = dci_child_pool_id;
                    tcm_layers[*num_layers].pm_tcm_num = dci_child_pm_tcm_id;
                }
                else
                {
                    /*get pool id from last layer */
                    if(tcm_layers[*num_layers-1].pm_tcm_num == ODU_TFRM_MESSAGE_TYPE_PM)
                    {
                        
                        tcm_layers[*num_layers].pool_id = odu_tfrm_handle->var.pm_ctxt_ptr[chnl_id].child_pool_id;
                        tcm_layers[*num_layers].pm_tcm_num = odu_tfrm_handle->var.pm_ctxt_ptr[chnl_id].child_tcm_num;

                    }
                    else
                    {

                        tcm_layers[*num_layers].pool_id = odu_tfrm_handle->var.tcm_pool_ptr[tcm_layers[*num_layers-1].pool_id].child_pool_id;
                        tcm_layers[*num_layers].pm_tcm_num = odu_tfrm_handle->var.tcm_pool_ptr[tcm_layers[*num_layers-1].pool_id].child_tcm_num;

                    }

                
                }
                
                
                if(tcm_layers[*num_layers].pm_tcm_num == ODU_TFRM_MESSAGE_TYPE_PM)
                {

                    if(odu_tfrm_handle->var.pm_ctxt_ptr[chnl_id].child_tcm_num == ODU_TFRM_MESSAGE_DONT_CARE )
                    {
                        layer_list_complete = TRUE;
                    } 

                }
                else
                {
                
                    if(odu_tfrm_handle->var.tcm_pool_ptr[tcm_layers[*num_layers].pool_id].child_tcm_num == ODU_TFRM_MESSAGE_DONT_CARE )
                    {
                        layer_list_complete = TRUE;
                    } 

                }
                
                (*num_layers)++;
                  
            }

    }
         
    PMC_RETURN(result);
} /* odu_tfrm_layer_hierarchy_get */


/*******************************************************************************
*  odu_tfrm_update_pm_tcm_ctxt
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*   *odu_tfrm_handle    - pointer to ODU_TFRM handle instance to be operated on
*   chnl_id             - channel ID to request TCM layer information on
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*   parent_id           - Parent layer to provide the sources of CI_SSF in the
*                         event of trail signal fail consequential action.
*                         See odu_tfrm_pm_tcm_id_t for legal values. \n
*                         Always set to ODU_TFRM_MESSAGE_DONT_CARE when 'mode'
*                         set to 0. Always set to ODU_TFRM_MESSAGE_TYPE_DCI if
*                         the layer is the first layer being provisioned for
*                         the channel.
*   mode                - See odu_tfrm_pm_tcm_mode_t.  Set the mode
*                         of the TCM layer
*   prov_action         - TRUE : provision the PM/TMC
*                         FALSE : deprovision the PM/TMC
*   tcm_pool_id         - Pool of the TCM
* OUTPUTS:
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_tfrm_update_pm_tcm_ctxt(odu_tfrm_handle_t *odu_tfrm_handle,
                                             UINT32 chnl_id,
                                             odu_tfrm_pm_tcm_id_t pm_tcm_num,
                                             odu_tfrm_pm_tcm_id_t parent_id,
                                             odu_tfrm_pm_tcm_mode_t mode,
                                             BOOL8 prov_action,
                                             UINT32 tcm_pool_id)
{

/* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
   
    UINT32 i;
    UINT32 insert_index = 0;
    UINT32 remove_index = 0;

    odu_tfrm_layer_struct_t tcm_layers_old[ODU_SOURCE_MAX_PM_TCM_LAYERS];
    UINT32 num_layers_old = 0;

    PMC_ENTRY();
    
    /*get the existing tfrm layer_hierachy */
    /* returns TCMs/PM-NIM/and PM*/
    result = odu_tfrm_layer_hierarchy_get(odu_tfrm_handle,
                                          chnl_id,
                                          tcm_layers_old,
                                          &num_layers_old);

   
    if(parent_id == ODU_TFRM_MESSAGE_TYPE_DCI && prov_action == TRUE)
    {
        insert_index = 0;
    }
    else
    {
        insert_index = 0;

        if(num_layers_old > 0){

            /* find of the index where the TCM/PM is getting inserted into */
            for ( i = 0; i< ODU_SOURCE_MAX_PM_TCM_LAYERS ; i++)
            {
                if(tcm_layers_old[i].pm_tcm_num == parent_id)
                {
                    insert_index = i+1;
                 }

                if(tcm_layers_old[i].pm_tcm_num == pm_tcm_num)
                {
                    remove_index = i;
                }


            }

            /*if inserting */
            if(prov_action == TRUE)
            {
                if((insert_index == 0) || (insert_index == ODU_SOURCE_MAX_PM_TCM_LAYERS))
                {
                      PMC_RETURN(ODU_TFRM_PARENT_LAYER_ERR);
                }
            }
        }

    }

    if(result == PMC_SUCCESS)
    {
        if(prov_action)
        {
            if (pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
            {
                /*prov for TCM*/
  
                /* set the context */
                odu_tfrm_handle->var.tcm_pool_ptr[tcm_pool_id].assign_state = TRUE;
                odu_tfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num = pm_tcm_num;

                odu_tfrm_handle->var.tcm_pool_ptr[tcm_pool_id].child_pool_id = tcm_layers_old[insert_index].pool_id;
                odu_tfrm_handle->var.tcm_pool_ptr[tcm_pool_id].child_tcm_num = tcm_layers_old[insert_index].pm_tcm_num;
                    
                
                if(parent_id == ODU_TFRM_MESSAGE_TYPE_DCI && insert_index == 0)
                {
                    /* Set DCI's new child.  Do not change DCI child if NIMs are provisioned at a higher layer*/
                    odu_tfrm_handle->var.dci_pm_tcm_child[chnl_id] = pm_tcm_num;
                }
                   

                if(insert_index>0)
                {
                    if(tcm_layers_old[insert_index-1].pm_tcm_num == ODU_TFRM_MESSAGE_TYPE_PM)
                    {
                        /* update higher layer's child information */
                        odu_tfrm_handle->var.pm_ctxt_ptr[chnl_id].child_pool_id = tcm_pool_id;
                        odu_tfrm_handle->var.pm_ctxt_ptr[chnl_id].child_tcm_num = pm_tcm_num;
                    }
                    else
                    {
                        /* update higher layer's child information */
                        odu_tfrm_handle->var.tcm_pool_ptr[tcm_layers_old[insert_index-1].pool_id].child_pool_id = tcm_pool_id;
                        odu_tfrm_handle->var.tcm_pool_ptr[tcm_layers_old[insert_index-1].pool_id].child_tcm_num = pm_tcm_num;
                    }

                }

            } 
            else {

                odu_tfrm_handle->var.dci_pm_tcm_child[chnl_id] = ODU_TFRM_MESSAGE_TYPE_PM;
                odu_tfrm_handle->var.pm_ctxt_ptr[chnl_id].assigned_state = TRUE;

                if(num_layers_old >= 1)
                {
                    odu_tfrm_handle->var.pm_ctxt_ptr[chnl_id].child_pool_id = tcm_layers_old[insert_index].pool_id;
                    odu_tfrm_handle->var.pm_ctxt_ptr[chnl_id].child_tcm_num = tcm_layers_old[insert_index].pm_tcm_num;
                }
             
            }
        }
        else
        {
            /* if deprov action */
            if (pm_tcm_num != ODU_TFRM_MESSAGE_TYPE_PM)
            {

                 /*deprov prov for TCM */
                 
                /* update dowm stream parent_tcm_num */

                
                if(num_layers_old > 1)
                {
                    /*if num_layers_old == 1, then we are removing the only existing layer.  No child PID considerations */

                    if(remove_index == 0)
                    {
                        /* first layer is being removed.  Need to set new dci_pm_tcm_child context */
                        odu_tfrm_handle->var.dci_pm_tcm_child[chnl_id] = tcm_layers_old[1].pm_tcm_num;
                    }
                    else
                    {

                        if(tcm_layers_old[remove_index-1].pm_tcm_num == ODU_TFRM_MESSAGE_TYPE_PM)
                        {
                            odu_tfrm_handle->var.pm_ctxt_ptr[chnl_id].child_pool_id =  odu_tfrm_handle->var.tcm_pool_ptr[tcm_layers_old[remove_index].pool_id].child_pool_id;
                            odu_tfrm_handle->var.pm_ctxt_ptr[chnl_id].child_tcm_num =  odu_tfrm_handle->var.tcm_pool_ptr[tcm_layers_old[remove_index].pool_id].child_tcm_num;

                        }
                        else
                        {

                            odu_tfrm_handle->var.tcm_pool_ptr[tcm_layers_old[remove_index-1].pool_id].child_pool_id =  odu_tfrm_handle->var.tcm_pool_ptr[tcm_layers_old[remove_index].pool_id].child_pool_id;
                            odu_tfrm_handle->var.tcm_pool_ptr[tcm_layers_old[remove_index-1].pool_id].child_tcm_num =  odu_tfrm_handle->var.tcm_pool_ptr[tcm_layers_old[remove_index].pool_id].child_tcm_num;

                        }


                    }


                }
                else
                {
                    /* num_layers_old = 1. Last layer removed.  Unset DCI child */
                    odu_tfrm_handle->var.dci_pm_tcm_child[chnl_id] = ODU_TFRM_MESSAGE_DONT_CARE;
                }


                /*clean up context */

                /* set the context back to default state */
                odu_tfrm_handle->var.tcm_pool_ptr[tcm_pool_id].assign_state = FALSE;
                odu_tfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num = ODU_TFRM_MESSAGE_DONT_CARE;

                odu_tfrm_handle->var.tcm_pool_ptr[tcm_pool_id].child_pool_id = ODU_TFRM_UNUSED_POOL_ID;
                odu_tfrm_handle->var.tcm_pool_ptr[tcm_pool_id].child_tcm_num = ODU_TFRM_MESSAGE_DONT_CARE;
                odu_tfrm_handle->var.tcm_pool_ptr[tcm_pool_id].chnl_id = 0xFF; 
                odu_tfrm_handle->var.tcm_pool_ptr[tcm_pool_id].odukt_maint_sig = 0;

                                   
            } else {
                
                 /*PM layer is always first*/

                 odu_tfrm_handle->var.pm_ctxt_ptr[chnl_id].assigned_state = FALSE;
                 odu_tfrm_handle->var.pm_ctxt_ptr[chnl_id].child_tcm_num = ODU_TFRM_MESSAGE_DONT_CARE;
                 odu_tfrm_handle->var.pm_ctxt_ptr[chnl_id].child_pool_id = ODU_TFRM_UNUSED_POOL_ID;

                 if(num_layers_old > 1)
                 {
                    odu_tfrm_handle->var.dci_pm_tcm_child[chnl_id] = tcm_layers_old[1].pm_tcm_num;
                 }
                 else
                 {
                    /* last layer removed.  Unset DCI child */
                    odu_tfrm_handle->var.dci_pm_tcm_child[chnl_id] = ODU_TFRM_MESSAGE_DONT_CARE;
                 }
            }

            /*on deprov, must set the layer to transparent as it
            will not be refreshed in odu_tfrm_gen_and_activate_tcm_pm_layers()*/
            if(result == PMC_SUCCESS)
            {
                result = odu_tfrm_pm_tcmi_mode_set(odu_tfrm_handle,
                                                   chnl_id,
                                                   pm_tcm_num,
                                                   TRUE);
            }

        }    
    } /* end of if(result == PMC_SUCCESS) */


    PMC_RETURN(result);


} /* odu_tfrm_update_pm_tcm_ctxt */

/*******************************************************************************
* FUNCTION: odu_trfm_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   odu_tfrm_handle  - Pointer to ODU_TRFM handle
*   chnl_id          - chnl id
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_tfrm_int_validate(odu_tfrm_handle_t    *odu_tfrm_handle,
                                        UINT32                chnl_id)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != odu_tfrm_handle, ODU_TFRM_ERR_INVALID_PTR, 0, 0);
    
    if (chnl_id >= odu_tfrm_handle->cfg.num_chans)
    {
        result = ODU_TFRM_LOG_ERR_INVALID_PARAMETERS;
    }

    PMC_RETURN(result);
} /* odu_tfrm_int_validate */



/*******************************************************************************
* FUNCTION: odu_trfm_all_tcm_iae_force
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forces an IAE event at all operational TCMs (inserts 010 into STAT field for
*   16 multiframes)
*
* INPUTS:
*   odu_tfrm_handle  - Pointer to ODU_TRFM handle
*   chnl_id          - chnl id
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_trfm_all_tcm_iae_force(odu_tfrm_handle_t    *odu_tfrm_handle,
                                            UINT32                chnl_id)
{
    PMC_ERROR       result = PMC_SUCCESS;

    UINT32 current_iae_ins;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != odu_tfrm_handle, ODU_TFRM_ERR_INVALID_PTR, 0, 0);
    

    if (chnl_id >= odu_tfrm_handle->cfg.num_chans)
    {
        result = ODU_TFRM_LOG_ERR_INVALID_PARAMETERS;
    }


    current_iae_ins = odu_tfrm_field_IAE_INS_get(NULL, odu_tfrm_handle, chnl_id);
   
    
    odu_tfrm_field_IAE_INS_set(NULL, odu_tfrm_handle, chnl_id, (~current_iae_ins & 0x1));


    PMC_RETURN(result);
} /* odu_trfm_all_tcm_iae_force */


/*
** End of file
*/


