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
*   DESCRIPTION : This file contains C functions for receive direction ODU
*   overhead processing methods.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/
#include "pmc_sys.h"
#include "util_global.h"
#include "odu_rfrm_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/

/*
** Local Structures and Unions
*/
typedef void (* defect_enable_fn) (odu_rfrm_buffer_t *b_ptr, odu_rfrm_handle_t *h_ptr, UINT32 value); /*!< type definition for defect_enable_fn */

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
const char ODU_RFRM_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    ODU_RFRM_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char ODU_RFRM_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Forward References
*/

PRIVATE void odu_rfrm_var_default_init(odu_rfrm_handle_t *hndl);

PRIVATE void odu_rfrm_internal_chnl_clean(odu_rfrm_handle_t *odu_rfrm_handle,
                                          UINT32 chnl,
                                          BOOL deprov_configs);

PRIVATE PMC_ERROR odu_rfrm_update_pm_tcm_ctxt(odu_rfrm_handle_t *odu_rfrm_handle,
                                             UINT32 chnl_id,
                                             odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                             odu_rfrm_pm_tcm_id_t parent_id,
                                             odu_rfrm_pm_tcm_mode_t mode,
                                             BOOL8 prov_action,
                                             UINT32 tcm_pool_id);

PRIVATE PMC_ERROR odu_rfrm_tcm_pool_update(odu_rfrm_handle_t *odu_rfrm_handle,
                                           UINT32 chnl_id,
                                           odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                           UINT32 pool_id, BOOL8 assign_state,
                                           odu_rfrm_pm_tcm_mode_t tcm_mode);

PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_defect_ctrl_mode_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                                       UINT32 chnl_id,
                                                       odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                       odu_rfrm_pm_tcm_mode_t mode,
                                                       UINT32 tcm_pool_id);
       
PRIVATE PMC_ERROR odu_rfrm_tcm_transparent_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                               UINT32 chnl_id, 
                                               odu_rfrm_pm_tcm_id_t tcm_num,
                                               UINT32 *transparent);     

PRIVATE PMC_ERROR odu_rfrm_remove_oh_update(odu_rfrm_handle_t *odu_rfrm_handle,
                                            UINT32 chnl_id);                                               
/*
** Public Functions
*/
/*******************************************************************************
* odu_rfrm_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for a ODU_RFRM block instance.
*
*
* INPUTS:
*   *parent             - pointer to parent handle.  If no parent pass in NULL
*   base_address        - base address of the ODU_RFRM subsystem relative to the
*                         device memory space
*   *sys_handle         - pointer to user system handle
*   *tsb_name           - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   odu_rfrm_handle_t   - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC odu_rfrm_handle_t *odu_rfrm_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    odu_rfrm_handle_t *odu_rfrm_handle;

    PMC_ENTRY();

    odu_rfrm_handle = (odu_rfrm_handle_t*)PMC_CTXT_CALLOC(sizeof(odu_rfrm_handle_t), parent);

    pmc_handle_init(parent, odu_rfrm_handle, sys_handle, PMC_MID_DIGI_ODU_RFRM, tsb_name, base_address);
    
    /* Register log strings. */
    pmc_log_block_strings_register(ODU_RFRM_LOG_ERR_STRINGS[0], ODU_RFRM_LOG_ERR_TABLE_BASE, ODU_RFRM_LOG_ERR_COUNT);

    PMC_RETURN(odu_rfrm_handle);
} /* odu_rfrm_ctxt_create */

/*******************************************************************************
* odu_rfrm_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for a ODU_RFRM block instance.
*
*
* INPUTS:
*   *odu_rfrm_handle         - pointer to ODU_RFRM handle instance
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
PUBLIC void odu_rfrm_ctxt_destroy(odu_rfrm_handle_t *odu_rfrm_handle)
{
    PMC_ENTRY();

    #ifdef ODU_RFRM_INT
    #undef ODU_RFRM_INT
    #endif
    #define ODU_RFRM_INT(handle, tsb_comb_field)                    \
    do {                                                            \
        PMC_CTXT_FREE(&(handle->var.int_comb.tsb_comb_field##_PM), odu_rfrm_handle);      \
        PMC_CTXT_FREE(&(handle->var.int_comb.tsb_comb_field##_PID), odu_rfrm_handle);     \
    } while(0)


    PMC_CTXT_FREE(&(odu_rfrm_handle->var.tcm_pool_ptr), odu_rfrm_handle);
    PMC_CTXT_FREE(&(odu_rfrm_handle->var.pm_provisioned), odu_rfrm_handle);
    PMC_CTXT_FREE(&(odu_rfrm_handle->var.pm_mode), odu_rfrm_handle);
    PMC_CTXT_FREE(&(odu_rfrm_handle->var.dci_pm_tcm_child), odu_rfrm_handle);
    PMC_CTXT_FREE(&(odu_rfrm_handle->var.maint_sig), odu_rfrm_handle);
    PMC_CTXT_FREE(&odu_rfrm_handle, odu_rfrm_handle);
    
    PMC_RETURN();
} /* odu_rfrm_ctxt_destroy */

/*******************************************************************************
* odu_rfrm_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an ODU_RFRM block handle instance. Based on the RFRM instance,
*   the maximum number of channels and the maximum pool size supported in the
*   TFRM instance will be determined and cfg_t data updated with these data.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance      
*   odu_rfrm_instance   - ODU_RFRM instance. See odu_rfrm_inst_t enum.
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
PUBLIC void odu_rfrm_handle_init(odu_rfrm_handle_t *odu_rfrm_handle,
                                 odu_rfrm_inst_t odu_rfrm_instance)
{
    /* variable declaration */
    UINT32 size;
   
    PMC_ENTRY();
    
    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);
    /*PMC_ASSERT(odu_rfrm_instance < ODU_RFRM_STGN, ODU_RFRM_LOG_ERR_INVALID_PARAMETERS, 0, 0);*/
   
    /* configure maximum number of channels and pool size for the RFRM instance */  
    switch (odu_rfrm_instance)
    {
        case ODU_RFRM_STG1:
            odu_rfrm_handle->cfg.odu_rfrm_inst   = ODU_RFRM_STG1;
            odu_rfrm_handle->cfg.num_chans       = ODU_RFRM_CHAN_NUM_12;
            odu_rfrm_handle->cfg.tcm_pool_size   = TCM_POOL_SIZE_12;
        break;
        case ODU_RFRM_STGN:
            odu_rfrm_handle->cfg.odu_rfrm_inst   = ODU_RFRM_STGN;
            odu_rfrm_handle->cfg.num_chans       = ODU_RFRM_CHAN_NUM_1;
            odu_rfrm_handle->cfg.tcm_pool_size   = TCM_POOL_SIZE_7;
        break;
        case ODU_RFRM_STG2:
            odu_rfrm_handle->cfg.odu_rfrm_inst   = ODU_RFRM_STG2;
            odu_rfrm_handle->cfg.num_chans       = ODU_RFRM_CHAN_NUM_96;
            odu_rfrm_handle->cfg.tcm_pool_size   = TCM_POOL_SIZE_96;
        break;
        case ODU_RFRM_STG3A:
            odu_rfrm_handle->cfg.odu_rfrm_inst   = ODU_RFRM_STG3A;
            odu_rfrm_handle->cfg.num_chans       = ODU_RFRM_CHAN_NUM_96;
            odu_rfrm_handle->cfg.tcm_pool_size   = TCM_POOL_SIZE_96;
        break;
        case ODU_RFRM_STG3B:
            odu_rfrm_handle->cfg.odu_rfrm_inst   = ODU_RFRM_STG3B;
            odu_rfrm_handle->cfg.num_chans       = ODU_RFRM_CHAN_NUM_96;
            odu_rfrm_handle->cfg.tcm_pool_size   = TCM_POOL_SIZE_96;
        break;
        case ODU_RFRM_STG4:
            odu_rfrm_handle->cfg.odu_rfrm_inst   = ODU_RFRM_STG4;
            odu_rfrm_handle->cfg.num_chans       = ODU_RFRM_CHAN_NUM_96;
            odu_rfrm_handle->cfg.tcm_pool_size   = TCM_POOL_SIZE_96;
        break;
    }
     
    size = sizeof(odu_rfrm_tcm_pool_t) * odu_rfrm_handle->cfg.tcm_pool_size;

    /*PMC_LOG_TRACE("%s, %s, %d, odu_rfrm_handle->cfg.tcm_pool_size = %u...\n", __FILE__, __FUNCTION__, __LINE__, odu_rfrm_handle->cfg.tcm_pool_size);*/

    odu_rfrm_handle->var.tcm_pool_ptr = (odu_rfrm_tcm_pool_t*)PMC_CTXT_CALLOC(size, odu_rfrm_handle);
    PMC_CTXT_REGISTER_PTR(&odu_rfrm_handle->var.tcm_pool_ptr, odu_rfrm_handle);


    odu_rfrm_handle->var.pm_provisioned = (BOOL8*)PMC_CTXT_CALLOC(sizeof(BOOL8)*odu_rfrm_handle->cfg.num_chans, odu_rfrm_handle);
    PMC_CTXT_REGISTER_PTR(&odu_rfrm_handle->var.pm_provisioned, odu_rfrm_handle);

    odu_rfrm_handle->var.pm_mode = (odu_rfrm_pm_tcm_mode_t*)PMC_CTXT_CALLOC(sizeof(odu_rfrm_pm_tcm_mode_t)*odu_rfrm_handle->cfg.num_chans, odu_rfrm_handle);
    PMC_CTXT_REGISTER_PTR(&odu_rfrm_handle->var.pm_mode, odu_rfrm_handle);

    odu_rfrm_handle->var.dci_pm_tcm_child = (odu_rfrm_pm_tcm_id_t*)PMC_CTXT_CALLOC(sizeof(odu_rfrm_pm_tcm_id_t)*odu_rfrm_handle->cfg.num_chans, odu_rfrm_handle);
    PMC_CTXT_REGISTER_PTR(&odu_rfrm_handle->var.dci_pm_tcm_child, odu_rfrm_handle);
       
    odu_rfrm_handle->var.maint_sig = (UINT8*)PMC_CTXT_CALLOC(sizeof(UINT8)*odu_rfrm_handle->cfg.num_chans, odu_rfrm_handle);
    PMC_CTXT_REGISTER_PTR(&odu_rfrm_handle->var.maint_sig, odu_rfrm_handle);


    pmc_bitarray_zero(odu_rfrm_handle->var.int_chnl_en, ODU_RFRM_CHAN_NUM_96);
    pmc_bitarray_zero(odu_rfrm_handle->var.int_pid_en,  TCM_POOL_SIZE_96);
    pmc_bitarray_zero(odu_rfrm_handle->var.int_comb_en, LAST_ODU_RFRM_INT_DEFECT);
    odu_rfrm_handle->var.int_comb_valid = FALSE;
    odu_rfrm_handle->var.int_comb = 0;

    odu_rfrm_var_default_init(odu_rfrm_handle);

    PMC_RETURN();
} /* odu_rfrm_handle_init*/

/*******************************************************************************
* odu_rfrm_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on odu_rfrm.
*
*
* INPUTS:
*   *odu_rfrm_handle      - pointer to ODU_RFRM handle instance
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
PUBLIC PMC_ERROR odu_rfrm_handle_restart_init(odu_rfrm_handle_t *odu_rfrm_handle,
                                              util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                              pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(ODU_RFRM_LOG_ERR_STRINGS[0], ODU_RFRM_LOG_ERR_TABLE_BASE, ODU_RFRM_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);
} /* odu_rfrm_handle_restart_init */

/*******************************************************************************
* odu_rfrm_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of an ODU_RFRM block instance.
*
*
* INPUTS:
*   *odu_rfrm_handle        - pointer to ODU_RFRM handle instance.
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
PUBLIC PMC_ERROR odu_rfrm_base_address_get(odu_rfrm_handle_t *odu_rfrm_handle)
{
    PMC_ENTRY();

    PMC_RETURN(odu_rfrm_handle->base.base_address);
} /* odu_rfrm_base_address_get */

/*******************************************************************************
* odu_rfrm_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes ram based registers to bring ODU_RFRM block to
*   a known state.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   stg2_sysotn_mode    - (field is only relavent for ODU_RFRM_STG2)\n
*                         TRUE : digi is being used for SYSOTN application\n
*                         FALSE : digi is not being used for SYSOTN application
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
PUBLIC PMC_ERROR odu_rfrm_init(odu_rfrm_handle_t *odu_rfrm_handle,
                               BOOL8             stg2_sysotn_mode)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;

    PMC_ENTRY();
    
    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);
    
       /* The following RAM based registers need to be initialized
        EXP_TTI_PID
        EXP_TTI_PM
        LAYER_CTRL0_PID 
        LAYER_CTRL0_PM
        LAYER_CTRL1_PID
        LAYER_CTRL1_PM
        CHANNEL_CTRL0
        TCM1_2_3_POOL_ID
        TCM4_5_6_POOL_ID
        FLOATING_PM_NIM_POOL_ID
        */
    odu_rfrm_lfield_range_EX_TTI_set(NULL, odu_rfrm_handle, 0, (odu_rfrm_handle->cfg.tcm_pool_size*8), 0);
    odu_rfrm_lfield_range_EX_TTI_PM_set(NULL, odu_rfrm_handle, 0, (odu_rfrm_handle->cfg.num_chans*8), 0);   
    
    for (i = 0; i < odu_rfrm_handle->cfg.tcm_pool_size; i++)
    {
        odu_rfrm_reg_LAYER_CTRL0_PID_array_write(NULL, odu_rfrm_handle, i, 0x0020000F);
        odu_rfrm_reg_LAYER_CTRL1_PID_array_write(NULL, odu_rfrm_handle, i, 0x000100);
        PMC_ATOMIC_YIELD(odu_rfrm_handle->base.parent_handle->parent_handle,0);
    }
    
    for (i = 0; i < odu_rfrm_handle->cfg.num_chans; i++)
    {
        odu_rfrm_reg_LAYER_CTRL0_PM_array_write(NULL, odu_rfrm_handle, i, 0x0020000F);
        /* must set PARENT_IS_DCI_PM = 1 to avoid interrupts when PM is not
        ** provisioned as part of the datapath
        */
        odu_rfrm_reg_LAYER_CTRL1_PM_array_write(NULL, odu_rfrm_handle, i, 0x000100);
        /*Setting all transparent bits to 1, setting LAST_IS_DCI to 1 */
        odu_rfrm_reg_CHANNEL_CTRL0_array_write(NULL, odu_rfrm_handle, i, 0x0007F008);
        odu_rfrm_field_MAINT_SIG_set(NULL, odu_rfrm_handle, i, 0);
        odu_rfrm_handle->var.maint_sig[i] = 0;
        odu_rfrm_reg_TCM1_2_3_PID_array_write(NULL, odu_rfrm_handle, i, 0);
        odu_rfrm_reg_TCM4_5_6_PID_array_write(NULL, odu_rfrm_handle, i, 0);
        odu_rfrm_reg_FLOATING_PM_NIM_PID_array_write(NULL, odu_rfrm_handle, i, 0);
        PMC_ATOMIC_YIELD(odu_rfrm_handle->base.parent_handle->parent_handle,0);

    }
    
    /*RFRM's that are recieving off the ODUKSW must have 
      LOFLOM_ACT_DIS set to 1.  All receive path frammers must have
      LOFLOM_ACT_DIS set to 0*/
    if(odu_rfrm_handle->cfg.odu_rfrm_inst == ODU_RFRM_STG1 ||
         (odu_rfrm_handle->cfg.odu_rfrm_inst == ODU_RFRM_STG2 &&
         stg2_sysotn_mode == FALSE) ||
         odu_rfrm_handle->cfg.odu_rfrm_inst == ODU_RFRM_STG3A ||
         odu_rfrm_handle->cfg.odu_rfrm_inst == ODU_RFRM_STGN)
    {
          for (i = 0; i < odu_rfrm_handle->cfg.num_chans; i++)
            {
                    odu_rfrm_field_LOFLOM_ACT_DIS_set(NULL, odu_rfrm_handle, i, FALSE);
            }
    }
    else
    {
        for (i = 0; i < odu_rfrm_handle->cfg.num_chans; i++)
            {
                odu_rfrm_field_LOFLOM_ACT_DIS_set(NULL, odu_rfrm_handle, i, TRUE);
          }
    }
    
    
                                                                                                                                                                                 
    PMC_RETURN(result);
} /* odu_rfrm_init */


/*******************************************************************************
* odu_rfrm_chnl_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the operational mode of an ODUk framer in ODU_RFRM
*   block.
*
*   When an ODUk channel uses the Frame Alignment Unit(FAU), the FAU is used to
*   detect FAS and MFAS alignment in the ODUk channel. When the FAU is disabled
*   for the ODUk channel, RX DCI control signals are used to determine the ODUk
*   channel's frame alignment.
*
*   When an ODUk channel is not operational, no overhead extraction
*   to registers or defect/consequntial action processing is performed.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   enbl                - Enables or disables the ODUk channel \n
*                         0 -- disabled; in passthru \n
*                         1 -- enabled; the ODUk channel is operational.
*   fau_enbl            - Enables or disables the FAU \n
*                         0 -- disabled; in passthru \n
*                         1 -- enabled; the FAU is out of passthru
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
PUBLIC PMC_ERROR odu_rfrm_chnl_init(odu_rfrm_handle_t *odu_rfrm_handle,
                                    UINT32 chnl_id, UINT32 enbl, UINT32 fau_enbl)
{
    /* variable declaration */
    odu_rfrm_buffer_t odu_rfrm_buffer[1];

    PMC_ENTRY();

    odu_rfrm_buffer_init(odu_rfrm_buffer, odu_rfrm_handle);

    /* Configure the channel to be operational or not */
    odu_rfrm_field_PASSTHRU_set(odu_rfrm_buffer, odu_rfrm_handle, chnl_id, 1-enbl);
    
    /* if stage 1 and N, there is no FAU */
    if (odu_rfrm_handle->cfg.odu_rfrm_inst != ODU_RFRM_STG1 
        || odu_rfrm_handle->cfg.odu_rfrm_inst != ODU_RFRM_STGN)
    {
        /* Set FAU_PASSTHRU bit */
        odu_rfrm_field_FAU_PASSTHRU_set(odu_rfrm_buffer, odu_rfrm_handle, chnl_id, 1-fau_enbl);
    }

    odu_rfrm_buffer_flush(odu_rfrm_buffer);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_chnl_init */

/*******************************************************************************
* odu_rfrm_passthru_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the PASSTHRU status for all channels
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*
* OUTPUTS:
*   passthru            - the passthru vector
*   fau_passthru        - the FAU passthru vector
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odu_rfrm_passthru_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                  UINT32 passthru[3],
                                  UINT32 fau_passthru[3])
{
    UINT32 word_itr,chnl_itr;
    /* variable declaration */
    PMC_ENTRY();

    for (word_itr= 0; word_itr < 3; word_itr++)
    {
        passthru[word_itr] = 0;
        fau_passthru[word_itr] = 0;
    }
    for (chnl_itr = 0;chnl_itr < odu_rfrm_handle->cfg.num_chans; chnl_itr++)
    {        
        passthru[chnl_itr/32]     |= (odu_rfrm_field_PASSTHRU_get(NULL,odu_rfrm_handle,chnl_itr) << (chnl_itr %32));
        fau_passthru[chnl_itr/32] |= (odu_rfrm_field_FAU_PASSTHRU_get(NULL,odu_rfrm_handle,chnl_itr) << (chnl_itr %32));
    }

    PMC_RETURN();
} /* odu_rfrm_passthru_get */


/*******************************************************************************
* odu_rfrm_pool_id_assign
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function either internally manages TCMi pool IDs or allows the calling
*   system to specify explicit IDs for use with a resource.
*
*   When user assigned pool IDs are used, if the pool ID is already allocated
*   to a different resource an error will be raised.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
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
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pool_id_assign(odu_rfrm_handle_t *odu_rfrm_handle,
                                         UINT32 chnl_id, 
                                         odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                         UINT8 assign_mode, UINT8 pool_id,
                                         UINT16 *assigned_pool_id)
{
    /* variable declaration */
    UINT32 i;
    UINT32 tcm_pool_id = 256;
    BOOL8 state = 1;
    UINT32 cnt = 0;

    PMC_ENTRY();

    PMC_LOG_TRACE("%s, %s, %d, odu_rfrm_pool_id_assign Called...\n", __FILE__, __FUNCTION__, __LINE__);

    /* if user assigns the pool_id, check to see if this pool_id is free */
    if (assign_mode == 1)
    {
        if (TRUE == odu_rfrm_handle->var.tcm_pool_ptr[pool_id].assign_state)
        {
            PMC_LOG_TRACE("%s, %s, %d, tcm_pool_id already assigned = %u...\n", __FILE__, __FUNCTION__, __LINE__, pool_id);
            PMC_RETURN(ODU_RFRM_LOG_CODE_POOL_ENTRY_UNAVAIL);
        }
        else
        {
            PMC_LOG_TRACE("%s, %s, %d, tcm_pool_id assigned = %u...\n", __FILE__, __FUNCTION__, __LINE__, pool_id);
            tcm_pool_id = pool_id;
        }
    }
    else
    {
        PMC_LOG_TRACE("%s, %s, %d, SW internally assigned POOL ID\n", __FILE__, __FUNCTION__, __LINE__);
        /* assign pool_id internally */
        /* Search for a free POOL ID and assign a unique TCM_POOL_ID */
        for (i = 0; i <  odu_rfrm_handle->cfg.tcm_pool_size; i++)
        {
            if (FALSE == odu_rfrm_handle->var.tcm_pool_ptr[i].assign_state)
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

        if (cnt == odu_rfrm_handle->cfg.tcm_pool_size)
        {
            PMC_LOG_TRACE("%s, %s, %d, Pool IDs all taken!!!\n", __FILE__, __FUNCTION__, __LINE__);
            PMC_RETURN(ODU_RFRM_LOG_CODE_POOL_FULL);
        }
    }

    /* update the tcm pool pool id table */
    if (tcm_pool_id < odu_rfrm_handle->cfg.tcm_pool_size )
    {
        
        odu_rfrm_tcm_pool_update(odu_rfrm_handle, chnl_id, pm_tcm_num, 
                                 tcm_pool_id, state, ODU_RFRM_PM_TCM_START); 
        PMC_LOG_TRACE("1. tcm_pool_id assignment stat: pool_id = %u, pm_tcm_num = %u, chnl_id = %u...\n", 
                       tcm_pool_id,
                       odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num, 
                       odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].chnl_id);

    }
    else
    {
        PMC_RETURN(ODU_RFRM_LOG_CODE_POOL_ENTRY_OUT_OF_RANGE);
    }

    PMC_LOG_TRACE("%s, %s, %d, 2. tcm_pool_id assignment stat: pool_id = %u, pm_tcm_num = %u, chnl_id = %u...\n", __FILE__, __FUNCTION__, __LINE__, 
                   tcm_pool_id,
                   odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num, 
                   odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].chnl_id);


    *assigned_pool_id = tcm_pool_id;
    

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pool_id_assign */

/*******************************************************************************
*  odu_rfrm_pool_id_return
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function looks up the pool id from the internal pool id table
*   and returns the pool id associated with the TCM/Floating TCM/PM_NIM field
*   instance of a channel.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
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
PUBLIC PMC_ERROR odu_rfrm_pool_id_return(odu_rfrm_handle_t *odu_rfrm_handle,
                                         UINT32 chnl_id, 
                                         odu_rfrm_pm_tcm_id_t tcm_num, 
                                         UINT32 *pool_id)
{
    /* variable declaration */
    UINT32 i;
    UINT32 count = 0;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_LOG_TRACE("%s, %s, %d, cfg_ptr->tcm_pool_size = %u\n", __FILE__, __FUNCTION__, __LINE__, odu_rfrm_handle->cfg.tcm_pool_size);
    PMC_LOG_TRACE("%s, %s, %d, tcm pool size = %u\n", __FILE__, __FUNCTION__, __LINE__, odu_rfrm_handle->cfg.tcm_pool_size);

    for (i = 0; i < odu_rfrm_handle->cfg.tcm_pool_size; i++)
    {
        PMC_LOG_TRACE("%s, %s, %d, TCM layer number: var_ptr->tcm_pool[%u]->tcm_num = %u\n", __FILE__, __FUNCTION__, __LINE__, i, (odu_rfrm_handle->var.tcm_pool_ptr[i]).tcm_num);
        if (odu_rfrm_handle->var.tcm_pool_ptr[i].chnl_id == chnl_id && 
            odu_rfrm_handle->var.tcm_pool_ptr[i].tcm_num == tcm_num)
        {
            /* found the entry */
            PMC_LOG_TRACE("%s, %s, %d, Found the entry: (var_ptr->tcm_pool[i])->chnl_id[%u] = %u, (var_ptr->tcm_pool[i])->tcm_num[%u] = %u\n", __FILE__, __FUNCTION__, __LINE__, i, odu_rfrm_handle->var.tcm_pool_ptr[i].chnl_id, i, odu_rfrm_handle->var.tcm_pool_ptr[i].tcm_num);
            *pool_id = i;
            count++;
        }
    }

    PMC_LOG_TRACE("%s, %s, %d, pool id duplicate count = %u\n", __FILE__, __FUNCTION__, __LINE__, count);

    /* check for duplicates */
    if (count > 1)
    {
        result = ODU_RFRM_LOG_CODE_POOL_ID_DUPLICATE;
    }
    else if (count == 0)
    {
      result = ODU_RFRM_LOG_CODE_POOL_ID_NOT_FOUND;
    }
    else if (*pool_id >= TCM_POOL_SIZE_96)
    {
        result = ODU_RFRM_LOG_ERR_INVALID_PARAMETERS;
    }
    else if (FALSE == odu_rfrm_handle->var.tcm_pool_ptr[*pool_id].assign_state)
    {
        result = ODU_RFRM_LOG_CODE_POOL_ID_UNASSIGNED;
    }

    PMC_RETURN(result);
} /* odu_rfrm_pool_id_return */

