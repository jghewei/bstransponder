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
*   DESCRIPTION : This file contains C functions for G.HAO LINE <-> LINE
*                 segment.
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

/*
** Internal Functions
*/


/*
**  Exported Interface Functions - Start
*/

/*******************************************************************************
* digi_ghao_line_line_lcr_size_tx_inc_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the LCR step for LINE<->LINE increase segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_tx_inc_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    UINT8 coreotn_tx_tpid;
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    BOOL mux_passthru;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    coreotn_tx_tpid = local_ghao_segment->coreotn_tx_tpid;
    mux_passthru = local_ghao_segment->mux_passthru;
    
    PMC_LOG_TRACE("Start G.HAO LCR resize TX INC RP\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    /* Initialize G.HAO in TSBs that need to be initialized */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 0) Extract added/removed calendar slots for MUX\n");
        ret_val = coreotn_ghao_db_chnl_get_masks(digi_handle->coreotn_handle,
                                                 UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,
                                                 ghao_mux_stage,
                                                 coreotn_ho_chnl_id,
                                                 coreotn_lo_chnl_id,
                                                 local_ghao_segment->coreotn_tx_new_trib_slots,
                                                 local_ghao_segment->coreotn_tx_old_cal_mask,
                                                 local_ghao_segment->coreotn_tx_new_cal_mask,
                                                 &local_ghao_segment->cal_to_add_remove_num,
                                                 local_ghao_segment->coreotn_tx_cal_to_add_remove);
    }

    if (ret_val == PMC_SUCCESS)
    {
        digi_ghao_gen_slots_add_rem_from_masks(local_ghao_segment->coreotn_tx_old_cal_mask,
                                               local_ghao_segment->coreotn_tx_new_cal_mask,
                                               local_ghao_segment->coreotn_tx_cal_slots_add_rem);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc_rp : 0a) MUX INIT: ghao_mux_stage=%d, ghao_id=%d, ho_chnl_id=%d, lo_chnl_id=%d, tbid=%d, tribslots=%d\n", ghao_mux_stage, ghao_segment_id, coreotn_ho_chnl_id, coreotn_lo_chnl_id, coreotn_tx_tpid, local_ghao_segment->ts_target);
        ret_val = coreotn_mux_ghao_init(digi_handle->coreotn_handle,
                                        ghao_mux_stage,
                                        ghao_segment_id,
                                        coreotn_ho_chnl_id, 
                                        coreotn_lo_chnl_id,
                                        coreotn_tx_tpid,
                                        local_ghao_segment->coreotn_tx_cal_slots_add_rem);
    }

    if(mux_passthru)
    {

        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO LCR resize tx_inc_rp : 0b) MUX INIT (passthru mux): ghao_mux_stage=%d, ghao_id=%d, ho_chnl_id=%d, lo_chnl_id=%d, tbid=%d, tribslots=%d\n", ghao_mux_stage, ghao_segment_id, coreotn_ho_chnl_id, coreotn_lo_chnl_id, coreotn_tx_tpid, local_ghao_segment->ts_target);
              ret_val = coreotn_mux_ghao_init(digi_handle->coreotn_handle,
                                            COREOTN_MUX_STAGE_TWO,
                                            ghao_segment_id,
                                            coreotn_lo_chnl_id, 
                                            coreotn_lo_chnl_id,
                                            coreotn_tx_tpid,
                                            local_ghao_segment->coreotn_tx_cal_slots_add_rem);
        }

    }


    /* Link Connection Resize (FIFO and Calendar Setup): */
    /* 1)	Once Control plane is ready. Send: */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc_rp : 1) MUX (Stage 1-2): [ADD, #a, NACK] and RP = 1, TSCC = 0 on all added Trib-slots.\n");
        ret_val = coreotn_mux_ghao_send_add_nack(digi_handle->coreotn_handle,
                                                 ghao_mux_stage,
                                                 ghao_segment_id);
    }


    PMC_LOG_TRACE("End G.HAO LCR resize TX INC RP\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_lcr_size_tx_inc_rp */



/*******************************************************************************
* digi_ghao_line_line_lcr_size_rx_inc_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the LCR step for LINE<->LINE increase segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_rx_inc_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    UINT8 coreotn_rx_tpid;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    coreotn_rx_tpid = local_ghao_segment->coreotn_rx_tpid;

    
    PMC_LOG_TRACE("Start G.HAO LCR resize RX INC RP\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    /* Initialize G.HAO in TSBs that need to be initialized */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc_rp : 0) DMX INIT: ghao_mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d, tribslots=%d\n", ghao_mux_stage, coreotn_ho_chnl_id, coreotn_lo_chnl_id, local_ghao_segment->ts_target);
        ret_val = coreotn_dmx_ghao_init(digi_handle->coreotn_handle,
                                        ghao_mux_stage,
                                        coreotn_lo_chnl_id,
                                        coreotn_rx_tpid,
                                        local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    /* Link Connection Resize (FIFO and Calendar Setup): */
    /* 1)	Once Control plane is ready. Send: */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc_rp : 1) DMX (Stage 1-2): Wait to receive RP = 1 and observe [ADD, #a, NACK] and TSCC = 0 in all new TS.\n");
        ret_val = coreotn_dmx_ghao_wait_for_add_nack(digi_handle->coreotn_handle,
                                                     ghao_mux_stage,
                                                     &coreotn_ho_chnl_id,
                                                     &coreotn_lo_chnl_id,
                                                     &coreotn_rx_tpid,
                                                     local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    PMC_LOG_TRACE("End G.HAO LCR resize RX INC RP\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_lcr_size_rx_inc_rp */



/*******************************************************************************
* digi_ghao_line_line_lcr_size_tx_inc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR resize step for LINE<->LINE increase segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_tx_inc(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    BOOL8 mux_passthru;
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT32 i;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    mux_passthru = local_ghao_segment->mux_passthru;
    UINT32 num_add_rem;
    UINT32 cal_add_remove[96];
    UINT32 fmf1_xoff_pre_adj;
    
    PMC_LOG_TRACE("Start G.HAO LCR resize TX INC\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    /* Databases for TX datapath */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : DB) Setup database UTIL_GEN_DB_LINE_CORE_OTN_TX_DB\n");
        ret_val = coreotn_ghao_db_chnl_rec_update(digi_handle->coreotn_handle,
                                                  UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,
                                                  ghao_mux_stage,
                                                  coreotn_ho_chnl_id,
                                                  coreotn_lo_chnl_id,
                                                  local_ghao_segment->coreotn_tx_new_trib_slots,
                                                  local_ghao_segment->coreotn_tx_old_cal_mask,
                                                  local_ghao_segment->coreotn_tx_new_cal_mask,
                                                  &local_ghao_segment->cal_to_add_remove_num,
                                                  local_ghao_segment->coreotn_tx_cal_to_add_remove);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : DB) Setup database UTIL_GEN_DB_OCPB_DCS TX\n");
        ret_val = digi_ghao_cpb_db_chnl_rec_update(digi_handle,
                                                   DIGI_GHAO_ODUKSW_COREOTN_COREOTN,
                                                   coreotn_lo_chnl_id,
                                                   local_ghao_segment->ts_target);
    }


    /* ODUKSW -> MUX direction: */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 2) MUX (Stage 1-2): Change EXPAND CFC FIFO to new TS value. Don't increase XOFF threshold.\n");
        ret_val = coreotn_mux_ghao_resize_fifo(digi_handle->coreotn_handle,
                                               ghao_mux_stage,
                                               ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 3) MUX (Stage 1-2): Configure all the standby pages to new configuration including STS_SW, new M value.  Configure prev_cn in standby page.\n");
        ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       ghao_segment_id);
    }

    if (ghao_mux_stage == COREOTN_MUX_STAGE_ONE && mux_passthru == TRUE)
    {
        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 4) MUX (Stage 2-3A): Change FIFO size to final TS size. Do not change XOFF threshold\n");
            ret_val = coreotn_mux_ghao_resize_fifo(digi_handle->coreotn_handle,
                                                   COREOTN_MUX_STAGE_TWO,
                                                   ghao_segment_id);
        }
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 5)	MPMO (stage 3B): Change FIFO Size to final TS size. Do not change XOFF threshold.\n");
        ret_val = coreotn_mpmo_ghao_fifo_resize_no_thresh_update(digi_handle->coreotn_handle,
                                                                 COREOTN_MPMO_TO_COREOTN,
                                                                 coreotn_lo_chnl_id,
                                                                 local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 6)	ODUk Switch (LINE -> LINE): Change the FIFO size to final TS\n");
        ret_val = coreotn_oduksw_ghao_queue_resize_zone_update(digi_handle->coreotn_handle,
                                                               COREOTN_ODUKSW_OUTPUT_COREOTN,
                                                               coreotn_lo_chnl_id,
                                                               0,
                                                               local_ghao_segment->ts_target,
                                                               ghao_increase);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 13) ODUk Switch (LINE -> LINE) : Change Calendar to final TS count\n");
        ret_val = digi_ghao_cpb_dcs_calendar_cfg(digi_handle,
                                                 DIGI_GHAO_ODUKSW_COREOTN_COREOTN,
                                                 coreotn_lo_chnl_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 14) MPMO (Stage 3B): Change Calendar to new TS Calendar, but don't enable 'valid' into those Calendar, meaning set the new Cal. Entries user bits to be disabled.\n");
        ret_val = coreotn_mpmo_ghao_scbs3_reprov_force_userbits(digi_handle->coreotn_handle,
                                                                COREOTN_MPMO_TO_COREOTN,
                                                                coreotn_lo_chnl_id,
                                                                local_ghao_segment->ts_target,
                                                                local_ghao_segment->coreotn_tx_new_cal_mask,
                                                                local_ghao_segment->coreotn_tx_old_cal_mask);
    }

    if (ghao_mux_stage == COREOTN_MUX_STAGE_ONE && mux_passthru == TRUE)
    {
        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 15) MUX (Stage 2-3A) : Do passthru re-config to new TS count, but don't enable the additional calendars\n");
            ret_val = coreotn_mux_ghao_recfg_passthru(digi_handle->coreotn_handle,
                                                      COREOTN_MUX_STAGE_TWO,
                                                      ghao_segment_id);
        }
    }



    if(mux_passthru)
    {
        num_add_rem  = MPMO_USERBIT_INC_PRE_WO_PASSTHRU; /*MPMO_USERBIT_INC_PRE_W_PASSTHRU;*/
    }
    else
    {
        num_add_rem  = MPMO_USERBIT_INC_PRE_WO_PASSTHRU;
    }


    /* Initialize userbit to old cal mask for COREOTN and MAPOTN */
    for(i = 0; i< 3; i++)
    {
        local_ghao_segment->coreotn_mpmo3b_userbits_mask[i] = local_ghao_segment->coreotn_tx_old_cal_mask[i];
    }


        
    if(ret_val == PMC_SUCCESS)
    {
        for(i=0;i<num_add_rem;i++)
        {
            if (ret_val == PMC_SUCCESS)
            {
                cal_add_remove[i] = local_ghao_segment->coreotn_tx_cal_to_add_remove[i];
            }
        }

        PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_1 : 28) MPMO (Stage 3B): Enable 'valid' on one of the to-be-added cal slots, meaning set the new Cal. Entries user bits to be enabled.\n");

        ret_val = coreotn_mpmo_update_userbit_mask(digi_handle->coreotn_handle,
                                                           COREOTN_MPMO_TO_COREOTN,
                                                           coreotn_lo_chnl_id,
                                                           cal_add_remove,
                                                           num_add_rem,
                                                           local_ghao_segment->coreotn_tx_new_cal_mask,
                                                           local_ghao_segment->coreotn_mpmo3b_userbits_mask);
    }


    if (ghao_mux_stage == COREOTN_MUX_STAGE_ONE && mux_passthru == TRUE)
    {
        for(i=0;i<FMF2_MUX_CAL_EN_INC_PRE_W_PASSTHRU;i++)
        {
            if (ret_val == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_1 : 29a) MUX (Stage 2-3A) : Enable lo_calen on the same calendar slots by setting the EXPAND.LO_FIRST_CYCLE[cal_slot_id] to 1\n");
                ret_val = coreotn_mux_ghao_passthru_cal_enable_diable(digi_handle->coreotn_handle,
                                                                      local_ghao_segment->coreotn_tx_cal_to_add_remove[i],
                                                                      TRUE);
            }
        }




        if(digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start == 1)
        {
            fmf1_xoff_pre_adj = FMF1_MUX_XOFF_INC_PRE_W_PASSTHRU_FROM1;
        }
        else
        {
            fmf1_xoff_pre_adj = FMF1_MUX_XOFF_INC_PRE_W_PASSTHRU;
        }



        for(i=0;i<fmf1_xoff_pre_adj;i++)
        {
            if (ret_val == PMC_SUCCESS)
            {

                PMC_LOG_TRACE("G.HAO LCR resize inc : 29b) MUX (EXP Stage 1-2): Increase XOFF threshold by 1TS equivalent every 2.5s\n ");
                ret_val = coreotn_odtu_mux_xoff_thresh_update(digi_handle->coreotn_handle,
                                                          ghao_mux_stage,
                                                          coreotn_lo_chnl_id,
                                                          TRUE,
                                                          &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_xoff_thresh);
            }
        }


        for(i=0;i<FMF2_MUX_XOFF_INC_PRE_W_PASSTHRU;i++)
        {
            if (ret_val == PMC_SUCCESS)
            {

                PMC_LOG_TRACE("G.HAO LCR resize inc : 29c) MUX (EXP Stage 2-3A): Increase XOFF threshold by 1TS equivalent every 2.5s\n ");
                ret_val = coreotn_odtu_mux_xoff_thresh_update(digi_handle->coreotn_handle,
                                                          COREOTN_MUX_STAGE_TWO,
                                                          coreotn_lo_chnl_id,
                                                          TRUE,
                                                          &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_passthru_xoff_thresh);
            }
        }

        for(i=0;i<MPMO_XOFF_INC_PRE_W_PASSTHRU;i++)
        {
            if (ret_val == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("G.HAO LCR resize inc : 29d) MPMO : Increase XOFF threshold by 1TS equivalent every 2.5s\n ");

                ret_val = coreotn_mpmo_xoff_thresh_change(digi_handle->coreotn_handle,
                                                          COREOTN_MPMO_TO_COREOTN,
                                                          coreotn_lo_chnl_id,
                                                          ghao_increase);
            }
        }
    }
    else
    {

        if(digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start == 1)
        {
            fmf1_xoff_pre_adj = FMF1_MUX_XOFF_INC_PRE_W0_PASSTHRU_FROM1;
        }
        else
        {
            fmf1_xoff_pre_adj = FMF1_MUX_XOFF_INC_PRE_W0_PASSTHRU;
        }



        for(i=0;i<fmf1_xoff_pre_adj;i++)
        {
            if (ret_val == PMC_SUCCESS)
            {

                PMC_LOG_TRACE("G.HAO LCR resize inc : 29b) MUX (EXP Stage 1-2): Increase XOFF threshold by 1TS equivalent every 2.5s\n ");
                ret_val = coreotn_odtu_mux_xoff_thresh_update(digi_handle->coreotn_handle,
                                                          ghao_mux_stage,
                                                          coreotn_lo_chnl_id,
                                                          TRUE,
                                                          &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_xoff_thresh);
            }
        }

    }


    PMC_LOG_TRACE("End G.HAO LCR resize TX INC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_lcr_size_tx_inc */



/*******************************************************************************
* digi_ghao_line_line_lcr_size_rx_inc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR resize step for LINE<->LINE increase segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_rx_inc(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR resize RX INC\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    /* Databases for RX datapath */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc : DB) Setup database UTIL_GEN_DB_LINE_CORE_OTN_RX_DB\n");
        ret_val = coreotn_ghao_db_chnl_rec_update(digi_handle->coreotn_handle,
                                                  UTIL_GEN_DB_LINE_CORE_OTN_RX_DB,
                                                  ghao_mux_stage,
                                                  coreotn_ho_chnl_id,
                                                  coreotn_lo_chnl_id,
                                                  local_ghao_segment->coreotn_rx_new_trib_slots,
                                                  local_ghao_segment->coreotn_rx_old_cal_mask,
                                                  local_ghao_segment->coreotn_rx_new_cal_mask,
                                                  &local_ghao_segment->cal_to_add_remove_num,
                                                  local_ghao_segment->coreotn_rx_cal_to_add_remove);
    }


    /* DMX -> RGFP direction: */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc : 20) MPMA (stage 3B) : Increase FIFO to final TS size.\n");
        ret_val = coreotn_mpma_ghao_fifo_resize(digi_handle->coreotn_handle,
                                                COREOTN_MPMA_TO_COREOTN,
                                                coreotn_lo_chnl_id,
                                                local_ghao_segment->ts_target);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc : 20b) ODUJAT (1-2 and 2-3a) : Increase FIFO to final TS size.\n");
        ret_val = coreotn_odujat_ghao_resize_fifo(digi_handle->coreotn_handle,
                                                  ghao_mux_stage,
                                                  coreotn_lo_chnl_id,
                                                  local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc : 21) ODUkSC (COREOTN) :  Change to new Calendar and scale down credits to reflect old BW.\n");
        ret_val = coreotn_oduksc_scbs3_cgen_reprov(digi_handle->coreotn_handle,
                                                   ghao_mux_stage,
                                                   coreotn_lo_chnl_id,
                                                   local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc : 22) DMX(Stage 1-2): Configure all the standby pages to new configuration including STS_SW, new M value.\n");
        ret_val = coreotn_dmx_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       &coreotn_ho_chnl_id,
                                                       &coreotn_lo_chnl_id,
                                                       ghao_increase,
                                                       local_ghao_segment->old_trib_slot_mask_rx,
                                                       local_ghao_segment->coreotn_rx_trib_slots_add_rem,
                                                       local_ghao_segment->coreotn_rx_tpid);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc : 23) MPMA (stage 3B): Increase Calendar\n");
        ret_val = coreotn_mpma_ghao_scbs3_reprov(digi_handle->coreotn_handle,
                                                 COREOTN_MPMA_TO_COREOTN,
                                                 coreotn_lo_chnl_id,
                                                 local_ghao_segment->ts_target);
    }


    PMC_LOG_TRACE("End G.HAO LCR resize RX INC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_lcr_size_rx_inc */



/*******************************************************************************
* digi_ghao_line_line_lcr_size_tx_dec_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the LCR step for LINE<->LINE decrease segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_tx_dec_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT32 i;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    PMC_LOG_TRACE("Start G.HAO LCR resize TX DEC RP\n");

    /* Initialize G.HAO in TSB that need to be initialized */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 0) Extract added/removed calendar slots for MUX\n");
        ret_val = coreotn_ghao_db_chnl_get_masks(digi_handle->coreotn_handle,
                                                 UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,
                                                 ghao_mux_stage,
                                                 coreotn_ho_chnl_id,
                                                 coreotn_lo_chnl_id,
                                                 local_ghao_segment->coreotn_tx_new_trib_slots,
                                                 local_ghao_segment->coreotn_tx_old_cal_mask,
                                                 local_ghao_segment->coreotn_tx_new_cal_mask,
                                                 &local_ghao_segment->cal_to_add_remove_num,
                                                 local_ghao_segment->coreotn_tx_cal_to_add_remove);
    }

    if (ret_val == PMC_SUCCESS)
    {
        digi_ghao_gen_slots_add_rem_from_masks(local_ghao_segment->coreotn_tx_old_cal_mask,
                                               local_ghao_segment->coreotn_tx_new_cal_mask,
                                               local_ghao_segment->coreotn_tx_cal_slots_add_rem);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 0a) MUX INIT: ghao_mux_stage=%d, ghao_id=%d, ho_chnl_id=%d, lo_chnl_id=%d, tbid=%d, tribslots=%d\n", ghao_mux_stage, ghao_segment_id, coreotn_ho_chnl_id, coreotn_lo_chnl_id, local_ghao_segment->coreotn_tx_tpid, local_ghao_segment->ts_target);
        ret_val = coreotn_mux_ghao_init(digi_handle->coreotn_handle,
                                        ghao_mux_stage,
                                        ghao_segment_id,
                                        coreotn_ho_chnl_id, 
                                        coreotn_lo_chnl_id,
                                        local_ghao_segment->coreotn_tx_tpid,
                                        local_ghao_segment->coreotn_tx_cal_slots_add_rem);
    }


    if(local_ghao_segment->mux_passthru)
    {
        if (ret_val == PMC_SUCCESS)
        {
            
              PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 0b) MUX INIT (passthru mux): ghao_mux_stage=%d, ghao_id=%d, ho_chnl_id=%d, lo_chnl_id=%d, tbid=%d, tribslots=%d\n", ghao_mux_stage, ghao_segment_id, coreotn_ho_chnl_id, coreotn_lo_chnl_id, local_ghao_segment->coreotn_tx_tpid, local_ghao_segment->ts_target);
              ret_val = coreotn_mux_ghao_init(digi_handle->coreotn_handle,
                                            COREOTN_MUX_STAGE_TWO,
                                            ghao_segment_id,
                                            coreotn_lo_chnl_id, 
                                            coreotn_lo_chnl_id,
                                            local_ghao_segment->coreotn_tx_tpid,
                                            local_ghao_segment->coreotn_tx_cal_slots_add_rem);
        }

    }


    /* 1)	Once Control plane is ready. Send: */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 1) MUX (Stage 1-2): [REM, #a, NACK] and RP = 1, TSCC = 0 on all removed Trib-slots.\n");
        ret_val = coreotn_mux_ghao_send_remove_nack(digi_handle->coreotn_handle,
                                                    ghao_mux_stage,
                                                    ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 2) Set MUX (Stage 1-2) in prev_Cn mode and do a config update.\n");
        ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 2b) MUX: Wait until config update is done\n");
        ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 2c) MUX (Stage 1-2): Synchronize standby page with active page\n");
        ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 2) MUX: Wait until config update is done\n");
        ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }

    /* Initialize userbit to old cal mask for COREOTN */
    for(i = 0; i< 3; i++)
    {
        local_ghao_segment->coreotn_mpmo3b_userbits_mask[i] = local_ghao_segment->coreotn_tx_old_cal_mask[i];
    }

    PMC_LOG_TRACE("End G.HAO LCR resize TX DEC RP\n");


    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_lcr_size_tx_dec_rp */



