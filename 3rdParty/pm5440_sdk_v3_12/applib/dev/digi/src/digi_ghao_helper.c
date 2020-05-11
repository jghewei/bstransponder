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
*   DESCRIPTION : This file contains C functions for G.HAO LINE <-> MAP
*                 segement.
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
#define GHAO_RAMP_RATE 512000000  /* bits / s^2 */

/*
** Local Macro Definitions
*/

/*
** Local Structures and Unions
*/
/*******************************************************************************
* ENUM: digi_ghao_firmware_seg_t
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Enumerated type for decribing a firmware segment (used only for the
*   firmware adjustments)
*
* ELEMENTS:
*   DIGI_GHAO_DB_MAPOTN    - database located in MAPOTN
*   DIGI_GHAO_DB_COREOTN   - database located in COREOTN
*   DIGI_GHAO_DB_ODUKSW    - database located in ODUKSW
*   DIGI_GHAO_DB_DCPB      - database located in DCPB
*
*******************************************************************************/
typedef enum
{
    DIGI_GHAO_DB_MAPOTN  = 0,
    DIGI_GHAO_DB_COREOTN = 1,
    DIGI_GHAO_DB_ODUKSW  = 2,
    DIGI_GHAO_DB_DCPB    = 3
} digi_ghao_db_ss_t;



/*
** Local Variables
*/

/*
** Log Messaging
*/

/*
** Forward References
*/

/*
** Internal Functions
*/

PRIVATE void digi_ghao_gen_cal_from_mask(UINT32 cal_mask[3],
                                         BOOL8 cal_slots[96]);

/*
**  Exported Interface Functions - Start
*/





/*******************************************************************************
* digi_ghao_segment_id_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   *
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*
*
* OUTPUTS:
*   ghao_segment_id        - A G.HAO segment ID.
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_segment_id_get(digi_handle_t *digi_handle,
                                           UINT32 *ghao_segment_id)
{


    UINT32 i;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    for(i=0;i<MAX_NUM_GHAO_SEGMENTS;i++)
    {
        if(digi_handle->var.ghao_cfg.ghao_segments[i].segment_in_use == FALSE)
        {
            digi_handle->var.ghao_cfg.ghao_segments[i].segment_in_use = TRUE;
            *ghao_segment_id = i;
            break;
        }

    }


    if(i < MAX_NUM_GHAO_SEGMENTS)
    {
        ret_val = PMC_SUCCESS;
    }
    else
    {
        ret_val = DIGI_ERR_GHAO_SEGS_FULL;
    }

    PMC_RETURN(ret_val);

} /* digi_ghao_segment_id_get */


/*******************************************************************************
* digi_ghao_validate_segment_id
* ______________________________________________________________________________
*
* DESCRIPTION:
*   *
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - A G.HAO segment ID.
*
* OUTPUTS:
*
* RETURNS:
*   None.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_validate_segment_id(digi_handle_t *digi_handle,
                                               UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();
    
    if(ghao_segment_id > MAX_NUM_GHAO_SEGMENTS)
    {
        ret_val = DIGI_ERR_GHAO_SEG_ID_INVALID;
    }
    else if(digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].segment_in_use == FALSE)
    {
        ret_val = DIGI_ERR_GHAO_SEG_ID_INVALID;
    }

    PMC_RETURN(ret_val);

} /* digi_ghao_validate_segment_id */

/*******************************************************************************
* digi_ghao_coreotn_odu_level_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Obtains the ODU LEVEL on the COREONT channel context pointer
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   chnl_handle            - Channel context pointer for COREOTN
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   UINT32 - ODU level associated with the chnl_handle
*
* NOTES:
*
*******************************************************************************/
PUBLIC UINT32 digi_ghao_coreotn_odu_level_get(digi_handle_t                 *digi_handle,
                                              util_global_oduk_port_data_t  *chnl_handle)
{


    UINT32 odu_level;
    PMC_ENTRY();

    odu_level   = (UINT32)chnl_handle->odu_level;

    PMC_RETURN(odu_level);

} /* digi_ghao_coreotn_odu_level_get */



/*******************************************************************************
* digi_ghao_pop_line_map_ctxt
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Populates the context for a LINE<->MAP G.HAO segment
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *coreotn_ptr           - Channel context pointer for COREOTN
*   *mapotn_ptr            - Channel context pointer for MAPOTN
*   ts_target              - The desire number of triblsots
*                            for the ODUFLEX to occupy at the end of
*                            the ramp.
*   old_tribslot_mask_rx   - The initial tribslot mask of the ODUFLEX
*                            before ramp in the RX direction
*   old_tribslot_mask_tx   - The initial tribslot mask of the ODUFLEX
*                            before ramp in the TX direction
*   new_tribslot_mask_rx   - The desired tribslot mask of the ODUFLEX
*                            in the RX direction at the end of the
*                            ramp
*   new_tribslot_mask_tx   - The desired tribslot mask of the ODUFLEX
*                            in the TX direction at the end of the
*                            ramp
*   ghao_segment_id        - The id number assigned to the G.HAO
*                            segment.
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
PUBLIC PMC_ERROR digi_ghao_pop_line_map_ctxt(digi_handle_t *digi_handle,
                                             util_global_oduk_port_data_t *coreotn_ptr,
                                             util_global_oduk_port_data_t *mapotn_ptr,
                                             UINT32 ts_target,
                                             UINT32 old_tribslot_mask_rx[3],
                                             UINT32 old_tribslot_mask_tx[3],
                                             UINT32 new_tribslot_mask_rx[3],
                                             UINT32 new_tribslot_mask_tx[3],
                                             UINT32 ghao_segment_id)
{

    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 ts_start;
    UINT32 coreotn_odu_level;
    int i, j;

    UINT32 dummy_chnl_id;
    UINT32 ho_chnl_id;
    coreotn_chnl_order_t dummy_order_type;
    UINT32 dummy_num_of_recs;
    line_core_otn_db_query_res *q_result;

    PMC_ENTRY();

    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].datapath_type = DIGI_LINE_MAP;

    /*set coreotn ptr and channel in context */
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_ptr = coreotn_ptr;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_lo_chnl_id = coreotn_ptr->channel;

    /*look up the HO CHNL in coreotn and store in context */
    ret_val = coreotn_chnl_id_and_order_get(digi_handle->coreotn_handle,
                                            coreotn_ptr->channel,
                                            0, /*TX DB*/
                                            &dummy_chnl_id,
                                            &ho_chnl_id,
                                            &dummy_order_type,
                                            &dummy_num_of_recs,
                                            &q_result);
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_ho_chnl_id = ho_chnl_id;


    /*set mapotn prt and channel in context */
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mapotn_ptr = mapotn_ptr;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mapotn_chnl_id = mapotn_ptr->channel;

    /*populate coreotn odu level (required to know mux stage to operate on */
    coreotn_odu_level = digi_ghao_coreotn_odu_level_get(digi_handle,
                                                       coreotn_ptr);



    if(coreotn_odu_level == (UINT32)DIGI_ODU_STRUCT_LEVEL_MO_ODU)
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage = COREOTN_MUX_STAGE_ONE;

        if(digi_handle->var.mux_stages == DIGI_OTN_MUX_STAGES_TWO)
        {
            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru = TRUE;
        }
        else
        {
            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru = FALSE;
        }

    }
    else
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage = COREOTN_MUX_STAGE_TWO;
            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru = FALSE;
    }


    PMC_LOG_TRACE("Call digi_ghao_pop_line_map_ctxt(): ghao_segment_id=%d, mapotn_chnl_id=%d, coreotn_odu_level=%d, ghao_mux_stage=%d,mux_passthru=%d\n", ghao_segment_id, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mapotn_chnl_id, coreotn_odu_level, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage);


    ts_start = 0;
    /* count ts_start using old_tribslot_mask_rx */
    for(i = 0; i<3; i++)
    {
        for(j=0;j<32;j++)
        {
            if((old_tribslot_mask_rx[i] & (1 << j)) != 0)
            {
                ts_start++;
            }
        }
    }


    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start = ts_start;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_target = ts_target;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_xoff_thresh = (ts_start * 2) + 2;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_passthru_xoff_thresh = (ts_start * 2) + 2;


    if(ts_start < ts_target)
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_increase = TRUE;
    }
    else
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_increase = FALSE;
    }

    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_SIZE_1_RP;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_SIZE_1_RP;


    /* Calendar slot array for added/removed slots */
    digi_ghao_gen_slots_add_rem_from_masks(old_tribslot_mask_rx,
                                           new_tribslot_mask_rx,
                                           digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_trib_slots_add_rem);

    PMC_LOG_TRACE("digi_ghao_pop_line_map_ctxt(): coreotn_rx_trib_slots_add_rem_nb=%d, coreotn_rx_trib_slots_add_rem\n", ts_target-ts_start);
    for (i = 0; i < 96; ++i)
    {
        PMC_LOG_TRACE("coreotn_rx_trib_slots_add_rem[%d]: %d\n", i, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_trib_slots_add_rem[i]);
    }

    /* Calendar slot array for old slots */
    digi_ghao_gen_cal_from_mask(old_tribslot_mask_rx,
                                          digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_old_trib_slots);
    digi_ghao_gen_cal_from_mask(old_tribslot_mask_tx,
                                          digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_tx_old_trib_slots);

    /* Calendar slot array for new slots */
    digi_ghao_gen_cal_from_mask(new_tribslot_mask_rx,
                                          digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_new_trib_slots);
    digi_ghao_gen_cal_from_mask(new_tribslot_mask_tx,
                                          digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_tx_new_trib_slots);

    /* Get tribport ID */
    coreotn_tpid_get(digi_handle->coreotn_handle,
                     digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage,
                     digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_lo_chnl_id,
                     &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_tx_tpid,
                     &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_tpid);

    PMC_RETURN(ret_val);

} /* digi_ghao_pop_line_map_ctxt */