/*******************************************************************************
* odu_rfrm_pm_tcmi_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures PM or TCMi layer of an ODUk channel in ODU_RFRM
*   block.  It configures whether a PM or TCMi layer of an ODUk channel is
*   transparently passed or if message processing is performed.
*
*   If message processing is performed prior to calling this function, a
*   TCM_POOL_ID must have been assigned for the resource (if pm_tcm_num is set
*   from 1 to 8) with odu_rfrm_pool_id_assign() API.
*
*   When called with pm_tcm_num set to 7 (Floating TCM) the function will
*   assign the TCMi associated with the Floating TCM (mode set to 1) or
*   unassign the TCMi (mode set to 0).
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM (must set float_tcmi_src) \n
*                         8         -- PM NIM (non-intrusive monitoring)
*   float_tcmi_src      - TCM number monitored by Floating TCM. \n
*                         Legal range: 1 - 6. Only valid when pm_tcm_num is set
*                         to 7.
*   parent_id           - Parent layer to provide the sources of CI_SSF in the
*                         event of trail signal fail consequential action.
*                         See odu_rfrm_pm_tcm_id_t for legal values. \n
*                         Always set to ODU_RFRM_MESSAGE_DONT_CARE when 'mode'
*                         set to 0. Always set to ODU_RFRM_MESSAGE_TYPE_DCI if
*                         the layer is the first layer being provisioned for
*                         the channel.
*   mode                - See odu_rfrm_pm_tcm_mode_t.  Set the mode
*                         of the TCM layer
*   prov_action         - TRUE : provision the PM/TMC/floating TCM/PM-NIM
*                         FALSE : deprovision the PM/TMC/floating TCM/PM-NIM
*
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
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                      UINT32 chnl_id,
                                      odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                      UINT32 float_tcmi_src,
                                      odu_rfrm_pm_tcm_id_t parent_id,
                                      odu_rfrm_pm_tcm_mode_t mode,
                                      BOOL8 prov_action)
{
    /* variable declaration */
    PMC_ERROR ret_code = PMC_SUCCESS;
    UINT32 i;
    UINT32 tcm_pool_id = -1;
    UINT32 wr_en = 1;
    odu_rfrm_buffer_t odu_rfrm_buffer;      
    UINT32 parent_pool_id = ODU_RFRM_UNASSIGNED_POOL_ID;
    odu_rfrm_layer_struct_t tcm_layers_new[ODU_SINK_MAX_PM_TCM_LAYERS];
    UINT32 num_layers_new = 0;

    PMC_ENTRY();

    /* argument checking */
    if (chnl_id > 95 || pm_tcm_num > LAST_ODU_RFRM_MESSAGE_TYPE 
        || (prov_action == TRUE && pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_FLOATING_TCM && (float_tcmi_src < 1 || float_tcmi_src > 6))
        || (prov_action == TRUE && (parent_id > ODU_RFRM_MESSAGE_TYPE_DCI))
        || (prov_action == TRUE && (mode >= ODU_RFRM_PM_TCM_LAST))
        || (prov_action == TRUE && (parent_id == ODU_RFRM_MESSAGE_TYPE_PM_NIM || parent_id == ODU_RFRM_MESSAGE_TYPE_PM))
        || (prov_action == TRUE && pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM_NIM && mode != ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR)
        || (prov_action == TRUE && pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_FLOATING_TCM && mode != ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR)
        || (prov_action == TRUE && pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM && mode == ODU_RFRM_PM_TCM_TT_MONITOR) 
        || (prov_action == TRUE && pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM && mode == ODU_RFRM_PM_TCM_TT_ERASE))
    {
        /* some error */
        PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }

    /* this should only be performed once */
    if (pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM)
    {
          ret_code = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);
    }
   
    if (ret_code == PMC_SUCCESS)
    {
        /* update the context */
        ret_code = odu_rfrm_update_pm_tcm_ctxt(odu_rfrm_handle,
                                               chnl_id,
                                               pm_tcm_num,
                                               parent_id,
                                               mode,
                                               prov_action,
                                               tcm_pool_id);
    }

    if (ret_code == PMC_SUCCESS)
    {
        /*get the new rfrm layer_hierachy */
        ret_code = odu_rfrm_layer_hierarchy_get(odu_rfrm_handle,
                                                chnl_id,
                                                &tcm_layers_new,
                                                &num_layers_new);
    }

    /* only update for provisioning. For deprovisioning, context will be updated
    ** when the layers get reconnected in the activation stage
    */ 
    if (PMC_SUCCESS == ret_code && prov_action == TRUE)
    {  
        /* update parent_is_dci and parent_pool_id for the current TCM/PM only */
        if (parent_id == ODU_RFRM_MESSAGE_TYPE_DCI)
        {
            if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
            { 
                odu_rfrm_field_PARENT_IS_DCI_PM_set(NULL, odu_rfrm_handle, chnl_id, 1);
            }
            else {
                odu_rfrm_field_PARENT_IS_DCI_set(NULL, odu_rfrm_handle, tcm_pool_id, 1);
            }
        }
        else {
            /* If parent is any TCM (PM would not be a parent in RFRM), update with the new parent's pool id */
            /* need to find the pool id of the parent */
            for (i = 0; i < num_layers_new; i++)
            {
                if (tcm_layers_new[i].pm_tcm_num == parent_id)
                {
                    /* parent layer found */
                    parent_pool_id = tcm_layers_new[i].pool_id;
                    break;
                }            
            }
                                                     
            if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
            {                                          
                odu_rfrm_field_PARENT_POOL_ID_PM_set(NULL, odu_rfrm_handle, chnl_id, parent_pool_id);
            }
            else
            {
                odu_rfrm_field_PARENT_POOL_ID_set(NULL, odu_rfrm_handle, tcm_pool_id, parent_pool_id);
            }            
        }        
    }            

    if(ret_code == PMC_SUCCESS)
    {
        if(prov_action == TRUE)
        {
            ret_code = odu_rfrm_pm_tcmi_defect_ctrl_mode_cfg(odu_rfrm_handle,
                                                             chnl_id,
                                                             pm_tcm_num,
                                                             mode,
                                                             tcm_pool_id);
            
        }
        else
        {
            ret_code = odu_rfrm_pm_tcmi_defect_ctrl_mode_cfg(odu_rfrm_handle,
                                                             chnl_id,
                                                             pm_tcm_num,
                                                             ODU_RFRM_PM_TCM_TT_TRANSPARENT,
                                                             tcm_pool_id);
        }
    }

    /*set the pool_id of the TCM/floating-TCM/PM-NIM*/
    /* If provisioning for a TCM/Floating TCM/or PM_NIM layer, find the pool id */
    if (pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM && ret_code == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("PM/TCMi layer is to be provisioned...tcm_pool_id assigned = %u...\n", tcm_pool_id);

        if (tcm_pool_id > odu_rfrm_handle->cfg.tcm_pool_size)
        {
            PMC_LOG_TRACE("%s, %s, %d, POOL ID unassigned: pool_id = %u\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);
            PMC_RETURN(ODU_RFRM_LOG_CODE_POOL_ID_UNASSIGNED);
        }

        /* Configure TCMx_POOL_ID register */
        /* WR_EN and TCMx_POLL_ID must be written in one register write */
        switch (pm_tcm_num)
        {
            case ODU_RFRM_MESSAGE_TYPE_TCM1: /* TCM1 */
                odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);  
                /* enable writing to POOL ID field */
                odu_rfrm_field_TCM1_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, wr_en);
                /* assign unique POOL ID */
                odu_rfrm_field_TCM1_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id);
                odu_rfrm_buffer_flush(&odu_rfrm_buffer);           
                break;
            case ODU_RFRM_MESSAGE_TYPE_TCM2: /* TCM2 */
                /* enable writing to POOL ID field */
                odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);
                odu_rfrm_field_TCM2_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, wr_en);
                /* assign unique POOL ID */
                odu_rfrm_field_TCM2_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id); 
                odu_rfrm_buffer_flush(&odu_rfrm_buffer); 
                break;
            case ODU_RFRM_MESSAGE_TYPE_TCM3: /* TCM3 */
                /* enable writing to POOL ID field */   
                odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);
                odu_rfrm_field_TCM3_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, wr_en);
                /* assign unique POOL ID */
                odu_rfrm_field_TCM3_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id);
                odu_rfrm_buffer_flush(&odu_rfrm_buffer); 
                break;
            case ODU_RFRM_MESSAGE_TYPE_TCM4: /* TCM4 */
                /* enable writing to POOL ID field */
                odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);
                odu_rfrm_field_TCM4_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, wr_en);
                /* assign unique POOL ID */
                odu_rfrm_field_TCM4_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id);
                odu_rfrm_buffer_flush(&odu_rfrm_buffer); 
                break;
            case ODU_RFRM_MESSAGE_TYPE_TCM5: /* TCM5 */
                odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);
                /* enable writing to POOL ID field */
                odu_rfrm_field_TCM5_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, wr_en);
                /* assign unique POOL ID */
                odu_rfrm_field_TCM5_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id);
                odu_rfrm_buffer_flush(&odu_rfrm_buffer); 
                break;
            case ODU_RFRM_MESSAGE_TYPE_TCM6: /* TCM6 */ 
                odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);
                /* enable writing to POOL ID field */
                odu_rfrm_field_TCM6_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, wr_en);
                /* assign unique POOL ID */
                odu_rfrm_field_TCM6_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id);
                odu_rfrm_buffer_flush(&odu_rfrm_buffer); 
                break;
            case ODU_RFRM_MESSAGE_TYPE_FLOATING_TCM: /* FLoating TCM */ 
                odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);
                /* enable writing to POOL ID field */
                odu_rfrm_field_FLOATING_TCM_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, wr_en);
                /* assign unique POOL ID */
                odu_rfrm_field_FLOATING_TCM_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id); 
                odu_rfrm_buffer_flush(&odu_rfrm_buffer); 
                break;
            case ODU_RFRM_MESSAGE_TYPE_PM_NIM: /* PM_NIM */
                odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);
                /* enable writing to POOL_ID field */
                odu_rfrm_field_PM_NIM_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, wr_en);
                /* assign unique POOL ID */
                odu_rfrm_field_PM_NIM_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id);
                odu_rfrm_buffer_flush(&odu_rfrm_buffer); 
                break;
            default:
                PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
        }
    }

    
    if( ret_code == PMC_SUCCESS)
    {
        if(prov_action == TRUE)
        {
            /* Set the MODE/MODE_PM config, deassert the trasparent bit */
            ret_code = odu_rfrm_pm_tcmi_mode_set(odu_rfrm_handle,
                                                 chnl_id,
                                                 pm_tcm_num,
                                                 float_tcmi_src,
                                                 mode,
                                                 tcm_pool_id);

        }
        else
        {
            /* Set the MODE/MODE_PM config, assert the trasparent bit */
            ret_code = odu_rfrm_pm_tcmi_mode_set(odu_rfrm_handle,
                                                 chnl_id,
                                                 pm_tcm_num,
                                                 float_tcmi_src,
                                                 ODU_RFRM_PM_TCM_TT_TRANSPARENT,
                                                 tcm_pool_id);
        }
    }

    /* update XX_REMOVE_OH bits based on maintenance signal configuration*/
    if(ret_code == PMC_SUCCESS)
    {
        ret_code = odu_rfrm_remove_oh_update(odu_rfrm_handle,
                                             chnl_id);
    }

    PMC_LOG_TRACE("Configuring ODU_RFRM for channel ID = %u...\n", chnl_id);

    PMC_RETURN(ret_code);

} /* odu_rfrm_pm_tcmi_cfg */

/*******************************************************************************
* odu_rfrm_pm_tcmi_mode_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Changes the mode of an already operational TCM.  Support hitless transition
*   between the following modes without having to deprovision the TCM:
*      ODU_STRUCT_PM_TCM_TT_OPERATIONAL -> ODU_STRUCT_PM_TCM_TT_MONITOR
*      ODU_STRUCT_PM_TCM_TT_MONITOR -> ODU_STRUCT_PM_TCM_TT_OPERATIONAL
*      ODU_STRUCT_PM_TCM_TT_OPERATIONAL -> ODU_STRUCT_PM_TCM_ NON_INTRUSIVE_MONITOR
*      ODU_STRUCT_PM_TCM_ NON_INTRUSIVE_MONITOR-> ODU_STRUCT_PM_TCM_TT_OPERATIONAL
*      ODU_STRUCT_PM_TCM_TT_MONITOR -> ODU_STRUCT_PM_TCM_ NON_INTRUSIVE_MONITOR
*      ODU_STRUCT_PM_TCM_ NON_INTRUSIVE_MONITOR-> ODU_STRUCT_PM_TCM_TT_MONITOR
*      ODU_STRUCT_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED  -> ODU_STRUCT_PM_TCM_TT_MONITOR
*      ODU_STRUCT_PM_TCM_TT_MONITOR -> ODU_STRUCT_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED
*      ODU_STRUCT_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED -> ODU_STRUCT_PM_TCM_ NON_INTRUSIVE_MONITOR
*      ODU_STRUCT_PM_TCM_ NON_INTRUSIVE_MONITOR-> ODU_STRUCT_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZERO
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance. For this API, only valid for: \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM (must set float_tcmi_src) \n
*   mode                - See odu_rfrm_pm_tcm_mode_t.  Set the mode
*                         of the TCM layer
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
PUBLIC PMC_ERROR odu_rfrm_tcmi_mode_update(odu_rfrm_handle_t *odu_rfrm_handle,
                                           UINT32 chnl_id,
                                           odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                           odu_rfrm_pm_tcm_mode_t mode)
{
    /* variable declaration */
    PMC_ERROR ret_code = PMC_SUCCESS;
    UINT32 float_tcmi_src = 0;
    UINT32 tcm_pool_id = 0;
    UINT32 num_layers;
    odu_rfrm_layer_struct_t odu_rfrm_layers[ODU_SINK_MAX_PM_TCM_LAYERS];

    PMC_ENTRY();

    /* argument checking */
    if (chnl_id > 95 || pm_tcm_num > LAST_ODU_RFRM_MESSAGE_TYPE
        || (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM_NIM)
        || (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_FLOATING_TCM)
        || (mode >= ODU_RFRM_PM_TCM_LAST)
        || (mode == ODU_RFRM_PM_TCM_TT_ERASE))
    {
        /* some error */
        PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }
    
   
    /*update the mode of the currently provisioned TCM*/
    if(pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
    {
        if(odu_rfrm_handle->var.pm_provisioned[chnl_id] == TRUE)
        {
            odu_rfrm_handle->var.pm_mode[chnl_id] = mode;
        }
        else
        {
            PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
        }
    }
    else
    {
        ret_code = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);
        
        if(ret_code == PMC_SUCCESS)
        {
            if(odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_mode == ODU_RFRM_PM_TCM_TT_ERASE)
            {
                /*mode update does not support transitions to/from ERASE mode*/
                PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
            }
            else
            {
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_mode = mode;
            }
        }
    }


    if(ret_code == PMC_SUCCESS)
    {
        ret_code = odu_rfrm_pm_tcmi_defect_ctrl_mode_cfg(odu_rfrm_handle,
                                                         chnl_id,
                                                         pm_tcm_num,
                                                         mode,
                                                         tcm_pool_id);
    }


    if( ret_code == PMC_SUCCESS)
    {
        /* Set the MODE/MODE_PM config, deassert the trasparent bit */
        ret_code = odu_rfrm_pm_tcmi_mode_set(odu_rfrm_handle,
                                             chnl_id,
                                             pm_tcm_num,
                                             float_tcmi_src,
                                             mode,
                                             tcm_pool_id);
    }

    if( ret_code == PMC_SUCCESS)
    {
        ret_code = odu_rfrm_layer_hierarchy_get(odu_rfrm_handle,
                                                chnl_id,
                                                &odu_rfrm_layers,
                                                &num_layers);
    }
    
    if (PMC_SUCCESS == ret_code)
    {
        /* connect to the new parent */
        ret_code = odu_rfrm_parent_pid_parent_is_dci_update(odu_rfrm_handle,
                                                            chnl_id,
                                                            odu_rfrm_layers,
                                                            num_layers);
    }     
        
    /*call function to set last_layer */
    if (PMC_SUCCESS == ret_code)
    {
        ret_code = odu_rfrm_last_layer_last_is_dci_update(odu_rfrm_handle,
                                                          chnl_id,
                                                          odu_rfrm_layers,
                                                          num_layers);
    }        


    /* update XX_REMOVE_OH bits based on maintenance signal configuration*/
    if(ret_code == PMC_SUCCESS)
    {
        ret_code = odu_rfrm_remove_oh_update(odu_rfrm_handle,
                                             chnl_id);
    }

    PMC_LOG_TRACE("Configuring ODU_RFRM for channel ID = %u...\n", chnl_id);

    PMC_RETURN(ret_code);

} /* odu_rfrm_pm_tcmi_mode_update */

/*******************************************************************************
* odu_rfrm_pm_tcmi_parent_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Changes the parent of an already operational PM or TCM.
* 
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - Enum, PM or TCMi field instance. See odu_rfrm_pm_tcm_id_t.
*   new_parent_tcm_num  - Enum, PM or TCMi field instance of the new parent.
*                         See odu_rfrm_pm_tcm_id_t
*   odu_rfrm_layers     - an array representing the current layer hierarchy
*   num_layers          - number of layers in the current hierarchy
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
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_parent_update(odu_rfrm_handle_t *odu_rfrm_handle,
                                                UINT32 chnl_id,
                                                odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                odu_rfrm_pm_tcm_id_t new_parent_tcm_num,
                                                odu_rfrm_layer_struct_t odu_rfrm_layers[ODU_SINK_MAX_PM_TCM_LAYERS],
                                                UINT32 num_layers)
{
    /* variable declaration */
    PMC_ERROR ret_code = PMC_SUCCESS;
    odu_rfrm_pm_tcm_id_t cur_parent_tcm_num = LAST_ODU_RFRM_MESSAGE_TYPE;
    odu_rfrm_pm_tcm_mode_t cur_tcm_mode = ODU_RFRM_PM_TCM_LAST;
    UINT32 cur_tcm_pool_id = 0;
    UINT32 new_parent_pool_id = 0;
    UINT32 i;

    PMC_ENTRY();

    /* argument checking */
    if (chnl_id > 95 || pm_tcm_num > LAST_ODU_RFRM_MESSAGE_TYPE
        || new_parent_tcm_num > LAST_ODU_RFRM_MESSAGE_TYPE
        || num_layers == 0 || num_layers > ODU_SINK_MAX_PM_TCM_LAYERS
        || (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
        || (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_DCI)
        || (new_parent_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM))
    {
        /* some error */
        PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }

    /* retrieve parent of the current pm_tcm_num */
    if (PMC_SUCCESS == ret_code)
    {
        for (i = 0; i < num_layers; i++)
        {
            if (odu_rfrm_layers[i].pm_tcm_num == pm_tcm_num)
            {
                /* if the current layer is the first in the hierarchy,
                ** its parent is DCI
                */
                if (i == 0)
                {
                    cur_parent_tcm_num = ODU_RFRM_MESSAGE_TYPE_DCI;
                }
                else {
                    cur_parent_tcm_num = odu_rfrm_layers[i-1].pm_tcm_num;
                }
                cur_tcm_mode = odu_rfrm_layers[i].tcm_mode;
                cur_tcm_pool_id = odu_rfrm_layers[i].pool_id;
            }

            if (odu_rfrm_layers[i].pm_tcm_num == new_parent_tcm_num)
            {
                new_parent_pool_id = odu_rfrm_layers[i].pool_id;
            }
        }
    }

    /* update the context to remove the current layer; this does not modify
    ** any register settings
    */
    if (PMC_SUCCESS == ret_code)
    {
        ret_code = odu_rfrm_update_pm_tcm_ctxt(odu_rfrm_handle,
                                               chnl_id,
                                               pm_tcm_num,
                                               cur_parent_tcm_num,
                                               cur_tcm_mode,
                                               FALSE,
                                               cur_tcm_pool_id);
    }

    /* update the context to reconnect the current layer to the new parent layer */
    if (PMC_SUCCESS == ret_code)
    {
        ret_code = odu_rfrm_update_pm_tcm_ctxt(odu_rfrm_handle,
                                               chnl_id,
                                               pm_tcm_num,
                                               new_parent_tcm_num,
                                               cur_tcm_mode,
                                               TRUE,
                                               cur_tcm_pool_id);
    }

    if (PMC_SUCCESS == ret_code)
    {
        odu_rfrm_field_PARENT_POOL_ID_set(NULL, odu_rfrm_handle, cur_tcm_pool_id,  new_parent_pool_id);
    }

    PMC_RETURN(ret_code);

} /* odu_rfrm_pm_tcmi_parent_update */

/*******************************************************************************
*  odu_rfrm_update_pm_tcm_ctxt
* ______________________________________________________________________________
*
* DESCRIPTION:
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - channel ID to request TCM layer information on
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM (must set float_tcmi_src) \n
*                         8         -- PM NIM (non-intrusive monitoring)
*   parent_id           - Parent layer to provide the sources of CI_SSF in the
*                         event of trail signal fail consequential action.
*                         See odu_rfrm_pm_tcm_id_t for legal values. \n
*                         Always set to ODU_RFRM_MESSAGE_DONT_CARE when 'mode'
*                         set to 0. Always set to ODU_RFRM_MESSAGE_TYPE_DCI if
*                         the layer is the first layer being provisioned for
*                         the channel.
*   mode                - See odu_rfrm_pm_tcm_mode_t.  Set the mode
*                         of the TCM layer
*   prov_action         - TRUE : provision the PM/TMC/floating TCM/PM-NIM
*                         FALSE : deprovision the PM/TMC/floating TCM/PM-NIM
*   tcm_pool_id         - Pool of the TCM/floating tcm/PM-NIM
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
PRIVATE PMC_ERROR odu_rfrm_update_pm_tcm_ctxt(odu_rfrm_handle_t *odu_rfrm_handle,
                                             UINT32 chnl_id,
                                             odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                             odu_rfrm_pm_tcm_id_t parent_id,
                                             odu_rfrm_pm_tcm_mode_t mode,
                                             BOOL8 prov_action,
                                             UINT32 tcm_pool_id)
{

/* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
   
    UINT32 i;
    INT8 j;
    UINT32 insert_index = 0;
    UINT32 remove_index = 0;

    odu_rfrm_layer_struct_t tcm_layers_old[ODU_SINK_MAX_PM_TCM_LAYERS];
    UINT32 num_layers_old = 0;

    BOOL8 post_nim_found = FALSE;

    PMC_ENTRY();

    /*get the existing rfrm layer_hierachy */
    /* returns TCMs/PM-NIM/and PM*/
    result = odu_rfrm_layer_hierarchy_get(odu_rfrm_handle,
                                          chnl_id,
                                          &tcm_layers_old,
                                          &num_layers_old);


    if(mode == ODU_RFRM_PM_TCM_TT_ERASE)
    {
        insert_index = 0;
        
        /*erase TCMs must be inserted last in the heirarchy but before PM layer*/
        for(j=0;j<(ODU_SINK_MAX_PM_TCM_LAYERS-1);j++)
        {
            if(prov_action == 1)
            {
                if(tcm_layers_old[j].pm_tcm_num != ODU_RFRM_MESSAGE_DONT_CARE &&
                   tcm_layers_old[j].pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM)
                {
                    insert_index = j+1;
                }
            } 
            else if(tcm_layers_old[j].pm_tcm_num == pm_tcm_num)
            {
                remove_index = j;
                break;
            }
        }
    }
    else
    {
        /*erase can't use this because erase has to ignore the parent*/
        if(parent_id == ODU_RFRM_MESSAGE_TYPE_DCI && prov_action == TRUE)
        {
            for(j=0;j<ODU_SINK_MAX_PM_TCM_LAYERS;j++)
            {
                /*have to insert after NIMs.  NIMs must stay connected to their parent. 
                The exception is in PM layer which is always the last layer */
                if( ((tcm_layers_old[j].tcm_mode != ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR)
                    || (tcm_layers_old[j].pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM))
                    && post_nim_found == FALSE)
                {
                    insert_index = j;
                    post_nim_found = TRUE;
                    break;
                }
            }
        }
        else
        {
            insert_index = 0;

            if(num_layers_old > 0){

                /* find of the index where the TCM/PM/PM-NIM is getting inserted into */
                for ( i = 0; i< ODU_SINK_MAX_PM_TCM_LAYERS ; i++)
                {
                    if(prov_action == 1)
                    {   
                        if(tcm_layers_old[i].pm_tcm_num == parent_id)
                        {
                            if(tcm_layers_old[i].tcm_mode == ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR ||
                               tcm_layers_old[i].tcm_mode == ODU_RFRM_PM_TCM_TT_ERASE)
                            {
                                  PMC_RETURN(ODU_RFRM_PARENT_LAYER_ERR);
                            }

                            /*have to insert after NIMs.  NIMs must stay connected to their parent. 
                            The exception is in PM layer which is always the last layer */
                            for(j=(i+1); j<ODU_SINK_MAX_PM_TCM_LAYERS && 
                                  post_nim_found == FALSE;j++)
                            {
                                if( ((tcm_layers_old[j].tcm_mode != ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR)
                                     || (tcm_layers_old[j].pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)))

                                {
                                    insert_index = j;
                                    post_nim_found = TRUE;
                                    break;
                                }                        
                            }
                        }
                    }
                    else if(tcm_layers_old[i].pm_tcm_num == pm_tcm_num)
                    {
                        remove_index = i;
                        break;
                    }


                }

                /*if inserting */
                if(prov_action == TRUE)
                {
                    if(insert_index == 0)
                    {
                          PMC_RETURN(ODU_RFRM_PARENT_LAYER_ERR);
                    }
                }
            }

        }

    }

    if(result == PMC_SUCCESS)
    {
        if(prov_action)
        {
            if (pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM)
            {
                /*prov for TCM, floating TCM and PM-NIM */
  
                /* set the context */
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].assign_state = TRUE;
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num = pm_tcm_num;
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_mode = mode;
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].chnl_id = chnl_id;

                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].child_pool_id = tcm_layers_old[insert_index].pool_id;
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].child_pm_tcm_num = tcm_layers_old[insert_index].pm_tcm_num;
                    
                
                if(parent_id == ODU_RFRM_MESSAGE_TYPE_DCI && insert_index == 0)
                {
                    /* Set DCI's new child.  Do not change DCI child if NIMs are provisioned at a higher layer*/
                    odu_rfrm_handle->var.dci_pm_tcm_child[chnl_id] = pm_tcm_num;
                }
                   

                if(insert_index>0)
                {
                    /* update higher layer's child information */
                    odu_rfrm_handle->var.tcm_pool_ptr[tcm_layers_old[insert_index-1].pool_id].child_pool_id = tcm_pool_id;
                    odu_rfrm_handle->var.tcm_pool_ptr[tcm_layers_old[insert_index-1].pool_id].child_pm_tcm_num = pm_tcm_num;

                }

            } 
            else {

                if(parent_id == ODU_RFRM_MESSAGE_TYPE_DCI && insert_index == 0)
                {
                    /* Set DCI's new child.  Do not change DCI child if NIMs are provisioned at a higher layer*/
                    odu_rfrm_handle->var.dci_pm_tcm_child[chnl_id] = ODU_RFRM_MESSAGE_TYPE_PM;
                }

                /*PM layer is always last*/
                odu_rfrm_handle->var.pm_provisioned[chnl_id] = TRUE;
                odu_rfrm_handle->var.pm_mode[chnl_id] = mode;

                if (0 != num_layers_old)
                {
                    odu_rfrm_handle->var.tcm_pool_ptr[tcm_layers_old[num_layers_old-1].pool_id].child_pm_tcm_num = ODU_RFRM_MESSAGE_TYPE_PM;
                }        
            }
        }
        else
        {
            /* if deprov action */
            if (pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM)
            {

                 /*deprov prov for TCM, floating TCM and PM-NIM */
                 
                /* update dowm stream parent_tcm_num */

                
                if(num_layers_old > 1)
                {
                    /*if num_layers_old == 1, then we are removing the only existing layer.  No child PID considerations */

                    if(remove_index == 0)
                    {
                        
                        /* first layer is being removed.  Need to set new dci_pm_tcm_child context */
                        odu_rfrm_handle->var.dci_pm_tcm_child[chnl_id] = tcm_layers_old[1].pm_tcm_num;

                    }
                    else if (0 < remove_index &&
                             (tcm_layers_old[remove_index+1].pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM ||
                              tcm_layers_old[remove_index+1].pm_tcm_num == ODU_RFRM_MESSAGE_DONT_CARE))
                    {

                        /*if removing the last layer */
                        odu_rfrm_handle->var.tcm_pool_ptr[tcm_layers_old[remove_index-1].pool_id].child_pool_id =  odu_rfrm_handle->var.tcm_pool_ptr[tcm_layers_old[remove_index].pool_id].child_pool_id;
                        odu_rfrm_handle->var.tcm_pool_ptr[tcm_layers_old[remove_index-1].pool_id].child_pm_tcm_num =  odu_rfrm_handle->var.tcm_pool_ptr[tcm_layers_old[remove_index].pool_id].child_pm_tcm_num;

                    }
                    else 
                    {
                        /* will only get here if num_layers_old > 2, and if layer is in the "middle" of the heirarchy (other two cases will look after it) */
                        odu_rfrm_handle->var.tcm_pool_ptr[tcm_layers_old[remove_index-1].pool_id].child_pool_id =  odu_rfrm_handle->var.tcm_pool_ptr[tcm_layers_old[remove_index].pool_id].child_pool_id;
                        odu_rfrm_handle->var.tcm_pool_ptr[tcm_layers_old[remove_index-1].pool_id].child_pm_tcm_num =  odu_rfrm_handle->var.tcm_pool_ptr[tcm_layers_old[remove_index].pool_id].child_pm_tcm_num;
                    }

                }
                else
                {
                    /* num_layers_old = 1. Last layer removed.  Unset DCI child */
                    odu_rfrm_handle->var.dci_pm_tcm_child[chnl_id] = ODU_RFRM_MESSAGE_DONT_CARE;
                }


                /*clean up context */

                /* set the context back to default state */
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].assign_state = FALSE;
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num = ODU_RFRM_MESSAGE_DONT_CARE;
                /* Needs to go back to start state.  Transparent state is used for maint sig insertion*/
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_mode = ODU_RFRM_PM_TCM_START;

                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].child_pool_id = ODU_RFRM_UNASSIGNED_POOL_ID;
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].child_pm_tcm_num = ODU_RFRM_MESSAGE_DONT_CARE;
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].chnl_id = 0xFF; 
                    
                /*odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].parent_tcm_num = ODU_RFRM_MESSAGE_DONT_CARE;*/

               
            } else {
                
                 /*PM layer is always last*/
                 odu_rfrm_handle->var.pm_provisioned[chnl_id] = FALSE;
                 odu_rfrm_handle->var.pm_mode[chnl_id] = ODU_RFRM_PM_TCM_START;


                 if(num_layers_old > 1)
                 {
                    odu_rfrm_handle->var.tcm_pool_ptr[tcm_layers_old[num_layers_old-2].pool_id].child_pm_tcm_num = ODU_RFRM_MESSAGE_DONT_CARE;
                    odu_rfrm_handle->var.tcm_pool_ptr[tcm_layers_old[num_layers_old-2].pool_id].child_pool_id = ODU_RFRM_UNASSIGNED_POOL_ID;
                 }
                 else
                 {
                    /* last layer removed.  Unset DCI child */
                    odu_rfrm_handle->var.dci_pm_tcm_child[chnl_id] = ODU_RFRM_MESSAGE_DONT_CARE;
                 }
            }
        }    
    } /* end of if(result == PMC_SUCCESS) */


    PMC_RETURN(result);


} /* odu_rfrm_update_pm_tcm_ctxt */


