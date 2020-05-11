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
/*
** Include Files
*/
#include "cbr_fsgm_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
#define CBRC_FSGM_PATT_INS_TIP_POLL_MAX_CNT 100
#define CBRC_FSGM_PATT_INS_TIP_POLL_DELAY   100
#define CBRC_FSGM_USER_PATTERN_NUM_WORD     5  
/*
** Local Macro Definitions
*/

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/* 
 * The following creates the LOG/ERR strings from above by pulling out the text
 * portion of the LOG_ERR_TABLE above 
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char CBR_FSGM_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    CBR_FSGM_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else  /* no strings */

const char CBR_FSGM_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Public Functions
*/

/*******************************************************************************
* cbr_fsgm_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an CBR_FSGM block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the CBR_FSGM subsystem relative to 
*                           the device memory space
*  *sys_handle            - pointer to user system handle
*  *tsb_name              - Name of subsystem
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   cbr_fsgm_handle         - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC cbr_fsgm_handle_t *cbr_fsgm_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    cbr_fsgm_handle_t *cbr_fsgm_handle;

    PMC_ENTRY();

    cbr_fsgm_handle = (cbr_fsgm_handle_t*)PMC_CTXT_CALLOC(sizeof(cbr_fsgm_handle_t), parent);
    
    pmc_handle_init(parent, cbr_fsgm_handle, sys_handle, PMC_MID_DIGI_CBR_FSGM, tsb_name, base_address);

    PMC_RETURN(cbr_fsgm_handle);
} /* cbr_fsgm_ctxt_create */

 
/*******************************************************************************
* cbr_fsgm_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an CBR_FSGM block instance.
*
*
* INPUTS:
*   *cbr_fsgm_handle         - pointer to CBR_FSGM handle instance
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
PUBLIC void cbr_fsgm_ctxt_destroy(cbr_fsgm_handle_t *cbr_fsgm_handle)
{
    PMC_ENTRY();
    pmc_handle_mid_verify(cbr_fsgm_handle, PMC_MID_DIGI_CBR_FSGM);
    cbr_fsgm_handle->base.tsb_mid = PMC_MID_INVALID;
    PMC_CTXT_FREE(&cbr_fsgm_handle, cbr_fsgm_handle);
    PMC_RETURN();
} /* cbr_fsgm_ctxt_destroy */



/*******************************************************************************
* FUNCTION: cbr_fsgm_var_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialized dynamic context. 
*   
*
* INPUTS:
*   cbr_fsgm_handle - Pointer to subsystem handle
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_var_init(cbr_fsgm_handle_t    *cbr_fsgm_handle)
{
    PMC_ERROR     result = PMC_SUCCESS;    
    UINT8         i;

    PMC_ENTRY();


    for (i = 0; i < CBR_FSGM_MAX_INDEX; i++)
    {
        cbr_fsgm_handle->var.mode[i] = CBR_FSGM_MODE_NONE; 
    } /* end for */

    PMC_RETURN(result);
} /* cbr_fsgm_var_init */




/*******************************************************************************
* cbr_fsgm_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an CBR_FSGM block handle instance.
*
*
* INPUTS:
*   *cbr_fsgm_handle         - pointer to CBR_FSGM handle instance
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
PUBLIC void cbr_fsgm_handle_init(cbr_fsgm_handle_t *cbr_fsgm_handle)
{
    PMC_ENTRY();

    cbr_fsgm_var_init(cbr_fsgm_handle);

    PMC_RETURN();
} /* cbr_fsgm_handle_init */



/*******************************************************************************
* FUNCTION: cbr_fsgm_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bring CBR_FSGM block into configurable state 
*
* INPUTS:
*   cbr_fsgm_handle   - Pointer to subsystem handle
*   energy_state      - Energy state to put CBRC into
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_init(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                               pmc_energy_state_t    energy_state)
{
    PMC_ERROR     result = PMC_SUCCESS;    
    UINT8         i;

    PMC_ENTRY();
    
    /* Check Arguments */
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);


    switch (energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            /* Put CBR_FSGM into soft reset */
            cbr_fsgm_reset(cbr_fsgm_handle, CBR_FSGM_RESET);
          
            /* Put PATT_INS into soft reset */
            for (i = 0; i < CBR_FSGM_MAX_INDEX; i++)
            {
                cbr_fsgm_patt_ins_reset(cbr_fsgm_handle, i, CBR_FSGM_RESET); 
            }
            
            /* Disable fault insertion */
            for (i = 0; i < CBR_FSGM_MAX_INDEX; i++)
            {
                cbr_fsgm_field_CH_n_FAULT_INS_ENA_set(NULL, cbr_fsgm_handle, i, CBR_FSGM_DISABLE);
                cbr_fsgm_field_CH_n_LOS_INS_ENA_set(NULL, cbr_fsgm_handle, i, CBR_FSGM_DISABLE);
            }
            break;

        case PMC_ENERGY_STATE_RESET:
            cbr_fsgm_var_init(cbr_fsgm_handle);
            break;
        
        default:
            PMC_ASSERT(FALSE, CBR_FSGM_ERR_INVALID_ARG, energy_state, 0);
            break;
    } /* end switch */

    PMC_RETURN(result);
} /* cbr_fsgm_init */




/*******************************************************************************
* cbr_fsgm_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   CBRC instance.
*
*
* INPUTS:
*   *cbr_fsgm_handle      - pointer to cbr_fsgm handle instance to be operated on
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   TRUE              - Resources are all in start state (not provisioned).
*   FALSE             - At least one resource is provisioned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL cbr_fsgm_start_state_test(cbr_fsgm_handle_t *cbr_fsgm_handle)
{
    BOOL                start_state = TRUE;
    UINT8               i;

    
    PMC_ENTRY();
    PMC_ASSERT(cbr_fsgm_handle!=NULL, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
   

    /* Traverse through all indices to find one that has been allocated */ 
    i = 0;
    while ((i < CBR_FSGM_MAX_INDEX) && (TRUE == start_state))
    {
        /* Resource has been allocated: SDH_PMG is not in start state */
        if (cbr_fsgm_handle->var.mode[i] != CBR_FSGM_MODE_NONE)
        {
            start_state = FALSE;
        }
        
        i++;
    } /* end while */


    
    PMC_RETURN(start_state);
    
} /* cbr_fsgm_start_state_test */