/*******************************************************************************
* digi_ghao_line_line_lcr_size_rx_dec_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the LCR step for LINE<->LINE decrease segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_rx_dec_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    PMC_LOG_TRACE("Start G.HAO LCR resize RX DEC RP\n");

    /* Initialize G.HAO in TSB that need to be initialized */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec_rp : 0) Extract added/removed calendar slots for DMX\n");
        ret_val = coreotn_ghao_db_chnl_get_masks(digi_handle->coreotn_handle,
                                                 UTIL_GEN_DB_LINE_CORE_OTN_RX_DB,
                                                 ghao_mux_stage,
                                                 coreotn_ho_chnl_id,
                                                 coreotn_lo_chnl_id,
                                                 local_ghao_segment->coreotn_rx_new_trib_slots,
                                                 local_ghao_segment->coreotn_rx_old_cal_mask,
                                                 local_ghao_segment->coreotn_rx_new_cal_mask,
                                                 &local_ghao_segment->cal_to_add_remove_num,
                                                 local_ghao_segment->coreotn_rx_cal_to_add_remove);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec_rp : 0) DMX INIT: ghao_mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d, tribslots=%d\n", ghao_mux_stage, coreotn_ho_chnl_id, coreotn_lo_chnl_id, local_ghao_segment->ts_target);
        ret_val = coreotn_dmx_ghao_init(digi_handle->coreotn_handle,
                                        ghao_mux_stage,
                                        coreotn_lo_chnl_id,
                                        local_ghao_segment->coreotn_rx_tpid,
                                        local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    /* 1)	Once Control plane is ready. Send: */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec_rp : 1) DMX (Stage 1-2): DMX (Stage 1-2): Wait to receive RP = 1 and observe [REM, #a, NACK] and TSCC = 0 in all removed TS.\n");
        ret_val = coreotn_dmx_ghao_wait_for_remove_nack(digi_handle->coreotn_handle,
                                                        ghao_mux_stage,
                                                        &coreotn_ho_chnl_id,
                                                        &coreotn_lo_chnl_id,
                                                        &local_ghao_segment->coreotn_rx_tpid,
                                                        local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }

    PMC_LOG_TRACE("End G.HAO LCR resize RX DEC RP\n");


    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_lcr_size_rx_dec_rp */



/*******************************************************************************
* digi_ghao_line_line_lcr_size_tx_dec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR resize step for LINE<->LINE decrease segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_tx_dec(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    BOOL8 mux_passthru;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    mux_passthru = local_ghao_segment->mux_passthru;

    
    PMC_LOG_TRACE("Start G.HAO LCR resize TX DEC\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    /* LCR (FIFO and Calendar resizing) - Calendar against Traffic, FIFO with traffic: */


    if(ret_val == PMC_SUCCESS)
    {
        /* Before updating the database, it's required to perform
        the ghao "deprovision" step on all active entries */
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : Deprov MUX lo channel\n");
        ret_val = coreotn_mux_ghao_lo_chnl_deprov(digi_handle->coreotn_handle,
                                                  ghao_mux_stage,
                                                  coreotn_lo_chnl_id,
                                                  coreotn_ho_chnl_id);
    }

    /* Databases for TX datapath */
    if (ret_val == PMC_SUCCESS)
    {

        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : DB) Setup database UTIL_GEN_DB_LINE_CORE_OTN_TX_DB\n");
        ret_val = coreotn_ghao_db_chnl_rec_update(digi_handle->coreotn_handle,
                                                  UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,
                                                  ghao_mux_stage,
                                                  coreotn_ho_chnl_id,
                                                  coreotn_lo_chnl_id,
                                                  local_ghao_segment->coreotn_tx_new_trib_slots,
                                                  local_ghao_segment->coreotn_tx_old_cal_mask,
                                                  local_ghao_segment->coreotn_tx_new_cal_mask,
                                                  &local_ghao_segment->cal_to_add_remove_num,
                                                  local_ghao_segment->coreotn_tx_cal_to_add_remove);
    }

    /*this call replaces digi_ghao_cpb_db_chnl_rec_update() and digi_ghao_cpb_dcs_calendar_cfg()
    for the ODUKSW at queue 2 (towards COREOTN) for the decrease only.  There
    is a special calendar requirement on this resize*/
    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : DB) Setup database UTIL_GEN_DB_OCPB_DCS TX\n");
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : 24) ODUk Switch (MAP -> LINE): Change Calendar to final TS + 2\n");
        ret_val = digi_ghao_cpb_dcs_calendar_cfg_plus_userbits(digi_handle,
                                                               ghao_segment_id);
    }



    /* ODUKSW -> MUX direction: */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : 21) MUX (Stage 1-2): Configure all the standby pages to new configuration including STS_SW, new M value. Set the TXJC.PREV_CN_SEL to 0 in the standby page\n");
        ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : 29) ODUk Switch (LINE -> LINE): Change FIFO to final size.\n");
        ret_val = coreotn_oduksw_ghao_queue_resize_zone_update(digi_handle->coreotn_handle,
                                                               COREOTN_ODUKSW_OUTPUT_COREOTN,
                                                               coreotn_lo_chnl_id,
                                                               0,
                                                               local_ghao_segment->ts_target,
                                                               ghao_increase);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : 30) MPMO(Stage 3B): Change FIFO to final size + 2\n");
        ret_val = coreotn_mpmo_ghao_fifo_resize_no_thresh_update(digi_handle->coreotn_handle,
                                                                 COREOTN_MPMO_TO_COREOTN,
                                                                 coreotn_lo_chnl_id,
                                                                 local_ghao_segment->ts_target+local_ghao_segment->fo1_mpmo_thresh_pre_post_fw);
    }

    if (ghao_mux_stage == COREOTN_MUX_STAGE_ONE && mux_passthru == TRUE)
    {
        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO LCR resize tx_dec : 31) MUX (Stage 2-3A): Change FIFO to final size(final size + 1TS)\n");
            ret_val = coreotn_mux_ghao_resize_fifo(digi_handle->coreotn_handle,
                                                   COREOTN_MUX_STAGE_TWO,
                                                   ghao_segment_id);
        }
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : 32) MUX (Stage 1-2): Change expand FIFO to final size(final size + 1TS)\n");
        ret_val = coreotn_mux_ghao_resize_fifo(digi_handle->coreotn_handle,
                                               ghao_mux_stage,
                                               ghao_segment_id);
    }


    PMC_LOG_TRACE("End G.HAO LCR resize TX DEC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_lcr_size_tx_dec */



/*******************************************************************************
* digi_ghao_line_line_lcr_size_rx_dec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR resize step for LINE<->LINE decrease segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_size_rx_dec(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR resize RX DEC\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    /* LCR (FIFO and Calendar resizing) - Calendar against Traffic, FIFO with traffic: */


    if (ret_val == PMC_SUCCESS)
    {
        /*It is nessecary to call this deprov API in the demux before
        changing the database */
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : Deprov DMX lo channel\n");
        ret_val = coreotn_dmx_ghao_lo_chnl_deprov(digi_handle->coreotn_handle,
                                                  ghao_mux_stage,
                                                  coreotn_lo_chnl_id,
                                                  coreotn_ho_chnl_id);
    }


    /* Databases for RX datapath */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec : DB) Setup database UTIL_GEN_DB_LINE_CORE_OTN_RX_DB\n");
        ret_val = coreotn_ghao_db_chnl_rec_update(digi_handle->coreotn_handle,
                                                  UTIL_GEN_DB_LINE_CORE_OTN_RX_DB,
                                                  ghao_mux_stage,
                                                  coreotn_ho_chnl_id,
                                                  coreotn_lo_chnl_id,
                                                  local_ghao_segment->coreotn_rx_new_trib_slots,
                                                  local_ghao_segment->coreotn_rx_old_cal_mask,
                                                  local_ghao_segment->coreotn_rx_new_cal_mask,
                                                  &local_ghao_segment->cal_to_add_remove_num,
                                                  local_ghao_segment->coreotn_rx_cal_to_add_remove);
    }


    /* DMX -> ODUKSW */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec : 36) MPMA(Stage 3B) : Change Calendar to final TS\n");
        ret_val = coreotn_mpma_ghao_scbs3_reprov(digi_handle->coreotn_handle,
                                                 COREOTN_MPMA_TO_COREOTN,
                                                 coreotn_lo_chnl_id,
                                                 local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec : 37) ODUkSC (COREOTN): Change Calendar to final TS and credits to match new rate.\n");
        ret_val = coreotn_oduksc_scbs3_cgen_reprov(digi_handle->coreotn_handle,
                                                   ghao_mux_stage,
                                                   coreotn_lo_chnl_id,
                                                   local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec : 38) DMX (Stage 1-2): Configure all the standby pages to new configuration including STS_SW, new M value.\n");
        ret_val = coreotn_dmx_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       &coreotn_ho_chnl_id,
                                                       &coreotn_lo_chnl_id,
                                                       ghao_increase,
                                                       local_ghao_segment->old_trib_slot_mask_rx,
                                                       local_ghao_segment->coreotn_rx_trib_slots_add_rem,
                                                       local_ghao_segment->coreotn_rx_tpid);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec : 38b) ODUJAT (1-2 and 2-3a) : Decrease FIFO to final TS size.\n");
        ret_val = coreotn_odujat_ghao_resize_fifo(digi_handle->coreotn_handle,
                                                  ghao_mux_stage,
                                                  coreotn_lo_chnl_id,
                                                  local_ghao_segment->ts_target);
    }


    PMC_LOG_TRACE("End G.HAO LCR resize RX DEC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_lcr_size_rx_dec */



/*******************************************************************************
* digi_ghao_line_line_lcr_knife_edge_tx_inc_1_add_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 1 (add ack) for LINE<->LINE increase segment
*   in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_tx_inc_1_add_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    BOOL8 mux_passthru;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    mux_passthru = local_ghao_segment->mux_passthru;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge TX INC 1 ADD_ACK\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_1 : 27) MUX: Send [ADD, #a, ACK] on all TS\n");
        ret_val = coreotn_mux_ghao_send_add_ack(digi_handle->coreotn_handle,
                                                ghao_mux_stage,
                                                ghao_segment_id);
    }

    PMC_LOG_TRACE("End G.HAO LCR knife edge TX INC 1 ADD_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_line_lcr_knife_edge_tx_inc_1_add_ack */



/*******************************************************************************
* digi_ghao_line_line_lcr_knife_edge_rx_inc_1_add_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 1 (add ack) for LINE<->LINE increase segment
*   in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_rx_inc_1_add_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge RX INC 1 ADD_ACK\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_inc_1 : 27) DMX: Wait to receive [ADD, #a, ACK] on all TS.\n");
        ret_val = coreotn_dmx_ghao_wait_for_add_ack(digi_handle->coreotn_handle,
                                                    ghao_mux_stage,
                                                    &coreotn_ho_chnl_id,
                                                    &coreotn_lo_chnl_id,
                                                    &local_ghao_segment->coreotn_rx_tpid,
                                                    local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge RX INC 1 ADD_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_line_lcr_knife_edge_rx_inc_1_add_ack */



/*******************************************************************************
* digi_ghao_line_line_lcr_knife_edge_tx_inc_2_norm_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 2 (norm ack) for LINE<->LINE increase
*   segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_tx_inc_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge TX INC 2 NORM_ACK\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_2 : 29) MUX: Once DMX ACK is received send [NORM, #a, ACK] in all TS that needs to be changed. This is done by setting G.HAO update bit in the MUX. The MUX will automatically do Knife Edge in the next RMF after sending NORM command.\n");
        ret_val = coreotn_mux_ghao_send_norm(digi_handle->coreotn_handle,
                                                ghao_mux_stage,
                                                ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_2 : 29b) MUX: Wait until config update is done\n");
        ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge TX INC 2 NORM_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_line_lcr_knife_edge_tx_inc_2_norm_ack */



/*******************************************************************************
* digi_ghao_line_line_lcr_knife_edge_rx_inc_2_norm_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 2 (norm ack) for LINE<->LINE increase
*   segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_rx_inc_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge RX INC 2 NORM_ACK\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_inc_2 : 29) DMX: After ACK cmd is sent in MUX, DMX can receive NORM as soon as one RMF later. Wait to receive NORM in all TS. DMX HW will automatically do Knife edge in the next RMF after receiving NORM Command.\n");
        ret_val = coreotn_dmx_ghao_wait_for_norm_ack(digi_handle->coreotn_handle,
                                                    ghao_mux_stage,
                                                    &coreotn_ho_chnl_id,
                                                    &coreotn_lo_chnl_id,
                                                    &local_ghao_segment->coreotn_rx_tpid,
                                                    local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge RX INC 2 NORM_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_line_lcr_knife_edge_rx_inc_2_norm_ack */



/*******************************************************************************
* digi_ghao_line_line_lcr_knife_edge_tx_inc_3_idle_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 3 (idle nack) for LINE<->LINE increase
*   segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_tx_inc_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge TX INC 3 IDLE_NACK\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_3 : 30) MUX (Stage 1-2): Finish LCR by sending [IDLE, 0, NACK] for every added TS at the RMF.\n");
        ret_val = coreotn_mux_ghao_send_idle_nack(digi_handle->coreotn_handle,
                                                  ghao_mux_stage,
                                                  ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_3 : 30b) MUX (Stage 1-2): Synchronize standby page with active page\n");
        ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_3 : 30c) MUX: Wait until config update is done\n");
        ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }


    /* After changing the last tribslot, two bits will be set in
    TC_WR_IPT_VALID.  It's required to remove the "old" valid*/
    if(ret_val == PMC_SUCCESS)
    {
        ret_val = coreotn_mux_ghao_clear_old_last_cal_slot(digi_handle->coreotn_handle, 
                                                           ghao_mux_stage,
                                                           coreotn_lo_chnl_id,
                                                           ghao_increase,
                                                           local_ghao_segment->coreotn_tx_old_trib_slots,
                                                           local_ghao_segment->coreotn_tx_new_trib_slots,
                                                           local_ghao_segment->coreotn_tx_old_cal_mask);

    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge TX INC 3 IDLE_NACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_line_lcr_knife_edge_tx_inc_3_idle_nack */



/*******************************************************************************
* digi_ghao_line_line_lcr_knife_edge_rx_inc_3_idle_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 3 (idle nack) for LINE<->LINE increase
*   segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_rx_inc_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge RX INC 3 IDLE_NACK\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_inc_3 : 30) DMX(Stage 1-2): Wait to receive [IDLE, 0, NACK] on every added TS to exit LCR.\n");
        ret_val = coreotn_dmx_ghao_wait_for_idle_nack(digi_handle->coreotn_handle,
                                                      ghao_mux_stage,
                                                      &coreotn_ho_chnl_id,
                                                      &coreotn_lo_chnl_id,
                                                      &local_ghao_segment->coreotn_rx_tpid,
                                                      local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_inc_3 : 30d) DMX (Stage 1-2): Wait until config update is done\n");
        ret_val = coreotn_dmx_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_inc_3 : 30e) DMX (Stage 1-2): Synchronize standby page with active page\n");
        ret_val = coreotn_dmx_ghao_update_stdby_config_after_knife_edge(digi_handle->coreotn_handle,
                                                                        ghao_mux_stage,
                                                                        coreotn_ho_chnl_id,
                                                                        coreotn_lo_chnl_id,
                                                                        local_ghao_segment->ts_target,
                                                                        local_ghao_segment->new_trib_slot_mask_rx,
                                                                        local_ghao_segment->coreotn_rx_tpid,
                                                                        local_ghao_segment->ghao_increase,
                                                                        local_ghao_segment->cal_to_add_remove_num,
                                                                        local_ghao_segment->coreotn_rx_cal_to_add_remove);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge RX INC 3 IDLE_NACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_line_lcr_knife_edge_rx_inc_3_idle_nack */



/*******************************************************************************
* digi_ghao_line_line_lcr_knife_edge_tx_dec_1_rem_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 1 (rem ack) for LINE<->LINE decrease segment
*   in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_tx_dec_1_rem_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge TX DEC 1 REM_ACK\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_1 : 44) MUX: Send [REM, #a, ACK] on all TS\n");
        ret_val = coreotn_mux_ghao_send_remove_ack(digi_handle->coreotn_handle,
                                                   ghao_mux_stage,
                                                   ghao_segment_id);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge TX DEC 1 REM_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_line_lcr_knife_edge_tx_dec_1_rem_ack */



/*******************************************************************************
* digi_ghao_line_line_lcr_knife_edge_rx_dec_1_rem_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 1 (rem ack) for LINE<->LINE decrease segment
*   in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_rx_dec_1_rem_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge RX DEC 1 REM_ACK\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_dec_1 : 44) DMX: Wait to receive [REM, #a, ACK] on all TS.\n");
        ret_val = coreotn_dmx_ghao_wait_for_remove_ack(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       &coreotn_ho_chnl_id,
                                                       &coreotn_lo_chnl_id,
                                                       &local_ghao_segment->coreotn_rx_tpid,
                                                       local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge RX DEC 1 REM_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_line_lcr_knife_edge_rx_dec_1_rem_ack */



/*******************************************************************************
* digi_ghao_line_line_lcr_knife_edge_tx_dec_2_norm_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 2 (norm ack) for LINE<->LINE decrease segment
*   in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_tx_dec_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge TX DEC 2 NORM_ACK\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_2 : 45) MUX: Once DMX ACK is received Send [NORM, #a, ACK] in all TS that needs to be changed. This is done by setting G.HAO update bit in the MUX. The MUX will automatically do Knife Edge in the next RMF after sending NORM command.\n");
        ret_val = coreotn_mux_ghao_send_norm(digi_handle->coreotn_handle,
                                             ghao_mux_stage,
                                             ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_2 : 45b) MUX: Wait until config update is done\n");
        ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge TX DEC 2 NORM_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_line_lcr_knife_edge_tx_dec_2_norm_ack */



/*******************************************************************************
* digi_ghao_line_line_lcr_knife_edge_rx_dec_2_norm_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 2 (norm ack) for LINE<->LINE decrease segment
*   in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_rx_dec_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge RX DEC 2 NORM_ACK\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_dec_2 : 45) DMX: After ACK cmd is sent in MUX, DMX can receive NORM as soon as one RMF later. Wait to receive NORM in all TS. DMX HW will automatically do Knife edge in the next RMF after receiving NORM Command.\n");
        ret_val = coreotn_dmx_ghao_wait_for_norm_ack(digi_handle->coreotn_handle,
                                                     ghao_mux_stage,
                                                     &coreotn_ho_chnl_id,
                                                     &coreotn_lo_chnl_id,
                                                     &local_ghao_segment->coreotn_rx_tpid,
                                                     local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge RX DEC 2 NORM_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_line_lcr_knife_edge_rx_dec_2_norm_ack */



/*******************************************************************************
* digi_ghao_line_line_lcr_knife_edge_tx_dec_3_idle_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 3 (idle nack) for LINE<->LINE decrease
*   segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_tx_dec_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    BOOL8 mux_passthru;
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT32 cal_add_remove[96];
    UINT32 i;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    mux_passthru = local_ghao_segment->mux_passthru;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge TX DEC 3 IDLE_NACK\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }

    if (ghao_mux_stage == COREOTN_MUX_STAGE_ONE && mux_passthru == TRUE)
    {
        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 46) MUX (Stage 2-3A): Turn valid off on last to-be-removed Calendar (by clear EXP.LO_FIRST_CYCLE).\n");
            ret_val = coreotn_mux_ghao_recfg_exp_lo_calen(digi_handle->coreotn_handle,
                                                          COREOTN_MUX_STAGE_TWO,
                                                          ghao_segment_id,
                                                          local_ghao_segment->coreotn_tx_cal_to_add_remove[local_ghao_segment->cal_to_add_remove_num-1]);
        }
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 47) MPMO (stage 3B): Turn valid off  on the last to-be-removed calendar\n");
        for(i=0;i< local_ghao_segment->fo1_mpmo_userbit_update_pre_post_fw;i++)
        {
            cal_add_remove[i] = local_ghao_segment->coreotn_tx_cal_to_add_remove[local_ghao_segment->cal_to_add_remove_num-(i+1)];
        }
        ret_val = coreotn_mpmo_update_userbit_mask(digi_handle->coreotn_handle,
                                                   COREOTN_MPMO_TO_COREOTN,
                                                   coreotn_lo_chnl_id,
                                                   cal_add_remove,
                                                   local_ghao_segment->fo1_mpmo_userbit_update_pre_post_fw,
                                                   local_ghao_segment->coreotn_tx_old_cal_mask,
                                                   local_ghao_segment->coreotn_mpmo3b_userbits_mask);

    }

    if (ret_val == PMC_SUCCESS)
    {
        /* in the LCR resize, it's required keep the DPI calendar >= to the DCI
        calendar.  It's required to change the ODUKSW calendar after removing
        the userbits */
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : DB) Setup database UTIL_GEN_DB_OCPB_DCS TX\n");
        ret_val = digi_ghao_cpb_db_chnl_rec_update(digi_handle,
                                                   DIGI_GHAO_ODUKSW_COREOTN_COREOTN,
                                                   coreotn_lo_chnl_id,
                                                   local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 47b) ODUk Switch (LINE -> LINE): Change Calendar to final TS\n");
        ret_val = digi_ghao_cpb_dcs_calendar_cfg(digi_handle,
                                                 DIGI_GHAO_ODUKSW_COREOTN_COREOTN,
                                                 coreotn_lo_chnl_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 47c) MPMO (Stage 3B): Decrease XOFF threshold\n");
        for(i=0;i<local_ghao_segment->fo1_mpmo_thresh_pre_post_fw ;i++)
        {
            ret_val = coreotn_mpmo_xoff_thresh_change(digi_handle->coreotn_handle,
                                                      COREOTN_MPMO_TO_COREOTN,
                                                      coreotn_lo_chnl_id,
                                                      ghao_increase);
        }
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 47d) MPMO (Stage 3B): Change FIFO to final size\n");
        ret_val = coreotn_mpmo_ghao_fifo_resize_no_thresh_update(digi_handle->coreotn_handle,
                                                                 COREOTN_MPMO_TO_COREOTN,
                                                                 coreotn_lo_chnl_id,
                                                                 local_ghao_segment->ts_target);
    }

    /* perform XOFF on 1st FMF1 & FMF2 if needed*/
    if (ret_val == PMC_SUCCESS)
    {
        for(i=0;i<digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_thresh_pre_post_fw ;i++)
        {
            PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 48) MUX (EXP Stage 1-2 and Stage 2-3A): Decrease XOFF threshold by 1TS equivalent every 2.5s\n ");
            ret_val = coreotn_odtu_mux_xoff_thresh_update(digi_handle->coreotn_handle,
                                                          ghao_mux_stage,
                                                          coreotn_lo_chnl_id,
                                                          FALSE,
                                                          &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_xoff_thresh);
        }
    }

    if (ret_val == PMC_SUCCESS)
    {
         PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 49) MPMO (stage 3B): Change Calendar to new configuration.\n");
        ret_val = coreotn_mpmo_ghao_scbs3_reprov_force_userbits(digi_handle->coreotn_handle,
                                                                COREOTN_MPMO_TO_COREOTN,
                                                                coreotn_lo_chnl_id,
                                                                local_ghao_segment->ts_target,
                                                                local_ghao_segment->coreotn_tx_new_cal_mask,
                                                                local_ghao_segment->coreotn_tx_new_cal_mask);
    }

    if (ghao_mux_stage == COREOTN_MUX_STAGE_ONE && mux_passthru == TRUE)
    {
        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 50) MUX (Stage 2-3A): Reconfigure MUX passthru to clear configuration on all to-be-removed calendar slots.\n");
            ret_val = coreotn_mux_ghao_recfg_passthru(digi_handle->coreotn_handle,
                                                      COREOTN_MUX_STAGE_TWO,
                                                      ghao_segment_id);
        }

        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 53) MUX (Stage 2-3A): Change FIFO to final size\n");
            ret_val = coreotn_mux_ghao_resize_fifo(digi_handle->coreotn_handle,
                                                   COREOTN_MUX_STAGE_TWO,
                                                   ghao_segment_id);
        }

        for(i=0;i<digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru_thresh_pre_post_fw ;i++)
        {
            PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 53a) MUX (EXP Stage 2-3A): Decrease XOFF threshold by 1TS equivalent every 2.5s\n ");
            ret_val = coreotn_odtu_mux_xoff_thresh_update(digi_handle->coreotn_handle,
                                                          COREOTN_MUX_STAGE_TWO,
                                                          coreotn_lo_chnl_id,
                                                          FALSE,
                                                          &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_passthru_xoff_thresh);
        }

        for(i=0;i<digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fmf2_mux_cal_en_pre_post_fw;i++)
        {
            if (ret_val == PMC_SUCCESS)
            {
               PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 53b) MUX (EXP Stage 2-3A): removed last calendat valid\n ");

                ret_val = coreotn_mux_ghao_passthru_cal_enable_diable(digi_handle->coreotn_handle,
                                                                      local_ghao_segment->coreotn_tx_cal_to_add_remove[local_ghao_segment->cal_to_add_remove_num-(i+1)],
                                                                      FALSE);
            }
        }

    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 54) MUX (Stage 1-2): Change expand FIFO to final size\n");
        ret_val = coreotn_mux_ghao_resize_fifo(digi_handle->coreotn_handle,
                                               ghao_mux_stage,
                                               ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 55) MUX(Stage 1-2): Finish LCR by sending [IDLE, 0, NACK] for every added TS at the RMF.\n");
        ret_val = coreotn_mux_ghao_send_idle_nack(digi_handle->coreotn_handle,
                                                  ghao_mux_stage,
                                                  ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 55b) MUX (Stage 1-2): Synchronize standby page with active page\n");
        ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 55c) MUX: Wait until config update is done\n");
        ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge TX DEC 3 IDLE_NACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_line_lcr_knife_edge_tx_dec_3_idle_nack */



