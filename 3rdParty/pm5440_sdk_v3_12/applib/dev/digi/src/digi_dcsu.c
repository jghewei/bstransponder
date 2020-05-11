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
*     This file contains functions for DIGI CBR run time configuration.
*
*   NOTES:
*
*******************************************************************************/

/*
** Include Files
*/
#include "digi_loc.h"
#include "dcsu_io.h"
#include "tl_ic_io.h"
/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/
/* Bit values used to set HW to low/high */
#define SET_TO_HIGH     (1)
#define SET_TO_LOW      (0)
/*
** Local Structures and Unions
*/

/*
** Local Variables
*/
/* PMC_Q_USLEEP(h,time_in_microseconds) */

/*
** Private Functions
*/
#ifndef DOXYGEN_PUBLIC_ONLY
PRIVATE digi_dcsu_num_t digi_dcsu_inst_to_num_convert(digi_dcsu_inst_t dcsu_inst);

PRIVATE PMC_ERROR digi_dcsu_ctxt_audit(digi_handle_t   *digi_handle,
                                       BOOL8            *clean_state_ptr);

PRIVATE PMC_ERROR digi_dcsu_ctxt_cleanup(digi_handle_t   *digi_handle,
                                         BOOL8            *clean_state_ptr);

PRIVATE BOOL8 dcsu_status_from_reg_space_retrieve(digi_handle_t   *h, 
                                                  digi_dcsu_inst_t dcsu_inst);
#endif /* DOXYGEN_PUBLIC_ONLY */


/*
** Public Functions
*/

/*******************************************************************************
* digi_dcsu_diag_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function allows initialization of all DCSU instances for 
*   diagnostic purpose.
*   It shall be called after digi_device_init to enable the 
*   DCSU diagnostic mode. It is recommanded to disable this mode 
*   before  before provisionning datapaths. In normal operational mode, DCSU 
*   instances are initialized based on datapaths etablished in the device. 
*   
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  enable             - TRUE: enable DCSU diagnostic mode (all DCSU \n
*                       instances are enabled)
*                       FALSE: restore DCSU instances configuration
*
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS when operation is executed successfully 
*   or a descriptive error code will be returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_dcsu_diag_init(digi_handle_t *digi_handle, BOOL8 enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ATOMIC_ENTRY(digi_handle);

    if (TRUE == enable)
    {
        digi_dcsu_conditional_init(digi_handle, DIGI_DCSU_INST_SYS2);
        
        digi_dcsu_conditional_init(digi_handle, DIGI_DCSU_INST_OTN1);
        digi_dcsu_conditional_init(digi_handle, DIGI_DCSU_INST_OTN2);
        
        digi_dcsu_conditional_init(digi_handle, DIGI_DCSU_INST_ENET1);
        digi_dcsu_conditional_init(digi_handle, DIGI_DCSU_INST_ENET2);
        digi_dcsu_conditional_init(digi_handle, DIGI_DCSU_INST_ENET3);
        digi_dcsu_conditional_init(digi_handle, DIGI_DCSU_INST_PTP);
        
    } else 
    {
        digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_OTN2);        
        digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_ENET1);
        digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_ENET2);
        digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_PTP);
        if (TRUE == pmc_is_digi_rev_a_revision(&digi_handle->base))
        {   
            digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_SYS2);
        }
        
        if (TRUE == pmc_is_digi_rev_a_revision(&digi_handle->base))
        {
            digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_ENET3);
        }    
    }
    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_dcsu_diag_init */


