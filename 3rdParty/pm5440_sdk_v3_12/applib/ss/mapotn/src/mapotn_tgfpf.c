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
*   DESCRIPTION : This file contains C functions for the MAPOTN GFP Mapper
*                 configurations.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "mapotn_loc.h"


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
** Forward References
*/

/*
** Public Functions
*/

/*******************************************************************************
*  mapotn_tgfpf_pause_frm_mem_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the 4 pause frame templates (memory & time) for the TGFPF block
*   in MAPOTN subsystem.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   pause_time          - PAUSE_TIME value for the 4 templates
*   pause_frame_mem     - PAUSE_FRAME_MEM value (4 * 18 * 32 bits)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_pause_frm_mem_cfg(mapotn_handle_t *mapotn_handle,
                                                UINT32 pause_time[MAPOTN_TGFPF_PAUSE_FRAME_TEMPLATE_CNT],
                                                UINT32 pause_frame_mem[MAPOTN_TGFPF_PAUSE_FRAME_MEM_TOTAL_REG_CNT])
{
    /* variable declaration */
    UINT32 registers_per_template = MAPOTN_TGFPF_PAUSE_FRAME_MEM_TOTAL_REG_CNT/MAPOTN_TGFPF_PAUSE_FRAME_TEMPLATE_CNT;
    UINT32 pause_frame_mem_template[registers_per_template];
    UINT32 template_iter;
    UINT32 pos;

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    for ( template_iter = 0; template_iter < (UINT32)MAPOTN_TGFPF_PAUSE_FRAME_TEMPLATE_CNT; ++template_iter)
    {
        mapotn_tgfpf_field_PAUSE_TIME_set(NULL, mapotn_handle, template_iter, pause_time[template_iter]);
    }

    for ( template_iter = 0; template_iter < (UINT32)MAPOTN_TGFPF_PAUSE_FRAME_TEMPLATE_CNT; ++template_iter)
    {
        for (pos = 0; pos < registers_per_template; ++pos)
        {
            pause_frame_mem_template[pos] = pause_frame_mem[template_iter*registers_per_template + pos];
        }
        mapotn_tgfpf_lfield_PAUSE_FRAME_MEM_set(NULL, mapotn_handle, template_iter, pause_frame_mem_template);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_pause_frm_mem_cfg */


/*******************************************************************************
* mapotn_tgfpf_pause_frm_insrt
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Inserts the pause XOFF frame into the GFP traffic. The XOFF pause template
*   to use for the channel must have been previously selected.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_pause_frm_insrt(mapotn_handle_t *mapotn_handle,
                                              UINT32 chnl)
{
    /* variable declaration */
    UINT32 pause_frm_rqst;
    UINT32 pause_frm_tx_int;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    /* variable initialization */
    pause_frm_rqst = 0 ;
    pause_frm_tx_int = 0;
    
    /* Ensure that PAUSE_FRAME_REQ is 0, i.e. there is no pause frame 
       transmission in progress */
    pause_frm_rqst = mapotn_tgfpf_lfield_range_PAUSE_FRAME_REQ_get(NULL, mapotn_handle, chnl, chnl);
    
    if(pause_frm_rqst == 0)
    {
        /* Set PAUSE_FRAME_REQ to 1 to initiate the pause frame transfer */
        mapotn_tgfpf_lfield_range_PAUSE_FRAME_REQ_set(NULL, mapotn_handle, chnl, chnl, TRUE);

        PMC_RETURN(PMC_SUCCESS);
    }
    else
    {
        PMC_RETURN(MAPOTN_TGFPF_ERR_PAUSE_FRAME_TX_IN_PROGRESS);
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_pause_frm_insrt */


/*******************************************************************************
* mapotn_tgfpf_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes and enables the TGFPF block.
*
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   energy_state      - enum defining state to set the TGFPF module to
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_init(mapotn_handle_t *mapotn_handle, 
                                   pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
   
    PMC_ENTRY();
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        /* Place TGFPF TSB in operational */
        mapotn_tgfpf_enbl(mapotn_handle, TRUE);
        PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
        ret_val = mapotn_tgfpf_block_non_default_cfg(mapotn_handle);
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = cfc_init(mapotn_handle->cfc_packing_handle, energy_state);
        }
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = cfc_bypass(mapotn_handle->cfc_packing_handle, FALSE);
        }
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = cfc_init(mapotn_handle->cfc_prefetch_handle, energy_state);
        }
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = cfc_bypass(mapotn_handle->cfc_prefetch_handle, FALSE); 
        }
        
        if(PMC_SUCCESS == ret_val)
        {
            /* initialize OHFS_INSERT */
            ret_val = ohfs_insert_init(mapotn_handle->ohfs_insert_handle, energy_state);
        }
        /* Configure the ordered set value (common to all channels) */
        if(PMC_SUCCESS == ret_val)
        {
            ret_val =  mapotn_tgfpf_order_set_upi_cfg(mapotn_handle,mapotn_handle->var.tgfpf_ordered_set_upi);
        }
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        ret_val = cfc_init(mapotn_handle->cfc_packing_handle, energy_state);

        if(PMC_SUCCESS == ret_val)
        {
            ret_val = cfc_init(mapotn_handle->cfc_prefetch_handle, energy_state);
        }
        if(PMC_SUCCESS == ret_val)
        {
            /* initialize OHFS_INSERT */
            ret_val = ohfs_insert_init(mapotn_handle->ohfs_insert_handle, energy_state);
        }
        /* Place TGFPF TSB in diabled mode*/
        mapotn_tgfpf_enbl(mapotn_handle, FALSE);  
        
    } else {
        PMC_ASSERT(FALSE, CFC_ERR_INVALID_PARAMETERS, 0, 0);
    }    
 
    PMC_RETURN(ret_val);

} /* mapotn_tgfpf_init */


/*******************************************************************************
* mapotn_tgfpf_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   TGFPF instance. It will test the channel mapping mode for all resources 
*   within the TGFPF instance if any are set to UTIL_GLOBAL_GFP then FALSE
*   is returned, else TRUE.  Should TRUE be returned it can be assumed that
*   no resources are provisioned against the TGFPF and it would be
*   possible for the higher layer module to put the module in lowpower standby
*   mode of operation.
*
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8             - FALSE:  Resource(s) not in start state \n
*                       TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 mapotn_tgfpf_start_state_test(mapotn_handle_t *mapotn_handle) 
{
    UINT32 i;
    PMC_ENTRY();
    PMC_ASSERT(mapotn_handle!=NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    for(i=0; i<MAPOTN_NUM_CHANL; i++)
    {    
        if(mapotn_handle->var.tx_chnl_ctxt[i].mapping_mode == UTIL_GLOBAL_GFP)
            PMC_RETURN(FALSE);        
    }
            
    if((ohfs_insert_start_state_test(mapotn_handle->ohfs_insert_handle) == FALSE) ||
       (cfc_start_state_test(mapotn_handle->cfc_packing_handle) == FALSE) ||
       (cfc_start_state_test(mapotn_handle->cfc_prefetch_handle) == FALSE))
        PMC_RETURN(FALSE);
        
    PMC_RETURN(TRUE);

} /* mapotn_tgfpf_start_state_test */


/*
** Private Functions
*/