/*******************************************************************************
* FUNCTION: cbr_fsgm_provision()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provision an index within the CBR_FSGM block to a certain mode.
*   Puts the index into reset for further configuration.
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*   mode            - Mode that this index is working in
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_provision(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                    UINT32                index,
                                    cbr_fsgm_mode_t       mode)
{
    PMC_ERROR     result = PMC_SUCCESS;    
    UINT8         start_index, end_index;
    UINT8         i;
    const cbr_fsgm_user_fault_pattern_t user_pattern[LAST_CBR_FSGM_MODE] =
        {
            CBR_FSGM_USER_FAULT_PATTERN_NONE,           /* CBR_FSGM_MODE_NONE */
            CBR_FSGM_USER_FAULT_PATTERN_NONE,           /* CBR_FSGM_MODE_40G_STS768 */
            CBR_FSGM_USER_FAULT_PATTERN_NONE,           /* CBR_FSGM_MODE_40G_STM256 */
            CBR_FSGM_USER_FAULT_PATTERN_NONE,           /* CBR_FSGM_MODE_10G_STS192 */
            CBR_FSGM_USER_FAULT_PATTERN_NONE,           /* CBR_FSGM_MODE_10G_STM64 */
            CBR_FSGM_USER_FAULT_PATTERN_64B_66B,        /* CBR_FSGM_MODE_10G_FC1200_ODU1F */
            CBR_FSGM_USER_FAULT_PATTERN_64B_66B,        /* CBR_FSGM_MODE_10G_FC1200_ODU2F */
            CBR_FSGM_USER_FAULT_PATTERN_NONE,           /* CBR_FSGM_MODE_10G_FC1200_GFP_ODU2E */
            CBR_FSGM_USER_FAULT_PATTERN_8B_10B_FC_NOS,  /* CBR_FSGM_MODE_10G_FC800 */
            CBR_FSGM_USER_FAULT_PATTERN_8B_10B,         /* CBR_FSGM_MODE_10G_CPRI_5G */
            CBR_FSGM_USER_FAULT_PATTERN_8B_10B,         /* CBR_FSGM_MODE_10G_CPRI_6G */
            CBR_FSGM_USER_FAULT_PATTERN_8B_10B,         /* CBR_FSGM_MODE_10G_CPRI_10G */
            CBR_FSGM_USER_FAULT_PATTERN_8B_10B,         /* CBR_FSGM_MODE_10G_GDPS_5G */
            CBR_FSGM_USER_FAULT_PATTERN_8B_10B,         /* CBR_FSGM_MODE_10G_GDPS_10G */
            CBR_FSGM_USER_FAULT_PATTERN_NONE,           /* CBR_FSGM_MODE_10G_STS192_PRBS */
            CBR_FSGM_USER_FAULT_PATTERN_NONE,           /* CBR_FSGM_MODE_10G_STM64_PRBS */
            CBR_FSGM_USER_FAULT_PATTERN_64B_66B_LF_10GE,/* CBR_FSGM_MODE_10G_10GE_LF */
            CBR_FSGM_USER_FAULT_PATTERN_64B_66B_RF_10GE,/* CBR_FSGM_MODE_10G_10GE_RF */
            CBR_FSGM_USER_FAULT_PATTERN_8B_10B_FC_IDLE, /* CBR_FSGM_MODE_10G_8B_10B_IDLE */
            CBR_FSGM_USER_FAULT_PATTERN_64B_66B_FC_IDLE,/* CBR_FSGM_MODE_10G_64B_66B_IDLE */
            CBR_FSGM_USER_FAULT_PATTERN_8B_10B_ARB,        /* CBR_FSGM_MODE_10G_8B_10B_ARB */
        };

    PMC_ENTRY();

    PMC_ASSERT(cbr_fsgm_handle!=NULL, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_ARG, index, CBR_FSGM_MAX_INDEX);
    PMC_ASSERT(mode  < LAST_CBR_FSGM_MODE, CBR_FSGM_ERR_INVALID_ARG, mode, LAST_CBR_FSGM_MODE);

    
    if ((CBR_FSGM_MODE_NONE == mode) ||
        (CBR_FSGM_MODE_10G_FC1200_GFP_ODU2E == mode)) /* unsupported mode */
    {
        PMC_RETURN(PMC_SUCCESS);
    }
   



    /* Bring CBR_FSGM out of reset if needed */
    if (TRUE == cbr_fsgm_start_state_test(cbr_fsgm_handle)) 
    {
        cbr_fsgm_reset(cbr_fsgm_handle, CBR_FSGM_ACTIVE);
    }


    /* Store mode internally */
    cbr_fsgm_handle->var.mode[index] = mode;
   

    if (cbr_fsgm_mode_is_40g(cbr_fsgm_handle, index)) {
        start_index = 0;
        end_index   = CBR_FSGM_MAX_INDEX;

        cbr_fsgm_field_MODE_40G_set(NULL, cbr_fsgm_handle, CBR_FSGM_ENABLE);
    } else {
        start_index = index;
        end_index   = start_index + 1;
        
        cbr_fsgm_field_MODE_40G_set(NULL, cbr_fsgm_handle, CBR_FSGM_DISABLE);
    }

    

    /* Set bus_width */
    for (i = start_index; i < end_index; i++)
    {
        cbr_fsgm_bus_width_set(cbr_fsgm_handle, i);
    }


    /* Set fault size */
    for (i = start_index; i < end_index; i++)
    {
        cbr_fsgm_fault_size_set(cbr_fsgm_handle, i);
    }


    /* Set fault mode */
    for (i = start_index; i < end_index; i++)
    {
        cbr_fsgm_fault_mode_set(cbr_fsgm_handle, i);
    }


    /* Set user fault pattern to default */
    for (i = start_index; i < end_index; i++)
    {
        cbr_fsgm_user_fault_pattern_set(cbr_fsgm_handle, i, user_pattern[mode]);
    }


    /* Disable fault insertion */
    for (i = start_index; i < end_index; i++)
    {
        cbr_fsgm_field_CH_n_FAULT_INS_ENA_set(NULL, cbr_fsgm_handle, i, CBR_FSGM_DISABLE);
        cbr_fsgm_field_CH_n_FAULT_PROP_ENA_set(NULL, cbr_fsgm_handle, i, CBR_FSGM_DISABLE);
        cbr_fsgm_field_CH_n_LOS_INS_ENA_set(NULL, cbr_fsgm_handle, i, CBR_FSGM_DISABLE);
        cbr_fsgm_field_CH_n_LOS_PROP_ENA_set(NULL, cbr_fsgm_handle, i, CBR_FSGM_DISABLE);
    }



    /* Bring CBR_FSGM PATT_INS into reset */
    for (i = start_index; i < end_index; i++)
    {
        cbr_fsgm_patt_ins_reset(cbr_fsgm_handle, i, CBR_FSGM_RESET);
    } 




    /* Set gap mode */
    cbr_fsgm_gap_val_set(cbr_fsgm_handle, index, mode);


    PMC_RETURN(result);
} /* cbr_fsgm_provision */


/*******************************************************************************
* FUNCTION: cbr_fsgm_unprovision()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Unprovisions index within CBR_FSGM.
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_unprovision(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                      UINT32                index)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    
    PMC_ASSERT(cbr_fsgm_handle!=NULL, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_ARG, index, CBR_FSGM_MAX_INDEX);
    
    if (CBR_FSGM_MODE_NONE == cbr_fsgm_handle->var.mode[index])
    {
        PMC_RETURN(PMC_SUCCESS);
    }
   

    /* Store mode internally */
    cbr_fsgm_handle->var.mode[index] = CBR_FSGM_MODE_NONE;


    /* Put CBR_FSGM out of reset if needed */
    if (TRUE == cbr_fsgm_start_state_test(cbr_fsgm_handle)) 
    {
        cbr_fsgm_reset(cbr_fsgm_handle, CBR_FSGM_RESET);
    }

    PMC_RETURN(result);
} /* cbr_fsgm_unprovision */


/*******************************************************************************
* FUNCTION: cbr_fsgm_activate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activates index within CBR_FSGM. Brings index out of reset. 
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_activate(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                   UINT32                index)
{
    PMC_ERROR           result = PMC_SUCCESS;    
    UINT8               start_index, end_index;
    UINT8               i;
    BOOL                fault_ins_ena;

    PMC_ENTRY();


    PMC_ASSERT(cbr_fsgm_handle!=NULL, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_ARG, index, CBR_FSGM_MAX_INDEX);
    
    if (CBR_FSGM_MODE_NONE == cbr_fsgm_handle->var.mode[index])
    {
        PMC_RETURN(PMC_SUCCESS);
    }


    if (cbr_fsgm_mode_is_40g(cbr_fsgm_handle, index)) {
        start_index = 0;
        end_index   = CBR_FSGM_MAX_INDEX;
    } else {
        start_index = index;
        end_index   = start_index + 1;
    }
   
    /* Put CBR_FSGM PATT_INS out of reset and enable */
    for (i = start_index; i < end_index; i++)
    {
        cbr_fsgm_patt_ins_reset(cbr_fsgm_handle, i, CBR_FSGM_ACTIVE);
        cbr_fsgm_patt_ins_enable(cbr_fsgm_handle, i, CBR_FSGM_ENABLE);
    } 

    /* For 40G links, CH0 enables the entire logical group */
    cbr_fsgm_ch_en(cbr_fsgm_handle, index, CBR_FSGM_ENABLE);


    if (cbr_fsgm_mode_is_oc192(cbr_fsgm_handle, index))
    {
        fault_ins_ena = CBR_FSGM_ENABLE;
    }
    else
    {
        fault_ins_ena = CBR_FSGM_DISABLE;
    }

    /* Configure fault insertion behaviour */
    for (i = start_index; i < end_index; i++)
    {
        cbr_fsgm_field_CH_n_FAULT_INS_ENA_set (NULL, cbr_fsgm_handle, i, fault_ins_ena);
        cbr_fsgm_field_CH_n_FAULT_PROP_ENA_set(NULL, cbr_fsgm_handle, i, CBR_FSGM_ENABLE);
        cbr_fsgm_field_CH_n_LOS_INS_ENA_set   (NULL, cbr_fsgm_handle, i, CBR_FSGM_ENABLE);
        cbr_fsgm_field_CH_n_LOS_PROP_ENA_set  (NULL, cbr_fsgm_handle, i, CBR_FSGM_ENABLE);
    }
    
    

    PMC_RETURN(result);
} /* cbr_fsgm_activate */