/*******************************************************************************
* FUNCTION: digi_dcsu_int_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables DCSU block interrupts based on bits that are set 
*   in the interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   dcsu_inst       - Specifies the DCSU block.
*   int_table_ptr   - pointer to interrupt table with bits set to
*                     enable interrupts.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_dcsu_int_enable(digi_handle_t             *digi_handle,
                                      digi_dcsu_inst_t           dcsu_inst,
                                      digi_dcsu_int_t           *int_table_ptr)
{      
    PMC_ERROR                result = PMC_SUCCESS;
    digi_dcsu_int_t         *int_en_table_ptr;
    digi_device_int_t       *device_int_en_table_ptr;
    digi_dcsu_num_t          link;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (LAST_DIGI_DCSU_INST > dcsu_inst)
    {
        /* Callback for interrupt table must be registered before enable ints */   
        if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_DEVICE))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
        
        if (PMC_SUCCESS == result)
        {
            /* Get internal enabled interrupt table to check */
            result = digi_int_en_table_get(digi_handle,
                                           DIGI_INT_TYPE_DEVICE,
                                           (void**) &device_int_en_table_ptr);
        }
        
        /* enable interrupts */
        if (PMC_SUCCESS == result)
        {  
            link = digi_dcsu_inst_to_num_convert(dcsu_inst);
            
            int_en_table_ptr = &device_int_en_table_ptr->dcsu[(UINT32)dcsu_inst];
            
            digi_dcsu_int_cfg(digi_handle,
                              dcsu_inst,
                              int_table_ptr,
                              int_en_table_ptr,
                              DIGI_ENABLE);
        }
    }
    else
    {
        result = DIGI_ERR_INVALID_ARG;
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_dcsu_int_enable */


/*******************************************************************************
* FUNCTION: digi_dcsu_int_disable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables DCSU block interrupts based on bits that are set in the
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   dcsu_inst       - Specifies the DCSU block.
*   int_table_ptr   - pointer to interrupt table with bits set to
*                     enable interrupts.
*
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_dcsu_int_disable(digi_handle_t             *digi_handle,
                                       digi_dcsu_inst_t           dcsu_inst,
                                       digi_dcsu_int_t           *int_table_ptr)
{      
    PMC_ERROR                result = PMC_SUCCESS;
    digi_dcsu_int_t         *int_en_table_ptr;
    digi_device_int_t       *device_int_en_table_ptr;
    digi_dcsu_num_t          link;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (LAST_DIGI_DCSU_INST > dcsu_inst)
    {        
        if (PMC_SUCCESS == result)
        {
            /* Get internal enabled interrupt table to check */
            result = digi_int_en_table_get(digi_handle,
                                           DIGI_INT_TYPE_DEVICE,
                                           (void**) &device_int_en_table_ptr);
        }
        
        /* disable interrupts */
        if (PMC_SUCCESS == result)
        {  
            link = digi_dcsu_inst_to_num_convert(dcsu_inst);
            
            int_en_table_ptr = &device_int_en_table_ptr->dcsu[(UINT32)dcsu_inst];
            
            digi_dcsu_int_cfg(digi_handle,
                              dcsu_inst,
                              int_table_ptr,
                              int_en_table_ptr,
                              DIGI_DISABLE);
        }
    }
    else
    {
        result = DIGI_ERR_INVALID_ARG;
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_dcsu_int_disable */


/*******************************************************************************
* FUNCTION: digi_dcsu_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves DCSU block interrupt table.
*
*   Note that this function just reads out register values and output them, 
*   without checking the target DCSU status.
*   Generally, the caller should ensure the target DCSU is active before using
*   the retrieved values.
*
*   The desired interrupt/status bits to retrieve (to reduce function execution
*   time) can be specified using the filter interrupt table input.
*
* INPUTS:
*   digi_handle        - pointer to DIGI handle instance.
*   dcsu_inst          - Specifies the DCSU block.
*
* OUTPUTS:
*   int_table_ptr  - pointer to interrupt table to store interrupt
*                    information. 
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
t*******************************************************************************/
PUBLIC PMC_ERROR digi_dcsu_int_retrieve(digi_handle_t              *digi_handle,
                                        digi_dcsu_inst_t            dcsu_inst,
                                        digi_dcsu_int_t            *int_table_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;
    digi_dcsu_int_t         *filt_table_ptr = NULL;
    digi_dcsu_num_t          link;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (LAST_DIGI_DCSU_INST > dcsu_inst)
    {

        if (PMC_SUCCESS == result)
        {  
            /* modifiy the OTN1 clock frequency to match a nominal OTU4 clock frequency */
            link = digi_dcsu_inst_to_num_convert(dcsu_inst);
        
            UTIL_GLOBAL_INT_BUFFER_INIT(digi_handle, dcsu, UTIL_GLOBAL_INT_OPTIMIZE);
        
#ifdef DIGI_DCSU_INT
#undef DIGI_DCSU_INT
#endif
#define DIGI_DCSU_INT  UTIL_GLOBAL_INT_RETRIEVE

            DIGI_DCSU_INT_TABLE_DEFINE();  

#ifdef DIGI_DCSU_STATUS
#undef DIGI_DCSU_STATUS
#endif
#define DIGI_DCSU_STATUS  UTIL_GLOBAL_STATUS_RETRIEVE

            DIGI_DCSU_STATUS_TABLE_DEFINE();  

            UTIL_GLOBAL_INT_BUFFER_FLUSH(digi_handle, dcsu);          
        }
    }
    else
    {
        result = DIGI_ERR_INVALID_ARG;
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_dcsu_int_retrieve */

/*******************************************************************************
* FUNCTION: digi_dcsu_int_retrieve_act()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves active DCSU block interrupt table.
*   If the target DCSU dcsu_inst is not activated yet, DIGI_ERR_NOT_PROVISONED 
*   is returned, and no outputs.
*
*   The desired interrupt/status bits to retrieve (to reduce function execution
*   time) can be specified using the filter interrupt table input.
*
* INPUTS:
*   digi_handle        - pointer to DIGI handle instance.
*   dcsu_inst          - Specifies the DCSU block.
*
* OUTPUTS:
*   int_table_ptr  - pointer to interrupt table to store interrupt
*                    information. 
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*   DIGI_ERR_NOT_PROVISONED - DCSU dcsu_inst is inactive.
*
* NOTES:
*
t*******************************************************************************/
PUBLIC PMC_ERROR digi_dcsu_int_retrieve_act(digi_handle_t              *digi_handle,
                                        digi_dcsu_inst_t            dcsu_inst,
                                        digi_dcsu_int_t            *int_table_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;
    digi_dcsu_int_t         *filt_table_ptr = NULL;
    digi_dcsu_num_t          link;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (LAST_DIGI_DCSU_INST > dcsu_inst)
    {

        if (FALSE == digi_handle->var.dcsu_init_state[dcsu_inst])
        {
            result = DIGI_ERR_NOT_PROVISONED;
        }

        if (PMC_SUCCESS == result)
        {  
            /* modifiy the OTN1 clock frequency to match a nominal OTU4 clock frequency */
            link = digi_dcsu_inst_to_num_convert(dcsu_inst);
        
            UTIL_GLOBAL_INT_BUFFER_INIT(digi_handle, dcsu, UTIL_GLOBAL_INT_OPTIMIZE);
        
#ifdef DIGI_DCSU_INT
#undef DIGI_DCSU_INT
#endif
#define DIGI_DCSU_INT  UTIL_GLOBAL_INT_RETRIEVE

            DIGI_DCSU_INT_TABLE_DEFINE();  

#ifdef DIGI_DCSU_STATUS
#undef DIGI_DCSU_STATUS
#endif
#define DIGI_DCSU_STATUS  UTIL_GLOBAL_STATUS_RETRIEVE

            DIGI_DCSU_STATUS_TABLE_DEFINE();  

            UTIL_GLOBAL_INT_BUFFER_FLUSH(digi_handle, dcsu);          
        }
    }
    else
    {
        result = DIGI_ERR_INVALID_ARG;
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_dcsu_int_retrieve_act */

/*******************************************************************************
* FUNCTION: digi_dcsu_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears DCSU interrupts based on bits that are set in the given
*   interrupt table.
*
* INPUTS:
*   digi_handle        - pointer to DIGI handle instance.       
*   dcsu_inst          - Specifies which DCSU block to reset (see dcsu_inst_t)
*   int_table_ptr      - pointer to interrupt table with bits to clear
*
* OUTPUTS:
*  NONE.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_dcsu_int_clear(digi_handle_t       *digi_handle,
                                     digi_dcsu_inst_t     dcsu_inst,
                                     digi_dcsu_int_t     *int_table_ptr)
{      
    PMC_ERROR                result = PMC_SUCCESS;
    digi_dcsu_int_t         *int_en_table_ptr;
    digi_device_int_t       *device_int_en_table_ptr;
    digi_dcsu_num_t          link;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (LAST_DIGI_DCSU_INST > dcsu_inst)
    {
        if (PMC_SUCCESS == result)
        {
            /* Get internal enabled interrupt table to check */
            result = digi_int_en_table_get(digi_handle,
                                           DIGI_INT_TYPE_DEVICE,
                                           (void**) &device_int_en_table_ptr);
        }

        if (PMC_SUCCESS == result)
        {  

            /* modifiy the OTN1 clock frequency to match a nominal OTU4 clock frequency */
            link = digi_dcsu_inst_to_num_convert(dcsu_inst);

            int_en_table_ptr = &device_int_en_table_ptr->dcsu[(UINT32)dcsu_inst];        
        
#ifdef DIGI_DCSU_INT
#undef DIGI_DCSU_INT
#endif
#define DIGI_DCSU_INT  UTIL_GLOBAL_INT_CLEAR

            DIGI_DCSU_INT_TABLE_DEFINE();  

        }
    }
    else
    {
        result = DIGI_ERR_INVALID_ARG;
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_dcsu_int_clear */

/*******************************************************************************
* FUNCTION: digi_dcsu_int_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches DCSU active interrupts.
*
* INPUTS:
*   digi_handle        - pointer to DIGI handle instance.
*   dcsu_inst          - Specifies the DCSU block.
*
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
t*******************************************************************************/
PUBLIC PMC_ERROR digi_dcsu_int_enabled_check(digi_handle_t              *digi_handle,
                                             digi_dcsu_inst_t            dcsu_inst,
                                             BOOL                       *int_found_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;
    digi_dcsu_int_t         *int_en_table_ptr;
    digi_device_int_t       *device_int_en_table_ptr;
    digi_dcsu_num_t          link;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    if (LAST_DIGI_DCSU_INST > dcsu_inst)
    {
        /* Callback for interrupt table must be registered before enable ints */   
        if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_DEVICE))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
    
        if (PMC_SUCCESS == result)
        {
            /* Get internal enabled interrupt table to check */
            result = digi_int_en_table_get(digi_handle,
                                           DIGI_INT_TYPE_DEVICE,
                                           (void**) &device_int_en_table_ptr);
        }

        if (PMC_SUCCESS == result)
        {  

            /* modifiy the OTN1 clock frequency to match a nominal OTU4 clock frequency */
            link = digi_dcsu_inst_to_num_convert(dcsu_inst);

            int_en_table_ptr = &device_int_en_table_ptr->dcsu[(UINT32)dcsu_inst];
        
#ifdef DIGI_DCSU_INT
#undef DIGI_DCSU_INT
#endif
#define DIGI_DCSU_INT  UTIL_GLOBAL_INT_CHECK

            DIGI_DCSU_INT_TABLE_DEFINE();          
        }
    }
    else
    {
        result = DIGI_ERR_INVALID_ARG;
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_dcsu_int_enabled_check */

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */

/*******************************************************************************
* dcsu_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to initialize the DCSU for a specific DCSU module.
*
* INPUTS:
*  *h                 - pointer to DIGI handle instance
*  dcsu_inst          - refers to a DCSU module.  These modules have fixed values
*                       assigned to each term as they are used to address physical
*                       resources.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS when operation is executed successfully 
*   or a descriptive error code will be returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR dcsu_init(digi_handle_t *h, digi_dcsu_inst_t dcsu_inst)
{
    UINT32 boot_mode;
    UINT32 num_failed_polls;
    dcsu_buffer_t b[1];
    digi_dcsu_num_t dcsu_num = digi_dcsu_inst_to_num_convert(dcsu_inst);
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();


    PMC_ASSERT(NULL != h, DIGI_ERR_NULL_HANDLE, 0, 0);   
    PMC_ASSERT((UINT32)dcsu_inst < (UINT32)LAST_DIGI_DCSU, DIGI_ERR_INVALID_ARG, 0, 0);
    
    if (DIGI_DCSU_INST_SYS2 != dcsu_inst)
    {
        /* ensure BOOT_MODE is '0' to allow SW configuration of DCSU */
        boot_mode = dcsu_field_BOOT_MODE_get(NULL, h, dcsu_num);
        PMC_ASSERT(boot_mode==0, DIGI_ERR_CODE_ASSERT, 0, 0);
    }

    /*  Step (1) Release hardware reset (REGS_PRESETN = 1) */       
    /*  Step (2) Configure all DCSU_40N registers except for ARSTB and DIV_RSTB */
    dcsu_buffer_init( b, h, dcsu_num );

    switch(dcsu_inst) 
    {
    case DIGI_DCSU_INST_OTN1:
        tl_ic_field_OTN1_DCSU_IDDQ_set(NULL, h, SET_TO_LOW);
        tl_ic_field_OTN1_DCSU_RESET_set(NULL, h, SET_TO_LOW);
        break;
    case DIGI_DCSU_INST_OTN2:
        tl_ic_field_OTN2_DCSU_IDDQ_set(NULL, h, SET_TO_LOW);
        tl_ic_field_OTN2_DCSU_RESET_set(NULL, h, SET_TO_LOW);
        break;      

    case DIGI_DCSU_INST_ENET3:
        tl_ic_field_ENET3_DCSU_IDDQ_set(NULL, h, SET_TO_LOW);
        tl_ic_field_ENET3_DCSU_RESET_set(NULL, h, SET_TO_LOW);
        break;
    case DIGI_DCSU_INST_ENET1:
        tl_ic_field_ENET1_DCSU_IDDQ_set(NULL, h, SET_TO_LOW);
        tl_ic_field_ENET1_DCSU_RESET_set(NULL, h, SET_TO_LOW);
        break;  
    case DIGI_DCSU_INST_ENET2:
        tl_ic_field_ENET2_DCSU_IDDQ_set(NULL, h, SET_TO_LOW);            
        tl_ic_field_ENET2_DCSU_RESET_set(NULL, h, SET_TO_LOW);
        break;  
    case DIGI_DCSU_INST_PTP:
        tl_ic_field_PTP_DCSU_IDDQ_set(NULL, h, SET_TO_LOW);            
        tl_ic_field_PTP_DCSU_RESET_set(NULL, h, SET_TO_LOW);
        break;  
    case DIGI_DCSU_INST_SYS2:
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, dcsu_inst, 0);
        break;
    }

    if(dcsu_inst != DIGI_DCSU_INST_SYS2)
    {
        dcsu_field_DIV_RSTB_set(NULL, h, dcsu_num, SET_TO_LOW);
        dcsu_field_ARSTB_set(NULL, h, dcsu_num, SET_TO_LOW);
    
        /* apply default configuration for initialization procedure to complete */
        dcsu_field_DCO_CTL_CLKSEL_set(b, h, dcsu_num, 1);
        dcsu_field_DIV2_ENB_set(b, h, dcsu_num, 0);
        dcsu_field_DIV3_ENB_set(b, h, dcsu_num, 0);
        dcsu_field_DIV4_ENB_set(b, h, dcsu_num, 0);
    
        dcsu_field_SHIFT_CLK_SEL_set(b, h, dcsu_num, 0);
        dcsu_field_AC_PULSE_REQ_set(b, h, dcsu_num, 0);
        dcsu_field_CLK_STOP_set(b, h, dcsu_num, 0);
        dcsu_field_CLK_BYPASS_EN_set(b, h, dcsu_num, 0);
    
        dcsu_field_POST_SYNC_EN_set(b, h, dcsu_num, 0);
        dcsu_field_POST_SYNC_CNT_set(b, h, dcsu_num, 0);
    }

    if (TRUE == pmc_is_digi_rev_a_revision(&h->base))
    {
        switch(dcsu_inst) 
        {
        case DIGI_DCSU_INST_OTN1: /* rate 349.92 MHz */
            dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
            dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
            dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 72);
            dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 0);
            dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 1);
            dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 3);
            break;
        case DIGI_DCSU_INST_OTN2:
            dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
            dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);  /* should be 3 */
            dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 36); /* should be 36 */
            dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 172);  /* should be: 172 */
            dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 229); 
            dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 1);
            break;      
            
        case DIGI_DCSU_INST_ENET3: /* rate 388.8 MHz */
            dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
            dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
            dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 40);
            dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 0);
            dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 1);
            dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 1);     
            break;
        case DIGI_DCSU_INST_ENET1: /* rate 653 MHz */
            dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
            dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
            dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 67);
            dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 44);
            dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 263);
            dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 1);        
            break;   /* rate 515.625 MHz */
        case DIGI_DCSU_INST_ENET2:
            dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
            dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
            dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 53);
            dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 31);
            dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 648);
            dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 1);
            break;  
        case DIGI_DCSU_INST_PTP:
            /** PTP CLK is 125 Mhz clock */
            dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
            dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
            dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 64);
            dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 73);
            dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 243);
            dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 9);
            break;
        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, dcsu_inst, 0);
            break;
        }
    }
    else
    {
        switch(dcsu_inst) 
        {
        case DIGI_DCSU_INST_OTN1: /* rate 349.92 MHz */
            dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
            dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
            dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 72);
            dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 0);
            dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 1);
            dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 3);
            break;
        case DIGI_DCSU_INST_OTN2:
            dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
            dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
            dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 36);
            dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 172);  /* old value: 1 */
            dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 229); /* old value: 4 */
            dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 1);
            break;      
            
        case DIGI_DCSU_INST_ENET3: 
            /* ref311_clk*/ 
            dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 0);
            dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
            dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 40);
            dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 0);
            dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 1);
            dcsu_field_DIV1_ENB_set(b, h, dcsu_num, 0);
            dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 0x83);
            break;
        case DIGI_DCSU_INST_ENET1: /* rate 573.48 MHz */
            dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
            dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
            dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 59);
            dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 0);
            dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 1);
            dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 1);        
            break;   
        case DIGI_DCSU_INST_ENET2:/* rate 515.625 MHz */
            dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
            dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
            dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 53);
            dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 31);
            dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 648);
            dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 1);
            break;  
        case DIGI_DCSU_INST_PTP:
            /** PTP CLK is 125 Mhz clock */
            dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
            dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
            dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 64);
            dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 1290256896ULL);
            dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 4294964736ULL);
            dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 9);
            break;             
        case DIGI_DCSU_INST_SYS2: /* For SYS2 ref clock 653 MHz*/
            dcsu_field_DIV3_ENB_set(b, h, dcsu_num, 0);
            break;

        default:
            PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, dcsu_inst, 0);
            break;
        }
    }
        

    /* apply register configuration */
    dcsu_buffer_flush( b ); /* use the buffer to apply these values. */

    if (DIGI_DCSU_INST_SYS2 != dcsu_inst)
    {        
        /*  (3)     Set ARSTB high (deasserted) to start the DCO clocks */
        dcsu_field_ARSTB_set(NULL, h, dcsu_num, SET_TO_HIGH);
        
        /*  (4)     Wait 5 us for the DCO clocks and dividers to stabilize */
        PMC_Q_USLEEP(h,5);
        
        /*  (5)     Set DIV_RSTB low to reset the DCO dividers and align DCO output clocks. */
        dcsu_field_DIV_RSTB_set(NULL, h, dcsu_num, SET_TO_LOW);
    
        /*  (6) Wait 100 ns */
        PMC_Q_USLEEP(h,1);
        
        /*  (7)     Set DIV_RSTB high */
        dcsu_field_DIV_RSTB_set(NULL, h, dcsu_num, SET_TO_HIGH);
        
        /*  (8)     Wait 100 ns */
        PMC_Q_USLEEP(h,1);
        
        /*  (9)     Assert POST_UPDATE_EN and FB_UPDATE_EN to load in the post divider and feedback divider configuration   */
        dcsu_field_POST_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_HIGH);
        dcsu_field_FB_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_HIGH);
        
        /*  (10)    Wait 100 ns */
        PMC_Q_USLEEP(h,1);
        
        /*  (11)    Deassert POST_UPDATE_EN and FB_UPDATE_EN.  */
        dcsu_field_POST_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_LOW);
        dcsu_field_FB_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_LOW);
        
        /* Poll LOCK_V after waiting 2 mseconds */ 
        /* add a wait... */
        PMC_Q_USLEEP(h,2000);
    
        result = dcsu_field_LOCK_V_poll(NULL,
                                        h,
                                        dcsu_num,
                                        1,
                                        PMC_POLL_EQUAL,
                                        DCSU_INIT_POLL_ITERATIONS,
                                        &num_failed_polls,
                                        5);
    }

    {
        h->var.dcsu_init_state[dcsu_inst] = TRUE;
    }   
   
    PMC_RETURN(result);


} /* dcsu_init */