/*******************************************************************************
* digi_ghao_pop_line_line_ctxt
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Populates the context for a LINE<->LINE G.HAO segment
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *coreotn_ptr           - Channel context pointer for COREOTN
*   ts_target              - The desire number of triblsots
*                            for the ODUFLEX to occupy at the end of
*                            the ramp.
*   old_tribslot_mask_rx   - The initial tribslot mask of the ODUFLEX
*                            before ramp in the RX direction
*   old_tribslot_mask_tx   - The initial tribslot mask of the ODUFLEX
*                            before ramp in the TX direction
*   new_tribslot_mask_rx   - The desired tribslot mask of the ODUFLEX
*                            in the RX direction at the end of the
*                            ramp
*   new_tribslot_mask_tx   - The desired tribslot mask of the ODUFLEX
*                            in the TX direction at the end of the
*                            ramp
*   ghao_segment_id        - The id number assigned to the G.HAO
*                            segment.
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
PUBLIC PMC_ERROR digi_ghao_pop_line_line_ctxt(digi_handle_t              *digi_handle,
                                              util_global_oduk_port_data_t *coreotn_ptr,
                                              UINT32 ts_target,
                                              UINT32 old_tribslot_mask_rx[3],
                                              UINT32 old_tribslot_mask_tx[3],
                                              UINT32 new_tribslot_mask_rx[3],
                                              UINT32 new_tribslot_mask_tx[3],
                                              UINT32 ghao_segment_id)
{

    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 ts_start;
    UINT32 coreotn_odu_level;
    int i, j;

    UINT32 dummy_chnl_id;
    UINT32 ho_chnl_id;
    coreotn_chnl_order_t dummy_order_type;
    UINT32 dummy_num_of_recs;
    line_core_otn_db_query_res *q_result;

    PMC_ENTRY();

    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].datapath_type = DIGI_LINE_LINE;

    /*set coreotn ptr and channel in context */
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_ptr = coreotn_ptr;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_lo_chnl_id = coreotn_ptr->channel;

    /*look up the HO CHNL in coreotn and store in context */
    ret_val = coreotn_chnl_id_and_order_get(digi_handle->coreotn_handle,
                                            coreotn_ptr->channel,
                                            0, /*TX DB*/
                                            &dummy_chnl_id,
                                            &ho_chnl_id,
                                            &dummy_order_type,
                                            &dummy_num_of_recs,
                                            &q_result);


    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_ho_chnl_id = ho_chnl_id;


    /*populate coreotn odu level (required to know mux stage to operate on */
    coreotn_odu_level = digi_ghao_coreotn_odu_level_get(digi_handle,
                                                       coreotn_ptr);


    if(coreotn_odu_level == (UINT32)DIGI_ODU_STRUCT_LEVEL_MO_ODU)
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage = COREOTN_MUX_STAGE_ONE;

        if(digi_handle->var.mux_stages == DIGI_OTN_MUX_STAGES_TWO)
        {
            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru = TRUE;
        }
        else
        {
            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru = FALSE;
        }
    }
    else
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage = COREOTN_MUX_STAGE_TWO;
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru = FALSE;
    }

    PMC_LOG_TRACE("Call digi_ghao_pop_line_line_ctxt(): ghao_segment_id=%d, mapotn_chnl_id=%d, coreotn_odu_level=%d, ghao_mux_stage=%d,mux_passthru=%d\n", ghao_segment_id, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mapotn_chnl_id, coreotn_odu_level, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage);


    ts_start = 0;
    /* count ts_start using old_tribslot_mask_rx */
    for(i = 0; i<3; i++)
    {
        for(j=0;j<32;j++)
        {
            if((old_tribslot_mask_rx[i] & (1 << j)) != 0)
            {
                ts_start++;
            }
        }
    }


    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start = ts_start;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_target = ts_target;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_xoff_thresh = (ts_start * 2) + 2;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_passthru_xoff_thresh = (ts_start * 2) + 2;


    if(ts_start < ts_target)
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_increase = TRUE;
    }
    else
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_increase = FALSE;
    }

    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_SIZE_1_RP;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_SIZE_1_RP;


    /* Calendar slot array for added/removed slots */
    digi_ghao_gen_slots_add_rem_from_masks(old_tribslot_mask_rx,
                                           new_tribslot_mask_rx,
                                           digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_trib_slots_add_rem);

    PMC_LOG_TRACE("digi_ghao_pop_line_line_ctxt(): coreotn_rx_trib_slots_add_rem_nb=%d, coreotn_rx_trib_slots_add_rem\n", ts_target-ts_start);
    for (i = 0; i < 96; ++i)
    {
        PMC_LOG_TRACE("coreotn_rx_trib_slots_add_rem[%d]: %d\n", i, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_trib_slots_add_rem[i]);
    }

    /* Calendar slot array for old slots */
    digi_ghao_gen_cal_from_mask(old_tribslot_mask_rx,
                                          digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_old_trib_slots);
    digi_ghao_gen_cal_from_mask(old_tribslot_mask_tx,
                                          digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_tx_old_trib_slots);

    /* Calendar slot array for new slots */
    digi_ghao_gen_cal_from_mask(new_tribslot_mask_rx,
                                          digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_new_trib_slots);
    digi_ghao_gen_cal_from_mask(new_tribslot_mask_tx,
                                          digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_tx_new_trib_slots);

    /* Get tribport ID */
    coreotn_tpid_get(digi_handle->coreotn_handle,
                     digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage,
                     digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_lo_chnl_id,
                     &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_tx_tpid,
                     &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_tpid);

    PMC_RETURN(ret_val);

} /* digi_ghao_pop_line_line_ctxt */



/*******************************************************************************
* digi_ghao_pop_map_sifd_ctxt
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Populates the context for a MAP<->SIFD G.HAO segment
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *mapotn_ptr            - Channel context pointer for MAPOTN
*   *sifd_ptr              - Channel context pointer for SIFD
*   sifd_old_cal_mask      - Old SIFD calendar mask
*   sifd_new_cal_mask      - New SIFD calendar mask
*   ts_target              - The desire number of triblsots
*                            for the ODUFLEX to occupy at the end of
*                            the ramp.
*   ghao_segment_id        - The id number assigned to the G.HAO
*                            segment.
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
PUBLIC PMC_ERROR digi_ghao_pop_map_sifd_ctxt(digi_handle_t              *digi_handle,
                                             util_global_oduk_port_data_t *mapotn_ptr,
                                             util_global_oduk_port_data_t *sifd_ptr,
                                             UINT32 sifd_old_cal_mask[3],
                                             UINT32 sifd_new_cal_mask[3],
                                             UINT32 ts_target,
                                             UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    util_global_oduk_port_t chnl1_port_type;

    PMC_ENTRY();

    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].datapath_type = DIGI_MAP_SIFD;

    /*set mapotn prt and channel in context */
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mapotn_ptr = mapotn_ptr;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mapotn_chnl_id = mapotn_ptr->channel;
    /*set sifd prt and channel in context */
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr = sifd_ptr;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_chnl_id = sifd_ptr->channel;

    /* Don't care parameters */
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage = COREOTN_MUX_STAGE_ONE;



    ret_val = mapotn_db_chnl_num_entries_get(digi_handle->mapotn_handle,
                                             mapotn_ptr->channel,
                                             UTIL_GEN_DB_ODUKSC_DB,
                                             &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start);



    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_target = ts_target;

    if(digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start < ts_target)
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_increase = TRUE;
    }
    else
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_increase = FALSE;
    }

    chnl1_port_type = ((util_global_oduk_port_data_t*)sifd_ptr)->port_type;
    
    if((chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1 &&
       cpb_oduksw_port0_get(coreotn_oduksw_handle_get(digi_handle->coreotn_handle)) == CPB_ODUKSW_PORT0_ILKN) ||
       chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN2)
    {   
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_SIZE_1_RP;
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_SIZE_1_RP;
    }
    else
    {

    }



    PMC_RETURN(ret_val);

} /* digi_ghao_pop_map_sifd_ctxt */



