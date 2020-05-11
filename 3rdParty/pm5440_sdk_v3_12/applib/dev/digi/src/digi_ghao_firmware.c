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
* digi_ghao_do_fw_tick
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will be called once every FIRMWARE_TICK_US.  It is
*   intended to perform all of the G.HAO operation that will need to
*   be written into firmware.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
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
PUBLIC PMC_ERROR digi_ghao_do_fw_tick(digi_handle_t *digi_handle)
{

    UINT32 seg_idx;

    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT32 fw_tick_us;
    
    PMC_ENTRY();


    fw_tick_us = digi_handle->var.ghao_cfg.fw_tick_us;

    for(seg_idx=0;seg_idx<digi_handle->var.ghao_cfg.num_fw_segments;seg_idx++)
    {
        PMC_LOG_TRACE("FIRMWARE TIME: %.3f s, slot_num: %d, digi_handle=%p\n",
                  ((DOUBLE)digi_handle->var.ghao_cfg.ghao_fw_segments[seg_idx].total_fw_ticks * (DOUBLE)fw_tick_us / 1000000),
                  (UINT32)(((DOUBLE)digi_handle->var.ghao_cfg.ghao_fw_segments[seg_idx].total_fw_ticks * (DOUBLE)fw_tick_us / 1000000) / 2.5),
                  digi_handle);

        if(digi_handle->var.ghao_cfg.ghao_fw_segments[seg_idx].seg_type == DIGI_GHAO_CORE_SEG)
        {
            if(digi_handle->var.ghao_cfg.ghao_fw_segments[seg_idx].increase == TRUE)
            {
                if(ret_val == PMC_SUCCESS)
                {
                    ret_val = digi_ghao_do_fw_tick_core_inc(digi_handle, seg_idx);
                }
            }
            else
            {
                if(ret_val == PMC_SUCCESS)
                {
                    ret_val = digi_ghao_do_fw_tick_core_dec(digi_handle, seg_idx);
                }
            }
        }
        else if(digi_handle->var.ghao_cfg.ghao_fw_segments[seg_idx].seg_type == DIGI_GHAO_MAP_SEG)
        {
            if(digi_handle->var.ghao_cfg.ghao_fw_segments[seg_idx].increase == TRUE)
            {
                if(ret_val == PMC_SUCCESS)
                {
                    ret_val = digi_ghao_do_fw_tick_map_inc(digi_handle, seg_idx);
                }
            }
            else
            {
                if(ret_val == PMC_SUCCESS)
                {
                    ret_val = digi_ghao_do_fw_tick_map_dec(digi_handle, seg_idx);
                }
            }
        }
        else
        {
            PMC_ASSERT(FALSE, DIGI_ERR_NO_SW_CONTENT, 0, 0);

        }

    }


    PMC_RETURN(ret_val);
}


/*******************************************************************************
* digi_ghao_do_fw_tick_core_inc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will be called once every FIRMWARE_TICK_US.  It is
*   intended to perform all of the G.HAO operation that will need to
*   be written into firmware for COREOTN segment increase.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   seg_idx                - Segment index
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
PUBLIC PMC_ERROR digi_ghao_do_fw_tick_core_inc(digi_handle_t *digi_handle,
                                               UINT32 seg_idx)
{

    BOOL8 bwr_ind_coreotn_polled = FALSE;
    UINT32 fo1_bwr_ind[3];

    UINT32 chnl_id;

    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT32 fw_tick_us;
    UINT32 word_tick;
    UINT32 sub_word_tick;
    UINT32 ts_tick;
    UINT32 cn_tick;
    UINT32 current_num_slots;
    UINT32 ramp_done = FALSE;
    digi_ghao_firmware_ctrl_t *firmware_segment;
    
    PMC_ENTRY();

    firmware_segment = &digi_handle->var.ghao_cfg.ghao_fw_segments[seg_idx];
    chnl_id = firmware_segment->chnl_id;
    fw_tick_us = digi_handle->var.ghao_cfg.fw_tick_us;

    if (firmware_segment->seg_type != DIGI_GHAO_CORE_SEG || firmware_segment->increase != TRUE)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if(firmware_segment->state == DIGI_GHAO_FW_START)
    {
        /* Detect the start of ramp */
        PMC_LOG_TRACE("FW SEG : %u CORE INC in START state\n", seg_idx);

        /*this should only be done once per tick*/
        if(bwr_ind_coreotn_polled == FALSE)
        {
            if(ret_val == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("G.HAO BWR FW core_inc : 39c) RFRM 3B will detect BWR_IND and send to MPMO to begin RAMP.\n");
                ret_val = coreotn_mpmo_bwr_ind_get(digi_handle->coreotn_handle,
                                                   COREOTN_MPMO_TO_COREOTN,
                                                   fo1_bwr_ind);  

                bwr_ind_coreotn_polled = TRUE;
            }
        }


#ifndef PMC_SW_SIMULATION
        if( (fo1_bwr_ind[chnl_id/32] & (1 << (chnl_id%32))) != 0)
        {
            /*start ramping on this segment T=0!*/
            firmware_segment->state = DIGI_GHAO_FW_RAMP;
            PMC_LOG_TRACE("FW SEG : %u CORE INC in state change START -> RAMP\n", seg_idx);

            firmware_segment->total_fw_ticks = 0;

            if(ret_val == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("G.HAO BWR FW core_inc : 39d) MPMO(Stage 3B): Set new Cn\n");
                ret_val = coreotn_ghao_mpmo_new_cn_set(digi_handle->coreotn_handle,
                                                       chnl_id,
                                                       firmware_segment->new_cn_base_int,
                                                       (firmware_segment->new_cn_base_n | 0x80000000),
                                                       firmware_segment->new_cn_base_d,
                                                       firmware_segment->program_cn_n_first);
                firmware_segment->cn_changed = FALSE;
            }
        }
#else
        firmware_segment->state = DIGI_GHAO_FW_RAMP;
        
        firmware_segment->total_fw_ticks = 0;

        PMC_LOG_TRACE("FW SEG : %u CORE INC in state change START -> RAMP\n", seg_idx);

        if(ret_val == PMC_SUCCESS)
        {

            ret_val = coreotn_ghao_mpmo_new_cn_set(digi_handle->coreotn_handle,
                                                       chnl_id,
                                                       firmware_segment->new_cn_base_int,
                                                       (firmware_segment->new_cn_base_n | 0x80000000),
                                                       firmware_segment->new_cn_base_d,
                                                       firmware_segment->program_cn_n_first);
            firmware_segment->cn_changed = FALSE;
        }

        PMC_LOG_TRACE("FW SEG : %u CORE INC in state change START -> RAMP\n", seg_idx);
