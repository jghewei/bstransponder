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
#include "fc1200_pmon_loc.h"

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
const char FC1200_PMON_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    FC1200_PMON_LOG_ERR_TABLE 
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else  /* no strings */

const char FC1200_PMON_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

/*
** Public Functions
*/

/*******************************************************************************
* fc1200_pmon_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an FC1200_PMON block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the FC1200_PMON subsystem relative to 
*                           the device memory space
*  *sys_handle            - pointer to user system handle
*  *tsb_name              - Name of subsystem
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   fc1200_pmon_handle         - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC fc1200_pmon_handle_t *fc1200_pmon_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    fc1200_pmon_handle_t *fc1200_pmon_handle;

    PMC_ENTRY();

    fc1200_pmon_handle = (fc1200_pmon_handle_t*)PMC_CTXT_CALLOC(sizeof(fc1200_pmon_handle_t), parent);
    
    pmc_handle_init(parent, fc1200_pmon_handle, sys_handle, PMC_MID_DIGI_FC1200_PMON, tsb_name, base_address);

    PMC_RETURN(fc1200_pmon_handle);
} /* fc1200_pmon_ctxt_create */

 
/*******************************************************************************
* fc1200_pmon_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an FC1200_PMON block instance.
*
*
* INPUTS:
*   *fc1200_pmon_handle         - pointer to FC1200_PMON handle instance
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
PUBLIC void fc1200_pmon_ctxt_destroy(fc1200_pmon_handle_t *fc1200_pmon_handle)
{
    PMC_ENTRY();
    pmc_handle_mid_verify(fc1200_pmon_handle, PMC_MID_DIGI_FC1200_PMON);
    fc1200_pmon_handle->base.tsb_mid = PMC_MID_INVALID;
    PMC_CTXT_FREE(&fc1200_pmon_handle, fc1200_pmon_handle);
    PMC_RETURN();
} /* fc1200_pmon_ctxt_destroy */

/*******************************************************************************
* FUNCTION: fc1200_pmon_var_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes dynamic context 
*
* INPUTS:
*   fc1200_pmon_handle_ptr    - Pointer to block handle
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc1200_pmon_var_init(fc1200_pmon_handle_t      *fc1200_pmon_handle_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();



    PMC_RETURN(result);
} /* fc1200_pmon_var_init */



/*******************************************************************************
* fc1200_pmon_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an FC1200_PMON block handle instance.
*
*
* INPUTS:
*   *fc1200_pmon_handle         - pointer to FC1200_PMON handle instance
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
PUBLIC void fc1200_pmon_handle_init(fc1200_pmon_handle_t *fc1200_pmon_handle)
{
    PMC_ENTRY();

    fc1200_pmon_var_init(fc1200_pmon_handle);
     
    PMC_RETURN();
} /* fc1200_pmon_handle_init */





/*******************************************************************************
* fc1200_pmon_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bring FC1200_PMON blocks into configurable state 
*
* INPUTS:
*   fc1200_pmon_handle_ptr   - Pointer to subsystem handle
*   energy_state             - Energy state to put FC1200_PMON into
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc1200_pmon_init(fc1200_pmon_handle_t    *fc1200_pmon_handle_ptr,
                                  pmc_energy_state_t       energy_state)
{
    PMC_ENTRY();
  
    /* Check Arguments */
    PMC_ASSERT(NULL != fc1200_pmon_handle_ptr, FC1200_PMON_ERR_NULL_HANDLE, 0, 0);
    

    switch (energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            break;

        case PMC_ENERGY_STATE_RESET:
            fc1200_pmon_var_init(fc1200_pmon_handle_ptr);
            break;
        
        default:
            PMC_ASSERT(FALSE, FC1200_PMON_ERR_INVALID_ARG, energy_state, 0);
            break;
    } /* end switch */





    PMC_RETURN(PMC_SUCCESS);
} /* fc1200_pmon_init */