/*******************************************************************************
* FUNCTION: cbr_fsgm_deactivate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deactivates index within CBR_FSGM block. Brings the index back into 
*   reset for further config.
*   
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_deactivate(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                     UINT32                index)
{
    PMC_ERROR     result = PMC_SUCCESS;    
    UINT8         start_index, end_index;
    UINT8         i;

    PMC_ENTRY();

    PMC_ASSERT(cbr_fsgm_handle!=NULL, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_ARG, index, CBR_FSGM_MAX_INDEX);
    
    if (CBR_FSGM_MODE_NONE == cbr_fsgm_handle->var.mode[index])
    {
        PMC_RETURN(PMC_SUCCESS);
    }

    if (cbr_fsgm_mode_is_40g(cbr_fsgm_handle, index)) {
        start_index = 0;
        end_index   = CBR_FSGM_MAX_INDEX;
    } else {
        start_index = index;
        end_index   = start_index + 1;
    }


    /* Bring CBR_FSGM PATT_INS into reset */
    for (i = start_index; i < end_index; i++)
    {
        cbr_fsgm_patt_ins_reset(cbr_fsgm_handle, i, CBR_FSGM_RESET);
        cbr_fsgm_patt_ins_enable(cbr_fsgm_handle, i, CBR_FSGM_DISABLE);
    } 
    
    /* Disable fault insertion */
    for (i = start_index; i < end_index; i++)
    {
        cbr_fsgm_field_CH_n_FAULT_INS_ENA_set(NULL, cbr_fsgm_handle, i, CBR_FSGM_DISABLE);
        cbr_fsgm_field_CH_n_FAULT_PROP_ENA_set(NULL, cbr_fsgm_handle, i, CBR_FSGM_DISABLE);
        cbr_fsgm_field_CH_n_LOS_INS_ENA_set(NULL, cbr_fsgm_handle, i, CBR_FSGM_DISABLE);
        cbr_fsgm_field_CH_n_LOS_PROP_ENA_set(NULL, cbr_fsgm_handle, i, CBR_FSGM_DISABLE);
    }


    /* For 40G links, CH0 disables the logical group */
    cbr_fsgm_ch_en(cbr_fsgm_handle, index, CBR_FSGM_DISABLE);


    PMC_RETURN(result);
} /* cbr_fsgm_deactivate */


/*******************************************************************************
* FUNCTION: cbr_fsgm_cleanup()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Cleanup register space CBR_FSGM.
*
* INPUTS:
*   cbr_fsgm_handle     - Handle to block
*   slice_start_index    - Current block start index
*                          link index that are out out slice index 
*                          range will be ignored.
*   num_link_to_clean    - Number of links to cleanup
*   links                - List of links to cleanup
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void cbr_fsgm_cleanup(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                             UINT32                slice_start_index,
                             UINT32                num_link_to_clean,
                             UINT32               *links)
{
    UINT32 index;
    UINT32 itr;
    UINT32 num_link_to_clean_in_slice = 0;

    PMC_ENTRY();
    
    PMC_ASSERT(cbr_fsgm_handle!=NULL, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);

    for (itr = 0; itr < num_link_to_clean; itr += 1)
    {        
        if (slice_start_index > links[itr])
        {
            continue;
        }

        if (links[itr] >= (slice_start_index + CBR_FSGM_MAX_INDEX))
        {
            break;
        }
            
        index = links[itr] % CBR_FSGM_MAX_INDEX;

        if (0 == (links[itr] % CBR_FSGM_MAX_INDEX))
        {
            num_link_to_clean_in_slice = 1;
        }
        else
        {            
            num_link_to_clean_in_slice += 1;
        }

        /* Store mode internally */
        cbr_fsgm_handle->var.mode[index] = CBR_FSGM_MODE_NONE;

        cbr_fsgm_patt_ins_reset(cbr_fsgm_handle, index, CBR_FSGM_RESET);
        cbr_fsgm_patt_ins_enable(cbr_fsgm_handle, index, CBR_FSGM_DISABLE);

        cbr_fsgm_field_CH_n_FAULT_INS_ENA_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_DISABLE);
        cbr_fsgm_field_CH_n_FAULT_PROP_ENA_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_DISABLE);
        cbr_fsgm_field_CH_n_LOS_INS_ENA_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_DISABLE);
        cbr_fsgm_field_CH_n_LOS_PROP_ENA_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_DISABLE);

        /* For 40G links, CH0 disables the logical group */
        cbr_fsgm_ch_en(cbr_fsgm_handle, index, CBR_FSGM_DISABLE);

        if (CBR_FSGM_MAX_INDEX == num_link_to_clean_in_slice)
        {
            cbr_fsgm_reset(cbr_fsgm_handle, CBR_FSGM_RESET);
        }
    }

    PMC_RETURN();
} /* cbr_fsgm_cleanup */

/*******************************************************************************
* FUNCTION: cbr_fsgm_reset()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Puts the CBR FSGM into or out of reset.   
*
* INPUTS:
*   cbr_fsgm_handle          - Pointer to subsystem handle
*   reset                    - CBR_FSGM_RESET or CBR_FSGM_ACTIVE
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_reset(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                BOOL                  reset)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);

    cbr_fsgm_field_SW_RESET_set(NULL, cbr_fsgm_handle, reset);


    PMC_RETURN(result);
} /* cbr_fsgm_reset */






/*******************************************************************************
* FUNCTION: cbr_fsgm_ch_en()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables a channel in the CBR FSGM. 
*
* INPUTS:
*   cbr_fsgm_handle          - Pointer to subsystem handle
*   index                    - Index within block (0..3)
*   enable                   - CBR_FSGM_ENABLE or CBR_FSGM_DISABLE
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_ch_en(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                UINT32                index,
                                BOOL                  enable)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);

    cbr_fsgm_field_CH_n_ENA_set(NULL, cbr_fsgm_handle, index, enable);
    cbr_fsgm_field_CH_n_PN11_MON_ENA_set(NULL, cbr_fsgm_handle, index, enable);

    PMC_RETURN(result);
} /* cbr_fsgm_ch_en */





/*******************************************************************************
* FUNCTION: cbr_fsgm_patt_ins_reset()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Puts an index within the PATT_INS into or out of reset.
*
* INPUTS:
*   cbr_fsgm_handle          - Pointer to subsystem handle
*   index                    - Index within block (0..3)
*   reset                    - CBR_FSGM_RESET or CBR_FSGM_ACTIVE
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_patt_ins_reset(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                         UINT32                index,
                                         BOOL                  reset)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();

    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);


    fc_cbr_patt_ins_field_SOFT_RST_set(NULL, cbr_fsgm_handle, index, reset);


    PMC_RETURN(result);
} /* cbr_fsgm_patt_ins_reset */


/*******************************************************************************
* FUNCTION: cbr_fsgm_patt_ins_reset_status_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Puts an index within the PATT_INS into or out of reset.
*
* INPUTS:
*   cbr_fsgm_handle          - Pointer to subsystem handle
*   index                    - Index within block (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 cbr_fsgm_patt_ins_reset_status_get(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                                UINT32                index)
{
    BOOL8     result;

    PMC_ENTRY();

    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);

    result = fc_cbr_patt_ins_field_SOFT_RST_get(NULL, cbr_fsgm_handle, index);


    PMC_RETURN(result);
} /* cbr_fsgm_patt_ins_reset_status_get */