#endif
    }


    /* For increase, first adjustment needs to occur are t=0 */
    if(firmware_segment->state == DIGI_GHAO_FW_RAMP)
    {

        /* LINE <-> ODUKSW :
         - t = 0 is detected by receiving BWR_IND in FO1 MPMO 
         - MPMO STG3B, decrease/increase xoff threshold by 1 word every 1.25 s
         - MPMO STG3B, turn on user bit in SCBS3 every 2.5 s (start 1 ts
           tick after threshold adjustment)
         - MPMO STG3B, make DSP adjustments every 2.5 s
         - MUX, turn on ts_valid every 2.5 s
         - MUX, decrease/increase xoff threshold by 1 word every 1.25 s */

        word_tick = ((DOUBLE)(firmware_segment->total_fw_ticks))/(firmware_segment->word_update_step_core_map_us/fw_tick_us);
        sub_word_tick = ((DOUBLE)(firmware_segment->total_fw_ticks))/((firmware_segment->word_update_step_core_map_us/3)/fw_tick_us);
        ts_tick = ((DOUBLE)(firmware_segment->total_fw_ticks))/(firmware_segment->ts_update_step_us/fw_tick_us);
        cn_tick = ((DOUBLE)(firmware_segment->total_fw_ticks))/((DOUBLE)AUGMENTED_CN_DELAY_US/fw_tick_us); 
        

        PMC_LOG_TRACE("FW seg id : %u , seg type : CORE \n",seg_idx);
        PMC_LOG_TRACE("total ticks : %u \n", firmware_segment->total_fw_ticks);
        PMC_LOG_TRACE("word tick : %u \n", word_tick);
        PMC_LOG_TRACE("ts tick : %u \n", ts_tick);

        if(cn_tick > 0 && 
           firmware_segment->cn_changed == FALSE)
        {
              ret_val = coreotn_ghao_mpmo_new_cn_set(digi_handle->coreotn_handle,
                                                       chnl_id,
                                                       firmware_segment->new_cn_base_int,
                                                       firmware_segment->new_cn_base_n,
                                                       firmware_segment->new_cn_base_d,
                                                       firmware_segment->program_cn_n_first);
              firmware_segment->cn_changed = TRUE;
        }


        if( firmware_segment->current_word_step == 0 ||
            word_tick == (firmware_segment->current_word_step) ||
            sub_word_tick == (firmware_segment->current_sub_word_step))
        {
            /* Word adjustment */
            if( firmware_segment->current_word_step == 0 ||
                word_tick == (firmware_segment->current_word_step))
            {
                firmware_segment->current_word_step++;
                /*should only be incremented once per word tick*/
                firmware_segment->thresh_update_itr ++;
            }

            firmware_segment->current_sub_word_step++;

            PMC_LOG_TRACE("FW seg id : %u, seg type : CORE, Start Word adjustment %d: TS %d, Word %d, current_word_step=%d\n",
                      seg_idx,
                      word_tick,
                      ts_tick,
                      word_tick % 2,
                      firmware_segment->current_word_step);


            /* XOFF adjustment starts at t=0s */

            PMC_LOG_TRACE("FIRMWARE : Performing XOFF threshold update iteration %d\n", firmware_segment->thresh_update_itr);

            /* perform "word" adjustment in COREOTN - MPMO FO1 and MUX */
            if(ret_val == PMC_SUCCESS && 
                firmware_segment->thresh_update_itr <= firmware_segment->mpmo_thresh_update_itr_last &&
                ((firmware_segment->current_sub_word_step-1) %3) == 0 )
            {
                PMC_LOG_TRACE("G.HAO BWR FW core_inc : 39e) MPMO (Stage 3B): Increase XOFF threshold by 1TS equivalent every 2.5s\n");
                ret_val = coreotn_mpmo_xoff_thresh_change(digi_handle->coreotn_handle,
                                                              COREOTN_MPMO_TO_COREOTN,
                                                              firmware_segment->chnl_id,
                                                              firmware_segment->increase);
            }



            if(ret_val == PMC_SUCCESS)
            {
                /* If channel is passing through FMF2, perform FMF2 adjustment*/
                if(firmware_segment->ghao_mux_stage == COREOTN_MUX_STAGE_ONE && 
                   firmware_segment->fmf2_passthru == TRUE)
                {


                    if(firmware_segment->thresh_update_itr <= firmware_segment->passthru_mux_thresh_update_itr_last &&
                       ((firmware_segment->current_sub_word_step-1) %3) == 1 )
                    {
                        PMC_LOG_TRACE("G.HAO BWR FW core_inc : 39f) MUX (EXP Stage 1-2 and Stage 2-3A): Increase XOFF threshold by 1TS equivalent every 2.5s\n");
                        ret_val = coreotn_odtu_mux_xoff_thresh_update(digi_handle->coreotn_handle,
                                                                      COREOTN_MUX_STAGE_TWO,
                                                                      firmware_segment->chnl_id,
                                                                      firmware_segment->increase,
                                                                      &firmware_segment->xoff_thresh_passthru_mux);

                    }

                }
            }


            if(ret_val == PMC_SUCCESS &&
               firmware_segment->thresh_update_itr <= firmware_segment->mux_thresh_update_itr_last  &&
               ((firmware_segment->current_sub_word_step-1) %3) == 2 )
            {
                PMC_LOG_TRACE("G.HAO BWR FW core_inc : 39f) MUX (EXP Stage 1-2 and Stage 2-3A): Increase XOFF threshold by 1TS equivalent every 2.5s\n");
                /* if odu_level is MO, need to perform adustments in FMF1 mux and FMF2 mux  */
                /*note, as this is implemented currently, current_word_step is the new fill level
                in the mux*/
                ret_val = coreotn_odtu_mux_xoff_thresh_update(digi_handle->coreotn_handle,
                                                                  firmware_segment->ghao_mux_stage,
                                                                  firmware_segment->chnl_id,
                                                                  firmware_segment->increase,
                                                                  &firmware_segment->xoff_thresh);
            }



        }


        if( firmware_segment->current_ts_step == 0 ||
            ts_tick == firmware_segment->current_ts_step)
        {
            /* Tribslot adjustment */
            current_num_slots = firmware_segment->ts_start + firmware_segment->current_ts_step;

            firmware_segment->current_ts_step++;

            PMC_LOG_TRACE("FW seg id : %u, seg type : CORE, Start tribslot adjustment %d, current_ts_step=%d, current_num_slots=%d\n",
                      seg_idx,
                      ts_tick,
                      firmware_segment->current_ts_step,
                      current_num_slots);

            PMC_LOG_TRACE("FIRMWARE : Current number of tribslots in the ramp : %u \n", current_num_slots);

            /* perform "TS" adjustment in COREOTN - MPMO FO1 and MUX
                - USER_BIT in MPMO 
                - DSP GAIN in MPMO */

            /*in the increase scenerio, the userbit adjustment start at t=0.  However, the first adjustment was
            made in LCR, so the userbit adjustments will complete 1 TS before the xoff threshold adjustments*/
            
            if(firmware_segment->userbits_update_itr < firmware_segment->userbits_update_itr_last)
            {

                firmware_segment->userbits_update_itr ++;
                PMC_LOG_TRACE("FIRMWARE : Performing userbit adjustment iteration %d\n", firmware_segment->userbits_update_itr);

                if(ret_val == PMC_SUCCESS)
                {
                    
                    UINT32 cal_add_remove[96];
                    cal_add_remove[0] = firmware_segment->cal_slot_to_add_remove[firmware_segment->userbits_update_itr-1];

                    PMC_LOG_TRACE("G.HAO BWR FW core_inc : 39g) MPMO (Stage 3B): Turn valid on per Calendar every 2.5s\n");
                    ret_val = coreotn_mpmo_update_userbit_mask(digi_handle->coreotn_handle,
                                                               COREOTN_MPMO_TO_COREOTN,
                                                               firmware_segment->chnl_id,
                                                               cal_add_remove,
                                                               1,
                                                               firmware_segment->new_cal_mask,
                                                               firmware_segment->current_userbit);
                }
            }

            
            if(firmware_segment->passthru_mux_cal_update_itr < firmware_segment->passthru_mux_cal_update_itr_last &&
               firmware_segment->ghao_mux_stage == COREOTN_MUX_STAGE_ONE && 
               firmware_segment->fmf2_passthru == TRUE)
            {

                firmware_segment->passthru_mux_cal_update_itr++;
                PMC_LOG_TRACE("FIRMWARE : Performing calendar enable on pasthru mux iteration %d\n", firmware_segment->passthru_mux_cal_update_itr);

                if(ret_val == PMC_SUCCESS)
                {
                    ret_val = coreotn_mux_ghao_passthru_cal_enable_diable(digi_handle->coreotn_handle,
                                                                          firmware_segment->cal_slot_to_add_remove[firmware_segment->passthru_mux_cal_update_itr-1],
                                                                          TRUE);
                }
            }




            if (current_num_slots <= firmware_segment->ts_target &&
                current_num_slots > firmware_segment->ts_start)
            {
                /* update DSP gain in MPMO */
                if(ret_val == PMC_SUCCESS)
                {
                    PMC_LOG_TRACE("G.HAO BWR FW core_inc : 39h) MPMO (Stage 3B): Adjust age gain\n");
                    ret_val = coreotn_mpmo_adjust_age_gain(digi_handle->coreotn_handle,
                                                           COREOTN_MPMO_TO_COREOTN,
                                                           firmware_segment->chnl_id,
                                                           current_num_slots);
                }

            }

            if(current_num_slots >= (firmware_segment->ts_target+1))
            {
                /* Nothing else to do for this segment */
                firmware_segment->state = DIGI_GHAO_FW_END; 
                PMC_LOG_TRACE("FW SEG : %u CORE INC in state change RAMP -> END\n", seg_idx);
            }

        }
    }
    else if(digi_handle->var.ghao_cfg.ghao_fw_segments[seg_idx].state == DIGI_GHAO_FW_END)
    {   
        /* Firmware should not implement this! */

        if(ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO BWR FW core_inc : 40a) ODUk SC (COREOTN): Wait for RAMP done interrupt\n");
            ret_val = coreotn_oduksc_ramp_done_get(digi_handle->coreotn_handle,
                                                   firmware_segment->ghao_mux_stage,
                                                   firmware_segment->chnl_id,
                                                   &ramp_done);
#ifdef PMC_SW_SIMULATION
            ramp_done = TRUE;
#endif
        }

        if( ramp_done )
        {
            if(ret_val == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("G.HAO BWR FW core_inc : 40c) ODUk SC (COREOTN): Clean-up ramp\n");
                ret_val = coreotn_oduksc_ramp_done_isr(digi_handle->coreotn_handle,
                                                       firmware_segment->ghao_mux_stage,
                                                       firmware_segment->chnl_id);
            }

            firmware_segment->state =   DIGI_GHAO_FW_DONE;
            PMC_LOG_TRACE("FW SEG : %u CORE DEC in state change END -> DONE\n", seg_idx);
        }
    }


    firmware_segment->total_fw_ticks++;
    
    PMC_RETURN(ret_val);

} /* digi_ghao_do_fw_tick_core_inc */