/*******************************************************************************
*  odu_rfrm_validate_pm_tcm_params
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API validates that the parameter given for RX TCM prov.  This
*   must be called before any call that influences the context is called. Note, 
*   it is imperative that all error checking is done up-front as to not disrupt
*   the context.
*       For provisioning:
*          - not trying to provision already consumed resource
*          - if pool_assign_mode = 1, make sure not supplying an already used PID
*          - Can only connect to an opperational parent (parent must
*            be operational and can't be programmed as a NIM
*          - Can't connect to PM parent
*       For deprov:
*          - must be trying to deprov an already existing TCM
*
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - channel ID to request TCM layer information on
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM (must set float_tcmi_src) \n
*                         8         -- PM NIM (non-intrusive monitoring)
*   parent_id           - Parent layer to provide the sources of CI_SSF in the
*                         event of trail signal fail consequential action.
*                         See odu_rfrm_pm_tcm_id_t for legal values. \n
*                         Always set to ODU_RFRM_MESSAGE_DONT_CARE when 'mode'
*                         set to 0. Always set to ODU_RFRM_MESSAGE_TYPE_DCI if
*                         the layer is the first layer being provisioned for
*                         the channel.
*   mode                - See odu_rfrm_pm_tcm_mode_t.  Set the mode
*                         of the TCM layer
*   prov_action         - TRUE : provision the PM/TMC/floating TCM/PM-NIM
*                         FALSE : deprovision the PM/TMC/floating TCM/PM-NIM
*   tcm_pool_id         - Pool of the TCM/floating tcm/PM-NIM
*   assign_mode         - 0 : SW assigns the pool id
*                         1 : user provides the pool id
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
PUBLIC PMC_ERROR odu_rfrm_validate_pm_tcm_params(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id,
                                                 odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                 odu_rfrm_pm_tcm_id_t parent_id,
                                                 odu_rfrm_pm_tcm_mode_t mode,
                                                 BOOL8 prov_action,
                                                 UINT8 tcm_pool_id,
                                                 UINT8 assign_mode)
{

/* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
   
    UINT32 i;
    UINT32 insert_index = 0;
    UINT32 remove_index = 0xFFFFFFFF;
    BOOL8 valid = 0;

    odu_rfrm_layer_struct_t tcm_layers_old[ODU_SINK_MAX_PM_TCM_LAYERS];
    UINT32 num_layers_old = 0;

    PMC_ENTRY();

    /*perform checking on the pool id*/
    if(assign_mode == 1 && prov_action == TRUE )
    {
        valid = odu_rfrm_tcm_pool_id_check(odu_rfrm_handle, tcm_pool_id,
                                           chnl_id, pm_tcm_num);
        
        if (valid == 1)
        {
            PMC_LOG_TRACE("%s, %s, %d, tcm_pool_id already assigned = %u...\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);
            PMC_RETURN(ODU_RFRM_LOG_CODE_POOL_ENTRY_UNAVAIL);
        }
    }

    /*get the existing rfrm layer_hierachy */
    /* returns TCMs/PM-NIM/and PM*/
    result = odu_rfrm_layer_hierarchy_get(odu_rfrm_handle,
                                          chnl_id,
                                          &tcm_layers_old,
                                          &num_layers_old);

    if (PMC_SUCCESS == result)
    {
        if(num_layers_old == ODU_SINK_MAX_PM_TCM_LAYERS && prov_action == TRUE)
        {
            PMC_RETURN(TCM_PM_PM_NIM_FULL);
        
        } 
        else if(num_layers_old == 0 && prov_action == FALSE)
        {
            PMC_RETURN(TCM_PM_PM_NIM_EMPTY);    
        }
    }

    if (PMC_SUCCESS == result)
    {
        if(parent_id == ODU_RFRM_MESSAGE_TYPE_DCI && prov_action == TRUE)
        {
            insert_index = 0;
        }
        else
        {
            insert_index = 0;
        
            if(num_layers_old > 0){
        
                /* find of the index where the TCM/PM/PM-NIM is getting inserted into */
                for ( i = 0; i< ODU_SINK_MAX_PM_TCM_LAYERS ; i++)
                {
                    if(tcm_layers_old[i].pm_tcm_num == parent_id)
                    {
                        insert_index = i+1;
                    }
        
        
                    
                    if(tcm_layers_old[i].pm_tcm_num == pm_tcm_num)
                    {
                        if(prov_action == TRUE)
                        {
                            /* throw an error if trying to provision the same resource
                            more than once */
                            PMC_RETURN(TCM_PM_PM_NIM_ALREADY_PROVISIONED);
                        }
                        else
                        {
                            remove_index = i;
                        }
                    }
        
        
                }
        
                /*if inserting */
                if(prov_action == TRUE)
                {
                    if(insert_index == 0)
                    {
                          /* This error is thrown if the parent layer provided isn't provisioned*/
                          PMC_RETURN(ODU_RFRM_PARENT_LAYER_ERR);
                    }
                    else if( tcm_layers_old[insert_index-1].tcm_mode == ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR ||
                            tcm_layers_old[insert_index-1].tcm_mode ==  ODU_RFRM_PM_TCM_TT_ERASE)
                    {
                        /*it's illegal to connect to a nim */
                        PMC_RETURN(ODU_RFRM_PARENT_LAYER_ERR);
                    }
                }
                else
                {
                    if(remove_index == 0xFFFFFFFF)
                    {
                        /*if remove index is still equal to it's initialized value, then
                          this resource can not be deprovisioned because the resource
                          was never provisioned in the first place*/
                        PMC_RETURN(TCM_PM_PM_NIM_ALREADY_PROVISIONED);
                    }
        
                }
            }
        
        }
    }

    PMC_RETURN(result);


} /* odu_rfrm_update_pm_tcm_ctxt */


/*******************************************************************************
* odu_rfrm_pm_tcmi_defect_ctrl_mode_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures consequential action propagation and detection mode
*   based on the operational mode of the layer being provisioned.
*
*   Prior to calling this API, the layer must have been configured with
*   odu_rfrm_pm_tcmi_cfg() API.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM (must set float_tcmi_src) \n
*                         8         -- PM NIM (non-intrusive monitoring)
*   mode                - functional mode of layer sink function. See
*                         odu_rfrm_pm_tcm_mode_t for valid options. \n
*                         0 -- Transparent (termination) \n
*                         1 -- Monitor (termination) \n
*                         2 -- Operational (termination) \n
*                         3 -- Non-intrusive monitoring
*   tcm_pool_id         - Pool id of the TCM/floating TCM/PM-NIM
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
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_defect_ctrl_mode_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                                       UINT32 chnl_id,
                                                       odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                       odu_rfrm_pm_tcm_mode_t mode,
                                                       UINT32 tcm_pool_id)
{
    /* variable declaration */
    UINT32 ais_dis;
    UINT32 tsd_dis;
    UINT32 tsf_dis;
    UINT32 bw_dis;

    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);

    /* argument checking */
    if (chnl_id > 95 || pm_tcm_num > LAST_ODU_RFRM_MESSAGE_TYPE
        || mode >= ODU_RFRM_PM_TCM_LAST)
    {
        PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }
    
    PMC_LOG_TRACE("pm_tcm_num = %u\n", pm_tcm_num);
    
    /* configure for each operating mode */
    switch (mode)
    {
         case ODU_RFRM_PM_TCM_TT_OPERATIONAL:
            ais_dis = 0;
            bw_dis = 0;
            tsd_dis = 0;
            tsf_dis = 0;
            break;
         case ODU_RFRM_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED:
            ais_dis = 0;
            bw_dis = 0;
            tsd_dis = 0;
            tsf_dis = 0;
            break;
        case ODU_RFRM_PM_TCM_TT_MONITOR:
            ais_dis = 1;
            bw_dis = 0;
            tsd_dis = 1;
            tsf_dis = 1;
            break;
       case ODU_RFRM_PM_TCM_TT_TRANSPARENT:
            ais_dis = 0;
            bw_dis = 1;
            tsd_dis = 0;
            tsf_dis = 0;
            break;
       case ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR:
       case ODU_RFRM_PM_TCM_TT_ERASE:
            ais_dis = 1;
            bw_dis = 1;
            tsd_dis = 0;
            tsf_dis = 0;
            break;
       default:
            PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }

    if (PMC_SUCCESS == ret_val)
    {
        if (pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM)
        {
            /* set AIS_DIS */
            ret_val = odu_rfrm_pm_tcmi_ais_disable(odu_rfrm_handle, 
                                                   chnl_id, 
                                                   pm_tcm_num, 
                                                   ais_dis,
                                                   tcm_pool_id);
        }                                                
    }
    
    if (PMC_SUCCESS == ret_val)
    {
        /* set BW_DIS */
        ret_val = odu_rfrm_pm_tcmi_bw_act_cfg(odu_rfrm_handle, 
                                              chnl_id, 
                                              pm_tcm_num, 
                                              bw_dis,
                                              tcm_pool_id);
    }
    
    if (PMC_SUCCESS == ret_val)
    {
        /* set TSD_DIS and TSF_DIS */
        ret_val = odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg(odu_rfrm_handle, 
                                                    chnl_id, 
                                                    pm_tcm_num, 
                                                    tcm_pool_id,
                                                    2, 
                                                    tsd_dis, 
                                                    tsf_dis);
    }


    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_tcmi_defect_ctrl_mode_cfg */

/*******************************************************************************
* odu_rfrm_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function activates or deactivates the provisioned layer.
*
*   Prior to calling this API, the layer must have been configured with
*   odu_rfrm_pm_tcmi_cfg() API.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   provision_pm        - Use this to provision the PM layer if this is
*                         a path terminated node.
*                         TRUE : provision the PM layer
*                         FALSE : do not provision the PM layer
*   enbl                - Enables or disables the ODUk channel \n
*                         0 -- disabled; in passthru \n
*                         1 -- enabled; the ODUk channel is operational.
*   fau_enbl            - Enables or disables the FAU \n
*                         0 -- disabled; in passthru \n
*                         1 -- enabled; the FAU is out of passthru
*   deprov_configs     - TRUE : deprov all RFRM configrations that have 
*                               been set through run-tim APIs (TCMs, etc)
*                        FALSE : deprov RFRM configurations
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
PUBLIC PMC_ERROR odu_rfrm_chnl_prov(odu_rfrm_handle_t *odu_rfrm_handle,
                                    UINT32 chnl_id,
                                    BOOL8 provision_pm,
                                    UINT32 enbl, 
                                    UINT32 fau_enbl,
                                    BOOL deprov_configs)
{
    odu_rfrm_layer_struct_t odu_rfrm_layers[ODU_SINK_MAX_PM_TCM_LAYERS];
    UINT32 num_layers;

    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    if (enbl == 1)
    {
        /*provision PM-layer or PM-NIM layer */
        
        if(PMC_SUCCESS == result && provision_pm == TRUE )
        {
            
            result = odu_rfrm_pm_tcmi_cfg(odu_rfrm_handle,
                                          chnl_id,
                                          ODU_RFRM_MESSAGE_TYPE_PM,
                                          0,
                                          ODU_RFRM_MESSAGE_TYPE_DCI,
                                          ODU_RFRM_PM_TCM_TT_OPERATIONAL,
                                          TRUE);

            /* get the new rfrm layer_hierachy */
            if(result == PMC_SUCCESS)
            {
                result = odu_rfrm_layer_hierarchy_get(odu_rfrm_handle,
                                                      chnl_id,
                                                      &odu_rfrm_layers,
                                                      &num_layers);
            }
        
            /*call function to set last_layer */
            if (result == PMC_SUCCESS)
            {
                result = odu_rfrm_last_layer_last_is_dci_update(odu_rfrm_handle,
                                                                chnl_id,
                                                                odu_rfrm_layers,
                                                                num_layers);
            }     

        }
    } else 
    {
        odu_rfrm_internal_chnl_clean(odu_rfrm_handle,
                                     chnl_id,
                                     deprov_configs);
    }

                                     
    PMC_RETURN(result);
} /* odu_rfrm_chnl_prov */
      
/*******************************************************************************
* odu_rfrm_tcmi_bw_act_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the generation of backward consequential actions
*   for TCM1 to TCM6 or Floating TCM layer.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM1 \n
*                         8         -- PM NIM (non-intrusive monitoring)
*   bw_dis              - Controls whether or not backward consequential actions
*                         will be detected and processed. \n
*                         0 -- aBDI, aBEI and aBIAE consequential actions are
*                              detected and processed normally. \n
*                         1 -- aBDI, aBEI, and aBIAE consequential actions are
*                              disabled.
*   tcm_pool_id         - Pool of the TCM/floating TCM/PM-NIM
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
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_bw_act_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                             UINT32 chnl_id,
                                             odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                             UINT8 bw_dis,
                                             UINT32 tcm_pool_id)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* argument checking */
    if (pm_tcm_num >= LAST_ODU_RFRM_MESSAGE_TYPE) {
        PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }

    /* BW_DIS */
    if (pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM)
    {
        odu_rfrm_field_BW_DIS_set(NULL, odu_rfrm_handle, tcm_pool_id, bw_dis);
    }
    else
    {
        odu_rfrm_field_BW_DIS_PM_set(NULL, odu_rfrm_handle, chnl_id, bw_dis);
    }

    PMC_RETURN(ret_val);
} /* odu_rfrm_pm_tcmi_bw_act_cfg */

/*******************************************************************************
* odu_rfrm_pm_tcmi_bw_act_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the configuration of backward consequential action
*   control for TCM1 to TCM6 or Floating TCM layer.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM1 \n
*                         8         -- PM NIM (non-intrusive monitoring)
*
* OUTPUTS:
*   *bw_dis             - extracted BW_DIS value
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_bw_act_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                             UINT32 chnl_id,
                                             odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                             UINT8 *bw_dis)
{
    /* variable declaration */
    UINT32 pool_id;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    /* argument checking */
    if (pm_tcm_num >= LAST_ODU_RFRM_MESSAGE_TYPE) 
    {
        result = ODU_RFRM_LOG_ERR_INVALID_PARAMETERS;
    }
    else if (pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM)
    {
        /* BW_DIS */
        /* find tcm pool id */
        result = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, pm_tcm_num, &pool_id);

        if (PMC_SUCCESS == result)
        {
            *bw_dis = odu_rfrm_field_BW_DIS_get(NULL, odu_rfrm_handle, pool_id);
        }
    }
    else
    {
        *bw_dis = odu_rfrm_field_BW_DIS_PM_get(NULL, odu_rfrm_handle, chnl_id);
    }

    PMC_RETURN(result);
} /* odu_rfrm_pm_tcmi_bw_act_get */


/*******************************************************************************
* odu_rfrm_pm_tcmi_deg_th_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures defect errored block count threshold for 
*   detecting dDEG.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM1 \n
*                         8         -- PM NIM (non-intrusive monitoring)
*   deg_thr             - degraded defect one second errored block count
*                         threshold
*                         
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
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_deg_th_set(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id,
                                                 odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                 UINT32 deg_thr)
{
    /* variable declaration */
    UINT32 pool_id;
    PMC_ERROR ret_val;

    PMC_ENTRY();

    /* deg_thr is a maximum 20 bit value */
    if(deg_thr > 1048575){
        PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }

    if (pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM)
    {
        /* for TCM layer, find TCM POOL ID */
        ret_val = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id,
                                              pm_tcm_num, &pool_id);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
        odu_rfrm_field_DEG_THR_set(NULL, odu_rfrm_handle, pool_id, deg_thr);
    }
    else
    {
        odu_rfrm_field_DEG_THR_PM_set(NULL, odu_rfrm_handle, chnl_id, deg_thr);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_tcmi_deg_th_set */

/*******************************************************************************
* odu_rfrm_pm_tcmi_deg_th_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures defect errored block count threshold for 
*   detecting dDEG.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM1 \n
*                         8         -- PM NIM (non-intrusive monitoring)
*
* OUTPUTS:
*   deg_thr             - degraded defect one second errored block count
*                         threshold
*                         
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_deg_th_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id,
                                                 odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                 UINT32 *deg_thr)
{
    /* variable declaration */
    UINT32 pool_id;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM)
    {
        /* for TCM layer, find TCM POOL ID */
        ret_val = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, pm_tcm_num, &pool_id);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
        *deg_thr = odu_rfrm_field_DEG_THR_get(NULL, odu_rfrm_handle, pool_id);
    }
    else
    {
        *deg_thr = odu_rfrm_field_DEG_THR_PM_get(NULL, odu_rfrm_handle, chnl_id);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_tcmi_deg_th_get */

/*******************************************************************************
* odu_rfrm_pm_tcmi_deg_m_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the number of degraded defect monitoring intervals
*   for detecting dDEG.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM1 \n
*                         8         -- PM NIM (non-intrusive monitoring)
*   deg_m               - The number of degraded one second intervals
*                         required to detect dDEG
*                         Valid range: 2-10
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
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_deg_m_set(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id,
                                                 odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                 UINT8 deg_m)
{
    /* variable declaration */
    UINT32 pool_id;
    PMC_ERROR ret_val;

    PMC_ENTRY();

    if (pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM)
    {
        /* for TCM layer, find TCM POOL ID */
        ret_val = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, pm_tcm_num, &pool_id);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
        odu_rfrm_field_DEG_M_set(NULL, odu_rfrm_handle, pool_id, deg_m);
    }
    else
    {
        odu_rfrm_field_DEG_M_PM_set(NULL, odu_rfrm_handle, chnl_id, deg_m);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_tcmi_deg_m_set */

/*******************************************************************************
* odu_rfrm_pm_tcmi_deg_m_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the number of degraded defect monitoring intervals
*   for detecting dDEG.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM1 \n
*                         8         -- PM NIM (non-intrusive monitoring)
*
* OUTPUTS:
*   deg_m               - The number of degraded one second intervals
*                         required to detect dDEG
*                         Valid range: 2-10
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_deg_m_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id,
                                                 odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                 UINT8 *deg_m)
{
    /* variable declaration */
    UINT32 pool_id;
    PMC_ERROR ret_val;

    PMC_ENTRY();

    if (pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM)
    {
        /* for TCM layer, find TCM POOL ID */
        ret_val = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, pm_tcm_num, &pool_id);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
        *deg_m = odu_rfrm_field_DEG_M_get(NULL, odu_rfrm_handle, pool_id);
    }
    else
    {
        *deg_m = odu_rfrm_field_DEG_M_PM_get(NULL, odu_rfrm_handle, chnl_id);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_tcmi_deg_m_get */

/*******************************************************************************
* odu_rfrm_action_proc_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function controls whether or not a defect will contribute to its
*   corresponding consequential action declarations.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   dLTC_dis            - Controls whether or not dLTC defect contributes to
*                         the aBDI consequential action detection\n
*                         0 -- dLTC defect contributes to consequential
*                              action declaration. \n
*                         1 -- dLTC defect does not contribute to
*                             consequential action declaration.
*   dAIS_dis            - Controls whether or not dAIS defect contributes to
*                         declaration of consequential action \n
*                         0 -- dAIS defect contributes to consequential
*                              action declaration. \n
*                         1 -- dAIS defect does not contribute to aAIS
*                              consequential action declaration.
*   dOCI_dis            - Controls whether or not dOCI defect contributes to
*                         declaration of consequential action \n
*                         0 -- dOCI defect contributes to consequential
*                              action declaration. \n
*                         1 -- dOCI defect does not contribute to
*                              consequential action declaration.
*   dLCK_dis            - Controls whether or not dLCK defect contributes to
*                         declaration of AIS consequential action \n
*                         0 -- dLCK defect contributes to consequential
*                              action declaration. \n
*                         1 -- dLCK defect does not contribute to
*                              consequential action declaration.
*   wr_mask             - Mask to control which bits get written
*                         Bit 0 -- write dLTC_ACT_DIS bit
*                         Bit 1 -- write dAIS_ACT_DIS bit
*                         Bit 2 -- write dOCI_ACT_DIS bit
*                         Bit 3 -- write dLCK_ACT_DIS bit
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
PUBLIC PMC_ERROR odu_rfrm_action_proc_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                          UINT32 dLTC_dis, UINT32 dAIS_dis,
                                          UINT32 dOCI_dis, UINT32 dLCK_dis,
                                          UINT8 wr_mask)
{
    UINT32 i;
    PMC_ENTRY();
    
    for (i = 0; i < 4; i++)
    {
        if ((wr_mask >> i) & 1)
        {
            if (0 == i) {
                odu_rfrm_field_DLTC_ACT_DIS_set(NULL, odu_rfrm_handle, dLTC_dis);
            }
            else if (1 == i) {
                odu_rfrm_field_DAIS_ACT_DIS_set(NULL, odu_rfrm_handle, dAIS_dis);
            }
            else if (2 == i) {
                odu_rfrm_field_DOCI_ACT_DIS_set(NULL, odu_rfrm_handle, dOCI_dis);
            }
            else {
                odu_rfrm_field_DLCK_ACT_DIS_set(NULL, odu_rfrm_handle, dLCK_dis);
            }
        }
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_action_proc_cfg */

/*******************************************************************************
* odu_rfrm_action_proc_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function controls whether or not a defect will contribute to its
*   corresponding consequential action declarations.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*
* OUTPUTS:
*   dLTC_dis            - Controls whether or not dLTC defect contributes to
*                         the aBDI consequential action detection\n
*                         0 -- dLTC defect contributes to consequential
*                              action declaration. \n
*                         1 -- dLTC defect does not contribute to
*                             consequential action declaration.
*   dAIS_dis            - Controls whether or not dAIS defect contributes to
*                         declaration of consequential action \n
*                         0 -- dAIS defect contributes to consequential
*                              action declaration. \n
*                         1 -- dAIS defect does not contribute to aAIS
*                              consequential action declaration.
*   dOCI_dis            - Controls whether or not dOCI defect contributes to
*                         declaration of consequential action \n
*                         0 -- dOCI defect contributes to consequential
*                              action declaration. \n
*                         1 -- dOCI defect does not contribute to
*                              consequential action declaration.
*   dLCK_dis            - Controls whether or not dLCK defect contributes to
*                         declaration of AIS consequential action \n
*                         0 -- dLCK defect contributes to consequential
*                              action declaration. \n
*                         1 -- dLCK defect does not contribute to
*                              consequential action declaration.
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_action_proc_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                          UINT32 *dLTC_dis, UINT32 *dAIS_dis,
                                          UINT32 *dOCI_dis, UINT32 *dLCK_dis)
{
    PMC_ENTRY();

    *dLTC_dis = odu_rfrm_field_DLTC_ACT_DIS_get(NULL, odu_rfrm_handle);
    *dAIS_dis = odu_rfrm_field_DAIS_ACT_DIS_get(NULL, odu_rfrm_handle);
    *dOCI_dis = odu_rfrm_field_DOCI_ACT_DIS_get(NULL, odu_rfrm_handle);
    *dLCK_dis = odu_rfrm_field_DLCK_ACT_DIS_get(NULL, odu_rfrm_handle);
    

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_action_proc_get */

/*******************************************************************************
* odu_rfrm_chnl_dloflom_act_dis_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the current control of dLOFLOM defect consequential
*   action.
*
* INPUTS:
*   *odu_rfrm_handle   - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id            - Channel ID to be processed
*                        Valid range: 0 - 95
*
* OUTPUTS:
*   dloflom_dis        - Controls whether or not dLOFLOM defect causes nominal 
*                        'ODUk-AIS' to be inserted in the channel. \n
*                        0 -- dLOFLOM defect causes nominal rate 'ODUk-AIS' to be
*                             inserted in the channel. \n
*                        1 -- dLOFLOM defect does not cause nominal rate
*                             'ODUk-AIS' to be inserted in the channel.
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_chnl_dloflom_act_dis_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                   UINT32 chnl_id, BOOL8 *dloflom_dis)
{
    PMC_ENTRY();

    *dloflom_dis = odu_rfrm_field_LOFLOM_ACT_DIS_get(NULL, odu_rfrm_handle, chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_chnl_dloflom_act_dis_get */

/*******************************************************************************
* odu_rfrm_chnl_ins_ais_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures insertion of ODUk-AIS in the channel when loss of
*   frame of loss of multi-frame event occurs.
*
* INPUTS:
*   *odu_rfrm_handle   - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id            - Channel ID to be processed
*                        Valid range: 0 - 95
*   dloflom_dis        - Controls whether or not dLOFLOM defect causes nominal 
*                        'ODUk-AIS' to be inserted in the channel. \n
*                        0 -- dLOFLOM defect causes nominal rate 'ODUk-AIS' to be
*                             inserted in the channel. \n
*                        1 -- dLOFLOM defect does not cause nominal rate
*                             'ODUk-AIS' to be inserted in the channel.
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
PUBLIC PMC_ERROR odu_rfrm_chnl_ins_ais_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                           UINT32 chnl_id, BOOL8 dloflom_dis)
{
    PMC_ENTRY();

    odu_rfrm_field_LOFLOM_ACT_DIS_set(NULL, odu_rfrm_handle, chnl_id,
                                      dloflom_dis);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_chnl_ins_ais_cfg */

/*******************************************************************************
* odu_rfrm_pm_tcmi_ais_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures detection and generation of aAIS consequential
*   action on a TCMi or PM_NIM layer.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - TCMi field instance: \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM \n
*                         8         -- PM NIM
*   ais_dis             - Controls processing of aAIS consequential action. \n
*                         1 -- aAIS consequential action is disabled. \n
*                         0 -- aAIS consequential action is detected and
*                              processed normally.
*   tcm_pool_id         - TCM pool ID
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
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_ais_disable(odu_rfrm_handle_t *odu_rfrm_handle,
                                              UINT32 chnl_id,
                                              odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                              UINT8 ais_dis,
                                              UINT32 tcm_pool_id)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
    {
        result = ODU_RFRM_LOG_ERR_INVALID_PARAMETERS;
    }
    else
    {
        BOOL8 valid = odu_rfrm_tcm_pool_id_check(odu_rfrm_handle, 
                                                 tcm_pool_id,
                                                 chnl_id, pm_tcm_num);
        if (TRUE == valid)
        {
            odu_rfrm_field_AIS_DIS_set(NULL, odu_rfrm_handle, tcm_pool_id, ais_dis);
        }
        else
        {
            result = ODU_RFRM_LOG_CODE_POOL_ID_UNASSIGNED;
        }
    }

    PMC_RETURN(result);
} /* odu_rfrm_pm_tcmi_ais_disable */


/*******************************************************************************
* odu_rfrm_pm_tcmi_ais_dis_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures detection and generation of aAIS consequential
*   action.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - TCMi field instance: \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM \n
*                         8         -- PM NIM
*
* OUTPUTS:
*   *ais_dis            - current configuration for processing of aAIS 
*                         consequential action. \n
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_ais_dis_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                              UINT32 chnl_id,
                                              odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                              UINT8 *ais_dis)
{
    /* variable declaration */
    UINT32 tcm_pool_id;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    /* search TCM POOL ID */
    result = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);
    if (PMC_SUCCESS == result)
    {
        *ais_dis = odu_rfrm_field_AIS_DIS_get(NULL, odu_rfrm_handle, tcm_pool_id);
    }

    PMC_RETURN(result);
} /* odu_rfrm_pm_tcmi_ais_dis_get */

/*******************************************************************************
* odu_rfrm_tcm_ebc_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures detection and generation of aAIS consequential
*   action.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance:\n
*                         1 to 6    -- TCM1 to TCM6\n
*
* OUTPUTS:
*   *ebc             - COREOTN ODUkt far/near end counters for a TCM layer of
*                      an ODUk channel
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_tcm_ebc_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                      UINT32 chnl_id,
                                      odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                      odu_rfrm_odukt_counters_t *ebc)
{
    /* variable declaration */
    UINT32 tcm_pool_id;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    /* search TCM POOL ID */
    result = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);
    if (PMC_SUCCESS == result)
    {
        ebc->odukt_sk_piae = odu_rfrm_field_PIAE_get(NULL, odu_rfrm_handle, tcm_pool_id);
        ebc->odukt_sk_pn_ds = odu_rfrm_field_PN_DS_get(NULL, odu_rfrm_handle, tcm_pool_id);
        ebc->odukt_sk_pn_ebc = odu_rfrm_field_PN_EBC_get(NULL, odu_rfrm_handle, tcm_pool_id);
        ebc->odukt_sk_pbiae = odu_rfrm_field_PBIAE_get(NULL, odu_rfrm_handle, tcm_pool_id);
        ebc->odukt_sk_pf_ds = odu_rfrm_field_PF_DS_get(NULL, odu_rfrm_handle, tcm_pool_id);
        ebc->odukt_sk_pf_ebc = odu_rfrm_field_PF_EBC_get(NULL, odu_rfrm_handle, tcm_pool_id);
    }

    PMC_RETURN(result);
} /* odu_rfrm_tcm_ebc_get */

