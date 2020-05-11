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
*   DESCRIPTION : This file contains C functions for operating MAPOTN features.
*
*   NOTES:
*
*******************************************************************************/
/*
** Include Files
*/

#include "digi_loc.h"


/*
** Local Enumerated Types
*/

/*
** Local Constants
*/
/* Defines default transmission time of 3 frames in ODU0 rate*/
#define DIGI_MAP_ODU0_OPU_CSF_PERISTANCE_DEFAULT_VALUE 76

/* Compute OPU_CSF persistance time */
#define DIGI_MAP_OPU_CSF_PERISTANCE_TIME_COMPUTE(odu_rate)              \
    PMC_CEIL((DIGI_MAP_ODU0_OPU_CSF_PERISTANCE_DEFAULT_VALUE * (DOUBLE)UTIL_GLOBAL_ODU0_RATE) / odu_rate)


/*
** Local Macro Definitions
*/
#define DIGI_MAPPER_PAUSE_PRIORITY 0x1
#define DIGI_MAPPER_THRESH_DEFAULT_VAL 0x3

#define DIGI_MAPPER_PMON_TABLE_DEFINE()         \
    DIGI_MAPPER_PMON(counts);                   \

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/*
** Log Messaging
*/

/*
** Forward References
*/
#ifndef DOXYGEN_PUBLIC_ONLY
PRIVATE PMC_ERROR digi_mapper_pmon_chnl_retrieve(digi_handle_t              *digi_handle,
                                                 util_global_switch_data_t  *chnl_handle,   
                                                 digi_mapper_pmon_chnl_t    *pmon_table_ptr);
PRIVATE PMC_ERROR digi_mapper_pmon_fo2_chnl_retrieve(digi_handle_t                  *digi_handle,
                                                     util_global_switch_data_t      *chnl_handle,   
                                                     digi_mapper_pmon_fo2_chnl_t    *pmon_table_ptr);
PRIVATE PMC_ERROR digi_mapper_int_summary_get_internal(digi_handle_t               *digi_handle,
                                                       UINT32                       unused,
                                                       digi_mapper_int_summary_t   *int_summary_ptr);
PRIVATE PMC_ERROR digi_mapper_int_chnl_enabled_check_internal(digi_handle_t       *digi_handle,
                                                              digi_mapper_chnl_t  *chnl_handle,
                                                              BOOL                *int_found_ptr);
PRIVATE PMC_ERROR digi_mapper_gfp_rx_fcs_chk_mode_internal_set(digi_handle_t *digi_handle,
                                                               digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                               UINT32 cnt_udf_fcs,
                                                               UINT32 fcs_chk_mode, 
                                                               UINT32 preamble_size);

PRIVATE BOOL8 digi_mapper_is_far_end_enet_sys(digi_handle_t *digi_handle,
                                              digi_mapper_chnl_def_t* mapper_chnl_handle_ptr);

PRIVATE PMC_ERROR digi_mapper_ilkn_gfp_tx_upi_sel_chnl_validate(digi_handle_t      *digi_handle,
                                                                digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                                BOOL8               check_activation);

PRIVATE PMC_ERROR digi_mapper_chnl_dcpb_dest_validate(digi_handle_t          *digi_handle,
                                                      digi_mapper_chnl_t     *chnl_ctxt_ptr,
                                                      util_global_cpb_port_t  dest);
        
PRIVATE PMC_ERROR digi_mapper_int_chnl_fw_opu_csf_retrieve(digi_handle_t             *digi_handle,
                                                           UINT32                     start_idx,
                                                           UINT32                     start_chnl,
                                                           UINT32                     end_chnl,  
                                                           digi_mapper_int_chnl_t    *filt_table_ptr,
                                                           digi_mapper_int_chnl_t    *int_table_ptr);

PRIVATE PMC_ERROR digi_mapper_enet_pmon_handle_get(digi_handle_t          *digi_handle,
                                                   digi_mapper_chnl_t      *chnl_handle,
                                                   enet_handle_t          **enet_ptr,
                                                   UINT32                  *link);

PRIVATE PMC_ERROR digi_mapper_enet_int_chnl_filt_retrieve_internal(digi_handle_t           *digi_handle,
                                                                   digi_mapper_chnl_t      *chnl_handle,
                                                                   BOOL8                    ret_enet_pmon,
                                                                   enet_int_chnl_t         *filt_table_ptr,
                                                                   enet_int_chnl_t         *int_table_ptr);

PRIVATE PMC_ERROR digi_mapper_enet_etrans_int_chnl_enabled_check_internal(digi_handle_t              *digi_handle,
                                                                          digi_mapper_chnl_t         *chnl_handle,
                                                                          digi_mapper_int_chnl_t     *int_en_table_ptr,
                                                                          BOOL8                      *int_found_ptr);

PRIVATE PMC_ERROR digi_mapper_enet_pmon_int_chnl_enabled_check_internal(digi_handle_t              *digi_handle,
                                                                        digi_mapper_chnl_def_t         *chnl_handle,
                                                                        digi_mapper_int_chnl_t     *int_en_table_ptr,
                                                                        BOOL8                      *int_found_ptr);

PRIVATE PMC_ERROR digi_mapper_enet_int_chnl_cfg(digi_handle_t             *digi_handle,
                                                digi_mapper_chnl_t        *chnl_handle,   
                                                enet_int_chnl_t           *int_table_ptr,
                                                BOOL8                      is_enet_pmon,
                                                BOOL8                      clear,
                                                BOOL8                      enable);
#endif /* DOXYGEN_PUBLIC_ONLY */
                                  
/*
** Public Functions
*/

/*
**  Exported Interface Functions - Start
*/


/*******************************************************************************
* digi_mapper_gfp_tx_pti_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets transmit the Payload Type Indicator (PTI) of the GFP Payload Header 
*   field for a mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*   pti_value              - Payload Type Indicator (PTI) of the GFP Payload 
*                            Header field                   
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
PUBLIC PMC_ERROR digi_mapper_gfp_tx_pti_set(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            UINT32 pti_value)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
        
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /*
     * If the mapping mode is FC 1200 GFP then then the fc1200 GFP 
     * PTI should be set via the CBRC handle. Return an error to let the 
     * caller know this is not valid for a mapper handle.
     */    
    if (DIGI_MAP_ODU2EP_FC_1200_GFP == mapper_chnl_handle_ptr->mode)
    {
        result = DIGI_ERR_MAPPING_NOT_SUPPORTED;
    }
    else
    {        
        mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
        
        result = mapotn_tgfpf_chnl_pti_set(digi_handle->mapotn_handle, mapotn_chnl, 
                                           pti_value);
    }
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_tx_pti_set */  


/*******************************************************************************
* digi_mapper_gfp_tx_pti_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves transmit Payload Type Indicator field of the GFP frame for a 
*   mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on                 
*
* OUTPUTS:
*   *pti_value             - pointer to storage for Payload Type Indicator value
*                            of mapper channel.  
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.     
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_tx_pti_get(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            UINT32 *pti_value)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    UINT32 pti_val = 0;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));

    /*
     * If the mapping mode is FC 1200 GFP then then the fc1200 GFP 
     * PTI should be returned via the CBRC handle. Return an error to let 
     * the caller know this is not valid for a mapper handle.
     */    
    if (DIGI_MAP_ODU2EP_FC_1200_GFP == mapper_chnl_handle_ptr->mode)
    {
        result = DIGI_ERR_MAPPING_NOT_SUPPORTED;
    }
    else
    {
        mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
        
        result = mapotn_tgfpf_chnl_pti_get(digi_handle->mapotn_handle, mapotn_chnl, 
                                           &pti_val);
        
        *pti_value =  pti_val;
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_tx_pti_get */

/*******************************************************************************
* digi_mapper_gfp_csf_insert_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the csf_insert field of the GFP frame for a 
*   mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on                 
*
* OUTPUTS:
*   *csf_insert            - pointer to csf_insert of mapper channel.  
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.     
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_csf_insert_get(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            UINT32 *csf_insert)
{
  digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
  UINT32 mapotn_chnl;
  PMC_ERROR result = PMC_SUCCESS;
    
  PMC_ATOMIC_ENTRY(digi_handle);

  /* Check arguments */
  PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
  PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

  /* check that mapper handle belongs to digi context */ 
  DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

  mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
  DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
  /* check handle provisioning state */
  DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));

  mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
        
  result = mapotn_tgfpf_ohfs_insert_csf_cfg_get(digi_handle->mapotn_handle, mapotn_chnl, 
                                         csf_insert);
        

  PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_csf_insert_get */
                                              

                                              

/*******************************************************************************
* digi_mapper_gfp_tx_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets transmit the User Payload Indicator (UPI) of the GFP Payload Header 
*   field for a mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*   upi_value              - User Payload Indicator (UPI) of the GFP Payload 
*                            Header field                   
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
PUBLIC PMC_ERROR digi_mapper_gfp_tx_upi_set(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            UINT32 upi_value)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));

    /*
     * If the mapping mode is FC 1200 GFP then then the fc1200 GFP 
     * UPI value should be set via the CBRC handle and not the mapper
     * handle. Return an error to let the caller know this is not valid.
     */    
    if (DIGI_MAP_ODU2EP_FC_1200_GFP == mapper_chnl_handle_ptr->mode)
    {
        result = DIGI_ERR_MAPPING_NOT_SUPPORTED;
    }
    else
    {
        mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
        result = mapotn_tgfpf_chnl_upi_set(digi_handle->mapotn_handle, mapotn_chnl, 
                                           upi_value);
    }
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_tx_upi_set */  


/*******************************************************************************
* digi_mapper_gfp_tx_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves transmit User Payload Indicator field of the GFP frame for a 
*   mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on                  
*
* OUTPUTS:
*   *upi_value             - pointer to storage for User Payload Indicator value
*                            of mapper channel. 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.     
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_tx_upi_get(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            UINT32 *upi_value)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    UINT32 upi_val = 0;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));

    /*
     * If the mapping mode is FC 1200 GFP then then the fc1200 GFP 
     * UPI value should be returned via the CBRC handle and not the mapper
     * handle. Return an error to let the caller know this is not valid.
     */    
    if (DIGI_MAP_ODU2EP_FC_1200_GFP == mapper_chnl_handle_ptr->mode)
    {
        result = DIGI_ERR_MAPPING_NOT_SUPPORTED;
    }
    else
    {
        mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
        
        result = mapotn_tgfpf_chnl_upi_get(digi_handle->mapotn_handle, mapotn_chnl, 
                                           &upi_val);
        
        *upi_value =  upi_val;
    }
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_tx_upi_get */


/*******************************************************************************
* digi_mapper_ilkn_gfp_tx_upi_sel_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets the source of the User Payload Indicator (UPI) of the 
*   GFP Payload Header for a mapper channel.
*
*   This function shall be called:
*   - only when the traffic source interface is ILKN,
*   - before channel activation.
*   Otherwise a descriptive error code will be returned.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*    ilkn_upi_sel          - When TRUE, mapper UPI is selected from ILKN RX UPI.\n
*                            When FALSE, UPI is selected from TYPE_HDR_UPI 
*                            register field. 
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
PUBLIC PMC_ERROR digi_mapper_ilkn_gfp_tx_upi_sel_set(digi_handle_t      *digi_handle,
                                                     digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                     BOOL8               ilkn_upi_sel)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* validate that mapper is conencted to ILKN and ILKN RX header TYPE is enabled */
    /* also validate that channel is not already activated */
    result = digi_mapper_ilkn_gfp_tx_upi_sel_chnl_validate(digi_handle, chnl_ctxt_ptr, TRUE);

    /* configure UPI_SEL if validate succeeded */
    if (PMC_SUCCESS == result)
    {
        mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);

        DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
        mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
                
        result = mapotn_tgfpf_chnl_upi_sel_set(digi_handle->mapotn_handle, mapotn_chnl, 
                                               ilkn_upi_sel);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_ilkn_gfp_tx_upi_sel_set */  


/*******************************************************************************
* digi_mapper_ilkn_gfp_tx_upi_sel_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves selector of transmit User Payload Indicator field of the GFP frame for a 
*   mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on.
*
* OUTPUTS:
*   *ilkn_upi_sel          - When TRUE, mapper uses UPI from ILKN RX UPI.\n
*                            When FALSE, mapper uses UPI the TYPE_HDR_UPI 
*                            register field. 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.     
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_ilkn_gfp_tx_upi_sel_get(digi_handle_t      *digi_handle,
                                                     digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                     BOOL8              *ilkn_upi_sel)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);
    
    /* validate that mapper is conencted to ILKN and ILKN RX header TYPE is enabled */
    result = digi_mapper_ilkn_gfp_tx_upi_sel_chnl_validate(digi_handle, chnl_ctxt_ptr, FALSE);

    /* retrieve UPI_SEL value if validatation succeeded */
    if (PMC_SUCCESS == result)
    {
        mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);

        DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

        /* check handle provisioning state */
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
  
        mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
        
        result = mapotn_tgfpf_chnl_upi_sel_get(digi_handle->mapotn_handle, mapotn_chnl, 
                                               ilkn_upi_sel);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_ilkn_gfp_tx_upi_sel_get */

/*******************************************************************************
* digi_mapper_gfp_tx_exi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets transmit the Extension Header Indicator (EXI) of the GFP Payload Header 
*   field for a mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*   exi_value              - Extension Header Indicator (EXI) of the GFP Payload 
*                            Header field                   
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
PUBLIC PMC_ERROR digi_mapper_gfp_tx_exi_set(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            UINT32 exi_value)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));

    /*
     * If the mapping mode is FC 1200 GFP then then the fc1200 GFP 
     * UPI value should be set via the CBRC handle and not the mapper
     * handle. Return an error to let the caller know this is not valid.
     */    
    if (DIGI_MAP_ODU2EP_FC_1200_GFP == mapper_chnl_handle_ptr->mode)
    {
        result = DIGI_ERR_MAPPING_NOT_SUPPORTED;
    }
    else
    {
        mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
        result = mapotn_tgfpf_chnl_exi_set(digi_handle->mapotn_handle, mapotn_chnl, 
                                           exi_value);
    }
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_tx_exi_set */  


