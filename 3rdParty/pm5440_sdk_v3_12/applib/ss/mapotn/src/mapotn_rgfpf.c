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
*   DESCRIPTION : This file contains C functions for the MAPOTN GFP De-mapper
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
#ifndef DOXYGEN_PUBLIC_ONLY
                                          
PRIVATE PMC_ERROR mapotn_rgfpf_fltr_client_frames(mapotn_handle_t *mapotn_handle, 
                                                  UINT32 chnl);
                                                    
PRIVATE PMC_ERROR mapotn_rgfpf_fltr_mcf_frames(mapotn_handle_t *mapotn_handle, 
                                               UINT32 chnl);
                                                 
PRIVATE PMC_ERROR mapotn_rgfpf_fltr_cmf_frames(mapotn_handle_t *mapotn_handle, 
                                               UINT32 chnl, 
                                               mapotn_rgfpf_cmf_dest_t cmf_dest); 
                                                 
PRIVATE PMC_ERROR mapotn_rgfpf_fltr_cmf_upi_range(mapotn_handle_t *mapotn_handle, 
                                                  UINT32 chnl, 
                                                  mapotn_rgfpf_cmf_dest_t cmf_dest,
                                                  UINT32 upi_range_start, 
                                                  UINT32 upi_range_end,
                                                  UINT32 next_filter_offset);
                                                                                                  
PRIVATE PMC_ERROR mapotn_rgfpf_fltr_add(mapotn_handle_t *mapotn_handle, 
                                        UINT32 chnl, 
                                        mapotn_rgfpf_fltr_t *f);
                                          
PRIVATE PMC_ERROR mapotn_rgfpf_default_filter_cfg(mapotn_handle_t *mapotn_handle, 
                                                 UINT32 chnl,
                                                 util_global_map_adapt_func_t adaptation_function);

PRIVATE void mapotn_rgfpf_filter_cfg(mapotn_handle_t *mapotn_handle,
                                    UINT32 chnl, 
                                    UINT32 fltr_index, 
                                    mapotn_rgfpf_fltr_t *fltr_ptr);

PRIVATE void mapotn_rgfpf_filter_cfg_get(mapotn_handle_t *mapotn_handle,
                                         UINT32 chnl, 
                                         UINT32 fltr_index, 
                                         mapotn_rgfpf_fltr_t *fltr_ptr);

PRIVATE PMC_ERROR mapotn_rgfpf_fc1200_filter_cfg(mapotn_handle_t *mapotn_handle,
                                                 UINT32 chnl);
#endif

/*
** Public Functions
*/

/*******************************************************************************
* mapotn_rgfpf_client_classify_prov
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Performs basic GFPF filter configuration.  It configures the GFP-F filters 
*   to classify PTI types of b'000 (Data), b'100 (CMF) and b'101 (MCF).
*   These identified data streams will be sent to the CPB, CPU FW port, or the
*   CSF FIFO.  The mapping is as follows:
*
*   PTI b'000 -> CPB\n
*   PTI b'101 (MCF) -> CPU FW port\n 
*   PTI b'100 (CMF) -> CPU FW port or CSF FIFO\n 
*
*   The PTI b'100 (CMF) may be configured to filter out a further subset of the
*   available UPI range.  Upto two UPI ranges are supported.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                                                   
*   cmf_dest            - CMF (PTI = 100) Destination:\n 
*                         0 -- CPU FW Port\n
*                         1 -- CSF FIFO\n
*   cmf_upi_filt_mode   - CMF (PTI b'100) UPI Filtering Mode:\n
*                         0 -- All UPI values to CPU FW port\n
*                         1 -- One range of UPI values to CPU FW port, other 
*                             UPI values dropped\n
*                         2 -- Two range of UPI values to CPU FW port, other
*                             UPI values dropped\n
*   upi_range_1_start   - UPI Range 1 Filter Start Value
*                         Valid for cmf_upi_filt_mode set to 1 or 2
*   upi_range_1_end     - UPI Range 1 Filter End Value
*                         Valid for cmf_upi_filt_mode set to 1 or 2
*   upi_range_2_start   - UPI Range 2 Filter Start Value
*                         Valid for cmf_upi_filt_mode set to 2
*   upi_range_2_end     - UPI Range 2 Filter End Value
*                         Valid for cmf_upi_filt_mode set to 2
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
*                                                                               
*******************************************************************************/

PUBLIC PMC_ERROR mapotn_rgfpf_client_classify_prov(mapotn_handle_t *mapotn_handle,
                                                   UINT32 chnl, mapotn_rgfpf_cmf_dest_t cmf_dest,
                                                   mapotn_rgfpf_cmf_upi_filt_mode_t cmf_upi_filt_mode,
                                                   UINT32 upi_range_1_start, UINT32 upi_range_1_end, 
                                                   UINT32 upi_range_2_start, UINT32 upi_range_2_end)
{                                               
    PMC_ERROR res = MAPOTN_RGFPF_ERR_INVALID_PARAMETERS;
    UINT32 range_1_no_filters = 0;
    UINT32 range_2_no_filters = 0;
    mapotn_rgfpf_chnl_fltr_mgmt_t * fltr_mgmt;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(cmf_dest <= MAPOTN_CSF_FIFO, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(cmf_upi_filt_mode <= MAPOTN_FORWARD_TWO_UPI_RANGE, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    fltr_mgmt = &(mapotn_handle->var.mapotn_rgfpf_chnl_fltr_mgmt[chnl]);

    /* mapotn_rgfpf_client_classify_prov should only be called once.  */ 
    if (fltr_mgmt->client_classify_prov)
    {
        PMC_RETURN(MAPOTN_ERR_CLIENT_CLASSIFY_FILTERS_ALREADY_PROV);
    } 
    fltr_mgmt->client_classify_prov = TRUE;

    /* send all client frames to the egress */
    mapotn_rgfpf_fltr_client_frames(mapotn_handle, chnl);

    /* send all MCF frames to the CPU */
    mapotn_rgfpf_fltr_mcf_frames(mapotn_handle, chnl);

    /* send CMF frames to the CPU or CSF fifo -  either all CMF frames, or CMF frames with a UPI value within 1 or 2 ranges.*/
    switch(cmf_upi_filt_mode) 
    {
        case MAPOTN_FORWARD_ALL_UPI:
            /* CMF frames with any UPI value - send to cpu or csf fifo */
            res = mapotn_rgfpf_fltr_cmf_frames(mapotn_handle,chnl,cmf_dest);
            break;
        case MAPOTN_FORWARD_ONE_UPI_RANGE: 
            /* One range of UPI values to CPU FW port, other UPI values will be handled by the next filter */
            res = mapotn_rgfpf_fltr_cmf_upi_range(mapotn_handle,chnl,cmf_dest,upi_range_1_start,upi_range_1_end,2);
            break;
        case MAPOTN_FORWARD_TWO_UPI_RANGE: 
            /* Two range of UPI values to CPU FW port, other UPI values will be handled by the next filter */

            /* calculate the # of filters for each range (value will be used to determine the next filter offset) */
            range_1_no_filters  = (upi_range_1_start == 0 || upi_range_1_end == 0xff || upi_range_1_start == upi_range_1_end) ? 1 : 2;
            range_2_no_filters  = (upi_range_2_start == 0 || upi_range_2_end == 0xff || upi_range_2_start == upi_range_2_end) ? 1 : 2;
        
            res = mapotn_rgfpf_fltr_cmf_upi_range(mapotn_handle,chnl,cmf_dest,upi_range_1_start,upi_range_1_end,range_1_no_filters + range_2_no_filters);
            res |= mapotn_rgfpf_fltr_cmf_upi_range(mapotn_handle,chnl,cmf_dest,upi_range_2_start,upi_range_2_end,range_2_no_filters);
            break;
        default:
            res = MAPOTN_RGFPF_ERR_INVALID_PARAMETERS;
    }      
    PMC_RETURN(res);
}

/*******************************************************************************
* mapotn_rgfpf_user_client_classify_prov
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Performs user specific GFP frame classification.  The basic classification
*   configuration performed by mapotn_rgfpf_client_classify_prov, will exclude
*   all data with PTI not matching b'000 (Data), b'100 (CMF), and b'101 (MCF).
*   For CMF some UPI values will have been excluded based on the CMF UPI range
*   selection configuration.  These excluded data streams are internally
*   dropped.  
*
*   This function provides the ability to:\n
*   1) Transfer an excluded data stream to CPB of CPU FW port.\n
*   2) Transfer data bound to CPB to CPU FW port or internally drop it.\n
*   3) Transfer data bound to CPU FW port to CPB or internally drop it.\n
*
*   The classification and action may occur prior to the basic filter operation
*   or after it.  For items 2 and 3 above if the destination is CPU FW port or
*   CPB then the classification must occur prior to the basic classification.
*
*   This function returns an error if there are not sufficient filter resources
*   available in the GFP filter block for the requested filtering.
*   
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95   
*   frame_field_mask    - Mask of frame fields to use for classification\n
*                         bit 0 -- Process PTI comparison\n
*                         bit 1 -- Process UPI comparison\n
*                         bit 2 -- Process EXI comparison\n
*                         bit 3 -- Process PFI comparison\n
*   pti_comp            - Comparison type for PTI value\n
*                         MAPOTN_FLD_EQ -- matches\n
*                         MAPOTN_FLD_NQ -- does not match\n
*                         MAPOTN_FLD_GT -- greater than\n
*                         MAPOTN_FLD_LT -- less than\n
*   pti_value           - Value of PTI to compare with received frame
*   upi_comp            - Comparison type for UPI value\n
*                         MAPOTN_FLD_EQ -- matches\n
*                         MAPOTN_FLD_NQ -- does not match\n
*   upi_value           - Value of UPI to compare with received frame
*   exi_comp            - Comparison type for EXI value\n
*                         MAPOTN_FLD_EQ -- matches\n
*                         MAPOTN_FLD_NQ -- does not match\n
*   exi_value           - Value of EXI to compare with received frame
*   pfi_comp            - Comparison type for PFI value\n
*                         MAPOTN_FLD_EQ -- matches\n
*                         MAPOTN_FLD_NQ -- does not match\n
*   pfi_value           - Value of PFI to compare with received frame (0 or 1)\n
*   proc_seq            - Unused parameter - set to 0.\n
*   action              - Action to perform on identified GFP Frames\n
*                         MAPOTN_ACTION_DROP   -- Drop\n
*                         MAPOTN_ACTION_EGRESS -- Send to gress\n
*                         MAPOTN_ACION_CPU     -- Send to CPU FW Port\n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                                
*                                                                               
* NOTES:                                                                            
*        PFI is implemented using a generic filter. Therefore, the user can only 
*        call this function once with a mask selecting PFI. Otherwise the 
*        generic bytes will get replaced for the channel.
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_user_client_classify_prov(mapotn_handle_t *mapotn_handle, 
                                                        UINT32 chnl, 
                                                        UINT32 frame_field_mask, 
                                                        mapotn_rgfpf_comparison_op_t pti_comp, 
                                                        UINT32 pti_value,
                                                        mapotn_rgfpf_comparison_op_t upi_comp, 
                                                        UINT32 upi_value,
                                                        mapotn_rgfpf_comparison_op_t exi_comp, 
                                                        UINT32 exi_value,
                                                        mapotn_rgfpf_comparison_op_t pfi_comp, 
                                                        UINT32 pfi_value,
                                                        mapotn_rgfpf_proc_seq_t proc_seq, 
                                                        mapotn_rgfpf_classify_actions_t action)
{
    mapotn_rgfpf_fltr_t f;
    mapotn_rgfpf_fltr_t * f_ptr = &f;
    UINT32 comp_op = 0;
    UINT32 flt_action;
    PMC_ERROR res = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(pti_comp <= MAPOTN_FLD_LT, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(upi_comp <= MAPOTN_FLD_LT, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(exi_comp <= MAPOTN_FLD_LT, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(pfi_comp <= MAPOTN_FLD_LT, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(proc_seq <= MAPOTN_POST_CLASSIFY, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(action <= MAPOTN_ACTION_CPU, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_MEMSET(f_ptr,0,sizeof(f));   
   
    PMC_LOG(PMC_LOG_SEV_LOW, MAPOTN_LOG_CODE_LOG, 0, 0, 
        "client classify: Ch = %u, frame mask = %u\n", chnl,frame_field_mask);  
    
    /* translate 'action' values to filter action values */
    switch (action)
    {
        case MAPOTN_ACTION_DROP:    flt_action = MAPOTN_DROP_THE_FRAME; break;
        case MAPOTN_ACTION_EGRESS:  flt_action = MAPOTN_SEND_TO_EGRESS; break;
        case MAPOTN_ACTION_CPU:     flt_action = MAPOTN_SEND_TO_CPU; break;
        default: PMC_RETURN(MAPOTN_RGFPF_ERR_INVALID_PARAMETERS);
    }

    /* PFI filter - use generic filter */
    /* PFI is the 5th bit in the 5th byte of the frame */
    if (frame_field_mask & MAPOTN_FLD_MASK_PFI) 
    {
        f.generic_val = (pfi_value << 28);
        f.generic_msk =0x10000000;        /* pfi is the 5th bit */
        f.generic_ofs = 4;            /* pfi is in the 5th byte */
        f.select_generic = 1; 
        f.match_indicator = 0;
        f.match_indicator = MAPOTN_USE_EXI_SET | MAPOTN_USE_PTI_SET;
        comp_op = (pfi_comp ? MAPOTN_GEN_NQ : MAPOTN_GEN_EQ);
    } 
    else  
    {
        /* Otherwise it's a exi/upi/pti filter */
        f.match_indicator = MAPOTN_USE_EXI_SET | MAPOTN_USE_PTI_SET | MAPOTN_USE_UPI_SET; 

        /* UPI filter - Equal or Not equal to upi_value */
        if (frame_field_mask & MAPOTN_FLD_MASK_UPI)
        { 
            f.upi_msk = 0xff;
            f.upi_val = upi_value;
            /*
                Note: the api was defined with upi comparison operators: = and !=.
                This implementation supports =,!=,<,>.
            */
            switch (upi_comp)
            {
                case MAPOTN_FLD_EQ: comp_op |= MAPOTN_UPI_EQ; break;
                case MAPOTN_FLD_NQ: comp_op |= MAPOTN_UPI_NQ; break;
                case MAPOTN_FLD_LT: comp_op |= MAPOTN_UPI_LT; break;
                case MAPOTN_FLD_GT: comp_op |= MAPOTN_UPI_GT; break;
                default: PMC_RETURN(MAPOTN_RGFPF_ERR_INVALID_PARAMETERS);
            }
            /* Trigger a upi defect if the frame is dropped */
            if (flt_action == MAPOTN_DROP_THE_FRAME)
            {
                f.match_indicator ^= MAPOTN_USE_UPI_SET;
            }
        }

        /* EXI filter - Equal or Not equal to exi_value */
        if (frame_field_mask & MAPOTN_FLD_MASK_EXI)
        {  
            f.exi_msk = 0xf;
            f.exi_val = exi_value;
            /*
                Note: the api was defined with just exi comparison operators: = and !=.
                This implementation supports =,!=,<,>.
                If you want to limit it to = and !=, then use this line instead:
                comp_op |= (exi_comp ? EXI_NQ : EXI_EQ);
            */
            switch (exi_comp)
            {
                case MAPOTN_FLD_EQ: comp_op |= MAPOTN_EXI_EQ; break;
                case MAPOTN_FLD_NQ: comp_op |= MAPOTN_EXI_NQ; break;
                case MAPOTN_FLD_LT: comp_op |= MAPOTN_EXI_LT; break;
                case MAPOTN_FLD_GT: comp_op |= MAPOTN_EXI_GT; break;
                default: PMC_RETURN(MAPOTN_RGFPF_ERR_INVALID_PARAMETERS);
            }
            /* enable the exi defect */
            if (flt_action == MAPOTN_DROP_THE_FRAME)
            {
                f.match_indicator ^= MAPOTN_USE_EXI_SET;
            }
        }

        /* PTI filter = equal, not equal, <, > */
        if (frame_field_mask & MAPOTN_FLD_MASK_PTI) 
        {  
            f.pti_msk = 7;
            f.pti_val = pti_value;
            switch (pti_comp)
            {
                case MAPOTN_FLD_EQ: comp_op |= MAPOTN_PTI_EQ; break;
                case MAPOTN_FLD_NQ: comp_op |= MAPOTN_PTI_NQ; break;
                case MAPOTN_FLD_LT: comp_op |= MAPOTN_PTI_LT; break;
                case MAPOTN_FLD_GT: comp_op |= MAPOTN_PTI_GT; break;
                default: PMC_RETURN(MAPOTN_RGFPF_ERR_INVALID_PARAMETERS);
            }
            if (flt_action == MAPOTN_DROP_THE_FRAME)
            {
                f.match_indicator ^= MAPOTN_USE_PTI_SET;
            }
        }
    }
    f.comp_operator = comp_op;
    f.action = flt_action;

    f.next_fltr_ofs = 0xf; /* last filter */
    f.cont_operator = 1;   /* Override */
    res = mapotn_rgfpf_fltr_add(mapotn_handle,chnl,&f);
    PMC_RETURN(res);
} /* mapotn_rgfpf_user_client_classify_prov */

/*******************************************************************************
* mapotn_rgfpf_classify_unprov
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Unprovisions all 10 RGFPF filters for a channel.
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
PUBLIC PMC_ERROR mapotn_rgfpf_classify_unprov(mapotn_handle_t *mapotn_handle, 
                                              UINT32 chnl)    
{
    mapotn_rgfpf_chnl_fltr_mgmt_t * fltr_mgmt;
    UINT32 i;

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    /* Get the index of the next filter to be configured */
    fltr_mgmt = &(mapotn_handle->var.mapotn_rgfpf_chnl_fltr_mgmt[chnl]);

    mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_0_array_write(NULL, mapotn_handle, chnl,0);
    mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_1_array_write(NULL, mapotn_handle, chnl,0);
    mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_2_array_write(NULL, mapotn_handle, chnl,0); 
    
    for (i=0;i < MAPOTN_RGFPF_CHNL_NUM_FILTERS; i++)
    {
        mapotn_rgfpf_reg_FILTER_CONFIGURATION_0_array_write(NULL, mapotn_handle, i, chnl, 0);  
        mapotn_rgfpf_reg_FILTER_CONFIGURATION_1_array_write(NULL, mapotn_handle, i, chnl ,0x00001e80); /* drop all */
    }
     
    fltr_mgmt->client_classify_prov = FALSE;
    fltr_mgmt->next_filter = 0;
    PMC_RETURN(PMC_SUCCESS);
} /* mapotn_rgfpf_classify_unprov */