/*******************************************************************************
* digi_ghao_line_line_lcr_knife_edge_rx_dec_3_idle_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 3 (idle nack) for LINE<->LINE decrease
*   segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_rx_dec_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge RX DEC 3 IDLE_NACK\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_dec_3 : 55a) MPMA(Stage 3B): Change  FIFO size to final TS\n");
        ret_val = coreotn_mpma_ghao_fifo_resize(digi_handle->coreotn_handle,
                                                COREOTN_MPMA_TO_COREOTN,
                                                coreotn_lo_chnl_id,
                                                local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_dec_3 : 55b) DMX(Stage 1-2): Wait to receive [IDLE, 0, NACK] on every added TS to exit LCR.\n");
        ret_val = coreotn_dmx_ghao_wait_for_idle_nack(digi_handle->coreotn_handle,
                                                      ghao_mux_stage,
                                                      &coreotn_ho_chnl_id,
                                                      &coreotn_lo_chnl_id,
                                                      &local_ghao_segment->coreotn_rx_tpid,
                                                      local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_dec_3 : 55d) DMX (Stage 1-2): Wait until config update is done\n");
        ret_val = coreotn_dmx_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_dec_3 : 55e) DMX (Stage 1-2): Synchronize standby page with active page\n");
        ret_val = coreotn_dmx_ghao_update_stdby_config_after_knife_edge(digi_handle->coreotn_handle,
                                                                        ghao_mux_stage,
                                                                        coreotn_ho_chnl_id,
                                                                        coreotn_lo_chnl_id,
                                                                        local_ghao_segment->ts_target,
                                                                        local_ghao_segment->new_trib_slot_mask_rx,
                                                                        local_ghao_segment->coreotn_rx_tpid,
                                                                        local_ghao_segment->ghao_increase,
                                                                        local_ghao_segment->cal_to_add_remove_num,
                                                                        local_ghao_segment->coreotn_rx_cal_to_add_remove);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge RX DEC 3 IDLE_NACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_line_lcr_knife_edge_rx_dec_3_idle_nack */