/*******************************************************************************
* digi_mapper_gfp_tx_exi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves transmit Extension Header Indicator field of the GFP frame for a 
*   mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on                   
*
* OUTPUTS:
*   *exi_value             - pointer to storage for Extension Header Indicator
*                            valued of mapper channel.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_tx_exi_get(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            UINT32 *exi_value)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    UINT32 exi_val = 0;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));

    /*
     * If the mapping mode is FC 1200 GFP then then the fc1200 GFP 
     * UPI value should be returned via the CBRC handle and not the mapper
     * handle. Return an error to let the caller know this is not valid.
     */    
    if (DIGI_MAP_ODU2EP_FC_1200_GFP == mapper_chnl_handle_ptr->mode)
    {
        result = DIGI_ERR_MAPPING_NOT_SUPPORTED;
    }
    else
    {
        mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
        result = mapotn_tgfpf_chnl_exi_get(digi_handle->mapotn_handle, mapotn_chnl, 
                                           &exi_val);
        
        *exi_value =  exi_val;
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_tx_exi_get */


/*******************************************************************************
* digi_mapper_gfp_tx_ext_header_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables and sets transmit Extension header of the GFP frame for a mapper 
*   channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*   ext_hdr_enbl           - defines whether or not the extension header is to 
*                            be inserted into the GFP Client Data frame\n
*                            0 -- the extension header is not inserted\n  
*                            1 -- the extension header is inserted\n                   
*   ext_hdr_len            - define the byte length of the Extension Header 
*                            (including the eHEC field) that is to be inserted
*                             in the Client Data frame
*   ext_hdr_mem[]          - extension header memory value (16 * 32 bits)
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:   
*   For the following modes from ENET_LINE to MAPOTN are not allowed to generate
*   an extension header.  Extension header must be 0 length for ENET_LINE to 
*   MAPOTN datapaths.
*   DIGI_MAP_ODU4P_100_GE_GFP 
*   DIGI_MAP_ODU3P_40_GE_GFP
*   DIGI_MAP_ODU2P_10_GE_6_2_GFP 
*   DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP 
*   DIGI_MAP_ODU3P_PKT_40_GE_GMP
*   DIGI_MAP_ODU4P_PKT_100_GE_GMP 
*   DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP
*   DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_tx_ext_header_set(digi_handle_t *digi_handle,
                                                   digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                   UINT32 ext_hdr_enbl,
                                                   UINT32 ext_hdr_len, 
                                                   UINT32 ext_hdr_mem[MAPOTN_TGFPF_EXTENSION_HEADER_REG_CNT])
{

    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    digi_mapping_mode_t mode;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
        
    mode = mapper_chnl_handle_ptr->mode;
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    if( DIGI_MAP_ODU4P_100_GE_GFP == mode ||
        DIGI_MAP_ODU3P_40_GE_GFP == mode ||
        DIGI_MAP_ODU2P_10_GE_6_2_GFP == mode ||
        DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP == mode ||
        DIGI_MAP_ODU3P_PKT_40_GE_GMP == mode ||
        DIGI_MAP_ODU4P_PKT_100_GE_GMP == mode || 
        DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP == mode ||
        DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP == mode )
    {
        result = DIGI_ERR_MAPPER_EXT_HDR_GEN_NOT_ALLOWED;
    }
    else 
    {
        result = mapotn_tgfpf_chnl_ext_hdr_cfg(digi_handle->mapotn_handle, 
                                               mapotn_chnl, ext_hdr_enbl,
                                               ext_hdr_len, ext_hdr_mem);
    }
    if (mapper_chnl_handle_ptr->switch_data.cpb_data.op_state.active_state == UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE) 
    { 
        PMC_LOG(PMC_LOG_SEV_HIGH, DIGI_ERR_CPB_MUST_NOT_BE_ACTIVE,0,0, "ERROR: CPB chnl state = ACTIVE\n");
        result = DIGI_ERR_CPB_MUST_NOT_BE_ACTIVE;
    }
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_tx_ext_header_set */                                
                              

/*******************************************************************************
* digi_mapper_gfp_tx_ext_header_get
* ______________________________________________________________________________
*
* DESCRIPTION: 
*   Retrieves transmit Extension header enable and length field of the GFP frame 
*   for a mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on 
*
* OUTPUTS:
*   *ext_hdr_enbl          - pointer to storage for EXT HDR EN field of mapper
*                            channel.                 
*   *ext_hdr_len           - pointer to storage for EXT HDR LEN field of mapper
*                            channel.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_tx_ext_header_get(digi_handle_t *digi_handle,
                                                   digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                   UINT32 *ext_hdr_enbl,
                                                   UINT32 *ext_hdr_len)
{
    
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    UINT32 ext_hdr_en = 0;
    UINT32 ext_hdr_l = 0;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));

    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_tgfpf_chnl_ext_hdr_get(digi_handle->mapotn_handle,
                                           mapotn_chnl,
                                           &ext_hdr_en,
                                           &ext_hdr_l);
                                          
    *ext_hdr_enbl = ext_hdr_en;
    *ext_hdr_len = ext_hdr_l; 
    
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_tx_ext_header_get */


/*******************************************************************************
* digi_mapper_gfp_rx_ext_header_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets receive GFP frames extension header check and extension header 
*   extraction length and value for a mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*   ehec_chk               - defines whether or not the GFP frames extension 
*                            header HEC (eHEC) is checked.\n
*                            0 -- the GFP frame eHEC field is not checked\n
*                            1 -- the GFP frame eHEC is checked when frame 
*                                 delineation machine is in SYNC state\n
*   ehec_ecc               - defines whether or not single-bit error correction 
*                             is performed on the GFP Extension Header\n
*                            0 -- no single bit error correction is done\n
*                            1 -- perform single-bit error correction\n
*   exi_val1               - EXI value with which the extracted EXI value from 
*                            received GFP frame is compared to determine the 
*                            length of Extension header  
*   exi_len1               - length of the extension header if the incoming GFP 
*                            frame's extracted EXI value matchs with EXI_VALUE1
*   exi_val2               - EXI value with which the extracted EXI value from 
*                            received GFP frame is compared to determine the 
*                            length of Extension header 
*   exi_len2               - length of the extension header if the incoming GFP 
*                            frame's extracted EXI value matchs with EXI_VALUE2
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*   For the following modes from ENET_LINE to MAPOTN are not allowed to generate
*   an extension header.  Extension header must be 0 length for ENET_LINE to 
*   MAPOTN datapaths.
*   DIGI_MAP_ODU4P_100_GE_GFP 
*   DIGI_MAP_ODU3P_40_GE_GFP
*   DIGI_MAP_ODU2P_10_GE_6_2_GFP 
*   DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP 
*   DIGI_MAP_ODU3P_PKT_40_GE_GMP
*   DIGI_MAP_ODU4P_PKT_100_GE_GMP 
*   DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP
*   DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_rx_ext_header_set(digi_handle_t *digi_handle,
                                                   digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                   UINT32 ehec_chk,
                                                   UINT32 ehec_ecc,
                                                   UINT32 exi_val1, 
                                                   UINT32 exi_len1,    
                                                   UINT32 exi_val2, 
                                                   UINT32 exi_len2)
{

    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    digi_mapping_mode_t mode;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mode = mapper_chnl_handle_ptr->mode;
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    if( DIGI_MAP_ODU4P_100_GE_GFP == mode ||
        DIGI_MAP_ODU3P_40_GE_GFP == mode ||
        DIGI_MAP_ODU2P_10_GE_6_2_GFP == mode ||
        DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP == mode ||
        DIGI_MAP_ODU3P_PKT_40_GE_GMP == mode ||
        DIGI_MAP_ODU4P_PKT_100_GE_GMP == mode || 
        DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP == mode ||
        DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP == mode )
    {
        result = DIGI_ERR_MAPPER_EXT_HDR_GEN_NOT_ALLOWED;
    }
    else 
    {
        result = mapotn_rgfpf_extn_hdr_extrct_cfg(digi_handle->mapotn_handle, 
                                                  mapotn_chnl, ehec_chk, ehec_ecc,
                                                  exi_val1, exi_len1, 
                                                  exi_val2, exi_len2);
    }                                          
    
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_rx_ext_header_set */                                
                              

/*******************************************************************************
* digi_mapper_gfp_rx_ext_header_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves receive GFP frames extension header check and extension header 
*   extraction length and value for a mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*
* OUTPUTS:
*   *ehec_chk              - pointer to storage for EHEC CHK field of mapper 
*                            channel
*   *ehec_ecc              - pointer to storage for EHEC ECC field of mapper 
*                            channel
*   *exi_val1              - pointer to storage for EXI VALUE1 field of mapper 
*                            channel  
*   *exi_len1              - pointer to storage for EXI LENGTH1 field of mapper 
*                            channel
*   *exi_val2              - pointer to storage for EXI VALUE2 field of mapper 
*                            channel 
*   *exi_len2              - pointer to storage for EXI LENGTH2 field of mapper 
*                            channel
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_rx_ext_header_get(digi_handle_t *digi_handle,
                                                   digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                   UINT32 *ehec_chk,
                                                   UINT32 *ehec_ecc,
                                                   UINT32 *exi_val1, 
                                                   UINT32 *exi_len1, 
                                                   UINT32 *exi_val2, 
                                                   UINT32 *exi_len2)
{
    
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    UINT32 ehec_ck = 0;
    UINT32 ehec_err_c = 0;
    UINT32 exi_v1 = 0;
    UINT32 exi_l1 = 0;
    UINT32 exi_v2 = 0;
    UINT32 exi_l2 = 0;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
 
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
   
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_rgfpf_extn_hdr_extrct_get(digi_handle->mapotn_handle,
                                              mapotn_chnl, &ehec_ck, 
                                              &ehec_err_c, &exi_v1, &exi_l1,                                           
                                              &exi_v2, &exi_l2);
    *ehec_chk = ehec_ck;     
    *ehec_ecc = ehec_err_c;                                    
    *exi_val1 = exi_v1;
    *exi_len1 = exi_l1;
    *exi_val2 = exi_v2;
    *exi_len2 = exi_l2;                                 
    
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_rx_ext_header_get */

/*******************************************************************************
* digi_mapper_gfp_tx_fcs_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets transmit payload FCS field of a GFP frame for a mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*   fcs_mode               - defines whether or not the payload FCS field is  
*                            inserted in a GFP frame\n 
*                            0 -- Do not insert any FCS\n
*                            1 -- Insert pFCS for GFP Frame\n
*                            2 -- Calculate eFCS for special Ethernet traffic 
*                                 received from the CPB after skipping 4-bytes 
*                                 of the packet\n
*                            3 -- Calculate eFCS for special Ethernet traffic 
*                                 received from the CPB after skipping 8-bytes 
*                                 of the packet\n
*                            4 -- Calculate eFCS for special Ethernet traffic 
*                            received from the CPB after skipping 12-bytes of 
*                            the packet\n
*                            5 -- Calculate eFCS for special Ethernet traffic 
*                                 received from the CPB after skipping 16-bytes 
*                                 of the packet\n
*                            6 -- Calculate eFCS for special Ethernet traffic 
*                                 received from the CPB including the preamble 
*                                 bytes\n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*   For the following modes from ENET_LINE to MAPOTN are not allowed to 
*   optionally insert FCS.  The FCS insertion is restricted for these datapaths 
*   as ENET_LINE does not provide flexibility.
*   DIGI_MAP_ODU4P_100_GE_GFP 
*   DIGI_MAP_ODU3P_40_GE_GFP
*   DIGI_MAP_ODU2P_10_GE_6_2_GFP 
*   DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP 
*   DIGI_MAP_ODU3P_PKT_40_GE_GMP
*   DIGI_MAP_ODU4P_PKT_100_GE_GMP 
*   DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP
*   DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_tx_fcs_mode_set(digi_handle_t *digi_handle,
                                                 digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                 mapotn_fcs_mode_t fcs_mode)
{
    PMC_ERROR result = PMC_SUCCESS;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);
    
    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);

    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));

    result = digi_mapper_gfp_tx_fcs_mode_internal_set(digi_handle,
                                                      chnl_ctxt_ptr,
                                                      fcs_mode);
           
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_tx_fcs_mode_set */     
                              
/*******************************************************************************
* digi_mapper_gfp_tx_fcs_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves transmit payload FCS field of a GFP frame for a mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*
* OUTPUTS:
*   *fcs_mode              - pointer to storage for FCS mode of the mapper
*                            channel.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_tx_fcs_mode_get(digi_handle_t *digi_handle,
                                                 digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                 mapotn_fcs_mode_t *fcs_mode)
{
    
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    UINT32 fcs_m = 0;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));

    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_tgfpf_chnl_payload_fcs_get(digi_handle->mapotn_handle,
                                               mapotn_chnl, (mapotn_fcs_mode_t*)&fcs_m);
    
    *fcs_mode = (mapotn_fcs_mode_t)fcs_m;
    
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_tx_fcs_mode_get */

/*******************************************************************************
* digi_mapper_gfp_rx_fcs_chk_mode_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets receive payload FCS check mode field of a GFP frame for a mapper 
*   channel.
*   This API is used when the FCS is terminated in MAPOTN. The pFCS shall be
*   terminated in MAPOTN, while the eFCS can be either:
*   - checked and terminated in MAPOTN
*   - not checked in MAPOTN and forwarded downstream
*   The ENET can be used to monitor the eFCS in a non-intrusive way when
*   configured in enhanced PMON mode. Non-intrusive eFCS monitoring in the ENET
*   is not supported when a GFP extension header is transmitted inband to the
*   COPI.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*   cnt_udf_fcs            - defines whether the GFP processor counts only 
*                            FCS erred User Data Frames (PTI = 000) or any 
*                            FCS erred frame.\n 
*                            0 -- any GFP frame exhibiting a payload FCS error 
*                                 is counted in the ERR_FCS_CNT PM counter\n 
*                            1 -- only GFP User Data Frames (PTI = 000) 
*                                 exhibiting a payload FCS error is counted in 
*                                 the ERR_FCS_CNT PM counter\n
*   fcs_chk_mode           - FCS check mode\n
*                            0 -- No FCS check\n
*                            1 -- pFCS check\n
*                            2 -- eFCS check after skipping programmed number of
*                                 preambles\n
*                            3 -- eFCS check from 1st byte onwards (no preamble
*                                 skipping)\n
*   preamble_size          - length of preamble that needs to be processed in 
*                            payload\n
*                            0 -- Three bytes of preamble and one byte of SFD\n
*                            1 -- Seven bytes of preamble and one byte of SFD\n
*                            2 -- Eleven bytes of preamble and one byte of SFD\n
*                            3 -- Fifteen bytes of preamble and one byte of SFD\n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*   For the following modes from ENET_LINE to MAPOTN are not allowed to 
*   optionally insert FCS.  The FCS insertion is restricted for these datapaths 
*   as ENET_LINE does not provide flexibility.
*   DIGI_MAP_ODU3P_PKT_40_GE_GMP
*   DIGI_MAP_ODU4P_PKT_100_GE_GMP 
*   DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP
*   DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP
*
*   The following are allowed with the noted restrictions:
*   DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET
*   DIGI_MAP_ODU3P_40_GE_GFP_ENET
*   DIGI_MAP_ODU4P_100_GE_GFP_ENET
*   As long as ENET_LINE terminated. Not supported for ENET_SYS terminated
*   data paths.
*
*   DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP
*   DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP
*   DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET
*   DIGI_MAP_ODU2P_10_GE_6_2_GFP
*   DIGI_MAP_ODU4P_100_GE_GFP 
*   DIGI_MAP_ODU3P_40_GE_GFP
*   If fcs_chnk_mode is (0,1) (pFCS). Rejected if fcs_chk_mode (2,3) (eFCS)
*   values as MAPOTN RGFP stripping is not allowed.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_rx_fcs_chk_mode_set(digi_handle_t *digi_handle,
                                                     digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                     UINT32 cnt_udf_fcs,
                                                     UINT32 fcs_chk_mode, 
                                                     UINT32 preamble_size)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    if ((digi_mapper_is_mapotn_mode_gfp_enet(mapper_chnl_handle_ptr) == TRUE) &&
        (digi_mapper_is_far_end_enet_sys(digi_handle, mapper_chnl_handle_ptr)) == TRUE) {

        result = DIGI_ERR_MAPPER_CANNOT_MODIFY;

    } 

    /* Check that MAPPER is not mapped to 10GE GSUP73 ENET  */

    if ((PMC_SUCCESS == result) &&
        (mapper_chnl_handle_ptr->mode == DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP ||
         mapper_chnl_handle_ptr->mode == DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP ||
         mapper_chnl_handle_ptr->mode == DIGI_MAP_ODU2P_CBR_10_GE_7_3_GFP_ENET ||
         mapper_chnl_handle_ptr->mode == DIGI_MAP_ODU2P_10_GE_6_2_GFP ||
         mapper_chnl_handle_ptr->mode == DIGI_MAP_ODU4P_100_GE_GFP ||
         mapper_chnl_handle_ptr->mode ==DIGI_MAP_ODU3P_40_GE_GFP) &&
        ((PMC_SUCCESS == digi_mapper_chnl_dcpb_dest_validate(digi_handle, chnl_ctxt_ptr, UTIL_GLOBAL_CPB_PORT_ENET_LINE)) || 
         (PMC_SUCCESS == digi_mapper_chnl_dcpb_dest_validate(digi_handle, chnl_ctxt_ptr, UTIL_GLOBAL_CPB_PORT_ENET_SYS))))
    {
        if (MAPOTN_RX_FCS_CHK_MODE_SKIP_N_PREAMBLE == fcs_chk_mode || MAPOTN_RX_FCS_CHK_MODE_NO_PREAMBLE_SKIP == fcs_chk_mode) {
            result = DIGI_ERR_RGFP_EFCS_TO_PKT_10GE_7_3_GFP_ILLEGAL;
        }
    }

    if (PMC_SUCCESS == result)
    {    
        result = digi_mapper_gfp_rx_fcs_chk_mode_internal_set(digi_handle,
                                                              chnl_ctxt_ptr,
                                                              cnt_udf_fcs,
                                                              fcs_chk_mode, 
                                                              preamble_size);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_rx_fcs_chk_mode_set */     
                             

                              
/*******************************************************************************
* digi_mapper_null_frame_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures mapper to send and receive NULL client on an alreay provisioned
*   channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*   null_frame_en          - TRUE : In TX ignores ingress data and will map a
*                                   NULL client into OPUk. In RX, NULL client
*                                   is demapped (PT received, payload is ignored).\n
*                            FALSE : Resumes previously configured operation
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
PUBLIC PMC_ERROR digi_mapper_null_frame_cfg(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            BOOL null_frame_en)
{
    
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;

    UINT32 *null_chan_en = NULL;
    UINT32 *prbs_null_chan_en = NULL;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));

    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    /*call API to retrieve spare context that lives in COREOTN*/
    coreotn_mapotn_null_frame_mode_ptr_get(digi_handle->coreotn_handle,
                                           &null_chan_en,
                                           &prbs_null_chan_en);
    

    if(null_frame_en)
    {
        null_chan_en[mapotn_chnl/32] = null_chan_en[mapotn_chnl/32] | (1 << (mapotn_chnl % 32));
    }
    else
    {
        null_chan_en[mapotn_chnl/32] = null_chan_en[mapotn_chnl/32] & ~(1 << (mapotn_chnl % 32));
    }

    
    result = mapotn_null_frame_cfg(digi_handle->mapotn_handle,
                                   mapotn_chnl, 
                                   UTIL_GLOBAL_DIR_BOTH,
                                   null_chan_en,
                                   prbs_null_chan_en);
                                               
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_null_frame_cfg */


/*******************************************************************************
* digi_mapper_null_frame_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets mapper's NULL frame mode
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on   
*                            
* OUTPUTS:
*   *null_frame_en         - TRUE: OPU NULL payload is enabled\n
*                            FALSE: OPU NULL payload is disabled 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_null_frame_get(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr,
                                            BOOL *null_frame_en)
{
    
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 *null_chan_en = NULL;
    UINT32 *prbs_null_chan_en = NULL;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
  
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
  
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
   coreotn_mapotn_null_frame_mode_ptr_get(digi_handle->coreotn_handle,
					   &null_chan_en,
					   &prbs_null_chan_en);
   
   *null_frame_en = (null_chan_en[mapotn_chnl/32] >> (mapotn_chnl % 32)) & 0x1 ;
   
                                           
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_null_frame_cfg */



#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */
/*******************************************************************************
* digi_mapper_gfp_rx_fcs_chk_mode_internal_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets receive payload FCS check mode field of a GFP frame for a mapper 
*   channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*   cnt_udf_fcs            - defines whether the GFP processor counts only 
*                            FCS erred User Data Frames (PTI = 000) or any 
*                            FCS erred frame.\n 
*                            0 -- any GFP frame exhibiting a payload FCS error 
*                                 is counted in the ERR_FCS_CNT PM counter\n 
*                            1 -- only GFP User Data Frames (PTI = 000) 
*                                 exhibiting a payload FCS error is counted in 
*                                 the ERR_FCS_CNT PM counter\n
*   fcs_chk_mode           - FCS check mode\n
*                            0 -- No FCS check\n
*                            1 -- pFCS check\n
*                            2 -- eFCS check after skipping programmed number of
*                                 preambles\n
*                            3 -- eFCS check from 1st byte onwards (no preamble
*                                 skipping)\n
*   preamble_size          - length of preamble that needs to be processed in 
*                            payload\n
*                            0 -- Three bytes of preamble and one byte of SFD\n
*                            1 -- Seven bytes of preamble and one byte of SFD\n
*                            2 -- Eleven bytes of preamble and one byte of SFD\n
*                            3 -- Fifteen bytes of preamble and one byte of SFD\n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*   For the following modes from ENET_LINE to MAPOTN are not allowed to 
*   optionally insert FCS.  The FCS insertion is restricted for these datapaths 
*   as ENET_LINE does not provide flexibility.
*   DIGI_MAP_ODU3P_PKT_40_GE_GMP
*   DIGI_MAP_ODU4P_PKT_100_GE_GMP 
*   DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP
*   DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_mapper_gfp_rx_fcs_chk_mode_internal_set(digi_handle_t *digi_handle,
                                                               digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                               UINT32 cnt_udf_fcs,
                                                               UINT32 fcs_chk_mode, 
                                                               UINT32 preamble_size)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    digi_mapping_mode_t mode;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
   
    mode = mapper_chnl_handle_ptr->mode;
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    if( DIGI_MAP_ODU3P_PKT_40_GE_GMP == mode ||
        DIGI_MAP_ODU4P_PKT_100_GE_GMP == mode || 
        DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP == mode ||
        DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP == mode )
    {
        result = DIGI_ERR_MAPPER_FCS_INSERTION_NOT_ALLOWED;
    } 
    else 
    {
        result = mapotn_rgfpf_fcs_chk_cfg(digi_handle->mapotn_handle,
                                          mapotn_chnl, cnt_udf_fcs, fcs_chk_mode);
    }
                                      
    if (PMC_SUCCESS == result)
    {
        result = mapotn_rgfpf_preamble_sfd_mode_cfg(digi_handle->mapotn_handle,
                                                    mapotn_chnl, preamble_size);
    }
    
    PMC_RETURN(result);

} /* digi_mapper_gfp_rx_fcs_chk_mode_internal_set */
#endif /* DOXYGEN_PUBLIC_ONLY */
                              
/*******************************************************************************
* digi_mapper_gfp_rx_fcs_chk_mode_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves receive payload FCS check mode field of a GFP frame for a mapper 
*   channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*
* OUTPUTS:
*   *fcs_chk_mode          - pointer to storage for FCS check mode of the mapper
*                            channel.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_rx_fcs_chk_mode_get(digi_handle_t *digi_handle,
                                                     digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                     UINT32 *fcs_chk_mode)
{
    
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_rgfpf_fcs_chk_mode_get(digi_handle->mapotn_handle, 
                                           mapotn_chnl, 
                                           fcs_chk_mode);

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_rx_fcs_chk_mode_get */


/*******************************************************************************
* digi_mapper_opu_tx_pt_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets transmit OPU payload type field for a mapper channel. 
*
*   G.709 "Table 15-8 - Payload type code points" dictates that
*   PT should change when inserting NULL frame or PRBS.  Note, that the
*   SW does not modify the TX/RX PT when inserting PRBS or NULL frame
*   (programming the RX/TX PT is the responsibility of the user).
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on 
*   pt_val                 - PT value. Refer to Table 15-8 of G.709 for  
*                            standards defined values. 
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
PUBLIC PMC_ERROR digi_mapper_opu_tx_pt_set(digi_handle_t *digi_handle,
                                           digi_mapper_chnl_t *chnl_ctxt_ptr,
                                           UINT8 pt_val)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_ohfs_insert_chnl_payload_type_set(digi_handle->mapotn_handle,
                                                      mapotn_chnl, pt_val);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_mapper_opu_tx_pt_set */


/*******************************************************************************
* digi_mapper_opu_tx_pt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves transmit OPU payload type field for an mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on 
*
* OUTPUTS:
*   *pt                    - pointer to storage for payload type of the OPU 
*                            payload
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_opu_tx_pt_get(digi_handle_t *digi_handle,
                                           digi_mapper_chnl_t *chnl_ctxt_ptr,
                                           UINT8 *pt)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(pt != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_ohfs_insert_chnl_payload_type_get(digi_handle->mapotn_handle,
                                                      mapotn_chnl, pt);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_mapper_opu_tx_pt_get */


/*******************************************************************************
* digi_mapper_opu_rx_pt_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets receive expected OPU payload type field for a mapper channel. 
*
*   G.709 "Table 15-8 - Payload type code points" dictates that
*   PT should change when inserting NULL frame or PRBS.  Note, that the
*   SW does not modify the TX/RX PT when inserting PRBS or NULL frame
*   (programming the RX/TX PT is the responsibility of the user).
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on 
*   pt_val                 - PT value. Refer to Table 15-8 of G.709 for  
*                            standards defined values. 
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
PUBLIC PMC_ERROR digi_mapper_opu_rx_pt_set(digi_handle_t *digi_handle,
                                           digi_mapper_chnl_t *chnl_ctxt_ptr,
                                           UINT8 pt_val)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_ohfs_remove_chnl_payload_type_set(digi_handle->mapotn_handle,
                                                      mapotn_chnl, pt_val);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_mapper_opu_rx_pt_set */


/*******************************************************************************
* digi_mapper_opu_rx_accpt_pt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves receive accepted OPU payload type field for an mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on 
*
* OUTPUTS:
*   *accpt_pt_val          - pointer to storage for payload type of the OPU 
*                            payload
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_opu_rx_accpt_pt_get(digi_handle_t *digi_handle,
                                                 digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                 UINT8 *accpt_pt_val)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(accpt_pt_val != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_ohfs_remove_chnl_accpt_payload_type_get(digi_handle->mapotn_handle,
                                                            mapotn_chnl, accpt_pt_val);
    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_mapper_opu_rx_accpt_pt_get */


/*******************************************************************************
* digi_mapper_opu_rx_exp_pt_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves receive expected OPU payload type field for an mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on 
*   *exp_pt_val            - pointer to storage for payload type of the OPU 
*                            payload
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
PUBLIC PMC_ERROR digi_mapper_opu_rx_exp_pt_get(digi_handle_t *digi_handle,
                                               digi_mapper_chnl_t *chnl_ctxt_ptr,
                                               UINT8 *exp_pt_val)
{
        
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(exp_pt_val != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
   
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_ohfs_remove_chnl_exp_payload_type_get(digi_handle->mapotn_handle,
                                                          mapotn_chnl, exp_pt_val);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_mapper_opu_rx_exp_pt_get */


/*******************************************************************************
* digi_mapper_gfp_client_classify_prov
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
*   *digi_handle        - pointer to DIGI handle instance
*   *chnl_ctxt_ptr      - pointer to mapper channel handle to be operated on                                                  
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
PUBLIC PMC_ERROR digi_mapper_gfp_client_classify_prov(digi_handle_t *digi_handle,
                                                      digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                      mapotn_rgfpf_cmf_dest_t cmf_dest,
                                                      mapotn_rgfpf_cmf_upi_filt_mode_t cmf_upi_filt_mode,
                                                      UINT32 upi_range_1_start, UINT32 upi_range_1_end, 
                                                      UINT32 upi_range_2_start, UINT32 upi_range_2_end)
{                                            
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));

    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    result = mapotn_rgfpf_client_classify_prov(digi_handle->mapotn_handle,
                                               mapotn_chnl, cmf_dest, cmf_upi_filt_mode,
                                               upi_range_1_start, upi_range_1_end, 
                                               upi_range_2_start, upi_range_2_end);
    
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_client_classify_prov */


/*******************************************************************************
* digi_mapper_gfp_classify_unprov
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*    Unprovisions all 10 RGFPF filters for a mapper channel.
*                                                                               
* INPUTS:                                                                       
*   *digi_handle        - pointer to DIGI handle instance
*   *chnl_ctxt_ptr      - pointer to mapper channel handle to be operated on   
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
PUBLIC PMC_ERROR digi_mapper_gfp_classify_unprov(digi_handle_t *digi_handle,
                                                 digi_mapper_chnl_t *chnl_ctxt_ptr)    
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    result = mapotn_rgfpf_classify_unprov(digi_handle->mapotn_handle, mapotn_chnl);
    
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_gfp_classify_unprov */


/*******************************************************************************
* digi_mapper_gfp_user_client_classify_prov
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
*   *digi_handle        - pointer to DIGI handle instance
*   *chnl_ctxt_ptr      - pointer to mapper channel handle to be operated on   
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
*   pfi_value           - Value of PFI to compare with received frame (0 or 1)
*   proc_seq            - This parameter is unused (set to MAPOTN_PRE_CLASSIFY).
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
PUBLIC PMC_ERROR digi_mapper_gfp_user_client_classify_prov(digi_handle_t *digi_handle,
                                                           digi_mapper_chnl_t *chnl_ctxt_ptr, 
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
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
        
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_rgfpf_user_client_classify_prov(digi_handle->mapotn_handle, 
                                                    mapotn_chnl, frame_field_mask, 
                                                    pti_comp, pti_value,
                                                    upi_comp, upi_value,
                                                    exi_comp, exi_value,
                                                    pfi_comp, pfi_value,
                                                    proc_seq, action);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_mapper_gfp_user_client_classify_prov */

/*******************************************************************************
* digi_mapper_gfp_classify_cfg
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Configure a filter to classify GFP frames based on PTI, UPI and/or EXI. 
*   A Filter can be configured to send client frames to the egress, to send 
*   management frames to the CPU or to drop frames. \n
*
*   The filters should be configured as follows:\n
*           1) Management frame filter  (Optional)\n
*               -Send management frames to CPU.\n
*           2) Extension header filter (Optional)\n
*               -Configure a filter to drop frames with an incorrect extension 
*                header. A dEXM will be triggered if the extension header 
*                indicator is invalid.\n
*           3) Client frame UPI filters \n
*               -Configure filters to send client frames with particular UPIs
*                to the egress. In addition, a filter can be configured to
*                drop the remaining client frames (that have incorrect UPI
*                values) and trigger a dUPM.\n
*           4) Drop remaining frames\n\n
*
*    1) Send managmenet frames to the CPU \n
* \code
*           result = digi_mapper_gfp_classify_cfg(
*                       digi_handle, 
*                       mapper_chnl_ptr,
*                       0,                              -- filter 0
*                       0x1,                            -- Check PTI
*                       MAPOTN_FLD_EQ,4,                -- check for PTI=4
*                       0,0,                   -        -- UPI = don't care
*                       0,0,                            -- EXI - don't care
*                       MAPOTN_ACTION_CPU);             -- send to cpu    \endcode
*
*   2) Extension header indicator filter \n
*       Create a filter to drop frames with an extension header indicator 
*       that's not 'exi'. A dEXM will be triggered when a frame is dropped 
*       due to an extension header mismatch.\n
*\code
*           result = digi_mapper_gfp_classify_cfg(
*                       digi_handle, 
*                       mapper_chnl_ptr,
*                       1,                              -- filter 1
*                       0x4,                            -- Check EXI
*                       0,0,                            -- PTI = Don't care
*                       0,0,                   -        -- UPI = don't care
*                       MAPONT_FLD_NQ,exi,              -- EXI != 'exi'
*                       MAPOTN_ACTION_DROP),            -- Drop frames      \endcode
*       
*   3) UPI Filters \n
*        Create two filters - one to send client frames with the expected UPI to 
*        the egress and a second filter to drop the remaining client frames that
*        have non-matching UPI values.\n
*       Filter 2: send client frames with a particular UPI value ('upi') to
*        the egress:\n \code
*           result = digi_mapper_gfp_classify_cfg(
*                       digi_handle, 
*                       mapper_chnl_ptr,
*                       2,                     -- filter # 2
*                       0x3,                   -- Check UPI and PTI
*                       MAPOTN_FLD_EQ,1,       -- PTI = client frames
*                       MAPOTN_FLD_EQ,upi,     -- UPI = expected 'upi'
*                       0,0,                   -- EXI - don't care
*                       MAPOTN_ACTION_EGRESS)  -- send to egress        \endcode \n
*
*       Filter 3: Drop remaining client frames + trigger a dUPM.\n \code
*           result = digi_mapper_gfp_classify_cfg(
*                       digi_handle, 
*                       mapper_chnl_ptr,
*                       3,                     -- filter # 3
*                       0x1,                   -- Check PTI
*                       MAPOTN_FLD_EQ,1,       -- PTI = client frames
*                       0,0,                   -- UPI = don't care
*                       0,0,                   -- EXI = don't care
*                       MAPOTN_ACTION_DROP)    -- Drop frames            \endcode \n
*
*       If UPI checking is not required, the two filters can be replaced with 
*       one filter that sends all client frames to the egress.\n
*
*       Filter 2: Send client frames to the egress\n \code
*           result = digi_mapper_gfp_classify_cfg(
*                       digi_handle, 
*                       mapper_chnl_ptr,
*                       2,                     -- filter # 2
*                       0x1,                   -- Check PTI
*                       MAPOTN_FLD_EQ, 0,      -- check for PTI=0
*                       0,0,                   -- UPI = don't care
*                       0,0,                   -- EXI - don't care
*                       MAPOTN_ACTION_EGRESS); -- Send to egress \endcode \n
*   4) Drop remaining frames\n
*       Filter 4: Create a filter to drop the remaining frames.\n \code
*           result = digi_mapper_gfp_classify_cfg(
*                       digi_handle, 
*                       mapper_chnl_ptr,
*                       4,                     -- Filter # 4
*                       0x0,                   -- Don't check PTI/UPI/EXI
*                       0,0,                   -- PTI = Don't care.
*                       0,0,                   -- UPI = don't care
*                       0,0,                   -- EXI - don't care
*                       MAPOTN_ACTION_DROP);   -- Drop frames \endcode \n
*
* INPUTS:                                                                       
*   *digi_handle        - pointer to DIGI handle instance
*   *chnl_ctxt_ptr      - pointer to mapper channel handle to be operated on   
*   filter_no           - Filter number (0..9)
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
*   This function is an alternative to using 
*   digi_mapper_gfp_client_classify_prov/digi_mapper_gfp_user_client_classify
*   for applications which require set/get operations. It is not possible to
*   mix calls to digi_mapper_gfp_classify_cfg with calls to
*   digi_mapper_gfp_client_classify_prov or 
*   digi_mapper_gfp_user_client_classify.
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_classify_cfg(digi_handle_t *digi_handle,
                                              digi_mapper_chnl_t *chnl_ctxt_ptr, 
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
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_rgfpf_classify_cfg(digi_handle->mapotn_handle, 
                                       mapotn_chnl, 
                                       filter_no, 
                                       frame_field_mask, 
                                       pti_comp, pti_value,
                                       upi_comp, upi_value,
                                       exi_comp, exi_value,
                                       action);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_mapper_gfp_classify_cfg */


/*******************************************************************************
* digi_mapper_gfp_classify_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Retrieve the configuration of a particular filter. \n
*                                                                               
* INPUTS:                                                                       
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on   
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
*   calls to mapotn_rgfpf_classify_set. 
*                                                                               
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_classify_get(digi_handle_t *digi_handle,
                                            digi_mapper_chnl_t *chnl_ctxt_ptr, 
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
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
 
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
       
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_rgfpf_classify_get(digi_handle->mapotn_handle, 
                                     mapotn_chnl, 
                                     filter_no, 
                                     frame_field_mask_ptr, 
                                     pti_comp_ptr, pti_value_ptr,
                                     upi_comp_ptr, upi_value_ptr,
                                     exi_comp_ptr, exi_value_ptr,
                                     action_ptr);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_gfp_classify_get */
/*******************************************************************************
* digi_mapper_cpu_fifo_enable
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Enables CPU FIFO. 
*                                                                               
*                                                                               
* INPUTS:                                                                       
*   *digi_handle        - pointer to DIGI handle instance
*   *chnl_ctxt_ptr      - pointer to mapper channel handle to be operated on                                                   
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
PUBLIC PMC_ERROR digi_mapper_cpu_fifo_enable(digi_handle_t *digi_handle,
                                             digi_mapper_chnl_t *chnl_ctxt_ptr)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
        
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    result = mapotn_rgfpf_cpu_fifo_reset(digi_handle->mapotn_handle,
                                         mapotn_chnl, 1);
    if(PMC_SUCCESS == result)
    {
        result = mapotn_rgfpf_cpu_fifo_reset(digi_handle->mapotn_handle,
                                             mapotn_chnl, 0);                                                                         
    }
                                                                                                                                   
    
    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_cpu_fifo_enable */



/*******************************************************************************
*  digi_mapper_gfp_pause_frm_mem_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the 4 pause frame templates (memory & time) for the TGFPF block
*   in MAPOTN subsystem.
*
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
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
PUBLIC PMC_ERROR digi_mapper_gfp_pause_frm_mem_cfg(digi_handle_t *digi_handle,
                                                   UINT32 pause_time[MAPOTN_TGFPF_PAUSE_FRAME_TEMPLATE_CNT],
                                                   UINT32 pause_frame_mem[MAPOTN_TGFPF_PAUSE_FRAME_MEM_TOTAL_REG_CNT])
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    result = mapotn_tgfpf_pause_frm_mem_cfg(digi_handle->mapotn_handle, 
                                            pause_time, pause_frame_mem);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_mapper_gfp_pause_frm_mem_cfg */


/*******************************************************************************
* digi_mapper_rx_flow_control_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enable/Disable Ethernet RX flow control of a provisioned channel.\n
*  
*  By enabling RX flow conbtrol, MAPOTN channel interprets XOFF and XON pause \n
*  frames received in the ENET stream interface. 
*
*  Only ENET GFP-stream support WAN flow control.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   *digi_otn_mapper_prov   - pointer to MAPPER channel handle instance
*   *digi_mapotn_gfp_enet_pause_filter_cfg - pointer to the GFP ENET pause
*                                            filter configuration
*   enable                  - TRUE : Flow control enabled\n
*                             FALSE: Flow control disabled 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_rx_flow_control_set(digi_handle_t                            *digi_handle,
                                                 digi_mapper_chnl_t                       *digi_otn_mapper_prov,
                                                 digi_mapotn_gfp_enet_pause_filter_cfg_t  *digi_mapotn_gfp_enet_pause_filter_cfg,
                                                 BOOL8                                    enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 mapotn_chnl;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
       
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check handles */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_otn_mapper_prov, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(digi_mapotn_gfp_enet_pause_filter_cfg != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
        
    /* resolve handles */
    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(digi_otn_mapper_prov);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(mapper_chnl_handle_ptr);    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    /* configure RX flow control */
    rc = mapotn_rgfpf_pause_frm_detect_cfg(digi_handle->mapotn_handle,
                                           mapotn_chnl, 
                                           digi_mapotn_gfp_enet_pause_filter_cfg->pause_filter,
                                           enable, 
                                           digi_mapotn_gfp_enet_pause_filter_cfg->pause_mode,
                                           digi_mapotn_gfp_enet_pause_filter_cfg->pause_upi, 
                                           digi_mapotn_gfp_enet_pause_filter_cfg->pause_pti,
                                           enable);

    PMC_ATOMIC_RETURN(digi_handle,rc);
}

/*******************************************************************************
* digi_mapper_rx_flow_control_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Get Ethernet RX flow control configuration of a provisioned channel.\n
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   *digi_otn_mapper_prov   - pointer to MAPPER channel handle instance
*
* OUTPUTS:
*   *digi_mapotn_gfp_enet_pause_filter_cfg - pointer to the GFP ENET pause
*                                            filter configuration
*   *enable                - TRUE : Flow control enabled\n
*                            FALSE: Flow control disabled 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_rx_flow_control_get(digi_handle_t                           *digi_handle,
                                                 digi_mapper_chnl_t                      *digi_otn_mapper_prov,
                                                 digi_mapotn_gfp_enet_pause_filter_cfg_t *digi_mapotn_gfp_enet_pause_filter_cfg,
                                                 BOOL8                                   *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 mapotn_chnl;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    BOOL8 dummy8;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check handles */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_otn_mapper_prov, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(digi_mapotn_gfp_enet_pause_filter_cfg != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    
    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, digi_otn_mapper_prov, rc);

    /* resolve handles */
    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(digi_otn_mapper_prov);
    DIGI_CHNL_HANDLE_MAGIC_CHECK(mapper_chnl_handle_ptr);    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

     /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));

    /* get RX flow control configuration */
    rc = mapotn_rgfpf_pause_frm_detect_cfg_get(digi_handle->mapotn_handle,
                                               mapotn_chnl, 
                                               &digi_mapotn_gfp_enet_pause_filter_cfg->pause_filter,
                                               enable, 
                                               &digi_mapotn_gfp_enet_pause_filter_cfg->pause_mode,
                                               &digi_mapotn_gfp_enet_pause_filter_cfg->pause_upi, 
                                               &digi_mapotn_gfp_enet_pause_filter_cfg->pause_pti,
                                               &dummy8);
    
   
    PMC_ATOMIC_RETURN(digi_handle,rc);
}
/*******************************************************************************
* digi_mapper_tx_flow_control_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Enable/Disable Ethernet TX flow control of a provisioned channel.\n
*  
*   TX flow control can be controlled either by local resource (DCPB) or by \n
*  external resource (flow control information from ILKN interface). Whenever\n
*  the DIGI_ENET_FLOW_CONTROL_BUFFER_LOCAL mode is set (DCPB resources are\n
*  used to control TX flow control), it is assumed that DCPB buffer has been\n
*  properly sized when it has been allocated with digi_cpb_internal_chnl_map\n
*  API.
*
*  Only ENET GFP-stream support WAN flow control.
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   *digi_otn_mapper_prov   - pointer to MAPPER channel handle instance
*   *digi_mapotn_wan_pause_cfg - pointer to the ENET wan pause configuration
*   enable                  - TRUE : Flow control enabled\n
*                             FALSE: Flow control disabled 
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_tx_flow_control_set(digi_handle_t                      *digi_handle,
                                                 digi_mapper_chnl_t                 *digi_otn_mapper_prov,
                                                 digi_mapotn_wan_pause_cfg_t        *digi_mapotn_wan_pause_cfg,
                                                 BOOL8                               enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 mapotn_chnl;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
       
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check handles */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_otn_mapper_prov, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(digi_mapotn_wan_pause_cfg != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
       
    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, digi_otn_mapper_prov, rc);

    /* resolve handles */
    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(digi_otn_mapper_prov);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(mapper_chnl_handle_ptr);    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));

    if (TRUE == enable ) 
    {
        /* [BT] CPB zone bypass is only used for ENET/MAP -> SIFD data paths. It can't be set here because we
         * also need to know the SIFD port and channel. The method to set zone bypass is to first call
         * digi_ilkn_cpb_chnl_prov and pass in rx_fc_reach_extend = FALSE.
         rc = cpb_dpi_slave_port_chnl_zone_type_cfg(digi_handle->dcpb_handle,
         mapper_chnl_handle_ptr->switch_data.cpb_data.port_type,
         mapper_chnl_handle_ptr->switch_data.cpb_data.channel,
         (DIGI_ENET_FLOW_CONTROL_BUFFER_EXTERNAL == digi_mapotn_wan_pause_cfg->buffer_mode)?CPB_SLV_CHNL_ZONE_FIFO_AND_MSTR_XOFF:CPB_SLV_CHNL_ZONE_FIFO);
        */

        /* zone threshold when they are not used */
        if (PMC_SUCCESS == rc && DIGI_ENET_FLOW_CONTROL_BUFFER_LOCAL == digi_mapotn_wan_pause_cfg->buffer_mode)
        {
            /* configure DCPB threshold */
            rc = cpb_dpi_slave_port_chnl_zone_thresh_cfg_check(digi_handle->dcpb_handle,
                                                               mapper_chnl_handle_ptr->switch_data.cpb_data.port_type,
                                                               mapper_chnl_handle_ptr->switch_data.cpb_data.channel,
                                                               0,
                                                               0,                                                                 
                                                               digi_mapotn_wan_pause_cfg->xon_xoff_threshold);                
        }
        /* configure zone level in MAPOTN */
        /* we are always using 3per convention */
        if (PMC_SUCCESS == rc) 
        {
            rc = mapotn_rgfpf_chnl_cpb_zone_thrld_cfg(digi_handle->mapotn_handle,
                                                      mapotn_chnl, 
                                                      DIGI_MAPPER_THRESH_DEFAULT_VAL);         
        }
    }
        
    if (PMC_SUCCESS == rc) 
    {
        rc = mapotn_tgfpf_chnl_pause_frm_cfg(digi_handle->mapotn_handle,
                                             mapotn_chnl,
                                             enable,
                                             DIGI_MAPPER_PAUSE_PRIORITY,           
                                             digi_mapotn_wan_pause_cfg->xon_template_id, 
                                             digi_mapotn_wan_pause_cfg->xoff_template_id);
    }

    PMC_ATOMIC_RETURN(digi_handle,rc);
}

