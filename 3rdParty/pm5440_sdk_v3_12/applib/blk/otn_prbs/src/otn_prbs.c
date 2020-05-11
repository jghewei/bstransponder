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
*       OTN PRBS API
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "otn_prbs_loc.h"


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
const char OTN_PRBS_LOG_ERR_STRINGS[][PMC_LOG_STRING_SIZEOF] =
{
    "Start of log string list",
    OTN_PRBS_LOG_ERR_TABLE
};
#undef PMC_LOG_ERR_PAIR_ENTRY_CREATE

#else /* no strings */

const char OTN_PRBS_LOG_ERR_STRINGS[1][1] = {};

#endif /* PMC_SW_SIMULATION */


/*
** Forward References
*/

/*
** Public Functions
*/

/*******************************************************************************
* otn_prbs_ctxt_create
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Creates and initializes a handle for an OTN_PRBS block instance.
*
*
* INPUTS:
*   *parent               - pointer to parent handle.  If no parent pass in
*                           NULL
*   base_address          - base address of the oduksc subsystem relative to the
*                           device memory space
*   *sys_handle           - pointer to user system handle
*   *tsb_name             - unique string identifier of the block
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   otn_prbs_handle_t     - pointer to created handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC otn_prbs_handle_t *otn_prbs_ctxt_create(pmc_handle_t *parent, UINT32 base_address, pmc_sys_handle_t *sys_handle, const char *tsb_name)
{
    otn_prbs_handle_t *otn_prbs_handle;

    PMC_ENTRY();
    
    otn_prbs_handle = (otn_prbs_handle_t*)PMC_CTXT_CALLOC(sizeof(otn_prbs_handle_t), parent);

    pmc_handle_init(parent, otn_prbs_handle, sys_handle, PMC_MID_DIGI_OTN_PRBS, tsb_name, base_address);

    /* Register log strings. */
    pmc_log_block_strings_register(OTN_PRBS_LOG_ERR_STRINGS[0], OTN_PRBS_LOG_ERR_TABLE_BASE, OTN_PRBS_LOG_ERR_COUNT);

    PMC_RETURN(otn_prbs_handle);
} /* otn_prbs_ctxt_create */


/*******************************************************************************
* otn_prbs_ctxt_destroy
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Destroys a handle for an OTN_PRBS block instance.
*
*
* INPUTS:
*   *otn_prbs_handle   - pointer to OTN_PRBS handle instance
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
PUBLIC void otn_prbs_ctxt_destroy(otn_prbs_handle_t *otn_prbs_handle)
{
    PMC_ENTRY();
    
    PMC_CTXT_FREE(&otn_prbs_handle, otn_prbs_handle);
    
    PMC_RETURN();
} /* otn_prbs_ctxt_destroy */

/*******************************************************************************
* otn_prbs_handle_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes an OTN_PRBS block handle instance.
*
*
* INPUTS:
*   *otn_prbs_handle     - pointer to OTN_PRBS handle instance
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
PUBLIC void otn_prbs_handle_init(otn_prbs_handle_t *otn_prbs_handle)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    /* OTN_PRBS parameters */
    otn_prbs_handle->cfg.max_chnl         = OTN_PRBS_NUM_CHNL; 
    
    PMC_RETURN();

} /* otn_prbs_handle_init */

/*******************************************************************************
* otn_prbs_handle_restart_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Performs warm or crash restart processing on otn_prbs.
*
*
* INPUTS:
*   *otn_prbs_handle      - pointer to OTN_PRBS handle instance
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
PUBLIC PMC_ERROR otn_prbs_handle_restart_init(otn_prbs_handle_t *otn_prbs_handle,
                                              util_global_restart_init_cfg_t *restart_init_cfg_ptr, 
                                              pmc_energy_state_t top_energy_state_reg)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (restart_init_cfg_ptr->phase == UTIL_GLOBAL_RESTART_INIT_PHASE_FIRST)
    {
        /* Register log strings. */
        pmc_log_block_strings_register(OTN_PRBS_LOG_ERR_STRINGS[0], OTN_PRBS_LOG_ERR_TABLE_BASE, OTN_PRBS_LOG_ERR_COUNT);
    }

    PMC_RETURN(ret_val);
} /* otn_prbs_handle_restart_init */


