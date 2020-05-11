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
#include "fc800_pmg_loc.h"

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
const char FC800_PMG_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    FC800_PMG_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else  /* no strings */

const char FC800_PMG_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Public Functions
*/

/*******************************************************************************
* fc800_pmg_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an FC800_PMG block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the FC800_PMG subsystem relative to 
*                           the device memory space
*  *sys_handle            - pointer to user system handle
*  *tsb_name              - Name of subsystem
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   fc800_pmg_handle         - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC fc800_pmg_handle_t *fc800_pmg_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    fc800_pmg_handle_t *fc800_pmg_handle;

    PMC_ENTRY();

    fc800_pmg_handle = (fc800_pmg_handle_t*)PMC_CTXT_CALLOC(sizeof(fc800_pmg_handle_t), parent);
    
    pmc_handle_init(parent, fc800_pmg_handle, sys_handle, PMC_MID_DIGI_FC800_PMG, tsb_name, base_address);

    PMC_RETURN(fc800_pmg_handle);
} /* fc800_pmg_ctxt_create */

 
/*******************************************************************************
* fc800_pmg_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an FC800_PMG block instance.
*
*
* INPUTS:
*   *fc800_pmg_handle         - pointer to FC800_PMG handle instance
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
PUBLIC void fc800_pmg_ctxt_destroy(fc800_pmg_handle_t *fc800_pmg_handle)
{
    PMC_ENTRY();
    pmc_handle_mid_verify(fc800_pmg_handle, PMC_MID_DIGI_FC800_PMG);
    fc800_pmg_handle->base.tsb_mid = PMC_MID_INVALID;
    PMC_CTXT_FREE(&fc800_pmg_handle, fc800_pmg_handle);
    PMC_RETURN();
} /* fc800_pmg_ctxt_destroy */



/*******************************************************************************
* FUNCTION: fc800_pmg_var_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes dynamic context 
*
* INPUTS:
*   fc800_pmg_handle_ptr    - Pointer to block handle
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc800_pmg_var_init(fc800_pmg_handle_t      *fc800_pmg_handle_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;    
    UINT32          i;

    PMC_ENTRY();

    for (i = 0; i < FC800_PMG_MAX_INDEX; i++)
    {
        fc800_pmg_handle_ptr->var.mode[i] = LAST_FC800_PMG_MODE;    
    }



    PMC_RETURN(result);
} /* fc800_pmg_var_init */





/*******************************************************************************
* fc800_pmg_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an FC800_PMG block handle instance.
*
*
* INPUTS:
*   *fc800_pmg_handle         - pointer to FC800_PMG handle instance
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
PUBLIC void fc800_pmg_handle_init(fc800_pmg_handle_t *fc800_pmg_handle)
{
    PMC_ENTRY();

    fc800_pmg_var_init(fc800_pmg_handle);
     
    PMC_RETURN();
} /* fc800_pmg_handle_init */





/*******************************************************************************
* fc800_pmg_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bring FC800_PMG blocks into configurable state 
*
* INPUTS:
*   fc800_pmg_handle_ptr   - Pointer to subsystem handle
*   energy_state           - Energy state to put FC800_PMG into
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc800_pmg_init(fc800_pmg_handle_t    *fc800_pmg_handle_ptr,
                                pmc_energy_state_t     energy_state)
{

    PMC_ENTRY();
  
    /* Check Arguments */
    PMC_ASSERT(NULL != fc800_pmg_handle_ptr, FC800_PMG_ERR_NULL_HANDLE, 0, 0);
    

    switch (energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            break;

        case PMC_ENERGY_STATE_RESET:
            fc800_pmg_var_init(fc800_pmg_handle_ptr);
            break;
        
        default:
            PMC_ASSERT(FALSE, FC800_PMG_ERR_INVALID_ARG, energy_state, 0);
            break;
    } /* end switch */





    PMC_RETURN(PMC_SUCCESS);
} /* fc800_pmg_init */