/*******************************************************************************
* digi_ghao_pop_line_sifd_ctxt
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Populates the context for a LINE<->SIFD G.HAO segment
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   *coreotn_ptr           - Channel context pointer for COREOTN
*   *sifd_ptr              - Channel context pointer for SIFD
*   ts_target              - The desire number of triblsots
*                            for the ODUFLEX to occupy at the end of
*                            the ramp.
*   old_tribslot_mask_rx   - The initial tribslot mask of the ODUFLEX
*                            before ramp in the RX direction
*   old_tribslot_mask_tx   - The initial tribslot mask of the ODUFLEX
*                            before ramp in the TX direction
*   new_tribslot_mask_rx   - The desired tribslot mask of the ODUFLEX
*                            in the RX direction at the end of the
*                            ramp
*   new_tribslot_mask_tx   - The desired tribslot mask of the ODUFLEX
*                            in the TX direction at the end of the
*                            ramp
*   sifd_old_cal_mask      - Old SIFD calendar mask
*   sifd_new_cal_mask      - New SIFD calendar mask
*   ghao_segment_id        - The id number assigned to the G.HAO
*                            segment.
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
PUBLIC PMC_ERROR digi_ghao_pop_line_sifd_ctxt(digi_handle_t              *digi_handle,
                                              util_global_oduk_port_data_t *coreotn_ptr,
                                              util_global_oduk_port_data_t *sifd_ptr,
                                              UINT32 ts_target,
                                              UINT32 old_tribslot_mask_rx[3],
                                              UINT32 old_tribslot_mask_tx[3],
                                              UINT32 new_tribslot_mask_rx[3],
                                              UINT32 new_tribslot_mask_tx[3],
                                              UINT32 sifd_old_cal_mask[3],
                                              UINT32 sifd_new_cal_mask[3],
                                              UINT32 ghao_segment_id)
{

    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 ts_start;
    UINT32 coreotn_odu_level;
    UINT32 i;
    UINT32 j;

    UINT32 dummy_chnl_id;
    UINT32 ho_chnl_id;
    coreotn_chnl_order_t dummy_order_type;
    UINT32 dummy_num_of_recs;
    line_core_otn_db_query_res *q_result;

    PMC_ENTRY();

    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].datapath_type = DIGI_LINE_SIFD;

    /*set coreotn ptr and channel in context */
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_ptr = coreotn_ptr;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_lo_chnl_id = coreotn_ptr->channel;

    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr = sifd_ptr;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_chnl_id = sifd_ptr->channel;


    /*look up the HO CHNL in coreotn and store in context */
    ret_val = coreotn_chnl_id_and_order_get(digi_handle->coreotn_handle,
                                            coreotn_ptr->channel,
                                            0, /*TX DB*/
                                            &dummy_chnl_id,
                                            &ho_chnl_id,
                                            &dummy_order_type,
                                            &dummy_num_of_recs,
                                            &q_result);

    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_ho_chnl_id = ho_chnl_id;


    /*populate coreotn odu level (required to know mux stage to operate on */
    coreotn_odu_level = digi_ghao_coreotn_odu_level_get(digi_handle,
                                                       coreotn_ptr);


    if(coreotn_odu_level == (UINT32)DIGI_ODU_STRUCT_LEVEL_MO_ODU)
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage = COREOTN_MUX_STAGE_ONE;

        if(digi_handle->var.mux_stages == DIGI_OTN_MUX_STAGES_TWO)
        {
            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru = TRUE;
        }
        else
        {
            digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru = FALSE;
        }
    }
    else
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage = COREOTN_MUX_STAGE_TWO;
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru = FALSE;
    }

    PMC_LOG_TRACE("Call digi_ghao_pop_line_sifd_ctxt(): ghao_segment_id=%d, mapotn_chnl_id=%d, coreotn_odu_level=%d, ghao_mux_stage=%d,mux_passthru=%d\n", ghao_segment_id, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mapotn_chnl_id, coreotn_odu_level, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage);


    ts_start = 0;
    /* count ts_start using old_tribslot_mask_rx */
    for(i = 0; i<3; i++)
    {
        for(j=0;j<32;j++)
        {
            if((old_tribslot_mask_rx[i] & (1 << j)) != 0)
            {
                ts_start++;
            }
        }
    }


    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start = ts_start;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_target = ts_target;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_xoff_thresh = (ts_start * 2) + 2;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_passthru_xoff_thresh = (ts_start * 2) + 2;


    if(ts_start < ts_target)
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_increase = TRUE;
    }
    else
    {
        digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_increase = FALSE;
    }

    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_tx = DIGI_GHAO_LCR_SIZE_1_RP;
    digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_step_rx = DIGI_GHAO_LCR_SIZE_1_RP;


    /* Calendar slot array for added/removed slots */
    digi_ghao_gen_slots_add_rem_from_masks(old_tribslot_mask_rx,
                                           new_tribslot_mask_rx,
                                           digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_trib_slots_add_rem);

    PMC_LOG_TRACE("digi_ghao_pop_line_sifd_ctxt(): coreotn_rx_trib_slots_add_rem_nb=%d, coreotn_rx_trib_slots_add_rem\n", ts_target-ts_start);
    for (i = 0; i < 96; ++i)
    {
        PMC_LOG_TRACE("coreotn_rx_trib_slots_add_rem[%d]: %d\n", i, digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_trib_slots_add_rem[i]);
    }

    /* Calendar slot array for old slots */
    digi_ghao_gen_cal_from_mask(old_tribslot_mask_rx,
                                          digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_old_trib_slots);
    digi_ghao_gen_cal_from_mask(old_tribslot_mask_tx,
                                          digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_tx_old_trib_slots);

    /* Calendar slot array for new slots */
    digi_ghao_gen_cal_from_mask(new_tribslot_mask_rx,
                                          digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_new_trib_slots);
    digi_ghao_gen_cal_from_mask(new_tribslot_mask_tx,
                                          digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_tx_new_trib_slots);

    /* Get tribport ID */
    coreotn_tpid_get(digi_handle->coreotn_handle,
                     digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ghao_mux_stage,
                     digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_lo_chnl_id,
                     &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_tx_tpid,
                     &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_rx_tpid);


    PMC_RETURN(ret_val);

} /* digi_ghao_pop_line_sifd_ctxt */



