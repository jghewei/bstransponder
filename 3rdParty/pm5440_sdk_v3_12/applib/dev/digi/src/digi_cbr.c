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

/*
** Local Enumerated Types
*/

/*
** Local Constants
*/

/*
** Local Macro Definitions
*/
#define DIGI_CBR_PMON_TABLE_DEFINE()           \
    DIGI_CBR_PMON(sdh_pmg);                    \
    DIGI_CBR_PMON(fc1200);                     \
    DIGI_CBR_PMON(fc_cbr);                     \

/*
** Local Structures and Unions
*/

/*
** Local Variables
*/

/*
** Private Functions
*/
#ifndef DOXYGEN_PUBLIC_ONLY
PRIVATE PMC_ERROR digi_cbr_int_chnl_filt_retrieve_internal(digi_handle_t           *digi_handle,
                                                           digi_cbr_client_chnl_t  *chnl_handle,
                                                           digi_cbr_int_chnl_t     *filt_table_ptr,
                                                           BOOL8                   fw_retrieve_done,
                                                           digi_cbr_int_chnl_t     *int_table_ptr);

PRIVATE PMC_ERROR digi_cbrc_int_fw_retrieve(digi_handle_t         *digi_handle,
                                            digi_cbr_int_chnl_t   *filt_table_ptr,
                                            digi_cbr_int_t        *int_table_ptr);

PRIVATE PMC_ERROR digi_cbrc_int_fw_chnl_retrieve(digi_handle_t          *digi_handle,
                                                 digi_cbr_client_chnl_t *chnl_handle,
                                                 digi_cbr_int_chnl_t    *filt_table_ptr,
                                                 digi_cbr_int_chnl_t    *int_table_ptr);

PRIVATE PMC_ERROR digi_cbrc_proxied_int_chnl_enabled_check(digi_handle_t     *digi_handle,
                                                         cbrc_handle_t     *cbrc_handle,
                                                         UINT32             link,
                                                         cbrc_int_chnl_t   *int_en_table_ptr,
                                                         BOOL              *int_found_ptr);

PRIVATE PMC_ERROR digi_cbrc_proxied_int_chnl_clear(digi_handle_t    *digi_handle,
                                                 cbrc_handle_t    *cbrc_handle,
                                                 UINT32            link,
                                                 cbrc_int_chnl_t  *int_table_ptr);


/*******************************************************************************
* FUNCTION: digi_cbr_pmon_chnl_retrieve_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves a channel PMON table with PMON counters
*   for a single channel in the subsystem.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to retrieve PMON on
*
* OUTPUTS:
*   pmon_table_ptr  - pointer to channel PMON table to store counters 
*                     for a single channel
*
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_cbr_pmon_chnl_retrieve_internal(digi_handle_t            *digi_handle,
                                                       digi_cbr_client_chnl_t   *chnl_handle,   
                                                       digi_cbr_pmon_chnl_t     *pmon_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    UINT32              link;
    digi_pmon_t        *fw_pmon_ptr;
   

    PMC_ENTRY();
    result = digi_pmon_chnl_param_check(digi_handle, chnl_handle, pmon_table_ptr); 

    if (PMC_SUCCESS == result)
    {
        result = digi_pmon_shared_mem_get(digi_handle, &fw_pmon_ptr); 
    }

    if (PMC_SUCCESS == result)
    {        
        link = digi_cbr_channel_num_get(digi_handle,
                                        chnl_handle);

        PMC_MEMSET(pmon_table_ptr, 0, sizeof(digi_cbr_pmon_chnl_t));

#ifdef DIGI_CBR_PMON 
#undef DIGI_CBR_PMON
#endif
#define DIGI_CBR_PMON(tbl_name)                                         \
        do {                                                            \
            PMC_MEMCPY(&pmon_table_ptr->tbl_name,                       \
                       &fw_pmon_ptr->cbrc_##tbl_name[link],             \
                       sizeof(pmon_table_ptr->tbl_name));               \
        } while(0)

        DIGI_CBR_PMON_TABLE_DEFINE();

    }




    
    PMC_RETURN(result);
} /* digi_cbr_pmon_chnl_retrieve_internal */

/*******************************************************************************
* FUNCTION: digi_cbr_int_chnl_enabled_check_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches enabled interrupt bits in provided channel of the subsystem.
*   Returns if interrupt is found.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle       - pointer to channel handle to check interrupts
*
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
PRIVATE PMC_ERROR digi_cbr_int_chnl_enabled_check_internal(digi_handle_t          *digi_handle,
                                                           digi_cbr_client_chnl_t *chnl_handle,
                                                           BOOL                   *int_found_ptr)
{

    PMC_ERROR                    result = PMC_SUCCESS;
    cbrc_handle_t               *cbrc_handle;
    digi_cbr_int_t              *int_en_table_ptr;
    UINT32                       start_link = 0;
    UINT32                       end_link   = 0;
    UINT32                       i;
    digi_int_type_t              int_type = DIGI_INT_TYPE_CBR;
    digi_cbr_client_chnl_def_t  *handle_pool_ptr;
    BOOL8                        multichannels;

    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == cbrc_start_state_test(digi_handle->cbrc_handle))
    {
        PMC_RETURN(PMC_SUCCESS);
    }


    cbrc_handle = digi_handle->cbrc_handle;
    
    
    /* Get handle pool ptr */
    handle_pool_ptr  = digi_handle->handle_pool.cbr_chnl;

    /* Determine if are modfiying one channel or all channels */
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
                                       DIGI_INT_TYPE_CBR,
                                       (void**) &int_en_table_ptr);

    }

    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result) && (FALSE == *int_found_ptr); i++)
    {
        if (TRUE == multichannels)
        {
            chnl_handle = &handle_pool_ptr[i];
        }

        if (util_global_is_chnl_prov(chnl_handle))
        {
            result = cbrc_int_chnl_enabled_check(cbrc_handle, 
                                                 i,
                                                 &int_en_table_ptr->cbrc_chnl[i].cbrc,
                                                 int_found_ptr);
        }
    }


    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result) && (FALSE == *int_found_ptr); i++)
    {
        if (TRUE == multichannels)
        {
            chnl_handle = &handle_pool_ptr[i];
        }

        if (util_global_is_chnl_prov(chnl_handle))
        {
            result = digi_cbrc_proxied_int_chnl_enabled_check(digi_handle, 
                                                             cbrc_handle,
                                                             i,
                                                             &int_en_table_ptr->cbrc_chnl[i].cbrc,
                                                             int_found_ptr);
        }
    }

#ifdef PMC_SW_SIMULATION
        /* Force found interrupt for simulation so that callback runs */
        *int_found_ptr = TRUE;
#endif


    PMC_RETURN(result);
} /* digi_cbr_int_chnl_enabled_check_internal */


/*******************************************************************************
* FUNCTION: digi_cbr_int_summary_get_internal()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves summary of CBR interrupts
*   
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   unused          - Unused
*
* OUTPUTS:
*   int_summary_ptr - Pointer to interrupt summary structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_cbr_int_summary_get_internal(digi_handle_t            *digi_handle,
                                                    UINT32                    unused,
                                                    digi_cbr_int_summary_t   *int_summary_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    UINT32                       start_link = 0;
    UINT32                       end_link = 0;
    UINT32                       i, j;
    digi_cbr_client_chnl_def_t  *handle_pool_ptr;
    digi_cbr_client_chnl_t      *chnl_handle;
    BOOL                         int_found;
    digi_int_type_t              int_type = DIGI_INT_TYPE_CBR;

    PMC_ENTRY();

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
    if (NULL == int_summary_ptr)
    {
        result = DIGI_ERR_NULL_HANDLE;
    }

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == cbrc_start_state_test(digi_handle->cbrc_handle))
    {
        PMC_RETURN(PMC_SUCCESS);
    }


    /* Get handle pool ptr */
    handle_pool_ptr  = digi_handle->handle_pool.cbr_chnl;
    
    


    /* Get range of handles */ 
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }



    /* Find channels with active interrupts */
    if (PMC_SUCCESS == result)
    {
        j = 0;
        for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
        {
            int_found = FALSE;
            chnl_handle = &handle_pool_ptr[i];
    
            if (util_global_is_chnl_prov(chnl_handle))
            {
                result = digi_cbr_int_chnl_enabled_check_internal(digi_handle,
                                                                  chnl_handle,
                                                                  &int_found);
    
    
                if ((TRUE == int_found) && (PMC_SUCCESS == result))
                {
                    int_summary_ptr->active_chnl[j] = chnl_handle;
                    j++;
                }
            }
        }

        int_summary_ptr->active_chnl_num = j;
    
        int_summary_ptr->mask = DIGI_CBR_INT_SUMMARY_MASK_ACTIVE_CHNL_NUM |
                                DIGI_CBR_INT_SUMMARY_MASK_ACTIVE_CHNL;
    }

    PMC_RETURN(result);
} /* digi_cbr_int_summary_get_internal */

/*******************************************************************************
* FUNCTION: digi_cbrc_proxied_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears FW proxied cbrc interrupts based on bits that are set in the 
*   channel interrupt table.
*   
*
* INPUTS:
*   digi_handle       - Pointer to DIGI handle instance
*   cbrc_handle       - Pointer to subsystem handle
*   link              - Post-mld link (0..11)
*   int_table_ptr     - pointer to channel interrupt table with
*                       bits set to clear interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_cbrc_proxied_int_chnl_clear(digi_handle_t    *digi_handle,
                                             cbrc_handle_t    *cbrc_handle,
                                             UINT32            link,
                                             cbrc_int_chnl_t  *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    if ((NULL == int_table_ptr || TRUE == int_table_ptr->fc1200_pcs_rx_rf_i) && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_clear(digi_handle,
                                                     INTR_IPI_CBRC_FC1200_PCS_RX_RF,
                                                     link);
    }


    if ((NULL == int_table_ptr || TRUE == int_table_ptr->fc1200_pcs_rx_lf_i) && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_clear(digi_handle,
                                                     INTR_IPI_CBRC_FC1200_PCS_RX_LF,
                                                     link);
    }


    if ((NULL == int_table_ptr || TRUE == int_table_ptr->fc1200_pcs_rx_loss_sync_i) && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_clear(digi_handle,
                                                     INTR_IPI_CBRC_FC1200_PCS_RX_LOSS_SYNC,
                                                     link);
    }


    if ((NULL == int_table_ptr || TRUE == int_table_ptr->fc1200_pcs_rx_high_ber_i) && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_clear(digi_handle,
                                                     INTR_IPI_CBRC_FC1200_PCS_RX_HIGH_BER,
                                                     link);
    }


    if ((NULL == int_table_ptr || TRUE == int_table_ptr->fc1200_pcs_rx_los_i) && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_clear(digi_handle,
                                                     INTR_IPI_CBRC_FC1200_PCS_RX_LOS,
                                                     link);
    }


    if ((NULL == int_table_ptr || TRUE == int_table_ptr->fc1200_pmon_rx.los_int_i) && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_clear(digi_handle,
                                                     INTR_IPI_CBRC_FC1200_PMON_ILVR_LOS_INT,
                                                     link);
    }


    if ((NULL == int_table_ptr || TRUE == int_table_ptr->fc1200_pmon_rx.loss_sync_int_i) && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_clear(digi_handle,
                                                     INTR_IPI_CBRC_FC1200_PMON_ILVR_LOSS_SYNC_INT,
                                                     link);
    }


    if ((NULL == int_table_ptr || TRUE == int_table_ptr->fc1200_pmon_rx.high_ber_int_i) && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_clear(digi_handle,
                                                     INTR_IPI_CBRC_FC1200_PMON_ILVR_HIGH_BER,
                                                     link);
    }


    if ((NULL == int_table_ptr || TRUE == int_table_ptr->fc1200_pmon_tx.los_int_i) && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_clear(digi_handle,
                                                     INTR_IPI_CBRC_FC1200_PMON_DLVR_LOS_INT,
                                                     link);
    }


    if ((NULL == int_table_ptr || TRUE == int_table_ptr->fc1200_pmon_tx.loss_sync_int_i ) && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_clear(digi_handle,
                                                     INTR_IPI_CBRC_FC1200_PMON_DLVR_LOSS_SYNC_INT,
                                                     link);
    }


    if ((NULL == int_table_ptr || TRUE == int_table_ptr->fc1200_pmon_tx.high_ber_int_i) && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_clear(digi_handle,
                                                     INTR_IPI_CBRC_FC1200_PMON_DLVR_HIGH_BER,
                                                     link);
    }

    PMC_RETURN(result);
} /* digi_cbrc_proxied_int_chnl_clear */