/*******************************************************************************
* fc800_pmg_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   FC800_PMG instance.
*
*
* INPUTS:
*   *fc800_pmg_handle      - pointer to fc800_pmg handle instance to be operated on
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
PUBLIC BOOL fc800_pmg_start_state_test(fc800_pmg_handle_t *fc800_pmg_handle)
{
    BOOL                start_state = TRUE;

    /* Variable declaration */
    
    PMC_ENTRY();
    PMC_ASSERT(fc800_pmg_handle!=NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);
    
    PMC_RETURN(start_state);
    
} /* fc800_pmg_start_state_test */











/*******************************************************************************
* FUNCTION: fc800_pmg_mode_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures an index within the FC800 PMG to operate in the desired mode.
*
* INPUTS:
*   fc800_pmg_handle_ptr   - Pointer to subsystem handle
*   index                  - Index within FC800_PMG (0..11)
*   mode                   - Mode to configure the FC800_PMG
*   is_ilvr                - Distinguish dlvr or ilvr config, which with different
*                            settings of mode FC800_PMG_MODE_10G_8B_10B_ARB.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc800_pmg_mode_cfg(fc800_pmg_handle_t    *fc800_pmg_handle_ptr,
                                    UINT32                 index,
                                    fc800_pmg_mode_t       mode,
                                    BOOL8                  is_ilvr)
{
    PMC_ERROR result = PMC_SUCCESS;

    BOOL fc800_pmg_set = FC800_PMG_DISABLE;

    PMC_ENTRY();
    
    
    PMC_ASSERT(fc800_pmg_handle_ptr !=NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(mode < LAST_FC800_PMG_MODE, FC800_PMG_ERR_INVALID_ARG, mode, LAST_FC800_PMG_MODE);

    if (TRUE == is_ilvr)
    {
        fc800_pmg_set = FC800_PMG_ENABLE;
    }

    /* Store mode */
    fc800_pmg_handle_ptr->var.mode[index] = mode;

    /* Configure standard cfg */
    switch (mode)
    {
        case FC800_PMG_MODE_10G_FC800:
        case FC800_PMG_MODE_10G_CPRI_5G:
        case FC800_PMG_MODE_10G_CPRI_6G:
        case FC800_PMG_MODE_10G_CPRI_10G:
        case FC800_PMG_MODE_10G_GDPS_5G:
        case FC800_PMG_MODE_10G_GDPS_10G:
        case FC800_PMG_MODE_10G_8B_10B_IDLE:
        case FC800_PMG_MODE_10G_8B_10B_ARB:      
            fc_cbr_field_BUS_WIDTH_set             (NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_BUS_WIDTH);
            fc_cbr_field_RX_FC_CONT_ALIGN_MODE_set (NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_CONT_ALIGN_MODE);
            fc_cbr_field_RX_ENABLE_BYTE_ALIGN_set  (NULL, fc800_pmg_handle_ptr, index, FC800_PMG_ENABLE);
            fc_cbr_field_RX_LCV_DLOLB_EN_set       (NULL, fc800_pmg_handle_ptr, index, fc800_pmg_set);
            fc_cbr_field_RX_XDET_DLOLB_EN_set      (NULL, fc800_pmg_handle_ptr, index, fc800_pmg_set);
            fc_cbr_field_ASD_DLOLB_EN_set          (NULL, fc800_pmg_handle_ptr, index, FC800_PMG_ENABLE);
            fc_cbr_field_RX_LOSS_SYNC_DLOLB_EN_set (NULL, fc800_pmg_handle_ptr, index, FC800_PMG_DISABLE);
            fc_cbr_field_RX_DRL_DLOLB_EN_set       (NULL, fc800_pmg_handle_ptr, index, fc800_pmg_set);
            fc_cbr_field_RX_LCV_DLOLB_THRSH_set    (NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_LCV_DLOLB_THRSH);
            fc_cbr_field_RX_XDET_INTERVAL_set      (NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_XDET_INTERVAL);
            fc_cbr_field_RX_XDET_DEASSERT_THRSH_set(NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_XDET_DEASSERT_THRSH);
            fc_cbr_field_RX_FRM_SCRAM_set          (NULL, fc800_pmg_handle_ptr, index, FC800_PMG_ENABLE);
            fc_cbr_field_RX_DLOLB_INT_PER_set      (NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_DLOLB_INT_PER);
          
            break;

        case FC800_PMG_MODE_10G_FC1200_ODU1F:
        case FC800_PMG_MODE_10G_FC1200_ODU2F:
        case FC800_PMG_MODE_10G_FC1200_GFP_ODU2E:
        case FC800_PMG_MODE_10G_64B_66B_IDLE:
            break;
    
        case FC800_PMG_MODE_40G_STS768:
        case FC800_PMG_MODE_40G_STM256:
            break;
         
        case FC800_PMG_MODE_10G_STS192:
        case FC800_PMG_MODE_10G_STM64:
        case FC800_PMG_MODE_10G_STS192_PRBS:
        case FC800_PMG_MODE_10G_STM64_PRBS:
            break;

        default:
            PMC_ASSERT(0, FC800_PMG_ERR_INVALID_ARG, mode, LAST_FC800_PMG_MODE);
            break;
    } /* end switch */



    /* Configure traffic mode and clock divider */
    switch (mode)
    {
        case FC800_PMG_MODE_10G_FC800:
        case FC800_PMG_MODE_10G_8B_10B_IDLE:
        case FC800_PMG_MODE_10G_8B_10B_ARB:
            fc_cbr_field_MODE_set(NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_MODE_FC);
            fc_cbr_field_RX_DLOLB_CLK_DIV_set(NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_CLK_DIV_FC800);
            break;

        case FC800_PMG_MODE_10G_CPRI_5G:
            fc_cbr_field_MODE_set(NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_MODE_ESCON);
            fc_cbr_field_RX_DLOLB_CLK_DIV_set(NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_CLK_DIV_CPRI_5G);
            break;

        case FC800_PMG_MODE_10G_CPRI_6G:
            fc_cbr_field_MODE_set(NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_MODE_ESCON);
            fc_cbr_field_RX_DLOLB_CLK_DIV_set(NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_CLK_DIV_CPRI_6G);
            break;

        case FC800_PMG_MODE_10G_CPRI_10G:
            fc_cbr_field_MODE_set(NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_MODE_ESCON);
            fc_cbr_field_RX_DLOLB_CLK_DIV_set(NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_CLK_DIV_CPRI_10G);
            break;

        case FC800_PMG_MODE_10G_GDPS_5G:
            fc_cbr_field_MODE_set(NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_MODE_ESCON);
            fc_cbr_field_RX_DLOLB_CLK_DIV_set(NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_CLK_DIV_GDPS_5G);
            break;

        case FC800_PMG_MODE_10G_GDPS_10G:
            fc_cbr_field_MODE_set(NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_MODE_ESCON);
            fc_cbr_field_RX_DLOLB_CLK_DIV_set(NULL, fc800_pmg_handle_ptr, index, FC800_PMG_FC_CBR_CLK_DIV_GDPS_10G);
            break;

        case FC800_PMG_MODE_10G_FC1200_ODU1F:
        case FC800_PMG_MODE_10G_FC1200_ODU2F:
        case FC800_PMG_MODE_10G_FC1200_GFP_ODU2E:
        case FC800_PMG_MODE_10G_64B_66B_IDLE:
            break;
    

        case FC800_PMG_MODE_40G_STS768:
        case FC800_PMG_MODE_40G_STM256:
            break;
         
        case FC800_PMG_MODE_10G_STS192:
        case FC800_PMG_MODE_10G_STM64:
        case FC800_PMG_MODE_10G_STS192_PRBS:
        case FC800_PMG_MODE_10G_STM64_PRBS:
            break;

        default:
            PMC_ASSERT(0, FC800_PMG_ERR_INVALID_ARG, mode, LAST_FC800_PMG_MODE);
            break;
    } /* end switch */



    PMC_RETURN(result);
} /* fc800_pmg_mode_cfg */