/*******************************************************************************
* mapotn_tgfpf_chnl_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provisions a channel for the specifiec adaptaion function and ODUk rate.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_prov(mapotn_handle_t *mapotn_handle,
                                        UINT32 chnl)
{
    /* variable declaration */
    util_global_map_adapt_func_t adaptation_function;
    util_global_mapping_mode_t mapping_mode;
    util_global_odukp_type_t odukp_type;
    DOUBLE client_rate;
    UINT32 num_cal_entries;
    mapotn_src_dest_t mapper_source;
    UINT32 prefetch_thrld;
    UINT32 packing_low_thrld;
    UINT32 packing_high_thrld;
    UINT32 ext_hdr_enbl, ext_hdr_len;
    UINT32 pti_value, exi_value, upi_value;
    UINT32 upi_sel;
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 priority = 0;
    
    /* variable initialization */ 
    prefetch_thrld = 0;
    ext_hdr_enbl   = 0;
    ext_hdr_len    = 0;
    pti_value      = 0;
    exi_value      = 0;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* retrieve channel's status from memory */          
    adaptation_function = mapotn_handle->var.tx_chnl_ctxt[chnl].adaptation_func; 
    odukp_type          = mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type;      
    client_rate         = mapotn_handle->var.tx_chnl_ctxt[chnl].client_rate; 
    num_cal_entries     = mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries;
    mapping_mode        = mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode;
    mapper_source       = mapotn_handle->var.tx_chnl_ctxt[chnl].src_dest;
    
    if(UTIL_GLOBAL_MAP_ODU2P_10_GE_7_3_GFP == adaptation_function)
    {
        upi_sel = 1;
        upi_value = 0x14;
    }
    else 
    {
        upi_value = 0x1;
        upi_sel = 0;
    }


    if (UTIL_GLOBAL_GFP == mapping_mode)
    {
        priority = mapotn_tgfpf_field_CPB_MAP_get( NULL, mapotn_handle, 
                                                   chnl);
    }
    
    /* Disable auto_flow_control */
    mapotn_tgfpf_chnl_pause_frm_cfg(mapotn_handle, chnl, 0, priority,
                                    MAPOTN_PAUSE_FRM_MEMORY0,MAPOTN_PAUSE_FRM_MEMORY0); 
    
    /* Write the channel Configuration RAMs TX_CHAN_CFG_RAM0/1/2 with all the required fields. */
    mapotn_tgfpf_chnl_payload_hdr_cfg(mapotn_handle, chnl, ext_hdr_enbl, ext_hdr_len, pti_value, exi_value, upi_value, upi_sel);
    if (mapper_source == MAPOTN_DEST_ENET || UTIL_GLOBAL_MAP_ODU2P_10_GE_7_3_GFP == adaptation_function)
    {
        /* FCS disabled */
        mapotn_tgfpf_chnl_payload_fcs_cfg(mapotn_handle, chnl, MAPOTN_NO_FCS, 0, 0, 0, 0, 1, 1);
    }
    else
    {
        /* pFCS enabled */
        mapotn_tgfpf_chnl_payload_fcs_cfg(mapotn_handle, chnl, MAPOTN_PFCS, 0, 0, 0, 0, 1, 1);
    }
    mapotn_tgfpf_chnl_frm_scrmbl_cfg(mapotn_handle, chnl, 0, 1);


    /* CPB frame length */
    mapotn_tgfpf_chnl_prefetch_thrld_cfg(mapotn_handle, chnl, prefetch_thrld); 
    mapotn_tgfpf_chnl_client_data_frm_len_cfg(mapotn_handle,chnl,0x5f2, 0x0); 
    
    /* Channelize FIFO Config for PREFETCH CFC and PACKING CFC */
    /* Channelized FIFO Controller Inverted Page Table */
    
    ret_val = cfc_fifo_build(mapotn_handle->cfc_prefetch_handle, chnl, num_cal_entries);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }
    
    ret_val = cfc_fifo_build(mapotn_handle->cfc_packing_handle, chnl, num_cal_entries);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }
    
    /* Set prefetch fifo threshold */
    mapotn_tgfpf_prefetch_threshold_get(mapotn_handle, chnl, num_cal_entries, &prefetch_thrld);
    mapotn_tgfpf_chnl_prefetch_thrld_cfg(mapotn_handle, chnl, prefetch_thrld);

    
    /* Set packing fifo thresholds */
    mapotn_tgfpf_packing_threshold_get(mapotn_handle, chnl, num_cal_entries, &packing_low_thrld, &packing_high_thrld);
    mapotn_tgfpf_chnl_packer_thrld_cfg(mapotn_handle, chnl, packing_low_thrld, packing_high_thrld); 
                 

    /* OHFS INSERT Config  */
    ret_val = ohfs_insert_chnl_prov(mapotn_handle->ohfs_insert_handle, chnl, odukp_type, adaptation_function, client_rate, mapping_mode, (util_global_odu_line_payload_t)0, num_cal_entries);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_tgfpf_chnl_prov */