/*******************************************************************************
* odu_rfrm_tcmi_ltc_to_ais_tsf_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures if a loss of tandem connection defect will
*   contribute to the generation of AIS and TSF consequenctial action for a TCM 
*   layer, a floating TCM layer or a PM NIM layer.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   tcm_num             - TCMi field instance: \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM \n
*                         8         -- PM NIM
*   dLTC_dis            - Controls whether or not dLTC defect contributes to
*                         declaration of AIS and TSF consequential action \n
*                         0 -- dLTC defect contributes to the aAIS and aTSF 
*                              consequential action declaration. \n 
*                         1 -- dLTC defect does not contribute to aAIS and aTSF
*                              consequential action declaration.
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
PUBLIC PMC_ERROR odu_rfrm_tcmi_ltc_to_ais_tsf_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                                  UINT32 chnl_id,
                                                  odu_rfrm_pm_tcm_id_t tcm_num,
                                                  UINT32 dLTC_dis)
{
    /* variable declaration */
    UINT32 start_idx;
    UINT32 tcm_pool_id;
    PMC_ERROR result;

    PMC_ENTRY();

    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);

    /* search TCM POOL ID */
    result = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, tcm_num, &tcm_pool_id);
    if (PMC_SUCCESS == result)
    {
        start_idx = tcm_pool_id;

        /* Configure dLTC action */
        odu_rfrm_field_LTC_ACT_DIS_set(NULL, odu_rfrm_handle, start_idx, dLTC_dis);
    }

    PMC_RETURN(result);
} /* odu_rfrm_tcmi_ltc_to_ais_tsf_cfg */

/*******************************************************************************
* odu_rfrm_tcmi_ltc_to_ais_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the current mode for generation of AIS from dLTC
*   consequenctial action for a TCM layer, a floating TCM layer or a PM NIM layer.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   tcm_num             - TCMi field instance: \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM \n
*                         8         -- PM NIM
*
* OUTPUTS:
*   dLTC_dis            - mode of AIS generation from dLTC
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_tcmi_ltc_to_ais_tsf_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                  UINT32 chnl_id,
                                                  odu_rfrm_pm_tcm_id_t tcm_num,
                                                  BOOL8 *dLTC_dis)
{
    /* variable declaration */
    UINT32 start_idx;
    UINT32 tcm_pool_id;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);

    /* search TCM POOL ID */
    result = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, tcm_num, &tcm_pool_id);
    if (PMC_SUCCESS == result)
    {
        start_idx = tcm_pool_id;

        *dLTC_dis = odu_rfrm_field_LTC_ACT_DIS_get(NULL, odu_rfrm_handle, start_idx);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_tcmi_ltc_to_ais_get */

/*******************************************************************************
* odu_rfrm_pm_tcmi_tim_dis_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures if a trace identifier mismatch defect will
*   contribute to the generation of AIS consequenctial action for a TCM,
*   a floating TCM, a PM NIM or a PM layer.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM \n
*                         8         -- PM NIM
*   dTIM_dis            - Controls whether or not dTIM defect contributes to
*                         declaration of AIS consequential action \n
*                         0 -- dTIM defect contributes to the aAIS consequential
*                              action declaration. \n
*                         1 -- dTIM defect does not contribute to the aAIS
*                              consequential action declaration.
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
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_tim_dis_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                              UINT32 chnl_id,
                                              odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                              BOOL8 dTIM_dis)
{
    /* variable declaration */
    UINT32 start_idx;
    UINT32 tcm_pool_id;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);

    if (pm_tcm_num != 0)
    {
        /* search TCM POOL ID */
        result = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, 
                                         pm_tcm_num, &tcm_pool_id);
        if (PMC_SUCCESS == result)
        {
            start_idx = tcm_pool_id;
            /* Configure dTIM action */
            odu_rfrm_field_TIM_ACT_DIS_set(NULL, odu_rfrm_handle, start_idx, dTIM_dis);
        }
    }
    else
    {
        odu_rfrm_field_TIM_ACT_DIS_PM_set(NULL, odu_rfrm_handle, chnl_id, dTIM_dis);
    }

    PMC_RETURN(result);
} /* odu_rfrm_pm_tcmi_tim_dis_cfg */

/*******************************************************************************
* odu_rfrm_pm_tcmi_tim_dis_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the mode of generation of trace identifier mismatch 
*   defect.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM \n
*                         8         -- PM NIM
*
* OUTPUTS:
*   *dTIM_dis           - extracted mode of tim disable
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_tim_dis_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                              UINT32 chnl_id,
                                              odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                              BOOL8 *dTIM_dis)
{
    /* variable declaration */
    UINT32 tcm_pool_id;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);

    if (pm_tcm_num != 0)
    {
        /* search TCM POOL ID */
        result = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);

        if (PMC_SUCCESS == result)
        {
            /* Configure dTIM action */
            *dTIM_dis = odu_rfrm_field_TIM_ACT_DIS_get(NULL, odu_rfrm_handle, tcm_pool_id);
        }
    }
    else
    {
        *dTIM_dis = odu_rfrm_field_TIM_ACT_DIS_PM_get(NULL, odu_rfrm_handle, chnl_id);
    }

    PMC_RETURN(result);
} /* odu_rfrm_pm_tcmi_tim_dis_get */

/*******************************************************************************
*  odu_rfrm_pm_tcmi_tim_det_mode_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function configures the receive data path processing of the PM or TCMi
*   TTI message.  It configures how the TTI message is monitored and which
*   fields of the message contribute to TIM defect detection.
*
*   odu_rfrm_exp_tti_cfg() must be called to configure the expected TTI
*   message in order for the processing performed by these feature to be
*   meaningful.
*
*   This API is only valid for messages which are being processed (ie not in
*   transparent mode).
*
*  INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM \n
*                         8         -- PM NIM (non-intrusive monitoring)
*   tim_det_mode        - trail trace identifier mismatch detection mode \n
*                         0 -- dTIM defect detection is disabled. \n
*                         1 -- the expected SAPI field is used when determining
*                              the dTIM defect. \n
*                         2 -- the expected DAPI field is used when determining
*                              the dTIM defect. \n 
*                         3 -- both the exptected SAPI field and the expected
*                              DAPI field are used when determining the dTIM
*                              defect.
*
*  OUTPUTS:        None.
*
*  RETURNS:       
*   PMC_SUCCESS on success
*
******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_tim_det_mode_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                                   UINT32 chnl_id,
                                                   odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                   UINT32 tim_det_mode)
{
    /* variable declaration */
    UINT32 tcm_pool_id;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);

    /* Register 0xCC00 + 0x40*N(N=0:351): ODU_RFRM - LAYER_CONTROL0
        TIM_DET_MO */
    if (pm_tcm_num == 0)
    {
        odu_rfrm_field_TIM_DET_MO_PM_set(NULL, odu_rfrm_handle, chnl_id, tim_det_mode);
    }
    else
    {
        /* get the tcm pool id */
        result = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);

        if (PMC_SUCCESS == result)
        {
            odu_rfrm_field_TIM_DET_MO_set(NULL, odu_rfrm_handle, tcm_pool_id, tim_det_mode);
        }
    }

    PMC_RETURN(result);
} /* odu_rfrm_pm_tcmi_tim_det_mode_cfg */

/*******************************************************************************
*  odu_rfrm_pm_tcmi_tim_det_mode_ext
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function extracts TIM detection mode for processing of the PM or TCMi
*   TTI message. It configures how the TTI message is monitored and which
*   fields of the message contribute to TIM defect detection.
*
*  INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM \n
*                         8         -- PM NIM (non-intrusive monitoring)
*
*  OUTPUTS:       
*   *tim_det_mode       - trail trace identifier mismatch detection mode
*
*  RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR - If pm_tcm_num and chnl_id is not valid
*
******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_tim_det_mode_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                                   UINT32 chnl_id,
                                                   odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                   UINT8 *tim_det_mode)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 tcm_pool_id;
    
    PMC_ENTRY();
    
    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);

    /* Register 0xCC00 + 0x40*N(N=0:351): ODU_RFRM - LAYER_CONTROL0
        TIM_DET_MO */
    if (pm_tcm_num == 0)
    {
        *tim_det_mode = odu_rfrm_field_TIM_DET_MO_PM_get(NULL, odu_rfrm_handle, chnl_id);
    }
    else
    {
        /* get the tcm pool id */
        result = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);

        if (PMC_SUCCESS == result)
        {
            *tim_det_mode = odu_rfrm_field_TIM_DET_MO_get(NULL, odu_rfrm_handle, tcm_pool_id);
        }
    }
    
    PMC_RETURN(result);
    
} /* odu_rfrm_pm_tcmi_tim_det_mode_ext */

/*******************************************************************************
*  odu_rfrm_chnl_maint_sig_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function configures the forced insertion of ODUk maintenance signals
*   for the channel.
*
*  INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   force_alarm_sig     - ODUk Alarm Signal Control \n
*                         0 -- no alarm signal forced \n
*                         1 -- Force ODUK_AIS \n
*                         2 -- Force ODUK_OCI \n
*                         3 -- Force ODUK_LCK
*
*
*  OUTPUTS:
*
*  RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR   - maintenance signal was not provisioned successfully
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_chnl_maint_sig_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                             UINT32 chnl_id,
                                             UINT32 force_alarm_sig)
{
    /* variable decalration */
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_layer_struct_t rfrm_tcm_layers[ODU_SINK_MAX_PM_TCM_LAYERS];
    UINT32 num_rfrm_layers = 0;

    PMC_ENTRY();
    
    odu_rfrm_field_MAINT_SIG_set(NULL, odu_rfrm_handle, chnl_id, force_alarm_sig);

    result = odu_rfrm_layer_hierarchy_get(odu_rfrm_handle,
                                          chnl_id,
                                          &rfrm_tcm_layers,
                                          &num_rfrm_layers);    


    odu_rfrm_handle->var.maint_sig[chnl_id] = force_alarm_sig;

    /* update XX_REMOVE_OH bits based on new maintenance signal configuration*/
    if(result == PMC_SUCCESS)
    {

        result = odu_rfrm_remove_oh_update(odu_rfrm_handle,
                                           chnl_id);
    }


    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_chnl_maint_sig_cfg */

/*******************************************************************************
*  odu_rfrm_chnl_maint_sig_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function configures the forced insertion of ODUk maintenance signals
*   for the channel.
*
*  INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*
*
*  OUTPUTS:
*   *maint_sig          - extracted maintenace signal on the channel
*
*  RETURNS:
*
*******************************************************************************/
PUBLIC void odu_rfrm_chnl_maint_sig_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                             UINT32 chnl_id, 
                                             UINT8 *maint_sig)
{
    /* variable decalration */
    PMC_ENTRY();

    *maint_sig = odu_rfrm_handle->var.maint_sig[chnl_id];

    PMC_RETURN();
} /* odu_rfrm_chnl_maint_sig_get */

/*******************************************************************************
*  odu_rfrm_tcm_maint_sig_cfg
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function configures the forced insertion of ODUk maintenance signals
*   for a TCM/PM layer.  You may only provide operational TCMs to this API.
*   Note, when providing a TCM/PM to this API, the maintenance signal will be 
*   forced before processing sinking this TCM/PM (The maintenance signal will 
*   be seen for this TCM/PM and all downstream TCMs/PM depending on configuration)
*
*
*  INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi instance. See odu_rfrm_pm_tcm_id_t.
*                         Can provide:
*                           ODU_RFRM_MESSAGE_TYPE_PM 
*                           ODU_RFRM_MESSAGE_TYPE_TCM1  
*                           ODU_RFRM_MESSAGE_TYPE_TCM2 
*                           ODU_RFRM_MESSAGE_TYPE_TCM3  
*                           ODU_RFRM_MESSAGE_TYPE_TCM4
*                           ODU_RFRM_MESSAGE_TYPE_TCM5  
*                           ODU_RFRM_MESSAGE_TYPE_TCM6 
*                           ODU_RFRM_MESSAGE_TYPE_FLOATING_TCM
*   force_alarm_sig     - ODUk Alarm Signal Control to be
*                         force at the TCM.
*                         0 -- no alarm signal forced \n
*                         1 -- Force ODUK_AIS \n
*                         2 -- Force ODUK_OCI \n
*                         3 -- Force ODUK_LCK
*
*
*  OUTPUTS:
*
*  RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR - If pm_tcm_num and chnl_id is not valid

*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_tcm_maint_sig_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                            UINT32 chnl_id,
                                            odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                            UINT32 force_alarm_sig)
{
    /* variable declaration */
    UINT32 tcm_pool_id;
    PMC_ERROR ret_val;

    PMC_ENTRY();

            
    /* read TCMx_POOL_ID to find the TCM POOL ID assigned to this channel */
    
    ret_val = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id);
    
    /* configure TCM_MAINT_SIG */
    if(ret_val == PMC_SUCCESS)
    {
        /*apply the maintenance signal force value */
        odu_rfrm_field_TCM_MAINT_SIG_set(NULL, 
                                         odu_rfrm_handle, 
                                         tcm_pool_id,
                                         force_alarm_sig);
         
      
        
        
        odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].maint_sig = force_alarm_sig;

    }


    /* update XX_REMOVE_OH bits based on new maintenance signal configuration*/
    if(ret_val == PMC_SUCCESS)
    {

        ret_val = odu_rfrm_remove_oh_update(odu_rfrm_handle,
                                           chnl_id);
    }


    PMC_RETURN(ret_val);
} /* odu_rfrm_tcm_maint_sig_cfg */

/*******************************************************************************
*  odu_rfrm_tcm_maint_sig_get
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   This function configures the forced insertion of ODUk maintenance signals
*   for a TCM layer.
*
*  INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - channel ID
*                         Valid range: 0 - 95
*   tcm_num             - Enum for TCM layer instance. See odu_rfrm_pm_tcm_id_t\n
*                         1 to 6 -- TCM1 to TCM6
*
*  OUTPUTS:        
*   *maint_sig          - Retreived maintenance signal
*
*  RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR - If pm_tcm_num and chnl_id is not valid
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_tcm_maint_sig_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                            UINT32 chnl_id, 
                                            odu_rfrm_pm_tcm_id_t tcm_num,
                                            UINT8 *maint_sig)
{
    /* variable declaration */
    UINT32 tcm_pool_id;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* read TCMx_POOL_ID to find the TCM POOL ID assigned to this channel */
    ret_val = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, tcm_num, &tcm_pool_id);
    
    if (PMC_SUCCESS == ret_val)
    {
        /* retreive TCM_MAINT_SIG */
        *maint_sig = odu_rfrm_field_TCM_MAINT_SIG_get(NULL, odu_rfrm_handle, tcm_pool_id);
    }
    
    PMC_RETURN(ret_val);
} /* odu_rfrm_tcm_maint_sig_get */

/*******************************************************************************
* odu_rfrm_acpt_tti_lock
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function set/unset LOCK_TTI, helping retrieving the accepted TTI.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   set                 - 1(lock)/0(unlock)
*
* RETURNS:
*   PMC_SUCCESS
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_acpt_tti_lock(odu_rfrm_handle_t *odu_rfrm_handle, BOOL8 set)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    odu_rfrm_field_LOCK_TTI_set(NULL, odu_rfrm_handle, !!set);

    PMC_RETURN(result);
} /* odu_rfrm_acpt_tti_lock */