/*******************************************************************************
* FUNCTION: cbr_fsgm_gap_val_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set Numerator Gap ratio value for all traffic type( FC800,CPRI,GDPS). 
*   Ser the pattern insertion mode (8b10b for FC800,CPRI/GDPS) 
*
* INPUTS:
*   cbr_fsgm_handle          - Pointer to subsystem handle
*   index                    - Index within block (0..3)
*   gap_mode                 - Traffic mode 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_gap_val_set(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                      UINT32                index,
                                      cbr_fsgm_mode_t       gap_mode)
{
    UINT32 cbr_fsgm_gap_denom = CBR_FSGM_GAP_DENOM;
    PMC_ERROR     result = PMC_SUCCESS;  
    
    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);

    if (CBR_FSGM_MODE_10G_10GE_LF == gap_mode ||
        CBR_FSGM_MODE_10G_10GE_RF == gap_mode)
    {
        cbr_fsgm_gap_denom = CBR_FSGM_GAP_DENOM_10GE;
    }

    switch (gap_mode) 
    {
        case CBR_FSGM_MODE_10G_FC800:
           cbr_fsgm_field_CH_n_PATT_MODE_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_8B10B_PATT_MODE);
           cbr_fsgm_field_CH_n_GAP_NUM_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_GAP_NUM_FC800);  
           break;
        case CBR_FSGM_MODE_10G_CPRI_5G:
           cbr_fsgm_field_CH_n_PATT_MODE_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_8B10B_PATT_MODE);
           cbr_fsgm_field_CH_n_GAP_NUM_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_GAP_NUM_CPRI5);  
           break;
        case CBR_FSGM_MODE_10G_CPRI_6G:
           cbr_fsgm_field_CH_n_PATT_MODE_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_8B10B_PATT_MODE);
           cbr_fsgm_field_CH_n_GAP_NUM_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_GAP_NUM_CPRI6);  
           break;
        case CBR_FSGM_MODE_10G_CPRI_10G:
           cbr_fsgm_field_CH_n_GAP_NUM_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_GAP_NUM_CPRI10);  
           cbr_fsgm_field_CH_n_PATT_MODE_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_8B10B_PATT_MODE);
           break;
        case CBR_FSGM_MODE_10G_GDPS_5G:
           cbr_fsgm_field_CH_n_PATT_MODE_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_8B10B_PATT_MODE);
           cbr_fsgm_field_CH_n_GAP_NUM_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_GAP_NUM_GDPS5);  
           break;
        case CBR_FSGM_MODE_10G_GDPS_10G:
           cbr_fsgm_field_CH_n_PATT_MODE_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_8B10B_PATT_MODE);
           cbr_fsgm_field_CH_n_GAP_NUM_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_GAP_NUM_GDPS10);  
           break;
        case CBR_FSGM_MODE_10G_FC1200_ODU1F:
        case CBR_FSGM_MODE_10G_FC1200_ODU2F:
           cbr_fsgm_field_CH_n_PATT_MODE_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_64B66B_PATT_MODE);
           cbr_fsgm_field_CH_n_GAP_NUM_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_GAP_NUM_FC1200);  
           break;
        case CBR_FSGM_MODE_10G_STS192:
        case CBR_FSGM_MODE_10G_STM64:
        case CBR_FSGM_MODE_40G_STS768:
        case CBR_FSGM_MODE_40G_STM256:
           cbr_fsgm_field_CH_n_PATT_MODE_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_PN11_PATT_MODE);
           cbr_fsgm_field_CH_n_GAP_NUM_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_GAP_NUM_STS192);  
           break;
        case CBR_FSGM_MODE_10G_10GE_LF:
        case CBR_FSGM_MODE_10G_10GE_RF:
           cbr_fsgm_field_CH_n_PATT_MODE_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_64B66B_PATT_MODE);
           cbr_fsgm_field_CH_n_GAP_NUM_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_GAP_NUM_10GE);  
           break;
        default:
            break;
    }     

    cbr_fsgm_field_CH_n_GAP_DENOM_set(NULL, cbr_fsgm_handle, index, cbr_fsgm_gap_denom);

    PMC_RETURN(result);
} /* cbr_fsgm_gap_val_set */


/*******************************************************************************
* FUNCTION: cbr_fsgm_bus_width_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set Bus Width value for all traffic type( FC800,CPRI,GDPS). 
*
* INPUTS:
*   cbr_fsgm_handle          - Pointer to subsystem handle
*   index                    - Index within block (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_bus_width_set(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                        UINT8                 index)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);

    switch (cbr_fsgm_handle->var.mode[index])
      {
      case CBR_FSGM_MODE_NONE:
      case CBR_FSGM_MODE_10G_FC1200_GFP_ODU2E:
      case CBR_FSGM_MODE_40G_STM256:
      case CBR_FSGM_MODE_40G_STS768:
        break; 

      case CBR_FSGM_MODE_10G_FC800:
      case CBR_FSGM_MODE_10G_CPRI_5G:
      case CBR_FSGM_MODE_10G_CPRI_6G:
      case CBR_FSGM_MODE_10G_CPRI_10G:
      case CBR_FSGM_MODE_10G_GDPS_5G:
      case CBR_FSGM_MODE_10G_GDPS_10G:
      case CBR_FSGM_MODE_10G_8B_10B_IDLE:
      case CBR_FSGM_MODE_10G_8B_10B_ARB:      
        fc_cbr_patt_ins_field_BUS_WIDTH_set(NULL, cbr_fsgm_handle, index, 
                                            FC_CBR_PATT_INS_CFG_BUS_WIDTH_40BIT_8B_10B);
        break;

      case CBR_FSGM_MODE_10G_STS192:
      case CBR_FSGM_MODE_10G_STM64:
      case CBR_FSGM_MODE_10G_10GE_LF:
      case CBR_FSGM_MODE_10G_10GE_RF:
        fc_cbr_patt_ins_field_BUS_WIDTH_set(NULL, cbr_fsgm_handle, index, 
                                            FC_CBR_PATT_INS_CFG_BUS_WIDTH_32BIT);
        break;

      case CBR_FSGM_MODE_10G_FC1200_ODU1F:
      case CBR_FSGM_MODE_10G_FC1200_ODU2F:
    case CBR_FSGM_MODE_10G_64B_66B_IDLE:
        fc_cbr_patt_ins_field_BUS_WIDTH_set(NULL, cbr_fsgm_handle, index, 
                                            FC_CBR_PATT_INS_CFG_BUS_WIDTH_32BIT);
        break;

      case CBR_FSGM_MODE_10G_STS192_PRBS:
      case CBR_FSGM_MODE_10G_STM64_PRBS:
        break;

      default:
        PMC_ASSERT(0, CBR_FSGM_ERR_CODE_ASSERT, cbr_fsgm_handle->var.mode[index], LAST_CBR_FSGM_MODE);
        break;
      } /* end switch */
    
    
    PMC_RETURN(result);
}; /* cbr_fsgm_bus_width_set */

/*******************************************************************************
* FUNCTION: cbr_fsgm_fault_mode_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set Fault Mode value for all traffic type( FC800,CPRI,GDPS). 
*
* INPUTS:
*   cbr_fsgm_handle          - Pointer to subsystem handle
*   index                    - Index within block (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_fault_mode_set(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                         UINT8                 index)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);

    switch (cbr_fsgm_handle->var.mode[index])
      {
      case CBR_FSGM_MODE_NONE:
      case CBR_FSGM_MODE_10G_FC1200_GFP_ODU2E:
      case CBR_FSGM_MODE_40G_STM256:
      case CBR_FSGM_MODE_40G_STS768:
        break; 

      case CBR_FSGM_MODE_10G_FC800:
      case CBR_FSGM_MODE_10G_CPRI_5G:
      case CBR_FSGM_MODE_10G_CPRI_6G:
      case CBR_FSGM_MODE_10G_CPRI_10G:
      case CBR_FSGM_MODE_10G_GDPS_5G:
      case CBR_FSGM_MODE_10G_GDPS_10G:
      case CBR_FSGM_MODE_10G_8B_10B_IDLE:
      case CBR_FSGM_MODE_10G_8B_10B_ARB:      
        fc_cbr_patt_ins_field_FAULT_MODE_set(NULL, cbr_fsgm_handle, index,
                                             FC_CBR_PATT_INS_CFG_FAULT_MODE_Nx8BIT_Nx10BIT);
        fc_cbr_patt_ins_field_SCRAM_EN_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_DISABLE);
        break;

      case CBR_FSGM_MODE_10G_STS192:
      case CBR_FSGM_MODE_10G_STM64:
        fc_cbr_patt_ins_field_FAULT_MODE_set(NULL, cbr_fsgm_handle, index, 
                                             FC_CBR_PATT_INS_CFG_FAULT_MODE_PN_11);
        fc_cbr_patt_ins_field_SCRAM_EN_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_DISABLE);
        break;
      
      case CBR_FSGM_MODE_10G_FC1200_ODU1F:
      case CBR_FSGM_MODE_10G_FC1200_ODU2F:
      case CBR_FSGM_MODE_10G_10GE_LF:
      case CBR_FSGM_MODE_10G_10GE_RF:
      case CBR_FSGM_MODE_10G_64B_66B_IDLE:
        fc_cbr_patt_ins_field_FAULT_MODE_set(NULL, cbr_fsgm_handle, index, 
                                             FC_CBR_PATT_INS_CFG_FAULT_MODE_64B_66B);
        fc_cbr_patt_ins_field_SCRAM_EN_set(NULL, cbr_fsgm_handle, index, CBR_FSGM_ENABLE);
        break;

      case CBR_FSGM_MODE_10G_STS192_PRBS:
      case CBR_FSGM_MODE_10G_STM64_PRBS:
        break;

      default:
        PMC_ASSERT(0, CBR_FSGM_ERR_CODE_ASSERT, cbr_fsgm_handle->var.mode[index], LAST_CBR_FSGM_MODE);
        break;
      } /* end switch */
    
    
    PMC_RETURN(result);
}; /* cbr_fsgm_fault_mode_set */