/*******************************************************************************
* otn_prbs_base_address_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the base address of an OTN_PRBS block instance.
*
*
* INPUTS:
*   *otn_prbs_handle   - pointer to OTN_PRBS handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   Base address.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 otn_prbs_base_address_get(otn_prbs_handle_t *otn_prbs_handle)
{
    PMC_ENTRY();
    
    PMC_RETURN(otn_prbs_handle->base.base_address);
} /* otn_prbs_base_address_get */

/*******************************************************************************
*  otn_prbs_chnl_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize PRBS block to generate or monitor the pattern on the channel.
*
* INPUTS:
*   *otn_prbs_handle    - pointer to OTN_PRBS handle instance
*   chnl                - channel ID to be provisioned 
*                         Valid range: 0 - 11
*   prbs_operation      - mode of operation \n
*                         0 -- PRBS monitor \n
*                         1 -- PRBS generator
*   mode                - operational mode \n
*                         0 -- disable \n
*                         1 -- enable
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otn_prbs_chnl_init(otn_prbs_handle_t *otn_prbs_handle, 
                                    UINT32 chnl, UINT32 prbs_operation,
                                    UINT32 mode)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    if (prbs_operation == 1)
    {
        otn_prbs_field_PRBS_GEN_EN_set(NULL, otn_prbs_handle, chnl, mode);
    }
    else {
        otn_prbs_field_PRBS_MON_EN_set(NULL, otn_prbs_handle, chnl, mode);
    }
       
    PMC_RETURN(ret_val);
} /* otn_prbs_chnl_init */

/*******************************************************************************
*  otn_prbs_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel with the specified PRBS pattern.
*
* INPUTS:
*   *otn_prbs_handle    - pointer to OTN_PRBS handle instance
*   chnl                - channel ID that carries PRBS pattern
*                         Valid range: 0 - 11
*   dci_chnl            - DCI channel that is to be mapped to the PRBS channel
*                         Valid range: 0 - 95
*   prbs_pattern        - PRBS pattern to transmit. See otn_prbs_pattern_t.
*                         0 -- OTN_INVERTED_PRBS_11
*                         1 -- OTN_INVERTED_PRBS_31
*                         2 -- OTN_PRBS_11
*                         3 -- OTN_PRBS_31
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otn_prbs_chnl_prov(otn_prbs_handle_t *otn_prbs_handle,
                                    UINT32 chnl, UINT32 dci_chnl, 
                                    otn_prbs_pattern_t prbs_pattern)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 inverted_patt;
    UINT32 prbs_sel;
    
    PMC_ENTRY();
    
    /* argument checking */
    if (chnl > 12 || dci_chnl > 95 || prbs_pattern > OTN_PRBS_31)
    {
        PMC_RETURN(OTN_PRBS_ERR_INVALID_ARGS);
    }
    
    /* program the PRBS pattern to transmit */
    switch (prbs_pattern)
    {
        case OTN_INVERTED_PRBS_11:
            inverted_patt = 1;
            prbs_sel = 0;
        break;
        case OTN_INVERTED_PRBS_31:
            inverted_patt = 1;
            prbs_sel = 1;
        break;
        case OTN_PRBS_11:
            inverted_patt = 0;
            prbs_sel = 0;
        break;
        case OTN_PRBS_31:
            inverted_patt = 0;
            prbs_sel = 1;
        break;
        default:
            PMC_RETURN(OTN_PRBS_ERR_INVALID_ARGS);
        break;
    }
    
    otn_prbs_field_PRBS_CHID_set(NULL, otn_prbs_handle, chnl, dci_chnl);
    otn_prbs_field_PRBS_SEL_INV_NONINV_set(NULL, otn_prbs_handle, chnl, inverted_patt);
    otn_prbs_field_PRBS_SEL_31_11_set(NULL, otn_prbs_handle, chnl, prbs_sel);
    
    PMC_RETURN(ret_val);
    
} /* otn_prbs_chnl_prov */