/*******************************************************************************
* fc1200_pmon_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   FC1200_PMON instance.
*
*
* INPUTS:
*   *fc1200_pmon_handle      - pointer to fc1200_pmon handle instance to be operated on
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
PUBLIC BOOL fc1200_pmon_start_state_test(fc1200_pmon_handle_t *fc1200_pmon_handle)
{
    BOOL                start_state = TRUE;

    /* Variable declaration */
    
    PMC_ENTRY();
    PMC_ASSERT(fc1200_pmon_handle!=NULL, FC1200_PMON_ERR_NULL_HANDLE, 0, 0);
    
    PMC_RETURN(start_state);
    
} /* fc1200_pmon_start_state_test */











/*******************************************************************************
* FUNCTION: fc1200_pmon_reset()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Puts an index in the FC800_PMG block into and out of reset
*
* INPUTS:
*   fc1200_pmon_handle_ptr - Pointer to subsystem handle
*   index                  - Index within FC1200 PMON (0..11)
*   reset                  - FC1200_PMON_RESET or FC1200_PMON_ACTIVE
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc1200_pmon_reset(fc1200_pmon_handle_t    *fc1200_pmon_handle_ptr,
                                   UINT32                   index,
                                   BOOL                     reset)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    
    PMC_ASSERT(fc1200_pmon_handle_ptr !=NULL, FC1200_PMON_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC1200_PMON_MAX_INDEX, FC1200_PMON_ERR_INVALID_ARG, index, FC1200_PMON_MAX_INDEX);

    fc1200_pmon_field_SW_RESET_set(NULL, fc1200_pmon_handle_ptr, index, reset);

    PMC_RETURN(result);
} /* fc1200_pmon_reset */


/*******************************************************************************
* FUNCTION: fc1200_pmon_dlolb_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures FC1200 PMON DLOLB insertion options.
*   
*
* INPUTS:
*   fc1200_handle_ptr   - Pointer to subsystem handle
*   index               - Index within FC1200_PMON block (0..12)
*  *cfg_ptr             - pointer to configuration structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc1200_pmon_dlolb_cfg(fc1200_pmon_handle_t        *fc1200_handle_ptr,
                                       UINT32                       index,
                                       fc1200_pmon_dlolb_cfg_t     *cfg_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    
    UINT32        mask;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != fc1200_handle_ptr, FC1200_PMON_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC1200_PMON_MAX_INDEX, FC1200_PMON_ERR_INVALID_ARG, index, FC1200_PMON_MAX_INDEX);
    PMC_ASSERT(NULL != cfg_ptr, FC1200_PMON_ERR_NULL_HANDLE, 0, 0);


    mask = cfg_ptr->mask;

    if (mask & FC1200_PMON_DLOLB_CFG_MASK_LOS_ENABLE)
    {
        fc1200_pmon_field_LOS_DLOLB_ENA_set(NULL, fc1200_handle_ptr, index, cfg_ptr->los_enable);
    }
    
    if (mask & FC1200_PMON_DLOLB_CFG_MASK_LOSS_SYNC_ENABLE)
    {
        fc1200_pmon_field_LOSS_SYNC_DLOLB_ENA_set(NULL, fc1200_handle_ptr, index, cfg_ptr->loss_sync_enable);
    }
    
    if (mask & FC1200_PMON_DLOLB_CFG_MASK_HIGH_BER_ENABLE)
    {
        fc1200_pmon_field_HIGH_BER_DLOLB_ENA_set(NULL, fc1200_handle_ptr, index, cfg_ptr->high_ber_enable);
    }
    
    PMC_RETURN(result);
} /* fc1200_pmon_dlolb_cfg */