/*******************************************************************************
* FUNCTION: digi_cbrc_proxied_int_chnl_enabled_check()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Searches for active interrupt bits based on the enabled interrupt table.
*   
*
* INPUTS:
*   digi_handle       - Pointer to DIGI handle instance
*   cbrc_handle       - Pointer to subsystem handle
*   link              - Link
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
PUBLIC PMC_ERROR digi_cbrc_proxied_int_chnl_enabled_check(digi_handle_t     *digi_handle,
                                                         cbrc_handle_t     *cbrc_handle,
                                                         UINT32             link,
                                                         cbrc_int_chnl_t   *int_en_table_ptr,
                                                         BOOL              *int_found_ptr)
{
    UINT32 value_i = 0;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();

    if ((NULL == int_en_table_ptr || TRUE == int_en_table_ptr->fc1200_pcs_rx_rf_i) && 
        FALSE == *int_found_ptr && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                                  INTR_IPI_CBRC_FC1200_PCS_RX_RF,
                                                                  link,
                                                                  &value_i);
        if (0 == value_i) {
            *int_found_ptr = FALSE; 
        }
        else {
            *int_found_ptr = TRUE;
        }
    }


    if ((NULL == int_en_table_ptr || TRUE == int_en_table_ptr->fc1200_pcs_rx_lf_i) && 
        FALSE == *int_found_ptr && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                                  INTR_IPI_CBRC_FC1200_PCS_RX_LF,
                                                                  link,
                                                                  &value_i);
        if (0 == value_i) {
            *int_found_ptr = FALSE; 
        }
        else {
            *int_found_ptr = TRUE;
        }
    }


    if ((NULL == int_en_table_ptr || TRUE == int_en_table_ptr->fc1200_pcs_rx_loss_sync_i) && 
        FALSE == *int_found_ptr && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                                  INTR_IPI_CBRC_FC1200_PCS_RX_LOSS_SYNC,
                                                                  link,
                                                                  &value_i);
        if (0 == value_i) {
            *int_found_ptr = FALSE; 
        }
        else {
            *int_found_ptr = TRUE;
        }
    }


    if ((NULL == int_en_table_ptr || TRUE == int_en_table_ptr->fc1200_pcs_rx_high_ber_i) && 
        FALSE == *int_found_ptr && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                                  INTR_IPI_CBRC_FC1200_PCS_RX_HIGH_BER,
                                                                  link,
                                                                  &value_i);
        if (0 == value_i) {
            *int_found_ptr = FALSE; 
        }
        else {
            *int_found_ptr = TRUE;
        }
    }


    if ((NULL == int_en_table_ptr || TRUE == int_en_table_ptr->fc1200_pcs_rx_los_i) && 
        FALSE == *int_found_ptr && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                                  INTR_IPI_CBRC_FC1200_PCS_RX_LOS,
                                                                  link,
                                                                  &value_i);
        if (0 == value_i) {
            *int_found_ptr = FALSE; 
        }
        else {
            *int_found_ptr = TRUE;
        }
    }


    if ((NULL == int_en_table_ptr || TRUE == int_en_table_ptr->fc1200_pmon_rx.los_int_i) && 
        FALSE == *int_found_ptr && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                                  INTR_IPI_CBRC_FC1200_PMON_ILVR_LOS_INT,
                                                                  link,
                                                                  &value_i);
        if (0 == value_i) {
            *int_found_ptr = FALSE; 
        }
        else {
            *int_found_ptr = TRUE;
        }
    }


    if ((NULL == int_en_table_ptr || TRUE == int_en_table_ptr->fc1200_pmon_rx.loss_sync_int_i) && 
        FALSE == *int_found_ptr && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                                  INTR_IPI_CBRC_FC1200_PMON_ILVR_LOSS_SYNC_INT,
                                                                  link,
                                                                  &value_i);
        if (0 == value_i) {
            *int_found_ptr = FALSE; 
        }
        else {
            *int_found_ptr = TRUE;
        }
    }


    if ((NULL == int_en_table_ptr || TRUE == int_en_table_ptr->fc1200_pmon_rx.high_ber_int_i) && 
        FALSE == *int_found_ptr && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                                  INTR_IPI_CBRC_FC1200_PMON_ILVR_HIGH_BER,
                                                                  link,
                                                                  &value_i);
        if (0 == value_i) {
            *int_found_ptr = FALSE; 
        }
        else {
            *int_found_ptr = TRUE;
        }
    }


    if ((NULL == int_en_table_ptr || TRUE == int_en_table_ptr->fc1200_pmon_tx.los_int_i) && 
        FALSE == *int_found_ptr && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                                  INTR_IPI_CBRC_FC1200_PMON_DLVR_LOS_INT,
                                                                  link,
                                                                  &value_i);
        if (0 == value_i) {
            *int_found_ptr = FALSE; 
        }
        else {
            *int_found_ptr = TRUE;
        }
    }


    if ((NULL == int_en_table_ptr || TRUE == int_en_table_ptr->fc1200_pmon_tx.loss_sync_int_i) && 
        FALSE == *int_found_ptr && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                                  INTR_IPI_CBRC_FC1200_PMON_DLVR_LOSS_SYNC_INT,
                                                                  link,
                                                                  &value_i);
        if (0 == value_i) {
            *int_found_ptr = FALSE; 
        }
        else {
            *int_found_ptr = TRUE;
        }
    }


    if ((NULL == int_en_table_ptr || TRUE == int_en_table_ptr->fc1200_pmon_tx.high_ber_int_i) && 
        FALSE == *int_found_ptr && 
        PMC_SUCCESS == result) 
    {
        result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                                  INTR_IPI_CBRC_FC1200_PMON_DLVR_HIGH_BER,
                                                                  link,
                                                                  &value_i);
        if (0 == value_i) {
            *int_found_ptr = FALSE; 
        }
        else {
            *int_found_ptr = TRUE;
        }
    }

    PMC_RETURN(result);
} /* digi_cbrc_proxied_int_chnl_enabled_check */

/*******************************************************************************
* FUNCTION: digi_cbr_int_enabled_check_internal()
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
*   digi_handle     - pointer to DIGI handle instance.
*
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_enabled_check_internal(digi_handle_t *digi_handle)
{
    BOOL                int_found = FALSE;
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_TYPE_CBR;

    PMC_ENTRY();
    
    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == cbrc_start_state_test(digi_handle->cbrc_handle))
    {
        PMC_RETURN(PMC_SUCCESS);
    }

    if ((PMC_SUCCESS == result) &&
        (FALSE == digi_int_is_cb_registered(digi_handle, int_type)))
    {
        result = DIGI_ERR_INT_MUST_REG_CB_BEFORE_ENABLE;
    }


    if (PMC_SUCCESS == result)
    {
        result = digi_cbr_int_chnl_enabled_check_internal(digi_handle,
                                                          DIGI_INT_ALL_CHNLS,
                                                          &int_found);
    }

    if ((PMC_SUCCESS == result) &&
        (TRUE == int_found))
    {
        result = digi_int_callback_run(digi_handle, DIGI_INT_ALL_CHNLS, DIGI_INT_TYPE_CBR);
    }

    PMC_RETURN(result);
} /* digi_cbr_int_enabled_check_internal */


#endif /* DOXYGEN_PUBLIC_ONLY */



/*
** Public Functions
*/

#ifndef DOXYGEN_PUBLIC_ONLY
/*******************************************************************************
* FUNCTION: digi_cbr_channel_num_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the CBR channel number from the channel handle.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *chnl_handle           - pointer to channel handle.
*
* OUTPUTS:
*
* RETURNS:
*   link        - link number
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 digi_cbr_channel_num_get(digi_handle_t            *digi_handle,
                                       digi_cbr_client_chnl_t   *chnl_handle)
{
    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;
    PMC_ERROR        result;

    PMC_ENTRY();

    result = digi_cbr_validate_params(digi_handle, chnl_handle, &cbrc_handle, &link);

    PMC_ASSERT(PMC_SUCCESS == result, DIGI_ERR_CODE_ASSERT, 0, 0);

    PMC_RETURN(link);
} /* digi_cbr_channel_num_get */
#endif /* DOXYGEN_PUBLIC_ONLY */



/*******************************************************************************
* FUNCTION: digi_cbr_sdh_pn11_ins_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configures if PN11 is inserted into the datapath when an LOS or LOF occurs.
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  *chnl_ctxt_ptr     - pointer to channel context pointer
*   dir               - Direction of traffic
*   lof_los           - Configure insertion PN11 on LOF and/or LOS
*   ins_enable        - Enable or disable PN11 insertion
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_pn11_ins_set(digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           cbrc_port_dir_t           dir,
                                           sdh_pmg_pn11_insert_t     lof_los,
                                           BOOL                      ins_enable)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        /* Configure the SDH_PMG */
        result = cbrc_sdh_pmg_pn11_ins(cbrc_handle, dir, link, lof_los, ins_enable);
    } /* end if */

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_pn11_ins */




/*******************************************************************************
* FUNCTION: digi_cbr_sdh_pn11_ins_set_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Retrieves if PN11 is inserted into the datapath when an LOS or LOF occurs.
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  *chnl_ctxt_ptr     - pointer to channel context pointer
*   dir               - Direction of traffic
*   lof_los           - Configure insertion PN11 on LOF and/or LOS
*
* OUTPUTS:
*  *ins_enable_ptr    - PN11 insertion is enabled or disabled
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_pn11_ins_set_get(digi_handle_t            *digi_handle,
                                               digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                               cbrc_port_dir_t           dir,
                                               sdh_pmg_pn11_insert_t     lof_los,
                                               BOOL                     *ins_enable_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    /* Configure the SDH_PMG */
    if (PMC_SUCCESS == result)
    {
        result = cbrc_sdh_pmg_pn11_ins_get(cbrc_handle, dir, link, lof_los, ins_enable_ptr);
    } /* end if */

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_pn11_ins_get */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_oof_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Force an Out Of Frame into the next framing boundary.
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  *chnl_ctxt_ptr     - pointer to channel context pointer
*   dir               - Direction of traffic
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_oof_force(digi_handle_t            *digi_handle,
                                        digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                        cbrc_port_dir_t           dir)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        /* Configure the SDH_PMG */
        result = cbrc_sdh_pmg_oof_force(cbrc_handle, dir, link);
    } /* end if */

    
     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_oof_force */














/*******************************************************************************
* FUNCTION: digi_cbr_sdh_rrmp_alarm_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configures the RRMP alarm behaviour.
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  *chnl_ctxt_ptr     - pointer to channel context pointer
*   dir               - Direction of traffic
*  *alarm_cfg         - Alarm configuration structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_rrmp_alarm_cfg(digi_handle_t            *digi_handle,
                                             digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                             cbrc_port_dir_t           dir,
                                             sdh_pmg_rrmp_alarm_cfg_t *alarm_cfg)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        /* Configure the RRMP*/
        result = cbrc_sdh_pmg_rrmp_cfg(cbrc_handle, dir, link,
                                       SDH_PMG_RRMP_CFG_TYPE_ALARM,
                                       alarm_cfg);
    } /* end if */

     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_rrmp_alarm_cfg */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_rrmp_alarm_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the RRMP alarm behaviour.
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  *chnl_ctxt_ptr     - pointer to channel context pointer
*   dir               - Direction of traffic
*
* OUTPUTS:
*  *alarm_cfg         - Alarm configuration structure
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_rrmp_alarm_cfg_get(digi_handle_t            *digi_handle,
                                                 digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                 cbrc_port_dir_t           dir,
                                                 sdh_pmg_rrmp_alarm_cfg_t *alarm_cfg)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        /* Configure the RRMP*/
        result = cbrc_sdh_pmg_rrmp_cfg_get(cbrc_handle, dir, link,
                                           SDH_PMG_RRMP_CFG_TYPE_ALARM,
                                           alarm_cfg);
    } /* end if */

     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_rrmp_alarm_cfg_get */



/*******************************************************************************
* FUNCTION: digi_cbr_sdh_rrmp_scramble_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configures the RRMP overhead descrambling behaviour.
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  *chnl_ctxt_ptr     - pointer to channel context pointer
*   dir               - Direction of traffic
*  *scramble_cfg      - Scrambling configuration structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_rrmp_scramble_cfg(digi_handle_t                *digi_handle,
                                                digi_cbr_client_chnl_t       *chnl_ctxt_ptr,
                                                cbrc_port_dir_t               dir,
                                                sdh_pmg_rrmp_scramble_cfg_t  *scramble_cfg)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        /* Configure the RRMP */
        result = cbrc_sdh_pmg_rrmp_cfg(cbrc_handle, dir, link,
                                       SDH_PMG_RRMP_CFG_TYPE_SCRAMBLE,
                                       scramble_cfg);
    } 
    
     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_rrmp_scramble_cfg */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_rrmp_scramble_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Retrieves the RRMP overhead descrambling behaviour.
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  *chnl_ctxt_ptr     - pointer to channel context pointer
*   dir               - Direction of traffic
*
* OUTPUTS:
*  *scramble_cfg      - Scrambling configuration structure
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_rrmp_scramble_cfg_get(digi_handle_t                *digi_handle,
                                                    digi_cbr_client_chnl_t       *chnl_ctxt_ptr,
                                                    cbrc_port_dir_t               dir,
                                                    sdh_pmg_rrmp_scramble_cfg_t  *scramble_cfg)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        /* Configure the RRMP */
        result = cbrc_sdh_pmg_rrmp_cfg_get(cbrc_handle, dir, link,
                                           SDH_PMG_RRMP_CFG_TYPE_SCRAMBLE,
                                           scramble_cfg);
    } 
    
     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_rrmp_scramble_cfg_get */



/*******************************************************************************
* FUNCTION: digi_cbr_sdh_rrmp_framing_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configures the RRMP framing behaviour.
*
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  *chnl_ctxt_ptr     - pointer to channel context pointer
*   dir               - Direction of traffic
*  *framing_cfg       - Frame configuration structure.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_rrmp_framing_cfg(digi_handle_t               *digi_handle,
                                               digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                               cbrc_port_dir_t              dir,
                                               sdh_pmg_rrmp_framing_cfg_t  *framing_cfg)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        /* Configure the RRMP */
        result = cbrc_sdh_pmg_rrmp_cfg(cbrc_handle, dir, link,
                                       SDH_PMG_RRMP_CFG_TYPE_FRAMING,
                                       framing_cfg);
    } 
    
     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_rrmp_framing_cfg */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_rrmp_framing_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Retrieves the RRMP framing behaviour.
