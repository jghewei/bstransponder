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
*   DESCRIPTION:
*     This file contains functions to configure the SDH_PMG block.
*
*   NOTES:
*
*******************************************************************************/

/*
** Include Files
*/
#include "sdh_pmg_loc.h"

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
#define SDH_PMG_RTTP_ASCII_CR           0x0D
#define SDH_PMG_RTTP_ASCII_LF           0x0A
#define SDH_PMG_RTTP_ASCII_CRC_START    0x80
#define SDH_PMG_RTTP_INVALID_INDEX      0xFF

/*
** Local Macro Definitions
*/

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/*! sber_tbl stores the register configuration values corresponding to the B1 and B2 BER thresholds in the table indices */
PRIVATE sdh_pmg_sber_info_t  sber_tbl[LAST_SDH_PMG_SBER_BYTE][LAST_SDH_PMG_SBER_STS_MODES][LAST_SDH_PMG_SBER_BER] =
{
    /*[SDH_PMG_SBER_BYTE_B1] = */
    {
        /* [SDH_PMG_SBER_STS_192] */
        {
            /* set_threshold, clr_threshold, int_per_2,int_per_1  */
            {0, 0, 0, 0},                     /* 0: SDH_PMG_SBER_BER_DISABLED */   
            {0, 0, 0, 0},                     /* 1: SDH_PMG_SBER_BER_1E_03 */       
            {0, 0, 0, 0},                     /* 2: SDH_PMG_SBER_BER_1E_04 */       
            {0x003F, 0x003F, 0x0000, 0x0015}, /* 3: SDH_PMG_SBER_BER_1E_05 */ 
            {0x0019, 0x0012, 0x0000, 0x0027}, /* 4: SDH_PMG_SBER_BER_1E_06 */
            {0x0016, 0x000F, 0x0000, 0x0136}, /* 5: SDH_PMG_SBER_BER_1E_07 */
            {0x0015, 0x000F, 0x0000, 0x0BD9}, /* 6: SDH_PMG_SBER_BER_1E_08 */
            {0x0015, 0x000F, 0x0000, 0x762D}, /* 7: SDH_PMG_SBER_BER_1E_09 */
            {0x0015, 0x000F, 0x0004, 0x9D7D}, /* 8: SDH_PMG_SBER_BER_1E_10 */
            {0x0015, 0x000F, 0x002E, 0x269F}, /* 9: SDH_PMG_SBER_BER_1E_11 */
            {0, 0, 0, 0},                     /* 10: SDH_PMG_SBER_BER_1E_12 */
        },
       /* [SDH_PMG_SBER_STS_768] */
        {
            /* set_threshold, clr_threshold, int_per_2, int_per_1  */
            {0, 0, 0, 0},                     /* 0: SDH_PMG_SBER_BER_DISABLED */       
            {0, 0, 0, 0},                     /* 1: SDH_PMG_SBER_BER_1E_03*/       
            {0, 0, 0, 0},                     /* 2: SDH_PMG_SBER_BER_1E_04 */       
            {0x03E3, 0x0667, 0x0000, 0x010E}, /* 3: SDH_PMG_SBER_BER_1E_05 */
            {0x0025, 0x001E, 0x0000, 0x0014}, /* 4: SDH_PMG_SBER_BER_1E_06 */
            {0x0017, 0x0010, 0x0000, 0x0054}, /* 5: SDH_PMG_SBER_BER_1E_07 */
            {0x0015, 0x000F, 0x0000, 0x02FC}, /* 6: SDH_PMG_SBER_BER_1E_08 */
            {0x0015, 0x000F, 0x0000, 0x1D91}, /* 7: SDH_PMG_SBER_BER_1E_09 */
            {0x0015, 0x000F, 0x0001, 0x2765}, /* 8: SDH_PMG_SBER_BER_1E_10 */
            {0x0015, 0x000F, 0x000B, 0x89AE}, /* 9: SDH_PMG_SBER_BER_1E_11 */
            {0x0001, 0x0000, 0x001E, 0x83CE}, /* 10: SDH_PMG_SBER_BER_1E_12 */
        },
    },
    /* [SDH_PMG_SBER_BYTE_B2] */
    {
     /*   [SDH_PMG_SBER_STS_192] */
        {
            /* set_threshold, clr_threshold, int_per_2,int_per_1  */
            {0, 0, 0, 0},                     /* 0: SDH_PMG_SBER_BER_DISABLED */       
            {0x0AB8, 0x076C, 0x0000, 0x0004}, /* 1: SDH_PMG_SBER_BER_1E_03 */  
            {0x0056, 0x0026, 0x0000, 0x0001}, /* 2: SDH_PMG_SBER_BER_1E_04 */       
            {0x001E, 0x0012, 0x0000, 0x0004}, /* 3: SDH_PMG_SBER_BER_1E_05 */
            {0x0016, 0x000F, 0x0000, 0x001F}, /* 4: SDH_PMG_SBER_BER_1E_06 */
            {0x0015, 0x000F, 0x0000, 0x0132}, /* 5: SDH_PMG_SBER_BER_1E_07 */
            {0x0015, 0x000F, 0x0000, 0x0BF3}, /* 6: SDH_PMG_SBER_BER_1E_08 */
            {0x0015, 0x000F, 0x0000, 0x777A}, /* 7: SDH_PMG_SBER_BER_1E_09 */
            {0x0015, 0x000F, 0x0004, 0xAABC}, /* 8: SDH_PMG_SBER_BER_1E_10 */
            {0x0015, 0x000F, 0x002E, 0xAB57}, /* 9: SDH_PMG_SBER_BER_1E_11 */
            {0, 0, 0, 0},                     /* 10: SDH_PMG_SBER_BER_1E_12 */
        },
       /* [SDH_PMG_SBER_STS_768] */
        {
            /* set_threshold, clr_threshold, int_per_2,int_per_1  */
            {0, 0, 0, 0},                     /* 0: SDH_PMG_SBER_BER_DISABLED */
            {0x0ABA, 0x07AA, 0x0000, 0x0001}, /* 1: SDH_PMG_SBER_BER_1E_03 */ 
            {0x018F, 0x007C, 0x0000, 0x0001}, /* 2: SDH_PMG_SBER_BER_1E_04 */       
            {0x001E, 0x0012, 0x0000, 0x0001}, /* 3: SDH_PMG_SBER_BER_1E_05 */
            {0x0017, 0x0010, 0x0000, 0x0008}, /* 4: SDH_PMG_SBER_BER_1E_06 */
            {0x0016, 0x000F, 0x0000, 0x004D}, /* 5: SDH_PMG_SBER_BER_1E_07 */
            {0x0015, 0x000F, 0x0000, 0x02FD}, /* 6: SDH_PMG_SBER_BER_1E_08 */
            {0x0015, 0x000F, 0x0000, 0x1DDF}, /* 7: SDH_PMG_SBER_BER_1E_09 */
            {0x0015, 0x000F, 0x0001, 0x2AAF}, /* 8: SDH_PMG_SBER_BER_1E_10 */
            {0x0015, 0x000F, 0x000B, 0xAAD6}, /* 9: SDH_PMG_SBER_BER_1E_11 */
            {0, 0, 0, 0},                     /* 10: SDH_PMG_SBER_BER_1E_12 */
        },
    },
}; 




/* Only include the strings if logging with text*/
#ifdef PMC_LOG_STRINGS

/* 
 * The following creates the LOG/ERR strings from above by pulling out the text
 * portion of the LOG_ERR_TABLE above 
 */
#define PMC_LOG_ERR_PAIR_ENTRY_CREATE( enum, str ) str,
const char SDH_PMG_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{ "Start of log string list", SDH_PMG_LOG_ERR_TABLE };
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else  /* no strings */ 
const char SDH_PMG_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */

PRIVATE BOOL sdh_pmg_is_sts_768(sdh_pmg_sts_mode_t    mode);
PRIVATE PMC_ERROR sdh_pmg_rttp_crc_calc(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                        UINT8                     msg[15],
                                        UINT32                   *e164Crc);
PRIVATE PMC_ERROR sdh_pmg_int_init(sdh_pmg_handle_t    *sdh_pmg_handle,
                                   BOOL                 enable);
PRIVATE UINT32 sdh_pmg_rrmp_index_get(sdh_pmg_handle_t    *sdh_pmg_handle,
                                      UINT32               index);
PRIVATE PMC_ERROR sdh_pmg_range_get(sdh_pmg_handle_t    *sdh_pmg_handle,
                                    UINT32               index,
                                    UINT8               *start_index,
                                    UINT8               *end_index);
PRIVATE UINT32 sdh_pmg_rttp_index_get(sdh_pmg_handle_t    *sdh_pmg_handle,
                                      UINT32               index);
PRIVATE PMC_ERROR sdh_pmg_rttp_algo_set(sdh_pmg_handle_t    *sdh_pmg_handle,
                                        UINT32               index,
                                        sdh_pmg_rttp_algo_t  algo);
PRIVATE PMC_ERROR sdh_pmg_rttp_expect_trace_set_internal(sdh_pmg_handle_t      *sdh_pmg_handle,
                                                         UINT32                 index,
                                                         sdh_pmg_rttp_trace_t  *expect_trace_ptr);
PRIVATE PMC_ERROR sdh_pmg_rttp_arg_check(sdh_pmg_handle_t      *sdh_pmg_handle_ptr,
                                         UINT32                 index,
                                         sdh_pmg_rttp_algo_t    algo,
                                         sdh_pmg_rttp_trace_t  *expect_trace_ptr);
PRIVATE PMC_ERROR sdh_pmg_rttp_trace_get_arg_check(sdh_pmg_handle_t           *sdh_pmg_handle,
                                                   UINT32                      index,
                                                   sdh_pmg_rttp_trace_type_t   trace_type,
                                                   sdh_pmg_rttp_trace_t       *trace_ptr);
PRIVATE PMC_ERROR sdh_pmg_rttp_trace_get_internal(sdh_pmg_handle_t          *sdh_pmg_handle,
                                                  UINT32                     index,
                                                  sdh_pmg_rttp_trace_type_t  trace_type,
                                                  UINT8                     *trace_msg_ptr,
                                                  UINT8                     *cr_index,
                                                  UINT8                     *lf_index,
                                                  UINT8                     *crc_index);
PRIVATE PMC_ERROR sdh_pmg_rttp_trace_realign(sdh_pmg_handle_t       *sdh_pmg_handle_ptr,
                                             UINT32                  index,
                                             UINT8                  *trace_msg_ptr,
                                             UINT8                   cr_index,
                                             UINT8                   lf_index,
                                             UINT8                   crc_index,
                                             sdh_pmg_rttp_trace_t   *trace_ptr);

PUBLIC PMC_ERROR sdh_pmg_chnl_en(sdh_pmg_handle_t      *sdh_pmg_handle_ptr,
                                  UINT32                 index,
                                  BOOL                   enable);

/*
** Public Functions
*/

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */

/*******************************************************************************
* sdh_pmg_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an SDH_PMG block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the SDH_PMG subsystem relative to 
*                           the device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - Name of tsb
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   sdh_pmg_handle        - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC sdh_pmg_handle_t *sdh_pmg_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    sdh_pmg_handle_t *sdh_pmg_handle;

    PMC_ENTRY();

    sdh_pmg_handle = (sdh_pmg_handle_t*)PMC_CTXT_CALLOC(sizeof(sdh_pmg_handle_t), parent);
    
    pmc_handle_init(parent, sdh_pmg_handle, sys_handle, PMC_MID_DIGI_SDH_PMG, tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(SDH_PMG_LOG_ERR_STRINGS[0], SDH_PMG_LOG_ERR_TABLE_BASE, SDH_PMG_LOG_ERR_COUNT);
    
    PMC_RETURN(sdh_pmg_handle);
} /* sdh_pmg_ctxt_create */

 
/*******************************************************************************
* sdh_pmg_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an SDH_PMG block instance.
*
*
* INPUTS:
*   *sdh_pmg_handle         - pointer to SDH_PMG handle instance
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
PUBLIC void sdh_pmg_ctxt_destroy(sdh_pmg_handle_t *sdh_pmg_handle)
{
    PMC_ENTRY();

    pmc_handle_mid_verify(sdh_pmg_handle, PMC_MID_DIGI_SDH_PMG);
    sdh_pmg_handle->base.tsb_mid = PMC_MID_INVALID;
    PMC_CTXT_FREE(&sdh_pmg_handle, sdh_pmg_handle);

    PMC_RETURN();
} /* sdh_pmg_ctxt_destroy */


/*******************************************************************************
* FUNCTION: sdh_pmg_var_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes dynamic context 
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sdh_pmg_var_init(sdh_pmg_handle_t    *sdh_pmg_handle_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;    
    UINT8         i, j;

    PMC_ENTRY();

    for (i = 0; i < SDH_PMG_MAX_INDEX; i++)
    {
        /* reset sdh mode info */
        sdh_pmg_handle_ptr->var.mode[i] = LAST_SDH_PMG_STS_MODE;
        
        /* reset rttp info */
        sdh_pmg_handle_ptr->var.rttp_len[i] = LAST_SDH_PMG_RTTP_LENGTH;
        sdh_pmg_handle_ptr->var.rttp_algo[i] = SDH_PMG_RTTP_ALGO_DISABLED;
    
        /* reset SBER BER info */ 
        for (j = 0; j < LAST_SDH_PMG_SBER_ALARM; j++)
        {
            sdh_pmg_handle_ptr->var.sber_ber[j][i] = SDH_PMG_SBER_BER_DISABLED;
        }
    }

    /* reset sber info */
    sdh_pmg_handle_ptr->var.sber_byte = SDH_PMG_SBER_BYTE_B1;
   



    PMC_RETURN(result);
} /* sdh_pmg_var_init */





/*******************************************************************************
* sdh_pmg_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an SDH_PMG block handle instance.
*
*
* INPUTS:
*   *sdh_pmg_handle_ptr         - pointer to SDH_PMG handle instance
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
PUBLIC void sdh_pmg_handle_init(sdh_pmg_handle_t *sdh_pmg_handle_ptr)
{
    PMC_ENTRY();

    sdh_pmg_var_init(sdh_pmg_handle_ptr);

    PMC_RETURN();
} /* sdh_pmg_handle_init */



/*******************************************************************************
* sdh_pmg_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Bring SDH_PMG blocks into configurable state 
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   energy_state         - Energy state to put SDH_PMG into
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_init(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                              pmc_energy_state_t   energy_state)
{
    UINT8   i;

    PMC_ENTRY();
  
    /* Check Arguments */
    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    

    switch (energy_state)
    {
        case PMC_ENERGY_STATE_RUNNING:
            /* Enable Top */
            sdh_pmg_field_TOP_SW_RST_set(NULL, sdh_pmg_handle_ptr, SDH_PMG_ACTIVE);

            for (i = 0; i < SDH_PMG_MAX_INDEX; i++)
            {
                sdh_pmg_prov_reset(sdh_pmg_handle_ptr, i, SDH_PMG_RESET);
            } /* end for */
            
            /* Enable interrupts to go to external pin */
            sdh_pmg_int_init(sdh_pmg_handle_ptr, SDH_PMG_ENABLE);

            break;

        case PMC_ENERGY_STATE_RESET:
            /* Disable Top */
            sdh_pmg_field_TOP_SW_RST_set(NULL, sdh_pmg_handle_ptr, SDH_PMG_RESET);
            
            /* Reset dynamic context */
            sdh_pmg_var_init(sdh_pmg_handle_ptr);
            
            /* Disable interrupts from going to external pin */
            sdh_pmg_int_init(sdh_pmg_handle_ptr, SDH_PMG_DISABLE);

            break;
        
        default:
            PMC_ASSERT(FALSE, SDH_PMG_ERR_INVALID_ARG, energy_state, 0);
            break;
    } /* end switch */





    PMC_RETURN(PMC_SUCCESS);
} /* sdh_pmg_init */