/*******************************************************************************
* FUNCTION: fc1200_pmon_dlolb_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves FC1200 PMON DLOLB insertion options.
*   
*
* INPUTS:
*   fc1200_handle_ptr   - Pointer to subsystem handle
*   index               - Index within FC1200_PMON block (0..12)
*
* OUTPUTS:
*  *cfg_ptr             - pointer to configuration structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc1200_pmon_dlolb_cfg_get(fc1200_pmon_handle_t        *fc1200_handle_ptr,
                                           UINT32                       index,
                                           fc1200_pmon_dlolb_cfg_t     *cfg_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != fc1200_handle_ptr, FC1200_PMON_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC1200_PMON_MAX_INDEX, FC1200_PMON_ERR_INVALID_ARG, index, FC1200_PMON_MAX_INDEX);
    PMC_ASSERT(NULL != cfg_ptr, FC1200_PMON_ERR_NULL_HANDLE, 0, 0);


    cfg_ptr->mask = 0;

    cfg_ptr->mask |= FC1200_PMON_DLOLB_CFG_MASK_LOS_ENABLE;
    cfg_ptr->los_enable = fc1200_pmon_field_LOS_DLOLB_ENA_get(NULL, fc1200_handle_ptr, index);
    
    cfg_ptr->mask |= FC1200_PMON_DLOLB_CFG_MASK_LOSS_SYNC_ENABLE;
    cfg_ptr->loss_sync_enable = fc1200_pmon_field_LOSS_SYNC_DLOLB_ENA_get(NULL, fc1200_handle_ptr, index);
    
    cfg_ptr->mask |= FC1200_PMON_DLOLB_CFG_MASK_HIGH_BER_ENABLE;
    cfg_ptr->high_ber_enable = fc1200_pmon_field_HIGH_BER_DLOLB_ENA_get(NULL, fc1200_handle_ptr, index);
    
    PMC_RETURN(result);
} /* fc1200_pmon_dlolb_cfg_get */



/*******************************************************************************
* FUNCTION: fc1200_pmon_dlolb_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Forces DLOLB.
*   
*
* INPUTS:
*   fc1200_handle_ptr   - Pointer to subsystem handle
*   index               - Index within FC1200_PMON block (0..12)
*   enable              - Enable/disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc1200_pmon_dlolb_force(fc1200_pmon_handle_t        *fc1200_handle_ptr,
                                         UINT32                       index,
                                         BOOL                         enable)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != fc1200_handle_ptr, FC1200_PMON_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC1200_PMON_MAX_INDEX, FC1200_PMON_ERR_INVALID_ARG, index, FC1200_PMON_MAX_INDEX);


    fc1200_pmon_field_FORCE_DLOLB_set(NULL, fc1200_handle_ptr, index, enable);
    
    PMC_RETURN(result);
} /* fc1200_pmon_dlolb_force */



/*******************************************************************************
* FUNCTION: fc1200_pmon_dlolb_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Retrieves if DLOLB is forced.
*   
*
* INPUTS:
*   fc1200_handle_ptr   - Pointer to subsystem handle
*   index               - Index within FC1200_PMON block (0..12)
*
* OUTPUTS:
*  *enable_ptr          - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc1200_pmon_dlolb_force_get(fc1200_pmon_handle_t        *fc1200_handle_ptr,
                                             UINT32                       index,
                                             BOOL                        *enable_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != fc1200_handle_ptr, FC1200_PMON_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC1200_PMON_MAX_INDEX, FC1200_PMON_ERR_INVALID_ARG, index, FC1200_PMON_MAX_INDEX);
    PMC_ASSERT(NULL != enable_ptr, FC1200_PMON_ERR_NULL_HANDLE, 0, 0);


    *enable_ptr = fc1200_pmon_field_FORCE_DLOLB_get(NULL, fc1200_handle_ptr, index);
    
    PMC_RETURN(result);
} /* fc1200_pmon_dlolb_force_get */


/*******************************************************************************
* FUNCTION: fc1200_pmon_invalid_block_mode_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures how blocks with invalid block type encodings are handled when
*   encountered during processing.
*   
*
* INPUTS:
*   fc1200_handle_ptr   - Pointer to subsystem handle
*   index               - Index within FC1200_PMON block (0..12)
*   mode                - configures behaviour when an invalid block is received
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc1200_pmon_invalid_block_mode_set(fc1200_pmon_handle_t             *fc1200_handle_ptr,
                                                    UINT32                            index,
                                                    fc1200_pmon_invalid_block_mode_t  mode) 
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != fc1200_handle_ptr, FC1200_PMON_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC1200_PMON_MAX_INDEX, FC1200_PMON_ERR_INVALID_ARG, index, FC1200_PMON_MAX_INDEX);

    if (mode >= LAST_FC1200_PMON_INVALID_BLOCK_MODE) 
    {
        PMC_RETURN(FC1200_PMON_ERR_INVALID_ARG);
    }

    fc1200_pmon_field_INVALID_BLK_TYPE_MODE_set(NULL, fc1200_handle_ptr, index, mode);
    
    PMC_RETURN(result);
} /* fc1200_pmon_invalid_block_mode_set */