*
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  *chnl_ctxt_ptr     - pointer to channel context pointer
*   dir               - Direction of traffic
*
* OUTPUTS:
*  *framing_cfg       - Frame configuration structure.
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_rrmp_framing_cfg_get(digi_handle_t               *digi_handle,
                                                   digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                                   cbrc_port_dir_t              dir,
                                                   sdh_pmg_rrmp_framing_cfg_t  *framing_cfg)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        /* Configure the RRMP */
        result = cbrc_sdh_pmg_rrmp_cfg_get(cbrc_handle, dir, link,
                                           SDH_PMG_RRMP_CFG_TYPE_FRAMING,
                                           framing_cfg);
    } 
    
     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_rrmp_framing_cfg_get */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_rrmp_byte_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Retrieves RRMP extracted bytes
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  *chnl_ctxt_ptr     - pointer to channel context pointer
*   dir               - Direction of traffic
*   type              - Byte to extract
*
* OUTPUTS:
*   byte_ptr          - Extracted byte
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_rrmp_byte_get(digi_handle_t            *digi_handle,
                                            digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                            cbrc_port_dir_t           dir,
                                            sdh_pmg_rrmp_byte_type_t  type,
                                            UINT8                    *byte_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);
    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        /* Retrieve from the RRMP */
        result = cbrc_sdh_pmg_rrmp_byte_get(cbrc_handle, dir, link, type, byte_ptr);
    } 

    
     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_rrmp_byte_get */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_trmp_alarm_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Configures the TRMP alarm behaviour.
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  *chnl_ctxt_ptr     - pointer to channel context pointer
*  *alarm_cfg_ptr         - Alarm configuration structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_trmp_alarm_cfg(digi_handle_t            *digi_handle,
                                             digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                             sdh_pmg_trmp_alarm_cfg_t *alarm_cfg_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;
    digi_cbr_client_chnl_def_t  *chnl_ctxt_def_ptr = (digi_cbr_client_chnl_def_t   *)chnl_ctxt_ptr;


    PMC_ATOMIC_ENTRY(digi_handle);

    /* Check arguments */
    PMC_ASSERT(NULL != alarm_cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);


    /* check handle provisioning state */
    if (PMC_SUCCESS != result || UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED != chnl_ctxt_def_ptr->switch_data.header.prov_state)
    {
        result = DIGI_ERR_NOT_PROVISONED;
    }

    if (PMC_SUCCESS == result)
    {
        /* Configure the TRMP*/
        result = cbrc_sdh_pmg_trmp_cfg(cbrc_handle, link,
                                     SDH_PMG_TRMP_CFG_TYPE_ALARM,
                                     alarm_cfg_ptr);
    } /* end if */

     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_trmp_alarm_cfg */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_trmp_alarm_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the TRMP alarm behaviour.
*
* INPUTS:
*  *digi_handle       - pointer to DIGI handle instance
*  *chnl_ctxt_ptr     - pointer to channel context pointer
*
* OUTPUTS:
*  *alarm_cfg         - Alarm configuration structure
*
* RETURNS:
*   PMC_SUCCESS
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_trmp_alarm_cfg_get(digi_handle_t            *digi_handle,
                                                 digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                 sdh_pmg_trmp_alarm_cfg_t *alarm_cfg)
{
  PMC_ERROR        result = PMC_SUCCESS;    
  UINT32           link;
  cbrc_handle_t   *cbrc_handle;

  digi_cbr_client_chnl_def_t  *chnl_ctxt_def_ptr = (digi_cbr_client_chnl_def_t   *)chnl_ctxt_ptr; 

  PMC_ATOMIC_ENTRY(digi_handle);
    
  /* Check arguments */
  result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

  /* check handle provisioning state */
  if (PMC_SUCCESS != result || UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED != chnl_ctxt_def_ptr->switch_data.header.prov_state)
  {
      result = DIGI_ERR_NOT_PROVISONED;
  }
  

  if (PMC_SUCCESS == result)
  {
      /* Configure the TRMP*/
      result = cbrc_sdh_pmg_trmp_cfg_get(cbrc_handle, link,
                                         SDH_PMG_TRMP_CFG_TYPE_ALARM,
                                         alarm_cfg);
  } /* end if */

     

  PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_trmp_alarm_cfg_get */



/*******************************************************************************
* FUNCTION: digi_cbr_sdh_sber_byte_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Configures the byte (B1 or B2) that the SDH SBER block will use to detect
*    signal degrade and signal fail. The SBER in the SDH_PMG can only monitor
*    either the B1 or B2 byte at one time. Changing this configuration will
*    affect all channels.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*     byte             - Sets the byte that the BER will be calculated from
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_sber_byte_cfg(digi_handle_t                *digi_handle,
                                            sdh_pmg_sber_byte_t           byte)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    cbrc_handle = digi_handle->cbrc_handle;

    if (PMC_SUCCESS == result)
    {
        /* Config SBER */
        result = cbrc_sdh_pmg_sber_byte_cfg(cbrc_handle, byte);
    } 

    
     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_sber_byte_cfg */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_sber_byte_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Retrieves the configured byte (B1 or B2) that the SDH SBER block will use
*    to detect signal degrade and signal fail. 
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*
* OUTPUTS:
*    *byte_ptr         - Configured byte that the BER will be calculated from
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_sber_byte_get(digi_handle_t                *digi_handle,
                                            sdh_pmg_sber_byte_t          *byte_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    cbrc_handle = digi_handle->cbrc_handle;

    if (PMC_SUCCESS == result)
    {
        /* Config SBER */
        result = cbrc_sdh_pmg_sber_byte_get(cbrc_handle, byte_ptr);
    } 

    
     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_sber_byte_get */



/*******************************************************************************
* FUNCTION: digi_cbr_sdh_sber_sf_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables SF monitoring and sets the threshold, or disables monitoring.
*   The byte from which the BER is calculated must be configured first
*   with digi_cbr_sdh_sber_byte_cfg().
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*     ber              - SF bit error rate, or disable SF monitoring
*                        Valid inputs are:\n
*                        B1 STS-768: SDH_PMG_SBER_BER_1E_05 to SDH_PMG_SBER_BER_1E_12 \n
*                        B1 STS-192: SDH_PMG_SBER_BER_1E_05 to SDH_PMG_SBER_BER_1E_11 \n    
*                        B2 STS-768: SDH_PMG_SBER_BER_1E_04 to SDH_PMG_SBER_BER_1E_11 \n
*                        B2 STS-192: SDH_PMG_SBER_BER_1E_04 to SDH_PMG_SBER_BER_1E_11 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_sber_sf_enable(digi_handle_t            *digi_handle,
                                             digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                             cbrc_port_dir_t           dir,
                                             sdh_pmg_sber_ber_t        ber)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    

    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        /* Enable SBER */
        result = cbrc_sdh_pmg_sber_enable(cbrc_handle, dir, link, SDH_PMG_SBER_ALARM_SF, ber);
    } 

    
     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_sber_sf_enable */





/*******************************************************************************
* FUNCTION: digi_cbr_sdh_sber_sd_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables SD monitoring and sets the threshold, or disables monitoring.
*   The byte from which the BER is calculated must be configured first
*   with digi_cbr_sdh_sber_byte_cfg().
*   
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*     ber              - SD bit error rate, or disable SD monitoring
*                        Valid inputs are:\n
*                        B1 STS-768: SDH_PMG_SBER_BER_1E_05 to SDH_PMG_SBER_BER_1E_12 \n
*                        B1 STS-192: SDH_PMG_SBER_BER_1E_05 to SDH_PMG_SBER_BER_1E_11 \n    
*                        B2 STS-768: SDH_PMG_SBER_BER_1E_04 to SDH_PMG_SBER_BER_1E_11 \n
*                        B2 STS-192: SDH_PMG_SBER_BER_1E_04 to SDH_PMG_SBER_BER_1E_11 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_sber_sd_enable(digi_handle_t            *digi_handle,
                                             digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                             cbrc_port_dir_t           dir,
                                             sdh_pmg_sber_ber_t        ber)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        /* Enable SBER */
        result = cbrc_sdh_pmg_sber_enable(cbrc_handle, dir, link, SDH_PMG_SBER_ALARM_SD, ber);
    } 

    
     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_sber_sd_enable */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_sber_sd_ber_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves configured SD BER rate   
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*
* OUTPUTS:
*    *ber_ptr          - configured SD bit error rate, or disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_sber_sd_ber_get(digi_handle_t            *digi_handle,
                                              digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                              cbrc_port_dir_t           dir,
                                              sdh_pmg_sber_ber_t       *ber_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        /* Retrieve SBER */
        result = cbrc_sdh_pmg_sber_ber_get(cbrc_handle, dir, link, SDH_PMG_SBER_ALARM_SD, ber_ptr);
    } 

    
     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_sber_sd_ber_get */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_sber_sf_ber_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves configured SF BER rate   
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*
* OUTPUTS:
*    *ber_ptr          - configured SF bit error rate, or disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_sber_sf_ber_get(digi_handle_t            *digi_handle,
                                              digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                              cbrc_port_dir_t           dir,
                                              sdh_pmg_sber_ber_t       *ber_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        /* Retrieve SBER */
        result = cbrc_sdh_pmg_sber_ber_get(cbrc_handle, dir, link, SDH_PMG_SBER_ALARM_SF, ber_ptr);
    } 

    
     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_sber_sf_ber_get */



/*******************************************************************************
* FUNCTION: digi_cbr_sdh_rttp_len_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the length of the tail trace message for a specified channel
*   within the CBRC. Also configures the algorithm to be used.
*
* INPUTS:
*  *digi_handle      - pointer to DIGI handle instance
*  *chnl_ctxt_ptr    - pointer to channel context pointer
*   dir              - direction to deactivate
*   length           - length of trace message
*   algo             - Disable trace message processing, or use valid
*                      algorithm.
*
*                      1 byte messages:  SDH_PMG_RTTP_ALGO_3
*
*                      16 byte messages: SDH_PMG_RTTP_ALGO_1
*                                          SDH_PMG_RTTP_ALGO_2
*                      User must provide a 15 byte message, and the function
*                      will automatically insert a E.164 CRC in byte 16.
*
*                      64 byte messages: SDH_PMG_RTTP_ALGO_1
*                                          SDH_PMG_RTTP_ALGO_2
*                      User must provide a 62 byte trace message, and the
*                      function will automatically insert CR/LF in bytes
*                      63 and 64.
* 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_rttp_len_cfg(digi_handle_t               *digi_handle,
                                           digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                           cbrc_port_dir_t              dir,
                                           sdh_pmg_rttp_length_t        length,
                                           sdh_pmg_rttp_algo_t          algo)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        /* Configure the length */
        result = cbrc_sdh_pmg_rttp_len_cfg(cbrc_handle, dir, link, length, algo);
    } 

    
     

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_rttp_len_cfg */



/*******************************************************************************
* FUNCTION: digi_cbr_sdh_rttp_len_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Obtains the length of the tail trace message for a specified channel
*   within the CBRC.
*
* INPUTS:
*  *digi_handle      - pointer to DIGI handle instance
*  *chnl_ctxt_ptr    - pointer to channel context pointer
*   dir              - direction to deactivate
*
* OUTPUTS:
*  *length_ptr       - length of configured trace message
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_rttp_len_get(digi_handle_t               *digi_handle,
                                           digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                           cbrc_port_dir_t              dir,
                                           sdh_pmg_rttp_length_t       *length_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link UNUSED;
    cbrc_handle_t   *cbrc_handle UNUSED;
    sdh_pmg_rttp_cfg_t rttp_cfg;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_sdh_pmg_rttp_cfg_get(cbrc_handle, dir, link, &rttp_cfg);
    } 

    if (PMC_SUCCESS == result)
    {
        *length_ptr = rttp_cfg.length;
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_rttp_len_get */

