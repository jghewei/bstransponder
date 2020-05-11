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
*   DESCRIPTION : This file contains C functions for the creation and 
*                 destruction of MAPOTN subsystem and data path configuration.
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
** Log Messaging
*/

/*
** Forward References
*/
 
PRIVATE PMC_ERROR mapotn_rgfpf_unpack_fifo_thrld_change(mapotn_handle_t *mapotn_handle, 
                                                        UINT32 chnl,
                                                        UINT32 num_cal_entries,
                                                        BOOL8 thrld_minus_one);

/*
** Public Functions
*/

/*******************************************************************************
* mapotn_ghao_db_chnl_rec_update
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Allocate calendar slots to the list of tribslots in argument. If the number
*   of calendar slots is increased, new calendar slots are allocated. Otherwise,
*   the calendar slots to be removed are deallocated.
*
* INPUTS:
*   mapotn_handle          - handle to the MAPOTN instance to be operated on
*   db_id                  - Database ID in MAPOTN
*   chnl                   - Channel ID
*   new_num_cal_entries    - Number of calendar entries after resize
*
* OUTPUTS:
*   old_cal_mask           - Old calendar mask for channel
*   new_cal_mask           - New calendar mask for channel
*   cal_slots_to_add_remove_num  - Number of calendar slots to add or remove
*   cal_slots_to_add_remove      - List of ordered calendar slots to add/remove
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_ghao_db_chnl_rec_update (mapotn_handle_t *mapotn_handle,
                                                 util_gen_db_id_t db_id,
                                                 UINT32 chnl, 
                                                 UINT32 new_num_cal_entries,
                                                 UINT32 old_cal_mask[3],
                                                 UINT32 new_cal_mask[3],
                                                 UINT32 *cal_slots_to_add_remove_num,
                                                 UINT32 cal_slots_to_add_remove[79])
{
    UINT32 db_handle;
    mapotn_db_rec rec;
    mapotn_db_key	key;
    UINT32 num_of_recs;
    mapotn_db_query_res *q_result;
    mapotn_db_query_res temp_result;
    UINT32 i;
    UINT32 *available_rec_array;
    UINT32 cal_slots_add_remove_cnt = 0;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (db_id == UTIL_GEN_DB_ODUKSC_DB)
    {
        db_handle = mapotn_handle->oduksc_db_handle;
    }
    else if (db_id == UTIL_GEN_DB_MPMA_DB)
    {
        db_handle = mapotn_handle->mpma_db_handle;
    }
    else if (db_id == UTIL_GEN_DB_MPMO_DB)
    {
        db_handle = mapotn_handle->mpmo_db_handle;
    }
    else
    {
        PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
    }


    /* Initialize masks */
    for (i=0; i<3; ++i)
    {
        old_cal_mask[i] = 0;
        new_cal_mask[i] = 0;
    }

    for (i = 0; i < 79; ++i)
    {
        cal_slots_to_add_remove[i] = UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE;
    }

    key.calendar_entry = CALENDAR_ENTRIES_NUM;
    key.channel_num = chnl;
    num_of_recs = gen_db_query(&(mapotn_handle->base),
                               db_id, 
                               db_handle,
                               (void *)&key,
                               (void **)&q_result); 

    if (num_of_recs == 0)
    {
        PMC_RETURN(MAPOTN_ERR_DB_ENTRY_NOT_FOUND);
    }
    temp_result = *q_result;

    /* Add to old calendar mask */
    for (i=0; i<num_of_recs; ++i)
    {
        PMC_LOG_TRACE("mapotn_ghao_db_chnl_rec_update(): Check old: chnl=%d, num_of_recs=%d, calendar_entry=%d\n",
                  q_result->p_recs[i]->channel_num,
                  num_of_recs,
                  q_result->p_recs[i]->calendar_entry);

        old_cal_mask[q_result->p_recs[i]->calendar_entry/32] |= (1 << (q_result->p_recs[i]->calendar_entry%32));
    }

    if (new_num_cal_entries > num_of_recs)
    {
        cal_slots_add_remove_cnt = new_num_cal_entries - num_of_recs;

        available_rec_array = (UINT32*)PMC_CALLOC(sizeof(UINT32)*mapotn_handle->cfg.num_mapotn_chanl);
        PMC_ASSERT(available_rec_array != NULL, MAPOTN_ERR_CODE_ASSERT, 0, 0);

        /* Reuse currently allocated slots */
        for (i=0; i<3; ++i)
        {
            new_cal_mask[i] = old_cal_mask[i];
        }

        /* Get the list of unused calendar slots slots to use */
        ret_val = mapotn_db_empty_rec_get(mapotn_handle,
                                          cal_slots_add_remove_cnt,
                                          db_id,
                                          db_handle,
                                          available_rec_array);
        if (ret_val != PMC_SUCCESS) {
            PMC_FREE(&available_rec_array);
            PMC_RETURN(ret_val);  /* if not enough available records return an error */
        }
    
    
        /* Add the records to the database */
        for(i=0; i<cal_slots_add_remove_cnt; i++)
        {
            rec.channel_num = chnl;
            rec.calendar_entry = available_rec_array[i];

            PMC_LOG_TRACE("mapotn_ghao_db_chnl_rec_update(): Allocate cal_slot: chnl=%d, calendar_entry=%d\n",
                      rec.channel_num,
                      rec.calendar_entry);

            ret_val = gen_db_add_record(&(mapotn_handle->base),
                                       db_id,
                                       db_handle,
                                       (void *)&rec);
            if (ret_val != PMC_SUCCESS) {
                PMC_FREE(&available_rec_array);
                PMC_RETURN(ret_val);
            } 
            

            /* Add to new calendar mask */
            new_cal_mask[rec.calendar_entry/32] |= (1 << (rec.calendar_entry%32));
            /* Save added slot */
            cal_slots_to_add_remove[i] = rec.calendar_entry;
        }
    
        PMC_FREE(&available_rec_array);
    }
    else if (new_num_cal_entries < num_of_recs)
    {
        UINT32 pos = 0;
        cal_slots_add_remove_cnt = num_of_recs - new_num_cal_entries;

        /* Remove unused calendar slots */
        for (i=num_of_recs-1; i>=new_num_cal_entries; --i)
        {
            /* Remove the records from the database */
            key.calendar_entry = temp_result.p_recs[i]->calendar_entry;
            key.channel_num = chnl;

            PMC_LOG_TRACE("mapotn_ghao_db_chnl_rec_update(): Deallocate cal_slot: chnl=%d, calendar_entry=%d\n",
                      key.channel_num,
                      key.calendar_entry);

            ret_val = gen_db_rem_record(&(mapotn_handle->base),
                                       db_id,
                                       db_handle,
                                       (void *)&key);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN(ret_val);
            }
            /* Save removed slot */
            cal_slots_to_add_remove[pos] = temp_result.p_recs[i]->calendar_entry;
            pos ++;
        }
        /* Add to new calendar mask */
        for (i=0; i<new_num_cal_entries; ++i)
        {
            /* Add to new calendar mask */
            new_cal_mask[temp_result.p_recs[i]->calendar_entry/32] |= (1 << (temp_result.p_recs[i]->calendar_entry%32));
        }
    }
    else
    {
        /*PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);*/
    }


    if (db_id == UTIL_GEN_DB_ODUKSC_DB)
    {
        PMC_LOG_TRACE("mapotn_ghao_db_chnl_rec_update(): mapotn_oduksc_old_cal_slot_mask= %08X %08X %08X\n", old_cal_mask[2], old_cal_mask[1], old_cal_mask[0]);
        PMC_LOG_TRACE("mapotn_ghao_db_chnl_rec_update(): mapotn_oduksc_new_cal_slot_mask= %08X %08X %08X\n", new_cal_mask[2], new_cal_mask[1], new_cal_mask[0]);
    }
    else if (db_id == UTIL_GEN_DB_MPMA_DB)
    {
        PMC_LOG_TRACE("mapotn_ghao_db_chnl_rec_update(): mapotn_mpma_old_cal_slot_mask= %08X %08X %08X\n", old_cal_mask[2], old_cal_mask[1], old_cal_mask[0]);
        PMC_LOG_TRACE("mapotn_ghao_db_chnl_rec_update(): mapotn_mpma_new_cal_slot_mask= %08X %08X %08X\n", new_cal_mask[2], new_cal_mask[1], new_cal_mask[0]);
    }
    else if (db_id == UTIL_GEN_DB_MPMO_DB)
    {
        PMC_LOG_TRACE("mapotn_ghao_db_chnl_rec_update(): mapotn_mpmo_old_cal_slot_mask= %08X %08X %08X\n", old_cal_mask[2], old_cal_mask[1], old_cal_mask[0]);
        PMC_LOG_TRACE("mapotn_ghao_db_chnl_rec_update(): mapotn_mpmo_new_cal_slot_mask= %08X %08X %08X\n", new_cal_mask[2], new_cal_mask[1], new_cal_mask[0]);
    }

    PMC_LOG_TRACE("mapotn_ghao_db_chnl_rec_update(): cal_slots_to_add_remove=%d,\n", cal_slots_add_remove_cnt);
    for (i = 0; i < cal_slots_add_remove_cnt; ++i)
    {
        PMC_LOG_TRACE("cal_slots_to_add_remove[%d]: %d\n", i, cal_slots_to_add_remove[i]);
    }

    *cal_slots_to_add_remove_num = cal_slots_add_remove_cnt;

    PMC_RETURN(ret_val);

} /* mapotn_ghao_db_chnl_rec_update */