/*******************************************************************************
* FUNCTION: fc1200_pmon_invalid_block_mode_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves how blocks with invalid block type encodings are handled when
*   encountered during processing.
*   
*
* INPUTS:
*   fc1200_handle_ptr   - Pointer to subsystem handle
*   index               - Index within FC1200_PMON block (0..12)
*
* OUTPUTS:
*  *mode_ptr            - behaviour when an invalid block is received
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc1200_pmon_invalid_block_mode_get(fc1200_pmon_handle_t             *fc1200_handle_ptr,
                                                    UINT32                            index,
                                                    fc1200_pmon_invalid_block_mode_t *mode_ptr) 
{
    PMC_ERROR     result = PMC_SUCCESS;    

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != fc1200_handle_ptr, FC1200_PMON_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < FC1200_PMON_MAX_INDEX, FC1200_PMON_ERR_INVALID_ARG, index, FC1200_PMON_MAX_INDEX);
    PMC_ASSERT(NULL != mode_ptr, FC1200_PMON_ERR_NULL_HANDLE, 0, 0);


    *mode_ptr = (fc1200_pmon_invalid_block_mode_t) fc1200_pmon_field_range_INVALID_BLK_TYPE_MODE_get(NULL, fc1200_handle_ptr, index, 0, 1);
    
    PMC_RETURN(result);
} /* fc1200_pmon_invalid_block_mode_get */


/*******************************************************************************
* FUNCTION: fc1200_pmon_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   fc1200_pmon_handle     - Pointer to subsystem handle
*   index                  - Index within FC1200_PMON (0..11)
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
PUBLIC PMC_ERROR fc1200_pmon_int_validate(fc1200_pmon_handle_t  *fc1200_pmon_handle,
                                          UINT32                 index,
                                          void                  *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != fc1200_pmon_handle, FC1200_PMON_ERR_NULL_HANDLE, 0, 0);
   
    if (index >= FC1200_PMON_MAX_INDEX)
    {
        result = FC1200_PMON_ERR_INVALID_ARG;
    }

    if (NULL == non_null_ptr)
    {
        result = FC1200_PMON_ERR_NULL_HANDLE;
    }


    PMC_RETURN(result);
} /* fc1200_pmon_int_validate */


/*******************************************************************************
* FUNCTION: fc1200_pmon_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   fc1200_pmon_handle- Pointer to subsystem handle
*   index             - Index within FC1200_PMON (0..11)
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
PUBLIC PMC_ERROR fc1200_pmon_int_chnl_enable(fc1200_pmon_handle_t    *fc1200_pmon_handle,
                                             UINT32                   index,
                                             fc1200_pmon_int_chnl_t  *int_table_ptr,
                                             fc1200_pmon_int_chnl_t  *int_en_table_ptr,
                                             BOOL                     enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32 link = index;

    PMC_ENTRY();

    result = fc1200_pmon_int_validate(fc1200_pmon_handle, index, int_en_table_ptr);

    #ifdef FC1200_PMON_INT
    #undef FC1200_PMON_INT
    #endif
    #define FC1200_PMON_INT    UTIL_GLOBAL_INT_ENABLE
    
    FC1200_PMON_INT_TABLE_DEFINE_FW();
   
    PMC_RETURN(result);
} /* fc1200_pmon_int_chnl_enable */