/*******************************************************************************
* mapotn_rgfpf_classify_cfg
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Configure one particular RGFPF filter. This function can be used as an
*   alternative to the mapotn_rgfpf_user_client_classify_prov and 
*   mapotn_rgfpf_client_classify_prov functions in cases where set/get operators
*   are needed.\n
*
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95   
*   filter_no           - Filter number
*   frame_field_mask    - Mask of frame fields to use for classification\n
*                         bit 0 -- Process PTI comparison\n
*                         bit 1 -- Process UPI comparison\n
*                         bit 2 -- Process EXI comparison\n
*                         bit 3 -- Process PFI comparison\n
*   pti_comp            - Comparison type for PTI value\n
*                         MAPOTN_FLD_EQ -- matches\n
*                         MAPOTN_FLD_NQ -- does not match\n
*                         MAPOTN_FLD_GT -- greater than\n
*                         MAPOTN_FLD_LT -- less than\n
*   pti_value           - Value of PTI to compare with received frame
*   upi_comp            - Comparison type for UPI value\n
*                         MAPOTN_FLD_EQ -- matches\n
*                         MAPOTN_FLD_NQ -- does not match\n
*   upi_value           - Value of UPI to compare with received frame
*   exi_comp            - Comparison type for EXI value\n
*                         MAPOTN_FLD_EQ -- matches\n
*                         MAPOTN_FLD_NQ -- does not match\n
*   exi_value           - Value of EXI to compare with received frame
*   action              - Action to perform on identified GFP Frames\n
*                         MAPOTN_ACTION_DROP   -- Drop\n
*                         MAPOTN_ACTION_EGRESS -- Send to gress\n
*                         MAPOTN_ACION_CPU     -- Send to CPU FW Port\n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                                
*                                                                               
* NOTES:                                                                            
*   See digi_mapper_gfp_classify_cfg for details on usage.
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_classify_cfg(mapotn_handle_t *mapotn_handle, 
                                           UINT32 chnl, 
                                           UINT32 filter_no,
                                           UINT32 frame_field_mask, 
                                           mapotn_rgfpf_comparison_op_t pti_comp, 
                                           UINT32 pti_value,
                                           mapotn_rgfpf_comparison_op_t upi_comp, 
                                           UINT32 upi_value,
                                           mapotn_rgfpf_comparison_op_t exi_comp, 
                                           UINT32 exi_value,
                                           mapotn_rgfpf_classify_actions_t action)
{
    mapotn_rgfpf_fltr_t f;
    mapotn_rgfpf_fltr_t * f_ptr = &f;
    UINT32 comp_op = 0;
    mapotn_filter_action_type_t flt_action;
    PMC_ERROR res = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(pti_comp <= MAPOTN_FLD_LT, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(upi_comp <= MAPOTN_FLD_LT, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(exi_comp <= MAPOTN_FLD_LT, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(action < LAST_MAPOTN_ACTION, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_MEMSET(f_ptr,0,sizeof(f));   
   
    PMC_LOG(PMC_LOG_SEV_LOW, MAPOTN_LOG_CODE_LOG, 0, 0, 
        "filter cfg: Ch = %u, frame mask = %u\n", chnl,frame_field_mask);  
    /* translate 'action' values to filter action values */
    switch (action)
    {
        case MAPOTN_ACTION_DROP:
          flt_action = MAPOTN_DROP_THE_FRAME; 
          break;

        case MAPOTN_ACTION_EGRESS:
          flt_action = MAPOTN_SEND_TO_EGRESS; 
          break;

        case MAPOTN_ACTION_CPU:
        case MAPOTN_ACTION_CPU_NOCSF:
          flt_action = MAPOTN_SEND_TO_CPU; 
          break;

        case MAPOTN_ACTION_CSF:     
          flt_action = MAPOTN_DROP_THE_FRAME_STORE_UPI_CMF; 
          break;
        default: PMC_RETURN(MAPOTN_RGFPF_ERR_INVALID_PARAMETERS);
    }

    
    /* Otherwise it's a exi/upi/pti filter */
    f.match_indicator = MAPOTN_USE_EXI_SET | MAPOTN_USE_PTI_SET | MAPOTN_USE_UPI_SET; 

    if (MAPOTN_SEND_TO_CPU == flt_action && MAPOTN_ACTION_CPU == action)
    {
        f.match_indicator |= MAPOTN_USE_ECSF;
    }

    /* UPI filter - Equal or Not equal to upi_value */
    if (frame_field_mask & MAPOTN_FLD_MASK_UPI)
    { 
        f.upi_msk = 0xff;
        f.upi_val = upi_value;
        
        switch (upi_comp)
        {
            case MAPOTN_FLD_EQ: comp_op |= MAPOTN_UPI_EQ; break;
            case MAPOTN_FLD_NQ: comp_op |= MAPOTN_UPI_NQ; break;
            case MAPOTN_FLD_LT: comp_op |= MAPOTN_UPI_LT; break;
            case MAPOTN_FLD_GT: comp_op |= MAPOTN_UPI_GT; break;
            default: PMC_RETURN(MAPOTN_RGFPF_ERR_INVALID_PARAMETERS);
        }
        /* Trigger a upi defect if the frame is dropped */
        if (flt_action == MAPOTN_DROP_THE_FRAME)
        {
            f.match_indicator ^= MAPOTN_USE_UPI_SET;
        }
    }

    /* EXI filter - Equal or Not equal to exi_value */
    if (frame_field_mask & MAPOTN_FLD_MASK_EXI)
    {  
        f.exi_msk = 0xf;
        f.exi_val = exi_value;
        
        switch (exi_comp)
        {
            case MAPOTN_FLD_EQ: comp_op |= MAPOTN_EXI_EQ; break;
            case MAPOTN_FLD_NQ: comp_op |= MAPOTN_EXI_NQ; break;
            case MAPOTN_FLD_LT: comp_op |= MAPOTN_EXI_LT; break;
            case MAPOTN_FLD_GT: comp_op |= MAPOTN_EXI_GT; break;
            default: PMC_RETURN(MAPOTN_RGFPF_ERR_INVALID_PARAMETERS);
        }
        /* enable the exi defect */
        if (flt_action == MAPOTN_DROP_THE_FRAME)
        {
            f.match_indicator ^= MAPOTN_USE_EXI_SET;
        }
    }

    /* PTI filter = equal, not equal, <, > */
    if (frame_field_mask & MAPOTN_FLD_MASK_PTI) 
    {  
        f.pti_msk = 7;
        f.pti_val = pti_value;
        switch (pti_comp)
        {
            case MAPOTN_FLD_EQ: comp_op |= MAPOTN_PTI_EQ; break;
            case MAPOTN_FLD_NQ: comp_op |= MAPOTN_PTI_NQ; break;
            case MAPOTN_FLD_LT: comp_op |= MAPOTN_PTI_LT; break;
            case MAPOTN_FLD_GT: comp_op |= MAPOTN_PTI_GT; break;
            default: PMC_RETURN(MAPOTN_RGFPF_ERR_INVALID_PARAMETERS);
        }
        if (flt_action == MAPOTN_DROP_THE_FRAME)
        {
            f.match_indicator ^= MAPOTN_USE_PTI_SET;
        }
    }
    f.comp_operator = comp_op;
    f.action = flt_action;

    f.next_fltr_ofs = 0xf; /* last filter */
    f.cont_operator = 1;   /* Override */
    mapotn_rgfpf_filter_cfg(mapotn_handle,chnl,filter_no,&f);
    PMC_RETURN(res);
} /* mapotn_rgfpf_classify_cfg */