/*******************************************************************************
* digi_ghao_do_fw_tick_core_dec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will be called once every FIRMWARE_TICK_US.  It is
*   intended to perform all of the G.HAO operation that will need to
*   be written into firmware for COREOTN segment decrease.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   seg_idx                - Segment index
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
PUBLIC PMC_ERROR digi_ghao_do_fw_tick_core_dec(digi_handle_t *digi_handle,
                                               UINT32 seg_idx)
{

    BOOL8 bwr_ind_coreotn_polled = FALSE;
    UINT32 fo1_bwr_ind[3];

    UINT32 chnl_id;

    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT32 fw_tick_us;
    UINT32 word_tick;
    UINT32 sub_word_tick;
    UINT32 ts_tick;
    UINT32 cn_tick;
    int current_num_slots;
    UINT32 last_word_step;
    UINT32 ramp_done = FALSE;
    digi_ghao_firmware_ctrl_t *firmware_segment;
    
    PMC_ENTRY();

    firmware_segment = &digi_handle->var.ghao_cfg.ghao_fw_segments[seg_idx];
    chnl_id = firmware_segment->chnl_id;
    fw_tick_us = digi_handle->var.ghao_cfg.fw_tick_us;
    last_word_step = (firmware_segment->ts_start - firmware_segment->ts_target - 1) * firmware_segment->ts_update_step_us / firmware_segment->word_update_step_core_map_us;

    if (firmware_segment->seg_type != DIGI_GHAO_CORE_SEG || firmware_segment->increase != FALSE)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if(firmware_segment->state == DIGI_GHAO_FW_START)
    {
        /* Detect the start of ramp */
        PMC_LOG_TRACE("FW SEG : %u CORE DEC in START state\n", seg_idx);

        /*this should only be done once per tick*/
        if(bwr_ind_coreotn_polled == FALSE)
        {
            if(ret_val == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("G.HAO BWR FW core_dec : 11c) RFRM 3B will detect BWR_IND and send to MPMO to begin RAMP.\n");
                ret_val = coreotn_mpmo_bwr_ind_get(digi_handle->coreotn_handle,
                                                   COREOTN_MPMO_TO_COREOTN,
                                                   fo1_bwr_ind);  

                bwr_ind_coreotn_polled = TRUE;
            }
        }


#ifndef PMC_SW_SIMULATION
        if( (fo1_bwr_ind[chnl_id/32] & (1 << (chnl_id%32))) != 0)
        {
            /*start ramping on this segment T=0!*/
            firmware_segment->state = DIGI_GHAO_FW_RAMP;

            firmware_segment->total_fw_ticks = 0;

            PMC_LOG_TRACE("FW SEG : %u CORE DEC in state change START -> RAMP\n", seg_idx);

            if(ret_val == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("G.HAO BWR FW core_dec : 11d) MPMO(Stage 3B): Set new Cn\n");
                ret_val = coreotn_ghao_mpmo_new_cn_set(digi_handle->coreotn_handle,
                                                       chnl_id,
                                                       firmware_segment->new_cn_base_int,
                                                       (firmware_segment->new_cn_base_n | 0x80000000 ),
                                                       firmware_segment->new_cn_base_d,
                                                       firmware_segment->program_cn_n_first);

                firmware_segment->cn_changed = FALSE;
            }
        }