/*******************************************************************************
* FUNCTION: digi_cbr_sdh_rttp_algo_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Obtains the algo of the tail trace message for a specified channel
*   within the CBRC.
*
* INPUTS:
*  *digi_handle      - pointer to DIGI handle instance
*  *chnl_ctxt_ptr    - pointer to channel context pointer
*   dir              - direction to deactivate
*
* OUTPUTS:
*  *algo_ptr         - algo of configured trace message
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_rttp_algo_get(digi_handle_t               *digi_handle,
                                           digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                           cbrc_port_dir_t              dir,
                                           sdh_pmg_rttp_algo_t         *algo_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link UNUSED;
    cbrc_handle_t   *cbrc_handle UNUSED;
    sdh_pmg_rttp_cfg_t rttp_cfg;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_sdh_pmg_rttp_cfg_get(cbrc_handle, dir, link, &rttp_cfg);
    }

    if (PMC_SUCCESS == result)
    {
        *algo_ptr = rttp_cfg.algo;
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_rttp_algo_get */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_rttp_expect_trace_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Configures the expected trace message.
*
*    digi_cbr_sdh_rttp_len_cfg() is required to be called first to configure
*    the length of the trace message and the algorithm to be used.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*    *expect_trace     - Pointer to trace message structure. Size and
*                        trace_msg must be set. Set to NULL if disabling
*                        trace message processing.
*                        
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_rttp_expect_trace_set(digi_handle_t             *digi_handle,
                                                    digi_cbr_client_chnl_t    *chnl_ctxt_ptr,
                                                    cbrc_port_dir_t            dir,
                                                    sdh_pmg_rttp_trace_t      *expect_trace)
{
    PMC_ERROR               result = PMC_SUCCESS;    
    UINT32                  link;
    cbrc_handle_t          *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }
    
    if (PMC_SUCCESS == result)
    {
        result = cbrc_sdh_pmg_rttp_expect_trace_set(cbrc_handle,
                                                    dir, link,
                                                    expect_trace);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_rttp_expect_trace_set */





/*******************************************************************************
* FUNCTION: digi_cbr_sdh_rttp_cap_trace_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Obtains the captured trace message for the channel.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*
* OUTPUTS:
*    *cap_trace        - pointer to trace message structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_rttp_cap_trace_get(digi_handle_t               *digi_handle,
                                                 digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                                 cbrc_port_dir_t              dir,
                                                 sdh_pmg_rttp_trace_t        *cap_trace)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);
    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_sdh_pmg_rttp_trace_get(cbrc_handle,
                                             dir, link,
                                             SDH_PMG_RTTP_TRACE_TYPE_CAPTURED,
                                             cap_trace);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_rttp_cap_trace_get */



/*******************************************************************************
* FUNCTION: digi_cbr_sdh_rttp_accept_trace_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Obtains the accepted trace message for the channel.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*
* OUTPUTS:
*    *accept_trace     - pointer to trace message structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_rttp_accept_trace_get(digi_handle_t               *digi_handle,
                                                    digi_cbr_client_chnl_t      *chnl_ctxt_ptr,
                                                    cbrc_port_dir_t              dir,
                                                    sdh_pmg_rttp_trace_t        *accept_trace)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);
    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_sdh_pmg_rttp_trace_get(cbrc_handle,
                                             dir, link,
                                             SDH_PMG_RTTP_TRACE_TYPE_ACCEPTED,
                                             accept_trace);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_rttp_accept_trace_get */





/*******************************************************************************
* FUNCTION: digi_cbr_sdh_rttp_expect_trace_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*   Obtains the expected trace message for the channel.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*
* OUTPUTS:
*    *expect_trace     - pointer to trace message structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_rttp_expect_trace_get(digi_handle_t              *digi_handle,
                                                    digi_cbr_client_chnl_t     *chnl_ctxt_ptr,
                                                    cbrc_port_dir_t             dir,
                                                    sdh_pmg_rttp_trace_t       *expect_trace)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);
    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_sdh_pmg_rttp_trace_get(cbrc_handle,
                                             dir, link,
                                             SDH_PMG_RTTP_TRACE_TYPE_EXPECTED,
                                             expect_trace);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_sdh_rttp_expect_trace_get */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_prbs_mon_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Configures the SDH PRBS monitor on a given channel.
*
*    Note that valid data should be received by CBRC RX channel to allow proper  
*    execution of CBRC PRBS monitor configuration, otherwise an error 
*    code would be returned.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*    *cfg_ptr          - PRBS configuration structure to configure payload
*                        mode, inversion, and enable/disable PRBS
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*   The API should be called after CBRC datapath is provisionned and activated.
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_prbs_mon_set(digi_handle_t           *digi_handle,
                                           digi_cbr_client_chnl_t  *chnl_ctxt_ptr, 
                                           sdh_pmg_prbs_cfg_t      *cfg_ptr)
{   
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;
    PMC_ERROR        rc = PMC_SUCCESS;  

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != cfg_ptr, DIGI_ERR_INVALID_ARG, 0, 0);

    /* Check arguments */
    rc = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);
    /* check handle provisioning state */
    if (PMC_SUCCESS == rc)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == rc)
    {
        rc = cbrc_sdh_prbs_set(cbrc_handle, CBRC_PORT_DIR_RX, link,  cfg_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_cbr_sdh_prbs_mon_set */

/*******************************************************************************
* FUNCTION: digi_cbr_sdh_prbs_mon_resync()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Forces PRBS monitor to resample patter.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*   The API should be called after CBRC datapath is provisionned and activated.
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_prbs_mon_resync(digi_handle_t           *digi_handle,
                                              digi_cbr_client_chnl_t  *chnl_ctxt_ptr)
{   
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;
    PMC_ERROR        rc = PMC_SUCCESS;  

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    rc = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);
    /* check handle provisioning state */
    if (PMC_SUCCESS == rc)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == rc)
    {
        rc = cbrc_sdh_prbs_resync(cbrc_handle, link);
    } 

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_cbr_sdh_prbs_resync */

/*******************************************************************************
* FUNCTION: digi_cbr_sdh_prbs_gen_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Configures the SDH PRBS generator on a given channel.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*    *cfg_ptr          - PRBS configuration structure to configure payload
*                        mode, inversion, and enable/disable PRBS
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*   The API should be called after CBRC datapath is provisionned and activated.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_prbs_gen_set(digi_handle_t           *digi_handle,
                                           digi_cbr_client_chnl_t  *chnl_ctxt_ptr, 
                                           sdh_pmg_prbs_cfg_t      *cfg_ptr)
{   
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;
    PMC_ERROR        rc = PMC_SUCCESS;  

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    rc = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == rc)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == rc) 
    {
        rc = cbrc_sdh_prbs_set(cbrc_handle, CBRC_PORT_DIR_TX, link, cfg_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_cbr_sdh_prbs_mon_set */

/*******************************************************************************
* FUNCTION: digi_cbr_sdh_prbs_error_ins()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Insert an error into the PRBS stream. The MSB of the next byte will be
*    inverted, inducing a single bit error.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*   The API should be called after CBRC datapath is provisionned and activated.
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_prbs_error_ins(digi_handle_t           *digi_handle,
                                             digi_cbr_client_chnl_t  *chnl_ctxt_ptr)
{   
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;
    PMC_ERROR        rc = PMC_SUCCESS;  

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    rc = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == rc)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == rc)
    {
        rc = cbrc_sdh_prbs_error_ins(cbrc_handle, link);
    } 

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_cbr_sdh_prbs_error_ins */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_prbs_mon_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Retrieves SDH PMG PRBS Monitor configuration on a given channel.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*
* OUTPUTS:
*    *cfg_ptr          - PRBS monitor configuration structure of configured payload
*                        mode, inversion, and enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*   The API should be called after CBRC datapath is provisionned and activated.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_prbs_mon_get(digi_handle_t           *digi_handle,
                                           digi_cbr_client_chnl_t  *chnl_ctxt_ptr,
                                           sdh_pmg_prbs_cfg_t      *cfg_ptr)
{
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;
    PMC_ERROR        rc = PMC_SUCCESS;  

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    rc = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == rc)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == rc)
    {
        rc = cbrc_sdh_prbs_get(cbrc_handle, CBRC_PORT_DIR_RX, link, FALSE, cfg_ptr);
    }  

    PMC_ATOMIC_RETURN(digi_handle,rc);

} /* digi_cbr_sdh_prbs_mon_get */



/*******************************************************************************
* FUNCTION: digi_cbr_sdh_prbs_gen_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Retrieves SDH PMG PRBS Generator configuration on a given channel.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*
* OUTPUTS:
*    *cfg_ptr          - PRBS generator configuration structure of configured payload
*                        mode, inversion, and enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*   The API should be called after CBRC datapath is provisionned and activated.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_prbs_gen_get(digi_handle_t           *digi_handle,
                                           digi_cbr_client_chnl_t  *chnl_ctxt_ptr,
                                           sdh_pmg_prbs_cfg_t      *cfg_ptr)
{
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;
    PMC_ERROR        rc = PMC_SUCCESS;  

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    rc = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == rc)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == rc)
    {
        rc = cbrc_sdh_prbs_get(cbrc_handle, CBRC_PORT_DIR_TX, link, FALSE, cfg_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,rc);

} /* digi_cbr_sdh_prbs_gen_get */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_prbs_mon_sync_status_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Retrieves the sync status on the STS-192 PRBS for a given path.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointerw
*
* OUTPUTS:
*    *status           - PRBS monitor status for given STS-1 path.
*    *err_cnt          - Errors counter value
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*   The API should be called after CBRC datapath is provisionned and activated.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_prbs_mon_sync_status_get(digi_handle_t           *digi_handle,
                                                       digi_cbr_client_chnl_t  *chnl_ctxt_ptr, 
                                                       util_patt_status_t      *status,
                                                       UINT32                  *err_cnt)
{
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;
    PMC_ERROR        rc = PMC_SUCCESS;  

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    rc = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == rc)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == rc)
    {
        rc = cbrc_sdh_prbs_status_get(cbrc_handle, link, status, err_cnt);
    } 

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_cbr_sdh_prbs_mon_sync_status_get */


/*******************************************************************************
* FUNCTION: digi_cbr_sdh_prbs_mon_lfsr_state_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*
*    Retrieves PRBS monitor LFSR state.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*
* OUTPUTS:
*    *status          - read LFSR stages state. The state all zeros
*                       could happen if a problem occurs.
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*   The API should be called after CBRC datapath is provisionned and activated.
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_prbs_mon_lfsr_state_get(digi_handle_t           *digi_handle,
                                                      digi_cbr_client_chnl_t  *chnl_ctxt_ptr, 
                                                      UINT32                   *status)
{
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;
    PMC_ERROR        rc = PMC_SUCCESS;  

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    rc = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == rc)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == rc)
    {
        rc = cbrc_sdh_prbs_mon_lfsr_state_get(cbrc_handle, link, status);
    } 

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_cbr_sdh_prbs_mon_lfsr_state_get */

/*******************************************************************************
* FUNCTION: digi_cbr_sdh_prbs_oh_ins_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    All overheads are composed of one byte expect for K1K2 overhead that 
*    are composed of two bytes.
*
*    Inserts specified overhead byte into the PRBS output.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     ins_type         - overhead byte to insert
*    *ins_value_ptr    - array of one or two bytes to insert into overhead 
*                        depending on overhead type.
*                        For K1K2 overhead, 
*                             - ins_value_ptr[0] = K1 overhead byte
*                             - ins_value_ptr[1] = K2 overhead byte
*                        For other overhead,
*                             - ins_value_ptr[0] = overhead byte
*     ins_enable       - Enable or disable the byte insertion. If FALSE, then
*                        ins_value can be NULL.
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_prbs_oh_ins_set(digi_handle_t           *digi_handle,
                                              digi_cbr_client_chnl_t  *chnl_ctxt_ptr, 
                                              sdh_pmg_prbs_oh_ins_t    ins_type,
                                              char                    *ins_value_ptr,
                                              BOOL                     ins_enable)
{
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;
    PMC_ERROR        rc = PMC_SUCCESS;  

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    rc = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == rc)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == rc)
    {
        rc = cbrc_sdh_prbs_oh_ins_set(cbrc_handle, link, ins_type, ins_value_ptr, ins_enable);
    } 

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_cbr_sdh_prbs_oh_ins_set */



/*******************************************************************************
* FUNCTION: digi_cbr_sdh_prbs_oh_ins_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*    All overheads are composed of one byte expect for K1K2 overhead that 
*    are composed of two bytes.
*
*    Retrieves specified PRBS overhead byte and insertion status.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     ins_type         - overhead byte to insert
*     
*
* OUTPUTS:
*    *ins_value_ptr    - array of one or two bytes inserted into overhead.
*                        For K1K2 overhead, 
*                             - ins_value_ptr[0] = K1 overhead byte
*                             - ins_value_ptr[1] = K2 overhead byte
*                        For other overhead,
*                             - ins_value_ptr[0] = overhead byte
*    *ins_enable_ptr   - overhead insertion status.
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sdh_prbs_oh_ins_get(digi_handle_t           *digi_handle,
                                              digi_cbr_client_chnl_t  *chnl_ctxt_ptr, 
                                              sdh_pmg_prbs_oh_ins_t    ins_type,
                                              char                    *ins_value_ptr,
                                              BOOL                    *ins_enable_ptr)
{
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;
    PMC_ERROR        rc = PMC_SUCCESS;  

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    rc = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == rc)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == rc)
    {
        rc = cbrc_sdh_prbs_oh_ins_get(cbrc_handle, link, ins_type, ins_value_ptr, ins_enable_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,rc);
} /* digi_cbr_sdh_prbs_oh_ins_get */


/*******************************************************************************
* FUNCTION: digi_cbr_fsgm_pattern_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures fault pattern to be inserted during LOS or faults.
*   
*
* INPUTS:
*    *digi_handle        - pointer to DIGI handle instance
*    *chnl_ctxt_ptr      - pointer to channel context pointer
*     dir                - direction of traffic
*     pattern            - Fault pattern to be inserted 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fsgm_pattern_cfg(digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           cbrc_port_dir_t           dir,
                                           cbr_fsgm_pattern_t        pattern)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fsgm_pattern_cfg(cbrc_handle,
                                       link,
                                       dir,
                                       pattern);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fsgm_pattern_cfg */


/*******************************************************************************
* FUNCTION: digi_cbr_fsgm_pattern_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets fault pattern to be inserted during LOS or faults.
*   
*
* INPUTS:
*    *digi_handle        - pointer to DIGI handle instance
*    *chnl_ctxt_ptr      - pointer to channel context pointer
*     dir                - direction of traffic
*
* OUTPUTS:
*    *pattern_ptr        - Configured fault pattern to be inserted 
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fsgm_pattern_cfg_get(
                                           digi_handle_t            *digi_handle,
                                           cbrc_port_dir_t           dir,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           cbr_fsgm_pattern_t       *pattern_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fsgm_pattern_cfg_get(cbrc_handle,
                                           link,
                                           dir,
                                           pattern_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fsgm_pattern_cfg_get */