/*******************************************************************************
* FUNCTION: fc800_pmg_mode_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Interpolate FC800 configured mode.
*
* INPUTS:
*   fc800_pmg_handle_ptr   - Pointer to subsystem handle
*   index                  - Index within FC800_PMG (0..11)
*
* OUTPUTS:
*   *mode_ptr              - FC800_PMG index configured mode
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void fc800_pmg_mode_get(fc800_pmg_handle_t    *fc800_pmg_handle_ptr,
                               UINT32                 index,
                               fc800_pmg_mode_t      *mode_ptr)
{

    PMC_ENTRY();
        
    PMC_ASSERT(fc800_pmg_handle_ptr !=NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);
   
    *mode_ptr =  LAST_FC800_PMG_MODE;

    /* interpolate mode regarding register space content */
    switch (fc_cbr_field_MODE_get(NULL, fc800_pmg_handle_ptr, index))
    {
    case FC800_PMG_FC_CBR_MODE_FC:
        *mode_ptr =  FC800_PMG_MODE_10G_FC800;
        break;
    case FC800_PMG_FC_CBR_MODE_ESCON:
        switch (fc_cbr_field_RX_DLOLB_CLK_DIV_get(NULL, fc800_pmg_handle_ptr, index))
        {
        case FC800_PMG_FC_CBR_CLK_DIV_CPRI_5G:
            *mode_ptr = FC800_PMG_MODE_10G_CPRI_5G;
            break;
        case FC800_PMG_FC_CBR_CLK_DIV_CPRI_6G:
            *mode_ptr =  FC800_PMG_MODE_10G_CPRI_6G;
            break;
        case FC800_PMG_FC_CBR_CLK_DIV_CPRI_10G:
            *mode_ptr = FC800_PMG_MODE_10G_CPRI_10G;
            break;
        case FC800_PMG_FC_CBR_CLK_DIV_GDPS_5G:
            *mode_ptr = FC800_PMG_MODE_10G_GDPS_5G;
            break;
        case FC800_PMG_FC_CBR_CLK_DIV_GDPS_10G:
            *mode_ptr = FC800_PMG_MODE_10G_GDPS_10G;
            break;
        default:
            break;
        }
    default:
        break;
    } /* end switch */

    PMC_RETURN();
} /* fc800_pmg_mode_get */