/*******************************************************************************
* digi_ghao_pop_firmware_ctxt
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Populates the "firmware" context
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
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
PUBLIC PMC_ERROR digi_ghao_pop_firmware_ctxt(digi_handle_t *digi_handle)
{
    UINT32 i, j;
    UINT32 seg_index = 0;
    UINT32 ts_start = 0;
    UINT32 ts_target = 0;
    DOUBLE rate_start = 0;
    DOUBLE rate_target = 0;

    UINT32 cn_base_int = 0;
    UINT32 cn_base_n = 0;
    UINT32 cn_base_d = 0;
    BOOL program_cn_n_first;

    UINT32 pkt_size = 0;
    UINT32 pkt_per_int = 0;
    UINT32 pkt_per_n = 0;
    UINT32 pkt_per_d = 0;
    DOUBLE eps = 0;
    util_global_oduk_port_t chnl1_port_type;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    for(i=0;i<MAX_NUM_GHAO_SEGMENTS;i++)
    {
        if(digi_handle->var.ghao_cfg.ghao_segments[i].segment_in_use &&
           digi_handle->var.ghao_cfg.ghao_segments[i].ghao_step_tx ==  DIGI_GHAO_BWR_END_1_TSCC)
        {
            /* Calculate start & target rates */
            ts_start = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
            if (ts_start == 0)
            {
                PMC_RETURN(DIGI_ERR_INVALID_ARG);
            }
            else if (ts_start <= 8)
            {
                /* 1.24917723 Gb/s per TS*/
                rate_start = (ts_start * UTIL_GLOBAL_ODU2_TS_RATE);
            }
            else if (ts_start <= 32)
            {
                /* 1.25447054 Gb/s per TS*/
                rate_start = (ts_start * UTIL_GLOBAL_ODU3_TS_RATE);
            }
            else if (ts_start <= 80)
            {
                /* 1.301467133 Gb/s per TS*/
                rate_start = (ts_start * UTIL_GLOBAL_ODU4_TS_RATE);
            }
            else
            {
                PMC_RETURN(DIGI_ERR_INVALID_ARG);
            }

            ts_target = digi_handle->var.ghao_cfg.ghao_segments[i].ts_target;
            if (ts_target == 0)
            {
                PMC_RETURN(DIGI_ERR_INVALID_ARG);
            }
            else if (ts_target <= 8)
            {
                /* 1.24917723 Gb/s per TS*/
                rate_target = (ts_target * UTIL_GLOBAL_ODU2_TS_RATE);
            }
            else if (ts_target <= 32)
            {
                /* 1.25447054 Gb/s per TS*/
                rate_target = (ts_target * UTIL_GLOBAL_ODU3_TS_RATE);
            }
            else if (ts_target <= 80)
            {
                /* 1.301467133 Gb/s per TS*/
                rate_target = (ts_target * UTIL_GLOBAL_ODU4_TS_RATE);
            }
            else
            {
                PMC_RETURN(DIGI_ERR_INVALID_ARG);
            }


            if(digi_handle->var.ghao_cfg.ghao_segments[i].datapath_type == DIGI_LINE_MAP)
            {


                /* calculate new opsa parameters based on new client rate */
                if(ret_val == PMC_SUCCESS)
                {

                    /*use OIF calculator for LINE<->MAP paths*/
                    ret_val = coreotn_oif_pkt_period_calc(digi_handle->coreotn_handle,
                                                          rate_target,
                                                          digi_handle->var.ghao_cfg.ghao_segments[i].oif_pkt_size,
                                                          digi_handle->var.ghao_cfg.ghao_segments[i].header_size,
                                                          &pkt_size,
                                                          &pkt_per_int,
                                                          &pkt_per_n,
                                                          &pkt_per_d,
                                                          &eps);

                }

                if(ret_val == PMC_SUCCESS)
                {
                    ret_val = coreotn_ghao_mpmo_calculate_new_cn_max_cn_d(digi_handle->coreotn_handle,
                                                                          digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_lo_chnl_id,
                                                                          rate_target,
                                                                          pkt_size,
                                                                          pkt_per_int,
                                                                          pkt_per_n,
                                                                          pkt_per_d,
                                                                          &cn_base_int,
                                                                          &cn_base_n,
                                                                          &cn_base_d,
                                                                          &program_cn_n_first);

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cn_base_int = cn_base_int;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cn_base_n = cn_base_n;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cn_base_d = cn_base_d;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].program_cn_n_first = program_cn_n_first;
                }





                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].state = DIGI_GHAO_FW_START;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].seg_type = DIGI_GHAO_CORE_SEG;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].chnl_id = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_lo_chnl_id;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ghao_mux_stage = digi_handle->var.ghao_cfg.ghao_segments[i].ghao_mux_stage;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].increase = digi_handle->var.ghao_cfg.ghao_segments[i].ghao_increase;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_start = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_target = digi_handle->var.ghao_cfg.ghao_segments[i].ts_target;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].fmf2_passthru = digi_handle->var.ghao_cfg.ghao_segments[i].mux_passthru;

                for(j=0;j<79;j++)
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].cal_slot_to_add_remove[j] = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_cal_to_add_remove[j];
                }
                for(j=0;j<3;j++)
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[j] = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_old_cal_mask[j];
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[j] = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_new_cal_mask[j];
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[j] = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mpmo3b_userbits_mask[j];
                }
                PMC_LOG_TRACE("digi_ghao_pop_firmware_ctxt(): DIGI_GHAO_CORE_SEG old_cal_mask= %08X %08X %08X, new_cal_mask= %08X %08X %08X, current_userbit= %08X %08X %08X\n",
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[0],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[0],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[0]);

                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_cal_slots = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_ts_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].total_fw_ticks = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_word_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_sub_word_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mux_xoff_thresh;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh_passthru_mux = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mux_passthru_xoff_thresh;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].thresh_update_itr = 0;
                if (digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].increase)
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr =  digi_handle->var.ghao_cfg.ghao_segments[i].fo1_mpmo_userbit_update_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mpmo_thresh_update_itr_last = (ts_target - ts_start) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].fo1_mpmo_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mux_thresh_update_itr_last = (ts_target - ts_start) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].mux_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr_last = ts_target - ts_start;

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us = (rate_target - rate_start) / GHAO_RAMP_RATE / ((DOUBLE)ts_target - (DOUBLE)ts_start) * 1e6;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 2;

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_thresh_update_itr_last = (ts_target - ts_start) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].mux_passthru_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_cal_update_itr_last = (ts_target - ts_start) - digi_handle->var.ghao_cfg.ghao_segments[i].fmf2_mux_cal_en_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_cal_update_itr =  digi_handle->var.ghao_cfg.ghao_segments[i].fmf2_mux_cal_en_pre_post_fw;
                }
                else
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr = 0;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mpmo_thresh_update_itr_last = (ts_start - ts_target) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].fo1_mpmo_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mux_thresh_update_itr_last = (ts_start - ts_target) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].mux_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr_last = ts_start - ts_target - digi_handle->var.ghao_cfg.ghao_segments[i].fo1_mpmo_userbit_update_pre_post_fw;

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us = (rate_start - rate_target) / GHAO_RAMP_RATE / ((DOUBLE)ts_start - (DOUBLE)ts_target) * 1e6;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 2;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_thresh_update_itr_last = (ts_start - ts_target) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].mux_passthru_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_cal_update_itr_last = (ts_start - ts_target) - digi_handle->var.ghao_cfg.ghao_segments[i].fmf2_mux_cal_en_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_cal_update_itr =  digi_handle->var.ghao_cfg.ghao_segments[i].fmf2_mux_cal_en_pre_post_fw;
                }
                PMC_LOG_TRACE("digi_ghao_pop_firmware_ctxt(): DIGI_GHAO_CORE_SEG ts_start=%d, ts_target=%d, rate_start=%g Gbps, rate_target=%g Gbps, ts_update_step_us=%.6g s, word_update_step_core_map_us=%.6g s\n",
                          ts_start,
                          ts_target,
                          rate_start / 1e9,
                          rate_target / 1e9,
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 1e6,
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us / 1e6);


                seg_index++;

                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].state = DIGI_GHAO_FW_START;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].seg_type = DIGI_GHAO_MAP_SEG;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].chnl_id = digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_chnl_id;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].increase = digi_handle->var.ghao_cfg.ghao_segments[i].ghao_increase;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_start = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_target = digi_handle->var.ghao_cfg.ghao_segments[i].ts_target;

                for(j=0;j<79;j++)
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].cal_slot_to_add_remove[j] = digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_mpma_cal_to_add_remove[j];
                }


                for(j=0;j<3;j++)
                {
                    /* MPMO in FO2 needs to slave to mapotn MPMA claendar */
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[j] = digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_mpma_old_cal_mask[j];
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[j] = digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_mpma_new_cal_mask[j];
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[j] = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mpmo4_userbits_mask[j];
                }
                PMC_LOG_TRACE("digi_ghao_pop_firmware_ctxt(): DIGI_GHAO_MAP_SEG old_cal_mask= %08X %08X %08X, new_cal_mask= %08X %08X %08X, current_userbit= %08X %08X %08X\n",
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[0],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[0],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[0]);

                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_cal_slots = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_ts_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].total_fw_ticks = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_word_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_sub_word_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].thresh_update_itr = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr = 0;
                if (digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].increase)
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mpmo_thresh_update_itr_last = (ts_target - ts_start) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].fo2_mpmo_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr_last = ts_target - ts_start - digi_handle->var.ghao_cfg.ghao_segments[i].fo2_mpmo_userbit_update_pre_post_fw ;

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us = (rate_target - rate_start) / GHAO_RAMP_RATE / ((DOUBLE)ts_target - (DOUBLE)ts_start) * 1e6;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 2;
                }
                else
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mpmo_thresh_update_itr_last = (ts_start - ts_target) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].fo2_mpmo_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr_last = ts_start - ts_target - digi_handle->var.ghao_cfg.ghao_segments[i].fo2_mpmo_userbit_update_pre_post_fw;

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us = (rate_start - rate_target) / GHAO_RAMP_RATE / ((DOUBLE)ts_start - (DOUBLE)ts_target) * 1e6;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 2;
                }
                PMC_LOG_TRACE("digi_ghao_pop_firmware_ctxt(): DIGI_GHAO_MAP_SEG ts_start=%d, ts_target=%d, rate_start=%g Gbps, rate_target=%g Gbps, ts_update_step_us=%.6g s, word_update_step_core_map_us=%.6g s\n",
                          ts_start,
                          ts_target,
                          rate_start / 1e9,
                          rate_target / 1e9,
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 1e6,
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us / 1e6);

                seg_index++;

            }
            else if(digi_handle->var.ghao_cfg.ghao_segments[i].datapath_type == DIGI_LINE_LINE)
            {
                /* calculate new Cn for COREOTN MPMO based on new client rate and new pkt size */
                if(ret_val == PMC_SUCCESS)
                {

                    /*use OIF calculator for LINE-CORE-LINE paths */
                    ret_val = coreotn_oif_pkt_period_calc(digi_handle->coreotn_handle,
                                                          rate_target,
                                                          digi_handle->var.ghao_cfg.ghao_segments[i].oif_pkt_size,
                                                          digi_handle->var.ghao_cfg.ghao_segments[i].header_size,
                                                          &pkt_size,
                                                          &pkt_per_int,
                                                          &pkt_per_n,
                                                          &pkt_per_d,
                                                          &eps);

                }

                if(ret_val == PMC_SUCCESS)
                {
                    ret_val = coreotn_ghao_mpmo_calculate_new_cn_max_cn_d(digi_handle->coreotn_handle,
                                                                          digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_lo_chnl_id,
                                                                          rate_target,
                                                                          pkt_size,
                                                                          pkt_per_int,
                                                                          pkt_per_n,
                                                                          pkt_per_d,
                                                                          &cn_base_int,
                                                                          &cn_base_n,
                                                                          &cn_base_d,
                                                                          &program_cn_n_first);

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cn_base_int = cn_base_int;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cn_base_n = cn_base_n;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cn_base_d = cn_base_d;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].program_cn_n_first = program_cn_n_first;
                }




                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].state = DIGI_GHAO_FW_START;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].seg_type = DIGI_GHAO_CORE_SEG;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].chnl_id = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_lo_chnl_id;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ghao_mux_stage = digi_handle->var.ghao_cfg.ghao_segments[i].ghao_mux_stage;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].increase = digi_handle->var.ghao_cfg.ghao_segments[i].ghao_increase;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_start = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_target = digi_handle->var.ghao_cfg.ghao_segments[i].ts_target;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].fmf2_passthru = digi_handle->var.ghao_cfg.ghao_segments[i].mux_passthru;


                for(j=0;j<79;j++)
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].cal_slot_to_add_remove[j] = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_cal_to_add_remove[j];
                }
                for(j=0;j<3;j++)
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[j] = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_old_cal_mask[j];
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[j] = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_new_cal_mask[j];
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[j] = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mpmo3b_userbits_mask[j];
                }
                PMC_LOG_TRACE("digi_ghao_pop_firmware_ctxt(): DIGI_GHAO_CORE_SEG old_cal_mask= %08X %08X %08X, new_cal_mask= %08X %08X %08X, current_userbit= %08X %08X %08X\n",
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[0],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[0],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[0]);

                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_cal_slots = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_ts_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].total_fw_ticks = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_word_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_sub_word_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mux_xoff_thresh;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh_passthru_mux = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mux_passthru_xoff_thresh;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].thresh_update_itr = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr = 0;
                if (digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].increase)
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr = digi_handle->var.ghao_cfg.ghao_segments[i].fo1_mpmo_userbit_update_pre_post_fw;

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mpmo_thresh_update_itr_last = (ts_target - ts_start) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].fo1_mpmo_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mux_thresh_update_itr_last = (ts_target - ts_start) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].mux_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr_last = ts_target - ts_start;

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us = (rate_target - rate_start) / GHAO_RAMP_RATE / ((DOUBLE)ts_target - (DOUBLE)ts_start) * 1e6;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 2;
                     digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_thresh_update_itr_last = (ts_target - ts_start) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].mux_passthru_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_cal_update_itr_last = (ts_target - ts_start) - digi_handle->var.ghao_cfg.ghao_segments[i].fmf2_mux_cal_en_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_cal_update_itr =  digi_handle->var.ghao_cfg.ghao_segments[i].fmf2_mux_cal_en_pre_post_fw;
                }
                else
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr = 0;

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mpmo_thresh_update_itr_last = (ts_start - ts_target) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].fo1_mpmo_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mux_thresh_update_itr_last = (ts_start - ts_target) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].mux_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr_last = ts_start - ts_target - digi_handle->var.ghao_cfg.ghao_segments[i].fo1_mpmo_userbit_update_pre_post_fw;

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us = (rate_start - rate_target) / GHAO_RAMP_RATE / ((DOUBLE)ts_start - (DOUBLE)ts_target) * 1e6;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 2;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_thresh_update_itr_last = (ts_start - ts_target) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].mux_passthru_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_cal_update_itr_last = (ts_start - ts_target) - digi_handle->var.ghao_cfg.ghao_segments[i].fmf2_mux_cal_en_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_cal_update_itr =  digi_handle->var.ghao_cfg.ghao_segments[i].fmf2_mux_cal_en_pre_post_fw;
                }
                PMC_LOG_TRACE("digi_ghao_pop_firmware_ctxt(): DIGI_GHAO_CORE_SEG ts_start=%d, ts_target=%d, rate_start=%g Gbps, rate_target=%g Gbps, ts_update_step_us=%.6g s, word_update_step_core_map_us=%.6g s\n",
                          ts_start,
                          ts_target,
                          rate_start / 1e9,
                          rate_target / 1e9,
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 1e6,
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us / 1e6);


                seg_index++;

            }
            else if(digi_handle->var.ghao_cfg.ghao_segments[i].datapath_type == DIGI_MAP_SIFD)
            {
                chnl1_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[i].sifd_ptr)->port_type;

                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].state = DIGI_GHAO_FW_START;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].seg_type = DIGI_GHAO_MAP_SEG;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].chnl_id = digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_chnl_id;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].increase = digi_handle->var.ghao_cfg.ghao_segments[i].ghao_increase;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_start = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_target = digi_handle->var.ghao_cfg.ghao_segments[i].ts_target;


                for(j=0;j<79;j++)
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].cal_slot_to_add_remove[j] = digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_mpma_cal_to_add_remove[j];
                }


                for(j=0;j<3;j++)
                {
                    /* MPMO in FO2 needs to slave to mapotn MPMA claendar */
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[j] = digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_mpma_old_cal_mask[j];
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[j] = digi_handle->var.ghao_cfg.ghao_segments[i].mapotn_mpma_new_cal_mask[j];
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[j] = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mpmo4_userbits_mask[j];
                }
                PMC_LOG_TRACE("digi_ghao_pop_firmware_ctxt(): DIGI_GHAO_MAP_SEG old_cal_mask= %08X %08X %08X, new_cal_mask= %08X %08X %08X, current_userbit= %08X %08X %08X\n",
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[0],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[0],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[0]);

                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_cal_slots = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_ts_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].total_fw_ticks = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_word_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_sub_word_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].thresh_update_itr = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr = 0;

                if (digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].increase)
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mpmo_thresh_update_itr_last = (ts_target - ts_start) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].fo2_mpmo_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr_last = ts_target - ts_start - digi_handle->var.ghao_cfg.ghao_segments[i].fo2_mpmo_userbit_update_pre_post_fw;

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us = (rate_target - rate_start) / GHAO_RAMP_RATE / ((DOUBLE)ts_target - (DOUBLE)ts_start) * 1e6;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 2;
                }
                else
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mpmo_thresh_update_itr_last = (ts_start - ts_target) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].fo2_mpmo_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr_last = ts_start - ts_target - digi_handle->var.ghao_cfg.ghao_segments[i].fo2_mpmo_userbit_update_pre_post_fw;

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us = (rate_start - rate_target) / GHAO_RAMP_RATE / ((DOUBLE)ts_start - (DOUBLE)ts_target) * 1e6;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 2;


                }
                PMC_LOG_TRACE("digi_ghao_pop_firmware_ctxt(): DIGI_GHAO_MAP_SEG ts_start=%d, ts_target=%d, rate_start=%g Gbps, rate_target=%g Gbps, ts_update_step_us=%.6g s, word_update_step_core_map_us=%.6g s\n",
                          ts_start,
                          ts_target,
                          rate_start / 1e9,
                          rate_target / 1e9,
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 1e6,
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us / 1e6);

                seg_index++;


                if((chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1 &&
                    cpb_oduksw_port0_get(coreotn_oduksw_handle_get(digi_handle->coreotn_handle)) == CPB_ODUKSW_PORT0_ILKN) ||
                   chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN2)
                {
                    /*do nothing*/
                }
                else
                {

                      /* calculate new Cn for SIFD MPMO based on new client rate */
                      if(ret_val == PMC_SUCCESS)
                      {

                          /*use minimum epsilon calculator for MAP<->SIFD paths (packets terminate in
                          SIFD MPMO and don't leave the device) */
                          ret_val = coreotn_min_eps_pkt_period_calc(digi_handle->coreotn_handle,
                                                                    rate_target,
                                                                    &pkt_per_int,
                                                                    &pkt_per_n,
                                                                    &pkt_per_d,
                                                                    &pkt_size,
                                                                    &eps);


                      }






                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].state = DIGI_GHAO_FW_START;

                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].chnl_id = digi_handle->var.ghao_cfg.ghao_segments[i].sifd_chnl_id;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].increase = digi_handle->var.ghao_cfg.ghao_segments[i].ghao_increase;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_start = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_target = digi_handle->var.ghao_cfg.ghao_segments[i].ts_target;



                      for(j=0;j<3;j++)
                      {
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[j] = digi_handle->var.ghao_cfg.ghao_segments[i].sifd_old_cal_mask[j];
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[j] = digi_handle->var.ghao_cfg.ghao_segments[i].sifd_new_cal_mask[j];
                      }
                      PMC_LOG_TRACE("digi_ghao_pop_firmware_ctxt(): DIGI_GHAO_SIFD_SEG old_cal_mask= %08X %08X %08X, new_cal_mask= %08X %08X %08X\n",
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[2],
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[1],
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[0],
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[2],
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[1],
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[0]);

                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_cal_slots = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_ts_step = 0;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].total_fw_ticks = 0;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_word_step = 0;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_sub_word_step = 0;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh = 0;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].thresh_update_itr = 0;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr = 0;
                      PMC_LOG_TRACE("digi_ghao_pop_firmware_ctxt(): DIGI_GHAO_SIFD_SEG ts_start=%d, ts_target=%d, rate_start=%g Gbps, rate_target=%g Gbps, ts_update_step_us=%.6g s, word_update_step_sifd_us=%.6g s\n",
                                ts_start,
                                ts_target,
                                rate_start / 1e9,
                                rate_target / 1e9,
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 1e6,
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_sifd_us / 1e6);

                      seg_index++;
                }


            }
            else if(digi_handle->var.ghao_cfg.ghao_segments[i].datapath_type == DIGI_LINE_SIFD)
            {

                chnl1_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[i].sifd_ptr)->port_type;

                if((chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1 &&
                    cpb_oduksw_port0_get(coreotn_oduksw_handle_get(digi_handle->coreotn_handle)) == CPB_ODUKSW_PORT0_ILKN) ||
                   chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN2)
                {

                    /* calculate new Cn for COREOTN MPMO based on new client rate and new pkt size */
                    if(ret_val == PMC_SUCCESS)
                    {

                        /*use OIF calculator for LINE-CORE-SIFD paths (for packets terminating at CORE MPMO */
                        ret_val = coreotn_oif_pkt_period_calc(digi_handle->coreotn_handle,
                                                              rate_target,
                                                              digi_handle->var.ghao_cfg.ghao_segments[i].oif_pkt_size,
                                                              digi_handle->var.ghao_cfg.ghao_segments[i].header_size,
                                                              &pkt_size,
                                                              &pkt_per_int,
                                                              &pkt_per_n,
                                                              &pkt_per_d,
                                                              &eps);

                    }

                }
                else
                {


              }



                if(ret_val == PMC_SUCCESS)
                {
                    ret_val = coreotn_ghao_mpmo_calculate_new_cn_max_cn_d(digi_handle->coreotn_handle,
                                                                          digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_lo_chnl_id,
                                                                          rate_target,
                                                                          pkt_size,
                                                                          pkt_per_int,
                                                                          pkt_per_n,
                                                                          pkt_per_d,
                                                                          &cn_base_int,
                                                                          &cn_base_n,
                                                                          &cn_base_d,
                                                                          &program_cn_n_first);

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cn_base_int = cn_base_int;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cn_base_n = cn_base_n;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cn_base_d = cn_base_d;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].program_cn_n_first = program_cn_n_first;
                }




                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].state = DIGI_GHAO_FW_START;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].seg_type = DIGI_GHAO_CORE_SEG;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].chnl_id = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_lo_chnl_id;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ghao_mux_stage = digi_handle->var.ghao_cfg.ghao_segments[i].ghao_mux_stage;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].increase = digi_handle->var.ghao_cfg.ghao_segments[i].ghao_increase;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_start = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_target = digi_handle->var.ghao_cfg.ghao_segments[i].ts_target;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].fmf2_passthru = digi_handle->var.ghao_cfg.ghao_segments[i].mux_passthru;


                for(j=0;j<79;j++)
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].cal_slot_to_add_remove[j] = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_cal_to_add_remove[j];
                }
                for(j=0;j<3;j++)
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[j] = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_old_cal_mask[j];
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[j] = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_tx_new_cal_mask[j];
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[j] = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mpmo3b_userbits_mask[j];
                }
                PMC_LOG_TRACE("digi_ghao_pop_firmware_ctxt(): DIGI_GHAO_CORE_SEG old_cal_mask= %08X %08X %08X, new_cal_mask= %08X %08X %08X, current_userbit= %08X %08X %08X\n",
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[0],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[0],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[2],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[1],
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[0]);

                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_cal_slots = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_ts_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].total_fw_ticks = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_word_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_sub_word_step = 0;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mux_xoff_thresh;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh_passthru_mux = digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mux_passthru_xoff_thresh;
                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].thresh_update_itr = 0;

                if (digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].increase)
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr = digi_handle->var.ghao_cfg.ghao_segments[i].fo1_mpmo_userbit_update_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mpmo_thresh_update_itr_last = (ts_target - ts_start) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].fo1_mpmo_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mux_thresh_update_itr_last = (ts_target - ts_start) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].mux_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr_last = ts_target - ts_start;

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us = (rate_target - rate_start) / GHAO_RAMP_RATE / ((DOUBLE)ts_target - (DOUBLE)ts_start) * 1e6;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 2;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_thresh_update_itr_last = (ts_target - ts_start) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].mux_passthru_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_cal_update_itr_last = (ts_target - ts_start) - digi_handle->var.ghao_cfg.ghao_segments[i].fmf2_mux_cal_en_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_cal_update_itr =  digi_handle->var.ghao_cfg.ghao_segments[i].fmf2_mux_cal_en_pre_post_fw;
                }
                else
                {
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr = 0;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mpmo_thresh_update_itr_last = (ts_start - ts_target) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].fo1_mpmo_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].mux_thresh_update_itr_last = (ts_start - ts_target) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].mux_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr_last = ts_start - ts_target - digi_handle->var.ghao_cfg.ghao_segments[i].fo1_mpmo_userbit_update_pre_post_fw;

                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us = (rate_start - rate_target) / GHAO_RAMP_RATE / ((DOUBLE)ts_start - (DOUBLE)ts_target) * 1e6;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 2;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_thresh_update_itr_last = (ts_start - ts_target) * 2 - digi_handle->var.ghao_cfg.ghao_segments[i].mux_passthru_thresh_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_cal_update_itr_last = (ts_start - ts_target) - digi_handle->var.ghao_cfg.ghao_segments[i].fmf2_mux_cal_en_pre_post_fw;
                    digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].passthru_mux_cal_update_itr =  digi_handle->var.ghao_cfg.ghao_segments[i].fmf2_mux_cal_en_pre_post_fw;
                }
                PMC_LOG_TRACE("digi_ghao_pop_firmware_ctxt(): DIGI_GHAO_CORE_SEG ts_start=%d, ts_target=%d, rate_start=%g Gbps, rate_target=%g Gbps, ts_update_step_us=%.6g s, word_update_step_core_map_us=%.6g s\n",
                          ts_start,
                          ts_target,
                          rate_start / 1e9,
                          rate_target / 1e9,
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 1e6,
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_core_map_us / 1e6);


                seg_index++;


                if((chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1 &&
                    cpb_oduksw_port0_get(coreotn_oduksw_handle_get(digi_handle->coreotn_handle)) == CPB_ODUKSW_PORT0_ILKN) ||
                   chnl1_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN2)
                {
                    /*do nothing*/
                }
                else
                {

                      /* calculate new Cn for SIFD MPMO based on new client rate and new pkt size */
                      if(ret_val == PMC_SUCCESS)
                      {

                          /*use minimum epsilon calculator for LINE<->SIFD paths (packets terminate in
                          SIFD MPMO and don't leave the device) */
                          ret_val = coreotn_min_eps_pkt_period_calc(digi_handle->coreotn_handle,
                                                                    rate_target,
                                                                    &pkt_per_int,
                                                                    &pkt_per_n,
                                                                    &pkt_per_d,
                                                                    &pkt_size,
                                                                    &eps);


                      }

      /* For SIFD segment with ILKN, packets are not terminated in SIFD MPMO*/


                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].state = DIGI_GHAO_FW_START;

                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].chnl_id = digi_handle->var.ghao_cfg.ghao_segments[i].sifd_chnl_id;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].increase = digi_handle->var.ghao_cfg.ghao_segments[i].ghao_increase;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_start = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_target = digi_handle->var.ghao_cfg.ghao_segments[i].ts_target;


                      for(j=0;j<3;j++)
                      {
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[j] = digi_handle->var.ghao_cfg.ghao_segments[i].sifd_old_cal_mask[j];
                          digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[j] = digi_handle->var.ghao_cfg.ghao_segments[i].sifd_new_cal_mask[j];
                      }
                      PMC_LOG_TRACE("digi_ghao_pop_firmware_ctxt(): DIGI_GHAO_SIFD_SEG old_cal_mask= %08X %08X %08X, new_cal_mask= %08X %08X %08X\n",
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[2],
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[1],
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].old_cal_mask[0],
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[2],
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[1],
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].new_cal_mask[0]);

                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_cal_slots = digi_handle->var.ghao_cfg.ghao_segments[i].ts_start;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_ts_step = 0;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].total_fw_ticks = 0;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_word_step = 0;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_sub_word_step = 0;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh = 0;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].thresh_update_itr = 0;
                      digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].userbits_update_itr = 0;
                      PMC_LOG_TRACE("digi_ghao_pop_firmware_ctxt(): DIGI_GHAO_SIFD_SEG ts_start=%d, ts_target=%d, rate_start=%g Gbps, rate_target=%g Gbps, ts_update_step_us=%.6g s, word_update_step_sifd_us=%.6g s\n",
                                ts_start,
                                ts_target,
                                rate_start / 1e9,
                                rate_target / 1e9,
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].ts_update_step_us / 1e6,
                                digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].word_update_step_sifd_us / 1e6);

                      seg_index++;
                }

            }


        } /* if(digi_handle->var.ghao_cfg.ghao_segments[i].segment_in_use) */

    } /* for(i=0;i<MAX_NUM_FIRMWARE_SEGMENTS;i++) */

    digi_handle->var.ghao_cfg.num_fw_segments = seg_index;


    PMC_RETURN(ret_val);

}