/*******************************************************************************
* FUNCTION: digi_cbr_fsgm_user_fault_pattern_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Sets User Fault Pattern value to be inserted.
*   
*
* INPUTS:
*    *digi_handle        - pointer to DIGI handle instance
*    *chnl_ctxt_ptr      - pointer to channel context pointer
*     dir                - direction of traffic
*     user_pattern       - User fault pattern to configure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fsgm_user_fault_pattern_set(digi_handle_t                 *digi_handle,
                                                      digi_cbr_client_chnl_t        *chnl_ctxt_ptr,
                                                      cbrc_port_dir_t                dir,
                                                      cbr_fsgm_user_fault_pattern_t  user_pattern)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    /*
     * If EGRESS direction and the pattern is not NONE then beed to force 
     * nominal if cbrc fsgm force fault has been enabled.  The force fault
     * can be enabled prior to the setting of the pattern or vice versa.
     */
    if (PMC_SUCCESS == result && 
        CBR_FSGM_USER_FAULT_PATTERN_NONE != user_pattern &&
        CBRC_PORT_DIR_TX == dir)
    {
        BOOL force_fault;
        PMC_ERROR rc = cbrc_fsgm_fault_force_get(cbrc_handle,
                                                 link,
                                                 dir,
                                                 &force_fault);
        
        if (PMC_SUCCESS == rc && TRUE == force_fault)
        {
            digi_cbr_client_chnl_def_t *cbrc_chnl_ptr = (digi_cbr_client_chnl_def_t *)chnl_ctxt_ptr;

            if (UTIL_GLOBAL_CHNL_PROV_STATE_PROVISIONED ==
                cbrc_chnl_ptr->switch_data.header.prov_state)
            {
                rc = digi_cpb_update_nominal_replacement(digi_handle, 
                                                         0,
                                                         (util_global_switch_data_def_t *)chnl_ctxt_ptr,
                                                         DIGI_OPSA_CFG_ACTION_DEACTIVATE,
                                                         FALSE);
            }
        }
    }
        
    if (PMC_SUCCESS == result)
    {
        result = cbrc_fsgm_user_fault_pattern_set(cbrc_handle,
                                                  link,
                                                  dir,
                                                  user_pattern);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fsgm_user_fault_pattern_set */



/*******************************************************************************
* FUNCTION: digi_cbr_fsgm_user_fault_pattern_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves configured User Fault Pattern value to be inserted.
*   
*
* INPUTS:
*    *digi_handle        - pointer to DIGI handle instance
*    *chnl_ctxt_ptr      - pointer to channel context pointer
*     dir                - direction of traffic
*
* OUTPUTS:
*   user_pattern_ptr  - Configured user fault pattern
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fsgm_user_fault_pattern_get(digi_handle_t                 *digi_handle,
                                                      digi_cbr_client_chnl_t        *chnl_ctxt_ptr,
                                                      cbrc_port_dir_t                dir,
                                                      cbr_fsgm_user_fault_pattern_t *user_pattern_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fsgm_user_fault_pattern_get(cbrc_handle,
                                                  link,
                                                  dir,
                                                  user_pattern_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fsgm_user_fault_pattern_get */





/*******************************************************************************
* FUNCTION: digi_cbr_fsgm_fault_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forces fault pattern into output. Fault pattern is configured with 
*   digi_cbr_fsgm_pattern_cfg();
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*     enable           - Enable/disable
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS        - when function execution is successful otherwise a 
*                        descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fsgm_fault_force(digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                           cbrc_port_dir_t           dir,
                                           BOOL                      enable)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;
    digi_cbr_client_chnl_def_t *chnl_def_ptr = (digi_cbr_client_chnl_def_t *)chnl_ctxt_ptr;
    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, 
                                           &chnl_def_ptr->switch_data);
    }

    /*
     * If egress direction need to check the pattern to determine whether
     * forcing or removing nominal signal is required.
     * If NOS pattern and enable is false then remove nomrimal signal,
     * opsa add, or force nominal, opsa drop.
     */
    if (PMC_SUCCESS == result && CBRC_PORT_DIR_TX == dir)
    {
        /*
         * The function digi_cpb_update_nominal_replacement will check that
         * the pattern is set before calling opsa add where as the drop
         * function does not perform that check
         */
        if (FALSE == enable)
        {
            result = digi_cpb_update_nominal_replacement(digi_handle, 
                                                         chnl_def_ptr->port_uid,
                                                         &chnl_def_ptr->switch_data,
                                                         DIGI_OPSA_CFG_ACTION_ACTIVATE,
                                                         FALSE);
        }
        else
        {
            cbr_fsgm_user_fault_pattern_t pattern;
            
            result = cbrc_fsgm_user_fault_pattern_get(cbrc_handle,
                                                      link,
                                                      dir,
                                                      &pattern);
            
            if (PMC_SUCCESS == result && 
                CBR_FSGM_USER_FAULT_PATTERN_NONE != pattern)
            {
                result = digi_cpb_update_nominal_replacement(digi_handle, 
                                                             0,
                                                             &chnl_def_ptr->switch_data,
                                                             DIGI_OPSA_CFG_ACTION_DEACTIVATE,
                                                             FALSE);
            }
        }
    }
    if (PMC_SUCCESS == result)
    {
         /* For FC800 case, set CHN_FIFO_TH to 0x18 when forcing replacement pattern on egress direction.*/
         if ( (TRUE==enable) && (DIGI_CBR_CLIENT_10G_FC800 == chnl_def_ptr->mode) &&
              (CBRC_PORT_DIR_TX == dir) )
         {
             (void)cbrc_fc800_dlvr_mode_cfg(cbrc_handle,
                                            link, 
                                            CBRC_PKT_DINT_FIFO_THRESH_FC800_NOS);
         } 

         result = cbrc_fsgm_fault_force(cbrc_handle,
                                       link,
                                       dir,
                                       enable);
      
         /* For FC800 case, set CHN_FIFO_TH back to 0x11 after removeing replacement pattern insertion on egress direction.*/
         if ( (FALSE == enable) && (DIGI_CBR_CLIENT_10G_FC800 == chnl_def_ptr->mode) &&
              (CBRC_PORT_DIR_TX == dir) )
         {
             (void)cbrc_fc800_dlvr_mode_cfg(cbrc_handle,
                                            link, 
                                            CBRC_PKT_DINT_FIFO_THRESH_FC800); 
         }
         
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fsgm_fault_force */


/*******************************************************************************
* FUNCTION: digi_cbr_fsgm_fault_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if fault pattern is being forced into the output. 
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir                - direction of traffic
*
* OUTPUTS:
*    *enable_ptr       - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS          - when function execution is successful otherwise a 
*                          descriptive error is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fsgm_fault_force_get(digi_handle_t            *digi_handle,
                                               digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                               cbrc_port_dir_t           dir,
                                               BOOL                     *enable_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fsgm_fault_force_get(cbrc_handle,
                                           link,
                                           dir,
                                           enable_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fsgm_fault_force_get */


/*******************************************************************************
* FUNCTION: digi_cbr_fsgm_pn11_mon_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables PN11 monitoring. Valid for 10G and 40G modes.
*   
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*     enable           - Enable PN11 monitoring
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fsgm_pn11_mon_enable(digi_handle_t            *digi_handle,
                                               digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                               cbrc_port_dir_t           dir,
                                               BOOL                      enable)

{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fsgm_pn11_mon_enable(cbrc_handle,
                                           link,
                                           dir,
                                           enable);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fsgm_pn11_mon_enable */

/*******************************************************************************
* FUNCTION: digi_cbr_fsgm_pn11_mon_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if PN11 monitoring is enabled or disabled. 
*   
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*
* OUTPUTS:
*    *enable_ptr       - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fsgm_pn11_mon_enable_get(digi_handle_t            *digi_handle,
                                                   digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                   cbrc_port_dir_t           dir,
                                                   BOOL                     *enable_ptr)

{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fsgm_pn11_mon_enable_get(cbrc_handle,
                                               link,
                                               dir,
                                               enable_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fsgm_pn11_mon_enable_get */



/*******************************************************************************
* FUNCTION: digi_cbr_fsgm_insert_cfg()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures how patterns are inserted into the output stream.
*
* INPUTS:
*    *digi_handle        - pointer to DIGI handle instance
*    *chnl_ctxt_ptr      - pointer to channel context pointer
*     dir                - direction of traffic
*    *cfg_ptr            - pointer to pattern insertion configuration structure
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fsgm_insert_cfg(digi_handle_t                *digi_handle,
                                          digi_cbr_client_chnl_t       *chnl_ctxt_ptr,
                                          cbrc_port_dir_t               dir,
                                          cbr_fsgm_pattern_cfg_t       *cfg_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fsgm_insert_cfg(cbrc_handle,
                                      link,
                                      dir,
                                      cfg_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fsgm_insert_cfg */


/*******************************************************************************
* FUNCTION: digi_cbr_fsgm_insert_cfg_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves config for how patterns are inserted into the output stream.
*
* INPUTS:
*    *digi_handle        - pointer to DIGI handle instance
*    *chnl_ctxt_ptr      - pointer to channel context pointer
*     dir                - direction of traffic
*
* OUTPUTS:
*    *cfg_ptr            - Configured pattern insertion configuration structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fsgm_insert_cfg_get(digi_handle_t                *digi_handle,
                                              digi_cbr_client_chnl_t       *chnl_ctxt_ptr,
                                              cbrc_port_dir_t               dir,
                                              cbr_fsgm_pattern_cfg_t       *cfg_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fsgm_insert_cfg_get(cbrc_handle,
                                          link,
                                          dir,
                                          cfg_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fsgm_insert_cfg_get */







/*******************************************************************************
* FUNCTION: digi_cbr_patt_ins_fault_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forces the generation of the selected fault pattern.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*     force            - Enable the insertion of the fault pattern
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_patt_ins_fault_force(digi_handle_t            *digi_handle,
                                               digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                               cbrc_port_dir_t           dir,
                                               BOOL                      force)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_patt_ins_fault_force(cbrc_handle,
                                           link,
                                           dir,
                                           force);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_patt_ins_fault_force */


/*******************************************************************************
* FUNCTION: digi_cbr_patt_ins_fault_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if the generation of the selected fault pattern is forced.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*
* OUTPUTS:
*    *force_ptr        - Force generation is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_patt_ins_fault_force_get(digi_handle_t            *digi_handle,
                                                   digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                                   cbrc_port_dir_t           dir,
                                                   BOOL                     *force_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_patt_ins_fault_force_get(cbrc_handle,
                                               link,
                                               dir,
                                               force_ptr);
    } 

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_patt_ins_fault_force_get */




/*******************************************************************************
* FUNCTION: digi_cbr_patt_ins_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables the insertion of the fault pattern as a consequential action based
*   on configured traffic mode in channel
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*     enable           - Enables the insertion when needed
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_patt_ins_enable(digi_handle_t            *digi_handle,
                                          digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                          cbrc_port_dir_t           dir,
                                          BOOL                      enable)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_patt_ins_enable(cbrc_handle,
                                      link,
                                      dir,
                                      enable);
    } 



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_patt_ins_enable */



/*******************************************************************************
* FUNCTION: digi_cbr_patt_ins_enable_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves  if the insertion of the fault pattern as a consequential action based
*   on configured traffic mode in channel is enabled or disabled.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*
* OUTPUTS:
*    *enable_ptr       - Feature is enabled/disabled
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_patt_ins_enable_get(digi_handle_t            *digi_handle,
                                              digi_cbr_client_chnl_t   *chnl_ctxt_ptr,
                                              cbrc_port_dir_t           dir,
                                              BOOL                     *enable_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_patt_ins_enable_get(cbrc_handle,
                                          link,
                                          dir,
                                          enable_ptr);
    } 



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_patt_ins_enable_get */



/*******************************************************************************
* FUNCTION: digi_cbr_fc800_dlolb_gen_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures what defects contribute to DLOLB generation.
*
* INPUTS:
*    *digi_handle       - pointer to DIGI handle instance
*    *chnl_ctxt_ptr     - pointer to channel context pointer
*     dir               - direction of traffic
*     dlolb_gen_cfg_ptr - DLOLB generation configuration
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc800_dlolb_gen_set(digi_handle_t          *digi_handle,
                                              digi_cbr_client_chnl_t *chnl_ctxt_ptr,
                                              cbrc_port_dir_t         dir,
                                              fc800_pmg_dlolb_gen_t  *dlolb_gen_cfg_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc800_dlolb_gen_set(cbrc_handle,
                                          link,
                                          dir,
                                          dlolb_gen_cfg_ptr);
    } 



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc800_dlolb_gen_set */



/*******************************************************************************
* FUNCTION: digi_cbr_fc800_dlolb_gen_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves what defects contribute to DLOLB generation.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*
* OUTPUTS:
*   dlolb_gen_cfg_ptr  - DLOLB generation configuration
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc800_dlolb_gen_get(digi_handle_t             *digi_handle,
                                              digi_cbr_client_chnl_t    *chnl_ctxt_ptr,
                                              cbrc_port_dir_t            dir,
                                              fc800_pmg_dlolb_gen_t     *dlolb_gen_cfg_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc800_dlolb_gen_get(cbrc_handle,
                                          link,
                                          dir,
                                          dlolb_gen_cfg_ptr);
    } 



    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_cbr_fc800_dlolb_gen_get */


/*******************************************************************************
* FUNCTION: digi_cbr_pmon_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation PMON table with PMON counters 
*   over multiple channels.
*
*   digi_cbr_pmon_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   pmon_table_ptr  - pointer to aggregation PMON table to store counters 
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_pmon_retrieve(digi_handle_t       *digi_handle,
                                        digi_cbr_pmon_t     *pmon_table_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    digi_cbr_client_chnl_t      *chnl_handle;
    UINT32                       i;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_pmon_param_check(digi_handle, pmon_table_ptr); 

    if (PMC_SUCCESS == result)
    {
        PMC_MEMSET(pmon_table_ptr, 0, sizeof(digi_cbr_pmon_t));

        for (i = 0; (i < DIGI_CBRC_CHNL_MAX) && (PMC_SUCCESS == result); i++)
        {
            chnl_handle = &digi_handle->handle_pool.cbr_chnl[i];
           
            if (util_global_is_chnl_prov(chnl_handle))
            {
                pmon_table_ptr->handle[i] = chnl_handle;
                result = digi_cbr_pmon_chnl_retrieve_internal(digi_handle,
                                                              chnl_handle,
                                                              &pmon_table_ptr->cbr_chnl[i]);
            }
        } /* end for */
    }

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_pmon_retrieve */

/*******************************************************************************
* FUNCTION: digi_cbr_pmon_chnl_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a channel handle and an aggregated PMON table, retrieve a pointer
*   to the corresponding channel PMON table.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   chnl_handle     - pointer to channel handle to retrieve PMON on
*   pmon_table_ptr  - Aggregated PMON table
*
* OUTPUTS:
*   pmon_chnl_table_pptr - pointer to channel PMON table corresponding to channel
*                          handle. Set to NULL if channel has not been provisioned.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_pmon_chnl_get(digi_handle_t                *digi_handle,
                                        digi_cbr_client_chnl_t       *chnl_handle,
                                        digi_cbr_pmon_t              *pmon_table_ptr,
                                        digi_cbr_pmon_chnl_t        **pmon_chnl_table_pptr)
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
    index = digi_cbr_channel_num_get(digi_handle, chnl_handle);

    /* Set return table to channel table */
    if (pmon_table_ptr->handle[index] == chnl_handle)
    {
        *pmon_chnl_table_pptr = &pmon_table_ptr->cbr_chnl[index];
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_cbr_pmon_chnl_get */


/*******************************************************************************
* FUNCTION: digi_cbr_int_register_cb()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for aggregation interrupt table processing.
*
*   This callback will be called when an active interrupt is found in any
*   CBR channel.
*
*   The callback will be called a minimum of zero times if no active interrupts
*   are found, or a maximum of one time if there is an interrupt in at least
*   one channel.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   token           - user token to be stored and passed back to callback\n
*                     Optional, can be NULL.
*   token_size      - token size in bytes\n
*                     Optional, can be 0.
*   user_cb         - callback to be registered against this interrupt type
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_register_cb(digi_handle_t      *digi_handle,
                                          void               *token,
                                          UINT32              token_size,
                                          digi_int_cb_fcn     user_cb)
{
    PMC_ERROR            result = PMC_SUCCESS;
    digi_int_type_t      int_type = DIGI_INT_TYPE_CBR;
    
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
} /* digi_cbr_int_register_cb */


/*******************************************************************************
* FUNCTION: digi_cbr_int_chnl_register_cb()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Registers a callback for channel interrupt table processing.
*
*   For each CBR channel where an active interrupt is found, the callback will be
*   called.
*
*   The callback is called a minimum of zero times if no active interrupts are
*   found, or a maximum of the number of channels in the subsystem if there is
*   an active interrupt in each of the channels.
*   
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   token           - user token to be stored and passed back to callback\n
*                     Optional, can be NULL.
*   token_size      - token size in bytes\n
*                     Optional, can be 0.
*   user_cb         - callback to be registered against this interrupt type
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_chnl_register_cb(digi_handle_t           *digi_handle,
                                               void                    *token,
                                               UINT32                   token_size,
                                               digi_int_cb_fcn          user_cb)
{
    PMC_ERROR            result = PMC_SUCCESS;
    digi_int_type_t      int_type = DIGI_INT_CHNL_TYPE_CBR;

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
} /* digi_cbr_int_chnl_register_cb */



/*******************************************************************************
* FUNCTION: digi_cbr_int_chnl_enable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enables interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to enable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to enable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     enable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_chnl_enable(digi_handle_t            *digi_handle,
                                          digi_cbr_client_chnl_t   *chnl_handle,   
                                          cbrc_int_chnl_t          *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_TYPE_CBR;

    cbrc_handle_t      *cbrc_handle;
    UINT32              start_link = 0;
    UINT32              end_link = 0;
    UINT32              i;
    digi_cbr_int_t     *int_en_table_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);


    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == cbrc_start_state_test(digi_handle->cbrc_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }

    cbrc_handle = digi_handle->cbrc_handle;

    /* Callback for interrupt table must be registered before enable ints */
    if (PMC_SUCCESS == result)
    {
        if(!digi_int_is_cb_registered(digi_handle, DIGI_INT_TYPE_CBR) &&
           !digi_int_is_cb_registered(digi_handle, DIGI_INT_CHNL_TYPE_CBR))
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


    /* Enable interrupts */
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {

        result = cbrc_int_chnl_enable(cbrc_handle, 
                                      i,
                                      int_table_ptr,
                                      &int_en_table_ptr->cbrc_chnl[i].cbrc,
                                      DIGI_ENABLE);
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_int_chnl_enable */



/*******************************************************************************
* FUNCTION: digi_cbr_int_chnl_disable()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Disables interrupts based on bits that are set in the channel
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to disable interrupts on, or
*                     DIGI_INT_ALL_CHNLS to disable interrupts on all channels
*                     of the subsystem
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     disable interrupts
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_chnl_disable(digi_handle_t            *digi_handle,
                                           digi_cbr_client_chnl_t   *chnl_handle,   
                                           cbrc_int_chnl_t          *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_CHNL_TYPE_CBR;

    cbrc_handle_t      *cbrc_handle;
    UINT32              start_link, end_link;
    UINT32              i;
    digi_cbr_int_t     *int_en_table_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);
    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == cbrc_start_state_test(digi_handle->cbrc_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }

    cbrc_handle = digi_handle->cbrc_handle;

    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         (void*) chnl_handle,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }


    /* Get internal enabled interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       DIGI_INT_TYPE_CBR,
                                       (void**) &int_en_table_ptr);

    }

    if (PMC_SUCCESS == result)
    {
        for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
        {
            /* Disable interrupts */
            result = cbrc_int_chnl_enable(cbrc_handle, 
                                          i,
                                          int_table_ptr,
                                          &int_en_table_ptr->cbrc_chnl[i].cbrc,
                                          DIGI_DISABLE);
        }
    }
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_int_chnl_disable */




/*******************************************************************************
* FUNCTION: digi_cbr_int_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears interrupts based on bits that are set in the aggregated 
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     clear interrupts
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_clear(digi_handle_t            *digi_handle,
                                    digi_cbr_int_t           *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_TYPE_CBR;
    digi_cbr_int_t     *int_en_table_ptr;

    cbrc_handle_t      *cbrc_handle;
    cpb_handle_t       *cpb_handle;
    UINT32              start_link = 0;
    UINT32              end_link   = 0;
    UINT32              i;
    UINT32              cpb_port;
    UINT32              cpb_chnl;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);
    result = digi_int_param_check(digi_handle, DIGI_INT_ALL_CHNLS, int_type, int_table_ptr); 

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == cbrc_start_state_test(digi_handle->cbrc_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }


    cbrc_handle = digi_handle->cbrc_handle;
    cpb_handle  = digi_handle->dcpb_handle;

    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {

        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }
    
    /* Get internal enabled interrupt table to update */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       DIGI_INT_TYPE_CBR,
                                       (void**) &int_en_table_ptr);

    }

    /* Clear interrupts */
    if (IS_DIGI_INT_ALL_INTS(int_table_ptr))
    {
        result = digi_cbr_int_chnl_clear(digi_handle,
                                          DIGI_INT_ALL_CHNLS,
                                          DIGI_INT_ALL_INTS);
    }
    else
    {
        for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
        {
            if (NULL != int_table_ptr->handle[i])
            {
                result = cbrc_int_chnl_clear(cbrc_handle, 
                                             i,
                                             &int_table_ptr->cbrc_chnl[i].cbrc,
                                             &int_en_table_ptr->cbrc_chnl[i].cbrc);
                    
                if (PMC_SUCCESS == result)
                {
                    result = digi_cbrc_proxied_int_chnl_clear(digi_handle,
                                                              cbrc_handle,
                                                              i,
                                                              &int_table_ptr->cbrc_chnl[i].cbrc);

                }

                if (PMC_SUCCESS == result)
                {
                    result = digi_cpb_port_chnl_get(digi_handle, int_table_ptr->handle[i], &cpb_port, &cpb_chnl);    
                }

                /* Clear CPB interrupts */
                if (PMC_SUCCESS == result)
                {
                    result = cpb_int_chnl_clear(cpb_handle,
                                                cpb_port,
                                                cpb_chnl,      
                                                &int_table_ptr->cbrc_chnl[i].cpb,
                                                &int_en_table_ptr->cbrc_chnl[i].cpb);
                }
            }
        }
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_int_clear */


/*******************************************************************************
* FUNCTION: digi_cbr_int_chnl_clear()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Clears channel interrupts based on bits that are set in the aggregated 
*   interrupt table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to clear interrupt on
*   int_table_ptr   - pointer to channel interrupt table with bits set to
*                     clear interrupts
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_chnl_clear(digi_handle_t            *digi_handle,
                                         digi_cbr_client_chnl_t   *chnl_handle,
                                         digi_cbr_int_chnl_t      *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_CHNL_TYPE_CBR;
    digi_cbr_int_t     *int_en_table_ptr;

    cbrc_handle_t      *cbrc_handle;
    cpb_handle_t       *cpb_handle;
    UINT32              start_link = 0;
    UINT32              end_link   = 0;
    UINT32              i;
    UINT32              cpb_port;
    UINT32              cpb_chnl;
    BOOL8               multichannels;
    digi_cbr_client_chnl_def_t  *handle_pool_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);
    PMC_ASSERT(digi_handle != NULL, DIGI_ERR_NULL_HANDLE, 0, 0);  
    result = digi_int_param_check(digi_handle, DIGI_INT_ALL_CHNLS, int_type, int_table_ptr); 

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == cbrc_start_state_test(digi_handle->cbrc_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle, PMC_SUCCESS);
    }


    cbrc_handle = digi_handle->cbrc_handle;
    cpb_handle  = digi_handle->dcpb_handle;
    handle_pool_ptr = digi_handle->handle_pool.cbr_chnl;

    /* Determine if are modfiying one channel or all channels */
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
    if (PMC_SUCCESS == result)
    {
        result = digi_int_en_table_get(digi_handle,
                                       DIGI_INT_TYPE_CBR,
                                       (void**) &int_en_table_ptr);

    }

    /* Clear interrupts */
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        if (TRUE == multichannels)
        {
            chnl_handle = &handle_pool_ptr[i];
        }

        if (util_global_is_chnl_prov(chnl_handle))
        {
            if (PMC_SUCCESS == result)
            {
                result = cbrc_int_chnl_clear(cbrc_handle, 
                                             i,
                                             UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cbrc),
                                             &int_en_table_ptr->cbrc_chnl[i].cbrc);
            }

            if (PMC_SUCCESS == result)
            {
                result = digi_cbrc_proxied_int_chnl_clear(digi_handle,
                                                         cbrc_handle,
                                                         i,
                                                         UTIL_GLOBAL_INT_SAFE_GET_OFFSET(int_table_ptr, cbrc));

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
                                            &int_en_table_ptr->cbrc_chnl[i].cpb);
            }
        }
    }
    
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_int_chnl_clear */