/*******************************************************************************
* mapotn_ghao_db_chnl_get_masks
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reserve calendar slots to the list of tribslots in argument. If the number
*   of calendar slots is increased, new calendar slots are reserved. Otherwise,
*   the calendar slots to be removed are unreserved.
*   This function only generates the old and new calendar slot masks and
*   doesn't make any change to the database.
*
* INPUTS:
*   mapotn_handle          - handle to the MAPOTN instance to be operated on
*   db_id                  - Database ID in MAPOTN
*   chnl                   - Channel ID
*   new_num_cal_entries    - Number of calendar entries after resize
*
* OUTPUTS:
*   old_cal_mask           - Old calendar mask for channel
*   new_cal_mask           - New calendar mask for channel
*   cal_slots_to_add_remove_num  - Number of calendar slots to add or remove
*   cal_slots_to_add_remove      - List of ordered calendar slots to add/remove
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise. 
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_ghao_db_chnl_get_masks (mapotn_handle_t *mapotn_handle,
                                                util_gen_db_id_t db_id,
                                                UINT32 chnl, 
                                                UINT32 new_num_cal_entries,
                                                UINT32 old_cal_mask[3],
                                                UINT32 new_cal_mask[3],
                                                UINT32 *cal_slots_to_add_remove_num,
                                                UINT32 cal_slots_to_add_remove[79])
{
    UINT32 db_handle;
    mapotn_db_rec rec;
    mapotn_db_key	key;
    UINT32 num_of_recs;
    mapotn_db_query_res *q_result;
    UINT32 i;
    UINT32 *available_rec_array;
    UINT32 cal_slots_add_remove_cnt;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    if (db_id == UTIL_GEN_DB_ODUKSC_DB)
    {
        db_handle = mapotn_handle->oduksc_db_handle;
    }
    else if (db_id == UTIL_GEN_DB_MPMA_DB)
    {
        db_handle = mapotn_handle->mpma_db_handle;
    }
    else if (db_id == UTIL_GEN_DB_MPMO_DB)
    {
        db_handle = mapotn_handle->mpmo_db_handle;
    }
    else
    {
        PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
    }


    /* Initialize masks */
    for (i=0; i<3; ++i)
    {
        old_cal_mask[i] = 0;
        new_cal_mask[i] = 0;
    }

    for (i = 0; i < 79; ++i)
    {
        cal_slots_to_add_remove[i] = UTIL_SCHD_SCBS3_NULL_ENTRY_VALUE;
    }

    key.calendar_entry = CALENDAR_ENTRIES_NUM;
    key.channel_num = chnl;
    num_of_recs = gen_db_query(&(mapotn_handle->base),
                               db_id, 
                               db_handle,
                               (void *)&key,
                               (void **)&q_result); 

    if (num_of_recs == 0)
    {
        PMC_RETURN(MAPOTN_ERR_DB_ENTRY_NOT_FOUND);
    }

    /* Add to old calendar mask */
    for (i=0; i<num_of_recs; ++i)
    {
        PMC_LOG_TRACE("mapotn_ghao_db_chnl_rec_update(): Check old: chnl=%d, num_of_recs=%d, calendar_entry=%d\n",
                  q_result->p_recs[i]->channel_num,
                  num_of_recs,
                  q_result->p_recs[i]->calendar_entry);

        old_cal_mask[q_result->p_recs[i]->calendar_entry/32] |= (1 << (q_result->p_recs[i]->calendar_entry%32));
    }

    if (new_num_cal_entries > num_of_recs)
    {
        cal_slots_add_remove_cnt = new_num_cal_entries - num_of_recs;

        available_rec_array = (UINT32*)PMC_CALLOC(sizeof(UINT32)*mapotn_handle->cfg.num_mapotn_chanl);
        PMC_ASSERT(available_rec_array != NULL, MAPOTN_ERR_CODE_ASSERT, 0, 0);

        /* Reuse currently allocated slots */
        for (i=0; i<3; ++i)
        {
            new_cal_mask[i] = old_cal_mask[i];
        }

        /* Get the list of unused calendar slots slots to use */
        ret_val = mapotn_db_empty_rec_get(mapotn_handle,
                                          cal_slots_add_remove_cnt,
                                          db_id,
                                          db_handle,
                                          available_rec_array);
        if (ret_val != PMC_SUCCESS) {
            PMC_FREE(&available_rec_array);
            PMC_RETURN(ret_val);  /* if not enough available records return an error */
        }
    
    
        /* Add the records to the database */
        for(i=0; i<cal_slots_add_remove_cnt; i++)
        {
            PMC_LOG_TRACE("mapotn_ghao_db_chnl_rec_update(): Reserve cal_slot: chnl=%d, calendar_entry=%d\n",
                      chnl,
                      available_rec_array[i]);

            /* Add to new calendar mask */
            rec.calendar_entry = available_rec_array[i];
            new_cal_mask[rec.calendar_entry/32] |= (1 << (rec.calendar_entry%32));
            /* Save added slot */
            cal_slots_to_add_remove[i] = q_result->p_recs[i]->calendar_entry;
        }
    
        PMC_FREE(&available_rec_array);
    }
    else if (new_num_cal_entries < num_of_recs)
    {
        UINT32 pos = 0;
        cal_slots_add_remove_cnt = num_of_recs - new_num_cal_entries;

        /* Remove unused calendar slots */
        for (i=num_of_recs-1; i>=new_num_cal_entries; --i)
        {
            PMC_LOG_TRACE("mapotn_ghao_db_chnl_rec_update(): Unreserve cal_slot: chnl=%d, calendar_entry=%d\n",
                      chnl,
                      q_result->p_recs[i]->calendar_entry);

            /* Save removed slot */
            cal_slots_to_add_remove[pos] = q_result->p_recs[i]->calendar_entry;
            pos ++;
        }
        /* Add to new calendar mask */
        for (i=0; i<new_num_cal_entries; ++i)
        {
            /* Add to new calendar mask */
            new_cal_mask[q_result->p_recs[i]->calendar_entry/32] |= (1 << (q_result->p_recs[i]->calendar_entry%32));
        }
    }
    else
    {
        PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
    }


    if (db_id == UTIL_GEN_DB_ODUKSC_DB)
    {
        PMC_LOG_TRACE("mapotn_ghao_db_chnl_get_masks(): mapotn_oduksc_old_cal_slot_mask= %08X %08X %08X\n", old_cal_mask[2], old_cal_mask[1], old_cal_mask[0]);
        PMC_LOG_TRACE("mapotn_ghao_db_chnl_get_masks(): mapotn_oduksc_new_cal_slot_mask= %08X %08X %08X\n", new_cal_mask[2], new_cal_mask[1], new_cal_mask[0]);
    }
    else if (db_id == UTIL_GEN_DB_MPMA_DB)
    {
        PMC_LOG_TRACE("mapotn_ghao_db_chnl_get_masks(): mapotn_mpma_old_cal_slot_mask= %08X %08X %08X\n", old_cal_mask[2], old_cal_mask[1], old_cal_mask[0]);
        PMC_LOG_TRACE("mapotn_ghao_db_chnl_get_masks(): mapotn_mpma_new_cal_slot_mask= %08X %08X %08X\n", new_cal_mask[2], new_cal_mask[1], new_cal_mask[0]);
    }
    else if (db_id == UTIL_GEN_DB_MPMO_DB)
    {
        PMC_LOG_TRACE("mapotn_ghao_db_chnl_get_masks(): mapotn_mpmo_old_cal_slot_mask= %08X %08X %08X\n", old_cal_mask[2], old_cal_mask[1], old_cal_mask[0]);
        PMC_LOG_TRACE("mapotn_ghao_db_chnl_get_masks(): mapotn_mpmo_new_cal_slot_mask= %08X %08X %08X\n", new_cal_mask[2], new_cal_mask[1], new_cal_mask[0]);
    }

    PMC_LOG_TRACE("mapotn_ghao_db_chnl_get_masks(): cal_slots_to_add_remove=%d,\n", cal_slots_add_remove_cnt);
    for (i = 0; i < cal_slots_add_remove_cnt; ++i)
    {
        PMC_LOG_TRACE("cal_slots_to_add_remove[%d]: %d\n", i, cal_slots_to_add_remove[i]);
    }

    *cal_slots_to_add_remove_num = cal_slots_add_remove_cnt;

    PMC_RETURN(ret_val);

} /* mapotn_ghao_db_chnl_get_masks */