/*******************************************************************************
* digi_ghao_save_firmware_ctxt
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Save the "firmware" context back to the G.HAO segment context once the
*  firmware sequence is completed.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
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
PUBLIC PMC_ERROR digi_ghao_save_firmware_ctxt(digi_handle_t *digi_handle)
{

    UINT32 i, j;
    UINT32 seg_index = 0;

    PMC_ENTRY();


    for(i=0;i<MAX_NUM_GHAO_SEGMENTS;i++)
    {
        if(digi_handle->var.ghao_cfg.ghao_segments[i].segment_in_use &&
           digi_handle->var.ghao_cfg.ghao_segments[i].ghao_step_tx ==  DIGI_GHAO_BWR_END_1_TSCC)
        {
            if(digi_handle->var.ghao_cfg.ghao_segments[i].datapath_type == DIGI_LINE_MAP)
            {
                /* seg_type = DIGI_GHAO_CORE_SEG */
                for(j=0;j<3;j++)
                {
                    digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mpmo3b_userbits_mask[j] = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[j];
                }
                digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mux_xoff_thresh = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh;

                digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mux_passthru_xoff_thresh = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh_passthru_mux;

                seg_index++;

                /* seg_type = DIGI_GHAO_MAP_SEG */
                for(j=0;j<3;j++)
                {
                    digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mpmo4_userbits_mask[j] = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[j];
                }
            }
            else if(digi_handle->var.ghao_cfg.ghao_segments[i].datapath_type == DIGI_MAP_SIFD)
            {
                /* seg_type = DIGI_GHAO_MAP_SEG */
                for(j=0;j<3;j++)
                {
                    digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mpmo4_userbits_mask[j] = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[j];
                }
            }
            else if(digi_handle->var.ghao_cfg.ghao_segments[i].datapath_type == DIGI_LINE_LINE)
            {
                /* seg_type = DIGI_GHAO_CORE_SEG */
                for(j=0;j<3;j++)
                {
                    digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mpmo3b_userbits_mask[j] = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[j];
                }
                digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mux_xoff_thresh = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh;
                digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mux_passthru_xoff_thresh = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh_passthru_mux;

                seg_index++;
            }
            else if(digi_handle->var.ghao_cfg.ghao_segments[i].datapath_type == DIGI_LINE_SIFD)
            {
                /* seg_type = DIGI_GHAO_CORE_SEG */
                for(j=0;j<3;j++)
                {
                    digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mpmo3b_userbits_mask[j] = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].current_userbit[j];
                }
                digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mux_xoff_thresh = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh;
                digi_handle->var.ghao_cfg.ghao_segments[i].coreotn_mux_passthru_xoff_thresh = digi_handle->var.ghao_cfg.ghao_fw_segments[seg_index].xoff_thresh_passthru_mux;

                seg_index++;
            }


        } /* if(digi_handle->var.ghao_cfg.ghao_segments[i].segment_in_use) */

    } /* for(i=0;i<MAX_NUM_FIRMWARE_SEGMENTS;i++) */


    PMC_RETURN(PMC_SUCCESS);

} /* digi_ghao_save_firmware_ctxt */