/*******************************************************************************
* FUNCTION: digi_cbr_int_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits
*   over multiple channels.
*
*   digi_cbr_int_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_retrieve(digi_handle_t      *digi_handle,
                                       digi_cbr_int_t     *int_table_ptr)
{
    PMC_ERROR     result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);


    result = digi_cbr_int_filt_retrieve(digi_handle,
                                        DIGI_INT_ALL_INTS,
                                        int_table_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_int_retrieve */


/*******************************************************************************
* FUNCTION: digi_cbr_int_chnl_filt_retrieve()
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
*   digi_handle     - pointer to DIGI handle instance.
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
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_chnl_filt_retrieve(digi_handle_t           *digi_handle,
                                                 digi_cbr_client_chnl_t  *chnl_handle,
                                                 digi_cbr_int_chnl_t     *filt_table_ptr,
                                                 digi_cbr_int_chnl_t     *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_cbr_int_chnl_filt_retrieve_internal(digi_handle,
                                                      chnl_handle,
                                                      filt_table_ptr,
                                                      FALSE,
                                                      int_table_ptr);

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_int_chnl_filt_retrieve */



/*******************************************************************************
* FUNCTION: digi_cbr_int_filt_retrieve()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with interrupt bits
*   over multiple channels.
*
*   digi_cbr_int_chnl_get() can be used to retrieve a specific chnl table
*   from the aggregation table.
*
*   The desired interrupt/status bits to retrieved (to reduce function execution
*   time) can be specified using the filter table input.
*   
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
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
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_filt_retrieve(digi_handle_t           *digi_handle,
                                            digi_cbr_int_chnl_t     *filt_table_ptr,
                                            digi_cbr_int_t          *int_table_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    digi_int_type_t              int_type = DIGI_INT_TYPE_CBR;
    digi_cbr_client_chnl_t      *chnl_ptr;
    digi_cbr_client_chnl_def_t  *handle_pool_ptr;
    UINT32                       i;
    UINT32                       start_link = 0;
    UINT32                       end_link   = 0;

    PMC_ATOMIC_ENTRY(digi_handle);

    result = digi_int_param_check(digi_handle, DIGI_INT_ALL_CHNLS, int_type, int_table_ptr); 

    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == cbrc_start_state_test(digi_handle->cbrc_handle))
    {
        PMC_ATOMIC_RETURN(digi_handle,PMC_SUCCESS);
    }


    handle_pool_ptr = digi_handle->handle_pool.cbr_chnl;

    PMC_MEMSET(int_table_ptr, 0, sizeof(digi_cbr_int_t));


    /* Determine if are modfiying one channel or all channels */
    if (PMC_SUCCESS == result)
    {

        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }
    

    /* Retrieve FW interrupts for all channels */
    if (PMC_SUCCESS == result)
    {
        /* Retrieve interrupts */
        result = digi_cbrc_int_fw_retrieve(digi_handle,
                                           filt_table_ptr,
                                           int_table_ptr);
    }

    /* Retrieve interrupts */
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        chnl_ptr = &handle_pool_ptr[i];

        if (util_global_is_chnl_prov(chnl_ptr))
        {
            int_table_ptr->handle[i] = chnl_ptr;

            result = digi_cbr_int_chnl_filt_retrieve_internal(digi_handle,
                                                              chnl_ptr,
                                                              filt_table_ptr,
                                                              TRUE,
                                                              &int_table_ptr->cbrc_chnl[i]);
        }
    }


    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_int_filt_retrieve */




/*******************************************************************************
* FUNCTION: digi_cbr_int_summary_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves summary of CBR interrupts.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   unused          - Unused
*
* OUTPUTS:
*   int_summary_ptr - Pointer to interrupt summary structure
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_summary_get(digi_handle_t              *digi_handle,
                                          UINT32                      unused,
                                          digi_cbr_int_summary_t     *int_summary_ptr)
{
    PMC_ERROR   result;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    result = digi_cbr_int_summary_get_internal(digi_handle,
                                               unused,
                                               int_summary_ptr);
    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_int_summary_get */