/*******************************************************************************
* odu_rfrm_acpt_tti_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the accepted TTI message for the TCM pool ID or the
*   PM layer.
*
*   The function exptracts the 64 byte TTI relative to the 16 byte Source Access
*   Point Identifier (SAPI), the 16 byte Destination Access Point Identifier
*   (DAPI), and the 32 byte Operator Specific fields.
*
*   Accepted TTI should not be read until a complete accepted meesage has been
*   written.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi instance \n
*                         0       -- PM \n
*                         1 to 6  -- TCM1 to TCM6 \n
*                         7       -- Floating TCM \n
*                         8       -- PM_NIM
*
* OUTPUTS:
*   *sapi_ptr          - SAPI characters (16 bytes)
*   *dapi_ptr          - DAPI characters (16 bytes)
*   *opspec_ptr        - Operator Specific (32 bytes)
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_acpt_tti_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                       UINT32 chnl_id, odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                       UINT8 *sapi_ptr, UINT8 *dapi_ptr,
                                       UINT8 *opspec_ptr)
{
    /* variable declaration */
    UINT32 value, mask;
    UINT32 tcm_pool_id = -1;
    UINT32 i;
    UINT32 row = 0;
    UINT32 start_idx = 0, dapi_idx = 0, opspec_idx = 0;
    UINT32 tmp_sapi[16], tmp_dapi[16], tmp_opspec[32];
    PMC_ERROR ret_val;

    PMC_ENTRY();

    PMC_LOG_TRACE("%s, %s, %d, calling odu_rfrm_acpt_tti_ext...\n", __FILE__, __FUNCTION__, __LINE__);

    /*
    ** if it is TCM layer, Find the correct TCM_POOL_ID register for the given
    ** TCM layer of a channel
    */
    if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
    {
        /* PM layer */
        start_idx = chnl_id * 16;
        /* check if start_idx is within the range */
        if (start_idx > 1535)
        {
            PMC_LOG_TRACE("%s, %s, %d, start_idx = %u... is OUT OF RANGE.\n", __FILE__, __FUNCTION__, __LINE__, start_idx);
            PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
        }
    }
    else
    {
        /* TCM/Floating TCM/PM NIM layer, search for TCM POOL ID */
        ret_val = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id,
                                              pm_tcm_num, &tcm_pool_id);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
        start_idx = tcm_pool_id * 16;
    }

    PMC_LOG_TRACE("%s, %s, %d, tcm_pool_id = %u...\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);
    PMC_LOG_TRACE("%s, %s, %d, start_idx = %u...\n", __FILE__, __FUNCTION__, __LINE__, start_idx);

    /* find end_index for this PM/TCM layer */
    dapi_idx = start_idx + 4;
    opspec_idx = dapi_idx + 4;

    PMC_LOG_TRACE("%s, %s, %d, dapi_idx = %u...\n", __FILE__, __FUNCTION__, __LINE__, dapi_idx);
    PMC_LOG_TRACE("%s, %s, %d, opspec_idx = %u...\n", __FILE__, __FUNCTION__, __LINE__, opspec_idx);

    /*
    ** Each TCM or PM requires reading 16 consecutive registers. The first 4
    ** registers are for SAPI[0]-SAPI[15]. The next 4 registers are for DAPI[0]-
    ** DAPI[15]. The last 8 registers are for OpSpec[0]-[31].
    */

    /* Extract SAPI fields */
    mask = 0xFF;

    row = 0;

    for (i = start_idx; i < dapi_idx; i++)
    {
        if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
        {   /* PM TTI message */
            value = odu_rfrm_reg_AC_TTI_PM_array_read(NULL, odu_rfrm_handle, i);
        }
        else
        {   /* TCMi, Floating TCMi or PM NIM TTI message */
            value = odu_rfrm_reg_AC_TTI_PID_array_read(NULL, odu_rfrm_handle, i);
        }

        PMC_LOG_TRACE("%s, %s, %d, row = %u\n", __FILE__, __FUNCTION__, __LINE__, row);

        tmp_sapi[row*4 + 3] = value & mask;
        tmp_sapi[row*4 + 2] = (value >> 8) & mask;
        tmp_sapi[row*4 + 1] = (value >> 16) & mask;
        tmp_sapi[row*4] = (value >> 24) & mask;

        *sapi_ptr = tmp_sapi[row*4];
        PMC_LOG_TRACE("%s, %s, %d, tmp_sapi[%u] = 0x%x, *sapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4, tmp_sapi[row*4], *sapi_ptr);
        sapi_ptr++;
        *sapi_ptr = tmp_sapi[row*4 + 1];
        PMC_LOG_TRACE("%s, %s, %d, tmp_sapi[%u] = 0x%x, *sapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4 + 1, tmp_sapi[row*4 + 1], *sapi_ptr);
        sapi_ptr++;
        *sapi_ptr = tmp_sapi[row*4 + 2];
        PMC_LOG_TRACE("%s, %s, %d, tmp_sapi[%u] = 0x%x, *sapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4 + 2, tmp_sapi[row*4 + 2], *sapi_ptr);
        sapi_ptr++;
        *sapi_ptr = tmp_sapi[row*4 + 3];
        PMC_LOG_TRACE("%s, %s, %d, tmp_sapi[%u] = 0x%x, *sapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4 + 3, tmp_sapi[row*4 + 3], *sapi_ptr);
        sapi_ptr++;

        row++;
    }

    row = 0;
    /* Extract DAPI fields */
    for (i = dapi_idx; i < opspec_idx; i++)
    {
        if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
        {   /* PM TTI message */
            value = odu_rfrm_reg_AC_TTI_PM_array_read(NULL, odu_rfrm_handle, i);
        }
        else
        {   /* TCMi, Floating TCMi or PM NIM TTI message */
            value = odu_rfrm_reg_AC_TTI_PID_array_read(NULL, odu_rfrm_handle, i);
        }

        tmp_dapi[row*4 + 3] = value & mask;
        tmp_dapi[row*4 + 2] = (value >> 8) & mask;
        tmp_dapi[row*4 + 1] = (value >> 16) & mask;
        tmp_dapi[row*4] = (value >> 24) & mask;

        *dapi_ptr = tmp_dapi[row*4];
        PMC_LOG_TRACE("%s, %s, %d, tmp_dapi[%u] = 0x%x, *dapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4, tmp_dapi[row*4], *dapi_ptr);
        dapi_ptr++;
        *dapi_ptr = tmp_dapi[row*4 + 1];
        PMC_LOG_TRACE("%s, %s, %d, tmp_dapi[%u] = 0x%x, *dapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4 + 1, tmp_dapi[row*4 + 1], *dapi_ptr);
        dapi_ptr++;
        *dapi_ptr = tmp_dapi[row*4 + 2];
        PMC_LOG_TRACE("%s, %s, %d, tmp_dapi[%u] = 0x%x, *dapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4 + 2, tmp_dapi[row*4 + 2], *dapi_ptr);
        dapi_ptr++;
        *dapi_ptr = tmp_dapi[row*4 + 3];
        PMC_LOG_TRACE("%s, %s, %d, tmp_dapi[%u] = 0x%x, *dapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4 + 3, tmp_dapi[row*4 + 3], *dapi_ptr);
        dapi_ptr++;

        row++;
    }

    row = 0;
    /* Extract Op Spec fields */
    for (i = opspec_idx; i < (opspec_idx + 8); i++)
    {
        if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
        {   /* PM TTI message */
            value = odu_rfrm_reg_AC_TTI_PM_array_read(NULL, odu_rfrm_handle, i);
        }
        else
        {   /* TCMi, Floating TCMi or PM NIM TTI message */
            value = odu_rfrm_reg_AC_TTI_PID_array_read(NULL, odu_rfrm_handle, i);
        }

        tmp_opspec[row*4 + 3] = value & mask;
        tmp_opspec[row*4 + 2] = (value >> 8) & mask;
        tmp_opspec[row*4 + 1] = (value >> 16) & mask;
        tmp_opspec[row*4] = (value >> 24) & mask;

        *opspec_ptr = tmp_opspec[row*4];
        PMC_LOG_TRACE("%s, %s, %d, tmp_opspec[%u] = 0x%x, *opspec_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4, tmp_opspec[row*4], *opspec_ptr);
        opspec_ptr++;
        *opspec_ptr = tmp_opspec[row*4 + 1];
        PMC_LOG_TRACE("%s, %s, %d, tmp_opspec[%u] = 0x%x, *opspec_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4 + 1, tmp_opspec[row*4 + 1], *opspec_ptr);
        opspec_ptr++;
        *opspec_ptr = tmp_opspec[row*4 + 2];
        PMC_LOG_TRACE("%s, %s, %d, tmp_opspec[%u] = 0x%x, *opspec_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4 + 2, tmp_opspec[row*4 + 2], *opspec_ptr);
        opspec_ptr++;
        *opspec_ptr = tmp_opspec[row*4 + 3];
        PMC_LOG_TRACE("%s, %s, %d, tmp_opspec[%u] = 0x%x, *opspec_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4 + 3, tmp_opspec[row*4 + 3], *opspec_ptr);
        opspec_ptr++;
        
        row++;
    }  
    
    
    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_acpt_tti_ext */

/*******************************************************************************
* odu_rfrm_ftfl_ext
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
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
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
PUBLIC PMC_ERROR odu_rfrm_ftfl_ext(odu_rfrm_handle_t *odu_rfrm_handle,
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

    PMC_LOG_TRACE("%s, %s, %d, calling odu_rfrm_ftfl_ext...\n", __FILE__, __FUNCTION__, __LINE__);

    /* find the register for given channel_id */
    start_idx = chnl_id * 8;
    boif_idx = start_idx + 4;
    mask = 0xFF;

    /* Set LOCK_TTI bit to 1 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 1);

    PMC_LOG_TRACE("%s, %s, %d, start_idx = %u...\n", __FILE__, __FUNCTION__, __LINE__, start_idx);
    PMC_LOG_TRACE("%s, %s, %d, boif_idx = %u...\n", __FILE__, __FUNCTION__, __LINE__, boif_idx);

    /* fwd_id and fwd_op_id[9] */
    for (i = start_idx; i < start_idx + 3; i++)
    {
        value = odu_rfrm_reg_FTFL_array_read(NULL, odu_rfrm_handle, i);

        /* Extract fwd_id 
        tmp_fwd_id[0] = (value >> 24) & mask;
        PMC_LOG_TRACE("%s, %s, %d, tmp_fwd_id = 0x%x...\n", __FILE__, __FUNCTION__, __LINE__, tmp_fwd_id[0]);
        */
        
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
        value = odu_rfrm_reg_FTFL_array_read(NULL, odu_rfrm_handle, i);

        /* Extract bwd_id 
        tmp_bwd_id[0] = (value >> 24) & 0xFF;
        */
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
    
    /* Set LOCK_TTI bit to 0 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 0);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_ftfl_ext */


/*******************************************************************************
* odu_rfrm_acpt_aps_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the accepted APS/PCC message for a particular
*   connection monitoring level of an ODU channel.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   layer_num           - Connection monitoring level \n
*                         0      -- ODUk Path \n
*                         1 to 6 -- ODUk TCM1 - TCM6 \n
*                         7      -- ODUk server layer trail
*
* OUTPUTS:
*   *aps_val            - accepted APS/PCC message for the connection monitoring
*                         level
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_acpt_aps_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                       UINT32 chnl_id, UINT32 layer_num,
                                       UINT32 *aps_val)
{
    /* variable declaration */
    UINT32 idx;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    /* Set LOCK to 1 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 1);
    
    idx = chnl_id * 8 + layer_num;
    PMC_LOG_TRACE("%s, %s, %d, idx = %u...\n", __FILE__, __FUNCTION__, __LINE__, idx);

    *aps_val = odu_rfrm_reg_AC_APS_PCC_array_read(NULL, odu_rfrm_handle, idx);
    PMC_LOG_TRACE("%s, %s, %d, aps_val = %u...\n", __FILE__, __FUNCTION__, __LINE__, *aps_val);

    /* Set LOCK to 0 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 0);
    
    PMC_RETURN(result);
} /* odu_rfrm_acpt_aps_ext */

/*******************************************************************************
* odu_rfrm_exp_tti_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the expected receive PM or TCMi TTI message.
*
*   The function takes in the 16 byte Source Access Point Identifier (SAPI),
*   the 16 byte Destination Access Point Identifier (DAPI)as arguments to fill
*   the SAPI and DAPI fields of the 64 byte TTI.
*
*   The range of a SAPI or DAPI characters 15 to 1 are 0 to 127.
*   SAPI and DAPI character 0 must be set to 0x00. Function will return
*   ODU_RFRM_LOG_ERR_INVALID_PARAMETERS if these rules are not adhered.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6 \n
*                         7      -- Floating TCM \n
*                         8      -- PM NIM
*   sapi                - SAPI characters (16 bytes)
*   dapi                - DAPI characters (16 bytes)
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
PUBLIC PMC_ERROR odu_rfrm_exp_tti_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                          UINT32 chnl_id, odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                          UINT8 sapi[16], UINT8 dapi[16])
{
    /* variable declaration */
    UINT32 tcm_pool_id;
    UINT32 i, j;
    UINT32 start_idx = 0;
    UINT32 tti_to_write[8];
    UINT32 byte_val;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    j =0;
    /* SAPI */
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


    PMC_LOG_TRACE("%s, %s, %d, calling odu_rfrm_exp_tti_cfg...\n", __FILE__, __FUNCTION__, __LINE__);

    if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
    {
        /* PM layer */
        start_idx = chnl_id * 8;
        /* check if start_idx is within the range */
        if (start_idx > 767)
        {
            PMC_LOG_TRACE("%s, %s, %d, start_idx = %u... is OUT OF RANGE.\n", __FILE__, __FUNCTION__, __LINE__, start_idx);
            result = ODU_RFRM_LOG_ERR_INVALID_PARAMETERS;
        }
    }
    else
    {
        /* TCM layer, search for TCM POOL ID */
        result = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id,
                                         pm_tcm_num, &tcm_pool_id);
        if (PMC_SUCCESS == result)
        {
            start_idx = tcm_pool_id * 8;
        }
    }


    if (PMC_SUCCESS == result && pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
    {   /* PM layer */
        odu_rfrm_reg_EXP_TTI_PM_array_burst_write(NULL, odu_rfrm_handle, start_idx, 8, tti_to_write);
    }
    else if (PMC_SUCCESS == result)
    {
        odu_rfrm_reg_EXP_TTI_PID_array_burst_write(NULL, odu_rfrm_handle, start_idx, 8, tti_to_write);
    }

    PMC_RETURN(result);
} /* odu_rfrm_exp_tti_cfg */

/*******************************************************************************
* odu_rfrm_exp_tti_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the expected receive PM or TCMi TTI message.
*
*   The function exptracts the 64 byte TTI relative to the 16 byte Source Access
*   Point Identifier (SAPI) and the 16 byte Destination Access Point Identifier
*   (DAPI).
*
*   Accepted TTI should not be read until a complete accepted meesage has been
*   written.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM
*
* OUTPUTS:
*   *sapi_ptr           - pointer to storage of SAPI characters (16 bytes)
*   *dapi_ptr           - pointer to storage of DAPI characters (16 bytes)
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_exp_tti_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                      UINT32 chnl_id, odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                      UINT8 *sapi_ptr, UINT8 *dapi_ptr)
{
    /* variable declaration */
    UINT32 value, mask;
    UINT32 tcm_pool_id;
    UINT32 i, row = 0;
    UINT32 start_idx = 0, dapi_idx = 0, end_idx = 0;
    UINT32 tmp_sapi[16], tmp_dapi[16];
    PMC_ERROR ret_val;

    PMC_ENTRY();

    mask = 0xFF;

    if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
    {
        /* PM layer */
        start_idx = chnl_id * 8;
        /* check if start_idx is within the range */
    }
    else
    {
        /* TCM layer, search for TCM POOL ID */
        ret_val = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id,
                                              pm_tcm_num, &tcm_pool_id);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
        start_idx = tcm_pool_id * 8;
    }

    dapi_idx = start_idx + 4;
    end_idx = dapi_idx + 4;

    row = 0;

    for (i = start_idx; i < dapi_idx; i++)
    {
        if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
        { /* PM layer */
            value = odu_rfrm_reg_EXP_TTI_PM_array_read(NULL, odu_rfrm_handle, i);
        }
        else
        { /* TCM layer */
            value = odu_rfrm_reg_EXP_TTI_PID_array_read(NULL, odu_rfrm_handle, i);
        }

        tmp_sapi[row*4 + 3] = value & mask;
        PMC_LOG_TRACE("%s, %s, %d, value = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, value);
        tmp_sapi[row*4 + 2] = (value >> 8) & mask;
        PMC_LOG_TRACE("%s, %s, %d, (value >> 8) = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, (value >> 8));
        tmp_sapi[row*4 + 1] = (value >> 16) & mask;
        PMC_LOG_TRACE("%s, %s, %d, (value >> 16) = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, (value >> 16));
        tmp_sapi[row*4] = (value >> 24) & mask;
        PMC_LOG_TRACE("%s, %s, %d, (value >> 24) = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, (value >> 24));

        *sapi_ptr = tmp_sapi[row*4];
        PMC_LOG_TRACE("%s, %s, %d, tmp_sapi[%u] = 0x%x, *sapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4, tmp_sapi[row*4], *sapi_ptr);
        sapi_ptr++;
        *sapi_ptr = tmp_sapi[row*4 + 1];
        PMC_LOG_TRACE("%s, %s, %d, tmp_sapi[%u] = 0x%x, *sapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4+1, tmp_sapi[row*4+1], *sapi_ptr);
        sapi_ptr++;
        *sapi_ptr = tmp_sapi[row*4 + 2];
        PMC_LOG_TRACE("%s, %s, %d, tmp_sapi[%u] = 0x%x, *sapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4+2, tmp_sapi[row*4+2], *sapi_ptr);
        sapi_ptr++;
        *sapi_ptr = tmp_sapi[row*4 + 3];
        PMC_LOG_TRACE("%s, %s, %d, tmp_sapi[%u] = 0x%x, *sapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4+3, tmp_sapi[row*4+3], *sapi_ptr);
        sapi_ptr++;

        row++;
    }

    row = 0;
    /* Extract DAPI fields */
    for (i = dapi_idx; i < end_idx; i++)
    {
        if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
        { /* PM layer */
            value = odu_rfrm_reg_EXP_TTI_PM_array_read(NULL, odu_rfrm_handle, i);
        }
        else
        { /* TCM layer */
            value = odu_rfrm_reg_EXP_TTI_PID_array_read(NULL, odu_rfrm_handle, i);
        }

        tmp_dapi[row*4 + 3] = value & mask;
        tmp_dapi[row*4 + 2] = (value >> 8) & mask;
        tmp_dapi[row*4 + 1] = (value >> 16) & mask;
        tmp_dapi[row*4] = (value >> 24) & mask;

        *dapi_ptr = tmp_dapi[row*4];
        PMC_LOG_TRACE("%s, %s, %d, tmp_dapi[%u] = 0x%x, *dapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4, tmp_dapi[row*4], *dapi_ptr);
        dapi_ptr++;
        *dapi_ptr = tmp_dapi[row*4 + 1];
        PMC_LOG_TRACE("%s, %s, %d, tmp_dapi[%u] = 0x%x, *dapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4 + 1, tmp_dapi[row*4 + 1], *dapi_ptr);
        dapi_ptr++;
        *dapi_ptr = tmp_dapi[row*4 + 2];
        PMC_LOG_TRACE("%s, %s, %d, tmp_dapi[%u] = 0x%x, *dapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4 + 2, tmp_dapi[row*4 + 2], *dapi_ptr);
        dapi_ptr++;
        *dapi_ptr = tmp_dapi[row*4 + 3];
        PMC_LOG_TRACE("%s, %s, %d, tmp_dapi[%u] = 0x%x, *dapi_ptr = 0x%x\n", __FILE__, __FUNCTION__, __LINE__, row*4 + 3, tmp_dapi[row*4 + 3], *dapi_ptr);
        dapi_ptr++;

        row++;
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_exp_tti_ext */

/*******************************************************************************
* odu_rfrm_pm_ebc_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the accepted receive GCC1 and GCC2 bytes.
*
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *ebc             - COREOTN ODUkp far/near end counters for a PM layer of
*                      an ODUk channel
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_ebc_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                     UINT32 chnl_id,
                                     odu_rfrm_odukp_counters_t *ebc)
{
    PMC_ENTRY();

    /* Set LOCK to 1 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 1);

    /* Read ebc */
    ebc->odukp_sk_pn_ds_pm = odu_rfrm_field_PN_DS_PM_get(NULL, odu_rfrm_handle, chnl_id);
    ebc->odukp_sk_pn_ebc_pm = odu_rfrm_field_PN_EBC_PM_get(NULL, odu_rfrm_handle, chnl_id);
    ebc->odukp_sk_pf_ds_pm = odu_rfrm_field_PF_DS_PM_get(NULL, odu_rfrm_handle, chnl_id);
    ebc->odukp_sk_pf_ebc_pm = odu_rfrm_field_PF_EBC_PM_get(NULL, odu_rfrm_handle, chnl_id);

    /* Set LOCK to 0 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 0);
    
    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_ebc_get */

/*******************************************************************************
* odu_rfrm_acpt_gcc_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the accepted receive GCC1 and GCC2 bytes.
*
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   oh_field            - selects overhead field to be extracted
*                         0 -- GCC1 byte
*                         1 -- GCC2 byte
*
* OUTPUTS:
*   *gcc_val            - extracted overhead GCC byte
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_acpt_gcc_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                       UINT32 chnl_id, UINT8 oh_field, 
                                       UINT16 *gcc_val)
{
    PMC_ENTRY();

    /* Set LOCK to 1 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 1);

    /* Read GCC1 byte */
    if (oh_field == 0)
    {
        *gcc_val = odu_rfrm_field_GCC1_get(NULL, odu_rfrm_handle, chnl_id);
    }
    else {        
        *gcc_val = odu_rfrm_field_GCC2_get(NULL, odu_rfrm_handle, chnl_id);
    }        

    /* Set LOCK to 0 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 0);
    
    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_acpt_gcc_ext */

/*******************************************************************************
* odu_rfrm_acpt_stat_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the accepted receive PM or TCM STAT OH byte.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi layer instance \n
*                         0      -- PM \n
*                         1 to 6 -- TCM1 to TCM6
*   *stat               - Extracted STAT OH byte
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
PUBLIC PMC_ERROR odu_rfrm_acpt_stat_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                        UINT32 chnl_id, 
                                        odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                        UINT8 *stat)
{
    PMC_ENTRY();

    /* Set LOCK to 1 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 1);

    switch (pm_tcm_num)
    {
        /* PM layer */
        case ODU_RFRM_MESSAGE_TYPE_PM:
            *stat = odu_rfrm_field_AC_PM_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        /* TCM1 */
        case ODU_RFRM_MESSAGE_TYPE_TCM1:
            *stat = odu_rfrm_field_AC_TCM1_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        /* TCM2 */
        case ODU_RFRM_MESSAGE_TYPE_TCM2:
            *stat = odu_rfrm_field_AC_TCM2_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        /* TCM3 */
        case ODU_RFRM_MESSAGE_TYPE_TCM3:
            *stat = odu_rfrm_field_AC_TCM3_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        /* TCM4 */
        case ODU_RFRM_MESSAGE_TYPE_TCM4:
            *stat = odu_rfrm_field_AC_TCM4_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        /* TCM5 */
        case ODU_RFRM_MESSAGE_TYPE_TCM5:
            *stat = odu_rfrm_field_AC_TCM5_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        /* TCM6 */
        case ODU_RFRM_MESSAGE_TYPE_TCM6:
            *stat = odu_rfrm_field_AC_TCM6_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        default:
            PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }
    
    /* Set LOCK to 0 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 0);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_acpt_stat_ext */

/*******************************************************************************
* odu_rfrm_acpt_pm_tcm_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the accepted receive PM&TCM OH byte.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   *pm_tcm             - Extracted PM&TCM OH byte
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
PUBLIC PMC_ERROR odu_rfrm_acpt_pm_tcm_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                          UINT32 chnl_id, UINT8 *pm_tcm)
{
    PMC_ENTRY();

    /* Set LOCK to 1 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 1);

    /* Read PM_TCM field */
    *pm_tcm = odu_rfrm_field_PM_TCM_get(NULL, odu_rfrm_handle, chnl_id);

    /* Set LOCK to 0 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 0);
    
    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_acpt_pm_tcm_ext */

/*******************************************************************************
* odu_rfrm_acpt_tcm_act_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the accepted receive TCM ACT OH byte.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   *tcm_act            - Extracted TCM ACT OH byte
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
PUBLIC PMC_ERROR odu_rfrm_acpt_tcm_act_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                           UINT32 chnl_id, UINT8 *tcm_act)
{
    PMC_ENTRY();

    /* Set LOCK to 1 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 1);

    /* Read TCM_ACT field */
    *tcm_act = odu_rfrm_field_TCM_ACT_get(NULL, odu_rfrm_handle, chnl_id);

    /* Set LOCK to 0 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 0);
    
    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_acpt_tcm_act_ext */

/*******************************************************************************
* odu_rfrm_acpt_res1_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the accepted receive RES1 OH byte.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   *res1               - Extracted RES1 OH byte
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
PUBLIC PMC_ERROR odu_rfrm_acpt_res1_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                        UINT32 chnl_id, UINT16 *res1)
{
    PMC_ENTRY();

    /* Set LOCK to 1 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 1);

    /* Read RES1 field */
    *res1 = odu_rfrm_field_RES1_get(NULL, odu_rfrm_handle, chnl_id);

    /* Set LOCK to 0 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 0);
    
    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_acpt_res1_ext */

/*******************************************************************************
* odu_rfrm_acpt_res2_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the accepted receive RES2 OH byte.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *res2               - Extracted ODUk RES2 bytes (row 4 column 9-14)
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_acpt_res2_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                        UINT32 chnl_id, UINT8 *res2)
{
    UINT32 res2_hi;
    UINT16 res2_low;
    UINT32 i;
    
    PMC_ENTRY();

    /* Set LOCK to 1 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 1);

    /* Read RES2 field */
    res2_low = odu_rfrm_field_RES2_get(NULL, odu_rfrm_handle, chnl_id);
    res2_hi = odu_rfrm_field_RES3_get(NULL, odu_rfrm_handle, chnl_id);

    res2[1] = (res2_low & 0xFF);
    res2_low = res2_low >> 8;
    res2[0] = (res2_low & 0xFF);
    
    for (i = 5; i > 1; i--)
    {
        res2[i] = (res2_hi & 0xFF);
        res2_hi = res2_hi >> 8;
    }
     
    /* Set LOCK to 0 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 0);
    
    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_acpt_res2_ext */

/*******************************************************************************
* odu_rfrm_acpt_exp_ext
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function extracts the accepted receive EXP OH field.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   *exp                - Extracted EXP OH byte
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
PUBLIC PMC_ERROR odu_rfrm_acpt_exp_ext(odu_rfrm_handle_t *odu_rfrm_handle,
                                       UINT32 chnl_id, UINT16 *exp)
{
    PMC_ENTRY();

    /* Set LOCK to 1 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 1);

    /* Read RES2 field */
    *exp = odu_rfrm_field_EXP_get(NULL, odu_rfrm_handle, chnl_id);

    /* Set LOCK to 0 */
    odu_rfrm_field_LOCK_set(NULL, odu_rfrm_handle, 0);
    
    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_acpt_exp_ext */

/*******************************************************************************
* odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures whether or not the consequential actions are
*   generated as a result of TSD or TSF defect detection.
*
*   If configuring for TCMi layers, the layer must be in operational mode to
*   be able to properly generate defect consequential actions.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi instance \n
*                         0       -- PM \n
*                         1 to 6  -- TCM1 to TCM6 \n
*                         7       -- Floating TCM \n
*                         8       -- PM_NIM
*   tcm_pool_id         - Pool id of the TCM/floating TCM/PM-NIM
*   defect_ctrl         - determines which defect detection to configure.
*                         0 -- TSD
*                         1 -- TSF
*                         2 -- both TSD and TSF
*   tsd_dis             - Controls the aTSD consequential action \n
*                         0 -- The aTSB consequential action is detected and
*                              processed normally. \n
*                         1 -- The aTSD consequential action is disabled.
*   tsf_dis             - Controls the aTSF consequential action \n
*                         0 -- The aTSF consequential action is detected and
*                              processed normally. \n
*                         1 -- The aTSF consequential action is disabled.
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
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg(odu_rfrm_handle_t *odu_rfrm_handle,
                                                   UINT32 chnl_id,
                                                   odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                   UINT32 tcm_pool_id,
                                                   UINT8 defect_ctrl,
                                                   UINT8 tsd_dis,
                                                   UINT8 tsf_dis)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    /* retrieve POOL ID */
    if (pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM)
    {
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }

        if (defect_ctrl == 1 || defect_ctrl == 2)
        {    
            /* configure TSD_DIS and TSF_DIS */
            odu_rfrm_field_TSF_DIS_set(NULL, odu_rfrm_handle, tcm_pool_id, tsf_dis);
        }
        
        if (defect_ctrl == 0 || defect_ctrl == 2)
        {
            odu_rfrm_field_TSD_DIS_set(NULL, odu_rfrm_handle, tcm_pool_id, tsd_dis);
        }            
    }
    else
    {
        if (defect_ctrl == 1 || defect_ctrl == 2)
        {
            odu_rfrm_field_TSF_DIS_PM_set(NULL, odu_rfrm_handle, chnl_id, tsf_dis);
        }
        
        if (defect_ctrl == 0 || defect_ctrl == 2)
        {
            odu_rfrm_field_TSD_DIS_PM_set(NULL, odu_rfrm_handle, chnl_id, tsd_dis);
        }            
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_tcmi_tsd_tsf_proc_cfg */

/*******************************************************************************
* odu_rfrm_pm_tcmi_tsd_tsf_dis_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retreives the current configuration of aTSD and aTSF defect 
*   generation and detection.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - ODU Channel ID
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi instance \n
*                         0       -- PM \n
*                         1 to 6  -- TCM1 to TCM6 \n
*                         7       -- Floating TCM \n
*                         8       -- PM_NIM
*
* OUTPUTS:
*   *tsd_dis            - current configuration to disable the aTSD  
*                         consequential action \n
*   *tsf_dis            - current configuration to disable the aTSF  
*                         consequential action \n
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_tsd_tsf_dis_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                                  UINT32 chnl_id,
                                                  odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                  UINT8 *tsd_dis,
                                                  UINT8 *tsf_dis)
{
    /* variable declaration */
    UINT32 pool_id;
    PMC_ERROR ret_val;

    PMC_ENTRY();

    /* retrieve POOL ID */
    if (pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM)
    {
        ret_val = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, pm_tcm_num, &pool_id);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }

        /* configure TSD_DIS and TSF_DIS */
        *tsf_dis = odu_rfrm_field_TSF_DIS_get(NULL, odu_rfrm_handle, pool_id);
        *tsd_dis = odu_rfrm_field_TSD_DIS_get(NULL, odu_rfrm_handle, pool_id);
    }
    else
    {
        *tsf_dis = odu_rfrm_field_TSF_DIS_PM_get(NULL, odu_rfrm_handle, chnl_id);
        *tsd_dis = odu_rfrm_field_TSD_DIS_PM_get(NULL, odu_rfrm_handle, chnl_id);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_pm_tcmi_tsd_tsf_dis_get */

/*******************************************************************************
* odu_rfrm_is_chnl_schd
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function checks if the channel has been scheduled on DCI bus.
*
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
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
PUBLIC BOOL8 odu_rfrm_is_chnl_schd(odu_rfrm_handle_t *odu_rfrm_handle,
                                  UINT32 chnl_id)
{
    /* variable declaration */
    UINT32 i;
    UINT32 aps_val;
    BOOL8 result = FALSE;
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    for (i = 0; i < 8; i++)
    {
        ret_val = odu_rfrm_acpt_aps_ext(odu_rfrm_handle, chnl_id, i, &aps_val);
     
        if (PMC_SUCCESS == ret_val)
        {
            if (aps_val != 0x55000000)
            {
                result = FALSE; 
                break;   
            }  
            else {
                result = TRUE;
            }
        }
    }                                  
    
    PMC_RETURN(result);
} /* odu_rfrm_is_chnl_schd */

/*******************************************************************************
* odu_rfrm_internal_chnl_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleans a  channel
*
* INPUTS:
*   *odu_rfrm_handle   - pointer to ODU_RFRM handle instance to be operated on
*   chnl               - the channel that needs to be cleaned
*   deprov_configs     - TRUE : deprov all RFRM configrations that have 
*                               been set through run-tim APIs (TCMs, etc)
*                        FALSE : deprov RFRM configurations
*
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
PRIVATE void odu_rfrm_internal_chnl_clean(odu_rfrm_handle_t *odu_rfrm_handle,
                                          UINT32 chnl,
                                          BOOL deprov_configs)
{
    UINT32 pool_itr;
    odu_rfrm_buffer_t b[1];
    PMC_ENTRY();
    
    odu_rfrm_buffer_init(b,odu_rfrm_handle);


    odu_rfrm_field_PASSTHRU_set(b, odu_rfrm_handle, chnl, TRUE);
    odu_rfrm_field_FAU_PASSTHRU_set(b, odu_rfrm_handle, chnl, TRUE);

    
    if(deprov_configs)
    {
        odu_rfrm_field_MAINT_SIG_set(b, odu_rfrm_handle, chnl, 0);
        
        if (odu_rfrm_handle->cfg.odu_rfrm_inst == ODU_RFRM_STG1)
        {
            odu_rfrm_reg_CHANNEL_CTRL0_array_write(b, odu_rfrm_handle, chnl, 0x0007F008);  
        } else 
        {
            odu_rfrm_reg_CHANNEL_CTRL0_array_write(b, odu_rfrm_handle, chnl, 0x0007F008);  
        }
        /* odu_rfrm_reg_CHANNEL_CTRL0_array_write(b, odu_rfrm_handle, chnl, 0x0007F008);   */
       
        odu_rfrm_field_MODE_PM_set(b, odu_rfrm_handle, chnl, 0);

        odu_rfrm_handle->var.pm_provisioned[chnl] = FALSE;
        odu_rfrm_handle->var.pm_mode[chnl] = ODU_RFRM_PM_TCM_START; 
        odu_rfrm_handle->var.dci_pm_tcm_child[chnl] = ODU_RFRM_MESSAGE_DONT_CARE;
        odu_rfrm_handle->var.maint_sig[chnl] = 0;
        for (pool_itr = 0; pool_itr < odu_rfrm_handle->cfg.tcm_pool_size; pool_itr++)
        {
            if (odu_rfrm_handle->var.tcm_pool_ptr[pool_itr].chnl_id == chnl)
            {
                odu_rfrm_handle->var.tcm_pool_ptr[pool_itr].assign_state      = 0;
                odu_rfrm_handle->var.tcm_pool_ptr[pool_itr].chnl_id           = 0xFF;
                odu_rfrm_handle->var.tcm_pool_ptr[pool_itr].tcm_num           = ODU_RFRM_MESSAGE_DONT_CARE;
                odu_rfrm_handle->var.tcm_pool_ptr[pool_itr].tcm_mode          = ODU_RFRM_PM_TCM_START;
                
                odu_rfrm_handle->var.tcm_pool_ptr[pool_itr].child_pool_id     = ODU_RFRM_UNASSIGNED_POOL_ID;
                odu_rfrm_handle->var.tcm_pool_ptr[pool_itr].child_pm_tcm_num   = ODU_RFRM_MESSAGE_DONT_CARE;
                odu_rfrm_handle->var.tcm_pool_ptr[pool_itr].maint_sig          = 0;
                
                odu_rfrm_handle->var.num_pool_free += 1;
                /* odu_rfrm_reg_LAYER_CTRL0_PID_array_write(b, odu_rfrm_handle, pool_itr, 0x0020000F); */
                /* odu_rfrm_reg_LAYER_CTRL1_PID_array_write(b, odu_rfrm_handle, pool_itr, 0x000100); */
            }
        }

    }
   
    odu_rfrm_buffer_flush(b);
    
    PMC_RETURN();
} /* odu_rfrm_internal_chnl_clean */

/*******************************************************************************
* odu_rfrm_chnl_clean
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function clean a set of channels
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
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
PUBLIC void odu_rfrm_chnl_clean(odu_rfrm_handle_t *odu_rfrm_handle,
                                UINT32 *chnl,
                                UINT32 num_chnl)
{
    UINT32 chnl_itr;
    PMC_ENTRY();
    
    for(chnl_itr = 0; chnl_itr < num_chnl; chnl_itr++)
    {        
        odu_rfrm_internal_chnl_clean(odu_rfrm_handle,
                                     chnl[chnl_itr],
                                     TRUE);
    }
        
    PMC_RETURN();
} /* odu_rfrm_chnl_clean */

/*
** Private Functions
*/
/*******************************************************************************
*  odu_rfrm_var_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initialize ODU_RFRM var_t variables. Elements in TCM POOL ID
*   context structure are also initialized in this function.
*
* INPUTS:
*   hndl     - handle to the ODU_RFRM instance to be operated on
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE void odu_rfrm_var_default_init(odu_rfrm_handle_t *hndl)
{
    /* variable declaration */
    UINT32 i;

    PMC_ENTRY();

    for (i = 0; i < hndl->cfg.num_chans; i++)
    {     
        hndl->var.pm_provisioned[i] = FALSE;
        hndl->var.pm_mode[i] = ODU_RFRM_PM_TCM_START;   
        hndl->var.dci_pm_tcm_child[i] = ODU_RFRM_MESSAGE_DONT_CARE;
        hndl->var.maint_sig[i] = 0;
    }
     hndl->var.num_pool_free = hndl->cfg.tcm_pool_size;
    /* initialize TCM POOL ID context table */
    for (i = 0; i < hndl->cfg.tcm_pool_size; i++)
    {
        hndl->var.tcm_pool_ptr[i].chnl_id           = 0xFF;
        hndl->var.tcm_pool_ptr[i].tcm_num           = ODU_RFRM_MESSAGE_DONT_CARE;
        hndl->var.tcm_pool_ptr[i].tcm_mode          = ODU_RFRM_PM_TCM_START;
        
        hndl->var.tcm_pool_ptr[i].child_pool_id     = ODU_RFRM_UNASSIGNED_POOL_ID;
        hndl->var.tcm_pool_ptr[i].child_pm_tcm_num  = ODU_RFRM_MESSAGE_DONT_CARE;
        hndl->var.tcm_pool_ptr[i].maint_sig         = 0;
    }


    PMC_RETURN();
} /* odu_rfrm_var_default_init */

/*******************************************************************************
*  odu_rfrm_block_non_default_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures non-default register settings associated with
*   the ODU_RFRM block in COREOTN subsystem.
*
* INPUTS:
*   *hndl               - pointer to ODU_RFRM handle instance to be operated on
*   framer_instance     - odu_rfrm instance.
*
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
PUBLIC PMC_ERROR odu_rfrm_block_non_default_cfg(odu_rfrm_handle_t *hndl,
                                                UINT32 framer_instance)
{
    PMC_ENTRY();
    PMC_LOG_TRACE("%s, %s, %d, Block is in operational mode, applying non-default settings...\n", __FILE__, __FUNCTION__, __LINE__);

    PMC_RETURN(PMC_SUCCESS);

} /* odu_rfrm_block_non_default_cfg */

/*******************************************************************************
* odu_rfrm_pm_tcmi_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function activates or deactivates the provisioned layer.
*
*   Prior to calling this API, the layer must have been configured with
*   odu_rfrm_pm_tcmi_cfg() API.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM (must set float_tcmi_src) \n
*                         8         -- PM NIM (non-intrusive monitoring)
*   float_tcmi_src      - The TCM number for the floating TCM
*   mode                - functional mode of layer sink function. 
*                         See odu_rfrm_pm_tcm_mode_t for valid options. \n
*                         0 -- Transparent (termination) \n
*                         1 -- Monitor (termination) \n
*                         2 -- Operational (termination) \n
*                         3 -- Non-intrusive monitoring
*   tcm_pool_id         - The TCM pool id for the TCM/floating TCM
*                         /PM-NIM
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
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_mode_set(odu_rfrm_handle_t *odu_rfrm_handle,
                                           UINT32 chnl_id,
                                           odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                           UINT32 float_tcmi_src,  
                                           odu_rfrm_pm_tcm_mode_t mode,
                                           UINT32 tcm_pool_id)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 transparent_mode;
    UINT32 op_mode;

    
    /* variable declaration */
    PMC_ENTRY();

    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);

    if (PMC_SUCCESS == result)
    {
        /* configure for each operating mode */
        switch (mode)
        {
             case ODU_RFRM_PM_TCM_TT_OPERATIONAL:
                transparent_mode = 0;
                op_mode = 0x3;
                break;
             case ODU_RFRM_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED:
                transparent_mode = 0;
                op_mode = 0x3;
                break;
            case ODU_RFRM_PM_TCM_TT_MONITOR:
                transparent_mode = 0;
                op_mode = 0x2;
                break;
           case ODU_RFRM_PM_TCM_TT_TRANSPARENT:
                transparent_mode = 1;
                op_mode = 0;
                break;
           case ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR:
                transparent_mode = 0;
                op_mode = 0x1;
                break;
           case ODU_RFRM_PM_TCM_TT_ERASE:          
                transparent_mode = 0;
                op_mode = 0x3;
                break;
           default:
                PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
        }        


        if (PMC_SUCCESS == result)
        {
            /* set mode */
            if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
            {
                odu_rfrm_field_MODE_PM_set(NULL, odu_rfrm_handle, chnl_id, op_mode);
            }          
            else
            {
                odu_rfrm_field_MODE_set(NULL, odu_rfrm_handle, tcm_pool_id, op_mode);       
            }                                               
        }
   
        /* Enable/disable desired layer of the channel */
        switch (pm_tcm_num)
        {
            case ODU_RFRM_MESSAGE_TYPE_TCM1: /* TCM1 */
                odu_rfrm_field_TCM1_TRANSPARENT_set(NULL, odu_rfrm_handle, chnl_id, transparent_mode);
                if(mode == ODU_RFRM_PM_TCM_TT_TRANSPARENT)
                {
                    odu_rfrm_field_TCM1_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, 0);
                }
                break;
            case ODU_RFRM_MESSAGE_TYPE_TCM2: /* TCM2 */
                odu_rfrm_field_TCM2_TRANSPARENT_set(NULL, odu_rfrm_handle, chnl_id, transparent_mode);
                if(mode == ODU_RFRM_PM_TCM_TT_TRANSPARENT)
                {
                    odu_rfrm_field_TCM2_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, 0);
                }
                break;
            case ODU_RFRM_MESSAGE_TYPE_TCM3: /* TCM3 */
                odu_rfrm_field_TCM3_TRANSPARENT_set(NULL, odu_rfrm_handle, chnl_id, transparent_mode);
                if(mode == ODU_RFRM_PM_TCM_TT_TRANSPARENT)
                {
                    odu_rfrm_field_TCM3_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, 0);
                }                
                break;
            case ODU_RFRM_MESSAGE_TYPE_TCM4: /* TCM4 */
                odu_rfrm_field_TCM4_TRANSPARENT_set(NULL, odu_rfrm_handle, chnl_id, transparent_mode);
                if(mode == ODU_RFRM_PM_TCM_TT_TRANSPARENT)
                {
                    odu_rfrm_field_TCM4_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, 0);
                }   
                break;
            case ODU_RFRM_MESSAGE_TYPE_TCM5: /* TCM5 */
                odu_rfrm_field_TCM5_TRANSPARENT_set(NULL, odu_rfrm_handle, chnl_id, transparent_mode);
                if(mode == ODU_RFRM_PM_TCM_TT_TRANSPARENT)
                {
                    odu_rfrm_field_TCM5_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, 0);
                }  
                break;
            case ODU_RFRM_MESSAGE_TYPE_TCM6: /* TCM6 */
                odu_rfrm_field_TCM6_TRANSPARENT_set(NULL, odu_rfrm_handle, chnl_id, transparent_mode);
                if(mode == ODU_RFRM_PM_TCM_TT_TRANSPARENT)
                {
                    odu_rfrm_field_TCM6_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, 0);
                }  
                break;
            case ODU_RFRM_MESSAGE_TYPE_FLOATING_TCM:
                if(transparent_mode == 0)
                {
                    odu_rfrm_field_FLOATING_TCM_ID_set (NULL, odu_rfrm_handle, chnl_id, float_tcmi_src);
                }
                else
                {
                    odu_rfrm_field_FLOATING_TCM_ID_set (NULL, odu_rfrm_handle, chnl_id, 0);
                }
               break;
           case ODU_RFRM_MESSAGE_TYPE_PM_NIM: /* PM_NIM */
                odu_rfrm_field_PM_NIM_TRANSPARENT_set(NULL, odu_rfrm_handle, chnl_id, transparent_mode);
                break;
           case ODU_RFRM_MESSAGE_TYPE_PM:    
                /*no actions for PM layer */
                break;
            default:
                PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
        }   
    }        
   
    /* update XX_REMOVE_OH bits based on new maintenance signal configuration*/
    if(result == PMC_SUCCESS)
    {

        result = odu_rfrm_remove_oh_update(odu_rfrm_handle,
                                           chnl_id);
    }

    
    PMC_RETURN(result);
} /* odu_rfrm_pm_tcmi_mode_set */


/*******************************************************************************
* odu_rfrm_pm_tcmi_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function activates or deactivates the provisioned layer.
*
*   Prior to calling this API, the layer must have been configured with
*   odu_rfrm_pm_tcmi_cfg() API.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   pm_tcm_num          - PM or TCMi field instance: \n
*                         0         -- PM \n
*                         1 to 6    -- TCM1 to TCM6 \n
*                         7         -- Floating TCM (must set float_tcmi_src) \n
*                         8         -- PM NIM (non-intrusive monitoring)
*
* OUTPUTS:
*   *tcm_mode           - functional mode of the selected layer.
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*              0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_pm_tcmi_mode_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                           UINT32 chnl_id,
                                           odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                           odu_rfrm_pm_tcm_mode_t *tcm_mode)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 tcm_pool_id;
    UINT8 mode;
    UINT32 transparent;
    odu_rfrm_layer_struct_t tcm_layers[ODU_SINK_MAX_PM_TCM_LAYERS];
    UINT32 num_layers = 0;
    UINT32 i;   
    UINT32 float_tcmi_src = 0;

    /* variable declaration */
    PMC_ENTRY();

    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);


    if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
    {
        mode = odu_rfrm_field_MODE_PM_get(NULL, odu_rfrm_handle, chnl_id);
    }
    else 
    {
        transparent = 0;
        if (pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_FLOATING_TCM)
        {
            result = odu_rfrm_tcm_transparent_get(odu_rfrm_handle, chnl_id, pm_tcm_num, &transparent);
        }
        else
        {
            float_tcmi_src = odu_rfrm_field_FLOATING_TCM_ID_get(NULL, odu_rfrm_handle, chnl_id);

            if(float_tcmi_src == 0)
            {
                transparent = 1;
            }
            else
            {
                transparent = 0;
            }
        }


        if (transparent == 1)
        {
            mode = 0;
        } 
        else 
        {      
            /* find pool id */
            result = odu_rfrm_pool_id_return(odu_rfrm_handle, chnl_id, pm_tcm_num, &tcm_pool_id); 
            
            if (PMC_SUCCESS == result)
            {
                mode = odu_rfrm_field_MODE_get(NULL, odu_rfrm_handle, tcm_pool_id);
            }
        }
    }
   
    if (PMC_SUCCESS == result)
    {
        /* configure for each operating mode */
        switch (mode)
        {
            case 3:
                if (pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
                {
                    *tcm_mode = ODU_RFRM_PM_TCM_TT_OPERATIONAL;
                }
                else
                {
                      if (result == PMC_SUCCESS)
                      {
                          /*get the new rfrm layer_hierachy */
                          result = odu_rfrm_layer_hierarchy_get(odu_rfrm_handle,
                                                                chnl_id,
                                                                &tcm_layers,
                                                                &num_layers);
                      }
                      /* need ability to be able to return ODU_RFRM_PM_TCM_TT_OPERATIONAL
                      or ODU_RFRM_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED*/
                      for(i=0;i<num_layers;i++)
                      {
                          if(tcm_layers[i].pm_tcm_num == pm_tcm_num)
                          {
                              *tcm_mode= tcm_layers[i].tcm_mode;
                          }
                      }
                }
                break;
            case 2:
                 *tcm_mode = ODU_RFRM_PM_TCM_TT_MONITOR;
                 break;
            case 0:
                 *tcm_mode = ODU_RFRM_PM_TCM_TT_TRANSPARENT;
                 break;
            case 1:
                 *tcm_mode = ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR;
                 break;
            default:
                 PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
        }    
    }
       
    PMC_RETURN(result);
} /* odu_rfrm_pm_tcmi_mode_get */

/*******************************************************************************
* odu_rfrm_pm_int_enbl
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function initializes PM layer on an operational channel.
*
*   Prior to calling this API, the layer must have been configured with
*   odu_rfrm_pm_tcmi_cfg() API.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95
*   int_enbl            - controls whether defec and action changes of state
*                         contribute to generation of an interrupt.
*                         0 -- interrupt generation is disabled.
*                         1 -- defect or consequential action state changes may
*                              cause an interrupt to be generated.
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
PUBLIC PMC_ERROR odu_rfrm_pm_int_enbl(odu_rfrm_handle_t *odu_rfrm_handle,
                                      UINT32 chnl_id, UINT32 int_enbl)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);
    
    odu_rfrm_field_INT_EN_PM_set(NULL, odu_rfrm_handle, chnl_id, int_enbl);
    
    PMC_RETURN(result);
} /* odu_rfrm_pm_int_enbl */
    




/*******************************************************************************
* FUNCTION: odu_rfrm_int_transfer_done_clr()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Triggers a transfer of ODU_RFRM interrupt tables to the shadow registers
*   for reading by SW.
*   
*
* INPUTS:
*   odu_rfrm_handle     - Pointer to odu_rfrm handle
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_int_transfer_done_clr(odu_rfrm_handle_t    *odu_rfrm_handle)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    odu_rfrm_field_DEF_ACT_DONE_I_set_to_clear(NULL, odu_rfrm_handle, 1);

    PMC_RETURN(result);
} /* odu_rfrm_int_transfer_done_clr */




/*******************************************************************************
* FUNCTION: odu_rfrm_int_transfer_done_poll()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Polls RFRMs for completion of interrupt shadow register transfer.  
*   
*
* INPUTS:
*   odu_rfrm_handle     - Pointer to odu_rfrm handle
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_int_transfer_done_poll(odu_rfrm_handle_t    *odu_rfrm_handle)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          equal_value = 0x1;
    UINT32          max_count = 100;
    UINT32          delay_between_polls_in_ms = 10;

    PMC_ENTRY();


    result = odu_rfrm_field_DEF_ACT_DONE_I_poll(NULL, odu_rfrm_handle,
                                                equal_value,
                                                PMC_POLL_EQUAL,
                                                max_count,
                                                NULL, /* num_failed_polls */
                                                delay_between_polls_in_ms);

    PMC_RETURN(result);
} /* odu_rfrm_int_transfer_done_poll */




/*******************************************************************************
* FUNCTION: odu_rfrm_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   odu_rfrm_handle    - Pointer to subsystem handle
*   chnl_id            - Channel ID: Valid range: 0 - 95
*   int_table_ptr      - pointer to aggregation or channel interrupt table with
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_int_validate(odu_rfrm_handle_t       *odu_rfrm_handle,
                                       UINT32                   chnl_id,
                                       void                    *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != odu_rfrm_handle, ODU_RFRM_ERR_INVALID_PTR, 0, 0);
    
    if (chnl_id >= ODU_RFRM_NUM_CHNL_GET(odu_rfrm_handle))
    {
        result = ODU_RFRM_LOG_ERR_INVALID_PARAMETERS;
    }

    if (NULL == int_table_ptr)
    {
        result = ODU_RFRM_ERR_INVALID_PTR;
    }


    PMC_RETURN(result);
} /* odu_rfrm_int_validate */

/*******************************************************************************
* FUNCTION: odu_rfrm_int_pid_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for PID interrupt functions.
*   
*
* INPUTS:
*   odu_rfrm_handle    - Pointer to subsystem handle
*   pool_id            - Pool id
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_int_pid_validate(odu_rfrm_handle_t       *odu_rfrm_handle,
                                           UINT32                   pool_id)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != odu_rfrm_handle, ODU_RFRM_ERR_INVALID_PTR, 0, 0);
    
    if (pool_id >= ODU_RFRM_NUM_PID_CHNL_GET(odu_rfrm_handle))
    {
        result = ODU_RFRM_LOG_ERR_INVALID_PARAMETERS;
    }



    PMC_RETURN(result);
} /* odu_rfrm_int_pid_validate */


/*******************************************************************************
* FUNCTION: odu_rfrm_int_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   odu_rfrm_handle   - Pointer to subsystem handle
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
PUBLIC PMC_ERROR odu_rfrm_int_enable(odu_rfrm_handle_t     *odu_rfrm_handle,
                                     odu_rfrm_int_t        *int_table_ptr,
                                     odu_rfrm_int_t        *int_en_table_ptr,
                                     BOOL8                  enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(NULL != odu_rfrm_handle, ODU_RFRM_ERR_INVALID_PTR, 0, 0);


    #ifdef ODU_RFRM_INT_SINGLE
    #undef ODU_RFRM_INT_SINGLE
    #endif
    #define ODU_RFRM_INT_SINGLE UTIL_GLOBAL_INT_ENABLE_SINGLE

    ODU_RFRM_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* odu_rfrm_int_enable */



/*******************************************************************************
* FUNCTION: odu_rfrm_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the interrupt table.
*   
*
* INPUTS:
*   odu_rfrm_handle   - Pointer to subsystem handle
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
PUBLIC PMC_ERROR odu_rfrm_int_clear(odu_rfrm_handle_t      *odu_rfrm_handle,
                                    odu_rfrm_int_t         *int_table_ptr,
                                    odu_rfrm_int_t         *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != odu_rfrm_handle, ODU_RFRM_ERR_INVALID_PTR, 0, 0);
    

#ifdef ODU_RFRM_INT_SINGLE
#undef ODU_RFRM_INT_SINGLE
#endif
#define ODU_RFRM_INT_SINGLE UTIL_GLOBAL_INT_CLEAR_SINGLE
    
    ODU_RFRM_INT_TABLE_DEFINE();
    
    PMC_RETURN(result);
} /* odu_rfrm_int_clear */



/*******************************************************************************
* FUNCTION: odu_rfrm_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the unchannelized interrupt table.
*   
*
* INPUTS:
*   odu_rfrm_handle   - Pointer to subsystem handle
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
PUBLIC PMC_ERROR odu_rfrm_int_retrieve(odu_rfrm_handle_t   *odu_rfrm_handle,
                                       odu_rfrm_int_t      *filt_table_ptr,
                                       odu_rfrm_int_t      *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
 
    PMC_ENTRY();

    PMC_ASSERT(NULL != odu_rfrm_handle, ODU_RFRM_ERR_INVALID_PTR, 0, 0);
    
    if (NULL == int_table_ptr)
    {
        result = ODU_RFRM_ERR_INVALID_PTR;
    }

    if (PMC_SUCCESS == result)
    {
        UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odu_rfrm_handle, odu_rfrm, UTIL_GLOBAL_INT_OPTIMIZE);
    
        /* Interrupt Bits */
#ifdef ODU_RFRM_INT_SINGLE
#undef ODU_RFRM_INT_SINGLE
#endif
#define ODU_RFRM_INT_SINGLE UTIL_GLOBAL_INT_RETRIEVE_SINGLE

        ODU_RFRM_INT_TABLE_DEFINE();

        UTIL_GLOBAL_INT_BUFFER_FLUSH(odu_rfrm_handle, odu_rfrm);
    }

    PMC_RETURN(result);
} /* odu_rfrm_int_retrieve */



/*******************************************************************************
* FUNCTION: odu_rfrm_int_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   odu_rfrm_handle   - Pointer to subsystem handle
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
PUBLIC PMC_ERROR odu_rfrm_int_enabled_check(odu_rfrm_handle_t   *odu_rfrm_handle,
                                            odu_rfrm_int_t      *int_en_table_ptr,
                                            BOOL8               *int_found_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(int_en_table_ptr != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(int_found_ptr != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);


    #ifdef ODU_RFRM_INT_SINGLE
    #undef ODU_RFRM_INT_SINGLE
    #endif
    #define ODU_RFRM_INT_SINGLE UTIL_GLOBAL_INT_CHECK_SINGLE

    ODU_RFRM_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* odu_rfrm_int_enabled_check */



/*******************************************************************************
* FUNCTION: odu_rfrm_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   odu_rfrm_handle   - Pointer to subsystem handle
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
PUBLIC PMC_ERROR odu_rfrm_int_chnl_enable(odu_rfrm_handle_t     *odu_rfrm_handle,
                                          UINT32                 chnl_id,
                                          odu_rfrm_int_chnl_t   *int_table_ptr,
                                          odu_rfrm_int_chnl_t   *int_en_table_ptr,
                                          BOOL8                  enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;

    PMC_ENTRY();

    result = odu_rfrm_int_validate(odu_rfrm_handle, chnl_id, (void *) int_table_ptr);


    #ifdef ODU_RFRM_INT
    #undef ODU_RFRM_INT
    #endif
    #define ODU_RFRM_INT UTIL_GLOBAL_INT_ENABLE_RANGE
    
    ODU_RFRM_INT_CHNL_TABLE_DEFINE();


    #ifdef ODU_RFRM_INT_NOOP
    #undef ODU_RFRM_INT_NOOP
    #endif
    #define ODU_RFRM_INT_NOOP UTIL_GLOBAL_INT_NOOP


    #ifdef ODU_RFRM_INT
    #undef ODU_RFRM_INT
    #endif
    #define ODU_RFRM_INT(handle, tsb_name, tsb_field, tsb_comb_field, struct_field) \
    do {                                                                        \
        if (TRUE == int_table_ptr->struct_field) {                              \
            PMC_RETURN(ODU_RFRM_ERR_INT_INCORRECT_PM_API);                      \
        }                                                                       \
    } while(0)

    ODU_RFRM_INT_PM_TABLE_DEFINE();


    PMC_RETURN(result);
} /* odu_rfrm_int_chnl_enable */


/*******************************************************************************
* FUNCTION: odu_rfrm_int_pm_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables interrupts on an ODU RFRM PM channel
*   
*
* INPUTS:
*   odu_rfrm_handle   - Pointer to subsystem handle
*   chnl_id           - Channel ID
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
PUBLIC PMC_ERROR odu_rfrm_int_pm_chnl_enable(odu_rfrm_handle_t       *odu_rfrm_handle,
                                              UINT32                  chnl_id,
                                              BOOL8                   enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    
    PMC_ENTRY();

    result = odu_rfrm_int_validate(odu_rfrm_handle, chnl_id, &chnl_id);
    
    
    odu_rfrm_field_INT_EN_PM_set(NULL, odu_rfrm_handle, chnl_id, enable);

    if (TRUE == enable) 
    {
        pmc_bitarray_set_bit(odu_rfrm_handle->var.int_chnl_en, chnl_id);
    }
    else
    {
        pmc_bitarray_clr_bit(odu_rfrm_handle->var.int_chnl_en, chnl_id);
    }

    PMC_RETURN(result);
} /* odu_rfrm_int_pm_chnl_enable */



/*******************************************************************************
* FUNCTION: odu_rfrm_int_pid_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables interrupts on an ODU RFRM TCM pool id.
*   
*
* INPUTS:
*   odu_rfrm_handle   - Pointer to subsystem handle
*   pool_id           - Pool ID
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
PUBLIC PMC_ERROR odu_rfrm_int_pid_chnl_enable(odu_rfrm_handle_t       *odu_rfrm_handle,
                                              UINT32                   pool_id,
                                              BOOL8                    enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    result = odu_rfrm_int_pid_validate(odu_rfrm_handle, pool_id);
    
    odu_rfrm_field_INT_EN_set(NULL, odu_rfrm_handle, pool_id, enable);

    if (TRUE == enable) 
    {
        pmc_bitarray_set_bit(odu_rfrm_handle->var.int_pid_en, pool_id);
    }
    else
    {
        pmc_bitarray_clr_bit(odu_rfrm_handle->var.int_pid_en, pool_id);
    }

    PMC_RETURN(result);
} /* odu_rfrm_int_pid_chnl_enable */


/*******************************************************************************
* FUNCTION: odu_rfrm_int_pid_chnl_tbl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   odu_rfrm_handle   - Pointer to subsystem handle
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
PUBLIC PMC_ERROR odu_rfrm_int_pid_chnl_tbl_enable(odu_rfrm_handle_t       *odu_rfrm_handle,
                                                  UINT32                   chnl_id,
                                                  odu_rfrm_int_pid_chnl_t *int_table_ptr,
                                                  odu_rfrm_int_pid_chnl_t *int_en_table_ptr,
                                                  BOOL8                     enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    result = odu_rfrm_int_pid_validate(odu_rfrm_handle, chnl_id);
    
    #ifdef ODU_RFRM_INT_NOOP
    #undef ODU_RFRM_INT_NOOP
    #endif
    #define ODU_RFRM_INT_NOOP UTIL_GLOBAL_INT_NOOP
    
    #ifdef ODU_RFRM_INT
    #undef ODU_RFRM_INT
    #endif
    #define ODU_RFRM_INT(handle, tsb_name, tsb_field, tsb_comb_field, struct_field) \
    do {                                                                        \
        if (TRUE == int_table_ptr->struct_field) {                              \
            PMC_RETURN(ODU_RFRM_ERR_INT_INCORRECT_PID_API);                     \
        }                                                                       \
    } while(0)

    ODU_RFRM_INT_PID_TABLE_DEFINE();




    PMC_RETURN(result);
} /* odu_rfrm_int_pid_chnl_tbl_enable */



/*******************************************************************************
* FUNCTION: odu_rfrm_int_defect_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disable specified defect.
*   
*
* INPUTS:
*   odu_rfrm_handle   - Pointer to subsystem handle
*   defect            - defect type
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
PUBLIC PMC_ERROR odu_rfrm_int_defect_enable(odu_rfrm_handle_t       *odu_rfrm_handle,
                                            odu_rfrm_int_defect_t    defect,
                                            BOOL8                    enable)
{
    PMC_ERROR               result = PMC_SUCCESS;
    
    /* Combined Interrupt Information */
    pmc_bitarray_t         *int_comb_en_ptr = odu_rfrm_handle->var.int_comb_en; 

    const defect_enable_fn  defect_enable[LAST_ODU_RFRM_INT_DEFECT] = 
    {
        odu_rfrm_field_COMB_DLCK_E_set,    /* ODU_RFRM_INT_DEFECT_COMB_DLCK */
        odu_rfrm_field_COMB_DOCI_E_set,    /* ODU_RFRM_INT_DEFECT_COMB_DOCI */ 
        odu_rfrm_field_COMB_DAIS_E_set,    /* ODU_RFRM_INT_DEFECT_COMB_DAIS */   
        odu_rfrm_field_COMB_DTIM_E_set,    /* ODU_RFRM_INT_DEFECT_COMB_DTIM */   
        odu_rfrm_field_COMB_DDEG_E_set,    /* ODU_RFRM_INT_DEFECT_COMB_DDEG */   
        odu_rfrm_field_COMB_DBDI_E_set,    /* ODU_RFRM_INT_DEFECT_COMB_DBDI */   
        odu_rfrm_field_COMB_DLTC_E_set,    /* ODU_RFRM_INT_DEFECT_COMB_DLTC */   
        odu_rfrm_field_COMB_DIAE_E_set,    /* ODU_RFRM_INT_DEFECT_COMB_DIAE */   
        odu_rfrm_field_COMB_DBIAE_E_set,   /* ODU_RFRM_INT_DEFECT_COMB_DBIAE */  
        odu_rfrm_field_COMB_DLOFLOM_E_set, /* ODU_RFRM_INT_DEFECT_COMB_DLOFLOM */
        odu_rfrm_field_COMB_OOF_E_set,     /* ODU_RFRM_INT_DEFECT_COMB_OOF */ 
        NULL,                              /* 11 - unused */
        odu_rfrm_field_COMB_ATSF_E_set,    /* ODU_RFRM_INT_DEFECT_COMB_ATSF */ 
        odu_rfrm_field_COMB_ATSD_E_set,    /* ODU_RFRM_INT_DEFECT_COMB_ATSD */  
        odu_rfrm_field_COMB_AAIS_E_set,    /* ODU_RFRM_INT_DEFECT_COMB_AAIS */  
        odu_rfrm_field_COMB_ABDI_E_set,    /* ODU_RFRM_INT_DEFECT_COMB_ABDI */  
    };

    PMC_ENTRY();

    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);

    if (NULL == defect_enable[defect])
    {
        PMC_RETURN(result);
    }

    /* Enable/disable odu rfrm defect */
    defect_enable[defect](NULL, odu_rfrm_handle, enable);
    
    /* Mark defect as enabled/disabled */
    if (TRUE == enable) 
    {
        pmc_bitarray_set_bit(int_comb_en_ptr, defect);
    }
    else
    {
        pmc_bitarray_clr_bit(int_comb_en_ptr, defect);
    }

    PMC_RETURN(result);
} /* odu_rfrm_int_defect_enable */


/*******************************************************************************
* FUNCTION: odu_rfrm_int_summary_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves summary of ODU RFRM interrupts.
*   
*
* INPUTS:
*   odu_rfrm_handle   - Pointer to odu_rfrm handle
*   summary_type      - Type of summary to retrieve
*
* OUTPUTS:
*   bitmask_ptr      - Bit mask of active ints/chnls
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_int_summary_get(odu_rfrm_handle_t             *odu_rfrm_handle,
                                          odu_rfrm_int_summary_type_t    summary_type,
                                          UINT32                        *bitmask_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(odu_rfrm_handle != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);
    PMC_ASSERT(bitmask_ptr     != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);

    /* Check if summary bits are valid */
    if (FALSE == odu_rfrm_handle->var.int_comb_valid) 
    {
        PMC_RETURN(ODU_RFRM_ERR_INT_COMB_BITS_NOT_VALID);
    }


    switch (summary_type)
    {
      case ODU_RFRM_INT_SUMMARY_TYPE_ACTIVE_PM:
        odu_rfrm_lfield_PM_DEF_I_get(NULL, odu_rfrm_handle, bitmask_ptr);
        break;
        
      case ODU_RFRM_INT_SUMMARY_TYPE_ACTIVE_PID:
        odu_rfrm_lfield_PID_DEF_I_get(NULL, odu_rfrm_handle, bitmask_ptr);
        break;
        
      case ODU_RFRM_INT_SUMMARY_TYPE_ACTIVE_INT:
        *bitmask_ptr = odu_rfrm_handle->var.int_comb;
        break;
    
      default:
        PMC_ASSERT(NULL, ODU_RFRM_ERR_CODE_ASSERT, 0, 0);
        break;
    } /* end switch */
    


    PMC_RETURN(result);
} /* odu_rfrm_int_summary_get */


/*******************************************************************************
* FUNCTION: odu_rfrm_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   odu_rfrm_handle   - Pointer to subsystem handle
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
PUBLIC PMC_ERROR odu_rfrm_int_chnl_retrieve(odu_rfrm_handle_t        *odu_rfrm_handle,
                                            UINT32                    chnl_id,
                                            odu_rfrm_int_chnl_t      *filt_table_ptr,
                                            odu_rfrm_int_chnl_t      *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link1 = chnl_id;
    UINT32          link2 = chnl_id;

    PMC_ENTRY();

    result = odu_rfrm_int_validate(odu_rfrm_handle, chnl_id, (void *) int_table_ptr);


    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odu_rfrm_handle, odu_rfrm, UTIL_GLOBAL_INT_OPTIMIZE);
    
    /* Interrupt Bits */
    #ifdef ODU_RFRM_INT
    #undef ODU_RFRM_INT
    #endif
    #define ODU_RFRM_INT UTIL_GLOBAL_INT_RETRIEVE_RANGE

    ODU_RFRM_INT_CHNL_TABLE_DEFINE();
    
    
    #ifdef ODU_RFRM_INT_NOOP
    #undef ODU_RFRM_INT_NOOP
    #endif
    #define ODU_RFRM_INT_NOOP UTIL_GLOBAL_INT_NOOP
    
    
    #ifdef ODU_RFRM_INT
    #undef ODU_RFRM_INT
    #endif
    #define ODU_RFRM_INT    UTIL_GLOBAL_INT_ODU_RFRM_RETRIEVE

    ODU_RFRM_INT_PM_TABLE_DEFINE();
    
    
    
    /* Status Bits */
    #ifdef ODU_RFRM_STATUS_NOOP
    #undef ODU_RFRM_STATUS_NOOP
    #endif
    #define ODU_RFRM_STATUS_NOOP UTIL_GLOBAL_INT_NOOP
    
    #ifdef ODU_RFRM_STATUS
    #undef ODU_RFRM_STATUS
    #endif
    #define ODU_RFRM_STATUS     UTIL_GLOBAL_STATUS_ODU_RFRM_RETRIEVE
    
    ODU_RFRM_STATUS_PM_TABLE_DEFINE();

    UTIL_GLOBAL_INT_BUFFER_FLUSH(odu_rfrm_handle, odu_rfrm);

    PMC_RETURN(result);
} /* odu_rfrm_int_chnl_retrieve */


/*******************************************************************************
* FUNCTION: odu_rfrm_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Clear interrupts based on the bits set in the channel interrupt table
*   
*
* INPUTS:
*   odu_rfrm_handle   - Pointer to subsystem handle
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   int_table_ptr     - Pointer to channel interrupt table  or NULL. 
*                       Currently only AC_APS interrupt will be cleared.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_int_chnl_clear(odu_rfrm_handle_t        *odu_rfrm_handle,
                                         UINT32                    chnl_id,
                                         odu_rfrm_int_chnl_t      *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    if (chnl_id >= ODU_RFRM_NUM_CHNL_GET(odu_rfrm_handle))
    {
        PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odu_rfrm_handle, odu_rfrm, 
                                       UTIL_GLOBAL_INT_OPTIMIZE);

    /* Interrupt Bits */
    #ifdef ODU_RFRM_INT_CHNL_RANGE
    #undef ODU_RFRM_INT_CHNL_RANGE
    #endif
    #define ODU_RFRM_INT_CHNL_RANGE UTIL_GLOBAL_INT_LFIELD_CLEAR_SINGLE

    ODU_RFRM_INT_CHNL_TABLE_CLEAR_DEFINE();

    PMC_RETURN(result);
} /* odu_rfrm_int_chnl_clear */