/*******************************************************************************
*  otn_prbs_ber_thres_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the defect threshold count for the given PRBS channel. 
*   If the bit error rate goes over the programmed threshold in a gvien LCLK 
*   one second interval, the PRBS monitor will enter out of lock state.
*
* INPUTS:
*   *otn_prbs_handle    - pointer to OTN_PRBS handle instance
*   prbs_ber_lo_thres   - lower 32 bits of the defect threshold count for the 
*                         given PRBS channel. 
*   prbs_ber_hi_thres   - upper 3 bits of the defect threshold count for the 
*                         given PRBS channel. 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otn_prbs_ber_thres_cfg(otn_prbs_handle_t *otn_prbs_handle,
                                        UINT32 prbs_ber_lo_thres, 
                                        UINT32 prbs_ber_hi_thres)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    otn_prbs_lfield_range_PRBS_BER_THRES_set(NULL, otn_prbs_handle, 0, 31, 
                                             prbs_ber_lo_thres);
    otn_prbs_lfield_range_PRBS_BER_THRES_set(NULL, otn_prbs_handle, 32, 34, 
                                             prbs_ber_hi_thres);                                             
                                             
    
    PMC_RETURN(ret_val);    
} /* otn_prbs_ber_thres_cfg */

/*******************************************************************************
*  otn_prbs_chnl_bit_err_inject
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Injecst a single bit error into the PRBS pattern for the channel.
*
* INPUTS:
*   *otn_prbs_handle    - pointer to OTN_PRBS handle instance
*   chnl                - channel ID to be configured. 
*   insert_err          - control whether or not a single bit error will be 
*                         inserted in the PRBS pattern.
*                         1 -- a single bit erro is inserted once into the PRBS
*                              pattern for the channel
*                         0 -- defect insertion is disabled.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otn_prbs_chnl_bit_err_inject(otn_prbs_handle_t *otn_prbs_handle,
                                              UINT32 chnl, UINT32 insert_err)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    /* argument checking */
    if (chnl > 12 || insert_err > 1)
    {
        PMC_RETURN(OTN_PRBS_ERR_INVALID_ARGS);
    }
    
    /* inject a single bit error */
    otn_prbs_field_PRBS_INS_DEFECT_set(NULL, otn_prbs_handle, chnl, insert_err);                                             
                                               
    PMC_RETURN(ret_val);    
} /* otn_prbs_chnl_bit_err_inject */

/*******************************************************************************
*  otn_prbs_chnl_defect_cnt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retreives defect count for the PRBS channel.
*
* INPUTS:
*   *otn_prbs_handle    - pointer to OTN_PRBS handle instance
*   chnl                - channel ID to be configured. 
*
* OUTPUTS:
*   defect_cnt          - defect count for the given PRBS channel.
*
* RETURNS:
*   PMC_SUCCESS if successful, error code otherwise
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR otn_prbs_chnl_defect_cnt_get(otn_prbs_handle_t *otn_prbs_handle,
                                              UINT32 chnl, UINT32 *defect_cnt)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    /* argument checking */
    if (chnl > 11)
    {
        PMC_RETURN(OTN_PRBS_ERR_INVALID_ARGS);
    }
    
    *defect_cnt = otn_prbs_field_PRBS_TSE_CHAN_REG_get(NULL, otn_prbs_handle, chnl);                                         
                                               
    PMC_RETURN(ret_val);    
} /* otn_prbs_chnl_defect_cnt_get */

/*
** End of file
*/