/*******************************************************************************
* FUNCTION: digi_cbr_int_chnl_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Given a channel handle and an aggregated interrupt table, retrieve a pointer
*   to the corresponding channel interrupt table.
*
* INPUTS:
*   digi_handle     - Pointer to digi handle
*   chnl_handle     - pointer to channel handle to retrieve interrupt on
*   int_table_ptr   - Aggregated interrupt table
*
* OUTPUTS:
*   int_chnl_table_pptr  - pointer to channel interrupt table corresponding to channel
*                          handle. Set to NULL if channel has not been provisioned.
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_chnl_get(digi_handle_t              *digi_handle,
                                       digi_cbr_client_chnl_t     *chnl_handle,
                                       digi_cbr_int_t             *int_table_ptr,
                                       digi_cbr_int_chnl_t       **int_chnl_table_pptr)
{
    PMC_ERROR       result = PMC_SUCCESS;
    UINT32          index;

    PMC_ATOMIC_ENTRY(digi_handle);

    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != chnl_handle, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != int_table_ptr, DIGI_ERR_INVALID_ARG, 0, 0);
    PMC_ASSERT(NULL != int_chnl_table_pptr, DIGI_ERR_INVALID_ARG, 0, 0);

    /* Set return table to NULL */
    *int_chnl_table_pptr = NULL;

    /* Get index corresponding to handle */
    index = digi_cbr_channel_num_get(digi_handle, chnl_handle);

    /* Set return table to channel table */
    if (int_table_ptr->handle[index] == chnl_handle)
    {
        *int_chnl_table_pptr = &int_table_ptr->cbrc_chnl[index];
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_cbr_int_chnl_get */


/*******************************************************************************
* FUNCTION: digi_cbr_int_enabled_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves an aggregation interrupt table with enabled interrupt bits
*   over multiple channels.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*
* OUTPUTS:
*   int_table_ptr  - pointer to aggregation interrupt table to store enabled interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_enabled_get(digi_handle_t       *digi_handle,
                                          digi_cbr_int_t      *int_table_ptr)
{
    PMC_ERROR                        result = PMC_SUCCESS;
    digi_int_type_t                  int_type = DIGI_INT_TYPE_CBR;
    UINT32                           start_link = 0;
    UINT32                           end_link = 0;
    UINT32                           i;
    digi_cbr_int_t                  *int_en_table_ptr;
    digi_cbr_client_chnl_def_t      *handle_pool_ptr;
    digi_cbr_client_chnl_t          *chnl_ptr;
    
    
    PMC_ATOMIC_ENTRY(digi_handle);
    result = digi_int_param_check(digi_handle, DIGI_INT_ALL_CHNLS, int_type, int_table_ptr); 

    PMC_MEMSET(int_table_ptr, 0, sizeof(digi_cbr_int_t));
    handle_pool_ptr = digi_handle->handle_pool.cbr_chnl;





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
        /* Copy enabled table to output */
        PMC_MEMCPY(int_table_ptr, int_en_table_ptr, sizeof(*int_table_ptr));
    }

    
    for (i = start_link; (i < end_link) && (PMC_SUCCESS == result); i++)
    {
        chnl_ptr = &handle_pool_ptr[i];
        int_table_ptr->handle[i] = NULL;

        if (util_global_is_chnl_prov(chnl_ptr))
        {
            /* Store handle */
            int_table_ptr->handle[i] = chnl_ptr; 
        }
    }

    PMC_ATOMIC_RETURN(digi_handle, result);
} /* digi_cbr_int_enabled_get */


/*******************************************************************************
* FUNCTION: digi_cbr_int_enabled_check()
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
*   digi_handle     - pointer to DIGI handle instance.
*
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_enabled_check(digi_handle_t   *digi_handle)
{
    PMC_ERROR            result   = PMC_SUCCESS;

    PMC_ATOMIC_ENTRY(digi_handle);
    

    if (PMC_SUCCESS == result)
    {
        result = digi_cbr_int_enabled_check_internal(digi_handle);
    }

    

    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_int_enabled_check */

/*******************************************************************************
* FUNCTION: digi_cbr_int_chnl_enabled_check()
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
*   A callback must be registered for the interrupt type before this API can be called.
*
* INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   int_summary_ptr - If digi_cbr_int_summary_get() was run prior to 
*                     calling this function, the pointer to the interrupt summary
*                     structure can be passed in to decrease the search
*                     time for active interrupts in this function.
*                     Set this parameter to NULL if the summary was not retrieved.
*
*   
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_int_chnl_enabled_check(digi_handle_t            *digi_handle,
                                                 digi_cbr_int_summary_t   *int_summary_ptr)
{
    PMC_ERROR                    result = PMC_SUCCESS;
    UINT32                       i;
    BOOL                         int_summary_free_req = FALSE;         
    digi_int_type_t              int_type = DIGI_INT_CHNL_TYPE_CBR;
    digi_cbr_client_chnl_t      *chnl_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    PMC_ASSERT(NULL != digi_handle, DIGI_ERR_NULL_HANDLE, 0, 0);
   
    /* Skip interrupt retrieve if subsystem is not powered up */
    if (PMC_SUCCESS == result && TRUE == cbrc_start_state_test(digi_handle->cbrc_handle))
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
        int_summary_ptr = (digi_cbr_int_summary_t *) PMC_MALLOC(sizeof(digi_cbr_int_summary_t));
        PMC_MEMSET(int_summary_ptr, 0, sizeof(digi_cbr_int_summary_t));
        int_summary_free_req = TRUE;
    }
    result = digi_cbr_int_summary_get_internal(digi_handle,
                                               0, /* unused */
                                               int_summary_ptr);


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
} /* digi_cbr_int_chnl_enabled_check */

/*******************************************************************************
* digi_cbr_sfis_odd_parity_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures CBR client SFIS bits of odd parity such as 
*   there are inverted  or not.
*
*   Note that traffic will disrupt during the function execution.
*
*   Feature supported only for 40G and 100G SFI-S only.
*
* INPUTS:
*   digi_handle       - pointer to digi handle instance.
*   *chnl_ctxt_ptr    - pointer to channel context pointer.
*   dir               - direction concerned by the configuration: 
*                       >UTIL_GLOBAL_DIR_RX for SFIS RX interface odd parity bits.
*                       >UTIL_GLOBAL_DIR_TX for SFIS TX interface odd parity bits.
*                       >UTIL_GLOBAL_DIR_BOTH for SFIS RX and TX interfaces odd parity bits.
*   inv               - when TRUE all bits of odd parity are inverted.
*                       when FALSE all bits of odd parity are processed normally.
* OUTPUTS:
*   NONE.
*
* RETURNS:
*   PMC_SUCCESS when function is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sfis_odd_parity_set(digi_handle_t             *digi_handle,
                                              digi_cbr_client_chnl_t    *chnl_ctxt_ptr,
                                              util_global_direction_t    dir,
                                              BOOL8                      inv)
{
    PMC_ERROR        rc = PMC_SUCCESS;
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
        
    /* Check arguments */
    rc = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);
    if (PMC_SUCCESS == rc)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    /* call SS function that executes low-level operations */
    if (PMC_SUCCESS == rc)
    {
        rc = cbrc_sfis_odd_parity_set(cbrc_handle,
                                      link,
                                      dir,
                                      inv);
    }

    PMC_ATOMIC_RETURN(digi_handle, rc);
}/* digi_cbr_sfis_odd_parity_set */

/*******************************************************************************
* digi_cbr_sfis_odd_parity_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns CBR client SFIS odd parity status: inverted or not.
*
*   Feature supported only for 40G and 100G SFI-S only.
*
* INPUTS:
*   digi_handle         - pointer to digi handle instance.
*   *chnl_ctxt_ptr      - pointer to channel context pointer
*   dir                 - direction concerned by the configuration: RX or TX.
*
* OUTPUTS:
*   *inv_ptr            - when TRUE all bits of odd parity are inverted
*                         when FALSE all bits of odd parity are processed normally
*  
* RETURNS:
*   PMC_SUCCESS when function is successfully executed otherwise a descriptive 
*   error code is returned.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_sfis_odd_parity_get(digi_handle_t             *digi_handle,
                                              digi_cbr_client_chnl_t    *chnl_ctxt_ptr,                                          
                                              util_global_direction_t    dir,
                                              BOOL8                     *inv_ptr)
{
    PMC_ERROR        rc = PMC_SUCCESS;
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    rc = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    if (PMC_SUCCESS == rc)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    /* call SS function that executes low-level operations */
    if (PMC_SUCCESS == rc)
    {
        rc = cbrc_sfis_odd_parity_get(cbrc_handle,
                                      link,
                                      dir,
                                      inv_ptr);
    }

    PMC_ATOMIC_RETURN(digi_handle, rc);
}/* digi_cbr_sfis_odd_parity_get */

/*
** Private Functions
*/
#ifndef DOXYGEN_PUBLIC_ONLY /* Signals doxygen to not include all APIs */
/*******************************************************************************
* FUNCTION: digi_cbr_int_chnl_filt_retrieve()
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
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to retrieve interrupt on
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*   fw_retrieve_done - TRUE means fw int retrival is already done, FALSE, it 
*                      needs to be done 
*
* OUTPUTS:
*   int_table_ptr  - pointer to channel interrupt table to store interrupt
*                    information
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PRIVATE PMC_ERROR digi_cbr_int_chnl_filt_retrieve_internal(digi_handle_t           *digi_handle,
                                                          digi_cbr_client_chnl_t  *chnl_handle,
                                                          digi_cbr_int_chnl_t     *filt_table_ptr,
                                                          BOOL8                   fw_retrieve_done,
                                                          digi_cbr_int_chnl_t     *int_table_ptr)
{
    PMC_ERROR           result = PMC_SUCCESS;
    digi_int_type_t     int_type = DIGI_INT_CHNL_TYPE_CBR;         
    UINT32              start_link = 0;
    UINT32              end_link   = 0;
    UINT32              cpb_port;
    UINT32              cpb_chnl;

    PMC_ENTRY();
            
    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 
    
    if (FALSE == fw_retrieve_done) {
        PMC_MEMSET(int_table_ptr, 0, sizeof(digi_cbr_int_chnl_t));   
    }
   
    if ((PMC_SUCCESS == result) &&
        (NULL == chnl_handle))
    {
        PMC_RETURN(DIGI_ERR_NULL_HANDLE);
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

    /* Retrieve FW interrupts for channel */
    if (PMC_SUCCESS == result)
    {
        if (FALSE == fw_retrieve_done)
        {
            /* Retrieve interrupts */
            result = digi_cbrc_int_fw_chnl_retrieve(digi_handle,
                                                    chnl_handle,
                                                    filt_table_ptr,
                                                    int_table_ptr);
        }
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_int_chnl_retrieve(digi_handle->cbrc_handle, 
                                        start_link,
                                        UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cbrc),
                                        &int_table_ptr->cbrc);
    }
        
    if (PMC_SUCCESS == result)
    {
        result = digi_cpb_port_chnl_get(digi_handle, chnl_handle, &cpb_port, &cpb_chnl);    
    }

    /* Clear CPB interrupts */
    if (PMC_SUCCESS == result)
    {
        result = cpb_int_chnl_retrieve(digi_handle->dcpb_handle,
                                       cpb_port,
                                       cpb_chnl,      
                                       UTIL_GLOBAL_INT_SAFE_GET_OFFSET(filt_table_ptr, cpb),
                                       &int_table_ptr->cpb);
    }

    PMC_RETURN(result);
} /* digi_cbr_int_chnl_filt_retrieve_internal */