/*******************************************************************************
* sdh_pmg_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   SDH_PMG instance.
*
*
* INPUTS:
*   *sdh_pmg_handle      - pointer to sdh_pmg handle instance to be operated on
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
PUBLIC BOOL sdh_pmg_start_state_test(sdh_pmg_handle_t *sdh_pmg_handle)
{
    BOOL                start_state = TRUE;
    UINT8               i;

    /* Variable declaration */
    
    PMC_ENTRY();
    PMC_ASSERT(sdh_pmg_handle!=NULL, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    
    
    /* Traverse through all indices to find one that has been allocated */ 
    i = 0;
    while ((i < SDH_PMG_MAX_INDEX) && (TRUE == start_state))
    {
        /* Resource has been allocated: SDH_PMG is not in start state */
        if (sdh_pmg_handle->var.mode[i] != LAST_SDH_PMG_STS_MODE)
        {
            start_state = FALSE;
        }
        
        i++;
    } /* end while */


    PMC_RETURN(start_state);
    
} /* sdh_pmg_start_state_test */










/*******************************************************************************
* sdh_pmg_cfg_mode
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   oc768_mode           - True to configure for OC768, false to configure for 
*                          OC192
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_cfg_mode(sdh_pmg_handle_t      *sdh_pmg_handle_ptr,
                                  BOOL                   oc768_mode)
{
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    

    sdh_pmg_field_OC_768_MODE_set(NULL, sdh_pmg_handle_ptr, oc768_mode);

    PMC_RETURN(PMC_SUCCESS);
} /* sdh_pmg_cfg_mode */

/*******************************************************************************
* sdh_pmg_cfg_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - link index in slice
*
* OUTPUTS:
*  *mode_ptr             - SDH PMG mode associated to link
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void sdh_pmg_cfg_mode_get(sdh_pmg_handle_t      *sdh_pmg_handle_ptr,
                                      INT32                  index,
                                      sdh_pmg_sts_mode_t    *mode_ptr)
{
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    
    if (0x1 == sdh_pmg_field_OC_768_MODE_get(NULL, sdh_pmg_handle_ptr))
    {
        *mode_ptr = SDH_PMG_STS_MODE_40G_STS768;
    }
    else
    {
        *mode_ptr = SDH_PMG_STS_MODE_10G_STS192;
    }
    
    PMC_RETURN();
} /* sdh_pmg_cfg_mode_get */


/*******************************************************************************
* sdh_pmg_prov_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Reset SDH PMG for a given link.   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Index of slice within sdh_pmg (0..3)
*   reset                - True to put slice into reset
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_prov_reset(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                                    UINT32               index,
                                    BOOL                 reset)
{
    
    PMC_ENTRY();
  
    /* Check Arguments */
    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
   

    /* Reset = 1 */
    sdh_pmg_field_CH_SBER_U_SW_RST_set(NULL, sdh_pmg_handle_ptr, index, reset);
    sdh_pmg_field_CH_RTTP_U_SW_RST_set(NULL, sdh_pmg_handle_ptr, index, reset);
    /* RRMP_U_AGG_LVL must be set before RRMP is brought out of reset */
    sdh_pmg_field_CH_RRMP_U_SW_RST_set(NULL, sdh_pmg_handle_ptr, index, reset);

    /* Powered Down = 1 */
    sdh_pmg_field_CH_LOW_PWR_set(NULL, sdh_pmg_handle_ptr, index, reset);
           

    PMC_RETURN(PMC_SUCCESS);
} /* sdh_pmg_prov_reset */

/*******************************************************************************
* sdh_pmg_prov_status_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns SDH PMG provisioning status for a given link. 
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Index of slice within sdh_pmg (0..3)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8   - TRUE if SDH_PMG link is provisioned
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 sdh_pmg_prov_status_get(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                                     UINT32               index)
{
    BOOL8 result = FALSE;
    PMC_ENTRY();
  
    /* Check Arguments */
    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
   
    /* if index is out of reset */ 
    if (0 == sdh_pmg_field_CH_SBER_U_SW_RST_get(NULL, sdh_pmg_handle_ptr, index) &&
        0 == sdh_pmg_field_CH_RTTP_U_SW_RST_get(NULL, sdh_pmg_handle_ptr, index) &&
        0 == sdh_pmg_field_CH_RRMP_U_SW_RST_get(NULL, sdh_pmg_handle_ptr, index) && 
        0 == sdh_pmg_field_CH_LOW_PWR_get(NULL, sdh_pmg_handle_ptr, index))
    {
        result = TRUE;
    }
           
    PMC_RETURN(result);
} /* sdh_pmg_prov_status_get */



/*******************************************************************************
* sdh_pmg_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   mode                 - STS mode of this sdh_pmg (OC768 or OC192)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_prov(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                              UINT32               index,
                              sdh_pmg_sts_mode_t   mode)
{
    PMC_ERROR           result = PMC_SUCCESS;
    BOOL                oc768_mode;
    UINT8               start_index, end_index;
    UINT8               i;
    
    PMC_ENTRY();

    /* Check Arguments */
    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }

   
    /* Configure link */
    if (PMC_SUCCESS == result)
    {
        if (sdh_pmg_is_sts_768(mode))
        {
            oc768_mode = TRUE;
            start_index = 0;
            end_index   = SDH_PMG_MAX_INDEX;

        } else {
            oc768_mode = FALSE;
            start_index = index;
            end_index   = start_index + 1;
        }
                
        result = sdh_pmg_cfg_mode(sdh_pmg_handle_ptr, oc768_mode);
        
    }


    /* Resets must be unset after mode config */
    for (i = start_index; (PMC_SUCCESS == result) && (i < end_index); i++)
    {
        result = sdh_pmg_prov_reset(sdh_pmg_handle_ptr, i, SDH_PMG_ACTIVE);
    }
    
    /* Update internal data structures */
    for (i = start_index; (PMC_SUCCESS == result) && (i < end_index); i++)
    {
        sdh_pmg_handle_ptr->var.mode[i] = mode;
    }
    
    /* Turn on consequential actions for OC-192 */
    if ((PMC_SUCCESS == result) && !sdh_pmg_is_sts_768(mode))
    {
        result = sdh_pmg_pn11_ins(sdh_pmg_handle_ptr, index, SDH_PMG_PN11_INSERT_LOF, SDH_PMG_ENABLE);
    }


    PMC_RETURN(result);
} /* sdh_pmg_prov */

/*******************************************************************************
* sdh_pmg_unprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_unprov(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                                UINT32               index)
{
    PMC_ERROR            result = PMC_SUCCESS;
    UINT8                start_index, end_index;
    UINT8                i;
    sdh_pmg_sts_mode_t   mode;

    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    
    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }
    
    if (LAST_SDH_PMG_STS_MODE == mode)
    {
        PMC_RETURN(SDH_PMG_ERR_NOT_PROVISIONED);
    }

    if (PMC_SUCCESS == result)
    {
        result = sdh_pmg_range_get(sdh_pmg_handle_ptr,
                                   index,
                                   &start_index,
                                   &end_index);
    }



    /* Turn off consequential actions */
    if ((PMC_SUCCESS == result) && !sdh_pmg_is_sts_768(mode))
    {
        result = sdh_pmg_pn11_ins(sdh_pmg_handle_ptr, index, SDH_PMG_PN11_INSERT_LOF, SDH_PMG_DISABLE);
    }

    for (i = start_index; (PMC_SUCCESS == result) && (i < end_index); i++)
    {
        result = sdh_pmg_prov_reset(sdh_pmg_handle_ptr, i, SDH_PMG_RESET);
    }
    
    /* Update internal data structures */
    for (i = start_index; (PMC_SUCCESS == result) && (i < end_index); i++)
    {
        sdh_pmg_handle_ptr->var.mode[i] = LAST_SDH_PMG_STS_MODE;
    }

    PMC_RETURN(result);
} /* sdh_pmg_unprov */

/*******************************************************************************
* sdh_pmg_cleanup
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Cleanup SDH PMG register space.
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   slice_start_index    - Current block start index
*                          link index that are out out slice index 
*                          range will be ignored.
*   num_link_to_clean    - Number of links to cleanup
*   links                - List of links to cleanup
*
* OUTPUTS:
*  None.
*
* RETURNS:
*  None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC void sdh_pmg_cleanup(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                            UINT32                slice_start_index,
                            UINT32                num_link_to_clean,
                            UINT32               *links)
{
    UINT8                index;
    UINT32 itr;

    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    
    for (itr = 0; itr < num_link_to_clean; itr += 1)
    {        
        if (slice_start_index > links[itr])
        {
            continue;
        }

        if (links[itr] >= (slice_start_index + SDH_PMG_MAX_INDEX))
        {
            break;
        }
            
        index = links[itr] % SDH_PMG_MAX_INDEX;

        /* disable channel */
        (void)sdh_pmg_chnl_en(sdh_pmg_handle_ptr, index, FALSE);

        /* Turn off consequential actions */
        (void)sdh_pmg_pn11_ins(sdh_pmg_handle_ptr, index, SDH_PMG_PN11_INSERT_LOF, SDH_PMG_DISABLE);
        
        /* reset link sdh pmg block */
        (void)sdh_pmg_prov_reset(sdh_pmg_handle_ptr, index, SDH_PMG_RESET);
    
        /* Update internal data structures */        
        sdh_pmg_handle_ptr->var.mode[index] = LAST_SDH_PMG_STS_MODE;
    }

    PMC_RETURN();
} /* sdh_pmg_cleanup */

/*******************************************************************************
* sdh_pmg_chnl_en
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Index of slice within sdh_pmg (0..3)
*   enable               - True to enable this slice
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_chnl_en(sdh_pmg_handle_t      *sdh_pmg_handle_ptr,
                                 UINT32                 index,
                                 BOOL                   enable)
{
    PMC_ENTRY();
  
    /* Check Arguments */
    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    
    /* Enable ASCLK needs to be set before channls are enabled */ 
    sdh_pmg_field_RRMP_U_ASCLK_set(NULL, sdh_pmg_handle_ptr, enable);
    /* Enable/disable the channel */
    sdh_pmg_field_CHNL_EN_set(NULL, sdh_pmg_handle_ptr, index, enable);


    PMC_RETURN(PMC_SUCCESS);
} /* sdh_pmg_chnl_en */




/*******************************************************************************
* sdh_pmg_activate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Activate or deactivates a given link/block in the sdh_pmg 
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_activate(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                                  UINT32               index)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT8               start_index, end_index;
    UINT8               i;
    sdh_pmg_sts_mode_t  mode;
    
    PMC_ENTRY();
    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    
    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }

    if (LAST_SDH_PMG_STS_MODE == mode)
    {
        PMC_RETURN(SDH_PMG_ERR_NOT_PROVISIONED);
    }


    if (sdh_pmg_is_sts_768(mode))
    {
        /* SDH_PMG in 40G mode only Channel 3 needs to be enabled */
        start_index = 3;
        end_index   = SDH_PMG_MAX_INDEX;

    } else {
        start_index = index;
        end_index   = start_index + 1;
    }
    
    
    for (i = start_index; (PMC_SUCCESS == result) && (i < end_index); i++)
    {
        result = sdh_pmg_chnl_en(sdh_pmg_handle_ptr, i, TRUE);
    }

    PMC_RETURN(result);
} /* sdh_pmg_activate */


/*******************************************************************************
* sdh_pmg_deactivate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_deactivate(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                                    UINT32               index)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT8               start_index, end_index;
    UINT8               i;
    sdh_pmg_sts_mode_t  mode;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    
    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }
  
    if (LAST_SDH_PMG_STS_MODE == mode)
    {
        PMC_RETURN(SDH_PMG_ERR_NOT_PROVISIONED);
    }




    if (PMC_SUCCESS == result)
    {
        result = sdh_pmg_range_get(sdh_pmg_handle_ptr,
                                   index,
                                   &start_index,
                                   &end_index);
    }


    for (i = start_index; (PMC_SUCCESS == result) && (i < end_index); i++)
    {
        result = sdh_pmg_chnl_en(sdh_pmg_handle_ptr, index, FALSE);
    }

    PMC_RETURN(result);
} /* sdh_pmg_deactivate */




/*******************************************************************************
* FUNCTION: sdh_pmg_pn11_ins()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures if PN11 is inserted into the datapath when an LOS or LOF occurs.
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   lof_los              - Configure insertion PN11 on LOF and/or LOS
*   ins_enable           - Enable or disable PN11 insertion
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_pn11_ins(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                  UINT32                    index,
                                  sdh_pmg_pn11_insert_t     lof_los,
                                  BOOL                      ins_enable)
{
    PMC_ERROR                           result = PMC_SUCCESS;    
    UINT8                               start_index, end_index;
    UINT8                               i;
    sdh_pmg_sts_mode_t                  mode;

     
    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    PMC_ASSERT(lof_los < LAST_SDH_PMG_PN11_INSERT, SDH_PMG_ERR_INVALID_ARG, lof_los, LAST_SDH_PMG_PN11_INSERT);
    
    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }

    if (LAST_SDH_PMG_STS_MODE == mode)
    {
        PMC_RETURN(SDH_PMG_ERR_NOT_PROVISIONED);
    }


    if (PMC_SUCCESS == result)
    {
        result = sdh_pmg_range_get(sdh_pmg_handle_ptr,
                                   index,
                                   &start_index,
                                   &end_index);
    }

    
    
    for (i = start_index; (i < end_index) && (PMC_SUCCESS == result); i++)
    {
         switch (lof_los)
         {
             case SDH_PMG_PN11_INSERT_LOF:
                 sdh_pmg_field_LOF_PN11_INSERT_set(NULL, sdh_pmg_handle_ptr, i, ins_enable);
                 break;
             
             case SDH_PMG_PN11_INSERT_LOS:
                 sdh_pmg_field_LOS_PN11_INSERT_set(NULL, sdh_pmg_handle_ptr, i, ins_enable);
                 break;
             
             default:
                 break;
         } /* end switch */
    } /* end for */ 


    PMC_RETURN(result);
} /* sdh_pmg_pn11_ins */




/*******************************************************************************
* FUNCTION: sdh_pmg_pn11_ins_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if PN11 is inserted into the datapath when an LOS or LOF occurs.
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   lof_los              - Configure insertion PN11 on LOF and/or LOS
*
* OUTPUTS:
*  *ins_enable_ptr       - PN11 insertion is enabled or disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_pn11_ins_get(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                      UINT32                    index,
                                      sdh_pmg_pn11_insert_t     lof_los,
                                      BOOL                     *ins_enable_ptr)
{
    PMC_ERROR                           result = PMC_SUCCESS;    
    sdh_pmg_sts_mode_t                  mode;

     
    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    PMC_ASSERT(lof_los < LAST_SDH_PMG_PN11_INSERT, SDH_PMG_ERR_INVALID_ARG, lof_los, LAST_SDH_PMG_PN11_INSERT);
    
    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }

    if (LAST_SDH_PMG_STS_MODE == mode)
    {
        PMC_RETURN(SDH_PMG_ERR_NOT_PROVISIONED);
    }
    
    if (NULL == ins_enable_ptr)
    {
        PMC_RETURN(SDH_PMG_ERR_NULL_HANDLE);
    }



    switch (lof_los)
    {
        case SDH_PMG_PN11_INSERT_LOF:
            *ins_enable_ptr = sdh_pmg_field_LOF_PN11_INSERT_get(NULL, sdh_pmg_handle_ptr, index);
            break;
         
        case SDH_PMG_PN11_INSERT_LOS:
            *ins_enable_ptr = sdh_pmg_field_LOS_PN11_INSERT_get(NULL, sdh_pmg_handle_ptr, index);
            break;
         
        default:
            break;
    } /* end switch */


    PMC_RETURN(result);
} /* sdh_pmg_pn11_ins_get */