/*******************************************************************************
* digi_mapper_tx_flow_control_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Get Ethernet TX flow control configuration of a provisioned channel.\n
*  
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   *digi_otn_mapper_prov   - pointer to MAPPER channel handle instance
*
* OUTPUTS:
*   *digi_mapotn_wan_pause_cfg - pointer to the ENET WAN pause configuration
*                               Only valid when flow control is enabled
*   *enable                 - TRUE : Flow control enabled\n
*                             FALSE: Flow control disabled 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_tx_flow_control_get(digi_handle_t                      *digi_handle,
                                                 digi_mapper_chnl_t                 *digi_otn_mapper_prov,
                                                 digi_mapotn_wan_pause_cfg_t        *digi_mapotn_wan_pause_cfg,
                                                 BOOL8                              *enable)
{
    PMC_ERROR rc = PMC_SUCCESS;
    UINT32 mapotn_chnl;
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 dummy;
    BOOL8 dummy8;
    cpb_dpi_chnl_zone_mode_t mode;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check handles */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != digi_otn_mapper_prov, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(digi_mapotn_wan_pause_cfg != NULL, DIGI_ERR_INVALID_ARG, 0, 0);
    
    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, digi_otn_mapper_prov, rc);

    /* resolve handles */
    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(digi_otn_mapper_prov);
    DIGI_CHNL_HANDLE_MAGIC_CHECK(mapper_chnl_handle_ptr);    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
   
    /* get TX flow control configuration */
    rc = mapotn_tgfpf_chnl_pause_frm_cfg_get(digi_handle->mapotn_handle,
                                             mapotn_chnl,
                                             enable,
                                             &dummy8,
                                             &digi_mapotn_wan_pause_cfg->xon_template_id, 
                                             &digi_mapotn_wan_pause_cfg->xoff_template_id);

    if (PMC_SUCCESS == rc && TRUE == *enable ) 
    {
        /* configure TX flow control  */    
        rc = cpb_dpi_slave_port_chnl_zone_type_cfg_get(digi_handle->dcpb_handle,                                                       
                                                       mapper_chnl_handle_ptr->switch_data.cpb_data.port_type,
                                                       mapper_chnl_handle_ptr->switch_data.cpb_data.channel,
                                                       &mode);
        digi_mapotn_wan_pause_cfg->buffer_mode =  (mode == CPB_SLV_CHNL_ZONE_FIFO)?DIGI_ENET_FLOW_CONTROL_BUFFER_LOCAL:DIGI_ENET_FLOW_CONTROL_BUFFER_EXTERNAL;
        /* zone threshold when they are not used */
        if (PMC_SUCCESS == rc && DIGI_ENET_FLOW_CONTROL_BUFFER_LOCAL == digi_mapotn_wan_pause_cfg->buffer_mode)
        {
            /* configure DCPB threshold */
            rc = cpb_dpi_slave_port_chnl_zone_thresh_cfg_get(digi_handle->dcpb_handle,                                 
                                                             mapper_chnl_handle_ptr->switch_data.cpb_data.port_type,
                                                             mapper_chnl_handle_ptr->switch_data.cpb_data.channel,
                                                             &dummy,
                                                             &dummy,                                                                 
                                                             &digi_mapotn_wan_pause_cfg->xon_xoff_threshold);                
        }

    }
       
    PMC_ATOMIC_RETURN(digi_handle,rc);
}

/*******************************************************************************
*  digi_mapper_gfp_pause_frm_mac_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the Ethernet MAC destination address. This function should be 
*   configured with MAC destination address present in pause frames. 
*   Pause time is extracted only if the Pause frame is having configured MAC 
*   destination address or the unique multi-cast address of 0x0180C2000001.
*
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   mac_da_lsb          - 32 bit LSB of MAC destination address
*   mac_da_msb          - 16 bit MSB of MAC destination address
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
PUBLIC PMC_ERROR digi_mapper_gfp_pause_frm_mac_cfg(digi_handle_t *digi_handle,
                                                   UINT32 mac_da_lsb,
                                                   UINT32 mac_da_msb)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    result = mapotn_rgfpf_enet_mac_dest_addr_cfg(digi_handle->mapotn_handle,
                                                 mac_da_lsb, mac_da_msb);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_mapper_gfp_pause_frm_mac_cfg */

/*******************************************************************************
* digi_mapper_gfp_xoff_pause_frm_insrt
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Inserts the XOFF pause frame into the GFP traffic. The XOFF pause template
*   to use for the channel must have been previously selected.
*
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   *chnl_ctxt_ptr      - pointer to mapper channel handle to be operated on 
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
PUBLIC PMC_ERROR digi_mapper_gfp_xoff_pause_frm_insrt(digi_handle_t *digi_handle,
                                                      digi_mapper_chnl_t *chnl_ctxt_ptr)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_tgfpf_pause_frm_insrt(digi_handle->mapotn_handle,
                                          mapotn_chnl);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
    
} /* digi_mapper_gfp_xoff_pause_frm_insrt */

/*******************************************************************************
* digi_mapper_dplm_to_ssf_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Controls consequential actions asociated with DPLM
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   *chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on 
*   enable          - 1 : enable DPLM to SSF consequential action\n
*                     0 : disable DPLM to SSF consequential action
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
PUBLIC PMC_ERROR digi_mapper_dplm_to_ssf_set(digi_handle_t *digi_handle, 
                                             digi_mapper_chnl_t  *chnl_ctxt_ptr,
                                             BOOL8 enable)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_map_to_ssf_set(digi_handle->mapotn_handle,
                                   mapotn_chnl, 
                                   OHFS_REMOVE_MAP_DPLM_TO_SSF, 
                                   (UINT32)enable);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_dplm_to_ssf_set */

/*******************************************************************************
* digi_mapper_dplm_to_ssf_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets consequential action associated with DPLM.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   *chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on 
*
* OUTPUTS:
*   *enable         - 1 : DPLM to SSF consequential action is enabled\n
*                     0 : DPLM to SSF consequential action is disabled
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_dplm_to_ssf_get(digi_handle_t *digi_handle, 
                                             digi_mapper_chnl_t  *chnl_ctxt_ptr,
                                             BOOL8 *enable)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 enable32;
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_map_to_ssf_get(digi_handle->mapotn_handle,
                                   mapotn_chnl, 
                                   OHFS_REMOVE_MAP_DPLM_TO_SSF, 
                                   &enable32);
    *enable = enable32;

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_dplm_to_ssf_get */

/*******************************************************************************
* digi_mapper_dcsf_to_ssf_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Controls consequential actions asociated with DCSF
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   *chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on 
*   enable          - 1 : enable DCSF to SSF consequential action\n
*                     0 : disable DCSF to SSF consequential action
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
PUBLIC PMC_ERROR digi_mapper_dcsf_to_ssf_set(digi_handle_t *digi_handle, 
                                             digi_mapper_chnl_t  *chnl_ctxt_ptr,
                                             BOOL8 enable)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_map_to_ssf_set(digi_handle->mapotn_handle,
                                   mapotn_chnl, 
                                   OHFS_REMOVE_MAP_DCSF_TO_SSF, 
                                   (UINT32)enable);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_dcsf_to_ssf_set */

/*******************************************************************************
* digi_mapper_dcsf_to_ssf_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets consequential action associated with DCSF.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   *chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on 
*
* OUTPUTS:
*   *enable         - 1 : DCSFto SSF consequential action is enabled\n
*                     0 : DCSF to SSF consequential action is disabled
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_dcsf_to_ssf_get(digi_handle_t *digi_handle, 
                                             digi_mapper_chnl_t  *chnl_ctxt_ptr,
                                             BOOL8 *enable)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 enable32;
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_map_to_ssf_get(digi_handle->mapotn_handle,
                                   mapotn_chnl, 
                                   OHFS_REMOVE_MAP_DCSF_TO_SSF, 
                                   &enable32);
    *enable = enable32;

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_dcsf_to_ssf_get */


/*******************************************************************************
* digi_mapper_sf_to_ssf_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Controls mapping of SF to SSF
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   *chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on 
*   enable          - 1 : enable SF to SSF consequential action\n
*                     0 : disable SF to SSF consequential action
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
PUBLIC PMC_ERROR digi_mapper_sf_to_ssf_set(digi_handle_t *digi_handle, 
                                           digi_mapper_chnl_t  *chnl_ctxt_ptr,
                                           BOOL8 enable)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_map_to_ssf_set(digi_handle->mapotn_handle,
                                   mapotn_chnl, 
                                   OHFS_REMOVE_MAP_SF_TO_SSF, 
                                   (UINT32)enable);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_sf_to_ssf_set */

/*******************************************************************************
* digi_mapper_sf_to_ssf_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Controls mapping of SF to SSF
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   *chnl_ctxt_ptr  - pointer to mapper channel handle to be operated on 
*
* OUTPUTS:
*   *enable         - 1 : SF to SSF consequential action is enabled\n
*                     0 : SF to SSF consequential action is disabled
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_sf_to_ssf_get(digi_handle_t *digi_handle, 
                                           digi_mapper_chnl_t  *chnl_ctxt_ptr,
                                           BOOL8 *enable)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 enable32;
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* check handle provisioning state */
    DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, (&mapper_chnl_handle_ptr->switch_data));
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    
    result = mapotn_map_to_ssf_get(digi_handle->mapotn_handle,
                                   mapotn_chnl, 
                                   OHFS_REMOVE_MAP_SF_TO_SSF, 
                                   &enable32);
    *enable = enable32;

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_sf_to_ssf_get */
         
/*******************************************************************************
* digi_mapper_pmon_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation PMON table with PMON counters 
*   over multiple channels.
*
*   digi_mapper_pmon_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*
* OUTPUTS:
*   pmon_table_ptr  - pointer to aggregation PMON table to store counters 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_pmon_retrieve(digi_handle_t       *digi_handle,
                                           digi_mapper_pmon_t  *pmon_table_ptr)
{
    PMC_ERROR                       result = PMC_SUCCESS;
    digi_mapper_chnl_t             *chnl_handle;
    digi_mapper_chnl_def_t         *chnl_def_handle;
    UINT32                          i;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_pmon_param_check(digi_handle, pmon_table_ptr); 

    if (PMC_SUCCESS == result)
    {
        PMC_MEMSET(pmon_table_ptr, 0, sizeof(digi_mapper_pmon_t));

        for (i = 0; (i < DIGI_MAPOTN_CHNL_MAX) && (PMC_SUCCESS == result); i++)
        {
            chnl_handle = (digi_mapper_chnl_t *) &digi_handle->handle_pool.mapper_chnl[i];
            chnl_def_handle = (digi_mapper_chnl_def_t *) chnl_handle;
           
            if (util_global_is_chnl_prov(chnl_def_handle))
            {
                pmon_table_ptr->handle[i] = chnl_handle;
                result = digi_mapper_pmon_chnl_retrieve(digi_handle,
                                                        chnl_handle,
                                                        &pmon_table_ptr->mapper_chnl[i]);
            }
        } /* end for */
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_pmon_retrieve */


/*******************************************************************************
* digi_mapper_pmon_fo2_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation PMON table with PMON counters 
*   over multiple channels.
*
*   digi_mapper_pmon_fo2_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*
* OUTPUTS:
*   pmon_table_ptr  - pointer to aggregation PMON table to store counters 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_pmon_fo2_retrieve(digi_handle_t          *digi_handle,
                                               digi_mapper_pmon_fo2_t  *pmon_table_ptr)
{
    PMC_ERROR                       result = PMC_SUCCESS;
    digi_mapper_chnl_t             *chnl_handle;
    digi_mapper_chnl_def_t         *chnl_def_handle;
    UINT32                          i;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_pmon_param_check(digi_handle, pmon_table_ptr); 

    if (PMC_SUCCESS == result)
    {
        PMC_MEMSET(pmon_table_ptr, 0, sizeof(digi_mapper_pmon_fo2_t));

        for (i = 0; (i < DIGI_MAPOTN_CHNL_MAX) && (PMC_SUCCESS == result); i++)
        {
            chnl_handle = (digi_mapper_chnl_t *) &digi_handle->handle_pool.mapper_chnl[i];
            chnl_def_handle = (digi_mapper_chnl_def_t *) chnl_handle;
           
            if (util_global_is_chnl_prov(chnl_def_handle))
            {
                pmon_table_ptr->handle[i] = chnl_handle;
                result = digi_mapper_pmon_fo2_chnl_retrieve(digi_handle,
                                                            chnl_handle,
                                                            &pmon_table_ptr->mapper_fo2_chnl[i]);
            }
        } /* end for */
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_pmon_fo2_retrieve */




/*******************************************************************************
* digi_mapper_pmon_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a channel handle and an aggregated PMON table, retrieve a pointer
*   to the corresponding channel PMON table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to retrieve PMON on
*   pmon_table_ptr  - Aggregated PMON table
*
* OUTPUTS:
*   pmon_chnl_table_pptr - pointer to channel PMON table corresponding to channel
*                          handle. Set to NULL if channel has not been provisioned.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_pmon_chnl_get(digi_handle_t                *digi_handle,
                                           digi_mapper_chnl_t           *chnl_handle,
                                           digi_mapper_pmon_t           *pmon_table_ptr,
                                           digi_mapper_pmon_chnl_t     **pmon_chnl_table_pptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          index;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != pmon_table_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != pmon_chnl_table_pptr, DIGI_ERR_INVALID_ARG, 0, 0);

    /* Set return table to NULL */
    *pmon_chnl_table_pptr = NULL;

    /* Get index corresponding to handle */
    digi_mapper_channel_num_get(digi_handle, chnl_handle, &index, NULL, NULL, NULL);

    /* Set return table to channel table */
    if (pmon_table_ptr->handle[index] == chnl_handle)
    {
        *pmon_chnl_table_pptr = &pmon_table_ptr->mapper_chnl[index];
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_pmon_chnl_get */


/*******************************************************************************
* digi_mapper_pmon_fo2_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a channel handle and an aggregated PMON table, retrieve a pointer
*   to the corresponding channel PMON table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to retrieve PMON on
*   pmon_table_ptr  - Aggregated PMON table
*
* OUTPUTS:
*   pmon_chnl_table_pptr - pointer to channel PMON table corresponding to channel
*                          handle. Set to NULL if channel has not been provisioned.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_pmon_fo2_chnl_get(digi_handle_t                *digi_handle,
                                               digi_mapper_chnl_t           *chnl_handle,
                                               digi_mapper_pmon_fo2_t       *pmon_table_ptr,
                                               digi_mapper_pmon_fo2_chnl_t **pmon_chnl_table_pptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          index;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != pmon_table_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != pmon_chnl_table_pptr, DIGI_ERR_INVALID_ARG, 0, 0);

    /* Set return table to NULL */
    *pmon_chnl_table_pptr = NULL;

    /* Get index corresponding to handle */
    digi_mapper_channel_num_get(digi_handle, chnl_handle, &index, NULL, NULL, NULL);

    /* Set return table to channel table */
    if (pmon_table_ptr->handle[index] == chnl_handle)
    {
        *pmon_chnl_table_pptr = &pmon_table_ptr->mapper_fo2_chnl[index];
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_pmon_fo2_chnl_get */



/*******************************************************************************
* digi_mapper_int_register_cb
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for aggregation interrupt table processing.
*
*   This callback will be called when an active interrupt is found in any
*   MAPPER channel, except for Stg4 FO2 OTN interrupts. Interrupts found in
*   FO2 will be available via the callback registered via digi_otn_int_register_cb()
*
*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of one time if there is an interrupt in at least
*   one channel.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   token           - user token to be stored and passed back to callback\n
*                     Optional, can be NULL.
*   token_size      - token size in bytes\n
*                     Optional, can be 0.
*   user_cb         - callback to be registered against this interrupt type
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
PUBLIC PMC_ERROR digi_mapper_int_register_cb(digi_handle_t      *digi_handle,
                                             void               *token,
                                             UINT32              token_size,
                                             digi_int_cb_fcn     user_cb)
{
    PMC_ERROR            result = PMC_SUCCESS;
    digi_int_type_t      int_type = DIGI_INT_TYPE_OTN_MAPPER;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);



    if (PMC_SUCCESS == result)
    {
        result = digi_int_callback_set(digi_handle,
                                       int_type,
                                       token,
                                       token_size,
                                       user_cb);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_register_cb */


/*******************************************************************************
* digi_mapper_int_chnl_register_cb
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for channel interrupt table processing.
*
*   For each mapper channel where an active interrupt is found, the callback will be
*   called.
*
*   The callback is called a minimum of zero times if no active interrupts are
*   found, or a maximum of the number of channels in the subsystem if there is
*   an active interrupt in each of the channels.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   token           - user token to be stored and passed back to callback\n
*                     Optional, can be NULL.
*   token_size      - token size in bytes\n
*                     Optional, can be 0.
*   user_cb         - callback to be registered against this interrupt type
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
PUBLIC PMC_ERROR digi_mapper_int_chnl_register_cb(digi_handle_t      *digi_handle,
                                                  void               *token,
                                                  UINT32              token_size,
                                                  digi_int_cb_fcn     user_cb)
{
    PMC_ERROR            result = PMC_SUCCESS;
    digi_int_type_t      int_type = DIGI_INT_CHNL_TYPE_OTN_MAPPER;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    
    if (PMC_SUCCESS == result)
    {
        result = digi_int_callback_set(digi_handle,
                                       int_type,
                                       token,
                                       token_size,
                                       user_cb);
    }

   
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_chnl_register_cb */

/*******************************************************************************
* digi_mapper_int_fo2_chnl_register_cb
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for mapper FO@ channel interrupt table processing.
*
*   For each mapper channel where an active interrupt is found, the callback will be
*   called.
*
*   The callback is called a minimum of zero times if no active interrupts are
*   found, or a maximum of the number of channels in the subsystem if there is
*   an active interrupt in each of the channels.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   token           - user token to be stored and passed back to callback\n
*                     Optional, can be NULL.
*   token_size      - token size in bytes\n
*                     Optional, can be 0.
*   user_cb         - callback to be registered against this interrupt type
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
PUBLIC PMC_ERROR digi_mapper_int_fo2_chnl_register_cb(digi_handle_t      *digi_handle,
                                                      void               *token,
                                                      UINT32              token_size,
                                                      digi_int_cb_fcn     user_cb)
{
    PMC_ERROR            result = PMC_SUCCESS;
    digi_int_type_t      int_type = DIGI_INT_CHNL_TYPE_OTN_MAPPER_FO2;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    
    if (PMC_SUCCESS == result)
    {
        result = digi_int_callback_set(digi_handle,
                                       int_type,
                                       token,
                                       token_size,
                                       user_cb);
    }

   
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_fo2_chnl_register_cb */

/*******************************************************************************
* digi_mapper_int_enet_etrans_chnl_register_cb
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for mapper ENET/ETRANS (non-enet_pmon) channel interrupt table processing.
*
*   For each mapper channel where an active interrupt is found, the callback will be
*   called.
*
*   The callback is called a minimum of zero times if no active interrupts are
*   found, or a maximum of the number of channels in the subsystem if there is
*   an active interrupt in each of the channels.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   token           - user token to be stored and passed back to callback\n
*                     Optional, can be NULL.
*   token_size      - token size in bytes\n
*                     Optional, can be 0.
*   user_cb         - callback to be registered against this interrupt type
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
PUBLIC PMC_ERROR digi_mapper_int_enet_etrans_chnl_register_cb(digi_handle_t      *digi_handle,
                                                              void               *token,
                                                              UINT32              token_size,
                                                              digi_int_cb_fcn     user_cb)
{
    PMC_ERROR            result = PMC_SUCCESS;
    digi_int_type_t      int_type = DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET_ETRANS;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    
    if (PMC_SUCCESS == result)
    {
        result = digi_int_callback_set(digi_handle,
                                       int_type,
                                       token,
                                       token_size,
                                       user_cb);
    }

   
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_enet_etrans_chnl_register_cb */

/*******************************************************************************
* digi_mapper_int_enet_pmon_chnl_register_cb
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for mapper ENET PMON channel interrupt table processing.
*
*   For each mapper channel having a ENET PMON where an active interrupt is found,
*   the callback will be called.
*
*   The callback is called a minimum of zero times if no active interrupts are
*   found, or a maximum of the number of channels in the subsystem if there is
*   an active interrupt in each of the channels.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   token           - user token to be stored and passed back to callback\n
*                     Optional, can be NULL.
*   token_size      - token size in bytes\n
*                     Optional, can be 0.
*   user_cb         - callback to be registered against this interrupt type
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
PUBLIC PMC_ERROR digi_mapper_int_enet_pmon_chnl_register_cb(digi_handle_t      *digi_handle,
                                                            void               *token,
                                                            UINT32              token_size,
                                                            digi_int_cb_fcn     user_cb)
{
    PMC_ERROR            result = PMC_SUCCESS;
    digi_int_type_t      int_type = DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET_PMON;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    
    if (PMC_SUCCESS == result)
    {
        result = digi_int_callback_set(digi_handle,
                                       int_type,
                                       token,
                                       token_size,
                                       user_cb);
    }

   
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_enet_pmon_chnl_register_cb */

/*******************************************************************************
*  digi_mapper_int_mapotn_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   int_table_ptr   - pointer to interrupt table with bits set to enable
*                     interrupts. If NULL, enables/disables all interrupts
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
PUBLIC PMC_ERROR digi_mapper_int_mapotn_enable(digi_handle_t        *digi_handle,
                                               mapotn_int_t         *int_table_ptr)
{
    PMC_ERROR              result = PMC_SUCCESS;
    digi_int_type_t        int_type = DIGI_INT_TYPE_OTN_MAPPER;

    mapotn_handle_t       *mapotn_handle;
    digi_mapper_int_t     *int_en_table_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    mapotn_handle = digi_handle->mapotn_handle;


    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == mapotn_start_state_test(digi_handle->mapotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
    }


    /* Callback for interrupt table must be registered before enable ints */
    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_OTN_MAPPER) &&
           !digi_int_is_cb_registered(digi_handle, DIGI_INT_CHNL_TYPE_OTN_MAPPER))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
    }


    /* Get internal enabled aggregate interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }

    if (PMC_SUCCESS == result)
    {
        /* Enable interrupts */
        result = mapotn_int_enable(mapotn_handle, 
                                   int_table_ptr,
                                   &int_en_table_ptr->mapotn,
                                   DIGI_ENABLE);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_mapotn_enable */