/*******************************************************************************
* dcsu_uninit()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configures if PN11 is inserted into the datapath when an LOS or LOF occurs.
*
* INPUTS:
*  *h                 - pointer to DIGI handle instance
*  dcsu_inst          - refers to a DCSU module.  These modules have fixed values
*                       assigned to each term as they are used to address physical
*                       resources.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS when operation is executed successfully 
*   or a descriptive error code will be returned.
*
* NOTES:
*
*******************************************************************************/
PMC_ERROR dcsu_uninit(digi_handle_t *h, digi_dcsu_inst_t dcsu_inst)
{

    PMC_ENTRY();

    PMC_ASSERT(NULL != h, DIGI_ERR_NULL_HANDLE, 0, 0);

    digi_dcsu_act(h, dcsu_inst, FALSE);

    switch(dcsu_inst)
    {
    case DIGI_DCSU_INST_OTN1:
        tl_ic_field_OTN1_DCSU_RESET_set(NULL, h, SET_TO_HIGH); 
        tl_ic_field_OTN1_DCSU_IDDQ_set(NULL, h, SET_TO_HIGH);             
        break;
    case DIGI_DCSU_INST_OTN2:
        tl_ic_field_OTN2_DCSU_RESET_set(NULL, h, SET_TO_HIGH); 
        tl_ic_field_OTN2_DCSU_IDDQ_set(NULL, h, SET_TO_HIGH);
        break;            

    case DIGI_DCSU_INST_ENET3:
        tl_ic_field_ENET3_DCSU_RESET_set(NULL, h, SET_TO_HIGH);
        tl_ic_field_ENET3_DCSU_IDDQ_set(NULL, h, SET_TO_HIGH);
        break;
    case DIGI_DCSU_INST_ENET1:
        tl_ic_field_ENET1_DCSU_IDDQ_set(NULL, h, SET_TO_HIGH); 
        tl_ic_field_ENET1_DCSU_RESET_set(NULL, h, SET_TO_HIGH);
        break;  
    case DIGI_DCSU_INST_ENET2:
        tl_ic_field_ENET2_DCSU_RESET_set(NULL, h, SET_TO_HIGH);
        tl_ic_field_ENET2_DCSU_IDDQ_set(NULL, h, SET_TO_HIGH);           
        break;  
    case DIGI_DCSU_INST_PTP:
        tl_ic_field_PTP_DCSU_IDDQ_set(NULL, h, SET_TO_HIGH);            
        tl_ic_field_PTP_DCSU_RESET_set(NULL, h, SET_TO_HIGH);
        break;  
    case DIGI_DCSU_INST_SYS2: /* For SYS2 ref clock 653 MHz*/
        if (TRUE != pmc_is_digi_rev_a_revision(&h->base))
        {
            dcsu_field_DIV3_ENB_set(NULL, h, DIGI_DCSU_SYS2, 1);
        }
        else
        {
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
        }
        break;

    default:
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
        break;
    }

    h->var.dcsu_init_state[dcsu_inst] = FALSE;

    PMC_RETURN(PMC_SUCCESS);

} /* dcsu_uninit */