/*******************************************************************************
* mapotn_rgfpf_cfc_thresh_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the CFC fifos and thresholds in the RGFPF for an ODUFLEX channel
*   so that it supports the specified rate in G.HAO ODUflex resizing.
*   In G.HAO ODUflex resizing, the reprovision is in the following order:
*   Ramp-up:
*   1) mapotn_rgfpf_chnl_unpack_fifo_reprov
*   2) mapotn_rgfpf_chnl_egress_fifo_reprov
*   Ramp-down:
*   1) mapotn_rgfpf_chnl_egress_fifo_reprov
*   2) mapotn_rgfpf_chnl_unpack_fifo_reprov
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of tribslots
*                         1 to 8 -> ODU2, 1 to 32 -> ODU3, 1 to 80 -> ODU4.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_cfc_thresh_reprov(mapotn_handle_t *mapotn_handle,
                                                UINT32 chnl,
                                                UINT32 oduflex_gfp_rate)
{
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_LOG_TRACE( "Channel = %u, oduflex_gfp_rate = %u\n", chnl, oduflex_gfp_rate);  

    /* Avideh, do you want to add a new state for re-provisioning */
    if (mapotn_handle->var.rx_chnl_ctxt[chnl].state != MAPOTN_CHNL_OPERATIONAL)
    {
        PMC_RETURN(MAPOTN_CHNL_STATE_NOT_OPERATIONAL);
    }

    /*
       it is operational, energy state is correct
       check if this channel is valid candidate for reprovisioning
    */
    if (mapotn_handle->var.rx_chnl_ctxt[chnl].adaptation_func != UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP  ||
        mapotn_handle->var.rx_chnl_ctxt[chnl].odukp_type      != UTIL_GLOBAL_ODUFLEX_GFP ||
        mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode    != UTIL_GLOBAL_GFP ||
        mapotn_handle->var.rx_chnl_ctxt[chnl].src_dest        != MAPOTN_DEST_CPB )
    {
        PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
    }

    /*
      validate new gfp flex rate
     */
    if ( mapotn_handle->var.rx_chnl_ctxt[chnl].num_cal_entries == oduflex_gfp_rate ||
         oduflex_gfp_rate > 80)
    {
          PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
    }

    if (oduflex_gfp_rate > mapotn_handle->var.rx_chnl_ctxt[chnl].num_cal_entries)
    {
        /* RAMP UP */

        /* Reprovision unpack fifo */
        ret_val = mapotn_rgfpf_chnl_unpack_fifo_reprov(mapotn_handle, chnl, oduflex_gfp_rate);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        /* Reprovision egress fifo */
        ret_val = mapotn_rgfpf_chnl_egress_fifo_reprov(mapotn_handle, chnl, oduflex_gfp_rate);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    } else {
        /* RAMP DOWN */

        /* Reprovision egress fifo */
        ret_val = mapotn_rgfpf_chnl_egress_fifo_reprov(mapotn_handle, chnl, oduflex_gfp_rate);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        /* Reprovision unpack fifo */
        ret_val = mapotn_rgfpf_chnl_unpack_fifo_reprov(mapotn_handle, chnl, oduflex_gfp_rate);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    /* Update var context */
    mapotn_handle->var.rx_chnl_ctxt[chnl].num_cal_entries = oduflex_gfp_rate;

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_cfc_thresh_reprov */


/*******************************************************************************
* mapotn_rgfpf_chnl_unpack_fifo_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the unpack fifo  with a new number of calendar slots for the
*   specified channel. This function is used for G.HAO ODUflex resizing. This
*   function reprovisions the CFC fifo and updates the unpack fifo xoff
*   threshold.
*   The function calls for G.HAO should be in the following order:
*   Ramp-up:
*   2) mapotn_rgfpf_chnl_unpack_fifo_reprov (thresholds, CFC fifo)
*   2) mapotn_tgfpf_chnl_egress_fifo_reprov (threshold, CFC fifo)
*   Ramp-down:
*   1) mapotn_tgfpf_chnl_egress_fifo_reprov (CFC fifo, threshold)
*   1) mapotn_rgfpf_chnl_unpack_fifo_reprov (CFC fifo, thresholds)
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   new_num_cal_entries - number of calendar entries after resizing
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES: The wait delays defined here are a minimum delay. Extending these
*        delays would not cause any issue.
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_chnl_unpack_fifo_reprov(mapotn_handle_t *mapotn_handle,
                                                      UINT32 chnl,
                                                      UINT32 new_num_cal_entries)
{
    /* variable declaration */
    UINT32 num_cal_entries;
    UINT32 ramp_down_ratio;
    UINT32 wait_delay_us;
    PMC_ERROR ret_val = PMC_SUCCESS;

    /* variable initialization */
    num_cal_entries     = new_num_cal_entries;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_LOG(PMC_LOG_SEV_HIGHEST, MAPOTN_LOG_CODE_LOG, 0, 0, "Channel = %u, num_cal_entries = %u\n", chnl, num_cal_entries );



    if (num_cal_entries > mapotn_handle->var.rx_chnl_ctxt[chnl].num_cal_entries) {
        /* Ramp up */

        /* Resize Channelized FIFO Controller */
        ret_val = cfc_fifo_resize(mapotn_handle->cfc_unpack_handle, chnl, num_cal_entries);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }

        /* Reconfigure unpack fifo threshold */
        ret_val = mapotn_rgfpf_unpack_fifo_thrld_change(mapotn_handle, chnl, num_cal_entries, FALSE);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
    } else {
        /* Ramp down */

        /* Reconfigure unpack fifo threshold */
        ret_val = mapotn_rgfpf_unpack_fifo_thrld_change(mapotn_handle, chnl, num_cal_entries, FALSE);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }

        /* Add some delay to let the fifo fill level decrease before resizing CFC fifo */
        ramp_down_ratio = mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries / num_cal_entries;
        wait_delay_us = (ramp_down_ratio / 2) + 1; /* Fifo with 2 locations/TS and expected half full*/
        PMC_Q_USLEEP(mapotn_handle,wait_delay_us);

        /* Resize Channelized FIFO Controller */
        ret_val = cfc_fifo_resize(mapotn_handle->cfc_unpack_handle, chnl, num_cal_entries);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_chnl_unpack_fifo_reprov */


/*******************************************************************************
* mapotn_rgfpf_chnl_egress_fifo_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the egress fifo  with a new number of calendar slots for the
*   specified channel. This function is used for G.HAO ODUflex resizing. This
*   function reprovisions the CFC fifo and updates the egress fifo xoff
*   threshold.
*   The function calls for G.HAO should be in the following order:
*   Ramp-up:
*   2) mapotn_rgfpf_chnl_unpack_fifo_reprov (thresholds, CFC fifo)
*   2) mapotn_tgfpf_chnl_egress_fifo_reprov (threshold, CFC fifo)
*   Ramp-down:
*   1) mapotn_tgfpf_chnl_egress_fifo_reprov (CFC fifo, threshold)
*   1) mapotn_rgfpf_chnl_unpack_fifo_reprov (CFC fifo, thresholds)
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   new_num_cal_entries - number of calendar entries after resizing
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_chnl_egress_fifo_reprov(mapotn_handle_t *mapotn_handle,
                                                      UINT32 chnl,
                                                      UINT32 new_num_cal_entries)
{
    /* variable declaration */
    UINT32 num_cal_entries;
    UINT32 egress_thrld;
    PMC_ERROR ret_val = PMC_SUCCESS;

    /* variable initialization */
    num_cal_entries     = new_num_cal_entries;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_LOG(PMC_LOG_SEV_HIGHEST, MAPOTN_LOG_CODE_LOG, 0, 0, "Channel = %u, num_cal_entries = %u\n", chnl, num_cal_entries );


    if (num_cal_entries > mapotn_handle->var.rx_chnl_ctxt[chnl].num_cal_entries) {
        /* Ramp up */

        /* Resize Channelized FIFO Controller */
        ret_val = cfc_fifo_resize(mapotn_handle->cfc_egress_handle, chnl, num_cal_entries);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }

        /* Reconfigure egress fifo threshold */
        egress_thrld = num_cal_entries;
        mapotn_rgfpf_field_EGRESS_XOFF_THRLD_set(NULL, mapotn_handle, chnl, egress_thrld);
    } else {
        /* Ramp down */

        /* Reconfigure egress fifo threshold */
        egress_thrld = num_cal_entries;
        mapotn_rgfpf_field_EGRESS_XOFF_THRLD_set(NULL, mapotn_handle, chnl, egress_thrld);

        /* Resize Channelized FIFO Controller */
        ret_val = cfc_fifo_resize(mapotn_handle->cfc_egress_handle, chnl, num_cal_entries);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_chnl_egress_fifo_reprov */


/*******************************************************************************
* mapotn_rgfpf_rcoh_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function returns a pointer to the RCOH header received which is
*   different from the last received RCOH.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *rcoh               - pointer to RCOH header received
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_rcoh_get(mapotn_handle_t *mapotn_handle,
                                       UINT32 chnl,
                                       UINT32 *rcoh)
{
    /* variable declaration */
    UINT32 num_iter;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    /* Transfer RCOH_STATUS active ram to register */
    ohfs_remove_trnsfr_rcoh_to_shadow_en(mapotn_handle->ohfs_remove_handle);

    /* Poll for RCOH status transfer done */
    ret_val = mapotn_rgfpf_field_RCOH_TRNSFR_SHDW_DNE_I_poll ( NULL, mapotn_handle, 1, PMC_POLL_EQUAL,
                                                               10, &num_iter, 1 );
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    /* Clear RCOH status transfer done Interrupt */
    mapotn_rgfpf_field_RCOH_TRNSFR_SHDW_DNE_I_set_to_clear( NULL, mapotn_handle , 1);

    /* Read RCOH status */
    ohfs_remove_rcoh_get(mapotn_handle->ohfs_remove_handle, chnl, rcoh);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_rcoh_get */


/*******************************************************************************
* mapotn_rgfpf_rcoh_ncs_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function reads the RCOH status register and returns the NCS bit
*   received in RCOH overhead.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *ncs                - pointer to NCS bit received
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_rcoh_ncs_get(mapotn_handle_t *mapotn_handle,
                                       UINT32 chnl,
                                       UINT32 *ncs)
{
    /* variable declaration */
    UINT32 rcoh;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    /* Read RCOH status */
    ret_val = mapotn_rgfpf_rcoh_get( mapotn_handle, chnl, &rcoh );
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    /* Read NCS */
    *ncs = (rcoh >> 6) & 0x1;

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_rcoh_ncs_get */


/*******************************************************************************
* mapotn_rgfpf_rcoh_bwr_ind_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function reads the RCOH status register and returns the BWR_IND bit
*   received in RCOH overhead.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *bwr_ind            - pointer to BWR_IND bit received
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_rcoh_bwr_ind_get(mapotn_handle_t *mapotn_handle,
                                               UINT32 chnl,
                                               UINT32 *bwr_ind)
{
    /* variable declaration */
    UINT32 rcoh;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    /* Read RCOH status */
    ret_val = mapotn_rgfpf_rcoh_get( mapotn_handle, chnl, &rcoh );
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(PMC_ERR_FAIL);
    }

    /* BWR_IND is on bits 7 and 15 */
    if (((rcoh >> 7) & 0x1) == ((rcoh >> 15) & 0x1))
    {
        /* Compare BWR_IND */
    	*bwr_ind = (rcoh >> 7) & 0x1;
    }
    else
    {
        /* BWR_IND differs */
        PMC_RETURN(PMC_ERR_FAIL);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_rcoh_bwr_ind_get */


/*******************************************************************************
* mapotn_rgfpf_rcoh_ncs_poll
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This functions polls the RCOH status register until the NCS bit received in
*   RCOH overhead is equal to the expected value.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   ncs                 - RCOH NCS value to poll
*   poll_delay_us       - Delay in microsecond between each read
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_rcoh_ncs_poll(mapotn_handle_t *mapotn_handle,
                                            UINT32 chnl,
                                            UINT32 ncs,
                                            UINT32 poll_delay_us)
{
    /* variable declaration */
    UINT32 max_poll_iter;
    UINT32 rcoh;
    UINT32 cur_iter;
    UINT32 ncs_read;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    max_poll_iter = 100;

    cur_iter = 0;
    while (cur_iter < max_poll_iter)
    {
        /* Read RCOH status */
        ret_val = mapotn_rgfpf_rcoh_get( mapotn_handle, chnl, &rcoh );
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(PMC_ERR_FAIL);
        }

        /* Compare NCS */
        ncs_read = (rcoh >> 6) & 0x1;
        if (ncs_read == ncs)
        {
            PMC_RETURN(PMC_SUCCESS);
        }

        /* Wait polling delay */
        PMC_Q_USLEEP(mapotn_handle,poll_delay_us);
        cur_iter ++;
    }

    PMC_RETURN(PMC_ERR_FAIL);

} /* mapotn_rgfpf_rcoh_ncs_poll */