/*******************************************************************************
* digi_mapper_int_mapotn_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disables interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   int_table_ptr   - pointer to interrupt table with bits set to disable
*                     interrupts. If NULL, enables/disables all interrupts
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
PUBLIC PMC_ERROR digi_mapper_int_mapotn_disable(digi_handle_t        *digi_handle,
                                                mapotn_int_t         *int_table_ptr)
{
    PMC_ERROR              result = PMC_SUCCESS;
    digi_int_type_t        int_type = DIGI_INT_TYPE_OTN_MAPPER;

    mapotn_handle_t       *mapotn_handle;
    digi_mapper_int_t     *int_en_table_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    mapotn_handle = digi_handle->mapotn_handle;


    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == mapotn_start_state_test(digi_handle->mapotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
    }

   
    /* Get internal enabled aggregate interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }

    if (PMC_SUCCESS == result)
    {
        /* Disable interrupts */
        result = mapotn_int_enable(mapotn_handle, 
                                   int_table_ptr,
                                   &int_en_table_ptr->mapotn,
                                   DIGI_DISABLE);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_mapotn_disable */



/*******************************************************************************
* digi_mapper_int_fo2_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   int_table_ptr   - pointer to interrupt table with bits set to enable
*                     interrupts. If NULL, enables/disables all interrupts
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
PUBLIC PMC_ERROR digi_mapper_int_fo2_enable(digi_handle_t             *digi_handle,
                                            coreotn_int_framer_t      *int_table_ptr)
{
    PMC_ERROR              result = PMC_SUCCESS;
    digi_int_type_t        int_type = DIGI_INT_TYPE_OTN_MAPPER;

    mapotn_handle_t       *mapotn_handle;
    coreotn_handle_t      *coreotn_handle;
    digi_mapper_int_t     *int_en_table_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    mapotn_handle = digi_handle->mapotn_handle;
    coreotn_handle = digi_handle->coreotn_handle;


    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == coreotn_stg4_start_state_test(digi_handle->coreotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
    }


    /* Callback for interrupt table must be registered before enable ints */
    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_OTN_MAPPER) &&
           !digi_int_is_cb_registered(digi_handle, DIGI_INT_CHNL_TYPE_OTN_MAPPER) &&
           !digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_OTN))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
    }


    /* Get internal enabled aggregate interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }

    if (PMC_SUCCESS == result)
    {
        /* Enable interrupts */
        result = coreotn_fo2_int_enable(coreotn_handle, 
                                        int_table_ptr,
                                        &int_en_table_ptr->fo2,
                                        DIGI_ENABLE);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_fo2_enable */



/*******************************************************************************
* digi_mapper_int_fo2_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disables interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   int_table_ptr   - pointer to interrupt table with bits set to disable
*                     interrupts. If NULL, enables/disables all interrupts
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
PUBLIC PMC_ERROR digi_mapper_int_fo2_disable(digi_handle_t             *digi_handle,
                                             coreotn_int_framer_t      *int_table_ptr)
{
    PMC_ERROR              result = PMC_SUCCESS;
    digi_int_type_t        int_type = DIGI_INT_TYPE_OTN_MAPPER;

    mapotn_handle_t       *mapotn_handle;
    coreotn_handle_t      *coreotn_handle;
    digi_mapper_int_t     *int_en_table_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    mapotn_handle = digi_handle->mapotn_handle;
    coreotn_handle = digi_handle->coreotn_handle;


    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == coreotn_stg4_start_state_test(digi_handle->coreotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
    }


    /* Get internal enabled aggregate interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }

    if (PMC_SUCCESS == result)
    {
        /* Disable interrupts */
        result = coreotn_fo2_int_enable(coreotn_handle, 
                                        int_table_ptr,
                                        &int_en_table_ptr->fo2,
                                        DIGI_DISABLE);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_fo2_disable */



/*******************************************************************************
* digi_mapper_int_mapotn_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with unchannelized interrupt bits
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
*   digi_otn_odu_int_start(COREOTN_LATCH_DEFECT_STG4 ) must be used before
*   calling this function to retrieve the latest FO2 status.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_int_mapotn_filt_retrieve(digi_handle_t     *digi_handle,
                                                      mapotn_int_t      *filt_table_ptr,
                                                      mapotn_int_t      *int_table_ptr)
{
    PMC_ERROR         result = PMC_SUCCESS;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(int_table_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == mapotn_start_state_test(digi_handle->mapotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }

    PMC_MEMSET(int_table_ptr, 0, sizeof(mapotn_int_t));


    /* Retrieve unchannelized interrupts for MAPOTN */
    if (PMC_SUCCESS == result)
    {
        result = mapotn_int_retrieve(digi_handle->mapotn_handle, 
                                     filt_table_ptr,
                                     int_table_ptr);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_mapotn_filt_retrieve */



/*******************************************************************************
* digi_mapper_int_fo2_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with unchannelized interrupt bits
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
*   digi_otn_odu_int_start(COREOTN_LATCH_DEFECT_STG4) needs to be called before
*   retrieving RFRM status.
*
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_int_fo2_filt_retrieve(digi_handle_t           *digi_handle,
                                                   coreotn_int_framer_t    *filt_table_ptr,   
                                                   coreotn_int_framer_t    *int_table_ptr)
{
    PMC_ERROR         result = PMC_SUCCESS;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(int_table_ptr != NULL, DIGI_ERR_INVALID_ARG, 0, 0);

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == coreotn_stg4_start_state_test(digi_handle->coreotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
    }

    PMC_MEMSET(int_table_ptr, 0, sizeof(coreotn_int_framer_t));

    /* Retrieve unchannelized interrupts for OTN FO2 (stage 4) */
    if (PMC_SUCCESS == result)
    {
        result = coreotn_fo2_int_retrieve(digi_handle->coreotn_handle, 
                                          filt_table_ptr,
                                          int_table_ptr);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_fo2_filt_retrieve */


/*******************************************************************************
* digi_mapper_int_mapotn_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the aggregated 
*   interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   int_table_ptr   - pointer to aggregated interrupt table with bits set to
*                     clear interrupts.
*                     DIGI_INT_ALL_INTS to clear all interrupts.
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
PUBLIC PMC_ERROR digi_mapper_int_mapotn_clear(digi_handle_t      *digi_handle,
                                              mapotn_int_t       *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;

    mapotn_handle_t    *mapotn_handle;
    digi_mapper_int_t  *int_en_table_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    mapotn_handle = digi_handle->mapotn_handle;

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == mapotn_start_state_test(mapotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }


    /* Get internal enabled interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       DIGI_INT_TYPE_OTN_MAPPER,
                                       (void**) &int_en_table_ptr);

    }

    /* Clear unchannelized interrupts for MAPOTN */
    if (PMC_SUCCESS == result)
    {
        result = mapotn_int_clear(mapotn_handle,
                                  int_table_ptr,
                                  &int_en_table_ptr->mapotn);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_mapotn_clear */



/*******************************************************************************
* digi_mapper_int_fo2_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the aggregated 
*   interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   int_table_ptr   - pointer to aggregated interrupt table with bits set to
*                     clear interrupts.
*                     DIGI_INT_ALL_INTS to clear all interrupts.
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
PUBLIC PMC_ERROR digi_mapper_int_fo2_clear(digi_handle_t           *digi_handle,
                                           coreotn_int_framer_t    *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;

    mapotn_handle_t    *mapotn_handle;
    digi_mapper_int_t  *int_en_table_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    mapotn_handle = digi_handle->mapotn_handle;

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == mapotn_start_state_test(mapotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }


    /* Get internal enabled interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       DIGI_INT_TYPE_OTN_MAPPER,
                                       (void**) &int_en_table_ptr);

    }

    /* Clear unchannelized interrupts for OTN FO2 (stage 4) */
    if (PMC_SUCCESS == result)
    {
        result = coreotn_fo2_int_clear(digi_handle->coreotn_handle,
                                       int_table_ptr,
                                       &int_en_table_ptr->fo2);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_fo2_clear */


/*******************************************************************************
* digi_mapper_int_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     enable interrupts
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
PUBLIC PMC_ERROR digi_mapper_int_chnl_enable(digi_handle_t             *digi_handle,
                                             digi_mapper_chnl_t        *chnl_handle,   
                                             mapotn_int_chnl_t         *int_table_ptr)
{
    PMC_ERROR              result = PMC_SUCCESS;
    digi_int_type_t        int_type = DIGI_INT_TYPE_OTN_MAPPER;

    mapotn_handle_t       *mapotn_handle;
    UINT32                 start_link = 0;
    UINT32                 end_link = 0;
    UINT32                 i;
    digi_mapper_int_t     *int_en_table_ptr;
        
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 
        
    mapotn_handle = digi_handle->mapotn_handle;


    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == mapotn_start_state_test(digi_handle->mapotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
    }


    /* Callback for interrupt table must be registered before enable ints */
    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_OTN_MAPPER) &&
           !digi_int_is_cb_registered(digi_handle, DIGI_INT_CHNL_TYPE_OTN_MAPPER))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
    }

    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         (void*) chnl_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }

    /* Get internal enabled aggregate interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }

    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        /* Enable interrupts */
        result = mapotn_int_chnl_enable(mapotn_handle, 
                                        i,
                                        int_table_ptr,
                                        &int_en_table_ptr->mapper_chnl[i].status,
                                        DIGI_ENABLE);
        PMC_ATOMIC_YIELD(digi_handle,0);                                
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_chnl_enable */

/*******************************************************************************
* digi_mapper_int_chnl_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to disable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to disable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     disable interrupts
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
PUBLIC PMC_ERROR digi_mapper_int_chnl_disable(digi_handle_t             *digi_handle,
                                              digi_mapper_chnl_t        *chnl_handle,   
                                              mapotn_int_chnl_t         *int_table_ptr)
{
    PMC_ERROR              result = PMC_SUCCESS;
    digi_int_type_t        int_type = DIGI_INT_TYPE_OTN_MAPPER;

    mapotn_handle_t       *mapotn_handle;
    UINT32                 start_link = 0;
    UINT32                 end_link = 0;
    UINT32                 i;
    digi_mapper_int_t     *int_en_table_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 
        
    mapotn_handle = digi_handle->mapotn_handle;

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == mapotn_start_state_test(digi_handle->mapotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
    }


    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         (void*) chnl_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }


    /* Get internal enabled aggregate interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }


    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        /* Enable interrupts */
        result = mapotn_int_chnl_enable(mapotn_handle, 
                                        i,
                                        int_table_ptr,
                                        &int_en_table_ptr->mapper_chnl[i].status,
                                        DIGI_DISABLE);
        PMC_ATOMIC_YIELD(digi_handle,0);                                
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_chnl_disable */




/*******************************************************************************
* digi_mapper_int_fo2_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables OTN FO2 interrupts based on bits that are set in the channel
*   interrupt table. When an interrupt is detected the callback associated
*   with OTN will be called.
*
*   Note: RFRM interrupts must be enabled using digi_otn_odu_int_defect_enable()
*         and digi_otn_odu_int_pm_chnl_enable(). See the ODU RFRM interrupt
*         table odu_rfrm_int_chnl_t for a list of affected interrupts.
*
*         Individual RFRM defects are enabled across all channels and PIDs in a
*         given framer. Similarly, a channel or PID will be enabled for all
*         RFRM defects across a framer. 
*
*         For example, the API digi_otn_odu_int_defect_enable() can be used to
*         enable the DAIS defect in the mid-order framer. Any enabled channel
*         or PID in the mid-order framer that is in the DAIS state will then
*         cause an interrupt. If a channel or PID is disabled, then no defects
*         on that channel or PID will cause an interrupt.
*
*         Non-RFRM interrupts behave normally.
*
*
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     in the Stage 4 ODU level
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     enable interrupts
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
PUBLIC PMC_ERROR digi_mapper_int_fo2_chnl_enable(digi_handle_t              *digi_handle,
                                                 digi_mapper_chnl_t         *chnl_handle,
                                                 coreotn_int_framer_chnl_t  *int_table_ptr)
{      
    PMC_ERROR                   result = PMC_SUCCESS;
    digi_int_type_t             int_type = DIGI_INT_CHNL_TYPE_OTN_MAPPER_FO2;
    UINT32                      start_chnl = 0;
    UINT32                      end_chnl = 0;
    UINT32                      i;
    coreotn_int_chnl_info_t     chnl_info;
    coreotn_handle_t           *coreotn_handle;
    coreotn_int_framer_chnl_t  *framer_en_ptr;
    digi_otn_odu_chnl_t        *odu_handle = NULL;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == coreotn_stg4_start_state_test(digi_handle->coreotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
    }

    /* Callback for interrupt table must be registered before enable ints */
    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_OTN_MAPPER) && 
           !digi_int_is_cb_registered(digi_handle, DIGI_INT_CHNL_TYPE_OTN_MAPPER) &&
           !digi_int_is_cb_registered(digi_handle, DIGI_INT_CHNL_TYPE_OTN_MAPPER_FO2))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
    }

    /* get odu handle */
    if ((PMC_SUCCESS == result) &&
        (!IS_DIGI_INT_ALL_CHNLS(chnl_handle)))
    {
        result = digi_otn_get_chnl_ptr_for_tcm_apps_internal(digi_handle, 
                                                             chnl_handle,
                                                             &odu_handle);
    }

    /* Determine if we are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        /* Get range of channels to retrieve within ODU level */
        result = digi_otn_odu_int_link_range_get(digi_handle,
                                                 odu_handle,
                                                 ODU_STRUCT_LEVEL_4_ODU,
                                                 &start_chnl,
                                                 &end_chnl);
    }
    

    /* Get specific ODU level framer enabled interrupt table */
    if (PMC_SUCCESS == result)
    {
        result = digi_otn_odu_int_en_framer_get(digi_handle,
                                                ODU_STRUCT_LEVEL_4_ODU,
                                                &framer_en_ptr);
    }

    /* Enable interrupts */
    coreotn_handle      = digi_handle->coreotn_handle;
    chnl_info.odu_level = ODU_STRUCT_LEVEL_4_ODU;
    chnl_info.source    = COREOTN_INT_SOURCE_LINE; 

    

    for (i = start_chnl; (i < end_chnl) && (PMC_SUCCESS == result); i++)
    {
        chnl_info.chnl_id = i;
        result = coreotn_int_framer_chnl_enable(coreotn_handle, 
                                                &chnl_info,
                                                int_table_ptr,
                                                &framer_en_ptr[i],
                                                DIGI_ENABLE);
    }

    
    if (PMC_SUCCESS == result)
    {
        result = digi_otn_odu_int_framer_chnl_ddeg_enable(digi_handle,
                                                          ODU_STRUCT_LEVEL_4_ODU,
                                                          chnl_info.source,
                                                          start_chnl,
                                                          end_chnl,
                                                          int_table_ptr,
                                                          framer_en_ptr,
                                                          DIGI_ENABLE);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_fo2_chnl_enable */

/*******************************************************************************
* digi_mapper_int_fo2_chnl_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disables OTN FO2 interrupts based on bits that are set in the channel
*   interrupt table. 
*
*   Note: RFRM interrupts must be enabled using digi_otn_odu_int_defect_enable()
*         and digi_otn_odu_int_pm_chnl_enable(). See the ODU RFRM interrupt
*         table odu_rfrm_int_chnl_t for a list of affected interrupts.
*
*         Individual RFRM defects are enabled across all channels and PIDs in a
*         given framer. Similarly, a channel or PID will be enabled for all
*         RFRM defects across a framer. 
*
*         For example, the API digi_otn_odu_int_defect_enable() can be used to
*         enable the DAIS defect in the mid-order framer. Any enabled channel
*         or PID in the mid-order framer that is in the DAIS state will then
*         cause an interrupt. If a channel or PID is disabled, then no defects
*         on that channel or PID will cause an interrupt.
*
*         Non-RFRM interrupts behave normally.
*
*
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to disable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to disable interrupts on all channels
*                     in the Stage 4 ODU level
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     disable interrupts
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
PUBLIC PMC_ERROR digi_mapper_int_fo2_chnl_disable(digi_handle_t              *digi_handle,
                                                  digi_mapper_chnl_t         *chnl_handle,
                                                  coreotn_int_framer_chnl_t  *int_table_ptr)
{      
    PMC_ERROR                   result = PMC_SUCCESS;
    digi_int_type_t             int_type = DIGI_INT_CHNL_TYPE_OTN_MAPPER_FO2;
    UINT32                      start_chnl = 0;
    UINT32                      end_chnl = 0;
    UINT32                      i;
    coreotn_int_chnl_info_t     chnl_info;
    coreotn_handle_t           *coreotn_handle;
    coreotn_int_framer_chnl_t  *framer_en_ptr;
    digi_otn_odu_chnl_t        *odu_handle = NULL;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == coreotn_stg4_start_state_test(digi_handle->coreotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
    }

    /* get odu handle */
    if ((PMC_SUCCESS == result) &&
        (!IS_DIGI_INT_ALL_CHNLS(chnl_handle)))
    {
        result = digi_otn_get_chnl_ptr_for_tcm_apps_internal(digi_handle, 
                                                             chnl_handle,
                                                             &odu_handle);
    }
    
    /* Determine if we are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        /* Get range of channels to retrieve within ODU level */
        result = digi_otn_odu_int_link_range_get(digi_handle,
                                                 odu_handle,
                                                 ODU_STRUCT_LEVEL_4_ODU,
                                                 &start_chnl,
                                                 &end_chnl);
    }

    
    /* Get specific ODU level framer enabled interrupt table */
    if (PMC_SUCCESS == result)
    {
        result = digi_otn_odu_int_en_framer_get(digi_handle,
                                                ODU_STRUCT_LEVEL_4_ODU,
                                                &framer_en_ptr);
    }


    /* Enable interrupts */
    coreotn_handle      = digi_handle->coreotn_handle;
    chnl_info.odu_level = ODU_STRUCT_LEVEL_4_ODU;
    chnl_info.source    = COREOTN_INT_SOURCE_LINE; 

    

    for (i = start_chnl; (i < end_chnl) && (PMC_SUCCESS == result); i++)
    {
        chnl_info.chnl_id = i;
        result = coreotn_int_framer_chnl_enable(coreotn_handle, 
                                                &chnl_info,
                                                int_table_ptr,
                                                &framer_en_ptr[i],
                                                DIGI_DISABLE);
    }
    
    
    if (PMC_SUCCESS == result)
    {
        result = digi_otn_odu_int_framer_chnl_ddeg_enable(digi_handle,
                                                          ODU_STRUCT_LEVEL_4_ODU,
                                                          chnl_info.source,
                                                          start_chnl,
                                                          end_chnl,
                                                          int_table_ptr,
                                                          framer_en_ptr,
                                                          DIGI_DISABLE);
    }


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_fo2_chnl_disable */

/*******************************************************************************
* digi_mapper_int_fo2_chnl_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the FO2 ODU channel interrupt table corresponding to a mapper chnl.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
*   digi_otn_odu_int_start(COREOTN_LATCH_DEFECT_STG4) or 
*   digi_otn_odu_int_start(COREOTN_LATCH_DEFECT_ALL) needs to be called before
*   retrieving interrupts to trigger RFRM status transfer.
*
*   digi_otn_ddeg_int_start() must be used before calling this function to
*   retrieve the latest dDEG status.
*   
*
* INPUTS:
*   *digi_handle          - pointer to DIGI handle instance
*   chnl_handle           - pointer to channel handle to retrieve interrupts
*   framer_filt_table_ptr - pointer to filter table for ODU framer status.
*                           Mark fields as TRUE within this table to indicate that
*                           the interrupt/status will be retrieved
*                           to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                           interrupt/status.
*   pid_filt_table_ptr    - pointer to filter table for ODU PID status.
*                           Mark fields as TRUE within this table to indicate that
*                           the interrupt/status will be retrieved
*                           to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                           interrupt/status.
*
* OUTPUTS:
*   int_table_ptr  - pointer to channel interrupt table to store interrupt
*                    information. 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_int_fo2_chnl_filt_retrieve(digi_handle_t              *digi_handle,
                                                        digi_mapper_chnl_t         *chnl_handle,
                                                        coreotn_int_framer_chnl_t  *framer_filt_table_ptr,
                                                        odu_rfrm_int_pid_chnl_t    *pid_filt_table_ptr,
                                                        digi_otn_odu_int_chnl_t    *int_table_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;
    digi_otn_odu_chnl_t    *odu_handle = NULL;

    PMC_ATOMIC_ENTRY(digi_handle);
        
        
    result = digi_mapper_chnl_handle_validate(digi_handle, chnl_handle);

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == coreotn_stg4_start_state_test(digi_handle->coreotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }


    /* Get ODU handle from mapper handle */
    if (PMC_SUCCESS == result)
    {
        result = digi_otn_get_chnl_ptr_for_tcm_apps_internal(digi_handle, 
                                                             chnl_handle,
                                                             &odu_handle);
    }

    /* Retrieve Stage 4 interrupts */
    if (PMC_SUCCESS == result)
    {
        result = digi_otn_odu_int_chnl_filt_retrieve(digi_handle,
                                                     odu_handle,
                                                     framer_filt_table_ptr,
                                                     pid_filt_table_ptr,
                                                     int_table_ptr);
    }


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_fo2_chnl_filt_retrieve */

/*******************************************************************************
* digi_mapper_enet_etrans_int_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables channel(s) ENET/ETRANS interrupts based on bits that are set in the 
*   given interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel ENET interrupt table with bits set to
*                     enable interrupts
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*  To enable mapper ENET PMON  interrupts use digi_mapper_enet_pmon_int_chnl_enable().
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_enet_etrans_int_chnl_enable(digi_handle_t             *digi_handle,
                                                         digi_mapper_chnl_t        *chnl_handle,   
                                                         enet_int_chnl_t           *int_table_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    result = digi_mapper_enet_int_chnl_cfg(digi_handle,
                                           chnl_handle,
                                           int_table_ptr,
                                           FALSE,
                                           FALSE,
                                           TRUE);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_enet_etrans_int_chnl_enable */

/*******************************************************************************
* digi_mapper_enet_etrans_int_chnl_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables channel(s) ENET/ETRANS interrupts based on bits that are set in the 
*   given interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to disable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to disable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel ENET interrupt table with bits set to
*                     disable interrupts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*  To disable mapper ENET PMON  interrupts use digi_mapper_enet_pmon_int_chnl_disable().
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_enet_etrans_int_chnl_disable(digi_handle_t          *digi_handle,
                                                          digi_mapper_chnl_t     *chnl_handle,   
                                                          enet_int_chnl_t        *int_table_ptr)
{
    PMC_ERROR              result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    result = digi_mapper_enet_int_chnl_cfg(digi_handle,
                                           chnl_handle,
                                           int_table_ptr,
                                           FALSE,
                                           FALSE,
                                           FALSE);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_enet_etrans_int_chnl_disable */

/*******************************************************************************
* digi_mapper_enet_etrans_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clear channel(s) ENET/ETRANS interrupts based on bits that are set in the 
*   given interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to clear interrupts on, or
*                     DIGI_INT_ALL_CHNLS to clear interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel ENET interrupt table with bits set to
*                     clear interrupts
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*  To enable mapper ENET PMON  interrupts use digi_mapper_enet_pmon_int_chnl_clear().
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_enet_etrans_int_chnl_clear(digi_handle_t             *digi_handle,
                                                        digi_mapper_chnl_t        *chnl_handle,   
                                                        enet_int_chnl_t           *int_table_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    result = digi_mapper_enet_int_chnl_cfg(digi_handle,
                                           chnl_handle,
                                           int_table_ptr,
                                           FALSE,
                                           TRUE,
                                           FALSE);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_enet_etrans_int_chnl_clear */

/*******************************************************************************
* digi_mapper_enet_pmon_int_chnl_enable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the mapper channel
*   ENET PMON interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel ENET interrupt table with bits set to
*                     enable interrupts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*  To enable mapper ENET ETRANS interrupts use digi_mapper_enet_etrans_int_chnl_enable().
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_enet_pmon_int_chnl_enable(digi_handle_t             *digi_handle,
                                                       digi_mapper_chnl_t        *chnl_handle,
                                                       enet_int_chnl_t           *int_table_ptr)
{
    PMC_ERROR              result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    result = digi_mapper_enet_int_chnl_cfg(digi_handle,
                                           chnl_handle,
                                           int_table_ptr,
                                           TRUE,
                                           FALSE,
                                           TRUE);
            
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_enet_pmon_int_chnl_enable */

/*******************************************************************************
* digi_mapper_enet_pmon_int_chnl_disable
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disables interrupts based on bits that are set in the mapper channel ENET PMON
*   interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to disable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to disable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel ENET interrupt table with bits set to
*                     disable interrupts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*  To disable mapper ENET ETRANS interrupts use digi_mapper_enet_etrans_int_chnl_disable().
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_enet_pmon_int_chnl_disable(digi_handle_t             *digi_handle,
                                                        digi_mapper_chnl_t        *chnl_handle,
                                                        enet_int_chnl_t           *int_table_ptr)
{
    PMC_ERROR              result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    result = digi_mapper_enet_int_chnl_cfg(digi_handle,
                                           chnl_handle,
                                           int_table_ptr,
                                           TRUE,
                                           FALSE,
                                           FALSE);
            
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_enet_pmon_int_chnl_disable */

/*******************************************************************************
* digi_mapper_enet_pmon_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the mapper channel ENET PMON
*   interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to clear interrupts on, or
*                     DIGI_INT_ALL_CHNLS to clear interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel ENET interrupt table with bits set to
*                     disable interrupts
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*  To disable mapper ENET ETRANS interrupts use digi_mapper_enet_etrans_int_chnl_clear().
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_enet_pmon_int_chnl_clear(digi_handle_t             *digi_handle,
                                                      digi_mapper_chnl_t        *chnl_handle,
                                                      enet_int_chnl_t           *int_table_ptr)
{
    PMC_ERROR              result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    result = digi_mapper_enet_int_chnl_cfg(digi_handle,
                                           chnl_handle,
                                           int_table_ptr,
                                           TRUE,
                                           TRUE,
                                           FALSE);
            
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_enet_pmon_int_chnl_clear */

/*******************************************************************************
* digi_mapper_int_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the aggregated 
*   interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   int_table_ptr   - pointer to aggregated interrupt table with bits set to
*                     clear interrupts.\n
*                     DIGI_INT_ALL_INTS to clear all interrupts.
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
PUBLIC PMC_ERROR digi_mapper_int_clear(digi_handle_t             *digi_handle,
                                       digi_mapper_int_t         *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_CHNL_TYPE_OTN_MAPPER;

    digi_mapper_chnl_t *chnl_handle;
    mapotn_handle_t    *mapotn_handle;
    cpb_handle_t       *cpb_handle;
    UINT32              start_link = 0;
    UINT32              end_link = 0;
    UINT32              i;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    mapotn_handle = digi_handle->mapotn_handle;
    cpb_handle    = digi_handle->dcpb_handle;

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == mapotn_start_state_test(mapotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }


    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }
    

    /* Clear unchannelized interrupts for MAPOTN */
    if (PMC_SUCCESS == result)
    {
        result = digi_mapper_int_mapotn_clear(digi_handle,
                                              &int_table_ptr->mapotn);
    }

    /* Clear unchannelized interrupts for OTN FO2 (stage 4) */
    if (PMC_SUCCESS == result)
    {
        result = digi_mapper_int_fo2_clear(digi_handle,
                                           UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, fo2));
    }


    /* Clear channelized interrupts */
    if (IS_DIGI_INT_ALL_INTS(int_table_ptr))
    {
        result = digi_mapper_int_chnl_clear(digi_handle,
                                            DIGI_INT_ALL_CHNLS,
                                            DIGI_INT_ALL_INTS);
    }
    else
    {
        for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
        {
            chnl_handle = int_table_ptr->handle[i];

            if (NULL != chnl_handle)
            {
                result = digi_mapper_int_chnl_clear(digi_handle,
                                                    chnl_handle,
                                                    UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, mapper_chnl[i]));
            }
        }
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapotn_int_clear */