#else
        firmware_segment->state = DIGI_GHAO_FW_RAMP;

        firmware_segment->total_fw_ticks = 0;

        PMC_LOG_TRACE("FW SEG : %u CORE DEC in state change START -> RAMP\n", seg_idx);

        if(ret_val == PMC_SUCCESS)
        {

            ret_val = coreotn_ghao_mpmo_new_cn_set(digi_handle->coreotn_handle,
                                                       chnl_id,
                                                       firmware_segment->new_cn_base_int,
                                                       (firmware_segment->new_cn_base_n | 0x80000000),
                                                       firmware_segment->new_cn_base_d,
                                                       firmware_segment->program_cn_n_first);
            firmware_segment->cn_changed = FALSE;
        }


        PMC_LOG_TRACE("FW SEG : %u CORE DEC in state change START -> RAMP\n", seg_idx);
#endif
    }


    /* For decrease, first adjustment needs to occur are t=2.5 or later */
    if(firmware_segment->state == DIGI_GHAO_FW_RAMP)
    {

        /* LINE <-> ODUKSW :
         - t = 0 is detected by receiving BWR_IND in FO1 MPMO 
         - MPMO STG3B, decrease/increase xoff threshold by 1 word every 1.25 s
         - MPMO STG3B, turn on user bit in SCBS3 every 2.5 s (start 1 ts
           tick after threshold adjustment)
         - MPMO STG3B, make DSP adjustments every 2.5 s
         - MUX, turn on ts_valid every 2.5 s
         - MUX, decrease/increase xoff threshold by 1 word every 1.25 s */

        word_tick = ((DOUBLE)(firmware_segment->total_fw_ticks))/(firmware_segment->word_update_step_core_map_us/fw_tick_us);
        sub_word_tick = ((DOUBLE)(firmware_segment->total_fw_ticks))/((firmware_segment->word_update_step_core_map_us/3)/fw_tick_us);
        ts_tick = ((DOUBLE)(firmware_segment->total_fw_ticks))/(firmware_segment->ts_update_step_us/fw_tick_us);
        cn_tick = ((DOUBLE)(firmware_segment->total_fw_ticks))/((DOUBLE)AUGMENTED_CN_DELAY_US/fw_tick_us); 


        PMC_LOG_TRACE("FW seg id : %u , seg type : CORE \n",seg_idx);
        PMC_LOG_TRACE("total ticks : %u \n", firmware_segment->total_fw_ticks);
        PMC_LOG_TRACE("word tick : %u \n", word_tick);
        PMC_LOG_TRACE("ts tick : %u \n", ts_tick);

        if(cn_tick > 0 && 
           firmware_segment->cn_changed == FALSE)
        {
              ret_val = coreotn_ghao_mpmo_new_cn_set(digi_handle->coreotn_handle,
                                                       chnl_id,
                                                       firmware_segment->new_cn_base_int,
                                                       firmware_segment->new_cn_base_n,
                                                       firmware_segment->new_cn_base_d,
                                                       firmware_segment->program_cn_n_first);
              firmware_segment->cn_changed = TRUE;
        }


        if( firmware_segment->current_word_step == 0 ||
            word_tick == firmware_segment->current_word_step ||
            sub_word_tick == firmware_segment->current_sub_word_step)
        {
            
            if(firmware_segment->current_word_step == 0 ||
            word_tick == firmware_segment->current_word_step)
            {
                /* Word adjustment */
                firmware_segment->current_word_step++;

                if (firmware_segment->current_word_step >= 5)
                {
                    /* XOFF adjustment starts at t=5s (current_word_step == 5)
                    can only update this iterator once per word tick */
                    firmware_segment->thresh_update_itr++;
                }

            }

            firmware_segment->current_sub_word_step++;

            PMC_LOG_TRACE("FW seg id : %u, seg type : CORE, Start Word adjustment %d: TS %d, Word %d, current_word_step=%d\n",
                      seg_idx,
                      word_tick,
                      ts_tick,
                      word_tick % 2,
                      firmware_segment->current_word_step);

            
            /* XOFF adjustment starts at t=5s (current_word_step == 5)*/
            if (firmware_segment->current_word_step >= 5)
            {

                PMC_LOG_TRACE("FIRMWARE : Performing XOFF threshold update iteration %d\n", firmware_segment->thresh_update_itr);

                /* perform "word" adjustment in COREOTN - MPMO FO1 and MUX */
                if(ret_val == PMC_SUCCESS &&
                    firmware_segment->thresh_update_itr <= firmware_segment->mpmo_thresh_update_itr_last &&
                    ((firmware_segment->current_sub_word_step-1) %3) == 0)
                {
                    PMC_LOG_TRACE("G.HAO BWR FW core_dec : 11e) MPMO (Stage 3B): Decrease XOFF threshold by 1TS equivalent every 2.5s until it hits (taget+1) TS equivalent.\n");
                    ret_val = coreotn_mpmo_xoff_thresh_change(digi_handle->coreotn_handle,
                                                              COREOTN_MPMO_TO_COREOTN,
                                                              firmware_segment->chnl_id,
                                                              firmware_segment->increase);
                }


                if(ret_val == PMC_SUCCESS)
                {
                    /* If channel is passing through FMF2, perform FMF2 adjustment*/
                    if(firmware_segment->ghao_mux_stage == COREOTN_MUX_STAGE_ONE && 
                        firmware_segment->fmf2_passthru == TRUE)
                    {


                        if(firmware_segment->thresh_update_itr <= firmware_segment->passthru_mux_thresh_update_itr_last &&
                           ((firmware_segment->current_sub_word_step-1) %3) == 1 )
                        {

                            ret_val = coreotn_odtu_mux_xoff_thresh_update(digi_handle->coreotn_handle,
                                                                          COREOTN_MUX_STAGE_TWO,
                                                                          firmware_segment->chnl_id,
                                                                          firmware_segment->increase,
                                                                          &firmware_segment->xoff_thresh_passthru_mux);

                        }

                    }
                }

                if (firmware_segment->thresh_update_itr <= firmware_segment->mux_thresh_update_itr_last &&
                    ((firmware_segment->current_sub_word_step-1) %3) == 2 )
                {
                    if(ret_val == PMC_SUCCESS)
                    {
                        PMC_LOG_TRACE("G.HAO BWR FW core_dec : 11f) MUX (EXP Stage 1-2 and Stage 2-3A): Decrease XOFF threshold by 1TS equivalent every 2.5s until it hits (taget+1) TS equivalent.\n");
                        /* if odu_level is MO, need to perform adustments in FMF1 mux and FMF2 mux  */
                        /*note, as this is implemented currently, current_word_step is the new fill level
                        in the mux*/
                        ret_val = coreotn_odtu_mux_xoff_thresh_update(digi_handle->coreotn_handle,
                                                                      firmware_segment->ghao_mux_stage,
                                                                      firmware_segment->chnl_id,
                                                                      firmware_segment->increase,
                                                                      &firmware_segment->xoff_thresh);
                    }
                }






            }
        }


        if( firmware_segment->current_ts_step == 0 || 
            ts_tick == (firmware_segment->current_ts_step))
        {
 
            current_num_slots = firmware_segment->ts_start - firmware_segment->current_ts_step;
           
            
            if(current_num_slots < (int)firmware_segment->ts_target)
            {
                current_num_slots = firmware_segment->ts_target;
            }
            

            firmware_segment->current_ts_step++;

            PMC_LOG_TRACE("FW seg id : %u, seg type : CORE, Start tribslot adjustment %d, current_ts_step=%d, current_num_slots=%d\n",
                      seg_idx,
                      ts_tick,
                      firmware_segment->current_ts_step,
                      current_num_slots);

            PMC_LOG_TRACE("FIRMWARE : Current number of tribslots in the ramp : %u \n", current_num_slots);

            /* perform "TS" adjustment in COREOTN - MPMO FO1 and MUX
                - USER_BIT in MPMO 
                - DSP GAIN in MPMO */

            /*in the decrease scenerio, the userbit adjustment start at t=7.5s.  However, the last adjustment will
            be done in LCR, so the userbit adjustments will complete 1 TS after the xoff threshold adjustments*/
            if (firmware_segment->current_ts_step >= 4 &&
                firmware_segment->userbits_update_itr < firmware_segment->userbits_update_itr_last)
            {
                /* Userbits adjustment starts at t=7.5 s (current_step =4) */
                firmware_segment->userbits_update_itr ++;
                PMC_LOG_TRACE("FIRMWARE : Performing userbit adjustment iteration %d\n", firmware_segment->userbits_update_itr);

                if(ret_val == PMC_SUCCESS)
                {

                    UINT32 cal_add_remove[96];
                    cal_add_remove[0] = firmware_segment->cal_slot_to_add_remove[firmware_segment->userbits_update_itr-1];


                    PMC_LOG_TRACE("G.HAO BWR FW core_dec : 11g) MPMO (Stage 3B): Turn valid off per Calendar every 2.5s\n");
                    ret_val = coreotn_mpmo_update_userbit_mask(digi_handle->coreotn_handle,
                                                               COREOTN_MPMO_TO_COREOTN,
                                                               firmware_segment->chnl_id,
                                                               cal_add_remove,
                                                               1,
                                                               firmware_segment->old_cal_mask,
                                                               firmware_segment->current_userbit);
                }

            }

            if(firmware_segment->current_ts_step >= 4 &&
               firmware_segment->passthru_mux_cal_update_itr < firmware_segment->passthru_mux_cal_update_itr_last &&
               firmware_segment->ghao_mux_stage == COREOTN_MUX_STAGE_ONE && 
               firmware_segment->fmf2_passthru == TRUE)
            {

                firmware_segment->passthru_mux_cal_update_itr ++;
                PMC_LOG_TRACE("FIRMWARE : Performing calendar disable on pasthru mux iteration %d\n", firmware_segment->passthru_mux_cal_update_itr);

                if(ret_val == PMC_SUCCESS)
                {
                    ret_val = coreotn_mux_ghao_passthru_cal_enable_diable(digi_handle->coreotn_handle,
                                                                          firmware_segment->cal_slot_to_add_remove[firmware_segment->passthru_mux_cal_update_itr-1],
                                                                          FALSE);
                }
            }



            if (current_num_slots < (int)firmware_segment->ts_start &&
                current_num_slots >= (int)firmware_segment->ts_target &&
                firmware_segment->ts_start >= firmware_segment->ts_target + ts_tick)
            {
                /* update DSP gain in MPMO */
                if(ret_val == PMC_SUCCESS)
                {
                    PMC_LOG_TRACE("G.HAO BWR FW core_dec : 11h) MPMO (Stage 3B): Adjust age gain\n");
                    ret_val = coreotn_mpmo_adjust_age_gain(digi_handle->coreotn_handle,
                                                           COREOTN_MPMO_TO_COREOTN,
                                                           firmware_segment->chnl_id,
                                                           current_num_slots);
                }

            }

            if(current_num_slots <= (int)firmware_segment->ts_target && 
               firmware_segment->thresh_update_itr > firmware_segment->mpmo_thresh_update_itr_last &&
               firmware_segment->thresh_update_itr > firmware_segment->mux_thresh_update_itr_last &&
               firmware_segment->userbits_update_itr == firmware_segment->userbits_update_itr_last)
            {
                /* Nothing else to do for this segment */
                firmware_segment->state = DIGI_GHAO_FW_END; 
                PMC_LOG_TRACE("FW SEG : %u CORE DEC in state change RAMP -> END\n", seg_idx);
            }
        }
    }
    else if(digi_handle->var.ghao_cfg.ghao_fw_segments[seg_idx].state == DIGI_GHAO_FW_END)
    {
        /*firmware should not implement this! */

        if(ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO BWR FW core_dec : 12a) ODUk SC (COREOTN): Wait for RAMP done interrupt\n");
            ret_val = coreotn_oduksc_ramp_done_get(digi_handle->coreotn_handle,
                                                   firmware_segment->ghao_mux_stage,
                                                   firmware_segment->chnl_id,
                                                   &ramp_done);
#ifdef PMC_SW_SIMULATION
            ramp_done = TRUE;
#endif
        }

        if( ramp_done )
        {
            if(ret_val == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("G.HAO BWR FW core_dec : 12c) ODUk SC (COREOTN): Clean-up ramp\n");
                ret_val = coreotn_oduksc_ramp_done_isr(digi_handle->coreotn_handle,
                                                       firmware_segment->ghao_mux_stage,
                                                       firmware_segment->chnl_id);
            }

            firmware_segment->state =   DIGI_GHAO_FW_DONE;
            PMC_LOG_TRACE("FW SEG : %u CORE DEC in state change END -> DONE\n", seg_idx);
        }
    }

    
    firmware_segment->total_fw_ticks++;

    PMC_RETURN(ret_val);

} /* digi_ghao_do_fw_tick_core_dec */