/*******************************************************************************
* mapotn_rgfpf_classify_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Retrieve the configuration of a particular filter. 
*
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle         - Pointer to MAPOTN handle instance
*   chnl                   - Channel ID to be configured
*                            Valid range: 0 - 95   
*   filter_no              - Filter Number (0..9)
*   *frame_field_mask_ptr  - Mask of frame fields to use for classification\n
*                            bit 0 -- Process PTI comparison\n
*                            bit 1 -- Process UPI comparison\n
*                            bit 2 -- Process EXI comparison\n
*   *pti_comp_ptr          - Comparison type for PTI value\n
*                            MAPOTN_FLD_EQ -- matches\n
*                            MAPOTN_FLD_NQ -- does not match\n
*                            MAPOTN_FLD_GT -- greater than\n
*                            MAPOTN_FLD_LT -- less than\n
*   *pti_value_ptr         - Value of PTI to compare with received frame
*   *upi_comp_ptr          - Comparison type for UPI value\n
*                            MAPOTN_FLD_EQ -- matches\n
*                            MAPOTN_FLD_NQ -- does not match\n
*   *upi_value_ptr         - Value of UPI to compare with received frame
*   *exi_comp_ptr          - Comparison type for EXI value\n
*                            MAPOTN_FLD_EQ -- matches\n
*                            MAPOTN_FLD_NQ -- does not match\n
*   *exi_value_ptr         - Value of EXI to compare with received frame
*   *action_ptr            - Action to perform on identified GFP Frames\n
*                            MAPOTN_ACTION_DROP   -- Drop\n
*                            MAPOTN_ACTION_EGRESS -- Send to gress\n
*                            MAPOTN_ACION_CPU     -- Send to CPU FW Port\n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                                
*                                                                               
* NOTES:                                                                            
*   This function should be used to retrieve the filter configuration set by
*   calls to mapotn_rgfpf_filter_set. 
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_classify_get(mapotn_handle_t *mapotn_handle, 
                                          UINT32 chnl, 
                                          UINT32 filter_no,
                                          UINT32 *frame_field_mask_ptr, 
                                          mapotn_rgfpf_comparison_op_t *pti_comp_ptr, 
                                          UINT32 *pti_value_ptr,
                                          mapotn_rgfpf_comparison_op_t *upi_comp_ptr, 
                                          UINT32 *upi_value_ptr,
                                          mapotn_rgfpf_comparison_op_t *exi_comp_ptr, 
                                          UINT32 *exi_value_ptr,
                                          mapotn_rgfpf_classify_actions_t *action_ptr)
{
    mapotn_rgfpf_fltr_t f;
    mapotn_rgfpf_fltr_t * f_ptr = &f;
    UINT32 comp_op = 0;
    PMC_ERROR res = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, chnl, 0);
    PMC_ASSERT(filter_no < MAPOTN_RGFPF_CHNL_NUM_FILTERS, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, filter_no,0);

    PMC_MEMSET(f_ptr,0,sizeof(f));   
  
    mapotn_rgfpf_filter_cfg_get(mapotn_handle,chnl, filter_no, f_ptr);

    if (f_ptr->select_generic)
    {
        /* The mapotn_rgfpf_filter_set function doesn't configure generic filters -> return an error */
        PMC_RETURN(MAPOTN_RGFPF_ERR_UNEXPECTED_FILTER_CFG);
    }
   
    comp_op = f_ptr->comp_operator; 

    switch (comp_op & 3)
    {
        case MAPOTN_UPI_EQ: *upi_comp_ptr = MAPOTN_FLD_EQ; break;
        case MAPOTN_UPI_NQ: *upi_comp_ptr = MAPOTN_FLD_NQ; break;
        case MAPOTN_UPI_LT: *upi_comp_ptr = MAPOTN_FLD_LT; break;
        case MAPOTN_UPI_GT: *upi_comp_ptr = MAPOTN_FLD_GT; break;
        default: PMC_ASSERT(FALSE, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS,0,0);
    }

    switch (comp_op & 0xc)
    {
        case MAPOTN_PTI_EQ: *pti_comp_ptr = MAPOTN_FLD_EQ; break;
        case MAPOTN_PTI_NQ: *pti_comp_ptr = MAPOTN_FLD_NQ; break;
        case MAPOTN_PTI_LT: *pti_comp_ptr = MAPOTN_FLD_LT; break;
        case MAPOTN_PTI_GT: *pti_comp_ptr = MAPOTN_FLD_GT; break;
        default: PMC_ASSERT(FALSE, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS,0,0);
    }
    switch (comp_op & 0x30)
    {
        case MAPOTN_EXI_EQ: *exi_comp_ptr = MAPOTN_FLD_EQ; break;
        case MAPOTN_EXI_NQ: *exi_comp_ptr = MAPOTN_FLD_NQ; break;
        case MAPOTN_EXI_LT: *exi_comp_ptr = MAPOTN_FLD_LT; break;
        case MAPOTN_EXI_GT: *exi_comp_ptr = MAPOTN_FLD_GT; break;
        default: PMC_ASSERT(FALSE, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS,0,0);
    }
        
    /* translate 'action' values */
    switch ((mapotn_filter_action_type_t)f_ptr->action)
    {
        case MAPOTN_DROP_THE_FRAME: *action_ptr = MAPOTN_ACTION_DROP;break;
        case MAPOTN_SEND_TO_EGRESS: *action_ptr = MAPOTN_ACTION_EGRESS;break;
        case MAPOTN_SEND_TO_CPU: *action_ptr = MAPOTN_ACTION_CPU;break;
        case MAPOTN_DROP_THE_FRAME_STORE_UPI_CMF: *action_ptr = MAPOTN_ACTION_CSF;break;
        default: PMC_RETURN(MAPOTN_RGFPF_ERR_INVALID_PARAMETERS);
    }

    *pti_value_ptr = f_ptr->pti_val;
    *upi_value_ptr = f_ptr->upi_val;
    *exi_value_ptr = f_ptr->exi_val;

    *frame_field_mask_ptr = 0;
    if (f_ptr->pti_msk == 7)
    {
        *frame_field_mask_ptr |= MAPOTN_FLD_MASK_PTI;
    }
    if (f_ptr->exi_msk == 0xf)
    {
        *frame_field_mask_ptr |= MAPOTN_FLD_MASK_EXI;
    }
    if (f_ptr->upi_msk == 0xff)
    {
        *frame_field_mask_ptr |= MAPOTN_FLD_MASK_UPI;
    }

    PMC_RETURN(res);
} /* mapotn_rgfpf_classify_get */




/*******************************************************************************
* mapotn_rgfpf_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initializes and enables the RGFPF block.
*
*
* INPUTS:
*   *mapotn_handle    - pointer to MAPOTN handle instance
*   energy_state      - enum defining state to set the RGFPF module to
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
PUBLIC PMC_ERROR mapotn_rgfpf_init(mapotn_handle_t *mapotn_handle, 
                                   pmc_energy_state_t energy_state)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
   
    PMC_ENTRY();
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    if(PMC_ENERGY_STATE_RUNNING == energy_state)
    {
        
        PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);       
        ret_val = mapotn_rgfpf_block_non_default_cfg(mapotn_handle);
       
        PMC_ATOMIC_YIELD(mapotn_handle->base.parent_handle,0);
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = cfc_init(mapotn_handle->cfc_unpack_handle, energy_state);
        }
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = cfc_bypass(mapotn_handle->cfc_unpack_handle, FALSE);
        }
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = cfc_init(mapotn_handle->cfc_egress_handle, energy_state);
        }
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = cfc_bypass(mapotn_handle->cfc_egress_handle, FALSE); 
        }
        
        if(PMC_SUCCESS == ret_val)
        {
            /* initialize OHFS_REMOVE */
            ret_val = ohfs_remove_init(mapotn_handle->ohfs_remove_handle, energy_state);
        }
        /* Configure the Ordered set UPI value (common to all channels) */
        if(PMC_SUCCESS == ret_val)
        {
            ret_val = mapotn_rgfpf_ordered_set_upi_cfg(mapotn_handle,mapotn_handle->var.rgfpf_ordered_set_upi);
        }

        if (PMC_SUCCESS == ret_val)
        {
            UINT32 i;
            for (i=0; i < MAPOTN_NUM_CHANL; i++)
            {
                mapotn_rgfpf_field_EGRESS_FCS_DISCARD_set(NULL,
                                                          mapotn_handle,
                                                          i,
                                                          1);
            }
        }
    }
    else if (PMC_ENERGY_STATE_RESET == energy_state)
    {
        ret_val = cfc_init(mapotn_handle->cfc_unpack_handle, energy_state);

        if(PMC_SUCCESS == ret_val)
        {
            ret_val = cfc_init(mapotn_handle->cfc_egress_handle, energy_state);
        }
        if(PMC_SUCCESS == ret_val)
        {
            /* initialize OHFS_REMOVE */
            ret_val = ohfs_remove_init(mapotn_handle->ohfs_remove_handle, energy_state);
        }
        
    } else {
        PMC_ASSERT(FALSE, CFC_ERR_INVALID_PARAMETERS, 0, 0);
    }    
 
    PMC_RETURN(ret_val);

} /* mapotn_rgfpf_init */   