/*******************************************************************************
* FUNCTION: fc800_pmg_reset()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Puts an index in the FC800_PMG block into and out of reset
*
* INPUTS:
*   fc800_pmg_handle_ptr   - Pointer to subsystem handle
*   index                  - Index within FC800_PMG (0..11)
*   reset                  - FC800_PMG_RESET or FC800_PMG_ACTIVE
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc800_pmg_reset(fc800_pmg_handle_t    *fc800_pmg_handle_ptr,
                                 UINT32                 index,
                                 BOOL                   reset)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    
    PMC_ASSERT(fc800_pmg_handle_ptr !=NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC800_PMG_MAX_INDEX, FC800_PMG_ERR_INVALID_ARG, index, FC800_PMG_MAX_INDEX);

    fc_cbr_field_SOFT_RST_set(NULL, fc800_pmg_handle_ptr, index, reset);
    fc_cbr_field_RX_SOFT_RST_set(NULL, fc800_pmg_handle_ptr, index, reset);


    PMC_RETURN(result);
} /* fc800_pmg_reset */


/*******************************************************************************
* FUNCTION: fc800_pmg_status_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns link FC800 PMG status: ACTIVE or RESET
*
* INPUTS:
*   fc800_pmg_handle_ptr   - Pointer to subsystem handle
*   index                  - Index within FC800_PMG (0..11)
*   
* OUTPUTS:
*   NONE.                   
*
* RETURNS:
*   ACTIVE(TRUE) or RESET(FALSE)
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 fc800_pmg_status_get(fc800_pmg_handle_t    *fc800_pmg_handle_ptr,
                                  UINT32                 index)
{
    BOOL8 rc  = FALSE;

    PMC_ENTRY();
    
    PMC_ASSERT(fc800_pmg_handle_ptr !=NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC800_PMG_MAX_INDEX, FC800_PMG_ERR_INVALID_ARG, index, FC800_PMG_MAX_INDEX);

    if (0 == fc_cbr_field_SOFT_RST_get(NULL, fc800_pmg_handle_ptr, index) &&
        0 == fc_cbr_field_RX_SOFT_RST_get(NULL, fc800_pmg_handle_ptr, index))
    {
        rc = TRUE;
    }


    PMC_RETURN(rc);
} /* fc800_pmg_status_get */