/*******************************************************************************
* digi_ghao_gen_slots_add_rem_from_masks
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Function to generate the tribslots added/removed vector from tribslot masks
*
* INPUTS:
*   tribslot_mask_1      - The first tribslot mask
*   tribslot_mask_2      - The second tribslot mask
*
* OUTPUTS:
*   tribslot_slots       - Array with the tribslots
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PUBLIC void digi_ghao_gen_slots_add_rem_from_masks(UINT32 tribslot_mask_1[3],
                                                   UINT32 tribslot_mask_2[3],
                                                   BOOL8 tribslot_slots[96])
{
    UINT32 i;
    BOOL8 val_1;
    BOOL8 val_2;

    PMC_ENTRY();

    for(i=0;i<96;i++)
    {
        val_1 = (tribslot_mask_1[i/32] >> (i & 0x1F)) & 0x1;
        val_2 = (tribslot_mask_2[i/32] >> (i & 0x1F)) & 0x1;
        tribslot_slots[i] = val_1 ^ val_2;
    }

    PMC_RETURN();

} /* digi_ghao_gen_slots_add_rem_from_masks */





/*******************************************************************************
* digi_ghao_gen_cal_from_mask
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Function to generate the calendar slots from the calendar slot mask
*
* INPUTS:
*   cal_mask           - The calendar mask

*
* OUTPUTS:
*   cal_slots          - Array with the calendar slots
*
* RETURNS:
*
* NOTES:
*
*******************************************************************************/
PRIVATE void digi_ghao_gen_cal_from_mask(UINT32 cal_mask[3],
                                         BOOL8 cal_slots[96])