/*******************************************************************************
* mapotn_rgfpf_start_state_test
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function identifies if any resources are provisioned against the 
*   RGFPF instance. It will test the channel mapping mode for all resources 
*   within the RGFPF instance if any are set to UTIL_GLOBAL_GFP then FALSE
*   is returned, else TRUE.  Should TRUE be returned it can be assumed that
*   no resources are provisioned against the RGFPF and it would be
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
*   BOOL8                 - FALSE:  Resource(s) not in start state \n
*                           TRUE:   Resources all in start state
*
* NOTES:
*
*******************************************************************************/
PUBLIC BOOL8 mapotn_rgfpf_start_state_test(mapotn_handle_t *mapotn_handle) 
{
    UINT32 i;
    PMC_ENTRY();
    PMC_ASSERT(mapotn_handle!=NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    for(i=0; i<MAPOTN_NUM_CHANL; i++)
    {    
        if(mapotn_handle->var.rx_chnl_ctxt[i].mapping_mode == UTIL_GLOBAL_GFP)
            PMC_RETURN(FALSE);        
    }
    
    if((cfc_start_state_test(mapotn_handle->cfc_unpack_handle) == FALSE) ||
       (cfc_start_state_test(mapotn_handle->cfc_egress_handle) == FALSE))
        PMC_RETURN(FALSE);
        
    PMC_RETURN(TRUE);

} /* mapotn_rgfpf_start_state_test */                                                 
                                                                           

/*
** Private Functions
*/

/*******************************************************************************
*  mapotn_rgfpf_block_non_default_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures Non-default register settings associated with the RGFPF block in 
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
PUBLIC PMC_ERROR mapotn_rgfpf_block_non_default_cfg(mapotn_handle_t *mapotn_handle)
{
    /* variable declaration */
    UINT32 chnl;
    UINT32 itr;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_LOG_TRACE("Block is in operational mode, applying non-default settings...\n");  
    for (chnl = 0; chnl < mapotn_handle->cfg.num_mapotn_chanl; chnl++)
    {
        /* initialize RAMs */
        mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_0_array_write(NULL, mapotn_handle, chnl,0x1a40301e);
        mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_1_array_write(NULL, mapotn_handle, chnl,0x0400003e);
        mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_2_array_write(NULL, mapotn_handle, chnl,0x00001808);
        mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_3_array_write(NULL, mapotn_handle, chnl,0x00000000);
        mapotn_rgfpf_reg_FRAME_LENGTH_CHECK_CONFIGURATION_array_write(NULL, mapotn_handle, chnl, 0xFFFF0000);
        for (itr = 0; itr < MAPOTN_FRM_LEN_STAT_COUNTER_CONFIG_N; itr += 1)
        {
            mapotn_rgfpf_reg_FRM_LEN_STAT_COUNTER_CONFIG_array_write( NULL, mapotn_handle, itr,chnl,0x00000000);
        }
        mapotn_rgfpf_reg_FRAME_LENGTH_CHECK_CONFIGURATION_array_write(NULL, mapotn_handle, chnl, 0xFFFF0000);
        mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_0_array_write(NULL, mapotn_handle, chnl,0);
        mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_1_array_write(NULL, mapotn_handle, chnl,0);
        mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_2_array_write(NULL, mapotn_handle, chnl,0); 
        /* Initial value for the filters - drop all frames */
        for (itr = 0; itr < MAPOTN_RGFPF_CHNL_NUM_FILTERS; itr += 1)
        {
            mapotn_rgfpf_reg_FILTER_CONFIGURATION_0_array_write(NULL, mapotn_handle, itr, chnl,0);
            mapotn_rgfpf_reg_FILTER_CONFIGURATION_1_array_write(NULL, mapotn_handle, itr, chnl,0x00015e80);
        }
    }
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_block_non_default_cfg */


/*******************************************************************************
* mapotn_rgfpf_chnl_default_init
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Depending on the state resets all the channel based configuration registers 
*   to its its default value, or make channel registers accessible and apply any
*   non-default configuration to the set of registers associated with this 
*   channel.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   state               - RGFPF channel register state\n
*                         0 -- ACCESSIBLE make channel registers accessible and 
*                             apply any non-default configuration to the set of 
*                             registers associated with this channel
*                             (PMC_BLOCK_ALTERABLE)\n
*                         1 -- DEFAULT force channel config registers to default 
*                             (these registers are unmodifiable by write access)
*                             (PMC_BLOCK_DEFAULT)\n
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
PUBLIC PMC_ERROR mapotn_rgfpf_chnl_default_init(mapotn_handle_t *mapotn_handle,
                                                UINT32 chnl,
                                                pmc_block_reg_access_state_enum state)
{
    /* variable declaration */
    UINT32 itr;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    if (state == PMC_BLOCK_ALTERABLE)
    {
        /* Reset the self initialization bit CH_SELF_INIT*/
        /*mapotn_rgfpf_field_CH_SELF_INIT_set(NULL, mapotn_handle, chnl, FALSE);*/
    }
    else if (state == PMC_BLOCK_DEFAULT)
    {            
        /* initialize RAMs */
        mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_0_array_write(NULL, mapotn_handle, chnl,0x1a40301e);
        mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_1_array_write(NULL, mapotn_handle, chnl,0x0400003e);
        mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_2_array_write(NULL, mapotn_handle, chnl,0x10001808);
        mapotn_rgfpf_reg_GFP_CHANNEL_CONFIGURATION_3_array_write(NULL, mapotn_handle, chnl,0x00000000);
        mapotn_rgfpf_reg_FRAME_LENGTH_CHECK_CONFIGURATION_array_write(NULL, mapotn_handle, chnl, 0xFFFF0000);
        for (itr = 0; itr < MAPOTN_FRM_LEN_STAT_COUNTER_CONFIG_N; itr += 1)
        {
            mapotn_rgfpf_reg_FRM_LEN_STAT_COUNTER_CONFIG_array_write( NULL, mapotn_handle, itr,chnl,0x00000000);
        }

        mapotn_rgfpf_reg_FRAME_LENGTH_CHECK_CONFIGURATION_array_write(NULL, mapotn_handle, chnl, 0xFFFF0000);
        mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_0_array_write(NULL, mapotn_handle, chnl,0);
        mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_1_array_write(NULL, mapotn_handle, chnl,0);
        mapotn_rgfpf_reg_GENERIC_FILTER_CONFIG_2_array_write(NULL, mapotn_handle, chnl,0); 
        /* Initial value for the filters - drop all frames */
        for (itr = 0; itr < MAPOTN_RGFPF_CHNL_NUM_FILTERS; itr += 1)
        {
            mapotn_rgfpf_reg_FILTER_CONFIGURATION_0_array_write(NULL, mapotn_handle, itr, chnl,0);
            mapotn_rgfpf_reg_FILTER_CONFIGURATION_1_array_write(NULL, mapotn_handle, itr, chnl,0x00015e80);
        }
    }

    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_rgfpf_chnl_default_init */


/*******************************************************************************
* mapotn_rgfpf_chnl_prov
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
PUBLIC PMC_ERROR mapotn_rgfpf_chnl_prov(mapotn_handle_t *mapotn_handle,
                                        UINT32 chnl)
{
    /* variable declaration */
    util_global_map_adapt_func_t adaptation_function;
    util_global_mapping_mode_t mapping_mode;
    util_global_odukp_type_t odukp_type;
    DOUBLE client_rate;
    UINT32 num_cal_entries;
    PMC_ERROR ret_val = PMC_SUCCESS;
    mapotn_src_dest_t src_dest;  
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* retrieve channel's status from memory */          
    adaptation_function = mapotn_handle->var.rx_chnl_ctxt[chnl].adaptation_func; 
    odukp_type          = mapotn_handle->var.rx_chnl_ctxt[chnl].odukp_type;      
    client_rate         = mapotn_handle->var.rx_chnl_ctxt[chnl].client_rate;
    num_cal_entries     = mapotn_handle->var.rx_chnl_ctxt[chnl].num_cal_entries;
    mapping_mode        = mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode;    
    src_dest            = mapotn_handle->var.rx_chnl_ctxt[chnl].src_dest;    

    PMC_LOG(PMC_LOG_SEV_LOW, MAPOTN_LOG_CODE_LOG, 0, 0, "Channel = %u, adaptation_function = %u\n", chnl, adaptation_function); 
    
    /* Load all configuartion memories to its default values */

    mapotn_rgfpf_chnl_default_init(mapotn_handle, chnl, PMC_BLOCK_DEFAULT);

    /* the default initialization, disable FCS, stripping and other traffic option */
    if (UTIL_GLOBAL_MAP_ODU2EP_FC_1200_GFP == adaptation_function)
    {
        mapotn_rgfpf_field_FC1200_TRAFFIC_set(NULL, mapotn_handle, chnl, 1);
        mapotn_rgfpf_field_GFP_FCS_CHK_MODE_set(NULL, mapotn_handle, chnl, 1); 
        mapotn_rgfpf_field_EGRESS_PFCS_STRIP_set(NULL, mapotn_handle, chnl, 1);
        mapotn_rgfpf_field_EHEC_CHK_set(NULL, mapotn_handle, chnl, 0);  
        mapotn_rgfpf_field_EHEC_ECC_set(NULL, mapotn_handle, chnl, 0);  
        mapotn_rgfpf_field_SHUT_DATA_set(NULL, mapotn_handle, chnl, 1);  
        mapotn_rgfpf_field_I_SFAIL_PROPAGATE_set(NULL, mapotn_handle, chnl, 1);
    } 
    else if (UTIL_GLOBAL_MAP_ODU2P_10_GE_7_3_GFP == adaptation_function ||
               src_dest == MAPOTN_DEST_ENET)
    {
        /* no FCS */
        mapotn_rgfpf_fcs_chk_cfg(mapotn_handle,chnl,0,0);
    } 
    else 
    {
        /* pFCS is enabled */
        mapotn_rgfpf_fcs_chk_cfg(mapotn_handle,chnl,0,1);
    }

    mapotn_rgfpf_field_SHUT_DATA_set(NULL, mapotn_handle, chnl, 1);

    mapotn_rgfpf_field_I_SFAIL_PROPAGATE_set(NULL, mapotn_handle, chnl, 1);

    mapotn_rgfpf_field_CPU_ONLY1BUF_set(NULL, mapotn_handle, chnl, 1);

    /* OHFS Config - Configure Per channel OHFS REMOVE: common config reg */
    /* setting up the functional mode */
    ret_val = ohfs_remove_func_mode_cfg(mapotn_handle->ohfs_remove_handle, chnl, 0);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }

    /* dual ohfs remove mode - use rgmp's ohfs remove for defects */
    if (TRUE == MAPOTN_DUAL_OHFS_REMOVE_MODE)
    {
        ret_val = ohfs_remove_func_mode_cfg(mapotn_handle->rgmp_handle->ohfs_remove_handle, chnl, 0);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
     }
    
    /* configure common configurations and FS stripping */
    ret_val = ohfs_remove_chnl_prov(mapotn_handle->ohfs_remove_handle, chnl, odukp_type, 
                                   (UINT32) adaptation_function, mapping_mode, (util_global_odu_line_payload_t)0);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }

    /* dual ohfs remove mode - use rgmp's ohfs remove for defects */
    if (TRUE == MAPOTN_DUAL_OHFS_REMOVE_MODE)
    {
        /* configure common configurations and FS stripping */
        ret_val = ohfs_remove_chnl_prov(mapotn_handle->rgmp_handle->ohfs_remove_handle, chnl, odukp_type, 
                                       (UINT32) adaptation_function, mapping_mode, (util_global_odu_line_payload_t)0);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
    }
 
    /* Channelize FIFO Config for UNPACK CFC and EGRESS CFC */
    /* Channelized FIFO Controller Inverted Page Table */
    
    ret_val = cfc_fifo_build(mapotn_handle->cfc_unpack_handle, chnl, num_cal_entries);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }
    
    ret_val = cfc_fifo_build(mapotn_handle->cfc_egress_handle, chnl, num_cal_entries);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }
    

    /* set xoff num_page */
    ret_val = mapotn_rgfpf_xoff_fifo_thrld_cfg(mapotn_handle, chnl, num_cal_entries); 
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }

    /* filter Config */
    ret_val = mapotn_rgfpf_default_filter_cfg(mapotn_handle, chnl, adaptation_function);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }          
    
    /* RCP defect are always enabled with the channel */
    ret_val = mapotn_rgfpf_ch_defects_en(mapotn_handle, chnl,TRUE,TRUE,TRUE,TRUE);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }          
        
    /*
     * Default configuration is to assert signal fail when there is a loss of 
     * frame delineation or an extension header mismatch.
     */
    mapotn_rgfpf_field_DLFD_SFAIL_set(NULL, mapotn_handle, chnl, TRUE);  
    mapotn_rgfpf_field_DEXM_SFAIL_set(NULL, mapotn_handle, chnl, TRUE);  

    /* 
     *  CPU FIFO configuration for FW
     *   -Include type header in frames sent to FIFO
     *   -Reset the CPU FIFO
     */
    mapotn_rgfpf_field_CPU_FIFO_FORMAT_set(NULL, mapotn_handle, chnl,2);
    mapotn_rgfpf_field_CPU_RST_set(NULL, mapotn_handle, chnl,1);
    mapotn_rgfpf_field_CPU_RST_set(NULL, mapotn_handle, chnl,0);

    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_rgfpf_chnl_prov */