/*******************************************************************************
* digi_ghao_do_fw_tick_map_inc
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will be called once every FIRMWARE_TICK_US.  It is
*   intended to perform all of the G.HAO operation that will need to
*   be written into firmware for MAPOTN segment increase.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   seg_idx                - Segment index
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
PUBLIC PMC_ERROR digi_ghao_do_fw_tick_map_inc(digi_handle_t *digi_handle,
                                              UINT32 seg_idx)
{

    UINT32 bwr_ind_read[3];

    UINT32 chnl_id;

    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT32 fw_tick_us;
    UINT32 word_tick;
    UINT32 ts_tick;
    UINT32 current_num_slots;
    UINT32 ramp_done = FALSE;
    digi_ghao_firmware_ctrl_t *firmware_segment;
    UINT32 i;
    UINT32 chnl;
    UINT32 dcs_entry_to_add = 0;
    BOOL8 pinch_enable;

    PMC_ENTRY();

    firmware_segment = &digi_handle->var.ghao_cfg.ghao_fw_segments[seg_idx];
    chnl_id = firmware_segment->chnl_id;
    fw_tick_us = digi_handle->var.ghao_cfg.fw_tick_us;

    if (firmware_segment->seg_type != DIGI_GHAO_MAP_SEG || firmware_segment->increase != TRUE)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if(firmware_segment->state == DIGI_GHAO_FW_START)
    {
        PMC_LOG_TRACE("FW SEG : %u MAP INC in START state\n", seg_idx);

        if(ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO BWR FW map_inc : 39d) RFRM (Stage 4) will detect BWR_IND and send to MPMO to begin RAMP.\n");
            ret_val = coreotn_mpmo_bwr_ind_get(digi_handle->coreotn_handle,
                                               COREOTN_MPMO_TO_MAPOTN,
                                               bwr_ind_read);
        }

#ifndef PMC_SW_SIMULATION
        if( (bwr_ind_read[chnl_id/32] & (1 << (chnl_id%32))) != 0)
        {
            /*start ramping on this segment T=0!*/
            firmware_segment->state = DIGI_GHAO_FW_RAMP;
            
            firmware_segment->total_fw_ticks = 0;

            PMC_LOG_TRACE("FW SEG : %u MAP INC in state change START -> RAMP\n", seg_idx);
        }