/*******************************************************************************
* FUNCTION: sdh_pmg_oof_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Force an Out Of Frame into the next framing boundary.
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_oof_force(sdh_pmg_handle_t    *sdh_pmg_handle_ptr,
                                   UINT32               index)
{
    PMC_ERROR                           result = PMC_SUCCESS;    
    UINT8                               rrmp_index;
    sdh_pmg_sts_mode_t                  mode;

     
    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    
    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }

    if (LAST_SDH_PMG_STS_MODE == mode)
    {
        PMC_RETURN(SDH_PMG_ERR_NOT_PROVISIONED);
    }



    
    rrmp_index = sdh_pmg_rrmp_index_get(sdh_pmg_handle_ptr, index);
    
    rrmp_field_FOOF_set(NULL, sdh_pmg_handle_ptr, rrmp_index, SDH_PMG_ENABLE);


    PMC_RETURN(result);
} /* sdh_pmg_oof_force */



/*******************************************************************************
* FUNCTION: sdh_pmg_rrmp_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the RRMP:
*    - alarm behavriour
*    - overhead descrambling behaviour
*    - framing behaviour
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   type                 - RRMP config type
*  *rrmp_cfg_ptr         - Configuration structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_rrmp_cfg(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                  UINT32                    index,
                                  sdh_pmg_rrmp_cfg_type_t   type,
                                  void                     *rrmp_cfg_ptr)
{
    PMC_ERROR                           result = PMC_SUCCESS;    
    UINT8                               rrmp_index;
    sdh_pmg_rrmp_alarm_cfg_t           *alarm_cfg_ptr;
    sdh_pmg_rrmp_scramble_cfg_t        *scramble_cfg_ptr;
    sdh_pmg_rrmp_framing_cfg_t         *framing_cfg_ptr;
    UINT32                              mask;
    sdh_pmg_sts_mode_t                  mode;

     
    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != rrmp_cfg_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    
    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }

    if (LAST_SDH_PMG_STS_MODE == mode)
    {
        PMC_RETURN(SDH_PMG_ERR_NOT_PROVISIONED);
    }



    rrmp_index = sdh_pmg_rrmp_index_get(sdh_pmg_handle_ptr, index);
    
    
    switch (type)
    {
      case SDH_PMG_RRMP_CFG_TYPE_ALARM:
        alarm_cfg_ptr = (sdh_pmg_rrmp_alarm_cfg_t*)rrmp_cfg_ptr;
        mask = alarm_cfg_ptr->mask;

        if (mask & SDH_PMG_RRMP_ALARM_CFG_MASK_EXTENDED_APS)
        {
            rrmp_field_EXTAPSDIS_set(NULL, sdh_pmg_handle_ptr, rrmp_index, alarm_cfg_ptr->extended_APS_en);
        }

        if (mask & SDH_PMG_RRMP_ALARM_CFG_MASK_APS_ALGO)
        {
            rrmp_field_APSUNST_set(NULL, sdh_pmg_handle_ptr, rrmp_index, alarm_cfg_ptr->APS_algo);
        }

        if (mask & SDH_PMG_RRMP_ALARM_CFG_MASK_LRDI_ALGO)
        {
            rrmp_field_LRDI3_set(NULL, sdh_pmg_handle_ptr, rrmp_index, alarm_cfg_ptr->LRDI_algo);
        }

        if (mask & SDH_PMG_RRMP_ALARM_CFG_MASK_LAIS_ALGO)
        {
            rrmp_field_LAIS3_set(NULL, sdh_pmg_handle_ptr, rrmp_index, alarm_cfg_ptr->LAIS_algo);
        }

        if (mask & SDH_PMG_RRMP_ALARM_CFG_MASK_LOS_SHORT)
        {
            rrmp_field_LOSSPEN_set(NULL, sdh_pmg_handle_ptr, rrmp_index, alarm_cfg_ptr->LOS_short_en);
        }

        if (mask & SDH_PMG_RRMP_ALARM_CFG_MASK_LREI_ALGO)
        {
            rrmp_field_LREIACCBLK_set(NULL, sdh_pmg_handle_ptr, rrmp_index, alarm_cfg_ptr->LREI_algo);
        }
        
        if (mask & SDH_PMG_RRMP_ALARM_CFG_MASK_SSM_FRAMES)
        {
            rrmp_field_THREE_FRMSSM_set(NULL, sdh_pmg_handle_ptr, rrmp_index, alarm_cfg_ptr->SSM_frames);
        }
        
        if (mask & SDH_PMG_RRMP_ALARM_CFG_MASK_LBIP_ACCUM)
        {
            rrmp_field_LBIPEACCBLK_set(NULL, sdh_pmg_handle_ptr, rrmp_index, alarm_cfg_ptr->LBIP_accum);
        }
        
        if (mask & SDH_PMG_RRMP_ALARM_CFG_MASK_SBIP_ACCUM)
        {
            rrmp_field_SBIPEACCBLK_set(NULL, sdh_pmg_handle_ptr, rrmp_index, alarm_cfg_ptr->SBIP_accum);
        }
        if (mask & SDH_PMG_RRMP_ALARM_CFG_MASK_M0EN)
        {
            rrmp_field_M0EN_set(NULL, sdh_pmg_handle_ptr, rrmp_index, alarm_cfg_ptr->M0_en);
        } 
        break;
        
      case SDH_PMG_RRMP_CFG_TYPE_FRAMING:
        framing_cfg_ptr = (sdh_pmg_rrmp_framing_cfg_t*)rrmp_cfg_ptr;
        mask = framing_cfg_ptr->mask;
        
        if (mask & SDH_PMG_RRMP_FRAMING_CFG_MASK_ALGO)
        {
            rrmp_field_FRM768SEL_set(NULL, sdh_pmg_handle_ptr, rrmp_index, framing_cfg_ptr->algo);
        }
        break;
        
      case SDH_PMG_RRMP_CFG_TYPE_SCRAMBLE:
        scramble_cfg_ptr = (sdh_pmg_rrmp_scramble_cfg_t*)rrmp_cfg_ptr;
        mask = scramble_cfg_ptr->mask;
        
        if (mask & SDH_PMG_RRMP_SCRAMBLE_CFG_MASK_J0)
        {
            rrmp_field_DESCRAMJ0SEL_set(NULL, sdh_pmg_handle_ptr, rrmp_index, scramble_cfg_ptr->J0_descramble);
        }
        
        if (mask & SDH_PMG_RRMP_SCRAMBLE_CFG_MASK_A1A2)
        {
            rrmp_field_DESCRAMA1A2SEL_set(NULL, sdh_pmg_handle_ptr, rrmp_index, scramble_cfg_ptr->A1A2_descramble);
        }
        break;
    
      default:
        break;
    } /* end switch */
        

    PMC_RETURN(result);
} /* sdh_pmg_rrmp_cfg */



/*******************************************************************************
* FUNCTION: sdh_pmg_rrmp_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves RRMP configuration for:
*    - alarm behavriour
*    - overhead descrambling behaviour
*    - framing behaviour
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   type                 - RRMP config type
*
* OUTPUTS:
*  *rrmp_cfg_ptr         - Configuration structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_rrmp_cfg_get(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                      UINT32                    index,
                                      sdh_pmg_rrmp_cfg_type_t   type,
                                      void                     *rrmp_cfg_ptr)
{
    PMC_ERROR                           result = PMC_SUCCESS;    
    UINT8                               rrmp_index;
    sdh_pmg_rrmp_alarm_cfg_t           *alarm_cfg_ptr;
    sdh_pmg_rrmp_scramble_cfg_t        *scramble_cfg_ptr;
    sdh_pmg_rrmp_framing_cfg_t         *framing_cfg_ptr;
    sdh_pmg_sts_mode_t                  mode;

     
    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != rrmp_cfg_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    
    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }

    if (LAST_SDH_PMG_STS_MODE == mode)
    {
        PMC_RETURN(SDH_PMG_ERR_NOT_PROVISIONED);
    }



    rrmp_index = sdh_pmg_rrmp_index_get(sdh_pmg_handle_ptr, index);
    
    
    switch (type)
    {
      case SDH_PMG_RRMP_CFG_TYPE_ALARM:
        alarm_cfg_ptr = (sdh_pmg_rrmp_alarm_cfg_t*)rrmp_cfg_ptr;
        alarm_cfg_ptr->mask = SDH_PMG_RRMP_ALARM_CFG_MASK_EXTENDED_APS | 
                              SDH_PMG_RRMP_ALARM_CFG_MASK_APS_ALGO     | 
                              SDH_PMG_RRMP_ALARM_CFG_MASK_LRDI_ALGO    | 
                              SDH_PMG_RRMP_ALARM_CFG_MASK_LAIS_ALGO    | 
                              SDH_PMG_RRMP_ALARM_CFG_MASK_LOS_SHORT    | 
                              SDH_PMG_RRMP_ALARM_CFG_MASK_LREI_ALGO    | 
                              SDH_PMG_RRMP_ALARM_CFG_MASK_SSM_FRAMES   | 
                              SDH_PMG_RRMP_ALARM_CFG_MASK_LBIP_ACCUM   | 
                              SDH_PMG_RRMP_ALARM_CFG_MASK_SBIP_ACCUM   |
                              SDH_PMG_RRMP_ALARM_CFG_MASK_M0EN;

        alarm_cfg_ptr->extended_APS_en = rrmp_field_EXTAPSDIS_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
        alarm_cfg_ptr->APS_algo        = (sdh_pmg_rrmp_aps_algo_t) rrmp_field_APSUNST_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
        alarm_cfg_ptr->LRDI_algo       = rrmp_field_LRDI3_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
        alarm_cfg_ptr->LAIS_algo       = rrmp_field_LAIS3_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
        alarm_cfg_ptr->LOS_short_en    = rrmp_field_LOSSPEN_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
        alarm_cfg_ptr->LREI_algo       = rrmp_field_LREIACCBLK_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
        alarm_cfg_ptr->SSM_frames      = rrmp_field_THREE_FRMSSM_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
        alarm_cfg_ptr->LBIP_accum      = rrmp_field_LBIPEACCBLK_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
        alarm_cfg_ptr->SBIP_accum      = rrmp_field_SBIPEACCBLK_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
        alarm_cfg_ptr->M0_en           = rrmp_field_M0EN_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
        break;
        
      case SDH_PMG_RRMP_CFG_TYPE_FRAMING:
        framing_cfg_ptr = (sdh_pmg_rrmp_framing_cfg_t*)rrmp_cfg_ptr;
        framing_cfg_ptr->mask = SDH_PMG_RRMP_FRAMING_CFG_MASK_ALGO;
        
        framing_cfg_ptr->algo = rrmp_field_FRM768SEL_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
        break;
        
      case SDH_PMG_RRMP_CFG_TYPE_SCRAMBLE:
        scramble_cfg_ptr = (sdh_pmg_rrmp_scramble_cfg_t*)rrmp_cfg_ptr;
        scramble_cfg_ptr->mask = SDH_PMG_RRMP_SCRAMBLE_CFG_MASK_J0 |
                                 SDH_PMG_RRMP_SCRAMBLE_CFG_MASK_A1A2;
        
        scramble_cfg_ptr->J0_descramble     = rrmp_field_DESCRAMJ0SEL_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
        scramble_cfg_ptr->A1A2_descramble   = rrmp_field_DESCRAMA1A2SEL_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
        break;
    
      default:
        break;
    } /* end switch */
        

    PMC_RETURN(result);
} /* sdh_pmg_rrmp_cfg_get */


/*******************************************************************************
* FUNCTION: sdh_pmg_rrmp_byte_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves RRMP extracted bytes
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   type                 - Byte to extract
*
* OUTPUTS:
*   byte_ptr          - Extracted byte
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_rrmp_byte_get(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                       UINT32                    index,
                                       sdh_pmg_rrmp_byte_type_t  type,
                                       UINT8                    *byte_ptr)
{
    PMC_ERROR                           result = PMC_SUCCESS;    
    UINT8                               rrmp_index;
    sdh_pmg_sts_mode_t                  mode;

     
    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    PMC_ASSERT(type < LAST_SDH_PMG_RRMP_BYTE_TYPE, SDH_PMG_ERR_INVALID_ARG, type, LAST_SDH_PMG_RRMP_BYTE_TYPE);
    PMC_ASSERT(NULL != byte_ptr, SDH_PMG_ERR_INVALID_ARG, 0, 0);
    
    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }

    if (LAST_SDH_PMG_STS_MODE == mode)
    {
        PMC_RETURN(SDH_PMG_ERR_NOT_PROVISIONED);
    }



    
    
    if (PMC_SUCCESS == result)
    {
        rrmp_index = index * SDH_PMG_RRMP_SLICES_PER_INDEX;
         switch (type)
         {
             case SDH_PMG_RRMP_BYTE_TYPE_J0:
                 *byte_ptr = rrmp_field_J0V_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
                 break;
             
             case SDH_PMG_RRMP_BYTE_TYPE_SSM:
                 *byte_ptr = rrmp_field_SSMV_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
                 break;
             
             case SDH_PMG_RRMP_BYTE_TYPE_K2:
                 *byte_ptr = rrmp_field_K2V_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
                 break;
             
             case SDH_PMG_RRMP_BYTE_TYPE_K1:
                 *byte_ptr = rrmp_field_K1V_get(NULL, sdh_pmg_handle_ptr, rrmp_index);
                 break;
         
             default:
                 break;
         } /* end switch */
         
    }


    PMC_RETURN(result);
} /* sdh_pmg_rrmp_byte_get */




/*******************************************************************************
* FUNCTION: sdh_pmg_sber_byte_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configures the byte (B1 or B2) that the SDH SBER block will use to detect
*    signal degrade and signal fail. The SBER slice can only monitor either the
*    B1 or B2 byte at one time.
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   byte                 - Sets the byte that the BER will be calculated from
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_sber_byte_cfg(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                       sdh_pmg_sber_byte_t       byte)

{
    PMC_ERROR                           result = PMC_SUCCESS;    

     
    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(byte < LAST_SDH_PMG_SBER_BYTE, SDH_PMG_ERR_INVALID_ARG, byte, LAST_SDH_PMG_SBER_BYTE);
    



    /* Set BER calculation from B1 or B2 byte */
    sdh_pmg_field_LBIP_BER_SEL_set(NULL, sdh_pmg_handle_ptr, byte);

    sdh_pmg_handle_ptr->var.sber_byte = byte;




    PMC_RETURN(result);
} /* sdh_pmg_sber_byte_cfg */


/*******************************************************************************
* FUNCTION: sdh_pmg_sber_byte_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Retrieves the configured byte that the SDH SBER block will use to detect
*    signal degrade and signal failure.
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*
* OUTPUTS:
*   byte_ptr             - Gets the byte that the BER will be calculated from
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_sber_byte_get(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                       sdh_pmg_sber_byte_t      *byte_ptr)

{
    PMC_ERROR                           result = PMC_SUCCESS;    

     
    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    



    /* Get BER calculation from B1 or B2 byte */
    *byte_ptr = sdh_pmg_handle_ptr->var.sber_byte;




    PMC_RETURN(result);
} /* sdh_pmg_sber_byte_get */