/*******************************************************************************
* mapotn_rgfpf_chnl_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Deprovisions the specific RGFPF channel.
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
PUBLIC PMC_ERROR mapotn_rgfpf_chnl_deprov(mapotn_handle_t *mapotn_handle,
                                          UINT32 chnl)
{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* configure common configurations and FS stripping */
    ret_val = ohfs_remove_chnl_unprov(mapotn_handle->ohfs_remove_handle, 0, chnl);
    if (ret_val != PMC_SUCCESS)
    {
        PMC_RETURN(ret_val);
    }

    ret_val = mapotn_rgfpf_classify_unprov(mapotn_handle,chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    /* Destroy Channelize FIFO for UNPACK CFC */
    ret_val = cfc_fifo_destroy(mapotn_handle->cfc_unpack_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    
    /* Destroy Channelize FIFO for EGRESS CFC */
    ret_val = cfc_fifo_destroy(mapotn_handle->cfc_egress_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }    
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* mapotn_rgfpf_chnl_deprov */


/*******************************************************************************
* mapotn_rgfpf_chnl_activate
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
PUBLIC PMC_ERROR mapotn_rgfpf_chnl_activate(mapotn_handle_t *mapotn_handle,
                                            UINT32 chnl) 

{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* Bring the CFC FIFOs out of reset */
    ret_val = cfc_fifo_activate(mapotn_handle->cfc_unpack_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    ret_val = cfc_fifo_activate(mapotn_handle->cfc_egress_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    /* dual ohfs remove mode - use rgmp's ohfs remove for defects */
    if (TRUE == MAPOTN_DUAL_OHFS_REMOVE_MODE)
    {
        mapotn_handle->rgmp_handle->var.rgmp_chnl_cfg[chnl].state = RGMP_CHNL_EQUIPPED;
        rgmp_chnl_activate(mapotn_handle->rgmp_handle, chnl);
    }

    /* Take out channel out of reset by de-asserting bit CHN_RST */
    mapotn_rgfpf_chnl_reset_cfg(mapotn_handle, chnl, PMC_BLOCK_ALTERABLE);
     
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_chnl_activate */


/*******************************************************************************
* mapotn_rgfpf_chnl_deactivate
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
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/                                        
PUBLIC PMC_ERROR mapotn_rgfpf_chnl_deactivate(mapotn_handle_t *mapotn_handle,
                                              UINT32 chnl)                                        

{
    /* variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* Put the CFC FIFOs in reset */
    ret_val = cfc_fifo_deactivate(mapotn_handle->cfc_unpack_handle, chnl);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }
    ret_val = cfc_fifo_deactivate(mapotn_handle->cfc_egress_handle, chnl);
    if (ret_val != PMC_SUCCESS ) {
        PMC_RETURN(ret_val);
    }

    /* dual ohfs remove mode - use rgmp's ohfs remove for defects */
    if (TRUE == MAPOTN_DUAL_OHFS_REMOVE_MODE)
    {
        rgmp_chnl_deactivate(mapotn_handle->rgmp_handle, chnl);
        mapotn_handle->rgmp_handle->var.rgmp_chnl_cfg[chnl].state = RGMP_CHNL_START;
    }

    /* put channel in reset by asserting bit CHN_RST */
    mapotn_rgfpf_chnl_reset_cfg(mapotn_handle, chnl, PMC_BLOCK_DEFAULT);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_chnl_deactivate */

/*******************************************************************************
* mapotn_rgfpf_chnl_clean
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Clean a set of RGFPF channels
*                                                                                                                                                     
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   *chnl               - set of channel ID to be cleaned
*   num_chnl            - number of channel ID to be cleaned
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
PUBLIC void mapotn_rgfpf_chnl_clean( mapotn_handle_t *mapotn_handle,
                                     UINT32          *chnl,
                                     UINT32           num_chnl )                
{
    /* variable declaration */

    UINT32 chnl_itr;
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    for(chnl_itr = 0; chnl_itr< num_chnl;chnl_itr++)
    {
        mapotn_rgfpf_chnl_reset_cfg(mapotn_handle, chnl[chnl_itr], PMC_BLOCK_DEFAULT);
    }

    /* Put the CFC FIFOs in reset */
    cfc_fifo_clean(mapotn_handle->cfc_unpack_handle, chnl,num_chnl);
    cfc_fifo_clean(mapotn_handle->cfc_egress_handle, chnl,num_chnl);
    ohfs_remove_chnl_clean(mapotn_handle->ohfs_remove_handle, chnl,num_chnl);    

 

    PMC_RETURN();

} /* mapotn_rgfpf_chnl_clean */

/*******************************************************************************
* mapotn_rgfpf_volatile_ctxt_init
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Initialize RGFPF volatile conetxt from register space
*                                                                                                                                                     
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID 
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
PUBLIC void mapotn_rgfpf_volatile_ctxt_init( mapotn_handle_t *mapotn_handle,
                                             UINT32          chnl)                
{

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    
    PMC_RETURN();

} /* mapotn_rgfpf_volatile_ctxt_init */

/*******************************************************************************
*  mapotn_rgfpf_chnl_reset_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures reset mode for a channel.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   chnl_state          - register access state of RGFPF channel\n
*                         0 -- RGFPF channel is not in reset mode
*                             (PMC_BLOCK_ALTERABLE)\n
*                         1 -- RGFPF channels pipeline, counters and state 
*                             machines are reset to their default values
*                             (PMC_BLOCK_DEFAULT)\n
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
PUBLIC PMC_ERROR mapotn_rgfpf_chnl_reset_cfg(mapotn_handle_t *mapotn_handle,
                                             UINT32 chnl,
                                             pmc_block_reg_access_state_enum chnl_state)
{
    /* variable declaration */

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    mapotn_rgfpf_field_CHN_RST_set(NULL, mapotn_handle, chnl, chnl_state);
    
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_chnl_reset_cfg */

/*******************************************************************************
*  mapotn_rgfpf_filter_cfg                                             
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:                                                                  
*   Configures a specific channel filter.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle       - pointer to MAPOTN handle instance                     
*   chnl                 - channel ID to be configured                           
*                          Valid range: 0 - 95                                   
*   fltr_index           - RGFPF filter number to be configured                  
*   *fltr_ptr            - pointer to the RGFPF filter structure containing the 
*                          values 
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
PRIVATE void mapotn_rgfpf_filter_cfg(mapotn_handle_t *mapotn_handle,
                                    UINT32 chnl, 
                                    UINT32 fltr_index, 
                                    mapotn_rgfpf_fltr_t *fltr_ptr)
{
   
    PMC_ENTRY();
    

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(fltr_index < MAPOTN_RGFPF_CHNL_NUM_FILTERS, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_rgfpf_field_SELECT_GENERIC_set(NULL, mapotn_handle, fltr_index, chnl, fltr_ptr->select_generic);
    
    if(fltr_ptr->select_generic == 1)
    {
        PMC_LOG(PMC_LOG_SEV_LOW, MAPOTN_LOG_CODE_LOG, 0, 0, "fltr_ptr->select_generic= %u\n", fltr_ptr->select_generic);     
        mapotn_rgfpf_field_GENERIC_OFFSET_set(NULL, mapotn_handle, chnl, fltr_ptr->generic_ofs);
        mapotn_rgfpf_field_GENERIC_MASK_set(NULL, mapotn_handle, chnl, fltr_ptr->generic_msk);
        mapotn_rgfpf_field_GENERIC_VALUE_set(NULL, mapotn_handle, chnl, fltr_ptr->generic_val);
        mapotn_rgfpf_field_EXI_MASK_set(NULL, mapotn_handle, fltr_index, chnl, 0);
        mapotn_rgfpf_field_PTI_MASK_set(NULL, mapotn_handle, fltr_index, chnl, 0);
        mapotn_rgfpf_field_UPI_MASK_set(NULL, mapotn_handle, fltr_index, chnl, 0);
        mapotn_rgfpf_field_EXI_VALUE_set(NULL, mapotn_handle, fltr_index, chnl, 0);
        mapotn_rgfpf_field_PTI_VALUE_set(NULL, mapotn_handle, fltr_index, chnl, 0);
        mapotn_rgfpf_field_UPI_VALUE_set(NULL, mapotn_handle, fltr_index, chnl, 0);
    }
    else 
    {
        PMC_LOG(PMC_LOG_SEV_LOW, MAPOTN_LOG_CODE_LOG, 0, 0, "fltr_ptr->select_generic= %u\n", fltr_ptr->select_generic);     
        mapotn_rgfpf_field_EXI_MASK_set(NULL,  mapotn_handle, fltr_index, chnl, fltr_ptr->exi_msk);
        mapotn_rgfpf_field_PTI_MASK_set(NULL,  mapotn_handle, fltr_index, chnl, fltr_ptr->pti_msk);
        mapotn_rgfpf_field_UPI_MASK_set(NULL,  mapotn_handle, fltr_index, chnl, fltr_ptr->upi_msk);
        mapotn_rgfpf_field_EXI_VALUE_set(NULL, mapotn_handle, fltr_index, chnl, fltr_ptr->exi_val);
        mapotn_rgfpf_field_PTI_VALUE_set(NULL, mapotn_handle, fltr_index, chnl, fltr_ptr->pti_val);
        mapotn_rgfpf_field_UPI_VALUE_set(NULL, mapotn_handle, fltr_index, chnl, fltr_ptr->upi_val);   
    }
    
    mapotn_rgfpf_field_MATCH_INDICATOR_set(NULL, mapotn_handle, fltr_index, chnl, fltr_ptr->match_indicator);
    mapotn_rgfpf_field_NEXT_FILTER_OFFSET_set(NULL, mapotn_handle, fltr_index, chnl, fltr_ptr->next_fltr_ofs);
    mapotn_rgfpf_field_CONTINUATION_OPERATOR_set(NULL, mapotn_handle, fltr_index, chnl, fltr_ptr->cont_operator);
    mapotn_rgfpf_field_ACTION_set(NULL, mapotn_handle, fltr_index, chnl, fltr_ptr->action);
    mapotn_rgfpf_field_COMPARE_OPERATOR_set(NULL, mapotn_handle, fltr_index, chnl, fltr_ptr->comp_operator);

    PMC_RETURN();

}  /* mapotn_rgfpf_filter_cfg */

/*******************************************************************************
*  mapotn_rgfpf_filter_cfg_get                                             
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:                                                                  
*   Configures a specific channel filter.
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle       - pointer to MAPOTN handle instance                     
*   chnl                 - channel ID to be configured                           
*                          Valid range: 0 - 95                                   
*   fltr_index           - RGFPF filter number to be configured                  
*   *fltr_ptr            - pointer to the RGFPF filter structure containing the 
*                          values 
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
PRIVATE void mapotn_rgfpf_filter_cfg_get(mapotn_handle_t *mapotn_handle,
                                         UINT32 chnl, 
                                         UINT32 fltr_index, 
                                         mapotn_rgfpf_fltr_t *fltr_ptr)
{
   
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(fltr_index < MAPOTN_RGFPF_CHNL_NUM_FILTERS, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    fltr_ptr->select_generic = mapotn_rgfpf_field_SELECT_GENERIC_get(NULL, mapotn_handle, fltr_index, chnl);
    memset(fltr_ptr, 0, sizeof(*fltr_ptr));
    
    if(fltr_ptr->select_generic == 1)
    {
        fltr_ptr->generic_ofs = mapotn_rgfpf_field_GENERIC_OFFSET_get(NULL, mapotn_handle, chnl);
        fltr_ptr->generic_msk = mapotn_rgfpf_field_GENERIC_MASK_get(NULL, mapotn_handle, chnl);
        fltr_ptr->generic_val = mapotn_rgfpf_field_GENERIC_VALUE_get(NULL, mapotn_handle, chnl);
    }
    else 
    {
        fltr_ptr->exi_msk = mapotn_rgfpf_field_EXI_MASK_get(NULL,  mapotn_handle, fltr_index, chnl);
        fltr_ptr->pti_msk = mapotn_rgfpf_field_PTI_MASK_get(NULL,  mapotn_handle, fltr_index, chnl);
        fltr_ptr->upi_msk = mapotn_rgfpf_field_UPI_MASK_get(NULL,  mapotn_handle, fltr_index, chnl);
        fltr_ptr->exi_val = mapotn_rgfpf_field_EXI_VALUE_get(NULL, mapotn_handle, fltr_index, chnl);
        fltr_ptr->pti_val = mapotn_rgfpf_field_PTI_VALUE_get(NULL, mapotn_handle, fltr_index, chnl);
        fltr_ptr->upi_val = mapotn_rgfpf_field_UPI_VALUE_get(NULL, mapotn_handle, fltr_index, chnl);
    }
    
    fltr_ptr->match_indicator = mapotn_rgfpf_field_MATCH_INDICATOR_get(NULL, mapotn_handle, fltr_index, chnl);
    fltr_ptr->next_fltr_ofs = mapotn_rgfpf_field_NEXT_FILTER_OFFSET_get(NULL, mapotn_handle, fltr_index, chnl);
    fltr_ptr->cont_operator = mapotn_rgfpf_field_CONTINUATION_OPERATOR_get(NULL, mapotn_handle, fltr_index, chnl);
    fltr_ptr->action = mapotn_rgfpf_field_ACTION_get(NULL, mapotn_handle, fltr_index, chnl);
    fltr_ptr->comp_operator = mapotn_rgfpf_field_COMPARE_OPERATOR_get(NULL, mapotn_handle, fltr_index, chnl);

    PMC_RETURN();

}  /* mapotn_rgfpf_filter_cfg */



/*******************************************************************************
* mapotn_rgfpf_chnl_cpb_zone_thrld_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the threshold for CPB zone for a channel.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   val                 - threshold for CPB zone 
*                         Valid range: 0 - 3
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
PUBLIC PMC_ERROR mapotn_rgfpf_chnl_cpb_zone_thrld_cfg(mapotn_handle_t *mapotn_handle,
                                                      UINT32 chnl, 
                                                      UINT32 val)
{
    /* variable  declaration */
    UINT32 start_bit, stop_bit;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    start_bit = chnl * MAPOTN_RGFPF_CPB_ZONE_THRLD_CHNL_NUM_BITS;
    stop_bit = start_bit + MAPOTN_RGFPF_CPB_ZONE_THRLD_CHNL_NUM_BITS - 1;
    mapotn_rgfpf_lfield_range_CPB_ZONE_THRLD_set(NULL, mapotn_handle, start_bit, stop_bit, val );

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_chnl_cpb_zone_thrld_cfg */


/*******************************************************************************
*  mapotn_rgfpf_egress_pfcs_efcs_strip_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the data format that is bound to Egress interface for a channel.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   efcs_strip          - defines whether Ethernet FCS (eFCS) has to be sent to  
*                         egress interface or stripped off\n
*                         0 -- no FCS strip\n 
*                         1 -- eFCs is stripped\n 
*   pfcs_strip          - defines whether paylodad FCS (pFCS) has to be sent to  
*                         egress interface or stripped off\n
*                         0 -- no FCS strip\n 
*                         1 -- pFCs is stripped\n
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_egress_pfcs_efcs_strip_cfg(mapotn_handle_t *mapotn_handle,
                                                         UINT32 chnl,
                                                         UINT32 efcs_strip, 
                                                         UINT32 pfcs_strip)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    mapotn_rgfpf_field_EGRESS_EFCS_STRIP_set(NULL, mapotn_handle, chnl, efcs_strip);
    mapotn_rgfpf_field_EGRESS_PFCS_STRIP_set(NULL, mapotn_handle, chnl, pfcs_strip);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_egress_pfcs_efcs_strip_cfg */


/*******************************************************************************
*  mapotn_rgfpf_extn_hdr_extrct_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures GFP frames extension header check, single bit error correction on
*   the GFP extension header, and extension header extraction for a channel.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   ehec_chk            - defines whether or not the GFP frames extension header
*                         HEC (eHEC) is checked.\n
*                         0 -- the GFP frame eHEC field is not checked\n
*                         1 -- the GFP frame eHEC is checked when frame 
*                             delineation machine is in SYNC state\n
*   ehec_ecc            - defines whether or not single-bit error correction is 
*                         performed on the GFP Extension Header\n
*                         0 -- no single bit error correction is done\n
*                         1 -- perform single-bit error correction\n
*   exi_val1            - EXI value with which the extracted EXI value from 
*                         received GFP frame is compared to determine the length
*                         of Extension header  
*   exi_len1            - length of the extension header if the incoming GFP 
*                         frame's extracted EXI value matchs with EXI_VALUE1
*   exi_val2            - EXI value with which the extracted EXI value from 
*                         received GFP frame is compared to determine the length
*                         of Extension header 
*   exi_len2            - length of the extension header if the incoming GFP 
*                         frame's extracted EXI value matchs with EXI_VALUE2
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
PUBLIC PMC_ERROR mapotn_rgfpf_extn_hdr_extrct_cfg(mapotn_handle_t *mapotn_handle,
                                                  UINT32 chnl,
                                                  UINT32 ehec_chk,
                                                  UINT32 ehec_ecc,
                                                  UINT32 exi_val1, 
                                                  UINT32 exi_len1,                                           
                                                  UINT32 exi_val2, 
                                                  UINT32 exi_len2)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_rgfpf_field_EHEC_CHK_set(NULL, mapotn_handle, chnl, ehec_chk);
    mapotn_rgfpf_field_EHEC_ECC_set(NULL, mapotn_handle, chnl, ehec_ecc);
    
    mapotn_rgfpf_field_EXI_VALUE1_set(NULL, mapotn_handle, chnl, exi_val1);
    mapotn_rgfpf_field_EXI_LENGTH1_set(NULL, mapotn_handle, chnl, exi_len1);
    
    mapotn_rgfpf_field_EXI_VALUE2_set(NULL, mapotn_handle, chnl, exi_val2);
    mapotn_rgfpf_field_EXI_LENGTH2_set(NULL, mapotn_handle, chnl, exi_len2);    

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_extn_hdr_extrct_cfg */



/*******************************************************************************
*  mapotn_rgfpf_pause_frm_detect_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures pause frame detection and whether to drop or forward pause frames
*   to Egress interface for a channel. 
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   pause_filter        - defines whether to drop or forward pause frames to 
*                         egress interface\n
*                         0 -- Frames qualified as pause frames are forwarded to 
*                             egress interface\n
*                         1 -- Frames qualified as pause frames are not forwarded
*                             to egress interface\n
*   pause_enable        - activates the pause check logic\n
*                         0 -- Pause check is disabled\n
*                         1 -- Pause check is enabled\n
*   pause_mode          - set if spd and preamble bytes exist
*   pause_upi           - Pause frame UPI value
*   pause_pti           - Pause frame PTI value
*   auto_flow_ctrl_en   - Enable automatic flow control
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:  Once PAUSE_ENABLE is set to 1 then RGFPF will check the incoming GFP
*         frame to determine whether it is a pause frame. An incoming frame is 
*         classified as PAUSE frame when the following conditions are satisfied:
*         1) PTI is equal to the configured PAUSE_PTI value
*         2) UPI is equal to the configured PAUSE_UPI value
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_pause_frm_detect_cfg(mapotn_handle_t *mapotn_handle,
                                                   UINT32 chnl,
                                                   UINT32 pause_filter,
                                                   UINT32 pause_enable, 
                                                   UINT32 pause_mode,
                                                   UINT32 pause_upi, 
                                                   UINT32 pause_pti,
                                                   UINT32 auto_flow_ctrl_en)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_rgfpf_field_PAUSE_FILTER_set(NULL, mapotn_handle, chnl, pause_filter);
    mapotn_rgfpf_field_PAUSE_ENABLE_set(NULL, mapotn_handle, chnl, pause_enable);
    mapotn_rgfpf_field_PAUSE_MODE_set(NULL, mapotn_handle, chnl, pause_mode);

    mapotn_rgfpf_field_PAUSE_PTI_set(NULL, mapotn_handle, chnl, pause_pti);
    mapotn_rgfpf_field_PAUSE_UPI_set(NULL, mapotn_handle, chnl, pause_upi);
    
    mapotn_rgfpf_lfield_range_AUTO_FLOW_CTRL_EN_set(NULL,mapotn_handle,chnl, chnl, auto_flow_ctrl_en); 

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_pause_frm_detect_cfg */

/*******************************************************************************
*  mapotn_rgfpf_pause_frm_detect_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Get the configuration for pause frame detection 
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
*
* OUTPUTS:
*   *pause_filter        - defines whether to drop or forward pause frames to 
*                         egress interface\n
*                         0 -- Frames qualified as pause frames are forwarded to 
*                             egress interface\n
*                         1 -- Frames qualified as pause frames are not forwarded
*                             to egress interface\n
*   *pause_enable        - activates the pause check logic\n
*                         0 -- Pause check is disabled\n
*                         1 -- Pause check is enabled\n
*   *pause_mode          - set if spd and preamble bytes exist
*   *pause_upi           - Pause frame UPI value
*   *pause_pti           - Pause frame PTI value
*   *auto_flow_ctrl_en   - Enable automatic flow control
*
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_pause_frm_detect_cfg_get( mapotn_handle_t                 *mapotn_handle,
                                                        UINT32                           chnl,
                                                        BOOL8                           *pause_filter,
                                                        BOOL8                           *pause_enable, 
                                                        BOOL8                           *pause_mode,
                                                        BOOL8                           *pause_upi, 
                                                        BOOL8                           *pause_pti,
                                                        BOOL8                           *auto_flow_ctrl_en )
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    *pause_filter = mapotn_rgfpf_field_PAUSE_FILTER_get(NULL, mapotn_handle, chnl);
    *pause_enable = mapotn_rgfpf_field_PAUSE_ENABLE_get(NULL, mapotn_handle, chnl);
    *pause_mode = mapotn_rgfpf_field_PAUSE_MODE_get(NULL, mapotn_handle, chnl);

    *pause_pti = mapotn_rgfpf_field_PAUSE_PTI_get(NULL, mapotn_handle, chnl);
    *pause_upi = mapotn_rgfpf_field_PAUSE_UPI_get(NULL, mapotn_handle, chnl);
    
    *auto_flow_ctrl_en = mapotn_rgfpf_lfield_range_AUTO_FLOW_CTRL_EN_get(NULL,mapotn_handle,chnl, chnl); 

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_pause_frm_detect_cfg_get */



/*******************************************************************************
*  mapotn_rgfpf_xoff_fifo_thrld_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the XOFF threshold of unpacking & egress FIFOs for a channel.  
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                    
*   num_cal_entries     - number of calendar entries
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES: When fill level crosses this value then depending on S_OLD status 
*        received for the given channel Xoff indication is given or small GFP  
*        frames (defined by configured thresholds) are dropped
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_xoff_fifo_thrld_cfg(mapotn_handle_t *mapotn_handle,
                                                  UINT32 chnl,
                                                  UINT32 num_cal_entries)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    if (pmc_is_digi_rev_a_revision(&mapotn_handle->base) == TRUE)
    {
        mapotn_rgfpf_field_UNPACK_XOFF_THRLD_set(NULL, mapotn_handle, chnl, num_cal_entries);
    }
    else
    {
        mapotn_rgfpf_field_UNPACK_XOFF_THRLD2_set(NULL, mapotn_handle, chnl, num_cal_entries * 3);
    }
    mapotn_rgfpf_field_EGRESS_XOFF_THRLD_set(NULL, mapotn_handle, chnl,num_cal_entries);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_xoff_fifo_thrld_cfg */


/*******************************************************************************
*  mapotn_rgfpf_egress_frm_len_chk_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the minimum and maximum lengths for client frame  which should be
*   allowed to be passed downstream for a channel. 
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                     
*   max_frm_len         - maximum allowed byte length of client payload area in  
*                         a GFP data frame
*   min_frm_len         - minimum allowed byte length of client payload area in 
*                         a GFP data frame
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
PUBLIC PMC_ERROR mapotn_rgfpf_egress_frm_len_chk_cfg(mapotn_handle_t *mapotn_handle,
                                                     UINT32 chnl,
                                                     UINT16 max_frm_len, 
                                                     UINT16 min_frm_len)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_rgfpf_field_MAXFL_set(NULL, mapotn_handle, chnl, max_frm_len);
    mapotn_rgfpf_field_MINFL_set(NULL, mapotn_handle, chnl, min_frm_len);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_egress_frm_len_chk_cfg */


/*******************************************************************************
*  mapotn_rgfpf_frm_len_stat_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the minumum and maximum value for a range of lengths of client 
*   payload area of a GFP frame to be counted for a channel. 
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95  
*   bucket_no           - bucket number to be configured
*                         valid range: 0 - 6                   
*   max_frm_len         - maximum byte length of client payload area of a GFP 
*                         frame
*   min_frm_len         - minimum byte length of client payload area of a GFP 
*                         frame 
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
PUBLIC PMC_ERROR mapotn_rgfpf_frm_len_stat_cfg(mapotn_handle_t *mapotn_handle,
                                               UINT32 chnl,
                                               UINT32 bucket_no,
                                               UINT32 max_frm_len, 
                                               UINT32 min_frm_len)
{
    /* variable declaration */

    
    PMC_ENTRY(); 
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    mapotn_rgfpf_field_MINFL_TR_set(NULL, mapotn_handle, bucket_no, chnl,min_frm_len);
    mapotn_rgfpf_field_MAXFL_TR_set(NULL, mapotn_handle, bucket_no, chnl,max_frm_len);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_frm_len_stat_cfg */

/*******************************************************************************
*  mapotn_rgfpf_frm_len_stat_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the configuration of the minumum and maximum value for a range of 
*   lengths of client payload area of a GFP frame to be counted for a channel. 
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95  
*   bucket_no           - bucket number to be configured
*                         valid range: 0 - 6                   
*                        
*
* OUTPUTS:
*   *max_frm_len        - maximum byte length of client payload area of a GFP 
*                         frame
*   *min_frm_len        - minimum byte length of client payload area of a GFP 
*                         frame 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_frm_len_stat_cfg_get(mapotn_handle_t *mapotn_handle,
                                                   UINT32 chnl, 
                                                   UINT32 bucket_no,
                                                   UINT32 *max_frm_len, 
                                                   UINT32 *min_frm_len)
{
    /* variable declaration */

    
    PMC_ENTRY(); 
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    *min_frm_len = mapotn_rgfpf_field_MINFL_TR_get(NULL, mapotn_handle, bucket_no, chnl);
    *max_frm_len = mapotn_rgfpf_field_MAXFL_TR_get(NULL, mapotn_handle, bucket_no, chnl);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_frm_len_stat_cfg_get */


/*******************************************************************************
*  mapotn_rgfpf_preamble_sfd_mode_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the number of preamble and SFD bytes for Ethernet frames of a 
*   channel.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   preamble_size       - length of preamble that needs to be processed in 
*                         payload\n
*                         0 -- Three bytes of preamble and one byte of SFD\n
*                         1 -- Seven bytes of preamble and one byte of SFD\n
*                         2 -- Eleven bytes of preamble and one byte of SFD\n
*                         3 -- Fifteen bytes of preamble and one byte of SFD\n
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
PUBLIC PMC_ERROR mapotn_rgfpf_preamble_sfd_mode_cfg(mapotn_handle_t *mapotn_handle,
                                                    UINT32 chnl,
                                                    UINT32 preamble_size)
{
   /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_rgfpf_field_PREAMBLE_SFD_MODE_set(NULL, mapotn_handle, chnl, preamble_size);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_preamble_sfd_mode_cfg */


/*******************************************************************************
*  mapotn_rgfpf_fcs_chk_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures frame check sequence (FCS) mode on a GFP frame of a channel.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   cnt_udf_fcs         - defines whether the GFP processor counts only 
*                         FCS erred User Data Frames (PTI = 000) or any 
*                         FCS erred frame.\n 
*                         0 -- any GFP frame exhibiting a payload FCS error is 
*                              counted in the ERR_FCS_CNT PM counter\n 
*                         1 -- only GFP User Data Frames (PTI = 000) exhibiting a
*                              payload FCS error is counted in the ERR_FCS_CNT PM
*                              counter\n
*   gfp_fcs_chk_mode    - FCS check mode\n
*                         0 -- No FCS check\n
*                         1 -- pFCS check\n
*                         2 -- eFCS check after skipping programmed number of
*                              preambles\n
*                         3 -- eFCS check from 1st byte onwards (no preamble
*                              skipping)\n
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
PUBLIC PMC_ERROR mapotn_rgfpf_fcs_chk_cfg(mapotn_handle_t *mapotn_handle,
                                          UINT32 chnl,
                                          UINT32 cnt_udf_fcs,
                                          UINT32 gfp_fcs_chk_mode)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_rgfpf_field_CNT_UDF_FCS_set(NULL, mapotn_handle, chnl, cnt_udf_fcs);
    mapotn_rgfpf_field_GFP_FCS_CHK_MODE_set(NULL, mapotn_handle, chnl, gfp_fcs_chk_mode);

    if(1 == gfp_fcs_chk_mode)
    {    
        mapotn_rgfpf_field_EGRESS_PFCS_STRIP_set(NULL, mapotn_handle, chnl, 1);
    } 
    else if( 2 == gfp_fcs_chk_mode || 3 == gfp_fcs_chk_mode)
    {
        mapotn_rgfpf_field_EGRESS_EFCS_STRIP_set(NULL, mapotn_handle, chnl, 1);
    }
    else 
    {
        mapotn_rgfpf_egress_pfcs_efcs_strip_cfg(mapotn_handle, chnl, 0, 0);
    }
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_fcs_chk_cfg */


/*******************************************************************************
*  mapotn_rgfpf_enet_mac_dest_addr_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures Ethernet MAC destination address. This function should be 
*   configured with MAC destination address present in pause frames. 
*   Pause time is extracted only if the Pause frame is having configured MAC 
*   destination address or the unique multi-cast address of 0x0180C2000001.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   mac_da_lsb          - LSB 32 bits of pause frame MAC address
*   mac_da_msb          - MSB 16 bits of pause frame MAC address
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
PUBLIC PMC_ERROR mapotn_rgfpf_enet_mac_dest_addr_cfg(mapotn_handle_t *mapotn_handle,
                                                     UINT32 mac_da_lsb, 
                                                     UINT32 mac_da_msb)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_rgfpf_field_MAC_DA_LSB_set(NULL, mapotn_handle, mac_da_lsb);
    mapotn_rgfpf_field_MAC_DA_MSB_set(NULL, mapotn_handle, mac_da_msb);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_enet_mac_dest_addr_cfg */


/*******************************************************************************
*  mapotn_rgfpf_ordered_set_upi_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the User Payload Indentifier(UPI) value of ordered set frames.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   ord_set_upi         - UPI value of ordered set frames
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
PUBLIC PMC_ERROR mapotn_rgfpf_ordered_set_upi_cfg(mapotn_handle_t *mapotn_handle,
                                                  UINT32 ord_set_upi) 
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_rgfpf_field_ORD_SET_UPI_set(NULL, mapotn_handle, ord_set_upi);
    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_ordered_set_upi_cfg */

/* LCOV_EXCL_START */
/*******************************************************************************
*  mapotn_rgfpf_ordered_set_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieve the configured User Payload Indentifier(UPI) value of ordered set 
*   frames.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32              - Configured UPI value for ordered set frames
*
* NOTES: 
*
*******************************************************************************/
PUBLIC UINT32 mapotn_rgfpf_ordered_set_upi_get(mapotn_handle_t *mapotn_handle)
{
    UINT32 upi;
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    upi = mapotn_rgfpf_field_ORD_SET_UPI_get(NULL, mapotn_handle);

    PMC_RETURN(upi);

} /* mapotn_rgfpf_ordered_set_upi_get */
/* LCOV_EXCL_STOP */


/* LCOV_EXCL_START */
/*******************************************************************************
* mapotn_rgfpf_basic_fltr_cfg
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Creates a simple filter - send all client frames to the egress.
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
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_basic_fltr_cfg(mapotn_handle_t *mapotn_handle, 
                                             UINT32 chnl)    
{
    mapotn_rgfpf_chnl_fltr_mgmt_t * fltr_mgmt;
    PMC_ERROR res = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
        
    /* filter 0 will send frames to egress */
    fltr_mgmt = &(mapotn_handle->var.mapotn_rgfpf_chnl_fltr_mgmt[chnl]);
    fltr_mgmt->next_filter = 0;
 
    res = mapotn_rgfpf_fltr_client_frames(mapotn_handle,chnl);

    /* reset next_filter to 0. That way, when client_classify is called it will replace filter 0.
       If we keep this function, should add a flag in fltr mgmt to keep track of this filter. */

    fltr_mgmt->next_filter = 0;
    PMC_RETURN(res); 
} /* mapotn_rgfpf_basic_fltr_cfg */
/* LCOV_EXCL_STOP */

/*******************************************************************************
* mapotn_rgfpf_default_filter_cfg
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Creates the default filter filter configuration.
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95   
*   adaptation_function - adaptation function
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
PRIVATE PMC_ERROR mapotn_rgfpf_default_filter_cfg(mapotn_handle_t *mapotn_handle, 
                                                  UINT32 chnl,
                                                  util_global_map_adapt_func_t adaptation_function)
{
    mapotn_rgfpf_chnl_fltr_mgmt_t * fltr_mgmt;
    PMC_ERROR res = PMC_SUCCESS;

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
        
    fltr_mgmt = &(mapotn_handle->var.mapotn_rgfpf_chnl_fltr_mgmt[chnl]);
    fltr_mgmt->next_filter = 0;
    fltr_mgmt->client_classify_prov = FALSE;

    switch(adaptation_function)
    {
        case UTIL_GLOBAL_MAP_ODU2EP_FC_1200_GFP:
            res = mapotn_rgfpf_fc1200_filter_cfg(mapotn_handle,chnl);
            break;
        default:
            /* filter 0 will send frames to egress */
            res = mapotn_rgfpf_fltr_client_frames(mapotn_handle,chnl);
            break;  
     }

    /* Reset next_filter to 0. The user can override the filter config by calling 
     * client_classify/user_client_classify.
     */
    fltr_mgmt->next_filter = 0;
    PMC_RETURN(res); 
} /* mapotn_rgfpf_default_filter_cfg */