/*******************************************************************************
* digi_ghao_line_line_lcr_knife_edge_tx_dec_4_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 4 (rp) for LINE<->LINE decrease segment
*   in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_tx_dec_4_rp(digi_handle_t *digi_handle,
                                                                UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT32 dummy_userbit_mask[3] = {0,0,0};
    UINT32 dummy_set_mask[3] = {0,0,0};

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge TX DEC 4 RP\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_4 : 56) After receiving IDLE NACK in the DMX, then the MUX  can set RP = 0.\n");
        ret_val = coreotn_mux_ghao_send_rp(digi_handle->coreotn_handle,
                                           ghao_mux_stage,
                                           ghao_segment_id,
                                           0);
    }


    /* Perform clean-up */
    /* take MPMOs out of custom userbit mode */
    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_4 : 56b) Take MPMO (Stage 3B) out of custom userbit mode\n");
        ret_val = coreotn_mpmo_set_custom_userbit_mask(digi_handle->coreotn_handle,
                                                       coreotn_lo_chnl_id,
                                                       COREOTN_MPMO_TO_COREOTN,
                                                       FALSE,
                                                       dummy_userbit_mask,
                                                       dummy_set_mask,
                                                       FALSE);
    }

    
    /* After changing the last tribslot, two bits will be set in
    TC_WR_IPT_VALID.  It's required to remove the "old" valid*/
    if(ret_val == PMC_SUCCESS)
    {
        ret_val = coreotn_mux_ghao_clear_old_last_cal_slot(digi_handle->coreotn_handle, 
                                                           ghao_mux_stage,
                                                           coreotn_lo_chnl_id,
                                                           ghao_increase,
                                                           local_ghao_segment->coreotn_tx_old_trib_slots,
                                                           local_ghao_segment->coreotn_tx_new_trib_slots,
                                                           local_ghao_segment->coreotn_tx_old_cal_mask);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge TX DEC 4 RP\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_line_lcr_knife_edge_tx_dec_4_rp */



/*******************************************************************************
* digi_ghao_line_line_lcr_knife_edge_rx_dec_4_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 4 (rp) for LINE<->LINE decrease segment
*   in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_lcr_knife_edge_rx_dec_4_rp(digi_handle_t *digi_handle,
                                                                UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge RX DEC 4 RP\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_dec_4 : 56) DMX, wait to receive RP = 0 and terminate G.HAO protocol.\n");
        ret_val = coreotn_dmx_ghao_wait_for_rp(digi_handle->coreotn_handle,
                                               ghao_mux_stage,
                                               &coreotn_ho_chnl_id,
                                               &coreotn_lo_chnl_id,
                                               &local_ghao_segment->coreotn_rx_tpid,
                                               local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }
    

    PMC_LOG_TRACE("End G.HAO LCR knife edge RX DEC 4 RP\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_line_lcr_knife_edge_rx_dec_4_rp */



/*******************************************************************************
* digi_ghao_line_line_bwr_setup_tx_1_tscc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the BWR setup step 1 (tscc) for LINE<->LINE decrease segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_bwr_setup_tx_1_tscc(digi_handle_t *digi_handle,
                                                         UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    UINT32 ts_target;
    coreotn_mux_stage_t ghao_mux_stage;
    BOOL8 ghao_increase;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    ts_target = local_ghao_segment->ts_target;
    ghao_increase = local_ghao_segment->ghao_increase;


    PMC_LOG_TRACE("Start G.HAO BWR setup TX 1 TSCC\n");

    /* perform bwr_setup operation */

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR set-up tx_1 : 33/4) MPMO (stage 3B): set target rate and set into GMP special mode. (get ready for ramp)  \n");
        ret_val = coreotn_ghao_mpmo_bwr_rate_param_set(digi_handle->coreotn_handle,
                                                       COREOTN_MPMO_TO_COREOTN,
                                                       coreotn_lo_chnl_id,
                                                       ts_target,
                                                       ghao_increase);
    }

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR set-up rx_1 : 34/5) MPMA (Stage 3B) : Disable MPMA DSP\n");
        /* Note: Although it can be seen as RX datapath, this step must be done right after step 33/4 */
        ret_val = coreotn_ghao_mpma_dsp_enable_set(digi_handle->coreotn_handle,
                                                   COREOTN_MPMA_TO_COREOTN,
                                                   coreotn_lo_chnl_id,
                                                   FALSE,
                                                   0);
    }

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR set-up tx_1 : 35/6) MUX (Stage 1-2): Set TSCC bit on all TS to be sent  \n");
        ret_val = coreotn_mux_ghao_send_tscc(digi_handle->coreotn_handle,
                                             ghao_mux_stage,
                                             ghao_segment_id,
                                             1);
    }


    PMC_LOG_TRACE("End G.HAO BWR setup TX 1 TSCC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_bwr_setup_tx_1_tscc */



/*******************************************************************************
* digi_ghao_line_line_bwr_setup_rx_1_tscc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the BWR setup step 1 (tscc) for LINE<->LINE decrease segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_bwr_setup_rx_1_tscc(digi_handle_t *digi_handle,
                                                         UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    UINT32 ts_target;
    coreotn_mux_stage_t ghao_mux_stage;
    BOOL8 ghao_increase;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    ts_target = local_ghao_segment->ts_target;
    ghao_increase = local_ghao_segment->ghao_increase;


    PMC_LOG_TRACE("Start G.HAO BWR setup RX 1 TSCC\n");

    /* perform bwr_setup operation */

    if(ret_val == PMC_SUCCESS)
    {
        /* set up ODUKSC in COREOTN to begin ramp */
        PMC_LOG_TRACE("G.HAO BWR set-up rx_1 : 32b/3b) ODUk SC (COREOTN): Set up ODUkSc to begin RAMP. But don't begin ramp \n");
        ret_val = coreotn_oduksc_ramp_prov(digi_handle->coreotn_handle,
                                           ghao_mux_stage,
                                           coreotn_lo_chnl_id,
                                           ts_target);
    }

    
    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR start : 39b/11b) ODUkSc (COREOTN): Begins ramp after RFRM(3A) BWR_IND received.\n");
        ret_val = coreotn_oduksc_ramp_start(digi_handle->coreotn_handle,
                                            local_ghao_segment->ghao_mux_stage,
                                            coreotn_lo_chnl_id);
    }


    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR set-up rx_1 : 36/7) DMX(Stage 1-2): Wait to receive TSCC bit on all TS  \n");
        ret_val = coreotn_dmx_ghao_wait_for_tscc(digi_handle->coreotn_handle,
                                                 ghao_mux_stage,
                                                 &coreotn_ho_chnl_id,
                                                 &coreotn_lo_chnl_id,
                                                 &local_ghao_segment->coreotn_rx_tpid,
                                                 local_ghao_segment->coreotn_rx_trib_slots_add_rem,
                                                 1);
    }


    PMC_LOG_TRACE("End G.HAO BWR setup RX 1 TSCC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_bwr_setup_rx_1_tscc */



/*******************************************************************************
* digi_ghao_line_line_bwr_ramp_start_tx
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Starts the ramp in TX. Triggers the firmware routine that will perform the
*   per TS ramp in the datapath.  It is required that this API is called
*   on all intermediate nodes before it is called for source nodes.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_bwr_ramp_start_tx(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    digi_ghao_seg_ctrl_t *local_ghao_segment;
    BOOL mux_passthru;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    mux_passthru = local_ghao_segment->mux_passthru;

    /* perform bwr start operation */

    PMC_LOG_TRACE("Start G.HAO BWR start TX\n");

    /* Update pre-firmware operations done */
    if (local_ghao_segment->ghao_increase)
    {
        if(mux_passthru == FALSE)
        {
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_thresh_pre_post_fw = MPMO_XOFF_INC_PRE_WO_PASSTHRU;
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo2_mpmo_thresh_pre_post_fw = 0;
              /*on an increase, one xoff threshold adjustment is performed before the firmware
              to avoid underflows in the EXP CFC*/
              if(digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start == 1)
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_thresh_pre_post_fw = FMF1_MUX_XOFF_INC_PRE_W0_PASSTHRU_FROM1;
              }
              else
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_thresh_pre_post_fw = FMF1_MUX_XOFF_INC_PRE_W0_PASSTHRU;
              }
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_userbit_update_pre_post_fw = MPMO_USERBIT_INC_PRE_WO_PASSTHRU;
        }
        else
        {
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_thresh_pre_post_fw = MPMO_XOFF_INC_PRE_W_PASSTHRU;
              
              if(digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start == 1)
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_thresh_pre_post_fw = FMF1_MUX_XOFF_INC_PRE_W_PASSTHRU_FROM1;
              }
              else
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_thresh_pre_post_fw = FMF1_MUX_XOFF_INC_PRE_W_PASSTHRU;
              }

              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_userbit_update_pre_post_fw = MPMO_USERBIT_INC_PRE_WO_PASSTHRU; /*MPMO_USERBIT_INC_PRE_W_PASSTHRU;*/

              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru_thresh_pre_post_fw = FMF2_MUX_XOFF_INC_PRE_W_PASSTHRU;
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fmf2_mux_cal_en_pre_post_fw = FMF2_MUX_CAL_EN_INC_PRE_W_PASSTHRU;
        }
    }
    else
    {
        if(mux_passthru == FALSE)
        {
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_thresh_pre_post_fw = MPMO_USERBIT_DEC_PRE_WO_PASSTHRU;
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo2_mpmo_thresh_pre_post_fw = 0;
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_thresh_pre_post_fw = FMF1_MUX_XOFF_DEC_PRE_W0_PASSTHRU;
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo2_mpmo_userbit_update_pre_post_fw = 0;

              /*decreasing by 1 is a special case for userbit adjustments*/
              if( ((digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start 
                   - digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_target) == 1) &&
                   MPMO_USERBIT_DEC_PRE_WO_PASSTHRU > 1)
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_userbit_update_pre_post_fw = 1;
              }
              else
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_userbit_update_pre_post_fw = MPMO_USERBIT_DEC_PRE_WO_PASSTHRU;
              }
        }
        else
        {
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_thresh_pre_post_fw = MPMO_XOFF_DEC_POST_W_PASSTHRU;
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_thresh_pre_post_fw = FMF1_MUX_XOFF_DEC_POST_W_PASSTHRU;
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru_thresh_pre_post_fw = FMF2_MUX_XOFF_DEC_POST_W_PASSTHRU;

              /*decreasing by 1 is a special case for userbit adjustments*/
              if( ((digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start 
                   - digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_target) == 1) &&
                   MPMO_USERBIT_DEC_POST_W_PASSTHRU > 1)
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_userbit_update_pre_post_fw = 1;
              }
              else
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_userbit_update_pre_post_fw = MPMO_USERBIT_DEC_POST_W_PASSTHRU;
              }

              /*decreasing by 1 is a special case for fmf2 cal adjustments*/
              if( ((digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start 
                   - digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_target) == 1) &&
                   FMF2_MUX_CAL_EN_DEC_POST_W_PASSTHRU > 1)
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fmf2_mux_cal_en_pre_post_fw = 1;
              }
              else
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fmf2_mux_cal_en_pre_post_fw = FMF2_MUX_CAL_EN_DEC_POST_W_PASSTHRU;
              }
        }
    }


    PMC_LOG_TRACE("End G.HAO BWR start TX\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_bwr_ramp_start_tx */



/*******************************************************************************
* digi_ghao_line_line_bwr_ramp_start_rx
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Starts the ramp in RX. Triggers the firmware routine that will perform the
*   per TS ramp in the datapath.  It is required that this API is called
*   on all intermediate nodes before it is called for source nodes.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_line_bwr_ramp_start_rx(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT8 coreotn_lo_chnl_id;


    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;

    /* perform bwr start operation */

    PMC_LOG_TRACE("Start G.HAO BWR start RX\n");


    PMC_LOG_TRACE("Start G.HAO BWR start RX\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_bwr_ramp_start_rx */



/*******************************************************************************
* digi_ghao_line_line_bwr_ramp_end_tx_1_tscc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the BWR end step 1 (tscc) for LINE<->LINE decrease segment in TX
*  This need to be called when the ramp is complete to put to datapath
*  back into normal operating more.
*
*  For intermediate nodes, call after source nodes have
*  completed.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_line_line_bwr_ramp_end_tx_1_tscc(digi_handle_t *digi_handle,
                                                            UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    digi_ghao_seg_ctrl_t *local_ghao_segment;
    DOUBLE odu_bitrate;
    UINT32 ts_target;
    UINT8 coreotn_lo_chnl_id;
    UINT8 coreotn_ho_chnl_id;
    UINT32 oif_pkt_size;
    UINT32 oif_header_size;
    coreotn_mux_stage_t ghao_mux_stage;

    UINT32 pkt_size_ptr;
    UINT32 pkt_per_int_ptr;
    UINT32 pkt_per_n_ptr;
    UINT32 pkt_per_d_ptr;
    DOUBLE eps_ptr;


    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ts_target = local_ghao_segment->ts_target;
    
    oif_pkt_size = local_ghao_segment->oif_pkt_size;
    oif_header_size = local_ghao_segment->header_size;

    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;


    /* perform bwr end operation */

    PMC_LOG_TRACE("Start G.HAO BWR end TX 1 TSCC\n");

    if (ts_target <= 8)
    {
        odu_bitrate = (ts_target * UTIL_GLOBAL_ODU2_TS_RATE);
    } 
    else if (ts_target <= 32)
    {
        odu_bitrate = (ts_target * UTIL_GLOBAL_ODU3_TS_RATE);
    } 
    else 
    {
        odu_bitrate = (ts_target * UTIL_GLOBAL_ODU4_TS_RATE);
    }
    

    if(ret_val == PMC_SUCCESS)
    {

        /* calculate new opsa parameters based on new client rate.
        LINE-CORE-LINE datapaths need to use OIF calculator */
        ret_val = coreotn_oif_pkt_period_calc(digi_handle->coreotn_handle,
                                              odu_bitrate,
                                              oif_pkt_size,
                                              oif_header_size,
                                              &pkt_size_ptr,
                                              &pkt_per_int_ptr,
                                              &pkt_per_n_ptr,
                                              &pkt_per_d_ptr,
                                              &eps_ptr);
    }


    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end tx_1 : 43/15) MPMO (Stage 3B): Set new Packet size, Cn_base(DSP) and take it out of GMP special mode. \n");
        ret_val = coreotn_ghao_mpmo_bwr_end_param_set(digi_handle->coreotn_handle,
                                                      COREOTN_MPMO_TO_COREOTN,
                                                      coreotn_lo_chnl_id,
                                                      odu_bitrate,
                                                      pkt_size_ptr,
                                                      pkt_per_int_ptr,
                                                      pkt_per_n_ptr,
                                                      pkt_per_d_ptr,
                                                      eps_ptr);
    }

    if (local_ghao_segment->ghao_increase)
    {
        if(ret_val == PMC_SUCCESS)
        {

            PMC_LOG_TRACE("G.HAO BWR end tx_1 : 44) MUX (Stage 1-2): Do a config_update to disable PREV_CN_SEL \n");

            ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                               ghao_mux_stage,
                                                               ghao_segment_id);        
        }

        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO BWR end tx_1 : 44b) MUX: Wait until config update is done\n");
            ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                               ghao_mux_stage,
                                                               coreotn_ho_chnl_id);
        }

        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO BWR end tx_1 : 44c) MUX (Stage 1-2): Synchronize standby page with active page\n");
            ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           ghao_segment_id);
        }

        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO BWR end tx_1 : 44d) MUX: Wait until config update is done\n");
            ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                               ghao_mux_stage,
                                                               coreotn_ho_chnl_id);
        }
    }

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end tx_1 : 45a/16a) MUX (Stage 1-2): Unset TSCC. \n");
        ret_val = coreotn_mux_ghao_send_tscc(digi_handle->coreotn_handle,
                                             ghao_mux_stage,
                                             ghao_segment_id,
                                             0);
    }


    PMC_LOG_TRACE("End G.HAO BWR end TX 1 TSCC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_bwr_ramp_end_tx_1_tscc */



/*******************************************************************************
* digi_ghao_line_line_bwr_ramp_end_rx_1_tscc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the BWR end step 1 (tscc) for LINE<->LINE decrease segment in RX
*  This need to be called when the ramp is complete to put to datapath
*  back into normal operating more.
*
*  For intermediate nodes, call after source nodes have
*  completed.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_line_line_bwr_ramp_end_rx_1_tscc(digi_handle_t *digi_handle,
                                                            UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    digi_ghao_seg_ctrl_t *local_ghao_segment;
    DOUBLE odu_bitrate;
    UINT32 ts_target;
    UINT8 coreotn_lo_chnl_id;
    UINT8 coreotn_ho_chnl_id;
    UINT32 oif_pkt_size;
    UINT32 oif_header_size;
    coreotn_mux_stage_t ghao_mux_stage;

    UINT32 pkt_size_ptr;
    UINT32 pkt_per_int_ptr;
    UINT32 pkt_per_n_ptr;
    UINT32 pkt_per_d_ptr;
    DOUBLE eps_ptr;


    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ts_target = local_ghao_segment->ts_target;
    
    oif_pkt_size = local_ghao_segment->oif_pkt_size;
    oif_header_size = local_ghao_segment->header_size;

    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;


    /* perform bwr end operation */

    PMC_LOG_TRACE("Start G.HAO BWR end RX 1 TSCC\n");

    if (ts_target <= 8)
    {
        odu_bitrate = (ts_target * UTIL_GLOBAL_ODU2_TS_RATE);
    } 
    else if (ts_target <= 32)
    {
        odu_bitrate = (ts_target * UTIL_GLOBAL_ODU3_TS_RATE);
    } 
    else 
    {
        odu_bitrate = (ts_target * UTIL_GLOBAL_ODU4_TS_RATE);
    }
    

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end rx_1 : 45b/16b) DMX (Stage 1-2): Wait till TSCC is unset. \n");
        ret_val = coreotn_dmx_ghao_wait_for_tscc(digi_handle->coreotn_handle,
                                                 ghao_mux_stage,
                                                 &coreotn_ho_chnl_id,
                                                 &coreotn_lo_chnl_id,
                                                 &local_ghao_segment->coreotn_rx_tpid,
                                                 local_ghao_segment->coreotn_rx_trib_slots_add_rem,
                                                 0);
    }

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end rx_1 : 46a/17a) MPMA (Stage 3B): Set new Packet size, period and MPMA_DSP gain. \n");
        ret_val = coreotn_ghao_mpma_bwr_end_param_set(digi_handle->coreotn_handle,
                                                      COREOTN_MPMA_TO_COREOTN,
                                                      coreotn_lo_chnl_id,
                                                      odu_bitrate,
                                                      oif_pkt_size,
                                                      oif_header_size,
                                                      FALSE,
                                                      &pkt_size_ptr,
                                                      &pkt_per_int_ptr,
                                                      &pkt_per_n_ptr,
                                                      &pkt_per_d_ptr,
                                                      &eps_ptr);
    }

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end rx_1 : 46b/17b) MPMA (Stage 3B): Turn on DSP. \n");
        ret_val = coreotn_ghao_mpma_dsp_enable_set(digi_handle->coreotn_handle,
                                                   COREOTN_MPMA_TO_COREOTN,
                                                   coreotn_lo_chnl_id,
                                                   TRUE,
                                                   ts_target);
    }

    
    PMC_LOG_TRACE("G.HAO BWR end rx_1 : 47) Wait 3ms \n");

    /* wait 20 ms */
    PMC_Q_USLEEP(digi_handle,20000);


    PMC_LOG_TRACE("End G.HAO BWR end RX 1 TSCC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_bwr_ramp_end_rx_1_tscc */



/*******************************************************************************
* digi_ghao_line_line_bwr_ramp_end_tx_3_inc_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the BWR end step 3 (rp) for LINE<->LINE decrease segment in TX
*  This API will deprovision the G.HAO segment and free up the ghao_segment_id
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_line_line_bwr_ramp_end_tx_3_inc_rp(digi_handle_t *digi_handle,
                                                              UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT32 ts_target;
    UINT8 coreotn_lo_chnl_id;
    UINT8 coreotn_ho_chnl_id;
    UINT32 oif_pkt_size;
    UINT32 oif_header_size;
    coreotn_mux_stage_t ghao_mux_stage;
    UINT32 dummy_userbit_mask[3] = {0,0,0};
    UINT32 dummy_set_mask[3] = {0,0,0};

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ts_target = local_ghao_segment->ts_target;
    
    oif_pkt_size = local_ghao_segment->oif_pkt_size;
    oif_header_size = local_ghao_segment->header_size;

    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;


    /* perform bwr end operation */

    PMC_LOG_TRACE("Start G.HAO BWR end TX 3 RP\n");

    if (!local_ghao_segment->ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end tx_3 : 50a) MUX (Stage 1-2): Remove RP bit out of MUX \n");
        ret_val = coreotn_mux_ghao_send_rp(digi_handle->coreotn_handle,
                                           ghao_mux_stage,
                                           ghao_segment_id,
                                           0);
    }

    /* Perform clean-up */
    /* take MPMOs out of custom userbit mode */

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end tx_3 : 50c) Take MPMO (Stage 3B) out of custer userbit mode\n");
        ret_val = coreotn_mpmo_set_custom_userbit_mask(digi_handle->coreotn_handle,
                                                       coreotn_lo_chnl_id,
                                                       COREOTN_MPMO_TO_COREOTN,
                                                       FALSE,
                                                       dummy_userbit_mask,
                                                       dummy_set_mask,
                                                       FALSE);
    }


    PMC_LOG_TRACE("End G.HAO BWR end TX 3 RP\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_bwr_ramp_end_tx_3_inc_rp */



/*******************************************************************************
* digi_ghao_line_line_bwr_ramp_end_rx_3_inc_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the BWR end step 3 (rp) for LINE<->LINE decrease segment in RX
*  This API will deprovision the G.HAO
*  segment and free up the ghao_segment_id
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_line_line_bwr_ramp_end_rx_3_inc_rp(digi_handle_t *digi_handle,
                                                              UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT32 ts_target;
    UINT8 coreotn_lo_chnl_id;
    UINT8 coreotn_ho_chnl_id;
    UINT32 oif_pkt_size;
    UINT32 oif_header_size;
    coreotn_mux_stage_t ghao_mux_stage;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ts_target = local_ghao_segment->ts_target;
    
    oif_pkt_size = local_ghao_segment->oif_pkt_size;
    oif_header_size = local_ghao_segment->header_size;

    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;


    /* perform bwr end operation */

    PMC_LOG_TRACE("Start G.HAO BWR end RX 3 RP\n");

    if (!local_ghao_segment->ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end rx_3 : 50b) Wait in DMX for RP bit to be unset. G. HAO is finished. \n");
        ret_val = coreotn_dmx_ghao_wait_for_rp(digi_handle->coreotn_handle,
                                               ghao_mux_stage,
                                               &coreotn_ho_chnl_id,
                                               &coreotn_lo_chnl_id,
                                               &local_ghao_segment->coreotn_rx_tpid,
                                               local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    PMC_LOG_TRACE("End G.HAO BWR end RX 3 RP\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_line_bwr_ramp_end_rx_3_inc_rp */






/******************************************************************************** 
* This section of the file contains C functions for G.HAO LINE <-> ILKN segement.
********************************************************************************/

/*******************************************************************************
* digi_ghao_line_ilkn_lcr_size_tx_inc_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the LCR step for LINE<->ILKN increase segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_tx_inc_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    UINT8 sifd_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    UINT8 coreotn_tx_tpid;
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    BOOL mux_passthru;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = (UINT8) local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    coreotn_tx_tpid = local_ghao_segment->coreotn_tx_tpid;
    mux_passthru = local_ghao_segment->mux_passthru;
    
    PMC_LOG_TRACE("Start G.HAO LCR resize TX INC RP\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    /* Initialize G.HAO in TSBs that need to be initialized */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 0) Extract added/removed calendar slots for MUX\n");
        ret_val = coreotn_ghao_db_chnl_get_masks(digi_handle->coreotn_handle,
                                                 UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,
                                                 ghao_mux_stage,
                                                 coreotn_ho_chnl_id,
                                                 coreotn_lo_chnl_id,
                                                 local_ghao_segment->coreotn_tx_new_trib_slots,
                                                 local_ghao_segment->coreotn_tx_old_cal_mask,
                                                 local_ghao_segment->coreotn_tx_new_cal_mask,
                                                 &local_ghao_segment->cal_to_add_remove_num,
                                                 local_ghao_segment->coreotn_tx_cal_to_add_remove);
    }

    if (ret_val == PMC_SUCCESS)
    {
        digi_ghao_gen_slots_add_rem_from_masks(local_ghao_segment->coreotn_tx_old_cal_mask,
                                               local_ghao_segment->coreotn_tx_new_cal_mask,
                                               local_ghao_segment->coreotn_tx_cal_slots_add_rem);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc_rp : 0a) MUX INIT: ghao_mux_stage=%d, ghao_id=%d, ho_chnl_id=%d, lo_chnl_id=%d, tbid=%d, tribslots=%d\n", ghao_mux_stage, ghao_segment_id, coreotn_ho_chnl_id, coreotn_lo_chnl_id, coreotn_tx_tpid, local_ghao_segment->ts_target);
        ret_val = coreotn_mux_ghao_init(digi_handle->coreotn_handle,
                                        ghao_mux_stage,
                                        ghao_segment_id,
                                        coreotn_ho_chnl_id, 
                                        coreotn_lo_chnl_id,
                                        coreotn_tx_tpid,
                                        local_ghao_segment->coreotn_tx_cal_slots_add_rem);
    }

    if(mux_passthru)
    {

        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO LCR resize tx_inc_rp : 0b) MUX INIT (passthru mux): ghao_mux_stage=%d, ghao_id=%d, ho_chnl_id=%d, lo_chnl_id=%d, tbid=%d, tribslots=%d\n", ghao_mux_stage, ghao_segment_id, coreotn_ho_chnl_id, coreotn_lo_chnl_id, coreotn_tx_tpid, local_ghao_segment->ts_target);
              ret_val = coreotn_mux_ghao_init(digi_handle->coreotn_handle,
                                            COREOTN_MUX_STAGE_TWO,
                                            ghao_segment_id,
                                            coreotn_lo_chnl_id, 
                                            coreotn_lo_chnl_id,
                                            coreotn_tx_tpid,
                                            local_ghao_segment->coreotn_tx_cal_slots_add_rem);
        }

    }


    /* Link Connection Resize (FIFO and Calendar Setup): */
    /* 1)	Once Control plane is ready. Send: */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc_rp : 1) MUX (Stage 1-2): [ADD, #a, NACK] and RP = 1, TSCC = 0 on all added Trib-slots.\n");
        ret_val = coreotn_mux_ghao_send_add_nack(digi_handle->coreotn_handle,
                                                 ghao_mux_stage,
                                                 ghao_segment_id);
    }


    PMC_LOG_TRACE("End G.HAO LCR resize TX INC RP\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_lcr_size_tx_inc_rp */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_size_rx_inc_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the LCR step for LINE<->ILKN increase segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_rx_inc_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    UINT8 sifd_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    UINT8 coreotn_rx_tpid;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = (UINT8) local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    coreotn_rx_tpid = local_ghao_segment->coreotn_rx_tpid;

    
    PMC_LOG_TRACE("Start G.HAO LCR resize RX INC RP\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    /* Initialize G.HAO in TSBs that need to be initialized */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc_rp : 0) DMX INIT: ghao_mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d, tribslots=%d\n", ghao_mux_stage, coreotn_ho_chnl_id, coreotn_lo_chnl_id, local_ghao_segment->ts_target);
        ret_val = coreotn_dmx_ghao_init(digi_handle->coreotn_handle,
                                        ghao_mux_stage,
                                        coreotn_lo_chnl_id,
                                        coreotn_rx_tpid,
                                        local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    /* Link Connection Resize (FIFO and Calendar Setup): */
    /* 1)	Once Control plane is ready. Send: */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc_rp : 1) DMX (Stage 1-2): Wait to receive RP = 1 and observe [ADD, #a, NACK] and TSCC = 0 in all new TS.\n");
        ret_val = coreotn_dmx_ghao_wait_for_add_nack(digi_handle->coreotn_handle,
                                                     ghao_mux_stage,
                                                     &coreotn_ho_chnl_id,
                                                     &coreotn_lo_chnl_id,
                                                     &coreotn_rx_tpid,
                                                     local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }



    PMC_LOG_TRACE("End G.HAO LCR resize RX INC RP\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_lcr_size_rx_inc_rp */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_size_tx_inc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR resize step for LINE<->ILKN increase segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_tx_inc(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    UINT8 sifd_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    BOOL8 mux_passthru;
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT32 i;
    UINT32 num_add_rem;
    UINT32 cal_add_remove[96];
    UINT32 fmf1_xoff_pre_adj;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = (UINT8) local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    mux_passthru = local_ghao_segment->mux_passthru;

    
    PMC_LOG_TRACE("Start G.HAO LCR resize TX INC\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    /* Databases for TX datapath */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : DB) Setup database UTIL_GEN_DB_LINE_CORE_OTN_TX_DB\n");
        ret_val = coreotn_ghao_db_chnl_rec_update(digi_handle->coreotn_handle,
                                                  UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,
                                                  ghao_mux_stage,
                                                  coreotn_ho_chnl_id,
                                                  coreotn_lo_chnl_id,
                                                  local_ghao_segment->coreotn_tx_new_trib_slots,
                                                  local_ghao_segment->coreotn_tx_old_cal_mask,
                                                  local_ghao_segment->coreotn_tx_new_cal_mask,
                                                  &local_ghao_segment->cal_to_add_remove_num,
                                                  local_ghao_segment->coreotn_tx_cal_to_add_remove);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : DB) Setup database UTIL_GEN_DB_OCPB_DCS TX\n");
        ret_val = digi_ghao_cpb_db_chnl_rec_update(digi_handle,
                                                   DIGI_GHAO_ODUKSW_SIFD1_COREOTN,
                                                   coreotn_lo_chnl_id,
                                                   local_ghao_segment->ts_target);
    }


    /* ILKN -> MUX direction: */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 10) MUX (Stage 1-2): Change EXPAND CFC FIFO to new TS value. Don't increase XOFF threshold.\n");
        ret_val = coreotn_mux_ghao_resize_fifo(digi_handle->coreotn_handle,
                                               ghao_mux_stage,
                                               ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 11) MUX (Stage 1-2): Configure all the standby pages to new configuration including STS_SW, new M value.  Configure prev_cn in standby page.\n");
        ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       ghao_segment_id);
    }

    if (ghao_mux_stage == COREOTN_MUX_STAGE_ONE && mux_passthru == TRUE)
    {
        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 12) MUX (Stage 2-3A): Change FIFO size to final TS size. Do not change XOFF threshold\n");
            ret_val = coreotn_mux_ghao_resize_fifo(digi_handle->coreotn_handle,
                                                   COREOTN_MUX_STAGE_TWO,
                                                   ghao_segment_id);
        }
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 13) MPMO (stage 3B): Change FIFO Size to final TS size. Do not change XOFF threshold.\n");
        ret_val = coreotn_mpmo_ghao_fifo_resize_no_thresh_update(digi_handle->coreotn_handle,
                                                                 COREOTN_MPMO_TO_COREOTN,
                                                                 coreotn_lo_chnl_id,
                                                                 local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {

        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 14) ODUk Switch (SIFD -> LINE): Change the FIFO size to final TS\n");
        ret_val = coreotn_oduksw_ghao_queue_resize_zone_update(digi_handle->coreotn_handle,
                                                               COREOTN_ODUKSW_OUTPUT_COREOTN,
                                                               coreotn_lo_chnl_id,
                                                               local_ghao_segment->ts_target*2,
                                                               local_ghao_segment->ts_target,
                                                               ghao_increase);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 17) ODUk Switch (SIFD -> LINE) : Change Calendar to final TS count\n");
        ret_val = digi_ghao_cpb_dcs_calendar_cfg(digi_handle,
                                                 DIGI_GHAO_ODUKSW_SIFD1_COREOTN,
                                                 coreotn_lo_chnl_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 18) MPMO (Stage 3B): Change Calendar to new TS Calendar, but don't enable 'valid' into those Calendar, meaning set the new Cal. Entries user bits to be disabled.\n");
        ret_val = coreotn_mpmo_ghao_scbs3_reprov_force_userbits(digi_handle->coreotn_handle,
                                                                COREOTN_MPMO_TO_COREOTN,
                                                                coreotn_lo_chnl_id,
                                                                local_ghao_segment->ts_target,
                                                                local_ghao_segment->coreotn_tx_new_cal_mask,
                                                                local_ghao_segment->coreotn_tx_old_cal_mask);
    }

    if (ghao_mux_stage == COREOTN_MUX_STAGE_ONE && mux_passthru == TRUE)
    {
        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO LCR resize tx_inc : 19) MUX (Stage 2-3A) : Do passthru re-config to new TS count, but don't enable the additional calendars\n");
            ret_val = coreotn_mux_ghao_recfg_passthru(digi_handle->coreotn_handle,
                                                      COREOTN_MUX_STAGE_TWO,
                                                      ghao_segment_id);
        }
    }

    if(mux_passthru)
    {
        num_add_rem  = MPMO_USERBIT_INC_PRE_WO_PASSTHRU;
    }
    else
    {
        num_add_rem  = MPMO_USERBIT_INC_PRE_WO_PASSTHRU;
    }


    /* Initialize userbit to old cal mask for COREOTN and MAPOTN */
    for(i = 0; i< 3; i++)
    {
        local_ghao_segment->coreotn_mpmo3b_userbits_mask[i] = local_ghao_segment->coreotn_tx_old_cal_mask[i];
    }


        
    if(ret_val == PMC_SUCCESS)
    {
        for(i=0;i<num_add_rem;i++)
        {
            if (ret_val == PMC_SUCCESS)
            {
                cal_add_remove[i] = local_ghao_segment->coreotn_tx_cal_to_add_remove[i];
            }
        }

        PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_1 : 28) MPMO (Stage 3B): Enable 'valid' on one of the to-be-added cal slots, meaning set the new Cal. Entries user bits to be enabled.\n");

        ret_val = coreotn_mpmo_update_userbit_mask(digi_handle->coreotn_handle,
                                                           COREOTN_MPMO_TO_COREOTN,
                                                           coreotn_lo_chnl_id,
                                                           cal_add_remove,
                                                           num_add_rem,
                                                           local_ghao_segment->coreotn_tx_new_cal_mask,
                                                           local_ghao_segment->coreotn_mpmo3b_userbits_mask);
    }


    if (ghao_mux_stage == COREOTN_MUX_STAGE_ONE && mux_passthru == TRUE)
    {
        for(i=0;i<FMF2_MUX_CAL_EN_INC_PRE_W_PASSTHRU;i++)
        {
            if (ret_val == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_1 : 29a) MUX (Stage 2-3A) : Enable lo_calen on the same calendar slots by setting the EXPAND.LO_FIRST_CYCLE[cal_slot_id] to 1\n");
                ret_val = coreotn_mux_ghao_passthru_cal_enable_diable(digi_handle->coreotn_handle,
                                                                      local_ghao_segment->coreotn_tx_cal_to_add_remove[i],
                                                                      TRUE);
            }
        }




        if(digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start == 1)
        {
            fmf1_xoff_pre_adj = FMF1_MUX_XOFF_INC_PRE_W_PASSTHRU_FROM1;
        }
        else
        {
            fmf1_xoff_pre_adj = FMF1_MUX_XOFF_INC_PRE_W_PASSTHRU;
        }



        for(i=0;i<fmf1_xoff_pre_adj;i++)
        {
            if (ret_val == PMC_SUCCESS)
            {

                PMC_LOG_TRACE("G.HAO LCR resize inc : 29b) MUX (EXP Stage 1-2): Increase XOFF threshold by 1TS equivalent every 2.5s\n ");
                ret_val = coreotn_odtu_mux_xoff_thresh_update(digi_handle->coreotn_handle,
                                                          ghao_mux_stage,
                                                          coreotn_lo_chnl_id,
                                                          TRUE,
                                                          &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_xoff_thresh);
            }
        }

        for(i=0;i<FMF2_MUX_XOFF_INC_PRE_W_PASSTHRU;i++)
        {
            if (ret_val == PMC_SUCCESS)
            {

                PMC_LOG_TRACE("G.HAO LCR resize inc : 29c) MUX (EXP Stage 2-3A): Increase XOFF threshold by 1TS equivalent every 2.5s\n ");
                ret_val = coreotn_odtu_mux_xoff_thresh_update(digi_handle->coreotn_handle,
                                                          COREOTN_MUX_STAGE_TWO,
                                                          coreotn_lo_chnl_id,
                                                          TRUE,
                                                          &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_passthru_xoff_thresh);
            }
        }

        for(i=0;i<MPMO_XOFF_INC_PRE_W_PASSTHRU;i++)
        {
            if (ret_val == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("G.HAO LCR resize inc : 29d) MPMO : Increase XOFF threshold by 1TS equivalent every 2.5s\n ");

                ret_val = coreotn_mpmo_xoff_thresh_change(digi_handle->coreotn_handle,
                                                          COREOTN_MPMO_TO_COREOTN,
                                                          coreotn_lo_chnl_id,
                                                          ghao_increase);
            }
        }
    }
    else
    {

        if(digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start == 1)
        {
            fmf1_xoff_pre_adj = FMF1_MUX_XOFF_INC_PRE_W0_PASSTHRU_FROM1;
        }
        else
        {
            fmf1_xoff_pre_adj = FMF1_MUX_XOFF_INC_PRE_W0_PASSTHRU;
        }



        for(i=0;i<fmf1_xoff_pre_adj;i++)
        {
            if (ret_val == PMC_SUCCESS)
            {

                PMC_LOG_TRACE("G.HAO LCR resize inc : 29b) MUX (EXP Stage 1-2): Increase XOFF threshold by 1TS equivalent every 2.5s\n ");
                ret_val = coreotn_odtu_mux_xoff_thresh_update(digi_handle->coreotn_handle,
                                                          ghao_mux_stage,
                                                          coreotn_lo_chnl_id,
                                                          TRUE,
                                                          &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_xoff_thresh);
            }
        }

    }


    PMC_LOG_TRACE("End G.HAO LCR resize TX INC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_lcr_size_tx_inc */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_size_rx_inc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR resize step for LINE<->ILKN increase segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_rx_inc(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    UINT8 sifd_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    util_global_oduk_port_t sifd_port_type;
    digi_ghao_cpb_datapath_t oduksw_datapath;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = (UINT8) local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    sifd_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr)->port_type;
    
    PMC_LOG_TRACE("Start G.HAO LCR resize RX INC\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    /* Databases for RX datapath */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc : DB) Setup database UTIL_GEN_DB_LINE_CORE_OTN_RX_DB\n");
        ret_val = coreotn_ghao_db_chnl_rec_update(digi_handle->coreotn_handle,
                                                  UTIL_GEN_DB_LINE_CORE_OTN_RX_DB,
                                                  ghao_mux_stage,
                                                  coreotn_ho_chnl_id,
                                                  coreotn_lo_chnl_id,
                                                  local_ghao_segment->coreotn_rx_new_trib_slots,
                                                  local_ghao_segment->coreotn_rx_old_cal_mask,
                                                  local_ghao_segment->coreotn_rx_new_cal_mask,
                                                  &local_ghao_segment->cal_to_add_remove_num,
                                                  local_ghao_segment->coreotn_rx_cal_to_add_remove);
    }

    if(sifd_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1)
    {
        oduksw_datapath = DIGI_GHAO_ODUKSW_COREOTN_SIFD1;
    }
    else
    {
        oduksw_datapath = DIGI_GHAO_ODUKSW_COREOTN_SIFD2;
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc : DB) Setup database UTIL_GEN_DB_OCPB_DCS RX\n");
        ret_val = digi_ghao_cpb_db_chnl_rec_update(digi_handle,
                                                   oduksw_datapath,
                                                   sifd_chnl_id,
                                                   local_ghao_segment->ts_target);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize inc : 3) ODUk Switch (LINE -> SIFD): Configure FIFO for the ending number of TS\n");
        ret_val = coreotn_oduksw_ghao_queue_resize_zone_update(digi_handle->coreotn_handle,
                                                               (UINT32)sifd_port_type,
                                                               sifd_chnl_id,
                                                               local_ghao_segment->ts_target*2,                                                               local_ghao_segment->ts_target,
                                                               ghao_increase);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc : 4) MPMA (stage 3B) : Increase FIFO to final TS size.\n");
        ret_val = coreotn_mpma_ghao_fifo_resize(digi_handle->coreotn_handle,
                                                COREOTN_MPMA_TO_COREOTN,
                                                coreotn_lo_chnl_id,
                                                local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc : 4b) ODUJAT (1-2 and 2-3a) : Increase FIFO to final TS size.\n");
        ret_val = coreotn_odujat_ghao_resize_fifo(digi_handle->coreotn_handle,
                                                  ghao_mux_stage,
                                                  coreotn_lo_chnl_id,
                                                  local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc : 5) ODUkSC (COREOTN) :  Change to new Calendar and scale down credits to reflect old BW.\n");
        ret_val = coreotn_oduksc_scbs3_cgen_reprov(digi_handle->coreotn_handle,
                                                   ghao_mux_stage,
                                                   coreotn_lo_chnl_id,
                                                   local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc : 6) DMX(Stage 1-2): Configure all the standby pages to new configuration including STS_SW, new M value.\n");
        ret_val = coreotn_dmx_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       &coreotn_ho_chnl_id,
                                                       &coreotn_lo_chnl_id,
                                                       ghao_increase,
                                                       local_ghao_segment->old_trib_slot_mask_rx,
                                                       local_ghao_segment->coreotn_rx_trib_slots_add_rem,
                                                       local_ghao_segment->coreotn_rx_tpid);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_inc : 8) MPMA (stage 3B): Increase Calendar\n");
        ret_val = coreotn_mpma_ghao_scbs3_reprov(digi_handle->coreotn_handle,
                                                 COREOTN_MPMA_TO_COREOTN,
                                                 coreotn_lo_chnl_id,
                                                 local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize inc : 9) ODUk Switch (LINE -> SIFD) : Change Calendar to final TS count\n");
        ret_val = digi_ghao_cpb_dcs_calendar_cfg(digi_handle,
                                                 oduksw_datapath,
                                                 sifd_chnl_id);
    }


    PMC_LOG_TRACE("End G.HAO LCR resize RX INC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_lcr_size_rx_inc */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_size_tx_dec_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the LCR step for LINE<->ILKN decrease segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_tx_dec_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    UINT8 sifd_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT32 i;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = (UINT8) local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    PMC_LOG_TRACE("Start G.HAO LCR resize TX DEC RP\n");

    /* Initialize G.HAO in TSB that need to be initialized */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 0) Extract added/removed calendar slots for MUX\n");
        ret_val = coreotn_ghao_db_chnl_get_masks(digi_handle->coreotn_handle,
                                                 UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,
                                                 ghao_mux_stage,
                                                 coreotn_ho_chnl_id,
                                                 coreotn_lo_chnl_id,
                                                 local_ghao_segment->coreotn_tx_new_trib_slots,
                                                 local_ghao_segment->coreotn_tx_old_cal_mask,
                                                 local_ghao_segment->coreotn_tx_new_cal_mask,
                                                 &local_ghao_segment->cal_to_add_remove_num,
                                                 local_ghao_segment->coreotn_tx_cal_to_add_remove);
    }

    if (ret_val == PMC_SUCCESS)
    {
        digi_ghao_gen_slots_add_rem_from_masks(local_ghao_segment->coreotn_tx_old_cal_mask,
                                               local_ghao_segment->coreotn_tx_new_cal_mask,
                                               local_ghao_segment->coreotn_tx_cal_slots_add_rem);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 0a) MUX INIT: ghao_mux_stage=%d, ghao_id=%d, ho_chnl_id=%d, lo_chnl_id=%d, tbid=%d, tribslots=%d\n", ghao_mux_stage, ghao_segment_id, coreotn_ho_chnl_id, coreotn_lo_chnl_id, local_ghao_segment->coreotn_tx_tpid, local_ghao_segment->ts_target);
        ret_val = coreotn_mux_ghao_init(digi_handle->coreotn_handle,
                                        ghao_mux_stage,
                                        ghao_segment_id,
                                        coreotn_ho_chnl_id, 
                                        coreotn_lo_chnl_id,
                                        local_ghao_segment->coreotn_tx_tpid,
                                        local_ghao_segment->coreotn_tx_cal_slots_add_rem);
    }


    if(local_ghao_segment->mux_passthru)
    {
        if (ret_val == PMC_SUCCESS)
        {
            
              PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 0b) MUX INIT: ghao_mux_stage=%d, ghao_id=%d, ho_chnl_id=%d, lo_chnl_id=%d, tbid=%d, tribslots=%d\n", ghao_mux_stage, ghao_segment_id, coreotn_ho_chnl_id, coreotn_lo_chnl_id, local_ghao_segment->coreotn_tx_tpid, local_ghao_segment->ts_target);
              ret_val = coreotn_mux_ghao_init(digi_handle->coreotn_handle,
                                            COREOTN_MUX_STAGE_TWO,
                                            ghao_segment_id,
                                            coreotn_lo_chnl_id, 
                                            coreotn_lo_chnl_id,
                                            local_ghao_segment->coreotn_tx_tpid,
                                            local_ghao_segment->coreotn_tx_cal_slots_add_rem);
        }

    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 1b) MUX (Stage 1-2): [REM, #a, NACK] and RP = 1, TSCC = 0 on all removed Trib-slots.\n");
        ret_val = coreotn_mux_ghao_send_remove_nack(digi_handle->coreotn_handle,
                                                    ghao_mux_stage,
                                                    ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 1e) Set MUX (Stage 1-2) in prev_Cn mode and do a config update.\n");
        ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 1f) MUX: Wait until config update is done\n");
        ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 1g) MUX (Stage 1-2): Synchronize standby page with active page\n");
        ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec_rp : 1h) MUX: Wait until config update is done\n");
        ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }

    /* Initialize userbit to old cal mask for COREOTN and SIFD */
    for(i = 0; i< 3; i++)
    {
        local_ghao_segment->coreotn_mpmo3b_userbits_mask[i] = local_ghao_segment->coreotn_tx_old_cal_mask[i];
    }


    PMC_LOG_TRACE("End G.HAO LCR resize TX DEC RP\n");


    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_lcr_size_tx_dec_rp */