/*******************************************************************************
* FUNCTION: fc800_pmg_user_pattern_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the user defined pattern to be inserted into the transmit path.
*
* INPUTS:
*   fc800_pmg_handle     - Pointer to fc800_pmg handle
*   index                - Index within FC800_PMG (0..11)
*   user_pattern         - 32-bit user pattern to be inserted
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc800_pmg_user_pattern_set(fc800_pmg_handle_t    *fc800_pmg_handle,
                                            UINT32                 index,
                                            UINT32                 user_pattern)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(fc800_pmg_handle !=NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC800_PMG_MAX_INDEX, FC800_PMG_ERR_INVALID_ARG, index, FC800_PMG_MAX_INDEX);

    fc_cbr_reg_TX_USER_PATTERN_LSB_write(NULL, fc800_pmg_handle, index, user_pattern);
    fc_cbr_field_TX_USR_PATT_UPDATE_set(NULL, fc800_pmg_handle, index, FC800_PMG_ENABLE);

    PMC_RETURN(result);
} /* fc800_pmg_user_pattern_set */


/*******************************************************************************
* FUNCTION: fc800_pmg_user_pattern_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the user defined pattern to be inserted into the transmit path. 
*   
*
* INPUTS:
*   fc800_pmg_handle     - Pointer to fc800_pmg handle
*   index                - Index within FC800_PMG (0..11)
*
* OUTPUTS:
*   user_pattern_ptr     - configured 32-bit user pattern to be inserted
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc800_pmg_user_pattern_get(fc800_pmg_handle_t    *fc800_pmg_handle,
                                            UINT32                 index,
                                            UINT32                *user_pattern_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(fc800_pmg_handle !=NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC800_PMG_MAX_INDEX, FC800_PMG_ERR_INVALID_ARG, index, FC800_PMG_MAX_INDEX);
    PMC_ASSERT(user_pattern_ptr !=NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);

    *user_pattern_ptr = fc_cbr_reg_TX_USER_PATTERN_LSB_read(NULL, fc800_pmg_handle, index);



    PMC_RETURN(result);
} /* fc800_pmg_user_pattern_get */



/*******************************************************************************
* FUNCTION: fc800_pmg_pattern_ins_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the pattern insertion during DLOLB or fc800_pmg_pattern_force() 
*
* INPUTS:
*   fc800_pmg_handle    - Pointer to fc800_pmg handle
*   index               - Index within FC800_PMG (0..11)
*   pattern_ins_cfg     - Source of pattern to insert (user or NOS/PN11)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc800_pmg_pattern_ins_set(fc800_pmg_handle_t           *fc800_pmg_handle,
                                           UINT32                        index,
                                           fc800_pmg_pattern_ins_cfg_t   pattern_ins_cfg)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(fc800_pmg_handle !=NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC800_PMG_MAX_INDEX, FC800_PMG_ERR_INVALID_ARG, index, FC800_PMG_MAX_INDEX);
    PMC_ASSERT(pattern_ins_cfg < LAST_FC800_PMG_PATTERN_INS_CFG, FC800_PMG_ERR_INVALID_ARG, 0, 0);

    
    fc_cbr_field_TX_NOS_PN11_SEL_set(NULL, fc800_pmg_handle, index, (fc800_pmg_pattern_ins_cfg_t) pattern_ins_cfg);

    PMC_RETURN(result);
} /* fc800_pmg_pattern_ins_set */


/*******************************************************************************
* FUNCTION: fc800_pmg_pattern_ins_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the configured the pattern insertion
*
* INPUTS:
*   fc800_pmg_handle    - Pointer to fc800_pmg handle
*   index               - Index within FC800_PMG (0..11)
*
* OUTPUTS:
*   pattern_ins_cfg_ptr - Source of pattern to insert (user or NOS/PN11)
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc800_pmg_pattern_ins_get(fc800_pmg_handle_t           *fc800_pmg_handle,
                                           UINT32                        index,
                                           fc800_pmg_pattern_ins_cfg_t  *pattern_ins_cfg_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(fc800_pmg_handle !=NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC800_PMG_MAX_INDEX, FC800_PMG_ERR_INVALID_ARG, index, FC800_PMG_MAX_INDEX);
    PMC_ASSERT(pattern_ins_cfg_ptr !=NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);

    
    *pattern_ins_cfg_ptr = (fc800_pmg_pattern_ins_cfg_t)
                                fc_cbr_field_TX_NOS_PN11_SEL_get(NULL, fc800_pmg_handle, index);

    PMC_RETURN(result);
} /* fc800_pmg_pattern_ins_get */