/*******************************************************************************
* digi_mapper_int_chnl_clear
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the aggregated 
*   interrupt table sor a given channel.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to clear interrupt on.\n
*                     DIGI_INT_ALL_CHNLS to clear interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     clear interrupts.\n
*                     DIGI_INT_ALL_INTS to clear all interrupts.
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
PUBLIC PMC_ERROR digi_mapper_int_chnl_clear(digi_handle_t          *digi_handle,
                                            digi_mapper_chnl_t     *chnl_handle,
                                            digi_mapper_int_chnl_t *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_CHNL_TYPE_OTN_MAPPER;

    mapotn_handle_t         *mapotn_handle;
    cpb_handle_t            *cpb_handle;
    coreotn_handle_t        *coreotn_handle;
    digi_otn_odu_chnl_t     *odu_handle = NULL;
    digi_mapper_int_t       *int_en_table_ptr;
    digi_mapper_chnl_def_t  *handle_pool_ptr;
    UINT32                   i;
    UINT32                   start_link = 0;
    UINT32                   end_link = 0;
    UINT32                   cpb_port;
    UINT32                   cpb_chnl;
    BOOL8                    multichannels;
    coreotn_int_chnl_info_t  chnl_info;
    enet_handle_t           *enet_ptr;
    UINT32                   link;
    UINT32                   dummy;

    PMC_ATOMIC_ENTRY(digi_handle);
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);  

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == mapotn_start_state_test(digi_handle->mapotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
    }

    handle_pool_ptr = digi_handle->handle_pool.mapper_chnl;
    mapotn_handle   = digi_handle->mapotn_handle;
    coreotn_handle  = digi_handle->coreotn_handle;
    cpb_handle      = digi_handle->dcpb_handle;
   
    /* Get link */
    if (PMC_SUCCESS == result)
    {

        result = digi_int_link_range_get(digi_handle,
                                         chnl_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }
    if (IS_DIGI_INT_ALL_CHNLS(chnl_handle))
    {
        multichannels = TRUE;
    }
    else
    {
        multichannels = FALSE;
    }


    /* Get internal enabled interrupt table to update */
    if (PMC_SUCCESS == result && NULL == int_table_ptr)
    {
        result = digi_int_en_table_get(digi_handle,
                                       DIGI_INT_TYPE_OTN_MAPPER,
                                       (void**) &int_en_table_ptr);

    }


    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        digi_mapper_int_chnl_t *chnl_table_ptr = int_table_ptr;

        if (NULL == int_table_ptr)
        {
            chnl_table_ptr = &int_en_table_ptr->mapper_chnl[i];
        }

        if (TRUE == multichannels)
        {
            chnl_handle = &handle_pool_ptr[i];
        }

        if (util_global_is_chnl_prov(chnl_handle))
        {
            /* Clear mapotn interrupts */
            if (PMC_SUCCESS == result)
            {
                result = mapotn_int_chnl_clear(mapotn_handle, 
                                               i,
                                               UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, status),
                                               &chnl_table_ptr->status);
            }

            if (PMC_SUCCESS == result)
            {
                result = digi_cpb_port_chnl_get(digi_handle, chnl_handle, &cpb_port, &cpb_chnl);    
            }

            /* Clear CPB interrupts */
            if (PMC_SUCCESS == result)
            {
                result = cpb_int_chnl_clear(cpb_handle,
                                            cpb_port,
                                            cpb_chnl,      
                                            UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cpb),
                                            &chnl_table_ptr->cpb); 
            }

            /* Get stg4 handle from mapper handle */
            if (PMC_SUCCESS == result)
            {
                result = digi_otn_get_chnl_ptr_for_tcm_apps_internal(digi_handle, 
                                                                     chnl_handle,
                                                                     &odu_handle);
            }

            if (PMC_SUCCESS == result)
            {
                result = digi_otn_odu_blk_filter_set(digi_handle,
                                                     UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, fo2.status),
                                                     &chnl_info.blk_filter);
            }

            if (PMC_SUCCESS == result)
            {
                chnl_info.chnl_id   = digi_otn_odu_chnl_num_internal_get(digi_handle, odu_handle);
                chnl_info.odu_level = ODU_STRUCT_LEVEL_4_ODU;
                chnl_info.source    = COREOTN_INT_SOURCE_LINE;
                result = digi_otn_pool_id_get(digi_handle, odu_handle, chnl_info.rx_pool_id); 
            }

            /* Clear interrupts for OTN FO2 (stage 4) */
            if (PMC_SUCCESS == result)
            {
                result = coreotn_int_chnl_clear(coreotn_handle,
                                                &chnl_info,
                                                UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, fo2.status));
            }
            
            /* clear  ENET (etrans) interrupt associated to MAPOTN channel */
            if (PMC_SUCCESS == result && chnl_table_ptr->enet_etrans_valid)
            {
                result = digi_enet_validate_parms(digi_handle,
                                                  chnl_handle,
                                                  &enet_ptr,
                                                  &link);
                if (PMC_SUCCESS == result && 
                    TRUE == enet_is_link_mapotn_test(enet_ptr, link, &dummy))
                {
                    /* Enable enet interrupts */
                    result = enet_int_chnl_clear(enet_ptr, 
                                                 link,
                                                 UTIL_GLOBAL_INT_SAFE_GET_OFFSET(chnl_table_ptr, enet_etrans),
                                                 &chnl_table_ptr->enet_etrans,
                                                 FALSE);
                }
                else
                {
                    result = PMC_SUCCESS;
                }            
            } 
            /* clear  ENET PMON interrupt associated to MAPOTN channel */
            if (PMC_SUCCESS == result &&
                chnl_table_ptr->enet_pmon_valid &&
                DIGI_NO_PMON != ((digi_mapper_chnl_def_t *)chnl_handle)->enet_pmon)
            {
                result = digi_mapper_enet_pmon_handle_get(digi_handle,
                                                          chnl_handle,
                                                          &enet_ptr,
                                                          &link);
                if (PMC_SUCCESS == result && 
                    NULL != enet_ptr)
                {
                    /* Enable enet interrupts */
                    result = enet_int_chnl_clear(enet_ptr, 
                                                 link,
                                                 UTIL_GLOBAL_INT_SAFE_GET_OFFSET(chnl_table_ptr, enet_pmon),
                                                 &chnl_table_ptr->enet_pmon,
                                                 FALSE);
                }
            }
        }
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_chnl_clear */


/*******************************************************************************
* digi_mapper_int_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits
*   over multiple channels.
*
*   digi_mapper_int_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
*   digi_otn_odu_int_start(COREOTN_LATCH_DEFECT_STG4 ) must be used before
*   calling this function to retrieve the latest FO2 status.
*
*   digi_otn_ddeg_int_start() must be used before calling this function to
*   retrieve the latest dDEG status.
*   
*
* INPUTS:
*   *digi_handle   - pointer to DIGI handle instance
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_int_retrieve(digi_handle_t       *digi_handle,
                                          digi_mapper_int_t   *int_table_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_mapper_int_filt_retrieve(digi_handle,
                                           DIGI_INT_ALL_INTS,
                                           int_table_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_retrieve */



/*******************************************************************************
* digi_mapper_int_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits
*   over multiple channels.
*
*   digi_mapper_int_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
*   digi_otn_odu_int_start(COREOTN_LATCH_DEFECT_STG4 ) must be used before
*   calling this function to retrieve the latest FO2 status.
*
*   digi_otn_ddeg_int_start() must be used before calling this function to
*   retrieve the latest dDEG status.
*
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_int_filt_retrieve(digi_handle_t             *digi_handle,
                                               digi_mapper_int_chnl_t    *filt_table_ptr,   
                                               digi_mapper_int_t         *int_table_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;
    digi_int_type_t                  int_type = DIGI_INT_TYPE_OTN_MAPPER;

    UINT32                           start_link = 0;
    UINT32                           end_link = 0;
    UINT32                           i;
    digi_mapper_chnl_def_t          *handle_pool_ptr;
    digi_mapper_chnl_t              *chnl_ptr;
    BOOL8 first = TRUE;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_int_param_check(digi_handle, DIGI_INT_ALL_CHNLS, int_type, int_table_ptr); 

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == mapotn_start_state_test(digi_handle->mapotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }

    PMC_MEMSET(int_table_ptr, 0, sizeof(digi_mapper_int_t));

    handle_pool_ptr = digi_handle->handle_pool.mapper_chnl;


    /* enable interrupt caching, clear cache */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, TRUE,TRUE);

    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }


    /* Retrieve unchannelized interrupts for MAPOTN */
    if (PMC_SUCCESS == result)
    {
        result = digi_mapper_int_mapotn_filt_retrieve(digi_handle, 
                                                      DIGI_INT_ALL_INTS,
                                                      &int_table_ptr->mapotn);
    }

    /* Retrieve unchannelized interrupts for OTN FO2 (stage 4) */
    if (PMC_SUCCESS == result)
    {
        result = digi_mapper_int_fo2_filt_retrieve(digi_handle, 
                                                   DIGI_INT_ALL_INTS,
                                                   &int_table_ptr->fo2);
    }
  
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        chnl_ptr = &handle_pool_ptr[i];

        if (util_global_is_chnl_prov(chnl_ptr))
        {
            if (TRUE == first)
            {
                mapotn_retrieve_chnl_registers(digi_handle->mapotn_handle);
                first = FALSE;
            }

            int_table_ptr->handle[i] = chnl_ptr; 

            result = digi_mapper_int_chnl_filt_retrieve(digi_handle,
                                                        chnl_ptr,
                                                        filt_table_ptr,
                                                        &int_table_ptr->mapper_chnl[i]);

           
        }   
    }

    /* disable interrupt caching */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, FALSE, FALSE);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_filt_retrieve */

/*******************************************************************************
* digi_mapper_int_chnl_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an channel interrupt table with interrupt bits.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to retrieve interrupt on
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*
* OUTPUTS:
*   int_table_ptr  - pointer to channel interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_int_chnl_filt_retrieve(digi_handle_t           *digi_handle,
                                                    digi_mapper_chnl_t      *chnl_handle,
                                                    digi_mapper_int_chnl_t  *filt_table_ptr,
                                                    digi_mapper_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;
    mapotn_handle_t                 *mapotn_handle;
    coreotn_handle_t                *coreotn_handle;
    cpb_handle_t                    *cpb_handle;
    UINT32                           start_link = 0;
    UINT32                           end_link = 0;
    digi_otn_odu_chnl_t             *odu_handle = NULL;
    coreotn_int_chnl_info_t          chnl_info;
    UINT32                           cpb_port;
    UINT32                           cpb_chnl;
    digi_int_type_t                  int_type = DIGI_INT_CHNL_TYPE_OTN_MAPPER;
    odu_struct_t                    *stg4_handle_pool_ptr;

    PMC_ATOMIC_ENTRY(digi_handle);
   
    mapotn_handle        = digi_handle->mapotn_handle;
    coreotn_handle       = digi_handle->coreotn_handle;
    cpb_handle           = digi_handle->dcpb_handle;
    stg4_handle_pool_ptr = digi_handle->handle_pool.s4_odu_struct;


    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 
   
    if ((PMC_SUCCESS == result) &&
        (NULL == chnl_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,DIGI_ERR_NULL_HANDLE);
    }

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == mapotn_start_state_test(digi_handle->mapotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }


    /* Get link */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         chnl_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }

    /* Retrieve status for enabled interrupts */
    if (PMC_SUCCESS == result)
    {
        result = mapotn_int_chnl_retrieve(mapotn_handle, 
                                          start_link,
                                          UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, status),
                                          &int_table_ptr->status);
    }
           
    /* Get stg4 handle from mapper handle */
    if (PMC_SUCCESS == result)
    {
        result = digi_otn_get_chnl_ptr_for_tcm_apps_internal(digi_handle, 
                                                             chnl_handle,
                                                             &odu_handle);
    }
    
    
    if (PMC_SUCCESS == result)
    {
        result = digi_otn_odu_blk_filter_set(digi_handle,
                                             UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, fo2.status),
                                             &chnl_info.blk_filter);
    }


    if (PMC_SUCCESS == result)
    {
        chnl_info.chnl_id   = digi_otn_odu_chnl_num_internal_get(digi_handle, odu_handle);
        chnl_info.odu_level = ODU_STRUCT_LEVEL_4_ODU;
        chnl_info.source    = COREOTN_INT_SOURCE_LINE;
        result = digi_otn_pool_id_get(digi_handle, odu_handle, chnl_info.rx_pool_id); 
    }
   

    /* Retrieve status for OTN FO2 (stage 4) */
    if (PMC_SUCCESS == result)
    {
        result = coreotn_int_chnl_retrieve(coreotn_handle,
                                           &chnl_info,
                                           UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, fo2.status),
                                           &int_table_ptr->fo2.status);
    }

    if (PMC_SUCCESS == result)
    {
        result = digi_otn_odu_int_ddeg_stage_chnl_retrieve(digi_handle, 
                                                           &stg4_handle_pool_ptr[chnl_info.chnl_id],
                                                           COREOTN_DDEG_MON_CLIENT,
                                                           NULL, /* unused */
                                                           &int_table_ptr->fo2.status);
    }


    if (PMC_SUCCESS == result)
    {
        result = digi_cpb_port_chnl_get(digi_handle, chnl_handle, &cpb_port, &cpb_chnl);    
    }
    
    /* Retrieve CPB interrupts */
    if (PMC_SUCCESS == result)
    {
        result = cpb_int_chnl_retrieve(cpb_handle,
                                       cpb_port,
                                       cpb_chnl,      
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cpb),
                                       &int_table_ptr->cpb);
    }

    /* retrieve ENET (etrans) interrupt associated to mapper channel */
    if (PMC_SUCCESS == result &&
        (NULL == filt_table_ptr ||
         TRUE == filt_table_ptr->enet_etrans_valid))
    {
        result = digi_mapper_enet_etrans_int_chnl_filt_retrieve(digi_handle,
                                                                chnl_handle,
                                                                UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr,enet_etrans),
                                                                &int_table_ptr->enet_etrans);
        if (PMC_SUCCESS == result)
        {
            int_table_ptr->enet_etrans_valid = TRUE;
        }
    } 
    /* retrieve ENET PMON interrupt associated to mapper channel */
    if (PMC_SUCCESS == result &&
        (NULL == filt_table_ptr ||
         TRUE == filt_table_ptr->enet_pmon_valid))
    {
        result = digi_mapper_enet_pmon_int_chnl_filt_retrieve(digi_handle,
                                                              chnl_handle,
                                                              UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr,enet_pmon),
                                                              &int_table_ptr->enet_pmon);
        if (PMC_SUCCESS == result)
        {
            int_table_ptr->enet_pmon_valid = TRUE;
        }
    }
    
    /* read channels opu_csf_v from firmware */
    if (PMC_SUCCESS == result)
    {
        result = digi_mapper_int_chnl_fw_opu_csf_retrieve(digi_handle,
                                                          0,
                                                          start_link,
                                                          end_link,
                                                          filt_table_ptr,
                                                          int_table_ptr);
    }    

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_chnl_filt_retrieve */

/*******************************************************************************
* digi_mapper_enet_etrans_int_chnl_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves a mapper channel ENET ETRANS interrupt table with interrupt bits.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
*
* INPUTS:
*   *digi_handle         - pointer to DIGI handle instance
*   chnl_handle          - pointer to channel handle to retrieve interrupt on
*   filt_table_ptr       - pointer to filter table. Mark fields as TRUE within this
*                          table to indicate that the interrupt/status will be retrieved
*                          to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                          interrupt/status.
*
* OUTPUTS:
*   int_table_ptr  - pointer to channel interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_enet_etrans_int_chnl_filt_retrieve(digi_handle_t           *digi_handle,
                                                                digi_mapper_chnl_t      *chnl_handle,
                                                                enet_int_chnl_t         *filt_table_ptr,
                                                                enet_int_chnl_t         *int_table_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    result = digi_mapper_enet_int_chnl_filt_retrieve_internal(digi_handle,
                                                              chnl_handle,
                                                              FALSE,
                                                              filt_table_ptr,
                                                              int_table_ptr);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_enet_etrans_int_chnl_filt_retrieve */

/*******************************************************************************
* digi_mapper_enet_pmon_int_chnl_filt_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves a mapper channel ENET PMON interrupt table with interrupt bits.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
*
* INPUTS:
*   *digi_handle         - pointer to DIGI handle instance
*   chnl_handle          - pointer to channel handle to retrieve interrupt on
*   filt_table_ptr       - pointer to filter table. Mark fields as TRUE within this
*                          table to indicate that the interrupt/status will be retrieved
*                          to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                          interrupt/status.
*
* OUTPUTS:
*   int_table_ptr  - pointer to channel interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_enet_pmon_int_chnl_filt_retrieve(digi_handle_t           *digi_handle,
                                                              digi_mapper_chnl_t      *chnl_handle,
                                                              enet_int_chnl_t         *filt_table_ptr,
                                                              enet_int_chnl_t         *int_table_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_mapper_enet_int_chnl_filt_retrieve_internal(digi_handle,
                                                              chnl_handle,
                                                              TRUE,
                                                              filt_table_ptr,
                                                              int_table_ptr);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_enet_pmon_int_chnl_filt_retrieve */

/*******************************************************************************
* digi_mapper_int_summary_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves summary of mapper interrupts.
*
*   digi_otn_odu_int_start(COREOTN_LATCH_DEFECT_STG4) or 
*   digi_otn_odu_int_start(COREOTN_LATCH_DEFECT_ALL) needs to be called before
*   checking for active ints.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   unused          - Unused
*
* OUTPUTS:
*   int_summary_ptr - Pointer to interrupt summary structure
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_int_summary_get(digi_handle_t              *digi_handle,
                                             UINT32                      unused,
                                             digi_mapper_int_summary_t  *int_summary_ptr)
{
    PMC_ERROR       result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_mapper_int_summary_get_internal(digi_handle,
                                                  unused,
                                                  int_summary_ptr);


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_summary_get */

/*******************************************************************************
* digi_mapper_int_chnl_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a channel handle and an aggregated interrupt table, retrieve a pointer
*   to the corresponding channel interrupt table.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to retrieve interrupt on
*   int_table_ptr   - Aggregated interrupt table
*
* OUTPUTS:
*   int_chnl_table_pptr  - pointer to channel interrupt table corresponding to channel
*                          handle. Set to NULL if channel has not been provisioned.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_int_chnl_get(digi_handle_t              *digi_handle,
                                          digi_mapper_chnl_t         *chnl_handle,
                                          digi_mapper_int_t          *int_table_ptr,
                                          digi_mapper_int_chnl_t    **int_chnl_table_pptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          index;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != int_table_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != int_chnl_table_pptr, DIGI_ERR_INVALID_ARG, 0, 0);


    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    /* Set return table to NULL */
    *int_chnl_table_pptr = NULL;

    /* Get index corresponding to handle */
    digi_mapper_channel_num_get(digi_handle, chnl_handle, &index, NULL, NULL, NULL);

    /* Set return table to channel table */
    if (int_table_ptr->handle[index] == chnl_handle)
    {
        *int_chnl_table_pptr = &int_table_ptr->mapper_chnl[index];
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_mapper_int_chnl_get */



/*******************************************************************************
* digi_mapper_int_enabled_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with enabled interrupt bits
*   over multiple channels.
*
*
* INPUTS:
*   *digi_handle   - pointer to DIGI handle instance
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store enabled interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_int_enabled_get(digi_handle_t       *digi_handle,
                                             digi_mapper_int_t   *int_table_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;
    digi_int_type_t                  int_type = DIGI_INT_TYPE_OTN_MAPPER;

    UINT32                           start_link = 0;
    UINT32                           end_link = 0;
    UINT32                           i;
    digi_mapper_int_t               *int_en_table_ptr;
    digi_mapper_chnl_def_t          *handle_pool_ptr;
    odu_struct_t                    *stg4_handle_pool_ptr;
    digi_mapper_chnl_t              *chnl_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);
    result = digi_int_param_check(digi_handle, DIGI_INT_ALL_CHNLS, int_type, int_table_ptr); 

    PMC_MEMSET(int_table_ptr, 0, sizeof(digi_mapper_int_t));

    handle_pool_ptr = digi_handle->handle_pool.mapper_chnl;
    stg4_handle_pool_ptr = digi_handle->handle_pool.s4_odu_struct;


    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }


    /* Get internal enabled interrupt table to check */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }
   
    if (PMC_SUCCESS == result)
    {
        /* Copy table to output */
        PMC_MEMCPY(int_table_ptr, int_en_table_ptr, sizeof(*int_table_ptr));
    }

    
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        chnl_ptr = &handle_pool_ptr[i];
        int_table_ptr->handle[i] = NULL;

        if (util_global_is_chnl_prov(chnl_ptr))
        {
            /* Store channel handle */
            int_table_ptr->handle[i] = chnl_ptr; 
            
            result = digi_otn_odu_int_chnl_stage_enabled_get_helper(digi_handle,
                                                                    &stg4_handle_pool_ptr[i],
                                                                    ODU_STRUCT_LEVEL_4_ODU,
                                                                    &int_table_ptr->mapper_chnl[i].fo2.status);
        }

    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_enabled_get */

/*******************************************************************************
* digi_mapper_int_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all channels of the subsystem.
*
*   The callback associated with the aggregation interrupt type will be called
*   if an active interrupt is found in any channel. The callback will be
*   called a minimum of zero times if no active interrupts are found, or
*   a maximum of one time if there is an interrupt in at least one channel.
*
*   A callback must be registered for the interrupt type before this API
*   can be called.
*
* INPUTS:
*   *digi_handle   - pointer to DIGI handle instance
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
PUBLIC PMC_ERROR digi_mapper_int_enabled_check(digi_handle_t      *digi_handle)
{
    PMC_ERROR                result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    result = digi_mapper_int_enabled_check_internal(digi_handle);
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_enabled_check */

/*******************************************************************************
* digi_mapper_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all channels of the subsystem.
*
*   The callback associated with the channel interrupt type will be called
*   for each channel that has an active interrupt.

*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of once per channel if there is an active interrupt
*   on every channel.
*
*   digi_otn_odu_int_start(COREOTN_LATCH_DEFECT_STG4) or 
*   digi_otn_odu_int_start(COREOTN_LATCH_DEFECT_ALL) needs to be called before
*   checking for active ints.
*
*   digi_otn_ddeg_int_start() needs to be called if checking for active dDEG ints.
*
*   A callback must be registered for the interrupt type before this API can be called.
*
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   int_summary_ptr - If digi_mapper_int_summary_get() was run prior to 
*                     calling this function, the pointer to the interrupt summary
*                     structure can be passed in to decrease the search
*                     time for active interrupts in this function.
*                     Set this parameter to NULL if the summary was not retrieved.
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
PUBLIC PMC_ERROR digi_mapper_int_chnl_enabled_check(digi_handle_t              *digi_handle,
                                                    digi_mapper_int_summary_t  *int_summary_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    UINT32                       i;
    BOOL                         int_summary_free_req = FALSE;         
    digi_int_type_t              int_type = DIGI_INT_CHNL_TYPE_OTN_MAPPER;
    digi_mapper_chnl_t          *chnl_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
   
    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == mapotn_start_state_test(digi_handle->mapotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }


    if(!digi_int_is_cb_registered(digi_handle, int_type))
    {
        result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
    }


    /* Find active channels */ 
    if ((PMC_SUCCESS == result) &&
        (NULL == int_summary_ptr))
    {
        int_summary_ptr = (digi_mapper_int_summary_t *) PMC_MALLOC(sizeof(digi_mapper_int_summary_t));
        PMC_MEMSET(int_summary_ptr, 0, sizeof(digi_mapper_int_summary_t));
        int_summary_free_req = TRUE;
        
        result = digi_mapper_int_summary_get_internal(digi_handle,
                                                      0, /* unused */
                                                      int_summary_ptr);
    }



    /* Call user callback for all active channels */
    for (i = 0; (PMC_SUCCESS == result) && (i < int_summary_ptr->active_chnl_num); i++)
    {
        chnl_handle = int_summary_ptr->active_chnl[i];

        result = digi_int_callback_run(digi_handle, 
                                       chnl_handle,
                                       int_type);
    }
        
    
    if (TRUE == int_summary_free_req)
    {
        PMC_FREE(&int_summary_ptr);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_int_chnl_enabled_check */

/*******************************************************************************
* digi_mapper_int_fo2_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all FO2 channels of the subsystem.
*
*   The callback associated with the channel interrupt type will be called
*   for each channel that has an active interrupt.

*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of once per channel if there is an active interrupt
*   on every channel.
*
*   digi_otn_odu_int_start(COREOTN_LATCH_DEFECT_STG4) or 
*   digi_otn_odu_int_start(COREOTN_LATCH_DEFECT_ALL) needs to be called before
*   checking for active ints.
*
*   digi_otn_ddeg_int_start() needs to be called if checking for active dDEG ints.
*
*   A callback must be registered for the interrupt type before this API can be called.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   int_summary_ptr - If digi_otn_odu_int_summary_get() was run prior to 
*                     calling this function, the pointer to the interrupt summary
*                     structure can be passed in to decrease the search
*                     time for active interrupts in this function.
*                     Set this parameter to NULL if the summary was not retrieved.
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
PUBLIC PMC_ERROR digi_mapper_int_fo2_chnl_enabled_check(digi_handle_t               *digi_handle,
                                                        digi_otn_odu_int_summary_t  *int_summary_ptr)
{
    PMC_ERROR               result = PMC_SUCCESS;
    BOOL                    int_summary_free_req = FALSE;         
    digi_mapper_chnl_t     *chnl_handle;
    UINT32                  i;
    UINT32                  chnl_num;
    odu_struct_odu_level_t  odu_level = ODU_STRUCT_LEVEL_4_ODU;
    digi_mapper_chnl_def_t *handle_pool_ptr;
  
    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    handle_pool_ptr = digi_handle->handle_pool.mapper_chnl;
 
    
    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == coreotn_stg4_start_state_test(digi_handle->coreotn_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }

    if ((PMC_SUCCESS == result) &&
        (!digi_int_is_cb_registered(digi_handle, DIGI_INT_CHNL_TYPE_OTN_MAPPER_FO2)))
    {
        result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
    }


    /* Check ODU level of summary information */
    if (NULL != int_summary_ptr)
    {
        if (FALSE == (int_summary_ptr->mask & DIGI_OTN_ODU_INT_SUMMARY_MASK_ODU_LEVEL))
        {
            PMC_ATOMIC_RETURN(digi_handle, DIGI_ERR_INT_SUMMARY_ODU_LEVEL_NOT_AVAIL);
        }

        if (odu_level != int_summary_ptr->odu_level)
        {
            PMC_ATOMIC_RETURN(digi_handle, DIGI_ERR_INT_SUMMARY_ODU_LEVEL_MISMATCH);
        }
    }


    if (NULL == int_summary_ptr)
    {
        int_summary_ptr = (digi_otn_odu_int_summary_t *) PMC_MALLOC(sizeof(digi_otn_odu_int_summary_t));
        PMC_MEMSET(int_summary_ptr, 0, sizeof(digi_otn_odu_int_summary_t));
        int_summary_free_req = TRUE;
    }



    /* Get active channels for this odu level */
    if (PMC_SUCCESS == result)
    {
        result = digi_otn_odu_int_active_chnl_get(digi_handle,
                                                  odu_level,
                                                  TRUE,
                                                  int_summary_ptr);
    }

    /* For active chnls in list, call user callback */
    for (i = 0; (i < int_summary_ptr->active_chnl_num) && (PMC_SUCCESS == result); i++)
    {
        /* Get mapper handle corresponding to Stage 4 */
        chnl_num    = digi_otn_odu_chnl_num_internal_get(digi_handle, int_summary_ptr->active_chnl[i]);
        chnl_handle = &handle_pool_ptr[chnl_num];

        result = digi_int_callback_run(digi_handle, 
                                       chnl_handle,
                                       DIGI_INT_CHNL_TYPE_OTN_MAPPER_FO2);
    } 


    if (TRUE == int_summary_free_req)
    {
        PMC_FREE(&int_summary_ptr);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);

} /* digi_mapper_int_fo2_chnl_enabled_check */