/*******************************************************************************
* FUNCTION: sdh_pmg_sber_ber_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves configured BER rate for SD or SF.
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   alarm                - Enable/disable SD or SF alarm for given BER
*
* OUTPUTS:
*   ber_ptr              - configured SD or SF bit error rate, or disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_sber_ber_get(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                      UINT32                    index,
                                      sdh_pmg_sber_alarm_t      alarm,
                                      sdh_pmg_sber_ber_t       *ber_ptr)

{
    PMC_ERROR                           result = PMC_SUCCESS;    
    sdh_pmg_sts_mode_t                  mode;

     
    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    PMC_ASSERT(alarm < LAST_SDH_PMG_SBER_ALARM, SDH_PMG_ERR_INVALID_ARG, alarm, LAST_SDH_PMG_SBER_ALARM);
    

    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }
    if (NULL == ber_ptr)
    {
        PMC_RETURN(SDH_PMG_ERR_NULL_HANDLE);
    }


    /* Get configured BER */
    *ber_ptr = sdh_pmg_handle_ptr->var.sber_ber[alarm][index];




    PMC_RETURN(result);
} /* sdh_pmg_sber_ber_get */

/*******************************************************************************
* FUNCTION: sdh_pmg_sber_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables SD/SF monitoring and sets the threshold, or disables monitoring.
*   The byte from which the BER is calculated must be configured first
*   with digi_cbr_sdh_sber_byte_cfg().
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   alarm                - Enable/disable SD or SF alarm for given BER
*   ber                  - Bit error rate, or disable monitoring
*                          Valid inputs are:\n
*                          B1 STS-768: SDH_PMG_SBER_BER_1E_05 to SDH_PMG_SBER_BER_1E_12 \n
*                          B1 STS-192: SDH_PMG_SBER_BER_1E_05 to SDH_PMG_SBER_BER_1E_11 \n    
*                          B2 STS-768: SDH_PMG_SBER_BER_1E_04 to SDH_PMG_SBER_BER_1E_11 \n
*                          B2 STS-192: SDH_PMG_SBER_BER_1E_04 to SDH_PMG_SBER_BER_1E_11 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_sber_enable(sdh_pmg_handle_t             *sdh_pmg_handle_ptr,
                                     UINT32                        index,
                                     sdh_pmg_sber_alarm_t          alarm,
                                     sdh_pmg_sber_ber_t            ber)
{
    PMC_ERROR               result = PMC_SUCCESS;    
    UINT8                   sber_index;
    sdh_pmg_sts_mode_t      mode;
    sdh_pmg_sber_byte_t     byte;
    UINT8                   sber_sts;

     
    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    PMC_ASSERT(ber < LAST_SDH_PMG_SBER_BER, SDH_PMG_ERR_INVALID_ARG, ber, LAST_SDH_PMG_SBER_BER);
    PMC_ASSERT(alarm < LAST_SDH_PMG_SBER_ALARM, SDH_PMG_ERR_INVALID_ARG, alarm, LAST_SDH_PMG_SBER_ALARM);

    /* Byte is set to default as B1 */
    byte = sdh_pmg_handle_ptr->var.sber_byte;
    PMC_ASSERT(byte < LAST_SDH_PMG_SBER_BYTE, SDH_PMG_ERR_INVALID_ARG, byte, LAST_SDH_PMG_SBER_BYTE);
    
    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }

    if (LAST_SDH_PMG_STS_MODE == mode)
    {
        PMC_RETURN(SDH_PMG_ERR_NOT_PROVISIONED);
    }


    /* Check BER Bounds if we're not disabling */
    if (SDH_PMG_SBER_BER_DISABLED != ber)
    {
        if (SDH_PMG_SBER_BYTE_B1 == byte)
        {
            if (sdh_pmg_is_sts_768(mode))
            {
                /* B1, STS 768 */
                if ((ber < SDH_PMG_SBER_BER_1E_05) || (ber > SDH_PMG_SBER_BER_1E_12))
                {
                    PMC_RETURN(SDH_PMG_ERR_SBER_INVALID_BER);
                }
            } else {
                /* B1, STS 192 */
                if ((ber < SDH_PMG_SBER_BER_1E_05) || (ber > SDH_PMG_SBER_BER_1E_11))
                {
                    PMC_RETURN(SDH_PMG_ERR_SBER_INVALID_BER);
                }
            }
        } else {
            /* B2, STS 192/768 */
            if ((ber < SDH_PMG_SBER_BER_1E_03) || (ber > SDH_PMG_SBER_BER_1E_11))
            {
                PMC_RETURN(SDH_PMG_ERR_SBER_INVALID_BER);
            }
        }
    }


    /* Store configured BER */
    sdh_pmg_handle_ptr->var.sber_ber[alarm][index] = ber;


    
    if (sdh_pmg_is_sts_768(mode))
    {
        sber_sts    = SDH_PMG_SBER_STS_768;

    } else {
        sber_sts    = SDH_PMG_SBER_STS_192;
    }
    


    if (PMC_SUCCESS == result)
    {
        sber_index = sdh_pmg_sber_index_get(sdh_pmg_handle_ptr, index);    
    }


    if (SDH_PMG_SBER_ALARM_SD == alarm)
    {
        /* SD Config */
        if (SDH_PMG_SBER_BER_DISABLED == ber)
        {
            sber_indirect_SD_PATH_MONING_EN_buffer_t    enable_buffer;
            sber_indirect_SD_PATH_MONING_EN_buffer_init(&enable_buffer);   
           


            /* Disable SD */
            enable_buffer.base.mask[0] = SBER_INDIRECT_REG_SD_PATH_MONING_EN_BIT_SD_EN_MSK;
            enable_buffer.base.data[0] = SDH_PMG_DISABLE << SBER_INDIRECT_REG_SD_PATH_MONING_EN_BIT_SD_EN_OFF;

            /* Write to config indirect register */
            sber_indirect_SD_PATH_MONING_EN_buffer_apply(sdh_pmg_handle_ptr, sber_index, &enable_buffer);
            
        }
        else
        {
            sber_indirect_SD_PATH_MONING_EN_buffer_t                   enable_buffer;
            sber_indirect_SD_BERM_SET_TH_buffer_t                      set_buffer;
            sber_indirect_SD_BERM_CLEAR_TH_buffer_t                    clr_buffer;
            sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_t    ip1_buffer;
            sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_t    ip2_buffer;

            sber_indirect_SD_PATH_MONING_EN_buffer_init               (&enable_buffer);   
            sber_indirect_SD_BERM_SET_TH_buffer_init                  (&set_buffer);   
            sber_indirect_SD_BERM_CLEAR_TH_buffer_init                (&clr_buffer);   
            sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_init(&ip1_buffer);   
            sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_init(&ip2_buffer);   
            
            
            /* Enable SD */
            enable_buffer.base.mask[0] = SBER_INDIRECT_REG_SD_PATH_MONING_EN_BIT_SD_EN_MSK;
            enable_buffer.base.data[0] = SDH_PMG_ENABLE << SBER_INDIRECT_REG_SD_PATH_MONING_EN_BIT_SD_EN_OFF;
            
            set_buffer.base.mask[0] = SBER_INDIRECT_REG_SD_BERM_SET_TH_BIT_SDST_MSK;
            set_buffer.base.data[0] = sber_tbl[byte][sber_sts][ber].set_threshold << SBER_INDIRECT_REG_SD_BERM_SET_TH_BIT_SDST_OFF;
            
            clr_buffer.base.mask[0] = SBER_INDIRECT_REG_SD_BERM_CLEAR_TH_BIT_SDCT_MSK;
            clr_buffer.base.data[0] = sber_tbl[byte][sber_sts][ber].clr_threshold << SBER_INDIRECT_REG_SD_BERM_CLEAR_TH_BIT_SDCT_OFF;
            
            ip1_buffer.base.mask[0] = SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_1_BIT_SDIP_MSK;
            ip1_buffer.base.data[0] = sber_tbl[byte][sber_sts][ber].int_per_1     << SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_1_BIT_SDIP_OFF;
            
            ip2_buffer.base.mask[0] = SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_2_BIT_SDIP_MSK;
            ip2_buffer.base.data[0] = sber_tbl[byte][sber_sts][ber].int_per_2     << SBER_INDIRECT_REG_SD_BERM_SET_INTEGRATION_PERIOD_2_BIT_SDIP_OFF;
            
            /* Write to config indirect register */
            sber_indirect_SD_BERM_SET_TH_buffer_apply                   (sdh_pmg_handle_ptr, sber_index, &set_buffer);
            sber_indirect_SD_BERM_CLEAR_TH_buffer_apply                 (sdh_pmg_handle_ptr, sber_index, &clr_buffer);
            sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_1_buffer_apply (sdh_pmg_handle_ptr, sber_index, &ip1_buffer);
            sber_indirect_SD_BERM_SET_INTEGRATION_PERIOD_2_buffer_apply (sdh_pmg_handle_ptr, sber_index, &ip2_buffer);
            sber_indirect_SD_PATH_MONING_EN_buffer_apply                (sdh_pmg_handle_ptr, sber_index, &enable_buffer);
        } /* end if disable */
    } else {
        /* SF Config */
        if (SDH_PMG_SBER_BER_DISABLED == ber)
        {
            sber_indirect_SF_PATH_MONING_EN_buffer_t    enable_buffer;
            sber_indirect_SF_PATH_MONING_EN_buffer_init(&enable_buffer);   
            
            
            /* Disable SF */
            enable_buffer.base.mask[0] = SBER_INDIRECT_REG_SF_PATH_MONING_EN_BIT_SF_EN_MSK;
            enable_buffer.base.data[0] = SDH_PMG_DISABLE << SBER_INDIRECT_REG_SF_PATH_MONING_EN_BIT_SF_EN_OFF;

            /* Write to config indirect register */
            sber_indirect_SF_PATH_MONING_EN_buffer_apply(sdh_pmg_handle_ptr, sber_index, &enable_buffer);
            
        }
        else
        {
            sber_indirect_SF_PATH_MONING_EN_buffer_t                   enable_buffer;
            sber_indirect_SF_BERM_SET_TH_buffer_t                      set_buffer;
            sber_indirect_SF_BERM_CLEAR_TH_buffer_t                    clr_buffer;
            sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_t    ip1_buffer;
            sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_t    ip2_buffer;

            sber_indirect_SF_PATH_MONING_EN_buffer_init               (&enable_buffer);   
            sber_indirect_SF_BERM_SET_TH_buffer_init                  (&set_buffer);   
            sber_indirect_SF_BERM_CLEAR_TH_buffer_init                (&clr_buffer);   
            sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_init(&ip1_buffer);   
            sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_init(&ip2_buffer);   
            
            
            /* Enable SF */
            enable_buffer.base.mask[0] = SBER_INDIRECT_REG_SF_PATH_MONING_EN_BIT_SF_EN_MSK;
            enable_buffer.base.data[0] = SDH_PMG_ENABLE << SBER_INDIRECT_REG_SF_PATH_MONING_EN_BIT_SF_EN_OFF;
            
            set_buffer.base.mask[0] = SBER_INDIRECT_REG_SF_BERM_SET_TH_BIT_SFST_MSK;
            set_buffer.base.data[0] = sber_tbl[byte][sber_sts][ber].set_threshold << SBER_INDIRECT_REG_SF_BERM_SET_TH_BIT_SFST_OFF;
            
            clr_buffer.base.mask[0] = SBER_INDIRECT_REG_SF_BERM_CLEAR_TH_BIT_SFCT_MSK;
            clr_buffer.base.data[0] = sber_tbl[byte][sber_sts][ber].clr_threshold << SBER_INDIRECT_REG_SF_BERM_CLEAR_TH_BIT_SFCT_OFF;
            
            ip1_buffer.base.mask[0] = SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_1_BIT_SFIP_MSK;
            ip1_buffer.base.data[0] = sber_tbl[byte][sber_sts][ber].int_per_1     << SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_1_BIT_SFIP_OFF;
            
            ip2_buffer.base.mask[0] = SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_2_BIT_SFIP_MSK;
            ip2_buffer.base.data[0] = sber_tbl[byte][sber_sts][ber].int_per_2     << SBER_INDIRECT_REG_SF_BERM_SET_INTEGRATION_PERIOD_2_BIT_SFIP_OFF;
            
            /* Write to config indirect register */
            sber_indirect_SF_BERM_SET_TH_buffer_apply                   (sdh_pmg_handle_ptr, sber_index, &set_buffer);
            sber_indirect_SF_BERM_CLEAR_TH_buffer_apply                 (sdh_pmg_handle_ptr, sber_index, &clr_buffer);
            sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_1_buffer_apply (sdh_pmg_handle_ptr, sber_index, &ip1_buffer);
            sber_indirect_SF_BERM_SET_INTEGRATION_PERIOD_2_buffer_apply (sdh_pmg_handle_ptr, sber_index, &ip2_buffer);
            sber_indirect_SF_PATH_MONING_EN_buffer_apply                (sdh_pmg_handle_ptr, sber_index, &enable_buffer);
        } /* end if disable */
    } /* end if alarm type */

    PMC_RETURN(result);
} /* sdh_pmg_sber_enable */







/*******************************************************************************
* FUNCTION: sdh_pmg_rttp_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the parameters for tail trace message processing within the 
*   SDH_PMG. Currently length is the only modifiable parameter, but the
*   config structure is used to keep the API flexible as there are 
*   multiple parameters in the RTTP block. Also configures the algorithm to be
*   used.
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   rttp_cfg_ptr         - Config structure with length setting.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_rttp_cfg(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                  UINT32                    index,
                                  sdh_pmg_rttp_cfg_t       *rttp_cfg_ptr)
{
    PMC_ERROR                           result = PMC_SUCCESS;    
    sdh_pmg_sts_mode_t                  mode;
    rttp_indirect_TRACE_CFG_buffer_t    buffer;
    UINT8                               rttp_index;
    sdh_pmg_rttp_length_t               length;
    sdh_pmg_rttp_algo_t                 algo;
    BOOL8                               length16  = FALSE;
    BOOL8                               sync_crlf = FALSE;

     
    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != rttp_cfg_ptr, SDH_PMG_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);

    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }

    
    length = rttp_cfg_ptr->length;
    algo = rttp_cfg_ptr->algo;
    if ((SDH_PMG_RTTP_LENGTH_1_BYTE   != length) && 
        (SDH_PMG_RTTP_LENGTH_16_BYTES != length) &&
        (SDH_PMG_RTTP_LENGTH_64_BYTES != length))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_ARG);
    }
    
    

    if (PMC_SUCCESS == result)
    {
        rttp_index = sdh_pmg_rttp_index_get(sdh_pmg_handle_ptr, index);    
    }

    if (PMC_SUCCESS == result)
    {
        switch (length)
        {
            case SDH_PMG_RTTP_LENGTH_16_BYTES:
            case SDH_PMG_RTTP_LENGTH_64_BYTES:
                rttp_indirect_TRACE_CFG_buffer_init(&buffer);

                /* Set SYNC_CRLF field (ALGO 1 and 2 only)
                 * 0 - synchronizes when receiving a byte with its MSB set to logic 1
                 * 1 - synchronizes when receiving a CR/LF
                 *
                 * Only 64 byte trace messages will sync on CR/LF
                 */
                if (SDH_PMG_RTTP_LENGTH_64_BYTES == length)
                {
                    sync_crlf = TRUE;
                }
                buffer.base.mask[0] = RTTP_INDIRECT_REG_TRACE_CFG_BIT_SYNC_CRLF_MSK;
                buffer.base.data[0] = (sync_crlf) << RTTP_INDIRECT_REG_TRACE_CFG_BIT_SYNC_CRLF_OFF;

                /* Set LENGTH16 field (ALGO 1 and 2 only)
                 * 0 - trace message is 64 bytes
                 * 1 - trace message is 16 bytes
                 */
                if (SDH_PMG_RTTP_LENGTH_16_BYTES == length)
                {
                    length16 = TRUE;
                }

                buffer.base.mask[0] |= RTTP_INDIRECT_REG_TRACE_CFG_BIT_LENGTH16_MSK;
                buffer.base.data[0] |= length16 << RTTP_INDIRECT_REG_TRACE_CFG_BIT_LENGTH16_OFF;

                /* Configure length */
                rttp_indirect_TRACE_CFG_buffer_apply(sdh_pmg_handle_ptr,
                                                     rttp_index,
                                                     &buffer);
                 
                break;

            case SDH_PMG_RTTP_LENGTH_1_BYTE:
                break;

            default:
                break;
        } /* end switch */
       

        /* store rttp length */
        sdh_pmg_handle_ptr->var.rttp_len[index] = length;
    }


    if (SDH_PMG_RTTP_ALGO_DISABLED == algo)
    {
        /* Disable RTTP monitoring and clear internal config */
        if (PMC_SUCCESS == result)
        {
            result = sdh_pmg_rttp_algo_set(sdh_pmg_handle_ptr, index, SDH_PMG_RTTP_ALGO_DISABLED);
        } 

    }
    else
    {
        /* Disable RTTP first */
        if (PMC_SUCCESS == result)
        {
            result = sdh_pmg_rttp_algo_set(sdh_pmg_handle_ptr, index, SDH_PMG_RTTP_ALGO_DISABLED);
        } 
        
        /* Set correct algorithm and re-enable RTTP processing */
        if (PMC_SUCCESS == result)
        {
            result = sdh_pmg_rttp_algo_set(sdh_pmg_handle_ptr, index, algo);
        } 
    }

    PMC_RETURN(result);
} /* sdh_pmg_rttp_cfg */