/*******************************************************************************
* FUNCTION: fc800_pmg_dlolb_gen_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures what defects contribute to DLOLB generation.
*
* INPUTS:
*   fc800_pmg_handle    - Pointer to fc800_pmg handle
*   index               - Index within FC800_PMG (0..11)
*   dlolb_gen_cfg_ptr   - DLOLB generation configuration
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc800_pmg_dlolb_gen_set(fc800_pmg_handle_t           *fc800_pmg_handle,
                                         UINT32                        index,
                                         fc800_pmg_dlolb_gen_t        *dlolb_gen_cfg_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(fc800_pmg_handle !=NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC800_PMG_MAX_INDEX, FC800_PMG_ERR_INVALID_ARG, index, FC800_PMG_MAX_INDEX);
    PMC_ASSERT(dlolb_gen_cfg_ptr != NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);

    if (dlolb_gen_cfg_ptr->mask & FC800_PMG_DLOLB_GEN_MASK_LCV)
    {
        fc_cbr_field_RX_LCV_DLOLB_EN_set(NULL, fc800_pmg_handle, index, dlolb_gen_cfg_ptr->lcv);
    }

    if (dlolb_gen_cfg_ptr->mask & FC800_PMG_DLOLB_GEN_MASK_XDET)
    {
        fc_cbr_field_RX_XDET_DLOLB_EN_set(NULL, fc800_pmg_handle, index, dlolb_gen_cfg_ptr->xdet);
    }

    if (dlolb_gen_cfg_ptr->mask & FC800_PMG_DLOLB_GEN_MASK_ASD)
    {
        fc_cbr_field_ASD_DLOLB_EN_set(NULL, fc800_pmg_handle, index, dlolb_gen_cfg_ptr->asd);
    }

    if (dlolb_gen_cfg_ptr->mask & FC800_PMG_DLOLB_GEN_MASK_DENSITY)
    {
        fc_cbr_field_RX_DRL_DLOLB_EN_set(NULL, fc800_pmg_handle, index, dlolb_gen_cfg_ptr->density);
    }

    if (dlolb_gen_cfg_ptr->mask & FC800_PMG_DLOLB_GEN_MASK_LOSS_SYNC)
    {
        fc_cbr_field_RX_LOSS_SYNC_DLOLB_EN_set(NULL, fc800_pmg_handle, index, dlolb_gen_cfg_ptr->loss_sync);
    }


    PMC_RETURN(result);
} /* fc800_pmg_dlolb_gen_set */