/*******************************************************************************
* mapotn_rgfpf_rcoh_bwr_ind_poll
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function polls the RCOH status register until the BWR_IND bit received
*   in RCOH overhead is equal to the expected value.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   bwr_ind             - RCOH BWR_IND value to poll
*   poll_delay_us       - Delay in microsecond between each read
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_rgfpf_rcoh_bwr_ind_poll(mapotn_handle_t *mapotn_handle,
                                                UINT32 chnl,
                                                UINT32 bwr_ind,
                                                UINT32 poll_delay_us)
{
    /* variable declaration */
    UINT32 max_poll_iter;
    UINT32 rcoh;
    UINT32 cur_iter;
    UINT32 bwr_ind_read;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    max_poll_iter = 100;

    cur_iter = 0;
    while (cur_iter < max_poll_iter)
    {
        /* Read RCOH status */
        ret_val = mapotn_rgfpf_rcoh_get( mapotn_handle, chnl, &rcoh );
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(PMC_ERR_FAIL);
        }

        /* BWR_IND is on bits 7 and 15 */
        if (((rcoh >> 7) & 0x1) == ((rcoh >> 15) & 0x1))
        {
            /* Compare BWR_IND */
            bwr_ind_read = (rcoh >> 7) & 0x1;
            if (bwr_ind_read == bwr_ind)
            {
                PMC_RETURN(PMC_SUCCESS);
            }
        }
        else
        {
            /* BWR_IND differs, continue polling */
        }

        /* Wait polling delay */
        PMC_Q_USLEEP(mapotn_handle,poll_delay_us);
        cur_iter ++;
    }

    PMC_RETURN(PMC_ERR_FAIL);

} /* mapotn_rgfpf_rcoh_bwr_ind_poll */