/*******************************************************************************
* FUNCTION: odu_rfrm_int_pid_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   odu_rfrm_handle   - Pointer to subsystem handle
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   pool_id_ptr       - Pointer to array of TCM PIDs
*   filt_table_ptr    - pointer to filter table. Mark fields as TRUE within this
*                       table to indicate that the interrupt/status will be retrieved
*                       to the output table. NULL to retrieve all 
*                       interrupt/status.
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
PUBLIC PMC_ERROR odu_rfrm_int_pid_chnl_retrieve(odu_rfrm_handle_t         *odu_rfrm_handle,
                                                UINT32                     chnl_id,
                                                UINT32                    *pool_id_ptr,
                                                odu_rfrm_int_pid_chnl_t   *filt_table_ptr,
                                                UINT32                    *int_table_pid_ptr,
                                                odu_rfrm_int_pid_chnl_t   *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          pool_id;
    UINT32          i;

    PMC_ENTRY();

    result = odu_rfrm_int_validate(odu_rfrm_handle, chnl_id, (void*) int_table_ptr);
    

    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(odu_rfrm_handle, odu_rfrm, UTIL_GLOBAL_INT_OPTIMIZE);

    /* Get TCM pool id's for given RFRM chnl */
    for (i = 0; i < ODU_RFRM_PIDS_PER_CHNL; i++)
    {
        pool_id = pool_id_ptr[i];
        
        /* Store TCM pool id in interrupt table */
        int_table_pid_ptr[i] = pool_id;

        /* Retrieve pool id interrupts if PID is valid*/
        if (ODU_RFRM_TCM_PID_IS_VALID(pool_id))
        {
            /* Interrupt bits */
            #ifdef ODU_RFRM_INT
            #undef ODU_RFRM_INT
            #endif
            #define ODU_RFRM_INT    UTIL_GLOBAL_INT_ODU_RFRM_PID_RETRIEVE

            ODU_RFRM_INT_PID_TABLE_DEFINE();
            
            
            
            /* Status bits */
            #ifdef ODU_RFRM_STATUS
            #undef ODU_RFRM_STATUS
            #endif
            #define ODU_RFRM_STATUS     UTIL_GLOBAL_STATUS_ODU_RFRM_PID_RETRIEVE

            ODU_RFRM_STATUS_PID_TABLE_DEFINE();
        }
    } /* end for */
    
    UTIL_GLOBAL_INT_BUFFER_FLUSH(odu_rfrm_handle, odu_rfrm);

    PMC_RETURN(result);
} /* odu_rfrm_int_pid_chnl_retrieve */