#else
        firmware_segment->state = DIGI_GHAO_FW_RAMP;
        
        firmware_segment->total_fw_ticks = 0;

        PMC_LOG_TRACE("FW SEG : %u MAP INC in state change START -> RAMP\n", seg_idx);
#endif

    }


    /* For increase, first adjustment needs to occur are t=0 */
    if(firmware_segment->state == DIGI_GHAO_FW_RAMP)
    {
        /* MAP <-> ODUKSW :
         - t = 0 is when BWR_IND is received in FO2 MPMO
         - MPMO STG4, decrease/increase xoff threshold by 1 word every 1.25 s 
         - MPMO STG4, turn on user bit in SCBS3 every 2.5 s (start 1 ts tick after
          threshold adjustments)
         - if SIFD is in the path, set BWR_IND in MPMA 
         - End of ramp considerations (let's not worry about this too much for now):
             - poll for ramp done #1
             - extend ramp config by increasing M2*
             - Set BWR_IND = 0*
             * delay for these should be limited as much as possible */

        word_tick = ((DOUBLE)(firmware_segment->total_fw_ticks))/(firmware_segment->word_update_step_core_map_us/fw_tick_us);
        ts_tick = ((DOUBLE)(firmware_segment->total_fw_ticks))/(firmware_segment->ts_update_step_us/fw_tick_us);

        PMC_LOG_TRACE("FW seg id : %u , seg type : MAP: \n",seg_idx);
        PMC_LOG_TRACE("total ticks : %u \n", firmware_segment->total_fw_ticks);
        PMC_LOG_TRACE("word tick : %u \n", word_tick);
        PMC_LOG_TRACE("ts tick : %u \n", ts_tick);                     

        if( firmware_segment->current_word_step == 0 ||
            word_tick == (firmware_segment->current_word_step))
        {
            /* Word adjustment */
            firmware_segment->current_word_step++;

            PMC_LOG_TRACE("FW seg id : %u, seg type : MAP, Start Word adjustment %d: TS %d, Word %d, current_word_step=%d\n",
                      seg_idx,
                      word_tick,
                      ts_tick,
                      word_tick % 2,
                      firmware_segment->current_word_step);


            /* perform "word" adjustment in COREOTN - MPMO FO2 */

            if (firmware_segment->thresh_update_itr < firmware_segment->mpmo_thresh_update_itr_last)
            {
                /* XOFF adjustment starts at t=0s */
                firmware_segment->thresh_update_itr ++;
                PMC_LOG_TRACE("FIRMWARE : Performing XOFF threshold update iteration %d\n", firmware_segment->thresh_update_itr);

                if(ret_val == PMC_SUCCESS)
                {
                    PMC_LOG_TRACE("G.HAO BWR FW map_inc : 39i) MPMO(Stage 4): Increase XOFF threshold and remove the pinch of the calendar by 1TS equivalent every 2.5s\n");
                    ret_val = coreotn_mpmo_xoff_thresh_change(digi_handle->coreotn_handle,
                                                              COREOTN_MPMO_TO_MAPOTN,
                                                              firmware_segment->chnl_id,
                                                              firmware_segment->increase);
                }
            }
        }


        if( firmware_segment->current_ts_step == 0 ||
            ts_tick == (firmware_segment->current_ts_step))
        {                    
            current_num_slots = firmware_segment->ts_start + firmware_segment->current_ts_step;

            firmware_segment->current_ts_step++;

            PMC_LOG_TRACE("FW seg id : %u, seg type : MAP, Start tribslot adjustment %d, current_ts_step=%d, current_num_slots=%d\n",
                      seg_idx,
                      ts_tick,
                      firmware_segment->current_ts_step,
                      current_num_slots);

            /* perform "TS" adjustment in COREOTN - MPMO FO2
                - USER_BIT in MPMO 
                - DSP GAIN in MPMO */

            /*in the increase scenerio, the userbit adjustment start at t=0.  However, the first adjustment was
            made in LCR, so the userbit adjustments will complete 1 TS before the xoff threshold adjustments*/
            if (firmware_segment->userbits_update_itr < firmware_segment->userbits_update_itr_last)
            {
                /* Userbits adjustment starts at t=0s */
                firmware_segment->userbits_update_itr ++;
                PMC_LOG_TRACE("FIRMWARE : Performing userbit adjustment iteration 1+%d\n", firmware_segment->userbits_update_itr);


                if(ret_val == PMC_SUCCESS)
                {
                    /* populate ODUKSW DCS calendar with MAPOTN DB contents */
                    for (i = 0; i < 96; i++)
                    {
                        ret_val = mapotn_db_entry_get(digi_handle->mapotn_handle, i, &chnl);

                        coreotn_dcs_to_mapotn_pinch_entry_get(digi_handle->coreotn_handle,
                                                              i,
                                                              &pinch_enable);
                        if(pinch_enable == TRUE &&
                           chnl== firmware_segment->chnl_id)
                        {
                            dcs_entry_to_add = i;
                            break;
                        }
                    }
                }


                if (ret_val == PMC_SUCCESS)
                {
                    ret_val = coreotn_dcs_to_mapotn_pinch_entry_set(digi_handle->coreotn_handle,
                                                                    dcs_entry_to_add,
                                                                    FALSE);
                }

                if(ret_val == PMC_SUCCESS)
                {
                    PMC_LOG_TRACE("G.HAO BWR FW map_inc : 39j) ODUK Switch (LINE/SIFD -> MAP): Enable 1 TS calendar equivalent every 2.5s\n");
                    ret_val = digi_ghao_cpb_dcs_calendar_add_rem(digi_handle,
                                                                 DIGI_GHAO_ODUKSW_COREOTN_MAPOTN,
                                                                 firmware_segment->chnl_id,
                                                                 TRUE,
                                                                 &firmware_segment->current_cal_slots);
                }



                if(ret_val == PMC_SUCCESS)
                {

                    UINT32 cal_add_remove[96];
                    cal_add_remove[0] = firmware_segment->cal_slot_to_add_remove[firmware_segment->userbits_update_itr-1];

                    PMC_LOG_TRACE("G.HAO BWR FW map_inc : 39j) MPMO (Stage 4): Turn valid on per Calendar every 2.5s\n");
                    ret_val = coreotn_mpmo_update_userbit_mask(digi_handle->coreotn_handle,
                                                               COREOTN_MPMO_TO_MAPOTN,
                                                               firmware_segment->chnl_id,
                                                               cal_add_remove,
                                                               1,
                                                               firmware_segment->new_cal_mask,
                                                               firmware_segment->current_userbit);
                }

            }




            if (current_num_slots < firmware_segment->ts_target)
            {
            }

            if(current_num_slots >= firmware_segment->ts_target)
            {
                /* There are end of RAMP considerations for MAPOTN */
                firmware_segment->state = DIGI_GHAO_FW_END; 
                PMC_LOG_TRACE("FW SEG : %u MAP INC in state change RAMP -> END\n", seg_idx);
            }

        }

    }

    if(firmware_segment->state == DIGI_GHAO_FW_END)
    {
        /*the only firmware requirement here is to detect ramp done in MAPOTN.
        However, for this to be done properly, it has to be running as a seperate
        parallel process, as the MAPOTN segment will still be in DIGI_GHAO_FW_RAMP
        state when the ramp done interrupt occurs */

        if(ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO BWR FW map_inc : 12a) ODUk SC (MAPOTN): Wait for RAMP#1 done interrupt\n");
            ret_val = mapotn_oduksc_ramp_done_get(digi_handle->mapotn_handle,
                                                  firmware_segment->chnl_id,
                                                  &ramp_done);
#ifdef PMC_SW_SIMULATION
            ramp_done = TRUE;
#endif
        }

        if( ramp_done )
        {
            if(ret_val == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("G.HAO BWR FW map_inc : 12b) TGFP: After interrupt unset BWR_IND bit.\n");
                ret_val = mapotn_tgfpf_rcoh_bwr_ind_set(digi_handle->mapotn_handle,
                                                        firmware_segment->chnl_id,
                                                        0);
            }

            if(ret_val == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("G.HAO BWR FW map_inc : 12c) ODUk SC (MAPOTN): Clean-up ramp\n");
                ret_val = mapotn_oduksc_ramp_done_isr(digi_handle->mapotn_handle,
                                                      firmware_segment->chnl_id);
            }

            firmware_segment->state =   DIGI_GHAO_FW_DONE;
            PMC_LOG_TRACE("FW SEG : %u CORE MAP in state change END -> DONE\n", seg_idx);
        }
    }


    firmware_segment->total_fw_ticks++;
    
    PMC_RETURN(PMC_SUCCESS);
    
} /* digi_ghao_do_fw_tick_map_inc */