/*******************************************************************************
* FUNCTION: sdh_pmg_rttp_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the configured parameters for tail trace message processing.
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*
* OUTPUTS:
*   rttp_cfg_ptr         - Configured structure with length setting.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_rttp_cfg_get(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                      UINT32                    index,
                                      sdh_pmg_rttp_cfg_t       *rttp_cfg_ptr)
{
    PMC_ERROR                           result = PMC_SUCCESS;    
    sdh_pmg_sts_mode_t                  mode;

     
    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != rttp_cfg_ptr, SDH_PMG_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);

    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }

    rttp_cfg_ptr->length = sdh_pmg_handle_ptr->var.rttp_len[index];
    rttp_cfg_ptr->algo = sdh_pmg_handle_ptr->var.rttp_algo[index];

    PMC_RETURN(result);
} /* sdh_pmg_rttp_cfg_get */



/*******************************************************************************
* FUNCTION: sdh_pmg_rttp_expect_trace_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    Configures the expected trace message.
*
*    sdh_pmg_rttp_len_cfg() is required to be called first to configure
*    the length of the trace message and the algorithm to be used.
*
*    16 byte trace messages will have a CRC inserted in the byte 16.
*    64 byte trace messages will have CR/LF inserted in byte 63 and 64.
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   *expect_trace_ptr    - pointer to trace message structure. Can be NULL
*                          if algo is SDH_PMG_RTTP_ALGO_DISABLED
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_rttp_expect_trace_set(sdh_pmg_handle_t      *sdh_pmg_handle_ptr,
                                               UINT32                 index,
                                               sdh_pmg_rttp_trace_t  *expect_trace_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;
    sdh_pmg_rttp_algo_t     algo;

     
    PMC_ENTRY();

    algo = sdh_pmg_handle_ptr->var.rttp_algo[index];

    /* Check arguments */
    if (PMC_SUCCESS == result)
    {
        result = sdh_pmg_rttp_arg_check(sdh_pmg_handle_ptr,
                                        index,
                                        algo,
                                        expect_trace_ptr);
    }

    if (LAST_SDH_PMG_RTTP_ALGO == algo)
    {
        /* Disable RTTP monitoring and clear internal config */
        if (PMC_SUCCESS == result)
        {
            sdh_pmg_handle_ptr->var.rttp_len[index] = LAST_SDH_PMG_RTTP_LENGTH;
        } 

    }
    else
    {
        /* Program new expected trace message */
        if (PMC_SUCCESS == result)
        {
            result = sdh_pmg_rttp_expect_trace_set_internal(sdh_pmg_handle_ptr, index, expect_trace_ptr);
        }
    }

    PMC_RETURN(result);
} /* sdh_pmg_rttp_expect_trace_set */


#ifdef CBRC_UT 
/*{{{ */ /* Unit Test Functions */

#define SDH_PMG_RTTP_UT_CRC_MIDDLE_OFFSET       7
#define SDH_PMG_RTTP_UT_CRC_FIRST_OFFSET        (SDH_PMG_RTTP_LENGTH_16_BYTES - 1)

#define SDH_PMG_RTTP_UT_CRLF_MIDDLE_OFFSET      40
#define SDH_PMG_RTTP_UT_CRLF_FIRST_OFFSET       (SDH_PMG_RTTP_LENGTH_64_BYTES - 2)
#define SDH_PMG_RTTP_UT_CRLF_SPLIT_OFFSET       (SDH_PMG_RTTP_LENGTH_64_BYTES - 1)


typedef enum {
    SDH_PMG_RTTP_UT_1_BYTE = 0,
    SDH_PMG_RTTP_UT_16_BYTES_CRC_MIDDLE,
    SDH_PMG_RTTP_UT_16_BYTES_CRC_LAST,
    SDH_PMG_RTTP_UT_16_BYTES_CRC_FIRST,
    SDH_PMG_RTTP_UT_64_BYTES_CRLF_MIDDLE,
    SDH_PMG_RTTP_UT_64_BYTES_CRLF_LAST,
    SDH_PMG_RTTP_UT_64_BYTES_CRLF_FIRST,
    SDH_PMG_RTTP_UT_64_BYTES_CRLF_SPLIT,

    LAST_SDH_PMG_RTTP_UT_COUNTER,
} sdh_pmg_rttp_ut_counter_t;


sdh_pmg_rttp_ut_counter_t  rttp_ut_counter;

static UINT8 rttp_ut_1_byte[1];
static UINT8 rttp_ut_64_bytes[64];
static UINT8 rttp_ut_16_bytes[16];



void sdh_pmg_rttp_ut_init_trace()
{
    UINT32  i;
    PMC_ENTRY();

    /* 1 byte trace message */
    rttp_ut_1_byte[0] = 1;



    /* 16 byte trace message */
    for (i = 0; i < SDH_PMG_RTTP_LENGTH_16_BYTES; i++)
    {
        rttp_ut_16_bytes[i] = i;
    }
    rttp_ut_16_bytes[15] = SDH_PMG_RTTP_ASCII_CRC_START; /* CRC in last byte */ 


    /* 64 byte trace message */
    for (i = 0; i < SDH_PMG_RTTP_LENGTH_64_BYTES; i++)
    {
        rttp_ut_64_bytes[i] = i;
    }
    rttp_ut_64_bytes[62] = SDH_PMG_RTTP_ASCII_CR; /* CR in last bytes */
    rttp_ut_64_bytes[63] = SDH_PMG_RTTP_ASCII_LF; /* LF in last bytes */

    PMC_RETURN();
}



void sdh_pmg_rttp_ut_buffer_fill(sdh_pmg_rttp_ut_counter_t  counter,
                                 UINT32                     index,
                                 UINT32                    *buffer)
{
    UINT32  src_index;
    PMC_ENTRY();


    switch (counter)
    {
        case SDH_PMG_RTTP_UT_1_BYTE:
            *buffer = rttp_ut_1_byte[0]; 
            break;
        
        case SDH_PMG_RTTP_UT_16_BYTES_CRC_MIDDLE:
            src_index = (index + SDH_PMG_RTTP_UT_CRC_MIDDLE_OFFSET) % SDH_PMG_RTTP_LENGTH_16_BYTES;
            *buffer = rttp_ut_16_bytes[src_index];
            break;
        
        case SDH_PMG_RTTP_UT_16_BYTES_CRC_LAST:             
            src_index = index;
            *buffer = rttp_ut_16_bytes[src_index];
            break;
    
        case SDH_PMG_RTTP_UT_16_BYTES_CRC_FIRST:             
            src_index = (index + SDH_PMG_RTTP_UT_CRC_FIRST_OFFSET) % SDH_PMG_RTTP_LENGTH_16_BYTES;
            *buffer = rttp_ut_16_bytes[src_index];
            break;
    
        case SDH_PMG_RTTP_UT_64_BYTES_CRLF_MIDDLE:             
            src_index = (index + SDH_PMG_RTTP_UT_CRLF_MIDDLE_OFFSET) % SDH_PMG_RTTP_LENGTH_64_BYTES;
            *buffer = rttp_ut_64_bytes[src_index];
            break;
    
        case SDH_PMG_RTTP_UT_64_BYTES_CRLF_LAST:             
            src_index = index;
            *buffer = rttp_ut_64_bytes[src_index];
            break;
    
        case SDH_PMG_RTTP_UT_64_BYTES_CRLF_FIRST:             
            src_index = (index + SDH_PMG_RTTP_UT_CRLF_FIRST_OFFSET) % SDH_PMG_RTTP_LENGTH_64_BYTES;
            *buffer = rttp_ut_64_bytes[src_index];
            break;
    
        case SDH_PMG_RTTP_UT_64_BYTES_CRLF_SPLIT:             
            src_index = (index + SDH_PMG_RTTP_UT_CRLF_SPLIT_OFFSET) % SDH_PMG_RTTP_LENGTH_64_BYTES;
            *buffer = rttp_ut_64_bytes[src_index];
            break;
    
        default:
            PMC_ASSERT(0, SDH_PMG_ERR_CODE_ASSERT, 0, 0);
            break;
    } /* end switch */
   

    PMC_RETURN();
}



PMC_ERROR sdh_pmg_rttp_ut_check_data(sdh_pmg_rttp_ut_counter_t  counter,
                                     sdh_pmg_rttp_trace_t      *trace_ptr,
                                     UINT8                     *tmp_trace_ptr)
{
    PMC_ERROR       rc = PMC_ERR_FAIL;
    void           *src_buffer;
    UINT32          i;

    PMC_ENTRY();

 
    switch (trace_ptr->size)
    {
        case SDH_PMG_RTTP_LENGTH_64_BYTES:
            src_buffer = rttp_ut_64_bytes; 
            break;
        
        case SDH_PMG_RTTP_LENGTH_16_BYTES:
            src_buffer = rttp_ut_16_bytes; 
            break;
        
        case SDH_PMG_RTTP_LENGTH_1_BYTE:             
            src_buffer = rttp_ut_1_byte; 
            break;
    
        default:
            PMC_ASSERT(0, SDH_PMG_ERR_CODE_ASSERT, trace_ptr->size, 0);
            break;
    } /* end switch */
    


    if (0 ==PMC_MEMCMP(trace_ptr->trace_msg, src_buffer, trace_ptr->size))
    {
        rc = PMC_SUCCESS;
    }

    /* Print tmp buffer */
    PMC_LOG_NOTRACE("==================================================================\n");
    PMC_LOG_NOTRACE("UT Counter:   %i\n", counter);
    PMC_LOG_NOTRACE("Tmp Buffer:   ");
    for (i = 0; i < trace_ptr->size; i++)
    {
        PMC_LOG_NOTRACE ("0x%02x ", tmp_trace_ptr[i]);
    } 
    PMC_LOG_NOTRACE("\n");

    /* Print trace_ptr (output to user) */
    PMC_LOG_NOTRACE("Trace Buffer: ");
    for (i = 0; i < trace_ptr->size; i++)
    {
        PMC_LOG_NOTRACE ("0x%02x ", trace_ptr->trace_msg[i]);
    } 
    PMC_LOG_NOTRACE("\n");
    PMC_LOG_NOTRACE("===================================================================\n");


    PMC_RETURN(rc);
}
/*}}}*/
#endif /* CBRC_UT */


/*******************************************************************************
* FUNCTION: sdh_pmg_rttp_trace_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Extracts trace message for a given index.
*   
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   trace_type           - Captured, accepted, or expected trace
*                          Trace message must be enabled to retrieve 
*                          the captured/accepted message. The expected
*                          message can always be retrieved.
*                          Accepted trace message is only available when using
*                          SDH_PMG_RTTP_ALGO_2.
*
* OUTPUTS:
*   trace_ptr            - Pointer to trace message structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_rttp_trace_get(sdh_pmg_handle_t           *sdh_pmg_handle_ptr,
                                        UINT32                      index,
                                        sdh_pmg_rttp_trace_type_t   trace_type,
                                        sdh_pmg_rttp_trace_t       *trace_ptr)
{
    PMC_ERROR                 result = PMC_SUCCESS;
    /* buffer to store extracted unaligned trace message locally */
    UINT8                     trace_msg_tmp_ptr[LAST_SDH_PMG_RTTP_LENGTH];
    UINT8                     cr_index, lf_index, crc_index;
    sdh_pmg_rttp_length_t     length;

    
    PMC_ENTRY();


#ifdef CBRC_UT 
/*{{{ */ /* Unit Test Code */
    static BOOL cbrc_ut_init = FALSE;

    /* Initialize test data once */
    if (FALSE == cbrc_ut_init)
    {
        cbrc_ut_init = TRUE;
        rttp_ut_counter = SDH_PMG_RTTP_UT_1_BYTE;
        sdh_pmg_rttp_ut_init_trace();
    }
    
    PMC_ASSERT(rttp_ut_counter < LAST_SDH_PMG_RTTP_UT_COUNTER, SDH_PMG_ERR_CODE_ASSERT, 0, 0);
/*}}}*/
#endif /* CBRC_UT */

     
    result = sdh_pmg_rttp_trace_get_arg_check(sdh_pmg_handle_ptr,
                                              index,
                                              trace_type,
                                              trace_ptr);


    if (PMC_SUCCESS == result)
    {
        /* Initialize output trace message structure */
        length = sdh_pmg_handle_ptr->var.rttp_len[index];
        trace_ptr->e164_crc_ptr = NULL;
        trace_ptr->crlf_ptr     = NULL;
        trace_ptr->size         = length;

        PMC_MEMSET(trace_msg_tmp_ptr, 0, sizeof(trace_msg_tmp_ptr));
    }



    /* Retrieve trace message */
    if (PMC_SUCCESS == result)
    {
        result = sdh_pmg_rttp_trace_get_internal(sdh_pmg_handle_ptr,
                                                 index,
                                                 trace_type,
                                                 trace_msg_tmp_ptr,
                                                 &cr_index,
                                                 &lf_index,
                                                 &crc_index);
    }




    /* Realign trace message output so that CRC/CRLF is the last character */
    if (PMC_SUCCESS == result)
    {
        result = sdh_pmg_rttp_trace_realign(sdh_pmg_handle_ptr,
                                            index,
                                            trace_msg_tmp_ptr,
                                            cr_index,
                                            lf_index,
                                            crc_index,
                                            trace_ptr);
    }




#ifdef CBRC_UT 
/*{{{ */ /* Unit Test Code */
    /* Check if retrieved trace data is aligned */
    result = sdh_pmg_rttp_ut_check_data(rttp_ut_counter, trace_ptr, trace_msg_tmp_ptr);
    
    /* Increment counter to next test */
    rttp_ut_counter++;
    PMC_RETURN(result);