/*******************************************************************************
* FUNCTION: odu_rfrm_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   odu_rfrm_handle   - Pointer to subsystem handle
*   chnl_id           - Channel ID: Valid range: 0 - 95
*   int_summary_ptr   - pointer to OTN ODU interrupt summary information
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
PUBLIC PMC_ERROR odu_rfrm_int_chnl_enabled_check(odu_rfrm_handle_t        *odu_rfrm_handle,
                                                 UINT32                    chnl_id,
                                                 odu_rfrm_int_summary_t   *int_summary_ptr,
                                                 odu_rfrm_int_chnl_t      *int_en_table_ptr,
                                                 BOOL8                    *int_found_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;
    UINT32                  link1 = chnl_id;
    UINT32                  link2 = chnl_id;
    UINT32                  i;
    UINT32                  pool_id;

    /* Combined Interrupt Information */
    pmc_bitarray_t         *int_comb_en_ptr = odu_rfrm_handle->var.int_comb_en; 
    pmc_bitarray_t         *int_chnl_en_ptr = odu_rfrm_handle->var.int_chnl_en; 
    pmc_bitarray_t         *int_pid_en_ptr  = odu_rfrm_handle->var.int_pid_en; 
    pmc_bitarray_t          active_int_bitarray[PMC_BITARRAY_SIZE(LAST_ODU_RFRM_INT_DEFECT)];

    PMC_ENTRY();

    result = odu_rfrm_int_validate(odu_rfrm_handle, chnl_id, int_found_ptr);
    PMC_ASSERT(int_summary_ptr != NULL, ODU_RFRM_ERR_INVALID_PTR, 0, 0);




    

    #ifdef ODU_RFRM_INT
    #undef ODU_RFRM_INT
    #endif
    #define ODU_RFRM_INT UTIL_GLOBAL_INT_CHECK_RANGE

    ODU_RFRM_INT_CHNL_TABLE_DEFINE();




    if (FALSE == *int_found_ptr)
    {
        BOOL active_chnl = FALSE;


        /* Check if channel has an active interrupt */
        if (pmc_bitarray_is_bit_set(int_summary_ptr->active_chnl_bitmask, chnl_id) &&
            pmc_bitarray_is_bit_set(int_chnl_en_ptr, chnl_id))
        {
            active_chnl = TRUE;
        }
        

        /* Check if the pool ids associated with the chnl have an active interrupt */
        for (i = 0; (i < ODU_RFRM_PIDS_PER_CHNL) && (FALSE == active_chnl); i++)
        {
            pool_id = int_summary_ptr->pool_id[i]; 
            
            if (ODU_RFRM_TCM_PID_IS_VALID(pool_id) &&
                pmc_bitarray_is_bit_set(int_summary_ptr->active_pid_bitmask, pool_id) &&
                pmc_bitarray_is_bit_set(int_pid_en_ptr, pool_id))
            {
                active_chnl = TRUE;
            }
        } 



        /* Check if enabled defect type is active in combined status */
        if (TRUE == active_chnl)
        {
            pmc_bitarray_zero(active_int_bitarray,
                              LAST_ODU_RFRM_INT_DEFECT);

            /* Store current active interrupts */
            pmc_bitarray_or(active_int_bitarray,
                            int_summary_ptr->active_int_bitmask,
                            LAST_ODU_RFRM_INT_DEFECT);

            /* Compare active and enabled interrupts */
            pmc_bitarray_and(active_int_bitarray,
                             int_comb_en_ptr,
                             LAST_ODU_RFRM_INT_DEFECT);

            if (FALSE == pmc_bitarray_is_all_zeroes(active_int_bitarray, LAST_ODU_RFRM_INT_DEFECT))
            {
                *int_found_ptr = TRUE;
            }
        }
    }



    PMC_RETURN(result);
} /* odu_rfrm_int_chnl_enabled_check */



/*******************************************************************************
* FUNCTION: odu_rfrm_int_start()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Stores summary interrupt bits in context so that we can still get the
*   information in int_summary_get().
*   Clears the active summary interrupt bits (COMB_x_I)
*
* INPUTS:
*   odu_rfrm_handle     - Pointer to odu_rfrm handle
*   start               - TRUE for RFRM interrupt processing start,
*                         FALSE for interrupt processing end
*   clear               - Whether the interrupts should be cleared or not
*                         after being collected.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_int_start(odu_rfrm_handle_t    *odu_rfrm_handle,
                                    BOOL                  start,
                                    BOOL8                 clear)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          active_int;

    PMC_ENTRY();
   
    if (TRUE == start)
    {
        active_int = odu_rfrm_reg_DEFECT_INT_STAT_read(NULL, odu_rfrm_handle);
        if (TRUE == clear)
        {
            odu_rfrm_reg_DEFECT_INT_STAT_write(NULL, odu_rfrm_handle, 
                                               active_int); 
        }

        /* 
         * Store combined interrupt register (so that we can later call 
         * int_summary_get) 
         * Or the active_int in case this is called multiple times to 
         * ensure that current interrupts are not inadvertently cleared.
         */
        odu_rfrm_handle->var.int_comb_valid = TRUE;
        odu_rfrm_handle->var.int_comb       |= active_int;
    }
    else
    {
        odu_rfrm_handle->var.int_comb_valid = FALSE;
        odu_rfrm_handle->var.int_comb       = 0;
    }

    PMC_RETURN(result);
} /* odu_rfrm_int_start */

/*******************************************************************************
*  odu_rfrm_tcm_pool_id_search
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function searches through the pool and finds the corresponding
*   pool pool ID for the TCM layer of the channel.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - channel ID
*   tcm_num             - TCM layer number for the channel \n
*                         1 to 6 -- TCM1 to TCM6 \n
*                         7      -- Floating TCM \n
*                         8      -- PM NIM
*
* OUTPUTS:
*   *pool_id            - pool_id
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_tcm_pool_id_search(odu_rfrm_handle_t *odu_rfrm_handle,
                                             UINT32 chnl_id, 
                                             odu_rfrm_pm_tcm_id_t tcm_num,
                                             UINT32 *pool_id)
{
    /* variable declaration */
    UINT32 tcm_pool_id;

    PMC_ENTRY();

    switch (tcm_num)
    {
        case ODU_RFRM_MESSAGE_TYPE_TCM1:
            tcm_pool_id = odu_rfrm_field_TCM1_POOL_ID_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        case ODU_RFRM_MESSAGE_TYPE_TCM2:
            tcm_pool_id = odu_rfrm_field_TCM2_POOL_ID_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        case ODU_RFRM_MESSAGE_TYPE_TCM3:
            tcm_pool_id = odu_rfrm_field_TCM3_POOL_ID_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        case ODU_RFRM_MESSAGE_TYPE_TCM4:
            tcm_pool_id = odu_rfrm_field_TCM4_POOL_ID_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        case ODU_RFRM_MESSAGE_TYPE_TCM5:
            tcm_pool_id = odu_rfrm_field_TCM5_POOL_ID_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        case ODU_RFRM_MESSAGE_TYPE_TCM6:
            tcm_pool_id = odu_rfrm_field_TCM6_POOL_ID_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        case ODU_RFRM_MESSAGE_TYPE_FLOATING_TCM:
            tcm_pool_id = odu_rfrm_field_FLOATING_TCM_POOL_ID_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        case ODU_RFRM_MESSAGE_TYPE_PM_NIM:
            tcm_pool_id = odu_rfrm_field_PM_NIM_POOL_ID_get(NULL, odu_rfrm_handle, chnl_id);
            break;
        default:
            PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }

    PMC_LOG_TRACE("%s, %s, %d, tcm_pool_id = %u...\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);

    if (tcm_pool_id > 255)
    {
        PMC_LOG_TRACE("%s, %s, %d, index for TCM layer is out of range = %u...\n", __FILE__, __FUNCTION__, __LINE__, tcm_pool_id);
        PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }

    *pool_id = tcm_pool_id;

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_tcm_pool_id_search */

/*******************************************************************************
*  odu_rfrm_tcm_pool_id_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function checks if the retrieved pool id is a valid ID.
*   If the retrieved pool ID is not assigned previously, the pool ID
*   is not valid.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   pool_id             - TCM POOL ID
*   chnl_id             - Channel identified associated with the TCM
*   tcm_num             - TCM Number used for this pool id.
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
PUBLIC BOOL8 odu_rfrm_tcm_pool_id_check(odu_rfrm_handle_t *odu_rfrm_handle,
                                        UINT32 pool_id,
                                        UINT32 chnl_id,
                                        odu_rfrm_pm_tcm_id_t tcm_num)
{
    /* variable declaration */
    BOOL8 valid;

    PMC_ENTRY();

    if (pool_id >= TCM_POOL_SIZE_96)
    {
        valid = FALSE;
    }
    else if (FALSE == odu_rfrm_handle->var.tcm_pool_ptr[pool_id].assign_state ||
             chnl_id != odu_rfrm_handle->var.tcm_pool_ptr[pool_id].chnl_id ||
             tcm_num != odu_rfrm_handle->var.tcm_pool_ptr[pool_id].tcm_num)
    {
        valid = FALSE;
    }
    else
    {
        valid = TRUE;
    }

    PMC_RETURN(valid);
} /* odu_rfrm_tcm_pool_id_check */

/*******************************************************************************
*  odu_rfrm_tcm_pool_id_ctxt_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function compares the pool id retrieved from the register and the pool
*   id stored in the SW TCM pool context.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed 
*                         Legal range: 0 - 95
*   pm_tcm_num          - PM NIM or TCMi field instance: \n
*                         1 to 6  -- TCM1 - TCM6 \n
*                         7       -- Floating TCM \n
*                         8       -- PM NIM
*   pool_id             - TCM pool POOL ID
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
PUBLIC PMC_ERROR odu_rfrm_tcm_pool_id_ctxt_audit(odu_rfrm_handle_t *odu_rfrm_handle,
                                                 UINT32 chnl_id, odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                                 UINT32 pool_id)
{
    /* variable declaration */
    odu_rfrm_var_t var;
    odu_rfrm_tcm_pool_t tcm_pool;

    PMC_ENTRY();

    /* retreive cfg struct for the handle */
    var = (odu_rfrm_handle->var);
    tcm_pool = var.tcm_pool_ptr[pool_id];

    if ((tcm_pool.assign_state) == 0)
    {
        PMC_LOG_TRACE("%s, %s, %d, tcm_pool_ptr[%u] = %u... is unassigned.\n", __FILE__, __FUNCTION__, __LINE__, pool_id, tcm_pool.assign_state);
        PMC_RETURN(ODU_RFRM_LOG_CODE_POOL_ID_UNASSIGNED);
    }
    else
    {
        /* check chnl_id and tcm_num context info in the pool */
        if (((tcm_pool.chnl_id) != chnl_id) || ((tcm_pool.tcm_num) != pm_tcm_num))
        {
            PMC_RETURN(ODU_RFRM_LOG_CODE_POOL_ID_MISMATCH);
        }
        else
        {
            PMC_RETURN(PMC_SUCCESS);
        }
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_tcm_pool_id_ctxt_audit */



/*******************************************************************************
*  odu_rfrm_tcm_pool_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function updates the pool entry state when a TCM, Floating TCM or
*   PM_NIM layer is provisioned or unprovisioned. When a new layer is added,
*   the state of the pool entry is updated to 'Assigned'. When a layer is
*   removed, the state of the pool entry is updated to 'Unassigned'.
*
*   The total number of free entries available in the pool will be updated
*   accordingly in this function.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID
*                         Legal range: 0 - 95
*   pm_tcm_num          - PM or TCMi layer \n
*                         1 to 6 -- TCM to TCM6 \n
*                         7      -- Floating TCM \n
*                         8      -- PM NIM
*   pool_id             - TCM pool POOL ID
*   assign_state        - pool entry state \n
*                         1 -- Assigned \n
*                         0 -- Unassigned (Free)
*   tcm_mode            - Operating mode of the pool resource.
*                         See odu_rfrm_pm_tcm_mode_t.
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
PRIVATE PMC_ERROR odu_rfrm_tcm_pool_update(odu_rfrm_handle_t *odu_rfrm_handle,
                                           UINT32 chnl_id,
                                           odu_rfrm_pm_tcm_id_t pm_tcm_num,
                                           UINT32 pool_id, BOOL8 assign_state,
                                           odu_rfrm_pm_tcm_mode_t tcm_mode)
{

    PMC_ENTRY();

    /*
    BOOL8 assign_state;
    UINT32 chnl_id;
    odu_rfrm_pm_tcm_id_t tcm_num;
    odu_rfrm_pm_tcm_mode_t tcm_mode;
    odu_rfrm_pm_tcm_id_t parent_tcm_num;
    UINT32 parent_pool_id;
    */
    PMC_LOG_TRACE("%s, %s, %d, chnl_id = %u, pm_tcm_num = %u, pool_id = %u, state = %u\n", __FILE__, __FUNCTION__, __LINE__, chnl_id, pm_tcm_num, pool_id, assign_state);

    if (assign_state == 0)
    {
        /* disabled state. layer is removed from the hierarchy */
        odu_rfrm_handle->var.tcm_pool_ptr[pool_id].tcm_num = ODU_RFRM_MESSAGE_DONT_CARE;
        odu_rfrm_handle->var.tcm_pool_ptr[pool_id].chnl_id = 0xFF;
        odu_rfrm_handle->var.tcm_pool_ptr[pool_id].tcm_mode = tcm_mode;

        /* Update the number of free entries in the pool */
        odu_rfrm_handle->var.num_pool_free =+ 1;
    }
    else
    {
        odu_rfrm_handle->var.tcm_pool_ptr[pool_id].tcm_num = pm_tcm_num;
        odu_rfrm_handle->var.tcm_pool_ptr[pool_id].chnl_id = chnl_id;
        odu_rfrm_handle->var.tcm_pool_ptr[pool_id].tcm_mode = tcm_mode;

        /* if this is the first time assigning this pool id
        ** then decrement the number of available pool ids
        */
        if (odu_rfrm_handle->var.tcm_pool_ptr[pool_id].assign_state != 1)
        {
            /* Update the number of free entries in the pool */
            odu_rfrm_handle->var.num_pool_free =- 1;
        }
    }
    
    /* Update the pool entry state */
    odu_rfrm_handle->var.tcm_pool_ptr[pool_id].assign_state = assign_state;

    PMC_LOG_TRACE("%s, %s, %d, state of tcm_pool_ptr[%u] = %u....\n", __FILE__, __FUNCTION__, __LINE__, pool_id, odu_rfrm_handle->var.tcm_pool_ptr[pool_id].assign_state);
    PMC_LOG_TRACE("%s, %s, %d, TCM number of tcm_pool_ptr[%u] = %u....\n", __FILE__, __FUNCTION__, __LINE__, pool_id, odu_rfrm_handle->var.tcm_pool_ptr[pool_id].tcm_num);
    PMC_LOG_TRACE("%s, %s, %d, TCM state of tcm_pool_ptr[%u] = %u....\n", __FILE__, __FUNCTION__, __LINE__, pool_id, odu_rfrm_handle->var.tcm_pool_ptr[pool_id].tcm_mode);
    PMC_LOG_TRACE("%s, %s, %d, Channel ID of tcm_pool_ptr[%u] = %u....\n", __FILE__, __FUNCTION__, __LINE__, pool_id, odu_rfrm_handle->var.tcm_pool_ptr[pool_id].chnl_id);

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_tcm_pool_update */

/*******************************************************************************
*  odu_rfrm_dci_child_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retreives all non-transparent sub-layers in the layer 
*   hierarchy on a given ODU channel.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - channel ID to request TCM layer information on
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   child_tcm_pm_id_ptr     - The TCM/PM/PM-NIM type of the child connected to DCI
*                         See odu_rfrm_pm_tcm_id_t (if no child connected,
*                         returns ODU_RFRM_MESSAGE_DONT_CARE).
*   child_pool_id_ptr       - The pool id of the child connected to DCI.
*                         If no child connect, or if the child is the
*                         PM layer, returns ODU_RFRM_UNASSIGNED_POOL_ID
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_dci_child_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                        UINT32 chnl_id,
                                        UINT32 *child_pool_id_ptr,
                                        odu_rfrm_pm_tcm_id_t *child_tcm_pm_id_ptr)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
   
    UINT32 i;
    UINT32 count = 0;
    UINT32 child_pool_id;
    odu_rfrm_pm_tcm_id_t child_tcm_pm_id;
    odu_rfrm_tcm_pool_t *tcm_pool_ptr;
    UINT32 tcm_pool_size;
    
    PMC_ENTRY();

    tcm_pool_ptr = odu_rfrm_handle->var.tcm_pool_ptr;
    tcm_pool_size = odu_rfrm_handle->cfg.tcm_pool_size;

    child_pool_id = ODU_RFRM_UNASSIGNED_POOL_ID;
    child_tcm_pm_id = ODU_RFRM_MESSAGE_DONT_CARE;


    child_tcm_pm_id = odu_rfrm_handle->var.dci_pm_tcm_child[chnl_id];

    for (i = 0; i < tcm_pool_size; i++)
    {
        if(tcm_pool_ptr[i].chnl_id == chnl_id)
        {

            if(tcm_pool_ptr[i].tcm_num == child_tcm_pm_id){
                child_pool_id = i;
                child_tcm_pm_id = tcm_pool_ptr[i].tcm_num;
                count++;
            }

        }
 
    }        
    
    if(count == 0){
        if(odu_rfrm_handle->var.pm_provisioned[chnl_id] == TRUE){
            child_tcm_pm_id = ODU_RFRM_MESSAGE_TYPE_PM;
        }
    }
    *child_tcm_pm_id_ptr = child_tcm_pm_id;
    *child_pool_id_ptr = child_pool_id;
    if(count > 1){
        PMC_RETURN(ODU_RFRM_ERR_DCI_CHILD_GET);
    }
     
    PMC_RETURN(result);
} /* odu_rfrm_dci_child_get */

/*******************************************************************************
*  odu_rfrm_layer_hierarchy_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retreives all non-transparent sub-layers in the layer 
*   hierarchy on a given ODU channel.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - channel ID to request TCM layer information on
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   tcm_layers          - array of 9 odu_rfrm_layer_struct_t elements
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
PUBLIC PMC_ERROR odu_rfrm_layer_hierarchy_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                              UINT32 chnl_id,
                                              odu_rfrm_layer_struct_t (*tcm_layers)[ODU_SINK_MAX_PM_TCM_LAYERS],
                                              UINT32 *num_layers)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
   
    UINT32 i;
    UINT32 dci_child_pool_id = ODU_RFRM_UNASSIGNED_POOL_ID;
    odu_rfrm_pm_tcm_id_t dci_child_pm_tcm_id;
    BOOL8 layer_list_complete = FALSE;
    
    PMC_ENTRY();

    /* initialize output variables */
    for(i=0;i<ODU_SINK_MAX_PM_TCM_LAYERS;i++){
        (*tcm_layers)[i].pool_id = ODU_RFRM_UNASSIGNED_POOL_ID;
        (*tcm_layers)[i].pm_tcm_num = ODU_RFRM_MESSAGE_DONT_CARE;
        (*tcm_layers)[i].tcm_mode = ODU_RFRM_PM_TCM_START;
        (*tcm_layers)[i].maint_sig = 0;
    }

    *num_layers = 0;

    result = odu_rfrm_dci_child_get(odu_rfrm_handle,
                                    chnl_id,
                                    &dci_child_pool_id,
                                    &dci_child_pm_tcm_id);

    if(result == PMC_SUCCESS && dci_child_pm_tcm_id != ODU_RFRM_MESSAGE_DONT_CARE) {

        if(dci_child_pm_tcm_id == ODU_RFRM_MESSAGE_TYPE_PM){
        
            (*tcm_layers)[0].pm_tcm_num = ODU_RFRM_MESSAGE_TYPE_PM;
            (*tcm_layers)[0].tcm_mode = odu_rfrm_handle->var.pm_mode[chnl_id]; 
            (*num_layers) = 1;

        }
        else 
        {
            while(layer_list_complete == FALSE)
            {
                if( (*num_layers) == 0){
                    (*tcm_layers)[*num_layers].pool_id = dci_child_pool_id;
                    (*tcm_layers)[*num_layers].pm_tcm_num = dci_child_pm_tcm_id;
                    (*tcm_layers)[*num_layers].tcm_mode = odu_rfrm_handle->var.tcm_pool_ptr[dci_child_pool_id].tcm_mode;
                    (*tcm_layers)[*num_layers].maint_sig = odu_rfrm_handle->var.tcm_pool_ptr[dci_child_pool_id].maint_sig;
                }
                else
                {
                    /*get pool id from last layer */
                    (*tcm_layers)[*num_layers].pool_id = odu_rfrm_handle->var.tcm_pool_ptr[(*tcm_layers)[*num_layers-1].pool_id].child_pool_id;
                    (*tcm_layers)[*num_layers].pm_tcm_num = odu_rfrm_handle->var.tcm_pool_ptr[(*tcm_layers)[*num_layers-1].pool_id].child_pm_tcm_num;

                    /*get mode and child_pm_tcm_num from current layer */
                    if( (*tcm_layers)[*num_layers].pm_tcm_num != ODU_RFRM_MESSAGE_TYPE_PM )
                    {
                        (*tcm_layers)[*num_layers].tcm_mode = odu_rfrm_handle->var.tcm_pool_ptr[(*tcm_layers)[*num_layers].pool_id].tcm_mode;
                        /*get the maintenance signal of the current layer */
                        (*tcm_layers)[*num_layers].maint_sig = odu_rfrm_handle->var.tcm_pool_ptr[(*tcm_layers)[*num_layers].pool_id].maint_sig;  
                    }
                    else
                    {
                        (*tcm_layers)[*num_layers].tcm_mode = odu_rfrm_handle->var.pm_mode[chnl_id]; 
                    }
                
                }
                
                if( (*tcm_layers)[*num_layers].pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM 
                    || odu_rfrm_handle->var.tcm_pool_ptr[(*tcm_layers)[*num_layers].pool_id].child_pm_tcm_num == ODU_RFRM_MESSAGE_DONT_CARE )
                {
                    layer_list_complete = TRUE;
                } 
                
                (*num_layers)++;
                  
            }
        }
    }
         
    PMC_RETURN(result);
} /* odu_rfrm_layer_hierarchy_get */

/*******************************************************************************
*  odu_rfrm_parent_pid_parent_is_dci_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retreives all non-transparent sub-layers in the layer 
*   hierarchy on a given ODU channel.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - channel ID to request TCM layer information on
*                         Valid range: 0 - 95
*   tcm_layers          - array of 9 odu_rfrm_layer_struct_t elements
*   num_layers          - an array of TCM instance number and its associated
*                         pool id.
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
PUBLIC PMC_ERROR odu_rfrm_parent_pid_parent_is_dci_update(odu_rfrm_handle_t *odu_rfrm_handle,
                                                          UINT32 chnl_id,
                                                          odu_rfrm_layer_struct_t tcm_layers[ODU_SINK_MAX_PM_TCM_LAYERS],
                                                          UINT32 num_layers)
{
    /* variable declaration */
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 i;
    UINT32 bw_cnt;
    BOOL8 found_op_layer = FALSE;
    
    PMC_ENTRY();

    /* now, look after PARENT_POOL_ID and PARENT_POOL_ID_PM */
    if(num_layers > 0)
    {
        for(i=num_layers-1;i>0;i--)
        {   
            /* starting with the last layer, search backwards to find the
            first operational TCM.  Use this as the parent_pool_id */
            
            found_op_layer = FALSE;
            bw_cnt = 1;
            
            /* don't perform this for i = 0 */
            while(found_op_layer == FALSE && i > 0)
            {
            
                if(tcm_layers[i-bw_cnt].tcm_mode != ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR &&
                   tcm_layers[i-bw_cnt].tcm_mode != ODU_RFRM_PM_TCM_TT_ERASE )
                { 
                    found_op_layer = TRUE;
            
                    if(tcm_layers[i].pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
                    {
                          odu_rfrm_field_PARENT_POOL_ID_PM_set(NULL, odu_rfrm_handle, chnl_id, tcm_layers[i-bw_cnt].pool_id );
                    }
                    else
                    {
                          odu_rfrm_field_PARENT_POOL_ID_set(NULL, odu_rfrm_handle, tcm_layers[i].pool_id,  tcm_layers[i-bw_cnt].pool_id);
                    }     
                }
                else
                {
                    bw_cnt++;
                    if(bw_cnt > i)
                    {
                        /* if it gets here, then there is no operational layer between layer i
                        and DCI (parent pool id doesn't need to get set in this case)*/
                        found_op_layer = TRUE;
                    }           
                }            
            }
        }
    }
    /*
     * Ensure the flag is reset as it may have been set to TRUE in the 
     * previous iteration.
     */
    found_op_layer = FALSE;

    /* update parent_is_dci/parent_is_dci_pm */
    for(i=0;i<num_layers;i++)
    { 
        /* perform for all layers in case a layer is inserted*/
        if(tcm_layers[i].pm_tcm_num == ODU_RFRM_MESSAGE_TYPE_PM)
        {
            if(found_op_layer == FALSE)
            {
                odu_rfrm_field_PARENT_IS_DCI_PM_set(NULL, odu_rfrm_handle, chnl_id, 1);
            }
            else
            {
                odu_rfrm_field_PARENT_IS_DCI_PM_set(NULL, odu_rfrm_handle, chnl_id, 0);
            }
        }
        else
        {
            if(found_op_layer == FALSE)
            {
                odu_rfrm_field_PARENT_IS_DCI_set(NULL, odu_rfrm_handle, tcm_layers[i].pool_id, 1);
            }
            else
            {
                odu_rfrm_field_PARENT_IS_DCI_set(NULL, odu_rfrm_handle, tcm_layers[i].pool_id, 0);
            }
        }

        /* After the first operation TCM has had the PARENT_IS_DCI set, all downstream
        TCMs/PMs should not have this parameter set*/
        if(tcm_layers[i].tcm_mode != ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR &&
           tcm_layers[i].tcm_mode != ODU_RFRM_PM_TCM_TT_ERASE)
        {
            found_op_layer = TRUE;
        }
    }
         
    PMC_RETURN(result);
} /* odu_rfrm_parent_pid_parent_is_dci_update */