/*******************************************************************************
*  digi_cbrc_int_fw_retrieve
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   When RX_LOS asserted is on 10GE port, send RF ordered set on the 10GE egress direction.
*   This API will set the consequential action of:
*   RX_LOS->Tx_RF for 10GE port in Gsup7.3 GFP mapping mode. 
*  
*  INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*
*  OUTPUTS: 
*   int_table_ptr  - pointer to aggregation interrupt table to store enabled interrupt
*                    information
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   
*******************************************************************************/
PRIVATE PMC_ERROR digi_cbrc_int_fw_retrieve(digi_handle_t         *digi_handle,
                                            digi_cbr_int_chnl_t   *filt_table_ptr,
                                            digi_cbr_int_t        *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;

    digi_int_type_t int_type = DIGI_INT_TYPE_CBR;
    UINT32 start_link = 0;
    UINT32 end_link = 0;
    UINT32 link;
    UINT32 value_i;
    BOOL8 int_val;
    digi_cbr_client_chnl_def_t *handle_pool_ptr;

    PMC_ENTRY();

 
    /* Get handle pool ptr */
    handle_pool_ptr = digi_handle->handle_pool.cbr_chnl;


    /* Determine link range */
    if (PMC_SUCCESS == result)
    {
        result = digi_int_link_range_get(digi_handle,
                                         DIGI_INT_ALL_CHNLS,
                                         int_type,
                                         &start_link,
                                         &end_link);
    }


    if (digi_handle->var.fw_intr_mon_running[INTR_IPI_CBRC_FC1200] == TRUE)
    {
        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pcs_rx_rf_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PCS_RX_RF,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (link = start_link; link < end_link; ++link)
                {
                    if (util_global_is_chnl_prov(&handle_pool_ptr[link]))
                    {
                        int_val = (value_i >> link) & 0x1;
                        int_table_ptr->cbrc_chnl[link].cbrc.fc1200_pcs_rx_rf_i = int_val;
                    }
                }
            }
        }

        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pcs_rx_lf_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PCS_RX_LF,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (link = start_link; link < end_link; ++link)
                {
                    if (util_global_is_chnl_prov(&handle_pool_ptr[link]))
                    {
                        int_val = (value_i >> link) & 0x1;
                        int_table_ptr->cbrc_chnl[link].cbrc.fc1200_pcs_rx_lf_i = int_val;
                    }
                }
            }
        }

        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pcs_rx_loss_sync_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PCS_RX_LOSS_SYNC,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (link = start_link; link < end_link; ++link)
                {
                    if (util_global_is_chnl_prov(&handle_pool_ptr[link]))
                    {
                        int_val = (value_i >> link) & 0x1;
                        int_table_ptr->cbrc_chnl[link].cbrc.fc1200_pcs_rx_loss_sync_i = int_val;
                    }
                }
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pcs_rx_high_ber_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PCS_RX_HIGH_BER,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (link = start_link; link < end_link; ++link)
                {
                    if (util_global_is_chnl_prov(&handle_pool_ptr[link]))
                    {
                        int_val = (value_i >> link) & 0x1;
                        int_table_ptr->cbrc_chnl[link].cbrc.fc1200_pcs_rx_high_ber_i = int_val;
                    }
                }
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pcs_rx_los_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PCS_RX_LOS,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (link = start_link; link < end_link; ++link)
                {
                    if (util_global_is_chnl_prov(&handle_pool_ptr[link]))
                    {
                        int_val = (value_i >> link) & 0x1;
                        int_table_ptr->cbrc_chnl[link].cbrc.fc1200_pcs_rx_los_i = int_val;
                    }
                }
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pmon_rx.los_int_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PMON_ILVR_LOS_INT,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (link = start_link; link < end_link; ++link)
                {
                    if (util_global_is_chnl_prov(&handle_pool_ptr[link]))
                    {
                        int_val = (value_i >> link) & 0x1;
                        int_table_ptr->cbrc_chnl[link].cbrc.fc1200_pmon_rx.los_int_i = int_val;
                    }
                }
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pmon_rx.loss_sync_int_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PMON_ILVR_LOSS_SYNC_INT,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (link = start_link; link < end_link; ++link)
                {
                    if (util_global_is_chnl_prov(&handle_pool_ptr[link]))
                    {
                        int_val = (value_i >> link) & 0x1;
                        int_table_ptr->cbrc_chnl[link].cbrc.fc1200_pmon_rx.loss_sync_int_i = int_val;
                    }
                }
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pmon_rx.high_ber_int_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PMON_ILVR_HIGH_BER,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (link = start_link; link < end_link; ++link)
                {
                    if (util_global_is_chnl_prov(&handle_pool_ptr[link]))
                    {
                        int_val = (value_i >> link) & 0x1;
                        int_table_ptr->cbrc_chnl[link].cbrc.fc1200_pmon_rx.high_ber_int_i = int_val;
                    }
                }
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pmon_tx.los_int_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PMON_DLVR_LOS_INT,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (link = start_link; link < end_link; ++link)
                {
                    if (util_global_is_chnl_prov(&handle_pool_ptr[link]))
                    {
                        int_val = (value_i >> link) & 0x1;
                        int_table_ptr->cbrc_chnl[link].cbrc.fc1200_pmon_tx.los_int_i = int_val;
                    }
                }
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pmon_tx.loss_sync_int_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PMON_DLVR_LOSS_SYNC_INT,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (link = start_link; link < end_link; ++link)
                {
                    if (util_global_is_chnl_prov(&handle_pool_ptr[link]))
                    {
                        int_val = (value_i >> link) & 0x1;
                        int_table_ptr->cbrc_chnl[link].cbrc.fc1200_pmon_tx.loss_sync_int_i = int_val;
                    }
                }
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pmon_tx.high_ber_int_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PMON_DLVR_HIGH_BER,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                for (link = start_link; link < end_link; ++link)
                {
                    if (util_global_is_chnl_prov(&handle_pool_ptr[link]))
                    {
                        int_val = (value_i >> link) & 0x1;
                        int_table_ptr->cbrc_chnl[link].cbrc.fc1200_pmon_tx.high_ber_int_i = int_val;
                    }
                }
            }
        }
    }

    PMC_RETURN(result);
} /* End of digi_cbrc_int_fw_retrieve*/   


/*******************************************************************************
*  digi_cbrc_int_fw_chnl_retrieve
*  ___________________________________________________________________________
*
*  DESCRIPTION:
*   Retrieve the CBRC interrupts that are handled by FW. The following
*   interrupts are retrieved for the specified link:
*     INTR_IPI_CBRC_FC1200_PCS_RX_RF,
           INTR_IPI_CBRC_FC1200_PCS_RX_LF,
           INTR_IPI_CBRC_FC1200_PCS_RX_LOSS_SYNC,
           INTR_IPI_CBRC_FC1200_PCS_RX_HIGH_BER,
           INTR_IPI_CBRC_FC1200_PCS_RX_LOS,
           INTR_IPI_CBRC_FC1200_PMON_ILVR_LOS_INT,
           INTR_IPI_CBRC_FC1200_PMON_ILVR_LOSS_SYNC_INT,
           INTR_IPI_CBRC_FC1200_PMON_ILVR_HIGH_BER,
           INTR_IPI_CBRC_FC1200_PMON_DLVR_LOS_INT,
           INTR_IPI_CBRC_FC1200_PMON_DLVR_LOSS_SYNC_INT,
           INTR_IPI_CBRC_FC1200_PMON_DLVR_HIGH_BER
*  
*  INPUTS:
*   digi_handle     - pointer to DIGI handle instance.
*   chnl_handle     - pointer to channel handle to retrieve interrupt on
*   filt_table_ptr  - pointer to filter table. Mark fields as TRUE within this
*                     table to indicate that the interrupt/status will be retrieved
*                     to the output table. DIGI_INT_ALL_INTS to retrieve all 
*                     interrupt/status.
*
*  OUTPUTS: 
*   int_table_ptr  - pointer to channel interrupt table to store interrupt
*                    information
* 
*  RETURNS:
*   PMC_SUCCESS if everything went OK, number of error codes otherwise.
*
*  NOTES:
*   
*******************************************************************************/
PRIVATE PMC_ERROR digi_cbrc_int_fw_chnl_retrieve(digi_handle_t          *digi_handle,
                                                 digi_cbr_client_chnl_t *chnl_handle,
                                                 digi_cbr_int_chnl_t    *filt_table_ptr,
                                                 digi_cbr_int_chnl_t    *int_table_ptr)
{
    PMC_ERROR result = PMC_SUCCESS;        
    UINT32 link = 0;
    digi_int_type_t int_type = DIGI_INT_TYPE_CBR;
    UINT32 value_i;
    cbrc_handle_t *cbrc_handle;

    PMC_ENTRY();


    result = digi_int_param_check(digi_handle, chnl_handle, int_type, int_table_ptr); 

    if (PMC_SUCCESS == result) {
        result = digi_cbr_validate_params(digi_handle, chnl_handle, &cbrc_handle, &link);
    }

    if (digi_handle->var.fw_intr_mon_running[INTR_IPI_CBRC_FC1200] == TRUE)
    {
        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pcs_rx_rf_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PCS_RX_RF,
                                                              link,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                int_table_ptr->cbrc.fc1200_pcs_rx_rf_i = value_i;
            }
        }

        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pcs_rx_lf_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PCS_RX_LF,
                                                              link,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                int_table_ptr->cbrc.fc1200_pcs_rx_lf_i = value_i;
            }
        }

        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pcs_rx_loss_sync_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PCS_RX_LOSS_SYNC,
                                                              link,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                int_table_ptr->cbrc.fc1200_pcs_rx_loss_sync_i = value_i;
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pcs_rx_high_ber_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PCS_RX_HIGH_BER,
                                                              link,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                int_table_ptr->cbrc.fc1200_pcs_rx_high_ber_i = value_i;
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pcs_rx_los_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PCS_RX_LOS,
                                                              link,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                int_table_ptr->cbrc.fc1200_pcs_rx_los_i = value_i;
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pmon_rx.los_int_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PMON_ILVR_LOS_INT,
                                                              link,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                int_table_ptr->cbrc.fc1200_pmon_rx.los_int_i = value_i;
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pmon_rx.loss_sync_int_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PMON_ILVR_LOSS_SYNC_INT,
                                                              link,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                int_table_ptr->cbrc.fc1200_pmon_rx.loss_sync_int_i = value_i;
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pmon_rx.high_ber_int_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PMON_ILVR_HIGH_BER,
                                                              link,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                int_table_ptr->cbrc.fc1200_pmon_rx.high_ber_int_i = value_i;
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pmon_tx.los_int_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PMON_DLVR_LOS_INT,
                                                              link,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                int_table_ptr->cbrc.fc1200_pmon_tx.los_int_i = value_i;
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pmon_tx.loss_sync_int_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PMON_DLVR_LOSS_SYNC_INT,
                                                              link,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                int_table_ptr->cbrc.fc1200_pmon_tx.loss_sync_int_i = value_i;
            }
        }

        /* Read CBRC interrupts from FW */
        if (filt_table_ptr == DIGI_INT_ALL_INTS || filt_table_ptr->cbrc.fc1200_pmon_tx.high_ber_int_i == TRUE)
        {
            /* Read PCS_RX_RF */
            if (PMC_SUCCESS == result)
            {
                result = digi_fw_cbrc_intr_channelized_monitor_status_get(digi_handle,
                                                              INTR_IPI_CBRC_FC1200_PMON_DLVR_HIGH_BER,
                                                              link,
                                                              &value_i);
            }
            /* Save to interrupt table */
            if (PMC_SUCCESS == result)
            {
                int_table_ptr->cbrc.fc1200_pmon_tx.high_ber_int_i = value_i;
            }
        }
    }

    PMC_RETURN(result);
} /* End of digi_cbrc_int_fw_chnl_retrieve*/   
#endif /* DOXYGEN_PUBLIC_ONLY */



#ifdef PMC_DEPRECATED
/*******************************************************************************
* FUNCTION: digi_cbr_fc800_user_pattern_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the user defined pattern to be inserted into the datapath.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*     user_pattern     - 32-bit user pattern to be inserted
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc800_user_pattern_set(digi_handle_t             *digi_handle,
                                                 digi_cbr_client_chnl_t    *chnl_ctxt_ptr,
                                                 cbrc_port_dir_t            dir,
                                                 UINT32                     user_pattern)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc800_user_pattern_set(cbrc_handle,
                                             link,
                                             dir,
                                             user_pattern);
    } 



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc800_user_pattern_set */



/*******************************************************************************
* FUNCTION: digi_cbr_fc800_user_pattern_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves the user defined pattern to be inserted into the transmit path. 
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*
* OUTPUTS:
*     user_pattern_ptr - configured 32-bit user pattern to be inserted
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc800_user_pattern_get(digi_handle_t             *digi_handle,
                                                 digi_cbr_client_chnl_t    *chnl_ctxt_ptr,
                                                 cbrc_port_dir_t            dir,
                                                 UINT32                    *user_pattern_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc800_user_pattern_get(cbrc_handle,
                                             link,
                                             dir,
                                             user_pattern_ptr);
    } 



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc800_user_pattern_get */




/*******************************************************************************
* FUNCTION: digi_cbr_fc800_pattern_ins_set()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Configures the pattern insertion during DLOLB or fc800_pmg_pattern_force() 
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*     pattern_ins_cfg  - Source of pattern to insert (user or NOS/PN11)
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc800_pattern_ins_set(digi_handle_t                *digi_handle,
                                                digi_cbr_client_chnl_t       *chnl_ctxt_ptr,
                                                cbrc_port_dir_t               dir,
                                                fc800_pmg_pattern_ins_cfg_t   pattern_ins_cfg)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc800_pattern_ins_set(cbrc_handle,
                                            link,
                                            dir,
                                            pattern_ins_cfg);
    } 



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc800_pattern_ins_set */



/*******************************************************************************
* FUNCTION: digi_cbr_fc800_pattern_ins_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Gets the configured the pattern insertion
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
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
PUBLIC PMC_ERROR digi_cbr_fc800_pattern_ins_get(digi_handle_t                *digi_handle,
                                                digi_cbr_client_chnl_t       *chnl_ctxt_ptr,
                                                cbrc_port_dir_t               dir,
                                                fc800_pmg_pattern_ins_cfg_t  *pattern_ins_cfg_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc800_pattern_ins_get(cbrc_handle,
                                            link,
                                            dir,
                                            pattern_ins_cfg_ptr);
    } 



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc800_pattern_ins_get */





/*******************************************************************************
* FUNCTION: digi_cbr_fc800_pattern_force()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Forces the configured pattern into the datastream.
*
*   The pattern source (user pattern or NOS/PN11) can be configured with 
*   digi_cbr_fc800_pattern_ins_set().
*
*   The user pattern can be configured with digi_cbr_fc800_user_pattern_set().
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
*     force            - TRUE to force the pattern. 
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS     - On success, otherwise specific error code.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_cbr_fc800_pattern_force(digi_handle_t          *digi_handle,
                                              digi_cbr_client_chnl_t *chnl_ctxt_ptr,
                                              cbrc_port_dir_t         dir,
                                              BOOL                    force)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc800_pattern_force(cbrc_handle,
                                          link,
                                          dir,
                                          force);
    } 



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc800_pattern_force */



/*******************************************************************************
* FUNCTION: digi_cbr_fc800_pattern_force_get()
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Retrieves if the configured pattern is forced into the datastream.
*
* INPUTS:
*    *digi_handle      - pointer to DIGI handle instance
*    *chnl_ctxt_ptr    - pointer to channel context pointer
*     dir              - direction of traffic
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
PUBLIC PMC_ERROR digi_cbr_fc800_pattern_force_get(digi_handle_t             *digi_handle,
                                                  digi_cbr_client_chnl_t    *chnl_ctxt_ptr,
                                                  cbrc_port_dir_t            dir,
                                                  BOOL                      *force_ptr)
{
    PMC_ERROR        result = PMC_SUCCESS;    
    UINT32           link;
    cbrc_handle_t   *cbrc_handle;

    PMC_ATOMIC_ENTRY(digi_handle);
    
    /* Check arguments */
    result = digi_cbr_validate_params(digi_handle, chnl_ctxt_ptr, &cbrc_handle, &link);

    /* check handle provisioning state */
    if (PMC_SUCCESS == result)
    {
        DIGI_SWITCH_DATA_HANDLE_PROV_CHECK(digi_handle, & ((digi_cbr_client_chnl_def_t   *) chnl_ctxt_ptr)->switch_data);
    }

    if (PMC_SUCCESS == result)
    {
        result = cbrc_fc800_pattern_force_get(cbrc_handle,
                                              link,
                                              dir,
                                              force_ptr);
    } 



    PMC_ATOMIC_RETURN(digi_handle,result);
} /* digi_cbr_fc800_pattern_force_get */
#endif  /* PMC_DEPRECATED */



/*
** End of file
*/