/*******************************************************************************
* FUNCTION: fc800_pmg_dlolb_gen_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets what defects contribute to DLOLB generation.
*
* INPUTS:
*   fc800_pmg_handle    - Pointer to fc800_pmg handle
*   index               - Index within FC800_PMG (0..11)
*
* OUTPUTS:
*   dlolb_gen_cfg_ptr   - DLOLB generation configuration
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc800_pmg_dlolb_gen_get(fc800_pmg_handle_t           *fc800_pmg_handle,
                                         UINT32                        index,
                                         fc800_pmg_dlolb_gen_t        *dlolb_gen_cfg_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(fc800_pmg_handle !=NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC800_PMG_MAX_INDEX, FC800_PMG_ERR_INVALID_ARG, index, FC800_PMG_MAX_INDEX);
    PMC_ASSERT(dlolb_gen_cfg_ptr != NULL, FC800_PMG_ERR_NULL_HANDLE, 0, 0);
    
    
    
    dlolb_gen_cfg_ptr->mask = 0;

    dlolb_gen_cfg_ptr->mask |= FC800_PMG_DLOLB_GEN_MASK_LCV;
    dlolb_gen_cfg_ptr->lcv = fc_cbr_field_RX_LCV_DLOLB_EN_get(NULL, fc800_pmg_handle, index);

    dlolb_gen_cfg_ptr->mask |= FC800_PMG_DLOLB_GEN_MASK_XDET;
    dlolb_gen_cfg_ptr->xdet = fc_cbr_field_RX_XDET_DLOLB_EN_get(NULL, fc800_pmg_handle, index);

    dlolb_gen_cfg_ptr->mask |= FC800_PMG_DLOLB_GEN_MASK_ASD;
    dlolb_gen_cfg_ptr->asd = fc_cbr_field_ASD_DLOLB_EN_get(NULL, fc800_pmg_handle, index);

    dlolb_gen_cfg_ptr->mask |= FC800_PMG_DLOLB_GEN_MASK_DENSITY;
    dlolb_gen_cfg_ptr->density = fc_cbr_field_RX_DRL_DLOLB_EN_get(NULL, fc800_pmg_handle, index);

    dlolb_gen_cfg_ptr->mask |= FC800_PMG_DLOLB_GEN_MASK_LOSS_SYNC;
    dlolb_gen_cfg_ptr->loss_sync = fc_cbr_field_RX_LOSS_SYNC_DLOLB_EN_get(NULL, fc800_pmg_handle, index);
    

    PMC_RETURN(result);
} /* fc800_pmg_dlolb_gen_get */




/*******************************************************************************
* FUNCTION: fc800_pmg_pattern_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forces the configured pattern into the datastream.
*
*   The pattern source (user pattern or NOS/PN11) can be configured with 
*   fc800_pmg_pattern_ins_set().
*
*   The user pattern can be configured with fc800_pmg_user_pattern_set().
*
* INPUTS:
*   fc800_pmg_handle     - Pointer to fc800_pmg handle
*   index                - Index within FC800_PMG (0..11)
*   force                - TRUE to force the pattern. 
*                        
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc800_pmg_pattern_force(fc800_pmg_handle_t    *fc800_pmg_handle,
                                         UINT32                 index,
                                         BOOL                   force)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    fc_cbr_field_TX_FORCE_NOS_PN11_set(NULL, fc800_pmg_handle, index, force);
    

    PMC_RETURN(result);
} /* fc800_pmg_pattern_force */

/*******************************************************************************
* FUNCTION: fc800_pmg_pattern_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if the configured pattern is forced into the datastream.
*
* INPUTS:
*   fc800_pmg_handle     - Pointer to fc800_pmg handle
*   index                - Index within FC800_PMG (0..11)
*                        
* OUTPUTS:
*   force_ptr            - TRUE if pattern is forced into datastream.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc800_pmg_pattern_force_get(fc800_pmg_handle_t    *fc800_pmg_handle,
                                             UINT32                 index,
                                             BOOL                  *force_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    *force_ptr = fc_cbr_field_TX_FORCE_NOS_PN11_get(NULL, fc800_pmg_handle, index);
    

    PMC_RETURN(result);
} /* fc800_pmg_pattern_force_get */




/*******************************************************************************
* FUNCTION: fc800_pmg_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   fc800_pmg_handle       - Pointer to subsystem handle
*   index                  - Index within FC800_PMG (0..11)
*   non_null_ptr           - Pointer to be checked that can't be NULL
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc800_pmg_int_validate(fc800_pmg_handle_t    *fc800_pmg_handle,
                                        UINT32                 index,
                                        void                  *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != fc800_pmg_handle, FC800_PMG_ERR_NULL_HANDLE, 0, 0);
   
    if (index >= FC800_PMG_MAX_INDEX)
    {
        result = FC800_PMG_ERR_INVALID_ARG;
    }

    if (NULL == non_null_ptr)
    {
        result = FC800_PMG_ERR_NULL_HANDLE;
    }


    PMC_RETURN(result);
} /* fc800_pmg_int_validate */