/*******************************************************************************
* mapotn_rgfpf_fc1200_filter_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   FC1200 filter configuration.
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
PRIVATE PMC_ERROR mapotn_rgfpf_fc1200_filter_cfg(mapotn_handle_t *mapotn_handle,
                                                 UINT32 chnl)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
	mapotn_rgfpf_fltr_t f;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    /* Filter 0  - Generic filter to trigger generic defect on every frame */
    /* This filter will never match if( 0 != 0). When it mismatches, a generic defect is triggered. */
    f.generic_ofs = 0;
    f.generic_msk = 0;
    f.generic_val = 0;
    f.select_generic = 1;
    f.match_indicator = MAPOTN_USE_UPI_SET; /* USE_UPI_SET must be 1 to see GENERIC defect */
    f.next_fltr_ofs = 0xf;   /* Last filter */
    f.cont_operator = 1;     /* Override */
    f.comp_operator = 0x03;  /* Not equal */
    f.action = MAPOTN_SEND_TO_EGRESS;   

    ret_val = mapotn_rgfpf_fltr_add(mapotn_handle, chnl, &f);

    /* Filter 1 - check extension header */
    f.upi_msk = 0;
    f.upi_val = 0;
    f.exi_msk = 0xf;
    f.exi_val = 0;
    f.pti_msk = 0;
    f.pti_val = 0;
    f.select_generic = 0;
    f.match_indicator = MAPOTN_USE_UPI_SET | MAPOTN_USE_PTI_SET; /* clear dUPM, leave dEXM */
    f.next_fltr_ofs = 0xf;   /* Last filter */
    f.cont_operator = 1;     /* Override */
    f.comp_operator = MAPOTN_UPI_EQ | MAPOTN_PTI_EQ | MAPOTN_EXI_NQ;   /* exi != 0, pti & upi can be anything */
    f.action = MAPOTN_DROP_THE_FRAME;  

    ret_val = mapotn_rgfpf_fltr_add(mapotn_handle, chnl, &f);

    /* Filter 2 - check UPI/PTI */
    f.upi_msk = 0xff;
    f.upi_val = 0x15; 
    f.exi_msk = 0;
    f.exi_val = 0;
    f.pti_msk = 0x7;
    f.pti_val = MAPOTN_GFP_CLIENT;
    f.select_generic = 0;
    f.match_indicator = MAPOTN_USE_UPI_SET | MAPOTN_USE_PTI_SET | MAPOTN_USE_EXI_SET; /* clear dUPM and dEXM */
    f.next_fltr_ofs = 0xf;   /* last filter */
    f.cont_operator = 1;     /* Override */
    f.comp_operator = MAPOTN_UPI_EQ | MAPOTN_PTI_EQ | MAPOTN_EXI_EQ;  /* equal */
    f.action = MAPOTN_SEND_TO_EGRESS;   

    ret_val = mapotn_rgfpf_fltr_add(mapotn_handle, chnl, &f);

    /* Drop all remaining frames and trigger a dUPM */
    f.upi_msk = 0;
    f.upi_val = 0;
    f.exi_msk = 0;
    f.exi_val = 0;
    f.pti_msk = 0;
    f.pti_val = 0;
    f.select_generic = 0;
    f.match_indicator = 0x8; /* Trigger dUPM */
    f.next_fltr_ofs = 0xf;   /* last filter */
    f.cont_operator = 1;     /* Override */
    f.comp_operator = MAPOTN_UPI_EQ | MAPOTN_PTI_EQ | MAPOTN_EXI_EQ;  /* equal */
    f.action = MAPOTN_DROP_THE_FRAME;   

    ret_val = mapotn_rgfpf_fltr_add(mapotn_handle, chnl, &f);

    PMC_RETURN(ret_val);
} /* mapotn_rgfpf_fc1200_filter_cfg */