/*******************************************************************************
* mapotn_tgfpf_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisions the specific TGFPF channel.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_deprov(mapotn_handle_t *mapotn_handle,
                                          UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
     
    PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
    /* OHFS INSERT deprov  */
    ret_val = ohfs_insert_chnl_unprov(mapotn_handle->ohfs_insert_handle, chnl);
    if (ret_val != PMC_SUCCESS){
        PMC_RETURN(ret_val);
    }
    
    PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
    
    /* Destroy Channelize FIFO for PACKING CFC */
        ret_val = cfc_fifo_destroy(mapotn_handle->cfc_packing_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
    /* Destroy Channelize FIFO for PREFETCH CFC */
    ret_val = cfc_fifo_destroy(mapotn_handle->cfc_prefetch_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    
    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_tgfpf_chnl_deprov */


/*******************************************************************************
* mapotn_tgfpf_chnl_activate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Activates a provisioned channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
*                                                                               
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                                   
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                     
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_activate(mapotn_handle_t *mapotn_handle,
                                            UINT32 chnl) 

{
    /* variable declaration */
    mapotn_src_dest_t mapper_source;
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    /* Bring the CFC FIFOs out of reset */
    ret_val = ohfs_insert_chnl_activate(mapotn_handle->ohfs_insert_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    ret_val = cfc_fifo_activate(mapotn_handle->cfc_prefetch_handle,chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    ret_val = cfc_fifo_activate(mapotn_handle->cfc_packing_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
 
    /* Configure TX_CHAN_CPB_MAP[95:0] bits according to the channel traffic source. 
       Set to 1 for CPB traffic. */
    mapper_source       = mapotn_handle->var.tx_chnl_ctxt[chnl].src_dest;
    mapotn_tgfpf_chnl_mapping_cfg(mapotn_handle, chnl, mapper_source); 
    
    /* Take channel out of reset by de-asserting bit TX_CHAN_RESET */
    mapotn_tgfpf_chnl_reset(mapotn_handle, chnl, PMC_BLOCK_ALTERABLE);
     
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_activate */


/*******************************************************************************
* mapotn_tgfpf_chnl_deactivate
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Deactivates a provisioned channel. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                   
*                                                                               
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                     
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_deactivate(mapotn_handle_t *mapotn_handle,
                                              UINT32 chnl) 

{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* put channel in reset by asserting bit TX_CHAN_RESET */
    mapotn_tgfpf_chnl_reset(mapotn_handle, chnl, PMC_BLOCK_DEFAULT);

    /* Put the CFC FIFOs in reset */
    ret_val = ohfs_insert_chnl_deactivate(mapotn_handle->ohfs_insert_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    ret_val = cfc_fifo_deactivate(mapotn_handle->cfc_prefetch_handle,chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    ret_val = cfc_fifo_deactivate(mapotn_handle->cfc_packing_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    /* reset cpb_map back to 0 */
    mapotn_tgfpf_field_CPB_MAP_set( NULL, mapotn_handle, chnl, 0); 
     
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_deactivate */

/*******************************************************************************
* mapotn_tgfpf_chnl_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of TGFPF channels
*                                                                                                                                                     
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   *chnl               - set of channel ID to be cleaned
*   num_chnl            - number of channel ID to be cleaned
*
* OUTPUTS:
*   None
*
* RETURNS:
*   None.                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                        
PUBLIC void mapotn_tgfpf_chnl_clean( mapotn_handle_t  *mapotn_handle,
                                     UINT32           *chnl,
                                     UINT32            num_chnl )              
{
    /* variable declaration */

    UINT32 chnl_itr;
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    /* clean sub-block */
    cfc_fifo_clean(mapotn_handle->cfc_prefetch_handle, chnl,num_chnl);
    cfc_fifo_clean(mapotn_handle->cfc_packing_handle, chnl,num_chnl);
    ohfs_insert_chnl_clean(mapotn_handle->ohfs_insert_handle, chnl,num_chnl);

    for(chnl_itr = 0; chnl_itr< num_chnl;chnl_itr++)
    {
        mapotn_tgfpf_chnl_reset(mapotn_handle, chnl[chnl_itr], PMC_BLOCK_DEFAULT);
    }

    PMC_RETURN();

} /* mapotn_tgfpf_chnl_clean */

/*******************************************************************************
*  mapotn_tgfpf_block_non_default_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures Non-default register settings associated with the TGFPF block in 
*   MAPOTN subsystem.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_block_non_default_cfg(mapotn_handle_t *mapotn_handle)
{
    /* variable declaration */
    UINT32 chnl;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_LOG_TRACE("Block is in operational mode, applying non-default settings...\n");  
    for (chnl = 0; chnl < mapotn_handle->cfg.num_mapotn_chanl; chnl++)
    {
        mapotn_tgfpf_reg_TX_CHAN_CFG_RAM0_array_write(NULL, mapotn_handle,chnl,0x00000000);
        mapotn_tgfpf_reg_TX_CHAN_CFG_RAM1_array_write(NULL, mapotn_handle,chnl,0x00000007);
        mapotn_tgfpf_reg_TX_CHAN_CFG_RAM2_array_write(NULL, mapotn_handle,chnl,0x00000207);
        mapotn_tgfpf_reg_FRAME_LENGTH_CFG_array_write(NULL, mapotn_handle,chnl,0x05F20000);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_block_non_default_cfg */


/*******************************************************************************
* mapotn_tgfpf_ohfs_insert_csf_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the csf_insert of ohfs_insert configuration for the specified channel.
*
*
* INPUTS:
*   *mapotn_handle        - pointer to MAPOTN handle instance
*   chnl                  - channel ID to be configured
*                           Valid range: 0 - 95
*
* OUTPUTS:
*   csf_insrt             - CSF_INSERT value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_ohfs_insert_csf_cfg_get(mapotn_handle_t *mapotn_handle,
                                                       UINT32 chnl,
                                                       UINT32 *csf_insrt)
{
  /* variable declaration */

  PMC_ENTRY();
    
  PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
  PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

  ohfs_insert_csf_insert_get(mapotn_handle->ohfs_insert_handle, chnl, csf_insrt);

  PMC_RETURN(PMC_SUCCESS); 
    
} /* mapotn_tgfpf_ohfs_insert_csf_cfg_get */


/*******************************************************************************
*  mapotn_tgfpf_ext_hdr_mem_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the channel's extension header memory for the TGFPF block in
*   MAPOTN subsystem.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   ext_hdr_mem         - EXT_HDR_MEM value (16 * 32 bits)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_ext_hdr_mem_cfg(mapotn_handle_t *mapotn_handle,
                                              UINT32 chnl,
                                              UINT32 ext_hdr_mem[MAPOTN_TGFPF_EXTENSION_HEADER_REG_CNT])
{
    /* variable declaration */

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    mapotn_tgfpf_lfield_EXT_HDR_MEM_set(NULL, mapotn_handle, chnl, ext_hdr_mem);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_ext_hdr_mem_cfg */


/*******************************************************************************
* mapotn_tgfpf_enbl
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Places the TGFPF96 TSB in operational or disabled mode.
*
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   enable            - operational/disabled mode\n
*                       0 -- the TGFPF TSB ignores all data requests coming from 
*                            ODUK Scheduler (disabled mode)\n
*                       1 -- the TGFPF TSB will accept data requests from the 
*                            ODUK Scheduler, and provides valid data to the
*                            downstream OTN Tx Framer subsystem (operational)\n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_enbl(mapotn_handle_t *mapotn_handle, 
                                   UINT32 enable)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_tgfpf_field_GFPF_MAP_EN_set(NULL, mapotn_handle, enable);
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_enbl */


/*******************************************************************************
* mapotn_tgfpf_chnl_reset
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Resets a particular channel.
*
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   chnl_state        - register access state of TGFPF channel\n
*                       0 -- TGFPF channel is not in reset mode
*                            (PMC_BLOCK_ALTERABLE)\n
*                       1 -- TGFPF channel's pipeline, counters and state machines
*                            are reset to their default values
*                            (PMC_BLOCK_DEFAULT)\n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_reset(mapotn_handle_t *mapotn_handle,
                                         UINT32 chnl,
                                         pmc_block_reg_access_state_enum chnl_state)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_tgfpf_field_TX_CHAN_RESET_set(NULL, mapotn_handle, chnl, chnl_state);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_reset */


/*******************************************************************************
* mapotn_tgfpf_chnl_mapping_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures whether the channel is mapped to the CPB or not. 
*
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   mapper_source     - source of data stream\n 
*                       0 -- CPB Subsystem\n
*                       1 -- ENET_LINE Subsystem\n   
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_mapping_cfg(mapotn_handle_t *mapotn_handle,
                                               UINT32 chnl,
                                               mapotn_src_dest_t mapper_source)
{
    /* variable declaration */
    UINT32 chnl_cpb_map;
    
    /* variable initialization */ 
    chnl_cpb_map = 0;

    PMC_ENTRY();
    
    /* argument checking */
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(mapper_source <= MAPOTN_DEST_CPB_ENET, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    if(mapper_source == MAPOTN_DEST_CPB) /* CPB */
    {
        chnl_cpb_map = 1;
    }
    else if (mapper_source == MAPOTN_DEST_ENET) /* ENET */
    {
        chnl_cpb_map = 0; 
    }
         
    mapotn_tgfpf_field_CPB_MAP_set( NULL, mapotn_handle, chnl, chnl_cpb_map); 
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_mapping_cfg */


/*******************************************************************************
* mapotn_tgfpf_chnl_mapping_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   return Configures whether if the channel is  mapped to the CPB or not. 
*
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   mapotn_src_dest_t - mapped to ENET or CPB
*
* NOTES:
*
*******************************************************************************/
PUBLIC mapotn_src_dest_t mapotn_tgfpf_chnl_mapping_get(mapotn_handle_t *mapotn_handle,
                                                       UINT32 chnl)
{
    mapotn_src_dest_t src;
    UINT32 chnl_cpb_map;
    
    PMC_ENTRY();
    
    /* argument checking */
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    chnl_cpb_map = mapotn_tgfpf_field_CPB_MAP_get( NULL, mapotn_handle, chnl);

    if (chnl_cpb_map == 1)
    {
        src = MAPOTN_DEST_CPB;
    } else 
    {
        src = MAPOTN_DEST_ENET;
    }
                 
    PMC_RETURN(src);

} /* mapotn_tgfpf_chnl_mapping_get */


/*******************************************************************************
* mapotn_tgfpf_chnl_prefetch_thrld_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   For channels mapped to the CPB it configures the pre-fetch FIFO fill level 
*   at which XOFF will be signalled to the CPB for the channel.\n
*   
*   For channels mapped to the ENET-SS it configures the fill level below which 
*   the packing FIFO must fall before a pause or management frame can be  
*   inserted into the data stream.
*
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   thrld             - FIFO fill level
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_prefetch_thrld_cfg(mapotn_handle_t *mapotn_handle,
                                                      UINT32 chnl, 
                                                      UINT32 thrld)
{
    /* variable declaration */

    PMC_ENTRY();
    
    /* argument checking */
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_tgfpf_field_PREFETCH_FIFO_THRESH_set( NULL, mapotn_handle, chnl, thrld);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_prefetch_thrld_cfg */


/*******************************************************************************
* mapotn_tgfpf_chnl_packer_thrld_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the Packer FIFO thresholds.
*
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   low_thrld         - used to control insertion of GFP Idle frames in the 
*                       Packer FIFO, when the Packer FIFO occupancy <= 
*                       PACKER_FIFO_LOW_THRESH value for a channel. The default
*                       value is a threshold of 2-locations for a ODU0 channel  
*   high_thrld        - used to assert the back pressure to Prefetch FIFO, when
*                       Packer FIFO occupancy >= PACKER_FIFO_HIGH_THRESH value 
*                       for a channel. The default value is 7-locations.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_packer_thrld_cfg(mapotn_handle_t *mapotn_handle,
                                                    UINT32 chnl,
                                                    UINT32 low_thrld, 
                                                    UINT32 high_thrld)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_tgfpf_field_PACKER_FIFO_HIGH_THRESH_set( NULL, mapotn_handle, chnl, high_thrld);
    mapotn_tgfpf_field_PACKER_FIFO_LOW_THRESH_set( NULL, mapotn_handle, chnl, low_thrld);
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_packer_thrld_cfg */


/*******************************************************************************
* mapotn_tgfpf_chnl_pause_frm_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables the automatic insertion of Ethernet Pause Frames 
*  (automatic Flow control) periodically by the GFPF Mapper.
*
*
* INPUTS:
*   *mapotn_handle     - pointer to MAPOTN handle instance
*   chnl               - channel ID to be configured
*                        Valid range: 0 - 95
*   enable             - enables/disables\n
*                        0 -- automatic Flow control is disabled for the channel, 
*                             and the GFP Demapper indication will be ignored\n
*                        1 -- automatic Flow control is enabled for the channel\n
*   priority           - priority of GFP Pause/Management frames with respect to
*                        Client Data Frames \n
*                        0 -- Indicates Low Priority Insertion of Pause/Management
*                             Frames w.r.t Client Data Frames, i.e., GFP 
*                             Pause/Management Frames are inserted in data stream,
*                             when there is no client data arriving from the 
*                             upstream\n
*                        1 -- Indicates High Priority Insertion of 
*                             Pause/Management Frames w.r.t Client Data Frames\n
*   xon_pause           - select which of 4 pause frame templates is used to 
*                         transmit a XON Pause frame when the GFP Demapper 
*                         indicates that Pause Frames should no longer be 
*                         inserted:\n
*                         0 -- Pause frame Memory0 will be used for insertion.\n
*                         1 -- Pause frame Memory1 will be used for insertion.\n
*                         2 -- Pause frame Memory2 will be used for insertion.\n
*                         3 -- Pause frame Memory3 will be used for insertion.\n
*  xoff_pause           - select which of 4 pause frame templates is used to 
*                         insert Pause frames, when indicated by the GFP 
*                         Demapper or CPU.\n
*                         0 -- Pause frame Memory0 will be used for insertion.\n
*                         1 -- Pause frame Memory1 will be used for insertion.\n
*                         2 -- Pause frame Memory2 will be used for insertion.\n
*                         3 -- Pause frame Memory3 will be used for insertion.\n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_pause_frm_cfg(mapotn_handle_t *mapotn_handle,
                                                 UINT32 chnl, 
                                                 UINT32 enable,
                                                 UINT32 priority,                                                 
                                                 mapotn_tgfpf_pause_frm_memory_t   xon_pause,                   
                                                 mapotn_tgfpf_pause_frm_memory_t   xoff_pause )
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
     
    mapotn_tgfpf_field_AUTO_FLOW_CTRL_EN_set(NULL, mapotn_handle, chnl, enable);  
    mapotn_tgfpf_field_INSERT_FRM_PRIORITY_set(NULL, mapotn_handle, chnl, priority);
    mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XOFF_set(NULL, mapotn_handle, chnl, xoff_pause);
    mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XON_set(NULL, mapotn_handle, chnl, xon_pause);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_pause_frm_cfg */


/*******************************************************************************
* mapotn_tgfpf_chnl_pause_frm_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get configuration to enables/disables the automatic insertion of Ethernet Pause Frames 
*  (automatic Flow control) periodically by the GFPF Mapper.
*
*
* INPUTS:
*   *mapotn_handle     - pointer to MAPOTN handle instance
*   chnl               - channel ID to be configured
*                        Valid range: 0 - 95
*
* OUTPUTS:
*   *enable             - enables/disables\n
*                        0 -- automatic Flow control is disabled for the channel, 
*                             and the GFP Demapper indication will be ignored\n
*                        1 -- automatic Flow control is enabled for the channel\n
*   *priority           - priority of GFP Pause/Management frames with respect to
*                        Client Data Frames \n
*                        0 -- Indicates Low Priority Insertion of Pause/Management
*                             Frames w.r.t Client Data Frames, i.e., GFP 
*                             Pause/Management Frames are inserted in data stream,
*                             when there is no client data arriving from the 
*                             upstream\n
*                        1 -- Indicates High Priority Insertion of 
*                             Pause/Management Frames w.r.t Client Data Frames\n
*   *xon_pause           - select which of 4 pause frame templates is used to 
*                         transmit a XON Pause frame when the GFP Demapper 
*                         indicates that Pause Frames should no longer be 
*                         inserted:\n
*                         0 -- Pause frame Memory0 will be used for insertion.\n
*                         1 -- Pause frame Memory1 will be used for insertion.\n
*                         2 -- Pause frame Memory2 will be used for insertion.\n
*                         3 -- Pause frame Memory3 will be used for insertion.\n
*  *xoff_pause           - select which of 4 pause frame templates is used to 
*                         insert Pause frames, when indicated by the GFP 
*                         Demapper or CPU.\n
*                         0 -- Pause frame Memory0 will be used for insertion.\n
*                         1 -- Pause frame Memory1 will be used for insertion.\n
*                         2 -- Pause frame Memory2 will be used for insertion.\n
*                         3 -- Pause frame Memory3 will be used for insertion.\n
*
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_pause_frm_cfg_get(mapotn_handle_t *mapotn_handle,
                                                     UINT32 chnl, 
                                                     BOOL8 *enable,
                                                     BOOL8 *priority,                                                 
                                                     mapotn_tgfpf_pause_frm_memory_t   *xon_pause,                   
                                                     mapotn_tgfpf_pause_frm_memory_t   *xoff_pause )
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
     
    *enable = mapotn_tgfpf_field_AUTO_FLOW_CTRL_EN_get(NULL, mapotn_handle, chnl);  
    *priority = mapotn_tgfpf_field_INSERT_FRM_PRIORITY_get(NULL, mapotn_handle, chnl);

    *xoff_pause = (mapotn_tgfpf_pause_frm_memory_t) mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XOFF_get(NULL, mapotn_handle, chnl);
    *xon_pause = (mapotn_tgfpf_pause_frm_memory_t) mapotn_tgfpf_field_PAUSE_FRM_TEMPLATE_XON_get(NULL, mapotn_handle, chnl);
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_pause_frm_cfg_get */

                                  
/*******************************************************************************
*  mapotn_tgfpf_chnl_payload_hdr_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures payload header of the GFP Client Data frame with optional
*   extension header insertion into the GFP Client Data frame.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   ext_hdr_enbl        - defines whether or not the extension header is to be 
*                         inserted into the GFP Client Data frame\n
*                         0 -- the extension header is not inserted\n  
*                         1 -- the extension header is inserted\n                    
*   ext_hdr_len         - define the byte length of the Extension Header 
*                         (including the eHEC field) that is to be inserted in 
*                          the Client Data frame
*   pti_value           - Payload Type Indicator (PTI) of the GFP Payload Header
*                         field
*   exi_value           - Extension Header Indicator (EXI) of the GFP Payload 
*                         Header field
*   upi_value           - User Payload Indicator (UPI) of the GFP Payload 
*                         Header field
*   upi_sel             - the source of GFP UPI field\n
*                         0 -- UPI is selected from upstream\n
*                         1 -- UPI is selected from the DPI type bus\n
*                         NOTE: upi_sel should be set '1' for the channels 
*                         carrying GSUP43 C7.3 traffic (data & ordered sets)
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_payload_hdr_cfg(mapotn_handle_t *mapotn_handle,
                                                   UINT32 chnl,
                                                   UINT32 ext_hdr_enbl,
                                                   UINT32 ext_hdr_len,
                                                   UINT32 pti_value,
                                                   UINT32 exi_value,
                                                   UINT32 upi_value,
                                                   UINT32 upi_sel)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_tgfpf_field_EXT_HDR_EN_set(NULL, mapotn_handle, chnl, ext_hdr_enbl);
    mapotn_tgfpf_field_EXT_HDR_LEN_set(NULL, mapotn_handle, chnl, ext_hdr_len);
    mapotn_tgfpf_field_TYPE_HDR_PTI_set(NULL, mapotn_handle, chnl, pti_value);
    mapotn_tgfpf_field_TYPE_HDR_EXI_set(NULL, mapotn_handle, chnl, exi_value);
    mapotn_tgfpf_field_TYPE_HDR_UPI_set(NULL, mapotn_handle, chnl, upi_value);
    mapotn_tgfpf_field_UPI_SEL_set( NULL, mapotn_handle, chnl, upi_sel);
    
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_payload_hdr_cfg */


/*******************************************************************************
* mapotn_tgfpf_chnl_payload_fcs_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures whether or not the payload FCS field is inserted in  a GFP frame.
*
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   fcs_mode          - define whether or not the payload FCS field is inserted 
*                       in a GFP frame. See mapotn_fcs_mode_t \n 
*                       0 -- Do not insert any FCS\n
*                       1 -- Insert pFCS for GFP Frame\n
*                       2 -- Calculate eFCS for special Ethernet traffic received
*                            from the CPB after skipping 4-bytes of the packet\n
*                       3 -- Calculate eFCS for special Ethernet traffic received
*                            from the CPB after skipping 8-bytes of the packet\n
*                       4 -- Calculate eFCS for special Ethernet traffic received
*                            from the CPB after skipping 12-bytes of the packet\n
*                       5 -- Calculate eFCS for special Ethernet traffic received
*                            from the CPB after skipping 16-bytes of the packet\n
*                       6 -- Calculate eFCS for special Ethernet traffic received
*                           from the CPB including the preamble bytes\n
*   pfcs_rev_obyte    - the byte positions of the FCS value coming out of the 
*                       FCS Engine. 
*                       valid only when fcs_mode is set to 1\n
*                       0 -- FCS values from the FCS generator logic are not byte 
*                            reversed\n
*                       1 -- FCS values from the FCS generator logic are byte 
*                            reversed\n
*   pfcs_rev_obit     - the order of bits of each FCS byte coming out of FCS 
*                       Engine.
*                       valid only when fcs_mode is set to 1\n
*                       0 -- each byte of FCS values from the FCS generator logic 
*                            are not bit reversed\n
*                       1 -- each byte of FCS values from the FCS generator logic 
*                            are bit reversed\n
*   pfcs_rev_ibit     - the order of payload data bits sent to the FCS 
*                       calculation logic. 
*                       valid only when fcs_mode is set to 1\n
*                       0 -- bytes from the GFP Payload Information field 
*                            excluding the FCS bytes, are sent to the FCS checking
*                            logic most significant bit (MSB) first\n
*                       1 -- bytes from the GFP Payload Information field, are 
*                            sent to the FCS calculation logic with their bits 
*                            reversed\n
*   pfcs_rev_crpt_inv - defines whether or not the payload FCS field is to be 
*                       inverted if client data frame is erred, before being 
*                       appended to the GFP frame. 
*                       valid only when fcs_mode is set to 1\n
*                       0 -- the value of the payload FCS field of a GFP frame is 
*                            not inverted if erred data frame is encountered\n
*                       1 -- the one's complement of the payload FCS field is 
*                            appended to a GFP frame\n
*   pfcs_rev_inv     - defines whether or not the value of the payload FCS 
*                       field is inverted before being appended to a GFP frame.
*                       valid only when fcs_mode is set to 1\n
*                       0 -- the value of the payload FCS field of a GFP
*                            frame is not inverted\n
*                       1 -- the one's complement of the payload FCS field is
*                            appended to a GFP frame\n
*   pfcs_rev_init    - defines the initial residue value of the CRC used for 
*                       calculating the FCS of the GFP Payload Information field.
*                       valid only when fcs_mode is set to 1\n
*                       0 -- the initial residue is all-zeros\n
*                       1 -- the initial residue for the FCS calculation is
*                            all ones.\n
*
*
*                     
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_payload_fcs_cfg(mapotn_handle_t *mapotn_handle,
                                                   UINT32 chnl, 
                                                   mapotn_fcs_mode_t fcs_mode,     
                                                   UINT32 pfcs_rev_obyte,    
                                                   UINT32 pfcs_rev_obit,     
                                                   UINT32 pfcs_rev_ibit,     
                                                   UINT32 pfcs_rev_crpt_inv, 
                                                   UINT32 pfcs_rev_inv,      
                                                   UINT32 pfcs_rev_init)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    /* argument checking */
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(fcs_mode <= MAPOTN_EFCS, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* Configure FCS_INS_POS[1:0] indicate the number of
       Preamble bytes present in incoming data traffic */
    if ((fcs_mode > MAPOTN_PFCS) && (fcs_mode < MAPOTN_EFCS)){
        mapotn_tgfpf_field_FCS_INS_POS_set(NULL, mapotn_handle, chnl, fcs_mode-2);
    }

    /* Configure FCS_INS[1:0] control bits */
    if (fcs_mode < MAPOTN_EFCS_4_BYTES) 
    {
        mapotn_tgfpf_field_FCS_INS_set(NULL, mapotn_handle, chnl, fcs_mode);
    } 
    else if (fcs_mode == MAPOTN_EFCS) 
    {
        mapotn_tgfpf_field_FCS_INS_set(NULL, mapotn_handle, chnl, 0x3);
    } 
    else 
    {
        mapotn_tgfpf_field_FCS_INS_set(NULL, mapotn_handle, chnl, 0x2);
    }
    
    if (fcs_mode == MAPOTN_PFCS) 
    {
        mapotn_tgfpf_field_PFCS_REV_OBYTE_set(NULL, mapotn_handle, chnl, pfcs_rev_obyte);
        mapotn_tgfpf_field_PFCS_REV_OBIT_set(NULL, mapotn_handle, chnl, pfcs_rev_obit);
        mapotn_tgfpf_field_PFCS_REV_IBIT_set(NULL, mapotn_handle, chnl, pfcs_rev_ibit);
        mapotn_tgfpf_field_PFCS_CRPT_INV_set(NULL, mapotn_handle, chnl, pfcs_rev_crpt_inv);
        mapotn_tgfpf_field_PFCS_INV_set(NULL, mapotn_handle, chnl, pfcs_rev_inv);
        mapotn_tgfpf_field_PFCS_INIT_set(NULL, mapotn_handle, chnl, pfcs_rev_init);
    }
    
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_tgfpf_chnl_payload_fcs_cfg */


/*******************************************************************************
* mapotn_tgfpf_chnl_frm_scrmbl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables/disables the scrambling for GFP Payload area and Core header.
*
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   payload_scrmbl    - defines whether or not payload scrambling is enabled or
*                       not\n
*                       0 -- the GFP Payload Area is scrambled\n
*                       1 -- the GFP Payload Information field is not scrambled\n
*   core_hdr_scrmbl   - defines whether or not transmitted GFP Core Headers are 
*                       scrambled with the DC balancing code 0xB6AB31E0\n
*                       0 -- GFP Core Headers are non-scrambled\n
*                       1 -- GFP Core Headers are scrambled\n                    
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_frm_scrmbl_cfg(mapotn_handle_t *mapotn_handle,
                                                  UINT32 chnl, 
                                                  UINT32 payload_scrmbl, 
                                                  UINT32 core_hdr_scrmbl)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_tgfpf_field_SCRMBL_set(NULL, mapotn_handle, chnl, payload_scrmbl);
    mapotn_tgfpf_field_DC_BALANCE_set(NULL, mapotn_handle, chnl, core_hdr_scrmbl);
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_frm_scrmbl_cfg */


/*******************************************************************************
*  mapotn_tgfpf_chnl_client_data_frm_len_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the minimum and maximum lengths for client data frames that 
*   should be counted for a channel. 
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                     
*   max_frm_len         - maximum byte length of client data frames can be 
*                         received from the CPB
*   min_frm_len         - minimum byte length of client data frames can be 
*                         received from the CPB
*                        
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_client_data_frm_len_cfg(mapotn_handle_t *mapotn_handle,
                                                           UINT32 chnl,
                                                           UINT32 max_frm_len, 
                                                           UINT32 min_frm_len)
{
    /* variable declaration */

    PMC_ENTRY(); 
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    mapotn_tgfpf_field_MAX_FRM_LENGTH_set(NULL, mapotn_handle, chnl, max_frm_len);
    mapotn_tgfpf_field_MIN_FRM_LENGTH_set(NULL, mapotn_handle, chnl, min_frm_len);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_client_data_frm_len_cfg */



/*******************************************************************************
*  mapotn_tgfpf_order_set_upi_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures User Payload Indentifier(UPI) value of ordered set frames. This
*   configuration is used only when ethernet FCS check is enabled. 
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   upi_value           - User Payload Indicator (UPI) value for the channels
*                         carrying GSUP43 C7.3 traffic (data & ordered sets)
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_order_set_upi_cfg(mapotn_handle_t *mapotn_handle,
                                                UINT32 upi_value)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_tgfpf_field_ORD_SET_UPI_set(NULL, mapotn_handle, upi_value);
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_order_set_upi_cfg */

/* LCOV_EXCL_START */
/*******************************************************************************
*  mapotn_tgfpf_order_set_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the configured User Payload Indentifier(UPI) value for ordered set
*   frames.
*
*
* INPUTS:
*   *mapotn_handle     - pointer to MAPOTN handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32             - User Payload Indicator (UPI) for ordered set frames.
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 mapotn_tgfpf_order_set_upi_get(mapotn_handle_t *mapotn_handle)
{
    UINT32 upi;
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    upi = mapotn_tgfpf_field_ORD_SET_UPI_get(NULL, mapotn_handle);
    
    PMC_RETURN(upi);

} /* mapotn_tgfpf_order_set_upi_get */
/* LCOV_EXCL_END */



/*******************************************************************************
*  mapotn_tgfpf_chnl_ext_hdr_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the extension header of the GFP Client Data frame.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   ext_hdr_enbl        - defines whether or not the extension header is to be 
*                         inserted into the GFP Client Data frame\n
*                         0 -- the extension header is not inserted\n  
*                         1 -- the extension header is inserted\n                   
*   ext_hdr_len         - define the byte length of the Extension Header 
*                         (including the eHEC field) that is to be inserted in 
*                          the Client Data frame
*   ext_hdr_mem         - ext_hdr_mem value
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_ext_hdr_cfg(mapotn_handle_t *mapotn_handle,
                                               UINT32 chnl,
                                               UINT32 ext_hdr_enbl,
                                               UINT32 ext_hdr_len, 
                                               UINT32 ext_hdr_mem[MAPOTN_TGFPF_EXTENSION_HEADER_REG_CNT])
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_tgfpf_field_EXT_HDR_EN_set(NULL, mapotn_handle, chnl, ext_hdr_enbl);
    mapotn_tgfpf_field_EXT_HDR_LEN_set(NULL, mapotn_handle, chnl, ext_hdr_len);
    
    mapotn_tgfpf_ext_hdr_mem_cfg(mapotn_handle, chnl, ext_hdr_mem);
    
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_ext_hdr_cfg */


/*******************************************************************************
*  mapotn_tgfpf_chnl_ext_hdr_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves Extension header enable and lenght field of the GFP Client Data
*   frame for a channel.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
*
* OUTPUTS:
*   *ext_hdr_enbl       - pointer to storage for EXT HDR EN field  
*   *ext_hdr_len        - pointer to storage for EXT HDR LEN field
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_ext_hdr_get(mapotn_handle_t *mapotn_handle,
                                               UINT32 chnl,
                                               UINT32 *ext_hdr_enbl,
                                               UINT32 *ext_hdr_len)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    *ext_hdr_enbl = mapotn_tgfpf_field_EXT_HDR_EN_get(NULL, mapotn_handle, chnl);
    *ext_hdr_len = mapotn_tgfpf_field_EXT_HDR_LEN_get(NULL, mapotn_handle, chnl); 
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_ext_hdr_get */


/*******************************************************************************
* mapotn_tgfpf_chnl_payload_fcs_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves whether or not the payload FCS field is inserted in a GFP frame.
*
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*
*                     
* OUTPUTS:
*   fcs_mode          - pointer to the payload FCS field if is inserted 
*                       in a GFP frame. See mapotn_fcs_mode_t \n 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_payload_fcs_get(mapotn_handle_t *mapotn_handle,
                                                   UINT32 chnl, 
                                                   mapotn_fcs_mode_t *fcs_mode)
{
    /* variable declaration */
    UINT32 fcs_ins_pos = 0;
    UINT32 fcs_ins = 0;
    mapotn_fcs_mode_t fcs_m = MAPOTN_NO_FCS;
    
    PMC_ENTRY();
    
    /* argument checking */
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    fcs_ins = mapotn_tgfpf_field_FCS_INS_get(NULL, mapotn_handle, chnl); 
    
    if (fcs_ins == 0)
    {
        fcs_m = MAPOTN_NO_FCS;
    }
    else if (fcs_ins == 1)
    {
        fcs_m= MAPOTN_PFCS;
    }
    else if (fcs_ins == 2)
    {
        fcs_ins_pos = mapotn_tgfpf_field_FCS_INS_POS_get(NULL, mapotn_handle, chnl);
        if(fcs_ins_pos == 0)
        {
            fcs_m = MAPOTN_EFCS_4_BYTES;
        }
        if(fcs_ins_pos == 1)
        {
            fcs_m = MAPOTN_EFCS_8_BYTES;
        }
        if(fcs_ins_pos == 2)
        {
            fcs_m = MAPOTN_EFCS_12_BYTES;
        }
        if(fcs_ins_pos == 3)
        {
            fcs_m = MAPOTN_EFCS_16_BYTES;
        }
    }
    else if (fcs_ins == 3)
    {
        fcs_m = MAPOTN_EFCS;
    }
    else
    {
        PMC_RETURN(MAPOTN_ERR_CODE_ASSERT);
    }
    
    *fcs_mode = fcs_m;
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_tgfpf_chnl_payload_fcs_get */


/*******************************************************************************
* mapotn_tgfpf_chnl_pti_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the Payload Type Indicator (PTI) of the GFP Payload Header field for a 
*   channel.
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   pti_value         - Payload Type Indicator (PTI) of the GFP Payload Header
*                       field                   
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_pti_set(mapotn_handle_t *mapotn_handle,
                                           UINT32 chnl, UINT32 pti_value)
{
    PMC_ENTRY();

    /* argument checking */
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_tgfpf_field_TYPE_HDR_PTI_set(NULL, mapotn_handle, chnl, pti_value);
     
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_pti_set */  


/*******************************************************************************
* mapotn_tgfpf_chnl_pti_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves Payload Type Indicator field of the GFP frame for a channel.
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*
* OUTPUTS:
*   *pti_value        - pointer to storage for Payload Type Indicator value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_pti_get(mapotn_handle_t *mapotn_handle,
                                           UINT32 chnl, UINT32 *pti_value)
{
    PMC_ENTRY();

    /* argument checking */
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    *pti_value = mapotn_tgfpf_field_TYPE_HDR_PTI_get(NULL, mapotn_handle, chnl);
     
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_pti_get */
                                              

/*******************************************************************************
* mapotn_tgfpf_chnl_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the User Payload Indicator (UPI) of the GFP Payload Header field for a
*   channel.
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   upi_value         - User Payload Indicator (UPI) of the GFP Payload 
*                       Header field
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_upi_set(mapotn_handle_t *mapotn_handle,
                                           UINT32 chnl, UINT32 upi_value)
{

   PMC_ENTRY();

    /* argument checking */
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_tgfpf_field_TYPE_HDR_UPI_set(NULL, mapotn_handle, chnl, upi_value);
     
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_upi_set */  


/*******************************************************************************
* mapotn_tgfpf_chnl_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves User Payload Indicator field of the GFP frame for a channel.
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*
* OUTPUTS:
*   *upi_value        - pointer to storage for User Payload Indicator value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_upi_get(mapotn_handle_t *mapotn_handle,
                                           UINT32 chnl, UINT32 *upi_value)
{
    PMC_ENTRY();

    /* argument checking */
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    *upi_value = mapotn_tgfpf_field_TYPE_HDR_UPI_get(NULL, mapotn_handle, chnl);
     
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_upi_get */


/*******************************************************************************
* mapotn_tgfpf_chnl_upi_sel_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the Selector of the User Payload Indicator (UPI) of the GFP Payload
*   Header field for a channel.
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   upi_sel           - when TRUE, UPI is selected from the TYPE_HDR_UPI register field.
*                       when FALSE, UPI is selected from 8 MSBs of the data type sideband input.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_upi_sel_set(mapotn_handle_t *mapotn_handle,
                                               UINT32           chnl, 
                                               BOOL8            upi_sel)
{

   PMC_ENTRY();

    /* argument checking */
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_tgfpf_field_UPI_SEL_set(NULL, mapotn_handle, chnl, TRUE == upi_sel ? 1 : 0 );
     
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_upi_sel_set */  


/*******************************************************************************
* mapotn_tgfpf_chnl_upi_sel_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the Selector of the User Payload Indicator (UPI) of the GFP Payload
*   Header field for a channel.
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*
* OUTPUTS:
*   *upi_sel          - pointer to storage for User Payload Indicator selector
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_upi_sel_get(mapotn_handle_t *mapotn_handle,
                                               UINT32           chnl, 
                                               BOOL8           *upi_sel)
{
    PMC_ENTRY();

    /* argument checking */
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(upi_sel != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    *upi_sel = (0 == mapotn_tgfpf_field_UPI_SEL_get(NULL, mapotn_handle, chnl)) ? FALSE : TRUE;
     
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_upi_sel_get */

/*******************************************************************************
* mapotn_tgfpf_chnl_exi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the Extension Header Indicator (EXI) of the GFP Payload Header field 
*   for a channel.
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*   exi_value         - Extension Header Indicator (EXI) of the GFP Payload 
*                       Header field
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_exi_set(mapotn_handle_t *mapotn_handle,
                                           UINT32 chnl, UINT32 exi_value)
{

   PMC_ENTRY();

    /* argument checking */
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_tgfpf_field_TYPE_HDR_EXI_set(NULL, mapotn_handle, chnl, exi_value);
     
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_exi_set */  


/*******************************************************************************
* mapotn_tgfpf_chnl_exi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves Extension Header Indicator field of the GFP frame for a channel.
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   chnl              - channel ID to be configured
*                       Valid range: 0 - 95
*
* OUTPUTS:
*   *exi_value        - pointer to storage for Extension Header Indicator value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_exi_get(mapotn_handle_t *mapotn_handle,
                                           UINT32 chnl, UINT32 *exi_value)
{
    PMC_ENTRY();

    /* argument checking */
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    *exi_value = mapotn_tgfpf_field_TYPE_HDR_EXI_get(NULL, mapotn_handle, chnl);
     
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_exi_get */


/*******************************************************************************
* mapotn_tgfpf_pmon_latch_trig_set
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Transfer the current PMON count to a holding register and resets the PMON count.  
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*                                                                             
* OUTPUTS:
*   None
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                                
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_pmon_latch_trig_set(mapotn_handle_t *mapotn_handle)
{
    UINT32 pmon_latch_trig = 1;
    UINT32 count = 100, count_itr = 0;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    pmon_latch_trig =     mapotn_tgfpf_field_PMON_UPDATE_get(NULL, mapotn_handle);
    while(count_itr < count && pmon_latch_trig == 1)
    {
        PMC_Q_USLEEP(mapotn_handle,100);
        pmon_latch_trig =     mapotn_tgfpf_field_PMON_UPDATE_get(NULL, mapotn_handle);
        count_itr += 1;
    }
    if (pmon_latch_trig == 1)
    {
        PMC_RETURN(MAPOTN_BLOCK_NOT_OPERATIONAL);
    }
    mapotn_tgfpf_field_PMON_UPDATE_set(NULL, mapotn_handle, 1);
    pmon_latch_trig =    mapotn_tgfpf_field_PMON_UPDATE_get(NULL, mapotn_handle);
    while(count_itr < count && pmon_latch_trig == 1)
    {
        PMC_Q_USLEEP(mapotn_handle,100);
        pmon_latch_trig =     mapotn_tgfpf_field_PMON_UPDATE_get(NULL, mapotn_handle);
        count_itr += 1;
    }
    if (pmon_latch_trig == 1)
    {
        PMC_RETURN(MAPOTN_BLOCK_NOT_OPERATIONAL);
    }    
    

    PMC_RETURN(PMC_SUCCESS);
}  /* mapotn_tgfpf_pmon_latch_trig_set */


/*******************************************************************************
* mapotn_tgfpf_fcs_corrupt_set
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Enable or disable corruption of GFP frame payload FCS when a errored data 
*   frame is encountered for a given channel.
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to obtain idle frame count from
*   enable              - when TRUE, GFP frame payload FCS is corrupted when
*                         a errored frame is encountered.
*                         when FALSE, corruption is disabled.
*
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
PUBLIC void mapotn_tgfpf_fcs_corrupt_set(mapotn_handle_t    *mapotn_handle,
                                         UINT32              chnl,
                                         BOOL8               enable)
{
    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_tgfpf_field_PFCS_CRPT_INV_set(NULL, mapotn_handle, chnl, (UINT32)enable);

    PMC_RETURN();
}  /* mapotn_tgfpf_fcs_corrupt_set */


/*******************************************************************************
* mapotn_tgfpf_fcs_corrupt_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Returns the state of PFCS_CRPT_INV register that corrupts payload
*   FCS of a GFP frame when a errored data frame is encountered.
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to obtain idle frame count from
*
*                                                                             
* OUTPUTS:   
*   None.                                                                   
*                                                                               
* RETURNS:                                                                      
*    BOOL8              - The state of PFCS_CRPT_INV register that corrupts 
*                         payload FCS of a GFP frame.
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC BOOL8 mapotn_tgfpf_fcs_corrupt_get(mapotn_handle_t    *mapotn_handle,
                                          UINT32              chnl)
{
    BOOL8 fcs_crpt_state;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    fcs_crpt_state = (BOOL8)mapotn_tgfpf_field_PFCS_CRPT_INV_get(NULL, mapotn_handle, chnl);

    PMC_RETURN(fcs_crpt_state);
}  /* mapotn_tgfpf_fcs_corrupt_get */


/*******************************************************************************
* mapotn_tgfpf_activated_chnl_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns all started channels
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*                                                                             
* OUTPUTS:
*   *actv_chnl          - all activated channels
*
* RETURNS:
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void mapotn_tgfpf_activated_chnl_get(mapotn_handle_t *mapotn_handle,
                                            BOOL8 *actv_chnl)
{
    UINT32 itr;
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    for (itr = 0; itr < MAPOTN_NUM_CHANL; itr++)
    {
        actv_chnl[itr] = mapotn_tgfpf_field_TX_CHAN_RESET_get(NULL, mapotn_handle, itr)==1?FALSE:TRUE;
    }

    PMC_RETURN();
}  /* mapotn_tgfpf_activated_chnl_get */


/*******************************************************************************
* mapotn_tgfpf_prefetch_threshold_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get the prefetch fifo threshold value for the specified number of calendar
*   slots.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   num_cal_entries     - number of calendar entries for the channel
*
* OUTPUTS:
*   *prefetch_thresh    - pointer to the calculated prefetch fifo threshold
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_prefetch_threshold_get(mapotn_handle_t *mapotn_handle,
                                                     UINT32 chnl,
                                                     UINT32 num_cal_entries,
                                                     UINT32 *prefetch_thresh)
{
    /* variable declaration */

    /* variable initialization */
    *prefetch_thresh = 0;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    if (num_cal_entries == 1)
    {
        *prefetch_thresh = 1;
    }
    else
    {
        if (mapotn_handle->var.tx_chnl_ctxt[chnl].src_dest == MAPOTN_DEST_ENET)
        {
            /* For ENET traffic, this threshold is the management/pause frame
               insertion threshold and packing fifo high threshold minus 3
               Note that the lowest ENET rate is ODU2 (8 slots) */
            *prefetch_thresh = num_cal_entries * 3 - 4;
        }
        else
        {
            /* The PREFETCH_FIFO_THRESH value for a particular channel should be set
               to one half the size of the FIFO plus 1 */
            *prefetch_thresh = num_cal_entries + 1;
        };
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_prefetch_threshold_get */


/*******************************************************************************
* mapotn_tgfpf_packing_threshold_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get the packing fifo threshold value for the specified number of calendar
*   slots.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   num_cal_entries     - number of calendar entries for the channel
*
* OUTPUTS:
*   *packing_low_thrld  - pointer to the calculated low packing fifo threshold
*   *packing_high_thrld - pointer to the calculated high packing fifo threshold
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_packing_threshold_get(mapotn_handle_t *mapotn_handle,
                                                    UINT32 chnl,
                                                    UINT32 num_cal_entries,
                                                    UINT32 *packing_low_thrld,
                                                    UINT32 *packing_high_thrld)
{
    /* variable declaration */

    /* variable initialization */
    *packing_low_thrld = 0;
    *packing_high_thrld = 0;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    /* The PACKER_FIFO_LOW_THRESH value for a particular channel is set to
      set to 1/4 of the fifo size to handle misaligned calendars  */
    /* The PACKER_FIFO_HIGH_THRESH value for a particular channel is set to
      set to 3/4 of the fifo size minus one to handle misaligned calendars  */
    if (num_cal_entries == 1)
    {
        *packing_high_thrld = 4;
        *packing_low_thrld = 1;
    }
    else if (num_cal_entries <= 4)
    {
        *packing_high_thrld = num_cal_entries * 3;
        *packing_low_thrld = num_cal_entries;
    }
    else
    {
        *packing_high_thrld = num_cal_entries * 3 - 1;
        *packing_low_thrld = num_cal_entries;
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_packing_threshold_get */



/*
** End of file
*/