/*******************************************************************************
* digi_dcsu_act()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Activate/Deactivate DCSU instance.
*
* INPUTS:
*  *h                 - pointer to DIGI handle instance
*  dcsu_inst          - refers to a DCSU module.  These modules have fixed values
*                       assigned to each term as they are used to address physical
*                       resources.
*  act                - TRUE DCSU is activated.
*                       FALSE it is deactivated.    
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_dcsu_act(digi_handle_t *h, 
                          digi_dcsu_inst_t dcsu_inst,
                          BOOL8            act)
{
    digi_dcsu_num_t dcsu_num = digi_dcsu_inst_to_num_convert(dcsu_inst);
    UINT32 enable = (TRUE == act) ? 0 : 1;
    dcsu_buffer_t b[1];
        
    PMC_ENTRY();

    PMC_ASSERT(NULL != h, DIGI_ERR_NULL_HANDLE, 0, 0);

    dcsu_buffer_init( b, h, dcsu_num );

    if(dcsu_inst != DIGI_DCSU_INST_SYS2)
    {   
        /* apply default configuration for initialization procedure to complete */
        dcsu_field_DIV2_ENB_set(b, h, dcsu_num, enable);
        dcsu_field_DIV4_ENB_set(b, h, dcsu_num, enable);
    }

    dcsu_field_DIV3_ENB_set(b, h, dcsu_num, enable);

    dcsu_buffer_flush( b);

    
    h->var.dcsu_init_state[dcsu_inst] = act;

    switch(dcsu_inst) 
    {
    case DIGI_DCSU_INST_OTN1: 
        tl_ic_field_OTN1_DCSU_INTR_EN_DEVICE_set    (NULL, h, act);
        tl_ic_field_OTN1_DCSU_INTR_EN_PCIE_set      (NULL, h, act);
        break;
    case DIGI_DCSU_INST_OTN2:
        tl_ic_field_OTN2_DCSU_INTR_EN_DEVICE_set    (NULL, h, act);
        tl_ic_field_OTN2_DCSU_INTR_EN_PCIE_set      (NULL, h, act);
        break;      

    case DIGI_DCSU_INST_ENET3:
        tl_ic_field_FIC1_DCSU_INTR_EN_DEVICE_set    (NULL, h, act);
        tl_ic_field_FIC1_DCSU_INTR_EN_PCIE_set      (NULL, h, act);
        break;
    case DIGI_DCSU_INST_ENET1:
        tl_ic_field_ENET1_DCSU_INTR_EN_DEVICE_set    (NULL, h, act);
        tl_ic_field_ENET1_DCSU_INTR_EN_PCIE_set      (NULL, h, act);
        break;  
    case DIGI_DCSU_INST_ENET2:
        tl_ic_field_ENET2_DCSU_INTR_EN_DEVICE_set    (NULL, h, act);
        tl_ic_field_ENET2_DCSU_INTR_EN_PCIE_set      (NULL, h, act);
        break;  
    case DIGI_DCSU_INST_PTP:
        tl_ic_field_PTP_DCSU_INTR_EN_DEVICE_set    (NULL, h, act);
        tl_ic_field_PTP_DCSU_INTR_EN_PCIE_set      (NULL, h, act);
        break;  
    case DIGI_DCSU_INST_SYS1:
        tl_ic_field_SYS_DCSU_INTR_EN_DEVICE_set    (NULL, h, act);
        tl_ic_field_SYS_DCSU_INTR_EN_PCIE_set      (NULL, h, act);
        break;
    case DIGI_DCSU_INST_SYS2:
        tl_ic_field_SYS2_DCSU_INTR_EN_DEVICE_set    (NULL, h, act);
        tl_ic_field_SYS2_DCSU_INTR_EN_PCIE_set      (NULL, h, act);
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, dcsu_inst, 0);
        break;
    }

    PMC_RETURN();

} /* digi_dcsu_act */