{
    UINT32 i;

    PMC_ENTRY();

    for(i=0;i<96;i++)
    {
        cal_slots[i] = (cal_mask[i/32] >> (i & 0x1F)) & 0x1;
    }

    PMC_RETURN();

} /* digi_ghao_gen_cal_from_mask */



/*******************************************************************************
* digi_ghao_cpb_dcs_calendar_add_rem
* ______________________________________________________________________________
*
* DESCRIPTION:
*   In increase, add one calendar slot for the specified channel to the DCPB or
*   ODUKSW database. In decrease, remove one calendar slot. Calendar slots are
*   allocated among the available slots. Then, reconfigure the DCS calendar to
*   apply the changes.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   cpb_datapath           - DCPB/ODUKSW datapath
*   chnl                   - Channel ID
*   increase               - 0: decrease; 1: increase
*   num_slots              - old number of calendar slots
*
* OUTPUTS:
*   num_slots              - new number of calendar slots
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_cpb_dcs_calendar_add_rem(digi_handle_t *digi_handle,
                                                    digi_ghao_cpb_datapath_t cpb_datapath,
                                                    UINT32 chnl,
                                                    BOOL increase,
                                                    UINT32 *num_slots)
{
    UINT32 num_cal_entries_final;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();


    /* Get number of calendar slots after reconfig */
    if (increase)
    {
        num_cal_entries_final = *num_slots + 1;
    }
    else
    {
        num_cal_entries_final = *num_slots - 1;
    }


    PMC_LOG_TRACE("Call digi_ghao_cpb_dcs_calendar_add_rem(): cpb_datapath=%d, chnl=%d, increase=%d, num_cal_entries_final=%d\n", cpb_datapath, chnl, increase, num_cal_entries_final);

    /* Allocate/deallocate the new/old calendar slot */
    if (result == PMC_SUCCESS)
    {
        result = digi_ghao_cpb_db_chnl_rec_update( digi_handle,
                                                   cpb_datapath,
                                                   chnl,
                                                   num_cal_entries_final );
    }

    /* Reconfigure the DCS calendar */
    if (result == PMC_SUCCESS)
    {
        result = digi_ghao_cpb_dcs_calendar_cfg(digi_handle,
                                                cpb_datapath,
                                                chnl);
    }

    *num_slots = num_cal_entries_final;

    PMC_RETURN(result);

} /* digi_ghao_cpb_dcs_calendar_add_rem */



/*******************************************************************************
* digi_ghao_cpb_db_chnl_rec_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Update calendar slots for a channel to the DCPB or ODUKSW database. Current
*   calendar slots are first deallocated. Then the target number of calendar slots
*   is allocated among the available slots.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   cpb_datapath           - DCPB/ODUKSW datapath
*   chnl                   - Channel ID
*   num_cal_entries_final  - Number of calendar entries after resize
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_cpb_db_chnl_rec_update(digi_handle_t *digi_handle,
                                                  digi_ghao_cpb_datapath_t cpb_datapath,
                                                  UINT32 chnl,
                                                  UINT32 num_cal_entries_final)
{
    BOOL is_oduksw;
    cpb_handle_t *cpb_handle;
    UINT32 egress_mux_output_port;
    UINT32 egress_mux_input_port;
    UINT32 rd_channel_id;
    UINT32 rd_queue_id;
    UINT32 dummy_ho_chnl;
    coreotn_chnl_order_t dummy_order_type;
    PMC_ERROR result = PMC_SUCCESS;
    UINT32 current_num_entries = 0;
    UINT32 dcs_entries[96] = { 0 };
    UINT32 entries_removed = 0;
    UINT32 i;

    PMC_ENTRY();


    switch (cpb_datapath)
    {
        case DIGI_GHAO_DCPB_SIFD_MAPOTN:
            cpb_handle = digi_handle->dcpb_handle;
            egress_mux_output_port = 3;
            is_oduksw = FALSE;
            break;
        case DIGI_GHAO_ODUKSW_MAPOTN_COREOTN:
        case DIGI_GHAO_ODUKSW_SIFD1_COREOTN:
        case DIGI_GHAO_ODUKSW_SIFD2_COREOTN:
        case DIGI_GHAO_ODUKSW_COREOTN_COREOTN:
            cpb_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
            egress_mux_output_port = COREOTN_ODUKSW_OUTPUT_COREOTN;
            is_oduksw = TRUE;
            break;
        case DIGI_GHAO_ODUKSW_COREOTN_MAPOTN:
        case DIGI_GHAO_ODUKSW_SIFD1_MAPOTN:
        case DIGI_GHAO_ODUKSW_SIFD2_MAPOTN:
            cpb_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
            egress_mux_output_port = COREOTN_ODUKSW_OUTPUT_MAPOTN;
            is_oduksw = TRUE;
            break;
        case DIGI_GHAO_ODUKSW_MAPOTN_SIFD1:
        case DIGI_GHAO_ODUKSW_COREOTN_SIFD1:
            cpb_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
            egress_mux_output_port = COREOTN_ODUKSW_OUTPUT_SIFD1;
            is_oduksw = TRUE;
            break;
        case DIGI_GHAO_ODUKSW_MAPOTN_SIFD2:
        case DIGI_GHAO_ODUKSW_COREOTN_SIFD2:
            cpb_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
            egress_mux_output_port = COREOTN_ODUKSW_OUTPUT_SIFD2;
            is_oduksw = TRUE;
            break;
        default:
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
            break;
    }


    if (cpb_datapath == DIGI_GHAO_ODUKSW_COREOTN_MAPOTN ||
        cpb_datapath == DIGI_GHAO_ODUKSW_SIFD1_MAPOTN ||
        cpb_datapath == DIGI_GHAO_ODUKSW_SIFD2_MAPOTN)
    {
        /* Use the same calendar as MPMO stage 4 and MAPOTN MPMA */
        UINT32 cal_idx;
        UINT32 cal_chnl;

        /* Deallocate current calendar slots */
        if (result == PMC_SUCCESS)
        {
            result = coreotn_oduksw_db_entry_clear( digi_handle->coreotn_handle,
                                                    (coreotn_oduksw_output_port_type_t)egress_mux_output_port,
                                                    chnl );
        }

        for (cal_idx = 0; cal_idx < 96; ++cal_idx)
        {
            /* Get MAPOTN MPMA database */
            result = mapotn_db_entry_get(digi_handle->mapotn_handle, cal_idx, &cal_chnl);

            if (result == PMC_SUCCESS && cal_chnl == chnl)
            {
                PMC_LOG_TRACE("digi_ghao_cpb_db_chnl_rec_update(): Allocate cal_slot: egress_mux_output_port=%d, channel=%d, calendar_entry=%d\n",
                          egress_mux_output_port,
                          chnl,
                          cal_idx);

                /* Set ODUKSW database identical as MAPOTN MPMA*/
                result = coreotn_oduksw_db_entry_set(digi_handle->coreotn_handle,
                                                     (coreotn_oduksw_output_port_type_t)egress_mux_output_port,
                                                     cal_idx,
                                                     chnl);
                if(PMC_SUCCESS != result)
                {
                    PMC_RETURN(result);
                }
            }
        }
    }
    else if(cpb_datapath == DIGI_GHAO_ODUKSW_MAPOTN_COREOTN ||
            cpb_datapath == DIGI_GHAO_ODUKSW_SIFD1_COREOTN ||
            cpb_datapath == DIGI_GHAO_ODUKSW_SIFD2_COREOTN ||
            cpb_datapath == DIGI_GHAO_ODUKSW_COREOTN_COREOTN)
    {
        /* Get the DCS instance */
        if (result == PMC_SUCCESS)
        {
            result = cpb_dcs_egress_dpi_chnl_source_get( cpb_handle,
                                                         egress_mux_output_port,
                                                         chnl,
                                                         &egress_mux_input_port,
                                                         &rd_channel_id,
                                                         &rd_queue_id );
        }

        /* Deallocate current calendar slots */
        if (result == PMC_SUCCESS)
        {
            result = cpb_dcs_db_entry_clear( cpb_handle,
                                             egress_mux_input_port,
                                             egress_mux_output_port,
                                             chnl );
        }


        /*paths towards COREOTN must use COREOTN DB populate method*/
        if(result == PMC_SUCCESS)
        {


            result = coreotn_db_ret_sched_populate(digi_handle->coreotn_handle,
                                                    (coreotn_oduksw_output_port_type_t)egress_mux_output_port,
                                                    chnl,
                                                    &dummy_ho_chnl,
                                                    &dummy_order_type);

        }


    }
    else
    {
        /* Get the DCS instance */
        if (result == PMC_SUCCESS)
        {
            result = cpb_dcs_egress_dpi_chnl_source_get( cpb_handle,
                                                         egress_mux_output_port,
                                                         chnl,
                                                         &egress_mux_input_port,
                                                         &rd_channel_id,
                                                         &rd_queue_id );
        }

        if(result == PMC_SUCCESS)
        {
            result = cpb_dcs_db_num_entry_get(cpb_handle,
                                                egress_mux_input_port,
                                                egress_mux_output_port,
                                                chnl,
                                                &current_num_entries);

        }

        if(num_cal_entries_final > current_num_entries)
        {
            /*For increase, add new entries*/

            /* Allocate the new calendar slots */
            if (result == PMC_SUCCESS)
            {
                result = cpb_dcs_db_entry_set_all( cpb_handle,
                                                   egress_mux_input_port,
                                                   egress_mux_output_port,
                                                   chnl,
                                                   (num_cal_entries_final-current_num_entries) );
            }

        }
        else if(num_cal_entries_final != current_num_entries)
        {
            /*for decrease, remove only existing entries*/

            if(result == PMC_SUCCESS)
            {

                result = cpb_dcs_db_entry_all_get(cpb_handle,
                                                  egress_mux_input_port,
                                                  dcs_entries);
            }

            for(i=0;i<96;i++)
            {
                if(dcs_entries[i] == rd_channel_id)
                {
                    if(result == PMC_SUCCESS)
                    {
                        result = cpb_dcs_db_calendar_entry_clear(cpb_handle,
                                                                   egress_mux_input_port,
                                                                   egress_mux_output_port,
                                                                   chnl,
                                                                   i);
                    }
                                                   
                    entries_removed++;
                }

                if(entries_removed == (current_num_entries - num_cal_entries_final))
                {
                    break;
                }
            }
        }

    }

    PMC_RETURN(result);

} /* digi_ghao_cpb_db_chnl_rec_update */