/*******************************************************************************
* FUNCTION: cbr_fsgm_fault_size_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Set Fault Size value for all traffic type( FC800,CPRI,GDPS). 
*
* INPUTS:
*   cbr_fsgm_handle          - Pointer to subsystem handle
*   index                    - Index within block (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_fault_size_set(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                         UINT8                 index)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);

    switch (cbr_fsgm_handle->var.mode[index])
      {
      case CBR_FSGM_MODE_NONE:
      case CBR_FSGM_MODE_10G_FC1200_ODU1F:
      case CBR_FSGM_MODE_10G_FC1200_ODU2F:
      case CBR_FSGM_MODE_10G_FC1200_GFP_ODU2E:
      case CBR_FSGM_MODE_40G_STM256:
      case CBR_FSGM_MODE_40G_STS768:
      case CBR_FSGM_MODE_10G_STS192:
      case CBR_FSGM_MODE_10G_STM64:
      case CBR_FSGM_MODE_10G_64B_66B_IDLE:
        break; 

      case CBR_FSGM_MODE_10G_FC800:
      case CBR_FSGM_MODE_10G_8B_10B_IDLE:
      case CBR_FSGM_MODE_10G_8B_10B_ARB:      
        fc_cbr_patt_ins_field_FAULT_SIZE_set(NULL, cbr_fsgm_handle, index, 
                                             FC_CBR_PATT_INS_CFG_FAULT_SIZE_4);
        break;

      case CBR_FSGM_MODE_10G_CPRI_5G:
      case CBR_FSGM_MODE_10G_CPRI_6G:
      case CBR_FSGM_MODE_10G_CPRI_10G:
      case CBR_FSGM_MODE_10G_GDPS_5G:
      case CBR_FSGM_MODE_10G_GDPS_10G:
        fc_cbr_patt_ins_field_FAULT_SIZE_set(NULL, cbr_fsgm_handle, index,
                                             FC_CBR_PATT_INS_CFG_FAULT_SIZE_16);
        break;

      case CBR_FSGM_MODE_10G_STS192_PRBS:
      case CBR_FSGM_MODE_10G_STM64_PRBS:
      case CBR_FSGM_MODE_10G_10GE_LF:
      case CBR_FSGM_MODE_10G_10GE_RF:
        break;

      default:
        PMC_ASSERT(0, CBR_FSGM_ERR_CODE_ASSERT, cbr_fsgm_handle->var.mode[index], LAST_CBR_FSGM_MODE);
        break;
      } /* end switch */
    
    
    PMC_RETURN(result);
}; /* cbr_fsgm_fault_size_set */

/*******************************************************************************
* FUNCTION: cbr_fsgm_user_fault_pattern_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets User Fault Pattern value to be inserted.
*
* INPUTS:
*   cbr_fsgm_handle          - Pointer to subsystem handle
*   index                    - Index within block (0..3)
*   user_pattern             - User fault pattern to configure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_user_fault_pattern_set(cbr_fsgm_handle_t              *cbr_fsgm_handle,
                                                 UINT8                           index,
                                                 cbr_fsgm_user_fault_pattern_t   user_pattern)
{
    PMC_ERROR     result = PMC_SUCCESS;
    UINT32        num_poll_fails = 0;
    const cbr_fsgm_pattern_t pattern_type[LAST_CBR_FSGM_USER_FAULT_PATTERN] =
                    { 
                        LAST_CBR_FSGM_PATTERN,      /* CBR_FSGM_USER_FAULT_PATTERN_NONE  */
                        CBR_FSGM_PATTERN_8B_10B,    /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_K30_7_CODE */
                        CBR_FSGM_PATTERN_8B_10B,    /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_ERR_CODE */
                        CBR_FSGM_PATTERN_8B_10B,    /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_FC_NOS */
                        CBR_FSGM_PATTERN_8B_10B,    /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B */
                        CBR_FSGM_PATTERN_8B_10B,   /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_ARB */
                        CBR_FSGM_PATTERN_64B_66B,   /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B */
                        CBR_FSGM_PATTERN_64B_66B,   /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B_LF_10GE */
                        CBR_FSGM_PATTERN_64B_66B,   /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B_RF_10GE */
                        CBR_FSGM_PATTERN_8B_10B,    /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_FC_IDLE */
                        CBR_FSGM_PATTERN_64B_66B,   /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B_FC_IDLE */
                    };


    const UINT32  flt_patt[LAST_CBR_FSGM_USER_FAULT_PATTERN][CBRC_FSGM_USER_PATTERN_NUM_WORD] =
                    { 
                        /* CBR_FSGM_USER_FAULT_PATTERN_NONE  */
                        {0, 0, 0, 0, 0}, 

                        /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_K30_7_CODE */
                        {
                            FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_159_128
                         },

                        /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_ERR_CODE */
                        {
                            FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_159_128
                         },
                        
                        /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_FC_NOS */
                        {
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_159_128
                         },
                        
                        /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B */
                        {
                            FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_159_128
                         },
                        
                       /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_ARB */
                        {
                            FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_159_128
                         },

                      /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B */
                        {
                            FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_159_128
                         },

                        /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B_LF_10GE */
                        {
                            FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_159_128
                         },

                        /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B_RF_10GE */
                        {
                            FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_159_128
                         },

            /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_FC_IDLE */
                        {
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_159_128
                         },

            /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B_FC_IDLE */
                        {
                            FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_159_128
                         },
                    };

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);

    if (user_pattern >= LAST_CBR_FSGM_USER_FAULT_PATTERN)
    {
        PMC_RETURN(CBR_FSGM_ERR_INVALID_ARG);
    }
  
    /* Configure CBR FSGM to insert proper 8b/10b or 64b/66b pattern */
    if (LAST_CBR_FSGM_PATTERN != pattern_type[user_pattern])
    {
        result = cbr_fsgm_pattern_cfg(cbr_fsgm_handle, index, pattern_type[user_pattern]);
    }



    /* Configure user pattern array */
    if (PMC_SUCCESS == result)
    {
        fc_cbr_patt_ins_lfield_USER_FLT_PATT_set(NULL, cbr_fsgm_handle, index, (UINT32*) flt_patt[user_pattern]);



        /* poll to ensure TIP=0 and then update the pattern */
        if ((0 != fc_cbr_patt_ins_field_TIP_poll(NULL, cbr_fsgm_handle, index, 0, PMC_POLL_EQUAL, 
                                                 CBRC_FSGM_PATT_INS_TIP_POLL_MAX_CNT,
                                                 &num_poll_fails, CBRC_FSGM_PATT_INS_TIP_POLL_DELAY)) ||
            (num_poll_fails >= CBRC_FSGM_PATT_INS_TIP_POLL_MAX_CNT))
        {
           result = FALSE;
        }
        else
        {
            fc_cbr_patt_ins_field_UPDATE_USER_PATT_set(NULL, cbr_fsgm_handle, index, 1);
        }
    }

    PMC_RETURN(result);
}; /* cbr_fsgm_user_fault_pattern_set */