/*******************************************************************************
*  odu_rfrm_last_layer_last_is_dci_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retreives all non-transparent sub-layers in the layer 
*   hierarchy on a given ODU channel.
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - channel ID to request TCM layer information on
*                         Valid range: 0 - 95
*   tcm_layers          - array of 9 odu_rfrm_layer_struct_t elements
*   num_layers          - an array of TCM instance number and its associated
*                         pool id.
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
PUBLIC PMC_ERROR odu_rfrm_last_layer_last_is_dci_update(odu_rfrm_handle_t *odu_rfrm_handle,
                                                        UINT32 chnl_id,
                                                        odu_rfrm_layer_struct_t tcm_layers[ODU_SINK_MAX_PM_TCM_LAYERS],
                                                        UINT32 num_layers)
{
    /* variable declaration */
    UINT32 i;
    PMC_ERROR result = PMC_SUCCESS;
    odu_rfrm_pm_tcm_id_t last_operational_layer = ODU_RFRM_MESSAGE_DONT_CARE;
    UINT32 last_layer;
    BOOL8 op_tcms = FALSE;

    PMC_ENTRY();
  
    if(num_layers == 0)
    {
        odu_rfrm_field_LAST_IS_DCI_set(NULL, odu_rfrm_handle, chnl_id, 1);
    }
    else
    {

        for(i=0;i<num_layers;i++)
        {
            if(tcm_layers[i].tcm_mode != ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR &&
               tcm_layers[i].tcm_mode != ODU_RFRM_PM_TCM_TT_ERASE)
            {
                op_tcms = TRUE;
                last_operational_layer = tcm_layers[i].pm_tcm_num;
            }

        }
          
        if(op_tcms == TRUE)
        {
            /* Only deassert LAST_IS_DCI if one of the layer is not a NIM */
            odu_rfrm_field_LAST_IS_DCI_set(NULL, odu_rfrm_handle, chnl_id, 0);
            
        
            switch (last_operational_layer)
            {
                case ODU_RFRM_MESSAGE_TYPE_FLOATING_TCM:
                    last_layer = 0;
                break;
                case ODU_RFRM_MESSAGE_TYPE_TCM1:
                    last_layer = 1;
                break;
                case ODU_RFRM_MESSAGE_TYPE_TCM2:
                    last_layer = 2;
                break;
                case ODU_RFRM_MESSAGE_TYPE_TCM3:
                    last_layer = 3;
                break;
                case ODU_RFRM_MESSAGE_TYPE_TCM4:
                    last_layer = 4;
                break;
                case ODU_RFRM_MESSAGE_TYPE_TCM5:
                    last_layer = 5;
                break;
                case ODU_RFRM_MESSAGE_TYPE_TCM6:
                    last_layer = 6;
                break;
                case ODU_RFRM_MESSAGE_TYPE_PM:
                    last_layer = 7;
                break;
                default:
                    PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
                break;
            }

            odu_rfrm_field_LAST_LAYER_set(NULL, odu_rfrm_handle, chnl_id, last_layer);
        }
        else
        {
            odu_rfrm_field_LAST_IS_DCI_set(NULL, odu_rfrm_handle, chnl_id, 1);
        }/* end of if (op_tcms == TRUE) */

    }
             
    PMC_RETURN(result);
} /* odu_rfrm_last_layer_last_is_dci_update */

/*******************************************************************************
*  odu_rfrm_tcm_transparent_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function searches transparent mode 
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - channel ID
*   tcm_num             - TCM layer number for the channel \n
*                         1 to 6 -- TCM1 to TCM6 \n
*                         7      -- Floating TCM \n
*                         8      -- PM NIM
*
* OUTPUTS:
*   *tcm_transparent        - transparent mode 
*
* RETURNS:
*   PMC_ERR_FAIL - Internally generated error code
*               0 - PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_rfrm_tcm_transparent_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                               UINT32 chnl_id, 
                                               odu_rfrm_pm_tcm_id_t tcm_num,
                                               UINT32 *tcm_transparent)
{
    PMC_ENTRY();
    *tcm_transparent = 1;
    switch (tcm_num)
    {
        
    case ODU_RFRM_MESSAGE_TYPE_TCM1:
        *tcm_transparent = odu_rfrm_field_TCM1_TRANSPARENT_get(NULL, odu_rfrm_handle, chnl_id);
        break;
    case ODU_RFRM_MESSAGE_TYPE_TCM2:
        *tcm_transparent = odu_rfrm_field_TCM2_TRANSPARENT_get(NULL, odu_rfrm_handle, chnl_id);
        break;
    case ODU_RFRM_MESSAGE_TYPE_TCM3:
        *tcm_transparent = odu_rfrm_field_TCM3_TRANSPARENT_get(NULL, odu_rfrm_handle, chnl_id);
        break;
    case ODU_RFRM_MESSAGE_TYPE_TCM4:
        *tcm_transparent = odu_rfrm_field_TCM4_TRANSPARENT_get(NULL, odu_rfrm_handle, chnl_id);
        break;
    case ODU_RFRM_MESSAGE_TYPE_TCM5:
        *tcm_transparent = odu_rfrm_field_TCM5_TRANSPARENT_get(NULL, odu_rfrm_handle, chnl_id);
        break;
    case ODU_RFRM_MESSAGE_TYPE_TCM6:
        *tcm_transparent = odu_rfrm_field_TCM6_TRANSPARENT_get(NULL, odu_rfrm_handle, chnl_id);
        break;
    case ODU_RFRM_MESSAGE_TYPE_PM_NIM:
        *tcm_transparent = odu_rfrm_field_PM_NIM_TRANSPARENT_get(NULL, odu_rfrm_handle, chnl_id);
        break;
    default:
        PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }

    PMC_RETURN(PMC_SUCCESS);
} /* odu_rfrm_tcm_transparent_get */

/*******************************************************************************
*  odu_rfrm_remove_oh_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function updates all of the XX_REMOVE_OH bits every time:
*      - a channelized maintenance signal is applied
*      - a maintenance signal at a TCM is applied
*      - a TCM is provisioned or deprovisioned
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - channel ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR - if odu_rfrm_layer_hierarchy_get() fails
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR odu_rfrm_remove_oh_update(odu_rfrm_handle_t *odu_rfrm_handle,
                                            UINT32 chnl_id)
{
    PMC_ERROR result = PMC_SUCCESS;

    BOOL remove_oh;
    UINT32 lowest_tcm;
    BOOL parent_is_oci_lck;
    odu_rfrm_layer_struct_t tcm_layers[ODU_SINK_MAX_PM_TCM_LAYERS];
    UINT32 num_layers;
    UINT32 i;

    PMC_ENTRY();
    
    /*When a maintenance signal is applied, the REMOVE_OH Bit must be
    cleared under two conditions:
        - all TCMs upstream of a TCM where a maint sig is applied
        - LCK/OCI, first downstream TCM should have it cleared */
    
    if (result == PMC_SUCCESS)
    {
        /*get the new rfrm layer_hierachy */
        result = odu_rfrm_layer_hierarchy_get(odu_rfrm_handle,
                                              chnl_id,
                                              &tcm_layers,
                                              &num_layers);
    }


    lowest_tcm = 0;
    
    /* find the lower layer maintenance signals at TCMs */
    for(i=0;i<num_layers;i++)
    {
        if(tcm_layers[i].maint_sig != 0)
        {
            lowest_tcm = i;
        }
    }

    
    if(odu_rfrm_handle->var.maint_sig[chnl_id] == 3 ||
        odu_rfrm_handle->var.maint_sig[chnl_id] == 2)
    {
        parent_is_oci_lck = TRUE;
    }
    else
    {
        parent_is_oci_lck = FALSE;
    }

    for(i=0;i<num_layers;i++)
    {

        if(tcm_layers[i].tcm_mode == ODU_RFRM_PM_TCM_TT_OPERATIONAL)
        {   
            remove_oh = TRUE;
        }
        else
        {
            remove_oh = FALSE;
        }

        
        if(tcm_layers[i].tcm_mode == ODU_RFRM_PM_TCM_TT_ERASE)
        { 
            remove_oh = TRUE;
        }
        else
        {
            /* set remove_oh for all upstream TCMs, regardless of "remove_oh"
            set above */
            if(i<lowest_tcm)
            {
                remove_oh=FALSE;
            }

            if(parent_is_oci_lck)
            {
                remove_oh=FALSE;
            }
        }

        /* REMOVE_OH must also be set for the first operational TCM after
        TCM-LCK/TCM-OCI*/
        if((tcm_layers[i].maint_sig == 3 ||
           tcm_layers[i].maint_sig == 2) &&
           (tcm_layers[i].tcm_mode == ODU_RFRM_PM_TCM_TT_OPERATIONAL ||
           tcm_layers[i].tcm_mode == ODU_RFRM_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED))
        {
            parent_is_oci_lck = TRUE;
        }
        else if(tcm_layers[i].tcm_mode == ODU_RFRM_PM_TCM_TT_OPERATIONAL ||
                tcm_layers[i].tcm_mode == ODU_RFRM_PM_TCM_TT_OPERATIONAL_TCMOH_NOT_ZEROED)
        {
            parent_is_oci_lck = FALSE;
        }


            
        switch (tcm_layers[i].pm_tcm_num)
        {
              case ODU_RFRM_MESSAGE_TYPE_TCM1: /* TCM1 */
                    odu_rfrm_field_TCM1_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, remove_oh);
                    break;
              case ODU_RFRM_MESSAGE_TYPE_TCM2: /* TCM2 */
                    odu_rfrm_field_TCM2_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, remove_oh);
                    break;
              case ODU_RFRM_MESSAGE_TYPE_TCM3: /* TCM3 */
                    odu_rfrm_field_TCM3_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, remove_oh);
                    break;
              case ODU_RFRM_MESSAGE_TYPE_TCM4: /* TCM4 */
                    odu_rfrm_field_TCM4_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, remove_oh);
                    break;
              case ODU_RFRM_MESSAGE_TYPE_TCM5: /* TCM5 */
                    odu_rfrm_field_TCM5_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, remove_oh);
                    break;
              case ODU_RFRM_MESSAGE_TYPE_TCM6: /* TCM6 */
                    odu_rfrm_field_TCM6_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, remove_oh);
                    break;
              case ODU_RFRM_MESSAGE_TYPE_FLOATING_TCM:
                    /*REMOVE_OH should not be set for floating TCMs*/
                    break;
              case ODU_RFRM_MESSAGE_TYPE_PM_NIM: /* PM_NIM */
                    /*no actions for PM-NIM layer */
                    break;
              case ODU_RFRM_MESSAGE_TYPE_PM:    
                    /*no actions for PM layer */
                    break;
              default:
                    PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
          } 

    }


    PMC_RETURN(result);

} /* odu_rfrm_remove_oh_update */


/*******************************************************************************
* odu_rfrm_tcmi_resource_transition
* ______________________________________________________________________________
*
* DESCRIPTION: 
*    This API is used to transition between a TCMi-NIM provisioned resource to 
*    a floating TCM resource monitoring the same TCM level with the same 
*    pool id (visa-versa).  No changes in the TCM layering heirarchy are 
*    supported with this API.
*
*  For example:
*     DIGI_OTN_TCM_NUM_TCM1 as NIM -> DIGI_OTN_TCM_FLOATING_TCM (monitoring TCM1)
*       - or -
*     DIGI_OTN_TCM_FLOATING_TCM (monitoring TCM1) -> DIGI_OTN_TCM_NUM_TCM1 as NIM
*
* INPUTS:
*   *odu_rfrm_handle    - pointer to ODU_RFRM handle instance to be operated on
*   chnl_id             - Channel ID to be processed
*                         Valid range: 0 - 95              
*   tcm_pool_id         - Pool id of the TCM resource to transition (if the resource
*                         is currently provisioned as a TCMi-NIM resource, it will transition to
*                         a floating TCM resource.  If currently provisioned as a floating
*                         TCM resource, it will transition to a TCMi-NIM resource)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*   PMC_ERROR - If no TCM is provisioned for that resource or if resource is not
*                                 provisioned as a NIM or floating NIM
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR odu_rfrm_tcmi_resource_transition(odu_rfrm_handle_t *odu_rfrm_handle,
                                                   UINT32 chnl_id,
                                                   UINT32 tcm_pool_id)
{
    /* variable declaration */
    PMC_ERROR ret_code = PMC_SUCCESS;
    UINT32 float_tcmi_src = 0;
    UINT32 num_layers;
    odu_rfrm_layer_struct_t odu_rfrm_layers[ODU_SINK_MAX_PM_TCM_LAYERS];
    UINT32 ch_control0;
    UINT32 ch_control0_new;
    odu_rfrm_buffer_t odu_rfrm_buffer; 
    UINT32 current_layer = 0;
    UINT32 i;

    PMC_ENTRY();

    /* argument checking */
    if (chnl_id > 95 || 
        tcm_pool_id > (odu_rfrm_handle->cfg.tcm_pool_size-1) )
    {
        /* some error */
        PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }


    /* can only resource provisioned as a NIM */
    if(odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].assign_state == 0 ||
       odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_mode != ODU_RFRM_PM_TCM_NON_INTRUSIVE_MONITOR)
    {
        PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
    }

    if( ret_code == PMC_SUCCESS)
    {
        ret_code = odu_rfrm_layer_hierarchy_get(odu_rfrm_handle,
                                                chnl_id,
                                                &odu_rfrm_layers,
                                                &num_layers);
    } 
    
    if(odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num == ODU_RFRM_MESSAGE_TYPE_FLOATING_TCM)
    {
        /*transition floating tcmi resource to tcmi resource */

        float_tcmi_src = odu_rfrm_field_FLOATING_TCM_ID_get(NULL, odu_rfrm_handle, chnl_id);

        if(float_tcmi_src != 0)
        {
            for(i=0;i<num_layers;i++)
            {
                if((UINT32)odu_rfrm_layers[i].pm_tcm_num == float_tcmi_src)
                {
                    /* can't transition to TCMi resource because it's already in use*/
                    PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
                }
            }
        }
    }
    else
    {

        float_tcmi_src = odu_rfrm_field_FLOATING_TCM_ID_get(NULL, odu_rfrm_handle, chnl_id);

        if(float_tcmi_src != 0)
        {
            /* can't transition to floating TCMi resource because it's already in use*/
            PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
        }
    }
    
    for(i=0;i<num_layers;i++)
    {
        if(odu_rfrm_layers[i].pool_id == tcm_pool_id)
        {
            current_layer = i;
        }
    }

    if(odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num == ODU_RFRM_MESSAGE_TYPE_FLOATING_TCM)
    {
        /*transition floating tcmi resource to tcmi resource */

        float_tcmi_src = odu_rfrm_field_FLOATING_TCM_ID_get(NULL, odu_rfrm_handle, chnl_id);

        switch (float_tcmi_src)
        {
            case 1: /* TCM1 */
                odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);  
                /* enable writing to POOL ID field */
                odu_rfrm_field_TCM1_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, 1);
                /* assign unique POOL ID */
                odu_rfrm_field_TCM1_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id);
                odu_rfrm_buffer_flush(&odu_rfrm_buffer);
                odu_rfrm_field_TCM1_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, 0);
                
                ch_control0 = odu_rfrm_reg_CHANNEL_CTRL0_array_read(NULL, odu_rfrm_handle, chnl_id);
                ch_control0_new = (ch_control0 & ~(0x1 << 12) & ~(7 << 8));

                /*unassign floating TCM*/
                odu_rfrm_field_FLOATING_TCM_ID_set(NULL, odu_rfrm_handle, chnl_id, 0);
                
                /*make TCM1 non-transparent without a read-modify wirte*/
                odu_rfrm_reg_CHANNEL_CTRL0_array_write(NULL, odu_rfrm_handle, chnl_id, ch_control0_new);

                /*update context*/
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num = ODU_RFRM_MESSAGE_TYPE_TCM1;
                break;
            case 2: /* TCM2 */
                odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);  
                /* enable writing to POOL ID field */
                odu_rfrm_field_TCM2_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, 1);
                /* assign unique POOL ID */
                odu_rfrm_field_TCM2_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id);
                odu_rfrm_buffer_flush(&odu_rfrm_buffer);
                odu_rfrm_field_TCM2_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, 0);
             
                ch_control0 = odu_rfrm_reg_CHANNEL_CTRL0_array_read(NULL, odu_rfrm_handle, chnl_id);
                ch_control0_new = (ch_control0 & ~(0x1 << 13) & ~(7 << 8));

                /*unassign floating TCM*/
                odu_rfrm_field_FLOATING_TCM_ID_set(NULL, odu_rfrm_handle, chnl_id, 0);
                
                /*make TCM2 non-transparent without a read-modify wirte*/
                odu_rfrm_reg_CHANNEL_CTRL0_array_write(NULL, odu_rfrm_handle, chnl_id, ch_control0_new);

                /*update context*/
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num = ODU_RFRM_MESSAGE_TYPE_TCM2;

                break;
            case 3: /* TCM3 */
                odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);  
                /* enable writing to POOL ID field */
                odu_rfrm_field_TCM3_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, 1);
                /* assign unique POOL ID */
                odu_rfrm_field_TCM3_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id);
                odu_rfrm_buffer_flush(&odu_rfrm_buffer);
                odu_rfrm_field_TCM3_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, 0);
                
                ch_control0 = odu_rfrm_reg_CHANNEL_CTRL0_array_read(NULL, odu_rfrm_handle, chnl_id);
                ch_control0_new = (ch_control0 & ~(0x1 << 14) & ~(7 << 8));

                /*unassign floating TCM*/
                odu_rfrm_field_FLOATING_TCM_ID_set(NULL, odu_rfrm_handle, chnl_id, 0);
                
                /*make TCM3 non-transparent without a read-modify wirte*/
                odu_rfrm_reg_CHANNEL_CTRL0_array_write(NULL, odu_rfrm_handle, chnl_id, ch_control0_new);

                /*update context*/
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num = ODU_RFRM_MESSAGE_TYPE_TCM3;

                break;
            case 4: /* TCM4 */
                odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);  
                /* enable writing to POOL ID field */
                odu_rfrm_field_TCM4_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, 1);
                /* assign unique POOL ID */
                odu_rfrm_field_TCM4_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id);
                odu_rfrm_buffer_flush(&odu_rfrm_buffer);
                odu_rfrm_field_TCM4_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, 0);
                
                ch_control0 = odu_rfrm_reg_CHANNEL_CTRL0_array_read(NULL, odu_rfrm_handle, chnl_id);
                ch_control0_new = (ch_control0 & ~(0x1 << 15) & ~(7 << 8));

                /*unassign floating TCM*/
                odu_rfrm_field_FLOATING_TCM_ID_set(NULL, odu_rfrm_handle, chnl_id, 0);
                
                /*make TCM4 non-transparent without a read-modify wirte*/
                odu_rfrm_reg_CHANNEL_CTRL0_array_write(NULL, odu_rfrm_handle, chnl_id, ch_control0_new);

                /*update context*/
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num = ODU_RFRM_MESSAGE_TYPE_TCM4;

                break;
            case 5: /* TCM5 */
                odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);  
                /* enable writing to POOL ID field */
                odu_rfrm_field_TCM5_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, 1);
                /* assign unique POOL ID */
                odu_rfrm_field_TCM5_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id);
                odu_rfrm_buffer_flush(&odu_rfrm_buffer);
                odu_rfrm_field_TCM5_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, 0);
                
                ch_control0 = odu_rfrm_reg_CHANNEL_CTRL0_array_read(NULL, odu_rfrm_handle, chnl_id);
                ch_control0_new = (ch_control0 & ~(0x1 << 16) & ~(7 << 8));

                /*unassign floating TCM*/
                odu_rfrm_field_FLOATING_TCM_ID_set(NULL, odu_rfrm_handle, chnl_id, 0);
                
                /*make TCM5 non-transparent without a read-modify wirte*/
                odu_rfrm_reg_CHANNEL_CTRL0_array_write(NULL, odu_rfrm_handle, chnl_id, ch_control0_new);

                /*update context*/
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num = ODU_RFRM_MESSAGE_TYPE_TCM5;

                break;
            case 6: /* TCM6 */ 
                odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);  
                /* enable writing to POOL ID field */
                odu_rfrm_field_TCM6_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, 1);
                /* assign unique POOL ID */
                odu_rfrm_field_TCM6_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id);
                odu_rfrm_buffer_flush(&odu_rfrm_buffer);
                odu_rfrm_field_TCM6_REMOVE_OH_set(NULL, odu_rfrm_handle, chnl_id, 0);
                
                ch_control0 = odu_rfrm_reg_CHANNEL_CTRL0_array_read(NULL, odu_rfrm_handle, chnl_id);
                ch_control0_new = (ch_control0 & ~(0x1 << 17) & ~(7 << 8));

                /*unassign floating TCM*/
                odu_rfrm_field_FLOATING_TCM_ID_set(NULL, odu_rfrm_handle, chnl_id, 0);
                
                /*make TCM6 non-transparent without a read-modify wirte*/
                odu_rfrm_reg_CHANNEL_CTRL0_array_write(NULL, odu_rfrm_handle, chnl_id, ch_control0_new);

                /*update context*/
                odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num = ODU_RFRM_MESSAGE_TYPE_TCM6;

                break;
            default:
                PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
          } 


    }
    else
    {
        /*transition tcmi resource to floating tcmi resource */
        ch_control0 = odu_rfrm_reg_CHANNEL_CTRL0_array_read(NULL, odu_rfrm_handle, chnl_id);

        odu_rfrm_buffer_init(&odu_rfrm_buffer, odu_rfrm_handle);
        /* enable writing to POOL ID field */
        odu_rfrm_field_FLOATING_TCM_WR_EN_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, 1);
        /* assign unique POOL ID */
        odu_rfrm_field_FLOATING_TCM_POOL_ID_set(&odu_rfrm_buffer, odu_rfrm_handle, chnl_id, tcm_pool_id); 
        odu_rfrm_buffer_flush(&odu_rfrm_buffer); 

        switch (odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num)
        {
              case ODU_RFRM_MESSAGE_TYPE_TCM1: /* TCM1 */
                    ch_control0_new = ch_control0 | (1 << 8) | (0x1 << 12);
                    /*set TCM1 to transparent*/
                    odu_rfrm_field_TCM1_TRANSPARENT_set(NULL, odu_rfrm_handle, chnl_id, 1);
                    break;
              case ODU_RFRM_MESSAGE_TYPE_TCM2: /* TCM2 */
                    ch_control0_new = ch_control0 | (2 << 8) | (0x1 << 13);     
                    /*set TCM2 to transparent*/
                    odu_rfrm_field_TCM2_TRANSPARENT_set(NULL, odu_rfrm_handle, chnl_id, 1);
                    break;
              case ODU_RFRM_MESSAGE_TYPE_TCM3: /* TCM3 */
                    ch_control0_new = ch_control0 | (3 << 8) | (0x1 << 14); 
                    /*set TCM3 to transparent*/
                    odu_rfrm_field_TCM3_TRANSPARENT_set(NULL, odu_rfrm_handle, chnl_id, 1);                    
                    break;
              case ODU_RFRM_MESSAGE_TYPE_TCM4: /* TCM4 */
                    ch_control0_new = ch_control0 | (4 << 8) | (0x1 << 15);
                    /*set TCM4 to transparent*/
                    odu_rfrm_field_TCM4_TRANSPARENT_set(NULL, odu_rfrm_handle, chnl_id, 1);                    
                    break;
              case ODU_RFRM_MESSAGE_TYPE_TCM5: /* TCM5 */
                    ch_control0_new = ch_control0 | (5 << 8) | (0x1 << 16);   
                    /*set TCM5 to transparent*/
                    odu_rfrm_field_TCM5_TRANSPARENT_set(NULL, odu_rfrm_handle, chnl_id, 1);                    
                    break;
              case ODU_RFRM_MESSAGE_TYPE_TCM6: /* TCM6 */
                    ch_control0_new = ch_control0 | (6 << 8) |  (0x1 << 17);       
                    /*set TCM6 to transparent*/
                    odu_rfrm_field_TCM6_TRANSPARENT_set(NULL, odu_rfrm_handle, chnl_id, 1);                    
                    break;
              default:
                    PMC_RETURN(ODU_RFRM_LOG_ERR_INVALID_PARAMETERS);
        }

        /*Set FLOATING_TCM_ID without a read-modify wirte*/
        odu_rfrm_reg_CHANNEL_CTRL0_array_write(NULL, odu_rfrm_handle, chnl_id, ch_control0_new);

        /*update context*/
        odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num = ODU_RFRM_MESSAGE_TYPE_FLOATING_TCM;



    }

    
    if(current_layer == 0)
    {
         odu_rfrm_handle->var.dci_pm_tcm_child[chnl_id] = odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num;

    }
    else
    {
        odu_rfrm_handle->var.tcm_pool_ptr[odu_rfrm_layers[current_layer-1].pool_id].child_pm_tcm_num = odu_rfrm_handle->var.tcm_pool_ptr[tcm_pool_id].tcm_num;
    }

    
    if( ret_code == PMC_SUCCESS)
    {
        ret_code = odu_rfrm_layer_hierarchy_get(odu_rfrm_handle,
                                                chnl_id,
                                                &odu_rfrm_layers,
                                                &num_layers);
    }    
   
        
    /* call function to set last_layer in the event that LAST_LAYER has changed */
    if (PMC_SUCCESS == ret_code)
    {
        ret_code = odu_rfrm_last_layer_last_is_dci_update(odu_rfrm_handle,
                                                          chnl_id,
                                                          odu_rfrm_layers,
                                                          num_layers);
    }        


    PMC_LOG_TRACE("Configuring ODU_RFRM for channel ID = %u...\n", chnl_id);

    PMC_RETURN(ret_code);

} /* odu_rfrm_tcmi_resource_transition */

/*******************************************************************************
* odu_rfrm_pm_status_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*  Copies over the int_chnl_en, int_pid_en, and int_comb_en data arrays into
*  the passed in table.
*
* INPUTS:
*  odu_rfrm_handle    - ODU_RFRM handle instance to be operated
*
* OUTPUTS:
*  pm_table_ptr       - Reference to the table to hold the status values.
*
* RETURNS:
*  None
*
* NOTES:
*
*******************************************************************************/
PUBLIC void odu_rfrm_pm_status_get(odu_rfrm_handle_t *odu_rfrm_handle,
                                   util_global_otn_pm_status_t *pm_table_ptr)
{
    /* variable declaration */
    int i = 0;

    PMC_ENTRY();

    for (i = 0; i < UTIL_GLOBAL_NUM_ODU_RFRM_CHNL; i++)
    {
        pm_table_ptr->int_chnl_en[i] = pmc_bitarray_is_bit_set(odu_rfrm_handle->var.int_chnl_en, i);
    }

    for (i = 0; i < UTIL_GLOBAL_TCM_POOL_SIZE_96; i++)
    {
        pm_table_ptr->int_pid_en[i] = pmc_bitarray_is_bit_set(odu_rfrm_handle->var.int_pid_en, i);
    }

    for (i = 0; i < UTIL_GLOBAL_NUM_ODU_RFRM_DEFECTS; i++)
    {
        pm_table_ptr->int_comb_en[i] = pmc_bitarray_is_bit_set(odu_rfrm_handle->var.int_comb_en, i);
    }

    PMC_RETURN();
}

/*
** End of file
*/