/*******************************************************************************
* digi_mapper_enet_etrans_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all mapper ENET ETRANS (NON-ENET-PMON) channels.
*
*   The callback associated with the channel interrupt type will be called
*   for each channel that has an active interrupt.

*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of once per channel if there is an active interrupt
*   on every channel.
*
*   A callback must be registered for DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET interrupt 
*   type before this API can be called.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   int_summary_ptr - If digi_mapper_int_summary_get() was run prior to 
*                     calling this function, the pointer to the interrupt summary
*                     structure can be passed in to decrease the search
*                     time for active interrupts in this function.
*                     Set this parameter to NULL if the summary was not retrieved.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*  To check mapper ENET PMON interrupts use digi_mapper_enet_pmon_int_chnl_enabled_check().
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_enet_etrans_int_chnl_enabled_check(digi_handle_t              *digi_handle,
                                                                digi_mapper_int_summary_t  *int_summary_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    UINT32                       i;  
    digi_int_type_t              int_type = DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET_ETRANS;
    digi_mapper_chnl_def_t      *chnl_handle = DIGI_INT_ALL_CHNLS;
    digi_mapper_int_t           *int_en_table_ptr;
    digi_mapper_chnl_def_t      *handle_pool_ptr;
    BOOL8                        int_found;
    UINT32                       start_link = 0;
    UINT32                       end_link = 0;
    BOOL                         int_summary_free_req = FALSE;    
    UINT32                       j;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
   
    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == enet_start_state_test(digi_handle->enet_line_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }


    if((PMC_SUCCESS == result) &&
       (!digi_int_is_cb_registered(digi_handle, DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET_ETRANS)))
    {
        result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
    }

    /* Determine if are accessing one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         chnl_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }

    /* Get internal enabled interrupt table to check */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       DIGI_INT_TYPE_OTN_MAPPER,
                                       (void**) &int_en_table_ptr);

    }
    if (PMC_SUCCESS == result &&
        (NULL == int_summary_ptr))
    {
        int_summary_ptr = (digi_mapper_int_summary_t *) PMC_MALLOC(sizeof(digi_mapper_int_summary_t));
        int_summary_free_req = TRUE;
    }

    if (PMC_SUCCESS == result)
    {
        handle_pool_ptr = digi_handle->handle_pool.mapper_chnl;
        PMC_MEMSET(int_summary_ptr, 0, sizeof(digi_mapper_int_summary_t));
    }

    /* Call user callback for all active channels */
    for (i = start_link, j = 0; (PMC_SUCCESS == result) && (i < end_link); i++)
    {
        chnl_handle = &handle_pool_ptr[i];
        int_found = FALSE;
        result = digi_mapper_enet_etrans_int_chnl_enabled_check_internal(digi_handle,
                                                                         chnl_handle,
                                                                         &int_en_table_ptr->mapper_chnl[i],
                                                                         (BOOL8 *)&int_found);
        
        if (PMC_SUCCESS == result &&
            TRUE == int_found)
        {
            int_summary_ptr->active_chnl[j] = (digi_mapper_chnl_t  *)chnl_handle;
            j += 1;
            result = digi_int_callback_run(digi_handle, 
                                           chnl_handle,
                                           int_type);
        }
    }
        
    int_summary_ptr->active_chnl_num = j;

    int_summary_ptr->mask = DIGI_MAPPER_INT_SUMMARY_MASK_ACTIVE_CHNL_NUM |
        DIGI_MAPPER_INT_SUMMARY_MASK_ACTIVE_CHNL;

    if (TRUE == int_summary_free_req)
    {
        PMC_FREE(&int_summary_ptr);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_enet_etrans_int_chnl_enabled_check */

/*******************************************************************************
* digi_mapper_enet_pmon_int_chnl_enabled_check
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all mapper ENET PMON channels.
*
*   The callback associated with the channel interrupt type will be called
*   for each channel that has an active interrupt.

*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of once per channel if there is an active interrupt
*   on every channel.
*
*   A callback must be registered for DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET interrupt 
*   type before this API can be called.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   int_summary_ptr - If digi_mapper_int_summary_get() was run prior to 
*                     calling this function, the pointer to the interrupt summary
*                     structure can be passed in to decrease the search
*                     time for active interrupts in this function.
*                     Set this parameter to NULL if the summary was not retrieved.
*
*   
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*  To check mapper ENET ETRANS interrupts use digi_mapper_enet_etrans_int_chnl_enabled_check().
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_enet_pmon_int_chnl_enabled_check(digi_handle_t              *digi_handle,
                                                              digi_mapper_int_summary_t  *int_summary_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    UINT32                       i;  
    UINT32                       j;  
    digi_int_type_t              int_type = DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET_PMON;
    digi_mapper_chnl_def_t      *chnl_handle = DIGI_INT_ALL_CHNLS;
    digi_mapper_int_t           *int_en_table_ptr;
    digi_mapper_chnl_def_t      *handle_pool_ptr;
    BOOL8                        int_found;
    UINT32                       start_link = 0;
    UINT32                       end_link = 0;
    BOOL                         int_summary_free_req = FALSE;    

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
   
    if((PMC_SUCCESS == result) &&
       (!digi_int_is_cb_registered(digi_handle, DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET_PMON)))
    {
        result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
    }

    /* Determine if are accessing one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         chnl_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }

    /* Get internal enabled interrupt table to check */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       DIGI_INT_TYPE_OTN_MAPPER,
                                       (void**) &int_en_table_ptr);

    }
    if (PMC_SUCCESS == result &&
        (NULL == int_summary_ptr))
    {
        int_summary_ptr = (digi_mapper_int_summary_t *) PMC_MALLOC(sizeof(digi_mapper_int_summary_t));
        int_summary_free_req = TRUE;
    }

    if (PMC_SUCCESS == result)
    {
        handle_pool_ptr = digi_handle->handle_pool.mapper_chnl;
        PMC_MEMSET(int_summary_ptr, 0, sizeof(digi_mapper_int_summary_t));
    }

    /* Call user callback for all active channels */
    for (i = start_link, j = 0; (PMC_SUCCESS == result) && (i < end_link); i++)
    {
        chnl_handle = &handle_pool_ptr[i];
        int_found = FALSE;

        result = digi_mapper_enet_pmon_int_chnl_enabled_check_internal(digi_handle,
                                                                       chnl_handle,
                                                                       &int_en_table_ptr->mapper_chnl[i],
                                                                       (BOOL8 *)&int_found);
        
        if (PMC_SUCCESS == result &&
            TRUE == int_found)
        {
            int_summary_ptr->active_chnl[j] = (digi_mapper_chnl_t  *)chnl_handle;
            j += 1;
            result = digi_int_callback_run(digi_handle, 
                                           chnl_handle,
                                           int_type);
        }
    }
        
    int_summary_ptr->active_chnl_num = j;

    int_summary_ptr->mask = DIGI_MAPPER_INT_SUMMARY_MASK_ACTIVE_CHNL_NUM |
        DIGI_MAPPER_INT_SUMMARY_MASK_ACTIVE_CHNL;

    if (TRUE == int_summary_free_req)
    {
        PMC_FREE(&int_summary_ptr);
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_mapper_enet_pmon_int_chnl_enabled_check */

/*******************************************************************************
* digi_mapper_gfp_tx_fcs_corrupt_set
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*  Enable or disable TGFPF frame FCS corruption feature. When enabled, the 
*  device inverses GFPF frame FCS when an errored data frame is encountered.
*                                                                               
* INPUTS:                                               
*   digi_handle         - pointer to DIGI handle instance.   
*   *chnl_ctxt_ptr      - pointer to mapper channel handle to be operated on 
*   enable              - when TRUE, GFP frame payload FCS is corrupted when
*                         a errored frame is encountered.\n
*                         when FALSE, corruption is disabled.
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
PUBLIC PMC_ERROR digi_mapper_gfp_tx_fcs_corrupt_set(digi_handle_t       *digi_handle,
                                                    digi_mapper_chnl_t  *chnl_ctxt_ptr,
                                                    BOOL8                enable)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32                  mapotn_chnl;
    PMC_ERROR               result;

    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    /* find mapotn channel */    
    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    mapotn_tgfpf_fcs_corrupt_set(digi_handle->mapotn_handle, mapotn_chnl, enable);

    PMC_ATOMIC_RETURN(digi_handle, result);

}  /* digi_mapper_gfp_tx_fcs_corrupt_set */


/*******************************************************************************
* digi_mapper_gfp_tx_fcs_corrupt_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*  Returns the status of TGFPF frame FCS corruption feature. When enabled, the 
* device inverses GFPF frame FCS when an errored data frame is encountered.
*                                                                               
* INPUTS:                
*   digi_handle         - pointer to DIGI handle instance.   
*   *chnl_ctxt_ptr      - pointer to mapper channel handle to be operated on 
*
*                                                                             
* OUTPUTS:                                                                      
*    None.
*                                                                           
* RETURNS:                                                                      
*    BOOL8              - The state of PFCS_CRPT_INV register that corrupts 
*                         payload FCS of a GFP frame.
*                                                                               
* NOTES:                                                                        
*                                                                               
*******************************************************************************/
PUBLIC BOOL8 digi_mapper_gfp_tx_fcs_corrupt_get(digi_handle_t       *digi_handle,
                                                digi_mapper_chnl_t  *chnl_ctxt_ptr)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32                  mapotn_chnl;
    BOOL8                   status;
    PMC_ERROR               result;

#ifdef PMC_TESTS 
    PMC_ATOMIC_ENTRY_VOID(digi_handle);
#else 
    PMC_ATOMIC_ENTRY(digi_handle);
#endif
    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* find mapotn channel */    
    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_ctxt_ptr, result);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    status = mapotn_tgfpf_fcs_corrupt_get(digi_handle->mapotn_handle, mapotn_chnl);

    PMC_ATOMIC_RETURN(digi_handle, status);
}  /* digi_mapper_gfp_tx_fcs_corrupt_get */

/*******************************************************************************
* digi_mapper_gfp_frm_len_stats_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the collection of frame length statistics in the
*   GFP demapper. Seven ranges of client frame lengths can be specified
*   using the digi_mapper_gfp_stats_cfg_t structure: \n
*       range 0: min_fl[0]  > length > max_fl[0]     \n
*       range 1: min_fl[1]  > length > max_fl[1]     \n
*       ...                                          \n
*       range 6: min_fl[6]  > length > max_fl[6]     \n
*   
*   Where 'length' is the size of the client frame payload area (i.e. excluding 
*   the type header, extension header and checksum). \n
*
*   Frames are only counted if the PTI=0. \n
*
*   The statistics can be retrieved using the Frame Length Statistics Counter 
*   PMON: FRM_LEN_STAT_CNT_PMON. \n
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance
*   chnl_handle        - pointer to channel handle
*   gfp_stats_cfg_ptr  - Frame length range configuration
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
PUBLIC PMC_ERROR digi_mapper_gfp_frm_len_stat_cfg(digi_handle_t              *digi_handle,
                                                  digi_mapper_chnl_t          *chnl_handle,
                                                  digi_mapper_gfp_stats_cfg_t *gfp_stats_cfg_ptr)
                                                    
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          bucket,chnl;
    digi_mapper_chnl_def_t *mapper_chnl_ptr = (digi_mapper_chnl_def_t *)chnl_handle;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != gfp_stats_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);


    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    DIGI_CHNL_HANDLE_MAGIC_CHECK(mapper_chnl_ptr);    
    chnl = mapper_chnl_ptr->mapotn_chnl;

    for(bucket=0; bucket < DIGI_MAPPER_GFP_STATS_MAX; bucket++)
    {
        (void)mapotn_rgfpf_frm_len_stat_cfg(digi_handle->mapotn_handle, 
                                            chnl,
                                            bucket,
                                            gfp_stats_cfg_ptr->max_fl[bucket],
                                            gfp_stats_cfg_ptr->min_fl[bucket]); 
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_mapper_gfp_frm_len_stat_cfg */

/*******************************************************************************
* digi_mapper_gfp_frm_len_stat_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function retrieves the current configuration for capturing frame length 
*   statistics in the GFP demapper.  The configuration is stored in the 
*   digi_mapper_gfp_stats_cfg_t structure which contains 7 ranges of frame 
*   lengths: \n
*       range 0: min_fl[0]  > length > max_fl[0]  \n
*       range 1: min_fl[1]  > length > max_fl[1]  \n
*       ...                                       \n
*       range 6: min_fl[6]  > length > max_fl[6]  \n
*   
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance
*   chnl_handle        - pointer to channel handle
*   gfp_stats_cfg_ptr  - Frame length range configuration (digi_mapper_gfp_stats_cfg_t)
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
PUBLIC PMC_ERROR digi_mapper_gfp_frm_len_stat_cfg_get(digi_handle_t              *digi_handle,
                                                      digi_mapper_chnl_t          *chnl_handle,
                                                      digi_mapper_gfp_stats_cfg_t *gfp_stats_cfg_ptr)
                                                    
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          bucket,chnl;
    UINT32          max_fl, min_fl;
    digi_mapper_chnl_def_t *mapper_chnl_ptr = (digi_mapper_chnl_def_t *)chnl_handle;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != gfp_stats_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    DIGI_CHNL_HANDLE_MAGIC_CHECK(mapper_chnl_ptr);    
    chnl = mapper_chnl_ptr->mapotn_chnl;

    for(bucket=0; bucket < DIGI_MAPPER_GFP_STATS_MAX; bucket++)
    {
        (void)mapotn_rgfpf_frm_len_stat_cfg_get(digi_handle->mapotn_handle, 
                                                chnl,
                                                bucket,
                                                &max_fl,
                                                &min_fl);
        gfp_stats_cfg_ptr->max_fl[bucket] = max_fl;
        gfp_stats_cfg_ptr->min_fl[bucket] = min_fl; 
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_mapper_gfp_frm_len_stat_cfg_get */

/*******************************************************************************
* digi_mapper_gfp_frm_len_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures minimum and maximum length of payload information 
*   filed in a GFP frame. Frames with client payload information field 
*   (payload area excluding header and optional pFCS) out of the range 
*   of lengths are unconditionally discarded.
*
*   Frame minimum and maximum length check is applicable only for frames with PTI = "000". \n
*
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance
*   chnl_handle        - pointer to channel handle
*   max_frm_len        - Frame maximum length
*   min_frm_len        - Frame minimum length
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
PUBLIC PMC_ERROR digi_mapper_gfp_frm_len_cfg(digi_handle_t              *digi_handle,
                                             digi_mapper_chnl_t         *chnl_handle,
                                             UINT16                     max_frm_len,
                                             UINT16                     min_frm_len)
    
{
    PMC_ERROR               result;
    UINT32                  chnl;
    digi_mapper_chnl_def_t *mapper_chnl_ptr = (digi_mapper_chnl_def_t *)chnl_handle;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);

    /* check that mapper handle belongs to digi context */ 
    DIGI_MAPPER_CHNL_HANDLE_CHECK(digi_handle, chnl_handle, result);

    DIGI_CHNL_HANDLE_MAGIC_CHECK(mapper_chnl_ptr);    
    chnl = mapper_chnl_ptr->mapotn_chnl;
    (void)mapotn_rgfpf_egress_frm_len_chk_cfg(digi_handle->mapotn_handle, 
                                              chnl,
                                              max_frm_len,
                                              min_frm_len); 
    
    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_mapper_gfp_frm_len_cg */

/*******************************************************************************
* digi_mapper_gfp_csf_status_get
* ______________________________________________________________________________
*                                                                               
* DESCRIPTION:
*   Retrieves the status of CSF frame/defect indication received.
*                                                                               
* INPUTS:                                                                       
*   *digi_handle        - pointer to DIGI handle instance
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
PUBLIC PMC_ERROR digi_mapper_gfp_csf_status_get(digi_handle_t   *digi_handle,
                                                UINT32          *valid,
                                                UINT32          *chid,
                                                UINT32          *upi)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != valid, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != chid, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != upi, DIGI_ERR_INVALID_ARG, 0, 0);

    result = mapotn_rgfpf_csf_stat(digi_handle->mapotn_handle, valid, chid, upi);
    
    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_mapper_gfp_csf_status_get */


#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */

#ifdef PMC_DEPRECATED

/* LCOV_EXCL_START */

/*******************************************************************************
* digi_mapper_ordered_set_upi_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED - This function was replaced by digi_gsup43_7_3_legacy_mode_cfg
*
*   This function configures the GFP ordered set UPI value for both the TX and RX.
*   The UPI values apply to all GFP channels. This function should be called 
*   before any GFP channels are configured. 
*
*
* INPUTS:
*   *digi_handle       - pointer to DIGI handle instance
*   tx_upi_val         - TGFPF UPI value
*   rx_upi_val         - RGFPF UPI value
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
PUBLIC PMC_ERROR digi_mapper_ordered_set_upi_cfg(digi_handle_t  *digi_handle,
                                                 UINT8 tx_upi_val, UINT8 rx_upi_val)
    
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);

    result = mapotn_ordered_set_upi_cfg(digi_handle->mapotn_handle,
                                        tx_upi_val,
                                        rx_upi_val);

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_mapper_ordered_set_upi_set */

/*******************************************************************************
* digi_mapper_ordered_set_upi_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   DEPRECATED - This function was replaced by digi_gsup43_7_3_legacy_mode_get
*
*   This function retrieves the configured ordered set UPI value for both RGFPF 
*   and TGFPF.
*
* INPUTS:
*   *digi_handle   - pointer to DIGI handle instance
*
* OUTPUTS:
*   tx_upi         - Configured TGFPF UPI value
*   rx_upi         - Configured RGFPF UPI value
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_ordered_set_upi_get(digi_handle_t *digi_handle,
                                                 UINT8 *tx_upi,
                                                 UINT8 *rx_upi)
{
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);

    *tx_upi = mapotn_tgfpf_order_set_upi_get(digi_handle->mapotn_handle);
    *rx_upi = mapotn_rgfpf_ordered_set_upi_get(digi_handle->mapotn_handle);
    
    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_mapper_ordered_set_upi_get */

/* LCOV_EXCL_STOP */

#endif /* PMC_DEPRECATED */
#endif

#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */


/* private functions */
/*******************************************************************************
* digi_mapper_int_chnl_fw_opu_csf_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves OPU CSF status from firmware for multiple channels.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   start_idx       - table start index in which interrupts will be stored
*   start_chnl      - index of first channel to be processed
*   end_chnl        - index of last channel to be processed
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_mapper_int_chnl_fw_opu_csf_retrieve(digi_handle_t            *digi_handle,
                                                           UINT32                    start_idx,
                                                           UINT32                    start_chnl,
                                                           UINT32                    end_chnl,  
                                                           digi_mapper_int_chnl_t   *filt_table_ptr,
                                                           digi_mapper_int_chnl_t   *int_table_ptr)
{
    UINT32 rc = PMC_SUCCESS;
    UINT32 i;
    UINT32 opu_csf_v_bits95_64;
    UINT32 opu_csf_v_bits63_32;
    UINT32 opu_csf_v_bits31_0 ;
    digi_mapper_chnl_def_t          *handle_pool_ptr;
    digi_mapper_chnl_t              *chnl_ptr;

    PMC_ENTRY();

    if (DIGI_INT_ALL_INTS != filt_table_ptr)
    {
        if (FALSE == filt_table_ptr->status.opu_csf_i && 
            FALSE == filt_table_ptr->status.opu_csf_v) 
        {
            PMC_RETURN(PMC_SUCCESS);
        }
    }

    /* Special case for OPU CSF opu_csf_v is computed by FW */
    
    rc = digi_fw_opu_csf_int_rsp(digi_handle,
                                 &opu_csf_v_bits95_64,
                                 &opu_csf_v_bits63_32,
                                 &opu_csf_v_bits31_0);


    handle_pool_ptr = digi_handle->handle_pool.mapper_chnl;

    for (i = start_chnl; i < end_chnl && rc == PMC_SUCCESS; i++, start_idx += 1)
    {       
        chnl_ptr = &handle_pool_ptr[i];
        
        if (util_global_is_chnl_prov(chnl_ptr) && 
            (((digi_mapper_chnl_def_t *)chnl_ptr)->switch_data.oduk_data.op_state.map_state.is_output_mapped ||
             ((digi_mapper_chnl_def_t *)chnl_ptr)->switch_data.cpb_data.op_state.map_state.is_output_mapped))
        { 
            /* Special case for OPU CSF status is computed by FW */
            switch (i / 32)
            {
            case 0:
                int_table_ptr[start_idx].status.opu_csf_v = (0x1 & (opu_csf_v_bits31_0 >> (i % 32)));
                break;
            case 1:
                int_table_ptr[start_idx].status.opu_csf_v = (0x1 & (opu_csf_v_bits63_32 >> (i % 32)));
                break;
            case 2:
                int_table_ptr[start_idx].status.opu_csf_v = (0x1 & (opu_csf_v_bits95_64 >> (i % 32)));
                break;
            default: 
                break;
            }
        }
    }

    PMC_RETURN(rc);
} /* digi_mapper_int_chnl_fw_opu_csf_retrieve */

/*******************************************************************************
* digi_mapper_int_chnl_enabled_check_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in provided channel of the subsystem.
*   Returns if interrupt is found.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to check interrupts
*
*   
* OUTPUTS:
*   int_found_ptr     - Returns TRUE if an active interrupt is found
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_mapper_int_chnl_enabled_check_internal(digi_handle_t       *digi_handle,
                                                              digi_mapper_chnl_t  *chnl_handle,
                                                              BOOL                *int_found_ptr)
{
    PMC_ERROR                result = PMC_SUCCESS;
    digi_int_type_t          int_type = DIGI_INT_TYPE_OTN_MAPPER;

    mapotn_handle_t         *mapotn_handle;
    UINT32                   start_link = 0;
    UINT32                   end_link = 0;
    UINT32                   i;
    digi_mapper_int_t       *int_en_table_ptr;
    digi_mapper_chnl_def_t  *handle_pool_ptr;
    BOOL8                    multichannels;
    BOOL8                    bool8_found_ptr = *int_found_ptr;
    
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == mapotn_start_state_test(digi_handle->mapotn_handle))
    {
        PMC_RETURN(PMC_SUCCESS);
    }
    mapotn_handle = digi_handle->mapotn_handle;
    handle_pool_ptr = digi_handle->handle_pool.mapper_chnl;
    
    /* Determine if are accessing one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         chnl_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }
    if (IS_DIGI_INT_ALL_CHNLS(chnl_handle))
    {
        multichannels = TRUE;
    }
    else
    {
        multichannels = FALSE;
    }


    /* Get internal enabled interrupt table to check */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }
    
    
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result) && (FALSE == bool8_found_ptr); i++)
    {
        if (TRUE == multichannels)
        {
            chnl_handle = &handle_pool_ptr[i];
        }

        if (util_global_is_chnl_prov(chnl_handle))
        {
            /* Check enabled interrupts */
            result = mapotn_int_chnl_enabled_check(mapotn_handle, 
                                                   i,
                                                   &int_en_table_ptr->mapper_chnl[i].status,
                                                   &bool8_found_ptr);
      
            if (PMC_SUCCESS == result &&
                FALSE == bool8_found_ptr)
            {
                result = digi_mapper_enet_etrans_int_chnl_enabled_check_internal(digi_handle,
                                                                                 chnl_handle,
                                                                                 &int_en_table_ptr->mapper_chnl[i],
                                                                                 &bool8_found_ptr);
            }

            if (PMC_SUCCESS == result &&
                FALSE == bool8_found_ptr)
            {
                result = digi_mapper_enet_pmon_int_chnl_enabled_check_internal(digi_handle,
                                                                               (digi_mapper_chnl_def_t *)chnl_handle,
                                                                               &int_en_table_ptr->mapper_chnl[i],
                                                                               &bool8_found_ptr);
            }        
        }
    } /* for (i = start_link; ... */

    *int_found_ptr = bool8_found_ptr;
    
    /* disable interrupt caching */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, FALSE, FALSE);


    /* enable interrupt caching, don't clear cache */
    pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, TRUE, FALSE);

    PMC_RETURN(result);
} /* digi_mapper_int_chnl_enabled_check_internal */



/*******************************************************************************
* digi_mapper_int_enabled_check_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in all channels of the subsystem.
*
*   The callback associated with the aggregation interrupt type will be called
*   if an active interrupt is found in any channel. The callback will be
*   called a minimum of zero times if no active interrupts are found, or
*   a maximum of one time if there is an interrupt in at least one channel.
*
*   A callback must be registered for the interrupt type before this API
*   can be called.
*
* INPUTS:
*   *digi_handle     - pointer to DIGI handle instance
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
PUBLIC PMC_ERROR digi_mapper_int_enabled_check_internal(digi_handle_t  *digi_handle)
{
    PMC_ERROR                result = PMC_SUCCESS;
    BOOL                     int_found = FALSE;
    
    PMC_ENTRY();
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == mapotn_start_state_test(digi_handle->mapotn_handle))
    {
        PMC_RETURN(PMC_SUCCESS);
    }

    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_OTN_MAPPER))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
    }


    if (PMC_SUCCESS == result)
    {
        result = digi_int_top_enabled_check(digi_handle,
                                            DIGI_INT_TYPE_OTN_MAPPER,
                                            &int_found);
    }

    
    if ((PMC_SUCCESS == result) && (FALSE == int_found))
    {
        BOOL etrans_in_path = mapotn_is_etrans_in_path(digi_handle->mapotn_handle);
        
        if (TRUE == etrans_in_path)
        {
            result = digi_int_top_enabled_check(digi_handle,
                                                DIGI_INT_TYPE_ENET_LINE,
                                                &int_found);                                    
        }
    }
    
    /* Check enabled interrupts for enh dDeg */
    if (PMC_SUCCESS == result && FALSE == int_found)
    {
        result = digi_otn_odu_int_ddeg_enabled_check(digi_handle, 
                                                     COREOTN_DDEG_MON_CLIENT, 
                                                     &int_found);
    }
    if (PMC_SUCCESS == result && FALSE == int_found)
    {
        result = digi_int_top_enabled_check(digi_handle,
                                            DIGI_INT_TYPE_ENET_SYS,
                                            &int_found);           
    }