/*******************************************************************************
* digi_ghao_do_fw_tick_map_dec
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function will be called once every FIRMWARE_TICK_US.  It is
*   intended to perform all of the G.HAO operation that will need to
*   be written into firmware for MAPOTN segment decrease.
*
*
* INPUTS:
*   *digi_handle           - pointer to DIGI handle instance
*   seg_idx                - Segment index
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
PUBLIC PMC_ERROR digi_ghao_do_fw_tick_map_dec(digi_handle_t *digi_handle,
                                              UINT32 seg_idx)
{

    UINT32 bwr_ind_read[3];

    UINT32 chnl_id;

    PMC_ERROR ret_val = PMC_SUCCESS;

    UINT32 fw_tick_us;
    UINT32 word_tick;
    UINT32 ts_tick;
    int current_num_slots;
    UINT32 last_word_step;
    UINT32 ramp_done = FALSE;
    digi_ghao_firmware_ctrl_t *firmware_segment;
    UINT32 dcs_entry_to_remove=0;
    UINT32 cnt = 0;
    UINT32 i;
    UINT32 chnl;

    PMC_ENTRY();

    firmware_segment = &digi_handle->var.ghao_cfg.ghao_fw_segments[seg_idx];
    chnl_id = firmware_segment->chnl_id;
    fw_tick_us = digi_handle->var.ghao_cfg.fw_tick_us;
    last_word_step = (firmware_segment->ts_start - firmware_segment->ts_target - 1) * firmware_segment->ts_update_step_us / firmware_segment->word_update_step_core_map_us;

    if (firmware_segment->seg_type != DIGI_GHAO_MAP_SEG || firmware_segment->increase != FALSE)
    {
        PMC_RETURN(DIGI_ERR_INVALID_ARG);
    }


    if(firmware_segment->state == DIGI_GHAO_FW_START)
    {
        PMC_LOG_TRACE("FW SEG : %u MAP DEC in START state\n", seg_idx);

        if(ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO BWR FW map_dec : 11d) RFRM  (Stage 4) will detect BWR_IND and send to MPMO to begin RAMP.\n");
            ret_val = coreotn_mpmo_bwr_ind_get(digi_handle->coreotn_handle,
                                               COREOTN_MPMO_TO_MAPOTN,
                                               bwr_ind_read);
        }

#ifndef PMC_SW_SIMULATION
        if( (bwr_ind_read[chnl_id/32] & (1 << (chnl_id%32))) != 0)
        {
            /*start ramping on this segment T=0!*/
            firmware_segment->state = DIGI_GHAO_FW_RAMP;
            
            firmware_segment->total_fw_ticks = 0;

            PMC_LOG_TRACE("FW SEG : %u MAP DEC in state change START -> RAMP\n", seg_idx);
        }
#else
        firmware_segment->state = DIGI_GHAO_FW_RAMP;
        
        firmware_segment->total_fw_ticks = 0;

        PMC_LOG_TRACE("FW SEG : %u MAP DEC in state change START -> RAMP\n", seg_idx);