/*******************************************************************************
* mapotn_rgfpf_fltr_client_frames
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*    Adds a filter for client frames (send all frames with PTI=0 to egress).
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
PRIVATE PMC_ERROR mapotn_rgfpf_fltr_client_frames(mapotn_handle_t *mapotn_handle, 
                                                  UINT32 chnl)
{
    mapotn_rgfpf_fltr_t f; 
    mapotn_rgfpf_fltr_t * f_ptr = &f; 
    PMC_ERROR res = PMC_SUCCESS;
     
    PMC_ENTRY(); 
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    PMC_MEMSET(f_ptr,0,sizeof(f));   
   
    f.select_generic = 0;
    f.pti_msk = 7;                         /* 3 bits */
    f.pti_val = MAPOTN_GFP_CLIENT;         /*  data frame (PTI=0) */
    f.exi_msk = 0;
    f.upi_msk = 0;
    f.upi_val = 0;
    f.exi_val = 0;
    f.match_indicator = MAPOTN_USE_EXI_SET | MAPOTN_USE_PTI_SET | MAPOTN_USE_UPI_SET; 
    f.next_fltr_ofs = 0xf;          /* last filter */
    
    f.comp_operator = MAPOTN_PTI_EQ | MAPOTN_UPI_EQ | MAPOTN_EXI_EQ; /* equal operator for pti, equal for upi, */
    f.action = MAPOTN_SEND_TO_EGRESS;
    res = mapotn_rgfpf_fltr_add(mapotn_handle,chnl,&f);
    
    PMC_RETURN(res);
} /* mapotn_rgfpf_fltr_client_frames */


/*******************************************************************************
* mapotn_rgfpf_fltr_mcf_frames
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*    Adds a filter for MCF frames (send all frames with PTI=5 to the CPU).
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
PRIVATE PMC_ERROR mapotn_rgfpf_fltr_mcf_frames(mapotn_handle_t *mapotn_handle, 
                                               UINT32 chnl) 
{
    mapotn_rgfpf_fltr_t f; 
    mapotn_rgfpf_fltr_t * f_ptr = &f; 
    PMC_ERROR res = PMC_SUCCESS;
 
    PMC_ENTRY();  
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    PMC_MEMSET(f_ptr,0,sizeof(f));    
    
    f.select_generic = 0;
    f.generic_ofs = 0; 
    f.generic_msk = 0;
    f.generic_val = 0;
    f.exi_msk = 0;
    f.pti_msk = 7;          /* 3 bits: */
    f.upi_msk = 0;
    f.exi_val = 0;
    f.pti_val = MAPOTN_GFP_MCF;    /* PTI=5 */
    f.upi_val = 0;
    
    f.comp_operator = MAPOTN_PTI_EQ | MAPOTN_UPI_EQ | MAPOTN_EXI_EQ; /* equal operator for pti, equal for upi, */
    f.match_indicator = MAPOTN_USE_EXI_SET | MAPOTN_USE_PTI_SET | MAPOTN_USE_UPI_SET; 
    f.next_fltr_ofs = 0xf; /* last filter */
    f.action = MAPOTN_SEND_TO_CPU;
    res = mapotn_rgfpf_fltr_add(mapotn_handle,chnl,&f);
    
    PMC_RETURN(res);
} /* mapotn_rgfpf_fltr_mcf_frames */

/*******************************************************************************
* mapotn_rgfpf_fltr_cmf_frames
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*    Adds a filter for CMF frames (send all frames with PTI=4 to cpu or 
*    csf fifo).
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95   
*   cmf_dest            - CMF frame destination (cpu/csf fifo)
*                         Refer to mapotn_rgfpf_cmf_dest_t
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
PRIVATE PMC_ERROR mapotn_rgfpf_fltr_cmf_frames(mapotn_handle_t *mapotn_handle, 
                                               UINT32 chnl,
                                               mapotn_rgfpf_cmf_dest_t cmf_dest)
{
    mapotn_rgfpf_fltr_t f; 
    PMC_ERROR res = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(cmf_dest <= MAPOTN_CSF_FIFO, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    f.select_generic = 0;
    f.generic_ofs = 0; 
    f.generic_msk = 0;
    f.generic_val = 0;
    f.exi_msk = 0;
    f.pti_msk = 7;
    f.upi_msk = 0;
    f.exi_val = 0;
    f.pti_val = MAPOTN_GFP_CMF;          /*  CMF frame (PTI=4) */
    f.upi_val = 0;
    f.match_indicator = MAPOTN_USE_EXI_SET | MAPOTN_USE_PTI_SET | MAPOTN_USE_UPI_SET | MAPOTN_USE_ECSF;  
    f.next_fltr_ofs = 0xf;      /* last filter */
    f.action = (cmf_dest == MAPOTN_CPU ? MAPOTN_SEND_TO_CPU : MAPOTN_DROP_THE_FRAME_STORE_UPI_CMF);
    f.comp_operator = MAPOTN_PTI_EQ | MAPOTN_UPI_EQ | MAPOTN_EXI_EQ; /* equal operator for pti, equal for upi, */
    f.cont_operator = 0;        /* merge */
    res = mapotn_rgfpf_fltr_add(mapotn_handle,chnl,&f);
    
    PMC_RETURN(res);
} /* mapotn_rgfpf_fltr_cmf_frames */ 