/*******************************************************************************
* FUNCTION: cbr_fsgm_user_fault_pattern_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves configured User Fault Pattern value to be inserted.
*
* INPUTS:
*   cbr_fsgm_handle          - Pointer to subsystem handle
*   index                    - Index within block (0..3)
*
* OUTPUTS:
*   user_pattern_ptr         - Configured user fault pattern
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_user_fault_pattern_get(cbr_fsgm_handle_t              *cbr_fsgm_handle,
                                                 UINT8                           index,
                                                 cbr_fsgm_user_fault_pattern_t  *user_pattern_ptr)
{
    const cbr_fsgm_pattern_t pattern_type[LAST_CBR_FSGM_USER_FAULT_PATTERN] =
                    { 
                        LAST_CBR_FSGM_PATTERN,      /* CBR_FSGM_USER_FAULT_PATTERN_NONE  */
                        CBR_FSGM_PATTERN_8B_10B,    /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_K30_7_CODE */
                        CBR_FSGM_PATTERN_8B_10B,    /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_ERR_CODE */
                        CBR_FSGM_PATTERN_8B_10B,    /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_FC_NOS */
                        CBR_FSGM_PATTERN_8B_10B,    /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B */
                        CBR_FSGM_PATTERN_8B_10B,   /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_ARB */                        
                        CBR_FSGM_PATTERN_64B_66B,   /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B */
                        CBR_FSGM_PATTERN_64B_66B,   /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B_LF_10GE */
                        CBR_FSGM_PATTERN_64B_66B,   /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B_RF_10GE */
                        CBR_FSGM_PATTERN_8B_10B,    /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_FC_IDLE */
                        CBR_FSGM_PATTERN_64B_66B,   /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B_FC_IDLE */
                    };


    const UINT32  flt_patt[LAST_CBR_FSGM_USER_FAULT_PATTERN][CBRC_FSGM_USER_PATTERN_NUM_WORD] =
                    { 
                        /* CBR_FSGM_USER_FAULT_PATTERN_NONE  */
                        {0, 0, 0, 0, 0}, 

                        /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_K30_7_CODE */
                        {
                            FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_8B_10B_K30_7_CODE_USER_FLT_PATT_159_128
                         },

                        /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_ERR_CODE */
                        {
                            FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_8B_10B_ERR_CODE_USER_FLT_PATT_159_128
                         },
                        
                        /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_FC_NOS */
                        {
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_NOS_USER_FLT_PATT_159_128
                         },
                        
                        /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B */
                        {
                            FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_8B_10B_USER_FLT_PATT_159_128
                         },

                       /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_ARB */
                        {
                            FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_8B_10B_ARB_USER_FLT_PATT_159_128
                         },
                        
                        /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B */
                        {
                            FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_64B_66B_USER_FLT_PATT_159_128
                         },

                        /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B_LF_10GE */
                        {
                            FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_64B_66B_LF_10GE_USER_FLT_PATT_159_128
                         },

                        /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B_RF_10GE */
                        {
                            FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_64B_66B_RF_10GE_USER_FLT_PATT_159_128
                         },

            /* CBR_FSGM_USER_FAULT_PATTERN_8B_10B_FC_IDLE */
                        {
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_8B_10B_FC_IDLE_USER_FLT_PATT_159_128
                         },

            /* CBR_FSGM_USER_FAULT_PATTERN_64B_66B_FC_IDLE */
                        {
                            FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_31_0,
                            FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_63_32,
                            FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_95_64,
                            FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_127_96,
                            FC_CBR_PATT_INS_CFG_64B_66B_FC_IDLE_USER_FLT_PATT_159_128
                         }, 
                    };
    cbr_fsgm_pattern_t   pattern_type_act;
    UINT32  flt_patt_act[CBRC_FSGM_USER_PATTERN_NUM_WORD];
    UINT32 itr;
    UINT32 jtr;
    PMC_ERROR     result = PMC_SUCCESS;

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != user_pattern_ptr, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);

    *user_pattern_ptr = CBR_FSGM_USER_FAULT_PATTERN_NONE;
    fc_cbr_patt_ins_lfield_USER_FLT_PATT_get(NULL, cbr_fsgm_handle, index, flt_patt_act);
    cbr_fsgm_pattern_cfg_get(cbr_fsgm_handle, index, &pattern_type_act);
    
    for (itr = 0; itr < LAST_CBR_FSGM_USER_FAULT_PATTERN; itr += 1)
    {
        if (pattern_type_act == pattern_type[itr])
        {
            for (jtr = 0; jtr < CBRC_FSGM_USER_PATTERN_NUM_WORD; jtr += 1)
            {
                if (flt_patt_act[jtr] != flt_patt[itr][jtr])
                {
                    break;
                }
            }
            if (CBRC_FSGM_USER_PATTERN_NUM_WORD == jtr)
            {
                *user_pattern_ptr = (cbr_fsgm_user_fault_pattern_t)itr;
                break;
            }
        }
    }

    PMC_RETURN(result);
}; /* cbr_fsgm_user_fault_pattern_get */



/*******************************************************************************
* FUNCTION: cbr_fsgm_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables or disables a channel's Fault Insertion in the CBR FSGM. 
*
* INPUTS:
*   cbr_fsgm_handle          - Pointer to subsystem handle
*   index                    - Index within block (0..3)
*   fault_ins_en             - cbr_fsgm fault insertion enable 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_cfg(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                              UINT32                index,
                              UINT32                fault_ins_en)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);

    cbr_fsgm_field_CH_n_FAULT_INS_ENA_set(NULL, cbr_fsgm_handle, index, fault_ins_en);

    PMC_RETURN(result);
} /* cbr_fsgm_cfg */


/*******************************************************************************
* FUNCTION: cbr_fsgm_pattern_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures fault pattern to be inserted during LOS or faults.
*   
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*   pattern         - Fault pattern to be inserted 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_pattern_cfg(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                      UINT32                index,
                                      cbr_fsgm_pattern_t    pattern)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);

    if (pattern >= LAST_CBR_FSGM_PATTERN)
    {
        PMC_RETURN(CBR_FSGM_ERR_INVALID_ARG);
    }


    /* 8b/10b and 64b/66b are only available for 10G */
    if (((CBR_FSGM_PATTERN_8B_10B == pattern) || (CBR_FSGM_PATTERN_64B_66B == pattern)) &&
        (cbr_fsgm_mode_is_40g(cbr_fsgm_handle, index)))
    {
        PMC_RETURN(CBR_FSGM_ERR_INVALID_PATTERN_FOR_40G);
    }

    
    cbr_fsgm_field_CH_n_PATT_MODE_set(NULL, cbr_fsgm_handle, index, pattern);
    

    PMC_RETURN(result);
} /* cbr_fsgm_pattern_cfg */



/*******************************************************************************
* FUNCTION: cbr_fsgm_pattern_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves configured fault pattern to be inserted during LOS or faults.
*   
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*
* OUTPUTS:
*  *pattern_ptr     - Configured fault pattern to be inserted 
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_pattern_cfg_get(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                          UINT32                index,
                                          cbr_fsgm_pattern_t   *pattern_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != pattern_ptr, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);


    
    *pattern_ptr = (cbr_fsgm_pattern_t) cbr_fsgm_field_CH_n_PATT_MODE_get(NULL, cbr_fsgm_handle, index);
    

    PMC_RETURN(result);
} /* cbr_fsgm_pattern_cfg_get */




/*******************************************************************************
* FUNCTION: cbr_fsgm_fault_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forces fault pattern into output. Fault pattern is configured with 
*   cbr_fsgm_pattern_cfg();
*   
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*   enable          - Enable or disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_fault_force(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                      UINT32                index,
                                      BOOL                  enable)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();

    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);


    cbr_fsgm_field_CH_n_FORCE_FAULT_set(NULL, cbr_fsgm_handle, index, enable);
    

    PMC_RETURN(result);
} /* cbr_fsgm_fault_force */



/*******************************************************************************
* FUNCTION: cbr_fsgm_fault_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if fault pattern is being forced into the output. 
*   
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*
* OUTPUTS:
*  *enable_ptr      - Feature is enabled or disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_fault_force_get(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                          UINT32                index,
                                          BOOL                 *enable_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();

    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);


    *enable_ptr = cbr_fsgm_field_CH_n_FORCE_FAULT_get(NULL, cbr_fsgm_handle, index);
    

    PMC_RETURN(result);
} /* cbr_fsgm_fault_force_get */





/*******************************************************************************
* FUNCTION: cbr_fsgm_pn11_mon_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables PN11 monitoring. Valid for 10G and 40G modes.
*   
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*   enable          - Enable PN11 monitoring
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_pn11_mon_enable(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                          UINT32                index,
                                          BOOL                  enable)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);


    cbr_fsgm_field_CH_n_PN11_MON_ENA_set(NULL, cbr_fsgm_handle, index, enable);

    PMC_RETURN(result);
} /* cbr_fsgm_pn11_mon_enable */



/*******************************************************************************
* FUNCTION: cbr_fsgm_pn11_mon_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if PN11 monitoring is enabled or disabled. 
*   
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*
* OUTPUTS:
*  *enable_ptr      - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_pn11_mon_enable_get(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                              UINT32                index,
                                              BOOL                 *enable_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);


    *enable_ptr = cbr_fsgm_field_CH_n_PN11_MON_ENA_get(NULL, cbr_fsgm_handle, index);

    PMC_RETURN(result);
} /* cbr_fsgm_pn11_mon_enable_get */