#endif

    }


    /* For decrease, first adjustment needs to occur are t=2.5 or later */
    if(firmware_segment->state == DIGI_GHAO_FW_RAMP)
    {
        /* MAP <-> ODUKSW :
         - t = 0 is when BWR_IND is received in FO2 MPMO
         - MPMO STG4, decrease/increase xoff threshold by 1 word every 1.25 s 
         - MPMO STG4, turn on user bit in SCBS3 every 2.5 s (start 1 ts tick after
          threshold adjustments)
         - if SIFD is in the path, set BWR_IND in MPMA 
         - End of ramp considerations (let's not worry about this too much for now):
             - poll for ramp done #1
             - extend ramp config by increasing M2*
             - Set BWR_IND = 0*
             * delay for these should be limited as much as possible */

        word_tick = ((DOUBLE)(firmware_segment->total_fw_ticks))/(firmware_segment->word_update_step_core_map_us/fw_tick_us);
        ts_tick = ((DOUBLE)(firmware_segment->total_fw_ticks))/(firmware_segment->ts_update_step_us/fw_tick_us);

        PMC_LOG_TRACE("FW seg id : %u , seg type : MAP: \n",seg_idx);
        PMC_LOG_TRACE("total ticks : %u \n", firmware_segment->total_fw_ticks);
        PMC_LOG_TRACE("word tick : %u \n", word_tick);
        PMC_LOG_TRACE("ts tick : %u \n", ts_tick);                     

        if( firmware_segment->current_word_step == 0 ||
            word_tick == (firmware_segment->current_word_step))
        {
            /* Word adjustment */
            firmware_segment->current_word_step++;

            PMC_LOG_TRACE("FW seg id : %u, seg type : MAP, Start Word adjustment %d: TS %d, Word %d, current_word_step=%d\n",
                      seg_idx,
                      word_tick,
                      ts_tick,
                      word_tick % 2,
                      firmware_segment->current_word_step);


            if (firmware_segment->current_word_step >= 3 &&
                firmware_segment->thresh_update_itr < firmware_segment->mpmo_thresh_update_itr_last)
            {
                /* XOFF adjustment starts at t=2.5s */
                firmware_segment->thresh_update_itr ++;
                PMC_LOG_TRACE("FIRMWARE : Performing XOFF threshold update iteration %d\n", firmware_segment->thresh_update_itr);

                if(ret_val == PMC_SUCCESS)
                {
                    PMC_LOG_TRACE("G.HAO BWR FW map_dec : 11i) MPMO(Stage 4): Decrease XOFF threshold by 1TS equivalent every 2.5suntil it hits (taget+1)TS equivalent.\n");
                    ret_val = coreotn_mpmo_xoff_thresh_change(digi_handle->coreotn_handle,
                                                              COREOTN_MPMO_TO_MAPOTN,
                                                              firmware_segment->chnl_id,
                                                              firmware_segment->increase);
                }
            }
        }


        if( firmware_segment->current_ts_step == 0 ||
            ts_tick == (firmware_segment->current_ts_step))
        {                    
            /* Tribslot adjustment */
            current_num_slots = firmware_segment->ts_start - firmware_segment->current_ts_step;
            
            if(current_num_slots < (int)firmware_segment->ts_target)
            {
                current_num_slots = firmware_segment->ts_target;
            }

            firmware_segment->current_ts_step++;

            PMC_LOG_TRACE("FW seg id : %u, seg type : MAP, Start tribslot adjustment %d, current_ts_step=%d, current_num_slots=%d\n",
                      seg_idx,
                      ts_tick,
                      firmware_segment->current_ts_step,
                      current_num_slots);

            PMC_LOG_TRACE("FIRMWARE : Current number of tribslots in the ramp : %u \n", current_num_slots);

            /* perform "TS" adjustment in COREOTN - MPMO FO2
                - USER_BIT in MPMO 
                - DSP GAIN in MPMO */

            /*in the decrease scenerio, the userbit adjustment start at t=2.5s.  However, the last adjustment will
            be done in LCR, so the userbit adjustments will complete 1 TS after the xoff threshold adjustments*/
            if (firmware_segment->current_ts_step >= 2 &&
                firmware_segment->userbits_update_itr < firmware_segment->userbits_update_itr_last)
            {
                /* Userbits adjustment starts at t=2.5s */
                firmware_segment->userbits_update_itr ++;
                PMC_LOG_TRACE("FIRMWARE : Performing userbit adjustment iteration %d\n", firmware_segment->userbits_update_itr);


                if(ret_val == PMC_SUCCESS)
                {
                    
                    UINT32 cal_add_remove[96];
                    cal_add_remove[0] = firmware_segment->cal_slot_to_add_remove[firmware_segment->userbits_update_itr-1];

                    PMC_LOG_TRACE("G.HAO BWR FW map_dec : 11j) MPMO (Stage 4): Turn valid off per Calendar every 2.5s\n");
                    ret_val = coreotn_mpmo_update_userbit_mask(digi_handle->coreotn_handle,
                                                               COREOTN_MPMO_TO_MAPOTN,
                                                               firmware_segment->chnl_id,
                                                               cal_add_remove,
                                                               1,
                                                               firmware_segment->old_cal_mask,
                                                               firmware_segment->current_userbit);
                }

                if(ret_val == PMC_SUCCESS)
                {

                    /* populate ODUKSW DCS calendar with MAPOTN DB contents */
                    for (i = 0; i < 96; i++)
                    {
                        ret_val = mapotn_db_entry_get(digi_handle->mapotn_handle, i, &chnl);
                        
                        if(cnt == firmware_segment->userbits_update_itr-1)
                        { 
                            dcs_entry_to_remove = i;
                        }
                        if(chnl== firmware_segment->chnl_id)
                        {
                            cnt++;
                        }
                    }
                }


                if (ret_val == PMC_SUCCESS)
                {
                    ret_val = coreotn_dcs_to_mapotn_pinch_entry_set(digi_handle->coreotn_handle,
                                                                    dcs_entry_to_remove,
                                                                    TRUE);
                }
                                                      

                if(ret_val == PMC_SUCCESS)
                {
                    PMC_LOG_TRACE("G.HAO BWR FW map_dec : 26e) ODUK Switch (LINE/SIFD -> MAP): Disable 1 TS calendar equivalent every 2.5s\n");
                    ret_val = digi_ghao_cpb_dcs_calendar_add_rem(digi_handle,
                                                                 DIGI_GHAO_ODUKSW_COREOTN_MAPOTN,
                                                                 firmware_segment->chnl_id,
                                                                 FALSE,
                                                                 &firmware_segment->current_cal_slots);
                }

            }

            if (current_num_slots < (int)firmware_segment->ts_start &&
                current_num_slots >= (int)firmware_segment->ts_target &&
                firmware_segment->ts_start >= firmware_segment->ts_target + ts_tick)
            {
            }

            if(current_num_slots <= (int)firmware_segment->ts_target &&
               firmware_segment->thresh_update_itr == firmware_segment->mpmo_thresh_update_itr_last &&
               firmware_segment->userbits_update_itr == firmware_segment->userbits_update_itr_last)
            {
                /* There are end of RAMP considerations for MAPOTN */
                firmware_segment->state = DIGI_GHAO_FW_END; 
                PMC_LOG_TRACE("FW SEG : %u MAP DEC in state change RAMP -> END\n", seg_idx);
            }

        }

    }

    if(firmware_segment->state == DIGI_GHAO_FW_END)
    {
        /*the only firmware requirement here is to detect ramp done in MAPOTN.
        However, for this to be done properly, it has to be running as a seperate
        parallel process, as the MAPOTN segment will still be in DIGI_GHAO_FW_RAMP
        state when the ramp done interrupt occurs */

        if(ret_val == PMC_SUCCESS)
        {
            PMC_LOG_TRACE("G.HAO BWR FW map_dec : 12a) ODUk SC (MAPOTN): Wait for RAMP#1 done interrupt\n");
            ret_val = mapotn_oduksc_ramp_done_get(digi_handle->mapotn_handle,
                                                  firmware_segment->chnl_id,
                                                  &ramp_done);
#ifdef PMC_SW_SIMULATION
            ramp_done = TRUE;
#endif
        }

        if( ramp_done )
        {
            if(ret_val == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("G.HAO BWR FW map_dec : 12b) TGFP: After interrupt unset BWR_IND bit.\n");
                ret_val = mapotn_tgfpf_rcoh_bwr_ind_set(digi_handle->mapotn_handle,
                                                        firmware_segment->chnl_id,
                                                        0);
            }

            if(ret_val == PMC_SUCCESS)
            {
                PMC_LOG_TRACE("G.HAO BWR FW map_dec : 12c) ODUk SC (MAPOTN): Clean-up ramp\n");
                ret_val = mapotn_oduksc_ramp_done_isr(digi_handle->mapotn_handle,
                                                      firmware_segment->chnl_id);
            }

            firmware_segment->state =   DIGI_GHAO_FW_DONE;
            PMC_LOG_TRACE("FW SEG : %u MAP DEC in state change END -> DONE\n", seg_idx);
        }
    }

    
    firmware_segment->total_fw_ticks++;

    PMC_RETURN(PMC_SUCCESS);
    
} /* digi_ghao_do_fw_tick_map_dec */







/*
** End of file
*/