/*******************************************************************************
* digi_ghao_line_ilkn_lcr_size_rx_dec_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Initialize the LCR step for LINE<->ILKN decrease segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_rx_dec_rp(digi_handle_t *digi_handle,
                                                        UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    UINT8 sifd_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = (UINT8) local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    PMC_LOG_TRACE("Start G.HAO LCR resize RX DEC RP\n");

    /* Initialize G.HAO in TSB that need to be initialized */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec_rp : 0) Extract added/removed calendar slots for DMX\n");
        ret_val = coreotn_ghao_db_chnl_get_masks(digi_handle->coreotn_handle,
                                                 UTIL_GEN_DB_LINE_CORE_OTN_RX_DB,
                                                 ghao_mux_stage,
                                                 coreotn_ho_chnl_id,
                                                 coreotn_lo_chnl_id,
                                                 local_ghao_segment->coreotn_rx_new_trib_slots,
                                                 local_ghao_segment->coreotn_rx_old_cal_mask,
                                                 local_ghao_segment->coreotn_rx_new_cal_mask,
                                                 &local_ghao_segment->cal_to_add_remove_num,
                                                 local_ghao_segment->coreotn_rx_cal_to_add_remove);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec_rp : 0) DMX INIT: ghao_mux_stage=%d, ho_chnl_id=%d, lo_chnl_id=%d, tribslots=%d\n", ghao_mux_stage, coreotn_ho_chnl_id, coreotn_lo_chnl_id, local_ghao_segment->ts_target);
        ret_val = coreotn_dmx_ghao_init(digi_handle->coreotn_handle,
                                        ghao_mux_stage,
                                        coreotn_lo_chnl_id,
                                        local_ghao_segment->coreotn_rx_tpid,
                                        local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    /* 1)	Once Control plane is ready. Send: */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec_rp : 1c) DMX (Stage 1-2): DMX (Stage 1-2): Wait to receive RP = 1 and observe [REM, #a, NACK] and TSCC = 0 in all removed TS.\n");
        ret_val = coreotn_dmx_ghao_wait_for_remove_nack(digi_handle->coreotn_handle,
                                                        ghao_mux_stage,
                                                        &coreotn_ho_chnl_id,
                                                        &coreotn_lo_chnl_id,
                                                        &local_ghao_segment->coreotn_rx_tpid,
                                                        local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    PMC_LOG_TRACE("End G.HAO LCR resize RX DEC RP\n");


    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_lcr_size_rx_dec_rp */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_size_tx_dec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR resize step for LINE<->ILKN decrease segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_tx_dec(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    UINT8 sifd_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    BOOL8 mux_passthru;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = (UINT8) local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    mux_passthru = local_ghao_segment->mux_passthru;

    
    PMC_LOG_TRACE("Start G.HAO LCR resize TX DEC\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    /* LCR (FIFO and Calendar resizing) - Calendar against Traffic, FIFO with traffic: */


    if(ret_val == PMC_SUCCESS)
    {
        /* Before updating the database, it's required to perform
        the ghao "deprovision" step on all active entries */
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : Deprov MUX lo channel\n");
        ret_val = coreotn_mux_ghao_lo_chnl_deprov(digi_handle->coreotn_handle,
                                                  ghao_mux_stage,
                                                  coreotn_lo_chnl_id,
                                                  coreotn_ho_chnl_id);
    }

    /* Databases for TX datapath */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : DB) Setup database UTIL_GEN_DB_LINE_CORE_OTN_TX_DB\n");
        ret_val = coreotn_ghao_db_chnl_rec_update(digi_handle->coreotn_handle,
                                                  UTIL_GEN_DB_LINE_CORE_OTN_TX_DB,
                                                  ghao_mux_stage,
                                                  coreotn_ho_chnl_id,
                                                  coreotn_lo_chnl_id,
                                                  local_ghao_segment->coreotn_tx_new_trib_slots,
                                                  local_ghao_segment->coreotn_tx_old_cal_mask,
                                                  local_ghao_segment->coreotn_tx_new_cal_mask,
                                                  &local_ghao_segment->cal_to_add_remove_num,
                                                  local_ghao_segment->coreotn_tx_cal_to_add_remove);
    }

    /*this call replaces digi_ghao_cpb_db_chnl_rec_update() and digi_ghao_cpb_dcs_calendar_cfg()
    for the ODUKSW at queue 2 (towards COREOTN) for the decrease only.  There
    is a special calendar requirement on this resize*/
    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : DB) Setup database UTIL_GEN_DB_OCPB_DCS TX\n");
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : 24) ODUk Switch (MAP -> LINE): Change Calendar to final TS + 2\n");
        ret_val = digi_ghao_cpb_dcs_calendar_cfg_plus_userbits(digi_handle,
                                                               ghao_segment_id);
    }

    /* ILKN -> MUX direction: */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : 35) MUX (Stage 1-2): Configure all the standby pages to new configuration including STS_SW, new M value. Set the TXJC.PREV_CN_SEL to 0 in the standby page\n");
        ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       ghao_segment_id);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : 39) ODUk Switch (SIFD -> LINE): Change FIFO to final size.\n");
        ret_val = coreotn_oduksw_ghao_queue_resize_zone_update(digi_handle->coreotn_handle,
                                                               COREOTN_ODUKSW_OUTPUT_COREOTN,
                                                               coreotn_lo_chnl_id,
                                                               local_ghao_segment->ts_target*2,
                                                               local_ghao_segment->ts_target,
                                                               ghao_increase);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : 40) MPMO(Stage 3B): Change FIFO to final size\n");
        ret_val = coreotn_mpmo_ghao_fifo_resize_no_thresh_update(digi_handle->coreotn_handle,
                                                                 COREOTN_MPMO_TO_COREOTN,
                                                                 coreotn_lo_chnl_id,
                                                                 local_ghao_segment->ts_target+local_ghao_segment->fo1_mpmo_thresh_pre_post_fw);
    }

    if (ghao_mux_stage == COREOTN_MUX_STAGE_ONE && mux_passthru == TRUE)
    {
        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO LCR resize tx_dec : 41) MUX (Stage 2-3A): Change FIFO to final size(final size + 1TS)\n");
            ret_val = coreotn_mux_ghao_resize_fifo(digi_handle->coreotn_handle,
                                                   COREOTN_MUX_STAGE_TWO,
                                                   ghao_segment_id);
        }
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize tx_dec : 42) MUX (Stage 1-2): Change expand FIFO to final size(final size + 1TS)\n");
        ret_val = coreotn_mux_ghao_resize_fifo(digi_handle->coreotn_handle,
                                               ghao_mux_stage,
                                               ghao_segment_id);
    }


    PMC_LOG_TRACE("End G.HAO LCR resize TX DEC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_lcr_size_tx_dec */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_size_rx_dec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR resize step for LINE<->ILKN decrease segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_size_rx_dec(digi_handle_t *digi_handle,
                                                     UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    UINT8 sifd_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    digi_ghao_cpb_datapath_t oduksw_datapath;
    util_global_oduk_port_t sifd_port_type;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = (UINT8) local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    sifd_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr)->port_type;
    
    PMC_LOG_TRACE("Start G.HAO LCR resize RX DEC\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    /* LCR (FIFO and Calendar resizing) - Calendar against Traffic, FIFO with traffic: */


    if (ret_val == PMC_SUCCESS)
    {
        /*It is nessecary to call this deprov API in the demux before
        changing the database */
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec : Deprov DMX lo channel\n");
        ret_val = coreotn_dmx_ghao_lo_chnl_deprov(digi_handle->coreotn_handle,
                                                  ghao_mux_stage,
                                                  coreotn_lo_chnl_id,
                                                  coreotn_ho_chnl_id);
    }


    /* Databases for RX datapath */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec : DB) Setup database UTIL_GEN_DB_LINE_CORE_OTN_RX_DB\n");
        ret_val = coreotn_ghao_db_chnl_rec_update(digi_handle->coreotn_handle,
                                                  UTIL_GEN_DB_LINE_CORE_OTN_RX_DB,
                                                  ghao_mux_stage,
                                                  coreotn_ho_chnl_id,
                                                  coreotn_lo_chnl_id,
                                                  local_ghao_segment->coreotn_rx_new_trib_slots,
                                                  local_ghao_segment->coreotn_rx_old_cal_mask,
                                                  local_ghao_segment->coreotn_rx_new_cal_mask,
                                                  &local_ghao_segment->cal_to_add_remove_num,
                                                  local_ghao_segment->coreotn_rx_cal_to_add_remove);
    }


    if(sifd_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1)
    {
        oduksw_datapath = DIGI_GHAO_ODUKSW_COREOTN_SIFD1;
    }
    else
    {
        oduksw_datapath = DIGI_GHAO_ODUKSW_COREOTN_SIFD2;
    }

    if (ret_val == PMC_SUCCESS)
    {
        /* in the LCR resize, it's required keep the DPI calendar >= to the DCI
        calendar.  It's required to change the ODUKSW calendar after removing
        the userbits */
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec : DB) Setup database UTIL_GEN_DB_OCPB_DCS RX\n");
        ret_val = digi_ghao_cpb_db_chnl_rec_update(digi_handle,
                                                   oduksw_datapath,
                                                   sifd_chnl_id,
                                                   local_ghao_segment->ts_target);
    }

    /* DMX -> SIFD */
    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec : 28) ODUk Switch (LINE -> SIFD) : Change Calendar to final TS\n");
        ret_val = digi_ghao_cpb_dcs_calendar_cfg(digi_handle,
                                                 oduksw_datapath,
                                                 sifd_chnl_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec : 29) MPMA(Stage 3B) : Change Calendar to final TS\n");
        ret_val = coreotn_mpma_ghao_scbs3_reprov(digi_handle->coreotn_handle,
                                                 COREOTN_MPMA_TO_COREOTN,
                                                 coreotn_lo_chnl_id,
                                                 local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec : 30) ODUkSC (COREOTN): Change Calendar to final TS and credits to match new rate.\n");
        ret_val = coreotn_oduksc_scbs3_cgen_reprov(digi_handle->coreotn_handle,
                                                   ghao_mux_stage,
                                                   coreotn_lo_chnl_id,
                                                   local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec : 31) DMX (Stage 1-2): Configure all the standby pages to new configuration including STS_SW, new M value.\n");
        ret_val = coreotn_dmx_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       &coreotn_ho_chnl_id,
                                                       &coreotn_lo_chnl_id,
                                                       ghao_increase,
                                                       local_ghao_segment->old_trib_slot_mask_rx,
                                                       local_ghao_segment->coreotn_rx_trib_slots_add_rem,
                                                       local_ghao_segment->coreotn_rx_tpid);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec : 31b) ODUJAT (1-2 and 2-3a) : Decrease FIFO to final TS size.\n");
        ret_val = coreotn_odujat_ghao_resize_fifo(digi_handle->coreotn_handle,
                                                  ghao_mux_stage,
                                                  coreotn_lo_chnl_id,
                                                  local_ghao_segment->ts_target);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR resize rx_dec : 33) ODUk Switch (LINE -> SIFD): Change FIFO size to final TS\n");
        ret_val = coreotn_oduksw_ghao_queue_resize_zone_update(digi_handle->coreotn_handle,
                                                               (UINT32)sifd_port_type,
                                                               sifd_chnl_id,
                                                               local_ghao_segment->ts_target*2,
                                                               local_ghao_segment->ts_target,
                                                               ghao_increase);
    }

    PMC_LOG_TRACE("End G.HAO LCR resize RX DEC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_lcr_size_rx_dec */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_1_add_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 1 (add ack) for LINE<->ILKN increase segment
*   in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_1_add_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    BOOL8 mux_passthru;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    mux_passthru = local_ghao_segment->mux_passthru;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge TX INC 1 ADD_ACK\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_1 : 22) MUX: Send [ADD, #a, ACK] on all TS\n");
        ret_val = coreotn_mux_ghao_send_add_ack(digi_handle->coreotn_handle,
                                                ghao_mux_stage,
                                                ghao_segment_id);
    }

    PMC_LOG_TRACE("End G.HAO LCR knife edge TX INC 1 ADD_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_1_add_ack */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_1_add_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 1 (add ack) for LINE<->ILKN increase segment
*   in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_1_add_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge RX INC 1 ADD_ACK\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_inc_1 : 22) DMX: Wait to receive [ADD, #a, ACK] on all TS.\n");
        ret_val = coreotn_dmx_ghao_wait_for_add_ack(digi_handle->coreotn_handle,
                                                    ghao_mux_stage,
                                                    &coreotn_ho_chnl_id,
                                                    &coreotn_lo_chnl_id,
                                                    &local_ghao_segment->coreotn_rx_tpid,
                                                    local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge RX INC 1 ADD_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_1_add_ack */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_2_norm_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 2 (norm ack) for LINE<->ILKN increase
*   segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge TX INC 2 NORM_ACK\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_2 : 25) MUX: Once DMX ACK is received send [NORM, #a, ACK] in all TS that needs to be changed. This is done by setting G.HAO update bit in the MUX. The MUX will automatically do Knife Edge in the next RMF after sending NORM command.\n");
        ret_val = coreotn_mux_ghao_send_norm(digi_handle->coreotn_handle,
                                                ghao_mux_stage,
                                                ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_2 : 25b) MUX: Wait until config update is done\n");
        ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge TX INC 2 NORM_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_2_norm_ack */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_2_norm_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 2 (norm ack) for LINE<->ILKN increase
*   segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge RX INC 2 NORM_ACK\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_inc_2 : 25) DMX: After ACK cmd is sent in MUX, DMX can receive NORM as soon as one RMF later. Wait to receive NORM in all TS. DMX HW will automatically do Knife edge in the next RMF after receiving NORM Command.\n");
        ret_val = coreotn_dmx_ghao_wait_for_norm_ack(digi_handle->coreotn_handle,
                                                    ghao_mux_stage,
                                                    &coreotn_ho_chnl_id,
                                                    &coreotn_lo_chnl_id,
                                                    &local_ghao_segment->coreotn_rx_tpid,
                                                    local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge RX INC 2 NORM_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_2_norm_ack */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_3_idle_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 3 (idle nack) for LINE<->ILKN increase
*   segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge TX INC 3 IDLE_NACK\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_3 : 26) MUX (Stage 1-2): Finish LCR by sending [IDLE, 0, NACK] for every added TS at the RMF.\n");
        ret_val = coreotn_mux_ghao_send_idle_nack(digi_handle->coreotn_handle,
                                                  ghao_mux_stage,
                                                  ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_3 : 26b) MUX (Stage 1-2): Synchronize standby page with active page\n");
        ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_inc_3 : 26c) MUX: Wait until config update is done\n");
        ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }


    /* After changing the last tribslot, two bits will be set in
    TC_WR_IPT_VALID.  It's required to remove the "old" valid*/
    if(ret_val == PMC_SUCCESS)
    {
        ret_val = coreotn_mux_ghao_clear_old_last_cal_slot(digi_handle->coreotn_handle, 
                                                           ghao_mux_stage,
                                                           coreotn_lo_chnl_id,
                                                           ghao_increase,
                                                           local_ghao_segment->coreotn_tx_old_trib_slots,
                                                           local_ghao_segment->coreotn_tx_new_trib_slots,
                                                           local_ghao_segment->coreotn_tx_old_cal_mask);

    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge TX INC 3 IDLE_NACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_tx_inc_3_idle_nack */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_3_idle_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 3 (idle nack) for LINE<->ILKN increase
*   segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge RX INC 3 IDLE_NACK\n");

    if (!ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_inc_3 : 26) DMX(Stage 1-2): Wait to receive [IDLE, 0, NACK] on every added TS to exit LCR.\n");
        ret_val = coreotn_dmx_ghao_wait_for_idle_nack(digi_handle->coreotn_handle,
                                                      ghao_mux_stage,
                                                      &coreotn_ho_chnl_id,
                                                      &coreotn_lo_chnl_id,
                                                      &local_ghao_segment->coreotn_rx_tpid,
                                                      local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_inc_3 : 26d) DMX (Stage 1-2) DMX: Wait until config update is done\n");
        ret_val = coreotn_dmx_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_inc_3 : 26e) DMX (Stage 1-2): Synchronize standby page with active page\n");
        ret_val = coreotn_dmx_ghao_update_stdby_config_after_knife_edge(digi_handle->coreotn_handle,
                                                                        ghao_mux_stage,
                                                                        coreotn_ho_chnl_id,
                                                                        coreotn_lo_chnl_id,
                                                                        local_ghao_segment->ts_target,
                                                                        local_ghao_segment->new_trib_slot_mask_rx,
                                                                        local_ghao_segment->coreotn_rx_tpid,
                                                                        local_ghao_segment->ghao_increase,
                                                                        local_ghao_segment->cal_to_add_remove_num,
                                                                        local_ghao_segment->coreotn_rx_cal_to_add_remove);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge RX INC 3 IDLE_NACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_rx_inc_3_idle_nack */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_1_rem_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 1 (rem ack) for LINE<->ILKN decrease segment