/*******************************************************************************
* mapotn_rgfpf_fltr_cmf_upi_range
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*    Filters CMF frames with a UPI value within a range (sent to CPU or csf 
*    fifo)
*
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95   
*   cmf_dest            - CMF frame destination (cpu/csf fifo)
*                         Refer to mapotn_rgfpf_cmf_dest_t                                                                            
*   upi_range_start     - upi range start value
*   upi_range_end       - upi range end value
*   next_filter_offset  - next filter offset
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise..                                                                         
*                                                                               
* NOTES:                                                                            
*                                                                               
*******************************************************************************/
PRIVATE PMC_ERROR mapotn_rgfpf_fltr_cmf_upi_range(mapotn_handle_t *mapotn_handle, 
                                                  UINT32 chnl, 
                                                  mapotn_rgfpf_cmf_dest_t cmf_dest,
                                                  UINT32 upi_range_start, 
                                                  UINT32 upi_range_end, 
                                                  UINT32 next_filter_offset)
{
    mapotn_rgfpf_fltr_t f; 
    PMC_ERROR res = PMC_SUCCESS;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(cmf_dest <= MAPOTN_CSF_FIFO, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    
    f.select_generic = 0;
    /* note - generic_ofs/msk/val are per channel not per filter. */
    f.generic_ofs = 0; 
    f.generic_msk = 0;
    f.generic_val = 0;
    f.exi_msk = 0;
    f.exi_val = 0;
    f.pti_msk = 7;                      /* 3 bits */
    f.pti_val = MAPOTN_GFP_CMF;                /* CMF frame (PTI=4) */
    f.upi_msk = 0xff;                   /* 8 bits */
    f.match_indicator = MAPOTN_USE_EXI_SET | MAPOTN_USE_PTI_SET | MAPOTN_USE_UPI_SET | MAPOTN_USE_ECSF; 

    /* if the range is all upi - create a filter for all CMF */
    if (upi_range_start == 0 && upi_range_end == 0xff)
    {
        res = mapotn_rgfpf_fltr_cmf_frames(mapotn_handle,chnl,cmf_dest);
        PMC_RETURN(res);
    }
    
    /* if the upi_range_start == upi_range_end, just create one filter */
    if (upi_range_start == upi_range_end)
    {
        f.upi_val = upi_range_start;
        f.comp_operator = MAPOTN_PTI_EQ | MAPOTN_UPI_EQ; /* equal operator for pti, less than for upi, */
        f.action = (cmf_dest == MAPOTN_CPU ? MAPOTN_SEND_TO_CPU : MAPOTN_DROP_THE_FRAME_STORE_UPI_CMF);
        f.next_fltr_ofs = 0xf;              
        f.cont_operator = 1; 
        res = mapotn_rgfpf_fltr_add(mapotn_handle,chnl,&f);
        PMC_RETURN(res);
    }
    /* if the start of the range = 0, create one filter for the range < upi_range_end + 1 */
    if (upi_range_start == 0)
    {
        /* Add a filter for  UPI < upi_range_end + 1, send to CPU or CSF fifo. final filter */
        f.upi_val = upi_range_end + 1;
        f.comp_operator = MAPOTN_PTI_EQ | MAPOTN_UPI_LT; /* equal operator for pti, less than for upi, */
        f.action = (cmf_dest == MAPOTN_CPU ? MAPOTN_SEND_TO_CPU : MAPOTN_DROP_THE_FRAME_STORE_UPI_CMF);
        f.next_fltr_ofs = 0xf;              
        f.cont_operator = 1; 
        res = mapotn_rgfpf_fltr_add(mapotn_handle,chnl,&f);
        PMC_RETURN(res);
    }
    
    /* if upi_range_end == 0xff, create a filter for > upi_range_start -1 */
    if (upi_range_end == 0xff)
    {
        /* Add a filter for UPI > upi_range_start -1, jump to next_filter_offset */
        f.upi_val = upi_range_start - 1;
        f.next_fltr_ofs = 0xf;
        f.comp_operator = MAPOTN_PTI_EQ | MAPOTN_UPI_GT ; /* equal operator for pti, greater than for upi, */
        f.action = (cmf_dest == MAPOTN_CPU ? MAPOTN_SEND_TO_CPU : MAPOTN_DROP_THE_FRAME_STORE_UPI_CMF);
        f.cont_operator = 1; 
        res = mapotn_rgfpf_fltr_add(mapotn_handle,chnl,&f);
        PMC_RETURN(res);
    }

    /* At this point, upi_range_start > 0 and upi_range_end < 0xff */

    /* Add a filter for UPI < min, jump to next_filter_offset */
    f.upi_val = upi_range_start;
    f.match_indicator = 0;
    f.next_fltr_ofs = next_filter_offset;
    f.comp_operator = MAPOTN_PTI_EQ | MAPOTN_UPI_LT ; /* equal operator for pti, greater than for upi, */
    f.action = MAPOTN_DROP_THE_FRAME; 
    f.cont_operator = 1; 
    res = mapotn_rgfpf_fltr_add(mapotn_handle,chnl,&f);
    if (res != PMC_SUCCESS) 
    {
       PMC_RETURN(res);
    }
    
    /* Add a filter for  UPI < upi_range_end + 1, send to CPU or CSF fifo. final filter */
    f.upi_val = upi_range_end + 1;
    f.match_indicator = MAPOTN_USE_EXI_SET | MAPOTN_USE_PTI_SET | MAPOTN_USE_UPI_SET | MAPOTN_USE_ECSF;
    f.comp_operator = MAPOTN_PTI_EQ | MAPOTN_UPI_LT; /* equal operator for pti, less than for upi, */
    f.action = (cmf_dest == MAPOTN_CPU ? MAPOTN_SEND_TO_CPU : MAPOTN_DROP_THE_FRAME_STORE_UPI_CMF);
    f.next_fltr_ofs = 0xf;              
    res = mapotn_rgfpf_fltr_add(mapotn_handle,chnl,&f);
    
    PMC_RETURN(res);
} /* mapotn_rgfpf_fltr_cmf_upi_range */ 



/*******************************************************************************
* mapotn_rgfpf_fltr_add
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*    Adds a filter. 
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle       - pointer to MAPOTN handle instance
*   chnl                 - channel ID to be configured
*                          Valid range: 0 - 95   
*   *f                   - pointer to the filter structure mapotn_rgfpf_fltr_t
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
PRIVATE PMC_ERROR mapotn_rgfpf_fltr_add(mapotn_handle_t *mapotn_handle, 
                                        UINT32 chnl, 
                                        mapotn_rgfpf_fltr_t *f)
{
    mapotn_rgfpf_chnl_fltr_mgmt_t * fltr_mgmt;
    UINT32 flt_no;
    
    PMC_ENTRY();

    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    /* Get the index of the next filter to be configured */
    fltr_mgmt = &(mapotn_handle->var.mapotn_rgfpf_chnl_fltr_mgmt[chnl]);
    flt_no = fltr_mgmt->next_filter;
    if (flt_no >= MAPOTN_RGFPF_CHNL_NUM_FILTERS)
    {
        PMC_RETURN(MAPOTN_RGFPF_ERR_NO_FILTER_AVAILABLE);
    }

    /* Configure the filter */
    mapotn_rgfpf_filter_cfg(mapotn_handle,chnl,flt_no,f);
    fltr_mgmt->next_filter++;

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_fltr_add */ 



/*******************************************************************************
* mapotn_rgfpf_ch_defects_en
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Enables/disables dLFD, dCSF, dUPM, dEXM channel defects.
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   dlfd_en             - enable signal to indicate dLFD detect event on the 
*                         defect output ports\n
*                         0 -- dLFD defect event is not generated\n
*                         1 -- dLFD defect event is generated\n
*   dcsf_en             - enable signal to indicate dCSF detect event on the 
*                         defect output ports\n
*                         0 -- dCSF defect event is not generated\n
*                         1 -- dCSF defect event is generated\n 
*   dupm_en             - enable signal to indicate dUPM detect event on the 
*                         defect output ports\n
*                         0 -- dUPM defect event is not generated\n
*                         1 -- dUPM defect event is generated\n 
*   dexm_en             - enable signal to indicate dEXM detect event on the 
*                         defect output ports\n
*                         0 -- dEXM defect event is not generated\n
*                         1 -- dEXM defect event is generated\n  
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
PUBLIC PMC_ERROR mapotn_rgfpf_ch_defects_en(mapotn_handle_t *mapotn_handle,  
                                            UINT32 chnl,
                                            UINT32 dlfd_en,
                                            UINT32 dcsf_en,
                                            UINT32 dupm_en,
                                            UINT32 dexm_en) 
                        

{
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    mapotn_rgfpf_field_DLFD_EN_set(0, mapotn_handle,chnl,dlfd_en);
    mapotn_rgfpf_field_DCSF_EN_set(0, mapotn_handle,chnl,dcsf_en);
    mapotn_rgfpf_field_DUPM_EN_set(0, mapotn_handle,chnl,dupm_en);
    mapotn_rgfpf_field_DEXM_EN_set(0, mapotn_handle,chnl,dexm_en);
    PMC_RETURN(PMC_SUCCESS);
} /* mapotn_rgfpf_ch_defects_en */ 

/*******************************************************************************
* mapotn_rgfpf_cpu_fifo_reset
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Resets the CPU data FIFO for a channel.
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95 
*   reset_val           - reset value \n
*                         0 -- CPU data FIFO operates normally \n
*                         1 -- CPU frame data FIFO is emptied \n
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
PUBLIC PMC_ERROR mapotn_rgfpf_cpu_fifo_reset(mapotn_handle_t *mapotn_handle, 
                                             UINT32 chnl,
                                             UINT32 reset_val)
{
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_LOG(PMC_LOG_SEV_LOW, MAPOTN_LOG_CODE_LOG, 0, 0, "*** DISABLED RESET\n");
    mapotn_rgfpf_field_CPU_RST_set(NULL, mapotn_handle, chnl,reset_val);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_cpu_fifo_reset */


/*******************************************************************************
* mapotn_rgfpf_csf_stat
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Retrieves the status of CSF frame/defect indication received.
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*                                                                             
* OUTPUTS:
*   *valid              - pointer to validate the status
*   *chid               - pointer to channel ID on which CSF frame/defect 
*                         indication is received
*   *upi                - pointer to the UPI values of CSF frame/defect 
*                         indication received
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.                                                                     
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/

PUBLIC PMC_ERROR mapotn_rgfpf_csf_stat(mapotn_handle_t *mapotn_handle,
                                       UINT32 *valid,
                                       UINT32 *chid,
                                       UINT32 *upi)
{
    UINT32 x;

    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    x = mapotn_rgfpf_reg_CSF_STATUS_read(NULL, mapotn_handle);
    *valid = (x & RGFPF96_CORE_REG_CSF_STATUS_BIT_CSF_ST_VALID_MSK) >> RGFPF96_CORE_REG_CSF_STATUS_BIT_CSF_ST_VALID_OFF;
    *upi =   (x & RGFPF96_CORE_REG_CSF_STATUS_BIT_CSF_ST_UPI_MSK)   >> RGFPF96_CORE_REG_CSF_STATUS_BIT_CSF_ST_UPI_OFF;
    *chid =  (x & RGFPF96_CORE_REG_CSF_STATUS_BIT_CSF_ST_CHID_MSK)  >> RGFPF96_CORE_REG_CSF_STATUS_BIT_CSF_ST_CHID_OFF;
    PMC_LOG(PMC_LOG_SEV_LOW, MAPOTN_LOG_CODE_LOG, 0, 0, "csf fifo: valid=%x,upi=%x,chid=%x\n",*valid,*upi,*chid);
    PMC_RETURN(PMC_SUCCESS);
} /* mapotn_rgfpf_csf_stat */


/*******************************************************************************
*  mapotn_rgfpf_extn_hdr_extrct_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures GFP frames extension header check, single bit error correction on
*   the GFP extension header, and extension header extraction for a channel.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *ehec_chk           - pointer to storage for EHEC CHK field 
*   *ehec_ecc           - pointer to storage for EHEC ECC field 
*   *exi_val1           - pointer to storage for EXI VALUE1 field 
*   *exi_len1           - pointer to storage for EXI LENGHT1 field 
*   *exi_val2           - pointer to storage for EXI VALUE2 field 
*   *exi_len2           - pointer to storage for EXI LENGHT2 field 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_extn_hdr_extrct_get(mapotn_handle_t *mapotn_handle,
                                                  UINT32 chnl,
                                                  UINT32 *ehec_chk, 
                                                  UINT32 *ehec_ecc,
                                                  UINT32 *exi_val1, 
                                                  UINT32 *exi_len1,                                           
                                                  UINT32 *exi_val2, 
                                                  UINT32 *exi_len2)
{
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
   
    *ehec_chk = mapotn_rgfpf_field_EHEC_CHK_get(NULL, mapotn_handle, chnl);
    *ehec_ecc = mapotn_rgfpf_field_EHEC_ECC_get(NULL, mapotn_handle, chnl);
    
    *exi_val1 = mapotn_rgfpf_field_EXI_VALUE1_get(NULL, mapotn_handle, chnl);
    *exi_len1 = mapotn_rgfpf_field_EXI_LENGTH1_get(NULL, mapotn_handle, chnl);
    
    *exi_val2 = mapotn_rgfpf_field_EXI_VALUE2_get(NULL, mapotn_handle, chnl);
    *exi_len2 = mapotn_rgfpf_field_EXI_LENGTH2_get(NULL, mapotn_handle, chnl);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_extn_hdr_extrct_get */


/*******************************************************************************
* mapotn_rgfpf_fcs_chk_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves receive payload FCS check mode field of a GFP frame for a channel. 
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
*
* OUTPUTS:
*   *gfp_fcs_chk_mode   - pointer to storage for FCS check mode
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_fcs_chk_mode_get(mapotn_handle_t *mapotn_handle,
                                               UINT32 chnl, 
                                               UINT32 *gfp_fcs_chk_mode)
{
    
    /* variable declaration */
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    *gfp_fcs_chk_mode = mapotn_rgfpf_field_GFP_FCS_CHK_MODE_get(NULL, mapotn_handle, chnl);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_fcs_chk_mode_get */

/*******************************************************************************
* mapotn_rgfpf_pmon_latch_trig_set
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Transfer the current PMON count to a holding register and resets the PMON count.
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
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
PUBLIC PMC_ERROR mapotn_rgfpf_pmon_latch_trig_set(mapotn_handle_t *mapotn_handle)
{
    UINT32 pmon_latch_trig = 1;
    UINT32 count = 100, count_itr = 0;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    pmon_latch_trig =     mapotn_rgfpf_field_PMON_LATCH_TRIG_get(NULL, mapotn_handle);
    while(count_itr < count && pmon_latch_trig == 1)
    {
        PMC_Q_USLEEP(mapotn_handle,100);
        pmon_latch_trig =     mapotn_rgfpf_field_PMON_LATCH_TRIG_get(NULL, mapotn_handle);
        count_itr += 1;
    }
    if (pmon_latch_trig == 1)
    {
        PMC_RETURN(MAPOTN_BLOCK_NOT_OPERATIONAL);
    }
    mapotn_rgfpf_field_PMON_LATCH_TRIG_set(NULL, mapotn_handle, 1);
    pmon_latch_trig =     mapotn_rgfpf_field_PMON_LATCH_TRIG_get(NULL, mapotn_handle);
    while(count_itr < count && pmon_latch_trig == 1)
    {
        PMC_Q_USLEEP(mapotn_handle,100);
        pmon_latch_trig =     mapotn_rgfpf_field_PMON_LATCH_TRIG_get(NULL, mapotn_handle);
        count_itr += 1;
    }
    if (pmon_latch_trig == 1)
    {
        PMC_RETURN(MAPOTN_BLOCK_NOT_OPERATIONAL);
    }    
    
    PMC_RETURN(PMC_SUCCESS);
}  /* mapotn_rgfpf_pmon_latch_trig_set */

/*******************************************************************************
* mapotn_rgfpf_activated_chnl_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   This function returns all started channels
*                                                                               
* INPUTS:                                                                       
*   *mapotn_handle      - pointer to MAPOTN handle instance
*                                                                             
* OUTPUTS:
*   *actv_chnl         - all activated channels
*
* RETURNS:
*   None.                                                                        
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC void mapotn_rgfpf_activated_chnl_get(mapotn_handle_t *mapotn_handle,
                                            BOOL8 *actv_chnl)
{
    UINT32 itr;
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    
    for (itr = 0; itr < MAPOTN_NUM_CHANL; itr++)
    {
        actv_chnl[itr] = mapotn_rgfpf_field_CHN_RST_get(NULL, mapotn_handle, itr)==1?FALSE:TRUE;
    }

    PMC_RETURN();
}  /* mapotn_rgfpf_activated_chnl_get */


/*
** End of file
*/