/*}}}*/
#endif /* CBRC_UT */

    PMC_RETURN(result);
} /* sdh_pmg_rttp_trace_get */




/*******************************************************************************
* FUNCTION: sdh_pmg_int_validate()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validates input for interrupt functions.
*   
*
* INPUTS:
*   sdh_pmg_handle       - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   non_null_ptr         - Pointer to be checked that can't be NULL
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_int_validate(sdh_pmg_handle_t    *sdh_pmg_handle,
                                      UINT32               index,
                                      void                *non_null_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != sdh_pmg_handle, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
   
    if (index >= SDH_PMG_MAX_INDEX)
    {
        result = SDH_PMG_ERR_INVALID_INDEX;
    }

    if (NULL == non_null_ptr)
    {
        result = SDH_PMG_ERR_INVALID_ARG;
    }


    PMC_RETURN(result);
} /* sdh_pmg_int_validate */


/*******************************************************************************
* FUNCTION: sdh_pmg_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   sdh_pmg_handle    - Pointer to subsystem handle
*   index             - Link within sdh_pmg (0..3)
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
PUBLIC PMC_ERROR sdh_pmg_int_chnl_enable(sdh_pmg_handle_t    *sdh_pmg_handle,
                                         UINT32               index,
                                         sdh_pmg_int_chnl_t  *int_table_ptr,
                                         sdh_pmg_int_chnl_t  *int_en_table_ptr,
                                         BOOL                 enable)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link;

    sber_indirect_SD_SF_BERM_INT_EN_buffer_t            sd_sf_buffer;
    rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_t trace_buffer; 
    BOOL            sd_i, sf_i, tim_i, tiu_i;

    PMC_ENTRY();

    result = sdh_pmg_int_validate(sdh_pmg_handle, index, int_en_table_ptr);

   
    link = sdh_pmg_rrmp_index_get(sdh_pmg_handle, index);

    #ifdef SDH_PMG_INT
    #undef SDH_PMG_INT
    #endif
    #define SDH_PMG_INT    UTIL_GLOBAL_INT_ENABLE

    #ifdef SDH_PMG_INT_IND
    #undef SDH_PMG_INT_IND
    #endif
    #define SDH_PMG_INT_IND     UTIL_GLOBAL_INT_NOOP
    
    
    SDH_PMG_INT_TABLE_DEFINE();



    /* SD/SF Indirects */
    link = sdh_pmg_sber_index_get(sdh_pmg_handle, index);    
    if (NULL == int_table_ptr)
    {
        sd_i = TRUE;
        sf_i = TRUE;
    }
    else
    {
        sd_i = int_table_ptr->sd_i;
        sf_i = int_table_ptr->sf_i;
    }

    if ((TRUE == sd_i) || (TRUE == sf_i))
    {
        sber_indirect_SD_SF_BERM_INT_EN_buffer_init(&sd_sf_buffer);

        if (sd_i)
        {
            sd_sf_buffer.base.data[0] |= (enable) << SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SD_E_OFF;
            int_en_table_ptr->sd_i = enable;
        }

        if (sf_i)
        {
            sd_sf_buffer.base.data[0] |= (enable) << SBER_INDIRECT_REG_SD_SF_BERM_INT_EN_BIT_SF_E_OFF;
            int_en_table_ptr->sf_i = enable;
        } 

        sber_indirect_SD_SF_BERM_INT_EN_buffer_apply(sdh_pmg_handle,
                                                     link,
                                                     &sd_sf_buffer);
    }

    
    /* RTTP Indirects */
    link = sdh_pmg_rttp_index_get(sdh_pmg_handle, index);
    if(NULL == int_table_ptr)
    {
        tim_i = TRUE;
        tiu_i = TRUE;
    }
    else 
    {
        tim_i = int_table_ptr->tim_i;
        tiu_i = int_table_ptr->tiu_i;
    }

    if ((TRUE == tim_i) || (TRUE == tiu_i))
    {
        rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_init(&trace_buffer);

        if (tim_i)
        {
            trace_buffer.base.data[0] |= (enable) << RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIM_E_OFF;
            int_en_table_ptr->tim_i = enable;
        }

        if (tiu_i)
        {
            trace_buffer.base.data[0] |= (enable) << RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_EN_BIT_TIU_E_OFF;
            int_en_table_ptr->tiu_i = enable;
        }

        rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_EN_buffer_apply(sdh_pmg_handle,
                                                                link,
                                                                &trace_buffer);
        
    }


    PMC_RETURN(result);
} /* sdh_pmg_int_chnl_enable */




/*******************************************************************************
* FUNCTION: sdh_pmg_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   sdh_pmg_handle       - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   int_table_ptr        - pointer to channel interrupt table with
*                          bits set to clear interrupts
*   int_en_table_ptr     - Pointer to table tracking enabled interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR sdh_pmg_int_chnl_clear(sdh_pmg_handle_t    *sdh_pmg_handle,
                                        UINT32               index,
                                        sdh_pmg_int_chnl_t  *int_table_ptr,
                                        sdh_pmg_int_chnl_t  *int_en_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link;
    
    
    sber_indirect_SD_SF_BERM_INT_STAT_buffer_t            sd_sf_buffer;
    rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t trace_buffer; 
    BOOL            sd_i, sf_i, tim_i, tiu_i;

    PMC_ENTRY();

    result = sdh_pmg_int_validate(sdh_pmg_handle, index, int_en_table_ptr);



    link = sdh_pmg_rrmp_index_get(sdh_pmg_handle, index);
    
    #ifdef SDH_PMG_INT
    #undef SDH_PMG_INT
    #endif
    #define SDH_PMG_INT UTIL_GLOBAL_INT_SDH_PMG_CLEAR

    
    
    #ifdef SDH_PMG_INT_IND
    #undef SDH_PMG_INT_IND
    #endif
    #define SDH_PMG_INT_IND     UTIL_GLOBAL_INT_NOOP
    

    SDH_PMG_INT_TABLE_DEFINE();



    /* SD/SF Indirects */
    link = sdh_pmg_sber_index_get(sdh_pmg_handle, index);    
    if (NULL == int_table_ptr)
    {
        sd_i = TRUE;
        sf_i = TRUE;
    }
    else
    {
        sd_i = int_table_ptr->sd_i;
        sf_i = int_table_ptr->sf_i;
    }

    if ((TRUE == sd_i) || (TRUE == sf_i))
    {
        sber_indirect_SD_SF_BERM_INT_STAT_buffer_init(&sd_sf_buffer);
        sd_sf_buffer.base.data[0] |= (sd_i) << SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SD_I_OFF;
        sd_sf_buffer.base.data[0] |= (sf_i) << SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SF_I_OFF;

        sber_indirect_SD_SF_BERM_INT_STAT_buffer_apply(sdh_pmg_handle,
                                                       link,
                                                       &sd_sf_buffer);
    }



    /* RTTP Indirects */
    link = sdh_pmg_rttp_index_get(sdh_pmg_handle, index);
    if(NULL == int_table_ptr)
    {
        tim_i = TRUE;
        tiu_i = TRUE;
    }
    else 
    {
        tim_i = int_table_ptr->tim_i;
        tiu_i = int_table_ptr->tiu_i;
    }

    if ((TRUE == tim_i) || (TRUE == tiu_i))
    {
        rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_init(&trace_buffer);
        trace_buffer.base.data[0] |= (tim_i) << RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIM_I_OFF;
        trace_buffer.base.data[0] |= (tiu_i) << RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIU_I_OFF;

        rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_apply(sdh_pmg_handle,
                                                                  link,
                                                                  &trace_buffer);
    }

    PMC_RETURN(result);
} /* sdh_pmg_int_chnl_clear */




/*******************************************************************************
* FUNCTION: sdh_pmg_int_chnl_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves all interrupts to the channelized interrupt table.
*   
*
* INPUTS:
*   sdh_pmg_handle       - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   filt_table_ptr       - pointer to filter table. Mark fields as TRUE within this
*                          table to indicate that the interrupt/status will be retrieved
*                          to the output table. NULL to retrieve all 
*                          interrupt/status.
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
PUBLIC PMC_ERROR sdh_pmg_int_chnl_retrieve(sdh_pmg_handle_t    *sdh_pmg_handle,
                                           UINT32               index,
                                           sdh_pmg_int_chnl_t  *filt_table_ptr,
                                           sdh_pmg_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link;
    sber_indirect_SD_SF_BERM_INT_STAT_buffer_t              sd_sf_int;
    rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t   trace_int; 
    sber_indirect_SD_SF_BERM_STAT_buffer_t                  sd_sf_stat;
    rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_t       trace_stat; 

    PMC_ENTRY();

    result = sdh_pmg_int_validate(sdh_pmg_handle, index, int_table_ptr);



    link = sdh_pmg_rrmp_index_get(sdh_pmg_handle, index);

    UTIL_GLOBAL_INT_BUFFER_INIT(sdh_pmg_handle, rrmp, UTIL_GLOBAL_INT_OPTIMIZE);

    #ifdef SDH_PMG_INT
    #undef SDH_PMG_INT
    #endif
    #define SDH_PMG_INT UTIL_GLOBAL_INT_RETRIEVE
    
    
    #ifdef SDH_PMG_INT_IND
    #undef SDH_PMG_INT_IND
    #endif
    #define SDH_PMG_INT_IND     UTIL_GLOBAL_INT_NOOP
    
    
    SDH_PMG_INT_TABLE_DEFINE();
    
    
    #ifdef SDH_PMG_STATUS
    #undef SDH_PMG_STATUS
    #endif
    #define SDH_PMG_STATUS UTIL_GLOBAL_STATUS_RETRIEVE
    
    
    #ifdef SDH_PMG_STATUS_IND
    #undef SDH_PMG_STATUS_IND
    #endif
    #define SDH_PMG_STATUS_IND     UTIL_GLOBAL_INT_NOOP
    

    SDH_PMG_STATUS_TABLE_DEFINE();

    
    UTIL_GLOBAL_INT_BUFFER_FLUSH(sdh_pmg_handle, rrmp);
    
    
    
    /* SD/SF Indirects */
    link = sdh_pmg_sber_index_get(sdh_pmg_handle, index);    
    sber_indirect_SD_SF_BERM_INT_STAT_buffer_init(&sd_sf_int);
    
    sber_indirect_SD_SF_BERM_INT_STAT_buffer_retrieve(sdh_pmg_handle,
                                                      link,
                                                      &sd_sf_int);
   
    int_table_ptr->sd_i = (sd_sf_int.base.data[0] & SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SD_I_MSK) 
                            >> SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SD_I_OFF;
    int_table_ptr->sf_i = (sd_sf_int.base.data[0] & SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SF_I_MSK) 
                            >> SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SF_I_OFF;
   


    sber_indirect_SD_SF_BERM_STAT_buffer_init(&sd_sf_stat);
    
    sber_indirect_SD_SF_BERM_STAT_buffer_retrieve(sdh_pmg_handle,
                                                  link,
                                                  &sd_sf_stat);
   
    int_table_ptr->sd_v = (sd_sf_stat.base.data[0] & SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SD_V_MSK) 
                            >> SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SD_V_OFF;
    int_table_ptr->sf_v = (sd_sf_stat.base.data[0] & SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SF_V_MSK) 
                            >> SBER_INDIRECT_REG_SD_SF_BERM_STAT_BIT_SF_V_OFF;
                 

    /* RTTP Indirects */
    link = sdh_pmg_rttp_index_get(sdh_pmg_handle, index);
    rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_init(&trace_int);
    
    rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_retrieve(sdh_pmg_handle,
                                                                 link,
                                                                 &trace_int);
    int_table_ptr->tim_i = (trace_int.base.data[0] & RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIM_I_MSK)
                                >> RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIM_I_OFF;
    int_table_ptr->tiu_i = (trace_int.base.data[0] & RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIU_I_MSK)
                                >> RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIU_I_OFF;
   


    rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_init(&trace_stat);
    
    rttp_indirect_TRACE_MISMATCH_UNSTBL_STAT_buffer_retrieve(sdh_pmg_handle,
                                                             link,
                                                             &trace_stat);
    int_table_ptr->tim_v = (trace_stat.base.data[0] & RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT_BIT_TIM_V_MSK)
                                >> RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT_BIT_TIM_V_OFF;
    int_table_ptr->tiu_v = (trace_stat.base.data[0] & RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT_BIT_TIU_V_MSK)
                                >> RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_STAT_BIT_TIU_V_OFF;

    PMC_RETURN(result);
} /* sdh_pmg_int_chnl_retrieve */



/*******************************************************************************
* FUNCTION: sdh_pmg_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   sdh_pmg_handle       - Pointer to subsystem handle
*   index                - Link within sdh_pmg (0..3)
*   int_en_table_ptr     - Pointer to table tracking enabled interrupts
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
PUBLIC PMC_ERROR sdh_pmg_int_chnl_enabled_check(sdh_pmg_handle_t     *sdh_pmg_handle,
                                                UINT32                index,
                                                sdh_pmg_int_chnl_t   *int_en_table_ptr,
                                                BOOL                 *int_found_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          link;
    sber_indirect_SD_SF_BERM_INT_STAT_buffer_t            sd_sf_buffer;
    rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_t trace_buffer; 

    PMC_ENTRY();

    result = sdh_pmg_int_validate(sdh_pmg_handle, index, int_found_ptr);



    link = sdh_pmg_rrmp_index_get(sdh_pmg_handle, index);
    
    #ifdef SDH_PMG_INT
    #undef SDH_PMG_INT
    #endif
    #define SDH_PMG_INT UTIL_GLOBAL_INT_CHECK
    

    SDH_PMG_INT_TABLE_DEFINE();

    
    /* SD/SF Indirects */
    link = sdh_pmg_sber_index_get(sdh_pmg_handle, index);    
    if (FALSE == *int_found_ptr)
    {
        sber_indirect_SD_SF_BERM_INT_STAT_buffer_init(&sd_sf_buffer);
        
        sber_indirect_SD_SF_BERM_INT_STAT_buffer_retrieve(sdh_pmg_handle,
                                                          link,
                                                          &sd_sf_buffer);

        if ((FALSE == *int_found_ptr) && (TRUE == int_en_table_ptr->sf_i))
        {
            *int_found_ptr = (sd_sf_buffer.base.data[0] & SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SF_I_MSK) 
                                >> SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SF_I_OFF;
        }
        
        if ((FALSE == *int_found_ptr) && (TRUE == int_en_table_ptr->sd_i))
        {
            *int_found_ptr = (sd_sf_buffer.base.data[0] & SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SD_I_MSK) 
                                >> SBER_INDIRECT_REG_SD_SF_BERM_INT_STAT_BIT_SD_I_OFF;
        }

                     
    }


    /* RTTP Indirects */
    link = sdh_pmg_rttp_index_get(sdh_pmg_handle, index);
    if (FALSE == *int_found_ptr)
    {
        rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_init(&trace_buffer);
        
        rttp_indirect_TRACE_MISMATCH_UNSTBL_INT_STAT_buffer_retrieve(sdh_pmg_handle,
                                                                     link,
                                                                     &trace_buffer);


        if ((FALSE == *int_found_ptr) && (TRUE == int_en_table_ptr->tim_i))
        {
            *int_found_ptr = (trace_buffer.base.data[0] & RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIM_I_MSK)
                                    >> RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIM_I_OFF;
        }
        
        if ((FALSE == *int_found_ptr) && (TRUE == int_en_table_ptr->tiu_i))
        {
            *int_found_ptr = (trace_buffer.base.data[0] & RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIU_I_MSK)
                                    >> RTTP_INDIRECT_REG_TRACE_MISMATCH_UNSTBL_INT_STAT_BIT_TIU_I_OFF;
        }
    }
    
    
    PMC_RETURN(result);
} /* sdh_pmg_int_chnl_enabled_check */