/*******************************************************************************
* FUNCTION: fc1200_pmon_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   fc1200_pmon_handle  - Pointer to subsystem handle
*   index               - Index within FC1200_PMON (0..11)
*   int_table_ptr       - pointer to channel interrupt table with
*                         bits set to clear interrupts
*   int_en_table_ptr    - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc1200_pmon_int_chnl_clear(fc1200_pmon_handle_t    *fc1200_pmon_handle,
                                            UINT32                   index,
                                            fc1200_pmon_int_chnl_t  *int_table_ptr,
                                            fc1200_pmon_int_chnl_t  *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link   = index;

    PMC_ENTRY();

    result = fc1200_pmon_int_validate(fc1200_pmon_handle, index, int_en_table_ptr);

    #ifdef FC1200_PMON_INT
    #undef FC1200_PMON_INT
    #endif
    #define FC1200_PMON_INT UTIL_GLOBAL_INT_CLEAR
    

#ifdef FC1200_PMON_INT_WO_FW_PROXY
    FC1200_PMON_INT_TABLE_DEFINE_FW();
#endif
   
    PMC_RETURN(result);
} /* fc1200_pmon_int_chnl_clear */




/*******************************************************************************
* FUNCTION: fc1200_pmon_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   fc1200_pmon_handle  - Pointer to subsystem handle
*   index               - Index within FC1200_PMON (0..11)
*   filt_table_ptr      - pointer to filter table. Mark fields as TRUE within this
*                         table to indicate that the interrupt/status will be retrieved
*                         to the output table. NULL to retrieve all 
*                         interrupt/status.
*
* OUTPUTS:
*   int_table_ptr       - pointer to channel interrupt table to fill with 
*                         retrieved data
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc1200_pmon_int_chnl_retrieve(fc1200_pmon_handle_t    *fc1200_pmon_handle,
                                               UINT32                   index,
                                               fc1200_pmon_int_chnl_t  *filt_table_ptr,
                                               fc1200_pmon_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link   = index;

    PMC_ENTRY();

    result = fc1200_pmon_int_validate(fc1200_pmon_handle, index, int_table_ptr);

    UTIL_GLOBAL_INT_BUFFER_INIT(fc1200_pmon_handle, fc1200_pmon, UTIL_GLOBAL_INT_OPTIMIZE);
    
    #ifdef FC1200_PMON_INT
    #undef FC1200_PMON_INT
    #endif
    #define FC1200_PMON_INT UTIL_GLOBAL_INT_RETRIEVE
    

#ifdef FC1200_PMON_INT_WO_FW_PROXY
    FC1200_PMON_INT_TABLE_DEFINE_FW();
#endif
   
    #ifdef FC1200_PMON_STATUS
    #undef FC1200_PMON_STATUS
    #endif
    #define FC1200_PMON_STATUS UTIL_GLOBAL_STATUS_RETRIEVE
    

    FC1200_PMON_STATUS_TABLE_DEFINE();
    
    UTIL_GLOBAL_INT_BUFFER_FLUSH(fc1200_pmon_handle, fc1200_pmon);
    
    PMC_RETURN(result);
} /* fc1200_pmon_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: fc1200_pmon_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   fc1200_pmon_handle  - Pointer to subsystem handle
*   index               - Index within FC1200_PMON (0..11)
*   int_en_table_ptr    - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*   int_found_ptr       - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR fc1200_pmon_int_chnl_enabled_check(fc1200_pmon_handle_t     *fc1200_pmon_handle,
                                                    UINT32                    index,
                                                    fc1200_pmon_int_chnl_t   *int_en_table_ptr,
                                                    BOOL                     *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link   = index;

    PMC_ENTRY();

    result = fc1200_pmon_int_validate(fc1200_pmon_handle, index, int_found_ptr);


    #ifdef FC1200_PMON_INT
    #undef FC1200_PMON_INT
    #endif
    #define FC1200_PMON_INT UTIL_GLOBAL_INT_CHECK
    

#ifdef FC1200_PMON_INT_WO_FW_PROXY
    FC1200_PMON_INT_TABLE_DEFINE_FW();
#endif
   
    PMC_RETURN(result);
} /* fc1200_pmon_int_chnl_enabled_check */


/*
** Private Functions
*/


/*
** End of file
*/