/*******************************************************************************
* FUNCTION: cbr_fsgm_insert_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures how patterns are inserted into the output stream.
*   
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*  *cfg_ptr         - pointer to pattern insertion configuration structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_insert_cfg(cbr_fsgm_handle_t        *cbr_fsgm_handle,
                                     UINT32                    index,
                                     cbr_fsgm_pattern_cfg_t   *cfg_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    
    UINT32        mask;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != cfg_ptr, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);

    mask = cfg_ptr->mask;

    if (mask & CBR_FSGM_PATTERN_CFG_MASK_FAULT_PROP)
    {
        cbr_fsgm_field_CH_n_FAULT_PROP_ENA_set(NULL, cbr_fsgm_handle, index, cfg_ptr->fault_prop);
    }
    
    if (mask & CBR_FSGM_PATTERN_CFG_MASK_LOS_PROP)
    {
        cbr_fsgm_field_CH_n_LOS_PROP_ENA_set(NULL, cbr_fsgm_handle, index, cfg_ptr->los_prop);
    }

    if (mask & CBR_FSGM_PATTERN_CFG_MASK_FAULT_INSERT)
    {
        cbr_fsgm_field_CH_n_FAULT_INS_ENA_set(NULL, cbr_fsgm_handle, index, cfg_ptr->fault_insert);
    }
    
    if (mask & CBR_FSGM_PATTERN_CFG_MASK_LOS_INSERT)
    {
        cbr_fsgm_field_CH_n_LOS_INS_ENA_set(NULL, cbr_fsgm_handle, index, cfg_ptr->los_insert);
    }


    PMC_RETURN(result);
} /* cbr_fsgm_insert_cfg */


/*******************************************************************************
* FUNCTION: cbr_fsgm_insert_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves config for how patterns are inserted into the output stream.
*   
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*
* OUTPUTS:
*  *cfg_ptr         - Configured pattern insertion configuration structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_insert_cfg_get(cbr_fsgm_handle_t        *cbr_fsgm_handle,
                                         UINT32                    index,
                                         cbr_fsgm_pattern_cfg_t   *cfg_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != cfg_ptr, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);


    cfg_ptr->mask = 0;

    cfg_ptr->mask |= CBR_FSGM_PATTERN_CFG_MASK_FAULT_PROP;
    cfg_ptr->fault_prop = cbr_fsgm_field_CH_n_FAULT_PROP_ENA_get(NULL, cbr_fsgm_handle, index);
    
    cfg_ptr->mask |= CBR_FSGM_PATTERN_CFG_MASK_LOS_PROP;
    cfg_ptr->los_prop = cbr_fsgm_field_CH_n_LOS_PROP_ENA_get(NULL, cbr_fsgm_handle, index);

    cfg_ptr->mask |= CBR_FSGM_PATTERN_CFG_MASK_FAULT_INSERT;
    cfg_ptr->fault_insert = cbr_fsgm_field_CH_n_FAULT_INS_ENA_get(NULL, cbr_fsgm_handle, index);
    
    cfg_ptr->mask |= CBR_FSGM_PATTERN_CFG_MASK_LOS_INSERT;
    cfg_ptr->los_insert = cbr_fsgm_field_CH_n_LOS_INS_ENA_get(NULL, cbr_fsgm_handle, index);


    PMC_RETURN(result);
} /* cbr_fsgm_insert_cfg_get */



/*******************************************************************************
* FUNCTION: cbr_fsgm_patt_ins_fault_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forces the generation of the selected fault pattern.
*   
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*   force           - Enable the insertion of the fault pattern
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_patt_ins_fault_force(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                               UINT32                index,
                                               BOOL                  force)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);


    fc_cbr_patt_ins_field_FORCE_FAULT_set(NULL, cbr_fsgm_handle, index, force);

    PMC_RETURN(result);
} /* cbr_fsgm_patt_ins_fault_force */

/*******************************************************************************
* FUNCTION: cbr_fsgm_patt_ins_fault_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if the generation of the selected fault pattern is forced.
*   
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*
* OUTPUTS:
*   force_ptr       - Enable the insertion of the fault pattern
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_patt_ins_fault_force_get(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                                   UINT32                index,
                                                   BOOL                 *force_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);


    *force_ptr = fc_cbr_patt_ins_field_FORCE_FAULT_get(NULL, cbr_fsgm_handle, index);

    PMC_RETURN(result);
} /* cbr_fsgm_patt_ins_fault_force_get */




/*******************************************************************************
* FUNCTION: cbr_fsgm_patt_ins_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables the insertion of the fault pattern as a consequential action based
*   on configured traffic mode in channel
*   
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*   enable          - Enables the insertion when needed
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_patt_ins_enable(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                          UINT32                index,
                                          BOOL                  enable)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);


    fc_cbr_patt_ins_field_FC_CBR_PATT_INS_EN_set(NULL, cbr_fsgm_handle, index, enable);

    PMC_RETURN(result);
} /* cbr_fsgm_patt_ins_enable */

/*******************************************************************************
* FUNCTION: cbr_fsgm_patt_ins_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves  if the insertion of the fault pattern as a consequential action based
*   on configured traffic mode in channel is enabled or disabled.
*   
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*
* OUTPUTS:
*  *enable_ptr     - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_patt_ins_enable_get(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                              UINT32                index,
                                              BOOL                 *enable_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);


    *enable_ptr = fc_cbr_patt_ins_field_FC_CBR_PATT_INS_EN_get(NULL, cbr_fsgm_handle, index);

    PMC_RETURN(result);
} /* cbr_fsgm_patt_ins_enable_get */




/*******************************************************************************
* FUNCTION: cbr_fsgm_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   cbr_fsgm_handle       - Pointer to subsystem handle
*   index                 - Index within CBR_FSGM (0..11)
*   non_null_ptr          - Pointer to be checked that can't be NULL
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_int_validate(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                       UINT32                index,
                                       void                 *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
   
    if (index >= CBR_FSGM_MAX_INDEX)
    {
        result = CBR_FSGM_ERR_INVALID_ARG;
    }

    if (NULL == non_null_ptr)
    {
        result = CBR_FSGM_ERR_NULL_HANDLE;
    }


    PMC_RETURN(result);
} /* cbr_fsgm_int_validate */


/*******************************************************************************
* FUNCTION: cbr_fsgm_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   cbr_fsgm_handle   - Pointer to subsystem handle
*   index             - Index within CBR_FSGM (0..11)
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to enable interrupts. If NULL, enables/disables
*                       all interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*   enable            - TRUE to enable interrupts, FALSE to disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_int_chnl_enable(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                          UINT32                index,
                                          cbr_fsgm_int_chnl_t  *int_table_ptr,
                                          cbr_fsgm_int_chnl_t  *int_en_table_ptr,
                                          BOOL                  enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link   = index;

    PMC_ENTRY();

    result = cbr_fsgm_int_validate(cbr_fsgm_handle, index, int_en_table_ptr);

    #ifdef CBR_FSGM_INT
    #undef CBR_FSGM_INT
    #endif
    #define CBR_FSGM_INT    UTIL_GLOBAL_INT_ENABLE
  
    #ifdef CBR_FSGM_INT_FIELD
    #undef CBR_FSGM_INT_FIELD
    #endif
    #define CBR_FSGM_INT_FIELD    UTIL_GLOBAL_INT_FIELD4_ENABLE
   
    CBR_FSGM_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* cbr_fsgm_int_chnl_enable */




/*******************************************************************************
* FUNCTION: cbr_fsgm_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   cbr_fsgm_handle   - Pointer to subsystem handle
*   index             - Index within CBR_FSGM (0..11)
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_int_chnl_clear(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                         UINT32                index,
                                         cbr_fsgm_int_chnl_t  *int_table_ptr,
                                         cbr_fsgm_int_chnl_t  *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link   = index;

    PMC_ENTRY();

    result = cbr_fsgm_int_validate(cbr_fsgm_handle, index, int_en_table_ptr);

    #ifdef CBR_FSGM_INT
    #undef CBR_FSGM_INT
    #endif
    #define CBR_FSGM_INT UTIL_GLOBAL_INT_CLEAR
    
    #ifdef CBR_FSGM_INT_FIELD
    #undef CBR_FSGM_INT_FIELD
    #endif
    #define CBR_FSGM_INT_FIELD    UTIL_GLOBAL_INT_FIELD4_CLEAR
   

    CBR_FSGM_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* cbr_fsgm_int_chnl_clear */