#endif /* DOXYGEN_PUBLIC_ONLY */
/*
** Private Functions
*/
/*******************************************************************************
* sdh_pmg_is_sts_768
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks to see if mode is STS768.
*
* INPUTS:
*   mode - SDH PMG mode
*
* OUTPUTS:
*
* RETURNS:
*   TRUE if mode is for STS768
*   FALSE if mode is not for STS768
*
* NOTES:
*
*******************************************************************************/
PRIVATE BOOL sdh_pmg_is_sts_768(sdh_pmg_sts_mode_t    mode)
{
    PMC_ENTRY();

    switch (mode)
    {
        case SDH_PMG_STS_MODE_40G_STS768:
        case SDH_PMG_STS_MODE_40G_STM256:
            PMC_RETURN(TRUE);
            break;

        case SDH_PMG_STS_MODE_10G_STS192:
        case SDH_PMG_STS_MODE_10G_STM64:
        case SDH_PMG_STS_MODE_10G_STS192_PRBS:
        case SDH_PMG_STS_MODE_10G_STM64_PRBS:
            PMC_RETURN(FALSE);
            break;

        default:
            PMC_RETURN(FALSE);
            break;
            
    }

    PMC_RETURN(FALSE);
} /* sdh_pmg_is_sts_768 */


/*******************************************************************************
*  FUNCTION: sdh_pmg_rttp_crc_calc
* ______________________________________________________________________________
*
*  DESCRIPTION:   
*    This function calculates the E.164 CRC byte for a 15 byte
*    message.
*
*  INPUTS:         
*    sdh_pmg_handle_ptr   - Pointer to subsystem handle
*    msg                  - 15 byte expected TTM
*                           Legal range of each byte 0x00 to 0x7F
*
*  OUTPUTS:
*    *e164Crc             - Pointer to storage for the E.164 CRC
*                           associated with the 15 byte message.
*                           Legal range 0x80 to 0xFF
*
*  RETURNS:        
*   PMC_SUCCESS           - On success, otherwise specific error code.
*
*******************************************************************************/
PRIVATE PMC_ERROR sdh_pmg_rttp_crc_calc(sdh_pmg_handle_t         *sdh_pmg_handle_ptr,
                                        UINT8                     msg[15],
                                        UINT32                   *e164Crc)
{
    UINT32 syndromeTable[256];
    UINT32 i, j, syndrome;
    UINT32 crcPolynomial = 0x89; /* X^7 + X^3 + 1 */
    UINT32 crcAccum = 0;

    PMC_ENTRY();

    for (i = 0; i < 256; i++) {
        /* syndrome = ((i & 0x80) != 0)? (i ^ crcPolynomial) : i; */
        if ((i & 0x80) != 0) {
            /* XOR i with the CRC Polynomial */
            syndrome = i ^ crcPolynomial;
        }
        else {
            syndrome = i;
        }

        for (j = 0; j < 7; j++) {
            if (((syndrome <<= 1) & 0x80) != 0) {
                syndrome ^= crcPolynomial;
            }
        }

        syndromeTable[i] = syndrome;
    }

    *e164Crc = 0x80;
    crcAccum = syndromeTable[(crcAccum << 1) ^ *e164Crc];

    for (i = 0; i < 15; i++) {
        crcAccum = syndromeTable[(crcAccum << 1) ^ msg[i]];
    }

    *e164Crc ^= crcAccum;

    PMC_RETURN(PMC_SUCCESS);
} /* sdh_pmg_rttp_crc_calc() */


/*******************************************************************************
* FUNCTION: sdh_pmg_rrmp_index_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Converts a SDH_PMG index (0 - 3), into the corresponding index to 
*   access the RRMP. This is needed because the RRMP was originally designed
*   for an OC-48 datarate, so four blocks were needed to be tiled together
*   to achieve the OC-192 datarate. This has resulted in an indexing method
*   that does not match with the SDH PMG.
*
* INPUTS:
*   sdh_pmg_handle     - Pointer to sdh_pmg handle
*   index              - Link within sdh_pmg (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   rrmp_index          - Index to access RRMP
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 sdh_pmg_rrmp_index_get(sdh_pmg_handle_t    *sdh_pmg_handle,
                                      UINT32               index)
{
    UINT32               rrmp_index[SDH_PMG_MAX_INDEX];
    sdh_pmg_sts_mode_t   mode;

    PMC_ENTRY();
    
    mode = sdh_pmg_handle->var.mode[index];

    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);

    if (sdh_pmg_is_sts_768(mode))
    {
        rrmp_index[0] = 0;
        rrmp_index[1] = 0;
        rrmp_index[2] = 0;
        rrmp_index[3] = 0;
    }
    else
    {
        rrmp_index[0] = 12;
        rrmp_index[1] = 8;
        rrmp_index[2] = 4;
        rrmp_index[3] = 0;
    }
    

    PMC_RETURN(rrmp_index[index]);
} /* sdh_pmg_rrmp_index_get */


/*******************************************************************************
* FUNCTION: sdh_pmg_rttp_index_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Converts a SDH_PMG index (0 - 3), into the corresponding index to 
*   access the RTTP. 
*
* INPUTS:
*   sdh_pmg_handle     - Pointer to sdh_pmg handle
*   index              - Link within sdh_pmg (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   rttp_index          - Index to access RTTP
*
* NOTES:
*
*******************************************************************************/
PRIVATE UINT32 sdh_pmg_rttp_index_get(sdh_pmg_handle_t    *sdh_pmg_handle,
                                      UINT32               index)
{
    UINT32               rttp_index[SDH_PMG_MAX_INDEX];
    sdh_pmg_sts_mode_t   mode;

    PMC_ENTRY();

    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    
    mode = sdh_pmg_handle->var.mode[index];

    if (sdh_pmg_is_sts_768(mode))
    {
        rttp_index[0] = 0;
        rttp_index[1] = 0;
        rttp_index[2] = 0;
        rttp_index[3] = 0;
    }
    else
    {
        rttp_index[0] = 3;
        rttp_index[1] = 2;
        rttp_index[2] = 1;
        rttp_index[3] = 0;
    } 

    PMC_RETURN(rttp_index[index]);
} /* sdh_pmg_rttp_index_get */



/*******************************************************************************
* FUNCTION: sdh_pmg_sber_index_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Converts a SDH_PMG index (0 - 3), into the corresponding index to 
*   access the SBER. 
*
* INPUTS:
*   sdh_pmg_handle     - Pointer to sdh_pmg handle
*   index              - Link within sdh_pmg (0..3)
*
* OUTPUTS:
*
* RETURNS:
*   sber_index          - Index to access SBER
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 sdh_pmg_sber_index_get(sdh_pmg_handle_t    *sdh_pmg_handle,
                                     UINT32               index)
{
    UINT32  sber_index;
    PMC_ENTRY();

    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    PMC_ASSERT(sdh_pmg_handle != NULL, SDH_PMG_ERR_NULL_HANDLE, 0, 0);

    /* Same mapping as RTTP */
    sber_index = sdh_pmg_rttp_index_get(sdh_pmg_handle, index);

    PMC_RETURN(sber_index);
} /* sdh_pmg_sber_index_get */



/*******************************************************************************
* FUNCTION: sdh_pmg_range_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns range of indices to operate on within SDH_PMG based on mode. 
*
* INPUTS:
*   sdh_pmg_handle     - Pointer to sdh_pmg handle
*   index              - Index into SDH_PMG
*
* OUTPUTS:
*   start_index        - Beginning of range to operate on
*   end_index          - End of range to operate on
*
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sdh_pmg_range_get(sdh_pmg_handle_t    *sdh_pmg_handle,
                                    UINT32               index,
                                    UINT8               *start_index,
                                    UINT8               *end_index)
{
    PMC_ERROR            result = PMC_SUCCESS;
    sdh_pmg_sts_mode_t   mode;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != sdh_pmg_handle, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    PMC_ASSERT(NULL != start_index, SDH_PMG_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != end_index, SDH_PMG_ERR_INVALID_ARG, 0, 0);

    mode = sdh_pmg_handle->var.mode[index];

    if (sdh_pmg_is_sts_768(mode))
    {
        *start_index = 0;
        *end_index   = SDH_PMG_MAX_INDEX;

    } else {
        *start_index = index;
        *end_index   = index + 1;
    }

    PMC_RETURN(result);
} /* sdh_pmg_range_get */


/*******************************************************************************
* FUNCTION: sdh_pmg_int_init()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes/deinitializes interrupt and status support.
*   
*
* INPUTS:
*   sdh_pmg_handle     - Pointer to sdh_pmg handle
*   enable          - Enables/disables interrupt and status support.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sdh_pmg_int_init(sdh_pmg_handle_t    *sdh_pmg_handle,
                                   BOOL                 enable)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ENTRY();

    /* Enable SBER, RTTP, RRMP interrupts aggregators */
    sdh_pmg_reg_MTSB_INT_EN_write(NULL, sdh_pmg_handle, 0xFFF);

    PMC_RETURN(result);
} /* sdh_pmg_int_init */


/*******************************************************************************
* FUNCTION: sdh_pmg_rttp_arg_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks if RTTP expected message set arguments are valid, depending
*   on config.
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to sdh_pmg handle
*   index                - Link within sdh_pmg (0..3)
*   algo                 - Disable trace message processing, or use specified
*                          algorithm
*   *expect_trace_ptr    - pointer to trace message structure. Can be NULL
*                          if algo is SDH_PMG_RTTP_ALGO_DISABLED
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sdh_pmg_rttp_arg_check(sdh_pmg_handle_t      *sdh_pmg_handle_ptr,
                                         UINT32                 index,
                                         sdh_pmg_rttp_algo_t    algo,
                                         sdh_pmg_rttp_trace_t  *expect_trace_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;
    sdh_pmg_rttp_length_t   length;
    sdh_pmg_sts_mode_t      mode;
    UINT32                  i;

    PMC_ENTRY();

    
    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    PMC_ASSERT(algo  < LAST_SDH_PMG_RTTP_ALGO, SDH_PMG_ERR_INVALID_ARG, algo, LAST_SDH_PMG_RTTP_ALGO);

    
    /* Check if trace message is supplied */
    /* If we are disabling then do not need expect_trace_ptr otherwise we
     * do so return an error if it is NULL and algo is not DISABLED.
     */
    if (NULL == expect_trace_ptr && LAST_SDH_PMG_RTTP_ALGO != algo)
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_ARG);
    }

    /* Check if index is correct for STS 768 */
    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }


    /* Check if length has been configured */
    length = sdh_pmg_handle_ptr->var.rttp_len[index];
    if (LAST_SDH_PMG_RTTP_LENGTH == length)
    {
        PMC_RETURN(SDH_PMG_ERR_RTTP_LEN_NOT_CFG);
    }

    /*
     * Execute this code only if the trace pointer is valid
     */
    if (expect_trace_ptr)
    {
        /* Check incoming trace structure for valid lengths */
        switch (length)
        {
          case SDH_PMG_RTTP_LENGTH_1_BYTE:
            if (expect_trace_ptr->size != SDH_PMG_RTTP_LENGTH_1_BYTE)
            {
                PMC_RETURN(SDH_PMG_ERR_RTTP_LEN_MISMATCH);
            }
            break;
            
          case SDH_PMG_RTTP_LENGTH_16_BYTES:
            if (expect_trace_ptr->size != SDH_PMG_RTTP_LENGTH_16_BYTES)
            {
                PMC_RETURN(SDH_PMG_ERR_RTTP_LEN_MISMATCH);
             }
            
            /* Check expect trace msg characters */
            for (i = 0; i < (SDH_PMG_RTTP_LENGTH_16_BYTES - 1); i++) 
            {
                if (expect_trace_ptr->trace_msg[i] >= SDH_PMG_RTTP_ASCII_CRC_START)
                {
                    PMC_RETURN(SDH_PMG_ERR_RTTP_INVALID_MSG_CHAR);
                }
            }
            break;
            
          case SDH_PMG_RTTP_LENGTH_64_BYTES:             
            if (expect_trace_ptr->size != SDH_PMG_RTTP_LENGTH_64_BYTES)
            {
                PMC_RETURN(SDH_PMG_ERR_RTTP_LEN_MISMATCH);
            }
            break;
            
          default:
            break;
        } /* end switch */
    }

    /* Check if 1 byte length is being used with algo 3 */
    if ((SDH_PMG_RTTP_LENGTH_1_BYTE == length) && !(SDH_PMG_RTTP_ALGO_3 == algo))
    {
        PMC_RETURN(SDH_PMG_ERR_RTTP_ALGO3_LEN_MISMATCH);
    }


    PMC_RETURN(result);
} /* sdh_pmg_rttp_arg_check */


/*******************************************************************************
* FUNCTION: sdh_pmg_rttp_algo_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the RTTP algorithm on given SDH PMG slice.
*
* INPUTS:
*   sdh_pmg_handle     - Pointer to sdh_pmg handle
*   index              - Link within sdh_pmg (0..3)
*   algo               - Disable trace message processing, or use specified
*                        algorithm
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sdh_pmg_rttp_algo_set(sdh_pmg_handle_t    *sdh_pmg_handle,
                                        UINT32               index,
                                        sdh_pmg_rttp_algo_t  algo)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          rttp_index;
    BOOL8           enable = TRUE;
    rttp_indirect_TRACE_CFG_buffer_t        cfg_buffer;

    PMC_ENTRY();

    if (SDH_PMG_RTTP_ALGO_DISABLED == algo)
    {
        enable = FALSE;
    }

    rttp_index = sdh_pmg_rttp_index_get(sdh_pmg_handle, index);    
    rttp_indirect_TRACE_CFG_buffer_init(&cfg_buffer);

    /* Set correct algorithm and enable/disable RTTP processing */
    cfg_buffer.base.mask[0] = RTTP_INDIRECT_REG_TRACE_CFG_BIT_TTMMONEN_MSK |
                              RTTP_INDIRECT_REG_TRACE_CFG_BIT_ZEROEN_MSK |
                              RTTP_INDIRECT_REG_TRACE_CFG_BIT_ALGO_MSK;
    
    cfg_buffer.base.data[0] = (enable << RTTP_INDIRECT_REG_TRACE_CFG_BIT_TTMMONEN_OFF) |
                              (enable << RTTP_INDIRECT_REG_TRACE_CFG_BIT_ZEROEN_OFF) |
                              (algo << RTTP_INDIRECT_REG_TRACE_CFG_BIT_ALGO_OFF);
   
    /* Write config indirect register on RTTP slice */
    rttp_indirect_TRACE_CFG_buffer_apply(sdh_pmg_handle,
                                         rttp_index,
                                         &cfg_buffer);



    /* Store internal config on SDH_PMG slice*/
    sdh_pmg_handle->var.rttp_algo[index] = algo;

    PMC_RETURN(result);
} /* sdh_pmg_rttp_algo_set */