*   in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_1_rem_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge TX DEC 1 REM_ACK\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_1 : 43) MUX: Send [REM, #a, ACK] on all TS\n");
        ret_val = coreotn_mux_ghao_send_remove_ack(digi_handle->coreotn_handle,
                                                   ghao_mux_stage,
                                                   ghao_segment_id);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge TX DEC 1 REM_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_1_rem_ack */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_1_rem_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 1 (rem ack) for LINE<->ILKN decrease segment
*   in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_1_rem_ack(digi_handle_t *digi_handle,
                                                                     UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge RX DEC 1 REM_ACK\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_dec_1 : 43) DMX: Wait to receive [REM, #a, ACK] on all TS.\n");
        ret_val = coreotn_dmx_ghao_wait_for_remove_ack(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       &coreotn_ho_chnl_id,
                                                       &coreotn_lo_chnl_id,
                                                       &local_ghao_segment->coreotn_rx_tpid,
                                                       local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge RX DEC 1 REM_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_1_rem_ack */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_2_norm_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 2 (norm ack) for LINE<->ILKN decrease segment
*   in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge TX DEC 2 NORM_ACK\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_2 : 44) MUX: Once DMX ACK is received Send [NORM, #a, ACK] in all TS that needs to be changed. This is done by setting G.HAO update bit in the MUX. The MUX will automatically do Knife Edge in the next RMF after sending NORM command.\n");
        ret_val = coreotn_mux_ghao_send_norm(digi_handle->coreotn_handle,
                                             ghao_mux_stage,
                                             ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_2 : 44b) MUX: Wait until config update is done\n");
        ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge TX DEC 2 NORM_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_2_norm_ack */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_2_norm_ack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 2 (norm ack) for LINE<->ILKN decrease segment
*   in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_2_norm_ack(digi_handle_t *digi_handle,
                                                                      UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge RX DEC 2 NORM_ACK\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_dec_2 : 44) DMX: After ACK cmd is sent in MUX, DMX can receive NORM as soon as one RMF later. Wait to receive NORM in all TS. DMX HW will automatically do Knife edge in the next RMF after receiving NORM Command.\n");
        ret_val = coreotn_dmx_ghao_wait_for_norm_ack(digi_handle->coreotn_handle,
                                                     ghao_mux_stage,
                                                     &coreotn_ho_chnl_id,
                                                     &coreotn_lo_chnl_id,
                                                     &local_ghao_segment->coreotn_rx_tpid,
                                                     local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge RX DEC 2 NORM_ACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_2_norm_ack */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_3_idle_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 3 (idle nack) for LINE<->ILKN decrease
*   segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    BOOL8 mux_passthru;
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT32 cal_add_remove[96];
    UINT32 i;
    util_global_oduk_port_t sifd_port_type;
    digi_ghao_cpb_datapath_t oduksw_datapath;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    mux_passthru = local_ghao_segment->mux_passthru;
    sifd_port_type = ((util_global_oduk_port_data_t*)digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].sifd_ptr)->port_type;
    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge TX DEC 3 IDLE_NACK\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if (ghao_mux_stage == COREOTN_MUX_STAGE_ONE && mux_passthru == TRUE)
    {
        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 45) MUX (Stage 2-3A): Turn valid off on last to-be-removed Calendar (by clear EXP.LO_FIRST_CYCLE).\n");
            ret_val = coreotn_mux_ghao_recfg_exp_lo_calen(digi_handle->coreotn_handle,
                                                          COREOTN_MUX_STAGE_TWO,
                                                          ghao_segment_id,
                                                          local_ghao_segment->coreotn_tx_cal_to_add_remove[local_ghao_segment->cal_to_add_remove_num-1]);
        }
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 46a) MPMO (stage 3B): Turn valid off  on the last to-be-removed calendar\n");
        for(i=0;i< local_ghao_segment->fo1_mpmo_userbit_update_pre_post_fw;i++)
        {
            cal_add_remove[i] = local_ghao_segment->coreotn_tx_cal_to_add_remove[local_ghao_segment->cal_to_add_remove_num-(i+1)];
        }
        ret_val = coreotn_mpmo_update_userbit_mask(digi_handle->coreotn_handle,
                                                   COREOTN_MPMO_TO_COREOTN,
                                                   coreotn_lo_chnl_id,
                                                   cal_add_remove,
                                                   local_ghao_segment->fo1_mpmo_userbit_update_pre_post_fw,
                                                   local_ghao_segment->coreotn_tx_old_cal_mask,
                                                   local_ghao_segment->coreotn_mpmo3b_userbits_mask);

    }


    if(sifd_port_type == UTIL_GLOBAL_ODUK_PORT_ILKN1)
    {
        oduksw_datapath = DIGI_GHAO_ODUKSW_SIFD1_COREOTN;
    }
    else
    {
        oduksw_datapath = DIGI_GHAO_ODUKSW_SIFD2_COREOTN;
    }

    if (ret_val == PMC_SUCCESS)
    {
        /* in the LCR resize, it's required keep the DPI calendar >= to the DCI
        calendar.  It's required to change the ODUKSW calendar after removing
        the userbits */
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : DB) Setup database UTIL_GEN_DB_OCPB_DCS TX\n");
        ret_val = digi_ghao_cpb_db_chnl_rec_update(digi_handle,
                                                   oduksw_datapath,
                                                   coreotn_lo_chnl_id,
                                                   local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 46c) ODUk Switch (SIFD -> LINE): Change Calendar to final TS\n");
        ret_val = digi_ghao_cpb_dcs_calendar_cfg(digi_handle,
                                                 oduksw_datapath,
                                                 coreotn_lo_chnl_id);
    }



    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 46c) MPMO (Stage 3B): Decrease XOFF threshold\n");
        for(i=0;i<local_ghao_segment->fo1_mpmo_thresh_pre_post_fw ;i++)
        {
            ret_val = coreotn_mpmo_xoff_thresh_change(digi_handle->coreotn_handle,
                                                      COREOTN_MPMO_TO_COREOTN,
                                                      coreotn_lo_chnl_id,
                                                      ghao_increase);
        }
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 46d) MPMO (Stage 3B): Change FIFO to final size\n");
        ret_val = coreotn_mpmo_ghao_fifo_resize_no_thresh_update(digi_handle->coreotn_handle,
                                                                 COREOTN_MPMO_TO_COREOTN,
                                                                 coreotn_lo_chnl_id,
                                                                 local_ghao_segment->ts_target);
    }

    /* perform XOFF on 1st FMF1 & FMF2 if needed*/
    if (ret_val == PMC_SUCCESS)
    {
        for(i=0;i<digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_thresh_pre_post_fw ;i++)
        {
            PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 47) MUX (EXP Stage 1-2 and Stage 2-3A): Decrease XOFF threshold by 1TS equivalent\n ");
            ret_val = coreotn_odtu_mux_xoff_thresh_update(digi_handle->coreotn_handle,
                                                          ghao_mux_stage,
                                                          coreotn_lo_chnl_id,
                                                          FALSE,
                                                          &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_xoff_thresh);
        }
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 48) MPMO (stage 3B): Change Calendar to new configuration.\n");
        ret_val = coreotn_mpmo_ghao_scbs3_reprov_force_userbits(digi_handle->coreotn_handle,
                                                                COREOTN_MPMO_TO_COREOTN,
                                                                coreotn_lo_chnl_id,
                                                                local_ghao_segment->ts_target,
                                                                local_ghao_segment->coreotn_tx_new_cal_mask,
                                                                local_ghao_segment->coreotn_tx_new_cal_mask);
    }

 

    if (ghao_mux_stage == COREOTN_MUX_STAGE_ONE && mux_passthru == TRUE)
    {
        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 49) MUX (Stage 2-3A): Reconfigure MUX passthru to clear configuration on all to-be-removed calendar slots.\n");
            ret_val = coreotn_mux_ghao_recfg_passthru(digi_handle->coreotn_handle,
                                                      COREOTN_MUX_STAGE_TWO,
                                                      ghao_segment_id);
        }

        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 50) MUX (Stage 2-3A): Change FIFO to final size\n");
            ret_val = coreotn_mux_ghao_resize_fifo(digi_handle->coreotn_handle,
                                                   COREOTN_MUX_STAGE_TWO,
                                                   ghao_segment_id);
        }

        for(i=0;i<digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru_thresh_pre_post_fw ;i++)
        {
            PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 53a) MUX (EXP Stage 2-3A): Decrease XOFF threshold by 1TS equivalent every 2.5s\n ");
            ret_val = coreotn_odtu_mux_xoff_thresh_update(digi_handle->coreotn_handle,
                                                          COREOTN_MUX_STAGE_TWO,
                                                          coreotn_lo_chnl_id,
                                                          FALSE,
                                                          &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].coreotn_mux_passthru_xoff_thresh);
        }

        for(i=0;i<digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fmf2_mux_cal_en_pre_post_fw;i++)
        {
            if (ret_val == PMC_SUCCESS)
            {
               PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 53b) MUX (EXP Stage 2-3A): removed last calendat valid\n ");

                ret_val = coreotn_mux_ghao_passthru_cal_enable_diable(digi_handle->coreotn_handle,
                                                                      local_ghao_segment->coreotn_tx_cal_to_add_remove[local_ghao_segment->cal_to_add_remove_num-(i+1)],
                                                                      FALSE);
            }
        }       
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 51) MUX (Stage 1-2): Change expand FIFO to final size\n");
        ret_val = coreotn_mux_ghao_resize_fifo(digi_handle->coreotn_handle,
                                               ghao_mux_stage,
                                               ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 52) MUX(Stage 1-2): Finish LCR by sending [IDLE, 0, NACK] for every added TS at the RMF.\n");
        ret_val = coreotn_mux_ghao_send_idle_nack(digi_handle->coreotn_handle,
                                                  ghao_mux_stage,
                                                  ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 52b) MUX (Stage 1-2): Synchronize standby page with active page\n");
        ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                       ghao_mux_stage,
                                                       ghao_segment_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_3 : 52c) MUX: Wait until config update is done\n");
        ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge TX DEC 3 IDLE_NACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_3_idle_nack */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_3_idle_nack
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 3 (idle nack) for LINE<->ILKN decrease
*   segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_3_idle_nack(digi_handle_t *digi_handle,
                                                                       UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    UINT8 sifd_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = (UINT8) local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge RX DEC 3 IDLE_NACK\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }



    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_dec_3 : 46a) MPMA(Stage 3B): Change  FIFO size to final TS\n");
        ret_val = coreotn_mpma_ghao_fifo_resize(digi_handle->coreotn_handle,
                                                COREOTN_MPMA_TO_COREOTN,
                                                coreotn_lo_chnl_id,
                                                local_ghao_segment->ts_target);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_dec_3 : 52) DMX(Stage 1-2): Wait to receive [IDLE, 0, NACK] on every added TS to exit LCR.\n");
        ret_val = coreotn_dmx_ghao_wait_for_idle_nack(digi_handle->coreotn_handle,
                                                      ghao_mux_stage,
                                                      &coreotn_ho_chnl_id,
                                                      &coreotn_lo_chnl_id,
                                                      &local_ghao_segment->coreotn_rx_tpid,
                                                      local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_dec_3 : 52d) DMX (Stage 1-2) DMX: Wait until config update is done\n");
        ret_val = coreotn_dmx_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           coreotn_ho_chnl_id);
    }

    if (ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_dec_3 : 52e) DMX (Stage 1-2): Synchronize standby page with active page\n");
        ret_val = coreotn_dmx_ghao_update_stdby_config_after_knife_edge(digi_handle->coreotn_handle,
                                                                        ghao_mux_stage,
                                                                        coreotn_ho_chnl_id,
                                                                        coreotn_lo_chnl_id,
                                                                        local_ghao_segment->ts_target,
                                                                        local_ghao_segment->new_trib_slot_mask_rx,
                                                                        local_ghao_segment->coreotn_rx_tpid,
                                                                        local_ghao_segment->ghao_increase,
                                                                        local_ghao_segment->cal_to_add_remove_num,
                                                                        local_ghao_segment->coreotn_rx_cal_to_add_remove);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge RX DEC 3 IDLE_NACK\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_3_idle_nack */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_4_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 4 (rp) for LINE<->ILKN decrease segment