/*******************************************************************************
* mapotn_mpma_scbs3_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the scbs3 calendar for an ODUFLEX channel to the new number of
*   calendar slots for G.HAO ODUflex resizing.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of tribslots
*                         1 to 8 -> ODU2, 1 to 32 -> ODU3, 1 to 80 -> ODU4.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_mpma_scbs3_reprov(mapotn_handle_t *mapotn_handle,
                                          UINT32 chnl,
                                          UINT32 oduflex_gfp_rate)
{
    DOUBLE rate;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_LOG_TRACE( "Channel = %u, oduflex_gfp_rate = %u\n", chnl, oduflex_gfp_rate);

    /* Avideh, do you want to add a new state for re-provisioning */
    if (mapotn_handle->var.rx_chnl_ctxt[chnl].state != MAPOTN_CHNL_OPERATIONAL)
    {
        PMC_RETURN(MAPOTN_CHNL_STATE_NOT_OPERATIONAL);
    }

    /*
       it is operational, energy state is correct
       check if this channel is valid candidate for reprovisioning
    */
    if (mapotn_handle->var.rx_chnl_ctxt[chnl].adaptation_func != UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP  ||
        mapotn_handle->var.rx_chnl_ctxt[chnl].odukp_type      != UTIL_GLOBAL_ODUFLEX_GFP ||
        mapotn_handle->var.rx_chnl_ctxt[chnl].mapping_mode    != UTIL_GLOBAL_GFP ||
        mapotn_handle->var.rx_chnl_ctxt[chnl].src_dest        != MAPOTN_DEST_CPB )
    {
        PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
    }

    if(mapotn_handle->cfg.db_mode == MAPOTN_DB_MODE_LOCAL)
    {
        /* remove previous entries from the MPMA database */
        ret_val = mapotn_db_chnl_rec_rem(mapotn_handle, chnl, UTIL_GEN_DB_MPMA_DB);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        /* add channel data to the MPMA database */
        ret_val = mapotn_db_chnl_rec_add(mapotn_handle, chnl, oduflex_gfp_rate , UTIL_GEN_DB_MPMA_DB);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    /* reprovision MPMA SCBS3 */
    rate = oduflex_gfp_rate * UTIL_GLOBAL_ODU0_RATE;
    
    if (oduflex_gfp_rate >= 1 && oduflex_gfp_rate <= 8)
    {
        /* 1.24917723 Gb/s per TS*/
        rate = (oduflex_gfp_rate * UTIL_GLOBAL_ODU2_TS_RATE);
    }
    else if (oduflex_gfp_rate <= 32)
    {
        /* 1.25447054 Gb/s per TS*/
        rate = (oduflex_gfp_rate * UTIL_GLOBAL_ODU3_TS_RATE);
    }
    else if (oduflex_gfp_rate <= 80)
    {
        /* 1.301467133 Gb/s per TS*/
        rate = (oduflex_gfp_rate * UTIL_GLOBAL_ODU4_TS_RATE);
    }

    
    
    ret_val = mpma_schd_ch_reprov(mapotn_handle->mpma_handle, chnl, rate);

    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_mpma_scbs3_reprov */


/*******************************************************************************
* mapotn_tgfpf_cfc_thresh_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the CFC fifos and thresholds in the TGFPF for an ODUFLEX channel
*   so that it supports the specified rate in G.HAO ODUflex resizing.
*   In G.HAO ODUflex resizing, the reprovision is in the following order:
*   Ramp-up:
*   1) mapotn_tgfpf_chnl_ohfs_insert_fifo_reprov
*   2) mapotn_tgfpf_chnl_packing_fifo_reprov
*   3) mapotn_tgfpf_chnl_prefetch_fifo_reprov
*   Ramp-down:
*   1) mapotn_tgfpf_chnl_prefetch_fifo_reprov
*   2) mapotn_tgfpf_chnl_packing_fifo_reprov
*   3) mapotn_tgfpf_chnl_ohfs_insert_fifo_reprov
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of tribslots
*                         1 to 8 -> ODU2, 1 to 32 -> ODU3, 1 to 80 -> ODU4.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_cfc_thresh_reprov(mapotn_handle_t *mapotn_handle,
                                                UINT32 chnl,
                                                UINT32 oduflex_gfp_rate)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_LOG_TRACE( "Channel = %u, oduflex_gfp_rate = %u\n", chnl, oduflex_gfp_rate);  

    if (mapotn_handle->var.tx_chnl_ctxt[chnl].state != MAPOTN_CHNL_OPERATIONAL)
    {
         PMC_RETURN(MAPOTN_CHNL_STATE_NOT_OPERATIONAL);
    }

    /*
       it is operational, energy state is correct
       check if this channel is valid candidate for reprovisioning
    */
    if (mapotn_handle->var.tx_chnl_ctxt[chnl].adaptation_func != UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP  ||
        mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type      != UTIL_GLOBAL_ODUFLEX_GFP ||
        mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode    != UTIL_GLOBAL_GFP ||
        mapotn_handle->var.tx_chnl_ctxt[chnl].src_dest        != MAPOTN_DEST_CPB )
    {
        PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
    }

    /*
      validate new gfp flex rate
    */
    if ( mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries == oduflex_gfp_rate ||
         oduflex_gfp_rate > 80)
    {
        PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
    }


    if (oduflex_gfp_rate > mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries)
    {
        /* RAMP UP */

        /* Reprovision ohfs insert fifo */
        ret_val = mapotn_tgfpf_chnl_ohfs_insert_fifo_reprov(mapotn_handle, chnl, oduflex_gfp_rate);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        /* Reprovision packing fifo */
        ret_val = mapotn_tgfpf_chnl_packing_fifo_reprov(mapotn_handle, chnl, oduflex_gfp_rate);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        /* Reprovision prefetch fifo */
        ret_val = mapotn_tgfpf_chnl_prefetch_fifo_reprov(mapotn_handle, chnl, oduflex_gfp_rate);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    } else {
        /* RAMP DOWN */

        /* Reprovision prefetch fifo */
        ret_val = mapotn_tgfpf_chnl_prefetch_fifo_reprov(mapotn_handle, chnl, oduflex_gfp_rate);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        /* Reprovision packing fifo */
        ret_val = mapotn_tgfpf_chnl_packing_fifo_reprov(mapotn_handle, chnl, oduflex_gfp_rate);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        /* Reprovision prefetch fifo */
        ret_val = mapotn_tgfpf_chnl_ohfs_insert_fifo_reprov(mapotn_handle, chnl, oduflex_gfp_rate);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    /* Update var context */
    mapotn_handle->var.tx_chnl_ctxt[chnl].ghao_start_oduflex_gfp_rate = mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries;
    mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries = oduflex_gfp_rate;

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_cfc_thresh_reprov */


/*******************************************************************************
* mapotn_tgfpf_chnl_prefetch_fifo_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the prefetch fifo  with a new number of calendar slots for the
*   specified channel. This function is used for G.HAO ODUflex resizing. This
*   function reprovisions the CFC fifo and updates the prefetch fifo threshold.
*   For a large number of slots in ramp-up or ramp-done, this resizing is done
*   in a few iterations.
*   The function calls for G.HAO should be in the following order:
*   Ramp-up:
*   1) mapotn_tgfpf_chnl_ohfs_insert_reprov (threshold, CFC fifo)
*   2) mapotn_tgfpf_chnl_packing_fifo_reprov (thresholds, CFC fifo)
*   2) mapotn_tgfpf_chnl_prefetch_fifo_reprov (threshold, CFC fifo)
*   Ramp-down:
*   1) mapotn_tgfpf_chnl_prefetch_fifo_reprov (CFC fifo, threshold)
*   1) mapotn_tgfpf_chnl_packing_fifo_reprov (CFC fifo, thresholds)
*   2) mapotn_tgfpf_chnl_ohfs_insert_reprov (CFC fifo, threshold)
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   new_num_cal_entries - number of calendar entries after resizing
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES: The wait delays defined here are a minimum delay. Extending these
*        delays would not cause any issue.
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_prefetch_fifo_reprov(mapotn_handle_t *mapotn_handle,
                                                        UINT32 chnl,
                                                        UINT32 new_num_cal_entries)
{
    /* variable declaration */
    UINT32 prefetch_thrld;
    UINT32 num_cal_entries;
    PMC_ERROR ret_val = PMC_SUCCESS;
    UINT32 previous_num_cal_entries;

    /* variable initialization */
    prefetch_thrld = 0;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    /* retrieve channel's status from memory */
    num_cal_entries     = mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries;


    if (new_num_cal_entries > num_cal_entries) {
        /* Ramp up */

        /* Done in 1-3 steps as a workaround for an issue in the prefetch fifo block */
        while (num_cal_entries < new_num_cal_entries) {
            /* Ramp-up iteration */
            if (new_num_cal_entries < 4 * (num_cal_entries+1)) {
                /* Last iteration: ramp-up until final number of calendar entries */
                num_cal_entries = new_num_cal_entries;
            } else {
                /* Intermediate iteration */
                num_cal_entries = 4 * (num_cal_entries+1) - 1;
            }

            /* Resize Channelized FIFO Controller */
            ret_val = cfc_fifo_resize(mapotn_handle->cfc_prefetch_handle, chnl, num_cal_entries);
            if (ret_val != PMC_SUCCESS)
            {
                PMC_RETURN(ret_val);
            }

            /* Reconfigure prefetch fifo thresholds */
            mapotn_tgfpf_prefetch_threshold_get(mapotn_handle, chnl, num_cal_entries, &prefetch_thrld);
            mapotn_tgfpf_chnl_prefetch_thrld_cfg(mapotn_handle, chnl, prefetch_thrld);
        }
    } else {
        /* Ramp down */

        previous_num_cal_entries = mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries;

        /* Done in 1-3 steps as a workaround for an issue in the prefetch fifo block */
        while (num_cal_entries > new_num_cal_entries) {
            /* Ramp-down iteration */
            if (new_num_cal_entries > num_cal_entries / 4 /*- 1*/) {
                /* Last iteration: ramp-down until final number of calendar entries */
                num_cal_entries = new_num_cal_entries;
            } else {
                /* Intermediate iteration */
                num_cal_entries = num_cal_entries / 4 /*- 1*/ + 1;
            }

            /* Reconfigure prefetch fifo threshold */
            mapotn_tgfpf_prefetch_threshold_get(mapotn_handle, chnl, num_cal_entries, &prefetch_thrld);
            mapotn_tgfpf_chnl_prefetch_thrld_cfg(mapotn_handle, chnl, prefetch_thrld);     
      
            /*wait 100 us for prefetch fifo fill level to decrease*/
            PMC_Q_USLEEP(mapotn_handle,200);

            /* Resize Channelized FIFO Controller */
            ret_val = cfc_fifo_resize(mapotn_handle->cfc_prefetch_handle, chnl, num_cal_entries);
            if (ret_val != PMC_SUCCESS)
            {
                PMC_RETURN(ret_val);
            }

            previous_num_cal_entries = num_cal_entries;
        }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_prefetch_fifo_reprov */


/*******************************************************************************
* mapotn_tgfpf_chnl_packing_fifo_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the packing fifo  with a new number of calendar slots for the
*   specified channel. This function is used for G.HAO ODUflex resizing. This
*   function reprovisions the CFC fifo and updates the packing fifo low & high
*   thresholds.
*   The function calls for G.HAO should be in the following order:
*   Ramp-up:
*   1) mapotn_tgfpf_chnl_ohfs_insert_fifo_reprov (threshold, CFC fifo)
*   2) mapotn_tgfpf_chnl_packing_fifo_reprov (thresholds, CFC fifo)
*   2) mapotn_tgfpf_chnl_prefetch_fifo_reprov (threshold, CFC fifo)
*   Ramp-down:
*   1) mapotn_tgfpf_chnl_prefetch_fifo_reprov (CFC fifo, threshold)
*   1) mapotn_tgfpf_chnl_packing_fifo_reprov (CFC fifo, thresholds)
*   2) mapotn_tgfpf_chnl_ohfs_insert_fifo_reprov (CFC fifo, threshold)
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   new_num_cal_entries - number of calendar entries after resizing
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES: The wait delays defined here are a minimum delay. Extending these
*        delays would not cause any issue.
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_packing_fifo_reprov(mapotn_handle_t *mapotn_handle,
                                                       UINT32 chnl,
                                                       UINT32 new_num_cal_entries)
{
    /* variable declaration */
    UINT32 packing_low_thrld;
    UINT32 packing_high_thrld;
    UINT32 num_cal_entries;
    UINT32 ramp_down_ratio;
    UINT32 wait_delay_us;
    PMC_ERROR ret_val = PMC_SUCCESS;

    /* variable initialization */
    num_cal_entries     = new_num_cal_entries;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);


    if (num_cal_entries > mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries) {
        /* Ramp up */

        /* Resize Channelized FIFO Controller */
        if (mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries == 1) {
            /* For ramp-up from 1TS, the packing fifo high threshold needs to be set
               to 3 (instead of 4) during IPT end address change request in CFC fifo */
            /* Allocate IPT pages */
            ret_val = cfc_fifo_resize_one_ts_step1(mapotn_handle->cfc_packing_handle, chnl, num_cal_entries);
            if (ret_val != PMC_SUCCESS)
            {
                PMC_RETURN(ret_val);
            }

            /* Reconfigure packing fifo thresholds (high threshold is decreased by 1) */
            mapotn_tgfpf_packing_threshold_get(mapotn_handle, chnl, mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries, &packing_low_thrld, &packing_high_thrld);
            mapotn_tgfpf_chnl_packer_thrld_cfg(mapotn_handle, chnl, packing_low_thrld, packing_high_thrld-1);

            /* Send request to change the IPT end address */
            ret_val = cfc_fifo_resize_one_ts_step2(mapotn_handle->cfc_packing_handle, chnl, num_cal_entries, mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries);
            if (ret_val != PMC_SUCCESS)
            {
                PMC_RETURN(ret_val);
            }
        } else {
            /* Normal case */
            ret_val = cfc_fifo_resize(mapotn_handle->cfc_packing_handle, chnl, num_cal_entries);
            if (ret_val != PMC_SUCCESS)
            {
                PMC_RETURN(ret_val);
            }
        }

        /* Reconfigure packing fifo thresholds */
        mapotn_tgfpf_packing_threshold_get(mapotn_handle, chnl, num_cal_entries, &packing_low_thrld, &packing_high_thrld);
        mapotn_tgfpf_chnl_packer_thrld_cfg(mapotn_handle, chnl, packing_low_thrld, packing_high_thrld);
    } else {
        /* Ramp down */

        if (new_num_cal_entries == 1) {
            /* For ramp-up to 1TS, do in 2 steps: nTS->2TS, then 2TS->1TS */
            if (mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries > 2)
            {
                /* First step: Ramp down up to 2 TS */
                num_cal_entries = 2;

                /* Reconfigure packing fifo thresholds */
                mapotn_tgfpf_packing_threshold_get(mapotn_handle, chnl, num_cal_entries, &packing_low_thrld, &packing_high_thrld);
                mapotn_tgfpf_chnl_packer_thrld_cfg(mapotn_handle, chnl, packing_low_thrld, packing_high_thrld);

                /* Add some delay to let the fifo fill level decrease before resizing CFC fifo */
                ramp_down_ratio = mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries / new_num_cal_entries;
                wait_delay_us = (ramp_down_ratio * 3 / 2) + 1; /* Fifo with 4 locations/TS and expected 3/4 full*/
                PMC_Q_USLEEP(mapotn_handle,wait_delay_us);

                /* Resize Channelized FIFO Controller */
                ret_val = cfc_fifo_resize(mapotn_handle->cfc_packing_handle, chnl, num_cal_entries);
                if (ret_val != PMC_SUCCESS)
                {
                    PMC_RETURN(ret_val);
                }
                num_cal_entries = 1;
            }

            /* Second step: Ramp-down up to 1 TS */
            /* For ramp-down to 1TS, the packing fifo high threshold needs to be set
               to 3 (instead of 4) during IPT end address change request in CFC fifo */

            /* Reconfigure packing fifo thresholds (high threshold is decreased by 1) */
            mapotn_tgfpf_packing_threshold_get(mapotn_handle, chnl, num_cal_entries, &packing_low_thrld, &packing_high_thrld);
            mapotn_tgfpf_chnl_packer_thrld_cfg(mapotn_handle, chnl, packing_low_thrld, packing_high_thrld-1);

            /* Send request to change the IPT end address */
            ret_val = cfc_fifo_resize_one_ts_step1(mapotn_handle->cfc_packing_handle, chnl, num_cal_entries);
            if (ret_val != PMC_SUCCESS)
            {
                PMC_RETURN(ret_val);
            }

            /* Reconfigure packing fifo thresholds to the right value */
            mapotn_tgfpf_chnl_packer_thrld_cfg(mapotn_handle, chnl, packing_low_thrld, packing_high_thrld);

            /* Deallocate IPT pages */
            ret_val = cfc_fifo_resize_one_ts_step2(mapotn_handle->cfc_packing_handle, chnl, num_cal_entries, 2);
            if (ret_val != PMC_SUCCESS)
            {
                PMC_RETURN(ret_val);
            }
        } else {
            /* Normal case */
            /* Reconfigure packing fifo thresholds */
            mapotn_tgfpf_packing_threshold_get(mapotn_handle, chnl, num_cal_entries, &packing_low_thrld, &packing_high_thrld);
            mapotn_tgfpf_chnl_packer_thrld_cfg(mapotn_handle, chnl, packing_low_thrld, packing_high_thrld);

            /* Add some delay to let the fifo fill level decrease before resizing CFC fifo */
            ramp_down_ratio = mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries / num_cal_entries;
            wait_delay_us = (ramp_down_ratio * 3 / 2) + 1; /* Fifo with 4 locations/TS and expected 3/4 full*/
            PMC_Q_USLEEP(mapotn_handle,wait_delay_us);

            /* Resize Channelized FIFO Controller */
            ret_val = cfc_fifo_resize(mapotn_handle->cfc_packing_handle, chnl, num_cal_entries);
            if (ret_val != PMC_SUCCESS)
            {
                PMC_RETURN(ret_val);
            }
        }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_packing_fifo_reprov */


/*******************************************************************************
* mapotn_tgfpf_chnl_ohfs_insert_fifo_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision the packing fifo  with a new number of calendar slots for the
*   specified channel. This function is used for G.HAO ODUflex resizing. This
*   function reprovisions the CFC fifo and updates the packing fifo low & high
*   thresholds.
*   The function calls for G.HAO should be in the following order:
*   Ramp-up:
*   1) mapotn_tgfpf_chnl_ohfs_insert_fifo_reprov (threshold, CFC fifo)
*   2) mapotn_tgfpf_chnl_packing_fifo_reprov (thresholds, CFC fifo)
*   2) mapotn_tgfpf_chnl_prefetch_fifo_reprov (threshold, CFC fifo)
*   Ramp-down:
*   1) mapotn_tgfpf_chnl_prefetch_fifo_reprov (CFC fifo, threshold)
*   1) mapotn_tgfpf_chnl_packing_fifo_reprov (CFC fifo, thresholds)
*   2) mapotn_tgfpf_chnl_ohfs_insert_fifo_reprov (CFC fifo, threshold)
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   new_num_cal_entries - number of calendar entries after resizing
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES: The wait delays defined here are a minimum delay. Extending these
*        delays would not cause any issue.
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_chnl_ohfs_insert_fifo_reprov(mapotn_handle_t *mapotn_handle,
                                                           UINT32 chnl,
                                                           UINT32 new_num_cal_entries)
{
    /* variable declaration */
    util_global_mapping_mode_t mapping_mode;
    UINT32 num_cal_entries;
    UINT32 ramp_down_ratio;
    UINT32 wait_delay_us;
    PMC_ERROR ret_val= PMC_SUCCESS;

    /* variable initialization */
    num_cal_entries     = new_num_cal_entries;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    /* retrieve channel's status from memory */
    mapping_mode        = mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode;


    if (num_cal_entries > mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries) {
        /* Ramp up */

        /* Resize OHFS INSERT Channelized FIFO Controller */
        ret_val = ohfs_insert_chnl_cfc_reprov(mapotn_handle->ohfs_insert_handle, chnl, num_cal_entries);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }

        /* OHFS INSERT CFC fifo threshold */
        ret_val = ohfs_insert_chnl_thresh_reprov(mapotn_handle->ohfs_insert_handle, chnl,  mapping_mode, num_cal_entries);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
    } else {
        /* Ramp down */

        /* OHFS INSERT CFC fifo threshold */
        ret_val = ohfs_insert_chnl_thresh_reprov(mapotn_handle->ohfs_insert_handle, chnl,  mapping_mode, num_cal_entries);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }

        /* Add some delay to let the fifo fill level decrease before resizing CFC fifo */
        ramp_down_ratio = mapotn_handle->var.tx_chnl_ctxt[chnl].num_cal_entries / num_cal_entries;
        wait_delay_us = (ramp_down_ratio * num_cal_entries) + 1; /* Fifo with 4 locations/TS and expected half full, decreases by 1 location per calendar */
        PMC_Q_USLEEP(mapotn_handle,wait_delay_us);

        /* Resize OHFS INSERT Channelized FIFO Controller */
        ret_val = ohfs_insert_chnl_cfc_reprov(mapotn_handle->ohfs_insert_handle, chnl, num_cal_entries);
        if (ret_val != PMC_SUCCESS)
        {
            PMC_RETURN(ret_val);
        }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_chnl_ohfs_insert_fifo_reprov */


/*******************************************************************************
* mapotn_tgfpf_rcoh_ncs_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the NCS_EN and RCOH register fields for the
*   specified channel in the ohfs_insert. We assume BWR_IND=0.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   ncs                 - NCS value
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_rcoh_ncs_set(mapotn_handle_t *mapotn_handle,
                                           UINT32 chnl,
                                           UINT32 ncs)
{
    /* variable declaration */
	UINT32 rcoh;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    /* Set NCS (assume BWR_IND=0) */
    if(ncs)
    {
        rcoh = 0x00004000;
        ohfs_insert_rcoh_set(mapotn_handle->ohfs_insert_handle, chnl, rcoh);
        ohfs_insert_ncs_en_set(mapotn_handle->ohfs_insert_handle, chnl, ncs);
    }
    else
    {
        rcoh = 0x00000000;
        ohfs_insert_ncs_en_set(mapotn_handle->ohfs_insert_handle, chnl, ncs);
        ohfs_insert_rcoh_set(mapotn_handle->ohfs_insert_handle, chnl, rcoh);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_rcoh_ncs_set */


/*******************************************************************************
* mapotn_tgfpf_rcoh_bwr_ind_set
* ______________________________________________________________________________
*
* DESCRIPTION:
*   This function configures the RCOH register field with BWR_IND for the
*   specified channel in the ohfs_insert. We assume NCS=1.
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   bwr_ind             - BWR_IND value
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_tgfpf_rcoh_bwr_ind_set(mapotn_handle_t *mapotn_handle,
                                               UINT32 chnl,
                                               UINT32 bwr_ind)
{
    /* variable declaration */
	UINT32 rcoh;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_TGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    /* Set BWR_IND (assume NCS=1) */
    if(bwr_ind)
    {
        rcoh = 0x0080C000;
    }
    else
    {
        rcoh = 0x00004000;
    }

    ohfs_insert_rcoh_set(mapotn_handle->ohfs_insert_handle, chnl, rcoh);

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_tgfpf_rcoh_bwr_ind_set */


/*******************************************************************************
* mapotn_oduksc_scbs3_cgen_reprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Reprovision in ODUKSC the specified ODUFLEX channel for a specific rate in
*   G.HAO. No ramp is configured. The scbs3 is configured with the number of
*   calendar slots corresponding to the new rate. The credit generator is
*   configured so that the rate doesn't change despite the new number of
*   calendar slots.
*   In G.HAO ODUflex resizing, the functions are called in the following order:
*   Ramp-up:
*   1) mapotn_oduksc_scbs3_cgen_reprov
*   2) mapotn_oduksc_ramp_prov
*   3) mapotn_oduksc_ramp_start
*   4) mapotn_oduksc_ramp_done_isr (switch to 2nd part of the ramp)
*   5) mapotn_oduksc_ramp_done_isr (ramp done)
*   Ramp-down:
*   1) mapotn_oduksc_ramp_prov
*   2) mapotn_oduksc_ramp_start
*   3) mapotn_oduksc_ramp_done_isr (switch to 2nd part of the ramp)
*   4) mapotn_oduksc_ramp_done_isr (ramp done)
*   5) mapotn_oduksc_scbs3_cgen_reprov
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of tribslots
*                         1 to 8 -> ODU2, 1 to 32 -> ODU3, 1 to 80 -> ODU4.
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_oduksc_scbs3_cgen_reprov(mapotn_handle_t *mapotn_handle,
                                                 UINT32 chnl,
                                                 UINT32 oduflex_gfp_rate)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_LOG_TRACE( "Channel = %u, oduflex_gfp_rate = %u\n", chnl, oduflex_gfp_rate);

    if (mapotn_handle->var.tx_chnl_ctxt[chnl].state != MAPOTN_CHNL_OPERATIONAL)
    {
         PMC_RETURN(MAPOTN_CHNL_STATE_NOT_OPERATIONAL);
    }

    /*
       it is operational, energy state is correct
       check if this channel is valid candidate for reprovisioning
    */
    if (mapotn_handle->var.tx_chnl_ctxt[chnl].adaptation_func != UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP  ||
        mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type      != UTIL_GLOBAL_ODUFLEX_GFP ||
        mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode    != UTIL_GLOBAL_GFP ||
        mapotn_handle->var.tx_chnl_ctxt[chnl].src_dest        != MAPOTN_DEST_CPB )
    {
        PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
    }

    if(mapotn_handle->cfg.db_mode == MAPOTN_DB_MODE_LOCAL)
    {
        /* remove previous entries from the ODUKSC database */
        ret_val = mapotn_db_chnl_rec_rem(mapotn_handle, chnl, UTIL_GEN_DB_ODUKSC_DB);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
        /* add channel data to the ODUKSC database */
        ret_val = mapotn_db_chnl_rec_add(mapotn_handle, chnl, oduflex_gfp_rate , UTIL_GEN_DB_ODUKSC_DB);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    ret_val = oduksc_chnl_scbs3_cgen_reprov(
                  mapotn_handle->oduksc_handle,
                  chnl,
                  mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type,
                  oduflex_gfp_rate);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_oduksc_scbs3_cgen_reprov */


/*******************************************************************************
* mapotn_oduksc_ramp_prov
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Provision the ODUKSC ramp configuration for the specified ODUFLEX channel in
*   G.HAO. The ramp is not started yet. The scbs3 and the current rate are not
*   modified. The credit generator and ramp modulo counter are configured so
*   that the ramp is ready to be started. If the end_for_bwr_ind_250us is set,
*   two ramps will be used. The first one is configured at target rate +/-
*   128kb/s and is ready to be started. The second is programmed at target rate
*   in the oduksc standby memory, ready to be switched once the first ramp is
*   done.
*   In G.HAO ODUflex resizing, the functions are called in the following order:
*   Ramp-up:
*   1) mapotn_oduksc_scbs3_cgen_reprov
*   2) mapotn_oduksc_ramp_prov
*   3) mapotn_oduksc_ramp_start
*   4) mapotn_oduksc_ramp_done_isr (switch to 2nd part of the ramp)
*   5) mapotn_oduksc_ramp_done_isr (ramp done)
*   Ramp-down:
*   1) mapotn_oduksc_ramp_prov
*   2) mapotn_oduksc_ramp_start
*   3) mapotn_oduksc_ramp_done_isr (switch to 2nd part of the ramp)
*   4) mapotn_oduksc_ramp_done_isr (ramp done)
*   5) mapotn_oduksc_scbs3_cgen_reprov
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   oduflex_gfp_rate    - ODUflex GFP container rate in number of tribslots
*                         1 to 8 -> ODU2, 1 to 32 -> ODU3, 1 to 80 -> ODU4.
*   end_for_bwr_ind_250us - if set, the ramp will end 250us before the target
*                           rate is reached, allowing programming BWR_IND=0
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_oduksc_ramp_prov(mapotn_handle_t *mapotn_handle,
                                         UINT32 chnl,
                                         UINT32 oduflex_gfp_rate,
                                         BOOL end_for_bwr_ind_250us)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_LOG_TRACE( "Channel = %u, oduflex_gfp_rate = %u\n", chnl, oduflex_gfp_rate);

    if (mapotn_handle->var.tx_chnl_ctxt[chnl].state != MAPOTN_CHNL_OPERATIONAL)
    {
         PMC_RETURN(MAPOTN_CHNL_STATE_NOT_OPERATIONAL);
    }

    /*
       it is operational, energy state is correct
       check if this channel is valid candidate for reprovisioning
    */
    if (mapotn_handle->var.tx_chnl_ctxt[chnl].adaptation_func != UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP  ||
        mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type      != UTIL_GLOBAL_ODUFLEX_GFP ||
        mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode    != UTIL_GLOBAL_GFP ||
        mapotn_handle->var.tx_chnl_ctxt[chnl].src_dest        != MAPOTN_DEST_CPB )
    {
        PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
    }

    if (end_for_bwr_ind_250us)
    {
        /* Provision first ramp that will end 250us before target rate */
        ret_val = oduksc_chnl_cgen_ramp_prov(
                      mapotn_handle->oduksc_handle,
                      chnl,
                      mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type,
                      oduflex_gfp_rate,
                      TRUE);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }

        mapotn_handle->var.tx_chnl_ctxt[chnl].doing_ghao_ramp_until_bwr_ind = TRUE;
    } else {
        /* Provision ramp at target rate */
        ret_val = oduksc_chnl_cgen_ramp_prov(
                      mapotn_handle->oduksc_handle,
                      chnl,
                      mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type,
                      oduflex_gfp_rate,
                      FALSE);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_oduksc_ramp_prov */


/*******************************************************************************
* mapotn_oduksc_ramp_start
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Start the ODUKSC ramp for the resizing of an ODUFLEX channel in G.HAO. The
*   ramp modulo counter is started and the credit generator will start using it.
*   When the ramp-up or ramp-down will be completed, the ramp done interrupt
*   will be generated.
*   In G.HAO ODUflex resizing, the functions are called in the following order:
*   Ramp-up:
*   1) mapotn_oduksc_scbs3_cgen_reprov
*   2) mapotn_oduksc_ramp_prov
*   3) mapotn_oduksc_ramp_start
*   4) mapotn_oduksc_ramp_done_isr (switch to 2nd part of the ramp)
*   5) mapotn_oduksc_ramp_done_isr (ramp done)
*   Ramp-down:
*   1) mapotn_oduksc_ramp_prov
*   2) mapotn_oduksc_ramp_start
*   3) mapotn_oduksc_ramp_done_isr (switch to 2nd part of the ramp)
*   4) mapotn_oduksc_ramp_done_isr (ramp done)
*   5) mapotn_oduksc_scbs3_cgen_reprov
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   set_bwr_ind         - if set, set the BWR_IND bit in RCOH register, then
*                         wait 125us, then start the ramp
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_oduksc_ramp_start(mapotn_handle_t *mapotn_handle,
                                          UINT32 chnl,
                                          BOOL set_bwr_ind)
{
    /* Variable declaration */
    UINT32 wait_delay;
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_LOG_TRACE("Start mapotn_oduksc_ramp_start() for channel %d", chnl);

    if (mapotn_handle->var.tx_chnl_ctxt[chnl].state != MAPOTN_CHNL_OPERATIONAL)
    {
         PMC_RETURN(MAPOTN_CHNL_STATE_NOT_OPERATIONAL);
    }

    /*
       it is operational, energy state is correct
       check if this channel is valid candidate for reprovisioning
    */
    if (mapotn_handle->var.tx_chnl_ctxt[chnl].adaptation_func != UTIL_GLOBAL_MAP_ODUKP_PACKET_GFP  ||
        mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type      != UTIL_GLOBAL_ODUFLEX_GFP ||
        mapotn_handle->var.tx_chnl_ctxt[chnl].mapping_mode    != UTIL_GLOBAL_GFP ||
        mapotn_handle->var.tx_chnl_ctxt[chnl].src_dest        != MAPOTN_DEST_CPB )
    {
        PMC_RETURN(MAPOTN_ERR_INVALID_PARAMETERS);
    }

    if (set_bwr_ind)
    {
        /* Set BWR_IND */
        PMC_LOG_TRACE("Set BWR_IND for channel %d", chnl);
        mapotn_tgfpf_rcoh_bwr_ind_set(mapotn_handle, chnl, 1);

        /* Get the wait delay between BWR_IND=1 and ramp start */
        mapotn_bwr_ind_ramp_start_delay_get(mapotn_handle, chnl, &wait_delay);
        PMC_LOG_TRACE("Wait overhead propagation delay + 125us after BWR_IND for channel %d", chnl);
        PMC_Q_USLEEP(mapotn_handle,wait_delay);

        /* Start ramp */
        PMC_LOG_TRACE("Start ODUKSC ramp for channel %d", chnl);
        ret_val = oduksc_chnl_cgen_ramp_start(
                      mapotn_handle->oduksc_handle,
                      chnl,
                      mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }
    else
    {
        /* Start ramp */
        PMC_LOG_TRACE("Start ODUKSC ramp for channel %d", chnl);
        ret_val = oduksc_chnl_cgen_ramp_start(
                      mapotn_handle->oduksc_handle,
                      chnl,
                      mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_oduksc_ramp_start */


/*******************************************************************************
*  mapotn_oduksc_ramp_poll_end_deprov
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Call the ramp done ISR of the oduksc.
*   In G.HAO ODUflex resizing, the functions are called in the following order:
*   Ramp-up:
*   1) mapotn_oduksc_scbs3_cgen_reprov
*   2) mapotn_oduksc_ramp_prov
*   3) mapotn_oduksc_ramp_start
*   4) mapotn_oduksc_ramp_done_isr (switch to 2nd part of the ramp)
*   5) mapotn_oduksc_ramp_done_isr (ramp done)
*   Ramp-down:
*   1) mapotn_oduksc_ramp_prov
*   2) mapotn_oduksc_ramp_start
*   3) mapotn_oduksc_ramp_done_isr (switch to 2nd part of the ramp)
*   4) mapotn_oduksc_ramp_done_isr (ramp done)
*   5) mapotn_oduksc_scbs3_cgen_reprov
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*   clear_bwr_ind       - if set, clear the BWR_IND bit in RCOH register
*                         once the first part of the ramp is done
*   poll_delay_us       - ramp-done polling delay in microseconds
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_oduksc_ramp_poll_end_deprov(mapotn_handle_t *mapotn_handle,
                                                    UINT32 chnl,
                                                    BOOL clear_bwr_ind,
                                                    UINT32 poll_delay_us)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    PMC_LOG_TRACE("Start mapotn_oduksc_ramp_poll_end_deprov() for channel %d", chnl);

    /* Poll ODUKSC ramp done interrupt */
    PMC_LOG_TRACE("Poll RAMP_DONE for channel %d", chnl);
    ret_val = oduksc_chnl_ramp_done_poll(
                  mapotn_handle->oduksc_handle,
                  chnl,
                  poll_delay_us);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    if (mapotn_handle->var.tx_chnl_ctxt[chnl].doing_ghao_ramp_until_bwr_ind == TRUE)
    {
        /* Provision second part of the ramp that will end at target rate */
        PMC_LOG_TRACE("Provision and start the second part of the ramp for channel %d", chnl);
        ret_val = oduksc_chnl_cgen_ramp_end_prov(
                      mapotn_handle->oduksc_handle,
                      chnl,
                      mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }

        if (clear_bwr_ind)
        {
            /* Clear BWR_IND */
            PMC_LOG_TRACE("Clear BWR_IND for channel %d", chnl);
            mapotn_tgfpf_rcoh_bwr_ind_set(mapotn_handle, chnl, 0);

            /* Poll ODUKSC ramp done interrupt */
            PMC_LOG_TRACE("Poll RAMP_DONE for channel %d", chnl);
            ret_val = oduksc_chnl_ramp_done_poll(
                          mapotn_handle->oduksc_handle,
                          chnl,
                          poll_delay_us);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN(ret_val);
            }

            /* Ramp done interrupt routine */
            PMC_LOG_TRACE("RAMP_DONE interrupt routine for channel %d", chnl);
            ret_val = oduksc_ramp_done_isr(mapotn_handle->oduksc_handle, chnl);
            if (ret_val != PMC_SUCCESS) {
                PMC_RETURN(ret_val);
            }
        }
        else
        {
            /* Expect this function will be called a second time for polling
              the end of the ramp */
        }
    }
    else
    {
        if (clear_bwr_ind)
        {
            PMC_LOG_TRACE("Clear BWR_IND for channel %d", chnl);
            mapotn_tgfpf_rcoh_bwr_ind_set(mapotn_handle, chnl, 0);
        }

        /* Ramp done interrupt routine */
        PMC_LOG_TRACE("RAMP_DONE interrupt routine for channel %d", chnl);
        ret_val = oduksc_ramp_done_isr(mapotn_handle->oduksc_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_oduksc_ramp_poll_end_deprov */


/*******************************************************************************
*  mapotn_oduksc_ramp_done_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Call the ramp done ISR of the oduksc.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *ramp_done          - pointer to the ramp_done status
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_oduksc_ramp_done_get(mapotn_handle_t *mapotn_handle,
                                             UINT32 chnl,
                                             UINT32 *ramp_done)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    ret_val = oduksc_chnl_ramp_done_get(
                  mapotn_handle->oduksc_handle,
                  chnl,
                  ramp_done);
    if (ret_val != PMC_SUCCESS) {
        PMC_RETURN(ret_val);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_oduksc_ramp_done_get */


/*******************************************************************************
*  mapotn_oduksc_ramp_done_isr
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Call the ramp done ISR of the oduksc. If BWR_IND needs to be cleared,
*  provision the second part of the ramp on oduksc instead.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_oduksc_ramp_done_isr(mapotn_handle_t *mapotn_handle,
                                             UINT32    chnl)
{
    /* Variable declaration */
    PMC_ERROR ret_val = PMC_SUCCESS;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    if (mapotn_handle->var.tx_chnl_ctxt[chnl].doing_ghao_ramp_until_bwr_ind == TRUE)
    {
        /* Provision second part of the ramp that will end at target rate */
        PMC_LOG_TRACE("Provision and start the second part of the ramp for channel %d", chnl);
        ret_val = oduksc_chnl_cgen_ramp_end_prov(
                      mapotn_handle->oduksc_handle,
                      chnl,
                      mapotn_handle->var.tx_chnl_ctxt[chnl].odukp_type);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }

        /* Now in the second part of the ramp */
        mapotn_handle->var.tx_chnl_ctxt[chnl].doing_ghao_ramp_until_bwr_ind = FALSE;
    }
    else
    {
        /* Ramp done */
        PMC_LOG_TRACE("Ramp interrupt routine for channel %d", chnl);
        ret_val = oduksc_ramp_done_isr(mapotn_handle->oduksc_handle, chnl);
        if (ret_val != PMC_SUCCESS) {
            PMC_RETURN(ret_val);
        }
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_oduksc_ramp_done_isr */


/*******************************************************************************
*  mapotn_bwr_ind_ramp_start_delay_get
* ______________________________________________________________________________
*
* DESCRIPTION:
*  Get the wait delay required between BWR_IND assertion and ODUKSC ramp start.
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance to be operated on
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95
*
* OUTPUTS:
*   *bwr_ind_ramp_start_delay_us - pointer to the wait delay in microseconds
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES:
*
*******************************************************************************/
PUBLIC PMC_ERROR mapotn_bwr_ind_ramp_start_delay_get(mapotn_handle_t *mapotn_handle,
                                                     UINT32 chnl,
                                                     UINT32 *bwr_ind_ramp_start_delay_us)
{
    /* Variable declaration */
	UINT32 wait_delay;

    PMC_ENTRY();

    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_ERR_INVALID_PARAMETERS, 0, 0);

    /* Wait for RCOH propagation in ODU frame (1.25us for 80 TS, 100us for 1 TS) */
    wait_delay = 1.24 * 80 / mapotn_handle->var.tx_chnl_ctxt[chnl].ghao_start_oduflex_gfp_rate + 1;
    /* Wait 125 us */
    wait_delay += 125;
    *bwr_ind_ramp_start_delay_us = wait_delay;

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_bwr_ind_ramp_start_delay_get */




/*******************************************************************************
*  mapotn_rgfpf_unpack_fifo_thrld_change
* ______________________________________________________________________________
*
* DESCRIPTION:
*   Change the unpack fifo XOFF threshold to the normal value or normal value
*   minus one.  
*
*
* INPUTS:
*   *mapotn_handle      - pointer to MAPOTN handle instance
*   chnl                - channel ID to be configured
*                         Valid range: 0 - 95                    
*   num_cal_entries     - number of calendar entries
*   thrld_minus_one     - FALSE: Normal threshold value
*                       - TRUE: Normal threshold value minus one
*
*
* OUTPUTS:
*   None.
*
* RETURNS:
*    PMC_SUCCESS - on success, error otherwise.
*
* NOTES: When fill level crosses this value then depending on S_OLD status 
*        received for the given channel Xoff indication is given or small GFP  
*        frames (defined by configured thresholds) are dropped
*
*******************************************************************************/
PRIVATE PMC_ERROR mapotn_rgfpf_unpack_fifo_thrld_change(mapotn_handle_t *mapotn_handle,
                                                        UINT32 chnl,
                                                        UINT32 num_cal_entries,
                                                        BOOL8 thrld_minus_one)
{
    /* variable declaration */
    UINT32 unpack_thrld;
    
    PMC_ENTRY();
    
    PMC_ASSERT(mapotn_handle != NULL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(chnl < MAPOTN_NUM_CHANL, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);
    PMC_ASSERT(num_cal_entries >= 1, MAPOTN_RGFPF_ERR_INVALID_PARAMETERS, 0, 0);

    if (pmc_mpmo_mpma_reg_revision(&mapotn_handle->base) == TRUE)
    {
        /* REVA: Threshold =  num_cal_entries */
        unpack_thrld = num_cal_entries;
        if (thrld_minus_one)
        {
            unpack_thrld -= 1;
        }
        mapotn_rgfpf_field_UNPACK_XOFF_THRLD_set(NULL, mapotn_handle, chnl, unpack_thrld);
    }
    else
    {
        /* REVB: Threshold =  3 * num_cal_entries */
        unpack_thrld = num_cal_entries * 3;
        if (thrld_minus_one)
        {
            unpack_thrld -= 1;
        }
        mapotn_rgfpf_field_UNPACK_XOFF_THRLD2_set(NULL, mapotn_handle, chnl, unpack_thrld);
    }

    PMC_RETURN(PMC_SUCCESS);

} /* mapotn_rgfpf_unpack_fifo_thrld_change */


/*
** End of file
*/