/*******************************************************************************
* digi_dcsu_conditional_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a funtion to initialize the DCSU for subsystem conditionally.
*   
*   Initialize each DCSU if they are in start state.\n\n 
*
* INPUTS:
*  *h                 - pointer to DIGI handle instance
*  dcsu_inst          - refers to a DCSU module.  These modules have fixed values
*                       assigned to each term as they are used to address physical
*                       resources.
*
* OUTPUTS:
*  NONE.
*
* RETURNS:
*   NONE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_dcsu_conditional_init(digi_handle_t *h, digi_dcsu_inst_t dcsu_inst)
{
    PMC_ENTRY();

    /* verify parameters */   
    PMC_ASSERT((h != NULL), DIGI_ERR_INVALID_ARG, 0, 0);  
    PMC_ASSERT(dcsu_inst < LAST_DIGI_DCSU_INST, DIGI_ERR_INVALID_ARG, dcsu_inst, 0);


    if(h->var.dcsu_init_state[dcsu_inst] == FALSE)
    {
        digi_dcsu_act(h, dcsu_inst, TRUE);
    }

    PMC_RETURN();

}/* digi_dcsu_conditional_init */

/*******************************************************************************
* digi_dcsu_conditional_uninit()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a conditional funtion uninitializing DCSUs for each subsystem.
*   There is an excepton on OTN1 DCSU. OTN1 DCSU should be always configured
*   without performing conditional testing because it generates
*   the ACB reference clock.
*
*   Shut off the DCSU for subsystems if DCSU is not initialized and subsystems
*   are in start state.
*
* INPUTS:
*  *h               - pointer to DIGI handle instance
*  dcsu_inst        - refers to a DCSU module.  These modules have fixed values
*                     assigned to each term as they are used to address physical
*                     resources.
*
* OUTPUTS:
*  NONE.
*
* RETURNS:
*   NONE.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_dcsu_conditional_uninit(digi_handle_t *h, digi_dcsu_inst_t dcsu_inst)
{
    BOOL8 reset_dcsu = FALSE;

    PMC_ENTRY();

    /* verify parameters */   
    PMC_ASSERT((h != NULL), DIGI_ERR_INVALID_ARG, 0, 0);  
    PMC_ASSERT(dcsu_inst < LAST_DIGI_DCSU_INST, DIGI_ERR_INVALID_ARG, dcsu_inst, 0);

    if(h->var.dcsu_init_state[dcsu_inst] == FALSE) PMC_RETURN();

    /* the following switch statement will ensure that all channels which require
       the DCSU instance to be initialized for proper operation are in starting
       state, if so, the DCSU will be put in reset, else, do nothing */
    switch(dcsu_inst)
    {
    case DIGI_DCSU_INST_OTN1:
        /* must be initialized for proper operation of all subsystems. */
        reset_dcsu = TRUE;
        break;
    case DIGI_DCSU_INST_OTN2:
        /* must be initalized for proper operation of connections in LINEOTN */
        if( lineotn_start_state_test(h->lineotn_handle) == TRUE )
        {
            reset_dcsu = TRUE;
        }
        break;  
            
    case DIGI_DCSU_INST_ENET1:
    case DIGI_DCSU_INST_ENET2:
        /* must be initialized for proper operation of connections on ENET_LINE and ENET_SYS */
        if(enet_start_state_test(h->enet_sys_handle) == TRUE &&
           enet_start_state_test(h->enet_line_handle) == TRUE)
        {
            reset_dcsu = TRUE;
        }
        break; 
        /* DCSU ENET3 is used for ENET TRANS */
    case DIGI_DCSU_INST_ENET3:
        if (TRUE == pmc_is_digi_rev_a_revision(&h->base))
        {
            if ( enet_start_state_test(h->enet_line_handle) == TRUE)
            {
                reset_dcsu = TRUE;
            };
        }
        break;
    case DIGI_DCSU_INST_PTP:
        reset_dcsu = TRUE;
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }

    if(reset_dcsu==TRUE)
    {
        digi_dcsu_act(h, dcsu_inst, FALSE);
    }

    PMC_RETURN();

}/* digi_dcsu_conditional_uninit */