*   in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_4_rp(digi_handle_t *digi_handle,
                                                                UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT32 dummy_userbit_mask[3] = {0,0,0};
    UINT32 dummy_set_mask[3] = {0,0,0};

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge TX DEC 4 RP\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_4 : 53) After receiving IDLE NACK in the DMX, then the MUX  can set RP = 0.\n");
        ret_val = coreotn_mux_ghao_send_rp(digi_handle->coreotn_handle,
                                           ghao_mux_stage,
                                           ghao_segment_id,
                                           0);
    }


    /* Perform clean-up */
    /* take MPMOs out of custom userbit mode */
    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge tx_dec_4 : 53b) Take MPMO (Stage 3B) out of custom userbit mode\n");
        ret_val = coreotn_mpmo_set_custom_userbit_mask(digi_handle->coreotn_handle,
                                                       coreotn_lo_chnl_id,
                                                       COREOTN_MPMO_TO_COREOTN,
                                                       FALSE,
                                                       dummy_userbit_mask,
                                                       dummy_set_mask,
                                                       FALSE);
    }

    
    /* After changing the last tribslot, two bits will be set in
    TC_WR_IPT_VALID.  It's required to remove the "old" valid*/
    if(ret_val == PMC_SUCCESS)
    {
        ret_val = coreotn_mux_ghao_clear_old_last_cal_slot(digi_handle->coreotn_handle, 
                                                           ghao_mux_stage,
                                                           coreotn_lo_chnl_id,
                                                           ghao_increase,
                                                           local_ghao_segment->coreotn_tx_old_trib_slots,
                                                           local_ghao_segment->coreotn_tx_new_trib_slots,
                                                           local_ghao_segment->coreotn_tx_old_cal_mask);
    }


    PMC_LOG_TRACE("End G.HAO LCR knife edge TX DEC 4 RP\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_tx_dec_4_rp */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_4_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge step 4 (rp) for LINE<->ILKN decrease segment
*   in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_4_rp(digi_handle_t *digi_handle,
                                                                UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    BOOL8 ghao_increase;
    coreotn_mux_stage_t ghao_mux_stage;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge RX DEC 4 RP\n");

    if (ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO LCR knife edge rx_dec_4 : 53) DMX, wait to receive RP = 0 and terminate G.HAO protocol.\n");
        ret_val = coreotn_dmx_ghao_wait_for_rp(digi_handle->coreotn_handle,
                                               ghao_mux_stage,
                                               &coreotn_ho_chnl_id,
                                               &coreotn_lo_chnl_id,
                                               &local_ghao_segment->coreotn_rx_tpid,
                                               local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }
    

    PMC_LOG_TRACE("End G.HAO LCR knife edge RX DEC 4 RP\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_rx_dec_4_rp */



/*******************************************************************************
* digi_ghao_line_ilkn_lcr_knife_edge_ilkn
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the LCR knife edge ILKN step for LINE<->ILKN increase segment in
*   TX & RX
*  
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_lcr_knife_edge_ilkn(digi_handle_t *digi_handle,
                                                         UINT32 ghao_segment_id)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT8 sifd_chnl_id;
    BOOL8 ghao_increase;
    digi_ghao_seg_ctrl_t *local_ghao_segment;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ghao_increase = local_ghao_segment->ghao_increase;
    sifd_chnl_id = (UINT8) local_ghao_segment->sifd_chnl_id;

    
    PMC_LOG_TRACE("Start G.HAO LCR knife edge ILKN\n");



    PMC_LOG_TRACE("End G.HAO LCR knife edge ILKN\n");

    PMC_RETURN(ret_val);

} /*  digi_ghao_line_ilkn_lcr_knife_edge_ilkn */



/*******************************************************************************
* digi_ghao_line_ilkn_bwr_setup_tx_1_tscc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the BWR setup step 1 (tscc) for LINE<->ILKN decrease segment in TX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_setup_tx_1_tscc(digi_handle_t *digi_handle,
                                                         UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    UINT8 sifd_chnl_id;
    UINT32 ts_target;
    UINT32 ts_start;
    coreotn_mux_stage_t ghao_mux_stage;
    BOOL8 ghao_increase;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = (UINT8) local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    ts_target = local_ghao_segment->ts_target;
    ts_start = local_ghao_segment->ts_start;
    ghao_increase = local_ghao_segment->ghao_increase;


    PMC_LOG_TRACE("Start G.HAO BWR setup TX 1 TSCC\n");


    /* perform bwr_setup operation */

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR set-up tx_1 : 33/7) MPMO (stage 3B): set target rate and set into GMP special mode. (get ready for ramp)\n");
        ret_val = coreotn_ghao_mpmo_bwr_rate_param_set(digi_handle->coreotn_handle,
                                                       COREOTN_MPMO_TO_COREOTN,
                                                       coreotn_lo_chnl_id,
                                                       ts_target,
                                                       ghao_increase);
    }

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR set-up tx_1 : 35/9) MUX (Stage 1-2): Set TSCC bit on all TS to be sent\n");
        ret_val = coreotn_mux_ghao_send_tscc(digi_handle->coreotn_handle,
                                             ghao_mux_stage,
                                             ghao_segment_id,
                                             1);
    }


    PMC_LOG_TRACE("End G.HAO BWR setup TX 1 TSCC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_bwr_setup_tx_1_tscc */



/*******************************************************************************
* digi_ghao_line_ilkn_bwr_setup_rx_1_tscc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the BWR setup step 1 (tscc) for LINE<->ILKN decrease segment in RX
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_setup_rx_1_tscc(digi_handle_t *digi_handle,
                                                         UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;
    
    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT8 coreotn_ho_chnl_id;
    UINT8 coreotn_lo_chnl_id;
    UINT8 sifd_chnl_id;
    UINT32 ts_target;
    coreotn_mux_stage_t ghao_mux_stage;
    BOOL8 ghao_increase;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = (UINT8) local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;
    ts_target = local_ghao_segment->ts_target;
    ghao_increase = local_ghao_segment->ghao_increase;


    PMC_LOG_TRACE("Start G.HAO BWR setup RX 1 TSCC\n");

    /* perform bwr_setup operation */

    if(ret_val == PMC_SUCCESS)
    {
        /* set up ODUKSC in COREOTN to begin ramp */
        PMC_LOG_TRACE("G.HAO BWR set-up rx_1 : 27/2) ODUk SC (COREOTN): Set up ODUkSc to begin RAMP. But don't begin ramp \n");
        ret_val = coreotn_oduksc_ramp_prov(digi_handle->coreotn_handle,
                                           ghao_mux_stage,
                                           coreotn_lo_chnl_id,
                                           ts_target);
    }

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR start : 36b/11b) ODUkSc (COREOTN): Begins ramp after RFRM(3A) BWR_IND received.\n");
        ret_val = coreotn_oduksc_ramp_start(digi_handle->coreotn_handle,
                                            local_ghao_segment->ghao_mux_stage,
                                            coreotn_lo_chnl_id);
    }


    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR set-up rx_1 : 28/3) DMX(Stage 1-2): Wait to receive TSCC bit on all TS  \n");
        ret_val = coreotn_dmx_ghao_wait_for_tscc(digi_handle->coreotn_handle,
                                                 ghao_mux_stage,
                                                 &coreotn_ho_chnl_id,
                                                 &coreotn_lo_chnl_id,
                                                 &local_ghao_segment->coreotn_rx_tpid,
                                                 local_ghao_segment->coreotn_rx_trib_slots_add_rem,
                                                 1);
    }

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR set-up rx_1 : 30/6) MPMA (Stage 3B) : Disable MPMA DSP\n");
        ret_val = coreotn_ghao_mpma_dsp_enable_set(digi_handle->coreotn_handle,
                                                   COREOTN_MPMA_TO_COREOTN,
                                                   coreotn_lo_chnl_id,
                                                   FALSE,
                                                   0);
    }


    PMC_LOG_TRACE("End G.HAO BWR setup RX 1 TSCC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_bwr_setup_rx_1_tscc */



/*******************************************************************************
* digi_ghao_line_ilkn_bwr_ramp_start_tx
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Starts the ramp in TX. Triggers the firmware routine that will perform the
*   per TS ramp in the datapath.  It is required that this API is called
*   on all intermediate nodes before it is called for source nodes.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_ramp_start_tx(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    digi_ghao_seg_ctrl_t *local_ghao_segment;
    BOOL mux_passthru;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    mux_passthru = local_ghao_segment->mux_passthru;

    /* perform bwr start operation */

    PMC_LOG_TRACE("Start G.HAO BWR start TX\n");


        /* Update pre-firmware operations done */
    if (local_ghao_segment->ghao_increase)
    {

        if(mux_passthru == FALSE)
        {
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_thresh_pre_post_fw = MPMO_XOFF_INC_PRE_WO_PASSTHRU;
              /*on an increase, one xoff threshold adjustment is performed before the firmware
              to avoid underflows in the EXP CFC*/
              if(digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start == 1)
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_thresh_pre_post_fw = FMF1_MUX_XOFF_INC_PRE_W0_PASSTHRU_FROM1;
              }
              else
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_thresh_pre_post_fw = FMF1_MUX_XOFF_INC_PRE_W0_PASSTHRU;
              }
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_userbit_update_pre_post_fw = MPMO_USERBIT_INC_PRE_WO_PASSTHRU;
        }
        else
        {
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_thresh_pre_post_fw = MPMO_XOFF_INC_PRE_W_PASSTHRU;
              
              if(digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start == 1)
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_thresh_pre_post_fw = FMF1_MUX_XOFF_INC_PRE_W_PASSTHRU_FROM1;
              }
              else
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_thresh_pre_post_fw = FMF1_MUX_XOFF_INC_PRE_W_PASSTHRU;
              }

              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_userbit_update_pre_post_fw = MPMO_USERBIT_INC_PRE_WO_PASSTHRU;

              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru_thresh_pre_post_fw = FMF2_MUX_XOFF_INC_PRE_W_PASSTHRU;
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fmf2_mux_cal_en_pre_post_fw = FMF2_MUX_CAL_EN_INC_PRE_W_PASSTHRU;
        }
    }
    else
    { 
        if(mux_passthru == FALSE)
        {

              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_thresh_pre_post_fw = MPMO_USERBIT_DEC_PRE_WO_PASSTHRU;
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_thresh_pre_post_fw = FMF1_MUX_XOFF_DEC_PRE_W0_PASSTHRU;

              /*decreasing by 1 is a special case for userbit adjustments*/
              if( ((digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start 
                   - digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_target) == 1) &&
                   MPMO_USERBIT_DEC_PRE_WO_PASSTHRU > 1)
              {

                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_userbit_update_pre_post_fw = 1;
              }
              else
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_userbit_update_pre_post_fw = MPMO_USERBIT_DEC_PRE_WO_PASSTHRU;
              }


        }
        else
        {

              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_thresh_pre_post_fw = MPMO_XOFF_DEC_POST_W_PASSTHRU;
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_thresh_pre_post_fw = FMF1_MUX_XOFF_DEC_POST_W_PASSTHRU;
              digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].mux_passthru_thresh_pre_post_fw = FMF2_MUX_XOFF_DEC_POST_W_PASSTHRU;

              /*decreasing by 1 is a special case for userbit adjustments*/
              if( ((digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start 
                   - digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_target) == 1) &&
                   MPMO_USERBIT_DEC_POST_W_PASSTHRU > 1)
              {

                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_userbit_update_pre_post_fw = 1;
              }
              else
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fo1_mpmo_userbit_update_pre_post_fw = MPMO_USERBIT_DEC_POST_W_PASSTHRU;
              }


              /*decreasing by 1 is a special case for fmf2 cal adjustments*/
              if( ((digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_start 
                   - digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].ts_target) == 1) &&
                   FMF2_MUX_CAL_EN_DEC_POST_W_PASSTHRU > 1)
              {

                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fmf2_mux_cal_en_pre_post_fw = 1;
              }
              else
              {
                  digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id].fmf2_mux_cal_en_pre_post_fw = FMF2_MUX_CAL_EN_DEC_POST_W_PASSTHRU;
              }
        }

    }


    PMC_LOG_TRACE("End G.HAO BWR start TX\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_bwr_ramp_start_tx */



/*******************************************************************************
* digi_ghao_line_ilkn_bwr_ramp_start_rx
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Starts the ramp in RX. Triggers the firmware routine that will perform the
*   per TS ramp in the datapath.  It is required that this API is called
*   on all intermediate nodes before it is called for source nodes.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
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
PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_ramp_start_rx(digi_handle_t *digi_handle,
                                                       UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT8 coreotn_lo_chnl_id;


    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;

    /* perform bwr start operation */

    PMC_LOG_TRACE("Start G.HAO BWR start RX\n");




    PMC_LOG_TRACE("Start G.HAO BWR start RX\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_bwr_ramp_start_rx */



/*******************************************************************************
* digi_ghao_line_ilkn_bwr_ramp_end_tx_1_tscc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the BWR end step 1 (tscc) for LINE<->ILKN decrease segment in TX
*  This need to be called when the ramp is complete to put to datapath
*  back into normal operating more.
*
*  For intermediate nodes, call after source nodes have
*  completed.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_ramp_end_tx_1_tscc(digi_handle_t *digi_handle,
                                                            UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    digi_ghao_seg_ctrl_t *local_ghao_segment;
    DOUBLE odu_bitrate;
    UINT32 ts_target;
    UINT8 coreotn_lo_chnl_id;
    UINT8 coreotn_ho_chnl_id;
    UINT8 sifd_chnl_id;
    UINT32 oif_pkt_size;
    UINT32 oif_header_size;
    coreotn_mux_stage_t ghao_mux_stage;

    UINT32 pkt_size_ptr;
    UINT32 pkt_per_int_ptr;
    UINT32 pkt_per_n_ptr;
    UINT32 pkt_per_d_ptr;
    DOUBLE eps_ptr;


    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ts_target = local_ghao_segment->ts_target;
    
    oif_pkt_size = local_ghao_segment->oif_pkt_size;
    oif_header_size = local_ghao_segment->header_size;

    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;


    /* perform bwr end operation */

    PMC_LOG_TRACE("Start G.HAO BWR end TX 1 TSCC\n");

    if (ts_target <= 8)
    {
        odu_bitrate = (ts_target * UTIL_GLOBAL_ODU2_TS_RATE);
    } 
    else if (ts_target <= 32)
    {
        odu_bitrate = (ts_target * UTIL_GLOBAL_ODU3_TS_RATE);
    } 
    else 
    {
        odu_bitrate = (ts_target * UTIL_GLOBAL_ODU4_TS_RATE);
    }
    

    /* calculate new Cn for COREOTN MPMO based on new client rate and new pkt size */
    if(ret_val == PMC_SUCCESS)
    {

          /*use OIF calculator for LINE-CORE-ILKN paths (for packets terminating at CORE MPMO */
          ret_val = coreotn_oif_pkt_period_calc(digi_handle->coreotn_handle,
                                                odu_bitrate,
                                                oif_pkt_size,
                                                oif_header_size,
                                                &pkt_size_ptr,
                                                &pkt_per_int_ptr,
                                                &pkt_per_n_ptr,
                                                &pkt_per_d_ptr,
                                                &eps_ptr);

    }

    PMC_LOG_TRACE("G.HAO BWR end rx_1 : 41/16) Wait 3ms\n");

    /* wait 20 ms */
    PMC_Q_USLEEP(digi_handle,20000);


    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end tx_1 : 46/21) MPMO (Stage 3B): Set new Packet size, Cn_base(DSP) and take it out of GMP special mode.\n");
        ret_val = coreotn_ghao_mpmo_bwr_end_param_set(digi_handle->coreotn_handle,
                                                      COREOTN_MPMO_TO_COREOTN,
                                                      coreotn_lo_chnl_id,
                                                      odu_bitrate,
                                                      pkt_size_ptr,
                                                      pkt_per_int_ptr,
                                                      pkt_per_n_ptr,
                                                      pkt_per_d_ptr,
                                                      eps_ptr);
    }

    if (local_ghao_segment->ghao_increase)
    {
        if(ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO BWR end tx_1 : 47) MUX (Stage 1-2): Do a config_update to disable PREV_CN_SEL\n");
            ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           ghao_segment_id);        
        }

        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO BWR end tx_1 : 47b) MUX: Wait until config update is done\n");
            ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                               ghao_mux_stage,
                                                               coreotn_ho_chnl_id);
        }

        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO BWR end tx_1 : 47c) MUX (Stage 1-2): Synchronize standby page with active page\n");
            ret_val = coreotn_mux_ghao_update_stdby_config(digi_handle->coreotn_handle,
                                                           ghao_mux_stage,
                                                           ghao_segment_id);
        }

        if (ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO BWR end tx_1 : 47d) MUX: Wait until config update is done\n");
            ret_val = coreotn_mux_ghao_wait_config_update_done(digi_handle->coreotn_handle,
                                                               ghao_mux_stage,
                                                               coreotn_ho_chnl_id);
        }
    }

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end tx_1 : 48/23a) MUX (Stage 1-2): Unset TSCC.\n");
        ret_val = coreotn_mux_ghao_send_tscc(digi_handle->coreotn_handle,
                                             ghao_mux_stage,
                                             ghao_segment_id,
                                             0);
    }


    PMC_LOG_TRACE("End G.HAO BWR end TX 1 TSCC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_bwr_ramp_end_tx_1_tscc */



/*******************************************************************************
* digi_ghao_line_ilkn_bwr_ramp_end_rx_1_tscc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the BWR end step 1 (tscc) for LINE<->ILKN decrease segment in RX
*  This need to be called when the ramp is complete to put to datapath
*  back into normal operating more.
*
*  For intermediate nodes, call after source nodes have
*  completed.
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_ramp_end_rx_1_tscc(digi_handle_t *digi_handle,
                                                            UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    digi_ghao_seg_ctrl_t *local_ghao_segment;
    DOUBLE odu_bitrate;
    UINT32 ts_target;
    UINT8 coreotn_lo_chnl_id;
    UINT8 coreotn_ho_chnl_id;
    UINT8 sifd_chnl_id;
    UINT32 oif_pkt_size;
    UINT32 oif_header_size;
    coreotn_mux_stage_t ghao_mux_stage;

    UINT32 pkt_size_ptr;
    UINT32 pkt_per_int_ptr;
    UINT32 pkt_per_n_ptr;
    UINT32 pkt_per_d_ptr;
    DOUBLE eps_ptr;


    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ts_target = local_ghao_segment->ts_target;
    
    oif_pkt_size = local_ghao_segment->oif_pkt_size;
    oif_header_size = local_ghao_segment->header_size;

    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;


    /* perform bwr end operation */

    PMC_LOG_TRACE("Start G.HAO BWR end RX 1 TSCC\n");

    if (ts_target <= 8)
    {
        odu_bitrate = (ts_target * UTIL_GLOBAL_ODU2_TS_RATE);
    } 
    else if (ts_target <= 32)
    {
        odu_bitrate = (ts_target * UTIL_GLOBAL_ODU3_TS_RATE);
    } 
    else 
    {
        odu_bitrate = (ts_target * UTIL_GLOBAL_ODU4_TS_RATE);
    }
    

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end rx_1 : 39/14) DMX (Stage 1-2): Wait till TSCC is unset.\n");
        ret_val = coreotn_dmx_ghao_wait_for_tscc(digi_handle->coreotn_handle,
                                                 ghao_mux_stage,
                                                 &coreotn_ho_chnl_id,
                                                 &coreotn_lo_chnl_id,
                                                 &local_ghao_segment->coreotn_rx_tpid,
                                                 local_ghao_segment->coreotn_rx_trib_slots_add_rem,
                                                 0);
    }

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end rx_1 : 40/15a) MPMA (Stage 3B): Set new Packet size, period and MPMA_DSP gain.\n");
        ret_val = coreotn_ghao_mpma_bwr_end_param_set(digi_handle->coreotn_handle,
                                                      COREOTN_MPMA_TO_COREOTN,
                                                      coreotn_lo_chnl_id,
                                                      odu_bitrate,
                                                      oif_pkt_size,
                                                      oif_header_size,
                                                      FALSE,
                                                      &pkt_size_ptr,
                                                      &pkt_per_int_ptr,
                                                      &pkt_per_n_ptr,
                                                      &pkt_per_d_ptr,
                                                      &eps_ptr);
    }

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end rx_1 : 40b/15b) MPMA (Stage 3B): Turn on DSP.\n");
        ret_val = coreotn_ghao_mpma_dsp_enable_set(digi_handle->coreotn_handle,
                                                   COREOTN_MPMA_TO_COREOTN,
                                                   coreotn_lo_chnl_id,
                                                   TRUE,
                                                   ts_target);
    }


    PMC_LOG_TRACE("End G.HAO BWR end RX 1 TSCC\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_bwr_ramp_end_rx_1_tscc */



/*******************************************************************************
* digi_ghao_line_ilkn_bwr_ramp_end_tx_3_inc_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the BWR end step 3 (rp) for LINE<->ILKN decrease segment in TX
*  This API will deprovision the G.HAO segment and free up the ghao_segment_id
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_ramp_end_tx_3_inc_rp(digi_handle_t *digi_handle,
                                                              UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT32 ts_target;
    UINT8 coreotn_lo_chnl_id;
    UINT8 coreotn_ho_chnl_id;
    UINT8 sifd_chnl_id;
    UINT32 oif_pkt_size;
    UINT32 oif_header_size;
    coreotn_mux_stage_t ghao_mux_stage;
    UINT32 dummy_userbit_mask[3] = {0,0,0};
    UINT32 dummy_set_mask[3] = {0,0,0};

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ts_target = local_ghao_segment->ts_target;
    
    oif_pkt_size = local_ghao_segment->oif_pkt_size;
    oif_header_size = local_ghao_segment->header_size;

    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = (UINT8) local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;


    /* perform bwr end operation */

    PMC_LOG_TRACE("Start G.HAO BWR end TX 3 RP\n");

    if (!local_ghao_segment->ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end tx_3 : 50) Remove RP bit out of MUX\n");
        ret_val = coreotn_mux_ghao_send_rp(digi_handle->coreotn_handle,
                                           ghao_mux_stage,
                                           ghao_segment_id,
                                           0);
    }

    /* Perform clean-up */
    /* take MPMOs out of custom userbit mode */

    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end tx_3 : 50b) Take MPMO (Stage 3B) out of custer userbit mode\n");
        ret_val = coreotn_mpmo_set_custom_userbit_mask(digi_handle->coreotn_handle,
                                                       coreotn_lo_chnl_id,
                                                       COREOTN_MPMO_TO_COREOTN,
                                                       FALSE,
                                                       dummy_userbit_mask,
                                                       dummy_set_mask,
                                                       FALSE);
    }


    PMC_LOG_TRACE("End G.HAO BWR end TX 3 RP\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_bwr_ramp_end_tx_3_inc_rp */



/*******************************************************************************
* digi_ghao_line_ilkn_bwr_ramp_end_rx_3_inc_rp
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Perform the BWR end step 3 (rp) for LINE<->ILKN decrease segment in RX
*  This API will deprovision the G.HAO
*  segment and free up the ghao_segment_id
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   ghao_segment_id        - Segement id for the G.HAO  
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*   None
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR digi_ghao_line_ilkn_bwr_ramp_end_rx_3_inc_rp(digi_handle_t *digi_handle,
                                                              UINT32 ghao_segment_id)
{
    
    PMC_ERROR ret_val = PMC_SUCCESS;

    digi_ghao_seg_ctrl_t *local_ghao_segment;
    UINT32 ts_target;
    UINT8 coreotn_lo_chnl_id;
    UINT8 coreotn_ho_chnl_id;
    UINT8 sifd_chnl_id;
    UINT32 oif_pkt_size;
    UINT32 oif_header_size;
    coreotn_mux_stage_t ghao_mux_stage;

    PMC_ENTRY();

    /* set local context */
    local_ghao_segment = &digi_handle->var.ghao_cfg.ghao_segments[ghao_segment_id];
    ts_target = local_ghao_segment->ts_target;
    
    oif_pkt_size = local_ghao_segment->oif_pkt_size;
    oif_header_size = local_ghao_segment->header_size;

    coreotn_ho_chnl_id = (UINT8) local_ghao_segment->coreotn_ho_chnl_id;
    coreotn_lo_chnl_id = (UINT8) local_ghao_segment->coreotn_lo_chnl_id;
    sifd_chnl_id = (UINT8) local_ghao_segment->sifd_chnl_id;
    ghao_mux_stage = local_ghao_segment->ghao_mux_stage;


    /* perform bwr end operation */

    PMC_LOG_TRACE("Start G.HAO BWR end RX 3 RP\n");

    if (!local_ghao_segment->ghao_increase)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if(ret_val == PMC_SUCCESS)
    {
        PMC_LOG_TRACE("G.HAO BWR end rx_3 : 50) Wait in DMX for RP bit to be unset. G. HAO is finished. \n");
        ret_val = coreotn_dmx_ghao_wait_for_rp(digi_handle->coreotn_handle,
                                               ghao_mux_stage,
                                               &coreotn_ho_chnl_id,
                                               &coreotn_lo_chnl_id,
                                               &local_ghao_segment->coreotn_rx_tpid,
                                               local_ghao_segment->coreotn_rx_trib_slots_add_rem);
    }


    PMC_LOG_TRACE("End G.HAO BWR end RX 3 RP\n");

    PMC_RETURN(ret_val);

} /* digi_ghao_line_ilkn_bwr_ramp_end_rx_3_inc_rp */



/*
** End of file
*/