#ifdef PMC_SW_SIMULATION
    /* Force found interrupt for simulation so that callback runs */
    int_found = TRUE;
#endif

    if (PMC_SUCCESS == result && TRUE == int_found)
    {
        result = digi_int_callback_run(digi_handle, DIGI_INT_ALL_CHNLS, 
                                       DIGI_INT_TYPE_OTN_MAPPER);
    }


    PMC_RETURN(result);
} /* digi_mapper_int_enabled_check_internal */

/*******************************************************************************
* digi_mapper_gfp_tx_fcs_mode_internal_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets transmit payload FCS field of a GFP frame for a mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*   fcs_mode               - defines whether or not the payload FCS field is  
*                            inserted in a GFP frame\n 
*                            0 -- Do not insert any FCS\n
*                            1 -- Insert pFCS for GFP Frame\n
*                            2 -- Calculate eFCS for special Ethernet traffic 
*                                 received from the CPB after skipping 4-bytes 
*                                 of the packet\n
*                            3 -- Calculate eFCS for special Ethernet traffic 
*                                 received from the CPB after skipping 8-bytes 
*                                 of the packet\n
*                            4 -- Calculate eFCS for special Ethernet traffic 
*                            received from the CPB after skipping 12-bytes of 
*                            the packet\n
*                            5 -- Calculate eFCS for special Ethernet traffic 
*                                 received from the CPB after skipping 16-bytes 
*                                 of the packet\n
*                            6 -- Calculate eFCS for special Ethernet traffic 
*                                 received from the CPB including the preamble 
*                                 bytes\n
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*   For the following modes from ENET_LINE to MAPOTN are not allowed to 
*   optionally insert FCS.  The FCS insertion is restricted for these datapaths 
*   as ENET_LINE does not provide flexibility.
*   DIGI_MAP_ODU4P_100_GE_GFP 
*   DIGI_MAP_ODU3P_40_GE_GFP
*   DIGI_MAP_ODU2P_10_GE_6_2_GFP 
*   DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP 
*   DIGI_MAP_ODU3P_PKT_40_GE_GMP
*   DIGI_MAP_ODU4P_PKT_100_GE_GMP 
*   DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP
*   DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_tx_fcs_mode_internal_set(digi_handle_t *digi_handle,
                                                          digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                          mapotn_fcs_mode_t fcs_mode)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    digi_mapping_mode_t mode;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    mode = mapper_chnl_handle_ptr->mode;
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    if( DIGI_MAP_ODU4P_100_GE_GFP == mode ||
        DIGI_MAP_ODU3P_40_GE_GFP == mode ||
        DIGI_MAP_ODU2P_10_GE_6_2_GFP == mode ||
        DIGI_MAP_ODU2P_PKT_10_GE_7_3_GFP == mode ||
        DIGI_MAP_ODU3P_PKT_40_GE_GMP == mode ||
        DIGI_MAP_ODU4P_PKT_100_GE_GMP == mode || 
        DIGI_MAP_ODU1EP_PKT_10G3_7_2_BMP == mode ||
        DIGI_MAP_ODU2EP_PKT_10G3_7_1_BMP == mode )
    {
        result = DIGI_ERR_MAPPER_FCS_INSERTION_NOT_ALLOWED;
    }
    else if (mapper_chnl_handle_ptr->switch_data.cpb_data.op_state.active_state == UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE) 
    { 
        /* This function can only be called before the channel is activated */
        PMC_LOG(PMC_LOG_SEV_HIGH, DIGI_ERR_CPB_MUST_NOT_BE_ACTIVE,0,0, "ERROR: CPB chnl state = ACTIVE\n");
        result = DIGI_ERR_CPB_MUST_NOT_BE_ACTIVE;
    }
    else {
        result = mapotn_tgfpf_chnl_payload_fcs_cfg(digi_handle->mapotn_handle,
                                                   mapotn_chnl, fcs_mode,     
                                                   0,0,0,0,1,1);   
    }
                                                                                         
    
    PMC_RETURN(result);

} /* digi_mapper_gfp_tx_fcs_mode_internal_set */     

/*******************************************************************************
* digi_mapper_gfp_rx_fcs_mode_internal_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves RX payload FCS field of a GFP frame for a mapper channel.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*
* OUTPUTS:
*   *fcs_mode              - defines whether or not the payload FCS field is  
*                           inserted in a GFP frame\n 
*                           0 -- No FCS check\n
*                           1 -- pFCS check \n
*                           2 -- eFCS check after skipping programmed number 
*                                of preambles (determined by PREAMBLE_SFD_MODE)\n
*                           3 -- eFCS check from 1st byte onwards 
*                                (no preamble skipping)\n
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_gfp_rx_fcs_mode_internal_get(digi_handle_t      *digi_handle,
                                                          digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                          UINT32             *fcs_mode)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    PMC_ERROR result = PMC_SUCCESS;
    
    PMC_ENTRY();

    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;

    result = mapotn_rgfpf_fcs_chk_mode_get(digi_handle->mapotn_handle,
                                           mapotn_chnl, fcs_mode);   
    
    PMC_RETURN(result);

} /* digi_mapper_gfp_rx_fcs_mode_internal_get */     

/*******************************************************************************
* digi_mapper_channel_num_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function gets the mapotn channel number.
*
* INPUTS:
*   *digi_handle        - pointer to DIGI handle instance
*   *chnl_ctxt_ptr      - pointer to mapper channel handle to be operated on 
*
* OUTPUTS:
*   *mapotn_chnl_ptr    - pointer to storage of MAPOTN channel ID 
*   *fo2_chnl_ptr       - pointer to storage of COREOTN FO2 channel ID 
*   *enet_sys_link_ptr  - pointer to storage of ENET SYS link number 
*   *enet_line_link_ptr - pointer to storage of ENET line link number 
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_mapper_channel_num_get(digi_handle_t *digi_handle,
                                             digi_mapper_chnl_t *chnl_ctxt_ptr, 
                                             UINT32 *mapotn_chnl_ptr, 
                                             UINT32 *fo2_chnl_ptr, 
                                             UINT32 *enet_sys_link_ptr, 
                                             UINT32 *enet_line_link_ptr)
{
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr;
    UINT32 mapotn_chnl;
    UINT32 fo2_chnl; 
    
    PMC_ERROR rc = PMC_SUCCESS;
    
    PMC_ENTRY();
   
    /* Check arguments */
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(chnl_ctxt_ptr != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);
    
    mapotn_chnl = mapper_chnl_handle_ptr->mapotn_chnl;
    fo2_chnl = mapper_chnl_handle_ptr->switch_data.oduk_data.channel;
    
    switch(mapper_chnl_handle_ptr->enet_pmon){
    case DIGI_NO_PMON:
        enet_sys_link_ptr = NULL;
        enet_line_link_ptr = NULL;
        break;
    case DIGI_ENET_LINE_PMON:
        enet_sys_link_ptr = NULL;
        if (enet_line_link_ptr) {
            *enet_line_link_ptr = mapper_chnl_handle_ptr->switch_data.cpb_data.channel;
        }
        break;
    case DIGI_ENET_SYS_PMON:
        if (enet_sys_link_ptr) {
            *enet_sys_link_ptr = mapper_chnl_handle_ptr->switch_data.cpb_data.channel;
        }
        enet_line_link_ptr = NULL;
        break;    
    default:
        PMC_ASSERT(FALSE, DIGI_ERR_INVALID_ARG, 0, 0);
        break;
    }

    if (mapotn_chnl_ptr) {
        *mapotn_chnl_ptr = mapotn_chnl;
    }

    if (fo2_chnl_ptr) {
        *fo2_chnl_ptr = fo2_chnl;
    }

    PMC_RETURN(rc);

} /* digi_mapper_channel_num_get */
                                       
/*******************************************************************************
* digi_mapper_pmon_chnl_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves a channel PMON table with PMON counters
*   for a single channel in the subsystem.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to retrieve PMON on
*
* OUTPUTS:
*   pmon_table_ptr  - pointer to channel PMON table to store counters 
*                     for a single channel
*
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_mapper_pmon_chnl_retrieve(digi_handle_t              *digi_handle,
                                                 util_global_switch_data_t  *chnl_handle,   
                                                 digi_mapper_pmon_chnl_t    *pmon_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT32              mapper_link;
    UINT32              enet_line_link;
    UINT32              enet_sys_link;
    digi_pmon_t        *fw_pmon_ptr;
 
    PMC_ENTRY();
    result = digi_pmon_chnl_param_check(digi_handle, chnl_handle, pmon_table_ptr); 

    if (PMC_SUCCESS == result)
    {
        result = digi_pmon_shared_mem_get(digi_handle, &fw_pmon_ptr); 
    }

    if (PMC_SUCCESS == result)
    {        
        digi_mapper_channel_num_get(digi_handle,
                                    chnl_handle,
                                    &mapper_link,
                                    NULL,
                                    &enet_line_link,
                                    &enet_sys_link);

        PMC_MEMSET(pmon_table_ptr, 0, sizeof(digi_mapper_pmon_chnl_t));

#ifdef DIGI_MAPPER_PMON 
#undef DIGI_MAPPER_PMON
#endif
#define DIGI_MAPPER_PMON(tbl_name)                                      \
        do {                                                            \
            PMC_MEMCPY(&pmon_table_ptr->tbl_name,                       \
                       &fw_pmon_ptr->mapotn_##tbl_name[mapper_link],    \
                       sizeof(pmon_table_ptr->tbl_name));               \
        } while(0)

        DIGI_MAPPER_PMON_TABLE_DEFINE();
    }



    /* Retrieve enhanced PMON */
    if (PMC_SUCCESS == result)
    {
        result = digi_enet_enhanced_pmon_chnl_retrieve_internal(digi_handle,
                                                                chnl_handle,
                                                                &pmon_table_ptr->enet,
                                                                &pmon_table_ptr->enet_mode);

    }
    
    PMC_RETURN(result);
} /* digi_mapper_pmon_chnl_retrieve */


/*******************************************************************************
* digi_mapper_pmon_fo2_chnl_retrieve
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves a channel PMON table with PMON counters
*   for a single channel in the subsystem.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to retrieve PMON on
*
* OUTPUTS:
*   pmon_table_ptr  - pointer to channel PMON table to store counters 
*                     for a single channel
*
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_mapper_pmon_fo2_chnl_retrieve(digi_handle_t                  *digi_handle,
                                                     util_global_switch_data_t      *chnl_handle,   
                                                     digi_mapper_pmon_fo2_chnl_t    *pmon_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT32              mapper_link;
    UINT32              fo2_link;
    UINT32              enet_line_link;
    UINT32              enet_sys_link;
    odu_struct_t       *fo2_chnl_handle;
    digi_pmon_t        *fw_pmon_ptr;
 
    PMC_ENTRY();
    result = digi_pmon_chnl_param_check(digi_handle, chnl_handle, pmon_table_ptr); 

    if (PMC_SUCCESS == result)
    {
        result = digi_pmon_shared_mem_get(digi_handle, &fw_pmon_ptr); 
    }

    if (PMC_SUCCESS == result)
    {        
        digi_mapper_channel_num_get(digi_handle,
                                    chnl_handle,
                                    &mapper_link,
                                    &fo2_link,
                                    &enet_line_link,
                                    &enet_sys_link);

        PMC_MEMSET(pmon_table_ptr, 0, sizeof(digi_mapper_pmon_fo2_chnl_t));
    }


    /* Retrieve COREOTN FO2 (Stage 4) PMON */

    if (PMC_SUCCESS == result)
    {
        PMC_ASSERT(mapper_link == fo2_link, DIGI_ERR_CODE_ASSERT, mapper_link, fo2_link);

        /* Get FO2 channel handle */
        fo2_chnl_handle = &digi_handle->handle_pool.s4_odu_struct[fo2_link];

        result = digi_otn_odu_pmon_chnl_retrieve_internal(digi_handle,
                                                          fo2_chnl_handle,
                                                          &pmon_table_ptr->pmon);
    }

    
    PMC_RETURN(result);
} /* digi_mapper_pmon_fo2_chnl_retrieve */




/*******************************************************************************
* digi_mapper_int_summary_get_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves summary of Mapper interrupts
*   
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   unused          - Unused
*
* OUTPUTS:
*   int_summary_ptr - Pointer to interrupt summary structure
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_mapper_int_summary_get_internal(digi_handle_t               *digi_handle,
                                                       UINT32                       unused,
                                                       digi_mapper_int_summary_t   *int_summary_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    UINT32                       start_link = 0;
    UINT32                       end_link = 0;
    UINT32                       i, j;
    digi_mapper_chnl_def_t      *handle_pool_ptr;
    BOOL                         int_found;
    digi_int_type_t              int_type = DIGI_INT_TYPE_OTN_MAPPER;
    digi_mapper_chnl_t          *active_mapper[DIGI_MAPOTN_CHNL_MAX];
    digi_otn_odu_int_summary_t   odu_summary;
    UINT32                       chnl_num;

    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    PMC_ASSERT(NULL != int_summary_ptr, DIGI_ERR_NULL_HANDLE, 0, 0);

    PMC_MEMSET((void*) &odu_summary, 0, sizeof(odu_summary));
    PMC_MEMSET((void*) active_mapper, 0, sizeof(digi_mapper_chnl_t *) * DIGI_MAPOTN_CHNL_MAX);

    /* Get handle pool ptr */
    handle_pool_ptr  = digi_handle->handle_pool.mapper_chnl;
    

    /* Get range of handles */ 
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }



    if (FALSE == mapotn_start_state_test(digi_handle->mapotn_handle))
    {
        /* enable interrupt caching, clear cache */
        pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, TRUE, TRUE);


        /* Find channels with active interrupts, and place them in array based on index */
        for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
        {
            int_found = FALSE;

            result = digi_mapper_int_chnl_enabled_check_internal(digi_handle,
                                                                 &handle_pool_ptr[i],
                                                                 &int_found);

            if ((PMC_SUCCESS == result) && (TRUE == int_found))
            {
                active_mapper[i] = &handle_pool_ptr[i];
            }
        }

        /* disable interrupt caching */
        pmc_sys_int_cache_enable_set(((pmc_handle_t*)digi_handle)->sys_handle, FALSE, FALSE);
    }

    if (FALSE == coreotn_stg4_start_state_test(digi_handle->coreotn_handle))
    {
        /* Retrieve active FO2 channels */
        if (PMC_SUCCESS == result)
        {
            result = digi_otn_odu_int_active_chnl_get(digi_handle,
                                                      ODU_STRUCT_LEVEL_4_ODU,
                                                      FALSE, /* active channels will be ordered by index */
                                                      &odu_summary);
        }
    }



    /* Consolidate active mapper and FO2 channels into output summary table */
    j = 0;
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        if (active_mapper[i] != NULL) 
        {
            int_summary_ptr->active_chnl[j] = active_mapper[i];
            j++;
        }
        else if (odu_summary.active_chnl[i] != NULL)
        {
            /* Get mapper handle corresponding to Stage 4 */
            chnl_num = digi_otn_odu_chnl_num_internal_get(digi_handle, odu_summary.active_chnl[i]);

            /* Convert ODU FO2 handle to Mapper handle */
            int_summary_ptr->active_chnl[j] = &handle_pool_ptr[chnl_num];
            j++;
        }
    }
    



    int_summary_ptr->active_chnl_num = j;

    int_summary_ptr->mask = DIGI_MAPPER_INT_SUMMARY_MASK_ACTIVE_CHNL_NUM |
        DIGI_MAPPER_INT_SUMMARY_MASK_ACTIVE_CHNL;


    PMC_RETURN(result);
} /* digi_mapper_int_summary_get_internal */


/*******************************************************************************
* digi_mapper_is_mapotn_mode_gfp_enet
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function determines if the MAPOTN channel is one of the following:
*
*   DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET\n
*   DIGI_MAP_ODU3P_40_GE_GFP_ENET\n
*   DIGI_MAP_ODU4P_100_GE_GFP_ENET\n
*
*  It returns FALSE for any other mapping mode.
*
* INPUTS:
*   *mapper_chnl_handle_ptr  - pointer to Enet channel handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8                     - TRUE if the mapping mode is as described in the description.\n
*                               It returns FALSE otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PUBLIC BOOL8 digi_mapper_is_mapotn_mode_gfp_enet(digi_mapper_chnl_def_t* mapper_chnl_handle_ptr)
{
    PMC_ENTRY();
    BOOL8 rc = FALSE;

    if ((mapper_chnl_handle_ptr->mode == DIGI_MAP_ODU2P_10_GE_6_2_GFP_ENET) ||
        (mapper_chnl_handle_ptr->mode == DIGI_MAP_ODU3P_40_GE_GFP_ENET) ||
        (mapper_chnl_handle_ptr->mode == DIGI_MAP_ODU4P_100_GE_GFP_ENET)) {

        rc = TRUE;
    }

    PMC_RETURN(rc);
} /* digi_mapper_is_mapotn_mode_gfp_enet */

/*******************************************************************************
* digi_mapper_is_far_end_enet_sys
* ______________________________________________________________________________
*
* DESCRIPTION:
*  This function determines if the far end that is mapped to ENET_SYS
*
* INPUTS:
*   *digi_handle            - pointer to DIGI handle instance
*   *mapper_chnl_handle_ptr - pointer to mapper channel handle instance
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   BOOL8                   - TRUE if the mapped to ENET_SYS.\n
*                             It returns FALSE otherwise.
*
* NOTES:
*
*
*******************************************************************************/
PRIVATE BOOL8 digi_mapper_is_far_end_enet_sys(digi_handle_t *digi_handle,
                                              digi_mapper_chnl_def_t* mapper_chnl_handle_ptr)
{
    BOOL8 rc = FALSE;
    digi_enet_client_chnl_def_t *enet_chnl_handle_ptr = NULL;

    PMC_ENTRY();

    /* Get other end (ENET_SYS) chnl_handle_ptr */
    /* We need to look up the other end of the cpb channel (if there is one). */
    enet_chnl_handle_ptr = (digi_enet_client_chnl_def_t *)digi_cpb_input_port_get(digi_handle, mapper_chnl_handle_ptr);

    if (enet_chnl_handle_ptr == NULL) {

    } else if (enet_chnl_handle_ptr->switch_data.cpb_data.port_type == UTIL_GLOBAL_CPB_PORT_ENET_SYS) {

        rc = TRUE;
    }

    PMC_RETURN(rc);
} /* digi_mapper_is_far_end_enet_sys */


/*******************************************************************************
* digi_mapper_ilkn_gfp_tx_upi_sel_chnl_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validate that mapper handle provided to digi_mapper_ilkn_gfp_tx_upi_sel_get() or 
*   digi_mapper_ilkn_gfp_tx_upi_sel_set() follows these polcies:
*   - mapper is connected to ILKN.
*   - ILKN channel rx header is enabled.
*   - channel is not activated yet when digi_mapper_ilkn_gfp_tx_upi_sel_set() is called.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on
*    check_set_cond        - when TRUE, channel activation state is checked and
*                            ILKN RX header type is enabled (needed by digi_mapper_ilkn_gfp_tx_upi_sel_set).
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
PRIVATE PMC_ERROR digi_mapper_ilkn_gfp_tx_upi_sel_chnl_validate(digi_handle_t      *digi_handle,
                                                                digi_mapper_chnl_t *chnl_ctxt_ptr,
                                                                BOOL8               check_set_cond)
{   
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr; 
    PMC_ERROR result = PMC_SUCCESS;
    util_global_cpb_port_data_t *input_pc_ptr;
    util_global_switch_data_def_t *switch_input_pc_ptr;

    PMC_ENTRY();

    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    

    /* validate that */
    /* - mapper channel is mapped to a ILKN channel */
    /* - ILKN channel header type is enabled, if needed */
    switch_input_pc_ptr = (util_global_switch_data_def_t *)digi_cpb_input_port_get(digi_handle, &mapper_chnl_handle_ptr->switch_data); 
    if (NULL != switch_input_pc_ptr)
    {
        input_pc_ptr = &switch_input_pc_ptr->cpb_data;

        if (((UTIL_GLOBAL_CPB_PORT_ILKN1 != input_pc_ptr->port_type && 
              UTIL_GLOBAL_CPB_PORT_ILKN2 != input_pc_ptr->port_type) ||
             DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX <= input_pc_ptr->channel))
        {
            result = DIGI_ERR_INVALID_ARG;
        }
            
        /* Check that ILKN channel RX header type is TRUE if check_set_cond == TRUE */
        if( result == PMC_SUCCESS && TRUE == check_set_cond)
        {
            if (UTIL_GLOBAL_CPB_PORT_ILKN1 == input_pc_ptr->port_type && 
                TRUE == digi_handle->handle_pool.ilkn_1_chnl[input_pc_ptr->channel].rx_type_header)
            {
            }
            else if (UTIL_GLOBAL_CPB_PORT_ILKN2 == input_pc_ptr->port_type && 
                     TRUE == digi_handle->handle_pool.ilkn_2_chnl[input_pc_ptr->channel].rx_type_header)
            {
            }
            else
            {               
                result = DIGI_ERR_INVALID_ARG; 
            }
        }
    }
    else
    {
        result = DIGI_ERR_INVALID_ARG; 
    }

    /* Check that mapper channel is not activated yet if check_set_cond == TRUE */
    if (PMC_SUCCESS == result && TRUE == check_set_cond)
    {
        if (mapper_chnl_handle_ptr->switch_data.cpb_data.op_state.active_state == UTIL_GLOBAL_CHNL_ACTIVE_STATE_ACTIVE)
        {            
            result = DIGI_ERR_CPB_ALREADY_ACTIVE; 
        }
    }

    PMC_RETURN(result);
}/* digi_mapper_ilkn_gfp_tx_upi_sel_chnl_validate */

/*******************************************************************************
* digi_mapper_chnl_dcpb_dest_validate
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Validate that mapper channel is mapped to given destination.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_ctxt_ptr         - pointer to mapper channel handle to be operated on.
*    dest                  - channel end destination: ILKN1, ILKN2, etc.
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
PRIVATE PMC_ERROR digi_mapper_chnl_dcpb_dest_validate(digi_handle_t          *digi_handle,
                                                      digi_mapper_chnl_t     *chnl_ctxt_ptr,
                                                      util_global_cpb_port_t  dest)
{   
    digi_mapper_chnl_def_t *mapper_chnl_handle_ptr; 
    PMC_ERROR result = DIGI_ERR_INVALID_ARG;
    util_global_switch_data_def_t *input_pc_ptr;
    UINT32 itr;

    PMC_ENTRY();
    
    mapper_chnl_handle_ptr = (digi_mapper_chnl_def_t*)(chnl_ctxt_ptr);
    DIGI_CHNL_HANDLE_MAGIC_CHECK(chnl_ctxt_ptr);

    /* for each handle associated to destination in digi handle pool,
       try to find that handle input port is mapper channel */
    switch(dest) {

    case UTIL_GLOBAL_CPB_PORT_ILKN1:
        for (itr = 0; itr < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX; itr++)
        {
            if (TRUE == digi_handle->handle_pool.ilkn_1_chnl[itr].switch_data.cpb_data.op_state.map_state.is_output_mapped)
            {
                input_pc_ptr = (util_global_switch_data_def_t*)digi_cpb_input_port_get(digi_handle, (util_global_switch_data_t*)&(digi_handle->handle_pool.ilkn_1_chnl[itr]));
                if (NULL != input_pc_ptr && 
                    UTIL_GLOBAL_CPB_PORT_MAPOTN == input_pc_ptr->cpb_data.port_type &&
                    mapper_chnl_handle_ptr->switch_data.cpb_data.channel == input_pc_ptr->cpb_data.channel)
                {
                    result = PMC_SUCCESS;
                    break;
                }
            }
        }
        break;
    case UTIL_GLOBAL_CPB_PORT_ILKN2:
        for (itr = 0; itr < DIGI_SIFD_ILKN_ARB_DPI_CHANS_MAX; itr++)
        {
            if (TRUE == digi_handle->handle_pool.ilkn_2_chnl[itr].switch_data.cpb_data.op_state.map_state.is_output_mapped)
            {
                input_pc_ptr = (util_global_switch_data_def_t*)digi_cpb_input_port_get(digi_handle, (util_global_switch_data_t*)&(digi_handle->handle_pool.ilkn_2_chnl[itr]));
                if (NULL != input_pc_ptr && 
                    UTIL_GLOBAL_CPB_PORT_MAPOTN == input_pc_ptr->cpb_data.port_type &&
                    mapper_chnl_handle_ptr->switch_data.cpb_data.channel == input_pc_ptr->cpb_data.channel)
                {
                    result = PMC_SUCCESS;
                    break;
                }
            }
        }
        break;
    case UTIL_GLOBAL_CPB_PORT_CBRC:
        for (itr = 0; itr < DIGI_CBRC_CHNL_MAX; itr++)
        {
            if (TRUE == digi_handle->handle_pool.cbr_chnl[itr].switch_data.cpb_data.op_state.map_state.is_output_mapped)
            {
                input_pc_ptr = (util_global_switch_data_def_t*)digi_cpb_input_port_get(digi_handle, (util_global_switch_data_t*)&(digi_handle->handle_pool.cbr_chnl[itr]));
                if (NULL != input_pc_ptr && 
                    UTIL_GLOBAL_CPB_PORT_MAPOTN == input_pc_ptr->cpb_data.port_type &&
                    mapper_chnl_handle_ptr->switch_data.cpb_data.channel == input_pc_ptr->cpb_data.channel)
                {
                    result = PMC_SUCCESS;
                    break;
                }
            }
        }
        break;
    case UTIL_GLOBAL_CPB_PORT_MAPOTN:
        for (itr = 0; itr < DIGI_MAPOTN_CHNL_MAX; itr++)
        {
            if (TRUE == digi_handle->handle_pool.mapper_chnl[itr].switch_data.cpb_data.op_state.map_state.is_output_mapped)
            {
                input_pc_ptr = (util_global_switch_data_def_t*)digi_cpb_input_port_get(digi_handle, (util_global_switch_data_t*)&(digi_handle->handle_pool.mapper_chnl[itr]));
                if (NULL != input_pc_ptr && 
                    UTIL_GLOBAL_CPB_PORT_MAPOTN == input_pc_ptr->cpb_data.port_type &&
                    mapper_chnl_handle_ptr->switch_data.cpb_data.channel == input_pc_ptr->cpb_data.channel)
                {
                    result = PMC_SUCCESS;
                    break;
                }
            }
        }
        break;
    case UTIL_GLOBAL_CPB_PORT_ENET_LINE:
        for (itr = 0; itr < DIGI_ENET_LINE_CHNL_MAX; itr++)
        {
            if (TRUE == digi_handle->handle_pool.enet_line_chnl[itr].switch_data.cpb_data.op_state.map_state.is_output_mapped)
            {
                input_pc_ptr = (util_global_switch_data_def_t*)digi_cpb_input_port_get(digi_handle, (util_global_switch_data_t*)&(digi_handle->handle_pool.enet_line_chnl[itr]));
                if (NULL != input_pc_ptr && 
                    UTIL_GLOBAL_CPB_PORT_MAPOTN == input_pc_ptr->cpb_data.port_type &&
                    mapper_chnl_handle_ptr->switch_data.cpb_data.channel == input_pc_ptr->cpb_data.channel)
                {
                    result = PMC_SUCCESS;
                    break;
                }
            }
        }
        break;
    case UTIL_GLOBAL_CPB_PORT_ENET_SYS:
        for (itr = 0; itr < DIGI_ENET_SYS_CHNL_MAX; itr++)
        {
            if (TRUE == digi_handle->handle_pool.enet_sys_chnl[itr].switch_data.cpb_data.op_state.map_state.is_output_mapped)
            {
                input_pc_ptr = (util_global_switch_data_def_t*)digi_cpb_input_port_get(digi_handle, (util_global_switch_data_t*)&(digi_handle->handle_pool.enet_sys_chnl[itr]));
                if (NULL != input_pc_ptr && 
                    UTIL_GLOBAL_CPB_PORT_MAPOTN == input_pc_ptr->cpb_data.port_type &&
                    mapper_chnl_handle_ptr->switch_data.cpb_data.channel == input_pc_ptr->cpb_data.channel)
                {
                    result = PMC_SUCCESS;
                    break;
                }
            }
        }   
        break;
    default:
        break;
    }

    PMC_RETURN(result);
}/* digi_mapper_ilkn_gfp_tx_upi_sel_chnl_validate */