/*******************************************************************************
* digi_dcsu_inst_to_num_convert
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is a helper function to determine the digi_dcsu_num_t that is 
*   equivalent to the digi_dcsu_inst_t.\n\n
*
*   Should DCSU Num type not be valid an assertion is raised.
*
*
* INPUTS:
*   dcsu_inst         - enum of DCSU instance type
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   digi_dcsu_num_t   - Equivalent DCSU Number type enum for the DCSU Inst enum
*
* NOTES:
*
*******************************************************************************/
PRIVATE digi_dcsu_num_t digi_dcsu_inst_to_num_convert(digi_dcsu_inst_t dcsu_inst)
{
    digi_dcsu_num_t dcsu_num = LAST_DIGI_DCSU;
    PMC_ENTRY();

    switch(dcsu_inst)
    {
    case DIGI_DCSU_INST_OTN1:
        dcsu_num = DIGI_DCSU_OTN1;
        break;
    case DIGI_DCSU_INST_OTN2:
        dcsu_num = DIGI_DCSU_OTN2;
        break;
    case DIGI_DCSU_INST_ENET3:
        dcsu_num = DIGI_DCSU_ENET3;
        break;
    case DIGI_DCSU_INST_ENET1:
        dcsu_num = DIGI_DCSU_ENET1;
        break;
    case DIGI_DCSU_INST_ENET2:
        dcsu_num = DIGI_DCSU_ENET2;
        break;
    case DIGI_DCSU_INST_PTP:
        dcsu_num = DIGI_DCSU_PTP;
        break;
    case DIGI_DCSU_INST_SYS2:
        dcsu_num = DIGI_DCSU_SYS2;
        break;
    case DIGI_DCSU_INST_SYS1:
        dcsu_num = DIGI_DCSU_SYS1;
        break;
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }
    PMC_RETURN(dcsu_num);

} /* digi_dcsu_inst_to_num_convert */

/*******************************************************************************
* digi_dcsu_adjust_enet1
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API is used to adjust ENET1 clock frequency.  It tests the FB_DENOMINATOR
*   to determine if the desired rate is already set.  If already set no 
*   device change is made.
*   
* INPUTS:
*   h - digi_handle
*   rate - BOOL for rate, TRUE: 517 MHz FALSE: 573.48 MHz
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
PUBLIC void digi_dcsu_adjust_enet1(digi_handle_t *h, BOOL8 rate)
{

    digi_dcsu_num_t dcsu_num;
    UINT32 fb_denominator;
    dcsu_buffer_t b[1];

    PMC_ENTRY();

    /* modifiy the OTN1 clock frequency to match a nominal OTU4 clock frequency */
    dcsu_num = digi_dcsu_inst_to_num_convert(DIGI_DCSU_INST_ENET1);

    dcsu_buffer_init( b, h, dcsu_num );

    /* use FB_DENOMINATOR setting to determine if desired rate already set */
    fb_denominator = dcsu_field_FB_DENOMINATOR_get(b, h, dcsu_num);
    
    if(rate==TRUE)
    {
        /* 517 MHz Rate */
        if( fb_denominator != 243)
        {
            dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
            dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
            dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 53);
            dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 46);
            dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 243);
            dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 1);  
            /* apply register configuration */
            dcsu_buffer_flush( b ); /* use the buffer to apply these values. */

            /*  (9)     Assert POST_UPDATE_EN and FB_UPDATE_EN to load in the post divider and feedback divider configuration   */
            dcsu_field_POST_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_HIGH);
            dcsu_field_FB_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_HIGH);
            
            /*  (10)    Wait 100 ns */
            PMC_Q_USLEEP(h,1);
            
            /*  (11)    Deassert POST_UPDATE_EN and FB_UPDATE_EN.  */
            dcsu_field_POST_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_LOW);
            dcsu_field_FB_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_LOW);
        }
    }
    else
    {
        if( fb_denominator != 1)
        {
            /* 573.48 MHz Rate */
            dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
            dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
            dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 59);
            dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 0);
            dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 1);
            dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 1);  
            /* apply register configuration */
            dcsu_buffer_flush( b ); /* use the buffer to apply these values. */

            /*  (9)     Assert POST_UPDATE_EN and FB_UPDATE_EN to load in the post divider and feedback divider configuration   */
            dcsu_field_POST_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_HIGH);
            dcsu_field_FB_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_HIGH);
            
            /*  (10)    Wait 100 ns */
            PMC_Q_USLEEP(h,1);
            
            /*  (11)    Deassert POST_UPDATE_EN and FB_UPDATE_EN.  */
            dcsu_field_POST_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_LOW);
            dcsu_field_FB_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_LOW);
        }
    }

    PMC_RETURN();

} /* digi_dcsu_adjust_enet1 */

/*******************************************************************************
* digi_dcsu_adjust_enet2
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API is used to adjust ENET1 clock frequency.
*   
* INPUTS:
*   h - digi_handle
*   rate - BOOL for rate, TRUE: 517 MHz (over rate) FALSE: 573.48 MHz (default)
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
PUBLIC void digi_dcsu_adjust_enet2(digi_handle_t *h, BOOL8 rate)
{

    digi_dcsu_num_t dcsu_num;
    dcsu_buffer_t b[1];

    PMC_ENTRY();

    /* modifiy the OTN1 clock frequency to match a nominal OTU4 clock frequency */
    dcsu_num = digi_dcsu_inst_to_num_convert(DIGI_DCSU_INST_ENET2);

    dcsu_buffer_init( b, h, dcsu_num );
    
    if(rate==TRUE)
    {
        /* 515.625 + 30 ppm MHz Rate */
        dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
        dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
        dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 53);
        dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 1025);
        dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 20736);
        dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 1);  
    }
    else
    {
        /* 515.625 MHz Rate */
        dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
        dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 3);
        dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 53);
        dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 31);
        dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 648);
        dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 1);
    }


    /* apply register configuration */
    dcsu_buffer_flush( b ); /* use the buffer to apply these values. */

    /*  (9)     Assert POST_UPDATE_EN and FB_UPDATE_EN to load in the post divider and feedback divider configuration   */
    dcsu_field_POST_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_HIGH);
    dcsu_field_FB_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_HIGH);
    
    /*  (10)    Wait 100 ns */
    PMC_Q_USLEEP(h,1);
    
    /*  (11)    Deassert POST_UPDATE_EN and FB_UPDATE_EN.  */
    dcsu_field_POST_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_LOW);
    dcsu_field_FB_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_LOW);
    


    PMC_RETURN();

} /* digi_dcsu_adjust_enet2 */

/*******************************************************************************
* digi_dcsu_adjust_otu1_for_sysotn
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This API is used to adjust OTN1 clock frequency to match a nominal OTU4 
*   clock frequency.
*   
* INPUTS:
*   h - digi_handle
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

/* LCOV_EXCL_START */

PUBLIC void digi_dcsu_adjust_otu1_for_sysotn(digi_handle_t *h)
{

    digi_dcsu_num_t dcsu_num;
    dcsu_buffer_t b[1];

    PMC_ENTRY();

    /* modifiy the OTN1 clock frequency to match a nominal OTU4 clock frequency */
    dcsu_num = digi_dcsu_inst_to_num_convert(DIGI_DCSU_INST_OTN1);

    dcsu_buffer_init( b, h, dcsu_num );
    
    dcsu_field_DIVIDER_CLKSEL_set(b, h, dcsu_num, 1);
    dcsu_field_PRE_DIV_BY_set(b, h, dcsu_num, 1);
    dcsu_field_FB_INT_DIV_set(b, h, dcsu_num, 35);
    dcsu_field_FB_NUMERATOR_set(b, h, dcsu_num, 215);
    dcsu_field_FB_DENOMINATOR_set(b, h, dcsu_num, 227);
    dcsu_field_POST_DIV_BY_set(b, h, dcsu_num, 3);

    /* apply register configuration */
    dcsu_buffer_flush( b ); /* use the buffer to apply these values. */

    /*  (9)     Assert POST_UPDATE_EN and FB_UPDATE_EN to load in the post divider and feedback divider configuration   */
    dcsu_field_POST_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_HIGH);
    dcsu_field_FB_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_HIGH);
    
    /*  (10)    Wait 100 ns */
    PMC_Q_USLEEP(h,1);
    
    /*  (11)    Deassert POST_UPDATE_EN and FB_UPDATE_EN.  */
    dcsu_field_POST_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_LOW);
    dcsu_field_FB_UPDATE_EN_set(NULL, h, dcsu_num, SET_TO_LOW);
    


    PMC_RETURN();

} /* digi_dcsu_adjust_otu1_for_sysotn */