/*******************************************************************************
* FUNCTION: fc800_pmg_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   fc800_pmg_handle  - Pointer to subsystem handle
*   index             - Index within FC800_PMG (0..11)
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
PUBLIC PMC_ERROR fc800_pmg_int_chnl_enable(fc800_pmg_handle_t    *fc800_pmg_handle,
                                           UINT32                 index,
                                           fc800_pmg_int_chnl_t  *int_table_ptr,
                                           fc800_pmg_int_chnl_t  *int_en_table_ptr,
                                           BOOL                   enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link   = index;

    PMC_ENTRY();

    result = fc800_pmg_int_validate(fc800_pmg_handle, index, int_en_table_ptr);

    #ifdef FC800_PMG_INT
    #undef FC800_PMG_INT
    #endif
    #define FC800_PMG_INT    UTIL_GLOBAL_INT_ENABLE
    
    FC800_PMG_INT_TABLE_DEFINE();

    PMC_RETURN(result);
} /* fc800_pmg_int_chnl_enable */




/*******************************************************************************
* FUNCTION: fc800_pmg_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   fc800_pmg_handle  - Pointer to subsystem handle
*   index             - Index within FC800_PMG (0..11)
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
PUBLIC PMC_ERROR fc800_pmg_int_chnl_clear(fc800_pmg_handle_t    *fc800_pmg_handle,
                                          UINT32                 index,
                                          fc800_pmg_int_chnl_t  *int_table_ptr,
                                          fc800_pmg_int_chnl_t  *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link   = index;

    PMC_ENTRY();

    result = fc800_pmg_int_validate(fc800_pmg_handle, index, int_en_table_ptr);

    #ifdef FC800_PMG_INT
    #undef FC800_PMG_INT
    #endif
    #define FC800_PMG_INT UTIL_GLOBAL_INT_CLEAR
    

    FC800_PMG_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* fc800_pmg_int_chnl_clear */




/*******************************************************************************
* FUNCTION: fc800_pmg_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   fc800_pmg_handle  - Pointer to subsystem handle
*   index             - Index within FC800_PMG (0..11)
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
PUBLIC PMC_ERROR fc800_pmg_int_chnl_retrieve(fc800_pmg_handle_t    *fc800_pmg_handle,
                                             UINT32                 index,
                                             fc800_pmg_int_chnl_t  *filt_table_ptr,
                                             fc800_pmg_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link   = index;

    PMC_ENTRY();

    result = fc800_pmg_int_validate(fc800_pmg_handle, index, int_table_ptr);

    
    UTIL_GLOBAL_INT_BUFFER_INIT(fc800_pmg_handle, fc_cbr, UTIL_GLOBAL_INT_OPTIMIZE);

    #ifdef FC800_PMG_INT
    #undef FC800_PMG_INT
    #endif
    #define FC800_PMG_INT UTIL_GLOBAL_INT_RETRIEVE
    

    FC800_PMG_INT_TABLE_DEFINE();
    
    
    #ifdef FC800_PMG_STATUS
    #undef FC800_PMG_STATUS
    #endif
    #define FC800_PMG_STATUS UTIL_GLOBAL_STATUS_RETRIEVE
    

    FC800_PMG_STATUS_TABLE_DEFINE();
    
    UTIL_GLOBAL_INT_BUFFER_FLUSH(fc800_pmg_handle, fc_cbr);

    PMC_RETURN(result);
} /* fc800_pmg_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: fc800_pmg_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   fc800_pmg_handle  - Pointer to subsystem handle
*   index             - Index within FC800_PMG (0..11)
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
PUBLIC PMC_ERROR fc800_pmg_int_chnl_enabled_check(fc800_pmg_handle_t     *fc800_pmg_handle,
                                                  UINT32                 index,
                                                  fc800_pmg_int_chnl_t   *int_en_table_ptr,
                                                  BOOL                   *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link   = index;

    PMC_ENTRY();

    result = fc800_pmg_int_validate(fc800_pmg_handle, index, int_found_ptr);


    #ifdef FC800_PMG_INT
    #undef FC800_PMG_INT
    #endif
    #define FC800_PMG_INT UTIL_GLOBAL_INT_CHECK
    

    FC800_PMG_INT_TABLE_DEFINE();


    PMC_RETURN(result);
} /* fc800_pmg_int_chnl_enabled_check */





/*
** Private Functions
*/


/*
** End of file
*/
