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
*
*   NOTES:
*
*******************************************************************************/

#include "cpb_loc.h"
#include "scpb_reg_io_inline.h"
#include "qs_io_inline.h"
#include "cpb_ctrl_io_inline.h"

PUBLIC PMC_ERROR verification_FIFO_END_ADDR_CHG_REQ_poll_bit ( cpb_handle_t *cpb_handle,
                                                                UINT32 queue_system_id,
                                                                UINT32  poll_value,
                                                                UINT32 max_iterations,
                                                                UINT32 *num_iter);

PUBLIC PMC_ERROR verification_FIFO_END_ADDR_CHG_ABORT_poll_bit ( cpb_handle_t *cpb_handle,
        UINT32 queue_system_id,
        UINT32  poll_value,
        UINT32 max_iterations,
        UINT32 *num_iter);

PUBLIC PMC_ERROR verification_DCS_PAGE_SWAP_STATUS_poll_bit ( cpb_handle_t *cpb_handle, 
                                                    UINT32 dcs_inst,
                                                    UINT32  poll_value,
                                                    UINT32 max_iterations,
                                                    UINT32 *num_iter);

PUBLIC PMC_ERROR verification_FIFO_END_ADDR_CHG_REQ_poll_bit ( cpb_handle_t *cpb_handle,
        UINT32 queue_system_id, 
        UINT32  poll_value,
        UINT32 max_iterations, 
        UINT32 *num_iter)
{
    UINT32 i;
    UINT32 req_bit;
    UINT32 loop_iter = 0;
    
    PMC_ENTRY();

    for(i = 0; i < max_iterations; i++)
    {
        req_bit = qs_field_FIFO_END_ADDR_CHG_REQ_get(NULL, cpb_handle, queue_system_id);
        
        if(req_bit == poll_value)
        {
            *num_iter = loop_iter;
            PMC_RETURN(PMC_SUCCESS);
        }

        loop_iter++;

    }

    PMC_RETURN(CPB_ERR_POLL_FAIL);
}

PUBLIC PMC_ERROR verification_FIFO_END_ADDR_CHG_ABORT_poll_bit ( cpb_handle_t *cpb_handle,
        UINT32 queue_system_id,
        UINT32  poll_value,
        UINT32 max_iterations,
        UINT32 *num_iter)
{
    UINT32 i;
    UINT32 req_bit;
    UINT32 loop_iter = 0;
    
    PMC_ENTRY();

    for(i = 0; i < max_iterations; i++)
    {
        req_bit = qs_field_FIFO_END_ADDR_CHG_ABORT_get(NULL, cpb_handle, queue_system_id);
        
        if(req_bit == poll_value)
        {
            *num_iter = loop_iter;
            PMC_RETURN(PMC_SUCCESS);
        }

        loop_iter++;

    }

    PMC_RETURN(CPB_ERR_POLL_FAIL);
}

PUBLIC PMC_ERROR verification_DCS_PAGE_SWAP_STATUS_poll_bit ( cpb_handle_t *cpb_handle, 
        UINT32 dcs_inst,
        UINT32  poll_value,
        UINT32 max_iterations,
        UINT32 *num_iter)
{
    UINT32 i;
    UINT32 req_bit;
    UINT32 loop_iter = 0;
    
    PMC_ENTRY();

    for(i = 0; i < max_iterations; i++)
    {
        req_bit = dcs_field_DCS_PAGE_SWAP_STATUS_get( NULL, cpb_handle, dcs_inst );
        
        if(req_bit == poll_value)
        {
            *num_iter = loop_iter;
            PMC_RETURN(PMC_SUCCESS);
        }

        loop_iter++;

    }

    PMC_RETURN(CPB_ERR_POLL_FAIL);
    
    
}