/* LCOV_EXCL_STOP */

/*******************************************************************************
* FUNCTION: digi_dcsu_int_internal_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables DCSU block interrupts based on bits that are set 
*   in the interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   dcsu_inst       - Specifies the DCSU block.
*   int_table_ptr   - pointer to interrupt table with bits set to
*                     enable interrupts.
*   enable          - When TRUE interrupts are enabled, otherwise there are
*                     disabled
*
* OUTPUTS:
*   int_en_table_ptr  - Pointer to table tracking enabled interrupts
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_dcsu_int_cfg(digi_handle_t             *digi_handle,
                                   digi_dcsu_inst_t           dcsu_inst,
                                   digi_dcsu_int_t           *int_table_ptr,
                                   digi_dcsu_int_t           *int_en_table_ptr,
                                   BOOL                       enable)
{      
    PMC_ERROR                result = PMC_SUCCESS;
    digi_dcsu_num_t          link;

    PMC_ENTRY();

    link = digi_dcsu_inst_to_num_convert(dcsu_inst);
    
#ifdef DIGI_DCSU_INT
#undef DIGI_DCSU_INT
#endif
#define DIGI_DCSU_INT  UTIL_GLOBAL_INT_ENABLE

    DIGI_DCSU_INT_TABLE_DEFINE();            

    PMC_RETURN(result);
} /* digi_dcsu_int_enable */

/*******************************************************************************
* digi_dcsu_restart_init
* ___________________________________________________________________________
*
* DESCRIPTION:
*  Audit and cleanup DCSU context at digi top layer and in block level.
*
* INPUTS:
*  *digi_handle            - Pointer to the digi handle instance.
*  *restart_init_cfg_ptr   - pointer to restart init config
*
* OUTPUTS:
*  None
*
* RETURNS:
*  PMC_SUCCESS if operation is successfully executed otherwise a descriptive
*  error code is returned. 
*
* NOTES:
*  None
*******************************************************************************/
PUBLIC PMC_ERROR digi_dcsu_restart_init(digi_handle_t                  *digi_handle,
                                        util_global_restart_init_cfg_t *restart_init_cfg_ptr)
{
    PMC_ERROR rc = DIGI_ERR_UNKNOWN;
    BOOL8 dcsu_clean_state[LAST_DIGI_DCSU_INST];
    
    PMC_ENTRY();

    PMC_MEMSET((void *)&dcsu_clean_state[0], 0x0, sizeof(BOOL8) * (UINT32)LAST_DIGI_DCSU_INST);

    /* 
       Recover context from register space        
       - var context is partially recovered from register space (good enough to have a valid audit)
       - only enabled DCSU configruation are recovered
       - anyway a partial DCSU config is an error in warm restart and will be cleanup in crash-restarted
       - incoherency in register context means that the DCSU shall be disabled
    */

    /* 
     * Audit register context and memory context 
     *   - Create a new context that represents the intersection between memory and register space (ctxt_clean_ptr)
     */
    rc = digi_dcsu_ctxt_audit(digi_handle,
                              dcsu_clean_state); 
    PMC_LOG_TRACE("dcsu_ctxt_audit rc = %s\n", pmc_log_string_get(rc));
    
    if (DIGI_DCSU_ERR_CONTEXT_CLEANUP == rc && restart_init_cfg_ptr->is_warm_restart == FALSE)
    {
        /* 
         * Cleanup 
         *   - At the end, dcsu_ctxt is the new context
         *    - We keep enabled DCSU defined in ctxt_clean_ptr
         *    - We clean (deactivate/deprov) all DCSUs that are not enabled
         *       - they may have be partially enabled
         */                                    
        rc = digi_dcsu_ctxt_cleanup(digi_handle,
                                    dcsu_clean_state); 
    }

    if (PMC_SUCCESS == rc && UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST ==
        restart_init_cfg_ptr->phase)
    {
        UINT32 reg_value = tl_ic_field_OTN2_DCSU_RESET_get(NULL, digi_handle);
        if (1 == reg_value)
        {
            dcsu_init(digi_handle, DIGI_DCSU_INST_OTN2);
            fec_gfec_lowpwr_set(digi_handle->lineotn_handle, FALSE);
            fec_all_swizzle_fec_lowpwr_set(digi_handle->lineotn_handle, FALSE); 
            fec_gfec_lowpwr_set(digi_handle->sysotn_handle, FALSE);
            PMC_Q_USLEEP(digi_handle->sysotn_handle,1);
            fec_gfec_lowpwr_set(digi_handle->lineotn_handle, TRUE);    
            fec_all_swizzle_fec_lowpwr_set(digi_handle->lineotn_handle, TRUE);      
            fec_gfec_lowpwr_set(digi_handle->sysotn_handle, TRUE);
            digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_OTN2);
        }
        reg_value = tl_ic_field_OTN1_DCSU_RESET_get(NULL, digi_handle);

        if (1 == reg_value)
        {
            dcsu_init(digi_handle, DIGI_DCSU_INST_OTN1);
        }
        reg_value = tl_ic_field_ENET1_DCSU_RESET_get(NULL, digi_handle);

        if (1 == reg_value)
        {
            dcsu_init(digi_handle, DIGI_DCSU_INST_ENET1);
            dcsu_init(digi_handle, DIGI_DCSU_INST_ENET2);
            dcsu_init(digi_handle, DIGI_DCSU_INST_PTP);
            digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_ENET1);
            digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_ENET2);
            digi_dcsu_conditional_uninit(digi_handle, DIGI_DCSU_INST_PTP);
        }
    }

    PMC_RETURN(rc);

} /* digi_dcsu_restart_init */