/*******************************************************************************
* digi_map_opu_csf_int_default_cfg_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   The function computes OPU_CSF monitor parameters default values.
*
* INPUTS:
*   *digi_handle             - pointer to DIGI handle instance.
*   *mapper_chnl_ctxt_ptr    - pointer to mapper channel handle
*    enable                  - when TRUE opu_csf defect monitor parameters
*                              are computed for activation.
*                              otherwise there are computed for deactivation.
*
*
* OUTPUTS:
*   *trunk2client_tables_ptr - pointer to updated trunk2client tables in which OPU CSF
*                              monitor parameters default values are written.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_map_opu_csf_int_default_cfg_get(digi_handle_t                  *digi_handle,
                                                      digi_mapper_chnl_def_t         *mapper_chnl_ctxt_ptr,
                                                      BOOL8                           enable,
                                                      digi_trunk2client_tables_cmf_t *trunk2client_tables_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 opu_csf_persistance_time;
    DOUBLE odu_rate = UTIL_GLOBAL_ODU0_RATE;
    UINT32 oduflex_cbr_rate = 0;
    UINT32 odu_flex_data = 0;
    util_global_odukp_type_t oduk_data = UTIL_GLOBAL_LAST_ODUK;
    digi_group_size_t group_size;
    BOOL8 is_odu_flex = FALSE;
    
    PMC_ENTRY();

    if (TRUE == enable)
    {
        digi_oduk_params_convert(mapper_chnl_ctxt_ptr->odu_data, mapper_chnl_ctxt_ptr->odu_flex_data, &oduk_data, &group_size);
    
        digi_mapper_odu_flex_data_get(mapper_chnl_ctxt_ptr->mode, &odu_flex_data,
                                      &oduk_data,
                                      &oduflex_cbr_rate);
        switch(oduk_data)
        {            
        case UTIL_GLOBAL_ODU0:
            odu_rate =  UTIL_GLOBAL_ODU0_RATE;
            break;
        case UTIL_GLOBAL_ODU1:
            odu_rate =  UTIL_GLOBAL_ODU1_RATE;
            break;
        case UTIL_GLOBAL_ODU1E:
        case UTIL_GLOBAL_ODU1E_ETRANS:
            odu_rate = UTIL_GLOBAL_ODU1E_RATE;
            break;
        case UTIL_GLOBAL_ODU1F:
            odu_rate =  UTIL_GLOBAL_ODU1F_RATE;
            break;
        case UTIL_GLOBAL_ODU2:
            odu_rate =  UTIL_GLOBAL_ODU2_RATE;
            break;
        case UTIL_GLOBAL_ODU2E:
        case UTIL_GLOBAL_ODU2E_ETRANS:
            odu_rate =  UTIL_GLOBAL_ODU2E_RATE;
            break;
        case UTIL_GLOBAL_ODU2F:
            odu_rate =  UTIL_GLOBAL_ODU2F_RATE;
            break;
        case UTIL_GLOBAL_ODU3:
            odu_rate =  UTIL_GLOBAL_ODU3_RATE;
            break;
        case UTIL_GLOBAL_ODU3E1:
            odu_rate =  UTIL_GLOBAL_ODU3E1_RATE;
            break;
        case UTIL_GLOBAL_ODU3E2:
            odu_rate =  UTIL_GLOBAL_ODU3E2_RATE;
            break;
        case UTIL_GLOBAL_ODU4:
            odu_rate =  UTIL_GLOBAL_ODU4_RATE;
            break;
        case UTIL_GLOBAL_ODUFLEX_CBR:
        case UTIL_GLOBAL_ODUFLEX_GFP:
            odu_rate = UTIL_GLOBAL_ODU0_RATE * odu_flex_data;        
            is_odu_flex = TRUE;
            break;
        case UTIL_GLOBAL_ODUK_DONT_CARE:
            result = DIGI_ERR_INVALID_ARG;
            break;
        default:
            result = DIGI_ERR_INVALID_ARG;
            break;
        }
                    
        if (FALSE == is_odu_flex)
        {
            opu_csf_persistance_time = DIGI_MAP_OPU_CSF_PERISTANCE_TIME_COMPUTE(odu_rate);
            /* safety check */
            if (0 == opu_csf_persistance_time)
            {
                opu_csf_persistance_time = 1;
            }       
            if (DIGI_MAP_ODU0_OPU_CSF_PERISTANCE_DEFAULT_VALUE <  opu_csf_persistance_time)
            {
                opu_csf_persistance_time = DIGI_MAP_ODU0_OPU_CSF_PERISTANCE_DEFAULT_VALUE;
            }
        }
        else
        {
            if (0 != odu_flex_data)
            {
                opu_csf_persistance_time = DIGI_MAP_ODU0_OPU_CSF_PERISTANCE_DEFAULT_VALUE / odu_flex_data;
            }
            else
            {
                opu_csf_persistance_time = DIGI_MAP_ODU0_OPU_CSF_PERISTANCE_DEFAULT_VALUE; 
            }
        }
    }
    else
    {
        /* default value for ODU0 */
        opu_csf_persistance_time = DIGI_MAP_ODU0_OPU_CSF_PERISTANCE_DEFAULT_VALUE; 
    }

    /* fullfill monitor parameters default value */
    if (PMC_SUCCESS == result)
    {
        trunk2client_tables_ptr->port_mapping_cmf.opu_csf_persistance_time = opu_csf_persistance_time;
        trunk2client_tables_ptr->conseq_action_cmf.opu_csf_mon.monitor_defect = enable;
        trunk2client_tables_ptr->conseq_action_cmf.opu_csf_mon.priority_idx = 0;
    }

    PMC_RETURN(result); 
}/* digi_map_opu_csf_int_default_cfg_get */


/*******************************************************************************
* digi_map_opu_csf_int_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configure and activate/deactivate OPU_CSF monitor.
*
*   The function should be called during mapper channel mapping or demapping only to
*   to allow proper reading of OPU_CSF interrupt status value from firmware.
*
* INPUTS:
*   *digi_handle             - pointer to DIGI handle instance.
*   *mapper_chnl_ctxt_ptr    - pointer to mapper channel handle.
*    output_chnl             - identifier of output channel mapped to mapper over DCPB or ODUKSW
*    enable                  - when TRUE opu_csf interrupt status monitoring is activated
*                              otherwise it is deactivated.
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
PUBLIC PMC_ERROR digi_map_opu_csf_int_cfg(digi_handle_t          *digi_handle,
                                          digi_mapper_chnl_def_t *mapper_chnl_ctxt_ptr,
                                          UINT32                  output_chnl,
                                          BOOL8                   enable)
{
    PMC_ERROR result = PMC_SUCCESS;

    digi_trunk2client_tables_cmf_t trunk2client_tables;


    PMC_ENTRY();
    PMC_MEMSET((void *) &trunk2client_tables, 0, sizeof(trunk2client_tables));
    
    result = digi_map_opu_csf_int_default_cfg_get(digi_handle,
                                                  mapper_chnl_ctxt_ptr,
                                                  enable,
                                                  &trunk2client_tables);

    if (PMC_SUCCESS == result)
    {  
        result = digi_cmf_opu_csf_monitor_cfg(digi_handle,
                                              mapper_chnl_ctxt_ptr,
                                              output_chnl,
                                              &trunk2client_tables);
    }

    PMC_RETURN(result); 
}/* digi_map_opu_csf_int_cfg */

/*******************************************************************************
* digi_mapper_enet_pmon_handle_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Returns enet handle and link id associated to Mapper ENET PMON channel.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   *chnl_handle    - pointer to mapper channel handle
*   
* OUTPUTS:
*   **enet_pptr     - pointer to a pointer to ENET handle (LINE or SYS)
*   *link_ptr       - pointer to link number
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_mapper_enet_pmon_handle_get(digi_handle_t          *digi_handle,
                                                   digi_mapper_chnl_t      *chnl_handle,
                                                   enet_handle_t          **enet_pptr,
                                                   UINT32                  *link_ptr)
{
    
    PMC_ERROR                  result = PMC_SUCCESS;
    digi_enet_client_chnl_t   *digi_enet_handle_ptr = NULL;
    
    PMC_ENTRY();

    digi_enet_handle_ptr = digi_enet_enhanced_pmon_ptr_get((util_global_switch_data_def_t*)chnl_handle);
        
    if (NULL != digi_enet_handle_ptr)
    {
        result = digi_enet_validate_parms(digi_handle,
                                          digi_enet_handle_ptr,
                                          enet_pptr,
                                          link_ptr);
    }
    else
    {
        *enet_pptr = NULL;
    }

    PMC_RETURN(result);
}/* digi_mapper_enet_pmon_handle_get */

/*******************************************************************************
* digi_mapper_enet_int_chnl_filt_retrieve_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves a mapper channel ENET ETRANS or ENET PMON interrupt table
*   with interrupt bits.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*
*
* INPUTS:
*   *digi_handle         - pointer to DIGI handle instance
*   chnl_handle          - pointer to channel handle to retrieve interrupt on
*   ret_enet_pmon        - when TRUE ENET PMON interrupts are retrieved
*                          when FALSE ENET interrupts associated to MAPOTN <-> ENET/ETRANS datapath are retrieved. 
*   filt_table_ptr       - pointer to filter table. Mark fields as TRUE within this
*                          table to indicate that the interrupt/status will be retrieved
*                          to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                          interrupt/status.
*
* OUTPUTS:
*   int_table_ptr  - pointer to channel interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_mapper_enet_int_chnl_filt_retrieve_internal(digi_handle_t           *digi_handle,
                                                                   digi_mapper_chnl_t      *chnl_handle,
                                                                   BOOL8                    ret_enet_pmon,
                                                                   enet_int_chnl_t         *filt_table_ptr,
                                                                   enet_int_chnl_t         *int_table_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;
    digi_int_type_t                  int_type = (TRUE == ret_enet_pmon) ? DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET_PMON : DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET_ETRANS;
    enet_handle_t                   *enet_ptr;
    UINT32                           link;
    UINT32                           dummy;

    PMC_ENTRY();

    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 
   
    if ((PMC_SUCCESS == result) &&
        (NULL == chnl_handle))
    {
        PMC_RETURN(DIGI_ERR_NULL_HANDLE);
    }

    
    /* retrieve ENET (etrans) interrupt associated to MAPOTN channel */
    if (PMC_SUCCESS == result &&
        FALSE == ret_enet_pmon)
    {
        /* Skip interrupt retrieve if subsystem is not powered up */
        if (PMC_SUCCESS == result && TRUE == enet_start_state_test(digi_handle->enet_line_handle))
        {
            PMC_RETURN(PMC_SUCCESS);
        }
        
        /* Enable interrupts */
        result = digi_enet_validate_parms(digi_handle,
                                          chnl_handle,
                                          &enet_ptr,
                                          &link);
        

        if (PMC_SUCCESS == result &&  
            TRUE == enet_is_link_mapotn_test(enet_ptr, link, &dummy))
        { 
            /* Retieve interrupts from FW */
            if (digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_LINE] == TRUE)
            {
                result = digi_enet_int_fw_chnl_exco4m_rx_lobl_i_retrieve(digi_handle,
                                                                         DIGI_INT_TYPE_ENET_LINE,
                                                                         FALSE,
                                                                         TRUE,
                                                                         filt_table_ptr,
                                                                         int_table_ptr);
            }            

            if (PMC_SUCCESS == result)
            {
                /* retrieve enet/etrans interrupts */
                result = enet_int_chnl_retrieve(enet_ptr, 
                                                link,
                                                filt_table_ptr,
                                                int_table_ptr);
            }
        }
        else
        {
            result = PMC_SUCCESS;
        }
    }

    /* retrieve enet pmon interrupts */
    if (PMC_SUCCESS == result &&
        TRUE == ret_enet_pmon)
    {
        /* get enet pmon handle */
        result = digi_mapper_enet_pmon_handle_get(digi_handle,
                                                  (digi_mapper_chnl_def_t *)chnl_handle,
                                                  &enet_ptr,
                                                  &link);
        if (PMC_SUCCESS == result && 
            NULL != enet_ptr)
        { 
            if (PMC_SUCCESS == result && TRUE == enet_start_state_test(enet_ptr))
            {
                PMC_RETURN(PMC_SUCCESS);
            }

            /* Retieve interrupts from FW */
            if (DIGI_ENET_LINE_PMON == ((digi_mapper_chnl_def_t *)chnl_handle)->enet_pmon &&
                digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_LINE] == TRUE)
            {
                result = digi_enet_int_fw_chnl_exco4m_rx_lobl_i_retrieve(digi_handle,
                                                                         DIGI_INT_TYPE_ENET_LINE,
                                                                         TRUE,
                                                                         TRUE,
                                                                         filt_table_ptr,
                                                                         int_table_ptr);
            }   

            if (PMC_SUCCESS == result)
            {
                if (digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_SYS] == TRUE &&
                    DIGI_ENET_SYS_PMON == ((digi_mapper_chnl_def_t *)chnl_handle)->enet_pmon)
                {
                    result = digi_enet_int_fw_chnl_exco4m_rx_lobl_i_retrieve(digi_handle,
                                                                             DIGI_INT_TYPE_ENET_SYS,
                                                                             TRUE,
                                                                             FALSE,
                                                                             filt_table_ptr,
                                                                             int_table_ptr);
                }
            }

            if (PMC_SUCCESS == result)
            {
                /* retrieve enet interrupts in device */
                result = enet_int_chnl_retrieve(enet_ptr, 
                                                link,
                                                filt_table_ptr,
                                                int_table_ptr);
            }
        }
        else
        {
            result = PMC_SUCCESS;
        }
    }

    PMC_RETURN(result);
} /* digi_mapper_enet_int_chnl_filt_retrieve_internal */

/*******************************************************************************
* digi_mapper_enet_etrans_int_chnl_enabled_check_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches ENET ETRANS enabled interrupt bits of a given mapper channel.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to disable interrupts on
*   int_en_table_ptr- pointer to mapper channel interrupt table
*   
* OUTPUTS:
*   *int_found_ptr  - TRUE  when an enabled interrupt is found,
*                     FALSE otherwise.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*  To check mapper channel ENET PMON interrupts use 
*  digi_mapper_enet_pmon_int_chnl_enabled_check_internal().
*******************************************************************************/
PRIVATE PMC_ERROR digi_mapper_enet_etrans_int_chnl_enabled_check_internal(digi_handle_t              *digi_handle,
                                                                          digi_mapper_chnl_t         *chnl_handle,
                                                                          digi_mapper_int_chnl_t     *int_en_table_ptr,
                                                                          BOOL8                      *int_found_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    enet_handle_t               *enet_ptr;
    UINT32                       link;
    UINT32                       dummy;
    enet_int_chnl_t              enet_etrans_int_table;
    UINT32                       ltr;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    
    *int_found_ptr = FALSE;
    if (util_global_is_chnl_prov(chnl_handle))
    {
        /* check ENET (etrans) interrupts */
        result = digi_enet_validate_parms(digi_handle,
                                          chnl_handle,
                                          &enet_ptr,
                                          &link);
        if (PMC_SUCCESS == result && 
            TRUE == enet_is_link_mapotn_test(enet_ptr, link, &dummy))
        {
            
            if (TRUE == int_en_table_ptr->enet_etrans_valid)
            {
                /* check ENET interrupt generated by FW */
                if (link == 0 && digi_handle->var.fw_intr_mon_running[INTR_IPI_ENET_LINE] == TRUE)
                {
                    PMC_MEMSET(enet_etrans_int_table.etrans.exco4m.rx_lobl_i, 0, sizeof(enet_etrans_int_table.epmm.exco4m.rx_lobl_i));
                    result = digi_enet_int_fw_chnl_exco4m_rx_lobl_i_retrieve(digi_handle,
                                                                             DIGI_INT_TYPE_ENET_LINE,
                                                                             FALSE,
                                                                             TRUE,
                                                                             &int_en_table_ptr->enet_etrans,
                                                                             &enet_etrans_int_table);
                    if (PMC_SUCCESS == result)
                    {
                        for (ltr = 0; ltr < EXCO4M_N_LANES && FALSE == *int_found_ptr; ltr += 1)
                        {
                            if (TRUE == int_en_table_ptr->enet_etrans.etrans.exco4m.rx_lobl_i[ltr] &&
                                0 != enet_etrans_int_table.etrans.exco4m.rx_lobl_i[ltr])
                            {
                                *int_found_ptr = TRUE;
                            }
                        }
                    }
                }
                /* check ENET link interrupts in device */
                if (TRUE != *int_found_ptr)
                {
                    result = enet_int_chnl_enabled_check(enet_ptr, 
                                                         link,
                                                         &int_en_table_ptr->enet_etrans,
                                                         int_found_ptr,
                                                         FALSE);
                }
            }
        }
        else
        {
            result = PMC_SUCCESS;
        }
    }
    
    PMC_RETURN(result);
} /* digi_mapper_enet_etrans_int_chnl_enabled_check_internal */

/*******************************************************************************
* digi_mapper_enet_pmon_int_chnl_enabled_check_internal
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches ENET PMON enabled interrupt bits of one given mapper channels.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to disable interrupts on
*   int_en_table_ptr- pointer to mapper channel interrupt table
*
*   
* OUTPUTS:
*   *int_found_ptr  - TRUE  when an enabled interrupt is found,
*                     FALSE otherwise.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*  To check mapper ENET ETRANS interrupts use 
*  digi_mapper_enet_etrans_int_chnl_enabled_check_internal().
*******************************************************************************/
PRIVATE PMC_ERROR digi_mapper_enet_pmon_int_chnl_enabled_check_internal(digi_handle_t              *digi_handle,
                                                                        digi_mapper_chnl_def_t     *chnl_handle,
                                                                        digi_mapper_int_chnl_t     *int_en_table_ptr,
                                                                        BOOL8                      *int_found_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    enet_handle_t               *enet_ptr;
    UINT32                       link;
    enet_int_chnl_t              enet_int_table;
    UINT32                       ltr;
    UINT32 ipi_idx;

    PMC_ENTRY();
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);   

    *int_found_ptr = FALSE;
    if (util_global_is_chnl_prov(chnl_handle))
    {        
        /* check ENET PMON interrupts */
        result = digi_mapper_enet_pmon_handle_get(digi_handle,
                                                  chnl_handle,
                                                  &enet_ptr,
                                                  &link);
        if (PMC_SUCCESS == result && 
            NULL != enet_ptr)
        {         
            /* Skip interrupt retrieve if subsystem is not powered up */
            if (TRUE != enet_start_state_test(enet_ptr) &&
                TRUE == int_en_table_ptr->enet_pmon_valid)
            {
                /* check ENET interrupt generated by FW */
                ipi_idx = LAST_INTR_IPI_WORKAROUND;
                if (0 == link)
                {
                    if (DIGI_ENET_LINE_PMON == chnl_handle->enet_pmon)
                    {
                        ipi_idx = INTR_IPI_ENET_LINE;
                    }
                    else if (DIGI_ENET_SYS_PMON == chnl_handle->enet_pmon)
                    {
                        ipi_idx = INTR_IPI_ENET_SYS;
                    }
                }
                
                /* Retieve & check interrupts from FW */
                if (LAST_INTR_IPI_WORKAROUND != ipi_idx &&                        
                    digi_handle->var.fw_intr_mon_running[ipi_idx] == TRUE)
                {
                    PMC_MEMSET(enet_int_table.epmm.exco4m.rx_lobl_i, 0, sizeof(enet_int_table.epmm.exco4m.rx_lobl_i));
                    result = digi_enet_int_fw_chnl_exco4m_rx_lobl_i_retrieve(digi_handle,
                                                                             DIGI_INT_TYPE_ENET_LINE,
                                                                             TRUE,
                                                                             FALSE,
                                                                             &int_en_table_ptr->enet_pmon,
                                                                             &enet_int_table);
                    if (PMC_SUCCESS == result)
                    {
                        for (ltr = 0; ltr < EXCO4M_N_LANES && FALSE == *int_found_ptr; ltr += 1)
                        {
                            if (TRUE == int_en_table_ptr->enet_pmon.epmm.exco4m.rx_lobl_i[ltr] &&
                                0 != enet_int_table.epmm.exco4m.rx_lobl_i[ltr])
                            {
                                *int_found_ptr = TRUE;
                            }
                        }
                    }
                }
                /* check ENET link interrupts in device */
                if (TRUE != *int_found_ptr)
                {
                    result = enet_int_chnl_enabled_check(enet_ptr, 
                                                         link,
                                                         &int_en_table_ptr->enet_pmon,
                                                         int_found_ptr,
                                                         FALSE);
                }
            }
        }
        else
        {
            result = PMC_SUCCESS;
        }
    }
    
    PMC_RETURN(result);
} /* digi_mapper_enet_pmon_int_chnl_enabled_check_internal */

/*******************************************************************************
* digi_mapper_enet_int_chnl_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures ENET ETRANS or PMON interrupts of mapper channel.   
*   This API must be called after digi_cpb_chnl_map(), so as to avoid an inactive
*   companion ENET link.
*
* INPUTS:
*   *digi_handle    - pointer to DIGI handle instance
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel ENET interrupt table with bits set to
*                     enable interrupts
*   is_enet_pmon    - when TRUE enet_pmon interrupts are configured
*                     when FALSE enet_etrans interrupts are configured
*   clear           - when TRUE interrupts are cleared and enable value is ignored
*                     when FALSE enable value is considered
*   enable          - when TRUE interrupts active in int_table_ptr are enabled
*                     otherwise they are disabled
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*******************************************************************************/
PRIVATE PMC_ERROR digi_mapper_enet_int_chnl_cfg(digi_handle_t             *digi_handle,
                                                digi_mapper_chnl_t        *chnl_handle,   
                                                enet_int_chnl_t           *int_table_ptr,
                                                BOOL8                      is_enet_pmon,
                                                BOOL8                      clear,
                                                BOOL8                      enable)
{
    PMC_ERROR                result = PMC_SUCCESS;
    digi_int_type_t          int_type = DIGI_INT_TYPE_OTN_MAPPER;
    digi_int_type_t          chnl_int_type = (TRUE == is_enet_pmon) ? DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET_PMON : DIGI_INT_CHNL_TYPE_OTN_MAPPER_ENET_ETRANS;

    mapotn_handle_t         *mapotn_handle;
    UINT32                   start_link = 0;
    UINT32                   end_link = 0;
    UINT32                   i;
    digi_mapper_int_t       *int_en_table_ptr;
    digi_mapper_chnl_def_t  *handle_pool_ptr;
    enet_handle_t           *enet_ptr;
    UINT32                   link;
    UINT32                   dummy;
    enet_int_chnl_t          dummy_enet_int;
    enet_int_chnl_t         *enet_int_en_ptr;
    BOOL8                    cfg;       
    BOOL8                   *valid_ptr;
    
    PMC_ENTRY();

    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 
        
    mapotn_handle = digi_handle->mapotn_handle;
    handle_pool_ptr = digi_handle->handle_pool.mapper_chnl;

    /* Callback for interrupt table must be registered before enable ints */
    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_OTN_MAPPER) &&
           !digi_int_is_cb_registered(digi_handle, DIGI_INT_CHNL_TYPE_OTN_MAPPER) &&
           !digi_int_is_cb_registered(digi_handle, chnl_int_type))
        {
            result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
        }
    }


    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         (void*) chnl_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }


    /* Get internal enabled aggregate interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       int_type,
                                       (void**) &int_en_table_ptr);

    }

    /* Configure ENET/ETRANS interrupt to enable */
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        if (util_global_is_chnl_prov(&handle_pool_ptr[i]))
        {  
            cfg = FALSE;
            valid_ptr = NULL;
            if (FALSE == is_enet_pmon)
            {
                /* Enable interrupts */
                result = digi_enet_validate_parms(digi_handle,
                                                  &handle_pool_ptr[i],
                                                  &enet_ptr,
                                                  &link);
                if (PMC_SUCCESS == result && 
                    TRUE == enet_is_link_mapotn_test(enet_ptr, link, &dummy))
                {                   
                    enet_int_en_ptr = &int_en_table_ptr->mapper_chnl[i].enet_etrans;
                    valid_ptr = &int_en_table_ptr->mapper_chnl[i].enet_etrans_valid;
                    cfg = TRUE;
                }
                else
                {
                    result = PMC_SUCCESS;
                }
            }
            else
            {       
                result = digi_mapper_enet_pmon_handle_get(digi_handle,
                                                          &handle_pool_ptr[i],
                                                          &enet_ptr,
                                                          &link);
                if (PMC_SUCCESS == result &&
                    NULL != enet_ptr)
                {
                    enet_int_en_ptr = &int_en_table_ptr->mapper_chnl[i].enet_pmon;
                    valid_ptr = &int_en_table_ptr->mapper_chnl[i].enet_pmon_valid;
                    cfg = TRUE;
                }
                else
                {
                    result = PMC_SUCCESS;
                }
            }
            
            /* Skip interrupt configuration if ENET subsystem is not powered up */
            if (TRUE == cfg &&
                TRUE == enet_start_state_test(enet_ptr))
            {
                cfg = FALSE;
                result = DIGI_ERR_UNKNOWN;
            }

            if ( (TRUE == cfg) && (PMC_SUCCESS == result) )
            {
                if (TRUE == clear)
                {
                    /* clear interrupts */
                    result = enet_int_chnl_clear(enet_ptr, 
                                                 link,
                                                 int_table_ptr,
                                                 enet_int_en_ptr,
                                                 FALSE);
                }
                else
                {
                    /* Enable enet interrupts */
                    result = enet_int_chnl_enable(enet_ptr, 
                                                  link,
                                                  int_table_ptr,
                                                  enet_int_en_ptr,
                                                  enable,
                                                  FALSE);
                    
                    /* Update enet/etrans valid */
                    if (PMC_SUCCESS == result)
                    {
                        PMC_MEMSET((void*)&dummy_enet_int, 0, sizeof(enet_int_chnl_t));
                        if (0 == PMC_MEMCMP(enet_int_en_ptr, (void*)&dummy_enet_int, sizeof(enet_int_chnl_t)))
                        {
                            *valid_ptr = FALSE;
                        }
                        else
                        {
                            *valid_ptr = TRUE;
                        }       
                    } 
                }
            }
        }
    }
    PMC_RETURN(result);
} /* digi_mapper_enet_int_chnl_cfg */
#endif /* DOXYGEN_PUBLIC_ONLY */

/*
** End of file
*/