/*******************************************************************************
* digi_ghao_cpb_dcs_calendar_cfg
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reconfigures DCS calendar by performing page update and swap for DCPB or
*   ODUKSW.  It is assumed that the database has been modified prior to calling
*   this.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   cpb_datapath           - DCPB/ODUKSW datapath
*   chnl                   - Channel ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_cpb_dcs_calendar_cfg(digi_handle_t *digi_handle,
                                                digi_ghao_cpb_datapath_t cpb_datapath,
                                                UINT32 chnl)
{
    BOOL is_oduksw;
    cpb_handle_t *cpb_handle;
    UINT32 egress_mux_output_port;
    UINT32 egress_mux_input_port;
    UINT32 rd_channel_id;
    UINT32 rd_queue_id;
    PMC_ERROR result = PMC_SUCCESS;

    PMC_ENTRY();


    switch (cpb_datapath)
    {
        case DIGI_GHAO_DCPB_SIFD_MAPOTN:
            cpb_handle = digi_handle->dcpb_handle;
            egress_mux_output_port = 3;
            is_oduksw = FALSE;
            break;
        case DIGI_GHAO_ODUKSW_MAPOTN_COREOTN:
        case DIGI_GHAO_ODUKSW_SIFD1_COREOTN:
        case DIGI_GHAO_ODUKSW_SIFD2_COREOTN:
        case DIGI_GHAO_ODUKSW_COREOTN_COREOTN:
            cpb_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
            egress_mux_output_port = COREOTN_ODUKSW_OUTPUT_COREOTN;
            is_oduksw = TRUE;
            break;
        case DIGI_GHAO_ODUKSW_COREOTN_MAPOTN:
        case DIGI_GHAO_ODUKSW_SIFD1_MAPOTN:
        case DIGI_GHAO_ODUKSW_SIFD2_MAPOTN:
            cpb_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
            egress_mux_output_port = COREOTN_ODUKSW_OUTPUT_MAPOTN;
            is_oduksw = TRUE;
            break;
        case DIGI_GHAO_ODUKSW_MAPOTN_SIFD1:
        case DIGI_GHAO_ODUKSW_COREOTN_SIFD1:
            cpb_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
            egress_mux_output_port = COREOTN_ODUKSW_OUTPUT_SIFD1;
            is_oduksw = TRUE;
            break;
        case DIGI_GHAO_ODUKSW_MAPOTN_SIFD2:
        case DIGI_GHAO_ODUKSW_COREOTN_SIFD2:
            cpb_handle = coreotn_oduksw_handle_get(digi_handle->coreotn_handle);
            egress_mux_output_port = COREOTN_ODUKSW_OUTPUT_SIFD2;
            is_oduksw = TRUE;
            break;
        default:
            PMC_RETURN(DIGI_ERR_INVALID_ARG);
            break;
    }


    PMC_LOG_TRACE("Call digi_ghao_cpb_dcs_calendar_cfg(): cpb_datapath=%d, chnl=%d, egress_mux_output_port=%d, is_oduksw=%d\n", cpb_datapath, chnl, egress_mux_output_port, is_oduksw);

    /* Get the DCS instance */
    if (result == PMC_SUCCESS)
    {
        result = cpb_dcs_egress_dpi_chnl_source_get( cpb_handle,
                                                     egress_mux_output_port,
                                                     chnl,
                                                     &egress_mux_input_port,
                                                     &rd_channel_id,
                                                     &rd_queue_id );
    }

    /* Reconfigures DCS calendar by performing page update and swap */
    if (result == PMC_SUCCESS)
    {
        result = cpb_dcs_calendar_cfg( cpb_handle,
                                       egress_mux_input_port,
                                       rd_queue_id );
    }

    PMC_RETURN(result);

} /* digi_ghao_cpb_dcs_calendar_cfg */







/*******************************************************************************
* digi_ghao_cpb_dcs_calendar_cfg_plus_userbits
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reconfigures DCS with additional entries.  It's require to always keep
*   DPI calendar >= to the DCI. This is used as an intermediate step in
*   ODUKSW calendar swap to ensure this condition.  Since the DCS
*   at port 2 slave to the coreotn bd, it's required to perform a hack on
*   the database.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - ghaa segment ID
*
* OUTPUTS:
*
* RETURNS:
*   PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_cpb_dcs_calendar_cfg_plus_userbits(digi_handle_t *digi_handle,
                                                              UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 i;
    UINT32 dummy_tx_old_cal_mask[3];
    UINT32 dummy_rx_new_cal_mask[3];
    UINT32 dummy_cal_to_add_remove_num;
    UINT32 dummy_tx_cal_to_add_remove[79];
    BOOL8 temp_new_trib_slots[ODTU_MUX_NUM_CHANL];
    UINT32 temp_old_cal[3];
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT32 coreotn_ho_chnl_id;
    UINT32 coreotn_lo_chnl_id;
    coreotn_mux_stage_t ghao_mux_stage;


    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    for(i=0;i<3;i++)
    {
        temp_old_cal[i] = 0;
    }

    for(i=0;i<ODTU_MUX_NUM_CHANL;i++)
    {
        temp_new_trib_slots[i] = local_ghao_segment->coreotn_tx_new_trib_slots[i];
    }

    /* create new tribslots with addition of userbits */
    for(i=0;i<digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_userbit_update_pre_post_fw;i++)
    {
        temp_new_trib_slots[local_ghao_segment->coreotn_tx_cal_to_add_remove[i]] = TRUE;
    }

    /*Update database to include +fo1_mpmo_userbit_update_pre_post_fw entries (this is
    a required step, because to DCS calendar slaves to the coreotn BD*/
    if(ret_val == PMC_SUCCESS)
    {
        ret_val = coreotn_ghao_db_chnl_rec_update(digi_handle->coreotn_handle,
                                                  UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,
                                                  ghao_mux_stage,
                                                  coreotn_ho_chnl_id,
                                                  coreotn_lo_chnl_id,
                                                  temp_new_trib_slots,
                                                  dummy_tx_old_cal_mask,
                                                  dummy_rx_new_cal_mask,
                                                  &dummy_cal_to_add_remove_num,
                                                  dummy_tx_cal_to_add_remove);
    }

    if (ret_val == PMC_SUCCESS)
    {
        /* in the LCR resize, it's required keep the DPI calendar >= to the DCI
        calendar.  It's required to change the ODUKSW calendar after removing
        the userbits */
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : DB) Setup database UTIL_GEN_DB_OCPB_DCS TX\n");
        ret_val = digi_ghao_cpb_db_chnl_rec_update(digi_handle,
                                                   DIGI_GHAO_ODUKSW_MAPOTN_COREOTN,
                                                   coreotn_lo_chnl_id,
                                                   (local_ghao_segment->ts_target+ digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_userbit_update_pre_post_fw));
    }


    if(ret_val == PMC_SUCCESS)
    {
        ret_val = digi_ghao_cpb_dcs_calendar_cfg(digi_handle,
                                                 DIGI_GHAO_ODUKSW_MAPOTN_COREOTN,
                                                 coreotn_lo_chnl_id);
    }

    /*restore database to new configuration*/
    if(ret_val == PMC_SUCCESS)
    {
        ret_val = coreotn_ghao_db_chnl_rec_update(digi_handle->coreotn_handle,
                                                  UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,
                                                  ghao_mux_stage,
                                                  coreotn_ho_chnl_id,
                                                  coreotn_lo_chnl_id,
                                                  local_ghao_segment->coreotn_tx_new_trib_slots,
                                                  dummy_tx_old_cal_mask,
                                                  dummy_rx_new_cal_mask,
                                                  &dummy_cal_to_add_remove_num,
                                                  dummy_tx_cal_to_add_remove);
    }



    PMC_RETURN(ret_val);

} /* digi_ghao_cpb_dcs_calendar_cfg_plus_userbits */


/*
** End of file
*/