/*******************************************************************************
* FUNCTION: sdh_pmg_rttp_expect_trace_set_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets expected trace message on SDH_PMG slice.  
*
* INPUTS:
*   sdh_pmg_handle     - Pointer to sdh_pmg handle
*   index              - Link within sdh_pmg (0..3)
*   *expect_trace_ptr  - pointer to trace message structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sdh_pmg_rttp_expect_trace_set_internal(sdh_pmg_handle_t      *sdh_pmg_handle,
                                                         UINT32                 index,
                                                         sdh_pmg_rttp_trace_t  *expect_trace_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;
    sdh_pmg_rttp_length_t   length;
    UINT8                   i, j;
    UINT32                  crc;
    UINT32                  rttp_index;
    rttp_indirect_EXPECTED_TRACE_buffer_t   trace_buffer;



    PMC_ENTRY();
    
    PMC_ASSERT(NULL != expect_trace_ptr, SDH_PMG_ERR_INVALID_ARG, 0, 0);

    length = sdh_pmg_handle->var.rttp_len[index];

    /* Insert CRC and CR/LF based on configured length */
    switch (length)
    {
        /* Insert CRC in byte 16 */ 
        case SDH_PMG_RTTP_LENGTH_16_BYTES:
            sdh_pmg_rttp_crc_calc(sdh_pmg_handle,
                                  expect_trace_ptr->trace_msg, /* input  - 15 byte trace msg */
                                  &crc);                       /* output - crc */
            expect_trace_ptr->trace_msg[15] = crc; 

            break;
        
        /* Insert CR/LF in bytes 63 and 64 */
        case SDH_PMG_RTTP_LENGTH_64_BYTES:             
            expect_trace_ptr->trace_msg[62] = SDH_PMG_RTTP_ASCII_CR;
            expect_trace_ptr->trace_msg[63] = SDH_PMG_RTTP_ASCII_LF;
            break;
   
        /* No insertion */
        case SDH_PMG_RTTP_LENGTH_1_BYTE:
            break;

        default:
            break;
    } /* end switch */


    /* Write the expected trace message */
    /* i: index accessing indirect register
     * j: index accessing trace msg buffer */

    rttp_index = sdh_pmg_rttp_index_get(sdh_pmg_handle, index);    

    if (SDH_PMG_RTTP_LENGTH_16_BYTES == length)
    {
        j = 15; /* Write CRC byte into first indirect register */
    }
    else
    {
        j = 0; /* Write first trace msg byte into first indirect register */
    }

    for (i = 0; i < length; i++)
    {
        rttp_indirect_EXPECTED_TRACE_buffer_init(&trace_buffer);
        trace_buffer.base.mask[0] = RTTP_INDIRECT_REG_EXPECTED_TRACE_BIT_ETRACE_MSK;
        trace_buffer.base.data[0] = expect_trace_ptr->trace_msg[j] << RTTP_INDIRECT_REG_EXPECTED_TRACE_BIT_ETRACE_OFF;

        rttp_indirect_EXPECTED_TRACE_buffer_apply(sdh_pmg_handle,
                                                  rttp_index, i,
                                                  &trace_buffer);

        /* Wrap around trace msg buffer index */
        j = (j + 1) % length;
    } 

    PMC_RETURN(result);
} /* sdh_pmg_rttp_expect_trace_set_internal */



/*******************************************************************************
* FUNCTION: sdh_pmg_rttp_trace_get_arg_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Checks arguments for trace retrieval function.
*
* INPUTS:
*   sdh_pmg_handle_ptr   - Pointer to sdh_pmg handle
*   index                - Link within sdh_pmg (0..3)
*   trace_type           - Captured, accepted, or expected trace
*                          Trace message must be enabled to retrieve 
*                          the captured/accepted message. The expected
*                          message can always be retrieved.
*                          Accepted trace message is only available when using
*                          SDH_PMG_RTTP_ALGO_2.
*
* OUTPUTS:
*   trace_ptr            - Pointer to trace message structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sdh_pmg_rttp_trace_get_arg_check(sdh_pmg_handle_t           *sdh_pmg_handle_ptr,
                                                   UINT32                      index,
                                                   sdh_pmg_rttp_trace_type_t   trace_type,
                                                   sdh_pmg_rttp_trace_t       *trace_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;
    sdh_pmg_sts_mode_t      mode;
    sdh_pmg_rttp_length_t   length;
    sdh_pmg_rttp_algo_t     algo;

    PMC_ENTRY();

    PMC_ASSERT(NULL != sdh_pmg_handle_ptr, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(index < SDH_PMG_MAX_INDEX, SDH_PMG_ERR_INVALID_INDEX, index, SDH_PMG_MAX_INDEX);
    PMC_ASSERT(trace_type < LAST_SDH_PMG_RTTP_TRACE_TYPE, SDH_PMG_ERR_INVALID_ARG, trace_type, LAST_SDH_PMG_RTTP_TRACE_TYPE);
    PMC_ASSERT(NULL != trace_ptr, SDH_PMG_ERR_INVALID_ARG, 0, 0);


    /* Check if index is correct for STS 768 */
    mode = sdh_pmg_handle_ptr->var.mode[index];
    if (sdh_pmg_is_sts_768(mode) && (index != 0))
    {
        PMC_RETURN(SDH_PMG_ERR_INVALID_STS_768_INDEX);
    }


    /* Check if length has been configured */
    length = sdh_pmg_handle_ptr->var.rttp_len[index];
    if (LAST_SDH_PMG_RTTP_LENGTH == length)
    {
        PMC_RETURN(SDH_PMG_ERR_RTTP_LEN_NOT_CFG);
    }

    /* For captured and accepted trace, check if monitoring is enabled */
    algo = sdh_pmg_handle_ptr->var.rttp_algo[index];
    if ((SDH_PMG_RTTP_ALGO_DISABLED == algo) && (SDH_PMG_RTTP_TRACE_TYPE_EXPECTED != trace_type))
    {
        PMC_RETURN(SDH_PMG_ERR_RTTP_ALGO_DISABLED);
    }

    /* Accepted trace is only available for ALGO 2 */
    if ((SDH_PMG_RTTP_TRACE_TYPE_ACCEPTED == trace_type) && (SDH_PMG_RTTP_ALGO_2 != algo))
    {
        PMC_RETURN(SDH_PMG_ERR_RTTP_ACCEPTED_MISMATCH);
    }


    PMC_RETURN(result);
} /* sdh_pmg_rttp_trace_get_arg_check */


/*******************************************************************************
* FUNCTION: sdh_pmg_rttp_trace_get_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves desired trace message and index to CR, LF, and CRC.
*
* INPUTS:
*   sdh_pmg_handle_ptr - Pointer to sdh_pmg handle
*   index              - Link within sdh_pmg (0..3)
*   trace_type         - Captured, accepted, or expected trace
*
* OUTPUTS:
*   trace_msg_ptr      - Pointer to buffer for entire retrieved trace message
*   cr_index           - Pointer to CR element in message
*   lf_index           - Pointer to LF element in message
*   crc_index          - Pointer to CRC element in message
*
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sdh_pmg_rttp_trace_get_internal(sdh_pmg_handle_t          *sdh_pmg_handle_ptr,
                                                  UINT32                     index,
                                                  sdh_pmg_rttp_trace_type_t  trace_type,
                                                  UINT8                     *trace_msg_ptr,
                                                  UINT8                     *cr_index,
                                                  UINT8                     *lf_index,
                                                  UINT8                     *crc_index)
{
    PMC_ERROR       result = PMC_SUCCESS;
    void           *indirect_ptr;   /* buffer for indirect reads */
    rttp_indirect_buffer_t indirect_bfr;
    UINT8          *trace_byte_ptr;        /* pointer to character within trace message buffer */
    UINT8           i;
    UINT8           rttp_index;
    UINT32          length;

    PMC_ENTRY();

    PMC_ASSERT(sdh_pmg_handle_ptr != NULL, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(trace_msg_ptr      != NULL, SDH_PMG_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(cr_index           != NULL, SDH_PMG_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(lf_index           != NULL, SDH_PMG_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(crc_index          != NULL, SDH_PMG_ERR_INVALID_ARG, 0, 0);


    /* Initialize pointers to CRC/CR/LF */
    *cr_index  = SDH_PMG_RTTP_INVALID_INDEX;
    *lf_index  = SDH_PMG_RTTP_INVALID_INDEX;
    *crc_index = SDH_PMG_RTTP_INVALID_INDEX;
    
    /* Buffer for indirect reads */
    indirect_ptr = &indirect_bfr;
    PMC_MEMSET(indirect_ptr, 0, sizeof(rttp_indirect_buffer_t));
    

    
    /* Obtain RTTP index */
    rttp_index = sdh_pmg_rttp_index_get(sdh_pmg_handle_ptr, index);    
    

    length = sdh_pmg_handle_ptr->var.rttp_len[index];



    /* Extract trace message */
    for (i = 0; i < length; i++)
    {
        /* Retrieve a byte */
        switch (trace_type)
        {
            case SDH_PMG_RTTP_TRACE_TYPE_EXPECTED:
                rttp_indirect_EXPECTED_TRACE_buffer_retrieve(sdh_pmg_handle_ptr,
                                                             rttp_index, i,
                                                             (rttp_indirect_EXPECTED_TRACE_buffer_t*)indirect_ptr);
                break;
            
            case SDH_PMG_RTTP_TRACE_TYPE_CAPTURED:
                rttp_indirect_CAPTURED_TRACE_buffer_retrieve(sdh_pmg_handle_ptr,
                                                             rttp_index, i,
                                                             (rttp_indirect_CAPTURED_TRACE_buffer_t*)indirect_ptr);
                break;
            
            case SDH_PMG_RTTP_TRACE_TYPE_ACCEPTED:             
                rttp_indirect_ACCEPTED_TRACE_buffer_retrieve(sdh_pmg_handle_ptr,
                                                             rttp_index, i,
                                                             (rttp_indirect_ACCEPTED_TRACE_buffer_t*)indirect_ptr);
                break;
        
            default:
                break;
        } /* end switch */
     

#ifdef CBRC_UT 
/*{{{ */ /* Unit Test Code */
        /* Fill the obtained byte with our unit test data */
        sdh_pmg_rttp_ut_buffer_fill(rttp_ut_counter, i, (UINT32 *) ((rttp_indirect_buffer_t *) indirect_ptr)->data);
/* }}} */
#endif

        /* Copy byte into output trace message buffer */
        trace_byte_ptr = &trace_msg_ptr[i];
        *trace_byte_ptr = ((rttp_indirect_buffer_t *) indirect_ptr)->data[0];


        /* Identify CRLF for 64 byte messages */
        if (SDH_PMG_RTTP_LENGTH_64_BYTES == length)
        {
            if ((*lf_index == SDH_PMG_RTTP_INVALID_INDEX) &&
                (SDH_PMG_RTTP_ASCII_CR == *trace_byte_ptr))
                
            {
                *cr_index = i; 

                /* If CR is the last character, check the first character for the LF */
                if (*cr_index == (length - 1))
                {
                    if(SDH_PMG_RTTP_ASCII_LF == trace_msg_ptr[0])
                    {
                        *lf_index = 0;
                    }
                }
            }

            /* Look for LF only if CF has been found already */
            if ((*cr_index != SDH_PMG_RTTP_INVALID_INDEX) &&
                (*lf_index == SDH_PMG_RTTP_INVALID_INDEX) &&
                (SDH_PMG_RTTP_ASCII_LF == *trace_byte_ptr))
            {
                *lf_index = i;

                /* Check if CRLF is adjacent, if not throw away the indices */
                if (((*cr_index + 1) % length) != *lf_index )
                {
                    *cr_index = SDH_PMG_RTTP_INVALID_INDEX; 
                    *lf_index = SDH_PMG_RTTP_INVALID_INDEX; 
                }
            }
        }
        
        /* Identify CRC for 16 byte messages */
        if ((SDH_PMG_RTTP_LENGTH_16_BYTES == length) && (*trace_byte_ptr >= SDH_PMG_RTTP_ASCII_CRC_START))
        {
            *crc_index = i;
        }
    } /* end trace message for */

    

    PMC_RETURN(result);
} /* sdh_pmg_rttp_trace_get_internal */



/*******************************************************************************
* FUNCTION: sdh_pmg_rttp_trace_realign()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Realign trace message output so that CRC/CRLF is the last character.
*
* INPUTS:
*   sdh_pmg_handle_ptr     - Pointer to sdh_pmg handle
*   index                  - Link within sdh_pmg (0..3)
*   trace_msg_ptr          - Pointer to buffer storing entire retrieved trace message
*   cr_index               - CR index in message
*   lf_index               - LF index in message
*   crc_index              - CRC index in message
*
* OUTPUTS:
*   trace_ptr              - Pointer to trace message structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR sdh_pmg_rttp_trace_realign(sdh_pmg_handle_t       *sdh_pmg_handle_ptr,
                                             UINT32                  index,
                                             UINT8                  *trace_msg_ptr,
                                             UINT8                   cr_index,
                                             UINT8                   lf_index,
                                             UINT8                   crc_index,
                                             sdh_pmg_rttp_trace_t   *trace_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT8           end_index;
    UINT32          length;

    PMC_ENTRY();
    
    
    PMC_ASSERT(sdh_pmg_handle_ptr != NULL, SDH_PMG_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(trace_msg_ptr      != NULL, SDH_PMG_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(trace_ptr          != NULL, SDH_PMG_ERR_INVALID_ARG, 0, 0);


    length = sdh_pmg_handle_ptr->var.rttp_len[index];

    switch (length)
    {
        case SDH_PMG_RTTP_LENGTH_64_BYTES:
            if ((cr_index != SDH_PMG_RTTP_INVALID_INDEX) &&
                (lf_index != SDH_PMG_RTTP_INVALID_INDEX))
            {
                /* Check that CRLF are adjacent, otherwise log error */
                if ( ((cr_index + 1) % length) != lf_index)
                {
                    PMC_LOG(PMC_LOG_SEV_HIGHEST, SDH_PMG_ERR_RTTP_CRLF_NOT_ADJ, 0, 0);  
                }

                end_index = lf_index;
                trace_ptr->crlf_ptr = &trace_ptr->trace_msg[length - 2];     /* CRLF is in second last byte */
            }
            else 
            {
                /* Set end index to end buffer to copy without modification */
                end_index = length - 1;
                PMC_LOG(PMC_LOG_SEV_HIGHEST, SDH_PMG_ERR_RTTP_CRLF_NOT_FOUND, 0, 0);  
            }
            
            break;
        
        case SDH_PMG_RTTP_LENGTH_16_BYTES:
            if (crc_index != SDH_PMG_RTTP_INVALID_INDEX)
            {
                end_index = crc_index;
                trace_ptr->e164_crc_ptr = &trace_ptr->trace_msg[length - 1]; /* CRC is in last byte */
            } 
            else
            {
                /* Set end index to end buffer to copy without modification */
                end_index = length - 1;
                PMC_LOG(PMC_LOG_SEV_HIGHEST, SDH_PMG_ERR_RTTP_CRC_NOT_FOUND, 0, 0);
            }
            break;
        
        case SDH_PMG_RTTP_LENGTH_1_BYTE: 
            /* Set end index to end buffer to copy without modification */
            end_index = length - 1;
            break;
    
        default:
            PMC_ASSERT(0, SDH_PMG_ERR_CODE_ASSERT, length, LAST_SDH_PMG_RTTP_LENGTH);
            break;
    } /* end switch */
    




    /* Increment index so that it is pointing after the CRC/CRLF */
    end_index++;

    /* If CRC/CRLF is already the last byte, do not need to do this */
    if (end_index != trace_ptr->size)
    {
        /* Copy trace message from byte following CRC/CRLF to end of tmp buffer */
        PMC_MEMCPY(&trace_ptr->trace_msg[0], &trace_msg_ptr[end_index], trace_ptr->size - end_index);
    }

    /* Copy trace message from beginning of tmp buffer to the CRC/CRLF */
    PMC_MEMCPY(&trace_ptr->trace_msg[trace_ptr->size - end_index], &trace_msg_ptr[0], end_index); 


    PMC_RETURN(result);
} /* sdh_pmg_rttp_trace_realign */




/*
** End of file
*/