/*******************************************************************************
* FUNCTION: cbr_fsgm_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   cbr_fsgm_handle   - Pointer to subsystem handle
*   index             - Index within CBR_FSGM (0..11)
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
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_int_chnl_retrieve(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                                            UINT32                index,
                                            cbr_fsgm_int_chnl_t  *filt_table_ptr,
                                            cbr_fsgm_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link   = index;

    PMC_ENTRY();

    result = cbr_fsgm_int_validate(cbr_fsgm_handle, index, int_table_ptr);

    
    UTIL_GLOBAL_INT_BUFFER_INIT_SINGLE(cbr_fsgm_handle, cbr_fsgm, UTIL_GLOBAL_INT_OPTIMIZE);
    UTIL_GLOBAL_INT_BUFFER_INIT(cbr_fsgm_handle, fc_cbr_patt_ins, UTIL_GLOBAL_INT_OPTIMIZE);

    #ifdef CBR_FSGM_INT
    #undef CBR_FSGM_INT
    #endif
    #define CBR_FSGM_INT UTIL_GLOBAL_INT_RETRIEVE
    
    #ifdef CBR_FSGM_INT_FIELD
    #undef CBR_FSGM_INT_FIELD
    #endif
    #define CBR_FSGM_INT_FIELD    UTIL_GLOBAL_INT_FIELD4_RETRIEVE
   

    CBR_FSGM_INT_TABLE_DEFINE();
    
    
    #ifdef CBR_FSGM_STATUS
    #undef CBR_FSGM_STATUS
    #endif
    #define CBR_FSGM_STATUS UTIL_GLOBAL_STATUS_RETRIEVE
    
    #ifdef CBR_FSGM_STATUS_FIELD
    #undef CBR_FSGM_STATUS_FIELD
    #endif
    #define CBR_FSGM_STATUS_FIELD    UTIL_GLOBAL_STATUS_FIELD4_RETRIEVE
   

    CBR_FSGM_STATUS_TABLE_DEFINE();
   
    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbr_fsgm_handle, cbr_fsgm);
    UTIL_GLOBAL_INT_BUFFER_FLUSH(cbr_fsgm_handle, fc_cbr_patt_ins);


    PMC_RETURN(result);
} /* cbr_fsgm_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: cbr_fsgm_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   cbr_fsgm_handle   - Pointer to subsystem handle
*   index             - Index within CBR_FSGM (0..11)
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR cbr_fsgm_int_chnl_enabled_check(cbr_fsgm_handle_t     *cbr_fsgm_handle,
                                                 UINT32                 index,
                                                 cbr_fsgm_int_chnl_t   *int_en_table_ptr,
                                                 BOOL                  *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link   = index;

    PMC_ENTRY();

    result = cbr_fsgm_int_validate(cbr_fsgm_handle, index, int_found_ptr);


    #ifdef CBR_FSGM_INT
    #undef CBR_FSGM_INT
    #endif
    #define CBR_FSGM_INT UTIL_GLOBAL_INT_CHECK
    
    #ifdef CBR_FSGM_INT_FIELD
    #undef CBR_FSGM_INT_FIELD
    #endif
    #define CBR_FSGM_INT_FIELD    UTIL_GLOBAL_INT_FIELD4_CHECK
   

    CBR_FSGM_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* cbr_fsgm_int_chnl_enabled_check */


/*
** Private Functions
*/

/*******************************************************************************
* FUNCTION: cbr_fsgm_mode_is_40g()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   BOOL             - TRUE if cbr_fsgm index is configured for 40G
*
* NOTES:
*
*******************************************************************************/
BOOL cbr_fsgm_mode_is_40g(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                          UINT32                index)
{
    BOOL    result = FALSE;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);

    switch (cbr_fsgm_handle->var.mode[index])
    {
        case CBR_FSGM_MODE_40G_STS768:
        case CBR_FSGM_MODE_40G_STM256:
            result = TRUE;
            break; 

        case CBR_FSGM_MODE_NONE:
        case CBR_FSGM_MODE_10G_STS192:
        case CBR_FSGM_MODE_10G_STM64:
        case CBR_FSGM_MODE_10G_FC1200_ODU1F:
        case CBR_FSGM_MODE_10G_FC1200_ODU2F:
        case CBR_FSGM_MODE_10G_FC1200_GFP_ODU2E:
        case CBR_FSGM_MODE_10G_FC800:
        case CBR_FSGM_MODE_10G_CPRI_5G:
        case CBR_FSGM_MODE_10G_CPRI_6G:
        case CBR_FSGM_MODE_10G_CPRI_10G:
        case CBR_FSGM_MODE_10G_GDPS_5G:
        case CBR_FSGM_MODE_10G_GDPS_10G:
        case CBR_FSGM_MODE_10G_STS192_PRBS:
        case CBR_FSGM_MODE_10G_STM64_PRBS:
        case CBR_FSGM_MODE_10G_10GE_LF:
        case CBR_FSGM_MODE_10G_10GE_RF:
        case CBR_FSGM_MODE_10G_8B_10B_IDLE:
        case CBR_FSGM_MODE_10G_64B_66B_IDLE:
        case CBR_FSGM_MODE_10G_8B_10B_ARB:    
            result = FALSE;
            break;

        default:
            PMC_ASSERT(0, CBR_FSGM_ERR_CODE_ASSERT, cbr_fsgm_handle->var.mode[index], LAST_CBR_FSGM_MODE);
            break;
    } /* end switch */
    
    PMC_RETURN(result);
} /* cbr_fsgm_mode_is_40g */

/*******************************************************************************
* FUNCTION: cbr_fsgm_mode_is_oc192()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*
* INPUTS:
*   cbr_fsgm_handle - Handle to block
*   index           - Index within CBR_FSGM block (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   BOOL             - TRUE if cbr_fsgm index is configured for OC192
*
* NOTES:
*
*******************************************************************************/
BOOL cbr_fsgm_mode_is_oc192(cbr_fsgm_handle_t    *cbr_fsgm_handle,
                            UINT32                index)
{
    BOOL    result = FALSE;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != cbr_fsgm_handle, CBR_FSGM_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < CBR_FSGM_MAX_INDEX, CBR_FSGM_ERR_INVALID_INDEX, index, CBR_FSGM_MAX_INDEX);

    switch (cbr_fsgm_handle->var.mode[index])
    {
        case CBR_FSGM_MODE_10G_STS192:
        case CBR_FSGM_MODE_10G_STM64:
        case CBR_FSGM_MODE_10G_10GE_LF:
        case CBR_FSGM_MODE_10G_10GE_RF:
            result = TRUE;
            break; 

        case CBR_FSGM_MODE_NONE:
        case CBR_FSGM_MODE_40G_STS768:
        case CBR_FSGM_MODE_40G_STM256:
        case CBR_FSGM_MODE_10G_FC1200_ODU1F:
        case CBR_FSGM_MODE_10G_FC1200_ODU2F:
        case CBR_FSGM_MODE_10G_FC1200_GFP_ODU2E:
        case CBR_FSGM_MODE_10G_FC800:
        case CBR_FSGM_MODE_10G_CPRI_5G:
        case CBR_FSGM_MODE_10G_CPRI_6G:
        case CBR_FSGM_MODE_10G_CPRI_10G:
        case CBR_FSGM_MODE_10G_GDPS_5G:
        case CBR_FSGM_MODE_10G_GDPS_10G:
        case CBR_FSGM_MODE_10G_STS192_PRBS:
        case CBR_FSGM_MODE_10G_STM64_PRBS:
		case CBR_FSGM_MODE_10G_8B_10B_IDLE:
        case CBR_FSGM_MODE_10G_64B_66B_IDLE:
        case CBR_FSGM_MODE_10G_8B_10B_ARB:        
            result = FALSE;
            break;

        default:
            PMC_ASSERT(0, CBR_FSGM_ERR_CODE_ASSERT, cbr_fsgm_handle->var.mode[index], LAST_CBR_FSGM_MODE);
            break;
    } /* end switch */
    
    PMC_RETURN(result);
} /* cbr_fsgm_mode_is_oc192 */

/*
** End of file
*/