/*******************************************************************************
* digi_dcsu_ctxt_audit
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function audits DIGI_DCSU handle context variable against register space content.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI_DCSU handle instance.
*   *clean_state_ptr       - pointer to array of dcsus state.
*
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS if auditing is successful otherwise a descriptive error code is returned.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_dcsu_ctxt_audit(digi_handle_t   *digi_handle,
                                       BOOL8            *clean_state_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    PMC_ERROR loop_rc = PMC_SUCCESS;
    UINT32 itr;
    BOOL8 dcsu_en;
    digi_dcsu_num_t dcsu_num;
    BOOL8 uncond_init; 
    digi_m1_pmm_lite_clk_ref_t ref_clk_src;
    DOUBLE ref_clk_freq;
    BOOL8 rate_mon_en;
    digi_dcsu_inst_t dcsu_inst;
    PMC_ENTRY();
    
    for (itr = 0; itr < (UINT32)LAST_DIGI_DCSU_INST; itr += 1)
    { 
        if (DIGI_DCSU_INST_SYS1 == (digi_dcsu_inst_t)itr)
        {
            continue;
        }

        if (pmc_is_digi_rev_a_revision(&digi_handle->base) &&
            DIGI_DCSU_INST_SYS2 == (digi_dcsu_inst_t)itr)
        {
            continue;
        }

        dcsu_inst = (digi_dcsu_inst_t)itr;
        uncond_init = FALSE;
        loop_rc = PMC_SUCCESS;
        dcsu_num = digi_dcsu_inst_to_num_convert(dcsu_inst);
        dcsu_en = dcsu_status_from_reg_space_retrieve(digi_handle,
                                                      dcsu_inst);

        /* compare register space content against dcsu context */        
        switch (dcsu_inst)
        {
        case DIGI_DCSU_INST_OTN1:
        case DIGI_DCSU_INST_SYS2:
            uncond_init = TRUE;
            break;
        case DIGI_DCSU_INST_ENET3:
            if (TRUE != pmc_is_digi_rev_a_revision(&digi_handle->base))
            {
                uncond_init = TRUE;
            }
            break;
        default:
            break;
        }

        if (digi_handle->var.dcsu_init_state[itr] != dcsu_en)
        {

            loop_rc = DIGI_DCSU_ERR_CONTEXT_CLEANUP;
            if (TRUE == uncond_init)
            {
                loop_rc = DIGI_DCSU_ERR_CONTEXT_INCOHERENT;
            }
            else
            {
                if (FALSE == dcsu_en)
                {
                    /* DCSU context audit should be executed after all SS restart otherwise
                       false-error could be raised becaude DCSU and SS context are not in sync */
                    switch (dcsu_inst)
                    {
                    case DIGI_DCSU_INST_OTN2:
                        /* must be initalized for proper operation of connections in LINEOTN */
                        if( lineotn_start_state_test(digi_handle->lineotn_handle) != TRUE )
                        {
                            loop_rc = DIGI_DCSU_ERR_CONTEXT_INCOHERENT; 
                        }
                        break;  
                        
                    case DIGI_DCSU_INST_ENET1:
                    case DIGI_DCSU_INST_ENET2:
                        /* must be initialized for proper operation of connections on ENET_LINE and ENET_SYS */
                        if(enet_start_state_test(digi_handle->enet_sys_handle) != TRUE &&
                           enet_start_state_test(digi_handle->enet_line_handle) != TRUE)
                        {
                            loop_rc = DIGI_DCSU_ERR_CONTEXT_INCOHERENT; 
                        }
                        break; 
                        /* DCSU ENET3 is used for ENET TRANS */
                    case DIGI_DCSU_INST_ENET3:
                        if (TRUE == pmc_is_digi_rev_a_revision(&digi_handle->base))
                        {
                            if ( enet_start_state_test(digi_handle->enet_line_handle) != TRUE)
                            {
                                loop_rc = DIGI_DCSU_ERR_CONTEXT_INCOHERENT; 
                            }
                        }
                        break;
                    case DIGI_DCSU_INST_PTP:
                        digi_m1_rate_monitor_cfg_get(digi_handle,
                                                     &ref_clk_src,
                                                     &ref_clk_freq,
                                                     &rate_mon_en);
                        if (TRUE == rate_mon_en)
                        {
                            loop_rc = DIGI_DCSU_ERR_CONTEXT_INCOHERENT; 
                        }
                        break;

                    default:
                        break;
                    }
                }
            }            
        }

        if (PMC_SUCCESS == rc)
        {
            rc = loop_rc;
        }
        clean_state_ptr[itr] = dcsu_en;
        if (loop_rc == DIGI_DCSU_ERR_CONTEXT_INCOHERENT || dcsu_en == FALSE)
        {
            clean_state_ptr[itr] = FALSE;
        } 
    }
    
    PMC_RETURN(rc);
} /* digi_dcsu_ctxt_audit */

/*******************************************************************************
* digi_dcsu_ctxt_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function cleans up DIGI_DCSU handle context. 
*
* INPUTS:
*   *digi_handle           - pointer to DIGI_DCSU handle instance.
*   *clean_state_ptr       - pointer to array of dcsus state.
*
* OUTPUTS:
*   None
*
* RETURNS:
*  PMC_SUCCESS if operation is successfully executed otherwise a descriptive
*  error code is returned. 
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_dcsu_ctxt_cleanup(digi_handle_t   *digi_handle,
                                         BOOL8            *clean_state_ptr)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 itr;
    BOOL8 skip;

    PMC_ENTRY();

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* disable uncertain DCSU to leave register space in clean state */
    for (itr = 0; itr < (UINT32)LAST_DIGI_DCSU_INST; itr += 1)
    {
        if (FALSE == clean_state_ptr[itr])
        {      
            skip = FALSE;  
            switch((digi_dcsu_inst_t)itr)
            {
            case DIGI_DCSU_INST_OTN1:
            case DIGI_DCSU_INST_OTN2:
            case DIGI_DCSU_INST_ENET3:
            case DIGI_DCSU_INST_ENET1:
            case DIGI_DCSU_INST_ENET2:
            case DIGI_DCSU_INST_PTP:
            case DIGI_DCSU_INST_SYS2:
                break;
            default:
                skip = TRUE;
                break;
            }
            if (FALSE == skip)
            {
                digi_dcsu_act(digi_handle, (digi_dcsu_inst_t)itr, FALSE);
            }
        }
    }

    /* copy clean context in digi_handle context */
    PMC_MEMCPY((void*)digi_handle->var.dcsu_init_state, (void *)clean_state_ptr, sizeof(BOOL8) * LAST_DIGI_DCSU_INST);

    PMC_RETURN(rc);
} /* digi_dcsu_ctxt_cleanup */

/*******************************************************************************
* dcsu_status_from_reg_space_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This is an function to retrieve DCSU instance status from register space.
*
* INPUTS:
*  *h                 - pointer to DIGI handle instance
*  dcsu_inst          - refers to a DCSU module.  These modules have fixed values
*                       assigned to each term as they are used to address physical
*                       resources.
*
* OUTPUTS:
*   None
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL8 dcsu_status_from_reg_space_retrieve(digi_handle_t   *h, 
                                                  digi_dcsu_inst_t dcsu_inst)
{
    digi_dcsu_num_t dcsu_num = digi_dcsu_inst_to_num_convert(dcsu_inst);
    BOOL8 dcsu_status = TRUE;
    UINT8 div_enb = 0;

    PMC_ENTRY();

    PMC_ASSERT(NULL != h, DIGI_ERR_NULL_HANDLE, 0, 0);   
    PMC_ASSERT((UINT32)dcsu_inst < (UINT32)LAST_DIGI_DCSU, DIGI_ERR_INVALID_ARG, 0, 0);


    /* special case for SYS2 DCSU */
    if (DIGI_DCSU_INST_SYS2 != dcsu_inst)
    {           
        div_enb = dcsu_field_DIV3_ENB_get(NULL, h, dcsu_num);
    }
    else
    {
        if (TRUE != pmc_is_digi_rev_a_revision(&h->base))
        {      
            div_enb = dcsu_field_DIV3_ENB_get(NULL, h, dcsu_num);
        }
    }     

    if (TRUE == dcsu_status && 0 != div_enb)
    {
        dcsu_status = FALSE;
    }

    PMC_RETURN(dcsu_status);
} /* dcsu_status_from_reg_space_retrieve */


#endif /* DOXYGEN_PUBLIC_ONLY */

/*
** End of file
*/
